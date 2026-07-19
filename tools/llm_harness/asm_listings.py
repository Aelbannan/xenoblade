"""Retail asm listing helpers shared by the harness and Decomp Atlas."""

from __future__ import annotations

import re
from typing import Any, Iterable, List, Optional


def assembly_function_block(source: str, symbol: str) -> str:
    """Extract one function from a retail ``build/<region>/asm/*.s`` listing."""
    start_pattern = re.compile(rf'^\.fn\s+"?{re.escape(symbol)}"?\s*,.*$', re.MULTILINE)
    start = start_pattern.search(source)
    if start is None:
        return ""
    end_pattern = re.compile(rf'^\.endfn\s+"?{re.escape(symbol)}"?\s*$', re.MULTILINE)
    end = end_pattern.search(source, start.end())
    if end is None:
        return ""
    line_end = source.find("\n", end.end())
    return source[start.start() : (line_end + 1 if line_end >= 0 else end.end())]


# Objdiff/retail listings prefix each insn with address, file offset, and raw bytes.
_BYTECODE_COMMENT = re.compile(
    r"^[ \t]*/\*[ \t]*[0-9A-Fa-f]+[ \t]+[0-9A-Fa-f]+[ \t]+(?:[0-9A-Fa-f]{2}[ \t]+)+[ \t]*\*/[ \t]*"
)


def strip_listing_bytecode_comments(listing: str) -> str:
    """Drop ``/* VA OFFSET BYTES */`` prefixes; keep mnemonics/operands/.fn markers."""
    if not listing:
        return ""
    out: List[str] = []
    for line in listing.splitlines():
        out.append(_BYTECODE_COMMENT.sub("", line).rstrip())
    # Preserve a trailing newline when the input had one.
    text = "\n".join(out)
    if listing.endswith("\n"):
        text += "\n"
    return text


def format_instruction_listing(
    instructions: Iterable[Any],
    *,
    max_instructions: int = 0,
) -> str:
    """Format decoded PPC instructions into a compact listing for model prompts.

    Accepts either ppc_equivalence ``Instruction`` objects or dossier
    ``DecodedInstruction``-like dicts/objects with address/mnemonic/operands/
    relocation fields.
    """
    lines: List[str] = []
    for index, insn in enumerate(instructions):
        if max_instructions > 0 and index >= max_instructions:
            lines.append(f"... truncated after {max_instructions} instructions")
            break
        if isinstance(insn, dict):
            address = insn.get("address", 0)
            mnemonic = insn.get("mnemonic") or "?"
            operands = insn.get("operands") or ""
            reloc = insn.get("relocation")
            if isinstance(operands, (list, tuple)):
                ops = ", ".join(str(op) for op in operands)
            else:
                ops = str(operands)
            reloc_note = ""
            if isinstance(reloc, dict) and reloc.get("symbol"):
                reloc_note = f"  ; {reloc.get('symbol')}"
            elif isinstance(reloc, str) and reloc:
                reloc_note = f"  ; {reloc}"
        else:
            address = getattr(insn, "address", 0)
            mnemonic = (
                getattr(insn, "display_mnemonic", None)
                or getattr(insn, "mnemonic", None)
                or getattr(getattr(insn, "opcode", None), "value", "?")
            )
            raw_operands = getattr(insn, "operands", ())
            if isinstance(raw_operands, str):
                ops = raw_operands
            elif raw_operands:
                ops = ", ".join(str(op) for op in raw_operands)
            else:
                ops = ""
            reloc = getattr(insn, "relocation", None)
            reloc_note = ""
            if reloc is not None:
                sym = (
                    reloc.get("symbol")
                    if isinstance(reloc, dict)
                    else getattr(reloc, "symbol", None)
                    or getattr(reloc, "canonical_symbol", "")
                )
                if sym:
                    reloc_note = f"  ; {sym}"
        if isinstance(address, str) and address.startswith("0x"):
            addr_text = address
        else:
            try:
                addr_text = f"0x{int(address):08x}"
            except (TypeError, ValueError):
                addr_text = str(address)
        lines.append(f"{addr_text}:  {mnemonic} {ops}".rstrip() + reloc_note)
    return "\n".join(lines)


def compact_type_snippets(
    *,
    header_text: str,
    source_text: str,
    demangled: str,
    reloc_symbols: List[str],
    max_chars: int = 3000,
) -> List[str]:
    """Pull compact declaration lines for the target and referenced symbols.

    Prefer header prototypes / statics over dumping an entire class body or
    call-site noise from the .cpp.
    """
    class_name = ""
    method_name = ""
    match = re.search(r"(\w+)::(\w+)\s*\(", demangled)
    if match:
        class_name, method_name = match.group(1), match.group(2)
    elif demangled:
        method_name = demangled.split("(")[0].strip()

    reloc_heads = []
    for sym in reloc_symbols:
        head = sym.split("__", 1)[0]
        if head and head not in reloc_heads:
            reloc_heads.append(head)

    def _looks_like_decl(line: str) -> bool:
        if "(" in line and ")" in line:
            # Prototype or out-of-line definition — not a call statement.
            if line.endswith(";") or re.match(r"^[\w:<>,\s\*&]+\s+\w+::\w+\s*\(", line):
                return True
            return False
        # Data member / static definition.
        return bool(
            line.startswith("static ")
            or re.match(r"^[\w:<>,\s\*&]+\s+\w+(::\w+)?\s*;", line)
            or re.match(r"^[\w:<>,\s\*&]+\s+\w+::\w+\s*;", line)
        )

    prioritized: List[tuple[int, str]] = []
    seen = set()

    def _consider(blob: str, source_rank_boost: int) -> None:
        if not blob:
            return
        for line in blob.splitlines():
            stripped = line.strip()
            if not stripped or stripped.startswith("//") or stripped.startswith("#"):
                continue
            if stripped in seen:
                continue
            if stripped.startswith(("class ", "struct ", "enum ")):
                if class_name and re.match(
                    rf"(?:class|struct)\s+{re.escape(class_name)}\b", stripped
                ):
                    short = stripped.split("{", 1)[0].rstrip()
                    if not short.endswith(";"):
                        short += ";"
                    if short not in seen:
                        prioritized.append((2 + source_rank_boost, short))
                        seen.add(short)
                continue
            if not _looks_like_decl(stripped):
                continue
            score: Optional[int] = None
            if method_name and re.search(rf"\b{re.escape(method_name)}\s*\(", stripped):
                # Prefer Class::method or method inside Class scope prototypes.
                if class_name and (
                    f"{class_name}::{method_name}" in stripped
                    or (
                        source_rank_boost == 0
                        and re.search(rf"\b{re.escape(method_name)}\s*\(", stripped)
                        and not re.search(rf"\w+::{re.escape(method_name)}\s*\(", stripped)
                    )
                ):
                    score = 0 + source_rank_boost
            elif any(re.search(rf"\b{re.escape(head)}\b", stripped) for head in reloc_heads):
                if class_name and re.search(rf"\b{re.escape(class_name)}\b", stripped):
                    score = 1 + source_rank_boost
                elif source_rank_boost == 0 and not class_name:
                    score = 1
            if score is None:
                continue
            prioritized.append((score, stripped))
            seen.add(stripped)

    # Header first (boost 0), then source definitions (boost +3 so they rank worse).
    _consider(header_text or "", 0)
    _consider(source_text or "", 3)

    prioritized.sort(key=lambda item: (item[0], len(item[1])))
    keep: List[str] = []
    total = 0
    for _, snippet in prioritized:
        if total + len(snippet) + 1 > max_chars:
            break
        keep.append(snippet)
        total += len(snippet) + 1
        if len(keep) >= 12:
            break
    return keep


def select_similar_siblings(
    siblings: List[dict],
    *,
    demangled: str,
    max_bodies: int,
    max_chars_each: int,
) -> List[dict]:
    """Keep small same-class accepted siblings as optional style exemplars."""
    if max_bodies <= 0 or not siblings:
        return []
    class_name = ""
    match = re.search(r"(\w+)::", demangled)
    if match:
        class_name = match.group(1)

    scored: List[tuple[int, dict]] = []
    for row in siblings:
        body = str(row.get("body") or "")
        if not body or len(body) > max_chars_each:
            continue
        fn = str(row.get("function") or "")
        same_class = bool(class_name and fn.startswith(f"{class_name}::"))
        # Prefer same-class, then shorter bodies.
        rank = (0 if same_class else 1, len(body))
        if not same_class:
            # Unrelated siblings are usually noise; skip unless nothing else exists.
            continue
        scored.append((rank[1], {
            "id": row.get("id"),
            "function": row.get("function"),
            "status": row.get("status"),
            "body": body,
        }))

    scored.sort(key=lambda item: item[0])
    return [item[1] for item in scored[:max_bodies]]


# Backwards-compatible alias used by xenoblade_project.
_assembly_function_block = assembly_function_block
