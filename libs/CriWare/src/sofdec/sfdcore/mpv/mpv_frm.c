// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/mpv/mpv_frm
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void MPVFRM_Init(void) {}

void MPV_DecodeFrmSj() {}

extern s32 MPV_GoNextDelimSj(void* a);
extern s32 MPV_MoveChunk(void* a, s32 b, s32 c);

s32 MPV_SkipFrmSj(void* h, void* arg2) {
    s32 flag;
    if (MPVLIB_CheckHn(h))
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
        if (MPV_MoveChunk(arg2, 1, 4) != 4)
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
