#pragma once

/*
 * CBaseCur.hpp
 * Base class for cursor/layout overlay widgets.
 *
 * Manages a nw4r layout with animation transforms and visibility state.
 * Derived classes (CCur07..CCur22, CSubCur) each load a specific layout.
 *
 * Layout (size 0x16):
 *   +0x00: vtable pointer (set explicitly by constructor / derived classes)
 *   +0x04: arc resource accessor (passed at construction)
 *   +0x08: nw4r layout pointer (loaded by derived Init methods)
 *   +0x0C: animation transform 0
 *   +0x10: animation transform 1
 *   +0x14: mActive - active flag (0=idle, 1=running, 2=fading)
 *   +0x15: mVisible - visibility flag (0=hidden, 1=visible)
 *
 * Virtual methods (retail vtable slots, dispatched via this->method()):
 *   +0x08: initLayout() - 0 in the base table, initLayout__6CCurXXFv in leaves
 *   +0x0C: cleanup()
 *   +0x10: setRootPaneTranslate()
 *   +0x14: checkDeactivate()
 */

#include <types.h>
#include <nw4r/lyt.h>
#include <nw4r/math/math_types.h>

// Vtable symbol for CBaseCur (6 entries, .data)
extern "C" void* lbl_eu_805349A0[];

// Retail polymorphic cursor base (CCur.s tables: base lbl_eu_805349A0 plus
// leaf tables initLayout__6CCurXXFv at +0x08; cleanup, setRootPaneTranslate
// and checkDeactivate at +0x0C/+0x10/+0x14; base slot +0x08 is 0).
// __declspec(novtable): the tables live in the data blob and the ctors store
// the labels manually (same shape as cf::CHelp), so no TU emits a vtable.
// The retail method symbols keep their Fv names (setRootPaneTranslate reads
// its VEC3 from r4 despite the Fv suffix); the matching member definitions live in CCur.cpp and only the dispatch goes through these
// virtuals. initLayout is declared without a body: the base slot is 0 and
// novtable emits no reference to the missing symbol; virtual calls land on
// the leaf tables in the blob.
class CBaseCur {
public:
    CBaseCur(nw4r::lyt::ArcResourceAccessor*);
    ~CBaseCur();
    virtual void initLayout() = 0;   // +0x08 (0 in the base table)
    virtual void cleanup();      // +0x0C
    virtual void setRootPaneTranslate(const nw4r::math::VEC3* pos); // +0x10
    virtual void checkDeactivate(); // +0x14

    nw4r::lyt::ArcResourceAccessor* mArcResAcc;  // +0x04
    nw4r::lyt::Layout* mpLayout;                  // +0x08
    nw4r::lyt::AnimTransform* mpAnimTrans0;       // +0x0C
    nw4r::lyt::AnimTransform* mpAnimTrans1;       // +0x10
    u8 mActive;                                    // +0x14
    u8 mVisible;                                   // +0x15

    // Overlay on the implicit vptr at +0x00 so the free-function ctors can
    // store the retail table labels (same idiom as cf::CHelp::vtbl()).
    void*& vtbl() { return *reinterpret_cast<void**>(this); }
};

class CCur07 : public CBaseCur {
public:
    CCur07(nw4r::lyt::ArcResourceAccessor* = nullptr);
    virtual void initLayout();
};

class CCur09 : public CBaseCur {
public:
    CCur09(nw4r::lyt::ArcResourceAccessor* = nullptr);
    virtual void initLayout();
};

class CCur11 : public CBaseCur {
public:
    CCur11(nw4r::lyt::ArcResourceAccessor* = nullptr);
    virtual void initLayout();
};

class CCur14 : public CBaseCur {
public:
    CCur14(nw4r::lyt::ArcResourceAccessor* = nullptr);
    virtual void initLayout();
};

class CCur15 : public CBaseCur {
public:
    CCur15(nw4r::lyt::ArcResourceAccessor* = nullptr);
    virtual void initLayout();
};

class CCur16 : public CBaseCur {
public:
    CCur16(nw4r::lyt::ArcResourceAccessor* = nullptr);
    virtual void initLayout();
};

// CCUR18_MEMBER_DTOR: define before including this header in TUs whose owners
// rely on MWCC's implicit member destruction of an embedded CCur18 (e.g.
// UI::CPassiveSkillLine). Elsewhere owners call __dt__6CCur18Fv explicitly, and
// a declared dtor would add scope-exit calls for their stack temporaries.
class CCur18 : public CBaseCur {
public:
    CCur18(nw4r::lyt::ArcResourceAccessor* = nullptr);
    virtual void initLayout();
    virtual ~CCur18();
};

// Retail ctor/dtor of the embedded-cursor leaf (stripped short ctor name
// __ct__CCur18 plus the mangled dtor), defined in CCur.cpp as free functions
// that store the vtable label by hand. Owners embed a CCur18 by value and
// call these explicitly; copying a stack-constructed CCur18 into the member
// is the implicit operator= (copies +0x04..+0x15, keeps the vptr).
extern "C" CCur18* __ct__CCur18(CCur18* _this, nw4r::lyt::ArcResourceAccessor* arcResAcc);
extern "C" void* __dt__6CCur18Fv(CCur18* _this, int flags);

class CCur22 : public CBaseCur {
public:
    virtual ~CCur22();
    CCur22(nw4r::lyt::ArcResourceAccessor* = nullptr);
    virtual void initLayout();
};

class CSubCur : public CBaseCur {
public:
    CSubCur(nw4r::lyt::ArcResourceAccessor* = nullptr);
    ~CSubCur();
    virtual void initLayout();
};
