"""
TU-level evaluation and splitting logic for Xenoblade decompilation.

This module:
1. Parses the LLM's two-phase TU response (header + cpp)
2. Splits the .cpp into per-function bodies matching TU slots
3. Creates SourcePatch objects for the harness to evaluate
4. Handles the two-phase output format
"""
from __future__ import annotations

import json
import re
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any, Dict, List, Optional, Tuple

from tools.llm_harness.source_regions import (
    SourceRegion,
    TuSlot,
    find_function_region as _find_function_region,
    find_tu_slots as _find_tu_slots,
    tu_begin_marker as _tu_begin_marker,
    tu_end_marker as _tu_end_marker,
)
from tools.llm_harness.types import Candidate, SourcePatch


def strip_harness_markers(source: str) -> str:
    """Strip LLM-HARNESS marker comments from model output.
    
    Removes lines like:
      // LLM-HARNESS-BEGIN: us-8007eb50
      // LLM-HARNESS-END: us-8007eb50
      // LLM-HARNESS-TU-BEGIN: function:target_123
      // LLM-HARNESS-TU-END: function:target_123
    
    Also strips blank lines left by marker removal (single pass).
    """
    stripped = re.sub(
        r'(?m)^[ \t]*// LLM-HARNESS-(?:TU-)?(?:BEGIN|END): .*$',
        '',
        source,
    )
    # Collapse 2+ consecutive blank lines into 1
    stripped = re.sub(r'\n{3,}', '\n\n', stripped)
    return stripped.strip()


@dataclass
class TuDecompResponse:
    """Parsed response from the TU decompilation model."""
    phase1_header: str
    phase2_cpp: str
    hypothesis: str
    notes: List[str]
    next_change: str


@dataclass
class FunctionBody:
    """A single function body extracted from the TU source."""
    symbol: str
    target_id: str
    body: str
    start_line: int
    end_line: int


def parse_tu_response(response_text: str) -> TuDecompResponse:
    """
    Parse the model's JSON response into a structured object.
    
    Expected format:
    {
        "phase1_header": "...",
        "phase2_cpp": "...",
        "hypothesis": "...",
        "notes": [...],
        "next_change": "..."
    }
    """
    try:
        data = json.loads(response_text.strip())
    except json.JSONDecodeError as e:
        # Try to extract JSON from markdown fences if present
        match = re.search(r'```(?:json)?\s*(\{.*?\})\s*```', response_text, re.DOTALL)
        if match:
            data = json.loads(match.group(1))
        else:
            raise ValueError(f"Failed to parse TU response as JSON: {e}")
    
    return TuDecompResponse(
        phase1_header=data.get("phase1_header", ""),
        phase2_cpp=data.get("phase2_cpp", ""),
        hypothesis=data.get("hypothesis", ""),
        notes=data.get("notes", []),
        next_change=data.get("next_change", ""),
    )


def split_tu_into_functions(
    cpp_source: str,
    tu_slots: Dict[str, TuSlot],
    target_map: Dict[str, Tuple[str, str]],  # slot_id -> (symbol, target_id)
) -> List[FunctionBody]:
    """
    Split the complete TU .cpp source into per-function bodies.
    
    Args:
        cpp_source: Complete TU source from phase2_cpp
        tu_slots: Map of slot_id -> TuSlot (from source_regions.find_tu_slots)
        target_map: Map of slot_id -> (symbol, target_id) for function slots
    
    Returns:
        List of FunctionBody objects, one per function slot
    """
    results = []
    
    for slot_id, slot in tu_slots.items():
        if not slot_id.startswith("function:"):
            continue
        
        symbol, target_id = target_map.get(slot_id, ("", ""))
        if not symbol or not target_id:
            continue
        
        # Extract the function body from the source using slot boundaries
        # The slot's content_start/content_end mark the function body region
        if slot.content_start < len(cpp_source) and slot.content_end <= len(cpp_source):
            body = cpp_source[slot.content_start:slot.content_end].strip()
        else:
            # Fallback: try to find the function by name
            body = _extract_function_by_name(cpp_source, symbol)
        
        if body:
            results.append(FunctionBody(
                symbol=symbol,
                target_id=target_id,
                body=body,
                start_line=cpp_source[:slot.content_start].count('\n') + 1,
                end_line=cpp_source[:slot.content_end].count('\n') + 1,
            ))
    
    return results


def _extract_function_by_name(source: str, symbol: str) -> str:
    """Fallback: extract function body by searching for symbol in source."""
    # Look for the function definition pattern
    # This is a rough heuristic - proper implementation would use source_regions
    pattern = rf'{re.escape(symbol)}\s*\([^)]*\)\s*\{{'
    match = re.search(pattern, source)
    if not match:
        return ""
    
    start = match.end() - 1  # Position of opening brace
    # Find matching closing brace
    brace_count = 0
    for i, ch in enumerate(source[start:], start=start):
        if ch == '{':
            brace_count += 1
        elif ch == '}':
            brace_count -= 1
            if brace_count == 0:
                return source[match.start():i+1].strip()
    return ""


def create_function_patches(
    function_bodies: List[FunctionBody],
    original_source: str,
    tu_slots: Dict[str, TuSlot],
) -> List[SourcePatch]:
    """
    Create SourcePatch objects for each function from the TU response.
    
    The patches use the original source's slot boundaries so the harness
    can splice them in correctly.
    """
    patches = []
    
    for fb in function_bodies:
        slot_id = f"function:{fb.target_id}"
        slot = tu_slots.get(slot_id)
        
        if slot is None:
            continue
        
        # Validate the new source doesn't add forbidden patterns
        _validate_source_patch(original_source[slot.content_start:slot.content_end], fb.body)
        
        patches.append(SourcePatch(
            slot_id=slot_id,
            source=fb.body,
        ))
    
    return patches


def _validate_source_patch(original: str, candidate: str) -> None:
    """Validate that the candidate source doesn't introduce forbidden patterns."""
    forbidden = ("asm {", "asm void", "register r", 'asm("r', "sp[")
    added = [token for token in forbidden if candidate.count(token) > original.count(token)]
    if added:
        raise ValueError(f"Candidate adds forbidden low-level patterns: {', '.join(added)}")


def build_tu_candidate(
    tu_response: TuDecompResponse,
    original_source: str,
    tu_slots: Dict[str, TuSlot],
    target_map: Dict[str, Tuple[str, str]],
) -> Candidate:
    """
    Build a Candidate object from the TU decompilation response.
    
    This creates patches for each function slot and includes the improved header
    as a separate patch if it differs from the original.
    """
    # Split the CPP into function bodies
    function_bodies = split_tu_into_functions(
        tu_response.phase2_cpp,
        tu_slots,
        target_map,
    )
    
    # Create patches for each function
    patches = create_function_patches(function_bodies, original_source, tu_slots)
    
    # TODO: Also create a header patch if phase1_header differs from current header
    # This would require finding the header slot or creating a TU-level slot
    
    return Candidate(
        source=tu_response.phase2_cpp,  # Full TU source for reference
        hypothesis=tu_response.hypothesis,
        notes=tu_response.notes,
        next_change=tu_response.next_change,
        patches=patches,
    )


def evaluate_tu_candidate(
    adapter: Any,  # XenobladeAdapter
    unit_hint: str,
    candidate: Candidate,
) -> Any:  # Evaluation
    """
    Evaluate a TU-level candidate using the existing TU evaluation logic.
    
    This delegates to the adapter's _evaluate_tu method which handles
    building, objdiff, and equivalence checking at the TU level.
    """
    # The adapter's evaluate method already handles "tu-complete" workflow
    return adapter.evaluate("tu-complete", unit_hint, candidate)


def extract_tu_slots_and_targets(
    adapter: Any,
    unit_hint: str,
) -> Tuple[Dict[str, TuSlot], Dict[str, Tuple[str, str]]]:
    """
    Extract TU slots and build target map from the project.
    
    Returns:
        (tu_slots, target_map) where target_map is slot_id -> (symbol, target_id)
    """
    unit = adapter.project.resolve_unit(unit_hint)
    if unit.source_path is None:
        raise ValueError(f"Unit {unit.name!r} has no source path")
    
    source = unit.source_path.read_text(encoding="utf-8")
    slots = _find_tu_slots(source)
    
    # Add function slots from mismatched targets (mirroring _function_tu_slots logic)
    from tools.coop.lib.targets import load_targets
    from tools.llm_harness.xenoblade_project import _unit_matches
    
    targets = load_targets(adapter.config)
    wanted_symbols = set()
    for target in targets:
        if not target.symbol or target.symbol in wanted_symbols or not target.unit:
            continue
        try:
            if (_unit_matches(target.unit, unit_hint) and 
                target.source and target.source.is_file() and
                adapter.project.resolve_unit(target.unit).name == unit.name):
                region = _find_function_region(source, target)
                bounds = (region.content_start, region.content_end)
                if bounds not in {(s.content_start, s.content_end) for s in slots.values()}:
                    slot_id = f"function:{target.id}"
                    slots[slot_id] = TuSlot(
                        slot_id=slot_id,
                        kind="function",
                        start=region.start,
                        end=region.end,
                        content_start=region.content_start,
                        content_end=region.content_end,
                    )
                    wanted_symbols.add(target.symbol)
        except (OSError, ValueError):
            continue
    
    # Build target map
    target_map = {}
    for target in targets:
        if target.symbol and target.unit and _unit_matches(target.unit, unit_hint):
            slot_id = f"function:{target.id}"
            if slot_id in slots:
                target_map[slot_id] = (target.symbol, target.id)
    
    return slots, target_map