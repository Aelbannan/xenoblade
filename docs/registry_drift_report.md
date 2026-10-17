# Registry drift report — accepted targets vs decomp-object reality

Date: 2026-10-12 (UTC). Author: registry-hygiene agent (scope: 75 not-in-object rows).
Status: **hand-off for the later demotion pass — no registry edits made** (registry is
owned by another agent; see §6).

## 1. What was audited

- **Bucket N (authoritative, in-scope): 75 ACCEPTED targets whose symbol is not present
  in the decomp object.** Row list per coordinator brief (unit | symbol). Every row
  verified against: retail `config/us/symbols.txt` + `config/us/splits.txt` (retail truth),
  current decomp source (`src/…`, `libs/…`), and on-disk decomp objects (`build/us/src/…`,
  `build/us/obj/…`) via `nm` plus read-only `extract_function_pair`
  (`tools/ppc_equivalence/elf_symbols.py`, the same resolver the sweep uses).
- **Bucket R (regressed): coordinator's 160-row list was not attached to this task.**
  §5 encloses the reproducible as-of snapshot for the demotion pass to merge:
  48 non-overlapping sweep-stale FULL_MATCH rows (byte-identity, current objects),
  the 159 zero-score FULL_MATCH census by unit, and regeneration commands.
  4 rows overlap both buckets (byte-mismatch AND symbol-missing); they are counted in
  bucket N (§3) and flagged in §5.

## 2. As-of state (read this before demoting anything)

- **Two TUs in bucket N do not compile right now** (concurrent source wave mid-edit;
  verified via sanctioned `hexdiff.py`, which owns the repo-wide lock; failed builds do
  not clobber the last-good objects — Sep timestamps intact):
  - `src/kyoshin/cf/object/CfObjectModel.cpp` — MWCC 10211 (goto past init, :1022) +
    10197 (illegal overloading, :1060). Last-good object: Sep 17.
  - `src/kyoshin/menu/CMenuBattlePlayerState.cpp` — MWCC 10141 (expression syntax, :2073).
    Last-good object: Sep 19.
  - Consequence: all 44 CfObjectModel + all 10 CMenuBattlePlayerState ACCEPTED rows
    (54/75) are un-emittable until the wave finishes. **54 rows are HOLD, not demote.**
- Objects for the other 10 units are **fresh (Oct 12 ~23:25–23:32 UTC)**, so `nm`
  absence there is current ground truth.
- `tools/coop/registry_check.py`: **passed** (schema OK, README block current).
  `run.py targets validate`: 46 pre-existing `equivalence_certificate` errors
  (version/engine-hash drift on other owners' EQUIVALENT_MATCH rows) — untouched,
  out of scope. `targets sync-attempts` was **not** run per brief.

## 3. Bucket N — per-row classification and recommendation

Classes: (a) boot/asm · (b) template instantiation · (c) adjustor thunk ·
(d) static init/data · (e) mis-registered · (f) genuinely missing source
(the retail-named symbol has no source-produced definition — incl. linkage-drifted
definitions, mechanism column keeps the distinction).

Recommendations: **DEMOTE** (later pass demotes; bogus/stale acceptance) ·
**HOLD** (real row, owner in-flight work; do NOT demote) ·
**KEEP** (valid; re-verify only) · **RENAME** (fix symbol spelling, no demotion).

### 3.1 RVL_SDK boot (a) — 5 rows

Retail truth: `__start` is `.init, scope:weak` (global); the other four are
`.init, scope:local`. Decomp (`libs/RVL_SDK/src/revolution/os/__start.c`, fresh object):
`__start` = `W` (weak), the four helpers = `static` → local `t`. Local symbols can never
be global match targets; the four ACCEPTED/FULL_MATCH rows are bogus.

| unit | symbol | target id | class | mechanism | recommendation |
|---|---|---|---|---|---|
| RVL_SDK/src/revolution/os/__start | `__check_pad3` | us-800062c0 | (a) | retail-local + decomp-static | DEMOTE |
| RVL_SDK/src/revolution/os/__start | `__set_debug_bba` | us-800062f0 | (a) | retail-local + decomp-static | DEMOTE |
| RVL_SDK/src/revolution/os/__start | `__get_debug_bba` | us-80006300 | (a) | retail-local + decomp-static | DEMOTE |
| RVL_SDK/src/revolution/os/__start | `__start` | us-80006310 | (a) | weak asm boot vector; resolves | KEEP (re-verify via hexdiff post-wave) |
| RVL_SDK/src/revolution/os/__start | `__init_data` | us-80006510 | (a) | retail-local + decomp-static | DEMOTE |

### 3.2 Singletons (e)/(f) — 6 rows (retail unit mapping verified against splits.txt
`.text` ranges for all six; mappings are correct, emissions are not)

| unit | symbol | target id | reg. match | class | evidence | recommendation |
|---|---|---|---|---|---|---|
| kyoshin/CCollepedia | `func_80254204` | us-80256440 | EQUIV 98.6% | (f) | only an `extern "C"` prototype (:34) + 2 call sites; **no definition repo-wide** | DEMOTE |
| kyoshin/CPcKizunagram | `__ct__CPcKizunagram` | us-8025f8e0 | FULL 100% | (f) | no bare-symbol def; TU defines C++ `CPcKizunagram::CPcKizunagram()` (:152) which mangles differently; retail name is unmangled (cf. sibling `__ct__CPcKizunaCur` pattern, :127–129) | HOLD (owner: add `extern "C"` wrapper per file pattern; do NOT demote) |
| kyoshin/cf/CTaskCulling | `getOccBuffer__Q22cf12CTaskCullingFv` | task-culling-func-801A2C04 | FULL 100% | (e) | decomp src object **does** emit `getOccBuffer__Q22cf12CTaskCullingFv` (C++ member def :92); **retail** name at 0x801A4324 is `func_801A2C04__Q22cf12CTaskCullingFv` (target id itself still says `func-801A2C04`) — registry-side stale rename | RENAME registry symbol to retail spelling via `targets sync-symbols` (or rename retail symbol); no demotion |
| kyoshin/cf/CfResReloadImpl | `isReloadTypeOk` | us-8016e198 | EQUIV 88.6% | (f) | virtual declared in hpp (:103, slot 0x14) only; class is `__declspec(novtable)` so the missing def is link-silent; **no definition repo-wide** | DEMOTE |
| kyoshin/cf/CfScript | `format__Q22ml10FixStr<128>FPCce` | us-8006a014 | FULL 100% | (b) | source has comment only; fresh object defines `format__Q22ml11FixStr<128>FPCce` (**ml11** vs retail **ml10**) and merely references `U format__Q22ml10FixStr<64>FPCce` — template-declaration mangling drift in `FixStr.hpp`, TU mid-flight | HOLD (owner scope; do NOT demote) |
| kyoshin/code_800B06A4 | `emitSpeedEvent` | us-800b6e1c | EQUIV 73.9% | (f) | comment + 1 call site (:5221); **no definition repo-wide** | DEMOTE |

### 3.3 CfObjectModel vtable/thunk family (f) — 44 rows (ALL 44 are the unit's full
ACCEPTED set; TU non-compiling, see §2)

Per-definition census of current source: 7 `extern "C"` definitions (emit the bare
retail name once the TU compiles) · 3 prototype-only with definitions **lost**
(repo-wide: prototypes + vtable `(const void*)` refs only) · 34 C++-linkage definitions
(emit `__F…`-suffixed names, e.g. stale object shows `CfModel_ReleaseAll__FPQ22cf13CfObjectModel`
for retail-bare `CfModel_ReleaseAll`; need `extern "C"` — the wave is mid-conversion).
Every registry symbol matches retail `symbols.txt` exactly: the rows are correct,
the TU is in-flight. **HOLD all 44 — do NOT demote.**

| symbol | target id | mechanism | recommendation |
|---|---|---|---|
| `__dt__Q22cf13CfObjectModelFv` | us-800bb268 | `DEF+extC` (:157), exact in last-good object | HOLD |
| `__dt__800BAA24` | us-800bb344 | C++ def (:200) emits `__dt__800BAA24__FPUcl`; needs `extern "C"` | HOLD |
| `CfObjectModel_UnkVirtualFunc1__Q22cf13CfObjectModelFv` | us-800bb43c | prototype-only (:1148) + vtable refs; def lost (was exact in Sep-17 object) | HOLD (owner restores def) |
| `CfModel_ReleaseAll` | us-800bb484 | C++ def (:259) emits suffixed; needs `extern "C"` | HOLD |
| `CfObject_notifyEventDone__Q22cf13CfObjectModelFv` | us-800bb4dc | C++ def emits suffixed; needs `extern "C"` | HOLD |
| `CfObject_UnkVirtualFunc63__Q22cf8CfObjectFv` | us-800bb594 | C++ def emits suffixed; needs `extern "C"` | HOLD |
| `CfObject_UnkVirtualFunc63__Q22cf13CfObjectModelFv` | us-800bb5fc | C++ def emits suffixed; needs `extern "C"` | HOLD |
| `CfObj_KindBit1` | us-800bb6b8 | C++ def emits `__FPQ22cf8CfObject`; needs `extern "C"` | HOLD |
| `CfObj_KindBit2` | us-800bb6e8 | C++ def emits suffixed; needs `extern "C"` | HOLD |
| `CfObj_KindBit3` | us-800bb718 | C++ def emits suffixed; needs `extern "C"` | HOLD |
| `CfObj_KindBit7` | us-800bb748 | C++ def emits suffixed; needs `extern "C"` | HOLD |
| `CfObject_setMoveTargetVec__Q22cf13CfObjectModelFv` | us-800bb778 | C++ def emits suffixed; needs `extern "C"` | HOLD |
| `CfObject_UnkVirtualFunc22__Q22cf13CfObjectModelFv` | us-800bb7f4 | C++ def emits suffixed; needs `extern "C"` | HOLD |
| `CfObject_getPosVector__Q22cf13CfObjectModelFv` | us-800bba4c | prototype-only (:1142) + vtable refs; def lost (was exact in Sep-17 object) | HOLD (owner restores def) |
| `CfObject_UnkVirtualFunc24__Q22cf13CfObjectModelFv` | us-800bba88 | C++ def emits `__FPv`; needs `extern "C"` | HOLD |
| `CfObject_UnkVirtualFunc28__Q22cf13CfObjectModelFv` | us-800bbb20 | C++ def emits `__FPv`; needs `extern "C"` | HOLD |
| `CfObject_setMoveHeadAngle__Q22cf13CfObjectModelFv` | us-800bbb5c | C++ def emits suffixed; needs `extern "C"` | HOLD |
| `CfObject_getMoveHeadAngle__Q22cf13CfObjectModelFv` | us-800bbbb0 | C++ def emits `__FPv`; needs `extern "C"` | HOLD |
| `CfObject_UnkVirtualFunc34__Q22cf13CfObjectModelFv` | us-800bbc04 | prototype-only (:1145) + vtable refs (also prototyped by CfResTboxImpl/code_800C17DC/CREvtModel consumers); def lost | HOLD (owner restores def) |
| `CfObject_UnkVirtualFunc35__Q22cf13CfObjectModelFv` | us-800bbc38 | C++ def emits `__FPvf`; needs `extern "C"` | HOLD |
| `CfObject_UnkVirtualFunc36__Q22cf13CfObjectModelFv` | us-800bbc58 | C++ def emits `__FPv`; needs `extern "C"` | HOLD |
| `CfModel_GetEffOwner` | us-800bbc60 | C++ def emits suffixed; needs `extern "C"` | HOLD |
| `CfObject_UnkVirtualFunc54__Q22cf13CfObjectModelFv` | us-800bbe1c | C++ def emits suffixed; needs `extern "C"` | HOLD |
| `CfObjectModel_UnkVirtualFunc3__Q22cf13CfObjectModelFv` | us-800bbf10 | `DEF+extC` (:714), exact in last-good object | HOLD |
| `CfModel_SyncVisFlag` | us-800bbf38 | C++ def emits suffixed; needs `extern "C"` | HOLD |
| `CfObject_UnkVirtualFunc66__Q22cf13CfObjectModelFv` | us-800bbf5c | C++ def emits suffixed; needs `extern "C"` | HOLD |
| `CfObject_UnkVirtualFunc67__Q22cf13CfObjectModelFv` | us-800bbfa4 | C++ def emits suffixed; needs `extern "C"` | HOLD |
| `CfObject_UnkVirtualFunc70__Q22cf13CfObjectModelFv` | us-800bc028 | C++ def emits suffixed; needs `extern "C"` | HOLD |
| `CfObject_UnkVirtualFunc72__Q22cf13CfObjectModelFv` | us-800bc050 | C++ def emits `__FPvf`; needs `extern "C"` | HOLD |
| `CfObjectModel_UnkVirtualFunc10__Q22cf13CfObjectModelFv` | us-800bc058 | C++ def emits `__FPvf`; needs `extern "C"` | HOLD |
| `CfObjectModel_UnkVirtualFunc12__Q22cf13CfObjectModelFv` | us-800bc060 | C++ def emits `__FPvf`; needs `extern "C"` | HOLD |
| `CfObjectModel_UnkVirtualFunc13__Q22cf13CfObjectModelFv` | us-800bc068 | C++ def emits suffixed; needs `extern "C"` | HOLD |
| `CfObjectModel_UnkVirtualFunc15__Q22cf13CfObjectModelFv` | us-800bc234 | `DEF+extC` (:846), exact in last-good object | HOLD |
| `CfModel_GetFlag7A8` | us-800bc254 | `DEF+extC` (:854), exact in last-good object | HOLD |
| `CfObject_UnkVirtualFunc69__Q22cf13CfObjectModelFv` | us-800bc274 | `DEF+extC` (:863), exact in last-good object | HOLD |
| `CfObject_UnkVirtualFunc68__Q22cf13CfObjectModelFv` | us-800bc294 | `DEF+extC` (:878), exact in last-good object | HOLD |
| `CfModel_NotifyReady` | us-800bc328 | C++ def emits suffixed; needs `extern "C"` | HOLD |
| `CfModel_GetSpeedRate` | us-800bc39c | C++ def emits suffixed; needs `extern "C"` | HOLD |
| `CfObjectModel_UnkVirtualFunc8__Q22cf13CfObjectModelFv` | us-800bc3bc | `DEF+extC` (:920), exact in last-good object | HOLD |
| `CfObjectModel_UnkVirtualFunc7__Q22cf13CfObjectModelFv` | us-800bc3dc | C++ def emits suffixed; needs `extern "C"` | HOLD |
| `CfModel_InstallSub` | us-800bc3fc | C++ def emits suffixed; needs `extern "C"` | HOLD |
| `CfModel_ReattachTrg` | us-800bc470 | C++ def emits suffixed; needs `extern "C"` | HOLD |
| `CfModel_UpdateBdat` | us-800bc524 | C++ def `void CfModel_UpdateBdat()` (:1005, zero params) emits `__Fv`; needs `extern "C"` (arity vs retail is owner's call) | HOLD |
| `CfModel_GetBdatString` | us-800bc528 | C++ def (:1009) emits `__FUs`; needs `extern "C"` | HOLD |

### 3.4 CMenuBattlePlayerState — 10 rows (ALL 10 are the unit's full ACCEPTED set;
TU non-compiling, see §2; 9/10 exact in Sep-19 last-good object). Class split:
`__dt__8010B444` + `sinit_8010E9F8` → (d); the other 8 → (f). All 10 HOLD.

| symbol | target id | mechanism | recommendation |
|---|---|---|---|
| `__dt__8010B444` | us-8010bf20 | `extern "C"` array-element dtor def (:56); exact in last-good object | HOLD |
| `__dt__22CMenuBattlePlayerStateFv` | us-8010bf60 | `extern "C"` dtor def (:1086); exact in last-good object | HOLD |
| `BpsStateCreateSingleton` | us-8010d8a8 | C++ def (:1062) emits `BpsStateCreateSingleton__FP8CProcessP4CScn`; needs `extern "C"` | HOLD |
| `BpsStateSingletonToInt` | us-8010d924 | `extern "C"` def (:1018); exact in last-good object | HOLD |
| `BpsStateFlag7C9Set` | us-8010da38 | `extern "C"` def (:1019); exact in last-good object | HOLD |
| `BpsStateSelectSlotAnim` | us-8010dbb0 | `extern "C"` def (:1123); exact in last-good object | HOLD |
| `sinit_8010E9F8` | us-8010f4d4 | `extern "C"` static-init def (:1022); exact in last-good object | HOLD |
| `BpsStateDtorThunk58` | us-8010f528 | `extern "C"` this-adjustor def (:1026, `subi r3,r3,0x58` → dtor); exact in last-good object | HOLD |
| `BpsStateRenderThunk5C` | us-8010f530 | `extern "C"` def (:1032); exact in last-good object | HOLD |
| `BpsStateDtorThunk5C` | us-8010f538 | `extern "C"` this-adjustor def (:1038, `subi r3,r3,0x5c` → dtor); exact in last-good object | HOLD |

### 3.5 nw4r template/thunk/static-init (b)/(c)/(d) — 10 rows (fresh Oct-12 objects;
findings are current)

| unit | symbol | target id | reg. match | class | evidence | recommendation |
|---|---|---|---|---|---|---|
| nw4r/src/snd/snd_SoundArchivePlayer | `__dt__Q34nw4r2ut41LinkList<Q44nw4r3snd6detail8SeqSound,240>Fv` | us-804200c0 | FULL 0.0% | (b) | object emits only weak `__dt__…SeqSound,236>Fv` (template offset arg 236 ≠ retail 240 — struct-layout drift); no 240 instantiation anywhere; source never names the symbols (grep 0) | DEMOTE (re-register after layout fix if still emitted) |
| nw4r/src/snd/snd_SoundArchivePlayer | `__ct__Q34nw4r2ut42LinkList<Q44nw4r3snd6detail9StrmSound,240>Fv` | us-80420118 | FULL 0.0% | (b) | object emits only weak `__ct__…StrmSound,236>Fv` | DEMOTE |
| nw4r/src/snd/snd_SoundArchivePlayer | `__dt__Q34nw4r2ut42LinkList<Q44nw4r3snd6detail9StrmSound,240>Fv` | us-80420138 | FULL 0.0% | (b) | object emits only weak `__dt__…StrmSound,236>Fv` | DEMOTE |
| nw4r/src/snd/snd_SoundArchivePlayer | `__ct__Q34nw4r2ut42LinkList<Q44nw4r3snd6detail9WaveSound,240>Fv` | us-80420190 | FULL 0.0% | (b) | object emits only weak `__ct__…WaveSound,236>Fv` | DEMOTE |
| nw4r/src/snd/snd_SoundArchivePlayer | `__dt__Q34nw4r2ut42LinkList<Q44nw4r3snd6detail9WaveSound,240>Fv` | us-804201b0 | FULL 0.0% | (b) | object emits only weak `__dt__…WaveSound,236>Fv` | DEMOTE |
| nw4r/src/snd/snd_SoundPlayer | `__arraydtor$4226` | us-80422d84 | FULL 100% | (d) | source names nothing (grep 0); object emits only local `t __arraydtor$5907` (different fragment number — these renumber on every source edit; never a stable target) | DEMOTE |
| nw4r/src/snd/snd_StrmPlayer | `@208@OnShutdownSoundThread__Q44nw4r3snd6detail10StrmPlayerFv` | us-80427300 | FULL 0.0% | (c) | object emits weak `@212@…` (thunk this-delta 212 ≠ retail 208 — same layout drift as (b)); source never names them | DEMOTE |
| nw4r/src/snd/snd_StrmPlayer | `@208@OnUpdateVoiceSoundThread__Q44nw4r3snd6detail10StrmPlayerFv` | us-80427308 | FULL 0.0% | (c) | weak `@212@…` only | DEMOTE |
| nw4r/src/snd/snd_StrmPlayer | `@208@OnUpdateFrameSoundThread__Q44nw4r3snd6detail10StrmPlayerFv` | us-80427310 | FULL 0.0% | (c) | weak `@212@…` only | DEMOTE |
| nw4r/src/snd/snd_StrmPlayer | `@208@__dt__Q44nw4r3snd6detail10StrmPlayerFv` | us-80427318 | FULL 0.0% | (c) | weak `@212@…` only | DEMOTE |

### 3.6 Bucket-N counts

| class | rows | DEMOTE | HOLD | KEEP | RENAME |
|---|---|---|---|---|---|
| (a) boot/asm | 5 | 4 | 0 | 1 (`__start`) | 0 |
| (b) template instantiation | 6 | 5 (LinkList) | 1 (format FixStr) | 0 | 0 |
| (c) adjustor thunk | 4 | 4 | 0 | 0 | 0 |
| (d) static init/data | 3 | 1 (`__arraydtor$4226`) | 2 (BPS `__dt__8010B444`, `sinit_8010E9F8`) | 0 | 0 |
| (e) mis-registered | 1 | 0 | 0 | 0 | 1 (getOccBuffer) |
| (f) genuinely missing source | 56 | 3 (singletons: `func_80254204`, `isReloadTypeOk`, `emitSpeedEvent`) | 53 (44 CFM + 8 BPS + `__ct__CPcKizunagram`) | 0 | 0 |
| **total** | **75** | **17** | **56** | **1** | **1** |

Check: DEMOTE 4+5+4+1+3 = 17; HOLD 0+1+0+2+53 = 56; 17+56+1+1 = 75. ✓

Note on (f)/HOLD: the 53 HOLD rows have no current retail-named definition, but their
body (or an adjacent body one `extern "C"` line away) exists in the owning TU — owner
in-flight work (linkage conversion, lost-def restore, ctor wrapper), not demotion material.

## 4. Recommended registry actions (for the demotion-pass owner)

1. **DEMOTE 17 rows** (§3.6) ACCEPTED → NOT_STARTED/BACKLOG (sweep convention), clearing
   stale `full-instruction-match` certs/sidecar entries. Never re-accept compiler-emitted
   rows (weak LinkList instantiations, `@NNN@` thunks, `__arraydtor$N`, local boot
   helpers) as standalone targets — they are covered by owning-TU/unit-level matching.
2. **RENAME 1 row**: `task-culling-func-801A2C04` symbol → retail
   `func_801A2C04__Q22cf12CTaskCullingFv` (`targets sync-symbols`), or rename the retail
   symbol to `getOccBuffer__Q22cf12CTaskCullingFv` via `symbols rename-plan/rename-all`.
3. **HOLD 56 + KEEP 1**: no registry change. Re-run `hexdiff <unit> --all` for
   CfObjectModel / CMenuBattlePlayerState / CPcKizunagram / CfScript once the wave's TUs
   compile; the HOLD rows should resolve (or surface precise residuals) then.
4. Do NOT bulk-demote by unit: CFM/BPS ACCEPTED sets are 100% in bucket N only because
   their TUs are mid-edit, not because the rows are wrong.

## 5. Bucket R — regressed snapshot (merge with the coordinator's 160-row list)

Method: read-only `sweep_stale_full_match.py --dry-run` over all 14184 FULL_MATCH rows
(current on-disk objects, no rebuild) + zero-score census. The coordinator's exact
160-row audit (fresh builds at audit time) could not be reproduced 1:1 — the wave has
since rebuilt most objects and broken two TUs — so this section is the merge base, not
a replacement. Overlap with bucket N: 4 rows (`us-800bb268`, `us-800bb5fc`, `us-800bc524`,
`us-8010dbb0` — resolve in stale objects but bodies differ; primary handling stays §3).

### 5.1 Sweep-stale FULL_MATCH rows, excluding bucket-N overlap: 48 rows → DEMOTE (later pass)

| target id | unit (short) |
|---|---|
| us-80039a64 | CGame |
| us-80046f80 | pluginEve |
| us-800474f8 | pluginEve |
| us-800475fc | pluginEve |
| us-8004aa44 | pluginCfs |
| us-8004ac34 | pluginCfs |
| us-80095e28 | CtrlNpc |
| us-800ad138 | CfObjectEff |
| us-800ad3b4 | CfObjectEff |
| us-800ad3d4 | CfObjectEff |
| us-800c2a00 | CfObjectImplWalker |
| us-801729a4 | CfObjectActor |
| us-80172ab4 | CfObjectActor |
| us-8017cdb0 | CActorParam |
| us-8017d83c | CActorParam |
| us-8017e644 | CActorParam |
| us-8017ed30 | CActorParam |
| us-801876a8 | pluginBtl |
| us-801c5190 | CREvtLight |
| us-801c53e8 | CBgTex |
| us-801c55d8 | CBgTex |
| us-801c5968 | CTitleAHelp |
| us-801f55b8 | CScrollBar |
| us-801f561c | CScrollBar |
| us-801faa18 | CfObjectTbox |
| us-80223c1c | CModelDispMakeCrystal |
| us-8022616c | CMCEffStart |
| us-8022652c | CMCEffStart |
| us-80226a8c | CMCEffStart |
| us-80227b40 | CMCEffStart |
| us-8025e494 | CKizunagram |
| us-802a1100 | CMenuSkipTimer |
| us-802a71ac | CVS_THREAD_BATTLE_END |
| us-802ae038 | CVS_THREAD_BATTLE_END_SP |
| us-802b24f0 | CVS_THREAD_BATTLE_BEGIN |
| us-802b2738 | CVS_THREAD_BATTLE_BEGIN |
| us-802e247c | bta_dm_act |
| us-803270f0 | HBMBase |
| us-80354fb0 | OS |
| us-8035e0b0 | OSThread |
| us-80367a10 | vi |
| us-803a6168 | mwsfdsvr |
| us-803a6e80 | mwsfdsst |
| us-804185a8 | snd_FxDelayDpl2 |
| us-80446718 | CWorkRoot |
| us-804def98 | CNReqtaskSave |
| us-804e2e2c | CPackItem |
| us-804eb0b0 | CMdlAnmUV |

(Full per-row symbols: re-derive with the command in §5.3 — each row is a standard
non-byte-identical body, owning-TU scope, demote-and-re-earn.)

### 5.2 Zero-score FULL_MATCH census: 159 rows (includes bucket-N's nine 0.0% nw4r rows)

Top units: monolib/src/lod/LODMemMan 27 · nw4r/src/lyt/lyt_material 10 ·
kyoshin/cf/CfHikariItemManager 8 · monolib/src/mpfsys/UnkClass_80471EC8 8 ·
kyoshin/menu/CMenuTutorialList 7 · nw4r/src/snd/snd_SoundArchivePlayer 6 (5 in §3.5 + 1) ·
nw4r/src/snd/snd_StrmPlayer 5 (4 in §3.5 + 1) · CVS_THREAD_VISION_TELL 4 · CGXCache 4 ·
CMdlDynamics 4 · (remainder ≤3 per unit across nw4r g3d/lyt/monomlib scn — see
/tmp/zerosc.txt for the full by-unit table). Zero-score + FULL_MATCH + ACCEPTED is
self-contradictory (0% was never a match); recommend the demotion pass re-verify each
with fresh builds, then demote the still-zero rows. Do NOT conflate with §5.1 (those
have nonzero stale scores).

### 5.3 Regeneration commands (post-wave, for the demotion-pass owner)

```bash
.venv/bin/python3 tools/coop/sweep_stale_full_match.py --dry-run   # §5.1 list
.venv/bin/python3 tools/coop/run.py targets audit-promotion        # 227 EQUIVALENT_MATCH revalidation axis (separate)
.venv/bin/python3 tools/coop/hexdiff.py <unit> --all               # per-unit re-verify (owns the build lock)
```

## 6. Registry edits made

**None.** Per task scope the registry is owned by another agent; this report is the
hand-off. No source files touched. No `targets sync-attempts`. No `ninja`/`configure.py`
(only sanctioned `hexdiff.py` builds, which failed on the two broken TUs without
clobbering last-good objects). `registry_check.py` passes; `targets validate` shows only
the 46 pre-existing certificate errors noted in §2. Accordingly no `attempts.jsonl`
lines were appended (logging rule is per registry change).
