// Auto-scaffolded catalog TU for kyoshin/CCol6System
// Cleaned-up C++ for CCol6CheckBat; other stubs pending decomp.

#include "kyoshin/CCol6CheckBat.hpp"
#include "kyoshin/CCol6System.hpp"
#include "kyoshin/harness_catalog.hpp"

extern "C" void __dt__8CProcessFv(void* self, int flags);
extern "C" void __dt__10CScrollBarFv(void* self, int flags);

// Forward decls for the layout-build helpers defined below (extern "C" so the
// call sites in func_8015DB08 / func_80160118 bind to the unmangled retail
// symbols).
extern "C" void func_8015DD4C(CCol6System* self);
extern "C" void func_80160370(CCol6System* self);

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

// func_8015DB08 - first layout file-load completion handler: create the
// scratch region + host, detach the arc buffer into a fresh accessor, build
// the layout (func_8015DD4C), flag it ready and register the render callback.
extern "C" int func_8015DB08(CCol6System* self, CEventFile* event) {
    if (self->mFileHandle != event->mFileHandle) return 0;

    reinterpret_cast<UnkClass_8045F564*>(&self->mMemRegion)->createRegion(
        mtl::MemManager::getHandleMEM2(), 0x20000, &lbl_eu_80502050[0x38], 0);
    Class_8045F858 host(reinterpret_cast<UnkClass_8045F564*>(&self->mMemRegion));

    CFileHandle* handle = self->mFileHandle;
    u8* data = handle->mData;
    handle->mData = 0;
    mtl::MemManager::func_80434A4C(false);

    self->mArcAccessor = CLibLayout::createArcResourceAccessor();
    self->mArcAccessor->Attach(data, &lbl_eu_80502050[0x42]);
    func_8015DD4C(self);
    self->mFlag98 = 1;

    IScnRender* render = reinterpret_cast<IScnRender*>(self);
    if (self != 0) {
        render = reinterpret_cast<IScnRender*>(&self->mScnRender);
    }
    self->mScn->addRenderCB(render, 0xd, 0);

    self->mFileHandle = 0;
    reinterpret_cast<UnkClass_8045F564*>(&self->mMemRegion)->func_8045F810();
    return 1;
}

void CCol6Hint::cbRenderBefore() {}

void func_8015DCD0(){}

// func_8015DD4C - builds the first CCol6System layout (stub pending decomp).
extern "C" void func_8015DD4C(CCol6System* self) {}

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

// CCol6System::Term() - wait for the VI draw to finish, detach the render
// callback, release both embedded windows/cursors and the layout/accessor/
// scratch region, then clear the CCol6System singleton global.
void CCol6System::Term() {
    CDeviceVI::waitForDrawDone();
    func_801390E0(&mFileHandle);

    // The `if (this)` is the MWCC idiom that splits mr r4 / beq / addi +0x70
    // for the IScnRender subobject passed to removeRenderCB.
    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this != 0) {
        render = reinterpret_cast<IScnRender*>(&mScnRender);
    }
    mScn->removeRenderCB(render);

    mFlagA0 = 0;
    reinterpret_cast<CCol6CursorView*>(&mCur1)->vf3();
    reinterpret_cast<CCol6CursorView*>(&mCur2)->vf3();
    func_8022B7F4(&mSysWin1);
    func_8022B7F4(&mSysWin2);

    // The outer guard plus delete's own null-check reproduce retail's two beq's
    // on the same CR0 test (MWCC D2-inlined-into-D1 artifact).
    if (mpLayout != 0) {
        delete mpLayout;
        mpLayout = 0;
    }

    func_80139124(mArcAccessor);
    mArcAccessor = 0;
    reinterpret_cast<UnkClass_8045F564*>(&mMemRegion)->func_8045F778();
    lbl_eu_80664238 = 0;
}

void CCol6System::Move() {}

// func_80160118 - second layout file-load completion handler: create the
// scratch region + host, detach the arc buffer into a fresh accessor, build
// the layout (func_80160370), flag it ready and register the render callback.
extern "C" int func_80160118(CCol6System* self, CEventFile* event) {
    if (self->mFileHandle != event->mFileHandle) return 0;

    reinterpret_cast<UnkClass_8045F564*>(&self->mMemRegion)->createRegion(
        mtl::MemManager::getHandleMEM2(), 0x20000, &lbl_eu_80502050[0xd0], 0);
    Class_8045F858 host(reinterpret_cast<UnkClass_8045F564*>(&self->mMemRegion));

    CFileHandle* handle = self->mFileHandle;
    u8* data = handle->mData;
    handle->mData = 0;
    mtl::MemManager::func_80434A4C(false);

    self->mArcAccessor = CLibLayout::createArcResourceAccessor();
    self->mArcAccessor->Attach(data, &lbl_eu_80502050[0x42]);
    func_80160370(self);
    self->mFlagA0 = 1;

    IScnRender* render = reinterpret_cast<IScnRender*>(self);
    if (self != 0) {
        render = reinterpret_cast<IScnRender*>(&self->mScnRender);
    }
    self->mScn->addRenderCB(render, 0xd, 0);

    self->mFileHandle = 0;
    reinterpret_cast<UnkClass_8045F564*>(&self->mMemRegion)->func_8045F810();
    return 1;
}

// cbRenderBefore - draw both embedded windows through a stack DrawInfo. The
// last gate is written as `if (ready != 0) { ... }` so MWCC emits the retail
// bne/b pair (skip-body then skip-to-end) instead of an early-return beq.
void CCol6System::cbRenderBefore() {
    if (func_8013BE50() == 0) return;
    if (mFlagA0 == 0) return;
    if (CSysWin_isReady(&mSysWin1) == 0) return;
    if (CSysWin_isReady(&mSysWin2) != 0) {
        GXSetZMode((GXBool)0, GX_NEVER, (GXBool)0);
        nw4r::lyt::DrawInfo drawInfo;
        func_80137250(&drawInfo);
        if (mFlagA1 == 0) {
            func_80137038(mpLayout, &drawInfo, 0, 1);
            func_801D20B0(&mCur1, &drawInfo);
            func_8022B7C8(&mSysWin1, &drawInfo);
            func_801D20B0(&mCur2, &drawInfo);
        }
        func_8022B7C8(&mSysWin2, &drawInfo);
    }
}

void func_801602F4(){}

// func_80160370 - builds the second CCol6System layout (stub pending decomp).
extern "C" void func_80160370(CCol6System* self) {}

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

// CCol6Invite::Init() - set up the invite banner: resolve the three banner
// strings, bump the two invite counters in the global flag memory by the
// instance's byte args, then format the banner message and post it.
void CCol6Invite::Init() {
    char* str0 = func_8013639C((const void*)lbl_eu_80664098,
                               &lbl_eu_80502050[0x9], mArg2);

    func_8009D018(0x7fc, (u8)((u8)func_8009CF8C(0x7fc) + mArg3));
    func_8009D018(0x7fd, (u8)((u8)func_8009CF8C(0x7fd) + mArg4));

    char* str1 = func_80136190(lbl_eu_80502050, &lbl_eu_80502050[0x9], 0x7e);
    char* str2 = func_80136190(lbl_eu_80502050, &lbl_eu_80502050[0x9], 0x78);
    char* str3 = func_80136190(lbl_eu_80502050, &lbl_eu_80502050[0x9], 0x79);

    ml::FixStr<128> buf;
    buf.format(&lbl_eu_80502050[0x480], str0, str1, str2, mArg3, str3, mArg4);
    func_8013D55C(buf.mString, 0, 0);
}

void CCol6Invite::Move() {}
