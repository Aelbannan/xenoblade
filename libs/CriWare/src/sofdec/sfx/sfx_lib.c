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
const char lbl_eu_8051D178[] = "\nCRI SFX/WII Ver.2.48 Build:Oct 28 2009 11:38:06\n";
const unsigned char lbl_eu_8051D1AC[108] = {
    0x45,0x32,0x30,0x31,0x31,0x39,0x34,0x3A,0x20,0x73,0x66,0x78,0x5F,0x49,0x6E,0x69,
    0x74,0x48,0x6E,0x3A,0x20,0x77,0x6F,0x72,0x6B,0x20,0x73,0x69,0x7A,0x65,0x20,0x69,
    0x73,0x20,0x73,0x68,0x6F,0x72,0x74,0x2E,0x00,0x45,0x32,0x30,0x31,0x32,0x38,0x31,
    0x3A,0x20,0x53,0x66,0x78,0x5A,0x48,0x6E,0x3A,0x20,0x63,0x61,0x6E,0x27,0x74,0x20,
    0x63,0x72,0x65,0x61,0x74,0x65,0x2E,0x00,0x45,0x32,0x30,0x32,0x30,0x31,0x31,0x3A,
    0x20,0x53,0x66,0x78,0x41,0x48,0x6E,0x3A,0x20,0x63,0x61,0x6E,0x27,0x74,0x20,0x63,
    0x72,0x65,0x61,0x74,0x65,0x2E,0x00,0x00,0x00,0x00,0x00,0x00
};
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

/* Root block at lbl_eu_80619C00: 0x10-byte header followed by the 0x528-byte
 * library state. SFX_Init zeroes/reinits the library part. */
typedef struct SFXRoot {
    s32 init_count;                      /* 0x00 */
    s32 field_0x04;                      /* 0x04 */
    void (*default_error_fn)(u32, const char*); /* 0x08 */
    u32 field_0x0C;                      /* 0x0C */
    SFXLibState lib;                     /* 0x10 */
} SFXRoot;
SFXRoot lbl_eu_80619C00;
#define lbl_eu_80619C10 (lbl_eu_80619C00.lib)
#define lbl_eu_80619C04 (lbl_eu_80619C00.field_0x04)
#define lbl_eu_8051D178_str lbl_eu_8051D178


void SFX_Init(void) {
    if (lbl_eu_80619C00.init_count < 1) {
        lbl_eu_80619C00.default_error_fn = (void (*)(u32, const char*))&lbl_eu_8051D178;
        memset(&lbl_eu_80619C00.lib, 0, sizeof(SFXLibState));
        lbl_eu_80619C00.lib.max_handles = 8;
        lbl_eu_80619C00.lib.ccirFx = 1;
        CFT_Init();
        SFXSUD_Init();
        SFXZ_Init();
        SFXA_Init();
        lbl_eu_80619C00.field_0x04 = 0;
        lbl_eu_80619C00.init_count++;
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
    if ((unsigned long long)(long long)height >= 12319ULL) {
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
        } else {
        hn->zmv = zmv;

        alp = SFXA_Create();
        if (alp != NULL) {
            hn->field_0x30 = (u32)alp;

            lbl_eu_80619C10.init_count++;
            return (SFXHandleState*)hn;
        } else {
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
        }
    } else {
        lbl_eu_80619C10.error_count++;
        if (lbl_eu_80619C10.error_fn) {
            lbl_eu_80619C10.error_fn(lbl_eu_80619C10.error_arg,
                                     (const char*)lbl_eu_8051D1AC);
        }
        return NULL;
    }
}

void sfx_InitHn(SFXHandleState* hn, u32 width, u32 height) {
    u32 alignW;

    memset(hn, 0, 0xA0);

    /* Nested assignments: innermost add (bufA) allocates first, outermost
     * result lands in r3, matching the retail chain r5/r4/r3. */
    hn->field_0x04 = 0;
    hn->field_0x08 = 0;
    hn->field_0x0C = 0;
    hn->field_0x28 = 1;
    hn->field_0x2C = 0;
    hn->field_0x34 = 0;
    alignW = (width + 0x1F) & ~0x1F;
    hn->alignW = alignW;
    hn->bufCSize = (hn->bufBSize = (hn->bufASize = alignW + 0x400) + 0x400) + 0x400;
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
