// Cursor layout overlay widgets - CCur unit
// High-level C/C++ reconstruction from retail PPC

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "kyoshin/CCur.hpp"
#include "kyoshin/CQuestWindow_Pane.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"

// String table for cursor layout/anim resource names
// (declared in include/lbls_kyoshin.hpp; keep local extern for MWCC relocs)
extern char lbl_eu_80505DE8[];
// Retail 1.0f pool owned by CItemBoxGrid's .sdata2 tail (split
// 0x80667F30-0x80667FA4); this TU's .text loads it as an extern symbol
// (retail .text relocs), so no local pool is emitted and our .sdata2 is
// exactly the split's 0x4C.


// First word: the 0.0f the anim transforms are reset to (SetFrame(0.0f) loads
// it via sda21). It is followed immediately by the compiler-emitted RTTI class
// name strings with no padding, so the section stays at the split's 0x4C.
// The 1.0f operand loads CItemBoxGrid's 1.0f as extern (retail .text reloc
// shape), so no local 1.0f pool is emitted.

// ============================================================================
// CBaseCur constructor
// ============================================================================
CBaseCur::CBaseCur(nw4r::lyt::ArcResourceAccessor* arcResAcc) {
    mArcResAcc = arcResAcc;
    mpLayout = nullptr;
    mpAnimTrans0 = nullptr;
    mpAnimTrans1 = nullptr;
    mActive = 0;
    mVisible = 0;
}

CBaseCur::~CBaseCur() {}

// ============================================================================
// CBaseCur::Update: per-frame cursor update
// The goto/switch shape is the one that reproduces retail's branch tree
// (cmpi 0 -> zero block, cmpi 1 -> tail call, else straight to Animate);
// an if/else-if chain folds the compares differently.
// ============================================================================
void CBaseCur::Update() {
    if (mpLayout == nullptr) {
        return;
    }
    switch (mActive) {
    case 0:
        goto zero;
    case 1:
        goto one;
    default:
        goto animate;
    }
zero:
    advanceAnimTransform(mpAnimTrans0, 1.0f);
    goto animate;
one:
    checkDeactivate();
animate:
    mpLayout->Animate(0);
}

// ============================================================================
// CBaseCur::DrawLayout: draw cursor layout
// ============================================================================
void CBaseCur::DrawLayout(nw4r::lyt::DrawInfo* drawInfo) {
    if (mpLayout == nullptr) return;
    if (mVisible == 0) return;
    drawLayout(mpLayout, drawInfo, 0, 1);
}

// ============================================================================
// CBaseCur::cleanup: deinitialize cursor
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
// CBaseCur::setRootPaneTranslate: forward translation to root pane
// ============================================================================
void CBaseCur::setRootPaneTranslate(const nw4r::math::VEC3* trans) {
    SetPaneTranslate(mpLayout->GetRootPane(), trans);
}

// ============================================================================
// CBaseCur::SetPaneTranslate: set pane translate from VEC3
// ============================================================================
void CBaseCur::SetPaneTranslate(nw4r::lyt::Pane* pane, const nw4r::math::VEC3* trans) {
    pane->SetTranslate(*trans);
}

// ============================================================================
// CBaseCur::SetVisible: set cursor visibility
// ============================================================================
void CBaseCur::SetVisible(u8 val) {
    mVisible = val;
}

// ============================================================================
// CBaseCur::SetActive: activate cursor
// ============================================================================
void CBaseCur::SetActive() {
    mActive = 1;
    ActivateTail();
}

// ============================================================================
// CBaseCur::checkDeactivate: check anim finish and deactivate
// ============================================================================
void CBaseCur::checkDeactivate() {
    if (advanceAnimTransform(mpAnimTrans1, 1.0f) != 0) {
        mActive = 0;
        DeactivateTail();
    }
}

// ============================================================================
// CBaseCur::DeactivateTail: deactivation tail handler
// Unbinds the active anim transform, re-binds animTrans0, enables and resets
// its frame, then re-animates. Retail reloads mpLayout before every call.
// ============================================================================
void CBaseCur::DeactivateTail() {
    mpLayout->UnbindAnimation(mpAnimTrans1);
    mpLayout->BindAnimation(mpAnimTrans0);
    mpLayout->SetAnimationEnable(mpAnimTrans0, true);
    mpAnimTrans0->SetFrame(0.0f);
    mpLayout->Animate(0);
}

// ============================================================================
// CBaseCur::ActivateTail: activation tail handler
// Unbinds the active anim transform, re-binds animTrans1, enables and resets
// its frame, then re-animates. Retail reloads mpLayout before every call.
// ============================================================================
void CBaseCur::ActivateTail() {
    mpLayout->UnbindAnimation(mpAnimTrans0);
    mpLayout->BindAnimation(mpAnimTrans1);
    mpLayout->SetAnimationEnable(mpAnimTrans1, true);
    mpAnimTrans1->SetFrame(0.0f);
    mpLayout->Animate(0);
}

// ============================================================================
// CCur07 constructor
// ============================================================================
CCur07::CCur07(nw4r::lyt::ArcResourceAccessor* arcResAcc) : CBaseCur(arcResAcc) {}

// ============================================================================
// CCur07 destructor
// ============================================================================
CCur07::~CCur07() {}

// ============================================================================
// CCur07::initLayout: load layout curs07 and two anim transforms
// ============================================================================
void CCur07::initLayout() {
    buildLayout(&mpLayout, mArcResAcc, lbl_eu_80505DE8 + 0x50);
    bindLayoutAnimTransform(mpLayout, &mpAnimTrans0, mArcResAcc, lbl_eu_80505DE8 + 0x68);
    bindLayoutAnimTransform(mpLayout, &mpAnimTrans1, mArcResAcc, lbl_eu_80505DE8 + 0x85);
    mpLayout->UnbindAllAnimation();
    DeactivateTail();
}

// ============================================================================
// CCur09 constructor
// ============================================================================
CCur09::CCur09(nw4r::lyt::ArcResourceAccessor* arcResAcc) : CBaseCur(arcResAcc) {}

// ============================================================================
// CCur09 destructor
// ============================================================================
CCur09::~CCur09() {}

// ============================================================================
// CCur09::initLayout: load layout curs09 and one anim transform
// ============================================================================
void CCur09::initLayout() {
    buildLayout(&mpLayout, mArcResAcc, lbl_eu_80505DE8 + 0xa0);
    bindLayoutAnimTransform(mpLayout, &mpAnimTrans0, mArcResAcc, lbl_eu_80505DE8 + 0xb8);
    mpLayout->UnbindAllAnimation();
    DeactivateTail();
}

// ============================================================================
// CCur09::SetTwoPanes: set two pane positions on the curs09 cursor
// Uses named panes from the root pane's children.
// ============================================================================
// Retail reloads mpLayout/root pane for each lookup (no caching) - caching
// the root pane changes MWCC's register allocation away from stmw r28-r31.
void CCur09::SetTwoPanes(const nw4r::math::VEC3* trans0, const nw4r::math::VEC3* trans1) {
    SetPaneTranslate(mpLayout->GetRootPane()->FindPaneByName(lbl_eu_80505DE8 + 0xd5, true), trans0);
    SetPaneTranslate(mpLayout->GetRootPane()->FindPaneByName(lbl_eu_80505DE8 + 0xe1, true), trans1);
}

// ============================================================================
// CCur11 constructor
// ============================================================================
CCur11::CCur11(nw4r::lyt::ArcResourceAccessor* arcResAcc) : CBaseCur(arcResAcc) {}

// ============================================================================
// CCur11 destructor
// ============================================================================
CCur11::~CCur11() {}

// ============================================================================
// CCur11::initLayout: load layout curs11 and two anim transforms
// ============================================================================
void CCur11::initLayout() {
    buildLayout(&mpLayout, mArcResAcc, lbl_eu_80505DE8 + 0xed);
    bindLayoutAnimTransform(mpLayout, &mpAnimTrans0, mArcResAcc, lbl_eu_80505DE8 + 0x105);
    bindLayoutAnimTransform(mpLayout, &mpAnimTrans1, mArcResAcc, lbl_eu_80505DE8 + 0x122);
    mpLayout->UnbindAllAnimation();
    DeactivateTail();
}

// ============================================================================
// CCur11::ShowTwoPanes: set visibility of two named panes
// ============================================================================
// Same no-caching shape as CCur09::SetTwoPanes (retail reloads root pane twice).
void CCur11::ShowTwoPanes(bool visible) {
    if (mpLayout == nullptr) return;
    setPaneVisible(mpLayout->GetRootPane()->FindPaneByName(lbl_eu_80505DE8 + 0x13d, true), visible);
    setPaneVisible(mpLayout->GetRootPane()->FindPaneByName(lbl_eu_80505DE8 + 0x14d, true), visible);
}

// ============================================================================
// CCur14 constructor
// ============================================================================
CCur14::CCur14(nw4r::lyt::ArcResourceAccessor* arcResAcc) : CBaseCur(arcResAcc) {}

// ============================================================================
// CCur14 destructor
// ============================================================================
CCur14::~CCur14() {}

// ============================================================================
// CCur14::initLayout: load layout curs14 and two anim transforms
// ============================================================================
void CCur14::initLayout() {
    buildLayout(&mpLayout, mArcResAcc, lbl_eu_80505DE8 + 0x15d);
    bindLayoutAnimTransform(mpLayout, &mpAnimTrans0, mArcResAcc, lbl_eu_80505DE8 + 0x175);
    bindLayoutAnimTransform(mpLayout, &mpAnimTrans1, mArcResAcc, lbl_eu_80505DE8 + 0x192);
    mpLayout->UnbindAllAnimation();
    DeactivateTail();
}

// ============================================================================
// CCur15 constructor
// ============================================================================
CCur15::CCur15(nw4r::lyt::ArcResourceAccessor* arcResAcc) : CBaseCur(arcResAcc) {}

// ============================================================================
// CCur15 destructor
// ============================================================================
CCur15::~CCur15() {}

// ============================================================================
// CCur15::initLayout: load layout curs15 and two anim transforms
// ============================================================================
void CCur15::initLayout() {
    buildLayout(&mpLayout, mArcResAcc, lbl_eu_80505DE8 + 0x1ad);
    bindLayoutAnimTransform(mpLayout, &mpAnimTrans0, mArcResAcc, lbl_eu_80505DE8 + 0x1c5);
    bindLayoutAnimTransform(mpLayout, &mpAnimTrans1, mArcResAcc, lbl_eu_80505DE8 + 0x1e2);
    mpLayout->UnbindAllAnimation();
    DeactivateTail();
}

// ============================================================================
// CCur16 constructor
// ============================================================================
CCur16::CCur16(nw4r::lyt::ArcResourceAccessor* arcResAcc) : CBaseCur(arcResAcc) {}

// ============================================================================
// CCur16 destructor
// ============================================================================
CCur16::~CCur16() {}

// ============================================================================
// CCur16::initLayout: load layout curs16 and two anim transforms
// ============================================================================
void CCur16::initLayout() {
    buildLayout(&mpLayout, mArcResAcc, lbl_eu_80505DE8 + 0x1fd);
    bindLayoutAnimTransform(mpLayout, &mpAnimTrans0, mArcResAcc, lbl_eu_80505DE8 + 0x215);
    bindLayoutAnimTransform(mpLayout, &mpAnimTrans1, mArcResAcc, lbl_eu_80505DE8 + 0x232);
    mpLayout->UnbindAllAnimation();
    DeactivateTail();
}

// ============================================================================
// CCur18 constructor
// ============================================================================
CCur18::CCur18(nw4r::lyt::ArcResourceAccessor* arcResAcc) : CBaseCur(arcResAcc) {}

// ============================================================================
// CCur18 destructor
// ============================================================================
CCur18::~CCur18() {}

// ============================================================================
// CCur18::initLayout: load layout curs18 and two anim transforms
// ============================================================================
void CCur18::initLayout() {
    buildLayout(&mpLayout, mArcResAcc, lbl_eu_80505DE8 + 0x29d);
    bindLayoutAnimTransform(mpLayout, &mpAnimTrans0, mArcResAcc, lbl_eu_80505DE8 + 0x2b5);
    bindLayoutAnimTransform(mpLayout, &mpAnimTrans1, mArcResAcc, lbl_eu_80505DE8 + 0x2d2);
    mpLayout->UnbindAllAnimation();
    DeactivateTail();
}

// ============================================================================
// CCur22 constructor
// ============================================================================
CCur22::CCur22(nw4r::lyt::ArcResourceAccessor* arcResAcc) : CBaseCur(arcResAcc) {}

// ============================================================================
// CCur22 destructor
// ============================================================================
CCur22::~CCur22() {}

// ============================================================================
// CCur22::initLayout: load layout curs22 and two anim transforms
// ============================================================================
void CCur22::initLayout() {
    buildLayout(&mpLayout, mArcResAcc, lbl_eu_80505DE8 + 0x33d);
    bindLayoutAnimTransform(mpLayout, &mpAnimTrans0, mArcResAcc, lbl_eu_80505DE8 + 0x355);
    bindLayoutAnimTransform(mpLayout, &mpAnimTrans1, mArcResAcc, lbl_eu_80505DE8 + 0x372);
    mpLayout->UnbindAllAnimation();
    DeactivateTail();
}

// ============================================================================
// CCur22::HighlightPane: show the pane at `index` and hide the other three
// ============================================================================
void CCur22::HighlightPane(u8 index) {
    for (u8 i = 0; i < 4; i++) {
        // Branch form: retail emits li 0 / cmpl / bne / li 1, not select-arithmetic
        bool visible = false;
        if (i == index) {
            visible = true;
        }
        nw4r::lyt::Pane* pane = mpLayout->GetRootPane()->FindPaneByName(lbl_eu_80534828[i], true);
        setPaneVisible(pane, visible);
    }
}

// ============================================================================
// CCur22::PlacePaneByIndex: position a cursor pane by index
// Index 4 targets the root pane directly; indices 0-3 look up named sub-panes.
// ============================================================================
void CCur22::PlacePaneByIndex(const nw4r::math::VEC3* trans, u8 index) {
    if (index == 4) {
        SetPaneTranslate(mpLayout->GetRootPane(), trans);
    } else {
        nw4r::lyt::Pane* pane = mpLayout->GetRootPane()->FindPaneByName(lbl_eu_80534828[index], true);
        SetPaneTranslate(pane, trans);
    }
}

// ============================================================================
// CCur22::ShowPane: show pane by index (0-3 only)
// Retail takes a second flag from every caller but the body always shows the
// pane unconditionally (the emitted code stores a constant 1).
// ============================================================================
void CCur22::ShowPane(u8 index, u8) {
    if (index >= 4) return;
    nw4r::lyt::Pane* pane = mpLayout->GetRootPane()->FindPaneByName(lbl_eu_80534828[index], true);
    setPaneVisible(pane, 1);
}

// ============================================================================
// CSubCur constructor
// ============================================================================
CSubCur::CSubCur(nw4r::lyt::ArcResourceAccessor* arcResAcc) : CBaseCur(arcResAcc) {}

// ============================================================================
// CSubCur destructor
// ============================================================================
CSubCur::~CSubCur() {}

// ============================================================================
// CSubCur::initLayout: load layout subcur and two anim transforms
// ============================================================================
void CSubCur::initLayout() {
    buildLayout(&mpLayout, mArcResAcc, lbl_eu_80505DE8 + 0x38d);
    bindLayoutAnimTransform(mpLayout, &mpAnimTrans0, mArcResAcc, lbl_eu_80505DE8 + 0x3a9);
    bindLayoutAnimTransform(mpLayout, &mpAnimTrans1, mArcResAcc, lbl_eu_80505DE8 + 0x3ca);
    mpLayout->UnbindAllAnimation();
    DeactivateTail();
}

// ============================================================================
// CSubCur::ShowTwoSubPanes: set visibility of two named panes (subcur variant)
// Retail reloads mpLayout + root pane for the second lookup (no caching).
// ============================================================================
void CSubCur::ShowTwoSubPanes(bool visible) {
    if (mpLayout == nullptr) return;
    setPaneVisible(mpLayout->GetRootPane()->FindPaneByName(lbl_eu_80505DE8 + 0x3e9, true), visible);
    setPaneVisible(mpLayout->GetRootPane()->FindPaneByName(lbl_eu_80505DE8 + 0x3f7, true), visible);
}

// ============================================================================
// CSubCur::BothSubPanesOn: check if both subcur panes are visible/active
// Returns 1 if both named panes return non-zero from isPaneVisible, 0 otherwise.
// ============================================================================
int CSubCur::BothSubPanesOn() {
    if (mpLayout == nullptr) return 0;
    u32 result0 = isPaneVisible(mpLayout->GetRootPane()->FindPaneByName(lbl_eu_80505DE8 + 0x3e9, true));
    u32 result1 = isPaneVisible(mpLayout->GetRootPane()->FindPaneByName(lbl_eu_80505DE8 + 0x3f7, true));
    return (result0 != 0 && result1 != 0) ? 1 : 0;
}

// ============================================================================
// Retail data sections (byte-faithful).
//
// The retail unit owns .rodata (0x438) / .data (0x190) / .sdata (0x48) /
// .sdata2 (0x4C). The vtables and RTTI descriptors are now emitted by the
// compiler from the CBaseCur/CCurNN/CCur22/CSubCur class declarations (one
// [RTTI, offset-to-top, virtuals...] block per class, in declaration order,
// which is the retail order), so this TU only spells out the plain data:
//
// - .rodata: 4 pane-name strings (the const char* array's targets) followed
//   by the 0x408 layout/anim/pane name pool the code indexes as
//   lbl_eu_80505DE8+N (every +N in initLayout/Show Pane sites lands on a
//   string start; verified against the retail bytes).
// - .data: the pane-name pointer quad first, then the compiler-emitted
//   vtables + RTTI base lists (CCur22/CCur18 carry the trailing dtor slot;
//   the base table's initLayout slot is 0).
// - .sdata: the compiler-emitted {name, base-list} RTTI pairs.
// - .sdata2: the 0.0f SetFrame constant (retail 0.0f slot) plus the class-name
//   strings (0x4C, byte-exact). The 1.0f operand loads CItemBoxGrid's 1.0f as
//   extern (retail .text reloc shape), so no local pool is emitted.
// ============================================================================

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

// --- .data (retail 0x190): pane quad + vtables + descriptors, retail order ---
// (defined without extern "C": MWCC does not mangle global data names, so
// this merges with the header's extern "C" declaration; combining
// __declspec(section) with an explicit linkage spec is a syntax error).
__declspec(section ".data") __attribute__((used, aligned(8)))
const char* lbl_eu_80534828[4] = {
    lbl_eu_80505DB8, lbl_eu_80505DC4, lbl_eu_80505DD0, lbl_eu_80505DDC,
};
