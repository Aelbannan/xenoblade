// Auto-scaffolded catalog TU for CriWare/src/adx/ahx/ahx_sjd
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void AHXSJD_Init() {}

extern volatile s32 lbl_eu_805E64C0;

void AHXSJD_Finish(void) {
    if (lbl_eu_805E64C0 == 1) {
        AHXDCD_Finish();
    }
    --lbl_eu_805E64C0;
}

void AHXSJD_Create() {}

void AHXSJD_Destroy() {}

int AHXSJD_GetStat(void* self) { return *(s8*)((u8*)self + 9); }

typedef struct AHXSJDState {
    s8 status;
    u8 _01[0x0f];
    u32 input;
    u8 _14[0x10];
    u32 decodedDataLength;
    u32 decodedNumSmpl;
    u32 decodedNumSmplBase;
} AHXSJDState;

void AHXSJD_SetInSj(void* self, u32 val) { ((AHXSJDState*)self)->input = val; }

void AHXSJD_Start() {}

extern void AHXDCD_Reset(void*);

void AHXSJD_Stop(void* self) {
    AHXDCD_Reset(*(void**)((u8*)self));
    *(u8*)((u8*)self + 0x09) = 0;
}

void criware_8038CB9C() {}

void AHXSJD_ExecHndl() {}

u32 AHXSJD_GetDecDtLen(void* self) {
    return ((AHXSJDState*)self)->decodedDataLength;
}

u32 AHXSJD_GetDecNumSmpl(void* self) {
    AHXSJDState* state = (AHXSJDState*)self;
    return state->decodedNumSmplBase + state->decodedNumSmpl;
}

void AHXSJD_EntryFltFunc(void* self, void* func, void* ctx) {
    *(void**)((u8*)self + 0x54) = func;
    *(void**)((u8*)self + 0x58) = ctx;
}

void AHXDCD_SetExtPrm(void* a);
void AHXSJD_SetExtPrm(void* self) {
    AHXDCD_SetExtPrm(*(void**)((u8*)self + 0));
}

void AHXSJD_SetDecSmpl(void* self, u32 val) { *(u32*)((u8*)self + 0x34) = val; }

void func_8006BEE4(void* p) {
    u32* p32 = (u32*)p;
    p32[1] = 0;
}

void AHXSJD_SetLnkSw(void* self, int val) {
    if (val == 0) {
        *(u32*)((u8*)self + 0x38) = 0;
    } else {
        *(u32*)((u8*)self + 0x38) = 1;
    }
}

void AHXSJD_TermSupply(void* self) {
    *(u32*)((u8*)self + 0x0C) = 1;
}
