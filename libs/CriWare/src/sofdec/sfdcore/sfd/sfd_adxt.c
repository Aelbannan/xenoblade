// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_adxt
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include "libs/CriWare/src/sofdec/sfdcore/sfd/sfd_types.h"
#include "libs/CriWare/src/adx/adxt/adx_tlk.h"

// Large section arrays for data diff - generated
__attribute__((aligned(8), used)) const unsigned char __large_rodata_for_CriWare_src_sofdec_sfdcore_sfd_[0x70] = {0x80, 0x3B, 0xFC, 0xBC, 0x80, 0x3B, 0xFC, 0xF8, 0x80, 0x3B, 0xFD, 0x2C, 0x80, 0x3C, 0x08, 0x3C, 0x80, 0x3C, 0x0D, 0x18, 0x80, 0x3C, 0x0E, 0x00, 0x80, 0x3C, 0x0E, 0x08, 0x80, 0x3C, 0x0E, 0x68, 0x80, 0x3C, 0x0E, 0x94, 0x80, 0x3C, 0x0F, 0xBC, 0x80, 0x3C, 0x0F, 0xC8, 0x80, 0x3C, 0x0F, 0xD4, 0x80, 0x3C, 0x0F, 0xE0, 0x80, 0x3C, 0x0F, 0xEC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0xD8, 0x67, 0x7D, 0x3C, 0x23, 0xD7, 0x0A, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x28, 0x63, 0x29, 0x43, 0x52, 0x49, 0x00, 0x00}; // .rodata 0x70 at 0x8051C4E0

/* --- external ADX / SJ / SFD helpers used by this TU --- */

extern void ADXT_SetOutPan(void*);
void SFADXT_SetOutPan(SfdHandle* self) {
    ADXT_SetOutPan(*(void**)self->trn[3].hdr);
}

extern void ADXT_GetOutPan(void*);
void SFADXT_GetOutPan(SfdHandle* self) {
    ADXT_GetOutPan(*(void**)self->trn[3].hdr);
}

extern void ADXT_SetOutVol(void*);
void SFADXT_SetOutVol(SfdHandle* self) {
    ADXT_SetOutVol(*(void**)self->trn[3].hdr);
}

void ADXT_GetOutVol(void* a);
void SFADXT_GetOutVol(SfdHandle* self) {
    ADXT_GetOutVol(*(void**)self->trn[3].hdr);
}

/* pitch/speed conversion constants live in the f32/f64 literal pool:
 *   +0x50: log(ratio) scale, +0x54: semitone scale, +0x58: cent scale,
 *   +0x60: hi-magic double used by the int->float conversions */
extern u8 lbl_eu_8051C4E0[];
extern double log(double x);
extern void ADXT_SetTranspose(void *self, s32 transpose, s32 cent);

/* Convert playback speed ratio to an ADX transpose (semitones) plus a cent
 * fine-adjustment. Constants come from the read-only literal pool:
 *   +0x50: 12/log(2) style scaling, +0x54: semitone scale,
 *   +0x58: cent scale, +0x60: int->float conversion magic (0x4330000080000000). */
void SFADXT_SetSpeed(SfdHandle* handle, s32 speed, s32 base) {
    /* literal-pool base kept in a callee-saved register across the log() call */
    float *tbl = (float *)lbl_eu_8051C4E0;
    void **w = (void**)&handle->trn[3].hdr;
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
        float x = tbl[0x14] *
                  (float)log((float)speed / (float)base);
        transpose = tbl[0x15] * x;
        cent = transpose * 100;
        {
            /* floor-style cent rounding: nudge down by one cent when the
             * scaled remainder overshoots its truncated value */
            int m = tbl[0x16] + (x - cent);
            if ((double)m > tbl[0x16] + (x - cent)) {
                cent = tbl[0x16] + (x - m) - 1;
            } else {
                cent = tbl[0x16] + (x - m);
            }
        }
    }
    ADXT_SetTranspose(adxt, transpose, cent);
    SFTST_SetSpeed(&handle->tim.tst, speed, base);
}

u32 sfadxt_para[7];

void SFD_SetAdxtPara(u32 *src) {
    u32 *dst = sfadxt_para;
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
    UTY_MemsetDword((u32*)sfadxt_para, 0, 7);
    return 0;
}

int SFADXT_Finish() { SFA_Finish(); ADXT_Finish(); return 0; }

void SFCON_WriteTotSmplQue(void* a, u32 b, u32 c);
void criware_803BD518(void* a, void* b, void* c, u32 d, u32 e) {
    SFCON_WriteTotSmplQue(a, e, d);
}

/* per-stream server bookkeeping block shared with the EU hook object */
extern u32 sTraceSfadxtExecServer[];
extern void* gpSfdCoreTraceCb;

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
extern int ADXT_GetStat(void* self);

void sfadxt_ChkTermFlg(SfdHandle* self, s32 flag_idx);
void sfadxt_SetAdxtHd(SfdHandle* handle);

/* SFADXT_ExecServer - per-frame audio server callback: feeds stream data,
 * updates flow counters and propagates prepare/terminate flags. */
s32 SFADXT_ExecServer(SfdHandle* handle) {
    s32 ret = 0;
    s32 prep_a;
    SfdAdxtInf* w;
    s32 copied;
    /* NOTE: slot layout targets retail frame (sj@0x08 .. rd_b@0x1c) */
    s32 sj;
    s32 fc_a;
    s32 fc_b;
    s32 feed_out;
    s32 rd_a;
    s32 rd_b;

    /* notify the external server hook (if registered) that a tick started */
    if (gpSfdCoreTraceCb != NULL) {
        void** vt;
        sTraceSfadxtExecServer[3] = (u32)handle;
        vt = *(void***)gpSfdCoreTraceCb;
        ((void (*)(void*, void*))vt[9])(gpSfdCoreTraceCb, &sTraceSfadxtExecServer[1]);
    }

    if (SFSET_GetCond(handle, 6) == 0) {
        ret = 0;
    } else if (SFBUF_GetTermFlg(handle, handle->trn[3].termIdx) == 1) {
        ret = 0;
    } else {

    w = (SfdAdxtInf*)handle->trn[3].hdr;
    copied = 0;
    ret = SFBUF_RingGetRead(handle, handle->trn[3].bufIdx, &rd_a, &rd_b);
    if (ret == 0) {
        void (*feed)(void*, s32, s32, s32*);
        /* feed one chunk through the stream's read handler */
        copied = rd_b;
        feed = (void (*)(void*, s32, s32, s32*))w->feed;
        feed(handle, rd_a, rd_b, &feed_out);
        ret = SFBUF_RingAddRead(handle, handle->trn[3].bufIdx, feed_out);
    }
    if (ret == 0) {
        s64 upd;
        /* advance the running flow counters for both ring halves */
        SFBUF_RingGetSj(handle, handle->trn[3].bufIdx, (void**)&sj);
        SFBUF_GetFlowCnt((void*)sj, &fc_a, &fc_b);
        upd = SFBUF_UpdateFlowCnt((s32)handle->flowVidAHi, (s32)handle->flowVidALo, fc_a);
        handle->flowVidALo = (s32)upd;
        handle->flowVidAHi = (s32)(upd >> 32);
        upd = SFBUF_UpdateFlowCnt((s32)handle->flowVidBHi, (s32)handle->flowVidBLo, fc_b);
        handle->flowVidBLo = (s32)upd;
        handle->flowVidBHi = (s32)(upd >> 32);
        SFBUF_GetFlowCnt(w->sj, &fc_a, &fc_b);
        upd = SFBUF_UpdateFlowCnt((s32)handle->flowAudAHi, (s32)handle->flowAudALo, fc_a);
        handle->flowAudALo = (s32)upd;
        handle->flowAudAHi = (s32)(upd >> 32);
        upd = SFBUF_UpdateFlowCnt((s32)handle->flowAudBHi, (s32)handle->flowAudBLo, fc_b);
        handle->flowAudBLo = (s32)upd;
        handle->flowAudBHi = (s32)(upd >> 32);
    }

    prep_a = handle->trn[3].termIdx;
    {
        /* read-side index held across the first GetPrepFlg call */
        s32 idx_read = handle->trn[3].bufIdx;
        if (SFBUF_GetPrepFlg(handle, prep_a) != 1 &&
            SFBUF_GetPrepFlg(handle, idx_read) == 1 &&
        ADXT_GetStat(*(void**)handle->trn[3].hdr) != 3) {
        /* decoder ready on the read side but not yet flagged: mark prepared */
        SFBUF_SetPrepFlg(handle, prep_a, 1);
        }
    }

    sfadxt_ChkTermFlg(handle, copied);
    sfadxt_SetAdxtHd(handle);

    w = handle->trn[3].hdr;
    {
        s32 sfreq = SFSET_GetCond(handle, 0x1b);
        void* adxt = *(void**)w;
        if (w->sfreq != sfreq) {
            /* sample rate changed since last frame: push it to the decoder */
            w->sfreq = sfreq;
            ADXT_SetSvrFreq(adxt, sfreq);
        }
    }

    /* report the flow-counter pairs to the external server hook */
    if (gpSfdCoreTraceCb != NULL) {
        void** vt;
        sTraceSfadxtExecServer[0x74 / 4] = (u32)(&handle->flowVidAHi);
        sTraceSfadxtExecServer[0x80 / 4] = (u32)(&handle->flowVidBHi);
        sTraceSfadxtExecServer[0x8c / 4] = (u32)(&handle->flowSkip);
        vt = *(void***)gpSfdCoreTraceCb;
        ((void (*)(void*, void*))vt[9])(gpSfdCoreTraceCb, &sTraceSfadxtExecServer[0x6c / 4]);
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

void sfadxt_CopyData(SfdHandle* handle, void *buf, int size, int *out_size) {
    SJ_CHUNK info;
    SJ_CHUNK remain;
    SfdAdxtInf *w = (SfdAdxtInf *)handle->trn[3].hdr;
    void *chunk = w->sj;
    void **vt;
    int len;

    vt = *(void ***)chunk;
    ((int (*)(void *, int, int, SJ_CHUNK *))vt[6])(chunk, 0, (int)w->field_0x08, &info);

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
    w->copyLen += len;
    *out_size = len;
}

extern int ADXT_IsHeader(void *buf, int size, int *out);
extern int SFHDS_GetMuxVerNum(void *handle);
extern void *sfadxt_SearchAlign(void *handle, void *buf, int size);

void sfadxt_AdjustSync(SfdHandle* handle, void *buf, s32 size, s32 *out_size);

void sfadxt_ExcludeHdr(SfdHandle* handle, void *buf, int size, int *out_size) {
    SfdAdxtInf *w;
    s32 exclude = 0;
    int hdr;

    *out_size = 0;
    w = (SfdAdxtInf *)handle->trn[3].hdr;
    if (size >= 0x120) {
        if (ADXT_IsHeader(buf, size, &hdr)) {
            exclude = hdr;
        } else {
            /* old mux versions require scanning for the frame start */
            exclude = SFHDS_GetMuxVerNum(handle) >= 0x6c
                          ? 0
                          : (s32)((u8 *)sfadxt_SearchAlign(handle, buf, size) - (u8 *)buf);
        }
        w->feed = (void *)sfadxt_AdjustSync;
        *out_size = exclude;
        /* extend the running total by the sign-extended excluded count */
        handle->flowSkip += (s64)exclude;
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
    scan_end = p + 0x24;
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

void sfadxt_ExcludeSilence(SfdHandle* handle, u8 *buf, int size, int *out_size) {
    SfdAvp *p2670;
    SfdAdxtSync *conv;
    SfdAdxtInf *wp;
    int skip;
    u8 *p;
    SfdAdxtInf *w;

    *out_size = 0;
    w = (SfdAdxtInf *)handle->trn[3].hdr;
    if (SFHDS_GetMuxVerNum(handle) >= 108) {
        w->feed = (void *)sfadxt_ExcludeHdr;
    } else {
        p = buf;
        for (skip = 0; skip < size - 0x12; skip += 0x12) {
            if (memcmp(p, lbl_eu_8051C51C, 0x12) != 0) {
                w->feed = (void *)sfadxt_ExcludeHdr;
                break;
            }
            p += 0x12;
        }
        *out_size = skip;
        *out_size = skip;
        /* work pointer re-read from the handle (matches retail reload) */
        wp = (SfdAdxtInf *)handle->trn[3].hdr;
        /* 64-bit byte-position accumulator, updated with sign-extended skip */
        handle->flowSkip += skip;
        p2670 = handle->see.avplay;
        /* conv info block lives at +0xd0c past the header struct */
        if (p2670 == NULL) {
            conv = NULL;
        } else {
            if (wp->field_0x40 > 0) {
                conv = NULL;
            } else {
                conv = &p2670->adxtSync;
            }
        }
        {
            s32 ok;
            s32 n;
            if (conv == NULL) {
                ok = -1;
            } else {
                n = conv->numChan;
                ok = 0;
            }
            if (ok == 0) {
                w->syncOffset += (skip / (n * 0x12)) << 5;
            }
        }
    }
}

extern s32 ADXT_GetErrCode(void*);
extern void SFTST_SetAdjFlg(void* self, s32 val);
extern void SFBUF_SetTermFlg(void* self, s32 idx, u32 flg);
extern s32 SFBUF_GetTermFlg(void* self, s32 idx);
s32 sfadxt_adxterr;

extern void ADXT_TermSupply(void *self);

void sfadxt_ChkTermFlg(SfdHandle* self, s32 flag_idx) {
    s32 stat;
    SfdAdxtInf* w;
    void* adxt;
    s32 err;
    SfdTst* tim;

    tim = &self->tim.tst;
    w = (SfdAdxtInf*)self->trn[3].hdr;
    adxt = w->adxt;
    stat = ADXT_GetStat(adxt);
    err = ADXT_GetErrCode(adxt);

    /* publish the raw decoder error code for the EU-side error hook */
    if (err != 0)
        sfadxt_adxterr = err;

    /* condition 26 gates whether decoder errors are surfaced at all */
    if (SFSET_GetCond(self, 26) == 0)
        err = 0;

    if (err != 0) {
        switch (err) {
        case -1:
            SFLIB_SetErr(self, 0xFF000C08);
            break;
        case -2:
            SFLIB_SetErr(self, 0xFF000C09);
            break;
        default:
            SFLIB_SetErr(self, 0xFF000C07);
            break;
        }
    }

    /* stats 4/5 mean the decoder is stopping or stopped: cancel sync adjust */
    if ((u32)(stat - 4) <= 1)
        SFTST_SetAdjFlg(tim, 0);

    if (stat == 5 || err != 0)
        SFBUF_SetTermFlg(self, self->trn[3].termIdx, 1);
    /* last feed finished and nothing is buffered: terminate supply */
    if (SFBUF_GetTermFlg(self, self->trn[3].bufIdx) == 1 && flag_idx == 0) {
        ADXT_TermSupply(adxt);
        if (w->copyLen == 0)
            SFBUF_SetTermFlg(self, self->trn[3].termIdx, 1);
    }
}

extern int ADXT_GetStat(void *);
extern int ADXT_GetSfreq(void *);
extern int ADXT_GetNumSmpl(void *);
extern int ADXT_GetNumChan(void *);

void sfadxt_SetAdxtHd(SfdHandle* handle) {
    SfdAdxtInf *w = (SfdAdxtInf *)handle->trn[3].hdr;
    SfdAdxtSync *conv;

    if (handle->see.avplay == NULL) {
        conv = NULL;
    } else if (w->field_0x40 > 0) {
        conv = NULL;
    } else {
        conv = &handle->see.avplay->adxtSync;
    }
    if (conv == NULL) {
        return;
    }
    if (conv->seek != 0) {
        return;
    }
    {
        void *adxt = w->adxt;
        u32 ok = (u32)ADXT_GetStat(adxt) > 1;
        if (ok) {
            s32 x;
            conv->sfreq = ADXT_GetSfreq(adxt);
            conv->numSmpl = ADXT_GetNumSmpl(adxt);
            conv->numChan = ADXT_GetNumChan(adxt);
            x = conv->sfreq * conv->numChan;
            {
                int y = x * 8;
                conv->byteRate = (y + x) / 16;
            }
            conv->timeUnit = 1;
            conv->seek = 1;
        }
        return;
    }
}

extern void UTY_InitTmr(s32 newId);
extern s32 SFPLY_GetResetFlg(void);
extern u32 sSfdGlob[];
extern void ADXT_Pause(void *self, s32 flg);
extern void SFTST_Pause(void *self, u32 val);
extern void SFTIM_SetTimeFn(void *self, void *fn, u32 idx);
extern void *SJRBF_Create(void *pool_mem, u32 buf_size, u32 xtr_size);

s32 sfadxt_InitInf(SfdHandle* handle, u32 *inf) {
    u32 *prm = sfadxt_para;
    SfdTst *tim;
    u32 v0;
    u32 v1;
    u32 copyfn;
    s64 tol[2];
    s64 exc[2];
    s64 adj[2];
    s64 poff[2];

    /* both buffer sizes must be configured before init can proceed */
    if (prm[2] == 0 || prm[6] == 0) {
        return SFLIB_SetErr(NULL, 0xFF000C06);
    }

    v0 = prm[0];
    copyfn = (u32)sfadxt_CopyData;
    v1 = prm[1];
    inf[3] = v1;
    inf[2] = v0;
    v0 = prm[2];
    v1 = prm[3];
    inf[5] = v1;
    inf[4] = v0;
    v0 = prm[4];
    v1 = prm[5];
    inf[7] = v1;
    inf[6] = v0;
    inf[8] = prm[6];
    tim = &handle->tim.tst;
    inf[0] = 0;
    inf[1] = 0;
    inf[9] = (u32)-1;
    inf[10] = 1;
    inf[11] = 0;
    inf[12] = 0;
    inf[13] = 0;
    inf[14] = 0;
    inf[15] = copyfn;
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

s32 sfadxt_GetTime(SfdHandle* handle, int *out1, int *out2);
extern void ADXT_SetAutoRcvr(void *self, s32 flg);
extern void ADXT_EntryFltFunc(void *self, void *fltfn, void *handle);
extern void ADXT_StartSj(void *self);
extern void *ADXT_Create(void *a, void *b, s32 c);

s32 sfadxt_InitInf(SfdHandle* handle, u32 *inf);

s32 SFADXT_Create(SfdHandle* handle) {
    void *sj;
    s32 ret;
    u32 *inf;
    void *adxt;

    if (SFSET_GetCond(handle, 6) == 0) {
        return 0;
    }
    inf = (u32 *)(&handle->adxtInf);
    handle->trn[3].hdr = inf;
    /* init failure aborts creation; the init result is propagated */
    ret = sfadxt_InitInf(handle, inf);
    if (ret != 0) {
        return ret;
    }

    /* note: branch layout matches retail - create on the non-reset path */
    if (SFPLY_GetResetFlg() != 1) {
        adxt = ADXT_Create((void *)inf[5], (void *)inf[8], (s32)inf[7]);
    } else {
        /* soft-reset path: reuse the parked decoder handle */
        adxt = (void *)sSfdGlob[0x7d];
    }
    if (adxt != NULL) {
        goto setup;
    }
    adxt = NULL;
    goto chk;
setup:
    criware_eu_803878BC(adxt, NULL);
    ADXT_SetAutoRcvr(adxt, 0);
chk:
    if (adxt == NULL) {
        return SFLIB_SetErr(NULL, 0xFF000C04);
    }
    ADXT_EntryFltFunc(adxt, criware_803BD518, handle);

    /* store happens unconditionally; MWCC sinks it below the branch */
    /* store happens unconditionally; MWCC sinks it below the branch */
    inf[1] = (u32)(sj = SJRBF_Create((void *)inf[4], inf[2], inf[3]));
    if (sj == NULL) {
        return SFLIB_SetErr(NULL, 0xFF000C05);
    }
    inf[0] = (u32)adxt;

    handle->trn[7].hdr = &handle->aoapIf;
    handle->aoapIf.setOutPan = (void (*)(void*))SFADXT_SetOutPan;
    handle->aoapIf.getOutPan = (void (*)(void*))SFADXT_GetOutPan;
    handle->aoapIf.setOutVol = (void (*)(void*, s32))SFADXT_SetOutVol;
    handle->aoapIf.getOutVol = (s32 (*)(void*))SFADXT_GetOutVol;
    handle->aoapIf.setSpeed = (void (*)(void*, s32, s32))SFADXT_SetSpeed;
    handle->aoapIf.field_0x00 = (u32)((ADXT_OBJ_*)adxt)->rna;

    ADXT_StartSj(adxt);

    inf = (u32 *)handle->trn[3].hdr;
    adxt = *(void **)inf;
    inf[11] = 1;
    ADXT_Pause(adxt, 1);
    SFTST_Pause(&handle->tim.tst, 1);
    SFTIM_SetTimeFn(handle, (void *)sfadxt_GetTime, 2);
    SFSET_SetCond(handle, 0xf, 2);
    return 0;
}

u32 sfadxt_stat;
extern u64 UTY_GetTmr(void);
extern u64 UTY_GetTmrUnit(void);
extern void ADXT_GetTime(void *, u32 *, u32 *);
extern int SFTIM_ChkRegularTime(const void *, int *, int *);
extern void SFTST_Calc(void *, s32 *, void *, s32 *);

s32 sfadxt_GetTime(SfdHandle* handle, int *out1, int *out2) {
    SfdAdxtInf *w = (SfdAdxtInf *)handle->trn[3].hdr;
    void *adxt = *(void **)w;
    SfdTst *tim = &handle->tim.tst;

    u32 t[2];
    s64 cur1, cur2, tmr, unit;
    s32 result[4];

    if (SFTIM_ChkRegularTime(handle, out1, out2) == 0) {
        return 0;
    }
    if ((s32)handle->errorState == 4) {
        sfadxt_stat = (u32)ADXT_GetStat(adxt);
        ADXT_GetTime(adxt, &t[0], &t[1]);
        cur1 = (s64)(s32)t[0];
        cur2 = (s64)(s32)t[1];
        tmr = UTY_GetTmr();
        unit = UTY_GetTmrUnit();
        SFTST_Calc(tim, (s32 *)&cur1, (s32 *)&tmr, result);
        if (w->time < result[1]) {
            w->time = result[1];
            w->timeUnit = result[3];
        }
    }
    *out1 = w->time;
    *out2 = w->timeUnit;
    return 0;
}

extern void ADXT_Destroy(void *);
extern void ADXT_Stop(void *);
extern void UTY_FinishTmr(void);
/* parameter block / stream-work views used by Destroy: three word pairs plus
 * one trailing word are saved back to the global parameter block */
typedef struct {
    u32 lo;
    u32 hi;
} SFADXT_PAIR;

typedef struct {
    SFADXT_PAIR p[3];
    u32 d;
} SFADXT_PARA;

int SFADXT_Destroy(SfdHandle* handle) {
    int ret;
    void *adxt;
    void *chunk;
    SFADXT_PARA *para;
    SfdAdxtInf *w = (SfdAdxtInf *)handle->trn[3].hdr;

    adxt = w->adxt;
    chunk = w->sj;
    if (adxt == NULL) {
        return 0;
    }
    /* stash the stream configuration back into the global parameter block */
    para = (SFADXT_PARA *)sfadxt_para;
    /* the work area mirrors three word pairs of the parameter block */
    para->p[0] = *(SFADXT_PAIR *)&w->field_0x08;
    para->p[1] = *(SFADXT_PAIR *)&w->pool;
    para->p[2] = *(SFADXT_PAIR *)&w->field_0x18;
    para->d = (u32)w->field_0x20;
    if (SFPLY_GetResetFlg() != 1) {
        ADXT_Destroy(adxt);
        ret = 0;
    } else {
        /* reset path: only stop the decoder and park it for reuse after the
         * reload; the work area is re-read through the handle here */
        ADXT_Stop(((SfdAdxtInf *)handle->trn[3].hdr)->adxt);
        sSfdGlob[0x7d] = (u32)adxt;
        ret = 0;
    }
    ((void (*)(void *))(*(void ***)chunk)[3])(chunk);
    UTY_FinishTmr();
    return ret;
}

int SFADXT_RequestStop(void) { return 0x0; }

s32 SFADXT_Start(SfdHandle* handle) {
    void *adxt;
    SfdAdxtInf *w = (SfdAdxtInf *)handle->trn[3].hdr;
    SfdTst *tim = &handle->tim.tst;

    adxt = w->adxt;

    w->field_0x2c = 0;
    if (handle->field_0x005c != 1) {
        ADXT_Pause(adxt, 0);
        SFTST_Pause(tim, 0);
    }
    return 0;
}

u32 SFADXT_Stop(SfdHandle* self) {
    SfdAdxtInf* w = (SfdAdxtInf*)self->trn[3].hdr;
    ADXT_Stop(w->adxt);
    return 0;
}

extern void SFTST_GoNextFrame(void *, s32 *);

/* pause/unpause handler. mode 2 (partial resume while playing) advances the
 * decoder past the samples that elapsed during the pause window so the
 * stream timer stays in sync with video. */
s32 SFADXT_Pause(SfdHandle* handle, s32 *pause) {
    void *adxt = ((SfdAdxtInf *)handle->trn[3].hdr)->adxt;
    SfdTst *tim;

    switch (*pause) {
    case 0: {
        /* full unpause: reset the pending-sample count unless the stream
         * timer is already flagged as running */
        SfdAdxtInf *w = (SfdAdxtInf *)handle->trn[3].hdr;
        w->total = 0;
        tim = &handle->tim.tst;
        if (w->field_0x2c != 1) {
            ADXT_Pause(w->adxt, 0);
            SFTST_Pause(tim, 0);
        }
        break;
 }
    case 1:
        ADXT_Pause(adxt, 1);
        SFTST_Pause(&handle->tim.tst, 1);
        break;
    case 2:
        /* decoder mid-stream: drop the samples covered by one video frame */
        if ((u32)ADXT_GetStat(adxt) > 1) {
            SfdAdxtInf *w = (SfdAdxtInf *)handle->trn[3].hdr;
            s32 sfreq = ADXT_GetSfreq(adxt);
            int frm2;
            int frm1;
            s32 total;
            s32 mul;
            s64 go[2];

            SFTIM_GetTimeOneFrmVideo(handle, &frm1, &frm2);
            mul = UTY_MulDiv(sfreq, frm1, frm2);
            total = mul + w->total;
            /* keep whatever the decoder could not discard */
            w->total = total - ADXT_DiscardSmpl(adxt, total);            go[0] = (s64)mul;
            go[1] = (s64)sfreq;
            SFTST_GoNextFrame(&handle->tim.tst, (s32 *)go);
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

// name-guess: JP map has only lbl_8060A2C4; SFADXT_Seek stores this callback.
static void (*sfadxt_seekfn)(void *, u32, u32, u32);
extern void ADXT_SetTimeOfst(void *, u32);

s32 SFADXT_Seek(SfdHandle* handle) {
    SfdAdxtInf *w = (SfdAdxtInf *)handle->trn[3].hdr;
    SfdAdxtSync *conv;
    SfdAvp *p2670 = handle->see.avplay;
    void (*seekfn)(void *, u32, u32, u32);

    if (p2670 == NULL) {
        conv = NULL;
    } else if (w->field_0x40 > 0) {
        conv = NULL;
    } else {
        conv = &p2670->adxtSync;
    }
    if (conv == NULL) {
        return 0;
    }
    {
        s32 f34 = w->field_0x34;
        void *adxt = w->adxt;
        if (f34 != 0) {
            return 0;
        }
        if (conv->seek == 0) {
            return 0;
        }
        seekfn = sfadxt_seekfn;
        seekfn(adxt, (u32)conv->numChan, (u32)conv->sfreq,
               (u32)conv->numSmpl);
        ADXT_SetTimeOfst(adxt, 0);
        w->field_0x34 = 1;
        w->feed = (void *)sfadxt_ExcludeSilence;
    }
    return 0;
}

extern s32 SFTIM_GetAudioStartSample(void* self, s32 sampleRate);
extern s32 SFTIM_GetVideoStartSample(void* self, s32 mul, s32* out2);
extern void SFTIM_SetStartTime(void* self, u32 a, u32 b);
extern int ADXT_IsEndcode(void* self, int idx, int* out);
extern s32 ADXT_InsertSilence(void* adxt, s32 nchan, s32 nsmpl);

/* sfadxt_AdjustSync - adjust sync timing (0x280 bytes in retail). */
void sfadxt_AdjustSync(SfdHandle* handle, void* a2, s32 a3, s32* a4) {
    u8* tim = (u8*)&handle->tim;
    SfdAdxtInf* w;
    s32 freq;
    s32 chans;
    s32 video;
    s32 audio;
    s32 diff;
    s32 vstart;
    int ec;
    int ec2;
    s32 total;
    s32 enc;
    SfdAvp* hdr;
    SfdAdxtSync* conv;
    s32 err;

    *a4 = 0;
    hdr = handle->see.avplay;
    w = (SfdAdxtInf*)handle->trn[3].hdr;
    conv = hdr != NULL
               ? (w->field_0x40 <= 0 ? &hdr->adxtSync : NULL)
               : NULL;
    if (conv != NULL) {
        chans = conv->numChan;
        err = 0;
        freq = conv->sfreq;
    } else {
        err = -1;
    }
    if (err != 0) {
        w->feed = (void*)sfadxt_CopyData;
        return;
    }
    audio = SFTIM_GetAudioStartSample(tim, freq);
    if (audio < 0)
        return;
    if (SFSET_GetCond(handle, 5) == 0) {
        /* no sync requested: reset the start time to the audio position */
        SFTIM_SetStartTime(tim, audio, freq);
        w->feed = (void*)sfadxt_CopyData;
        return;
    }
    video = SFTIM_GetVideoStartSample(tim, freq, &vstart);
    if (video < 0)
        return;
    SFTIM_SetStartTime(tim, video, freq);
    /* lead/lag of audio vs video, minus already-applied correction */
    total = 0;
    diff = video - audio - w->syncOffset;
    if (diff >= 0) {
        /* fast-forward: drop whole frames (one frame = chans * 18 blocks,
         * 32 samples per block pair). video/freq/audio are reused
         * as frame-size / drop-count(+remainder) / cap. */
        video = chans * 18;
        freq = (diff / 32) * video;
        enc = 0;
        if (freq > 0) {
            s32 lim = (a3 / video) * video;
            audio = freq;
            if (lim < audio)
                audio = lim;
            tim = (u8*)a2;
            enc = 0;
            total = 0;
            while (total < audio) {
                if (ADXT_IsEndcode(tim, 18, &ec) != 0) {
                    enc = 1;
                    break;
                }
                tim += 18;
                total += 18;
            }
            w->syncOffset += (total / video) << 5;
            freq = freq - audio;
        }
        if (freq <= 0 && vstart != 0) {
            w->feed = (void*)sfadxt_CopyData;
            enc = ADXT_IsEndcode(a2, a3, &ec2);
        }
    } else {
        /* audio leads video: insert silence to pad the gap */
        if (vstart != 0) {
            s32 neg = ((-diff) / 32) << 5;
            if (neg > 0) {
                /* work pointer deliberately re-read through the handle */
                void* adxt = *(void**)handle->trn[3].hdr;
                s32 ins = ADXT_InsertSilence(adxt, chans, neg);
                neg -= ins;
                w->syncOffset -= ins;
            }
            if (neg <= 0)
                w->feed = (void*)sfadxt_CopyData;
        }
    }
    if (enc != 0)
        SFSET_SetCond(handle, 6, 0);
    *a4 = total;
    /* extend the running byte total by the sign-extended sample count */
    handle->flowSkip += (s64)total;
}
