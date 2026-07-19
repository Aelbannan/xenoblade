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

- **Modules:** `semantics.py` (`SymbolicOps`), `model.py` (machine state /
  `InvalidReason`)
- Z3 bitvector/FP encoding of Broadway user-mode integer and FP ops.
- Definedness via `state.valid` / `InvalidReason`.
- Trust risk: shared conceptual dispatch with ConcreteOps (common-mode error).

## ConcreteOps

- **Modules:** `semantics.py` (`ConcreteOps`), `fixtures/runner.py`
- Host interpreter used for corpus differentials and counterexample replay.
- Passing symbolic↔concrete differential proves internal consistency, **not**
  independent ISA correctness (see ledger `dolphin_interpreter`).

## Memory load/store composition

- **Modules:** `model.py` (byte memory), `memory_profile.py`, stack masking in
  `semantics.py` / `result.MemoryScope`
- Touched-byte extensional comparison; per-implementation private-stack masking
  (`MASKING_SEMANTICS`).
- Profiles: `assumed-ordinary-ram` (default) vs range-constrained profiles
  (`bounded-ordinary-ram`, `stack-and-known-globals`, `hardware-aware`) that
  **require nonempty ranges** and fail closed otherwise. Range constraints
  compose with private-stack masking.
- Trust risk: aliasing / masking bugs hiding public writes.

## Control flow and terminal pairing

- **Modules:** `semantics.py` (`execute_cfg`), `engine.py` (terminal pairing)
- Acyclic CFG with bounded back-edge visits (`max_loop_iterations`); exceeding
  the bound → inconclusive (never truncated). Exit kinds compared pairwise under
  path conditions.
- Trust risk: missed paths, incorrect branch conditions, terminal undercount.

## Contracts

- **Module:** `contract.py`
- Observable sets (`manual`, `ppc-eabi`, `auto`), timeout, resolution metadata.
- Trust risk: under-observation accepting divergent public state.

## Relocations / layout

- **Modules:** `ir.py` (relocation IR), `elf_symbols.py` / `dol_symbols.py`
  (extraction), layout constraints in `engine.py`
- Shared symbolic relocation addresses; linker field-range feasibility check.
- Trust risk: unsound layout allowing impossible but “equivalent” proofs.

## FP tables / helpers

- **Modules:** FP paths in `semantics.py`, `FloatingPointDomain` in `result.py`
- Rounding/NI/domain restrictions; paired-single / fused helpers.
- `FloatingPointDomain` flags either constrain SMT (`exclude_finite_overflow`,
  fused origin, NaN/Inf/subnormal allows, RN/NI) or fail closed when
  unsupported (`traps_enabled`, flag modeling, non-RNE rounding).
- Trust risk: incomplete FPSCR flags; domain exclusions masking real diffs.
- Ledger + promotion: FP proofs are confidence Tier C by policy.

## Stack analysis

- Private-stack enable/disable on escape/call; frame relation
  `symbolic-below-entry-sp`.
- Encoded in `MemoryScope` / `PrivateStackInfo` on certificates.
- Trust risk: asymmetric frames (historical union-mask bug class).

## Callee summaries / certificates

- **Modules:** `callee_inference.py`, `semantics.py` (`CalleeContract`),
  coop certificate chain in `tools/coop/lib/targets.py` /
  `tools/coop/lib/equivalence_check.py`
- Opaque EABI summaries; precise body-inferred / composed matched-callee lemmas;
  transitive certificate checks.
- Trust risk: stale or incomplete callee certificates → false leaf equivalence.

## Query construction

- **Module:** `engine.py` (`_terminal_difference`, `check_with_portfolio`)
- Difference query = layout ∧ memory constraints ∧ ∨(terminal mismatches).
- Trust risk: omitted observable or incorrect `Implies` on path conditions.

## Solver and runtime dependencies

- Z3 portfolio in `engine.py` (`check_with_portfolio`) with shared
  `deadline.Deadline`; Capstone (optional); Python version.
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

## Provenance

- **Modules:** `provenance.py`, certificate fields in
  `tools/coop/lib/equivalence_check.py`
- Engine-tree SHA-256 (`engine_hash`), coop certifier/policy SHA-256
  (`certifier_hash` over `equivalence_check.py`, `equivalence_policy.py`,
  `targets.py`), canonical request `source_hash`, git commit/dirty,
  architecture model, result format, callee contract sources, limits, memory
  profile, and FP domain on durable certificates.
- Trust risk: incomplete request hashing allowing silent reuse across changed
  premises; stale certificates after certifier/policy edits when
  `certifier_hash` is not checked.
