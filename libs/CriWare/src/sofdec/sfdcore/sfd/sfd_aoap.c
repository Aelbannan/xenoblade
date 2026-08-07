// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_aoap
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

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
void SFD_SetOutVol(void* self, s32 vol) {
    if (SFLIB_CheckHn(self)) {
        SFLIB_SetErr(NULL, 0xFF0001A3);
        return;
    }
    if (SFSET_GetCond(self, 6)) {
        void* t = *(void**)((u8*)self + 0x21BC);
        void (*fn)(void*, s32) = *(void(**)(void*, s32))((u8*)t + 0x0C);
        fn(self, vol);
    }
}

/* Return the audio-out volume of the AOAP player. */
s32 SFD_GetOutVol(void* self) {
    if (SFLIB_CheckHn(self)) {
        SFLIB_SetErr(NULL, 0xFF0001A4);
        return 0;
    }
    if (SFSET_GetCond(self, 6) == 0) {
        return 0;
    }
    {
        void* t = *(void**)((u8*)self + 0x21BC);
        s32 (*fn)(void*) = *(s32(**)(void*))((u8*)t + 0x10);
        return fn(self);
    }
}

/* Set the playback speed of the AOAP player. */
void SFAOAP_SetSpeed(void* self, s32 a, s32 b) {
    if (SFSET_GetCond(self, 6)) {
        void* t = *(void**)((u8*)self + 0x21BC);
        void (*fn)(void*, s32, s32) = *(void(**)(void*, s32, s32))((u8*)t + 0x14);
        if (fn != NULL) {
            fn(self, a, b);
        }
    }
}

int SFAOAP_Init(void) { return 0x0; }

int SFAOAP_Finish(void) { return 0x0; }

/* AOAP server: mirror BUF prep/term flags into the TRN flags. */
int SFAOAP_ExecServer(void* self) {
    if (SFSET_GetCond(self, 6) == 0) {
        return 0;
    }
    if (SFTRN_GetPrepFlg(self, 7) != 1 && SFBUF_GetPrepFlg(self, *(u32*)((u8*)self + 0x21C4)) == 1) {
        SFTRN_SetPrepFlg(self, 7, 1);
    }
    if (SFTRN_GetTermFlg(self, 7) != 1 && SFBUF_GetTermFlg(self, *(u32*)((u8*)self + 0x21C4)) == 1) {
        SFTRN_SetTermFlg(self, 7, 1);
    }
    return 0;
}

struct SFAOAPImpl {
    u8 pad0[0x21BC];
    void* field_0x21BC;
    u8 pad1[0x261C - 0x21C0];
    u8 field_0x261C;
};

/* Create the AOAP player and hook its implementation vtable. */
int SFAOAP_Create(void* self) {
    if (SFSET_GetCond(self, 6) == 0) {
        return 0;
    }
    struct SFAOAPImpl* impl = (struct SFAOAPImpl*)self;
    impl->field_0x21BC = &impl->field_0x261C;
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
