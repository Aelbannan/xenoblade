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
- Python: `.venv/bin/python3` only. Paseo CLI: `P=/Applications/Paseo.app/Contents/Resources/bin/paseo`.
- Workspace: `--workspace wks_5ac8e57131b986e1`. Agent titles must be ≤ 60 chars.
- Worker models (provider `pi`, `--thinking high`):
  - `hy4` = `opencode-go/hy4-flash-free` (default, no rate limit).
  - `ds` = `opencode-go/deepseek-v4.1-flash`. You also run on DeepSeek, and it returns 429 beyond 3
    concurrent, so run **at most 2 ds workers at once**.
- Opus: `--provider claude --model claude-opus-5-5 --mode auto`. **Exactly two Opus runs per
  wave** (plan, review), plus at most one follow-up `send` to the reviewer for retries.
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
- Opus did not write the file it was asked for, or did not end with its OK line.

## 1. Select
```bash
.venv/bin/python3 -m tools.coop.land wave census          # only if the residual census is >1 day old or a checkpoint landed since
.venv/bin/python3 -m tools.coop.land wave select {W} --n 30
```
This writes `/tmp/xb/waves/{W}/targets.tsv` with 30 candidates.

## 2. Plan (Opus, one blocking run)
```bash
$P run --provider claude --model claude-opus-5-5 --thinking medium --mode auto \
  --workspace wks_5ac8e57131b986e1 --title "{W} plan" --wait-timeout 45m \
  "Use the xb-wave-planner skill (.agents/skills/xb-wave-planner/SKILL.md). Wave: {W}." \
  > /tmp/xb/orch/{W}_plan.out 2>&1
```
Checks:
- `tail -3` of the output contains `PLAN-OK`;
- `targets.tsv` has 8 tab-separated columns on every row and 10–24 rows.

## 3. Prep and launch workers
```bash
.venv/bin/python3 -m tools.coop.land wave prep {W}     # table: task, model, model_id, prompt path
```
Launch one worker per row. Start every hy4 row at once. Start ds rows 2 at a time, and start the
next when one of them has written its `.result` file. Record each agent id in the state file.
```bash
$P run --no-wait --provider pi --model <model_id> --thinking high --workspace wks_5ac8e57131b986e1 \
  --title "{W}-NN <symbol, cut to fit 60 chars>" --label wave={W} "$(cat /tmp/xb/waves/{W}/<task>.prompt)"
```

## 4. Wait (and handle permissions)
Repeat until the command reports a result for every task:
```bash
.venv/bin/python3 -m tools.coop.land wave wait {W} --timeout 1200
```
Between calls, run `$P permit ls`:
- **Allow** requests whose paths are all inside /Users/ahmed/projects/xenoblade or /tmp/xb.
- **Deny** everything else, any malformed path, and any request to run git.
- Command: `$P permit allow|deny <agent> <req_id>`.

If a task still has no result after 2 hours, `$P stop` its agent and write
`BLOCKED <task>: timeout` into its `.result` file yourself.

## 5. Collect and batch review (Opus, one blocking run)
```bash
.venv/bin/python3 -m tools.coop.land wave collect {W} > /tmp/xb/orch/{W}_collect.txt
$P run --provider claude --model claude-opus-5-5 --thinking high --mode auto \
  --workspace wks_5ac8e57131b986e1 --title "{W} review" --wait-timeout 60m \
  "Use the xb-wave-reviewer skill (.agents/skills/xb-wave-reviewer/SKILL.md). Wave: {W}." \
  > /tmp/xb/orch/{W}_review.out 2>&1
```
Checks:
- the output ends with `REVIEW-OK`;
- `review.tsv` has one well-formed row per task in `targets.tsv`.

Save the reviewer's agent id (from `$P ls`) in the state file.

## 6. Record, retry once, land
For every review row:
```bash
.venv/bin/python3 -m tools.coop.land wave record {W} <task> <land|hold|drop> "<reason>"
```
Record `retry-ds` rows as `hold`.

**Retries (only if there are `retry-ds` rows):**
1. Copy those rows from `targets.tsv` into `/tmp/xb/waves/{W}r/targets.tsv`, with column 2 set to `ds`.
2. Copy their hint files into `/tmp/xb/waves/{W}r/hints/`.
3. Run steps 3–4 for wave `{W}r`, then `wave collect {W}r`.
4. Send the reviewer this message:
   `$P send <reviewer-id> "Retries for {W}: review wave {W}r the same way. Write /tmp/xb/waves/{W}r/review.tsv (land/hold/drop only). End with REVIEW-OK."`
5. Record those decisions under `{W}r`.

There are no further retries.

**Land:**
```bash
.venv/bin/python3 -m tools.coop.land wave queue {W} <land tasks…> --out /tmp/xb/orch/{W}.multi
# landed {W}r tasks: wave queue {W}r <tasks…> --out /tmp/xb/orch/{W}r.multi; cat it >> /tmp/xb/orch/{W}.multi
XB_LAND_OWNER=1 .venv/bin/python3 -m tools.coop.land apply /tmp/xb/orch/{W}.multi --log /tmp/xb/orch/{W}.log
```
The gate reverts the plans it rejects. That is a normal outcome, not a reason to stop. Note how many
passed and how many were rejected.

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
- any new lines in followups.md.

Then stop. Do not start another wave.
