// Auto-scaffolded catalog TU for CriWare/src/adx/wiirna/ax_rna
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// LLM-HARNESS-BEGIN: us-8039c384
void AXRNA_EntryErrFunc(void) {}
// LLM-HARNESS-END: us-8039c384

// LLM-HARNESS-BEGIN: us-8039c388
void AXRNA_Init() {}
// LLM-HARNESS-END: us-8039c388

// LLM-HARNESS-BEGIN: us-8039c3e0
void AXRNA_Finish() {}
// LLM-HARNESS-END: us-8039c3e0

// LLM-HARNESS-BEGIN: us-8039c464
void axrna_voice_drop() {}
// LLM-HARNESS-END: us-8039c464

// LLM-HARNESS-BEGIN: us-8039c4f8
void AXRNA_Create() {}
// LLM-HARNESS-END: us-8039c4f8

// LLM-HARNESS-BEGIN: us-8039c93c
void AXRNA_Destroy() {}
// LLM-HARNESS-END: us-8039c93c

// LLM-HARNESS-BEGIN: us-8039ca00
void AXRNA_SetTransSw() {}
// LLM-HARNESS-END: us-8039ca00

// LLM-HARNESS-BEGIN: us-8039cd00
void AXRNA_SetPlaySw() {}
// LLM-HARNESS-END: us-8039cd00

// LLM-HARNESS-BEGIN: us-8039ce90
void AXRNA_GetNumData() {}
// LLM-HARNESS-END: us-8039ce90

// LLM-HARNESS-BEGIN: us-8039cf04
typedef struct AXRNA { char pad0[3]; signed char type; char pad4[0x2c]; void *objs[1]; } AXRNA; int AXRNA_GetNumRoom(AXRNA *rna) { if (rna == NULL) { return -1; } void *obj = rna->objs[rna->type - 1]; int (*func)(void *, int) = ((int (**)(void *, int))(*(void ***)obj))[9]; return (unsigned int)func(obj, 0) >> 1; }
// LLM-HARNESS-END: us-8039cf04

// LLM-HARNESS-BEGIN: us-8039cf60
void axrna_update_play() {}
// LLM-HARNESS-END: us-8039cf60

// LLM-HARNESS-BEGIN: us-8039d0b0
void axrna_start_trans() {}
// LLM-HARNESS-END: us-8039d0b0

// LLM-HARNESS-BEGIN: us-8039d2bc
void criware_80399F4C() {}
// LLM-HARNESS-END: us-8039d2bc

// LLM-HARNESS-BEGIN: us-8039d698
void axrna_start_flash() {}
// LLM-HARNESS-END: us-8039d698

// LLM-HARNESS-BEGIN: us-8039d7f8
void AXRNA_ExecServer() {}
// LLM-HARNESS-END: us-8039d7f8

// LLM-HARNESS-BEGIN: us-8039d8fc
void AXRNA_SetNumChan(void* self, u8 numChan) {
    if (self != NULL) {
        *(u8*)((u8*)self + 0x03) = numChan;
    }
}
// LLM-HARNESS-END: us-8039d8fc

// LLM-HARNESS-BEGIN: us-8039d90c
void AXRNA_SetSfreq() {}
// LLM-HARNESS-END: us-8039d90c

// LLM-HARNESS-BEGIN: us-8039d9f8
void AXRNA_SetOutVol() {}
// LLM-HARNESS-END: us-8039d9f8

// LLM-HARNESS-BEGIN: us-8039daa8
void AXRNA_SetOutPan() {}
// LLM-HARNESS-END: us-8039daa8

// LLM-HARNESS-BEGIN: us-8039db60
void AXRNA_SetBitPerSmpl(void* self, u32 val) {
    if (self != NULL) {
        *(u32*)((u8*)self + 0x78) = val;
    }
}
// LLM-HARNESS-END: us-8039db60

// LLM-HARNESS-BEGIN: us-8039db70
int AXRNA_SetStmHdInfo(void) { return 0x0; }
// LLM-HARNESS-END: us-8039db70

// LLM-HARNESS-BEGIN: us-8039db78
int AXRNA_DiscardData(void) { return 0x0; }
// LLM-HARNESS-END: us-8039db78

// LLM-HARNESS-BEGIN: us-8039db80
void AXRNA_SetMain() {}
// LLM-HARNESS-END: us-8039db80

// LLM-HARNESS-BEGIN: us-8039dc44
void criware_8039A8E0() {}
// LLM-HARNESS-END: us-8039dc44
