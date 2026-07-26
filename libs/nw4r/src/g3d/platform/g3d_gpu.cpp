// Auto-scaffolded catalog TU for nw4r/src/g3d/platform/g3d_gpu
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern "C" void GDSetGenMode2__Q34nw4r3g3d4fifoFUcUcUcUc11_GXCullMode() {}

extern "C" void GDSetCullMode__Q34nw4r3g3d4fifoF11_GXCullMode() {}

extern "C" void GDSetTexCoordScale2__Q34nw4r3g3d4fifoF13_GXTexCoordIDUsUcUcUsUcUc() {}

extern "C" void GDSetIndTexMtx__Q34nw4r3g3d4fifoFUlRCQ34nw4r4math5MTX34() {}

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

extern "C" void GDSetCurrentMtx__Q34nw4r3g3d4fifoFPCUl() {}

extern "C" void GDLoadTexMtxImm3x3__Q34nw4r3g3d4fifoFRCQ34nw4r4math5MTX33Ul() {}
