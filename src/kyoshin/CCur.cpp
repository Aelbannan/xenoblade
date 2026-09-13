// Cursor layout overlay widgets - CCur unit
// High-level C/C++ reconstruction from retail PPC

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "kyoshin/CCur.hpp"

// String table for cursor layout/anim resource names
// (declared in include/lbls_kyoshin.hpp; keep local extern for MWCC relocs)
extern char lbl_eu_80505DE8[];

// (CBaseCur now carries the real virtuals; func_801D202C dispatches
// checkDeactivate() at +0x14 directly.)

// ============================================================================
// func_801D2150: Set pane translate from VEC3
// ============================================================================
// func_801D2150 is declared extern "C" in CCur.hpp (retail unmangled name);
// define it here with plain C++ syntax against that declaration.
DECOMP_DONT_INLINE void func_801D2150(nw4r::lyt::Pane* pane, const nw4r::math::VEC3* trans) {
    pane->SetTranslate(*trans);
}

// ============================================================================
// func_801D21CC: Deactivation tail handler
// Unbinds the active anim transform, re-binds animTrans0, enables and resets
// its frame, then re-animates. Retail reloads mpLayout before every call.
// noinline: -ipa would fold this body into checkDeactivate's call site
// (retail keeps the bl out-of-line).
extern "C" __declspec(noinline) void func_801D21CC(CBaseCur* cur) {
    cur->mpLayout->UnbindAnimation(cur->mpAnimTrans1);
    cur->mpLayout->BindAnimation(cur->mpAnimTrans0);
    cur->mpLayout->SetAnimationEnable(cur->mpAnimTrans0, true);
    cur->mpAnimTrans0->SetFrame(lbl_eu_80667FA4);
    cur->mpLayout->Animate(0);
}

// ============================================================================
// func_801D2264: Activation tail handler
// Unbinds the active anim transform, re-binds animTrans1, enables and resets
// its frame, then re-animates. Retail reloads mpLayout before every call.
// ============================================================================
// noinline: retail callers tail-branch; an inline would fold the body in.
extern "C" __declspec(noinline) void func_801D2264(CBaseCur* cur) {
    cur->mpLayout->UnbindAnimation(cur->mpAnimTrans0);
    cur->mpLayout->BindAnimation(cur->mpAnimTrans1);
    cur->mpLayout->SetAnimationEnable(cur->mpAnimTrans1, true);
    cur->mpAnimTrans1->SetFrame(lbl_eu_80667FA4);
    cur->mpLayout->Animate(0);
}

// ============================================================================
// func_801D202C: Per-frame cursor update
// ============================================================================
extern "C" void func_801D202C(CBaseCur* cur) {
    if (cur->mpLayout == NULL) {
        return;
    }
    switch (cur->mActive) {
    case 0:
        goto zero;
    case 1:
        goto one;
    default:
        goto animate;
    }
zero:
    advanceAnimTransform(cur->mpAnimTrans0, 1.0f);
    goto animate;
one:
    cur->checkDeactivate();
animate:
    cur->mpLayout->Animate(0);
}

// ============================================================================
// func_801D20B0: Draw cursor layout
// ============================================================================
extern "C" void func_801D20B0(CBaseCur* cur, nw4r::lyt::DrawInfo* drawInfo) {
    if (cur->mpLayout == NULL) return;
    if (cur->mVisible == 0) return;
    drawLayout(cur->mpLayout, drawInfo, 0, 1);
}

// ============================================================================
// CBaseCur::cleanup: Deinitialize cursor
// ============================================================================
void CBaseCur::cleanup() {
    mVisible = 0;
    mpAnimTrans0 = NULL;
    mpAnimTrans1 = NULL;
    if (mpLayout != NULL) {
        delete mpLayout;
        mpLayout = NULL;
    }
}

// ============================================================================
// CBaseCur::setRootPaneTranslate: Forward translation to root pane
// ============================================================================
extern "C" void setRootPaneTranslate__8CBaseCurFv(CBaseCur* cur, const nw4r::math::VEC3* trans) {
    func_801D2150(cur->mpLayout->GetRootPane(), trans);
}

// ============================================================================
// func_801D216C: Set cursor visibility
// ============================================================================
extern "C" void func_801D216C(CBaseCur* cur, u8 val) {
    cur->mVisible = val;
}

// ============================================================================
// func_801D2174: Activate cursor
// ============================================================================
extern "C" void func_801D2174(CBaseCur* cur) {
    cur->mActive = 1;
    func_801D2264(cur);
}

// ============================================================================
// CBaseCur::checkDeactivate: Check anim finish and deactivate
// ============================================================================
void CBaseCur::checkDeactivate() {
    if (advanceAnimTransform(mpAnimTrans1, 1.0f) != 0) {
        mActive = 0;
        func_801D21CC(this);
    }
}



// ============================================================================
// CBaseCur constructor
// ============================================================================
extern "C" DECOMP_DONT_INLINE void __ct__8CBaseCurFv(CBaseCur* _this, nw4r::lyt::ArcResourceAccessor* arcResAcc) {
    _this->vtbl() = (void*)lbl_eu_805349A0;
    _this->mArcResAcc = arcResAcc;
    _this->mpLayout = nullptr;
    _this->mpAnimTrans0 = nullptr;
    _this->mpAnimTrans1 = nullptr;
    _this->mActive = 0;
    _this->mVisible = 0;
}

// ============================================================================
// CBaseCur destructor
// ============================================================================
extern "C" void* __dt__8CBaseCurFv(CBaseCur* _this, int flags) {
    if (_this != NULL && flags > 0) {
        __dl__FPv(_this);
    }
    return _this;
}

// ============================================================================
// CCur07 constructor
// ============================================================================
extern "C" CCur07* __ct__CCur07(CCur07* _this, nw4r::lyt::ArcResourceAccessor* arcResAcc) {
    __ct__8CBaseCurFv(_this, arcResAcc);
    _this->vtbl() = (void*)lbl_eu_80534978;
    return _this;
}

// ============================================================================
// CCur07 destructor
// ============================================================================
extern "C" void* __dt__6CCur07Fv(CCur07* _this, int flags) {
    if (_this != NULL && flags > 0) {
        __dl__FPv(_this);
    }
    return _this;
}

// ============================================================================
// CCur07::initLayout: Load layout curs07 and two anim transforms
// ============================================================================
void CCur07::initLayout() {
    buildLayout(&mpLayout, mArcResAcc, lbl_eu_80505DE8 + 0x50);
    bindLayoutAnimTransform(mpLayout, &mpAnimTrans0, mArcResAcc, lbl_eu_80505DE8 + 0x68);
    bindLayoutAnimTransform(mpLayout, &mpAnimTrans1, mArcResAcc, lbl_eu_80505DE8 + 0x85);
    mpLayout->UnbindAllAnimation();
    func_801D21CC(this);
}

// ============================================================================
// CCur09 constructor
// ============================================================================
extern "C" CCur09* __ct__CCur09(CCur09* _this, nw4r::lyt::ArcResourceAccessor* arcResAcc) {
    __ct__8CBaseCurFv(_this, arcResAcc);
    _this->vtbl() = (void*)lbl_eu_80534950;
    return _this;
}

// ============================================================================
// CCur09 destructor
// ============================================================================
extern "C" void* __dt__6CCur09Fv(CCur09* _this, int flags) {
    if (_this != NULL && flags > 0) {
        __dl__FPv(_this);
    }
    return _this;
}

// ============================================================================
// CCur09::initLayout: Load layout curs09 and one anim transform
// ============================================================================
void CCur09::initLayout() {
    buildLayout(&mpLayout, mArcResAcc, lbl_eu_80505DE8 + 0xa0);
    bindLayoutAnimTransform(mpLayout, &mpAnimTrans0, mArcResAcc, lbl_eu_80505DE8 + 0xb8);
    mpLayout->UnbindAllAnimation();
    func_801D21CC(this);
}

// ============================================================================
// func_801D24E8: Set two pane positions on cursor (for CCur09 variant)
// Uses named panes from the root pane's children.
// ============================================================================
// Retail reloads mpLayout/root pane for each lookup (no caching) - caching
// the root pane changes MWCC's register allocation away from stmw r28-r31.
extern "C" void func_801D24E8(CBaseCur* cur, const nw4r::math::VEC3* trans0, const nw4r::math::VEC3* trans1) {
    func_801D2150(cur->mpLayout->GetRootPane()->FindPaneByName(lbl_eu_80505DE8 + 0xd5, true), trans0);
    func_801D2150(cur->mpLayout->GetRootPane()->FindPaneByName(lbl_eu_80505DE8 + 0xe1, true), trans1);
}

// ============================================================================
// CCur11 constructor
// ============================================================================
extern "C" CCur11* __ct__CCur11(CCur11* _this, nw4r::lyt::ArcResourceAccessor* arcResAcc) {
    __ct__8CBaseCurFv(_this, arcResAcc);
    _this->vtbl() = (void*)lbl_eu_80534928;
    return _this;
}

// ============================================================================
// CCur11 destructor
// ============================================================================
extern "C" void* __dt__6CCur11Fv(CCur11* _this, int flags) {
    if (_this != NULL && flags > 0) {
        __dl__FPv(_this);
    }
    return _this;
}

// ============================================================================
// CCur11::initLayout: Load layout curs11 and two anim transforms
// ============================================================================
void CCur11::initLayout() {
    buildLayout(&mpLayout, mArcResAcc, lbl_eu_80505DE8 + 0xed);
    bindLayoutAnimTransform(mpLayout, &mpAnimTrans0, mArcResAcc, lbl_eu_80505DE8 + 0x105);
    bindLayoutAnimTransform(mpLayout, &mpAnimTrans1, mArcResAcc, lbl_eu_80505DE8 + 0x122);
    mpLayout->UnbindAllAnimation();
    func_801D21CC(this);
}

// ============================================================================
// func_801D2670: Set visibility of two named panes
// ============================================================================
// Same no-caching shape as func_801D24E8 (retail reloads root pane twice).
extern "C" void func_801D2670(CBaseCur* cur, u8 visible) {
    if (cur->mpLayout == NULL) return;
    func_80124270(cur->mpLayout->GetRootPane()->FindPaneByName(lbl_eu_80505DE8 + 0x13d, true), visible);
    func_80124270(cur->mpLayout->GetRootPane()->FindPaneByName(lbl_eu_80505DE8 + 0x14d, true), visible);
}

// ============================================================================
// CCur14 constructor
// ============================================================================
extern "C" CCur14* __ct__CCur14(CCur14* _this, nw4r::lyt::ArcResourceAccessor* arcResAcc) {
    __ct__8CBaseCurFv(_this, arcResAcc);
    _this->vtbl() = (void*)lbl_eu_80534900;
    return _this;
}

// ============================================================================
// CCur14 destructor
// ============================================================================
extern "C" void* __dt__6CCur14Fv(CCur14* _this, int flags) {
    if (_this != NULL && flags > 0) {
        __dl__FPv(_this);
    }
    return _this;
}

// ============================================================================
// CCur14::initLayout: Load layout curs14 and two anim transforms
// ============================================================================
void CCur14::initLayout() {
    buildLayout(&mpLayout, mArcResAcc, lbl_eu_80505DE8 + 0x15d);
    bindLayoutAnimTransform(mpLayout, &mpAnimTrans0, mArcResAcc, lbl_eu_80505DE8 + 0x175);
    bindLayoutAnimTransform(mpLayout, &mpAnimTrans1, mArcResAcc, lbl_eu_80505DE8 + 0x192);
    mpLayout->UnbindAllAnimation();
    func_801D21CC(this);
}

// ============================================================================
// CCur15 constructor
// ============================================================================
extern "C" CCur15* __ct__CCur15(CCur15* _this, nw4r::lyt::ArcResourceAccessor* arcResAcc) {
    __ct__8CBaseCurFv(_this, arcResAcc);
    _this->vtbl() = (void*)lbl_eu_805348D8;
    return _this;
}

// ============================================================================
// CCur15 destructor
// ============================================================================
extern "C" void* __dt__6CCur15Fv(CCur15* _this, int flags) {
    if (_this != NULL && flags > 0) {
        __dl__FPv(_this);
    }
    return _this;
}

// ============================================================================
// CCur15::initLayout: Load layout curs15 and two anim transforms
// ============================================================================
void CCur15::initLayout() {
    buildLayout(&mpLayout, mArcResAcc, lbl_eu_80505DE8 + 0x1ad);
    bindLayoutAnimTransform(mpLayout, &mpAnimTrans0, mArcResAcc, lbl_eu_80505DE8 + 0x1c5);
    bindLayoutAnimTransform(mpLayout, &mpAnimTrans1, mArcResAcc, lbl_eu_80505DE8 + 0x1e2);
    mpLayout->UnbindAllAnimation();
    func_801D21CC(this);
}

// ============================================================================
// CCur16 constructor
// ============================================================================
extern "C" CCur16* __ct__CCur16(CCur16* _this, nw4r::lyt::ArcResourceAccessor* arcResAcc) {
    __ct__8CBaseCurFv(_this, arcResAcc);
    _this->vtbl() = (void*)lbl_eu_805348B0;
    return _this;
}

// ============================================================================
// CCur16 destructor
// ============================================================================
extern "C" void* __dt__6CCur16Fv(CCur16* _this, int flags) {
    if (_this != NULL && flags > 0) {
        __dl__FPv(_this);
    }
    return _this;
}

// ============================================================================
// CCur16::initLayout: Load layout curs16 and two anim transforms
// ============================================================================
void CCur16::initLayout() {
    buildLayout(&mpLayout, mArcResAcc, lbl_eu_80505DE8 + 0x1fd);
    bindLayoutAnimTransform(mpLayout, &mpAnimTrans0, mArcResAcc, lbl_eu_80505DE8 + 0x215);
    bindLayoutAnimTransform(mpLayout, &mpAnimTrans1, mArcResAcc, lbl_eu_80505DE8 + 0x232);
    mpLayout->UnbindAllAnimation();
    func_801D21CC(this);
}

// ============================================================================
// CCur18 constructor
// ============================================================================
extern "C" CCur18* __ct__CCur18(CCur18* _this, nw4r::lyt::ArcResourceAccessor* arcResAcc) {
    __ct__8CBaseCurFv(_this, arcResAcc);
    _this->vtbl() = (void*)lbl_eu_80534884;
    return _this;
}

// ============================================================================
// CCur18 destructor
// ============================================================================
extern "C" void* __dt__6CCur18Fv(CCur18* _this, int flags) {
    if (_this != NULL && flags > 0) {
        __dl__FPv(_this);
    }
    return _this;
}

// ============================================================================
// CCur18::initLayout: Load layout curs18 and two anim transforms
// ============================================================================
void CCur18::initLayout() {
    buildLayout(&mpLayout, mArcResAcc, lbl_eu_80505DE8 + 0x29d);
    bindLayoutAnimTransform(mpLayout, &mpAnimTrans0, mArcResAcc, lbl_eu_80505DE8 + 0x2b5);
    bindLayoutAnimTransform(mpLayout, &mpAnimTrans1, mArcResAcc, lbl_eu_80505DE8 + 0x2d2);
    mpLayout->UnbindAllAnimation();
    func_801D21CC(this);
}

// ============================================================================
// CCur22 constructor
// ============================================================================
extern "C" CCur22* __ct__CCur22(CCur22* _this, nw4r::lyt::ArcResourceAccessor* arcResAcc) {
    __ct__8CBaseCurFv(_this, arcResAcc);
    _this->vtbl() = (void*)lbl_eu_8053485C;
    return _this;
}

// ============================================================================
// CCur22 destructor
// ============================================================================
extern "C" void* __dt__6CCur22Fv(CCur22* _this, int flags) {
    if (_this != NULL && flags > 0) {
        __dl__FPv(_this);
    }
    return _this;
}

// ============================================================================
// CCur22::initLayout: Load layout curs22 and two anim transforms
// ============================================================================
void CCur22::initLayout() {
    buildLayout(&mpLayout, mArcResAcc, lbl_eu_80505DE8 + 0x33d);
    bindLayoutAnimTransform(mpLayout, &mpAnimTrans0, mArcResAcc, lbl_eu_80505DE8 + 0x355);
    bindLayoutAnimTransform(mpLayout, &mpAnimTrans1, mArcResAcc, lbl_eu_80505DE8 + 0x372);
    mpLayout->UnbindAllAnimation();
    func_801D21CC(this);
}

// ============================================================================
// func_801D2BFC: Set pane visibility for all 4 sub-panes by index
// Highlights the pane at `index` by setting it visible and hiding others.
// ============================================================================
extern "C" void func_801D2BFC(CBaseCur* cur, u8 index) {
    for (u8 i = 0; i < 4; i++) {
        // Branch form: retail emits li 0 / cmpl / bne / li 1, not select-arithmetic
        u8 visible = 0;
        if (i == index) {
            visible = 1;
        }
        nw4r::lyt::Pane* pane = cur->mpLayout->GetRootPane()->FindPaneByName(lbl_eu_80534828[i], true);
        func_80124270(pane, visible);
    }
}

// ============================================================================
// func_801D2C80: Position a cursor pane by index
// Index 4 targets the root pane directly; indices 0-3 look up named sub-panes.
// ============================================================================
extern "C" void func_801D2C80(CBaseCur* cur, const nw4r::math::VEC3* trans, u8 index) {
    if (index == 4) {
        func_801D2150(cur->mpLayout->GetRootPane(), trans);
    } else {
        nw4r::lyt::Pane* pane = cur->mpLayout->GetRootPane()->FindPaneByName(lbl_eu_80534828[index], true);
        func_801D2150(pane, trans);
    }
}

// ============================================================================
// func_801D2CF4: Show pane by index (0-3 only)
// ============================================================================
extern "C" void func_801D2CF4(CBaseCur* cur, u8 index) {
    if (index >= 4) return;
    nw4r::lyt::Pane* pane = cur->mpLayout->GetRootPane()->FindPaneByName(lbl_eu_80534828[index], true);
    func_80124270(pane, 1);
}

// ============================================================================
// CSubCur constructor
// ============================================================================
extern "C" CSubCur* __ct__CSubCur(CSubCur* _this, nw4r::lyt::ArcResourceAccessor* arcResAcc) {
    __ct__8CBaseCurFv(_this, arcResAcc);
    _this->vtbl() = (void*)lbl_eu_80534838;
    return _this;
}

// ============================================================================
// CSubCur destructor
// ============================================================================
extern "C" void* __dt__7CSubCurFv(CSubCur* _this, int flags) {
    if (_this != NULL && flags > 0) {
        __dl__FPv(_this);
    }
    return _this;
}

// ============================================================================
// CSubCur::initLayout: Load layout subcur and two anim transforms
// ============================================================================
void CSubCur::initLayout() {
    buildLayout(&mpLayout, mArcResAcc, lbl_eu_80505DE8 + 0x38d);
    bindLayoutAnimTransform(mpLayout, &mpAnimTrans0, mArcResAcc, lbl_eu_80505DE8 + 0x3a9);
    bindLayoutAnimTransform(mpLayout, &mpAnimTrans1, mArcResAcc, lbl_eu_80505DE8 + 0x3ca);
    mpLayout->UnbindAllAnimation();
    func_801D21CC(this);
}

// ============================================================================
// func_801D2E4C: Set visibility of two named panes (subcur variant)
// Retail reloads mpLayout + root pane for the second lookup (no caching).
// ============================================================================
extern "C" void func_801D2E4C(CBaseCur* cur, u8 visible) {
    if (cur->mpLayout == NULL) return;
    func_80124270(cur->mpLayout->GetRootPane()->FindPaneByName(lbl_eu_80505DE8 + 0x3e9, true), visible);
    func_80124270(cur->mpLayout->GetRootPane()->FindPaneByName(lbl_eu_80505DE8 + 0x3f7, true), visible);
}

// ============================================================================
// func_801D2ED8: Check if both subcur panes are visible/active
// Returns 1 if both named panes return non-zero from func_801C4648, 0 otherwise.
// ============================================================================
extern "C" u8 func_801D2ED8(CBaseCur* cur) {
    if (cur->mpLayout == NULL) return 0;
    u32 result0 = func_801C4648(cur->mpLayout->GetRootPane()->FindPaneByName(lbl_eu_80505DE8 + 0x3e9, true));
    u32 result1 = func_801C4648(cur->mpLayout->GetRootPane()->FindPaneByName(lbl_eu_80505DE8 + 0x3f7, true));
    return (result0 != 0 && result1 != 0) ? 1 : 0;
}