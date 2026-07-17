# Implementation sequencing and agent assignments

## Recommended parallel work breakdown

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

**TODO (deferred from P0-04):** Add a migration-report subcommand that enumerates every registry row with a stale `equivalence_certificate` (wrong version/architecture/format). Deployed when the first `EQUIVALENT_MATCH` certificate is created; until then there is nothing to migrate.

### Agent E — Soundness specification and adversarial tests

Deliver:

- `SOUNDNESS.md`;
- traceability table;
- structured soundness test package;
- definedness decision proposal and tests.

Should review, not merely duplicate, Agent A's assumptions.

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

## Dependency graph

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
