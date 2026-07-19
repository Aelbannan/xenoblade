# `ppc_equivalence` Trustworthiness and Correctness Work Plan

**Repository:** [`Aelbannan/xenoblade`](https://github.com/Aelbannan/xenoblade)  
**Primary subsystem:** [`tools/ppc_equivalence`](https://github.com/Aelbannan/xenoblade/tree/main/tools/ppc_equivalence)  
**Integration subsystem:** [`tools/coop`](https://github.com/Aelbannan/xenoblade/tree/main/tools/coop)  
**Audit snapshot:** `main` as inspected on 2026-07-17  
**Audience:** implementation agents, reviewers, and maintainers  
**Purpose:** convert the verifier audit into a precise, executable remediation and hardening plan

> [!IMPORTANT]
> This document deliberately distinguishes **proof-engine soundness**, **scope of the theorem**, **independent validation**, and **automatic project acceptance**. A solver returning `unsat` is trustworthy only when the encoded semantics, assumptions, observables, and promotion policy are all trustworthy.

---

## 1. Executive directive

The `ppc_equivalence` tool has a strong overall design: it fails closed on unsupported behavior, represents integer state with exact bitvectors, models relocations symbolically, records assumptions, supports compositional callee certificates, and has both concrete and Dolphin-oriented validation infrastructure.

There is nevertheless one release-blocking soundness defect in final-memory comparison:

- the current implementation combines the original and candidate private-stack predicates with `Or(...)`;
- that combined predicate is then used to mask **both** memories;
- consequently, an address that is private only to the original can also hide a candidate write at that address, and vice versa;
- when stack-frame sizes differ, a candidate write outside its own frame can be incorrectly hidden because the address happens to fall inside the original frame.

This can create a false `EQUIVALENT` result. Because the co-op workflow converts qualifying `ProofStatus.EQUIVALENT` results into `EQUIVALENT_MATCH`, and the target registry treats `EQUIVALENT_MATCH` as accepted, remediation must cover both the verifier and the promotion/registry path.

### Required immediate policy

Until the private-stack defect is fixed, regression-tested, versioned, and deployed:

1. **Do not automatically accept new equivalence proofs that use private-stack masking and observe memory.**
2. **Invalidate or re-prove existing `EQUIVALENT_MATCH` entries produced by the affected architecture-model version.**
3. **Do not silently grandfather old equivalence certificates.**
4. `FULL_MATCH` entries are not affected by this verifier defect and should remain accepted.

### Required completion order

1. Fix independent private-stack masking.
2. Add adversarial regression tests that fail on the old implementation.
3. Introduce a temporary and then permanent promotion-policy gate.
4. Bump semantic/result versions and invalidate stale caches/certificates.
5. Recompute existing equivalence-based acceptances.
6. Make proof deadlines globally enforceable.
7. Establish public CI evidence and automate the independent Dolphin gate.
8. Add provenance, randomized differential testing, mutation testing, formal scope documentation, and environment profiles.

---

## 2. Repository map and affected trust boundary

The following files are central to this plan. Agents should confirm names and line locations against the pinned implementation commit before editing.

| Area | Current location | Why it matters |
|---|---|---|
| Equivalence query and memory comparison | [`tools/ppc_equivalence/engine.py`](https://github.com/Aelbannan/xenoblade/blob/main/tools/ppc_equivalence/engine.py) | Builds terminal-state differences, stack masking, solver queries, layout checks, and result classification. |
| Instruction semantics and machine state | [`tools/ppc_equivalence/semantics.py`](https://github.com/Aelbannan/xenoblade/blob/main/tools/ppc_equivalence/semantics.py) | Defines symbolic/concrete architectural behavior, state validity, memory touches, stack tracking, calls, and instruction effects. |
| Result schema and architecture identifier | [`tools/ppc_equivalence/result.py`](https://github.com/Aelbannan/xenoblade/blob/main/tools/ppc_equivalence/result.py) | Defines `ProofStatus`, architecture model, result format, assumptions, and serialized proof evidence. |
| Contract construction | [`tools/ppc_equivalence/contract.py`](https://github.com/Aelbannan/xenoblade/blob/main/tools/ppc_equivalence/contract.py) | Determines observable live-outs and proof scope. |
| Decoder | [`tools/ppc_equivalence/decoder.py`](https://github.com/Aelbannan/xenoblade/blob/main/tools/ppc_equivalence/decoder.py) | Converts PPC instruction words into semantic IR and validates supported encodings. |
| CLI | [`tools/ppc_equivalence/run.py`](https://github.com/Aelbannan/xenoblade/blob/main/tools/ppc_equivalence/run.py) | User-facing proof execution and self-test entry points. |
| Unit/soundness tests | [`tools/ppc_equivalence/tests`](https://github.com/Aelbannan/xenoblade/tree/main/tools/ppc_equivalence/tests) | Main regression suite. |
| Proof/certificate integration | [`tools/coop/lib/equivalence_check.py`](https://github.com/Aelbannan/xenoblade/blob/main/tools/coop/lib/equivalence_check.py) | Runs proofs, creates certificates, caches outcomes, loads callee premises. |
| Match classification | [`tools/coop/lib/objdiff_report.py`](https://github.com/Aelbannan/xenoblade/blob/main/tools/coop/lib/objdiff_report.py) | Turns `ProofStatus.EQUIVALENT` plus fuzzy-match threshold into `EQUIVALENT_MATCH`. |
| Registry acceptance and certificate validation | [`tools/coop/lib/targets.py`](https://github.com/Aelbannan/xenoblade/blob/main/tools/coop/lib/targets.py) | Treats `EQUIVALENT_MATCH` as accepted and validates stored certificates. |
| CI | [`.github/workflows/build.yml`](https://github.com/Aelbannan/xenoblade/blob/main/.github/workflows/build.yml) | Runs current Python/ConcreteOps checks; does not currently execute the Dolphin gate. |
| Tool documentation | [`tools/ppc_equivalence/README.md`](https://github.com/Aelbannan/xenoblade/blob/main/tools/ppc_equivalence/README.md) | Public theorem/scope and usage claims. |
| Reference hierarchy | [`tools/ppc_equivalence/REFERENCES.md`](https://github.com/Aelbannan/xenoblade/blob/main/tools/ppc_equivalence/REFERENCES.md) | Records ISA and Broadway evidence sources. |
| Python dependencies | [`tools/ppc_equivalence/requirements.txt`](https://github.com/Aelbannan/xenoblade/blob/main/tools/ppc_equivalence/requirements.txt) | Exact package versions are pinned, but artifacts are not currently hash-locked. |

### Current version mismatch to resolve

At the audit snapshot, the implementation identifies:

- architecture model `broadway-ppc32-be-v18`;
- result format `7`.

The README still describes older identifiers (`v15` and result format `4`). Version synchronization is therefore already demonstrably necessary, not merely preventative.

---

## 3. Definition of a trustworthy result

A trustworthy proof requires all of the following:

1. **Correct theorem statement** — the project knows exactly what “equivalent” means.
2. **Correct decoding** — supported bytes are decoded into the intended PPC operation and operands.
3. **Correct semantics** — every supported operation changes architectural state as real Broadway hardware would within the declared model.
4. **Correct control-flow construction** — all feasible modeled paths are included, and unsupported loops/indirect control flow fail closed.
5. **Correct observability contract** — every state component relevant to the caller is compared.
6. **Correct environmental preconditions** — alignment, ordinary RAM, relocation feasibility, FP mode, call assumptions, and definedness are explicit and enforced or reported.
7. **Correct SMT query** — the query asks whether a concrete counterexample exists and does not accidentally mask differences or prove from contradictory constraints.
8. **Correct solver result handling** — only a definitive `unsat` is accepted as equivalence; timeout and `unknown` remain inconclusive.
9. **Independent validation** — tests can detect a mistake shared by the symbolic and concrete implementations.
10. **Correct promotion policy** — proof scope and confidence are sufficient for the project action taken.
11. **Durable provenance** — the result can be tied to exact code, inputs, dependencies, certificates, and validation corpus.

The workstreams below cover each layer.

---

# Part I — Release-blocking work

## P0-01 — Fix asymmetric private-stack memory masking

**Priority:** P0 / release blocker  
**Primary owner area:** `tools/ppc_equivalence/engine.py`  
**Dependencies:** none  
**Blocks:** automatic equivalence acceptance, model-version bump, re-certification

### 4.1 Problem statement

The final-memory comparison should ignore an implementation’s own dead private stack storage while still observing that implementation’s writes outside its own private frame.

The current structure is effectively:

```python
private = z3.Or(
    left_private(address),
    right_private(address),
)

left_byte = z3.If(private, initial_byte, z3.Select(left.memory, address))
right_byte = z3.If(private, initial_byte, z3.Select(right.memory, address))
```

This applies a union mask to both sides. It is unsound when the two implementations have different private intervals.

### 4.2 Counterexample

Illustrative original:

```asm
# 64-byte frame
stwu r1, -64(r1)
addi r1, r1, 64
blr
```

Illustrative candidate:

```asm
# 16-byte frame
stwu r1, -16(r1)
stw  r3, -32(r1)     # writes entry_sp - 48
addi r1, r1, 16
blr
```

At the store:

- candidate `r1 = entry_sp - 16`;
- store effective address is `entry_sp - 48`;
- that address is outside the candidate’s 16-byte frame;
- it is inside the original’s 64-byte frame.

The candidate write must be observable. A union mask hides it.

Potential encodings for a focused fixture are shown below; tests should verify them through the project decoder rather than trusting comments alone:

```text
Original:
9421ffc0  # stwu r1,-64(r1)
38210040  # addi r1,r1,64
4e800020  # blr

Candidate:
9421fff0  # stwu r1,-16(r1)
9061ffe0  # stw r3,-32(r1)
38210010  # addi r1,r1,16
4e800020  # blr
```

### 4.3 Required implementation

Mask each terminal memory independently:

```python
left_is_private = _private_stack_address(
    address,
    left.stack_low,
    initial.gpr[1],
    left.stack_private,
    ops,
)

right_is_private = _private_stack_address(
    address,
    right.stack_low,
    initial.gpr[1],
    right.stack_private,
    ops,
)

left_byte = z3.If(
    left_is_private,
    initial_byte,
    z3.Select(left.memory, address),
)

right_byte = z3.If(
    right_is_private,
    initial_byte,
    z3.Select(right.memory, address),
)

byte_difference = left_byte != right_byte
```

This expresses the intended quotient over dead private storage:

- the left side’s private bytes are replaced with common initial memory only on the left;
- the right side’s private bytes are replaced with common initial memory only on the right;
- a write public on one side remains visible even when the same address is private on the other side.

### 4.4 Important semantic edge cases

The patch must preserve the following behavior:

1. **No write at the address:** both sides still equal common initial memory.
2. **Write private only on the left:** left write is ignored; right public write or unchanged value remains compared.
3. **Write private only on the right:** symmetric case.
4. **Private on both sides:** both sides compare as initial memory.
5. **Stack privacy disabled on one side:** no masking may be applied to that side.
6. **Stack escape:** once the implementation can expose its stack pointer to unknown code or memory, privacy must remain disabled according to current stack-analysis policy.
7. **Calls:** if calls invalidate private-stack reasoning, the comparison must obey that per-side state.
8. **32-bit wraparound:** invalid/upward/wrapping stack layouts must not be classified as ordinary private intervals.
9. **Path-specific frames:** each terminal state’s `stack_low` and `stack_private` must be used; do not merge paths prematurely.
10. **Touched-byte optimization:** all addresses written by either side must remain included in the comparison set.

### 4.5 Required tests

Add a dedicated test class or module, for example:

```text
tools/ppc_equivalence/tests/test_private_stack_memory.py
```

Minimum matrix:

| Test | Expected result |
|---|---|
| Candidate has smaller frame and writes inside original-only private interval | `NOT_EQUIVALENT` |
| Original has smaller frame and writes inside candidate-only private interval | `NOT_EQUIVALENT` |
| Different frame sizes; each side writes only within its own frame | `EQUIVALENT` |
| One side has a private address and the other leaves it unchanged | `EQUIVALENT` if no public output differs |
| Same public write/value on both sides despite different frames | `EQUIVALENT` |
| Different public values at address private to only one side | `NOT_EQUIVALENT` |
| Stack-pointer escape disables masking on original only | Public difference remains visible |
| Stack-pointer escape disables masking on candidate only | Public difference remains visible |
| A call disables or prevents private-stack inference | No unsafe masking |
| Multiple terminal paths with different frame lows | Each pair is handled independently |
| Frame restored before return | Private writes remain treated according to owned frame history/policy |
| Upward/wrapping/invalid stack layout | `INCONCLUSIVE_LAYOUT` or the existing fail-closed equivalent |

Suggested test shape:

```python
def test_candidate_public_write_is_not_hidden_by_original_larger_frame():
    original = bytes.fromhex(
        "9421ffc0"  # stwu r1,-64(r1)
        "38210040"  # addi r1,r1,64
        "4e800020"  # blr
    )
    candidate = bytes.fromhex(
        "9421fff0"  # stwu r1,-16(r1)
        "9061ffe0"  # stw r3,-32(r1)
        "38210010"  # addi r1,r1,16
        "4e800020"  # blr
    )

    result = prove_bytes_for_test(
        original,
        candidate,
        contract="auto",
    )

    assert result.status == ProofStatus.NOT_EQUIVALENT
    assert result.counterexample is not None
```

Use the existing project test helpers rather than introducing a second proof setup. The test must first be run against the old implementation and demonstrated to fail by returning `EQUIVALENT`; record that evidence in the pull request.

### 4.6 Documentation changes

Replace ambiguous wording such as:

> memory outside either implementation’s private stack frame

with explicit language:

> Each implementation’s own private stack interval is masked independently. An address private to one implementation does not hide a write by the other implementation when that write lies outside the other implementation’s own private interval.

Add the asymmetric-frame example to the README or `SOUNDNESS.md`.

### 4.7 Result metadata

The result should expose whether private-stack masking participated in the proof. Do not serialize a symbolic expression as though it were a concrete address. Use a structured classification:

```json
{
  "memory_scope": {
    "comparison": "touched-byte-extensional",
    "private_stack": {
      "masking_semantics": "per-implementation-independent-v1",
      "original": {
        "enabled_on_all_terminal_paths": true,
        "disabled_reasons": [],
        "frame_relation": "symbolic-below-entry-sp"
      },
      "candidate": {
        "enabled_on_all_terminal_paths": true,
        "disabled_reasons": [],
        "frame_relation": "symbolic-below-entry-sp"
      }
    }
  }
}
```

If useful, include hashes or normalized SMT renderings for diagnostic expressions, but never make promotion depend on unstable pretty-printed Z3 text.

### 4.8 Acceptance criteria

- The old implementation fails the new asymmetric-frame tests.
- The corrected implementation passes the full existing suite and new matrix.
- A minimized SAT counterexample is produced for the primary one-sided-write case.
- README/soundness wording is unambiguous.
- The architecture model is bumped.
- Old cached/certified results are rejected after the model bump.

---

## P0-02 — Add a promotion safety gate and migrate accepted results

**Priority:** P0 / release blocker  
**Primary owner areas:** `tools/coop/lib/objdiff_report.py`, `tools/coop/lib/targets.py`, `tools/coop/lib/equivalence_check.py`  
**Dependencies:** coordinate with P0-01 and P0-04

### 5.1 Current behavior

The integration currently has two important transitions:

1. `objdiff_report.py` classifies a function as `EQUIVALENT_MATCH` when its fuzzy match is above the configured floor and the proof status is `ProofStatus.EQUIVALENT`.
2. `targets.py` treats `EQUIVALENT_MATCH` as an accepted match status and moves workflow state to `ACCEPTED`.

A raw proof status is therefore sufficient to trigger project acceptance after the fuzzy-match threshold. That policy is too coarse for assumption-heavy or affected proofs.

### 5.2 Immediate mitigation

Introduce a configuration-controlled kill switch before merging the stack fix. Example:

```yaml
ppc_equivalence:
  automatic_promotion: false
```

or a more targeted emergency policy:

```yaml
ppc_equivalence:
  automatic_promotion:
    enabled: true
    reject_architecture_models:
      - broadway-ppc32-be-v18
    reject_when:
      - private_stack_masking_used
```

The targeted policy is preferable only if the result currently provides reliable evidence that stack masking was unused. If it does not, disable all new automatic equivalence promotion until the fixed model is live.

### 5.3 Permanent promotion decision model

Do not let `classify_status` decide from `ProofStatus` alone. Introduce a policy object:

```python
from dataclasses import dataclass

@dataclass(frozen=True)
class PromotionDecision:
    allowed: bool
    confidence_tier: str | None
    blockers: tuple[str, ...]
    warnings: tuple[str, ...]


def classify_for_promotion(
    proof: ProofResult,
    policy: PromotionPolicy,
    validation_ledger: ValidationLedger,
) -> PromotionDecision:
    blockers: list[str] = []
    warnings: list[str] = []

    if proof.status is not ProofStatus.EQUIVALENT:
        blockers.append("proof-status-is-not-equivalent")

    if proof.architecture_model not in policy.allowed_models:
        blockers.append("architecture-model-not-allowed")

    if proof.result_format < policy.minimum_result_format:
        blockers.append("result-format-too-old")

    if not proof.provenance.engine_tree_sha256:
        blockers.append("missing-engine-provenance")

    if proof.memory_scope.private_stack.masking_semantics != (
        "per-implementation-independent-v1"
    ):
        blockers.append("unsafe-or-unknown-private-stack-masking")

    if proof.scope.has_unconstrained_symbolic_memory and not policy.allow_assumed_ram:
        blockers.append("unconstrained-symbolic-memory-domain")

    tier = compute_confidence_tier(proof, validation_ledger)
    if tier not in policy.allowed_confidence_tiers:
        blockers.append(f"confidence-tier-{tier}-not-allowed")

    return PromotionDecision(
        allowed=not blockers,
        confidence_tier=tier,
        blockers=tuple(blockers),
        warnings=tuple(warnings),
    )
```

`objdiff_report.classify_status` should receive the completed `PromotionDecision`, not infer trust from the solver status itself.

### 5.4 Suggested match-state representation

Keep the human-facing `EQUIVALENT_MATCH` status, but store proof confidence separately:

```json
{
  "status": "EQUIVALENT_MATCH",
  "equivalence_status": "equivalent",
  "equivalence_confidence": "B",
  "equivalence_model": "broadway-ppc32-be-vNEXT",
  "equivalence_policy": "auto-promotion-v2",
  "equivalence_certificate": { "...": "..." }
}
```

Do not encode confidence solely in the display status because future policy changes need to re-evaluate existing entries without rewriting semantic proof evidence.

### 5.5 Confidence tiers

Use explicit, machine-computed tiers. A suggested initial policy:

#### Tier A — narrow, strongest modeled proofs

- integer/bitvector operations only;
- no memory access;
- no calls or opaque summaries;
- no floating-point or paired-single operations;
- no cache, MMU, system, privileged, timing, or exception-sensitive operations;
- all opcodes in the proof have current independent Dolphin validation;
- no non-default environmental assumption beyond ordinary initial architectural state;
- current architecture model, result format, and complete provenance.

#### Tier B — ordinary compiler-generated memory/function proofs

- aligned ordinary-RAM accesses only;
- corrected independent private-stack masking;
- no possible MMIO/unmapped/protection-sensitive addresses under the selected memory profile;
- calls absent or all callees covered by current transitive certificates;
- no unsupported behavioral side effects;
- current independent validation for all used opcodes;
- current architecture model and complete provenance.

#### Tier C — assumption-heavy proofs

Any of:

- floating point or paired-single domain restrictions;
- call summaries with nontrivial environmental premises;
- system/cache/Special Purpose Register behavior that models values but not full hardware effects;
- ordinary-RAM behavior assumed rather than constrained;
- incomplete independent Dolphin validation;
- other declared restrictions with meaningful legal-input exclusions.

Tier C may still be valuable and may remain visible as `EQUIVALENT_MATCH`, but default unattended acceptance should initially allow only A and carefully defined B.

### 5.6 Migration of existing accepted targets

Build a deterministic migration command, for example:

```bash
python -m tools.coop.run equivalence audit-registry \
  --reject-model broadway-ppc32-be-v18 \
  --write-report artifacts/equivalence_registry_audit.json
```

The command should:

1. Load every target marked `EQUIVALENT_MATCH`.
2. Validate certificate format, model, hashes, and transitive dependencies.
3. Mark affected entries as requiring re-proof.
4. Preserve their historical evidence in append-only attempt logs.
5. Move workflow status from `ACCEPTED` to an explicit state such as `REVALIDATION_REQUIRED` or `ACTIVE`.
6. Re-run proofs under the fixed model where build artifacts are available.
7. Restore acceptance only after the permanent promotion policy approves the new proof.
8. Produce a machine-readable and human-readable summary.

Do not downgrade `FULL_MATCH` entries merely because they also happen to have a stale equivalence certificate; byte identity remains independent evidence.

### 5.7 Cache and certificate handling

Every cache key and certificate validator must include at least:

- architecture model;
- result format;
- certificate version;
- engine/source hash;
- original and candidate function hashes;
- normalized contract;
- environmental profile;
- relevant solver configuration;
- transitive callee-certificate root.

If any component differs, recompute rather than reuse.

### 5.8 Acceptance criteria

- A raw `ProofStatus.EQUIVALENT` can no longer directly imply acceptance.
- Promotion decisions contain explicit blockers and confidence tier.
- Old-model `EQUIVALENT_MATCH` entries are audited and cannot remain silently accepted.
- Full-match behavior remains unchanged.
- Registry migration is idempotent and covered by tests.
- Attempt history remains append-only and preserves old evidence.

---

## P0-03 — Enforce a real end-to-end proof deadline

**Priority:** P0  
**Primary owner area:** `tools/ppc_equivalence/engine.py`  
**Dependencies:** none

### 6.1 Problem statement

The default solver receives `timeout_ms`, but the fallback `simplify -> bit-blast -> sat` tactic path is separately constructed without a corresponding timeout. The tactic path is also attempted even when the default solver has already returned a definitive result.

Consequences:

- `timeout_ms` is not a reliable wall-clock limit;
- proofs can perform duplicate work after a conclusive answer;
- automation can hang or consume excessive resources;
- result timing does not express where the budget was spent.

### 6.2 Required semantic rule

`timeout_ms` must mean:

> Maximum elapsed wall-clock budget for the entire proof operation, including decoding/execution if policy chooses, query construction, layout feasibility, all solver portfolios, and counterexample extraction.

If query construction is intentionally excluded, rename the option to `solver_timeout_ms` and add a separate `proof_deadline_ms`. The preferred API is a single end-to-end deadline plus optional internal portfolio shares.

### 6.3 Deadline utility

```python
from dataclasses import dataclass
import time


@dataclass(frozen=True)
class Deadline:
    end_ns: int

    @classmethod
    def after_ms(cls, timeout_ms: int) -> "Deadline":
        if timeout_ms <= 0:
            raise ValueError("timeout_ms must be positive")
        return cls(time.monotonic_ns() + timeout_ms * 1_000_000)

    def remaining_ms(self) -> int:
        remaining_ns = self.end_ns - time.monotonic_ns()
        if remaining_ns <= 0:
            return 0
        # Round up so a positive remainder does not become an immediate timeout.
        return max(1, (remaining_ns + 999_999) // 1_000_000)

    def require_time(self, phase: str) -> int:
        remaining = self.remaining_ms()
        if remaining <= 0:
            raise ProofDeadlineExceeded(phase)
        return remaining
```

### 6.4 Solver portfolio behavior

Preferred initial behavior:

1. Run the default solver first.
2. Return immediately on `sat` or `unsat`.
3. Run fallback tactics only when the default solver returns `unknown`, and only with remaining budget.
4. Record each phase’s result and elapsed time.

```python
def check_with_portfolio(assertions, deadline: Deadline) -> SolverOutcome:
    phases: list[SolverPhase] = []

    default = z3.Solver()
    default.set(timeout=deadline.require_time("default-solver"))
    default.add(*assertions)

    started = time.monotonic_ns()
    result = default.check()
    phases.append(measure_phase("default", result, started))

    if result == z3.sat:
        return SolverOutcome.sat(default.model(), phases)
    if result == z3.unsat:
        return SolverOutcome.unsat(phases)

    fallback = z3.Then("simplify", "bit-blast", "sat").solver()
    fallback.set(timeout=deadline.require_time("bitblast-fallback"))
    fallback.add(*assertions)

    started = time.monotonic_ns()
    fallback_result = fallback.check()
    phases.append(measure_phase("bitblast", fallback_result, started))

    if fallback_result == z3.sat:
        return SolverOutcome.sat(fallback.model(), phases)
    if fallback_result == z3.unsat:
        return SolverOutcome.unsat(phases)
    return SolverOutcome.unknown(phases, reason=fallback.reason_unknown())
```

Do not assume every tactic solver honors timeout perfectly. For a strict hard wall-clock guarantee, run the proof worker in a subprocess and terminate it when the parent deadline expires.

### 6.5 Hard-limit subprocess design

A robust CLI/automation architecture:

```text
parent process
  -> serializes proof request to worker
  -> starts worker subprocess in its own process group
  -> waits until deadline
  -> receives structured result, or terminates process group
  -> returns INCONCLUSIVE_TIMEOUT with phase diagnostics
```

Requirements:

- no `fork` reuse of an active Z3 context unless tested safe;
- bounded stdout/stderr;
- temporary files cleaned after forced termination;
- platform-specific termination handled explicitly;
- proof cache written only after a complete, validated result;
- no partial certificate emitted after timeout.

### 6.6 Resource metadata

Add:

```json
{
  "resources": {
    "requested_deadline_ms": 120000,
    "elapsed_ms": 8732,
    "peak_rss_bytes": 123456789,
    "phases": [
      {
        "name": "layout-feasibility",
        "elapsed_ms": 4,
        "result": "sat"
      },
      {
        "name": "default-solver",
        "elapsed_ms": 8720,
        "result": "unsat"
      }
    ]
  }
}
```

Peak RSS may be populated by the worker/parent on supported platforms. Do not make cross-platform availability mandatory for proof validity.

### 6.7 Tests

- Definitive default `unsat` does not invoke fallback.
- Definitive default `sat` does not invoke fallback.
- Default `unknown` invokes fallback with less than the original remaining budget.
- Expired deadline before fallback yields `INCONCLUSIVE_TIMEOUT`.
- Layout solver uses the same deadline.
- Counterexample extraction cannot exceed the deadline.
- Cache/certificate writes do not occur after timeout.
- Subprocess hard timeout exits within a defined tolerance, e.g. deadline plus 500 ms in CI.
- `timeout_ms=0` behavior is explicit: either reject, map to configured default, or mean no limit. Do not leave it ambiguous.

### 6.8 Acceptance criteria

- No proof phase can receive the original full timeout after earlier phases consumed time.
- The fallback is not run after a conclusive default result.
- A deliberately pathological fixture terminates near the requested deadline.
- Timeout results remain inconclusive and never generate acceptance certificates.

---

## P0-04 — Version the semantic fix and invalidate stale evidence

**Priority:** P0  
**Primary owner areas:** `result.py`, certificate code, caches, registry validation  
**Dependencies:** P0-01 and schema decisions from P0-02/P1-05

### 7.1 Required version changes

The private-stack fix changes the theorem encoded by the engine. Therefore:

- bump `ARCHITECTURE_MODEL` to the next project-approved identifier;
- bump `RESULT_FORMAT` if result fields or serialized semantics change;
- bump `EQUIVALENCE_CERTIFICATE_VERSION` if certificate payload/validation changes;
- invalidate all proof caches keyed to the prior model;
- reject old certificates in automatic acceptance.

Do not hardcode `v19` in implementation plans if another semantic version lands first. Use “next model version” and let a single release commit choose the concrete identifier.

### 7.2 Why this is mandatory

A proof result is not merely a solver answer. It means:

```text
UNSAT under architecture model X, result schema Y, contract C, assumptions A,
and exact implementation inputs I.
```

Changing memory comparison changes `X`. Reusing an old result under a new model is unsound even if source bytes are unchanged.

### 7.3 Tests

- Old architecture model certificate is rejected.
- Old result format is rejected when required fields are absent.
- Old cache entry is not returned.
- Transitive caller certificate is rejected when any callee uses old model.
- Full-match registry row is not incorrectly invalidated as a byte match.
- Migration report enumerates every affected accepted equivalence row.

---

# Part II — Soundness and specification hardening

## P1-01 — Build an adversarial soundness-regression suite

**Priority:** P1, begin in parallel with P0 fixes  
**Primary owner area:** `tools/ppc_equivalence/tests`

### 8.1 Goal

Ordinary unit tests often demonstrate that intended examples work. A verifier additionally needs tests designed to make it incorrectly say `EQUIVALENT`.

Create a clearly named suite:

```text
tools/ppc_equivalence/tests/soundness/
  test_memory_masking.py
  test_observability_contracts.py
  test_definedness.py
  test_control_flow.py
  test_relocations.py
  test_calls_and_certificates.py
  test_floating_point_domains.py
  test_result_and_promotion.py
```

### 8.2 Core invariant

Every negative test should satisfy:

> There exists a concrete initial state inside the declared proof domain for which an observable result differs.

Then the expected verifier result is `NOT_EQUIVALENT`, never `EQUIVALENT`.

For unsupported or intentionally unmodeled behavior, expected status should be the exact inconclusive category, never a generic pass.

### 8.3 Required adversarial categories

#### Memory

- one-sided byte/halfword/word stores;
- partial overlap between stores;
- big-endian byte-order differences;
- differing update-address calculations;
- address wraparound;
- asymmetric private frames;
- stack pointer escape;
- writes before/after frame restoration;
- symbolic address equality and disequality boundaries;
- multiple stores where the last write differs;
- stores to same word through different widths;
- touched-address union completeness.

#### Observability contracts

- candidate changes a volatile GPR that is intentionally excluded by `ppc-eabi` and prove this is accepted only for a function contract;
- the same changed GPR under an internal-block contract must be rejected;
- CR field live-out omissions;
- `XER.CA`, `XER.OV`, `XER.SO` boundaries;
- FPSCR and FPR changes when written by only one side;
- exit target/kind differences;
- LR/CTR behavior under appropriate contracts.

#### Definedness and invalid states

- one side invalid and the other valid;
- both invalid for different reasons;
- divide-by-zero/overflow boundaries;
- unaligned memory access;
- FP inputs outside the modeled domain;
- verify exact intended partial-equivalence policy.

The test names must state whether the expected theorem is partial equivalence or exception equivalence. Do not let accidental current behavior become an undocumented specification.

#### Control flow

- branch predicate differs only at signed/unsigned boundary;
- condition-register field indexing differences;
- link bit and return behavior;
- one side has an extra feasible exit;
- infeasible paths caused by contradictory constraints;
- loop/back-edge rejection;
- indirect branch/call rejection;
- path-limit and instruction-limit outcomes.

#### Relocations and layout

- inconsistent relocation equality must not create vacuous proof;
- branch displacement exact lower/upper bounds;
- alignment boundaries;
- SDA range boundaries;
- same symbol/addend canonicalization;
- different symbols must not be unified;
- impossible layout must report inconclusive layout, not equivalence.

#### Calls and certificates

- missing callee certificate;
- stale direct certificate;
- stale transitive certificate;
- callee summary omits a read dependency;
- callee summary omits a write effect;
- changed helper hash;
- wrong architecture/result format;
- absolute-LR/return-address-sensitive callee rejected or downgraded;
- SAT through an opaque abstraction remains `INCONCLUSIVE_ABSTRACTION`.

#### Floating point and paired-single

Boundary corpus:

- `+0` and `-0`;
- smallest/largest subnormal;
- smallest normal;
- largest finite;
- positive/negative infinity;
- quiet/signaling NaN patterns where modeled;
- rounding tie cases;
- fused vs non-fused differences;
- reciprocal/rsqrt estimate table boundaries;
- paired-single quantization type/scale bounds;
- FPSCR compare/classification bits;
- legal inputs excluded by current assumptions must yield explicit restricted scope, not silent universal claims.

### 8.4 Test helper requirements

Provide one canonical helper that:

- decodes bytes through the production decoder;
- runs the production proof engine;
- verifies expected status;
- optionally checks a concrete witness with `ConcreteOps`;
- serializes a reproduction bundle when the assertion fails.

Avoid tests that manually construct internal state in ways real callers cannot.

### 8.5 Acceptance criteria

- Every historical soundness bug has a permanent regression test.
- Negative tests fail against a deliberately reintroduced defect.
- Status assertions are exact, not broad `!= EQUIVALENT` checks unless multiple inconclusive outcomes are truly allowed.
- Soundness tests run in required PR CI.

---

## P1-02 — Write a formal, auditable proof-scope specification

**Priority:** P1  
**Primary output:** `tools/ppc_equivalence/SOUNDNESS.md`

### 9.1 Purpose

The README is user documentation. Add a separate normative specification that maintainers can review against code.

### 9.2 Required theorem statement

Use a statement similar to:

> For all shared initial machine states satisfying the selected contract preconditions, environmental profile, relocation/layout constraints, and instruction-domain restrictions, there exists no feasible pair of modeled terminal executions whose exit behavior, definedness state, or selected observable architectural outputs differ after applying only the explicitly documented per-implementation private-storage abstraction.

Then define every term.

### 9.3 Required definitions

#### Initial-state quantification

- Which GPRs/FPRs/CR fields/FPSCR/XER/LR/CTR/memory values are symbolic?
- Which state is constrained by ABI or environment?
- Are both functions started from exactly the same state?

#### Feasible execution

- How path conditions are formed.
- How branches are paired.
- How loops, indirect branches, and path limits fail closed.

#### Terminal behavior

- normal return;
- direct branch/tail call;
- trap/invalid state;
- target address observability;
- fallthrough, if allowed.

#### `valid` and partial-domain equivalence

State explicitly whether the verifier proves partial equivalence:

```text
For all states where modeled operations remain valid, outputs agree.
```

or total exception equivalence:

```text
For all states, both value outputs and exception behavior agree.
```

The current design appears closer to partial equivalence. Document how pairs where both sides become invalid are treated, and prohibit marketing language that implies full hardware exception equivalence.

#### Memory model

- 32-bit byte-addressed array;
- big-endian loads/stores;
- touched-byte extensional comparison;
- alignment policy;
- ordinary RAM/MMIO assumptions;
- address-space wrapping;
- private-stack ownership and escape.

#### Contracts

- `auto` function contract;
- `ppc-eabi` limitations;
- internal-block live-outs;
- memory observability;
- one-sided writes included through union analysis.

#### Calls

- summary determinism;
- input/output state components;
- preserved state;
- memory token/effects;
- return-address/location-independence premise;
- certificate-chain validity.

#### Floating point

- rounding mode;
- NI mode;
- traps;
- overflow/underflow/inexact modeling;
- fused operation input restrictions;
- exact scope of estimates and paired-single operations.

#### Relocations

- canonical symbol identity;
- supported relocation formulas;
- linkability constraints;
- independent layout-feasibility check.

### 9.4 Traceability table

Include a table like:

| Soundness claim / assumption | Implementation | Tests | Result field |
|---|---|---|---|
| Independent stack masking | `engine._memory_difference` | `test_private_stack_memory.py` | `memory_scope.private_stack.masking_semantics` |
| Ordinary RAM range | memory-profile constraints | memory profile tests | `environment.memory_profile` |
| No unsupported loop | CFG executor | control-flow tests | `limits/unsupported_reason` |
| Nearest-even FP | FP preconditions | FP domain tests | `assumptions.floating_point.rounding` |
| Current callee chain | certificate validator | stale-transitive tests | `premises.callee_root_sha256` |

CI should fail if a normative assumption lacks an implementation or test reference.

### 9.5 Acceptance criteria

- A reviewer can reconstruct the exact theorem without reading source.
- Every result assumption has a normative definition.
- The README links to `SOUNDNESS.md` and uses the same terminology.
- Promotion policy cites explicit proof-scope fields rather than parsing prose.

---

## P1-03 — Add enforceable memory-environment profiles

**Priority:** P1  
**Primary owner areas:** engine, CLI, result schema, promotion policy

### 10.1 Problem statement

The current array model can represent any 32-bit address, while assumptions such as “mapped ordinary RAM,” no MMIO, valid protection, and ordinary cache behavior are largely external. For a symbolic pointer, the solver may choose an address that the model treats like normal RAM even though real hardware would access MMIO or fault.

A proof can still be useful under that assumption, but acceptance policy must know whether the assumption was enforced or merely declared.

### 10.2 Profiles

Introduce explicit profiles:

#### `assumed-ordinary-ram`

Current broad behavior:

- addresses are unconstrained except for modeled alignment/definedness;
- all accesses are interpreted as ordinary byte-array memory;
- result clearly records this as an external assumption;
- default auto-promotion should not classify this above Tier C when pointers are symbolic and unconstrained.

#### `bounded-ordinary-ram`

- caller supplies one or more valid RAM ranges;
- every touched byte of every memory operation must lie within a configured range;
- no address wraparound;
- alignment constraints are enforced;
- suitable for Tier B when ranges are appropriate for the executable/environment.

#### `stack-and-known-globals`

- stack accesses constrained to a configured valid stack region;
- relocation-backed globals constrained to known DOL sections or declared RAM ranges;
- unknown pointer accesses are rejected/inconclusive unless separately bounded.

#### `hardware-aware`

Future profile. Until actual MMIO/protection/cache semantics exist, unsupported regions must fail closed rather than be modeled as ordinary memory.

### 10.3 Range constraint helper

Be careful about width and overflow:

```python
def access_within_range(addr, width_bytes: int, low: int, high: int):
    if width_bytes <= 0:
        raise ValueError("width_bytes must be positive")

    width = z3.BitVecVal(width_bytes - 1, 32)
    last = addr + width

    return z3.And(
        z3.UGE(addr, z3.BitVecVal(low, 32)),
        z3.ULE(last, z3.BitVecVal(high, 32)),
        z3.UGE(last, addr),  # reject 32-bit wraparound
    )


def access_within_any_range(addr, width_bytes, ranges):
    return z3.Or(*[
        access_within_range(addr, width_bytes, low, high)
        for low, high in ranges
    ])
```

Use each instruction’s actual width. For vector/paired accesses or multi-part effects, cover all touched bytes.

### 10.4 Policy choice for out-of-profile paths

Do not simply constrain away illegal addresses and claim universal equivalence. The theorem must say equivalence is restricted to initial states satisfying the profile. Record the restriction prominently.

A separate “prove domain agreement” query may be useful:

- can one implementation access outside the profile while the other remains within it?
- can their access validity differ?

If so, classify according to the formal definedness policy rather than hiding it.

### 10.5 Result schema

```json
{
  "environment": {
    "memory_profile": "bounded-ordinary-ram",
    "ranges": [
      {"start": "0x80000000", "end": "0x817fffff", "kind": "ram"}
    ],
    "alignment": "instruction-natural",
    "mmio": "excluded-by-range",
    "address_wraparound": "rejected"
  }
}
```

### 10.6 Tests

- start/end boundaries for every access width;
- access whose first byte is in range but final byte is out;
- 32-bit wraparound;
- disjoint ranges;
- symbolic address inside/outside witness;
- one side accesses MMIO-excluded region;
- stack range interaction;
- result/promotion tier changes by profile.

---

## P1-04 — Clarify and test definedness and exception semantics

**Priority:** P1  
**Primary owner areas:** semantics, engine terminal comparison, `SOUNDNESS.md`

### 11.1 Required decision

Choose and document one of these models:

1. **Partial functional equivalence:** compare outputs only over states where both sides satisfy modeled validity preconditions.
2. **Definedness-preserving partial equivalence:** require both sides to agree on whether execution is defined, then compare outputs when defined.
3. **Exception-aware total equivalence:** model and compare actual exception classes/effects.

The current implementation appears intended to compare definedness while suppressing ordinary output differences when both sides are invalid. That is a legitimate model, but it must be precise.

### 11.2 Recommended near-term model

Use definedness-preserving partial equivalence:

- if exactly one side is valid, report difference;
- if both are valid, compare all observables;
- if both are invalid, require agreement on a normalized invalid reason class if that class is modeled;
- if invalid reasons are not modeled reliably, report `INCONCLUSIVE_UNMODELED_EXCEPTION` rather than universal equivalence for exception-heavy code.

### 11.3 Extend state if necessary

Instead of a single Boolean `valid`, consider:

```python
class InvalidReason(Enum):
    NONE = "none"
    UNALIGNED_ACCESS = "unaligned-access"
    DIVIDE_UNDEFINED = "divide-undefined"
    FP_DOMAIN_EXCLUDED = "fp-domain-excluded"
    UNSUPPORTED_SIDE_EFFECT = "unsupported-side-effect"
    MEMORY_PROFILE_VIOLATION = "memory-profile-violation"
```

Symbolic representation may use one-hot predicates or a small bitvector enum. Do not pretend to distinguish hardware exception vectors unless implemented.

### 11.4 Tests

- same invalid reason on both sides;
- different invalid reasons;
- valid vs invalid;
- both invalid but different earlier public writes;
- trap-enabled FP mode remains outside scope;
- unaligned load vs aligned load;
- divide edge cases.

### 11.5 Promotion impact

Exception-sensitive or domain-excluded proofs must be Tier C until exception behavior is modeled and independently validated.

---

## P1-05 — Strengthen call summaries and certificate soundness

**Priority:** P1  
**Primary owner areas:** `semantics.py`, `equivalence_check.py`, `targets.py`

### 12.1 Threat model

A caller proof that uses a callee summary is only as sound as:

- the callee proof;
- the inferred read/write contract;
- the summary’s state transition;
- the certificate validator;
- every transitive dependency;
- the premise that the callee does not depend on omitted context such as absolute return address.

### 12.2 Required certificate payload

At minimum:

```json
{
  "certificate_version": 2,
  "status": "equivalent",
  "architecture_model": "...",
  "result_format": 8,
  "target_id": "...",
  "original_function_sha256": "...",
  "candidate_function_sha256": "...",
  "contract_sha256": "...",
  "engine_tree_sha256": "...",
  "environment_profile_sha256": "...",
  "proof_request_sha256": "...",
  "direct_callees": [],
  "transitive_callee_root_sha256": "...",
  "helpers": [],
  "validation_ledger_sha256": "...",
  "confidence_tier": "B",
  "certificate_sha256": "..."
}
```

### 12.3 Read/write contract validation

For each summary:

- every state component read before being overwritten must be in inputs;
- every state component potentially written must be in outputs/effects;
- memory reads and writes need explicit region/effect representation;
- preserved nonvolatile state must be asserted, not assumed from naming alone;
- exit kind/target and definedness must be represented;
- nondeterministic or environment-dependent results must not be summarized as deterministic uninterpreted functions unless the nondeterminism is part of the function’s input token.

### 12.4 Return-address/location dependence

The summary currently relies on normal location-independent EABI behavior. Add detection/policy for callees that:

- read LR as data;
- spill and inspect the absolute return address;
- use PC/LR-derived tables in a way not covered by relocations;
- compare call-site identity.

Such functions should be rejected from ordinary location-independent summaries unless the call-site address is included as an explicit input.

### 12.5 Certificate-chain validation tests

- direct stale hash;
- transitive stale hash;
- cycle in certificate graph;
- changed contract with same code;
- changed environment profile;
- changed validation ledger;
- changed helper;
- missing memory effect;
- malformed canonical JSON/hash ambiguity;
- duplicate or reordered dependencies produce a stable canonical root.

### 12.6 Abstraction result handling

Preserve the safe current principle:

- `unsat` under sound summaries may prove caller equivalence;
- `sat` involving opaque summary values may not provide a concrete machine witness and must remain `INCONCLUSIVE_ABSTRACTION` unless concretized.

Document this in the result schema and UI.

---

## P1-06 — Audit the trusted computing base instruction by instruction

**Priority:** P1/P2  
**Primary output:** validation ledger and review records

### 13.1 Trusted computing base inventory

Create `tools/ppc_equivalence/TRUSTED_COMPUTING_BASE.md` covering:

- decoder field extraction and reserved-bit checks;
- IR construction;
- symbolic operations;
- ConcreteOps;
- memory load/store composition;
- control flow and terminal pairing;
- contracts;
- relocations/layout;
- FP tables/helpers;
- stack analysis;
- callee summaries/certificates;
- query construction;
- solver and Python/runtime dependencies;
- promotion policy.

### 13.2 Per-opcode validation ledger

Create a machine-readable file, e.g.:

```yaml
architecture_model: broadway-ppc32-be-vNEXT
corpus_version: 1
opcodes:
  add:
    decoder_unit: true
    concrete_symbolic_differential: true
    dolphin_interpreter: true
    boundary_vectors: true
    reviewer: agent-id-or-human
    evidence_sha256: "..."
  psq_l:
    decoder_unit: true
    concrete_symbolic_differential: true
    dolphin_interpreter: false
    boundary_vectors: partial
    restrictions:
      - "selected GQR domain only"
```

Promotion computes confidence from this ledger and the exact opcodes used by the proof.

### 13.3 Review procedure

For each instruction family:

1. Identify primary ISA reference and Broadway/Dolphin evidence.
2. Review field extraction and invalid encodings.
3. Review every state read/write.
4. Enumerate boundary cases.
5. Compare ConcreteOps and symbolic singleton behavior.
6. Execute generated fixtures in Dolphin interpreter.
7. Record evidence hash and model version.
8. Require a second reviewer for FP, memory-update, branch, and system instructions.

### 13.4 Common-mode risk

ConcreteOps and symbolic semantics often share dispatch and conceptual implementation. Passing their differential test is not independent proof of ISA correctness. The ledger must distinguish:

- internal consistency;
- independent execution against Dolphin;
- manual/reference review.

---

# Part III — Independent validation and CI

## P1-07 — Make the Dolphin interpreter corpus an automated gate

**Priority:** P1  
**Primary owner areas:** fixture generator, Dolphin harness, GitHub Actions

### 14.1 Current gap

The documented quality process includes generated DOL payloads executed by Dolphin’s interpreter, but the repository’s main CI workflow currently runs only fixture freshness, Python self-tests, ConcreteOps differential checks, and CLI help. At the audit snapshot, GitHub also displayed no completed runs for that workflow, so the repository lacked public pass history.

### 14.2 CI architecture

Use two levels:

#### Required PR gate — fast

- dependency/lock verification;
- fixture-generation freshness;
- Python tests including soundness suite;
- ConcreteOps-symbolic differential corpus;
- documentation/version synchronization;
- certificate/schema tests;
- small deterministic Dolphin smoke corpus if runtime is acceptable.

#### Scheduled or required specialized gate — full Dolphin

- full generated instruction corpus;
- boundary-heavy vectors;
- randomized seeded corpus;
- interpreter mode explicitly selected;
- emulator version/digest pinned;
- artifacts uploaded.

If full Dolphin runtime is practical, make it a required PR check for changes to decoder/semantics/engine/fixtures. Otherwise, require it before an architecture-model release and block model promotion until it passes.

### 14.3 Legal/reproducibility constraints

The corpus should use only generated DOL payloads and generated data. Do not require retail game assets in CI. Document all external binary licenses and distribution rules.

Pin:

- Dolphin release or commit;
- container image digest;
- configuration files;
- interpreter/JIT selection;
- fixture-generator version;
- expected-output corpus hash.

### 14.4 Artifact bundle

Each run should upload:

```text
ppc-equivalence-validation/
  summary.json
  junit.xml
  dolphin-version.txt
  container-digest.txt
  validation-ledger.yaml
  fixture-manifest.json
  generated-dol.sha256
  failures/
    <seed-or-case>/
      request.json
      payload.dol
      expected.json
      actual.json
      disassembly.txt
```

### 14.5 Branch protection

After the first successful run:

- require the fast equivalence job for all pull requests touching the subsystem;
- require the model-release Dolphin job before changing `ARCHITECTURE_MODEL`;
- add a visible status badge;
- prevent maintainers from advertising an opcode as independently validated unless ledger evidence exists.

### 14.6 Acceptance criteria

- At least one public successful CI run exists.
- Dolphin corpus is reproducible from a clean checkout without retail assets.
- Failure artifacts are sufficient for local reproduction.
- Model-version changes cannot merge without current independent validation evidence.

---

## P2-01 — Add deterministic randomized differential generation

**Priority:** P2  
**Primary owner areas:** generators, tests, Dolphin harness

### 15.1 Goal

Handwritten fixtures cannot cover all operand aliases, immediate boundaries, and interacting state. Add deterministic property-based generation.

### 15.2 Three-way oracle

For each generated program/input:

1. Execute with `ConcreteOps`.
2. Constrain the symbolic initial state to the same concrete state and evaluate/prove the symbolic final state.
3. Execute the generated DOL in Dolphin interpreter.

All modeled observable states must agree within the declared scope.

### 15.3 Generator design

Do not use uniform random instruction words. Generate valid instructions by family and deliberately weight difficult cases.

#### Operand alias strata

- all registers distinct;
- destination equals source A;
- destination equals source B;
- both sources equal;
- `r0` special cases;
- `r1` stack-sensitive cases;
- CR field boundaries;
- paired FPR aliases.

#### Immediate/boundary strata

- `0`, `1`, `-1`;
- signed min/max;
- mask boundaries;
- shifts `0`, word-size minus one, word-size, greater than word-size where encoding permits;
- branch displacement min/max;
- relocation range boundaries;
- carry/overflow transition values.

#### Memory strata

- aligned addresses;
- each misalignment class for negative/inconclusive tests;
- width overlap;
- update forms;
- stack frame boundaries;
- range start/end;
- no-wrap/wraparound.

#### Floating-point strata

Use explicit bit patterns, not only host-language floats:

- zeros, normals, subnormals, infinities, NaNs;
- tie-to-even boundaries;
- exponent transition values;
- reciprocal estimate table edges;
- PSQ scale/type boundaries.

### 15.4 Seed and reproduction format

Every run must print/store:

- master seed;
- generator version;
- architecture model;
- selected instruction family;
- exact program bytes;
- exact initial state bits;
- environment profile;
- expected/actual outputs.

Example:

```json
{
  "generator": "ppc-random-v1",
  "seed": 1489112045,
  "architecture_model": "broadway-ppc32-be-vNEXT",
  "program_hex": "...",
  "initial_state": {
    "gpr": ["0x..."],
    "fpr_bits": ["0x..."],
    "cr": "0x...",
    "xer": "0x...",
    "memory_bytes": {"0x80001000": "0xff"}
  }
}
```

### 15.5 Shrinking

On failure, minimize:

- number of instructions;
- nondefault registers/memory bytes;
- immediate magnitude where possible;
- path complexity.

Preserve the original full reproduction before shrinking.

### 15.6 Metamorphic properties

Add properties that do not require a second implementation:

- encoding/decoding round trip where supported;
- `x + 0 == x` without unexpected flags;
- rotate by zero preserves value with specified record behavior;
- store then load same width/address returns original bits in ordinary RAM;
- equivalent instruction aliases produce same state;
- branch inversion partitions condition domain;
- byte-swap expectations under big-endian composition.

### 15.7 Acceptance criteria

- Re-running a stored seed is deterministic.
- CI saves complete failure bundles.
- Coverage statistics show each supported opcode/family and boundary stratum.
- Dolphin and symbolic comparisons use exact bit patterns.

---

## P2-02 — Add mutation testing focused on semantic soundness

**Priority:** P2  
**Primary owner areas:** test tooling and CI

### 16.1 Goal

Line coverage only shows that code ran. Mutation testing asks whether tests detect a plausible semantic error.

### 16.2 Targeted mutations

Create deterministic custom mutations or use a Python mutation framework where practical. Required mutations include:

- invert `XER.CA` calculation;
- omit `XER.SO` propagation;
- swap signed and unsigned comparison;
- select the wrong CR field;
- reverse big-endian byte order;
- omit one touched byte from a word store;
- reintroduce union private-stack masking;
- change branch target by 4 bytes;
- ignore link-register update;
- turn fused operation into non-fused;
- alter FP rounding tie behavior;
- omit an FPSCR bit;
- change one reciprocal/rsqrt table entry;
- omit a relocation range constraint;
- skip layout-feasibility check;
- treat solver `unknown` as `unsat`;
- omit one callee read/write dependency;
- accept stale transitive certificate;
- run promotion on status alone.

### 16.3 Scope and thresholds

Start with critical files:

- `engine.py`;
- decoder core;
- integer/memory/branch semantics;
- result classification;
- certificate validation;
- promotion policy.

Set policy:

- no surviving P0/P1 targeted mutation without an explicit reviewed explanation;
- generic mutation-score threshold can begin lower and rise over time;
- equivalent or unreachable mutants must be documented, not counted as silently passing.

### 16.4 CI cadence

- targeted critical mutations on PRs touching relevant files;
- full mutation suite nightly/weekly;
- upload survivor report and source diff.

### 16.5 Acceptance criteria

- Reintroduced stack union bug is killed.
- `unknown -> equivalent` mutation is killed.
- endian, CR, carry, and certificate mutations are killed.
- survivors have owners and issue links.

---

# Part IV — Reproducibility, provenance, and documentation

## P1-08 — Add complete proof provenance

**Priority:** P1  
**Primary owner areas:** result schema, certificate builder, CLI, cache

### 17.1 Required provenance

Every durable proof result should record:

#### Source/tool identity

- Git commit SHA;
- dirty working-tree Boolean;
- deterministic SHA-256 of relevant engine source tree;
- architecture model;
- result format;
- certificate version.

#### Runtime identity

- Python version;
- OS/platform/architecture;
- Z3 version;
- Capstone version;
- dependency lock hash;
- container image digest when available.

#### Proof request identity

- normalized CLI/API arguments;
- contract and live-outs;
- limits and deadline;
- memory/environment profile;
- original/candidate base addresses;
- original/candidate bytes;
- relocation metadata;
- local symbol identity;
- callee premises.

#### Validation identity

- validation-ledger hash;
- Dolphin version/commit;
- independent corpus hash;
- date of validation;
- exact opcode evidence used to assign confidence tier.

### 17.2 Deterministic engine-tree hash

Hash only declared trust-boundary inputs, with normalized paths and bytes:

```python
def hash_engine_tree(repo_root: Path) -> str:
    paths = sorted(
        list((repo_root / "tools/ppc_equivalence").rglob("*.py"))
        + [repo_root / "tools/ppc_equivalence/requirements.lock"]
        + [repo_root / "tools/ppc_equivalence/validation_ledger.yaml"]
    )

    digest = hashlib.sha256()
    for path in paths:
        relative = path.relative_to(repo_root).as_posix().encode("utf-8")
        content = path.read_bytes()
        digest.update(len(relative).to_bytes(4, "big"))
        digest.update(relative)
        digest.update(len(content).to_bytes(8, "big"))
        digest.update(content)
    return digest.hexdigest()
```

Document the exact included paths. A source file that can affect semantics must not be omitted.

### 17.3 Canonical proof-request hash

Use canonical JSON:

```python
def canonical_json_sha256(value: object) -> str:
    payload = json.dumps(
        value,
        sort_keys=True,
        separators=(",", ":"),
        ensure_ascii=True,
        allow_nan=False,
    ).encode("utf-8")
    return hashlib.sha256(payload).hexdigest()
```

Avoid hashes of Python `repr`, unordered sets, absolute temporary paths, or Z3 pretty-printing.

### 17.4 Dirty trees

Local exploratory proofs may run on dirty trees, but default automatic acceptance should reject them unless an exact source-tree hash is stored and policy explicitly permits it. CI proofs should be clean.

### 17.5 Privacy/size

Function bytes and hashes are already project-local inputs. If full result artifacts are large, store content-addressed blobs and put their hashes/locations in the certificate. The certificate must remain self-consistent if external artifacts disappear; absence should make revalidation fail, not silently pass.

### 17.6 Acceptance criteria

- Two identical clean runs produce the same request and engine hashes.
- A one-byte semantic source change invalidates cache/certificate reuse.
- Dependency, contract, relocation, or callee changes invalidate reuse.
- Promotion can explain exactly which validation evidence supported the tier.

---

## P1-09 — Synchronize versioned documentation automatically

**Priority:** P1  
**Primary owner areas:** README generation/checks, CI

### 18.1 Goal

Prevent architecture/result identifiers and support claims from drifting between code and documentation.

### 18.2 Generated markers

In the README:

```markdown
<!-- BEGIN GENERATED PPC_EQUIVALENCE_VERSION -->
- Architecture model: `broadway-ppc32-be-vNEXT`
- Result format: `N`
- Certificate format: `M`
<!-- END GENERATED PPC_EQUIVALENCE_VERSION -->
```

Add a script:

```bash
python -m tools.ppc_equivalence.docs_sync --write
python -m tools.ppc_equivalence.docs_sync --check
```

`--check` exits nonzero when generated sections differ.

### 18.3 Generate more than version numbers

Where feasible, generate:

- `ProofStatus` table;
- supported opcode list from decoder registration;
- result schema version;
- known proof profiles;
- current assumptions sourced from structured constants;
- validation-ledger summary.

Do not generate normative explanatory prose that requires human review.

### 18.4 Tests/CI

- version change without regenerated README fails;
- newly supported opcode without ledger entry fails;
- documented opcode absent from decoder fails;
- result schema example validates against current schema.

---

## P1-10 — Lock dependencies and execution environment by hash

**Priority:** P1/P2  
**Primary owner areas:** packaging, CI

### 19.1 Current state

Exact package versions improve reproducibility, but version pins alone do not guarantee identical artifacts.

### 19.2 Required implementation

Choose one:

- `pip-tools` with `pip-compile --generate-hashes`;
- `uv.lock` with frozen sync;
- another repository-standard lock format that records artifact hashes.

Example CI install:

```bash
python -m pip install --upgrade pip
python -m pip install --require-hashes -r tools/ppc_equivalence/requirements.lock
```

Pin CI action versions and container images by immutable digest where practical.

### 19.3 Supply-chain/reproducibility checks

- lock regeneration is explicit and reviewed;
- CI verifies lock is fresh;
- result provenance records lock hash;
- Dolphin binary/container also pinned;
- no network access is required during the actual proof once environment is provisioned.

### 19.4 Acceptance criteria

- clean environments install identical declared artifacts;
- dependency change invalidates provenance/cache as policy requires;
- lockfile drift fails CI.

---

# Part V — Floating-point and specialized-model work

## P1-11 — Make floating-point proof domains machine-readable

**Priority:** P1/P2  
**Primary owner areas:** FP semantics, result schema, promotion policy, tests

### 20.1 Problem statement

The FP/paired-single model is useful but restricted by assumptions such as nearest-even rounding, NI state, trap exclusion, finite-overflow restrictions, incomplete status flags, and input-shape restrictions for some fused operations.

These restrictions must be represented as predicates and structured proof-domain metadata, not only prose.

### 20.2 Domain object

```python
@dataclass(frozen=True)
class FloatingPointDomain:
    rounding_modes: tuple[str, ...]
    require_ni_zero: bool
    traps_enabled: bool
    allow_nan: bool
    allow_infinity: bool
    allow_subnormal: bool
    exclude_finite_overflow: bool
    model_underflow_flag: bool
    model_inexact_flag: bool
    fused_input_domain: str
```

### 20.3 Result example

```json
{
  "floating_point_scope": {
    "used": true,
    "rounding_modes": ["nearest-even"],
    "ni": "required-zero",
    "traps": "excluded",
    "finite_overflow": "excluded",
    "underflow_flag": "not-fully-modeled",
    "inexact_flag": "not-fully-modeled",
    "fused_input_domain": "exact-expanded-binary32"
  }
}
```

### 20.4 Domain coverage query

For each proof, distinguish:

- no FP instructions;
- FP instructions but restrictions proven automatically from caller/input constraints;
- FP restrictions merely assumed;
- unsupported FP behavior encountered.

Only the second category should be eligible for stronger confidence without manual policy.

### 20.5 Testing

- every excluded domain has a boundary test;
- result includes exactly the restrictions actually used;
- promotion tier falls when assumptions are merely external;
- NaN/sign-zero comparisons follow reference behavior;
- FPSCR output observability is tested independently.

---

# Part VI — Diagnostics and maintainability

## P2-03 — Improve counterexample reproducibility and minimization

**Priority:** P2  
**Primary owner areas:** result/counterexample code, CLI

### 21.1 Required reproduction bundle

For every `NOT_EQUIVALENT` result, emit enough data to replay concretely:

```text
counterexample/
  proof-request.json
  model-values.json
  original.bin
  candidate.bin
  relocations.json
  contract.json
  replay.py-or-command.txt
  original-trace.json
  candidate-trace.json
  first-divergence.json
```

### 21.2 First-divergence diagnostics

After obtaining a model:

1. concretize the initial state;
2. run both sides with ConcreteOps;
3. produce step traces;
4. identify the earliest differing architectural state component;
5. verify that the final witness actually violates the selected contract.

If ConcreteOps cannot reproduce the symbolic SAT witness, classify `INTERNAL_ERROR` and retain all artifacts. Do not publish it as a definite inequivalence without investigation.

### 21.3 Minimize safely

Counterexample minimization must not change the proof domain. Suggested order:

- set irrelevant symbolic registers to zero;
- remove irrelevant memory bytes;
- simplify values while rechecking the SAT witness;
- minimize path/branch selectors.

Always preserve original model and minimized model.

---

## P2-04 — Add solver diagnostics and query audit artifacts

**Priority:** P2

### 22.1 Goal

When performance or correctness is questioned, reviewers need visibility into the actual query.

### 22.2 Optional debug artifacts

- SMT-LIB2 query dump;
- normalized assertion count by category;
- bitvector/array/function symbol counts;
- path-pair count;
- memory-touch count;
- relocation/layout constraint count;
- solver statistics;
- phase timings;
- reason for `unknown`.

### 22.3 Safety

- disabled by default for normal runs;
- deterministic naming by request hash;
- no cache acceptance based on debug files;
- queries may be large, so apply size limits/compression.

### 22.4 Differential query checker

For small generated cases, cross-check with a second formulation where possible, such as full finite-memory concretization or exhaustive enumeration over reduced bit widths/test semantics. This is not a replacement for the production query but can detect construction mistakes.

---

# Part VII — Implementation sequencing and agent assignments

## 23. Recommended parallel work breakdown

### Agent A — Stack soundness fix

Deliver:

- P0-01 code patch;
- asymmetric stack test matrix;
- result metadata for masking semantics;
- stack-focused documentation.

Must coordinate architecture-model bump with Agent D.

### Agent B — Promotion and migration

Deliver:

- emergency automatic-promotion gate;
- `PromotionDecision` policy layer;
- confidence storage;
- registry audit/migration command;
- tests for accepted-state transitions.

Can begin before verifier fix using a default-deny policy.

### Agent C — Deadline/resource control

Deliver:

- shared deadline abstraction;
- no redundant fallback after definitive result;
- subprocess hard timeout if adopted;
- resource metadata and tests.

Independent of stack work.

### Agent D — Versioning, provenance, and certificates

Deliver:

- architecture/result/certificate version bumps;
- source/request/dependency hashes;
- cache invalidation;
- transitive certificate validation changes;
- schema migration tests.

Depends on final schema choices from Agents A/B.

### Agent E — Soundness specification and adversarial tests

Deliver:

- `SOUNDNESS.md`;
- traceability table;
- structured soundness test package;
- definedness decision proposal and tests.

Should review, not merely duplicate, Agent A’s assumptions.

### Agent F — CI and Dolphin automation

Deliver:

- clean generated-DOL Dolphin environment;
- PR/nightly workflow split;
- artifacts and validation ledger integration;
- first public successful run;
- branch-protection instructions.

Can proceed largely in parallel.

### Agent G — Randomized generation and mutation testing

Deliver:

- deterministic generator and seed reproduction;
- three-way ConcreteOps/symbolic/Dolphin checks;
- shrinker;
- targeted mutation suite;
- coverage/mutation reports.

Depends on stable P0 semantics for final baseline but can prototype earlier.

### Agent H — Memory/FP profiles

Deliver:

- memory-environment profile API and constraints;
- machine-readable FP domain;
- promotion-tier hooks;
- boundary tests.

Coordinate with formal specification and result schema.

### Agent I — Documentation/locking/tooling

Deliver:

- generated README sections;
- docs freshness CI;
- hash-locked dependency environment;
- TCB document and validation-ledger schema.

---

## 24. Dependency graph

```text
P0-01 stack fix ───────────────┐
                               ├─> P0-04 model/schema bump ─> registry re-proof
P0-02 promotion kill switch ───┤
                               └─> permanent promotion policy

P0-03 deadline ────────────────────────────────────────────────┐
P1-01 soundness tests ─────────────────────────────────────────┤
P1-02 formal scope ─> P1-03 memory profiles / P1-04 validity ──┤
P1-05 certificates ─> P1-08 provenance ────────────────────────┤
P1-06 validation ledger ─> P1-07 Dolphin CI ───────────────────┤
P1-09 docs sync / P1-10 lock ──────────────────────────────────┤
                                                               └─> unrestricted policy rollout
P2 randomized + mutation + diagnostics ─> ongoing confidence increase
```

---

# Part VIII — Pull-request and review requirements

## 25. Required evidence for every semantic change

Any pull request changing decoder, semantics, stack logic, memory behavior, contracts, engine query construction, certificate interpretation, or promotion policy must include:

1. Architecture-model impact declaration.
2. Normative scope/documentation impact.
3. Positive tests.
4. At least one adversarial negative test.
5. ConcreteOps differential evidence where applicable.
6. Dolphin evidence for changed opcode behavior.
7. Mutation-test impact for critical logic.
8. Cache/certificate invalidation analysis.
9. Promotion-policy impact.
10. Reproduction artifacts for any historical bug fixed.

### 25.1 Model bump policy

Bump the architecture model when a change can alter whether a proof is `EQUIVALENT`, `NOT_EQUIVALENT`, or inconclusive for the same input request. Examples:

- instruction semantics correction;
- observability change;
- stack masking change;
- relocation constraint change;
- FP domain change;
- call summary behavior change;
- validity/exception policy change.

A pure performance optimization with bit-for-bit identical queries and results need not bump the model, but should still alter engine-source provenance.

### 25.2 Result-format bump policy

Bump result format when:

- required fields change;
- field meanings change;
- canonical serialization changes;
- downstream parsers need different behavior.

### 25.3 Certificate-version bump policy

Bump certificate version when:

- signed payload changes;
- validation rules change;
- transitive-root algorithm changes;
- confidence/policy evidence becomes part of acceptance.

---

# Part IX — Detailed acceptance gates

## 26. Gate 0 — Emergency safety

Before any further auto-acceptance:

- [ ] Automatic promotion disabled or affected proof class blocked.
- [ ] Existing `EQUIVALENT_MATCH` inventory exported.
- [ ] No old-model equivalence is silently treated as current evidence.

## 27. Gate 1 — Corrected model

- [ ] Independent per-side stack masking merged.
- [ ] All asymmetric-frame regression tests pass.
- [ ] Historical buggy implementation demonstrably fails those tests.
- [ ] Architecture model bumped.
- [ ] Result/certificate versions bumped as required.
- [ ] Old caches/certificates invalidated.
- [ ] Full existing Python suite passes.

## 28. Gate 2 — Safe promotion restoration

- [ ] Promotion uses `PromotionDecision`, not status alone.
- [ ] Allowed confidence tiers are configured explicitly.
- [ ] Registry migration completed.
- [ ] All affected accepted targets re-proved or downgraded.
- [ ] Workflow state transitions tested.
- [ ] Proof provenance complete enough for revalidation.

## 29. Gate 3 — Public validation

- [ ] Required CI checks have public successful runs.
- [ ] Dolphin generated-DOL corpus passes for current model.
- [ ] Validation ledger is current and hashed.
- [ ] Documentation versions match code.
- [ ] Dependency lock is reproducible.

## 30. Gate 4 — High-confidence maintenance

- [ ] Random differential corpus runs on schedule.
- [ ] Critical mutation suite has no unexplained survivors.
- [ ] Soundness traceability table is complete.
- [ ] Memory and FP profiles are machine-readable.
- [ ] Deadline tests enforce end-to-end bounds.
- [x] Counterexamples replay concretely.

---

# Part X — Suggested result schema

## 31. Example proof result envelope

The exact schema should be reviewed and versioned. This example shows the required separation of solver answer, theorem scope, evidence, and policy:

```json
{
  "result_format": 8,
  "architecture_model": "broadway-ppc32-be-vNEXT",
  "status": "equivalent",
  "solver": {
    "name": "z3",
    "version": "...",
    "answer": "unsat",
    "phases": [
      {"name": "default", "result": "unsat", "elapsed_ms": 123}
    ]
  },
  "scope": {
    "contract": "auto",
    "partial_equivalence": true,
    "observables": {
      "gpr": [3, 4],
      "fpr": [],
      "cr_fields": [0],
      "xer": ["ca"],
      "memory": true,
      "exit": true,
      "definedness": true
    },
    "memory": {
      "comparison": "touched-byte-extensional",
      "profile": "bounded-ordinary-ram",
      "private_stack_masking": "per-implementation-independent-v1"
    },
    "floating_point": {
      "used": false
    },
    "calls": {
      "used": false,
      "callee_root_sha256": null
    }
  },
  "assumptions": [],
  "provenance": {
    "git_commit": "...",
    "dirty": false,
    "engine_tree_sha256": "...",
    "dependency_lock_sha256": "...",
    "proof_request_sha256": "...",
    "original_function_sha256": "...",
    "candidate_function_sha256": "..."
  },
  "validation": {
    "ledger_sha256": "...",
    "dolphin_version": "...",
    "corpus_sha256": "...",
    "all_used_opcodes_independently_validated": true
  },
  "confidence": {
    "tier": "B",
    "policy": "auto-promotion-v2",
    "promotion_allowed": true,
    "blockers": [],
    "warnings": []
  },
  "resources": {
    "deadline_ms": 120000,
    "elapsed_ms": 145
  }
}
```

### 31.1 Design rules

- `status` reports the proof engine’s answer.
- `confidence` reports evidence quality and policy.
- `promotion_allowed` is derived, not trusted if read from an unvalidated artifact.
- assumptions are structured identifiers with parameters, not only prose.
- exact hashes bind every relevant input.
- old readers must reject unknown required semantics rather than guessing.

---

# Part XI — Risk register

## 32. Remaining risks after the P0 fix

| Risk | Why it matters | Mitigation | Residual confidence |
|---|---|---|---|
| Handwritten ISA semantic bug | Z3 proves the wrong formula correctly | Dolphin ledger, reference review, mutation/random differential tests | Medium until broad independent coverage |
| ConcreteOps common-mode bug | Concrete and symbolic paths may share the same conceptual mistake | Independent Dolphin execution | Low/medium depending opcode ledger |
| Unconstrained symbolic pointer interpreted as RAM | Real hardware may fault or access MMIO | Enforceable memory profiles; Tier C otherwise | Explicitly bounded |
| FP legal inputs excluded | Proof may cover only a subset of real inputs | Machine-readable domains, caller-proven constraints, Tier C | Explicitly bounded |
| Callee summary omits dependency/effect | Caller UNSAT may be unsound | Strong certificate contracts, negative tests, transitive hashes | Medium |
| Timeout not truly enforced in native solver | Automation instability | Worker subprocess hard deadline | Low after implementation |
| Documentation drift | Users overstate theorem | Generated sections and traceability CI | Low |
| Stale accepted registry state | Old proof survives model fix | Mandatory migration and model validation | Low |
| Solver/library supply-chain drift | Reproduction mismatch | Hash locks, container digest, provenance | Low |
| Path/instruction limits | Coverage gaps | Exact inconclusive statuses; never promote | Low if fail-closed |
| Exception behavior not modeled | “Equivalent” overstated | Formal partial-equivalence statement and definedness policy | Explicitly bounded |

---

# Part XII — Definition of done

## 33. Project-level completion checklist

The hardening project is complete only when all statements below are true:

### Correctness

- [ ] Private stack intervals are masked independently per implementation.
- [ ] Asymmetric frames cannot hide public writes.
- [ ] Definedness and exception scope are normatively specified.
- [ ] Memory environment is either enforced or explicitly tiered as an assumption.
- [ ] Calls are accepted only with current complete certificate chains.

### Solver operation

- [ ] One global deadline covers every proof phase.
- [ ] A definitive solver answer stops the portfolio immediately.
- [ ] Hard timeout prevents native solver overrun from hanging automation.
- [ ] `unknown` and timeout never become equivalence.

### Testing

- [ ] Historical bug regression passes.
- [ ] Adversarial soundness suite is required in CI.
- [ ] Generated randomized failures are reproducible by seed.
- [ ] Critical mutation set is killed or explicitly reviewed.
- [x] Counterexamples replay under ConcreteOps.

### Independent validation

- [ ] Current model has a successful generated-DOL Dolphin run.
- [ ] Every auto-promotable opcode has ledger evidence.
- [ ] CI artifacts include emulator/corpus hashes.

### Acceptance workflow

- [ ] Proof status alone cannot produce `EQUIVALENT_MATCH` acceptance.
- [ ] Confidence tier and policy decision are stored separately.
- [ ] All old-model accepted equivalence rows were revalidated or downgraded.
- [ ] `FULL_MATCH` remains the strongest static evidence and is not conflated with solver proof provenance.

### Reproducibility

- [ ] Engine tree, request, inputs, dependencies, and validation evidence are hashed.
- [ ] Dependencies and emulator environment are immutable/pinned.
- [ ] Documentation identifiers are generated from code.
- [ ] Caches and certificates reject any mismatched trust input.

---

# Appendix A — Minimal stack-fix patch sketch

This is a patch sketch, not a copy-paste guarantee. Agents must adapt names/types to the pinned source revision.

```diff
 def _memory_difference(initial, left, right, ops):
     differences = []
     for address in touched_addresses(left, right):
         initial_byte = z3.Select(initial.memory, address)
-
-        private = z3.Or(
-            _private_stack_address(
-                address,
-                left.stack_low,
-                initial.gpr[1],
-                left.stack_private,
-                ops,
-            ),
-            _private_stack_address(
-                address,
-                right.stack_low,
-                initial.gpr[1],
-                right.stack_private,
-                ops,
-            ),
-        )
-
-        left_byte = z3.If(private, initial_byte, z3.Select(left.memory, address))
-        right_byte = z3.If(private, initial_byte, z3.Select(right.memory, address))
+
+        left_is_private = _private_stack_address(
+            address,
+            left.stack_low,
+            initial.gpr[1],
+            left.stack_private,
+            ops,
+        )
+        right_is_private = _private_stack_address(
+            address,
+            right.stack_low,
+            initial.gpr[1],
+            right.stack_private,
+            ops,
+        )
+
+        # Private storage is an implementation-local abstraction. An address
+        # private to one implementation must not mask a public write by the
+        # other implementation.
+        left_byte = z3.If(
+            left_is_private,
+            initial_byte,
+            z3.Select(left.memory, address),
+        )
+        right_byte = z3.If(
+            right_is_private,
+            initial_byte,
+            z3.Select(right.memory, address),
+        )
 
         differences.append(left_byte != right_byte)
 
     return z3.Or(*differences) if differences else z3.BoolVal(False)
```

---

# Appendix B — Promotion-policy test examples

```python
def test_equivalent_status_is_not_sufficient_for_promotion():
    proof = make_result(
        status=ProofStatus.EQUIVALENT,
        architecture_model="broadway-ppc32-be-v18",
        private_stack_masking="union-mask-legacy",
    )

    decision = classify_for_promotion(proof, strict_policy(), validation_ledger())

    assert decision.allowed is False
    assert "architecture-model-not-allowed" in decision.blockers
    assert "unsafe-or-unknown-private-stack-masking" in decision.blockers


def test_full_match_does_not_depend_on_equivalence_promotion():
    evaluation = classify_match(
        function_match_percent=100.0,
        proof=None,
    )

    assert evaluation.status == "FULL_MATCH"


def test_tier_b_requires_independent_opcode_validation():
    proof = ordinary_memory_proof(all_opcodes=("lwz", "stw"))
    ledger = ledger_with_dolphin_validation("lwz")  # stw missing

    decision = classify_for_promotion(proof, strict_policy(), ledger)

    assert decision.allowed is False
    assert decision.confidence_tier == "C"
    assert "confidence-tier-C-not-allowed" in decision.blockers
```

---

# Appendix C — Registry migration pseudocode

```python
def audit_equivalent_matches(registry, current_policy, proof_store):
    changes = []

    for target in registry.targets:
        if target.status != "EQUIVALENT_MATCH":
            continue

        certificate = target.equivalence_certificate
        error = validate_certificate(certificate, registry.rows_by_id)

        if error is not None:
            changes.append(require_revalidation(target, error))
            continue

        proof = proof_store.load(certificate["proof_request_sha256"])
        decision = classify_for_promotion(
            proof,
            current_policy,
            load_validation_ledger(),
        )

        if not decision.allowed:
            changes.append(
                require_revalidation(
                    target,
                    ",".join(decision.blockers),
                )
            )

    return changes
```

Migration output must be reviewed before write mode and support `--dry-run`.

---

# Appendix D — Suggested new files

```text
tools/ppc_equivalence/
  SOUNDNESS.md
  TRUSTED_COMPUTING_BASE.md
  validation_ledger.yaml
  result_schema.json
  requirements.in
  requirements.lock
  docs_sync.py
  deadline.py
  environment.py
  provenance.py
  promotion.py                  # or place integration-specific policy in tools/coop
  tests/
    soundness/
      test_memory_masking.py
      test_observability_contracts.py
      test_definedness.py
      test_control_flow.py
      test_relocations.py
      test_calls_and_certificates.py
      test_floating_point_domains.py
      test_promotion_policy.py
    randomized/
      generator.py
      replay.py
      shrink.py
    mutation/
      targeted_mutations.yaml

tools/coop/
  lib/
    equivalence_policy.py
  commands/
    audit_equivalence_registry.py
```

Use the repository’s existing organization conventions where they differ; avoid duplicate policy implementations between `tools/ppc_equivalence` and `tools/coop`.

---

# Appendix E — Source links used for this plan

- [`tools/ppc_equivalence/README.md`](https://github.com/Aelbannan/xenoblade/blob/main/tools/ppc_equivalence/README.md)
- [`tools/ppc_equivalence/engine.py`](https://github.com/Aelbannan/xenoblade/blob/main/tools/ppc_equivalence/engine.py)
- [`tools/ppc_equivalence/semantics.py`](https://github.com/Aelbannan/xenoblade/blob/main/tools/ppc_equivalence/semantics.py)
- [`tools/ppc_equivalence/result.py`](https://github.com/Aelbannan/xenoblade/blob/main/tools/ppc_equivalence/result.py)
- [`tools/ppc_equivalence/contract.py`](https://github.com/Aelbannan/xenoblade/blob/main/tools/ppc_equivalence/contract.py)
- [`tools/ppc_equivalence/tests`](https://github.com/Aelbannan/xenoblade/tree/main/tools/ppc_equivalence/tests)
- [`tools/coop/lib/equivalence_check.py`](https://github.com/Aelbannan/xenoblade/blob/main/tools/coop/lib/equivalence_check.py)
- [`tools/coop/lib/objdiff_report.py`](https://github.com/Aelbannan/xenoblade/blob/main/tools/coop/lib/objdiff_report.py)
- [`tools/coop/lib/targets.py`](https://github.com/Aelbannan/xenoblade/blob/main/tools/coop/lib/targets.py)
- [`.github/workflows/build.yml`](https://github.com/Aelbannan/xenoblade/blob/main/.github/workflows/build.yml)
- [`tools/ppc_equivalence/REFERENCES.md`](https://github.com/Aelbannan/xenoblade/blob/main/tools/ppc_equivalence/REFERENCES.md)
- [`tools/ppc_equivalence/requirements.txt`](https://github.com/Aelbannan/xenoblade/blob/main/tools/ppc_equivalence/requirements.txt)

---

## Final implementation principle

The project should preserve automatic promotion, but only after making the promotion a conclusion of a complete evidence policy:

```text
current sound model
+ explicit theorem scope
+ definitive UNSAT
+ valid contract
+ valid environment profile
+ current transitive certificates
+ independent opcode validation
+ complete provenance
+ allowed confidence tier
= automatic EQUIVALENT_MATCH acceptance
```

Anything less may still be useful diagnostic evidence, but it must not silently become an accepted equivalence claim.
