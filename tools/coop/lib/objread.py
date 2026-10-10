"""Safe object-file reads.

GNU objcopy treats a **missing output path** as in-place conversion:
``objcopy -O binary --only-section=.text foo.o`` rewrites ``foo.o`` as a raw
section dump. During a wave-DL verification that mistake silently destroyed 86
object files and looked like a 600-symbol link regression.

Use these helpers instead of hand-rolling objcopy calls:

    from tools.coop.lib.objread import section_bytes, text_bytes, require_elf

Every function passes an explicit temporary output path, and ``require_elf``
gives a cheap guard before/after a batch of reads. The static counterpart is
``tools/coop/check_script_hazards.py`` (run by the test suite).
"""
from __future__ import annotations

import pathlib
import subprocess
import tempfile

ROOT = pathlib.Path(__file__).resolve().parents[3]
OBJCOPY = ROOT / "build" / "binutils" / "powerpc-eabi-objcopy"
OBJDUMP = ROOT / "build" / "binutils" / "powerpc-eabi-objdump"


class NotAnObjectError(RuntimeError):
    """The file is not an ELF relocatable object any more (raw dump/corrupt)."""


def is_elf(path: str | pathlib.Path) -> bool:
    """True when objdump can parse ``path`` as a relocatable object."""
    r = subprocess.run([str(OBJDUMP), "-f", str(path)], capture_output=True, text=True)
    return r.returncode == 0 and "file format not recognized" not in (r.stdout + r.stderr).lower()


def require_elf(path: str | pathlib.Path) -> pathlib.Path:
    """Return ``path`` if it is still an ELF object, else raise."""
    p = pathlib.Path(path)
    if not is_elf(p):
        raise NotAnObjectError(
            f"{p} is not an ELF object (raw dump or corrupt). Do not keep writing to it; "
            f"restore it (rebuild, or dtk dol split) before continuing."
        )
    return p


def section_bytes(obj: str | pathlib.Path, section: str) -> bytes | None:
    """Raw bytes of one section, without ever touching the input file.

    Returns None when the section does not exist or the object is unreadable.
    """
    obj = pathlib.Path(obj)
    if not is_elf(obj):
        return None
    with tempfile.TemporaryDirectory() as td:
        out = pathlib.Path(td) / "section.bin"
        r = subprocess.run(
            [str(OBJCOPY), "-O", "binary", f"--only-section={section}", str(obj), str(out)],
            capture_output=True,
        )
        if r.returncode != 0 or not out.is_file():
            return None
        return out.read_bytes()


def text_bytes(obj: str | pathlib.Path) -> bytes | None:
    """Raw ``.text`` bytes of an object (None when unreadable/absent)."""
    return section_bytes(obj, ".text")


def section_size(obj: str | pathlib.Path, section: str) -> int | None:
    """S_size of a section, or None when absent."""
    r = subprocess.run([str(OBJDUMP), "-h", str(obj)], capture_output=True, text=True)
    if r.returncode != 0:
        return None
    for line in r.stdout.splitlines():
        parts = line.split()
        if len(parts) >= 3 and parts[1] == section:
            try:
                return int(parts[2], 16)
            except ValueError:
                return None
    return None
