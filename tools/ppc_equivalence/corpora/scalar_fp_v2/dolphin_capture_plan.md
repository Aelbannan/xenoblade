# Scalar FP v2 Dolphin capture plan

Rows still on ``exact_kernel_v2`` that need live Broadway/Dolphin attestation.

## Inventory

- Capture candidates: **0** (NI=1 + non-RNE ``exact_kernel_v2`` rows)
- Broadway ``broadway.jsonl`` NI=1 fixtures: **0**
- Broadway non-RNE fixtures (scalar): **0** (only ``fctiw*`` convert tags)

## Last capture run

- Dolphin-attested (match exact kernel): **11**
- Mismatches / failures: **0**

## Remaining gaps (by theme)

None — all 11 NI/non-RNE ``exact_kernel_v2`` mismatches resolved (2026-07-22).

## Provenance (corpus)

- ``dolphin-capture``: **59**
- ``exact_kernel_v2``: **17**
- ``fixtures-broadway-jsonl``: **88**

## Exact inputs to capture next

Re-run when new ``exact_kernel_v2`` rows are added:

```bash
python3 -m tools.ppc_equivalence.scalar_fp_v2_capture --gen
python3 -m tools.ppc_equivalence.scalar_fp_v2_capture --capture --apply
python3 -m tools.ppc_equivalence.scalar_fp_v2_corpus --check
```

Harness: ``tools/test/compare_behaviour/ppc/scalar_fp_v2_capture.c`` (manifest id ``scalar-fp-v2-capture``, CPUCore=0 interpreter).
