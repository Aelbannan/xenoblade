"""Census relocation entries in PowerPC ELF32 BE relocatable objects."""

from __future__ import annotations

import argparse
import json
import sys
from collections import Counter, defaultdict
from dataclasses import dataclass
from pathlib import Path
from typing import Any

from tools.ppc_equivalence.elf_symbols import ElfSymbolError, SectionRelocation, list_section_relocations

R_PPC_ADDR32 = 1

# Common ELF32 PowerPC relocation type numbers (extend as needed).
R_PPC_RELOC_NAMES: dict[int, str] = {
    1: "R_PPC_ADDR32",
    2: "R_PPC_ADDR24",
    3: "R_PPC_ADDR16",
    4: "R_PPC_ADDR16_LO",
    5: "R_PPC_ADDR16_HI",
    6: "R_PPC_ADDR16_HA",
    7: "R_PPC_ADDR14",
    8: "R_PPC_ADDR14_BRTAKEN",
    9: "R_PPC_ADDR14_BRNTAKEN",
    10: "R_PPC_REL24",
    11: "R_PPC_REL14",
    12: "R_PPC_REL14_BRTAKEN",
    13: "R_PPC_REL14_BRNTAKEN",
    14: "R_PPC_GOT16",
    15: "R_PPC_GOT16_LO",
    16: "R_PPC_GOT16_HI",
    17: "R_PPC_GOT16_HA",
    18: "R_PPC_PLTREL24",
    19: "R_PPC_COPY",
    20: "R_PPC_GLOB_DAT",
    21: "R_PPC_JMP_SLOT",
    22: "R_PPC_RELATIVE",
    23: "R_PPC_LOCAL24PC",
    24: "R_PPC_UADDR32",
    25: "R_PPC_UADDR16",
    26: "R_PPC_REL32",
    27: "R_PPC_PLT32",
    28: "R_PPC_PLTREL32",
    29: "R_PPC_PLT16_LO",
    30: "R_PPC_PLT16_HI",
    31: "R_PPC_PLT16_HA",
    32: "R_PPC_SDAREL16",
    33: "R_PPC_SECTOFF",
    34: "R_PPC_SECTOFF_LO",
    35: "R_PPC_SECTOFF_HI",
    36: "R_PPC_SECTOFF_HA",
    37: "R_PPC_ADDR30",
    38: "R_PPC_DTPMOD32",
    39: "R_PPC_TPREL16",
    40: "R_PPC_TPREL16_LO",
    41: "R_PPC_TPREL16_HI",
    42: "R_PPC_TPREL16_HA",
    43: "R_PPC_TPREL32",
    44: "R_PPC_DTPREL16",
    45: "R_PPC_DTPREL16_LO",
    46: "R_PPC_DTPREL16_HI",
    47: "R_PPC_DTPREL16_HA",
    48: "R_PPC_DTPREL32",
    49: "R_PPC_GOT_TLSGD16",
    50: "R_PPC_GOT_TLSGD16_LO",
    51: "R_PPC_GOT_TLSGD16_HI",
    52: "R_PPC_GOT_TLSGD16_HA",
    53: "R_PPC_GOT_TLSLD16",
    54: "R_PPC_GOT_TLSLD16_LO",
    55: "R_PPC_GOT_TLSLD16_HI",
    56: "R_PPC_GOT_TLSLD16_HA",
    57: "R_PPC_GOT_TPREL16",
    58: "R_PPC_GOT_TPREL16_LO",
    59: "R_PPC_GOT_TPREL16_HI",
    60: "R_PPC_GOT_TPREL16_HA",
    101: "R_PPC_EMB_NADDR32",
    102: "R_PPC_EMB_NADDR16",
    103: "R_PPC_EMB_NADDR16_LO",
    104: "R_PPC_EMB_NADDR16_HI",
    105: "R_PPC_EMB_NADDR16_HA",
    106: "R_PPC_EMB_SDAI16",
    107: "R_PPC_EMB_SDA2I16",
    108: "R_PPC_EMB_SDA2REL",
    109: "R_PPC_EMB_SDA21",
    110: "R_PPC_EMB_SDA2I16",
    111: "R_PPC_EMB_SDA2REL",
    112: "R_PPC_EMB_SDA2I16",
    116: "R_PPC_EMB_RELSDA",
    117: "R_PPC_EMB_RELSDA",
    118: "R_PPC_EMB_RELSDA",
    119: "R_PPC_EMB_RELSDA",
    120: "R_PPC_EMB_RELSDA",
    121: "R_PPC_EMB_RELSDA",
    122: "R_PPC_EMB_RELSDA",
    123: "R_PPC_EMB_RELSDA",
    124: "R_PPC_EMB_RELSDA",
    125: "R_PPC_EMB_RELSDA",
    126: "R_PPC_EMB_RELSDA",
    127: "R_PPC_EMB_RELSDA",
    248: "R_PPC_VLE_REL8",
    249: "R_PPC_VLE_REL15",
    250: "R_PPC_VLE_REL24",
    251: "R_PPC_VLE_LO16A",
    252: "R_PPC_VLE_LO16D",
    253: "R_PPC_VLE_HI16A",
    254: "R_PPC_VLE_HI16D",
    255: "R_PPC_VLE_HA16A",
    256: "R_PPC_VLE_HA16D",
}

DATA_SECTION_PREFIXES = (
    ".rodata",
    ".data",
    ".sdata",
    ".sdata2",
    ".ctors",
    ".dtors",
    ".bss",
    ".sbss",
    ".sbss2",
)


def relocation_type_name(relocation_type: int) -> str:
    return R_PPC_RELOC_NAMES.get(relocation_type, f"R_PPC_{relocation_type}")


def section_class(section_name: str) -> str:
    if section_name == ".text":
        return "text"
    if section_name.startswith(DATA_SECTION_PREFIXES):
        return "data"
    if section_name.startswith(".rel") or section_name.startswith(".rela"):
        return "reloc_meta"
    return "other"


def iter_object_paths(root: Path) -> list[Path]:
    if root.is_file():
        return [root]
    return sorted(path for path in root.rglob("*.o") if path.is_file())


@dataclass
class CensusAccumulator:
    objects_scanned: int = 0
    objects_parsed: int = 0
    objects_with_relocations: int = 0
    parse_failures: list[dict[str, str]] | None = None
    by_target_section: dict[str, Counter[int]] | None = None
    by_section_class: dict[str, Counter[int]] | None = None
    addr32_non_text_files: list[dict[str, Any]] | None = None
    addr32_by_section: Counter[str] | None = None

    def __post_init__(self) -> None:
        if self.parse_failures is None:
            self.parse_failures = []
        if self.by_target_section is None:
            self.by_target_section = defaultdict(Counter)
        if self.by_section_class is None:
            self.by_section_class = defaultdict(Counter)
        if self.addr32_non_text_files is None:
            self.addr32_non_text_files = []
        if self.addr32_by_section is None:
            self.addr32_by_section = Counter()

    def ingest(self, path: Path, relocations: list[SectionRelocation]) -> None:
        self.objects_parsed += 1
        if not relocations:
            return
        self.objects_with_relocations += 1
        addr32_sections: Counter[str] = Counter()
        for entry in relocations:
            self.by_target_section[entry.target_section_name][entry.relocation_type] += 1
            self.by_section_class[section_class(entry.target_section_name)][entry.relocation_type] += 1
            if entry.relocation_type == R_PPC_ADDR32 and entry.target_section_name != ".text":
                addr32_sections[entry.target_section_name] += 1
                self.addr32_by_section[entry.target_section_name] += 1
        if addr32_sections:
            self.addr32_non_text_files.append(
                {
                    "path": str(path),
                    "addr32_by_section": dict(addr32_sections),
                    "total_addr32_non_text": sum(addr32_sections.values()),
                }
            )

    def to_payload(self, *, root: Path) -> dict[str, Any]:
        total_by_type: Counter[int] = Counter()
        for counter in self.by_target_section.values():
            total_by_type.update(counter)

        non_text_totals = Counter(self.by_section_class.get("data", Counter()))
        non_text_totals.update(self.by_section_class.get("other", Counter()))

        top_non_text = non_text_totals.most_common(20)
        top_text = self.by_section_class.get("text", Counter()).most_common(20)

        return {
            "root": str(root.resolve()),
            "objects_scanned": self.objects_scanned,
            "objects_parsed": self.objects_parsed,
            "objects_with_relocations": self.objects_with_relocations,
            "parse_failures": self.parse_failures,
            "totals_by_relocation_type": {
                str(relocation_type): {
                    "count": count,
                    "name": relocation_type_name(relocation_type),
                }
                for relocation_type, count in sorted(total_by_type.items())
            },
            "by_target_section": {
                section: {
                    str(relocation_type): {
                        "count": count,
                        "name": relocation_type_name(relocation_type),
                    }
                    for relocation_type, count in sorted(counter.items())
                }
                for section, counter in sorted(self.by_target_section.items())
            },
            "by_section_class": {
                section_class_name: {
                    str(relocation_type): {
                        "count": count,
                        "name": relocation_type_name(relocation_type),
                    }
                    for relocation_type, count in sorted(counter.items())
                }
                for section_class_name, counter in sorted(self.by_section_class.items())
            },
            "addr32_non_text": {
                "total": sum(
                    count
                    for section, count in self.addr32_by_section.items()
                    if section != ".text"
                ),
                "by_section": dict(sorted(self.addr32_by_section.items())),
                "example_files": sorted(
                    self.addr32_non_text_files,
                    key=lambda item: (-item["total_addr32_non_text"], item["path"]),
                )[:20],
            },
            "summary_top_types": {
                "text": [
                    {"type": relocation_type, "name": relocation_type_name(relocation_type), "count": count}
                    for relocation_type, count in top_text
                ],
                "non_text": [
                    {"type": relocation_type, "name": relocation_type_name(relocation_type), "count": count}
                    for relocation_type, count in top_non_text
                ],
            },
        }


def run_census(root: Path) -> CensusAccumulator:
    acc = CensusAccumulator()
    paths = iter_object_paths(root)
    acc.objects_scanned = len(paths)
    for path in paths:
        try:
            relocations = list_section_relocations(path)
        except (ElfSymbolError, OSError, ValueError) as exc:
            acc.parse_failures.append({"path": str(path), "error": str(exc)})
            continue
        acc.ingest(path, relocations)
    return acc


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("root", type=Path, help="Object file or directory tree to scan (*.o)")
    parser.add_argument("--json", type=Path, help="Write machine-readable census JSON here")
    args = parser.parse_args(argv)

    root = args.root
    if not root.exists():
        print(f"error: path does not exist: {root}", file=sys.stderr)
        return 2

    acc = run_census(root)
    payload = acc.to_payload(root=root)

    if args.json:
        args.json.parent.mkdir(parents=True, exist_ok=True)
        args.json.write_text(json.dumps(payload, indent=2, sort_keys=True) + "\n", encoding="utf-8")

    print(
        f"scanned {payload['objects_scanned']} objects; "
        f"parsed {payload['objects_parsed']}; "
        f"{payload['objects_with_relocations']} with relocations; "
        f"{len(payload['parse_failures'])} parse failures"
    )
    addr32 = payload["addr32_non_text"]
    print(f"R_PPC_ADDR32 in non-.text sections: {addr32['total']}")
    if args.json:
        print(f"wrote {args.json}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
