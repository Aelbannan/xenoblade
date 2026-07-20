# Xenoblade Chronicles Wii — split-screen co-op fork

Private/downstream fork of [xbret/xenoblade](https://github.com/xbret/xenoblade): byte-matching decompilation plus tooling toward **single-instance local split-screen co-op**.

| | |
|---|---|
| **Upstream** | [xbret/xenoblade](https://github.com/xbret/xenoblade) — do **not** upstream LLM-assisted matching or this fork’s co-op tools |
| **Agents** | Start at [`AGENTS.md`](AGENTS.md) → [`.cursor/skills/xenoblade-decomp/SKILL.md`](.cursor/skills/xenoblade-decomp/SKILL.md) |
| **Architecture** | [`PLAN.md`](PLAN.md) |
| **Targets** | [`DECOMP_MAP.md`](DECOMP_MAP.md) · checklist [`TASKS.md`](TASKS.md) |
| **Tool inventory** | [`FORK.md`](FORK.md) |

This repo does **not** ship game assets or retail assembly. You need a legally obtained copy of the game.

### Region hashes (`main.dol`)

| Region | SHA1 |
|:------:|------|
| JP | `a564033aee46988743d8f5e6fdc50a8c65791160` |
| EU | `10d34dbf901e5d6547718176303a6073ee80dda2` |
| US | `214b15173fa3bad23a067476d58d3933ad7037b7` |

The coop runner defaults to **US**. Use another region via `coop.json` or `configure.py --version`.

---

<!-- BEGIN GENERATED PPC_EQUIVALENCE_VERSION -->

- Architecture model: `broadway-ppc32-be-v37`
- Result format: `17`
- Certificate format: `12`

<!-- END GENERATED PPC_EQUIVALENCE_VERSION -->
<!-- BEGIN GENERATED PROOF_STATUS_TABLE -->

| Status | Value |
|---|---|
| `EQUIVALENT` | `equivalent` |
| `NOT_EQUIVALENT` | `not_equivalent` |
| `INCONCLUSIVE_TIMEOUT` | `inconclusive_timeout` |
| `INCONCLUSIVE_UNKNOWN` | `inconclusive_unknown` |
| `INCONCLUSIVE_UNSUPPORTED` | `inconclusive_unsupported` |
| `INCONCLUSIVE_ABSTRACTION` | `inconclusive_abstraction` |
| `INCONCLUSIVE_LAYOUT` | `inconclusive_layout` |
| `INCONCLUSIVE_UNVALIDATED_CALLEE` | `inconclusive_unvalidated_callee` |
| `INCONCLUSIVE_UNMODELED_EXCEPTION` | `inconclusive_unmodeled_exception` |
| `INVALID_INPUT` | `invalid_input` |
| `INTERNAL_ERROR` | `internal_error` |

<!-- END GENERATED PROOF_STATUS_TABLE -->
## Policy (this fork)

- Match bar: **`FULL_MATCH`** (100% instruction + reloc when a symbol is set).
- Source: **high-level C/C++ only** in `src/**` and `libs/**` — no asm / register micro-matching (narrow exceptions in `PLAN.md` §17.6).
- Below 100%: keep matching (or §17.6); optional PPC harness / equivalence — no host dual-oracle tests.
- Split object `.text` must fit the retail slice in `config/<region>/splits.txt`.
- Never commit `orig/`, `main.dol`, RELs, or disc assets.

---

## Quick start

### 1. Dependencies

**macOS**

```sh
brew install ninja
brew install --cask --no-quarantine gcenx/wine/wine-crossover
# After OS upgrades, if Wine is quarantined:
# sudo xattr -rd com.apple.quarantine '/Applications/Wine Crossover.app'
```

**Linux** — install ninja. On x86(_64), [wibo](https://github.com/decompals/wibo) is fetched automatically; other arches need wine.

**Windows** — Python and [ninja](https://github.com/ninja-build/ninja/releases) on `%PATH%`. Prefer native tooling (WSL breaks objdiff file watching).

Also useful: [objdiff](https://github.com/encounter/objdiff), [Dolphin](https://dolphin-emu.org/) (for PPC behaviour tests and gameplay).

### 2. Extract the game

With Dolphin, extract to `orig/<region>` (e.g. `orig/us`). Only these are required:

- `sys/main.dol`
- `files/rels/*.rel`

![](assets/dolphin-extract.png)

### 3. Configure the coop runner

```sh
cp tools/coop/coop.example.json coop.json
# Optional: set "dolphin" to your Dolphin binary for PPC tests
python3 tools/coop/run.py status
python3 tools/coop/run.py baseline   # sha1 + configure + ninja
```

Equivalent without the runner:

```sh
python3 configure.py --version us --map
ninja
```

---

## Everyday workflow

```sh
python3 tools/coop/run.py targets list
python3 tools/coop/run.py cycle <target-id> \
  --hypothesis "..." --next-change "..."

# Optional PPC evidence (when ppc_source is registered):
python3 tools/coop/run.py behaviour ppc <test-id>
python3 tools/coop/run.py behaviour audit      # size budget for registered tests

python3 tools/coop/run.py size <unit>          # .text vs split budget
python3 tools/coop/run.py symbols list         # UnkClass_* placeholders
```

| Subsystem | Entry |
|-----------|--------|
| Coop CLI | `python3 tools/coop/run.py --help` |
| Behaviour + PPC | [`tools/test/compare_behaviour/README.md`](tools/test/compare_behaviour/README.md) |
| PPC equivalence | [`tools/ppc_equivalence/README.md`](tools/ppc_equivalence/README.md) · `python3 tools/coop/run.py equivalence --help` |
| Symbol recovery | `python3 tools/symrecover.py --help` · [`FORK.md`](FORK.md) §4 |
| MWCC patterns | [`docs/MWCC_REFERENCE.md`](docs/MWCC_REFERENCE.md) |
| Attempt log | [`docs/evidence/decomp/attempts.jsonl`](docs/evidence/decomp/attempts.jsonl) |
| Ownership | [`docs/ownership.csv`](docs/ownership.csv) |

Claim a symbol in `docs/ownership.csv` before editing. Check off [`TASKS.md`](TASKS.md) only at `FULL_MATCH`.

---

## Diffing (objdiff)

After a successful build, root `objdiff.json` is ready. Open the project in [objdiff](https://github.com/encounter/objdiff), set **Project directory**, and select an object. Rebuilds track source, headers, `configure.py`, `splits.txt`, and `symbols.txt`.

![](assets/objdiff.png)

This fork’s runner passes `functionRelocDiffs=data_value` (see `coop.json`). Manual diffs:

```sh
python3 tools/coop/run.py diff <unit> --symbol <mangled>
```

---

## Editor / agent setup

- **VS Code / Cursor:** rename `.vscode.example` → `.vscode` for recommended settings.
- **Agents:** follow [`AGENTS.md`](AGENTS.md). The Xenoblade decomp skill and `.cursor/rules/xenoblade-decomp.mdc` apply automatically in this repo.

---

## Contributions & upstream

This is a **downstream** co-op fork. Keep all LLM-assisted reconstruction here.

Upstream [xbret/xenoblade](https://github.com/xbret/xenoblade) does **not** accept LLM-assisted contributions. Follow their [coding style](docs/coding_style_guidelines.md) when touching shared decomp code, and see [`FORK.md`](FORK.md) for what this tree adds beyond upstream.
