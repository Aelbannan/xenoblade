#!/usr/bin/env python3
"""Create C++ class headers for unmatched targets that lack them.

Groups unmatched targets by class (from MWCC demangling), creates a proper
header with class declaration and method stubs, and adds the #include to
the source file.

Usage:
    python3 tools/pi_harness/scaffold_headers.py --source <path> --all-unmatched [--dry-run]
    python3 tools/pi_harness/scaffold_headers.py --source <path> [--all-unmatched] --all-files [--dry-run]
"""

from __future__ import annotations

import argparse, json, os, re, sys
from collections import defaultdict
from dataclasses import dataclass
from typing import Optional

REPO_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
sys.path.insert(0, REPO_ROOT)
from tools.symbolrecover.lib.mwcc import demangle_symbol, DemangledSymbol


@dataclass
class MethodStub:
    symbol: str
    function: str
    is_ctor: bool
    is_dtor: bool
    is_const: bool
    args: str  # decoded args string


@dataclass
class ClassToScaffold:
    name: str
    namespace: Optional[str]
    methods: list[MethodStub]
    has_existing_header: bool = False


# ---------------------------------------------------------------------------
# Discovery
# ---------------------------------------------------------------------------

def _build_method(d: DemangledSymbol, raw_symbol: str) -> Optional[MethodStub]:
    """Extract method info from a demangled symbol."""
    cls = d.class_name
    if not cls:
        if d.is_ctor and raw_symbol.startswith("__ct__"):
            m = re.match(r"__ct__([A-Z]\w*)", raw_symbol)
            if m: cls = m.group(1)
        elif d.is_dtor and raw_symbol.startswith("__dt__"):
            m = re.match(r"__dt__([A-Z]\w*)", raw_symbol)
            if m: cls = m.group(1)
    if not cls:
        return None

    return MethodStub(
        symbol=raw_symbol,
        function=d.function,
        is_ctor=d.is_ctor,
        is_dtor=d.is_dtor,
        is_const=("CF" in raw_symbol),
        args=d.args or "()",
    )


def discover_classes(repo_root: str, source_path: str) -> list[ClassToScaffold]:
    """Find all classes referenced by unmatched targets in a source file."""
    targets_path = os.path.join(repo_root, "tools", "coop", "targets.json")
    with open(targets_path) as f:
        data = json.load(f)

    skipped = {"FULL_MATCH", "EQUIVALENT_MATCH"}
    src_rel = source_path[len(repo_root):].lstrip("/") if source_path.startswith(repo_root) else source_path

    by_class: dict[str, list[MethodStub]] = defaultdict(list)
    for t in data.get("targets", []):
        if t.get("status") in skipped or t.get("kind") != "function":
            continue
        if t.get("source") not in (src_rel, source_path):
            continue
        d = demangle_symbol(t["symbol"])
        m = _build_method(d, t["symbol"])
        if m:
            cls = d.class_name
            if not cls:
                if d.is_ctor and t["symbol"].startswith("__ct__"):
                    mm = re.match(r"__ct__([A-Z]\w*)", t["symbol"])
                    if mm: cls = mm.group(1)
                elif d.is_dtor and t["symbol"].startswith("__dt__"):
                    mm = re.match(r"__dt__([A-Z]\w*)", t["symbol"])
                    if mm: cls = mm.group(1)
            # Skip invalid class names: templates, nested mangling artifacts
            if cls and not re.search(r'[<>]|^Q\d|Fv$|^\d', cls):
                by_class[cls].append(m)

    # Determine namespace from first method
    results = []
    for cls, methods in by_class.items():
        ns = None
        # Check all demangled symbols for namespace info
        for t in data.get("targets", []):
            d = demangle_symbol(t["symbol"])
            if d.class_name == cls and d.namespace:
                ns = d.namespace
                break
        results.append(ClassToScaffold(name=cls, namespace=ns, methods=methods))

    return results


def _resolve_header(source_path: str) -> Optional[str]:
    """Find existing header for a source file."""
    hpp = source_path.replace(".cpp", ".hpp").replace(".c", ".hpp")
    if os.path.exists(hpp):
        return hpp
    m = re.match(r"((?:libs|src)/([^/]+))/src/(.+)\.(?:cpp|c)", source_path)
    if m:
        alt = f"{m.group(1)}/include/{m.group(2)}/{m.group(3)}.hpp"
        if os.path.exists(alt):
            return alt
    return None


def _class_in_header(header_path: str, class_name: str) -> bool:
    """Check if a class definition (not forward decl) exists in a header."""
    try:
        with open(header_path) as f:
            content = f.read()
    except Exception:
        return False
    # Match "class ClassName" not followed by ";"
    return bool(re.search(r'\bclass\s+' + re.escape(class_name) + r'\b(?!\s*;)', content))


# ---------------------------------------------------------------------------
# Header generation
# ---------------------------------------------------------------------------

def _infer_includes(source_path: str, classes: list[ClassToScaffold]) -> list[str]:
    """Determine #include lines for a new header."""
    includes = ["#pragma once", "", "#include <types.h>"]

    # Check source file for existing includes that suggest base classes
    try:
        with open(source_path) as f:
            src_content = f.read()
    except Exception:
        return includes

    # Find likely base class includes
    for m in re.finditer(r'#include\s+[<"]([^>"]+)[>"]', src_content):
        inc = m.group(1)
        base = os.path.basename(inc).replace(".hpp", "").replace(".h", "")
        # Include headers for classes that look like base classes
        if base in {c.name for c in classes}:
            continue  # don't self-include
        if base.startswith("C") and base not in ("CUtil", "CMath", "CTypes", "CCol4", "CRect", "CFixStr", "CProcess"):
            includes.append(f'#include "{inc}"')

    # Add work.hpp and IWorkEvent if needed
    for cls in classes:
        for m in cls.methods:
            if "WorkEvent" in m.args:
                if '#include "monolib/work.hpp"' not in includes:
                    includes.append('#include "monolib/work.hpp"')
                break

    includes.append("")
    return includes


def _generate_header(source_path: str, classes: list[ClassToScaffold]) -> str:
    """Generate a complete header file for the given classes."""
    includes = _infer_includes(source_path, classes)
    lines = list(includes)

    for cls in classes:
        ns_open = ns_close = ""
        if cls.namespace:
            lines.append(f"namespace {cls.namespace} {{")
            lines.append("")
            ns_close = f"}} // namespace {cls.namespace}\n"

        lines.append(f"class {cls.name} {{")
        lines.append("public:")

        # Separate ctors/dtors from regular methods
        specials = [m for m in cls.methods if m.is_ctor or m.is_dtor]
        regulars = [m for m in cls.methods if not m.is_ctor and not m.is_dtor]

        seen = set()
        for m in specials:
            if m.is_ctor:
                decl = f"    {cls.name}();"
            else:
                decl = f"    virtual ~{cls.name}();"
            if decl not in seen:
                seen.add(decl)
                lines.append(decl)

        for m in regulars:
            const = " const" if m.is_const else ""
            lines.append(f"    void {m.function}(){const};")

        lines.append("")
        lines.append("    // TODO: add fields")
        lines.append("};")
        lines.append(ns_close)

    return "\n".join(lines) + "\n"


def _add_include_to_source(source_path: str, header_rel_path: str) -> bool:
    """Add #include for the new header to the source file."""
    include_line = f'#include "{header_rel_path}"'
    try:
        with open(source_path) as f:
            content = f.read()
    except Exception:
        return False

    if include_line in content:
        return True

    # Insert after last existing include
    last_include_end = 0
    for m in re.finditer(r'(#include\s+[<"][^>"]+[>"])\s*\n', content):
        last_include_end = m.end()

    if last_include_end > 0:
        content = content[:last_include_end] + include_line + "\n" + content[last_include_end:]
    else:
        # Insert at top after any comments
        content = include_line + "\n" + content

    try:
        with open(source_path, "w", encoding="utf-8", newline="\n") as f:
            f.write(content)
        return True
    except Exception:
        return False


# ---------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------

def main() -> int:
    parser = argparse.ArgumentParser(description="Create missing class headers")
    parser.add_argument("--source", required=True, help="Path to source file")
    parser.add_argument("--all-unmatched", action="store_true",
                        help="Process all unmatched targets for this source")
    parser.add_argument("--dry-run", action="store_true")
    args = parser.parse_args()

    source_path = args.source
    classes = discover_classes(REPO_ROOT, source_path)

    if not classes:
        print("No classes found for this source")
        return 0

    existing_header = _resolve_header(source_path)

    # Filter to classes that don't have headers
    new_classes = []
    for cls in classes:
        if existing_header and _class_in_header(existing_header, cls.name):
            continue
        new_classes.append(cls)

    if not new_classes:
        print("All classes already have headers")
        return 0

    if existing_header:
        print(f"Classes to add to {existing_header}:")
    else:
        hpp_path = source_path.replace(".cpp", ".hpp").replace(".c", ".hpp")
        print(f"Would create: {hpp_path}")
    for cls in new_classes:
        print(f"  class {cls.name} ({len(cls.methods)} methods)")

    if args.dry_run:
        print("\n--- DRY RUN ---")
        return 0

    if existing_header:
        # Add classes to existing header
        header_code = _generate_header(source_path, new_classes)
        # Remove #pragma once and includes since header already has them
        header_code = re.sub(r'^#pragma once\n\n#include.*?\n\n', '', header_code, flags=re.DOTALL)
        try:
            with open(existing_header, "a", encoding="utf-8", newline="\n") as f:
                f.write("\n" + header_code)
            print(f"Added {len(new_classes)} class(es) to {existing_header}")
        except Exception as e:
            print(f"Error writing header: {e}", file=sys.stderr)
            return 1
    else:
        # Create new header
        hpp_path = source_path.replace(".cpp", ".hpp").replace(".c", ".hpp")
        header_code = _generate_header(source_path, new_classes)
        try:
            os.makedirs(os.path.dirname(hpp_path), exist_ok=True)
            with open(hpp_path, "w", encoding="utf-8", newline="\n") as f:
                f.write(header_code)
            print(f"Created {hpp_path}")
        except Exception as e:
            print(f"Error creating header: {e}", file=sys.stderr)
            return 1

        # Add include to source
        hpp_rel = hpp_path
        for base in [REPO_ROOT + "/src/", REPO_ROOT + "/libs/"]:
            if hpp_path.startswith(base):
                hpp_rel = hpp_path[len(base):]
                break
        _add_include_to_source(source_path, hpp_rel)
        print(f"Added #include to {source_path}")

    print(f"\nDone: {len(new_classes)} class(es) scaffolded")
    return 0


if __name__ == "__main__":
    sys.exit(main())
