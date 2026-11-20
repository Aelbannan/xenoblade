"""Fail-closed function byte comparison, shared by cycle and hexdiff.

Relocation names are identifiers, never wildcards or fuzzy name matches.  ELF
symbol indices retain section/value evidence even for stripped retail names.
Only proven equal relocation writes are masked from the byte comparison.
"""
from __future__ import annotations

import struct
import re
from dataclasses import dataclass, replace
from functools import lru_cache
from pathlib import Path

from tools.coop.lib.reloc_canon import LinkerLayout, addr16_immediate
from tools.coop.lib.weak_dedup import _Elf, load_unit_ranges, resolve_unit, linked_view
from tools.ppc_equivalence.elf_symbols import FunctionBytes, list_code_functions, _resolve_candidates


@dataclass(frozen=True)
class ByteComparison:
    match: bool
    retail: FunctionBytes
    decomp: FunctionBytes
    unresolved: tuple[int, ...] = ()
    reasons: tuple[str, ...] = ()


def unit_section_bases(root: Path, region: str, unit: str) -> dict[str, int]:
    path = root / "config" / region / "splits.txt"
    ranges = _ranges(path, path.stat().st_mtime_ns)
    owner = resolve_unit(ranges, unit)
    return {section: start for path, section, start, end in ranges if path == owner}


@lru_cache(maxsize=8)
def _ranges(path, mtime):
    return load_unit_ranges(path)


@lru_cache(maxsize=8)
def _layout(root, region, symbols_mtime, splits_mtime, lcf_mtime):
    return LinkerLayout.load(root, region)


def load_layout(root, region):
    symbols = root / "config" / region / "symbols.txt"
    splits = root / "config" / region / "splits.txt"
    lcf = root / "build" / region / "ldscript.lcf"
    return _layout(root, region, symbols.stat().st_mtime_ns, splits.stat().st_mtime_ns,
                   lcf.stat().st_mtime_ns if lcf.exists() else 0)


@lru_cache(maxsize=256)
def _object_symbols(path: Path, mtime: int, size: int):
    elf = _Elf(path.read_bytes())
    targets = {}
    for i, section in elf.rela_target.items():
        for offset, info, addend in elf.relas(i):
            index = info >> 8
            sym = elf.syms[index]
            targets[(elf.names[section], offset)] = (
                elf.symnames[index], sym[1], sym[3] >> 4, sym[5],
                elf.names[sym[5]] if 0 < sym[5] < len(elf.names) else None,
            )
    return targets


@lru_cache(maxsize=8)
def _symbol_aliases(root, region, mtime):
    # Only unique short-name aliases have an address. Overloads at distinct
    # addresses stay unresolved; a prefix alone is never equality evidence.
    layout = load_layout(root, region)
    aliases = {}
    for name, (section, address) in layout.symbols.items():
        short = name.split("__", 1)[0]
        if short:
            aliases.setdefault(short, set()).add(address)
    return aliases


def _named_address(name, layout):
    if layout is None or not name or (name.startswith(("@", "...", "$"))
                                      and not name.startswith("@STRING@")):
        return None
    _, address = layout.resolve_name(name)
    if address is not None:
        return address
    symbols = layout.root / "config" / layout.region / "symbols.txt"
    if symbols.is_file():
        aliases = _symbol_aliases(layout.root, layout.region, symbols.stat().st_mtime_ns)
        # Short C linker names preserved by the splitter can correspond to
        # one mangled C++ definition. Require a unique mapped address.
        addresses = aliases.get(name, set())
        if len(addresses) == 1:
            return next(iter(addresses))
        short = name.split("__", 1)[0]
        if short and short in layout.symbols and len(aliases.get(short, ())) == 1:
            return layout.symbols[short][1]
    return None


@lru_cache(maxsize=64)
def _linked_functions(path, mtime, size, root, region, unit, smtime, rmtime):
    view = linked_view(path, root, region, unit)
    return list_code_functions(view.path)


def compare_unit_functions(project, unit, left, right):
    root, region = project.root, project.config.region
    try:
        layout = load_layout(root, region)
        bases = unit_section_bases(root, region, unit.name)
        st = right.path.stat()
        functions = _linked_functions(right.path, st.st_mtime_ns, st.st_size, root, region, unit.name,
            (root / "config" / region / "symbols.txt").stat().st_mtime_ns,
            (root / "config" / region / "splits.txt").stat().st_mtime_ns)
        linked = pair_function(functions, right.name)
        if linked is None:
            raise ValueError(f"missing or ambiguous linked function {right.name}")
        right = linked
    except OSError:
        layout, bases = None, {}
    return compare_functions(left, right, linker_layout=layout, section_bases=bases)


def _target(function, reloc, layout, bases, canonical):
    name = reloc.symbol
    info = None
    try:
        st = function.path.stat()
        info = _object_symbols(function.path, st.st_mtime_ns, st.st_size).get(
            (function.section_name, function.value + reloc.offset))
    except (OSError, ValueError, IndexError, struct.error):
        pass
    addend = reloc.addend
    # REL implicit addends need decoding per relocation type. Refuse until
    # implemented; treating None as zero can certify the wrong destination.
    if addend is None:
        return None
    if info is not None:
        name, value, binding, shndx, section = info
        if shndx == 0xFFF1:
            return ("address", (value + addend) & 0xffffffff)
        # Named local functions also have retail identities; their raw object
        # offset can shift when preceding functions differ. Use the paired
        # retail symbol address, as for external calls.
        if section in (".text", ".init") and name not in ("", "(null)"):
            address = _named_address(name, layout)
            if address is not None:
                return ("address", (address + addend) & 0xffffffff)
        # Local/section labels belong to this object, not a global name map.
        # The same @N spelling can denote different objects on the two sides.
        if section is not None and (binding == 0 or name in ("", "(null)") or name.startswith(("@", "...", "$"))):
            if section in bases:
                return ("address", (bases[section] + value + addend) & 0xffffffff)
            return ("local", section, value + addend)
    if canonical and name in canonical:
        name = canonical[name]
    if name in ("", "(null)"):
        return None
    # @STRING@<mangled fn>@N are named compiler string-pool symbols: unlike
    # TU-local @N/.../$ labels, they are listed in symbols.txt and must resolve
    # through the linker layout (including when weak dedup turned a copy into
    # an UNDEF reference to the surviving retail copy).
    named = not name.startswith(("@", "...", "$")) or name.startswith("@STRING@")
    if layout is not None and named:
        address = _named_address(name, layout)
        if address is not None:
            return ("address", (address + addend) & 0xffffffff)
    if info is not None and info[4] is not None:
        section, value = info[4], info[1]
        if section in bases:
            return ("address", (bases[section] + value + addend) & 0xffffffff)
        return ("local", section, value + addend)
    if name.startswith(("@", "...", "$")) and not name.startswith("@STRING@"):
        return None
    return ("symbol", name, addend)


# (field width in bytes, bits installed by the linker). SDA21 also installs RA.
_FIELDS = {1: (4, 0xffffffff), 2: (4, 0x03fffffc),
           3: (2, 0xffff), 4: (2, 0xffff), 5: (2, 0xffff), 6: (2, 0xffff),
           10: (4, 0x03fffffc), 11: (4, 0xfffc), 12: (4, 0xfffc),
           13: (4, 0xfffc), 24: (4, 0xffffffff), 25: (2, 0xffff),
           26: (4, 0xffffffff), 109: (4, 0x001fffff)}


def compare_functions(left: FunctionBytes, right: FunctionBytes, *,
                      linker_layout: LinkerLayout | None = None,
                      section_bases: dict[str, int] | None = None,
                      canonical_symbols: dict[str, str] | None = None) -> ByteComparison:
    """Compare size, ordinary bytes, and every relocation's proven destination.

    Same exact undefined symbol + addend is a symbolic linker proof. Named
    aliases require equal resolved addresses. One-sided HI/HA/LO relocations
    require an address and the exact baked immediate on the opposite side.
    Unknown symbols, duplicate sites, unsupported types and implicit addends
    fail closed. Never use mined name/offset guesses as acceptance evidence.
    """
    bases = section_bases or {}
    lcode, rcode = bytearray(left.code), bytearray(right.code)
    lrel, rrel = {}, {}
    bad, reasons = set(), []
    for function, table in ((left, lrel), (right, rrel)):
        for reloc in function.relocations:
            if reloc.relocation_type == 0:  # R_PPC_NONE writes no bytes.
                continue
            if reloc.offset in table:
                bad.add(reloc.offset)
                reasons.append(f"duplicate relocation at +0x{reloc.offset:x}")
            table[reloc.offset] = reloc
    for offset in sorted(lrel.keys() | rrel.keys()):
        lr, rr = lrel.get(offset), rrel.get(offset)
        present = lr or rr
        field = _FIELDS.get(present.relocation_type)
        if field is None or offset < 0 or offset + field[0] > min(len(lcode), len(rcode)):
            bad.add(offset)
            reasons.append(f"unsupported/out-of-range relocation at +0x{offset:x}")
            continue
        width, mask = field
        lt = _target(left, lr, linker_layout, bases, canonical_symbols) if lr else None
        rt = _target(right, rr, linker_layout, bases, canonical_symbols) if rr else None
        if lr is not None and rr is not None:
            ok = lr.relocation_type == rr.relocation_type and lt is not None and lt == rt
        else:
            target = lt if lr else rt
            other = rcode if lr else lcode
            imm = (addr16_immediate(target[1], present.relocation_type)
                   if target and target[0] == "address" else None)
            ok = present.relocation_type in (4, 5, 6) and imm is not None and int.from_bytes(other[offset:offset + width], "big") == imm
        if not ok:
            bad.add(offset)
            category = "wrong-address relocation" if lt and rt and lt[0] == rt[0] == "address" and lt != rt else "unproven relocation"
            reasons.append(f"{category} at +0x{offset:x}: {lt!r} vs {rt!r}")
            continue
        for code in (lcode, rcode):
            value = int.from_bytes(code[offset:offset + width], "big") & ~mask
            code[offset:offset + width] = value.to_bytes(width, "big")
    lnorm = replace(left, code=bytes(lcode), relocations=())
    rnorm = replace(right, code=bytes(rcode), relocations=())
    ok = not bad and left.size == right.size == len(left.code) == len(right.code) and lcode == rcode
    return ByteComparison(ok, lnorm, rnorm, tuple(sorted(bad)), tuple(reasons))


def recover_retail_names(functions, root: Path, region: str, unit: str, rows=()):
    """Recover stripped names using retail splits, never decomp offsets."""
    if not any(f.name in ("", "(null)") for f in functions):
        return list(functions)
    bases = unit_section_bases(root, region, unit)
    names = {int(row["address"], 0): row["symbol"] for row in rows
             if row.get("address") and row.get("symbol")}
    return [replace(f, name=names.get(bases.get(f.section_name, 0) + f.value, f.name))
            if f.name in ("", "(null)") else f for f in functions]


def pair_function(functions, symbol):
    """Resolve one identity; ambiguity and missing names never use offsets."""
    candidates = _resolve_candidates(functions, symbol)
    if not candidates:
        alias = re.match(r"^@\d+@(.+)$", symbol)
        if alias:
            candidates = _resolve_candidates(functions, alias.group(1))
    return candidates[0] if len(candidates) == 1 else None


def resolve_unit_pair(project, unit, symbol):
    from tools.coop.lib.targets import load_targets_document
    rows = load_targets_document(project.config).get("targets", [])
    retail = recover_retail_names(list_code_functions(unit.target_path), project.root,
                                  project.config.region, unit.name, rows)
    left = pair_function(retail, symbol)
    right = pair_function(list_code_functions(unit.base_path), symbol)
    if left is None or right is None:
        raise ValueError(f"missing or ambiguous function pair: {symbol}")
    return left, right


def check_unit_function(project, unit, symbol):
    left, right = resolve_unit_pair(project, unit, symbol)
    return compare_unit_functions(project, unit, left, right)
