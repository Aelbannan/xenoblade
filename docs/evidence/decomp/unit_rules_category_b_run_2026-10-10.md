# Category B run 2026-10-10 — inert-rule burn-down + live-entry verdicts (UNIT_RULES burn-down)

Prompt: `docs/handoff/unit_rules/b_linker_behavior.md`. Owner tag: `category-b`.
Worktree: isolated paseo worktree (no access to the main checkout; no builds possible —
compilers/wibo/binutils are not downloadable here, network blocked).

Result: **29 keys deleted + 74 payload fields pruned** (entries 406 → 377, all
`--check`-clean, link edge provably untouched). Every remaining in-scope entry carries a
verdict below. The 22 `set_data_align` keys are preserved untouched with a fresh align
audit + recommendation (§5). No source files were changed (why: §7).

## 1. Scope re-derived (this worktree, `us` link edge)

Live = basename has a `RELOCPOST` (`*.reloc.o`) input in the `build/us/main.elf` edge.

| set | entries | live |
|---|---|---|
| `UNIT_RULES` total | 406 → **377** | 79 basenames |
| B scope (non-EDS key with a B-field other than bare `repack_after_drop`) | 100 | **27** |
| EDS-bearing keys (category D, untouched except align audit rows) | — | 20 of the 79 live |
| content/repack-only keys (category C/A, untouched) | — | rest |

B-fields in scope: `drop_data_tail/range/nobits`, `zero_data_range/nobits`,
`drop_text_symbols*`, `trim_text_size`, `pad_text_size`, `pad_data_section`,
`pad/trim_sdata2_size`, `set_data_align`, `retarget_relocs[_local]`
(`repack_after_drop` only where it services a drop).

Link-status classes for the 100 (ground truth = `build.ninja` edge + `configure.py`
`Matching` flags + `link_transform` routing in `tools/project.py`):

- **27 live** — `RELOCPOST` fires (§6 verdicts, all kept).
- **71 retail-linked** — link uses `build/us/obj/...` (retail split object);
  every sampled flag is `Object(NonMatching, ...)` → the rule never runs.
- **2 link-transform bypass** — `CProcess.o`, `HBMAxSound.o` link as
  `CProcess.link.o` / `HBMAxSound.link.o` (`RENAMESYM` copies); `unit_has_rules()`
  sees the `.link.o` basename, so the `UNIT_RULES` key never fires.

## 2. Deleted keys (29 whole-key, all inert, proof §4)

| key | fields removed | link status | evidence |
|---|---|---|---|
| `CArcItem.o` | `retarget_relocs`, `repack_after_drop` | retail | no `RELOCPOST` edge; `NonMatching` |
| `CGame.o` | `pad_data_section`, `drop_text_symbols` | retail | idem |
| `CMenuVision.o` | `retarget_relocs` | retail | idem |
| `CScnItemCameraNw4r.o` | `drop_data_tail`, `retarget_relocs` | retail | idem |
| `CStopwatchUtil.o` | `drop_data_tail`, `retarget_relocs` | retail | idem |
| `CfGimmickJump.o` | `drop_data_tail` | retail | idem |
| `CfGimmickObject.o` | `drop_data_tail` | retail | idem |
| `CfMapItemManager.o` | `drop_data_tail` | retail | idem |
| `CfObjectActor.o` | `drop_data_range` | retail | idem |
| `CfObjectPc.o` | `drop_data_range` | retail | idem |
| `CfRes.o` | `pad_data_section` | retail | idem |
| `CfResPcImpl.o` | `drop_data_range` | retail | idem |
| `CtrlNpc.o` | `drop_data_tail` | retail | idem |
| `CtrlObjectParam.o` | `drop_data_tail` | retail | idem |
| `OSReset.o` | `drop_data_range` | retail | idem |
| `WPADEncrypt.o` | `pad_data_section` | retail | idem |
| `code_80296898.o` | `zero_nobits` | retail | idem |
| `db_DbgPrintBase.o` | `drop_text_symbols` | retail | idem |
| `dw_Window.o` | `drop_text_symbols` | retail | idem |
| `g3d_resfile.o` | `drop_text_symbols` | retail | idem |
| `hcicmds.o` | `pad_data_section` | retail | idem |
| `mwsfdcre.o` | `drop_data_range` | retail | idem |
| `sfx_zmv.o` | `drop_data_range` | retail | idem |
| `snd_FxReverbStdDpl2.o` | `pad_data_section` | retail | idem |
| `snd_MmlSeqTrackAllocator.o` | `drop_data_range` | retail | idem |
| `snd_PlayerHeap.o` | `drop_text_symbols` | retail | idem |
| `snd_StrmChannel.o` | `drop_text_symbols` | retail | idem |
| `ut_DvdFileStream.o` | `retarget_relocs` | retail | idem |
| `ut_RomFont.o` | `drop_data_range`, `drop_text_symbols_as_undef` | retail | idem |

## 3. Pruned keys (38 keys, 74 fields; `set_data_align` never touched)

| key | B-fields removed | kept |
|---|---|---|
| `CChildListNode.o` | `pad_data_section`, `drop_text_symbols`, `drop_text_symbols_as_undef` | `exact_renames`, `inject_relocs` |
| `CCol4.o` | `zero_nobits`, `retarget_relocs` | `exact_renames` |
| `CDesktop.o` | `retarget_relocs` | `set_data_align` |
| `CDevice.o` | `drop_data_tail`, `zero_data_range`, `drop_nobits_range` | `exact_renames`, `inject_relocs` |
| `CDeviceClock.o` | `drop_data_tail`, `drop_nobits_range`, `drop_text_symbols_as_undef` | `exact_renames`, `inject_relocs` |
| `CDeviceVI.o` | `drop_data_tail`, `drop_text_symbols_as_undef` | `exact_renames` |
| `CMCEffStart.o` | `drop_data_tail`, `retarget_relocs` | `pool_patterns`, `exact_renames` |
| `CNBanner.o` | `drop_data_tail`, `retarget_relocs`, `drop_data_range`, `drop_text_symbols`, `repack_after_drop` | `data_reloc_offset_moves`, `add_symbols` |
| `CPackItem.o` | `drop_data_tail`, `drop_text_symbols_as_undef` | `set_data_align` |
| `CProcess.o` | `drop_data_tail`, `retarget_relocs`, `drop_data_range`, `pad_data_section` | `exact_renames` |
| `CScnRootNw4r.o` | `drop_data_tail` | `exact_renames` |
| `CTaskEnvironment.o` | `drop_data_tail`, `pad_data_section` | `exact_renames`, `inject_relocs` |
| `CView.o` | `drop_data_tail`, `retarget_relocs`, `drop_data_range`, `pad_data_section` | `exact_renames`, `swap_data_blocks` |
| `CWorkRoot.o` | `drop_data_range`, `drop_text_symbols`, `drop_text_symbols_as_undef`, `repack_after_drop` | `exact_renames` |
| `CWorkThread.o` | `drop_data_tail`, `retarget_relocs`, `drop_data_range`, `drop_nobits_range` | `set_data_align`, `data_pool_patterns`, `exact_renames`, `addend_sets` |
| `CfCam.o` | `drop_data_tail` | `set_data_align`, `add_symbols` |
| `CfCamEvent_1.o` | `drop_data_tail` | `pool_patterns` |
| `CfGimmickElv.o` | `drop_data_tail` | `pool_patterns` |
| `CfObjectImplPc.o` | `drop_data_tail` | `pool_patterns` |
| `HBMAxSound.o` | `pad_data_section` | `data_pool_patterns` |
| `KPAD.o` | `drop_data_range` | `permute_sdata2_words`, `exact_renames` |
| `UnkClass_80460C34.o` | `pad_data_section` | `exact_renames` |
| `WUD.o` | `drop_data_tail`, `retarget_relocs`, `zero_data_range` | `exact_renames`, `inject_relocs` |
| `adx_tlk.o` | `drop_data_tail`, `retarget_relocs` | `set_data_align`, `add_symbols` |
| `ax_rna.o` | `drop_data_range` | `set_data_align` |
| `code_804B2FF0.o` | `retarget_relocs` | `set_data_align`, `exact_renames` |
| `code_804BC9EC.o` | `pad_data_section` | `set_data_align`, `addend_sets` |
| `db_assert.o` | `drop_data_range`, `pad_data_section`, `drop_text_symbols` | `exact_renames`, `prefix_renames` |
| `dct_isr.o` | `drop_data_tail`, `retarget_relocs` | `addend_sets` |
| `pluginUi.o` | `drop_data_tail` | `pool_patterns` |
| `sfd_tim.o` | `drop_data_tail`, `retarget_relocs` | `addend_sets` |
| `snd_FxChorusDpl2.o` | `pad_data_section`, `drop_text_symbols` | `exact_renames` |
| `snd_MemorySoundArchive.o` | `drop_data_tail`, `drop_text_symbols` | `exact_renames` |
| `snd_Util.o` | `pad_data_section` | `pool_patterns`, `exact_renames`, `prefix_renames` |
| `ut_LockedCache.o` | `drop_text_symbols` | `exact_renames` |
| `vi.o` | `drop_data_range` | `exact_renames`, `addend_sets` |
| `yvm2.o` | `drop_data_range` | `set_data_align` |

Stale prose cleanup: entry comments describing removed rewrites were rewritten to
describe the kept payload (or retired with the entry); two orphaned header comments
(`CStopwatchUtil`, `snd_StrmChannel`) removed. Comment-only; payloads verified
identical for all kept fields (§4).

## 4. Proof (build-free, this worktree has no compilers — see §7)

- **Inertness (link impact: none).** Link-edge survey of the regenerated `us`
  `build.ninja`: none of the 29 deleted basenames has a `RELOCPOST` input; the 38
  pruned keys are likewise absent from the edge (71 retail-`obj/`, 2 `.link.o`
  bypass). `build.ninja` was not regenerated by this change and needs no
  regeneration: `unit_has_rules()` still resolves all 79 live basenames
  (verified post-edit), so every `RELOCPOST` edge is unchanged.
- **Payload discipline.** Machine-checked before/after: deleted key set == plan
  (29, §2); surviving keys lost only planned B-fields (74, §3); every kept field
  value is identical; `set_data_align` present on all 22 keys before and after;
  zero added lines of payload (comment prose only).
- **Gates.** `tools/check_unit_rules_frozen.py --check` → OK
  (`current 377, baseline 406, retired 138`; `74 retired field(s) awaiting --ratchet`).
  Raw `data diff` is rule-independent by construction, so deletions cannot change
  raw-gate outcomes; `--postprocess` diagnostics for pruned keys now run fewer
  rewrites (diagnostic-only, never promotion evidence).
- **Tool suite.** `python3 -m unittest discover -s tools/coop/tests -t .` → 474 ran, OK.
- **Recertification of affected targets.** No source file changed and no link input
  changed; there are no code targets to recertify. (Live units' targets are
  untouched by this run.)

## 5. `set_data_align` decision evidence (22 keys / 26 pairs, all preserved)

Fresh audit from `config/us/splits.txt` slice starts vs the rule payloads:

| key | section | rule | retail start | %8 | EDS |
|---|---|---|---|---|---|
| `CDesktop.o` | .sbss | 4 | 0x806656B4 | 4 | no |
| `CETrail.o` | .sbss | 4 | 0x806659BC | 4 | D |
| `CMdlAnmUV.o` | .sdata | 4 | 0x80663C74 | 4 | no |
| `CMdlDynamics.o` | .data | 4 | 0x805701FC | 4 | D |
| `CMdlMouth.o` | .rodata | 4 | 0x805247B4 | 4 | D |
| `CNReqtaskSave.o` | .rodata | 4 | 0x805245D4 | 4 | no |
| `CNReqtaskSaveBanner.o` | .rodata | 4 | 0x80524894 | 4 | no |
| `CPackItem.o` | .rodata | 4 | 0x805246FC | 4 | no |
| `CScnFilter.o` | .rodata | 4 | 0x805240AC | 4 | D |
| `CSysWinSelect.o` | .sbss | 4 | 0x80663FDC | 4 | no (live) |
| `CWorkThread.o` | .rodata/.sdata/.sbss | 4/4/4 | …474/…51C/…594 | 4/4/4 | no |
| `CfCam.o` | .bss/.sdata2 | 4/4 | …A2C/…29C | 4/4 | no |
| `UnkClass_8046368C.o` | .data | 4 | 0x8056D71C | 4 | no |
| `adx_suwii.o` | .rodata | 4 | 0x80519744 | 4 | no (live) |
| `adx_tlk.o` | .bss | 4 | 0x805E26D4 | 4 | no |
| `ax_rna.o` | .rodata | 4 | 0x8051914C | 4 | no |
| `code_80468434.o` | .sbss | 4 | 0x8066576C | 4 | no |
| `code_804B2FF0.o` | .data/.sbss | 4/4 | …3FC/…944 | 4/4 | no |
| `code_804BC9EC.o` | .bss | 4 | 0x8065F32C | 4 | no |
| `code_804BD8E8.o` | .sbss | 4 | 0x8066597C | 4 | D |
| `snd_StrmPlayer.o` | .bss | 8 | 0x8064FE00 | 0 | D |
| `yvm2.o` | .data | 4 | 0x8056F014 | 4 | no |

(EDS = also carries `extern_data_sections`: category D's payload; the align field is B's.)
All 25 `rule=4` pairs sit on retail starts ≡ 4 (mod 8) — the rule is load-bearing
for the reconstructed LCF wherever it fires (4 of the 16 non-EDS keys are live:
`CNReqtaskSave`, `CNReqtaskSaveBanner`, `CSysWinSelect`, `adx_suwii`). Prior-pass
probes (B doc §5/§7.10, cited not re-run — no compiler here) show MWCC emits 8
for every probed shape and `-align 4` does not change section alignment, so no
source/flag expression exists.

**Recommendation (owner decision needed):** keep all 22 keys until one of —
(a) toolchain LCF-placement work: emit per-unit input placement from `splits.txt`
so input alignment cannot pad (note: `build/us/ldscript.lcf` is dtk-generated
with section-level `ALIGN()` only; whether mwldeppc-1.1 accepts input-qualified
selectors such as `unit.o(.rodata)` is an untested probe for the toolchain owner;
modern CodeWarrior LCF lineage supports `ALIGN()` + location-counter moves and
`*(section)` selectors — AN4497), or (b) a `PLAN.md` §17.6 exception row covering
the class with the per-entry address table above as evidence. Do not pursue per-unit
source/flag fixes (refuted class, §7.10).

## 6. Live-entry verdicts (27 keys — all kept, evidence re-derived retail-side)

`R` = retail object fact re-derived in this run (ELF parse of `build/us/obj` /
`splits.txt` / `symbols.txt`); `P` = carried from the 2026-09-28 packets (decomp
objects unbuildable here — re-verify on a builder before deleting any of these).

| key | verdict | evidence | residual + owner decision |
|---|---|---|---|
| `CLibStaticData.o` | HANDOFF-CODE (+ HANDOFF/TOOL for the dtor) | `P`: drops live TU content (6/6 referenced). `R`: `__dt__10IWorkEventFv` has exactly 1 retail definer (`CTaskGame.o`) → UNDEF-resolves at link | match the emitting functions; dtor needs the out-of-line ownership recipe (compiler-generated-dtor negative result stands) |
| `CNReqtaskSave.o` | tooling-limit (LCF decision) | `R`: §5 row, mod8=4 | owner: §5(a)/(b) |
| `CNReqtaskSaveBanner.o` | tooling-limit (LCF decision) | `R`: §5 row, mod8=4 | owner: §5(a)/(b) |
| `CSysWinSelect.o` | tooling-limit (LCF decision) | `R`: §5 row, mod8=4 | owner: §5(a)/(b) |
| `CTaskManager.o` | HANDOFF-CODE | `P`: `drop_data_range .data 132..204`, referenced live content. `R`: retail `.data`=0x84 | match emitting functions |
| `CWorkSystemMem.o` | near-miss (trim-only) | `R`: `trim_text_size`=352=0x160=retail `.text` exactly | code-match the 0x18 overage, then delete the trim |
| `CfGimmick.o` | HANDOFF-CODE | `P`: 8/8 referenced sdata2 tail | match emitting functions |
| `CfObjectEne.o` | HANDOFF-CODE | `P`: 6/6 referenced sdata2 tail | match emitting functions |
| `HBMAnmController.o` | HANDOFF-CODE + tooling-limit (text) | `P`: data drops referenced. `R`: `__dt__…FrameControllerFv` definers=0 anywhere in retail; retarget target `__RTTI__…FrameController` in `symbols.txt`; retail `.text`=0x110 | data: match code. text: dead-strip orphan (B §7.11 class) — gate-only, needs the DOL-neutrality proof once the link produces a DOL |
| `adx_suwii.o` | tooling-limit (LCF decision) | `R`: §5 row, mod8=4 | owner: §5(a)/(b) |
| `bta_sys_main.o` | HANDOFF-D | `P`: 0 referenced (unreferenced tail) | D's data-ownership batch |
| `btm_devctl.o` | HANDOFF-D | `P`: 0 referenced | D's batch |
| `btm_pm.o` | HANDOFF-D | `P`: 0 referenced | D's batch |
| `btu_init.o` | near-miss (missing-tail) | `R`: retail `.sdata2`=0x8, tail [6,8) all zeros | define the 2 missing bytes in source if attributable, else §17.6 exception; honest path is whole-TU match |
| `dsp.o` | HANDOFF-D | `P`: 0 referenced | D's batch |
| `dvdDeviceError.o` | HANDOFF-D | `P`: 0 referenced | D's batch |
| `dvd_broadway.o` | tooling-limit (dead-strip) | `R`: all 4 dropped syms definers=0 in retail; retail `.text`=0x26B0 | gate-only drops; DOL-neutrality proof once linkable |
| `e_pow.o` | HANDOFF-D | `R`: `trim_sdata2_size`=272=0x110=retail `.sdata2` exactly | D's batch (pool ownership) |
| `fs.o` | HANDOFF-D | `P`: 0 referenced | D's batch |
| `g3d_resmat.o` | HANDOFF-CODE | `P`: 3/3 referenced sdata2 head | match emitting functions |
| `gki_time.o` | near-miss (missing-tail) | `R`: retail `.text`=0x590, tail 12 B zeros | whole-TU match is the honest path (`GKI_remove_from_timer_list` tail) |
| `hcisu_h2.o` | HANDOFF-D | `P`: 0 referenced | D's batch |
| `lyt_group.o` (hbm twin live) | tooling-limit (dead-strip) | `R`: both `LinkList` dtors definers=0; hbm retail `.text`=0x350 | gate-only; DOL-neutrality proof once linkable |
| `lyt_resourceAccessor.o` (hbm; nw4r twin uses scoped rename key) | near-miss (likely cosmetic) | `R`: retail `.data`=0x18 all zeros; rule keeps 24=full size, zeroes [20,24) already-zero bytes; retarget target in `symbols.txt` | one builder run proves it: if raw `.data` is 24 zero-tail bytes, delete all three fields |
| `rfc_ts_frames.o` | HANDOFF-D | `P`: 0 referenced | D's batch |
| `synpitch.o` | near-miss (missing-tail) | `R`: retail `.rodata`=0x18, tail [20,24) zeros | define-or-exception; whole-TU match is honest path |
| `ut_TextWriterBase.o#Q36nw4hbm2ut` (hbm twin) | near-miss (missing-tail) | `R`: hbm retail `.rodata`=0x18, tail zeros | define-or-exception; whole-TU match is honest path |

Skipped (live sibling WIP in main checkout — not touched, not re-verdictd):
`CActorParam.o` (`drop_data_tail`, `drop_nobits_range` + `exact_renames`),
`CBattleManager.o` (`drop_data_tail` + renames/pools/`add_symbols`),
`CItemBoxInfo.o` (`drop_data_tail` + `pool_patterns`).
`snd_SoundArchivePlayer.o` is EDS-bearing (D scope) + WIP → skipped.
`snd_SeqSound.o` has no key.

## 7. Environment notes (for the coordinator)

- **No source files changed.** This worktree cannot build: `build/compilers`,
  `build/tools/{wibo,objdiff-cli,sjiswrap.exe,binutils}` are absent and the
  sandbox blocks downloads (DNS fails; python socket use is denied). Unverified
  source edits would violate the workflow, so none were made; all deletions are
  provably link-inert (§4) and all live verdicts rest on retail-side facts + cited
  prior packets.
- **Worktree-only fix worth knowing:** `orig/us/files` was a broken symlink
  (target absent in the main checkout); `dtk dol split` requires the directory to
  exist, so the symlink was replaced with an empty dir (`rm orig/us/files &&
  mkdir -p orig/us/files`). `config/us/config.yml` references no RELs, so the
  split (1161 objects) is unaffected. `orig/` is gitignored; no repo content changed.
- `coop.json` points `ninja` at the main checkout (`/Users/ahmed/projects/xenoblade/ninja`,
  unreadable from here); `/opt/homebrew/bin/ninja` was used. `run.py`'s configure path
  invokes system `python3` (3.9); all configure work here used `.venv/bin/python3`.

## 8. Claim metadata for the coordinator (no `--claim` run — single-writer rule)

Suggested `--claim "<key>" --owner category-b --class layout --reason` per touched key:

- 29 §2 keys: `"inert-retail 2026-10-10: unit links retail obj, rule never fires; key deleted"`.
- 38 §3 keys: `"inert-retail 2026-10-10: B-fields pruned (rule never fires); kept <fields>"`.
- 4 live align keys: `"tooling-limit 2026-10-10: set_data_align LCF decision, audit §5"`.
- 3 WIP: `"skipped-WIP 2026-10-10: live sibling work in main checkout"`.

## 9. Central-apply items

1. `tools/check_unit_rules_frozen.py --ratchet` after review (29 keys + 74 fields
   awaiting; `--check` green, `--report`: 377 entries, layout 107).
2. `--claim` the §8 reasons (or equivalent verdict annotation).
3. No `configure.py` / `symbols.txt` / `splits.txt` / `targets.json` changes needed.
4. `attempts.jsonl`: nothing to append (no `cycle` runs — no compiler; no §17.6 uses).
5. Builder-side follow-ups: `lyt_resourceAccessor.o` one-build no-op proof (predicted
   deletable); 4 dead-strip text-GC live keys (`HBMAnmController`, `dvd_broadway`,
   `lyt_group`, + `CWorkSystemMem` trim) for the link-DOL neutrality proof once
   `main.elf` links to a DOL.
