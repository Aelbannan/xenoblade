// CriWare/src/sofdec/sfx/sfx_cnv
#include <harness_catalog.h>
#include "libs/CriWare/src/sofdec/sfx/sfx_types.h"

/* Shared Sofdec SDK error-message pool (.rodata 0x8051CF48, 0x230 bytes) */
extern const unsigned char lbl_eu_8051CF48[0x230];

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

/* Parameter block handed to the frame-conversion callback (0x20 bytes). */
typedef struct SFXCnvCbParam {
    u32 table;       /* +0x00: color-adjust table no. (0 when unused) */
    u32 _04;         /* +0x04: copy of SFXConvertState::_50 */
    u8 _08[0x10];
    u32 _18;         /* +0x18: src height check value */
    u32 mergeField;  /* +0x1c: 1 when field-merge conversion */
} SFXCnvCbParam;

void sfxcnv_ExecCnvFrmByCbFunc(SFXConvertState* self, SFXStmInf* stmInf,
                                SFXDstBufInf* dstBuf, BOOL useTable) {
    SFXCnvSrcBuf srcBuf;
    SFXCnvCbParam param;
    void (*cb)(SFXCnvSrcBuf*, SFXDstBufInf*, SFXCnvCbParam*);

    memset(&srcBuf, 0, sizeof(SFXCnvSrcBuf));

    sfxcnv_MakeCftSrcBuf(self, stmInf, &srcBuf);

    if (useTable == TRUE) {
        param.table = self->_38;
    } else {
        param.table = 0;
    }

    param._04 = self->_50;

    /* Zero source height: fall back to the line size and report. */
    if (self->_0C == 0) {
        param._18 = stmInf->bytesPerLine;
        SFXLIB_Error(self, stmInf, (const char*)&lbl_eu_8051CF48[0xfa]);
        goto heightDone;
    }
    param._18 = self->_0C;
heightDone:;

    if (SFX_IsMergeField(self, stmInf) == TRUE) {
        param.mergeField = 1;
    } else {
        param.mergeField = 0;
    }

    if (SFX_GetCnvBottomUp(self) == 1) {
        /* Bottom-up output: negate the line pitch and point the plane at
         * the last line instead of the first. */
        dstBuf->_04 += dstBuf->_10 * ((s32)dstBuf->_0C - 1);
        dstBuf->_10 = (u32)(-(s32)dstBuf->_10);
    }

    cb = (void (*)(SFXCnvSrcBuf*, SFXDstBufInf*, SFXCnvCbParam*))self->makeCnvFrameCallback;
    if (cb != NULL) {
        cb(&srcBuf, dstBuf, &param);
    }
}

void sfxcnv_MakeCftSrcBuf(SFXConvertState* self, SFXStmInf* stmInf,
                          SFXCnvSrcBuf* srcBuf) {
    /* Retail dispatches on the source plane count with a compare chain
     * (no jumptable); planes 1 and 2 share the same fill-out. */
    if ((s32)stmInf->srcType == 1) {
        srcBuf->type = 1;
        srcBuf->_04 = stmInf->_04;
        srcBuf->_08 = stmInf->width;
        srcBuf->_0C = stmInf->_0C;
        srcBuf->_10 = stmInf->_08;
    } else if ((s32)stmInf->srcType == 2) {
        srcBuf->type = 1;
        srcBuf->_04 = stmInf->_04;
        srcBuf->_08 = stmInf->width;
        srcBuf->_0C = stmInf->_0C;
        srcBuf->_10 = stmInf->_08;
    } else if ((s32)stmInf->srcType == 3) {
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
    } else {
        SFXLIB_Error(self, stmInf, (const char*)&lbl_eu_8051CF48[0x138]);
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
                             void* buf0, void* buf1, SFXDstBufInf* dstArray,
                             int xOfs, int yOfs, u32 width, u32 bytesPerLine) {
    stmInf->width = width;
    stmInf->bytesPerLine = bytesPerLine;

    SFX_ShiftYccPtrByPix(stmInf, xOfs, yOfs);

    memset(dstArray, 0, sizeof(SFXDstBufInf));

    sfxcnv_MakeDstBufInf(self, stmInf, buf0, dstArray, 0);
    sfxcnv_MakeDstBufInf(self, stmInf, buf1, dstArray, 1);

    dstArray->numPlanes = 2;
}

/* Absorbed .rodata 0x8051CF48 (0x230) */
const unsigned char lbl_eu_8051CF48[0x230] = {
    0x45,0x32,0x30,0x31,0x33,0x31,0x32,0x3A,0x20,0x73,0x66,0x78,
    0x63,0x6E,0x76,0x5F,0x49,0x73,0x43,0x6E,0x76,0x55,0x70,0x48,
    0x61,0x6C,0x66,0x20,0x3A,0x20,0x63,0x6F,0x6D,0x70,0x6F,0x20,
    0x69,0x73,0x20,0x69,0x6E,0x76,0x61,0x6C,0x69,0x64,0x2E,0x00,
    0x45,0x32,0x30,0x31,0x33,0x31,0x31,0x3A,0x20,0x73,0x66,0x78,
    0x63,0x6E,0x76,0x5F,0x4D,0x61,0x6B,0x65,0x54,0x61,0x62,0x6C,
    0x65,0x20,0x3A,0x20,0x63,0x6F,0x6D,0x70,0x6F,0x20,0x69,0x73,
    0x20,0x6E,0x6F,0x74,0x20,0x73,0x75,0x70,0x70,0x6F,0x72,0x74,
    0x2E,0x00,0x45,0x32,0x30,0x32,0x32,0x38,0x32,0x3A,0x20,0x53,
    0x46,0x58,0x5F,0x4D,0x61,0x6B,0x65,0x54,0x62,0x6C,0x5A,0x31,
    0x36,0x20,0x3A,0x20,0x7A,0x63,0x6C,0x69,0x70,0x20,0x69,0x73,
    0x20,0x6E,0x6F,0x74,0x20,0x73,0x65,0x74,0x2E,0x00,0x45,0x32,
    0x30,0x32,0x32,0x38,0x31,0x3A,0x20,0x53,0x46,0x58,0x5F,0x4D,
    0x61,0x6B,0x65,0x54,0x62,0x6C,0x5A,0x33,0x32,0x20,0x3A,0x20,
    0x7A,0x63,0x6C,0x69,0x70,0x20,0x69,0x73,0x20,0x6E,0x6F,0x74,
    0x20,0x73,0x65,0x74,0x2E,0x00,0x43,0x4F,0x4D,0x50,0x4F,0x00,
    0x45,0x34,0x31,0x31,0x31,0x39,0x30,0x32,0x3A,0x20,0x73,0x66,
    0x78,0x63,0x6E,0x76,0x5F,0x43,0x68,0x6B,0x43,0x6F,0x6D,0x70,
    0x6F,0x42,0x79,0x43,0x62,0x46,0x75,0x6E,0x63,0x20,0x3A,0x20,
    0x63,0x6F,0x6D,0x70,0x6F,0x20,0x69,0x73,0x20,0x6E,0x6F,0x74,
    0x20,0x73,0x75,0x70,0x70,0x6F,0x72,0x74,0x2E,0x00,0x45,0x30,
    0x35,0x30,0x37,0x30,0x38,0x30,0x36,0x3A,0x20,0x70,0x6C,0x65,
    0x61,0x73,0x65,0x20,0x73,0x65,0x74,0x20,0x70,0x69,0x74,0x63,
    0x68,0x20,0x62,0x79,0x20,0x6D,0x77,0x50,0x6C,0x79,0x46,0x78,
    0x53,0x65,0x74,0x4F,0x75,0x74,0x42,0x75,0x66,0x50,0x69,0x74,
    0x63,0x68,0x48,0x65,0x69,0x67,0x68,0x74,0x28,0x29,0x2E,0x00,
    0x45,0x34,0x31,0x31,0x31,0x39,0x30,0x31,0x3A,0x20,0x73,0x66,
    0x78,0x63,0x6E,0x76,0x5F,0x4D,0x61,0x6B,0x65,0x43,0x66,0x74,
    0x53,0x72,0x63,0x42,0x75,0x66,0x20,0x3A,0x20,0x66,0x72,0x61,
    0x6D,0x65,0x20,0x66,0x6F,0x72,0x6D,0x61,0x74,0x20,0x69,0x73,
    0x20,0x69,0x6E,0x76,0x61,0x6C,0x69,0x64,0x2E,0x00,0x45,0x30,
    0x35,0x30,0x37,0x30,0x38,0x30,0x37,0x3A,0x20,0x70,0x6C,0x65,
    0x61,0x73,0x65,0x20,0x73,0x65,0x74,0x20,0x70,0x69,0x74,0x63,
    0x68,0x20,0x62,0x79,0x20,0x6D,0x77,0x50,0x6C,0x79,0x46,0x78,
    0x53,0x65,0x74,0x4F,0x75,0x74,0x42,0x75,0x66,0x50,0x69,0x74,
    0x63,0x68,0x48,0x65,0x69,0x67,0x68,0x74,0x28,0x29,0x2E,0x00,
    0x45,0x34,0x30,0x31,0x31,0x38,0x30,0x31,0x3A,0x20,0x73,0x66,
    0x78,0x63,0x6E,0x76,0x5F,0x45,0x78,0x65,0x63,0x43,0x6F,0x70,
    0x79,0x41,0x6C,0x70,0x68,0x61,0x42,0x79,0x43,0x62,0x46,0x75,
    0x6E,0x63,0x20,0x3A,0x20,0x66,0x72,0x61,0x6D,0x65,0x20,0x66,
    0x6F,0x72,0x6D,0x61,0x74,0x20,0x69,0x73,0x20,0x69,0x6E,0x76,
    0x61,0x6C,0x69,0x64,0x2E,0x00,0x45,0x30,0x35,0x30,0x37,0x30,
    0x38,0x30,0x39,0x3A,0x20,0x70,0x6C,0x65,0x61,0x73,0x65,0x20,
    0x73,0x65,0x74,0x20,0x70,0x69,0x74,0x63,0x68,0x20,0x62,0x79,
    0x20,0x6D,0x77,0x50,0x6C,0x79,0x46,0x78,0x53,0x65,0x74,0x4F,
    0x75,0x74,0x42,0x75,0x66,0x50,0x69,0x74,0x63,0x68,0x48,0x65,
    0x69,0x67,0x68,0x74,0x28,0x29,0x2E,0x00,
};
