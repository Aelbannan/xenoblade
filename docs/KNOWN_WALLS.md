# Known walls — curated from docs/MWCC_CASES.md

Hand this to a matching agent in a brief instead of the 8,400-line reference.
"Wall" = fixed MWCC/codegen behavior that resists every documented source
lever. No wall is proven unreachable in principle (every retail function is
MWCC output, so a matching C++ reconstruction exists). Cross-check the cited
MWCC_CASES sections before re-litigating a ruled-out shape.

- **When to read this:** a residual resists the routed levers and you suspect a fixed-codegen wall;
  or you're writing a matching-agent brief.
- **Skip it when:** the residual is still routable (`reg_swap`/`structural`/reloc) — try `SKILL.md`
  Routing first.

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
   MWCC always emits stfd+psq_st for f14–f31 saves; witness rejects psq_* .
   → **FULL_MATCH-only**: push to `mismatch: 0`; stop only regalloc/witness attempts.
2. Any function containing a `bl` — witness never applies (0/11 certs contain bl;
   callee effects break terminal-state; .
   → **FULL_MATCH-only**: byte-identity still ACCEPTS. Keep pushing to `mismatch: 0` —
   the residual reg-swap/structural diffs are winnable; only `witness`/`certify` are off.
3. `li r0, X@sda21` — assembler accepts only @h/@ha/@l/@loword/@hiword (err 33135; .
   → `@l` fallback is byte-identical; catalogue.
4. `addi rD,base,0` peepholed to `mr` — ruled out across versions/shapes .
   → pass two distinct objects (OSRestart) or §17.6 `opword 0x387E0000` ; catalogue.
5. ~~`b .+4` sinit barrier~~ — **STALE / RESOLVED**: a merged-symbol tail call, not codegen;
   splitting the symbol in symbols.txt gives FULL_MATCH ×5 (MWCC_CASES §4; docs/evidence/per_unit_flags_2026-10-23.md §Walls).
   → split the merged symbol in symbols.txt; not a wall.
6. ~~mtctr/bdnz loops under unit-locked `-O4`~~ — **DISSOLVED**: hcicmds (GC/3.0a5.2 `-O4`) is
   57/59, `write_cur_iac_lap` FULL via the nested-loop source form; flag/version sweeps don't move the
   remaining header-store hoist residuals (docs/evidence/per_unit_flags_2026-10-23.md §Walls). → fix source shape.
7. WGPIPE MMIO stores — GX TEV setters 95–97% + reg rotation; symbolic 0xCC008000 rejected,
   rho non-injective . → **FULL_MATCH-only**: declaration-order regalloc toward byte-identity.
8. Solver-scale limits — big multi-branch functions with indirect/cross-object callees:
   4096-path limit, missing callee lemmas . → **FULL_MATCH-only**: byte-identity; match callee frontier first.
9. Auto-unroll of rlwimi/mtctr readers — every loop form unrolls; pragmas no-op .
   → catalogue at the static cap.
10. GQR5 mtspr setup — no source OSSetGQR reproduces orphaned GQR writes .
    → revisit after callee frontier; catalogue.
11. Uncontrollable CSE / rlwimi merge direction — MWCC-internal; 8+ variants ruled out .
    → **FULL_MATCH-only**: record; byte-identity still accepts.
12. `li`-float family — return-constant li floats above an r0-store; ~18 shapes ruled out
    ,2151). → keep best shape; catalogue.
13. ~~Per-function opt-level conflict~~ — **DISSOLVED**: `#pragma optimize_for_size on` (+ `dont_inline on`)
    reproduces -O4,s per function inside a -O4,p unit; where -O4,s dominates a unit, flip the unit
    flag instead (applied 2026-10-23 to CEquipItemBox, CItemBoxLine, CModelDispMakeCrystal, CTaskGame,
    CMCCrystalBox, CItemBoxGrid, CMCGetItemBox, CKizunaTalkList, CWorkRoot, CEquipChange, CCollepedia).
    Mixed TUs (CSaveLoad, CTitle, CPartyStateWin, CArtsInfo, CTagProcessor, CPassiveSkill, CMenuArtsSet,
    CQstLogList) stay -O4,p + pragmas (docs/evidence/per_unit_flags_2026-10-23.md).
14. Section padding — linker artifact; never fabricate in source .

## NOT walls
- Pure reg-swap diffs: witness-certifiable → EQUIVALENT_MATCH via `cycle`. Not a wall.
- Reloc name drift: `extern "C" lbl_eu_*` or tools/coop/reloc_map.py. Not a wall.
- SMT inconclusive_* classes: not acceptance inputs under the no-SMT policy; do not chase.
- Plateau ≠ unmatchable: after 3 non-improving attempts record the open-item packet
  (status, pct, size, mismatch categories, ruled-out, next 3 experiments), keep ACTIVE,
  switch angle. BLOCKED only for a concrete external/tooling limitation.
