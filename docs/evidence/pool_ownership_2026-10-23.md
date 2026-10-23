# `.sdata2` pool ownership: are cross-TU pool references real? (2026-10-23)

Research-only (no src/libs/config edits). This tests the claim in
`docs/evidence/decomp/unit_rules_category_d_decision.md` §1.3 and §3 that compiler-emitted
float pools "can never be owned TU-locally without changing codegen". The short answer is that
the claim is false. Almost every cross-TU pool reference comes from a wrong `.sdata2` split
boundary in `config/us/splits.txt`. The few real exceptions are named `const float` globals,
and MWCC compiles those to byte- and reloc-identical output with plain `extern`.

Scripts and raw outputs are in `.scratch/pool/`: `scan.py`, `eds.py`, `cls.py`, `prop.py`,
`scan.json`, `eds.json`, `proposed_sdata2_slices.tsv`, and the MWCC experiments `mc.cpp`,
`owner*.cpp`, `user.cpp` and `cc.sh`.

## Method

- Every `.sdata2`/`.rodata`/`.sbss2` symbol in `config/us/symbols.txt` gets its address and the
  `config/us/splits.txt` block that currently owns it.
- Every referrer of each symbol is found by scanning every reloc'd operand in
  `build/us/asm/**/*.s`, with the unit taken from the asm path.
- Each `UNIT_RULES` key in `tools/postprocess_reloc_names.py` whose `extern_data_sections`
  contains `.sdata2` is classified from the labels that its retail asm references.

## Global facts

| fact | value |
|---|---|
| `.sdata2` labels in symbols.txt | 4333 |
| labels referenced by more than one TU | **46** (about 1%) |
| sole-referrer runs: contiguous blocks of labels used by exactly one TU | 399 |
| runs whose address order contradicts `.text` link order | **5 / 399** |
| runs whose current split owner is not their only referrer | 216 runs / 1502 labels |
| of those, how many sit inside `g3d_resmat.cpp`, `CGXCache.cpp` or `FloatUtils.cpp` slices | 76 + 72 + 18 runs |
| retail copies of `1.0f` / `0.0f` in `.sdata2` | 343 / 357 |

Retail `.sdata2` is laid out exactly as concatenated per-TU MWCC pools in link order. There is
no linker constant merging (class c): `1.0f` appears 343 times. The supposedly "shared pools" are
oversized split slices that swallowed the pools of the TUs that follow them:

- **`CGXCache.cpp` `.sdata2` 0x8066A378–0x8066B52E (0x11B6 bytes)** holds 73 separate runs, in
  link order: CGXCache (0x8066A378, 9 labels), CDeviceFont, CDeviceFontInfoExt,
  CDeviceFontLayer, CDeviceGX, CDesktop, CDrawGX (0x8066A468–0x8066A478), CException,
  CLibCriMoviePlay, CLibHbm*, lod/*, mpfsys/*, scn/* (CScnFilter at 0x8066ABA0–0x8066ABA8),
  coli/*, effect/*, CMdl*, and core/code_804F0258. CGXCache's own pool is 9 labels.
- **`FloatUtils.cpp` 0x8066A178–0x8066A360** holds the pools of CAttrTransform, CCamUtil
  (0x8066A190–0x8066A1A0), MemManager, MTRand (0x8066A1D0–0x8066A1E0), CVec3 and CVec4. Next
  comes the MathConstants block (see class b), then CCol3, CCol4, CMat34, CMat44, CPlane, CQuat,
  CProc, CToken, CView, CViewFrame, CPadManager and CStopwatchUtil. FloatUtils itself has no pool
  entries; it only references `huge` and `hugeminus`.
- **`g3d_resmat.cpp` 0x80669A68–0x8066A178** (exactly the dissolved `nw4r_data.s` pool) holds the
  pools of 77 runs: g3d_resanm 0x80669AB0, resanmclr 0x80669AC8, resanmtexsrt 0x80669AD8,
  resanmchr 0x80669AE0–0x80669B08, resanmamblight 0x80669B10, resanmlight 0x80669B20, resanmfog
  0x80669B30, through snd_* (snd_WsdPlayer 0x8066A108–0x8066A128) and ut_* (ut_TextWriterBase
  ends at 0x8066A170).
- **kyoshin:** about 50 small off-by-one-TU errors, where a TU's `.sdata2` slice also covers the
  next TU's pool. Examples: pluginMath owns CActParamAnim's 54 labels; CActParamAnim owns
  CActParamData's and CTaskEnvironment's; CfMapItemManager owns CActorParam's 26; CMiniMap owns
  CMenuSymbolMark's 24.

The full first-cut boundary table (unit, proposed start, proposed end, current owner) is in
`.scratch/pool/proposed_sdata2_slices.tsv`, with 216 rows. Each end is the next run's start.
Before each table edit, check unreferenced string labels and alignment padding by hand.

## The 46 shared labels

| group | labels | referrers | class |
|---|---|---|---|
| `ml::pi, tau, halfpi, quarterpi, epsilon, rad2deg, deg2rad, huge, hugeminus` (0x8066A1F8–0x8066A21C) | 7 shown as shared; 0x214/0x218 are used only by FloatUtils | up to 39 TUs (kyoshin + monolib) | **(b)**: `libs/monolib/src/math/MathConstants.cpp` already exists |
| 0x8066AF20 `0.7071f` | 1 | 10+ kyoshin TUs | **(b)**: named global placed between the code_804BC9EC and code_804BD8E8 pools; defining TU still unconfirmed |
| CfCollCircle/Sphere/AABB/Cylinder/CapsuleImpl + CfObjectColl (0x80666910–0x80666934) | 8 | link-adjacent TUs 49–54 | **(a-merge)**: TU over-split (one original TU) or class-static consts |
| CMenuBattleDamage / CMenuBattlePlayerState(_ct) | 5 | adjacent | (a-merge) |
| CfMapItemManager / CActorParam | 4 | adjacent | (a-merge) |
| code_80213488 / CMCCrystalBox | 3 | adjacent | (a-merge) |
| lod code_804645CC / UnkClass_80466348 / code_80468434 | 5 | adjacent | (a-merge) |
| MPFDrawDisplayList / UnkClass_80471EC8 / MPFDrawBillLayTex | 3 | near-adjacent | (a-merge) or (b) |
| CScnRoot/CScnRootNw4r, CColiProc/code_804B2FF0, CETrail/code_804CC2B8 (4), CSchedule/code_804E36DC (2), CfCamEvent + 3 Intf TUs | 10 | adjacent | (a-merge) |
| 0x80669F30 `-90.4f` | 1 | snd_Channel + snd_EnvGenerator (link indices 903 and 905) | **(d)**: sits at the Channel/EnvGenerator pool seam. The candidate is a named `VOLUME_MIN_DB`-style global (snd_Types.h currently has a `static const`, which would duplicate). Unresolved. |

"Adjacent" means consecutive in `.text` link order. MWCC cannot share a pool across TUs, so
these were most likely one source TU that our splits divide in several places. The other
explanation is a `static const` class member defined in the first TU. Each group needs a
`.text`-boundary or `.sdata2`-boundary review. None of them needs a codegen change.

## MWCC proof for class (b)

Settings are the monolib cflags from `build.ninja`: Wii/1.1, `-O4,p`, `-str reuse,pool,readonly`
and `-ipa file`. See `.scratch/pool/cc.sh`.

- `mc.cpp` matches `MathConstants.cpp` plus an `extern const float pi, tau, ...;` declaration
  (the kind that belongs in `monolib/math.hpp`). It emits 9 **global** `.sdata2` objects,
  `pi__2ml` … `hugeminus__2ml`, in source order, 0x24 bytes:
  `40490fdb 40c90fdb 3fc90fdb 3f490fdb 358637bd 42652ee0 3c8efa35 56b5e621 d6b5e621`. That is
  retail 0x8066A1F8–0x8066A21C value for value, followed by the 4-byte pad to CCol3's pool at
  0x8066A220.
- `owner_noext.cpp` has the same definitions **without** an `extern` declaration in scope. C++
  `const` gives them internal linkage, so it emits **nothing**. This explains why the current
  `MathConstants.cpp` (NonMatching, not in splits.txt) cannot own the data today.
- `user.cpp` declares `extern const float pi, tau;` and uses them. It gets `R_PPC_EMB_SDA21`
  relocs against `pi__2ml` and `tau__2ml`, while its own literal `0.5f` stays in a local `@13`
  pool entry. This is the retail shape: references go to the named global and the TU's other
  literals stay in its own pool.

So class (b) needs no hack. Add a MathConstants split (`.sdata2` 0x8066A1F8–0x8066A220, no
`.text`), give `FloatUtils.cpp` an empty `.sdata2` slice, and make referrers use `ml::pi` and
the other names instead of `extern lbl_eu_8066A1F8` (for example `coli/code_804A6C60.cpp:135`
and `mpfsys/UnkClass_80471EC8.cpp:85` today).

## Classification of the `.sdata2` EDS entries

The current `UNIT_RULES` has **190** keys with `.sdata2` in EDS. The decision doc's figure of
208 came from an earlier snapshot of the table. Each key is listed under its dominant class, and
"+b" means the key also references MathConstants.

| class | keys |
|---|---|
| (a) boundary: the key's pool is inside another TU's slice; moving the boundary makes it TU-owned | 102 + 17 (+b) = **119** |
| (a, already): retail split already gives the unit its own `.sdata2` slice, so EDS is stale and can go once the source defines the pool | 41 + 14 (+b) = **55** |
| (a-merge): also shares labels with a link-adjacent TU, so the TU boundary needs review | 7 + 2 (+b) = **9** |
| (b) named constant (MathConstants or 0.7071) as a secondary need | included above: **33** keys use them; no key depends on (b) alone |
| (c) linker merging | **0** (no evidence; 343 copies of 1.0f) |
| (d) unresolved (snd -90.4f) | **1** |
| no `.sdata2` refs at all (EDS `.sdata2` inert) | **6** (code_802B9064, btu_hcif, snd_TaskManager, snd_SoundThread, CMenuTutorial, CMenuSkipTimer) |

## Conclusion

- The statement "can never be owned TU-locally without changing codegen" is **refuted** for
  `.sdata2`. The retail layout is per-TU pools in link order, with 5 order inversions in 399
  runs.
- Fixes:
  1. Shrink `CGXCache.cpp` to 0x8066A378–0x8066A3B8, `FloatUtils.cpp` to empty, and
     `g3d_resmat.cpp` to its own run.
  2. Give each referrer the slice listed in `proposed_sdata2_slices.tsv`.
  3. Add a data-only `MathConstants.cpp` split with an `extern` header declaration.
  4. Review the 9 adjacent-TU merge groups.
- With these fixes, full `.sdata2` ownership looks reachable. The expected residue is about 1–2
  named-constant TUs plus the snd `-90.4f` seam, and both fit the class (b) pattern.
- Not covered here: `.data` vtables in dissolved blobs and `.rodata`/`.sdata` EDS entries. The
  same scan (`scan.json` also has `.rodata`) is the next step.
- Caveats:
  - Boundary ends are a first cut; strings and padding labels with no referrers need manual
    placement.
  - Retail `.sdata2` contents are exact only after each owning source is matched, so moving a
    boundary turns the unit's data gate from vacuous to real.
  - Splits edits need a repo-wide link check.

## Status: split-fix applied (2026-10-23)

These changes affect only `config/us` (eu/jp splits do not have these `.sdata2` slices). The
scripts are in `.scratch/splitfix/`: `scan2.py` (this scan also counts `.4byte` data refs, so
string labels such as `"Mem1"`, `"CToken"` and `"CView"` resolve to their owners), `plan.py`,
`mkplan.py`, `kyplan.py`, `splits_edit.py`, `trim.py` and `edscount.py`.

**Rules used.** A run whose referrer is a NonMatching unit gets its slice: start at the run,
end at the next run, so the inter-TU pad stays inside the slice. A Matching referrer gets a
slice only if its raw MWCC `.sdata2` already equals the retail run. That slice is then trimmed
to the last content byte (see MWCC_PATTERNS "Section padding is a linker artifact"), and the
unit's `.sdata2` EDS and pool renames are removed. Any other Matching run is left as a dtk
gap (`auto_*` object). This keeps the link safe, and the run waits for the owner to emit the
pool from its source.

1. **MathConstants.** There is now a data-only split for `.sdata2` 0x8066A1F8–0x8066A21C.
   `ml::*` constants are renamed in us+eu `symbols.txt` (`pi__2ml` … `hugeminus__2ml`;
   `hugeminus` is 4 bytes). There are 115 source files, all referrers, where
   `lbl_eu_8066A1F8..218` declarations and their `#define`/`#undef` shields were removed and
   uses became `ml::pi` etc. The declaration comes from the existing
   `monolib/math/MathConstants.hpp`, which `math.hpp` includes; two TUs gained a direct include.
   `FloatUtils.cpp` now holds only `isErrFloat`, which uses `ml::huge`/`ml::hugeminus`, and
   has no `.sdata2` slice. MathConstants raw `.sdata2` = **MATCH**: 9 globals, with relocs as
   `pi__2ml`… It stays NonMatching in configure.py (owned by the promote agent), and it is
   ready to promote.
2. **FloatUtils slice** split into its 18 TUs. CVec3, CVec4, CCol3, CCol4 and MTRand moved
   their `extern lbl_` uses to literals, and CProc's 0.6f did the same. Each of these TUs
   emits its own pool.
3. **CGXCache** shrunk to 0x8066A378–0x8066A3A8. 58 NonMatching TUs got their slices.
   CScnItemCamera is skipped because its runs are spread across 5 places, which points to a
   `.text` boundary problem. 23 Matching runs are gaps, because their source pools differ
   from retail (list below).
4. **g3d_resmat** shrunk to 0x80669A68–0x80669AA0. The blob definitions at ≥0x80669AA0 were
   deleted from `g3d_resmat.cpp`, along with the `add_symbols` entries for them. 60
   NonMatching TUs and 3 matched Matching TUs (g3d_resanmclr, g3d_resanmfog, g3d_scnobj) now
   own their pools. 13 Matching runs are gaps.
5. **kyoshin off-by-one:** 61 slice rows. Owners were shrunk, and next-TU pools were either
   assigned or merged into an existing slice that began right after them. CMCCylinderGauge's
   EDS was retired.

**Numbers.** `.sdata2`-EDS keys went from 136 to 129. Keys on Matching (linked) units went
from 23 to 16, counted by basename against configure.py. Retired: CCamUtil (`.sdata2` only),
MTRand, CProc, g3d_resanmclr, g3d_resanmfog, g3d_scnobj (plus its two 0x80669CD8/CDC
retargets), CMCCylinderGauge. Also removed: the `pi/epsilon/halfpi__2ml` renames on CMat34
and CCamUtil. ocUnit's 0x3CA08052 pool rename now targets `rad2deg__2ml`, which is a
pre-existing wrong-literal residual. Raw data diff went from 418/1161 failures to 448/1198.
Units that became full MATCH: CMCCylinderGauge, g3d_resanmclr, g3d_resanmfog,
g3d_resanmamblight, g3d_resanmlight, g3d_camera, snd_Bank, MTRand, MathConstants, CMat44,
CPlane, CStopwatchUtil. `.sdata2` went FAIL→ok on 22 units. The 62 ok→FAIL units are all
NonMatching TUs whose gate was vacuous before and is now real, because their sources still
`extern` the retail labels. That is a source data issue for each of those TUs. No Matching
unit lost a data MATCH, and hexdiff shows no FULL_MATCH loss. link-gate missing went from 87
to 86. The remaining FAIL lines (`@1581`/`@1584`) are CfTaskMain's EDS-stripped string pool,
renumbered by concurrent header edits, not by these splits.

**Leftover gaps (Matching units whose source pool ≠ retail; they need matching work, not
splits):** g3d_resanm, g3d_resanmtexsrt, g3d_resanmchr, g3d_transform, g3d_anmobj, g3d_cpu,
g3d_fog, lyt_drawInfo, math_triangular, snd_BankFile, snd_EnvGenerator, snd_FxChorusDpl2,
snd_WsdFile, CDeviceGX, CDrawGX, CLib, CLibCri, CLibCriMoviePlay, CLibCriStreamingPlay, CLibG3d,
CLibHbm, CLibVM, CTaskLOD, UnkClass_80466348, CScnLightMan, CScnBlend, CScnFilter, CScnFogMan,
CScnFrame, code_804C8684, CWorkSystemCache, CNand, CERand, CMdlMouth, CMdlAnmEye,
CNReqtaskSaveBanner, CMenuPTState, CCur, CChainActorList. **CDrawGX:** its EDS is the bare
string `(".sdata2")`, which the substring bug turns into stripping .data, .sdata and .sdata2.
It was left untouched because its slice is a gap. Fixing the tuple changes link behaviour, so
it should be fixed together with CDrawGX's pool.

**Not done (need source or `.text` review):** the slices of pluginEve, pluginMath, CVision and
CfGimmickItem. Each of these Matching TUs defines its successor's pool in source, the same
pattern as FloatUtils. Also not done: CfTFile/CfObjectPc (address-order inversions),
CfGameManager/CfObjectEne (interleaved runs), CfObjectImplNpc, CBgTex (not in configure.py),
CScnItemCamera, and the `0.7071f` named global (0x8066AF20, left inside the code_804BD8E8
gap).

### Neighbouring-TU groups (no merges done) — recommendations

| group | recommendation |
|---|---|
| CfCollCircle/Sphere/AABB/Cylinder/CapsuleImpl + CfObjectColl | Most likely one TU (shared 8-label pool). Check whether the `.text` of 49–54 is contiguous with no padding between functions; if so, merge into a `CfColl.cpp`-style unit. Otherwise the shared values are `static const` class members defined in CfCollCircleImpl. |
| CMenuBattleDamage / CMenuBattlePlayerState(_ct) | `_ct` is a split-off ctor of the same class; merge `_ct` into CMenuBattlePlayerState first, then check for a Damage/State shared header const. |
| CfMapItemManager / CActorParam | The labels reference each other in both directions, which is a strong one-TU signal. Review the `.text` boundary at their junction. |
| code_80213488 / CMCCrystalBox | code_80213488 is a catalog stub TU. Its functions probably belong to CMCCrystalBox, so extend CMCCrystalBox's `.text`. |
| lod code_804645CC / UnkClass_80466348 / code_80468434 | A catalog-split LOD TU. Merge UnkClass_80466348 into code_804645CC, then re-check code_80468434. |
| MPFDrawDisplayList / UnkClass_80471EC8 / MPFDrawBillLayTex | The 3-way shared word at 0x8066A730 suggests a named const in MPFDraw*.hpp (class b) rather than a merge. Try `extern const` first. |
| CScnRoot / CScnRootNw4r | Nw4r subclass split from the base. Review the `.text` boundary; a likely merge. |
| CColiProc / code_804B2FF0 | code_804B2FF0 is a catalog stub; fold it into CColiProc. |
| CETrail / code_804CC2B8 (4 labels) and CSchedule / code_804E36DC (2), CfCamEvent + 3 Intf TUs | Catalog stubs adjacent to their real class. Fold each stub into its class TU, and the Intf TUs into CfCamEvent. |

**-90.4f seam (0x80669F30, snd_Channel/snd_EnvGenerator):** snd_EnvGenerator's slice now
starts at 0x80669F34. 0x80669F30 stays in snd_Channel's slice, which matches retail if the
value is snd_Channel's pool literal and EnvGenerator reaches it through a named global.
Recommendation: replace `snd_Types.h`'s `static const f32 VOLUME_MIN_DB` with
`extern const f32` and define it in snd_Channel.cpp. This is the same class (b) pattern as
MathConstants. Verify with MWCC before changing anything.

### `.rodata` scan (report only)

2540 `.rodata` labels: 27 are referenced by more than one TU and 99 have no reference (mostly
string-pool interiors). 115 slices contain labels referenced by a different TU. The biggest
are math_triangular (0x1010, nw4r sin/cos tables), CFloorMap (0xDF8), CPassiveSkill, ahx_dcd
(11 foreign referrers, a CriWare lib), CQstLogInfo and CQuestWindow. These are mostly
switch-table/string-pool slices that swallowed the next TU, the same off-by-one pattern as
`.sdata2`. Data in `.scratch/splitfix/foreign_ro.tsv`. Next step: the same planner with a
`.rodata` alignment-aware end, since `.rodata` slices are 8/32-aligned.
