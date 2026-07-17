# P2-03 — Improve counterexample reproducibility and minimization

**Priority:** P2  
**Primary owner areas:** result/counterexample code, CLI

---

## Required reproduction bundle

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

## First-divergence diagnostics

After obtaining a model:

1. concretize the initial state;
2. run both sides with ConcreteOps;
3. produce step traces;
4. identify the earliest differing architectural state component;
5. verify that the final witness actually violates the selected contract.

If ConcreteOps cannot reproduce the symbolic SAT witness, classify `INTERNAL_ERROR` and retain all artifacts. Do not publish it as a definite inequivalence without investigation.

## Minimize safely

Counterexample minimization must not change the proof domain. Suggested order:

- set irrelevant symbolic registers to zero;
- remove irrelevant memory bytes;
- simplify values while rechecking the SAT witness;
- minimize path/branch selectors.

Always preserve original model and minimized model.
