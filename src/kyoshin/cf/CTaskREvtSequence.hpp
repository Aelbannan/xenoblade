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
// body): takes (self, sequence-name string), returns self.
extern "C" void* __ct__cf_CTaskREvtSequence(void* self, const char* name);
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
// func_8049602C: scene vec4 setter (flat retail name; CTaskGame.hpp declares
// the same symbol with a CScnNw4r*/func_800407C8_tmp* shape).
extern "C" void func_8049602C(CScn* scene, int index, EvtSeqVec4* vec);
// func_801644B4 / func_80164CFC: event-task getter / teardown gate
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

// .sdata2 floats used by func_80169CD0 (fade vec, camera/player values).
extern f32 lbl_eu_80667670;
extern f32 lbl_eu_80667674;
extern f32 lbl_eu_80667678;
extern f32 lbl_eu_8066767C;

// ptmf table copied into field_0x3C by func_80168F38 (retail .data:0x80530A7C).
extern u32 lbl_eu_80530A7C[3];

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

// Pointer at CTaskREvtSequence::field_0x120: byte offset +0x28 added to the
// pointer itself by func_80169DD0 (walk cursor advance).
struct UnkSeq120 {
    u8 gap00[0x28];   // 0x00
    u32 field_0x28;   // 0x28
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
    u32 field_0xC;    // 0x0C: word read by func_8016A378
    u32 field_0x10;   // 0x10: size word read by func_80169DD0
    u32 field_0x14;   // 0x14: size word read by func_80169DD0
};  // size 0x18

// Object whose address is stored in UnkState_80664268::field_0xC4;
// func_8016C3DC reads its +0x4C word.
struct UnkStateC4 {
    u8 gap00[0x4C];   // 0x00
    u32 field_0x4C;   // 0x4C
};  // size 0x50

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
};
extern UnkState_80664268* lbl_eu_80664268;

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
    u32 field_0x20;   // 0x20
    u8 gap24[0x04];   // 0x24
    u32 field_0x28;   // 0x28
    u8 gap2C[0x10];   // 0x2C-0x3B
    u32 field_0x3C;   // 0x3C (inner-loop bound read by func_8016BB38)
};  // size 0x40

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
    virtual void* vf_0x08();   // user 0 -> vtable+0x08
    virtual void* vf_0x0C();   // user 1 -> vtable+0x0C
    virtual void* vf_0x10();   // user 2 -> vtable+0x10
    virtual void* vf_0x14();   // user 3 -> vtable+0x14: resolve the name object
    virtual void* vf_0x18();   // user 4 -> vtable+0x18 (func_80168F38 busy probe)
    virtual void* vf_0x1C();   // user 5
    virtual void* vf_0x20();   // user 6
    virtual void* vf_0x24();   // user 7 -> vtable+0x24 (func_80168F38 walk call)
    virtual void* vf_0x28();   // user 8
    virtual void* vf_0x2C();   // user 9 -> vtable+0x2C (func_80169DD0 match call)
    u8 gap04[0x10];            // 0x04-0x13
    s32 field_0x14;            // 0x14: type word (3 = realtime event)
    u8 gap18[0x04];            // 0x18
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

namespace cf {

class CTaskREvtSequence {
public:
    u8 gap00[0x20];
    u32 field_0x20;  // at 0x20, accessed by func_8016A354
    u8 gap24[0x18];  // 0x24-0x3C
    u32 field_0x3C;  // 0x3C (CTTask mMoveFunc slot; Init installs a ptmf here)
    u32 field_0x40;  // 0x40
    u32 field_0x44;  // 0x44
    u8 gap48[0x0C];  // 0x48-0x54
    u32 mEvt54[2];    // 0x54-0x5B (IWorkEvent sub-object head; readFile target)
    u32 field_0x5C;   // 0x5C (flag word; bits 2/4 handled by func_80169DD0)
    char mPath[0x40]; // 0x60-0xA0 (sequence name buffer, FixStr<64> head)
    u8 gapA0[0x04];   // 0xA0-0xA3
    // Embedded memory block (UnkBlock801682AC layout) cleaned up by the dtor.
    UnkEvtListEntry** field_0xA4; // 0xA4 (realtime-event list; freed by the dtor)
    u32 field_0xA8;   // 0xA8 (walk limit / count)
    u32 field_0xAC;   // 0xAC (spare word)
    u32 field_0xB0;   // 0xB0 (ALLOC_HANDLE; -1 = invalid)
    u8 gapB4[0x04];   // 0xB4-0xB7
    u32 field_0xB8;   // 0xB8 (CFileHandle* stored by func_801686B0)
    u32 field_0xBC;   // 0xBC (CFileHandle* from readCommonArchiveFile)
    u32 field_0xC0;   // 0xC0 (file-header object; freed by func_8016872C)
    u32 field_0xC4;   // 0xC4 (newly allocated buffer, read into by func_8016872C)
    u8 gapC8[0x08];            // 0xC8-0xCF
    UnkStateTable_D0* field_0xD0; // 0xD0 (event table walked by func_80169DD0)
    u8 gapD4[0x1C];            // 0xD4-0xEF
    UnkBB38Table* field_0xF0;  // 0xF0 (event table walked by func_8016BB38)
    u8 gapF4[0x04];            // 0xF4-0xF7
    u32 field_0xF8;            // 0xF8 (event index passed to func_80169DD0)
    u32 field_0xFC;            // 0xFC (readCommonArchiveFile handle / busy gate)
    u8 gap100[0x14];           // 0x100-0x113
    u8 field_0x114;            // 0x114 (byte cleared by func_80168F38)
    u8 field_0x115;            // 0x115 (Term tests this byte)
    u16 field_0x116;  // 0x116 (id halfword pair published by func_80168610)
    u16 field_0x118;  // 0x118
    u8 gap11A[0x02];  // 0x11A-0x11B
    u32 field_0x11C;  // 0x11C (func_80168028 arena pointer)
    UnkSeq120* field_0x120;    // 0x120 (walk cursor advanced by func_80169DD0)
    u8 gap124[0x04];  // 0x124-0x127
    u32 field_0x128;  // 0x128 (arena chunk size from func_80167D40)
    f32 field_0x12C;  // 0x12C (Init stores func_8048EA40() here)
    u32 field_0x130;  // 0x130 (zero-check by func_80169DD0)
    u16 field_0x134;  // 0x134 (sequence id halfword from func_8016E08C)
    u8 gap136[0x02];  // 0x136-0x137
    u32 field_0x138;  // 0x138 (frame counter incremented by func_80168F38)
public:
    ~CTaskREvtSequence();
    void Init();
    void Term();
    void cbRenderBefore();

    void Draw();
    void OnFileEvent();
    u32 func_8016A354();
};
} // namespace cf

// Sequence factory (this TU): allocate + construct + register under parent.
extern "C" cf::CTaskREvtSequence* func_8016AED4(CProcess* parent, const char* name);

// Sequence file loader (this TU): async readFile into field_0xB8 + ptmf table.
extern "C" void func_801686B0(cf::CTaskREvtSequence* self);

// Sibling in this TU (retail func_8016AF4C, unmangled): resolve a name
// string from a data object into `out`. C linkage (retail unmangled name).
extern "C" void func_8016AF4C(u8* data, const char* name, s32* out);
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

