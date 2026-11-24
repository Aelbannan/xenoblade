# Land gate (`tools/coop/land`)

The land gate is the only judge for changes to gated files (`src/`, `include/`, `libs/`, `config/`
sources/configs and `configure.py`). It measures the whole tree before and after a change, rejects any
regression, records accepted file states in a ledger, and the pre-commit hook refuses to commit a gated
file whose staged content the gate never passed.

Only the orchestrator runs it: `check` / `apply` / `adopt` require `XB_LAND_OWNER=1`. Matching agents
leave their edit in the working tree and report `DONE` / `BLOCKED`; the orchestrator harvests the edit
into a plan patch and lands it.

## Commands

```bash
python -m tools.coop.land pack <unit> <symbol> [--task T]     # context brief for one function (agents read this)
XB_LAND_OWNER=1 python -m tools.coop.land check [--only F…]   # judge pending tree edits, ledger on PASS
XB_LAND_OWNER=1 python -m tools.coop.land apply Q.multi --log L   # land a queue of plans, bisecting failures
python -m tools.coop.land verify-staged                       # pre-commit hook: staged gated files are ledgered
```

Queue line (`.multi`): `TASK<TAB>RENAMES<TAB>FLAGS<TAB>PRESTEP<TAB>ALLOW`, patch at
`/tmp/xb/plans/TASK.patch` (an empty patch is fine for prestep-only plans). A prestep script declares
the files it edits with `# TOUCHES:` and the symbol addresses with `# SYMS:`, and records its evidence
in a comment.

## What one measurement is

A locked full build (`all_source progress`) plus the final link (`link_gate.run_link("us")`, under the
same `hexdiff.lock`), then a whole-tree snapshot:

- per-function marks from `hexdiff --all --json --no-build --no-baseline` (cached per unit in
  `tools/coop/land/state/fn_cache.json`, keyed by the decomp/retail object shas plus a global key over
  symbols/splits/objdiff.json/hexdiff/postprocess/`tools/coop/lib`; errored units are never cached);
- per-section identical data bytes and extab pass/fail;
- per-unit and per-unpaired-header hard counts: faithful HARD categories plus `bans.py` counters;
- link result (missing / multiply-defined names).

Snapshots are stamped with the ban-counter set (`ban_keys`); a baseline taken with another counter set is
re-measured rather than reused.

## Rejections

- any function that was ✓ loses ✓ (rename-mapped);
- a whole-tree per-section drop in identical data bytes;
- extab passing → failing; a build failure; a new hexdiff error or missing unit;
- a link regression (new missing / multiply-defined name, or a rise in either count);
- a rise in any hard faithful category or ban counter, per unit or unpaired header.

Ban counters (`tools/coop/land/bans.py`, counted with comments and strings masked): `asm`, codegen
`pragma`, `#line`, `uintptr_t`, intrinsics, hand vtables/RTTI, pre-mangled identifiers, byte blobs,
and the steering shapes found in wave-1 review:

| counter | shape |
|---|---|
| `scalar_ptr_alias` | `const u32* p = &self->flags;` — re-reading one scalar through a local pointer |
| `cast_addr_alias` | `Vec* p = (Vec*)&tmp;` — a local seen through another type via a pointer |
| `addr_extern` | `extern T Name_8066B550;` — data under an address-suffixed name (`lbl_` is faithful `label`) |
| `single_use_inline` | new file-local `static inline` helper with exactly one call site |

The ratchet is "no rise", so pre-existing hits are tolerated until cleaned up; new ones are rejected.

## Allowances (fifth queue column)

- `unit hard:cat +N -- reason` — a reviewed, documented rise (e.g. an approved regression revert);
- `SRC merge:DST 0 -- reason` — a TU merge: SRC's functions are checked in DST and DST's hard
  baseline is the sum of both.

Every allowance needs a reason after `--`.

## Review still matters

The gate catches mechanical regressions; it cannot judge whether a shape is what the original
programmer wrote. Before queueing a harvested patch, read it and hold anything that steers the
compiler (see the pack rules in `tools/coop/land/pack.py`), rewrites a known SDK/middleware idiom, or
reorders against a known original. Held patches go to `/tmp/xb/plans/held/` with a one-line reason.

## Waves and mechanical censuses

`python -m tools.coop.land wave {census,select,prep,wait,collect,record,queue}` runs a low-touch
agent wave: register-allocation-only residuals are never selected, `collect` restores the tree,
writes patches and auto-holds anything that raises a counter. Mechanical fix candidates come from
`tools/coop/census_fixes.py {splits,sigs}` and `tools/coop/census_unit_rules.py`. The orchestrator
workflow is in `CLAUDE.md`.
The cheap-orchestrator steps (`wave launch|manage|check-plan|check-review|decide`,
`tools/coop/land/wave_ops.py`) exit non-zero with `ERROR:` lines so a DeepSeek orchestrator only
follows exit codes; see `docs/orchestrator_deepseek.md`.
