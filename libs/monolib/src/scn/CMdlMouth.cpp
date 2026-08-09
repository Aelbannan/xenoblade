// monolib/src/scn/CMdlMouth - mouth-animation helper object.
#include <harness_catalog.h>
#include <string.h>
#include <monolib/math.hpp>
#include <nw4r/math.h>
#include <nw4r/g3d/res/g3d_resmdl.h>
#include <nw4r/g3d/res/g3d_resnode.h>
#include <nw4r/g3d/res/g3d_resmat.h>
#include <nw4r/g3d/res/g3d_resanmchr.h>
#include <nw4r/db/db_assert.h>
#include <monolib/core/code_804E36DC.hpp>
#include "libs/monolib/src/scn/CMdlMouth.hpp"

// External monolib C-ABI function (binds a node id to the owner view).
void func_80488F44(void* arg, u32 nodeId);

extern "C" void func_804E6898(u8* self, float val) {
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

void func_804E65CC(CMdlMouth* self, void* arg2) {
    // Member field init (mirrors the retail ctor stores).
    self->field_0x04 = arg2;
    self->field_0x08 = 0;
    self->field_0x0C = -1;
    f32 z = lbl_eu_8066B310;
    self->field_0x1C = z;
    self->field_0x10 = z;
    self->field_0x14 = z;
    self->field_0x18 = z;
    self->field_0x24 = -1;
    self->field_0x28 = 0;
    self->value2C = lbl_eu_8066B314;

    nw4r::g3d::ResMdl resMdl(reinterpret_cast<nw4r::g3d::ResMdlData*>(
        *reinterpret_cast<u32*>(reinterpret_cast<u8*>(arg2) + 0x146C)));

    nw4r::g3d::ResNode node = resMdl.GetResNode(lbl_eu_80663C58);

    // Bind the named node if present; the inner panic is a DB_ASSERT on the
    // same validity check, so MWCC reuses the outer compare.
    if (node.IsValid()) {
        if (!node.IsValid()) {
            nw4r::db::Panic(lbl_eu_8056E1C8, 0x38, lbl_eu_8056E1A8);
        }
        self->field_0x0C = node.GetID();
        if (!node.IsValid()) {
            nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                            lbl_eu_80663910, lbl_eu_80663C6C);
        }
        self->field_0x20 = node.ref().rot.z;
        func_80488F44(arg2, self->field_0x0C);
    }

    // Scan materials for the eye-material name and stash its id.
    u32 num = resMdl.GetResMatNumEntries();
    for (u32 i = 0; i < num; i++) {
        nw4r::g3d::ResMat resMat = resMdl.GetResMat(i);
        if (!resMat.IsValid()) {
            nw4r::db::Panic(lbl_eu_8056E068, 0x26d, lbl_eu_8056E04C,
                            lbl_eu_806638E8, lbl_eu_80663C68);
        }
        const char* nm = resMat.ofs_to_ptr<char>(resMat.ref().name);
        if (strstr(nm, lbl_eu_80663C5C) != NULL) {
            if (!resMat.IsValid()) {
                nw4r::db::Panic(lbl_eu_8056E068, 0x26d, lbl_eu_8056E04C,
                                lbl_eu_806638E8, lbl_eu_80663C68);
            }
            self->field_0x24 = resMat.ref().id;
            break;
        }
    }
}

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