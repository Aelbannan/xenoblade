#pragma once

#include <types.h>

#include "monolib/work/CProcess.hpp"
#include "monolib/scn/IScnRender.hpp"

class CEventFile;

// Minimal CScn declaration local to this TU: only the render-callback members
// used by Init are needed. Declared with the real class tag (CScn) and
// parameter list so member calls emit the retail mangled symbol
// addRenderCB__4CScnFP10IScnRenderUlUl.
class CScn {
public:
    void addRenderCB(IScnRender* cb, u32 prio, u32 flag);
    void removeRenderCB(IScnRender* cb);
};

// CProcess base constructor imported from another TU (C-ABI, retail C-linkage
// symbol name - do not let C++ mangle its parameter list).
extern "C" void __ct__8CProcessFv(CProcess* self);

// null pointer-to-member-function constant (3 words).
extern u32 __ptmf_null[3];

// Generic task wrapper.
//
// Local copy (instead of monolib/work/CTTask.hpp) so that the Move/Draw/dtor
// methods are emitted out-of-line to match retail (the inline header versions
// would mark them inline and bloat the vtable / split budget).
template <typename T>
class CTTask : public CProcess {
public:
    typedef void (CProcess::*MoveFunc)();
    typedef void (CProcess::*DrawFunc)();

    CTTask();
    virtual ~CTTask();
    virtual void Move();
    virtual void Draw();

protected:
    MoveFunc mMoveFunc; // 0x3C - pointer-to-member-function (12 bytes)
    DrawFunc mDrawFunc; // 0x48
}; // size: 0x54

class CTaskGamePic : public CTTask<CTaskGamePic> {
public:
    CTaskGamePic();
    virtual ~CTaskGamePic();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();
    void create();
    void Draw();
    void OnFileEvent();

    // 0x0-0x54: CTTask<CTaskGamePic>
    u32 field_54;          // 0x54
    IScnRender mRenderCB;  // 0x58 render callback subobject
    CScn* mScene;          // 0x5C
    u32 mFlags;            // 0x60
}; // size: 0x64