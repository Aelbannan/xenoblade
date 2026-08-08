#pragma once

#include <types.h>

#include "monolib/work/CProcess.hpp"
#include "monolib/scn/IScnRender.hpp"

class CScn;
class CEventFile;

// CProcess base constructor imported from another TU (C-ABI, retail C-linkage
// symbol name - do not let C++ mangle its parameter list).
extern "C" void __ct__8CProcessFv(CProcess* self);

// Retail vtable data / PTMF constant (imports; defined in .data/.rodata splits).
// lbl_eu_80538C00 is the CTaskGameEvt vtable region; the secondary sub-object
// vtables (field_54 at +0x24, the IScnRender member at +0xAC) live inside the
// same block, so the constructor references them as offsets off this label.
// lbl_eu_80538CE8 is the interim CTTask<CTaskGameEvt> vtable.
extern char lbl_eu_80538C00[];     // CTaskGameEvt vtable region
extern char lbl_eu_80538CE8[];     // CTTask<CTaskGameEvt> vtable
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

class CTaskGameEvt : public CTTask<CTaskGameEvt> {
public:
    CTaskGameEvt(int arg);
    virtual ~CTaskGameEvt();
    void Init();
    void Term();
    void Move();
    static CTaskGameEvt* create(CProcess* pParent, int arg);

    // 0x0-0x54: CTTask<CTaskGameEvt>
    u32 field_54;          // 0x54
    IScnRender mRenderCB;  // 0x58 render callback subobject
    CScn* mScene;          // 0x5C
    u32 mFlags;            // 0x60
}; // size: 0x64

// Minimal declaration for the cf event-task singleton query (avoids pulling
//) the cf header, which would collide with the local CTTask copy).
namespace cf {
class CTaskGameCf {
public:
    static CTaskGameCf* getInstance();
};
} // namespace cf

// CScn OOL render-callback registration.
void addRenderCB__4CScnFP10IScnRenderUlUl(CScn* scn, IScnRender* cb, u32 prio, u32 flag);
void removeRenderCB__4CScnFP10IScnRender(CScn* scn, IScnRender* cb);

// Free-function imports defined in sibling TUs (cf/CTaskREvent.cpp).
void func_80165038();
void func_80164CFC();
int func_80164C48();
int func_80164954();