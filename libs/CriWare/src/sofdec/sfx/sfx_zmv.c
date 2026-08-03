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
    u8* base = lbl_eu_8061A260;
    u8* p = base + 12;
    s32 cnt = *(s32*)(base + 8);
    s32 i;
    for (i = 0; i < cnt; i++) {
        if (*(u32*)p != 0)
            goto found;
        p += 0x4C;
    }
    p = NULL;
found:
    if (p == NULL)
        return p;
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
    *(u32*)((u8*)self + 8) = 1;
    *(u32*)((u8*)self + 12) = a;
    *(u32*)((u8*)self + 16) = b;
    sfxzmv_SetTagGrp();
}
extern char lbl_eu_80518C38[];
extern void SJ_SearchTag(void* v, const char* a, const char* b, s32* out);

void sfxzmv_SetTagGrp(void* self) {
    struct { u32 a; u32 b; } v;
    s32 out[2];
    if (*(s32*)((u8*)self + 0xC) == 0) {
        *(s32*)((u8*)self + 0x18) = 1;
        *(s32*)((u8*)self + 0x1C) = 0;
        *(s32*)((u8*)self + 0x20) = 0;
        *(s32*)((u8*)self + 0x28) = 1;
        *(s32*)((u8*)self + 0x2C) = 0;
        *(s32*)((u8*)self + 0x30) = 0;
        return;
    }
    v.a = *(u32*)((u8*)self + 0xC);
    v.b = *(u32*)((u8*)self + 0x10);
    SJ_SearchTag(&v, lbl_eu_80518C38, lbl_eu_80518C38 + 6, &out[0]);
    *(s32*)((u8*)self + 0x18) = 1;
    *(s32*)((u8*)self + 0x1C) = out[0];
    SJ_SearchTag(&v, lbl_eu_80518C38 + 14, lbl_eu_80518C38, &out[0]);
    *(s32*)((u8*)self + 0x28) = 1;
    *(s32*)((u8*)self + 0x2C) = out[0];
    *(s32*)((u8*)self + 0x30) = out[1];
}
void SFXZ_GetZfrmRange() {}
void sfxzmv_MakeOrgZ32TblByDirect() {}
void sfxzmv_MakeOrgZ32TblByCCIR() {}
void SFXZ_MakeCnvZTbl() {}
void sfxzmv_MakeZ16TblFromOrgZ32() {}
void sfxzmv_MakeZ32TblFromOrgZ32() {}
