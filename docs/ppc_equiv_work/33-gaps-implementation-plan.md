# 33 — Implementation plan: certifying the doc-32 "not fixed" RVL gaps (rev 2)

Status: **draft, rev 2 — post round-1 adversarial review** (Kimi K3 integration /
GLM-5.2 soundness; findings verified against live runs by the plan owner
2026-08-03). Companion to `31-reg-swap-witness.md`, `32-witness-improvements-plan.md`,
`30-loop-support-implementation-plan.md`, `28-loop-jt-corpus-hits.md`,
`docs/witness_expansion_plan.md`.

Rev 2 incorporates round-1 findings where they **reproduce**; where a reviewer
claim was contradicted by a direct `certify_renaming_witness` run or objdump,
the plan records the correction (marked **[verified]** / **[refuted]**).

---

## 0. Problem statement

After doc-32 (A1–A5), these classes still fail the witness (28-function sweep,
2026-08-03; re-verified for this rev):

| Class | Functions (28-set) | Root cause |
|---|---|---|
| r3 abi-boundary (void, global) | `__OSPlayTimeFadeLastAIDCallback` (r3→r5), `__HBMSYNSetupVolumeEnvelope` (r3→r4) | r3 in `_UNCONDITIONALLY_FIXED_GPRS` even when dead-scratch **[verified: both fail `gate='abi-boundary'` on a consistent global rho]** |
| r3 abi-boundary (UINT8 return, global) | `btm_sec_mx_access_request` (r3→r4), `l2c_csm_config` (r5→r3) | same; both also `loop=True` **[verified: `gate='abi-boundary'` global, `_has_loop_or_non_return_indirect=True`]** |
| loop first-cut | `btm_acl_role_changed`, `bta_dm_pm_cback`, `process_service_search_attr_rsp`, `SDP_DeleteAttribute`, `btm_event_filter_complete`, `btm_process_inq_complete`, `__wudDeleteHandler` | `run_region_sliced_witness` rejects any backward branch / `bcctr` / `blrl` **[verified: ALL 7 have backward branches and ZERO `bcctr`/`bclr` — the region-path reject is the only blocker]** |
| fields (source/TU-layout) | `SITransfer` (baked `.bss` offset 0x40), `SIInterruptHandler`, `GetTypeCallback` (section-anchor) | decomp SIBios `.bss` emission puts `Packet` at 0x40, retail at 0x0; mechanism under investigation (see §3) |
| boundary-deadness | `BTM_CancelInquiry` | region boundary 56 changed-lane live-across **[verified: reproduces with certified-callee contracts]** |
| hard fields | `hcisu_h2_receive_msg` (rlwinm mask), `SetVolumeAllSeq` (float-pool order), `btm_sec_l2cap_access_req` (bc displacement) | scheduler/TU-pool driven; **out of scope** |

---

## 1. Conditional r3/r4 fixedness — extends doc-32 A3

### 1a. Global path: r3 conditional on `declared_return=="void"` only

`renaming_witness.py:295` `_UNCONDITIONALLY_FIXED_GPRS = {0, 1, 2, 3, 4, 13}`.

**[Refuted]** the rev-1 reviewer claim that `__OSPlayTimeFadeLastAIDCallback`
is a gate-2 i2f target — it fails `gate='abi-boundary'` on a consistent global
rho (verified). Both 1a targets are valid.

**[Adopted from GLM-5.2]** the r3-unfix is NOT sound on `None` (unknown)
`declared_return`: the CFG fixpoint models no exit live-out, and nothing
verifies an annotation against callers. **r3 becomes conditional ONLY when
`declared_return == "void"` is resolved from the registry (`targets.json` →
`extra.declared_return`, same source the SMT path uses at
equivalence_check.py:2228–2236).** On `None` or any non-void value, r3 stays
fixed (current behaviour). The "wrong annotation" integrity risk is shared
with the SMT path and is out of the witness's scope.

Design:
- `_UNCONDITIONALLY_FIXED_GPRS = {0, 1, 2, 13}` + a per-call conditional set:
  r3 fixed iff `declared_return != "void"` OR live-in (EABI arg range) OR
  live-across-call. (r0 remains fixed until doc-32 A2 lands; r4 remains fixed
  until A3 lands — §1a must not silently assume A2/A3 are committed
  **[verified: `_UNCONDITIONALLY_FIXED_GPRS` still contains 3 and 4]**).
- Thread `declared_return` through `certify_renaming_witness` →
  `check_gates` → `_check_abi_fixedness`, and into BOTH region-path call
  sites (renaming_witness.py ~:1510) and `run_region_sliced_witness`.
- **Certificate honesty (BLOCKER from Kimi K3, adopted):** the witness
  `ProofResult` (equivalence_check.py ~:1599) must embed
  `contract_resolution.abi_shape.declared_return` so the strict validator
  (targets.py `equivalence_certificate_error`, staleness check) does not
  reject the cert as stale when the registry declares `void`. Add a test:
  witness cert for a `declared_return="void"` target passes the validator.
- **Validator rho rule (MAJOR from Kimi K3, adopted):** either (a) drop the
  plan's claim that the validator enforces rho/declared_return consistency,
  or (b) add a validator rule: `evidence=="register-renaming-witness"` with a
  rho mapping r3 ⇒ `abi_shape.declared_return` must be `"void"`. Choose (b)
  with a test.

### 1b. Region path: trigger on gate-5 + per-region return-write analysis (gated)

**[Refuted]** Kimi K3's claim that `btm_sec_mx_access_request` /
`l2c_csm_config` "already reach the region path via `gate=='rho'`" — both fail
the GLOBAL gate 5 on a consistent rho and never reach the region path
(verified). The 2-line trigger (region slice when
`outcome.failure.gate == "abi-boundary"`) is therefore **not a no-op** for
these targets; it changes global-reject → region-reject.

**[Adopted from GLM-5.2]** the payoff for the UINT8-return targets is NOT
delivered by the trigger alone: per-region gate 5 uses function-level fixed
sets, so with `declared_return="UINT8"` every region's r3 rename is rejected.
Certifying them needs a **per-region return-write analysis**: a region's rho
may rename r3 only if the region provably never writes r3 observably (the
return value is written in a region where rho(3)=3). That analysis is real
design work — produce a soundness argument + accept/reject tests and gate it
on a second adversarial round. Default without it: 1b is sound but fixes
nothing (keep the trigger only if a global-abi-boundary target appears; none
is known in the 28-set).

Note both targets are `loop=True`: item 2 must land before either can certify.

---

## 2. Loop second-cut — bounded-iteration loops in the region path

`run_region_sliced_witness` (renaming_witness.py:1527–1533) rejects any
backward branch / `bcctr` / `blrl` before executing. **[Verified]** all 7
loop-group functions have backward direct branches and **no** `bcctr`/`bclr`,
so all 7 qualify for item 2 (rev-1's "only 3 of 7" is refuted — the reviewer
miscounted the switch functions, which compile to direct branch trees here).

Change:
- Split `_has_loop_or_non_return_indirect` into
  `_has_direct_backward_branch` + `_has_indirect_dispatch` (`bcctr`/`blrl`).
- Region path: drop the reject for **direct backward branches**; run the
  bounded executor (`max_loop_iterations`, default 2048). Loop overflow ⇒
  `ExecutionInconclusive` (semantics.py ~:5347 — no terminal recorded for an
  overflowing path) ⇒ degrade to SMT. **[verified: overflow raises before any
  terminal is recorded — no partial-unroll certificate possible]**
- Keep the reject for `bcctr`/`blrl`. **[corrected rationale]** the executor
  does NOT fail-closed on `bcctr` — it records an `"indirect-branch"` terminal
  with a symbolic CTR target (semantics.py ~:5640). The comparison of that
  terminal is sound (both sides share CTR under the shared-state binding), so
  the reject is a **scope/completeness choice, not a soundness requirement**;
  do not present it as fail-closed. `__wudDeleteHandler` (big switch) is a
  deferred `bcctr`-dispatch case only if its switch lowers to `bctr` —
  **[verified: it does not; it is a direct-branch tree, so it qualifies too]**
  — re-check per function with `hexdiff --asm` after the split.
- **Loop-boundary guard (placement, from Kimi K3):** after
  `boundaries = _rho_region_boundaries(...)` in `run_region_sliced_witness`,
  compute the address set inside every backward-branch span
  `[target, branch]`; if any boundary address falls inside a span, return
  `WitnessFailure(gate="loop", ...)` → SMT. Do **not** attempt "move to
  header" this iteration (soundness risk; the global path already executes
  the no-rebind loop case bounded). The four-lane deadness check
  (`_boundary_deadness_ok`) is the backstop for loop-carried lanes at
  non-loop boundaries.
- **Tests:** rewrite `test_loop_containing_target_rejected_first_cut`
  (test_renaming_witness.py:489) for the split predicates; add (a) region
  path accepts a direct-backward-loop target with no boundary in a loop span,
  (b) rejects when a boundary falls in a loop span, (c) overflow path → SMT
  not cert.

---

## 3. SIBios `.bss` layout — mechanism investigation before any reorder

**[Corrected from rev-1]** the cited `.text` orders were wrong (drawn from
symbol-table order, not `.text` offset). Objdump-verified orders:

- Retail: `CompleteTransfer(0)`, `SIInterruptHandler(0x300)`, `SIInit(0x6f0)`,
  `__SITransfer(0x7b0)`, `SISetCommand(0x960)`, `SITransferCommands(0x980)`,
  `SISetXY(0x990)`, `AlarmHandler(0x9f0)`, `SITransfer(0xa80)`,
  `GetTypeCallback(0xbf0)`, `SIGetType(0xeb0)`.
- Decomp: same prefix through `__SITransfer`, then `SISetXY(0x960)`,
  `AlarmHandler(0x9c0)`, `SITransfer(0xa50)`, `GetTypeCallback(0xbc0)`,
  `SIGetType(0xe80)`, `SISetCommand(0x1040)`, `SITransferCommands(0x1060)`.

Real delta: **2 functions** (`SISetCommand`, `SITransferCommands`) at
retail positions 5–6 vs decomp positions 10–11.

**[Open question — mechanism unknown]** the `.bss` mismatch is real (retail
`Packet`@0x0 vs decomp `Packet`@0x40) and NOT explained by:
- declaration order (decomp source declares `Packet` before `XferTime`, yet
  the rebuilt object emits `XferTime` first — verified by rebuild), nor
- `.text` first-reference order (retail's first function `CompleteTransfer`
  references `TypeTime`/`XferTime`, yet retail `.bss` starts with `Packet`).

The rev-1 reorder hypothesis ("moving 2 functions flips `.bss`") is therefore
**unlikely to work** — the two moved functions do not reference `Packet` or
`XferTime`. Re-scope item 3 as an **investigation**:
1. Experiment matrix on the real mechanism (declaration order variants,
   alignment/attribute effects, `-ipa` on/off, `-O4,p` vs `-O4,s`) — each
   tested via `hexdiff.py RVL_SDK/src/revolution/si/SIBios --all` with the 8
   FULL_MATCH siblings as the regression gate.
2. Only if a mechanism is found: apply it, claim
   `us-80364e50`/`us-803646d0`/`us-80364fc0`, verify the 3 targets clear
   their `fields` gates and the 8 siblings stay 100% (their relocs are
   named-symbol; verify by hexdiff, not assumption — Kimi K3's caution
   adopted).
3. If no mechanism is found within the experiment budget: record
   `SITransfer`/`SIInterruptHandler`/`GetTypeCallback` as SMT candidates
   (named-reloc siblings certify fine; the baked offset is a
   layout-level block).

Note: `list_text_functions` exists (`tools/ppc_equivalence/elf_symbols.py`);
rev-1's "does not exist" is refuted. Prefer `objdump -t | sort` for `.text`
ordering (symbol-table order ≠ `.text` offset order — the rev-1 error).

---

## 4. `BTM_CancelInquiry` boundary-deadness (us-802eb330)

**[Verified]** with certified-callee contracts the failure is
`abi-boundary: region boundary 56: a changed lane is live across the boundary`
— the plan's original premise **reproduces**. (Rev-1's executor-lemma failure
(`BTM_IsDeviceUp` no matched-callee lemma) occurs only with opaque/empty
contracts; Phase A must use `_load_certified_callees(proj, tid)`, not the
sweep's opaque-EABI path.) `loop=False`, `bcctr=0` — clean.

### Phase A — characterise (cheap)
Print the failing boundary/lane from the region driver (scratch script, not
committed code): which lane, which side live, whether the liveness is genuine
(used after the boundary) or a call-clobber artifact.

### Phase B — bounded source fix (3 attempts)
Reshape so the lane dies before boundary 56 (reorder a use, scope a local).
Callees are certified ⇒ a clean witness pass = EQUIVALENT_MATCH.

### Phase C — value-splitting design (gated)
**[Adopted from GLM-5.2]** the rev-1 sketch (z3.eq equality assertion at the
boundary) **cannot accept anything**: `_terminals_agree` has no constraint
parameter and compares AST identity — an asserted-but-not-compared equality
over-rejects, it does not false-certify. Phase C needs substitutive rebinding
or a solver-based comparison; produce a real design + tests and gate it on a
second adversarial round. Default: leave the function an SMT candidate.

---

## 5. Sequencing, scope, acceptance

1. **Item 1a** (foundation): conditional r3 on `declared_return=="void"`
   (registry-resolved), `abi_shape` embedding in the witness cert, validator
   rule (b), region-path threading. Re-sweep `__OSPlayTimeFadeLastAIDCallback`,
   `__HBMSYNSetupVolumeEnvelope`.
2. **Item 2** (loop second-cut): split predicate, drop direct-backward reject,
   loop-boundary guard, test rewrite. Re-sweep the 7 loop functions (all
   qualify; re-check `bcctr` per function after the split).
3. **Item 1b**: trigger extension + per-region return-write analysis design
   (gated on a second review); the trigger alone lands only if a
   global-abi-boundary target is found.
4. **Item 3**: `.bss` mechanism investigation (experiment matrix) → apply if
   found; else record SMT candidates.
5. **Item 4**: A → B → C (C gated).

Dependencies: 1b's targets need item 2 (both `loop=True`); 1a's cert work must
land before any 1a acceptance (validator rejects stale certs); item 2's tests
must be rewritten before the predicate split.

CI (full AGENTS.md gate, not the witness subset — Kimi K3's NIT adopted):
`gen_fixture_blob.py --check`, `tools/ppc_equivalence/tests`,
`tools.ppc_equivalence differential`, `docs_sync --check`,
`smell_report --check`, `tools/coop/tests`.

In scope: `renaming_witness.py`, `equivalence_check.py`,
`tools/coop/tests/test_renaming_witness.py`, `SOUNDNESS.md` via `docs_sync`,
`SIBios.c`, this doc. **Out of scope:** `bcctr`-dispatch modeling (deferred;
no known 28-set target uses it), the 3 hard-fields cases, i2f magic-double
pools (doc-32 A5), reloc-pipeline files. Shared-branch rules: no git
destructive ops; `hexdiff.py` for builds; claims before edits; no `--smt`
unless authorised.

## Files to read

- `renaming_witness.py` — gates 1–6, `_check_abi_fixedness` (:876, region
  call ~:1510), `_has_loop_or_non_return_indirect` (:645), region path
  (:1469–1670), `_rho_region_boundaries` (:1290–1350).
- `equivalence_check.py` — `_try_renaming_witness` (:1422), witness
  `ProofResult` (~:1599), `prove_unit_symbol` (:2759), `certify_unit_symbol`
  (~:2942), declared_return resolution (:2228–2236).
- `tools/coop/lib/targets.py` — `equivalence_certificate_error` (:72–260).
- `tools/ppc_equivalence/semantics.py` — loop overflow (:5347), bcctr
  terminal (:5640), `_terminals_agree`/`_execute_cfg_body`.
- `tools/ppc_equivalence/abi_infer.py` — declared_return/returns_i64.
- `docs/ppc_equiv_work/30-loop-support-implementation-plan.md`,
  `28-loop-jt-corpus-hits.md`, `docs/witness_expansion_plan.md`.
