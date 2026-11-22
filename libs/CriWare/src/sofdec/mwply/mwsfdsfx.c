// Auto-scaffolded catalog TU for CriWare/src/sofdec/mwply/mwsfdsfx
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

#include "libs/CriWare/src/sofdec/mwply/mwsfd_types.h"
#include "libs/CriWare/src/sofdec/sfx/sfx_types.h"
#include "libs/CriWare/src/sofdec/sfdcore/sfd/sfd_types.h"




extern void SFX_Init(void);
extern void SFX_SetErrFn(u32 a, u32 b);
void mwsfsfx_SfxErrCbFn(void* context, int code);
void mwsfsfx_SetYcc420plnInfToSfx(MWSFDPLY* a, MwlSfdFrmInf* src, SFXStmInf* sfx);
extern int MWSFD_IsEnableHndl(void*);

void MWSFSFX_Init(void) {
    SFX_Init();
    SFX_SetErrFn((u32)mwsfsfx_SfxErrCbFn, 0);
}

extern void MWSFSVM_Error(const char*, ...);
void mwsfsfx_SfxErrCbFn(void* context, int code) { MWSFSVM_Error((const char*)code); }

int MWSFSFX_CalcHnWorkSiz(void) { return 0x301f; }

void MWSFSFX_Create(void) { SFX_Create(); }

void MWSFSFX_Destroy(void) { SFX_Destroy(); }

void MWSFD_CnvFrmInfToSfx(MWSFDPLY* self, MwlSfdFrmInf* frm, SFXStmInf* sfx) {
    s32 mode;
    s32 v;

    switch (frm->field_0x08) {
    case 1:
        mode = 1;
        break;
    case 2:
        mode = 2;
        break;
    case 3:
        mode = 3;
        break;
    default:
        MWSFSVM_Error("E201184 MwsfdBufFmt value is invalid.");
        mode = 3;
        break;
    }
    frm->field_0x00 = mode;
    mwsfsfx_SetYcc420plnInfToSfx(self, frm, sfx);

    switch (frm->field_0x0c) {
    case 1:
    case 2:
    case 3:
        v = frm->field_0x0c;
        break;
    default:
        MWSFSVM_Error("E301272 picture_structure is invalid.");
        v = 3;
        break;
    }
    frm->field_0x04 = v;

    switch (frm->field_0x10) {
    case 1:
    case 2:
    case 3:
        v = frm->field_0x10;
        break;
    default:
        MWSFSVM_Error("E301273 chroma_format is invalid.");
        v = 3;
        break;
    }
    frm->field_0x08 = v;

    v = frm->field_0x14;
    if (v != 0 && v != 1) {
        MWSFSVM_Error("E301274 chromapos is invalid.");
        v = 0;
    }
    frm->field_0x14 = v;

    v = frm->field_0x18;
    if (v != 0 && v != 1) {
        MWSFSVM_Error("E301274 chromapos is invalid.");
        v = 0;
    }
    frm->field_0x18 = v;
    frm->field_0x1c = mode;
}

// name-guess: pool text "E2011911 mwPlyFxCnvFrmZ16: ..." names this API.
void mwPlyFxCnvFrmZ16(MWSFDPLY* self, MwlSfdFrmInf* frm) {
    if (MWSFD_IsEnableHndl(self) != 1) {
        MWSFSVM_Error("E2011911 mwPlyFxCnvFrmZ16: handle is invalid.");
        return;
    }
    if (frm == NULL) {
        MWSFSVM_Error("E2011912 mwPlyFxCnvFrmZ16: getfrm is failed.");
        return;
    }
    MWSFD_CnvFrmInfToSfx(self, frm, self->sfx);
}

// name-guess: pool text "E2011913 mwPlyFxCnvFrmZ32: ..." names this API.
void mwPlyFxCnvFrmZ32(MWSFDPLY* self, MwlSfdFrmInf* frm) {
    if (MWSFD_IsEnableHndl(self) != 1) {
        MWSFSVM_Error("E2011913 mwPlyFxCnvFrmZ32: handle is invalid.");
        return;
    }
    if (frm == NULL) {
        MWSFSVM_Error("E2011914 mwPlyFxCnvFrmZ32: getfrm is failed.");
        return;
    }
    MWSFD_CnvFrmInfToSfx(self, frm, self->sfx);
}

// name-guess: pool text "E201214 mwPlyFxSetCompoMode: ..." names this API.
void mwPlyFxSetCompoMode(MWSFDPLY* self, s32 mode) {
    if (MWSFD_IsEnableHndl(self) != 1) {
        MWSFSVM_Error("E201214 mwPlyFxSetCompoMode: handle is invalid.");
        return;
    }
    if (mode == 2) {
        MWSFSVM_Error("E204011 mwPlyFxSetCompoMode: COMPO_Z needs setting in MWPLY Creation.");
    }
    if (mode == 3) {
        MWSFSVM_Error("E204012 mwPlyFxSetCompoMode: COMPO_AUTO needs setting in MWPLY Creation.");
    }
    self->field_0x68 = mode;
}

int MWSFD_IsColAdjFrame(MWSFDPLY* self, MwlSfdFrmInf* frm) {
    MwlSfdFrame* entry = &self->frmFx[frm->field_0x38 % 8];
    int v;
    if (entry->state != 1)
        v = 0;
    else
        v = entry->colAdj;
    int r = v == 1;
    if (frm->field_0xa0)
        r = SUD_AnalyTypeCcs(frm->field_0xa0, frm->field_0xa4) == 1;
    return r;
}

int MWSFD_IsColAdjFile(MWSFDPLY* self, int idx) {
    MwlSfdFrame* entry = &self->frmFx[idx % 8];
    if (entry->state != 1) return 0;
    return entry->colAdjFile;
}

int MWSFD_GetFxType(MWSFDPLY* self, int idx) {
    MwlSfdFrame* entry = &self->frmFx[idx % 8];
    if (entry->state != 1) return 0x11;
    return entry->fxType;
}

extern void mwPlyCalcYccPlane(s32 a, s32 b, s32 c, s32* out);

void mwsfsfx_SetYcc420plnInfToSfx(MWSFDPLY* a, MwlSfdFrmInf* src, SFXStmInf* sfx) {
    s32 h = src->field_0x0c;
    s32 v = src->field_0x10;
    sfx->bytesPerLine = v;
    sfx->width = h;
    if (src->field_0x08 != 3) {
        sfx->_04 = src->field_0x00;
        sfx->_08 = h;
        sfx->_0C = v;
    } else {
        s32 plane[6];
        s32 w3;
        s32 w0;
        s32 t1;
        s32 t4;
        s32 t5;
        s32 t2;
        mwPlyCalcYccPlane(src->field_0x00, h, v, plane);
        w3 = plane[3];
        w0 = plane[0];
        sfx->_04 = w0;
        w0 = v / 2;
        sfx->_08 = w3;
        sfx->_0C = v;
        t1 = plane[1];
        t4 = plane[4];
        sfx->_18 = t4;
        sfx->_14 = t1;
        sfx->_1C = w0;
        t5 = plane[5];
        t2 = plane[2];
        sfx->_24 = t2;
        sfx->_28 = t5;
        sfx->_2C = w0;
    }
}

void SFX_SetCompoMode(void* a);
void MWSFSFX_SetCompoMode(MWSFDPLY* self) {
    SFX_SetCompoMode(self->sfx);
}

extern int MWSFD_IsEnableHndl(void*);
extern void MWSFSVM_Error(const char*, ...);
extern int SUD_AnalyTypeCcs(const char *, s32);
extern void SFX_SetOutBufSize(void *, u32, u32);
extern void SFX_SetUnitWidth(void *, u32);

u32 mwPlyFxGetCompoMode(MWSFDPLY* self) {
    if (MWSFD_IsEnableHndl(self) != 1) {
        MWSFSVM_Error("E2011915 mwPlyFxGetCompoMode: handle is invalid.");
        return 0;
    }
    return self->field_0x68;
}

void mwPlyFxSetOutBufPitchHeight(MWSFDPLY *self, u32 width, u32 height) {
    if (MWSFD_IsEnableHndl(self) != 1) {
        MWSFSVM_Error("E306091 MWSFSFX_SetOutBufSize: handle is invalid.");
        return;
    }
    void *sfx = self->sfx;
    SFX_SetOutBufSize(sfx, width, height);
    SFX_SetUnitWidth(sfx, 0);
}

// name-guess: the pool text "E307092 mwPlyFxGetOutBufPitchHeight: ..." names this API.
void mwPlyFxGetOutBufPitchHeight(MWSFDPLY* self, u32* width, u32* height) {
    if (MWSFD_IsEnableHndl(self) != 1) {
        MWSFSVM_Error("E307092 mwPlyFxGetOutBufPitchHeight: handle is invalid.");
        return;
    }
    *width = self->prm.width;
    *height = self->prm.height;
}

// name-guess: the pool text "E2011918 mwPlyFxSetOutZscale: ..." names this API.
void mwPlyFxSetOutZscale(MWSFDPLY* self, u32 scale) {
    if (MWSFD_IsEnableHndl(self) != 1) {
        MWSFSVM_Error("E2011918 mwPlyFxSetOutZscale: handle is invalid.");
        return;
    }
    self->field_0x6c = scale;
}

// name-guess: the pool text "E2011919 mwPlyFxGetOutZscale: ..." names this API.
u32 mwPlyFxGetOutZscale(MWSFDPLY* self) {
    if (MWSFD_IsEnableHndl(self) != 1) {
        MWSFSVM_Error("E2011919 mwPlyFxGetOutZscale: handle is invalid.");
        return 0;
    }
    return self->field_0x6c;
}

// name-guess: the pool text "E2011920 mwPlyFxSetOutZoffset: ..." names this API.
void mwPlyFxSetOutZoffset(MWSFDPLY* self, u32 offset) {
    if (MWSFD_IsEnableHndl(self) != 1) {
        MWSFSVM_Error("E2011920 mwPlyFxSetOutZoffset: handle is invalid.");
        return;
    }
    self->field_0x70 = offset;
}

// name-guess: the pool text "E2011921 mwPlyFxGetOutZoffset: ..." names this API.
u32 mwPlyFxGetOutZoffset(MWSFDPLY* self) {
    if (MWSFD_IsEnableHndl(self) != 1) {
        MWSFSVM_Error("E2011921 mwPlyFxGetOutZoffset: handle is invalid.");
        return 0;
    }
    return self->field_0x70;
}

int MWSFTAG_IsUseAinfSj(MWSFDPLY* self) {
    int v = (s32)self->prm.field_0x1c;
    if (v == 0 || v == 0x101) {
        return 1;
    }
    return 0;
}

extern void* SJRBF_Create(void* pool_mem, u32 buf_size, u32 xtr_size);
extern void criware_803A09B4(void* self);

s32 MWSFTAG_CreateAinfSj(MWSFDPLY* self) {
    s32 ok;
    s32 f = self->prm.mode;
    if (f == 0 || f == 0x101) {
        ok = 1;
    } else {
        ok = 0;
    }
    if (ok != 1)
        return 0;
    {
        s32 sj = (s32)SJRBF_Create(self->ainfBuf, self->ainfBufSize, 0);
        if (sj == 0) {
            MWSFSVM_Error("E201211 mwPlyCreate: can't create AddInfSJ");
            criware_803A09B4(self);
            return 0;
        }
        return sj;
    }
}

void MWSFTAG_DestroyAinfSj(MWSFDPLY* self) {
    void* p = self->ainfSj;
    if (p == NULL) return;
    SfdSjObj* sj = p;
    sj->vtbl->destroy(sj);
}

extern s32 SFD_SetUsrSj(void* sst, s32 a, s32 b, s32 c);

s32 MWSFTAG_SetAinfSj(MWSFDPLY* self) {
    s32 ok;
    if ((s32)self->prm.field_0x04 == 2 ||
        (s32)self->prm.field_0x04 == 6 ||
        (s32)self->prm.field_0x04 == 8 ||
        (s32)self->prm.field_0x04 == 0xa) {
        ok = 1;
    } else {
        ok = 0;
    }
    if (ok == 1)
        return 0;
    if (self->ainfSj == NULL)
        return 0;
    {
        s32 r = SFD_SetUsrSj(self->sfd, 2, (s32)self->ainfSj, 0);
        return (-r | r) >> 31;
    }
}

void MWSFTAG_InitTagInf(MWSFDPLY* self) {
    self->tagValid = 0;
    self->tagData = 0;
    self->tagSize = 0;
    self->tagState = -1;
}

void MWSFTAG_ResetAinfSj(MWSFDPLY* self) {
    void* p = self->ainfSj;
    if (p == NULL) return;
    SfdSjObj* sj = p;
    sj->vtbl->reset(sj);
}

extern void mwsftag_GetAinfFromSj(MWSFDPLY* self);
extern void SFX_SetTagInf(void* tag, u32 a, u32 b);
extern s32 SJ_SearchTag(u32* a, char* b, char* c, u32* out);

void MWSFTAG_UpdateTagInf(MWSFDPLY* self) {
    if (self->ainfSj == NULL)
        return;
    mwsftag_GetAinfFromSj(self);
    {
        void* tag = self->sfx;
        u32 out[2];
        u32 buf[2];
        u32 a = self->tagData;
        if (a == 0) {
            SFX_SetTagInf(tag, 0, 0);
            return;
        }
        buf[0] = a;
        buf[1] = self->tagSize;
        if (SJ_SearchTag(buf, "SFXINFS", "SFXINFE", out) == 0) {
            SFX_SetTagInf(tag, 0, 0);
        } else {
            SFX_SetTagInf(tag, out[0], out[1]);
        }
    }
}

void mwsftag_GetAinfFromSj(MWSFDPLY* self) {
    void* sj = self->ainfSj;
    SfdSjObj* sjObj = sj;
    u32 buf[2];
    u32 out[2];
    s32 ret;
    buf[0] = (u32)self->ainfBuf;
    ret = sjObj->vtbl->getAvail(sjObj, 1);
    if (ret == 0) {
        self->tagData = 0;
        self->tagSize = 0;
        self->tagValid = 1;
        return;
    }
    buf[1] = (u32)ret;
    if (SJ_SearchTag(buf, "CRITAGS", "CRITAGE", out) == 0) {
        self->tagData = 0;
        self->tagSize = 0;
        self->tagValid = 1;
        return;
    }
    if (self->tagBuf != NULL) {
        s32 t;
        memcpy(self->tagBuf, (void*)out[0], out[1]);
        self->tagData = (u32)self->tagBuf;
        self->tagSize = 0x7fffffff;
        self->tagValid = 1;
        ((void (*)(void*, s32, s32*))*(void**)((u8*)*(void**)((u8*)sj) + 0x18))(sj, 1, &t);
        sjObj->vtbl->putChunk(sjObj, 0, &t);
        sjObj->vtbl->reset(sjObj);
    } else {
        self->tagData = out[0];
        self->tagSize = out[1];
        self->tagValid = 1;
        if (!((s32)self->prm.field_0x04 == 2 || (s32)self->prm.field_0x04 == 6 ||
              (s32)self->prm.field_0x04 == 8 || (s32)self->prm.field_0x04 == 0xa)) {
            if (self->ainfSj != NULL) {
                SFD_SetUsrSj(self->sfd, 2, 0, 0);
            }
        }
    }
}


// name-guess: the pool text "W2121001 mwPlyAttachAddInfBuf(): ..." names this API.
void mwPlyAttachAddInfBuf(MWSFDPLY* self, void* buf, u32 size) {
    if (size < self->ainfBufSize) {
        MWSFSVM_Error("W2121001 mwPlyAttachAddInfBuf(): bufsize is short.");
        return;
    }
    self->ainfBuf = buf;
    self->ainfBufSize = size;
}

// name-guess: the pool texts "E202283/E202284 MWSFD_MakeTblZ16: ..." name this API.
void MWSFD_MakeTblZ16(MWSFDPLY* self, MwlSfdFrmInf* frm) {
    if (MWSFD_IsEnableHndl(self) != 1) {
        MWSFSVM_Error("E202283 MWSFD_MakeTblZ16: handle is invalid.");
        return;
    }
    if (frm == NULL) {
        MWSFSVM_Error("E202284 MWSFD_MakeTblZ16: getfrm is failed.");
        return;
    }
    MWSFD_CnvFrmInfToSfx(self, frm, self->sfx);
}

// name-guess: the pool texts "E202285/E202286 MWSFD_MakeTblZ32: ..." name this API.
void MWSFD_MakeTblZ32(MWSFDPLY* self, MwlSfdFrmInf* frm) {
    if (MWSFD_IsEnableHndl(self) != 1) {
        MWSFSVM_Error("E202285 MWSFD_MakeTblZ32: handle is invalid.");
        return;
    }
    if (frm == NULL) {
        MWSFSVM_Error("E202286 MWSFD_MakeTblZ32: getfrm is failed.");
        return;
    }
    MWSFD_CnvFrmInfToSfx(self, frm, self->sfx);
}

// name-guess: the pool text "E404010 mwPlySetCnvBottomUp: ..." names this API.
void mwPlySetCnvBottomUp(MWSFDPLY* self, u32 enable) {
    if (MWSFD_IsEnableHndl(self) != 1) {
        MWSFSVM_Error("E404010 mwPlySetCnvBottomUp: handle is invalid.");
        return;
    }
    self->field_0x74 = enable;
}

// name-guess: the pool text "E404011 mwPlyGetCnvBottomUp: ..." names this API.
u32 mwPlyGetCnvBottomUp(MWSFDPLY* self) {
    if (MWSFD_IsEnableHndl(self) != 1) {
        MWSFSVM_Error("E404011 mwPlyGetCnvBottomUp: handle is invalid.");
        return 0;
    }
    return self->field_0x74;
}

void MWSFD_GetZfrmRange() {}
