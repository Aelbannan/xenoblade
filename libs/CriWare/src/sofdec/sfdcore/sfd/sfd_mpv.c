// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_mpv
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void SFD_SetMpvCond() {}

void SFMPV_SaveCond() {}

void SFMPV_RestoreCond() {}

void MPV_SetMbCb(void* p, u32 a, u32 b, u32 c);
void SFD_SetMbCb(void* self, u32 a, u32 b, u32 c) {
    void* arg1 = *(void**)(*(void**)((u8*)self + 0x2068));
    ((u32*)self)[0xd8c / 4] = a;
    ((u32*)self)[0xd88 / 4] = c;
    ((u32*)self)[0xd84 / 4] = b;
    MPV_SetMbCb(arg1, a, b, c);
}

void SFMPV_Init() {}

int SFMPV_Finish(void) {
    MPV_Finish();
    return 0;
}

void SFMPV_ExecServer() {}

void sfmpv_ProcessAuxShc() {}

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

void sfmpv_IsEmptyBpic() {}

void sfmpv_IsLate() {}

void sfmpv_SkipFrm() {}

void sfmpv_DecodeFrm() {}

void sfmpv_SetFrmPara() {}

void fn_803C9948() {}

void fn_803C99C8() {}

void sfmpv_GoDdelim() {}

void SFMPV_Create() {}

void sfmpv_InitInf() {}

void sfmpv_InitFrmObj() {}

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

void SFMPV_Destroy() {}

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

void sfmpv_SetFrmInf() {}

void SFMPV_AddRead(void) { SFMPVF_AddRead(); }

void SFMPV_Seek() {}

int fn_803CA368(void) { return 0x0; }

void sfmpv_ReprocessShc() {}
