// TU: kyoshin/realtimeevt/CREvtLight
// Class: CREvtLight (inherits from cf::CREvtObj)
// Own vtable at 0x80533D90
// Total size: 0x50

#include <types.h>
#include <nw4r/math.h>
#include <revolution/MTX.h>
#include "kyoshin/realtimeevt/CREvtLight.hpp"
#include "kyoshin/realtimeevt/CREvtObj.hpp"
#include "include/kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"
#include "kyoshin/cf/object/CfObject.hpp"
#include "kyoshin/cf/object/CfObjectMap.hpp"
#include "monolib/math/CVec3.hpp"
// CScnEnvLgtCtrlListItem is the real owner of the +0x2F3C map-effect object
// (v24 at slot 0x68 takes the retail int flag; cf. CfObjectMap.cpp).
#include "libs/monolib/src/scn/CScnEnvLgtCtrl.hpp"

// Local view of getGameSubManager() past +0x2F3C (map-effect list item).
struct GameSubMapFxView {
    u8 pad_00[0x2F3C];
    CScnEnvLgtCtrlListItem* mapFx; // 0x2F3C
};

// Local view of lbl_eu_80663E14 past +0x7C (resource mgr); same slot as
// UnkSceneView::field_0x7C in CfObjectMap.hpp.
struct SceneResMgrView {
    u8 pad_00[0x7C];
    void* resMgr; // 0x7C
};


// Resource globals / imports (C ABI from external TUs). scnLgtCreateEnvController is
// declared in CfObjectMap.hpp as (void*, const void*, int) to cover both
// const char* (CREvtLight) and void* (CfObjectMap) call sites.
extern "C" void scnLgtRemoveLgtItem(void* mgr, void* handle);
extern "C" void* scnLgtCreateEnvController(void* mgr, const void* name, int flag);
extern "C" u32   REvtCam_IsTaskListEmpty(void);

// Own vtable and math constants / helpers.
extern "C" u32 lbl_eu_80533D90[];
extern "C" void  __ct__cf_CREvtObj(cf::CREvtObj* self, int arg);
extern "C" void  __dt__Q22cf8CREvtObjFv(cf::CREvtObj* self, int dealloc_flag);
extern "C" void  __dt__80185754(void* ptr);

extern "C" const f32 lbl_eu_80667F08;  // angle conversion factor
extern "C" const f32 lbl_eu_80667F0C;  // pi
extern "C" const f32 lbl_eu_80667F10;  // pi/2 threshold base
extern "C" const f32 lbl_eu_8066A1F8;  // -pi
extern "C" const f32 lbl_eu_8066A1FC;  // 2pi
extern "C" const f32 lbl_eu_8066A210;  // pi/2

// ============================================================================
// Constructor: __ct__CREvtLight
// r3 = this, r4 = arg
// Calls CREvtObj(this, 2), sets own vtable, initializes fields
// ============================================================================
extern "C" CREvtLight* __ct__CREvtLight(CREvtLight* self, u32 arg) {
    __ct__cf_CREvtObj((cf::CREvtObj*)self, 2);
    *(u32**)self = (u32*)((u32*)lbl_eu_80533D90);
    self->field_14 = 0;
    self->field_18 = arg;
    self->field_20 = nullptr;
    return self;
}

// ============================================================================
// __ct__801C3604: Reassignment constructor with cleanup
// r3 = this, r4 = dealloc_flag
// If this != null, releases old resource, calls CREvtObj dtor, optionally
// deallocates from CREvtMem.
// ============================================================================
CREvtLight* __ct__801C3604(CREvtLight* self, int dealloc_flag) {
    if (self != nullptr) {
        cf::CREvtObj* base = (cf::CREvtObj*)self;
        void* oldResource = self->field_20;

        // Update vtable before cleanup
        *(u32**)self = (u32*)((u32*)lbl_eu_80533D90);

        if (oldResource != nullptr) {
            // Release old resource through CScn manager
            void* mgr = reinterpret_cast<SceneResMgrView*>(lbl_eu_80663E14)->resMgr;
            scnLgtRemoveLgtItem(mgr, oldResource);
            self->field_20 = nullptr;

            // Notify the object behind the game manager's +0x2F3C pointer.
            if (cf::CfGameManager::getGameSubManager() != nullptr) {
                if (reinterpret_cast<GameSubMapFxView*>(
                        cf::CfGameManager::getGameSubManager())->mapFx != nullptr) {
                    CScnEnvLgtCtrlListItem* fx = reinterpret_cast<GameSubMapFxView*>(
                        cf::CfGameManager::getGameSubManager())->mapFx;
                    fx->v24(1);
                }
            }
        }

        // Call base destructor
        __dt__Q22cf8CREvtObjFv(base, 0);

        // Optionally deallocate from CREvtMem
        if (dealloc_flag > 0) {
            __dt__80185754(self);
        }
    }

    return self;
}

// ============================================================================
// CREvtLight_SetResource: Set resource
// r3 = this, r4 = resource_name, r5 = field_value
// Releases old resource if present, loads new resource if name != null,
// stores field_value at +0x1C.
// ============================================================================
extern "C" void CREvtLight_SetResource(CREvtLight* self, const char* resourceName, u32 fieldValue) {
    // Release old resource
    if (self->field_20 != nullptr) {
        void* mgr = reinterpret_cast<SceneResMgrView*>(lbl_eu_80663E14)->resMgr;
        scnLgtRemoveLgtItem(mgr, self->field_20);
        self->field_20 = nullptr;

        if (cf::CfGameManager::getGameSubManager() != nullptr) {
                if (reinterpret_cast<GameSubMapFxView*>(
                        cf::CfGameManager::getGameSubManager())->mapFx != nullptr) {
                    CScnEnvLgtCtrlListItem* fx = reinterpret_cast<GameSubMapFxView*>(
                        cf::CfGameManager::getGameSubManager())->mapFx;
                    fx->v24(1);
                }
            }
    }

    // Load new resource
    if (resourceName != nullptr) {
        void* mgr = reinterpret_cast<SceneResMgrView*>(lbl_eu_80663E14)->resMgr;
        void* handle = scnLgtCreateEnvController(mgr, resourceName, 7);
        self->field_20 = handle;

        if (cf::CfGameManager::getGameSubManager() != nullptr) {
            if (reinterpret_cast<GameSubMapFxView*>(
                    cf::CfGameManager::getGameSubManager())->mapFx != nullptr) {
                CScnEnvLgtCtrlListItem* fx = reinterpret_cast<GameSubMapFxView*>(
                    cf::CfGameManager::getGameSubManager())->mapFx;
                fx->v24(REvtCam_IsTaskListEmpty());
            }
        }
    }

    self->field_1C = fieldValue;
}

// ============================================================================
// CREvtLight_noop7C4: Empty virtual function
// ============================================================================
void CREvtLight_noop7C4(void) {}

// ============================================================================
// CREvtLight_noop7C8: Empty virtual function
// ============================================================================
void CREvtLight_noop7C8(void) {}

// ============================================================================
// CREvtLight_DistToTarget: Vector distance to target
// r3 = this, r4 = target object
// Calls vfunc 0xAC on target to get position, computes distance from
// this->mPos (0x3C), returns PSVECMag of the difference.
// ============================================================================
extern "C" f32 CREvtLight_DistToTarget(CREvtLight* self, cf::CfObject* target) {
    // Get target position via CfObject vtable slot 0xAC (CfObject_getPosVector)
    ml::CVec3* rawPos = target->CfObject_getPosVector();
    nw4r::math::VEC3* pos = reinterpret_cast<nw4r::math::VEC3*>(rawPos);

    // diff = targetPos - thisPos via VEC3::operator- (paired-single block),
    // then element-copied to a Vec for the magnitude call (retail stages
    // the sub result through a temporary before PSVECMag).
    nw4r::math::VEC3 tmp = *pos - *self->pos();
    Vec d, *dp = &d;
    dp->x = tmp.x;
    dp->y = tmp.y;
    dp->z = tmp.z;
    return PSVECMag(dp);
}

// ============================================================================
// CREvtLight_AngleState: Angle-based state check
// r3 = this, r4 = target object
// Computes horizontal angle from this to target, returns state (1, 2, or 4)
// based on angle thresholds.
// ============================================================================
extern "C" int CREvtLight_AngleState(CREvtLight* self, cf::CfObject* target) {
    // Get target position via CfObject vtable slot 0xAC (CfObject_getPosVector)
    ml::CVec3* rawPos = target->CfObject_getPosVector();
    nw4r::math::VEC3* pos = reinterpret_cast<nw4r::math::VEC3*>(rawPos);

    // diff = targetPos - thisPos via VEC3::operator- (paired-single block),
    // then element-copied to a Vec for the angle computation below.
    nw4r::math::VEC3 tmp = *pos - *self->pos();
    Vec diff;
    diff.x = tmp.x;
    diff.y = tmp.y;
    diff.z = tmp.z;

    // Compute angle using Atan2FIdx
    f32 angle = nw4r::math::Atan2FIdx(diff.x, diff.z);

    // Convert with factor, then subtract offset. Computed in two steps so
    // MWCC keeps the retail's separate fmuls/fsubs (no fp-contract fusion).
    f32 scaled = lbl_eu_80667F08 * angle;
    f32 result = scaled - self->mAngle;

    // Normalize result into the expected wrap range. The first condition is
    // written constant-first to match retail's fcmpo(pi, result) + cror lt.
    while (lbl_eu_8066A1F8 <= result) {
        result -= lbl_eu_8066A1FC;
    }
    while (result < -lbl_eu_8066A1F8) {
        result += lbl_eu_8066A1FC;
    }

    // Check thresholds
    f32 threshold = lbl_eu_80667F0C * lbl_eu_8066A210;
    f32 halfThreshold = lbl_eu_80667F10 * threshold;

    if (result > halfThreshold) {
        return 2;
    }
    if (result > threshold) {
        return 4;
    }
    if (result < -halfThreshold) {
        return 2;
    }
    if (result < -threshold) {
        return 4;
    }
    return 1;
}
// ============================================================================
// Retail data owned by this TU (dissolved blob; decomp emitted .text only).
// Defined here at EOF so -ipa codegen for the functions above is unaffected.
// The .sdata2 float trio (F08/F0C/F10) is defined below with exact retail
// bytes (F10 as f32 2.0f + a 4-byte zero tail word); consecutive const f32
// objects pack at 4-align with no padding, and codegen is unchanged.
// ============================================================================

// Cross-TU vtable slots (flat retail names).
extern "C" void CREvtObjVfunc08Default();
extern "C" void EvtObj_ConstFalse();
extern "C" void CREvtObjVfunc10Default();
extern "C" void REvtCam_AlwaysTrue1();
extern "C" void CREvtObjIsBusyDefault();
extern u32 lbl_eu_80662648[];  // sdata word pair (defined below).

// [.data] 0x80533D90-0x80533DB8 (40B): CREvtLight vtable (sdata locator,
// null word, dtor, and 7 virtual slots; retail lays one label over all 10).
u32 lbl_eu_80533D90[10] = {
    (u32)&lbl_eu_80662648, 0x00000000,
    (u32)&__ct__801C3604, (u32)&CREvtLight_noop7C8,
    (u32)&CREvtObjVfunc08Default, (u32)&EvtObj_ConstFalse,
    (u32)&CREvtObjVfunc10Default, (u32)&REvtCam_AlwaysTrue1,
    (u32)&CREvtObjIsBusyDefault, (u32)&CREvtLight_noop7C4,
};

// [.rodata] 0x80505360-0x80505370 (16B): class-name string (unreferenced pool
// bytes, kept under the retail label; sized [0x10] to carry the trailing pad).
__declspec(section ".rodata") __attribute__((used, aligned(4)))
char lbl_eu_80505360[0x10] = {
    0x63,0x66,0x3A,0x3A,0x43,0x52,0x45,0x76,0x74,0x4C,0x69,0x67,0x68,0x74,0x00,0x00,
};

// [.sdata] 0x80662648-0x80662650 (8B): null word pair referenced by vtable[0].
#pragma section data_type ".sdata" ".sdata"
extern __declspec(section ".sdata") u32 lbl_eu_80662648[2] = { 0x00000000, 0x00000000 };
#pragma section data_type ".data" ".data"

// [.sdata2] 0x80667F08-0x80667F18 (16B): lfs-read float trio + 4 zero tail
// (section 8-align pad). F08 = pi/128 angle factor, F0C = 60.0, F10 = 2.0.
#pragma section data_type ".sdata2" ".sdata2"
extern "C" __declspec(section ".sdata2") const f32 lbl_eu_80667F08 = 0.024543693f;
extern "C" __declspec(section ".sdata2") const f32 lbl_eu_80667F0C = 60.0f;
extern "C" __declspec(section ".sdata2") const f32 lbl_eu_80667F10 = 2.0f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
u32 CREvtLight_sdata2_tail[1] = { 0x00000000 };
#pragma section data_type ".data" ".data"
