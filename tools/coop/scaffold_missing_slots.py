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
from tools.llm_harness.source_regions import (
    begin_marker,
    end_marker,
    find_function_region,
)
from tools.symbolrecover.lib.mwcc import demangle_symbol

_INCLUDE_RE = re.compile(r'^\s*#\s*include\s+(?:"([^"]+)"|<([^>]+)>)', re.M)
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


def collect_plans(
    targets: Sequence[Target],
    *,
    project: Project,
    project_root: Path,
    include_header_inline: bool,
    skip_sinit: bool,
    allow_undeclared: bool,
    allow_foreign_class: bool,
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
        if _is_thunk_symbol(target.symbol) or _symbol_has_invalid_identifier_chars(
            target.symbol
        ):
            skips.append(
                SkipPlan(
                    target.id,
                    target.symbol,
                    target.function,
                    target.source,
                    "thunk" if _is_thunk_symbol(target.symbol) else "invalid_symbol",
                    "linker name contains '@' (thunk/anonymous namespace); "
                    "cannot emit as a C++ identifier",
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
