---
name: xb-orchestrator
description: >-
  Drive one Xenoblade matching wave end to end as the cheap orchestrator (DeepSeek): select,
  Opus plan, hy4/DeepSeek workers, Opus batch review, land gate, checkpoint. Invoke when told
  "orchestrate wave W…". Plumbing only — never judges source shape.
---

# Wave orchestrator

You are the ORCHESTRATOR of a Wii decompilation matching pipeline. Repo:
/Users/ahmed/projects/xenoblade. Your launch message gives the **wave name `W`** (e.g. W2) and the
**next checkpoint number `CP`**. Wherever these steps say `{W}` or `{CP}`, use those values.

Your job is plumbing: run commands, launch agents, move files, and check that each step's output
exists and looks well formed. You do NOT read C/C++ source, packs or patches, and you do NOT decide
whether a patch is faithful. Opus makes those decisions (skills `xb-wave-planner` and
`xb-wave-reviewer`), and the land gate checks them mechanically. Keep your context small: never
print whole files. Use `wc -l`, `head -5` and `cut`.

## Resuming and compaction (read this first, every time)

You keep a progress file, `/tmp/xb/orch/{W}.state`. After finishing each numbered step, append one
line to it:

    <step> done <ISO time> <short facts: agent ids, counts>

**If your context was compacted or summarised, or you are unsure where you are:**
1. Re-read this whole SKILL.md.
2. Read `/tmp/xb/orch/{W}.state`.
3. Continue from the first step that is not marked done.

Never redo a done step. Never re-launch workers whose `.result` files exist. Do not trust a summary
of these rules; the SKILL.md is the source of truth.

## Fixed facts
- Python: `.venv/bin/python3` only. Paseo CLI: always call it by its **full literal path**
  `/Applications/Paseo.app/Contents/Resources/bin/paseo`. Never put it in a variable or alias (`$P run …`): the permission
  classifier only allows the literal path, and a variable form will be denied.
- Workspace: `--workspace wks_5ac8e57131b986e1`. Agent titles must be ≤ 60 chars.
- Worker models (provider `pi`, `--thinking high`):
  - `hy4` = `opencode-go/hy4-flash-free` (default, no rate limit).
  - `ds` = `opencode-go/deepseek-v4.1-flash`. You also run on DeepSeek, and it returns 429 beyond 3
    concurrent, so run **at most 2 ds workers at once**.
- Opus: `--provider claude --model claude-opus-5-5 --mode auto`. **Exactly two Opus runs per
  wave** (plan, review). The only extra Opus messages allowed: one correction `send` each when
  check-plan / check-review fails, and one retry-review `send`.
- Wave files: `/tmp/xb/waves/{W}/`. Patches: `/tmp/xb/plans/`. Logs and state: `/tmp/xb/orch/`.
  `mkdir -p` them first.

## Never
- Never run git, except in step 7 exactly as written.
- Never edit any file under src/, include/, libs/, config/, configure.py or tools/. The single
  exception is `docs/orchestrator_followups.md`, which only Opus writes.
- Never edit a `.ctx.c` file. Never run `faithful --init`.
- Never `rm -rf`. Never touch build/us/.hexdiff.lock. Never put files in the repo root.
- Never push anywhere except `origin`. Never force-push.
- Never start a second wave, and never change these rules. If a step does not work as written, stop
  and report.

## Stop and report
Write the reason to `/tmp/xb/orch/{W}.STOP`, reply with it, and end, if:
- any command exits non-zero when this skill does not say how to handle it;
- `wave collect` prints a `STRAY` row;
- the gate's `apply` reports an abort, or "tree not quiet";
- the checkpoint prints `CHECKPOINT FAILED`;
- `check-plan` / `check-review` still fails after one correction round.

## 1. Select
```bash
.venv/bin/python3 -m tools.coop.land wave census          # only if the residual census is >1 day old or a checkpoint landed since
.venv/bin/python3 -m tools.coop.land wave select {W} --n 30
```
This writes `/tmp/xb/waves/{W}/targets.tsv` with 30 candidates.

## 2. Plan (Opus, one blocking run)
```bash
/Applications/Paseo.app/Contents/Resources/bin/paseo run --provider claude --model claude-opus-5-5 --thinking medium --mode auto \
  --workspace wks_5ac8e57131b986e1 --title "{W} plan" --wait-timeout 45m \
  "Use the xb-wave-planner skill (.agents/skills/xb-wave-planner/SKILL.md). Wave: {W}." \
  > /tmp/xb/orch/{W}_plan.out 2>&1
.venv/bin/python3 -m tools.coop.land wave check-plan {W}
```
Save the planner's agent id (from `/Applications/Paseo.app/Contents/Resources/bin/paseo ls`) in the state file. `check-plan` must print `OK`; it is the only gate (the `paseo run` output file shows a status table,
not Opus's reply, so do not look for `PLAN-OK` there). If check-plan fails, send
its ERROR lines once to the planner (`/Applications/Paseo.app/Contents/Resources/bin/paseo send <planner-id> "check-plan failed: … Fix the files and
end with PLAN-OK."`), then re-run check-plan. If it fails a second time, stop and report.

## 3. Prep and launch workers
```bash
.venv/bin/python3 -m tools.coop.land wave prep {W}
.venv/bin/python3 -m tools.coop.land wave launch {W}
```
`launch` is idempotent. It starts every hy4 task and up to 2 ds tasks, and records agent ids in
`/tmp/xb/waves/{W}/agents.json`. Never launch workers by hand.

## 4. Manage until done
```bash
.venv/bin/python3 -m tools.coop.land wave manage {W} --timeout 1200
```
`manage` starts the next ds task as slots free up and answers our workers' permission requests: it
allows requests inside the repo or /tmp/xb, denies git, the land gate, `rm -rf`, `.ctx.c` and
anything outside, and logs each answer to `permissions.log`. It also stops any task that has run
for more than 2 hours and marks it `BLOCKED timeout`. Exit codes:
- `0` (`ALL DONE`): go to step 5;
- `3` (`PENDING …`): run it again;
- anything else: stop and report.

## 5. Collect and batch review (Opus, one blocking run)
```bash
.venv/bin/python3 -m tools.coop.land wave collect {W} > /tmp/xb/orch/{W}_collect.txt
/Applications/Paseo.app/Contents/Resources/bin/paseo run --provider claude --model claude-opus-5-5 --thinking high --mode auto \
  --workspace wks_5ac8e57131b986e1 --title "{W} review" --wait-timeout 60m \
  "Use the xb-wave-reviewer skill (.agents/skills/xb-wave-reviewer/SKILL.md). Wave: {W}." \
  > /tmp/xb/orch/{W}_review.out 2>&1
.venv/bin/python3 -m tools.coop.land wave check-review {W}
```
Save the reviewer's agent id (from `/Applications/Paseo.app/Contents/Resources/bin/paseo ls`) in the state file. If check-review fails, send the
ERROR lines to the reviewer once, as in step 2; stop if it fails again.

## 6. Decide, retry once, land
```bash
.venv/bin/python3 -m tools.coop.land wave decide {W}
```
`decide` records every decision, moves non-landing patches to `plans/held/`, writes
`/tmp/xb/orch/{W}.multi`, and, if there were `retry-ds` rows, prepares the retry wave `{W}r`.

**If it printed `retry wave: {W}r`:**
1. Run `wave prep {W}r`, `wave launch {W}r` and `wave manage {W}r` (as in steps 3–4), then
   `wave collect {W}r > /tmp/xb/orch/{W}r_collect.txt`.
2. Send the reviewer this message:
   `/Applications/Paseo.app/Contents/Resources/bin/paseo send <reviewer-id> "Retries for {W}: review wave {W}r the same way (land/hold/drop only). End with REVIEW-OK."`
3. Run `wave check-review {W}r`, then `wave decide {W}r`.
4. Run `cat /tmp/xb/orch/{W}r.multi >> /tmp/xb/orch/{W}.multi`.

There are no further retries.

**Land** (skip this if `{W}.multi` is empty):
```bash
XB_LAND_OWNER=1 .venv/bin/python3 -m tools.coop.land apply /tmp/xb/orch/{W}.multi --log /tmp/xb/orch/{W}.log
```
The gate reverts the plans it rejects. That is a normal outcome, not a reason to stop. Note how many
passed and how many were rejected.
```bash
.venv/bin/python3 -m tools.coop.land wave gate-alarm {W} --log /tmp/xb/orch/{W}.log
```
Each `ALARM` line means one ban counter rejected more than 2 reviewer-approved plans: the counter is
too broad, or the review was too lax. The command records it in `docs/orchestrator_followups.md`.
Do not stop, and do not touch the counter; just repeat each ALARM line in the step 7 commit message
and the step 8 report, for the human.

## 6b. Promote recurring shapes (only when there are candidates)
```bash
.venv/bin/python3 -m tools.coop.land wave shapes --promote
```
If it prints no `PROMOTE` lines, skip this step. Otherwise, make at most one curator run per wave:
```bash
/Applications/Paseo.app/Contents/Resources/bin/paseo run --provider claude --model claude-opus-5-5 --thinking high --mode auto \
  --workspace wks_5ac8e57131b986e1 --title "{W} rule curator" --wait-timeout 45m \
  "Use the xb-rule-curator skill (.agents/skills/xb-rule-curator/SKILL.md). Slugs: <the PROMOTE slugs>." \
  > /tmp/xb/orch/{W}_curator.out 2>&1
.venv/bin/python3 -m tools.coop.land wave check-counter <each slug the curator reported ENFORCED>
.venv/bin/python3 -m unittest discover -s tools/coop/tests -t . -q
```
Both commands must pass (they are the gate; read the curator's ENFORCED / NOT-ENFORCEABLE lines with
`/Applications/Paseo.app/Contents/Resources/bin/paseo logs <curator-id> | tail -20`). The curator may also report a
slug as `NOT-ENFORCEABLE`; that is fine. If a check fails, send the ERROR lines to the curator once;
if it fails again, stop and report. Put every promoted counter in the step 7 commit message and the
step 8 report, so the human can veto it.

## 7. Checkpoint
```bash
.venv/bin/python3 -m tools.coop.land.checkpoint > /tmp/xb/orch/cp{CP}.txt 2>&1; tail -12 /tmp/xb/orch/cp{CP}.txt
```
If it prints `CHECKPOINT OK`:
```bash
git add -A src libs include config docs tools configure.py
git commit -q -m "Checkpoint {CP}: wave {W} (<N> plans landed, <M> rejected by gate) — <matched_functions line>"
git push -q origin HEAD
git fetch -q . HEAD:main && git push -q origin main
.venv/bin/python3 -m tools.coop.land.checkpoint --accept
```
If the commit hook refuses a file, stop and report. Never unstage files to get past it.

## 8. Report and end
Reply in at most 8 lines:
- the checkpoint line;
- landed / held / dropped / rejected counts;
- the commit hash;
- the path of plan.md;
- any new lines in followups.md;
- any counters promoted in step 6b;
- any gate-alarm ALARM lines.

Then stop. Do not start another wave.
