//! @file g3d_resanmfog.cpp
//! Evaluate a fog animation resource at a given frame.

#include <nw4r/g3d.h>

namespace nw4r {
namespace g3d {

//! Evaluate the fog animation at @a frame, storing the result in @a pResult.
//!
//! The fog channel is animated through three independent tracks (start Z,
//! end Z, and colour). Each track may be constant over the whole animation.
//! The per-track defaults are taken from the owning ResAnmScn resource so the
//! fog stays in sync with the rest of the scene animation.
void ResAnmFog::GetAnmResult(FogAnmResult* pResult, f32 frame) const {
    const ResAnmFogData& r = ref();
    const u32 flags = r.flags;

    // Frame range is governed by the owning scene's animation info.
    const ResAnmScnInfoData& rInfoData =
        ofs_to_ptr<ResAnmScnData>(r.toResAnmScnData)->info;

    const f32 clippedFrame = detail::ClipFrame(rInfoData, frame);

    const bool startZConstant = (flags & ResAnmFogData::FLAG_START_CONST) != 0;
    const bool endZConstant = (flags & ResAnmFogData::FLAG_END_CONST) != 0;
    const bool colorConstant = (flags & ResAnmFogData::FLAG_COLOR_CONST) != 0;

    pResult->type = r.type;
    pResult->startz = detail::GetResAnmResult(&r.startz, frame, startZConstant);
    pResult->endz = detail::GetResAnmResult(&r.endz, frame, endZConstant);
    pResult->color =
        detail::GetResColorAnmResult(&r.color, clippedFrame, colorConstant);
}

} // namespace g3d
} // namespace nw4r
