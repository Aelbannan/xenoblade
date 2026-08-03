# CriWare matching campaign — orchestration plan

Goal: reach ACCEPTED (FULL_MATCH / EQUIVALENT_MATCH) on all 546 unmatched
CriWare targets (workflow_status != ACCEPTED in `tools/coop/targets.json`,
unit starts with `CriWare`).

## Batch strategy (user-specified)

- **Leaf targets first** (no direct/unresolved/indirect calls) — 68 unmatched
  leaves; they populate the `callees-accepted` frontier bottom-up.
- TUs with **>= 20 unmatched targets** → one batch per TU
  (`sfd_mpv`, 30 targets).
- TUs with **< 20 unmatched targets** → pooled, chunked to ~30 targets per
  batch.
- Result: **17 batches, 546 targets**, ~10 agents concurrent
  (provider `pi`, model `openrouter/deepseek/deepseek-v4-flash-0731`,
  thinking `max`).

## Batch summary

| Batch | Targets | Leaf | Units |
|---|---|---|---|
| cri-01 | 30 | 1 | sfd_mpv |
| cri-02 | 37 | 21 | mpv_umc, sfx_zmv, mwsfdsfx, ahx_sbf |
| cri-03 | 34 | 17 | adx_bsc, adx_sjd, mwsfdply, sfd_buf, mwsfdfrm, ahx_dcd |
| cri-04 | 35 | 11 | adx_dcd, sfd_tim, mwsfdcre, sfd_tst, sfd_aoap, mps_dec |
| cri-05 | 30 | 3 | adx_stmc, mfci, mpv_mc |
| cri-06 | 31 | 7 | adx_baif, adx_amp, adx_bwav, mwsfdsvr, mwsfdsvm, mwsfdsst, mpv_deli |
| cri-07 | 30 | 8 | ahx_mflt_c, mps_lib, mwsfdset, mpv_get, mpv_frm, mwsfdsl, mpv_emp, sfd_con, sfd_pts |
| cri-08 | 31 | 0 | ax_rna, sfd_vom |
| cri-09 | 38 | 0 | adx_fs, mwsfdlib, sfd_see |
| cri-10 | 32 | 0 | adx_tlk, sfd_hds, sfd_trn |
| cri-11 | 38 | 0 | adx_sjd, mpv_vlc, sfd_mps |
| cri-12 | 33 | 0 | adx_tsvr, mpv_lib, sfd_lib, sfd_mpvf |
| cri-13 | 32 | 0 | adx_bwav, mpv_dec, sfd_pl2, sfd_set, ahx_link |
| cri-14 | 31 | 0 | ahx_sjd, sfd_ply, mwsfdsee, sfd_adxt |
| cri-15 | 31 | 0 | adx_xpnd, mpv_slice, sfd_uo, mwsfdsfx, mps_get, sfd_tmr, sfd_mem, mwsfdrna |
| cri-16 | 30 | 0 | adx_fini, mpv_mcy, sfd_seeki, sfd_pl2, sfd_buf, mwsfdrsc, mpv_cdec, sfd_con, mpv_m2v, sfd_see, mwsfdsst, mpvabdec, sfd_tst, mwsfdply |
| cri-17 | 23 | 0 | adx_amp, mwsfdset, sfd_trn, adx_mng, sfd_lib, sfd_mem, sfd_vom, mpv_deli, mps_get, sfd_pts, mpv_umc, mwsfdlib, mwsfdsfx, mpv_frm, sfd_tmr, ahx_cmn, sfd_aoap, mpv_slice, sfd_uo, sfd_hds, mwsfdsl, mpv_emp, mwsfdsst |

(Full per-target detail: `tools/coop/cri_orchestrator/plan.json`.)

## Agent rules (embedded in each batch prompt)

- No pi harness; no subagents; no questions; work until done.
- No `git reset` — other agents work on this branch; commit only your own
  files (units + shared headers + attempts.jsonl).
- `cycle` **without** `--smt` is the acceptance path. SMT probes,
  `run.py diff`, `equivalence *`, and `--contract` variants are OUT-OF-BAND.
- Callee readiness gate first: record blockers, don't waste cycles.
- Witness-blocked-but-clean (hexdiff 0 structural, reg-swap-only) → stall note
  with `next_change: "accept via --smt out-of-band"`, move on.
- Per-unit compiler flags OK (documented fixes only, in the unit's own
  `Object(...)` in configure.py).
- No external source hunting; max 3 grep/find per function; no hand
  disassembly; high-level C/C++ only.

## Babysitter

`tools/coop/cri_orchestrator/monitor.py` keeps ~10 agents running:

- `--running N` (default 10), `--interval 180s`, `--max-idle-min 90`
- Launch: `paseo run --background --provider pi
  --model openrouter/deepseek/deepseek-v4-flash-0731 --thinking max
  --cwd <repo> --title CRI-MATCH-<batch>: <label>`
- On finish → save agent log tail to `reports/<batch>.txt`, launch next batch.
- On hang (> 90 min no activity) or launch error → archive + relaunch (max 3).
- Status: `monitor.py --report`; log: `monitor.log`; state: `state.json`.

## Per-batch report (when a batch finishes)

- Targets accepted (FULL_MATCH / EQUIVALENT_MATCH + %)
- Per-target status / match %
- Blockers left for the out-of-band probe pass (witness-blocked-but-clean,
  callee-blocked, SMT-only-verifiable)
