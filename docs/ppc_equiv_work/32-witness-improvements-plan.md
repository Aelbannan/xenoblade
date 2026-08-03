# 32 — Witness improvements plan: certifying the remaining pure-reg-swap RVL pairs

Status: **draft for adversarial review** (GLM-5.2 soundness, Kimi K3 integration).
Companion to `31-reg-swap-witness.md` (the register-renaming witness) and
`docs/witness_expansion_plan.md` (region-sliced rho).

---

## 0. Problem statement

The RVL targets were swept through the current witness
(`tools/coop/lib/renaming_witness.py`, `certify_renaming_witness`). Of 200
non-FULL_MATCH RVL targets, 94 are **pure register-swap pairs** (hexdiff:
0 structural, >0 reg-swap) — exactly the witness's target class. Only **1 of
the 94 certifies today** (`__HBMSYNResetAllControllers`). The other 93 fail
at these gates:

| Gate | Count | Root cause |
|---|---|---|
| reloc (gate 2) | 28 | reloc symbol names differ (mostly TU-local `@N` labels / `...data.0`; a separate fix pipeline — `reloc_map.py` + `extern "C" lbl_*` — is running in parallel; ~17 are "do-not-chase" name drift with equal values) |
| rho (gate 4, incl. region path) | 23 | 7 are r0-involved (see A2); 16 are genuinely non-bijective local allocations |
| abi-boundary (gate 5) | 14 | 6 `r0→rX`, 7 `r3/r4→rX` (see A2/A3), 1 real boundary-deadness failure |
| loop (first-cut policy) | 12 | any backward branch → SMT |
| reject-list (gate 6) | 10 | **all are byte-identical `psq_st`/`psq_l` FP prologue/epilogue saves** (same raw word both sides; see A1) |
| fields (gate 3) | 3 | genuine non-register bit differences (source fixes, not witness) |
| structural | 1 | `bta_dm_sdp_result` diverges under region rho |
| execute / deadline | 2 | `WPADiExcludeButton` path-limit 256; `__wudStackCheckDeviceInfo` 20s deadline (see A4) |
| total | 93 | |

This plan proposes five witness-side changes. Sound improvements A1–A3 target
~25–30 of the 93; A4 is resource plumbing; A5 (reloc value-equality) is gated
on the parallel reloc-fix pipeline's report.

All five changes must preserve the witness's two soundness invariants:
1. **Shared-state symbolic execution** — retail `r_i` and decomp `r_rho(i)`
   share one variable; divergence always degrades to SMT, never certifies.
2. **Full-state structural terminal comparison** — every architectural
   component is compared; a divergence in any compared lane rejects.

---

## A1. Byte-identical reject-list slot exemption (10 targets)

### Motivation

All 10 reject-list failures are the **same pattern**: retail and decomp both
emit an identical `psq_st f31, 0xXX(r1), 1, qr3` (FP prologue save) / `psq_l`
(epilogue restore) at the same slot — same raw word, e.g. `0xf3e10078` vs
`0xf3e10078`. The rest of the function differs only in GPR colors. Gate 6
currently rejects the pair the moment it sees `psq_*` anywhere, even when the
slot cannot diverge.

### Design

- **Stream validation (gate 6):** a reject-list opcode slot is allowed iff
  `r_insn.raw == d_insn.raw` (byte-identical) **and** every GPR/FPR field of
  that slot is **rho-fixed** (identity-mapped). Otherwise the existing
  reject-list rejection stands (fall through to SMT).
- Because rho is built after stream validation, the check is split:
  - stream validation rejects reject-list slots that are *not* byte-identical
    (unchanged behaviour);
  - a new post-rho check rejects the pair when a byte-identical reject-list
    slot's register fields are renamed by rho.
- Execution proceeds through the existing `SymbolicOps` path for the
  exempted slot (the engine's psq/FPSCR semantics are used as-is — same as the
  `full-instruction-match` path, which accepts byte-identical functions
  containing these opcodes with **no** semantic proof at all).

### Soundness argument

The reject-list exists because *renaming through* these opcodes is unsound:
psq/FPSCR semantics are capability-stubbed, and a renamed operand would let a
stub mask a real divergence. The exemption closes that hole structurally:

- **Byte-identical slot ⇒ identical opcode and identical operand register
  numbers.** Both sides execute the same instruction on the same memory
  address.
- **rho-fixed operands ⇒ the slot reads/writes the same shared variables on
  both sides.** Under the shared-state binding, decomp register `j` holds
  retail's `rho^{-1}(j)` variable. A slot whose every GPR/FPR field `j` has
  `rho(j)=j` accesses `X_j` on both sides, so its inputs are the same ASTs and
  its outputs (registers, memory, FPSCR) are the same ASTs.
- If rho renames any operand register, the slot writes different shared
  variables; any *escape* (memory write, compared lane, FPSCR, LR) diverges and
  the structural comparison rejects. The only masked component is a dead
  register lane (region path); a dead lane cannot escape to memory because the
  slot's memory writes are compared unconditionally and any divergence there
  rejects. (Edge: an exempted slot that *writes only* a dead lane — e.g. a
  dead `psq_l` — writes memory too when the operand is rho-fixed; when it is
  not rho-fixed the exemption does not apply.)
- **Consistency with existing acceptance:** `full-instruction-match` certifies
  byte-identical functions containing `psq_*` with no solver. The exemption
  certifies pairs whose non-psq slots differ only in rho-fixed-safe colors —
  strictly more evidence than the byte-identical path requires, and the same
  semantics the byte-identical path trusts.

### Tests

- Accept: 2-cycle GPR swap + byte-identical `psq_st`/`psq_l` prologue save
  (the observed pattern).
- Reject: same pair with a *renamed* psq operand (e.g. `psq_st f31` retail vs
  `psq_st f30` decomp — bytes differ → reject-list; and `psq_st f31` both
  sides with rho(31)=30 elsewhere → post-rho rejection).
- Reject: `mffs`/`dcbz` pairs that are byte-identical but whose *operands*
  (f0 / none) cannot be rho-fixed under a renaming rho — confirm fail-closed.
- Regression: the existing `test_reject_list_opcodes_fall_back_to_smt` must
  keep passing (non-identical reject-list slots still reject).

---

## A2. Position-aware r0 (≈13 targets: 6 abi-boundary + 7 rho-region)

### Motivation

Retail code routinely uses **r0 as a real value register**: `li r0,97;
... stb r0,...` and 2-cycles like `{r0→r6, r6→r0}` (verified in
`GXCopyDisp`, `__wpadGetExtConfig`). Gate 5's `_UNCONDITIONALLY_FIXED_GPRS`
contains r0, so any rho touching r0 is rejected — and because the rho is
built before gate 5, the *region path* reports these as rho conflicts
("no consistent bijection in region [4,11)") even though the region rho
`{r0↔r6}` is a perfectly consistent bijection.

### The actual rule

On PPC, r0 is only special in the **RA field of D-form (and DS-form)
instructions** (`addi`, `lwz`, `stw`, `cmpwi`, `li`/`lis` encodings, …):
there `RA=r0` means the literal zero and the instruction reads no register.
In every other position — RD, RB, and **X-form RA** (`lwzx r3,r0,r5` reads
r0) — r0 is an ordinary register.

### Design

1. **Role table:** for D-form/DS-form opcodes, mark the RA field
   `non-register` (bit-equal required, excluded from rho accumulation). Add a
   `_D_FORM_RA_OPCODES` set; extend `_register_fields` so those opcodes return
   RA as non-renameable (the `(start, kind)` tuple gains a `FIXED` marker, or
   the rho builder consults the set directly).
2. **Rho builder (gate 4):** r0 accumulates normally from RD/RB and X-form RA.
3. **Gate 5:** remove r0 from `_UNCONDITIONALLY_FIXED_GPRS`. The D-form RA
   bit-equality (gate 3) already guarantees no instruction can misread decomp's
   r0 as a literal.
4. **Region path:** no change needed beyond the above — the r0↔r6 2-cycle
   regions then pass `_region_rho` + per-region gate 5.

### Soundness argument

The only semantic specialness of r0 is the D-form RA literal-zero encoding.
Gate 3 requires every non-register bit to be equal, and under this change the
D-form RA field *is* non-register: any pair where retail writes `RA=0` and
decomp writes `RA=rX` fails gate 3 (fields), and vice versa. Therefore no
instruction in a certified pair can differ in whether r0 is interpreted as
zero. Everywhere else, r0 is a value register and is renamed exactly like any
other GPR under the shared-state binding. X-form RA r0 reads are renamed
consistently because both sides' X-form RA fields are renameable GPR fields
participating in the same rho.

### Tests

- Accept: `li r0,97; lwz r6,552(r7); stb r0,..; ori r6,r6,0xF; stw r6,..;
  lwz r6,544(r7); stb r0,..; rlwinm r0,r6,0,0,29` vs the r0/r6-swapped
  version (the `GXCopyDisp` body) — global rho or region-sliced, whichever.
- Accept: `lwzx r3, r0, r5` vs `lwzx r3, r6, r5` under rho(0)=6 (X-form RA).
- Reject: `addi r3, 0, 5` vs `addi r3, r12, 5` (CX-2 must keep failing —
  D-form RA bit differs → gate 3).
- Reject: `lwz r3, 0(r0)` vs `lwz r3, 0(r12)` (D-form RA rename attempt).
- Regression: `test_cx2_zero_register_encoding_rejected` stays green.

---

## A3. Live-out/live-in-based r3/r4 fixedness (≈7 targets)

### Motivation

`_UNCONDITIONALLY_FIXED_GPRS = {0,1,2,3,4,13}` fixes r3/r4 even when the
function never reads them before writing (not live-in) and never returns
through them. Verified example: `__wudSyncDone` renames dead-scratch
`r4→r30` (`add r4,r29,r30` / `add r30,r29,r31` — written and consumed between
calls). r4 is a volatile scratch register there; the rename is sound.

### Design

- **r3 stays unconditionally fixed** (universal return register; only ~2 of
  the 14 abi-boundary failures involve r3 — conservative and cheap).
- **r4 (and f1) become conditionally fixed:** fixed iff
  (a) live-in in the EABI arg range (already computed), or
  (b) live-across-call (already computed), or
  (c) the function returns 64-bit/aggregate — the pipeline's `declared_return`
  already knows this (`abi_infer.py`); thread it into
  `certify_renaming_witness` as an optional parameter (default: fixed, i.e.
  current behaviour when unknown).
- Note the current CFG fixpoint models **no exit live-out** (terminals have no
  successors, so `live_out=∅` at exits); `declared_return` is the source of
  truth for the return set rather than re-engineering exit liveness.

### Soundness argument

An r4 rename is unsound only if the function's caller observes r4: i.e. r4 is
live-in (argument), live-out (return), or preserved across a call boundary in
a way the callee contract captures. Conditions (a)–(c) cover exactly those;
otherwise r4 is dead at entry, dead at every exit, and its post-call value is
a volatile clobber — renaming it changes nothing observable to any caller.
The structural terminal comparison remains the backstop: any lane the
comparison checks that diverges rejects.

### Tests

- Accept: `__wudSyncDone`-shape pair (r4 scratch, dead at entry/exit/calls,
  u32/void return) with `declared_return` set.
- Reject: same shape with `declared_return` = 64-bit (r4 is a return register).
- Reject: r4 live-in (read before write) renamed.
- Reject: r4 live across a call renamed (existing test
  `test_volatile_live_across_call_must_be_fixed` analog for r4).
- Regression: `test_cx1_shift_count_swap_rejected` (r4/r5 ABI args) stays
  green — r4 remains fixed when live-in.

---

## A4. Resource knobs + `WPADiExcludeButton` path blow-up (2 targets)

- `__wudStackCheckDeviceInfo` (7 calls, 18 regsw): witness hit the 20s
  `cfg-exploration` deadline in the sweep. The pipeline default
  (`prove_unit_symbol`) should be confirmed; if it is the same 20s, raise the
  witness budget to match the SMT probe budget — this is machine time, not
  soundness.
- `WPADiExcludeButton` (2 calls, 17 regsw): `ExecutionInconclusive: path limit
  exceeded (256)`. 256 paths for a small function is suspicious — **investigate
  the region driver's cross-region path accounting before raising the cap**
  (doc-31 round-2 fixed a double-counting bug in the same area). If it is a
  genuine accumulation bug, fix it; if it is a real blow-up, raise
  `max_paths` for the witness only (never the SMT probe).

---

## A5. Gate-2 reloc value-equality (up to ~17 targets, gated)

Held: the parallel reloc-fix pipeline
(`reloc_map.py` + `extern "C" lbl_*` declarations) is running on the 28 reloc
failures. Once it reports, the "do-not-chase" remainder (TU-local `@N` vs
`@M` labels whose *values* match) can only be certified if gate 2 accepts
relocs proven to resolve to the same address.

Design sketch (only if the report justifies it): gate 2 currently requires
`canonical_symbol` equality. Extend it to accept a pair when the mined map
(`tools/coop/retail_reloc_map.json`) or an address-resolution check proves
`retail_symbol + retail_addend == decomp_symbol + decomp_addend` (same target
address, different names). Soundness precondition: both reloc slots must have
**identical instruction bytes** (the displaced bits match) — the only
difference is the symbol name, which is not part of the executed semantics.
The mined map is evidence, not the sole authority: a value-equality decision
must be re-verifiable from the two objects' symbol tables at certification
time, and `data_value`-accepted pairs elsewhere in the repo (MWCC_REFERENCE §1)
are the precedent. This change is **out of scope for this review round**;
revisit with the pipeline's final per-function report.

---

## Sequencing, scope, and acceptance

1. Implement A1 → tests → `python -m unittest discover -s tools/coop/tests`
   → re-sweep the 10 reject-list targets on `cycle` (no `--smt`).
2. Implement A2 → tests → re-sweep the 13 r0 targets.
3. Implement A3 → tests → re-sweep the 7 r3/r4 targets.
4. A4 as investigation permits.
5. Measure per wave: how many of the 93 previously-failing targets now certify
   (`cycle` verdict `EQUIVALENT_MATCH` without `--smt`, split-size fit).

In scope: `tools/coop/lib/renaming_witness.py`,
`tools/coop/lib/equivalence_check.py` (threading `declared_return`), the
`test_renaming_witness.py` suite, `SOUNDNESS.md` via `docs_sync`, and this
doc. **Out of scope:** loop second-cut (fixpoint-enabled loops), value-splitting
region boundaries for the 16 non-r0 rho failures, source-level matching of the
3 `fields` failures, and anything touching the reloc-fix pipeline's files.

## Files to read

- `tools/coop/lib/renaming_witness.py` — gates 1–6, rho builder, region path.
- `docs/ppc_equiv_work/31-reg-swap-witness.md` — original spec + two
  adversarial review rounds (GLM-5.2/Kimi K3).
- `docs/witness_expansion_plan.md` — region-sliced rho, four-lane deadness.
- `tools/ppc_equivalence/abi_infer.py` — `declared_return` availability.
- `tools/coop/tests/test_renaming_witness.py` — existing regression corpus.
