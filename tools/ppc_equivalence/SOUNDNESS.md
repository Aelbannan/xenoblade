# Soundness specification

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
- CFG construction enumerates all acyclic paths. Loops and back-edges are
  unsupported: any backward branch terminates the path as inconclusive.
- Indirect branches (`bclr`/`bcctr` without a known target) are unsupported.
- Path count and instruction count are bounded by `max_paths` (default 256) and
  `max_instructions` (default 2048). Exceeding either produces
  `INCONCLUSIVE_UNSUPPORTED`.

### Terminal behavior

A terminal is a triple `(condition, state, exit_kind, exit_target)`:

| Exit kind | Meaning | Target compared |
|---|---|---|
| `return` | `bclr` with LK=0 | None |
| `branch` | `b`/`bc`/`bcctr` with LK=0 | Exit address |
| `fallthrough` | End of block | None |
| `call` | `bl`/`bcl` with recorded callee summary | Next instruction |
| `trap` | `twi` predicate satisfied | Exception vector |
| `sc` | `sc` instruction | Exception vector |
| `rfi` | `rfi` instruction | Return address |

- Exit-kind mismatch is always a difference.
- Exit-target mismatch is compared when both sides have a non-`None` target.
- `call` terminals are produced only when a matched-callee summary is available;
  otherwise the path is inconclusive.

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
  implementation. Masking is disabled when a call executes or an r1-derived
  value is stored to memory.
- Alignment: aligned loads/stores are required for multi-byte accesses.
- Memory-environment profiles (`assumed-ordinary-ram`, `bounded-ordinary-ram`,
  `stack-and-known-globals`, `hardware-aware`) are recorded on every result.
  - `assumed-ordinary-ram`: accesses are unconstrained ordinary RAM (external
    assumption; default).
  - `bounded-ordinary-ram`: when ranges are supplied, every touched address must
    lie in a configured range (no wraparound). Empty ranges remain a soft /
    unconstrained bound for compatibility.
  - `stack-and-known-globals` and `hardware-aware`: **require nonempty ranges**.
    With ranges, they use the same access-within-range constraint builder as
    bounded. Without ranges they **fail closed** (unsat domain →
    `INCONCLUSIVE_LAYOUT`) and never silently degrade to unconstrained RAM.
    `InvalidReason.MEMORY_PROFILE_VIOLATION` (code 10) is the reserved reason
    code for out-of-profile access tagging.
- 32-bit address wraparound is rejected through the layout-feasibility check.
- The initial memory array is shared between both implementations. Private-stack
  bytes are replaced with the common initial byte on each side independently.

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
  as deterministic opaque ABI transitions: inputs are preserved read set,
  outputs reflect write set, and the nonvolatile EABI state is preserved.
- Without a recorded summary, the call is inconclusive.
- A SAT model that involves an opaque summary is `INCONCLUSIVE_ABSTRACTION`,
  not `NOT_EQUIVALENT`.
- MWCC `_savegpr_*`/`_restgpr_*`/`_savefpr_*`/`_restfpr_*` helpers use fixed
  EABI r11-relative register load/store ranges.
- Matched callees are assumed to be location-independent EABI functions: the
  absolute link-register return address is not a semantic input.

### Floating point

- Rounding mode: nearest-even (`RN=00`).
- `FPSCR.NI=0` (non-IEEE mode disabled).
- Traps: disabled for all FP exceptions.
- Finite-input overflow is excluded.
- Invalid-operation (`VX`) and divide-by-zero (`ZX`) causes are tracked.
- Underflow, inexact, overflow flags are not modeled.
- Fused-single and paired-fused proofs require finite operands to be exact
  binary32 values expanded in FPRs.
- Paired-single uses independent binary32 lanes with Force25 multiplication,
  accumulated lane exceptions, and unconditional paired writeback under enabled
  exceptions.
- `fsqrt`/`fsqrts` encodings (later PowerPC revisions) are reserved on
  Broadway and rejected by the decoder.

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
| Ordinary RAM range assumed | memory-profile constraints | `test_memory_profile.py` | `environment.memory_profile` |
| Fail-closed stack/hardware profiles without ranges | `memory_profile.build_memory_constraints` | `test_memory_profile` fail-closed cases | `environment.fail_closed_empty_ranges`, `INCONCLUSIVE_LAYOUT` |
| No unsupported loop | `semantics.execute_cfg` | control-flow tests | `unsupported`, `status=INCONCLUSIVE_UNSUPPORTED` |
| Cache-disabled `dcbz`/`dcbz_l` → `CACHE_DISABLED` | `semantics._constrain_valid` | `test_definedness:test_dcbz_cache_disabled_sets_reason` | `state.invalid_reason` |
| Privileged `mfmsr`/`mtmsr`/`mfsr`/`mtsr` → `PRIVILEGED_INSTRUCTION` | `semantics._constrain_valid` | `test_definedness:test_mfmsr_user_mode_sets_reason` | `state.invalid_reason` |
| Nearest-even FP rounding | `semantics` FP operations | FP fixture corpus | `assumptions`, `floating_point_scope` |
| Current callee certificate chain | `_re attest_certificate_tree`, `equivalence_certificate_error` | `test_targets.test_validation_rejects_stale_certificate_dependency` | `equivalence_certificate.callees` |
| Deadline covering all phases | `deadline.Deadline`, `engine.check_with_portfolio` | `test_deadline.py` | `solver.phases` |
| Cache includes architecture model | `_cache_key`, `_cache_get` | `test_targets` versioning tests | cache JSON |
| Alignment constraint | `semantics` load/store `valid` predicate | `test_symbolic_relocations` alignment tests | `assumptions` |
| No vacuous proof through impossible layout | `engine.check_equivalence` layout feasibility | layout tests | `status=INCONCLUSIVE_LAYOUT` |
| Definedness-preserving partial equivalence | `engine._terminal_difference` | definedness tests | `mismatch.kind=definedness` |
| Invalid-reason comparison when both invalid | `engine._terminal_difference` | definedness tests | `mismatch.name=invalid-reason` |
| InvalidReason tracked per instruction | `semantics._constrain_valid` | definedness tests | `state.invalid_reason` |
| INCONCLUSIVE_UNMODELED_EXCEPTION (reserved) | `result.ProofStatus` | — | `status` |
| Exit-kind and exit-target comparison | `engine._terminal_difference` | `test_checker` control-flow tests | `mismatch.exit_kind`, `mismatch.exit_target` |
| Private stack disabled after call | `semantics._apply_call_summary` | escape tests | `memory_scope.private_stack.candidate.disabled_reasons` |
| Private stack disabled after r1 escape | `semantics._mark_stack_pointer_escape` | escape tests | `memory_scope.private_stack.original.disabled_reasons` |
| Architecture model versioned | `result.ARCHITECTURE_MODEL` | `test_targets.test_old_architecture_model_certificate_rejected` | Certificate `architecture` |
| Result format versioned | `result.RESULT_FORMAT` | `test_targets.test_old_result_format_certificate_rejected` | Certificate `result_format` |
| Certificate version accepted | `targets.EQUIVALENCE_CERTIFICATE_VERSION` | `test_targets.test_wrong_certificate_version_rejected` | Certificate `version` |
| Promotion policy gated | `coop.lib.equivalence_policy.classify_for_promotion` | policy tests | `PromotionDecision.blockers` |

## Result fields

| field | type | meaning |
|---|---|---|
| `status` | string | One of eight `ProofStatus` values |
| `architecture_model` | string | Canonical model identifier |
| `format` | int | Result schema version |
| `contract` | string | Contract preset or `manual` |
| `observables` | string[] | Compared state components |
| `assumptions` | string[] | Declared environmental and domain restrictions |
| `solver.result` | string | `sat` / `unsat` / `unknown` |
| `solver.phases` | object[] | Per-phase name, result, elapsed ms |
| `memory_scope` | object | Private-stack masking status per implementation |
| `mismatch` | object | First observed difference (kind, name, values) |
| `counterexample` | object | Concrete SAT witness (initial state + relocations) |
| `assumed_callees` | string[] | Callee premises actually used in the proof |
| `abstractions` | string[] | Reasons for `INCONCLUSIVE_ABSTRACTION` |
