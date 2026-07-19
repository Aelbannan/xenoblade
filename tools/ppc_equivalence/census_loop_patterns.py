"""Census loop and jump-table patterns in generated retail assembly.

Scans ``build/<region>/asm/**/*.s`` (or a user-supplied tree), decodes each
``.fn`` block, and counts hits for engine-supported shapes:

* **compare-affine** — ``li`` / body / ``addi -1`` / ``cmpwi`` / ``bne``
* **memory-loop** — ``mtctr`` / constant-stride ``stw|stwu|stb|sth`` / ``bdnz``
* **jump-table** — ``cmplwi`` / ``slwi`` / ``lwzx`` / ``mtctr`` / ``bctr``
* **dual-base / SDA jump-table hints** — multiple table bases or ``lwzx`` via
  ``r2``/``r13`` in the jump-table tail window

Recognition reuses ``loop_summary``, ``memory_loop``, and ``jump_table`` so
corpus counts align with what the equivalence engine would attempt (without
claiming retail functions are ``EQUIVALENT``).
"""

from __future__ import annotations

import argparse
import json
import re
import sys
from collections import Counter
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any

from tools.ppc_equivalence.decoder import decode_block
from tools.ppc_equivalence.ir import DecodeError, Instruction, UnsupportedInstruction
from tools.ppc_equivalence.jump_table import JumpTableCandidate, find_jump_table_candidates
from tools.ppc_equivalence.jump_table_auto import table_base_reg
from tools.ppc_equivalence.loop_summary import (
    CtrAffineLoopCandidate,
    find_compare_affine_loop_candidates,
    find_ctr_affine_loop_candidates,
)
from tools.ppc_equivalence.memory_loop import ConstantStrideStoreLoop, find_constant_stride_store_loops

_ASM_FN_RE = re.compile(r'^\.fn\s+"?(?P<symbol>[^",\s]+)"?\s*,')
_ASM_ENDFN_RE = re.compile(r"^\.endfn\b")
_ASM_INSN_RE = re.compile(
    r"^/\*\s*(?P<address>[0-9A-Fa-f]{8})\s+[0-9A-Fa-f]{8}\s+"
    r"(?P<bytes>(?:[0-9A-Fa-f]{2}\s+){3}[0-9A-Fa-f]{2})\s*\*/\s*"
    r"(?P<mnemonic>\S+)(?:\s+(?P<operand>.*))?$"
)

_SDA_BASE_REGS = frozenset({2, 13})
_STORE_MNEMONICS = frozenset({"stw", "stwu", "stb", "sth", "stbu", "sthu"})


@dataclass(frozen=True)
class AsmFunction:
    symbol: str
    path: Path
    unit: str
    words: tuple[int, ...]
    addresses: tuple[int, ...]

    @property
    def base_address(self) -> int:
        return self.addresses[0]


@dataclass(frozen=True)
class PatternHit:
    pattern: str
    unit: str
    path: str
    symbol: str
    address: int
    confidence: str
    engine_feature: str
    detail: dict[str, Any] = field(default_factory=dict)


@dataclass
class CensusAccumulator:
    asm_root: Path
    files_scanned: int = 0
    functions_scanned: int = 0
    functions_decoded: int = 0
    decode_failures: list[dict[str, str]] = field(default_factory=list)
    hits: list[PatternHit] = field(default_factory=list)
    totals: Counter[str] = field(default_factory=Counter)
    totals_by_unit: Counter[str] = field(default_factory=Counter)
    totals_by_confidence: Counter[str] = field(default_factory=Counter)
    text_store_loop_loops: int = 0
    text_compare_latch_loops: int = 0

    def record(self, hit: PatternHit) -> None:
        self.hits.append(hit)
        key = f"{hit.pattern}:{hit.confidence}"
        self.totals[hit.pattern] += 1
        self.totals[key] += 1
        self.totals_by_unit[f"{hit.pattern}:{hit.unit}"] += 1
        self.totals_by_confidence[hit.confidence] += 1

    def top_hits(self, pattern: str, *, limit: int = 12) -> list[PatternHit]:
        exact = [hit for hit in self.hits if hit.pattern == pattern and hit.confidence == "exact-pattern"]
        partial = [hit for hit in self.hits if hit.pattern == pattern and hit.confidence != "exact-pattern"]
        return (exact + partial)[:limit]

    def to_payload(self) -> dict[str, Any]:
        return {
            "asm_root": str(self.asm_root),
            "files_scanned": self.files_scanned,
            "functions_scanned": self.functions_scanned,
            "functions_decoded": self.functions_decoded,
            "decode_failures": self.decode_failures,
            "text_store_loop_loops": self.text_store_loop_loops,
            "text_compare_latch_loops": self.text_compare_latch_loops,
            "totals": dict(sorted(self.totals.items())),
            "totals_by_confidence": dict(sorted(self.totals_by_confidence.items())),
            "totals_by_unit": dict(sorted(self.totals_by_unit.items())),
            "top_hits": {
                pattern: [
                    {
                        "unit": hit.unit,
                        "path": hit.path,
                        "symbol": hit.symbol,
                        "address": f"0x{hit.address:08X}",
                        "confidence": hit.confidence,
                        "engine_feature": hit.engine_feature,
                        "detail": hit.detail,
                    }
                    for hit in self.top_hits(pattern)
                ]
                for pattern in (
                    "compare-affine",
                    "memory-loop",
                    "jump-table",
                    "jump-table-sda",
                    "jump-table-dual-base",
                    "ctr-affine",
                )
            },
        }


def unit_name(path: Path, root: Path) -> str:
    relative = path.relative_to(root)
    return relative.parts[0] if relative.parts else relative.as_posix()


def iter_asm_paths(root: Path) -> list[Path]:
    if root.is_file() and root.suffix == ".s":
        return [root]
    if not root.is_dir():
        return []
    return sorted(root.rglob("*.s"))


def parse_asm_functions(path: Path, *, root: Path) -> list[AsmFunction]:
    unit = unit_name(path, root)
    functions: list[AsmFunction] = []
    current_symbol: str | None = None
    words: list[int] = []
    addresses: list[int] = []

    def flush() -> None:
        nonlocal current_symbol, words, addresses
        if current_symbol and words:
            functions.append(
                AsmFunction(
                    symbol=current_symbol,
                    path=path,
                    unit=unit,
                    words=tuple(words),
                    addresses=tuple(addresses),
                )
            )
        current_symbol = None
        words = []
        addresses = []

    for line in path.read_text(encoding="utf-8", errors="replace").splitlines():
        stripped = line.strip()
        fn_match = _ASM_FN_RE.match(stripped)
        if fn_match:
            flush()
            current_symbol = fn_match.group("symbol")
            continue
        if _ASM_ENDFN_RE.match(stripped):
            flush()
            continue
        insn_match = _ASM_INSN_RE.match(stripped)
        if insn_match and current_symbol is not None:
            words.append(int(insn_match.group("bytes").replace(" ", ""), 16))
            addresses.append(int(insn_match.group("address"), 16))

    flush()
    return functions


def decode_function(function: AsmFunction) -> list[Instruction] | None:
    code = b"".join(word.to_bytes(4, "big") for word in function.words)
    try:
        return decode_block(code, function.base_address, validate_with_capstone=False)
    except (DecodeError, UnsupportedInstruction) as exc:
        return None


def _hit_from_compare(candidate: CtrAffineLoopCandidate, function: AsmFunction) -> PatternHit:
    return PatternHit(
        pattern="compare-affine",
        unit=function.unit,
        path=str(function.path),
        symbol=function.symbol,
        address=candidate.header_pc,
        confidence=candidate.confidence,
        engine_feature="affine-loop-summary / compare-affine-closed-form",
        detail={
            "header_pc": f"0x{candidate.header_pc:08X}",
            "latch_pc": f"0x{candidate.latch_pc:08X}",
            "trip_count": candidate.trip_count,
            "trip_count_reg": candidate.trip_count_reg,
            "notes": list(candidate.notes),
        },
    )


def _hit_from_memory(loop: ConstantStrideStoreLoop, function: AsmFunction) -> PatternHit:
    return PatternHit(
        pattern="memory-loop",
        unit=function.unit,
        path=str(function.path),
        symbol=function.symbol,
        address=loop.header_pc,
        confidence=loop.confidence,
        engine_feature="memory-loop-summary / constant-stride-store",
        detail={
            "header_pc": f"0x{loop.header_pc:08X}",
            "store_kind": loop.store_kind,
            "store_width": loop.store_width,
            "stride": loop.stride,
            "trip_count": loop.trip_count,
            "notes": list(loop.notes),
        },
    )


def _hit_from_jump_table(
    candidate: JumpTableCandidate,
    function: AsmFunction,
    *,
    pattern: str,
    base_reg: int | None,
) -> PatternHit:
    return PatternHit(
        pattern=pattern,
        unit=function.unit,
        path=str(function.path),
        symbol=function.symbol,
        address=candidate.branch_pc,
        confidence=candidate.confidence,
        engine_feature="jump-table-auto / readonly-image (descriptive only)",
        detail={
            "branch_pc": f"0x{candidate.branch_pc:08X}",
            "load_pc": f"0x{candidate.load_pc:08X}" if candidate.load_pc is not None else None,
            "mtctr_pc": f"0x{candidate.mtctr_pc:08X}" if candidate.mtctr_pc is not None else None,
            "bound_imm": candidate.bound_imm,
            "table_base_reg": base_reg,
            "table_base_expr": candidate.table_base_expr,
            "notes": list(candidate.notes),
        },
    )


def scan_function(
    function: AsmFunction,
    instructions: list[Instruction],
    acc: CensusAccumulator,
) -> None:
    for candidate in find_compare_affine_loop_candidates(instructions):
        acc.record(_hit_from_compare(candidate, function))

    for loop in find_constant_stride_store_loops(instructions):
        acc.record(_hit_from_memory(loop, function))

    for candidate in find_ctr_affine_loop_candidates(instructions):
        acc.record(
            PatternHit(
                pattern="ctr-affine",
                unit=function.unit,
                path=str(function.path),
                symbol=function.symbol,
                address=candidate.header_pc,
                confidence=candidate.confidence,
                engine_feature="affine-loop-summary / affine-closed-form",
                detail={
                    "header_pc": f"0x{candidate.header_pc:08X}",
                    "trip_count": candidate.trip_count,
                    "notes": list(candidate.notes),
                },
            )
        )

    jump_hits = find_jump_table_candidates(instructions)
    exact_bases: set[int] = set()
    for candidate in jump_hits:
        base_reg = table_base_reg(candidate, instructions)
        acc.record(_hit_from_jump_table(candidate, function, pattern="jump-table", base_reg=base_reg))
        if candidate.confidence == "exact-pattern" and base_reg is not None:
            exact_bases.add(base_reg)
            if base_reg in _SDA_BASE_REGS:
                acc.record(
                    _hit_from_jump_table(
                        candidate,
                        function,
                        pattern="jump-table-sda",
                        base_reg=base_reg,
                    )
                )

    if len(exact_bases) >= 2:
        for candidate in jump_hits:
            if candidate.confidence != "exact-pattern":
                continue
            base_reg = table_base_reg(candidate, instructions)
            acc.record(
                _hit_from_jump_table(
                    candidate,
                    function,
                    pattern="jump-table-dual-base",
                    base_reg=base_reg,
                )
            )


def scan_text_shapes(path: Path, acc: CensusAccumulator) -> None:
    """Conservative mnemonic-window scans for near-miss documentation."""
    in_function = False
    current_instructions: list[tuple[int, str, str]] = []

    def flush() -> None:
        nonlocal current_instructions
        if current_instructions:
            _scan_text_instruction_window(current_instructions, path, acc)
        current_instructions = []

    for line in path.read_text(encoding="utf-8", errors="replace").splitlines():
        stripped = line.strip()
        if _ASM_FN_RE.match(stripped):
            flush()
            in_function = True
            continue
        if _ASM_ENDFN_RE.match(stripped):
            flush()
            in_function = False
            continue
        if not in_function:
            continue
        match = _ASM_INSN_RE.match(stripped)
        if match:
            current_instructions.append(
                (
                    int(match.group("address"), 16),
                    match.group("mnemonic").lower(),
                    (match.group("operand") or "").strip(),
                )
            )

    flush()


def _scan_text_instruction_window(
    instructions: list[tuple[int, str, str]],
    path: Path,
    acc: CensusAccumulator,
) -> None:
    address_index = {address: index for index, (address, _, _) in enumerate(instructions)}

    for index, (address, mnemonic, operand) in enumerate(instructions):
        if mnemonic == "bne" and _is_compare_affine_latch(instructions, index):
            acc.text_compare_latch_loops += 1

        if mnemonic in ("bctr", "bcctr"):
            window = instructions[max(0, index - 5): index + 1]
            lwzx_ops = [item for item in window if item[1] == "lwzx"]
            if lwzx_ops and "mtctr" in {item[1] for item in window}:
                bases = {
                    item[2].split(",")[1].strip()
                    for item in lwzx_ops
                    if "," in item[2]
                }
                if len(bases) >= 2:
                    acc.record(
                        PatternHit(
                            pattern="jump-table-dual-base",
                            unit=unit_name(path, acc.asm_root),
                            path=str(path),
                            symbol="(text-window)",
                            address=address,
                            confidence="text-heuristic",
                            engine_feature="jump-table-auto / dual-base pairing",
                            detail={"lwzx_bases": sorted(bases)},
                        )
                    )

        if mnemonic != "bdnz":
            continue
        target = operand.split()[0]
        if not target.startswith(".L_"):
            continue
        header = int(target[3:], 16)
        header_index = address_index.get(header)
        if header_index is None or header_index >= index:
            continue
        if header_index < 1:
            continue
        prev_mnemonic = instructions[header_index - 1][1]
        if prev_mnemonic != "mtctr":
            continue
        body = instructions[header_index:index]
        if any(item[1] in _STORE_MNEMONICS for item in body):
            acc.text_store_loop_loops += 1


def _is_compare_affine_latch(
    instructions: list[tuple[int, str, str]],
    bne_index: int,
) -> bool:
    if bne_index < 2:
        return False
    _, cmp_mnemonic, cmp_operand = instructions[bne_index - 1]
    _, dec_mnemonic, dec_operand = instructions[bne_index - 2]
    if cmp_mnemonic != "cmpwi" or dec_mnemonic != "addi":
        return False
    if not (cmp_operand.endswith(", 0x0") or cmp_operand.endswith(", 0")):
        return False
    parts = [part.strip() for part in dec_operand.split(",")]
    if len(parts) < 3:
        return False
    if not parts[0].startswith("r") or parts[0] != parts[1] or parts[0] == "r0":
        return False
    return parts[2] in {"-0x1", "-1", "0xffffffff", "0xFFFFFFFF"}


def run_census(root: Path) -> CensusAccumulator:
    acc = CensusAccumulator(asm_root=root.resolve())
    paths = iter_asm_paths(root)
    acc.files_scanned = len(paths)

    for path in paths:
        scan_text_shapes(path, acc)
        for function in parse_asm_functions(path, root=root):
            acc.functions_scanned += 1
            instructions = decode_function(function)
            if instructions is None:
                acc.decode_failures.append(
                    {
                        "path": str(path),
                        "symbol": function.symbol,
                        "address": f"0x{function.base_address:08X}",
                    }
                )
                continue
            acc.functions_decoded += 1
            scan_function(function, instructions, acc)

    return acc


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "root",
        nargs="?",
        type=Path,
        default=Path("build/us/asm"),
        help="Assembly file or directory tree (default: build/us/asm)",
    )
    parser.add_argument("--json", type=Path, help="Write machine-readable census JSON here")
    args = parser.parse_args(argv)

    root = args.root
    if not root.exists():
        print(
            f"skip: assembly tree not found: {root}\n"
            "Run configure/baseline in a checkout with build/us/asm, then pass that path.",
            file=sys.stderr,
        )
        return 2

    acc = run_census(root)
    payload = acc.to_payload()

    if args.json:
        args.json.parent.mkdir(parents=True, exist_ok=True)
        args.json.write_text(json.dumps(payload, indent=2, sort_keys=True) + "\n", encoding="utf-8")

    print(
        f"scanned {payload['files_scanned']} asm files; "
        f"{payload['functions_decoded']}/{payload['functions_scanned']} functions decoded; "
        f"{len(payload['decode_failures'])} decode failures"
    )
    for pattern in (
        "compare-affine",
        "memory-loop",
        "jump-table",
        "jump-table-sda",
        "jump-table-dual-base",
        "ctr-affine",
    ):
        total = payload["totals"].get(pattern, 0)
        exact = payload["totals"].get(f"{pattern}:exact-pattern", 0)
        print(f"  {pattern}: {total} hits ({exact} exact-pattern)")
    print(
        "  text heuristics: "
        f"{payload['text_store_loop_loops']} mtctr/store/bdnz windows, "
        f"{payload['text_compare_latch_loops']} compare-affine latch shapes"
    )
    if args.json:
        print(f"wrote {args.json}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
