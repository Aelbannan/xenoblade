# P1-08 — Add complete proof provenance

**Priority:** P1  
**Primary owner areas:** result schema, certificate builder, CLI, cache

---

## Required provenance

Every durable proof result should record:

### Source/tool identity

- Git commit SHA;
- dirty working-tree Boolean;
- deterministic SHA-256 of relevant engine source tree;
- architecture model;
- result format;
- certificate version.

### Runtime identity

- Python version;
- OS/platform/architecture;
- Z3 version;
- Capstone version;
- dependency lock hash;
- container image digest when available.

### Proof request identity

- normalized CLI/API arguments;
- contract and live-outs;
- limits and deadline;
- memory/environment profile;
- original/candidate base addresses;
- original/candidate bytes;
- relocation metadata;
- local symbol identity;
- callee premises.

### Validation identity

- validation-ledger hash;
- Dolphin version/commit;
- independent corpus hash;
- date of validation;
- exact opcode evidence used to assign confidence tier.

## Deterministic engine-tree hash

Hash only declared trust-boundary inputs, with normalized paths and bytes:

```python
def hash_engine_tree(repo_root: Path) -> str:
    paths = sorted(
        list((repo_root / "tools/ppc_equivalence").rglob("*.py"))
        + [repo_root / "tools/ppc_equivalence/requirements.lock"]
        + [repo_root / "tools/ppc_equivalence/validation_ledger.yaml"]
    )

    digest = hashlib.sha256()
    for path in paths:
        relative = path.relative_to(repo_root).as_posix().encode("utf-8")
        content = path.read_bytes()
        digest.update(len(relative).to_bytes(4, "big"))
        digest.update(relative)
        digest.update(len(content).to_bytes(8, "big"))
        digest.update(content)
    return digest.hexdigest()
```

Document the exact included paths. A source file that can affect semantics must not be omitted.

## Canonical proof-request hash

Use canonical JSON:

```python
def canonical_json_sha256(value: object) -> str:
    payload = json.dumps(
        value,
        sort_keys=True,
        separators=(",", ":"),
        ensure_ascii=True,
        allow_nan=False,
    ).encode("utf-8")
    return hashlib.sha256(payload).hexdigest()
```

Avoid hashes of Python `repr`, unordered sets, absolute temporary paths, or Z3 pretty-printing.

## Dirty trees

Local exploratory proofs may run on dirty trees, but default automatic acceptance should reject them unless an exact source-tree hash is stored and policy explicitly permits it. CI proofs should be clean.

## Privacy/size

Function bytes and hashes are already project-local inputs. If full result artifacts are large, store content-addressed blobs and put their hashes/locations in the certificate. The certificate must remain self-consistent if external artifacts disappear; absence should make revalidation fail, not silently pass.

## Acceptance criteria

- Two identical clean runs produce the same request and engine hashes.
- A one-byte semantic source change invalidates cache/certificate reuse.
- Dependency, contract, relocation, or callee changes invalidate reuse.
- Promotion can explain exactly which validation evidence supported the tier.
