# Scalar FP v2 Dolphin capture plan

Rows still on ``exact_kernel_v2`` that need live Broadway/Dolphin attestation.

## Inventory

- Capture candidates: **55** (NI=1 + non-RNE ``exact_kernel_v2`` rows)
- Broadway ``broadway.jsonl`` NI=1 fixtures: **0**
- Broadway non-RNE fixtures (scalar): **0** (only ``fctiw*`` convert tags)

## Last capture run

- Dolphin-attested (match exact kernel): **44**
- Mismatches / failures: **11**

## Remaining gaps (by theme)

### NI=1 (3)

- `exact-ni1-fcmpu-subnorm-vs-zero`
- `exact-ni1-fadds-snan-ve`
- `exact-ni1-fmuls-force25-subnormal`

### non-RNE FPSCR transitions (4)

- `exact-fpscr-rip-subnormal-underflow`
- `exact-fpscr-rtz-add-truncate`
- `exact-fpscr-rtz-overflow-max-finite`
- `exact-fpscr-rtz-sub-truncate`

### non-RNE scalar result (4)

- `exact-rim-sub`
- `exact-rip-adds`
- `exact-rip-tiny-add`
- `exact-rtz-sub-truncate`

## Exact inputs to capture next

Re-run:

```bash
python3 -m tools.ppc_equivalence.scalar_fp_v2_capture --gen
python3 -m tools.ppc_equivalence.scalar_fp_v2_capture --capture --apply
python3 -m tools.ppc_equivalence.scalar_fp_v2_corpus --check
```

Harness: ``tools/test/compare_behaviour/ppc/scalar_fp_v2_capture.c`` (manifest id ``scalar-fp-v2-capture``, CPUCore=0 interpreter).
