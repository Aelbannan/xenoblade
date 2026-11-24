---
name: paseo-subagents
description: >-
  Launch and babysit paseo subagents for this repo. Invoke when delegating
  work to subagents, launching batch matching waves via tools/paseo-harness,
  or interacting with running subagents (status/prompt/permissions/cancel/
  archive).
---

# paseo subagents

Delegate independent work (parallel investigations, batch matching waves) to paseo subagents.
`AGENTS.md` only points here — this skill is the launch/interaction surface.
Upstream reference: the `paseo` skill (`getpaseo/paseo` → `skills/paseo/SKILL.md`) and
https://paseo.sh/docs/orchestration.md.

## Required runtime

- **Provider `pi`**, **model `opencode-go/hy4-flash-free`** (OpenCode Go).
- **Thinking level = lowest that fits**: `medium` routine, `high` multi-file, `xhigh`
  hard/uncertain. Levels: `minimal|low|medium|high|xhigh`; never `off`.
- Pass model/thinking explicitly — other paseo providers/models are not approved.
- **Same workspace, no worktrees** — every subagent must run in *your* workspace: pass your
  `workspaceId` explicitly (from `paseo_get_agent_status` on `$PASEO_AGENT_ID` →
  `snapshot.workspaceId`). Never omit it, and never create a new local/worktree workspace
  (`--isolation worktree`, `paseo_create_workspace`) or switch branches. The `paseo-harness` is the
  only exception: it owns its worktree lifecycle internally — don't create, switch, or clean up
  worktrees yourself.

## Launch

- Ad-hoc: `paseo_create_agent` with `title`, `provider: "pi"`,
  `model: "opencode-go/hy4-flash-free"`, `settings: { thinkingOptionId: "<level>" }`,
  `workspaceId: <your workspace id>` (same workspace — see above), and a **self-contained**
  `initialPrompt` — subagents do not inherit `AGENTS.md`/context, so include: repo path, exact
  target/unit/symbols, acceptance policy (§Never rules, no git, no size hacks, FULL_MATCH), and the
  command that proves done (`hexdiff`). `notifyOnFinish` defaults on.
- Batch matching waves: prefer `tools/paseo-harness/paseo-harness.py run --section <section>`
  (ready-frontier batching, tiered thinking, merge-back). The harness defaults to the approved model;
  see `tools/paseo-harness/README.md`.

## Interact

- **Finish notifications are automatic** — agent-scoped `paseo_create_agent` /
  `paseo_send_agent_prompt` default `notifyOnFinish: true`; the subagent's finish, error, or
  permission request arrives on its own. Leave it on and move on to other work — do **not** poll
  `paseo_list_agents` / `paseo_get_agent_status` just to check a running agent. (Top-level/human
  callers have no parent callback: poll, or `paseo wait <id>` / `paseo attach <id>`.)
- Steer: `paseo_send_agent_prompt`. Unblock: `paseo_respond_to_permission` (see
  `paseo_list_pending_permissions`).
- Stop: `paseo_cancel_agent` (interrupt, session kept) vs `paseo_kill_agent` (terminate).
- Check only when needed: `paseo_get_agent_status` / `paseo_get_agent_activity`; CLI
  `paseo inspect <id> --json`, `paseo logs <id>`. Clean up: `paseo_archive_agent`.
