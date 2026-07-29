// Auto-scaffolded catalog TU for kyoshin/CCol6System
// Cleaned-up C++ for CCol6CheckBat; other stubs pending decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CCol6System.hpp"
#include "kyoshin/CCol6CheckBat.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/util/MemManager.hpp"

// Singleton instance (retail: lbl_eu_80664230).
CCol6CheckBat* gCol6CheckBat;

int lbl_eu_80664230;
CCol6Hint* lbl_eu_80664234;
CCol6System* lbl_eu_80664238;
int lbl_eu_8066235C;

void func_8015D0B8() {
    lbl_eu_80664230 = 0;
    lbl_eu_80664234 = nullptr;
    lbl_eu_80664238 = nullptr;
    lbl_eu_8066235C = -1;
}

// CCol6CheckBat::~CCol6CheckBat()
CCol6CheckBat::~CCol6CheckBat() {}

// CCol6CheckBat::Init() - no-op; override satisfies CProcess pure-virtual.
void CCol6CheckBat::Init() {}

// CCol6CheckBat::Term() - clears singleton pointer.
void CCol6CheckBat::Term() {
    gCol6CheckBat = nullptr;
}

// CCol6CheckBat::Move() - update tick (stub pending decomp).
void CCol6CheckBat::Move() {}

void __ct__CCol6CheckBat(){}

void func_8015D310(){}

void func_8015D3A0(){}

/* CCol6Hint constructor: placement-new called from func_8015DCD0 factory. */
CCol6Hint::CCol6Hint(void* arg) {
    // Stub — fields initialized by this ctor; body TBD when ctor is decompiled.
}

/* CCol6Hint destructor: destroys owned sub-objects in reverse declaration
   order (CScrollBar, CCur18, mem region), then chains to CProcess base.
   The delete flag in r4 (MWCC convention) is handled by the compiler. */
CCol6Hint::~CCol6Hint() {}

void CCol6Hint::Init() {}

void CCol6Hint::Term() {}

void CCol6Hint::Move() {}

void func_8015DB08(){}

void CCol6Hint::cbRenderBefore() {}

/* Factory: creates the CCol6Hint singleton and registers it as a child
   of parent. Returns null if singleton already exists. */
CCol6Hint* func_8015DCD0(CProcess* parent, void* arg) {
    if (lbl_eu_80664234 != nullptr) {
        return nullptr;
    }
    CCol6Hint* hint = (CCol6Hint*)mtl::MemManager::allocate(0x17c, CWorkThreadSystem::getWorkMem());
    if (hint != nullptr) {
        hint = new (hint) CCol6Hint(arg);
    }
    lbl_eu_80664234 = hint;
    hint->Regist(parent, false);
    return lbl_eu_80664234;
}

void func_8015DD4C(){}

void func_8015E0BC(){}

void __ct__CCol6System(){}

void CCol6System::~CCol6System() {}

void CCol6System::Init() {}

void CCol6System::Term() {}

void CCol6System::Move() {}

void func_80160118(){}

void CCol6System::cbRenderBefore() {}

/* Factory: creates the CCol6System singleton and registers it as a child
   of parent. Returns null if singleton already exists. */
CCol6System* func_801602F4(CProcess* parent, void* arg) {
    if (lbl_eu_80664238 != nullptr) {
        return nullptr;
    }
    CCol6System* sys = (CCol6System*)mtl::MemManager::allocate(0x240, CWorkThreadSystem::getWorkMem());
    if (sys != nullptr) {
        sys = new (sys) CCol6System(arg);
    }
    lbl_eu_80664238 = sys;
    sys->Regist(parent, false);
    return lbl_eu_80664238;
}

void func_80160370(){}

void func_80160A6C(){}

void func_80160EE4(){}

void func_80161024(){}

void func_80161178(){}

void func_8016169C(){}

void func_80161C5C(){}

void func_80162000(){}

void func_80162C40(){}

void func_80162DB4(){}

void func_80162EF8(){}

void func_80163614(){}

void func_8016378C(){}

void func_801638C0(){}

void func_80163AF4(){}

void CCol6Invite::~CCol6Invite() {}

void CCol6Invite::Init() {}

extern u32 lbl_eu_8066423C;


void CCol6Invite::Move() {}
