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

    // Non-deleting teardown: zero the ring counters. The standalone retail
    // symbol __dt__8023BFCC is emitted separately (flat extern "C" function in
    // the .cpp); this inline member gives CfNandManager::~CfNandManager the
    // inlined null-checked call shape (addic./beq) retail shows.
    ~CfNandEventQueue() {
        mTail = 0;
        mHead = 0;
    }
};

namespace cf{
    // The IWorkEvent base is omitted from the C++ declaration (CfTaskMain
    // scheme): retail's dtor only calls CProcess::~CProcess, so a declared
    // IWorkEvent base would add a compiler-generated ~IWorkEvent call. The
    // secondary vtable slot at +0x54 is modelled as a plain member; the retail
    // dtor stores all three subobject vtables explicitly. IScnRender stays a
    // base (its dtor is inline-empty, so no call is emitted) so
    // static_cast<IScnRender*> keeps the retail null-checked +0x58 upcast.
    class CfNandManager : public CTTask<CfNandManager>, public IScnRender{
    public:
        // Retail US symbol for this getter is the unmangled func_8024005C
        // (called as a static member only from kyoshin/CTaskGameCf.cpp).
        static u32 func_8024005C();

        //0x000-0x054 CTTask
        //0x054-0x058 IWorkEvent vtable slot (interface base omitted from C++)
        /* 0x54 */ u32 mIWorkEventVtable;
        //0x058-0x05C IScnRender
        /* 0x5C */ u8* field_0x5C;          // heap buffer (freed by Term)
        /* 0x60 */ CfNandEventQueue mEventQueue; // event queue + ring metadata
        /* 0x170 */ u8 unk170[0x10];            // event ring data / padding
        /* 0x180 */ u32 field_180;              // status flags (bit0/bit29 read by func_8023EABC)
        /* 0x184 */ u16 field_184;
        /* 0x186 */ u16 field_186;
        /* 0x188 */ void (CfNandManager::*mMoveHook)(); // 12-byte ptmf hook (dispatched by Move)

        virtual ~CfNandManager();
        void Term();
        void Move();
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
// u16 event-status word (.sbss) cleared by func_8023FC18, set by the ctor
// and by func_8023F288's file-event handling.
extern u16 lbl_eu_8066476E;
// NAND resource teardown globals (.sbss) cleared by func_80240420.
extern u8* lbl_eu_8066477C;   // heap buffer freed via MemManager::deallocate
// Second half of the buffer pair allocated by func_80240360 (buf + size1).
extern u8* lbl_eu_80664780;
extern u16 lbl_eu_80664770;   // u16 counter cleared on teardown
// CNand singleton (.bss, 0x130).
extern CNandQueue lbl_eu_8065FD00;

// D80 scene flag (.sbss; holds a CScn*). Retail-unmangled C helper defined in
// CfRes.cpp; declared here for the CfNandManager TU's teardown path.
extern "C" int CfRes_getD80Flag();
// Global mode flag (.sbss); bit 0x200000 gates CfNandManager::Move's NAND pump.
extern u32 lbl_eu_80663E28;
// NAND teardown in-progress flag (.sbss u16) set around func_8023D3D8 in the
// __dt__8023E448 teardown path (also polled by CSysWinScenarioLog).
extern u16 lbl_eu_80664772;
// Extra sbss words cleared by CfNandManager::~CfNandManager.
extern u8  lbl_eu_8066476D;
extern u16 lbl_eu_80664774;
// CfNandManager vtable (.data, 0xC4 bytes; the dtor stores it explicitly).
extern const u32 lbl_eu_80536BBC[];
// Party-snapshot fallback data (.bss, 3 words) for missing player slots
// (func_8023C1F0) and the fallback float (.sbss).
extern u32 lbl_eu_8057164C[3];
extern f32 lbl_eu_80663E54;

// 0x10-byte party snapshot entry {3 words + float} filled by func_8023C1F0.
struct CfNandPartyEntry {
    u32 field_00;
    u32 field_04;
    u32 field_08;
    f32 field_0C;
};

// 12-byte info struct returned by the player vtable slot +0xAC.
struct CfNandPartyInfo {
    u32 field_00;
    u32 field_04;
    u32 field_08;
};

// 0x34-byte party snapshot (3 entries + CRC seed word) fed to OSCalcCRC16.
struct CfNandPartySnapshot {
    CfNandPartyEntry mEntry[3]; // 0x00..0x2F
    u32 field_30;               // 0x30
};

// Cast-only iface for the CfObjectPc player vtable (slot mapping mirrors
// CMainMenuPlayerVt: two hidden RTTI slots, placeholders 0x08..0xA8, so
// vf0AC lands at vtable+0xAC and vf0D8 at +0xD8). Never constructed.
struct CfNandPlayerVt {
    virtual void _v008();
    virtual void _v00C();
    virtual void _v010();
    virtual void _v014();
    virtual void _v018();
    virtual void _v01C();
    virtual void _v020();
    virtual void _v024();
    virtual void _v028();
    virtual void _v02C();
    virtual void _v030();
    virtual void _v034();
    virtual void _v038();
    virtual void _v03C();
    virtual void _v040();
    virtual void _v044();
    virtual void _v048();
    virtual void _v04C();
    virtual void _v050();
    virtual void _v054();
    virtual void _v058();
    virtual void _v05C();
    virtual void _v060();
    virtual void _v064();
    virtual void _v068();
    virtual void _v06C();
    virtual void _v070();
    virtual void _v074();
    virtual void _v078();
    virtual void _v07C();
    virtual void _v080();
    virtual void _v084();
    virtual void _v088();
    virtual void _v08C();
    virtual void _v090();
    virtual void _v094();
    virtual void _v098();
    virtual void _v09C();
    virtual void _v0A0();
    virtual void _v0A4();
    virtual void _v0A8();
    virtual CfNandPartyInfo* vf0AC();  // vtable +0xAC
    virtual void _v0B0();
    virtual void _v0B4();
    virtual void _v0B8();
    virtual void _v0BC();
    virtual void _v0C0();
    virtual void _v0C4();
    virtual void _v0C8();
    virtual void _v0CC();
    virtual void _v0D0();
    virtual void _v0D4();
    virtual float vf0D8();             // vtable +0xD8
};

// Word view of CBattleManager+0x194 (party-snapshot CRC seed read by
// func_8023C1F0).
struct CfNandBmView {
    u8 _pad[0x194];
    u32 field_194;
};
