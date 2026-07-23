# Soundness specification

<!-- BEGIN GENERATED PPC_EQUIVALENCE_VERSION -->

- Architecture model: `broadway-ppc32-be-v44`
- Result format: `24`
- Certificate format: `18`

<!-- END GENERATED PPC_EQUIVALENCE_VERSION -->
<!-- BEGIN GENERATED PROOF_STATUS_TABLE -->

| Status | Value |
|---|---|
| `EQUIVALENT` | `equivalent` |
| `NOT_EQUIVALENT` | `not_equivalent` |
| `INCONCLUSIVE_TIMEOUT` | `inconclusive_timeout` |
| `INCONCLUSIVE_UNKNOWN` | `inconclusive_unknown` |
| `INCONCLUSIVE_UNSUPPORTED` | `inconclusive_unsupported` |
| `INCONCLUSIVE_ABSTRACTION` | `inconclusive_abstraction` |
| `INCONCLUSIVE_LAYOUT` | `inconclusive_layout` |
| `INCONCLUSIVE_UNVALIDATED_CALLEE` | `inconclusive_unvalidated_callee` |
| `INCONCLUSIVE_UNMODELED_EXCEPTION` | `inconclusive_unmodeled_exception` |
| `INVALID_INPUT` | `invalid_input` |
| `INTERNAL_ERROR` | `internal_error` |

<!-- END GENERATED PROOF_STATUS_TABLE -->

## Two gates (do not conflate)

1. **EQUIVALENT_MATCH acceptance** (decomp target closed): objdiff fuzzy
   ≥ 50%, SMT `ProofStatus.EQUIVALENT`, and split-size fit. Confidence
   tiers and capability assurance do **not** block this gate. Certificates
   record `equivalence_strength` (`integer-core`, `fp-tier-c`,
   `callee-dependent`, …) for honesty.
2. **Promotion / Tier A–B reuse**: `classify_for_promotion` plus optional
   capability-assurance. Default capability manifests keep
   `shadow_mode=true` (assurance is advisory; legacy effect gates remain
   authoritative). The `PromotionPolicy` *dataclass* default is `True`
   for historical coop behavior, but the authoritative runtime value comes
   from `CoopConfig.automatic_promotion`, which defaults to **`False`**
   (`PromotionPolicy.from_config` propagates it), so real workflows have
   automatic promotion **off** by default. Keep `automatic_promotion: false`
   in `coop.json` for conservative workflows. FP and assumed callees are
   Tier **C**.

An `EQUIVALENT` result is always scoped to the listed observables and
assumptions. Unsupported opcodes, timeouts, and solver `unknown` are
inconclusive — never silent no-ops.

## Theorem

For all shared initial machine states satisfying the selected contract
preconditions, environmental profile, relocation/layout constraints, and
instruction-domain restrictions, there exists no feasible pair of modeled
terminal executions whose exit behavior, definedness state, or selected
observable architectural outputs differ after applying only the explicitly
documented per-implementation private-storage abstraction.

## Definitions

### Initial-state quantification

- All GPR (r0–r31), FPR (f0–f31), PS1 (f0.ps1–f31.ps1), GQR (gqr0–gqr7),
  SR (sr0–sr15), CR, FPSCR, XER, LR, CTR, MSR, time-base, SRR0, SRR1, and
  modeled auxiliary SPR values are independent symbolic 32-bit or 64-bit
  bitvectors.
- All memory bytes are independent symbolic 8-bit bitvectors.
- Both implementations receive the **same** initial state. There is no
  per-implementation initial-state splitting.
- ABI constraints are applied only through the contract: `ppc-eabi` selects a
  fixed subset of observables; `auto` augments it with persistent state written
  by either side.

### Feasible execution

- A path is feasible when the conjunction of path conditions (branch predicates,
  relocation/layout constraints, alignment constraints) is `sat`.
- CFG construction enumerates feasible paths. Back-edges are allowed up to
  `max_loop_iterations` visits per PC on a path (default 256). Exceeding the
  bound yields `INCONCLUSIVE_UNSUPPORTED` and never silently truncates the
  exploration. Constant-trip loops (for example `li`/`mtctr`/`bdnz` with a
  concrete count at or below the bound) complete normally when the exit
  condition becomes concrete. Affine CTR closed-form summaries
  (`loop_summary` / `affine-loop-summary`) are applied at recognized
  `li`/`mtctr`/`addi`/`bdnz` headers inside `execute_cfg` when the engine
  supplies a summary map; trip count must be a positive constant.
  Compare-affine countdown loops (`li` / affine body / `addi -1` / `cmpwi` /
  `bne`) use the same summary map with `proof_kind=compare-affine-closed-form`
  and leave CTR unmodified. A `FinalCompare` is applied after closed-form GPRs
  so CR (including XER.SO) matches the last `cmpwi`.
  Relational induction discharges matching CTR-affine and compare-affine pairs
  via five independent UNSAT queries
  (`relational_discharge.try_smt_discharge_ctr_affine` /
  `try_smt_discharge_compare_affine` /
  `relational_induction.try_discharge_relational`): initiation, preservation,
  exit agreement, postcondition, and ranking termination (CTR-descending or
  compare-affine counter-descending). Pattern match alone never marks
  `discharged`. Narrow templates only: equal CTR/counter, equal GPRs, constant
  offset, equal validity, equal CR fields, equal memory. Affine closed-form
  obligations (`loop_summary`) reach `status=discharged` only together with that
  relational companion and a matching `summary_sha256`. Shape-only natural
  sketches remain pending unless CTR-affine-backed; mismatched bodies stay
  unsupported for `EQUIVALENT`. Bulk+remainder pairs discharge when both sides
  share a constant-value contiguous `RangeWrite` and the bitvector identity
  `N = (1<<k)*(N>>k)+(N&mask)` with `remainder < (1<<k)` is UNSAT-proven;
  otherwise the sketch stays `pending`.
  Constant-stride store loops (`memory_loop` / `memory-loop-summary`, set
  algorithm `constant-stride-store-set-v3`, schema version `2`) with a
  positive concrete trip count are applied in closed form inside
  `execute_cfg` via typed `StoreEffect` / `apply_store_effect` (recording
  `memory_writes` + `memory_touches`, not memory alone), gated by an
  explicit entry-CTR premise (`CTR == summarized trip count`). Closed-form
  recognition alone **never** discharges: the engine builds a
  `MemoryLoopPlan` (summary + exact instruction witness) per side and calls
  `memory_loop_discharge.discharge_memory_loop_plan`, which proves
  `instructions ≡ summary` via `entry_guard` plus refinement queries
  (`body_step`, `postcondition`, `stack_escape`, `termination`, `footprint`)
  that execute the recognized witness with ordinary `execute_instruction` /
  `execute_bdnz_latch` semantics. Original and candidate plans are discharged
  independently — structural summary equality and equal header PCs are **not**
  required. Obligations then carry `status=discharged` only when every used
  side entry is discharged; recognition / `coverage=applied` / legacy v1
  self-referential transition-equivalence never authorizes. Refinement SAT is
  `INTERNAL_ERROR` (summary disagrees with instruction semantics), not
  `NOT_EQUIVALENT`. A summarized store of an `r1`-derived value clears
  `stack_private` (via `stack_escape.mark_stack_pointer_escape`) exactly as an
  ordinary D-form store would, so publishing the stack pointer through a
  summarized loop cannot hide divergent private-frame bytes. Bounded-remainder
  expansions stay `applied` (never discharged). Recognizer accepts
  exact `store; addi` or lone `stwu` bodies only (rejects reversed order,
  multi-store, calls, source==base). `mtctr 0` is unsupported (bdnz wrap). The
  heuristic `skip-branch` zero-trip guard is **disabled** for soundness: only a
  `concrete-nonzero` trip authorizes bounded-remainder expansion; symbolic
  remainders whose zero-trip case is only guarded by a nearby branch stay
  unsupported. Trip count
  must be recovered from bounded straight-line GPR materialization immediately
  before `mtctr` (`addi`/`addis`/`ori`/`oris`, `andi.`/`andis.` remainder masks,
  exact `srwi`-equivalent `rlwinm` forms (`rlwinm rA,rS,32-n,n,31` for
  `n` in `1..31`), self-`addi`, `or` register copies, or `lwz` only when the
  effective address and loaded word are proven from a per-side
  `MemoryLoopReadonlyContext` (never cross-side `dict.update`) — either an
  explicit `readonly_words` map or linked per-side image hydration via
  `memory_loop_image.try_build_memory_loop_readonly_words` in coop
  `_prove_bytes`, with `Select(initial_memory, addr+i)==byte` premises. The
  original side is hydrated from the retail **DOL only** (`elf_path=None`) and
  the candidate side from the linked **ELF only** (`dol_path=None`), so the two
  images are never conflated; `ReadonlyWordEvidence` records optional
  `artifact_sha256` / `image_kind` (`dol`|`elf`) / `section` provenance. The
  resulting `memory_loop_readonly` identity premise (`per-word-image-v1`,
  `build_memory_loop_readonly_obligation`) is built *before* the cache lookup
  and folded into both `_cache_key` and `proof_request_hash`, so cache reuse and
  the request hash bind the exact hydrated words. When CTR `lwz` addresses exist
  but evidence is missing on either side, the summary is disabled on **both**
  sides (ordinary execution proceeds). Same VA with disagreeing side values
  fails closed (`INCONCLUSIVE_UNSUPPORTED`). Symbolic or relocated sources
  remain partial. Trip counts above `MAX_MEMORY_LOOP_TRIPS` or spanning past
  32-bit remain unsupported.
- Indirect branches (`bclr`/`bcctr` without a known target) are unsupported.
  Jump-table pattern recognition (`jump_table.find_jump_table_candidates`) is
  descriptive only: matching the `cmplwi` / shift / `lwzx` / `mtctr` / `bctr`
  shape does not prove equivalence. Without a `JumpTableProofContext`, the
  engine fail-closes otherwise-matching jump-table functions to
  `INCONCLUSIVE_UNSUPPORTED`. Beyond that canonical shape,
  `jump_table_obligations.unresolved_indirect_exit_gate_reason` is a
  terminal-based catch-all: when neither a jump-table nor virtual-call proof
  context is attached, *any* retained `indirect-branch` / `call-indirect`
  terminal (for example a `bcctr` with no adjacent `lwzx`) demotes an
  otherwise-`EQUIVALENT` result to `INCONCLUSIVE_UNSUPPORTED`. Input-derived
  MWCC virtual thunks are exempt (both sides branch to a CTR value loaded from
  the shared `this` object and `exit.target` is compared for equality — see the
  exit-kind ABI filter). When a proof context is present, the retained
  unknown-remainder terminal is instead discharged by the coverage / no-write
  path below and the catch-all is skipped. With a context (explicit or auto-built by
  `jump_table_auto.try_auto_jump_table_context` from `lis`/`addi` + linked
  DOL/ELF hydration), CFG expands each enumerated CTR target and retains an
  unknown-remainder ``indirect-branch`` terminal. Initial ROM bytes are pinned;
  coverage and no-write are independent UNSAT discharges over remainder
  conditions and ``memory_writes`` hit-table queries (`discharge.py`), not
  final-memory-value inspection alone. `readonly-image` /
  `indirect-target-closure` obligations (schema v2) carry discharge digests
  (`jump_table_obligations.py`). On the EQUIVALENT-ready path
  (`validate_proof_features(require_equivalent_ready=True)`) the strict
  validators (`validate_readonly_image_obligation_strict`,
  `validate_indirect_targets_obligation_strict`) require `schema_version`
  exactly `2`, a known algorithm, top-level `status=discharged`, dict-form
  `coverage.result` / `no_write.result == "unsat"`, lowercase 64-hex SHA-256
  digests, both `original` and `candidate` sides, validated artifact hashes, and
  reject inconsistent flat mirrors — anything forged (failed/SAT/pending or
  legacy schema) fails closed and cannot authorize `EQUIVALENT`. Invisible
  ``gpr[base]==table.base`` /
  ``index < entry_count`` premises are not synthesized. Retail/candidate
  handlers are paired by
  logical case index (`jump_table_pairing.py`), not absolute address equality.
  Coop `_prove_bytes` attempts auto-context when a linked image is available:
  it passes per-side ``JumpTableArtifacts`` (retail ``main.dol`` + linked
  ``main.elf``), re-decoding a 768-byte text window around the dispatch tail
  so distant ``addis``/``addi`` table-base materialization is visible to
  ``resolve_table_base_va`` (up to 256 instructions back from the feeding
  ``addi``). Without resolvable base VAs or complete ADDR32 table
  hydration, auto-context returns ``None`` and the proof stays fail-closed.
- Path count and instruction count are bounded by `max_paths` (default 256) and
  `max_instructions` (default 2048). Exceeding either produces
  `INCONCLUSIVE_UNSUPPORTED`.

### Terminal behavior

A terminal is a triple `(condition, state, exit_kind, exit_target)`. Exit-kind
strings below are the exact values emitted by `semantics.execute_cfg`:

| Exit kind | Meaning | Target compared |
|---|---|---|
| `return` | `bclr` LK=0, or summarized matched call returning | LR / none |
| `direct-branch` | `b`/`bc`/`bcctr` with LK=0 and a concrete target | Exit address |
| `indirect-branch` | `bcctr` (or similar) without a call summary | Exit address |
| `fallthrough` | End of decoded block | None |
| `call` | Linked branch recorded before summary application | Next instruction |
| `call-indirect` | Linked indirect call without a resolved summary | Exit address |
| `program-exception` | `twi` trap taken | Exception vector (`0x700`) |
| `system-call` | `sc` | Exception vector (`0xC00`) |
| `return-from-interrupt` | `rfi` | Restored SRR0 |

- Exit-kind mismatch is always a difference.
- Exit-target mismatch is compared when both sides have a non-`None` target and
  neither side is `fallthrough`.
- Matched-callee summaries replace a `call` continuation with a summarized
  `return`; without a recorded summary the call path is inconclusive.

### `valid` and partial-domain equivalence

- `state.valid` is `True` when the terminal state satisfies all modeled
  architectural constraints (aligned accesses, defined divide inputs, modeled FP
  domain restrictions, supported opcode encodings).  When `valid` is `False`,
  `state.invalid_reason` records the first constraint that failed as a
  small-integer code drawn from ``InvalidReason``:

| Code | Reason |
|---|---|
| 0 | `NONE` (valid) |
| 1 | `UNALIGNED_ACCESS` |
| 2 | `DIVIDE_UNDEFINED` |
| 3 | `FP_DOMAIN_EXCLUDED` |
| 4 | `FP_ROUNDING_MODE` |
| 5 | `CACHE_DISABLED` |
| 6 | `PRIVILEGED_INSTRUCTION` |
| 7 | `PSQ_INVALID_TYPE` |
| 8 | `PSQ_NONFINITE_INTEGER_STORE` |
| 9 | `UNSUPPORTED_SIDE_EFFECT` |
| 10 | `MEMORY_PROFILE_VIOLATION` |

- The comparison is **definedness-preserving partial equivalence**:
  - If exactly one side is `valid`, the outputs differ.
  - If both sides are `valid`, all selected observables are compared.
  - If both sides are `invalid`, the `invalid_reason` codes are compared.
    A difference in reason codes is a mismatch.  When both sides agree on
    the reason code, **selected observables are still compared** (including
    touched memory).  A store that commits before a shared faulting op must
    not be invisible to `EQUIVALENT`.  No claim is made about the relative
    exception class or hardware exception delivery beyond the modeled reason
    code and compared architectural state.
  - All `invalid_reason` values are Z3 bitvector expressions directly
    comparable by the solver.  A reason-code mismatch is found when the
    solver returns `sat` on the inequality; `INCONCLUSIVE_UNMODELED_EXCEPTION`
    is reserved for future models where `invalid_reason` is opaque.
- A proof over unsupported opcodes or unaligned memory never escapes via a side
  condition to claim vacuous equivalence. The engine fails closed.

### Memory model

- 32-bit byte-addressed array with big-endian multi-byte loads/stores.
- Comparison is **touched-byte extensional**: every byte written by either
  implementation is compared individually.
- **Per-implementation independent private-stack masking**: each side's own
  private stack interval `[stack_low, entry_sp)` is masked independently.
  An address private to one implementation does not hide a write by the other
  implementation. Masking is disabled when a call executes (including fixed
  EABI `_savegpr_*`/`_restgpr_*`/`_savefpr_*`/`_restfpr_*` helpers) or when an
  r1-derived value is stored to a **non-r1-relative** address (direct `r1`,
  register aliases such as `mr`/`ori`, computed pointers such as
  `addi rD,r1,imm` published via `stw` to a public base, or `stmw` ranges that
  include `r1` targeting a public base), **or** when a load-derived (`Select`)
  word is stored to an address that is not itself r1-relative (fail-closed: the
  loaded word may equal the entry SP), **or** when any contract-compared GPR
  other than `r1` still depends on `input.gpr.r1` at a terminal (register
  publish: e.g. `addi r3,r1,8` left live across `blr` — the caller can load
  through that pointer into the former private frame). Escape detection walks
  the BV cone for `input.gpr.r1` and for `Select` nodes without descending into
  array `Store`/`Select` spines (avoids super-linear memory-history walks).
  Spills of r1-derived or loaded words back onto r1-relative addresses —
  including the standard `stwu r1,-N(r1)` back-chain save — keep private-stack
  masking. Temporary frame-pointer registers that are **not** compared at the
  exit (volatile scratch such as `r11`) do not clear masking by themselves.
  The compared-register gate runs in the engine after CFG exploration
  (`apply_compared_register_publish_escape`) once `observables_for_exit` is
  known; store-path escape remains in `stack_escape.mark_stack_pointer_escape`.
- Stack layout feasibility also rejects frames deeper than
  `MAX_PRIVATE_STACK_DEPTH` (16 MiB). That bound fail-closes SP wraparound
  through address zero, which would otherwise collapse `stack_low` near 0 and
  privatize almost all memory.
- Alignment: aligned loads/stores are required for multi-byte accesses.
- Memory-environment profiles (`assumed-ordinary-ram`, `bounded-ordinary-ram`,
  `stack-and-known-globals`, `hardware-aware`) are always recorded on
  `ProofResult.environment` as the effective solver environment (default
  `assumed-ordinary-ram` when the caller omits a profile). Tiering treats
  assumed-RAM as Tier C only when `memory` is among the contract observables,
  so register-only proofs stay eligible for Tier A. Promotion may still require
  bounded ranges via `require_bounded_ram`. Stage 3A binds a separately
  configured reviewed platform profile via coop `platform_profile` (name or
  path); its digest enters proof-request / cache identity as
  **`platform_profile_sha256`**. Address-coverage obligations use terminal path
  conditions only and must not fold `build_memory_constraints()` into the
  coverage query (`bounded_memory_obligations.build_bounded_memory_obligation_from_terminals`).

### Capability assurance (Wave 1–5)

Tier classification is being migrated from effect-type hard-gates
(`has_fp → C`, `has_memory_bus → C`, …) to **capability-assurance-v1**:

- Schema: `ProofResult.capability_assurance` /
  `tools/ppc_equivalence/capability_assurance.py`
  (`CapabilityAttestation` / `CapabilityAssurance`, schema version `1`).
- Promotion allowlist: `tools/coop/capability_manifest.json`
  (`allowed_tier_a_capabilities`, `shadow_mode`, `require_capability_assurance`).
- **Shadow mode (default):** `evaluate_capability_assurance` always runs and
  records `capability-assurance-shadow-tier-*` warnings, but authoritative
  `compute_confidence_tier` still uses legacy effect gates. Under shadow,
  legacy still forces Tier C for any FP; the assurance shadow tier may still
  report A for bitwise-only proofs. Default manifest keeps
  `shadow_mode=true` for safety.
- **Wave 5 authoritative canary:** optional file
  `tools/coop/capability_manifest.authoritative.json` sets
  `shadow_mode=false` and `require_capability_assurance=true` with the same
  safe allowlist (integer-core, provenance, fp-bitwise, bounded-memory,
  certified-calls — **not** scalar FP, MMIO, or gx-fifo). Point
  `capability_manifest_path` at that file and set
  `capability_assurance_shadow_mode=false` /
  `require_capability_assurance=true` in `coop.json` (knobs override the
  file when present). Keep `automatic_promotion=false`. Decision records
  include a `capability-assurance-audit:` warning plus
  `PromotionDecision.assurance_audit` (capabilities used, weakest status,
  shadow vs authoritative tier).
- **Wave 2 `fp-bitwise`:** promoteable ops are only `fmr` / `fabs` / `fneg` /
  `fnabs` (`tools/ppc_equivalence/fp_bitwise.py`, algorithm
  `fp-bitwise-ledger-v1`, model `fp-bitwise-v1`). Evidence requires opcodes,
  `ledger_sha256`, and `host_float: false`; unsupported remainder must be
  empty. Any non-bitwise FP opcode demands other FP capabilities (coarse
  `fp-scalar-arithmetic` today) and cannot earn promotion-grade `fp-bitwise`.
  Ledger coverage lives under `validation_ledger.yaml` → `capabilities.fp-bitwise`.
- **Wave 3 FP foundations (shadow):** FP opcodes demand distinct capabilities
  (`fp-load-store`, `fp-compare`, `fp-convert`, `fp-scalar-arithmetic`,
  `fp-fused-arithmetic`, `fp-paired-single`, `fp-psq`, `fp-traps`) via
  `fp_capabilities.classify_fp_capabilities`. Model-version stubs exist in the
  manifest with **empty** allowlists (except `fp-bitwise-v1`). Algorithms
  `fp-scalar-oracle-v1` / `fp-outcome-unify-v1` stay incomplete.
  `symbolic_fp_outcome` unifies SymbolicOps/ConcreteOps containers for bitwise
  ops (Z3 BitVec, no host float); symbolic `fadd` remains unsupported.
  Tier A for scalar requires all four RN modes **or** `precondition-closure-v1`
  with violation-query UNSAT — default nearest-even config alone is not proof
  (`fp_rounding.py`). FPSCR FX/FEX sticky behavior remains incomplete.
  Obligation schema: `fp_scalar_obligations.py`.
- **Wave 4 FP advanced (shadow, never promotion-grade):** obligation /
  attestation stubs in `fp_advanced_obligations.py` for
  `fp-fused-arithmetic`, `fp-paired-single`, `fp-psq`, and `fp-traps`
  (algorithms `fp-*-incomplete-v0`). Fused midpoint-tie residues and
  near-cancellation sticky residues remain incomplete; MSR FE0/FE1 precise
  vs imprecise trap modes remain incomplete (`fp_traps.fe0_fe1_modeling_status`,
  ledger `dimensions.fe0_fe1: false`). Even forged `status=promotion-grade`
  or a non-empty allowlist cannot promote these stubs. Tagging hooks in
  `fp_outcome.capability_tags_for_opcodes` /
  `fp_traps.capability_tags_for_trap_domain` demand the caps when fused /
  paired / PSQ opcodes or `traps_enabled` appear. Allowlists stay **empty**.
- **Wave 3 MMIO foundations:** MMIO is split into independently attested
  capabilities (`mmio-register-bank`, `mmio-read-side-effects`,
  `mmio-external-input`, `gx-fifo-write-trace`, `gx-fifo-read`,
  `mmio-loop-emission`, `mixed-address-space-routing`,
  `dma-interrupt-effects`). Reviewed hardware profiles live under
  `platform_profiles/` (e.g. `wii-broadway-xenoblade-us-v1`) and bind via
  `hardware_profile_sha256`. Obligation schema:
  `tools/ppc_equivalence/mmio_capability_obligations.py` (algorithms
  `mmio-register-bank-v2`, `gx-fifo-trace-v1`). Ad-hoc CLI bus maps are
  source=`ad-hoc-bus-map` and never promotion-grade. Profile hash mismatch
  and malformed obligations fail closed. **`gx-fifo-read`**, DMA, and loop
  emission remain incomplete / Tier C. The MMIO allowlist stays empty until
  canary (schema + incomplete grades land now).
- **Wave 4 MMIO advanced (shadow):** `gx-fifo-loop-refinement-v1` structural
  obligation proves ordinary N FIFO writes ≡ summarized N-event trace;
  incomplete unless both sides carry real (non-placeholder) UNSAT digests.
  `mmio-read-side-effects`, `mmio-external-input`, and
  `dma-interrupt-effects` attestation stubs are always incomplete / Tier C
  until a Dolphin hardware harness exists. **`gx-fifo-read`** remains
  explicitly non-promotable. Wave 4 advanced allowlists stay **empty**.
- **External event sharing (ReadOracle):** timer/status/input registers are
  modeled as `ReadOracle(device_id, event_index, device_state)`
  (`tools/ppc_equivalence/external_event.py`). The same physical external
  event may be shared only when both sides perform the same logical read at
  the same event index under equivalent device state; additional or reordered
  reads consume distinct events — original and candidate must not silently
  receive the same value after different read histories.
- Caller-supplied `status=promotion-grade` is ignored for trust; validators
  recompute grades from evidence + ledger + manifest.
- Legacy certificates without a `capability_assurance` field do not invent
  attestations (assurance path is unproven → would be Tier C).
- `allowed_engine_sha256` is required for promotion once assurance is
  authoritative; it remains optional while `shadow_mode` is true. Pin with:
  `python3 -c "from pathlib import Path; from tools.ppc_equivalence.provenance import hash_engine_tree; print(hash_engine_tree(Path('.')))"`.
  Provenance `git_dirty` is scoped to the engine/certifier trust boundary
  (not whole-repo dirt).

#### Wave 5 recertification notes

After an architecture bump (`automatic_promotion` stays `false`):

1. Run bottom-up `targets recertify` (`--dry-run` first) before expecting a
   `callees-accepted` frontier.
2. Expand `allowed_tier_a_capabilities` one capability / model version at a
   time; recertify leaves first after each change.
3. Never enable “all FP” or “all MMIO” in one bump — keep scalar FP and
   MMIO/gx-fifo allowlists empty until each has promotion-grade evidence.

**GX FIFO Tier-A (v43+):** the live architecture model is now
`broadway-ppc32-be-v44` (v43 introduced GX FIFO Tier-A; v44 fixed both-invalid
observable suppression; v42 stays reserved for the Phase 12 scalar FP exact
v2 production switch — see `SCALAR_FP_V2.md`). See
[`GX_FIFO_TIER_A.md`](GX_FIFO_TIER_A.md) for the frozen `gx-fifo-write-trace`
/ `gx-fifo-read` / `mmio-loop-emission` domain, and
`gx_fifo_v1_rollout.py` (`python3 -m tools.ppc_equivalence.gx_fifo_v1_rollout
readiness`) for the staged allowlist plumbing. `gx-fifo-read-v1` is outcome 3
(architecturally reserved / undefined for equivalence) — allowlisting it
authorizes the unsupported-read *policy* attestation, never a FIFO-load value
model. Default and authoritative manifests keep every gx-fifo /
mmio-loop-emission allowlist empty; staged canaries live at
`tools/coop/capability_manifest.gx_fifo_*_canary.json`.

  - `assumed-ordinary-ram`: accesses are unconstrained ordinary RAM (external
    assumption; default). Private-stack masking still applies independently.
  - `bounded-ordinary-ram`, `stack-and-known-globals`, and `hardware-aware`:
    **require nonempty explicit ranges**. With ranges, every touched byte
    address on a feasible path must lie in a configured range (no wraparound).
    Empty ranges **fail closed** (unsat domain → `INCONCLUSIVE_LAYOUT`) and
    never silently degrade to unconstrained RAM. Range constraints compose with
    private-stack masking: stack-private bytes are still compared via masking,
    but touches must also fall inside the declared RAM ranges.
    `InvalidReason.MEMORY_PROFILE_VIOLATION` (code 10) is the reserved reason
    code for out-of-profile access tagging.
- 32-bit address wraparound is rejected through the layout-feasibility check.
- The initial memory array is shared between both implementations. Private-stack
  bytes are replaced with the common initial byte on each side independently.
- **AddressSpace router:** `address_space.py` models ordered, non-overlapping
  regions (`ram` / `rom-image` / `mmio` / `unmapped`) with concrete `classify` /
  `classify_range`. ROM images can be built from ELF allocatable PROGBITS via
  `elf_symbols.rom_image_from_allocatable_section`. Symbolic accesses that may
  span regions path-split or fail closed. Under opt-in ``MemoryBus`` +
  ``proof_features: ["memory-bus"]``, AddressSpace classification binds into
  engine-generated ``memory_bus`` obligations; ``EQUIVALENT`` requires
  ``status=discharged`` with schema v2 attestations (see Memory-bus sections
  below). Without the memory-bus feature, default proofs keep unconstrained
  RAM and do not promote on AddressSpace alone.
- **MMIO device models (live on opt-in bus):** `device_model.py` defines
  ``DeviceModel`` implementations (``RegisterBankDevice``, write-only
  ``GxFifoStreamDevice``) and ``address_space.mmio_region`` /
  ``attach_mmio_region`` attach MMIO spans with optional ``device_id`` labels.
  Under ``build_memory_bus(..., devices=)`` + ``execute_cfg(..., memory_bus=)``
  with ``ConcreteOps``, matching ``device_id`` loads/stores hit the device
  (register-bank R/W; GX FIFO records writes and rejects reads). Missing
  devices, unsupported widths/alignments, and ``AccessOutcome.UNSUPPORTED``
  fail closed (``BusOutcome`` / ``ExecutionInconclusive``) with no silent RAM
  fallback. Symbolic pure-MMIO routing (register-bank / bounded FIFO) is live
  under discharged ``memory-bus`` obligations; mixed RAM/ROM/MMIO symbolic
  addresses and loop-summary × FIFO remain fail-closed and cannot discharge.
- **Symbolic register-bank / FIFO CFG routing (PR 14/15 progress):**
  ``symbolic_bus.py`` models extensional per-register bitvectors, nested
  ``addr == base + offset`` routing, write / W1C / read-clear formulas, and a
  *separate* unsupported-access query (``path ∧ ¬supported``). SAT on that
  query means inconclusive; ``supported`` must not be assumed into the
  equivalence query. ``SymbolicOps`` ``execute_cfg(..., memory_bus=)`` routes
  concrete MMIO loads/stores through register banks and emits bounded GX FIFO
  write traces; FIFO reads and symbolic-loop×FIFO remain unsupported.
  Pure-MMIO symbolic routing exists; mixed RAM/ROM/MMIO symbolic addresses
  remain fail-closed (``symbolic-mmio-mixed-address-space``). Loop-summary ×
  FIFO has a hard rejection (``symbolic-loop-fifo-emission``); bounded
  summarized emission is not supported. Both rejects are attested on the
  ``memory_bus`` obligation (``loop_fifo_policy=hard-reject``,
  ``mixed_space_symbolic_mmio=fail-closed``, ``cfg_rejection_reasons``) and
  cannot coexist with ``status=discharged``. Final touched device state is an
  automatic observable.
- **Memory-bus (Track D authorization):** ``memory-bus`` authorizes
  ``EQUIVALENT`` only with engine-generated ``memory_bus.status=discharged``
  under ``require_equivalent_ready`` and the strict validator (recomputed
  digests, per-side access coverage, theory/spec binding). Pure-MMIO symbolic
  routing exists; mixed RAM/ROM/MMIO symbolic addresses remain fail-closed.
  Loop-summary × FIFO hard-rejects; bounded summarized emission unsupported.
  Before authorization: every memory-access family routes or produces an
  explicit unsupported predicate. Architecture / result / certificate versions
  for the schema-v2 obligation shape are ``broadway-ppc32-be-v36`` / result
  format ``16`` / certificate ``11`` (v35 and earlier rejected). P1 cache
  revalidation, per-side coverage, and digest recomputation are required
  gates; ``memory-bus`` is cleared from ``UNSUPPORTED_FOR_EQUIVALENT``.
- **Memory-bus discharged obligation (Track A):** `status=discharged` requires
  schema v2 attestations — `bus_spec_sha256`, per-side unsupported-access
  (`result=unsat` + `query_sha256` + solver metadata over a nonempty
  `terminals` list where every child is `result=unsat` /
  `inconclusive=false` with valid solver metadata (aggregate binds complete
  terminal records), **or** vacuous `status=vacuously-discharged` /
  `reason=no-unsupported-predicates` with `cfg_trace_sha256` +
  `access_coverage_sha256`), register-bank / FIFO theory blocks (or explicit
  `none`) with exact MMIO `device_id` coverage and no register/FIFO overlap,
  `device_state_in_compare`, and per-side `access_coverage.status=complete`
  (``observed`` is not sufficient for discharge). Empty CFG predicates must
  **not** be silently rewritten as UNSAT; vacuous discharge is valid only with
  coverage attestation. Optional `bus_spec_canonical` lets JSON-only
  validators recompute `bus_spec_sha256` without a live bus.
  `require_equivalent_ready` demands `status=discharged` and
  validator pass. Weak / forged algorithm-regions-only JSON fails closed;
  discharged status is engine-generated via `build` + `enrich`, never trusted
  from caller JSON alone for the live proof path.
- **Memory-bus access families (Track B):** under ``memory_bus=``, every
  memory-touching opcode family either routes through Concrete/Symbolic bus
  helpers or raises a well-known ``ExecutionInconclusive`` /
  unsupported-access reject — never a silent RAM fallback. Coverage API:
  ``bus_access.record_bus_access_family`` / ``last_bus_access_coverage``
  (families + rejections + ``access_coverage_sha256``) feeds Track A
  ``access_coverage`` attestations. P1 requires **per-side** coverage
  (original vs candidate), not a single last-execution global snapshot.

  | Family | Policy |
  |---|---|
  | Integer load/store | Routed (1/2/4) |
  | Scalar FP (`lfs`/`lfd`/`stfiwx`/…) | Routed; **64-bit = two ordered BE 32-bit bus transactions** (atomic width-8 MMIO unsupported) |
  | PSQ | **Rejected on MMIO** (`memory-bus-unsupported-access-family:psq-mmio`); RAM/ROM still execute |
  | `lmw`/`stmw` | Routed as successive width-4 accesses |
  | `dcbz`/`dcbz_l` | **Rejected when the 32-byte block may touch MMIO**; RAM zeroes via width-1 bus stores |
  | Fixed EABI save/restore helpers | Routed; FPR helpers use the same split-64 policy |

- **Memory bus (opt-in Tier C):** `memory_bus.py` routes concrete 1/2/4-byte
  loads/stores through ``AddressSpace`` regions to RAM backing
  (``ConcreteMemory``), immutable ROM images, or live MMIO ``DeviceModel``
  instances keyed by ``device_id``. Multi-region spans, unmapped addresses,
  unsupported widths, missing devices, and ROM writes fail closed
  (``BusOutcome``). Pass ``memory_bus=`` to ``execute_cfg`` with ``ConcreteOps``
  (full routing) or ``SymbolicOps`` (MMIO/FIFO via ``SymbolicBusState``; RAM/ROM
  stay on the symbolic array + ROM constraints). Default proofs keep
  unconstrained ``ConcreteMemory``.
- **ELF data sections:** `list_allocatable_sections` / `extract_allocatable_section`
  expose SHF_ALLOC PROGBITS/NOBITS (including `.rodata` / `.data`) and attach
  REL/RELA entries (notably `R_PPC_ADDR32`) for jump-table census and later
  closure proofs. Text extraction behavior is unchanged.
- **Jump-table image hydration:** `jump_table_image.hydrate_jump_table` reads
  concrete ADDR32 table words from linked ELF (preferred) or DOL data sections.
  Per-TU `.o` files usually retain unresolved ADDR32 slots (see
  `docs/ppc_equiv_work/27-elf-data-reloc-census.md`), so linked hydration is
  required for most Xenoblade jump tables. Object-file hydration is allowed
  only when the slice has no unresolved ADDR32 relocs; otherwise the helper
  fails closed. Not yet wired into the equivalence engine.
- **Jump-table case pairing:** `jump_table_pairing.pair_jump_table_cases` aligns
  original and candidate table words by **logical case index** (`case-0`,
  `case-1`, …), not by absolute target-address equality. Entry-count mismatch
  fails closed. Obligation builders map each paired case to side-specific
  indirect-target payloads for readonly-image + closure proofs.

### Contracts

| Contract | Compared state |
|---|---|
| `auto` | `ppc-eabi` plus persistent non-ABI state written by either side |
| `ppc-eabi` | GPR r1/r2/r3/r4/r13–r31, FPR f1/f14–f31 (both lanes), CR2–CR4, memory |
| `ppc-eabi-fp` | `ppc-eabi` plus FPSCR |
| `strict` | All modeled state |
| `live-out` | Over-approximation of every written component |
| `manual` | Explicitly listed observables |

- Memory is always compared when selected.
- The `auto` contract resolves dynamically from both implementations' write
  effects. Volatile ABI scratch (temporary GPRs/FPRs, volatile CR fields, XER,
  LR, CTR) is never added. After CFG exploration, auto is refined with
  symbolic terminal≠initial writes for `AUTO_PERSISTENT_OBSERVABLES` so a
  mis-tabled opcode cannot silently omit a persistent observable.
- Symbolic FP `EQUIVALENT` under `allow_nan=True` additionally requires a
  NaN-freedom discharge (`nan_freedom.enforce_nan_freedom`): Z3's single-NaN
  theory cannot certify Broadway NaN payload identity.
- **Indirect-branch observables:** matched `indirect-branch` (`bctr` /
  unlinked `bcctr`) terminals compare the contract observable set **union**
  the full PowerPC EABI outgoing-argument registers (`r3`–`r10`, `f1`–`f8`
  and `.ps1` lanes) when no `AbiShape` is attached (fail-closed default). At a
  tail call those registers are live outgoing arguments to the callee, not
  dead return halves. MWCC virtual thunks must scratch the destination in a
  non-arg volatile (`r11`/`r12`) or otherwise preserve argument registers;
  using `r4`–`r10` / `f1`–`f8` as CTR scratch is correctly `NOT_EQUIVALENT` to
  an `r12` thunk that preserves them. `exit.target` (CTR) and `r3` (adjusted
  `this`) remain compared.
- **`AbiShape` (opt-in):** an explicit shape on `EquivalenceContract` may omit
  `r4` on return/fallthrough when `returns_i64=False`, omit `f1`/`f1.ps1` when
  `returns_float=False`, and truncate the outgoing-arg union on
  `indirect-branch`/`call-indirect` to the first `outgoing_gpr_args` /
  `outgoing_fpr_args` registers (defaults **8**/`8`). Inference
  (`abi_infer.infer_abi_shape`) narrows only for matching simple vtable
  dispatches that never touch `r4`–`r10`/`f1` (CTR scratch in `r11`/`r12`).
  A mangled exact param blob `Fv` may annotate that structural narrow; it
  never overrides a body that touches outgoing-arg registers (MWCC often
  still passes hidden `r4+` under shortened `…Fv` names). Nested encodings
  such as `FPFv` / `FPCFv` are not treated as void-no-args.

### Calls

- Direct calls (`bl`/`bcl`) with a recorded matched-callee summary are modeled
  as deterministic ABI transitions: inputs are the preserved read set,
  outputs reflect the write set, and nonvolatile EABI state is preserved.
  Summaries may be opaque EABI or precise body-inferred effect sets.
- Same-object inference composes nested precise summaries when every nested
  relocated callee has a non-opaque contract; otherwise it stays
  `nested-call-opaque-eabi`. Validation failures widen required effects rather
  than collapsing to full opaque EABI.
- **Standalone vs co-op callee policy:**
  - Standalone raw-block CLI never silently assumes relocated callees: missing
    premises stay inconclusive (or warn). Opt in with
    `--assume-relocated-callees` (opaque EABI) or `--callee-contracts`.
  - Standalone `check-objects` infers matched-callee contracts from both object
    bodies by default (`callee_inference`); disable with
    `--no-infer-matched-callees`.
  - Co-op `cycle` / certificate issuance requires a current
    `equivalence_certificate` for every direct callee (or MWCC save/restore
    helper). Certificates are re-attested from current object bytes before use;
    stale or missing callee certificates yield
    `INCONCLUSIVE_UNVALIDATED_CALLEE`.
- Without a recorded summary, the call is inconclusive.
- Opaque / effect summaries are **definedness-monotonic**: `call_token` always
  depends on entry `valid` and `invalid_reason`, and `_apply_call_summary`
  preserves the caller's first invalid reason (a callee cannot resurrect
  validity or replace an earlier reason with an unconstrained UF result).
- A SAT model that involves an opaque summary is `INCONCLUSIVE_ABSTRACTION`,
  not `NOT_EQUIVALENT`.
- MWCC `_savegpr_*`/`_restgpr_*`/`_savefpr_*`/`_restfpr_*` helpers use fixed
  EABI r11-relative register load/store ranges.
- Matched callees are assumed to be location-independent EABI functions: the
  absolute link-register return address is not a semantic input.
- Proof JSON and certificates record each used premise under
  `callee_contracts.<name>.source` (for example `opaque-eabi`,
  `inferred:…`, `certified:<sha256>`).

### Floating point

- Rounding mode: nearest-even (`RN=00`), enforced via `FloatingPointDomain`
  (`rounding_modes`) constraining `FPSCR[0:1]`.
- **FPSCR.NI (PR17 / Wave 4 Track B):** NI is read from live FPSCR state
  (bit 2). Default `require_ni_zero=true` still constrains `NI=0`. With
  `require_ni_zero=false`, Broadway flush-to-zero is modeled for
  `fp_outcome.NI_SUPPORTED_OPS` (scalar SoftFloat oracle ops plus Wave 3
  paired oracle subset): denormal operands → signed zero (Table 2-24);
  denormal / sub-single-normal results → signed zero (Table 2-25 /
  Dolphin `ForceSingle`). Proofs that allow NI and contain an NI-affected
  unsupported opcode fail closed as `INCONCLUSIVE_UNSUPPORTED` (no silent
  IEEE widening). Recorded under `floating_point_domain.ni` /
  `ni_supported_opcodes`. Still **Tier C** only.
- Traps: default domain keeps traps disabled (`traps_enabled=false`, coverage
  assumed `traps-disabled`). **PR18 (Wave 5 Track D):** `traps_enabled=true`
  delivers VE/ZE/OE/UE/XE program exceptions for SoftFloat scalar
  `TRAP_DELIVERY_SUPPORTED_OPS` and Wave-3 paired-oracle
  `TRAP_DELIVERY_PAIRED_OPS` (reuse `program-exception` / `0x700` /
  `_exception_entry` SRR0/SRR1/MSR; scalar destination suppression under
  enables; paired writeback remains unconditional). ConcreteOps latches
  OX/UX/XX from SoftFloat outcomes. **FEX already-set re-trap:** each
  instruction that raises an enabled exception delivers a program interrupt
  even when FEX was already 1 (precise policy; MSR FE0/FE1 imprecise modes
  deferred — `traps_enabled` assumes precise). Incomplete opcodes (estimates,
  compares, conversions, non-oracle paired) fail closed as
  `ExecutionInconclusive` / `INCONCLUSIVE_UNSUPPORTED`.   SymbolicOps still
  constrains OE/UE/XE clear (OX/UX/XX not SMT-modeled). When `fpscr` — or an
  FPR/CR field that received a sticky projection via `mffs` / `mcrfs`, or
  record-form FP Rc copying FPSCR[FX,FEX,VX,OX] into observed `cr1` / `cr` —
  is a compared observable after non-bitwise FP arithmetic on *differing*
  implementations, EQUIVALENT is demoted to `INCONCLUSIVE_ABSTRACTION`
  (`annotate_fpscr_sticky_incompleteness`). This demotion is independent of
  `allow_nan` (NaN-freedom is a separate gate). Byte-identical implementations
  keep EQUIVALENT (same code ⇒ same hardware stickies) with an honesty
  assumption. Still **Tier C** for promotion when FPSCR arithmetic is in play.
- Finite-input overflow is excluded when `exclude_finite_overflow=true`
  (default) via `_constrain_fp_defined_result` → `FP_DOMAIN_EXCLUDED`.
- Invalid-operation (`VX`) and divide-by-zero (`ZX`) causes are tracked.
- Underflow, inexact, overflow flags are not modeled; requesting
  `model_underflow_flag` / `model_inexact_flag` fails closed.
- Fused-single and paired-fused proofs require finite operands to be exact
  binary32 values expanded in FPRs when
  `fused_input_domain=exact-expanded-binary32` (default). Use
  `unrestricted` to skip that origin constraint; other values fail closed.
- Optional operand exclusions: `allow_nan`, `allow_infinity`, and
  `allow_subnormal` (defaults true) clear `valid` with `FP_DOMAIN_EXCLUDED`
  when false.
- **NaN-payload freedom (H2):** Z3's SMT-LIB FP theory collapses every NaN
  payload to a single abstract NaN, so `fpToIEEEBV` cannot distinguish
  Broadway sequences that differ only in NaN payload bits. When a proof uses
  FP and `allow_nan=True`, `nan_freedom.enforce_nan_freedom` runs an extra
  query after the divergence query returns `unsat` (would-be `EQUIVALENT`):
  it is `sat` iff some feasible, mutually-defined terminal pair carries a NaN
  in a *compared* FPR/PS1 lane whose value is produced through the FP theory
  **and** differs syntactically between the two implementations. `sat` demotes
  to `INCONCLUSIVE_UNSUPPORTED` (`nan-payload-unmodeled`); `unknown`/timeout
  fails closed; `unsat` keeps `EQUIVALENT` and records the
  `nan-freedom:proven` assumption. Two exemptions keep the gate precise:
  syntactically identical results (same deterministic function of shared
  inputs — e.g. `f(x) ≡ f(x)` self-proofs) and pure bit-vector values
  (`fmr`/`fabs`/`fneg`/`fnabs` sign moves, raw `lfd` loads) whose payload bits
  are already compared exactly by the divergence query. With `allow_nan=False`
  the domain already excludes NaN inputs via `FP_DOMAIN_EXCLUDED`, so the gate
  is skipped (recorded as `nan-freedom:domain-allow_nan=false`).
- Paired-single uses independent binary32 lanes with Force25 multiplication,
  accumulated lane exceptions, and unconditional paired writeback under enabled
  exceptions.
- `fsqrt`/`fsqrts` encodings (later PowerPC revisions) are reserved on
  Broadway and rejected by the decoder.
- CLI `--fp-domain` / coop `floating_point_domain` pass through to the engine;
  unsupported overrides never silently ignore the requested domain.
- **Promotion:** all FP proofs remain confidence **Tier C** under current policy.
  `floating_point_domain.coverage.status` distinguishes ``proven`` SMT-encoded
  restrictions from ``assumed`` ones (for example trap exclusion); neither tier
  is eligible for automatic `EQUIVALENT_MATCH` promotion until a future gate
  explicitly reopens FP to Tier A/B.
- **SoftFloat oracle scaffold:** `tools/ppc_equivalence/fp_oracle.py` holds a
  bit-level, host-float-free oracle for a small scalar-op subset (`fadd`/`fadds`/
  `fmul`/`fmuls`/`fsub`/`fsubs`/`fdiv`/`fdivs`/`fmadd`/`fmadds`/`fmsub`/`fmsubs`/
  `fnmadd`/`fnmadds`/`fnmsub`/`fnmsubs`). **ConcreteOps** routes those sixteen
  scalar opcodes through the oracle. The integer-significand path models finite
  normals/zeros, subnormal operands/results, ±Inf arithmetic, quiet/signaling
  NaN propagation, division by zero (±Inf), and overflow (±Inf). Default
  `exclude_finite_overflow` domain exclusions continue to mark overflowed
  finite→Inf results via `FP_DOMAIN_EXCLUDED`. Force25 for single fused forms is
  applied by semantics before the oracle. Negative fused forms negate finite
  results only (NaN payloads are never sign-flipped). **Still fail-closed**
  (`OracleUnimplementedError` → `ExecutionInconclusive`): Broadway single-FMA
  midpoint-tie residues with a nonzero addend, and fused near-cancellation with
  sticky residue. SymbolicOps, paired-single lanes, and all other FP ops still
  use host float or Z3. The oracle computes partial sticky indicators (`XX`
  inexact, `ZX`/`OX`/`VX` hints, `FPRF` class nibble) in `FpOracleResult` but
  does **not** latch them into FPSCR (`floating_point_domain.fpscr_flags` marks
  oracle-scaffold vs assumed bits); architectural FPSCR sticky bits for the
  sixteen routed opcodes still come from the semantics exception path. This
  remains **Tier C** only and is **not** a promotion path to Tier A/B.
- **Paired-single oracle (Wave 3 Track B, ConcreteOps):** ``ps_add``/``ps_sub``,
  ``ps_mul``, and ``ps_madd``/``ps_msub``/``ps_nmadd``/``ps_nmsub`` execute two
  scalar single-lane oracle outcomes merged by ``combine_paired_outcomes`` in
  ``fp_outcome.py`` (PS0 FPRF, accumulated VX subcauses, unconditional lane
  writeback, record-form CR1). Other paired families stay on the legacy
  semantics path; ``SymbolicOps`` is unchanged. Still **Tier C** only.
- **FPSCR.NI flush (Wave 4 Track B / PR17):** the same scalar SoftFloat +
  paired-oracle opcode set applies Broadway denormal operand/result flush when
  ``FPSCR.NI=1``. Deferred NI coverage: estimates, ``frsp``, converts, compares,
  single stores, and non-oracle paired families. **NI×trap (Wave 5):** when
  ``require_ni_zero=false`` and ``traps_enabled``, flush-to-zero composes with
  trap delivery for the intersection of NI-supported and trap-supported
  opcodes; NI-unsupported ops with NI possibly set stay inconclusive (no IEEE
  widening).
- **Unified `FPOutcome` scaffold (Track C):** `tools/ppc_equivalence/fp_outcome.py`
  defines `FPExceptionFlags` / `FPOutcome` plus SoftFloat and bits-API adapters
  (`outcome_from_oracle`, `oracle_from_outcome`, `outcome_from_result_bits`).
  Existing SoftFloat / ConcreteOps / SymbolicOps call sites are unchanged.
  SoftFloat now covers the Inf/NaN/div0/subnormal/overflow domains that host
  float previously modeled for ConcreteOps scalar paths. **Deferred:** Fraction
  / rational exact cross-check oracle, Broadway single-FMA midpoint residual
  with nonzero addend, native SymbolicOps/`FPOutcome` producers. **PR18 trap
  delivery:** `fp_traps.py` resolves VE/ZE/OE/UE/XE trap vs continue from
  `FPOutcome` + enable bits; CFG forks a `program-exception` terminal when
  `traps_enabled` (scalar SoftFloat + paired-oracle sets). SoftFloat OX/UX
  sticky latch on ConcreteOps is domain-gated: OX when
  `exclude_finite_overflow=False` or traps are enabled; UX only under
  `traps_enabled` (`model_underflow_flag` remains unsupported). SoftFloat
  `inexact` alone does **not** raise `FPSCR.XX` (Force25 / single-mul
  intermediates are not Broadway XX); under `traps_enabled` it still feeds XE
  enable matching. Estimates/compares/converts and MSR FE0/FE1
  modes remain deferred / fail-closed.

### Relocations

| Relocation | Constraint |
|---|---|
| `R_PPC_REL24` | Word-aligned target, signed 24-bit range |
| `R_PPC_REL14` | Word-aligned target, signed 14-bit range |
| `R_PPC_ADDR16_LO` | Unconstrained low 16 bits |
| `R_PPC_ADDR16_HI` | Unconstrained high 16 bits |
| `R_PPC_ADDR16_HA` | Unconstrained high 16 bits with carry adjustment |
| `R_PPC_EMB_SDA21` | Within signed-16 range of r2 (small data) or r13 (small data 2) |

- Unsupported relocation types fail closed.
- Symbolic addresses are shared across both objects: unresolved relocations with
  the same symbol name receive the same symbolic value.
- The layout-feasibility solver checks that an assignment exists satisfying all
  relocation constraints simultaneously. An impossible layout is
  `INCONCLUSIVE_LAYOUT`, never equivalence.
- Co-op `_prove_bytes` extracts `_SDA_BASE_` / `_SDA2_BASE_` (or section-end
  fallbacks) from the linked ELF when present, passes them as
  `sda_bases` / `symbol_addresses` into jump-table auto, and constrains initial
  `r13` / `r2` via `initial_gpr_bindings`. Absolute `relocation_bindings` are
  not forced for unlinked HA/LO proofs so typeinfo labels keep symbolic
  identity across DOL vs ELF placements.
- **Object-base MEM1 / RAM-only strip (opt-in):** `object_base.py` helpers plus
  engine `initial_gpr_ranges` constrain symbolic GPRs (typically `r3`) into
  MEM1 / profile RAM with unsigned `ULE` bounds, recorded as
  `object-base-mem1:rN:[lo,hi]` assumptions. Separately, when a hardware bus
  includes MMIO but neither side can form MMIO addresses from immediates /
  known high-halves (`0xCC00`/`0xCD00`) or MMIO-named relocs,
  `ram_only_memory_bus` may strip MMIO regions/devices for that probe.
  Coop knobs: `object_base_mem1`, `ram_only_when_no_mmio` (default false).

### Solver and timeout

- One shared wall-clock `Deadline` covers all solver phases.
- The default solver runs first. `sat` or `unsat` returns immediately.
- On `unknown`, a bit-blast fallback (`simplify` → `bit-blast` → `sat`) runs
  with the remaining deadline.
- Every phase is recorded as a `SolverPhase` with name, result, and elapsed ms.
- Cache entries include `architecture` and `result_format`; stale entries are
  rejected at read time.

## Traceability

| Soundness claim | Implementation | Tests | Result field |
|---|---|---|---|
| Independent per-side stack masking | `engine._private_stack_address`, `engine._memory_difference` | `test_private_stack_memory.py` | `memory_scope.private_stack.masking_semantics` |
| Stack wrap / oversized frame rejected | `semantics.MAX_PRIVATE_STACK_DEPTH`, `execute_cfg` layout update | `test_upward_stack_*` | `status=INCONCLUSIVE_LAYOUT` |
| Ordinary RAM range assumed | memory-profile constraints | `test_memory_profile.py` | `environment.memory_profile` |
| Fail-closed constrained profiles without ranges | `memory_profile.build_memory_constraints` | `test_memory_profile` fail-closed cases | `environment.fail_closed_empty_ranges`, `INCONCLUSIVE_LAYOUT` |
| No silently truncated loop | `semantics.execute_cfg` visit bound | loop-support tests | `unsupported`, `status=INCONCLUSIVE_UNSUPPORTED`, `limits.max_loop_iterations` |
| Bounded constant-trip loops | `semantics.execute_cfg` | loop-support tests | `status=EQUIVALENT` / `NOT_EQUIVALENT` |
| Cache-disabled `dcbz`/`dcbz_l` → `CACHE_DISABLED` | `semantics._constrain_valid` | `test_definedness:test_dcbz_cache_disabled_sets_reason` | `state.invalid_reason` |
| Privileged `mfmsr`/`mtmsr`/`mfsr`/`mtsr` → `PRIVILEGED_INSTRUCTION` | `semantics._constrain_valid` | `test_definedness:test_mfmsr_user_mode_sets_reason` | `state.invalid_reason` |
| Privileged `mfspr`/`mtspr` (SRR*/aux/TB) → `PRIVILEGED_INSTRUCTION` | `semantics._constrain_valid` | `test_definedness:test_mfspr_hid0_user_mode_sets_reason` | `state.invalid_reason` |
| FP domain exclusion → `FP_DOMAIN_EXCLUDED` | `semantics._constrain_fp_defined_result` / `_constrain_fp_value_domain` | `test_definedness:test_fadds_finite_overflow_sets_fp_domain_reason`, `test_fp_domain` | `state.invalid_reason`, `floating_point_domain` |
| Nearest-even FP rounding | `semantics` FP operations + domain validate | FP fixture corpus, `test_fp_domain` | `assumptions`, `floating_point_domain` |
| Unsupported FP domain fails closed | `FloatingPointDomain.validate`, `engine.check_equivalence` | `test_fp_domain` | `status=INCONCLUSIVE_UNSUPPORTED` |
| Current callee certificate chain | `_re attest_certificate_tree`, `equivalence_certificate_error` | `test_targets.test_validation_rejects_stale_certificate_dependency` | `equivalence_certificate.callees` |
| Precise composed callee summaries | `callee_inference.infer_matched_callee_contracts` | `test_callee_composition.py` | `callee_contracts.*.source` |
| CLI no silent relocated assume | `cli._run_check` | `test_callee_composition.py` | stderr warning / inconclusive |
| Deadline covering CFG, constraints, and solve | `deadline.Deadline`, `semantics.execute_cfg`, `engine.check_equivalence` | `test_deadline.py` | `status=INCONCLUSIVE_TIMEOUT`, `solver.phases` |
| Cache includes architecture model | `_cache_key`, `_cache_get` | `test_targets` versioning tests | cache JSON |
| Jump-table obligation payloads bind cache/cert identity | `proof_request_identity`, `_cache_key`, `equivalence_certificate_hash` | `test_jump_table_identity.py` | `source_hash`, `proof_features`, obligation blocks |
| Alignment constraint | `semantics` load/store `valid` predicate | `test_symbolic_relocations` alignment tests | `assumptions` |
| No vacuous proof through impossible layout | `engine.check_equivalence` layout feasibility | layout tests | `status=INCONCLUSIVE_LAYOUT` |
| Definedness-preserving partial equivalence | `engine._terminal_difference` | definedness tests | `mismatch.kind=definedness` |
| Invalid-reason comparison when both invalid | `engine._terminal_difference` | definedness tests | `mismatch.name=invalid-reason` |
| Both-invalid still compares observables/memory | `engine._terminal_difference` | `test_definedness.BothInvalidObservableTests` | `status=NOT_EQUIVALENT` when pre-fault stores diverge |
| InvalidReason tracked per instruction | `semantics._constrain_valid` | definedness tests | `state.invalid_reason` |
| Call summary preserves first invalid reason | `semantics.call_token`, `_apply_call_summary` | `test_definedness.CalleeSummaryDefinednessTests` | `state.invalid_reason` |
| INCONCLUSIVE_UNMODELED_EXCEPTION (reserved) | `result.ProofStatus` | — | `status` |
| Exit-kind and exit-target comparison | `engine._terminal_difference` | `test_checker` control-flow tests | `mismatch.exit_kind`, `mismatch.exit_target` |
| Private stack disabled after call | `semantics._apply_call_summary` (opaque and fixed helpers) | `test_call_disables_*`, `test_savegpr_*` | `memory_scope.private_stack.*.disabled_reasons` |
| Private stack disabled after r1 escape | `semantics._mark_stack_pointer_escape`, `stack_escape.apply_compared_register_publish_escape` | escape / alias / stmw tests, `RegisterPublishStackEscapeTests` | `memory_scope.private_stack.*.disabled_reasons` |
| Architecture model versioned | `result.ARCHITECTURE_MODEL` | `test_targets.test_old_architecture_model_certificate_rejected` | Certificate `architecture` |
| Result format versioned | `result.RESULT_FORMAT` | `test_targets.test_old_result_format_certificate_rejected` | Certificate `result_format` |
| Certificate version accepted | `targets.EQUIVALENCE_CERTIFICATE_VERSION` | `test_targets.test_wrong_certificate_version_rejected` | Certificate `version` |
| Promotion policy gated | `coop.lib.equivalence_policy.classify_for_promotion` | policy tests | `PromotionDecision.blockers` |
| Capability-assurance Wave 5 canary | `capability_manifest.authoritative.json`, `build_capability_assurance_audit` | `tools/coop/tests/test_capability_assurance_rollout.py` | `PromotionDecision.assurance_audit`, `capability-assurance-audit:` warnings |
| Concrete sampling secondary defense | `engine.run_concrete_sampling`, `--concrete-samples` | `test_concrete_sampling.py` | `concrete_sampling` (never a certificate) |

## Result fields

| field | type | meaning |
|---|---|---|
| `status` | string | One of the `ProofStatus` values |
| `architecture_model` | string | Canonical model identifier |
| `format` | int | Result schema version (`RESULT_FORMAT`) |
| `contract` | string | Contract preset or `manual` |
| `contract_resolution` | object | Auto-contract added observables (when applicable) |
| `observables` | string[] | Compared state components |
| `assumptions` | string[] | Declared environmental and domain restrictions |
| `solver.result` | string | `sat` / `unsat` / `unknown` |
| `solver.phases` | object[] | Per-phase name, result, elapsed ms |
| `limits` | object | `max_instructions`, `max_paths`, `max_loop_iterations` |
| `environment` | object | Memory profile, ranges, fail-closed flags |
| `memory_scope` | object | Private-stack masking status per implementation |
| `floating_point_domain` | object | FP domain + proven/assumed/unsupported coverage |
| `mismatch` | object | First observed difference (kind, name, values) |
| `counterexample` | object | Concrete SAT witness (initial state + relocations) |
| `repair_hint` | object | First concrete divergence hint on `not_equivalent` |
| `concrete_sampling` | object | Optional ConcreteOps secondary-defense report (`--concrete-samples`); never an equivalence certificate |
| `assumed_callees` | string[] | Callee premises actually used in the proof |
| `callee_contracts` | object | Per-callee `source` / `reads` / `writes` |
| `abstractions` | string[] | Reasons for `INCONCLUSIVE_ABSTRACTION` |
| `opcodes_used` | string[] | Opcodes enumerated for ledger confidence tiering |
| `engine_hash` | string | SHA-256 of declared engine trust-boundary tree |
| `certifier_hash` | string | SHA-256 of coop certifier/policy trust-boundary sources |
| `source_hash` | string | Canonical hash of proof-request inputs |
| `proof_features` | string[] | Declared proof features used by this result (optional) |
| `address_space` | object | Obligation block for `readonly-image` (optional) |
| `indirect_targets` | object | Obligation block for `indirect-target-closure` (optional) |
| `git_commit` / `git_dirty` | string / bool | Commit identity at proof time; `git_dirty` is **trust-boundary scoped** (engine `ENGINE_SOURCE_PATTERNS` + certifier `CERTIFIER_SOURCE_PATHS` only — unrelated decomp dirt does not set dirty) |
| `platform` / `python_version` / `z3_version` / `capstone_version` | string | Runtime identity |

## Proof features and obligations

Future soundness extensions (readonly image regions, indirect-target closure,
and similar) must declare which features were used and attach a matching
obligation block. The schema is:

```json
{
  "proof_features": ["readonly-image", "indirect-target-closure"],
  "address_space": {},
  "indirect_targets": {}
}
```

Rules (enforced by `tools.ppc_equivalence.proof_features`):

- **PR0 / Track D memory-bus gate cleared:** `memory-bus` is removed from
  `UNSUPPORTED_FOR_EQUIVALENT`. EQUIVALENT requires engine-generated
  `memory_bus.status=discharged` with schema v2 attestations under
  `require_equivalent_ready` and `validate_memory_bus_obligation_strict`
  (recomputed digests, per-side coverage, theory/spec binding; vacuous
  unsupported-access with coverage digests; forged algorithm/regions-only
  JSON fails closed). Cache probes return the re-gated proof status after
  demotion. **PR7 / Wave 5 Track B:** `relational-induction`, `affine-loop-summary`, and
  `memory-loop-summary` may authorize `EQUIVALENT` only under strict
  discharged obligations:
  - `relational-induction`: all five blocks carry independent UNSAT digests
    (`result=unsat`, `query_sha256`, solver metadata); SAT/unknown/timeout
    never discharges. Compare-affine uses counter-descending termination.
    Bulk+remainder attaches `discharged` only after identity + RangeWrite
    evidence; otherwise `pending`.
  - `affine-loop-summary`: `status=discharged` with matching `summary_sha256`,
    `relational_companion=discharged`, and a companion
    `relational-induction` feature also discharged. CTR or compare-affine
    closed-form application (`coverage=applied`) alone never authorizes.
  - `memory-loop-summary`: schema version exactly `2`, algorithm
    `constant-stride-store-set-v3`, both `original` and `candidate` arrays
    present, no legacy flat mirrors / v1 transition-equivalence algorithms,
    and `status=discharged` only when every side entry carries
    `entry_guard.result=unsat` plus
    `refinement.status=discharged` with required blocks (`body_step`,
    `postcondition`, `stack_escape`, `termination`, `footprint`) all
    `result=unsat` under known v2 algorithms (`memory_loop_discharge.py`).
    Each side proves `instructions ≡ summary` independently; structural
    summary equality across sides is not required. Closed-form recognition /
    `applied` alone never authorizes; refinement SAT is `INTERNAL_ERROR`.
    Bounded-remainder expansions stay `applied`.
  Jump-table coverage and no-write use independent UNSAT
  discharge (`discharge.py`) with remainder terminals retained on BCCTR
  expansion; obligation schema v2 carries coverage/no_write digests.
  Architecture / result / certificate for the memory-loop refinement shape:
  `broadway-ppc32-be-v37` / format `17` / certificate `12` (capability-assurance
  schema Wave 1). Certificates under `broadway-ppc32-be-v36` (and earlier
  rejected models) are stale.
- **Wave 5 Track B blockers kept documented (not freeze-worthy for closed-form):**
  bounded-remainder expansions stay `applied` (not discharged). Bulk+remainder
  without a shared constant-value `RangeWrite` stays `pending`.
  `ReadonlyWordEvidence` now records optional per-side `artifact_sha256` /
  `image_kind` / `section` provenance (DOL for original, ELF for candidate,
  never conflated), folded into the `memory_loop_readonly` identity premise
  (`per-word-image-v1`) that binds `_cache_key` and `proof_request_hash`;
  closed-form `li`/`mtctr` paths and explicit `readonly_words_sha256` binding
  cover current discharge.
- `proof_features` is mandatory whenever a proof relies on a feature.
- Each listed feature requires its obligation key: `readonly-image` →
  `address_space`, `indirect-target-closure` → `indirect_targets`.
- Obligation blocks without a matching `proof_features` entry are rejected
  (optional sub-fields alone cannot forge completeness).
- Unknown features, malformed types, missing obligation blocks, or reserved
  features that are not yet implemented fail closed: an `EQUIVALENT` result is
  demoted to `INCONCLUSIVE_UNSUPPORTED` and stored certificates are rejected.
- When no features are declared and no obligation blocks are present, legacy
  proofs and certificates remain valid (result format and architecture model are
  unchanged until a feature can actually return `EQUIVALENT`).

### Cache and certificate identity

Once jump-table proof features are declared, their obligation payloads are part
of durable identity:

- **`source_hash`** — `proof_request_hash` / `proof_request_identity` include
  sorted `proof_features` and canonical `address_space` / `indirect_targets`
  blocks when present. Changing `image_sha256`, target sets, `algorithm`
  versions, or other obligation fields changes the hash; omitting all three
  leaves legacy `source_hash` unchanged.
- **Coop proof cache** — `_cache_key` binds the same optional fields plus
  `engine_hash` and `certifier_hash`, so cached `EQUIVALENT` results cannot be
  reused across different table images or target closures.
- **Semantic certificates** — when a proof result carries these fields,
  `_build_equivalence_certificate` serializes them into the signed payload
  before `certificate_sha256` is computed.

These bindings apply whenever the fields are declared. Jump-table features are
accepted for `EQUIVALENT` under `JumpTableProofContext` on architecture model
`broadway-ppc32-be-v24` (certificate schema v3, result format 10); identity
wiring ensures stale cache entries and certificates fail closed when obligation
payloads change.
