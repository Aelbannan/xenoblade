#include "kyoshin/cf/CfCamTargetIntf.hpp"

extern "C" {
    void func_80074488(ml::CVec3* out, ml::CVec3* lookat, ml::CVec3 const* dir, f32 unk284);
    extern const f32 lbl_eu_80666268;
}

using namespace ml;

namespace cf {

extern "C" {

void CfCamTargetIntf_update(void*) {
}

void CfCamTargetIntf_setPos(void*, CfCamEvent* camEvent, CVec3 const* pos) {
    camEvent->unk28 = *pos;
    camEvent->unk274 = *pos;
    camEvent->unk25C.z = lbl_eu_80666268;
}

void CfCamTargetIntf_setDir(void*, CfCamEvent* camEvent, CVec3 const* dir) {
    CVec3 result;
    func_80074488(&result, &camEvent->mLookat, dir, camEvent->unk284);
    camEvent->unk274 = result;
    camEvent->unk25C = *dir;
}

void CfCamTargetIntf_setLookat(void*, CfCamEvent* camEvent, CVec3 const* lookat) {
    camEvent->mLookat = *lookat;
    camEvent->unk25C.z = lbl_eu_80666268;
}

} // extern "C"

} // namespace cf
