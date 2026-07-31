// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_mpvf
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void SFMPVF_InitPool() {}

void SFD_SetMpvParaTbl() {}

s32 sfmpvf_CheckMpvPara(void* self);

void* memcpy(void* dst, const void* src, size_t n);
s32 SFLIB_SetErr(void* h, u32 err_code);
extern void SFLIB_LockCs(void* cs);
extern void SFLIB_UnlockCs(void* cs);
extern u32 lbl_eu_80619B20[];

s32 SFMPVF_ReadGlobalMpvPara(void* self) {
    u64* s = (u64*)lbl_eu_80619B20;

    if (sfmpvf_CheckMpvPara(self) != 0) {
        return SFLIB_SetErr(0, 0xff000f15);
    }
    *(u64*)((u8*)self + 0x2780) = s[0];
    *(u64*)((u8*)self + 0x2788) = s[1];
    *(u64*)((u8*)self + 0x2790) = s[2];
    *(u64*)((u8*)self + 0x2798) = s[3];
    *(u32*)((u8*)self + 0x27a0) = *(u32*)((u8*)s + 0x20);
    memcpy((u8*)self + 0x27a4, (u32*)s + 0xa, 8);
    memcpy((u8*)self + 0x27ac, (u32*)s + 0xc, 0x40);
    return 0;
}

s32 SFMPVF_WriteGlobalMpvPara(void* self) {
    u32* g = lbl_eu_80619B20;

    *(u64*)((u8*)g + 0) = *(u64*)((u8*)self + 0x2780);
    *(u64*)((u8*)g + 8) = *(u64*)((u8*)self + 0x2788);
    *(u64*)((u8*)g + 0x10) = *(u64*)((u8*)self + 0x2790);
    *(u64*)((u8*)g + 0x18) = *(u64*)((u8*)self + 0x2798);
    *(u32*)((u8*)g + 0x20) = *(u32*)((u8*)self + 0x27a0);
    memcpy(g + 0xa, (u8*)self + 0x27a4, 8);
    memcpy(g + 0xc, (u8*)self + 0x27ac, 0x40);
    g[0x1c] = *(u32*)((u8*)self + 0x38f8);
    g[0x1d] = *(u32*)((u8*)self + 0x38fc);
    g[0x1e] = *(u32*)((u8*)self + 0x3900);
}

void SFD_CalcYccPlane() {}

void SFD_SetPicUsrBuf() {}

extern u32 lbl_eu_80619B20[];
void sfmpvf_SetPicUsrBuf(void* self, u32 w, u32 h, u32 stride);
void SFMPVF_ResetPicUsrBuf(void* self) {
    sfmpvf_SetPicUsrBuf(self, lbl_eu_80619B20[0x1c], lbl_eu_80619B20[0x1d], lbl_eu_80619B20[0x1e]);
}

void sfmpvf_SetPicUsrBuf(void* self, u32 w, u32 h, u32 stride) {}

void SFMPVF_InitPicUsr() {}

void SFMPVF_GetRead() {}

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
    s32 n;
    s32* p;
    s32 i;
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

void SFMPVF_HoldFrm() {}

void SFD_IsNextFrmReady() {}

void sfmpvf_SearchStbyFrm() {}

void sfmpvf_IsChkFirst() {}

void SFMPVF_ChkImageSize() {}
