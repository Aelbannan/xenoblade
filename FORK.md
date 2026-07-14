# Downstream fork: tooling and changes since `xbret/xenoblade`

This repository is a **private/downstream** fork of [xbret/xenoblade](https://github.com/xbret/xenoblade) for single-instance split-screen co-op. Upstream remains the byte-matching decompilation project; this document catalogs **everything added or changed for the co-op fork** (tools, tests, docs, agent policy, and reconstruction progress).

**Base:** forked from `xbret/xenoblade` (remote `upstream`).  
**Do not** upstream LLM-assisted matching work or this fork’s co-op tooling.

For day-to-day decomp workflow, start at [`AGENTS.md`](AGENTS.md) → [`.cursor/skills/xenoblade-decomp/SKILL.md`](.cursor/skills/xenoblade-decomp/SKILL.md).

---

## Quick map

| Area | Path | Role |
|------|------|------|
| Co-op architecture | [`PLAN.md`](PLAN.md) | Split-screen design, invariants, milestones, agent roles |
| Decomp targets | [`DECOMP_MAP.md`](DECOMP_MAP.md) | Per-function addresses, symbols, tiers |
| Checklist | [`TASKS.md`](TASKS.md) | Check off at `FULL_MATCH` |
| Coop runner | [`tools/coop/`](tools/coop/) | Build / diff / cycle / size / symbols / behaviour |
| Symbol recovery | [`tools/symrecover.py`](tools/symrecover.py) | `UnkClass_*` list/show/xref/rename |
| Behaviour tests | [`tools/test/compare_behaviour/`](tools/test/compare_behaviour/) | Host + PPC retail-vs-decomp oracles |
| PPC equivalence | [`tools/ppc_equivalence/`](tools/ppc_equivalence/) | Capstone + Z3 semantic equivalence for supported straight-line blocks |
| DOL opcode census | [`tools/dol_opcodes.py`](tools/dol_opcodes.py) | Capstone scan of `main.dol` text sections → unique PPC mnemonics / primary opcodes |
| Reloc postprocess | [`tools/postprocess_reloc_names.py`](tools/postprocess_reloc_names.py) | Rename MWCC `@N` pools → retail `lbl_eu_*` |
| MWCC patterns | [`docs/MWCC_REFERENCE.md`](docs/MWCC_REFERENCE.md) | Living matching reference |
| Evidence | [`docs/evidence/decomp/attempts.jsonl`](docs/evidence/decomp/attempts.jsonl) | Attempt log (JSONL) |
| Ownership | [`docs/ownership.csv`](docs/ownership.csv) | Symbol claim table |
| Agent skill | [`.cursor/skills/xenoblade-decomp/`](.cursor/skills/xenoblade-decomp/) | Auto-loaded decomp workflow |
| Policy macros | [`include/decomp.h`](include/decomp.h) | `DECOMP_PPC_*`, single-insn asm markers |

---

## 1. Agent / Cursor infrastructure

| Path | Purpose |
|------|---------|
| [`AGENTS.md`](AGENTS.md) | Entry point: reading order, quick commands, legal “do not” |
| [`.cursor/rules/xenoblade-decomp.mdc`](.cursor/rules/xenoblade-decomp.mdc) | Always-on: read skill, use `coop run`, `FULL_MATCH` policy |
| [`.cursor/skills/xenoblade-decomp/SKILL.md`](.cursor/skills/xenoblade-decomp/SKILL.md) | Full decomp loop, behaviour rules, symbol recovery, §17.6 exceptions |

**Policy deltas vs upstream:**

- Match bar is **`FULL_MATCH`** (100%) via `coop.json` (`match_policy: full`).
- Reconstruction is **high-level C/C++ only** (no asm / register micro-matching in `src/**` / `libs/**`), with narrow exceptions in `PLAN.md` §17.6.
- Below 100% static match → **mandatory** host behaviour tests + audit.
- Split object **`.text` size** must fit `config/<region>/splits.txt` before `Matching` / `FULL_MATCH`.

---

## 2. Project docs (fork-only)

| Document | Contents |
|----------|----------|
| [`PLAN.md`](PLAN.md) | Co-op mission, legal boundaries, architecture invariants (§3), multi-agent org (§6), contracts, render/HUD/input plan, decomp loop (§17), size/behaviour policy |
| [`DECOMP_MAP.md`](DECOMP_MAP.md) | Critical-path functions for split-screen (addresses, mangled symbols, tiers P0–P3) |
| [`TASKS.md`](TASKS.md) | Agent checklist synced to the map (~106 tracked; progress summarized at top) |
| [`docs/MWCC_REFERENCE.md`](docs/MWCC_REFERENCE.md) | Compiler behaviour, repo-proven patterns, pitfalls — **append breakthroughs here** |
| [`docs/ownership.csv`](docs/ownership.csv) | Who owns which symbol (`CLAIMED` / status) |
| [`docs/evidence/decomp/attempts.jsonl`](docs/evidence/decomp/attempts.jsonl) | Per-cycle hypothesis / match % / `runtime_test` |
| [`FORK.md`](FORK.md) | This catalog |

Upstream docs still present: `docs/coding_style_guidelines.md`, `docs/decompiling_guide.md`. Root [`README.md`](README.md) is fork-oriented (setup + coop workflow).

---

## 3. Coop runner (`tools/coop/`)

Unified CLI for configure, build, objdiff, size budget, attempt logging, symbol recovery, and behaviour tests.

### Setup

```bash
cp tools/coop/coop.example.json coop.json   # set region, dolphin path, match policy
python3 tools/coop/run.py status
python3 tools/coop/run.py baseline          # sha1 + configure + ninja
```

Config knobs (`coop.json` / example): `region` (`us` default), `match_policy`, `default_required_level`, `targets_file`, `attempt_log`, `objdiff_report_args` (includes `functionRelocDiffs=data_value`), optional `dolphin` path.

### Commands

| Command | What it does |
|---------|----------------|
| `status` | Health: region, policy, build dir, dol presence, target counts |
| `baseline` | Verify `main.dol`, configure, full ninja |
| `configure` | `python configure.py` for selected region |
| `ctx <source.cpp>` | Generate decomp.me / context via `tools/decompctx.py` |
| `build <unit>` | Build one translation unit |
| `diff <unit> [--symbol …]` | Build + objdiff report; enforces size budget |
| `size <unit>` / `size --all` | Decomp `.text` ≤ retail split slice |
| `cycle <target-id>` | ctx + build + diff + append `attempts.jsonl`; fails until `FULL_MATCH` |
| `queue [--tier P0]` | Cycle pending targets from `targets.json` |
| `targets list` / `show` | Curated target queue (`tools/coop/targets.json`) |
| `log [--tail N]` | Read attempt log |
| `symbols …` | Wraps `tools/symrecover.py` |
| `behaviour …` | Wraps `tools/test/compare_behaviour/run.py` |
| `equivalence …` | Wraps `tools/ppc_equivalence/run.py` (`decode`, `check-hex`, `check`, `replay`) |
| `opcodes …` | Wraps `tools/dol_opcodes.py` (default: this region's `main.dol`) |

### Library

| Module | Role |
|--------|------|
| `lib/config.py` | Load `coop.json` / yaml |
| `lib/project.py` | Paths, ninja, objdiff-cli |
| `lib/targets.py` | Parse `targets.json` |
| `lib/objdiff_report.py` | Match % / status classification (`FULL_MATCH`, …) |
| `lib/object_size.py` | Split `.text` budget vs `splits.txt` |
| `lib/attempts.py` | JSONL append/read |

---

## 4. Symbol recovery (`tools/symrecover.py`)

Standalone CLI (also `coop run symbols …`) for placeholder types left by stripped retail DOLs.

```bash
python3 tools/symrecover.py list --kind UnkClass
python3 tools/symrecover.py show 8043C59C
python3 tools/symrecover.py xref 8043C59C
python3 tools/symrecover.py demangle 'func_80459270__17UnkClass_8043C59CFv'
python3 tools/symrecover.py rename-plan UnkClass_8043C59C CViewRectData --verbose
python3 tools/symrecover.py rename-all UnkClass_8043C59C CViewRectDataCore --dry-run
```

| Subcommand | Effect |
|------------|--------|
| `list` / `show` / `xref` | Catalog placeholders; methods; splits / source / configure hits |
| `demangle` / `rtti` | MWCC demangle; `__RTTI__` listing |
| `rename-plan` | Preview rename; prefer same-length names for mangling stability |
| `rename-apply` | Symbol-map only |
| `rename-all` | Symbols + source + `configure.py` + `splits.txt` + `UnkClass_*.cpp/.hpp` renames + ownership |
| `compare-regions` | Hit counts across `us` / `eu` / `jp` |

Implementation: `tools/symbolrecover/lib/` (`parser`, `mwcc`, `catalog`, `xref`, `rename`, `rename_source`).

---

## 5. Behaviour comparison + PPC testing

**Primary doc:** [`tools/test/compare_behaviour/README.md`](tools/test/compare_behaviour/README.md)

Four layers on every registered test:

| Layer | Check | Needs Dolphin? |
|-------|-------|----------------|
| **static** | objdiff instruction + reloc match | No |
| **size** | decomp `.text` ≤ split budget | No |
| **host** | Same inputs → retail oracle vs decomp → same outputs (native host binary) | No |
| **ppc** | Minimal DOL: retail + decomp objects, headless Dolphin, GDB stub result buffer | Yes |

```bash
python3 tools/coop/run.py behaviour audit
python3 tools/coop/run.py behaviour compare --all
python3 tools/coop/run.py behaviour compare view-rect-data-clamp
python3 tools/coop/run.py behaviour ppc view-rect-data-clamp
python3 tools/coop/run.py behaviour ppc --all
```

Standalone: `python3 tools/test/compare_behaviour/run.py …`.

### Policy (non-`FULL_MATCH`)

Any symbol below 100% static match **must** have a `manifest.json` entry with `host_binary` and enough `run_scenario(...)` cases:

| Static match | Min host scenarios |
|--------------|-------------------|
| ≥ 100% | 0 (optional) |
| 95–99.9% | 8 |
| 90–94.9% | 12 |
| 80–89.9% | 20 |
| < 80% | 30 |

`audit` enforces coverage + size. Do not log `BEHAVIOR_VERIFIED` until audit passes.

### Layout

```text
tools/test/compare_behaviour/
  manifest.json          # test registry + PPC flags
  run.py                 # list | audit | static | host | ppc | compare
  README.md
  host/*.cpp             # dual oracles (retail_* + decomp_*) + scenarios
  ppc/*.c                # MWCC harnesses, mocks, retail/decomp slices, stubs
  ppc/ldscript.lcf       # minimal DOL layout
  ppc/crt0.c             # entry
  lib/
    ppc_builder.py       # link retail+decomp .o → test.dol (symbol prefixing, stubs)
    ppc_runner.py        # headless Dolphin + GDB stub readback
    gdb_stub.py          # Dolphin GDB protocol
    host_runner.py       # compile/run host tests
    static_compare.py    # objdiff wrapper
    audit.py / policy.py / manifest.py / map_lookup.py / paths.py
```

### How PPC tests work

1. Build prerequisites: `ninja` / `coop run baseline` so retail (`build/<region>/obj/…`) and decomp (`build/<region>/src/…`) objects exist.
2. `ppc_builder.py` compiles the harness with MWCC (via `wibo`), prefixes retail/decomp symbols to avoid clashes, optionally injects `ppc/stubs/` and semantic **slices** when full `.o` link fails (`mwldeppc` undefined refs), links with `ldscript.lcf` → DOL.
3. `ppc_runner.py` launches Dolphin `--batch --exec <test.dol>` with an isolated user folder (`General.GDBPort = 2160`).
4. Harness writes a result struct (`BEHAVIOUR_RESULT_MAGIC = 0xBEEFCAFE`); runner reads it over the GDB stub.
5. If Dolphin is missing, PPC is **SKIP** (host/static remain authoritative).

Configure Dolphin path via `"dolphin"` in `coop.json` or `DOLPHIN` env.

### Registered tests (manifest)

| id | Unit | Host | PPC |
|----|------|------|-----|
| `game-set-view-rect` | `CGame` | yes | no — large undefined set |
| `game-wk-standby-login` | `CGame` | yes | no — same |
| `view-rect-data-clamp` | `CViewRectDataCore` | yes | **yes** |
| `view-rect-data-store` | `CViewRectDataCore` | yes | **yes** |
| `view-rect-data-init` | `CViewRectDataCore` | yes | **yes** |
| `view-set-current-ring` | `CView` | yes | no — stack snapshot / link limits |
| `view-attach-render-work` | `CView` | yes | no |
| `cview-get-current-view` | `CView` | yes | **yes** (lbl oracle slices) |
| `cview-get-split-line` | `CView` | yes | **yes** (semantic slices) |
| `cview-set-split-line` | `CView` | yes | **yes** |
| `cview-set-disp` | `CView` | yes | **yes** |
| `cview-wkupdate-gate` | `CView` | yes | no |
| `cviewroot-getview` | `CViewRoot` | yes | **yes** (semantic slices) |
| `mtrand-float-rng` | `MTRand` | yes | no (not wired; static host lock) |
| `mtrand-integer-rng` | `MTRand` | yes | **yes** |
| `mtrand-getinstance` | `MTRand` | yes | **yes** (BSS stub) |
| `cfpadtask-update` | `CfPadTask` | yes | no — WPAD/KPAD externs |
| `cfpadtask-updatecfdapdata` | `CfPadTask` | yes | no |
| `proc-pss-create-view` | `CProc` | yes | no |

**PPC techniques used when full `.o` won’t link:**

- **Semantic slices** — thin C files compiling only the retail/decomp function body under test (`cview_*_retail.c` / `*_decomp.c`, `cviewroot_ppc_*.c`).
- **Mocks / stubs** — `cview_mock.*`, `cviewroot_mock.*`, `ppc/stubs/monolib_src_*`.
- **Symbol redefine** — e.g. MTRand retail `@LOCAL@…` → `rb_mtrand_singleton` via `objcopy --redefine-sym`.

## 5.1 PPC semantic equivalence check

[`tools/ppc_equivalence/`](tools/ppc_equivalence/) implements the first
sound-by-default vertical slice from the PPC equivalence-checker plan:

- Capstone decoding cross-checked against direct PPC field extraction;
- shared concrete and symbolic integer semantics;
- explicit GPR, CR, XER, LR, and CTR contracts;
- Z3 `equivalent` / `not_equivalent` / `inconclusive` results;
- JSON proof metadata, SMT-LIB export, and replayable counterexamples;
- unsupported opcodes are inconclusive, never no-ops.
- `coop run equivalence check*` defaults to the `ppc-eabi` function-boundary
  contract; `--contract strict` and manual `--observe` remain available.

It is evidence in addition to objdiff and behaviour testing. It does not relax
the fork's `FULL_MATCH` policy. Full scope, installation, supported opcodes,
and examples are in its [README](tools/ppc_equivalence/README.md).

```bash
python3 -m pip install -r tools/ppc_equivalence/requirements.txt
python3 tools/coop/run.py equivalence check-hex \
  --original 5463103a --candidate 1c630004
python3 -m unittest discover -s tools/ppc_equivalence/tests -v
```

---

## 6. Object / reloc postprocessing

MWCC emits TU-local `@N` float/double pools; retail uses stable `lbl_eu_*` names. objdiff CLI reports need aligned reloc **names** when instruction bytes already match (`PLAN.md` §17.6 / `docs/MWCC_REFERENCE.md`).

| Tool | Role |
|------|------|
| [`tools/postprocess_reloc_names.py`](tools/postprocess_reloc_names.py) | Per-unit rules: pool content → retail name; exact renames; optional `.sdata2` unsigned int-to-double magic patch |
| [`tools/postprocess_mtrand_object.py`](tools/postprocess_mtrand_object.py) | Thin wrapper → `postprocess_reloc_names.py` |
| [`tools/patch_mtrand_sdata2.py`](tools/patch_mtrand_sdata2.py) | Legacy / focused `.sdata2` low-word patch (`0` → `0x80000000`) for MTRand |

Wired from the build for units that need it (notably `MTRand.o`). Prefer extending `UNIT_RULES` in `postprocess_reloc_names.py` over one-off scripts.

---

## 7. Match-policy helpers (`include/decomp.h`)

Fork extensions on top of upstream force-active/literal macros:

| Macro | Purpose |
|-------|---------|
| `DECOMP_PPC_RLWINM` / `DECOMP_PPC_SHL1_U32` | MWCC `__rlwinm` builtins (§17.6) — count as high-level C |
| `DECOMP_ASM_INSN_BEGIN` / `END` | Markers for **single-instruction** asm carve-out only; log `policy_exception` in `attempts.jsonl` |

`NONMATCHING` / `COMPAT_ANY` builds expand PPC macros to portable shifts and no-op asm markers.

---

## 8. Source / reconstruction progress (fork work)

Not exhaustive — highlights of co-op-critical reconstruction beyond upstream:

### New / expanded monolib view stack

| Path | Notes |
|------|-------|
| `libs/monolib/src/core/CView.cpp` (+ `CView.hpp`) | View ring, split line, disp, setCurrent, wkUpdate gates, … |
| `libs/monolib/src/core/CViewRoot.cpp` (+ headers) | `getView` reslist walk |
| `libs/monolib/src/core/CViewRectDataCore.cpp` (+ `CViewRectData.hpp`) | Clamp / store / init (size-budget sensitive) |
| `libs/monolib/include/monolib/core/CViewFrame.hpp` | Split-frame related types |
| Related: `CProc.cpp`, `CVec4.hpp`, `CRect16`, `MTRand.*`, `CDeviceGX.cpp` | Matching / layout / RNG support |

### Game / input units touched

| Path | Notes |
|------|-------|
| `src/kyoshin/CGame.cpp` / `.hpp` | View rect, wkStandbyLogin, wkUpdate/wkRender investigation |
| `src/kyoshin/cf/CfPadTask.cpp` | Stick normalize, pad data, turbo/dpad deadzone |
| `src/kyoshin/plugin/plugin*.cpp` | Supporting edits as needed for link/match |
| `src/kyoshin/coop/PresentationGate.{hpp,cpp}` | Fork-only `ShouldRenderSplitScreen` (SDA E24/E28 + pause/HBM/modal); not in `configure.py` until split renderer links it |
| `DISCOVERY.md` + `docs/evidence/decomp/sda_flags_80663E24_28.json` | Presentation / streaming / camera flag dictionary from object parse |

### Build config

- `configure.py` — objects flipped / added for new TUs; compiler flags as matching requires.
- Region workflows oriented around **US** (`coop.json` default) while preserving upstream multi-region configure.

Track completed symbols in [`TASKS.md`](TASKS.md) and hypotheses in [`attempts.jsonl`](docs/evidence/decomp/attempts.jsonl).

---

## 9. What is intentionally unchanged / not forked yet

- Upstream **build pipeline** (`configure.py` / ninja / dtk / MWCC / objdiff) — fork wraps it; does not replace it.
- No committed `orig/`, `main.dol`, RELs, or disc assets.
- Co-op **gameplay/render patches** and optional Dolphin fork described in `PLAN.md` are roadmap; this catalog focuses on decomp tooling + matching infrastructure already in-tree.
- Upstream contribution policy remains: **no LLM-assisted PRs** to `xbret/xenoblade`.

---

## 10. Typical agent loop (recap)

```text
claim symbol in docs/ownership.csv
→ Ghidra/objdiff (reference only)
→ high-level C++ in owning TU
→ python3 tools/coop/run.py cycle <target-id> --hypothesis "..." --next-change "..."
→ if match < 100%:
     extend tools/test/compare_behaviour/ host (+ ppc_source if linkable)
     python3 tools/coop/run.py behaviour compare <test-id>
     python3 tools/coop/run.py behaviour audit
→ python3 tools/coop/run.py size <unit>
→ on FULL_MATCH: check TASKS.md; optional rename-all; flip Matching in configure.py
→ append reusable MWCC insight to docs/MWCC_REFERENCE.md
```

---

## 11. Keeping this document current

When you add:

- a new coop CLI subcommand → update §3  
- a behaviour / PPC test → update §5 table and `compare_behaviour/README.md`  
- a postprocess rule or tool → update §6  
- a major policy change → update §1 and `AGENTS.md` / skill  

Prefer linking into `README.md` sections inside each tool directory; keep **this file** as the fork-wide inventory.
