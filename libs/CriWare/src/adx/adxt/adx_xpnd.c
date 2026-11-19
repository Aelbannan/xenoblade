// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_xpnd
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include "libs/CriWare/src/adx/adxt/adx_xpnd.h"

void* memset(void* s, int c, size_t n);
unsigned char lbl_eu_805E4F80[0x3C0];
unsigned char lbl_eu_805E5340[8];
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

void ADXPD_SetCoef(ADXPD* self, void* tbl, void* idx) {
    ADX_GetCoefficient(idx, tbl, &self->coef[0], &self->coef[1]);
}

void ADXPD_SetDly(ADXPD* self, s16* a, s16* b) {
    self->dly[0][0] = a[0];
    self->dly[0][1] = b[0];
    self->dly[1][0] = a[1];
    self->dly[1][1] = b[1];
}

void ADXPD_GetDly(ADXPD* self, s16* a, s16* b) {
    a[0] = self->dly[0][0];
    b[0] = self->dly[0][1];
    a[1] = self->dly[1][0];
    b[1] = self->dly[1][1];
}

void ADXPD_SetExtPrm(ADXPD* self, s16 a, s16 b, s16 c) {
    self->extPrm[0] = a;
    self->extPrm[1] = b;
    self->extPrm[2] = c;
}

void ADXPD_GetExtPrm(ADXPD* self, s16* out1, s16* out2, s16* out3) {
    *out1 = self->extPrm[0];
    *out2 = self->extPrm[1];
    *out3 = self->extPrm[2];
}

void ADXPD_Destroy(ADXPD* self) {
    if (self == NULL) return;
    self->inUse = 0;
    memset(self, 0, 0x3c);
}

u32 ADXPD_GetStat(ADXPD* self) { return self->stat; }

int ADXPD_EntryMono(ADXPD* p, int r4, int r5, int r6, int r7) {
    if ((int)p->stat == 0) {
        p->mode = 1;
        p->in = (const u8*)r4;
        p->blkNum = r5;
        p->out1 = (s16*)r6;
        p->out2 = (s16*)r7;
        return 1;
    }
    return 0;
}

int ADXPD_EntrySte(ADXPD* p, int r4, int r5, int r6, int r7) {
    if ((int)p->stat == 0) {
        p->mode = 2;
        p->in = (const u8*)r4;
        p->blkNum = r5;
        p->out1 = (s16*)r6;
        p->out2 = (s16*)r7;
        return 1;
    }
    return 0;
}

int ADXPD_EntryPl2(ADXPD* p, int r4, int r5, int r6, int r7) {
    if ((int)p->stat == 0) {
        p->mode = 1;
        p->in = (const u8*)r4;
        p->blkNum = r5;
        p->out1 = (s16*)r6;
        p->out2 = (s16*)r7;
        return 1;
    }
    return 0;
}

void ADXPD_Start(ADXPD* self) {
    if (self->stat != 0) return;
    self->numBlk = 0;
    self->stat = 1;
}

void* memset(void* s, int c, size_t n);
void ADXPD_Stop(ADXPD* self) {
    self->stat = 0;
    memset(self->dly, 0, 8);
}

void ADXPD_Reset(ADXPD* self) {
    if ((s32)self->stat == 3) {
        self->stat = 0;
    }
}

u32 ADXPD_GetNumBlk(ADXPD* self) { return self->numBlk; }

extern int ADX_DecodeMono4(const u8* in, int nblocks, s16* out, s16* prev,
                           s16 c1, s16 c2, s16* hist, s16 scale, s16 pitch);
extern int ADX_DecodeSte4(const u8* in, int nblocks, s16* out1, s16* prev1,
                          s16* out2, s16* prev2, s16 c1, s16 c2,
                          s16* hist, s16 v1, s16 v2);

void ADXPD_ExecHndl(ADXPD* self) {
    if ((s32)self->stat == 1) {
        self->stat = 2;
    }
    if ((s32)self->stat == 2) {
        s32 r;
        s32 t;
        s32 sign;
        if (self->mode == 1) {
            s16 pitch = self->extPrm[2];
            r = ADX_DecodeMono4(self->in, self->blkNum,
                                self->out1, &self->dly[0][0],
                                self->coef[0], self->coef[1],
                                &self->extPrm[0], self->extPrm[1],
                                pitch);
            self->numBlk = r;
        } else {
            r = ADX_DecodeSte4(self->in, self->blkNum,
                               self->out1, &self->dly[0][0],
                               self->out2, &self->dly[1][0],
                               self->coef[0], self->coef[1],
                               &self->extPrm[0], self->extPrm[1],
                               self->extPrm[2]);
            sign = (r >> 31) & 1;
            t = (r & 1) ^ sign;
            self->numBlk = r;
            if (t - sign == 1) {
                *(u32*)lbl_eu_805E5340 = 1;
            }
        }
        self->stat = 3;
    }
}
