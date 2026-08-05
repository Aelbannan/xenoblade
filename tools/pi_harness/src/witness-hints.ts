// ---------------------------------------------------------------------------
// Witness rejection-gate hints (LOW-3). The register-renaming witness reports
// a rejection gate name (`witness-gate: <gate> | <reason>`) — these map each
// gate to actionable model advice. Gates come from
// tools/coop/lib/renaming_witness.py WitnessFailure literals:
//   reloc, rho, execute, structural, size, reject-list, mnemonic,
//   abi-boundary, fields, loop, deadline.
// Shared by the `witness` session tool (session-tools.ts) and the near-match
// singleton diagnosis block (orchestrator.ts). Pure — no I/O.
// ---------------------------------------------------------------------------

/** Extract the bare gate name from a gate string. The witness emits plain
 *  names ("reloc") but the per-process map may hold "reloc — slot 12: …"
 *  combined strings; hints key off the leading gate name. */
export function witnessGateName(gate: string): string {
  const m = gate.match(/^([a-z][a-z-]*)/);
  return m ? m[1] : gate;
}

/** Actionable hint text for a witness rejection gate. Returns the generic
 *  hint when the gate is unknown (never undefined — callers always get text).
 */
export function witnessHintForGate(gate: string, reason?: string): string {
  switch (witnessGateName(gate)) {
    case "reloc":
      return (
        "Reloc name drift: decomp emits a different reloc symbol than retail. " +
        "Run `hexdiff <unit> <symbol>` and look at the Reloc-drift suggestions — " +
        "approved fixes are usually `extern \"C\" <TYPE> <retail-symbol>;` in the " +
        "declaring header/.cpp (PLAN.md §17.6). If a conflicting non-extern-C " +
        "declaration already exists, REMOVE/CONVERT it — two declarations with " +
        "different linkage cause an illegal-overload build error."
      );
    case "rho":
      return (
        "Register-bijection conflict (local temp re-allocation). If the only diffs " +
        "are commutative operand swaps (`add rA,rB,rC` vs `add rA,rC,rB`) those are " +
        "handled; otherwise try reordering statements so MWCC allocates registers " +
        "in retail's order."
      );
    case "execute":
      return (
        "A callee lacks a lemma — the function calls an uncertified target. " +
        "Certify the callee first (its target must reach FULL_MATCH/EQUIVALENT_MATCH) " +
        "or check whether the call is to a helper that needs an opaque-EABI contract."
      );
    case "structural":
      return (
        "Terminal state diverges — a real semantic difference remains at a return path. " +
        "Compare the hexdiff mismatched-instruction list: the first-divergence label " +
        "(e.g. `gpr r20`, `memory`) says which component. If it is `memory` and hexdiff " +
        "shows only commutative/reg-swap diffs, this may be a known witness path-" +
        "condition over-rejection — re-check the hexdiff diff itself before editing."
      );
    case "size":
      return (
        "The decompiled body is a different size than retail (often a stub or an " +
        "incomplete function). Write the real body — a `void f(){}` stub never certifies."
      );
    case "reject-list":
      return (
        "The pair uses an opcode the witness rejects (the reject list: e.g. `mffs`, " +
        "non-byte-identical PSQ D-forms, or an SPR outside XER/LR/CTR). The witness " +
        "will never certify this instruction mix — restructure the source so MWCC " +
        "emits different code (expression/loop shape), or chase FULL_MATCH (byte " +
        "identity) instead."
      );
    case "mnemonic":
      return (
        "Different mnemonics at the same slot — a real instruction-selection " +
        "difference, not a register rename. Check hexdiff's mismatched-instruction " +
        "list and rewrite the source (expression order, comparison shape, struct " +
        "access) so MWCC emits retail's opcode."
      );
    case "abi-boundary":
      return (
        "The register bijection would move an ABI register (argument/return or a " +
        "fixed/nonvolatile register), which the witness forbids. The function's " +
        "argument/return registers or nonvolatile saves are allocated differently — " +
        "reorder locals/statements so MWCC keeps ABI registers fixed, or the declared " +
        "signature itself may differ from retail."
      );
    case "fields":
      return (
        "Non-register bits differ at the same mnemonic (immediates, LK/AA, BO/BI, " +
        "opcode/XO/Rc, SPR indices). This is immediate/encoding selection, not " +
        "register allocation — look at the hexdiff mismatch list and fix the " +
        "immediate/encoding in source."
      );
    case "loop":
      return (
        "The witness cannot model this loop-shaped code (unbounded iteration, an " +
        "indirect branch `bcctr`/`blrl`, or an unmodeled absolute tail branch). " +
        "Pursue FULL_MATCH (byte identity) or restructure the control flow into a " +
        "form the witness can bound."
      );
    case "deadline":
      return (
        "The witness hit its execution deadline (too many paths/iterations to " +
        "explore). The function is too complex for the witness — pursue FULL_MATCH " +
        "(byte identity) or simplify the control flow."
      );
    default:
      return (
        "The witness rejected the target at this gate. Inspect the gate reason " +
        "above, compare the hexdiff mismatched-instruction list, and fix the " +
        "divergence in source (or chase FULL_MATCH if the gate is a witness-model " +
        "limitation)."
      );
  }
}
