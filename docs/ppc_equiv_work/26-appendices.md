# Appendices

## Appendix A — Suggested new files

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

Use the repository's existing organization conventions where they differ; avoid duplicate policy implementations between `tools/ppc_equivalence` and `tools/coop`.

## Appendix B — Trustworthy result definition

A trustworthy proof requires all of the following:

1. **Correct theorem statement** — the project knows exactly what "equivalent" means.
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

## Appendix C — Repository map and affected trust boundary

| Area | Current location | Why it matters |
|---|---|---|
| Equivalence query and memory comparison | `tools/ppc_equivalence/engine.py` | Builds terminal-state differences, stack masking, solver queries, layout checks, and result classification. |
| Instruction semantics and machine state | `tools/ppc_equivalence/semantics.py` | Defines symbolic/concrete architectural behavior, state validity, memory touches, stack tracking, calls, and instruction effects. |
| Result schema and architecture identifier | `tools/ppc_equivalence/result.py` | Defines `ProofStatus`, architecture model, result format, assumptions, and serialized proof evidence. |
| Contract construction | `tools/ppc_equivalence/contract.py` | Determines observable live-outs and proof scope. |
| Decoder | `tools/ppc_equivalence/decoder.py` | Converts PPC instruction words into semantic IR and validates supported encodings. |
| CLI | `tools/ppc_equivalence/run.py` | User-facing proof execution and self-test entry points. |
| Unit/soundness tests | `tools/ppc_equivalence/tests` | Main regression suite. |
| Proof/certificate integration | `tools/coop/lib/equivalence_check.py` | Runs proofs, creates certificates, caches outcomes, loads callee premises. |
| Match classification | `tools/coop/lib/objdiff_report.py` | Turns `ProofStatus.EQUIVALENT` plus fuzzy-match threshold into `EQUIVALENT_MATCH`. |
| Registry acceptance and certificate validation | `tools/coop/lib/targets.py` | Treats `EQUIVALENT_MATCH` as accepted and validates stored certificates. |
| CI | `.github/workflows/build.yml` | Runs current Python/ConcreteOps checks; does not currently execute the Dolphin gate. |
| Tool documentation | `tools/ppc_equivalence/README.md` | Public theorem/scope and usage claims. |
| Reference hierarchy | `tools/ppc_equivalence/REFERENCES.md` | Records ISA and Broadway evidence sources. |
| Python dependencies | `tools/ppc_equivalence/requirements.txt` | Exact package versions are pinned, but artifacts are not currently hash-locked. |

### Current version mismatch to resolve

At the audit snapshot, the implementation identifies:

- architecture model `broadway-ppc32-be-v18`;
- result format `7`.

The README still describes older identifiers (`v15` and result format `4`). Version synchronization is therefore already demonstrably necessary, not merely preventative.
