#pragma once

#include <types.h>
#include "kyoshin/cf/IFactoryEvent.hpp"
#include "monolib/util/MemManager.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

namespace cf{

    // Forward declaration for the C-linkage imports below (pointer-only use).
    class CfObjectMove;

    // Character-voice event-list node owned by CCharVoiceMan. Link at +0x0C.
    // The first 0x0C bytes double as a __ptmf for type comparison; the +0x1C
    // word is the voice trigger's dispatch slot (a per-node virtual indirection
    // whose actual handler lives at a fixed offset in the block it points to).
    struct CVoiceNodeDispatch {
        u8 pad0[8];
        void (*fn_08)(void* node);                // +0x08 (level thread)
        void (*fn_0C)(void* node, void* arg);     // +0x0C (interact voice)
        u8 pad10[4];                              // +0x10
        int (*fn_14)(void* node);                 // +0x14 (voice probe; nonzero = dispatch)
        void* (*fn_18)(void* node, void* a, void* b); // +0x18 (battle voice)
    };

    // Fake SI over the voice nodes' dispatch block: the non-polymorphic pad
    // base puts the vptr at node+0x1C, so MWCC emits the retail r12 dispatch
    // idiom (`lwz r12,0x1c(rNode); lwz r12,slot(r12); mtctr; bctrl`) instead
    // of colouring the block pointer as a scratch r4. Slot offset = declared
    // index+1 (1 hidden slot), so #1=+0x08, #2=+0x0C, #4=+0x14, #5=+0x18.
    struct CVoiceNodeIfPad {
        u8 pad0[0x1C];   // -> +0x1C
    };
    struct CVoiceNodeIf : CVoiceNodeIfPad {
        virtual void vf08();                  // #1 => vtable +0x08 (level thread)
        virtual void vf0C(void* arg);         // #2 => vtable +0x0C (interact voice)
        virtual int vf10();                   // #3 => vtable +0x10 (level probe; func_802A330C compares its result)
        virtual int vf14();                   // #4 => vtable +0x14 (voice probe)
        virtual void* vf18(void* a, void* b); // #5 => vtable +0x18 (battle voice)
    };

    // Fake SIs for the edge/voice-source objects so MWCC emits the retail r12
    // dispatch idiom (`lwz r12,0(obj); lwz r12,slot(r12); mtctr; bctrl`)
    // instead of colouring the vptr as a scratch register for a fn-pointer
    // call. No pad: vptr lives at +0 of the cast object. Slot offset =
    // declared index*4 + 4 (1 hidden slot).
    struct CVoiceEdgeIf {
        virtual void e00(); virtual void e01(); virtual void e02(); virtual void e03();
        virtual void e04(); virtual void e05(); virtual void e06(); virtual void e07();
        virtual void e08(); virtual void e09(); virtual void e0A(); virtual void e0B();
        virtual void e0C(); virtual void e0D();
        virtual int fn_40(int arg);            // #15 => vtable +0x40 (browse gate)
    };
    struct CVoiceSrcInnerIf {
        virtual void s00(); virtual void s01(); virtual void s02(); virtual void s03();
        virtual void s04(); virtual void s05(); virtual void s06(); virtual void s07();
        virtual void s08(); virtual void s09();
        virtual void* fn_30();               // #11 => vtable +0x30
    };

    struct CSoundNode {
        u8 unk0[0xC];
        CSoundNode* next;   // +0x0C
        u8 unk10[0x8];      // +0x10 .. +0x17
        u32 field_18;       // +0x18
        CVoiceNodeDispatch* field_1C;  // +0x1C dispatch block
        u8 unk20[0x10];     // +0x20 .. +0x30
    };

    // Actor handed to the voice factory (flag word at +0x64).
    struct CVoiceFactoryActor {
        u8 unk0[0x64];
        u32 field_64;   // +0x64
    };

    // Two u32 id words from the battle-manager func_800EA444 result;
    // func_802A1D04 matches b->field_3F10 against either slot.
    struct CVoiceBtlSlot {
        u32 id0;   // +0x00
        u32 id1;   // +0x04
    };

    // Actor layout for func_802A2210 (reads a u32 at +0x3F10).
    struct CVoiceActorInfo {
        u8 unk0[0x3F10];
        u32 field_3F10;   // +0x3F10
    };

    // Player object whose character-voice move-base lives at +0x3E9C.
    // (Only the address of moveBase is taken; it is the CfObjectMove block.)
    struct CVoiceActorBase {
        u8 unk0[0x3E9C];
        u8 moveBase;   // +0x3E9C (marker; only address is used)
    };

    // vtable slot at +0x2BC used by func_802A1EA8's state check.
    struct CVoiceActorVTable {
        u8 pad[0x108];
        int (*fn_108)(void* self);        // +0x108 actor level
        u8 pad108[0x130 - 0x10C];
        float (*fn_130)(void* self);      // +0x130 actor gauge float
        u8 pad130[0x2BC - 0x134];
        int (*fn_2BC)(void* self);        // +0x2BC state check
    };

    // Fake SI over the large actor's vptr at +0: declared slot P lands at
    // vtable offset (P+2)*4 (2 implicit leading slots), so slot 173 = byte
    // 0x2BC. Exposes the state check as a real virtual so MWCC emits the
    // retail r12 dispatch (`lwz r12,0(self); lwz r12,0x2bc(r12); mtctr;
    // bctrl`) instead of colouring the vptr as a scratch r4.
    #define CVAPAD4(n)  virtual void v##n##0(); virtual void v##n##1(); virtual void v##n##2(); virtual void v##n##3();
    #define CVAPAD8(n)  CVAPAD4(n##0) CVAPAD4(n##1)
    #define CVAPAD16(n) CVAPAD8(n##0) CVAPAD8(n##1)
    #define CVAPAD32(n) CVAPAD16(n##0) CVAPAD16(n##1)
    #define CVAPAD64(n) CVAPAD32(n##0) CVAPAD32(n##1)
    struct CVoiceActorStateIf {
        CVAPAD64(0)   // slots 1-64
        virtual int fn_108();         // slot 65 -> vtable 0x108 (actor level)
        virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
        virtual void v6A(); virtual void v6B(); virtual void v6C(); virtual void v6D();
        virtual void v6E();           // slots 66-74
        virtual float fn_130();       // slot 75 -> vtable 0x130 (actor gauge float)
        CVAPAD64(8)   // slots 76-139
        CVAPAD32(9)   // slots 140-171
        virtual void v172();          // slot 172
        virtual void v173();          // slot 173
        virtual int fn_2BC();         // slot 174 -> vtable 0x2BC (state check)
    };
    #undef CVAPAD4
    #undef CVAPAD8
    #undef CVAPAD16
    #undef CVAPAD32
    #undef CVAPAD64

    // Move-base object embedded at CVoiceActorState +0x3E9C (CfObjectMove block).
    // Its vtable's +0x4C slot is invoked by func_802A1EA8.
    struct CVoiceMoveVTable {
        u8 pad[0x4C];
        void* (*fn_4C)(void* self);  // +0x4C
    };
    struct CVoiceMoveBase {
        CVoiceMoveVTable* vtable;    // +0x00
    };

    // Fake SI interface over the moveBase vtable so MWCC emits the real r12
    // dispatch idiom (`lwz r12,0(obj); lwz r12,off(r12); mtctr; bctrl`) instead
    // of colouring the vptr as a scratch r4.  vptr at +0 of the cast object;
    // fn_4C at vtable byte +0x4C = declared index 18 (1 hidden slot).
    struct CVoiceMoveIf {
        virtual void f00(); virtual void f01(); virtual void f02(); virtual void f03();
        virtual void f04(); virtual void f05(); virtual void f06(); virtual void f07();
        virtual void f08(); virtual void f09(); virtual void f0A(); virtual void f0B();
        virtual void f0C(); virtual void f0D(); virtual void f0E(); virtual void f0F();
        virtual void f10();
        virtual void* fn_4C();   // #18 => +0x4C (vptr + (N+1)*4)
    };

    // Chain rooted at CVoiceActorState +0x3F60 (battle-state object): the
    // block's +0x08 word points at a sub-block whose +0x18 signed word selects
    // the voice id (checked against [0x10, 0x17] by func_802A1C68).
    struct CVoiceF3F60Sub {
        u8 unk0[0x18];
        int field_18;   // +0x18 voice id
    };
    struct CVoiceF3F60Obj {
        u8 unk0[0x8];
        CVoiceF3F60Sub* field_8;   // +0x08
    };

    // Large CfObjectPc-derived actor with a flag word at +0x3F00 used by the
    // voice trigger functions (bit 30 battle/interact, bit 29 other-actor flag).
    struct CVoiceActorState {
        CVoiceActorVTable* vtable;   // +0x00
        u8 unk4[0x3E9C - 4];         // -> +0x3E9C
        CVoiceMoveBase moveBase;     // +0x3E9C (CfObjectMove block)
        u8 unk3EA0[0x3F00 - 0x3EA0]; // -> +0x3F00
        u32 field_3F00;   // flags word at +0x3F00
        u8 unk3F04[4];    // -> +0x3F08
        u32 field_3F08;   // +0x3F08 (bit 15 = 0x10000)
        u8 unk3F0C[0x1C]; // -> +0x3F28
        u16 field_3F28;   // +0x3F28
        u8 unk3F2A[0x36]; // -> +0x3F60
        CVoiceF3F60Obj* field_3F60; // +0x3F60
    };

    // Signed word at CBattleManager +0x194 (battle-participant count).
    struct CBattleCountAccessor {
        u8 unk0[0x194];
        int field_194;   // +0x194
    };

    // Ring-list node used by CBattleManager lists (next at +0x00).
    struct CVoiceBattleNode {
        CVoiceBattleNode* next;   // +0x00
    };

    // Accessor for CBattleManager return: list sentinel lives at +0x28.
    struct CBattleListAccessor {
        u8 unk0[0x28];
        CVoiceBattleNode* list0;   // +0x28
    };

    // CCharVoiceMan-side ring sentinel: CBattleManager +0x48.
    struct CBattleManagerNodeList {
        u8 unk0[0x48];
        CVoiceBattleNode* sentinel;   // +0x48
    };

    // Result of getCfObjectPc (a move-converted object); +0x3ED4 is a pointer
    // to an object holding a vtable whose +0x40 slot is probed with 0x800.
    struct CVoiceEdge;
    struct CVoiceBFC68 {
        u8 pad0[0x3ED4];
        CVoiceEdge* field_3ED4;   // +0x3ED4
    };
    struct CVoiceEdgeVTable {
        u8 pad[0x40];
        int (*fn_40)(void* self, int a);  // +0x40
    };
    struct CVoiceEdge {
        CVoiceEdgeVTable* vtable;   // +0x00
    };

    // Node returned by func_802A7A54; +0x04 is a pointer to an object whose
    // vtable +0x30 slot returns a pointer.
    struct CVoiceSrcInnerVTable {
        u8 pad[0x30];
        void* (*fn_30)(void* self);  // +0x30
    };
    struct CVoiceSrcInner {
        CVoiceSrcInnerVTable* vtable;   // +0x00
    };
    struct CVoiceSrcNode {
        u8 pad0[4];
        CVoiceSrcInner* field_4;   // +0x04
    };

    // Retail CCharVoiceMan vtable layout mirrors these declarations exactly:
    // dtor +0, FactoryEvent1 +4, FactoryEvent2 +8 (event id in r4),
    // FactoryEvent3 +0xC, FactoryEvent4 +0x10. The class is deliberately NOT
    // IFactoryEvent-derived: MWCC omits the base's pure-virtual slot, which
    // would otherwise push FactoryEvent2 to +0x10 instead of retail's +0x08.
    // novtable: the retail vtable is the shared-pool object lbl_eu_805398CC,
    // stored explicitly by the constructor (MWCC would otherwise emit its own
    // __vt__Q22cf13CCharVoiceMan symbol at the same slot positions).
    class __declspec(novtable) CCharVoiceMan {
    public:
        CCharVoiceMan();
        virtual ~CCharVoiceMan();
        virtual void FactoryEvent1();
        virtual void FactoryEvent2(int eventId);
        virtual void FactoryEvent3();
        virtual void FactoryEvent4();

        u8 unk4[0x200];
        u32 allocHead;        // +0x204 ring-buffer allocation head offset
        u32 headOffset;       // +0x208 nodeHead byte offset from &unk4[0]
        CSoundNode* nodeHead; // +0x20C head of voice-event list
        CSoundNode* nodeTail; // +0x210 tail of voice-event list
        u8 unk214;
        u8 unk215;
        u8 unk216[2]; //filler?
        float unk218;
        //Definitely classes of some kind. Maybe related to position?
        u32 unk21C;
        u8 unk220;
        u8 unk221;
        u8 fieldFlag;         // +0x222 field-id window [0x109,0x115] flag
        u8 autoTalkPending;   // +0x223 pending battle auto-talk flag
        u32 unk224;
        u8 unk228;
        u8 unk229;
        u8 unk22A;
        u8 unk22B;
        u32 unk22C;
        u32 unk230;
    };

}

// -- C-linkage imports (retail symbols) --------------------------------------
// CCharVoiceMan ctor symbol (placement construction of a heaped 0x234 buffer);
// kept external so the call site emits a real call rather than being inlined.
extern "C" cf::CCharVoiceMan* __ct__CCharVoiceMan(cf::CCharVoiceMan* self);
// Retail @sda21 reset float for unk218. const routes it into the readonly
// sdata2 pool, letting MWCC hoist the lfs above the frame stores (see
// kyoshin/CExchangeWin lfs-hoist rule).
extern "C" const float lbl_eu_80668C68;
// Retail @sda21 globals.
// Retail vtable data for CCharVoiceMan (shared data pool; stored explicitly
// by the ctor via *(void**)this).
extern "C" u8 lbl_eu_805398CC[28];
class CScn;
extern "C" u32 lbl_eu_80664A5C;    // global character-voice counter
// Scene-model helper returning an allocation-region handle (mtl MemManager).
extern "C" mtl::ALLOC_HANDLE func_80496004(void* src);
// Sibling voice-module free functions (cvsys TUs) - un-mangled retail names.
extern "C" int func_802B03A4(void* self);
extern "C" cf::CSoundNode* func_802A9604();
extern "C" cf::CSoundNode* __ct__802A4E48();
extern "C" cf::CSoundNode* __ct__802A96C0();
// Additional retail node constructors / helpers used by the enqueue functions.
extern "C" cf::CSoundNode* func_802A8B6C();
extern "C" cf::CSoundNode* func_802B5970();
extern "C" cf::CSoundNode* __ct__CVS_THREAD_ORDER(int order);
extern "C" cf::CSoundNode* __ct__802A8DE8();
extern "C" cf::CSoundNode* func_802A6958(void* actor);
extern "C" cf::CSoundNode* __ct__802A5830();
extern "C" cf::CSoundNode* __ct__802A8C04();
extern "C" cf::CSoundNode* func_802B8D4C();
// Node factory helpers for the frequency / level-up voice enqueue functions.
extern "C" cf::CSoundNode* func_802A3EF0();
extern "C" cf::CSoundNode* func_802AF3DC(void* self);
extern "C" cf::CSoundNode* func_802A5A14(void* a, void* c);
extern "C" cf::CSoundNode* __ct__802A7254(void* self, void* c);
extern "C" cf::CSoundNode* __ct__802A5ED4();
extern "C" cf::CSoundNode* func_802AF4FC(void* self, void* c);
extern "C" cf::CSoundNode* func_802AF43C(void* a, void* b, void* c);
extern "C" cf::CSoundNode* func_802A5B04(void* self, int flag);
extern "C" cf::CSoundNode* func_802AF49C(void* self);
// Per-object state flag lookup (tests a slot at self+0x8 against an id).
extern "C" int func_80148778(void* self, int id);
// MWCC member-function-pointer runtime helpers (compare / probe).
extern "C" int __ptmf_cmpr(void* a, void* b);
extern "C" long __ptmf_test(void* ptmf);
class CVS_THREAD;
// Reference member-function-pointer the voice nodes are matched against.
// Declared at full 12-byte ptmf size so MWCC treats it as ordinary .data
// (lis/addi @ha/@l like retail) instead of sda21-addressing a 4-byte extern.
extern "C" u8 lbl_eu_805398C0[12];
// Resolve a voice-action object from a voice source id (C-linkage).
extern "C" void* func_8016FE34(void* src);
// Play a specific character-voice sound (cvsys sibling; C-linkage retail name).
extern "C" void func_802AF9D0(void* r3, int voiceId, int r5);
// Unregister a factory event (destructor side; bare retail name 0x800B91FC).
extern "C" void func_800B88E0(void* self, cf::IFactoryEvent* event);
extern "C" cf::CSoundNode* func_802AF56C(void* c);
extern "C" int func_802A7870(void* arr, int capacity, int unused);
extern "C" cf::CSoundNode* __ct__802A92D8(void* b, void* a);
extern "C" void* func_802A3E88(void* node);
// Create a battle-voice source node from the battle-count delta (cvsys
// sibling). Takes (newCount, oldCount) in r3/r4 like retail.
extern "C" cf::CSoundNode* func_802A6820(int newCount, int oldCount);
// Pure voice nodes created with an int argument (1=levelled, 0=normal).
extern "C" cf::CSoundNode* __ct__802AF5CC(int a);
extern "C" cf::CSoundNode* __ct__802AB5F0();
extern "C" cf::CSoundNode* __ct__802A4870(int a);
// Battle-begin voice-thread constructor (cvsys sibling); called as
// (handle, actor) by func_802A216C.
extern "C" cf::CSoundNode* __ct__802AFA80(void* handle, void* actor);
// Convert a CfObjectMove to a battle-participant object.
extern "C" void* getCfObjectPc__FPQ22cf12CfObjectMove(void* self);
// Create a battle-voice node from a voice-action source.
extern "C" cf::CSoundNode* func_802B0344(void* src);
// Current battle/field phase id.
extern "C" u32 getQueuedFileEventCount__Q22cf13CfGameManagerFv(void);
// Create a battle-voice trigger node.
extern "C" void* func_802A7A54(int a);
// Run one node's level-thread handler (list sweep).
extern "C" void func_802A3ACC(void* node);
// Post-dispatch handler for a node whose +0x14 voice probe returned nonzero.
extern "C" void func_802A3E74(void* node);
// Check a just-resolved voice action against a source handle.
// Byte/word/shift @sda21 globals used by func_802A1610.
extern "C" u8  lbl_eu_80663DA0;
extern "C" u16 lbl_eu_80663E42;
extern "C" float lbl_eu_80668C6C;
extern "C" float lbl_eu_80668C70;
extern "C" float lbl_eu_80668C74;
extern "C" float lbl_eu_80668C78;
extern "C" float lbl_eu_80668C7C;
extern "C" cf::CSoundNode* func_802A6DF4();
extern "C" float lbl_eu_80668C80;  // 0x3F00-flag probability divisor
// CfGameManager singleton / player getter (retail pre-mangled C-ABI names).
extern "C" void* getInstance__Q22cf13CfGameManagerFv(void);
extern "C" void* getPlayer__Q22cf13CfGameManagerFi(int index);
#include "kyoshin/cf/CBattleManagerApi.hpp"
// Retail node constructors (cvsys TUs) - un-mangled retail names.
extern "C" cf::CSoundNode* __ct__CVS_THREAD_PARTY_GAGE();
extern "C" cf::CSoundNode* __ct__802A6AA8();
extern "C" cf::CSoundNode* __ct__802A5B88();
extern "C" cf::CSoundNode* __ct__802A86CC();
extern "C" cf::CSoundNode* __ct__802A6E84();
extern "C" cf::CSoundNode* func_802B9064();
extern "C" cf::CSoundNode* func_802A4798();
extern "C" cf::CSoundNode* func_802AE38C();
extern "C" cf::CSoundNode* func_802A7674();
extern "C" cf::CSoundNode* func_802A8AC8();
extern "C" cf::CSoundNode* func_802A7744();
extern "C" cf::CSoundNode* func_802A8628(void* self);