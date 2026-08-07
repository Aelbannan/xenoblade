#!/usr/bin/env python3
"""Scaffold marked placeholder stubs for targets with no editable source region.

Catalog import attributes many ready symbols to a ``.cpp`` that does not yet
contain a definition, so ``new`` auto-selection cannot find a region. This tool
appends harness-marked stub bodies so those targets become selectable.

Usage:
  python3 tools/coop/scaffold_missing_slots.py                 # dry-run
  python3 tools/coop/scaffold_missing_slots.py --tu kyoshin/CGame
  python3 tools/coop/scaffold_missing_slots.py --write --limit 20
  python3 tools/coop/scaffold_missing_slots.py --write --include-header-inline
"""
from __future__ import annotations

import argparse
import re
import sys
from collections import defaultdict
from dataclasses import dataclass
from pathlib import Path
from typing import Dict, List, Optional, Sequence, Tuple

ROOT = Path(__file__).resolve().parents[2]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from tools.coop.lib.config import load_config
from tools.coop.lib.project import Project
from tools.coop.lib.targets import Target, harness_targets, load_targets
from tools.coop.lib.source_regions import (
    begin_marker,
    end_marker,
    find_function_region,
)
from tools.symbolrecover.lib.mwcc import demangle_symbol

_INCLUDE_RE = re.compile(r'^\s*#\s*include\s+(?:"([^"]+)"|<([^>]+)>)', re.M)

# ---------------------------------------------------------------------------
# MWCC anonymous-namespace symbol decoding
#
# Anonymous namespaces are mangled as `@unnamed@<primary-file>@` and the linker
# name embeds the full lexical scope, e.g.
#   IsIncludeAnimationGroupRef__Q34nw4r3lyt24@unnamed@lyt_layout_cpp@F...
#   = nw4r::lyt::@unnamed@lyt_layout_cpp@::IsIncludeAnimationGroupRef(...)
# A stub reproduces that name by defining the function inside the same nested
# anonymous namespace of the same TU. Template instantiations must be forced
# via a typed function-pointer address-take (MWCC rejects explicit
# instantiation inside anonymous namespaces: "illegal explicit template
# specialization"). Both mechanisms are verified against mwcceppc 3.0a5.2.
# ---------------------------------------------------------------------------

_LLMH_T = "__LLMH_T__"  # template-arg placeholder (cannot collide with type names)

_MWCC_PRIM = {
    "v": "void",
    "b": "bool",
    "c": "char",
    "s": "short",
    "i": "int",
    "l": "long",
    "f": "float",
    "d": "double",
    "w": "wchar_t",
    "x": "long long",
}
_MWCC_UNSIGNED = {
    "c": "unsigned char",
    "s": "unsigned short",
    "i": "unsigned int",
    "l": "unsigned long",
    "x": "unsigned long long",
    "w": "wchar_t",
}


class _AnonNsUnsupported(Exception):
    """Raised with a skip reason when an anon-ns symbol cannot be stubbed."""

    def __init__(self, reason: str, detail: str = "") -> None:
        super().__init__(detail)
        self.reason = reason
        self.detail = detail


def _parse_mwcc_components(text: str, pos: int, count: int) -> Tuple[List[str], int]:
    """Parse `count` length-prefixed MWCC name components at text[pos].

    ``count == -1`` parses greedily until the next character is not a digit.
    """
    parts: List[str] = []
    while count == -1 or len(parts) < count:
        if pos >= len(text) or not text[pos].isdigit():
            if count == -1:
                break
            raise _AnonNsUnsupported("anon_ns_bad_mangle", "expected length prefix")
        m = re.match(r"\d+", text[pos:])
        if not m:
            raise _AnonNsUnsupported("anon_ns_bad_mangle", "expected length prefix")
        length = int(m.group(0))
        pos += m.end()
        if pos + length > len(text):
            raise _AnonNsUnsupported("anon_ns_bad_mangle", "truncated component")
        parts.append(text[pos : pos + length])
        pos += length
    return parts, pos


def _parse_mwcc_qualified(
    text: str, pos: int, tmpl_code: Optional[str]
) -> Tuple[str, List[str], int]:
    """Parse a ``Q<n>``-prefixed qualified name at text[pos].

    MWCC runs the count directly into the first component's length digit
    (e.g. ``Q34nw4r`` = 3 components, first is ``4nw4r``), so the split is
    ambiguous; try every prefix of the digit run and accept the one whose
    components parse and leave a plausible continuation.

    Returns (rendered_name, raw_components, next_pos).
    """
    digit_run = re.match(r"\d+", text[pos + 1 :])
    if not digit_run:
        raise _AnonNsUnsupported("anon_ns_bad_mangle", "Q without count")
    run = digit_run.group(0)
    for k in range(1, len(run) + 1):
        try:
            parts, p2 = _parse_mwcc_components(text, pos + 1 + k, int(run[:k]))
        except _AnonNsUnsupported:
            continue
        rendered = "::".join(
            (p.replace("<" + tmpl_code + ">", "<" + _LLMH_T + ">") if tmpl_code else p)
            for p in parts
        )
        if (
            p2 == len(text)
            or text[p2] in ("F", "_")
            or text[p2] in _MWCC_PRIM
            or text[p2] in "PRCQU"
            or text[p2].isdigit()
        ):
            return rendered, parts, p2
    raise _AnonNsUnsupported("anon_ns_bad_mangle", "Q parse failed")


def _decode_mwcc_type(
    text: str, pos: int, tmpl_code: Optional[str]
) -> Tuple[str, int]:
    """Decode one MWCC type code at text[pos] into a C type string.

    When the code equals tmpl_code (the template argument from the leaf, e.g.
    ``w`` in ``CalcStringRectImpl<w>``), the type is rendered as the _LLMH_T
    placeholder so the stub can be emitted both as a template definition and
    as a forced concrete instantiation.
    """
    if pos >= len(text):
        raise _AnonNsUnsupported("anon_ns_bad_args", "type underflow")
    ch = text[pos]
    if ch == "P":
        inner, pos = _decode_mwcc_type(text, pos + 1, tmpl_code)
        return inner + "*", pos
    if ch == "R":
        inner, pos = _decode_mwcc_type(text, pos + 1, tmpl_code)
        return inner + "&", pos
    if ch == "C":
        inner, pos = _decode_mwcc_type(text, pos + 1, tmpl_code)
        return "const " + inner, pos
    if ch == "U":
        base = text[pos + 1] if pos + 1 < len(text) else ""
        if base not in _MWCC_UNSIGNED:
            raise _AnonNsUnsupported("anon_ns_bad_args", f"bad unsigned code {base!r}")
        if tmpl_code and "U" + base == tmpl_code:
            return _LLMH_T, pos + 2
        return _MWCC_UNSIGNED[base], pos + 2
    if ch == "Q":
        name, _parts, npos = _parse_mwcc_qualified(text, pos, tmpl_code)
        return name, npos
    if ch.isdigit():
        # Bare length-prefixed name (e.g. 9ARCHandle, 14_GXIndTexMtxID).
        part, pos = _parse_mwcc_components(text, pos, 1)
        name = (
            part[0].replace("<" + tmpl_code + ">", "<" + _LLMH_T + ">")
            if tmpl_code
            else part[0]
        )
        return name, pos
    if ch in _MWCC_PRIM:
        if tmpl_code and ch == tmpl_code:
            return _LLMH_T, pos + 1
        return _MWCC_PRIM[ch], pos + 1
    raise _AnonNsUnsupported("anon_ns_bad_args", f"unknown type code {ch!r}")


def _concrete_tmpl_type(code: str) -> str:
    """Concrete C type for a template-argument code (w -> wchar_t, Uc -> unsigned char)."""
    if code.startswith("U") and code[1:] in _MWCC_UNSIGNED:
        return _MWCC_UNSIGNED[code[1:]]
    if code in _MWCC_PRIM:
        return _MWCC_PRIM[code]
    raise _AnonNsUnsupported("anon_ns_bad_args", f"bad template arg code {code!r}")


def _parse_anon_ns_head(
    symbol: str,
) -> Tuple[List[str], str, Optional[str], str, str]:
    """Parse the head of an @unnamed@ linker name (no argument decoding).

    Returns (ns_path, leaf_base, tmpl_code, args_part, ret_code). The leaf
    and namespace path are available even when the argument encoding is not
    decodable, so the duplicate guard can run on leaf presence alone.
    """
    if symbol.startswith("__ct__") or symbol.startswith("__dt__"):
        raise _AnonNsUnsupported(
            "anon_ns_member", "ctor/dtor of an anonymous-namespace class"
        )
    head, sep, rest = symbol.partition("__")
    if not sep:
        raise _AnonNsUnsupported("anon_ns_bad_mangle", "no __ separator")

    pos = 0
    if rest.startswith("Q"):
        _name, components, pos = _parse_mwcc_qualified(rest, 0, None)
    else:
        m = re.match(r"\d+", rest)
        if not m:
            raise _AnonNsUnsupported("anon_ns_bad_mangle", "no Q or length prefix")
        components, pos = _parse_mwcc_components(rest, 0, -1)

    anon_idx = None
    for i, comp in enumerate(components):
        if "@unnamed@" in comp:
            anon_idx = i
            break
    if anon_idx is None:
        raise _AnonNsUnsupported("anon_ns_bad_mangle", "no @unnamed@ component")
    if anon_idx < len(components) - 1:
        # Class member of an anonymous-namespace class: the class must be
        # reconstructed in the stub, and unused class members are not emitted
        # by MWCC (verified), so these are not stubbable.
        raise _AnonNsUnsupported(
            "anon_ns_member",
            f"member of anonymous-namespace class {components[anon_idx + 1]}",
        )
    ns_path = [c for c in components[:anon_idx]]

    tmpl_code: Optional[str] = None
    leaf_base = head
    tm = re.match(r"^(.+)<([^,>]+)>$", head)
    if tm:
        leaf_base, tmpl_code = tm.group(1), tm.group(2)

    if not rest[pos:].startswith("F"):
        raise _AnonNsUnsupported("anon_ns_bad_mangle", "expected F args")
    args_part = rest[pos + 1 :]
    ret_code = "v"
    if "_" in args_part:
        args_part, ret_code = args_part.rsplit("_", 1)
    return ns_path, leaf_base, tmpl_code, args_part, ret_code


def _decode_anon_ns_args(
    args_part: str, ret_code: str, tmpl_code: Optional[str]
) -> Tuple[List[str], str]:
    """Decode MWCC argument/return encodings into C type strings."""
    arg_types: List[str] = []
    i = 0
    while i < len(args_part):
        if args_part[i] == "e":
            arg_types.append("...")
            i += 1
            continue
        t, i = _decode_mwcc_type(args_part, i, tmpl_code)
        arg_types.append(t)
    ret = _decode_mwcc_type(ret_code, 0, tmpl_code)[0]
    return arg_types, ret


def _anon_ns_leaf_defined(source: str, leaf_base: str) -> bool:
    """True when the leaf already has a definition-like occurrence in the TU.

    Guards against appending a duplicate definition into the (merged) unnamed
    namespace of the TU. ``Min<u8>(...)`` call sites do not match ``Min(``.
    """
    return re.search(r"\b" + re.escape(leaf_base) + r"\s*\(", source) is not None


def _anon_ns_stub_text(
    target_id: str,
    ns_path: List[str],
    leaf_base: str,
    tmpl_code: Optional[str],
    arg_types: List[str],
    ret: str,
    concrete: Optional[str],
    template_available: bool = False,
    headers: Sequence[Path] = (),
) -> str:
    """Build the anonymous-namespace stub block (with harness markers).

    MWCC quirks (all verified against mwcceppc 3.0a5.2 with -ipa file):

    * Anonymous-namespace free functions and templates are only emitted when
      referenced by an EXTERN-linkage variable — internal-linkage anon-ns
      variables/functions get dead-code-eliminated by -ipa file.
    * A template defined inside the anon ns is mangled with the anon scope
      (e.g. CalcStringRectImpl<w>__Q34nw4r3lyt25@unnamed@lyt_textBox_cpp@...)
      when the forced address-take happens in the enclosing namespace.
    * A template DECLARED IN A HEADER (e.g. nw4r::ut::Min from ut_algorithm.h)
      is mangled with the anon scope when the TU contains any anonymous
      namespace and the address-take happens at global scope; it must NOT be
      redefined (that collides once any second header is included).
    """
    is_tmpl = tmpl_code is not None and concrete is not None
    if is_tmpl:
        params = ", ".join(f"{t} a{i + 1}" for i, t in enumerate(arg_types))
        params = params.replace(_LLMH_T, "T")
        ret_def = ret.replace(_LLMH_T, "T")
        body = _stub_body(ret_def)
    else:
        params = ", ".join(f"{t} a{i + 1}" for i, t in enumerate(arg_types))
        ret_def = ret
        body = _stub_body(ret)
    force_tag = target_id.replace("-", "_")
    lines = [begin_marker(target_id)]

    if is_tmpl and template_available:
        # Header template (e.g. nw4r::ut::Min): reference it at global scope;
        # the TU's anonymous namespace supplies the anon-scope mangling.
        qualified = "::".join(ns_path + [leaf_base])
        force_params = ", ".join(
            t.replace(_LLMH_T, concrete) if t != _LLMH_T else concrete
            for t in arg_types
        )
        fn_ret = concrete if ret == _LLMH_T else ret.replace(_LLMH_T, concrete)
        lines.append(
            f"typedef {fn_ret} (*LLMH_ForceFn_{force_tag})({force_params});"
        )
        lines.append(
            f"LLMH_ForceFn_{force_tag} LLMH_force_{force_tag} = "
            f"&{qualified}<{concrete}>;"
        )
        lines.append(end_marker(target_id))
        return "\n".join(lines) + "\n"

    for ns in ns_path:
        lines.append(f"namespace {ns} {{")
    # Forward-declare qualified types that no header declares (e.g.
    # nw4r::lyt::AnimationGroupRef is not recovered in this fork).
    for t in arg_types + ([ret] if ret != "void" and not is_tmpl else []):
        if _LLMH_T in t:
            continue
        m = re.match(r"^(?:const\s+)?([\w:]+)(?:\s*[&*]|\s*<.*>)?$", t)
        if not m:
            continue
        qname = m.group(1)
        leaf = qname.split("::")[-1]
        if qname.startswith("::") or leaf in {"void", "bool", "char", "short", "int", "long", "float", "double", "wchar_t", "unsigned", "signed"}:
            continue
        if not _header_has_type(headers, leaf):
            lines.append(f"struct {leaf};")
    lines.append("namespace {")
    if is_tmpl:
        lines.append("template <typename T>")
        lines.append(f"{ret_def} {leaf_base}({params}) {body}")
        force_params = ", ".join(
            t.replace(_LLMH_T, concrete) if t != _LLMH_T else concrete
            for t in arg_types
        )
        fn_ret = concrete if ret == _LLMH_T else ret.replace(_LLMH_T, concrete)
        lines.append(
            f"typedef {fn_ret} (*LLMH_ForceFn_{force_tag})({force_params});"
        )
    else:
        lines.append(f"{ret_def} {leaf_base}({params}) {body}")
    lines.append("}")
    # Extern-linkage force reference in the enclosing namespace: keeps the
    # anon-ns definition alive under -ipa file.
    if is_tmpl:
        force_params = ", ".join(
            t.replace(_LLMH_T, concrete) if t != _LLMH_T else concrete
            for t in arg_types
        )
        fn_ret = concrete if ret == _LLMH_T else ret.replace(_LLMH_T, concrete)
        lines.append(
            f"extern LLMH_ForceFn_{force_tag} LLMH_force_{force_tag} = "
            f"&{leaf_base}<{concrete}>;"
        )
    else:
        lines.append(
            f"extern void* LLMH_force_{force_tag} = (void*)&{leaf_base};"
        )
    for _ in ns_path:
        lines.append("}")
    lines.append(end_marker(target_id))
    return "\n".join(lines) + "\n"
_BROKEN_ARG_TOKEN_RE = re.compile(
    r"^(?:\.\.\.\*|const \.\.\.\*|[A-Za-z_]|unsigned char|int|long|double|float|"
    r"char|short|bool|wchar_t)$"
)


@dataclass(frozen=True)
class StubPlan:
    target_id: str
    symbol: str
    function: str
    source: Path
    unit: str
    stub_text: str
    return_type: str
    qualified_name: str
    args: str
    reason: str = "missing_region"


@dataclass(frozen=True)
class SkipPlan:
    target_id: str
    symbol: str
    function: str
    source: Path
    reason: str
    detail: str = ""


def _unit_matches(unit: Optional[str], needle: str) -> bool:
    if not unit:
        return False
    u = unit.removeprefix("main/")
    n = needle.removeprefix("main/")
    return u == n or u.endswith("/" + n) or n.endswith("/" + u)


def _args_look_broken(args: str) -> bool:
    inner = args.strip()
    if not inner or inner == "()":
        return False
    # Demangle crumbs / varargs fragments are not valid C prototypes.
    if "..." in inner:
        return True
    toks = [t.strip() for t in inner.strip("()").split(",") if t.strip()]
    if not toks:
        return False
    # Single-letter tokens like "i" / "v" / "R" / "f*" are demangle leftovers.
    if any(re.fullmatch(r"[A-Za-z]\*?", t) for t in toks):
        return True
    if len(toks) < 3:
        return False
    broken = sum(1 for t in toks if _BROKEN_ARG_TOKEN_RE.match(t) and len(t) <= 3)
    return broken >= max(2, (len(toks) + 1) // 2)


def _qualified_name(demangled_function: str, symbol: str) -> Tuple[str, str, bool]:
    """Return (qualified_name, args, is_c_linkage)."""
    info = demangle_symbol(symbol)
    if info.is_ctor and info.class_name:
        ns = f"{info.namespace}::" if info.namespace else ""
        return f"{ns}{info.class_name}::{info.class_name}", info.args or "()", False
    if info.is_dtor and info.class_name:
        ns = f"{info.namespace}::" if info.namespace else ""
        return f"{ns}{info.class_name}::~{info.class_name}", info.args or "()", False

    if info.class_name:
        ns = f"{info.namespace}::" if info.namespace else ""
        args = info.args or "()"
        if _args_look_broken(args):
            args = "()"
        return f"{ns}{info.class_name}::{info.function}", args, False

    # Free function / C symbol (func_*, __sinit_, plain names).
    name = info.function or demangled_function.split("(", 1)[0].strip()
    args = info.args or "()"
    if _args_look_broken(args):
        args = "()"
    # CodeWarrior C++ mangling uses "__" (e.g. func_8014A8F8__Fv). Unmangled
    # linker names must use extern "C"; mangled free functions must not.
    is_c = "__" not in symbol
    return name, args, is_c


def _infer_return_type(symbol: str, retail_size: Optional[int]) -> str:
    info = demangle_symbol(symbol)
    if info.is_ctor or info.is_dtor:
        return ""
    if symbol.endswith("Fv") or (info.args == "()" and symbol.endswith("Fv")):
        # Many Fv methods are void; bool-returning Fv still ends in Fv for args-only.
        # Prefer void; header lookup can override.
        if retail_size == 8:
            return "bool"
        return "void"
    if retail_size == 8:
        return "bool"
    if retail_size == 4:
        return "void"
    return "void"


def _stub_body(return_type: str) -> str:
    # Prefer patterns PlaceholderDetector recognizes after comment stripping.
    if return_type in {"", "void"}:
        return "{}"
    if return_type == "bool":
        return "{ return false; }"
    if return_type == "float":
        return "{ return 0.0f; }"
    if return_type == "double":
        return "{ return 0.0; }"
    return "{ return 0; }"


def _format_stub(
    *,
    target_id: str,
    qualified_name: str,
    args: str,
    return_type: str,
    is_c_linkage: bool,
    symbol: str = "",
    force_mangled_symbol: bool = False,
    c_source: bool = False,
) -> str:
    # Plain C TUs (-lang=c) reject extern "C" and often lack bool.
    if c_source and return_type == "bool":
        return_type = "int"
    body = _stub_body(return_type)
    if c_source and return_type == "int" and "false" in body:
        body = "{ return 0; }"
    if force_mangled_symbol and symbol:
        # Match retail linker name exactly (see CfGameManager Fv-but-has-args pattern).
        ret = return_type if return_type else "void"
        if c_source:
            signature = f"{ret} {symbol}{args}"
        else:
            signature = f'extern "C" {ret} {symbol}{args}'
    else:
        if return_type:
            signature = f"{return_type} {qualified_name}{args}"
        else:
            signature = f"{qualified_name}{args}"
        if is_c_linkage and "::" not in qualified_name and not c_source:
            signature = f'extern "C" {signature}'
    return (
        f"{begin_marker(target_id)}\n"
        f"{signature} {body}\n"
        f"{end_marker(target_id)}\n"
    )


def _include_roots(project_root: Path) -> List[Path]:
    roots = [
        project_root / "src",
        project_root / "include",
        project_root / "libs",
    ]
    # SDK layouts: libs/RVL_SDK/include, libs/nw4r/include, libs/monolib/include, …
    libs = project_root / "libs"
    if libs.is_dir():
        for child in sorted(libs.iterdir()):
            if not child.is_dir():
                continue
            for sub in (child / "include", child / "src"):
                if sub.is_dir():
                    roots.append(sub)
    return [p for p in roots if p.is_dir()]


def _build_header_index(roots: Sequence[Path]) -> Dict[str, List[Path]]:
    """Map header stem -> paths (e.g. IWorkEvent -> [.../IWorkEvent.hpp])."""
    index: Dict[str, List[Path]] = defaultdict(list)
    for root in roots:
        for path in root.rglob("*"):
            if path.suffix.lower() not in {".h", ".hpp", ".hh"}:
                continue
            if not path.is_file():
                continue
            index[path.stem].append(path)
    return index


def _resolve_includes(
    source_path: Path,
    source_text: str,
    roots: Sequence[Path],
    header_index: Dict[str, List[Path]],
    *,
    class_name: Optional[str] = None,
) -> List[Path]:
    found: List[Path] = []
    seen: set[Path] = set()
    candidates: List[Path] = [
        source_path.with_suffix(".hpp"),
        source_path.with_suffix(".h"),
    ]
    for match in _INCLUDE_RE.findall(source_text):
        rel = match[0] or match[1]
        if not rel:
            continue
        candidates.append(source_path.parent / rel)
        for root in roots:
            candidates.append(root / rel)
        # Also try header stem lookup for SDK includes.
        stem = Path(rel).stem
        candidates.extend(header_index.get(stem, []))
    if class_name:
        candidates.extend(header_index.get(class_name, []))
    for path in candidates:
        try:
            resolved = path.resolve()
        except OSError:
            continue
        if resolved in seen or not resolved.is_file():
            continue
        seen.add(resolved)
        found.append(resolved)
    return found


def _header_declaration(
    *,
    function_leaf: str,
    headers: Sequence[Path],
) -> Optional[Tuple[str, str, Path]]:
    """Return (return_type, args_with_parens, header) for a declaration ending in ';'.

    Skips inline definitions (those end with '{').
    """
    pattern = re.compile(
        rf"(?:virtual\s+|static\s+|inline\s+)*"
        rf"(?:([A-Za-z_][\w:<>\*&\s]*?)\s+)?"
        rf"{re.escape(function_leaf)}\s*"
        rf"(\((?:[^()]|\([^()]*\))*\))\s*"
        rf"(?:const\s*)?"
        rf"([;{{])",
        re.M,
    )
    for header in headers:
        try:
            text = header.read_text(encoding="utf-8")
        except OSError:
            continue
        for match in pattern.finditer(text):
            terminator = match.group(3)
            if terminator != ";":
                continue
            ret = (match.group(1) or "").strip()
            args = match.group(2)
            if ret in {"virtual", "static", "inline", "constexpr", ""}:
                # Empty ret is usually a call site (e.g. ``GetLightSet(i);``).
                if ret == "" and not function_leaf.startswith("~"):
                    continue
                if function_leaf.startswith("~") or ret == "":
                    return "", args, header
                continue
            # Drop trailing qualifiers accidentally captured
            ret = ret.split()[-1] if ret else ""
            if _args_look_broken(args):
                continue
            return ret, args, header
    return None


def _header_inline_definition(
    *,
    function_leaf: str,
    headers: Sequence[Path],
) -> Optional[Tuple[Path, str]]:
    """Return (header, snippet) if an inline body exists for this leaf name."""
    pattern = re.compile(
        rf"\b{re.escape(function_leaf)}\s*\((?:[^()]|\([^()]*\))*\)\s*\{{",
        re.M,
    )
    for header in headers:
        try:
            text = header.read_text(encoding="utf-8")
        except OSError:
            continue
        match = pattern.search(text)
        if match:
            start = max(0, match.start() - 40)
            end = min(len(text), match.end() + 40)
            return header, " ".join(text[start:end].split())
    return None


def _retail_size(project: Project, target: Target) -> Optional[int]:
    if not target.unit or not target.symbol:
        return None
    try:
        unit = project.resolve_unit(target.unit)
    except (OSError, ValueError):
        return None
    if unit.target_path is None or not unit.target_path.is_file():
        return None
    try:
        from tools.ppc_equivalence.elf_symbols import extract_function

        fn = extract_function(unit.target_path, target.symbol)
    except (OSError, ValueError):
        return None
    return int(fn.size)


def _tu_home_class(unit: Optional[str], source: Path) -> str:
    """Best-effort primary class name for a TU (source/unit basename)."""
    if unit:
        return Path(unit).name
    return source.stem


_THUNK_SYMBOL_RE = re.compile(r"^@\d+@")


def _is_thunk_symbol(symbol: str) -> bool:
    """MWCC virtual-adjustment thunks (e.g. @84@__dt__Q22cf9CfPadTaskFv)."""
    return bool(_THUNK_SYMBOL_RE.match(symbol))


def _symbol_has_invalid_identifier_chars(symbol: str) -> bool:
    """True when the linker name cannot be a C/C++ identifier as-written.

    Anonymous-namespace encodings embed ``@unnamed@…@`` (and thunks use a
    leading ``@NN@``). Template crumbs may leave ``<``/``>``. Those cannot be
    emitted as ``extern "C" <symbol>(…)``.
    """
    if "@" in symbol or "<" in symbol or ">" in symbol:
        return True
    # Force-mangled emission requires a plain identifier.
    if not re.match(r"^[A-Za-z_][\w$]*$", symbol):
        return True
    return False


def _ctor_dtor_symbol_body(symbol: str) -> Optional[str]:
    """Strip __ct__/__dt__ prefix; return None if not a ctor/dtor linker name."""
    for prefix in ("__ct__", "__dt__"):
        if symbol.startswith(prefix):
            return symbol[len(prefix) :]
    return None


def _undemangled_ctor_dtor_matches_home(symbol: str, home_class: str) -> bool:
    """True when an unscoped __ct__/__dt__ symbol belongs to the TU home class."""
    body = _ctor_dtor_symbol_body(symbol)
    if body is None:
        return True
    # Length-prefixed: 11CMCEffStartFv / Q22cf…
    if re.match(r"^\d+", body):
        return home_class in body
    # Informal names: cf_CfCam, CMCEffSuccess
    return home_class in body


def _transitive_headers(
    source_path: Path,
    source_text: str,
    roots: Sequence[Path],
    header_index: Dict[str, List[Path]],
    max_depth: int = 4,
) -> List[Path]:
    """Resolve the direct + transitive include closure of a TU's headers.

    The flat ``_resolve_includes`` only sees direct includes, but type/template
    lookups (e.g. ``nw4r::ut::Min`` in ut_algorithm.h pulled in by ut.h, or
    lyt_group.h pulled in by lyt.h) need the closure.
    """
    found: List[Path] = []
    seen: set[Path] = set()
    queue: List[Tuple[Path, int]] = [
        (p, 0)
        for p in _resolve_includes(source_path, source_text, roots, header_index)
    ]
    while queue:
        path, depth = queue.pop(0)
        try:
            resolved = path.resolve()
        except OSError:
            continue
        if resolved in seen:
            continue
        seen.add(resolved)
        found.append(path)
        if depth >= max_depth:
            continue
        try:
            text = path.read_text(encoding="utf-8")
        except OSError:
            continue
        for match in _INCLUDE_RE.findall(text):
            rel = match[0] or match[1]
            if not rel:
                continue
            cands = [path.parent / rel]
            for root in roots:
                cands.append(root / rel)
            cands.extend(header_index.get(Path(rel).stem, []))
            for cand in cands:
                try:
                    cres = cand.resolve()
                except OSError:
                    continue
                if cres not in seen and cres.is_file():
                    queue.append((cres, depth + 1))
    return found


def _header_has_type(headers: Sequence[Path], type_name: str) -> bool:
    """True when a class/struct/typedef/using declaration for type_name exists.

    Used to decide whether a decoded qualified type needs a forward
    declaration in the stub (the type may not have been recovered in this
    fork, e.g. ``nw4r::lyt::AnimationGroupRef``).
    """
    pattern = re.compile(
        r"\b(?:class|struct|union)\s+" + re.escape(type_name) + r"\b"
        r"|\btypedef\b[^;]*\b" + re.escape(type_name) + r"\b"
        r"|\busing\s+" + re.escape(type_name) + r"\s*="
    )
    for header in headers:
        try:
            text = header.read_text(encoding="utf-8")
        except OSError:
            continue
        if pattern.search(text):
            return True
    return False


def _free_template_in_headers(
    headers: Sequence[Path], leaf: str
) -> bool:
    """True when a header declares a FREE function template named `leaf`.

    Matches ``template <...> [inline] <ret> leaf(...)`` at namespace scope.
    Class members (``template <...> class X { ... leaf(...) ... }``) do not
    match because the class body ``{`` breaks the ``[A-Za-z_:<>,\\s]*`` gap.
    Used to decide whether an anon-ns stub may skip the template definition
    and only force-emit the existing instantiation (e.g. nw4r::ut::Min).
    """
    pattern = re.compile(
        r"\btemplate\s*<[^>]*>\s*(?:inline\s+)?"
        r"[A-Za-z_:<>,\s]*?\b"
        + re.escape(leaf)
        + r"\s*\("
    )
    for header in headers:
        try:
            text = header.read_text(encoding="utf-8")
        except OSError:
            continue
        if pattern.search(text):
            return True
    return False


def _try_plan_anon_ns(
    target: Target,
    source: str,
    roots: Sequence[Path],
    header_index: Dict[str, List[Path]],
) -> Tuple[Optional[StubPlan], Optional[SkipPlan]]:
    """Plan a stub for an anonymous-namespace symbol, or explain the skip."""
    symbol = target.symbol
    # Duplicate guard runs on the head parse (leaf + scope) so it works even
    # when the argument encoding is not decodable.
    try:
        ns_path, leaf_base, tmpl_code, args_part, ret_code = _parse_anon_ns_head(
            symbol
        )
    except _AnonNsUnsupported as exc:
        return None, SkipPlan(
            target.id, symbol, target.function, target.source, exc.reason, exc.detail
        )
    if _anon_ns_leaf_defined(source, leaf_base):
        return None, SkipPlan(
            target.id,
            symbol,
            target.function,
            target.source,
            "anon_ns_duplicate",
            f"{leaf_base} already defined in {target.source.name}; "
            "needs decomp-level scope fix, not a stub",
        )
    headers = _transitive_headers(
        target.source,
        source,
        roots,
        header_index,
    )
    if target.source not in headers:
        headers = list(headers) + [target.source]
    try:
        arg_types, ret = _decode_anon_ns_args(args_part, ret_code, tmpl_code)
        concrete = _concrete_tmpl_type(tmpl_code) if tmpl_code else None
    except _AnonNsUnsupported as exc:
        return None, SkipPlan(
            target.id, symbol, target.function, target.source, exc.reason, exc.detail
        )
    # When the template already exists in a header (e.g. nw4r::ut::Min), the
    # stub must NOT redefine it — MWCC instantiates header templates used from
    # an anonymous namespace into the anon-ns scope, so redefining Min there
    # collides. Emit only the forced address-take in that case.
    template_available = _free_template_in_headers(headers, leaf_base)
    if template_available:
        qualified = "::".join(ns_path + [leaf_base])
        concrete = _concrete_tmpl_type(tmpl_code) if tmpl_code else None
        if concrete and f"&{qualified}<{concrete}>" in source:
            # The emission hook already exists in the TU (moved-in-place form).
            return None, SkipPlan(
                target.id,
                symbol,
                target.function,
                target.source,
                "region_exists",
                f"{qualified}<{concrete}> already referenced in {target.source.name}",
            )
    stub_text = _anon_ns_stub_text(
        target.id,
        ns_path,
        leaf_base,
        tmpl_code,
        arg_types,
        ret,
        concrete,
        template_available=template_available,
        headers=headers,
    )
    display = "::".join(ns_path + [leaf_base])
    if tmpl_code:
        display += f"<{concrete}>"
    plan = StubPlan(
        target_id=target.id,
        symbol=symbol,
        function=target.function,
        source=target.source,
        unit=target.unit or "",
        stub_text=stub_text,
        return_type=ret if ret != _LLMH_T else (concrete or "T"),
        qualified_name=display,
        args="",
        reason="anon_ns",
    )
    return plan, None


def collect_plans(
    targets: Sequence[Target],
    *,
    project: Project,
    project_root: Path,
    include_header_inline: bool,
    skip_sinit: bool,
    allow_undeclared: bool,
    allow_foreign_class: bool,
    allow_anon_ns: bool,
    tu: Optional[str],
) -> Tuple[List[StubPlan], List[SkipPlan]]:
    roots = _include_roots(project_root)
    header_index = _build_header_index(roots)
    source_cache: Dict[Path, str] = {}
    # Cache key: (source_path, class_name)
    header_cache: Dict[Tuple[Path, str], List[Path]] = {}
    plans: List[StubPlan] = []
    skips: List[SkipPlan] = []

    for target in targets:
        if tu is not None and not _unit_matches(target.unit, tu):
            continue
        if not target.symbol or target.source is None or not target.source.is_file():
            skips.append(
                SkipPlan(
                    target.id,
                    target.symbol or "",
                    target.function,
                    target.source or Path("."),
                    "missing_source",
                )
            )
            continue
        if skip_sinit and (
            target.symbol.startswith("__sinit_")
            or target.symbol.startswith("sinit_")
            or target.function.startswith("__sinit_")
            or target.function.startswith("sinit_")
        ):
            skips.append(
                SkipPlan(
                    target.id,
                    target.symbol,
                    target.function,
                    target.source,
                    "sinit",
                )
            )
            continue
        if _is_thunk_symbol(target.symbol):
            skips.append(
                SkipPlan(
                    target.id,
                    target.symbol,
                    target.function,
                    target.source,
                    "thunk",
                    "MWCC virtual-adjustment thunks are compiler-generated; "
                    "no source construct produces a @NN@-prefixed symbol",
                )
            )
            continue
        if "@unnamed@" in target.symbol:
            if not allow_anon_ns:
                skips.append(
                    SkipPlan(
                        target.id,
                        target.symbol,
                        target.function,
                        target.source,
                        "invalid_symbol",
                        "anonymous-namespace encoding; pass --allow-anon-ns",
                    )
                )
                continue
            if target.source is None or not target.source.is_file():
                skips.append(
                    SkipPlan(
                        target.id,
                        target.symbol,
                        target.function,
                        target.source,
                        "missing_source",
                    )
                )
                continue
            try:
                anon_source = target.source.read_text(encoding="utf-8")
            except OSError as exc:
                skips.append(
                    SkipPlan(
                        target.id,
                        target.symbol,
                        target.function,
                        target.source,
                        "unreadable",
                        str(exc),
                    )
                )
                continue
            try:
                find_function_region(anon_source, target)
            except ValueError:
                pass
            else:
                skips.append(
                    SkipPlan(
                        target.id,
                        target.symbol,
                        target.function,
                        target.source,
                        "region_exists",
                    )
                )
                continue
            if (
                begin_marker(target.id) in anon_source
                or end_marker(target.id) in anon_source
            ):
                skips.append(
                    SkipPlan(
                        target.id,
                        target.symbol,
                        target.function,
                        target.source,
                        "markers_exist",
                    )
                )
                continue
            plan, skip = _try_plan_anon_ns(target, anon_source, roots, header_index)
            if skip is not None:
                skips.append(skip)
                continue
            plans.append(plan)
            continue
        if _symbol_has_invalid_identifier_chars(target.symbol):
            skips.append(
                SkipPlan(
                    target.id,
                    target.symbol,
                    target.function,
                    target.source,
                    "invalid_symbol",
                    "linker name cannot be emitted as a C++ identifier",
                )
            )
            continue

        source = source_cache.get(target.source)
        if source is None:
            try:
                source = target.source.read_text(encoding="utf-8")
            except OSError as exc:
                skips.append(
                    SkipPlan(
                        target.id,
                        target.symbol,
                        target.function,
                        target.source,
                        "unreadable",
                        str(exc),
                    )
                )
                continue
            source_cache[target.source] = source

        try:
            find_function_region(source, target)
        except ValueError:
            pass
        else:
            skips.append(
                SkipPlan(
                    target.id,
                    target.symbol,
                    target.function,
                    target.source,
                    "region_exists",
                )
            )
            continue

        if begin_marker(target.id) in source or end_marker(target.id) in source:
            skips.append(
                SkipPlan(
                    target.id,
                    target.symbol,
                    target.function,
                    target.source,
                    "markers_exist",
                )
            )
            continue

        demangled = demangle_symbol(target.symbol)
        home_class = _tu_home_class(target.unit, target.source)
        if (
            not allow_foreign_class
            and demangled.class_name
            and demangled.class_name != home_class
        ):
            skips.append(
                SkipPlan(
                    target.id,
                    target.symbol,
                    target.function,
                    target.source,
                    "foreign_class",
                    f"{demangled.class_name} != TU home class {home_class}",
                )
            )
            continue
        if (
            not allow_foreign_class
            and (demangled.is_ctor or demangled.is_dtor)
            and demangled.class_name is None
            and _ctor_dtor_symbol_body(target.symbol) is not None
            and not _undemangled_ctor_dtor_matches_home(target.symbol, home_class)
        ):
            skips.append(
                SkipPlan(
                    target.id,
                    target.symbol,
                    target.function,
                    target.source,
                    "foreign_class",
                    f"undemangled ctor/dtor {target.symbol} does not mention {home_class}",
                )
            )
            continue

        class_key = demangled.class_name or ""
        cache_key = (target.source, class_key)
        headers = header_cache.get(cache_key)
        if headers is None:
            headers = _resolve_includes(
                target.source,
                source,
                roots,
                header_index,
                class_name=demangled.class_name,
            )
            header_cache[cache_key] = headers

        leaf = demangled.function
        if not leaf or leaf == target.symbol:
            leaf = target.function.split("(", 1)[0].split("::")[-1].strip()
        # Demangler reports "constructor"/"destructor" when class_name is missing;
        # look up the raw linker symbol instead (e.g. __ct__cf_CfCam).
        if leaf in {"constructor", "destructor"} or leaf.startswith("@"):
            if demangled.is_ctor and demangled.class_name:
                leaf = demangled.class_name
            elif demangled.is_dtor and demangled.class_name:
                leaf = f"~{demangled.class_name}"
            else:
                leaf = target.symbol

        # Prefer the class's own header for decl/inline checks so inherited
        # inline names (e.g. CTTask::Draw) do not block CTaskGame::Draw.
        if demangled.class_name:
            class_headers = [h for h in headers if h.stem == demangled.class_name]
            search_headers: List[Path] = list(class_headers or headers)
        else:
            search_headers = list(headers)

        # TU .cpp/.c often forward-declares helpers; also search same-named
        # headers from the index (covers SDK headers pulled in transitively).
        if target.source not in search_headers:
            search_headers.append(target.source)
        for extra in header_index.get(target.source.stem, []):
            if extra not in search_headers:
                search_headers.append(extra)
        for extra in header_index.get(leaf.lstrip("~"), []):
            if extra not in search_headers:
                search_headers.append(extra)

        inline = _header_inline_definition(function_leaf=leaf, headers=search_headers)
        if inline is not None and not include_header_inline:
            header, snippet = inline
            try:
                header_rel = str(header.relative_to(project_root))
            except ValueError:
                header_rel = str(header)
            skips.append(
                SkipPlan(
                    target.id,
                    target.symbol,
                    target.function,
                    target.source,
                    "header_inline",
                    f"{header_rel}: {snippet[:100]}",
                )
            )
            continue

        qualified, args, is_c = _qualified_name(target.function, target.symbol)
        if "<" in qualified or ">" in qualified:
            skips.append(
                SkipPlan(
                    target.id,
                    target.symbol,
                    target.function,
                    target.source,
                    "template_name",
                    qualified,
                )
            )
            continue

        decl = _header_declaration(function_leaf=leaf, headers=search_headers)
        is_member = "::" in qualified
        # Prefer retail mangled parameter encoding for the linker name. When the
        # header recovered richer args than the symbol encodes (common Fv case),
        # keep the header args in the C prototype but force the mangled symbol
        # name via extern "C" (same pattern as func_8007C6B4__...Fv).
        force_mangled = False
        if decl is not None:
            decl_return, decl_args, _decl_header = decl
            if demangled.is_ctor or demangled.is_dtor:
                # Unscoped C-style __ct__/__dt__ keep the recovered return type
                # (often void*); real C++ ctors/dtors have no return type.
                if demangled.class_name is None and decl_return:
                    return_type = decl_return
                else:
                    return_type = ""
            elif decl_return:
                return_type = decl_return
            else:
                return_type = _infer_return_type(
                    target.symbol, _retail_size(project, target)
                )
            if (
                decl_args not in {"", "()"}
                and not _args_look_broken(decl_args)
                and (args in {"", "()"} or args != decl_args)
            ):
                # Match an existing prototype. For Fv members whose header has
                # richer args, also force the exact retail linker name.
                if is_member and target.symbol.endswith("Fv"):
                    force_mangled = True
                args = decl_args
        elif is_member and not allow_undeclared:
            skips.append(
                SkipPlan(
                    target.id,
                    target.symbol,
                    target.function,
                    target.source,
                    "undeclared",
                    f"no ';'-terminated declaration for {leaf} in related headers",
                )
            )
            continue
        else:
            retail_size = _retail_size(project, target)
            return_type = _infer_return_type(target.symbol, retail_size)
            if demangled.is_ctor or demangled.is_dtor:
                return_type = ""

        # Unscoped __ct__/__dt__ linker names must be emitted verbatim.
        if (demangled.is_ctor or demangled.is_dtor) and demangled.class_name is None:
            force_mangled = True
            if args in {"", None}:
                args = "()"
            if not return_type:
                return_type = "void"

        # Nested / qualified class encodings (e.g. Draw__Q216CUIBattleManager5CTestFv)
        # often demangle without class_name. A plain `void Draw()` would emit Draw__Fv.
        # Also force-mangle free functions when demangle drops args (func_X__Fi →
        # void func_X() would wrongly emit func_X__Fv).
        # And force-mangle when demangle leaves the raw linker name as the
        # "function" (common for __as__/__op* operators) — otherwise MWCC
        # re-mangles it to <symbol>__Fv.
        if not force_mangled and "__" in target.symbol and demangled.class_name is None:
            empty_args = args in {"", "()", None}
            simple_id = "__" not in qualified
            free_mangling_ok = (
                (target.symbol == qualified and simple_id)
                or target.symbol == f"{qualified}__Fv"
                or (
                    not empty_args
                    and simple_id
                    and target.symbol.startswith(f"{qualified}__F")
                )
            )
            if not free_mangling_ok:
                force_mangled = True
                if empty_args:
                    args = "()"
                if not return_type and not (demangled.is_ctor or demangled.is_dtor):
                    return_type = _infer_return_type(
                        target.symbol, _retail_size(project, target)
                    )

        # Force-mangled stubs only need a compilable prototype; drop demangle crumbs.
        if _args_look_broken(args):
            args = "()"

        c_source = target.source.suffix.lower() == ".c"
        # Plain C translation units cannot host C++ member/ctor syntax.
        if c_source and (
            "::" in qualified
            or demangled.is_ctor
            or demangled.is_dtor
            or demangled.class_name is not None
        ):
            force_mangled = True
            if args in {"", None}:
                args = "()"
            if not return_type:
                return_type = "void"

        # Avoid unknown typedef stubs that will not compile.
        if return_type in {"UNKWORD", "UNKTYPE", "undefined", "undefined4"} or (
            return_type
            and any(
                bad in return_type
                for bad in ("UNKWORD", "UNKTYPE", "undefined", "undefined4")
            )
        ):
            skips.append(
                SkipPlan(
                    target.id,
                    target.symbol,
                    target.function,
                    target.source,
                    "bad_return_type",
                    return_type,
                )
            )
            continue

        stub = _format_stub(
            target_id=target.id,
            qualified_name=qualified,
            args=args,
            return_type=return_type,
            is_c_linkage=is_c,
            symbol=target.symbol,
            force_mangled_symbol=force_mangled or c_source,
            c_source=c_source,
        )
        plans.append(
            StubPlan(
                target_id=target.id,
                symbol=target.symbol,
                function=target.function,
                source=target.source,
                unit=target.unit or "",
                stub_text=stub,
                return_type=return_type or "(ctor/dtor)",
                qualified_name=qualified if not force_mangled else target.symbol,
                args=args,
            )
        )

    return plans, skips


def apply_plans(
    plans: Sequence[StubPlan],
    *,
    project_root: Path,
) -> Tuple[int, List[str]]:
    """Append stubs per file. Returns (files_changed, errors)."""
    by_file: Dict[Path, List[StubPlan]] = defaultdict(list)
    for plan in plans:
        by_file[plan.source].append(plan)

    changed = 0
    errors: List[str] = []
    for path, file_plans in sorted(by_file.items(), key=lambda item: str(item[0])):
        try:
            original = path.read_text(encoding="utf-8")
        except OSError as exc:
            errors.append(f"{path}: {exc}")
            continue
        block = "\n" + "".join(plan.stub_text for plan in file_plans)
        if not original.endswith("\n"):
            block = "\n" + block
        updated = original + block
        # Validate each new region resolves.
        for plan in file_plans:
            try:
                region = find_function_region(updated, _TargetShim(plan))
            except ValueError as exc:
                errors.append(f"{plan.target_id}: post-insert region failed: {exc}")
                break
            body = updated[region.content_start : region.content_end]
            if begin_marker(plan.target_id) not in updated:
                errors.append(f"{plan.target_id}: markers missing after insert")
                break
            if plan.qualified_name.split("::")[-1].split("~")[-1] not in body and not region.marked:
                errors.append(f"{plan.target_id}: stub body missing expected name")
                break
        else:
            path.write_text(updated, encoding="utf-8")
            changed += 1
            continue
        # Leave file untouched on validation failure for this batch.
    return changed, errors


class _TargetShim:
    def __init__(self, plan: StubPlan) -> None:
        self.id = plan.target_id
        self.function = plan.function


def main(argv: Optional[List[str]] = None) -> int:
    parser = argparse.ArgumentParser(
        description=(
            "Append harness-marked placeholder stubs for ready targets whose "
            "mapped source file has no editable function region."
        )
    )
    parser.add_argument("--config", type=Path, default=None)
    parser.add_argument(
        "--selection",
        choices=("ready", "pending", "all"),
        default="ready",
        help="Target frontier (default: ready)",
    )
    parser.add_argument(
        "--tu",
        default=None,
        help="Restrict to one translation unit (e.g. kyoshin/CGame)",
    )
    parser.add_argument(
        "--limit",
        type=int,
        default=0,
        help="Only scaffold the first N plans (0 = all)",
    )
    parser.add_argument(
        "--write",
        action="store_true",
        help="Append stubs to source files (otherwise dry-run)",
    )
    parser.add_argument(
        "--include-header-inline",
        action="store_true",
        help=(
            "Also scaffold symbols that already have an inline body in a related "
            "header (may duplicate definitions; off by default)"
        ),
    )
    parser.add_argument(
        "--keep-sinit",
        action="store_true",
        help="Include __sinit_* static initializer symbols (skipped by default)",
    )
    parser.add_argument(
        "--allow-foreign-class",
        action="store_true",
        help=(
            "Allow member stubs whose class differs from the TU basename "
            "(e.g. CfObject methods attributed to CfGameManager.cpp; off by default)"
        ),
    )
    parser.add_argument(
        "--allow-undeclared",
        action="store_true",
        help=(
            "Allow member stubs without a matching header declaration "
            "(may fail to compile; off by default)"
        ),
    )
    parser.add_argument(
        "--allow-anon-ns",
        action="store_true",
        help=(
            "Also scaffold symbols inside anonymous namespaces (@unnamed@ link "
            "names) by reconstructing the nested namespace + signature from the "
            "mangling, and free templates (e.g. Min<Uc>) via a forced typed "
            "address-take. Off by default (thunks and anon-ns class members "
            "remain un-stubbable)."
        ),
    )
    parser.add_argument(
        "--status",
        default="NOT_STARTED",
        help="Only consider targets with this match status (default: NOT_STARTED)",
    )
    args = parser.parse_args(argv)

    config = load_config(args.config, ROOT)
    project = Project(config)
    raw = load_targets(config)
    if args.selection == "all":
        pool = [
            t
            for t in raw
            if t.status == args.status
            and t.kind == "function"
            and t.symbol
            and t.source is not None
            and t.source.is_file()
        ]
    else:
        pool = [
            t
            for t in harness_targets(
                raw, selection=args.selection, include_catalog=True
            )
            if t.status == args.status
            and t.symbol
            and t.source is not None
            and t.source.is_file()
        ]

    plans, skips = collect_plans(
        pool,
        project=project,
        project_root=config.project_root,
        include_header_inline=args.include_header_inline,
        skip_sinit=not args.keep_sinit,
        allow_undeclared=args.allow_undeclared,
        allow_foreign_class=args.allow_foreign_class,
        allow_anon_ns=args.allow_anon_ns,
        tu=args.tu,
    )
    if args.limit > 0:
        plans = plans[: args.limit]

    skip_counts: Dict[str, int] = defaultdict(int)
    for skip in skips:
        skip_counts[skip.reason] += 1

    by_file: Dict[Path, int] = defaultdict(int)
    for plan in plans:
        by_file[plan.source] += 1

    print(
        f"pool={len(pool)} planned={len(plans)} skipped={len(skips)} "
        f"files={len(by_file)}"
    )
    if skip_counts:
        print(
            "skips: "
            + " ".join(f"{key}={value}" for key, value in sorted(skip_counts.items()))
        )
    for plan in plans[:20]:
        rel = plan.source.relative_to(config.project_root)
        print(
            f"  {plan.target_id}: {plan.return_type} {plan.qualified_name}{plan.args} "
            f"-> {rel}"
        )
    if len(plans) > 20:
        print(f"  ... and {len(plans) - 20} more")

    header_inline_skips = [s for s in skips if s.reason == "header_inline"]
    if header_inline_skips and not args.include_header_inline:
        print(
            f"note: {len(header_inline_skips)} symbols skipped as header_inline "
            "(pass --include-header-inline to force)"
        )
        for skip in header_inline_skips[:5]:
            print(f"  skip {skip.target_id}: {skip.detail}")

    if not args.write:
        print("dry-run only; pass --write to append stubs")
        return 0

    if not plans:
        print("nothing to write")
        return 0

    changed, errors = apply_plans(plans, project_root=config.project_root)
    print(f"updated_files={changed} stubs={len(plans)}")
    for err in errors[:20]:
        print(f"error: {err}", file=sys.stderr)
    return 1 if errors else 0


if __name__ == "__main__":
    raise SystemExit(main())
