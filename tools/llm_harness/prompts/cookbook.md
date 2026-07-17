# Stable compiler cookbook

## PowerPC EABI essentials
- r1 = stack pointer (grows down). Frame setup: `stwu r1,-N(r1)`.
- r2 = small data area 2 (SDA2) base. Accessed via `R_PPC_EMB_SDA21`.
- r3-r10 = parameter passing. r3 also holds return value.
- r13 = small data area (SDA) base.
- LR (link register) holds return address. Saved/restored via mfspr/mtspr with spr=8.
- CR (condition register) has 8 4-bit fields. CR0 is updated by most integer ops.
- CTR (count register) used for loop control and indirect branches.
- EABI mandates 16-byte stack alignment at call sites.

## MWCC source-shaping guidance
- MWCC -O4,s inlines aggressively. Small helpers may not appear as call sites.
- MWCC may merge adjacent loads/stores or split a struct store into field stores.
- MWCC emits `rlwinm` for mask operations, `slwi`/`srwi` for shifts by constant, `sraw`/`srawi` for arithmetic shifts.
- MWCC hoists common subexpressions; consider named temporaries.
- MWCC may invert branch conditions (e.g. `beq` vs `bne`) without changing semantics.
- MWCC sometimes prefers `addi rA,rB,0` as a register move instead of `mr`/`or`.

## Common compiler lowering patterns
- `x > 0` → `cmpwi rX, 0; bgt` (signed) or `cmplwi rX, 0; bne` (unsigned).
- `x == 0` → `cmpwi rX, 0; beq` or `cntlzw rX, rX; srwi rX, rX, 5` for count-leading-zeros pattern.
- `x != 0` → `cmpwi rX, 0; bne`.
- `*dst = *src` (struct copy) → `lmw`/`stmw` for large structs, or paired `lwz`/`stw`.
- Boolean: `x ? 1 : 0` → `cntlzw` + `srwi`. `x ? 0 : 1` → `xori rX, rX, 1`.
- Ternary `a ? b : c` → `cmp; bc; mr/isel` (MWCC may emit `isel` on supported targets).
- Switch: MWCC uses binary search (comparison tree) or jump table (via `slwi` + `lwzx` + `mtctr` + `bctr`).
- Loop: `for` → `li` init, `cmpwi; bgt` exit, `addi` increment, `b` top.
- Null check: `cmplwi rX, 0; beq` or `cmplwi cr1, rX, 0; bc cr1, eq`.

## Project patch constraints
- Maximum function size: retail size + 0 bytes (must not exceed retail).
- No global additions, no header modifications in single-function patches.
- Use existing types from the repository; prefer `s32`/`u32` over raw `int`.
- Match the coding style of accepted siblings in the same translation unit.
- Function must compile with MWCC -O4,s via the project's build system.
- relocations must match retail exactly (type, symbol, addend).

## Output schemas
- Reconstruction: `{ "source": "...", "semantic_summary": {...}, "hypothesis": "...", "confidence": 0.0-1.0 }`.
- Compile repair: `{ "stage": "compile_repair", "repair_index": N, "source": "...", "blocked_on": "..." }`.
- Match improve: `{ "stage": "match_improve", "source": "...", "bounded_change": "...", "evidence": [...] }`.
- Semantic repair: `{ "stage": "semantic_repair", "primary_discrepancy": "...", "source": "...", "expected_effect": {...} }`.

## Promotion and proof terminology
- `FULL_MATCH`: bit-exact binary match (100% static match).
- `EQUIVALENT_MATCH`: PPC equivalence checker proved behavioral equivalence.
- `CODE_MATCH`: strong fuzzy match, functionally equivalent.
- `Structural score`: weighted comparison of calls, memory, CFG, constants, returns.
- `Proof status`: proved | disproved | inconclusive (timeout/unsupported/external/missing cert/unknown).
