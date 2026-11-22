// TU: kyoshin/realtimeevt/CREvtLight
// Class: cf::CREvtLight (inherits from cf::CREvtObj)
// Own vtable at 0x80533D90
// Total size: 0x24

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
#include "monolib/scn/CScn.hpp"  // CScn::mLgtCtrl (env-controller list owner)
// CScnEnvLgtCtrlListItem is the real owner of the +0x2F3C map-effect object
// (v24 at slot 0x68 takes the retail int flag; cf. CfObjectMap.cpp).
#include "libs/monolib/src/scn/CScnEnvLgtCtrl.hpp"

// Local view of getGameSubManager() past +0x2F3C (map-effect list item).
struct GameSubMapFxView {
    u8 pad_00[0x2F3C];
    CScnEnvLgtCtrlListItem* mapFx; // 0x2F3C
};



// Resource globals / imports (C ABI from external TUs). scnLgtCreateEnvController /
// scnLgtRemoveLgtItem are declared in CfObjectMap.hpp (owner = CScn::mLgtCtrl).
extern "C" u32   REvtCam_IsTaskListEmpty(void);

// ============================================================================
// Constructor: base CREvtObj(2) (light kind), then the light fields.
// ============================================================================
cf::CREvtLight::CREvtLight(u32 arg) : CREvtObj(2) {
    field_14 = 0;
    field_18 = arg;
    field_20 = nullptr;
}

// ============================================================================
// Destructor (vtable slot 0): releases the light resource and notifies the
// map-effect object; the CREvtObj base dtor and class operator delete
// (CREvtMem arena) follow implicitly.
// ============================================================================
cf::CREvtLight::~CREvtLight() {
    // (No `oldResource` local: a local here adds redundant destroy-base
    // actions to the retail exception table.)
    if (field_20 != nullptr) {
        CScnEnvLgtCtrlResList* mgr = gmScene->mLgtCtrl;
        scnLgtRemoveLgtItem(mgr, field_20);
        field_20 = nullptr;
        if (cf::CfGameManager::getGameSubManager() != nullptr) {
            if (reinterpret_cast<GameSubMapFxView*>(cf::CfGameManager::getGameSubManager())->mapFx != nullptr) {
                reinterpret_cast<GameSubMapFxView*>(cf::CfGameManager::getGameSubManager())->mapFx->v24(1);
            }
        }
    }
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
        CScnEnvLgtCtrlResList* mgr = gmScene->mLgtCtrl;
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
        CScnEnvLgtCtrlResList* mgr = gmScene->mLgtCtrl;
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
// vfunc_1C / vfunc_04: empty overrides
// ============================================================================
void cf::CREvtLight::vfunc_1C() {}

void cf::CREvtLight::vfunc_04() {}

// ============================================================================
// CREvtLight_DistToTarget: Vector distance to target
// r3 = this, r4 = target object
// Calls vfunc 0xAC on target to get position, computes distance from
// this->mPos (0x3C), returns PSVECMag of the difference.
// ============================================================================
extern "C" f32 CREvtLight_DistToTarget(CREvtLightPose* self, cf::CfObject* target) {
    // Get target position via CfObject vtable slot 0xAC (CfObject_getPosVector)
    ml::CVec3* rawPos = target->CfObject_getPosVector();
    nw4r::math::VEC3* pos = reinterpret_cast<nw4r::math::VEC3*>(rawPos);

    // diff = targetPos - thisPos via VEC3::operator- (paired-single block),
    // then element-copied to a Vec for the magnitude call (retail stages
    // the sub result through a temporary before PSVECMag). Plain Vec with
    // no intermediate pointer, matching the accepted AngleState sibling;
    // the unpinned &d lets the scheduler hoist it above the PS block.
    nw4r::math::VEC3 tmp = *pos - *self->pos();
    Vec d;
    d.x = tmp.x;
    d.y = tmp.y;
    d.z = tmp.z;
    return PSVECMag(&d);
}

// ============================================================================
// CREvtLight_AngleState: Angle-based state check
// r3 = this, r4 = target object
// Computes horizontal angle from this to target, returns state (1, 2, or 4)
// based on angle thresholds.
// ============================================================================
extern "C" int CREvtLight_AngleState(CREvtLightPose* self, cf::CfObject* target) {
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
    f32 scaled = 0.024543693f * angle;
    f32 result = scaled - self->mAngle;

    // Normalize result into the expected wrap range. The first condition is
    // written constant-first to match retail's fcmpo(pi, result) + cror lt.
    while (ml::pi <= result) {
        result -= ml::tau;
    }
    while (result < -ml::pi) {
        result += ml::tau;
    }

    // Check thresholds
    f32 threshold = 60.0f * ml::deg2rad;
    f32 halfThreshold = 2.0f * threshold;

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
