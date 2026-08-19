# Implementation plan: certifying functions with indirect (callback) calls

**Status:** reviewed & **parked** · **Scope:** `tools/coop`, `tools/ppc_equivalence`, registry schema, certifier
**Motivating targets:** `us-802e7f00 btm_acl_role_changed`, `us-802e7a48 btm_read_remote_features_complete`
(both 0-structural, size-exact, registry `equivalence_status: inconclusive_smt_disabled` — SMT was
*never invoked* on them; the retail bodies contain a real `bctrl` through a callback pointer:
`btm_cb.p_switch_role_cb(...)` / `btm_cb.p_acl_changed_cb(...)`).

> **When to read this:** only if you're extending the certifier to cover indirect (callback) calls/
> the `has_indirect_calls` gate. **Skip it:** this plan is **parked** — §0 (adversarial review) says the
> original diagnosis was wrong; don't treat §§1–8 as current.

> **§0 — Adversarial review outcome (2026-08-03).** Two independent adversarial reviews (Kimi K3,
> GLM 5.2, via paseo) plus a WS0 baseline probe run on the motivating targets. **Verdict: do not
> approve as written; the blocker diagnosis was wrong.** The corrected diagnosis, mandatory fixes,
> and the resurrection trigger are in §0.1–§0.5 below. Sections 1–8 below remain the historical
> proposal; where they conflict with §0, §0 wins.
>
> The full reviews: `/tmp/indirect_call_review_kimi_k3.md`, `/tmp/indirect_call_review_glm52.md`.

### §0.1 Corrected blocker diagnosis (WS0 baseline, 2026-08-03)

WS0 ran both probes on `us-802e7f00` / `us-802e7a48` with the coop `has_indirect_calls` gate error
suppressed exactly as WS1 proposed. Result: **WS1 alone does nothing for these targets.**

1. **Register-renaming witness fails on rho (Gate 4), not on the gate.** `us-802e7a48` has 5 real
   mismatches in 139 instructions — all local reg-swaps (`slot 53: gpr r5 maps to both r5 and r4`).
   `us-802e7f00` rejects at `slot 51: r30 maps to both r31 and r27`. The reg-swaps are *local temp
   allocation differences*, not a global register permutation — no bijection exists, so the witness
   can never certify them regardless of gate state. (The remaining apparent mismatches are
   branch-target absolute-address artifacts; raw bits are equal.)
2. **The SMT probe fails in the executor, not at the coop gate.** With the gate open, both targets
   return `INCONCLUSIVE_UNSUPPORTED: "call target None has no matched-callee lemma"` in ~2s. This is
   `semantics.py:5605-5609`: the symbolic executor has **no resume path after an unenumerated
   `bctrl`**. No path explosion, no unsupported opcode — just the missing opaque-call resume.
3. **Registry facts corrected:** `us-802f5648` is already FULL_MATCH/ACCEPTED (the "second half of the
   error" is gone); accepted `has_indirect_calls=true` count is 576 (575 FULL_MATCH + 1
   EQUIVALENT_MATCH, `us-802517fc`, which has **no certificate** and will fail the recertify wave).

So the plan's §1 premise ("the coop gate is the wall") was wrong; the real wall is the engine's
missing opaque-call resume path. WS2 is **necessary but not sufficient as written**: the rho failure
confirms the reviewers' BLOCKER 1 — with local reg-swaps, the two sides reach the `bctrl` with
different register *names* holding equal *values*, so any name-keyed UF token breaks.

### §0.2 Mandatory fixes before any WS2 work (fold into the workstreams below)

1. **Value-keyed shared UF (BLOCKER 1):** the opaque-callee summary must be one shared
   uninterpreted-function instance keyed on the **proven-equal symbolic CTR value** (canonical
   expression, e.g. equal `load(memory0, slot)` on both sides) with inputs narrowed to the callee's
   EABI `reads` set (excluding `lr`), proven equal under the renaming correspondence. Forbid
   fresh-per-side clobbers; forbid slot-only keying (a writable global can hold different values at
   different program points).
2. **No-write-to-slot discharge (BLOCKER 2):** `btm_cb` is a **writable** global; "same slot + same
   offset" gives same *address*, not same *value*. Add an UNSAT discharge proving neither side
   writes `[slot, slot+3]` between entry and the load (analogous to `write_hits_table_conditions`
   for ROM images), or fail closed. Add a negative test: same slot, one side stores before the call.
3. **LR modeling (BLOCKER 3):** the opaque-EABI contract's `reads` includes `lr` and the model sets
   `LR = pc+4` after `bctrl`, but retail/candidate VAs differ — congruence is impossible unless `lr`
   is excluded from the indirect-callee `reads` (as narrow-EABI already does) and the
   location-independence assumption is extended to opaque indirect callees with justification.
4. **Per-exit-PC gating (MAJOR):** the M1-gate exemption must discharge **per exit PC** and only for
   `exit_kind == "call-indirect"`. Never copy the per-function shape of
   `_is_input_derived_virtual_thunk` — a body with a covered `bctrl` plus an uncovered `bctr` must
   stay gated. Add the mixed-shape negative test.
5. **Two admission sites (MAJOR):** the new algorithm string goes in
   `capability_manifest.authoritative.json:48` **and** `KNOWN_INDIRECT_TARGET_ALGORITHMS`
   (`jump_table_obligations.py:555`); cert-format change must be **additive** (`proof_features` /
   new obligation field), never a `result_format` bump (invalidates every certificate).
6. **`us-802517fc` policy (MAJOR):** it is the ideal positive test *and* a guaranteed recertify
   regression; the plan must mint its first certificate via the new obligation or grandfather it
   explicitly. §5's "no regressions" criterion is otherwise unachievable.
7. **Stack/aliasing premise (MAJOR):** equal SP and equal live stack contents at the call site;
   different frame sizes around a `bctrl` must be a negative case.
8. **Negative matrix additions:** slot written before call; live reg-swap at call site; pointer in
   non-r12 register; `bctr` tail form; mismatched guard predicates; call-site PC/prefix mismatch;
   SDA-relative slot; repeated loads with intervening store.

### §0.3 Effort re-estimate (reviewers)

WS0 ½ day (done). WS1 ≈ 1 day (still worth doing: better diagnostics, declared-callee plumbing —
it also unblocks the ~6 vtable-dispatch targets at zero engine cost, though those are out of
scope for this plan). WS2 ≈ 2–3 weeks including a new executor resume path in `semantics.py`
(the repo's most soundness-sensitive file) and soundness review rounds. WS3 ≈ 3–5 days, dominated
by the recertify wave (~576 indirect-flagged accepted targets; ~8315 total if hash inputs rotate).
Total ≈ 3–4 weeks for one engine owner. The original "1–1.5 weeks" was not credible.

### §0.4 Resurrection trigger (parking decision)

This plan is **parked**, not rejected. Implement WS2 only when **any** of these holds:

- A writable-global-callback target the fork actually prioritises reaches ACTIVE
  (closest candidates today: CriWare sfd/adx near-match targets, WPAD — ~15 near-match targets); or
- Upstream `xbret/xenoblade` needs the opaque-call capability (the BTE callback targets this plan
  names are BACKLOG/unassigned and are not it); or
- A project decision to build the capability as infrastructure (2–3 week budget approved).

On resurrection: re-run WS0 (30 min) on the specific target(s) to confirm the executor resume gap
is the only wall, then implement §0.2 + WS1–WS3 with the corrected diagnosis. If the probe
reports anything other than `INCONCLUSIVE_UNSUPPORTED: call target None`, re-diagnose before
building.

### §0.5 What WS1 still buys today (cheap, independent)

WS1 (declared-indirect-callee schema + `_load_certified_callees` warning-not-error + precise
callee diagnostics) remains worth doing: it fixes misleading `inconclusive_unvalidated_callee`
diagnostics, removes the blunt `has_indirect_calls` error for vtable/jump-table-proven exits, and
makes the registry auditable — without touching the engine. It does **not** certify the motivating
targets (see §0.1) and must not be presented as doing so.

---

## 1. Problem statement

`EQUIVALENT_MATCH` requires the Z3 probe to return `EQUIVALENT`. For any target whose retail body
contains an indirect call, the probe is currently *never invoked*:

1. **Registry flag.** `parse_asm_calls` (`tools/coop/lib/targets.py:1360`) scans retail `.s` and sets
   `row["has_indirect_calls"] = function.has_indirect` (`targets.py:1470`) whenever a function contains
   `mtctr`/`bctrl` (or a non-canonical indirect exit).
2. **Certified-callee gate.** `_load_certified_callees` (`tools/coop/lib/equivalence_check.py:603`)
   appends `"registry has an unresolved indirect call"` **unconditionally** when `has_indirect_calls`
   is true, plus an error for every direct callee that is not yet ACCEPTED (the example the original
   text cited, `"callee 'us-802f5648' is not accepted"`, is **stale** — that callee is FULL_MATCH
   since 2026-08-03; see §0.1).
3. **Both probes refuse.** The register-renaming witness declines on the gate
   (`equivalence_check.py:1448` `if certified_context.errors: return None`); the SMT probe returns
   `INCONCLUSIVE_UNVALIDATED_CALLEE` (`equivalence_check.py:2965` `if context.errors and not bytes_identical`).
   **WS0 (2026-08-03) shows this is only half the story:** with the gate open, the witness still
   fails on rho (Gate 4) and the SMT probe fails in the executor with
   `INCONCLUSIVE_UNSUPPORTED: "call target None has no matched-callee lemma"` — see §0.1.
4. **Engine fail-closed gates.** Even if the coop gate were bypassed, the engine demotes
   `EQUIVALENT` unless a proof context covers every indirect exit:
   - `virtual_call_gate_reason` (`tools/ppc_equivalence/vtable_obligations.py:601`) recognises the
     `lwz r12, off(rX); mtctr r12; bctrl` pattern but demands
     `readonly-image + indirect-target-closure + semantic callee certificate` premises.
   - `build_virtual_call_obligations` (`vtable_obligations.py:213`) enumerates targets from a **vtable
     ROM image** (hashed artifact) — it has no path for a pointer loaded from a **writable global**.
   - M1 catch-all `unresolved_indirect_exit_gate_reason` (`tools/ppc_equivalence/jump_table_obligations.py:975`)
     fires on the terminal `exit_kind` unless a discharged jump-table/virtual-call context exists.
     It already contains one exemption: input-derived virtual thunks (`_is_input_derived_virtual_thunk`,
     `jump_table_obligations.py:952`) where both sides branch to a CTR loaded from the shared `this`.

**Escape hatch today:** `bytes_identical` bodies bypass everything (the `full-instruction-match`
certificate path). This is why 575 of the 576 accepted targets with `has_indirect_calls=true` are
FULL_MATCH; the single EQUIVALENT_MATCH one (`us-802517fc`) predates the gate and carries no certificate.
(Counts per registry, 2026-08-03; the plan was originally written against a stale 569/568 snapshot.)

**The two blocked targets cannot reach FULL_MATCH**: their remaining diffs are local register-allocation
tie-breaks (47 and 5 local temp reg-swaps — WS0 confirmed no global register bijection exists, see
§0.1) that are documented as not steerable from high-level C.

---

## 2. Key soundness insight (why this is tractable)

For *equivalence* the callee set does **not** need to be enumerated. If both bodies load the pointer
from the **same global slot** (same absolute address, same offset) and the loaded value is proven
symbolically equal at the call site, both sides invoke the *identical* callee with *identical*
pre-call state. The call is then an **opaque common effect**: the callee's clobbers/effects apply
symmetrically, so post-call state equality is preserved without knowing what the callee does.

This is exactly the reasoning already used in two places:
- the FULL_MATCH bypass comment (`equivalence_check.py:2966`): *"A byte-identical body is equivalent
  independently of its callees: both sides execute the same calls with the same machine state"*;
- the input-derived thunk exemption (`_is_input_derived_virtual_thunk`).

The plan generalises that reasoning into a **certified obligation** (not a bare exemption): prove
pointer equality at each `bctrl`, record the evidence in the certificate, and discharge the M1 gate.

**Declared callees are an auditability feature, not a soundness requirement.** Binding each declared
target's semantic certificate is optional; opaque EABI contracts suffice for the common-effect argument.

---

## 3. Design options

### A. Opaque-common-exit certification (recommended)
Prove `r12` (the CTR value) equal at each matching call-indirect exit; treat the call as a common
opaque effect; record an `indirect-target-closure`-style obligation in the certificate.

- Pros: no schema change strictly required; reuses the thunk-exemption reasoning; sound core is small
  ("same slot + same offset → same pointer → same callee"); generalises to any global-slot callback.
- Cons: touches the engine's fail-closed gates; requires new obligation type + cert format inputs;
  needs a negative-test matrix to keep the gate honest.

### B. Declared indirect callees (registry-driven enumeration)
Add `declared_indirect_callees: [target-id]` to the target record; resolve them into contracts in
`_load_certified_callees`; extend the virtual-call obligations machinery to enumerate *declared*
targets instead of ROM-image words.

- Pros: explicit, auditable; fixes the misleading diagnostics for unaccepted direct callees too;
  gives the cert a `callees` list that matches the existing schema.
- Cons: heavyweight (new obligations builder mirroring `build_virtual_call_obligations`); declarations
  can rot / lie; the ROM-image machinery doesn't fit writable globals, so most of it is new anyway.
- **Partial scope worth doing regardless:** fix `_load_certified_callees` so it resolves declared
  indirect callees *and* reports direct-callee blockers precisely. This improves diagnostics (the
  `us-802f5648` blocker is already resolved — it is FULL_MATCH/ACCEPTED) and makes the registry
  auditable; see §0.5.

### C. Force FULL_MATCH via register-allocation matching
Documented in this repo as not steerable for these functions (Chaitin cycles / temp tie-breaks).
Rejected for the motivating targets, but remains the only path for any function the engine cannot
prove (see §7).

**Recommendation: A as the engine core, with the B "registry/callee-context" plumbing added for
auditability and diagnostics.** Concretely the plan below implements A + the B-plumbing subset.

---

## 4. Implementation steps

### Workstream 1 — registry plumbing & callee context (½ day)

1. **Schema** (`tools/coop/targets.schema.json`): add
   `"declared_indirect_callees": { "type": "array", "items": { "type": "string" }, "uniqueItems": true }`
   to the function target object (next to `has_indirect_calls`).
2. **Callgraph sync** (`tools/coop/lib/targets.py`, `sync-calls` writer ~line 1470): preserve an
   existing `declared_indirect_callees` value when overwriting `has_indirect_calls`; add a CLI subcommand
   (`run.py targets declare-indirect <target-id> --callee <target-id> ...`) that validates the IDs and
   records the edge.
3. **`_load_certified_callees`** (`tools/coop/lib/equivalence_check.py:603`):
   - Replace the unconditional `if has_indirect: errors.append(...)` with: resolve
     `declared_indirect_callees` through the same code path as direct callees (contracts keyed by
     symbol/address, `address_to_target_id`, FULL_MATCH narrow-EABI handling, `_reattest_certificate_tree`
     per callee). Error only when a declared callee is missing or not accepted.
   - Keep a *warning*-level note (not an error) when `has_indirect_calls` is true and no declaration
     exists, so Option A's pointer-equality path can still proceed in Workstream 2 while the registry
     stays informative.
   - Add the declared callees to `CertifiedCalleeContext.dependencies` so certificates list them.
4. **Docs**: note the flag semantics in `MWCC_CASES.md` (§ on certified callees) so future agents
   know to declare callback edges after identifying them (symbol recovery already finds them).

### Workstream 2 — engine: opaque-common-exit obligation (2–4 days + review)

1. **New proof context** in `tools/ppc_equivalence` (new module `call_indirect_obligations.py`,
   mirroring `jump_table_obligations.py` structure):
   - `find_call_indirect_exits(insns)` — reuse `find_virtual_call_candidates` (`tools/ppc_equivalence/vtable.py:49`)
     or a sibling that matches the `lwz rX, off(G); mtctr rX; bctrl` shape, tolerating the
     `cmpwi rX,0; beq` guard that precedes the call (the BTE pattern does both).
   - `CallIndirectCommonExitProofContext` holding: exit PC (original + candidate), the global slot
     (absolute symbol + offset) each side loads from, and optional `declared_indirect_callees`.
   - `build_call_indirect_obligations(context)` returning `address_space` + `indirect_targets`
     entries with `algorithm="call-indirect-common-target-v1"`, mirroring
     `build_indirect_targets_obligation` (`jump_table_obligations.py:275`), plus the slot evidence
     (symbol, offset, artifact hashes) so the cert binds to the exact load site.
2. **Pointer-equality premise**: in `semantics.execute_cfg` or the obligations discharge, assert the
   CTR value at each matched exit is equal between sides (both read the same slot → same symbolic
   value). If the load sites differ (different symbol/offset), the obligation fails and the M1 gate
   stays closed.
3. **Gate relaxation** (`jump_table_obligations.py:975` `unresolved_indirect_exit_gate_reason`):
   exempt an exit when a discharged `call-indirect-common-target` obligation covers it — same shape as
   the existing `_is_input_derived_virtual_thunk` exemption (`jump_table_obligations.py:952`), but
   certificate-backed rather than syntactic.
4. **Wire-up**:
   - `prove_unit_symbol` (`equivalence_check.py:1960`, obligation assembly at ~2131-2143): build the
     new context after (or alongside) the jump-table/virtual-call contexts; set
     `proof_features = ["readonly-image", "indirect-target-closure"]` for the new path.
   - Engine obligation assembly (`tools/ppc_equivalence/engine.py:2039-2083`): attach the new
     `indirect_targets` entries and record them in the proof result (`indirect_targets_used`).
   - `capability_attachment.py:81` capability `"indirect-target-closure"` already maps to
     `indirect_targets` — verify the manifest (`tools/coop/capability_manifest.authoritative.json`)
     admits the new algorithm string, or extend it.
5. **Certificate format** (`proof_request_hash`, `equivalence_check.py` inputs): add the new obligation
   source/algorithm + slot evidence to the hash inputs; extend the certificate `summary`/`callees`
   with the declared indirect callees (when present). Bump `result_format` or add a
   `proof_features` entry — decide one, keep the schema `additionalProperties: true` so no breaking
   change is needed on the cert object.
6. **Engine hash pin**: `allowed_engine_sha256` in `coop.json` must be refreshed (the harness refuses
   to run probes against an un-pinned engine tree).

### Workstream 3 — soundness, tests, rollout (1–2 days)

1. **Positive cases** (new corpora in `tools/ppc_equivalence/corpora/`):
   - `us-802e7f00 btm_acl_role_changed`, `us-802e7a48 btm_read_remote_features_complete`:
     `cycle --smt` must reach `EQUIVALENT_MATCH` with the new obligation recorded.
   - A synthetic pair where both sides `lwz` the same global slot and `bctrl` (pointer equality holds).
2. **Negative cases** (must stay `NOT_EQUIVALENT`/`INCONCLUSIVE`):
   - Same shape but different slot offsets between sides.
   - One side direct `bl`, the other `bctrl`.
   - Pointer loaded from a different global symbol.
3. **Regression**: re-certify the 576 accepted `has_indirect_calls=true` targets
   (575 FULL_MATCH + `us-802517fc`; the latter needs the §0.2.6 policy before this step)
   (`run.py targets recertify --bottom-up --dry-run` then for real); all jump-table / vtable corpora
   tests (`python -m unittest discover -s tools/ppc_equivalence/tests`); the `ppc_equivalence`
   differential suite (`python -m tools.ppc_equivalence differential`).
4. **Docs**: extend `SOUNDNESS.md` with the common-effect argument and its premises (same slot, same
   offset, same loaded value, symmetric clobbers); update `TRUSTED_COMPUTING_BASE.md` if it lists
   gate exemptions; add a `MWCC_CASES.md` pattern note ("global-slot callback bctrl → declare
   indirect callees + common-exit obligation").
5. **Rollout order**: WS1 → WS2 → WS3, merging behind the engine-hash pin bump. Then accept the two
   motivating targets with `cycle --smt` and commit.

---

## 5. Success criteria

- `run.py cycle us-802e7f00 --smt` and `us-802e7a48 --smt` → status `EQUIVALENT_MATCH`,
  certificate `evidence: symbolic-equivalence` with the `call-indirect-common-target-v1` obligation.
- No previously-accepted target regresses (recertify wave is clean).
- Negative corpora still fail closed.

---

## 6. Out of scope / remaining blockers

- **`us-802e8974 btm_chg_all_acl_pkt_types`** stays blocked: its blocker is the SMT **path limit**
  (4096) on two call-heavy 4-iteration loops, not indirect calls. Fixing that needs loop summarisation
  for loops containing opaque calls (`bounded_remainder_loop.py` covers memory-only loops) — a separate
  engine workstream.
- **`us-802f5648 btsnd_hcic_write_link_super_tout`** (direct callee of `btm_acl_role_changed`) is
  **already FULL_MATCH/ACCEPTED** (2026-08-03 registry) — the "second half of the
  `inconclusive_unvalidated_callee` error" the original text described is gone; the only remaining
  blocker for `us-802e7f00` is the engine executor resume gap (§0.1).
- Functions whose callback pointer is loaded from a *register parameter* (not a global slot) are not
  covered by Option A; they need either the declared-callee enumeration (Option B) or remain FULL_MATCH-only.

---

## 7. Effort estimate

| Workstream | Estimate | Owner profile |
|---|---|---|
| WS1 registry/callee context | ~½ day | coop tooling |
| WS2 engine obligation + gates | 2–4 days | ppc_equivalence owner |
| WS3 tests/re-cert/docs | 1–2 days | engine + coop |
| Soundness review | 1–2 days (parallel) | reviewer |

Total ≈ 1–1.5 weeks for one engine-owning engineer including review. Main risk is the soundness review
of the common-exit obligation (the repo treats certifier soundness as a trust boundary: `SOUNDNESS.md`,
`TRUSTED_COMPUTING_BASE.md`, `allowed_engine_sha256` pin, capability manifest).

---

## 8. Risks

- **Over-broad gate relaxation**: the new exemption must be obligation-backed (pointer equality proven,
  load-site evidence hashed into the cert), never a syntactic pattern match. The negative matrix in §4-WS3
  is the guard.
- **Certificate format churn**: changing `proof_request_hash` inputs invalidates existing certificates
  on recertify; the schema is `additionalProperties: true` so additive fields are safe, but the recertify
  wave must be planned (bottom-up, leaves first).
- **Global-slot identity across DOL/ELF**: the slot evidence must not depend on absolute VA binding
  (unlinked HA/LO proofs share symbolic addresses — see the existing note at `equivalence_check.py:2151`).
  Key the obligation on symbol + offset, not resolved address.
