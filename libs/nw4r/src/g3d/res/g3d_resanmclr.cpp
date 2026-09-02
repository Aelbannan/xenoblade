#include <nw4r/g3d.h>

namespace nw4r {
namespace g3d {
namespace detail {

// Retail sdata2 pool constants: ClipFrame's 0.0f comes from the shared pool
// (lbl_eu_80669AC8); the u16->f32 conversion magic (2^52 double,
// lbl_eu_80669AD0) is compiler-generated — now explicitly referenced
// to avoid TU-local sdata2 pooling (absorb: retail sdata2 is 0).
extern "C" const float lbl_eu_80669AC8;
extern "C" const double lbl_eu_80669AD0;

inline f32 U16ToF32_shared(u16 v) {
    union { double d; struct { u32 hi; u32 lo; } w; } u;
    u.w.hi = 0x43300000;
    u.w.lo = v;
    return (f32)(u.d - lbl_eu_80669AD0);
}

inline f32 ClipFrameLocal(const ResAnmClrInfoData& rInfo, f32 frame) {
    if (frame <= lbl_eu_80669AC8) {
        return lbl_eu_80669AC8;
    }

    f32 numFrameF = U16ToF32_shared(rInfo.numFrame);
    if (numFrameF <= frame) {
        return numFrameF;
    }

    return frame;
}

} // namespace detail

void ResAnmClr::GetAnmResult(ClrAnmResult* pResult, u32 idx, f32 frame) const {
    const ResAnmClrMatData* pMatData = GetMatAnm(idx);
    const ResAnmClrAnmData* pAnmData = pMatData->anms;
    const ResAnmClrInfoData& rInfoData = ref().info;

    u32 flags = pMatData->flags;
    pResult->bRgbaExist = 0;

    if (flags == 0) {
        return;
    }

    f32 clippedFrame = detail::ClipFrameLocal(rInfoData, frame);

    for (int i = 0; i < ClrAnmResult::CLA_MAX;
         flags >>= ResAnmClrMatData::NUM_OF_FLAGS, i++) {

        if (!(flags & ResAnmClrMatData::FLAG_ANM_EXISTS)) {
            continue;
        }

        bool constant = flags & ResAnmClrMatData::FLAG_ANM_CONSTANT;

        pResult->bRgbaExist |= 1 << i;
        pResult->rgbaMask[i] = pAnmData->mask;
        pResult->rgba[i] = detail::GetResColorAnmResult(&pAnmData->color,
                                                        clippedFrame, constant);

        pAnmData++;
    }
}

} // namespace g3d
} // namespace nw4r
