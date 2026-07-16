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

**Current policy:** every target must reach **`EQUIVALENT_MATCH`** (function fuzzy ≥ 50% **and** `ppc_equivalence` proves `EQUIVALENT` under effect-aware `auto`—`ppc-eabi` or stronger—**and** split-size fit) or **`FULL_MATCH`** (100% static **and** split-size fit). Both are equal-tier acceptance outcomes. `coop run cycle` / `diff` probe equivalence automatically when fuzzy is in `[50, 100)`. Unit-level (no symbol) still requires 100% code + data.

**Source language:** reconstruction must be **high-level C or C++ only** (MWCC). Express recovered **semantics** — fields, locals, control flow, and normal function calls — not register-level or stack-level implementation detail.

Use retail assembly only as **read-only reference** (objdiff, Ghidra, `build/us/asm/`). Do **not** ship matched functions as inline `asm { }`, `asm void`, standalone `.s` fragments, or other non-C/C++ source in `src/**` or `libs/**`.

**High-level means:** readable code a human would write without looking at disassembly — struct members, parameters, return values, `if`/`else`, loops, and small named helpers. **Not** micro-matching prologue/epilogue shape in source.

## One-time setup

```bash
cp tools/coop/coop.example.json coop.json
# Place retail files at orig/<region>/sys/main.dol and orig/<region>/files/rels/*.rel

python3 tools/coop/run.py status
python3 tools/coop/run.py baseline   # sha1 + configure + ninja
```

Region defaults to `us` in `coop.json`. Change with `--config` or edit `region`.

## Pick a target

```bash
python3 tools/coop/run.py targets list
python3 tools/coop/run.py targets show <target-id>
python3 tools/coop/run.py targets sync-calls
```

Prefer `P0` / `P1` tiers unless the user names a specific function. Skip targets with `buildable=no` until source exists — recover via Ghidra first, add `.cpp` to `configure.py`, reconfigure.

For bottom-up matching across the complete symbol catalog, use the retail call
graph. `leaf` requires no direct, unresolved, or indirect calls;
`callees-accepted` requires at least one direct callee and every callee at
`EQUIVALENT_MATCH` or stronger; `ready` is their union:

```bash
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
→ python3 tools/coop/run.py cycle <target-id> \
    --hypothesis "..." --next-change "..." --runtime-test ""
→ verify split object size: `coop run size <unit>` (decomp `.text` ≤ retail split budget)
→ optional: `behaviour ppc <test-id>` when a PPC harness exists
→ if FAIL: inspect objdiff / build/coop-function-diff.json, revise, repeat
→ if PASS: `cycle` persists the accepted state in `targets.json`; release the claim and do not edit the same function concurrently
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
defaults function checks to effect-aware `--contract auto`. Use
`--contract strict` for all modeled state or manual `--observe` for the actual
live-outs of an internal basic block.

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
- Accept `STRUCTURAL` / `CODE_MATCH` / `HIGH_MATCH` as final state (policy is `EQUIVALENT_MATCH`)
- Submit AI-assisted reconstruction upstream
- **Use assembly as decompilation output** — no whole-function `asm { }` / `asm void` / `.s` units; **single-instruction** asm allowed per `PLAN.md` §17.6
- **Micro-manage registers or the stack in source** — use §17.6 intrinsics or logged single-instruction asm when C++ is exhausted

## Key paths

| Path | Role |
|------|------|
| `FORK.md` | Fork-only tooling inventory vs upstream `xbret/xenoblade` |
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
| `tools/decompctx.py` | Context for decomp.me / compilation |
| `docs/MWCC_REFERENCE.md` | MWCC matching reference — read before matching; **append new patterns/breakthroughs here** |
| `docs/MWCC_KNOWLEDGE_BASE.md` | Agent search protocol and structured-record migration plan |
| `tools/mwcc_kb.py` | Search reference patterns + attempt history; use `--json` for agents |
| `objdiff.json` | Generated; objdiff project config |

## Additional detail

- Full agent orchestration, Dolphin gates, and render invariants: `PLAN.md`
- Per-function identity/state: `tools/coop/targets.json`; use `targets show/status`
