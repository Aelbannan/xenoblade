from __future__ import annotations

import struct
from dataclasses import dataclass
from pathlib import Path

from tools.symbolrecover.lib.xref import SplitUnit, load_splits


@dataclass(frozen=True)
class ObjectSizeCheck:
    unit_hint: str
    split_path: str | None
    budget: int | None
    retail_text: int | None
    decomp_text: int | None
    ok: bool
    notes: str

    @property
    def over_by(self) -> int | None:
        if self.budget is None or self.decomp_text is None:
            return None
        delta = self.decomp_text - self.budget
        return delta if delta > 0 else None


def normalize_unit_hint(hint: str) -> str:
    path = hint.replace("\\", "/")
    if path.startswith("main/"):
        path = path[5:]
    if path.endswith((".cpp", ".c", ".cp", ".C", ".cc", ".cxx")):
        return path
    return f"{path}.cpp"


def find_split_unit(units: list[SplitUnit], hint: str) -> SplitUnit | None:
    norm = normalize_unit_hint(hint)
    for unit in units:
        unit_path = unit.path.replace("\\", "/")
        if unit_path == norm:
            return unit
        if unit_path.endswith("/" + norm):
            return unit
    stem = Path(norm).stem
    for ext in (".cpp", ".c", ".cp", ".C", ".cc", ".cxx"):
        candidate = f"{stem}{ext}"
        for unit in units:
            unit_path = unit.path.replace("\\", "/")
            if unit_path.endswith("/" + candidate) or unit_path == candidate:
                return unit
        # also allow hint without directory: match by filename
    for unit in units:
        unit_path = unit.path.replace("\\", "/")
        if Path(unit_path).stem == stem:
            return unit
    return None


def split_text_budget(unit: SplitUnit) -> int | None:
    if unit.text_start is None or unit.text_end is None:
        return None
    return unit.text_end - unit.text_start


def elf_section_size(path: Path, section_name: str) -> int | None:
    if not path.is_file():
        return None
    data = path.read_bytes()
    if len(data) < 0x34 or data[:4] != b"\x7fELF":
        return None
    if data[4] != 1 or data[5] != 2:
        return None

    e_shoff = struct.unpack_from(">I", data, 0x20)[0]
    e_shentsize = struct.unpack_from(">H", data, 0x2E)[0]
    e_shnum = struct.unpack_from(">H", data, 0x30)[0]
    e_shstrndx = struct.unpack_from(">H", data, 0x32)[0]
    if e_shentsize < 40 or e_shnum == 0 or e_shstrndx >= e_shnum:
        return None

    shstr_off = e_shoff + e_shstrndx * e_shentsize
    if shstr_off + 40 > len(data):
        return None
    shstr_hdr = data[shstr_off : shstr_off + 40]
    _, _, _, _, shstr_data_off, shstr_data_size, _, _, _, _ = struct.unpack(">IIIIIIIIII", shstr_hdr)
    if shstr_data_off + shstr_data_size > len(data):
        return None
    strtab = data[shstr_data_off : shstr_data_off + shstr_data_size]

    for index in range(e_shnum):
        off = e_shoff + index * e_shentsize
        if off + 40 > len(data):
            return None
        name_idx, _, _, _, _, size, _, _, _, _ = struct.unpack(">IIIIIIIIII", data[off : off + 40])
        if name_idx >= len(strtab):
            continue
        end = strtab.find(b"\x00", name_idx)
        if end < 0:
            continue
        name = strtab[name_idx:end].decode("ascii", errors="replace")
        if name == section_name:
            return size
    return None


def check_object_size(
    *,
    project_root: Path,
    region: str,
    unit_hint: str,
    retail_object: Path | None,
    decomp_object: Path | None,
) -> ObjectSizeCheck:
    splits_path = project_root / "config" / region / "splits.txt"
    split_unit = None
    budget = None
    if splits_path.is_file():
        split_unit = find_split_unit(load_splits(splits_path), unit_hint)
        if split_unit is not None:
            budget = split_text_budget(split_unit)

    retail_text = elf_section_size(retail_object, ".text") if retail_object else None
    decomp_text = elf_section_size(decomp_object, ".text") if decomp_object else None

    notes: list[str] = []
    if split_unit is None:
        notes.append("no split entry")
    elif budget is None:
        notes.append("split missing .text range")
    if retail_object and not retail_object.is_file():
        notes.append("retail .o missing")
    if decomp_object and not decomp_object.is_file():
        notes.append("decomp .o missing")
    if retail_text is None and retail_object and retail_object.is_file():
        notes.append("could not read retail .text")
    if decomp_text is None and decomp_object and decomp_object.is_file():
        notes.append("could not read decomp .text")

    ok = True
    if budget is not None and decomp_text is not None and decomp_text > budget:
        over = decomp_text - budget
        notes.append(f"decomp .text exceeds split budget by 0x{over:X} ({over} bytes)")
        ok = False
    elif budget is not None and decomp_text is not None:
        spare = budget - decomp_text
        notes.append(f"within split budget (0x{spare:X} spare)")

    return ObjectSizeCheck(
        unit_hint=unit_hint,
        split_path=split_unit.path if split_unit else None,
        budget=budget,
        retail_text=retail_text,
        decomp_text=decomp_text,
        ok=ok,
        notes="; ".join(notes) if notes else "ok",
    )


def format_size_check(check: ObjectSizeCheck) -> str:
    parts = []
    if check.budget is not None:
        parts.append(f"budget=0x{check.budget:X}")
    if check.retail_text is not None:
        parts.append(f"retail .text=0x{check.retail_text:X}")
    if check.decomp_text is not None:
        parts.append(f"decomp .text=0x{check.decomp_text:X}")
    status = "PASS" if check.ok else "FAIL"
    return f"{status} {' '.join(parts)} — {check.notes}"
