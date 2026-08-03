// Decompiled CriWare ax_rna TU — high-level C reconstruction
// Matches retail assembly at 0x8039C384..0x8039DD7C

#include <harness_catalog.h>

typedef struct {
    unsigned char* ptr;
    int size;
} SJ_CHUNK;

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
extern volatile u32 lbl_eu_8051914C;
extern u8 lbl_eu_805F2C08[];
extern u16 lbl_eu_805F2C04;
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
void AXRNA_SetSfreq(void* self, s32 sfreq);
void AXRNA_SetMain(void* self, u32 index, s32 val);
void criware_8039A8E0(void* self, u32 flags);
void axrna_update_play(void* self);
void axrna_start_trans(void* self);
void axrna_start_flash(void* self);
void criware_80399F4C(void* self);

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
    s32 i, j;
    for (i = 0; i < 0x10; i++) {
        for (j = 0; j < 2; j++) {
            if (voice == *(void**)((u8*)&lbl_eu_805F2C08 + i * 0xE4 + j * 4 + 8)) {
                MIXReleaseChannel(*(void**)((u8*)&lbl_eu_805F2C08 + i * 0xE4 + j * 4 + 8));
                *(u32*)((u8*)&lbl_eu_805F2C08 + i * 0xE4 + j * 4 + 8) = 0;
                return;
            }
        }
    }
}

void* AXRNA_Create(void* obj_arr, u32 maxnch, u8* buf) {
    void* slot;
    u32 i;
    u32 j;
    u32 n;
    u32 idx;
    u32* p;
    u32* dst;
    u32* src;
    void* sj;
    void* vpb;

    if (maxnch <= 0) {
        RNAERR_CallErrFunc((const char*)(lbl_eu_80519150 + 0));
        return NULL;
    }
    if (obj_arr == NULL) {
        RNAERR_CallErrFunc((const char*)(lbl_eu_80519150 + 0x28));
        return NULL;
    }
    p = (u32*)obj_arr;
    for (i = 0; i < maxnch; i++) {
        if (p[i] == 0) {
            RNAERR_CallErrFunc((const char*)(lbl_eu_80519150 + 0x4e));
            return NULL;
        }
    }

    idx = 0;
    for (n = 0; n < 2; n++) {
        for (j = 0; j < 8; j++) {
            if ((s8)lbl_eu_805F2C08[n * 8 * 0xE4 + j * 0xE4] != 0) {
                idx++;
            } else {
                goto found_slot;
            }
        }
    }
found_slot:
    if (idx >= 0x10) {
        RNAERR_CallErrFunc((const char*)(lbl_eu_80519150 + 0x76));
        return NULL;
    }

    slot = &lbl_eu_805F2C08[idx * 0xE4];
    ((u8*)slot)[3] = (u8)maxnch;
    ((u8*)slot)[2] = (u8)maxnch;

    dst = (u32*)((u8*)slot + 0x28);
    src = (u32*)obj_arr;
    for (i = 0; i < maxnch; i++) {
        *dst++ = *src++;
    }

    *(s32*)((u8*)slot + 0x7c) = 0;
    *(s32*)((u8*)slot + 0x88) = 0x7f;
    *(s32*)((u8*)slot + 0x8c) = -0x3c0;
    *(s32*)((u8*)slot + 0x90) = -0x3c0;
    *(s32*)((u8*)slot + 0x94) = -0x3c0;
    *(s32*)((u8*)slot + 0x98) = 0;

    for (i = 0; i < (s8)((u8*)slot)[2]; i++) {
        *(u32*)((u8*)slot + i * 4 + 0x10) = (u32)(buf + i * 0x2000);
        *(u32*)((u8*)slot + 0x18) = 0x1000;
        sj = SJRBF_Create(buf + i * 0x2000, 0x2000, 0);
        *(u32*)((u8*)slot + i * 4 + 0x30) = (u32)sj;
        if (sj == NULL) {
            RNAERR_CallErrFunc((const char*)(lbl_eu_80519150 + 0x97));
            AXRNA_Destroy(slot);
            return NULL;
        }
        vpb = AXAcquireVoice(0x1f, (void*)axrna_voice_drop, 0);
        *(u32*)((u8*)slot + i * 4 + 8) = (u32)vpb;
        if (vpb == NULL) {
            RNAERR_CallErrFunc((const char*)(lbl_eu_80519150 + 0xb2));
            AXRNA_Destroy(slot);
            return NULL;
        }
        GCRNA_LockCs();
        if (*(u32*)((u8*)slot + i * 4 + 8) != 0) {
            MIXInitChannel(*(void**)((u8*)slot + i * 4 + 8), 3,
                           *(s32*)((u8*)slot + 0x7c), *(s32*)((u8*)slot + 0x8c),
                           *(s32*)((u8*)slot + 0x90), *(s32*)((u8*)slot + 0x94),
                           0x40, *(s32*)((u8*)slot + 0x88), *(s32*)((u8*)slot + 0x98));
        }
        GCRNA_UnlockCs();
    }

    if (slot != NULL) {
        *(u16*)((u8*)slot + 0x9c) = lbl_eu_805F2C04;
    }
    if (slot != NULL) {
        *(u32*)((u8*)slot + 0xa0) = lbl_eu_80566050;
        *(u16*)((u8*)slot + 0x9e) = 1;
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
    for (i = 0; i < 4; i++) {
        AXRNA_SetMain(slot, i, 0);
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
            ((void(*)(void*))(*(void***)sj)[3])(sj);
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
        u8* p48;
        u8* p38;
        u8* ch;
        GCRNA_LockCs();
        ch = (u8*)self;
        p38 = (u8*)self + 0x38;
        p48 = (u8*)self + 0x48;
        for (i = 0; i < (s8)((u8*)self)[3]; i++) {
            void* sj = *(void**)(ch + 0x30);
            ((void(*)(void*))(*(void***)sj)[5])(sj);
            memset(p38, 0, 8);
            memset(p48, 0, 8);
            if (*(s32*)((u8*)self + 0xb4) == 1) {
                void* sj2 = *(void**)(ch + 0xc8);
                ((void(*)(void*))(*(void***)sj2)[5])(sj2);
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
        s32 nch = (s8)((u8*)self)[3];
        for (i = 0; i < nch; i++) {
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
        *(u8*)((u8*)self + 1) &= ~2;
    } else {
        RNAERR_CallErrFunc((const char*)(lbl_eu_80519150 + 0x142));
    }
}

void AXRNA_SetPlaySw(void* self, s32 sw) {
    s32 cur;
    s32 i;
    if (self == NULL)
        return;
    cur = (self == NULL) ? -1 : (s32)((*(u8*)((u8*)self + 1) >> 1) & 1);
    if (sw == cur)
        return;
    GCRNA_LockCs();
    if (sw == 1) {
        s32 limit = *(s32*)((u8*)self + 0xa4);
        if (limit == 0 || limit > 0x800) {
            sw = 0;
        } else {
            sw = 0x800;
        }
        for (i = 0; i < (s8)((u8*)self)[3]; i++) {
            void* v = *(void**)((u8*)self + i * 4 + 8);
            if (v != NULL) {
                AXPBADDR addr;
                u32 mem = *(u32*)((u8*)self + i * 4 + 0x10);
                u32 loop = *(u32*)((u8*)self + 0x18);
                u32 base = mem + 0x80000000u;
                addr.loopFlag = 1;
                addr.format = 0xa;
                addr.loopAddressHi = (u16)(base >> 17);
                addr.loopAddressLo = (u16)((base >> 15) & 0xffff);
                addr.endAddressHi = (u16)((base + loop * 2 - 2) >> 17);
                addr.endAddressLo = (u16)(((base + loop * 2 - 2) >> 15) & 0xffff);
                addr.currentAddressHi = (u16)((base + sw) >> 17);
                addr.currentAddressLo = (u16)(((base + sw) >> 15) & 0xffff);
                AXSetVoiceAddr(v, &addr);
                AXSetVoiceState(*(void**)((u8*)self + i * 4 + 8), 1);
            }
        }
        *(u8*)((u8*)self + 1) |= 2;
    } else if (sw == 0) {
        for (i = 0; i < (s8)((u8*)self)[3]; i++) {
            void* v = *(void**)((u8*)self + i * 4 + 8);
            if (v != NULL)
                AXSetVoiceState(v, 0);
        }
        *(u8*)((u8*)self + 1) &= ~1;
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
        void* q = *(void**)((u8*)p + 0);
        u32 r = (u32)((s32 (*)(void*, s32, void*))*(void**)((u8*)q + 36))(p, 0, q);
        return 0x1000 - (s32)(r >> 1) - *(s32*)((u8*)self + 0x74);
    }
}

typedef struct AXRNA { char pad0[3]; signed char type; char pad4[0x2c]; void *objs[1]; } AXRNA; int AXRNA_GetNumRoom(AXRNA *rna) { if (rna == NULL) { return -1; } void *obj = rna->objs[rna->type - 1]; int (*func)(void *, int) = ((int (**)(void *, int))(*(void ***)obj))[9]; return (unsigned int)func(obj, 0) >> 1; }

#pragma auto_inline off
void axrna_update_play(void* self) {
    s32 i;
    s32 cur;
    s32 a4;
    s32 r28;
    void* voice;

    cur = *(s32*)((u8*)self + 4);
    voice = *(void**)((u8*)self + ((s8)((u8*)self)[3] - 1) * 4 + 8);
    if (voice == NULL)
        return;
    a4 = (s32)(*(u32*)((u8*)voice + 0xa2) - ((*(u32*)((u8*)self + ((s8)((u8*)self)[3] - 1) * 4 + 0x10) + 0x80000000u) >> 1));
    *(s32*)((u8*)self + 0xa4) = a4;
    if (a4 < 0 || a4 > *(s32*)((u8*)self + 0x18)) {
        for (;;) {
        }
    }
    if (cur == -1) {
        if (a4 == 0) {
            r28 = 0;
        } else {
            cur = 0;
            *(s32*)((u8*)self + 4) = 0;
        }
    }
    if (cur != -1) {
        r28 = 0x1000 - cur + a4;
        if (a4 > cur) {
            r28 = a4 - cur;
        }
    }
    r28 = (r28 / 0x800) * 0x800;
    if (r28 > 0) {
        SJ_CHUNK out;
        for (i = 0; i < (s8)((u8*)self)[3]; i++) {
            ((void(*)(void*, s32, s32, void*))((*(void***)*(void**)((u8*)self + i * 4 + 0x30))[6]))(
                *(void**)((u8*)self + i * 4 + 0x30), 1, r28 * 2, &out);
            ((void(*)(void*, s32, void*))((*(void***)*(void**)((u8*)self + i * 4 + 0x30))[8]))(
                *(void**)((u8*)self + i * 4 + 0x30), 0, &out);
        }
        {
            s32 v = *(s32*)((u8*)self + 4) + r28;
            if (v >= 0x1000)
                v -= 0x1000;
            *(s32*)((u8*)self + 4) = v;
        }
    }
    *(s32*)((u8*)self + 0xa8) += r28;
}

void axrna_start_trans(void* self) {
    s32 i;
    SJ_CHUNK chunkA;
    SJ_CHUNK chunkB;
    SJ_CHUNK chunkC;
    SJ_CHUNK chunkD;
    s32 size;
    u8* ch = (u8*)self + 0x38;
    u8* cs = (u8*)self + 0x48;
    u8* p38 = (u8*)self;
    u8* p48 = (u8*)self;

    for (i = 0; i < (s8)((u8*)self)[3]; i++) {
        if (*(u32*)(ch + 8) == 0)
            goto next;
        if (*(u32*)(ch + 0x58) != 0)
            goto next;
        {
            void* sj = *(void**)(ch + 0x30);
            ((void(*)(void*, s32, s32, void*))(*(void***)sj)[6])(sj, 0, 0x2000, &chunkA);
        }
        {
            void* obj = *(void**)(ch + 0x28);
            ((void(*)(void*, s32, s32, void*))(*(void***)obj)[6])(obj, 1, chunkA.size, &chunkB);
        }
        size = (chunkA.size < chunkB.size) ? chunkA.size : chunkB.size;
        size = (size / 0x20) * 0x20;
        SJ_SplitChunk(&chunkA, size, &chunkA, &chunkC);
        {
            void* sj = *(void**)(ch + 0x30);
            ((void(*)(void*, s32, void*))(*(void***)sj)[7])(sj, 0, &chunkC);
        }
        SJ_SplitChunk(&chunkB, size, &chunkB, &chunkD);
        {
            void* obj = *(void**)(ch + 0x28);
            ((void(*)(void*, s32, void*))(*(void***)obj)[7])(obj, 1, &chunkD);
        }
        if (size == 0)
            return;
        *(s32*)(cs + 0x38) = (u32)chunkB.ptr;
        *(s32*)(cs + 0x3c) = chunkB.size;
        *(s32*)(cs + 0x48) = (u32)chunkA.ptr;
        *(s32*)(cs + 0x4c) = chunkA.size;
        *(s32*)((u8*)self + 0x60) = size >> 1;
        DCFlushRange(*(void**)(cs + 0x38), *(u32*)(cs + 0x3c));
        *(s32*)(ch + 0x58) = 1;
        memcpy(chunkA.ptr, chunkB.ptr, size);
        DCFlushRange(chunkA.ptr, size);
        if (*(s32*)(ch + 0x58) == 1) {
            void* obj = *(void**)(ch + 0x28);
            ((void(*)(void*, s32, void*))(*(void***)obj)[8])(obj, 0, p38);
            obj = *(void**)(ch + 0x30);
            ((void(*)(void*, s32, void*))(*(void***)obj)[8])(obj, 1, p48);
            *(s32*)(ch + 0x58) = 0;
            if (i == (s8)((u8*)self)[3] - 1) {
                *(s32*)((u8*)self + 0x64) += *(s32*)((u8*)self + 0x60);
            }
        }
    next:
        ch += 4;
        cs += 8;
        p38 += 8;
        p48 += 8;
    }
}

void criware_80399F4C(void* self) {
    s32 i;
    s32 nch;
    s32 src_len;
    s32 dst_len;
    s32 c4;
    s32 m;
    SJ_CHUNK chunk1;
    SJ_CHUNK chunk2;
    SJ_CHUNK out;
    SJ_CHUNK out2;
    SJ_CHUNK out3;

    src_len = *(s32*)((u8*)self + 0xbc);
    dst_len = (src_len * *(s32*)((u8*)self + 0xc0)) / 100;
    dst_len &= ~3;
    nch = (s8)((u8*)self)[3];
    if (nch > 1) {
        RNAERR_CallErrFunc((const char*)(lbl_eu_80519150 + 0x450));
        return;
    }
    for (i = 0; i < nch; i++) {
        void* sj = *(void**)((u8*)self + i * 4 + 0xc8);
        if ((s32)((s32(*)(void*, s32))((*(void***)sj)[9]))(sj, 0) < dst_len * 2) {
            goto skip;
        }
        {
            void* obj = *(void**)((u8*)self + 0x28);
            if ((s32)((s32(*)(void*, s32))((*(void***)obj)[9]))(obj, 1) < src_len * 2) {
                c4 = *(s32*)((u8*)self + 0xc4);
                if (c4 <= 2)
                    goto skip;
            }
        }
        ((void(*)(void*, s32, s32, void*))((*(void***)sj)[6]))(sj, 0, dst_len * 2, &chunk1);
        if (chunk1.size < dst_len * 2) {
            RNAERR_CallErrFunc((const char*)(lbl_eu_80519150 + 0x488));
            ((void(*)(void*, s32, void*))((*(void***)sj)[7]))(sj, 0, &chunk1);
            return;
        }
        {
            void* obj = *(void**)((u8*)self + i * 4 + 0x28);
            ((void(*)(void*, s32, s32, void*))((*(void***)obj)[6]))(obj, 1, src_len * 2, &chunk2);
            memcpy((void*)lbl_eu_805F3A4C, chunk2.ptr, chunk2.size);
            ((void(*)(void*, s32, void*))((*(void***)obj)[8]))(obj, 0, &chunk2);
            if ((chunk2.size >> 1) < src_len) {
                s32 half = chunk2.size >> 1;
                ((void(*)(void*, s32, s32, void*))((*(void***)obj)[6]))(obj, 1, (src_len - half) * 2, &chunk2);
                memcpy((void*)((u8*)lbl_eu_805F3A4C + half * 2), chunk2.ptr, chunk2.size);
                ((void(*)(void*, s32, void*))((*(void***)obj)[8]))(obj, 0, &chunk2);
                half += chunk2.size >> 1;
                if (half < src_len) {
                    memset((void*)((u8*)lbl_eu_805F3A4C + half * 2), 0, (src_len - half) * 2);
                }
            }
        }
        criware_8039B4E0((s16*)(void*)lbl_eu_805F3A4C, src_len, (s16*)chunk1.ptr, dst_len);
        ((void(*)(void*, s32, void*))((*(void***)sj)[8]))(sj, 1, &chunk1);
        *(s32*)((u8*)self + 0xc4) = 0;
        goto done;
    skip:
        c4 = *(s32*)((u8*)self + 0xc4);
        *(s32*)((u8*)self + 0xc4) = c4 + 1;
    done:
        {
            void* ring = *(void**)((u8*)self + i * 4 + 0x30);
            ((void(*)(void*, s32, s32, void*))((*(void***)ring)[6]))(ring, 0, 0x2000, &out);
        }
        {
            void* sj2 = *(void**)((u8*)self + i * 4 + 0xc8);
            ((void(*)(void*, s32, s32, void*))((*(void***)sj2)[6]))(sj2, 1, out.size, &chunk1);
        }
        m = (out.size < chunk1.size) ? out.size : chunk1.size;
        m = (m / 0x20) * 0x20;
        SJ_SplitChunk(&out, m, &out, &out2);
        {
            void* ring = *(void**)((u8*)self + i * 4 + 0x30);
            ((void(*)(void*, s32, void*))((*(void***)ring)[7]))(ring, 0, &out2);
        }
        SJ_SplitChunk(&chunk1, m, &chunk1, &out3);
        {
            void* sj2 = *(void**)((u8*)self + i * 4 + 0xc8);
            ((void(*)(void*, s32, void*))((*(void***)sj2)[7]))(sj2, 1, &out3);
        }
        memcpy(out.ptr, chunk1.ptr, m);
        DCFlushRange(out.ptr, m);
        {
            void* sj2 = *(void**)((u8*)self + i * 4 + 0xc8);
            ((void(*)(void*, s32, void*))((*(void***)sj2)[8]))(sj2, 0, &chunk1);
        }
        {
            void* ring = *(void**)((u8*)self + i * 4 + 0x30);
            ((void(*)(void*, s32, void*))((*(void***)ring)[8]))(ring, 1, &out);
        }
        *(s32*)((u8*)self + 0x60) = m >> 1;
        if (i == (s8)((u8*)self)[3] - 1) {
            *(s32*)((u8*)self + 0x64) += m >> 1;
        }
        if (*(s32*)((u8*)self + 0x64) > 0xBB80) {
            lbl_eu_805F3A48++;
        }
    }
}

void axrna_start_flash(void* self) {
    s32 i;
    SJ_CHUNK out;
    SJ_CHUNK split;
    u8* ch = (u8*)self;
    u8* cs = (u8*)self;
    u8* p48 = (u8*)self + 0x48;

    for (i = 0; i < (s8)((u8*)self)[3]; i++) {
        s32 size;
        if (*(u32*)(ch + 0x68) != 0)
            goto next;
        {
            void* sj = *(void**)(ch + 0x30);
            ((void(*)(void*, s32, s32, void*))(*(void***)sj)[6])(sj, 0, 0x2000, &out);
        }
        size = out.size;
        size = (size / 0x20) * 0x20;
        SJ_SplitChunk(&out, size, &out, &split);
        {
            void* sj = *(void**)(ch + 0x30);
            ((void(*)(void*, s32, void*))(*(void***)sj)[7])(sj, 0, &split);
        }
        if (size == 0)
            return;
        *(s32*)(cs + 0x48) = (u32)out.ptr;
        *(s32*)(cs + 0x4c) = out.size;
        *(s32*)((u8*)self + 0x70) = size >> 1;
        *(s32*)(ch + 0x68) = 1;
        memset(out.ptr, 0, size);
        DCFlushRange(out.ptr, size);
        if (*(s32*)(ch + 0x68) == 1) {
            void* obj;
            obj = *(void**)(ch + 0x30);
            ((void(*)(void*, s32, void*))(*(void***)obj)[8])(obj, 1, p48);
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
                criware_80399F4C(rna);
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
    s32 i;
    s32 hi;
    s32 lo;
    AXPBSRC src;
    if (self == NULL)
        return;
    if (sfreq == 0x7d1c) {
        RNAERR_CallErrFunc((const char*)(lbl_eu_80519150 + 0x4b5));
    }
    *(s32*)((u8*)self + 0x1c) = sfreq;
    hi = sfreq / 32000;
    lo = (sfreq << 8) / 125;
    for (i = 0; i < (s8)((u8*)self)[2]; i++) {
        GCRNA_LockCs();
        {
            void* e = *(void**)((u8*)self + 4 * i + 8);
            if (e != NULL) {
                src.ratioHi = (u16)hi;
                src.ratioLo = (u16)lo;
                src.currentAddressFrac = 0;
                src.last_samples[0] = 0;
                src.last_samples[1] = 0;
                src.last_samples[2] = 0;
                src.last_samples[3] = 0;
                AXSetVoiceSrcType(e, *(s32*)((u8*)self + 0xa0));
                AXSetVoiceSrc(e, &src);
            }
        }
        GCRNA_UnlockCs();
    }
}

void AXRNA_SetOutVol(void* self, s32 vol) {
    s32 i;
    s32 t;
    s32 v;
    if (self == NULL)
        return;
    t = vol & (vol >> 31);
    v = -960;
    if (t > -960)
        v = t;
    if (*(s32*)((u8*)self + 0x7c) == v)
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
    s32 p;
    s32 v;
    if (self == NULL)
        return;
    if (index >= (s8)((u8*)self)[2])
        return;
    p = (pan >= 0xf) ? 0xf : pan;
    v = (p <= -0xf) ? -0xf : p;
    if (*(s32*)((u8*)self + index * 4 + 0x80) == v)
        return;
    *(s32*)((u8*)self + index * 4 + 0x80) = v;
    GCRNA_LockCs();
    {
        void* e = *(void**)((u8*)self + index * 4 + 8);
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
    s32 t;
    s32 v;
    s32 i;
    s32* p;
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
    if (p[0xd0 / 4] == v)
        return;
    p[0xd0 / 4] = v;
    for (i = 0; i < (s8)((u8*)self)[2]; i++) {
        GCRNA_LockCs();
        {
            void* e = *(void**)((u8*)self + 4 * i + 8);
            if (e != NULL)
                MIXRmtSetFader(e, index, p[0xd0 / 4]);
        }
        GCRNA_UnlockCs();
    }
}

void criware_8039A8E0(void* self, u32 flags) {
    s32 i;
    u32 b4, b0, b1, b2, b3, b5;
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
    b4 = flags & 0x10;
    b0 = flags & 1;
    b1 = flags & 2;
    b2 = flags & 4;
    b3 = flags & 8;
    b5 = flags & 0xf;
    for (i = 0; i < (s8)((u8*)self)[2]; i++) {
        GCRNA_LockCs();
        {
            void* v = *(void**)((u8*)self + 4 * i + 8);
            if (v != NULL) {
            if (b4) {
                MIXSetFader(v, *(s32*)((u8*)self + 0x98));
            } else {
                MIXSetFader(v, -0x3c0);
            }
            if (b0)
                fader0 = *(s32*)((u8*)self + 0xd0);
            if (b1)
                fader1 = *(s32*)((u8*)self + 0xd4);
            if (b2)
                fader2 = *(s32*)((u8*)self + 0xd8);
            if (b3)
                fader3 = *(s32*)((u8*)self + 0xdc);
            if (b5) {
                MIXRmtSetVolumes(v, 0, fader0, fader1, fader2, fader3,
                                 -0x3c0, -0x3c0, -0x3c0, -0x3c0);
                AXSetVoiceRmtOn(v, 1);
            } else {
                AXSetVoiceRmtOn(v, 0);
            }
            }
        }
        GCRNA_UnlockCs();
    }
}
