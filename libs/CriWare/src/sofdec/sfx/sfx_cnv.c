// CriWare/src/sofdec/sfx/sfx_cnv
#include <harness_catalog.h>
#include "libs/CriWare/src/sofdec/sfx/sfx_types.h"

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
void SFXLIB_Error(void* self, void* stmInf, const char* msg);
void SFXINF_GetStmInf(void* stmInf, const char* name);
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

    if (splitField == -1) {
        u32 v0 = stmInf->_88;
        mergeField = FALSE;
        if (v0 == 0) {
            return mergeField;
        }
        if (SUD_AnalyTypeDivField(v0, stmInf->_8C) != 1) {
            return mergeField;
        }
        if (SFX_GetProgOut(self) != 0) {
            return mergeField;
        }
        mergeField = TRUE;
    } else if (splitField == 0) {
        mergeField = FALSE;
    } else if (splitField == 1) {
        mergeField = TRUE;
    } else {
        mergeField = FALSE;
    }

    return mergeField;
}

void sfxcnv_MakeTable(SFXConvertState* self, SFXStmInf* stmInf, u32 tableType) {
    BOOL needUpdate = TRUE;

    if (self->compoMode == 0x64) {
        needUpdate = FALSE;
    } else if (self->compoMode != tableType) {
        /* New mode, need to update */
    } else {
        u32 tmp = tableType - 4;
        if (tmp <= 1) {
            needUpdate = FALSE;
        } else if (tableType == 2) {
            if (SFXA_IsNeedUpdateLumiTbl(self->alphaState) != 1) {
                needUpdate = FALSE;
            }
        } else if (tableType == 1 || tableType == 0x15) {
            needUpdate = FALSE;
        }
    }

    if (needUpdate != TRUE) {
        return;
    }

    self->compoMode = tableType;

    switch (tableType) {
    case 0x0B:
    case 0x0D:
        SFXZ_MakeCnvZTbl((void*)self->_24, stmInf->_4C, self->_38);
        break;
    case 0x02:
        SFXA_MakeAlpLumiTbl(self->alphaState, stmInf->_4C, self->_38);
        break;
    case 0x04:
        SFXA_MakeAlp3110Tbl(self->alphaState, stmInf->_4C, self->_38);
        break;
    case 0x05:
        SFXA_MakeAlp3211Tbl(self->alphaState, stmInf->_4C, self->_38);
        break;
    case 0x15: {
        void (*cb)(u32) = (void (*)(u32))self->makeColorAdjustCallback;
        if (cb != NULL) {
            cb(self->_38);
        }
        break;
    }
    case 0x01:
        SFXCNV_MakeCcirFromY((u8*)self->_38);
        break;
    default:
        SFXLIB_Error(self, stmInf, "sfxcnv_MakeTable: unknown table type");
        break;
    }
}

void sfxcnv_ExecCnvFrmByCbFunc(SFXConvertState* self, SFXStmInf* stmInf,
                                SFXDstBufInf* dstBuf, BOOL useTable);
void sfxcnv_ExecCopyAlphaByCbFunc(SFXConvertState* self, SFXStmInf* stmInf,
                                   SFXDstBufInf* dstBuf);

void SFX_CnvFrmByCbFunc(SFXConvertState* self, SFXStmInf* stmInf,
                         SFXDstBufInf* dstBuf) {
    u32 fmt;

    if (self->formatType == 0) {
        self->formatType = (u32)SFXINF_GetStmInf(stmInf, "SFX_CnvFrmByCbFunc");
    }

    fmt = self->formatType;

    switch (fmt) {
    case 0x11:
        if (stmInf->_90 == 1) {
            sfxcnv_MakeTable(self, stmInf, 0x15);
            sfxcnv_ExecCnvFrmByCbFunc(self, stmInf, dstBuf, TRUE);
        } else {
            sfxcnv_ExecCnvFrmByCbFunc(self, stmInf, dstBuf, FALSE);
        }
        break;
    case 0x1001:
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
        u32 tableType;
        if (fmt == 0x51) {
            tableType = 4;
        } else if (fmt == 0x61) {
            tableType = 5;
        } else {
            tableType = (stmInf->_94 == 0x51) ? 4 : 5;
        }
        sfxcnv_MakeTable(self, stmInf, tableType);
        sfxcnv_ExecCnvFrmByCbFunc(self, stmInf, dstBuf, TRUE);
        break;
    }
    case 0x101:
        sfxcnv_ExecCnvFrmByCbFunc(self, stmInf, dstBuf, FALSE);
        break;
    default:
        SFXLIB_Error(self, stmInf, "SFX_CnvFrmByCbFunc: unknown format");
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
        SFXLIB_Error(self, stmInf, "sfxcnv_ExecCnvFrmByCbFunc: no buffer");
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

    switch (stmInf->srcType) {
    case 1:
        srcBuf->type = 1;
        srcBuf->_04 = stmInf->_04;
        srcBuf->_08 = stmInf->width;
        srcBuf->_0C = stmInf->_0C;
        srcBuf->_10 = stmInf->_08;
        break;
    case 2:
        srcBuf->type = 1;
        srcBuf->_04 = stmInf->_04;
        srcBuf->_08 = stmInf->width;
        srcBuf->_0C = stmInf->_0C;
        srcBuf->_10 = stmInf->_08;
        break;
    case 3: {
        s32 halfW = (s32)stmInf->width / 2;
        srcBuf->type = 3;
        srcBuf->_04 = stmInf->_04;
        srcBuf->_08 = stmInf->width;
        srcBuf->_0C = stmInf->_0C;
        srcBuf->_10 = stmInf->_08;
        srcBuf->_14 = stmInf->_14;
        srcBuf->_18 = (u32)halfW;
        srcBuf->_1C = stmInf->_1C;
        srcBuf->_20 = stmInf->_18;
        srcBuf->_24 = stmInf->_24;
        srcBuf->_28 = (u32)halfW;
        srcBuf->_2C = stmInf->_2C;
        srcBuf->_30 = stmInf->_28;
        break;
    }
    default:
        SFXLIB_Error(self, stmInf, "sfxcnv_MakeCftSrcBuf: unknown source type");
        break;
    }
}

void sfxcnv_ExecCopyAlphaByCbFunc(SFXConvertState* self, SFXStmInf* stmInf,
                                   SFXDstBufInf* dstBuf) {
    SFXCnvSrcBuf srcBuf;
    u32 dstInfo[4];
    u32 table;
    void (*cb)(SFXCnvSrcBuf*, u32*, u32*);

    sfxcnv_MakeCftSrcBuf(self, stmInf, &srcBuf);

    if (stmInf->srcType == 1 || stmInf->srcType == 2) {
        /* skip plane adjustment */
    } else if (stmInf->srcType == 3) {
        u32 w = srcBuf._08;
        u32 h = srcBuf._0C;
        u32 halfPlane = (u32)((s32)(w * h) / 2);
        u32 quarterPlane = (u32)((s32)halfPlane / 2);

        srcBuf._04 += halfPlane;
        srcBuf._14 += quarterPlane;
        srcBuf._24 += quarterPlane;

        dstInfo[0] = dstBuf->_04;
        dstInfo[1] = dstBuf->_08;
        dstInfo[2] = dstBuf->_0C;

        if (self->_08 == 0) {
            dstInfo[3] = srcBuf._08 << 2;
            SFXLIB_Error(self, stmInf, "sfxcnv_ExecCopyAlphaByCbFunc: no buffer");
        } else if (self->_10 == 0 && self->bytesPerPixelOut != 0) {
            dstInfo[3] = self->_08 / self->bytesPerPixelOut;
        } else {
            dstInfo[3] = self->_08;
        }
    } else {
        SFXLIB_Error(self, stmInf, "sfxcnv_ExecCopyAlphaByCbFunc: unknown type");
    }

    if (SFX_SetCcirFx() == 1) {
        table = self->_38;
    } else {
        table = 0;
    }

    cb = (void (*)(SFXCnvSrcBuf*, u32*, u32*))self->copyAlphaCallback;
    if (cb != NULL) {
        cb(&srcBuf, dstInfo, &table);
    }
}

void SFX_SetBytePerPixelOutBuf(void* self, u32 val) {
    ((SFXConvertState*)self)->bytesPerPixelOut = val;
}

void sfxcnv_MakeDstBufInf(SFXConvertState* self, SFXStmInf* stmInf,
                          void* bufPtr, SFXDstBufInf* dstArray, u32 idx) {
    SFXDstBufInf* entry = &dstArray[idx];
    int bpp;

    entry->_04 = (u32)(uintptr_t)bufPtr;
    entry->_08 = stmInf->width;

    switch (self->formatType) {
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
        SFXLIB_Error(NULL, NULL, "sfxcnv_MakeDstBufInf: unknown format");
        bpp = 0;
        break;
    }

    if (bpp == 1) {
        entry->_0C = (u32)((s32)stmInf->bytesPerLine / 2);
    } else {
        entry->_0C = stmInf->bytesPerLine;
    }

    if (self->_08 == 0) {
        u32 height = stmInf->width;
        SFXLIB_Error(self, stmInf, "sfxcnv_MakeDstBufInf: no buffer");
        entry->_10 = height;
    } else if (self->_10 == 0 && self->bytesPerPixelOut != 0) {
        entry->_10 = self->_08 / self->bytesPerPixelOut;
    } else {
        entry->_10 = self->_08;
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
