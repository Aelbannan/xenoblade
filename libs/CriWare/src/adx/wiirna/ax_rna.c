// Auto-scaffolded catalog TU for CriWare/src/adx/wiirna/ax_rna
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void AXRNA_EntryErrFunc(void) {}

void AXRNA_Init() {}

void AXRNA_Finish() {}

void axrna_voice_drop() {}

void AXRNA_Create() {}

void AXRNA_Destroy() {}

void AXRNA_SetTransSw() {}

void AXRNA_SetPlaySw() {}

void AXRNA_GetNumData() {}

typedef struct AXRNA { char pad0[3]; signed char type; char pad4[0x2c]; void *objs[1]; } AXRNA; int AXRNA_GetNumRoom(AXRNA *rna) { if (rna == NULL) { return -1; } void *obj = rna->objs[rna->type - 1]; int (*func)(void *, int) = ((int (**)(void *, int))(*(void ***)obj))[9]; return (unsigned int)func(obj, 0) >> 1; }

void axrna_update_play() {}

void axrna_start_trans() {}

void criware_80399F4C() {}

void axrna_start_flash() {}

void AXRNA_ExecServer() {}

void AXRNA_SetNumChan(void* self, u8 numChan) {
    if (self != NULL) {
        *(u8*)((u8*)self + 0x03) = numChan;
    }
}

void AXRNA_SetSfreq() {}

void AXRNA_SetOutVol() {}

void AXRNA_SetOutPan() {}

void AXRNA_SetBitPerSmpl(void* self, u32 val) {
    if (self != NULL) {
        *(u32*)((u8*)self + 0x78) = val;
    }
}

int AXRNA_SetStmHdInfo(void) { return 0x0; }

int AXRNA_DiscardData(void) { return 0x0; }

void AXRNA_SetMain() {}

void criware_8039A8E0() {}
