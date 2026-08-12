// Auto-scaffolded catalog TU for kyoshin/CCol6System
// Cleaned-up C++ for CCol6CheckBat; other stubs pending decomp.

#include "kyoshin/CCol6CheckBat.hpp"
#include "kyoshin/CCol6System.hpp"
#include "kyoshin/harness_catalog.hpp"

extern "C" void __dt__8CProcessFv(void* self, int flags);
extern "C" void __dt__10CScrollBarFv(void* self, int flags);

// Singleton instance (retail: lbl_eu_80664230).
CCol6CheckBat* gCol6CheckBat;

int lbl_eu_80664230;
int lbl_eu_80664234;
int lbl_eu_80664238;
int lbl_eu_8066235C;

void func_8015D0B8() {
    lbl_eu_80664230 = 0;
    lbl_eu_80664234 = 0;
    lbl_eu_80664238 = 0;
    lbl_eu_8066235C = -1;
}

// CCol6CheckBat::~CCol6CheckBat()
// extern "C" free-function form (CSysWin/CMenuQstCnt precedent): the retail
// dtor's outer null-check covers the delete, and the nested double null-check
// is the documented D2-inlined-into-D1 MWCC artifact (same as
// ~CSimpleEveTalkWin / ~CSystemWindow) guarding the CProcess base dtor.
// NOTE: default -O4,p (NOT optimize_for_size) — this retail keeps the
// separate stw r31/stw r30 saves instead of the stmw pair.
extern "C" CCol6CheckBat* __dt__13CCol6CheckBatFv(CCol6CheckBat* self, int flags) {
    if (self != 0) {
        if (self != 0) {
            if (self != 0) {
                __dt__8CProcessFv(reinterpret_cast<CProcess*>(self), 0);
            }
        }
        if (flags > 0) {
            operator delete(self);
        }
    }
    return self;
}

// CCol6CheckBat::Init() - no-op; override satisfies CProcess pure-virtual.
void CCol6CheckBat::Init() {}

// CCol6CheckBat::Term() - clears singleton pointer.
void CCol6CheckBat::Term() {
    *(CCol6CheckBat**)&lbl_eu_80664230 = nullptr;
}

// CCol6CheckBat::Move() - update tick (stub pending decomp).
void CCol6CheckBat::Move() {}

void __ct__CCol6CheckBat(){}

extern u32 func_8009CF8C(u32 resourceId);

int func_8015D310() {
    union {
        struct {
            u32 w;
            u8 b;
        };
        u8 bytes[5];
    } data;
    data.w = lbl_eu_80667540;
    data.b = lbl_eu_80667544;

    u8 result = func_8009CF8C(0x7fc);

    for (u32 i = 0; i < 5; i++) {
        u8 idx = (u8)i;
        if (result >= data.bytes[idx] && func_8009CF8C(idx + 0x804) == 0) {
            return 1;
        }
    }
    return 0;
}

void func_8015D3A0(){}

void __ct__CCol6Hint(){}

// CCol6Hint dtor (extern "C" free-function form, default -O4,p — retail keeps
// separate stw r31/stw r30 saves): sub-object dtors in +0x13C (CScrollBar),
// +0x124 (CCur18), +0x74 (UnkClass_8045F564) order with flags -1, then the
// CProcess base dtor behind the double-null guard (retail re-checks r30 and
// emits two beq's — the D2-inlined-into-D1 artifact), then flags-based delete.
extern "C" void* __dt__9CCol6HintFv(CCol6Hint* self, int flags) {
    if (self != 0) {
        __dt__10CScrollBarFv(&self->mScrollBar, -1);
        __dt__6CCur18Fv(self->mCur18, -1);
        __dt__17UnkClass_8045F564Fv(self->mMemRegion, -1);
        if (self != 0) {
            if (self != 0) {
                __dt__8CProcessFv(reinterpret_cast<CProcess*>(self), 0);
            }
        }
        if (flags > 0) {
            operator delete(self);
        }
    }
    return self;
}

void CCol6Hint::Init() {}

void CCol6Hint::Term() {}

void CCol6Hint::Move() {}

void func_8015DB08(){}

void CCol6Hint::cbRenderBefore() {}

void func_8015DCD0(){}

void func_8015DD4C(){}

void func_8015E0BC(){}

void __ct__CCol6System(){}

// extern "C" free-function form (CCol6Hint precedent): sub-dtors in retail
// order (mSysWin2/mSysWin1/mCur2/mCur1/mMemRegion, flags -1), double-null-
// guarded CProcess base dtor, flags-based delete; default -O4,p.
extern "C" void* __dt__11CCol6SystemFv(CCol6System* self, int flags) {
    if (self != 0) {
        __dt__7CSysWinFv(&self->mSysWin2, -1);
        __dt__7CSysWinFv(&self->mSysWin1, -1);
        __dt__6CCur18Fv(self->mCur2, -1);
        __dt__6CCur18Fv(self->mCur1, -1);
        __dt__17UnkClass_8045F564Fv(self->mMemRegion, -1);
        if (self != 0) {
            if (self != 0) {
                __dt__8CProcessFv(reinterpret_cast<CProcess*>(self), 0);
            }
        }
        if (flags > 0) {
            operator delete(self);
        }
    }
    return self;
}

void CCol6System::Init() {}

void CCol6System::Term() {}

void CCol6System::Move() {}

void func_80160118(){}

void CCol6System::cbRenderBefore() {}

void func_801602F4(){}

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

// CCol6Invite::~CCol6Invite()
// extern "C" free-function form (same retail shape as ~CCol6CheckBat):
// outer null-check covers the delete; nested double null-check (D2-inlined-
// into-D1 artifact) guards the CProcess base dtor; default -O4,p.
extern "C" void* __dt__11CCol6InviteFv(CCol6Invite* self, int flags) {
    if (self != 0) {
        if (self != 0) {
            if (self != 0) {
                __dt__8CProcessFv(reinterpret_cast<CProcess*>(self), 0);
            }
        }
        if (flags > 0) {
            operator delete(self);
        }
    }
    return self;
}

void CCol6Invite::Init() {}

void CCol6Invite::Move() {}
