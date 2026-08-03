# 33 — Implementation plan: certifying the doc-32 "not fixed" RVL gaps (rev 3)

Status: **draft, rev 3 — compatibility re-baseline after doc-32 A1/A2/A3 landed**
(commit `78dbecaa3`, 2026-08-04). Rev 1 = plan; rev 2 = round-1 adversarial
review (Kimi K3 integration / GLM-5.2 soundness) with owner verification of
contested claims; rev 3 = re-sweep of the 28-set against the landed witness.

---

## 0. What changed: the landed A1/A2/A3 re-baseline

`78dbecaa3` implemented doc-32 A1 (byte-identical PSQ exemption), A2
(position-aware r0), A3 (conditional r4/f1 + `declared_return` threading).
Re-sweeping the 28-set against the current witness changes the picture
materially:

| Gate | rev-2 (pre-landing) | rev-3 (post-landing) | Notes |
|---|---|---|---|
| fields | 4 | **19** | [A2 over-rejection — see §0.1] |
| reloc | 3 | 5 | 2 new: `BTM_CancelInquiry` (`@2396` vs `@776`), `btm_event_filter_complete` (`@2602` vs `@931`) — **stale map** (§0.2); AXFX ×2 + lyt dtor unchanged (i2f, A5 territory) |
| abi-boundary | 10 | 2 | `btu_hcif_process_event` (r4→r5, **live-in arg** — unsound to rename), `l2c_csm_config` (r3→r4, needs 1a) |
| loop | 7 | **1** | only `__wudDeleteHandler`; the other 6 are masked by A2 fields |
| rho/deadness | 4 | 1 | `rfc_alloc` (already ACCEPTED) |
| certified | 0 | 0 | |

### 0.1 BLOCKER for the plan's value — A2 over-rejects non-zero RA rotations

A2's role table (`_RA_LITERAL_OPCODES`, renaming_witness.py:119) drops the RA
field for **all** D/X-form load/stores, and `_register_fields` excludes it
from rho. That is correct for the r0-literal case (either side RA==0 ⇒ the
address semantics differ) but **over-rejects the pure-rotation case where BOTH
sides' RA are non-zero registers**: verified on six sampled fields failures —
`lha r0,0(r3)` vs `lha r0,0(r5)`, `lwz r0,4(r23)` vs `lwz r0,4(r22)`,
`addi r4,r30,616` vs `addi r4,r29,616`, `lbz r0,281(r29)` vs `lbz r0,281(r30)`
— every one is a sound rotation now failing gate 3 as `fields`.

**Fix (completeness, not soundness — soundness already holds):** make RA a
*conditional* register field: per slot, RA participates in rho unless
`retail_RA == 0 or decomp_RA == 0` (then it must be bit-equal). This requires
the gate-3 masks to become per-slot (`_gpr_fpr_masks` is per-opcode today) and
`_register_fields` to consult the decoded RA value. Soundness argument: with
both RAs non-zero, both sides read their RA registers and the shared-state
binding makes `retail.rA` ≡ `decomp.r_rho(A)` — equal EA ASTs; with either RA
zero, the address semantics differ unless bit-equal (reject). The
`lwzx r3,r0,r5` vs `lwzx r3,r6,r5` case stays rejected (retail reads no r0;
decomp reads r6 ⇒ EA differs unless the shared r6 is provably 0) — the
implementation's rejection there is sound even though doc-32 A2's accept test
claimed otherwise.

**This is the highest-value change now**: it restores the rotation class for
~16 of the 28 (and most of the 94), and it is a prerequisite for items 1a/2
below (their targets are masked by the fields failures).

### 0.2 Stale map — re-mine required

`BTM_CancelInquiry` and `btm_event_filter_complete` now fail gate 2 with new
TU-local numbers (`@776`, `@931`) not in `tools/coop/retail_reloc_map.json`
(no `btm_inq@776`/`btm_inq@931` entries). The objects changed under the
A1/A2/A3 rebuild. **Re-run `reloc_map.py mine` before any further sweep** —
the map is regenerable and holds no build lock.

---

## 1. Items, re-sequenced for the rev-3 baseline

### Item 0 (NEW, first): A2 refinement — per-slot RA rule (§0.1)

Implement + tests (accept: `lwz r0,4(r23)` vs `lwz r0,4(r22)` under rho;
reject: `lwz r0,0(r0)` vs `lwz r0,0(r4)`; regression: the landed A2 tests
must keep passing for true r0-literal cases). Re-sweep — expect most of the
16 fields failures to move back to rho/abi-boundary/loop.

### Item 1: r3 conditional fixedness (1a) + gate-5 trigger (1b) — smaller reach than rev-2

After item 0, the abi-boundary set is `btu_hcif_process_event` (r4→r5,
**live-in** — genuinely unsound to rename: the second argument arrives in r4;
SMT-only) and `l2c_csm_config` (r3→r4). 1a's original targets
(`__OSPlayTimeFadeLastAIDCallback`, `__HBMSYNSetupVolumeEnvelope`) return via
the region/fields path after item 0; both are void and write r3 only as dead
scratch, but A3's `_written_before_return` rule fixes any register written
before a return — so mirroring A3's rule for r3 does **not** unfix them. The
sound refinement is exit-live-out observability (r3 writes that never reach a
caller-observable exit are renamable for a void function). **Scope 1a as:
(a) add r3 to A3's conditional loop (needed for `l2c_csm_config` via 1b), (b)
exit-live-out modeling for the void-scratch case — gated on a soundness round,
or record the two void targets as SMT candidates.** 1b (gate-5 region
trigger) keeps its design; its only realistic target is `l2c_csm_config`
(needs 1a + item 2: `loop=True`).

### Item 2: loop second-cut — unchanged design, reach returns after item 0

`__wudDeleteHandler` is the only current loop-gate failure; the other six are
masked by A2 fields and will return to the loop gate after item 0. Design
unchanged from rev 2: split `_has_loop_or_non_return_indirect`, drop the
direct-backward-branch reject in `run_region_sliced_witness`, keep the
`bcctr`/`blrl` reject (executor records an `"indirect-branch"` terminal with a
symbolic CTR — comparison sound, reject is scope choice), loop-boundary guard
(reject when a boundary falls in a backward-branch span), rewrite
`test_loop_containing_target_rejected_first_cut`.

### Item 3: SIBios `.bss` — unchanged (no witness interaction)

`SITransfer` slot 9 is a genuine immediate diff (`addi r0,r31,0` vs
`addi r0,r31,64` — RA r31 both sides, not an RA rotation), so item 0 does not
touch it. Mechanism investigation as rev 2 (§3): declaration/attribute/`-ipa`
experiment matrix against the 8 FULL_MATCH siblings; reorder hypothesis
refuted (the 2-function `.text` delta does not reference `Packet`/`XferTime`).

### Item 4: `BTM_CancelInquiry` — re-mine first, then as rev 2

Currently fails at reloc (`@2396` vs `@776`) due to the stale map (§0.2).
After re-mine: expect the boundary-56 deadness failure to return (verified at
rev 2 with certified contracts). Phase A/B (characterise + bounded source)
then Phase C (value-splitting redesign — rev-2's `z3.eq` assertion over-
rejects; needs substitutive rebinding or solver comparison; gated).

---

## 2. Sequencing, scope, acceptance

1. **Item 0** — A2 per-slot RA refinement + tests → re-sweep (re-mine map
   first). Highest value; prerequisite for 1a/2.
2. **Item 2** — loop second-cut (reach: 7 after item 0).
3. **Item 1** — 1a (r3 into A3 loop + exit-live-out design, gated) + 1b
   (gate-5 trigger; target `l2c_csm_config`).
4. **Item 3** — SIBios mechanism investigation (independent; can run
   concurrently with 0–2).
5. **Item 4** — after re-mine: Phase A → B → C.

CI (full AGENTS.md gate): `gen_fixture_blob.py --check`,
`tools/ppc_equivalence/tests`, `tools.ppc_equivalence differential`,
`docs_sync --check`, `smell_report --check`, `tools/coop/tests`.

In scope: `renaming_witness.py`, `equivalence_check.py`,
`tools/coop/tests/test_renaming_witness.py`, `SOUNDNESS.md` via `docs_sync`,
`SIBios.c`, `retail_reloc_map.json` (re-mine only), this doc. **Out of
scope:** i2f magic pools (doc-32 A5), the 3 hard-fields cases, `bcctr`-
dispatch modeling, reloc-pipeline files. Shared-branch rules: no git
destructive ops; `hexdiff.py` for builds; claims before edits; no `--smt`
unless authorised.

## Files to read

- `renaming_witness.py` — `_RA_LITERAL_OPCODES` (:119), `_register_fields`
  (:148), `_gpr_fpr_masks` (:203), `_check_abi_fixedness` (:1090, A3 rules),
  `_TRUSTED_NON_64BIT_RETURNS` (:369), region path (:1579+).
- `equivalence_check.py` — `_try_renaming_witness` (:1422),
  `certify_unit_symbol` (~:2942), declared_return resolution (:2228–2236).
- `tools/coop/lib/targets.py` — `equivalence_certificate_error` (:72–260).
- `tools/ppc_equivalence/semantics.py` — loop overflow (:5347), bcctr
  terminal (:5640).
- `docs/ppc_equiv_work/31/32/30/28-reg-swap-witness*, witness-expansion*`.
