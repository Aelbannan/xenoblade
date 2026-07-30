#include <nw4r/g3d.h>

namespace nw4r {
namespace g3d {

bool AnmObjShp::TestExistence(u32 idx) const {
    return (mpBinding[idx] & (BINDING_UNDEFINED | BINDING_INVALID)) == 0;
}

bool AnmObjShp::TestDefined(u32 idx) const {
    return (mpBinding[idx] & BINDING_UNDEFINED) == 0;
}

void AnmObjShp::Release() {
    for (int i = 0; i < mNumBinding; i++) {
        mpBinding[i] = BINDING_UNDEFINED;
    }

    SetAnmFlag(FLAG_ANM_BOUND, false);
}

AnmObjShpRes* AnmObjShp::Attach(int /*idx*/, AnmObjShpRes* /*pRes*/) {
    return NULL;
}

AnmObjShpRes* AnmObjShp::Detach(int /*idx*/) {
    return NULL;
}

void AnmObjShp::DetachAll() {}

void AnmObjShp::SetWeight(int /*idx*/, f32 /*weight*/) {}

f32 AnmObjShp::GetWeight(int /*idx*/) const {
    return 0.0f;
}

} // namespace g3d
} // namespace nw4r
