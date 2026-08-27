#pragma once

#include <types.h>
#include "monolib/work/CTTask.hpp"
#include "monolib/lod/LODMemMan.hpp"

struct CScnEnvLgtCtrl;  // scene-light control ring (CScnEnvLgtCtrl.hpp)

// Pointee of CTaskLOD::mParam1 (offset 0x54): an opaque manager object whose
// +0x7C slot holds the scene-light control list head handed to func_804C2014
// by the activateLOD/deactivateLOD/setLODEnable wrappers.
struct LODParam1Obj {
    u8 _00[0x7C];
    CScnEnvLgtCtrl* field_0x7C;  // +0x7C light-control list head
};

/*
Task wrapper around a LODMemMan instance.

CTaskLOD manages the LOD (level-of-detail) memory manager lifecycle.  A single
global instance is reachable through `lbl_eu_80665730`; most public methods
delegate to the manager pointed at by mpActiveLOD (offset 0x1D40).

Inherits from CTTask<CTaskLOD> (0x54 bytes: CDoubleListNode + vtable +
CProcess + Move/Draw member pointers) which provides the Move/Draw callback
slots.  Those base-class Move/Draw slots are overridden here; the explicit
specialisations for CTTask<CTaskLOD> Move/Draw (used when other TUs include
CTasks' inline-only vtable helpers) are separate.
*/
class CTaskLOD : public CTTask<CTaskLOD> {
public:
    virtual ~CTaskLOD();
    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void Draw();
    void create();
    void acquireLODResource();
    void resetActiveLOD();
    void enableLOD();
    void disableLOD();
    void restorePrimaryLOD();
    void bindTaskToLOD();
    void notifyLODTick();
    void updateLODFrame();
    float getLODScale();
    void setLODFilterFlag();
    void updateLODRange();
    void syncLODTask();
    void activateLOD();
    void deactivateLOD();
    // setLODEnable takes a real r4 argument but is retail-named as no-arg
    // (`.Fv`); it is defined in CTaskLOD.cpp as the global function
    // `extern "C" void setLODEnable__8CTaskLODFv(CTaskLOD*, int)`.
    void* getLODData();
    void refreshLOD();
    void setLODParam();
    void removeLODEntry();
    void clearLODEntry();
    float getLODDistance();
    void* getLODObject();
    void updateLODObject();
    float getLODLevel();
    void* getLODOrSelf();

private:
    // Layout (0x1D44 total):
    //   0x00-0x53 : CTTask<CTaskLOD> base (CDoubleListNode + vtable + CProcess + PTMFs)
    //   0x54      : mParam1 (opaque pointee, see LODParam1Obj above)
    //   0x58      : mParam2
    //   0x5C      : mLODMemMan (embedded / primary)
    //   0x1D3C    : mpSecondaryLOD (optional separately-allocated LODMemMan)
    //   0x1D40    : mpActiveLOD (preferred: points to &mLODMemMan or mpSecondaryLOD)
    LODParam1Obj* mParam1;              // 0x54
    void* mParam2;                      // 0x58
    LOD::LODMemMan mLODMemMan;          // 0x5C (shared-header size 0x1500)
    u8 _5C_pad[0x7E0];                  // 0x155C .. 0x1D3C

public:
    LOD::LODMemMan* mpSecondaryLOD;     // 0x1D3C
    LOD::LODMemMan* mpActiveLOD;        // 0x1D40
};

// Delegate wrappers whose retail symbol-map names are the mislabeled `.Fv`
// family: the bodies take real arguments (passed in r4/r5/...), but the
// retail names mangle as no-arg.  They are therefore defined/declared as
// global functions carrying the full retail mangled name (MWCC emits
// `extern "C"` identifiers verbatim), so the wrapper symbols and their
// tail-call relocs carry the exact retail names.
extern "C" void dispatchLODArgs__8CTaskLODFv(CTaskLOD* self, void* a, void* b, void* c);
extern "C" void setLODEnable__8CTaskLODFv(CTaskLOD* self, int flag);
extern "C" void addLODEntry__8CTaskLODFv(CTaskLOD* self, void* a, void* b);
extern "C" void attachLODObject__8CTaskLODFv(CTaskLOD* self, void* a, void* b);
extern "C" void detachLODObject__8CTaskLODFv(CTaskLOD* self, void* a, void* b);
extern "C" void setLODObject__8CTaskLODFv(CTaskLOD* self, void* a, void* b);

// The one CTaskLOD instance reference used by the delegates below.
extern CTaskLOD* lbl_eu_80665730;