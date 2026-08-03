# 32 — Witness improvements plan: certifying the remaining pure-reg-swap RVL pairs

Status: **rev 4 — implemented, pending implementation review** (GLM-5.2 soundness,
Kimi K3 integration; both re-verified against code; second reviewer ran as
GLM-5.2/max). Round-1 findings G1-G11 / I1-I13 and round-2 findings F1-F11 /
R2-1-R2-10 are recorded below with resolutions. Companion to
`31-reg-swap-witness.md` and `docs/witness_expansion_plan.md`.

## R4. Implementation-review findings and fixes (2026-08-04)

The committed implementation (78dbecaa3) was adversarially reviewed by
GLM-5.2 (soundness) and Kimi K3 (integration) against the running code. Both
found false certificates; all are fixed and regression-tested:

| # | Sev | Area | Finding | Fix |
|---|---|---|---|---|
| IR1 | BLOCKER | A1 | **`_boundary_deadness_ok` ignores the PS1 sub-lane** — an FPR rebind replaces both `fpr[r]` and `ps1[r]`, but the four-lane check only consults `32+r`. A byte-identical `psq_st` + a double-precision kill of `fpr[fS]` left `ps1[fS]` live; the rebind stored a fresh variable on both sides and the memory comparison self-agreed — false certificate (exploitable today). | Four-lane check now also tests PS1 sub-lanes `64+{r, old_d, new_d, old_retail_of_new}` for FPR rebinds. |
| IR2 | BLOCKER | A3 | **Non-link `bcctr` indirect exits uncovered** — `_tail_call_reads_lane` iterated only `Opcode.B`; a trusted-metadata-unfixed return lane could be live-out to an unknown indirect callee (latent until the deferred `declared_return` entries land). | Non-link `BCCTR` now conservatively returns True (fix the lane); plus relocation-less absolute `b` tail calls covered (Kimi F2) and `callee_contracts=None` guarded (Kimi F3). |
| IR3 | BLOCKER | gate 5 | **Gate 5 validated the partial rho, execution used the extended permutation** (pre-existing, Kimi F1): partial `{5:3}` forced the extension to map r3 elsewhere; `li r5,1; blr` vs `li r3,1; blr` certified a non-equivalent pair. | `_check_abi_fixedness` now validates `gpr_perm()`/`fpr_perm()` (the actual execution permutation), not the partial dict. |
| IR4 | MAJOR | tests | **F1/default-FIXED tests passed for the wrong reasons** (GLM MAJOR 1/2, Kimi F5): the psq test rejected at gate-5 live-in (f5 EABI arg), not the ps1 use; the default-FIXED test actually exercised (c). | Both replaced with the exploit shapes: non-EABI f20/f21 psq rebind (rejects via PS1 deadness), `b`-tail-call body with no metadata (rejects via default-FIXED). |
| IR5 | MAJOR | tests | **No `bcctr`, absolute-tail-call, perm-gate, or abi_shape-cert tests** (GLM MAJOR 3, Kimi F4). | Added `ImplReviewRegressionTests`: bcctr exit keeps f1 fixed; absolute `b` keeps r4 fixed; perm-based gate 5 rejects the `{5:3}` repro; witness cert carries `abi_shape` and the §2.5.4 validator matches/mismatches. |
| IR6 | MINOR | A2 | **2-operand FP ops role table over-classified bits 6-10 as fC** (pre-existing, GLM MINOR 1) — spurious rho conflicts (e.g. fmr f1 vs f2 collided on fC). | `FRSP/FCTIW/FCTIWZ/FNEG/FMR/FNABS/FABS` now return `(_FD, _FB)` matching `_use_def`. |
| IR7 | NIT | certs | Stale assumptions text claimed "rho fixes r0" (pre-A2). | Updated to the A2/A3 policy. |

Tests: `test_renaming_witness.py` 54 (exploit shapes + regressions), full engine
suite 1972 green, fixture blob green, `tools/coop/tests` unchanged (5
pre-existing failures). Committed as the follow-up to 78dbecaa3.



All of I5 (provenance), A2, A1, A3-plumbing, and the A3 structural check are
implemented and tested:

- **I5:** `renaming_witness.py` added to `CERTIFIER_SOURCE_PATHS` (provenance.py).
- **A2:** `_RA_LITERAL_OPCODES` (D/DS/X-indexed load-store + ADDI/ADDIS + PSQ
  D-forms); r0 dropped from `_UNCONDITIONALLY_FIXED_GPRS`, added to
  `_VOLATILE_GPRS`; `_use_def_numbered` RA-read guard; hexdiff frozen
  `_classifier_*` copy + drift-detector tests; identity-first
  `_extend_permutation` (the extension previously rotated unused registers
  when A2 dropped identity RA entries from rho — a completeness bug found and
  fixed during implementation).
- **A1:** PSQ D-forms removed from `REJECT_OPCODES` (X-forms stay);
  byte-identity exemption in `_stream_validation_failure`; non-register
  fS/rA; explicit `_use_def` PSQ entries incl. `ps1[fS]` store use (F1);
  per-region post-rho belt-and-suspenders `_psq_operands_rho_fixed`.
- **A3:** r4/f1 conditional fixedness (default FIXED; unfixed only on trusted
  non-64-bit `declared_return` + no write-before-return + no tail-call callee
  reading the lane); forward `_written_before_return` (DFS over
  `_cfg_successors`) + contract-aware `_tail_call_reads_lane` (F7); registry
  `declared_return` lookup + explicit `abi_shape=` kwarg in
  `_try_renaming_witness` / `_build_equivalence_certificate` (R2-1).

Tests: `test_renaming_witness.py` 43 -> 55 (A2 + A1 + A3 groups), full
`tools/ppc_equivalence/tests` suite (1972) green, `gen_fixture_blob.py --check`
green, `tools/coop/tests` unchanged apart from 5 pre-existing failures (relaxed
hash checks + byte-identical summary, reproduced on clean HEAD).

Measured payoff on sampled targets: the psq reject-list targets now pass gate 6
and fail at the NEXT gate (reloc-name drift needing source `extern` fixes —
the parallel reloc-fix workflow's domain); `GXCopyTex`'s r0<->r6 region [4,11)
now passes A2 and fails only at the documented out-of-scope value-splitting
region [11,12) (read-write-overlap rho). Remaining blockers are reloc-name
drift, other agents' in-flight source changes, and the out-of-scope
value-splitting class.


---

## 0. Problem statement

The RVL targets were swept through the current witness
(`tools/coop/lib/renaming_witness.py`, `certify_renaming_witness`). 195 RVL
targets are non-FULL_MATCH (plan rev 1 said 200; 195 is the targets.json count).
Of those, 94 are **pure register-swap pairs** by hexdiff's classifier (0
structural, >0 reg-swap). Only **1 of the 94 certifies today**
(`__HBMSYNResetAllControllers`). The integration reviewer reproduced the sweep
independently (passes size+mnemonic) and confirmed the non-reloc gate counts
(rho 22 vs 23, abi-boundary 15 vs 14, loop 12 vs 12, reject-list 12 vs 10,
structural 2 vs 1, execute+deadline 2 vs 2; the reloc and fields counts are the
moving set boundary plus the parallel reloc-fix pipeline).

Two soundness invariants must survive every change:
1. **Shared-state symbolic execution** — retail `r_i` and decomp `r_rho(i)`
   share one variable; divergence always degrades to SMT, never certifies.
2. **Full-state structural terminal comparison** — `_terminals_agree`
   compares every GPR/FPR/PS1 lane unconditionally (no dead-lane masking in the
   global path), memory, LR, CR/XER/FPSCR, and all non-register state.

---

## R1. Round-1 review findings and resolutions

### GLM-5.2 (soundness)

| # | Sev | Change | Finding | Resolution in rev 2 |
|---|---|---|---|---|
| G1 | MINOR | A1 | "psq/FPSCR semantics are capability-stubbed" is false — `_psq_store_pair`/`_psq_load_pair`, `MFFS`/`MTFSF`, `DCBZ` are fully modeled (semantics.py:2007/1967/4152/3247). Soundness rests on shared binding + structural comparison, not stubness. | A1 rationale corrected; no code impact. |
| G2 | MINOR | A1 | Post-rho check is belt-and-suspenders (structural comparison is the backstop); region-path placement and psq field classification unspecified. | A1 rev: per-region post-rho (I12) + explicit psq operand classification. |
| G3 | MAJOR | A2 | **X-form indexed load/store RA=0 IS literal zero** (semantics.py:3531/3559 `ops.const(0) if ra == 0`), not a real register. The rev-1 accept test `lwzx r3,r0,r5` vs `lwzx r3,r6,r5` **does not certify** — addresses diverge, structural comparison rejects. | A2 rev: X-form indexed load/store RA joins the non-register set; accept test removed; X-form RA renameable only for arithmetic (add/subf/…) and cmpw/cmpwi-family (which read `gpr[ra]`). |
| G4 | MAJOR | A2 | `cmpwi`/`cmplwi`/`mulli`/`addic`/`addic.`/`subfic` are NOT r0-literal — they read `state.gpr[ra]` (semantics.py:3455; the literal-zero guard at 3296 is `ADDI, ADDIS` only). Rev-1 `_D_FORM_RA_OPCODES` enumeration over-broad. | A2 rev: rA=0-literal set = exactly `{ADDI, ADDIS}` ∪ D-form/DS-form integer load-store + update forms ∪ `{LMW, STMW}` ∪ FP D-form load-store + update ∪ PSQ D-forms. `cmpwi`/`mulli`/`addic`/`subfic` RA stays renameable. |
| G5 | MINOR | A2 | r0 missing from `_VOLATILE_GPRS` (range(3,13)) — latent only; opaque-EABI clobbers r0. | Add r0 to `_VOLATILE_GPRS` for EABI consistency. |
| G6 | MINOR | A2 | `_use_def` over-approximates X-form load/store RA as a use when RA=0; safe direction (over-live), only over-strict at region rebinds. | Mirror the engine's `if a[1]:` guard in `_use_def` for load/store RA. |
| G7 | BLOCKER | A3 | **False certificate.** `_terminals_agree` is rho-aligned: with rho(4)=5, retail gpr[4]=X4 vs decomp gpr[5]=X4 self-agree. A `long long` return + stale `declared_return="i32"` + rho{4↔5} certifies while the caller reads decomp r4=garbage. `combine_abi_shapes` narrows `returns_i64` (abi_infer.py:330) so a wrong declared shape overrides the conservative inference. | A3 rev: condition (c) replaced by a **structural** check — r4 (and f1) fixed iff written on any path before a non-link `bclr` return (equivalent to conservative `inferred.returns_i64`). Never trust the narrowed/declared shape. |
| G8 | MINOR | A3 | Exit live-out gap confirmed: terminals have `succ=()`, so the fixpoint cannot see return registers — corroborates G7. | Structural write-before-return check (G7) is computed from the instruction stream, not the fixpoint. |
| G9 | NIT | A3 | The 64-bit split pair (r3/r4) is the load-bearing case; r3 stays fixed, r4 is the high word. | Covered by G7's structural check. |
| G10 | NIT | A4 | Sound; investigate cross-region path accounting before raising caps. | See I8 — the WPADiExcludeButton diagnosis changed. |
| G11 | NIT | A5 | Sketch sound-in-principle; out of scope. | See I10 — framing corrected. |

### Kimi K3 (integration; ran as GLM-5.2/max)

| # | Sev | Change | Finding | Resolution in rev 2 |
|---|---|---|---|---|
| I1 | BLOCKER | A1 | **A1 breaks `test_reject_list_opcodes_fall_back_to_smt`** (test_renaming_witness.py:242): its five cases (`psq_l`/`mffs`/`mtfsb0`/`mcrfs`/`dcbz` + blr) are byte-identical with empty rho ⇒ under A1 they certify; the plan's "must keep passing" regression claim is false. | A1 rev: the test is **rewritten** (reject case uses a non-byte-identical reject word or asserts the new exempted behaviour). A1 scoped to the confirmed motivating set (psq_st/psq_l) with a test per opcode family. |
| I2 | BLOCKER | A3 | **Feasibility: all 7 claimed targets have `declared_return = None`** in targets.json (only 3 RVL targets carry one; none in the 93). `infer_abi_shape` sets `returns_i64=True` whenever either side writes r4 (abi_infer.py:60) — `__wudSyncDone` writes r4 as scratch, so inference keeps r4 fixed. As specified, A3 certifies ~0 targets. | A3 rev: the structural write-before-return check (G7) makes `__wudSyncDone`-style targets certifiable **without** any registry change (r4 written but never on a path to a return ⇒ unfixed); the payoff is re-measured. |
| I3 | MAJOR | A1 | **Region-path use/def hole:** `_use_def_numbered` skips REJECT opcodes; an exempted `psq_st f31,0(r1)` at a region end reads f31, but the fixpoint models no use ⇒ f31 looks dead at the boundary ⇒ rebind unsound. | A1 rev: model psq uses/defs in `_use_def_numbered` (psq_st: use fS+rA; psq_l: def fD+ps1[D], use rA; update forms also def rA). |
| I4 | MAJOR | A3 | **Certificate staleness:** the witness cert call passes no `abi_shape` (equivalence_check.py:1608); the §2.5.4 validator rejects certs when the registry has a `declared_return` and the cert has None. A3 makes this acute. | A3 rev: thread `declared_return` into `_build_equivalence_certificate` for the witness path and emit `abi_shape={"declared_return": …}` (raw string — I13), as the SMT path already does. |
| I5 | MAJOR | all | **Provenance hole:** `renaming_witness.py` is in neither `CERTIFIER_SOURCE_PATHS` nor `engine_hash` (provenance.py:20); A1–A3 semantics changes would not invalidate old certificates (hash checks currently relaxed at targets.py:107–117). | **Implement first:** add `tools/coop/lib/renaming_witness.py` to `CERTIFIER_SOURCE_PATHS` before any semantic change. |
| I6 | MAJOR | A2 | **hexdiff coupling:** hexdiff imports `_gpr_fpr_masks` (hexdiff.py:126) for `_pure_reg_swap`; A2 moving D-form RA out of the register mask changes hexdiff's classification repo-wide, mutating the plan's own 94-set instrument and other agents' diffs. | A2 rev: decouple — hexdiff keeps a frozen copy of the pre-A2 mask logic (explicitly documented as classifier-only), or A2 ships with a coordinated re-measurement of the target set. Choose decoupling (isolated blast radius on a shared branch). |
| I7 | MAJOR | A2 | `cmpwi` wrongly listed in A2's D-form-RA set (semantics.py:3455 reads `gpr[ra]`); aligns with G4. | A2 rev: set per G4. |
| I8 | MAJOR | A4 | **`WPADiExcludeButton` misdiagnosed:** at production `max_paths=4096, deadline=30s` it fails at `structural`, not path-limit-256 (the 256 was the sweep's witness-default budget). `__wudStackCheckDeviceInfo` does reproduce the 30s deadline. | A4 rev: re-measure at production budgets; investigate the structural divergence for WPADiExcludeButton; only the deadline case is a resource knob. |
| I9 | MINOR | §0 | 200 → 195 targets; gate counts otherwise credible. | §0 corrected. |
| I10 | MINOR | A5 | Gate 3 **skips** the raw-bit compare when a reloc is present (`if r_reloc is None:`); the displaced bits are not the enforcement mechanism. A5's authority must be the address-resolution check. | A5 note amended; still out of scope this round. |
| I11 | MINOR | A1 | Reject-list failures are 12 (not 10), all `psq_st`; byte-identical claim fully confirmed; epilogue `psq_l` also byte-identical but never gates (reject fires at the first psq slot). | Count corrected; A1 must handle both prologue psq_st and epilogue psq_l. |
| I12 | MINOR | A1 | Post-rho check must run **per-region** in the region path (slots are atomic, so a slot never straddles a boundary — but the check must consult the containing region's rho, in both `check_gates` and `run_region_sliced_witness`). | A1 rev: per-region placement specified. |
| I13 | NIT | A3 | `abi_shape_from_declared_return("aggregate")` returns None; the witness must branch on the **raw** `declared_return` string, not the narrowed shape. | A3 rev: thread the raw string. |

**Verdicts after round 1:** A1 rework (test rewrite + psq use/def + per-region check),
A2 rework (correct r0-literal set + hexdiff decoupling), A3 rework (structural
return check + cert abi_shape; feasibility restored without registry changes),
A4 rework (re-measure), A5 out of scope.

## R2. Round-2 review findings and resolutions

### GLM-5.2 r2 (soundness)

| # | Sev | Change | Finding | Resolution in rev 3 |
|---|---|---|---|---|
| F1 | BLOCKER | A1 | **psq_st use/def omits `ps1[fS]`** — `_psq_store_pair` reads BOTH `fpr[rs]` and `ps1[rs]` for W=0 stores (semantics.py:3911-3913/2016-2024). Without the ps1 use, region-boundary liveness sees `ps1[5]` dead → rebind to a fresh shared variable on both sides → the unconditional memory comparison self-agrees → **false certificate**. Exactly the I3 hole, on the ps1 lane. | A1 rev 3: `psq_st` uses = `{fS, ps1[fS], rA}` (rA guarded `ra!=0`); `psq_l` defs = `{fD, ps1[fD]}`, uses = `{rA}`. |
| F2 | BLOCKER | A3 | **Internal contradiction:** A3 §3 says default r4 **FIXED** (only trusted metadata unfixes); the I2 resolution says `__wudSyncDone` certifiable **without registry change** (default unfixed). Mutually exclusive. If default-unfixed, G7 reopens via `b` tail-call exits (live-out invisible to the fixpoint). | A3 rev 3: **default FIXED is the only sound choice.** I2's "certifiable without registry change" claim is **retracted**. The `b`-tail-call gap is closed by default-FIXED, not by (c) (F7). |
| F3 | BLOCKER | A3 | **Accept test impossible:** `__wudSyncDone` writes r4 before a return ⇒ (c) fires ⇒ r4 fixed **regardless of** `declared_return` ⇒ the "accept with declared_return=i32" test cannot certify. | A3 rev 3: accept test uses a **synthetic** shape where r4 is never written on any return path AND a non-i64 registry `declared_return` unfixes it. The `__wudSyncDone` label moves to the *reject* test. |
| F4 | MAJOR | A2 | `test_cx2_zero_register_encoding_rejected` (test_renaming_witness.py:139) asserts gate `abi-boundary`; under A2 the pair fails at gate **`fields`** (ADDI RA becomes bit-equal). "Stays green" is false. | A2 rev 3: rewrite the test to assert gate `fields`; replace the CX-2 rationale (r0 no longer fixed; rejection is RA bit-equality). |
| F5 | MAJOR | A2 | G6 liveness fix must also cover **ADDI/ADDIS** (engine literal-zero guard at semantics.py:3296), not only load/store forms. | A2 rev 3: mirror `ra != 0` for the ADDI/ADDIS group too (safe direction; completeness). |
| F6 | MAJOR | A1 | Ambiguity: are psq fS/rA register or non-register fields? If register, byte-identical slots contribute identity rho and a renamed operand elsewhere fails gate 4 (`rho`) first — the post-rho test "passes for the wrong reason". | A1 rev 3: **fS/rA are non-register** (bit-equal via gate 3); the byte-identical raw-word check is the exemption gate; the post-rho check is retained as per-region belt-and-suspenders only (G2/F6). |
| F7 | MAJOR | A3 | (c) does not cover **`b` tail-call exits** (no `bclr`/`bcctr`; live-out at terminals invisible). | A3 rev 3: default-FIXED covers these (F2); stated explicitly. |
| F8 | MINOR | A2 | DCBZ/DCBZ_L also treat RA=0 as literal (semantics.py:3261) — omitted from the "exactly complete" claim; reject-listed so harmless. | A2 rev 3: note the reject-listed carve-out. |
| F9 | MINOR | A1 | psq rA use should mirror the engine's `if a[1]:` guard (semantics.py:3874). | A1 rev 3: `ra != 0` guard on the psq rA use. |
| F10 | NIT | A4 | Sound framing; witness-only `deadline_ms` correctly scoped. | — |
| F11 | NIT | A5 | Framing correction (I10) accurate. | — |

### Kimi K3 r2 (integration; ran as GLM-5.2/max)

| # | Sev | Change | Finding | Resolution in rev 3 |
|---|---|---|---|---|
| R2-1 | BLOCKER | A3 | **`abi_shape` plumbing targets a dead path:** the witness `ProofResult` never sets `contract_resolution` (result.py:433 default None), so the §2.5.1 embedder at equivalence_check.py:1878-1885 never fires for the witness path; `proof_request_hash` (1513) also omits `abi_shape`. "As the SMT path already does" is not wired. | A3 rev 3: pass `abi_shape={"declared_return": raw}` as an **explicit kwarg** to `_build_equivalence_certificate` (already accepts it at :906, writes at :961-962); add a registry `declared_return` lookup to `_try_renaming_witness` (mirroring the SMT path's non-gated lookup at 2224-2236); bind `abi_shape` into `proof_request_hash` for source-hash consistency. |
| R2-2 | MAJOR | A2 | hexdiff decoupling has no enforcement: hexdiff.py:126 imports `_gpr_fpr_masks` live; a frozen copy will silently drift. | A2 rev 3: duplicate `_register_fields`/`_gpr_fpr_masks` into hexdiff as `_classifier_*` with a pinned header comment, plus a **drift-detector test** (X-indexed RA-literal fixtures) in the test suite. No module-level flag (same module, one address). |
| R2-3 | MAJOR | all | I5 provenance fix is audit-hygiene only until targets.py:107-117 (relaxed hash checks) is re-tightened; `test_provenance.py` covers the list change automatically. | Note in sequencing: re-tightening the hash check is a **separate, out-of-scope** policy step; the list append still lands first. |
| R2-4 | MAJOR | A3 | The structural write-before-return check is a **forward** reachability — `_cfg_liveness` is backward and won't serve it; plan does not specify the pass. | A3 rev 3: small forward DFS/BFS over `_cfg_successors` from each write site, terminating at non-link `bclr`/`bcctr` returns; **per-function, not per-region** (a write in region 0 reaching a return in region 2 must still fix r4). |
| R2-5 | MAJOR | A1 | Test rewrite must remove BOTH `PSQ_L*` and `PSQ_ST*` from `REJECT_OPCODES` (or branch on byte-identity before the reject check) — exempting only PSQ_ST leaves the epilogue `psq_l` rejecting; add a byte-identical `psq_st`+`psq_l` combined accept case (the observed shape). | A1 rev 3: scope = all four PSQ forms (`PSQ_L/PSQ_ST/PSQ_LU/PSQ_STU`); tests cover combined prologue+epilogue accept, byte-identical single-form accept, non-byte-identical reject. |
| R2-6 | MAJOR | A1 | Without explicit `_use_def` psq entries, the `elif not uses_raw and not defs_raw:` fallback over-approximates **all 96 lanes** at every psq slot — safe but defeats the precise I3 liveness the design needs. | A1 rev 3: add explicit `_use_def` branches for PSQ forms (per F1/F9); the REJECT_OPCODES skip branch must not swallow them. |
| R2-7 | MINOR | A2 | Corrected `_register_fields` flows into `_rho_region_boundaries`/`_region_rho` automatically (all share the same call site). No gap. | — |
| R2-8 | MAJOR | A3 | Scope list omits `tools/coop/targets.json` — A3's payoff requires adding `declared_return` entries (source-data step). | A3 rev 3: A3's payoff is **deferred** to a follow-up while the G7 closure ships; targets.json edits are contentious on a shared branch. |
| R2-9 | NIT | A4 | Reproduced: `WPADiExcludeButton` fails `structural` (1 terminal pair, global rho, 10.8s); `__wudStackCheckDeviceInfo` fails `deadline` (30s). | A4 rev 3: confirmed. |
| R2-10 | MINOR | all | Provenance tests stay green after I5; the trust-boundary dirty flag will now fire on witness edits — intended. No test breakage. | — |

**Verdicts after round 2:** A2 SOUND as revised (needs F4/F5/R2-2); A1 NOT sound as
revised (F1 blocker + F6/F9/R2-5/R2-6); A3 NOT sound as revised (F2/F3 blockers +
F7/R2-1/R2-4); A4 SOUND (reproduced); A5 out of scope.

---

## A1. Byte-identical reject-list slot exemption (PSQ forms; 10-12 targets)

### Motivation (confirmed)

All reject-list failures are **byte-identical `psq_st` FP prologue saves** (same
raw word both sides, e.g. `0xf3e100d8`); most also carry byte-identical `psq_l`
epilogue restores that never gate because the witness rejects at the first psq
slot. The rest of each function differs only in GPR colors.

### Design (rev 3, per F1/F6/F9/R2-5/R2-6)

1. **Scope the exemption to the four PSQ memory forms** `PSQ_L`, `PSQ_ST`,
   `PSQ_LU`, `PSQ_STU` (all four leave `REJECT_OPCODES` — exempting only the
   store forms would leave epilogue `psq_l` rejecting, R2-5). All other reject
   opcodes (`mffs`, `mtfsf`, `dcbz`, privileged, PSQ X-forms) stay
   unconditionally rejected.
2. **Stream validation:** a PSQ slot is allowed iff `r_insn.raw == d_insn.raw`
   (byte-identical); otherwise the existing reject-list rejection stands.
3. **Field classification (F6):** PSQ fS/fD and rA are **non-register**
   (bit-equal, excluded from rho). Gate 3 enforces their bit-equality; the
   explicit `raw == raw` check is the exemption gate.
4. **Post-rho check (per region, belt-and-suspenders only):** retained per G2/F6
   — in `check_gates` and per-region in `run_region_sliced_witness` — but not
   load-bearing for soundness (the structural comparison and the liveness fix in
   (5) are).
5. **Liveness (I3/F1/R2-6):** add explicit `_use_def` branches for PSQ forms so
   the REJECT skip branch does not swallow them and the unknown-opcode fallback
   (all 96 lanes) does not fire:
   - `psq_st`/`psq_stu`: uses `{fS, ps1[fS], rA}` (rA guarded `ra != 0`);
     `psq_stu` also defs rA.
   - `psq_l`/`psq_lu`: defs `{fD, ps1[fD]}`; uses `{rA}` (guarded);
     `psq_lu` also defs rA.
   Missing `ps1[fS]` in the store use is a false-certificate hole (F1).
6. Execution proceeds through the existing `SymbolicOps` path; the structural
   terminal comparison remains the soundness backstop (G2).
### Soundness argument

Byte-identical slot ⇒ identical opcode/operands/memory address on both sides;
rho-fixed operands ⇒ both sides access the same shared variables, so outputs
(memory, lanes, FPSCR) are identical ASTs. A renamed operand writes different
shared variables; any escape (memory — compared unconditionally; a live lane —
compared; FPSCR — compared) diverges and rejects; a dead lane is genuinely
unobservable and masking it is correct. Consistency: `full-instruction-match`
already accepts byte-identical functions containing psq with no solver.

### Tests (rev 3)

- Rewrite `test_reject_list_opcodes_fall_back_to_smt`: `mffs`/`mtfsb0`/`mcrfs`/
  `dcbz` keep asserting `reject-list`; the byte-identical `psq_l` case moves to
  a new PSQ-exemption test group. Add non-byte-identical `psq_st` reject.
- Accept: GPR 2-cycle + **combined byte-identical `psq_st` prologue AND `psq_l`
  epilogue** (the observed shape, R2-5).
- Accept: byte-identical single `psq_st` (or `psq_l`) with identity rho.
- Reject: psq word differs (gate 6, unchanged).
- Reject: `psq_st f31` byte-identical with `ps1[31]` renamed across a region
  boundary whose deadness is masked by a missing ps1 use — the F1 regression
  (must reject via the new ps1 use).
- Reject: psq read at a region end with a cross-boundary rename (I3/F1
  liveness).
- Regression: region-sliced suite; `test_stmw_range_use_modeled` analog for psq.
## A2. Position-aware r0 (≈13 targets: 6 abi-boundary + 7 rho-region)

### The actual rule (rev 3, per G3/G4/I7/F8 — verified against the engine)

r0 is literal zero **only** where the engine guards `ra == 0`:
- D-form/DS-form **memory** ops: `lwz`/`lwzu`/`lbz`/`lh`/`lha`/`st` family +
  update forms, `lmw`/`stmw`, FP `lfs`/`lfd`/`stfs`/`stfd` + update forms,
  PSQ D-forms.
- X-form **indexed load/store** (integer and FP): `lwzx`/`stwx`/`lfsx`/… —
  `ops.const(0) if ra == 0` (semantics.py:3531/3559). **Not** a real register.
  (`dcbz`/`dcbzl` also guard RA=0 — semantics.py:3261 — but they are
  reject-listed, so they never reach the role table; F8 carve-out.)
- Arithmetic `addi`/`addis` (the `ra == 0 and op in (ADDI, ADDIS)` guard,
  semantics.py:3296).

r0 is a **real register** (reads `gpr[ra]`) everywhere else, including:
- `cmpwi`/`cmplwi`/`cmpw`/`cmplw` (semantics.py:3455),
- `mulli`/`addic`/`addic.`/`subfic` (no guard outside ADDI/ADDIS),
- X-form arithmetic (`add`/`subf`/`and`/`or`/…), M-form (`rlwimi`/`rlwinm`),
  RD/RB/RS positions, and X-form load/store **third** operand (RB).

### Design (rev 3)

1. **Role table:** the RA field is `non-register` (bit-equal, excluded from
   rho) for exactly the memory-form set above (D/DS/X-indexed load-store).
   `_register_fields` returns RA as non-renameable for those opcodes.
2. **Rho builder (gate 4):** unchanged mechanics; r0 accumulates normally from
   every other position.
3. **Gate 5:** remove r0 from `_UNCONDITIONALLY_FIXED_GPRS`; add r0 to
   `_VOLATILE_GPRS` (G5).
4. **Liveness (G6/F5):** mirror the engine's `if a[1]:` RA-read guard in
   `_use_def` for load/store forms **and** the ADDI/ADDIS group (the engine's
   literal-zero guard covers both; semantics.py:3296). Safe direction, needed
   for region-rebind completeness.
5. **hexdiff decoupling (I6/R2-2):** duplicate `_register_fields` +
   `_gpr_fpr_masks` into hexdiff as `_classifier_register_fields` /
   `_classifier_gpr_fpr_masks` with a pinned header comment ("classifier-only
   frozen copy of the pre-A2 witness role table; do NOT update when the witness
   table changes — they intentionally diverge after A2, doc 32 §A2"), plus a
   **drift-detector test** pinning the frozen table against the A2 reject
   fixtures (X-indexed RA-literal pairs) so an accidental re-sync fails CI. No
   module-level flag (both callers import the same module at one address).
6. **Region path:** the corrected `_register_fields` applies automatically to
   `_rho_region_boundaries`/`_region_rho`.

### Soundness argument

The only r0-literal interpretation in the ISA is guarded by `ra == 0` in the
engine for exactly the memory + ADDI/ADDIS set. Gate 3 requires bit-equality of
the now-non-register RA fields, so a certified pair can never differ in whether
r0 is literal; everywhere else r0 is an ordinary register renamed under the
shared-state binding. No false certificate path: any misclassification only
shifts a divergence into a compared lane (reject), never past one (G3 showed
the accept-test pair is rejected, not certified).

### Tests (rev 3)

- Accept: `li r0,97; lwz r6,552(r7); stb r0,..; ori r6,r6,0xF; …` vs the
  r0/r6-swapped version (GXCopyDisp body).
- Accept: X-form arithmetic RA rename `add r3,r0,r5` vs `add r3,r6,r5` under
  rho(0)=6. Accept: `cmpwi cr0,r0,5` vs `cmpwi cr0,r6,5` under rho(0)=6.
- Reject: `addi r3,0,5` vs `addi r3,r12,5` — **gate `fields`** (F4: the
  `test_cx2_zero_register_encoding_rejected` assertion is rewritten from
  `abi-boundary` to `fields`; the CX-2 rationale becomes "D-form RA is
  bit-equal", not "rho must fix r0").
- Reject: `lwz r3,0(r0)` vs `lwz r3,0(r12)`; `lwzx r3,r0,r5` vs `lwzx r3,r6,r5`
  (X-indexed RA literal — no longer an accept case, G3).
- Regression: updated CX-2 test; the X-indexed-RA reject fixtures double as the
  hexdiff drift-detector fixtures (R2-2).
## A3. Structural return-register fixedness for r4/f1 (payoff deferred; G7 closure ships)

### Design (rev 3, per F2/F3/F7/R2-1/R2-4/R2-8)

1. **r3 stays unconditionally fixed.**
2. **r4 and f1 default to FIXED.** They are unfixed ONLY when BOTH hold:
   (a) no live-in in the EABI arg range and no live-across-call (existing
   gate-5 computations), AND
   (b) **trusted metadata:** a registry `declared_return` whose raw string is
   provably not 64-bit/aggregate (∈ {void, i32, u32, f32, f64, bool, ptr} per
   I13 — the narrowed `AbiShape` is NOT consulted; `aggregate`→None).
   The structural check (c) below can only ever **fix** r4, never unfix it
   (F2/F3):
   (c) **structurally: r4/f1 is written on any forward-CFG path reaching a
   non-link `bclr`/`bcctr` exit** — a new per-function forward DFS/BFS over the
   existing `_cfg_successors` (R2-4; NOT per-region: a write in region 0
   reaching a return in region 2 still fixes r4). `b` tail-call exits are
   covered by the default-FIXED rule (F7), not by (c).
3. **Payoff honesty (F2/F3/R2-8):** the I2 claim that `__wudSyncDone`-style
   targets certify "without any registry change" is **retracted** — (c) fixes
   r4 for them because r4 is written before a return. Certifying them requires
   adding `declared_return` entries to `tools/coop/targets.json` (a source-data
   step, deferred this round; see Scope). A3 rev 3 ships the **G7 closure**
   (soundness hardening) now; the certification payoff is a follow-up.
4. **Certificate plumbing (I4/R2-1):** add a registry `declared_return` lookup
   to `_try_renaming_witness` (mirroring the SMT path's non-gated lookup at
   equivalence_check.py:2224-2236), pass `abi_shape={"declared_return": raw}`
   as an **explicit kwarg** to `_build_equivalence_certificate` (it already
   accepts `abi_shape` at :906 and writes it at :961-962 — the §2.5.1
   `contract_resolution` embedder is dead for the witness path), and bind
   `abi_shape` into `proof_request_hash` at :1513. Absent a registry value,
   `abi_shape` stays None (both sides absent ⇒ validator passes).
5. **Provenance (I5/R2-3):** `renaming_witness.py` added to
   `CERTIFIER_SOURCE_PATHS` before any semantic change; re-tightening the
   targets.py:107-117 hash checks is a separate out-of-scope policy step.

### Soundness argument

A return-register rename is observable by the caller; gate 5 is the **only**
protection because the terminal comparison is rho-aligned and self-agrees on a
renamed return lane (G7). Default-FIXED pins r4/f1 unless trusted metadata says
the caller cannot observe them (non-64-bit return) AND the body never writes
them on a return path ((c), which only over-fixes). Every escape (live-in
argument, live-across-call value, write-before-return, `b` tail-call argument)
is covered by (a)/(b)/(c)/default-FIXED respectively. The structural comparison
remains the backstop for every other lane.

### Tests (rev 3)

- Reject (G7 regression): `long long` return (r4 written before `blr`) with ANY
  `declared_return` — default-FIXED + (c) fix r4 (F3: the `__wudSyncDone`
  label belongs here, not in accept).
- Accept (synthetic, F3): r4 never written on any return path (e.g. written
  and consumed between calls, function ends via `b` tail-call of a callee that
  does not observe r4 as an arg), registry `declared_return="i32"` → r4
  unfixed, cert issues, `abi_shape` present, §2.5.4 validates.
- Accept (default-FIXED + metadata absent): same synthetic body with NO
  registry `declared_return` → r4 stays fixed → a rho that renames r4 rejects
  at gate 5 (no cert, no false accept).
- Reject: `b` tail-call with r4 live-out as a callee argument, registry i32
  present — default-FIXED still fixes r4 (F7 regression).
- Reject: r4 live-in renamed; r4 live-across-call renamed.
- Regression: `test_cx1_shift_count_swap_rejected` (r4/r5 ABI args) stays green.

## A4. Resource and diagnosis (re-measured, rev 3 per I8/R2-9)

- `__wudStackCheckDeviceInfo` (us-8037a710): reproduces the ~30s
  `cfg-exploration` deadline at production budgets (`max_paths=4096,
  deadline_ms=30000`, 30.0s, `pairs=0`, `mode=None`) — a real resource case.
  Give the witness a comparable `deadline_ms` (machine time, not soundness).
- `WPADiExcludeButton` (us-8036fb80): **confirmed misdiagnosed in rev 1** — at
  production budgets it fails `structural` ("terminal pair (return, return)
  diverges structurally", `pairs=1`, `mode=None` (global rho), 10.8s). The
  pair diverges under a single global rho with ONE terminal pair — a genuine
  code difference or global-rho gap, not a resource case. Investigate before
  touching any budget.
- Do not raise `max_paths` for the SMT probe; witness-only budget changes.

## A5. Gate-2 reloc value-equality (out of scope this round; framing corrected)

Parallel reloc-fix pipeline result (2026-08-03): refreshing
`retail_reloc_map.json` (1901 entries) made gate 2 pass for TU-local `@N`↔`@M`
drift via the canonical-symbols hook — most rev-1 reloc-gate failures were a
stale-map artifact. The remaining reloc-blocked cases are the §7i group
(`double_8066BE60` vs `@1000`, i2f magic-double pool, reloc-presence differs).
If revisited: the authority is an **address-resolution check** (both reloc
slots resolve to the same target address, re-verifiable from the two objects'
symbol tables at certification time), **not** bit-equality of displaced fields —
gate 3 skips the raw-bit compare entirely when a reloc is present (I10).

---

## Sequencing, scope, and acceptance (rev 3)

1. **Provenance first (I5/R2-3/R2-10):** append `renaming_witness.py` to
   `CERTIFIER_SOURCE_PATHS` (one line, zero risk, `test_provenance.py` covers
   it). Note: audit-hygiene until the targets.py:107-117 hash checks are
   re-tightened (separate out-of-scope policy step).
2. **A2** (corrected r0 set per G3/G4/F4/F5/F8 + hexdiff decoupling per
   I6/R2-2) → tests → re-sweep the 13 r0 targets on `cycle` (no `--smt`).
3. **A1** (PSQ-only scope, non-register fS/rA, explicit `_use_def` psq entries
   incl. `ps1[fS]`, test rewrite, per-region belt-and-suspenders post-rho) →
   tests → re-sweep the ~12 psq targets.
4. **A3 plumbing + structural check** (R2-1 explicit `abi_shape` kwarg +
   registry lookup; R2-4 forward CFG pass, per-function) → tests → G7
   regression green. Payoff (r4-scratch certification) is **deferred** — it
   requires `tools/coop/targets.json` `declared_return` entries (R2-8).
5. **A4 diagnosis** — `WPADiExcludeButton` structural divergence
   (investigate, do not raise caps); `__wudStackCheckDeviceInfo` witness-only
   `deadline_ms` bump. Measure per wave: how many of the 93 previously-failing
   targets now certify (`cycle` verdict `EQUIVALENT_MATCH` without `--smt`,
   split-size fit).

In scope: `tools/coop/lib/renaming_witness.py` (incl. explicit `_use_def` PSQ
branches), `tools/coop/lib/equivalence_check.py` (abi_shape kwarg + registry
lookup + proof_request_hash), `tools/ppc_equivalence/provenance.py`
(CERTIFIER_SOURCE_PATHS), `tools/coop/tests/test_renaming_witness.py`
(rewrites + new, incl. hexdiff drift-detector fixtures),
`tools/coop/hexdiff.py` (frozen `_classifier_*` copy), `SOUNDNESS.md` via
`docs_sync`, and this doc. **Explicitly deferred:** `tools/coop/targets.json`
`declared_return` entries (A3 payoff, source-data step on a shared branch),
re-tightening the provenance hash checks. **Out of scope:** loop second-cut,
value-splitting for the 16 non-r0 rho failures, the 3 `fields` failures
(source), and the reloc-fix pipeline's files (no file overlap: A2's hexdiff
touch is `_pure_reg_swap`, the reloc agent's is the drift-suggestion block —
disjoint).

## Files to read

- `tools/coop/lib/renaming_witness.py` — gates 1–6, rho builder, region path.
- `docs/ppc_equiv_work/31-reg-swap-witness.md` — original spec + two prior
  review rounds.
- `docs/witness_expansion_plan.md` — region-sliced rho, four-lane deadness.
- `tools/ppc_equivalence/semantics.py` — rA=0 guards (3296/3455/3531/3559),
  psq/mffs/dcbz semantics (2007/4152/3247), call_token (1549).
- `tools/ppc_equivalence/abi_infer.py` — `returns_i64` inference and narrowing.
- `tools/ppc_equivalence/provenance.py` — `CERTIFIER_SOURCE_PATHS`.
- `tools/coop/tests/test_renaming_witness.py` — regression corpus (I1/F4 rewrites).
- `tools/ppc_equivalence/semantics.py` — PSQ pair (1967/2007/3860-3915),
  r0 guards (3296/3531/3559/3874/3877/4498).
