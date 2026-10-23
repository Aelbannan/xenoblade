# UNIT_RULES burn-down — 2026-10-23 (unit-rules agent)

Table: **367 → 319** (`--ratchet` 367→336→319; `check_unit_rules_frozen.py --check` green).
Scripts: `.scratch/unitrules_r/` (`noop_all.py` = postprocess-a-copy + byte compare for every key,
`refcheck.py` = which other link inputs reference/define a rule's rename targets,
`locked_build.py` = ninja under the shared `.hexdiff.lock`).

## 1. Bit-identical no-ops (31 keys) — deleted

`noop_all.py` copies each freshly rebuilt raw object, runs `postprocess_reloc_names.py` on the copy
and compares bytes. Identical ⇒ the rule cannot affect the link.

* Matching/live: `code_800A75FC.o` (.sdata2 already empty), `CTaskGameEffAfter.o`, `CVision.o`,
  `CCur.o` (empty `UnitRules()` husks), `CWorkSystemMem.o` (.text already 0x160),
  `CWorkSystemCache.o` (.sdata2 already empty), `CMenuQuestLog.o` (`@13609` no longer emitted).
* NonMatching (also inert in the link, A-residual criterion 1): `CDeviceFont`, `CMenuCollepedia`,
  `CfCamEvent`, `CMenuPassiveSkill`, `CMenuKizunaTalkList`, `CPcEffect07`, `btm_acl`, `port_rfc`,
  `CScnEnvLgtCtrl`, `snd_FxChorusDpl2`, `lyt_init`, `ut_LockedCache`, `g3d_restev`, `g3d_draw`,
  `CTutorial`, `CVS_THREAD`, `CModelDisp`, `CMCCrystalSupport`, `ahx_sbf`, `sfd_tim`,
  `CMenuBattlePlayerState_ct`, `code_804C8718`, `CScnRootNw4r`.
* `KPAD.o` (NonMatching): payload stale — `permute_sdata2_words` raises "bad permutation" on the
  current object, so it could never apply.

## 2. Source fixes (8 keys) — the padding lived in the source, not the linker

Each unit: `hexdiff --all` unchanged (fully matched before and after; hcisu_h2 stays 6/8, same two
functions as before the edit), raw `run.py data diff` **MATCH**.

| key | rule | fix |
|---|---|---|
| `fs.o` | `drop_data_tail .data 0x21` | `static char lbl_80547F38[0x28]` → the literal at both `OSReport` sites (-str reuse pools one copy) |
| `btm_pm.o` | `drop_data_tail .data 0x3D` | `static char btm_pm_unsniff_str[0x40]` → literal at the `LogMsg_2` site |
| `hcisu_h2.o` | `drop_data_tail .data 0x6B` | removed fake `\0\0\0\0\0` tail from the "Invalid length" literal |
| `bta_sys_main.o` | `drop_data_tail .data 0x35` | removed fake `\0\0\0` tail from the "unregistered event" literal |
| `rfc_ts_frames.o` | `drop_data_tail .sdata 0xF` | `char lbl_80665958[8]/lbl_80665960[8]` → `"Bad UA"`/`"Bad DM"` literals (MWCC puts them in .sdata) |
| `btm_devctl.o` | `drop_data_tail .sdata 0x12` | `lbl_8066592C[5]` / `#pragma sdata_threshold 12` `lbl_80665934[12]` → `"TRUE"`/`"FALSE"` literals |
| `dsp.o` | `drop_data_tail .data 0x7D` + local `@386→@355` | removed fake `\0\0\0` from `"10:01:57"`; the rename is a STB_LOCAL name only |
| `dvdDeviceError.o` | `.sdata2` tail + 7 local `@19xx→@8xx` | tail already fixed in source (no-op); renames are STB_LOCAL names only |

## 3. Link-neutral / dead in the link (9 keys) — deleted

* Local-only renames (STB_LOCAL in, STB_LOCAL out; the linker never resolves against locals):
  `CMenuBattleChain.o` (jumptable `@N`), `ut_Font.o` (`...data.0`), `CCol4.o` (`...bss.0`).
* `CChainActorList.o`: renames two global vtables to `lbl_eu_*`; `refcheck.py` shows no other link
  input references either name (only the unlinked retail split of the unit itself) ⇒ byte-neutral.
* `CLibCriMoviePlay.o`: `thunk_452_dt` → `@452@…`; no link input references the target.
* `CProcess.o`, `HBMAxSound.o`: both use `link_transform`, so `c_build` hands the link a
  `*.link.o`; `unit_has_rules("X.link.o")` is false and the rule was **never applied**.
* `CNumSelect.o`, `CNandData.o`: EDS stripped sections the retail split **does own** with
  identical bytes (CNumSelect `.sdata2` 16 B, CNandData `.bss` 0x338) and renamed them to labels
  no other link input defines — the rule manufactured undefined symbols
  (`lbl_eu_80668090`; and `CfNandManager.o` references `sInstance__9CNandData`, which the rename
  hid). Deleting the rule is a strict link improvement.

## 4. Remaining (319) — blockers and next angles

| blocker | keys (examples) | next angle |
|---|---|---|
| D policy: latent EDS on NonMatching units (~230) | `extern_data_sections` ± pool/exact renames | owner records Option C (D decision doc §3) → latent bulk batch |
| shared float-pool model (live, retail split has no .sdata2) | `CCamUtil`, `CDrawGX`, `CScnFilter`, `CScnFogMan`, `CScnFrame`, `MTRand`, `UnkClass_80466348`, `code_802B9064`, `btu_hcif`, `g3d_resanm*`, `CLibCriStreamingPlay`, `CMCCylinderGauge`, `CTaskLOD`, `ut_TagProcessorBase#Q34nw4r` | same D decision; pool labels are referenced by retail objects (e.g. `CGXCache.o`, `snd_WsdPlayer.o`) |
| linker tail padding modeled in the split | `btu_init.o` (.sdata2 6→8), `synpitch.o` (.rodata 0x14→0x18), `gki_time.o` (.text) | shorten `splits.txt` end + `symbols.txt` size (CLibCriMoviePlay precedent); needs a global re-split — coordinate with the link agent |
| retail linker dead-strip of weak/unused text | `lyt_group`, `lyt_arcResourceAccessor`, `dvd_broadway`, `lyt_window`, `lyt_animation`, `CLib`, `HBMAnmController`, `g3d_scnobj`, `snd_TaskManager` | LCF/near-miss decision (B doc §7.7) |
| retail callers use dtk C-names | `CArtsSet.o` (7 member names), `CtrlMoveNpc.o` (`__ct__cf_CtrlMoveNpc`) | `symbols.txt` rename ceremony to the mangled names (link agent owns symbols.txt), or match the callers |
| set_data_align (.rodata 4 vs 8) | `CNReqtaskSave`, `CNReqtaskSaveBanner`, `adx_suwii`, `CSysWinSelect` + 9 NonMatching | B doc §5 LCF/exception decision |
| typed-pool + trailing compiler literals | `CfGimmick`, `CfObjectEne`, `e_pow` (orphan 1.0), `g3d_resmat` | find the source shape that lets MWCC pool the literals in retail order |
| content (category C) | `CProc`, `CTaskManager`, `mtx`, `AXFX*`, `OSNet`, `rfc_mx_fsm`, `rfc_port_fsm`, `CMenuPTState`, `CScrollBar` | C prompt |
| fake data hacks | `bta_hh_utils.o` (`bta_hh_pool_orphan_strings` array keeps strings of dead-stripped functions) | same dead-strip decision as text GC |

Note: `build.ninja` must be regenerated (`configure.py`) before the link stops routing the
retired live keys through `*.reloc.o`; until then the postprocess just copies them unchanged.
