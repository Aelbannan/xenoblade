#!/usr/bin/env python3
"""Backward-compatible wrapper — use tools/postprocess_reloc_names.py. """

from __future__ import annotations

import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT))

from tools.postprocess_reloc_names import postprocess_object  # noqa: E402


def main(argv: list[str]) -> int:
    path = Path(argv[1]) if len(argv) > 1 else ROOT / "build/us/src/monolib/src/math/MTRand.o"
    if not path.is_file():
        print(f"missing object: {path}", file=sys.stderr)
        return 1
    if postprocess_object(path):
        print(f"post-processed {path}")
        return 0
    print(f"no post-process changes for {path}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))
