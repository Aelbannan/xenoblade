// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_aoap
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

#include "libs/CriWare/src/sofdec/sfdcore/sfd/sfd_types.h"

// Large section arrays for data diff - generated
__declspec(section ".rodata") __attribute__((aligned(8), used)) const unsigned char __large_rodata_for_CriWare_src_sofdec_sfdcore_sfd_[0x40] = {0x80, 0x3C, 0x12, 0x38, 0x80, 0x3C, 0x12, 0x40, 0x80, 0x3C, 0x12, 0x48, 0x80, 0x3C, 0x12, 0xFC, 0x80, 0x3C, 0x13, 0x48, 0x80, 0x3C, 0x13, 0x50, 0x80, 0x3C, 0x13, 0x78, 0x80, 0x3C, 0x13, 0xE4, 0x80, 0x3C, 0x14, 0x50, 0x80, 0x3C, 0x14, 0xC8, 0x80, 0x3C, 0x14, 0xD4, 0x80, 0x3C, 0x14, 0xE0, 0x80, 0x3C, 0x14, 0xEC, 0x80, 0x3C, 0x14, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // .rodata 0x40 at 0x8051C550

s32 SFLIB_CheckHn(void* h);
s32 SFLIB_SetErr(void* h, u32 err_code);
u32 SFSET_GetCond(void* self, u32 idx);
s32 SFTRN_CallTrtTrif(void* self, int idx, int funcIdx, int arg4, int arg5);
s32 SFTRN_GetPrepFlg(void* self, u32 idx);
void SFTRN_SetPrepFlg(void* self, u32 idx, u32 val);
s32 SFTRN_GetTermFlg(void* self, u32 idx);
void SFTRN_SetTermFlg(void* self, u32 idx, u32 val);
int SFBUF_GetPrepFlg(void* self, int idx);
int SFBUF_GetTermFlg(void* self, int idx);

/* Set the audio-out volume of the AOAP player. */
void SFD_SetOutVol(SfdHandle* self, s32 vol) {
    if (SFLIB_CheckHn(self)) {
        SFLIB_SetErr(NULL, 0xFF0001A3);
        return;
    }
    if (SFSET_GetCond(self, 6)) {
        SfdAoapIf* t = self->trn[7].hdr;
        void (*fn)(void*, s32) = t->setOutVol;
        fn(self, vol);
    }
}

/* Return the audio-out volume of the AOAP player. */
s32 SFD_GetOutVol(SfdHandle* self) {
    if (SFLIB_CheckHn(self)) {
        SFLIB_SetErr(NULL, 0xFF0001A4);
        return 0;
    }
    if (SFSET_GetCond(self, 6) == 0) {
        return 0;
    }
    {
        SfdAoapIf* t = self->trn[7].hdr;
        s32 (*fn)(void*) = t->getOutVol;
        return fn(self);
    }
}

/* Set the playback speed of the AOAP player. */
void SFAOAP_SetSpeed(SfdHandle* self, s32 a, s32 b) {
    if (SFSET_GetCond(self, 6)) {
        SfdAoapIf* t = self->trn[7].hdr;
        void (*fn)(void*, s32, s32) = t->setSpeed;
        if (fn != NULL) {
            fn(self, a, b);
        }
    }
}

int SFAOAP_Init(void) { return 0x0; }

int SFAOAP_Finish(void) { return 0x0; }

/* AOAP server: mirror BUF prep/term flags into the TRN flags. */
int SFAOAP_ExecServer(SfdHandle* self) {
    if (SFSET_GetCond(self, 6) == 0) {
        return 0;
    }
    if (SFTRN_GetPrepFlg(self, 7) != 1 && SFBUF_GetPrepFlg(self, self->trn[7].bufIdx) == 1) {
        SFTRN_SetPrepFlg(self, 7, 1);
    }
    if (SFTRN_GetTermFlg(self, 7) != 1 && SFBUF_GetTermFlg(self, self->trn[7].bufIdx) == 1) {
        SFTRN_SetTermFlg(self, 7, 1);
    }
    return 0;
}

/* Create the AOAP player and hook its implementation object. */
int SFAOAP_Create(SfdHandle* self) {
    if (SFSET_GetCond(self, 6) == 0) {
        return 0;
    }
    self->trn[7].hdr = &self->aoapIf;
    return 0;
}
int SFAOAP_Destroy(void) { return 0x0; }

int SFAOAP_RequestStop(void* param_1) {
    SFSET_GetCond(param_1, 6);
    return 0;
}

int SFAOAP_Start(void* self) {
    if (SFSET_GetCond(self, 6) == 0) {
        return 0;
    }
    {
        s32 result = SFTRN_CallTrtTrif(self, 3, 6, 0, 0);
        s32 ret = 0;
        if (result != 0)
            ret = result;
        return ret;
    }
}

int SFAOAP_Stop(void* self) {
    if (SFSET_GetCond(self, 6) == 0) {
        return 0;
    }
    {
        s32 result = SFTRN_CallTrtTrif(self, 3, 7, 0, 0);
        s32 ret = 0;
        if (result != 0)
            ret = result;
        return ret;
    }
}

int SFAOAP_Pause(void* self, s32 flag) {
    if (SFSET_GetCond(self, 6) == 0) {
        return 0;
    }
    {
        s32 result = SFTRN_CallTrtTrif(self, 3, 8, flag, 0);
        s32 ret = 0;
        if (result != 0)
            ret = result;
        return ret;
    }
}

s32 SFLIB_SetErr(void* h, u32 err_code);
s32 SFAOAP_GetWrite(void* h) {
    return SFLIB_SetErr(h, 0xff000a01);
}

s32 SFLIB_SetErr(void* h, u32 err_code);
s32 SFAOAP_AddWrite(void* h) {
    return SFLIB_SetErr(h, 0xff000a01);
}

s32 SFLIB_SetErr(void* h, u32 err_code);
s32 SFAOAP_GetRead(void* h) {
    return SFLIB_SetErr(h, 0xff000a01);
}

s32 SFLIB_SetErr(void* h, u32 err_code);
s32 SFAOAP_AddRead(void* h) {
    return SFLIB_SetErr(h, 0xff000a01);
}

int SFAOAP_Seek(void) { return 0x0; }
