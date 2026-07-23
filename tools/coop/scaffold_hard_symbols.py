#!/usr/bin/env python3
"""Scaffold the remaining hard catalog symbols (sinit / templates / @unnamed@).

After ``scaffold_catalog_tus.py``, ~200 targets remain because their linker names
are not valid C identifiers for ``extern "C"`` stubs (``<>``, ``@unnamed@``), or
because ``sinit_*`` was skipped by default.

This tool appends high-level C++ stubs into the already-created TU files:

- ``sinit_*`` / ``__sinit_*`` → ``extern "C" void sinit_…() {}``
- ``CTTask<T>::{Move,Draw,~CTTask}`` → explicit template specializations
- ``@unnamed@`` helpers → anonymous-namespace C++ (same pattern as nw4r headers)
- simple ``FixStr`` / ``CMsgParam`` / ``reslist`` template members when parseable

Usage:
  python3 tools/coop/scaffold_hard_symbols.py
  python3 tools/coop/scaffold_hard_symbols.py --tu kyoshin/CTaskGameEff --write --verify
  python3 tools/coop/scaffold_hard_symbols.py --write
"""
from __future__ import annotations

import argparse
import re
import subprocess
import sys
from collections import defaultdict
from dataclasses import dataclass, field
from pathlib import Path
from typing import Dict, List, Optional, Sequence, Set, Tuple

ROOT = Path(__file__).resolve().parents[2]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from tools.coop.lib.config import load_config
from tools.coop.lib.targets import (
    Target,
    load_targets,
    load_targets_document,
    write_targets_document,
)
from tools.coop.scaffold_catalog_tus import (
    _parse_configure_object_paths,
    resolve_source_path,
    verify_units,
)
from tools.coop.scaffold_missing_slots import (
    _format_stub,
    _infer_return_type,
    begin_marker,
    end_marker,
)
from tools.symbolrecover.lib.mwcc import KNOWN_NS_PREFIXES

_CTTASK_RE = re.compile(
    r"^(?P<method>__dt|Move|Draw)__\d+CTTask<(?P<targ>[^>]+)>Fv$"
)
_SIMPLE_UNNAMED_RE = re.compile(
    r"^(?P<name>[A-Za-z_][A-Za-z0-9_]*)__(?P<len>\d+)@unnamed@(?P<file>[^@]+)@"
    r"(?P<sig>F[A-Za-z0-9]*)$"
)
_SINIT_RE = re.compile(r"^(?:__)?sinit_[A-Za-z0-9_]+$")
_FIXSTR_RE = re.compile(r"^(?P<method>[A-Za-z_][A-Za-z0-9_]*)__Q22ml\d+FixStr<(?P<n>\d+)>(?P<sig>F.*)$")
_MSGPARAM_RE = re.compile(
    r"^(?P<method>__ct|__dt|[A-Za-z_][A-Za-z0-9_]*)__\d+CMsgParam<(?P<n>\d+)>(?P<sig>F.*)$"
)
_RESLIST_RE = re.compile(
    r"^(?P<method>__dt)__\d+(?P<cls>_reslist_base|reslist)<(?P<targ>[^>]+)>Fv$"
)


@dataclass
class HardStub:
    target_id: str
    symbol: str
    kind: str
    text: str
    forward_decls: Tuple[str, ...] = ()
    # For unnamed_q coalescing: (ns_parts, after_parts, method_name, args, ctor, dtor)
    unnamed_meta: Optional[Tuple[Tuple[str, ...], Tuple[str, ...], str, str, bool, bool]] = None


@dataclass
class UnitHardPlan:
    unit: str
    source_path: Path
    stubs: List[HardStub] = field(default_factory=list)
    skips: List[Tuple[str, str]] = field(default_factory=list)  # id, reason


def _unit_matches(unit: Optional[str], needle: str) -> bool:
    if not unit:
        return False
    u = unit.removeprefix("main/")
    n = needle.removeprefix("main/")
    return u == n or u.endswith("/" + n) or n.endswith("/" + u)


def _read_component(blob: str, pos: int) -> Tuple[Optional[str], int]:
    """Length-prefixed MWCC component; allows leading ``@`` (unnamed namespaces)."""
    if pos >= len(blob) or not blob[pos].isdigit():
        return None, pos
    num_start = pos
    while pos < len(blob) and blob[pos].isdigit():
        pos += 1
    digit_run = blob[num_start:pos]
    for end in range(1, len(digit_run) + 1):
        length = int(digit_run[:end])
        name_start = num_start + end
        if name_start + length <= len(blob):
            name = blob[name_start : name_start + length]
            if name and (name[0].isalpha() or name[0] in "_.@"):
                return name, name_start + length
    return None, num_start


def decode_type_name(blob: str) -> Optional[str]:
    """Decode a length-prefixed or Q-encoded type to a C++ name."""
    blob = blob.strip()
    if not blob:
        return None
    for prefix, namespace in KNOWN_NS_PREFIXES:
        if blob.startswith(prefix):
            cls, _ = _read_component(blob, len(prefix))
            if cls:
                return f"{namespace}::{cls}"
    # MWCC uses a single-digit Q-count (Q2…Q9). ``Q54nw4r…`` is Q5 + ``4nw4r``.
    if blob.startswith("Q") and len(blob) > 1 and blob[1].isdigit():
        n = int(blob[1])
        pos = 2
        parts: List[str] = []
        for _ in range(n):
            part, pos = _read_component(blob, pos)
            if part is None:
                return None
            if not part.startswith("@unnamed@"):
                parts.append(part)
        return "::".join(parts) if parts else None
    name, _ = _read_component(blob, 0)
    return name


def decode_cttask_targ(targ: str) -> str:
    decoded = decode_type_name(targ)
    if decoded:
        return decoded
    # Plain length-prefixed class: 12CTaskGameEff
    name, _ = _read_component(targ, 0)
    return name or targ


def _parse_q_path_and_sig(rest: str) -> Optional[Tuple[List[str], str]]:
    # Single-digit Q-count only (see decode_type_name).
    if not rest.startswith("Q") or len(rest) < 2 or not rest[1].isdigit():
        return None
    n = int(rest[1])
    pos = 2
    parts: List[str] = []
    for _ in range(n):
        part, pos = _read_component(rest, pos)
        if part is None:
            return None
        parts.append(part)
    return parts, rest[pos:]


def decode_simple_sig(sig: str) -> Optional[str]:
    """Return ``(args)`` for a small set of common F-signatures."""
    table = {
        "Fv": "()",
        "Fi": "(int)",
        "Ff": "(float)",
        "Fd": "(double)",
        "Fb": "(bool)",
        "FUl": "(unsigned long)",
        "FUi": "(unsigned int)",
        "FPv": "(void*)",
        "FPCv": "(const void*)",
        "FPc": "(char*)",
        "FPCc": "(const char*)",
    }
    return table.get(sig)


def decode_args_blob(sig: str) -> Optional[Tuple[str, Set[str]]]:
    """Best-effort MWCC argument decode → (cpp_args, forward_decl_types).

    Returns None when the signature is too complex for a scaffold stub.
    """
    if not sig.startswith("F"):
        return None
    simple = decode_simple_sig(sig)
    if simple is not None:
        return simple, set()

    blob = sig[1:]
    args: List[str] = []
    decls: Set[str] = set()
    i = 0

    def take_type(start: int) -> Tuple[Optional[str], int]:
        j = start
        prefix = ""
        while j < len(blob) and blob[j] in "PRC":
            ch = blob[j]
            if ch == "P":
                prefix = "*" + prefix if not prefix.startswith("&") else prefix
            elif ch == "R":
                prefix = "&" + prefix
            elif ch == "C":
                prefix = "const " + prefix if "const" not in prefix else prefix
            j += 1
        if j >= len(blob):
            return None, start
        if blob[j] == "Q" or blob[j].isdigit():
            # type name consumes the rest of a length-prefixed / Q token
            # Find a maximal valid decode by trying Q first then length-prefixed.
            if blob[j] == "Q":
                parsed = _parse_q_path_and_sig(blob[j:])
                if parsed is None:
                    return None, start
                parts, rem = parsed
                consumed = len(blob[j:]) - len(rem)
                named = [p for p in parts if not p.startswith("@unnamed@")]
                if not named:
                    return None, start
                typ = "::".join(named)
                decls.add(typ)
                if "&" in prefix and "const" in prefix:
                    cpp = f"const {typ}&"
                elif "*" in prefix:
                    stars = prefix.count("*")
                    cv = "const " if "const" in prefix else ""
                    cpp = f"{cv}{typ}{'*' * stars}"
                elif "&" in prefix:
                    cpp = f"{typ}&"
                else:
                    cpp = typ
                return cpp, j + consumed
            name, end = _read_component(blob, j)
            if name is None:
                return None, start
            decls.add(name)
            if "const" in prefix and "&" in prefix:
                return f"const {name}&", end
            if "&" in prefix:
                return f"{name}&", end
            stars = prefix.count("*")
            cv = "const " if "const" in prefix else ""
            return f"{cv}{name}{'*' * stars}", end
        prim = {
            "v": "void",
            "b": "bool",
            "c": "char",
            "s": "short",
            "i": "int",
            "l": "long",
            "x": "long long",
            "f": "float",
            "d": "double",
            "w": "wchar_t",
        }
        if blob.startswith("Ul", j):
            return ("unsigned long" + ("*" * prefix.count("*"))), j + 2
        if blob.startswith("Ui", j):
            return ("unsigned int" + ("*" * prefix.count("*"))), j + 2
        if blob.startswith("Us", j):
            return ("unsigned short" + ("*" * prefix.count("*"))), j + 2
        if blob.startswith("Uc", j):
            return ("unsigned char" + ("*" * prefix.count("*"))), j + 2
        if blob[j] in prim:
            t = prim[blob[j]]
            if "const" in prefix and "&" in prefix:
                return f"const {t}&", j + 1
            return t + ("*" * prefix.count("*")), j + 1
        return None, start

    while i < len(blob):
        typ, ni = take_type(i)
        if typ is None or ni == i:
            return None
        if typ != "void":
            args.append(typ)
        i = ni
    return "(" + ", ".join(args) + ")", decls


def _wrap_markers(target_id: str, body: str) -> str:
    return f"{begin_marker(target_id)}\n{body.rstrip()}\n{end_marker(target_id)}\n"


def stub_sinit(target: Target) -> HardStub:
    text = _format_stub(
        target_id=target.id,
        qualified_name=target.symbol,
        args="()",
        return_type="void",
        is_c_linkage=True,
        symbol=target.symbol,
        force_mangled_symbol=True,
        c_source=False,
    )
    return HardStub(target.id, target.symbol, "sinit", text)


def stub_cttask(target: Target) -> Optional[HardStub]:
    m = _CTTASK_RE.match(target.symbol)
    if not m:
        return None
    method = m.group("method")
    targ = decode_cttask_targ(m.group("targ"))
    # Forward-declare the derived type; keep CTTask local to avoid monolib inline clash.
    fwd = []
    if "::" in targ:
        ns, cls = targ.rsplit("::", 1)
        fwd.append(f"namespace {ns.split('::')[0]} {{ " + " ".join(
            f"namespace {p} {{" for p in ns.split("::")[1:]
        ) + f" class {cls}; " + ("} " * (len(ns.split('::'))-1)) + "}")
        # Simpler forward decl form:
        fwd = []
        parts = targ.split("::")
        decl = ""
        for i, p in enumerate(parts):
            if i < len(parts) - 1:
                decl += f"namespace {p} {{ "
            else:
                decl += f"class {p}; "
        decl += "}" * (len(parts) - 1)
        fwd = [decl] if len(parts) > 1 else [f"class {targ};"]
    else:
        fwd = [f"class {targ};"]

    if method == "__dt":
        body = f"template<> CTTask<{targ}>::~CTTask() {{}}"
    elif method == "Move":
        body = f"template<> void CTTask<{targ}>::Move() {{}}"
    else:
        body = f"template<> void CTTask<{targ}>::Draw() {{}}"
    text = _wrap_markers(target.id, body)
    return HardStub(target.id, target.symbol, "cttask", text, tuple(fwd))


def stub_simple_unnamed(target: Target) -> Optional[HardStub]:
    m = _SIMPLE_UNNAMED_RE.match(target.symbol)
    if not m:
        return None
    args = decode_simple_sig(m.group("sig"))
    if args is None:
        return None
    name = m.group("name")
    body = f"namespace {{\nvoid {name}{args} {{}}\n}}"
    return HardStub(target.id, target.symbol, "unnamed_file", _wrap_markers(target.id, body))


def stub_q_unnamed(target: Target) -> Optional[HardStub]:
    """Emit stubs for Func__Qn…@unnamed@… paths (and nested class helpers)."""
    sym = target.symbol
    if "@unnamed@" not in sym:
        return None
    if sym.startswith("__ct__"):
        func = "__ct"
        rest = sym[len("__ct__") :]
        is_ctor, is_dtor = True, False
    elif sym.startswith("__dt__"):
        func = "__dt"
        rest = sym[len("__dt__") :]
        is_ctor, is_dtor = False, True
    else:
        if "__" not in sym:
            return None
        func, rest = sym.split("__", 1)
        is_ctor = is_dtor = False
    parsed = _parse_q_path_and_sig(rest)
    if parsed is None:
        return None
    parts, sig = parsed
    unnamed_idx = next((i for i, p in enumerate(parts) if p.startswith("@unnamed@")), None)
    if unnamed_idx is None:
        return None

    decoded = decode_args_blob(sig if sig.startswith("F") else "F" + sig)
    if decoded is None:
        # Fall back to void() only for ctors/dtors with Fv
        if sig in {"Fv", ""} or sig == "Fv":
            args, decls = "()", set()
        else:
            return None
    else:
        args, decls = decoded

    ns_parts = tuple(p for p in parts[:unnamed_idx] if not p.startswith("@unnamed@"))
    after = tuple(p for p in parts[unnamed_idx + 1 :] if not p.startswith("@unnamed@"))

    # Types used by value need a complete dummy definition (forward decl is not
    # enough for MWCC). Pointer/ref args can stay as forward decls — handled in
    # _render_unnamed_q_groups via decls_complete.
    fwd_lines = []
    for typ in sorted(decls):
        bits = typ.split("::")
        # Prefer a tiny complete type so by-value args compile.
        if len(bits) == 1:
            fwd_lines.append(f"struct {bits[0]} {{ char _pad; }};")
        else:
            s = "".join(f"namespace {b} {{ " for b in bits[:-1])
            s += f"struct {bits[-1]} {{ char _pad; }}; " + ("}" * (len(bits) - 1))
            fwd_lines.append(s)

    # Method body only — class declarations are coalesced per-unit in _render_append.
    if after:
        cls = after[-1]
        if is_ctor:
            body = f"{cls}::{cls}{args} {{}}"
        elif is_dtor:
            body = f"{cls}::~{cls}() {{}}"
        else:
            body = f"void {cls}::{func}{args} {{}}"
    else:
        if is_ctor or is_dtor:
            return None
        body = f"void {func}{args} {{}}"

    return HardStub(
        target.id,
        target.symbol,
        "unnamed_q",
        _wrap_markers(target.id, body),
        tuple(fwd_lines),
        unnamed_meta=(ns_parts, after, func, args, is_ctor, is_dtor),
    )


def stub_fixstr(target: Target) -> Optional[HardStub]:
    m = _FIXSTR_RE.match(target.symbol)
    if not m:
        return None
    # format__Q22ml10FixStr<16>FPCce — demangle often broken; use varargs-safe stub
    n = m.group("n")
    method = m.group("method")
    if method != "format":
        return None
    body = (
        f"namespace ml {{\n"
        f"template <int N> class FixStr {{ public: void format(const char*, ...); }};\n"
        f"template <> void FixStr<{n}>::format(const char*, ...) {{}}\n"
        f"}}"
    )
    return HardStub(target.id, target.symbol, "fixstr", _wrap_markers(target.id, body))


def stub_msgparam(target: Target) -> Optional[HardStub]:
    m = _MSGPARAM_RE.match(target.symbol)
    if not m:
        return None
    n = m.group("n")
    method = m.group("method")
    sig = m.group("sig")
    # Emit specialization only; class template is coalesced in _render_append.
    if method == "__ct":
        body = f"template <> CMsgParam<{n}>::CMsgParam() {{}}"
    elif method == "__dt":
        body = f"template <> CMsgParam<{n}>::~CMsgParam() {{}}"
    else:
        decoded = decode_simple_sig(sig)
        if decoded is None:
            return None
        body = f"template <> void CMsgParam<{n}>::{method}{decoded} {{}}"
    return HardStub(
        target.id,
        target.symbol,
        "msgparam",
        _wrap_markers(target.id, body),
        (f"msgparam<{n}>",),  # marker for coalesced preamble
    )


def stub_reslist(target: Target) -> Optional[HardStub]:
    m = _RESLIST_RE.match(target.symbol)
    if not m:
        return None
    cls = m.group("cls")
    raw_targ = m.group("targ")
    fwd: List[str] = []
    if raw_targ.startswith("P"):
        inner = decode_type_name(raw_targ[1:])
        if not inner:
            return None
        targ_cpp = f"{inner}*"
        bits = inner.split("::")
        if len(bits) > 1:
            fwd.append(
                "".join(f"namespace {b} {{ " for b in bits[:-1])
                + f"struct {bits[-1]} {{ char _pad; }}; "
                + ("}" * (len(bits) - 1))
            )
        else:
            fwd.append(f"struct {inner} {{ char _pad; }};")
    else:
        targ_cpp = decode_cttask_targ(raw_targ)
        bits = targ_cpp.split("::")
        if len(bits) > 1:
            fwd.append(
                "".join(f"namespace {b} {{ " for b in bits[:-1])
                + f"struct {bits[-1]} {{ char _pad; }}; "
                + ("}" * (len(bits) - 1))
            )
        else:
            fwd.append(f"struct {targ_cpp} {{ char _pad; }};")

    if cls == "_reslist_base":
        body = f"template <> _reslist_base<{targ_cpp}>::~_reslist_base() {{}}"
        fwd.append("reslist_base_tpl")
    else:
        body = f"template <> reslist<{targ_cpp}>::~reslist() {{}}"
        fwd.append("reslist_tpl")
    return HardStub(
        target.id, target.symbol, "reslist", _wrap_markers(target.id, body), tuple(fwd)
    )


def plan_target(target: Target) -> Tuple[Optional[HardStub], Optional[str]]:
    sym = target.symbol
    if _SINIT_RE.match(sym):
        return stub_sinit(target), None
    if "CTTask<" in sym:
        stub = stub_cttask(target)
        return (stub, None) if stub else (None, "cttask_parse")
    if "@unnamed@" in sym:
        stub = stub_simple_unnamed(target) or stub_q_unnamed(target)
        return (stub, None) if stub else (None, "unnamed_complex")
    if "FixStr<" in sym:
        stub = stub_fixstr(target)
        return (stub, None) if stub else (None, "fixstr_parse")
    if "CMsgParam<" in sym:
        stub = stub_msgparam(target)
        return (stub, None) if stub else (None, "msgparam_parse")
    if "reslist" in sym and "<" in sym:
        stub = stub_reslist(target)
        return (stub, None) if stub else (None, "reslist_parse")
    if "sort<" in sym or "__sort" in sym:
        return None, "std_sort"
    if "<" in sym:
        return None, "template_other"
    return None, "unsupported"


_CTTASK_PREAMBLE = """\
// Local CTTask (out-of-line Move/Draw/dtor) for harness stubs.
// Do not include monolib/work/CTTask.hpp here — its inline methods collide.
template <typename T>
class CTTask {
public:
    CTTask();
    virtual ~CTTask();
    virtual void Move();
    virtual void Draw();
};
"""


def _is_catalog_scaffold(source: Path) -> bool:
    try:
        # Cheap check: catalog scaffolder stamps this banner.
        with source.open(encoding="utf-8", errors="ignore") as fh:
            head = fh.read(400)
        return "Auto-scaffolded catalog TU" in head
    except OSError:
        return False


def collect_hard_targets(
    targets: Sequence[Target],
    *,
    tu: Optional[str],
    prefix: Optional[str],
) -> List[Target]:
    """Select hard symbols that still need stubs.

    Only touches **catalog-scaffolded** TUs (or not-buildable rows). Never appends
    into hand-written / already-matched sources.
    """
    selected: List[Target] = []
    for target in targets:
        if target.kind != "function" or target.status != "NOT_STARTED":
            continue
        if not target.symbol or not target.unit:
            continue
        if not (
            _SINIT_RE.match(target.symbol)
            or "<" in target.symbol
            or "@" in target.symbol
        ):
            continue
        unit = target.unit.removeprefix("main/")
        if tu is not None and not _unit_matches(unit, tu):
            continue
        if prefix is not None and not (
            unit == prefix or unit.startswith(prefix.rstrip("/") + "/")
        ):
            continue
        if target.source is not None and target.source.is_file():
            if not _is_catalog_scaffold(target.source):
                continue
            try:
                text = target.source.read_text(encoding="utf-8")
            except OSError:
                continue
            if begin_marker(target.id) in text:
                continue
        selected.append(target)
    return selected


def build_plans(
    targets: Sequence[Target],
    *,
    project_root: Path,
    object_by_unit: Dict[str, str],
) -> List[UnitHardPlan]:
    by_unit: Dict[str, List[Target]] = defaultdict(list)
    for target in targets:
        by_unit[target.unit.removeprefix("main/")].append(target)

    plans: List[UnitHardPlan] = []
    for unit in sorted(by_unit):
        object_path = object_by_unit.get(unit)
        if object_path is None:
            plan = UnitHardPlan(unit, project_root / "MISSING")
            for t in by_unit[unit]:
                plan.skips.append((t.id, "no_configure_object"))
            plans.append(plan)
            continue
        source_path = resolve_source_path(project_root, object_path)
        plan = UnitHardPlan(unit, source_path)
        if source_path.suffix.lower() == ".c":
            for t in by_unit[unit]:
                plan.skips.append((t.id, "c_tu_hard_symbol"))
            plans.append(plan)
            continue
        for target in sorted(by_unit[unit], key=lambda t: t.id):
            if begin_marker(target.id) and source_path.is_file():
                existing = source_path.read_text(encoding="utf-8")
                if begin_marker(target.id) in existing:
                    plan.skips.append((target.id, "markers_exist"))
                    continue
            stub, reason = plan_target(target)
            if stub is None:
                plan.skips.append((target.id, reason or "unsupported"))
            else:
                plan.stubs.append(stub)
        plans.append(plan)
    return plans


def _render_unnamed_q_groups(stubs: Sequence[HardStub]) -> str:
    """Coalesce nested @unnamed@ class stubs so each class is defined once."""
    groups: Dict[
        Tuple[Tuple[str, ...], Tuple[str, ...]],
        List[HardStub],
    ] = defaultdict(list)
    free: List[HardStub] = []
    seen_fwd: Set[str] = set()
    fwd_block: List[str] = []

    for stub in stubs:
        for fwd in stub.forward_decls:
            if fwd not in seen_fwd:
                fwd_block.append(fwd)
                seen_fwd.add(fwd)
        meta = stub.unnamed_meta
        if meta is None:
            free.append(stub)
            continue
        ns_parts, after, _func, _args, _ctor, _dtor = meta
        if after:
            groups[(ns_parts, after)].append(stub)
        else:
            free.append(stub)

    chunks: List[str] = []
    for fwd in fwd_block:
        chunks.append(fwd + "\n")

    for (ns_parts, after), members in sorted(groups.items(), key=lambda kv: kv[0]):
        lines: List[str] = []
        indent = 0

        def emit(s: str) -> None:
            lines.append(("    " * indent) + s)

        for ns in ns_parts:
            emit(f"namespace {ns} {{")
            indent += 1
        emit("namespace {")
        indent += 1
        for ns in after[:-1]:
            emit(f"namespace {ns} {{")
            indent += 1
        cls = after[-1]
        emit(f"class {cls} {{")
        indent += 1
        emit("public:")
        for stub in members:
            assert stub.unnamed_meta is not None
            _ns, _after, func, args, is_ctor, is_dtor = stub.unnamed_meta
            if is_ctor:
                emit(f"{cls}{args};")
            elif is_dtor:
                emit(f"~{cls}();")
            else:
                emit(f"void {func}{args};")
        indent -= 1
        emit("};")
        for stub in members:
            # Keep markers around each out-of-line definition.
            body = stub.text
            # stub.text already includes markers + method body.
            # Indent method bodies for readability.
            indented = "\n".join(
                (("    " * indent) + ln if ln.strip() else ln)
                for ln in body.splitlines()
            )
            lines.append(indented)
        for _ in after[:-1]:
            indent -= 1
            emit("}")
        indent -= 1
        emit("}")
        for _ in ns_parts:
            indent -= 1
            emit("}")
        chunks.append("\n".join(lines) + "\n")

    for stub in free:
        # File-scope-ish unnamed function under namespaces.
        meta = stub.unnamed_meta
        if meta is None:
            chunks.append(stub.text)
            continue
        ns_parts, _after, func, args, is_ctor, is_dtor = meta
        lines = []
        indent = 0

        def emit2(s: str) -> None:
            lines.append(("    " * indent) + s)

        for ns in ns_parts:
            emit2(f"namespace {ns} {{")
            indent += 1
        emit2("namespace {")
        indent += 1
        # Markers wrap only the function.
        emit2(begin_marker(stub.target_id))
        emit2(f"void {func}{args} {{}}")
        emit2(end_marker(stub.target_id))
        indent -= 1
        emit2("}")
        for _ in ns_parts:
            indent -= 1
            emit2("}")
        chunks.append("\n".join(lines) + "\n")

    return "".join(chunks)


def _render_append(plan: UnitHardPlan) -> str:
    needs_cttask = any(s.kind == "cttask" for s in plan.stubs)
    chunks: List[str] = ["\n// --- hard-symbol stubs (scaffold_hard_symbols) ---\n"]
    if needs_cttask:
        chunks.append(_CTTASK_PREAMBLE + "\n")
    seen_fwd: Set[str] = set()
    unnamed_q = [s for s in plan.stubs if s.kind == "unnamed_q"]
    other = [s for s in plan.stubs if s.kind != "unnamed_q"]

    msg_ns = sorted(
        {
            fwd[len("msgparam<") : -1]
            for s in other
            if s.kind == "msgparam"
            for fwd in s.forward_decls
            if fwd.startswith("msgparam<")
        }
    )
    if msg_ns:
        # One class template; declare all methods used by specializations.
        methods: Dict[str, Set[str]] = defaultdict(set)  # n -> method decls
        for s in other:
            if s.kind != "msgparam":
                continue
            m = _MSGPARAM_RE.match(s.symbol)
            if not m:
                continue
            n = m.group("n")
            method = m.group("method")
            sig = m.group("sig")
            if method == "__ct":
                methods[n].add(f"CMsgParam();")
            elif method == "__dt":
                methods[n].add(f"~CMsgParam();")
            else:
                decoded = decode_simple_sig(sig) or "()"
                methods[n].add(f"void {method}{decoded};")
        chunks.append("template <int N> class CMsgParam {\npublic:\n")
        # Union of decls across N (MWCC needs members on the primary template).
        all_decls: Set[str] = set()
        for decls in methods.values():
            all_decls |= decls
        for d in sorted(all_decls):
            chunks.append(f"    {d}\n")
        chunks.append("};\n")

    # Type dummies first (needed by reslist / CTTask specializations).
    for stub in other:
        for fwd in stub.forward_decls:
            if fwd.startswith("msgparam<") or fwd in {"reslist_base_tpl", "reslist_tpl"}:
                continue
            if fwd not in seen_fwd:
                chunks.append(fwd + "\n")
                seen_fwd.add(fwd)

    if any(
        s.kind == "reslist" and "reslist_base_tpl" in s.forward_decls for s in other
    ):
        chunks.append(
            "template <typename T> class _reslist_base { public: ~_reslist_base(); };\n"
        )
    if any(s.kind == "reslist" and "reslist_tpl" in s.forward_decls for s in other):
        chunks.append(
            "template <typename T> class reslist { public: ~reslist(); };\n"
        )

    for stub in other:
        chunks.append(stub.text)
        if not stub.text.endswith("\n"):
            chunks.append("\n")
    if unnamed_q:
        chunks.append(_render_unnamed_q_groups(unnamed_q))
    return "".join(chunks)


def apply_plans(plans: Sequence[UnitHardPlan], *, config) -> Tuple[int, int, List[str]]:
    errors: List[str] = []
    files = 0
    stubs = 0
    document = load_targets_document(config)
    rows = {
        str(r["id"]): r
        for r in document.get("targets", [])
        if isinstance(r, dict) and isinstance(r.get("id"), str)
    }
    relative_sources: Dict[str, str] = {}

    hard_marker = "\n// --- hard-symbol stubs (scaffold_hard_symbols) ---\n"
    for plan in plans:
        if not plan.stubs:
            continue
        if not plan.source_path.is_file():
            errors.append(f"{plan.unit}: missing source {plan.source_path}")
            continue
        text = plan.source_path.read_text(encoding="utf-8")
        if hard_marker in text:
            text = text.split(hard_marker)[0]
        append = _render_append(plan)
        plan.source_path.write_text(text + append, encoding="utf-8")
        files += 1
        stubs += len(plan.stubs)
        rel = plan.source_path.relative_to(config.project_root).as_posix()
        for stub in plan.stubs:
            relative_sources[stub.target_id] = rel

    if relative_sources:
        for tid, source in relative_sources.items():
            row = rows.get(tid)
            if row is None:
                errors.append(f"{tid}: missing from targets.json")
                continue
            row["source"] = source
            if row.get("workflow_status") in {None, "DISCOVERY", "BACKLOG"}:
                row["workflow_status"] = "BACKLOG"
        write_targets_document(config, document)
    return files, stubs, errors


def main(argv: Optional[List[str]] = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--config", type=Path, default=None)
    parser.add_argument("--tu", default=None)
    parser.add_argument("--prefix", default=None)
    parser.add_argument("--write", action="store_true")
    parser.add_argument("--verify", action="store_true")
    args = parser.parse_args(argv)
    if args.verify and not args.write:
        parser.error("--verify requires --write")

    config = load_config(args.config, ROOT)
    object_by_unit = _parse_configure_object_paths(config.project_root / "configure.py")
    selected = collect_hard_targets(
        load_targets(config), tu=args.tu, prefix=args.prefix
    )
    plans = build_plans(
        selected, project_root=config.project_root, object_by_unit=object_by_unit
    )

    stub_n = sum(len(p.stubs) for p in plans)
    skip_n = sum(len(p.skips) for p in plans)
    skip_reasons: Dict[str, int] = defaultdict(int)
    kind_counts: Dict[str, int] = defaultdict(int)
    for p in plans:
        for _tid, reason in p.skips:
            skip_reasons[reason] += 1
        for s in p.stubs:
            kind_counts[s.kind] += 1

    print(
        f"hard_targets={len(selected)} units={len(plans)} "
        f"planned_stubs={stub_n} skipped={skip_n}"
    )
    if kind_counts:
        print(
            "kinds: "
            + " ".join(f"{k}={v}" for k, v in sorted(kind_counts.items()))
        )
    if skip_reasons:
        print(
            "skips: "
            + " ".join(f"{k}={v}" for k, v in sorted(skip_reasons.items()))
        )
    for plan in plans[:12]:
        print(
            f"  {plan.unit}: stubs={len(plan.stubs)} skips={len(plan.skips)} "
            f"-> {plan.source_path.relative_to(config.project_root)}"
        )
    if len(plans) > 12:
        print(f"  ... and {len(plans) - 12} more units")

    if not args.write:
        print("dry-run only; pass --write to append stubs and update targets.json")
        return 0

    files, stubs, errors = apply_plans(plans, config=config)
    print(f"updated_files={files} written_stubs={stubs}")
    for err in errors[:20]:
        print(f"error: {err}", file=sys.stderr)

    verify_failures: List[str] = []
    if args.verify and files:
        units = [p.unit for p in plans if p.stubs]
        verify_failures = verify_units(units, project_root=config.project_root)
        for line in verify_failures[:20]:
            print(f"verify: {line}", file=sys.stderr)

    return 1 if errors or verify_failures else 0


if __name__ == "__main__":
    raise SystemExit(main())
