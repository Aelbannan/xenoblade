# Known walls — curated from docs/MWCC_REFERENCE.md

Hand this to a matching agent in a brief instead of the 8,400-line reference.
"Wall" = fixed MWCC/codegen behavior that resists every documented source
lever. No wall is proven unreachable in principle (every retail function is
MWCC output, so a matching C++ reconstruction exists). Cross-check the cited
MWCC_REFERENCE sections before re-litigating a ruled-out shape.

## Wall classes — read these before the list

- **FULL_MATCH-only**: the register-renaming witness cannot certify the
  shortcut, but a byte-identical body (`mismatch: 0`) is still **accepted as
  FULL_MATCH** (no-SMT policy: FULL_MATCH is automatic). Keep iterating
  toward byte-identity; just don't spend calls on `witness`/`certify`. Do NOT
  stop at 94% because "the witness rejects" — that is a lost match.
- **dead-end**: no acceptance path at all (FULL_MATCH impossible + witness
  impossible). Stop early and record the open-item packet.

## Reading the hexdiff triage line
`name: 84.7% | 0 structural | 20 reg_swap | 0x20c/0x20c PASS`
- `structural`: opcode/immediate/branch differs — real mismatch.
- `reg_swap`: only register operands differ; JSON `pure_reg_swap` is the witness-certifiable subset.
- `size`: decomp .text vs retail split budget; `OVER` fails acceptance.

## Policy (do not violate)
- No SMT: acceptance is FULL_MATCH (100% static) or witness-certified EQUIVALENT_MATCH.
  Do not recommend the SMT probe; `accept via --smt out-of-band` notes are stale.
- No asm/register tricks outside the §17.6 PS backend; no `insn_patches`. High-level C/C++ only.

## Fixed-codegen artifacts (closest things to walls)
1. FPR-saving prologue — 0 structural + pure reg-swaps, witness never certifies:
   MWCC always emits stfd+psq_st for f14–f31 saves; witness rejects psq_* (§REF:201).
   → **FULL_MATCH-only**: push to `mismatch: 0`; stop only regalloc/witness attempts.
2. Any function containing a `bl` — witness never applies (0/11 certs contain bl;
   callee effects break terminal-state; §REF:201).
   → **FULL_MATCH-only**: byte-identity still ACCEPTS. Keep pushing to `mismatch: 0` —
   the residual reg-swap/structural diffs are winnable; only `witness`/`certify` are off.
3. `li r0, X@sda21` — assembler accepts only @h/@ha/@l/@loword/@hiword (err 33135; §REF:110).
   → `@l` fallback is byte-identical; catalogue.
4. `addi rD,base,0` peepholed to `mr` — ruled out across versions/shapes (§REF:947).
   → pass two distinct objects (OSRestart) or §17.6 `opword 0x387E0000` (§REF:1267); catalogue.
5. `b .+4` sinit barrier — unfolded vtable-store shape unreachable; 17+ forms, pragmas,
   6 MWCC versions ruled out (§REF:1749). → leave at COMPILES; needs a policy exception.
6. mtctr/bdnz loops under unit-locked `-O4` — shape needs `-O4,s`, not per-function (§REF:856).
   → record the -O4 static cap; negotiate a unit flag split.
7. WGPIPE MMIO stores — GX TEV setters 95–97% + reg rotation; symbolic 0xCC008000 rejected,
   rho non-injective (§REF:1215). → **FULL_MATCH-only**: declaration-order regalloc toward byte-identity.
8. Solver-scale limits — big multi-branch functions with indirect/cross-object callees:
   4096-path limit, missing callee lemmas (§REF:874). → **FULL_MATCH-only**: byte-identity; match callee frontier first.
9. Auto-unroll of rlwimi/mtctr readers — every loop form unrolls; pragmas no-op (§REF:1371).
   → catalogue at the static cap.
10. GQR5 mtspr setup — no source OSSetGQR reproduces orphaned GQR writes (§REF:128).
    → revisit after callee frontier; catalogue.
11. Uncontrollable CSE / rlwimi merge direction — MWCC-internal; 8+ variants ruled out (§REF:889).
    → **FULL_MATCH-only**: record; byte-identity still accepts.
12. `li`-float family — return-constant li floats above an r0-store; ~18 shapes ruled out
    (§REF:1370,2151). → keep best shape; catalogue.
13. Per-function opt-level conflict — one function needs `-O4,s`, unit needs `-O4` (§REF:856,943).
    → unit-level flag negotiation; catalogue.
14. Section padding — linker artifact; never fabricate in source (§REF:11).

## NOT walls
- Pure reg-swap diffs: witness-certifiable → EQUIVALENT_MATCH via `cycle`. Not a wall.
- Reloc name drift: `extern "C" lbl_eu_*` or tools/coop/reloc_map.py. Not a wall.
- SMT inconclusive_* classes: not acceptance inputs under the no-SMT policy; do not chase.
- Plateau ≠ unmatchable: after 3 non-improving attempts record the open-item packet
  (status, pct, size, mismatch categories, ruled-out, next 3 experiments), keep ACTIVE,
  switch angle. BLOCKED only for a concrete external/tooling limitation.
