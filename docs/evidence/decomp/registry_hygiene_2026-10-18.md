# Registry hygiene sweep — 2026-10-18

## Summary

- **Brief premise stale:** `UIWin_FlagBufCanShow` (`us-8013fdf4`, `kyoshin/CUIWindowManager`) is
  already `FULL_MATCH` / `ACCEPTED` / `instruction_match` 100.0 in `tools/coop/targets.json`
  (not `COMPILES`). Live hexdiff confirms `mismatch_count` 0 (byte-100%). No action needed.
- **Promotion direction:** 1645 `COMPILES` rows, **0** with `instruction_match` 100.0 — no
  promotion candidates exist in the registry. No tool offers hexdiff-based
  `COMPILES`→`FULL_MATCH` promotion anyway (promotion path is `cycle`, out of scope).
- **Demotion direction:** `sweep_stale_full_match.py --dry-run` flags 26 `FULL_MATCH` rows as
  non-byte-identical. **Live hexdiff contradicts the sweep on 3/3 sampled candidates**
  (all `mismatch_count` 0 after rebuild). The sweep reads prebuilt objects without rebuilding,
  so its verdicts are false positives against stale objects. **Apply mode was NOT run;
  zero registry writes performed** (`targets.json` untouched, so no backup was required).
- **Other checks:** `registry_check.py` passed; `targets validate` 19300 valid;
  `audit-promotion --dry-run` 0 affected; `migrate-report` 0 affected
  (7 `FULL_MATCH` rows noted with stale certs, informational only).

## Recommendation for human review

Run a full fresh build first, then re-run the sweep; only demote rows that are still
flagged after the rebuild. The 26 ids below are the re-sweep list.

## Commands used

```
.venv/bin/python3 tools/coop/sweep_stale_full_match.py --help
.venv/bin/python3 tools/coop/registry_check.py --help
.venv/bin/python3 tools/coop/sweep_stale_full_match.py --dry-run
.venv/bin/python3 tools/coop/hexdiff.py kyoshin/CUIWindowManager --symbol UIWin_FlagBufCanShow --json
.venv/bin/python3 tools/coop/hexdiff.py kyoshin/CGame --symbol wkStandbyLogin__5CGameFv --json
.venv/bin/python3 tools/coop/hexdiff.py kyoshin/CScrollBar --symbol CScrollBar_StepScrollIn --json
.venv/bin/python3 tools/coop/hexdiff.py monolib/src/work/CWorkRoot --symbol isRunning__11CWorkThreadCFv --json
.venv/bin/python3 tools/coop/run.py targets audit-promotion --dry-run
.venv/bin/python3 tools/coop/run.py targets migrate-report --json
.venv/bin/python3 tools/coop/run.py targets validate
```

## Candidate table

| target_id | symbol | unit | current status | hexdiff evidence | action taken / proposed | tool |
|---|---|---|---|---|---|---|
| us-8013fdf4 | UIWin_FlagBufCanShow | kyoshin/CUIWindowManager | FULL_MATCH | mismatch_count 0 (byte-100%, rebuilt) | NONE NEEDED — already FULL_MATCH; brief premise stale | hexdiff.py |
| us-80039a64 | wkStandbyLogin__5CGameFv | kyoshin/CGame | FULL_MATCH | mismatch_count 0 / 118 (byte-100%, rebuilt) — sweep false positive | NO CHANGE — human review: rebuild fully, then re-sweep | sweep_stale_full_match.py (dry-run only) |
| us-80046f80 | setFlag | kyoshin/plugin/pluginEve | FULL_MATCH | not individually re-verified (same sweep batch; sweep reads prebuilt objects) | NO CHANGE — human review: rebuild fully, then re-sweep | sweep_stale_full_match.py (dry-run only) |
| us-800474f8 | fadeIn | kyoshin/plugin/pluginEve | FULL_MATCH | not individually re-verified (same sweep batch) | NO CHANGE — human review: rebuild fully, then re-sweep | sweep_stale_full_match.py (dry-run only) |
| us-800475fc | fadeOut | kyoshin/plugin/pluginEve | FULL_MATCH | not individually re-verified (same sweep batch) | NO CHANGE — human review: rebuild fully, then re-sweep | sweep_stale_full_match.py (dry-run only) |
| us-8004aa44 | setFade | kyoshin/plugin/pluginCfs | FULL_MATCH | not individually re-verified (same sweep batch) | NO CHANGE — human review: rebuild fully, then re-sweep | sweep_stale_full_match.py (dry-run only) |
| us-8004ac34 | setDispOffArea | kyoshin/plugin/pluginCfs | FULL_MATCH | not individually re-verified (same sweep batch) | NO CHANGE — human review: rebuild fully, then re-sweep | sweep_stale_full_match.py (dry-run only) |
| us-8006b450 | CfCamEvt_RefreshCamVectors | kyoshin/cf/CfCamEvent | FULL_MATCH | not individually re-verified (same sweep batch) | NO CHANGE — human review: rebuild fully, then re-sweep | sweep_stale_full_match.py (dry-run only) |
| us-80095e28 | CtrlNpcUpdateMovement | kyoshin/cf/CtrlNpc | FULL_MATCH | not individually re-verified (same sweep batch) | NO CHANGE — human review: rebuild fully, then re-sweep | sweep_stale_full_match.py (dry-run only) |
| us-800c2a00 | Walker_SyncBattleFrame | kyoshin/cfsys/CfObjectImplWalker | FULL_MATCH | not individually re-verified (same sweep batch) | NO CHANGE — human review: rebuild fully, then re-sweep | sweep_stale_full_match.py (dry-run only) |
| us-8017cdb0 | CActorParam_UnkVirtualFunc175__Q22cf11CActorParamFv | kyoshin/cf/object/CActorParam | FULL_MATCH | not individually re-verified (same sweep batch) | NO CHANGE — human review: rebuild fully, then re-sweep | sweep_stale_full_match.py (dry-run only) |
| us-8017d83c | CActorParam_applyStatusHeal__Q22cf11CActorParamFv | kyoshin/cf/object/CActorParam | FULL_MATCH | not individually re-verified (same sweep batch) | NO CHANGE — human review: rebuild fully, then re-sweep | sweep_stale_full_match.py (dry-run only) |
| us-8017e644 | CActorParam_UnkVirtualFunc6__Q22cf11CActorParamFv | kyoshin/cf/object/CActorParam | FULL_MATCH | not individually re-verified (same sweep batch) | NO CHANGE — human review: rebuild fully, then re-sweep | sweep_stale_full_match.py (dry-run only) |
| us-8017ed30 | CActorParam_addHate__Q22cf11CActorParamFv | kyoshin/cf/object/CActorParam | FULL_MATCH | not individually re-verified (same sweep batch) | NO CHANGE — human review: rebuild fully, then re-sweep | sweep_stale_full_match.py (dry-run only) |
| us-801876a8 | test | kyoshin/plugin/pluginBtl | FULL_MATCH | not individually re-verified (same sweep batch) | NO CHANGE — human review: rebuild fully, then re-sweep | sweep_stale_full_match.py (dry-run only) |
| us-801c5968 | CTitleAHelp_load | kyoshin/CTitleAHelp | FULL_MATCH | not individually re-verified (same sweep batch) | NO CHANGE — human review: rebuild fully, then re-sweep | sweep_stale_full_match.py (dry-run only) |
| us-801f55b8 | CScrollBar_StepScrollIn | kyoshin/CScrollBar | FULL_MATCH | mismatch_count 0 / 25 (byte-100%, rebuilt) — sweep false positive | NO CHANGE — human review: rebuild fully, then re-sweep | sweep_stale_full_match.py (dry-run only) + hexdiff.py |
| us-801f561c | CScrollBar_StepScrollOut | kyoshin/CScrollBar | FULL_MATCH | not individually re-verified (same sweep batch) | NO CHANGE — human review: rebuild fully, then re-sweep | sweep_stale_full_match.py (dry-run only) |
| us-801faa18 | __dt__Q22cf12CfObjectTboxFv | kyoshin/cf/object/CfObjectTbox | FULL_MATCH | not individually re-verified (same sweep batch) | NO CHANGE — human review: rebuild fully, then re-sweep | sweep_stale_full_match.py (dry-run only) |
| us-8022616c | __ct__CMCEffUpPrm | kyoshin/makecrystal/CMCEffStart | FULL_MATCH | not individually re-verified (same sweep batch) | NO CHANGE — human review: rebuild fully, then re-sweep | sweep_stale_full_match.py (dry-run only) |
| us-8022652c | __ct__CMCEffSuccess | kyoshin/makecrystal/CMCEffStart | FULL_MATCH | not individually re-verified (same sweep batch) | NO CHANGE — human review: rebuild fully, then re-sweep | sweep_stale_full_match.py (dry-run only) |
| us-80226a8c | __ct__CMCEffCrystal | kyoshin/makecrystal/CMCEffStart | FULL_MATCH | not individually re-verified (same sweep batch) | NO CHANGE — human review: rebuild fully, then re-sweep | sweep_stale_full_match.py (dry-run only) |
| us-80227b40 | __ct__CMCEffCylinder | kyoshin/makecrystal/CMCEffStart | FULL_MATCH | not individually re-verified (same sweep batch) | NO CHANGE — human review: rebuild fully, then re-sweep | sweep_stale_full_match.py (dry-run only) |
| us-802a1100 | __dt__14CMenuSkipTimerFv | kyoshin/menu/CMenuSkipTimer | FULL_MATCH | not individually re-verified (same sweep batch) | NO CHANGE — human review: rebuild fully, then re-sweep | sweep_stale_full_match.py (dry-run only) |
| us-804185a8 | SetParam__Q34nw4r3snd11FxDelayDpl2FRCQ44nw4r3snd6detail12FxDelayParam | nw4r/src/snd/snd_FxDelayDpl2 | FULL_MATCH | not individually re-verified (same sweep batch) | NO CHANGE — human review: rebuild fully, then re-sweep | sweep_stale_full_match.py (dry-run only) |
| us-80446718 | isRunning__11CWorkThreadCFv | monolib/src/work/CWorkRoot | FULL_MATCH | mismatch_count 0 / 33 (byte-100%, rebuilt) — sweep false positive | NO CHANGE — human review: rebuild fully, then re-sweep | sweep_stale_full_match.py (dry-run only) + hexdiff.py |
| us-804eb0b0 | __ct__CMdlAnmUV | monolib/src/scn/CMdlAnmUV | FULL_MATCH | not individually re-verified (same sweep batch) | NO CHANGE — human review: rebuild fully, then re-sweep | sweep_stale_full_match.py (dry-run only) |

## Human-review list

All 26 sweep-flagged ids above (no demotion applied). Plus informational: `migrate-report`
notes 7 `FULL_MATCH` rows with stale certificate versions (affected_count 0 — no action).
