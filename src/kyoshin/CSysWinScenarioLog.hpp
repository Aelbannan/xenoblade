#pragma once

#include <types.h>
#include "monolib/work/CProcess.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"

namespace nw4r { namespace lyt {
    class Layout;
    class AnimTransform;
}}

class CSysWinScenarioLog : public CProcess {
public:
    CSysWinScenarioLog();
    virtual ~CSysWinScenarioLog();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();
    void OnFileEvent();

    /* 0x3c-0x63: region owned by layout / IWorkEvent sub-object */
    u8 _3C[0x64 - 0x3C];
    /* 0x64 */ u8 mFlag64;
    /* 0x65-0x66 */ u8 _65[2];
    /* 0x67 */ u8 mFlag67;
    /* 0x68-0x7b */ u8 _68[0x7C - 0x68];
    /* 0x7c: UnkClass_8045F564 embedded region (dtor calls ~UnkClass_8045F564) */
    UnkClass_8045F564 mMemRegion;
    /* 0x8c-0x8f */ u8 _8C[0x90 - 0x8C];
    /* 0x90 */ u8 mState;    // Move() state machine
    /* 0x91-0x9b */ u8 _91[0x9C - 0x91];
    /* 0x9c */ nw4r::lyt::Layout* mpLayout;
    /* 0xa0 */ nw4r::lyt::AnimTransform* mpAnim;
};


// Minimal intrusive singly-linked list node used by the battle-list scans
// in this unit (next pointer at +0x00).
struct CSysWinListNode {
    CSysWinListNode* next; // +0x00
};

// Battle-manager view used by func_8027EF50: the list sentinel node lives at
// +0x48 and the iteration counts nodes until it reaches the sentinel again.
struct CSysWinBattleMgrView {
    u8 _0[0x48];
    CSysWinListNode* mListSentinel; // +0x48
};

// Layout vtable hook: lets Move() call the nw4r Layout virtual at vtable
// offset 0x38 (14 slab padding virtuals then the slot-14 method).
class CSysWinLayoutHook {
public:
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual void s3();
    virtual void s4();
    virtual void s5();
    virtual void s6();
    virtual void s7();
    virtual void s8();
    virtual void s9();
    virtual void sA();
    virtual void sB();
    virtual void sC();
    virtual void sD();
    virtual void mAt38(u32 arg);   // vtable slot 14 == offset 0x38
};

// Pad-button view used by Move() (button word at +0x4).
struct CSysWinPadView {
    u8 _0[4];
    u32 mButtons; // +0x4
};

// Sub-object structure used by func_8027F2DC: the owner stores a pointer to a
// small sub-state object at +0x3F60 whose +0x8 points at an inner object with
// a u32 sub-state value at +0x18.
struct CScenarioLogSubInner {
    u8 _0[0x18];
    u32 field_0x18;                      // +0x18 sub-state value
};
struct CScenarioLogSub {
    u8 _0[0x8];
    CScenarioLogSubInner* field_0x8;     // +0x8
};

// func_8027F848 / func_8027F2DC / func_8027FC80 argument view: a big object
// with a scenario-flag dword at +0x3F00 (bit 1 = gate), a u16 flag at
// +0x3E6C (bit 12), and an embedded player object whose address at +0x3E9C is
// compared against CfGameManager::getPlayer(0). The sub-state tracking fields
// (+0x3F28 u16 index, +0x3F60 sub-object pointer) are read by func_8027F2DC.
struct CScenarioLogOwner {
    u8 _0[0x3E6C];
    u16 field_0x3E6C;                    // +0x3E6C flags (bit 12 = forced player)
    u8 _3E6E[0x3E9C - 0x3E6E];
    u8 field_0x3E9C;                     // +0x3E9C embedded player marker (address compared to getPlayer(0))
    u8 _3E9D[0x3F00 - 0x3E9D];
    u32 field_0x3F00;                    // +0x3F00 scenario flag word (bit 0 = gate)
    u8 _3F04[0x3F28 - 0x3F04];
    u16 field_0x3F28;                    // +0x3F28 sub-state index (0x8 -> base 3)
    u8 _3F2A[0x3F60 - 0x3F2A];
    CScenarioLogSub* field_0x3F60;       // +0x3F60 sub-state object
};

// Type returned by func_8027FC80's per-object virtual calls (vtable offset
// 0x224): the field_0 scalar drives the 0x5d / 0x5e close comparison.
struct CSysWinDevEntry {
    u32 field_0;                          // +0x0
};

// func_8027FC80 arg0 view: the device object. Only the flag word at +0x3374
// is read directly here; the vtable calls are dispatched via the helpers below.
struct CSysWinDevice {
    u8 _0[0x3374];
    u32 field_0x3374;                     // +0x3374 flags
};

// func_8027FC80 helper: invoke a virtual at a runtime vtable offset on a raw
// object pointer. MWCC cannot express these as member calls without emitting a
// whole new vtable, so the call goes through the object's stored vtable.
inline u32 csysWinCallE0(void* self) {
    typedef u32 (*Fn)(void*);
    void** vt = (void**)*(void**)self;
    return ((Fn)vt[0xE0 / 4])(self);
}
inline CScenarioLogOwner* csysWinCall9C(void* self) {
    typedef CScenarioLogOwner* (*Fn)(void*);
    void** vt = (void**)*(void**)self;
    return ((Fn)vt[0x9C / 4])(self);
}
inline CSysWinDevEntry* csysWinCall224(void* self) {
    typedef CSysWinDevEntry* (*Fn)(void*);
    void** vt = (void**)*(void**)self;
    return ((Fn)vt[0x224 / 4])(self);
}

// arg2 view for func_8027F848: window/state dword at +0x74 (bit 8) and
// +0x78 (bit 1).
struct CScenarioLogArg2 {
    u8 _0[0x74];
    u32 field_0x74;                      // +0x74 (bit 8 = battle window)
    u32 field_0x78;                      // +0x78 (bit 1)
};
