// Auto-scaffolded catalog TU for kyoshin/CCur
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/code_80135FDC.hpp"

// Shared helper: set pane visibility (extern, defined in code_80135FDC)
extern void func_80124270(nw4r::lyt::Pane*, u32);

// Vtable for CCur07 (set by constructor after base ctor runs)
extern "C" int lbl_eu_80534978[];

// Vtable symbols for CCur09, CCur11, CCur14 constructors
extern "C" void* lbl_eu_80534950[];
extern "C" void* lbl_eu_80534928[];
extern "C" void* lbl_eu_80534900[];

// Vtable symbols for CCur15, CCur16, CCur18 constructors
extern "C" void* lbl_eu_805348D8[];
extern "C" void* lbl_eu_805348B0[];
extern "C" void* lbl_eu_80534884[];

// Vtable symbols for CCur22 and CSubCur constructors
extern "C" void* lbl_eu_8053485C[];
extern "C" void* lbl_eu_80534838[];

extern "C" void __dl__FPv(void*);

// Forward declarations for CCur classes used in stubs

/* Sets the visible flag on a CBaseCur cursor widget. */
extern "C" void func_801D216C(void* pCur, u8 val) { ((CBaseCur*)pCur)->mVisible = val; }

/* Copy a VEC3 translation into a pane's translate field.
   Used as a shared helper by cursor layout update paths. */
DECOMP_DONT_INLINE void func_801D2150(nw4r::lyt::Pane* pane, const nw4r::math::VEC3* trans) {
    pane->SetTranslate(*trans);
}

/* Forward: shared tail handler for cursor deactivation (after mActive→0). */
void func_801D21CC(CBaseCur* cur);

/* Forward: shared tail handler for cursor activation. */
DECOMP_DONT_INLINE void func_801D2264(CBaseCur* cur);

/* CBaseCur virtual: deinitialize the cursor by destroying the layout
   and clearing all animation transforms and visibility. */
extern "C" void func_801D20DC__8CBaseCurFv(CBaseCur* cur) {
    cur->mVisible = 0;
    cur->mpAnimTrans0 = NULL;
    cur->mpAnimTrans1 = NULL;
    if (cur->mpLayout != NULL) {
        delete cur->mpLayout;
        cur->mpLayout = NULL;
    }
}

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

/* CBaseCur virtual: check if anim transform 1 has finished;
   if so, deactivate the cursor and run the cleanup tail handler. */
extern "C" void func_801D2180__8CBaseCurFv(CBaseCur* cur) {
    if (func_80137444(cur->mpAnimTrans1, 1.0f) != 0) {
        cur->mActive = 0;
        func_801D21CC(cur);
    }
}

/* CBaseCur default constructor: initialises vtable to the base vtable,
   stores the arc resource accessor passed in r4, and zeroes the
   remaining fields. */
extern "C" DECOMP_DONT_INLINE void __ct__8CBaseCurFv(CBaseCur* _this, nw4r::lyt::ArcResourceAccessor* arcResAcc) {
    _this->mVtable = lbl_eu_805349A0;
    _this->mArcResAcc = arcResAcc;
    _this->mpLayout = nullptr;
    _this->mpAnimTrans0 = nullptr;
    _this->mpAnimTrans1 = nullptr;
    _this->mActive = 0;
    _this->mVisible = 0;
}

/* CBaseCur destructor: frees the object if the flags parameter is > 0.
   Standard MWCC complete-object-destructor pattern. */
extern "C" void* __dt__8CBaseCurFv(CBaseCur* _this, int flags) {
    if (_this != 0 && flags > 0) {
        __dl__FPv(_this);
    }
    return _this;
}

/* func_801D20B0: Draw the cursor layout if it is loaded and visible.
   Checks for null layout and hidden flag before forwarding to the
   shared nw4r layout draw helper. */
extern "C" void func_801D20B0(CBaseCur* cur, nw4r::lyt::DrawInfo* drawInfo) {
    if (cur->mpLayout == 0) {
        return;
    }
    if (cur->mVisible == 0) {
        return;
    }
    func_80137038(cur->mpLayout, drawInfo, 0, 1);
}

/* Cleanup: unbind all animations from the layout, reset anim transform frame,
   and re-animate. Called after cursor deactivation (mActive → 0). */
void func_801D21CC(CBaseCur* cur){}

DECOMP_DONT_INLINE void func_801D2264(CBaseCur* cur){}

/* CCur07 constructor: chains to CBaseCur base constructor then
   overrides the vtable pointer with the CCur07 vtable. */
extern "C" CBaseCur* __ct__CCur07(CBaseCur* _this, nw4r::lyt::ArcResourceAccessor* arcResAcc) {
    __ct__8CBaseCurFv(_this, arcResAcc);
    _this->mVtable = lbl_eu_80534978;
    return _this;
}

/* CCur07 destructor: frees the object if flags > 0.
   Standard MWCC complete-object-destructor pattern. */
extern "C" void* __dt__6CCur07Fv(CBaseCur* _this, int flags) {
    if (_this != 0 && flags > 0) {
        __dl__FPv(_this);
    }
    return _this;
}

void func_virt___dt__6CCur07Fv() {}

/* CCur09 constructor: chains to CBaseCur then sets the CCur09 vtable. */
extern "C" CBaseCur* __ct__CCur09(CBaseCur* _this, nw4r::lyt::ArcResourceAccessor* arcResAcc) {
    __ct__8CBaseCurFv(_this, arcResAcc);
    _this->mVtable = lbl_eu_80534950;
    return _this;
}

/* CCur09 destructor: frees the object if flags > 0.
   Standard MWCC complete-object-destructor pattern. */
extern "C" void* __dt__6CCur09Fv(CBaseCur* _this, int flags) {
    if (_this != 0 && flags > 0) {
        __dl__FPv(_this);
    }
    return _this;
}

void func_virt___dt__6CCur09Fv() { }

void func_801D24E8(){}

/* CCur11 constructor: chains to CBaseCur then sets the CCur11 vtable. */
extern "C" CBaseCur* __ct__CCur11(CBaseCur* _this, nw4r::lyt::ArcResourceAccessor* arcResAcc) {
    __ct__8CBaseCurFv(_this, arcResAcc);
    _this->mVtable = lbl_eu_80534928;
    return _this;
}

/* CCur11 destructor: frees the object if flags > 0.
   Standard MWCC complete-object-destructor pattern. */
extern "C" void* __dt__6CCur11Fv(CBaseCur* _this, int flags) {
    if (_this != 0 && flags > 0) {
        __dl__FPv(_this);
    }
    return _this;
}

void func_virt___dt__6CCur11Fv() { }

void func_801D2670(){}

/* CCur14 constructor: chains to CBaseCur then sets the CCur14 vtable. */
extern "C" CBaseCur* __ct__CCur14(CBaseCur* _this, nw4r::lyt::ArcResourceAccessor* arcResAcc) {
    __ct__8CBaseCurFv(_this, arcResAcc);
    _this->mVtable = lbl_eu_80534900;
    return _this;
}

/* CCur14 destructor: frees the object if flags > 0.
   Standard MWCC complete-object-destructor pattern. */
extern "C" void* __dt__6CCur14Fv(CBaseCur* _this, int flags) {
    if (_this != 0 && flags > 0) {
        __dl__FPv(_this);
    }
    return _this;
}

void func_virt___dt__6CCur14Fv() { }

/* CCur15 constructor: chains to CBaseCur then sets the CCur15 vtable. */
extern "C" CBaseCur* __ct__CCur15(CBaseCur* _this, nw4r::lyt::ArcResourceAccessor* arcResAcc) {
    __ct__8CBaseCurFv(_this, arcResAcc);
    _this->mVtable = lbl_eu_805348D8;
    return _this;
}

/* CCur15 destructor: frees the object if flags > 0.
   Standard MWCC complete-object-destructor pattern. */
extern "C" void* __dt__6CCur15Fv(CBaseCur* _this, int flags) {
    if (_this != 0 && flags > 0) {
        __dl__FPv(_this);
    }
    return _this;
}

void func_virt___dt__6CCur15Fv() { }

/* CCur16 constructor: chains to CBaseCur then sets the CCur16 vtable. */
extern "C" CBaseCur* __ct__CCur16(CBaseCur* _this, nw4r::lyt::ArcResourceAccessor* arcResAcc) {
    __ct__8CBaseCurFv(_this, arcResAcc);
    _this->mVtable = lbl_eu_805348B0;
    return _this;
}

/* CCur16 destructor: frees the object if flags > 0.
   Standard MWCC complete-object-destructor pattern. */
extern "C" void* __dt__6CCur16Fv(CBaseCur* _this, int flags) {
    if (_this != 0 && flags > 0) {
        __dl__FPv(_this);
    }
    return _this;
}

void func_virt___dt__6CCur16Fv() { }

/* CCur18 constructor: chains to CBaseCur then sets the CCur18 vtable. */
extern "C" CBaseCur* __ct__CCur18(CBaseCur* _this, nw4r::lyt::ArcResourceAccessor* arcResAcc) {
    __ct__8CBaseCurFv(_this, arcResAcc);
    _this->mVtable = lbl_eu_80534884;
    return _this;
}

/* CCur18 destructor: frees the object if flags > 0.
   Standard MWCC complete-object-destructor pattern. */
extern "C" void* __dt__6CCur18Fv(CBaseCur* _this, int flags) {
    if (_this != 0 && flags > 0) {
        __dl__FPv(_this);
    }
    return _this;
}

void func_virt___dt__6CCur18Fv() { }

/* CCur22 constructor: chains to CBaseCur then sets the CCur22 vtable. */
extern "C" CBaseCur* __ct__CCur22(CBaseCur* _this, nw4r::lyt::ArcResourceAccessor* arcResAcc) {
    __ct__8CBaseCurFv(_this, arcResAcc);
    _this->mVtable = lbl_eu_8053485C;
    return _this;
}

/* CCur22 destructor: frees the object if flags > 0.
   Standard MWCC complete-object-destructor pattern. */
extern "C" void* __dt__6CCur22Fv(CBaseCur* _this, int flags) {
    if (_this != 0 && flags > 0) {
        __dl__FPv(_this);
    }
    return _this;
}

void func_virt___dt__6CCur22Fv() { }

void func_801D2BFC(){}

void func_801D2C80(){}

/* Show or hide named sub-panes (nul_curs06s/l, nul_curs07s/l)
   by index. Used for cursor visibility control per menu page. */
static const char* sPaneNames[] = {
    "nul_curs06s",
    "nul_curs06l",
    "nul_curs07s",
    "nul_curs07l",
};

extern "C" void func_801D2CF4(CBaseCur* cur, u8 index) {
    if (index >= 4) {
        return;
    }
    nw4r::lyt::Pane* pane = cur->mpLayout->GetRootPane()->FindPaneByName(sPaneNames[index], true);
    func_80124270(pane, 1);
}

/* CSubCur constructor: chains to CBaseCur then sets the CSubCur vtable. */
extern "C" CBaseCur* __ct__CSubCur(CBaseCur* _this, nw4r::lyt::ArcResourceAccessor* arcResAcc) {
    __ct__8CBaseCurFv(_this, arcResAcc);
    _this->mVtable = lbl_eu_80534838;
    return _this;
}

/* CSubCur destructor: frees the object if flags > 0.
   Standard MWCC complete-object-destructor pattern. */
extern "C" void* __dt__7CSubCurFv(CBaseCur* _this, int flags) {
    if (_this != 0 && flags > 0) {
        __dl__FPv(_this);
    }
    return _this;
}

void func_virt___dt__7CSubCurFv() { }

void func_801D2E4C(){}

void func_801D2ED8(){}
