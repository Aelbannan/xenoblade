#include <nw4r/g3d.h>

namespace nw4r {
namespace g3d {
namespace detail {

// Retail 0.0f constant (sdata2 lbl_eu_80669BE0); referenced by name so the
// PS-store kernels' lfs stays sda21-addressed and this TU stays data-free.
extern "C" { extern const f32 lbl_eu_80669BE0; }

void Copy32ByteBlocks(register void* pDst, register const void* pSrc,
                      register u32 size) {
    register f32 work0, work1, work2, work3;

    for (size /= 32; size > 0; size--) {
        ASM (
            lfd  work0, 0(pSrc)
            stfd work0, 0(pDst)

            lfd  work1, 8(pSrc)
            stfd work1, 8(pDst)

            lfd  work2, 16(pSrc)
            stfd work2, 16(pDst)
            
            lfd  work3, 24(pSrc)
            stfd work3, 24(pDst)
        )

        pDst = static_cast<u8*>(pDst) + 32;
        pSrc = static_cast<const u8*>(pSrc) + 32;
    }
}

void ZeroMemory32ByteBlocks(register void* pDst, register u32 size) {
    register f32 zero = lbl_eu_80669BE0;

    for (size /= 32; size > 0; size--) {
        ASM (
            psq_st zero,  0(pDst), 0, 0
            psq_st zero,  8(pDst), 0, 0
            psq_st zero, 16(pDst), 0, 0
            psq_st zero, 24(pDst), 0, 0
        )

        pDst = static_cast<u8*>(pDst) + 32;
    }
}

void ZeroMemory16ByteBlocks(register void* pDst, register u32 size) {
    register f32 zero = lbl_eu_80669BE0;

    for (size /= 16; size > 0; size--) {
        ASM (
            psq_st zero,  0(pDst), 0, 0
            psq_st zero,  8(pDst), 0, 0
        )

        pDst = static_cast<u8*>(pDst) + 16;
    }
}

} // namespace detail
} // namespace g3d
} // namespace nw4r
