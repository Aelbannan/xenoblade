// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfx/sfx_lib
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <string.h>
#include "libs/CriWare/src/sofdec/sfx/sfx_types.h"

extern void* SFXZ_Create(void);
extern void* SFXA_Create(void);
extern void* SFXSUD_Create(void);
extern void SFXZ_Destroy(void* self);
extern void SFXA_Destroy(void* self);
extern u8 lbl_eu_8051D1AC[];   /* Sofdec SDK error message pool */
void sfx_InitHn(SFXHandleState* hn, u32 width, u32 height);

/* ---- SFX library global state ----
 * lbl_eu_80619C10 is a 0x528-byte structure:
 *   +0x00: init_count (number of times SFX_Init called)
 *   +0x04: max_handles
 *   +0x08: error_fn (function pointer)
 *   +0x0C: error_arg
 *   +0x10: error_count
 *   +0x14: ccirFx
 *   +0x18: handle pool (max_handles entries, each 0xA0 bytes)
 */
/* Retail SFXHandleState entries are 0xA0 bytes; the shared header's
 * SFXHandleState is an unfinished 0x68-byte view, so walk the pool with this
 * local 0xA0-strided entry struct and cast at the API boundaries. */
typedef struct SFXHandlePoolEntry {
    u32 active;        /* 0x00 */
    u8 _04[0x20];      /* 0x04 */
    void* zmv;         /* 0x24 */
    u8 _28[0x08];      /* 0x28 */
    u32 field_0x30;    /* 0x30 */
    u8 _34[0x6C];      /* 0x34 - pad to 0xA0 stride */
} SFXHandlePoolEntry;

typedef struct SFXLibState {
    u32 init_count;       /* 0x00 */
    u32 max_handles;      /* 0x04 */
    void (*error_fn)(u32, const char*);  /* 0x08 */
    u32 error_arg;        /* 0x0C */
    u32 error_count;      /* 0x10 */
    u32 ccirFx;           /* 0x14 */
    SFXHandlePoolEntry handle[8];  /* 0x18, each 0xA0 bytes */
    u8 _518[0x10];        /* 0x518 - pad to 0x528 */
} SFXLibState;

extern SFXLibState lbl_eu_80619C10;
extern s32 lbl_eu_80619C00[4];   /* root: [0]=init_count, [1]=field_0x04, [2]=default err fn, [3]=field_0x0C */
extern u32 lbl_eu_8051D178;
extern u32 lbl_eu_80619C04;

void SFX_Init(void) {
    s32* s = lbl_eu_80619C00;
    if (s[0] < 1) {
        s[2] = (s32)&lbl_eu_8051D178;
        memset((u8*)s + 0x10, 0, 0x528);
        *(u32*)((u8*)s + 0x10 + 4) = 8;
        *(u32*)((u8*)s + 0x10 + 0x14) = 1;
        CFT_Init();
        SFXSUD_Init();
        SFXZ_Init();
        SFXA_Init();
        s[1] = 0;
        s[0]++;
    }
}

void SFX_SetErrFn(void (*fn)(u32, const char*), u32 arg) {
    lbl_eu_80619C10.error_fn = fn;
    lbl_eu_80619C10.error_arg = arg;
}

/* Walk the handle pool for a free slot; returns the slot pointer or NULL.
 * Inlined by MWCC into SFX_Create (retail shows the loop inlined with a
 * `bne next; b found` exit shape, which the `return p` form reproduces). */
static inline SFXHandlePoolEntry* SFX_FindFreeHandle(void) {
    SFXHandlePoolEntry* p;
    s32 count;

    p = lbl_eu_80619C10.handle;
    count = lbl_eu_80619C10.max_handles;
    while (count > 0) {
        if (p->active == 0) {
            return p;
        }
        count--;
        p++;
    }
    return NULL;
}

SFXHandleState* SFX_Create(u32 width, s32 height) {
    SFXHandlePoolEntry* hn;
    void* zmv;
    void* alp;

    hn = SFX_FindFreeHandle();
    if (hn == NULL) {
        return (SFXHandleState*)hn;
    }

    /* The work buffer size must be at least 0x301F bytes. */
    if ((s64)height < 12319) {
        lbl_eu_80619C10.error_count++;
        if (lbl_eu_80619C10.error_fn) {
            lbl_eu_80619C10.error_fn(lbl_eu_80619C10.error_arg,
                                     (const char*)lbl_eu_8051D1AC);
        }
        return NULL;
    }

    sfx_InitHn((SFXHandleState*)hn, width, height);

    zmv = SFXZ_Create();
    if (zmv == NULL) {
        lbl_eu_80619C10.error_count++;
        if (lbl_eu_80619C10.error_fn) {
            lbl_eu_80619C10.error_fn(lbl_eu_80619C10.error_arg,
                                     (const char*)&lbl_eu_8051D1AC[0x29]);
        }
        if (hn != NULL) {
            void* z = hn->zmv;
            u32 a = hn->field_0x30;
            hn->active = 0;
            SFXZ_Destroy(z);
            SFXA_Destroy((void*)a);
            lbl_eu_80619C10.init_count--;
        }
        return NULL;
    }
    hn->zmv = zmv;

    alp = SFXA_Create();
    if (alp == NULL) {
        lbl_eu_80619C10.error_count++;
        if (lbl_eu_80619C10.error_fn) {
            lbl_eu_80619C10.error_fn(lbl_eu_80619C10.error_arg,
                                     (const char*)&lbl_eu_8051D1AC[0x48]);
        }
        if (hn != NULL) {
            void* z = hn->zmv;
            u32 a = hn->field_0x30;
            hn->active = 0;
            SFXZ_Destroy(z);
            SFXA_Destroy((void*)a);
            lbl_eu_80619C10.init_count--;
        }
        return NULL;
    }
    hn->field_0x30 = (u32)alp;

    lbl_eu_80619C10.init_count++;
    return (SFXHandleState*)hn;
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

    void* zmv = hn->zmv;
    u32 f30 = hn->field_0x30;
    hn->active = 0;
    SFXZ_Destroy(zmv);
    SFXA_Destroy((void*)f30);
    lbl_eu_80619C10.init_count--;
}

void SFXLIB_Error(void* self, void* stmInf, const char* msg) {
    void (*fn)(u32, const char*) = lbl_eu_80619C10.error_fn;
    u32 arg = lbl_eu_80619C10.error_arg;
    lbl_eu_80619C10.error_count++;
    if (fn) {
        fn(arg, msg);
    }
}

u32 SFX_SetCcirFx(void) {
    return lbl_eu_80619C10.ccirFx;
}

u32 SFX_GetForceSplitField(void) {
    return lbl_eu_80619C04;
}
