// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_bsc
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern void ADXPD_Init(void);
extern void* ADXPD_Create(void);
extern void ADXPD_Destroy(void* pd);
extern void ADXPD_Stop(void* pd);
extern void ADXPD_Reset(void* pd);
extern void ADXPD_EntryMono(void* pd, void* data, void* dest, void* arg7);
extern void ADXPD_EntrySte(void* pd, void* data, void* dest, void* arg7);
extern void ADXPD_EntryPl2(void* pd, void* data, void* dest, void* arg7);
extern void ADXPD_Start(void* pd);
extern int ADXPD_GetStat(void* pd);
extern int ADXPD_GetNumBlk(void* pd);
extern void ADXPD_ExecHndl(void* pd);
extern void ADXPD_GetDly(void* pd, void* out1, void* out2);
extern void ADXPD_SetDly(void* pd, void* in1, void* in2);
extern void ADXPD_GetExtPrm(void* pd, void* out1, void* out2, void* out3);
extern void ADXPD_SetExtPrm(void* pd, s16 v1, s16 v2, s16 v3);

extern s32 ADX_DecodeInfo(void* data, s32 size, void* out_encoding,
    void* out_version, void* out_channelCount, void* out_sampleRate,
    void* out_blockSize, void* out_numChannels, void* out_totalSamples);
extern int ADXB_CheckSpsd(void* data);
extern int ADXB_CheckWav(void* data);
extern int ADXB_CheckAiff(void* data);
extern int ADXB_CheckAu(void* data);
extern void ADXB_DecodeHeaderSpsd(void* adxb, void* data, void* out_encoding);
extern void ADXB_DecodeHeaderWav(void* adxb, void* data, void* out_encoding);
extern void ADXB_DecodeHeaderAiff(void* adxb, void* data, void* out_encoding);
extern void ADXB_DecodeHeaderAu(void* adxb, void* data, void* out_encoding);
extern void ADXB_ExecOneAhx(void* adxb);
extern void ADXB_ExecOneSpsd(void* adxb);
extern void ADXB_ExecOneAiff(void* adxb);
extern void ADXB_ExecOneAu(void* adxb);
extern void ADXB_ExecOneWav(void* adxb);
extern void ADXCRS_Lock(void);
extern void ADXCRS_Unlock(void);

extern void CRICRW_Sprintf(char* buf, int size, const char* fmt, ...);

extern u32 lbl_eu_805E5370;
extern volatile u32 lbl_eu_805E5358;
extern u8 lbl_eu_805E5378[0x1000];
extern u32 lbl_eu_80560048;
extern s16 lbl_eu_80516B30[];

extern char lbl_eu_80517330[];
extern void ADXERR_CallErrFunc1_(const char* msg);
extern void ADXERR_CallErrFunc2_(const char* msg, const char* detail);

extern void (*lbl_eu_805E5364)(void*);
extern void (*lbl_eu_805E5360)(void*, s16, void*, void*);

/* Forward declarations */
int adxb_get_key(void* adxb, void* data, void* out_key1, void* out_key2, void* out_key3);
s32 ADXB_DecodeHeaderAdx(void* adxb, void* data, void* out_encoding);

void SKG_GenerateKey(u8* data, int len, s16* key1, s16* key2, s16* key3) {
    int i;
    s16 x;

    if (*(volatile u32*)&lbl_eu_805E5358 == 0) {
        *(volatile u32*)&lbl_eu_805E5358 = 1;
    }

    *key1 = 0;
    *key2 = 0;
    *key3 = 0;

    if (data == NULL && len <= 0) {
        return;
    }

    /* Pass 1: seed = 0x4A1D */
    x = 0x4A1D;
    for (i = 0; i < len; i++) {
        s16 mul = (s16)(x * lbl_eu_80516B30[0x80 + (s8)data[i]]);
        x = lbl_eu_80516B30[0x80 + (s16)((s16)((mul << 10) - (mul >> 21)) + (mul >> 21))];
    }
    *key1 = x;

    /* Pass 2: seed = 0x53FF */
    x = 0x53FF;
    for (i = 0; i < len; i++) {
        s16 mul = (s16)(x * lbl_eu_80516B30[0x80 + (s8)data[i]]);
        x = lbl_eu_80516B30[0x80 + (s16)((s16)((mul << 10) - (mul >> 21)) + (mul >> 21))];
    }
    *key2 = x;

    /* Pass 3: seed = 0x5DC1 */
    x = 0x5DC1;
    for (i = 0; i < len; i++) {
        s16 mul = (s16)(x * lbl_eu_80516B30[0x80 + (s8)data[i]]);
        x = lbl_eu_80516B30[0x80 + (s16)((s16)((mul << 10) - (mul >> 21)) + (mul >> 21))];
    }
    *key3 = x;
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

    for (i = 0; i < 16; i++) {
        if (*(s16*)(lbl_eu_805E5378 + i * 0x100) == 0) break;
    }
    if (i == 16) return NULL;

    slot = lbl_eu_805E5378 + i * 0x100;
    memset(slot, 0, 0x100);
    *(u16*)(slot + 0x00) = 1;

    {
        void* pd = ADXPD_Create();
        *(void**)(slot + 0x08) = pd;
        if (pd == NULL) {
            memset(slot, 0, 0x100);
            *(u16*)(slot + 0x00) = 0;
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
    s16* k1 = (s16*)out_key1;
    s16* k2 = (s16*)out_key2;
    s16* k3 = (s16*)out_key3;
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
    u8* d = (u8*)data;
    u8 encodingType;
    u32 totalSamples;
    s16 highPassVal;

    if (((u32)data & 1) != 0 && lbl_eu_805E5370 == 0) {
        ADXERR_CallErrFunc2_(lbl_eu_80517330 + 0x1F, lbl_eu_80517330 + 0x1F);
        return -1;
    }

    *(u16*)(p + 0x02) = 1;

    if (ADX_DecodeInfo(data, (s32)out_encoding,
            p + 0x0C, p + 0x0D, p + 0x0F, p + 0x0E,
            p + 0x14, p + 0x18, &highPassVal) < 0) {
        return 0;
    }

    encodingType = p[0x0C];
    if (encodingType > 4) {
        if (*(u32*)(p + 0xB8) == 0) {
            ADXERR_CallErrFunc2_(lbl_eu_80517330 + 0x3E, lbl_eu_80517330 + 0x5E);
            return -1;
        }
    }

    *(u32*)(p + 0x18) = (d[8] << 24) | (d[9] << 16) | (d[10] << 8) | d[11];
    *(s16*)(p + 0x02) = (s16)((d[0x0C] << 8) | d[0x0D]);
    p[0x0C] = d[4];
    p[5] = d[5];
    p[6] = d[6];
    *(u32*)(p + 0x10) = (d[7] << 8) | d[8];
    p[0x0F] = d[0x0F];
    p[0x10] = d[0x10];

    if (p[0x0F] > 4) {
        if (p[0x10] != 0) {
            *(u32*)(p + 0x28) = (d[0x11] << 24) | (d[0x12] << 16) | (d[0x13] << 8) | d[0x14];
            *(u32*)(p + 0x30) = (d[0x15] << 24) | (d[0x16] << 16) | (d[0x17] << 8) | d[0x18];
            *(u16*)(p + 0x24) = (s16)(s8)d[0x19];
        }
    }

    adxb_get_key(adxb, *(void**)(p + 0x38), p + 0xA0, p + 0xA2, p + 0xA4);
    *(u32*)(p + 0xB8) = 1;
    *(u32*)(p + 0x8C) = 0;
    *(u32*)(p + 0x88) = 0;
    *(u32*)(p + 0x04) = 0;

    return 1;
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
    u8* d = (u8*)data;
    u16 magic;
    s32 result;

    if (d == NULL) return -1;

    magic = (u16)((d[0] << 8) | d[1]);
    result = -1;

    if (magic == 0x8000) {
        result = ADXB_DecodeHeaderAdx(adxb, data, out_encoding);
    } else if (ADXB_CheckSpsd(data)) {
        ADXB_DecodeHeaderSpsd(adxb, data, out_encoding);
        result = 0;
    } else if (ADXB_CheckWav(data)) {
        ADXB_DecodeHeaderWav(adxb, data, out_encoding);
        result = 0;
    } else if (ADXB_CheckAiff(data)) {
        ADXB_DecodeHeaderAiff(adxb, data, out_encoding);
        result = 0;
    } else if (ADXB_CheckAu(data)) {
        ADXB_DecodeHeaderAu(adxb, data, out_encoding);
        result = 0;
    }
    return result;
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
    return (s8)*(u8*)((u8*)self + 0x0D);
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
        int ch = (s8)*(u8*)((u8*)self + 0x0F);
        *(u32*)((u8*)self + 0x74) = 0;
        *(u32*)((u8*)self + 0x4C) = size / ch;
    } else {
        *(void**)((u8*)self + 0x48) = data;
        int t = (s8)*(u8*)((u8*)self + 0x0D) / 8;
        *(u32*)((u8*)self + 0x74) = 0;
        *(u32*)((u8*)self + 0x4C) = size / (t * (s8)*(u8*)((u8*)self + 0x0E));
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
    u8* p = (u8*)adxb;
    u32 blkSize = *(u32*)(p + 0x58);
    u32 wrPos = *(u32*)(p + 0x70);
    u32 pcmOfst = *(u32*)(p + 0x68);
    u32 pcmBufSize = *(u32*)(p + 0x60);
    u32 blkSmpl = *(u32*)(p + 0x50);
    u32 maxBlks = *(u32*)(p + 0x4C);
    u32 numBlks;
    u32 endAdj;
    u32 decodeEnd;
    u32 pcmEnd;
    u32 availWrPos;
    u32 availBlks;
    void* pd;

    u32 tmp = (wrPos + blkSize) - 1;
    numBlks = tmp / blkSize;
    endAdj = tmp - (numBlks * blkSize);
    endAdj = (blkSize - 1) - endAdj;

    pcmEnd = (pcmBufSize - pcmOfst) + blkSize - 1;
    decodeEnd = pcmEnd / blkSize;

    if (numBlks < decodeEnd) {
        u32 check = pcmOfst + (numBlks * blkSize) - endAdj;
        if (check < pcmBufSize) {
            decodeEnd = numBlks + 1;
        }
    }

    availWrPos = *(u32*)(p + 0x6C);
    if (wrPos < availWrPos) {
        availWrPos = availWrPos + endAdj;
    }
    availBlks = availWrPos / blkSize;

    maxBlks = maxBlks / blkSmpl;
    if (maxBlks > availBlks) maxBlks = availBlks;
    if (maxBlks > numBlks) maxBlks = numBlks;
    if (maxBlks > decodeEnd) maxBlks = decodeEnd;

    pd = *(void**)(p + 0x08);
    if (blkSmpl == 2) {
        ADXPD_EntrySte(pd, *(void**)(p + 0x48),
            (void*)(*(u32*)(p + 0x5C) + (pcmOfst << 1)),
            (void*)(*(u32*)(p + 0x5C) + (*(u32*)(p + 0x64) << 1)));
        ADXPD_Start(pd);
    } else if (*(u32*)(p + 0xE0) != 0) {
        ADXPD_EntryPl2(pd, *(void**)(p + 0x48),
            (void*)(*(u32*)(p + 0x5C) + (pcmOfst << 1)),
            (void*)(*(u32*)(p + 0x5C) + (*(u32*)(p + 0x64) << 1)));
        ADXPD_Start(pd);
    } else {
        ADXPD_EntryMono(pd, *(void**)(p + 0x48),
            (void*)(*(u32*)(p + 0x5C) + (pcmOfst << 1)),
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
    u32 pcmBufSize = *(u32*)(p + 0x40);
    u32 workBufSize = *(u32*)(p + 0x44);
    s32 tmp;
    s32 numBlks;
    s32 endAdj;
    s32 numBlkPD;
    s32 decSmpl;
    s32 decDtLen;
    u32 pcmEnd;
    s32 samplesLeft;
    int i;

    tmp = (wrPos + blkSize) - 1;
    numBlks = tmp / blkSize;
    endAdj = tmp - (numBlks * blkSize);
    endAdj = (blkSize - 1) - endAdj;

    numBlkPD = ADXPD_GetNumBlk(*(void**)(p + 0x08));
    decSmpl = (numBlkPD * blkSize) / numBlkSmpl;
    if ((u32)(numBlks * (s32)(*(u32*)(p + 0x50))) <= (u32)numBlkPD) {
        decSmpl -= endAdj;
    }
    decDtLen = numBlkPD * numBlkSmpl;

    *(u32*)(p + 0x90) = (u32)decSmpl;
    *(u32*)(p + 0x94) = (u32)decDtLen;

    pcmEnd = pcmOfst + (u32)decSmpl;
    if (pcmEnd < pcmBufSize) return;

    samplesLeft = (s32)(pcmEnd - pcmBufSize);

    if (*(s32*)(p + 0x50) == 2 || *(s32*)(p + 0xE0) != 0) {
        for (i = 0; i < samplesLeft; i++) {
            ((s16*)pcmBase)[i] = ((s16*)(pcmBase + (pcmBufSize << 1)))[i];
        }
        for (i = 0; i < samplesLeft; i++) {
            ((s16*)(pcmBase + (workBufSize << 1)))[i] =
                ((s16*)(pcmBase + ((workBufSize + pcmBufSize) << 1)))[i];
        }
    } else {
        for (i = 0; i < samplesLeft; i++) {
            ((s16*)pcmBase)[i] = ((s16*)(pcmBase + (pcmBufSize << 1)))[i];
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
                for (i = 0; (u32)i < (*(u32*)((u8*)pd2 + 0x10) << 5); i++) {
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
        void (*wrCb)(void*, u32, u32, u32);
        wrCb = *(void (**)(void*, u32, u32, u32))(p + 0xF8);
        if (wrCb != NULL) {
            u32 dataLen = *(u32*)(p + 0x94);
            u32 decodedSmpl = *(u32*)(p + 0x90);
            s8 chan = (s8)*(u8*)(p + 0x0E);
            u32 prevLen = *(u32*)(p + 0xF0);
            u32 diff;
            void* ctx;
            diff = dataLen - prevLen;
            ctx = *(void**)(p + 0xFC);
            wrCb(ctx, diff, decodedSmpl * (chan << 1), decodedSmpl * (chan << 1));
            *(u32*)(p + 0xF0) = *(u32*)(p + 0x94);
        }
    }
}
