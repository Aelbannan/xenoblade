// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfx/sfx_alp
#include <harness_catalog.h>
#include "libs/CriWare/src/sofdec/sfx/sfx_types.h"

void* memset(void* s, int c, size_t n);
extern u8 lbl_eu_8061A138[0x128];
void SFXA_Init(void) {
    memset(lbl_eu_8061A138, 0, 0x128);
    ((SFXAlphaState*)lbl_eu_8061A138)->needsUpdate = 8;
}

SFXAlphaState* SFXA_Create(void) {
    SFXAlphaState* state;
    s32 count = *(s32*)(lbl_eu_8061A138 + 4);

    state = (SFXAlphaState*)(lbl_eu_8061A138 + 8);
    for (; count > 0; count--) {
        if (state->_00 == 0)
            goto found;
        state++;
    }
    state = NULL;
found:
    if (state == NULL)
        return state;
    state->arg0 = 0;
    state->arg1 = 0x1f;
    state->arg2 = 0x64;
    state->needsUpdate = 1;
    state->byte14 = 0;
    state->byte15 = 0x7f;
    state->byte16 = 0xff;
    *(s32*)lbl_eu_8061A138 += 1;
    state->_00 = 1;
    return state;
}
void SFXA_Destroy(void* self) {
    extern u8 lbl_eu_8061A138[];
    if (self == NULL) return;
    *(u32*)((u8*)self) = 0;
    *(s32*)lbl_eu_8061A138 -= 1;
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
    if (state->makeAlpTable) state->makeAlpTable(b, state->byte14, state->byte15, state->byte16);
}
u32 SFXA_IsNeedUpdateLumiTbl(void* self) { return ((SFXAlphaState*)self)->needsUpdate; }
