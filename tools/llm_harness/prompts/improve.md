Improve the existing target implementation with one bounded, evidence-driven change. The dossier includes prior harness attempts with mismatch summaries. Use them to avoid repeating losing hypotheses.

Improvement workflow:

1. **Read the mismatch evidence**: Prior attempt evaluation details contain "binary mismatch summary" sections showing instruction-word deltas between retail and candidate. Each delta is a clue:
   - `replace` regions: the candidate produced different instructions than retail. Look at the retail words — are they a different opcode (signed vs unsigned compare?), a different immediate, or a reordered sequence?
   - `insert` regions: the candidate has extra instructions retail doesn't. Usually means you wrote more code than necessary (e.g., an extra temporary, redundant load, unnecessary null check).
   - `delete` regions: retail has instructions the candidate is missing. Usually means you omitted something (a store, a side effect, a flag update).
   - Relocation deltas (retail-only or candidate-only): the candidate references a different or missing symbol. Check whether you used the wrong function, accessed the wrong global, or need an `extern "C"` linkage.
2. **Map deltas to root causes**:
   | Delta pattern | Likely C++ root cause |
   |---|---|
   | `cmpwi` vs `cmplwi` (or vice versa) | signed vs unsigned type mismatch |
   | `lwz` vs `lhz`/`lbz` | wrong load width (int vs short vs char) |
   | `stb` vs `stw` | wrong store width (bool/char vs int) |
   | Same opcodes, different order | expression reordering — break into subexpressions |
   | `bl target_A` vs `bl target_B` | wrong function call target |
   | Extra `mr` before return | redundant temporary — simplify expression |
   | Missing `stb r0, offset(rX)` | missed a boolean store or flag set |
   | Different branch target | wrong if/else structure or loop boundary |
   | `rlwinm` vs `slwi`/`srwi` | mask vs shift — try the other form |
   | Different `addi`/`li` immediate | wrong constant or offset |
   | `lwz`/`stw` pair per field vs single `lwz`/`stw` pair | field-by-field copy vs struct assignment — try `*dst = *src` |
3. **Make one bounded change**: Pick the single most impactful mismatch, fix it, and leave everything else alone. Do not restructure the function or change unrelated code.
4. **Preserve semantics**: The current candidate may already be mostly correct. Do not regress working parts. When in doubt, make the smallest possible edit to the current function.
5. **Consider MWCC knowledge**: If the dossier includes records about this function or similar mismatch patterns, apply the recorded fix if it genuinely fits. The main prompt already includes general MWCC compiler hints; focus on mismatch-specific patterns here.

Return only the complete target function definition.
