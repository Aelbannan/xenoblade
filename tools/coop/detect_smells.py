#!/usr/bin/env python3
"""
Detect code smells in FULL_MATCH / EQUIVALENT_MATCH functions:
1. Mangled / placeholder names (func_XXXXXXXX, UnkClass/Class_ patterns)
2. void* parameters that should be properly typed

Outputs JSON report. Cross-references targets.json against actual source files
to avoid counting already-renamed functions.
"""
import json, os, re, sys
from pathlib import Path
from collections import defaultdict

REPO_ROOT = Path(__file__).resolve().parent.parent.parent

def load_targets():
    with open(REPO_ROOT / "tools" / "coop" / "targets.json") as f:
        return json.load(f)

def is_kyoshin(src: str) -> bool:
    return src and "kyoshin" in src

def is_func_placeholder(name: str) -> bool:
    return bool(re.match(r"^func_[0-9a-fA-F]{7,8}$", name))

def has_unkclass(name: str, symbol: str) -> bool:
    return "UnkClass" in name or "UnkClass" in symbol or \
           bool(re.match(r"^Class_[0-9a-fA-F]+", name))

def get_void_ptr_params(source_file: str, targets_in_file: list) -> list:
    if not os.path.exists(source_file):
        return []
    try:
        with open(source_file) as f:
            lines = f.readlines()
    except Exception:
        return []
    results = []
    target_by_func = {}
    for t in targets_in_file:
        fn = t.get("function", "")
        sym = t.get("symbol", "")
        target_by_func[fn] = t
        target_by_func[sym] = t
    func_def_re = re.compile(
        r"^\s*(?:(?:static|inline|extern|virtual|volatile)\s+)*"
        r"([a-zA-Z_]\w*(?:\s*\*)?(?:\s*<[^>]*>)?)\s+"
        r"((?:[a-zA-Z_]\w*\s*::\s*)*[a-zA-Z_]\w*)"
        r"\s*\(([^)]*)\)"
    )
    for i, line in enumerate(lines, 1):
        m = func_def_re.search(line)
        if not m:
            continue
        return_type = m.group(1).strip()
        func_name = m.group(2).strip()
        params = m.group(3).strip()
        if "void*" not in params and "void *" not in params:
            continue
        matched_target = target_by_func.get(func_name)
        if not matched_target:
            for fn, t in target_by_func.items():
                if fn == func_name or t.get("symbol", "") == func_name:
                    matched_target = t
                    break
        if matched_target and matched_target.get("status") in ("FULL_MATCH", "EQUIVALENT_MATCH"):
            results.append({
                "target_id": matched_target.get("id", ""),
                "function_name": func_name,
                "symbol": matched_target.get("symbol", ""),
                "status": matched_target.get("status", ""),
                "signature": line.strip(),
                "line_number": i,
                "source_file": source_file,
                "params": params,
                "return_type": return_type,
            })
    return results

def scan_source_for_func_placeholders(source_file: str, targets_in_file: list) -> list:
    if not os.path.exists(source_file):
        return []
    try:
        with open(source_file) as f:
            content = f.read()
    except Exception:
        return []
    func_defs = re.findall(r'\b(func_[0-9a-fA-F]{7,8})\b', content)
    func_set = set(func_defs)
    target_by_func = {}
    for t in targets_in_file:
        fn = t.get("function", "")
        sym = t.get("symbol", "")
        if fn: target_by_func[fn] = t
        if sym: target_by_func[sym] = t
    results = []
    for func_name in func_set:
        t = target_by_func.get(func_name)
        if t and t.get("status") in ("FULL_MATCH", "EQUIVALENT_MATCH"):
            results.append({
                "target_id": t.get("id", ""),
                "function_name": func_name,
                "symbol": t.get("symbol", ""),
                "status": t.get("status", ""),
                "source_file": source_file,
                "issue_type": "func_placeholder",
                "unit": t.get("unit", ""),
            })
    return results

def detect_mangled_names(targets, check_source=False):
    issues = []
    seen_ids = set()
    for t in targets:
        if t.get("status") not in ("FULL_MATCH", "EQUIVALENT_MATCH"):
            continue
        fn = t.get("function", "")
        sym = t.get("symbol", "")
        src = t.get("source", "")
        issue_type = None
        if is_func_placeholder(fn):
            issue_type = "func_placeholder"
        elif has_unkclass(fn, sym):
            issue_type = "unkclass_placeholder"
        if issue_type:
            tid = t.get("id", "")
            seen_ids.add(tid)
            issues.append({
                "target_id": tid,
                "function_name": fn,
                "symbol": sym,
                "status": t.get("status", ""),
                "source_file": src,
                "issue_type": issue_type,
                "unit": t.get("unit", ""),
            })
    # Filter out already-renamed (func_ name absent from source)
    source_contents = {}
    for issue in issues:
        src = issue.get("source_file", "")
        if src and src not in source_contents and os.path.exists(src):
            try:
                with open(src) as f:
                    source_contents[src] = f.read()
            except Exception:
                source_contents[src] = ""
    filtered = []
    for issue in issues:
        src = issue.get("source_file", "")
        fn = issue.get("function_name", "")
        if src in source_contents:
            content = source_contents[src]
            if re.search(r'\b' + re.escape(fn) + r'\b', content):
                filtered.append(issue)
        else:
            filtered.append(issue)
    if check_source:
        targets_by_source = defaultdict(list)
        for t in targets:
            src = t.get("source", "")
            if src: targets_by_source[src].append(t)
        for src, file_targets in targets_by_source.items():
            if not is_kyoshin(src):
                continue
            for si in scan_source_for_func_placeholders(src, file_targets):
                if si["target_id"] not in seen_ids:
                    seen_ids.add(si["target_id"])
                    filtered.append(si)
    return filtered

def main():
    data = load_targets()
    targets = data["targets"]
    targets_by_source = defaultdict(list)
    for t in targets:
        src = t.get("source", "")
        if src: targets_by_source[src].append(t)

    mangled_issues = detect_mangled_names(targets, check_source=True)
    kyoshin_mangled = [i for i in mangled_issues if is_kyoshin(i.get("source_file", ""))]
    libs_mangled = [i for i in mangled_issues if not is_kyoshin(i.get("source_file", ""))]

    void_ptr_issues = []
    for src in [s for s in targets_by_source if is_kyoshin(s)]:
        void_ptr_issues.extend(get_void_ptr_params(src, targets_by_source[src]))

    report = {
        "summary": {
            "total_kyoshin_mangled": len(kyoshin_mangled),
            "total_libs_mangled": len(libs_mangled),
            "total_void_ptr": len(void_ptr_issues),
            "unique_sources_with_mangled": len(set(i["source_file"] for i in kyoshin_mangled)),
            "unique_sources_with_void_ptr": len(set(i["source_file"] for i in void_ptr_issues)),
        },
        "kyoshin_mangled_names": kyoshin_mangled,
        "libs_mangled_names": libs_mangled,
        "void_ptr_params": void_ptr_issues,
    }
    report["kyoshin_by_source"] = defaultdict(list)
    for i in kyoshin_mangled:
        report["kyoshin_by_source"][i["source_file"]].append(i)
    report["kyoshin_by_source"] = dict(report["kyoshin_by_source"])
    report["void_ptr_by_source"] = defaultdict(list)
    for i in void_ptr_issues:
        report["void_ptr_by_source"][i["source_file"]].append(i)
    report["void_ptr_by_source"] = dict(report["void_ptr_by_source"])
    return report

if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser(description="Detect code smells in FULL_MATCH/EQUIVALENT_MATCH functions")
    parser.add_argument("--json", action="store_true", default=True)
    parser.add_argument("--summary", action="store_true")
    parser.add_argument("--source", type=str)
    parser.add_argument("--top-sources", type=int, default=0)
    args = parser.parse_args()
    report = main()
    if args.source:
        report["kyoshin_mangled_names"] = [i for i in report["kyoshin_mangled_names"] if i["source_file"] == args.source]
        report["void_ptr_params"] = [i for i in report["void_ptr_params"] if i["source_file"] == args.source]
    if args.summary:
        print(json.dumps(report["summary"], indent=2))
        if args.top_sources:
            by_src = report["kyoshin_by_source"]
            top = sorted(by_src.items(), key=lambda x: len(x[1]), reverse=True)[:args.top_sources]
            print(f"\nTop {args.top_sources} sources by mangled name count:")
            for src, issues in top:
                void_count = len(report["void_ptr_by_source"].get(src, []))
                print(f"  {src}: {len(issues)} mangled, {void_count} void*")
    else:
        print(json.dumps({"summary": report["summary"], "kyoshin_mangled_names": report["kyoshin_mangled_names"], "void_ptr_params": report["void_ptr_params"]}, indent=2, default=str))
