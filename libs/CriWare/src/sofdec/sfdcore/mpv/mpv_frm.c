// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/mpv/mpv_frm
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void MPVFRM_Init(void) {}

void MPV_DecodeFrmSj() {}

void MPV_SkipFrmSj() {}

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
