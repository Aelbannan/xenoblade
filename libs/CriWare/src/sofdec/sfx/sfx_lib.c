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
// name-guess: retail .rodata 0x8051D178 is the SFX/WII version string that
// SFX_Init stores in sfxlib_header's default-error-function slot.
const char sCriSfxVersion[] = "\nCRI SFX/WII Ver.2.48 Build:Oct 28 2009 11:38:06\n";
void sfx_InitHn(SFXHandleState* hn, u32 width, u32 height);

/* ---- SFX library global state ----
 * sfxlib_state is a 0x528-byte structure:
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

/* The 0x10-byte header and the 0x528-byte library state are separate retail
 * objects; SFX_Init zeroes/reinits the library part. */
typedef struct SFXLibHeader {
    s32 init_count;                      /* 0x00 */
    s32 field_0x04;                      /* 0x04 */
    void (*default_error_fn)(u32, const char*); /* 0x08 */
    u32 field_0x0C;                      /* 0x0C */
} SFXLibHeader;
SFXLibHeader sfxlib_header;
SFXLibState sfxlib_state;
void SFX_Init(void) {
    if (sfxlib_header.init_count < 1) {
        sfxlib_header.default_error_fn = (void (*)(u32, const char*))sCriSfxVersion;
        memset(&sfxlib_state, 0, sizeof(SFXLibState));
        sfxlib_state.max_handles = 8;
        sfxlib_state.ccirFx = 1;
        CFT_Init();
        SFXSUD_Init();
        SFXZ_Init();
        SFXA_Init();
        sfxlib_header.field_0x04 = 0;
        sfxlib_header.init_count++;
    }
}

void SFX_SetErrFn(void (*fn)(u32, const char*), u32 arg) {
    sfxlib_state.error_fn = fn;
    sfxlib_state.error_arg = arg;
}

/* Walk the handle pool for a free slot; returns the slot pointer or NULL.
 * Inlined by MWCC into SFX_Create (retail shows the loop inlined with a
 * `bne next; b found` exit shape, which the `return p` form reproduces). */
static inline SFXHandlePoolEntry* SFX_FindFreeHandle(void) {
    SFXHandlePoolEntry* p;
    s32 count;

    p = sfxlib_state.handle;
    count = sfxlib_state.max_handles;
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
    if ((unsigned long long)(long long)height < 12319ULL) {
        sfxlib_state.error_count++;
        if (sfxlib_state.error_fn) {
            sfxlib_state.error_fn(sfxlib_state.error_arg,
                                  "E201194: sfx_InitHn: work size is short.");
        }
        return NULL;
    }

    sfx_InitHn((SFXHandleState*)hn, width, height);

    zmv = SFXZ_Create();
    if (zmv == NULL) {
        sfxlib_state.error_count++;
        if (sfxlib_state.error_fn) {
            sfxlib_state.error_fn(sfxlib_state.error_arg,
                                  "E201281: SfxZHn: can't create.");
        }
        if (hn != NULL) {
            void* z = hn->zmv;
            u32 a = hn->field_0x30;
            hn->active = 0;
            SFXZ_Destroy(z);
            SFXA_Destroy((void*)a);
            sfxlib_state.init_count--;
        }
        return NULL;
    }

    hn->zmv = zmv;

    alp = SFXA_Create();
    if (alp != NULL) {
        hn->field_0x30 = (u32)alp;
        sfxlib_state.init_count++;
        return (SFXHandleState*)hn;
    }

    sfxlib_state.error_count++;
    if (sfxlib_state.error_fn) {
        sfxlib_state.error_fn(sfxlib_state.error_arg,
                              "E202011: SfxAHn: can't create.");
    }
    if (hn != NULL) {
        void* z = hn->zmv;
        u32 a = hn->field_0x30;
        hn->active = 0;
        SFXZ_Destroy(z);
        SFXA_Destroy((void*)a);
        sfxlib_state.init_count--;
    }
    return NULL;
}

void sfx_InitHn(SFXHandleState* hn, u32 width, u32 height) {
    memset(hn, 0, 0xA0);
    hn->field_0x04 = 0;
    hn->field_0x08 = 0;
    hn->field_0x0C = 0;
    hn->field_0x28 = 1;
    hn->field_0x2C = 0;
    hn->field_0x34 = 0;
    hn->alignW = (width + 0x1F) & ~0x1F;
    hn->bufASize = hn->alignW + 0x400;
    hn->bufBSize = hn->bufASize + 0x400;
    hn->bufCSize = hn->bufBSize + 0x400;
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
    sfxlib_state.init_count--;
}

void SFXLIB_Error(void* self, void* stmInf, const char* msg) {
    void (*fn)(u32, const char*) = sfxlib_state.error_fn;
    u32 arg = sfxlib_state.error_arg;
    sfxlib_state.error_count++;
    if (fn) {
        fn(arg, msg);
    }
}

u32 SFX_SetCcirFx(void) {
    return sfxlib_state.ccirFx;
}

u32 SFX_GetForceSplitField(void) {
    return sfxlib_header.field_0x04;
}
