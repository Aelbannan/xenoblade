// monolib/src/scn/CMdlMouth - mouth-animation helper object.
#include <harness_catalog.h>
#include <monolib/math.hpp>
#include <nw4r/math.h>
#include <nw4r/g3d/res/g3d_resanmchr.h>
#include <monolib/core/code_804E36DC.hpp>
#include "libs/monolib/src/scn/CMdlMouth.hpp"

extern "C" void func_804E6898(void* self, float val) {
    ((CMdlMouth*)self)->value2C = val;
}

int func_804E68A0(CMdlMouth* self, u32 arg2, nw4r::g3d::ChrAnmResult* res) {
    // Mismatched own-node id: callers may only animate the bound node.
    if ((u32)self->field_0x0C != arg2) {
        return 0;
    }
    // Closing-timer decrement: while it counts down the rest is skipped.
    if (self->field_0x08 != 0 && self->field_0x28 > 0) {
        self->field_0x28 -= 1;
        if (self->field_0x28 > 0) {
            return 0;
        }
    }
    if (self->field_0x08 == 0) {
        return 1;
    }
    // Integrate mouth-open angle from the owner view's delta.
    f32 dt = func_80496288(((CMdlOwnerCtx*)self->field_0x04)->field_0x04);
    self->field_0x10 = self->field_0x14 * dt + self->field_0x10;
    if (self->field_0x10 > lbl_eu_8066A1F8) {
        // Reset to fully closed, then re-randomize open/close speeds.
        self->field_0x10 = lbl_eu_8066B310;
        self->field_0x14 = lbl_eu_8066B310;
        if (self->field_0x08 != 0) {
            self->field_0x14 = lbl_eu_8066B31C * (f32)ml::math::mtRand(100) + lbl_eu_8066B318;
            self->field_0x18 = lbl_eu_8066B320 * (f32)ml::math::mtRand(100) + lbl_eu_8066B318;
        }
    }
    nw4r::math::VEC3 v;
    f32 o = self->field_0x10;
    f32 s = nw4r::math::SinFIdx(lbl_eu_8066B330 * o);
    self->field_0x1C = self->field_0x18 * s * self->value2C;
    res->GetRotateDeg(&v);
    v.z = lbl_eu_8066B334 * self->field_0x1C + v.z;
    res->SetRotateDeg(&v);
    return 1;
}

CMdlMouth::~CMdlMouth() {}

void func_804E679C(CMdlMouth* self, int arg2, int arg3) {
    // All follow-up blocks are guarded by the mode-change condition: retail skips
    // the whole body with a single branch when field_0x08 already equals arg2.
    if (self->field_0x08 != arg2) {
        self->field_0x08 = arg2;
        if (arg2 != 0) {
            self->field_0x14 = lbl_eu_8066B31C * (f32)ml::math::mtRand(100) + lbl_eu_8066B318;
            self->field_0x18 = lbl_eu_8066B320 * (f32)ml::math::mtRand(100) + lbl_eu_8066B318;
        }
        if (arg2 != 0) {
            self->field_0x28 = 2;
        }
        if (arg3 != 0) {
            if (self->field_0x08 == 0) {
                f32 z = lbl_eu_8066B310;
                self->field_0x1C = z;
                self->field_0x10 = z;
                self->field_0x14 = z;
                self->field_0x18 = z;
            }
        }
    }
}

CMdlMouth::CMdlMouth() {
    field_0x04 = 0;
    field_0x08 = 0;
    field_0x0C = -1;
    f32 v = lbl_eu_8066B310;
    field_0x10 = v;
    field_0x14 = v;
    field_0x18 = v;
    field_0x1C = v;
    field_0x24 = -1;
    field_0x28 = 0;
    value2C = lbl_eu_8066B314;
}