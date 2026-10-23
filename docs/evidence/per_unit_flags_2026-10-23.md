# Per-unit compiler flag / mw_version evidence — 2026-10-23 ("flags" research agent, report-only)

Tooling: `.scratch/flags/sweep.py` takes the unit's exact `build.ninja` command line, swaps `mw_version` and
cflags (and optionally the source file), compiles into `.scratch/flags/out/`, and scores the object with
`hexdiff --all --json --no-build` logic, using the retail object as the reference. The script never writes
under `build/` or `src/`. The `base` row is recompiled in the same run, so headers that other agents edit
concurrently affect both sides equally. Baseline check: CFloorMap gives 29/75 here and 29/75 in real hexdiff.
Raw logs: `.scratch/flags/{batch_out,batch2_out,cfloormap_sweep1,cbm_sweep,hcic_sweep}.txt`,
plus per-function JSON in `.scratch/flags/out/*_results.json`.

## Inventory
- Every game TU uses `cflags_game` (base `-O4,p -inline auto -ipa file -use_lmw_stmw on -func_align 4`)
  with `mw_version` Wii/1.1. Per-unit `-O4,s`/`-func_align`/`mw_version` overrides already exist
  for about 50 units (configure.py lines 425–826). SDK, CRI, nw4r and BTE are pinned per library.
- No `.comment` section evidence exists. The DOL strips it and the repo has no record of it, so the
  version and flags have to be inferred from codegen alone.
- The repo already handles per-function `-O4,s` in a `-O4,p` unit with `#pragma optimize_for_size on`
  (MWCC_CASES §RVL wud/WUD, TagProcCompareIds). About 40 game TUs carry many of these regions
  (CItemBoxInfo 64, CEquipItemBox 38, CItemBoxGrid 37, CTagProcessor 37, …). A TU where most functions
  sit inside `optimize_for_size on` blocks was most likely `-O4,s` in the original build.

## Top-3 units (no flag/version win)
| unit | base | best alternative | verdict |
|---|---|---|---|
| CFloorMap | 29/75, 9074 words | Wii/1.3/1.5/1.7: −1 word; -O4,s −3 full; GC/3.0a5.2 −4; -O3/-schedule off −23; lmw off −1; func_align 16, inline deferred/smart: no change | current flags are correct; the residuals come from source shape |
| CBattleManager | 52/78, 24950 words | Wii/1.3 −25 words; -O4,s −19 full; func_align 16 −9; GC52 fails to compile | current flags are correct |
| CItemBoxInfo | 64/110, 18526 words | -O4,s ± stripping its 64 pragma regions: −1 full (setItemBoxVtxColor), ±2 fns | neutral; the pragma regions already reproduce -O4,s |

Batch over the 27 units with the most non-accepted functions (8 variants each: -O4,p/-O4,s, Wii/1.0/1.1/1.3/1.7,
GC/3.0a5.2/a3.4): **no unit gains from a version change.** The Wii/1.0–1.7 compilers produce identical
output for every unit (Wii/1.0 ≡ 1.1 ≡ 1.3 ≡ 1.7 in the game code), so the "different MWCC version"
hypothesis does not hold for Wii-era game TUs. The one exception is code_800B06A4 (known): Wii/1.x gives
+8/−3 full versus GC/3.0a5.2, which points to a mixed-version TU. Splitting that TU is the next step and
needs retail boundary evidence first. CfCamEvent_1 does best on GC/3.0a5.2 (Wii/1.x: +1/−3).

## Proposals (unit -O4,s, with the per-function `optimize_for_size` pragmas deleted)
Variant `Os_nopragma`: replace `-O4,p` with `-O4,s` and remove every `#pragma optimize_for_size` line.
push/pop/auto_inline pragmas stay. Both changes move the code toward the original source shape: one
unit flag replaces dozens of synthetic pragma regions.

| unit | full Δ (gain/loss) | mismatch words | .text vs budget | regressions |
|---|---|---|---|---|
| kyoshin/CEquipItemBox | 93→94 (+eibPageNext / 0) | 5646→4942 | 0xb5c4→0xad08 / 0xb7f0 | none (5 better, 0 worse); the flag alone gives the same result |
| kyoshin/CItemBoxLine | 48→51 (+AllocEntryStock, BuildLineLayout, SetPaneVisible / 0) | 3090→2695 | 0x7378→0x71ac / 0x79fc | FormatDisplayName 274→276 words |
| kyoshin/CEquipChange | 38→39 (+ConfirmSelect / 0) | **5133→1872** | **0x7278→0x4208** / 0x40e0 | 1 fn worse |
| kyoshin/makecrystal/CModelDispMakeCrystal | 53→56 (+InitFiles, OpenResultDialog, sinit / 0) | 3551→2834 | 0x645c→0x637c / 0x61e4 | 1 fn worse (16 better) |
| kyoshin/CTaskGame | 103→105 (+Draw, moveShowTitle / 0) | 761→537 | 0x45c8→0x4418 / 0x4044 | none |
| kyoshin/makecrystal/CMCCrystalBox | 73→73 | 2562→1440 | 0x73c4→**0x6b34 (now fits)** / 0x6d7c | none |
| kyoshin/CCollepedia | 67→68 (+__ct__CCollepedia / 0) | 1082→814 | 0x3b20→0x39e0 / 0x37d0 | 1 fn worse |
| kyoshin/CItemBoxGrid | 106→106 | 5059→4936 | 0xd2e0→0xd43c / 0xd4d4 | none (3 better) |
| kyoshin/makecrystal/CMCGetItemBox | 40→40 | 683→630 | 0x37ec→0x36fc / 0x3604 | none |
| kyoshin/CKizunaTalkList | 28→28 | 990→980 | | none |
| kyoshin/CQstLogList, monolib CWorkRoot | 0/0 | −3 / −5 words | CWorkRoot 0xaf8→0xa48 | none (also true for the flag alone) |
| kyoshin/menu/CMenuArtsSet (mixed) | 94→95 (+artsStepState, artsStepAhead / −CMenuArtsSet_isCreated) | 1314→1336 | | 4 worse; needs a closer look |

**Do NOT flip:** CSaveLoad (−10 full), CTitle (−1, currently 66/66), CPartyStateWin (3 worse),
CArtsInfo (1 worse), CTagProcessor and CPassiveSkill (neutral, mixed). In these units the
`optimize_for_size off` regions match retail -O4,p functions, so the original TU really did mix the two.

Total for the clean proposals: about +13 FULL_MATCH, 0 lost FULL_MATCH (CMenuArtsSet excluded), about −6.8k
mismatch words, and CMCCrystalBox's split-size overrun goes away. All numbers are from scratch builds.
The promote agent should apply each change and re-verify it with `hexdiff --all`.

## Walls
- **#5 `b .+4` sinit barrier — stale.** MWCC_CASES §4 already marks it RESOLVED: it is a merged-symbol tail call,
  and splitting the symbol in symbols.txt gives FULL_MATCH ×5. It is not a flags issue. KNOWN_WALLS should be updated.
- **#6 mtctr/bdnz under unit-locked -O4 — dissolved.** hcicmds (GC/3.0a5.2 `-O4`) is 57/59 and
  `write_cur_iac_lap` is FULL through the nested-loop source form (MWCC_CASES 2027-01 correction). Unit -O4,s
  is −12 full, Wii/1.x is −41/−42 full, and a3.4 is identical. The only remaining residuals are the
  per_inq_mode/change_name header-store hoist, and no flag affects them.
- **#13 per-function opt-level conflict — dissolved in general.** `#pragma optimize_for_size on` + `dont_inline on`
  reproduces -O4,s per function inside a -O4,p unit. Where -O4,s dominates a unit, flipping the unit flag
  (the table above) is the better match to the original source shape.
- **Still real (version/flag sweeps do not change them):** #1 FPR prologue, #2 bl/witness, #3 `@sda21` li,
  #4 addi→mr, #7 WGPIPE regalloc, #8 solver scale, #9 auto-unroll rlwimi (−O4,s also unrolls, per MWCC_CASES),
  #10 GQR5, #11 CSE/rlwimi direction, #12 li-float, #14 section padding. code_800B06A4 stays a mixed-version
  TU that no single version satisfies.

## Caveats
- Several sources did not compile during the sweep because other agents were mid-edit (COption,
  CfGameManager, code_804DEDA8, and early CItemBoxInfo). GC-version variants fail on Wii-only syntax
  (`__declspec(noinline)` at block scope), so these runs say nothing about GC versions.
- `-O4,s` with the pragmas still in place often makes .text larger than budget. Measure the variant that
  removes the pragmas, not just the flag.
