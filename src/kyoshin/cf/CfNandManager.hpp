#pragma once

#include <types.h>
#include "monolib/work/CTTask.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

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
// Scale-offset doubles used by the capture downsampler (func_80240084).
extern double lbl_eu_806686E8;                  // offset added past threshold
extern double lbl_eu_806686F0;                 // offset added below threshold
extern double lbl_eu_806686F8;                 // scale numerator offset
extern double lbl_eu_80668700;                 // scale denominator / coord offset

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

// --- imports used only by func_8023C2E4 ---
extern "C" void func_8025EC0C(u8* dst, const u8* src);   // kizuna compaction
struct CfNandSub14;
extern "C" void func_80174658(CfNandSub14* state);        // scratch initializer

// Clamp bounds fetched through the per-entry virtual call in func_8023C2E4.
struct CfNandClampView {
    u8 _pad04[4];
    f32 f04; // +0x04
    f32 f08; // +0x08
    f32 f0C; // +0x0C
};

// Subobject embedded at save-source entry +0x17C, dispatched virtually at
// vtable slot +0x224. Never constructed (novtable); placeholder slots mirror
// the CfNandPlayerVt recipe so vf224 lands at vtable+0x224.
struct __attribute__((packed)) __declspec(novtable) CfNandEntryVt {
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
    virtual void _v0AC();
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
    virtual void _v0D8();
    virtual void _v0DC();
    virtual void _v0E0();
    virtual void _v0E4();
    virtual void _v0E8();
    virtual void _v0EC();
    virtual void _v0F0();
    virtual void _v0F4();
    virtual void _v0F8();
    virtual void _v0FC();
    virtual void _v100();
    virtual void _v104();
    virtual void _v108();
    virtual void _v10C();
    virtual void _v110();
    virtual void _v114();
    virtual void _v118();
    virtual void _v11C();
    virtual void _v120();
    virtual void _v124();
    virtual void _v128();
    virtual void _v12C();
    virtual void _v130();
    virtual void _v134();
    virtual void _v138();
    virtual void _v13C();
    virtual void _v140();
    virtual void _v144();
    virtual void _v148();
    virtual void _v14C();
    virtual void _v150();
    virtual void _v154();
    virtual void _v158();
    virtual void _v15C();
    virtual void _v160();
    virtual void _v164();
    virtual void _v168();
    virtual void _v16C();
    virtual void _v170();
    virtual void _v174();
    virtual void _v178();
    virtual void _v17C();
    virtual void _v180();
    virtual void _v184();
    virtual void _v188();
    virtual void _v18C();
    virtual void _v190();
    virtual void _v194();
    virtual void _v198();
    virtual void _v19C();
    virtual void _v1A0();
    virtual void _v1A4();
    virtual void _v1A8();
    virtual void _v1AC();
    virtual void _v1B0();
    virtual void _v1B4();
    virtual void _v1B8();
    virtual void _v1BC();
    virtual void _v1C0();
    virtual void _v1C4();
    virtual void _v1C8();
    virtual void _v1CC();
    virtual void _v1D0();
    virtual void _v1D4();
    virtual void _v1D8();
    virtual void _v1DC();
    virtual void _v1E0();
    virtual void _v1E4();
    virtual void _v1E8();
    virtual void _v1EC();
    virtual void _v1F0();
    virtual void _v1F4();
    virtual void _v1F8();
    virtual void _v1FC();
    virtual void _v200();
    virtual void _v204();
    virtual void _v208();
    virtual void _v20C();
    virtual void _v210();
    virtual void _v214();
    virtual void _v218();
    virtual void _v21C();
    virtual void _v220();
    virtual CfNandClampView* vf224(); // vtable +0x224
};

// 0x14-byte scratch initialized by func_80174658 inside the func_8023C2E4
// entry loop, filled from the source entry, then copied into the destination.
struct CfNandSub14 {
    s16 f00;
    s16 f02;
    u32 f04;
    u8 f08;
    u8 _pad09[3];
    f32 f0C;
    f32 f10;
};

// Player-side view for func_8023C2E4's placement bit: getPlayer(0)+0xC4 points
// at an object whose +0x4EC word bit30 becomes the per-entry flag byte.
struct CfNandSub4EC {
    u8 _pad[0x4EC];
    u32 field4EC;
};
struct CfNandPlayerC4 {
    u8 _pad[0xC4];
    CfNandSub4EC* fieldC4;
};

// Unaligned head fields of a work entry (offsets 0x02..0x24 are 2-mod-4),
// kept in a minimal packed substruct so the rest of the entry stays naturally
// aligned (MWCC needs aligned arrays to emit its word-pair copy loops).
struct __attribute__((packed)) CfNandEntryHead {
    u16 f000;
    u32 f002;
    u32 f006;
    u32 f00A;
    u32 f00E;
    u32 f012;
    u32 f016;
    u16 gap01A;
    u32 f01C;
    u32 f020;
    u32 f024;
}; // 0x28

// One 0x304-byte destination entry built by func_8023C2E4.
struct CfNandWorkEntryDst {
    CfNandEntryHead head;
    u32 arr028[48]; // 0x28..0xE7
    u32 arr0E8[36]; // 0xE8..0x177
    u16 f178;
    u16 gap17A;
    u32 f17C;
    f32 f180;
    f32 f184;
    f32 f188;
    f32 f18C;
    f32 f190;
    f32 f194;
    s16 f198;
    s16 f19A;
    s16 f19C;
    u16 gap19E;
    f32 f1A0;
    f32 f1A4;
    s16 f1A8;
    s16 f1AA;
    s16 f1AC;
    s16 f1AE;
    s16 f1B0;
    s16 f1B2;
    s16 f1B4;
    s16 f1B6;
    u8 f1B8;
    u8 gap1B9[3];
    f32 f1BC;
    f32 f1C0;
    f32 f1C4;
    f32 f1C8;
    f32 f1CC;
    u8 f1D0;
    u8 f1D1;
    u8 f1D2;
    u8 f1D3;
    u8 f1D4;
    u8 gap1D5[3];
    f32 f1D8;
    u32 f1DC;
    u32 f1E0;
    u32 f1E4;
    u32 f1E8;
    u32 f1EC;
    u32 f1F0;
    u8 kizuna[0xAC]; // 0x1F4: compact kizuna block (func_8025EC0C)
    u32 f2A0;
    u32 f2A4;
    u32 f2A8;
    s16 f2AC;
    s16 f2AE;
    u32 f2B0;
    u8 f2B4;
    u8 gap2B5[3];
    f32 f2B8;
    f32 f2BC;
    u8 f2C0;
    u8 tail[0x40]; // zero-filled scratch region
    u8 gapEnd[3];
}; // 0x304

// One 0x3DD4-byte source entry read by func_8023C2E4.
struct CfNandWorkEntrySrc {
    CfNandEntryHead head;
    u32 arr028[48]; // 0x28..0xE7
    u32 arr0E8[36]; // 0xE8..0x177
    u16 f178;
    u16 gap17A;
    CfNandEntryVt sub; // 0x17C: virtually dispatched subobject
    u8 gap180[0x159C]; // 0x180..0x177B
    u32 f177C;
    u32 f1780;
    u32 f1784;
    u8 gap1788[0x1D4C]; // 0x1788..0x17CB
    u32 f17CC;
    f32 f17D0;
    f32 f17D4;
    f32 f17D8;
    f32 f17DC;
    f32 f17E0;
    f32 f17E4;
    s16 f17E8;
    s16 f17EA;
    s16 f17EC;
    u16 gap17EE;
    f32 f17F0;
    f32 f17F4;
    s16 f17F8;
    s16 f17FA;
    s16 f17FC;
    s16 f17FE;
    s16 f1800;
    s16 f1802;
    s16 f1804;
    s16 f1806;
    u8 f1808;
    u8 gap1809[3];
    f32 f180C;
    f32 f1810;
    f32 f1814;
    f32 f1818;
    f32 f181C;
    u8 f1820;
    u8 f1821;
    u8 f1822;
    u8 f1823;
    u8 f1824;
    u8 gap1825[3];
    f32 f1828;
    u32 f182C;
    u32 f1830;
    u32 f1834;
    u32 f1838;
    u32 f183C;
    u32 f1840;
    s16 f34D4;
    s16 f34D6;
    u32 f34D8;
    u8 f34DC;
    u8 gap34DD[3];
    f32 f34E0;
    f32 f34E4;
    u8 gap34E8[0x4C];
    u8 kizunaSrc[0x8A0]; // big kizuna block compacted into the destination
}; // 0x3DD4

// Whole destination buffer CRC16'd by func_8023C2E4 (0x6C28 bytes).
struct CfNandWorkBuf {
    u8 head[0x41F0]; // bulk-copied from the character blob
    CfNandWorkEntryDst entry[14];
};

// --- imports for the save-image builder (func_8023C93C) ---
extern "C" u8* func_8009CF0C();                     // capture-region source pointer
extern "C" void func_8006CBD8(u8* dst, u8* src);    // camera settings fill
extern "C" void func_8016E09C(struct CfNandWthrBlock* w);
extern "C" void func_80207C94(u8* dst);             // MINE region builder
f32 func_801C0014();                                // mangled __Fv in retail
extern "C" f32 func_801896A0();
extern "C" f32 func_801895EC();
extern "C" struct CfNandNameRoot* func_800B6CA0(); // save-name directory container
// Retail symbol is CfGameManager's static member but returns its result in r3.
extern "C" u32 func_8007F9AC__Q22cf13CfGameManagerFv();
extern "C" u8* getInstance__14Class_80296898Fv();
extern char* lbl_eu_806641B8;                       // item blob (see CItem.hpp)
extern u16 lbl_eu_80661AF4;
extern u16 lbl_eu_80661AF6;
extern f32 lbl_eu_80663D94;                         // progress-area float default

// Descriptor block preceding each CRC'd region of the save image.
struct CfNandSaveTagBlock {
    u32 tag;     // four-character region tag
    u32 length;  // region length
    u32 unk08;   // always 0x10
    u32 crc;     // CRC16 of the following region
};

// 0x0C progress area at image +0x11EB0.
struct CfNandProgressArea {
    u32 field00;
    f32 f04;
    u16 f08;
    u16 f0A;
};

// 0x10 weather block at image +0x24090 (+0xE carries the message id).
struct CfNandWthrBlock {
    u8 _pad0[0xE];
    u16 f0E;
};

// 0x10 sound/mix area at image +0x240C0 (three floats from audio queries).
struct CfNandSndArea {
    f32 f00;
    f32 f04;
    f32 f08;
    u32 f0C;
};

// Camera settings block at image +0x11F30 (filler writes through +0xC).
struct CfNandCamBlock {
    u8 _pad0[0xC];
    u32 f0C;
};

// Node of the circular save-name directory chain rooted by func_800B6CA0.
struct CfNandNameNode {
    CfNandNameNode* mNext; // 0x00
    u8 _pad04[4];
    u32 f08;
    u32 f0C;
    u32 f10;
    u32 f14;
    f32 f18;
    u32 f1C;
    u16 f20;
    u8 f22;
    u8 f23;
};

// Container whose +0x04 roots the circular name-directory chain.
struct CfNandNameRoot {
    u8 _pad0[4];
    CfNandNameNode* mNext;
};

// One compacted 0x1C-byte directory entry in the save image.
struct __attribute__((packed)) CfNandSaveNameEntry {
    u32 f04; // copied first, then cleared (retail keeps both stores)
    u32 f08;
    u32 f0C;
    u32 f10;
    f32 f14;
    u32 f18;
    u16 f1C;
    u8 f1E;
    u8 f1F;
}; // 0x1C

struct CfNandSaveNameTable {
    s32 count; // live entry counter (kept at table+0)
    CfNandSaveNameEntry entries[20];
}; // 0x234

// Full on-media save image assembled by func_8023C93C (0x248F0 bytes,
// version tag 0x70002): header, tagged CRC regions, and tails.
struct CfNandSaveImage {
    u32 magic;                    // 0x00: 'USRD'
    u32 totalSize;                // 0x04: 0x248F0
    u32 unk08;                    // 0x08: 0x10
    u32 version;                  // 0x0C: 0x70002
    CfNandSaveTagBlock slotTag;   // 0x10
    CfNandSaveBuf slot;           // 0x20: save-slot payload built by func_8023C7C4
    u8 slotPad[0x9BE8];           // pad the slot out to +0x9CC0
    u8 pad9CC0[0x360];            // -> 0xA020
    CfNandSaveTagBlock flagTag;   // 0xA020
    u8 flagData[0x1220];          // 0xA030
    CfNandSaveTagBlock gameTag;   // 0xB250
    CfNandWorkBuf work;           // 0xB260
    u8 pad11E88[0x18];            // -> 0x11EA0
    CfNandSaveTagBlock timeTag;   // 0x11EA0
    CfNandProgressArea progress;  // 0x11EB0
    u8 pad11EBC[0x14];            // -> 0x11ED0
    CfNandSaveTagBlock partyTag;  // 0x11ED0
    CfNandPartySnapshot snapshot; // 0x11EE0
    u8 pad11F14[0xC];             // -> 0x11F20
    CfNandSaveTagBlock camTag;    // 0x11F20
    CfNandCamBlock camBlock;      // 0x11F30
    u8 pad11F40[0x10];            // -> 0x11F50
    CfNandSaveTagBlock itemTag;   // 0x11F50
    u8 itemBlob[0x12120];         // 0x11F60
    CfNandSaveTagBlock wthrTag;   // 0x24080
    CfNandWthrBlock wthrBlock;    // 0x24090
    u8 pad240A0[0x10];            // -> 0x240B0
    CfNandSaveTagBlock sndTag;    // 0x240B0
    CfNandSndArea sndArea;        // 0x240C0
    u8 pad240D0[0x10];            // -> 0x240E0
    CfNandSaveTagBlock mineTag;   // 0x240E0
    u8 mineRegion[0x384];         // 0x240F0
    u8 pad24474[0x1C];            // -> 0x24490
    CfNandSaveTagBlock tboxTag;   // 0x24490
    CfNandSaveNameTable names;    // 0x244A0
    u8 pad246D4[0x1CC];           // -> 0x248A0
    CfNandSaveTagBlock optdTag;   // 0x248A0
    u8 optdBlob[0x40];            // 0x248B0
}; // 0x248F0
