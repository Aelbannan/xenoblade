// Auto-scaffolded catalog TU for CriWare/src/sofdec/mwply/mwsfdfrm
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void mwPlyGetCurFrm() {}

void mwl_convFrmInfFromSFD() {}

extern s32 MWSFD_GetUsePicUsr(void* a);

void mwsffrm_SetPicUsrInf(void* a, void* b, void* out) {
    void* inf = *(void**)((u8*)b + 0x38);
    s32 v = *(s32*)((u8*)inf + 0);
    s32 w = *(s32*)((u8*)inf + 4);
    u32 picadr = (u32)((u8*)a + 0x4A4);
    u32 pic = *(u32*)((u8*)a + 0x4bc);
    if (MWSFD_GetUsePicUsr(a) != 1) {
        *(s32*)((u8*)out + 0x4c) = 0;
        *(s32*)((u8*)out + 0x50) = 0;
        return;
    }
    if (pic == picadr) {
        *(s32*)((u8*)out + 0x4c) = 0;
        *(s32*)((u8*)out + 0x50) = 0;
        return;
    }
    if (v != 0 && w > 4) {
        v += 4;
        w -= 4;
    }
    *(s32*)((u8*)out + 0x4c) = v;
    *(s32*)((u8*)out + 0x50) = w;
}

extern void* SUD_SearchSudDat(void* a, void* b, void* c, void* d);

void mwsffrm_SetSudDatInf(void* a, void* b, void* out) {
    void* sud = *(void**)((u8*)b + 0x38);
    void* p = *(void**)((u8*)sud + 0);
    void* q = *(void**)((u8*)sud + 4);
    void* r1;
    void* r2;
    *(s32*)((u8*)out + 0xa0) = (s32)p;
    *(s32*)((u8*)out + 0xa4) = (s32)q;
    if (p != NULL && (s32)(u32)q >= 4) {
        SUD_SearchSudDat((u8*)p + 4, (u8*)q - 4, &r1, &r2);
        if (r1 != NULL && (s32)(u32)r2 >= 0) {
            *(s32*)((u8*)out + 0xa0) = (s32)r1;
            *(s32*)((u8*)out + 0xa4) = (s32)r2;
        }
    }
}

typedef struct {
    u32 a, b, c;
    s32 d, e, f;
} MwSfdYccPlane;

typedef struct {
    u32 a, b, c;
    s16 d, e;
} MwSfdYccPlaneTmp;

extern void SFD_CalcYccPlane(void *, void *, void *, void *);

void mwPlyCalcYccPlane(void *a, void *b, void *c, MwSfdYccPlane *out) {
    MwSfdYccPlaneTmp tmp;
    SFD_CalcYccPlane(a, b, c, &tmp);
    out->a = tmp.c;
    out->b = tmp.a;
    out->c = tmp.b;
    out->d = tmp.e;
    out->e = tmp.d;
    out->f = tmp.d;
}

void mwPlyRelCurFrm() {}

int mwPlyGetTotalFrmNum(void* self) {
    int index = *(s32*)((u8*)self + 0xd8) % 8;
    u32* entry = (u32*)((u8*)self + index * 0x38);
    if ((s32)entry[0xE0 / 4] != 1) return 0;
    return entry[0x100 / 4];
}

void MWSFFRM_InitSfhInfTable() {}

void SFD_SetSfdHeaderFn(void* a, void* cb, void* ctx);
void mwsffrm_CallbackAnalyzeSofdecHeader();
void MWSFFRM_SetShfCbFn(void* self) {
    SFD_SetSfdHeaderFn(*(void**)((u8*)self + 0x58), mwsffrm_CallbackAnalyzeSofdecHeader, self);
}

void mwsffrm_CallbackAnalyzeSofdecHeader() {}

extern s32 SFH_AnlyElemCodecAud(const void* sfd, s32 idx, s32* out);
extern s32 SFH_AnlyElemChNum(const void* sfd, s32 idx, s32* out);
extern s32 SFH_AnlyElemSmpHz(const void* sfd, s32 idx, s32* out);
extern s32 SFH_IsExistStmId(const void* sfd, s32 idx, s32* out);
extern s32 SFH_AnlyElemPicSz(const void* sfd, s32 idx, s32* sz, s32* rate);
extern s32 SFH_AnlyFtrNetWidth(const void* sfd, s32 idx, s32* out);
extern s32 SFH_AnlyFtrNetHeight(const void* sfd, s32 idx, s32* out);

void mwsffrm_AnalyAudioInfo(void* self, void* sj) {
    s32 i;
    s32 out;
    if (*(s32*)((u8*)self + 0x2A0) != 1) return;
    for (i = 0; i < 32; i++) {
        s32* ent = (s32*)((u8*)self + i * 16 + 0x2A4);
        s32 idx = (i + 0xC0) & 0xFF;
        ent[0] = 0;
        ent[1] = 0;
        ent[2] = 0;
        ent[3] = 0;
        if (SFH_AnlyElemCodecAud(sj, idx, &out) == 1 && out == 1) {
            ent[0] = 1;
            if (SFH_AnlyElemChNum(sj, idx, &out) == 1) {
                s32 ch = out;
                if ((u32)(ch - 2) > 3) {
                    ent[1] = 2;
                } else if (ch == 1) {
                    ent[1] = 1;
                } else if (ch == 6) {
                    ent[1] = 3;
                } else if (ch == 7) {
                    ent[1] = 4;
                } else if (ch == 8) {
                    ent[1] = 5;
                } else {
                    ent[1] = 0;
                }
            }
            if (SFH_AnlyElemSmpHz(sj, idx, &out) == 1) {
                ent[2] = out;
            }
            if (SFH_IsExistStmId(sj, idx, &out) == 1) {
                ent[3] = out;
            }
        }
    }
}

void mwsffrm_ChangeSettingSyncPlayback() {}

#pragma push
#pragma opt_propagation off
void criware_8039CD7C(void* a, s32* out1, s32* out2, s32* out3, s32* out4) {
    s32 stm;
    s32 sz;
    s32 rate;
    *out1 = 0;
    *out2 = 0;
    *out3 = 0;
    *out4 = 0;
    if (SFH_IsExistStmId(a, 0xE0, &stm) == 1 && stm == 1) goto body;
    goto exit;
exit:
    return;
body:
    if (SFH_AnlyElemPicSz(a, 0xE0, &sz, &rate) != 1) goto end;
    *out1 = sz;
    *out2 = rate;
    if (SFH_AnlyFtrNetWidth(a, 0xE0, &sz) != 0) {
        *out3 = sz;
    }
    if (SFH_AnlyFtrNetHeight(a, 0xE0, &rate) != 0) {
        *out4 = rate;
    }
end:
    return;
}
#pragma pop
