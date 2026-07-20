#!/usr/bin/env python3
"""Post-compile symbol fixes for the co-op fork link.

Applies objcopy transformations to resolve multiply-defined symbols,
missing vtable labels, and local-vs-global visibility mismatches that
prevent MWCC from linking main.dol.

This is a **temporary** companion to postprocess_reloc_names.py.
Once the underlying splits / source / symbols are corrected, each
rule here should be removed.

Usage (after mwcc compilation, before link):
    python3 tools/postprocess_link_fixes.py
"""

import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
OBJCOPY = ROOT / "build" / "binutils" / "powerpc-eabi-objcopy"

BUILD_US = ROOT / "build" / "us"
OBJ_DIR = BUILD_US / "obj"
SRC_DIR = BUILD_US / "src"


def run_objcopy(*args: str, input_file: Path) -> None:
    cmd = [str(OBJCOPY), *args, str(input_file)]
    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode != 0:
        print(f"WARNING: objcopy failed for {input_file}: {result.stderr.strip()}")


def globalize_symbols(obj: Path, symbols: list[str]) -> None:
    """Make local symbols global so the linker can resolve them."""
    sym_file = Path("/tmp/globalize_syms.txt")
    sym_file.write_text("\n".join(symbols) + "\n")
    run_objcopy(f"--globalize-symbols={sym_file}", input_file=obj)


def localize_symbols(obj: Path, symbols: list[str]) -> None:
    """Localize symbols to avoid multiply-defined errors."""
    for sym in symbols:
        run_objcopy(f"--localize-symbol={sym}", input_file=obj)


def rename_symbol(obj: Path, old: str, new: str) -> None:
    """Rename a symbol in an object file."""
    run_objcopy(f"--redefine-sym={old}={new}", input_file=obj)


# ---------------------------------------------------------------------------
# 1. Rename missing vtable / RTTI labels in retail data objects
# ---------------------------------------------------------------------------

# CDeviceBase vtable lives in monolibdata1.o under the generic label
rename_symbol(
    OBJ_DIR / "monolibdata1.o",
    "lbl_eu_8056BC80",
    "__vt__11CDeviceBase",
)

# ---------------------------------------------------------------------------
# 2. Localize multiply-defined symbols in retail data assembly objects
#    (symbols that are now provided by compiled C++ Matching objects)
# ---------------------------------------------------------------------------

# nw4r_data.o
localize_symbols(OBJ_DIR / "nw4r_data.o", ["lbl_eu_80665548"])

# monolibdata1.o
localize_symbols(
    OBJ_DIR / "monolibdata1.o",
    [
        "getInstance__Q22ml6MTRandFv",
        "lbl_eu_80665580",
        "lbl_eu_80665588",
        "__dt__9CProcRootFv",
        "lbl_eu_806655A0",
        "wkStandbyLogin__9CProcRootFv",
        "wkStandbyLogout__9CProcRootFv",
        "Tail__8CProcessFv",
        "__dt__8CProcessFv",
        "sFreeProcessList__11CProcessMan",
        "sRootProcessList__11CProcessMan",
    ],
)

# monolibdata2.o
localize_symbols(
    OBJ_DIR / "monolibdata2.o",
    [
        "__dt__9CDeviceGXFv",
        "cacheInstance__9CDeviceGX",
        "lbl_eu_80663750",
        "lbl_eu_80663754",
        "lbl_eu_80665698",
        "lbl_eu_806656A0",
        "viAfterDrawDone__9CDeviceGXFv",
        "viBeginFrame__9CDeviceGXFv",
        "wkStandbyLogin__9CDeviceGXFv",
        "wkStandbyLogout__9CDeviceGXFv",
    ],
)

# CTaskGame.o (contains CProcess functions from retail split overlap)
localize_symbols(
    OBJ_DIR / "kyoshin" / "CTaskGame.o",
    [
        "Regist__8CProcessFP8CProcessb",
        "Tail__8CProcessFv",
        "__ct__8CProcessFv",
        "__dt__8CProcessFv",
    ],
)

# ---------------------------------------------------------------------------
# 3. Globalize anonymous-namespace functions that must be visible to the
#    linker (referenced by retail data vtables).
# ---------------------------------------------------------------------------

# CTaskManager.o – CRootProc methods
_ctaskmanager = SRC_DIR / "monolib" / "src" / "core" / "CTaskManager.o"
if _ctaskmanager.exists():
    globalize_symbols(
        _ctaskmanager,
        [
            "Init__Q226@unnamed@CTaskManager_cpp@9CRootProcFv",
            "Term__Q226@unnamed@CTaskManager_cpp@9CRootProcFv",
            "Move__Q226@unnamed@CTaskManager_cpp@9CRootProcFv",
            "Draw__Q226@unnamed@CTaskManager_cpp@9CRootProcFv",
        ],
    )

# ---------------------------------------------------------------------------
# 4. Globalize __sinit_ guard symbols referenced by FORCEACTIVE in ldscript.lcf
# ---------------------------------------------------------------------------

_sinit_targets = {
    SRC_DIR / "monolib" / "src" / "math" / "CRect16.o": [r"__sinit_\CRect16_cpp"],
    SRC_DIR / "monolib" / "src" / "work" / "CProcess.o": [r"__sinit_\CProcess_cpp"],
}

for obj_path, symbols in _sinit_targets.items():
    if obj_path.exists():
        globalize_symbols(obj_path, symbols)

# ---------------------------------------------------------------------------
# 4b. MWCC C++ linkage: __unregister_fragment is defined with unsigned int
#     in Gecko_ExceptionPPC.cp while NMWException.h declares extern "C" int;
#     the definition gets a mangled export (__unregister_fragment__FUi) that
#     __init_cpp_exceptions.o cannot resolve.
# ---------------------------------------------------------------------------

_gecko_exception = SRC_DIR / "PowerPC_EABI_Support" / "src" / "Runtime" / "Gecko_ExceptionPPC.o"
if _gecko_exception.exists():
    rename_symbol(
        _gecko_exception,
        "__unregister_fragment__FUi",
        "__unregister_fragment",
    )

# ---------------------------------------------------------------------------
# 5. Additional localize / globalize rules discovered during linking.
#    Extend as needed when new multiply-defined or undefined errors appear.
#    Each entry should eventually be solved at the source/splits level.
# ---------------------------------------------------------------------------

if __name__ == "__main__":
    print("postprocess_link_fixes: applied symbol transformations")
