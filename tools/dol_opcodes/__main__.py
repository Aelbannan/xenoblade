#!/usr/bin/env python3
"""Census of PowerPC opcodes used in a GameCube/Wii DOL.

Default decoder matches each instruction word against the vendored PPC 750CL
ISA table (Broadway / Gekko lineage):

  tools/dol_opcodes/isa.yaml
  https://github.com/riptl/ppc750cl/blob/master/isa.yaml

Optional Capstone mode remains for comparison only — Capstone will invent
VSX/AltiVec mnemonics that are not on Broadway.

Requires PyYAML. Capstone is only needed for --decoder capstone / --compare.
"""

from __future__ import annotations

import argparse
import json
import struct
import sys
from collections import Counter
from dataclasses import dataclass
from pathlib import Path
from typing import Iterable, Sequence


PACKAGE_DIR = Path(__file__).resolve().parent
ROOT = PACKAGE_DIR.parents[1]
DEFAULT_ISA = PACKAGE_DIR / "isa.yaml"
NUM_TEXT = 7
NUM_DATA = 11


@dataclass(frozen=True)
class DolSection:
    name: str
    file_offset: int
    load_addr: int
    size: int


@dataclass(frozen=True)
class IsaOpcode:
    name: str
    bitmask: int
    pattern: int
    modifiers: tuple[str, ...]
    specificity: int  # popcount(bitmask); higher wins on ties


@dataclass(frozen=True)
class OpcodeStats:
    mnemonic_counts: Counter[str]
    primary_counts: Counter[int]
    instruction_count: int
    invalid_count: int
    sections: list[dict[str, object]]
    decoder: str
    isa_path: Path | None = None
    known_names: frozenset[str] | None = None
    compare_unknown: Counter[str] | None = None


def _u32_be(data: bytes, offset: int) -> int:
    return struct.unpack_from(">I", data, offset)[0]


def _ppc_bit(word: int, bit: int) -> int:
    """Read PowerPC bit numbering (bit 0 = MSB)."""
    return (word >> (31 - bit)) & 1


def parse_dol_sections(data: bytes, *, include_data: bool = False) -> list[DolSection]:
    if len(data) < 0x100:
        raise ValueError(f"DOL too small ({len(data)} bytes)")

    sections: list[DolSection] = []
    for index in range(NUM_TEXT):
        file_offset = _u32_be(data, 0x00 + 4 * index)
        load_addr = _u32_be(data, 0x48 + 4 * index)
        size = _u32_be(data, 0x90 + 4 * index)
        if file_offset and size:
            sections.append(DolSection(f"text{index}", file_offset, load_addr, size))

    if include_data:
        for index in range(NUM_DATA):
            file_offset = _u32_be(data, 0x1C + 4 * index)
            load_addr = _u32_be(data, 0x64 + 4 * index)
            size = _u32_be(data, 0xAC + 4 * index)
            if file_offset and size:
                sections.append(DolSection(f"data{index}", file_offset, load_addr, size))

    return sections


def load_isa(path: Path) -> tuple[list[IsaOpcode], dict[str, dict], frozenset[str]]:
    try:
        import yaml
    except ImportError as exc:
        raise SystemExit(
            "PyYAML is required for ISA matching. Install with:\n"
            "  python3 -m pip install pyyaml"
        ) from exc

    text = path.read_text(encoding="utf-8")
    # Allow a short comment preamble before the real YAML document.
    start = text.find("\nfields:")
    if start >= 0 and not text.lstrip().startswith("fields:"):
        text = text[start + 1 :]
    data = yaml.safe_load(text)

    modifiers = {entry["name"]: entry for entry in data.get("modifiers", [])}
    opcodes: list[IsaOpcode] = []
    known: set[str] = set()

    for entry in data["opcodes"]:
        bitmask = int(entry["bitmask"])
        pattern = int(entry["pattern"])
        mods = tuple(entry.get("modifiers") or ())
        op = IsaOpcode(
            name=entry["name"],
            bitmask=bitmask,
            pattern=pattern,
            modifiers=mods,
            specificity=bin(bitmask).count("1"),
        )
        opcodes.append(op)
        known.add(op.name)
        # Enumerate bit-modifier spellings (OE/Rc/LK/AA) so name sets cover
        # Capstone forms like add., bl, bla, addo.
        bit_mods = [modifiers[name] for name in mods if "bit" in modifiers.get(name, {})]
        n = len(bit_mods)
        for mask in range(1 << n):
            name = op.name
            # Names that already include Rc (andi., addic.) keep their trailing '.' .
            for index, mod in enumerate(bit_mods):
                if mask & (1 << index):
                    name += mod["suffix"]
            known.add(name)

    for entry in data.get("mnemonics", []):
        known.add(entry["name"])
        for mod_name in entry.get("modifiers") or ():
            mod = modifiers.get(mod_name)
            if mod and "bit" in mod:
                known.add(entry["name"] + mod["suffix"])

    # Most-specific encoding first for stable matching.
    opcodes.sort(key=lambda op: (-op.specificity, op.name))
    return opcodes, modifiers, frozenset(known)


def match_isa_opcode(
    word: int,
    opcodes: Sequence[IsaOpcode],
    modifiers: dict[str, dict],
) -> str | None:
    best: IsaOpcode | None = None
    for op in opcodes:
        if (word & op.bitmask) != op.pattern:
            continue
        if best is None or op.specificity > best.specificity:
            best = op
        # opcodes are sorted by descending specificity; first hit is enough
        break
    if best is None:
        return None

    name = best.name
    for mod_name in best.modifiers:
        mod = modifiers.get(mod_name)
        if not mod or "bit" not in mod:
            continue
        if _ppc_bit(word, int(mod["bit"])):
            name += mod["suffix"]
    return name


def collect_opcodes_isa(
    data: bytes,
    sections: Sequence[DolSection],
    isa_path: Path,
) -> OpcodeStats:
    opcodes, modifiers, known = load_isa(isa_path)
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
        usable = len(chunk) - (len(chunk) % 4)
        local_insns = 0
        local_invalid = 0

        for offset in range(0, usable, 4):
            word = struct.unpack_from(">I", chunk, offset)[0]
            primary_counts[word >> 26] += 1
            name = match_isa_opcode(word, opcodes, modifiers)
            if name is None:
                invalid_count += 1
                local_invalid += 1
                continue
            mnemonic_counts[name] += 1
            instruction_count += 1
            local_insns += 1

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
        decoder="ppc750cl-isa",
        isa_path=isa_path,
        known_names=known,
    )


def collect_opcodes_capstone(
    data: bytes,
    sections: Sequence[DolSection],
    *,
    known_names: frozenset[str] | None = None,
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
    compare_unknown: Counter[str] = Counter()
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
        usable = len(chunk) - (len(chunk) % 4)
        local_insns = 0
        local_invalid = 0

        pos = 0
        while pos < usable:
            decoded_any = False
            for insn in engine.disasm(chunk[pos:usable], section.load_addr + pos):
                decoded_any = True
                if insn.size != 4:
                    word = struct.unpack_from(">I", chunk, pos)[0]
                    primary_counts[word >> 26] += 1
                    invalid_count += 1
                    local_invalid += 1
                    pos += 4
                    break
                word = struct.unpack_from(">I", insn.bytes, 0)[0]
                primary_counts[word >> 26] += 1
                mnemonic_counts[insn.mnemonic] += 1
                if known_names is not None and insn.mnemonic not in known_names:
                    compare_unknown[insn.mnemonic] += 1
                instruction_count += 1
                local_insns += 1
                pos = insn.address + insn.size - section.load_addr
            if not decoded_any:
                word = struct.unpack_from(">I", chunk, pos)[0]
                primary_counts[word >> 26] += 1
                invalid_count += 1
                local_invalid += 1
                pos += 4

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
        decoder="capstone",
        known_names=known_names,
        compare_unknown=compare_unknown if known_names is not None else None,
    )


def _default_dol_path(region: str) -> Path:
    return ROOT / "orig" / region / "sys" / "main.dol"


def _resolve_dol(path: Path | None, region: str) -> Path:
    if path is not None:
        return path
    try:
        from tools.coop.lib.config import load_config

        return load_config(None, ROOT).main_dol
    except Exception:
        return _default_dol_path(region)


def _print_human(stats: OpcodeStats, *, by: str, sort: str) -> None:
    print(f"decoder: {stats.decoder}")
    if stats.isa_path is not None:
        print(f"isa: {stats.isa_path}")
    print(f"instructions: {stats.instruction_count}")
    print(f"undecoded words: {stats.invalid_count}")
    label = "opcodes" if stats.decoder == "ppc750cl-isa" else "mnemonics"
    print(f"unique {label}: {len(stats.mnemonic_counts)}")
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
        ordered = (
            sorted(items, key=lambda item: (-item[1], int(item[0])))
            if sort == "count"
            else sorted(items, key=lambda item: int(item[0]))
        )
        print(f"{'primary':>8}  {'count':>10}")
        for primary, count in ordered:
            print(f"{int(primary):>8}  {count:>10}")
        return

    items = stats.mnemonic_counts.items()
    ordered = (
        sorted(items, key=lambda item: (-item[1], str(item[0])))
        if sort == "count"
        else sorted(items, key=lambda item: str(item[0]))
    )
    col = "opcode" if stats.decoder == "ppc750cl-isa" else "mnemonic"
    print(f"{col:<16}  {'count':>10}")
    for mnemonic, count in ordered:
        print(f"{mnemonic:<16}  {count:>10}")

    if stats.compare_unknown:
        print()
        print(
            f"Capstone mnemonics NOT in PPC750CL isa.yaml "
            f"({len(stats.compare_unknown)} unique, "
            f"{sum(stats.compare_unknown.values())} hits):"
        )
        unknown = (
            sorted(stats.compare_unknown.items(), key=lambda item: (-item[1], item[0]))
            if sort == "count"
            else sorted(stats.compare_unknown.items())
        )
        for mnemonic, count in unknown:
            print(f"  {mnemonic:<16}  {count:>10}")


def _print_json(stats: OpcodeStats, *, by: str) -> None:
    payload: dict[str, object] = {
        "decoder": stats.decoder,
        "isa": str(stats.isa_path) if stats.isa_path else None,
        "instructions": stats.instruction_count,
        "undecoded_words": stats.invalid_count,
        "sections": stats.sections,
        "opcodes": dict(sorted(stats.mnemonic_counts.items())),
        "primary_opcodes": {
            str(key): value for key, value in sorted(stats.primary_counts.items())
        },
        "by": by,
    }
    if stats.compare_unknown is not None:
        payload["capstone_not_in_isa"] = dict(
            sorted(stats.compare_unknown.items())
        )
    json.dump(payload, sys.stdout, indent=2)
    sys.stdout.write("\n")


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        description=(
            "List PowerPC opcodes used in a GameCube/Wii DOL "
            "(default: match against PPC750CL isa.yaml)"
        ),
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
        "--decoder",
        choices=("isa", "capstone"),
        default="isa",
        help="isa = PPC750CL table (default); capstone = Capstone mnemonics",
    )
    parser.add_argument(
        "--isa",
        type=Path,
        default=DEFAULT_ISA,
        help=f"Path to isa.yaml (default: {DEFAULT_ISA})",
    )
    parser.add_argument(
        "--compare",
        action="store_true",
        help="With --decoder capstone, flag mnemonics absent from isa.yaml",
    )
    parser.add_argument(
        "--by",
        choices=("mnemonic", "primary"),
        default="mnemonic",
        help="Group by opcode/mnemonic name (default) or primary opcode field",
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
        help="Print one opcode/mnemonic per line (no counts)",
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

    if args.decoder == "isa":
        if not args.isa.is_file():
            print(f"error: ISA table not found: {args.isa}", file=sys.stderr)
            return 1
        stats = collect_opcodes_isa(data, sections, args.isa)
    else:
        known = None
        if args.compare:
            if not args.isa.is_file():
                print(f"error: ISA table not found: {args.isa}", file=sys.stderr)
                return 1
            known = load_isa(args.isa)[2]
        stats = collect_opcodes_capstone(data, sections, known_names=known)

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
