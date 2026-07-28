#!/usr/bin/env python3
"""Post-compile symbol fixes for the co-op fork link.

Applies objcopy transformations to resolve multiply-defined symbols
that prevent MWLD from linking main.dol.

Symbols are stripped from retail data objects when they conflict with
matched source objects that now provide the same definitions.

Usage (after mwcc compilation, before link):
    python3 tools/postprocess_link_fixes.py
"""

import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
OBJCOPY = ROOT / "build" / "binutils" / "powerpc-eabi-objcopy"

BUILD_US = ROOT / "build" / "us"
OBJ_DIR = BUILD_US / "obj"


def run_objcopy(*args: str, input_file: Path) -> subprocess.CompletedProcess:
    cmd = [str(OBJCOPY), *args, str(input_file)]
    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode != 0:
        print(f"WARNING: objcopy failed for {input_file}: {result.stderr.strip()}")
    return result


def strip_symbols(obj: Path, symbols: list[str]) -> None:
    """Strip (remove) symbols from an object file.
    These symbols are now provided by matched source objects, so they
    should not exist in the retail data objects at all.
    """
    if not symbols:
        return
    args = []
    for sym in symbols:
        args.append(f"--strip-symbol={sym}")
    run_objcopy(*args, input_file=obj)


# ---------------------------------------------------------------------------
# Strip multiply-defined symbols from retail data objects.
# Each symbol here is defined in both a retail data object and a matched
# source object; stripping the retail copy prevents multiply-defined errors.
# ---------------------------------------------------------------------------

strip_symbols(
    OBJ_DIR / "monolibdata1.o",
    [
        "lbl_eu_80665580",
        "lbl_eu_80665588",
        "sFreeProcessList__11CProcessMan",
        "sRootProcessList__11CProcessMan",
    ],
)

# ---------------------------------------------------------------------------
if __name__ == "__main__":
    print("postprocess_link_fixes: applied symbol transformations")
