// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_xpnd
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void* memset(void* s, int c, size_t n);
extern char lbl_eu_805E4F80[];
void ADXPD_Init(void) {
    memset(lbl_eu_805E4F80, 0, 0x3c0);
}

void ADXPD_Create() {}

void ADX_GetCoefficient(void* a, void* b, void* c, void* d);
void ADXPD_SetCoef(void* self, void* tbl, void* idx) {
    ADX_GetCoefficient(idx, tbl, (u8*)self + 0x30, (u8*)self + 0x32);
}

void ADXPD_SetDly(void* self, s16* a, s16* b) {
    *(s16*)((u8*)self + 0x28) = a[0];
    *(s16*)((u8*)self + 0x2A) = b[0];
    *(s16*)((u8*)self + 0x2C) = a[1];
    *(s16*)((u8*)self + 0x2E) = b[1];
}

void ADXPD_GetDly(void* self, s16* a, s16* b) {
    a[0] = *(s16*)((u8*)self + 0x28);
    b[0] = *(s16*)((u8*)self + 0x2A);
    a[1] = *(s16*)((u8*)self + 0x2C);
    b[1] = *(s16*)((u8*)self + 0x2E);
}

void ADXPD_SetExtPrm(void* self, s16 a, s16 b, s16 c) {
    *(s16*)((u8*)self + 0x34) = a;
    *(s16*)((u8*)self + 0x36) = b;
    *(s16*)((u8*)self + 0x38) = c;
}

void ADXPD_GetExtPrm(void* self, s16* out1, s16* out2, s16* out3) {
    *out1 = *(s16*)((u8*)self + 0x34);
    *out2 = *(s16*)((u8*)self + 0x36);
    *out3 = *(s16*)((u8*)self + 0x38);
}

void ADXPD_Destroy(void* self) {
    if (self == NULL) return;
    *(u32*)self = 0;
    memset(self, 0, 0x3c);
}

u32 ADXPD_GetStat(void* self) { return *(u32*)((u8*)self + 0xc); }

int ADXPD_EntryMono(void* r3, int r4, int r5, int r6, int r7) {
    if (*(int*)((char*)r3 + 0xc) != 0) return 0;
    *(int*)((char*)r3 + 0x14) = 1;
    *(int*)((char*)r3 + 0x18) = r4;
    *(int*)((char*)r3 + 0x1c) = r5;
    *(int*)((char*)r3 + 0x20) = r6;
    *(int*)((char*)r3 + 0x24) = r7;
    return 1;
}

int ADXPD_EntrySte(void* r3, int r4, int r5, int r6, int r7) {
    if (*(int*)((char*)r3 + 0xc) != 0) return 0;
    *(int*)((char*)r3 + 0x14) = 2;
    *(int*)((char*)r3 + 0x18) = r4;
    *(int*)((char*)r3 + 0x1c) = r5;
    *(int*)((char*)r3 + 0x20) = r6;
    *(int*)((char*)r3 + 0x24) = r7;
    return 1;
}

int ADXPD_EntryPl2(void* r3, int r4, int r5, int r6, int r7) {
    if (*(int*)((char*)r3 + 0xc) != 0) return 0;
    *(int*)((char*)r3 + 0x14) = 1;
    *(int*)((char*)r3 + 0x18) = r4;
    *(int*)((char*)r3 + 0x1c) = r5;
    *(int*)((char*)r3 + 0x20) = r6;
    *(int*)((char*)r3 + 0x24) = r7;
    return 1;
}

void ADXPD_Start(void* self) {
    if (*(u32*)((u8*)self + 0xc) != 0) return;
    *(u32*)((u8*)self + 0x10) = 0;
    *(u32*)((u8*)self + 0xc) = 1;
}

void* memset(void* s, int c, size_t n);
void ADXPD_Stop(void* self) {
    *(u32*)((u8*)self + 0x0c) = 0;
    memset((u8*)self + 0x28, 0, 8);
}

void ADXPD_Reset(void* self) {
    if (*(u32*)((u8*)self + 0x0c) == 3) {
        *(u32*)((u8*)self + 0x0c) = 0;
    }
}

u32 ADXPD_GetNumBlk(void* self) { return *(u32*)((u8*)self + 0x10); }

void ADXPD_ExecHndl() {}
