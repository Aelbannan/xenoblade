# Category D decision packet — `extern_data_sections` (279 entries)

**Status 2026-10-10 (paseo-unitrules-d): primary deliverable = this DECISION PACKET.
No mass removal executed; no `UNIT_RULES` edits made in this worktree.**
`tools/check_unit_rules_frozen.py --check` passes (406/406, retired 109 — unchanged).
All gate evidence below is **static-only**: this worktree has no configured build
(no `build.ninja` link line, no `build/compilers`, `orig/us/files` is a broken
symlink to the main checkout which this mission forbids touching, and the sandbox
denies reaching the main checkout's cached toolchain). The coordinator re-runs the
live-derivation snippet and all link gates centrally before any ratchet.

Policy update applied: PLAN.md §17.6 "Relocation name drift" exception row was REMOVED
in the main checkout (2026-10-10). No `extern "C" lbl_eu_*` spellings were added here;
the inherited `CErrorWii.o` item is **PAUSED** for owner sign-off (§7).

## 1. Exact inventory

Scope = `UNIT_RULES` entries with a truthy `extern_data_sections` field: **279 entries**
(task brief said 295; the table has shrunk since — 279 current).
Companion full table: `docs/evidence/decomp/unit_rules_category_d_inventory.md` (279 rows:
key, stripped sections, fused fields, live/latent, source TU, retail stripped sizes,
per-label split owners, bucket).

### 1.1 Live vs latent

Live = basename is `Matching` in `configure.py` (proxy for the README RELOCPOST
link-line snippet, which needs a configured `build.ninja` — deferred to coordinator):

| state | keys | bases |
|---|---|---|
| live (Matching, link rewrites the object today) | 43 | 40 |
| latent (NonMatching, retail placeholder links; EDS dormant) | 236 | — |

Live keys (43): `CCamUtil.o`, `CChainActorList.o`, `CCur.o`, `CDeviceGX.o`,
`CERand.o`, `CLib.o`, `CLibCriStreamingPlay.o`, `CLibHbm.o`, `CMCCylinderGauge.o`,
`CMdlMouth.o`, `CMenuBattleChain.o`, `CMenuPTState.o`, `CMenuQuestLog.o`, `CProc.o`,
`CScnFilter.o`, `CScnFogMan.o`, `CScrollBar.o`, `CTaskGameEffAfter.o`,
`CTaskGamePic.o`, `CTaskLOD.o`, `CVision.o`, `CWorkSystemCache.o`, `MTRand.o`,
`UnkClass_80466348.o`, `bta_hh_utils.o`, `btu_hcif.o`, `code_800A75FC.o`,
`code_802B9064.o`, `g3d_resanmchr.o`, `g3d_resanmfog.o`, `g3d_scnobj.o`,
`lyt_animation.o`, `lyt_animation.o#Q34nw4r3lyt`, `lyt_group.o#Q34nw4r3lyt`,
`lyt_picture.o`, `lyt_picture.o#Q34nw4r3lyt`, `lyt_window.o`,
`lyt_window.o#Q34nw4r3lyt`, `pluginEve.o`, `snd_AxfxImpl.o`, `snd_Task.o`,
`ut_TagProcessorBase.o#Q34nw4r`, `ut_TextWriterBase.o#Q34nw4r2ut`.

### 1.2 Standalone vs fused

| bucket | keys | meaning |
|---|---|---|
| STANDALONE-live | 7 | EDS is the only field; link-active |
| STANDALONE-latent | 22 | EDS only; dormant |
| FUSED-RENAME-live | 18 | EDS + pool/exact/prefix/add_symbols/symbol_sizes/set_data_align only |
| FUSED-RENAME-latent | 159 | same, dormant |
| FUSED-LAYOUT-live | 17 | EDS + drop/pad/trim/retarget/zero/text fields |
| FUSED-LAYOUT-latent | 43 | same, dormant |
| CONTENT-FUSED | 6 | EDS + a byte-content field (see §6) |
| STRING-BUG (inert) | 7 | `extern_data_sections` is a bare string, not a tuple |

STANDALONE-live (batch-1 candidates, §5.3): `CTaskGameEffAfter.o` (.data/.rodata/.sdata),
`CTaskGamePic.o` (.data/.rodata/.sdata), `CWorkSystemCache.o` (.sdata2),
`bta_hh_utils.o` (.rodata), `code_800A75FC.o` (.sdata2), `snd_AxfxImpl.o` (.sbss),
`snd_Task.o` (.data).

EDS section distribution (entry counts; 7 STRING-BUG keys excluded from section counts):
`.sdata2` 208, `.data` 98, `.rodata` 65, `.sdata` 59, `.sbss` 23, `.bss` 14,
`.sbss2` 2. (`.sdata2` float/double pools dominate: EDS is mostly the shared-pool
mechanism, not vtable stripping.)

Manifest metadata on EDS keys (for the coordinator's claim pass): class rename=232 /
layout=47 (232 EDS entries were claimed `category-a` as renames because of their fused
`exact_renames`/`pool_patterns`; derived triage is layout=274/content=5); owners
category-a=232, category-b=7, unassigned=40.

### 1.3 Which shared object owns each

`extern_data_sections`'s own docstring names the owners: `split1.s` / `nw4r_data.o` /
`monolibdata*.o`. Both `.s` headers are **gone from `config/us/splits.txt`**
(`splits.txt.bak_pre_nw4r` still has them; diff of backup→current):

| dissolved blob | ranges removed (all sections) |
|---|---|
| `split1.s` | .rodata 0x195c8, .data 0x16d90, .bss 0x72c8, .sdata 0x1680, .sbss 0xf30, .sdata2 0x34a0 (total 0x3d070) |
| `nw4r_data.s` | .rodata 0x4f70, .data 0x21a8, .bss 0x39a08, .sdata 0xaa, .sbss 0x110, .sdata2 0x710, .sbss2 0x8 (total 0x413f2) |

Their former ranges are now **unowned gaps** (e.g. `.sdata2` 0x80669A68–0x8066A178,
exactly `nw4r_data.s`'s pool). Label-owner resolution over all 740 lbl/jumptable
references inside EDS entries (`pool_patterns` + `data_pool_patterns` +
`exact_renames` + `retarget_relocs` → `symbols.txt` address → owning splits block):

- into dissolved-blob gaps: 349/466 `.sdata2` refs, 174/197 `.data` refs, 18/23
  `.rodata`, 11/17 `.sdata`, 16/20 `.sbss`, 17/17 `.bss` — the large majority;
- into owned TU ranges: `monolib/src/device/CGXCache.cpp` 95 (the surviving shared
  `.sdata2` pool 0x8066A378–0x8066B52E), `monolib/src/math/FloatUtils.cpp` 22,
  plus small counts in hbm lyt twins, `CScnItemPool.cpp`, `CProc.cpp`,
  `ut_TextWriterBase.cpp`, etc. (per-key detail in the inventory doc).

So "which shared data object owns each": **(i)** the dissolved `split1.s`/`nw4r_data.s`
blobs (majority — currently ownerless gaps; DOL bytes for those ranges come from
retail, not from any linked object); **(ii)** the surviving shared pools
(`CGXCache.cpp`, `FloatUtils.cpp`); **(iii)** the scoped-twin case below.

### 1.4 The scoped-twin ownership model (verified)

nw4r/hbm same-basename twins (`lyt_group`, `lyt_layout`, `lyt_picture`, `lyt_textBox`,
`lyt_material`, `lyt_animation`, `lyt_window`, `ut_TagProcessorBase`): the
`#Q34nw4r3lyt`/`#Q34nw4r`-scoped key strips `.data`/`.sdata`/`.sdata2` on the **nw4r**
twin (whose retail split block carries **none** of those sections — e.g. all five
`nw4r/src/lyt/*.cpp` blocks have empty data slices), renaming its vtable/allocator
to `lbl_eu_*` externs owned by the dissolved `nw4r_data.s` blob; the **hbm** twin keeps
TU-local data (e.g. `lyt_group.cpp` `.data` 0x8054A0D8–0x8054A100). Stripping and owning
are consistent per applicable twin in all 279 cases (checked programmatically).

### 1.5 Vacuous-MATCH proof (static, all 279)

For **every** EDS key, **every** stripped section is size 0 in the retail split block
of **every** TU the rule applies to (scoped keys resolve to the matching twin only):
programmatic check over `splits.txt` returns **0 nonempty applicable slices / 279 keys**.
Therefore `run.py data diff` on stripped sections compares empty-vs-empty and prints
MATCH for the whole category — unit promotion (`NonMatching → Matching`) currently
requires no real data evidence on any EDS unit. This is problem (1) from the brief,
now quantified: 43 live keys / 40 bases promote code-only while reported as code+data.

Problem (2): the final DOL's bytes for stripped sections come from retail objects /
dissolved-blob ranges, not from `src/**` — silent code-only matching, double-counted
as data matching in progress accounting.

### 1.6 Dangerous case (defines + strips)

No EDS unit both defines TU-local data in a stripped section *and* strips it in a way
that collides: stripped sections are retail-empty everywhere applicable, and the fused
rename fields (`exact_renames` of vtables to `lbl_eu_*`) are precisely the mechanism
that keeps `.text` relocs resolving to the external copy. The one structural hazard is
*removal* order: deleting EDS from a unit whose MWCC object still emits the section
(float pools, vtables, statics) reintroduces local definitions that duplicate the
shared/blob labels at link (`mwldeppc` aborts on duplicate strong definitions) or
fails the raw data gate. Hence per-batch link verification (§5), latent-first (§4).

## 2. The three options

### Option A — keep EDS as an explicit declared model

Keep the strip mechanism, but make it honest: introduce an explicit unit kind/status
(e.g. `MatchingCodeOnly`, or `Matching` + `dataOk=false`), stop reporting stripped
data as matched, and record EDS as mechanical link-composition rather than matching
evidence. Progress/README/`readme_status.py` report code-only units separately; the
headline percentage never absorbs them.

- Work: `configure.py` progress kinds + reporting + docs; per-unit `dataOk` audit
  (largely done by §1 table); declarations-only checker (§3 of the category brief) so
  foreign data stays `extern`-declared.
- Risk: the §17.6-violating postprocess stays in the link indefinitely; freeze never
  completes for 279 entries; each future TU promotion must classify data ownership.
- Acceptance consequences: solves reporting honesty (problem 2's "silent" half) but
  **not** vacuous MATCH (problem 1) unless the gate learns the code-only kind; DOL
  data still comes from retail objects.

### Option B — every Matching unit owns its data

Units that cannot match their data are demoted; EDS is deleted; each source defines
real data or the unit is NonMatching. The dissolved-blob ranges (`split1.s` 0x3d070 B,
`nw4r_data.s` 0x413f2 B) must be adopted into owning TUs (same absorb work as the
already-completed criware/nw4r plans) before their client units can match data.

- Work: very large — blob adoption + data matching for ~40 live bases + demotions;
  progress percentage drops when code-only units demote.
- Risk: link churn from adopting blob bytes; duplicate-definition hazards during
  migration; long tail of float-pool units (`.sdata2` 208 entries) whose pools can
  never be "owned" TU-locally without changing codegen.
- Acceptance consequences: solves both problems completely; every `Matching` means
  code+data again. Honest but expensive; float-pool sharing (CGXCache model) fights it.

### Option C — code-only matching policy (no EDS mechanism)

Adopt code-only matching as policy: the gate stops requiring data for declared
code-only units, EDS entries are deleted in link-verified batches, `.text` relocs keep
resolving to shared/blob labels via the *source-level* `extern` declarations (no object
rewrite). DOL bytes for shared ranges come from explicitly-owned data TUs
(`CGXCache.cpp`, `FloatUtils.cpp`, blob-adopting TUs) — tracked, not silent.

- Work: policy records in PLAN.md + gate support for code-only kind + declarations-only
  CI checker + batched EDS deletion with link verification (§5).
- Risk: needs the gate/tooling changes up front; per-batch verification load.
- Acceptance consequences: solves both problems; table shrinks to zero for the
  category; `Matching` regains meaning via the code-only label.

## 3. Recommended option and ratchet plan

**Recommend Option C, transitioned through A's labeling.** First record code-only
status explicitly (A's honesty, no mechanism change), then retire the EDS mechanism in
batches (C's end state). Option B is disproportionate: most stripped bytes are
compiler-emitted float pools and dissolved-blob vtables that no TU can "own" without
codegen changes; B would demote ~40 matching code bases for a data model the linker
cannot express TU-locally.

Ratchet order — **latent-first**, then live in small link-verified batches:

| batch | keys | content | verification |
|---|---|---|---|
| 0 — tooling | 7 STRING-BUG | bare-string EDS is provably inert (no section is named `.`/`s`/`d`/… — code reading of `extern_data_sections`); delete the field or fix to tuple | coordinator link check (deletion passes `--check` immediately) |
| 1 — latent bulk | 236 keys (latent buckets + latent CONTENT-FUSED where C has cleared content) | dormant: NonMatching units link retail placeholders, no RELOCPOST copy exists | coordinator confirms zero RELOCPOST copies in a configured link, then deletes in ~4 chunks of ~60 with a link check each |
| 2 — standalone live | 7 keys / 7 bases (§1.2) | per-unit: declarations-only audit + raw `data diff` + `hexdiff --all` before/after + `main.elf` rebuild | one unit at a time; expect most to need shared-pool `extern` adoption or to stay code-only-labelled, not plain deletion |
| 3 — fused live | 18 rename-live + 17 layout-live + `CProc.o` (content-fused, with C) | coordinate with B (layout fields) and C (content fields); EDS goes last per key | per-key, with the owning category |
| 4 — scoped twins | live `#` keys among batches 2–3 | verify twin selection (`select_unit_rules`) after each deletion | with batch 2–3 |

Predicted end state: 279 → 0 EDS entries; ~40 bases carry an explicit code-only label;
`--ratchet` per batch by the coordinator (counts recomputed centrally).

Batch 2 caveat (static prediction, NOT evidence): MWCC emits `.sdata2` pools for any
float-using TU, so deleting EDS there without a shared-pool reference model will flip
the raw data gate to MISMATCH. Those units are the core reason Option C (policy +
label), not silent deletion, is required. Do not bulk-delete live EDS.

## 4. Link-neutrality evidence for retired entries

**None retired in this worktree — no link-neutrality evidence claimed.**
Attempt count per entry: 0 (after 3 non-improving attempts the brief says move on;
with no build available, no attempts were spent). Candidate batch-1/2 units are
identified statically (§§1.2, 5.3) for an agent with a configured build.

## 5. Kept keys — verdicts

All 279 keys kept. Verdict classes:

- **policy-pending (236):** all latent keys — need the Option C decision + batch-1
  coordinator verification. Includes `s*nd_SoundArchivePlayer.o` (also skipped-WIP).
- **policy-pending (36):** all live keys except below — need Option C + batches 2–4.
- **tooling-limit (7):** STRING-BUG keys — `CDrawGX.o`, `CPartsChange.o`,
  `CMenuGetItemMulti.o`, `g3d_anmchr.o`, `g3d_restev.o`, `g3d_anmtexsrt.o`,
  `g3d_scnmdl.o`. Inert mistyped field; coordinator deletes with a link check.
- **skipped-WIP (1):** `snd_SoundArchivePlayer.o` — live sibling WIP in main checkout;
  untouched. (Other WIP basenames `CItemBoxInfo.o`, `CActorParam.o`,
  `CBattleManager.o`, `snd_SeqSound.o` carry no EDS — confirmed no D action.)
- **paused-owner-signoff (1, no key):** `CErrorWii.o` — §7.

Residuals: (i) live-derivation must be re-run from a configured `build.ninja` via the
README snippet (my live set is the `configure.py`-Matching proxy — 43 keys/40 bases);
(ii) gate evidence for every batch is link-measured, not static; (iii) CONTENT-FUSED
EDS (`CProc.o`, `CScn_80496B0C.o`, `btm_sec.o`, `g3d_state.o`, `lyt_layout.o`,
`lyt_pane.o#Q36nw4hbm3lyt`) needs category-C clearance first.

## 6. Handoffs from category B — D-side disposition

- **15 HANDOFF-D entries** (`CScnItemCameraNw4r.o`, `CScnRootNw4r.o`, `OSReset.o`,
  `bta_sys_main.o`, `btm_devctl.o`, `btm_pm.o`, `dsp.o`, `dvdDeviceError.o`, `e_pow.o`,
  `fs.o`, `hcisu_h2.o`, `rfc_ts_frames.o`, `snd_MemorySoundArchive.o`,
  `snd_MmlSeqTrackAllocator.o`, `yvm2.o`): **none carries an EDS key** (all are
  drop/trim-only). No D mechanism applies; their ownership question (weak
  vtables/RTTI names GC'd by the retail linker vs dissolved-blob blocks) is answered
  by §1.3 in the general case, but each rule is B's to retire. No D action.
- **Mixed packets** (`CfRes.o`, `snd_FxReverbStdDpl2.o`, `CDeviceVI.o`,
  `ut_RomFont.o`, `CModelDispMakeCrystal.o`, `ax_rna.o`, `code_80296898.o`,
  `ut_DvdFileStream.o`, `CDesktop.o`, `code_804B2FF0.o`, `CCol4.o`, plus
  `CView.o`/`CWorkThread.o`/`CTaskManager.o`): **none carries EDS** (verified).
  Ownership questions recorded; no D action until/unless EDS appears.
- **EDS-bearing `set_data_align` keys** (`CETrail.o`, `CScnFilter.o`, `CMdlMouth.o`,
  `CMdlDynamics.o`, `code_804BD8E8.o`, `snd_StrmPlayer.o`): EDS is D's, align field is
  B's (§5 of B doc). All 6 are latent except `CScnFilter.o`/`CMdlMouth.o` (live);
  EDS deletion sequenced after B clears the align field per key.
- **B's executed FYIs** (`CLibCriMoviePlay.o` `.sbss` gap, `CHelp_ClosePartyMenu.o`
  rename): no EDS involvement; noted as the correct unowned-gap model (cf. §1.3).

## 7. Inherited item: `CErrorWii.o` — PAUSED for owner sign-off

State found: no `UNIT_RULES` key (already removed by A). Source already declares
`extern "C" lbl_eu_80665A60/64/65/66` with **no definitions** (`CErrorWii.cpp:22–25`);
retail split block is `.text`-only (data-free, as intended). Owner of the bytes:
`ScheduleList.cpp` (NonMatching) **defines** `lbl_eu_80665A60/64/65/66` (+67) in its
`.sbss` slice 0x80665A50–0x80665A68 (`ScheduleList.cpp:152–159`).

Under the new policy (no `extern "C" lbl_eu_*` spellings to line up reloc names) both
sides need sign-off: the `CErrorWii.cpp` extern declarations **and** the
`ScheduleList.cpp` definitions use the banned spelling pattern. Options for the owner:
(a) grandfather both (smallest diff; ScheduleList must keep the labels defined at
promotion per the existing comment); (b) remove the `CErrorWii.cpp` externs and give
the TU real (non-`lbl_eu_`) static definitions — reintroduces the 7-byte `.sbss`
vs retail-size-0 mismatch at the raw gate; (c) keep externs but record a per-unit
§17.6 exception. **No change made here** per the policy update. Do not touch until
the owner rules.

## 8. Source files changed

None. (`libs/monolib/src/util/CErrorWii.cpp` and `libs/monolib/src/core/ScheduleList.cpp`
were read-only inspected for §7.)

## 9. Claim metadata for the coordinator (single EDS class)

Do NOT `--claim` per entry from here (parent is the sole shared-artifact writer).
Suggested single-class claim values when the coordinator runs it:

| keys | `--class` | `--owner` | `--reason` (suggested) |
|---|---|---|---|
| all 279 EDS keys (list: inventory doc §1 + §1.1 live list) | layout | category-d | `EDS code-only-data: <bucket>; batch <0/1/2/3/4> per decision doc` |

Buckets with counts for the reason strings: STANDALONE-live 7, STANDALONE-latent 22,
FUSED-RENAME-live 18, FUSED-RENAME-latent 159, FUSED-LAYOUT-live 17,
FUSED-LAYOUT-latent 43, CONTENT-FUSED 6, STRING-BUG 7.

## 10. Central actions required (coordinator only)

1. Re-derive live with the README snippet from a configured `build.ninja`; confirm
   the 43-key/40-base live set (or correct it).
2. Record the Option C (+ A-labelling transition) decision in PLAN.md (progress
   definition + §17.6 posture) — owner policy step, blocks batches 1–4.
3. Implement unit kind/status (`MatchingCodeOnly` or `Matching` + `dataOk=false`),
   progress/README/`readme_status.py` reporting, and the declarations-only CI checker
   (category brief steps 2–3, 5).
4. Batch 0: delete/fix the 7 STRING-BUG EDS fields with a link check.
5. Batch 1: verify zero RELOCPOST copies for the 236 latent keys in the live link,
   then delete in ~4 chunks with link checks; `--ratchet` per chunk.
6. Batches 2–4: per-unit `run.py data diff` (raw) + `hexdiff --all` before/after +
   `main.elf` rebuild; `--ratchet` per small batch.
7. Rule on §7 (`CErrorWii.o`/`ScheduleList.cpp` extern spellings) under the new policy.
8. Keep `snd_SoundArchivePlayer.o` parked until its sibling WIP lands.
9. `attempts.jsonl` lines from this subagent: none (no `cycle` runs; static-only).

## Appendix — derivation log (commands, all in-worktree)

- EDS census + buckets: `UNIT_RULES` scan via `tools/postprocess_reloc_names.py`
  (279 with truthy `extern_data_sections`; 7 bare-string).
- Live proxy: `Object(Matching, …)` basenames in `configure.py` (480 Matching objects,
  476 basenames; 43 EDS keys / 40 bases hit).
- Retail sizes: `config/us/splits.txt` slice parse per applicable TU (scoped keys →
  matching twin); 0/279 nonempty applicable slices.
- Label owners: 740 refs → `config/us/symbols.txt` addresses → owning splits block;
  gap majority quantified in §1.3.
- Dissolved-blob proof: `splits.txt` vs `splits.txt.bak_pre_nw4r` (removed headers
  `split1.s`, `nw4r_data.s`; totals in §1.3).
- Untouched: `UNIT_RULES`, `postprocess_reloc_names.frozen.json`, `configure.py`,
  `symbols.txt`, `splits.txt`, `attempts.jsonl`; final `--check` OK.

## 11. Status 2026-10-23 (eds-retire) — §10 actions executed

Owner adopted Option C via A labelling (PLAN.md §17.7). Scope change mid-run (coordinator): the
`.sdata2` EDS keys belong to the split-fix agent (pool-ownership research,
`docs/evidence/pool_ownership_2026-10-23.md`: the "shared pools" are wrong `splits.txt`
boundaries); eds-retire keeps tooling, batch 0, non-`.sdata2` latent keys and live
`.data`/`.rodata`/`.sdata`/`.bss` keys.

**§10.1 live re-derivation (configured `build.ninja`, `.scratch/eds/census.py`):** at start the
table had already shrunk (2026-10-23 burn-down): 319 rules, **247 EDS keys, 39 live / 36 bases,
208 latent, 6 STRING-BUG** (`g3d_restev.o` already retired). The 43/40 proxy count is stale:
`CChainActorList`, `CCur`, `CVision`, `CWorkSystemCache`, `code_800A75FC`, `CTaskGameEffAfter`,
`CMenuQuestLog`, … were retired; `CfTaskMain`, `CDrawGX`, `CScnFrame`, `snd_SoundSystem`,
`lyt_arcResourceAccessor`, `g3d_resanmclr`, `ut_TextWriterBase#Q34nw4r2ut`, `snd_TaskManager`
are live and missing from the proxy list.

**Correction to §1.2/§5 (STRING-BUG is NOT inert):** `extern_data_sections()` tests
`name in sections`; with a bare string that is a *substring* test, so `(".sdata2")` strips
`.data`, `.sdata` and `.sdata2`, and `(".rodata")` strips `.rodata` and `.data`. Harmless for the
5 latent keys; **live for `CDrawGX.o`** (`".sdata2"` string). It cannot be "fixed to a tuple"
(value change breaks the freeze); it is a `.sdata2` key → handed to split-fix.

**§10.3 tooling:** `Object(MatchingCodeOnly, …)` (tools/project.py marker, links like
`Matching`, objdiff category `code_only`); `configure.py progress` prints headline data from
owning TUs only + a separate code-only line; `readme_status.py` adds a code-only row;
`tools/coop/code_only_check.py` = declarations-only checker (CI: build.yml after the build);
`tools/coop/lib/code_only.py` parser; tests `tools/coop/tests/test_code_only.py`.

**Batches (each: `run.py link-gate` OK → `link-gate --ratchet` → `check_unit_rules_frozen --ratchet`; build.ninja unchanged for latent chunks):**

| batch | keys | result |
|---|---|---|
| 0 | `CPartsChange`, `CMenuGetItemMulti`, `g3d_anmchr`, `g3d_anmtexsrt`, `g3d_scnmdl` (latent) | EDS field deleted |
| 1a | 60 latent (55 carry `.sdata2` — ran **before** the scope change; link-neutral, ratcheted, cannot be re-added under the freeze) | 58 fields + 2 whole entries deleted |
| 1b | 38 latent non-`.sdata2` | 29 fields + 9 whole entries deleted |
| live | `lyt_group.o#Q34nw4r3lyt`, `lyt_arcResourceAccessor.o` | `.reloc.o` byte-identical, deleted |
| live | `snd_SoundSystem.o` (`.bss`/`.sbss`, raw data MATCH) | link improved: `lbl_eu_80665508/0C` no longer missing |
| live → code-only | `CfTaskMain`, `bta_hh_utils`, `CLib`, `snd_Task`, `CTaskGamePic` | raw data MISMATCH (source defines data the split doesn't own); EDS kept, relabelled `MatchingCodeOnly`; hexdiff --all unchanged (11/11, 5/5, 6/6, 1/1, 19/19) |

Counts: EDS 247 → **141** (105 latent + 31 live, all `.sdata2`-bearing → split-fix; `CDrawGX`
string; 5 code-only). UNIT_RULES 319 → **308**. link-gate before missing=93 sda_reloc=8 →
after missing=87 (2 from snd_SoundSystem; the rest from parallel agents) sda_reloc=8, 0
multiply-defined/internal.

**§7 ruling proposal (not implemented):** option (a)-prime — keep `CErrorWii.cpp`'s
declarations (it owns no data; retail split is `.text`-only, which is exactly the §17.7
declarations-only invariant), but rename the spelling once `ScheduleList.cpp` is matched: the
`.sbss` bytes 0x80665A60–66 belong to ScheduleList's slice, so ScheduleList should define them
under real (static member / file-scope) names and CErrorWii should reach them through
ScheduleList's header. Until then grandfather both `lbl_eu_*` spellings as listed cleanup debt
(no new ones); no §17.6 exception needed.
