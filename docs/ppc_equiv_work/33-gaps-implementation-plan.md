# 33 — Implementation plan: certifying the doc-32 "not fixed" RVL gaps (rev 5)

Status: **draft, rev 6 — Item 0 landed by the parallel impl-review line**. Rev history: 1 = plan; 2 = round-1 review; 3 = re-baseline after doc-32 A1/A2/A3 landed (`78dbecaa3`); 4 = freshness precondition; 5 = round-2 synthesis; 6 = re-sweep against the impl-review commits (`2d8f23de2` IR1–IR7, `662448aae` r2 polish) which **implemented Item 0 as "doc 32 A2 rev 5"** (per-slot RA rule).

---

## 0. Re-baseline (rev 6 — Item 0 LANDED)

Re-sweep of the 28-set against the current witness (post `662448aae`):

| Gate | rev-3 | rev-6 | Notes |
|---|---|---|---|
| fields | 19 | **4** | only the genuine diffs remain (`hcisu_h2` rlwinm mask, `SetVolumeAllSeq` pool order, `SITransfer` baked offset, `btm_sec_l2cap_access_req` bc disp) — the 15 rotation-based `fields` failures cleared exactly as Item 0 predicted |
| abi-boundary | 2 | **9** | the freed rotations resurfaced as r3/r4/perm rotations (`add_attr` r4→r21, `__wudLinkKeyEventStackCallback` r4→r29, `process_service_search_attr_req` r4→r0, `btm_sec_mx_access_request` r3→r4, `btu_hcif_process_event` r4→r5, `__OSPlayTimeFadeLastAIDCallback` r3→r5, `__HBMSYNSetupVolumeEnvelope` r3→r4, `l2c_csm_config` r3→r5, `BTM_CancelInquiry` boundary-56) |
| loop | 1 | **7** | all 7 loop functions back (reach confirmed for Item 2) |
| rho | 1 | 4 | `l2c_rcv_acl_data`, `bta_dm_pm_set_mode`, `__wudSyncStoredDevInfoToNand`, `rfc_alloc` |
| reloc | 5 | **3** | only i2f remain (AXFX ×2, lyt dtor); the stale-map cases are gone (map re-mined during impl-review) |
| deadline | 0 | 1 | `__wudSyncTryConnect` |

### Item 0 — **LANDED** (verify, don't implement)

The per-slot RA rule is implemented in `renaming_witness.py` as "doc 32 A2 rev 5":
- `_ra_field_is_register` (:110) — both-nonzero RA ⇒ renameable register pair; either zero ⇒ literal (bit-equal, excluded from rho).
- Gate 3 (:1013–1018) — `ra_rename_mask` excludes both-nonzero RA bits from the non-register diff.
- Gate 4 rho (:1058) and the region path (:1718/:1735/:1768) accumulate the RA field — the dual-change/split-brain requirement from round-2 is satisfied.
- `DriftDetectorTests` (test_renaming_witness.py:838) pins the *intentional divergence* between hexdiff's frozen classifier and the witness role table.

**Remaining work for Item 0:** run the round-2 test matrix against the landed code (accept `lwz r0,4(r23)` vs `lwz r0,4(r22)` under rho; reject `lwz r3,0(r0)` vs `lwz r3,0(r4)`; regression `lwzx r3,r0,r5` vs `lwzx r3,r6,r5` stays rejected; the mask/rho-agreement test) and confirm the 47-rotation/57-fields split on `add_attr` matches expectations — then close it out.

### Item 1 — DROPPED (rev 5), SMT-candidate list refreshed

With 9 abi-boundary targets now, the rev-5 decision holds: r3 stays unconditionally fixed (exit-live-out unsound), and A3's r4-unfix does not fire (no `declared_return` in the registry; `_written_before_return`). **Record as SMT candidates:** `add_attr`, `__wudLinkKeyEventStackCallback`, `process_service_search_attr_req`, `btm_sec_mx_access_request`, `btu_hcif_process_event`, `__OSPlayTimeFadeLastAIDCallback`, `__HBMSYNSetupVolumeEnvelope`, `l2c_csm_config`, plus `BTM_CancelInquiry` (Item 4 path first).

## 0.5 Reloc-map freshness — REWORKED (soundness-adjacent, not cost-only)

### F1 (GLM, round-2): the "staleness is cost-only" claim was wrong

Owner-verified scope: the mined map is **data-only** (`R_PPC_ADDR16_HA/LO` +
`R_PPC_EMB_SDA21`; zero REL24/REL14) and `_canonical_symbols_for_unit` can
only rewrite data relocs — the reviewer's callee-contract variant
(decomp `@N` aliasing a certified callee `foo`) is **not exploitable** (calls
are never canonicalized). The **data-address variant is real**: a stale map
rewrites a decomp `@N` data label to a retail name whose *linked data differs*
from the current build's `@N` — the witness certifies with symbolic addresses
that don't match the linked binary. Confirmed: `retail_reloc_map.json` is
**not** in `provenance.py` (untracked proof input). Mitigations below make
staleness fail-closed.

### Design (rev 5)

1. **`reloc_map.py ensure-fresh [--force]`** — fresh iff map exists AND
   `generated` exists AND no objdiff retail/decomp `.o` mtime is newer than
   `generated`; stale ⇒ `mine()`. Cost (owner-measured): scan 0.04 s,
   `mine` 1.6 s user / **~5 s wall** (rev-4's "1.7 s" was user-time —
   corrected; belt cost is per-affected-function).
2. **Per-unit trigger, lazy, post-build (Kimi R2, MAJOR).** Entry-point wiring
   in `cycle`/`diff` runs **before** the build step that stales the map (the
   edit-then-cycle case) and misses `recertify`. Instead: a lazy per-unit
   memo in `prove_unit_symbol` right where `canonical_symbols` is computed
   (:2833) — after the build, keyed per unit (this unit's two `.o` mtimes vs
   `generated`). The global rule is nearly always stale (measured
   1619/2076 units newer — a global trigger re-mines every cycle). Per-unit
   keys depend only on that unit's pair; a per-unit trigger with repo-wide
   `mine()` is sound (the belt covers the rare global-entry drift). All
   consumers (cycle, diff, batch-cycle in-process, harness subprocess,
   recertify) funnel through `prove_unit_symbol`.
3. **Belt: retry-once inside `_try_renaming_witness` (Kimi R2, MAJOR).** The
   gate must be surfaced — `_try_renaming_witness` currently returns bare
   `None`, losing the `gate=="reloc"` signal. Add a channel (diag out-param
   or tuple). On gate-2 `reloc` failure: re-mine + re-decode + retry once,
   and let the refreshed `canonical_symbols` also feed the SMT fallback
   (:2895). Re-derive `call_targets`/contracts after re-decode. **Reason-gate
   the belt**: skip when the failure is `reloc presence differs` (the i2f
   cases — re-mining cannot help; otherwise they pay ~5 s per process per
   function forever). Bounded once; no loop risk.
4. **Soundness: bind the map into the proof (GLM R2).** Record
   `reloc_map_sha256` (of the map file the canonical symbols were drawn from)
   in the witness `ProofResult`/`witness_payload` and include the map file
   path in `provenance.py` source paths (or bind the snapshot into
   `proof_request_hash`). Staleness then either (a) fails gate 2 (freshness
   enforced per invocation) or (b) is auditable — no silent proof-input
   change. Freshness check per-witness-invocation (memo only within one
   function's prove), so mid-sweep concurrent rebuilds cannot reuse a stale
   canonicalization.
5. **`generated` granularity:** stamp at `mine` START (or epoch float); a
   second-resolution stamp at end misses rebuilds during the mine window.
6. **`certify_unit_symbol`** witness call (:2981) passes no
   `canonical_symbols` (pre-existing) — the memo/belt never applies there;
   extend or scope-limit (note it).
7. **Dirty-tree policy:** `retail_reloc_map.json` is a committed shared
   artifact; `ensure-fresh` re-mines dirty the working tree during
   cycle/diff — document a commit policy (commit regenerated map separately,
   like the reloc-fix agent does).
8. **Test matrix (GLM):** (a) touch `.o` ⇒ re-mine; no change ⇒ no-op; (b)
   inject stale map ⇒ gate-2 `reloc` ⇒ retry re-mines ⇒ reaches the real next
   gate; (c) i2f presence-differs with fresh map ⇒ retry count == 1 (bounded);
   (d) **F1 regression:** inject a stale map aliasing a decomp `@N` to a
   retail name with different linked data ⇒ witness must NOT certify (passes
   only after the map is bound into `source_hash` / freshness is per-
   invocation); (e) two map contents that both pass gate 2 ⇒ payload records
   differing `reloc_map_sha256`.

---

## 1. Items — final list (rev 6)

- **Item 0 (LANDED — verify, don't implement):** the per-slot RA rule is in
  `renaming_witness.py` ("doc 32 A2 rev 5", §0 above). Remaining: run the
  round-2 test matrix against the landed code and close out.
- **Item 0.5 (OPEN):** freshness (§0.5) — per-unit lazy memo in
  `prove_unit_symbol`, retry-once belt inside `_try_renaming_witness` (gate
  surfaced), `reloc_map_sha256` bound into the witness cert + provenance,
  reason-gated belt, `generated` stamp-at-start, dirty-tree policy.
- **Item 1 (DROPPED):** see §0 — 9 abi-boundary targets recorded as SMT
  candidates; r3 stays fixed (exit-live-out unsound); A3's r4-unfix does not
  fire (no `declared_return` in registry).
- **Item 2 (GO):** loop second-cut — split `_has_loop_or_non_return_indirect`;
  drop the direct-backward-branch reject in `run_region_sliced_witness`; keep
  `bcctr`/`blrl` reject (executor records an `"indirect-branch"` terminal
  :5549 — comparison would self-agree under shared CTR, so the reject is a
  conservative scope choice, verified sound). **Loop-boundary guard
  (GLM NIT, adopted):** reject only when a boundary falls inside a
  backward-branch span with an **in-function target** (`by_index.get(target)`)
  — a backward `b` to an out-of-function address is a terminal, not a loop
  (`_cfg_successors` :664); include relocated non-link self-recursion
  (`canonical_symbol == local_symbol`, :742). Rewrite
  `test_loop_containing_target_rejected_first_cut`. Reach confirmed: 7.
- **Item 3 (independent):** SIBios `.bss` mechanism investigation
  (`SITransfer` slot 9 is a genuine immediate diff — Item 0 does not touch it;
  GLM verified gate-3 catches the immediate regardless).
- **Item 4 (after Items 0 + 0.5):** `BTM_CancelInquiry` — after 0.5 its gate 2
  passes; after Item 0, slot 56 (currently a non-zero-RA `sth` rotation
  failing `fields`) becomes the boundary-deadness failure again; Phase A
  characterise → Phase B bounded source (callees certified) → Phase C
  value-splitting redesign (gated on a future soundness round; rev-2's
  `z3.eq` assertion over-rejects).

## 2. Sequencing, scope, acceptance

1. **Item 0.5** (freshness + F1 mitigations) — prerequisite for trustworthy
   sweeps.
2. **Item 0** (A2 refinement) — restores the rotation class.
3. **Item 2** (loop) — reach 7 after Item 0.
4. **Item 3** (SIBios) — concurrent, independent.
5. **Item 4** — after 0 + 0.5.
6. Record the four SMT candidates (Item 1 targets) in targets.json.

CI (full AGENTS.md gate): `gen_fixture_blob.py --check`,
`tools/ppc_equivalence/tests`, `tools.ppc_equivalence differential`,
`docs_sync --check`, `smell_report --check`, `tools/coop/tests`.

In scope: `renaming_witness.py`, `equivalence_check.py`, `reloc_map.py`,
`run.py` (ensure-fresh wiring), `provenance.py` (map binding),
`tools/coop/tests/test_renaming_witness.py`, `SOUNDNESS.md` via `docs_sync`,
`SIBios.c`, this doc. **Out of scope:** i2f magic pools (doc-32 A5),
`bcctr`-dispatch modeling, the 3 hard-fields cases, reloc-pipeline files.
Shared-branch rules: no git destructive ops; `hexdiff.py` for builds; claims
before edits; no `--smt` unless authorised; commit regenerated map
separately.

## Files to read

- `renaming_witness.py` — `_RA_LITERAL_OPCODES` (:119), `_register_fields`
  (:148), `_gpr_fpr_masks` (:203), gates 3/4 (:951/:981), region path
  (:1579+), `_has_loop_or_non_return_indirect` (:645).
- `equivalence_check.py` — `_canonical_symbols_for_unit` (:39),
  `_try_renaming_witness` (:1422), `prove_unit_symbol` (:2759,
  canonical_symbols at :2833), `certify_unit_symbol` (~:2942).
- `reloc_map.py` — `mine` (:350), `generated` (:438), CLI (:695+).
- `tools/ppc_equivalence/provenance.py` — source paths for the map binding.
- `tools/ppc_equivalence/semantics.py` — `_dform_address` (:1854), X-form
  (:3531), PSQ (:3874), loop overflow (:5347), bcctr terminal (:5549).
- `tools/coop/tests/test_renaming_witness.py` — DriftDetector (:838-849),
  loop test (:489).
