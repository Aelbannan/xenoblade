# GX FIFO v1 validation corpora

Independent, versioned corpora for the three `GX_FIFO_TIER_A.md` Tier-A
capabilities:

| File | Capability | Model | Rows |
|---|---|---|---|
| `write_trace.jsonl` | `gx-fifo-write-trace` | `gx-fifo-trace-v2` | 12 |
| `read_policy.jsonl` | `gx-fifo-read` | `gx-fifo-read-v1` (outcome 3) | 12 |
| `loop_emission.jsonl` | `mmio-loop-emission` | `mmio-loop-emission-v1` | 14 |

Replay grader: `python -m tools.ppc_equivalence.gx_fifo_corpus --check`

## Independence rule (read this before editing any row)

Every data row's `expected` block is a **hand-authored architectural-contract
claim** — labeled `oracle: contract-v2` — describing what the reviewed
Tier-A model (`GX_FIFO_TIER_A.md`, `device_model.GxFifoStreamDevice`,
`gx_fifo_loop.py`, `gx_fifo_loop_discharge.py`) *claims* its behavior is.
Rows are **never** produced by first calling `GxFifoStreamDevice` /
`recognize_gx_fifo_loops` / `discharge_gx_fifo_loop_plan` and saving
whatever value came out — that would make the corpus circular (the model
grading itself). The grader (`tools/ppc_equivalence/gx_fifo_corpus.py`)
*does* execute those implementations to compare their actual behavior
against each row's declared expectation, exactly like
`scalar_fp_v2_corpus.py` replays `exact_scalar_arith` against
`expected_bits` — that is what makes `--check` meaningful: a forged or
mutated `expected` value must make replay fail (see
`tests/test_gx_fifo_corpus.py::GraderRejectsForgedExpectationsTests`).

`write_trace.jsonl` and `read_policy.jsonl` claims are grounded in:

- The in-tree SDK header evidence for `WGPIPE` (`0xCC008000`, write-gather
  pipe; see `GX_FIFO_TIER_A.md` "Shared foundations").
- `device_model.GxFifoStreamDevice`'s reviewed, hash-bound implementation
  (`hardware_profile wii-broadway-xenoblade-us-v2`), which is itself the
  in-repo encoding of that SDK evidence, not an independently-run oracle.
- For genuinely independent (Dolphin-interpreter) attestation of the
  write-gather side effects, see [`dolphin_capture_plan.md`](dolphin_capture_plan.md)
  and `tools/test/compare_behaviour/ppc/gx_fifo_v1_capture.c`. As of this
  corpus version, live Dolphin capture has **not** been run (see the plan's
  "Status" section); the write/read/loop fixtures here are the reviewed
  architectural contract, not yet cross-checked against a live capture.

`loop_emission.jsonl` claims are grounded in the narrow, deliberately-narrow
`mmio-loop-emission-v1` domain frozen in `GX_FIFO_TIER_A.md` ("Initial
domain" table): natural single-header CTR loop, one FIFO write per
iteration, concrete/proven device+address, width in `{1,2,4}`, positive
concrete trip count or explicit zero-skip, `N * writes_per_iteration <=
max_fifo_events`, invariant/simple-affine values only, and a fixed set of
prohibited shapes (calls, indirect branches, early exits, FIFO reads, other
MMIO, interrupts, DMA). Each row's `program` block encodes the *shape*
parameters (count, store width, base/source registers, an optional
`shape` mutation selector); `gx_fifo_corpus.build_loop_program` — written
independently of `gx_fifo_loop.py`/its tests — turns that shape into a
concrete PPC instruction list, which the grader then feeds to the real
recognizer/discharge implementation and compares against the row's
declared `expected` block.

## Files

### `write_trace.jsonl`

Header: `{"type": "gx_fifo_v1_write_trace_corpus", "schema_version": 1}`.

Each row runs a short sequence of `GxFifoStreamDevice.write(...)` calls
(optionally with `device` overrides, e.g. a tiny `max_fifo_events` for the
bound-edge fixture) and asserts the declared per-op outcome plus the
resulting `event_cursor` / `write_events` trace. Covers:

- Widths 1/2/4 (`write-width1-single`, `write-width2-single`, `write-width4-single`)
- Alignment negatives (`write-misaligned-width2`, `write-misaligned-width4`)
- Unsupported width negative (`write-unsupported-width8`)
- Out-of-range negative (`write-out-of-range-at-span`)
- Repeated writes (`write-repeated-three`)
- Bound edge with a small `max_fifo_events` override (`write-bound-edge-max-fifo-events`)
- Failure does not consume an event index (`write-mixed-success-fail-success`)
- Region boundary writes (`write-last-valid-address-width1`, `write-last-valid-address-width4`)

### `read_policy.jsonl`

Header: `{"type": "gx_fifo_v1_read_policy_corpus", "schema_version": 1}`.

Proves the outcome-3 unsupported-read policy fails closed for every width
and every address, including neighbors just outside the reviewed region,
and that repeated reads never advance `event_cursor` or record a
`read_events` entry. Documents (via row `note` fields, not numeric
assertions — outcome 3 defines no read value) that RAW/WAR ordering
relative to prior writes and reordered-read sharing are **not applicable**:
there is no read value to order or share.

### `loop_emission.jsonl`

Header: `{"type": "gx_fifo_v1_loop_emission_corpus", "schema_version": 1}`,
`fixture_metadata.max_events: 8` (deliberately small so bound-edge /
bound+1 fixtures stay tiny; production `max_fifo_events` default is 256).

Covers positive recognition+discharge (counts 1/2/3/5/7/8, widths 1/2/4,
invariant and affine values) and negative/rejection shapes: zero trip count
(CTR-zero-wrap), bound+1, unresolved trip count, an extra hidden write, an
early-exit-style inserted compare, a store address outside the GX region,
swapped instruction order (affine `addi` before the store), and an affine
`addi` targeting the wrong register.

## `manifest.json`

`corpus_sha256` is computed over the SHA-256 of each of the three JSONL
files' raw text, canonicalized (`tools.ppc_equivalence.provenance.canonical_json_sha256`)
— see `gx_fifo_corpus.compute_gx_fifo_v1_corpus_sha256`. Recompute after any
row edit:

```bash
python3 -m tools.ppc_equivalence.gx_fifo_corpus --check
```

The printed `corpus_sha256=...` line must match `manifest.json`'s
`corpus_sha256` field; keep them in sync by hand (this corpus intentionally
has no auto-write helper, unlike `scalar_fp_v2_harvest.py` — every row here
is hand-authored, not harvested).

## Mutation killers

See `tools/ppc_equivalence/tests/test_gx_fifo_corpus.py` and
`tools/ppc_equivalence/tests/test_gx_fifo_read.py` for the required
mutation-soundness tests (schema violations, forged expectations, a device
that advances its cursor on read, etc.), and
`tools/ppc_equivalence/tests/test_gx_fifo_mutation_soundness.py` for the
loop-recognition/discharge/obligation-level mutation tests (dropped/
duplicated/reordered events, off-by-one trip counts, omitted bound/
zero-wrap proofs, UNSAT→SAT/unknown solver results, forged digests, a
disagreeing caller-supplied `status`, and ad-hoc bus maps).

## Related

- [`../../GX_FIFO_TIER_A.md`](../../GX_FIFO_TIER_A.md)
- [`../../device_model.py`](../../device_model.py) — `GxFifoStreamDevice`
- [`../../gx_fifo_loop.py`](../../gx_fifo_loop.py) — recognition + closed-form summary
- [`../../gx_fifo_loop_discharge.py`](../../gx_fifo_loop_discharge.py) — per-side refinement discharge
- [`../../mmio_capability_obligations.py`](../../mmio_capability_obligations.py) — promotion-grade obligation validators
- [`dolphin_capture_plan.md`](dolphin_capture_plan.md)
