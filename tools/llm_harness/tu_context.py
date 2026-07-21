"""
TU-level context gathering for Xenoblade decompilation.

This module collects all context needed for a translation-unit-level decompilation prompt:
- Retail assembly for all functions in the TU
- Current source and header
- Matched sibling functions (few-shot examples)
- Callee summaries
- Struct/class context from already-matched TUs
"""
from __future__ import annotations

import json
import re
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any, Dict, List, Optional, Set

from tools.coop.lib.config import load_config
from tools.coop.lib.project import Project
from tools.coop.lib.targets import (
    ACCEPTED_MATCH_STATUSES,
    Target,
    get_target,
    harness_targets,
    load_targets,
    load_targets_document,
)
from tools.llm_harness.asm_listings import (
    assembly_function_block as _assembly_function_block,
    format_instruction_listing,
)
from tools.llm_harness.dossier import PromptConstraints, build_target_dossier, compact_model_facing_dossier, dossier_to_dict
from tools.llm_harness.source_regions import find_function_region as _find_function_region, find_tu_slots as _find_tu_slots, TuSlot
from tools.ppc_equivalence.elf_symbols import extract_function, list_text_functions


@dataclass
class TuContext:
    """Complete context for a TU-level decompilation prompt."""
    unit_name: str
    unit_hint: str
    source_path: Path
    header_path: Optional[Path]
    source_text: str
    header_text: str
    retail_asm: str  # Full retail assembly for the TU
    functions: List[FunctionContext]
    matched_siblings: List[MatchedFunction]
    callee_summaries: Dict[str, CalleeSummary]
    struct_context: str
    prompt_budget: Dict[str, Any]


@dataclass
class FunctionContext:
    """Context for a single function within a TU."""
    target_id: str
    symbol: str
    demangled: str
    status: str
    retail_asm: str
    retail_size: int
    current_source: str
    called_functions: List[str]
    callers: List[str]
    is_matched: bool


@dataclass
class MatchedFunction:
    """A FULL_MATCH/EQUIVALENT_MATCH function for few-shot examples."""
    target_id: str
    symbol: str
    demangled: str
    status: str
    body: str
    retail_size: int


@dataclass
class CalleeSummary:
    """Summary of a callee function for context."""
    target_id: str
    symbol: str
    demangled: str
    status: str
    purpose: str  # Brief description of what the function does
    signature: str  # Return type + params


def _unit_matches(unit: Optional[str], tu: str) -> bool:
    """Check if a target's unit hint matches a given TU name."""
    if unit is None:
        return False
    if tu.endswith(".o"):
        tu = tu[:-2]
    return unit == tu or unit.endswith("/" + tu) or tu.endswith("/" + unit)


def gather_tu_context(
    root: Path,
    unit_hint: str,
    config_path: Optional[Path] = None,
    prompt_budget: Optional[Dict[str, Any]] = None,
) -> TuContext:
    """
    Gather complete context for a translation unit.
    
    Args:
        root: Project root directory
        unit_hint: Unit name (e.g., "kyoshin/cf/CfPadTask" or "CfPadTask.o")
        config_path: Optional path to coop.json
        prompt_budget: Optional prompt budget overrides
        
    Returns:
        TuContext with all gathered information
    """
    config = load_config(config_path, root) if config_path else load_config(None, root)
    project = Project(config)
    
    # Resolve the unit
    unit = project.resolve_unit(unit_hint)
    if unit.source_path is None or not unit.source_path.is_file():
        raise ValueError(f"Unit {unit.name!r} has no source file")
    if unit.target_path is None or not unit.target_path.is_file():
        raise ValueError(f"Unit {unit.name!r} has no retail object")
    
    # Load source and header
    source_text = unit.source_path.read_text(encoding="utf-8")
    header_text = ""
    if unit.source_path is not None:
        header_candidates = [
            unit.source_path.with_suffix(".hpp"),
            unit.source_path.with_suffix(".h"),
            unit.source_path.with_suffix(".hxx"),
        ]
        for hp in header_candidates:
            if hp.is_file():
                header_text = hp.read_text(encoding="utf-8")
                break
    
    # Get all targets in this TU
    all_targets = load_targets(config)
    tu_targets = [t for t in all_targets if t.buildable and t.symbol and t.unit and _unit_matches(t.unit, unit_hint)]
    
    # Build prompt budget
    budget = {
        "max_chars": 60000,
        "max_decoded_instructions": 400,
        "max_declaration_chars": 12000,
        "max_callers": 3,
        "max_sibling_bodies": 3,
        "include_raw_hex": False,
        "max_output_tokens": 8192,
    }
    if prompt_budget:
        budget.update(prompt_budget)
    
    # Get retail assembly for entire TU
    retail_asm = _get_retail_asm_for_tu(root, unit)
    
    # Build function contexts
    functions = []
    matched_siblings = []
    callee_symbols: Set[str] = set()
    
    for target in tu_targets:
        if target.source is None or not target.source.is_file():
            continue
        try:
            source = target.source.read_text(encoding="utf-8")
            region = _find_function_region(source, target)
            current_source = source[region.content_start:region.content_end].strip()
        except (OSError, ValueError):
            current_source = ""
        
        # Get retail asm for this function
        func_asm = _assembly_function_block(retail_asm, target.symbol or "")
        
        # Get retail function size
        retail_size = 0
        try:
            retail_fn = extract_function(unit.target_path, target.symbol or "")
            retail_size = retail_fn.size
        except (OSError, ValueError):
            pass
        
        # Collect callees
        called = target.extra.get("called_functions", []) or []
        callers = target.extra.get("caller_functions", []) or []
        callee_symbols.update(called)
        
        is_matched = target.status in ACCEPTED_MATCH_STATUSES
        
        fctx = FunctionContext(
            target_id=target.id,
            symbol=target.symbol or "",
            demangled=target.function or "",
            status=target.status,
            retail_asm=func_asm,
            retail_size=retail_size,
            current_source=current_source,
            called_functions=called,
            callers=callers,
            is_matched=is_matched,
        )
        functions.append(fctx)
        
        # If matched, add as few-shot example
        if is_matched and current_source:
            matched_siblings.append(MatchedFunction(
                target_id=target.id,
                symbol=target.symbol,
                demangled=target.function or "",
                status=target.status,
                body=current_source,
                retail_size=retail_size,
            ))
    
    # Sort: matched first (for few-shot), then by retail size descending
    matched_siblings.sort(key=lambda m: (-m.retail_size, m.target_id))
    functions.sort(key=lambda f: (not f.is_matched, -f.retail_size, f.target_id))
    
    # Build callee summaries
    callee_summaries = _build_callee_summaries(root, config, callee_symbols)
    
    # Build struct context from matched TUs
    struct_context = _build_struct_context(root, config, tu_targets)
    
    return TuContext(
        unit_name=unit.name,
        unit_hint=unit_hint,
        source_path=unit.source_path,
        header_path=unit.source_path.with_suffix(".hpp") if unit.source_path else None,
        source_text=source_text,
        header_text=header_text,
        retail_asm=retail_asm,
        functions=functions,
        matched_siblings=matched_siblings[:budget.get("max_sibling_bodies", 5)],
        callee_summaries=callee_summaries,
        struct_context=struct_context,
        prompt_budget=budget,
    )


def _get_retail_asm_for_tu(root: Path, unit: Any) -> str:
    """Extract full retail assembly for a TU from the build asm output."""
    candidates = [
        root / "build" / "us" / "asm" / f"{unit.name.removeprefix('main/')}.s",
        root / "build" / "us" / "asm" / f"{unit.name}.s",
    ]
    for path in candidates:
        if path.is_file():
            return path.read_text(encoding="utf-8")
    return ""


def _build_callee_summaries(
    root: Path,
    config: Any,
    callee_symbols: Set[str],
) -> Dict[str, CalleeSummary]:
    """Build summaries for callee functions."""
    summaries = {}
    all_targets = load_targets(config)
    targets_by_symbol = {t.symbol: t for t in all_targets if t.symbol}
    
    for sym in callee_symbols:
        if sym in targets_by_symbol:
            target = targets_by_symbol[sym]
            # Build a brief purpose description based on name/status
            purpose = _infer_purpose(target)
            signature = _infer_signature(target)
            
            summaries[sym] = CalleeSummary(
                target_id=target.id,
                symbol=target.symbol,
                demangled=target.function or "",
                status=target.status,
                purpose=purpose,
                signature=signature,
            )
    
    return summaries


def _infer_purpose(target: Target) -> str:
    """Infer a brief purpose description from function name and status."""
    name = (target.function or target.symbol or "").lower()
    if target.status in ACCEPTED_MATCH_STATUSES:
        status_note = f"[{target.status}]"
    else:
        status_note = f"[{target.status}]"
    
    # Heuristic purpose inference
    if "init" in name or "ctor" in name:
        return f"Constructor/initialization {status_note}"
    if "term" in name or "dtor" in name or "destruct" in name:
        return f"Destructor/cleanup {status_note}"
    if "move" in name or "update" in name or "tick" in name:
        return f"Per-frame update {status_note}"
    if "draw" in name or "render" in name:
        return f"Rendering {status_note}"
    if "create" in name or "alloc" in name or "new" in name:
        return f"Factory/creation {status_note}"
    if "get" in name or "find" in name or "query" in name:
        return f"Query/accessor {status_note}"
    if "set" in name or "add" in name or "insert" in name:
        return f"Mutation/setter {status_note}"
    if "cb" in name or "callback" in name:
        return f"Callback {status_note}"
    return f"Function {status_note}"


def _infer_signature(target: Target) -> str:
    """Infer a C++-style signature from target info."""
    demangled = target.function or target.symbol or ""
    # Try to extract return type and params from demangled name
    # This is a rough heuristic - real signature would need DWARF or header
    return demangled


def _build_struct_context(
    root: Path,
    config: Any,
    tu_targets: List[Target],
) -> str:
    """Build struct/class context from already-matched TUs."""
    # Find all classes referenced by this TU's targets
    class_names = set()
    for target in tu_targets:
        # Extract class names from demangled function names
        demangled = target.function or target.symbol or ""
        # Match patterns like ClassName::MethodName
        matches = re.findall(r'(\w+)::', demangled)
        class_names.update(matches)
    
    # Also look at called functions for class names
    for target in tu_targets:
        for callee in target.extra.get("called_functions", []) or []:
            t = get_target(load_targets(config), callee)
            if t and t.function:
                matches = re.findall(r'(\w+)::', t.function)
                class_names.update(matches)
    
    # Gather struct definitions from matched TUs
    all_targets = load_targets(config)
    struct_defs = []
    
    # Look for headers that define these classes
    for cls_name in sorted(class_names):
        # Search for header files
        for target in all_targets:
            if target.source and target.source.is_file():
                try:
                    source = target.source.read_text(encoding="utf-8")
                    if f"class {cls_name}" in source or f"struct {cls_name}" in source:
                        # Extract the class definition
                        class_def = _extract_class_definition(source, cls_name)
                        if class_def:
                            struct_defs.append(f"// From {target.source.relative_to(root)}\n{class_def}")
                            break
                except OSError:
                    pass
    
    if not struct_defs:
        return "// No struct context available from matched TUs"
    
    return "\n\n".join(struct_defs[:10])  # Limit to avoid budget overflow


def _extract_class_definition(source: str, class_name: str) -> str:
    """Extract a class/struct definition from source text."""
    # Simple extraction - find class/struct and matching braces
    patterns = [
        rf"class\s+{re.escape(class_name)}\b.*?\n\s*\}};",
        rf"struct\s+{re.escape(class_name)}\b.*?\n\s*\}};",
    ]
    for pattern in patterns:
        match = re.search(pattern, source, re.DOTALL)
        if match:
            return match.group(0).strip()
    return ""


def format_tu_prompt(context: TuContext) -> str:
    """
    Format the TU context into a prompt for the LLM.
    
    This produces the complete prompt that gets sent to the model.
    """
    budget = context.prompt_budget
    max_chars = budget.get("max_chars", 60000)
    max_output = budget.get("max_output_tokens", 8192)
    
    # Build the dossier-like JSON for the model
    dossier = {
        "schema_version": 3,
        "repository": "xenoblade-wii-us",
        "workflow": "tu-decomp",
        "acceptance": "100% instruction match (FULL_MATCH) or EQUIVALENT_MATCH with certificate",
        "unit": context.unit_name,
        "source": str(context.source_path),
        "header": context.header_text[:budget.get("max_declaration_chars", 12000)] if context.header_text else "",
        "functions": [],
        "matched_siblings": [],
        "callee_summaries": [],
        "struct_context": context.struct_context,
        "constraints": {
            "max_output_tokens": max_output,
            "high_level_cpp_only": True,
            "no_inline_asm": True,
            "trust_asm_over_header": True,
            "consistent_layout_across_functions": True,
        },
    }
    
    # Add function contexts
    for f in context.functions:
        dossier["functions"].append({
            "target_id": f.target_id,
            "symbol": f.symbol,
            "demangled": f.demangled,
            "status": f.status,
            "retail_asm": f.retail_asm[:budget.get("max_decoded_instructions", 400) * 20] if f.retail_asm else "",
            "retail_size": f.retail_size,
            "current_source": f.current_source[:1000] if f.current_source else "",
            "called_functions": f.called_functions,
            "callers": f.callers,
            "is_matched": f.is_matched,
        })
    
    # Add matched siblings (few-shot examples)
    for m in context.matched_siblings:
        dossier["matched_siblings"].append({
            "target_id": m.target_id,
            "symbol": m.symbol,
            "demangled": m.demangled,
            "status": m.status,
            "body": m.body[:budget.get("max_declaration_chars", 12000)],
            "retail_size": m.retail_size,
        })
    
    # Add callee summaries
    for sym, callee in context.callee_summaries.items():
        dossier["callee_summaries"].append({
            "symbol": callee.symbol,
            "demangled": callee.demangled,
            "status": callee.status,
            "purpose": callee.purpose,
            "signature": callee.signature,
        })
    
    # Compact the dossier
    dossier = compact_model_facing_dossier(dossier)
    
    # Read prompt templates
    prompt_dir = context.source_path.parent / "tools" / "llm_harness" / "prompts"
    # Actually, we need the project root - let's find it
    # The prompt dir is passed via adapter, so we'll use a placeholder
    # The actual prompt rendering happens in the adapter
    
    return json.dumps(dossier, separators=(",", ":"), indent=None)


def render_tu_prompt(dossier_json: str, prompt_dir: Path) -> str:
    """Render the final prompt from dossier JSON and templates."""
    common = (prompt_dir / "common.md").read_text(encoding="utf-8")
    workflow = (prompt_dir / "tu-decomp.md").read_text(encoding="utf-8")
    tu_shared = (prompt_dir / "tu-shared.md").read_text(encoding="utf-8") if (prompt_dir / "tu-shared.md").is_file() else ""
    
    # The common.md template expects {{WORKFLOW_PROMPT}} and {{DOSSIER_JSON}}
    # and {{TU_SHARED}} for tu-shared.md content
    prompt = common.replace("{{WORKFLOW_PROMPT}}", workflow)
    prompt = prompt.replace("{{DOSSIER_JSON}}", dossier_json)
    prompt = prompt.replace("{{TU_SHARED}}", tu_shared)
    
    return prompt