/* Auto-scaffolded catalog TU for CriWare/src/sofdec/sfx/sfx_alp.
 *
 * SFX alpha (transparency) state pool: one global block (lbl_eu_8061A138)
 * holding a used-count, a slot-count and 8 fixed SFXAlphaState slots.
 * Built as Wii/1.1 - see the configure.py note on SFXA_Create's schedule. */
#include <harness_catalog.h>
#include "libs/CriWare/src/sofdec/sfx/sfx_types.h"

void* memset(void* s, int c, size_t n);
u8 lbl_eu_8061A138[0x128]; /* .bss 0x8061A138 size 0x128 */
void SFXA_Init(void) {
    memset(lbl_eu_8061A138, 0, 0x128);
    ((SFXAlphaState*)lbl_eu_8061A138)->needsUpdate = 8;
}

/* Global alpha-state pool: used-count, slot-count, then 8 fixed slots. */
typedef struct SFXAlphaGlobals {
    u32 usedCount;
    u32 slotCount;
    SFXAlphaState states[8];
} SFXAlphaGlobals;

/* Inlined free-slot scan; returning from inside the loop keeps MWCC's
 * branch-over-branch layout (bne continue / b found) in the inlined body. */
static inline SFXAlphaState* SFXAFindFreeSlot(SFXAlphaGlobals* globals) {
    SFXAlphaState* state = globals->states;
    s32 count = globals->slotCount;
    for (; count > 0; count--) {
        if (state->_00 == 0)
            return state;
        state++;
    }
    return NULL;
}

SFXAlphaState* SFXA_Create(void) {
    SFXAlphaState* state =
        SFXAFindFreeSlot((SFXAlphaGlobals*)lbl_eu_8061A138);
    if (state == NULL)
        return state;
    state->arg0 = 0;
    state->arg1 = 0x1f;
    state->arg2 = 0x64;
    state->needsUpdate = 1;
    state->byte14 = 0;
    state->byte15 = 0x7f;
    state->byte16 = 0xff;
    ((SFXAlphaGlobals*)lbl_eu_8061A138)->usedCount++;
    state->_00 = 1;
    return state;
}
void SFXA_Destroy(void* self) {
    if (self == NULL)
        return;
    ((SFXAlphaState*)self)->_00 = 0;
    ((SFXAlphaGlobals*)lbl_eu_8061A138)->usedCount--;
}
void SFXA_MakeAlpLumiTbl(void* self, u32 a, u32 b, u32 c) {
    SFXAlphaState* state = (SFXAlphaState*)self;
    SFXAlphaCallback cb = state->makeLumiTable;
    u32 a0 = state->arg0;
    u32 a1 = state->arg1;
    u32 a2 = state->arg2;
    if (cb != 0)
        cb(a0, a1, a2, b);
    state->needsUpdate = 0;
}
void SFXA_MakeAlp3110Tbl(void* self, int a, int b) {
    SFXAlphaState* state = (SFXAlphaState*)self;
    int arg2 = b;
    int arg3 = state->byte14;
    int arg4 = state->byte15;
    int arg5 = state->byte16;
    SFXAlphaTableCallback cb = state->makeAlp3110Table;
    if (cb) cb(arg2, arg3, arg4, arg5);
}
void SFXA_MakeAlp3211Tbl(void* self, int a, int b) {
    SFXAlphaState* state = (SFXAlphaState*)self;
    int arg2 = b;
    int arg3 = state->byte14;
    int arg4 = state->byte15;
    int arg5 = state->byte16;
    SFXAlphaTableCallback cb = state->makeAlpTable;
    if (cb) cb(arg2, arg3, arg4, arg5);
}
u32 SFXA_IsNeedUpdateLumiTbl(void* self) { return ((SFXAlphaState*)self)->needsUpdate; }
