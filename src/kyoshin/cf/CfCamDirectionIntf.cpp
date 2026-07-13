#include "kyoshin/cf/CfCamDirectionIntf.hpp"

extern "C" {
    void func_80074900(ml::CVec3* out, ml::CVec3* pos274, ml::CVec3 const* dir, f32 unk284);
    void func_800749AC(ml::CVec3* pos274, ml::CVec3* lookat, ml::CVec3* out);
    extern const f32 lbl_eu_80666268;
}

using namespace ml;

namespace cf {

extern "C" {

void CfCamDirectionIntf_update(void*) {
}

void CfCamDirectionIntf_setPos(void*, CfCamEvent* camEvent, CVec3 const* pos) {
    camEvent->unk28 = *pos;
    camEvent->unk274 = *pos;
}

void CfCamDirectionIntf_setDir(void*, CfCamEvent* camEvent, CVec3 const* dir) {
    CVec3 result;
    func_80074900(&result, &camEvent->unk274, dir, camEvent->unk284);
    camEvent->mLookat = result;
    camEvent->unk25C = *dir;
}

void CfCamDirectionIntf_setLookat(void*, CfCamEvent* camEvent, CVec3 const* lookat) {
    CVec3 result;
    func_800749AC(&camEvent->unk274, &camEvent->mLookat, &result);
    result.z = lbl_eu_80666268;
    camEvent->mLookat = *lookat;
    camEvent->unk25C = result;
}

} // extern "C"

} // namespace cf
