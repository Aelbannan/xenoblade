// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_bsc

#include <harness_catalog.h>
#include "libs/CriWare/src/adx/adxt/adx_bsc.h"
#include "libs/CriWare/src/adx/adxt/adx_xpnd.h"

u32 lbl_eu_805E5348;
u32 lbl_eu_805E534C;
u32 lbl_eu_805E5350;
u32 lbl_eu_805E5354;
u32 lbl_eu_805E5358;
void (*lbl_eu_805E535C)(void*, void*);
void (*lbl_eu_805E5360)(void*, s16, void*, void*);
void (*lbl_eu_805E5364)(void*);
unsigned char _pad_805E5368[8];
u32 lbl_eu_805E5370[2];
u8 lbl_eu_805E5378[0xFF8];
__declspec(section ".data") __attribute__((aligned(8))) u32 lbl_eu_80560048[2] = {1,0};

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

extern s16 lbl_eu_80516B30[];

extern char lbl_eu_80517330[];
extern void ADXERR_CallErrFunc1_(const char* msg);
extern void ADXERR_CallErrFunc2_(const char* msg, const char* detail);


/* Forward declarations */
int adxb_get_key(ADXB* adxb, void* data, void* out_key1, void* out_key2, void* out_key3);
s32 ADXB_DecodeHeaderAdx(ADXB* adxb, void* data, void* out_encoding);

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

u32 ADXB_GetDecErrMode(void) { return lbl_eu_805E5370[0]; }

void ADXB_Init(void) {
    u8* p = (u8*)&lbl_eu_805E5358;
    ADXPD_Init();
    (*(volatile u32*)p)++;
    memset(p + 0x20, 0, 0x1000);
    *(u32*)(p + 0x18) = 0;
}

u32 adxb_DefGetWr(ADXB* self, u32* out1, u32* out2, u32* out3) {
    *out1 = self->decPos;
    *out2 = (u32)self->workBuf - self->decPos;
    *out3 = self->totalSmpl - self->readSmpl;
    return (u32)self->pcmBuf;
}

void adxb_DefAddWr(ADXB* self, u32 unused, u32 addend) {
    self->decPos += addend;
    self->readSmpl += addend;
}

void* ADXB_Create(void* arg0, void* pcmBuf, void* workBuf, void* workSize) {
    int i;
    ADXB* slot;
    ADXB* p;

    p = (ADXB*)lbl_eu_805E5378;
    for (i = 0; i < 16; i++) {
        if (p->state == 0) break;
        p++;
    }
    if (i == 16) return NULL;

    slot = (ADXB*)lbl_eu_805E5378 + i;
    memset(slot, 0, 0x100);
    slot->state = 1;

    {
        void* pd = ADXPD_Create();
        slot->pd = pd;
        if (pd == NULL) {
            if (slot != NULL) {
                slot->pd = NULL;
                ADXPD_Destroy(pd);
                memset(slot, 0, 0x100);
                slot->state = 0;
            }
            return NULL;
        }
    }

    slot->ctx = arg0;
    slot->pcmBuf = pcmBuf;
    slot->workBuf = workBuf;
    slot->workSize = (u32)workSize;
    slot->getWr = (void*)adxb_DefGetWr;
    slot->getWrCtx = slot;
    slot->endCb = (void*)adxb_DefAddWr;
    slot->endCbCtx = slot;
    slot->ainfLen = 0;
    slot->defOutVol = 0;
    slot->defPan[0] = -0x80;
    slot->defPan[1] = -0x80;
    memset(&slot->loopTbl[0], 0, 0x10);
    return slot;
}

void ADXB_Destroy(ADXB* self) {
    void* pd;
    if (self == NULL) return;
    pd = self->pd;
    self->pd = NULL;
    ADXPD_Destroy(pd);
    memset(self, 0, 0x100);
    self->state = 0;
}

int adxb_get_key(ADXB* adxb, void* data, void* out_key1, void* out_key2, void* out_key3) {
    ADXB* p = (ADXB*)adxb;
    s16* k3 = (s16*)out_key3;
    s16* k2 = (s16*)out_key2;
    s16* k1 = (s16*)out_key1;
    s16* defKeys = (s16*)((u8*)&lbl_eu_805E5358 + 0x10);

    if (lbl_eu_80560048[0] == 0) {
        if (p->version < 4) {
            *k1 = 0;
            *k2 = 0;
            *k3 = 0;
        } else if (p->flags >= 0x10) {
            char buf[16];
            CRICRW_Sprintf(buf, 16, lbl_eu_80517330 + 0xA7, data);
            SKG_GenerateKey((u8*)buf, 8, k1, k2, k3);
        } else if (p->flags >= 8) {
            if (p->key[0] == 0 && p->key[1] == 0 && p->key[2] == 0) {
                p->key[0] = defKeys[0];
                p->key[1] = defKeys[1];
                p->key[2] = defKeys[2];
            }
            *k1 = p->key[0];
            *k2 = p->key[1];
            *k3 = p->key[2];
        } else {
            *k1 = 0;
            *k2 = 0;
            *k3 = 0;
        }
    } else {
        if (p->version < 4) {
            if (p->key[0] == 0 && p->key[1] == 0 && p->key[2] == 0 &&
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
            if (p->key[0] == 0 && p->key[1] == 0 && p->key[2] == 0) {
                p->key[0] = defKeys[0];
                p->key[1] = defKeys[1];
                p->key[2] = defKeys[2];
            }
            *k1 = p->key[0];
            *k2 = p->key[1];
            *k3 = p->key[2];
        }
    }
    return 0;
}

/* Local declaration order controls MWCC's two-pass stack-slot packing
 * (small scalars backward first, then aggregates/words backward); this
 * order reproduces the retail frame exactly. key[0] is the zero-init
 * scratch passed to the 0x805E535C callback; key[1..3] take the keys. */
s32 ADXB_DecodeHeaderAdx(ADXB* adxb, void* data, void* out_encoding) {
    ADXB* p = (ADXB*)adxb;
    s16 key[4];
    u32 dly1;
    u32 dly2;
    s16 hp;
    s16 l1;
    s16 l2;
    s16 l3;

    if (((u32)data & 1) != 0 && lbl_eu_805E5370[0] == 0) {
        ADXERR_CallErrFunc2_(lbl_eu_80517330, lbl_eu_80517330 + 0x1F);
        return -1;
    }

    p->field_0x02 = 1;

    if (ADX_DecodeInfo(data, (s32)out_encoding, &hp,
            &p->encoding, &p->bitPerSmpl, &p->blkByte, &p->numChan,
            &p->sfreq, &p->totalSmpl, &p->blkSmpl) < 0) {
        return 0;
    }

    if (p->encoding > 4) {
        if (p->ainf == NULL) {
            ADXERR_CallErrFunc2_(lbl_eu_80517330 + 0x3E, lbl_eu_80517330 + 0x5E);
            return -1;
        }

        p->bitPerSmpl = 8;
        p->blkByte = p->numChan * 192;
        p->blkSmpl = 96;
        p->fmt = 10;
        p->coefPos = 0;
        p->numLoop = 0;
        p->field_0x26 = 0;
        p->field_0x20 = 0;
        p->lpStartPos = 0;
        p->lpStartOfst = 0;
        p->lpEndPos = 0;
        p->lpEndOfst = 0;
        p->readSmpl = 0;

        if (ADX_DecodeInfoExVer(data, (s32)out_encoding, &p->version, &p->flags) < 0) {
            return 0;
        }

        key[0] = 0;

        if (adxb_get_key(adxb, (void*)p->totalSmpl, &key[1], &key[2], &key[3]) < 0) {
            return -1;
        }

        if (lbl_eu_805E535C != NULL) {
            lbl_eu_805E535C(p->ainf, &key[0]);
        }
    } else {
        if (ADX_DecodeInfoExVer(data, (s32)out_encoding, &p->version, &p->flags) < 0) {
            return 0;
        }

        if (adxb_get_key(adxb, (void*)p->totalSmpl, &l1, &l2, &l3) < 0) {
            return -1;
        }

        ADXPD_SetExtPrm(p->pd, l1, l2, l3);

        if (ADX_DecodeInfoExADPCM2(data, (s32)out_encoding, &p->coefPos) < 0) {
            return 0;
        }

        if (ADX_DecodeInfoExIdly(data, (s32)out_encoding, &dly1, &dly2) < 0) {
            return 0;
        }

        ADXPD_SetCoef(p->pd, p->sfreq, p->coefPos);
        ADXPD_SetDly(p->pd, &dly1, &dly2);
        /* Loop/ainf header info results are intentionally unchecked. */
        ADX_DecodeInfoExLoop(data, (s32)out_encoding,
            &p->field_0x20, &p->numLoop, &p->field_0x26, &p->lpStartPos,
            &p->lpStartOfst, &p->lpEndPos, &p->lpEndOfst);
        ADX_DecodeInfoAinf(data, (s32)out_encoding,
            &p->ainfLen, &p->loopTbl[0], &p->defOutVol, &p->defPan[0]);
        p->fmt = 0;
    }

    p->blkSmplW = p->numChan;
    p->blkSmplP = p->blkByte;
    p->blkByteW = p->blkSmpl;
    p->pcmBase = (u32)p->pcmBuf;
    p->pcmBufSize = (u32)p->workBuf;
    p->chByte = p->workSize;
    p->decPos = 0;

    return hp;
}

void ADXB_SetDefFmt(ADXB* self) {
    self->fmt = 0;
}

void ADXB_SetDefPrm(ADXB* self);
void criware_eu_8038A864(ADXB* self) {
    self->fmt = self->defFmt;
    ADXB_SetDefPrm(self);
}

void ADXB_SetDefPrm(ADXB* self) {
    ADXB* p = (ADXB*)self;

    p->field_0x02 = 1;
    p->sfreq = 48000;
    p->numChan = 2;
    p->bitPerSmpl = 16;
    p->totalSmpl = 0x7fffffff;
    p->blkByte = 0x7f;
    p->blkSmpl = 0x400;
    p->blkSmplW = 2;
    p->blkSmplP = 0x7f;
    p->blkByteW = 0x400;
    p->pcmBase = (u32)p->pcmBuf;
    p->pcmBufSize = (u32)p->workBuf;
    p->chByte = p->workSize;
    p->decPos = 0;
    p->coefPos = 0;
    p->numLoop = 0;
    p->field_0x26 = 0;
    p->field_0x20 = 0;
    p->lpStartPos = 0;
    p->lpStartOfst = 0;
    p->lpEndPos = 0;
    p->lpEndOfst = 0;
    p->readSmpl = 0;
    p->version = 0;
    p->flags = 0;
}

s32 ADXB_DecodeHeader(ADXB* adxb, void* data, void* out_encoding) {
    ADXB* p = (ADXB*)adxb;
    u8* d = (u8*)data;
    u16 magic;

    p->ainfLen = 0;
    p->defOutVol = 0;
    p->defPan[0] = -128;
    p->defPan[1] = -128;
    memset(&p->loopTbl[0], 0, 16);

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

void ADXB_EntryGetWrFunc(ADXB* self, void* func, void* ctx) {
    self->getWr = func;
    self->getWrCtx = ctx;
}

u32 ADXB_GetPcmBuf(ADXB* self) { return (u32)self->pcmBuf; }

s16 ADXB_GetFormat(ADXB* self) { return self->fmt; }

u32 ADXB_GetSfreq(ADXB* self) { return self->sfreq; }

s32 ADXB_GetNumChan(ADXB* self) {
    int ch = (u8)self->numChan;
    if (ch == 1 && self->field_0xe0 != 0)
        return 2;
    if (self == NULL) {
        ADXERR_CallErrFunc1_(lbl_eu_80517330 + 0x81);
        return -1;
    }
    return (s8)ch;
}

s16 ADXB_GetFmtBps(ADXB* self) {
    return self->bitPerSmpl;
}

s32 ADXB_GetOutBps(ADXB* self) {
    s16 val = self->fmt;
    if (val == 0) return 0x10;
    if (val == 2) {
        s16 sub = self->fmtSub;
        if (sub == 2) return 4;
        if (sub == 1) return 8;
        return 0x10;
    }
    if (val == 1) {
        s16 sub = self->fmtSub;
        if (sub == 2) return 4;
        return 0x10;
    }
    return 0x10;
}

u32 ADXB_GetBlkSmpl(ADXB* self) { return self->blkSmpl; }

u32 ADXB_GetTotalNumSmpl(ADXB* self) { return self->totalSmpl; }

s16 ADXB_GetNumLoop(ADXB* self) { return self->numLoop; }

u32 ADXB_GetLpStartPos(ADXB* self) { return self->lpStartPos; }

u32 ADXB_GetLpStartOfst(ADXB* self) {
    if (self == NULL) return 0;
    return self->lpStartOfst;
}

u32 ADXB_GetLpEndPos(ADXB* self) { return self->lpEndPos; }

u32 ADXB_GetLpEndOfst(ADXB* self) { return self->lpEndOfst; }

u32 ADXB_GetAinfLen(ADXB* self) { return self->ainfLen; }

s16 ADXB_GetDefOutVol(ADXB* self) { return self->defOutVol; }

s16 ADXB_GetDefPan(ADXB* self, u32 idx) {
    return self->defPan[idx];
}

void ADXB_TakeSnapshot(ADXB* self) {
    ADXPD_GetDly(self->pd, &self->dly[0], &self->dly2[0]);
    ADXPD_GetExtPrm(self->pd, &self->extPrm[0], &self->extPrm[1], &self->extPrm[2]);
}

void ADXB_RestoreSnapshot(ADXB* self) {
    ADXPD_SetDly(self->pd, (u32*)&self->dly[0], (u32*)&self->dly2[0]);
    ADXPD_SetExtPrm(self->pd,
        self->extPrm[0],
        self->extPrm[1],
        self->extPrm[2]);
}

void ADXB_SetLnkSw(ADXB* self, int val) {
    if (val == 0) {
        self->lnkSw = 0;
    } else {
        self->lnkSw = 1;
    }
}

u32 ADXB_GetStat(ADXB* self) { return self->stat; }

void ADXB_EntryData(ADXB* self, void* data, int size) {
    ADXB* p = (ADXB*)self;
    if (p->fmt == 0) {
        s32 q;
        p->data = data;
        q = size / p->blkByte;
        p->field_0x74 = 0;
        p->maxBlks = q;
    } else {
        s32 t = p->bitPerSmpl;
        s32 ch = p->numChan;
        s32 q = size / ((t / 8) * ch);
        p->data = data;
        p->field_0x74 = 0;
        p->maxBlks = q;
    }
    p->decNumSmpl = 0;
    p->decDtLen = 0;
    p->field_0xf4 = 0;
    p->prevDecDtLen = 0;
}

void ADXB_Start(ADXB* self) {
    if (self->stat != 0) return;
    self->stat = 1;
}

void ADXB_Stop(ADXB* self) {
    if (self->field_0xe0 != 0) {
        lbl_eu_805E5364(self);
    }
    ADXPD_Stop(self->pd);
    self->stat = 0;
}

void ADXB_Reset(ADXB* self) {
    if ((int)self->stat == 3) {
        ADXPD_Reset(self->pd);
        self->decPos = 0;
        self->stat = 0;
    }
}

u32 ADXB_GetDecDtLen(ADXB* self) { return self->decDtLen; }

u32 ADXB_GetDecNumSmpl(ADXB* self) { return self->decNumSmpl; }

void ADXB_EvokeDecode(ADXB* adxb) {
    s32 blkSize, wrPos, pcmOfst, pcmBufSize, blkSmpl, maxBlks, availWrPos;
    s32 tmp, numBlks, endAdj, decodeEnd, deSize, availBlks;

    blkSize = adxb->blkByteW;
    wrPos = adxb->wrPos;
    pcmOfst = adxb->pcmOfst;
    pcmBufSize = adxb->pcmBufSize;
    blkSmpl = adxb->blkSmplW;
    maxBlks = (s32)adxb->maxBlks;
    availWrPos = adxb->maxWrPos;
    numBlks = (wrPos + blkSize - 1) / blkSize;
    endAdj = (blkSize - 1) - ((wrPos + blkSize - 1) % blkSize);
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
        void* pd = adxb->pd;
        ADXPD_EntrySte(pd, adxb->data, maxBlks << 1,
            (void*)(adxb->pcmBase + (adxb->pcmOfst << 1)),
            (void*)(adxb->pcmBase + (adxb->pcmOfst << 1) + (adxb->chByte << 1)));
        ADXPD_Start(pd);
    } else if ((s32)adxb->field_0xe0 != 0) {
        void* pd = adxb->pd;
        ADXPD_EntryPl2(pd, adxb->data, maxBlks,
            (void*)(adxb->pcmBase + (adxb->pcmOfst << 1)),
            (void*)(adxb->pcmBase + (adxb->pcmOfst << 1) + (adxb->chByte << 1)));
        ADXPD_Start(pd);
    } else {
        void* pd = adxb->pd;
        ADXPD_EntryMono(pd, adxb->data,
            (void*)(adxb->pcmBase + (adxb->pcmOfst << 1)),
            NULL);
        ADXPD_Start(pd);
    }
}

void ADXB_EndDecode(ADXB* adxb) {
    ADXB* p;
    s32 blkSize, wrPos, numBlkSmpl;
    u32 pcmOfst, pcmBase;
    s32 pcmBufSize;
    u32 workBufSize;
    s32 tmp, endAdj2, numBlks, numBlkPD, decSmpl, decDtLen, pcmEnd, samplesLeft;
    int i;

    p = adxb;
    blkSize = p->blkByteW;
    wrPos = p->wrPos;
    numBlkSmpl = p->blkSmplP;
    pcmOfst = (u32)p->pcmOfst;
    pcmBase = p->pcmBase;
    pcmBufSize = (s32)p->workBuf;
    workBufSize = p->workSize;

    tmp = wrPos + blkSize - 1;
    endAdj2 = (blkSize - 1) - (tmp % blkSize);
    numBlks = tmp / blkSize;

    numBlkPD = ADXPD_GetNumBlk(p->pd);
    decSmpl = (numBlkPD * blkSize) / numBlkSmpl;
    if (numBlks * (s32)(u32)p->blkSmplW <= numBlkPD) {
        decSmpl -= endAdj2;
    }
    decDtLen = numBlkPD * numBlkSmpl;

    p->decNumSmpl = (u32)decSmpl;
    p->decDtLen = (u32)decDtLen;

    pcmEnd = pcmOfst + decSmpl;
    if (pcmEnd < pcmBufSize) return;

    samplesLeft = pcmEnd - pcmBufSize;

    if (p->blkSmplW == 2 || (s32)p->field_0xe0 != 0) {
        {
            s16* d = (s16*)pcmBase;
            const s16* s = (const s16*)(pcmBase + (pcmBufSize << 1));
            int i;
            for (i = samplesLeft; i > 0; i--) {
                *d++ = *s++;
            }
        }
        {
            s16* d = (s16*)(pcmBase + (workBufSize << 1));
            const s16* s = (const s16*)(pcmBase + ((workBufSize + pcmBufSize) << 1));
            int i;
            for (i = samplesLeft; i > 0; i--) {
                *d++ = *s++;
            }
        }
    } else {
        s16* d = (s16*)pcmBase;
        const s16* s = (const s16*)(pcmBase + (pcmBufSize << 1));
        int i;
        for (i = samplesLeft; i > 0; i--) {
            *d++ = *s++;
        }
    }
}

void ADXB_ExecOneAdx(ADXB* adxb) {
    ADXB* p = (ADXB*)adxb;

    if ((int)p->stat == 1) {
        if (ADXPD_GetStat(p->pd) == 0) {
            void (*getWr)(void*, u32*, u32*, u32*);
            void* ctx;

            getWr = (void (*)(void*, u32*, u32*, u32*))p->getWr;
            ctx = p->getWrCtx;
            getWr(ctx, (u32*)&p->pcmOfst, (u32*)&p->maxWrPos, (u32*)&p->wrPos);
            ADXB_EvokeDecode(adxb);
            p->stat = 2;
        }
    }

    if ((int)p->stat == 2) {
        ADXPD_ExecHndl(p->pd);
        if (ADXPD_GetStat(p->pd) == 3) {
            if ((int)p->field_0xe0 != 0) {
                int i;
                ADXPD* pd2 = p->pd;
                ADXCRS_Lock();
                for (i = 0; i < (int)(pd2->numBlk << 5); i++) {
                    s16* buf1 = pd2->out1;
                    s16* buf2 = pd2->out2;
                    lbl_eu_805E5360(adxb,
                        buf1[i],
                        (u8*)buf1 + (i << 1), (u8*)buf2 + (i << 1));
                }
                ADXCRS_Unlock();
            }
            ADXB_EndDecode(adxb);
            ADXPD_Reset(p->pd);
            {
                void (*eosCb)(void*, u32, u32);
                void* eosCtx;
                eosCb = (void (*)(void*, u32, u32))p->endCb;
                eosCtx = p->endCbCtx;
                eosCb(eosCtx, p->decDtLen, p->decNumSmpl);
            }
            p->stat = 3;
        }
    }
}

void ADXB_ExecHndl(ADXB* adxb) {
    ADXB* p = (ADXB*)adxb;
    s16 fmt = p->fmt;

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
        wrCb = (void (*)(void*, s32, s32))p->wrCb;
        if (wrCb != NULL) {
            s32 decodedSmpl, dataLen, prevLen, diff;
            prevLen = (s32)p->prevDecDtLen;
            dataLen = (s32)p->decDtLen;
            decodedSmpl = (s32)p->decNumSmpl;
            diff = dataLen - prevLen;
            if (diff < 0) {
                diff = (0x7FFFFFFF - prevLen) + dataLen;
            }
            wrCb(p->wrCbCtx, diff, p->numChan * (decodedSmpl << 1));
            p->prevDecDtLen = p->decDtLen;
        }
    }
}