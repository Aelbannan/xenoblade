---
name: xenoblade-decomp
description: >-
  Primary agent workflow for the Xenoblade Chronicles Wii co-op decompilation
  fork. Always read and follow at the start of tasks in this repository when
  decompiling, matching, editing src/kyoshin or configure.py, running the coop
  runner, objdiff, DECOMP_MAP targets, or MWCC FULL_MATCH work.
---

# Xenoblade decompilation

> **Source of truth:** `.claude/skills/xenoblade-decomp/SKILL.md` (Claude Code
> skill). This file is a **mirror** for Cursor use only — edit the `.claude`
> copy and sync this one from it. Do not diverge.

> **Auto-loaded** via root `AGENTS.md` and `.cursor/rules/xenoblade-decomp.mdc` (`alwaysApply: true`).

## Before you edit code

1. Read `PLAN.md` §2 (legal boundaries), §3 (architecture invariants), and §17 (decompilation loop).
2. Read the target entry in `DECOMP_MAP.md` and `tools/coop/targets.json`.
3. For MWCC matching work, read `docs/MWCC_REFERENCE.md` before editing source.
4. Confirm this is a **private/downstream fork** — do not upstream LLM-assisted matching work to `xbret/xenoblade`.

**Current policy:** every target must reach **`FULL_MATCH`** (100% function match when a symbol is set; 100% code + data for whole units). `coop run cycle` exits non-zero until the bar is met.

**Source language:** reconstruction must be **high-level C or C++ only** (MWCC). Express recovered **semantics** — fields, locals, control flow, and normal function calls — not register-level or stack-level implementation detail.

Use retail assembly only as **read-only reference** (objdiff, Ghidra, `build/us/asm/`). Do **not** ship matched functions as inline `asm { }`, `asm void`, standalone `.s` fragments, or other non-C/C++ source in `src/**` or `libs/**`.

**High-level means:** readable code a human would write without looking at disassembly — struct members, parameters, return values, `if`/`else`, loops, and small named helpers. **Not** micro-matching prologue/epilogue shape in source.

## One-time setup

```bash
cp tools/coop/coop.example.json coop.json
# Place retail files at orig/<region>/sys/main.dol and orig/<region>/files/rels/*.rel

python tools/coop/run.py status
python tools/coop/run.py baseline   # sha1 + configure + ninja
```

Region defaults to `us` in `coop.json`. Change with `--config` or edit `region`.

## Pick a target

```bash
python tools/coop/run.py targets list
python tools/coop/run.py targets show <target-id>
```

Prefer `P0` / `P1` tiers unless the user names a specific function. Skip targets with `buildable=no` until source exists — recover via Ghidra first, add `.cpp` to `configure.py`, reconfigure.

Claim symbols in `docs/ownership.csv` before editing (see `PLAN.md` §6).

## Symbol recovery (`tools/symrecover.py`)

After **`FULL_MATCH`** on a function (or when investigating `UnkClass_*` / `func_*` placeholders), run symbol recovery **before** renaming types in source.

```bash
# List unknown placeholder types in the active region
python tools/coop/run.py symbols list --kind UnkClass

# Inspect one type (name or address suffix)
python tools/coop/run.py symbols show 8043C59C
python tools/coop/run.py symbols xref 8043C59C

# Demangle symbols from config/<region>/symbols.txt
python tools/coop/run.py symbols demangle func_80459270__17UnkClass_8043C59CFv

# Plan a rename (checks MWCC mangling length compatibility)
python tools/coop/run.py symbols rename-plan UnkClass_8043C59C CViewRectData --verbose

# Apply symbol-map rename only
python tools/coop/run.py symbols rename-apply UnkClass_8043C59C CViewRectData --dry-run

# Apply full rename: symbols + source + configure + splits + file renames
python tools/coop/run.py symbols rename-all UnkClass_8043C59C CViewRectDataCore --dry-run
python tools/coop/run.py symbols rename-all UnkClass_8043C59C CViewRectDataCore
```

Equivalent standalone CLI: `python tools/symrecover.py <subcommand> …`

**Workflow when a type is understood:**

1. `symbols show` + `symbols xref` — methods, namespaces, split unit, source files.
2. `symbols demangle` on each symbol in the unit — recover method names and signatures.
3. `symbols rename-plan <old> <new>` — confirm mangling-compatible length when possible.
4. Decompile / match all functions in the unit at `FULL_MATCH`.
5. **`symbols rename-all <old> <new>`** — updates `symbols.txt`, source, `configure.py`, `splits.txt`, `ownership.csv`, and renames `UnkClass_*.cpp/.hpp` files (use `--dry-run` first).
6. `python configure.py && ninja` and `coop run diff` every affected symbol.
7. Log the recovered name in `attempts.jsonl` (`hypothesis` / `next_change`).

**Rules:**

- Retail `main.dol` is stripped — `symbols.txt` names are decomp annotations; recovered names must still match MWCC mangling for `FULL_MATCH`.
- Prefer **same-length** renames (`UnkClass_8045F564` → `CLibLayoutRegion`, 17 chars) to avoid re-mangling every symbol. `rename-all` refuses length mismatches unless `--force`.
- `rename-apply` without `--all` only edits symbol maps; use **`rename-all`** (or `rename-apply --all`) for source and build files.
- Headers already named semantically (e.g. `CViewRectData.hpp`) are updated in place; only files **named** `UnkClass_<addr>.cpp` are renamed on disk.
- Extend `KNOWN_NS_PREFIXES` in `tools/symbolrecover/lib/mwcc.py` when you find new namespaces (`cf`, `ml`, `LOD`, `mpfsys`, …).

## Decompilation loop

For each target:

```text
export assembly/symbols/types (Ghidra or objdiff) — **reference only**
→ draft/edit **high-level C or C++** in the owning translation unit
→ python tools/coop/run.py ctx <source.cpp>
→ python tools/coop/run.py cycle <target-id> \
    --hypothesis "..." --next-change "..." --runtime-test ""
→ if static match < 100%: **mandatory** behaviour tests (see below)
→ verify split object size: `coop run size <unit>` (decomp `.text` ≤ retail split budget)
→ if FAIL: inspect objdiff / build/coop-function-diff.json, revise, repeat
→ if PASS: mark target complete; do not edit the same function concurrently
```

### Behaviour comparison (required below `FULL_MATCH`)

Retail vs decomp **object behaviour** is verified with `tools/test/compare_behaviour/`:

```bash
python tools/coop/run.py behaviour audit
python tools/coop/run.py behaviour compare <test-id>
python tools/coop/run.py behaviour compare --all
```

| Static match | Host scenarios required |
|--------------|-------------------------|
| 100% | optional |
| 95–99.9% | ≥ 8 |
| 90–94.9% | ≥ 12 |
| 80–89.9% | ≥ 20 |
| < 80% | ≥ 30 |

**Rules:**

- Any symbol below **100%** objdiff match **must** have a `manifest.json` entry with `host_binary` and enough `run_scenario(...)` cases in `host/<host_binary>.cpp`.
- Each host test defines **`retail_*`** (oracle from asm/Ghidra) and **`decomp_*`** (from your source). Both receive the same inputs; outputs must match.
- Do **not** log `BEHAVIOR_VERIFIED` or treat semantics as settled until `behaviour audit` passes for that test id.
- Log `runtime_test: behaviour:<test-id>` in `attempts.jsonl` when host compare passes.
- Full policy: `tools/test/compare_behaviour/README.md`.

### Split object size (required before `Matching` / `FULL_MATCH`)

Each translation unit has a fixed retail `.text` slice in `config/<region>/splits.txt`. The decompiled object’s **`.text` section** must not exceed that budget — otherwise the unit cannot be linked into `main.dol` at the retail address.

```bash
python tools/coop/run.py size monolib/src/core/CViewRectDataCore
python tools/coop/run.py size --all
```

**Rules:**

- `diff`, `cycle`, and `behaviour compare` print a `size:` line and **exit non-zero** when decomp `.text` exceeds the split budget.
- Behaviour tests can pass while size fails (semantics ≠ codegen fit) — treat size overflow as a blocker for `FULL_MATCH` and `configure.py` `Matching` promotion.
- Retail budget = `splits.txt` `.text end - start`; compared against ELF `.text` in `build/<region>/src/...o` vs retail `build/<region>/obj/...o`.
- Implementation: `tools/coop/lib/object_size.py`.

Manual steps equivalent to `cycle`:

```bash
python tools/coop/run.py ctx src/kyoshin/cf/CfPadTask.cpp
python tools/coop/run.py build kyoshin/cf/CfPadTask
python tools/coop/run.py diff kyoshin/cf/CfPadTask --symbol <mangled-symbol>
python tools/coop/run.py size kyoshin/cf/CfPadTask
```

### decomp.me (optional)

For stubborn **small** functions: generate ctx → open unit in **objdiff** → Create scratch on decomp.me → paste matched code back → `cycle` again.

### Large functions

Decompose into leaf symbols/units first. Each leaf and the parent must still end at **`FULL_MATCH`** before the target is closed.

## Logging and evidence

- Attempts append to `docs/evidence/decomp/attempts.jsonl` (JSONL, one object per line).
- Required fields: `target_id`, `function`, `status`, `instruction_match`, `hypothesis`, `next_change`.
- Below **100%** static match: set `runtime_test` to `behaviour:<test-id>` after `coop run behaviour compare <test-id>` passes.
- After **100%** match, add Dolphin proof when `PLAN.md` requires it (`BEHAVIOR_VERIFIED`).

## MWCC patterns — read and update `docs/MWCC_REFERENCE.md`

- **Before matching:** consult `docs/MWCC_REFERENCE.md` for compiler behavior, repo-proven patterns, and known pitfalls.
- **After a breakthrough:** if you discover a reusable fix (new pattern, pragma/flag combo, struct/layout insight, regalloc trick, reloc naming rule, or confirmed hard cap), **append it to `docs/MWCC_REFERENCE.md`** in the same session — do not leave it only in `attempts.jsonl` or chat.
- **Where to add:**
  - Proven high-level fix → **Patterns that work in this repo** (new numbered subsection) or extend an existing one.
  - Symptom → cause → fix → **Pitfalls and failure modes** tables.
  - Compiler/tooling note → **MWCC compiler behavior** or **decomp.me workflow**.
  - Confirmed policy limit → **When FULL_MATCH may be unrealistic**.
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

## Approved policy exceptions (`PLAN.md` §17.6)

When C++ and decomp.me cannot close the last instruction(s), these are **allowed** if logged in `attempts.jsonl` with `"policy_exception": true`:

| Tool | Use |
|------|-----|
| `DECOMP_PPC_RLWINM` / `DECOMP_PPC_SHL1_U32` in `decomp.h` | MWCC `__rlwinm` builtins (SDK-equivalent); opcode selection e.g. `slwi` vs `rlwinm …,16,30` |
| `DECOMP_ASM_INSN_BEGIN` / `END` + one `asm { }` insn | Single-instruction gap in an otherwise ≥99% function |
| `extern "C" lbl_eu_*` | Reloc names when values match under `functionRelocDiffs=data_value` |
| Goto gate chains | Multi-exit guards (`setSplitLine` pattern) — not for prologue spill order alone |

**Still forbidden:** `register rN`, fake `sp[]` buffers, whole-function asm, standalone `.s`.

## Low-level techniques — do not use in `src/**` / `libs/**`

- inline or standalone **assembly**
- `register` locals, especially names like `r3`, `r4`, `r30`, `r31`, or `asm("rN")` bindings
- `asm { mr …, r1 }` or other inline asm snippets
- `volatile` byte arrays / fake stack buffers to mirror `sp+0xC` retail offsets
- `goto` labels whose only purpose is to duplicate asm branch layout
- parameters or identifiers named after GPRs (`u32* r4`, `register CView* r30`) to force MWCC register allocation
- raw pointer arithmetic and manual field offsets when a struct member or helper would express the same logic
- codegen-forcing macros — `DECOMP_FORCELITERAL` / `DECOMP_FORCEACTIVE` and **`DECOMP_PPC_*`** (`include/decomp.h`) are approved project infrastructure

## Do not

- Commit `orig/`, `main.dol`, RELs, or disc assets
- Call `CGame::wkRender` or full frame update twice for split-screen experiments
- Accept `STRUCTURAL` / `CODE_MATCH` as final state (project policy is `FULL_MATCH`)
- Submit AI-assisted reconstruction upstream
- **Use assembly as decompilation output** — no whole-function `asm { }` / `asm void` / `.s` units; **single-instruction** asm allowed per `PLAN.md` §17.6
- **Micro-manage registers or the stack in source** — use §17.6 intrinsics or logged single-instruction asm when C++ is exhausted

## Key paths

| Path | Role |
|------|------|
| `FORK.md` | Fork-only tooling inventory vs upstream `xbret/xenoblade` |
| `PLAN.md` | Co-op architecture, agent rules, milestones |
| `DECOMP_MAP.md` | Function-level decompilation map |
| `TASKS.md` | Agent checklist — check off at `FULL_MATCH` |
| `tools/coop/run.py` | Runner CLI (`symbols`, `behaviour`, `size` subcommands) |
| `tools/coop/lib/object_size.py` | Split `.text` budget check vs `config/<region>/splits.txt` |
| `tools/test/compare_behaviour/` | Retail vs decomp behaviour tests — **required below FULL_MATCH** |
| `tools/symrecover.py` | Symbol recovery: list/show/xref/demangle/rename-plan |
| `tools/symbolrecover/lib/` | symrecover library (parser, MWCC demangle, xref, rename) |
| `tools/coop/targets.json` | Curated target queue |
| `configure.py` | Per-object matching flags and compiler options |
| `tools/decompctx.py` | Context for decomp.me / compilation |
| `docs/MWCC_REFERENCE.md` | MWCC matching reference — read before matching; **append new patterns/breakthroughs here** |
| `objdiff.json` | Generated; objdiff project config |

## Additional detail

- Full agent orchestration, Dolphin gates, and render invariants: `PLAN.md`
- Per-function addresses and tiers: `DECOMP_MAP.md`
