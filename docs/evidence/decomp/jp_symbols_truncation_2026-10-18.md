# INCIDENT: `config/jp/symbols.txt` truncated (2026-10-18)

## Status

- `config/us/symbols.txt` — **repaired** (50181 lines; restored from a complete
  pre-repro backup; `configure.py` parses, units build, registry valid).
- `config/eu/symbols.txt` — intact (50176 lines).
- `config/jp/symbols.txt` — **STILL TRUNCATED** (31706 lines; file ends mid-line
  at `GetWarpModeT`). No complete copy exists under `/tmp`; **restore from VCS.**
  A copy of the truncated file is at `/tmp/jp_symbols_truncated_keep.txt`.

The US build pipeline is unaffected: nothing in `tools/coop/` or `configure.py`
reads `config/jp/symbols.txt` for a US build (`grep` confirms), and there is no
`build/jp/`.

## How it happened (my fault, not another orchestration)

1. A bulk member-ization unit (`CMiniMap`) failed to verify. To find the real
   error I reproduced the rename **by hand** with
   `run.py symbols rename-fn-all MiniMapFillMarkerEntry 'CMiniMapGimmickView::FillMarkerEntry' --demote-import-band`.
   That tool rewrites `symbols.txt` for **all three regions** (us, eu, jp).
2. I reverted by copying only `config/us/symbols.txt` back from my backup —
   **EU and JP were left renamed** (caught later; EU/JP names were fixed).
3. During a second reproduction I snapshotted `config/us/symbols.txt` while it was
   already truncated (31706 lines) and later **restored that truncated snapshot
   over US**, propagating the damage. (US and JP then had identical md5.)

## Root cause of the truncation itself

Not fully determined. The truncated files end mid-line at the same symbol
(`GetWarpModeT`), and `config/us` and `config/jp` ended up byte-identical, which
implies a partial write of `symbols.txt` (all regions) during a tool/agent
operation, plus my snapshot-and-restore of that partial state. The member-ization
tool writes `symbols.txt` per region; a partial/interrupted write is the most
likely mechanism. **Worth a tool hardening:** write `symbols.txt` atomically
(temp file + rename) so an interrupted write can never truncate it.

## Recovery

- US: restored from `/tmp/split_repro/symbols.txt` (50181 lines) — verified it
  contains all wave-1 and wave-2 landed member-ization names and the original
  `MiniMapFillMarkerEntry`, so it is consistent with the source tree.
- EU: intact; the one renamed entry (`FillMarkerEntry__19CMiniMapGimmickViewFv`)
  was reverted to `MiniMapFillMarkerEntry` so all regions agree.
- JP: **needs the human** — `git checkout -- config/jp/symbols.txt` (agents are
  forbidden from running git in this repo).

## Lessons

- When a rename tool touches N regions, a revert must restore **all N**, or the
  tree ends up region-skewed.
- Never snapshot a file and restore it later without verifying it is complete
  (check for a trailing partial line / expected line count).
- A hand-repro of a failure mutates the real tree; prefer a scratch copy, and if
  not possible, back up and restore *everything* the operation touches.
