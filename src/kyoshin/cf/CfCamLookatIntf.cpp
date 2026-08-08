#include "kyoshin/cf/CfCamLookatIntf.hpp"

using namespace ml;

namespace cf {

extern "C" {
    void func_80074488(ml::CVec3* out, ml::CVec3* lookat, ml::CVec3 const* dir, f32 unk284);
    // Compute result position from lookat, direction, and camera speed.
    extern const f32 lbl_eu_80666268;

// Stub: CfCamLookatIntf vtable update entry - does nothing for lookat cameras.
void CamLookatIntf_update(CfCamLookatIntf* /*self*/) {
}

// Copy the position and reset the direction Z to the default factor.
void CamLookatIntf_setPos(CfCamLookatIntf* /*self*/, CfCamEvent* camEvent, CVec3 const* pos) {
    camEvent->unk28 = *pos;
    camEvent->unk274 = *pos;
    camEvent->unk25C.z = lbl_eu_80666268;
}

// Set the camera direction and compute the resulting base position.
void CamLookatIntf_setDir(CfCamLookatIntf* /*self*/, CfCamEvent* camEvent, CVec3 const* dir) {
    CVec3 result;
    func_80074488(&result, &camEvent->mLookat, dir, camEvent->unk284);
    camEvent->unk274 = result;
    camEvent->unk25C = *dir;
}

// Set the lookat target directly (lookat cameras don't derive direction from position).
void CamLookatIntf_setLookat(CfCamLookatIntf* /*self*/, CfCamEvent* camEvent, CVec3 const* lookat) {
    camEvent->mLookat = *lookat;
    camEvent->unk25C.z = lbl_eu_80666268;
}
} // extern "C"

} // namespace cf