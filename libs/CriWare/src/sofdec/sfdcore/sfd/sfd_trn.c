// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_trn
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include "libs/CriWare/src/sofdec/sfdcore/sfd/sfd_types.h"

struct SFD_TR_CONFIG;

/* Setup/initializer function signature used by the SFD transport layer.
   The first argument is the transport context; the remaining three are
   transport-specific setup parameters (always NULL from this TU's calls). */
typedef s32 (*SfdTrSetupFunc)(void* self, struct SFD_TR_CONFIG* a1,
                              struct SFD_TR_CONFIG* a2, struct SFD_TR_CONFIG* a3);

/* Object pointed to by each SFD_TR_CONFIG entry; its first word is a function pointer. */
typedef struct SfdTrSetupObj {
    /* 0x00 */ SfdTrSetupFunc func;
} SfdTrSetupObj;

/* Config block copied into the transport setup table by SFTRN_Init (0x3C bytes). */
typedef struct SFD_TR_CONFIG {
    /* 0x00 */ SfdTrSetupObj* setup[15];
} SFD_TR_CONFIG;

extern s32 SFLIB_SetErr(void* h, u32 err_code);
extern u32 SFSET_SetCond(void* self, u32 idx, u32 val);
static s32 sftrn_BuildAll(SfdHandle* self, void* config);
static void sftrn_BuildSystem(SfdHandle* self, void* config);

s32 SFTRN_Init(SFD_TR_CONFIG* self, SFD_TR_CONFIG* config) {
    s32 i;
    s32 ret = 0;

    /* Copy the 15 setup pointers into the transport context. */
    *self = *config;

    /* Run each non-null setup entry's initializer; stop on first failure. */
    for (i = 0; i < 15; i++) {
        SfdTrSetupObj* obj = config->setup[i];
        if (obj == NULL) break;
        ret = obj->func(0, 0, 0, 0);
        if (ret != 0) break;
    }
    return ret;
}

s32 SFTRN_InitHn(void* self, void* dst, void* config) {
    s32 i;
    u32* data;
    u32** fnTable;
    u8* p;
    fnTable = *(u32***)config;
    data = (u32*)fnTable;
    p = (u8*)dst;
    /* Initialize 3 handler entries, each 0xCC bytes */
    for (i = 0; i < 3; i++) {
        u32 v0, v1, v2;
        *(u32*)(p + 0x08) = 0;
        v0 = data[0];
        *(u32*)(p + 0x04) = 0;
        *(u32*)(p + 0x00) = 0;
        *(u32*)(p + 0x0C) = v0;
        *(u32*)(p + 0x10) = 8;
        *(u32*)(p + 0x14) = 8;
        *(u32*)(p + 0x18) = 8;
        *(u32*)(p + 0x1C) = 8;
        *(s32*)(p + 0x20) = -1;
        *(u32*)(p + 0x4C) = 0;
        v1 = data[1];
        *(u32*)(p + 0x48) = 0;
        *(u32*)(p + 0x44) = 0;
        *(u32*)(p + 0x50) = v1;
        *(u32*)(p + 0x54) = 8;
        *(u32*)(p + 0x58) = 8;
        *(u32*)(p + 0x5C) = 8;
        *(u32*)(p + 0x60) = 8;
        *(s32*)(p + 0x64) = -1;
        *(u32*)(p + 0x90) = 0;
        v2 = data[2];
        data += 3;
        *(u32*)(p + 0x8C) = 0;
        *(u32*)(p + 0x88) = 0;
        *(u32*)(p + 0x94) = v2;
        *(u32*)(p + 0x98) = 8;
        *(u32*)(p + 0x9C) = 8;
        *(u32*)(p + 0xA0) = 8;
        *(u32*)(p + 0xA4) = 8;
        *(s32*)(p + 0xA8) = -1;
        p += 0xCC;
    }
    if (sftrn_BuildAll(self, fnTable) != 0)
        return SFLIB_SetErr(self, 0xff000302);
    return 0;
}

static s32 sftrn_BuildAll(SfdHandle* self, void* config) {
    u32* p = (u32*)config;
    if (p[1] != 0) {
        self->trn[0].termIdx = 0;
        self->buf[0].inTrn = 0;
        sftrn_BuildSystem(self, config);
    } else if (p[2] != 0) {
        self->trn[0].termIdx = 1;
        self->buf[1].inTrn = 0;
        self->buf[1].outTrn = 2;
        self->trn[2].bufIdx = 1;
        self->trn[2].termIdx = 3;
        self->buf[3].inTrn = 2;
        if (p[4] != 0) {
            self->buf[3].outTrn = 4;
            self->trn[4].bufIdx = 3;
            self->trn[4].termIdx = 5;
            self->buf[5].inTrn = 4;
            self->buf[5].outTrn = 6;
            self->trn[6].bufIdx = 5;
        } else {
            self->buf[3].outTrn = 6;
            self->trn[6].bufIdx = 3;
        }
        SFSET_SetCond(self, 6, 0);
        self->condBak[6] = 0;
    } else if (p[3] != 0) {
        self->trn[0].termIdx = 2;
        self->buf[2].inTrn = 0;
        self->buf[2].outTrn = 3;
        self->trn[3].bufIdx = 2;
        self->trn[3].termIdx = 4;
        self->buf[4].inTrn = 3;
        if (p[5] != 0) {
            self->buf[4].outTrn = 5;
            self->trn[5].bufIdx = 4;
            self->trn[5].termIdx = 6;
            self->buf[6].inTrn = 5;
            self->buf[6].outTrn = 7;
            self->trn[7].bufIdx = 6;
        } else {
            self->buf[4].outTrn = 7;
            self->trn[7].bufIdx = 4;
        }
        SFSET_SetCond(self, 5, 0);
        self->condBak[5] = 0;
    } else if (p[8] != 0) {
        self->trn[0].termIdx = 7;
        self->buf[7].inTrn = 0;
        self->buf[7].outTrn = 8;
        self->trn[8].bufIdx = 7;
        SFSET_SetCond(self, 6, 0);
        SFSET_SetCond(self, 5, 0);
        self->condBak[6] = 0;
        self->condBak[5] = 0;
    } else {
        return -1;
    }
    return 0;
}

static void sftrn_BuildSystem(SfdHandle* self, void* config) {
    u32* p = (u32*)config;
    self->buf[0].outTrn = 1;
    self->trn[1].bufIdx = 0;
    if (p[2] != 0) {
        self->trn[1].termIdx = 1;
        self->buf[1].inTrn = 1;
        self->buf[1].outTrn = 2;
        self->trn[2].bufIdx = 1;
        self->trn[2].termIdx = 3;
        self->buf[3].inTrn = 2;
        if (p[4] != 0) {
            self->buf[3].outTrn = 4;
            self->trn[4].bufIdx = 3;
            self->trn[4].termIdx = 5;
            self->buf[5].inTrn = 4;
            self->buf[5].outTrn = 6;
            self->trn[6].bufIdx = 5;
        } else {
            self->buf[3].outTrn = 6;
            self->trn[6].bufIdx = 3;
        }
    } else {
        SFSET_SetCond(self, 5, 0);
        self->condBak[5] = 0;
    }
    if (p[3] != 0) {
        self->trn[1].vidIdx = 2;
        self->buf[2].inTrn = 1;
        self->buf[2].outTrn = 3;
        self->trn[3].bufIdx = 2;
        self->trn[3].termIdx = 4;
        self->buf[4].inTrn = 3;
        if (p[5] != 0) {
            self->buf[4].outTrn = 5;
            self->trn[5].bufIdx = 4;
            self->trn[5].termIdx = 6;
            self->buf[6].inTrn = 5;
            self->buf[6].outTrn = 7;
            self->trn[7].bufIdx = 6;
        } else {
            self->buf[4].outTrn = 7;
            self->trn[7].bufIdx = 4;
        }
    } else {
        SFSET_SetCond(self, 6, 0);
        self->condBak[6] = 0;
    }
    if (p[8] != 0) {
        self->trn[1].usrIdx = 7;
        self->buf[7].inTrn = 1;
        self->buf[7].outTrn = 8;
        self->trn[8].bufIdx = 7;
    }
}

s32 SFTRN_CallTrSetup(SfdHandle* self, s32 idx) {
    s32 i;
    s32 ret = 0;
    SfdTrn* tr = self->trn;

    /* Call the idx-th setup function of each active transport work entry. */
    for (i = 0; i < 9; i++, tr++) {
        SfdTrSetupFunc* func_table = tr->table;
        if (func_table != NULL) {
            ret = func_table[idx](self, 0, 0, 0);
            if (ret != 0) break;
        }
    }
    return ret;
}

int SFTRN_CallTrtTrif(void* self, int idx, int funcIdx, int* arg4, int arg5) {
    void** table = *(void***)((u8*)self + idx * 0x44 + 0x1fe4);
    if (table == NULL) return 0;
    typedef int (*CallTrtFunc)(void*, int*, int, int);
    return ((CallTrtFunc)table[funcIdx])(self, arg4, arg5, 0);
}

void SFTRN_SetPrepFlg(SfdHandle* self, u32 idx, u32 val) {
    self->trn[idx].prepFlg = val;
}

u32 SFTRN_GetPrepFlg(SfdHandle* self, u32 idx) {
    return self->trn[idx].prepFlg;
}

void SFTRN_SetTermFlg(SfdHandle* self, u32 idx, u32 val) {
    self->trn[idx].termFlg = val;
}

u32 SFTRN_GetTermFlg(SfdHandle* self, u32 idx) {
    return self->trn[idx].termFlg;
}

u32 SFTRN_IsSetup(void* self, u32 idx) {
    s32 val = *(s32*)((u8*)self + idx * 0x44 + 0x1fe4);
    return val ? 1 : 0;
}
