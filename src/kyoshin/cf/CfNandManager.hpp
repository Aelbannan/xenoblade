#pragma once

#include <types.h>
#include "monolib/work/CTTask.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/scn/IScnRender.hpp"

// C-linkage runtime imports (retail symbol names - keep linkage/signature verbatim).
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

    // Retail ring initialization leaves mTag untouched (offset 4 is never
    // stored by the ctor or the array-construction loop).
    CfNandEvent() : mWord(0), mField8(0), mFieldC(0), mFieldE(0), mFieldF(0) {}

    // Full-field ctor: lets a call argument materialize directly into the
    // by-value parameter's invisible-reference slot.
    CfNandEvent(u32 w, u32 tag, u32 f8, u16 fc, u8 fe, u8 ff)
        : mWord(w), mTag(tag), mField8(f8), mFieldC(fc), mFieldE(fe),
          mFieldF(ff) {}
};

// TU-local helper removed: ring slots are constructed natively via the
// CfNandEventQueue member's default initialization inside the real ctor.

// Event queue embedded in CfNandManager at +0x60. Its destructor is the retail
// __dt__8023BFCC, which zeroes the head/tail counters (0x104/0x108 relative to
// this struct) before freeing the block. Entries are pushed through mRingBase
// at slot (mHead + mTail) % mSize (see func_8023FB28 / func_8023FD4C in
// CfNandManager.cpp). Counters are signed in retail (divw modulo).
struct CfNandEventQueue {
    CfNandEvent mFirst;     // 0x00 (+0x60): slot 0, constructed inline by the
                            // manager ctor
    CfNandEvent mRest[15];  // 0x10 (+0x70): slots 1..15, built by the
                            // array-construction loop
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

// Pending-event scratch copied out of the ring by func_8023EB78 and consumed
// by the event handler __dt__8023E63C. Note that mPayload (offset 0x178)
// aliases the save-block pointer read by func_8023CD9C / func_8023E544.
struct CfNandPendingEvent {
    u32 mCb;      // 0x170: raw callback dispatched when nonzero
    u32 mTag;     // 0x174
    u32 mPayload; // 0x178
    u16 mType;    // 0x17C (aliases CfNandManager::field_17C)
    u8  mSubtype; // 0x17E (aliases field_17E)
    u8  mFlag;    // 0x17F (aliases field_17F)
};

// Directory-entry count written by the NAND readdir request (func_804DA3E4)
// and consumed by the name-table scan in __dt__8023E63C.
extern s32 lbl_eu_80664778;
// Format argument handed to ml::FixStr<32>::format by the name builders.
extern u32 lbl_eu_806628C0;
// Default write-back table used by the NAND flush request in func_8023EB78.
extern u8 lbl_eu_80577358[];

// Save block pointed to by CfNandManager+0x178 and validated by func_8023E544:
// magic 'DMMY' placeholder word, version tag at +0xC (0x70001/0x70002), stored
// CRC16 at +0x1C, 0x9C80-byte payload at +0x20.
struct CfNandSaveBlock {
    u32 magic;        // 0x00: 0x444D4D59 ('DMMY') = empty slot
    u8 _pad04[8];
    u32 field_0C;     // 0x0C: version tag
    u8 _pad10[0xC];
    u32 field_1C;     // 0x1C: stored CRC16
    u8 data[0x9C80];  // 0x20: payload covered by the CRC
};

// Head of the full on-media save image validated by func_8023CD9C (offsets
// shared by all three version tags): 12 independently CRC16'd regions, each
// checksum word sitting immediately before its region.
struct CfNandSaveHead {
    u32 magic;        // 0x00
    u8 _04[8];
    u32 field_0C;     // 0x0C version tag
    u8 _10[0xC];
    u32 crc0;         // 0x1C
    u8 r1[0x9C80];    // 0x20
    u32 crc1;         // 0xA02C
    u8 r2[0x22C];     // 0xA030
    u32 crc2;         // 0xB25C
    u8 r3[0x6C28];    // 0xB260
    u32 crc3;         // 0x11EAC
    u8 r4[0xC];       // 0x11EB0
    u32 crc4;         // 0x11EDC
    u8 r5[0x34];      // 0x11EE0
    u32 crc5;         // 0x11F2C
    u8 r6[0x10];      // 0x11F30
    u32 crc6;         // 0x11F5C
};

// Version-0x70002 tail (base 0x11F60).
struct CfNandSaveTailOSC {
    u8 r7[0x12120];   // 0x11F60
    u32 crc7;         // 0x2408C
    u8 r8[0x10];      // 0x24090
    u32 crc8;         // 0x240BC
    u8 r9[0x10];      // 0x240C0
    u32 crc9;         // 0x240EC
    u8 r10[0x384];    // 0x240F0
    u32 crc10;        // 0x2449C
    u8 r11[0x234];    // 0x244A0
    u32 crc11;        // 0x248AC
    u8 r12[0x40];     // 0x248B0
};

// Version-0x70001 tail (base 0x11F60); same shape, larger region 7.
struct CfNandSaveTailCalc {
    u8 r7[0x157D0];   // 0x11F60
    u32 crc7;         // 0x2774C
    u8 r8[0x10];      // 0x27750
    u32 crc8;         // 0x2777C
    u8 r9[0x10];      // 0x27780
    u32 crc9;         // 0x277AC
    u8 r10[0x384];    // 0x277B0
    u32 crc10;        // 0x27B5C
    u8 r11[0x234];    // 0x27B60
    u32 crc11;        // 0x27F6C
    u8 r12[0x40];     // 0x27F70
};

namespace cf{
    // +0x54 interface slot (retail IWorkEvent): modelled as a novtable base so
    // IScnRender lands at +0x58 like retail. Declared-only virtuals and the
    // implicit trivial dtor keep the derived ctor/dtor free of base calls.
    class __declspec(novtable) CIWorkEventSlot {
    public:
        virtual void vf0C();
    };
    // The retail TU references its vtable blob through the plain
    // lbl_eu_80536BBC label, so suppress MWCC's auto __vt__ emission and the
    // auto vptr stores in ctors/dtors; the ctor/dtor hand-store all three
    // subobject vtables explicitly (Clib/CLibLayout recipe).
    class __declspec(novtable) CfNandManager : public CTTask<CfNandManager>, public CIWorkEventSlot, public IScnRender{
    public:
        // Retail US symbol for this getter is the unmangled func_8024005C
        // (called as a static member only from kyoshin/CTaskGameCf.cpp).
        static u32 func_8024005C();

        //0x000-0x054 CTTask
        //0x054-0x058 CIWorkEventSlot (novtable interface base, hand-stored)
        //0x058-0x05C IScnRender
        /* 0x5C */ u8* field_0x5C;          // heap buffer (freed by Term)
        /* 0x60 */ CfNandEventQueue mEventQueue; // event queue + ring metadata
        /* 0x170 */ CfNandPendingEvent mPending;
        // NOTE: there is no separate save-block member: mPending.mPayload
        // (0x178) doubles as the CfNandSaveBlock pointer checked by
        // func_8023E544 / func_8023CD9C.
        /* 0x17C-0x17F: no separate members - mPending.mType/mSubtype/mFlag
           live here (retail aliases). */
        /* 0x180 */ u32 field_180;              // status flags (bit0/bit29 read by func_8023EABC)
        /* 0x184 */ u16 field_184;
        /* 0x186 */ u16 field_186;
        /* 0x188 */ void (CfNandManager::*mMoveHook)(); // 12-byte ptmf hook (dispatched by Move)

        virtual ~CfNandManager();
        CfNandManager();
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
// Global presentation/event bitfield; 0xafa40000 mask gates func_8023F3C0.
extern u32 lbl_eu_80663E24;
// NAND teardown in-progress flag (.sbss u16) set around func_8023D3D8 in the
// __dt__8023E448 teardown path (also polled by CSysWinScenarioLog).
extern u16 lbl_eu_80664772;
// Extra sbss words cleared by CfNandManager::~CfNandManager.
extern u8  lbl_eu_8066476D;
extern u16 lbl_eu_80664774;
// CfNandManager vtable (.data, 0xC4 bytes; the dtor stores it explicitly).
extern u8 lbl_eu_80536BBC[];
// CTTask<CfNandManager> base vtable (stored at +0x10 before the derived
// vtable overwrite in the ctor).
extern u8 lbl_eu_80536CB4[];
// Static default move-hook ptmf (.data) copied into +0x188 by the ctor.
extern u32 lbl_eu_80536B20[3];
// Null pointer-to-member-function constant (defined in CUICfManager.cpp).
extern u32 __ptmf_null[3];
// NAND request helpers (libs/monolib/src/nand/CNand.cpp).
extern "C" int func_804DA29C(CNandQueue* self, const char* name, u32 a2, u32 a3,
                             void* a4, u32 a5);
extern "C" int func_804DA34C(CNandQueue* self, const char* name, u32 a2, u32 a3);
extern "C" int func_804DA3A0(CNandQueue* self, u32 a1);
extern "C" int func_804DA3E4(CNandQueue* self, u8* path, u32 size, void* outCount);
extern "C" int func_eu_804DE660(CNandQueue* self, u32 a1, u32 a2);

// Party-snapshot fallback data (.bss, 3 words) for missing player slots
// (func_8023C1F0) and the fallback float (.sbss).
extern u32 lbl_eu_8057164C[3];
extern f32 lbl_eu_80663E54;

// First 8 bytes shared by the snapshot entry and the player info struct;
// func_8023C1F0 copies it as one unit.
struct CfNandPair {
    u32 field_00;
    u32 field_04;
};

// 0x10-byte party snapshot entry {3 words + float} filled by func_8023C1F0.
struct CfNandPartyEntry {
    CfNandPair mPair; // 0x00
    u32 field_08;
    f32 field_0C;
};

// 12-byte info struct returned by the player vtable slot +0xAC.
struct CfNandPartyInfo {
    CfNandPair mPair; // 0x00
    u32 field_08;
};

// 0x34-byte party snapshot (3 entries + CRC seed word) fed to OSCalcCRC16.
struct CfNandPartySnapshot {
    CfNandPartyEntry mEntry[3]; // 0x00..0x2F
    u32 field_30;               // 0x30
};

// --- imports used by the CfNandManager TU (declared here so the .cpp stays
// free of local extern "C" scaffolding) ---
extern "C" u32 func_8009CF8C(u32 resourceId);   // message-count lookup - (u32) form must match CUICfManager.hpp (10197)
extern "C" u32 func_8009CF84();                 // save-region size lookup
extern "C" u32 func_8006A80C();                 // game-progress bitfield
extern "C" void* func_8009EC9C(u16 index);      // character-data lookup
extern "C" void func_80084F50__Q22cf13CfGameManagerFv();
extern char lbl_eu_8050B470[];                  // bdat column-name blob
extern void* lbl_eu_80664090;                   // bdat table object
extern f32 lbl_eu_806686E0;                     // fallback float constant

// Inner descriptor written at +0x14 of the texture block built by
// func_8023C68C (height/width/format of the 0xa4x0x74 capture tile).
struct CfNandTexSub {
    u16 height;      // +0x00: 0x74
    u16 width;       // +0x02: 0xa4
    u32 format;      // +0x04: 4
    u32 offset;      // +0x08: 32-aligned payload offset
    u32 field20;
    u32 field24;
    u32 field28;
    u32 field2C;
    f32 field30;
    u8 field34[4];
};

// Descriptor block filled by func_8023C68C (0x38-byte header + payload).
struct CfNandTexBlock {
    u32 field00;     // 0x20AF30 resource tag
    u32 field04;     // 1
    u32 field08;     // 0xC
    u32 field0C;     // offset of the sub-descriptor (runtime-computed)
    u32 field10;     // 0
    CfNandTexSub sub; // +0x14
}; // 0x38

// One 0x18-byte party record copied into save buffers.
struct CfNandRecord {
    u32 w[6];
};

// Source view over the large manager object func_8023C7C4 reads party data
// from (progress words near +0xB260 and two records at +0xD1FC).
struct CfNandSaveSource {
    u8 _pad0[0xB260];
    u16 fieldB260;
    u16 fieldB262;
    u8 _pad4[0xD1FC - 0xB264];
    CfNandRecord mRecord[2];
};

// Save buffer laid out by func_8023C7C4 (CRC16 covers the first 0x9C80 bytes).
struct CfNandSaveBuf {
    s16 field00;          // 0x00: scenario counter
    u8 field02;           // 0x02: day of month
    u8 field03;           // 0x03: hour
    u16 field04;          // 0x04: day of year
    u16 field06;          // 0x06: weekday + 1
    u8 field08;           // 0x08: month
    u8 field09;           // 0x09: year
    u16 field0A;          // 0x0A: progress bits 4..19
    u16 field0C;          // 0x0C: progress bits 20..25
    u16 field0E;          // 0x0E: source progress word
    u8 field10;
    u8 field11;           // 0x11: OR-merged source progress byte
    u16 field12;
    CfNandRecord mRecord[2]; // 0x14
    char mName[0x20];        // 0x44: bdat string column
    u16 field64;             // 0x64: character id
    u8 field66;              // 0x66: slot id
    u8 field67;              // 0x67: message count
    u8 _pad68[0x18];
    CfNandTexBlock tex;      // 0x80
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

// 0x80-stride per-player table whose entry address is written into pushed
// events by func_8023FEDC (.data).
extern u8 lbl_eu_80576AC0[];
