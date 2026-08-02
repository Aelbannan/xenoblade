# 31 — Register-renaming witness: fast-path equivalence certificates

Status: **design revised after adversarial review** (GLM-5.2 soundness review, Kimi K3
integration review; both verified against code). **Implemented**: the pre-SMT
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
