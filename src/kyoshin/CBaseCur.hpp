#pragma once

/*
 * CBaseCur.hpp
 * Base class for cursor/layout overlay widgets.
 *
 * Manages a nw4r layout with animation transforms and visibility state.
 * Derived classes (CCur07..CCur22, CSubCur) each load a specific layout.
 *
 * Layout (size 0x18):
 *   +0x00: vptr
 *   +0x04: arc resource accessor (passed at construction)
 *   +0x08: nw4r layout pointer (loaded by the leaf initLayout methods)
 *   +0x0C: animation transform 0
 *   +0x10: animation transform 1
 *   +0x14: mActive - active flag (0=idle, 1=running, 2=fading)
 *   +0x15: mVisible - visibility flag (0=hidden, 1=visible)
 *
 * Virtual methods (retail vtable slots, dispatched via this->method()):
 *   +0x08: initLayout() - pure in the base table, initLayout__6CCurXXFv in leaves
 *   +0x0C: cleanup()
 *   +0x10: setRootPaneTranslate()
 *   +0x14: checkDeactivate()
 */

#include <types.h>
#include <nw4r/lyt.h>
#include <nw4r/math/math_types.h>

class CBaseCur {
public:
    CBaseCur(nw4r::lyt::ArcResourceAccessor*);
    ~CBaseCur();

    virtual void initLayout() = 0;                                  // +0x08 (0 in the base table)
    virtual void cleanup();                                         // +0x0C
    virtual void setRootPaneTranslate(const nw4r::math::VEC3* pos); // +0x10
    virtual void checkDeactivate();                                 // +0x14

    void Update();
    void DrawLayout(nw4r::lyt::DrawInfo* drawInfo);
    void SetVisible(u8 val);
    void SetActive();
    void DeactivateTail();
    void ActivateTail();

    // Writes a VEC3 into a pane's translate (shared by every layout owner in
    // the kyoshin UI code; retail keeps it as one out-of-line helper).
    static void SetPaneTranslate(nw4r::lyt::Pane*, const nw4r::math::VEC3*);

    nw4r::lyt::ArcResourceAccessor* mArcResAcc;  // +0x04
    nw4r::lyt::Layout* mpLayout;                  // +0x08
    nw4r::lyt::AnimTransform* mpAnimTrans0;       // +0x0C
    nw4r::lyt::AnimTransform* mpAnimTrans1;       // +0x10
    u8 mActive;                                   // +0x14
    u8 mVisible;                                  // +0x15

    // Legacy overlay on the implicit vptr at +0x00. The CCur classes let the
    // compiler install their own vtable; the look-alike cursors declared in
    // other TUs (CCLPCur, ...) still build their table by hand and store the
    // retail label through this accessor.
    void*& vtbl() { return *reinterpret_cast<void**>(this); }
};

class CCur07 : public CBaseCur {
public:
    CCur07(nw4r::lyt::ArcResourceAccessor* = nullptr);
    ~CCur07();
    virtual void initLayout();
};

class CCur09 : public CBaseCur {
public:
    CCur09(nw4r::lyt::ArcResourceAccessor* = nullptr);
    ~CCur09();
    virtual void initLayout();
    void SetTwoPanes(const nw4r::math::VEC3* trans0, const nw4r::math::VEC3* trans1);
};

class CCur11 : public CBaseCur {
public:
    CCur11(nw4r::lyt::ArcResourceAccessor* = nullptr);
    ~CCur11();
    virtual void initLayout();
    void ShowTwoPanes(bool visible);
};

class CCur14 : public CBaseCur {
public:
    CCur14(nw4r::lyt::ArcResourceAccessor* = nullptr);
    ~CCur14();
    virtual void initLayout();
};

class CCur15 : public CBaseCur {
public:
    CCur15(nw4r::lyt::ArcResourceAccessor* = nullptr);
    ~CCur15();
    virtual void initLayout();
};

class CCur16 : public CBaseCur {
public:
    CCur16(nw4r::lyt::ArcResourceAccessor* = nullptr);
    ~CCur16();
    virtual void initLayout();
};

class CCur18 : public CBaseCur {
public:
    CCur18(nw4r::lyt::ArcResourceAccessor* = nullptr);
    virtual void initLayout();
    virtual ~CCur18();
};

// Retail ctor/dtor of the embedded-cursor leaf. Owners that embed a CCur18 by
// value call the constructor explicitly on the member; copying a stack-built
// CCur18 into the member is the implicit operator= (copies +0x04..+0x15,
// keeps the vptr).
extern "C" CCur18* __ct__6CCur18FPQ34nw4r3lyt19ArcResourceAccessor(CCur18* _this, nw4r::lyt::ArcResourceAccessor* arcResAcc);
extern "C" void* __dt__6CCur18Fv(CCur18* _this, int flags);

class CCur22 : public CBaseCur {
public:
    virtual ~CCur22();
    CCur22(nw4r::lyt::ArcResourceAccessor* = nullptr);
    virtual void initLayout();
    void HighlightPane(u8 index);
    void PlacePaneByIndex(const nw4r::math::VEC3* trans, u8 index);
    // Retail callers pass a second flag that the body never reads (it always
    // shows the pane); kept for the call-site ABI.
    void ShowPane(u8 index, u8 visible);
};

class CSubCur : public CBaseCur {
public:
    CSubCur(nw4r::lyt::ArcResourceAccessor* = nullptr);
    ~CSubCur();
    virtual void initLayout();
    void ShowTwoSubPanes(bool visible);
    int BothSubPanesOn();
};
