// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_adxt
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

/* --- external ADX / SJ / SFD helpers used by this TU --- */

extern void ADXT_SetOutPan(void*);
void SFADXT_SetOutPan(void* self) {
    ADXT_SetOutPan(*(void**)(*(void**)((u8*)self + 0x20ac)));
}

extern void ADXT_GetOutPan(void*);
void SFADXT_GetOutPan(void* self) {
    ADXT_GetOutPan(*(void**)(*(void**)((u8*)self + 0x20ac)));
}

extern void ADXT_SetOutVol(void*);
void SFADXT_SetOutVol(void* self) {
    ADXT_SetOutVol(*(void**)(*(void**)((u8*)self + 0x20ac)));
}

void ADXT_GetOutVol(void* a);
void SFADXT_GetOutVol(void* self) {
    ADXT_GetOutVol(*(void**)(*(void**)((u8*)self + 0x20ac)));
}

/* pitch/speed conversion constants live in the f32/f64 literal pool */
extern u8 lbl_eu_8051C4E0[];
extern double log(double x);
extern void ADXT_SetTranspose(void *self, s32 transpose, s32 cent);

void SFADXT_SetSpeed(void *handle, s32 speed, s32 base) {
    void *w = *(void **)((u8 *)handle + 0x20ac);
    void *adxt = *(void **)w;
    int transpose;
    int cent;

    if (adxt == NULL) {
        return;
    }
    if (base == 0) {
        speed = 1;
        base = 1;
    }
    if (speed == base) {
        transpose = 0;
        cent = 0;
    } else {
        /* semitone shift from the frequency ratio, then round the
         * cent remainder down to a whole cent multiple of 100 */
        float x = *(float *)(lbl_eu_8051C4E0 + 0x50) *
                  (float)log((double)speed / (double)base);
        transpose = (int)(*(float *)(lbl_eu_8051C4E0 + 0x54) * x);
        cent = transpose * 100;
        {
            double rem = (double)x - (double)cent;
            int m = (int)(*(float *)(lbl_eu_8051C4E0 + 0x58) + (float)rem);
            if ((double)(*(float *)(lbl_eu_8051C4E0 + 0x58) + (float)rem) > (double)m) {
                m = m - 1;
            }
            cent = m;
        }
    }
    ADXT_SetTranspose(adxt, transpose, cent);
    SFTST_SetSpeed((u8 *)handle + 0x1098, speed, base);
}

extern u32 lbl_eu_80606DE8[7];

void SFD_SetAdxtPara(u32 *src) {
    u32 *dst = lbl_eu_80606DE8;
    dst[0] = src[0];
    dst[1] = src[1];
    u32 tmp = src[2] + 0x1f;
    dst[2] = tmp & ~0x1f;
    dst[3] = src[3];
    dst[4] = src[4];
    dst[5] = src[5];
    tmp = src[6] + 0x1f;
    dst[6] = tmp & ~0x1f;
}

extern void ADXT_Init(void);
extern void SFA_Init(void);
extern int UTY_MemsetDword(u32*, u32, int);

int SFADXT_Init(void) {
    ADXT_Init();
    SFA_Init();
    UTY_MemsetDword((u32*)lbl_eu_80606DE8, 0, 7);
    return 0;
}

int SFADXT_Finish() { SFA_Finish(); ADXT_Finish(); return 0; }

void SFCON_WriteTotSmplQue(void* a, u32 b, u32 c);
void criware_803BD518(void* a, void* b, void* c, u32 d, u32 e) {
    SFCON_WriteTotSmplQue(a, e, d);
}

/* per-stream server bookkeeping block shared with the EU hook object */
extern u32 lbl_eu_8056899C[];
extern void* lbl_eu_80606E34;

extern s32 SFSET_GetCond(void* self, u32 idx);
extern s32 SFBUF_GetTermFlg(void* self, s32 idx);
extern s32 SFBUF_RingGetRead(void* self, s32 idx, s32* out_a, s32* out_b);
extern s32 SFBUF_RingAddRead(void* self, s32 idx, s32 val);
extern void SFBUF_RingGetSj(void* self, s32 idx, void** out);
extern void SFBUF_GetFlowCnt(void* sj, s32* out_a, s32* out_b);
extern s64 SFBUF_UpdateFlowCnt(s32 hi, s32 lo, s32 val);
extern s32 SFBUF_GetPrepFlg(void* self, s32 idx);
extern void SFBUF_SetPrepFlg(void* self, s32 idx, s32 flg);
extern void ADXT_SetSvrFreq(void* adxt, s32 freq);

void sfadxt_ChkTermFlg(void* self, s32 flag_idx);
void sfadxt_SetAdxtHd(void* handle);

/* SFADXT_ExecServer - per-frame audio server callback: feeds stream data,
 * updates flow counters and propagates prepare/terminate flags. */
s32 SFADXT_ExecServer(void* handle) {
    s32 ret;
    void* w;
    void* adxt;
    s32 rd_a;
    s32 rd_b;
    s32 copied;
    s32 feed_out;
    void* sj;
    s32 fc_a;
    s32 fc_b;
    s32 prep_a;
    s32 prep_b;
    s32 sfreq;

    /* notify the external server hook (if registered) that a tick started */
    if (lbl_eu_80606E34 != NULL) {
        void** vt;
        lbl_eu_8056899C[3] = (u32)handle;
        vt = *(void***)lbl_eu_80606E34;
        ((void (*)(void*, void*))vt[9])(lbl_eu_80606E34, &lbl_eu_8056899C[1]);
    }

    if (SFSET_GetCond(handle, 6) == 0) {
        ret = 0;
    } else if (SFBUF_GetTermFlg(handle, *(s32*)((u8*)handle + 0x20b8)) == 1) {
        ret = 0;
    } else {

    w = *(void**)((u8*)handle + 0x20ac);
    copied = 0;
    ret = SFBUF_RingGetRead(handle, *(s32*)((u8*)handle + 0x20b4), &rd_a, &rd_b);
    if (ret == 0) {
        void (*feed)(void*, s32, s32, s32*);
        /* feed one chunk through the stream's read handler */
        copied = rd_b;
        feed = *(void (**)(void*, s32, s32, s32*))((u8*)w + 0x3c);
        feed(handle, rd_a, rd_b, &feed_out);
        ret = SFBUF_RingAddRead(handle, *(s32*)((u8*)handle + 0x20b4), copied);
    }
    if (ret == 0) {
        s64 upd;
        /* update the running flow counters for both buffer halves */
        SFBUF_RingGetSj(handle, *(s32*)((u8*)handle + 0x20b4), &sj);
        SFBUF_GetFlowCnt(sj, &fc_a, &fc_b);
        upd = SFBUF_UpdateFlowCnt(*(s32*)((u8*)handle + 0x9c8), *(s32*)((u8*)handle + 0x9cc), fc_a);
        *(s32*)((u8*)handle + 0x9c8) = (s32)(upd >> 32);
        *(s32*)((u8*)handle + 0x9cc) = (s32)upd;
        upd = SFBUF_UpdateFlowCnt(*(s32*)((u8*)handle + 0x9d0), *(s32*)((u8*)handle + 0x9d4), fc_b);
        *(s32*)((u8*)handle + 0x9d0) = (s32)(upd >> 32);
        *(s32*)((u8*)handle + 0x9d4) = (s32)upd;
        SFBUF_GetFlowCnt(*(void**)((u8*)w + 4), &fc_a, &fc_b);
        upd = SFBUF_UpdateFlowCnt(*(s32*)((u8*)handle + 0x9e0), *(s32*)((u8*)handle + 0x9e4), fc_a);
        *(s32*)((u8*)handle + 0x9e0) = (s32)(upd >> 32);
        *(s32*)((u8*)handle + 0x9e4) = (s32)upd;
        upd = SFBUF_UpdateFlowCnt(*(s32*)((u8*)handle + 0x9e8), *(s32*)((u8*)handle + 0x9ec), fc_b);
        *(s32*)((u8*)handle + 0x9e8) = (s32)(upd >> 32);
        *(s32*)((u8*)handle + 0x9ec) = (s32)upd;
    }

    prep_a = *(s32*)((u8*)handle + 0x20b8);
    prep_b = *(s32*)((u8*)handle + 0x20b4);
    if (SFBUF_GetPrepFlg(handle, prep_a) != 1 && SFBUF_GetPrepFlg(handle, prep_b) == 1 &&
        ADXT_GetStat(*(void**)*(void**)((u8*)handle + 0x20ac)) != 3) {
        /* decoder ready on the read side but not yet flagged: mark prepared */
        SFBUF_SetPrepFlg(handle, prep_a, 1);
    }

    sfadxt_ChkTermFlg(handle, copied);
    sfadxt_SetAdxtHd(handle);

    w = *(void**)((u8*)handle + 0x20ac);
    adxt = *(void**)w;
    sfreq = SFSET_GetCond(handle, 0x1b);
    if (*(s32*)((u8*)w + 0x44) != sfreq) {
        /* sample rate changed since last frame: push it to the decoder */
        *(s32*)((u8*)w + 0x44) = sfreq;
        ADXT_SetSvrFreq(adxt, sfreq);
    }

    /* report the flow-counter pairs to the external server hook */
    if (lbl_eu_80606E34 != NULL) {
        void** vt;
        lbl_eu_8056899C[0x74 / 4] = (u32)((u8*)handle + 0x9c8);
        lbl_eu_8056899C[0x80 / 4] = (u32)((u8*)handle + 0x9d0);
        lbl_eu_8056899C[0x8c / 4] = (u32)((u8*)handle + 0x9d8);
        vt = *(void***)lbl_eu_80606E34;
        ((void (*)(void*, void*))vt[9])(lbl_eu_80606E34, &lbl_eu_8056899C[0x6c / 4]);
    }
    }
    return ret;
}

/* --- sfadxt_CopyData: copy user data into the ring buffer --- */

typedef struct {
    void *ptr;
    int size;
} SJ_CHUNK;

extern void MEM_Copy(void *dst, void *src, int size);
extern void SJ_SplitChunk(const SJ_CHUNK *src, int size, SJ_CHUNK *dst1, SJ_CHUNK *dst2);
extern int sjrbf_GetChunk(void *self, int mode, int size, SJ_CHUNK *out);
extern int sjrbf_UngetChunk(void *self, int mode, SJ_CHUNK *chunk);
extern int sjrbf_PutChunk(void *self, int mode, SJ_CHUNK *chunk);

void sfadxt_CopyData(void *handle, void *buf, int size, int *out_size) {
    SJ_CHUNK info;
    SJ_CHUNK remain;
    void *w = *(void **)((u8 *)handle + 0x20ac);
    void *chunk = *(void **)((u8 *)w + 4);
    void **vt;
    int len;

    vt = *(void ***)chunk;
    ((int (*)(void *, int, int, SJ_CHUNK *))vt[6])(chunk, 0, *(int *)((u8 *)w + 8), &info);

    {
        void *src = info.ptr;
        len = size < info.size ? size : info.size;
        len = len < 0x19000 ? len : 0x19000;
        MEM_Copy(src, buf, len);
    }
    if (len == 0) {
        vt = *(void ***)chunk;
        ((int (*)(void *, int, SJ_CHUNK *))vt[7])(chunk, 0, &info);
    } else {
        SJ_SplitChunk(&info, len, &info, &remain);
        vt = *(void ***)chunk;
        ((int (*)(void *, int, SJ_CHUNK *))vt[8])(chunk, 1, &info);
        vt = *(void ***)chunk;
        ((int (*)(void *, int, SJ_CHUNK *))vt[7])(chunk, 0, &remain);
    }
    *(s32 *)((u8 *)w + 0x48) += len;
    *out_size = len;
}

s32 sfadxt_AdjustSync(void* handle, void* a2, s32 a3, s32* a4);

extern int ADXT_IsHeader(void *buf, int size, int *out);
extern int SFHDS_GetMuxVerNum(void *handle);
extern void *sfadxt_SearchAlign(void *handle, void *buf, int size);

void sfadxt_ExcludeHdr(void *handle, void *buf, int size, int *out_size) {
    void *w;
    int exclude = 0;

    *out_size = 0;
    w = *(void **)((u8 *)handle + 0x20ac);
    if (size >= 288) {
        int hdr;
        if (ADXT_IsHeader(buf, size, &hdr)) {
            exclude = hdr;
        } else if (SFHDS_GetMuxVerNum(handle) < 108) {
            exclude = (int)sfadxt_SearchAlign(handle, buf, size) - (int)buf;
        }
        *(void **)((u8 *)w + 0x3c) = (void *)sfadxt_AdjustSync;
        *out_size = exclude;
        *(u64 *)((u8 *)handle + 0x9D8) += (u64)(s64)(s32)exclude;
    }
}

extern int ADXT_IsEndcode(void *self, int idx, int *out);
extern s32 SFLIB_SetErr(void *h, u32 err_code);

#pragma optimize_for_size on
void *sfadxt_SearchAlign(void *handle, void *buf, int size) {
    u8 *scan_end;
    u8 *p;
    u8 *q;
    u8 *end;
    u8 *start;
    u8 *found;
    s32 flag;

    end = (u8 *)buf + size;
    p = (u8 *)buf;
    scan_end = (u8 *)buf + 0x24;
    flag = 0;
    found = NULL;
    start = NULL;

    while (p < scan_end) {
        flag = 0;
        for (q = p; q < end; q += 0x12) {
            int ec;
            if ((s8)q[0] >= 0) {
                continue;
            }
            flag = 1;
            if (ADXT_IsEndcode(q, 0x12, &ec)) {
                if (found < q) {
                    found = q;
                    start = p;
                }
            }
            break;
        }
        if (!flag) {
            break;
        }
        p += 2;
    }
    if (flag) {
        if (start == NULL) {
            SFLIB_SetErr(handle, 0xFF000C0A);
            return (u8 *)buf;
        }
        p = start;
    }
    return p;
}
#pragma optimize_for_size reset

extern int memcmp(const void *s1, const void *s2, u32 size);
extern u8 lbl_eu_8051C51C[];

void sfadxt_ExcludeSilence(void *handle, u8 *buf, int size, int *out_size) {
    void *w;
    void *p2670;
    void *conv;
    int skip;
    u8 *p;

    skip = 0;
    *out_size = 0;
    w = *(void **)((u8 *)handle + 0x20ac);
    if (SFHDS_GetMuxVerNum(handle) >= 108) {
        *(void **)((u8 *)w + 0x3c) = (void *)sfadxt_ExcludeHdr;
    } else {
        p = buf;
        while (skip < size - 0x12) {
            if (memcmp(p, lbl_eu_8051C51C, 0x12) != 0) {
                *(void **)((u8 *)w + 0x3c) = (void *)sfadxt_ExcludeHdr;
                break;
            }
            p += 0x12;
            skip += 0x12;
        }
        *out_size = skip;
        *(u64 *)((u8 *)handle + 0x9D8) += (u64)(s64)(s32)skip;
        /* conv info block lives at +0xd0c past the header struct */
        p2670 = *(void **)((u8 *)handle + 0x2670);
        conv = NULL;
        if (p2670 != NULL && *(s32 *)((u8 *)w + 0x40) <= 0) {
            conv = (u8 *)p2670 + 0xd0c;
        }
        {
            s32 ok = -1;
            s32 n;
            if (conv != NULL) {
                n = *(s32 *)((u8 *)conv + 0xc);
                ok = 0;
            }
            if (ok == 0) {
                *(s32 *)((u8 *)w + 0x38) += (skip / (n * 0x12)) << 5;
            }
        }
    }
}

extern s32 ADXT_GetErrCode(void*);
extern void SFTST_SetAdjFlg(void* self, s32 val);
extern void SFBUF_SetTermFlg(void* self, s32 idx, u32 flg);
extern s32 SFBUF_GetTermFlg(void* self, s32 idx);
extern s32 lbl_eu_80606E04;

extern void ADXT_TermSupply(void *self);

void sfadxt_ChkTermFlg(void* self, s32 flag_idx) {
    void* tim = (u8*)self + 0x1098;
    void* w = *(void**)((u8*)self + 0x20ac);
    void* adxt = *(void**)((u8*)w);
    s32 stat = ADXT_GetStat(adxt);
    s32 err = ADXT_GetErrCode(adxt);

    if (err != 0)
        lbl_eu_80606E04 = err;

    if (SFSET_GetCond(self, 26) == 0)
        err = 0;

    if (err != 0) {
        if (err == -1) {
            SFLIB_SetErr(self, 0xFF000C08);
        } else if (err == -2) {
            SFLIB_SetErr(self, 0xFF000C09);
        } else {
            SFLIB_SetErr(self, 0xFF000C07);
        }
    }

    if ((u32)(stat - 4) <= 1)
        SFTST_SetAdjFlg(tim, 0);

    if (stat == 5 || err != 0)
        SFBUF_SetTermFlg(self, *(s32*)((u8*)self + 0x20B8), 1);
    if (SFBUF_GetTermFlg(self, *(s32*)((u8*)self + 0x20B4)) == 1 && flag_idx == 0) {
        ADXT_TermSupply(adxt);
        if (*(s32*)((u8*)w + 0x48) == 0)
            SFBUF_SetTermFlg(self, *(s32*)((u8*)self + 0x20B8), 1);
    }
}

extern int ADXT_GetStat(void *);
extern int ADXT_GetSfreq(void *);
extern int ADXT_GetNumSmpl(void *);
extern int ADXT_GetNumChan(void *);

void sfadxt_SetAdxtHd(void *handle) {
    void *w = *(void **)((u8 *)handle + 0x20ac);
    void *conv;

    if (*(void **)((u8 *)handle + 0x2670) == NULL) {
        conv = NULL;
    } else if (*(s32 *)((u8 *)w + 0x40) > 0) {
        conv = NULL;
    } else {
        conv = (u8 *)*(void **)((u8 *)handle + 0x2670) + 0xd0c;
    }
    if (conv == NULL) {
        return;
    }
    if (*(s32 *)((u8 *)conv) != 0) {
        return;
    }
    {
        void *adxt = *(void **)((u8 *)w);
        u32 ok = (u32)ADXT_GetStat(adxt) > 1;
        if (ok) {
            s32 x;
            *(s32 *)((u8 *)conv + 0x10) = ADXT_GetSfreq(adxt);
            *(s32 *)((u8 *)conv + 0x14) = ADXT_GetNumSmpl(adxt);
            *(s32 *)((u8 *)conv + 0xc) = ADXT_GetNumChan(adxt);
            x = *(s32 *)((u8 *)conv + 0x10) * *(s32 *)((u8 *)conv + 0xc);
            {
                int y = x * 8;
                *(s32 *)((u8 *)conv + 0x4) = (y + x) / 16;
            }
            *(s32 *)((u8 *)conv + 0x8) = 1;
            *(s32 *)((u8 *)conv) = 1;
        }
        return;
    }
}

extern void UTY_InitTmr(s32 newId);
extern s32 SFPLY_GetResetFlg(void);
extern u32 lbl_eu_80606E38[];
extern void ADXT_Pause(void *self, s32 flg);
extern void SFTST_Pause(void *self, u32 val);
extern void SFTIM_SetTimeFn(void *self, void *fn, u32 idx);
extern void *SJRBF_Create(void *pool_mem, u32 buf_size, u32 xtr_size);
extern void *SJRBF_Create(void *pool_mem, u32 buf_size, u32 xtr_size);

s32 sfadxt_InitInf(void *handle, u32 *inf) {
    u8 *tim = (u8 *)handle + 0x1098;
    s64 tol[2];
    s64 exc[2];
    s64 adj[2];
    s64 poff[2];

    if (lbl_eu_80606DE8[2] == 0 || lbl_eu_80606DE8[6] == 0) {
        SFLIB_SetErr(NULL, 0xFF000C06);
        return 0;
    }

    inf[0] = 0;
    inf[1] = 0;
    inf[2] = lbl_eu_80606DE8[0];
    inf[3] = lbl_eu_80606DE8[1];
    inf[4] = lbl_eu_80606DE8[2];
    inf[5] = lbl_eu_80606DE8[3];
    inf[6] = lbl_eu_80606DE8[4];
    inf[7] = lbl_eu_80606DE8[5];
    inf[8] = lbl_eu_80606DE8[6];
    inf[9] = (u32)-1;
    inf[10] = 1;
    inf[11] = 0;
    inf[12] = 0;
    inf[13] = 0;
    inf[14] = 0;
    inf[15] = (u32)sfadxt_CopyData;
    inf[16] = 0;
    inf[17] = (u32)-1;
    inf[18] = 0;
    inf[19] = 0;

    tol[0] = SFSET_GetCond(handle, 0x3f);
    tol[1] = 1000000;
    exc[0] = SFSET_GetCond(handle, 0x40);
    exc[1] = 1000000;
    adj[0] = SFSET_GetCond(handle, 0x41);
    adj[1] = 1000000;
    poff[0] = SFSET_GetCond(handle, 0x42);
    poff[1] = 1000000;

    SFTST_Create(tim);
    SFTST_SetTstFlg(tim, SFSET_GetCond(handle, 0x48));
    SFTST_SetTolerance(tim, (s32 *)tol);
    SFTST_SetExcessErr(tim, (s32 *)exc);
    SFTST_SetAdjStart(tim, (s32 *)adj);
    SFTST_SetAdjPoff(tim, (s32 *)poff);
    SFTST_SetMovaveRange(tim, SFSET_GetCond(handle, 0x3e));
    SFTST_SetSpeed(tim, 1, 1);
    UTY_InitTmr(SFSET_GetCond(handle, 0x3d));
    return 0;
}

s32 sfadxt_GetTime(void *handle, int *out1, int *out2);
extern void ADXT_SetAutoRcvr(void *self, s32 flg);
extern void ADXT_EntryFltFunc(void *self, void *fltfn, void *handle);
extern void ADXT_StartSj(void *self);
extern void *ADXT_Create(void *a, void *b, s32 c);

s32 SFADXT_Create(void *handle) {
    u32 *inf;
    void *adxt;
    void *sj;

    if (SFSET_GetCond(handle, 6) == 0) {
        return 0;
    }
    inf = (u32 *)((u8 *)handle + 0x25c0);
    *(void **)((u8 *)handle + 0x20ac) = inf;
    sfadxt_InitInf(handle, inf);

    if (SFPLY_GetResetFlg() == 1) {
        adxt = (void *)lbl_eu_80606E38[0x7d];
    } else {
        adxt = ADXT_Create((void *)inf[5], (void *)inf[8], (s32)inf[7]);
    }
    if (adxt != NULL) {
        criware_eu_803878BC(adxt, NULL);
        ADXT_SetAutoRcvr(adxt, 0);
    }
    if (adxt == NULL) {
        SFLIB_SetErr(NULL, 0xFF000C04);
        return 0;
    }
    ADXT_EntryFltFunc(adxt, criware_803BD518, handle);

    sj = SJRBF_Create((void *)inf[4], inf[2], inf[3]);
    if (sj == NULL) {
        SFLIB_SetErr(NULL, 0xFF000C05);
        return 0;
    }
    inf[1] = (u32)sj;
    inf[0] = (u32)adxt;

    *(u32 *)((u8 *)handle + 0x21bc) = (u32)((u8 *)handle + 0x261c);
    *(u32 *)((u8 *)handle + 0x261c) = *(u32 *)((u8 *)adxt + 0xc);
    *(void **)((u8 *)handle + 0x2620) = (void *)SFADXT_SetOutPan;
    *(void **)((u8 *)handle + 0x2624) = (void *)SFADXT_GetOutPan;
    *(void **)((u8 *)handle + 0x2628) = (void *)SFADXT_SetOutVol;
    *(void **)((u8 *)handle + 0x262c) = (void *)SFADXT_GetOutVol;
    *(void **)((u8 *)handle + 0x2630) = (void *)SFADXT_SetSpeed;

    ADXT_StartSj(adxt);

    inf = *(u32 **)((u8 *)handle + 0x20ac);
    adxt = *(void **)inf;
    inf[11] = 1;
    ADXT_Pause(adxt, 1);
    SFTST_Pause((u8 *)handle + 0x1098, 1);
    SFTIM_SetTimeFn(handle, (void *)sfadxt_GetTime, 2);
    SFSET_SetCond(handle, 0xf, 2);
    return 0;
}

extern u32 lbl_eu_80606E08;
extern u64 UTY_GetTmr(void);
extern u64 UTY_GetTmrUnit(void);
extern void ADXT_GetTime(void *, u32 *, u32 *);
extern int SFTIM_ChkRegularTime(const void *, int *, int *);
extern void SFTST_Calc(void *, s32 *, void *, s32 *);

s32 sfadxt_GetTime(void *handle, int *out1, int *out2) {
    void *w = *(void **)((u8 *)handle + 0x20ac);
    void *adxt = *(void **)((u8 *)w);
    u8 *tim = (u8 *)handle + 0x1098;

    u32 t[2];
    s64 cur1, cur2, tmr, unit;
    s32 result[4];

    if (SFTIM_ChkRegularTime(handle, out1, out2) == 0) {
        return 0;
    }
    if (*(s32 *)((u8 *)handle + 0x54) == 4) {
        lbl_eu_80606E08 = (u32)ADXT_GetStat(adxt);
        ADXT_GetTime(adxt, &t[0], &t[1]);
        cur1 = (s64)(s32)t[0];
        cur2 = (s64)(s32)t[1];
        tmr = UTY_GetTmr();
        unit = UTY_GetTmrUnit();
        SFTST_Calc(tim, (s32 *)&cur1, (s32 *)&tmr, result);
        if (*(s32 *)((u8 *)w + 0x24) < result[1]) {
            *(s32 *)((u8 *)w + 0x24) = result[1];
            *(s32 *)((u8 *)w + 0x28) = result[3];
        }
    }
    *out1 = *(s32 *)((u8 *)w + 0x24);
    *out2 = *(s32 *)((u8 *)w + 0x28);
    return 0;
}

extern void ADXT_Destroy(void *);
extern void ADXT_Stop(void *);
extern void UTY_FinishTmr(void);
int SFADXT_Destroy(void *handle) {
    int ret;
    void *adxt;
    void *chunk;

    {
        void *w = *(void **)((u8 *)handle + 0x20ac);
        adxt = *(void **)w;
        chunk = ((void **)w)[1];

        if (adxt == NULL) {
            return 0;
        }
        lbl_eu_80606DE8[0] = ((u32 *)w)[2];
        lbl_eu_80606DE8[1] = ((u32 *)w)[3];
        lbl_eu_80606DE8[2] = ((u32 *)w)[4];
        lbl_eu_80606DE8[3] = ((u32 *)w)[5];
        lbl_eu_80606DE8[4] = ((u32 *)w)[6];
        lbl_eu_80606DE8[5] = ((u32 *)w)[7];
        lbl_eu_80606DE8[6] = ((u32 *)w)[8];
        if (SFPLY_GetResetFlg() == 1) {
            /* reloads the adxt pointer through the work area on the reset path */
            ADXT_Stop(*(void **)*(void **)((u8 *)handle + 0x20ac));
            lbl_eu_80606E38[0x7d] = (u32)adxt;
        } else {
            ADXT_Destroy(adxt);
        }
        ret = 0;
    }
    {
        void **vt = *(void ***)chunk;
        ((void (*)(void *))vt[3])(chunk);
    }
    UTY_FinishTmr();
    return ret;
}

int SFADXT_RequestStop(void) { return 0x0; }

s32 SFADXT_Start(void *handle) {
    void *adxt;
    void *w = *(void **)((u8 *)handle + 0x20ac);
    void *tim = (u8 *)handle + 0x1098;

    adxt = *(void **)((u8 *)w);

    *(s32 *)((u8 *)w + 0x2c) = 0;
    if (*(s32 *)((u8 *)handle + 0x5c) != 1) {
        ADXT_Pause(adxt, 0);
        SFTST_Pause(tim, 0);
    }
    return 0;
}

u32 SFADXT_Stop(void* self) {
    void* p = *(void**)((u8*)self + 0x20ac);
    ADXT_Stop(*(void**)((u8*)p));
    return 0;
}

extern void SFTIM_GetTimeOneFrmVideo(void *, int *, int *);
extern int ADXT_DiscardSmpl(void *, u32);
extern int UTY_MulDiv(int, int, int);
extern void SFTST_GoNextFrame(void *, s32 *);

s32 SFADXT_Pause(void *handle, s32 *pause) {
    void *w = *(void **)((u8 *)handle + 0x20ac);
    void *adxt = *(void **)((u8 *)w);

    switch (*pause) {
    case 0:
        *(s32 *)((u8 *)w + 0x30) = 0;
        if (*(s32 *)((u8 *)w + 0x2c) != 1) {
            ADXT_Pause(adxt, 0);
            SFTST_Pause((u8 *)handle + 0x1098, 0);
        }
        break;
    case 1:
        ADXT_Pause(adxt, 1);
        SFTST_Pause((u8 *)handle + 0x1098, 1);
        break;
    case 2:
        if ((u32)ADXT_GetStat(adxt) > 1) {
            s32 sfreq = ADXT_GetSfreq(adxt);
            int frm1, frm2;
            s64 m1, m2;
            s32 total;

            SFTIM_GetTimeOneFrmVideo(handle, &frm1, &frm2);
            m1 = (s64)UTY_MulDiv(sfreq, frm1, frm2);
            m2 = (s64)sfreq;
            total = *(s32 *)((u8 *)w + 0x30) + (s32)m1;
            *(s32 *)((u8 *)w + 0x30) = total - ADXT_DiscardSmpl(adxt, total);
            SFTST_GoNextFrame((u8 *)handle + 0x1098, (s32 *)&m1);
        }
        break;
    }
    return 0;
}

s32 SFLIB_SetErr(void* h, u32 err_code);
s32 SFADXT_GetWrite(void* h) {
    return SFLIB_SetErr(h, 0xff000c03);
}

s32 SFLIB_SetErr(void* h, u32 err_code);
s32 SFADXT_AddWrite(void* h) {
    return SFLIB_SetErr(h, 0xff000c03);
}

s32 SFLIB_SetErr(void* h, u32 err_code);
s32 SFADXT_GetRead(void* h) {
    return SFLIB_SetErr(h, 0xff000c03);
}

s32 SFLIB_SetErr(void* h, u32 err_code);
s32 SFADXT_AddRead(void* h) {
    return SFLIB_SetErr(h, 0xff000c03);
}

extern void *lbl_eu_80606E0C;
extern void ADXT_SetTimeOfst(void *, u32);

s32 SFADXT_Seek(void *handle) {
    void *w = *(void **)((u8 *)handle + 0x20ac);
    void *conv;
    void *p2670 = *(void **)((u8 *)handle + 0x2670);
    void (*seekfn)(void *, u32, u32, u32);

    if (p2670 == NULL) {
        conv = NULL;
    } else if (*(s32 *)((u8 *)w + 0x40) > 0) {
        conv = NULL;
    } else {
        conv = (u8 *)p2670 + 0xd0c;
    }
    if (conv == NULL) {
        return 0;
    }
    {
        s32 f34 = *(s32 *)((u8 *)w + 0x34);
        void *adxt = *(void **)((u8 *)w);
        if (f34 != 0) {
            return 0;
        }
        if (*(s32 *)((u8 *)conv) == 0) {
            return 0;
        }
        seekfn = (void (*)(void *, u32, u32, u32))lbl_eu_80606E0C;
        seekfn(adxt, *(u32 *)((u8 *)conv + 0xc), *(u32 *)((u8 *)conv + 0x10),
               *(u32 *)((u8 *)conv + 0x14));
        ADXT_SetTimeOfst(adxt, 0);
        *(s32 *)((u8 *)w + 0x34) = 1;
        *(void **)((u8 *)w + 0x3c) = (void *)sfadxt_ExcludeSilence;
    }
    return 0;
}

extern s32 SFTIM_GetAudioStartSample(void* self, s32 sampleRate);
extern s32 SFTIM_GetVideoStartSample(void* self, s32 mul, s32* out2);
extern void SFTIM_SetStartTime(void* self, u32 a, u32 b);
extern int ADXT_IsEndcode(void* self, int idx, int* out);
extern s32 ADXT_InsertSilence(void* adxt, s32 nchan, s32 nsmpl);

/* sfadxt_AdjustSync - adjust sync timing (0x280 bytes in retail). */
s32 sfadxt_AdjustSync(void* handle, void* a2, s32 a3, s32* a4) {
    u8* tim = (u8*)handle + 0xD98;
    void* w;
    void* conv;
    s32 err = 0;
    s32 chans;
    s32 freq;
    s32 audio;
    s32 video;
    s32 diff;
    s32 vstart;
    int ec;
    int ec2;
    s32 enc;
    s32 rem;
    s32 total;
    void* hdr;

    *a4 = 0;
    hdr = *(void**)((u8*)handle + 0x2670);
    w = *(void**)((u8*)handle + 0x20ac);
    if (hdr == NULL) {
        conv = NULL;
    } else if (*(s32*)((u8*)w + 0x40) > 0) {
        /* converter still busy: no info block available */
        conv = NULL;
    } else {
        /* conv info block lives at +0xd0c past the converter header */
        conv = (u8*)hdr + 0xd0c;
    }
    if (conv != NULL) {
        chans = *(s32*)((u8*)conv + 0xc);
        err = 0;
        freq = *(s32*)((u8*)conv + 0x10);
    }
    if (err != 0) {
        *(void**)((u8*)w + 0x3c) = (void*)sfadxt_CopyData;
        return;
    }
    audio = SFTIM_GetAudioStartSample(tim, freq);
    if (audio < 0)
        return;
    if (SFSET_GetCond(handle, 5) == 0) {
        /* no sync requested: reset the start time to the audio position */
        SFTIM_SetStartTime(tim, audio, freq);
        *(void**)((u8*)w + 0x3c) = (void*)sfadxt_CopyData;
        return;
    }
    video = SFTIM_GetVideoStartSample(tim, freq, &vstart);
    if (video < 0)
        return;
    SFTIM_SetStartTime(tim, video, freq);
    /* lead/lag of audio vs video, minus already-applied correction */
    diff = video - audio - *(s32*)((u8*)w + 0x38);
    total = 0;
    if (diff >= 0) {
        /* fast-forward: drop whole frames (one frame = chans * 18 blocks,
         * 32 samples per block pair) */
        s32 mul = chans * 18;
        s32 n = (diff / 32) * mul;
        enc = 0;
        if (n > 0) {
            void* p = a2;
            s32 max = n;
            s32 lim = (a3 / mul) * mul;
            if (lim < n)
                max = lim;
            total = 0;
            while (total < max) {
                if (ADXT_IsEndcode(p, 18, &ec) != 0) {
                    enc = 1;
                    break;
                }
                p = (u8*)p + 18;
                total += 18;
            }
            *(s32*)((u8*)w + 0x38) += (total / mul) << 5;
            rem = n - max;
        }
        if (rem <= 0 && vstart != 0) {
            *(void**)((u8*)w + 0x3c) = (void*)sfadxt_CopyData;
            enc = ADXT_IsEndcode(a2, a3, &ec2);
        }
    } else {
        /* audio leads video: insert silence to pad the gap */
        if (vstart != 0) {
            s32 neg = ((-diff) / 32) << 5;
            if (neg > 0) {
                s32 ins = ADXT_InsertSilence(*(void**)((u8*)handle + 0x20ac), chans, neg);
                neg -= ins;
                *(s32*)((u8*)w + 0x38) -= ins;
            }
            if (neg <= 0)
                *(void**)((u8*)w + 0x3c) = (void*)sfadxt_CopyData;
        }
    }
    if (enc != 0)
        SFSET_SetCond(handle, 6, 0);
    *a4 = total;
    *(u64*)((u8*)handle + 0x9D8) += (u64)(s64)total;
}
