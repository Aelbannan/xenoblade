#!/usr/bin/env python3
"""List PowerPC opcodes used in a GameCube/Wii DOL (default: retail main.dol).

Requires Capstone (same pin as tools/ppc_equivalence):
  python3 -m pip install -r tools/ppc_equivalence/requirements.txt
"""

from __future__ import annotations

import argparse
import json
import struct
import sys
from collections import Counter
from dataclasses import dataclass
from pathlib import Path
from typing import Iterable, Iterator, Sequence


ROOT = Path(__file__).resolve().parents[1]
NUM_TEXT = 7
NUM_DATA = 11


@dataclass(frozen=True)
class DolSection:
    name: str
    file_offset: int
    load_addr: int
    size: int


@dataclass(frozen=True)
class OpcodeStats:
    mnemonic_counts: Counter[str]
    primary_counts: Counter[int]
    instruction_count: int
    invalid_count: int
    sections: list[dict[str, object]]


def _u32_be(data: bytes, offset: int) -> int:
    return struct.unpack_from(">I", data, offset)[0]


def parse_dol_sections(data: bytes, *, include_data: bool = False) -> list[DolSection]:
    """Parse DOL section table. Code lives in the text slots; data is optional."""
    if len(data) < 0x100:
        raise ValueError(f"DOL too small ({len(data)} bytes)")

    sections: list[DolSection] = []
    for index in range(NUM_TEXT):
        file_offset = _u32_be(data, 0x00 + 4 * index)
        load_addr = _u32_be(data, 0x48 + 4 * index)
        size = _u32_be(data, 0x90 + 4 * index)
        if file_offset and size:
            sections.append(
                DolSection(f"text{index}", file_offset, load_addr, size)
            )

    if include_data:
        for index in range(NUM_DATA):
            file_offset = _u32_be(data, 0x1C + 4 * index)
            load_addr = _u32_be(data, 0x64 + 4 * index)
            size = _u32_be(data, 0xAC + 4 * index)
            if file_offset and size:
                sections.append(
                    DolSection(f"data{index}", file_offset, load_addr, size)
                )

    return sections


def _iter_words(chunk: bytes, base_addr: int) -> Iterator[tuple[int, int]]:
    usable = len(chunk) - (len(chunk) % 4)
    for offset in range(0, usable, 4):
        yield base_addr + offset, struct.unpack_from(">I", chunk, offset)[0]


def collect_opcodes(
    data: bytes,
    sections: Sequence[DolSection],
) -> OpcodeStats:
    try:
        import capstone
    except ImportError as exc:
        raise SystemExit(
            "Capstone is required. Install with:\n"
            "  python3 -m pip install -r tools/ppc_equivalence/requirements.txt"
        ) from exc

    engine = capstone.Cs(
        capstone.CS_ARCH_PPC,
        capstone.CS_MODE_32 | capstone.CS_MODE_BIG_ENDIAN,
    )
    engine.detail = False

    mnemonic_counts: Counter[str] = Counter()
    primary_counts: Counter[int] = Counter()
    instruction_count = 0
    invalid_count = 0
    section_rows: list[dict[str, object]] = []

    for section in sections:
        end = section.file_offset + section.size
        if end > len(data):
            raise ValueError(
                f"{section.name}: section extends past file "
                f"(offset=0x{section.file_offset:x} size=0x{section.size:x})"
            )
        chunk = data[section.file_offset : end]
        local_insns = 0
        local_invalid = 0
        decoded_spans: set[int] = set()

        for insn in engine.disasm(chunk, section.load_addr):
            mnemonic_counts[insn.mnemonic] += 1
            instruction_count += 1
            local_insns += 1
            for byte_off in range(insn.address, insn.address + insn.size, 4):
                decoded_spans.add(byte_off)

            # Primary opcode is bits 0–5 of the PPC instruction word.
            if insn.size >= 4:
                word = struct.unpack_from(">I", insn.bytes, 0)[0]
                primary_counts[word >> 26] += 1

        for addr, word in _iter_words(chunk, section.load_addr):
            if addr not in decoded_spans:
                invalid_count += 1
                local_invalid += 1
                primary_counts[word >> 26] += 1

        section_rows.append(
            {
                "name": section.name,
                "file_offset": section.file_offset,
                "load_addr": section.load_addr,
                "size": section.size,
                "instructions": local_insns,
                "undecoded_words": local_invalid,
            }
        )

    return OpcodeStats(
        mnemonic_counts=mnemonic_counts,
        primary_counts=primary_counts,
        instruction_count=instruction_count,
        invalid_count=invalid_count,
        sections=section_rows,
    )


def _default_dol_path(region: str) -> Path:
    return ROOT / "orig" / region / "sys" / "main.dol"


def _resolve_dol(path: Path | None, region: str) -> Path:
    if path is not None:
        return path
    try:
        from tools.coop.lib.config import load_config

        config = load_config(None, ROOT)
        return config.main_dol
    except Exception:
        return _default_dol_path(region)


def _print_human(stats: OpcodeStats, *, by: str, sort: str) -> None:
    print(f"instructions: {stats.instruction_count}")
    print(f"undecoded words: {stats.invalid_count}")
    print(f"unique mnemonics: {len(stats.mnemonic_counts)}")
    print(f"unique primary opcodes: {len(stats.primary_counts)}")
    print()
    print("sections:")
    for row in stats.sections:
        print(
            f"  {row['name']}: "
            f"VA=0x{int(row['load_addr']):08x} "
            f"file=0x{int(row['file_offset']):x} "
            f"size=0x{int(row['size']):x} "
            f"insns={row['instructions']} "
            f"undecoded={row['undecoded_words']}"
        )
    print()

    if by == "primary":
        items: Iterable[tuple[object, int]] = stats.primary_counts.items()
        if sort == "count":
            ordered = sorted(items, key=lambda item: (-item[1], int(item[0])))
        else:
            ordered = sorted(items, key=lambda item: int(item[0]))
        print(f"{'primary':>8}  {'count':>10}  mnemonic-family (bits 0-5)")
        for primary, count in ordered:
            print(f"{int(primary):>8}  {count:>10}")
        return

    items = stats.mnemonic_counts.items()
    if sort == "count":
        ordered = sorted(items, key=lambda item: (-item[1], str(item[0])))
    else:
        ordered = sorted(items, key=lambda item: str(item[0]))
    print(f"{'mnemonic':<16}  {'count':>10}")
    for mnemonic, count in ordered:
        print(f"{mnemonic:<16}  {count:>10}")


def _print_json(stats: OpcodeStats, *, by: str) -> None:
    payload = {
        "instructions": stats.instruction_count,
        "undecoded_words": stats.invalid_count,
        "sections": stats.sections,
        "mnemonics": dict(sorted(stats.mnemonic_counts.items())),
        "primary_opcodes": {
            str(key): value for key, value in sorted(stats.primary_counts.items())
        },
        "by": by,
    }
    json.dump(payload, sys.stdout, indent=2)
    sys.stdout.write("\n")


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        description="List PowerPC opcodes used in a GameCube/Wii DOL",
    )
    parser.add_argument(
        "dol",
        nargs="?",
        type=Path,
        help="Path to DOL (default: orig/<region>/sys/main.dol from coop.json)",
    )
    parser.add_argument(
        "--region",
        default="us",
        help="Region used when resolving the default main.dol (default: us)",
    )
    parser.add_argument(
        "--include-data",
        action="store_true",
        help="Also scan DOL data sections (usually not code)",
    )
    parser.add_argument(
        "--by",
        choices=("mnemonic", "primary"),
        default="mnemonic",
        help="Group by Capstone mnemonic (default) or PPC primary opcode field",
    )
    parser.add_argument(
        "--sort",
        choices=("name", "count"),
        default="name",
        help="Sort human output by name (default) or descending count",
    )
    parser.add_argument(
        "--json",
        action="store_true",
        help="Emit JSON instead of a table",
    )
    parser.add_argument(
        "--names-only",
        action="store_true",
        help="Print one mnemonic per line (no counts); implies --by mnemonic",
    )
    return parser


def main(argv: Sequence[str] | None = None) -> int:
    args = build_parser().parse_args(argv)
    dol_path = _resolve_dol(args.dol, args.region)
    if not dol_path.is_file():
        print(f"error: DOL not found: {dol_path}", file=sys.stderr)
        return 1

    data = dol_path.read_bytes()
    sections = parse_dol_sections(data, include_data=args.include_data)
    if not sections:
        print(f"error: no sections found in {dol_path}", file=sys.stderr)
        return 1

    stats = collect_opcodes(data, sections)

    if args.names_only:
        for mnemonic in sorted(stats.mnemonic_counts):
            print(mnemonic)
        return 0

    if args.json:
        _print_json(stats, by=args.by)
    else:
        print(f"dol: {dol_path}")
        _print_human(stats, by=args.by, sort=args.sort)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
