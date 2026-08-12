// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_xpnd
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void* memset(void* s, int c, size_t n);
extern char lbl_eu_805E4F80[];
void ADXPD_Init(void) {
    memset(lbl_eu_805E4F80, 0, 0x3c0);
}

void* ADXPD_Create(void) {
    u32* p = (u32*)lbl_eu_805E4F80;
    s32 count = 0;

    while (count < 16) {
        if (*p == 0)
            break;
        p += 15;
        count++;
    }
    if (count == 16)
        return NULL;
    {
        u8* slot = (u8*)lbl_eu_805E4F80 + count * 60;
        memset(slot, 0, 60);
        *(u32*)slot = 1;
        *(u32*)(slot + 4) = (u32)count;
        *(u32*)(slot + 8) = 0;
        *(u32*)(slot + 12) = 0;
        ADX_GetCoefficient((void*)500, (void*)44100, (void*)(slot + 48), (void*)(slot + 50));
        memset(slot + 40, 0, 8);
        return slot;
    }
}

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
    if (*(int*)((char*)r3 + 0xc) == 0) {
        *(int*)((char*)r3 + 0x14) = 1;
        *(int*)((char*)r3 + 0x18) = r4;
        *(int*)((char*)r3 + 0x1c) = r5;
        *(int*)((char*)r3 + 0x20) = r6;
        *(int*)((char*)r3 + 0x24) = r7;
        return 1;
    }
    return 0;
}

int ADXPD_EntrySte(void* r3, int r4, int r5, int r6, int r7) {
    if (*(int*)((char*)r3 + 0xc) == 0) {
        *(int*)((char*)r3 + 0x14) = 2;
        *(int*)((char*)r3 + 0x18) = r4;
        *(int*)((char*)r3 + 0x1c) = r5;
        *(int*)((char*)r3 + 0x20) = r6;
        *(int*)((char*)r3 + 0x24) = r7;
        return 1;
    }
    return 0;
}

int ADXPD_EntryPl2(void* r3, int r4, int r5, int r6, int r7) {
    if (*(int*)((char*)r3 + 0xc) == 0) {
        *(int*)((char*)r3 + 0x14) = 1;
        *(int*)((char*)r3 + 0x18) = r4;
        *(int*)((char*)r3 + 0x1c) = r5;
        *(int*)((char*)r3 + 0x20) = r6;
        *(int*)((char*)r3 + 0x24) = r7;
        return 1;
    }
    return 0;
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
    if (*(s32*)((u8*)self + 0x0c) == 3) {
        *(u32*)((u8*)self + 0x0c) = 0;
    }
}

u32 ADXPD_GetNumBlk(void* self) { return *(u32*)((u8*)self + 0x10); }

extern u32 lbl_eu_805E5340;
extern int ADX_DecodeMono4(const u8* in, int nblocks, s16* out, s16* prev,
                           s16 c1, s16 c2, s16* hist, s16 scale, s16 pitch);
extern int ADX_DecodeSte4(const u8* in, int nblocks, s16* out1, s16* prev1,
                          s16* out2, s16* prev2, s16 c1, s16 c2,
                          s16* hist, s16 v1, s16 v2);

void ADXPD_ExecHndl(void* self) {
    if (*(s32*)((u8*)self + 12) == 1) {
        *(s32*)((u8*)self + 12) = 2;
    }
    if (*(s32*)((u8*)self + 12) == 2) {
        s32 r;
        s32 t;
        s32 sign;
        if (*(s32*)((u8*)self + 20) == 1) {
            s16 pitch = *(s16*)((u8*)self + 56);
            r = ADX_DecodeMono4(*(u8**)((u8*)self + 24), *(s32*)((u8*)self + 28),
                                *(s16**)((u8*)self + 32), (s16*)((u8*)self + 40),
                                *(s16*)((u8*)self + 48), *(s16*)((u8*)self + 50),
                                (s16*)((u8*)self + 52), *(s16*)((u8*)self + 54),
                                pitch);
            *(s32*)((u8*)self + 16) = r;
        } else {
            r = ADX_DecodeSte4(*(u8**)((u8*)self + 24), *(s32*)((u8*)self + 28),
                               *(s16**)((u8*)self + 32), (s16*)((u8*)self + 40),
                               *(s16**)((u8*)self + 36), (s16*)((u8*)self + 44),
                               *(s16*)((u8*)self + 48), *(s16*)((u8*)self + 50),
                               (s16*)((u8*)self + 52), *(s16*)((u8*)self + 54),
                               *(s16*)((u8*)self + 56));
            sign = (r >> 31) & 1;
            t = (r & 1) ^ sign;
            *(s32*)((u8*)self + 16) = r;
            if (t - sign == 1) {
                lbl_eu_805E5340 = 1;
            }
        }
        *(s32*)((u8*)self + 12) = 3;
    }
}
