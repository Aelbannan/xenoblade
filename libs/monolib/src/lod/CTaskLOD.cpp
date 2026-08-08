// Auto-scaffolded catalog TU for monolib/src/lod/CTaskLOD
// Replace stubs with high-level C/C++ during decomp.

#include <types.h>
#include "monolib/work/CProcess.hpp"
#include "monolib/lod/LODMemMan.hpp"

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// Local CTTask (out-of-line Move/Draw/dtor) for harness stubs.
// Do not include monolib/work/CTTask.hpp here - its inline methods collide
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
    void create();
    void func_80462A08();
    void func_80462AC0();
    void func_80462B30();
    void func_80462B4C();
    void func_80462B68();
    void func_80462BC8();
    void func_80462BE4();
    void func_80462BFC();
    void func_80462C14();
    void func_80462C48();
    void func_80462C80();
    void func_80462CBC();
    void func_80462CD8();
    void func_80462D04();
    void func_80462D5C();
    void func_80462DB4();
    void func_80462E1C();
    void func_80462E3C();
    void func_80462E58();
    void func_80462ED0();
    void func_80462EF4();
    void func_80462F10();
    void func_80462F2C();
    void func_80462F4C();
    void func_80462F70();
    void func_80462F94();
    void func_80462FB8();
    void func_80462FD8();
    void func_80462FF4();
    void func_80463014();
};

// CTaskLOD::Init() - no-op; initialisation is performed in create().
void CTaskLOD::Init() {}

// --- Remaining harness stubs (empty bodies) ---

CTaskLOD::~CTaskLOD() {}

void CTaskLOD::Term() {}

extern void func_8046DBC8__Q23LOD9LODMemManFv();
void CTaskLOD::Move() { func_8046DBC8__Q23LOD9LODMemManFv(); }

void CTaskLOD::create() {}

void CTaskLOD::func_80462A08() {}

void CTaskLOD::func_80462AC0() {}

void CTaskLOD::func_80462B30() {}

void CTaskLOD::func_80462B4C() {}

void CTaskLOD::func_80462B68() {}

void CTaskLOD::func_80462BC8() {}

void CTaskLOD::func_80462BE4() {}

void CTaskLOD::func_80462BFC() {}

void CTaskLOD::func_80462C14() {}

void CTaskLOD::func_80462C48() {}

void CTaskLOD::func_80462C80() {}

void CTaskLOD::func_80462CBC() {}

void CTaskLOD::func_80462CD8() {}

void CTaskLOD::func_80462D04() {}

void CTaskLOD::func_80462D5C() {}

void CTaskLOD::func_80462DB4() {}

void CTaskLOD::func_80462E1C() {}

void CTaskLOD::func_80462E3C() {}

void CTaskLOD::func_80462E58() {}

void CTaskLOD::func_80462ED0() {}

void CTaskLOD::func_80462EF4() {}

void CTaskLOD::func_80462F10() {}

void CTaskLOD::func_80462F2C() {}

void CTaskLOD::func_80462F4C() {}

void CTaskLOD::func_80462F70() {}

void CTaskLOD::func_80462F94() {}

void CTaskLOD::func_80462FB8() {}

void CTaskLOD::func_80462FD8() {}

void CTaskLOD::func_80462FF4() {}

void CTaskLOD::func_80463014() {}
