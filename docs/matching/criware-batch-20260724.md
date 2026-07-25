# CriWare Batch Match — 14/20 Targets

Date: 2026-07-24
Agent: ahmed

## Scope

20 NOT_STARTED CriWare (CRI ADX/Sofdec) functions from P9 catalog.

## Results

### 14 ACCEPTED (FULL_MATCH)

| # | Target ID | Function | Module | Size |
|---|-----------|----------|--------|:----:|
| 1 | `us-803a553c` | MWSFD_GetCond | mwsfdset | 0x18 |
| 2 | `us-803a55a4` | MWSFD_IsEnableHndl | mwsfdset | 0x18 |
| 3 | `us-803c2430` | SFBUF_SetTermFlg | sfd_buf | 0x18 |
| 4 | `us-80383bbc` | ADXSJD_GetLpStartOfst | adx_sjd | 0x1C |
| 5 | `us-80388a64` | ADXPD_GetExtPrm | adx_xpnd | 0x1C |
| 6 | `us-80391a54` | AHXDCD_GetTotalNumSmpl | ahx_dcd | 0x1C |
| 7 | `us-803a1ac4` | MWSFTAG_InitTagInf | mwsfdsfx | 0x1C |
| 8 | `us-803a98e4` | MPVERR_InitErrInf | mpv_err | 0x1C |
| 9 | `us-803c2900` | SFHDS_InitFhd | sfd_hds | 0x1C |
| 10 | `us-803c34dc` | fn_803C34DC | sfd_lib | 0x1C |
| 11 | `us-803ce16c` | SFPTS_InitPtsQue | sfd_pts | 0x1C |
| 12 | `us-803d13e4` | SFTRN_IsSetup | sfd_trn | 0x1C |
| 13 | `us-80388b44` | ADXPD_Start | adx_xpnd | 0x20 |
| 14 | `us-80391484` | AHXDCD_Finish | ahx_dcd | 0x1C |

### 6 Not Accepted (released for others)

| Target ID | Function | Best | Issue |
|-----------|----------|:----:|-------|
| `us-803d3a48` | VER1_AnlyHdrSfhVer | STRUCTURAL | r0/r3 reg-swap on return value store |
| `us-8038a328` | adxb_DefAddWr | HIGH_MATCH | Instruction ordering, proven equivalent |
| `us-8038da7c` | ADXT_DetachAhx | CODE_MATCH | Global addr in r3 vs r4, not equivalent |
| `us-803c58d0` | SFD_SetMbCb | STRUCTURAL | Complex pointer chain |
| `us-80388a80` | ADXPD_Destroy | HIGH_MATCH | stw/li reordering |
| `us-80388ea8` | ADXB_SetAhxInSj | CODE_MATCH | Global addr in r4 vs r5, not equivalent |

## Changed Files

- `libs/CriWare/src/sofdec/mwply/mwsfdset.c`
- `libs/CriWare/src/sofdec/sfdcore/sfd/sfd_buf.c`
- `libs/CriWare/src/sofdec/sfdcore/sfh/sfh_ver1.c`
- `libs/CriWare/src/adx/adxt/adx_sjd.c`
- `libs/CriWare/src/adx/adxt/adx_xpnd.c`
- `libs/CriWare/src/adx/adxt/adx_bsc.c`
- `libs/CriWare/src/adx/ahx/ahx_link.c`
- `libs/CriWare/src/adx/ahx/ahx_dcd.c`
- `libs/CriWare/src/sofdec/mwply/mwsfdsfx.c`
- `libs/CriWare/src/sofdec/sfdcore/mpv/mpv_err.c`
- `libs/CriWare/src/sofdec/sfdcore/sfd/sfd_hds.c`
- `libs/CriWare/src/sofdec/sfdcore/sfd/sfd_lib.c`
- `libs/CriWare/src/sofdec/sfdcore/sfd/sfd_mpv.c`
- `libs/CriWare/src/sofdec/sfdcore/sfd/sfd_pts.c`
- `libs/CriWare/src/sofdec/sfdcore/sfd/sfd_trn.c`
- `libs/CriWare/src/adx/adxt/adx_bahx.c`

## Learnings

1. **Branch inversion**: MWCC generates `bne` when checking `self == NULL` (equality) and `beq` when checking `self != NULL` (inequality). This is counterintuitive.
2. **Signed comparison**: Use `s32` for comparisons with constants when retail uses `cmpwi` (signed compare immediate).
3. **3-param trick**: C functions with unused second param force addend into r5 matching retail register allocation.
4. **Instruction ordering**: MWCC aggressively schedules `li` before dependent `stw` — no C-level barrier (volatile, expression nesting) prevents this.
5. **Equivalence cap**: HIGH_MATCH (50-99%) with proven equivalence doesn't auto-promote to EQUIVALENT_MATCH via the cycle command.
