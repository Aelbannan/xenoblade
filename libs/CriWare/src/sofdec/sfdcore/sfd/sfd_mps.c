// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_mps
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void SFD_SetElementOutSj() {}

void SFMPS_Init() {}

int SFMPS_Finish(void) {
    MPS_Finish();
    return 0;
}

void SFMPS_ExecServer() {}

void sfmps_DecodeSomeUnit() {}

void criware_803C1490() {}

void criware_803C1570(void* self, u32 a, u32 b) {
    if (self != NULL) {
        *(u32*)((u8*)self + 0x39a0) = a;
        *(u32*)((u8*)self + 0x39a4) = b;
    }
}

void sfmps_DecodeOneUnit() {}

void sfmps_pesfn() {}

void sfmps_SkipNext() {}

void sfmps_CopyPketData() {}

void sfmps_CopyAudio() {}

void sfmps_CopyVideo() {}

void sfmps_CopyPrvate() {}

void sfmps_CopyUsrSj() {}

int sfmps_CopyPadding(void) { return 0x1; }

void sfmps_CopyDstBuft() {}

void sfmps_ChkSupply() {}

void sfmps_GetStmNum() {}

void sfmps_SetMvInf() {}

void sfmps_SetMpsHd() {}

void sfmps_SetMpsRaw() {}

void SFMPS_Create() {}

void sfmps_InitInf() {}

s32 SFLIB_SetErr(void* h, u32 err_code);
s32 sfmps_ErrFn(void* h, u32 err_code) {
    return SFLIB_SetErr(h, err_code);
}

void SFMPS_Destroy() {}

int SFMPS_RequestStop(void) { return 0x0; }

int SFMPS_Start(void) { return 0x0; }

int SFMPS_Stop(void) { return 0x0; }

int SFMPS_Pause(void) { return 0x0; }

s32 SFLIB_SetErr(void* h, u32 err_code);
s32 SFMPS_GetWrite(void* h) {
    return SFLIB_SetErr(h, 0xff000d0b);
}

s32 SFLIB_SetErr(void* h, u32 err_code);
s32 SFMPS_AddWrite(void* h) {
    return SFLIB_SetErr(h, 0xff000d0b);
}

s32 SFLIB_SetErr(void* h, u32 err_code);
s32 SFMPS_GetRead(void* h) {
    return SFLIB_SetErr(h, 0xff000d0b);
}

s32 SFLIB_SetErr(void* h, u32 err_code);
s32 SFMPS_AddRead(void* h) {
    return SFLIB_SetErr(h, 0xff000d0b);
}

void SFMPS_Seek() {}

u32 SFMPS_GetConcatCnt(void* self) {
    void* ptr = *(void**)((u8*)self + 0x2024);
    return *(u32*)((u8*)ptr + 0x20);
}
