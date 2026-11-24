# Cheap orchestrator loop (DeepSeek drives, Opus plans and reviews)

One wave = **plan (Opus) → match (hy4 / DeepSeek workers) → batch review (Opus) → land (gate) → checkpoint**.

| role | model | skill |
|---|---|---|
| orchestrator | pi `opencode-go/deepseek-v4.1-flash` | `.agents/skills/xb-orchestrator` |
| planner | claude `claude-opus-5-5`, thinking medium | `.agents/skills/xb-wave-planner` (also `.claude/skills/`) |
| workers | pi hy4-flash-free / deepseek-v4.1-flash | none: the generated pack + prompt from `wave prep` |
| reviewer | claude `claude-opus-5-5`, thinking high | `.agents/skills/xb-wave-reviewer` (also `.claude/skills/`) |

Launch it from a normal terminal (outside any restricted process):

```bash
P=/Applications/Paseo.app/Contents/Resources/bin/paseo
$P run --no-wait --provider pi --model opencode-go/deepseek-v4.1-flash --thinking high \
  --workspace wks_5ac8e57131b986e1 --title "orchestrator W2" \
  "Read .agents/skills/xb-orchestrator/SKILL.md and follow it. Wave: W2. Next checkpoint: 23. If your context is ever compacted, re-read that SKILL.md and /tmp/xb/orch/W2.state before doing anything else."
```

To run a wave without letting the orchestrator commit, add "Skip step 7" to the launch message.

## Why it is split this way

- **Opus makes the two judgement calls.** Wave 1 showed these need judgement: picking targets a cheap
  model can actually finish, and catching compiler steering the land gate cannot see (all 5 wave-1
  holds). Both Opus runs read compact, file-based inputs, so each stays short: 2 runs per wave, plus
  one follow-up message when there are retries.
- **DeepSeek never sees patches.** A cheap model asked to judge faithfulness tends to approve its
  peers' steering.
- **Retries happen once**, and only for blocks that aren't about register allocation. Wave 1 showed
  those are the only retries that pay off.
- **The orchestrator logs each finished step** in `/tmp/xb/orch/<W>.state`. After a compaction it
  re-reads the skill and that file and resumes from the first step not marked done, so it never
  works from a summary of the rules.
