// Auto-scaffolded catalog TU for kyoshin/CCol6System
// Cleaned-up C++ for CCol6CheckBat; other stubs pending decomp.

#include "kyoshin/CCol6CheckBat.hpp"
#include "kyoshin/harness_catalog.hpp"

// Singleton instance (retail: lbl_eu_80664230).
CCol6CheckBat* gCol6CheckBat;

extern "C" int lbl_eu_80664230;
extern "C" int lbl_eu_80664234;
extern "C" int lbl_eu_80664238;
extern "C" int lbl_eu_8066235C;

extern "C" void func_8015D0B8() {
    lbl_eu_80664230 = 0;
    lbl_eu_80664234 = 0;
    lbl_eu_80664238 = 0;
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

extern "C" void __ct__CCol6CheckBat() {}

extern "C" void func_8015D310() {}

extern "C" void func_8015D3A0() {}

extern "C" void __ct__CCol6Hint() {}

extern "C" void __dt__9CCol6HintFv() {}

extern "C" void Init__9CCol6HintFv() {}

extern "C" void Term__9CCol6HintFv() {}

extern "C" void Move__9CCol6HintFv() {}

extern "C" void func_8015DB08() {}

extern "C" void cbRenderBefore__9CCol6HintFv() {}

extern "C" void func_8015DCD0() {}

extern "C" void func_8015DD4C() {}

extern "C" void func_8015E0BC() {}

extern "C" void __ct__CCol6System() {}

extern "C" void __dt__11CCol6SystemFv() {}

extern "C" void Init__11CCol6SystemFv() {}

extern "C" void Term__11CCol6SystemFv() {}

extern "C" void Move__11CCol6SystemFv() {}

extern "C" void func_80160118() {}

extern "C" void cbRenderBefore__11CCol6SystemFv() {}

extern "C" void func_801602F4() {}

extern "C" void func_80160370() {}

extern "C" void func_80160A6C() {}

extern "C" void func_80160EE4() {}

extern "C" void func_80161024() {}

extern "C" void func_80161178() {}

extern "C" void func_8016169C() {}

extern "C" void func_80161C5C() {}

extern "C" void func_80162000() {}

extern "C" void func_80162C40() {}

extern "C" void func_80162DB4() {}

extern "C" void func_80162EF8() {}

extern "C" void func_80163614() {}

extern "C" void func_8016378C() {}

extern "C" void func_801638C0() {}

extern "C" void func_80163AF4() {}

extern "C" void __dt__11CCol6InviteFv() {}

extern "C" void Init__11CCol6InviteFv() {}

extern u32 lbl_eu_8066423C;
extern "C" void Term__11CCol6InviteFv(void* self) {
    *((u8*)self + 0x67) = 0;
    lbl_eu_8066423C = 0;
}

extern "C" void Move__11CCol6InviteFv() {}
