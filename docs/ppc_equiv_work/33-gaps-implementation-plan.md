# 33 — Implementation plan: certifying the doc-32 "not fixed" RVL gaps (rev 5)

Status: **draft, rev 5 — round-2 adversarial review incorporated** (Kimi K3
integration + GLM-5.2 soundness, both completed 2026-08-04; contested claims
owner-verified). Rev history: 1 = plan; 2 = round-1 review; 3 = re-baseline
after doc-32 A1/A2/A3 landed; 4 = freshness precondition; 5 = round-2
synthesis.

---

## 0. Round-2 verdict summary

| Item | Verdict | Key changes vs rev 4 |
|---|---|---|
| 0 (A2 per-slot RA) | **GO** (sound) | dual per-slot change (gate-3 masks AND gate-4 rho + region path) via one shared helper; PSQ exclusion; DriftDetector test update |
| 0.5 (freshness) | **REWORK — soundness-adjacent** | F1: "staleness is cost-only" was false; bind map snapshot into `source_hash`; per-invocation freshness in `prove_unit_symbol`; belt inside `_try_renaming_witness` |
| 1 (r3/1b) | **DROP** | 0 reachable targets; exit-live-out for r3 is unsound — leave r3 fixed, SMT the four targets |
| 2 (loop) | **GO** (sound) | in-function backward-branch span guard; self-tail-recursion included |
| 3 (SIBios) | no interaction | unchanged, independent |
| 4 (BTM) | needs Item 0 + 0.5 | slot 56 currently fails `fields` (Item 0 restores boundary-deadness) |

---

## 0.1 A2 over-rejects non-zero RA rotations (Item 0)

Unchanged from rev 4 — `_RA_LITERAL_OPCODES` (renaming_witness.py:119) drops
the RA field for all D/X-form load/stores; verified: `lwz r0,4(r23)` vs
`lwz r0,4(r22)`, `lbz r0,281(r29)` vs `lbz r0,281(r30)`, `lha r0,0(r3)` vs
`lha r0,0(r5)` are pure rotations failing gate 3.

**Fix (completeness; soundness verified by round-2 GLM):** per-slot conditional
RA field — RA participates in rho unless `retail_RA == 0 or decomp_RA == 0`
(then bit-equal). GLM verified all three EA cases against
`semantics.py` (`_dform_address` :1854, X-form :3531, PSQ :3874) and the
r0-as-value/literal coexistence (engine's per-instruction `ra==0` guard).

**Implementation requirements (round-2, both reviewers):**
1. **Dual change, one helper.** `_gpr_fpr_masks` (gate-3 mask, :203) AND
   `_register_fields` (gate-4 rho + `_rho_region_boundaries`/`_region_rho`)
   must both become slot-aware (`_slot_register_fields(r_insn, d_insn)`).
   Changing only one is an unsound split-brain: a field exempt from bit-
   equality MUST enter rho, else the executor sees divergent EA ASTs.
   `operand_roles` (:264, audit-only) and the region path inherit it.
   Consumers (verified list): gate 3 mask (:951), gate 4 rho (:981), region
   boundaries (:1628/:1640), region rho (:1661), `_use_def_numbered` guard
   (:582 — already per-slot, no change), `_psq_operands_rho_fixed` (:1150 —
   unchanged), `_symbolic_initial_pair` (perm-driven — no change).
2. **PSQ exclusion:** keep PSQ forms out of the refinement (liveness-guard-only
   as today); no soundness hole either way (raw==raw pre-check).
3. **DriftDetector test update:** `test_renaming_witness.py:838-849` asserts
   opcode-keyed `_gpr_fpr_masks(op)` drops RA — must be re-expressed for the
   per-slot rule.
4. **Test matrix (GLM):** accept `lwz r0,4(r23)` vs `lwz r0,4(r22)`
   (rho{23→22}); accept `lwzx r3,r23,r5` vs `lwzx r3,r22,r5`; reject
   `lwz r3,0(r0)` vs `lwz r3,0(r4)`; reject `lwzx r3,r0,r5` vs
   `lwzx r3,r6,r5` (regression — stays rejected, doc-32 A2's accept test was
   wrong); accept a 2-slot pair where r0 is literal in one slot and value in
   the other; **mask/rho-agreement test** — for a non-zero-RA rotation, RA
   must appear in the gate-3 mask AND the slot's rho (catches split-brain).

---

## 0.5 Reloc-map freshness — REWORKED (soundness-adjacent, not cost-only)

### F1 (GLM, round-2): the "staleness is cost-only" claim was wrong

Owner-verified scope: the mined map is **data-only** (`R_PPC_ADDR16_HA/LO` +
`R_PPC_EMB_SDA21`; zero REL24/REL14) and `_canonical_symbols_for_unit` can
only rewrite data relocs — the reviewer's callee-contract variant
(decomp `@N` aliasing a certified callee `foo`) is **not exploitable** (calls
are never canonicalized). The **data-address variant is real**: a stale map
rewrites a decomp `@N` data label to a retail name whose *linked data differs*
from the current build's `@N` — the witness certifies with symbolic addresses
that don't match the linked binary. Confirmed: `retail_reloc_map.json` is
**not** in `provenance.py` (untracked proof input). Mitigations below make
staleness fail-closed.

### Design (rev 5)

1. **`reloc_map.py ensure-fresh [--force]`** — fresh iff map exists AND
   `generated` exists AND no objdiff retail/decomp `.o` mtime is newer than
   `generated`; stale ⇒ `mine()`. Cost (owner-measured): scan 0.04 s,
   `mine` 1.6 s user / **~5 s wall** (rev-4's "1.7 s" was user-time —
   corrected; belt cost is per-affected-function).
2. **Per-unit trigger, lazy, post-build (Kimi R2, MAJOR).** Entry-point wiring
   in `cycle`/`diff` runs **before** the build step that stales the map (the
   edit-then-cycle case) and misses `recertify`. Instead: a lazy per-unit
   memo in `prove_unit_symbol` right where `canonical_symbols` is computed
   (:2833) — after the build, keyed per unit (this unit's two `.o` mtimes vs
   `generated`). The global rule is nearly always stale (measured
   1619/2076 units newer — a global trigger re-mines every cycle). Per-unit
   keys depend only on that unit's pair; a per-unit trigger with repo-wide
   `mine()` is sound (the belt covers the rare global-entry drift). All
   consumers (cycle, diff, batch-cycle in-process, harness subprocess,
   recertify) funnel through `prove_unit_symbol`.
3. **Belt: retry-once inside `_try_renaming_witness` (Kimi R2, MAJOR).** The
   gate must be surfaced — `_try_renaming_witness` currently returns bare
   `None`, losing the `gate=="reloc"` signal. Add a channel (diag out-param
   or tuple). On gate-2 `reloc` failure: re-mine + re-decode + retry once,
   and let the refreshed `canonical_symbols` also feed the SMT fallback
   (:2895). Re-derive `call_targets`/contracts after re-decode. **Reason-gate
   the belt**: skip when the failure is `reloc presence differs` (the i2f
   cases — re-mining cannot help; otherwise they pay ~5 s per process per
   function forever). Bounded once; no loop risk.
4. **Soundness: bind the map into the proof (GLM R2).** Record
   `reloc_map_sha256` (of the map file the canonical symbols were drawn from)
   in the witness `ProofResult`/`witness_payload` and include the map file
   path in `provenance.py` source paths (or bind the snapshot into
   `proof_request_hash`). Staleness then either (a) fails gate 2 (freshness
   enforced per invocation) or (b) is auditable — no silent proof-input
   change. Freshness check per-witness-invocation (memo only within one
   function's prove), so mid-sweep concurrent rebuilds cannot reuse a stale
   canonicalization.
5. **`generated` granularity:** stamp at `mine` START (or epoch float); a
   second-resolution stamp at end misses rebuilds during the mine window.
6. **`certify_unit_symbol`** witness call (:2981) passes no
   `canonical_symbols` (pre-existing) — the memo/belt never applies there;
   extend or scope-limit (note it).
7. **Dirty-tree policy:** `retail_reloc_map.json` is a committed shared
   artifact; `ensure-fresh` re-mines dirty the working tree during
   cycle/diff — document a commit policy (commit regenerated map separately,
   like the reloc-fix agent does).
8. **Test matrix (GLM):** (a) touch `.o` ⇒ re-mine; no change ⇒ no-op; (b)
   inject stale map ⇒ gate-2 `reloc` ⇒ retry re-mines ⇒ reaches the real next
   gate; (c) i2f presence-differs with fresh map ⇒ retry count == 1 (bounded);
   (d) **F1 regression:** inject a stale map aliasing a decomp `@N` to a
   retail name with different linked data ⇒ witness must NOT certify (passes
   only after the map is bound into `source_hash` / freshness is per-
   invocation); (e) two map contents that both pass gate 2 ⇒ payload records
   differing `reloc_map_sha256`.

---

## 1. Items — final list

- **Item 0 (GO):** A2 per-slot RA refinement (§0.1). Re-sweep after 0.5 —
  expect most of the 19 `fields` failures to move back to rho/abi/loop.
- **Item 0.5 (REWORKED, GO):** freshness (§0.5).
- **Item 1 (DROPPED):** r3 conditional + gate-5 trigger — **zero reachable
  targets** (Kimi R2: `l2c_csm_config` is a 3-cycle over live-in EABI args
  from `mr` scheduling — not a rename; `btu_hcif_process_event` r4 is
  written-before-read scratch, blocked by A3 default-FIXED +
  `_written_before_return` + 17 missing callee contracts). GLM R2: exit-live-
  out for r3 is **unsound** (fixpoint live-out at returns is empty; `bclr`-
  tail-calls to `*`-contract callees reading r3 are invisible to
  `_tail_call_reads_lane`; `_TRUSTED_NON_64BIT_RETURNS` is not per-register
  correct — i32/u32/bool/ptr return in r3, so r3's trust set would be
  `{void,f32,f64}` only). **Leave r3 unconditionally fixed; record
  `__OSPlayTimeFadeLastAIDCallback`, `__HBMSYNSetupVolumeEnvelope`,
  `btu_hcif_process_event`, `l2c_csm_config` as SMT candidates.**
- **Item 2 (GO):** loop second-cut — split `_has_loop_or_non_return_indirect`;
  drop the direct-backward-branch reject in `run_region_sliced_witness`; keep
  `bcctr`/`blrl` reject (executor records an `"indirect-branch"` terminal
  :5549 — comparison would self-agree under shared CTR, so the reject is a
  conservative scope choice, verified sound). **Loop-boundary guard
  (GLM NIT, adopted):** reject only when a boundary falls inside a
  backward-branch span with an **in-function target** (`by_index.get(target)`)
  — a backward `b` to an out-of-function address is a terminal, not a loop
  (`_cfg_successors` :664); include relocated non-link self-recursion
  (`canonical_symbol == local_symbol`, :742). Rewrite
  `test_loop_containing_target_rejected_first_cut`.
- **Item 3 (independent):** SIBios `.bss` mechanism investigation
  (`SITransfer` slot 9 is a genuine immediate diff — Item 0 does not touch it;
  GLM verified gate-3 catches the immediate regardless).
- **Item 4 (after Items 0 + 0.5):** `BTM_CancelInquiry` — after 0.5 its gate 2
  passes; after Item 0, slot 56 (currently a non-zero-RA `sth` rotation
  failing `fields`) becomes the boundary-deadness failure again; Phase A
  characterise → Phase B bounded source (callees certified) → Phase C
  value-splitting redesign (gated on a future soundness round; rev-2's
  `z3.eq` assertion over-rejects).

## 2. Sequencing, scope, acceptance

1. **Item 0.5** (freshness + F1 mitigations) — prerequisite for trustworthy
   sweeps.
2. **Item 0** (A2 refinement) — restores the rotation class.
3. **Item 2** (loop) — reach 7 after Item 0.
4. **Item 3** (SIBios) — concurrent, independent.
5. **Item 4** — after 0 + 0.5.
6. Record the four SMT candidates (Item 1 targets) in targets.json.

CI (full AGENTS.md gate): `gen_fixture_blob.py --check`,
`tools/ppc_equivalence/tests`, `tools.ppc_equivalence differential`,
`docs_sync --check`, `smell_report --check`, `tools/coop/tests`.

In scope: `renaming_witness.py`, `equivalence_check.py`, `reloc_map.py`,
`run.py` (ensure-fresh wiring), `provenance.py` (map binding),
`tools/coop/tests/test_renaming_witness.py`, `SOUNDNESS.md` via `docs_sync`,
`SIBios.c`, this doc. **Out of scope:** i2f magic pools (doc-32 A5),
`bcctr`-dispatch modeling, the 3 hard-fields cases, reloc-pipeline files.
Shared-branch rules: no git destructive ops; `hexdiff.py` for builds; claims
before edits; no `--smt` unless authorised; commit regenerated map
separately.

## Files to read

- `renaming_witness.py` — `_RA_LITERAL_OPCODES` (:119), `_register_fields`
  (:148), `_gpr_fpr_masks` (:203), gates 3/4 (:951/:981), region path
  (:1579+), `_has_loop_or_non_return_indirect` (:645).
- `equivalence_check.py` — `_canonical_symbols_for_unit` (:39),
  `_try_renaming_witness` (:1422), `prove_unit_symbol` (:2759,
  canonical_symbols at :2833), `certify_unit_symbol` (~:2942).
- `reloc_map.py` — `mine` (:350), `generated` (:438), CLI (:695+).
- `tools/ppc_equivalence/provenance.py` — source paths for the map binding.
- `tools/ppc_equivalence/semantics.py` — `_dform_address` (:1854), X-form
  (:3531), PSQ (:3874), loop overflow (:5347), bcctr terminal (:5549).
- `tools/coop/tests/test_renaming_witness.py` — DriftDetector (:838-849),
  loop test (:489).
