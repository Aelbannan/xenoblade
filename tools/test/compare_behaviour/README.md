# Behaviour comparison: retail vs decompiled

Compare **retail split objects** (`build/<region>/obj/...`) with **decompiled objects** (`build/<region>/src/...`).

| Layer | What it checks | Dolphin? |
|-------|----------------|----------|
| **static** | objdiff instruction + relocation match | No |
| **size** | decomp `.text` ≤ retail split budget (`splits.txt`) | No |
| **host** | Same inputs → retail oracle vs decomp → same outputs | No |
| **ppc** | Real PPC retail vs decomp objects (when `ppc_source` set) | Yes (headless) |
| **audit** | Non-`FULL_MATCH` entries: host scenarios + size budget | No |

## Quick start

```bash
python tools/coop/run.py behaviour audit
python tools/coop/run.py behaviour compare --all
python tools/coop/run.py behaviour compare view-rect-data-clamp
```

Standalone:

```bash
python tools/test/compare_behaviour/run.py audit
python tools/test/compare_behaviour/run.py compare --all
```

Requires `python tools/coop/run.py baseline` (or `ninja`) so retail and decomp `.o` files exist.

## Split object size

Decompiled `.text` must fit the retail split slice in `config/<region>/splits.txt`:

```bash
python tools/coop/run.py size monolib/src/core/CViewRectDataCore
python tools/coop/run.py size --all
```

`diff`, `cycle`, and `behaviour compare` also print a size line. `decomp .text` larger than the split budget exits non-zero. Behaviour tests may pass while size fails — treat overflow as a blocker for `FULL_MATCH` and `configure.py` `Matching` promotion.

## Policy: non-`FULL_MATCH` objects

**Any function below 100% static match MUST have a host test** registered in `manifest.json`.

Minimum `run_scenario(...)` count in `host/<host_binary>.cpp`:

| Static match | Min scenarios |
|--------------|---------------|
| ≥ 100% | 0 (host optional) |
| 95–99.9% | 8 |
| 90–94.9% | 12 |
| 80–89.9% | 20 |
| < 80% | 30 |

`compare` and `audit` enforce host scenario counts. `audit` also fails when decomp `.text` exceeds the split budget. Agents may not log `BEHAVIOR_VERIFIED` or stop semantic iteration on a below-`FULL_MATCH` symbol until `audit` passes for that test id.

### Agent workflow

```text
coop run cycle <target>     # static match
→ if match < 100%:
    add/extend host/<name>.cpp (retail oracle from asm + decomp from src/)
    register manifest.json
    coop run behaviour compare <test-id>
    coop run behaviour audit
→ log attempts.jsonl with runtime_test: behaviour:<test-id>
```

## Adding a test

1. Add entry to `manifest.json` (`id`, `unit`, `symbol`, `host_binary`).
2. Create `host/<host_binary>.cpp`:
   - `retail_*` — semantics from retail asm / Ghidra (not from decomp source)
   - `decomp_*` — from `src/` / `libs/` (keep in sync when editing)
   - `run_scenario(name, ...)` for each case; meet minimum count for match tier
3. `python tools/coop/run.py behaviour compare <id>`
4. `python tools/coop/run.py behaviour audit`

## Registered tests

| id | Unit | Purpose |
|----|------|---------|
| `game-set-view-rect` | `kyoshin/CGame` | FULL_MATCH sanity baseline |
| `view-rect-data-clamp` | `CViewRectDataCore` | Clamp path (~88% static) |
| `view-rect-data-store` | `CViewRectDataCore` | Max-size store |
| `view-rect-data-init` | `CViewRectDataCore` | Render-mode init |
| `game-wk-standby-login` | `CGame` | wkStandbyLogin rect/vec4 helpers (~99.5%) |
| `view-set-current-ring` | `CView` | setCurrent ring semantics (asm→C++ lock) |
| `mtrand-float-rng` | `MTRand` | randFloat BE int-to-double oracle (~89.6%) |
| `mtrand-integer-rng` | `MTRand` | Integer RNG sequence |
| `mtrand-getinstance` | `MTRand` | getInstance singleton init (~99.8%) |
| `cfpadtask-update` | `CfPadTask` | update stick normalize + pad gate (~99.6%) |
| `cfpadtask-updatecfdapdata` | `CfPadTask` | updateCfPadData deadzone/dpad/turbo |
| `cview-wkupdate-gate` | `CView` | wkUpdate split-frame gate logic |
| `cviewroot-getview` | `CViewRoot` | getView reslist walk (FULL_MATCH guard) |

PPC harnesses exist for rows marked **PPC** in the eligibility table above (`view-rect-data-*`, `mtrand-getinstance`, `mtrand-integer-rng`).

## Relation to objdiff

- **static** = same as `coop run diff` (retail `.o` vs your `.o`).
- At **100%**, static match implies identical behaviour; host tests are optional regression guards.
- Below **100%**, host tests are **mandatory** evidence that retail and decomp semantics agree on documented inputs.

## PPC runtime harness (headless Dolphin)

For tests with `ppc_source` in `manifest.json`, the runner builds a minimal test DOL
(retail + decomp `.o` with prefixed symbols + MWCC harness) and runs it **headless**:

```bash
python tools/coop/run.py behaviour ppc view-rect-data-clamp
python tools/coop/run.py behaviour compare view-rect-data-clamp   # static + host + ppc
python tools/coop/run.py behaviour ppc --all                      # all ppc_source tests
```

### PPC eligibility (2026-07-13)

| Test | PPC | Notes |
|------|-----|-------|
| `view-rect-data-clamp` | yes | 32 scenarios |
| `view-rect-data-store` | yes | 11 scenarios |
| `view-rect-data-init` | yes | 8 scenarios |
| `mtrand-getinstance` | yes | retail BSS stub via `ppc/stubs/monolib_src_math_MTRand.c` |
| `mtrand-integer-rng` | yes | `srand` + `rand31` on stack objects |
| `mtrand-float-rng` | no | `randFloat` codegen diverges (~89.6% static) |
| `game-set-view-rect` | no | `CGame.o` — 64 undefined symbols |
| `game-wk-standby-login` | no | same `CGame.o` unit |
| `view-set-current-ring` | no | `CView.o` retail — 80 undefined symbols |
| `cview-get-current-view` | yes | 11 scenarios; lbl oracle slices (`cview_get_current_view_*.c`) — full `CView.o` link crashes mwldeppc |
| `cview-get-split-line` | no | trimmed `CView.o` crashes mwldeppc |
| `cview-set-split-line` | no | same |
| `cview-set-disp` | no | same |
| `cview-wkupdate-gate` | no | `CView.o` — needs full object + extern stubs |
| `cviewroot-getview` | yes | 13 scenarios; semantic retail/decomp slices (`cviewroot_ppc_*.c`) — trimmed `.o` crashes mwldeppc |
| `cfpadtask-update` | no | `CfPadTask.o` — 52+ WPAD/KPAD/game symbols |
| `cfpadtask-updatecfdapdata` | no | same `CfPadTask.o` unit |

Units with only **one** undefined symbol (`getRenderModeObj`) or **retail BSS stubs** (MTRand) link cleanly. Large game/lib slices need a stub layer not yet implemented.

Retail `MTRand.o` references `@LOCAL@…instance_806561E0` externally; the builder renames that UND to `rb_mtrand_singleton` via `objcopy --redefine-sym` on the prefixed retail object.

Requirements:

- `ninja` / `coop run baseline` so retail and decomp `.o` files exist
- Dolphin on `PATH`, or set `"dolphin": "/path/to/Dolphin"` in `coop.json` (or `DOLPHIN` env)
- Dolphin runs with `--batch --exec <test.dol>` (no GUI)
- Results are read back via Dolphin's GDB stub (`General.GDBPort = 2160` in an isolated user folder)

If Dolphin is missing, PPC build still runs and the step is reported as **SKIP** (host/objdiff
checks remain authoritative).
