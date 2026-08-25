---
name: xenoblade-decomp
description: >-
  Primary agent workflow for the Xenoblade Chronicles Wii decompilation
  fork. Invoke at the start of tasks in this repository when decompiling,
  matching, editing src/kyoshin or configure.py, running the coop runner,
  objdiff, DECOMP_MAP targets, or MWCC EQUIVALENT_MATCH / FULL_MATCH work.
---

# Xenoblade decompilation

Operational workflow + acceptance policy for this **private/downstream** decomp/matching fork.
Read only the routing doc for the residual you're chasing (see **Routing**).

## Rules that never change

> **Authoritative source for the policy below is `PLAN.md` §17** (and §2 for the fork boundary); this is the session-facing summary — if files ever disagree, PLAN §17 wins.

- **Private fork** — do NOT submit LLM-assisted matching upstream to `xbret/xenoblade`.
- **Source is high-level C/C++ only.** No asm, `register`, or stack tricks outside the
  §17.6 Gekko paired-single backend (see **Policy exceptions**).
- **Acceptance** = `EQUIVALENT_MATCH` (fuzzy ≥ 50% **and** witness-certified equivalence **and**
  split-size fit) **or** `FULL_MATCH` (100% static + split-size fit). Same tier; **prefer
  `FULL_MATCH`** when reachable (cheaper to certify). Unit level always needs 100% code+data.
  `STRUCTURAL` / `CODE_MATCH` / `HIGH_MATCH` are **not** final.
- **no-SMT policy (hard):** the Z3 probe is **disabled** here. Never run `--smt`/`--linked` or
  plain `run.py diff`. The only equivalence path is the **register-renaming witness** inside
  `cycle`/`batch-cycle` (runs by default, no flag). A target the witness can't certify must reach
  `FULL_MATCH` or stay a recorded near-miss.
- **Matching is always possible.** A plateau = the current angle is exhausted, not impossible.
  Angles in order: source/declaration/expression shape → `mw_version`/unit flags → reloc naming
  (`extern "C" lbl_eu_*`) → register witness → tooling → §17.6 exception. Mark `BLOCKED` only for a
  concrete external/tooling limit. Never say a target is impossible; record the residual + next angle.
- **Callee gate:** if any callee isn't `FULL`/`EQUIVALENT_MATCH`, witness certification closes and
  only `FULL_MATCH` works — push for it, don't default to moving on.
- **`tools/coop/targets.json` is the sole source** of function identity/state — use
  `targets show/status`, never hand-maintained checklists. `docs/ownership.csv` is legacy.

## Glossary

| Term | Meaning |
|------|---------|
| `FULL_MATCH` | 100% static byte match (code + relocs + size); automatic `full-instruction-match` cert, no solver. |
| `EQUIVALENT_MATCH` | Fuzzy ≥ 50% **and** the register-renaming witness certifies equivalence **and** split-size fit. |
| `reg_swap` | Same instructions, different register colors → `docs/register_mapping.md`. |
| `structural` | Opcode/immediate/branch differs → `docs/scheduling.md` / `docs/instruction_selection.md`. |
| `witness` | The register-renaming proof that certifies `EQUIVALENT_MATCH`; runs inside `cycle`/`batch-cycle`. |
| `no-SMT` | Z3 solver is disabled here; never run `--smt`/`--linked`/plain `run.py diff`. |
| `split-size` | Decomp `.text` ≤ retail `splits.txt` budget; gates unit promotion. |
| `callees-accepted` | A safe frontier where every callee is already `EQUIVALENT_MATCH`/`FULL_MATCH`. |
| `policy_exception` | §17.6 allowed escape; must be logged in `attempts.jsonl`. |

## Before you edit

- Read `PLAN.md` §§2, 3, 17 (legal boundaries, invariants, matching loop). Read other sections only
  when the task touches that subsystem.
- `python3 tools/coop/run.py targets show <id>` — confirm identity + current state.
- For MWCC work, search the KB first (below), open the top records, and cite knowledge IDs in the
  cycle hypothesis. Don't repeat a recorded failed experiment without a new reason.

## Setup

```bash
cp tools/coop/coop.example.json coop.json   # first time only
# retail files at orig/<region>/sys/main.dol and orig/<region>/files/rels/*.rel  (default region: us)
python3 tools/coop/run.py status
python3 tools/coop/run.py baseline          # sha1 + configure + ninja
```

Always use `.venv/bin/python3` (venv has 3.13.6; system python 3.9 fails on project syntax).

## Scratch / compiler experiments

All throwaway probes (MWCC behavior, flag comparisons, snippets, decomp.me ctx drafts) live in the
repo-root `.scratch/` (gitignored). Never in `src/**`/`libs/**`/`tools/**`; never commit them.

## Pick a target

```bash
python3 tools/coop/run.py targets list | show <id> | sync-calls
python3 tools/coop/run.py targets claim <id> --owner <agent>   # then release when done
python3 tools/coop/run.py targets claim-smallest --owner <agent> [--num 5]  # quick wins
```

- Skip `buildable=no` until source exists (recover via Ghidra, add `.cpp` to `configure.py`).
- Frontiers (safe for bottom-up): `leaf`, `callees-accepted`, `ready`. Refresh certificates
  leaves-first before expecting a callee frontier:
  `targets recertify --bottom-up` then `harness --selection ready --include-catalog --dry-run`.
- Rerun `sync-calls` after regenerating retail asm / changing the symbol map.

## Decompilation loop

Per target: export retail asm/symbols (**reference only**) → search KB by identity + mismatch →
draft high-level C/C++ in the owning TU → ✱**iterate with `hexdiff`** (fast; `cycle` is the final
gate, ~1s vs 2-3min):

```bash
python3 tools/coop/hexdiff.py <unit> --symbol <mangled> [--brief|--json|--asm|--relocs]
python3 tools/coop/hexdiff.py <unit> --all      # unit triage: one build, per-function table
```

- Read the one-line verdict + `reg_swap`/`structural` breakdown. If a change raises a count, **revert**.
  After 3 non-improving attempts, record an open-item packet (status/%, size, categories, ruled-out
  hypotheses, exact residual, next 3 experiments) and switch angle.
- **Prefer `hexdiff` over raw `ninja`** — it owns the build + repo-wide lock
  (`build/<region>/.hexdiff.lock`), safe for concurrent agents. Only run `ninja`/`configure.py`
  when hexdiff can't express it (e.g. full-tree rebuild). Find symbols with `hexdiff --list`/`--asm`,
  not objdump/grep. No external source hunting (retail asm + MWCC_CASES + codebase only).
- **Acceptance** (only when hexdiff is clean, or an open item is recorded):
  `python3 tools/coop/run.py cycle <id> --hypothesis "..." --next-change "..."` — the witness runs
  inside by default. On FAIL, inspect build/coop-function-diff.json, revise, retry. On PASS the state
  persists in `targets.json`; **release the claim**; don't edit the same function concurrently.
- **Split-size gate** (unit promotion): `python3 tools/coop/run.py size <unit>` — decomp `.text` must
  be ≤ retail `splits.txt` budget. Size overflow blocks TU promotion (not per-function acceptance).
- Batch: `batch-cycle.py` with `--hypothesis-map` / `--default-hypothesis` / `--summary` (no `--linked`).

### pi-harness sessions (no bash)

Structured tools cover the loop: `hexdiff` (→ hexdiff.py), `symbols`, `targets`, `kb` (→ mwcc_kb.py),
`ctx` (→ run.py ctx), plus read/edit/write/grep/find/ls. Hard blocks enforced at tool level: no
`--smt`/`--linked`, no plain `run.py diff`, no git push/revert, no `cycle`/`ninja`/`configure.py`
(harness-owned), no registry writes. The witness is the only equivalence path, run inside `cycle`.

## Symbol recovery (`run.py symbols`)

Run **after** a match (or when investigating `UnkClass_*`/`func_*`), before renaming types:
`symbols list --kind UnkClass | show <addr> | xref <addr> | demangle <sym> | rename-plan <old> <new> | rename-all <old> <new> [--dry-run]`.

- `rename-all` updates symbols + source + `configure.py` + `splits.txt` + file renames. Prefer
  **same-length** names (avoids re-mangling); `--force` for mismatch. `rename-apply` alone = maps only.
- `_Fv`/`_Fi`/`_FUl` suffixes are **uneducated guesses**, never signature evidence — recover the real
  param list from retail usage (`symbols xref`, `hexdiff --asm`).
- Add new namespaces to `KNOWN_NS_PREFIXES` in `tools/symbolrecover/lib/mwcc.py`.

## Logging & evidence

- Append to `docs/evidence/decomp/attempts.jsonl`: `target_id, function, status, instruction_match,
  hypothesis, next_change` (optional `runtime_test`). Append-only history; live state = `targets.json`.
- **Breakthrough?** append it in the same session — **general/reusable** fix → `docs/MWCC_PATTERNS.md`,
  **per-target** record → `docs/MWCC_CASES.md` (don't bury it in attempts.jsonl). After 100%, add
  Dolphin proof when `PLAN.md` requires `BEHAVIOR_VERIFIED`.

  **Use this record format (both files):**

  ```
  ## <symbol/unit> — <residual> → <fix> (<compiler flags>, <result>)
  - Symptom:   what hexdiff shows
  - Cause:     why retail differs
  - Fix:       the concrete change / recipe
  - Result:    FULL_MATCH / EQUIVALENT_MATCH / <n>% near-miss
  - Confidence: repo_proven / hypothesis / negative_result   # general (PATTERNS) only
  - Applies to/a.k.a.: related flags, §17.6 escapes, filenames   # general (PATTERNS) only
  ```

## MWCC knowledge (patterns + cases)

```bash
python3 tools/mwcc_kb.py search "<symbol-or-mismatch-terms>" --kind reference --json
python3 tools/mwcc_kb.py search "<terms>" --kind attempt --json   # and show <id>
```

The SQLite index covers **both** `docs/MWCC_PATTERNS.md` (general) and `docs/MWCC_CASES.md`
(per-target) plus attempts/contributions — one search, either bucket. Search order: exact symbol →
one query per mismatch category → repo-proven patterns → prior attempts. Open full records — don't
act from snippets. Full protocol: `docs/MWCC_PATTERNS.md`.

## Mark a TU matching

```python
Object(Matching, "kyoshin/cf/CfPadTask.cpp"),   # was NonMatching
```
in `configure.py`, then reconfigure and verify `ninja` succeeds (via hexdiff if possible).

## Common mismatch fixes

Fix semantics/types first, then expression order with normal C++ (named locals/helpers,
`if/else`/`switch`, no `goto` chains copied from asm). **No register/stack tricks.**

- signed vs unsigned; wrong struct field width/type.
- branch/switch lowering — natural control flow, not asm-shaped gotos.
- expression order — break into named locals/helpers; no `register`/`asm("rN")`.
- missing `-O4,s` or per-unit `extra_cflags` in `configure.py`; wrong virtual/adjusted-this call.
- relocation target — declare globals via `extern "C"` retail linker names; access as normal objects.
- ABI — real C++ params/structs; never fake `Fv`/`u32* r4` register params.
- templates in headers — `#pragma auto_inline off` + explicit instantiation + `#pragma pop`;
  check `Ui` vs `Ul` (`unsigned int` on PPC32). See `MWCC_CASES` §Template pitfalls.

## Policy exceptions (PLAN.md §17.6) — only after C++ is exhausted

The only allowed escapes from high-level C/C++ are listed in **PLAN.md §17.6**: the `DECOMP_PPC_*`
builtins (`decomp.h`), `extern "C" lbl_eu_*` reloc naming, goto-gate chains, the isolated Gekko
paired-single backend, and the Wii boot-entry vectors (`InitMetroTRK*`). **Read §17.6 for the
authoritative boundaries before using any.** Every use must be logged in `attempts.jsonl` with
`"policy_exception": true`.

**Still forbidden everywhere else:** `register rN`, fake `sp[]`, inline `asm{}`, `asm void`/.s units,
transcribed retail asm, and binary patching / `postprocess_reloc_names.py` (a narrow linker-ADDR16
bake for DOL-split absolutes like `_stack_addr` is the only ok case).

## Do not

- Commit `orig/`, `main.dol`, RELs, or disc assets; submit AI reconstruction upstream.
- Decompile to asm or registers outside the PS backend; micro-manage registers/stack.
- Chase byte-identity via binary patching / instruction insertion.

## Routing — read the doc for the residual you're chasing

| Residual | Read |
|---|---|
| `reg_swap` (pure color diff) | `docs/register_mapping.md` (declaration-order allocator contract) |
| `structural` (scheduling / loop) | `docs/scheduling.md` (`-O4,p` vs `-O4,s`) |
| `mr`/`addi`/`lbzu` / load selection | `docs/instruction_selection.md` |
| reloc name drift (bytes match, names differ) | `tools/coop/reloc_map.py diff/mine` (→ `extern "C"`) |
| unknown type / name | symbol recovery above |
| plateau ~97-99.9% | `MWCC_CASES` "Quick diagnostic" |
| hard matching question | search both via `mwcc_kb.py` (patterns + cases) |

## Key paths

| Path | Role |
|------|------|
| `tools/coop/targets.json` | Canonical function registry + state (`targets show/status`) |
| `tools/coop/run.py` | Runner CLI (`targets`/`symbols`/`behaviour`/`size`/`cycle`) |
| `tools/coop/hexdiff.py` | Headless hex diff — the iteration tool |
| `tools/coop/reloc_map.py` | Reloc name-drift detection/miner |
| `tools/coop/batch-cycle.py` | Mass-acceptance after matching |
| `configure.py` | Per-object matching flags / `mw_version` |
| `docs/MWCC_PATTERNS.md` | General/reusable MWCC knowledge + KB protocol + templates |
| `docs/MWCC_CASES.md` | **Per-target** matching records (search via `mwcc_kb.py`) |
| `tools/mwcc_kb.py` | Search patterns + cases + attempt history (SQLite index) |
| `PLAN.md` | Architecture / matching policy (§§2, 3, 17) |
