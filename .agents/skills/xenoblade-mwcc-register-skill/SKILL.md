---
name: xenoblade-mwcc-register-skill
description: Diagnose MetroWerks CodeWarrior Wii/1.1 register allocation for Xenoblade Chronicles (xbret/xenoblade). Use when matching Xenoblade C++ to PowerPC assembly, explaining GPR/FPR/VR register-order differences, live-across-call allocation, spills, save/restore sets, declaration- or expression-order effects, scheduler-driven mismatches, coalescing, or source changes intended to improve an objdiff register mismatch.
---

# Xenoblade MWCC Register Allocation (Wii/1.1)

Scope: game code (`kyoshin` lib) built with Wii/1.1 under `cflags_game`.
Every allocator rule below was established in Wii 1.1 `mwcceppc.exe`
(SHA-256 `14f99570099c6ddee35759d7f9f668ee014c460758f89e0cdf2a51f8f4d8fbe5`)
and carries an evidence tag:

- **confirmed** — proven by Wii 1.1 bytes, strings, or control flow.
- **inferred** — strong conclusion from Wii 1.1 control flow or xrefs.
- **reference** — GC-era hypothesis awaiting Wii validation. Never a fact.

The underlying findings live in the sibling `mwcc-wii-1.1` project
(`docs/WII_1_1_*.md`, `src/backend/*`). Each rule names the Wii address
so it can be re-checked. Do not transfer a rule to another compiler
build without re-validating it there.

Upstream note: `xbret/xenoblade` does not accept LLM-assisted
contributions, and this fork never submits LLM-assisted matching
upstream either. Use this skill as a local diagnostic aid; author any
upstream change yourself.

## 0. Fingerprint the compiler first

```sh
sha256sum <compilers>/Wii/1.1/mwcceppc.exe
```

It must print
`14f99570099c6ddee35759d7f9f668ee014c460758f89e0cdf2a51f8f4d8fbe5`.
If it differs, treat every `inferred` rule below as a hypothesis and
re-validate before using it. Then confirm the TU's profile in
`references/xenoblade-profile.md` — game objects, SDK objects, and
several libraries use different flags, and `-O4,s` objects exist inside
`kyoshin` itself.

## 1. Diagnose a mismatch

1. Confirm compiler hash (§0) and the TU's flag set
   (`references/xenoblade-profile.md`).
2. Locate the first meaningful divergence. Separate pure
   physical-register renaming from instruction selection, scheduling,
   loads/stores, stack layout, and symbol differences.
3. Identify the values live at every mismatching instruction. Record
   calls, ABI inputs/results, volatile or address-taken objects, inline
   asm, and CR/CTR/LR/XER constraints.
4. Reason about post-optimization virtual values. Per-class numbering
   starts at the class bound (32 for VR/FPR/GPR) in creation order
   (**inferred**); source declaration order is only indirect evidence.
5. Account for copy coalescing and interference. The parent map at
   `0x00739A20` is a union-find; an alias follows its root's color
   (**inferred**). Non-overlapping values may reuse a color. A value
   crossing a call conflicts with the volatile colors for that value.
6. Apply the driver, simplify, spill-ranking, and coloring rules from
   `references/allocator-rules.md`. Use `scripts/regalloc_explain.py`
   for pools, preference orders, and spill-score arithmetic.
7. If the graph rules do not explain the output, investigate upstream
   before tuning declarations: the 61-pass IRO pipeline, the default
   width-2 scheduler model, silent peephole rewrites, and load/store
   form selection all run before or after coloring.
8. Make the smallest semantic-preserving source change and verify with
   the real compiler and objdiff. Prefer real project types and
   mergeable C++; never introduce raw-offset casts, fake volatility, or
   dead operations merely to steer registers.

## 2. Choose source-level rewrites

- Reorder independent expressions or declarations only when it plausibly
  changes post-optimization creation order or overlap.
- Shorten or extend a live range with an equivalent expression shape.
- Preserve or eliminate a copy to test coalescing.
- Reshape a branch or expression tree when scheduling or destructive
  target choice differs.
- Check signedness, width, volatile qualification, and address-taking
  when instruction selection or stack homes differ.
- Treat calls and raw inline-asm registers as fixed conflicts, not as
  global bans on volatile registers.
- Match `-O4,p` vs `-O4,s` per object before tuning allocation: size
  mode changes codegen upstream of the allocator.

## 3. Wii traps (do not re-learn these)

- Frame slots are NOT at object `+0x2A` — refuted; the slot dword is at
  `+0x48`. Never cite `+0x2A`.
- `0x00772280` is the **volatile** pool, `0x00772528` the **claim
  (saved)** pool. Earlier notes had them flipped; three independent
  facts pin the current assignment.
- There is no Broadway machine model. Everything schedules on the
  default width-2 model — a changed instruction order is usually
  scheduling, not coloring.
- Spill-select (`0x00593130`) is a single-`ret` stub. Spill choice lives
  in simplify's cost/degree ranking, not in a select-time heuristic.
- The `stmw`/`lmw` save threshold is unproven for Wii 1.1. Do not cite
  the GC-era "five contiguous saves" rule here.

## 4. Map

- `references/xenoblade-profile.md` — compiler identity and exact flags.
- `references/allocator-rules.md` — the Wii 1.1 allocator, rule by rule.
- `references/mismatch-workflow.md` — triage checklist and evidence
  boundaries.
- `scripts/regalloc_explain.py` — pools, orders, spill arithmetic.

## 5. Fork integration (this checkout)

- Workflow, acceptance policy, and command routing live in the main
  skill at `.agents/skills/xenoblade-decomp/SKILL.md` — this register
  skill covers allocator diagnosis only. Load both on matching tasks.
- Run scripts with `.venv/bin/python3`, never the system `python3`
  (system is 3.9 and fails on project syntax).
- Verify builds with `python3 tools/coop/hexdiff.py <unit> --symbol
  <sym>` where possible; it runs the build under the repo lock.
  Function identity/state comes from `tools/coop/targets.json`.
- Never run `git` here — leave all version control to the human.
- Treat the repo root as sacred: keep all skill files under
  `.agents/skills/`.
