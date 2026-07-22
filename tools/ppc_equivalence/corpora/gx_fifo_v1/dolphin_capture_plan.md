# GX FIFO v1 Dolphin capture plan

This corpus's `write_trace.jsonl` / `read_policy.jsonl` fixtures are
currently **architectural-contract fixtures** (`oracle: contract-v2`): they
encode what the reviewed Tier-A model (`GX_FIFO_TIER_A.md`,
`device_model.GxFifoStreamDevice`) claims, grounded in the in-tree SDK
`WGPIPE` header evidence. They are **not yet** cross-checked against a live
Dolphin-interpreter capture. This document is the plan for doing that
independently of the Python device model, mirroring
`tools/ppc_equivalence/scalar_fp_v2_capture.py` /
`corpora/scalar_fp_v2/dolphin_capture_plan.md`.

## Status

- Live Dolphin capture runs: **0**
- DOL harness source: `tools/test/compare_behaviour/ppc/gx_fifo_v1_capture.c`
  + `gx_fifo_v1_capture_cases.inc` (exists; **not yet run under Dolphin in
  this environment** — Dolphin must be launched outside the restricted
  process per `AGENTS.md`).
- Manifest wiring: **not yet added** to
  `tools/test/compare_behaviour/manifest.json` (deliberately deferred until
  a capture run has actually validated the harness end-to-end; see
  "Next steps").

## What independent evidence is (and is not) obtainable here

The GX write-gather pipe (`WGPIPE`, `0xCC008000`) is documented in-tree only
as a **write** target (`GXHardware.h`). Dolphin's `MMIO` implementation
routes `WGPIPE` stores into its own GPU/FIFO command-processor emulation —
it does not expose a byte-addressable readback of "the last N FIFO write
events" the way this corpus's `write_events` trace does. That means a live
Dolphin capture can attest to:

1. **No CPU exception/hang** when performing an aligned width-{1,2,4}
   store to `WGPIPE` at each declared offset (this *is* obtainable and is
   what the harness below exercises).
2. Indirectly, via Dolphin's own FIFO consumer (`OpcodeDecoder` /
   `CommandProcessor`), that the store bytes were actually enqueued — but
   extracting that signal deterministically through the GDB stub (the same
   mechanism `scalar_fp_v2_capture.py` uses for FPR/FPSCR) is materially
   harder than reading a fixed-size struct at a known symbol, and is **not**
   implemented yet.

It can **never** attest to a read value from `WGPIPE` (see
`GX_FIFO_TIER_A.md` outcome 3) — `read_policy.jsonl`'s fail-closed
unsupported-read claims are a *policy* stance, not something a hardware
capture could contradict or confirm with a "correct" value.

## Harness

`tools/test/compare_behaviour/ppc/gx_fifo_v1_capture.c` (guest symbol
`behaviour_main`) + `gx_fifo_v1_capture_cases.inc` (table-driven cases:
`id`, byte `offset` from `WGPIPE` base, `width`, `value`). For each case it
performs exactly one aligned store of the declared width to
`0xCC008000 + offset` and records a `{magic, completed}` observation at a
known symbol (`g_gx_fifo_v1_capture_results`), mirroring
`scalar_fp_v2_capture.c`'s `g_scalar_fp_v2_capture_results` pattern. Store
only legal, generated fixtures — never retail binaries or disc assets (see
`AGENTS.md` "Do not").

Current cases mirror the `write_trace.jsonl` width/alignment/repeat themes
(6 cases: width-1/2/4 singles, an offset+4 width-4 write, and two repeated
width-1 writes at the same offset).

## Steps to run a live capture (future work)

1. Add a `gx-fifo-v1-capture` entry to
   `tools/test/compare_behaviour/manifest.json` (mirror the
   `scalar-fp-v2-capture` entry: `ppc_source`, `ppc_cpu_core: 0`, a stub
   `unit`/`symbol` anchor since this harness needs no real game unit).
2. Build + run under headless Dolphin (`CPUCore=0` interpreter,
   **outside** the restricted process — see `AGENTS.md`):

   ```bash
   python3 -m tools.test.compare_behaviour.ppc_runner --test gx-fifo-v1-capture
   ```

3. Read `g_gx_fifo_v1_capture_results` via the GDB stub at exit (see
   `scalar_fp_v2_capture.run_dolphin_capture` for the pattern:
   `lookup_symbol_address` + `gdb_read_memory_at_exit`). Confirm every case
   completed (`completed == 1`, correct `magic`) with no CPU exception.
4. If Dolphin exposes a FIFO-consumer-side signal for the enqueued bytes in
   the future, extend the harness to capture and compare it against
   `write_trace.jsonl`'s declared `value`/`width`/`address` per event, and
   upgrade those rows' `oracle` field (new value, e.g.
   `dolphin-capture`) — never overwrite `contract-v2` fixtures silently;
   add a provenance field the way `scalar_fp_v2_capture.py` does
   (`apply_capture_to_corpora`).
5. Re-run `python3 -m tools.ppc_equivalence.gx_fifo_corpus --check` and
   update `manifest.json`'s `corpus_sha256` plus this plan's "Status"
   section.

## Remaining gaps (honest)

- No live Dolphin evidence yet for any row in this corpus.
- No FIFO-consumer-side readback mechanism identified for verifying enqueued
  byte content (only "did not fault" is currently obtainable).
- `read_policy.jsonl` cannot be upgraded past `contract-v2` by capture in
  principle — it is a policy corpus, not a value corpus (see "What
  independent evidence is (and is not) obtainable here" above).
- `loop_emission.jsonl` is graded against the in-repo recognizer/discharge
  implementation directly (see `README.md`); it has no separate Dolphin
  capture path — a loop's summarized effect on `WGPIPE` reduces to the same
  per-iteration write already covered by `write_trace.jsonl`.
