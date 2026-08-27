// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_mpvf
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

u32 SFSET_GetCond(void* self, u32 idx);
s32 SFTIM_IsGetFrmTime(void* self, void* frm);
s32 SFTIM_ExecCyclicFrameOutput(void* self);

// Global MPV parameter block layout (shared with InitPool/SetMpvParaTbl below).
typedef struct SfdMpvPara {
    u64 head[4];     /* 0x00 */
    u32 field_0x20;
    u32 stride[2];   /* 0x28 */
    u32 buf[16];     /* 0x30 */
    u32 field_0x70[3];
} SfdMpvPara;

extern u32 lbl_eu_80619B20[];
void* memset(void* dst, int val, size_t n);
void* memcpy(void* dst, const void* src, size_t n);

void SFMPVF_InitPool(void) {
    u8* p = (u8*)lbl_eu_80619B20;
    memset(p + 0, 0, 0x24);
    memset(p + 0x28, 0, 8);
    memset(p + 0x30, 0, 0x40);
}

void SFD_SetMpvParaTbl(u32* src, u32* strides, u32* tbl) {
    u32* g = lbl_eu_80619B20;
    u32* p = g;
    u32* q = g + 0xA;
    u32* r = g + 0xC;
    s32 i = 0;
    s32 j, k;

    *(u64*)&p[0] = *(u64*)&src[0];
    *(u64*)&p[2] = *(u64*)&src[2];
    *(u64*)&p[4] = *(u64*)&src[4];
    *(u64*)&p[6] = *(u64*)&src[6];
    p[4] = 0;
    p[8] = 0;
    q[0] = (strides[0] + 0x1F) & ~0x1F;
    q[1] = (strides[1] + 0x1F) & ~0x1F;

    for (j = 0; j < 4; j++) {
        for (k = 0; k < 4; k++) {
            if (i < src[7]) {
                r[j * 4 + k] = (tbl[k] + 0x1F) & ~0x1F;
            } else {
                r[j * 4 + k] = 0;
            }
            i++;
        }
        tbl += 4;
    }
}

s32 sfmpvf_CheckMpvPara(void* self) {
    SfdMpvPara* tbl = (SfdMpvPara*)lbl_eu_80619B20;
    SfdMpvPara* p = tbl;
    u32* st;
    u32* b;
    /* head[2].lo = width @0x10, head[3].hi = frm_cnt @0x1c */
    s32 cnt = (s32)(p->head[3] >> 32);
    s32 i;

    if ((u32)(cnt - 1) > 0xF) {
        return -1;
    }
    if ((u32)p->head[2] == 0 || p->field_0x20 == 0) {
        st = tbl->stride;
        if (*st == 0) {
            return -1;
        }
        if (st[1] == 0) {
            return -1;
        }
        b = tbl->buf;
        for (i = 0; i < cnt; i++) {
            if (*b == 0) {
                return -1;
            }
            b++;
        }
    }
    return 0;
}

s32 SFLIB_SetErr(void* h, u32 err_code);
extern void SFLIB_LockCs(void* cs);
extern void SFLIB_UnlockCs(void* cs);
extern void* SFMPVF_HoldFrm(void* self);

// Shared layout of the global MPV parameter block and its in-handle copy
// (handle copy lives at self+0x2780).
typedef struct SfdMpvParaView {
    u64 head[4];     /* 0x00 */
    u32 field_0x20;
    u32 stride[2];   /* 0x28 */
    u32 buf[16];     /* 0x30 */
    u32 field_0x70[3];
} SfdMpvParaView;

s32 SFMPVF_ReadGlobalMpvPara(void* self) {
    SfdMpvParaView* tbl = (SfdMpvParaView*)lbl_eu_80619B20;
    SfdMpvParaView* dst = (SfdMpvParaView*)((u8*)self + 0x2780);

    if (sfmpvf_CheckMpvPara(self) != 0) {
        return SFLIB_SetErr(0, 0xff000f15);
    }
    dst->head[0] = tbl->head[0];
    dst->head[1] = tbl->head[1];
    dst->head[2] = tbl->head[2];
    dst->head[3] = tbl->head[3];
    dst->field_0x20 = tbl->field_0x20;
    memcpy(dst->stride, tbl->stride, sizeof(tbl->stride));
    memcpy(dst->buf, tbl->buf, sizeof(tbl->buf));
    return 0;
}

void SFMPVF_WriteGlobalMpvPara(void* self) {
    u8* s = (u8*)self;
    SfdMpvParaView* pv = (SfdMpvParaView*)lbl_eu_80619B20;
    u64* d = pv->head;

    /* walking stores resist CSE so the table stays in two base registers */
    *d++ = *(u64*)(s + 0x2780);
    *d++ = *(u64*)(s + 0x2788);
    *d++ = *(u64*)(s + 0x2790);
    *d++ = *(u64*)(s + 0x2798);
    pv->field_0x20 = *(u32*)(s + 0x27a0);
    memcpy(pv->stride, s + 0x27a4, 8);
    memcpy(pv->buf, s + 0x27ac, 0x40);
    pv->field_0x70[0] = *(u32*)(s + 0x38f8);
    pv->field_0x70[1] = *(u32*)(s + 0x38fc);
    pv->field_0x70[2] = *(u32*)(s + 0x3900);
}

void SFD_CalcYccPlane(s32 base, s32 w, s32 h, void* out) {
    s32 wa = (w + 15) / 16;
    s32 h15 = h + 15;
    *(s32*)((u8*)out + 8) = base;
    s32 w16 = wa * 16;
    s32 w32 = (w16 + 31) / 32;
    s32 t = (((((wa >> 27) & 1) + w16) >> 1) + 31) / 32;
    *(s16*)((u8*)out + 14) = (s16)(w32 * 32);
    s32 ha = h15 / 16;
    *(s16*)((u8*)out + 12) = (s16)(t * 32);
    s32 h16 = ha * 16;
    s32 u = (((ha >> 27) & 1) + h16) >> 1;
    *(s32*)((u8*)out + 0) = base + h16 * (w32 * 32);
    *(s32*)((u8*)out + 4) = base + h16 * (w32 * 32) + u * (t * 32);
}

s32 sfmpvf_SetPicUsrBuf(void* self, u32 w, u32 h, u32 stride) {
    u32* p = (u32*)((u8*)self + 0x38f8);
    s32 i;
    s32 hm1;

    if (w == 0 || h == 0 || stride == 0) {
        for (i = 0; i < 37; i++)
            p[i] = 0;
        return 0;
    }
    if ((s32)h < *(s32*)((u8*)self + 0x30) + 3)
        return SFLIB_SetErr(self, 0xff000f1d);
    {
        u32 cur = w + stride;
        hm1 = (s32)h - 1;
        p[0] = w;
        p[1] = h;
        p[2] = stride;
        p[3] = w;
        p[4] = 0;
        for (i = 0; i < (hm1 >= 16 ? 16 : hm1); i++) {
            p[5 + 2 * i] = cur;
            p[6 + 2 * i] = 0;
            cur += stride;
        }
    }
    return 0;
}
void SFMPVF_TermDec(void* self) { *(u32*)((u8*)self + 0x27F0) = 1; }

u32 SFMPVF_IsTermDec(void* self) { return *(u32*)((u8*)self + 0x27f0); }

void SFMPVF_SetGopStat(void* self) {
    s32* p = (s32*)((u8*)self + 0x27f8);
    s32 cnt = *(s32*)((u8*)self + 0x27ec);
    while (cnt-- > 0) {
        if (*p == 2 || *p == 4)
            *(p + 0x3F) = 1;
        p += 0x44;
    }
}

s32 SFMPVF_GetNumFrm(void* self) {
    void* cs;
    s32 n;
    s32* p;
    s32 cnt;
    SFLIB_LockCs(&cs);
    n = *(s32*)((u8*)self + 0x27ec);
    p = (s32*)((u8*)self + 0x27f8);
    cnt = 0;
    while (n-- > 0) {
        if ((*p == 2 || *p == 4) && *(p + 0x19) == -1) {
            cnt++;
        }
        p += 0x44;
    }
    if (*(s32*)((u8*)self + 0x27f0) == 1 && cnt == 0) {
        cnt = -1;
    }
    SFLIB_UnlockCs(&cs);
    return cnt;
}

void* SFMPVF_AllocFrm(void* self) {
    void* cs;
    s32* p;
    s32 i;
    s32 n;
    SFLIB_LockCs(&cs);
    n = *(s32*)((u8*)self + 0x27ec);
    p = (s32*)((u8*)self + 0x27f8);
    for (i = 0; i < n; i++, p += 0x44) {
        if (p[0] == 0 && p[1] == 0) {
            p[0] = 1;
            break;
        }
    }
    if (i == n) {
        p = NULL;
    }
    SFLIB_UnlockCs(&cs);
    return p;
}

void SFMPVF_FreeFrm(void* self) {
    if (self != NULL) {
        *(u32*)((u8*)self + 0x00) = 0;
    }
}

void SFMPVF_StbyFrm(void* self) {
    if (self != NULL) {
        *(u32*)((u8*)self + 0x00) = 2;
    }
}

void SFMPVF_RefStbyFrm(void* self) {
    if (self != NULL) {
        *(u32*)((u8*)self + 0x00) = 4;
    }
}

void SFMPVF_EndRefFrm(void* self) {
    if (self == NULL) return;
    if (*(s32*)self == 4) {
        *(s32*)self = 2;
        return;
    }
    *(s32*)self = 0;
}

s32 sfmpvf_IsChkFirst(void* a, void* b);

void* SFMPVF_HoldFrm(void* self) {
    void* cs;
    s32 i;
    s32 cnt;
    u8* p;
    void* best;
    s32 count;
    SFLIB_LockCs(&cs);
    count = *(s32*)((u8*)self + 0x27ec);
    p = (u8*)self + 0x27f8;
    best = NULL;
    cnt = 0;
    for (i = 0; i < count; i++, p += 0x110) {
        s32 st = *(s32*)p;
        if ((st == 2 || st == 4) && *(s32*)(p + 0x64) == -1) {
            cnt++;
            if (sfmpvf_IsChkFirst(best, p)) {
                best = p;
            }
        }
    }
    if (cnt == 1 && *(s32*)((u8*)self + 0x27f0) == 0 && *(s32*)((u8*)best + 0xfc) == 0) {
        best = NULL;
    }
    SFLIB_UnlockCs(&cs);
    return best;
}

void sfmpvf_SearchStbyFrm(void* h, void** outA, void** outB);
u32 SFSET_GetCond(void* self, u32 idx);
s32 SFTIM_IsGetFrmTimeTunit(void* self, s32 cmpA, s32 cmpB);

s32 SFD_IsNextFrmReady(void* h) {
    void* cs;
    void* frm;
    void* dummy;
    if (SFLIB_CheckHn(h) != 0) {
        SFLIB_SetErr(0, 0xff000183);
        return 0;
    }
    SFLIB_LockCs(&cs);
    if (*(s32*)((u8*)h + 0x54) != 4) {
        frm = NULL;
    } else {
        sfmpvf_SearchStbyFrm(h, &dummy, &frm);
        if (frm != NULL && SFSET_GetCond(h, 0xf) != 0 &&
            SFTIM_IsGetFrmTimeTunit(h, *(u32*)((u8*)frm + 0x44), *(u32*)((u8*)frm + 0x48)) == 0) {
            frm = NULL;
        }
    }
    SFLIB_UnlockCs(&cs);
    return frm != NULL;
}

void sfmpvf_SearchStbyFrm(void* h, void** outA, void** outB) {
    u8* p;
    s32 count;
    s32 cnt;
    s32 i;
    count = *(s32*)((u8*)h + 0x27ec);
    *outA = NULL;
    *outB = NULL;
    p = (u8*)h + 0x27f8;
    cnt = 0;
    for (i = 0; i < count; i++, p += 0x110) {
        s32 st = *(s32*)p;
        if ((st == 2 || st == 4) && *(s32*)(p + 0x64) == -1) {
            cnt++;
            if (sfmpvf_IsChkFirst(*outA, p)) {
                *outB = *outA;
                *outA = p;
            } else if (sfmpvf_IsChkFirst(*outB, p)) {
                *outB = p;
            }
        }
    }
    if (*(s32*)((u8*)h + 0x27f0) == 0) {
        cnt--;
    }
    if (cnt <= 0) {
        *outA = NULL;
        *outB = NULL;
    } else if (cnt == 1) {
        *outB = NULL;
    }
}

s32 sfmpvf_IsChkFirst(void* a, void* b) {
    if (a == NULL) {
        return 1;
    }
    if (*(s32*)((u8*)b + 0x54) < *(s32*)((u8*)a + 0x54)) {
        return 1;
    }
    if (*(s32*)((u8*)b + 0x54) > *(s32*)((u8*)a + 0x54)) {
        return 0;
    }
    if (*(s32*)((u8*)b + 0xF0) < *(s32*)((u8*)a + 0xF0)) {
        return 1;
    }
    if (*(s32*)((u8*)b + 0xF0) > *(s32*)((u8*)a + 0xF0)) {
        return 0;
    }
    if (*(s32*)((u8*)b + 0xF4) < *(s32*)((u8*)a + 0xF4)) {
        return 1;
    }
    if (*(s32*)((u8*)b + 0xF4) > *(s32*)((u8*)a + 0xF4)) {
        return 0;
    }
    {
        s32 x = *(s32*)((u8*)a + 0xF8) ^ *(s32*)((u8*)b + 0xF8);
        return ((x >> 1) - (x & *(s32*)((u8*)a + 0xF8))) < 0;
    }
}

// Frame-slot table layout: cnt/self+0x27f8 array of 0x110-byte frames,
// plus a parallel 0x88-stride "release" slot array rooted at self+0x1758.
typedef void (*SFMPVF_ReadCb)(void*, void*);

s32 SFMPVF_GetRead(void* self, void** out, u32* outVal, SFMPVF_ReadCb cb) {
    void* cs;
    void* frm;
    u8* slot;
    u8* p;
    s32 cnt;
    s32 i;
    s32 v;

    if (SFSET_GetCond(self, 0xf) == 0 && *(u32*)((u8*)self + 0x5c) != 1) {
        *out = NULL;
        return 0;
    }

    frm = SFMPVF_HoldFrm(self);
    if (frm == NULL) {
        *out = NULL;
        return 0;
    }

    // Find the held frame's index and its release slot.
    cnt = *(s32*)((u8*)self + 0x27ec);
    p = (u8*)self + 0x27f8;
    slot = NULL;
    for (i = 0; i < cnt; i++, p += 0x110) {
        if (p == (u8*)frm) {
            slot = (u8*)self + i * 0x88 + 0x1758;
            break;
        }
    }

    *(u32*)slot = 1;
    *(void**)((u8*)self + 0x27f4) = frm;
    *out = slot + 8;
    cb(self, frm);

    *(u32*)((u8*)self + 0x1018) = *(u32*)((u8*)frm + 0x14);
    *(u32*)((u8*)self + 0x101c) = *(u32*)((u8*)frm + 0x18);

    if (!SFTIM_IsGetFrmTime(self, *out)) {
        *out = NULL;
        return 0;
    }
    if (!SFTIM_ExecCyclicFrameOutput(self)) {
        *out = NULL;
        return 0;
    }

    if (*(s32*)((u8*)self + 0x68) == 2) {
        v = *(s32*)((u8*)self + 0x6c);
        *(s32*)((u8*)self + 0x6c) = v + 1;
        if (v + 1 < 0) {
            *(s32*)((u8*)self + 0x6c) = 0;
        }
        *(s32*)((u8*)frm + 0x64) = v;
        *outVal = v;
    }
    return 0;
}

s32 SFMPVF_AddRead(void* self, void* handle, void** out) {
    void* cs;
    u8* frm;
    u8* rel;
    u8* p;
    u8* q;
    s32 cnt;
    s32 i;
    s32 ret;

    SFLIB_LockCs(&cs);
    frm = NULL;
    rel = NULL;
    cnt = *(s32*)((u8*)self + 0x27ec);

    if (*(s32*)((u8*)self + 0x68) == 2) {
        // Playing: locate the frame whose display slot matches the requested
        // frame id in *out, then map it to its release slot.
        p = (u8*)self + 0x27f8;
        for (i = 0; i < cnt; i++, p += 0x110) {
            if (*(s32*)(p + 0x64) == *(s32*)out) {
                frm = p;
                break;
            }
        }
        if (frm == NULL) {
            ret = SFLIB_SetErr(self, 0xff000f1f);
            if (ret != 0) {
                SFLIB_UnlockCs(&cs);
                return ret;
            }
        }
        p = (u8*)self + 0x27f8;
        for (i = 0; i < cnt; i++, p += 0x110) {
            if (p == frm) {
                rel = (u8*)self + i * 0x88 + 0x1758;
                break;
            }
        }
    } else {
        // Not playing: handle points just past a header word that must be 1.
        rel = (u8*)handle - 8;
        if (*(s32*)(u8*)rel != 1) {
            ret = SFLIB_SetErr(self, 0xff000f0e);
            SFLIB_UnlockCs(&cs);
            return ret;
        } else {
            // Find which frame owns this handle.
            p = (u8*)self + 0x1758;
            q = (u8*)self + 0x27f8;
            for (i = 0; i < 16; i++, p += 0x88, q += 0x110) {
                if (*(void**)(p + 8) == handle) {
                    frm = q;
                    break;
                }
            }
            if (*(void**)((u8*)self + 0x27f4) != frm) {
                ret = SFLIB_SetErr(self, 0xff000f0f);
                SFLIB_UnlockCs(&cs);
                return ret;
            }
        }
    }

    *(u32*)rel = 0;
    if (frm != NULL) {
        if (*(s32*)frm == 4) {
            *(s32*)frm = 3;
        } else {
            *(s32*)frm = 0;
        }
        *(s32*)(frm + 0x64) = -1;
    }
    SFLIB_UnlockCs(&cs);
    return 0;
}

s32 SFMPVF_ChkImageSize(void* self, u32 w, u32 h) {
    s32 cw = *(s32*)((u8*)self + 0x91c);
    if (cw > 0 && (cw != (s32)w || *(s32*)((u8*)self + 0x920) != (s32)h)) {
        return -1;
    }
    {
        s32 (*cb)(void*) = *(s32(**)(void*))((u8*)self + 0xd7c);
        void* arg = *(void**)((u8*)self + 0xd80);
        if (cb != NULL && cb(arg) != 0) {
            return -1;
        }
    }
    return 0;
}
