"""``python -m tools.ppc_equivalence`` entrypoint (delegates to CLI)."""

from __future__ import annotations

from tools.ppc_equivalence.cli import main

if __name__ == "__main__":
    raise SystemExit(main())
