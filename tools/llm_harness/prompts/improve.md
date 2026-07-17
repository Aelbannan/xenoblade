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
3. **Make one bounded change**: Pick the single most impactful mismatch, fix it, and leave everything else alone. Do not restructure the function or change unrelated code.
4. **Preserve semantics**: The current candidate may already be mostly correct. Do not regress working parts. When in doubt, make the smallest possible edit to the current function.
5. **Consider MWCC knowledge**: If the dossier includes records about this function or similar mismatch patterns, apply the recorded fix if it genuinely fits.

Common MWCC pitfalls to check:

- **Signed/unsigned mismatch**: MWCC `lbz`/`lhz`/`lwz` are unsigned loads. If retail uses `cmpwi` (signed compare) after `lwz`, the source type is `int`, not `u32`. If retail uses `cmplwi` (unsigned), the source is `u32`.
- **Type width**: `stb` stores a byte. The source should be `u8`, `s8`, `char`, or `bool`. `sth` is 16-bit, `stw` is 32-bit.
- **Expression ordering**: If the candidate logic is correct but bytes differ, break `a + b * c + d` into `t = b * c; result = a + t + d` and try different groupings.
- **Load/store patterns**: MWCC may use `lwz r0, off(rX)` / `stw r0, off(rY)` pairs for struct-to-struct copy instead of field-by-field. Writing a simple assignment `*dst = *src` may match better than individual field copies.

Return only the complete target function definition.
