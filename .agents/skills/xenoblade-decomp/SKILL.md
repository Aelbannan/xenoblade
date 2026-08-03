---
name: xenoblade-decomp
description: >-
  Primary agent workflow for the Xenoblade Chronicles Wii co-op decompilation
  fork. Invoke at the start of tasks in this repository when decompiling,
  matching, editing src/kyoshin or configure.py, running the coop runner,
  objdiff, DECOMP_MAP targets, or MWCC EQUIVALENT_MATCH / FULL_MATCH work.
---

# Xenoblade decompilation

> **Auto-loaded** via root `AGENTS.md` and this skill. Follow it at the start of
> tasks in this repository when decompiling, matching, editing `src/kyoshin` or
> `configure.py`, running the coop runner, objdiff, `DECOMP_MAP` targets, or
> MWCC `EQUIVALENT_MATCH` / `FULL_MATCH` work.

## Before you edit code

1. Read `PLAN.md` §2 (legal boundaries), §3 (architecture invariants), and §17 (decompilation loop).
2. Run `targets show <target-id>` and treat `tools/coop/targets.json` as the sole source of function identity and current target state.
3. For MWCC matching work, search the knowledge base by function/symbol and
   observed mismatch before editing source; open the relevant full records from
   `docs/MWCC_REFERENCE.md` and prior attempts (protocol below).
4. Confirm this is a **private/downstream fork** — do not upstream LLM-assisted matching work to `xbret/xenoblade`.

**Current policy:** every target must reach **`EQUIVALENT_MATCH`** (function fuzzy ≥ 50% **and** `ppc_equivalence` proves `EQUIVALENT` under effect-aware `auto`—`ppc-eabi` or stronger—**and** split-size fit) or **`FULL_MATCH`** (100% static **and** split-size fit). Both are equal-tier acceptance outcomes, **but prefer `FULL_MATCH` when it is reachable**: a 100% static match is cheaper to certify (automatic `full-instruction-match` certificate, no `--smt` needed) and is stronger evidence than a semantic proof. Use `EQUIVALENT_MATCH` when 100% static is genuinely unreachable (register allocation, scheduling, immediate/instruction selection, FP codegen). Unit-level (no symbol) still requires 100% code + data.

**SMT probe is opt-in (cost control):** `coop run cycle` runs only the cheap pre-SMT register-renaming witness by default; the full Z3 probe runs only with `--smt`. `coop run diff` keeps the full probe on by default (use `--no-smt` to skip while iterating). A skipped probe logs `inconclusive_smt_disabled` and can never reach `EQUIVALENT_MATCH` — to accept a function the witness cannot certify (scheduling/immediate/instruction-selection diffs), run `cycle <target-id> --smt` and confirm status is `EQUIVALENT_MATCH`. When a function is stuck above ~90% and looks semantically equivalent, run `diff <unit> --symbol <sym>` (full probe) for the divergence oracle before rewriting. **FULL_MATCH (100% static) targets are unaffected:** they still get a `full-instruction-match` certificate automatically on `cycle` (or `batch-cycle`) even without `--smt` — byte-identical bodies are certified without the solver, so the `callees-accepted` frontier keeps populating.

**Probe etiquette (each probe costs 15-30 min of machine time):** run the probe **once, at acceptance time** — not during iteration (hexdiff is the iteration tool). Before `cycle --smt`, confirm the callee tree is ready: indirect calls, unresolved callees, or `called_functions` not yet FULL_MATCH/EQUIVALENT_MATCH fail closed no matter what the probe does (`cycle` prints this as an early warning). On a callee-blocked target, spend a cycle only to land FULL_MATCH; otherwise record the blocker and move on. Never retry the same function with `--contract` variants (strict/live-out/memory/ppc-eabi) — accept the `auto` outcome. Concurrent probes starve hexdiff builds and have hung agents for hours; when other agents are active, prefer deferring the probe to a quiet moment.

**Source language:** reconstruction must be **high-level C or C++ only** (MWCC), except for the isolated Gekko paired-single backend exception in §17.6. Express recovered **semantics** — fields, locals, control flow, and normal function calls — rather than register-level or stack-level implementation detail outside that exception.

Use retail assembly only as **read-only reference** (objdiff, Ghidra, `build/us/asm/`) except inside a documented isolated PS backend. Do **not** ship arbitrary assembly, register/stack micro-matching, standalone `.s` fragments, or other non-C/C++ source in `src/**` or `libs/**`.

**High-level means:** readable code a human would write without looking at disassembly — struct members, parameters, return values, `if`/`else`, loops, and small named helpers. **Not** micro-matching prologue/epilogue shape in source.

## One-time setup

```bash
cp tools/coop/coop.example.json coop.json
# Place retail files at orig/<region>/sys/main.dol and orig/<region>/files/rels/*.rel

python3 tools/coop/run.py status
python3 tools/coop/run.py baseline   # sha1 + configure + ninja
```

Region defaults to `us` in `coop.json`. Change with `--config` or edit `region`.

## Scratch files and compiler experiments

All throwaway scratch/compiler tests — MWCC behavior probes, isolated flag
comparisons, small C/C++ snippets, decomp.me ctx drafts — go in the repo-root
`.scratch/` directory (gitignored). Do not create scratch files in `src/**`,
`libs/**`, `tools/**`, or other committed paths, and do not commit `.scratch/`
contents. Anything you wouldn't ship stays there.

```bash
mkdir -p .scratch   # e.g. .scratch/mwcc_flag_probe.cpp, .scratch/ps_backend_test.cpp
```

## Scratch files and compiler experiments

All throwaway scratch/compiler tests — MWCC behavior probes, isolated flag
comparisons, small C/C++ snippets, decomp.me ctx drafts — go in the repo-root
`.scratch/` directory (gitignored). Do not create scratch files in `src/**`,
`libs/**`, `tools/**`, or other committed paths, and do not commit `.scratch/`
contents. Anything you wouldn't ship stays there.

```bash
mkdir -p .scratch   # e.g. .scratch/mwcc_flag_probe.cpp, .scratch/ps_backend_test.cpp
```

## Scratch files and compiler experiments

All throwaway scratch/compiler tests — MWCC behavior probes, isolated flag
comparisons, small C/C++ snippets, decomp.me ctx drafts — go in the repo-root
`.scratch/` directory (gitignored). Do not create scratch files in `src/**`,
`libs/**`, `tools/**`, or other committed paths, and do not commit `.scratch/`
contents. Anything you wouldn't ship stays there.

```bash
mkdir -p .scratch   # e.g. .scratch/mwcc_flag_probe.cpp, .scratch/ps_backend_test.cpp
```

## Pick a target

```bash
python3 tools/coop/run.py targets list
python3 tools/coop/run.py targets show <target-id>
python3 tools/coop/run.py targets sync-calls
```

Prefer buildable targets inside the user's named feature slice, or let the harness pick the `ready` frontier when the user has not named a function. Skip targets with `buildable=no` until source exists — recover via Ghidra first, add `.cpp` to `configure.py`, reconfigure.

For bottom-up matching across the complete symbol catalog, use the retail call
graph. `leaf` requires no direct, unresolved, or indirect calls;
`callees-accepted` requires at least one direct callee and every callee
accepted (`EQUIVALENT_MATCH` / `FULL_MATCH`) with a current semantic
certificate; `ready` is their union. When accepted targets lack certificates,
refresh them leaves-first before expecting a `callees-accepted` frontier:

```bash
python3 tools/coop/run.py targets recertify --bottom-up --dry-run
python3 tools/coop/run.py targets recertify --bottom-up
python3 tools/coop/run.py harness --selection leaf --include-catalog --dry-run
python3 tools/coop/run.py harness --selection callees-accepted --include-catalog --dry-run
python3 tools/coop/run.py harness --selection ready --include-catalog --limit 20
```

Functions with indirect or unresolved calls are excluded from these safe
frontiers until those edges are modeled. Rerun `targets sync-calls` after
regenerating retail assembly or changing the symbol map.

Claim the target before editing; the registry supplies the source path as the default exclusive scope:

```bash
python3 tools/coop/run.py targets claim <target-id> --owner <agent>
python3 tools/coop/run.py targets release <target-id> --owner <agent>
```

To claim the smallest NOT_STARTED function(s) by binary size (useful for quick
wins or onboarding new agents):

```bash
# Claim the single smallest function
python3 tools/coop/run.py targets claim-smallest --owner <agent>

# Claim the 5 smallest
python3 tools/coop/run.py targets claim-smallest --owner <agent> --num 5

# Just list the smallest without claiming
python3 tools/coop/run.py targets claim-smallest --no-claim --num 10
```

Each claimed target prints its id, demangled function name, source path, and
binary size. Only buildable function-kind targets with `NOT_STARTED` status
are considered.

`docs/ownership.csv` is legacy history, not current coordination state.

## Symbol recovery (`tools/symrecover.py`)

After **`EQUIVALENT_MATCH`** / **`FULL_MATCH`** on a function (or when investigating `UnkClass_*` / `func_*` placeholders), run symbol recovery **before** renaming types in source.

```bash
# List unknown placeholder types in the active region
python3 tools/coop/run.py symbols list --kind UnkClass

# Inspect one type (name or address suffix)
python3 tools/coop/run.py symbols show 8043C59C
python3 tools/coop/run.py symbols xref 8043C59C

# Demangle symbols from config/<region>/symbols.txt
python3 tools/coop/run.py symbols demangle func_80459270__17UnkClass_8043C59CFv

# Plan a rename (checks MWCC mangling length compatibility)
python3 tools/coop/run.py symbols rename-plan UnkClass_8043C59C CViewRectData --verbose

# Apply symbol-map rename only
python3 tools/coop/run.py symbols rename-apply UnkClass_8043C59C CViewRectData --dry-run

# Apply full rename: symbols + source + configure + splits + file renames
python3 tools/coop/run.py symbols rename-all UnkClass_8043C59C CViewRectDataCore --dry-run
python3 tools/coop/run.py symbols rename-all UnkClass_8043C59C CViewRectDataCore
```

Equivalent standalone CLI: `python tools/symrecover.py <subcommand> …`

**Workflow when a type is understood:**

1. `symbols show` + `symbols xref` — methods, namespaces, split unit, source files.
2. `symbols demangle` on each symbol in the unit — recover method names and signatures.
3. `symbols rename-plan <old> <new>` — confirm mangling-compatible length when possible.
4. Decompile / match all functions in the unit at `EQUIVALENT_MATCH` (or `FULL_MATCH`).
5. **`symbols rename-all <old> <new>`** — updates `symbols.txt`, source, `configure.py`, `splits.txt`, the legacy ownership history, and renames `UnkClass_*.cpp/.hpp` files (use `--dry-run` first). Update the canonical target record when its symbol changes.
6. `python3 configure.py && ninja` and `coop run diff` every affected symbol.
7. Log the recovered name in `attempts.jsonl` (`hypothesis` / `next_change`).

**Rules:**

- Retail `main.dol` is stripped — `symbols.txt` names are decomp annotations; recovered names must still match MWCC mangling for matching.
- Prefer **same-length** renames (`UnkClass_8045F564` → `CLibLayoutRegion`, 17 chars) to avoid re-mangling every symbol. `rename-all` refuses length mismatches unless `--force`.
- `rename-apply` without `--all` only edits symbol maps; use **`rename-all`** (or `rename-apply --all`) for source and build files.
- Headers already named semantically (e.g. `CViewRectData.hpp`) are updated in place; only files **named** `UnkClass_<addr>.cpp` are renamed on disk.
- Extend `KNOWN_NS_PREFIXES` in `tools/symbolrecover/lib/mwcc.py` when you find new namespaces (`cf`, `ml`, `LOD`, `mpfsys`, …).

## Decompilation loop

For each target:

```text
export assembly/symbols/types (Ghidra or objdiff) — **reference only**
→ search MWCC knowledge by identity + mismatch; open top records
→ draft/edit **high-level C or C++** in the owning translation unit
→ python3 tools/coop/run.py ctx <source.cpp>
→ **Rapid feedback loop** (use `hexdiff`, not `cycle`; ~1s vs 2-3min):
    python3 tools/coop/hexdiff.py <unit> --all          # unit triage: one build, table of all functions
    python3 tools/coop/hexdiff.py <unit> --symbol <mangled-symbol> --brief
    → check the one-line verdict (mismatch/structural/reg_swap) went down; if it went up, revert the edit
    → iterate until 0 structural / 0 mismatches or stalls for 3 attempts
→ **Final acceptance** (only when hexdiff shows few misses or 3 attempts stalled):
    python3 tools/coop/run.py cycle <target-id> \
        --hypothesis "..." --next-change "..." --runtime-test ""
    # If fuzzy is in [50, 100) and the register-renaming witness did not
    # certify, re-run with --smt so the full probe can reach EQUIVALENT_MATCH.
→ verify split object size: `coop run size <unit>` (decomp `.text` ≤ retail split budget)
→ optional: `behaviour ppc <test-id>` when a PPC harness exists
→ if `cycle` FAILS: inspect objdiff / build/coop-function-diff.json, revise, repeat
→ if `cycle` PASSES: the accepted state is persisted in `targets.json`; release the claim and do not edit the same function concurrently
```

### Batch cycle (mass-acceptance after matching)

After matching a set of functions (e.g. an entire unit or milestone), mass-cycle
all of them at once with `batch-cycle.py` instead of running `cycle` one-by-one:

```bash
# Per-target hypothesis/next-change via JSON map
python3 tools/coop/batch-cycle.py us-80345678 us-80345680 \
    --hypothesis-map batch-map.json

# Shared defaults for all targets
python3 tools/coop/batch-cycle.py us-80345678 us-80345680 \
    --default-hypothesis "high-level C reconstruction complete" \
    --default-next-change "verify static match and equivalence"

# Dry-run to preview
python3 tools/coop/batch-cycle.py us-80345678 \
    --hypothesis-map batch-map.json --dry-run

# Write structured JSON summary for agent handoff / CI
python3 tools/coop/batch-cycle.py us-80345678 us-80345680 \
    --default-hypothesis "batch cleanup" \
    --default-next-change "accept if pass" \
    --summary /tmp/batch-summary.json

# Allow linked DOL/ELF fallback for SMT equivalence
python3 tools/coop/batch-cycle.py us-80345678 --linked
```

Processes targets sequentially, continues on failure, exits 0 only when all pass.
Full reference: `batch-cycle.py --help`.

Hypothesis map JSON format (`target_id` → per-target overrides):

```json
{
  "us-80345678": {
    "hypothesis": "specific hypothesis text",
    "next_change": "specific next change text",
    "runtime_test": "behaviour:<test-id>"
  }
}
```

### Bounded attempt protocol

- State one mismatch hypothesis and make one bounded source change per cycle.
- Preserve the best-known candidate; do not compound regressions with unrelated edits.
- After three non-improving attempts, record a stall packet: best status/percent,
  size result, mismatch categories, ruled-out hypotheses, exact blocker, and the
  next three bounded experiments.
- A stall is not acceptance. Keep the target `ACTIVE` or set it `BLOCKED` only
  for a concrete external/tooling limitation.
- Final handoff must report target status, static percent, equivalence result and
  contract when applicable, size result, changed files, reusable insight, claim
  release state, and remaining risk.

### Behaviour comparison (static + optional PPC)

Retail vs decomp objects can be checked with `tools/test/compare_behaviour/` (no host dual-oracle layer):

```bash
python3 tools/coop/run.py behaviour audit              # size budget for registered tests
python3 tools/coop/run.py behaviour compare <test-id>  # static + ppc if present
python3 tools/coop/run.py behaviour ppc <test-id>      # headless Dolphin only
```

**Rules:**

- Acceptance bar remains **`EQUIVALENT_MATCH`** (or `FULL_MATCH`) + split-size fit.
- Host `*.cpp` dual-oracle tests were **removed** — do not add them back.
- Below 100%, continue matching toward `EQUIVALENT_MATCH` (SMT + split-size) or `FULL_MATCH`, with optional **PPC** when `ppc_source` is set.
- Full policy: `tools/test/compare_behaviour/README.md`.

### Split object size (required before `Matching` / acceptance)

Each translation unit has a fixed retail `.text` slice in `config/<region>/splits.txt`. The decompiled object’s **`.text` section** must not exceed that budget — otherwise the unit cannot be linked into `main.dol` at the retail address.

```bash
python3 tools/coop/run.py size monolib/src/core/CViewRectDataCore
python3 tools/coop/run.py size --all
```

**Rules:**

- `diff`, `cycle`, and `behaviour compare` print a `size:` line and **exit non-zero** when decomp `.text` exceeds the split budget.
- Behaviour tests can pass while size fails (semantics ≠ codegen fit) — treat size overflow as a blocker for acceptance and `configure.py` `Matching` promotion.
- Retail budget = `splits.txt` `.text end - start`; compared against ELF `.text` in `build/<region>/src/...o` vs retail `build/<region>/obj/...o`.
- Implementation: `tools/coop/lib/object_size.py`.

Manual steps equivalent to `cycle`:

```bash
python3 tools/coop/run.py ctx src/kyoshin/cf/CfPadTask.cpp
python3 tools/coop/run.py build kyoshin/cf/CfPadTask
python3 tools/coop/run.py diff kyoshin/cf/CfPadTask --symbol <mangled-symbol>
python3 tools/coop/run.py size kyoshin/cf/CfPadTask
```

### Instruction-level hex diff (`tools/coop/hexdiff.py`)

**Primary rapid feedback tool** — ~1s per build+diff vs 2-3min for `cycle`. Use hexdiff during iterative editing, run `cycle` only for final acceptance.

> **Prefer hexdiff over raw ninja:** hexdiff performs the build itself and holds the repo-wide build lock (`build/<region>/.hexdiff.lock`), making it safe for concurrent agents. Only run `ninja`/`configure.py` directly when hexdiff cannot express the operation (e.g. full-tree rebuild after reconfiguration).

```bash
# Terminal mode — colour-coded side-by-side, one-line verdict first
python3 tools/coop/hexdiff.py <unit> --symbol <mangled-symbol>

# JSON mode — machine-readable, consumable by scripts / cycle fallback
python3 tools/coop/hexdiff.py <unit> --symbol <mangled-symbol> --json

# Skip rebuild when the object is already up to date
python3 tools/coop/hexdiff.py <unit> --symbol <mangled-symbol> --no-build

# Show relocation tables alongside the diff
python3 tools/coop/hexdiff.py <unit> --symbol <mangled-symbol> --relocs

# Unit triage — one build, per-function match table (no --symbol needed)
python3 tools/coop/hexdiff.py <unit> --all

# List retail function symbols to find mangled names (no build)
python3 tools/coop/hexdiff.py <unit> --list [substr]

# Iteration mode — one-line verdict, then mismatched instructions only
python3 tools/coop/hexdiff.py <unit> --symbol <mangled-symbol> --brief

# Full clean disassembly of both sides (replaces objdump)
python3 tools/coop/hexdiff.py <unit> --symbol <mangled-symbol> --asm
```

**Enhanced output** (terminal and JSON):
- **One-line verdict first** — every terminal run opens with `name: 84.7% | 0 structural | 20 reg_swap | 0x20c/0x20c PASS` — triage without parsing output.
- **Reg-swap vs structural breakdown** — terminal e.g. `6 mismatch(es), 6 pure reg-swaps (100%), 13 relocs`. JSON: `reg_swap_count`, `structural_count`.
- **Register mapping table** — terminal and JSON `reg_mapping` show retail→decomp register pairs per instruction/opcode/operand-position. E.g. `addi: r3→r5, lwz: r5→r3, psq_l: r3→r5, r5→r3` — instantly reveals Chaitin swap patterns.
- **Compiler config line** — the unit's configured `mw_version`/`extra_cflags` from configure.py is printed (e.g. `GC/3.0a5.2 -func_align 4`), so you know the exact compiler contract before touching flags.
- **KB hints** — detects known MWCC_REFERENCE stall signatures (alignment nop `ori r0,r0,0` near `mtctr` on one side; bte-family unit without `-func_align 4`) and prints the documented flag fix.
- **Reloc name-drift section** — terminal ends with `Reloc name drift (N):` listing each byte-identical/reloc-name-different site with the approved source fix (`extern "C"` declaration) plus an EQUIVALENT_MATCH fallback note when the symbol can't be named in source; JSON adds `reloc_drift` + `reloc_suggestions`. Uses the mined map (below); rebuild it after accepting reloc fixes.
- **Per-instruction flags** — JSON per-offset entries include `retail_asm`, `decomp_asm`, `reg_swap` (bool), `structural` (bool).

**Workflow rules:**
- **Find symbols with `hexdiff --list`, read disassembly with `--asm`** — do NOT run objdump / llvm-objdump / powerpc-eabi-objdump or grep `.s` files for names; hexdiff prints the same disassembly with match annotations. Read the one-line verdict — do NOT grep hexdiff's JSON apart. Search budget: max 3 grep/find per function.
- **Per-unit compiler flags are a documented matching tool:** if hexdiff shows a diff that MWCC_REFERENCE attributes to flags (`-func_align 4/16`, `-ipa off`, `mw_version="GC/3.0a5.2"`), search MWCC_REFERENCE for the documented fix and apply it to the unit's `Object(...)` in configure.py (`mw_version` / `extra_cflags`), then hexdiff-verify and revert if it doesn't help. Do NOT blind-sweep flag combos, edit `cflags_sdk` globals, or leave failed flag experiments in configure.py.
- **No external source hunting:** retail ASM + MWCC_REFERENCE + the codebase are the only references. Do NOT web-search / curl external BTE or SDK sources — wiced-history is Broadcom-proprietary, and downloaded C is never compiled with MWCC so it cannot match retail codegen.

Output legend:
- **Green** — instruction bytes match
- **Red** — byte mismatch (retail hex vs decomp hex shown side by side)
- **Yellow** — unresolved ELF relocation placeholder (the linker will fill this)

The `<unit>` argument accepts any objdiff unit hint (e.g. `kyoshin/COccCulling`)
or source path (e.g. `src/kyoshin/COccCulling.cpp`). The `--symbol` accepts the
mangled name, case-insensitive exact, or unique substring.

The tool uses the same ELF parser as `ppc_equivalence`
(`tools/ppc_equivalence/elf_symbols.py`) and automatically resolves the
retail/decomp `.o` pair from the objdiff project config. It builds the decomp
object via `ninja` before diffing unless `--no-build` is passed.

**When to use:** before editing source to understand the exact mismatch pattern
(register swap, instruction selection, branch target, relocation), and after
each edit to verify improvement or spot regressions. The `--json` output is
designed as a drop-in replacement for `objdiff-cli diff -o` when the cycle
command's function-diff JSON is unavailable.

### Reloc name-drift map (`tools/coop/reloc_map.py`)

Standalone detector + repo map miner for MWCC_REFERENCE §1 (the #1 cause of
99.3-99.9% near-misses: bytes identical, reloc *names* differ).

```bash
# Per-function reloc drift + concrete fixes
python3 tools/coop/reloc_map.py diff <unit> --symbol <mangled-sym> --no-build

# Batch-mine the named-symbol map across every retail/decomp objdiff pair
python3 tools/coop/reloc_map.py mine          # → tools/coop/retail_reloc_map.json
python3 tools/coop/reloc_map.py show --global-only
python3 tools/coop/reloc_map.py show --symbol spInstance
```

The miner aligns relocs **per function pair** (same name + equal `.text` size)
and classifies `name` / `addend` / `layout` / `structural` drift; TU-local
labels (`@N`, `...bss.0`) get unit-scoped keys. Re-run `mine` after accepting
reloc fixes so suggestions refresh. Tests: `tools/coop/tests/test_reloc_map.py`.

### PPC semantic equivalence (optional additional evidence)

For supported bounded PPC blocks, the field decoder + Z3 checker can prove
or refute selected live-out state even when bytes differ:

```bash
python3 tools/coop/run.py equivalence check-hex \
  --original <retail-hex> --candidate <decomp-hex>
python3 tools/coop/run.py equivalence check-unit <unit> --symbol <mangled-or-token>
```

Read `tools/ppc_equivalence/README.md` before use. An equivalence result applies
only to its printed observables and assumptions. Unsupported instructions,
timeouts, and solver `unknown` are inconclusive. This check feeds
`EQUIVALENT_MATCH` when fuzzy ≥ 50%; it does not replace split-size checks. Continue to `FULL_MATCH` or close at `EQUIVALENT_MATCH` — both satisfy the acceptance bar.

`check-unit` / `check-objects` extract the named `.text` symbol from the
objdiff retail/decomp `.o` pair. Functions with unresolved ELF relocations are
inconclusive rather than proving placeholder immediates. The co-op wrapper
defaults function checks to effect-aware `--contract auto` — always use the
default; do NOT re-run probes with `--contract` variants (strict/live-out/
memory/ppc-eabi) or manual `--observe` retries. Each probe run costs 15-30 min;
contract retries are an anti-pattern (they hammer the solver and stall
concurrent agents). Accept the `auto` outcome — inconclusive means record it and
move on.

### decomp.me (optional)

For stubborn **small** functions: generate ctx → open unit in **objdiff** → Create scratch on decomp.me → paste matched code back → `cycle` again.

### Large functions

Decompose into leaf symbols/units first. Each leaf and the parent must still end at **`EQUIVALENT_MATCH`** (or `FULL_MATCH`) before the target is closed.

## Logging and evidence

- Attempts append to `docs/evidence/decomp/attempts.jsonl` (JSONL, one object per line).
- Current function/workflow state lives only in `tools/coop/targets.json`; `attempts.jsonl` is append-only history.
- Required fields: `target_id`, `function`, `status`, `instruction_match`, `hypothesis`, `next_change`.
- Optional: set `runtime_test` to `behaviour:<test-id>` / `ppc:<test-id>` when a PPC harness passes.
- After **100%** match, add Dolphin proof when `PLAN.md` requires it (`BEHAVIOR_VERIFIED`).

## MWCC patterns — search, read, and update the knowledge base

- **Before matching:** use the generated SQLite/FTS index, which combines
  `docs/MWCC_REFERENCE.md` with `docs/evidence/decomp/attempts.jsonl`:

  ```bash
  python3 tools/mwcc_kb.py search "<function-or-mangled-symbol>" --json
  python3 tools/mwcc_kb.py search "<short mismatch terms>" --kind reference --json
  python3 tools/mwcc_kb.py search "<short mismatch terms>" --kind attempt --json
  python3 tools/mwcc_kb.py show <result-id> --json
  ```

  Search in this order: exact function/symbol; one short query per observed
  mismatch category; repo-proven reference patterns; prior attempts. Start with
  the default all-terms mode. If recall is empty, remove a term or use
  `--mode any`, then narrow with `--tag`, `--kind`, or `--status`. Open the full
  top records—do not act from snippets alone. Name the relevant knowledge IDs
  in the cycle hypothesis and do not repeat a recorded failed experiment
  without a new reason. Full protocol: `docs/MWCC_KNOWLEDGE_BASE.md`.
- **After a breakthrough:** if you discover a reusable fix (new pattern, pragma/flag combo, struct/layout insight, regalloc trick, reloc naming rule, or confirmed hard cap), **append it to `docs/MWCC_REFERENCE.md`** in the same session — do not leave it only in `attempts.jsonl` or chat.
- **Where to add:**
  - Proven high-level fix → **Patterns that work in this repo** (new numbered subsection) or extend an existing one.
  - Symptom → cause → fix → **Pitfalls and failure modes** tables.
  - Compiler/tooling note → **MWCC compiler behavior** or **decomp.me workflow**.
  - Confirmed policy limit → **When FULL_MATCH or EQUIVALENT_MATCH may be unrealistic**.
- Keep entries concise: function/symbol, symptom, fix, match %, and file path. Link to retail symbol names where relevant.

## Marking configure.py matching

When a whole translation unit reaches full match, update the object in `configure.py`:

```python
Object(Matching, "kyoshin/cf/CfPadTask.cpp"),  # was NonMatching
```

Then reconfigure and verify `ninja` still succeeds.

## Common mismatch fixes (MWCC / C++)

Prefer fixing semantics and types first; only then tune expression order with normal C++ (extra locals, subexpressions, helper calls). **Do not** steer codegen with register or stack tricks.

- signed vs unsigned comparisons
- wrong struct field width or type
- branch/switch lowering shape — use natural `if`/`else` or `switch`, not `goto` chains copied from asm
- expression order — break into named locals or helpers; avoid `register` variables and `asm("rN")` bindings
- missing `-O4,s` or per-unit `extra_cflags` from `configure.py`
- incorrect virtual or adjusted-this call
- relocation target wrong — declare globals with retail linker names via `extern "C"` where needed; access them as normal C++ objects/fields
- ABI quirks — prefer proper C++ parameters and struct layout; split into helpers rather than fake `Fv`/`u32* r4` register parameters
- template functions in headers — MWCC `-inline auto` omits standalone
  bodies. Use `#pragma push` / `#pragma auto_inline off` + explicit
  `template …` instantiation + `#pragma pop` to force emission. Check
  retail symbol mangling (`Ui` vs `Ul`) — if the template uses `u32` but
  retail shows `Ui` (`unsigned int`), change the header definition to
  `unsigned int` (ABI-identical on PPC32). See `docs/MWCC_REFERENCE.md`
  §Template pitfalls for full protocol.

## Approved policy exceptions (`PLAN.md` §17.6)

When C++ and decomp.me cannot close the last instruction(s), these are **allowed** if logged in `attempts.jsonl` with `"policy_exception": true`:

| Tool | Use |
|------|-----|
| `DECOMP_PPC_RLWINM` / `DECOMP_PPC_SHL1_U32` in `decomp.h` | MWCC `__rlwinm` builtins (SDK-equivalent); opcode selection e.g. `slwi` vs `rlwinm …,16,30` |
| `extern "C" lbl_eu_*` | Reloc names when values match under `functionRelocDiffs=data_value` |
| Goto gate chains | Multi-exit guards (`setSplitLine` pattern) — not for prologue spill order alone |
| **Isolated MWCC Gekko paired-single backend** | A named Wii/MWCC kernel requires `psq_*`, `ps_*`, or `fres` operations unavailable through approved high-level C++/MWCC builtins. See the requirements below. |
| **Wii boot-entry vectors (`InitMetroTRK*`)** | A named Wii/MWCC target is a hardware boot-entry vector entered with a non-standard ABI (no valid stack frame, hardware ID in `r5`) so MWCC's mandatory frame prologue cannot reproduce it, after the C++ path is exhausted (PLAN.md §17.6). `asm void` + `nofralloc` transcribing only the named boot-vector body, guarded to MWCC with a complete C fallback for PC/non-MWCC builds. Log every use with `"policy_exception": true`. |
| **Wii boot-entry vectors (`InitMetroTRK*`)** | A named Wii/MWCC target is a hardware boot-entry vector entered with a non-standard ABI (no valid stack frame, hardware ID in `r5`) so MWCC's mandatory frame prologue cannot reproduce it, after the C++ path is exhausted (PLAN.md §17.6). `asm void` + `nofralloc` transcribing only the named boot-vector body, guarded to MWCC with a complete C fallback for PC/non-MWCC builds. Log every use with `"policy_exception": true`. |

#### Isolated Gekko paired-single backend

This is a narrow hardware-backend exception, not a general assembly allowance:

- Keep the kernel in a designated C/C++ PS backend file or `.inl` included by the owning TU, or in a clearly marked backend region; do not add a general-purpose asm utility or standalone `.s` implementation.
- Guard it to the Wii/MWCC build. Provide a complete readable scalar/high-level fallback for non-MWCC and PC builds; validate that fallback for numerical/gameplay equivalence rather than paired-single bit identity.
- Prefer `__vec2x32float__`, `__fres`, and other MWCC builtins first. Use `ASM`/`asm void` only for the documented PS kernel and its minimum loads, stores, scalar operations, comparisons, and branches.
- Compiler-managed `register`/`__REGISTER` operands are allowed only inside the isolated kernel when required by MWCC PS syntax. Explicit `register rN`/`asm("rN")` bindings, fake stack frames, hand-written prologues/epilogues, unrelated control flow, binary patching, and register/stack choreography remain forbidden.
- Log every use in `docs/evidence/decomp/attempts.jsonl` with `"policy_exception": true`, naming the target, opcode set, guard, fallback, and validation evidence.

**Still forbidden outside the isolated PS backend:** `register rN`, fake `sp[]` buffers, arbitrary inline `asm { }`, arbitrary **`asm void` / whole-function asm bodies**, standalone `.s`, and transcribed retail asm blocks. Do not use the exception to close unrelated GPR, stack, or control-flow mismatches.

## Low-level techniques — do not use in `src/**` / `libs/**` outside the isolated PS backend

- inline or standalone **assembly** outside the isolated PS backend
- `register` locals outside the isolated PS backend, especially numbered GPR names like `r3`, `r4`, `r30`, `r31`, or `asm("rN")` bindings
- `asm { mr …, r1 }` or other inline asm snippets
- `volatile` byte arrays / fake stack buffers to mirror `sp+0xC` retail offsets
- `goto` labels whose only purpose is to duplicate asm branch layout
- parameters or identifiers named after GPRs (`u32* r4`, `register CView* r30`) to force MWCC register allocation
- raw pointer arithmetic and manual field offsets when a struct member or helper would express the same logic
- codegen-forcing macros — `DECOMP_FORCELITERAL` / `DECOMP_FORCEACTIVE` and **`DECOMP_PPC_*`** (`include/decomp.h`) are approved project infrastructure

## Do not

- Commit `orig/`, `main.dol`, RELs, or disc assets
- Call `CGame::wkRender` or full frame update twice for split-screen experiments
- Accept `STRUCTURAL` / `CODE_MATCH` / `HIGH_MATCH` as final state (policy is `EQUIVALENT_MATCH`)
- Submit AI-assisted reconstruction upstream
- **Use arbitrary assembly as decompilation output** — no `asm void` bodies, inline `asm { }` of any size, or `.s` units outside the isolated PS backend; the exception is never an acceptable artifact for non-PS matching
- **Micro-manage registers or the stack in source** — use §17.6 intrinsics when C++ is exhausted
- **Post-process Chaitin / register soft-caps in `.text`** — no `insn_patches`, `insert_insns`, `reloc_offset_moves`, or any `postprocess_reloc_names.py` usage. EQUIVALENT_MATCH is the acceptance bar; do not chase byte-identity through binary patching. Narrow linker-ADDR16 bake (`bake_linker_addrs` / `force_symbol_relocs` for DOL-split absolute symbols like `_stack_addr`) is allowed

## LLM decompilation harness (tools/llm_decomp)

Conversational agent harness for matching — **replaces the retired
`tools/llm_harness` single-shot solve loop; do not use its commands.**
One session = one target. The model receives a markdown brief (retail
ASM, locked signature, writable scope, rules) and tools: `read_file`,
`grep`, `patch` (SEARCH/REPLACE, all-or-nothing), `build`, `diff`,
`equivalence`, `submit`. The harness owns file writes (delta lint gate —
no asm, no register/stack tricks, no extern "C" outside lbl_*, no void*,
no DECOMP_* macros, no new pragmas), verification (byte-exact sibling
baselines + split size + SMT), and acceptance. Acceptance is automatic on
FULL_MATCH / proven EQUIVALENT_MATCH with zero regressions; `submit` is a
checkpoint, not a finish line.

```bash
python3 tools/llm_decomp/run.py solve <target-id> --dry-run
python3 tools/llm_decomp/run.py solve <target-id>
python3 tools/llm_decomp/run.py pipeline --tu kyoshin/CGame
python3 tools/llm_decomp/run.py pipeline --number 4 --dry-run
python3 tools/llm_decomp/run.py reconcile    # after crashes: restore files, abort orphans
python3 tools/llm_decomp/run.py show-config
```

Session types: `match` (default), `type-recovery` (model UnkClass_*
types in headers), `rename` (harness-mediated symrecover),
`tu-cleanup` (byte-identical polish), `size-trim`. Pipelines chain match
sessions per TU with carryover summaries, promote accepted matches to
targets.json, checkpoint-commit per stage, and record completion in
tools/llm_decomp/tu_ledger.json. Config: `llm-decomp.json` (per-session
models/budgets, docs/llm_decomp_design.md §9). Transcripts:
build/llm-decomp/sessions/<target>/<session>/conversation.jsonl. Full
design: docs/llm_decomp_design.md.

## Key paths

| Path | Role |
|------|------|
| `PLAN.md` | Co-op architecture, agent rules, milestones |
| `COOP_IMPLEMENTATION_MAP.md` | Capability graph and co-op implementation handoffs |
| `tools/coop/run.py` | Runner CLI (`symbols`, `behaviour`, `size` subcommands) |
| `tools/coop/lib/object_size.py` | Split `.text` budget check vs `config/<region>/splits.txt` |
| `tools/test/compare_behaviour/` | Retail vs decomp behaviour tests — optional evidence below the bar |
| `tools/symrecover.py` | Symbol recovery: list/show/xref/demangle/rename-plan |
| `tools/symbolrecover/lib/` | symrecover library (parser, MWCC demangle, xref, rename) |
| `tools/coop/targets.json` | Canonical function registry and current target state |
| `tools/coop/targets.schema.json` | Registry data contract |
| `configure.py` | Per-object matching flags and compiler options |
| `tools/coop/hexdiff.py` | Headless instruction-level hex diff (builds, compares, colour-codes, reg-swap detection, register mapping table); modes: `--symbol` diff / `--all` unit table / `--list` symbols / `--brief` verdict / `--asm` disassembly; prints unit compiler config + KB hints; uses `ppc_equivalence` ELF parser |
| `tools/coop/reloc_map.py` | Reloc name-drift detection + repo map miner (`run.py reloc-map diff/mine/show`); suggests the approved source `extern "C" lbl_eu_*` fix |
| `tools/coop/batch-cycle.py` | Mass-cycle multiple targets sequentially with per-target hypothesis/next-change, continues on failure, optional JSON summary |
| `docs/MWCC_REFERENCE.md` | MWCC matching reference — read before matching; **append new patterns/breakthroughs here** |
| `docs/MWCC_KNOWLEDGE_BASE.md` | Agent search protocol and structured-record migration plan |
| `tools/mwcc_kb.py` | Search reference patterns + attempt history; use `--json` for agents |
| `objdiff.json` | Generated; objdiff project config |

## Additional detail

- Full agent orchestration, Dolphin gates, and render invariants: `PLAN.md`
- Per-function identity/state: `tools/coop/targets.json`; use `targets show/status`
