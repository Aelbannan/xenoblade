// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_bsc
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern void ADXPD_Init(void);
extern void* ADXPD_Create(void);
extern void ADXPD_Destroy(void* pd);
extern void ADXPD_Stop(void* pd);
extern void ADXPD_Reset(void* pd);
extern void ADXPD_EntryMono(void* pd, void* data, void* dest, void* arg7);
extern void ADXPD_EntrySte(void* pd, void* data, s32 num, void* dest, void* arg7);
extern void ADXPD_EntryPl2(void* pd, void* data, s32 num, void* dest, void* arg7);
extern void ADXPD_Start(void* pd);
extern int ADXPD_GetStat(void* pd);
extern int ADXPD_GetNumBlk(void* pd);
extern void ADXPD_ExecHndl(void* pd);
extern void ADXPD_GetDly(void* pd, void* out1, void* out2);
extern void ADXPD_SetDly(void* pd, void* in1, void* in2);
extern void ADXPD_GetExtPrm(void* pd, void* out1, void* out2, void* out3);
extern void ADXPD_SetExtPrm(void* pd, s16 v1, s16 v2, s16 v3);

extern s32 ADX_DecodeInfo(void* data, s32 size, void* out_audioOffset,
    void* out_encoding, void* out_version, void* out_blockSize,
    void* out_channelCount, void* out_sampleRate,
    void* out_totalSamples, void* out_samplesPerBlock);
extern s32 ADX_DecodeInfoExVer(void* data, s32 size, void* out_version, void* out_flags);
extern s32 ADX_DecodeInfoExADPCM2(void* data, s32 size, void* out);
extern s32 ADX_DecodeInfoExIdly(void* data, s32 size, void* out1, void* out2);
extern s32 ADX_DecodeInfoExLoop(void* data, s32 size,
    void* out1, void* out2, void* out3, void* out4,
    void* out5, void* out6, void* out7);
extern s32 ADX_DecodeInfoAinf(void* data, s32 size,
    void* out1, void* out2, void* out3, void* out4);
extern void ADXPD_SetCoef(void* pd, u32 sampleRate, s16 coef);
extern int ADXB_CheckSpsd(void* data);
extern int ADXB_CheckWav(void* data);
extern int ADXB_CheckAiff(void* data);
extern int ADXB_CheckAu(void* data);
extern s32 ADXB_DecodeHeaderSpsd(void* adxb, void* data, void* out_encoding);
extern s32 ADXB_DecodeHeaderWav(void* adxb, void* data, void* out_encoding);
extern s32 ADXB_DecodeHeaderAiff(void* adxb, void* data, void* out_encoding);
extern s32 ADXB_DecodeHeaderAu(void* adxb, void* data, void* out_encoding);
extern void ADXB_ExecOneAhx(void* adxb);
extern void ADXB_ExecOneSpsd(void* adxb);
extern void ADXB_ExecOneAiff(void* adxb);
extern void ADXB_ExecOneAu(void* adxb);
extern void ADXB_ExecOneWav(void* adxb);
extern void ADXCRS_Lock(void);
extern void ADXCRS_Unlock(void);

extern void CRICRW_Sprintf(char* buf, int size, const char* fmt, ...);

extern u32 lbl_eu_805E5370;
extern u32 lbl_eu_805E5358;
extern u8 lbl_eu_805E5378[0x1000];
extern u32 lbl_eu_80560048;
extern s16 lbl_eu_80516B30[];

extern char lbl_eu_80517330[];
extern void ADXERR_CallErrFunc1_(const char* msg);
extern void ADXERR_CallErrFunc2_(const char* msg, const char* detail);

extern void (*lbl_eu_805E5364)(void*);
extern void (*lbl_eu_805E535C)(void*, void*);
extern void (*lbl_eu_805E5360)(void*, s16, void*, void*);

/* Forward declarations */
int adxb_get_key(void* adxb, void* data, void* out_key1, void* out_key2, void* out_key3);
s32 ADXB_DecodeHeaderAdx(void* adxb, void* data, void* out_encoding);

static s16 SKG_GenerateKey(u8* data, s16 len, s16* key1, s16* key2, s16* key3) {
    s16 x1;
    s16 x2;
    s16 x3;
    int i;

    if (lbl_eu_805E5358 == 0) {
        lbl_eu_805E5358 = lbl_eu_805E5358 + 1;
    }

    *key1 = 0;
    *key2 = 0;
    *key3 = 0;

    if (data == NULL && len <= 0) {
        return 0;
    }

    x1 = 0x4A1D;
    for (i = 0; i < len; i++) {
        u32 m = (u32)((s32)x1 * (s32)lbl_eu_80516B30[0x80 + (s8)data[i]]);
        u32 b = m << 22;
        u32 s = m >> 31;
        u32 t = (b - s);
        t = (t << 10) | (t >> 22);
        t = t + s;
        x1 = lbl_eu_80516B30[t];
    }

    x2 = 0x53FF;
    for (i = 0; i < len; i++) {
        u32 m = (u32)((s32)x2 * (s32)lbl_eu_80516B30[0x80 + (s8)data[i]]);
        u32 b = m << 22;
        u32 s = m >> 31;
        u32 t = (b - s);
        t = (t << 10) | (t >> 22);
        t = t + s;
        x2 = lbl_eu_80516B30[t];
    }

    x3 = 0x5DC1;
    for (i = 0; i < len; i++) {
        u32 m = (u32)((s32)x3 * (s32)lbl_eu_80516B30[0x80 + (s8)data[i]]);
        u32 b = m << 22;
        u32 s = m >> 31;
        u32 t = (b - s);
        t = (t << 10) | (t >> 22);
        t = t + s;
        x3 = lbl_eu_80516B30[t];
    }

    *key1 = x1;
    *key2 = x2;
    *key3 = x3;

    return 0;
}

u32 ADXB_GetDecErrMode(void) { return lbl_eu_805E5370; }

void ADXB_Init(void) {
    u8* p = (u8*)&lbl_eu_805E5358;
    ADXPD_Init();
    (*(volatile u32*)p)++;
    memset(p + 0x20, 0, 0x1000);
    *(u32*)(p + 0x18) = 0;
}

u32 adxb_DefGetWr(void* self, u32* out1, u32* out2, u32* out3) {
    *out1 = *(u32*)((u8*)self + 0x8C);
    *out2 = *(u32*)((u8*)self + 0x40) - *(u32*)((u8*)self + 0x8C);
    *out3 = *(u32*)((u8*)self + 0x18) - *(u32*)((u8*)self + 0x88);
    return *(u32*)((u8*)self + 0x3C);
}

void adxb_DefAddWr(void* self, u32 unused, u32 addend) {
    *(u32*)((u8*)self + 0x8c) += addend;
    *(u32*)((u8*)self + 0x88) += addend;
}

void* ADXB_Create(void* arg0, void* pcmBuf, void* workBuf, void* workSize) {
    int i;
    u8* slot;
    u8* p;

    p = (u8*)lbl_eu_805E5378;
    for (i = 0; i < 16; i++) {
        if (*(s16*)p == 0) break;
        p += 0x100;
    }
    if (i == 16) return NULL;

    slot = (u8*)lbl_eu_805E5378 + i * 0x100;
    memset(slot, 0, 0x100);
    *(u16*)(slot + 0x00) = 1;

    {
        void* pd = ADXPD_Create();
        *(void**)(slot + 0x08) = pd;
        if (pd == NULL) {
            if (slot != NULL) {
                *(void**)(slot + 0x08) = NULL;
                ADXPD_Destroy(pd);
                memset(slot, 0, 0x100);
                *(u16*)(slot + 0x00) = 0;
            }
            return NULL;
        }
    }

    *(void**)(slot + 0x38) = arg0;
    *(void**)(slot + 0x3C) = pcmBuf;
    *(void**)(slot + 0x40) = workBuf;
    *(void**)(slot + 0x44) = workSize;
    *(void**)(slot + 0x78) = (void*)adxb_DefGetWr;
    *(void**)(slot + 0x7C) = slot;
    *(void**)(slot + 0x80) = (void*)adxb_DefAddWr;
    *(void**)(slot + 0x84) = slot;
    *(u32*)(slot + 0xC4) = 0;
    *(s16*)(slot + 0xD8) = 0;
    *(s16*)(slot + 0xDA) = -0x80;
    *(s16*)(slot + 0xDC) = -0x80;
    memset(slot + 0xC8, 0, 0x10);
    return slot;
}

void ADXB_Destroy(void* self) {
    void* pd;
    if (self == NULL) return;
    pd = *(void**)((u8*)self + 0x08);
    *(void**)((u8*)self + 0x08) = NULL;
    ADXPD_Destroy(pd);
    memset(self, 0, 0x100);
    *(u16*)((u8*)self + 0x00) = 0;
}

int adxb_get_key(void* adxb, void* data, void* out_key1, void* out_key2, void* out_key3) {
    u8* p = (u8*)adxb;
    s16* k3 = (s16*)out_key3;
    s16* k2 = (s16*)out_key2;
    s16* k1 = (s16*)out_key1;
    s16* defKeys = (s16*)((u8*)&lbl_eu_805E5358 + 0x10);

    if (lbl_eu_80560048 == 0) {
        if (p[0xEC] < 4) {
            *k1 = 0;
            *k2 = 0;
            *k3 = 0;
        } else if (p[0xED] >= 0x10) {
            char buf[16];
            CRICRW_Sprintf(buf, 16, lbl_eu_80517330 + 0xA7, data);
            SKG_GenerateKey((u8*)buf, 8, k1, k2, k3);
        } else if (p[0xED] >= 8) {
            if (*(s16*)(p + 0xA0) == 0 && *(s16*)(p + 0xA2) == 0 && *(s16*)(p + 0xA4) == 0) {
                *(s16*)(p + 0xA0) = defKeys[0];
                *(s16*)(p + 0xA2) = defKeys[1];
                *(s16*)(p + 0xA4) = defKeys[2];
            }
            *k1 = *(s16*)(p + 0xA0);
            *k2 = *(s16*)(p + 0xA2);
            *k3 = *(s16*)(p + 0xA4);
        } else {
            *k1 = 0;
            *k2 = 0;
            *k3 = 0;
        }
    } else {
        if (p[0xEC] < 4) {
            if (*(s16*)(p + 0xA0) == 0 && *(s16*)(p + 0xA2) == 0 && *(s16*)(p + 0xA4) == 0 &&
                defKeys[0] == 0 && defKeys[1] == 0 && defKeys[2] == 0) {
                *k1 = 0;
                *k2 = 0;
                *k3 = 0;
                return 0;
            }
            *k1 = -0x1000;
            *k2 = -0x7F24;
            *k3 = 0x7FFF;
            ADXERR_CallErrFunc1_(lbl_eu_80517330 + 0xAC);
            return -1;
        } else {
            if (*(s16*)(p + 0xA0) == 0 && *(s16*)(p + 0xA2) == 0 && *(s16*)(p + 0xA4) == 0) {
                *(s16*)(p + 0xA0) = defKeys[0];
                *(s16*)(p + 0xA2) = defKeys[1];
                *(s16*)(p + 0xA4) = defKeys[2];
            }
            *k1 = *(s16*)(p + 0xA0);
            *k2 = *(s16*)(p + 0xA2);
            *k3 = *(s16*)(p + 0xA4);
        }
    }
    return 0;
}

s32 ADXB_DecodeHeaderAdx(void* adxb, void* data, void* out_encoding) {
    u8* p = (u8*)adxb;
    s16 l3, l2, l1;
    s16 highPassVal;
    s16 dly2, dly1;
    s16 sp32;
    s16 k1, k2, k3;

    if (((u32)data & 1) != 0 && lbl_eu_805E5370 == 0) {
        ADXERR_CallErrFunc2_(lbl_eu_80517330, lbl_eu_80517330 + 0x1F);
        return -1;
    }

    *(u16*)(p + 0x02) = 1;

    if (ADX_DecodeInfo(data, (s32)out_encoding, &highPassVal,
            p + 0x0C, p + 0x0D, p + 0x0F, p + 0x0E,
            p + 0x14, p + 0x18, p + 0x10) < 0) {
        return 0;
    }

    if ((s8)p[0x0C] > 4) {
        if (*(u32*)(p + 0xB8) == 0) {
            ADXERR_CallErrFunc2_(lbl_eu_80517330 + 0x3E, lbl_eu_80517330 + 0x5E);
            return -1;
        }

        p[0x0D] = 8;
        p[0x0F] = p[0x0E] * 192;
        *(u32*)(p + 0x10) = 96;
        *(s16*)(p + 0x98) = 10;
        *(s16*)(p + 0x1C) = 0;
        *(s16*)(p + 0x24) = 0;
        *(s16*)(p + 0x26) = 0;
        *(u32*)(p + 0x20) = 0;
        *(u32*)(p + 0x28) = 0;
        *(u32*)(p + 0x2C) = 0;
        *(u32*)(p + 0x30) = 0;
        *(u32*)(p + 0x34) = 0;
        *(u32*)(p + 0x88) = 0;

        if (ADX_DecodeInfoExVer(data, (s32)out_encoding, p + 0xEC, p + 0xED) < 0) {
            return 0;
        }

        sp32 = 0;

        if (adxb_get_key(adxb, (void*)*(u32*)(p + 0x18), &k1, &k2, &k3) < 0) {
            return -1;
        }

        if (lbl_eu_805E535C != NULL) {
            lbl_eu_805E535C(*(void**)(p + 0xB8), &sp32);
        }
    } else {
        if (ADX_DecodeInfoExVer(data, (s32)out_encoding, p + 0xEC, p + 0xED) < 0) {
            return 0;
        }

        if (adxb_get_key(adxb, (void*)*(u32*)(p + 0x18), &l1, &l2, &l3) < 0) {
            return -1;
        }

        ADXPD_SetExtPrm(*(void**)(p + 0x08), l1, l2, l3);

        if (ADX_DecodeInfoExADPCM2(data, (s32)out_encoding, p + 0x1C) < 0) {
            return 0;
        }

        if (ADX_DecodeInfoExIdly(data, (s32)out_encoding, &dly1, &dly2) < 0) {
            return 0;
        }

        ADXPD_SetCoef(*(void**)(p + 0x08), *(u32*)(p + 0x14), *(s16*)(p + 0x1C));
        ADXPD_SetDly(*(void**)(p + 0x08), &dly1, &dly2);
        ADX_DecodeInfoExLoop(data, (s32)out_encoding,
            p + 0x20, p + 0x24, p + 0x26, p + 0x28,
            p + 0x2C, p + 0x30, p + 0x34);
        ADX_DecodeInfoAinf(data, (s32)out_encoding,
            p + 0xC4, p + 0xC8, p + 0xD8, p + 0xDA);
    }

    *(s32*)(p + 0x50) = (s8)p[0x0E];
    *(s32*)(p + 0x54) = (s8)p[0x0F];
    *(s32*)(p + 0x58) = *(u32*)(p + 0x10);
    *(s32*)(p + 0x5C) = *(u32*)(p + 0x3C);
    *(s32*)(p + 0x60) = *(u32*)(p + 0x40);
    *(s32*)(p + 0x64) = *(u32*)(p + 0x44);
    *(u32*)(p + 0x8C) = 0;

    return highPassVal;
}

void ADXB_SetDefFmt(void* self) {
    *(s16*)((u8*)self + 0x98) = 0;
}

void ADXB_SetDefPrm(void* self);
void criware_eu_8038A864(void* self) {
    *(s16*)((u8*)self + 0x98) = *(s16*)((u8*)self + 0x9a);
    ADXB_SetDefPrm(self);
}

void ADXB_SetDefPrm(void* self) {
    u8* p = (u8*)self;
    u32 v3c = *(u32*)(p + 0x3c);
    u32 v40 = *(u32*)(p + 0x40);
    u32 v44 = *(u32*)(p + 0x44);

    *(u16*)(p + 0x02) = 1;
    *(u32*)(p + 0x14) = 48000;
    *(u8*)(p + 0x0e) = 2;
    *(u8*)(p + 0x0d) = 16;
    *(u32*)(p + 0x18) = 0x7fffffff;
    *(u8*)(p + 0x0f) = 0x7f;
    *(u32*)(p + 0x10) = 0x400;
    *(u32*)(p + 0x50) = 2;
    *(u32*)(p + 0x54) = 0x7f;
    *(u32*)(p + 0x58) = 0x400;
    *(u32*)(p + 0x5c) = v3c;
    *(u32*)(p + 0x60) = v40;
    *(u32*)(p + 0x64) = v44;
    *(u32*)(p + 0x8c) = 0;
    *(u16*)(p + 0x1c) = 0;
    *(u16*)(p + 0x24) = 0;
    *(u16*)(p + 0x26) = 0;
    *(u32*)(p + 0x20) = 0;
    *(u32*)(p + 0x28) = 0;
    *(u32*)(p + 0x2c) = 0;
    *(u32*)(p + 0x30) = 0;
    *(u32*)(p + 0x34) = 0;
    *(u32*)(p + 0x88) = 0;
    *(u8*)(p + 0xec) = 0;
    *(u8*)(p + 0xed) = 0;
}

s32 ADXB_DecodeHeader(void* adxb, void* data, void* out_encoding) {
    u8* p = (u8*)adxb;
    u8* d = (u8*)data;
    u16 magic;

    *(u32*)(p + 0xC4) = 0;
    *(s16*)(p + 0xD8) = 0;
    *(s16*)(p + 0xDA) = -128;
    *(s16*)(p + 0xDC) = -128;
    memset(p + 0xC8, 0, 16);

    magic = (u16)((d[0] << 8) | d[1]);

    if (magic == 0x8000) {
        return ADXB_DecodeHeaderAdx(adxb, data, out_encoding);
    }
    if (ADXB_CheckSpsd(data)) {
        return ADXB_DecodeHeaderSpsd(adxb, data, out_encoding);
    }
    if (ADXB_CheckWav(data)) {
        return ADXB_DecodeHeaderWav(adxb, data, out_encoding);
    }
    if (ADXB_CheckAiff(data)) {
        return ADXB_DecodeHeaderAiff(adxb, data, out_encoding);
    }
    if (ADXB_CheckAu(data)) {
        return ADXB_DecodeHeaderAu(adxb, data, out_encoding);
    }
    return -1;
}

void ADXB_EntryGetWrFunc(void* self, void* func, void* ctx) {
    *(void**)((u8*)self + 0x78) = func;
    *(void**)((u8*)self + 0x7c) = ctx;
}

u32 ADXB_GetPcmBuf(void* self) { return *(u32*)((u8*)self + 0x3c); }

s16 ADXB_GetFormat(void* self) { return *(s16*)((u8*)self + 0x98); }

u32 ADXB_GetSfreq(void* self) { return *(u32*)((u8*)self + 0x14); }

s32 ADXB_GetNumChan(void* self) {
    int ch = *(u8*)((u8*)self + 0x0E);
    if (ch == 1 && *(u32*)((u8*)self + 0xE0) != 0)
        return 2;
    if (self == NULL) {
        ADXERR_CallErrFunc1_(lbl_eu_80517330 + 0x81);
        return -1;
    }
    return (s8)ch;
}

s16 ADXB_GetFmtBps(void* self) {
    return *(s8*)((u8*)self + 0x0D);
}

s32 ADXB_GetOutBps(void* self) {
    s16 val = *(s16*)((u8*)self + 0x98);
    if (val == 0) return 0x10;
    if (val == 2) {
        s16 sub = *(s16*)((u8*)self + 0x9c);
        if (sub == 2) return 4;
        if (sub == 1) return 8;
        return 0x10;
    }
    if (val == 1) {
        s16 sub = *(s16*)((u8*)self + 0x9c);
        if (sub == 2) return 4;
        return 0x10;
    }
    return 0x10;
}

u32 ADXB_GetBlkSmpl(void* self) { return *(u32*)((u8*)self + 0x10); }

u32 ADXB_GetTotalNumSmpl(void* self) { return *(u32*)((u8*)self + 0x18); }

s16 ADXB_GetNumLoop(void* self) { return *(s16*)((u8*)self + 0x24); }

u32 ADXB_GetLpStartPos(void* self) { return *(u32*)((u8*)self + 0x28); }

u32 ADXB_GetLpStartOfst(void* self) {
    if (self == NULL) return 0;
    return *(u32*)((u8*)self + 0x2c);
}

u32 ADXB_GetLpEndPos(void* self) { return *(u32*)((u8*)self + 0x30); }

u32 ADXB_GetLpEndOfst(void* self) { return *(u32*)((u8*)self + 0x34); }

u32 ADXB_GetAinfLen(void* self) { return *(u32*)((u8*)self + 0xc4); }

s16 ADXB_GetDefOutVol(void* self) { return *(s16*)((u8*)self + 0xd8); }

s16 ADXB_GetDefPan(void* self, u32 idx) {
    return *(s16*)((u8*)self + 0xda + idx * 2);
}

void ADXB_TakeSnapshot(void* self) {
    ADXPD_GetDly(*(void**)((u8*)self + 0x08), (u8*)self + 0xb0, (u8*)self + 0xb4);
    ADXPD_GetExtPrm(*(void**)((u8*)self + 0x08), (u8*)self + 0xa6, (u8*)self + 0xa8, (u8*)self + 0xaa);
}

void ADXB_RestoreSnapshot(void* self) {
    ADXPD_SetDly(*(void**)((u8*)self + 0x08), (u32*)((u8*)self + 0xB0), (u32*)((u8*)self + 0xB4));
    ADXPD_SetExtPrm(*(void**)((u8*)self + 0x08),
        *(s16*)((u8*)self + 0xA6),
        *(s16*)((u8*)self + 0xA8),
        *(s16*)((u8*)self + 0xAA));
}

void ADXB_SetLnkSw(void* self, int val) {
    if (val == 0) {
        *(u32*)((u8*)self + 0xAC) = 0;
    } else {
        *(u32*)((u8*)self + 0xAC) = 1;
    }
}

u32 ADXB_GetStat(void* self) { return *(u32*)((u8*)self + 0x4); }

void ADXB_EntryData(void* self, void* data, int size) {
    if (*(s16*)((u8*)self + 0x98) == 0) {
        *(void**)((u8*)self + 0x48) = data;
        {
            int ch = (s8)*(u8*)((u8*)self + 0x0F);
            *(u32*)((u8*)self + 0x74) = 0;
            *(u32*)((u8*)self + 0x4C) = size / ch;
        }
    } else {
        int t = (s8)*(u8*)((u8*)self + 0x0D);
        int ch = (s8)*(u8*)((u8*)self + 0x0E);
        *(void**)((u8*)self + 0x48) = data;
        *(u32*)((u8*)self + 0x74) = 0;
        *(u32*)((u8*)self + 0x4C) = size / ((t / 8) * ch);
    }
    *(u32*)((u8*)self + 0x90) = 0;
    *(u32*)((u8*)self + 0x94) = 0;
    *(u32*)((u8*)self + 0xF4) = 0;
    *(u32*)((u8*)self + 0xF0) = 0;
}

void ADXB_Start(void* self) {
    if (*(u32*)((u8*)self + 0x04) != 0) return;
    *(u32*)((u8*)self + 0x04) = 1;
}

void ADXB_Stop(void* self) {
    if (*(u32*)((u8*)self + 0xE0) != 0) {
        lbl_eu_805E5364(self);
    }
    ADXPD_Stop(*(void**)((u8*)self + 0x08));
    *(u32*)((u8*)self + 0x04) = 0;
}

void ADXB_Reset(void* self) {
    if (*(int*)((u8*)self + 0x04) == 3) {
        ADXPD_Reset(*(void**)((u8*)self + 0x08));
        *(u32*)((u8*)self + 0x8C) = 0;
        *(u32*)((u8*)self + 0x04) = 0;
    }
}

u32 ADXB_GetDecDtLen(void* self) { return *(u32*)((u8*)self + 0x94); }

u32 ADXB_GetDecNumSmpl(void* self) { return *(u32*)((u8*)self + 0x90); }

void ADXB_EvokeDecode(void* adxb) {
    s32 blkSize = *(s32*)((u8*)adxb + 0x58);
    s32 wrPos = *(s32*)((u8*)adxb + 0x70);
    s32 pcmOfst = *(s32*)((u8*)adxb + 0x68);
    s32 pcmBufSize = *(s32*)((u8*)adxb + 0x60);
    s32 blkSmpl = *(s32*)((u8*)adxb + 0x50);
    s32 maxBlks = *(s32*)((u8*)adxb + 0x4C);
    s32 availWrPos = *(s32*)((u8*)adxb + 0x6C);
    s32 tmp;
    s32 numBlks;
    s32 endAdj;
    s32 decodeEnd;
    s32 deSize;
    s32 availBlks;

    tmp = (wrPos + blkSize) - 1;
    numBlks = tmp / blkSize;
    endAdj = (blkSize - 1) - (tmp % blkSize);
    decodeEnd = (pcmBufSize - pcmOfst + blkSize - 1) / blkSize;
    deSize = decodeEnd * blkSize;
    maxBlks = maxBlks / blkSmpl;

    if (numBlks < decodeEnd) {
        if (pcmOfst + deSize - endAdj < pcmBufSize) {
            decodeEnd++;
        }
    }

    if (wrPos < availWrPos) {
        availBlks = (availWrPos + endAdj) / blkSize;
        if (maxBlks > availBlks) maxBlks = availBlks;
    }
    if (maxBlks > numBlks) maxBlks = numBlks;
    if (maxBlks > decodeEnd) maxBlks = decodeEnd;

    if (blkSmpl == 2) {
        void* pd = *(void**)((u8*)adxb + 0x08);
        ADXPD_EntrySte(pd, *(void**)((u8*)adxb + 0x48), maxBlks << 1,
            (void*)(*(u32*)((u8*)adxb + 0x5C) + (*(u32*)((u8*)adxb + 0x68) << 1)),
            (void*)(*(u32*)((u8*)adxb + 0x5C) + (*(u32*)((u8*)adxb + 0x68) << 1) + (*(u32*)((u8*)adxb + 0x64) << 1)));
        ADXPD_Start(pd);
    } else if (*(s32*)((u8*)adxb + 0xE0) != 0) {
        void* pd = *(void**)((u8*)adxb + 0x08);
        ADXPD_EntryPl2(pd, *(void**)((u8*)adxb + 0x48), maxBlks,
            (void*)(*(u32*)((u8*)adxb + 0x5C) + (*(u32*)((u8*)adxb + 0x68) << 1)),
            (void*)(*(u32*)((u8*)adxb + 0x5C) + (*(u32*)((u8*)adxb + 0x68) << 1) + (*(u32*)((u8*)adxb + 0x64) << 1)));
        ADXPD_Start(pd);
    } else {
        void* pd = *(void**)((u8*)adxb + 0x08);
        ADXPD_EntryMono(pd, *(void**)((u8*)adxb + 0x48),
            (void*)(*(u32*)((u8*)adxb + 0x5C) + (*(u32*)((u8*)adxb + 0x68) << 1)),
            NULL);
        ADXPD_Start(pd);
    }
}

void ADXB_EndDecode(void* adxb) {
    u8* p = (u8*)adxb;
    s32 blkSize = *(s32*)(p + 0x58);
    s32 wrPos = *(s32*)(p + 0x70);
    s32 numBlkSmpl = *(s32*)(p + 0x54);
    u32 pcmOfst = *(u32*)(p + 0x68);
    u32 pcmBase = *(u32*)(p + 0x5C);
    s32 pcmBufSize = *(s32*)(p + 0x40);
    u32 workBufSize = *(u32*)(p + 0x44);
    s32 tmp;
    s32 endAdj2;
    s32 numBlks;
    s32 numBlkPD;
    s32 decSmpl;
    s32 decDtLen;
    s32 pcmEnd;
    s32 samplesLeft;
    int i;

    tmp = wrPos + blkSize - 1;
    endAdj2 = (blkSize - 1) - (tmp % blkSize);
    numBlks = tmp / blkSize;

    numBlkPD = ADXPD_GetNumBlk(*(void**)(p + 0x08));
    decSmpl = (numBlkPD * blkSize) / numBlkSmpl;
    if (numBlks * (s32)(*(u32*)(p + 0x50)) <= numBlkPD) {
        decSmpl -= endAdj2;
    }
    decDtLen = numBlkPD * numBlkSmpl;

    *(u32*)(p + 0x90) = (u32)decSmpl;
    *(u32*)(p + 0x94) = (u32)decDtLen;

    pcmEnd = pcmOfst + decSmpl;
    if (pcmEnd < pcmBufSize) return;

    samplesLeft = pcmEnd - pcmBufSize;

    if (*(s32*)(p + 0x50) == 2 || *(s32*)(p + 0xE0) != 0) {
        {
            s16* d = (s16*)pcmBase;
            const s16* s = (const s16*)(pcmBase + (pcmBufSize << 1));
            s16 n = samplesLeft;
            while (n > 0) {
                *d++ = *s++;
                n--;
            }
        }
        {
            s16* d = (s16*)(pcmBase + (workBufSize << 1));
            const s16* s = (const s16*)(pcmBase + ((workBufSize + pcmBufSize) << 1));
            while (samplesLeft > 0) {
                *d++ = *s++;
                samplesLeft--;
            }
        }
    } else {
        s16* d = (s16*)pcmBase;
        const s16* s = (const s16*)(pcmBase + (pcmBufSize << 1));
        while (samplesLeft > 0) {
            *d++ = *s++;
            samplesLeft--;
        }
    }
}

void ADXB_ExecOneAdx(void* adxb) {
    u8* p = (u8*)adxb;

    if (*(int*)(p + 0x04) == 1) {
        if (ADXPD_GetStat(*(void**)(p + 0x08)) == 0) {
            void (*getWr)(void*, u32*, u32*, u32*);
            void* ctx;

            getWr = *(void (**)(void*, u32*, u32*, u32*))(p + 0x78);
            ctx = *(void**)(p + 0x7C);
            getWr(ctx, (u32*)(p + 0x68), (u32*)(p + 0x6C), (u32*)(p + 0x70));
            ADXB_EvokeDecode(adxb);
            *(int*)(p + 0x04) = 2;
        }
    }

    if (*(int*)(p + 0x04) == 2) {
        ADXPD_ExecHndl(*(void**)(p + 0x08));
        if (ADXPD_GetStat(*(void**)(p + 0x08)) == 3) {
            if (*(int*)(p + 0xE0) != 0) {
                void* pd2 = *(void**)(p + 0x08);
                int i;
                ADXCRS_Lock();
                for (i = 0; i < (int)(*(u32*)((u8*)pd2 + 0x10) << 5); i++) {
                    void* buf1 = *(void**)((u8*)pd2 + 0x20);
                    void* buf2 = *(void**)((u8*)pd2 + 0x24);
                    lbl_eu_805E5360(adxb,
                        *(s16*)((u8*)buf1 + (i << 1)),
                        (u8*)buf1 + (i << 1), (u8*)buf2 + (i << 1));
                }
                ADXCRS_Unlock();
            }
            ADXB_EndDecode(adxb);
            ADXPD_Reset(*(void**)(p + 0x08));
            {
                void (*eosCb)(void*, u32, u32);
                void* eosCtx;
                eosCb = *(void (**)(void*, u32, u32))(p + 0x80);
                eosCtx = *(void**)(p + 0x84);
                eosCb(eosCtx, *(u32*)(p + 0x94), *(u32*)(p + 0x90));
            }
            *(int*)(p + 0x04) = 3;
        }
    }
}

void ADXB_ExecHndl(void* adxb) {
    u8* p = (u8*)adxb;
    s16 fmt = *(s16*)(p + 0x98);

    if (fmt == 0) {
        ADXB_ExecOneAdx(adxb);
    } else if (fmt == 0xA) {
        ADXB_ExecOneAhx(adxb);
    } else if (fmt == 2) {
        ADXB_ExecOneSpsd(adxb);
    } else if (fmt == 3) {
        ADXB_ExecOneAiff(adxb);
    } else if (fmt == 4) {
        ADXB_ExecOneAu(adxb);
    } else if (fmt == 1) {
        ADXB_ExecOneWav(adxb);
    }

    {
        void (*wrCb)(void*, s32, s32);
        wrCb = *(void (**)(void*, s32, s32))(p + 0xF8);
        if (wrCb != NULL) {
            s32 prevLen = *(s32*)(p + 0xF0);
            s32 dataLen = *(s32*)(p + 0x94);
            s32 decodedSmpl = *(s32*)(p + 0x90);
            s32 diff = dataLen - prevLen;
            if (diff < 0) {
                diff = (0x7FFFFFFF - prevLen) + dataLen;
            }
            s8 chan = (s8)*(u8*)(p + 0x0E);
            wrCb(*(void**)(p + 0xFC), diff, chan * (decodedSmpl << 1));
            *(u32*)(p + 0xF0) = *(u32*)(p + 0x94);
        }
    }
}
