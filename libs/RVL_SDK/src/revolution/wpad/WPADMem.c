// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/wpad/WPADMem
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// LLM-HARNESS-BEGIN: us-80378810
void WPADiClearMemBlock(int index) {
    extern void* __rvl_p_wpadcb[];
    void* block = __rvl_p_wpadcb[index];
    *(unsigned int*)((char*)block + 0x998) = 0;
    *(unsigned int*)((char*)block + 0x99c) = 0;
    *(unsigned short*)((char*)block + 0x9a0) = 0;
    *(unsigned int*)((char*)block + 0x9a4) = 0;
    *(unsigned int*)((char*)block + 0x9a8) = 0;
}
// LLM-HARNESS-END: us-80378810

// LLM-HARNESS-BEGIN: us-80378840
void WPADWriteExtReg() {}
// LLM-HARNESS-END: us-80378840
