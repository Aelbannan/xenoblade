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

- [ ] Promotion uses `PromotionDecision`, not status alone.
- [ ] Allowed confidence tiers are configured explicitly.
- [ ] Registry migration completed.
- [ ] All affected accepted targets re-proved or downgraded.
- [ ] Workflow state transitions tested.
- [ ] Proof provenance complete enough for revalidation.

## Gate 3 — Public validation

- [ ] Required CI checks have public successful runs.
- [ ] Dolphin generated-DOL corpus passes for current model.
- [ ] Validation ledger is current and hashed.
- [ ] Documentation versions match code.
- [ ] Dependency lock is reproducible.

## Gate 4 — High-confidence maintenance

- [ ] Random differential corpus runs on schedule.
- [ ] Critical mutation suite has no unexplained survivors.
- [ ] Soundness traceability table is complete.
- [ ] Memory and FP profiles are machine-readable.
- [ ] Deadline tests enforce end-to-end bounds.
- [ ] Counterexamples replay concretely.
