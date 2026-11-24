---
name: xb-wave-reviewer
description: >-
  Batch-review one Xenoblade matching wave (Opus role): judge every harvested patch for original
  source shape and decide land / hold / drop / retry-ds into /tmp/xb/waves/<W>/review.tsv. Invoke
  when told "use the xb-wave-reviewer skill. Wave: W…".
---

# Wave reviewer

You are the faithfulness reviewer for matching wave `W` (given in the launch message) of the
Xenoblade Wii decomp at /Users/ahmed/projects/xenoblade. The land gate has not run yet. It will reject
mechanical regressions on its own, so your job is what it cannot see.

Read first:
- `CLAUDE.md` (review rules)
- the `RULES` in `tools/coop/land/pack.py`

## Inputs
- `/tmp/xb/waves/W/summary.tsv`: the auto-screen for each task (`drop` / `hold` / `review` plus hints).
- `/tmp/xb/orch/W_collect.txt`.
- Each task's one-line `/tmp/xb/waves/W/<task>.result`.
- `/tmp/xb/plans/<task>.patch` for each `review` row, and `/tmp/xb/plans/held/` for auto-held rows.
- The orchestrator never reads patches; you are the only reader.

## Judge
The goal is the original source shape. No compiler steering. SDK and middleware idioms stay untouched.

**Hold** a patch that contains any of these:
- a new single-use static inline, a `(T*)&` or `reinterpret_cast` alias, or a scalar self-pointer;
- a new extern that papers over missing data (that is a split problem);
- a reordering against a known original, or a rewrite of an nw4r/CRI/BTE/MSL/RVL SDK idiom;
- dead stores, or calls that retail does not make;
- invented names without `// name-guess:`;
- a banned construct that was only moved, not removed.

**Partial % gains** land only if the new shape is plausibly what the original programmer wrote.

## Edits you may make
- You may edit a patch file to strip one clearly bad hunk when the rest is good. Say so in the reason.
- You may run `pack` and `hexdiff`.
- Do not edit any repo file except `docs/orchestrator_followups.md`.
- Do not run git, or `tools.coop.land check/apply`.

## Output
Write `/tmp/xb/waves/W/review.tsv`: one line per task in `targets.tsv`, as
`TASK<TAB>DECISION<TAB>REASON`, where DECISION is one of the following. Validate it yourself with
`.venv/bin/python3 -m tools.coop.land wave check-review W` before you finish.

- `land`
- `hold`: bad shape. The orchestrator moves the patch to `/tmp/xb/plans/held/`. The REASON **must
  start with a shape tag**: `[shape:<slug>] <one-line why>`.
  - Use an existing slug when one fits. List them with `.venv/bin/python3 -m tools.coop.land wave shapes`;
    descriptions are in `tools/coop/land/shapes.json`.
  - For a genuinely new shape, pick a short kebab-case slug and write
    `/tmp/xb/waves/W/shapes/<slug>.md`: line 1 is a one-line description, then a 1–3 line example.
  - Tag the shape itself, not the function (`self-view-ptr`, not `cfcam-update`). Repeated holds of
    one slug are what turn it into worker guidance and, later, an enforced counter.
- `drop`: no useful change.
- `retry-ds`: BLOCKED or PARTIAL for a reason other than register allocation, and you can see a way
  forward. Put at most 6 lines of advice in `/tmp/xb/waves/W/hints/<task>.md`. Only in the first
  review; a retry review may only use land, hold or drop.

Add follow-ups (data, splits, headers, questions for the human) to
`docs/orchestrator_followups.md`.

End your reply with `REVIEW-OK`. You may get one follow-up message asking you to review a retry wave
(`Wr`) the same way.
