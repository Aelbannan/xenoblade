# `ppc_equivalence` Trustworthiness and Correctness Work Plan — Step-Sized Documents

This directory contains the implementation-step-sized breakdown of [`docs/ppc_equivalence_recommended_work.md`](../ppc_equivalence_recommended_work.md), the full verifier audit hardening plan.

Each document is self-contained for a single agent/implementation step.

---

## Release-blocking (P0)

| Doc | Title | Priority | Area |
|---|---|---|---|
| [01](01-P0-01-stack-masking-fix.md) | P0-01 — Fix asymmetric private-stack memory masking | P0 | `engine.py` |
| [02](02-P0-02-promotion-gate.md) | P0-02 — Add a promotion safety gate and migrate accepted results | P0 | `objdiff_report.py`, `targets.py`, `equivalence_check.py` |
| [03](03-P0-03-proof-deadline.md) | P0-03 — Enforce a real end-to-end proof deadline | P0 | `engine.py` |
| [04](04-P0-04-version-invalidation.md) | P0-04 — Version the semantic fix and invalidate stale evidence | P0 | `result.py`, certificates, caches |

## Soundness and specification hardening (P1)

| Doc | Title | Priority | Area |
|---|---|---|---|
| [05](05-P1-01-soundness-regression-suite.md) | P1-01 — Build an adversarial soundness-regression suite | P1 | tests |
| [06](06-P1-02-proof-scope-spec.md) | P1-02 — Write a formal, auditable proof-scope specification | P1 | `SOUNDNESS.md` |
| [07](07-P1-03-memory-profiles.md) | P1-03 — Add enforceable memory-environment profiles | P1 | engine, CLI, schema |
| [08](08-P1-04-definedness-exception.md) | P1-04 — Clarify and test definedness and exception semantics | P1 | semantics, engine, `SOUNDNESS.md` |
| [09](09-P1-05-call-certificates.md) | P1-05 — Strengthen call summaries and certificate soundness | P1 | `semantics.py`, `equivalence_check.py`, `targets.py` |
| [10](10-P1-06-tcb-audit.md) | P1-06 — Audit the trusted computing base instruction by instruction | P1/P2 | validation ledger |
| [11](11-P1-07-dolphin-ci.md) | P1-07 — Make the Dolphin interpreter corpus an automated gate | P1 | CI, fixture generator |
| [14](14-P1-08-proof-provenance.md) | P1-08 — Add complete proof provenance | P1 | schema, certificates, CLI |
| [15](15-P1-09-docs-sync.md) | P1-09 — Synchronize versioned documentation automatically | P1 | README, CI |
| [16](16-P1-10-dependency-lock.md) | P1-10 — Lock dependencies and execution environment by hash | P1/P2 | packaging, CI |
| [17](17-P1-11-floating-point-domains.md) | P1-11 — Make floating-point proof domains machine-readable | P1/P2 | FP semantics, schema |

## Independent validation / randomized testing (P2)

| Doc | Title | Priority | Area |
|---|---|---|---|
| [12](12-P2-01-randomized-generation.md) | P2-01 — Add deterministic randomized differential generation | P2 | generators, tests |
| [13](13-P2-02-mutation-testing.md) | P2-02 — Add mutation testing focused on semantic soundness | P2 | test tooling |
| [18](18-P2-03-counterexample-repro.md) | P2-03 — Improve counterexample reproducibility and minimization | P2 | result/counterexample code |
| [19](19-P2-04-solver-diagnostics.md) | P2-04 — Add solver diagnostics and query audit artifacts | P2 | diagnostics |

## Supporting documents

| Doc | Description |
|---|---|
| [20](20-implementation-sequencing.md) | Agent assignments and dependency graph |
| [21](21-pr-review-requirements.md) | PR/review requirements and bump policies |
| [22](22-acceptance-gates.md) | Gate-by-gate acceptance checklist |
| [23](23-result-schema.md) | Suggested proof result envelope schema |
| [24](24-risk-register.md) | Risk register after P0 fix |
| [25](25-definition-of-done.md) | Project-level completion checklist |
| [26](26-appendices.md) | Suggested new files, trustworthy result definition, repository map |

---

**Source:** [`docs/ppc_equivalence_recommended_work.md`](../ppc_equivalence_recommended_work.md) (audit snapshot 2026-07-17)
