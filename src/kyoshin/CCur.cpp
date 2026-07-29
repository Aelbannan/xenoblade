// Auto-scaffolded catalog TU for kyoshin/CCur
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/code_80135FDC.hpp"

// Vtable for CCur07 (set by constructor after base ctor runs)
extern "C" int lbl_eu_80534978[];

// CCur07: cursor widget 07. Overrides the base vtable with its own.
class CCur07 : public CBaseCur {
public:
    CCur07(nw4r::lyt::ArcResourceAccessor* arcResAcc);
};

// Forward declarations for CCur classes used in stubs

/* Sets the visible flag on a CBaseCur cursor widget. */
extern "C" void func_801D216C(void* pCur, u8 val) { ((CBaseCur*)pCur)->mVisible = val; }

/* Copy a VEC3 translation into a pane's translate field.
   Used as a shared helper by cursor layout update paths. */
DECOMP_DONT_INLINE void func_801D2150(nw4r::lyt::Pane* pane, const nw4r::math::VEC3* trans) {
    pane->SetTranslate(*trans);
}

/* Forward: shared tail handler for cursor activation. */
DECOMP_DONT_INLINE void func_801D2264(CBaseCur* cur);

/* CBaseCur virtual: load root pane and forward translation.
   extern "C" is used to match the retail symbol name exactly. */
extern "C" void func_801D2144__8CBaseCurFv(CBaseCur* cur, const nw4r::math::VEC3* trans) {
    func_801D2150(cur->mpLayout->GetRootPane(), trans);
}

/* Activate cursor and hand off to the shared tail handler.
   extern "C" used to match the retail symbol name exactly. */
extern "C" void func_801D2174(CBaseCur* cur) {
    cur->mActive = 1;
    func_801D2264(cur);
}


/* CBaseCur constructor: initialises vtable, stores the arc resource
   accessor, and zeroes the remaining fields. */
CBaseCur::CBaseCur(nw4r::lyt::ArcResourceAccessor* arcResAcc) {
    mVtable = lbl_eu_805349A0;
    mArcResAcc = arcResAcc;
    mpLayout = nullptr;
    mpAnimTrans0 = nullptr;
    mpAnimTrans1 = nullptr;
    mActive = 0;
    mVisible = 0;
}

/* func_801D20B0: Draw the cursor layout if it is loaded and visible.
   Checks for null layout and hidden flag before forwarding to the
   shared nw4r layout draw helper. */
void func_801D20B0(CBaseCur* cur, nw4r::lyt::DrawInfo* drawInfo) {
    if (cur->mpLayout == 0) {
        return;
    }
    if (cur->mVisible == 0) {
        return;
    }
    func_80137038(cur->mpLayout, drawInfo, 0, 1);
}

void func_801D21CC(){}

DECOMP_DONT_INLINE void func_801D2264(CBaseCur* cur){}

/* CCur07: cursor widget 07. Overrides the base vtable with its own. */
class CCur07 : public CBaseCur {
public:
    CCur07(nw4r::lyt::ArcResourceAccessor* arcResAcc);
};

/* CCur07 constructor: chains to CBaseCur base constructor then
   overrides the vtable pointer with the CCur07 vtable. */
CCur07::CCur07(nw4r::lyt::ArcResourceAccessor* arcResAcc) : CBaseCur(arcResAcc) {
    mVtable = lbl_eu_80534978;
}

void __dt__6CCur07Fv() {}

void func_virt___dt__6CCur07Fv() {}

void __ct__CCur09(){}

void __dt__6CCur09Fv() {}

void func_virt___dt__6CCur09Fv() { }

void func_801D24E8(){}

void __ct__CCur11(){}

void __dt__6CCur11Fv() {}

void func_virt___dt__6CCur11Fv() { }

void func_801D2670(){}

void __ct__CCur14(){}

void __dt__6CCur14Fv() {}

void func_virt___dt__6CCur14Fv() { }

void __ct__CCur15(){}

void __dt__6CCur15Fv() {}

void func_virt___dt__6CCur15Fv() { }

void __ct__CCur16(){}

void __dt__6CCur16Fv() {}

void func_virt___dt__6CCur16Fv() { }

void __ct__CCur18(){}

void __dt__6CCur18Fv() {}

void func_virt___dt__6CCur18Fv() { }

void __ct__CCur22(){}

void __dt__6CCur22Fv() {}

void func_virt___dt__6CCur22Fv() { }

void func_801D2BFC(){}

void func_801D2C80(){}

void func_801D2CF4(){}

void __ct__CSubCur(){}

void __dt__6CSubCurFv() {}

void func_virt___dt__6CSubCurFv() { }

void func_801D2E4C(){}

void func_801D2ED8(){}
