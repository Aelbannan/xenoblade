#include "kyoshin/cf/CfCamDirectionIntf.hpp"

using namespace ml;

namespace cf {

extern "C" {
    void func_80074900(ml::CVec3* out, ml::CVec3* pos274, ml::CVec3 const* dir, f32 unk284);
    // Compute result lookat from base position, current lookat, and direction.
    void func_800749AC(ml::CVec3* pos274, ml::CVec3* lookat, ml::CVec3* out);
    extern const f32 lbl_eu_80666268;

// Stub: CfCamDirectionIntf vtable update entry - does nothing for directional cameras.
void CfCamDirectionIntf_update(CfCamDirectionIntf* /*self*/) {
}

// Copy the position into both the base position and the orbit pivot.
void CfCamDirectionIntf_setPos(CfCamDirectionIntf* /*self*/, CfCamEvent* camEvent, CVec3 const* pos) {
    camEvent->unk28 = *pos;
    camEvent->unk274 = *pos;
}

// Set the camera direction and compute the resulting lookat target.
void CfCamDirectionIntf_setDir(CfCamDirectionIntf* /*self*/, CfCamEvent* camEvent, CVec3 const* dir) {
    CVec3 result;
    func_80074900(&result, &camEvent->unk274, dir, camEvent->unk284);
    camEvent->mLookat = result;
    camEvent->unk25C = *dir;
}

// Set the lookat target directly and derive the direction vector from the
// base position to the new lookat point.
void CfCamDirectionIntf_setLookat(CfCamDirectionIntf* /*self*/, CfCamEvent* camEvent, CVec3 const* lookat) {
    CVec3 result;
    func_800749AC(&camEvent->unk274, &camEvent->mLookat, &result);
    result.z = lbl_eu_80666268;
    camEvent->mLookat = *lookat;
    camEvent->unk25C = result;
}
} // extern "C"

} // namespace cf
