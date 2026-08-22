// Cursor layout overlay widgets - CCur unit
// High-level C/C++ reconstruction from retail PPC

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "kyoshin/CCur.hpp"

// String table for cursor layout/anim resource names
// (declared in include/lbls_kyoshin.hpp; keep local extern for MWCC relocs)
extern char lbl_eu_80505DE8[];

// Fake SI interface for the CBaseCur vtable slot +0x14 (vtable[5]): real
// virtual dispatch reproduces the retail lwz r12,0(r3); lwz r12,20(r12)
// sequence (manual ((void**)mVtable)[5] casts emit a scratch r4).
struct CBaseCurVt5 {
    virtual void _v00(); virtual void _v04(); virtual void _v08();
    virtual void m14();  // slot 5 => +0x14, no args (retail leaves r4 untouched)
};

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
// noinline: -ipa would fold this body into func_801D2180's call site
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
    func_80137444(cur->mpAnimTrans0, 1.0f);
    goto animate;
one:
    reinterpret_cast<CBaseCurVt5*>(cur)->m14();
animate:
    cur->mpLayout->Animate(0);
}

// ============================================================================
// func_801D20B0: Draw cursor layout
// ============================================================================
extern "C" void func_801D20B0(CBaseCur* cur, nw4r::lyt::DrawInfo* drawInfo) {
    if (cur->mpLayout == NULL) return;
    if (cur->mVisible == 0) return;
    func_80137038(cur->mpLayout, drawInfo, 0, 1);
}

// ============================================================================
// CBaseCur::func_801D20DC: Deinitialize cursor
// ============================================================================
extern "C" void func_801D20DC__8CBaseCurFv(CBaseCur* cur) {
    cur->mVisible = 0;
    cur->mpAnimTrans0 = NULL;
    cur->mpAnimTrans1 = NULL;
    if (cur->mpLayout != NULL) {
        delete cur->mpLayout;
        cur->mpLayout = NULL;
    }
}

// ============================================================================
// CBaseCur::func_801D2144: Forward translation to root pane
// ============================================================================
extern "C" void func_801D2144__8CBaseCurFv(CBaseCur* cur, const nw4r::math::VEC3* trans) {
    func_801D2150(cur->mpLayout->GetRootPane(), trans);
}

// ============================================================================
// func_801D216C: Set cursor visibility
// ============================================================================
extern "C" void func_801D216C(void* pCur, u8 val) {
    ((CBaseCur*)pCur)->mVisible = val;
}

// ============================================================================
// func_801D2174: Activate cursor
// ============================================================================
extern "C" void func_801D2174(CBaseCur* cur) {
    cur->mActive = 1;
    func_801D2264(cur);
}

// ============================================================================
// CBaseCur::func_801D2180: Check anim finish and deactivate
// ============================================================================
extern "C" void func_801D2180__8CBaseCurFv(CBaseCur* cur) {
    if (func_80137444(cur->mpAnimTrans1, 1.0f) != 0) {
        cur->mActive = 0;
        func_801D21CC(cur);
    }
}



// ============================================================================
// CBaseCur constructor
// ============================================================================
extern "C" DECOMP_DONT_INLINE void __ct__8CBaseCurFv(CBaseCur* _this, nw4r::lyt::ArcResourceAccessor* arcResAcc) {
    _this->mVtable = (void*)lbl_eu_805349A0;
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
extern "C" CBaseCur* __ct__CCur07(CBaseCur* _this, nw4r::lyt::ArcResourceAccessor* arcResAcc) {
    __ct__8CBaseCurFv(_this, arcResAcc);
    _this->mVtable = (void*)lbl_eu_80534978;
    return _this;
}

// ============================================================================
// CCur07 destructor
// ============================================================================
extern "C" void* __dt__6CCur07Fv(CBaseCur* _this, int flags) {
    if (_this != NULL && flags > 0) {
        __dl__FPv(_this);
    }
    return _this;
}

// ============================================================================
// CCur07::func_801D2378: Load layout curs07 and two anim transforms
// ============================================================================
extern "C" void func_801D2378__6CCur07Fv(CBaseCur* cur) {
    func_80136E84(&cur->mpLayout, cur->mArcResAcc, lbl_eu_80505DE8 + 0x50);
    func_80136F08(cur->mpLayout, &cur->mpAnimTrans0, cur->mArcResAcc, lbl_eu_80505DE8 + 0x68);
    func_80136F08(cur->mpLayout, &cur->mpAnimTrans1, cur->mArcResAcc, lbl_eu_80505DE8 + 0x85);
    cur->mpLayout->UnbindAllAnimation();
    func_801D21CC(cur);
}

// ============================================================================
// CCur09 constructor
// ============================================================================
extern "C" CBaseCur* __ct__CCur09(CBaseCur* _this, nw4r::lyt::ArcResourceAccessor* arcResAcc) {
    __ct__8CBaseCurFv(_this, arcResAcc);
    _this->mVtable = (void*)lbl_eu_80534950;
    return _this;
}

// ============================================================================
// CCur09 destructor
// ============================================================================
extern "C" void* __dt__6CCur09Fv(CBaseCur* _this, int flags) {
    if (_this != NULL && flags > 0) {
        __dl__FPv(_this);
    }
    return _this;
}

// ============================================================================
// CCur09::func_801D2478: Load layout curs09 and one anim transform
// ============================================================================
extern "C" void func_801D2478__6CCur09Fv(CBaseCur* cur) {
    func_80136E84(&cur->mpLayout, cur->mArcResAcc, lbl_eu_80505DE8 + 0xa0);
    func_80136F08(cur->mpLayout, &cur->mpAnimTrans0, cur->mArcResAcc, lbl_eu_80505DE8 + 0xb8);
    cur->mpLayout->UnbindAllAnimation();
    func_801D21CC(cur);
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
extern "C" CBaseCur* __ct__CCur11(CBaseCur* _this, nw4r::lyt::ArcResourceAccessor* arcResAcc) {
    __ct__8CBaseCurFv(_this, arcResAcc);
    _this->mVtable = (void*)lbl_eu_80534928;
    return _this;
}

// ============================================================================
// CCur11 destructor
// ============================================================================
extern "C" void* __dt__6CCur11Fv(CBaseCur* _this, int flags) {
    if (_this != NULL && flags > 0) {
        __dl__FPv(_this);
    }
    return _this;
}

// ============================================================================
// CCur11::func_801D25EC: Load layout curs11 and two anim transforms
// ============================================================================
extern "C" void func_801D25EC__6CCur11Fv(CBaseCur* cur) {
    func_80136E84(&cur->mpLayout, cur->mArcResAcc, lbl_eu_80505DE8 + 0xed);
    func_80136F08(cur->mpLayout, &cur->mpAnimTrans0, cur->mArcResAcc, lbl_eu_80505DE8 + 0x105);
    func_80136F08(cur->mpLayout, &cur->mpAnimTrans1, cur->mArcResAcc, lbl_eu_80505DE8 + 0x122);
    cur->mpLayout->UnbindAllAnimation();
    func_801D21CC(cur);
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
extern "C" CBaseCur* __ct__CCur14(CBaseCur* _this, nw4r::lyt::ArcResourceAccessor* arcResAcc) {
    __ct__8CBaseCurFv(_this, arcResAcc);
    _this->mVtable = (void*)lbl_eu_80534900;
    return _this;
}

// ============================================================================
// CCur14 destructor
// ============================================================================
extern "C" void* __dt__6CCur14Fv(CBaseCur* _this, int flags) {
    if (_this != NULL && flags > 0) {
        __dl__FPv(_this);
    }
    return _this;
}

// ============================================================================
// CCur14::func_801D2778: Load layout curs14 and two anim transforms
// ============================================================================
extern "C" void func_801D2778__6CCur14Fv(CBaseCur* cur) {
    func_80136E84(&cur->mpLayout, cur->mArcResAcc, lbl_eu_80505DE8 + 0x15d);
    func_80136F08(cur->mpLayout, &cur->mpAnimTrans0, cur->mArcResAcc, lbl_eu_80505DE8 + 0x175);
    func_80136F08(cur->mpLayout, &cur->mpAnimTrans1, cur->mArcResAcc, lbl_eu_80505DE8 + 0x192);
    cur->mpLayout->UnbindAllAnimation();
    func_801D21CC(cur);
}

// ============================================================================
// CCur15 constructor
// ============================================================================
extern "C" CBaseCur* __ct__CCur15(CBaseCur* _this, nw4r::lyt::ArcResourceAccessor* arcResAcc) {
    __ct__8CBaseCurFv(_this, arcResAcc);
    _this->mVtable = (void*)lbl_eu_805348D8;
    return _this;
}

// ============================================================================
// CCur15 destructor
// ============================================================================
extern "C" void* __dt__6CCur15Fv(CBaseCur* _this, int flags) {
    if (_this != NULL && flags > 0) {
        __dl__FPv(_this);
    }
    return _this;
}

// ============================================================================
// CCur15::func_801D2878: Load layout curs15 and two anim transforms
// ============================================================================
extern "C" void func_801D2878__6CCur15Fv(CBaseCur* cur) {
    func_80136E84(&cur->mpLayout, cur->mArcResAcc, lbl_eu_80505DE8 + 0x1ad);
    func_80136F08(cur->mpLayout, &cur->mpAnimTrans0, cur->mArcResAcc, lbl_eu_80505DE8 + 0x1c5);
    func_80136F08(cur->mpLayout, &cur->mpAnimTrans1, cur->mArcResAcc, lbl_eu_80505DE8 + 0x1e2);
    cur->mpLayout->UnbindAllAnimation();
    func_801D21CC(cur);
}

// ============================================================================
// CCur16 constructor
// ============================================================================
extern "C" CBaseCur* __ct__CCur16(CBaseCur* _this, nw4r::lyt::ArcResourceAccessor* arcResAcc) {
    __ct__8CBaseCurFv(_this, arcResAcc);
    _this->mVtable = (void*)lbl_eu_805348B0;
    return _this;
}

// ============================================================================
// CCur16 destructor
// ============================================================================
extern "C" void* __dt__6CCur16Fv(CBaseCur* _this, int flags) {
    if (_this != NULL && flags > 0) {
        __dl__FPv(_this);
    }
    return _this;
}

// ============================================================================
// CCur16::func_801D2978: Load layout curs16 and two anim transforms
// ============================================================================
extern "C" void func_801D2978__6CCur16Fv(CBaseCur* cur) {
    func_80136E84(&cur->mpLayout, cur->mArcResAcc, lbl_eu_80505DE8 + 0x1fd);
    func_80136F08(cur->mpLayout, &cur->mpAnimTrans0, cur->mArcResAcc, lbl_eu_80505DE8 + 0x215);
    func_80136F08(cur->mpLayout, &cur->mpAnimTrans1, cur->mArcResAcc, lbl_eu_80505DE8 + 0x232);
    cur->mpLayout->UnbindAllAnimation();
    func_801D21CC(cur);
}

// ============================================================================
// CCur18 constructor
// ============================================================================
extern "C" CBaseCur* __ct__CCur18(CBaseCur* _this, nw4r::lyt::ArcResourceAccessor* arcResAcc) {
    __ct__8CBaseCurFv(_this, arcResAcc);
    _this->mVtable = (void*)lbl_eu_80534884;
    return _this;
}

// ============================================================================
// CCur18 destructor
// ============================================================================
extern "C" void* __dt__6CCur18Fv(CBaseCur* _this, int flags) {
    if (_this != NULL && flags > 0) {
        __dl__FPv(_this);
    }
    return _this;
}

// ============================================================================
// CCur18::func_801D2A78: Load layout curs18 and two anim transforms
// ============================================================================
extern "C" void func_801D2A78__6CCur18Fv(CBaseCur* cur) {
    func_80136E84(&cur->mpLayout, cur->mArcResAcc, lbl_eu_80505DE8 + 0x29d);
    func_80136F08(cur->mpLayout, &cur->mpAnimTrans0, cur->mArcResAcc, lbl_eu_80505DE8 + 0x2b5);
    func_80136F08(cur->mpLayout, &cur->mpAnimTrans1, cur->mArcResAcc, lbl_eu_80505DE8 + 0x2d2);
    cur->mpLayout->UnbindAllAnimation();
    func_801D21CC(cur);
}

// ============================================================================
// CCur22 constructor
// ============================================================================
extern "C" CBaseCur* __ct__CCur22(CBaseCur* _this, nw4r::lyt::ArcResourceAccessor* arcResAcc) {
    __ct__8CBaseCurFv(_this, arcResAcc);
    _this->mVtable = (void*)lbl_eu_8053485C;
    return _this;
}

// ============================================================================
// CCur22 destructor
// ============================================================================
extern "C" void* __dt__6CCur22Fv(CBaseCur* _this, int flags) {
    if (_this != NULL && flags > 0) {
        __dl__FPv(_this);
    }
    return _this;
}

// ============================================================================
// CCur22::func_801D2B78: Load layout curs22 and two anim transforms
// ============================================================================
extern "C" void func_801D2B78__6CCur22Fv(CBaseCur* cur) {
    func_80136E84(&cur->mpLayout, cur->mArcResAcc, lbl_eu_80505DE8 + 0x33d);
    func_80136F08(cur->mpLayout, &cur->mpAnimTrans0, cur->mArcResAcc, lbl_eu_80505DE8 + 0x355);
    func_80136F08(cur->mpLayout, &cur->mpAnimTrans1, cur->mArcResAcc, lbl_eu_80505DE8 + 0x372);
    cur->mpLayout->UnbindAllAnimation();
    func_801D21CC(cur);
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
extern "C" CBaseCur* __ct__CSubCur(CBaseCur* _this, nw4r::lyt::ArcResourceAccessor* arcResAcc) {
    __ct__8CBaseCurFv(_this, arcResAcc);
    _this->mVtable = (void*)lbl_eu_80534838;
    return _this;
}

// ============================================================================
// CSubCur destructor
// ============================================================================
extern "C" void* __dt__7CSubCurFv(CBaseCur* _this, int flags) {
    if (_this != NULL && flags > 0) {
        __dl__FPv(_this);
    }
    return _this;
}

// ============================================================================
// CSubCur::func_801D2DC8: Load layout subcur and two anim transforms
// ============================================================================
extern "C" void func_801D2DC8__7CSubCurFv(CBaseCur* cur) {
    func_80136E84(&cur->mpLayout, cur->mArcResAcc, lbl_eu_80505DE8 + 0x38d);
    func_80136F08(cur->mpLayout, &cur->mpAnimTrans0, cur->mArcResAcc, lbl_eu_80505DE8 + 0x3a9);
    func_80136F08(cur->mpLayout, &cur->mpAnimTrans1, cur->mArcResAcc, lbl_eu_80505DE8 + 0x3ca);
    cur->mpLayout->UnbindAllAnimation();
    func_801D21CC(cur);
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