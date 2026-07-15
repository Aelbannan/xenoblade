# Behaviour comparison: retail vs decompiled

Compare **retail split objects** (`build/<region>/obj/...`) with **decompiled objects** (`build/<region>/src/...`).

Fork-wide tooling context: [`FORK.md`](../../../FORK.md) §5.

| Layer | What it checks | Dolphin? |
|-------|----------------|----------|
| **static** | objdiff instruction + relocation match | No |
| **size** | decomp `.text` ≤ retail split budget (`splits.txt`) | No |
| **ppc** | Real PPC retail vs decomp objects (when `ppc_source` set) | Yes (headless) |
| **audit** | Registered tests: size budget (PPC optional) | No |

Host dual-oracle binaries (`host/*.cpp`) were removed — they were mostly tautological shared models and are no longer part of policy.

## Quick start

```bash
python tools/coop/run.py behaviour audit
python tools/coop/run.py behaviour compare --all
python tools/coop/run.py behaviour compare view-rect-data-clamp
python tools/coop/run.py behaviour ppc view-rect-data-clamp
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

## Policy

Acceptance remains **`FULL_MATCH`** (objdiff) + split-size fit. Below 100%, prefer:

1. Continue matching / §17.6 exceptions (`insn_patches`, narrow asm) when co-op needs the symbol.
2. Optional **PPC** harness when the unit links (`ppc_source` in `manifest.json`).
3. Optional Capstone+Z3 checks via `tools/ppc_equivalence/` for supported blocks.

`behaviour audit` only fails on split-size overflow for registered tests — it does **not** require host scenario counts.

### Agent workflow

```text
coop run cycle <target>     # static match
→ verify size: coop run size <unit>
→ optional: behaviour ppc <test-id> when ppc_source exists
→ log attempts.jsonl
```

## Adding a PPC test

1. Add entry to `manifest.json` (`id`, `unit`, `symbol`, optional `ppc_source` / `ppc_stubs`).
   Use `ppc_cpu_core: 0` only for an ISA oracle that must run through Dolphin's
   interpreter; ordinary retail/decomp tests should use Dolphin's default JIT.
2. Create harness under `ppc/` (see existing slices/mocks).
3. `python tools/coop/run.py behaviour ppc <id>`
4. `python tools/coop/run.py behaviour compare <id>`

## Relation to objdiff

- **static** = same as `coop run diff` (retail `.o` vs your `.o`).
- At **100%**, static match implies identical behaviour for that function.
- Below **100%**, PPC (when available) is real dual-object evidence; static/size alone do not prove semantics.

## PPC runtime harness (headless Dolphin)

For tests with `ppc_source` in `manifest.json`, the runner builds a minimal test DOL
(retail + decomp `.o` with prefixed symbols + MWCC harness) and runs it **headless**:

```bash
python tools/coop/run.py behaviour ppc view-rect-data-clamp
python tools/coop/run.py behaviour compare view-rect-data-clamp   # static + ppc
python tools/coop/run.py behaviour ppc --all                      # all ppc_source tests
```

Requirements:

- `ninja` / `coop run baseline` so retail and decomp `.o` files exist
- Dolphin on `PATH`, or set `"dolphin": "/path/to/Dolphin"` in `coop.json` (or `DOLPHIN` env)
- Dolphin runs with `--batch --exec <test.dol>` (no GUI)
- Results are read back via Dolphin's GDB stub (`General.GDBPort = 2160` in an isolated user folder)

If Dolphin is missing, PPC build still runs and the step is reported as **SKIP** (static/size remain authoritative).

### Broadway equivalence fixtures (closed loop)

The equivalence checker's supported phase-1/phase-2 families share one corpus
with the Python model:

```bash
python tools/ppc_equivalence/gen_fixture_blob.py
python tools/coop/run.py equivalence differential
python tools/coop/run.py behaviour ppc ppc-equivalence-fixtures
```

`ppc-equivalence-fixtures` builds a generic DOL that loads each fixture’s
GPR/CR/XER/CTR state, copies instruction words into a scratch buffer, runs them
under Dolphin’s PowerPC interpreter (`CPUCore = 0`), and compares the same
expected result/CR/XER/(memory) values that Python `ConcreteOps` checks. The
interpreter is intentional: this path has found ARM64-JIT differences in
CR0’s SO bit.

Edit cases only in `tools/ppc_equivalence/fixtures/corpus.py`, then regenerate.
A passing Dolphin run reports `passed: 40 failed: 0`. Failures include actual
and expected result/CR/XER triples.
