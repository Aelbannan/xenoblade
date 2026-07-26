// Auto-scaffolded catalog TU for nw4r/src/g3d/platform/g3d_gpu
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// LLM-HARNESS-BEGIN: us-803e97cc
extern "C" void GDSetGenMode2__Q34nw4r3g3d4fifoFUcUcUcUc11_GXCullMode() {}
// LLM-HARNESS-END: us-803e97cc

// LLM-HARNESS-BEGIN: us-803e984c
extern "C" void GDSetCullMode__Q34nw4r3g3d4fifoF11_GXCullMode() {}
// LLM-HARNESS-END: us-803e984c

// LLM-HARNESS-BEGIN: us-803e987c
extern "C" void GDSetTexCoordScale2__Q34nw4r3g3d4fifoF13_GXTexCoordIDUsUcUcUsUcUc() {}
// LLM-HARNESS-END: us-803e987c

// LLM-HARNESS-BEGIN: us-803e98ec
extern "C" void GDSetIndTexMtx__Q34nw4r3g3d4fifoFUlRCQ34nw4r4math5MTX34() {}
// LLM-HARNESS-END: us-803e98ec

// LLM-HARNESS-BEGIN: us-803e9b84
extern "C" void GDResetCurrentMtx__Q34nw4r3g3d4fifoFv() {
    volatile unsigned char* const fifo = (volatile unsigned char*)0xCC008000;
    *fifo = 0x08;
    *fifo = 0x30;
    *(volatile unsigned int*)fifo = 0x3cf40000 - 0x3100;
    *fifo = 0x08;
    *fifo = 0x40;
    *(volatile unsigned int*)fifo = 0x00f40000 - 0x30c4;
    *fifo = 0x10;
    *(volatile unsigned short*)fifo = 0x0001;
    *(volatile unsigned short*)fifo = 0x1018;
    *(volatile unsigned int*)fifo = 0x3cf40000 - 0x3100;
    *(volatile unsigned int*)fifo = 0x00f40000 - 0x30c4;
}
// LLM-HARNESS-END: us-803e9b84

// LLM-HARNESS-BEGIN: us-803e9be0
extern "C" void GDSetCurrentMtx__Q34nw4r3g3d4fifoFPCUl() {}
// LLM-HARNESS-END: us-803e9be0

// LLM-HARNESS-BEGIN: us-803e9c5c
extern "C" void GDLoadTexMtxImm3x3__Q34nw4r3g3d4fifoFRCQ34nw4r4math5MTX33Ul() {}
// LLM-HARNESS-END: us-803e9c5c
