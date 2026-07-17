# P1-07 — Make the Dolphin interpreter corpus an automated gate

**Priority:** P1  
**Primary owner areas:** fixture generator, Dolphin harness, GitHub Actions

---

## Current gap

The documented quality process includes generated DOL payloads executed by Dolphin's interpreter, but the repository's main CI workflow currently runs only fixture freshness, Python self-tests, ConcreteOps differential checks, and CLI help. At the audit snapshot, GitHub also displayed no completed runs for that workflow, so the repository lacked public pass history.

## CI architecture

Use two levels:

### Required PR gate — fast

- dependency/lock verification;
- fixture-generation freshness;
- Python tests including soundness suite;
- ConcreteOps-symbolic differential corpus;
- documentation/version synchronization;
- certificate/schema tests;
- small deterministic Dolphin smoke corpus if runtime is acceptable.

### Scheduled or required specialized gate — full Dolphin

- full generated instruction corpus;
- boundary-heavy vectors;
- randomized seeded corpus;
- interpreter mode explicitly selected;
- emulator version/digest pinned;
- artifacts uploaded.

If full Dolphin runtime is practical, make it a required PR check for changes to decoder/semantics/engine/fixtures. Otherwise, require it before an architecture-model release and block model promotion until it passes.

## Legal/reproducibility constraints

The corpus should use only generated DOL payloads and generated data. Do not require retail game assets in CI. Document all external binary licenses and distribution rules.

Pin:

- Dolphin release or commit;
- container image digest;
- configuration files;
- interpreter/JIT selection;
- fixture-generator version;
- expected-output corpus hash.

## Artifact bundle

Each run should upload:

```text
ppc-equivalence-validation/
  summary.json
  junit.xml
  dolphin-version.txt
  container-digest.txt
  validation-ledger.yaml
  fixture-manifest.json
  generated-dol.sha256
  failures/
    <seed-or-case>/
      request.json
      payload.dol
      expected.json
      actual.json
      disassembly.txt
```

## Branch protection

After the first successful run:

- require the fast equivalence job for all pull requests touching the subsystem;
- require the model-release Dolphin job before changing `ARCHITECTURE_MODEL`;
- add a visible status badge;
- prevent maintainers from advertising an opcode as independently validated unless ledger evidence exists.

## Acceptance criteria

- At least one public successful CI run exists.
- Dolphin corpus is reproducible from a clean checkout without retail assets.
- Failure artifacts are sufficient for local reproduction.
- Model-version changes cannot merge without current independent validation evidence.
