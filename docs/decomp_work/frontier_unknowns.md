# Frontier Unknown Symbols Report

Generated from `tools/coop/frontier_refs.py report --top 50`.

## Summary

| Metric | Value |
|--------|-------|
| Ready frontier size (NOT_STARTED) | 3,398 |
| Unique unknown symbols referenced | 353 |
| Total reference occurrences | 834 |
| Top 50 coverage | 36.7% of all refs |
| UnkClass_* entries | 1 |
| lbl_* entries | 31 |
| func_* entries | 321 |

## Approach

The ready frontier comprises NOT_STARTED functions that are either **leaves** (no
called functions, no unresolved calls, no indirect calls) or **callees-accepted**
(all direct callees are FULL_MATCH or EQUIVALENT_MATCH).  For each frontier
function we locate its retail asm section by matching the function's address
against `# .text:offset | 0xADDRESS | size: 0xSIZE` headers in the per-unit
`.s` file.  Lines between consecutive section headers are scanned for
instruction operands referencing `UnkClass_*`, `lbl_*`, or `func_*` symbols
(8-hex-digit address form).  Unknown symbols are ranked by the number of
distinct frontier functions that reference them.

## Top 50 Unknown Symbols

| # | Symbol | Address | Ref Count | Frontier Funcs | Referencing Units |
|---|--------|---------|-----------|----------------|-------------------|
| 1 | `func_801C4124` | 0x801C4124 | 31 | 31 | CPartyStateWin, CMakeCrystalWin, CMenuItemExchange, …(+10) |
| 2 | `func_80062028` | 0x80062028 | 25 | 17 | CfRes |
| 3 | `func_8022B8A8` | 0x8022B8A8 | 22 | 14 | CCollepedia, CEquipItemBox, CFloorMap, …(+5) |
| 4 | `func_801F3BE0` | 0x801F3BE0 | 11 | 10 | CfGimmick |
| 5 | `func_801F3660` | 0x801F3660 | 8 | 8 | CFloorMap, CItemBoxLine, CKizunaTalkList, …(+5) |
| 6 | `func_8022B8A0` | 0x8022B8A0 | 8 | 8 | CCollepedia, CItemBoxLine, COption, …(+5) |
| 7 | `func_80082D54` | 0x80082D54 | 7 | 6 | CChainActorPc, CCharVoiceMan, CMenuBattleChain, …(+1) |
| 8 | `func_8004B3F0` | 0x8004B3F0 | 26 | 5 | CfCam |
| 9 | `func_80063278` | 0x80063278 | 13 | 5 | CfRes |
| 10 | `func_80063284` | 0x80063284 | 5 | 5 | CfRes |
| 11 | `func_801ED800` | 0x801ED800 | 5 | 5 | CMenuItemExchange, CMenuShopBuy |
| 12 | `func_8022B8B0` | 0x8022B8B0 | 5 | 5 | CSkipTimer, CUIErrMesWin, CModelDispMakeCrystal |
| 13 | `func_800822F4` | 0x800822F4 | 5 | 5 | CVS_THREAD_HP |
| 14 | `func_8004B3D8` | 0x8004B3D8 | 5 | 5 | CScnFilterMan, CScnVirtualLight |
| 15 | `func_800631F0` | 0x800631F0 | 9 | 4 | CfRes |
| 16 | `func_80086DBC` | 0x80086DBC | 4 | 4 | CCharEffect, CTaskREvtSequence, pluginTime |
| 17 | `func_801CB0F4` | 0x801CB0F4 | 4 | 4 | CMenuItemExchange, CMenuShopSell |
| 18 | `func_801D3320` | 0x801D3320 | 4 | 4 | CEquipItemBox, CItemBoxGrid, CTutorialList |
| 19 | `func_8006C6B4` | 0x8006C6B4 | 6 | 3 | CfCam |
| 20 | `setMemInitFlag` | 0x80434A4C | 5 | 3 | CLibCriMoviePlay, Class_8045F858, UnkClass_8045F564 |
| 21 | `func_800AA318` | 0x800AA318 | 4 | 3 | CTaskREvtSequence, CCharVoice, CREvtModelPc |
| 22 | `func_80086DA0` | 0x80086DA0 | 3 | 3 | ocCfp, pluginTime |
| 23 | `func_8006BFC4` | 0x8006BFC4 | 3 | 3 | CfCam |
| 24 | `func_80227CCC` | 0x80227CCC | 3 | 3 | CMenuQuestLog |
| 25 | `func_8003B1EC` | 0x8003B1EC | 3 | 3 | CFloorMap, CfBdat, code_801862C0 |
| 26 | `func_801EB020` | 0x801EB020 | 3 | 3 | CItemBoxGrid, CItemBoxLine |
| 27 | `func_8044BE24` | 0x8044BE24 | 3 | 3 | code_804F0258 |
| 28 | `func_8044BE1C` | 0x8044BE1C | 3 | 3 | code_804F0258 |
| 29 | `func_801C4B60` | 0x801C4B60 | 18 | 2 | CKizunaTalkList, CKizunagram |
| 30 | `func_800596AC` | 0x800596AC | 5 | 2 | CfCam |
| 31 | `func_800631DC` | 0x800631DC | 4 | 2 | CfRes |
| 32 | `func_800407C8` | 0x800407C8 | 4 | 2 | CTaskGamePic, CScnVirtualLight |
| 33 | `lbl_80518A28` | 0x80518A28 | 4 | 2 | lyt_animation |
| 34 | **`CLODCacheManagerS`** | **0x804630C0** | **4** | **2** | **CLODCacheManagerS** |
| 35 | `func_80065804` | 0x80065804 | 3 | 2 | CfRes |
| 36 | `func_8006BF14` | 0x8006BF14 | 3 | 2 | CfCam |
| 37 | `func_8013EC58` | 0x8013EC58 | 2 | 2 | ocUnit, pluginUi |
| 38 | `func_800442DC` | 0x800442DC | 2 | 2 | CTaskGame |
| 39 | `func_8009ECB0` | 0x8009ECB0 | 2 | 2 | CChainActorPc, pluginCfs |
| 40 | `func_8006348C` | 0x8006348C | 2 | 2 | CfRes |
| 41 | `func_80063494` | 0x80063494 | 2 | 2 | CfRes |
| 42 | `func_8006354C` | 0x8006354C | 2 | 2 | CfRes |
| 43 | `func_80065818` | 0x80065818 | 2 | 2 | CfRes |
| 44 | `func_8006E5A4` | 0x8006E5A4 | 2 | 2 | CfCam, CfGameManager |
| 45 | `func_80080F40` | 0x80080F40 | 2 | 2 | CfGameManager, code_8018F8D8 |
| 46 | `func_80086B14` | 0x80086B14 | 2 | 2 | CfObjectEff |
| 47 | `func_800B2D88` | 0x800B2D88 | 2 | 2 | code_800B06A4 |
| 48 | `func_80069EA0` | 0x80069EA0 | 2 | 2 | CfObjectMove |
| 49 | `func_8003AA34` | 0x8003AA34 | 2 | 2 | CPartsChange, CfBdat |
| 50 | `func_8003B41C` | 0x8003B41C | 2 | 2 | CfBdat, code_801862C0 |

## UnkClass_* Analysis

Only **one** `UnkClass_*` symbol is referenced by the ready frontier:

### `CLODCacheManagerS` (rank #34)

| Property | Value |
|----------|-------|
| Address | 0x804630C0 |
| Namespace | `LOD` |
| Methods | 5 (all `Fv` — no params) |
| Ref Count | 4 (from 2 frontier functions) |
| Ref Functions | `us-8046765c`, `us-8046766c` |
| Translation Unit | `monolib/src/lod/CLODCacheManagerS.cpp` |
| Header | None generated yet (scaffolded) |
| configure.py | `Object(NonMatching, "monolib/src/lod/CLODCacheManagerS.cpp")` |

**Demangled methods:**
- `initLodTables` — likely constructor; stores computed pointer to static
- `func_80463118` — reads u16 at `this+0x0`, converts to float
- `func_8046323C` — same pattern as above
- `func_8046339C` — multiple params, u16-to-float conversion
- `func_80463590` — same u16-to-float pattern

**Proposed semantic name: `CLODCacheManagerS`** (17 chars, same-length)

Rationale: the class lives in the `LOD` namespace alongside `CTaskLOD` and
`LODMemMan`. Its methods all convert stored u16 values to float, suggesting a
configuration/parameter cache for LOD levels. Name length is exactly 17
characters, matching `CLODCacheManagerS` for mangling-compatible rename.

**Rename plan verification:**
```
python3 tools/coop/run.py symbols rename-plan CLODCacheManagerS CLODCacheManagerS --verbose
```
→ PASS: mangling-compatible (both 17 chars), 15 symbol lines to change across 3 regions.

### Ready-to-apply rename command (NOT executed — dry-run only):

```
python3 tools/coop/run.py symbols rename-all CLODCacheManagerS CLODCacheManagerS --dry-run
```

When WS-2 completes and exclusive access is released, apply with:

```
python3 tools/coop/run.py symbols rename-all CLODCacheManagerS CLODCacheManagerS
```

## Observations

1. **The frontier is clean of heavy unknown-type dependencies.** Only 1
   `UnkClass_*` symbol appears, and it's referenced by only 2 frontier
   functions. Most unknowns are `func_*` symbols — functions that lack
   semantic names but are already in the catalog.

2. **`CfRes` unit dominates the unknowns list.** 6 different `func_*` symbols
   (ranks 2, 9, 10, 15, 31, 35, 40, 41, 42, 43) are from `CfRes`,
   suggesting a cluster of unresolved function pointers in the resource
   system.

3. **`func_801C4124` is the most-widely referenced unknown** — called from 31
   frontier functions across 13 different UI units. This is a high-value
   target for early matching.

4. **The `lbl_*` entries are all data labels** (strings/constants) rather than
   code references. They represent global data accessed via `sda21`-relative
   addressing. Most are in the `lyt_animation` layout system.

5. **No `UnkClass_*` symbols appear in the top 15** (only 1 exists in the
   entire frontier), so the WS-6 deliverable for 15 rename plans is
   constrained by the data — only 1 proposal is possible.

## Next Steps

1. **Match `func_801C4124` first** — highest frontier function count (31),
   blocking many UI components.
2. **Investigate `CfRes` unknowns** — cluster of 10 related functions suggests
   a single unresolved vtable or function pointer table.
3. **Rename `CLODCacheManagerS` → `CLODCacheManagerS`** after WS-2 releases
   exclusive targets.json access.
4. Re-run `frontier_refs.py` periodically as the frontier advances to surface
   newly-exposed unknowns.
