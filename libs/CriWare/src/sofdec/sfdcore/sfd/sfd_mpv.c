// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_mpv
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

s32 MPV_SetCond(void* mpv, s32 cond, s32 val);
s32 MPV_GetCond(void* mpv, s32 cond, s32* out);
s32 SFLIB_CheckHn(void* h);
s32 SFLIB_SetErr(void* h, u32 err_code);

s32 SFD_SetMpvCond(void* h, s32 cond, s32 val) {
    void* mpv;
    if (h == NULL) {
        mpv = NULL;
    } else {
        if (SFLIB_CheckHn(h) != 0) {
            return SFLIB_SetErr(0, 0xff000181);
        }
        mpv = **(void***)((u8*)h + 0x2068);
    }
    if (cond == 5) {
        val = 0;
    }
    if (MPV_SetCond(mpv, cond, val) != 0) {
        return SFLIB_SetErr(h, 0xff000f12);
    }
    return 0;
}

s32 SFMPV_SaveCond(void* h, s32* conds, u32 count) {
    void* mpv;
    s32 i;
    s32 max;
    u32 n;
    mpv = **(void***)((u8*)h + 0x2068);
    if (mpv == NULL) {
        return 0;
    }
    n = count >> 2;
    max = 0x10;
    if (n <= 0x10) {
        max = n;
    }
    for (i = 0; i < max; i++) {
        MPV_GetCond(mpv, i, conds + i);
    }
    return max;
}

s32 SFMPV_RestoreCond(void* h, s32* conds, s32 count) {
    u32 p = *(u32*)((u8*)h + 0x2068);
    void* mpv = *(void**)(void*)p;
    if (mpv != NULL) {
        s32 i;
        for (i = 0; i < count; i++) {
            MPV_SetCond(mpv, i, conds[i]);
        }
    }
}

void MPV_SetMbCb(void* p, u32 a, u32 b, u32 c);
void SFD_SetMbCb(void* self, u32 a, u32 b, u32 c) {
    void* arg1 = *(void**)(*(void**)((u8*)self + 0x2068));
    ((u32*)self)[0xd8c / 4] = a;
    ((u32*)self)[0xd88 / 4] = c;
    ((u32*)self)[0xd84 / 4] = b;
    MPV_SetMbCb(arg1, a, b, c);
}

s32 MPV_SetErrFunc(u32 a, void* fn, u32 b);
s32 MPV_Init(u32 a, void* b);
extern u32 lbl_eu_80607AF8;
extern u32 lbl_eu_80619B18;
void sfmpv_ErrFn(s32 val, u32 err);
void SFMPVF_InitPool();
s32 fn_803C34F8(s32 val, u32 err_code);

s32 SFMPV_Init(void) {
    s32 r;
    u32 zero = 0;
    if (MPV_SetErrFunc(0, sfmpv_ErrFn, 0) != 0) {
        return SFLIB_SetErr(0, 0xff000f0b);
    }
    r = MPV_Init(8, &lbl_eu_80607AF8);
    if (r != 0) {
        return fn_803C34F8(0, ((u32)r + 0xfd0000 == 0xff05) ? 0xff000f13 : 0xff000f01);
    }
    SFMPVF_InitPool();
    lbl_eu_80619B18 = zero;
    return 0;
}

int SFMPV_Finish(void) {
    MPV_Finish();
    return 0;
}

void SFMPV_ExecServer() {}

s32 MPV_DecodePicAtr(u32 handle, void* in, void* out);
void sfmpv_ProcessAuxShc(void* self) {
    typedef struct SfdAtr {
        u32 w;
        u32 h;
    } SfdAtr;
    SfdAtr atr;
    u32 out;
    u32 handle;
    void* shc = *(void**)((u8*)self + 0x2068);
    atr.w = *(u32*)((u8*)self + 0xd90);
    handle = *(u32*)shc;
    atr.h = *(u32*)((u8*)self + 0xd94);
    if (atr.w != 0) {
        if (atr.h != 0) {
            if (*(s32*)((u8*)shc + 8) == 0xc0) {
                if (MPV_DecodePicAtr(handle, &atr, &out) == 0) {
                    *(u32*)((u8*)shc + 4) = 2;
                    *(u32*)((u8*)shc + 8) = 0xc8;
                }
            }
        }
    }
}

void sfmpv_IsVbvEnough() {}

void sfmpv_GetActiveSize() {}

void sfmpv_NeedSafeDlmRefresh() {}

void sfmpv_SearchDelim() {}

void sfmpv_BsearchDelim() {}

void sfmpv_DecodeOneUnit() {}

void sfmpv_ConcatSub() {}

void sfmpv_DecodePicAtr() {}

void sfmpv_ComplementPts() {}

void sfmpv_CalcRepeatField() {}

void sfmpv_DoReformTc() {}

void sfmpv_Pts2Tc() {}

void sfmpv_NextTc() {}

void sfmpv_FirstPicAtr() {}

void sfmpv_SetMpvHd() {}

void sfmpv_ChkBufSiz() {}

void sfmpv_IsSkip() {}

void sfmpv_UpdateDefect() {}

u32 SFSET_GetCond(void* self, u32 idx);
s32 MPV_IsEmptyBpic(u32 a, u32 b, u32 size);
s32 MPV_IsEmptyPpic(u32 a, u32 b, u32 size);

s32 sfmpv_IsEmptyBpic(void* self, s32 type, void* bpic) {
    if (SFSET_GetCond(self, 7) != 0) {
        return 0;
    }
    if (type == 3) {
        s32 r = MPV_IsEmptyBpic(*(u32*)bpic, *(u32*)((u8*)bpic + 4),
                                *(u32*)((u8*)self + 0x924) * *(u32*)((u8*)self + 0x928));
        if (r != 0) {
            *(u32*)((u8*)self + 0x970) += 1;
        }
        return r;
    }
    if (type == 2) {
        s32 r = MPV_IsEmptyPpic(*(u32*)bpic, *(u32*)((u8*)bpic + 4),
                                *(u32*)((u8*)self + 0x924) * *(u32*)((u8*)self + 0x928));
        if (r != 0) {
            *(u32*)((u8*)self + 0x974) += 1;
        }
        return r;
    }
    return 0;
}

void sfmpv_IsLate() {}

void sfmpv_SkipFrm() {}

void sfmpv_DecodeFrm() {}

void sfmpv_SetFrmPara() {}

void fn_803C9948() {}

void fn_803C99C8() {}

void sfmpv_GoDdelim() {}

void SFMPV_Create() {}

void sfmpv_InitInf() {}

void SFTIM_InitTtu(void* self, int val);
int UTY_MemsetDword(u32* dst, u32 val, int count);

void sfmpv_InitFrmObj(void* frm, const u32* src, s32 count) {
    s32 i;
    for (i = 0; i < count; i++) {
        *(u32*)((u8*)frm + 0x00) = 0;
        *(u32*)((u8*)frm + 0x04) = 0;
        *(u16*)((u8*)frm + 0x0c) = 0;
        *(u16*)((u8*)frm + 0x0e) = 0;
        *(u16*)((u8*)frm + 0x10) = 0;
        *(u16*)((u8*)frm + 0x12) = 0;
        *(u32*)((u8*)frm + 0x14) = 5;
        SFTIM_InitTtu((u8*)frm + 0x18, 0);
        *(u32*)((u8*)frm + 0x08) = src[0];
        *(u32*)((u8*)frm + 0x44) = 0;
        *(u32*)((u8*)frm + 0x48) = 1;
        *(u32*)((u8*)frm + 0x4c) = 0;
        *(u32*)((u8*)frm + 0x50) = 0;
        *(u32*)((u8*)frm + 0x54) = 0;
        *(u32*)((u8*)frm + 0x58) = 0;
        *(u32*)((u8*)frm + 0x5c) = 0;
        *(u32*)((u8*)frm + 0x64) = -1;
        UTY_MemsetDword((u32*)((u8*)frm + 0x68), -1, 0x20);
        *(u32*)((u8*)frm + 0xec) = -1;
        *(u32*)((u8*)frm + 0xe8) = -1;
        *(u32*)((u8*)frm + 0xf0) = 0;
        *(u32*)((u8*)frm + 0xf4) = 0;
        *(u32*)((u8*)frm + 0xf8) = 0;
        *(u32*)((u8*)frm + 0xfc) = 0;
        *(u16*)((u8*)frm + 0x100) = 0;
        frm = (u8*)frm + 0x110;
        src += 1;
    }
}

s32 SFLIB_SetErr(void* h, u32 err_code);
s32 fn_803C34F8(s32 val, u32 err_code);
void sfmpv_ErrFn(s32 val, u32 err) {
    u32 tmp = err + 3;
    if (tmp <= 1) return;
    if (err == 0) return;
    if ((err & 0xFFFF0000) == 0xFF040000)
        fn_803C34F8(val, err);
    else
        SFLIB_SetErr((void*)val, err);
}

void SFMPVF_WriteGlobalMpvPara(void* self);
s32 MPV_Destroy(u32 mpv);

s32 SFMPV_Destroy(void* self) {
    void* shc = *(void**)((u8*)self + 0x2068);
    u32 mpv = *(u32*)shc;
    if (mpv == 0) {
        return 0;
    }
    SFMPVF_WriteGlobalMpvPara(self);
    if (MPV_Destroy(mpv) != 0) {
        return SFLIB_SetErr(self, 0xff000f0c);
    }
    *(u32*)shc = 0;
    return 0;
}

int SFMPV_RequestStop(void* self) {
    void* p;
    int result;
    
    p = *(void**)(*(void**)((u8*)self + 0x2068));
    if (p == NULL) {
        result = 0;
    } else {
        MPV_RequestStop(p);
        result = 0;
    }
    return result;
}

int SFMPV_Start(void) { return 0x0; }

int SFMPV_Stop(void) { return 0x0; }

int SFMPV_Pause(void) { return 0x0; }

s32 SFLIB_SetErr(void* h, u32 err_code);
s32 SFMPV_GetWrite(void* h) {
    return SFLIB_SetErr(h, 0xff000f0d);
}

s32 SFLIB_SetErr(void* h, u32 err_code);
s32 SFMPV_AddWrite(void* h) {
    return SFLIB_SetErr(h, 0xff000f0d);
}

void SFMPVF_GetRead(void* self, void* a, void* b, void* cb);
void sfmpv_SetFrmInf();
void SFMPV_GetRead(void* self, void* a, void* b) {
    SFMPVF_GetRead(self, a, b, sfmpv_SetFrmInf);
}

void sfmpv_SetFrmInf(void* self, void* frm, void* info) {
    *(u32*)((u8*)info + 0x00) = *(u32*)((u8*)frm + 0x68);
    *(u32*)((u8*)info + 0x04) = *(u32*)((u8*)frm + 0x6c);
    *(u32*)((u8*)info + 0x08) = *(u32*)((u8*)frm + 0x70);
    *(u32*)((u8*)info + 0x0c) = *(u32*)((u8*)frm + 0x74);
    *(u32*)((u8*)info + 0x10) = *(u32*)((u8*)frm + 0x80);
    *(u32*)((u8*)info + 0x14) = *(u32*)((u8*)frm + 0x44);
    *(u32*)((u8*)info + 0x18) = *(u32*)((u8*)frm + 0x48);
    *(u32*)((u8*)info + 0x1c) = *(u32*)((u8*)self + 0x40);
    *(u32*)((u8*)info + 0x20) = *(u32*)((u8*)frm + 0x08);
    *(u32*)((u8*)info + 0x24) = *(u32*)((u8*)frm + 0x4c);
    *(u32*)((u8*)info + 0x28) = *(u32*)((u8*)frm + 0x50);
    *(u32*)((u8*)info + 0x2c) = *(u32*)((u8*)frm + 0x54);
    *(u32*)((u8*)info + 0x30) = *(u32*)((u8*)frm + 0x58);
    *(u32*)((u8*)info + 0x34) = *(u32*)((u8*)frm + 0x5c);
    *(u32*)((u8*)info + 0x38) = *(u32*)((u8*)frm + 0x60);
    *(u32*)((u8*)info + 0x3c) = *(u32*)((u8*)frm + 0xa8);
    *(u32*)((u8*)info + 0x40) = *(u32*)((u8*)frm + 0xac);
    if (*(u32*)((u8*)frm + 0xa8) == 0) {
        *(u32*)((u8*)info + 0x48) = 2;
    } else {
        *(u32*)((u8*)info + 0x48) = 1;
    }
    *(u32*)((u8*)info + 0x50) = *(u32*)((u8*)frm + 0xe8);
    *(u32*)((u8*)info + 0x54) = *(u32*)((u8*)frm + 0xec);
    *(u32*)((u8*)info + 0x58) = *(u32*)((u8*)frm + 0xa0);
    *(u32*)((u8*)info + 0x5c) = *(u32*)((u8*)frm + 0xa4);
    *(u32*)((u8*)info + 0x60) = *(u32*)((u8*)frm + 0xb0);
    *(u32*)((u8*)info + 0x64) = *(u32*)((u8*)frm + 0xb4);
    *(u16*)((u8*)info + 0x68) = *(s16*)((u8*)frm + 0xb8);
    *(u16*)((u8*)info + 0x6a) = *(s16*)((u8*)frm + 0xba);
    *(u8*)((u8*)info + 0x6c) = *(u8*)((u8*)frm + 0xbd);
    *(u8*)((u8*)info + 0x6d) = *(u8*)((u8*)frm + 0xbe);
    *(u8*)((u8*)info + 0x6e) = *(u8*)((u8*)frm + 0xbf);
    *(u8*)((u8*)info + 0x6f) = *(u8*)((u8*)frm + 0xc1);
    *(u8*)((u8*)info + 0x70) = *(u8*)((u8*)frm + 0xc2);
    *(u8*)((u8*)info + 0x71) = *(u8*)((u8*)frm + 0xc3);
    *(u8*)((u8*)info + 0x72) = *(u8*)((u8*)frm + 0xc4);
    *(u8*)((u8*)info + 0x73) = *(u8*)((u8*)frm + 0xc5);
    *(u8*)((u8*)info + 0x74) = *(u8*)((u8*)frm + 0xc6);
    *(u8*)((u8*)info + 0x75) = *(u8*)((u8*)frm + 0xc7);
    *(u8*)((u8*)info + 0x76) = *(u8*)((u8*)frm + 0xc8);
    *(u8*)((u8*)info + 0x77) = *(u8*)((u8*)frm + 0xc9);
    *(u8*)((u8*)info + 0x78) = *(u8*)((u8*)frm + 0xca);
    *(u8*)((u8*)info + 0x79) = *(u8*)((u8*)frm + 0xcb);
    *(u8*)((u8*)info + 0x7a) = *(u8*)((u8*)frm + 0xcc);
    *(u16*)((u8*)info + 0x7c) = *(s16*)((u8*)frm + 0x100);
}

void SFMPV_AddRead(void) { SFMPVF_AddRead(); }

void SFMPV_Seek() {}

int fn_803CA368(void) { return 0x0; }

void sfmpv_ReprocessShc() {}
