// Decompiled CriWare ax_rna TU - high-level C reconstruction
// Matches retail assembly at 0x8039C384..0x8039DD7C

#include <harness_catalog.h>

typedef struct {
    unsigned char* ptr;
    int size;
} SJ_CHUNK;

/* SJ object vtable - the first word of every SJRBF/SJMEM object points to
 * this function table (retail data at lbl_eu_80565C30 / lbl_eu_80565C00).
 * Slot names come from the retail vtable relocations: slot 3 = Destroy,
 * 4 = GetUuid, 5 = Reset, 6 = GetChunk, 7 = UngetChunk, 8 = PutChunk,
 * 9 = get-avail (SJRBF fn_80397A74 / SJMEM GetNumData), 10 = IsGetChunk,
 * 11 = EntryErrFunc. Slots 0-2 are NULL in retail (reserved base-object
 * methods). Slot 9 is also invoked with a 3-arg list at one call site
 * (retail passes the vtable as the 3rd arg). */
struct SjObjVtbl {
    void (*reserved0)(void *self);                               /* 0x00 NULL in retail */
    void (*reserved1)(void *self);                               /* 0x04 NULL in retail */
    void (*reserved2)(void *self);                               /* 0x08 NULL in retail */
    void (*destroy)(void *self);                                 /* 0x0C SJRBF_Destroy */
    void *(*getUuid)(void *self);                                /* 0x10 SJRBF_GetUuid */
    void (*reset)(void *self);                                   /* 0x14 SJRBF_Reset */
    int (*getChunk)(void *self, int mode, int size, void *out);  /* 0x18 SJRBF_GetChunk */
    int (*ungetChunk)(void *self, int mode, void *chunk);        /* 0x1C SJRBF_UngetChunk */
    int (*putChunk)(void *self, int mode, void *chunk);          /* 0x20 SJRBF_PutChunk */
    int (*getAvail)(void *self, int mode);                       /* 0x24 SJRBF fn_80397A74 */
    int (*isGetChunk)(void *self, int mode, int size, int *out); /* 0x28 SJRBF_IsGetChunk */
    void (*entryErrFunc)(void *self, void *cb, void *arg);       /* 0x2C SJRBF_EntryErrFunc */
};

/* Fetch an SJ object's method table (its first word). */
#define SJ_VT(obj) (*(struct SjObjVtbl **)(obj))

typedef struct {
    u16 loopFlag;
    u16 format;
    u16 loopAddressHi;
    u16 loopAddressLo;
    u16 endAddressHi;
    u16 endAddressLo;
    u16 currentAddressHi;
    u16 currentAddressLo;
} AXPBADDR;

typedef struct {
    u16 ratioHi;
    u16 ratioLo;
    u16 currentAddressFrac;
    u16 last_samples[4];
} AXPBSRC;

// External globals
extern u32 lbl_eu_805F2C00;

// One RNA slot in the global table (stride 0xE4); voices live at offset 8.
typedef struct {
    u8 pad0[8];
    void* voices[2];
    u8 pad[0xE4 - 0x10];
} AXRNASLOT;
extern volatile u32 lbl_eu_8051914C;
extern u8 lbl_eu_805F2C08[];
extern u32 lbl_eu_805F2C04;
extern u32 lbl_eu_80566050;
extern u32 lbl_eu_80566054[];
extern u32 lbl_eu_805F3A48;
extern u32 lbl_eu_805F3A4C;
extern u8 lbl_eu_80519150[];

// External functions
extern void RNAERR_CallErrFunc(const char* msg);
extern void RNAERR_EntryErrFunc();
extern void GCRNA_LockCs(void);
extern void GCRNA_UnlockCs(void);
extern void MIXReleaseChannel(void* vpb);
extern void MIXSetInput(void* vpb, s32 input);
extern void MIXSetPan(void* vpb, s32 pan);
extern void MIXSetFader(void* vpb, s32 fader);
extern void MIXRmtSetFader(void* chanInfo, s32 selector, s32 value);
extern void MIXRmtSetVolumes(void* chanInfo, u32 mask, s32 v0, s32 v1, s32 v2, s32 v3,
                             s32 v4, s32 v5, s32 v6, s32 v7);
extern void MIXInitChannel(void* vpb, u32 flags, s32 input, s32 auxA, s32 auxB, s32 pan,
                           s32 panL, s32 panR, s32 fader);
extern void* SJRBF_Create(void* pool_mem, u32 buf_size, u32 xtr_size);
extern void SJ_SplitChunk(const SJ_CHUNK* src, int size, SJ_CHUNK* dst1, SJ_CHUNK* dst2);
extern void* AXAcquireVoice(u32 prio, void* callback, u32 userContext);
extern void AXFreeVoice(void* vpb);
extern void AXSetVoiceAddr(void* vpb, AXPBADDR* addr);
extern void AXSetVoiceState(void* vpb, u16 state);
extern void AXSetVoiceSrcType(void* vpb, u32 type);
extern void AXSetVoiceSrc(void* vpb, AXPBSRC* src);
extern void AXSetVoiceRmtOn(void* vpb, u16 on);
extern void DCFlushRange(void* addr, u32 nbytes);
extern void criware_8039B4E0(s16* src, s32 src_len, s16* dst, s32 dst_len);

void AXRNA_EntryErrFunc(void* fn) { RNAERR_EntryErrFunc(fn); }

void axrna_voice_drop(void* voice);
void AXRNA_Destroy(void* self);
void AXRNA_SetTransSw(void* self, s32 sw);
void AXRNA_SetPlaySw(void* self, s32 sw);
/* RNA object layout used by the streaming feed. The per-channel view has
 * stride 4, so all channel state is reached through one union window:
 * ch[0] = input ring (0x28), ch[2] = playback ring (0x30), ch[40] =
 * output SJ (0xC8); MWCC strength-reduces these indexes into a single
 * walking pointer, matching retail. Field accesses stay through the
 * struct so pointers reload after opaque calls, as in retail. */
typedef struct AxRnaFeed {
    u8 pad0[3];
    s8 nch;              /* 0x03 channel count */
    u8 pad1[0x24];       /* 0x04..0x27 */
    union {
        void* ch[41];    /* 0x28..0xCB stride-4 channel window */
        struct {
            u8 pad2[0x38];   /* 0x28..0x5F */
            s32 halfFeedPos; /* 0x60 last-fed size (samples) */
            s32 feedTotal;   /* 0x64 running fed total */
            u8 pad3[0x54];   /* 0x68..0xBB */
            s32 srcLen;      /* 0xBC input chunk length (samples) */
            s32 ratio;       /* 0xC0 resample ratio */
            s32 skipCnt;     /* 0xC4 consecutive underflow counter */
            void* outSj0;    /* 0xC8 output SJ of channel 0 */
        } sc;
    } u;
} AxRnaFeed;

void AXRNA_SetSfreq(void* self, s32 sfreq);
void AXRNA_SetMain(void* self, u32 index, s32 val);
void criware_8039A8E0(void* self, u32 flags);
void axrna_update_play(void* self);
void axrna_start_trans(void* self);
void axrna_start_flash(void* self);
void criware_80399F4C(AxRnaFeed* _this);

void AXRNA_Init(void) {
    (void)lbl_eu_8051914C;
    if (lbl_eu_805F2C00 == 0) {
        memset(lbl_eu_805F2C08, 0, 0xE40);
    }
    lbl_eu_805F2C00++;
}

void AXRNA_Finish(void) {
    s32 i;
    if (--lbl_eu_805F2C00 == 0) {
        for (i = 0; i < 0x10; i++) {
            if ((s32)lbl_eu_805F2C08[i * 0xE4] == 1) {
                AXRNA_Destroy(&lbl_eu_805F2C08[i * 0xE4]);
            }
        }
        memset(lbl_eu_805F2C08, 0, 0xE40);
    }
}

void axrna_voice_drop(void* voice) {
    u32* row;
    u32* w;
    s32 i, j;
    row = (u32*)lbl_eu_805F2C08;
    for (i = 0; i < 0x10; i++) {
        w = row;
        for (j = 0; j < 2; j++) {
            if (voice == (void*)w[2]) {
                AXRNASLOT* slot = (AXRNASLOT*)lbl_eu_805F2C08;
                MIXReleaseChannel(slot[i].voices[j]);
                slot[i].voices[j] = NULL;
                return;
            }
            w++;
        }
        row += 0xE4 / 4;
    }
}

void* AXRNA_Create(void* obj_arr, s32 maxnch, u8* buf) {
    void* slot;
    void* sj;
    void* vpb;
    u32 j;
    u32 n;
    u8* bufp;
    u8* ch;
    u32 i;

    if (maxnch <= 0) {
        RNAERR_CallErrFunc((const char*)(lbl_eu_80519150 + 0));
        return NULL;
    }
    if (obj_arr == NULL) {
        RNAERR_CallErrFunc((const char*)(lbl_eu_80519150 + 0x28));
        return NULL;
    }
    {
        u32* walk = (u32*)obj_arr;
        u32 k;
        for (k = 0; k < (u32)maxnch; k++) {
            if (*walk == 0) {
                RNAERR_CallErrFunc((const char*)(lbl_eu_80519150 + 0x4e));
                return NULL;
            }
            walk++;
        }
    }

    {
        u8* q = lbl_eu_805F2C08;
        u32 cnt = 0;
        for (n = 0; n < 2; n++) {
            for (j = 0; j < 8; j++) {
                if ((s8)q[0] != 0) {
                    q += 0xE4;
                    cnt++;
                } else {
                    goto found_slot;
                }
            }
        }
    found_slot:
        if (cnt == 0x10) {
            RNAERR_CallErrFunc((const char*)(lbl_eu_80519150 + 0x76));
            return NULL;
        }
        slot = &lbl_eu_805F2C08[cnt * 0xE4];
    }
    ((u8*)slot)[3] = (u8)maxnch;
    ((u8*)slot)[2] = (u8)maxnch;

    {
        u32* d = (u32*)slot;
        u32* s = (u32*)obj_arr;
        u32 k;
        for (k = 0; k < (u32)(s8)((u8*)slot)[2];) {
            /* increment order matches retail: load, ++k, store, ++s, ++d */
            u32 v = *s;
            k++;
            d[0xa] = v;
            s++;
            d++;
        }
    }

    *(s32*)((u8*)slot + 0x7c) = 0;
    *(s32*)((u8*)slot + 0x88) = 0x7f;
    *(s32*)((u8*)slot + 0x8c) = -0x3c0;
    *(s32*)((u8*)slot + 0x90) = -0x3c0;
    *(s32*)((u8*)slot + 0x94) = -0x3c0;
    *(s32*)((u8*)slot + 0x98) = 0;

    bufp = buf;
    ch = (u8*)slot;
    for (i = 0; i < (s8)((u8*)slot)[2]; i++) {
        *(u32*)(ch + 0x10) = (u32)bufp;
        *(u32*)((u8*)slot + 0x18) = 0x1000;
        sj = SJRBF_Create(*(void**)(ch + 0x10), 0x2000, 0);
        *(u32*)(ch + 0x30) = (u32)sj;
        if (sj == NULL) {
            RNAERR_CallErrFunc((const char*)(lbl_eu_80519150 + 0x97));
            AXRNA_Destroy(slot);
            return NULL;
        }
        vpb = AXAcquireVoice(0x1f, (void*)axrna_voice_drop, 0);
        *(u32*)(ch + 8) = (u32)vpb;
        if (vpb == NULL) {
            RNAERR_CallErrFunc((const char*)(lbl_eu_80519150 + 0xb2));
            AXRNA_Destroy(slot);
            return NULL;
        }
        GCRNA_LockCs();
        {
            void* v = *(void**)(ch + 8);
            if (v != 0) {
                MIXInitChannel(v, 3,
                               *(s32*)((u8*)slot + 0x7c), *(s32*)((u8*)slot + 0x8c),
                               *(s32*)((u8*)slot + 0x90), *(s32*)((u8*)slot + 0x94),
                               0x40, *(s32*)((u8*)slot + 0x88), *(s32*)((u8*)slot + 0x98));
            }
        }
        GCRNA_UnlockCs();
        bufp += 0x2000;
        ch += 4;
    }

    {
        u32 tbl;
        tbl = lbl_eu_805F2C04;
        if (slot != NULL) {
            *(u16*)((u8*)slot + 0x9c) = (u16)tbl;
        }
        tbl = lbl_eu_80566050;
        if (slot != NULL) {
            *(u32*)((u8*)slot + 0xa0) = tbl;
            *(u16*)((u8*)slot + 0x9e) = 1;
        }
    }
    *(u16*)((u8*)slot + 0x9e) = 0;
    AXRNA_SetSfreq(slot, 0xBB80);
    if (slot != NULL) {
        *(u32*)((u8*)slot + 0x78) = 0x10;
    }
    if ((s8)((u8*)slot)[2] == 2) {
        if (slot != NULL) {
            if ((s8)((u8*)slot)[2] > 0) {
                if (*(s32*)((u8*)slot + 0x80) != -0xf) {
                    *(s32*)((u8*)slot + 0x80) = -0xf;
                    GCRNA_LockCs();
                    if (*(u32*)((u8*)slot + 8) != 0) {
                        MIXSetPan(*(void**)((u8*)slot + 8), (s32)lbl_eu_80566054[0]);
                    }
                    GCRNA_UnlockCs();
                }
            }
        }
        if (slot != NULL) {
            if ((s8)((u8*)slot)[2] > 1) {
                if (*(s32*)((u8*)slot + 0x84) != 0xf) {
                    *(s32*)((u8*)slot + 0x84) = 0xf;
                    GCRNA_LockCs();
                    if (*(u32*)((u8*)slot + 0xc) != 0) {
                        MIXSetPan(*(void**)((u8*)slot + 0xc), (s32)lbl_eu_80566054[0x1e]);
                    }
                    GCRNA_UnlockCs();
                }
            }
        }
    } else {
        if (slot != NULL) {
            if ((s8)((u8*)slot)[2] > 0) {
                if (*(s32*)((u8*)slot + 0x80) != 0) {
                    *(s32*)((u8*)slot + 0x80) = 0;
                    GCRNA_LockCs();
                    if (*(u32*)((u8*)slot + 8) != 0) {
                        MIXSetPan(*(void**)((u8*)slot + 8), (s32)lbl_eu_80566054[0xf]);
                    }
                    GCRNA_UnlockCs();
                }
            }
        }
    }
    *(s32*)((u8*)slot + 0xb4) = 0;
    {
        s32 k;
        for (k = 0; k < 4; k++) {
            AXRNA_SetMain(slot, k, 0);
        }
    }
    criware_8039A8E0(slot, 0x10);
    ((u8*)slot)[1] = 0;
    ((u8*)slot)[0] = 1;
    return slot;
}

void AXRNA_Destroy(void* self) {
    s32 i;
    if (self == NULL)
        return;
    AXRNA_SetPlaySw(self, 0);
    AXRNA_SetTransSw(self, 0);
    for (i = 0; i < (s8)((u8*)self)[2]; i++) {
        void* sj = *(void**)((u8*)self + i * 4 + 0x30);
        if (sj != NULL) {
            SJ_VT(sj)->destroy(sj);
        }
        GCRNA_LockCs();
        {
            void* v = *(void**)((u8*)self + i * 4 + 8);
            if (v != NULL) {
                MIXReleaseChannel(v);
                AXFreeVoice(*(void**)((u8*)self + i * 4 + 8));
            }
        }
        GCRNA_UnlockCs();
    }
    memset(self, 0, 0xE4);
}

void AXRNA_SetTransSw(void* self, s32 sw) {
    /* Declaration order drives MWCC callee-saved assignment (retail:
     * ch=r29, p38=r28, p48=r27, loop counter=r26). */
    u8* ch;
    u8* p38;
    u8* p48;
    s32 cur;
    s32 i, j, k;
    if (self == NULL)
        return;
    if (self == NULL) {
        cur = -1;
    } else {
        cur = (s32)(*(u8*)((u8*)self + 1) & 1);
    }
    if (sw == cur)
        return;
    if (sw == 1) {
        GCRNA_LockCs();
        ch = (u8*)self;
        p38 = (u8*)self + 0x38;
        p48 = (u8*)self + 0x48;
        for (i = 0; i < (s8)((u8*)self)[3]; i++) {
            void* sj = *(void**)(ch + 0x30);
            SJ_VT(sj)->reset(sj);
            memset(p38, 0, 8);
            memset(p48, 0, 8);
            if (*(s32*)((u8*)self + 0xb4) == 1) {
                void* sj2 = *(void**)(ch + 0xc8);
                SJ_VT(sj2)->reset(sj2);
            }
            *(s32*)(ch + 0x58) = 0;
            ch += 4;
            p38 += 8;
            p48 += 8;
        }
        *(s32*)((u8*)self + 0x60) = 0;
        *(s32*)((u8*)self + 0x64) = 0;
        *(s32*)((u8*)self + 0x70) = 0;
        *(s32*)((u8*)self + 0x74) = 0;
        *(s32*)((u8*)self + 0x04) = -1;
        *(s32*)((u8*)self + 0xa4) = 0;
        *(s32*)((u8*)self + 0xc4) = 0;
        *(u8*)((u8*)self + 1) |= 1;
        GCRNA_UnlockCs();
    } else if (sw == 0) {
        for (i = 0; i < (s32)(s8)((u8*)self)[3]; i++) {
            s32 cnt = 0;
            for (j = 0; j < 0x14; j++) {
                for (k = 0; k < 0xa; k++) {
                    if (*(volatile s32*)((u8*)self + i * 4 + 0x58) == 0)
                        goto wait1;
                    cnt++;
                }
            }
        wait1:
            if (cnt == 0xc8) {
                RNAERR_CallErrFunc((const char*)(lbl_eu_80519150 + 0xd5));
                return;
            }
            cnt = 0;
            for (j = 0; j < 0x14; j++) {
                for (k = 0; k < 0xa; k++) {
                    if (*(volatile s32*)((u8*)self + i * 4 + 0x68) == 0)
                        goto wait2;
                    cnt++;
                }
            }
        wait2:
            if (cnt == 0xc8) {
                RNAERR_CallErrFunc((const char*)(lbl_eu_80519150 + 0x10b));
                return;
            }
        }
        /* NB: bug-for-bug, retail masks with 2 (clears every other flag bit) */
        *(u8*)((u8*)self + 1) &= 2;
    } else {
        RNAERR_CallErrFunc((const char*)(lbl_eu_80519150 + 0x142));
    }
}

void AXRNA_SetPlaySw(void* self, s32 sw) {
    /* Walking per-channel pointer (stride 4): voices at +8, buffer bases
     * at +0x10. Stop flag byte: bit1 = playing, bit0 = transferring.
     * NB: flag/fmt hold the hoisted AXPBADDR header words across the
     * AX calls (retail r30/r31); declaration order drives MWCC
     * callee-saved assignment. */
    u16 flag;
    u16 fmt;
    s32 cur;
    s32 i;
    u8* p;
    if (self == NULL)
        return;
    if (self == NULL) {
        cur = -1;
    } else {
        cur = (s32)((*(u8*)((u8*)self + 1) >> 1) & 1);
    }
    if (sw == cur)
        return;
    GCRNA_LockCs();
    if (sw == 1) {
        if (*(s32*)((u8*)self + 0xa4) == 0 || *(s32*)((u8*)self + 0xa4) > 0x800) {
            sw = 0;
        } else {
            sw = 0x800;
        }
        p = (u8*)self;
        i = 0;
        flag = 1;
        fmt = 0xa;
        for (; i < (s8)((u8*)self)[3]; i++) {
            if (*(void**)(p + 8) != NULL) {
                AXPBADDR addr;
                /* AX addresses split hi/lo around the 0x80000000 base */
                u32 mem = *(u32*)(p + 0x10);
                u32 loop = *(u32*)((u8*)self + 0x18);
                u32 base = mem + 0x80000000u;
                u32 t = loop * 2;
                u32 endm = base + t - 2;
                u32 curad = base + sw;
                addr.loopFlag = flag;
                addr.format = fmt;
                addr.loopAddressHi = (u16)(base >> 17);
                addr.loopAddressLo = (u16)(base >> 15);
                addr.endAddressHi = (u16)(endm >> 17);
                addr.endAddressLo = (u16)(endm >> 15);
                addr.currentAddressHi = (u16)(curad >> 17);
                addr.currentAddressLo = (u16)(curad >> 15);
                AXSetVoiceAddr(*(void**)(p + 8), &addr);
                AXSetVoiceState(*(void**)(p + 8), flag);
            }
            p += 4;
        }
        *(u8*)((u8*)self + 1) |= 2;
    } else if (sw == 0) {
        p = (u8*)self;
        for (i = 0; i < (s8)((u8*)self)[3]; i++) {
            if (*(void**)(p + 8) != NULL)
                AXSetVoiceState(*(void**)(p + 8), 0);
            p += 4;
        }
        /* NB: bug-for-bug, retail masks with 1 (clears every other flag bit) */
        *(u8*)((u8*)self + 1) &= 1;
    } else {
        RNAERR_CallErrFunc((const char*)(lbl_eu_80519150 + 0x163));
    }
    GCRNA_UnlockCs();
}

s32 AXRNA_GetNumData(void* self) {
    if (self == NULL)
        return -1;
    {
        u8* e = (u8*)self + (((s32)((s8)*(u8*)((u8*)self + 3) - 1) << 2) & 0xFFFFFFFC);
        void* p = *(void**)((u8*)e + 0x30);
        struct SjObjVtbl *q = SJ_VT(p);
        /* Slot 9 in its 3-arg (avail) shape. */
        return 0x1000 - (s32)(((u32)((s32 (*)(void*, s32, void*))q->getAvail)(p, 0, q)) >> 1)
               - *(s32*)((u8*)self + 0x74);
    }
}

typedef struct AXRNA {
    char pad0[3];
    signed char type;
    char pad4[0x2c];
    void *objs[1];
} AXRNA;

int AXRNA_GetNumRoom(AXRNA *rna) {
    if (rna == NULL) {
        return -1;
    }
    void *obj = rna->objs[rna->type - 1];
    return (unsigned int)SJ_VT(obj)->getAvail(obj, 0) >> 1;
}

#pragma auto_inline off
void axrna_update_play(void* self) {
    s32 i;
    s32 delta;
    s32 a4;
    void* voice;
    s32 cur;

    cur = *(s32*)((u8*)self + 4);
    voice = *(void**)((u8*)self + ((s8)((u8*)self)[3] - 1) * 4 + 8);
    if (voice == NULL)
        return;
    {
        u32 mem = *(u32*)((u8*)self + ((s8)((u8*)self)[3] - 1) * 4 + 0x10);
        /* retail: (mem + 0x80000000) >> 1 -- addis/srwi pair */
        a4 = (s32)(*(u32*)((u8*)voice + 0xa2) - ((mem + 0x80000000u) >> 1));
    }
    *(s32*)((u8*)self + 0xa4) = a4;
    if (a4 < 0 || a4 > *(s32*)((u8*)self + 0x18)) {
        for (;;) {
        }
    }
    if (cur == -1) {
        if (a4 == 0) {
            delta = 0;
        } else {
            cur = 0;
            *(s32*)((u8*)self + 4) = 0;
        }
    }
    if (cur != -1) {
        /* wrap-around delta: 0x1000 - (cur - a4), or plain a4 - cur when ahead */
        delta = 0x1000 - (cur - a4);
        if (a4 > cur) {
            delta = a4 - cur;
        }
    }
    delta = (delta / 0x800) * 0x800;
    if (delta > 0) {
        SJ_CHUNK out;
        void* selfcp = self;
        s32 bytes = delta * 2;
        for (i = 0; i < (s8)((u8*)self)[3]; i++) {
            /* NB: keep the un-hoisted object load; hoisting it here shifts
             * register allocation on this function (baseline diff profile). */
            ((void(*)(void*, s32, s32, void*))((*(void***)*(void**)((u8*)selfcp + i * 4 + 0x30))[6]))(
                *(void**)((u8*)selfcp + i * 4 + 0x30), 1, bytes, &out);
            ((void(*)(void*, s32, void*))((*(void***)*(void**)((u8*)selfcp + i * 4 + 0x30))[8]))(
                *(void**)((u8*)selfcp + i * 4 + 0x30), 0, &out);
        }
        {
            s32 v = *(s32*)((u8*)self + 4) + delta;
            *(s32*)((u8*)self + 4) = v;
            if (v >= 0x1000) {
                *(s32*)((u8*)self + 4) = v - 0x1000;
            }
        }
    }
    *(s32*)((u8*)self + 0xa8) += delta;
}

void axrna_start_trans(void* self) {
    /* Walkers per retail: ch strides 4 over channels (voices +8, flags
     * +0x58, playback ring +0x30, input ring +0x28); st strides 8 over
     * the cached last-chunk slots (chunkB copy at +0x38, chunkA at
     * +0x48); p38/p48 are the putChunk arguments. */
    u8* ch;
    u8* st;
    u8* p38;
    u8* p48;
    s32 i;
    void* sj;
    void* obj;
    SJ_CHUNK chunkB;
    SJ_CHUNK chunkD;
    SJ_CHUNK chunkA;
    SJ_CHUNK chunkC;
    s32 size;
    p38 = (u8*)self + 0x38;
    p48 = (u8*)self + 0x48;
    ch = (u8*)self;
    st = (u8*)self;

    for (i = 0; i < (s8)((u8*)self)[3]; i++) {
        if (*(u32*)(ch + 8) == 0)
            goto next;
        if (*(u32*)(ch + 0x58) != 0)
            goto next;
        sj = *(void**)(ch + 0x30);
        SJ_VT(sj)->getChunk(sj, 0, 0x2000, &chunkA);
        obj = *(void**)(ch + 0x28);
        SJ_VT(obj)->getChunk(obj, 1, chunkA.size, &chunkB);
        /* signed truncating divide-by-32 round-down (srawi/addze in retail) */
        size = ((chunkB.size < chunkA.size) ? chunkB.size : chunkA.size) / 0x20
               * 0x20;
        SJ_SplitChunk(&chunkA, size, &chunkA, &chunkC);
        sj = *(void**)(ch + 0x30);
        SJ_VT(sj)->ungetChunk(sj, 0, &chunkC);
        SJ_SplitChunk(&chunkB, size, &chunkB, &chunkD);
        obj = *(void**)(ch + 0x28);
        SJ_VT(obj)->ungetChunk(obj, 1, &chunkD);
        if (size == 0)
            return;
        *(SJ_CHUNK*)(st + 0x38) = chunkB;
        *(SJ_CHUNK*)(st + 0x48) = chunkA;
        /* logical shift right here (srwi in retail), unlike start_flash */
        *(u32*)((u8*)self + 0x60) = (u32)size >> 1;
        DCFlushRange(*(void**)(st + 0x38), *(u32*)(st + 0x3c));
        *(s32*)(ch + 0x58) = 1;
        memcpy(chunkA.ptr, chunkB.ptr, size);
        DCFlushRange(chunkA.ptr, size);
        if (*(s32*)(ch + 0x58) == 1) {
            obj = *(void**)(ch + 0x28);
            SJ_VT(obj)->putChunk(obj, 0, p38);
            obj = *(void**)(ch + 0x30);
            SJ_VT(obj)->putChunk(obj, 1, p48);
            *(s32*)(ch + 0x58) = 0;
            if (i == (s32)(s8)((u8*)self)[3] - 1) {
                *(u32*)((u8*)self + 0x64) += *(u32*)((u8*)self + 0x60);
            }
        }
    next:
        ch += 4;
        st += 8;
        p38 += 8;
        p48 += 8;
    }
}

/* Streaming feed: resamples input-ring data into the output rings, then
 * mirrors one chunk per channel from the output rings back into the
 * playback rings. NB (bug-for-bug): the availability probe reads
 * _this->in_ring but every actual chunk op walks the per-channel pointer
 * chp (stride 4), and the skip counter only resets on a successful feed. */
void criware_80399F4C(AxRnaFeed* _this) {
    /* Declaration order drives MWCC callee-saved assignment (retail:
     * chp=r29, dstx2=r28, srcx2=r27, i=r26, src_len=r25, dst_len=r24,
     * m=r23). */
    AxRnaFeed* chp;
    s32 dstx2;
    s32 srcx2;
    s32 i;
    s32 src_len;
    s32 dst_len;
    s32 m;
    s32 c4;
    SJ_CHUNK chunk2;
    SJ_CHUNK out;
    SJ_CHUNK split1;
    SJ_CHUNK chunk1;
    SJ_CHUNK split2;

    src_len = _this->u.sc.srcLen;
    dst_len = (src_len * _this->u.sc.ratio / 100) & ~3;
    if (_this->nch > 1) {
        RNAERR_CallErrFunc((const char*)(lbl_eu_80519150 + 0x450));
        return;
    }

    chp = _this;
    dstx2 = dst_len * 2;
    srcx2 = src_len * 2;
    for (i = 0; i < _this->nch; i++) {
        /* NB: every SJ access reloads the object pointer from the slot,
         * matching retail (no caching across the opaque vtable calls). */
        if ((u32)SJ_VT(chp->u.ch[40])->getAvail(chp->u.ch[40], 0)
            < (u32)dstx2) {
            goto tail;
        }
        if ((u32)SJ_VT(_this->u.ch[0])->getAvail(_this->u.ch[0], 1)
            < (u32)srcx2) {
            c4 = _this->u.sc.skipCnt;
            if (c4 <= 2) {
                goto skip_inc;
            }
        }
        /* resample src_len input samples into a dst_len-sample chunk */
        SJ_VT(chp->u.ch[40])->getChunk(chp->u.ch[40], 0, dstx2, &chunk1);
        if ((u32)chunk1.size < (u32)dstx2) {
            RNAERR_CallErrFunc((const char*)(lbl_eu_80519150 + 0x488));
            /* retail recomputes this slot from the base (+i*4), not chp */
            SJ_VT(_this->u.ch[i + 40])->ungetChunk(_this->u.ch[i + 40], 0,
                                                   &chunk1);
            return;
        }
        SJ_VT(chp->u.ch[0])->getChunk(chp->u.ch[0], 1, srcx2, &chunk2);
        memcpy(lbl_eu_805F3A4C, chunk2.ptr, chunk2.size);
        SJ_VT(chp->u.ch[0])->putChunk(chp->u.ch[0], 0, &chunk2);
        m = (u32)chunk2.size >> 1;
        if (m < src_len) {
            /* ring wrapped: pull the remainder, zero-fill any shortfall */
            SJ_VT(chp->u.ch[0])->getChunk(chp->u.ch[0], 1, (src_len - m) * 2,
                                          &chunk2);
            memcpy((char*)lbl_eu_805F3A4C + m * 2, chunk2.ptr, chunk2.size);
            SJ_VT(chp->u.ch[0])->putChunk(chp->u.ch[0], 0, &chunk2);
            m += (u32)chunk2.size >> 1;
            if (m < src_len) {
                memset((char*)lbl_eu_805F3A4C + m * 2, 0, (src_len - m) * 2);
            }
        }
        criware_8039B4E0((s16*)lbl_eu_805F3A4C, src_len, (s16*)chunk1.ptr,
                         dst_len);
        SJ_VT(chp->u.ch[40])->putChunk(chp->u.ch[40], 1, &chunk1);
        _this->u.sc.skipCnt = 0;
        goto tail;
    skip_inc:
        _this->u.sc.skipCnt = c4 + 1;
    tail:
        /* mirror one aligned chunk from the output ring to the playback ring */
        SJ_VT(chp->u.ch[2])->getChunk(chp->u.ch[2], 0, 0x2000, &out);
        SJ_VT(chp->u.ch[40])->getChunk(chp->u.ch[40], 1, out.size, &chunk1);
        /* signed round-to-multiple-of-32 of the smaller chunk size */
        m = ((chunk1.size < out.size) ? chunk1.size : out.size) / 0x20 * 0x20;
        SJ_SplitChunk(&out, m, &out, &split1);
        SJ_VT(chp->u.ch[2])->ungetChunk(chp->u.ch[2], 0, &split1);
        SJ_SplitChunk(&chunk1, m, &chunk1, &split2);
        SJ_VT(chp->u.ch[40])->ungetChunk(chp->u.ch[40], 1, &split2);
        memcpy(out.ptr, chunk1.ptr, m);
        DCFlushRange(out.ptr, m);
        SJ_VT(chp->u.ch[40])->putChunk(chp->u.ch[40], 0, &chunk1);
        SJ_VT(chp->u.ch[2])->putChunk(chp->u.ch[2], 1, &out);
        _this->u.sc.halfFeedPos = (u32)m >> 1;
        if (i == _this->nch - 1) {
            _this->u.sc.feedTotal += (u32)m >> 1;
        }
        if (_this->u.sc.feedTotal > 0xBB80) {
            lbl_eu_805F3A48++;
        }
        chp = (AxRnaFeed*)((char*)chp + 4);
    }
}

void axrna_start_flash(void* self) {
    u8* ch;
    u8* cs;
    u8* p48 = (u8*)self + 0x48;
    s32 i;
    s32 size;
    SJ_CHUNK out;
    SJ_CHUNK split;
    ch = (u8*)self;
    cs = (u8*)self;

    for (i = 0; i < (s8)((u8*)self)[3]; i++) {
        if (*(u32*)(ch + 0x68) != 0)
            goto next;
        {
            void* sj = *(void**)(ch + 0x30);
            SJ_VT(sj)->getChunk(sj, 0, 0x2000, &out);
        }
        /* signed round-to-zero divide by 32 (srawi/addze in retail) */
        size = (out.size / 0x20) * 0x20;
        SJ_SplitChunk(&out, size, &out, &split);
        {
            void* sj = *(void**)(ch + 0x30);
            SJ_VT(sj)->ungetChunk(sj, 0, &split);
        }
        if (size == 0)
            return;
        *(SJ_CHUNK*)(cs + 0x48) = out;
        *(s32*)((u8*)self + 0x70) = (s32)((u32)size >> 1);
        *(s32*)(ch + 0x68) = 1;
        memset(out.ptr, 0, size);
        DCFlushRange(out.ptr, size);
        if (*(s32*)(ch + 0x68) == 1) {
            void* obj;
            obj = *(void**)(ch + 0x30);
            SJ_VT(obj)->putChunk(obj, 1, p48);
            *(s32*)(ch + 0x68) = 0;
            if (i == (s8)((u8*)self)[3] - 1) {
                *(s32*)((u8*)self + 0x74) += *(s32*)((u8*)self + 0x70);
            }
        }
    next:
        ch += 4;
        cs += 8;
        p48 += 8;
    }
}
#pragma auto_inline on

void AXRNA_ExecServer(void) {
    u32 i;
    u8* rna;
    s32 st;

    GCRNA_LockCs();
    for (i = 0; i < 0x10; i++) {
        rna = lbl_eu_805F2C08 + i * 0xE4;
        if ((s32)rna[0] != 1)
            continue;
        if (rna == NULL)
            continue;
        if (rna == NULL) {
            st = -1;
        } else {
            st = ((rna[1] >> 1) & 1);
        }
        if (st == 1)
            axrna_update_play(rna);
        if (rna == NULL) {
            st = -1;
        } else {
            st = (rna[1] & 1);
        }
        if (st == 1) {
            if (*(s32*)(rna + 0xB4) == 1)
                criware_80399F4C((AxRnaFeed*)rna);
            else
                axrna_start_trans(rna);
        } else {
            if (rna == NULL) {
                st = -1;
            } else {
                st = ((rna[1] >> 1) & 1);
            }
            if (st == 1 && *(s32*)(rna + 0x74) < *(s32*)(rna + 0x18))
                axrna_start_flash(rna);
        }
    }
    GCRNA_UnlockCs();
}

void AXRNA_SetNumChan(void* self, u8 numChan) {
    if (self != NULL) {
        *(u8*)((u8*)self + 0x03) = numChan;
    }
}

void AXRNA_SetSfreq(void* self, s32 sfreq) {
    u8* p;
    s32 hi;
    u16 lo;
    s32 i;
    AXPBSRC src;
    if (self == NULL)
        return;
    if (sfreq == 0x7d1c) {
        RNAERR_CallErrFunc((const char*)(lbl_eu_80519150 + 0x4b5));
    }
    *(s32*)((u8*)self + 0x1c) = sfreq;
    /* AX SRC ratio: integer part = rate/32000 (magic 0x10624dd3, shift 11);
     * fraction part computed as (rate<<8)/125 (same magic, shift 3). */
    hi = sfreq / 32000;
    lo = (sfreq << 8) / 125;
    p = (u8*)self;
    /* NB: reuse sfreq as the channel-loop counter so MWCC recycles its
     * register (retail: li r26, 0 over the dead parameter). */
    for (i = 0; i < (s8)((u8*)self)[2]; i++) {
        GCRNA_LockCs();
        {
            void* e = *(void**)(p + 8);
            if (e != NULL) {
                src.ratioHi = (u16)hi;
                src.ratioLo = lo;
                src.currentAddressFrac = 0;
                src.last_samples[0] = 0;
                src.last_samples[1] = 0;
                src.last_samples[2] = 0;
                src.last_samples[3] = 0;
                AXSetVoiceSrcType(e, *(s32*)((u8*)self + 0xa0));
                /* reload: AXSetVoiceSrcType may have changed the slot */
                AXSetVoiceSrc(*(void**)(p + 8), &src);
            }
        }
        GCRNA_UnlockCs();
        p += 4;
    }
}

void AXRNA_SetOutVol(void* self, s32 vol) {
    s32 t;
    s32 v;
    s32 i;
    if (self == NULL)
        return;
    t = vol & (vol >> 31);
    v = -960;
    if (t > -960)
        v = t;
    if (v == *(s32*)((u8*)self + 0x7c))
        return;
    *(s32*)((u8*)self + 0x7c) = v;
    for (i = 0; i < (s8)((u8*)self)[2]; i++) {
        GCRNA_LockCs();
        {
            void* e = *(void**)((u8*)self + 4 * i + 8);
            if (e != NULL)
                MIXSetInput(e, v);
        }
        GCRNA_UnlockCs();
    }
}

void AXRNA_SetOutPan(void* self, s32 index, s32 pan) {
    /* channel view of an RNA object: per-channel voice pointers at +8,
     * per-channel pan values at +0x80 */
    typedef struct {
        u8 flags[2];
        s8 nch;
        u8 maxnch;
        u8 pad3[4];
        void* voices[4];   /* 0x08 */
        u8 pad4[0x80 - 0x18];
        s32 chanPan[4];    /* 0x80 */
    } AxRnaChans;
    AxRnaChans* rna;
    s32 p;
    s32 v;
    if (self == NULL)
        return;
    rna = (AxRnaChans*)self;
    if (index >= rna->nch)
        return;
    /* clamp pan into [-15, 15] */
    p = (pan < 0xf) ? pan : 0xf;
    v = (p > -0xf) ? p : -0xf;
    if (rna->chanPan[index] == v)
        return;
    rna->chanPan[index] = v;
    GCRNA_LockCs();
    {
        void* e = rna->voices[index];
        if (e != NULL) {
            MIXSetPan(e, (s32)lbl_eu_80566054[v + 0xf]);
        }
    }
    GCRNA_UnlockCs();
}

void AXRNA_SetBitPerSmpl(void* self, u32 val) {
    if (self != NULL) {
        *(u32*)((u8*)self + 0x78) = val;
    }
}

int AXRNA_SetStmHdInfo(void) { return 0x0; }

int AXRNA_DiscardData(void) { return 0x0; }

void AXRNA_SetMain(void* self, u32 index, s32 val) {
    s32* p;
    s32 t;
    s32 v;
    u8* q;
    s32 i;
    if (self == NULL)
        return;
    if (index > 3) {
        RNAERR_CallErrFunc((const char*)(lbl_eu_80519150 + 0x4e4));
        return;
    }
    t = val & (val >> 31);
    v = -960;
    if (t > -960)
        v = t;
    p = (s32*)((u8*)self + index * 4);
    if (v == p[0xd0 / 4])
        return;
    p[0xd0 / 4] = v;
    {
        q = (u8*)self;
        for (i = 0; i < (s8)((u8*)self)[2]; i++) {
            GCRNA_LockCs();
            {
                void* e = *(void**)(q + 8);
                if (e != NULL)
                    MIXRmtSetFader(e, index, p[0xd0 / 4]);
            }
            GCRNA_UnlockCs();
            q += 4;
        }
    }
}

/* Per-channel remote (controller) volume update. NOTE (bug-for-bug):
 * retail gates the 0xd4 tap with flag bit 2 and the 0xd8 tap with bit 1
 * (crossed versus the natural bit order). */
void criware_8039A8E0(void* self, u32 flags) {
    s32 vol;
    u8* p;
    u32 b4;
    u32 b0;
    u32 b2;
    u32 b1;
    u32 b3;
    u32 msk;
    s32 i;
    s32 fader0;
    s32 fader1;
    s32 fader2;
    s32 fader3;

    fader0 = -0x3c0;
    fader1 = -0x3c0;
    fader2 = -0x3c0;
    fader3 = -0x3c0;
    if (self == NULL)
        return;
    ((u8*)self)[0xe0] = (u8)(flags & 0x1f);
    p = (u8*)self;
    i = 0;
    b4 = flags & 0x10;
    b0 = flags & 1;
    b2 = flags & 4;
    b1 = flags & 2;
    b3 = flags & 8;
    msk = flags & 0xf;
    vol = -0x3c0;
    while (i < (s8)((u8*)self)[2]) {
        GCRNA_LockCs();
        if (*(void**)(p + 8) != NULL) {
            if (b4) {
                MIXSetFader(*(void**)(p + 8), *(s32*)((u8*)self + 0x98));
            } else {
                MIXSetFader(*(void**)(p + 8), -0x3c0);
            }
            if (b0)
                fader0 = *(s32*)((u8*)self + 0xd0);
            if (b2)
                fader1 = *(s32*)((u8*)self + 0xd4);
            if (b1)
                fader2 = *(s32*)((u8*)self + 0xd8);
            if (b3)
                fader3 = *(s32*)((u8*)self + 0xdc);
            if (msk) {
                MIXRmtSetVolumes(*(void**)(p + 8), 0, fader0,
                                 fader1, fader2, fader3, vol, vol, vol, vol);
                AXSetVoiceRmtOn(*(void**)(p + 8), 1);
            } else {
                AXSetVoiceRmtOn(*(void**)(p + 8), 0);
            }
        }
        GCRNA_UnlockCs();
        p += 4;
        i++;
    }
}
