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


def run_objcopy(*args: str, input_file: Path) -> subprocess.CompletedProcess:
    cmd = [str(OBJCOPY), *args, str(input_file)]
    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode != 0:
        print(f"WARNING: objcopy failed for {input_file}: {result.stderr.strip()}")
    return result


def globalize_symbols(obj: Path, symbols: list[str]) -> None:
    """Make local symbols global so the linker can resolve them."""
    sym_file = Path("/tmp/globalize_syms.txt")
    sym_file.write_text("\n".join(symbols) + "\n")
    run_objcopy(f"--globalize-symbols={sym_file}", input_file=obj)


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


def batch_transform(obj: Path, **kwargs) -> None:
    """Apply multiple transformations to an object in a single objcopy call.
    
    Keyword args:
        localize: list[str] — symbols to localize
        globalize: list[str] — symbols to globalize
        rename: dict[str, str] — old->new symbol mappings
    """
    args = []
    for sym in kwargs.get("localize", []):
        args.append(f"--localize-symbol={sym}")
    for sym in kwargs.get("globalize", []):
        args.append(f"--globalize-symbol={sym}")
    for old, new in kwargs.get("rename", {}).items():
        args.append(f"--redefine-sym={old}={new}")
    if args:
        run_objcopy(*args, input_file=obj)


def rename_symbol(obj: Path, old: str, new: str) -> None:
    """Rename a symbol in an object file."""
    run_objcopy(f"--redefine-sym={old}={new}", input_file=obj)


# ---------------------------------------------------------------------------
# 1. Rename + localize multiply-defined symbols in retail data objects.
#    MWLD (Wii/1.1) has a bug where multiple objcopy invocations on the
#    same file cause ELF_gen.c:2802 internal error, so all transforms per
#    file are done in a single call.
# ---------------------------------------------------------------------------

# nw4r_data.o — remove symbol now provided by matched ut_FileStream.o
strip_symbols(OBJ_DIR / "nw4r_data.o", ["lbl_eu_80665548"])

# monolibdata1.o — rename vtable label + strip matched symbols
# (strip removes them entirely; they're now provided by source objects)
strip_symbols(
    OBJ_DIR / "monolibdata1.o",
    [
        "getInstance__Q22ml6MTRandFv",
        "lbl_eu_80665580",
        "lbl_eu_80665588",
        "__dt__9CProcRootFv",
        "lbl_eu_806655A0",
        "lbl_eu_806655A8",
        "sRsrcPointerList__5CRsrc",
        "wkStandbyLogin__9CProcRootFv",
        "wkStandbyLogout__9CProcRootFv",
        "Tail__8CProcessFv",
        "__dt__8CProcessFv",
        "sFreeProcessList__11CProcessMan",
        "sRootProcessList__11CProcessMan",
    ],
)
# Separate rename (objcopy can't combine --strip-symbol with --redefine-sym)
batch_transform(
    OBJ_DIR / "monolibdata1.o",
    rename={"lbl_eu_8056BC80": "__vt__11CDeviceBase"},
)

# monolibdata2.o — strip symbols now provided by matched CDeviceGX.o
strip_symbols(
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
# The decompiled CProcess.o (MatchingFor US) references these CProcess
# member functions but they were placed in CTaskGame.o by the original
# compiler with LOCAL visibility. Globalize them so the linker resolves
# the references.
globalize_symbols(
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
# 5. Additional localize / globalize rules discovered during linking.
#    Extend as needed when new multiply-defined or undefined errors appear.
#    Each entry should eventually be solved at the source/splits level.
# ---------------------------------------------------------------------------
# 6. Inject vtable symbols from CDevice_vt.o into CDevice.o.
#    CDevice_vt.o is a retail object containing the CDeviceBase vtable that
#    CDeviceGX.o needs, but it is not tracked in the project config and
#    therefore not listed in the link rspfile. We use partial linking to
#    merge its symbols into CDevice.o so the linker can resolve them.
# ---------------------------------------------------------------------------

# CDevice_vt.o handling disabled: the vtable symbols it provides are already
# renamed in monolibdata1.o (see step 1). CDevice_vt.o has empty sections
# and linking it causes mwld internal errors.
# _cdevice_vt = OBJ_DIR / ...
# (disabled)

# ---------------------------------------------------------------------------
# 7. Redirect CLibVM.o's reference to the generic label used by .bak
#    monolibdata2.o (MWLD ELF_gen.c:2802 crash with spInstance__6CLibVM
#    when other symbols in the same object are localized).
# ---------------------------------------------------------------------------

_clibvm_retail = OBJ_DIR / "monolib" / "src" / "lib" / "CLibVM.o"
if _clibvm_retail.exists():
    rename_symbol(
        _clibvm_retail,
        "spInstance__6CLibVM",
        "lbl_eu_80665720",
    )

# ---------------------------------------------------------------------------
if __name__ == "__main__":
    print("postprocess_link_fixes: applied symbol transformations")
