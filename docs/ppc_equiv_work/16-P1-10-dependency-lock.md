# P1-10 — Lock dependencies and execution environment by hash

**Priority:** P1/P2  
**Primary owner areas:** packaging, CI

---

## Current state

Exact package versions improve reproducibility, but version pins alone do not guarantee identical artifacts.

## Required implementation

Choose one:

- `pip-tools` with `pip-compile --generate-hashes`;
- `uv.lock` with frozen sync;
- another repository-standard lock format that records artifact hashes.

Example CI install:

```bash
python -m pip install --upgrade pip
python -m pip install --require-hashes -r tools/ppc_equivalence/requirements.lock
```

Pin CI action versions and container images by immutable digest where practical.

## Supply-chain/reproducibility checks

- lock regeneration is explicit and reviewed;
- CI verifies lock is fresh;
- result provenance records lock hash;
- Dolphin binary/container also pinned;
- no network access is required during the actual proof once environment is provisioned.

## Acceptance criteria

- clean environments install identical declared artifacts;
- dependency change invalidates provenance/cache as policy requires;
- lockfile drift fails CI.
