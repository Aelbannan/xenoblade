# Risk register — remaining risks after the P0 fix

| Risk | Why it matters | Mitigation | Residual confidence |
|---|---|---|---|
| Handwritten ISA semantic bug | Z3 proves the wrong formula correctly | Dolphin ledger, reference review, mutation/random differential tests | Medium until broad independent coverage |
| ConcreteOps common-mode bug | Concrete and symbolic paths may share the same conceptual mistake | Independent Dolphin execution | Low/medium depending opcode ledger |
| Unconstrained symbolic pointer interpreted as RAM | Real hardware may fault or access MMIO | Enforceable memory profiles; Tier C otherwise | Explicitly bounded |
| FP legal inputs excluded | Proof may cover only a subset of real inputs | Machine-readable domains, caller-proven constraints, Tier C | Explicitly bounded |
| Callee summary omits dependency/effect | Caller UNSAT may be unsound | Strong certificate contracts, negative tests, transitive hashes | Medium |
| Timeout not truly enforced in native solver | Automation instability | Worker subprocess hard deadline | Low after implementation |
| Documentation drift | Users overstate theorem | Generated sections and traceability CI | Low |
| Stale accepted registry state | Old proof survives model fix | Mandatory migration and model validation | Low |
| Solver/library supply-chain drift | Reproduction mismatch | Hash locks, container digest, provenance | Low |
| Path/instruction limits | Coverage gaps | Exact inconclusive statuses; never promote | Low if fail-closed |
| Exception behavior not modeled | "Equivalent" overstated | Formal partial-equivalence statement and definedness policy | Explicitly bounded |
