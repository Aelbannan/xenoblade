// Auto-scaffolded catalog TU for CriWare/src/sofdec/mwply/mwsfdsfx
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern void SFX_Init(void);
extern void SFX_SetErrFn(u32 a, u32 b);
void mwsfsfx_SfxErrCbFn(void* context, int code);

void MWSFSFX_Init(void) {
    SFX_Init();
    SFX_SetErrFn((u32)mwsfsfx_SfxErrCbFn, 0);
}

extern void MWSFSVM_Error(const char*, ...);
void mwsfsfx_SfxErrCbFn(void* context, int code) { MWSFSVM_Error((const char*)code); }

int MWSFSFX_CalcHnWorkSiz(void) { return 0x301f; }

void MWSFSFX_Create(void) { SFX_Create(); }

void MWSFSFX_Destroy(void) { SFX_Destroy(); }

void MWSFD_CnvFrmInfToSfx() {}

int MWSFD_IsColAdjFrame(void *a, void *b) {
    int idx = *(s32 *)((u8 *)b + 0x38);
    u8 *entry = (u8 *)a + (idx % 8) * 0x38;
    int v;
    if (*(s32 *)(entry + 0xE0) != 1)
        v = 0;
    else
        v = *(u32 *)(entry + 0xF8);
    int r = v == 1;
    if (*(u32 *)((u8 *)b + 0xA0))
        r = SUD_AnalyTypeCcs(*(u32 *)((u8 *)b + 0xA0), *(u32 *)((u8 *)b + 0xA4)) == 1;
    return r;
}

int MWSFD_IsColAdjFile(void* self, int idx) {
    int index = idx % 8;
    u32* entry = (u32*)((u8*)self + index * 0x38);
    if ((s32)entry[0xE0 / 4] != 1) return 0;
    return entry[0xFC / 4];
}

int MWSFD_GetFxType(void* self, int idx) {
    int index = idx % 8;
    u32* entry = (u32*)((u8*)self + index * 0x38);
    if ((s32)entry[0xE0 / 4] != 1) return 0x11;
    return entry[0x104 / 4];
}

void mwsfsfx_SetYcc420plnInfToSfx() {}

void SFX_SetCompoMode(void* a);
void MWSFSFX_SetCompoMode(void* self) {
    SFX_SetCompoMode(*(void**)((u8*)self + 0xC8));
}

extern int MWSFD_IsEnableHndl(void*);
extern void MWSFSVM_Error(const char*, ...);
extern char lbl_eu_80519EC8[];
extern int SUD_AnalyTypeCcs(const char *, s32);
extern void SFX_SetOutBufSize(void *, u32, u32);
extern void SFX_SetUnitWidth(void *, u32);

u32 mwPlyFxGetCompoMode(void* self) {
    if (MWSFD_IsEnableHndl(self) != 1) {
        MWSFSVM_Error(lbl_eu_80519EC8 + 0x201);
        return 0;
    }
    return *(u32*)((u8*)self + 0x68);
}

void mwPlyFxSetOutBufPitchHeight(void *self, u32 width, u32 height) {
    if (MWSFD_IsEnableHndl(self) != 1) {
        MWSFSVM_Error(lbl_eu_80519EC8 + 0x232);
        return;
    }
    void *sfx = *(void **)((u8 *)self + 0xC8);
    SFX_SetOutBufSize(sfx, width, height);
    SFX_SetUnitWidth(sfx, 0);
}

int MWSFTAG_IsUseAinfSj(void* self) {
    int v = *(s32*)((u8*)self + 0x20);
    if (v == 0 || v == 0x101) {
        return 1;
    }
    return 0;
}

extern void* SJRBF_Create(void* a, void* b, s32 c);
extern void criware_803A09B4(void* self);
extern char lbl_eu_80519EC8[];

s32 MWSFTAG_CreateAinfSj(void* self) {
    s32 ok;
    s32 f = *(s32*)((u8*)self + 0x28);
    if (f == 0 || f == 0x101) {
        ok = 1;
    } else {
        ok = 0;
    }
    if (ok != 1)
        return 0;
    {
        s32 r = (s32)SJRBF_Create(*(void**)((u8*)self + 0x4c4), *(void**)((u8*)self + 0x4c8), 0);
        if (r != 0)
            MWSFSVM_Error(lbl_eu_80519EC8 + 0x362);
        criware_803A09B4(self);
    }
    return 0;
}

void MWSFTAG_DestroyAinfSj(void* self) {
    void* p = *(void**)((u8*)self + 0x4c0);
    if (p == NULL) return;
    void** vtable = *(void***)p;
    ((void (*)(void*))vtable[3])(p);
}

extern s32 SFD_SetUsrSj(void* sst, s32 a, s32 b, s32 c);

s32 MWSFTAG_SetAinfSj(void* self) {
    s32 ok;
    if (*(s32*)((u8*)self + 8) == 2 ||
        *(s32*)((u8*)self + 8) == 6 ||
        *(s32*)((u8*)self + 8) == 8 ||
        *(s32*)((u8*)self + 8) == 0xa) {
        ok = 1;
    } else {
        ok = 0;
    }
    if (ok == 1)
        return 0;
    if (*(s32*)((u8*)self + 0x4c0) == 0)
        return 0;
    {
        s32 r = SFD_SetUsrSj(*(void**)((u8*)self + 0x58), 2, *(s32*)((u8*)self + 0x4c0), 0);
        return (-r | r) >> 31;
    }
}

void MWSFTAG_InitTagInf(void* self) {
    *(u32*)((u8*)self + 0x4d8) = 0;
    *(u32*)((u8*)self + 0x4dc) = 0;
    *(u32*)((u8*)self + 0x4e0) = 0;
    *(u32*)((u8*)self + 0x4d4) = -1;
}

void MWSFTAG_ResetAinfSj(void* self) {
    void* p = *(void**)((u8*)self + 0x4c0);
    if (p == NULL) return;
    void** vtable = *(void***)p;
    ((void (*)(void*))vtable[5])(p);
}

extern void mwsftag_GetAinfFromSj(void* self);
extern void SFX_SetTagInf(void* tag, u32 a, u32 b);
extern s32 SJ_SearchTag(u32* a, char* b, char* c, u32* out);

void MWSFTAG_UpdateTagInf(void* self) {
    if (*(u32*)((u8*)self + 0x4c0) == 0)
        return;
    mwsftag_GetAinfFromSj(self);
    {
        void* tag = *(void**)((u8*)self + 0xc8);
        u32 a = *(u32*)((u8*)self + 0x4dc);
        u32 b = *(u32*)((u8*)self + 0x4e0);
        u32 out[2];
        if (a == 0) {
            SFX_SetTagInf(tag, 0, 0);
            return;
        }
        if (SJ_SearchTag(&a, lbl_eu_80519EC8 + 0x38d, lbl_eu_80519EC8 + 0x395, out) != 0) {
            SFX_SetTagInf(tag, out[0], out[1]);
        } else {
            SFX_SetTagInf(tag, 0, 0);
        }
    }
}


void MWSFD_GetZfrmRange() {}
