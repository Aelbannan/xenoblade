// Auto-scaffolded catalog TU for CriWare/src/sofdec/mwply/mwsfdfrm
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void mwPlyGetCurFrm() {}

void mwl_convFrmInfFromSFD() {}

void mwsffrm_SetPicUsrInf() {}

void mwsffrm_SetSudDatInf() {}

void mwPlyCalcYccPlane() {}

void mwPlyRelCurFrm() {}

void mwPlyGetTotalFrmNum() {}

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
