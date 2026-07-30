// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfx/sfx_zmv
#include <harness_catalog.h>
#include "libs/CriWare/src/sofdec/sfx/sfx_types.h"

void* memset(void* s, int c, size_t n);

extern u8 lbl_eu_8061A260[];
extern float lbl_eu_8051D218;

void SFXZ_Init(void) {
    memset(lbl_eu_8061A260, 0, 0x26c);
    *(u32*)(lbl_eu_8061A260 + 8) = 8;
    *(u32*)(lbl_eu_8061A260 + 4) = 0;
}

void* SFXZ_Create(void) {
    u8* p = lbl_eu_8061A260;
    s32 cnt = *(s32*)(p + 8);
    p += 12;
    if (cnt <= 0) goto L_null;
L_loop:
    if (*(u32*)p != 0) goto L_next;
    goto L_found;
L_next:
    p += 0x4C;
    if (--cnt != 0) goto L_loop;
L_null:
    p = NULL;
L_found:
    if (p == NULL) return NULL;
    {
        float f0 = lbl_eu_8051D218;
        *(float*)(p + 0x3C) = f0;
        *(float*)(p + 0x40) = f0;
    }
    *(u32*)(p + 0x44) = 0;
    *(u32*)(p + 0x48) = 0;
    *(u32*)(p + 0x04) = 0;
    (*(u32*)lbl_eu_8061A260)++;
    *(u32*)p = 1;
    return p;
}

void SFXZ_Destroy(void* self) {
    if (self == NULL) return;
    *(u32*)((u8*)self) = 0;
    *(s32*)lbl_eu_8061A260 -= 1;
}
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