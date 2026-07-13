#!/usr/bin/env python3
"""Patch MWCC @2858 int-to-double pool in MTRand.o .sdata2 (low word 0 -> 0x80000000)."""

from __future__ import annotations

import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
DEFAULT_OBJ = ROOT / "build/us/src/monolib/src/math/MTRand.o"
MAGIC_HI = 0x43300000
MAGIC_LO = 0x80000000


def patch_object(path: Path) -> bool:
    data = bytearray(path.read_bytes())
    if data[:4] != b"\x7fELF":
        raise ValueError(f"not ELF: {path}")

    ei_data = data[5]
    if ei_data != 2:
        raise ValueError("expected big-endian ELF")

    e_shoff = struct.unpack_from(">I", data, 32)[0]
    e_shentsize = struct.unpack_from(">H", data, 46)[0]
    e_shnum = struct.unpack_from(">H", data, 48)[0]
    e_shstrndx = struct.unpack_from(">H", data, 50)[0]

    def read_shdr(index: int) -> tuple[int, int, int, int]:
        off = e_shoff + index * e_shentsize
        sh_name, sh_type, sh_flags, sh_addr, sh_offset, sh_size, _link, _info, _align, _entsize = struct.unpack_from(
            ">IIIIIIIIII", data, off
        )
        return sh_name, sh_type, sh_offset, sh_size

    shstr_name, _, shstr_off, shstr_size = read_shdr(e_shstrndx)

    def sh_name_at(index: int) -> str:
        name_off, _, _, _ = read_shdr(index)
        end = data.index(0, shstr_off + name_off)
        return data[shstr_off + name_off : end].decode("ascii")

    target_idx = None
    for i in range(e_shnum):
        if sh_name_at(i) == ".sdata2":
            target_idx = i
            break
    if target_idx is None:
        return False

    _, _, sec_off, sec_size = read_shdr(target_idx)
    patched = False
    for off in range(0, max(0, sec_size - 7), 4):
        hi = struct.unpack_from(">I", data, sec_off + off)[0]
        lo = struct.unpack_from(">I", data, sec_off + off + 4)[0]
        if hi == MAGIC_HI and lo == 0:
            struct.pack_into(">I", data, sec_off + off + 4, MAGIC_LO)
            patched = True
    if patched:
        path.write_bytes(data)
    return patched


def main(argv: list[str]) -> int:
    path = Path(argv[1]) if len(argv) > 1 else DEFAULT_OBJ
    if not path.is_file():
        print(f"missing object: {path}", file=sys.stderr)
        return 1
    if patch_object(path):
        print(f"patched {path}")
        return 0
    print(f"no patch needed for {path}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))
