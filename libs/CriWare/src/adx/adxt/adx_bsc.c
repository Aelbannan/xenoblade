// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_bsc

#include <harness_catalog.h>
#include "libs/CriWare/src/adx/adxt/adx_bsc.h"
#include "libs/CriWare/src/adx/adxt/adx_xpnd.h"

/* AHX callback pointers at 0x5348..0x5354 now belong to adx_bahx. */
/* adxb_ahx_in_sj / dec_smpl / term_supply / exec_one are imported there. */
/* No local storage here; keep these four lines for stable source lines. */
/* (The callback pool starts at skg_init_count/ahxsetextfunc.)          */
static u32 skg_init_count;
extern void (*lbl_eu_805E535C)(void*, void*);
extern void (*pl2encodefunc)(void*, s16, void*, void*);
extern void (*pl2resetfunc)(void*);
extern unsigned char _pad_805E5368[8];
extern u32 adxb_err_mode[2];
extern u8 adxb_obj[0x1000];
__declspec(section ".data") __attribute__((aligned(8))) u32 adxb_force_flag[2] = {1,0};


extern void ADXPD_Init(void);
extern void* ADXPD_Create(void);

extern void ADXPD_Destroy(void* pd);

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

extern const s16 ahx_key_table[];

extern const char adxbsc_error_messages[];
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

    if (skg_init_count == 0) {
        skg_init_count = skg_init_count + 1;
    }

    *key1 = 0;
    *key2 = 0;
    *key3 = 0;

    if (data == NULL && len <= 0) {
        return 0;
    }

    x1 = 0x4A1D;
    for (i = 0; i < len; i++) {
        u32 m = (u32)((s32)x1 * (s32)ahx_key_table[0x80 + (s8)data[i]]);
        u32 b = m << 22;
        u32 s = m >> 31;
        u32 t = (b - s);
        t = (t << 10) | (t >> 22);
        t = t + s;
        x1 = ahx_key_table[t];
    }

    x2 = 0x53FF;
    for (i = 0; i < len; i++) {
        u32 m = (u32)((s32)x2 * (s32)ahx_key_table[0x80 + (s8)data[i]]);
        u32 b = m << 22;
        u32 s = m >> 31;
        u32 t = (b - s);
        t = (t << 10) | (t >> 22);
        t = t + s;
        x2 = ahx_key_table[t];
    }

    x3 = 0x5DC1;
    for (i = 0; i < len; i++) {
        u32 m = (u32)((s32)x3 * (s32)ahx_key_table[0x80 + (s8)data[i]]);
        u32 b = m << 22;
        u32 s = m >> 31;
        u32 t = (b - s);
        t = (t << 10) | (t >> 22);
        t = t + s;
        x3 = ahx_key_table[t];
    }

    *key1 = x1;
    *key2 = x2;
    *key3 = x3;

    return 0;
}

u32 ADXB_GetDecErrMode(void) { return adxb_err_mode[0]; }

void ADXB_Init(void) {
    u8* p = (u8*)&skg_init_count;
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

    p = (ADXB*)adxb_obj;
    for (i = 0; i < 16; i++) {
        if (p->state == 0) break;
        p++;
    }
    if (i == 16) return NULL;

    slot = (ADXB*)adxb_obj + i;
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
    s16* defKeys = (s16*)((u8*)&skg_init_count + 0x10);

    if (adxb_force_flag[0] == 0) {
        if (p->version < 4) {
            *k1 = 0;
            *k2 = 0;
            *k3 = 0;
        } else if (p->flags >= 0x10) {
            char buf[16];
            CRICRW_Sprintf(buf, 16, adxbsc_error_messages + 0xA7, data);
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
            ADXERR_CallErrFunc1_(adxbsc_error_messages + 0xAC);
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

    if (((u32)data & 1) != 0 && adxb_err_mode[0] == 0) {
        ADXERR_CallErrFunc2_(adxbsc_error_messages, adxbsc_error_messages + 0x1F);
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
            ADXERR_CallErrFunc2_(adxbsc_error_messages + 0x3E, adxbsc_error_messages + 0x5E);
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
        ADXERR_CallErrFunc1_(adxbsc_error_messages + 0x81);
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
        pl2resetfunc(self);
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
                    pl2encodefunc(adxb,
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

/* .bss: definitions after the code, reverse retail order for MWCC emission. */
u8 adxb_obj[0x1000];
u32 adxb_err_mode[2];
unsigned char _pad_805E5368[8];
void (*pl2resetfunc)(void*);
void (*pl2encodefunc)(void*, s16, void*, void*);
void (*lbl_eu_805E535C)(void*, void*);

/* Retail .rodata 0x80516B30..0x80517408: shared AHX key table and
 * ADXB error/version strings, emitted by this TU in link order. */
const s16 ahx_key_table[] = {
    16411, 16417, 16421, 16427, 16433, 16447, 16451, 16453, 16477, 16481, 16487, 16493,
    16519, 16529, 16547, 16553, 16561, 16567, 16573, 16603, 16607, 16619, 16631, 16633,
    16649, 16651, 16657, 16661, 16673, 16691, 16693, 16699, 16703, 16729, 16741, 16747,
    16759, 16763, 16787, 16811, 16823, 16829, 16831, 16843, 16871, 16879, 16883, 16889,
    16901, 16903, 16921, 16927, 16931, 16937, 16943, 16963, 16979, 16981, 16987, 16993,
    17011, 17021, 17027, 17029, 17033, 17041, 17047, 17053, 17077, 17093, 17099, 17107,
    17117, 17123, 17137, 17159, 17167, 17183, 17189, 17191, 17203, 17207, 17209, 17231,
    17239, 17257, 17291, 17293, 17299, 17317, 17321, 17327, 17333, 17341, 17351, 17359,
    17377, 17383, 17387, 17389, 17393, 17401, 17417, 17419, 17431, 17443, 17449, 17467,
    17471, 17477, 17483, 17489, 17491, 17497, 17509, 17519, 17539, 17551, 17569, 17573,
    17579, 17581, 17597, 17599, 17609, 17623, 17627, 17657, 17659, 17669, 17681, 17683,
    17707, 17713, 17729, 17737, 17747, 17749, 17761, 17783, 17789, 17791, 17807, 17827,
    17837, 17839, 17851, 17863, 17881, 17891, 17903, 17909, 17911, 17921, 17923, 17929,
    17939, 17957, 17959, 17971, 17977, 17981, 17987, 17989, 18013, 18041, 18043, 18047,
    18049, 18059, 18061, 18077, 18089, 18097, 18119, 18121, 18127, 18131, 18133, 18143,
    18149, 18169, 18181, 18191, 18199, 18211, 18217, 18223, 18229, 18233, 18251, 18253,
    18257, 18269, 18287, 18289, 18301, 18307, 18311, 18313, 18329, 18341, 18353, 18367,
    18371, 18379, 18397, 18401, 18413, 18427, 18433, 18439, 18443, 18451, 18457, 18461,
    18481, 18493, 18503, 18517, 18521, 18523, 18539, 18541, 18553, 18583, 18587, 18593,
    18617, 18637, 18661, 18671, 18679, 18691, 18701, 18713, 18719, 18731, 18743, 18749,
    18757, 18773, 18787, 18793, 18797, 18803, 18839, 18859, 18869, 18899, 18911, 18913,
    18917, 18919, 18947, 18959, 18973, 18979, 19001, 19009, 19013, 19031, 19037, 19051,
    19069, 19073, 19079, 19081, 19087, 19121, 19139, 19141, 19157, 19163, 19181, 19183,
    19207, 19211, 19213, 19219, 19231, 19237, 19249, 19259, 19267, 19273, 19289, 19301,
    19309, 19319, 19333, 19373, 19379, 19381, 19387, 19391, 19403, 19417, 19421, 19423,
    19427, 19429, 19433, 19441, 19447, 19457, 19463, 19469, 19471, 19477, 19483, 19489,
    19501, 19507, 19531, 19541, 19543, 19553, 19559, 19571, 19577, 19583, 19597, 19603,
    19609, 19661, 19681, 19687, 19697, 19699, 19709, 19717, 19727, 19739, 19751, 19753,
    19759, 19763, 19777, 19793, 19801, 19813, 19819, 19841, 19843, 19853, 19861, 19867,
    19889, 19891, 19913, 19919, 19927, 19937, 19949, 19961, 19963, 19973, 19979, 19991,
    19993, 19997, 20011, 20021, 20023, 20029, 20047, 20051, 20063, 20071, 20089, 20101,
    20107, 20113, 20117, 20123, 20129, 20143, 20147, 20149, 20161, 20173, 20177, 20183,
    20201, 20219, 20231, 20233, 20249, 20261, 20269, 20287, 20297, 20323, 20327, 20333,
    20341, 20347, 20353, 20357, 20359, 20369, 20389, 20393, 20399, 20407, 20411, 20431,
    20441, 20443, 20477, 20479, 20483, 20507, 20509, 20521, 20533, 20543, 20549, 20551,
    20563, 20593, 20599, 20611, 20627, 20639, 20641, 20663, 20681, 20693, 20707, 20717,
    20719, 20731, 20743, 20747, 20749, 20753, 20759, 20771, 20773, 20789, 20807, 20809,
    20849, 20857, 20873, 20879, 20887, 20897, 20899, 20903, 20921, 20929, 20939, 20947,
    20959, 20963, 20981, 20983, 21001, 21011, 21013, 21017, 21019, 21023, 21031, 21059,
    21061, 21067, 21089, 21101, 21107, 21121, 21139, 21143, 21149, 21157, 21163, 21169,
    21179, 21187, 21191, 21193, 21211, 21221, 21227, 21247, 21269, 21277, 21283, 21313,
    21317, 21319, 21323, 21341, 21347, 21377, 21379, 21383, 21391, 21397, 21401, 21407,
    21419, 21433, 21467, 21481, 21487, 21491, 21493, 21499, 21503, 21517, 21521, 21523,
    21529, 21557, 21559, 21563, 21569, 21577, 21587, 21589, 21599, 21601, 21611, 21613,
    21617, 21647, 21649, 21661, 21673, 21683, 21701, 21713, 21727, 21737, 21739, 21751,
    21757, 21767, 21773, 21787, 21799, 21803, 21817, 21821, 21839, 21841, 21851, 21859,
    21863, 21871, 21881, 21893, 21911, 21929, 21937, 21943, 21961, 21977, 21991, 21997,
    22003, 22013, 22027, 22031, 22037, 22039, 22051, 22063, 22067, 22073, 22079, 22091,
    22093, 22109, 22111, 22123, 22129, 22133, 22147, 22153, 22157, 22159, 22171, 22189,
    22193, 22229, 22247, 22259, 22271, 22273, 22277, 22279, 22283, 22291, 22303, 22307,
    22343, 22349, 22367, 22369, 22381, 22391, 22397, 22409, 22433, 22441, 22447, 22453,
    22469, 22481, 22483, 22501, 22511, 22531, 22541, 22543, 22549, 22567, 22571, 22573,
    22613, 22619, 22621, 22637, 22639, 22643, 22651, 22669, 22679, 22691, 22697, 22699,
    22709, 22717, 22721, 22727, 22739, 22741, 22751, 22769, 22777, 22783, 22787, 22807,
    22811, 22817, 22853, 22859, 22861, 22871, 22877, 22901, 22907, 22921, 22937, 22943,
    22961, 22963, 22973, 22993, 23003, 23011, 23017, 23021, 23027, 23029, 23039, 23041,
    23053, 23057, 23059, 23063, 23071, 23081, 23087, 23099, 23117, 23131, 23143, 23159,
    23167, 23173, 23189, 23197, 23201, 23203, 23209, 23227, 23251, 23269, 23279, 23291,
    23293, 23297, 23311, 23321, 23327, 23333, 23339, 23357, 23369, 23371, 23399, 23417,
    23431, 23447, 23459, 23473, 23497, 23509, 23531, 23537, 23539, 23549, 23557, 23561,
    23563, 23567, 23581, 23593, 23599, 23603, 23609, 23623, 23627, 23629, 23633, 23663,
    23669, 23671, 23677, 23687, 23689, 23719, 23741, 23743, 23747, 23753, 23761, 23767,
    23773, 23789, 23801, 23813, 23819, 23827, 23831, 23833, 23857, 23869, 23873, 23879,
    23887, 23893, 23899, 23909, 23911, 23917, 23929, 23957, 23971, 23977, 23981, 23993,
    24001, 24007, 24019, 24023, 24029, 24043, 24049, 24061, 24071, 24077, 24083, 24091,
    24097, 24103, 24107, 24109, 24113, 24121, 24133, 24137, 24151, 24169, 24179, 24181,
    24197, 24203, 24223, 24229, 24239, 24247, 24251, 24281, 24317, 24329, 24337, 24359,
    24371, 24373, 24379, 24391, 24407, 24413, 24419, 24421, 24439, 24443, 24469, 24473,
    24481, 24499, 24509, 24517, 24527, 24533, 24547, 24551, 24571, 24593, 24611, 24623,
    24631, 24659, 24671, 24677, 24683, 24691, 24697, 24709, 24733, 24749, 24763, 24767,
    24781, 24793, 24799, 24809, 24821, 24841, 24847, 24851, 24859, 24877, 24889, 24907,
    24917, 24919, 24923, 24943, 24953, 24967, 24971, 24977, 24979, 24989, 25013, 25031,
    25033, 25037, 25057, 25073, 25087, 25097, 25111, 25117, 25121, 25127, 25147, 25153,
    25163, 25169, 25171, 25183, 25189, 25219, 25229, 25237, 25243, 25247, 25253, 25261,
    25301, 25303, 25307, 25309, 25321, 25339, 25343, 25349, 25357, 25367, 25373, 25391,
    25409, 25411, 25423, 25439, 25447, 25453, 25457, 25463, 25469, 25471, 25523, 25537,
    25541, 25561, 25577, 25579, 25583, 25589, 25601, 25603, 25609, 25621, 25633, 25639,
    25643, 25657, 25667, 25673, 25679, 25693, 25703, 25717, 25733, 25741, 25747, 25759,
    25763, 25771, 25793, 25799, 25801, 25819, 25841, 25847, 25849, 25867, 25873, 25889,
    25903, 25913, 25919, 25931, 25933, 25939, 25943, 25951, 25969, 25981, 25997, 25999,
    26003, 26017, 26021, 26029, 26041, 26053, 26083, 26099, 26107, 26111, 26113, 26119,
    26141, 26153, 26161, 26171, 26177, 26183, 26189, 26203, 26209, 26227, 26237, 26249,
    26251, 26261, 26263, 26267, 26293, 26297, 26309, 26317, 26321, 26339, 26347, 26357,
    26371, 26387, 26393, 26399, 26407, 26417, 26423, 26431, 26437, 26449, 26459, 26479,
    26489, 26497, 26501, 26513, 26539, 26557, 26561, 26573, 26591, 26597, 26627, 26633,
    26641, 26647, 26669, 26681,
};

const char adxbsc_error_messages[0xD8] = {
    0x45,0x30,0x34,0x31,0x30,0x32,0x35,0x30,0x31,0x20,0x61,0x64,0x78,0x73,0x6A,0x64,
    0x5F,0x64,0x65,0x63,0x6F,0x64,0x65,0x5F,0x70,0x72,0x65,0x70,0x3A,0x20,0x00,0x54,
    0x68,0x65,0x20,0x64,0x61,0x74,0x61,0x20,0x61,0x6C,0x69,0x67,0x6E,0x6D,0x65,0x6E,
    0x74,0x20,0x69,0x73,0x20,0x69,0x6C,0x6C,0x65,0x67,0x61,0x6C,0x2E,0x00,0x45,0x31,
    0x30,0x36,0x30,0x31,0x30,0x31,0x20,0x41,0x44,0x58,0x42,0x5F,0x44,0x65,0x63,0x6F,
    0x64,0x65,0x48,0x65,0x61,0x64,0x65,0x72,0x41,0x64,0x78,0x3A,0x20,0x00,0x63,0x61,
    0x6E,0x27,0x74,0x20,0x70,0x6C,0x61,0x79,0x20,0x41,0x48,0x58,0x20,0x64,0x61,0x74,
    0x61,0x20,0x62,0x79,0x20,0x74,0x68,0x69,0x73,0x20,0x68,0x61,0x6E,0x64,0x6C,0x65,
    0x00,0x45,0x32,0x30,0x30,0x35,0x30,0x34,0x32,0x37,0x30,0x31,0x20,0x3A,0x20,0x4E,
    0x55,0x4C,0x4C,0x20,0x70,0x6F,0x69,0x6E,0x74,0x65,0x72,0x20,0x69,0x73,0x20,0x70,
    0x61,0x73,0x73,0x65,0x64,0x2E,0x00,0x25,0x30,0x38,0x58,0x00,0x45,0x30,0x39,0x30,
    0x39,0x30,0x34,0x30,0x20,0x3A,0x20,0x43,0x61,0x6E,0x20,0x6E,0x6F,0x74,0x20,0x70,
    0x6C,0x61,0x79,0x20,0x74,0x68,0x65,0x20,0x6F,0x6C,0x64,0x20,0x66,0x6F,0x72,0x6D,
    0x61,0x74,0x2E,0x00,0x00,0x00,0x00,0x00
};

