# 29 — Declared-return ABI shapes for EQUIVALENT_MATCH

Status: plan v2 (adversarially reviewed; findings folded in). Motivating case:
`__prep_buffer` (`us-802c06ec`, MSL `buffer_io.c`) — pure Chaitin register-swap
vs retail, proven memory-equivalent, but the auto contract keeps `r4` as a
live-out (i64-return-half assumption) so the proof fails on a dead volatile.

## 1. Problem

`tools/ppc_equivalence/abi_infer.py::infer_abi_shape` sets
`returns_i64=False` only when **both sides return and neither writes `r4`**.
Under a pure register-allocation difference (Chaitin rotation), `r4` **is**
written as scratch but is dead at the `blr`. The write-based test therefore
keeps `returns_i64=True`, `observables_for_exit()` keeps `r4`, and the proof
reports `not_equivalent` on a register no caller can observe.

This blocks `EQUIVALENT_MATCH` for the dominant near-miss category
(reg-swap-only residuals at 95–99% fuzzy) and forces `FULL_MATCH` on
functions whose coloring is source-insensitive (verified on `__prep_buffer`:
identical IR across 8 source permutations, with and without `-ipa file`).

## 2. Design

**Trusted declaration → narrow `AbiShape`.** An agent supplies the function's
source-level return-type class (`declared_return`). The probe attaches a
narrowing `AbiShape` derived from that declaration, in fail-closed conjunction
with the body-inferred shape. `observables_for_exit()` (existing code,
`contract.py`) then omits `r4` (and `f1`/`f1.ps1`) at `return`/`fallthrough`
exits. Indirect exits (`indirect-branch`/`call-indirect`) are untouched.

### 2.1 Type-class mapping (fixed after adversarial finding #1)

| `declared_return`          | returns_i64 | returns_float | Compared at exit        |
|---------------------------|-------------|---------------|--------------------------|
| `void`                    | False       | False         | r4, f1(+ps1) dropped     |
| `i32`,`u32`,`bool`,`ptr`  | False       | False         | r4, f1(+ps1) dropped     |
| `f32`,`f64`               | False       | **True**      | r4 dropped; **f1 kept**  |
| `i64`,`u64`               | True        | False         | r4 kept; f1 dropped      |
| `aggregate`,`f128`        | —           | —             | no narrowing (None)      |
| unknown / absent          | —           | —             | no narrowing (None)      |

Only `r4` / `f1` / `f1.ps1` are ever dropped — exactly the existing
`observables_for_exit()` behavior. `r3` is **always** kept, even for `void`.

### 2.2 `AbiShape` gains a real `declared_return` field (finding #3)

`tools/ppc_equivalence/abi_shape.py`:

```python
@dataclass(frozen=True, slots=True)
class AbiShape:
    returns_i64: bool = True
    returns_float: bool = True
    outgoing_gpr_args: int = 8
    outgoing_fpr_args: int = 8
    source: str = "default-conservative"
    declared_return: str | None = None   # NEW
```

`to_dict`/`from_dict` round-trip it (`to_dict` includes the key only when not
None, keeping old payload shapes byte-stable). Because
`EquivalenceContract.resolution_dict()` embeds `AbiShape.to_dict()`, the
declaration now flows into `ProofResult.contract_resolution`, the probe cache
key, and (once persisted, §2.5) certificates — making the §2.6 tier cap and
§2.7 invalidation implementable at all.

### 2.3 Fail-closed conjunction with inference

```python
combined.returns_i64   = inferred.returns_i64   and declared.returns_i64
combined.returns_float = inferred.returns_float and declared.returns_float
```

A declaration may only narrow further than inference; it can never re-add an
observable body analysis removed. `outgoing_*_args` come from the inferred
shape only. `declared_return` string is preserved on the combined shape.

### 2.4 API (frozen — parallel work builds against this)

```python
# tools/ppc_equivalence/abi_infer.py
DECLARED_RETURN_SHAPES: dict[str, AbiShape]  # per §2.1

def abi_shape_from_declared_return(declared: str | None) -> AbiShape | None:
    """Narrowing shape for a trusted return-type declaration.

    Returns None for None/unknown/"aggregate"/"f128" (no narrowing).
    Shape source is f"declared-return:{declared}"."""

def combine_abi_shapes(inferred: AbiShape, declared: AbiShape | None) -> AbiShape:
    """Fail-closed conjunction (§2.3). declared=None -> inferred.
    Source: "+".join of distinct non-default sources, order: inferred, declared."""
```

### 2.5 Certificate + cache identity (findings #3, #5, #6)

1. `_build_equivalence_certificate` (`equivalence_check.py`) gains an
   `abi_shape` field (dict) when the proof used one; persisted in the
   certificate JSON. (`abi_shape` was previously only in the cache-key
   payload, never in certificates.)
2. `proof_result_from_certificate` (`equivalence_policy.py`) restores
   `contract_resolution` (incl. `abi_shape`) from the certificate so the
   registry/recertify path sees the declaration.
3. `provenance.proof_request_identity` gains an optional `abi_shape`
   parameter; the `_prove_bytes` call site passes the combined payload.
4. `equivalence_certificate_error` gains a registry-vs-certificate check:
   if the registry target's current `declared_return` differs from the
   certificate's `abi_shape.declared_return`, the certificate is stale →
   `REVALIDATION_REQUIRED`. (This replaces re-attestation's digest-only view
   for declarations.)

### 2.6 Registry field

`tools/coop/targets.schema.json` function-target properties gain:

```json
"declared_return": {
  "type": "string",
  "enum": ["void","i32","u32","bool","ptr","f32","f64","i64","u64","aggregate","f128"]
}
```

Representation pinned (finding #9): **no `Target` dataclass field** — the
value lives in `Target.extra` (already round-trips via `load_targets` /
`_target_registry_rows`). Only schema + validation tests change.
`targets validate` must pass on the existing 19 300-target registry.

### 2.7 Probe integration (`_prove_bytes`, `equivalence_check.py`)

1. Resolve `declared_return` (first hit wins):
   explicit `declared_return` kwarg → registry lookup via `target_id`.
   Registry lookup is **not** gated on `abi_shape_inference_enabled`
   (finding #7): an explicit declaration always applies.
2. Registry lookup hardening (finding #8): `get_target` `KeyError` → treat as
   no declaration. If the resolved target has a `symbol` and it differs from
   the symbol being proved → fail closed (`INVALID_INPUT`), never apply
   another function's declaration.
3. Attach: `declared = abi_shape_from_declared_return(value)`; if not None,
   `resolved_contract = with_abi_shape(resolved_contract, combine_abi_shapes(inferred, declared))`
   — also when inference was conservative (then `inferred` is
   `AbiShape.conservative()` for the conjunction).
4. `prove_unit_symbol` / `certify_unit_symbol` accept a `declared_return`
   kwarg (explicit wins over registry) and forward it.
5. `equivalence check-unit` CLI (`tools/coop/run.py`): add
   `--declared-return` (argparse `choices=` the §2.6 enum — finding #13) and
   wire it into `prove_unit_symbol` for **both** the normal and `--linked`
   paths (finding #4); the `--linked` arg parser must reject unknown flags
   instead of silently ignoring them. The engine-level `check-objects` /
   `check-hex` paths in `tools/ppc_equivalence/cli.py` are out of scope for
   registry declarations (no target context) — documented, not wired.
6. Cycle attempt logging (finding #11): when a declaration participated in a
   passing proof, `cycle` includes `"declared_return": <value>` in the
   `attempts.jsonl` record (optional field).

### 2.8 Caller-corroboration gate (finding #2)

`declared_return` narrowing is refused (treated as no declaration, with a
warning in the probe detail) unless **both**:

1. the target has `has_indirect_calls == false` and empty
   `unresolved_called_functions` in the registry call graph (already tracked);
2. the target has **≥1 direct in-registry caller** — computed by scanning the
   registry's `called_functions` edges for the target's symbol (i.e. there
   exists at least one direct call site that will be independently verified
   when matched). Address-taken-only functions (vtable slots, callbacks with
   no direct caller) get no narrowing in v1.

Escape hatch: `prove_unit_symbol(..., force_declared_return=True)` /
`--force-declared-return` records `"declared_return_forced": true` in the
certificate payload for audit. No registry path sets it.

### 2.9 Promotion / tiers (findings #3, #12)

`_compute_confidence_tier_legacy` (`equivalence_policy.py`): after tier
computation, if `result.contract_resolution.get("abi_shape", {}).get("declared_return")`
is set, cap the tier at **C**. With §2.2/§2.5 this now works on both the live
path (`ProofResult.contract_resolution`) and the recertify path
(`proof_result_from_certificate` restoring it). Promotion to
`EQUIVALENT_MATCH` is unaffected (coop.json allows A/B/C). Fuzzy floor stays
50.

## 3. Soundness argument (v2)

Threat: a function declared `void`/`i32` actually returns i64 in retail; the
narrowed proof accepts an `r4` divergence a caller depends on.

Mitigations:

1. **Caller corroboration (eventual).** If retail truly returns via `r4`,
   any retail caller that consumes it reads `r4` after the call. The decomp
   caller, compiled against the void/32-bit declaration, does not reproduce
   that read pattern, so the **caller** fails its own byte comparison /
   proof. §2.8 guarantees at least one such direct call site exists; v1
   excludes address-taken-only functions where no direct witness exists.
2. **Load-bearing invariant (now tested, §4 T-C1):** certified callee
   summaries derive their `writes` from body analysis
   (`validate_callee_contract`), **not** from narrowed observables — so a
   parent proof still sees `r4` clobbered by a narrowed callee and fails if
   it actually depends on it.
3. **Dead-return case is harmless.** If no caller anywhere reads `r4`, the
   declaration error is semantically inert by construction.
4. **Only `r4`/`f1` are ever dropped.** Memory, nonvolatiles, `r3`, CR
   fields, and `f1` for float returns remain compared; real semantic
   differences still fail (§4 P/N tests).
5. **Fail-closed conjunction** (§2.3) and enum-restricted declarations
   (§2.6); unknown/aggregate types get no narrowing.
6. **Auditability + invalidation.** Certificates carry
   `abi_shape.declared_return`; `proof_request_identity` binds it;
   declaration changes invalidate stored certificates via the §2.5.4
   consistency check; passing cycles log the declaration (§2.7.6).

Accepted residual risk: a wrong declaration whose only consumers are
matched-never (unmatched forever) would not be caught by (1); mitigated by
tier-C capping and bottom-up recertification flags on declaration change.

## 4. Testing plan (strict)

New `tools/ppc_equivalence/tests/test_declared_return_shape.py`:

- **S1** mapping table §2.1 (every enum value; None/unknown/aggregate → None).
- **S2** `combine_abi_shapes`: declared cannot widen; outgoing-arg counts
  pass through; `declared_return` preserved; source strings join.
- **S3** `AbiShape.to_dict`/`from_dict` round-trip incl. `declared_return`;
  old payloads without the key still load.

New `tools/ppc_equivalence/tests/test_declared_return_equivalence.py`
(style of `test_adv_r4_thunk_leak.py`):

- **P1** `__prep_buffer`-shaped reg-swapped pair: auto → `NOT_EQUIVALENT`
  with `mismatch.name == "r4"`; auto + declared `void` → `EQUIVALENT`;
  auto + declared `i64` → `NOT_EQUIVALENT`.
- **P2** float-return pair differing only in `f1` at exit: declared `f32` →
  `NOT_EQUIVALENT` (finding #1 regression: f1 kept); same pair identical in
  `f1` but differing in `r4` → `EQUIVALENT`. Add an `f1.ps1` variant.
- **N1** memory divergence (different stored value/offset): declared `void`
  → still `NOT_EQUIVALENT`.
- **N2** `r3` divergence at exit: declared `void` → still `NOT_EQUIVALENT`.
- **N3** indirect-branch thunk with declared `void`: `r4` clobber before
  `bctr` → still `NOT_EQUIVALENT` (indirect exits never narrowed);
  `test_adv_r4_thunk_leak.py` keeps passing unchanged.
- **N4** declared `void` but divergence in a nonvolatile (r31) →
  `NOT_EQUIVALENT`.

Coop tests (`tools/coop/tests/`, extend or new):

- **C1** certified-narrowed-callee composition: callee certified under
  declared `void`; parent body reads `r4` after the call and diverges →
  parent proof `NOT_EQUIVALENT` (guards §3.2 invariant).
- **C2** cache/cert invalidation: prove with `void`, then with `i64`, then
  absent → distinct cache keys, distinct `certificate_sha256`;
  `equivalence_certificate_error` flags registry-vs-certificate
  `declared_return` mismatch.
- **C3** registry → `_prove_bytes` lookup: `declared_return` in registry is
  picked up via `target_id`; `KeyError` → no narrowing; symbol mismatch →
  `INVALID_INPUT`.
- **C4** schema: accepts each enum value, rejects others; registry
  round-trip preserves the field; `targets validate` passes.
- **C5** conservative-inference attach path: inference conservative +
  declared `void` → shape attached and r4 dropped (§2.7.3 second case).
- **C6** byte-identical `certify_unit_symbol` path: no narrowing, callee
  summary still lists `r4` in writes, no bogus `declared_return` recorded.
- **C7** tier cap: register-only proof + declared `void` that would
  otherwise be Tier A/B → Tier C, on both live and
  `proof_result_from_certificate` paths.
- **C8** §2.8 gate: target with no direct in-registry caller → narrowing
  refused (probe detail mentions refusal); `force_declared_return` override
  narrows and records `"declared_return_forced": true`.

Integration (coordinator, not flash agents):

1. Registry edit: `us-802c06ec.declared_return = "void"` → `targets validate`
   → `cycle us-802c06ec` → expect `EQUIVALENT_MATCH`, certificate contains
   `declared_return`; attempt log contains the field.
2. `targets recertify --bottom-up --dry-run` clean; then flip the registry
   value to `i64` in a scratch copy → recertify flags REVALIDATION_REQUIRED.
3. Full CI gate:
   `python tools/ppc_equivalence/gen_fixture_blob.py --check`;
   `python -m unittest discover -s tools/ppc_equivalence/tests -p "test_*.py"`;
   `python -m tools.ppc_equivalence differential`;
   `python -m tools.ppc_equivalence.docs_sync --check`.

## 5. Work split (flash-agent tasks)

- **T1 core engine**: `abi_shape.py` field, `abi_infer.py` mapping +
  `combine_abi_shapes`, tests S1–S3 and P1–N4.
- **T2 plumbing**: schema field (§2.6), `_prove_bytes` integration incl.
  hardening (§2.7.1–4), CLI wiring (§2.7.5), certificate field +
  `proof_request_identity` + `proof_result_from_certificate` restore
  (§2.5.1–3), consistency check (§2.5.4), cycle attempt logging (§2.7.6),
  §2.8 gate, tests C2–C6, C8.
- **T3 policy**: tier cap (§2.9) + test C7. Depends only on the frozen
  `declared_return` field name in `contract_resolution.abi_shape`.
- **T4 docs**: `tools/ppc_equivalence/README.md`,
  `tools/ppc_equivalence/SOUNDNESS.md` trust note, `docs/MWCC_REFERENCE.md`
  pattern entry, `docs_sync --write` at the end.

T1∥T2∥T3∥T4 in parallel against the frozen API in §2.4, the
`declared-return:<type>` source convention, and the §2.6 enum. Coordinator
integrates, runs the full suite plus live verification, then an adversary
agent re-reviews the merged diff.

## 6. Explicit non-goals (v1)

- No source-parsing return-type extraction (declaration is agent-supplied).
- No dropping of `r3` or anything beyond `r4`/`f1`/`f1.ps1`.
- No change to `indirect-branch`/`call-indirect` exit handling.
- No narrowing for address-taken-only targets without
  `force_declared_return`.
- No fuzzy-floor or promotion-threshold changes.
- No `configure.py` / splits / matched-source edits.
- No `targets update` CLI (registry edits stay manual + `targets recertify`).
