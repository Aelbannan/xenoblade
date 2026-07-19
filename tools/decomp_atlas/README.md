# Decomp Atlas

Local browsable catalog for Xenoblade decompilation targets.

Atlas owns **presentation and generated indexes**. The co-op registry
(`tools/coop/targets.json`) and LLM harness remain authoritative for identity,
acceptance, and execution.

Visual concepts are adapted from [Mizuchi’s Decomp Atlas](https://github.com/macabeus/mizuchi)
(MIT — see `LICENSE-MIZUCHI`). The C-only indexer, compiler, Claude runner,
m2c, decomp-permuter, and integrator are **not** ported.

## Non-goals

Atlas does **not**:

- compile generated code
- accept or promote candidates
- write `targets.json` or source files
- claim/release targets
- expose arbitrary shell execution
- run Dolphin

Generated retail-derived content stays under `build/decomp-atlas/` (gitignored).

## Setup

```bash
# Optional: build the React UI (served by the Python server)
cd tools/decomp_atlas && npm install && npm run build && cd ../..

# Index the catalog (fast metadata)
python3 tools/decomp_atlas/run.py index

# Optional: structural similarity + function cloud
python3 tools/decomp_atlas/run.py index --vectors

# Optional: extract C++ / PPC artifacts (slow; per-function warnings OK)
python3 tools/decomp_atlas/run.py index --full --vectors

# Serve API + UI on 127.0.0.1:8765
python3 tools/decomp_atlas/run.py serve

# Optional: allow-listed harness jobs (new / improve only)
python3 tools/decomp_atlas/run.py serve --enable-jobs
```

Co-op wrappers:

```bash
python3 tools/coop/run.py atlas status
python3 tools/coop/run.py atlas index --vectors
python3 tools/coop/run.py atlas serve
```

Dev UI with hot reload (proxies `/api` to `:8765`):

```bash
# terminal 1
python3 tools/decomp_atlas/run.py serve
# terminal 2
cd tools/decomp_atlas && npm run dev
```

## Architecture

```text
targets.json / objdiff.json / attempts.jsonl / harness results
        │
        ▼
  Atlas indexer ──► build/decomp-atlas/<region>/atlas.sqlite
        │
        ▼
  Local Python API ──► React UI
        │
        ├── prompt preview ──► existing LLM harness
        └── optional jobs ──► allow-listed `new` / `improve`
```

## Names

Every function keeps three distinct names — never conflate them:

| Role | Example |
|------|---------|
| Target ID (URLs / DB keys) | `us-804415c4` |
| Display name | `CView::renderView` |
| MWCC symbol | `renderView__5CViewFv` |

## Testing

CI uses a synthetic fixture (no retail objects):

```bash
python -m unittest discover -s tools/decomp_atlas/tests -p 'test_*.py'
```

## Legal

- Package code in this directory is part of the Xenoblade co-op fork.
- UI concepts adapted from Mizuchi remain under the MIT license in `LICENSE-MIZUCHI`.
- Do not commit `build/decomp-atlas/` — it may contain retail-derived assembly.
