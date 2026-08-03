// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfx/sfx_lib
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <string.h>
#include "libs/CriWare/src/sofdec/sfx/sfx_types.h"

extern void* SFXZ_Create(void);
extern void* SFXA_Create(void);
extern void* SFXSUD_Create(void);
void sfx_InitHn(SFXHandleState* hn, u32 width, u32 height);

/* ---- SFX library global state ----
 * lbl_eu_80619C10 is a0x528-byte structure:
 *   +0x00: init_count (number of times SFX_Init called)
 *   +0x04: max_handles
 *   +0x08: error_fn (function pointer)
 *   +0x0C: error_arg
 *   +0x10: error_count
 *   +0x14: ccirFx
 *   +0x18: main SFX state / handle pool (SFXHandleState[max_handles], each0xA0 bytes)
 */
typedef struct SFXLibState {
    u32 init_count;       /* 0x00 */
    u32 max_handles;      /* 0x04 */
    void (*error_fn)(u32, const char*);  /* 0x08 */
    u32 error_arg;        /* 0x0C */
    u32 error_count;      /* 0x10 */
    u32 ccirFx;           /* 0x14 */
    SFXHandleState handle[8];  /* 0x18, each0xA0 bytes */
} SFXLibState;

extern SFXLibState lbl_eu_80619C10;
extern s32 lbl_eu_80619C00[4];   /* root: [0]=init_count, [1]=field_0x04, [2]=default err fn, [3]=field_0x0C */
extern u32 lbl_eu_8051D178;
extern u32 lbl_eu_80619C04;

void SFX_Init(void) {
    if (lbl_eu_80619C00[0] < 1) {
        lbl_eu_80619C00[2] = (s32)&lbl_eu_8051D178;
        memset((u8*)lbl_eu_80619C00 + 0x10, 0, 0x528);
        *(u32*)((u8*)lbl_eu_80619C00 + 0x14) = 8;
        *(u32*)((u8*)lbl_eu_80619C00 + 0x24) = 1;
        CFT_Init();
        SFXSUD_Init();
        SFXZ_Init();
        SFXA_Init();
        lbl_eu_80619C00[1] = 0;
        lbl_eu_80619C00[0]++;
    }
}

void SFX_SetErrFn(void (*fn)(u32, const char*), u32 arg) {
    lbl_eu_80619C10.error_fn = fn;
    lbl_eu_80619C10.error_arg = arg;
}

SFXHandleState* SFX_Create(u32 width, u32 height) {
    SFXHandleState* hn;
    void* zmv;
    void* alp;
    int i;

    /* Find free handle slot */
    hn = NULL;
    for (i = 0; i < (int)lbl_eu_80619C10.max_handles; i++) {
        if (lbl_eu_80619C10.handle[i].active == 0) {
            hn = &lbl_eu_80619C10.handle[i];
            break;
        }
    }
    if (hn == NULL) {
        return NULL;
    }

    /* Validate size */
    if ((int)width > 12319) {
        lbl_eu_80619C10.error_count++;
        if (lbl_eu_80619C10.error_fn) {
            lbl_eu_80619C10.error_fn(lbl_eu_80619C10.error_arg, "...");
        }
        return NULL;
    }

    sfx_InitHn(hn, width, height);

    zmv = SFXZ_Create();
    if (zmv == NULL) {
        lbl_eu_80619C10.error_count++;
        if (lbl_eu_80619C10.error_fn) {
            lbl_eu_80619C10.error_fn(lbl_eu_80619C10.error_arg, "...");
        }
        /* Cleanup */
        if (hn != NULL) {
            void* hn_zmv = hn->zmv;
            u32 hn_alp = hn->field_0x30;
            hn->active = 0;
            SFXZ_Destroy(hn_zmv);
            SFXA_Destroy(hn_alp);
            lbl_eu_80619C10.init_count--;
        }
        return NULL;
    }
    hn->zmv = zmv;

    alp = SFXA_Create();
    if (alp == NULL) {
        lbl_eu_80619C10.error_count++;
        if (lbl_eu_80619C10.error_fn) {
            lbl_eu_80619C10.error_fn(lbl_eu_80619C10.error_arg, "...");
        }
        /* Cleanup */
        if (hn != NULL) {
            void* hn_zmv = hn->zmv;
            u32 hn_alp = hn->field_0x30;
            hn->active = 0;
            SFXZ_Destroy(hn_zmv);
            SFXA_Destroy(hn_alp);
            lbl_eu_80619C10.init_count--;
        }
        return NULL;
    }
    hn->field_0x30 = (u32)alp;

    lbl_eu_80619C10.init_count++;
    return hn;
}

void sfx_InitHn(SFXHandleState* hn, u32 width, u32 height) {
    u32 alignW;
    u32 bufA, bufB, bufC;

    memset(hn, 0, 0xA0);
    alignW = (width + 0x1F) & ~0x1F;
    bufA = alignW + 0x400;
    bufB = bufA + 0x400;
    bufC = bufB + 0x400;

    hn->field_0x04 = 0;
    hn->field_0x08 = 0;
    hn->field_0x0C = 0;
    hn->field_0x28 = 1;
    hn->field_0x2C = 0;
    hn->field_0x34 = 0;
    hn->alignW = alignW;
    hn->bufASize = bufA;
    hn->bufBSize = bufB;
    hn->bufCSize = bufC;
    hn->width = width;
    hn->height = height;
    hn->field_0x58 = (u32)-1;
    hn->field_0x64 = 0;
    hn->active = 1;
}

void SFX_Destroy(SFXHandleState* hn) {
    if (hn == NULL)
        return;

    hn->active = 0;
    SFXZ_Destroy(hn->zmv);
    SFXA_Destroy(hn->field_0x30);
    lbl_eu_80619C10.init_count--;
}

void SFXLIB_Error(u32 arg, const char* msg) {
    void (*fn)(u32, const char*) = lbl_eu_80619C10.error_fn;
    u32 fnArg = lbl_eu_80619C10.error_arg;
    lbl_eu_80619C10.error_count++;
    if (fn) {
        fn(fnArg, msg);
    }
}

u32 SFX_SetCcirFx(void) {
    return lbl_eu_80619C10.ccirFx;
}

u32 SFX_GetForceSplitField(void) {
    return lbl_eu_80619C04;
}
