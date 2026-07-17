# Phase 0 — Promotion Safety

**Source:** Section 8 of the LLM Decompilation Improvement Plan  
**Covers:** Baseline capture, candidate ranking, promotion gate, placeholder detection, locks, revalidation, transactional writes, rollback, validation steps, promotion journal  
**PR:** 1 — Baseline-aware transactional promotion

---

## 8.1 Capture and evaluate a baseline

Before any model call:

1. Read the current canonical target source.
2. Hash it.
3. Build/evaluate it using the same path used for generated candidates.
4. Store its result in the experiment state.
5. Include it as a ranking candidate.

Pseudo-code:

```python
def capture_baseline(
    adapter: ProjectAdapter,
    target_id: str,
    experiment_dir: Path,
) -> BaselineSnapshot:
    source = adapter.read_target_source(target_id)
    source_hash = sha256_text(source)

    evaluation = adapter.evaluate_source(
        target_id=target_id,
        source=source,
        workspace_root=adapter.root,
        artifact_dir=experiment_dir / "baseline",
    )

    return BaselineSnapshot(
        target_id=target_id,
        source_text=source,
        source_hash=source_hash,
        object_hash=evaluation.object_hash,
        evaluation=evaluation,
        captured_at=utc_now(),
        root_revision=git_head_or_none(adapter.root),
        dirty_paths=git_dirty_paths(adapter.root),
    )
```

If evaluating canonical source requires no patch, add a dedicated adapter method rather than fabricating a candidate patch.

## 8.2 Separate experiment winner from promotion winner

```python
generated_winner = max(
    generated_candidates,
    key=rank_candidate,
)

decision = compare_for_promotion(
    baseline=baseline.evaluation,
    candidate=generated_winner.evaluation,
    policy=config.promotion,
)
```

The generated winner must always be written to experiment evidence even if it is not promoted.

## 8.3 Candidate ranking

Use lexicographic ranking for hard priorities and weighted scoring only within comparable tiers.

Recommended rank:

```python
def rank_candidate(e: CandidateEvaluation) -> tuple:
    return (
        int(not e.object_regressions),
        semantic_tier(e),
        structural_bucket(e),
        round(e.match_percent, 4),
        int(size_is_acceptable(e)),
        -positive_size_delta(e),
        -warning_count(e),
    )
```

Suggested semantic tier:

```python
def semantic_tier(e: CandidateEvaluation) -> int:
    if e.status == CandidateStatus.FULL_MATCH:
        return 5
    if e.status == CandidateStatus.EQUIVALENT_MATCH:
        return 4
    if (
        e.compile_report.succeeded
        and e.structural_report is not None
        and e.structural_report.total_score >= 0.75
    ):
        return 3
    if e.compile_report.succeeded:
        return 2
    if e.status == CandidateStatus.BLOCKED:
        return 1
    return 0
```

Do not make fuzzy match percentage outrank a sound equivalence proof.

## 8.4 Promotion gate

```python
@dataclass(frozen=True)
class PromotionPolicy:
    enabled: bool = True
    require_monotonic_rank: bool = True
    allow_first_compile: bool = True
    first_compile_min_structural_score: float = 0.60
    structural_alignment_threshold: float = 0.75
    protect_accepted_functions: bool = True
    revalidate_against_latest_root: bool = True
    rollback_on_failure: bool = True
```

```python
def passes_promotion_gate(
    baseline: CandidateEvaluation,
    candidate: CandidateEvaluation,
    policy: PromotionPolicy,
    baseline_is_placeholder: bool,
) -> tuple[bool, str]:
    if not policy.enabled:
        return False, "automatic promotion is disabled"

    if candidate.object_regressions:
        return False, "candidate has object regressions"

    if candidate.accepted_function_regressions:
        return False, "candidate regresses accepted functions"

    candidate_rank = rank_candidate(candidate)
    baseline_rank = rank_candidate(baseline)

    if candidate.status == CandidateStatus.FULL_MATCH:
        return True, "full match"

    if candidate.status == CandidateStatus.EQUIVALENT_MATCH:
        if candidate_rank > baseline_rank:
            return True, "proved equivalent and rank improved"
        return False, "equivalent candidate did not improve canonical rank"

    if candidate.compile_report.succeeded and not baseline.compile_report.succeeded:
        score = structural_score(candidate)
        if (
            policy.allow_first_compile
            and score >= policy.first_compile_min_structural_score
        ):
            return True, "first compiling structurally plausible implementation"

    if baseline_is_placeholder and candidate.compile_report.succeeded:
        score = structural_score(candidate)
        if score >= policy.first_compile_min_structural_score:
            return True, "replaces placeholder with plausible compiling implementation"

    if policy.require_monotonic_rank and candidate_rank <= baseline_rank:
        return False, "candidate does not improve canonical rank"

    return True, "candidate improves canonical rank"
```

## 8.5 Placeholder detection

Do not rely only on zero match.

Support configured and heuristic detection:

```python
class PlaceholderDetector:
    def is_placeholder(self, source: str, metadata: dict) -> bool:
        if metadata.get("placeholder") is True:
            return True

        normalized = remove_comments_and_whitespace(source)

        known_patterns = (
            "return 0;",
            "return false;",
            "{}",
            "abort();",
            "UNIMPLEMENTED",
        )
        return any(pattern in normalized for pattern in known_patterns)
```

Prefer explicit target metadata over heuristic detection.

## 8.6 Promotion lock

Add a dedicated lock:

```python
class Harness:
    def __init__(self, ...):
        ...
        self._adapter_lock = threading.RLock()
        self._promotion_lock = threading.Lock()
```

Do not reuse `_adapter_lock` without carefully auditing its lock ordering. A separate lock makes canonical mutation obvious.

All canonical mutation must occur inside:

```python
with self._promotion_lock:
    result = self.promotion_manager.promote(...)
```

## 8.7 Revalidate against the latest root

A candidate built in an isolated worktree may be stale after other promotions.

Inside the promotion lock:

1. Re-read canonical source.
2. Re-hash target and owning file.
3. Capture a fresh baseline if the source changed.
4. Apply the candidate to a fresh temporary worktree based on the latest root.
5. Re-evaluate it.
6. Re-run the promotion comparison.
7. Only then write to canonical source.

```python
def prepare_latest_candidate(
    target_id: str,
    candidate_patch: SourcePatch,
) -> PreparedPromotion:
    latest_baseline = capture_baseline(...)
    workspace = workspace_manager.create_from_current_root(...)

    adapter.apply_patch(workspace, candidate_patch)
    latest_evaluation = adapter.evaluate(workspace, target_id)

    return PreparedPromotion(
        baseline=latest_baseline,
        candidate_evaluation=latest_evaluation,
        workspace=workspace,
    )
```

## 8.8 Transactional write and rollback

Before mutation:

- Save original file bytes.
- Save mode bits if relevant.
- Record current hash.
- Record any generated state files that promotion changes.
- Record affected object artifact paths if the build overwrites shared output.

After mutation:

1. Build the owning object.
2. Re-evaluate the target.
3. Verify protected functions.
4. Run `coop cycle`.
5. Confirm success exit codes.
6. Confirm source still contains the expected candidate.
7. Write promotion journal.

On failure:

- Restore original file atomically.
- Rebuild or clean affected object output if required.
- Re-run a minimal baseline validation.
- Record rollback.
- Surface rollback failures prominently.

```python
@contextmanager
def source_transaction(path: Path):
    original = path.read_bytes()
    original_mode = path.stat().st_mode

    try:
        yield
    except BaseException:
        atomic_write_bytes(path, original)
        os.chmod(path, stat.S_IMODE(original_mode))
        raise
```

Use temporary files plus `os.replace` for atomic source writes.

## 8.9 Promotion validation steps

Represent every step explicitly:

```python
@dataclass
class ValidationStepResult:
    name: str
    succeeded: bool
    exit_code: int | None
    detail: str
    artifact_paths: list[str]
```

Default steps:

```text
1. source_hash_precondition
2. apply_candidate
3. compile_owning_object
4. evaluate_target
5. verify_target_rank
6. verify_protected_functions
7. verify_object_size/layout constraints
8. coop_cycle
9. source_hash_postcondition
10. commit_promotion_journal
```

## 8.10 Promotion journal

Write to:

```text
build/llm-harness/promotions/YYYY-MM-DD/<promotion-id>.json
```

Example:

```json
{
  "schema_version": 1,
  "promotion_id": "prom-20260717T120000Z-abcd1234",
  "target_id": "SomeClass__func",
  "experiment_id": "20260717T115800Z-1234abcd",
  "candidate_id": "candidate-07",
  "source_file": "src/...",
  "baseline_source_hash": "sha256:...",
  "candidate_source_hash": "sha256:...",
  "baseline_rank": [1, 2, 31.4, 1, 0, 0],
  "candidate_rank": [1, 3, 72.0, 1, 0, 0],
  "reason": "structural alignment and match improved",
  "validation_steps": [],
  "promoted": true,
  "rolled_back": false,
  "timestamp": "2026-07-17T12:00:00Z"
}
```
