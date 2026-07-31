// Auto-scaffolded catalog TU for CriWare/src/sofdec/mwply/mwsfdfrm
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void mwPlyGetCurFrm() {}

void mwl_convFrmInfFromSFD() {}

void mwsffrm_SetPicUsrInf() {}

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

void mwsffrm_AnalyAudioInfo() {}

void mwsffrm_ChangeSettingSyncPlayback() {}

void criware_8039CD7C() {}
