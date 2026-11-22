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
  the copies mwld discarded.  Weak symbols that retail places in this unit
  (or whose address belongs to no split unit) are not deduplicated.
* A function is **dead-stripped** when mwld would have garbage-collected it:
  it is defined in the object (any binding), its name has **no retail copy
  anywhere** in ``config/<region>/symbols.txt``, and no relocation from a
  retained range reaches it (computed to a fixpoint, so a dead function does
  not keep its callees alive; a function's own ``extabindex``/``extab``
  records do not count as references to it).  These are the functions retail
  inlined into every caller and the compiler-generated destructors nothing
  calls.  Because a decomp function may simply be *spelled* differently from
  its retail symbol, the rule only runs in a unit where **every** retail
  function that ``symbols.txt`` places in the unit's ``.text`` range is
  defined by name in the object: then no retail function is left for an
  unknown name to stand for.  A function that is referenced from retained
  code or data, or whose name (or a ``name__suffix`` spelling of it) exists
  in ``symbols.txt``, is never dead-stripped.
* A global/weak data object (a vtable, say) with no retail name goes with the
  dead functions when it is referenced and **every** reference comes from an
  excluded range, and every retail symbol in this unit's slice of that data
  section is spelled the same way in the object (otherwise the object may
  simply name retail's copy differently).  Such data does not keep a function alive either: a
  destructor named only by a vtable that only dead constructors install is
  dead too.  A ``__vt__`` object with no retail name that **nothing** in the
  object references is dropped as well (an abstract base's vtable emitted at
  its key function; no constructor installs it).  **Local** data of a dead function stays, as it does in retail
  (the statics and literals of an SDK function the game never calls are still
  in the retail slice).
* A symbol the compiler marked **force-active** (``#pragma force_active`` /
  ``FORCEACTIVE``; the ``.comment`` section's per-symbol active flag) is a
  root, as it is for mwld: it is never excluded.
* The exception records (``extabindex``/``extab``) of an excluded weak function
  go with it.  **Local/anonymous data stays by default.**  A local object that
  every relocation references only from excluded copies is dropped only when
  the retail object proves it absent: its bytes do not appear at the offset the
  object would occupy in the linked section (allowing for preceding exclusions).
  Retail often keeps RTTI name strings and base lists even though the weak RTTI
  locator that named them was deduplicated to another unit; dropping those
  locals would shrink the retail slice (CfBdat's ``@8669``/``@8668``).  With no
  retail object available the rule is deliberately conservative: keep the data.

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


# -- retail evidence -----------------------------------------------------------

_RETAIL_DATA_SECTIONS = (".data", ".rodata", ".sdata", ".sdata2",
                         ".bss", ".sbss", ".sbss2")
_RETAIL_CACHE: dict[tuple, dict[str, bytes]] = {}


def _retail_object(project_root: Path, region: str, unit_hint: str) -> Path | None:
    """Split retail object for *unit_hint*, if it has been extracted."""
    rel = _norm_unit(unit_hint)
    path = project_root / "build" / region / "obj" / f"{rel}.o"
    return path if path.is_file() else None


class RetailSections(dict):
    """Retail data-section bytes plus, per section, its relocation sites.

    ``relocs[name]`` is the set of ``(offset, reloc type)`` pairs the retail
    split object relocates in that section.  Bytes alone are not evidence: a
    relocated pointer word is zero in both objects whatever it points at, so
    two different objects can share their unrelocated bytes.
    """

    def __init__(self, data=(), relocs=None):
        super().__init__(data)
        self.relocs: dict[str, set[tuple[int, int]]] = relocs if relocs is not None else {}


def _reloc_sites(elf: "_Elf") -> dict[int, set[tuple[int, int]]]:
    """Section index -> {(offset, reloc type)} of every relocation in it."""
    out: dict[int, set[tuple[int, int]]] = {}
    for ri, tgt in elf.rela_target.items():
        sites = out.setdefault(tgt, set())
        for off, info, _addend in elf.relas(ri):
            sites.add((off, info & 0xFF))
    return out


def _retail_sections(path: Path | None) -> RetailSections | None:
    """Section bytes + reloc sites of the retail split object (evidence for local cascade)."""
    if path is None or not path.is_file():
        return None
    try:
        st = path.stat()
        key = (str(path), st.st_mtime_ns, st.st_size)
        hit = _RETAIL_CACHE.get(key)
        if hit is None:
            elf = _Elf(path.read_bytes())
            sites = _reloc_sites(elf)
            hit = RetailSections(
                {name: elf.contents[i] for i, name in enumerate(elf.names)
                 if name in _RETAIL_DATA_SECTIONS},
                {name: sites.get(i, set()) for i, name in enumerate(elf.names)
                 if name in _RETAIL_DATA_SECTIONS})
            _RETAIL_CACHE.clear()
            _RETAIL_CACHE[key] = hit
        return hit
    except OSError:
        return None


def _retail_keeps(retail_sections: dict[str, bytes] | None, elf: _Elf,
                  sec: int, a: int, b: int,
                  excl: dict[int, list[tuple[int, int]]]) -> tuple[bool, int]:
    """Does retail still carry this local object at its linked offset?

    ``excl`` holds the ranges removed *before* this candidate was considered,
    so the expected retail offset is the raw offset minus those bytes.  No
    retail object at all is not evidence; an absent section in a known retail
    object means a zero-size section, i.e. the local was not kept.
    """
    cut = sum(min(end, a) - start for start, end in excl.get(sec, ()) if start < a)
    linked = a - cut
    if retail_sections is None:
        return True, linked
    name = elf.names[sec]
    data = retail_sections.get(name, b"")
    if linked + (b - a) > len(data):
        return False, linked
    if data[linked:linked + (b - a)] != elf.contents[sec][a:b]:
        return False, linked
    retail_relocs = getattr(retail_sections, "relocs", None)
    if retail_relocs is not None:
        # Same bytes but relocated at different words (or with a different
        # reloc type) is a different object: e.g. a 12-byte RTTI base list
        # {type*, 0, 0} against a vtable {RTTI*, 0, dtor*}.
        ours = {(off - a, t) for off, t in _reloc_sites(elf).get(sec, ()) if a <= off < b}
        theirs = {(off - linked, t) for off, t in retail_relocs.get(name, ())
                  if linked <= off < linked + (b - a)}
        if ours != theirs:
            return False, linked
    return True, linked


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


def _force_active(elf: _Elf) -> set[int]:
    """Symbol indices MWCC flagged force-active in ``.comment`` (never link-dead)."""
    ci = next((i for i, n in enumerate(elf.names) if n == ".comment"), None)
    if ci is None:
        return set()
    raw = elf.contents[ci]
    if len(raw) < 0x2C or raw[:11] != b"CodeWarrior":
        return set()
    head = 0x2C  # fixed MWCC .comment header, then 8 bytes per symbol (align:4, vis:1, active:1, pad:2)
    out = set()
    for i in range(1, len(elf.syms)):
        off = head + 8 * i
        if off + 8 > len(raw):
            break
        if raw[off + 5] & 0x08:
            out.add(i)
    return out


def _has_retail_name(name: str, symbol_addrs: dict[str, int]) -> bool:
    """True if *name*, or a ``retail__suffix`` spelling of it, is in symbols.txt.

    Conservative on purpose: a hand-spelled flat symbol such as
    ``Foo__3BarFv__FP3Bar`` stands for retail ``Foo__3BarFv`` and must never be
    treated as link-dead.
    """
    if name in symbol_addrs:
        return True
    pos = name.find("__", 1)
    while pos != -1:
        if name[:pos] in symbol_addrs:
            return True
        pos = name.find("__", pos + 1)
    return False


def _all_retail_symbols_defined(elf: _Elf, unit_ranges, symbol_addrs: dict[str, int],
                                this_unit: str, section: str = ".text") -> bool:
    """Every retail symbol in this unit's *section* range is defined by name here.

    With no ``.text`` range there is nothing to compare against, so the answer
    is no.  A data section the unit has no split for holds no retail symbol
    that could be this object's under another name, so the answer is yes.
    """
    text = [(a, b) for u, sec, a, b in unit_ranges if u == this_unit and sec == section]
    if not text:
        return section != ".text"
    defined = {elf.symnames[i] for i, s in enumerate(elf.syms)
               if s[5] not in (SHN_UNDEF, SHN_ABS, SHN_COMMON) and elf.symnames[i]}
    flat = set()
    for name in defined:
        pos = name.find("__", 1)
        while pos != -1:
            flat.add(name[:pos])
            pos = name.find("__", pos + 1)
    for name, addr in symbol_addrs.items():
        if _in(text, addr) and name not in defined and name not in flat:
            # A switch table's retail placeholder (``jumptable_<region>_<addr>``) names a
            # compiler-anonymous local (``@N``) the object can never spell the same way;
            # it cannot be a global object of this unit under another name.
            if section != ".text" and name.startswith("jumptable_"):
                continue
            return False
    return True


def _dead_strip(elf: _Elf, ranges, unit_ranges, symbol_addrs: dict[str, int], this_unit: str,
                excl: dict[int, list[tuple[int, int]]], weak: set[int]) -> list[int]:
    """Function symbol indices mwld would garbage-collect (see module doc), in symbol order."""
    if not _all_retail_symbols_defined(elf, unit_ranges, symbol_addrs, this_unit):
        return []
    named_cache: dict[str, bool] = {}

    def named(section: str) -> bool:
        if section not in named_cache:
            named_cache[section] = _all_retail_symbols_defined(elf, unit_ranges, symbol_addrs, this_unit, section)
        return named_cache[section]

    active = _force_active(elf)
    not_data = ("extab", "extabindex", ".ctors", ".dtors", ".comment", ".symtab", ".strtab", ".shstrtab")
    cands: dict[int, tuple[int, int, int]] = {}
    # Data that can only pass liveness on (never a root unless nothing names it):
    # global/weak data objects with no retail name.  Local data is always live.
    transit: set[int] = set()
    for i, s in enumerate(elf.syms):
        if i not in ranges or i in weak or i in active or s[5] >= elf.shnum:
            continue
        name = elf.symnames[i]
        sec_name = elf.names[s[5]]
        a, b = ranges[i]
        if not name or b <= a or _in(excl.get(s[5], []), a):
            continue
        typ, bind = s[3] & 0xF, s[3] >> 4
        if sec_name == ".text":
            if typ != 2 or _has_retail_name(name, symbol_addrs):  # STT_FUNC only
                continue
        else:
            if sec_name.startswith(".rela") or sec_name in not_data:
                continue
            if bind == STB_LOCAL or typ != 1 or _has_retail_name(name, symbol_addrs):
                continue
            if not named(elf.names[s[5]]):
                continue
            transit.add(i)
        cands[i] = (s[5], a, b)
    # A vtable is only ever named by its class's constructors and destructors.
    # One that no relocation in its own (key-function) unit names and that has
    # no retail symbol belongs to a class retail never constructs out of line
    # (an abstract base whose derived constructors are all inlined): mwld drops
    # it.  Unlike other nameless data it is not "left alone" below.
    orphan_vt = {i for i in transit if elf.symnames[i].startswith("__vt__")}
    if len(transit) == len(cands) and not orphan_vt:
        return []
    if not cands:
        return []

    by_sec: dict[int, list[tuple[int, int, int]]] = {}
    for i, (sec, a, b) in cands.items():
        by_sec.setdefault(sec, []).append((a, b, i))

    def cand_at(sec: int, off: int) -> int | None:
        for a, b, i in by_sec.get(sec, ()):
            if a <= off < b:
                return i
        return None

    refs = []  # (source section, source offset, target section, target offset)
    for ri, src_sec in elf.rela_target.items():
        for off, info, addend in elf.relas(ri):
            sym = elf.syms[info >> 8] if (info >> 8) < len(elf.syms) else None
            if sym is None or sym[5] in (SHN_UNDEF, SHN_ABS, SHN_COMMON):
                continue
            refs.append((src_sec, off, sym[5], sym[1] + addend))

    # A function owns its extabindex triple and the extab record that triple names.
    idx = next((i for i, n in enumerate(elf.names) if n == "extabindex"), None)
    tab = next((i for i, n in enumerate(elf.names) if n == "extab"), None)
    entry_owner: dict[int, int] = {}
    record_owner: list[tuple[int, int]] = []  # (record start, owning candidate or -1)
    if idx is not None:
        for src, off, tsec, toff in refs:
            if src == idx and off % 12 == 0:
                owner = cand_at(tsec, toff)
                entry_owner[off // 12] = -1 if owner is None else owner
        if tab is not None:
            for src, off, tsec, toff in refs:
                if src == idx and off % 12 == 8 and tsec == tab:
                    record_owner.append((toff, entry_owner.get(off // 12, -1)))
            record_owner.sort()

    LIVE, DEAD = -1, -2

    def source_owner(sec: int, off: int) -> int:
        """LIVE, DEAD, or the candidate index whose liveness this source inherits."""
        if _in(excl.get(sec, []), off):
            return DEAD
        if sec == idx:
            return entry_owner.get(off // 12, LIVE)
        if sec == tab and record_owner:
            owner = LIVE
            for start, who in record_owner:
                if start > off:
                    break
                owner = who
            return owner
        c = cand_at(sec, off)
        return LIVE if c is None else c

    edges: dict[int, set[int]] = {}
    live: set[int] = set()
    named: set[int] = set()
    for src, off, tsec, toff in refs:
        target = cand_at(tsec, toff)
        if target is None:
            continue
        if src == idx and off % 12 == 0:
            continue  # the function's own exception-table index entry
        owner = source_owner(src, off)
        if owner == target:
            continue
        named.add(target)
        if owner == DEAD:
            continue
        if owner == LIVE:
            live.add(target)
        else:
            edges.setdefault(owner, set()).add(target)
    orphan_vt -= named
    live |= transit - named - orphan_vt  # data nothing names is left alone (and keeps its referents)
    work = list(live)
    while work:
        for nxt in edges.get(work.pop(), ()):
            if nxt not in live:
                live.add(nxt)
                work.append(nxt)
    return [i for i in cands if i not in live and (i not in transit or i in orphan_vt)]


def compute_exclusions(elf: _Elf, unit_ranges, symbol_addrs: dict[str, int],
                       this_unit: str | None,
                       retail_sections: dict[str, bytes] | None = None
                       ) -> tuple[dict[int, list[tuple[int, int]]], list[str], set[int]]:
    """(section -> excluded ranges, descriptions, excluded weak symbol indices).

    *retail_sections* is the retail object's data-section bytes, used as the
    evidence that a local object referenced only by deduplicated weak copies
    really was dropped by mwld.  Without it those locals are kept.
    """
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
        if owner == this_unit:
            continue
        if owner is None:
            # The retail copy lives outside every split (an unassigned
            # auto_* object), so it is still not this unit's: mwld kept that
            # copy and discarded ours.
            owner = f"an unsplit retail object at 0x{addr:08X}"
        a, b = ranges[i]
        excl.setdefault(s[5], []).append((a, b))
        weak.add(i)
        notes.append(f"{elf.names[s[5]]} {name} 0x{b - a:X} (weak; retail keeps the copy in {owner})")

    # Every reloc as (source section, offset, target section, target offset).
    refs = []
    for ri, tgt_sec in elf.rela_target.items():
        for off, info, addend in elf.relas(ri):
            sym = elf.syms[info >> 8] if (info >> 8) < len(elf.syms) else None
            if sym is None or sym[5] in (SHN_UNDEF, SHN_ABS, SHN_COMMON):
                continue
            refs.append((tgt_sec, off, sym[5], sym[1] + addend))
    active = _force_active(elf)

    # Weak copies mwld discarded take their exception records and the local
    # data only they reference (RTTI name strings) with them.
    if weak:
        _cascade(elf, ranges, refs, excl, notes, weak, active, symbol_addrs,
                 retail_sections, local=True)

    # Functions mwld garbage-collected take their exception records and any
    # nameless global/weak data object (a vtable) only they reference.  Local
    # data stays: retail keeps the statics and literals of a stripped function.
    dead = _dead_strip(elf, ranges, unit_ranges, symbol_addrs, this_unit, excl, weak)
    for i in dead:
        s = elf.syms[i]
        a, b = ranges[i]
        excl.setdefault(s[5], []).append((a, b))
        notes.append(f"{elf.names[s[5]]} {elf.symnames[i]} 0x{b - a:X} "
                     f"(dead-strip; nothing retained references it and retail has no copy)")
    if dead:
        _cascade(elf, ranges, refs, excl, notes, weak, active, symbol_addrs, local=False,
                 named=lambda sec: _all_retail_symbols_defined(elf, unit_ranges, symbol_addrs, this_unit, sec))
    for sec in excl:
        excl[sec] = _merge(excl[sec])
    return excl, notes, weak


def _cascade(elf: _Elf, ranges, refs, excl, notes: list[str], weak: set[int], active: set[int],
             symbol_addrs: dict[str, int], retail_sections: dict[str, bytes] | None = None,
             *, local: bool, named=None) -> None:
    """Exclude what only excluded ranges use: extab records, then data.

    ``local=True`` keeps a local object unless the retail object proves it
    absent at its linked offset; ``local=False`` strips only global/weak data
    objects that have no retail name, and only in a section where
    ``named(section)`` holds (every retail symbol of this unit's slice of that
    section is spelled the same way here, so "no retail name" means "not in
    retail" rather than "named differently").
    """
    def excluded(sec: int, off: int) -> bool:
        return _in(excl.get(sec, []), off)

    # extabindex triples whose function lies in an excluded range.
    idx = next((i for i, n in enumerate(elf.names) if n == "extabindex"), None)
    tab = next((i for i, n in enumerate(elf.names) if n == "extab"), None)
    if idx is not None:
        for src, off, tsec, toff in refs:
            if src == idx and off % 12 == 0 and excluded(tsec, toff) and not excluded(idx, off):
                excl.setdefault(idx, []).append((off, off + 12))
                notes.append(f"extabindex entry +0x{off:X} (record of an excluded function)")
    if idx is not None and tab is not None:
        starts = sorted({toff for src, off, tsec, toff in refs if src == idx and tsec == tab and off % 12 == 8})
        bounds = starts + [elf.sh[tab][5]]
        for k, start in enumerate(starts):
            if excluded(tab, start):
                continue
            users = [(src, off) for src, off, tsec, toff in refs if src == idx and tsec == tab and toff == start]
            if users and all(excluded(src, off) for src, off in users):
                excl.setdefault(tab, []).append((start, bounds[k + 1]))
                notes.append(f"extab record +0x{start:X} 0x{bounds[k + 1] - start:X} (only an excluded function uses it)")

    changed = True
    while changed:
        changed = False
        cands = []
        for i, s in enumerate(elf.syms):
            if i not in ranges or i in weak or i in active or s[5] >= elf.shnum:
                continue
            is_local = (s[3] >> 4) == STB_LOCAL
            if is_local != local:
                continue
            if not is_local and ((s[3] & 0xF) != 1 or _has_retail_name(elf.symnames[i], symbol_addrs)):
                continue
            sec = s[5]
            if elf.names[sec] == ".text" or elf.names[sec].startswith(".rela"):
                continue
            if not is_local and named is not None and not named(elf.names[sec]):
                continue
            a, b = ranges[i]
            if b <= a or _in(excl.get(sec, []), a):
                continue
            cands.append((sec, a, b, i))
        # Section order matters: dropping one local shifts the linked offset
        # (and therefore the retail evidence) of every local after it.
        for sec, a, b, i in sorted(cands, key=lambda c: (c[0], c[1])):
            users = [(src, off) for src, off, tsec, toff in refs if tsec == sec and a <= toff < b]
            if not (users and all(excluded(src, off) for src, off in users)):
                continue
            if local:
                keeps, linked = _retail_keeps(retail_sections, elf, sec, a, b, excl)
                if keeps:
                    continue
                reason = (f"only excluded symbols reference it; retail lacks those bytes at +0x{linked:X}")
            else:
                reason = "only excluded symbols reference it"
            excl.setdefault(sec, []).append((a, b))
            notes.append(f"{elf.names[sec]} {elf.symnames[i]} 0x{b - a:X} ({reason})")
            changed = True


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


def _sym_aligns(elf: _Elf) -> dict[int, int]:
    """Per-symbol alignment MWCC recorded in ``.comment`` (symbol index -> bytes)."""
    ci = next((i for i, n in enumerate(elf.names) if n == ".comment"), None)
    if ci is None:
        return {}
    raw = elf.contents[ci]
    if len(raw) < 0x2C or raw[:11] != b"CodeWarrior":
        return {}
    out = {}
    for i in range(1, len(elf.syms)):
        off = 0x2C + 8 * i
        if off + 8 > len(raw):
            break
        out[i] = struct.unpack_from(">I", raw, off)[0]
    return out


def _keep_alignment(elf: _Elf, excl: dict[int, list[tuple[int, int]]]):
    """Trim excluded data ranges so the object after each one stays aligned.

    mwld places every surviving object at its own alignment, so dropping a
    12-byte neighbour in front of an 8-aligned string table leaves 4 bytes of
    padding behind.  Returns (ranges, zero) where ``zero`` lists the bytes of
    an excluded range that are kept as that padding.
    """
    aligns = _sym_aligns(elf)
    starts: dict[int, dict[int, int]] = {}
    for i, sy in enumerate(elf.syms):
        if sy[5] >= elf.shnum or (sy[3] & 0xF) in (STT_SECTION, STT_FILE):
            continue
        al = aligns.get(i, 1)
        if al > 1:
            d = starts.setdefault(sy[5], {})
            d[sy[1]] = max(d.get(sy[1], 1), al)
    out: dict[int, list[tuple[int, int]]] = {}
    zero: dict[int, list[tuple[int, int]]] = {}
    for sec, rs in excl.items():
        if elf.names[sec] == ".text" or sec not in starts:
            out[sec] = rs
            continue
        cut, new = 0, []
        for a, b in _merge(rs):
            al = 1 if any(a2 <= b < b2 for a2, b2 in rs) else starts[sec].get(b, 1)
            pad = (-(b - cut - (b - a))) % al
            if pad and pad <= b - a:
                zero.setdefault(sec, []).append((b - pad, b))
                b -= pad
            if b > a:
                new.append((a, b))
            cut += b - a
        out[sec] = new
    return out, zero


def rewrite(elf: _Elf, excl: dict[int, list[tuple[int, int]]], weak: set[int]) -> bytes:
    full = excl
    excl, zero = _keep_alignment(elf, excl)
    maps = {sec: _mapper(rs) for sec, rs in excl.items()}
    contents = list(elf.contents)
    for sec, zs in zero.items():
        if elf.sh[sec][1] != SHT_NOBITS:
            buf = bytearray(contents[sec])
            for a, b in zs:
                buf[a:b] = bytes(b - a)
            contents[sec] = bytes(buf)
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
            if i in ranges and _in(full[s[5]], ranges[i][0]) and (s[3] & 0xF) != STT_SECTION:
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
            if tgt in full and _in(full[tgt], off):
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
# Bump when rewrite() changes what it writes for the same exclusions, or when
# the exclusion rule changes.
_REWRITE_VERSION = "4:reloc-aware-retail-evidence"


def linked_view(decomp_object: Path, project_root: Path, region: str, unit_hint: str,
                *, retail_object: Path | None = None) -> LinkedView:
    """The decomp object as mwld would contribute it to the link (see module doc).

    The retail split object supplies the byte/size evidence for local objects
    that only deduplicated weak copies reference.  Pass *retail_object* when
    the caller has it; otherwise the conventional split-object path is used.
    """
    decomp_object = Path(decomp_object)
    try:
        project_root = Path(project_root)
        unit_ranges, symbol_addrs = _layout(project_root, region)
    except OSError:
        return LinkedView(decomp_object)
    if retail_object is None:
        retail_object = _retail_object(project_root, region, unit_hint)
    return linked_view_for(decomp_object, unit_ranges, symbol_addrs,
                           resolve_unit(unit_ranges, unit_hint),
                           retail_sections=_retail_sections(retail_object))


def linked_view_for(decomp_object: Path, unit_ranges, symbol_addrs: dict[str, int],
                    this_unit: str | None,
                    *, retail_sections: dict[str, bytes] | None = None) -> LinkedView:
    """:func:`linked_view` with explicit layout inputs (tests)."""
    data = decomp_object.read_bytes()
    elf = _Elf(data)
    excl, notes, weak = compute_exclusions(elf, unit_ranges, symbol_addrs, this_unit,
                                           retail_sections)
    if not excl:
        return LinkedView(decomp_object)
    # The exclusions and the retail evidence are part of the key: a rule change
    # must not reuse a view written under the old rule.
    evidence = repr(sorted((s, len(b), hashlib.sha1(b).hexdigest()[:8])
                           for s, b in (retail_sections or {}).items()))
    evidence += repr(sorted((s, sorted(r)) for s, r in
                            getattr(retail_sections, "relocs", {}).items()))
    key = (_REWRITE_VERSION
           + repr(sorted((s, tuple(rs)) for s, rs in excl.items()))
           + repr(sorted(weak)) + evidence)
    digest = hashlib.sha1(data + str(this_unit).encode() + key.encode()).hexdigest()[:16]

    out = _VIEW_DIR / f"{decomp_object.stem}.{digest}.o"
    if not out.is_file():
        tmp = out.with_suffix(f".{os.getpid()}.tmp")
        tmp.write_bytes(rewrite(elf, excl, weak))
        tmp.replace(out)
    removed = {elf.names[s]: sum(b - a for a, b in rs) for s, rs in excl.items()}
    return LinkedView(out, notes, removed)

