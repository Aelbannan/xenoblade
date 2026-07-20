# Scalar FP v2 validation corpora (Phase 11)

Independent, versioned corpora for the exact scalar-FP v2 promotion path.
Expected rows must **not** be generated from the implementation under test at
promotion time when claiming Dolphin/Broadway provenance; interim
`exact_kernel_v2` rows are clearly marked.

Replay grader: `python -m tools.ppc_equivalence.scalar_fp_v2_corpus --check`

Regenerate from Broadway fixtures:

```bash
python3 -m tools.ppc_equivalence.scalar_fp_v2_harvest --inventory
python3 -m tools.ppc_equivalence.scalar_fp_v2_harvest --write
```

## Fixture sources (in-repo)

| Source | Path | Role |
|---|---|---|
| Broadway corpus (Python) | `tools/ppc_equivalence/fixtures/corpus.py` | Dolphin-validated expected FPR/FPSCR/CR |
| Broadway JSONL export | `tools/ppc_equivalence/fixtures/broadway.jsonl` | Same cases; used by `gen_fixture_blob` / behaviour PPC |
| Differential fixtures | `tools/ppc_equivalence/fixtures/broadway.jsonl` + `validation_ledger.yaml` | Per-opcode `dolphin_interpreter: true` rows |
| Harvest tool | `tools/ppc_equivalence/scalar_fp_v2_harvest.py` | Maps fixtures → corpus rows; filters replay-passing |

Live Dolphin re-capture is **not** required for this pass — expected bits are
copied from the existing Broadway fixture oracle. Rows that disagree with the
exact-v2 replay kernel are dropped; gaps are filled with honestly marked
`exact_kernel_v2` supplements.

## Files

| File | Purpose |
|---|---|
| `scalar_rn.jsonl` | Non-fused scalar result bits (RN nearest-even) |
| `fpscr.jsonl` | FPSCR transition vectors |
| `ni.jsonl` | NI=0/1 operand/result flush |
| `compare_convert_control.jsonl` | Compare, convert, FPSCR control ops |
| `traps_fe.jsonl` | FE0/FE1 trap delivery |
| `fused_residual.jsonl` | Fused midpoint + sticky-residue cases |

Each file begins with a JSON schema header (`type: scalar_fp_v2_corpus`).

## Provenance policy

| Value | Meaning |
|---|---|
| `fixtures-broadway-jsonl` | Expected bits/FPSCR/CR from Broadway fixture oracle |
| `exact_kernel_v2` | Expected values from exact-v2 kernel (gap fill or NI=1) |
| `oracle_rne_interim` | Legacy interim RNE placeholders |
| `dolphin-capture` | Reserved for future live Dolphin JSONL capture rows |

## Coverage gaps (honest)

- **RN modes:** Broadway fixtures use default RN only; no RTP/RTN/RTZ corpus rows yet.
- **NI=1:** No Broadway fixtures set FPSCR.NI; NI=1 rows use `exact_kernel_v2`.
- **Fused residual:** `fmadds-single-rounding` and invalid-operand fused cases fail exact-v2 replay and are excluded until kernel parity.
- **Trap FE:** Imprecise FE0/FE0+FE1 writeback rows derived via trap planner, not live MSR harness capture.
- **Convert:** `fctiw` / `fctiwz` not in compare_convert schema (only `frsp` + compares).
- **Record form:** Scalar `fcmp*` record CR1 shadowing not harvested (fixtures use BF>0 without record).

## Mutation killers

See `tools/ppc_equivalence/tests/test_scalar_fp_v2_corpora.py` for required
mutations. Promotion binds `corpus_sha256` (via
`compute_scalar_fp_v2_corpus_sha256`) and `validation_ledger_hash` via
schema-v2 obligations.

## Related

- [`../../SCALAR_FP_V2.md`](../../SCALAR_FP_V2.md) Phase 11–12
- [`../../fp_scalar_obligations_v2.py`](../../fp_scalar_obligations_v2.py)
- [`../../scalar_fp_v2_harvest.py`](../../scalar_fp_v2_harvest.py)
