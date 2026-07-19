# Trusted computing base — PPC equivalence

Inventory of components whose bugs can silently produce false `EQUIVALENT`
results. Validation evidence lives in `validation_ledger.yaml`; promotion
requires every opcode used by a proof to carry Dolphin interpreter evidence
(or demotes confidence / blocks Gate 2/3 auto-promotion).

Architecture model: see `result.ARCHITECTURE_MODEL`.

## Decoder

- **Module:** `decoder.py`
- Field extraction, reserved-bit rejection, Capstone optional cross-check.
- Invalid encodings raise `UnsupportedInstruction` / `DecodeError` rather than
  inventing semantics.
- Trust risk: mis-decoded fields → wrong IR opcode/operands.

## IR construction

- **Module:** `ir.py`
- `Opcode`, `Instruction`, `SUPPORTED_OPCODES` / `SUPPORTED_FP_OPCODES`.
- Decoder output is the only path into symbolic and concrete execution.
- Trust risk: opcode missing from `SUPPORTED_*` but still executed, or wrong
  operand packing.

## Symbolic operations

- **Modules:** `ops.py`, `semantics.py`
- Z3 bitvector/FP encoding of Broadway user-mode integer and FP ops.
- Definedness via `state.valid` / `InvalidReason`.
- Trust risk: shared conceptual dispatch with ConcreteOps (common-mode error).

## ConcreteOps

- **Modules:** `concrete_ops.py`, `fixtures/runner.py`
- Host interpreter used for corpus differentials and counterexample replay.
- Passing symbolic↔concrete differential proves internal consistency, **not**
  independent ISA correctness (see ledger `dolphin_interpreter`).

## Memory load/store composition

- **Modules:** `memory.py`, `memory_profile.py`, stack masking in `semantics` /
  `result.MemoryScope`
- Touched-byte extensional comparison; per-implementation private-stack masking
  (`MASKING_SEMANTICS`).
- Profiles: bounded ordinary RAM vs assumed ordinary RAM (tier-affecting).
- Trust risk: aliasing / masking bugs hiding public writes.

## Control flow and terminal pairing

- **Modules:** `cfg.py`, `engine.py` (`execute_cfg`, terminal pairing)
- Acyclic CFG only; loops → inconclusive. Exit kinds compared pairwise under
  path conditions.
- Trust risk: missed paths, incorrect branch conditions, terminal undercount.

## Contracts

- **Module:** `contracts.py`
- Observable sets (`manual`, `ppc-eabi`, `auto`), timeout, resolution metadata.
- Trust risk: under-observation accepting divergent public state.

## Relocations / layout

- **Modules:** `relocations.py`, layout constraints in `ops` / `engine`
- Shared symbolic relocation addresses; linker field-range feasibility check.
- Trust risk: unsound layout allowing impossible but “equivalent” proofs.

## FP tables / helpers

- **Modules:** FP paths in `semantics.py` / `concrete_ops.py`,
  `FloatingPointDomain` in `result.py`
- Rounding/NI/domain restrictions; paired-single / fused helpers.
- Trust risk: incomplete FPSCR flags; domain exclusions masking real diffs.
- Ledger + promotion: FP proofs are confidence Tier C by policy.

## Stack analysis

- Private-stack enable/disable on escape/call; frame relation
  `symbolic-below-entry-sp`.
- Encoded in `MemoryScope` / `PrivateStackInfo` on certificates.
- Trust risk: asymmetric frames (historical union-mask bug class).

## Callee summaries / certificates

- **Modules:** `callee.py`, coop certificate chain in `tools/coop/lib/targets.py`
- Opaque EABI summaries; matched-callee lemmas; transitive certificate checks.
- Trust risk: stale or incomplete callee certificates → false leaf equivalence.

## Query construction

- **Module:** `engine.py` (`pair_differences`, portfolio solver setup)
- Difference query = layout ∧ memory constraints ∧ ∨(terminal mismatches).
- Trust risk: omitted observable or incorrect `Implies` on path conditions.

## Solver and runtime dependencies

- Z3 portfolio (`solver_portfolio.py`), Capstone (optional), Python version.
- Pinned in `requirements.lock`; versions recorded on `ProofResult`.
- Trust risk: solver bugs, nondeterminism, unpinned deps changing semantics.

## Promotion policy

- **Module:** `tools/coop/lib/equivalence_policy.py`
- `PromotionDecision` from status + architecture + provenance + confidence tier
  + **validation ledger** (`opcodes_used` ⊆ dolphin-validated set).
- Ledger file: `validation_ledger.yaml` (hashed into `engine_hash` via
  `provenance.hash_engine_tree`).
- Common-mode note: ConcreteOps + symbolic agreement alone never upgrades an
  opcode to `dolphin_interpreter: true`.
