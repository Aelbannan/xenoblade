# base:gone history restore (2026-09-19)

Read-only git search (`git log -S` / `git show`) for 421 base:gone FULL_MATCH symbols.
Copied **only** function definitions found in history into the working tree. No checkout/revert.

## Results

- History hits (needle in file blob): **328** / 421
- Real definitions extracted & kept: **106**
- Repo-wide extras kept after audit: **16**
- Rejected after audit (still in header / `.ctx.c` / wrong TU): **19**

## Kept restores by file

| n | file |
|--:|------|
| 40 | `libs/RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act.c` |
| 17 | `src/kyoshin/cf/CfNandManager.cpp` |
| 10 | `src/kyoshin/cf/CfRes.cpp` |
| 8 | `src/kyoshin/cf/CfBdat.cpp` |
| 6 | `libs/CriWare/src/sofdec/sfx/sfx_cnv.c` |
| 4 | `libs/CriWare/src/sofdec/sfdcore/mpv/mpv_slice.c` |
| 4 | `libs/nw4r/src/g3d/g3d_calcview.cpp` |
| 3 | `libs/CriWare/src/sofdec/sfdcore/sfd/sfd_mpvf.c` |
| 2 | `src/kyoshin/cf/CtrlPc.cpp` |
| 2 | `libs/CriWare/src/sofdec/sfdcore/mps/mps_dec.c` |
| 1 | `src/kyoshin/CScrollBar.cpp` |
| 1 | `src/kyoshin/COption.cpp` |
| 1 | `src/kyoshin/makecrystal/CModelDispMakeCrystal.cpp` |
| 1 | `src/kyoshin/menu/CMenuEnemyState.cpp` |
| 1 | `src/kyoshin/menu/CMenuQstCnt.cpp` |
| 1 | `src/kyoshin/help/CHelp_ClosePartyMenu.cpp` |
| 1 | `src/kyoshin/help/CHelp.cpp` |
| 1 | `src/kyoshin/cf/chain/CChain.cpp` |
| 1 | `libs/CriWare/src/adx/adxt/adx_tsvr.c` |
| 1 | `libs/monolib/src/core/CPackItem.cpp` |
## Not restored (no definition in history)

Most remaining `base:gone` rows only appear as **calls** or `extern` decls in history
(e.g. many `code_800B06A4` helpers), or as **header inlines that still exist** today
(so the missing object symbol is a mangling/export issue, not deleted source).

## Method

1. `git log -S <needle> -- <unit source>` then `git show <commit>:<path>`
2. Extract brace-balanced definition only
3. Append to current file with a history marker comment
4. Audit-removed: `.ctx.c` scratch, wrong-TU namesakes, bodies still present as header inlines

