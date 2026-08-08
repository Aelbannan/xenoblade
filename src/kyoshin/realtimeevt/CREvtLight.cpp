// TU: kyoshin/realtimeevt/CREvtLight
// Class: CREvtLight (inherits from cf::CREvtObj)
// Own vtable at 0x80533D90
// Total size: 0x50

#include <types.h>
#include <cstring>
#include <cmath>
#include "kyoshin/realtimeevt/CREvtLight.hpp"
#include "kyoshin/realtimeevt/CREvtObj.hpp"
#include "kyoshin/cf/CfGameManager.hpp"

extern "C" {
// CfGameManager accessor

// Resource management (via CScn at lbl_eu_80663E14)
void* lbl_eu_80663E14;
void* func_804C1BA0(void* mgr, const char* name, int flag);
void  func_804C1D7C(void* mgr, void* handle);

// Other helpers
u32 func_80180940(void);

// CREvtObj constructor/destructor
void __ct__cf_CREvtObj(cf::CREvtObj* self, int arg);
void __dt__Q22cf8CREvtObjFv(cf::CREvtObj* self, int dealloc_flag);
void __dt__80185754(void* ptr);

// Float constants
extern f32 lbl_eu_80667F08;  // 0x3E22F983... (conversion factor)
extern f32 lbl_eu_80667F0C;  // 0x40490FDB... (π)
extern f32 lbl_eu_80667F10;  // 0x3FDF_AED0... (π/2?)
extern f32 lbl_eu_8066A1F8;  // -π
extern f32 lbl_eu_8066A1FC;  // 2π
extern f32 lbl_eu_8066A210;  // π/2
}

// ============================================================================
// Constructor: __ct__CREvtLight
// r3 = this, r4 = arg
// Calls CREvtObj(this, 2), sets own vtable, initializes fields
// ============================================================================
extern "C" void __ct__CREvtLight(CREvtLight* self, u32 arg) {
    __ct__cf_CREvtObj((cf::CREvtObj*)self, 2);
    self->vtable = (void*)lbl_eu_80533D90;
    self->field_14 = 0;
    self->field_18 = arg;
    self->field_20 = 0;
}

// ============================================================================
// __ct__801C3604: Constructor with cleanup (called during reassignment)
// r3 = this, r4 = dealloc_flag
// If this != null, releases old resource, calls CREvtObj dtor, optionally
// deallocates from CREvtMem.
// ============================================================================
extern "C" void __ct__801C3604(CREvtLight* self, int dealloc_flag) {
    if (self == nullptr) {
        return;
    }

    cf::CREvtObj* base = (cf::CREvtObj*)self;
    u32 oldResource = self->field_20;

    // Update vtable before cleanup
    self->vtable = (void*)lbl_eu_80533D90;

    if (oldResource != 0) {
        // Release old resource through CScn manager
        void* mgr = *(void**)((u8*)lbl_eu_80663E14 + 0x7C);
        func_804C1D7C(mgr, (void*)oldResource);
        self->field_20 = 0;

        // Notify game manager
        void* gameMgr = cf::CfGameManager::func_80083298();
        if (gameMgr != nullptr) {
            void* obj = *(void**)((u8*)gameMgr + 0x2F3C);
            if (obj != nullptr) {
                // Call vfunc 0x68 (index 26) on the object
                void** vtable = *(void***)obj;
                typedef void (*VFunc)(void*, int);
                ((VFunc)vtable[0x68 / 4])(obj, 1);
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

// ============================================================================
// func_801C36C4: Set resource
// r3 = this, r4 = resource_name, r5 = field_value
// Releases old resource if present, loads new resource if name != null,
// stores field_value at +0x1C.
// ============================================================================
extern "C" void func_801C36C4(CREvtLight* self, const char* resourceName, u32 fieldValue) {
    // Release old resource
    if (self->field_20 != 0) {
        void* mgr = *(void**)((u8*)lbl_eu_80663E14 + 0x7C);
        func_804C1D7C(mgr, (void*)self->field_20);
        self->field_20 = 0;

        void* gameMgr = cf::CfGameManager::func_80083298();
        if (gameMgr != nullptr) {
            void* obj = *(void**)((u8*)gameMgr + 0x2F3C);
            if (obj != nullptr) {
                void** vtable = *(void***)obj;
                typedef void (*VFunc)(void*, int);
                ((VFunc)vtable[0x68 / 4])(obj, 1);
            }
        }
    }

    // Load new resource
    if (resourceName != nullptr) {
        void* mgr = *(void**)((u8*)lbl_eu_80663E14 + 0x7C);
        void* handle = func_804C1BA0(mgr, resourceName, 7);
        self->field_20 = (u32)handle;

        void* gameMgr = cf::CfGameManager::func_80083298();
        if (gameMgr != nullptr) {
            void* obj = *(void**)((u8*)gameMgr + 0x2F3C);
            if (obj != nullptr) {
                u32 value = func_80180940();
                void** vtable = *(void***)obj;
                typedef void (*VFunc)(void*, u32);
                ((VFunc)vtable[0x68 / 4])(obj, value);
            }
        }
    }

    self->field_1C = fieldValue;
}

// ============================================================================
// func_801C37C4: Empty virtual function
// ============================================================================
extern "C" void func_801C37C4(void) {}

// ============================================================================
// func_801C37C8: Empty virtual function
// ============================================================================
extern "C" void func_801C37C8(void) {}

// ============================================================================
// func_801C37CC: Vector distance to target
// r3 = this, r4 = target object
// Calls vfunc 0xAC on target to get position, computes distance from
// this->pos_3C, returns PSVECMag of the difference.
// ============================================================================
extern "C" f32 func_801C37CC(CREvtLight* self, void* target) {
    // Call vfunc 0xAC (index 43) on target to get position pointer
    void** vtable = *(void***)target;
    typedef void* (*VFunc)(void*);
    f32* pos = (f32*)((VFunc)vtable[0xAC / 4])(target);

    // Compute difference vector
    f32 dx = pos[0] - *(f32*)((u8*)self + 0x3C);
    f32 dy = pos[1] - *(f32*)((u8*)self + 0x40);
    f32 dz = pos[2] - *(f32*)((u8*)self + 0x44);

    // Store as Vec3 on stack
    f32 diff[3] = { dx, dy, dz };

    // Return magnitude
    return PSVECMag((const f32*)diff);
}

// ============================================================================
// func_801C3850: Angle-based state check
// r3 = this, r4 = target object
// Computes horizontal angle from this to target, returns state (1, 2, or 4)
// based on angle thresholds.
// ============================================================================
extern "C" int func_801C3850(CREvtLight* self, void* target) {
    // Get target position
    void** vtable = *(void***)target;
    typedef void* (*VFunc)(void*);
    f32* pos = (f32*)((VFunc)vtable[0xAC / 4])(target);

    // Compute horizontal difference (x, z)
    f32 dx = pos[0] - *(f32*)((u8*)self + 0x3C);
    f32 dz = pos[2] - *(f32*)((u8*)self + 0x44);

    // Compute angle using Atan2FIdx
    f32 angle = Atan2FIdx__Q24nw4r4mathFff(dz, dx);

    // Convert with factor and subtract offset
    f32 result = lbl_eu_80667F08 * angle - *(f32*)((u8*)self + 0x4C);

    // Normalize to [-π, π]
    while (lbl_eu_8066A1F8 > result) {
        result += lbl_eu_8066A1FC;
    }
    while (result < -lbl_eu_8066A1F8) {
        result -= lbl_eu_8066A1FC;
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