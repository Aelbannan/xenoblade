from __future__ import annotations

import json
import shutil
import tempfile
import unittest
from pathlib import Path

from tools.decomp_atlas.lib.indexer import run_index
from tools.decomp_atlas.lib.server import AtlasAPI
from tools.decomp_atlas.tests import PROJECT


class ServerApiTests(unittest.TestCase):
    def setUp(self) -> None:
        self.tmp = tempfile.TemporaryDirectory()
        self.root = Path(self.tmp.name) / "project"
        shutil.copytree(PROJECT, self.root)
        result = run_index(self.root, vectors=True, config_path=self.root / "coop.json")
        self.assertTrue(result.get("ok"), result)
        self.api = AtlasAPI(self.root, enable_jobs=False, config_path=self.root / "coop.json")

    def tearDown(self) -> None:
        self.tmp.cleanup()

    def test_project_and_functions(self) -> None:
        project = self.api.project_info()
        self.assertEqual(project["region"], "us")
        self.assertFalse(project["jobs_enabled"])
        listing = self.api.list_functions({"page": 1, "page_size": 10, "readiness": "safe_leaf"})
        self.assertGreaterEqual(listing["total"], 1)
        self.assertTrue(all(item["readiness"] == "safe_leaf" for item in listing["items"]))
        leaf = self.api.get_function("demo-leaf")
        self.assertEqual(leaf["display_name"], "Demo::leaf")
        self.assertEqual(leaf["symbol"], "leaf__4DemoFv")

    def test_edges_units_projection(self) -> None:
        callees = self.api.get_edges("demo-callees-ready", direction="callees")
        self.assertTrue(any(item["id"] == "demo-accepted" for item in callees["items"]))
        callers = self.api.get_edges("demo-accepted", direction="callers")
        self.assertTrue(any(item["id"] == "demo-callees-ready" for item in callers["items"]))
        units = self.api.get_units()
        self.assertTrue(any(u["unit"] == "demo/Demo" for u in units["items"]))
        projection = self.api.get_projection()
        self.assertGreaterEqual(len(projection["items"]), 1)

    def test_attempts_experiments(self) -> None:
        attempts = self.api.get_attempts("demo-leaf")
        self.assertGreaterEqual(len(attempts["items"]), 1)
        experiments = self.api.get_experiments("demo-leaf")
        self.assertEqual(len(experiments["items"]), 2)

    def test_stdlib_handler_routes(self) -> None:
        from tools.decomp_atlas.lib.server import make_stdlib_handler

        handler_cls = make_stdlib_handler(self.api)
        status, body, ctype = handler_cls._route_get(  # type: ignore[attr-defined]
            object.__new__(handler_cls), "/api/project", {}
        )
        self.assertEqual(status, 200)
        self.assertIn("application/json", ctype)
        payload = json.loads(body.decode("utf-8"))
        self.assertEqual(payload["region"], "us")


if __name__ == "__main__":
    unittest.main()
