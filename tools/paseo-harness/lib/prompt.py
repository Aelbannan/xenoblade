"""Agent prompt builder.

Deliberately compact: agents are told to read big docs LAZILY (only when a
mismatch is actually observed) instead of pre-loading AGENTS.md + PLAN.md +
MWCC_REFERENCE into context where they are re-billed every turn.
"""
from .common import ROOT


def build_prompt(batch, *, section, worktree_path=None, branch=None,
                 launched_at=None, relaunch=0, seed_text=None, allow_smt=False):
    rows = []
    for td in batch["target_details"]:
        leaf = "leaf" if td.get("leaf") else "    "
        rows.append(
            f"  {td['id']}  {leaf}  {td['function']}  [{td['unit'].split('/')[-1]}]  "
            f"cur={td.get('status')} {td.get('match_percent') or '-'}% size={td.get('size')}"
        )
    table = "\n".join(rows)
    units = ", ".join(u.split("/")[-1] for u in batch["units"])
    bid = batch["batch_id"]

    env = []
    if worktree_path:
        env.append(
            f"- Working directory: {worktree_path}  (branch {branch}, created from main "
            f"at {launched_at or 'wave start'}). Only you work in this worktree — commit "
            f"freely (git add <your files> && git commit). Never touch other branches or worktrees."
        )
        env.append(
            "- Shared registries (tools/coop/targets.json, docs/evidence/decomp/attempts.jsonl, "
            "config/<region>/symbols.txt) are merged back centrally by the harness after your "
            "batch finishes. Commit your changes to them as normal (claim/cycle write them), "
            "but never hand-edit them to force a state."
        )
        env.append(
            "- The harness has configured this worktree; the first hexdiff/cycle run "
            "builds the unit (can take a few minutes), afterwards builds are incremental. "
            "Always use .venv/bin/python3 (never system python3)."
        )
    else:
        env.append(
            "- Working tree is SHARED with other agents: commit ONLY your own files and the "
            "registry (targets.json / attempts.jsonl); never git reset or revert others' work."
        )

    smt = ""
    if allow_smt:
        smt = (
            "- Stuck >90% and semantically clean? You may run `cycle <id> --smt` ONCE at "
            "acceptance time (probe etiquette: callee tree ready; never --contract variants)."
        )
    else:
        smt = "- NEVER run `cycle --smt` / `diff` / equivalence probes in-session — the harness runs those out-of-band."

    seed = ""
    if seed_text and relaunch > 1:
        seed = (
            f"\nPREVIOUS ATTEMPT NOTES (attempt {relaunch - 1} of this batch ended "
            f"{'in error' if relaunch > 1 else ''}; resume from where it left off — do NOT re-litigate "
            f"targets it already touched or blockers it already recorded):\n{seed_text[-4000:]}"
        )

    digest = ""
    if batch.get("digest_path"):
        digest = (
            f"\nBATCH KB DIGEST (read this file before iterating — it lists the top "
            f"MWCC_REFERENCE records for your batch's symbols; walls already documented "
            f"there are blockers to record, not experiments to repeat):\n{batch['digest_path']}"
        )

    stalls = [td for td in batch["target_details"] if td.get("stall")]
    stall_section = ""
    if stalls:
        rows = "\n".join(f"  {td['id']} {td['function']}: {td['stall']}" for td in stalls[:15])
        more = f"\n  ... {len(stalls) - 15} more in the table" if len(stalls) > 15 else ""
        stall_section = (
            f"\nKNOWN STALLS (from prior attempts — do NOT repeat these ruled-out "
            f"experiments; record the target as blocked and move on):\n{rows}{more}\n"
        )

    return f"""You are a Xenoblade Chronicles Wii decompilation matching agent in the co-op fork at {ROOT}.
Section: {section} | Batch {bid} ({batch['label']}): {batch['total']} targets ({batch['leaf_count']} leaf, {batch['likely_blocked']} likely callee-blocked).
Goal: reach FULL_MATCH (100% static) and ACCEPT every target in your batch. All targets in this batch are on the ready frontier (leaf, or every callee accepted and certified) — callee blockers should be rare; if you hit one, record it and move on.

ENVIRONMENT — {'ISOLATED git worktree' if worktree_path else 'shared working tree'}:
{chr(10).join('- ' + e for e in env)}

ACCEPTANCE (FULL_MATCH campaign — no SMT in-session):
- Accept only via: .venv/bin/python3 tools/coop/run.py cycle <target-id> --hypothesis "..." --next-change "..." WITHOUT --smt. FULL_MATCH auto-accepts on cycle; the cheap register-renaming witness may also certify EQUIVALENT_MATCH — both count, prefer FULL_MATCH.
{smt}
- cycle enforces the split-size check: a size mismatch is a real blocker — record it and move on.
- NEVER add stub declarations, padding, or dummy bodies to make a target's size or status line up. Size hacking corrupts the registry and wastes the batch.

READ (lazy, on demand — do NOT pre-read big docs):
1. AGENTS.md once (short, at repo root).
2. When you hit a mismatch: search the KB first — .venv/bin/python3 tools/mwcc_kb.py search "<mangled-or-term>" --json, open the top records only. Read PLAN.md §17 only if the loop requires it. Do NOT read MWCC_REFERENCE.md end-to-end.
3. Per target: .venv/bin/python3 tools/coop/run.py targets show <target-id> (callee readiness gate) — do not skip this.

YOUR BATCH {bid} — {batch['total']} targets across units: {units}
{table}

RULES (non-negotiable):
- Scope: only source files under YOUR units ({units}), shared headers (additive changes only), and configure.py Object(...) entries for YOUR units (documented flag fixes only). Never edit tooling (tools/coop, tools/ppc_equivalence, retail_reloc_map.json, configure.py globals) or other sections' files.
- High-level C/C++ only — no asm, no register/stack micro-management (isolated PS backend exception PLAN.md §17.6 only).
- No external source hunting (no web_search / fetch_content / curl). No hand disassembly — hexdiff prints it (no objdump / llvm-objdump).
- Search budget: max 3 grep/find per function; read hexdiff --json directly.
- Do NOT use the pi harness (tools/pi_harness). Do NOT use subagents. Do NOT ask questions — keep working until the batch is done or every remaining target is blocked.
- Minimal git: status/diff once at start and once before commit; commit per unit or several targets at once with batch-cycle.py — do not commit every keystroke.
- Do NOT git reset, do NOT revert other worktrees' files, do NOT push.
- Per-unit compiler flags are a legitimate tool: if MWCC_REFERENCE attributes a diff to flags (-func_align 4/16, -ipa off, mw_version="GC/3.0a5.2"), apply the documented fix to YOUR unit's Object(...), hexdiff-verify, revert if it doesn't help. No blind flag-sweeping, no failed experiments left in place.

WORKFLOW per target:
1. Claim: .venv/bin/python3 tools/coop/run.py targets claim <target-id> --owner {bid}
2. Inspect: .venv/bin/python3 tools/coop/run.py targets show <target-id> — callee-blocked? record and skip.
3. Iterate fast: .venv/bin/python3 tools/coop/hexdiff.py <unit> --symbol <mangled-or-substr> --brief ; use --json for detail. hexdiff builds itself and holds this worktree's build lock — never run ninja/configure.py directly.
4. Accept: cycle WITHOUT --smt (above), or mass-accept several at once with .venv/bin/python3 tools/coop/batch-cycle.py <id...> --default-hypothesis "..." --default-next-change "...".
5. Commit: git add <your unit files> docs/evidence/decomp/attempts.jsonl && git commit -m "paseo-{section}: match <units> batch {bid}"

{digest}{stall_section}
FINAL REPORT (end of session, plain text):
- Per target: id | function | final status (FULL_MATCH/EQUIVALENT_MATCH/ACTIVE+%) | blocker
- List of accepted targets; list of blockers (callee-blocked, wall, size)
- Reusable insights: append proven fixes to docs/MWCC_REFERENCE.md in-session (pattern, symptom, fix, match %).
{seed}
"""
