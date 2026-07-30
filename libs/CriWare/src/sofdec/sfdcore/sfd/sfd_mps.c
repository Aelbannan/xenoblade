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

typedef struct {
    u8 _000[0x9A0];
    u32 stm_id;
    u32 stm_inf;
    u8 _9A8[0x3B4];
    u32 pes_fn;
    u32 pes_obj;
} SFMPS_HN;

void sfmps_pesfn(SFMPS_HN* hn, u8 stream_kind, u32 arg3, u32 arg4) {
    void (*cb)(u32, u8*, u32*, u32*, u32, u32*);
    u8 kind_copy;
    u32 args[2];
    u32 stm_info[2];
    
    cb = (void (*)(u32, u8*, u32*, u32*, u32, u32*))hn->pes_fn;
    if (cb == NULL) return;
    
    kind_copy = stream_kind;
    args[0] = arg3;
    args[1] = arg4;
    stm_info[0] = hn->stm_id;
    stm_info[1] = hn->stm_inf;
    cb(hn->pes_obj, &kind_copy, stm_info, args, 0, NULL);
}

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

int MPS_Destroy(void*);

int SFMPS_Destroy(void* self) {
    void* mps = *(void**)((u8*)self + 0x2024);
    if (MPS_Destroy(*(void**)mps)) {
        return SFLIB_SetErr(self, 0xFF000D0A);
    }
    return 0;
}

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
