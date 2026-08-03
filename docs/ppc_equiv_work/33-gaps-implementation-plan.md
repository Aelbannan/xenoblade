# 33 — Implementation plan: certifying the doc-32 "not fixed" RVL gaps (rev 4)

Status: **draft, rev 4 — round-2 adversarial review target**. Rev 1 = plan;
rev 2 = round-1 review (Kimi K3 integration / GLM-5.2 soundness) + owner
verification; rev 3 = re-baseline after doc-32 A1/A2/A3 landed (commit
`78dbecaa3`); rev 4 = adds the **reloc-map freshness precondition** (Item 0.5)
and re-sequences. Companion docs: `31/32-witness-*`, `30-loop-support-*`,
`28-loop-jt-*`, `docs/witness_expansion_plan.md`.

---

## 0. Re-baseline (rev 3, still current)

Doc-32 A1/A2/A3 landed (`78dbecaa3`). Re-sweep of the 28-set against the
current witness:

| Gate | rev-2 | rev-3 (post-landing) | Notes |
|---|---|---|---|
| fields | 4 | **19** | A2 over-rejection (§0.1) |
| reloc | 3 | 5 | 2 were stale-map (`BTM_CancelInquiry` `@2396` vs `@776`; `btm_event_filter_complete` `@2602` vs `@931`) — **fixed by Item 0.5**; AXFX ×2 + lyt dtor are i2f (A5 territory, unchanged) |
| abi-boundary | 10 | 2 | `btu_hcif_process_event` (r4→r5, live-in arg — unsound to rename); `l2c_csm_config` (r3→r4, needs Item 1) |
| loop | 7 | 1 | only `__wudDeleteHandler`; 6 masked by A2 fields |
| rho/deadness | 4 | 1 | `rfc_alloc` (already ACCEPTED) |

### 0.1 A2 over-rejects non-zero RA rotations (BLOCKER for the plan's value)

`_RA_LITERAL_OPCODES` (renaming_witness.py:119) drops the RA field from the
role table for all D/X-form load/stores. Correct for r0-literal cases (either
side RA==0 ⇒ address semantics differ), but over-rejects **pure non-zero RA
rotations** — verified on six sampled `fields` failures: `lha r0,0(r3)` vs
`lha r0,0(r5)`, `lwz r0,4(r23)` vs `lwz r0,4(r22)`, `addi r4,r30,616` vs
`addi r4,r29,616`, `lbz r0,281(r29)` vs `lbz r0,281(r30)` — all sound
rotations now failing gate 3.

**Fix (completeness; soundness already holds):** RA becomes a *conditional*
register field, per slot: RA participates in rho unless
`retail_RA == 0 or decomp_RA == 0` (then bit-equal required). Requires the
gate-3 masks to be per-slot (`_gpr_fpr_masks` is per-opcode today) and
`_register_fields` to consult the decoded RA value. Soundness: both RAs
non-zero ⇒ both sides read their RA registers; shared-state binding makes
`retail.rA` ≡ `decomp.r_rho(A)` ⇒ equal EA ASTs. Either RA zero ⇒ address
semantics differ unless bit-equal ⇒ reject. The `lwzx r3,r0,r5` vs
`lwzx r3,r6,r5` case stays rejected (retail reads no r0; decomp reads r6 —
doc-32 A2's accept test for it was wrong; the implementation's rejection is
sound). Belt: per-slot masks must compose with A1's PSQ exemption (PSQ rA is
already special — "drives the liveness RA-use guard only") and with the region
path's per-region rho.

---

## 0.5 (Item 0.5) Reloc-map freshness as a witness precondition

### Why

The canonical-symbols hook (`_canonical_symbols_for_unit`,
equivalence_check.py:39) reads `tools/coop/retail_reloc_map.json`, keyed by
decomp TU-local label (`@N`) → retail name. **The map goes stale whenever any
retail/decomp `.o` pair is rebuilt** (TU-local `@N` numbering shifts), which
happens constantly on this shared branch (the A1/A2/A3 rebuilds alone
invalidated `btm_inq`'s entries: `@2396` vs `@776`, `@2602` vs `@931`). A
stale map fails gate 2 with *new* `@N` names the map lacks — silent, wasted
cycles. Re-mining is cheap: **1.7 s measured** (no build, no lock, 879 units).

Staleness never causes a **false certificate** (a stale map only *over-rejects*
gate 2 — the witness never certifies with an unmapped label); it only wastes
time. So the freshness mechanism is a **cost/robustness** change, not a
soundness one — reviewers should confirm this claim.

### Design

1. **`reloc_map.py ensure-fresh [--force]`** subcommand: fresh iff the map
   exists AND `generated` exists AND **no objdiff unit's retail `.o`
   (`target_path`) or decomp `.o` (`base_path`) mtime is newer than
   `generated`**. Stale ⇒ run `mine()`. Print `fresh` / `re-mined`. Cost:
   unit scan ~0.5 s; re-mine ~1.7 s worst case.
2. **Wiring — sweep entry points:** `run.py cycle`, `run.py diff`,
   `batch-cycle.py` run `ensure-fresh` once at start of each invocation.
3. **Wiring — witness path (`prove_unit_symbol`):** a module-level
   memoized freshness check before the first witness use of
   `canonical_symbols` per process (per-process memo avoids paying the ~0.5 s
   scan on every `cycle`). Plus a **retry-once belt**: if the witness outcome
   fails at `gate == "reloc"`, re-mine + re-decode + retry once. The belt
   covers mid-sweep rebuilds by concurrent agents that a start-of-sweep memo
   missed; it costs ≤1 re-mine (1.7 s) + one re-decode per affected function
   and is bounded to a single retry.
4. **`hexdiff.py`:** best-effort — its reloc-drift suggestions are advisory;
   document "run `ensure-fresh` before a hexdiff sweep" rather than paying the
   scan in the rapid-iteration loop.
5. **Tests:** (a) touch a `.o` → `ensure-fresh` re-mines; (b) no change →
   no-op (no rebuild side effects); (c) inject a stale map → witness fails
   gate 2 → retry-once re-mines → certifies (or reaches the real next gate);
   (d) retry never loops (bounded once).

Reviewer questions for this item:
- Is the per-process memo + retry-once belt the right split, or should the
  retry be unconditional (drop the mtime pre-check entirely — 1.7 s is
  already negligible)?
- Where exactly should the memo live so `batch-cycle`, the harness, and
  `recertify` all share it without re-paying the scan?
- Does re-mine + re-decode on gate-2 failure interact safely with the
  `_try_renaming_witness` control flow (the witness returns `None` on failure
  → SMT; the retry must re-enter before SMT, not after)?

---

## 1. Items, re-sequenced

### Item 0 — A2 per-slot RA refinement (§0.1)
Tests: accept `lwz r0,4(r23)` vs `lwz r0,4(r22)` under rho; reject
`lwz r0,0(r0)` vs `lwz r0,0(r4)`; regression: landed A2 r0-literal tests keep
passing. Re-sweep after Item 0.5 — expect most of the 19 `fields` failures to
move back to rho/abi-boundary/loop.

### Item 0.5 — freshness precondition (§0.5)
Lands before any sweep re-runs; self-contained; no witness-code interaction
beyond the retry-once hook in `prove_unit_symbol`.

### Item 1 — r3 conditional (1a) + gate-5 trigger (1b)
After Items 0/0.5 the abi-boundary set is `btu_hcif_process_event` (r4→r5,
live-in — genuinely unsound; SMT-only) and `l2c_csm_config` (r3→r4). 1a's
original void targets (`__OSPlayTimeFadeLastAIDCallback`,
`__HBMSYNSetupVolumeEnvelope`) write r3 as dead scratch before returns, so
A3's `_written_before_return` rule keeps r3 fixed; fixing them needs
exit-live-out observability (r3 writes that never reach a caller-observable
exit are renamable for void) — **gate that on a soundness round, or record
the two as SMT candidates**. 1a (r3 into A3's conditional loop) is still
needed for `l2c_csm_config` via 1b. 1b keeps its design (region trigger on
`gate=="abi-boundary"`); its only realistic target is `l2c_csm_config`
(`loop=True` ⇒ also needs Item 2).

### Item 2 — loop second-cut (unchanged design from rev 2)
Reach: 1 now (`__wudDeleteHandler`), 7 after Item 0. Split
`_has_loop_or_non_return_indirect`; drop the direct-backward-branch reject in
`run_region_sliced_witness`; keep the `bcctr`/`blrl` reject (executor records
an `"indirect-branch"` terminal with a symbolic CTR — sound to compare, reject
is scope choice); loop-boundary guard (reject when a boundary falls inside a
backward-branch span — do not attempt "move to header"); rewrite
`test_loop_containing_target_rejected_first_cut`.

### Item 3 — SIBios `.bss` mechanism (no witness interaction)
`SITransfer` slot 9 is a genuine immediate diff (`addi r0,r31,0` vs
`addi r0,r31,64` — RA r31 both sides; Item 0 does not touch it). Mechanism
investigation (declaration/attribute/`-ipa` experiment matrix against the 8
FULL_MATCH siblings); the 2-function `.text` reorder hypothesis was refuted.
Runs concurrently with Items 0–2.

### Item 4 — `BTM_CancelInquiry` (after Item 0.5)
Item 0.5 already restores its gate-2 pass (map re-mined). Expect the
boundary-56 deadness failure (verified at rev 2 with certified contracts).
Phase A characterise → Phase B bounded source (3 attempts; callees certified)
→ Phase C value-splitting redesign (rev-2's `z3.eq` assertion over-rejects;
needs substitutive rebinding or solver comparison; gated on a second soundness
round).

---

## 2. Sequencing, scope, acceptance

1. **Item 0.5** (freshness) — prerequisite for trustworthy sweeps; 30 min.
2. **Item 0** (A2 RA refinement) — restores the rotation class (~16 of 28);
   highest value.
3. **Item 2** (loop) — reach 7 after Item 0.
4. **Item 1** (r3/1b) — reach 1 (`l2c_csm_config`).
5. **Item 3** (SIBios) — concurrent, independent.
6. **Item 4** (BTM) — after 0.5; Phase A/B now, C gated.

CI (full AGENTS.md gate): `gen_fixture_blob.py --check`,
`tools/ppc_equivalence/tests`, `tools.ppc_equivalence differential`,
`docs_sync --check`, `smell_report --check`, `tools/coop/tests`.

In scope: `renaming_witness.py`, `equivalence_check.py`, `reloc_map.py`,
`run.py` (ensure-fresh wiring), `tools/coop/tests/test_renaming_witness.py`,
`SOUNDNESS.md` via `docs_sync`, `SIBios.c`, this doc. **Out of scope:** i2f
magic pools (doc-32 A5), the 3 hard-fields cases, `bcctr`-dispatch modeling,
reloc-pipeline files. Shared-branch rules: no git destructive ops;
`hexdiff.py` for builds; claims before edits; no `--smt` unless authorised.

## Files to read

- `renaming_witness.py` — `_RA_LITERAL_OPCODES` (:119), `_register_fields`
  (:148), `_gpr_fpr_masks` (:203), `_check_abi_fixedness` (:1090),
  `_TRUSTED_NON_64BIT_RETURNS` (:369), region path (:1579+).
- `equivalence_check.py` — `_canonical_symbols_for_unit` (:39),
  `_try_renaming_witness` (:1422), `prove_unit_symbol` (:2759).
- `reloc_map.py` — `mine` (:350), `generated` (:438), CLI parsers (:695+).
- `tools/coop/lib/targets.py` — `equivalence_certificate_error` (:72–260).
- `tools/ppc_equivalence/semantics.py` — loop overflow (:5347), bcctr
  terminal (:5640).
