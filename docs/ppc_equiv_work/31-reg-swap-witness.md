# 31 — Register-renaming witness: fast-path equivalence certificates

Status: **design revised after adversarial review** (GLM-5.2 soundness review, Kimi K3
integration review; both verified against code). **Third adversarial review** (2026-08-04,
GLM-5.2 + Kimi K3 via pi/OpenRouter, both AGREE) found and closed three false-certificate
holes and two doc/reach gaps — see §6. **Implemented**: the pre-SMT
register-renaming witness (evidence `register-renaming-witness`) ships in
`tools/coop/lib/renaming_witness.py`, wired as a pre-SMT filter in
`prove_unit_symbol`/`certify_unit_symbol`, with the evidence whitelist,
certificate plumbing, hexdiff role-table refinement, SOUNDNESS.md theorem
extension, and the test suite in
`tools/coop/tests/test_renaming_witness.py`.

Predecessor: the "reg-swap-only fast path" plan. This doc is the corrected,
implementable spec. It supersedes the original plan's wording wherever they differ.

---

## 0. Problem statement

Proving equivalence for near-identical function pairs (instruction streams
position-aligned, same mnemonics per slot, only register colors differ — MWCC
register-allocation soft-caps) uses the full Z3 CFG-exploration path and times
out / errors on exactly these pairs. 192 working targets sit at ≥99% instruction
match (173 CODE_MATCH, 19 EQUIVALENT_MATCH). The plan's headline case
`func_80462068` is NOT_STARTED with zero attempt records — do not cite it as
evidence in commit messages or docs.

Data (Kimi K3 audit of attempts.jsonl on the ≥99% set):
- `inconclusive_unvalidated_callee` dominates (551 attempts) — NOT solver difficulty.
- Genuine solver timeouts on this set: ~16/668 (~2.4%). 609 of 657 global
  `internal_error` records are "z3-solver is required" environment artifacts.
- **Realistic payoff: ~10–40 certificates**, not 187. 57 of the 192 are leaves
  (no called functions) where the callee gate is moot; scheduling/immediate/frame
  skew, indirect calls (43), jump tables, and size drift subtract the rest.

## 1. Soundness contract (GLM review — the core finding)

The original plan's step 4 ("interpret both streams on symbolic registers with
rho applied as a rewrite; identical layout ⇒ no solver") describes **operand-
position rewriting**, which is UNSOUND. Refuted by concrete counterexamples that
pass gates 1–3 as originally written:

- **CX-1** `slw r3,r4,r5` vs `slw r3,r5,r4` — shift-count swap; ρ={r4↔r5} is a
  valid involution, both scratch ⇒ gates pass, but `shl(A,B) ≠ shl(B,A)`.
- **CX-2** `rA=0` zero-register encoding — `addi r3,0,5` is literal 5, not r0+5.
  ρ mapping r0→r12 turns it into `addi rD,r12,5` (reads a register retail never
  reads). Gate 3 omitted r0 from its fixed set.
- **CX-3** `mfspr r3,GQR0` vs `mfspr r3,GQR1` — SPR index (bits 11–20) is an
  immediate, but the hexdiff classifier records it as a register rename. GQRs
  are user-mode; the engine's privileged fail-closed path does not cover them.
- **CX-4** `cmpw cr1,r5,r6` vs `cror cr1,cr1,cr1` — BF/BA/BB/BT are CR-bit
  *immediates*, conflated with GPR r0 by the classifier's `_rd/_ra/_rb`.

**The sound formulation (mandatory):** *symbolic-variable substitution on the
initial state*, not operand rewriting.

1. Bind the initial machine state so `retail_r_i` and `decomp_r_ρ(i)` share one
   symbolic variable (an `initial_gpr_bindings`-style mechanism; see
   `object_base.py` for the existing pattern).
2. Execute **both** sides through the existing audited
   `SymbolicOps`/`execute_cfg` (semantics.py) — same semantics that already
   handle rA=0, shift counts, CR fields, SPR indices, relocs, calls. **Do not
   write a new interpreter.**
3. Compare terminal symbolic state with **structural `z3.eq`** (the engine
   already uses `z3.eq` as a fast path at engine.py:166/184/313; extend it to
   the terminal observable comparison, or a straight-line variant). Identical
   trees ⇒ identical values; any divergence ⇒ `z3.eq` False ⇒ fall through to
   SMT. Incompleteness degrades to SMT — never a false certificate.

### Required gates (in order)

1. **Size equality** — same instruction count.
2. **Per-slot reloc equality** — offset/type/symbol/addend must all match
   (relocs are addresses, not colors). Reject on any difference.
3. **Non-register field equality** — immediates, branch displacements, Rc/OE
   bits, CR/FPR/ps operands, SPR indices, FXM masks must be bit-equal. Requires
   a **per-instruction operand-role table**: each 5-bit field is one of
   `{gpr, fpr, cr_bit, cr_field_bf, spr_index, fxm_mask, immediate}`. Only
   `{gpr, fpr}` fields enter ρ; everything else must be equal.
4. **ρ bijection** — single-valued in both directions, consistent across all
   mnemonics/positions. (hexdiff's `reg_map` is keyed by `(mnem, position)` and
   allows many-to-one; do NOT use it as ρ. Build ρ fresh from the role table.)
5. **ABI-boundary fixedness** — ρ must fix: r0 (zero-register encoding), r1, r2,
   r13, LR, CTR (if used), all argument/return registers, and every **volatile**
   register live across a call (opaque-EABI clobber set: r3–r12, f0–f13,
   cr0/cr1/cr5–cr7, xer, ctr, lr, fpscr). Nonvolatile permutations across calls
   (e.g. r20↔r25, both preserved by EABI) are SOUND and must NOT be pre-rejected
   — that is exactly the Chaitin-cycle class this feature exists for.
6. **Reject-list (fall straight to SMT, never certify via renaming):**
   `ps_*`, `psq_*`, `mtfsf`/`mffs`/`mcrfs`/`mtfsb*`/`mtfsfi`, `mtspr`/`mfspr`
   to GQRs (912–919) or any non-{LR,CTR,XER} SPR, `dcbz`/`icbi`/`tlb*`, and
   anything the engine marks PRIVILEGED_INSTRUCTION or INCONCLUSIVE_UNSUPPORTED.
   The witness certifies only integer + load/store + branch + cr-logical over
   the role-cleaned operand set.

### Certificate honesty

- Evidence label: `register-renaming-witness` (new; see §2 consumers).
- Record `opcodes_used`, the ρ map, and the structural-eq result in the cert.
- **Do not** set `observables=[]` (the byte-identical path can because
  `bytes_identical` is trivially sufficient; the renaming path is not).
- Callee summary: same conservative opaque-EABI envelope as byte-identical
  (`skip_semantic_validation`-style), but the flag's precondition is documented
  as byte-identity — add a **separate parameter** with its own justification
  comment rather than silently reusing the flag.

## 2. Integration site (Kimi review — the other core finding)

- **Where the witness goes:** a **pre-SMT filter in the prove path**
  (`prove_unit_symbol`), BEFORE the memory_bus SMT attempt. `certify_unit_symbol`
  is only reached at 100% match (objdiff_report.py:224–232); 99.x% pairs go to
  `prove_unit_symbol`. There is no "fall through to SMT" inside
  `certify_unit_symbol` — on witness failure the SMT probe must still run. That
  is a real control-flow change; the witness must run before any solver work
  (also before the `except Exception: pass` SMT-first block at
  equivalence_check.py:2371–2387, which would otherwise eat the timeout the fast
  path exists to avoid).
- **Evidence whitelist:** `tools/coop/lib/targets.py:103–104` only accepts
  `{symbolic-equivalence, full-instruction-match}`; a renaming cert would be
  rejected as "certificate evidence is not recognized" until the whitelist is
  extended. Find and update every consumer (policy, assurance, promotion,
  ledgers, docs_sync, tests).
- **Provenance:** `proof_request_hash`/`contract` are hardcoded to
  `"full-instruction-match"` (equivalence_check.py:2435, 2455) regardless of
  evidence; thread `contract="register-renaming-witness"` through so source_hash
  and the audit trail are honest.
- **Callee staleness:** renaming certs use the same `_build_equivalence_certificate`
  (callees, summary_sha256) ⇒ bottom-up recertify works unchanged. But these
  targets are CODE_MATCH (not ACCEPTED), so `targets recertify` never touches
  them — certification happens in the cycle/harness path, and the new evidence
  must be accepted there too.

## 3. Scope and sequencing

- New module: `tools/coop/lib/renaming_witness.py` (role table, ρ builder,
  witness check, reject-list) + `tools/coop/tests/test_renaming_witness.py`.
- Modified: `tools/coop/lib/equivalence_check.py`, `tools/coop/lib/targets.py`,
  `tools/coop/lib/equivalence_policy.py`, `tools/coop/hexdiff.py` (role table in
  the classifier — additive to the uncommitted reloc-drift/split-budget changes
  already in that file from another agent; do not disturb those), `engine.py`
  (narrow: `_symbolic_initial` binding + terminal `z3.eq`), `SOUNDNESS.md`
  (theorem extension, via docs_sync), tests.
- CI gates: `gen_fixture_blob.py --check`, full `tools/ppc_equivalence/tests`
  suite (currently 1972 tests), `docs_sync --check`.
- Payoff target: certify a first wave of leaf CODE_MATCH ≥99% targets end-to-end
  (prove path → certificate → EQUIVALENT_MATCH), then report the measured count.

## 4. Working-tree constraints

- `tools/coop/hexdiff.py` currently has uncommitted split-budget + reloc-drift
  changes from another (idle) agent. Do not revert, commit, or restructure
  them; add the operand-role table as a separate, additive change.
- Read-only review of docs; commit only the files this feature touches.
- Use `tools/coop/hexdiff.py` (or `run.py harness`) for verification; hexdiff
  holds the repo-wide build lock and is safe for concurrent agents. Do not run
  `ninja`/`configure.py` directly except when hexdiff cannot express the op.

---

## 5. Expansion B: region-sliced (position-dependent) rho — soundness theorem

**Status: implemented** (witness_expansion_plan rev 5, 2026-08-03). The global
single-bijection witness rejects pairs whose register correspondence is not one
consistent bijection across the whole function — MWCC's *local*
register-allocation differences (a temp lives in r5 in one region and r4 in
another). Expansion B splits the stream into regions with a consistent bijection
each, rebinds the shared symbolic variables at region boundaries, and compares
each terminal under its exit region's rho.

### Soundness formulation (mandatory)

The invariant is **one shared symbolic variable per (value, live range)** —
never "(register, range)". A live range of retail `r_i` is a maximal interval
where its value flows without an intervening def; two ranges of the same
register are value-disjoint (the first's value dies before the second begins).
Rebinding at a region boundary from `rho_k` to `rho_{k+1}` is sound iff every
changed binding is **dead on both sides at the boundary**:

- **Retail direction:** for every retail `r` with `rho_k(r) ≠ rho_{k+1}(r)`,
  the old decomp lane `rho_k(r)` must be dead.
- **Decomp direction:** for every decomp `d` with
  `rho_k^{-1}(d) ≠ rho_{k+1}^{-1}(d)`, the old retail lane `rho_k^{-1}(d)` must
  be dead.

The implementation uses the equivalent **four-lane rule** per changed lane (the
two directions are exact duals): retail `r`, old decomp `rho_k(r)`, new decomp
`rho_{k+1}(r)`, and the retail lane `rho_k^{-1}(rho_{k+1}(r))` whose value the
new decomp lane previously carried — all dead. The round-2 stale-lane
counterexample (`rho_k(i)=j`, `rho_{k+1}(i')=j` with `i≠i'`, stale decomp lane
`m = rho_k(i')`) is exactly the fourth lane. Coverage proof: every changed
binding is a mapping-change for some retail `r` iff it is a preimage-change for
`d = rho(r)`; no binding change escapes both directions (round-4 review).

Liveness is a **backwards dataflow fixpoint over the real CFG**
(`_cfg_liveness`), not the straight-line approximation — the straight-line
version ignores branches and would report loop-carried values dead, making the
boundary deadness assertion unsound. Unknown opcodes over-approximate use+def
of all GPR/FPR/PS1. PS1 lanes are defined by scalar-s FP arithmetic
(`_FP_SINGLE_ARITH`, semantics.py:2116), single-precision FP loads
(`lfs`/`lfsu`/`lfsx`/`lfsux`, semantics.py:3613), and `ps_*` ops.

### Masking (terminal comparison)

`_terminals_agree` compares all 32 GPR/FPR/PS1 lanes, memory, LR, CR/XER/FPSCR,
and non-register state. Region-sliced execution rebinds changed lanes to fresh
shared variables; a lane dead at the exit may hold a divergent fresh variable
and is **masked only by the per-exit live-out set**. This is sound: a divergent
value can escape into observable state only via memory (shared verbatim,
compared unconditionally), LR/exit_target (never masked), CR/XER/FPSCR (never
renamed), or a live lane (compared). A spill of a divergent lane exists on both
sides (stores are position-aligned) and diverges in the shared memory array.
Soundness rests on the fixpoint's live-out being correct (over-approximation is
safe; under-approximation is the unsound direction, mitigated by the
unknown-opcode over-approximation and the per-exit masking being the only
weakening).

### Implementation-review fixes — round 2 (2026-08-03, third commit)

The second commit's fixes were re-reviewed (GLM-5.2 + Kimi K3). All round-1
findings verified fixed with PoC-verified load-bearing changes; **no new
false-certificate hole found**. Residual fixes applied:

- **max_paths cross-region accounting** (MINOR): the `+=` accumulator
  double-counted prior regions' terminals (over-strict → premature SMT
  fallback). Replaced with a snapshot check of the cumulative
  frontier+terminal count after each region.
- **MFSPR/MFTB `_use_def`** (NIT): the SPR/TBR index (operand 1) was treated
  as a GPR use; now a pure def of rt (conservative either way, corrected for
  cleanliness).
- **New coverage** (`ImplReview2RegressionTests`): STMW-boundary end-to-end
  PoC (load-bearing for the range-use fix), 3-region rebind chain (multi-
  boundary `old_rho` composition), TWI immediate-not-register, early-return-
  before-boundary, region-driver robustness (never crashes/certifies on
  infeasible seeds).

Round-2 verified-sound (no action): RLWIMI was the only GPR read-modify-write
under-approximation (exhaustive audit vs the engine's use-def table); the
BCLR BO-mask `(BO & 0x14) == 0x14` is exact for always-taken encodings (BO
20-23) across all 32 BO values; per-region gate 5 composes soundly with the
four-lane deadness for region-local live-ins; `_stream_validation_failure`
preserves global-path gate order/messages and the reloc exemption; no
unchecked rho reaches execution or terminal comparison.

### Implementation-review fixes (2026-08-03, second commit)

The first implementation commit (a9d6cd02) was adversarially reviewed against
the code (GLM-5.2 + Kimi K3). Three false-certificate holes were found and
fixed:

1. **Per-region gate 5 (BLOCKER):** the region-sliced path never ran
   `_check_abi_fixedness` — ABI-boundary registers (r0/r1/r2/r13/args/returns,
   live-across-call volatiles) could be remapped in any region's rho and the
   structural comparison self-agrees under the region perm. Fixed: every
   region's rho is gate-5-checked before execution.
2. **Liveness under-approximation (BLOCKERs):** `_use_def` missed RLWIMI's
   accumulator read (operand 0 is read-modify-write) and STMW's full
   rD..r31 read range, and `_cfg_successors` dropped the predicated-`bclr`
   fallthrough edge the executor actually takes (only BO=20 `blr` is
   constant-true). All three under-approximated a *use* — the unsound
   direction — letting a live lane appear dead at a boundary and be rebound.
   Fixed in `_use_def` / `_use_def_numbered` / `_cfg_successors`; TWI's `to`
   immediate is no longer treated as a GPR.
3. **Full-stream gates (MAJOR):** gates 2/3/6 were only checked up to the
   first rho conflict (the global path returns early), so a reject-list /
   reloc / field violation after a conflict could certify. Fixed: the region
   path validates the FULL stream (`_stream_validation_failure`) before
   slicing.

Also fixed: `local_symbol` now threads per side (candidate self-recursion
detection), the loop predicate flags non-link relocated self-calls (tail
recursion), driver-side `max_paths` cross-region accounting, and seed-time
feasibility pre-checks. Regression corpus: `ImplReviewRegressionTests`.

### Fail-closed guards

- **`pairs_checked == 0` guard:** if every terminal pair's combined path
  condition is provably unsat (or exits are empty), the witness rejects — a
  vacuous `True` would be a false certificate. `z3.simplify` is
  equivalence-preserving in the `False` direction, so all-disjoint means genuine
  divergence (round-4 review).
- **First-cut loop policy:** any target whose full-function CFG contains a
  backward branch or a non-return indirect branch (`bctr`/`bcctr`, or `bclr`
  with link) falls through to SMT. Return-position `bclr` (`link=False`, incl.
  predicated `beqlr`/`bnelr`) is a terminal, not rejected.
- **Call model:** across-call rebinding only for precise (non-`*`) contracts
  with the rebound lane not in `contract.reads`, and dead at the call site or in
  `contract.writes`. Opaque `*` contracts read every register (call_token,
  semantics.py:1539) — rebinding any observed lane is unsound and rejected.

### Executor extension

`execute_cfg` gains two strictly-additive kwargs (default `None`; all existing
callers unchanged):
- `stop_at_pcs`: pauses the frontier at pop-time BEFORE executing a boundary
  instruction, collecting `(pc, state, condition, visit_counts, steps)` into
  `paused_out`.
- `initial_seed`: overrides the hardcoded `(start, state, ops.bool(True), {},
  0)` seed so the region driver resumes at a boundary PC with a conjoined
  condition, carried visit_counts, and carried steps.

The pause is at pop-time, so resume re-executes the boundary instruction under
the next region's binding (never `pc + 4`). `visit_counts` carryover preserves
the `max_loop_iterations` bound; driver-side accounting sums `max_paths` across
regions; budget exhaustion falls to SMT.

### Scope limits

- Only **register-resident** values are rebound; spill-offset differences are
  Gate-3 rejects (non-register immediates) and same-offset spills already flow
  through the shared memory array.
- Loop-containing targets are first-cut SMT (fixpoint-enabled second cut is
  future work).
- Indirect `bctrl` targets are out of scope (parked plan
  `indirect_call_certification_plan.md`).

## 6. Third adversarial review (2026-08-04) — false certificates closed

GLM-5.2 and Kimi K3 (pi/OpenRouter) independently reviewed the implemented
witness against the code; **both AGREE on all findings** (probe:
`.scratch/witness_review_probe.py`, 13/13 lines confirmed).  All four fixes
shipped in `tools/coop/lib/renaming_witness.py` + `semantics.py`.

### F1 (BLOCKER) — gate 5 did not enforce entry/exit observability

The input binding (`decomp.gpr[j] = X_{inverse[j]}`) assumes the CALLER
renamed its registers.  That fiction is sound only for lanes whose
caller-visible values coincide on both sides; gate 5 fixed only live-in
r3–r10, returns, and live-across-call volatiles.  Reproduced false
certificates (all previously CERTIFIED): `mr r3,r11` vs `mr r3,r12` (returns
r11-in vs r12-in); `add r3,r3,r20` vs `add r3,r3,r25`; `li r20,7` vs
`li r25,7` (nonvolatile clobber without restore); Kimi H1 (FPR live-in),
H2 (live-in → real callee arg), H5 (`r0` in a genuine RB operand is a
live-in read — doc 32 A2's "r0 renameable" claim held only for write-before-
read uses).  The suite's own `test_nonvolatile_permutation_across_call_accepted`
and `test_pure_nonvolatile_color_swap_accepted` asserted false certificates.

Fixes: (a) gate 5 fixes **every** live-in lane (all of r0–r31/f0–f31, with a
spill-only carve-out for prologue `stw rN, c(r1)` saves so the Chaitin class
survives); (b) a per-terminal **nonvolatile preservation check** for permuted
nonvolatile lanes (terminal ≡ entry binding after `z3.simplify`); (c) the
region-sliced path rejects non-identity mappings on nonvolatile lanes
outright.  Residual (documented, EABI-acceptable): volatile clobbers
(`li r11,7` vs `li r12,7`) still certify — a conforming caller never reads
volatile lanes post-call.

### F2 (BLOCKER) — global path certified bcctr

`_has_indirect_dispatch` was enforced only in the region path; the global
path certified `mtctr; bcctr` pairs (the shared-CTR terminal self-agrees
without jump-table target modeling — the doc's own reason for the reject).
`_has_loop_or_non_return_indirect` had no production caller.  Fix: the
`bcctr`/`blrl` reject now runs in `run_structural_witness` too.

### F3 (MAJOR) — opaque contracts killed the across-call Chaitin class

`SymbolicOps.call_token` keys the transition on every register for opaque
contracts (the FULL_MATCH default); any non-identity rho at a call site
rearranged the token arguments and diverged the UF results.  Fix: the witness
passes its `(gpr_perm, fpr_perm)` to `execute_cfg` (new strictly-additive
`witness_register_perm` kwarg), and `_apply_call_summary` canonicalizes the
register tuples to retail lane order for the token and un-canonicalizes the
summarized writes.  Sound because a genuine EABI callee observes only
fixed/shared lanes; the SMT path is unchanged.  Note: an opaque summary
REPLACES memory, so a save/restore ACROSS an opaque call still falls to SMT
at the preservation check (fail-closed).

### S1 (MINOR) — memory compared structurally

`_terminals_agree` compared memory with raw structural equality while
LR/exit_target got the base-relative carve-out; a post-call `mflr; stw`
stores `pc + 4` and over-rejected.  Fix: `_memory_arrays_agree` recombines
the byte-level store chain into stored words and compares them with the
base-relative rule.  Full pair (`bl; mflr r3; stw r3,0(r1); li r3,0; blr`)
now certifies; aliased/uninterpreted memory still rejects.

### Reach after the fixes

The fast path certifies: volatile temp renames (written-before-read, dead at
calls), spill-only prologue-save Chaitin cycles whose restore simplifies,
across-call renames of lanes dead at the call (via the F3 token), and
location-aware memory stores.  Nonvolatile clobbers, live-in reads, indirect
dispatch, and aliased save/restore bodies fall to SMT — never a certificate.

## 7. Fourth adversarial review (2026-08-04) — round-3 holes closed

GLM-5.2 and Kimi K3 (pi/OpenRouter) plus the session agent independently
reviewed HEAD after the R8/R9 and callee-cert-independence commits.  Three
false-certificate holes and one over-rejection were found and closed:

### R1 (BLOCKER, Kimi + GLM + session) — narrow gate read scan is stream-order

`_full_match_callee_body_fits_narrow`'s "precise live-in read scan"
accumulated a `written` set in STREAM order, so a live-in read reachable only
on a branch-skip path (`bne .L1; li r6,0; .L1: mr r3,r6` reads ENTRY r6 on
the taken path) was shadowed by a later-in-stream def and certified
`reads={r3,r4,r5}` — re-opening the outgoing-argument false-certificate
class R8 was meant to close (real corpus: `long2str`/`wprintf` read ENTRY
r0).  Fix: the gate now uses the witness's own CFG liveness fixpoint
(`_cfg_liveness` entry-live-in), which is path-accurate; the straight-line
scratch shape (`li r6,7; …; or r5,r6,r6`) still fits narrow.

### R2 (BLOCKER, Kimi + session) — tail-call trampoline callee bodies

The gate's internal-call reject was link-only (`_is_call`); a FULL_MATCH
callee ending in a non-link relocated `b` (tail call) passed with zero
register traffic while physically handing r3–r10 to its tail target
(corpus: `LogMsg_0..6 = b LogMsg`, 1002/4313 narrow-scan-ok callees).
Fix: the gate rejects any call form (`_is_call or _is_tail_call`).

### R3 (BLOCKER, GLM-5.2) — opaque callee observed lanes missed f0/f9–f13

`_call_observed_lanes` fixed the EABI argument window r3–r10/f1–f8 (+ps1)
plus r11/r12 for opaque/unknown callees, but a genuine EABI callee may read
ANY volatile register (EABI constrains preservation, not reads).  A
`bl callee; lfs f0,0(r1); fmr f3,f0; blr` vs f9 pair certified while the
physical callee read divergent entry f0 — the F3 token canonicalization hid
it.  Fix: opaque/unknown callees now fix ALL 96 lanes (GPR/FPR/PS1) —
matching the original F3 design ("rebinding any observed lane is unsound").
The earlier r11/r12 patch (502e50099) was GPR-incomplete and is superseded.

### R4 (MAJOR, Kimi + GLM + session) — `lr` in the narrow reads poisoned the token

R8 added `lr` to `_FM_CALLEE_READS`; for PRECISE contracts `call_token`
keys on every declared read, and at a `bl` site `lr = pc+4` is a
location-dependent constant — so every call through a narrow-EABI callee
over-rejected cross-base (and masked R1/R2 when bases differed).  Fix:
`call_token` excludes `lr` for precise contracts too (mirroring the opaque
branch); `lr` stays in the read ENVELOPE (a callee may `mflr`) but never
keys the token.  The over-rejection is gone and, with R1/R2 fixed, removing
the accidental mask is sound.

### Closure status

Prior rounds F1/F2/F3/S1, R7, R8/R9, and the freshness rounds 3/4 were
re-verified intact.  Keying consistency, the errors-gate removal, region-
path per-region gate 5, and the pi-harness wiring were all verified clean.
Regression tests: `test_renaming_witness.Round3AdversarialReviewTests` +
`test_certify_unit_symbol.NarrowCalleeReadsValidationTests`
(branch-skip and tail-call callees fall back to opaque; opaque FPR perm
rejected; narrow call-bearing pair certifies cross-base).

## 8. Eighth adversarial review (2026-08-04) — ps1 preservation + reloc-field gate

Session agent + GLM-5.2 (full report) + Kimi K3 (partial, two provider errors).
The session agent probed 25 candidate shapes; GLM independently re-derived the
reloc-exemption finding and the contract-reads trust boundary.  **No
witness-internal false certificate besides R8-1** (below); the store-reorder
lead (`stw r5,0(r1); stw r6,0(r1)` vs `stw r6,0(r1); stw r5,0(r1)` to the same
slot, "certified") was verified SOUND: under the actual rho both sides store
the same shared variables in the same overwrite order, so the final memory is
identical — a false alarm from probing with the identity perm.

### R8-1 (BLOCKER) — F1 preservation check missed the PS1 half of permuted nonvolatile FPRs

`_nonvolatile_preservation_failure` verified only `fpr[lane]` for permuted
f14–f31.  Scalar-single FP (`fadds`/`fmuls`/`fdivs`/`frsp`, semantics.py:4500
`if is_single: state.with_ps1(fd, d_bits)`) and single-precision loads
(`lfs`, :3626) write `ps1[fd]`; a prologue that restores only the double half
(`stfd`/`lfd`) leaves the ps1 half clobbered.  Probe P24
(`stfd f20,8(r1); fadds f20,f21,f22; lfd f20,8(r1); blr` vs f20↔f25) certified:
the terminal comparison indexes ps1 by the same perm, so the clobber
self-agreed while the physical caller-visible ps1 lane diverged (retail
clobbers ps1[20], decomp preserves it) — the F1c class for the ps1 half, and
the SMT path would reject it (f14.ps1–f31.ps1 are `ppc-eabi` observables).
Fix: the preservation check now also requires
`z3.eq(z3.simplify(exit.ps1[lane]), initial.ps1[lane])` for permuted lanes.
Over-rejection is confined to functions that clobber a permuted nonvolatile's
ps1 without restoring it — exactly the unsound class; identity-mapped lanes
are still compared physically and unaffected.

### R8-2 (MAJOR, robustness) — gate 3 relocation exemption skipped ALL non-register bits

`_stream_validation_failure` skipped the entire non-register bit comparison
for any slot carrying a relocation (`if r_reloc is None:`).  The relocated
field is only the address field (LI/BD/displacement); LK/AA/BO/BI on
relocated `b`/`bc` — same `Opcode.B`/`Opcode.BC` regardless of `link` — were
left to the downstream terminal comparison, which caught every probed shape
(`bl sym` vs `b sym`, `beq` vs `bne`, `bc` vs `bcl`; exit-kind/LR/clobber
divergence) but violated the documented gate-3 contract.  Fix: the exemption
now masks only the relocation's address-field bits per type (REL24: bits 6-29;
REL14: 16-29; ADDR16_*: 16-31; EMB_SDA21: 11-31 — mirroring the decoder
fixup masks); all other non-register bits are compared as before.  The
relocated-branch flag differences are now rejected at gate 3 directly.

### Trust boundaries re-verified (GLM + session)

- **Precise-contract `reads` soundness is load-bearing** (GLM F1) — and the
  session agent FOUND and FIXED a real under-approximation mechanism
  (2026-08-04): `_validate_callee_contract_impl` collects reads from the
  final values of WRITTEN components plus terminal conditions, and a memory
  write's final value is deliberately excluded from that scan (huge
  Store/Select cones).  A store-only leaf (`stw r11, 0(r3); blr`) therefore
  validated to reads={memory, r3, valid} — the ADDRESS r3 was captured only
  via the store's definedness constraint, but the stored VALUE r11 was
  dropped.  The witness's gate-5 call-observed rule then left r11 free to
  rename at a call site and the F3 token canonicalization hid the
  divergence: a caller pair renaming r11<->r12 (dead at the call) around
  such a callee CERTIFIED while the physical substitution stores a different
  value (probe reproduced end-to-end, certified=True before the fix).  Fix:
  the validation now enumerates the register reads of memory-WRITING
  instructions directly via `register_effects` (sound over-approximation;
  declared contracts already contain these reads, so validations stay
  valid).  Regression: `StoreSourceReadCaptureTests` + caller-pair probe now
  rejects at gate 5 while a constant-store callee (no r11 read) still
  certifies.  Opaque (`reads="*"` → all 96 lanes fixed) and the narrow
  FULL_MATCH contract (validated by `_full_match_callee_body_fits_narrow`)
  are sound.  The module docstring's "non-EABI opaque callee"
  false-certificate class is already closed by the fix-all-lanes rule — the
  docstring is stale (GLM F3).
- **Location-independence**: `_value_equal`'s base-relative rule applies to
  memory store words, not just LR/exit targets (probe P4d certifies a
  post-call `mflr; stw` storing different absolute addresses).  The SMT path
  shares the opaque-lr UF model, so witness and SMT agree; the residual is vs
  real hardware for functions that return/store their own address.
- **Caller-renaming fiction**: nonvolatile-permutation certificates are
  equivalence modulo a consistently-renamed caller (P11 class); the SMT path
  rejects them.  Accepted design, recorded rho in the certificate.
- **Engine/decoder (R8-5, FIXED)**: the decoder's unary X-form table had
  the EXTSH/EXTSB XO values swapped (922↔954) — real 16-bit `extsh`
  (XO 954) executed as an 8-bit extension and real 8-bit `extsb`
  (XO 922) as 16-bit.  Verified by execution (r4=0x8005: XO 954 produced
  0x00000005, XO 922 produced 0xFFFF8001) and by the retail census (XO 954
  = the common 16-bit extend, 3078 sites vs 1265 for XO 922).  The operand
  convention was CORRECT all along (dest at bits 16-20, source at bits
  21-25, bits 11-15 reserved — the retail census shows MWCC conforms); only
  the width mapping was wrong.  Fix: decoder.py unary table now maps
  922→EXTSB / 954→EXTSH; fixture corpus + blob regenerated; regression test
  (`test_extsh_extsb_widths_match_isa`) + pinned-encoding test updated.
  Impact: every equivalence verdict (witness AND SMT) for width-sensitive
  extsh/extsb pairs was computed under the wrong model; the engine_hash
  field auto-invalidates previously-issued certificates (hash_engine_tree
  covers decoder.py).  `dcbt`/`dcbst`/`dcbf` are engine no-ops;
  `lwarx`/`stwcx`/`eieio` are not decodable at all.

Regression tests: `Round8AdversarialReviewTests` (ps1-clobber reject, identity
scalar-s certify, relocated b-vs-bl / beq-vs-bne gate-3 rejects, identical
relocated pair certifies).
