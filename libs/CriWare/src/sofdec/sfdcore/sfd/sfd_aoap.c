// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_aoap
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

#include "libs/CriWare/src/sofdec/sfdcore/sfd/sfd_types.h"

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

/* name-guess: the only map name for the retail .rodata object at 0x8051C550
 * is the generic lbl_eu_8051C550.  Its fourteen ADDR32 relocations name the
 * SFAOAP service functions below, followed by two zero slots. */
typedef struct SfdAoapFuncTable {
    int (*init)(void);                  /* 0x00 */
    int (*finish)(void);                /* 0x04 */
    int (*execServer)(SfdHandle*);      /* 0x08 */
    int (*create)(SfdHandle*);          /* 0x0C */
    int (*destroy)(void);               /* 0x10 */
    int (*requestStop)(void*);          /* 0x14 */
    int (*start)(void*);                /* 0x18 */
    int (*stop)(void*);                 /* 0x1C */
    int (*pause)(void*, s32);           /* 0x20 */
    s32 (*getWrite)(void*);             /* 0x24 */
    s32 (*addWrite)(void*);             /* 0x28 */
    s32 (*getRead)(void*);              /* 0x2C */
    s32 (*addRead)(void*);              /* 0x30 */
    int (*seek)(void);                  /* 0x34 */
    void* reserved[2];                  /* 0x38 */
} SfdAoapFuncTable;

__attribute__((aligned(8), used)) const SfdAoapFuncTable sSfdAoapFuncTable = {
    SFAOAP_Init,
    SFAOAP_Finish,
    SFAOAP_ExecServer,
    SFAOAP_Create,
    SFAOAP_Destroy,
    SFAOAP_RequestStop,
    SFAOAP_Start,
    SFAOAP_Stop,
    SFAOAP_Pause,
    SFAOAP_GetWrite,
    SFAOAP_AddWrite,
    SFAOAP_GetRead,
    SFAOAP_AddRead,
    SFAOAP_Seek,
    NULL,
    NULL,
};
