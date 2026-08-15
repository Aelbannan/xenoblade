// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_trn
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

struct SFTRN;
struct SFD_TR_CONFIG;

/* Setup/initializer function signature used by the SFD transport layer.
   The first argument is the transport context; the remaining three are
   transport-specific setup parameters (always NULL from this TU's calls). */
typedef s32 (*SfdTrSetupFunc)(struct SFTRN* self, struct SFD_TR_CONFIG* a1,
                              struct SFD_TR_CONFIG* a2, struct SFD_TR_CONFIG* a3);

/* Object pointed to by each SFD_TR_CONFIG entry; its first word is a function pointer. */
typedef struct SfdTrSetupObj {
    /* 0x00 */ SfdTrSetupFunc func;
} SfdTrSetupObj;

/* Config block copied into SFTRN at offset 0 by SFTRN_Init (0x3C bytes). */
typedef struct SFD_TR_CONFIG {
    /* 0x00 */ SfdTrSetupObj* setup[15];
} SFD_TR_CONFIG;

/* One transport work entry at SFTRN+0x1FD8 (0x44 bytes each). */
typedef struct SfdTrSetupEntry {
    /* 0x00 */ u32 prep_flg;
    /* 0x04 */ u32 term_flg;
    /* 0x08 */ u32 field_0x08;
    /* 0x0C */ SfdTrSetupFunc* func_table;
    /* 0x10 */ u32 field_0x10[13];
} SfdTrSetupEntry;

/* SFD transport structure (partial layout: only fields used by sfd_trn.c). */
typedef struct SFTRN {
    /* 0x000 */ SFD_TR_CONFIG config;
    /* 0x03C */ u8 pad_0x03C[0x1FD8 - 0x3C];
    /* 0x1FD8 */ SfdTrSetupEntry tr_work[9];
} SFTRN;

extern s32 SFLIB_SetErr(void* h, u32 err_code);
extern u32 SFSET_SetCond(void* self, u32 idx, u32 val);
static s32 sftrn_BuildAll(void* self, void* config);
static void sftrn_BuildSystem(void* self, void* config);

s32 SFTRN_Init(SFTRN* self, SFD_TR_CONFIG* config) {
    s32 i;
    s32 ret = 0;

    /* Copy the 15 setup pointers into the transport context. */
    self->config = *config;

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

static s32 sftrn_BuildAll(void* self, void* config) {
    u32* p = (u32*)config;
    if (p[1] != 0) {
        *(s32*)((u8*)self + 0x1fec) = 0;
        *(s32*)((u8*)self + 0x1404) = 0;
        sftrn_BuildSystem(self, config);
    } else if (p[2] != 0) {
        *(s32*)((u8*)self + 0x1fec) = 1;
        *(s32*)((u8*)self + 0x1478) = 0;
        *(s32*)((u8*)self + 0x147c) = 2;
        *(s32*)((u8*)self + 0x2070) = 1;
        *(s32*)((u8*)self + 0x2074) = 3;
        *(s32*)((u8*)self + 0x1560) = 2;
        if (p[4] != 0) {
            *(s32*)((u8*)self + 0x1564) = 4;
            *(s32*)((u8*)self + 0x20f8) = 3;
            *(s32*)((u8*)self + 0x20fc) = 5;
            *(s32*)((u8*)self + 0x1648) = 4;
            *(s32*)((u8*)self + 0x164c) = 6;
            *(s32*)((u8*)self + 0x2180) = 5;
        } else {
            *(s32*)((u8*)self + 0x1564) = 6;
            *(s32*)((u8*)self + 0x2180) = 3;
        }
        SFSET_SetCond(self, 6, 0);
        *(s32*)((u8*)self + 0xbc4) = 0;
    } else if (p[3] != 0) {
        *(s32*)((u8*)self + 0x1fec) = 2;
        *(s32*)((u8*)self + 0x14ec) = 0;
        *(s32*)((u8*)self + 0x14f0) = 3;
        *(s32*)((u8*)self + 0x20b4) = 2;
        *(s32*)((u8*)self + 0x20b8) = 4;
        *(s32*)((u8*)self + 0x15d4) = 3;
        if (p[5] != 0) {
            *(s32*)((u8*)self + 0x15d8) = 5;
            *(s32*)((u8*)self + 0x213c) = 4;
            *(s32*)((u8*)self + 0x2140) = 6;
            *(s32*)((u8*)self + 0x16bc) = 5;
            *(s32*)((u8*)self + 0x16c0) = 7;
            *(s32*)((u8*)self + 0x21c4) = 6;
        } else {
            *(s32*)((u8*)self + 0x15d8) = 7;
            *(s32*)((u8*)self + 0x21c4) = 4;
        }
        SFSET_SetCond(self, 5, 0);
        *(s32*)((u8*)self + 0xbc0) = 0;
    } else if (p[8] != 0) {
        *(s32*)((u8*)self + 0x1fec) = 7;
        *(s32*)((u8*)self + 0x1730) = 0;
        *(s32*)((u8*)self + 0x1734) = 8;
        *(s32*)((u8*)self + 0x2208) = 7;
        SFSET_SetCond(self, 6, 0);
        SFSET_SetCond(self, 5, 0);
        *(s32*)((u8*)self + 0xbc4) = 0;
        *(s32*)((u8*)self + 0xbc0) = 0;
    } else {
        return -1;
    }
    return 0;
}

static void sftrn_BuildSystem(void* self, void* config) {
    u32* p = (u32*)config;
    *(s32*)((u8*)self + 0x1408) = 1;
    *(s32*)((u8*)self + 0x202c) = 0;
    if (p[2] != 0) {
        *(s32*)((u8*)self + 0x2030) = 1;
        *(s32*)((u8*)self + 0x1478) = 1;
        *(s32*)((u8*)self + 0x147c) = 2;
        *(s32*)((u8*)self + 0x2070) = 1;
        *(s32*)((u8*)self + 0x2074) = 3;
        *(s32*)((u8*)self + 0x1560) = 2;
        if (p[4] != 0) {
            *(s32*)((u8*)self + 0x1564) = 4;
            *(s32*)((u8*)self + 0x20f8) = 3;
            *(s32*)((u8*)self + 0x20fc) = 5;
            *(s32*)((u8*)self + 0x1648) = 4;
            *(s32*)((u8*)self + 0x164c) = 6;
            *(s32*)((u8*)self + 0x2180) = 5;
        } else {
            *(s32*)((u8*)self + 0x1564) = 6;
            *(s32*)((u8*)self + 0x2180) = 3;
        }
    } else {
        SFSET_SetCond(self, 5, 0);
        *(s32*)((u8*)self + 0xbc0) = 0;
    }
    if (p[3] != 0) {
        *(s32*)((u8*)self + 0x2034) = 2;
        *(s32*)((u8*)self + 0x14ec) = 1;
        *(s32*)((u8*)self + 0x14f0) = 3;
        *(s32*)((u8*)self + 0x20b4) = 2;
        *(s32*)((u8*)self + 0x20b8) = 4;
        *(s32*)((u8*)self + 0x15d4) = 3;
        if (p[5] != 0) {
            *(s32*)((u8*)self + 0x15d8) = 5;
            *(s32*)((u8*)self + 0x213c) = 4;
            *(s32*)((u8*)self + 0x2140) = 6;
            *(s32*)((u8*)self + 0x16bc) = 5;
            *(s32*)((u8*)self + 0x16c0) = 7;
            *(s32*)((u8*)self + 0x21c4) = 6;
        } else {
            *(s32*)((u8*)self + 0x15d8) = 7;
            *(s32*)((u8*)self + 0x21c4) = 4;
        }
    } else {
        SFSET_SetCond(self, 6, 0);
        *(s32*)((u8*)self + 0xbc4) = 0;
    }
    if (p[8] != 0) {
        *(s32*)((u8*)self + 0x2038) = 7;
        *(s32*)((u8*)self + 0x1730) = 1;
        *(s32*)((u8*)self + 0x1734) = 8;
        *(s32*)((u8*)self + 0x2208) = 7;
    }
}

s32 SFTRN_CallTrSetup(SFTRN* self, s32 idx) {
    s32 i;
    s32 ret = 0;
    SfdTrSetupEntry* tr = self->tr_work;

    /* Call the idx-th setup function of each active transport work entry. */
    for (i = 0; i < 9; i++, tr++) {
        if (tr->func_table != NULL) {
            ret = tr->func_table[idx](self, 0, 0, 0);
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

void SFTRN_SetPrepFlg(void* self, u32 idx, u32 val) {
    *(u32*)((u8*)self + 0x1fd8 + idx * 0x44) = val;
}

u32 SFTRN_GetPrepFlg(void* self, u32 idx) {
    return *(u32*)((u8*)self + 0x1fd8 + idx * 0x44);
}

void SFTRN_SetTermFlg(void* self, u32 idx, u32 val) {
    *(u32*)((u8*)self + 0x1fdc + idx * 0x44) = val;
}

u32 SFTRN_GetTermFlg(void* self, u32 idx) {
    return *(u32*)((u8*)self + 0x1fdc + idx * 0x44);
}

u32 SFTRN_IsSetup(void* self, u32 idx) {
    s32 val = *(s32*)((u8*)self + idx * 0x44 + 0x1fe4);
    return val ? 1 : 0;
}
