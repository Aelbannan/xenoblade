# Definition of done

The hardening project is complete only when all statements below are true:

## Correctness

- [x] Private stack intervals are masked independently per implementation.
- [x] Asymmetric frames cannot hide public writes.
- [x] Definedness and exception scope are normatively specified (`SOUNDNESS.md`).
- [x] Memory environment is either enforced or explicitly tiered as an assumption
      (`stack-and-known-globals` / `hardware-aware` fail closed without ranges).
- [x] Calls are accepted only with current complete certificate chains.

## Solver operation

- [x] One global deadline covers every proof phase.
- [x] A definitive solver answer stops the portfolio immediately.
- [ ] Hard timeout prevents native solver overrun from hanging automation
      (subprocess hard-kill still deferred).
- [x] `unknown` and timeout never become equivalence.

## Testing

- [x] Historical bug regression passes.
- [x] Adversarial soundness suite is required in CI
      (`tools/ppc_equivalence/tests/soundness/` + discover step).
- [x] Generated randomized failures are reproducible by seed.
- [x] Critical mutation set is killed or explicitly reviewed.
- [x] Counterexamples replay under ConcreteOps.

## Independent validation

- [ ] Current model has a successful generated-DOL Dolphin run.
- [x] Every auto-promotable opcode has ledger evidence.
- [ ] CI artifacts include emulator/corpus hashes.
- [x] Schedule Dolphin gate is honest: hard-fail when present, skip when absent;
      not required on every PR.

## Acceptance workflow

- [x] Proof status alone cannot produce `EQUIVALENT_MATCH` acceptance.
- [x] Confidence tier and policy decision are stored separately.
- [x] All old-model accepted equivalence rows were revalidated or downgraded
      (audit command; currently zero affected rows).
- [x] `FULL_MATCH` remains the strongest static evidence and is not conflated with solver proof provenance.

## Reproducibility

- [x] Engine tree, request, inputs, dependencies, and validation evidence are hashed
      (generators + fixtures/*.py + optional ledger included in engine tree).
- [x] Dependencies and emulator environment are immutable/pinned
      (hash lock in CI; emulator pin still external).
- [x] Documentation identifiers are generated from code (`docs_sync`).
- [x] Caches and certificates reject any mismatched trust input.
