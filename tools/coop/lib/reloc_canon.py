"""Canonical relocation-target resolution (UNIT_RULES category A).

The retail split objects under ``build/<region>/obj/`` are dtk/ppcdis
*reconstructions* of the original compiler output.  Their relocation symbol
spellings, section alignment and linker-script-only symbols are splitter
artifacts: two relocations that name different symbols can still resolve to the
same linked value, and a relocation whose value the retail side simply baked
can be equivalent to a decomp relocation the linker resolves at link time.

This module answers the one question every representation-level comparison
needs: **what does this relocation resolve to in the linked image?**

* ``symbols.txt`` gives every named symbol's section + absolute address.
* ``splits.txt`` gives every output section's address range, so a TU-local
  symbol (``@N`` pool entries, ``$N`` statics) can be resolved against its
  section base even though it has no global name.
* ``ldscript.lcf`` gives the linker-script-only absolutes (``_stack_addr``,
  ``_db_stack_end``, ``__ArenaLo``, ...) as formulas over ``_f_<section>`` /
  ``SIZEOF(.section)``.

The module also computes the *immediate* a given ``R_PPC_ADDR16_*`` relocation
installs, which is what lets the comparator equate a decomp ``lis/addi`` pair
(the linker resolves it) with the retail split's baked immediate (the splitter
already resolved it).

Pure stdlib; the CLI (``python3 tools/coop/lib/reloc_canon.py --check``)
verifies the linker absolutes that the retired ``bake_linker_addrs`` /
``force_symbol_relocs`` UNIT_RULES entries used to hard-code.
"""

from __future__ import annotations

import argparse
import re
import sys
import tempfile
from dataclasses import dataclass, field
from pathlib import Path

# ── relocation types (PowerPC EABI + MW EMB) ────────────────────────────────
R_PPC_ADDR32 = 1
R_PPC_ADDR24 = 2
R_PPC_ADDR16 = 3
R_PPC_ADDR16_LO = 4
R_PPC_ADDR16_HI = 5
R_PPC_ADDR16_HA = 6
R_PPC_REL24 = 10
R_PPC_UADDR32 = 24
R_PPC_UADDR16 = 25
R_PPC_EMB_SDA21 = 109

#: Canonical classes: two relocations only compare equal inside one class.
ADDR16_HA, ADDR16_HI, ADDR16_LO = "ADDR16_HA", "ADDR16_HI", "ADDR16_LO"
_TYPE_CLASS = {
    R_PPC_ADDR32: "ADDR32",
    R_PPC_UADDR32: "ADDR32",
    R_PPC_ADDR24: "ADDR24",
    R_PPC_ADDR16: "ADDR16",
    R_PPC_ADDR16_LO: ADDR16_LO,
    R_PPC_ADDR16_HI: ADDR16_HI,
    R_PPC_ADDR16_HA: ADDR16_HA,
    R_PPC_UADDR16: "ADDR16",
    R_PPC_REL24: "REL24",
    R_PPC_EMB_SDA21: "SDA21",
}

#: Relocation types whose in-place field is a 16-bit address immediate.
ADDR16_TYPES = frozenset({R_PPC_ADDR16, R_PPC_ADDR16_LO, R_PPC_ADDR16_HI, R_PPC_ADDR16_HA})


def type_class(relocation_type: int) -> str:
    """Canonical class name for a relocation type (unknown -> ``R_<n>``)."""
    return _TYPE_CLASS.get(relocation_type, f"R_{relocation_type}")


def addr16_immediate(addr: int, relocation_type: int) -> int | None:
    """The 16-bit immediate an ``R_PPC_ADDR16_*`` relocation installs for *addr*.

    ``None`` when *relocation_type* is not an ADDR16-class relocation.
    """
    if relocation_type not in ADDR16_TYPES:
        return None
    addr &= 0xFFFFFFFF
    if relocation_type == R_PPC_ADDR16_LO:
        return addr & 0xFFFF
    if relocation_type == R_PPC_ADDR16_HI:
        return (addr >> 16) & 0xFFFF
    if relocation_type == R_PPC_ADDR16_HA:
        return ((addr + 0x8000) >> 16) & 0xFFFF
    return addr & 0xFFFF  # R_PPC_ADDR16 is LO for these targets


# ── layouts ────────────────────────────────────────────────────────────────


class LinkerLayoutError(RuntimeError):
    """Malformed or missing layout input."""


@dataclass(frozen=True)
class SectionRange:
    name: str
    start: int
    end: int

    @property
    def size(self) -> int:
        return self.end - self.start


@dataclass(frozen=True)
class CanonTarget:
    """One reloc target resolved to its linked value."""

    name: str
    section: str | None
    addr: int | None
    #: "symbol" | "linker-absolute" | "section-relative" | "unknown"
    kind: str

    def key(self, relocation_type: int) -> tuple:
        """Comparison key: same class + same resolved address => equal."""
        return (type_class(relocation_type), self.section, self.addr, self.kind == "unknown")

    def immediate(self, relocation_type: int) -> int | None:
        """The immediate the linker installs for this target, when resolvable."""
        if self.addr is None:
            return None
        return addr16_immediate(self.addr, relocation_type)


_SPLIT_SECTION_RE = re.compile(
    r"^\s*(?P<name>\.?[A-Za-z_][\w.]*)\s+start:(?P<start>0x[0-9A-Fa-f]+)\s+end:(?P<end>0x[0-9A-Fa-f]+)\s*$"
)
_SYMBOL_RE = re.compile(
    r"^(?P<name>\S+)\s*=\s*(?P<section>\S+?):(?P<addr>0x[0-9A-Fa-f]+)\s*;"
)
_ASSIGN_RE = re.compile(r"^\s*([A-Za-z_][A-Za-z0-9_]*)\s*=\s*([^;]+);")
_IDENT_RE = re.compile(r"[A-Za-z_][A-Za-z0-9_.]*")
_SIZEOF_RE = re.compile(r"SIZEOF\(\s*([^)]+?)\s*\)")


#: The linker-script absolutes dtk writes into ``build/<region>/ldscript.lcf``.
#: Copied verbatim from the generated file so the check can also run where the
#: (gitignored) build tree is absent, e.g. CI over the committed splits.txt.
#: The build tree's lcf always wins when it exists.
DEFAULT_LCF_TEMPLATE = """
MEMORY
{
    text : origin = 0x80004000
}

SECTIONS
{
    _stack_end = _f_sbss2 + SIZEOF(.sbss2);
    _stack_addr = (_stack_end + 0xFFFF + 0x7) & ~0x7;
    _db_stack_addr = (_stack_addr + 0x2000);
    _db_stack_end = _stack_addr;
    __ArenaLo = (_db_stack_addr + 0x1f) & ~0x1f;
    __ArenaHi = 0x81700000;
}
"""


@dataclass
class LinkerLayout:
    """Region layout: named symbols, section ranges, linker-script absolutes."""

    region: str
    root: Path
    symbols: dict[str, tuple[str, int]] = field(default_factory=dict)
    sections: dict[str, SectionRange] = field(default_factory=dict)
    linker_absolutes: dict[str, int] = field(default_factory=dict)
    lcf_source: str = ""

    # -- constructors --------------------------------------------------------

    @classmethod
    def load(cls, root: Path | str, region: str = "us") -> "LinkerLayout":
        root = Path(root)
        layout = cls(region=region, root=root)
        splits = root / "config" / region / "splits.txt"
        symbols = root / "config" / region / "symbols.txt"
        if symbols.is_file():
            layout.symbols = parse_symbols(symbols)
        if splits.is_file():
            layout.sections = parse_splits(splits)
        else:
            raise LinkerLayoutError(f"missing {splits}")
        lcf = root / "build" / region / "ldscript.lcf"
        if lcf.is_file():
            layout.lcf_source = str(lcf)
            layout.linker_absolutes = eval_ldscript(lcf, layout.sections)
        else:
            layout.lcf_source = "<built-in template>"
            with tempfile.NamedTemporaryFile("w", suffix=".lcf", delete=False) as fh:
                fh.write(DEFAULT_LCF_TEMPLATE)
                tmp = Path(fh.name)
            try:
                layout.linker_absolutes = eval_ldscript(tmp, layout.sections)
            finally:
                tmp.unlink(missing_ok=True)
        return layout

    # -- resolution ----------------------------------------------------------

    def section_start(self, section: str) -> int | None:
        rng = self.sections.get(section)
        return rng.start if rng else None

    def resolve_name(self, name: str) -> tuple[str | None, int | None]:
        """Resolve a global/linker symbol name to (section, absolute address)."""
        if name in self.linker_absolutes:
            return None, self.linker_absolutes[name]
        hit = self.symbols.get(name)
        if hit is not None:
            return hit[0], hit[1]
        return None, None

    def resolve_local(self, section: str | None, value: int) -> int | None:
        """Resolve a section-relative symbol value (ET_REL st_value)."""
        if section is None:
            return None
        base = self.section_start(section)
        if base is None:
            return None
        return base + value

    def canon_reloc(
        self,
        symbol: str | None,
        addend: int | None = 0,
        *,
        section: str | None = None,
        value: int | None = None,
    ) -> CanonTarget:
        """Canonical target for a reloc on *symbol*.

        *section*/*value* describe a symbol **defined in the object** (its home
        section and section-relative ``st_value``).  The object's own definition
        wins over the global ``symbols.txt`` map: TU-local ``@N`` / ``$N`` names
        collide across units, so ``symbols.txt`` must not answer for them.  The
        definition only resolves to an absolute address when *section* has a
        known base; otherwise the caller keeps a section-relative key.  Global
        (UNDEF or linker-script) names resolve through ``symbols.txt`` / the
        ldscript; when nothing resolves the name is kept and the comparator
        falls back to name equality (fail-closed).
        """
        addend = addend or 0
        if not symbol or symbol in ("(null)",):
            if value is not None:
                addr = self.resolve_local(section, value)
                return CanonTarget(symbol or "", section, addr, "section-relative")
            return CanonTarget(symbol or "", section, None, "unknown")
        if value is not None:
            addr = self.resolve_local(section, value)
            if addr is not None:
                return CanonTarget(symbol, section, addr + addend, "section-relative")
            sec, gaddr = self.resolve_name(symbol)
            if gaddr is not None:
                return CanonTarget(symbol, sec, gaddr + addend, "symbol")
            return CanonTarget(symbol, section, None, "section-relative")
        sec, addr = self.resolve_name(symbol)
        if addr is not None:
            kind = "linker-absolute" if sec is None and symbol in self.linker_absolutes else "symbol"
            return CanonTarget(symbol, sec, addr + addend, kind)
        return CanonTarget(symbol, section, None, "unknown")


# ── parsers ────────────────────────────────────────────────────────────────


def parse_splits(path: Path) -> dict[str, SectionRange]:
    """Section ranges (min start / max end) from dtk ``splits.txt``."""
    out: dict[str, SectionRange] = {}
    for line in path.read_text(encoding="utf-8", errors="replace").splitlines():
        m = _SPLIT_SECTION_RE.match(line)
        if not m:
            continue
        name = m.group("name")
        if not name.startswith("."):
            continue
        start, end = int(m.group("start"), 16), int(m.group("end"), 16)
        cur = out.get(name)
        if cur is None:
            out[name] = SectionRange(name, start, end)
        else:
            out[name] = SectionRange(name, min(cur.start, start), max(cur.end, end))
    return out


def parse_symbols(path: Path) -> dict[str, tuple[str, int]]:
    """``name = .section:0xADDR; // ...`` map from dtk ``symbols.txt``."""
    out: dict[str, tuple[str, int]] = {}
    for line in path.read_text(encoding="utf-8", errors="replace").splitlines():
        m = _SYMBOL_RE.match(line)
        if not m:
            continue
        out[m.group("name")] = (m.group("section"), int(m.group("addr"), 16))
    return out


def _split_expr(expr: str) -> list[str]:
    """Tokenize a linker-script arithmetic expression."""
    return re.findall(r"SIZEOF\([^)]*\)|[A-Za-z_][A-Za-z0-9_.]*|0[xX][0-9A-Fa-f]+|\d+|[()+\-*/&|~<>]", expr)


def _eval_tokens(tokens: list[str], variables: dict[str, int], sections: dict[str, SectionRange]) -> int:
    """Evaluate +, -, *, &, |, ~ over numbers and known names."""
    out: list[str] = []
    for tok in tokens:
        if tok.startswith("SIZEOF("):
            sec = tok[len("SIZEOF("):-1].strip()
            rng = sections.get(sec)
            if rng is None:
                raise KeyError(sec)
            out.append(str(rng.size))
            continue
        ident = _IDENT_RE.fullmatch(tok)
        if ident and not re.fullmatch(r"0[xX][0-9A-Fa-f]+|\d+", tok):
            name = tok
            if name.startswith(("_f_", "_e_")) and name[3:].lstrip(".") in {
                s.lstrip(".") for s in sections
            }:
                sec = next(k for k in sections if k.lstrip(".") == name[3:].lstrip("."))
                out.append(str(sections[sec].start if name[1] == "f" else sections[sec].end))
            elif name in variables:
                out.append(str(variables[name]))
            else:
                raise KeyError(name)
            continue
        out.append(tok)
    expr = " ".join(out)
    if not re.fullmatch(r"[0-9A-Fa-fxX\s+\-*/&|~()]+", expr):
        raise ValueError(expr)
    return int(eval(expr, {"__builtins__": {}}, {})) & 0xFFFFFFFF  # noqa: S307 - token whitelist above


def eval_ldscript(path: Path, sections: dict[str, SectionRange]) -> dict[str, int]:
    """Evaluate the linker-script absolute assignments (``NAME = EXPR;``).

    Only assignments whose expression is fully resolvable are kept; block
    directives (``GROUP:``, ``FORCEACTIVE`` entries, ``origin = ...`` inside
    MEMORY) simply fail to evaluate and are skipped.
    """
    variables: dict[str, int] = {}
    for line in path.read_text(encoding="utf-8", errors="replace").splitlines():
        if line.lstrip().startswith("#"):
            continue
        m = _ASSIGN_RE.match(line)
        if not m:
            continue
        name, expr = m.group(1), m.group(2)
        try:
            variables[name] = _eval_tokens(_split_expr(expr), variables, sections)
        except (KeyError, ValueError, SyntaxError):
            continue
    return variables


# ── known retail absolutes (the retired bake_linker_addrs targets) ──────────
#: Every value the retired ``bake_linker_addrs`` / ``force_symbol_relocs``
#: UNIT_RULES entries hard-coded, as the *retail* link resolves them.  The
#: check below re-evaluates the ldscript formulas and fails when the layout
#: drifts, so the honest link stays loud instead of silently baking a stale
#: address.  See docs/evidence/decomp/unit_rules_category_a.md.
RETAIL_LINKER_ABSOLUTES: dict[str, int] = {
    "_stack_addr": 0x8067B560,     # OSThread.o / __start.o bake_linker_addrs
    "_db_stack_end": 0x8067B560,   # OS.o bake_linker_addrs
    "__ArenaLo": 0x8067D560,       # OS.o bake_linker_addrs
    "memcpy": 0x80004000,          # OS.o force_symbol_relocs
}


def verify_known_absolutes(layout: LinkerLayout) -> list[str]:
    """Return mismatches between the ldscript/retail layout and the baked values."""
    problems: list[str] = []
    for name, expected in sorted(RETAIL_LINKER_ABSOLUTES.items()):
        _, actual = layout.resolve_name(name)
        if actual is None:
            problems.append(f"{name}: unresolved (expected 0x{expected:08X})")
        elif actual != expected:
            problems.append(
                f"{name}: resolves to 0x{actual:08X}, expected 0x{expected:08X}")
    return problems


def _default_root() -> Path:
    return Path(__file__).resolve().parents[3]


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("--region", default="us")
    parser.add_argument("--root", default=None, help="repository root (default: auto)")
    parser.add_argument("--check", action="store_true",
                        help="verify the retired bake targets still resolve as retail")
    parser.add_argument("--resolve", metavar="SYMBOL", action="append", default=[],
                        help="print the resolved address of a symbol")
    args = parser.parse_args(argv)

    root = Path(args.root).resolve() if args.root else _default_root()
    try:
        layout = LinkerLayout.load(root, args.region)
    except LinkerLayoutError as exc:
        print(f"ERROR: {exc}", file=sys.stderr)
        return 2
    for name in args.resolve:
        sec, addr = layout.resolve_name(name)
        if addr is None:
            print(f"{name}: unresolved")
        else:
            print(f"{name} = {sec or '(absolute)'}:0x{addr:08X}")
    if args.check or not args.resolve:
        problems = verify_known_absolutes(layout)
        if problems:
            print(f"linker-absolute check FAILED ({args.region}):")
            for p in problems:
                print(f"  {p}")
            return 1
        print(
            f"linker-absolute check OK ({args.region}, layout from {layout.lcf_source}): "
            f"{len(RETAIL_LINKER_ABSOLUTES)} retired bake targets resolve as retail"
        )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
