# P1-06 — Audit the trusted computing base instruction by instruction

**Priority:** P1/P2  
**Primary output:** validation ledger and review records

---

## Trusted computing base inventory

Create `tools/ppc_equivalence/TRUSTED_COMPUTING_BASE.md` covering:

- decoder field extraction and reserved-bit checks;
- IR construction;
- symbolic operations;
- ConcreteOps;
- memory load/store composition;
- control flow and terminal pairing;
- contracts;
- relocations/layout;
- FP tables/helpers;
- stack analysis;
- callee summaries/certificates;
- query construction;
- solver and Python/runtime dependencies;
- promotion policy.

## Per-opcode validation ledger

Create a machine-readable file, e.g.:

```yaml
architecture_model: broadway-ppc32-be-vNEXT
corpus_version: 1
opcodes:
  add:
    decoder_unit: true
    concrete_symbolic_differential: true
    dolphin_interpreter: true
    boundary_vectors: true
    reviewer: agent-id-or-human
    evidence_sha256: "..."
  psq_l:
    decoder_unit: true
    concrete_symbolic_differential: true
    dolphin_interpreter: false
    boundary_vectors: partial
    restrictions:
      - "selected GQR domain only"
```

Promotion computes confidence from this ledger and the exact opcodes used by the proof.

## Review procedure

For each instruction family:

1. Identify primary ISA reference and Broadway/Dolphin evidence.
2. Review field extraction and invalid encodings.
3. Review every state read/write.
4. Enumerate boundary cases.
5. Compare ConcreteOps and symbolic singleton behavior.
6. Execute generated fixtures in Dolphin interpreter.
7. Record evidence hash and model version.
8. Require a second reviewer for FP, memory-update, branch, and system instructions.

## Common-mode risk

ConcreteOps and symbolic semantics often share dispatch and conceptual implementation. Passing their differential test is not independent proof of ISA correctness. The ledger must distinguish:

- internal consistency;
- independent execution against Dolphin;
- manual/reference review.
