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

## Required runtime

- **Provider `pi`**, **model `opencode-go/muse-spark-1.3-contributor`** (OpenCode Go).
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
  `model: "opencode-go/muse-spark-1.3-contributor"`, `settings: { thinkingOptionId: "<level>" }`,
  `workspaceId: <your workspace id>` (same workspace — see above), and a **self-contained**
  `initialPrompt` — subagents do not inherit `AGENTS.md`/context, so include: repo path, exact
  target/unit/symbols, acceptance policy (§Never rules, no git, no size hacks, FULL_MATCH), and the
  command that proves done (`hexdiff`). `notifyOnFinish` defaults on.
- Batch matching waves: prefer `tools/paseo-harness/paseo-harness.py run --section <section>`
  (ready-frontier batching, tiered thinking, merge-back). The harness defaults to the approved model;
  see `tools/paseo-harness/README.md`.

## Interact

- Find: `paseo_list_agents`. Poll: `paseo_get_agent_status` / `paseo_get_agent_activity`.
- Steer: `paseo_send_agent_prompt`. Unblock: `paseo_respond_to_permission`.
- Stop: `paseo_cancel_agent` (interrupt, session kept) vs `paseo_kill_agent` (terminate).
- Clean up: `paseo_archive_agent`. CLI equivalents: `paseo inspect <id> --json`, `paseo logs <id>`.
