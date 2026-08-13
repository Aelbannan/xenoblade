#pragma once

#include <types.h>
#include "monolib/work/CTTask.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/scn/IScnRender.hpp"

// C-linkage runtime imports (retail symbol names - keep linkage/signatures verbatim).
extern "C" long __ptmf_test(void* ptmf);
extern "C" void __dl__FPv(void*);

// NAND request-queue layout (mirrors libs/monolib/src/nand/CNand.cpp).
struct CNandQueue {
    u8 mReq[0x120]; // 0x000: 8 CNRequest entries (0x24 each)
    s16 mHead;      // 0x120: consume index (ring head)
    s16 mTail;      // 0x122: produce index (ring tail)
    u8 mStatus;     // 0x124
    u8 mFlag;       // 0x125
    u8* mTable;     // 0x128: shared callback table
};

// NAND completion pump (defined in libs/monolib/src/nand/CNand.cpp).
extern "C" void func_804DA1CC(CNandQueue* self);

// Single event slot of the CfNandManager event ring (0x10 bytes).
struct CfNandEvent {
    u32 mWord;    // 0x0: payload word
    u32 mTag;     // 0x4: event tag
    u32 mField8;  // 0x8
    u16 mFieldC;  // 0xC
    u8  mFieldE;  // 0xE
    u8  mFieldF;  // 0xF
};

// Event queue embedded in CfNandManager at +0x60. Its destructor is the retail
// __dt__8023BFCC, which zeroes the head/tail counters (0x104/0x108 relative to
// this struct) before freeing the block. Entries are pushed through mRingBase
// at slot (mHead + mTail) % mSize (see func_8023FB28 / func_8023FD4C in
// CfNandManager.cpp). Counters are signed in retail (divw modulo).
struct CfNandEventQueue {
    u8 field_0x00[0x100];   // 0x00: ring data / padding
    CfNandEvent* mRingBase; // 0x100: ring base pointer
    s32 mHead;              // 0x104: head counter
    s32 mTail;              // 0x108: tail counter
    s32 mSize;              // 0x10C: ring size
};

namespace cf{
    class CfNandManager : public CTTask<CfNandManager>, public IWorkEvent, public IScnRender{
    public:
        // Retail US symbol for this getter is the unmangled func_8024005C
        // (called as a static member only from kyoshin/CTaskGameCf.cpp).
        static u32 func_8024005C();

        //0x000-0x054 CTTask
        //0x054-0x058 IWorkEvent
        //0x058-0x05C IScnRender
        /* 0x5C */ u8* field_0x5C;          // heap buffer (freed by Term)
        /* 0x60 */ CfNandEventQueue mEventQueue; // event queue + ring metadata
        /* 0x170 */ u8 unk170[0x188 - 0x170];    // event ring data / padding
        /* 0x188 */ void (CfNandManager::*mMoveHook)(); // 12-byte ptmf hook (dispatched by Move)

        virtual ~CfNandManager();
        void Term();
        void Move();
        u32 func_8023E4D4();
        void create();
        void cbRenderBefore();
        void Init();
        void OnFileEvent();
        void func_802405FC();
        void func_80240604();
        void func_8024060C();
    }; //size = 0x194
} //namespace cf

// cf::CfNandManager singleton (set by the ctor; .sbss).
extern cf::CfNandManager* lbl_eu_80664768;
// NAND queue idle flag checked by func_8023C1C0 (.sbss byte).
extern u8 lbl_eu_8066476C;
// CNand singleton (.bss, 0x130).
extern CNandQueue lbl_eu_8065FD00;

// D80 scene flag (.sbss; holds a CScn*). Retail-unmangled C helper defined in
// CfRes.cpp; declared here for the CfNandManager TU's teardown path.
extern "C" int CfRes_getD80Flag();
// NAND resource teardown globals (.sbss) cleared by func_80240420.
extern u8* lbl_eu_8066477C;   // heap buffer freed via MemManager::deallocate
extern u16 lbl_eu_80664770;   // u16 counter cleared on teardown
// Global mode flag (.sbss); bit 0x200000 gates CfNandManager::Move's NAND pump.
extern u32 lbl_eu_80663E28;
// NAND teardown in-progress flag (.sbss u16) set around func_8023D3D8 in the
// __dt__8023E448 teardown path (also polled by CSysWinScenarioLog).
extern u16 lbl_eu_80664772;
