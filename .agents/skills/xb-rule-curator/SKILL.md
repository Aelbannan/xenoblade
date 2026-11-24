---
name: xb-rule-curator
description: >-
  Turn a recurring rejected shape (reviewer holds tagged [shape:<slug>]) into an enforced land-gate
  ban counter (Opus role). Invoke when told "use the xb-rule-curator skill. Slugs: …".
---

# Rule curator

The wave reviewer has held patches showing the same compiler-steering shape at least 3 times across
at least 2 waves. Workers already see it as advice in their packs. Your job is to make the land gate
enforce it, without blocking legitimate code. Repo: /Users/ahmed/projects/xenoblade. The slugs are
in the launch message.

Read first:
- `docs/land_gate.md` (the ban counter table and the "no rise" ratchet)
- `tools/coop/land/bans.py`
- `tools/coop/land/shapes.py`

## For each slug
1. **Look at the evidence.** Run `.venv/bin/python3 -m tools.coop.land wave shapes --show <slug>`.
   It prints the registry entry and the archived examples: `held/<W>/<T>/{orig,final}` (rejected)
   and every `landed/…` example (accepted, your false-positive corpus).
2. **Decide whether a text counter can express the shape.**
   - Counters run on file text with comments and strings masked (`bans.mask`). Each counter is a
     regex in `BANS`, or a small function like `_single_use_static_inline`.
   - If the shape is semantic, i.e. it depends on what a pointer aliases or why code was reordered,
     do not force a regex. Set `"enforceable": false` and `"why": "…"` for the slug in
     `tools/coop/land/shapes.json`, so it is never proposed again. Report
     `NOT-ENFORCEABLE <slug>: <why>` and stop for that slug; it stays as pack advice.
3. **Write the counter.**
   - Add it to `BANS` in `tools/coop/land/bans.py`; it counts automatically. Use the slug with `_`
     for `-` as the key.
   - Make it as narrow as possible. It must rise on every held example, and on no landed example.
   - Pre-existing hits elsewhere in the tree are acceptable, because the gate only rejects a rise.
     Report how many there are: `grep` is fine, or count with `bans.count` over `src/` and `libs/`.
4. **Register it.** Set `"counter": "<key>"` for the slug in `tools/coop/land/shapes.json`.
5. **Test it.** Add a test class to `tools/coop/tests/test_curated_bans.py` (create the file if
   needed). Include at least one positive case taken from a held example, and at least one negative
   case of the nearest legitimate spelling, ideally taken from a landed example or an SDK file.
6. **Document it.** Add one row to the ban counter table in `docs/land_gate.md`.

## Validate (all must pass)
```bash
.venv/bin/python3 -m tools.coop.land wave check-counter <each ENFORCED slug>
.venv/bin/python3 -m unittest discover -s tools/coop/tests -t . -q
```

## Never
- Never touch src/, include/, libs/, config/ or configure.py.
- Never run git, or `tools.coop.land check/apply`.
- Never remove or loosen an existing counter.
- Never edit `tools/coop/land/compare.py`, `runner.py` or `snapshot.py`. Counters are data in
  `bans.py`; the gate machinery stays as it is.

## Reply
For each slug, one line:
- `ENFORCED <slug> -> <key> (<pre-existing hits> existing hits)`, or
- `NOT-ENFORCEABLE <slug>: <why>`.

Then a final line: `CURATE-OK`.
