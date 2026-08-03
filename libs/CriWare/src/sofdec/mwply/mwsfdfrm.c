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
    if (MWSFD_GetUsePicUsr(a) != 1) {
        *(s32*)((u8*)out + 0x4c) = 0;
        *(s32*)((u8*)out + 0x50) = 0;
        return;
    }
    if (*(s32*)((u8*)a + 0x4bc) == 0) {
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

void mwsffrm_SetSudDatInf() {}

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

void criware_8039CD7C() {}
