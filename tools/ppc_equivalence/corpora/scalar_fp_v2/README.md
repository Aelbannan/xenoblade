# Scalar FP v2 validation corpora (Phase 11)

Independent, versioned corpora for the exact scalar-FP v2 promotion path.
Expected rows must **not** be generated from the implementation under test at
promotion time when claiming Dolphin/Broadway provenance; interim
`exact_kernel_v2` rows are clearly marked.

Replay grader: `python -m tools.ppc_equivalence.scalar_fp_v2_corpus --check`

Regenerate from Broadway fixtures + exact-kernel supplements:

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
| Exact kernel supplements | `fp_exact.py`, `fp_exact_convert.py`, … | Gap fill for NI=1 and non-RNE when fixtures lack coverage |

Live Dolphin re-capture for NI=1 / non-RNE rows:

```bash
python3 -m tools.ppc_equivalence.scalar_fp_v2_capture --gen --capture --apply --write-plan
```

See [`dolphin_capture_plan.md`](dolphin_capture_plan.md) for remaining gaps.

## Files

| File | Purpose | Rows (current) |
|---|---|---|
| `scalar_rn.jsonl` | Non-fused scalar result bits (RNE + RTZ/RIP/RIM + NI=1) | 37 |
| `fpscr.jsonl` | FPSCR transition vectors (RNE + non-RNE) | 34 |
| `ni.jsonl` | NI=0 Broadway harvest + NI=1 exact supplements | 33 |
| `compare_convert_control.jsonl` | Compare, convert, NI=1 / non-RNE `frsp` | 20 |
| `traps_fe.jsonl` | FE0/FE1 trap delivery | 20 |
| `fused_residual.jsonl` | Fused midpoint + sticky-residue cases | 20 |

Each file begins with a JSON schema header (`type: scalar_fp_v2_corpus`).

**Total replay rows:** 164 (all pass `--check` as of last harvest).

## Provenance policy

| Value | Meaning |
|---|---|
| `fixtures-broadway-jsonl` | Expected bits/FPSCR/CR from Broadway fixture oracle |
| `exact_kernel_v2` | Expected values from exact-v2 kernel (gap fill or NI=1 / non-RNE) |
| `oracle_rne_interim` | Legacy interim RNE placeholders |
| `dolphin-capture` | Live Dolphin interpreter capture via ``scalar_fp_v2_capture`` harness |

## Coverage (current)

### NI=1 (`exact_kernel_v2` — no live Dolphin NI fixtures)

| Corpus | NI=1 rows | Themes |
|---|---|---|
| `ni.jsonl` | 15 | invalid/NaN, subnormal flush, VE sticky flags |
| `scalar_rn.jsonl` | 4 | subnormal add/mul, signed-zero |
| `compare_convert_control.jsonl` | 5 | `frsp` flush-to-zero, subnormal `fcmpu` |

### Non-RNE rounding (`exact_kernel_v2` — Broadway fixtures are RNE-only)

| Mode | `scalar_rn.jsonl` | `fpscr.jsonl` | `compare_convert_control.jsonl` |
|---|---|---|---|
| RTZ (toward-zero) | 5 | 5 | 2 (`frsp` tie + pi) |
| RIP (toward +∞) | 5 | 5 | 2 |
| RIM (toward −∞) | 5 | 5 | 1 |

Themes include directed overflow (RTZ saturates to max finite vs RNE → ∞),
subnormal underflow (RIP), signed-zero / neg-zero edges, and RN-dependent
`frsp` tie cases (`0x3ff0000000400000`).

## Coverage gaps (honest)

- **Live Dolphin NI=1:** No Broadway fixture sets FPSCR.NI; all NI=1 rows are
  `exact_kernel_v2`. Re-capture via behaviour PPC + `mtfsfi` NI bit still open.
- **Live Dolphin non-RNE:** RTZ/RIP/RIM rows use exact kernel expected bits;
  no independent Dolphin JSONL oracle yet.
- **Fused residual:** `fmadds-single-rounding` and invalid-operand fused cases
  fail exact-v2 replay and are excluded until kernel parity.
- **Trap FE:** Imprecise FE0/FE0+FE1 writeback rows derived via trap planner,
  not live MSR harness capture.
- **Convert:** `fctiw` / `fctiwz` not in compare_convert schema (only `frsp` +
  compares).
- **Record form:** Scalar `fcmp*` record CR1 shadowing not harvested (fixtures
  use BF>0 without record).
- **Ledger dimensions:** `validation_ledger.yaml` `rounding_modes` /
  `subnormals` remain `false` until live Dolphin attestation — corpus replay
  alone does not flip promotion dimensions.

## Mutation killers

See `tools/ppc_equivalence/tests/test_scalar_fp_v2_corpora.py` for required
mutations. Promotion binds `corpus_sha256` (via
`compute_scalar_fp_v2_corpus_sha256`) and `validation_ledger_hash` via
schema-v2 obligations.

## Related

- [`../../SCALAR_FP_V2.md`](../../SCALAR_FP_V2.md) Phase 11–12
- [`../../fp_scalar_obligations_v2.py`](../../fp_scalar_obligations_v2.py)
- [`../../scalar_fp_v2_harvest.py`](../../scalar_fp_v2_harvest.py)
