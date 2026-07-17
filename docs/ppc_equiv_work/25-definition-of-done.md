# Definition of done

The hardening project is complete only when all statements below are true:

## Correctness

- [ ] Private stack intervals are masked independently per implementation.
- [ ] Asymmetric frames cannot hide public writes.
- [ ] Definedness and exception scope are normatively specified.
- [ ] Memory environment is either enforced or explicitly tiered as an assumption.
- [ ] Calls are accepted only with current complete certificate chains.

## Solver operation

- [ ] One global deadline covers every proof phase.
- [ ] A definitive solver answer stops the portfolio immediately.
- [ ] Hard timeout prevents native solver overrun from hanging automation.
- [ ] `unknown` and timeout never become equivalence.

## Testing

- [ ] Historical bug regression passes.
- [ ] Adversarial soundness suite is required in CI.
- [ ] Generated randomized failures are reproducible by seed.
- [ ] Critical mutation set is killed or explicitly reviewed.
- [ ] Counterexamples replay under ConcreteOps.

## Independent validation

- [ ] Current model has a successful generated-DOL Dolphin run.
- [ ] Every auto-promotable opcode has ledger evidence.
- [ ] CI artifacts include emulator/corpus hashes.

## Acceptance workflow

- [ ] Proof status alone cannot produce `EQUIVALENT_MATCH` acceptance.
- [ ] Confidence tier and policy decision are stored separately.
- [ ] All old-model accepted equivalence rows were revalidated or downgraded.
- [ ] `FULL_MATCH` remains the strongest static evidence and is not conflated with solver proof provenance.

## Reproducibility

- [ ] Engine tree, request, inputs, dependencies, and validation evidence are hashed.
- [ ] Dependencies and emulator environment are immutable/pinned.
- [ ] Documentation identifiers are generated from code.
- [ ] Caches and certificates reject any mismatched trust input.
