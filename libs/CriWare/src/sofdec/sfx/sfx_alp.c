// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfx/sfx_alp
#include <harness_catalog.h>
#include "libs/CriWare/src/sofdec/sfx/sfx_types.h"

void* memset(void* s, int c, size_t n);
extern u8 lbl_eu_8061A138[0x128];
void SFXA_Init(void) {
    memset(lbl_eu_8061A138, 0, 0x128);
    ((SFXAlphaState*)lbl_eu_8061A138)->needsUpdate = 8;
}
void SFXA_Create() {}
void SFXA_Destroy(void* self) {
    extern u8 lbl_eu_8061A138[];
    if (self == NULL) return;
    *(u32*)((u8*)self) = 0;
    *(s32*)lbl_eu_8061A138 -= 1;
}
void SFXA_MakeAlpLumiTbl(void* self, u32 a, u32 b, u32 c) {
    SFXAlphaState* state = (SFXAlphaState*)self;
    if (state->makeLumiTable) state->makeLumiTable(state->arg0, state->arg1, state->arg2, c);
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
