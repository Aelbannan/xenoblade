#include <types.h>
#include "monolib/work/CTTask.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/util/MemManager.hpp"
#include "libs/monolib/src/scn/CScnEnvLgtCtrl.hpp"

class CTaskLOD;

#include "monolib/lod/UnkClass_8046D144.hpp"

// Pointee of CTaskLOD::mParam1 (offset 0x54): an opaque manager object whose
// +0x7C slot holds the scene-light control list head handed to scnLgtCallLightV26
// by the activateLOD/deactivateLOD/setLODEnable wrappers.
struct LODParam1Obj {
    u8 _00[0x7C];
    CScnEnvLgtCtrl* field_0x7C;  // +0x7C light-control list head
};

// CTaskLOD wraps a LODMemMan and manages its lifecycle.
// Inherits from CTTask<CTaskLOD> (0x54: CDoubleListNode + vtable + CProcess +
// Move/Draw member-pointer slots).
// Layout (0x1D44 total):
//   0x00-0x53 : CTTask<CTaskLOD> base
//   0x54 ..    : unknown state fields (not used by the matched delegates)
//   0x1D40    : mpActiveLOD (points to the LODMemMan to delegate to)
class CTaskLOD : public CTTask<CTaskLOD> {
public:
    CTaskLOD(LODParam1Obj* p1, void* p2, u32 handle, u32 size);
    virtual ~CTaskLOD();
    virtual void Init();
    virtual void Term();
    virtual void Move();
    void resetActiveLOD();
    void enableLOD();
    void disableLOD();
    void restorePrimaryLOD();
    void bindTaskToLOD();
    static void notifyLODTick(float scale);
    static void updateLODFrame(float scale);
    float getLODScale();
    void setLODFilterFlag();
    void updateLODRange();
    void syncLODTask();
    void activateLOD();
    void deactivateLOD();
    void* getLODData();
    void refreshLOD();
    void removeLODEntry();
    void clearLODEntry();
    float getLODDistance();
    void* getLODObject();
    void updateLODObject();
    float getLODLevel();
    CTaskLOD* getLODOrSelf();

    LODParam1Obj* mParam1;              // 0x54
    void* mParam2;                      // 0x58
    LOD::UnkClass_8046D144 mLODMemMan;          // 0x5C (local class size 0x70)
    LOD::UnkClass_8046D144* mpSecondaryLOD;     // 0x1D3C
    LOD::UnkClass_8046D144* mpActiveLOD;        // 0x1D40
};

// Global instance pointer referenced by the delegate wrappers.  Retail owns a
// single 8-byte, 8-aligned .sbss slot here (the pointer plus a 4-byte
// anonymous tail); define it as a 2-element array so the object reproduces
// the retail size and alignment.  All accesses go through element 0 (MWCC
// emits byte-identical code for arr[0] and the scalar form).
// (plain definition: global data names are never mangled by MWCC, and an
// `extern "C"` qualified declaration would be taken as extern storage class,
// emitting no storage)
__attribute__((aligned(8))) CTaskLOD* lbl_eu_80665730[2];

// Shared float pool owned by CTaskLOD's sibling split (values verified from
// the retail object: 8066A5B8 = 1.0f, 8066A5BC = 0.0f). Referencing the
// retail names keeps the SDA21 relocs right and leaves no TU-local .sdata2
// pool (retail .sdata2 is empty); literals would pool anonymously here.

// LODMemMan delegate entry points used by the wrappers below.  The retail
// symbol map labels them `.Fv` (no-arg) even though the retail bodies take
// real arguments (r4/r5/...).  A C++ declaration cannot express that shape
// (any arg list mangles the name away from the retail symbol), so they are
// declared with C linkage: MWCC emits the identifier verbatim, making both
// the wrapper symbols and the tail-call relocs carry the exact retail names
// (no reloc drift).  Same pattern as the accepted LODMemMan.cpp TU.
// setElementVisibility is a real LOD::UnkClass_8046D144 member (int task, int flag).
// updateElementVisibility is a real LOD::UnkClass_8046D144 member (int task, int flag).
// setElementFlag80 is a real LOD::UnkClass_8046D144 member (int task, int flag).
// linkTaskElements is a real LOD::UnkClass_8046D144 member (int task, int value).
// setTimeParams is a real LOD::UnkClass_8046D144 member (u16 a, u16 b, u16 c).
// clearElementFlag is a real LOD::UnkClass_8046D144 member (int task).
// setElementFlag is a real LOD::UnkClass_8046D144 member (int task).
// toggleElementFlag is a real LOD::UnkClass_8046D144 member (int task, int flag).
// setPauseFlag is a real LOD::UnkClass_8046D144 member (int param).
// attachSceneObjects is a real LOD::UnkClass_8046D144 member (u32 param).
// isElementFlagClear is a real LOD::UnkClass_8046D144 member (int task) -> bool.
extern "C" void configureFadeMode__Q23LOD9LODMemManFv(LOD::UnkClass_8046D144* self, int a, int b);
extern "C" void updateSubObject__Q23LOD17UnkClass_8046D144Fv(LOD::UnkClass_8046D144* self, CTaskLOD* task);
extern "C" void setElementLodLevel__Q23LOD9LODMemManFv(LOD::UnkClass_8046D144* self, CTaskLOD* task);
extern "C" void setElementScale__Q23LOD9LODMemManFv(LOD::UnkClass_8046D144* self, CTaskLOD* task);
// refreshElementScale is a real LOD::UnkClass_8046D144 member (int task).
// getElementScale is a real LOD::UnkClass_8046D144 member (int task) -> f32.
extern "C" float getBaseScale__Q23LOD17UnkClass_8046D144Fv(LOD::UnkClass_8046D144* self);
// getElementState is a real LOD::UnkClass_8046D144 member (int task) -> u8.
extern "C" void setElementCachedValue__Q23LOD9LODMemManFv(LOD::UnkClass_8046D144* self, CTaskLOD* task);
// getElementCachedValue is a real LOD::UnkClass_8046D144 member (int task) -> f32.
// updateSingleElement is a real LOD::UnkClass_8046D144 member (int id) -> u8.
// freePersistentBuffer is a static LOD::LODMemMan member (LODMemMan.hpp).


// (memberized: scnLgtCallLightV26 -> CScnEnvLgtCtrl::CallLightV26; declaration moved into class CScnEnvLgtCtrl)
// teardownG3dObjects is a real LOD::UnkClass_8046D144 member (int param).
extern "C" void configureShrinkTargets__Q23LOD9LODMemManFv(LOD::UnkClass_8046D144* self, CTaskLOD* task, int a);
extern "C" void* resetAndReload__Q23LOD9LODMemManFv(LOD::UnkClass_8046D144* self, CTaskLOD* task, LODParam1Obj* p1);
extern "C" void allocPersistentBuffer__Q23LOD9LODMemManFv(u32 handle, u32 size);
extern "C" void getLodVisState__Q23LOD17UnkClass_8046A530Fv(LOD::UnkClass_8046D144* self, u32 a, u32 b);
// (memberized: scnLgtCallLightV27 -> CScnEnvLgtCtrl::CallLightV27; declaration moved into class CScnEnvLgtCtrl)

// --- FULL_MATCH functions ---

// Delegate wrappers.  Each checks the singleton instance pointer
// `lbl_eu_80665730` and, if non-null, forwards to its active LODMemMan
// (mpActiveLOD at 0x1D40).  The forwarding call is tail-call optimised by
// MWCC into a `b` branch.
//
// notifyLODTick / updateLODFrame forward their float argument untouched.
// func_8046E594 and func_8046E5BC take a real argument in retail (passed via
// r4: `1`/`0`/`this`), but the shared symbol map labels them `.Fv` (no-arg),
// so those bodies are byte-identical with a branch-reloc NAME drift
// (`.Fi`/`.FP8CTaskLOD` vs retail `.Fv`).

void CTaskLOD::notifyLODTick(float scale) {
    if (lbl_eu_80665730[0]) {
        lbl_eu_80665730[0]->mpActiveLOD->setLodScaleAndRefresh(scale);
    }
}

void CTaskLOD::updateLODFrame(float scale) {
    if (lbl_eu_80665730[0]) {
        lbl_eu_80665730[0]->mpActiveLOD->setBaseScale(scale);
    }
}

void CTaskLOD::enableLOD() {
    if (lbl_eu_80665730[0]) {
        // Retail symbol is .Fv though r4 carries the arg; route through the
        // extern "C" retail-named wrapper so the reloc name matches.
        lbl_eu_80665730[0]->mpActiveLOD->setPauseFlag(1);
    }
}

void CTaskLOD::disableLOD() {
    if (lbl_eu_80665730[0]) {
        lbl_eu_80665730[0]->mpActiveLOD->setPauseFlag(0);
    }
}

void CTaskLOD::bindTaskToLOD() {
    if (lbl_eu_80665730[0]) {
        lbl_eu_80665730[0]->mpActiveLOD->attachSceneObjects((u32)this);
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

inline CTaskLOD::CTaskLOD(LODParam1Obj* p1, void* p2, u32 handle, u32 size)
    : mParam1(p1), mParam2(p2), mpSecondaryLOD(0), mpActiveLOD(&mLODMemMan) {
    if (size) {
        allocPersistentBuffer__Q23LOD9LODMemManFv(handle, size);
    }
    getLodVisState__Q23LOD17UnkClass_8046A530Fv(mpActiveLOD, handle, size);
}

// Factory (the retail symbol is mislabeled `.Fv` but the body reads r4-r7).
extern "C" CTaskLOD* create__8CTaskLODFv(CTaskLOD* parent, LODParam1Obj* p1,
                                          void* p2, u32 handle, u32 size) {
    CTaskLOD* t = new (CWorkThreadSystem::getWorkMem()) CTaskLOD(p1, p2, handle, size);
    t->Regist(parent, false);
    lbl_eu_80665730[0] = t;
    return t;
}

// Allocate/replace the secondary LODMemMan when `enable` is set, then return
// the active LODMemMan's resetAndReload result (0 when there is no singleton).
extern "C" void* acquireLODResource__8CTaskLODFv(CTaskLOD* self, bool enable) {
    if (lbl_eu_80665730[0]) {
        if (enable) {
            CTaskLOD* t = lbl_eu_80665730[0];
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
        return resetAndReload__Q23LOD9LODMemManFv(
            lbl_eu_80665730[0]->mpActiveLOD, self, lbl_eu_80665730[0]->mParam1);
    }
    return 0;
}

// Teardown + re-bind: release the active LODMemMan's g3d objects, destroy the
// secondary LOD instance, then point mpActiveLOD back at the embedded
// mLODMemMan and enable the LOD system.  The whole body works on the global
// instance (retail never touches `this` except via the delegate call).
void CTaskLOD::resetActiveLOD() {
    if (lbl_eu_80665730[0]) {
        lbl_eu_80665730[0]->mpActiveLOD->teardownG3dObjects((int)this);
        CTaskLOD* t = lbl_eu_80665730[0];
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
// whole body works on the global instance (retail never touches `this`).
void CTaskLOD::restorePrimaryLOD() {
    if (lbl_eu_80665730[0]) {
        CTaskLOD* t = lbl_eu_80665730[0];
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
    if (lbl_eu_80665730[0]) {
        getBaseScale__Q23LOD17UnkClass_8046D144Fv(lbl_eu_80665730[0]->mpActiveLOD);
    }
    return 1.0f;
}

// Toggle the 0x40 flag bit on the active LODMemMan at offset 0x6c: set when
// `this` is non-null, clear when null.
void CTaskLOD::setLODFilterFlag() {
    if (lbl_eu_80665730[0]) {
        LOD::UnkClass_8046D144* lod = lbl_eu_80665730[0]->mpActiveLOD;
        if (this) {
            lod->field_0x6c |= 0x40;
        } else {
            lod->field_0x6c &= ~0x40;
        }
    }
}

// Forwards the tail call to the active LODMemMan's func_8046F024 with args
// (1, this - 1) when `this` is non-null, otherwise (0, 0).
void CTaskLOD::updateLODRange() {
    if (lbl_eu_80665730[0]) {
        if (this) {
            configureFadeMode__Q23LOD9LODMemManFv(lbl_eu_80665730[0]->mpActiveLOD, 1, (int)this - 1);
        } else {
            configureFadeMode__Q23LOD9LODMemManFv(lbl_eu_80665730[0]->mpActiveLOD, 0, 0);
        }
    }
}

void CTaskLOD::syncLODTask() {
    if (lbl_eu_80665730[0]) {
        updateSubObject__Q23LOD17UnkClass_8046D144Fv(lbl_eu_80665730[0]->mpActiveLOD, this);
    }
}

// Fetches the active LODMemMan and forwards task + args (a, b) to it
// (retail `mr r8,r3; mr r0,r4; mr r6,r5` sets up this/a/b for the delegate).
// c is unused (dropped).  Defined as a global function carrying the retail
// `.Fv` symbol name (see note above).
extern "C" void dispatchLODArgs__8CTaskLODFv(CTaskLOD* self, void* a, void* b, void* c) {
    if (lbl_eu_80665730[0]) {
        lbl_eu_80665730[0]->mpActiveLOD->setTimeParams((u32)(int)self, (u32)(int)a, (u32)(int)b);
    }
}

// LOD activation wrappers: enable/disable LOD for `this` task on the active
// manager, then dispatch the same task + flag through the singleton's
// scene-light control list (scnLgtCallLightV26 ring walk).
void CTaskLOD::activateLOD() {
    if (lbl_eu_80665730[0]) {
        lbl_eu_80665730[0]->mpActiveLOD->clearElementFlag((int)this);
        (lbl_eu_80665730[0]->mParam1->field_0x7C)->CallLightV26(this, (void*)1);
    }
}

void CTaskLOD::deactivateLOD() {
    if (lbl_eu_80665730[0]) {
        lbl_eu_80665730[0]->mpActiveLOD->setElementFlag((int)this);
        (lbl_eu_80665730[0]->mParam1->field_0x7C)->CallLightV26(this, (void*)0);
    }
}

// Setter-style wrapper: forwards the task + flag pair through the active
// manager and the scene-light ring.  Defined as a global carrying the retail
// `.Fv` symbol name because the body reads a real r4 argument (see the note
// on the dispatchLODArgs family above).
extern "C" void setLODEnable__8CTaskLODFv(CTaskLOD* self, int flag) {
    if (lbl_eu_80665730[0]) {
        lbl_eu_80665730[0]->mpActiveLOD->toggleElementFlag((int)self, flag);
        (lbl_eu_80665730[0]->mParam1->field_0x7C)->CallLightV26(self, (void*)flag);
    }
}
// Value-delegates.  Like the void wrappers above, each forwards to the
// active LODMemMan when the singleton `lbl_eu_80665730` is set, passing
// `this` as the trailing CTaskLOD* argument (retail `mr r4, r3`).  The
// pointer/float returns come straight back from the delegate.
void* CTaskLOD::getLODData() {
    if (lbl_eu_80665730[0]) {
        return (void*)lbl_eu_80665730[0]->mpActiveLOD->isElementFlagClear((int)this);
    }
    return 0;
}

void CTaskLOD::refreshLOD() {
    if (lbl_eu_80665730[0]) {
        setElementLodLevel__Q23LOD9LODMemManFv(lbl_eu_80665730[0]->mpActiveLOD, this);
    }
}

// Forward the task + arg pair to the active LODMemMan (func_8046E988), then
// walk the singleton's scene-light control ring (scnLgtCallLightV27) with the same
// task/arg and the float value.  `b` is unused in retail.
extern "C" void setLODParam__8CTaskLODFv(CTaskLOD* self, float val, int a, int b) {
    if (lbl_eu_80665730[0]) {
        configureShrinkTargets__Q23LOD9LODMemManFv(lbl_eu_80665730[0]->mpActiveLOD, self, a);
        (lbl_eu_80665730[0]->mParam1->field_0x7C)->CallLightV27(self, (void*)a, val);
    }
}

// Same delegate-shape as dispatchLODArgs: forwards `self` + first arg `a` to the
// active LODMemMan (tail-call `b`).  `b` is unused in retail.
extern "C" void addLODEntry__8CTaskLODFv(CTaskLOD* self, int a, int b) {
    if (lbl_eu_80665730[0]) {
        lbl_eu_80665730[0]->mpActiveLOD->setElementVisibility((int)self, a);
    }
}

void CTaskLOD::removeLODEntry() {
    if (lbl_eu_80665730[0]) {
        setElementScale__Q23LOD9LODMemManFv(lbl_eu_80665730[0]->mpActiveLOD, this);
    }
}

void CTaskLOD::clearLODEntry() {
    if (lbl_eu_80665730[0]) {
        lbl_eu_80665730[0]->mpActiveLOD->refreshElementScale((int)this);
    }
}

float CTaskLOD::getLODDistance() {
    if (lbl_eu_80665730[0]) {
        return lbl_eu_80665730[0]->mpActiveLOD->getElementScale((int)this);
    }
    return 0.0f;
}
extern "C" void attachLODObject__8CTaskLODFv(CTaskLOD* self, int a, int b) {
    if (lbl_eu_80665730[0]) {
        lbl_eu_80665730[0]->mpActiveLOD->updateElementVisibility((int)self, a);
    }
}

extern "C" void detachLODObject__8CTaskLODFv(CTaskLOD* self, int a, int b) {
    if (lbl_eu_80665730[0]) {
        lbl_eu_80665730[0]->mpActiveLOD->setElementFlag80((int)self, a);
    }
}

extern "C" void setLODObject__8CTaskLODFv(CTaskLOD* self, int a, int b) {
    if (lbl_eu_80665730[0]) {
        lbl_eu_80665730[0]->mpActiveLOD->linkTaskElements((int)self, a);
    }
}
void* CTaskLOD::getLODObject() {
    if (lbl_eu_80665730[0]) {
        return (void*)lbl_eu_80665730[0]->mpActiveLOD->getElementState((int)this);
    }
    return 0;
}

void CTaskLOD::updateLODObject() {
    if (lbl_eu_80665730[0]) {
        setElementCachedValue__Q23LOD9LODMemManFv(lbl_eu_80665730[0]->mpActiveLOD, this);
    }
}

float CTaskLOD::getLODLevel() {
    if (lbl_eu_80665730[0]) {
        return lbl_eu_80665730[0]->mpActiveLOD->getElementCachedValue((int)this);
    }
    return 0.0f;
}

// func_8046EF7C returns a pointer; when there is no active LOD the retail
// body returns `this` unchanged (beqlr with r3 still holding `this`).
CTaskLOD* CTaskLOD::getLODOrSelf() {
    if (lbl_eu_80665730[0]) {
        return (CTaskLOD*)(u32)lbl_eu_80665730[0]->mpActiveLOD->updateSingleElement((int)this);
    }
    return this;
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
