// TU: kyoshin/realtimeevt/CREvtLight
// Class: CREvtLight (inherits from cf::CREvtObj)
// Own vtable at 0x80533D90
// Total size: 0x50

#include <types.h>
#include <nw4r/math.h>
#include <revolution/MTX.h>
#include "kyoshin/realtimeevt/CREvtLight.hpp"
#include "kyoshin/realtimeevt/CREvtObj.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "monolib/math/CVec3.hpp"

// Resource globals / imports (C ABI from external TUs).
// lbl_eu_80663E14 must be at global scope (not anonymous namespace) so the
// SDA21 reloc keeps the plain symbol name.
extern void* lbl_eu_80663E14;
extern "C" void* func_804C1BA0(void* mgr, const char* name, int flag);
extern "C" void  func_804C1D7C(void* mgr, void* handle);
extern "C" u32   func_80180940(void);

// Own vtable and math constants / helpers.
extern "C" void* lbl_eu_80533D90[];
extern "C" void  __ct__cf_CREvtObj(cf::CREvtObj* self, int arg);
extern "C" void  __dt__Q22cf8CREvtObjFv(cf::CREvtObj* self, int dealloc_flag);
extern "C" void  __dt__80185754(void* ptr);

extern "C" f32 lbl_eu_80667F08;  // angle conversion factor
extern "C" f32 lbl_eu_80667F0C;  // pi
extern "C" f32 lbl_eu_80667F10;  // pi/2 threshold base
extern "C" f32 lbl_eu_8066A1F8;  // -pi
extern "C" f32 lbl_eu_8066A1FC;  // 2pi
extern "C" f32 lbl_eu_8066A210;  // pi/2

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
            if (cf::CfGameManager::func_80083298() != nullptr) {
                if (*(void**)((u8*)cf::CfGameManager::func_80083298() + 0x2F3C) != nullptr) {
                    CREvtLightNotifyIf* notif =
                        (CREvtLightNotifyIf*)*(void**)((u8*)cf::CfGameManager::func_80083298() + 0x2F3C);
                    notif->_v068(1);
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

        if (cf::CfGameManager::func_80083298() != nullptr) {
            if (*(void**)((u8*)cf::CfGameManager::func_80083298() + 0x2F3C) != nullptr) {
                CREvtLightNotifyIf* notif =
                    (CREvtLightNotifyIf*)*(void**)((u8*)cf::CfGameManager::func_80083298() + 0x2F3C);
                notif->_v068(1);
            }
        }
    }

    // Load new resource
    if (resourceName != nullptr) {
        void* mgr = ((void**)lbl_eu_80663E14)[0x7C / 4];
        void* handle = func_804C1BA0(mgr, resourceName, 7);
        self->field_20 = (u32)handle;

        if (cf::CfGameManager::func_80083298() != nullptr) {
            if (*(void**)((u8*)cf::CfGameManager::func_80083298() + 0x2F3C) != nullptr) {
                CREvtLightNotifyIf* notif =
                    (CREvtLightNotifyIf*)*(void**)((u8*)cf::CfGameManager::func_80083298() + 0x2F3C);
                notif->_v068(func_80180940());
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
// diff = a - b with retail's paired-single block, copying into the caller's
// Vec. The `out` register parameter forces MWCC to materialize &diff at the
// call site (before the block), reproducing retail's early addi r0; the work
// vars declared in reverse order give the retail FPR allocation (f0/f1/f3).
static inline void lightSubOut(register Vec* out,
                               register const nw4r::math::VEC3* pA,
                               register const nw4r::math::VEC3* pB) {
    nw4r::math::VEC3 t;
    register nw4r::math::VEC3* pOut = &t;
    register f32 work2, work1, work0;
    ASM (
        psq_l  work0, 0x0(pA), 0, 0
        psq_l  work1, 0x0(pB), 0, 0
        ps_sub work2, work0, work1
        psq_l  work0, 0x8(pA), 1, 0
        psq_l  work1, 0x8(pB), 1, 0
        psq_st work2, 0x0(pOut), 0, 0
        ps_sub work2, work0, work1
        psq_st work2, 0x8(pOut), 1, 0
    )
    out->x = t.x;
    out->y = t.y;
    out->z = t.z;
}

// Local copy of the nw4r VEC3Sub paired-single inline with the register
// vars declared in REVERSE order - the declaration order drives the FPR
// allocation (MWCC_REFERENCE), yielding retail's f0/f1/f3 block here.
static inline nw4r::math::VEC3* lightVecSubRev(register nw4r::math::VEC3* pOut,
                                               register const nw4r::math::VEC3* pA,
                                               register const nw4r::math::VEC3* pB) {
    register f32 work2, work1, work0;
    ASM (
        psq_l  work0, 0x0(pA), 0, 0
        psq_l  work1, 0x0(pB), 0, 0
        ps_sub work2, work0, work1
        psq_l  work0, 0x8(pA), 1, 0
        psq_l  work1, 0x8(pB), 1, 0
        psq_st work2, 0x0(pOut), 0, 0
        ps_sub work2, work0, work1
        psq_st work2, 0x8(pOut), 1, 0
    )
    return pOut;
}

f32 func_801C37CC(CREvtLight* self, CREvtLightTargetIf* target) {
    // Get target position via vtable slot 0xAC
    nw4r::math::VEC3* pos = target->_v0AC();

    // diff = targetPos - thisPos; the temporary copy feeds PSVECMag.
    Vec diff;
    lightSubOut(&diff, pos, self->pos());

    return PSVECMag(&diff);
}

// ============================================================================
// func_801C3850: Angle-based state check
// r3 = this, r4 = target object
// Computes horizontal angle from this to target, returns state (1, 2, or 4)
// based on angle thresholds.
// ============================================================================
int func_801C3850(CREvtLight* self, CREvtLightTargetIf* target) {
    // Get target position via vtable slot 0xAC
    nw4r::math::VEC3* pos = target->_v0AC();

    // diff = targetPos - thisPos (paired-single ops)
    nw4r::math::VEC3 t;
    lightVecSubRev(&t, pos, self->pos());
    Vec diff;
    diff.x = t.x;
    diff.y = t.y;
    diff.z = t.z;

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