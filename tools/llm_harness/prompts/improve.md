Improve the existing target implementation with one bounded, evidence-driven change.

You are given:
- The exact source of the best prior candidate (the function you are editing).
- Structured binary feedback showing decoded retail vs candidate instruction windows, relocation deltas, and size/frame differences.
- A mismatch fingerprint identifying the failure pattern.

Workflow:
1. **Read the binary feedback**: The `repair_context.binary_feedback` object contains:
   - `first_difference_offset`: byte offset of first mismatch.
   - `instruction_windows`: aligned retail/candidate windows around first 3 differences (4 instructions context each).
   - `differences`: up to 20 instruction diffs with offset, raw words, mnemonics, and likely cause.
   - `relocation_differences`: retail-only and candidate-only relocations.
   - `function_size`: retail vs candidate byte size.
   - `stack_frame`: retail vs candidate stack frame size.
   - `structural_findings`: component-level structural scores.

2. **Map the earliest delta to a root cause**:
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
   | `lwz`/`stw` pair per field vs single pair | field-by-field copy vs struct assignment — try `*dst = *src` |
   | Relocation name mismatch `lbl_eu_*__2cf` vs `lbl_eu_*` | C++ name mangling — add `extern "C"` to the symbol declaration |
   | Relocation name mismatch `@N` vs `lbl_eu_*` | TU-local pool entry — use `extern "C"` data copy instead of PTMF/function-ref expression |

3. **Make one bounded change**: Pick the single most impactful mismatch at the earliest offset, fix it, and leave everything else alone. Do not restructure the function or change unrelated code.

4. **Preserve semantics**: The current candidate may already be mostly correct. Do not regress working parts. When in doubt, make the smallest possible edit to the current function.

Return only the complete target function definition.