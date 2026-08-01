// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_mpvf
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern u32 lbl_eu_80619B20[];
void* memset(void* dst, int val, size_t n);
void* memcpy(void* dst, const void* src, size_t n);

void SFMPVF_InitPool(void) {
    u8* p = (u8*)lbl_eu_80619B20;
    memset(&lbl_eu_80619B20[0], 0, 0x24);
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
    u32* p;
    u32* g = (u32*)lbl_eu_80619B20;
    u32* q;
    u32* r;
    s32 width;
    s32 i;
    p = g;
    width = (s32)p[0x1C / 4];
    if ((u32)(width - 1) > 0xF) {
        return -1;
    }
    if (p[0x10 / 4] == 0 || p[0x20 / 4] == 0) {
        q = g + 0xA;
        if (q[0] == 0) {
            return -1;
        }
        if (q[1] == 0) {
            return -1;
        }
        r = g + 0xC;
        for (i = 0; i < width; i++, r++) {
            if (*r == 0) {
                return -1;
            }
        }
    }
    return 0;
}

s32 SFLIB_SetErr(void* h, u32 err_code);
extern void SFLIB_LockCs(void* cs);
extern void SFLIB_UnlockCs(void* cs);
extern void* SFMPVF_HoldFrm(void* self);

s32 SFMPVF_ReadGlobalMpvPara(void* self) {
    u64* s = (u64*)lbl_eu_80619B20;
    u32* g = (u32*)lbl_eu_80619B20;

    if (sfmpvf_CheckMpvPara(self) != 0) {
        return SFLIB_SetErr(0, 0xff000f15);
    }
    *(u64*)((u8*)self + 0x2780) = s[0];
    *(u64*)((u8*)self + 0x2788) = s[1];
    *(u64*)((u8*)self + 0x2790) = s[2];
    *(u64*)((u8*)self + 0x2798) = s[3];
    *(u32*)((u8*)self + 0x27a0) = g[8];
    memcpy((u8*)self + 0x27a4, g + 0xa, 8);
    memcpy((u8*)self + 0x27ac, g + 0xc, 0x40);
    return 0;
}

s32 SFMPVF_WriteGlobalMpvPara(void* self) {
    u64* d = (u64*)lbl_eu_80619B20;
    u32* g = (u32*)lbl_eu_80619B20;

    d[0] = *(u64*)((u8*)self + 0x2780);
    d[1] = *(u64*)((u8*)self + 0x2788);
    d[2] = *(u64*)((u8*)self + 0x2790);
    d[3] = *(u64*)((u8*)self + 0x2798);
    g[8] = *(u32*)((u8*)self + 0x27a0);
    memcpy(g + 0xa, (u8*)self + 0x27a4, 8);
    memcpy(g + 0xc, (u8*)self + 0x27ac, 0x40);
    g[0x1c] = *(u32*)((u8*)self + 0x38f8);
    g[0x1d] = *(u32*)((u8*)self + 0x38fc);
    g[0x1e] = *(u32*)((u8*)self + 0x3900);
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

s32 sfmpvf_SetPicUsrBuf(void* self, u32 w, u32 h, u32 stride);
s32 SFLIB_CheckHn(void* h);

s32 SFD_SetPicUsrBuf(void* sfd, void* buf, s32 height, s32 count) {
    if (SFLIB_CheckHn(sfd) != 0) {
        return SFLIB_SetErr(0, 0xff000185);
    }
    return sfmpvf_SetPicUsrBuf(sfd, (u32)buf, height, count);
}

void SFMPVF_ResetPicUsrBuf(void* self) {
    sfmpvf_SetPicUsrBuf(self, lbl_eu_80619B20[0x1c], lbl_eu_80619B20[0x1d], lbl_eu_80619B20[0x1e]);
}

s32 sfmpvf_SetPicUsrBuf(void* self, u32 w, u32 h, u32 stride) {
    u32* p = (u32*)((u8*)self + 0x38f8);
    u32 n;
    u32 lim;
    u32 i;
    u32 cur;

    if (w == 0 || h == 0 || stride == 0) {
        for (i = 0; i < 0x94 / 4; i++) {
            p[i] = 0;
        }
        return 0;
    }
    if ((s32)h < *(s32*)((u8*)self + 0x30) + 3) {
        return SFLIB_SetErr(self, 0xff000f1d);
    }
    p[0] = w;
    p[1] = h;
    p[2] = stride;
    p[3] = w;
    p[4] = 0;
    cur = w + stride;
    n = h - 1;
    lim = (n >= 16) ? 16 : n;
    for (i = 0; i < lim; i++) {
        p[5 + 2 * i] = cur;
        p[6 + 2 * i] = 0;
        cur += stride;
    }
    return 0;
}

void SFMPVF_InitPicUsr(void* self) {
    u32* w = (u32*)self;
    u32 i;
    for (i = 0; i < 0x94 / 4; i++) {
        w[i] = 0;
    }
}

extern u32 SFSET_GetCond(void* self, u32 idx);
extern s32 SFTIM_IsGetFrmTime(void* self, void* frm);
extern s32 SFTIM_ExecCyclicFrameOutput(void* self);

s32 SFMPVF_GetRead(void* self, void** ppRead, u32* pFrmCnt, void (*callback)(void*, void*, void*)) {
    void* frm;
    void* entry;
    s32 i;
    u32 cnt = *(u32*)((u8*)self + 0x27ec);
    u8* base = (u8*)self + 0x27f8;

    if (SFSET_GetCond(self, 0xf) == 0 && *(s32*)((u8*)self + 0x5c) == 1) {
        *ppRead = NULL;
        return 0;
    }

    frm = SFMPVF_HoldFrm(self);
    if (frm == NULL) {
        *ppRead = NULL;
        return 0;
    }

    /* Search for the held frame's entry index */
    entry = NULL;
    for (i = 0; i < (s32)cnt; i++) {
        void* e = base + i * 0x110;
        if (e == frm) {
            entry = (u8*)self + 0x1758 + i * 0x88;
            break;
        }
    }

    *(u32*)entry = 1;
    *(void**)((u8*)self + 0x27f4) = frm;
    *ppRead = (u8*)entry + 8;

    /* Call the read callback */
    callback(self, frm, *ppRead);

    /* Copy frame info */
    *(u32*)((u8*)self + 0x1018) = *(u32*)((u8*)*ppRead + 0x14);
    *(u32*)((u8*)self + 0x101c) = *(u32*)((u8*)*ppRead + 0x18);

    if (SFTIM_IsGetFrmTime(self, *ppRead) == 0) {
        *ppRead = NULL;
        return 0;
    }

    if (SFTIM_ExecCyclicFrameOutput(self) == 0) {
        *ppRead = NULL;
        return 0;
    }

    if (*(s32*)((u8*)self + 0x68) == 2) {
        s32 frameCnt = *(s32*)((u8*)self + 0x6c);
        u32 newCnt = frameCnt + 1;
        *(u32*)((u8*)self + 0x6c) = newCnt;
        if ((s32)newCnt < 0) {
            *(u32*)((u8*)self + 0x6c) = 0;
        }
        *(u32*)((u8*)frm + 0x64) = frameCnt;
        *pFrmCnt = frameCnt;
    }
    return 0;
}

void SFMPVF_AddRead() {}

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
    if (*(u32*)self == 4) {
        *(u32*)self = 2;
        return;
    }
    *(u32*)self = 0;
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
    *outA = NULL;
    *outB = NULL;
    count = *(s32*)((u8*)h + 0x27ec);
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
        s32 r = (x >> 1) - (x & *(s32*)((u8*)a + 0xF8));
        return (u32)r >> 31;
    }
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
