# Category C run 2026-10-10 — content-rule triage (STATIC-ONLY; build-blocked)

Owner: category-C subagent (paseo-unitrules-c worktree).
Status: **no deletions**. Worktree cannot compile (no `build/tools/wibo`,
no `sjiswrap.exe`; network/DNS blocked so `download_tool.py` cannot fetch
them; sandbox denies symlinking the main checkout's cached toolchain).
All verdicts below are **static-only**: retail-object analysis (pyelftools
over `build/us/obj/**`), UNIT_RULES table-comment review, `targets.json`
state survey, source-shape review. Every deletion/recertify step needs a
wibo-backed follow-up (`hexdiff --all` + `run.py data diff <unit>` raw +
`run.py cycle` + link rebuild).

Live set re-derived with the README snippet (adapted: this worktree's link
rule is `build build/us/main.elf | build/us/main.elf.MAP: link $`):
**content entries=28, live=9** — the same 9 as the category prompt:
`CProc.o CTaskManager.o mtx.o AXFXReverbHiExp.o AXFXChorusExp.o
AXFXChorusExpDpl2.o OSNet.o rfc_mx_fsm.o rfc_port_fsm.o` (79 `.reloc.o`
bases in the link). `code_80135FDC.o` carries **no key** any more (only a
residual comment) — already retired, no action.
`--check` passes (406/406, retired 109); `--report`: content=10 (claim
metadata) vs 28 derived — the 18 gap is unclaimed triage-class content.

WIP-skip overlap: **none** (no content key basename is in
CItemBoxInfo/CActorParam/CBattleManager/snd_SoundArchivePlayer/snd_SeqSound).

## 1. Deleted keys — NONE (blocked)

No `UNIT_RULES` edits made (deletions-only policy respected; nothing met the
raw-gate + link-evidence + hexdiff-before/after bar without a compiler).

## 2. Kept keys — per-entry verdicts (all static-only)

Legend: NM-code = near-miss, code-driven (match ACTIVE fns, re-derive);
NM-mask = near-miss, rule masks a likely code residual (recertify first);
SRC-CAND = concrete source fix hypothesised, needs build verify;
TOOL = genuine tooling/linker limit, leave + packet; DISS = data-ownership /
blob-dissolve shape, leave (D-adjacent).

### LIVE (9)

| key | verdict | evidence (retail .o) | residual + next build-backed step |
|---|---|---|---|
| `mtx.o` | NM-code, SRC-CAND | pool `[1.0,0.0,0.5,3.0,2.0,-1.0,0x3C8EFA35]`, rule swaps leading pair. 34 targets FULL under `data_value` (reloc VALUES equal; only pool slots differ) + 2 BACKLOG (`C_MTXLightFrustum` us-80350780, `C_MTXLightOrtho` us-80350930). First-use order: ours pools 0.0f first (suspect `mtx_ps.inl:38` `c0=0.0f` or TU order), retail 1.0f first. `Unit01={0,1.0}` lives in `.sdata`, not `.sdata2` — not the cause. | Match the 2 BACKLOG fns; if pool still flipped, move a 1.0f first-use earlier (or 0.0f later) with zero codegen delta, `hexdiff --all` + raw data gate. No demotions (code FULL). |
| `AXFXReverbHiExp.o` | NM-code | pool head `[32000.0, 0.0, …]` (0x40 pool, rest already matches after prior exp-local pow rewrite per table comment). 6/7 FULL; `__InitParams` us-802d8c10 BACKLOG. | Match `__InitParams`; reorder exp-local pow literals so 32000.0f pools first (cf. KB `f6783218a4`: compute exponent in a local first). |
| `AXFXChorusExp.o` | NM-code | pool tail `[65536,256,32000,0.00390625]` vs MWCC `[256,32000,0.00390625,65536]` (3-swap rotation at 0x20-0x2C). 3 FULL + 4 ACTIVE CODE_MATCH 97.9–99.9 (`AXFXChorusExpInit` us-802dbd90, `SettingsUpdate` us-802dbff0, `__InitParams` us-802dc590, `__CalcLFO` us-802dc790). | Match the 4; pool should collapse to retail order, rule deletable. No demotions. |
| `AXFXChorusExpDpl2.o` | NM-code | same rotation, Dpl2 twins. 3 FULL + 4 ACTIVE (us-802dc8b0, us-802dcb30, us-802dd100, us-802dd310). | Same as above. |
| `OSNet.o` | **SRC-CAND (strongest)** | `.data` 0x161: strings only in swap range (no relocs there — link-affecting but behaviour-neutral). Retail order `…%d\n, NWC24iPrepareShutdown, /dev/net/kd/request, NWC24Suspend…`; ours pools kd/req first. TU function order == retail `.text` order, so cause is intra-TU first-use: `NWC24SuspendScheduler` (line ~113) uses kd/req before the discarded `(void)"NWC24iPrepareShutdown"` casts in `NWC24iRequestShutdown` (~line 170); within `NWC24iPrepareShutdown` itself MWCC pooled the kd/req arg before `__FUNCTION__`. 7 FULL + 1 EQUIV. | Hoist `(void)"NWC24iPrepareShutdown";` to the top of `NWC24iPrepareShutdown()` (before the `NWC24iOpenResourceManager_` call) — pools Prepare first with (expected) zero codegen change; verify both callers stay FULL + raw `.data` order flips, delete rule. |
| `CProc.o` | NM-code | `.data` 0xC8 all-zero, vtable symbols (`__vt__CProc`@0, `reslist<Ul>`@0xA0, base-list@0xAC, typeinfo@0xB8); `.data` relocs confirm inject targets (`lbl_80663538`@160, `lbl_80663540`@172+184); `.sdata` 16 zero with `lbl_8056B28C` ref@4. 9 FULL + `CProc::CProc` us-8043b5c8 HIGH 86.5 + `pssCreateView` us-8043bc8c HIGH 88.7. `trim_text_size`=2844=retail `.text` (ours bigger). | Match ctor + pssCreateView (ctor likely owns reslist vtable emission order); re-derive swap/inject/pad/trim. No demotions. |
| `CTaskManager.o` | NM-code (vtable-shape) | All 16 FULL. Retail `.data` 132 zero + relocs (2 vtable images + `updateMsg` jumptable → `CView::updateMsg` + addends). Rule drops ours 204→132 (MWCC appends CRootProc/CTTask vtables @+0x84) + injects dtor/Move/Draw `CTTask<RootProc>` @44/60/64 (words we zero, retail relocs). | Reshape `CTTask<RootProc>` vtable emission (class/base-list/`-RTTI` ownership experiments) to emit retail's 132B shape; raw data gate. No demotions (code FULL). |
| `rfc_mx_fsm.o` | **NM-mask (stale ACCEPT?)** | All 10 FULL, yet `.data`+624 word=0 + RELA → `rfc_mx_sm_state_disc_wait_ua` addend **300** (label @ func+300 = `.text`+0xA14); table comment says decomp emits **296**. ±4B ⟹ current-source case label sits 4B earlier ⟹ our codegen differs ⟹ ACCEPTED/FULL state of us-80301fbc is stale or was certified under different source. | Rebuild + `hexdiff --symbol rfc_mx_sm_state_disc_wait_ua`; expect ~4B drift. Likely demote us-80301fbc (near-miss) + delete rule. NO deletion/demotion without rebuild. |
| `rfc_port_fsm.o` | **NM-mask (stale ACCEPT?)** | All 17 FULL, yet +0x228 retail 192 vs decomp 272 (= retail +0x22c value — shifted/duplicated caseattr) in `rfc_port_sm_orig_wait_sec_check` (us-80302874) + second pin @+0x22c=272. Same conclusion as above. | Rebuild + hexdiff us-80302874; likely demote + delete rule. |

### LATENT (19)

| key | verdict | note |
|---|---|---|
| `CNBanner.o` | NM-code | drops/repack/`data_reloc_offset_moves` + 1 `drop_text_symbols`; 6 FULL + ctor STRUCTURAL 32% (us-804f96c0) + dtor HIGH 93% + 1 BACKLOG. Match ctor/dtor first. |
| `CGXCache.o` | TOOL/NM-code mix | `add_symbols` = dissolve naming (needs link proof); `addend_patches` = documented HIGH_MATCH §17.6 residual (`func_80449D68` sels 4..13 retail-4). 30 FULL + 18 ACTIVE incl. 7 COMPILES. Leave; residual already packeted in table comment. |
| `CScn_80496B0C.o` | DISS | `zero_data_range` + pad + injects + carrier undefs (monolibdata dissolve; MWCC migrates zero arrays to `.bss`). 18 FULL + 11 ACTIVE. Leave. |
| `KPAD.o` | NM-code | 28-word pool with duplicate 0.0-words @13–15; drop + 28-permute; 20 FULL + 2 EQUIV + ACTIVE `read_kpad_ext` HIGH 0.7% / `KPADiRead` 16% / `KPADInitEx` 9% / `KPADiSamplingCallback` BACKLOG. "Decomp order follows its unmatched text" — match the 4, re-derive. |
| `CTaskEnvironment.o` | NM-code/DISS | magic rename + sdata2-tail drop + 7 vtable injects (CTaskManager precedent); 18 FULL + 18 ACTIVE (dtor/Term/Move/Draw ACTIVE). Leave to matching. |
| `code_804BC9EC.o` | TOOL | 20× `addend_sets` = documented §17.6 residual (`func_804BC9F4` dense 20-entry switch; `-ipa` variant drift 544/388/440; absolute pins). 10 FULL + 2 COMPILES. Leave + table comment is the packet. |
| `CView.o` | NM-code | 10-swap set (data/sdata/rodata string+vtable order) + drops/pads; 16 FULL + 1 EQUIV + 28 ACTIVE (mostly COMPILES/STRUCTURAL). Heavily matching-gated. Leave. |
| `CWorkThread.o` | NM-code | 5× `addend_sets` = documented wkStandby 13.2% §17.6 residual + vtable/RTTI renames (weak `'<'`-name structs); ctor/dtor HIGH 86–89% ACTIVE (us-80439f54, us-8043a460). Match ctor/dtor/wkStandby, re-derive. |
| `CChildListNode.o` | TOOL (GC) + SRC-CAND | drops = linker-GC orphans (strong copies in CProcess.o per comment) + pad/inject/rename for 16B template vtable w/ RTTI slot; 2/2 FULL. Hypothesis: source-side weak-dtor kill (CDeviceClock pattern) could retire the text drops; needs build. Leave. |
| `lyt_layout.o` | TOOL (GC) | 4× `drop_text_symbols` = unreferenced weak orphans (B §7.7 retail-linker-GC pattern; B §7.6: restructuring regresses matches). 27 FULL + 7 ACTIVE. Leave. |
| `lyt_pane.o#Q36nw4hbm3lyt` | TOOL (GC) | Scoped hbm twin, same orphan pattern; 58 FULL + 6 open. Leave. |
| `CDeviceFileCri.o` | NM-code | 3 renames (incl. `__dt__FP10IExceptionFv` extab body) + 8× `addend_patches` (wkUpdate residual); 12 FULL + 13 ACTIVE. Leave to matching. |
| `CDevice.o` | DISS | zero/drop/nobits ranges + renames; 13 FULL + 4 ACTIVE. Leave. |
| `CDeviceClock.o` | TOOL (GC) + NM-code | `drop_text_symbols_as_undef` weak-dtor kill + tails + `'<'`→`<>` spelling renames; 8 FULL + 4 ACTIVE. Leave. |
| `g3d_state.o` | DISS/repr | `.text` retargets onto blob anchor (reloc-symbol-only) + inject/pad/extern/pool + drops; 30 FULL + 1 EQUIV + 16 ACTIVE. Leave. |
| `vi.o` | NM-code | 2× `drop_data_range` + 9× `addend_sets` (`__VIRetraceHandler` HIGH **16.3%** us-80366e40 + `getTiming`); 24 FULL + 4 ACTIVE. Match RetraceHandler + `VIGetNextField` (98.59), re-derive. |
| `WUD.o` | NM-code | tail/zero/renames + 10× injects (jumptable `jumptable_80562FA0` + `__wudSecurityEventStackCallback` CODE 98.5 us-8037db40) + `.text` retargets; 58 FULL + 2 EQUIV + 26 ACTIVE. Leave to matching. |
| `dct_isr.o` | NM-code | rodata-tail drop + 4× `.text` retargets + 4× `.text` addends (blob-table addressing); 2 FULL + 4 STRUCTURAL (us-803bd574 33%, us-803bd704 51%, two ~1%). Leave. |
| `sfd_tim.o` | NM-code | rodata-tail drop + 2× `.text` retargets + 2× `.text` addends; 26 FULL + 7 EQUIV + 13 ACTIVE. Leave. |

`code_80135FDC.o`: no key (comment-only residual) — already retired, nothing to do.

Proposed (NOT applied — needs wibo): demote us-80301fbc + us-80302874 to
recorded near-miss IF rebuild confirms the addend drift; all other affected
targets stay as-is.

## 3. Source files changed

*None under `src/**` / `libs/**`* (no edit verifiable without a compiler).
Worktree setup/config only:
- `coop.json`: `"ninja"` repointed main-checkout → `./ninja` (local wrapper;
  old value unusable from this sandbox). Revertible one-liner.
- `orig/us/files`: dangling symlink → empty local dir (target absent; dtk
  DOL split requires the path to exist). Restore: `rmdir orig/us/files &&
  ln -s /Users/ahmed/projects/xenoblade/orig/us/files orig/us/files`.
- Regenerated: `build.ninja` (1.8M), `objdiff.json` (1.2M), `build/us/{asm,obj,config.json,ldscript.lcf,…}` (retail DOL split OK),
  `build/compilers` (146M, fully downloaded), `build/tools/dtk`,
  `tools/objdiff` untouched (pre-existing arm64 cli, works).
- New: this doc + `.scratch/catc/*.py` analysis scripts (gitignored).

## 4. Claim metadata for the coordinator (do NOT run --claim here)

All class `content` (triage-derived; `--report` currently shows 10 claimed):

`CTaskManager.o` vtable-shape near-miss, all-code-FULL · `CNBanner.o`
code-driven (ctor STRUCTURAL) · `CGXCache.o` §17.6 jumptable residual +
dissolve naming · `CScn_80496B0C.o` blob-dissolve ownership ·
`rfc_mx_fsm.o` masked addend residual, recertify us-80301fbc ·
`rfc_port_fsm.o` masked addend residual, recertify us-80302874 · `mtx.o`
pool first-use order · `KPAD.o` pool duplicates follow unmatched text ·
`AXFXReverbHiExp.o` leading-pair pool order · `AXFXChorusExp.o` /
`AXFXChorusExpDpl2.o` trailing-window rotation, match 4+4 fns · 
`CTaskEnvironment.o` vtable injects + ACTIVE dtors ·
`code_804BC9EC.o` §17.6 dense-switch residual (tooling-limit) ·
`CProc.o` reslist emission order + 2 HIGH fns · `CView.o`
string/vtable-order, matching-gated · `CWorkThread.o` wkStandby residual +
ctor/dtor · `CChildListNode.o` GC orphans, weak-dtor-kill candidate ·
`lyt_layout.o` / `lyt_pane.o#Q36nw4hbm3lyt` linker-GC orphans (tooling-limit)
· `CDeviceFileCri.o` wkUpdate residual · `CDevice.o` / `CDeviceClock.o`
dissolve + weak-dtor kill · `g3d_state.o` blob-anchor retargets ·
`vi.o` RetraceHandler-driven addends · `WUD.o` SecurityCallback-driven
injects · `OSNet.o` string-order, discarded-cast hoist candidate ·
`dct_isr.o` / `sfd_tim.o` blob-table addressing, matching-gated.

Suggested reason strings are the per-row notes above prefixed
`cat-c-static-2026-10-10:`.

## 5. Coordinator central actions

- Provide `build/tools/wibo` + `build/tools/sjiswrap.exe` (+ `build/binutils`):
  network blocked here (`nodename nor servname`); sandbox denies referencing
  the main checkout (absolute *and* `..`-relative), so the suggested symlink
  workaround could not be applied. With wibo present, first jobs:
  (a) `hexdiff.py OSNet --all` after hoisting the Prepare cast (predicts
  FULLs retained + pool flip);
  (b) `hexdiff.py rfc_mx_fsm --symbol rfc_mx_sm_state_disc_wait_ua` and
  `rfc_port_fsm --symbol rfc_port_sm_orig_wait_sec_check` (predicts ~4B /
  case-shift drift → demote + delete);
  (c) `run.py data diff mtx` raw (predicts only pool-order delta).
- `build.ninja`'s `python = "/Users/ahmed/projects/xenoblade/.venv/bin/python3"`
  predates this run (same header before reconfigure) — builds invoking `$python`
  may need repointing to the worktree venv when wibo lands.
- `attempts.jsonl` lines to append: **none** (no acceptance events; per
  instructions nothing appended here).
- symbols/splits/configure edits: **none** required from this run.
- B-handover `pad_*` items (`btu_init.o`, `hcicmds.o`, …): layout-class, out
  of C scope — not triaged here.

## Coordinator cross-check + port provenance (2026-10-10, main checkout)

- **Ported** from `paseo/unitrules/c` by the coordinator: this doc + `.scratch/catc/` analysis
  scripts. C's `coop.json` (local `./ninja` path tweak) and regenerated build artifacts were
  deliberately **not** ported; `docs/handoff/unit_rules/c_byte_falsification.md` was unchanged on the
  branch.
- **The rfc claim is disproven by the live gate.** In the main checkout (toolchain present):
  `hexdiff RVL_SDK/.../rfc_mx_fsm --symbol rfc_mx_sm_state_disc_wait_ua` → **100.0% | 0 structural |
  0 reg_swap | size PASS**; same for `rfc_port_fsm` / `rfc_port_sm_orig_wait_sec_check`. Only
  reloc-name drift (`@868`→`@540`, section-symbol `...data.0`) is reported, which the comparator
  ignores. **Do not demote us-80301fbc / us-80302874 and do not delete the `addend_sets` rules on
  that basis** — the static addend reading did not reproduce.
- **Build-blocked status is superseded for follow-up:** the main checkout has
  wibo/sjiswrap/dtk/compilers, so §5's jobs (a) `OSNet` hoist, (b) rfc re-checks (done above),
  (c) `mtx` raw diff can be run here.
- Claim metadata applied centrally: all 28 keys → owner `category-c`, class `content`, reason
  `cat-c-static-2026-10-10: see this doc §2 for the per-entry verdict`.
