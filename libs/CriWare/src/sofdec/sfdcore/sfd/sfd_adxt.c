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

void SFADXT_SetSpeed() {}

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

void SFADXT_ExecServer() {}

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

int sfadxt_AdjustSync(void *handle);

extern int ADXT_IsHeader(void *buf, int size, int *out);
extern int SFHDS_GetMuxVerNum(void *handle);
extern void *sfadxt_SearchAlign(void *handle, void *buf, int size);

void sfadxt_ExcludeHdr(void *handle, void *buf, int size, int *out_size) {
    int exclude = 0;

    *out_size = 0;
    {
        void *w = *(void **)((u8 *)handle + 0x20ac);
        if (size >= 288) {
            int hdr;
            if (ADXT_IsHeader(buf, size, &hdr)) {
                exclude = hdr;
            } else {
                if (SFHDS_GetMuxVerNum(handle) < 108) {
                    exclude = (int)sfadxt_SearchAlign(handle, buf, size) - (int)buf;
                }
            }
            *(void **)((u8 *)w + 0x3c) = (void *)sfadxt_AdjustSync;
            *out_size = exclude;
            *(u64 *)((u8 *)handle + 0x9D8) += (u64)(s64)(s32)exclude;
        }
    }
}

extern int ADXT_IsEndcode(void *self, int idx, int *out);
extern s32 SFLIB_SetErr(void *h, u32 err_code);

#pragma optimize_for_size on
void *sfadxt_SearchAlign(void *handle, void *buf, int size) {
    u8 *end = (u8 *)buf + size;
    u8 *p = (u8 *)buf;
    u8 *found = NULL;
    u8 *start = NULL;
    u8 *scan_end = (u8 *)buf + 0x24;
    s32 unit = 0x12;
    s32 flag = 0;
    u8 *q;
    s32 cnt;

    while (p < scan_end) {
        cnt = (s32)((u32)(end - p + 0x11) / (u32)unit);
        q = p;
        flag = 0;
        if (p < end) {
            do {
                int ec;
                if ((s8)q[0] >= 0) {
                    q += 0x12;
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
            } while (--cnt);
        }
        if (flag) {
            p += 2;
        } else {
            break;
        }
    }
    if (!flag) {
        return p;
    }
    if (start == NULL) {
        SFLIB_SetErr(handle, 0xFF000C0A);
        return (u8 *)buf;
    }
    p = start;
    return p;
}
#pragma optimize_for_size reset

extern int memcmp(const void *s1, const void *s2, u32 size);
extern u8 lbl_eu_8051C51C[];

void sfadxt_ExcludeSilence(void *handle, u8 *buf, int size, int *out_size) {
    void *w = *(void **)((u8 *)handle + 0x20ac);
    int skip = 0;

    *out_size = 0;
    if (SFHDS_GetMuxVerNum(handle) >= 108) {
        *(void **)((u8 *)w + 0x3c) = (void *)sfadxt_ExcludeHdr;
    } else {
        u8 *p = buf;
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
        {
            void *conv = NULL;
            if (*(void **)((u8 *)handle + 0x2670) != NULL &&
                *(s32 *)((u8 *)w + 0x40) <= 0) {
                conv = (u8 *)*(void **)((u8 *)handle + 0x2670) + 0xd0c;
            }
            if (conv != NULL) {
                int n = *(s32 *)((u8 *)conv + 0xc);
                *(s32 *)((u8 *)w + 0x38) += (skip / (n * 0x12)) << 5;
            }
        }
    }
}

void sfadxt_ChkTermFlg() {}

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
        if ((u32)ADXT_GetStat(adxt) >= 2) {
            *(s32 *)((u8 *)conv + 0x10) = ADXT_GetSfreq(adxt);
            *(s32 *)((u8 *)conv + 0x14) = ADXT_GetNumSmpl(adxt);
            *(s32 *)((u8 *)conv + 0xc) = ADXT_GetNumChan(adxt);
            *(s32 *)((u8 *)conv + 0x4) =
                (*(s32 *)((u8 *)conv + 0x10) * *(s32 *)((u8 *)conv + 0xc) * 8 +
                 *(s32 *)((u8 *)conv + 0x10) * *(s32 *)((u8 *)conv + 0xc)) /
                16;
            *(s32 *)((u8 *)conv + 0x8) = 1;
            *(s32 *)((u8 *)conv) = 1;
        }
    }
}

void SFADXT_Create() {}

void sfadxt_InitInf() {}

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

    if (SFTIM_ChkRegularTime(handle, out1, out2) == 0) {
        return 0;
    }
    if (*(s32 *)((u8 *)handle + 0x54) == 4) {
        u32 t1, t2;
        s64 cur1, cur2, tmr, unit;
        s32 result[4];

        lbl_eu_80606E08 = (u32)ADXT_GetStat(adxt);
        ADXT_GetTime(adxt, &t1, &t2);
        cur1 = (s64)(s32)t1;
        cur2 = (s64)(s32)t2;
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

extern int SFPLY_GetResetFlg(void);
extern void ADXT_Destroy(void *);
extern void ADXT_Stop(void *);
extern void UTY_FinishTmr(void);
extern u32 lbl_eu_80606E38[];
int SFADXT_Destroy(void *handle) {
    void *w = *(void **)((u8 *)handle + 0x20ac);
    void *adxt = *(void **)((u8 *)w);
    void *chunk = *(void **)((u8 *)w + 4);

    if (adxt == NULL) {
        return 0;
    }
    lbl_eu_80606DE8[0] = *(u32 *)((u8 *)w + 8);
    lbl_eu_80606DE8[1] = *(u32 *)((u8 *)w + 0xc);
    lbl_eu_80606DE8[2] = *(u32 *)((u8 *)w + 0x10);
    lbl_eu_80606DE8[3] = *(u32 *)((u8 *)w + 0x14);
    lbl_eu_80606DE8[4] = *(u32 *)((u8 *)w + 0x18);
    lbl_eu_80606DE8[5] = *(u32 *)((u8 *)w + 0x1c);
    lbl_eu_80606DE8[6] = *(u32 *)((u8 *)w + 0x20);
    if (SFPLY_GetResetFlg() == 1) {
        ADXT_Stop(*(void **)((u8 *)w));
        lbl_eu_80606E38[0x1f4 / 4] = (u32)adxt;
    } else {
        ADXT_Destroy(adxt);
    }
    {
        void **vt = *(void ***)chunk;
        ((void (*)(void *))vt[3])(chunk);
    }
    UTY_FinishTmr();
    return 0;
}

int SFADXT_RequestStop(void) { return 0x0; }

extern void ADXT_Pause(void *, int);
extern void SFTST_Pause(void *, u32);

s32 SFADXT_Start(void *handle) {
    void *w = *(void **)((u8 *)handle + 0x20ac);
    void *adxt = *(void **)((u8 *)w);
    void *tim = (u8 *)handle + 0x1098;

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
    void *adxt = *(void **)((u8 *)w);
    void *conv = NULL;
    void (*seekfn)(void *, u32, u32, u32);

    if (*(void **)((u8 *)handle + 0x2670) != NULL &&
        *(s32 *)((u8 *)w + 0x40) <= 0) {
        conv = (u8 *)*(void **)((u8 *)handle + 0x2670) + 0xd0c;
    }
    if (conv == NULL) {
        return 0;
    }
    if (*(s32 *)((u8 *)w + 0x34) != 0) {
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
    return 0;
}
