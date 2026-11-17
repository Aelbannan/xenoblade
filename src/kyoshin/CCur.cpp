// Cursor layout overlay widgets - CCur unit
// High-level C/C++ reconstruction from retail PPC

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "kyoshin/CCur.hpp"

// String table for cursor layout/anim resource names
// (declared in include/lbls_kyoshin.hpp; keep local extern for MWCC relocs)
extern char lbl_eu_80505DE8[];
// Retail 1.0f pool owned by CItemBoxGrid's .sdata2 tail (split
// 0x80667F30-0x80667FA4); this TU's .text loads it as an extern symbol
// (retail .text relocs), so no local pool is emitted and our .sdata2 is
// exactly the split's 0x4C.
extern const f32 lbl_eu_80667FA0;

// Pane visibility helper (retail bare symbol; canonical extern "C" decl).
extern "C" void setPaneVisible(nw4r::lyt::Pane*, u32);
extern "C" bool isPaneVisible(nw4r::lyt::Pane*);

// --- .sdata2 (retail content 0x4C): 0.0f + class-name strings ---
// Retail FA4 slot is the first word of this TU's .sdata2 (symbols.txt:
// float at FA4), immediately followed by the class-name strings with no
// padding, so the honest source shape is a single object: a struct whose
// first member is the 0.0f. The struct itself carries the retail name
// (linker/frontier care about offset+section, not the C++ type); code
// reads the float as lbl_eu_80667FA4.f0, which MWCC loads via sda21
// exactly like a standalone extern float. The struct is non-const so the
// .f0 reads cannot be const-folded into a trailing pool (verified trap).
// A standalone loose float does NOT work: MWCC 8-aligns a following
// aggregate, inserting 4 pad bytes (measured 0x50 vs retail 0x4C).
struct Sdata2_CCur {
    f32 f0;             // +0x00 (retail FA4 slot) 0.0f
    char s_CSubCur[8];  // +0x04
    char s_CCur22[8];   // +0x0C
    char s_CCur18[8];   // +0x14
    char s_CCur16[8];   // +0x1C
    char s_CCur15[8];   // +0x24
    char s_CCur14[8];   // +0x2C
    char s_CCur11[8];   // +0x34
    char s_CCur09[8];   // +0x3C
    char s_CCur07[8];   // +0x44
};
__declspec(section ".sdata2") __attribute__((used))
Sdata2_CCur lbl_eu_80667FA4 = {
    0.0f, "CSubCur", "CCur22", "CCur18", "CCur16", "CCur15",
    "CCur14", "CCur11", "CCur09", "CCur07",
};


// (CBaseCur now carries the real virtuals; Cur_Update dispatches
// checkDeactivate() at +0x14 directly.)

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
    if (_this != nullptr && flags > 0) {
        operator delete(_this);
    }
    return _this;
}

// ============================================================================
// Cur_Update: Per-frame cursor update
// ============================================================================
extern "C" void Cur_Update(CBaseCur* cur) {
    if (cur->mpLayout == nullptr) {
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
    advanceAnimTransform(cur->mpAnimTrans0, lbl_eu_80667FA0);
    goto animate;
one:
    cur->checkDeactivate();
animate:
    cur->mpLayout->Animate(0);
}

// ============================================================================
// Cur_DrawLayout: Draw cursor layout
// ============================================================================
extern "C" void Cur_DrawLayout(CBaseCur* cur, nw4r::lyt::DrawInfo* drawInfo) {
    if (cur->mpLayout == nullptr) return;
    if (cur->mVisible == 0) return;
    drawLayout(cur->mpLayout, drawInfo, 0, 1);
}

// ============================================================================
// CBaseCur::cleanup: Deinitialize cursor
// ============================================================================
void CBaseCur::cleanup() {
    mVisible = 0;
    mpAnimTrans0 = nullptr;
    mpAnimTrans1 = nullptr;
    if (mpLayout != nullptr) {
        delete mpLayout;
        mpLayout = nullptr;
    }
}

// ============================================================================
// CBaseCur::setRootPaneTranslate: Forward translation to root pane
// ============================================================================
extern "C" void setRootPaneTranslate__8CBaseCurFv(CBaseCur* cur, const nw4r::math::VEC3* trans) {
    Cur_SetPaneTranslate(cur->mpLayout->GetRootPane(), trans);
}

// ============================================================================
// Cur_SetPaneTranslate: Set pane translate from VEC3
// ============================================================================
// Cur_SetPaneTranslate is declared extern "C" in CCur.hpp (retail unmangled name);
// define it here with plain C++ syntax against that declaration.
DECOMP_DONT_INLINE void Cur_SetPaneTranslate(nw4r::lyt::Pane* pane, const nw4r::math::VEC3* trans) {
    pane->SetTranslate(*trans);
}

// ============================================================================
// Cur_SetVisible: Set cursor visibility
// ============================================================================
extern "C" void Cur_SetVisible(CBaseCur* cur, u8 val) {
    cur->mVisible = val;
}

// ============================================================================
// Cur_SetActive: Activate cursor
// ============================================================================
extern "C" void Cur_SetActive(CBaseCur* cur) {
    cur->mActive = 1;
    Cur_ActivateTail(cur);
}

// ============================================================================
// CBaseCur::checkDeactivate: Check anim finish and deactivate
// ============================================================================
void CBaseCur::checkDeactivate() {
    if (advanceAnimTransform(mpAnimTrans1, lbl_eu_80667FA0) != 0) {
        mActive = 0;
        Cur_DeactivateTail(this);
    }
}



// ============================================================================
// Cur_DeactivateTail: Deactivation tail handler
// Unbinds the active anim transform, re-binds animTrans0, enables and resets
// its frame, then re-animates. Retail reloads mpLayout before every call.
// noinline: -ipa would fold this body into checkDeactivate's call site
// (retail keeps the bl out-of-line).
extern "C" __declspec(noinline) void Cur_DeactivateTail(CBaseCur* cur) {
    cur->mpLayout->UnbindAnimation(cur->mpAnimTrans1);
    cur->mpLayout->BindAnimation(cur->mpAnimTrans0);
    cur->mpLayout->SetAnimationEnable(cur->mpAnimTrans0, true);
    cur->mpAnimTrans0->SetFrame(lbl_eu_80667FA4.f0);
    cur->mpLayout->Animate(0);
}

// ============================================================================
// Cur_ActivateTail: Activation tail handler
// Unbinds the active anim transform, re-binds animTrans1, enables and resets
// its frame, then re-animates. Retail reloads mpLayout before every call.
// ============================================================================
// noinline: retail callers tail-branch; an inline would fold the body in.
extern "C" __declspec(noinline) void Cur_ActivateTail(CBaseCur* cur) {
    cur->mpLayout->UnbindAnimation(cur->mpAnimTrans0);
    cur->mpLayout->BindAnimation(cur->mpAnimTrans1);
    cur->mpLayout->SetAnimationEnable(cur->mpAnimTrans1, true);
    cur->mpAnimTrans1->SetFrame(lbl_eu_80667FA4.f0);
    cur->mpLayout->Animate(0);
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
    if (_this != nullptr && flags > 0) {
        operator delete(_this);
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
    Cur_DeactivateTail(this);
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
    if (_this != nullptr && flags > 0) {
        operator delete(_this);
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
    Cur_DeactivateTail(this);
}

// ============================================================================
// Cur_SetTwoPanes09: Set two pane positions on cursor (for CCur09 variant)
// Uses named panes from the root pane's children.
// ============================================================================
// Retail reloads mpLayout/root pane for each lookup (no caching) - caching
// the root pane changes MWCC's register allocation away from stmw r28-r31.
extern "C" void Cur_SetTwoPanes09(CBaseCur* cur, const nw4r::math::VEC3* trans0, const nw4r::math::VEC3* trans1) {
    Cur_SetPaneTranslate(cur->mpLayout->GetRootPane()->FindPaneByName(lbl_eu_80505DE8 + 0xd5, true), trans0);
    Cur_SetPaneTranslate(cur->mpLayout->GetRootPane()->FindPaneByName(lbl_eu_80505DE8 + 0xe1, true), trans1);
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
    if (_this != nullptr && flags > 0) {
        operator delete(_this);
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
    Cur_DeactivateTail(this);
}

// ============================================================================
// Cur_ShowTwoPanes: Set visibility of two named panes
// ============================================================================
// Same no-caching shape as Cur_SetTwoPanes09 (retail reloads root pane twice).
extern "C" void Cur_ShowTwoPanes(CBaseCur* cur, u8 visible) {
    if (cur->mpLayout == nullptr) return;
    setPaneVisible(cur->mpLayout->GetRootPane()->FindPaneByName(lbl_eu_80505DE8 + 0x13d, true), visible);
    setPaneVisible(cur->mpLayout->GetRootPane()->FindPaneByName(lbl_eu_80505DE8 + 0x14d, true), visible);
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
    if (_this != nullptr && flags > 0) {
        operator delete(_this);
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
    Cur_DeactivateTail(this);
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
    if (_this != nullptr && flags > 0) {
        operator delete(_this);
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
    Cur_DeactivateTail(this);
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
    if (_this != nullptr && flags > 0) {
        operator delete(_this);
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
    Cur_DeactivateTail(this);
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
    if (_this != nullptr && flags > 0) {
        operator delete(_this);
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
    Cur_DeactivateTail(this);
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
    if (_this != nullptr && flags > 0) {
        operator delete(_this);
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
    Cur_DeactivateTail(this);
}

// ============================================================================
// Cur_HighlightPane: Set pane visibility for all 4 sub-panes by index
// Highlights the pane at `index` by setting it visible and hiding others.
// ============================================================================
extern "C" void Cur_HighlightPane(CBaseCur* cur, u8 index) {
    for (u8 i = 0; i < 4; i++) {
        // Branch form: retail emits li 0 / cmpl / bne / li 1, not select-arithmetic
        u8 visible = 0;
        if (i == index) {
            visible = 1;
        }
        nw4r::lyt::Pane* pane = cur->mpLayout->GetRootPane()->FindPaneByName(lbl_eu_80534828[i], true);
        setPaneVisible(pane, visible);
    }
}

// ============================================================================
// Cur_PlacePaneByIndex: Position a cursor pane by index
// Index 4 targets the root pane directly; indices 0-3 look up named sub-panes.
// ============================================================================
extern "C" void Cur_PlacePaneByIndex(CBaseCur* cur, const nw4r::math::VEC3* trans, u8 index) {
    if (index == 4) {
        Cur_SetPaneTranslate(cur->mpLayout->GetRootPane(), trans);
    } else {
        nw4r::lyt::Pane* pane = cur->mpLayout->GetRootPane()->FindPaneByName(lbl_eu_80534828[index], true);
        Cur_SetPaneTranslate(pane, trans);
    }
}

// ============================================================================
// Cur_ShowPane: Show pane by index (0-3 only)
// ============================================================================
extern "C" void Cur_ShowPane(CBaseCur* cur, u8 index) {
    if (index >= 4) return;
    nw4r::lyt::Pane* pane = cur->mpLayout->GetRootPane()->FindPaneByName(lbl_eu_80534828[index], true);
    setPaneVisible(pane, 1);
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
    if (_this != nullptr && flags > 0) {
        operator delete(_this);
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
    Cur_DeactivateTail(this);
}

// ============================================================================
// Cur_ShowTwoSubPanes: Set visibility of two named panes (subcur variant)
// Retail reloads mpLayout + root pane for the second lookup (no caching).
// ============================================================================
extern "C" void Cur_ShowTwoSubPanes(CBaseCur* cur, u8 visible) {
    if (cur->mpLayout == nullptr) return;
    setPaneVisible(cur->mpLayout->GetRootPane()->FindPaneByName(lbl_eu_80505DE8 + 0x3e9, true), visible);
    setPaneVisible(cur->mpLayout->GetRootPane()->FindPaneByName(lbl_eu_80505DE8 + 0x3f7, true), visible);
}

// ============================================================================
// Cur_BothSubPanesOn: Check if both subcur panes are visible/active
// Returns 1 if both named panes return non-zero from isPaneVisible, 0 otherwise.
// ============================================================================
extern "C" u8 Cur_BothSubPanesOn(CBaseCur* cur) {
    if (cur->mpLayout == nullptr) return 0;
    u32 result0 = isPaneVisible(cur->mpLayout->GetRootPane()->FindPaneByName(lbl_eu_80505DE8 + 0x3e9, true));
    u32 result1 = isPaneVisible(cur->mpLayout->GetRootPane()->FindPaneByName(lbl_eu_80505DE8 + 0x3f7, true));
    return (result0 != 0 && result1 != 0) ? 1 : 0;
}
// ============================================================================
// Retail data sections (wave DL class C rebuild, byte-faithful).
//
// The retail unit owns .rodata (0x438) / .data (0x190) / .sdata (0x48) /
// .sdata2 (0x4C); the decomp TU previously emitted only a 1.0f literal pool,
// so every data label was a link-frontier row. Shapes decoded from the
// retail object (objdump -dr) and from the code's label-relative accesses:
//
// - .rodata: 4 pane-name strings (the const char* array's targets) followed
//   by the 0x408 layout/anim/pane name pool the code indexes as
//   lbl_eu_80505DE8+N (every +N in initLayout/ShowPane sites lands on a
//   string start; verified against the retail bytes).
// - .data: the pane-name pointer quad, then 9 leaf vtable blobs plus the
//   CBaseCur base blob, each followed by its RTTI base descriptor.
//   These are [ti, 0, virtuals...] blobs (same head shape as CfBdat's
//   8052E718 / CScrollBar's tables): the classes stay novtable with the
//   ctors storing the labels explicitly, so no TU emits a competing
//   __vt__ symbol. CCur22/CCur18 carry a trailing dtor slot; the rest do
//   not (their dtors are still defined in this TU for .text - they just
//   have no vtable slot). Leaf [0] points at the TU's own .sdata typeinfo;
//   the base blob and every descriptor point at CMenuArtsSelect's 61DD8.
// - .sdata: 9 {name, base} typeinfo pairs (CScrollBar-626F8 idiom): name ->
//   the TU's own .sdata2 class-name string, base -> the .data descriptor.
// - .sdata2: the 0.0f SetFrame constant (retail lbl_eu_80667FA4 slot) plus
//   the 9 class-name strings (0x4C, byte-exact). The 1.0f operand loads
//   CItemBoxGrid's lbl_eu_80667FA0 as extern (retail .text reloc shape), so
//   no local pool is emitted.
// The frozen CCur.o rule entry (pool 1.0f rename + .sdata2 strip) was
// retired with this change: with source-owned sections there is nothing to
// dissolve or strip (frozen check stays OK; field removal is burn-down).
// ============================================================================

// Vtable function slots defined in this TU as C++ members (same linkage
// pairing as CfScript's __dt__ decl: declared here for address-taking, the
// member definitions above carry the same mangled symbols).
extern "C" {
void initLayout__7CSubCurFv(void);
void initLayout__6CCur22Fv(void);
void initLayout__6CCur18Fv(void);
void initLayout__6CCur16Fv(void);
void initLayout__6CCur15Fv(void);
void initLayout__6CCur14Fv(void);
void initLayout__6CCur11Fv(void);
void initLayout__6CCur09Fv(void);
void initLayout__6CCur07Fv(void);
void cleanup__8CBaseCurFv(void);
void checkDeactivate__8CBaseCurFv(void);
}
// Base-class descriptor word owned by CMenuArtsSelect's .sdata (linked
// retail object defines it; incomplete array so MWCC emits no local copy).
extern u32 lbl_eu_80661DD8[];
// Forward declarations for the RTTI base descriptors (defined in retail
// order in the .data block below; complete [N] bounds match the defs).
extern void* lbl_eu_80534850[3];
extern void* lbl_eu_80534878[3];
extern void* lbl_eu_805348A0[4];
extern void* lbl_eu_805348C8[4];
extern void* lbl_eu_805348F0[4];
extern void* lbl_eu_80534918[4];
extern void* lbl_eu_80534940[4];
extern void* lbl_eu_80534968[4];
extern void* lbl_eu_80534990[4];

// --- .rodata (retail 0x438): pane names + layout/anim name pool ---
// MWCC packs these exact-sized arrays in source order (cf. CfScript rodata).
__declspec(section ".rodata") __attribute__((used))
char lbl_eu_80505DB8[12] = "nul_curs06s";
__declspec(section ".rodata") __attribute__((used))
char lbl_eu_80505DC4[12] = "nul_curs06l";
__declspec(section ".rodata") __attribute__((used))
char lbl_eu_80505DD0[12] = "nul_curs07s";
__declspec(section ".rodata") __attribute__((used))
char lbl_eu_80505DDC[12] = "nul_curs07l";
__declspec(section ".rodata") __attribute__((used))
char lbl_eu_80505DE8[0x408] =
    "mf00_reg00_curs08.brlyt\0"
    "mf00_reg00_curs08_roop.brlan\0"
    "mf00_reg00_curs08_on.brlan\0"
    "mf00_reg00_curs07.brlyt\0"
    "mf00_reg00_curs07_roop.brlan\0"
    "mf00_reg00_curs07_on.brlan\0"
    "mf00_reg00_curs09.brlyt\0"
    "mf00_reg00_curs09_roop.brlan\0"
    "pic_pgarw_r\0"
    "pic_pgarw_l\0"
    "mf00_reg00_curs11.brlyt\0"
    "mf00_reg00_curs11_roop.brlan\0"
    "mf00_reg00_curs11_on.brlan\0"
    "pic_arrow08_l00\0"
    "pic_arrow08_r00\0"
    "mf00_reg00_curs14.brlyt\0"
    "mf00_reg00_curs14_roop.brlan\0"
    "mf00_reg00_curs14_on.brlan\0"
    "mf00_reg00_curs15.brlyt\0"
    "mf00_reg00_curs15_roop.brlan\0"
    "mf00_reg00_curs15_on.brlan\0"
    "mf00_reg00_curs16.brlyt\0"
    "mf00_reg00_curs16_roop.brlan\0"
    "mf00_reg00_curs16_on.brlan\0"
    "mf00_reg00_curs17.brlyt\0"
    "mf00_reg00_curs17_roop.brlan\0"
    "mf00_reg00_curs17_on.brlan\0"
    "mf00_reg00_curs18.brlyt\0"
    "mf00_reg00_curs18_roop.brlan\0"
    "mf00_reg00_curs18_on.brlan\0"
    "mf00_reg00_curs21.brlyt\0"
    "mf00_reg00_curs21_roop.brlan\0"
    "mf00_reg00_curs21_on.brlan\0"
    "mf00_reg00_curs22.brlyt\0"
    "mf00_reg00_curs22_roop.brlan\0"
    "mf00_reg00_curs22_on.brlan\0"
    "mf01_main22_subcursor.brlyt\0"
    "mf01_main22_subcursor_roop.brlan\0"
    "mf01_main22_subcursor_on.brlan\0"
    "pic_arrow_l00\0"
    "pic_arrow_r00\0";

// --- .sdata (retail 0x48): {name, base} typeinfo pairs ---
__declspec(section ".sdata") __attribute__((used, aligned(8)))
void* lbl_eu_80662680[2] = { (void*)lbl_eu_80667FA4.s_CSubCur, lbl_eu_80534850 };
__declspec(section ".sdata") __attribute__((used))
void* lbl_eu_80662688[2] = { (void*)lbl_eu_80667FA4.s_CCur22, lbl_eu_80534878 };
__declspec(section ".sdata") __attribute__((used))
void* lbl_eu_80662690[2] = { (void*)lbl_eu_80667FA4.s_CCur18, lbl_eu_805348A0 };
__declspec(section ".sdata") __attribute__((used))
void* lbl_eu_80662698[2] = { (void*)lbl_eu_80667FA4.s_CCur16, lbl_eu_805348C8 };
__declspec(section ".sdata") __attribute__((used))
void* lbl_eu_806626A0[2] = { (void*)lbl_eu_80667FA4.s_CCur15, lbl_eu_805348F0 };
__declspec(section ".sdata") __attribute__((used))
void* lbl_eu_806626A8[2] = { (void*)lbl_eu_80667FA4.s_CCur14, lbl_eu_80534918 };
__declspec(section ".sdata") __attribute__((used))
void* lbl_eu_806626B0[2] = { (void*)lbl_eu_80667FA4.s_CCur11, lbl_eu_80534940 };
__declspec(section ".sdata") __attribute__((used))
void* lbl_eu_806626B8[2] = { (void*)lbl_eu_80667FA4.s_CCur09, lbl_eu_80534968 };
__declspec(section ".sdata") __attribute__((used))
void* lbl_eu_806626C0[2] = { (void*)lbl_eu_80667FA4.s_CCur07, lbl_eu_80534990 };
// --- .data (retail 0x190): pane quad + vtables + descriptors, retail order ---
// (defined without extern "C": MWCC does not mangle global data names, so
// this merges with the header's extern "C" declaration; combining
// __declspec(section) with an explicit linkage spec is a syntax error).
__declspec(section ".data") __attribute__((used, aligned(8)))
const char* lbl_eu_80534828[4] = {
    lbl_eu_80505DB8, lbl_eu_80505DC4, lbl_eu_80505DD0, lbl_eu_80505DDC,
};
__declspec(section ".data") __attribute__((used))
void* lbl_eu_80534838[6] = {
    lbl_eu_80662680, 0,
    (void*)&initLayout__7CSubCurFv, (void*)&cleanup__8CBaseCurFv,
    (void*)&setRootPaneTranslate__8CBaseCurFv, (void*)&checkDeactivate__8CBaseCurFv,
};
__declspec(section ".data") __attribute__((used))
void* lbl_eu_80534850[3] = { lbl_eu_80661DD8, 0, 0 };
__declspec(section ".data") __attribute__((used))
void* lbl_eu_8053485C[7] = {
    lbl_eu_80662688, 0,
    (void*)&initLayout__6CCur22Fv, (void*)&cleanup__8CBaseCurFv,
    (void*)&setRootPaneTranslate__8CBaseCurFv, (void*)&checkDeactivate__8CBaseCurFv,
    (void*)&__dt__6CCur22Fv,
};
__declspec(section ".data") __attribute__((used))
void* lbl_eu_80534878[3] = { lbl_eu_80661DD8, 0, 0 };
__declspec(section ".data") __attribute__((used))
void* lbl_eu_80534884[7] = {
    lbl_eu_80662690, 0,
    (void*)&initLayout__6CCur18Fv, (void*)&cleanup__8CBaseCurFv,
    (void*)&setRootPaneTranslate__8CBaseCurFv, (void*)&checkDeactivate__8CBaseCurFv,
    (void*)&__dt__6CCur18Fv,
};
__declspec(section ".data") __attribute__((used))
void* lbl_eu_805348A0[4] = { lbl_eu_80661DD8, 0, 0, 0 };
__declspec(section ".data") __attribute__((used))
void* lbl_eu_805348B0[6] = {
    lbl_eu_80662698, 0,
    (void*)&initLayout__6CCur16Fv, (void*)&cleanup__8CBaseCurFv,
    (void*)&setRootPaneTranslate__8CBaseCurFv, (void*)&checkDeactivate__8CBaseCurFv,
};
__declspec(section ".data") __attribute__((used))
void* lbl_eu_805348C8[4] = { lbl_eu_80661DD8, 0, 0, 0 };
__declspec(section ".data") __attribute__((used))
void* lbl_eu_805348D8[6] = {
    lbl_eu_806626A0, 0,
    (void*)&initLayout__6CCur15Fv, (void*)&cleanup__8CBaseCurFv,
    (void*)&setRootPaneTranslate__8CBaseCurFv, (void*)&checkDeactivate__8CBaseCurFv,
};
__declspec(section ".data") __attribute__((used))
void* lbl_eu_805348F0[4] = { lbl_eu_80661DD8, 0, 0, 0 };
__declspec(section ".data") __attribute__((used))
void* lbl_eu_80534900[6] = {
    lbl_eu_806626A8, 0,
    (void*)&initLayout__6CCur14Fv, (void*)&cleanup__8CBaseCurFv,
    (void*)&setRootPaneTranslate__8CBaseCurFv, (void*)&checkDeactivate__8CBaseCurFv,
};
__declspec(section ".data") __attribute__((used))
void* lbl_eu_80534918[4] = { lbl_eu_80661DD8, 0, 0, 0 };
__declspec(section ".data") __attribute__((used))
void* lbl_eu_80534928[6] = {
    lbl_eu_806626B0, 0,
    (void*)&initLayout__6CCur11Fv, (void*)&cleanup__8CBaseCurFv,
    (void*)&setRootPaneTranslate__8CBaseCurFv, (void*)&checkDeactivate__8CBaseCurFv,
};
__declspec(section ".data") __attribute__((used))
void* lbl_eu_80534940[4] = { lbl_eu_80661DD8, 0, 0, 0 };
__declspec(section ".data") __attribute__((used))
void* lbl_eu_80534950[6] = {
    lbl_eu_806626B8, 0,
    (void*)&initLayout__6CCur09Fv, (void*)&cleanup__8CBaseCurFv,
    (void*)&setRootPaneTranslate__8CBaseCurFv, (void*)&checkDeactivate__8CBaseCurFv,
};
__declspec(section ".data") __attribute__((used))
void* lbl_eu_80534968[4] = { lbl_eu_80661DD8, 0, 0, 0 };
__declspec(section ".data") __attribute__((used))
void* lbl_eu_80534978[6] = {
    lbl_eu_806626C0, 0,
    (void*)&initLayout__6CCur07Fv, (void*)&cleanup__8CBaseCurFv,
    (void*)&setRootPaneTranslate__8CBaseCurFv, (void*)&checkDeactivate__8CBaseCurFv,
};
__declspec(section ".data") __attribute__((used))
void* lbl_eu_80534990[4] = { lbl_eu_80661DD8, 0, 0, 0 };
__declspec(section ".data") __attribute__((used))
void* lbl_eu_805349A0[6] = {
    lbl_eu_80661DD8, 0, 0,
    (void*)&cleanup__8CBaseCurFv,
    (void*)&setRootPaneTranslate__8CBaseCurFv, (void*)&checkDeactivate__8CBaseCurFv,
};
