Improve the best prior candidate with one bounded, evidence-driven change.

Evidence: editable candidate source, `repair_context.binary_feedback` (first difference, instruction windows, relocation deltas, size/frame), and mismatch fingerprint.

Map the earliest delta, then change only that cause:

| Delta pattern | Likely C++ root cause |
|---|---|
| `cmpwi` vs `cmplwi` | signed vs unsigned |
| `lwz` vs `lhz`/`lbz` or `stb` vs `stw` | wrong load/store width |
| Same opcodes, different order | expression reordering — split temporaries |
| `bl` target differs | wrong call |
| Extra `mr` before return | redundant temporary |
| Missing `stb r0, off(rX)` | missed bool/flag store |
| Different branch target | wrong if/else or loop shape |
| `rlwinm` vs `slwi`/`srwi` | mask vs shift form |
| Wrong `addi`/`li` immediate | wrong constant or offset |
| Field-wise `lwz`/`stw` vs one pair | try `*dst = *src` |
| `lbl_eu_*__2cf` vs `lbl_eu_*` | add `extern "C"` |
| `@N` vs `lbl_eu_*` | TU-local pool — copy `extern "C"` data, not PTMF/ref expression |

If the candidate still uses a stub signature that contradicts `retail_asm` (void/no-params vs return/`this`/args in the listing), fix that ABI first before chasing later instruction deltas.

Do not restructure unrelated code. Preserve already-correct semantics.

Return only the complete target function definition.
