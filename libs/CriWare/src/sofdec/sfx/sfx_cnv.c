// CriWare/src/sofdec/sfx/sfx_cnv
#include <harness_catalog.h>
#include "libs/CriWare/src/sofdec/sfx/sfx_types.h"

/* Shared Sofdec SDK error-message pool (.rodata 0x8051CF48, 0x230 bytes) */
extern u8 lbl_eu_8051CF48[];

/* Forward declarations */
int SFX_GetForceSplitField(void);
int SFX_GetSplitField(void* self);
int SFX_GetProgOut(void* self);
int SUD_AnalyTypeDivField(u32 a, u32 b);
int SFXA_IsNeedUpdateLumiTbl(SFXAlphaState* alpha);
void SFXA_MakeAlpLumiTbl(SFXAlphaState* alpha, u32 a, u32 b);
void SFXA_MakeAlp3110Tbl(SFXAlphaState* alpha, u32 a, u32 b);
void SFXA_MakeAlp3211Tbl(SFXAlphaState* alpha, u32 a, u32 b);
void SFXZ_MakeCnvZTbl(void* zmvState, u32 a, u32 b);
void SFXCNV_MakeCcirFromY(u8* table);

void SFXCNV_MakeCcirFromY(u8* table) {
    s32 i;
    for (i = 0; i < 16; i++) {
        table[i] = 0;
    }
    /* Y-to-RGB luma ramp: 1.164*(y-16)+0.5 rounded to nearest. The int->
     * float conversion bias and the two scale constants are read from the
     * shared literal pool (lbl_eu_8051CF38/3C/40) by the postprocess
     * pool_patterns mapping. */
    for (i = 16; i < 0xEC; i++) {
        table[i] = (u8)(s32)(1.164f * (i - 16) + 0.5f);
    }
    for (i = 0xEC; i < 0x100; i++) {
        table[i] = 0xFF;
    }
}
void SFXLIB_Error(void* self, void* stmInf, const char* msg);
int SFXINF_GetStmInf(void* stmInf, const char* name);
int SFX_GetCnvBottomUp(void* self);
int SFX_SetCcirFx(void);
void SFX_ShiftYccPtrByPix(void* stmInf, int xOfs, int yOfs);

BOOL SFX_IsMergeField(SFXConvertState* self, SFXStmInf* stmInf) {
    int splitField;
    BOOL mergeField;

    if (SFX_GetForceSplitField() == 1) {
        return FALSE;
    }

    splitField = SFX_GetSplitField(self);

    if (splitField == -1) goto caseNeg1;
    if (splitField == 0) goto case0;
    if (splitField != 1) goto caseDefault;
    goto case1;

caseNeg1: {
    u32 v0 = stmInf->_88;
    mergeField = FALSE;
    if (v0 == 0) goto done;
    if (SUD_AnalyTypeDivField(v0, stmInf->_8C) != 1) goto done;
    if (SFX_GetProgOut(self) != 0) goto done;
    mergeField = TRUE;
    goto done;
}
case0:
    mergeField = FALSE;
    goto done;
case1:
    mergeField = TRUE;
    goto done;
caseDefault:
    mergeField = FALSE;
done:
    return mergeField;
}

void sfxcnv_MakeTable(SFXConvertState* self, SFXStmInf* stmInf, s32 tableType) {
    BOOL needUpdate = TRUE;
    s32 compoMode = (s32)self->compoMode;

    /* Rebuild the composition table unless the mode is unchanged and the
     * table type needs no refresh (4/5 -> static, 2 -> alpha-dependent). */
    if (compoMode == 0x64) {
        needUpdate = FALSE;
    } else if (compoMode == tableType) {
        if (tableType == 4 || tableType == 5) {
            needUpdate = FALSE;
        } else if (tableType == 2) {
            if (SFXA_IsNeedUpdateLumiTbl(self->alphaState) != 1) {
                needUpdate = FALSE;
            }
        } else if (tableType == 1) {
            needUpdate = FALSE;
        } else if (tableType == 0x15) {
            needUpdate = FALSE;
        }
    }

    if (needUpdate != TRUE) {
        return;
    }

    self->compoMode = tableType;

    /* Retail dispatches with a compare chain (no address jumptable). */
    if (tableType == 0x0B || tableType == 0x0D) {
        SFXZ_MakeCnvZTbl((void*)self->_24, stmInf->_4C, self->_38);
    } else if (tableType == 0x02) {
        SFXA_MakeAlpLumiTbl(self->alphaState, stmInf->_4C, self->_38);
    } else if (tableType == 0x04) {
        SFXA_MakeAlp3110Tbl(self->alphaState, stmInf->_4C, self->_38);
    } else if (tableType == 0x05) {
        SFXA_MakeAlp3211Tbl(self->alphaState, stmInf->_4C, self->_38);
    } else if (tableType == 0x15) {
        void (*cb)(u32) = (void (*)(u32))self->makeColorAdjustCallback;
        if (cb != NULL) {
            cb(self->_38);
        }
    } else if (tableType == 0x01) {
        SFXCNV_MakeCcirFromY((u8*)self->_38);
    } else {
        SFXLIB_Error(self, stmInf, (const char*)&lbl_eu_8051CF48[0x30]);
    }
}

void sfxcnv_ExecCnvFrmByCbFunc(SFXConvertState* self, SFXStmInf* stmInf,
                                SFXDstBufInf* dstBuf, BOOL useTable);
void sfxcnv_ExecCopyAlphaByCbFunc(SFXConvertState* self, SFXStmInf* stmInf,
                                   SFXDstBufInf* dstBuf);

void SFX_CnvFrmByCbFunc(SFXConvertState* self, SFXStmInf* stmInf,
                         SFXDstBufInf* dstBuf) {
    s32 fmt;

    fmt = (s32)self->formatType;
    if (fmt == 0) {
        fmt = (s32)SFXINF_GetStmInf(stmInf, (const char*)&lbl_eu_8051CF48[0xba]);
        self->formatType = (u32)fmt;
    }

    switch (fmt) {
    case 0x11:
        if ((s32)stmInf->_90 == 1) {
            goto cnvByLumiTable;
        }
        sfxcnv_ExecCnvFrmByCbFunc(self, stmInf, dstBuf, FALSE);
        break;
    case 0x1001:
    cnvByLumiTable:
        sfxcnv_MakeTable(self, stmInf, 0x15);
        sfxcnv_ExecCnvFrmByCbFunc(self, stmInf, dstBuf, TRUE);
        break;
    case 0x21:
        sfxcnv_MakeTable(self, stmInf, 1);
        sfxcnv_ExecCnvFrmByCbFunc(self, stmInf, dstBuf, FALSE);
        sfxcnv_ExecCopyAlphaByCbFunc(self, stmInf, dstBuf);
        break;
    case 0x31:
        sfxcnv_MakeTable(self, stmInf, 2);
        sfxcnv_ExecCnvFrmByCbFunc(self, stmInf, dstBuf, TRUE);
        break;
    case 0x41:
    case 0x51:
    case 0x61: {
        s32 tableType;
        if (fmt == 0x51) {
            tableType = 4;
        } else if (fmt == 0x61) {
            tableType = 5;
        } else {
            tableType = 5;
            if ((s32)stmInf->_94 == 0x51) {
                tableType = 4;
            }
        }
        sfxcnv_MakeTable(self, stmInf, tableType);
        sfxcnv_ExecCnvFrmByCbFunc(self, stmInf, dstBuf, TRUE);
        break;
    }
    case 0x101:
        sfxcnv_ExecCnvFrmByCbFunc(self, stmInf, dstBuf, FALSE);
        break;
    default:
        SFXLIB_Error(self, stmInf, (const char*)&lbl_eu_8051CF48[0xc0]);
        break;
    }
}

void sfxcnv_MakeCftSrcBuf(SFXConvertState* self, SFXStmInf* stmInf,
                          SFXCnvSrcBuf* srcBuf);

void sfxcnv_ExecCnvFrmByCbFunc(SFXConvertState* self, SFXStmInf* stmInf,
                                SFXDstBufInf* dstBuf, BOOL useTable) {
    SFXCnvSrcBuf srcBuf;
    u32 table;
    u32 mergeFlag;
    void (*cb)(SFXCnvSrcBuf*, SFXDstBufInf*, u32*);

    memset(&srcBuf, 0, sizeof(SFXCnvSrcBuf));

    sfxcnv_MakeCftSrcBuf(self, stmInf, &srcBuf);

    if (useTable == TRUE) {
        table = self->_38;
    } else {
        table = 0;
    }

    if (self->_0C == 0) {
        SFXLIB_Error(self, stmInf, (const char*)&lbl_eu_8051CF48[0xfa]);
    }

    mergeFlag = (SFX_IsMergeField(self, stmInf) == TRUE) ? 1 : 0;

    if (SFX_GetCnvBottomUp(self) == 1) {
        s32 stride = (s32)dstBuf->_10;
        u32 height = dstBuf->_0C;
        dstBuf->_10 = (u32)(-stride);
        dstBuf->_04 += (u32)((height - 1) * stride);
    }

    cb = (void (*)(SFXCnvSrcBuf*, SFXDstBufInf*, u32*))self->makeCnvFrameCallback;
    if (cb != NULL) {
        cb(&srcBuf, dstBuf, &table);
    }
}

void sfxcnv_MakeCftSrcBuf(SFXConvertState* self, SFXStmInf* stmInf,
                          SFXCnvSrcBuf* srcBuf) {
    memset(srcBuf, 0, sizeof(SFXCnvSrcBuf));

    {
        s32 type = stmInf->srcType;
        if (type == 1) goto case1;
        if (type == 2) goto case2;
        if (type != 3) goto caseDefault;
        goto case3;
case1:
        srcBuf->type = 1;
        srcBuf->_04 = stmInf->_04;
        srcBuf->_08 = stmInf->width;
        srcBuf->_0C = stmInf->_0C;
        srcBuf->_10 = stmInf->_08;
        goto done;
case2:
        srcBuf->type = 1;
        srcBuf->_04 = stmInf->_04;
        srcBuf->_08 = stmInf->width;
        srcBuf->_0C = stmInf->_0C;
        srcBuf->_10 = stmInf->_08;
        goto done;
case3:
        srcBuf->type = 3;
        srcBuf->_04 = stmInf->_04;
        srcBuf->_08 = stmInf->width;
        srcBuf->_0C = stmInf->_0C;
        srcBuf->_10 = stmInf->_08;
        srcBuf->_14 = stmInf->_14;
        srcBuf->_18 = (u32)((s32)stmInf->width / 2);
        srcBuf->_1C = stmInf->_1C;
        srcBuf->_20 = stmInf->_18;
        srcBuf->_24 = stmInf->_24;
        srcBuf->_28 = (u32)((s32)stmInf->width / 2);
        srcBuf->_2C = stmInf->_2C;
        srcBuf->_30 = stmInf->_28;
        goto done;
caseDefault:
        SFXLIB_Error(self, stmInf, (const char*)&lbl_eu_8051CF48[0x138]);
done:
        ;
    }
}

void sfxcnv_ExecCopyAlphaByCbFunc(SFXConvertState* self, SFXStmInf* stmInf,
                                   SFXDstBufInf* dstBuf) {
    SFXCnvSrcBuf srcBuf;
    SFXDstBufInf dstInfo;
    u32 table[8];
    void (*cb)(SFXCnvSrcBuf*, u32*, u32*);

    sfxcnv_MakeCftSrcBuf(self, stmInf, &srcBuf);

    /* 1- or 2-plane source: nothing to adjust (dstInfo stays stale). */
    if (stmInf->srcType - 1 <= 1) {
        /* skip plane adjustment */
    } else if ((s32)stmInf->srcType == 3) {
        /* 3-plane: luma pointer skips a half plane, each chroma pointer a
         * quarter plane; dstInfo._04.._0C are plane buffers, _10 the height. */
        u32 halfPlane = (u32)((s32)(srcBuf._10 * srcBuf._0C) / 2);

        srcBuf._04 += halfPlane;

        u32 quarterPlane = (u32)((s32)halfPlane / 2);
        srcBuf._14 += quarterPlane;
        srcBuf._24 += quarterPlane;

        dstInfo._04 = dstBuf->_04;
        dstInfo._08 = dstBuf->_08;
        dstInfo._0C = dstBuf->_0C;

        if ((s32)self->_08 == 0) {
            ((u32*)&dstInfo)[4] = srcBuf._08 << 2;
            SFXLIB_Error(self, stmInf, (const char*)&lbl_eu_8051CF48[0x172]);
        } else if ((s32)self->_10 == 0 && (s32)self->bytesPerPixelOut != 0) {
            ((u32*)&dstInfo)[4] = (u32)((s32)self->_08 / (s32)self->bytesPerPixelOut);
        } else {
            ((u32*)&dstInfo)[4] = self->_08;
        }
    } else {
        SFXLIB_Error(self, stmInf, (const char*)&lbl_eu_8051CF48[0x1b0]);
    }

    if (SFX_SetCcirFx() == 1) {
        table[0] = self->_38;
    } else {
        table[0] = 0;
    }

    cb = (void (*)(SFXCnvSrcBuf*, u32*, u32*))self->copyAlphaCallback;
    if (cb != NULL) {
        cb(&srcBuf, (u32*)&dstInfo, table);
    }
}

void SFX_SetBytePerPixelOutBuf(void* self, u32 val) {
    ((SFXConvertState*)self)->bytesPerPixelOut = val;
}

void sfxcnv_MakeDstBufInf(SFXConvertState* self, SFXStmInf* stmInf,
                          void* bufPtr, SFXDstBufInf* dstArray, u32 idx) {
    s32 bpp;
    u32 ofs = idx * 16;
    SFXDstBufInf* dstEntry = (SFXDstBufInf*)((u8*)dstArray + ofs);

    /* Destination entries are 16-byte plane records.  The first two stores use
     * dstEntry (dies before the switch).  The later sites re-derive the entry
     * pointer as a u32* so MWCC keeps the per-block address adds (retail
     * recomputes `add r3, r31, r29` at every later site). */
    dstEntry->_04 = (u32)bufPtr;
    dstEntry->_08 = stmInf->width;

    switch ((s32)self->formatType) {
    case 0x11:
    case 0x31:
    case 0x41:
    case 0x51:
    case 0x61:
    case 0x71:
    case 0xF1:
    case 0x111:
    case 0x1001:
        bpp = 0;
        break;
    case 0x21:
    case 0x101:
        bpp = 1;
        break;
    default:
        SFXLIB_Error(NULL, NULL, (const char*)lbl_eu_8051CF48);
        bpp = 0;
        break;
    }

    if (bpp == 1) {
        ((u32*)((u8*)dstArray + ofs))[3] = (u32)((s32)stmInf->bytesPerLine / 2);
    } else {
        ((u32*)((u8*)dstArray + ofs))[3] = stmInf->bytesPerLine;
    }

    if ((s32)self->_08 == 0) {
        ((u32*)((u8*)dstArray + ofs))[4] = stmInf->width;
        SFXLIB_Error(self, stmInf, (const char*)&lbl_eu_8051CF48[0x1f2]);
    } else if ((s32)self->_10 == 0 && (s32)self->bytesPerPixelOut != 0) {
        ((u32*)((u8*)dstArray + ofs))[4] =
            (u32)((s32)self->_08 / (s32)self->bytesPerPixelOut);
    } else {
        ((u32*)((u8*)dstArray + ofs))[4] = self->_08;
    }
}

void SFX_Make2PlaneCftDstBuf(SFXConvertState* self, SFXStmInf* stmInf,
                             void* buf0, void* buf1, int xOfs, int yOfs,
                             SFXDstBufInf* dstArray, u32 width, u32 bytesPerLine) {
    stmInf->width = width;
    stmInf->bytesPerLine = bytesPerLine;

    SFX_ShiftYccPtrByPix(stmInf, xOfs, yOfs);

    memset(dstArray, 0, sizeof(SFXDstBufInf));

    sfxcnv_MakeDstBufInf(self, stmInf, buf0, dstArray, 0);
    sfxcnv_MakeDstBufInf(self, stmInf, buf1, dstArray, 1);

    dstArray->numPlanes = 2;
}
