#pragma once

#include <types.h>
#include "monolib/work/CTTask.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "monolib/scn/CScn.hpp"
#include "kyoshin/CTitle.hpp"

class CProcess;

// Title-screen menu task. Bases (retail vtable layout): the CTTask<CMenuTitle>
// task at +0x00, IWorkEvent at +0x54 and IScnRender at +0x58 (the compiler
// emits the `@84@` / `@88@` adjustor thunks for the secondary vtables).
class CMenuTitle : public CTTask<CMenuTitle>, public IWorkEvent, public IScnRender {
public:
    CMenuTitle(CScn* scene, CTitleAction* action);
    virtual ~CMenuTitle();

    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void cbRenderBefore();

    // Only one title menu exists at a time; returns null when one is already
    // running, otherwise the new instance (also stored in sInstance).
    static CMenuTitle* create(CProcess* parent, CScn* scene, CTitleAction* action);

    static CMenuTitle* sInstance;

private:
    // State handlers dispatched through sStateFuncs[mState] from Move().
    void stateSetReady();
    void stateOnLoadDone();
    void stateShowSub();
    void stateOnAnimDone4();
    void statePollPress();
    void stateAdvanceProgress();
    void stateHandleInput();
    void stateOnAnimDone2();
    void stateOnAnimDone9();
    void stateNoop();

    typedef void (CMenuTitle::*StateFunc)();
    static const StateFunc sStateFuncs[10];

    /* 0x5C */ CScn* mScene;
    /* 0x60 */ CTitle mTitle;
    /* 0xE8 */ u8 mState;       // index into sStateFuncs
    /* 0xE9 */ u8 mInputEnable; // draw gate / input enable
    /* 0xEA */ u8 mInputLock;   // transient lock (non-zero blocks input)
    /* 0xEC */ f32 mProgress;
}; // size: 0xF0

// CTitle controller helpers (retail C-linkage, defined in CTitle.cpp).
extern "C" int CTitle_isLoadDone(CTitle* self);
extern "C" int CTitle_isAnimDone(CTitle* self);
extern "C" int CTitle_confirmSelection(CTitle* self);
extern "C" void CTitle_beginLogoOutro(CTitle* self);
extern "C" void CTitle_beginMenuOutro(CTitle* self);
extern "C" void CTitle_moveCursorUp(CTitle* self);
extern "C" void CTitle_moveCursorDown(CTitle* self);
extern "C" void CTitle_stepLogoMenu(CTitle* self);
extern "C" void CTitle_showMenu(CTitle* self);
extern "C" void CTitle_startLoad(CTitle* self);
extern "C" void CTitle_teardown(CTitle* self);
extern "C" void CTitle_update(CTitle* self);
extern "C" void CTitle_draw(CTitle* self, void* drawInfo);
