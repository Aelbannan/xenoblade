// nw4r/src/g3d/platform/g3d_gpu — fifo GPU command emitters
// Matched against retail via hexdiff; GX fifo writes via the 0xCC008000 port.

#include <nw4r/g3d/platform/g3d_gpu.h>

namespace nw4r {
namespace g3d {
namespace fifo {

void GDSetGenMode2(u8 numTexGens, u8 numChans, u8 numTevs, u8 numInds,
                   GXCullMode cullMode) {
    volatile unsigned char* const fifo = (volatile unsigned char*)0xCC008000;

    register const f32* cullTbl = &lbl_eu_80669BC8;
    const unsigned char* cull = (const unsigned char*)cullTbl;

    *fifo = 0x61;
    *(volatile unsigned int*)fifo = 0xFE07FC3F;

    *fifo = 0x61;
    *(volatile unsigned int*)fifo =
        numTexGens | (numChans << 4) | ((numTevs - 1) << 10) |
        (cull[cullMode] << 14) | (numInds << 16);

    *fifo = 0x10;
    *(volatile unsigned short*)fifo = 0x0000;
    *(volatile unsigned short*)fifo = 0x1009;
    *(volatile unsigned int*)fifo = numChans;

    *fifo = 0x10;
    *(volatile unsigned short*)fifo = 0x0000;
    *(volatile unsigned short*)fifo = 0x103F;
    *(volatile unsigned int*)fifo = numTexGens;
}

void GDSetCullMode(GXCullMode cullMode) {
    volatile unsigned char* const fifo = (volatile unsigned char*)0xCC008000;

    register const f32* cullTbl = &lbl_eu_80669BC8;
    const unsigned char* cull = (const unsigned char*)cullTbl;

    *fifo = 0x61;
    *(volatile unsigned int*)fifo = 0xFE00C000;

    *fifo = 0x61;
    *(volatile unsigned int*)fifo = cull[cullMode] << 14;
}

void GDSetTexCoordScale2(GXTexCoordID coord, u16 scaleS, GXBool biasS,
                         GXBool wrapS, u16 scaleT, GXBool biasT,
                         GXBool wrapT) {
    volatile unsigned char* const fifo = (volatile unsigned char*)0xCC008000;

    u32 tex0 = (scaleS - 1) | (biasS << 16) | (wrapS << 17) |
               ((coord * 2 + 0x30) << 24);
    u32 tex1 = (scaleT - 1) | (biasT << 16) | (wrapT << 17) |
               ((coord * 2 + 0x31) << 24);

    *fifo = 0x61;
    *(volatile unsigned int*)fifo = 0xFE03FFFF;

    *fifo = 0x61;
    *(volatile unsigned int*)fifo = tex0;

    *fifo = 0x61;
    *(volatile unsigned int*)fifo = tex1;
}

void GDResetCurrentMtx() {
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

void GDSetCurrentMtx(const u32* pIdArray) {
    volatile unsigned char* const fifo = (volatile unsigned char*)0xCC008000;

    u32 a0 = pIdArray[0];
    u32 a1 = pIdArray[1];
    u32 a2 = pIdArray[2];
    u32 a3 = pIdArray[3];
    u32 b0 = pIdArray[4];
    u32 b1 = pIdArray[5];
    u32 b2 = pIdArray[6];
    u32 b3 = pIdArray[7];

    *fifo = 0x10;
    *(volatile unsigned short*)fifo = 0x0001;
    *(volatile unsigned short*)fifo = 0x1018;
    *(volatile unsigned int*)fifo =
        (a0 << 6) | (a1 << 12) | (a2 << 18) | (a3 << 24);
    *(volatile unsigned int*)fifo =
        b0 | (b1 << 6) | (b2 << 12) | (b3 << 18);
}

} // namespace fifo
} // namespace g3d
} // namespace nw4r

extern "C" void GDSetIndTexMtx__Q34nw4r3g3d4fifoFUlRCQ34nw4r4math5MTX34(u32 id, const nw4r::math::MTX34& rMtx) {}
extern "C" void GDLoadTexMtxImm3x3__Q34nw4r3g3d4fifoFRCQ34nw4r4math5MTX33Ul(const nw4r::math::MTX33& rMtx, u32 id) {}
