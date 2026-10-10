# Category A residual — pure `exact_renames` re-triage (2026-10-10)

<!-- Owner: coordinator-a-residual. Scope: the 13 table entries whose only field is
     `exact_renames` (the residue category A parked as "source handoffs"). -->

## Status

- **9 entries retired** (proven inert; table 406 → 396, `check_unit_rules_frozen.py --check` green,
  119 tombstones, no ratchet yet — coordinator performs one ratchet after the 2026-10-10 B/C/D/E
  batch).
- **4 entries kept** as active / load-bearing, with the residual + next angle recorded below.
- Policy change same day: `PLAN.md` §17.6 no longer approves relocation-name spellings
  (`extern "C" lbl_eu_*`) — see §"Policy" below. None of the four kept fixes may add such
  spellings.

## How "inert" was proven (no build needed)

`rename_exact` (`tools/postprocess_reloc_names.py`) renames only symbols **present in the
source-built object** (it runs `nm` over the object and drops `(old, new)` pairs whose `old` is
absent), and `tools/project.py` only routes **`Object(Matching, …)`** source objects through
`link_reloc_postprocess` at all. Therefore an entry is a *deterministic no-op* when either:

1. the unit is `NonMatching` (the link uses its retail split object; the source object is never
   postprocessed), or
2. every `old` name in the entry is absent from the current source object (`nm` inventory).

Freshness of each source object vs its `.cpp` was verified (`object mtime >= source mtime`) before
using (2). Commands: `build/binutils/powerpc-eabi-nm build/us/src/<unit>.o`,
`grep -nE 'Object\((Matching|NonMatching), "<path>' configure.py`.

## Retired (9)

| key | fields | why inert | evidence |
|---|---|---|---|
| `CBattleState.o` | `exact_renames` (`__vt__Q22cf12CBattleState`→`lbl_eu_8052E9B0`) | unit `NonMatching` (`configure.py:535`); retail-linked | rule never applied |
| `CChainTime.o` | `exact_renames` (vtable) | unit `NonMatching` (`configure.py:700`) | rule never applied |
| `lyt_drawInfo.o` | `exact_renames` (`@5093`→`@230`) | unit `NonMatching` (`configure.py:1690`) | rule never applied |
| `snd_SoundArchive.o` | `exact_renames` (vtable) | unit `NonMatching` (`configure.py:1742`) | rule never applied |
| `snd_WaveSound.o` | `exact_renames` (vtable) | unit `NonMatching` (`configure.py:1764`) | rule never applied |
| `CDeviceFileJob.o` | `exact_renames` (`__vt__14CDeviceFileJob`→`lbl_eu_8056C4D8`) | unit `NonMatching` (`configure.py:1891`) | rule never applied |
| `g3d_resanmscn.o` | `exact_renames` (5× `ResNameData_*`→`lbl_eu_805690E0…`) | source object already **defines** `lbl_eu_805690E0/E100/E120/E140/E160` (`nm … D`); every `ResNameData_*` old name absent → `rename_exact` no-op | `nm build/us/src/nw4r/src/g3d/res/g3d_resanmscn.o` |
| `lyt_resourceAccessor.o#Q34nw4r3lyt` | `exact_renames` (`__vt__Q34nw4r3lyt16ResourceAccessor`→`lbl_eu_80569CA0`) | source object already **defines** `lbl_eu_80569CA0` (`nm … D`); old `__vt__` absent → no-op | `nm build/us/src/nw4r/src/lyt/lyt_resourceAccessor.o` |
| `lyt_bounding.o` | `exact_renames` (5× `@5114..@5118`→`lbl_8054D71C..`) | current object emits `@5107..@5111`; every old `@51xx` in the entry is absent → no-op (stale pool numbering) | `nm build/us/src/RVL_SDK/src/revolution/hbm/nw4hbm/lyt/lyt_bounding.o` |

Claim metadata applied for all nine: owner `coordinator-a-residual`, class `rename`, per-key reason
above.

## Kept (4) — active / load-bearing

`config/us/symbols.txt` cross-check (2026-10-10): each kept rule's retail name is the
config-expected symbol, so deleting it desyncs `dtk dol diff` unless the config entry is renamed via
the `symbols rename-all` ceremony:

| kept key | retail name | in `config/us/symbols.txt`? |
|---|---|---|
| `CArtsSet.o` | `lbl_eu_8052F5D8` | yes |
| `CtrlMoveNpc.o` | `__ct__cf_CtrlMoveNpc` | yes (`0x801A0DAC`, size 0x58) |
| `CLibCriMoviePlay.o` | `@452@__dt__16CLibCriMoviePlayFv` | yes (`0x8045F410`, size 0x8); `thunk_452_dt` absent |
| `ut_Font.o` | `lbl_eu_8056AE30` | yes |

| key | raw-object fact | why it stays | next angle |
|---|---|---|---|
| `CArtsSet.o` | `__vt__Q22cf9_sArtsSet` is an **undefined** reference (`nm … U`); `setArtsSlotRC__…` etc. are defined `T` | the vtable pair retargets a live `U` to `lbl_eu_8052F5D8`; the member renames feed NonMatching retail callers (e.g. `build/us/obj/kyoshin/cf/CtrlObjectParam.o` references `setArtsSlotRC`) | match the owning callers (`CtrlObjectParam`, `CItem`) so they emit the mangled member names, then the renames become inert; or formal `symbols.txt` rename ceremony with owner sign-off |
| `CtrlMoveNpc.o` | `__ct__Q22cf12CCtrlMoveNpcFv` defined `T`; target `__ct__cf_CtrlMoveNpc` is an artifact-style name | NonMatching `CtrlNpc.o` retail split references `__ct__cf_CtrlMoveNpc`; the rule resolves it to this TU's definition | match `CtrlNpc.cpp` so its decompiled ctor call emits the natural mangled name; then delete |
| `CLibCriMoviePlay.o` | `thunk_452_dt` defined `T`; artifact `@452@__dt__16CLibCriMoviePlayFv` referenced by the unit's own retail split + its `.reloc.o` | deleting flips the linked-ELF symbol name; no cross-object definer/use was found, so the open question is `dtk dol diff` symbol expectations, not the link | rebuild + `ninja diff` in a green tree with the key removed; delete if `dol diff` is clean, else record as a `dol diff` name expectation |
| `ut_Font.o` | key matches two units: `RVL_SDK/…/ut_Font` (`Matching`, defines `...data.0` `d`) and `nw4r/ut/ut_Font` (`NonMatching`) | the rename is active for the RVL unit; **`nw4r/src/ut/ut_CharStrmReader.o` (Matching) also defines `lbl_eu_8056AE30` (`D`)** — the same label from two source objects is a multiply-defined anomaly worth resolving with the pool-label work (category E) before touching this key | reconcile data ownership (`ut_CharStrmReader` vs RVL `ut_Font`); prefer a source-side single owner |

## Policy (2026-10-10)

`PLAN.md` §17.6's "Relocation name drift" exception row was removed; `SKILL.md` angles/exceptions/
routing and `MWCC_PATTERNS.md` §1 were updated to match, and `tools/pi_harness/lint.py` no longer
permits `extern "C" lbl_*` as the escape. Existing `extern "C" lbl_eu_*` spellings are legacy
cleanup debt, not an approved fix. This is why the four kept entries above are *not* to be fixed by
adding/keeping source label spells.

## Verification run

```bash
.venv/bin/python3 tools/check_unit_rules_frozen.py --check    # OK: within frozen baseline
.venv/bin/python3 tools/check_unit_rules_frozen.py --report   # entries 396 (baseline 406, retired 119)
.venv/bin/python3 -c "import tools.postprocess_reloc_names as p; print(len(p.UNIT_RULES))"  # 396
```

Backup of the pre-batch table: `.scratch/unitrules_a_residual/postprocess_reloc_names.py.pre_a_residual`.

## Not done / needs a green tree

- No link (`main.elf`) or `dtk dol diff` proof was possible in this tree state: the main checkout's
  link is red with pre-existing undefineds, and the paseo worktrees cannot download the toolchain
  (coordinator unblocked the worktrees with cached-tool symlinks — see the 2026-10-10 B/C/D/E batch
  reports). The nine retirements are no-op by construction, so they need no link proof; the four
  kept ones still need the `ninja diff` step above.
- One coordinator `--ratchet` should run once the 2026-10-10 B/C/D/E batch lands (this batch is
  deletion-only; baseline stays 406 until then).
