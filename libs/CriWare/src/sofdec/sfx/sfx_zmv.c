// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfx/sfx_zmv
#include <harness_catalog.h>
#include "libs/CriWare/src/sofdec/sfx/sfx_types.h"

void SFXZ_Init() {}
void SFXZ_Create() {}
void SFXZ_Destroy(void* self) { if (self != NULL) ((SFXZmvState*)self)->active = 0; }
void sfxzmv_SetTagGrp();
void SFXZ_SetTagInf(void* self, u32 a, u32 b) {
    SFXZmvState* state = (SFXZmvState*)self;
    state->active = 1;
    state->arg0 = a;
    state->arg1 = b;
    sfxzmv_SetTagGrp();
}
void sfxzmv_SetTagGrp() {}
void SFXZ_GetZfrmRange() {}
void sfxzmv_MakeOrgZ32TblByDirect() {}
void sfxzmv_MakeOrgZ32TblByCCIR() {}
void SFXZ_MakeCnvZTbl() {}
void sfxzmv_MakeZ16TblFromOrgZ32() {}
void sfxzmv_MakeZ32TblFromOrgZ32() {}
