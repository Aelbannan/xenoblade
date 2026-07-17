# P0-02 — Add a promotion safety gate and migrate accepted results

**Priority:** P0 / release blocker  
**Primary owner areas:** `tools/coop/lib/objdiff_report.py`, `tools/coop/lib/targets.py`, `tools/coop/lib/equivalence_check.py`  
**Dependencies:** coordinate with P0-01 and P0-04

---

## Current behavior

The integration currently has two important transitions:

1. `objdiff_report.py` classifies a function as `EQUIVALENT_MATCH` when its fuzzy match is above the configured floor and the proof status is `ProofStatus.EQUIVALENT`.
2. `targets.py` treats `EQUIVALENT_MATCH` as an accepted match status and moves workflow state to `ACCEPTED`.

A raw proof status is therefore sufficient to trigger project acceptance after the fuzzy-match threshold. That policy is too coarse for assumption-heavy or affected proofs.

## Immediate mitigation

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

## Permanent promotion decision model

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

## Suggested match-state representation

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

## Confidence tiers

Use explicit, machine-computed tiers. A suggested initial policy:

### Tier A — narrow, strongest modeled proofs

- integer/bitvector operations only;
- no memory access;
- no calls or opaque summaries;
- no floating-point or paired-single operations;
- no cache, MMU, system, privileged, timing, or exception-sensitive operations;
- all opcodes in the proof have current independent Dolphin validation;
- no non-default environmental assumption beyond ordinary initial architectural state;
- current architecture model, result format, and complete provenance.

### Tier B — ordinary compiler-generated memory/function proofs

- aligned ordinary-RAM accesses only;
- corrected independent private-stack masking;
- no possible MMIO/unmapped/protection-sensitive addresses under the selected memory profile;
- calls absent or all callees covered by current transitive certificates;
- no unsupported behavioral side effects;
- current independent validation for all used opcodes;
- current architecture model and complete provenance.

### Tier C — assumption-heavy proofs

Any of:

- floating point or paired-single domain restrictions;
- call summaries with nontrivial environmental premises;
- system/cache/Special Purpose Register behavior that models values but not full hardware effects;
- ordinary-RAM behavior assumed rather than constrained;
- incomplete independent Dolphin validation;
- other declared restrictions with meaningful legal-input exclusions.

Tier C may still be valuable and may remain visible as `EQUIVALENT_MATCH`, but default unattended acceptance should initially allow only A and carefully defined B.

## Migration of existing accepted targets

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

## Cache and certificate handling

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

## Acceptance criteria

- A raw `ProofStatus.EQUIVALENT` can no longer directly imply acceptance.
- Promotion decisions contain explicit blockers and confidence tier.
- Old-model `EQUIVALENT_MATCH` entries are audited and cannot remain silently accepted.
- Full-match behavior remains unchanged.
- Registry migration is idempotent and covered by tests.
- Attempt history remains append-only and preserves old evidence.

## Promotion-policy test examples

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

## Registry migration pseudocode

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
