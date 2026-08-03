// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/mpv/mpv_frm
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void MPVFRM_Init(void) {}

extern s32 MPVM2V_DecodeFrm(void* h, void* a, void* b);
extern void MPVUMC_InitOutRfb(void* h);
extern void MPVCMC_InitMcOiRt(void* h);
extern void MPVCMC_SetCcnt(void* h);
extern void MPVCDEC_StartFrame(void* h);
extern void MPVBDEC_StartFrame(void* h);
extern s32 MPVSL_DecPicture(void* h, void* a);
extern void MPVUMC_EndOfFrame(void* h);

s32 MPV_DecodeFrmSj(void* h, void* a2, void* a3) {
    u32 v1;
    u32 v2;
    if (MPVLIB_CheckHn(h))
        return MPVERR_SetCode(NULL, 0xFF030209);
    if (*(s32*)((u8*)h + 0xcfc) == 2)
        return MPVM2V_DecodeFrm(h, a2, a3);
    {
        u8* src = (u8*)h + 0xbe8;
        u8* dst = (u8*)a3 - 4;
        s32 n = 8;
        v1 = *(u32*)((u8*)h + 0xbe8);
        v2 = *(u32*)((u8*)h + 0xbec);
        do {
            *(u32*)(dst + 4) = *(u32*)(src + 4);
            *(u32*)(dst + 8) = *(u32*)(src + 8);
            src += 8;
            dst += 8;
        } while (--n != 0);
        *(u32*)(dst + 4) = *(u32*)(src + 4);
    }
    MPVUMC_InitOutRfb(h);
    MPVCMC_InitMcOiRt(h);
    MPVCMC_SetCcnt(h);
    MPVCDEC_StartFrame(h);
    MPVBDEC_StartFrame(h);
    MPVSL_DecPicture(h, a2);
    MPVUMC_EndOfFrame(h);
    {
        u8* src = (u8*)h + 0xb58;
        u8* dst = (u8*)*(u32*)((u8*)a3 + 0x34) - 4;
        s32 n = 16;
        do {
            *(u32*)(dst + 4) = *(u32*)(src + 4);
            *(u32*)(dst + 8) = *(u32*)(src + 8);
            src += 8;
            dst += 8;
        } while (--n != 0);
    }
    *(u32*)((u8*)a3 + 0x38) = (u32)a2 - v1;
    *(u32*)((u8*)a3 + 0x3c) = (u32)a2 - v2;
    *(u16*)((u8*)a3 + 0x40) = *(s16*)((u8*)h + 0xc30);
    return 0;
}

extern s32 MPV_GoNextDelimSj(void* a);
extern s32 MPV_MoveChunk(void* a, s32 b, s32 c);

s32 MPV_SkipFrmSj(void* h, void* arg2) {
    s32 flag;
    if (MPVLIB_CheckHn(h) != 0)
        return MPVERR_SetCode(NULL, 0xFF03020A);
    for (;;) {
        s32 r;
        flag = 0xFF030305;
        r = MPV_GoNextDelimSj(arg2);
        if (r == 0)
            break;
        if ((r & 0xcc) != 0) {
            flag = 0;
            break;
        }
        if (MPV_MoveChunk(arg2, 1, 4) == 4)
            break;
    }
    return MPVERR_SetCode(h, flag);
}

extern int MPVLIB_CheckHn(void*);
extern int MPVERR_SetCode(void*, int);
extern int MPVM2V_RequestStop(void*);

s32 MPV_RequestStop(void* handle) {
    if (MPVLIB_CheckHn(handle)) {
        return MPVERR_SetCode(NULL, 0xFF030211);
    }
    *(u32*)((u8*)handle + 0xDA8) = 1;
    if ((s32)*(u32*)((u8*)handle + 0xCFC) == 2) {
        return MPVM2V_RequestStop(handle);
    }
    return 1;
}
