# Acceptance gates

## Gate 0 — Emergency safety

Before any further auto-acceptance:

- [x] Automatic promotion disabled or affected proof class blocked.
- [x] Existing `EQUIVALENT_MATCH` inventory exported.
- [x] No old-model equivalence is silently treated as current evidence.

## Gate 1 — Corrected model

- [x] Independent per-side stack masking merged.
- [x] All asymmetric-frame regression tests pass.
- [x] Historical buggy implementation demonstrably fails those tests.
- [x] Architecture model bumped.
- [x] Result/certificate versions bumped as required.
- [x] Old caches/certificates invalidated.
- [x] Full existing Python suite passes.

## Gate 2 — Safe promotion restoration

- [x] Promotion uses `PromotionDecision`, not status alone.
- [x] Allowed confidence tiers are configured explicitly.
- [x] Registry migration completed (`targets audit-promotion` /
      `equivalence audit-registry`; currently zero live `EQUIVALENT_MATCH` rows).
- [x] All affected accepted targets re-proved or downgraded (vacuous: none).
- [x] Workflow state transitions tested (`test_audit_promotion`).
- [x] Proof provenance complete enough for revalidation (engine tree covers
      package + generators + fixtures/*.py + optional validation ledger).

## Gate 3 — Public validation

- [ ] Required CI checks have public successful runs.
- [ ] Dolphin generated-DOL corpus passes for current model.
- [x] Validation ledger is current and hashed.
- [x] Documentation versions match code (`docs_sync --check` in CI).
- [x] Dependency lock is reproducible (multi-platform `--require-hashes` lock).
- [x] Schedule/workflow_dispatch Dolphin step fails hard when Dolphin is present;
      skips clearly when unavailable (not required on every PR).

## Gate 4 — High-confidence maintenance

- [x] Random differential corpus runs on schedule
      (`generate --seed 1 --count 16 --differential` on schedule/dispatch).
- [x] Critical mutation suite has no unexplained survivors (19 targeted kills).
- [ ] Soundness traceability table is complete (CI-enforced row↔impl links).
- [x] Memory and FP profiles are machine-readable (`environment`,
      `floating_point_domain.coverage`).
- [x] Deadline tests enforce end-to-end bounds.
- [x] Counterexamples replay concretely (traces + minimization).
