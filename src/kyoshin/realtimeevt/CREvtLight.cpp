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
#include "kyoshin/cf/object/CfObject.hpp"
#include "kyoshin/cf/object/CfObjectMap.hpp"
#include "monolib/math/CVec3.hpp"

// Resource globals / imports (C ABI from external TUs). func_804C1BA0 is
// declared in CfObjectMap.hpp as (void*, const void*, int) to cover both
// const char* (CREvtLight) and void* (CfObjectMap) call sites.
extern "C" void func_804C1D7C(void* mgr, void* handle);
extern "C" u32   func_80180940(void);

// Own vtable and math constants / helpers.
extern "C" void* lbl_eu_80533D90[];
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
CREvtLight* __ct__CREvtLight(CREvtLight* self, u32 arg) {
    __ct__cf_CREvtObj((cf::CREvtObj*)self, 2);
    self->vtable = (void*)lbl_eu_80533D90;
    self->field_14 = 0;
    self->field_18 = arg;
    self->field_20 = 0;
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
        u32 oldResource = self->field_20;

        // Update vtable before cleanup
        self->vtable = (void*)lbl_eu_80533D90;

        if (oldResource != 0) {
            // Release old resource through CScn manager
            void* mgr = ((void**)lbl_eu_80663E14)[0x7C / 4];
            func_804C1D7C(mgr, (void*)oldResource);
            self->field_20 = 0;

            // Notify the object behind the game manager's +0x2F3C pointer.
            if (cf::CfGameManager::getGameSubManager() != nullptr) {
                if (*(void**)((u8*)cf::CfGameManager::getGameSubManager() + 0x2F3C) != nullptr) {
                    UnkMapFxObj* fx =
                        (UnkMapFxObj*)*(void**)((u8*)cf::CfGameManager::getGameSubManager() + 0x2F3C);
                    fx->vfunc_0x68(1);
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
// func_801C36C4: Set resource
// r3 = this, r4 = resource_name, r5 = field_value
// Releases old resource if present, loads new resource if name != null,
// stores field_value at +0x1C.
// ============================================================================
void func_801C36C4(CREvtLight* self, const char* resourceName, u32 fieldValue) {
    // Release old resource
    if (self->field_20 != 0) {
        void* mgr = ((void**)lbl_eu_80663E14)[0x7C / 4];
        func_804C1D7C(mgr, (void*)self->field_20);
        self->field_20 = 0;

        if (cf::CfGameManager::getGameSubManager() != nullptr) {
            if (*(void**)((u8*)cf::CfGameManager::getGameSubManager() + 0x2F3C) != nullptr) {
                UnkMapFxObj* fx =
                    (UnkMapFxObj*)*(void**)((u8*)cf::CfGameManager::getGameSubManager() + 0x2F3C);
                fx->vfunc_0x68(1);
            }
        }
    }

    // Load new resource
    if (resourceName != nullptr) {
        void* mgr = ((void**)lbl_eu_80663E14)[0x7C / 4];
        void* handle = func_804C1BA0(mgr, resourceName, 7);
        self->field_20 = (u32)handle;

        if (cf::CfGameManager::getGameSubManager() != nullptr) {
            if (*(void**)((u8*)cf::CfGameManager::getGameSubManager() + 0x2F3C) != nullptr) {
                UnkMapFxObj* fx =
                    (UnkMapFxObj*)*(void**)((u8*)cf::CfGameManager::getGameSubManager() + 0x2F3C);
                fx->vfunc_0x68(func_80180940());
            }
        }
    }

    self->field_1C = fieldValue;
}

// ============================================================================
// func_801C37C4: Empty virtual function
// ============================================================================
void func_801C37C4(void) {}

// ============================================================================
// func_801C37C8: Empty virtual function
// ============================================================================
void func_801C37C8(void) {}

// ============================================================================
// func_801C37CC: Vector distance to target
// r3 = this, r4 = target object
// Calls vfunc 0xAC on target to get position, computes distance from
// this->mPos (0x3C), returns PSVECMag of the difference.
// ============================================================================
f32 func_801C37CC(CREvtLight* self, cf::CfObject* target) {
    // Get target position via CfObject vtable slot 0xAC (CfObject_UnkVirtualFunc23)
    ml::CVec3* rawPos = target->CfObject_UnkVirtualFunc23();
    nw4r::math::VEC3* pos = reinterpret_cast<nw4r::math::VEC3*>(rawPos);

    // diff = targetPos - thisPos; copied through a temporary so MWCC emits
    // the same paired-single block plus element copy before PSVECMag.
    nw4r::math::VEC3 delta;
    nw4r::math::VEC3Sub(&delta, pos, self->pos());
    Vec d;
    d.x = delta.x;
    d.y = delta.y;
    d.z = delta.z;
    return PSVECMag(&d);
}

// ============================================================================
// func_801C3850: Angle-based state check
// r3 = this, r4 = target object
// Computes horizontal angle from this to target, returns state (1, 2, or 4)
// based on angle thresholds.
// ============================================================================
int func_801C3850(CREvtLight* self, cf::CfObject* target) {
    // Get target position via CfObject vtable slot 0xAC (CfObject_UnkVirtualFunc23)
    ml::CVec3* rawPos = target->CfObject_UnkVirtualFunc23();
    nw4r::math::VEC3* pos = reinterpret_cast<nw4r::math::VEC3*>(rawPos);

    // diff = targetPos - thisPos (paired-single ops)
    nw4r::math::VEC3 delta;
    nw4r::math::VEC3Sub(&delta, pos, self->pos());
    Vec diff;
    diff.x = delta.x;
    diff.y = delta.y;
    diff.z = delta.z;

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