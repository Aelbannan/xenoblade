#pragma once

#include <types.h>

// C-linkage runtime imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void func_80261A80();
extern "C" float func_8048EA40();
extern "C" u8* getGlobalSda(void);
extern "C" void func_800599E0(void* sda, u32 a, u32 b, u32 c);
extern "C" void func_eu_8049AB50(u8* self, unsigned char byte);
extern "C" void func_8048EA38(float v);
// Sibling in this TU: CTaskREvtSequence::Term forwards `this` (retail name
// func_8016A480, unmangled -> C linkage).
extern "C" void func_8016A480(void* self);
// UI-state flag toggle used by func_801686B0 (defined in CInfoCf.cpp).
extern "C" void func_80166788();
// Sibling ctor in this TU (retail __ct__cf_CTaskREvtSequence, 0x218-byte
// body): takes (self, sequence-name string), returns self. The retail symbol
// is the pre-mangled name (a global function, not a cf::CTaskREvtSequence
// member); declared here so the definition keeps C linkage (CTaskREvent
// precedent).
namespace cf {
class CTaskREvtSequence;
}
extern "C" cf::CTaskREvtSequence* __ct__cf_CTaskREvtSequence(
    cf::CTaskREvtSequence* self, const char* name);
// func_80168484 (this unit) toggles a flag and fades BGM (menu sound system).
extern "C" void func_80168484(int a);
extern "C" void func_80189318(s32 clearName, float fadeTime);
extern "C" void func_80189424(float vol);

// Imports for func_80168610 (event-sequence boot): sequence-system / UI gates
// (C-linkage, retail unmangled names), CfGameManager helpers, the id getter
// and the sound-system flag arm.
extern "C" bool func_8012E6DC();
extern "C" int func_80062A00();
extern "C" void func_800866A0__Q22cf13CfGameManagerFv();
extern "C" void func_80086D98__Q22cf13CfGameManagerFv(u16* first, u16* second);
extern "C" u32 func_8016E08C();
extern "C" void func_800B9438(void* arg);
// Event-manager readiness check (defined at global scope in CTaskREvent.cpp,
// so C++ linkage mangles it back to the retail func_80164910__Fv).
u32 func_80164910();

// Name-string helper for realtime-event entries (CREvtModel.cpp): returns
// entry->field_0x1C + 0x10 as a C string.
extern "C" void* func_801727D0(void* self);
// CProcess base destructor (retail __dt__8CProcessFv).
class CProcess;
extern "C" void __dt__8CProcessFv(CProcess* self, int flag);
// Realtime-event arena allocator (CREvtMem.cpp): returns the chunk size.
extern "C" u32 func_80167D40(u32 a, u32 b);
extern "C" void* func_80167F6C(void* ptr, u32 alignment, int useMEM1);

// .sbss presentation/mode flag (co-op secondary mode bitfield)
extern u32 lbl_eu_80663E28;

// .sbss event-sequence flag word (bit 25 cleared by Term)
extern u32 lbl_eu_80663EE0;

// .sdata2 BGM fade volume used by func_80168484
extern f32 lbl_eu_80667658;

#include "monolib/scn/CScn.hpp"
#include "monolib/scn/IScnRender.hpp"
// ResAnmChr is returned by value (single pointer word) by func_8049E708.
#include "nw4r/g3d/res/g3d_resanmchr.h"
extern CScn* lbl_eu_80663E14; // scene object

// Move-callback ptmf table copied into CTaskREvtSequence::field_0x3C by Init
// (retail .data:0x80530AF4, size 0x18; only the first 3 words are read).
extern u32 lbl_eu_80530AF4[6];

// ptmf table copied into field_0x3C by func_801686B0 (retail .data:0x80530A4C)
extern u32 lbl_eu_80530A4C[3];

// ptmf table copied into field_0x3C by func_80168610 (retail .data:0x80530A40)
extern u32 lbl_eu_80530A40[6];

// ptmf table copied into field_0x3C by func_8016872C (retail .data:0x80530A58)
extern u32 lbl_eu_80530A58[6];

// String pool used by func_8016B788: name string at +0x5D passed to
// func_8016AF4C (retail .data:0x80503098).
extern char lbl_eu_80503098[];

// Retail CfGameManager helper: the retail mangled name is Fv, but the call
// site passes the halfword pair + 0 (declared with the caller shape, same
// scheme as CTaskREvent.hpp's CfGameManager helpers).
extern "C" void func_80086B5C__Q22cf13CfGameManagerFv(int a, int b, int c);

// Imports for the event-sequence batch targets (flat retail names, declared
// per this header's existing C-linkage pattern).
// Forward decls so the extern "C" imports below can name the return/arg
// types before their full definitions.
struct EvtSeqVec4;
struct EvtSeqMgrView;
struct UnkEvtPlayer;
class CfEvtCamManager;
class UnkEvtListEntry;  // full layout below (UnkState_80664268::field_0x10C)
// func_8016DF4C: reload-count selector (CfResReloadImpl.cpp).
extern "C" void func_8016DF4C(u32 type);
// func_8049602C: scene vec4 setter - declared once on kyoshin/CTaskGame.hpp
// (flat retail name); TUs here reach it via that header.

// (CREvtCamera.cpp / CTaskREvent.cpp).
extern "C" u32 func_801644B4();
extern "C" void func_80164CFC();
// func_80168028: realtime-event arena slot lookup (CREvtMem.cpp) - returns an
// arena address used as a plain integer by func_80169DD0.
extern "C" u32 func_80168028(u32 idx);
// func_802A1500: voice-event manager reset (CCharVoiceMan.cpp).
extern "C" void func_802A1500();
// CfGameManager helpers with the caller shape (same scheme as CTaskREvent.hpp).
extern "C" EvtSeqMgrView* func_80086B04__Q22cf13CfGameManagerFv();
extern "C" u32 func_8007DE94__Q22cf13CfGameManagerFv(u32 index, u32 value);
// Camera-manager getter: the CfGameManager.hpp member is declared non-static,
// but the retail call site passes no `this` (Fv), so declare the mangled
// symbol as a free function (same scheme as CTaskREvent.hpp).
extern "C" CfEvtCamManager* func_800821F8__Q22cf13CfGameManagerFv();
// func_800AA714: path -> packed id (code_800AA008.cpp; flat retail name).
extern "C" u32 func_800AA714(const char* path);
// func_8049EB60: player/event-object refresh (CfCam.hpp declares the same
// symbol with a void* shape).
extern "C" bool func_8049EB60(UnkEvtPlayer* obj);

// Imports for func_80169F28 (event-sequence dispatch): the four realtime-event
// object setup helpers (kyoshin/realtimeevt/*.cpp). The third argument is the
// address of the id-list element (the callees store it into +0x43C/+0x1C
// etc.).
struct CREvtCamera;
struct CREvtEffect;
struct CREvtLight;
struct CREvtMovie;
struct CREvtMovieScript;
struct CREvtParam;
extern "C" void func_80180664(CREvtCamera* self, void* eventData,
                              void* elemAddr);
extern "C" void func_80185378(CREvtEffect* self, void* src, void* elemAddr);
extern "C" void func_801C36C4(CREvtLight* self, const char* resourceName,
                              u32 elemAddr);
extern "C" void func_80294BA4(CREvtMovie* self);
// Imports for func_80168800 (event-sequence setup): realtime-event arena
// allocator + object ctors (kyoshin/realtimeevt/*.cpp), the name-matching
// helper (CREvtModel.cpp), CfGameManager gates and the vision-arm pair
// (CMenuVision.cpp).
extern "C" void* func_80185748(void* size);
extern "C" CREvtCamera* __ct__CREvtCamera(CREvtCamera* self, u32 param);
extern "C" CREvtEffect* __ct__CREvtEffect(CREvtEffect* self,
                                          CREvtParam* param);
extern "C" CREvtLight* __ct__CREvtLight(CREvtLight* self, u32 arg);
extern "C" CREvtMovie* __ct__CREvtMovie(CREvtMovie* self,
                                        CREvtMovieScript* scriptData);
extern "C" void* func_801730D0(void* self);
extern "C" bool func_80087250__Q22cf13CfGameManagerFv();
extern "C" void func_8007CE94__Q22cf13CfGameManagerFv();
extern "C" u32 func_801AC088();
extern "C" void func_801AC1F8();
extern "C" void func_80294980(CREvtMovie* self);

// Imports for func_8016ABA8 (async file-event handler): scene dim helper
// (CfObjectImplWalker.cpp) and the menu tag-processor (re)init pair
// (code_8025FB10.cpp).
extern "C" u8* func_8049627C(CScn* scene, int flag);
extern "C" void func_802618D8(u8* arg);
extern "C" void func_80261944(int arg);
// Imports for func_80169050 (fade/arm sequence state): CfGameManager helpers
// (caller shape - retail symbols are Fv) and the voice/UI-system arm helper
// (code_801A929C.cpp).
extern "C" void func_8007C0F8__Q22cf13CfGameManagerFv();
extern "C" void func_8007FE1C__Q22cf13CfGameManagerFv(int a, int b);
extern "C" void func_801AAC78(int v);
// Imports for the ctor: menu/event-system singleton gate + arm
// (CMenuEnemyState.cpp).
extern "C" void* func_80110A70();
extern "C" void func_80111074();
// Character-anim resource lookup (CScnItemAnim.cpp): returns a single-word
// ResAnmChr wrapper; func_8016AF4C / func_8016B5A4 walk it.
extern "C" nw4r::g3d::ResAnmChr func_8049E708(u8* data, int index);
// ResFile entry-count getter called on the raw file base + 0xC (retail emits
// addi+bl with no ResFile object spill; CREvtModel precedent).
extern "C" u32 GetResAnmChrNumEntries__Q34nw4r3g3d7ResFileCFv(u8* resFile);

// .sdata2 floats used by func_80169CD0 (fade vec, camera/player values).
extern f32 lbl_eu_80667668;
extern f32 lbl_eu_8066766C;
extern f32 lbl_eu_80667670;
extern f32 lbl_eu_80667674;
extern f32 lbl_eu_80667678;
extern f32 lbl_eu_8066767C;

// ptmf table copied into field_0x3C by func_80168F38 (retail .data:0x80530A7C).
extern u32 lbl_eu_80530A7C[3];

// ptmf tables copied into field_0x3C by func_80169050 (retail .data:
// 0x80530A88 / 0x80530A94).
extern u32 lbl_eu_80530A88[3];
extern u32 lbl_eu_80530A94[3];

// ptmf tables copied into field_0x3C by func_80168800 (retail .data:
// 0x80530A64 / 0x80530A70).
extern u32 lbl_eu_80530A64[3];
extern u32 lbl_eu_80530A70[3];

// Vtable block at retail .data:0x80530BF4 (interim, overwritten by the ctor)
// and 0x80530B0C (final CTTask/IScnRender block written by the ctor: primary
// at +0x10, secondary sub-vtables at +0x24 and +0xAC).
extern u32 lbl_eu_80530BF4[];
extern u32 lbl_eu_80530B0C[];

// Panic file/format strings used by func_8016AF4C / func_8016B5A4
// (retail .data:0x80530C..).
extern char lbl_eu_80530D18[];
extern char lbl_eu_80530CFC[];
extern char lbl_eu_80530CF0[];
extern char lbl_eu_80530F08[];
extern char lbl_eu_80530EE0[];
extern char lbl_eu_80530D54[];
extern char lbl_eu_80530D2C[];
extern char lbl_eu_80530DC4[];
extern char lbl_eu_80530DA8[];
extern char lbl_eu_80530D68[];
extern char lbl_eu_80530ECC[];
extern char lbl_eu_80530E88[];
extern char lbl_eu_80530E1C[];
extern char lbl_eu_80530DD8[];
extern char lbl_eu_80530E74[];
extern char lbl_eu_80530E30[];
// .sdata string extension marker used by func_8016C450's filename strip
// (retail .sdata:0x806623C0, size 2).
extern char lbl_eu_806623C0[2];
extern char lbl_eu_80530D94[];
extern char lbl_eu_80530D78[];
// .sbss type-name strings passed as nw4r Panic varargs (fixed size keeps the
// SDA21 reloc, CfBdat.hpp precedent).
extern char lbl_eu_806623C4[4];
extern char lbl_eu_806623C8[4];
extern char lbl_eu_806623CC[4];

// .sbss event-window halfword toggled by func_80169050 (bit 0).
extern u16 lbl_eu_806642E0;

// .sdata2 BGM volume used by func_80169050's fade calls.
extern f32 lbl_eu_8066765C;

// 4-float stack blob passed to func_8049602C by func_80169CD0 (fade vec).
struct EvtSeqVec4 {
    f32 x, y, z, w;
};

// Object behind CScn+0x5C: float written at +0xD4 by func_80169CD0.
struct UnkScn5C {
    u8 gap00[0xD4];   // 0x00
    f32 field_0xD4;   // 0xD4
};

// Layout view of CScn exposing the +0x5C pointer (func_80169CD0 writes
// through it). Mirrors the monolib CScn head (CTTask + IWorkEvent).
struct EvtSeqScnView {
    u8 gap00[0x5C];       // 0x00
    UnkScn5C* field_0x5C; // 0x5C
};

// Player/event object at UnkClass_800821F8::field_0xC: float pair at
// +0x1EC/+0x1F0 written by func_80169CD0.
struct UnkEvtPlayer {
    u8 gap00[0x1EC];   // 0x00
    f32 field_0x1EC;   // 0x1EC
    f32 field_0x1F0;   // 0x1F0
};

// Camera/event manager returned by CfGameManager::func_800821F8 (the retail
// object behind the forward-declared UnkClass_800821F8; also modelled as
// CREvtCamManager in CREvtCamera.hpp). The retail vtable slot at +0x3C is
// user virtual 13.
class CfEvtCamManager {
public:
    virtual void vf_0x08();
    virtual void vf_0x0C();
    virtual void vf_0x10();
    virtual void vf_0x14();
    virtual void vf_0x18();
    virtual void vf_0x1C();
    virtual void vf_0x20();
    virtual void vf_0x24();
    virtual void vf_0x28();
    virtual void vf_0x2C();
    virtual void vf_0x30();
    virtual void vf_0x34();
    virtual void vf_0x38();
    virtual void vf_0x3C(f32 value);  // +0x3C: set float
    u8 gap04[0x08];                   // 0x04-0x0B
    UnkEvtPlayer* field_0xC;          // 0x0C: player object pointer
};

// Pointer at CTaskREvtSequence::field_0x120: walk header of the loaded
// sequence buffer. The word offsets at +0x14..+0x28 locate the entry table
// and the four id lists inside the buffer (func_80169F28); func_80169DD0
// advances the cursor by the +0x28 offset.
struct UnkSeq120 {
    u8 gap00[0x14];   // 0x00
    u32 field_0x14;   // 0x14: offset to the 0x14-byte entry table
    u32 field_0x18;   // 0x18: offset to id-list A (+0xC data start)
    u32 field_0x1C;   // 0x1C: offset to id-list B (+0xC data start)
    u32 field_0x20;   // 0x20: offset to id-list C (+0xC data start)
    u32 field_0x24;   // 0x24: offset to id-list D (+0xC data start)
    u32 field_0x28;   // 0x28: walk cursor advance (func_80169DD0)
};

// Object behind the container +0x4594/+0x4598 pointers (func_8016C118): the
// vtable+0x18 slot (user 4) returns a name string fed to func_800AA714.
class UnkObj4594 {
public:
    virtual void vf_0x08();
    virtual void vf_0x0C();
    virtual void vf_0x10();
    virtual void vf_0x14();
    virtual const char* vf_0x18();  // +0x18: name/path string
};

// Character/effect container behind the manager list nodes: node->field_0x8
// points +0x3E9C into this object; the two model objects at +0x4594/+0x4598.
struct UnkContainer4594 {
    u8 gap00[0x4594];          // 0x00
    UnkObj4594* field_0x4594;  // 0x4594
    UnkObj4594* field_0x4598;  // 0x4598
};

// Node of the circular object list headed by CfGameManager::field_0x4
// (walked by func_8016C118).
struct UnkNode4594 {
    UnkNode4594* field_0x0;   // 0x00: next node
    u8 gap04[0x04];           // 0x04
    UnkObj4594* field_0x8;    // 0x08: sub-object pointer (+0x3E9C into container)
};

// View of the CfGameManager object returned by func_80086B04: object list
// head at +0x4.
struct EvtSeqMgrView {
    u8 gap00[0x04];         // 0x00
    UnkNode4594* field_0x4; // 0x04
};

// Entry of the 0x14-byte-stride table pointed to by UnkState_80664268::field_0xE4
struct UnkStateEntry_E4 {
    u8 gap00[0x0C];   // 0x00
    u32 field_0x0C;   // 0x0C
    u8 gap10[0x04];   // 0x10
};  // size 0x14

// Element of the table pointed to by UnkState_80664268::field_0xD0.
// func_8016A378 walks it: entry index = field_0xF8, byte stride = field_0x4,
// then reads the word at +0xC of the selected entry.
struct UnkStateTable_D0 {
    u8 gap00[0x04];   // 0x00
    u32 field_0x4;    // 0x04: byte stride between entries
    u8 gap08[0x04];   // 0x08
    u32 field_0xC;    // 0x0C: count word read by func_8016A378
    u32 field_0x10;   // 0x10: size word read by func_80169DD0
    u32 field_0x14;   // 0x14: size word read by func_80169DD0
    u8 gap18[0x20];   // 0x18-0x37
    u32 field_0x38;   // 0x38: flag word (bits 0/1/3 read by func_80169F28)
    f32 field_0x3C;   // 0x3C: fade volume compared by func_80169F28
};  // size 0x40

// Object whose address is stored in UnkState_80664268::field_0xC4;
// func_8016C3DC reads its +0x4C word.
struct UnkStateC4 {
    u8 gap00[0x4C];   // 0x00
    u32 field_0x4C;   // 0x4C
};  // size 0x50

// Realtime-event object base (func_80169F28 reads the name-data id word at
// +0x43C after a camera setup call and stores it into the table entry).
struct EvtSeqObj43C {
    u8 gap00[0x43C];
    u32 field_0x43C;
};

// Scene view exposing the +0x7C pointer whose target word is OR'd/AND'd by
// func_80169F28 (driven by bits 0/1 of the entry flag word).
struct EvtSeqScn7C {
    u8 gap00[0x7C];
    u32* field_0x7C;
};

// Object whose address is cached in the .sbss pointer lbl_eu_80664268;
// func_8016A35C / func_8016A3A8 / func_8016A3C4 read its words.
struct UnkState_80664268 {
    u8 gap00[0x5C];                  // 0x00
    u32 field_0x5C;                  // 0x5C: flag word (bits 9/10/19 read below)
    u8 gap60[0x44];                  // 0x60
    u32* field_0xA4;                 // 0xA4: realtime-event list base (array of entries)
    u32 field_0xA8;                  // 0xA8: walk limit (end clamped to this by func_8016AD44)
    u8 gapAC[0x18];                  // 0xAC
    u32 field_0xC4;                  // 0xC4: base address added to entry offset
    u8 gapC8[0x08];                  // 0xC8
    UnkStateTable_D0* field_0xD0;    // 0xD0: table walked by func_8016A378
    u8 gapD4[0x10];                  // 0xD4
    UnkStateEntry_E4* field_0xE4;    // 0xE4: 0x14-stride table (0 when absent)
    u8 gapE8[0x10];                  // 0xE8
    u32 field_0xF8;                  // 0xF8
    u32 field_0xFC;                  // 0xFC
    u32 field_0x100;                 // 0x100
    u32 field_0x104;                 // 0x104
    u8 gap108[0x04];                 // 0x108-0x10B
    UnkEvtListEntry** field_0x10C;   // 0x10C: realtime-event entry pointer list
    s32 field_0x110;                 // 0x110: entry count (signed walk bound)
    u8 field_0x114;                  // 0x114 (byte gate read/stored by func_80168514)
    u8 gap115[0x01];                 // 0x115
    u16 field_0x116;                 // 0x116 (halfword read by func_8016841C)
    u16 field_0x118;                 // 0x118
    u8 gap11C[0x20];                 // 0x11C-0x13B
    u8 field_0x13C[8];               // 0x13C: per-state-index byte counters
                                     // (indexed by field_0xF8-range indices,
                                     // bumped by func_8016BDA8)
};
extern UnkState_80664268* lbl_eu_80664268;

// Minimal layout view of the realtime-event model object passed to
// func_8016BDA8 (type word 0x14, flag word 0x18, data pointer 0x1C).
struct EvtBdabModel {
    u8 gap00[0x14];   // 0x00
    u32 field_0x14;   // 0x14: event type word (+6 vs +1 counter bump)
    u32 field_0x18;   // 0x18: flag word (bit 8 selects the packed-slot path)
    void* field_0x1C; // 0x1C: id/slot table (EvtBdabPtr1C)
};

// Readiness probe called by func_8016BDA8 (retail resolves it as the flat
// symbol func_801729D0).
extern "C" int func_801729D0(EvtBdabModel* self);

// View of the buffer at UnkState_80664268::field_0xC4 exposing the slot
// table base word at +0x80 (func_8016BDA8 compares slot ids against it).
struct EvtSeqC4View80 {
    u8 gap00[0x80];   // 0x00
    u32 field_0x80;   // 0x80
};

// Object at CREvtModel::mPtr1C (func_8016BDA8): id word at +0x30, eight
// halfword event slots at +0x38, flag word at +0x58.
struct EvtBdabPtr1C {
    u8 gap00[0x30];      // 0x00
    u32 field_0x30;      // 0x30: claimed id word
    u16 field_0x38[8];   // 0x38: halfword slots (low byte = packed id)
    u16 gap48[0x08];     // 0x48-0x57
    u32 field_0x58;      // 0x58: flag word (bit 9 read by func_8016BDA8)
};

// Layout view of CTaskREvtSequence exposing the IWorkEvent secondary base at
// +0x54 (CTTask head 0x00-0x54, then IWorkEvent). func_801686B0 uses it to
// reproduce the retail null-guarded upcast when passing the callback object.
struct EvtSeqHead54 {
    u8 gap00[0x54];
};
struct EvtSeqWithEvt : public EvtSeqHead54, public IWorkEvent {};

// Object behind UnkState_80664268::field_0xC4: realtime-event walk bounds at
// +0x20 (count base) and +0x28 (start index / run length).
struct UnkStateC4Obj {
    u8 gap00[0x20];   // 0x00
    u32 field_0x20;   // 0x20 (id-list A count, func_80169F28 loop 2)
    u32 field_0x24;   // 0x24 (id-list B count, func_80169F28 loop 3)
    u32 field_0x28;   // 0x28 (start index / id-list C count, func_80169F28 loop 1)
    u8 gap2C[0x08];   // 0x2C-0x33
    u32 field_0x34;   // 0x34 (id-list D count, func_80169F28 loop 4)
    u8 gap38[0x04];   // 0x38-0x3B
    u32 field_0x3C;   // 0x3C (inner-loop bound read by func_8016BB38)
    u8 gap40[0x08];   // 0x40-0x47
    u32 field_0x48;   // 0x48 (final list count, func_80169F28 loop 5)
};  // size 0x4C

// Name-data block at UnkEvtListEntry::field_0x1C: id word at +0x0C (compared
// against CBdatEntry mNameData by func_8016A27C), name string at +0x10
// (func_801727D0 returns field_0x1C + 0x10).
struct UnkEvtNameData {
    u8 gap00[0x0C];      // 0x00
    u32 field_0x0C;      // 0x0C
    char mName[0x20];    // 0x10
    u32 field_0x30;      // 0x30 (priority word compared by func_8016BC1C)
    u8 gap34[0x1C];      // 0x34
};  // size 0x50

// Realtime-event list entry (UnkState_80664268::field_0xA4): vtable at +0x0
// with the resolve slot at absolute offset 0x14 (MWCC vtables carry a 2-word
// offset-to-top/RTTI header, so that is user-virtual index 3), type word at
// +0x14, name-data pointer at +0x1C (same first 0x20 bytes as CREvtModel).
class UnkEvtListEntry {
public:
    virtual void* vf_0x08(int flag);   // user 0 -> vtable+0x08: release/stop
    virtual void* vf_0x0C();   // user 1 -> vtable+0x0C
    virtual void* vf_0x10();   // user 2 -> vtable+0x10
    virtual void* vf_0x14();   // user 3 -> vtable+0x14: resolve the name object
    virtual void* vf_0x18();   // user 4 -> vtable+0x18 (func_80168F38 busy probe)
    virtual void* vf_0x1C();   // user 5
    virtual void* vf_0x20();   // user 6
    virtual void* vf_0x24();   // user 7 -> vtable+0x24 (func_80168F38 walk call)
    virtual void* vf_0x28();   // user 8
    virtual void* vf_0x2C();   // user 9 -> vtable+0x2C (func_80169DD0 match call)
    virtual void* vf_0x30();            // user 10 -> vtable+0x30
    virtual void* vf_0x34(void* arg, void* elem);  // user 11 -> vtable+0x34 (func_80169F28 loop 2 call)
    virtual void* vf_0x38();   // user 12 -> vtable+0x38
    virtual void* vf_0x3C();   // user 13 -> vtable+0x3C (func_8016A480 stop call)
    u8 gap04[0x10];            // 0x04-0x13
    s32 field_0x14;            // 0x14: type word (3 = realtime event)
    u32 field_0x18;            // 0x18: flag word (bit 7 read by func_8016A480)
    UnkEvtNameData* field_0x1C; // 0x1C: name-data pointer
    u8 gap20[0x1C];            // 0x20-0x3B
    u8 field_0x3C;             // 0x3C: byte gate (func_8016BB38 returns when 0)
    u8 gap3D[0x03];            // 0x3D-0x3F
    u32 field_0x40;            // 0x40: busy flag (tested by func_8016ADF8)
    u8 gap44[0x04];            // 0x44-0x47
    u32 field_0x48;            // 0x48: busy flag (tested by func_8016ADF8)
};  // size 0x4C

// Object returned by UnkEvtListEntry::vf_0x14() on success; its vtable slot
// at absolute offset 0xA8 (user-virtual index 40) yields the final name
// object (only that slot is dispatched here).
class UnkEvtResolved {
public:
    virtual void* vf_0x08();
    virtual void* vf_0x0C();
    virtual void* vf_0x10();
    virtual void* vf_0x14();
    virtual void* vf_0x18();
    virtual void* vf_0x1C();
    virtual void* vf_0x20();
    virtual void* vf_0x24();
    virtual void* vf_0x28();
    virtual void* vf_0x2C();
    virtual void* vf_0x30();
    virtual void* vf_0x34();
    virtual void* vf_0x38();
    virtual void* vf_0x3C();
    virtual void* vf_0x40();
    virtual void* vf_0x44();
    virtual void* vf_0x48();
    virtual void* vf_0x4C();
    virtual void* vf_0x50();
    virtual void* vf_0x54();
    virtual void* vf_0x58();
    virtual void* vf_0x5C();
    virtual void* vf_0x60();
    virtual void* vf_0x64();
    virtual void* vf_0x68();
    virtual void* vf_0x6C();
    virtual void* vf_0x70();
    virtual void* vf_0x74();
    virtual void* vf_0x78();
    virtual void* vf_0x7C();
    virtual void* vf_0x80();
    virtual void* vf_0x84();
    virtual void* vf_0x88();
    virtual void* vf_0x8C();
    virtual void* vf_0x90();
    virtual void* vf_0x94();
    virtual void* vf_0x98();
    virtual void* vf_0x9C();
    virtual void* vf_0xA0();
    virtual void* vf_0xA4();
    virtual void* vf_0xA8();   // user 40 -> vtable+0xA8: final name object
};

// File-header object at CTaskREvtSequence::field_0xC0: size word at +0x18,
// flags halfword at +0x46 (both read by func_8016872C).
struct UnkFileHeader {
    u8 gap00[0x18];   // 0x00
    u32 field_0x18;   // 0x18
    u8 gap1C[0x2A];   // 0x1C-0x45
    u16 field_0x46;   // 0x46
};  // size 0x48

// Table at cf::CTaskREvtSequence::field_0xF0 (walked by func_8016BB38):
// byte stride at +0x4; each entry has a byte at +0x9, a signed halfword at
// +0xA and a name string at +0xC.
struct UnkBB38Table {
    u8 gap00[0x04];    // 0x00
    u32 field_0x4;     // 0x04: byte stride between entries
    u8 gap08[0x01];    // 0x08
    u8 field_0x9;      // 0x09
    s16 field_0xA;     // 0x0A
    char mName[0x40];  // 0x0C
};  // size 0x4C

// Type-2 event entry name object (UnkEvtListEntry::field_0x1C): packed
// token word at +0x20 passed to func_800AA318 by func_8016C300.
struct UnkEvtName2 {
    u8 gap00[0x20];    // 0x00
    u32 field_0x20;    // 0x20
};  // size 0x24

// Object behind UnkObj80168514::field_0x1C: id word at +0x34 (tested against
// 0xFFFFFFFF) and packed halfword at +0x38 (bits 8-15 compared against
// UnkState_80664268::field_0xF8).
struct UnkObj1C80168514 {
    u8 gap00[0x34];    // 0x00
    u32 field_0x34;    // 0x34
    u16 field_0x38;    // 0x38
};  // size 0x3A

// Object passed to func_80168514: standard MWCC vtable at +0x0 (2-word
// header, first user virtual at +0x08), flag word at +0x18, data object at
// +0x1C. The dispatched slots are user virtual 13 (vtable+0x3C, returns a
// tested value) and user virtual 14 (vtable+0x40).
class UnkObj80168514 {
public:
    virtual void vf_0x08();   // user 0
    virtual void vf_0x0C();   // user 1
    virtual void vf_0x10();   // user 2
    virtual void vf_0x14();   // user 3
    virtual void vf_0x18();   // user 4
    virtual void vf_0x1C();   // user 5
    virtual void vf_0x20();   // user 6
    virtual void vf_0x24();   // user 7
    virtual void vf_0x28();   // user 8
    virtual void vf_0x2C();   // user 9
    virtual void vf_0x30();   // user 10
    virtual void vf_0x34();   // user 11
    virtual void vf_0x38();   // user 12
    virtual int vf_0x3C();    // user 13 -> vtable+0x3C
    virtual void vf_0x40();   // user 14 -> vtable+0x40
    u8 gap04[0x14];           // 0x04-0x17
    u32 field_0x18;           // 0x18
    UnkObj1C80168514* field_0x1C;  // 0x1C
};  // size 0x20

// Object destructed by __dt__801682AC: memory-block wrapper with a data
// buffer (0x0), flag word (0x4), spare word (0x8) and ALLOC_HANDLE (0xC,
// 0xFFFFFFFF = invalid).
struct UnkBlock801682AC {
    u8* field_0x0; // 0x00 (opaque data buffer)
    u32 field_0x4; // 0x04
    u32 field_0x8; // 0x08
    u32 field_0xC; // 0x0C (ALLOC_HANDLE; -1 = invalid)
};

// File-event view consumed by func_8016ABA8 (async OnFileEvent handler): the
// shared monolib CEventFile only declares the head/tail, so expose the mid
// words (type, handle, two argument words, size/flag word).
struct EvtSeqFileEvent {
    s32 field_00;           // +0x00 event type (1 = data loaded)
    u32 field_04;           // +0x04 owning file handle (compared to 0xB8/0xBC/0xFC)
    u8 field_08[0xC - 0x08];
    u32 field_0C;           // +0x0C (arg0 passed to func_8016C450)
    u32 field_10;           // +0x10 (arg1 passed to func_8016C450)
    u32 field_14;           // +0x14 (size/flag compared against the language entry)
};

// Layout view of CTaskREvtSequence exposing the IScnRender secondary base at
// +0x58 (func_8016ABA8 registers it as a scene render callback).
struct EvtSeqHead58 {
    u8 gap00[0x58];
};
struct EvtSeqWithRender : public EvtSeqHead58, public IScnRender {};

// View of the field_0xC4 buffer (loaded sequence data): walk limit at +0x38,
// flags halfword at +0x44, per-language 8-byte entries at +0x50.
struct EvtSeqLangEntry {
    u32 field_0x0;   // 0x00
    u32 field_0x4;   // 0x04
};
// Entry of the type-tagged walk tables inside the sequence buffer
// (CTaskREvtSequence::field_0xC4): type word at +0, byte stride at +4; a -1
// type terminates the walk. The type-2/0xb/0xe tables are also viewed as
// UnkStateTable_D0 / UnkE8Table / UnkBB38Table respectively.
struct EvtSeqWalkEntry {
    s32 field_0x0;   // 0x00: entry type (-1 = terminator)
    u32 field_0x4;   // 0x04: byte stride to the next entry
};

// Sequence-buffer view for func_80168800: +0x8 locates the first walk
// entry; +0x20..+0x48 hold the per-kind entry counts; +0x4C the flags word.
struct EvtSeqWalkBuf {
    u8 gap00[0x8];    // 0x00
    u32 field_0x8;    // 0x08: offset to the first walk entry
    u8 gap0C[0x14];   // 0x0C-0x1F
    u32 field_0x20;   // 0x20: type-3 (model/effect) count
    u32 field_0x24;   // 0x24
    u32 field_0x28;   // 0x28: camera count
    u8 gap2C[0x08];   // 0x2C-0x33
    u32 field_0x34;   // 0x34: light count
    u8 gap38[0x10];   // 0x38-0x47
    u32 field_0x48;   // 0x48: movie count
    u32 field_0x4C;   // 0x4C: flags word
};  // size 0x50

// Object behind CTaskREvtSequence::field_0xCC (type-1 walk entry): the
// float at +0x10 is compared against the fade volume by func_80168800.
struct EvtSeqCC {
    u8 gap00[0x10];   // 0x00
    f32 field_0x10;   // 0x10
};

// Type-3 walk entry (CTaskREvtSequence::field_0xD4): name string at +0x10
// (matched/replaced by func_80168800), packed id at +0x20 and type word at
// +0x28.
struct EvtSeqType3Entry {
    u8 gap00[0x10];   // 0x00
    char mName[0x10]; // 0x10: name string
    u32 field_0x20;   // 0x20: packed id written on rename
    u32 field_0x28;   // 0x28: type word (3)
};  // size 0x2C

struct EvtSeqC4Buf {
    u8 gap00[0x38];                 // 0x00
    u32 field_0x38;                 // 0x38: inner-loop bound / walk limit
    u8 gap3C[0x08];                 // 0x3C-0x43
    s16 field_0x44;                 // 0x44: color/type halfword
    u8 gap46[0x0A];                 // 0x46-0x4F
    EvtSeqLangEntry field_0x50[8];  // 0x50: per-language entries
};

// Table at CTaskREvtSequence::field_0xE8 (walked by func_80169050 and
// func_8016B860): byte stride at +0x4; entries have an id halfword at +0x8, a
// type byte at +0xA, a signed duration halfword at +0xC, a gate halfword at
// +0xE and a signed value byte at +0x10 (data area starts at +0x10).
struct UnkE8Table {
    u8 gap00[0x04];   // 0x00
    u32 field_0x4;    // 0x04: byte stride between entries
    u16 field_0x8;    // 0x08: entry id (matched against field_0x104)
    u8 field_0xA;     // 0x0A: entry type (1/2/3/5/7 handled by func_8016B860)
    u8 gap0B;         // 0x0B
    s16 field_0xC;    // 0x0C: signed duration (frames when negative)
    u16 field_0xE;    // 0x0E: gate halfword (type-2 entries)
    s8 field_0x10;    // 0x10: signed value (type-5 entries)
};

// CFileHandle word at +0x8 read by func_8016ABA8 (CX stream size/state).
struct EvtSeqHandle8 {
    u8 gap00[0x08];
    u32 field_0x8;
};

// ResDic lookup result view (func_8016AF4C / func_8016B164 / func_8016B5A4):
// data offset at +0x4, a word at +0x8 (published by func_8016B164 into its
// second output) and the entry-type word at +0xC.
struct EvtSeqResEntry {
    u8 gap00[0x04];
    u32 field_0x4;   // +0x4: relative offset to the entry data
    u32 field_0x8;   // +0x8: word stored to *out2 by func_8016B164
    u32 field_0xC;   // +0xC: entry type word
};

// Word loaded through the resolved entry data by func_8016AF4C.
struct EvtSeqIdWord {
    u32 field_0x0;
};

// Buffer encrypted in place by func_8016C450 (retail arg1): the cipher writes
// bytes at [buf, buf->field_0x10 + 0x800).
struct EvtC450Buf {
    u8 gap00[0x10];
    u8* field_0x10;  // +0x10: end-of-buffer marker (end = field_0x10 + 0x800)
};

namespace cf {

class CTaskREvtSequence {
public:
    u8 gap00[0x10];
    u32 field_0x10;  // 0x10 (CTTask/CProcess vtable written by the ctor)
    u8 gap14[0x0C];  // 0x14-0x1F
    u32 field_0x20;  // at 0x20, accessed by func_8016A354
    u8 gap24[0x18];  // 0x24-0x3C
    u32 field_0x3C;  // 0x3C (CTTask mMoveFunc slot; Init installs a ptmf here)
    u32 field_0x40;  // 0x40
    u32 field_0x44;  // 0x44
    u32 field_0x48;  // 0x48 (mDrawFunc ptmf slot 0)
    u32 field_0x4C;  // 0x4C (mDrawFunc ptmf slot 1)
    u32 field_0x50;  // 0x50 (mDrawFunc ptmf slot 2)
    u32 mEvt54[2];    // 0x54-0x5B (IWorkEvent sub-object head; readFile target)
    u32 field_0x5C;   // 0x5C (flag word; bits 2/4 handled by func_80169DD0)
    char mPath[0x40]; // 0x60-0xA0 (sequence name buffer, FixStr<64> head)
    u32 field_0xA0;   // 0xA0 (name length stored by the ctor)
    // Embedded memory block (UnkBlock801682AC layout) cleaned up by the dtor.
    UnkEvtListEntry** field_0xA4; // 0xA4 (realtime-event list; freed by the dtor)
    u32 field_0xA8;   // 0xA8 (walk limit / count)
    u32 field_0xAC;   // 0xAC (spare word)
    u32 field_0xB0;   // 0xB0 (ALLOC_HANDLE; -1 = invalid)
    u32 field_0xB4;   // 0xB4
    u32 field_0xB8;   // 0xB8 (CFileHandle* stored by func_801686B0)
    u32 field_0xBC;   // 0xBC (CFileHandle* from readCommonArchiveFile)
    u32 field_0xC0;   // 0xC0 (file-header object; freed by func_8016872C)
    u32 field_0xC4;   // 0xC4 (newly allocated buffer, read into by func_8016872C)
    u32 field_0xC8;   // 0xC8
    u32 field_0xCC;   // 0xCC
    UnkStateTable_D0* field_0xD0; // 0xD0 (event table walked by func_80169DD0)
    u32 field_0xD4;   // 0xD4
    u32 field_0xD8;   // 0xD8
    u32 field_0xDC;   // 0xDC
    u32 field_0xE0;   // 0xE0
    u32 field_0xE4;   // 0xE4
    UnkE8Table* field_0xE8;  // 0xE8 (event table walked by func_80169050)
    u32 field_0xEC;   // 0xEC
    UnkBB38Table* field_0xF0;  // 0xF0 (event table walked by func_8016BB38)
    u32 field_0xF4;   // 0xF4
    u32 field_0xF8;            // 0xF8 (event index passed to func_80169DD0)
    u32 field_0xFC;            // 0xFC (readCommonArchiveFile handle / busy gate)
    u32 field_0x100;           // 0x100
    u32 field_0x104;           // 0x104
    u8 field_0x108;            // 0x108
    u8 field_0x109;            // 0x109 (ctor arms this to 1)
    u32 field_0x10C;           // 0x10C
    u32 field_0x110;           // 0x110
    u8 field_0x114;            // 0x114 (byte cleared by func_80168F38)
    u8 field_0x115;            // 0x115 (Term tests this byte)
    u16 field_0x116;  // 0x116 (id halfword pair published by func_80168610)
    u16 field_0x118;  // 0x118
    u8 gap11A[0x02];  // 0x11A-0x11B
    u32 field_0x11C;  // 0x11C (func_80168028 arena pointer)
    UnkSeq120* field_0x120;    // 0x120 (walk cursor advanced by func_80169DD0)
    u32 field_0x124;  // 0x124 (CX stream position written by func_8016ABA8)
    u32 field_0x128;  // 0x128 (arena chunk size from func_80167D40)
    f32 field_0x12C;  // 0x12C (Init stores func_8048EA40() here)
    u32 field_0x130;  // 0x130 (zero-check by func_80169DD0)
    u16 field_0x134;  // 0x134 (sequence id halfword from func_8016E08C)
    u8 gap136[0x02];  // 0x136-0x137
    u32 field_0x138;  // 0x138 (frame counter incremented by func_80168F38)
    u8 mBuf13C[0x100]; // 0x13C-0x23B (zeroed by the ctor)
    u32 field_0x23C;   // 0x23C (id-list A base; func_80169F28 scratch word)
    u32 field_0x240;   // 0x240 (id-list B base)
    u32 field_0x244;   // 0x244 (id-list C base)
    u32 field_0x248;   // 0x248 (id-list D base)
    u32 field_0x24C;   // 0x24C (entry-table base)
    u8 mCxBuffer[0x8A8]; // 0x250-0xAF7 (CX streaming-uncompression context)
public:
    ~CTaskREvtSequence();
    void Init();
    void Term();
    void cbRenderBefore();

    void Draw();
    bool OnFileEvent(CEventFile* ev);
    u32 func_8016A354();
};
} // namespace cf

// Sequence factory (this TU): allocate + construct + register under parent.
extern "C" cf::CTaskREvtSequence* func_8016AED4(CProcess* parent, const char* name);

// Sequence file loader (this TU): async readFile into field_0xB8 + ptmf table.
extern "C" void func_801686B0(cf::CTaskREvtSequence* self);

// Sibling in this TU (retail func_8016AF4C, unmangled): resolve a name
// string from a data object into `out`; returns 1 on a hit. C linkage
// (retail unmangled name).
extern "C" int func_8016AF4C(u8* data, const char* name, s32* out);
// Region-code helper (CTaskEnvironment.cpp): 0/1/2 region code from the
// global sda. C linkage (retail unmangled name).
extern "C" int func_80059C14(u8* sda);
// Token unpacker (code_800AA008.cpp): splits a packed word into four outputs.
// C linkage (retail unmangled name).
extern "C" void func_800AA318(u32 packed, u32* out0, u32* out1, u32* out2,
                              u32* out3);
// CTaskLOD helpers (code_80135FDC.cpp): task id -> active flag. s16 id so the
// call site passes the halfword directly (retail emits lha, no u8 conversion).
extern "C" void func_80462D5C__8CTaskLODFv(s16 taskId);
extern "C" int func_80462E1C__8CTaskLODFv(s16 taskId);

// Definitions in this TU (retail unmangled names; the extern "C" decls above
// keep the emitted symbols byte-identical to retail).
// Definitions in this TU (retail unmangled names; the extern "C" decls above
// keep the emitted symbols byte-identical to retail).
extern "C" void func_80168800(cf::CTaskREvtSequence* self);
extern "C" void func_80169CD0(cf::CTaskREvtSequence* self);
extern "C" void func_80168F38(cf::CTaskREvtSequence* self);
extern "C" void func_80169DD0(cf::CTaskREvtSequence* self, u32 idx);
extern "C" void func_8016BC1C(UnkEvtListEntry* self);
extern "C" u32 func_8016C118(u32 resId);
extern "C" int func_8016B788(u8* data);
extern "C" int func_8016ADF8(UnkEvtListEntry* self);
extern "C" int func_8016C300(UnkEvtListEntry* self);
extern "C" void func_8016BB38(cf::CTaskREvtSequence* self);
extern "C" int func_80168514(UnkObj80168514* self);
extern "C" bool func_8016ABA8(cf::CTaskREvtSequence* self, EvtSeqFileEvent* ev);
extern "C" int func_8016B5A4(u8* data, const char* name, s32* out);
extern "C" int func_8016B164(u8* data, const char* name, s32* out, s32* out2);
extern "C" int func_8016B384(u8* data, const char* name, f32* out);
extern "C" void func_80169A38(cf::CTaskREvtSequence* self);
extern "C" void func_80169F28(cf::CTaskREvtSequence* self);
extern "C" void func_8016B860(cf::CTaskREvtSequence* self);
extern "C" void func_8016C450(const char* path, EvtC450Buf* buf, s32 limit);

// Imports for func_8016B860 (event-sequence update): minimap/event gates
// (CMiniMap.cpp / CMenuKizunagram.cpp), tag-processor notify (code_8025FB10.cpp)
// and the menu BGM/voice helpers (code_80187F14.cpp / code_801A929C.cpp).
extern "C" s32 func_8011C2E8();
extern "C" void func_8011C400();
extern "C" void func_80261960(int index);
extern "C" void func_80189034(const char* name, int flag, float f1, float f2);
extern "C" void func_80188D34(const char* name, int flag, float f1, float f2);
extern "C" void func_8018896C(int index, u32 type, float f1, float f2);
extern "C" void func_801AACA8(u8 v);

// Imports for func_80169A38 (event-sequence update): scene fade helpers
// (CfObjectImplWalker.cpp).
extern "C" void func_80496294(CScn* scene, float value);

// ptmf tables copied into field_0x3C by func_801696CC (retail .data:
// fixed offsets 0x78/0x84/0x90 inside the lbl_eu_80530A40 pool).

// ptmf tables copied into field_0x3C by func_80169A38 (retail .data:
// 0x80530ADC / 0x80530AE8).
extern u32 lbl_eu_80530ADC[3];
extern u32 lbl_eu_80530AE8[3];

// Realtime-event camera task helpers (CREvtCamera.cpp) called by
// func_801696CC's tail.
extern "C" void func_80180210(int flag);
extern "C" void func_80180394();

// ---- Imports for func_8016925C (event-sequence start) ----
// Base event-name string getter (CTaskREvent.cpp; retail returns the pointer
// as a u32).
extern "C" const char* func_801644AC();
// Scene alloc-handle getter (CTaskGame.cpp).
extern "C" u32 func_80495FF0(CScn* scene);
// BGM stream starter (CTaskGame.cpp): volume arrives in f1.
extern "C" void func_80043738(u32 a1, const char* path, u32 handle, u32 a4,
                              u32 a5, u32 a6, float volume);
// Fade-volume scale source (CTaskREvent.cpp): returns a float in f1.
extern "C" f32 func_80164478();
// Minimap/system reset (CREvtModelMap.cpp).
extern "C" void func_8016FC0C(int val);
// Guest-mode-off reset (CREvtModelMap.cpp).
extern "C" void func_80180DCC();
// Second/third CfGameManager object-list getters (same scheme as
// func_80086B04 above).
extern "C" EvtSeqMgrView* func_80086B08__Q22cf13CfGameManagerFv();
extern "C" EvtSeqMgrView* func_80086B10__Q22cf13CfGameManagerFv();
// Extra .sdata2 floats / ptmf tables used by func_8016925C.
extern f32 lbl_eu_80667660;
extern f32 lbl_eu_80667664;
extern u32 lbl_eu_80530AA0[3];
extern u32 lbl_eu_80530AAC[3];

// Flags object reached through the character container at +0x3F34 during the
// event-sequence reset walks; its vtable slot at absolute 0x84 (user 31)
// arms a flag word.
class EvtSeqFlagsObj {
public:
    virtual void _f00();
    virtual void _f01();
    virtual void _f02();
    virtual void _f03();
    virtual void _f04();
    virtual void _f05();
    virtual void _f06();
    virtual void _f07();
    virtual void _f08();
    virtual void _f09();
    virtual void _f10();
    virtual void _f11();
    virtual void _f12();
    virtual void _f13();
    virtual void _f14();
    virtual void _f15();
    virtual void _f16();
    virtual void _f17();
    virtual void _f18();
    virtual void _f19();
    virtual void _f20();
    virtual void _f21();
    virtual void _f22();
    virtual void _f23();
    virtual void _f24();
    virtual void _f25();
    virtual void _f26();
    virtual void _f27();
    virtual void _f28();
    virtual void _f29();
    virtual void _f30();
    virtual void vf_0x84(int flag);  // user 31 -> vtable+0x84
};

// Object at the head of the character container (+0x3E9C) walked by the
// func_80086B04/B08 manager lists: the reset slots live at vtable offsets
// 0x158 (user 84) and 0x1C0 (user 110).
class EvtSeqResetObj {
public:
    virtual void _r000();
    virtual void _r001();
    virtual void _r002();
    virtual void _r003();
    virtual void _r004();
    virtual void _r005();
    virtual void _r006();
    virtual void _r007();
    virtual void _r008();
    virtual void _r009();
    virtual void _r010();
    virtual void _r011();
    virtual void _r012();
    virtual void _r013();
    virtual void _r014();
    virtual void _r015();
    virtual void _r016();
    virtual void _r017();
    virtual void _r018();
    virtual void _r019();
    virtual void _r020();
    virtual void _r021();
    virtual void _r022();
    virtual void _r023();
    virtual void _r024();
    virtual void _r025();
    virtual void _r026();
    virtual void _r027();
    virtual void _r028();
    virtual void _r029();
    virtual void _r030();
    virtual void _r031();
    virtual void _r032();
    virtual void _r033();
    virtual void _r034();
    virtual void _r035();
    virtual void _r036();
    virtual void _r037();
    virtual void _r038();
    virtual void _r039();
    virtual void _r040();
    virtual void _r041();
    virtual void _r042();
    virtual void _r043();
    virtual void _r044();
    virtual void _r045();
    virtual void _r046();
    virtual void _r047();
    virtual void _r048();
    virtual void _r049();
    virtual void _r050();
    virtual void _r051();
    virtual void _r052();
    virtual void _r053();
    virtual void _r054();
    virtual void _r055();
    virtual void _r056();
    virtual void _r057();
    virtual void _r058();
    virtual void _r059();
    virtual void _r060();
    virtual void _r061();
    virtual void _r062();
    virtual void _r063();
    virtual void _r064();
    virtual void _r065();
    virtual void _r066();
    virtual void _r067();
    virtual void _r068();
    virtual void _r069();
    virtual void _r070();
    virtual void _r071();
    virtual void _r072();
    virtual void _r073();
    virtual void _r074();
    virtual void _r075();
    virtual void _r076();
    virtual void _r077();
    virtual void _r078();
    virtual void _r079();
    virtual void _r080();
    virtual void _r081();
    virtual void _r082();
    virtual void _r083();
    virtual void vf_0x158(int arg);  // user 84 -> vtable+0x158: reset step
    virtual void _r085();
    virtual void _r086();
    virtual void _r087();
    virtual void _r088();
    virtual void _r089();
    virtual void _r090();
    virtual void _r091();
    virtual void _r092();
    virtual void _r093();
    virtual void _r094();
    virtual void _r095();
    virtual void _r096();
    virtual void _r097();
    virtual void _r098();
    virtual void _r099();
    virtual void _r100();
    virtual void _r101();
    virtual void _r102();
    virtual void _r103();
    virtual void _r104();
    virtual void _r105();
    virtual void _r106();
    virtual void _r107();
    virtual void _r108();
    virtual void _r109();
    virtual void vf_0x1C0(int arg);  // user 110 -> vtable+0x1C0: reset step 2
};

// Object reached through the func_80086B10 list nodes: a sub-object pointer
// at +0x98 whose vtable slot at absolute 0x80 (user 30) stops the task.
class EvtSeqB10Obj {
public:
    virtual void _b00();
    virtual void _b01();
    virtual void _b02();
    virtual void _b03();
    virtual void _b04();
    virtual void _b05();
    virtual void _b06();
    virtual void _b07();
    virtual void _b08();
    virtual void _b09();
    virtual void _b10();
    virtual void _b11();
    virtual void _b12();
    virtual void _b13();
    virtual void _b14();
    virtual void _b15();
    virtual void _b16();
    virtual void _b17();
    virtual void _b18();
    virtual void _b19();
    virtual void _b20();
    virtual void _b21();
    virtual void _b22();
    virtual void _b23();
    virtual void _b24();
    virtual void _b25();
    virtual void _b26();
    virtual void _b27();
    virtual void _b28();
    virtual void _b29();
    virtual void vf_0x80(int flag);  // user 30 -> vtable+0x80
};

// Node-object view for the func_80086B10 walk: the word at +0x98 names the
// task object stopped by the walk.
struct EvtSeqB10Node {
    u8 gap00[0x98];            // 0x00
    EvtSeqB10Obj* field_0x98;  // 0x98: stop target (null-tested)
};

// Halfword id pair at the head of the field_0xC4 buffer (published to
// CfGameManager / the global sda by func_8016925C when both are >= 0).
struct EvtSeqC4Pair {
    u8 gap00[0x40];   // 0x00
    s16 field_0x40;   // 0x40: id halfword A
    s16 field_0x42;   // 0x42: id halfword B
};

// .sdata2 int->double magic constant (2^52 + 2^31) and the scale float used
// by func_8016B860's frame-duration conversions.
extern f64 lbl_eu_80667680;
extern f32 lbl_eu_80667688;

// ---- Imports for func_8016A480 (event-sequence teardown / Term body) ----
class CFileHandle;
extern "C" void func_80180E1C();
extern "C" void cancel__11CDeviceFileFP11CFileHandle(CFileHandle* handle);
extern "C" void func_80462D04__8CTaskLODFv(s16 taskId);
extern "C" void func_80043BC4();
// Static shutdown for the shared menu-text state (code_8025FB10.cpp).
extern "C" void __dt__80261B1C();
extern "C" void func_80167EF8();
extern "C" void func_8008670C__Q22cf13CfGameManagerFv();
extern "C" void func_801338C8();
extern "C" void func_80133B80();
extern "C" void func_8012F750(u32 arg);
extern "C" void func_804900A0__FUl(u32 arg);

// Event-task object behind func_801644B4(): teardown-arm word at +0x1B8.
struct EvtSeqMgrTaskView {
    u8 gap00[0x1B8];
    u32 field_0x1B8;
};

