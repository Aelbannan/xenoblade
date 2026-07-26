// Auto-scaffolded catalog TU for monolib/src/lod/CTaskLOD
// Replace stubs with high-level C/C++ during decomp.

#include <types.h>
#include "monolib/work/CProcess.hpp"
#include "monolib/lod/LODMemMan.hpp"

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// Local CTTask (out-of-line Move/Draw/dtor) for harness stubs.
// Do not include monolib/work/CTTask.hpp here — its inline methods collide
// with these explicit specialisations.
template <typename T>
class CTTask : public CProcess {
public:
    CTTask();
    virtual ~CTTask();
    virtual void Move();
    virtual void Draw();
};

// Forward declaration for CTaskLOD class (needed by CTTask<CTaskLOD>).
class CTaskLOD;

// Explicit specialisations for CTTask<CTaskLOD> out-of-line members.
// These satisfy the linker references from the vtable (defined elsewhere).
template<> CTTask<CTaskLOD>::~CTTask() {}
template<> void CTTask<CTaskLOD>::Move() {}
template<> void CTTask<CTaskLOD>::Draw() {}

// --- FULL_MATCH functions ---

// CTaskLOD wraps a LODMemMan and manages its lifecycle.
// Inherits from CTTask<CTaskLOD> which provides Move/Draw callback slots.
// Layout (0x1D44 total):
//   0x00-0x53 : CTTask<CTaskLOD> base (CProcess + PTMFs)
//   0x54      : mParam1
//   0x58      : mParam2
//   0x5C      : mLODMemMan (embedded / primary)
//   0x1D3C    : mpSecondaryLOD (optional separately-allocated LODMemMan)
//   0x1D40    : mpActiveLOD (points to either &mLODMemMan or mpSecondaryLOD)
class CTaskLOD : public CTTask<CTaskLOD> {
public:
    virtual ~CTaskLOD();
    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void Draw();
};

// CTaskLOD::Init() — no-op; initialisation is performed in create().
void CTaskLOD::Init() {}

// --- Remaining harness stubs (empty bodies) ---

extern "C" void __dt__8CTaskLODFv() {}

extern "C" void Term__8CTaskLODFv() {}

extern void func_8046DBC8__Q23LOD9LODMemManFv();
extern "C" void Move__8CTaskLODFv(void* self) { func_8046DBC8__Q23LOD9LODMemManFv(); }

extern "C" void create__8CTaskLODFv() {}

extern "C" void func_80462A08__8CTaskLODFv() {}

extern "C" void func_80462AC0__8CTaskLODFv() {}

extern "C" void func_80462B30__8CTaskLODFv() {}

extern "C" void func_80462B4C__8CTaskLODFv() {}

extern "C" void func_80462B68__8CTaskLODFv() {}

extern "C" void func_80462BC8__8CTaskLODFv() {}

extern "C" void func_80462BE4__8CTaskLODFv() {}

extern "C" void func_80462BFC__8CTaskLODFv() {}

extern "C" void func_80462C14__8CTaskLODFv() {}

extern "C" void func_80462C48__8CTaskLODFv() {}

extern "C" void func_80462C80__8CTaskLODFv() {}

extern "C" void func_80462CBC__8CTaskLODFv() {}

extern "C" void func_80462CD8__8CTaskLODFv() {}

extern "C" void func_80462D04__8CTaskLODFv() {}

extern "C" void func_80462D5C__8CTaskLODFv() {}

extern "C" void func_80462DB4__8CTaskLODFv() {}

extern "C" void func_80462E1C__8CTaskLODFv() {}

extern "C" void func_80462E3C__8CTaskLODFv() {}

extern "C" void func_80462E58__8CTaskLODFv() {}

extern "C" void func_80462ED0__8CTaskLODFv() {}

extern "C" void func_80462EF4__8CTaskLODFv() {}

extern "C" void func_80462F10__8CTaskLODFv() {}

extern "C" void func_80462F2C__8CTaskLODFv() {}

extern "C" void func_80462F4C__8CTaskLODFv() {}

extern "C" void func_80462F70__8CTaskLODFv() {}

extern "C" void func_80462F94__8CTaskLODFv() {}

extern "C" void func_80462FB8__8CTaskLODFv() {}

extern "C" void func_80462FD8__8CTaskLODFv() {}

extern "C" void func_80462FF4__8CTaskLODFv() {}

extern "C" void func_80463014__8CTaskLODFv() {}
