# 33 — Implementation plan: certifying the doc-32 "not fixed" RVL gaps

Status: **draft for adversarial review** (Kimi K3 integration / GLM-5.2 soundness).
Companion to `31-reg-swap-witness.md`, `32-witness-improvements-plan.md`
(draft), `30-loop-support-implementation-plan.md`, `28-loop-jt-corpus-hits.md`,
and `docs/witness_expansion_plan.md` (region-sliced rho, four-lane deadness).

Source of the gap list: the reloc-drift sweep (2026-08-03) of the 28 RVL
functions whose gate-2 (reloc) failures were fixed by the mined-map
canonicalization / the `__wudLinkKeyEventStackCallback` declaration swap. After
gate 2 passes, the witness fails at later gates; doc-32 fixes A1–A5 cover
~10–12 of them. This plan covers the remainder, grouped into four work items.

---

## 0. Problem statement

After doc-32 (A1–A5), these classes still fail the witness (counts are from
the 28-function sweep; sibling classes exist in the 94):

| Class | Functions (28-set) | Root cause |
|---|---|---|
| r3/r4 abi-boundary (global) | `__OSPlayTimeFadeLastAIDCallback` (r3→r5, void), `__HBMSYNSetupVolumeEnvelope` (r3→r4, void) | r3 in `_UNCONDITIONALLY_FIXED_GPRS` even when dead-scratch (doc-32 A3 keeps r3 unconditionally fixed) |
| r3 abi-boundary (return-register, region-local) | `btm_sec_mx_access_request` (r3→r4), `l2c_csm_config` (r5→r3) — both return `UINT8` via r3 | gate-5 failure never reaches the region path (only gate-4 `rho` triggers `run_region_sliced_witness`); per-region gate 5 + deadness rebinding could accept a region-local scratch rename even when r3 is a return register, **if** the rename region provably never writes the return |
| loop first-cut | `btm_acl_role_changed`, `bta_dm_pm_cback`, `process_service_search_attr_rsp`, `SDP_DeleteAttribute`, `btm_event_filter_complete`, `btm_process_inq_complete`, `__wudDeleteHandler` | `run_region_sliced_witness` rejects any backward branch / `bcctr` / `blrl` before executing (line ~1527) |
| fields (source/TU-layout, not witness) | `SITransfer` (baked `.bss` offset 0x40), `SIInterruptHandler`, `GetTypeCallback` (section-anchor) | decomp SIBios `.bss` emission order puts `Packet` at 0x40, retail at 0x0; `.text` function order differs |
| boundary-deadness | `BTM_CancelInquiry` | region boundary 56: a changed lane is live across the boundary; two-direction deadness fails |
| hard fields | `hcisu_h2_receive_msg` (rlwinm mask), `SetVolumeAllSeq` (float-pool order), `btm_sec_l2cap_access_req` (bc displacement) | scheduler/TU-pool driven; **out of scope here** (source forensics, doc-32's "3 fields" + my 4th) |

---

## 1. Conditional r3/r4 fixedness — extends doc-32 A3

### 1a. Global path: make r3 conditional like A3's r4

`tools/coop/lib/renaming_witness.py:295`:
`_UNCONDITIONALLY_FIXED_GPRS = frozenset({0, 1, 2, 3, 4, 13})` →
after A2 (r0 removed) and A3 (r4 conditional), split into:

- `_UNCONDITIONALLY_FIXED_GPRS = frozenset({1, 2, 13})` (r0 handled by A2's
  D-form-RA position rule; r3/r4 conditional)
- r3 conditional rule (mirrors A3's r4 rule, with one addition):
  fixed iff
  (a) live-in at entry in the EABI arg range (`live_in & _EABI_ARG_GPRS`,
  already computed), or
  (b) live-across-call (already computed), or
  (c) `declared_return` is **not** `"void"` (r3 carries a scalar return
  value; 64-bit/aggregate also fixes r4), or
  (d) `declared_return` is unknown (`None`) — **conservative: fixed**
  (current behaviour).

Threading: `certify_renaming_witness(..., declared_return: str | None = None)`
→ `check_gates(..., declared_return)` → `_check_abi_fixedness(..., declared_return)`.
`_try_renaming_witness` (equivalence_check.py:1422) gains `declared_return` and
passes it; both call sites resolve it the same way the SMT path already does —
`prove_unit_symbol` has it as a kwarg; `certify_unit_symbol` (line ~2942) and
the registry fallback (equivalence_check.py:2228–2236:
`targets.json` → `extra.declared_return`) must be wired so the witness sees the
same value the SMT probe would. Default `None` ⇒ r3/r4 fixed ⇒ zero behaviour
change when a caller does not thread it.

Certificate honesty: the witness certificate (`witness_payload`) must record
the `declared_return` used, and the strict validator
(`equivalence_certificate_error`) must accept a rho containing r3/r4 only when
the certificate's declared_return justifies it. Verify the
`proof_request_hash` provenance includes the declared_return so the audit
trail stays honest.

**Expected fixes (global, void):** `__OSPlayTimeFadeLastAIDCallback`,
`__HBMSYNSetupVolumeEnvelope`.

### 1b. Region path: trigger region slicing on gate-5 failures

`certify_renaming_witness` currently falls to the region-sliced witness only
when `outcome.failure.gate == "rho"`. A global gate-5 (`abi-boundary`) failure
with a *region-local* rename never gets the region path's per-region gate 5 +
boundary-deadness rebinding — even though `run_region_sliced_witness`
already runs `_check_abi_fixedness` per region (line ~1510) and would reject a
function-wide rename correctly (no boundaries ⇒ one region ⇒ global rho ⇒
gate 5 rejects).

Change: also attempt `run_region_sliced_witness` when
`outcome.failure.gate == "abi-boundary"`. Soundness rests on the existing
per-region gate 5 (function-level live-in/live-across/declared_return fixed
sets) + the four-lane deadness rebinding — no new soundness machinery, only a
trigger extension. Confirm the region path's `live_in`/`live_across`
computation still uses the FULL function's sets (it does: `_liveness_sets`
runs over all instructions in `_check_abi_fixedness`), so a region cannot
hide an entry-live or call-live rename.

**Expected fixes (region-local, return-register):** `btm_sec_mx_access_request`,
`l2c_csm_config` — **only if** the r3 rotation is contained in a region that
never writes the return path; the reviewers must construct the counterexample
if this is unsound (e.g. a region whose rho renames r3 while a later region
writes r3 with rho(3)=3 — the return write is fixed, the scratch region's
write is dead ⇒ likely sound, but prove it).

---

## 2. Loop second-cut — bounded-iteration loops in the region path

`run_region_sliced_witness` (renaming_witness.py:1527–1533) rejects any
backward branch / `bcctr` / `blrl` before executing. The executor
(`execute_cfg` via `_run_region`) already takes `max_loop_iterations`
(default 2048 from equivalence_check) and the engine has loop support
(doc-28/30: CTR-affine, compare-affine, memory-loop recognizers + bounded
unrolling fallback). The global path (`run_structural_witness`) already
executes loops bounded; only the region path has the first-cut reject.

Change:
- Split `_has_loop_or_non_return_indirect` into
  `_has_direct_backward_branch` (loop) vs `_has_indirect_dispatch`
  (`bcctr`/`blrl` — jump tables / vtable / non-return indirect).
- Region path: drop the reject for **direct backward branches**; let the
  bounded executor run (loop iterations capped at `max_loop_iterations`;
  exceeding ⇒ `ExecutionInconclusive` ⇒ degrade to SMT, never certify).
- Keep the reject for `bcctr`/`blrl` (jump-table dispatch needs the
  `jump_table` finder integration — doc-28/30 scope, **deferred**; the
  executor marks indirect branches inconclusive anyway, so this is
  conservative, not a lost opportunity).

Soundness/design constraints for review:
1. **Region boundaries must not fall inside a loop body.** `_rho_region_boundaries`
   splits at bijection conflicts and call sites; a boundary inside a loop body
   would rebind lanes mid-iteration, breaking the loop's register invariants.
   Specify: when a boundary's address lies inside a backward-branch span, move
   it to the loop header (or reject → SMT). Reviewers: verify the rebinding
   driver + executor's loop-unrolling interact correctly with `paused_out`
   resumption across the boundary.
2. The four-lane deadness assertion at boundaries is computed with the loop
   fixpoint liveness (already `_liveness_sets` handles loops) — confirm it
   stays sound when the executor unrolls a loop that spans a region.

**Expected fixes:** `btm_acl_role_changed` (counted loop), and the
`bta_dm_pm_cback` / `btm_*` group **if** their switches compile to direct
branch trees rather than `bcctr`; `__wudDeleteHandler` (big switch) likely
stays on the deferred `bcctr` path. The plan must verify per function which
it is (hexdiff `--asm`, look for `bcctr`).

---

## 3. SIBios TU-order reorder (source experiment — not a witness change)

### Hypothesis

`.bss` emission order follows first-reference order across the TU's `.text`
function order (evidence: SIBios declares `Packet` before `XferTime` yet the
object emits `XferTime` first; `Alarm` is declared last but sits 4th; the
decomp `.text` order differs from retail). Reordering the 11 function
definitions in `libs/RVL_SDK/src/revolution/si/SIBios.c` to the retail `.text`
order should flip `.bss` emission to `Packet` at 0x0, fixing:

- `SITransfer` (us-80364e50): `&Packet[chan]` stops baking +0x40
  (`addi r0, r31, 64` → `0`), witness gate-3 `fields` clears;
- `SIInterruptHandler` (us-803646d0), `GetTypeCallback` (us-80364fc0):
  section-anchor `...bss.0` becomes `Packet`-anchored (or a map-covered
  offset-0 anchor — either way the baked offsets clear).

Retail `.text` order (from the retail object): `AlarmHandler`,
`CompleteTransfer`, `SIInterruptHandler`, `SIInit`, `__SITransfer`,
`SISetCommand`, `SITransferCommands`, `SISetXY`, `SITransfer`,
`GetTypeCallback`, `SIGetType`. Current decomp order:
`SIInterruptHandler`, `__SITransfer`, `GetTypeCallback`, `AlarmHandler`,
`CompleteTransfer`, `SITransfer`, `SIInit`, `SIGetType`, `SISetXY`,
`SISetCommand`, `SITransferCommands`.

### Procedure

1. Claim `us-80364e50`, `us-803646d0`, `us-80364fc0` (owner `paseo-reloc-fix`
   or the impl agent).
2. Reorder function definitions (mechanical move of whole function blocks;
   keep forward declarations).
3. `hexdiff.py RVL_SDK/src/revolution/si/SIBios --all` — verify the 8
   FULL_MATCH functions stay 100% (their relocs are named-symbol →
   order-independent) and the 3 targets improve. Revert on any regression.
4. If the anchor becomes `...bss.0` at offset 0 (the WUD quirk), the static
   match still clears the baked offsets; re-mine the map and re-run the
   witness.

Risk note: `.text` order is not compared by objdiff (per-function matching),
and the TU split budget is unchanged (total `.text` size invariant under
reorder). `-ipa` behaviour with reordered functions is the main unknown.

---

## 4. `BTM_CancelInquiry` boundary-deadness (us-802eb330)

Failure: `region boundary 56: a changed lane is live across the boundary
(two-direction deadness)`. This is the region path's `_boundary_deadness_ok`
assertion failing for one lane at one boundary — the one "real"
boundary-deadness failure in doc-32's abi-boundary count.

### Phase A — reproduce and characterise (cheap)

Re-run the region-sliced witness on `BTM_CancelInquiry` with
`certify_renaming_witness` and print the failing boundary/lane (extend the
failure detail temporarily in a scratch script, not in committed code).
Determine: which lane, which register, which side is live, and whether the
liveness is genuine (a value used after the boundary) or an artifact of the
fixpoint (e.g. a call-clobbered volatile counted as live-across).

### Phase B — source-level deadness fix (bounded, 3 attempts)

If the lane's liveness is genuine but the source can be reshaped so the value
dies before the boundary (reorder a use, scope a local), do it — bounded to 3
hexdiff iterations. `BTM_CancelInquiry`'s callees are certified
(`BTM_IsDeviceUp` etc.), so a clean witness pass = EQUIVALENT_MATCH.

### Phase C — value-splitting design (only if A/B fail)

This is doc-32's explicitly deferred "value-splitting region boundaries".
Design: at a boundary where a changed lane is live-across on one side, split
the lane's value into pre/post boundary symbolic variables with an equality
constraint at the boundary (phi-like), so the region rho rebinding does not
lose the carried value. Soundness: the equality constraint must be structural
(`z3.eq`) and participate in the terminal comparison; a divergence rejects.
This is **real design work** — produce a soundness argument + accept/reject
tests and gate it on a second adversarial round before implementation.
Default: leave the function as an SMT candidate (callees certified; SMT would
certify).

---

## Sequencing, scope, and acceptance

1. **Item 1a** (conditional r3, global) — smallest, self-contained; threads
   `declared_return`; tests + re-sweep `__OSPlayTimeFadeLastAIDCallback`,
   `__HBMSYNSetupVolumeEnvelope`.
2. **Item 1b** (gate-5 region trigger) — 2-line trigger + region-path tests;
   re-sweep `btm_sec_mx_access_request`, `l2c_csm_config`.
3. **Item 2** (loop second-cut) — split predicate + drop direct-backward
   reject; loop-boundary constraint; re-sweep the 7 loop functions.
4. **Item 3** (SIBios reorder) — source experiment, independent of 1–2; can
   run in parallel (claims + hexdiff only, no witness code).
5. **Item 4** — A (characterise) → B (bounded source) → C (design doc, gated).

In scope: `tools/coop/lib/renaming_witness.py`,
`tools/coop/lib/equivalence_check.py` (declared_return threading),
`tools/coop/tests/test_renaming_witness.py`, `SOUNDNESS.md` via `docs_sync`,
`libs/RVL_SDK/src/revolution/si/SIBios.c`, this doc.

**Out of scope:** jump-table `bcctr` dispatch modeling (doc-28/30 scope);
the 3 hard fields cases (`hcisu_h2` rlwinm mask, `SetVolumeAllSeq` float-pool
order, `btm_sec_l2cap_access_req` bc displacement — scheduler/TU-pool
forensics); i2f magic-double pools (doc-32 A5 territory, value-equal gate 2);
anything touching the reloc-fix pipeline's files.

CI: `python tools/ppc_equivalence/gen_fixture_blob.py --check`,
`python -m unittest discover -s tools/ppc_equivalence/tests -p "test_*.py"`,
`python -m tools.ppc_equivalence.docs_sync --check`, plus the
`tools/coop/tests` witness suite. Shared-branch rules apply: no git
destructive ops; `hexdiff.py` for builds; claim targets before editing;
no `--smt` unless explicitly authorised.

## Files to read

- `tools/coop/lib/renaming_witness.py` — gates 1–6, `_check_abi_fixedness`
  (876), `_has_loop_or_non_return_indirect` (645), region path (1469–1670).
- `tools/coop/lib/equivalence_check.py` — `_try_renaming_witness` (1422),
  `prove_unit_symbol` (2759), `certify_unit_symbol` (~2942), declared_return
  registry resolution (2228–2236).
- `tools/ppc_equivalence/abi_infer.py` — `declared_return` / `returns_i64`.
- `docs/ppc_equiv_work/30-loop-support-implementation-plan.md`,
  `28-loop-jt-corpus-hits.md` — engine loop/jump-table machinery.
- `docs/witness_expansion_plan.md` — region rebinding, four-lane deadness.
