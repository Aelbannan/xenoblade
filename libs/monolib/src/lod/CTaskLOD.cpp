#include <types.h>
#include "monolib/lod/CTaskLOD.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/scn/CScn.hpp"
#include "libs/monolib/src/scn/CScnEnvLgtCtrl.hpp"

// DEBT(mangled): updateSubObject (and its callee UnkClass_8046A530::updateLodPositions)
// really take the r4 value syncLODTask receives; until that chain gets real
// signatures the call keeps the retail-named C-linkage declaration.
extern "C" void updateSubObject__Q23LOD17UnkClass_8046D144Fv(LOD::UnkClass_8046D144* self, CTaskLOD* task);

// The one live CTaskLOD instance (published by create).
CTaskLOD* CTaskLOD::spInstance;

// --- FULL_MATCH functions ---

// Static entry points.  Each checks the singleton `spInstance` and, if set,
// forwards its arguments to the active manager (mpActiveLOD at 0x1D40); MWCC
// tail-calls the forwarding call (`b`).

void CTaskLOD::notifyLODTick(float scale) {
    if (spInstance) {
        spInstance->mpActiveLOD->setLodScaleAndRefresh(scale);
    }
}

void CTaskLOD::updateLODFrame(float scale) {
    if (spInstance) {
        spInstance->mpActiveLOD->setBaseScale(scale);
    }
}

void CTaskLOD::enableLOD() {
    if (spInstance) {
        spInstance->mpActiveLOD->setPauseFlag(1);
    }
}

void CTaskLOD::disableLOD() {
    if (spInstance) {
        spInstance->mpActiveLOD->setPauseFlag(0);
    }
}

void CTaskLOD::bindTaskToLOD(void* scene) {
    if (spInstance) {
        spInstance->mpActiveLOD->attachSceneObjects((u32)scene);
    }
}

// --- Remaining harness stubs (empty bodies) ---

template<>
CTTask<CTaskLOD>::~CTTask() {}

// Destroy the embedded LODMemMan member (retail calls its address-named dtor
// __dt__8046D144 with the member-dtor flag -1); the base CProcess dtor call
// (__dt__8CProcessFv) and the deleting-flag operator delete are emitted
// automatically by MWCC.
CTaskLOD::~CTaskLOD() {

}

void CTaskLOD::Init() {}

// Teardown: release the secondary LOD instance (if any), then stop the active
// LODMemMan and release the shared persistent LOD buffer.  The redundant
// inner null-check reproduces retail's doubled beq (MWCC keeps the delete's
// own guard when the guarded store follows); the dtor is called explicitly
// because the retail reloc name (__dt__8046D144) cannot be produced by a C++
// class dtor.
void CTaskLOD::Term() {
    if (mpSecondaryLOD) {
        if (mpSecondaryLOD) {
            delete mpSecondaryLOD;
            mpSecondaryLOD = 0;
        }
    }
    mpActiveLOD->clearManagers();
    LOD::LODMemMan::freePersistentBuffer();
}

void CTaskLOD::Move() { mpActiveLOD->updateLodTick(); }

// Draw is NOT defined here: retail's split has no Draw__8CTaskLODFv body
// (the vtable slot targets CTTask<CTaskLOD>::Draw) and nothing calls it, so
// defining the empty override would add a 4-byte function retail lacks.
// (declaration stays in the class; the virtual is never odr-used)

inline CTaskLOD::CTaskLOD(CScn* scn, CView* view, u32 handle, u32 size)
    : mScn(scn), mView(view), mpSecondaryLOD(0), mpActiveLOD(&mLODMemMan) {
    if (size) {
        LOD::LODMemMan::allocPersistentBuffer(handle, size);
    }
    mpActiveLOD->initElementBuffers(handle, size);
}

CTaskLOD* CTaskLOD::create(CProcess* parent, CScn* scn, CView* view, u32 handle, u32 size) {
    CTaskLOD* t = new (CWorkThreadSystem::getWorkMem()) CTaskLOD(scn, view, handle, size);
    t->Regist(parent, false);
    spInstance = t;
    return t;
}

// Allocate/replace the secondary LODMemMan when `enable` is set, then return
// the active LODMemMan's resetAndReload result (0 when there is no singleton).
bool CTaskLOD::acquireLODResource(void* data, bool enable) {
    if (spInstance) {
        if (enable) {
            CTaskLOD* t = spInstance;
            if (t->mpSecondaryLOD) {
                if (t->mpSecondaryLOD) {
                    delete t->mpSecondaryLOD;
                    t->mpSecondaryLOD = 0;
                }
            }
            LOD::UnkClass_8046D144* lod = (LOD::UnkClass_8046D144*)mtl::MemManager::allocate(
                0x1ce0, mtl::MemManager::getHandleMEM1());
            // No explicit null guard: MWCC's placement-new emits its own
            // (retail has exactly one beq here).  Re-assigning the result
            // keeps `lod` re-derived from the ctor return (retail keeps it
            // in r0 across the call instead of a saved register).
            lod = new (lod) LOD::UnkClass_8046D144();
            t->mpSecondaryLOD = lod;
            t->mpActiveLOD = lod;
            t->mLODMemMan.setPauseFlag(0);
        }
        return spInstance->mpActiveLOD->resetAndReload(
            (u32)data, (u8*)spInstance->mScn);
    }
    return 0;
}

// Teardown + re-bind: release the active LODMemMan's g3d objects, destroy the
// secondary LOD instance, then point mpActiveLOD back at the embedded
// mLODMemMan and enable the LOD system.  The whole body works on the global
// instance; `data` is only forwarded to teardownG3dObjects.
void CTaskLOD::resetActiveLOD(void* data) {
    if (spInstance) {
        spInstance->mpActiveLOD->teardownG3dObjects((int)data);
        CTaskLOD* t = spInstance;
        if (t->mpSecondaryLOD) {
            if (t->mpSecondaryLOD) {
                delete t->mpSecondaryLOD;
                t->mpSecondaryLOD = 0;
            }
        }
        t->mpActiveLOD = &t->mLODMemMan;
        t->mLODMemMan.setPauseFlag(1);
    }
}

// (Re)bind the singleton's active LOD: destroy the secondary instance, point
// mpActiveLOD at the embedded mLODMemMan, and enable the LOD system.  The
// whole body works on the global instance.
void CTaskLOD::restorePrimaryLOD() {
    if (spInstance) {
        CTaskLOD* t = spInstance;
        if (t->mpSecondaryLOD) {
            if (t->mpSecondaryLOD) {
                delete t->mpSecondaryLOD;
                t->mpSecondaryLOD = 0;
            }
        }
        t->mpActiveLOD = &t->mLODMemMan;
        t->mLODMemMan.setPauseFlag(1);
    }
}

// Calls the active LODMemMan's getter for its side-effect-free float access,
// then unconditionally returns the constant 1.0f (retail loads the 1.0f pool
// entry at 0x8066A5B8 after the call).
float CTaskLOD::getLODScale() {
    if (spInstance) {
        spInstance->mpActiveLOD->getBaseScale();
    }
    return 1.0f;
}

// Toggle the 0x40 flag bit on the active LODMemMan at offset 0x6c: set when
// `enable` is non-null, clear when null.
void CTaskLOD::setLODFilterFlag(void* enable) {
    if (spInstance) {
        LOD::UnkClass_8046D144* lod = spInstance->mpActiveLOD;
        if (enable) {
            lod->field_0x6C |= 0x40;
        } else {
            lod->field_0x6C &= ~0x40;
        }
    }
}

// Forwards (1, value - 1, amount) to configureFadeMode when `value` is
// non-zero, otherwise (0, 0, amount).
void CTaskLOD::updateLODRange(u16 value, float amount) {
    if (spInstance) {
        if (value) {
            spInstance->mpActiveLOD->configureFadeMode(1, value - 1, amount);
        } else {
            spInstance->mpActiveLOD->configureFadeMode(0, 0, amount);
        }
    }
}

void CTaskLOD::syncLODTask() {
    if (spInstance) {
        updateSubObject__Q23LOD17UnkClass_8046D144Fv(spInstance->mpActiveLOD, this);
    }
}

// Forwards the three time parameters to the active manager.
void CTaskLOD::dispatchLODArgs(u16 a, u16 b, u16 c) {
    if (spInstance) {
        spInstance->mpActiveLOD->setTimeParams(a, b, c);
    }
}

// LOD activation wrappers: enable/disable element `id` on the active
// manager, then dispatch the same task + flag through the singleton's
// scene-light control list (scnLgtCallLightV26 ring walk).
void CTaskLOD::activateLOD(int id) {
    if (spInstance) {
        spInstance->mpActiveLOD->clearElementFlag(id);
        (spInstance->mScn->mLgtCtrl)->CallLightV26((void*)id, (void*)1);
    }
}

void CTaskLOD::deactivateLOD(int id) {
    if (spInstance) {
        spInstance->mpActiveLOD->setElementFlag(id);
        (spInstance->mScn->mLgtCtrl)->CallLightV26((void*)id, (void*)0);
    }
}

// Setter-style wrapper: forwards the task + flag pair through the active
// manager and the scene-light ring.
void CTaskLOD::setLODEnable(int id, int flag) {
    if (spInstance) {
        spInstance->mpActiveLOD->toggleElementFlag(id, flag);
        (spInstance->mScn->mLgtCtrl)->CallLightV26((void*)id, (void*)flag);
    }
}
// Value-delegates.  Like the void wrappers above, each forwards element `id`
// (retail `mr r4, r3`) to the active manager; the result comes straight back
// from the delegate.
bool CTaskLOD::getLODData(int id) {
    if (spInstance) {
        return spInstance->mpActiveLOD->isElementFlagClear(id);
    }
    return 0;
}

void CTaskLOD::refreshLOD(int id, f32 value) {
    if (spInstance) {
        spInstance->mpActiveLOD->setElementLodLevel(id, value);
    }
}

// Forward (value, id, flag) to configureShrinkTargets, then walk the
// singleton's scene-light control ring (CallLightV27) with the same id/flag
// and the float value.
void CTaskLOD::setLODParam(u32 id, u32 flag, f32 value) {
    if (spInstance) {
        spInstance->mpActiveLOD->configureShrinkTargets(value, id, flag);
        (spInstance->mScn->mLgtCtrl)->CallLightV27((void*)id, (void*)flag, value);
    }
}

// Element id + flag forwarders (tail-call `b`).
void CTaskLOD::addLODEntry(int id, int a) {
    if (spInstance) {
        spInstance->mpActiveLOD->setElementVisibility(id, a);
    }
}

void CTaskLOD::removeLODEntry(int id, f32 value) {
    if (spInstance) {
        spInstance->mpActiveLOD->setElementScale(id, value);
    }
}

void CTaskLOD::clearLODEntry(int id) {
    if (spInstance) {
        spInstance->mpActiveLOD->refreshElementScale(id);
    }
}

f32 CTaskLOD::getLODDistance(int id) {
    if (spInstance) {
        return spInstance->mpActiveLOD->getElementScale(id);
    }
    return 0.0f;
}
void CTaskLOD::attachLODObject(int id, int a) {
    if (spInstance) {
        spInstance->mpActiveLOD->updateElementVisibility(id, a);
    }
}

void CTaskLOD::detachLODObject(int id, int a) {
    if (spInstance) {
        spInstance->mpActiveLOD->setElementFlag80(id, a);
    }
}

void CTaskLOD::setLODObject(int id, u16 a) {
    if (spInstance) {
        spInstance->mpActiveLOD->linkTaskElements(id, a);
    }
}
u8 CTaskLOD::getLODObject(int id) {
    if (spInstance) {
        return spInstance->mpActiveLOD->getElementState(id);
    }
    return 0;
}

void CTaskLOD::updateLODObject(int id, f32 value) {
    if (spInstance) {
        spInstance->mpActiveLOD->setElementCachedValue(id, value);
    }
}

f32 CTaskLOD::getLODLevel(int id) {
    if (spInstance) {
        return spInstance->mpActiveLOD->getElementCachedValue(id);
    }
    return 0.0f;
}

// Forwards element `id` to updateSingleElement (no result).
void CTaskLOD::getLODOrSelf(int id) {
    if (spInstance) {
        spInstance->mpActiveLOD->updateSingleElement(id);
    }
}

// Out-of-line specialisations of the CTTask<CTaskLOD> Move/Draw slots.  These
// emit the retail `Move__17CTTask<8CTaskLOD>Fv` / `Draw__17CTTask<8CTaskLOD>Fv`
// bodies: __ptmf_test on the member-ptr slot (0x3C / 0x48) then __ptmf_scall.
template<>
void CTTask<CTaskLOD>::Move() {
    if (mMoveFunc) {
        (static_cast<CTaskLOD*>(this)->*mMoveFunc)();
    }
}

template<>
void CTTask<CTaskLOD>::Draw() {
    if (mDrawFunc) {
        (static_cast<CTaskLOD*>(this)->*mDrawFunc)();
    }
}
