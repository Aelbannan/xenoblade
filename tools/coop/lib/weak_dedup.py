"""Linker weak-dedup view of a decomp object (PLAN.md §17.8).

The retail split objects (and ``splits.txt`` budgets) come from the
**linked** DOL.  mwld keeps a single copy of every weak definition (the first
one it sees) and discards the others, then dead-strips data that nothing
references any more.  So a unit whose original ``.o`` carried a weak copy
of, say, the inline ``IWorkEvent::~IWorkEvent`` (and the weak
``__RTTI__10IWorkEvent`` with its local name string) shows up in retail
**without** them.  Their only trace is a reference to the surviving copy in
another unit.

The decomp object is the raw compiler output, so the comparators have to
remove exactly what mwld would remove before they compare.  The rule is narrow:

* A decomp symbol is excluded only if it is **WEAK**, defined in the object, and
  the same name is in ``config/<region>/symbols.txt`` at a retail address
  that ``splits.txt`` assigns to a **different** unit.  Those are exactly
  the copies mwld discarded.  Global and local symbols, and weak symbols that
  retail places in this unit (or whose address belongs to no split unit),
  are never excluded.
* Local (non-``.text``) data is also excluded when **every** relocation that
  references it comes from an excluded range (worked out from relocations to
  a fixpoint, with no name lists).  This covers the RTTI name string and the
  ``extabindex``/``extab`` records of an excluded function.

:func:`linked_view` writes a temp copy of the object with those ranges cut
out (sections compacted, relocations re-based, and references to an
excluded weak symbol turned into UNDEF references by name, which resolve
through ``symbols.txt`` to the surviving retail copy, as in the link).  It
returns that path and the list of exclusions, which callers print so the
exclusion is never silent.
"""
from __future__ import annotations

import hashlib
import os
import re
import struct
import tempfile
from dataclasses import dataclass, field
from pathlib import Path

SHN_UNDEF = 0
SHN_ABS = 0xFFF1
SHN_COMMON = 0xFFF2
STB_LOCAL, STB_GLOBAL, STB_WEAK = 0, 1, 2
STT_SECTION, STT_FILE = 3, 4
SHT_SYMTAB, SHT_RELA, SHT_NOBITS = 2, 4, 8

_UNIT_RE = re.compile(r"^(\S.*):\s*$")
_RANGE_RE = re.compile(r"^\s+(\S+)\s+start:(0x[0-9A-Fa-f]+)\s+end:(0x[0-9A-Fa-f]+)")
_SYM_RE = re.compile(r"^(\S+)\s*=\s*(\S+?):(0x[0-9A-Fa-f]+)\s*;")


@dataclass
class LinkedView:
    path: Path
    #: Human-readable exclusions, e.g. ``.text __dt__10IWorkEventFv 0x40 (weak; retail copy in kyoshin/CTaskGame.cpp)``.
    excluded: list[str] = field(default_factory=list)
    #: Bytes removed per section name.
    removed: dict[str, int] = field(default_factory=dict)


# -- layout inputs -------------------------------------------------------------

_LAYOUT_CACHE: dict[tuple, tuple] = {}


def load_unit_ranges(splits: Path) -> list[tuple[str, str, int, int]]:
    """``(unit path, section, start, end)`` for every split range."""
    out = []
    unit = None
    for line in splits.read_text(encoding="utf-8").splitlines():
        m = _UNIT_RE.match(line)
        if m:
            unit = None if m.group(1) == "Sections" else m.group(1)
            continue
        m = _RANGE_RE.match(line)
        if m and unit is not None:
            out.append((unit, m.group(1), int(m.group(2), 16), int(m.group(3), 16)))
    return out


def load_symbol_addrs(symbols: Path) -> dict[str, int]:
    out: dict[str, int] = {}
    for line in symbols.read_text(encoding="utf-8").splitlines():
        m = _SYM_RE.match(line)
        if m:
            out.setdefault(m.group(1), int(m.group(3), 16))
    return out


def _layout(project_root: Path, region: str):
    splits = project_root / "config" / region / "splits.txt"
    symbols = project_root / "config" / region / "symbols.txt"
    key = (str(splits), splits.stat().st_mtime_ns, str(symbols), symbols.stat().st_mtime_ns)
    hit = _LAYOUT_CACHE.get(key)
    if hit is None:
        hit = (load_unit_ranges(splits), load_symbol_addrs(symbols))
        _LAYOUT_CACHE.clear()
        _LAYOUT_CACHE[key] = hit
    return hit


def _norm_unit(path: str) -> str:
    p = path.replace("\\", "/")
    if p.startswith("main/"):
        p = p[5:]
    p = re.sub(r"\.(cpp|c|cp|C|cc|cxx)$", "", p)
    return p


def resolve_unit(ranges, unit_hint: str) -> str | None:
    """Split unit path for *unit_hint* (objdiff name, path with/without ext)."""
    want = _norm_unit(unit_hint)
    paths = {u for u, *_ in ranges}
    for u in paths:
        nu = _norm_unit(u)
        if nu == want or want.endswith("/" + nu) or nu.endswith("/" + want):
            return u
    stem = want.rsplit("/", 1)[-1]
    cands = [u for u in paths if _norm_unit(u).rsplit("/", 1)[-1] == stem]
    return cands[0] if len(cands) == 1 else None


def owner_of(ranges, addr: int) -> str | None:
    for unit, _sec, start, end in ranges:
        if start <= addr < end:
            return unit
    return None


# -- ELF plumbing ----------------------------------------------------------------


class _Elf:
    def __init__(self, data: bytes):
        self.data = bytearray(data)
        self.shoff = struct.unpack_from(">I", data, 0x20)[0]
        self.shentsize = struct.unpack_from(">H", data, 0x2E)[0]
        self.shnum = struct.unpack_from(">H", data, 0x30)[0]
        self.shstrndx = struct.unpack_from(">H", data, 0x32)[0]
        self.sh = [list(struct.unpack_from(">IIIIIIIIII", data, self.shoff + i * self.shentsize))
                   for i in range(self.shnum)]
        s = self.sh[self.shstrndx]
        shstr = data[s[4]: s[4] + s[5]]
        self.names = [shstr[h[0]: shstr.index(b"\0", h[0])].decode(errors="replace") for h in self.sh]
        self.contents = [bytes(data[h[4]: h[4] + h[5]]) if h[1] != SHT_NOBITS else b"" for h in self.sh]
        self.symtab = next((i for i, h in enumerate(self.sh) if h[1] == SHT_SYMTAB), None)
        self.syms: list[list] = []
        self.symnames: list[str] = []
        if self.symtab is not None:
            st = self.sh[self.symtab]
            strtab = self.contents[st[6]]
            raw = self.contents[self.symtab]
            for o in range(0, len(raw) - 15, 16):
                sym = list(struct.unpack_from(">IIIBBH", raw, o))
                self.syms.append(sym)
                self.symnames.append(strtab[sym[0]: strtab.index(b"\0", sym[0])].decode(errors="replace")
                                     if sym[0] < len(strtab) else "")
        # rela section index -> target section index
        self.rela_target: dict[int, int] = {}
        for i, h in enumerate(self.sh):
            if h[1] != SHT_RELA:
                continue
            tgt = h[7]
            if not tgt and self.names[i].startswith(".rela"):
                tgt = next((j for j, n in enumerate(self.names) if n == self.names[i][5:]), 0)
            self.rela_target[i] = tgt

    def relas(self, i: int) -> list[list]:
        raw = self.contents[i]
        return [list(struct.unpack_from(">IIi", raw, o)) for o in range(0, len(raw) - 11, 12)]


def _sym_ranges(elf: _Elf) -> dict[int, tuple[int, int]]:
    """symbol index -> (start, end) within its section (size 0 => to next symbol)."""
    by_sec: dict[int, list[int]] = {}
    for i, s in enumerate(elf.syms):
        if s[5] in (SHN_UNDEF, SHN_ABS, SHN_COMMON) or s[5] >= elf.shnum:
            continue
        if (s[3] & 0xF) in (STT_SECTION, STT_FILE):
            continue
        by_sec.setdefault(s[5], []).append(s[1])
    out = {}
    for i, s in enumerate(elf.syms):
        if s[5] not in by_sec or (s[3] & 0xF) in (STT_SECTION, STT_FILE):
            continue
        # A symbol owns the bytes up to the next symbol (or the section end):
        # its own size plus the alignment padding the compiler put after it.
        # The linker drops both when it drops the object.
        start = s[1]
        later = [v for v in by_sec[s[5]] if v > start]
        end = min(later) if later else elf.sh[s[5]][5]
        out[i] = (start, max(end, start + s[2]))
    return out


def _in(ranges: list[tuple[int, int]], off: int) -> bool:
    return any(a <= off < b for a, b in ranges)


def compute_exclusions(elf: _Elf, unit_ranges, symbol_addrs: dict[str, int],
                       this_unit: str | None) -> tuple[dict[int, list[tuple[int, int]]], list[str], set[int]]:
    """(section -> excluded ranges, descriptions, excluded weak symbol indices)."""
    excl: dict[int, list[tuple[int, int]]] = {}
    notes: list[str] = []
    weak: set[int] = set()
    if this_unit is None:
        return excl, notes, weak
    ranges = _sym_ranges(elf)
    for i, s in enumerate(elf.syms):
        if (s[3] >> 4) != STB_WEAK or i not in ranges:
            continue
        name = elf.symnames[i]
        addr = symbol_addrs.get(name)
        if addr is None:
            continue
        owner = owner_of(unit_ranges, addr)
        if owner is None or owner == this_unit:
            continue
        a, b = ranges[i]
        excl.setdefault(s[5], []).append((a, b))
        weak.add(i)
        notes.append(f"{elf.names[s[5]]} {name} 0x{b - a:X} (weak; retail keeps the copy in {owner})")
    if not weak:
        return excl, notes, weak

    # Every reloc as (source section, offset, target section, target offset).
    refs = []
    for ri, tgt_sec in elf.rela_target.items():
        for off, info, addend in elf.relas(ri):
            sym = elf.syms[info >> 8] if (info >> 8) < len(elf.syms) else None
            if sym is None or sym[5] in (SHN_UNDEF, SHN_ABS, SHN_COMMON):
                continue
            refs.append((tgt_sec, off, sym[5], sym[1] + addend))

    def excluded(sec: int, off: int) -> bool:
        return _in(excl.get(sec, []), off)

    # extabindex triples whose function lies in an excluded range.
    idx = next((i for i, n in enumerate(elf.names) if n == "extabindex"), None)
    tab = next((i for i, n in enumerate(elf.names) if n == "extab"), None)
    if idx is not None:
        for src, off, tsec, toff in refs:
            if src == idx and off % 12 == 0 and excluded(tsec, toff):
                excl.setdefault(idx, []).append((off, off + 12))
                notes.append(f"extabindex entry +0x{off:X} (record of an excluded function)")
    if idx is not None and tab is not None:
        starts = sorted({toff for src, off, tsec, toff in refs if src == idx and tsec == tab and off % 12 == 8})
        bounds = starts + [elf.sh[tab][5]]
        for k, start in enumerate(starts):
            users = [(src, off) for src, off, tsec, toff in refs if src == idx and tsec == tab and toff == start]
            if users and all(excluded(src, off) for src, off in users):
                excl.setdefault(tab, []).append((start, bounds[k + 1]))
                notes.append(f"extab record +0x{start:X} 0x{bounds[k + 1] - start:X} (only an excluded function uses it)")

    # Dead-strip: local non-.text data referenced only from excluded ranges.
    changed = True
    while changed:
        changed = False
        for i, s in enumerate(elf.syms):
            if (s[3] >> 4) != STB_LOCAL or i not in ranges or i in weak:
                continue
            sec = s[5]
            if elf.names[sec] == ".text" or elf.names[sec].startswith(".rela"):
                continue
            a, b = ranges[i]
            if b <= a or _in(excl.get(sec, []), a):
                continue
            users = [(src, off) for src, off, tsec, toff in refs if tsec == sec and a <= toff < b]
            if users and all(excluded(src, off) for src, off in users):
                excl.setdefault(sec, []).append((a, b))
                notes.append(f"{elf.names[sec]} {elf.symnames[i]} 0x{b - a:X} (only excluded symbols reference it)")
                changed = True
    for sec in excl:
        excl[sec] = _merge(excl[sec])
    return excl, notes, weak


def _merge(rs: list[tuple[int, int]]) -> list[tuple[int, int]]:
    out: list[tuple[int, int]] = []
    for a, b in sorted(rs):
        if out and a <= out[-1][1]:
            out[-1] = (out[-1][0], max(out[-1][1], b))
        else:
            out.append((a, b))
    return out


def _mapper(rs: list[tuple[int, int]]):
    def m(off: int) -> int:
        cut = 0
        for a, b in rs:
            if off >= b:
                cut += b - a
            elif off > a:
                cut += off - a
        return off - cut
    return m


def rewrite(elf: _Elf, excl: dict[int, list[tuple[int, int]]], weak: set[int]) -> bytes:
    maps = {sec: _mapper(rs) for sec, rs in excl.items()}
    contents = list(elf.contents)
    sizes = [h[5] for h in elf.sh]
    for sec, rs in excl.items():
        removed = sum(b - a for a, b in rs)
        sizes[sec] -= removed
        if elf.sh[sec][1] != SHT_NOBITS:
            src = contents[sec]
            keep, pos = [], 0
            for a, b in rs:
                keep.append(src[pos:a])
                pos = b
            keep.append(src[pos:])
            contents[sec] = b"".join(keep)
    # Symbols.
    dead_local: set[int] = set()
    ranges = _sym_ranges(elf)
    syms = [list(s) for s in elf.syms]
    for i, s in enumerate(syms):
        if i in weak:
            s[1], s[2], s[5] = 0, 0, SHN_UNDEF
        elif s[5] in excl:
            if i in ranges and _in(excl[s[5]], ranges[i][0]) and (s[3] & 0xF) != STT_SECTION:
                dead_local.add(i)
                s[1], s[2], s[5] = 0, 0, SHN_UNDEF
            else:
                s[1] = maps[s[5]](s[1])
    if elf.symtab is not None:
        contents[elf.symtab] = b"".join(struct.pack(">IIIBBH", *s) for s in syms)
    # Relocations.
    for ri, tgt in elf.rela_target.items():
        out = []
        for off, info, addend in elf.relas(ri):
            if tgt in excl and _in(excl[tgt], off):
                continue
            new_off = maps[tgt](off) if tgt in excl else off
            si = info >> 8
            osym = elf.syms[si] if si < len(elf.syms) else None
            if osym is not None and si not in weak and si not in dead_local and osym[5] in excl:
                # Re-base the addend so value + addend still lands on the same byte.
                addend = maps[osym[5]](osym[1] + addend) - maps[osym[5]](osym[1])
            out.append(struct.pack(">IIi", new_off, info, addend))
        contents[ri] = b"".join(out)
        sizes[ri] = len(contents[ri])
    # Lay the file out again: header, section data, section header table.
    hdr = bytearray(elf.data[:0x34])
    body = bytearray()
    sh = [list(h) for h in elf.sh]
    pos = 0x34
    for i, h in enumerate(sh):
        h[5] = sizes[i]
        if i == 0:
            continue
        if h[1] == SHT_NOBITS:
            h[4] = pos
            continue
        align = max(h[8], 1)
        pad = (-pos) % align
        body += b"\0" * pad
        pos += pad
        h[4] = pos
        body += contents[i]
        pos += len(contents[i])
    pad = (-pos) % 4
    body += b"\0" * pad
    pos += pad
    struct.pack_into(">I", hdr, 0x20, pos)
    struct.pack_into(">H", hdr, 0x2E, 40)
    table = b"".join(struct.pack(">IIIIIIIIII", *h) for h in sh)
    return bytes(hdr + body + table)


_VIEW_DIR = Path(tempfile.gettempdir()) / "xeno_weak_dedup"


def linked_view(decomp_object: Path, project_root: Path, region: str, unit_hint: str) -> LinkedView:
    """The decomp object as mwld would contribute it to the link (see module doc)."""
    decomp_object = Path(decomp_object)
    try:
        unit_ranges, symbol_addrs = _layout(Path(project_root), region)
    except OSError:
        return LinkedView(decomp_object)
    return linked_view_for(decomp_object, unit_ranges, symbol_addrs,
                           resolve_unit(unit_ranges, unit_hint))


def linked_view_for(decomp_object: Path, unit_ranges, symbol_addrs: dict[str, int],
                    this_unit: str | None) -> LinkedView:
    """:func:`linked_view` with explicit layout inputs (tests)."""
    data = decomp_object.read_bytes()
    elf = _Elf(data)
    excl, notes, weak = compute_exclusions(elf, unit_ranges, symbol_addrs, this_unit)
    if not weak:
        return LinkedView(decomp_object)
    digest = hashlib.sha1(data + this_unit.encode()).hexdigest()[:16]
    _VIEW_DIR.mkdir(parents=True, exist_ok=True)
    out = _VIEW_DIR / f"{decomp_object.stem}.{digest}.o"
    if not out.is_file():
        tmp = out.with_suffix(f".{os.getpid()}.tmp")
        tmp.write_bytes(rewrite(elf, excl, weak))
        tmp.replace(out)
    removed = {elf.names[s]: sum(b - a for a, b in rs) for s, rs in excl.items()}
    return LinkedView(out, notes, removed)

