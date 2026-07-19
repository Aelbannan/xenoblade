from __future__ import annotations

import unittest

from tools.decomp_atlas.lib.scoring import classify_readiness, difficulty_score


class ScoringTests(unittest.TestCase):
    def test_readiness_buckets(self) -> None:
        certified = {"demo-accepted"}
        self.assertEqual(
            classify_readiness(
                {
                    "status": "FULL_MATCH",
                    "workflow_status": "ACCEPTED",
                    "buildable": True,
                },
                certified,
            ),
            "accepted",
        )
        self.assertEqual(
            classify_readiness(
                {
                    "status": "NOT_STARTED",
                    "workflow_status": "BACKLOG",
                    "buildable": True,
                    "callgraph_status": "complete",
                    "called_functions": [],
                    "unresolved_called_functions": [],
                    "has_indirect_calls": False,
                },
                certified,
            ),
            "safe_leaf",
        )
        self.assertEqual(
            classify_readiness(
                {
                    "status": "COMPILES",
                    "workflow_status": "BACKLOG",
                    "buildable": True,
                    "callgraph_status": "complete",
                    "called_functions": ["demo-accepted"],
                    "unresolved_called_functions": [],
                    "has_indirect_calls": False,
                },
                certified,
            ),
            "callees_accepted",
        )
        self.assertEqual(
            classify_readiness(
                {
                    "status": "NOT_STARTED",
                    "workflow_status": "BACKLOG",
                    "buildable": True,
                    "unresolved_called_functions": ["x"],
                    "has_indirect_calls": False,
                },
                certified,
            ),
            "blocked_unresolved",
        )
        self.assertEqual(
            classify_readiness(
                {
                    "status": "NOT_STARTED",
                    "workflow_status": "BACKLOG",
                    "buildable": True,
                    "unresolved_called_functions": [],
                    "has_indirect_calls": True,
                },
                certified,
            ),
            "blocked_indirect",
        )
        self.assertEqual(
            classify_readiness(
                {
                    "status": "NOT_STARTED",
                    "workflow_status": "CLAIMED",
                    "buildable": True,
                    "owner": "agent",
                },
                certified,
            ),
            "claimed",
        )
        self.assertEqual(
            classify_readiness(
                {"status": "NOT_STARTED", "workflow_status": "BACKLOG", "buildable": False},
                certified,
            ),
            "not_buildable",
        )
        self.assertEqual(
            classify_readiness(
                {
                    "status": "STRUCTURAL",
                    "workflow_status": "ACTIVE",
                    "buildable": True,
                    "callgraph_status": "incomplete",
                    "called_functions": ["a"],
                    "unresolved_called_functions": [],
                    "has_indirect_calls": False,
                },
                certified,
            ),
            "active_unclaimed",
        )

    def test_difficulty_bounds(self) -> None:
        score = difficulty_score(
            size=256,
            instruction_count=64,
            branch_count=8,
            relocation_count=4,
            stack_frame=48,
            direct_call_count=3,
            unresolved_call_count=1,
            has_indirect_calls=True,
            instruction_percent=50.0,
            attempt_count=5,
            non_improving_attempts=3,
        )
        self.assertGreaterEqual(score, 0.0)
        self.assertLessEqual(score, 100.0)
        self.assertGreater(score, difficulty_score(size=8, instruction_count=2))


if __name__ == "__main__":
    unittest.main()
