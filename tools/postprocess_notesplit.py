#!/usr/bin/env python3
"""
Post-process MWCC objects: copy .note.split section from retail object to
decompiled object.

The .note.split section is a decomp-toolkit (dtk) artifact added to retail
objects during split extraction. Decompiled objects compiled directly from
source lack this section. objdiff reports data: 0.0% for units where
.note.split is their only non-text alloc section.

Usage:
  python3 tools/postprocess_notesplit.py <decomp-object.o> [--retail <retail.o>]

The retail path is derived automatically by replacing build/us/src/ with
build/us/obj/ if --retail is not given. No-op when the decomp object already
has .note.split or the retail counterpart is missing.
"""

from __future__ import annotations

import argparse
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
OBJCOPY = ROOT / "build/binutils/powerpc-eabi-objcopy"


def has_section(path: Path, name: str) -> bool:
    """Check if an ELF object has a section with the given name."""
    data = path.read_bytes()

    # Parse ELF header
    e_shoff = struct.unpack(">I", data[0x20:0x24])[0]
    e_shentsize = struct.unpack(">H", data[0x2E:0x30])[0]
    e_shnum = struct.unpack(">H", data[0x30:0x32])[0]
    e_shstrndx = struct.unpack(">H", data[0x32:0x34])[0]

    # Read section header string table
    shstrtab_off = e_shoff + e_shstrndx * e_shentsize
    sh_name_off = struct.unpack(">I", data[shstrtab_off + 0x10: shstrtab_off + 0x14])[0]
    sh_size = struct.unpack(">I", data[shstrtab_off + 0x14: shstrtab_off + 0x18])[0]
    shstrtab = data[sh_name_off: sh_name_off + sh_size]

    for i in range(e_shnum):
        off = e_shoff + i * e_shentsize
        sh_name_idx = struct.unpack(">I", data[off: off + 4])[0]
        end = shstrtab.find(b"\x00", sh_name_idx)
        sec_name = shstrtab[sh_name_idx:end].decode("ascii", errors="replace")
        if sec_name == name:
            return True
    return False


def derive_retail_path(decomp_path: Path) -> Path | None:
    """Derive retail object path from decompiled object path.

    Converts e.g. build/us/src/PowerPC_EABI/.../foo.o
              -> build/us/obj/PowerPC_EABI/.../foo.o
    """
    path_str = str(decomp_path.resolve())
    # Try both src/ -> obj/ and the reverse
    for src_prefix, obj_prefix in [("build/us/src/", "build/us/obj/"),
                                    ("/src/", "/obj/")]:
        if src_prefix in path_str:
            retail_str = path_str.replace(src_prefix, obj_prefix, 1)
            retail = Path(retail_str)
            if retail.is_file():
                return retail

    # Try relative to project root
    try:
        rel = decomp_path.relative_to(ROOT)
        parts = list(rel.parts)
        # Find 'src' in path and replace with 'obj'
        for i, p in enumerate(parts):
            if p == "src":
                parts[i] = "obj"
                candidate = ROOT.joinpath(*parts)
                if candidate.is_file():
                    return candidate
    except ValueError:
        pass

    return None


def postprocess_object(decomp_path: Path, retail_path: Path | None = None) -> bool:
    """Copy .note.split from retail to decomp object.

    Returns True if changes were made, False if no-op.
    """
    decomp_path = decomp_path.resolve()

    if not decomp_path.is_file():
        print(f"missing decomp object: {decomp_path}", file=sys.stderr)
        return False

    # Check if decomp already has .note.split
    if has_section(decomp_path, ".note.split"):
        return False
    
    # Derive/find retail path
    if retail_path is None:
        retail_path = derive_retail_path(decomp_path)
    if retail_path is None or not retail_path.is_file():
        return False

    # Check retail has .note.split
    if not has_section(retail_path, ".note.split"):
        return False

    # Extract .note.split from retail
    with tempfile.TemporaryDirectory() as tmpdir:
        tmpfile = Path(tmpdir) / "notesplit.tmp"
        result = subprocess.run(
            [str(OBJCOPY), "--dump-section", f".note.split={tmpfile}",
             str(retail_path)],
            capture_output=True, text=True,
        )
        if result.returncode != 0 or not tmpfile.is_file():
            print(f"failed to extract .note.split from {retail_path}: {result.stderr.strip()}",
                  file=sys.stderr)
            return False

        # Make a copy of the decomp object, then add the section
        # objcopy --add-section modifies in-place, so we need intermediate
        import shutil
        temp_obj = Path(tmpdir) / "decomp_modified.o"
        shutil.copy2(decomp_path, temp_obj)

        result = subprocess.run(
            [str(OBJCOPY), "--add-section", f".note.split={tmpfile}",
             str(temp_obj), str(decomp_path)],
            capture_output=True, text=True,
        )
        if result.returncode != 0:
            print(f"failed to add .note.split to {decomp_path}: {result.stderr.strip()}",
                  file=sys.stderr)
            shutil.copy2(temp_obj, decomp_path)  # restore original
            return False

    print(f"added .note.split to {decomp_path}")
    return True


def main(argv: list[str]) -> int:
    parser = argparse.ArgumentParser(description="Add .note.split to decompiled ELF objects")
    parser.add_argument("objects", nargs="+", type=Path, help="Decompiled .o files")
    parser.add_argument("--retail", type=Path, help="Explicit retail .o path (derived auto)")
    args = parser.parse_args(argv[1:])

    rc = 0
    for obj in args.objects:
        if not postprocess_object(obj, retail_path=args.retail):
            rc = 1
    return rc


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))
