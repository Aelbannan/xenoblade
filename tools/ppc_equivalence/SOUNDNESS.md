# Soundness specification

<!-- BEGIN GENERATED PPC_EQUIVALENCE_VERSION -->

- Architecture model: `broadway-ppc32-be-v24`
- Result format: `10`
- Certificate format: `3`

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
  condition becomes concrete.
- Indirect branches (`bclr`/`bcctr` without a known target) are unsupported.
  Jump-table pattern recognition (`jump_table.find_jump_table_candidates`) is
  descriptive only: matching the `cmplwi` / shift / `lwzx` / `mtctr` / `bctr`
  shape does not prove equivalence. Without a `JumpTableProofContext`, the
  engine fail-closes otherwise-matching jump-table functions to
  `INCONCLUSIVE_UNSUPPORTED`. With a context (explicit or auto-built by
  `jump_table_auto.try_auto_jump_table_context` from `lis`/`addi` + linked
  DOL/ELF hydration), CFG expands each enumerated CTR target, ROM bytes are
  pinned with alias-safe no-write constraints, and `readonly-image` /
  `indirect-target-closure` obligations are attached
  (`jump_table_obligations.py`). Retail/candidate handlers are paired by
  logical case index (`jump_table_pairing.py`), not absolute address equality.
  Coop `_prove_bytes` attempts auto-context when a linked image is available.
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
    the reason code, value comparison is suppressed.  No claim is made about
    the relative exception class or hardware exception delivery.
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
  r1-derived value is stored to memory (direct `r1`, register aliases such as
  `mr`/`ori`, computed pointers such as `addi rD,r1,imm`, or `stmw` ranges that
  include `r1`). Escape detection is a free-variable check for `input.gpr.r1`
  in the stored expression (fail-closed; not a full taint lattice).
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
  bounded ranges via `require_bounded_ram`.
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
- **AddressSpace router (scaffold):** `address_space.py` models ordered,
  non-overlapping regions (`ram` / `rom-image` / `mmio` / `unmapped`) with
  concrete `classify` / `classify_range`. ROM images can be built from ELF
  allocatable PROGBITS via `elf_symbols.rom_image_from_allocatable_section`.
  Symbolic accesses that may span regions are intended to path-split or fail
  inconclusive. Not yet proof-producing: no `EQUIVALENT` path binds AddressSpace
  obligations into the solver or promotion gate.
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
  LR, CTR) is never added.

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
  (`require_ni_zero` + `rounding_modes`) constraining `FPSCR[0:2]`.
- `FPSCR.NI=0` (non-IEEE mode disabled); configs with `require_ni_zero=false`
  fail closed as `INCONCLUSIVE_UNSUPPORTED`.
- Traps: disabled for all FP exceptions (assumed; trap delivery is not
  modeled). `traps_enabled=true` fails closed.
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
- Paired-single uses independent binary32 lanes with Force25 multiplication,
  accumulated lane exceptions, and unconditional paired writeback under enabled
  exceptions.
- `fsqrt`/`fsqrts` encodings (later PowerPC revisions) are reserved on
  Broadway and rejected by the decoder.
- CLI `--fp-domain` / coop `floating_point_domain` pass through to the engine;
  unsupported overrides never silently ignore the requested domain.

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
| InvalidReason tracked per instruction | `semantics._constrain_valid` | definedness tests | `state.invalid_reason` |
| Call summary preserves first invalid reason | `semantics.call_token`, `_apply_call_summary` | `test_definedness.CalleeSummaryDefinednessTests` | `state.invalid_reason` |
| INCONCLUSIVE_UNMODELED_EXCEPTION (reserved) | `result.ProofStatus` | — | `status` |
| Exit-kind and exit-target comparison | `engine._terminal_difference` | `test_checker` control-flow tests | `mismatch.exit_kind`, `mismatch.exit_target` |
| Private stack disabled after call | `semantics._apply_call_summary` (opaque and fixed helpers) | `test_call_disables_*`, `test_savegpr_*` | `memory_scope.private_stack.*.disabled_reasons` |
| Private stack disabled after r1 escape | `semantics._mark_stack_pointer_escape` | escape / alias / stmw tests | `memory_scope.private_stack.*.disabled_reasons` |
| Architecture model versioned | `result.ARCHITECTURE_MODEL` | `test_targets.test_old_architecture_model_certificate_rejected` | Certificate `architecture` |
| Result format versioned | `result.RESULT_FORMAT` | `test_targets.test_old_result_format_certificate_rejected` | Certificate `result_format` |
| Certificate version accepted | `targets.EQUIVALENCE_CERTIFICATE_VERSION` | `test_targets.test_wrong_certificate_version_rejected` | Certificate `version` |
| Promotion policy gated | `coop.lib.equivalence_policy.classify_for_promotion` | policy tests | `PromotionDecision.blockers` |
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
| `git_commit` / `git_dirty` | string / bool | Repository identity at proof time |
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
