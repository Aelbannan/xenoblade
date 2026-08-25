#pragma once

#include <types.h>

namespace cf {

class CfGameManager;
class CfObjectMove;

// Minimal CfBdat view for this TU (see the include note in CfResPcImpl.cpp:
// the full header's s32/int clash with code_801862C0.hpp blocks compilation
// here). Same member signatures as cf/CfBdat.hpp.
class CfBdat {
public:
    static u32 func_801422A8(u32 param1);
    static const char* func_801424A8(u16 index);
    static void resetMapBdatFileDataPointers();
};

// Resource object behind the lookup entry's +0x2C slot (the "child" passed
// to func_80065CA4 by func_8018D0C4).
struct CfResPcEntryObj {
    u8 field_00[4];
};

// Entry returned by the resource-table lookup; +0x2C holds the resource
// object detached by func_8018D0C4, +0x04 is the resource pointer fed to
// func_800AA33C by func_8018E7E4.
struct CfResPcLookupEntry {
    /* 0x00 */ u32 field_00;
    /* 0x04 */ u8* field_04;
    u8 field_08[0x2C - 0x08];
    /* 0x2C */ CfResPcEntryObj* field_2C;
};

// 0x3C-byte resource-table entry returned by func_80062C28: an array base
// whose entries [1..5] are walked by func_8018DE8C (each has its resource
// object at +0x2C; entry[1]'s +0x2C is also reachable as base+0x68).
struct CfResPcTableEntry {
    /* 0x00 */ u32 field_00;
    /* 0x04 */ u8* field_04;
    u8 field_08[0x2C - 0x08];
    /* 0x2C */ CfResPcEntryObj* field_2C;
    u8 field_30[0x3C - 0x30];
};

// Sub-object at parent +0xC4: the flag word at +0x4EC is folded by
// func_8018DE8C when the player-positioning block ran.
struct CfResPcC4Obj {
    u8 field_00[0x4EC];
    /* 0x4EC */ u32 field_4EC;
};

// Cast-only vtable view of the object behind lookup-entry field_2C (its own
// vptr sits at +0x00): slot +0x08 builds the resource from (entry, slot),
// slot +0x40 reports whether it is still in use (entry). Dummies pin the
// offsets.
struct CfResPcEntryObjIf {
    virtual u8* _v008(CfResPcLookupEntry* entry, u32 slot);  // vtable offset 0x08
    virtual void _v00C(); virtual void _v010(); virtual void _v014();
    virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
    virtual void _v028(); virtual void _v02C(); virtual void _v030(); virtual void _v034();
    virtual void _v038(); virtual void _v03C();
    virtual int _v040(CfResPcLookupEntry* entry);  // vtable offset 0x40
};

// Cast-only vtable view of the object stored at parent+0x98: slot +0xC4
// installs a model (resource handle, bdat string, flag). Dummies pin the
// offsets.
struct CfResPc98ObjIf {
    virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
    virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
    virtual void _v028(); virtual void _v02C(); virtual void _v030(); virtual void _v034();
    virtual void _v038(); virtual void _v03C(); virtual void _v040(); virtual void _v044();
    virtual void _v048(); virtual void _v04C(); virtual void _v050(); virtual void _v054();
    virtual void _v058(); virtual void _v05C(); virtual void _v060();
    virtual void _v064(int arg);  // vtable offset 0x64 (dispatched by func_8018DE8C with 0)
    virtual void _v068(); virtual void _v06C(); virtual void _v070(); virtual void _v074();
    virtual void _v078(); virtual void _v07C(); virtual void _v080(); virtual void _v084();
    virtual void _v088(); virtual void _v08C(); virtual void _v090(); virtual void _v094();
    virtual void _v098(); virtual void _v09C(); virtual void _v0A0(); virtual void _v0A4();
    virtual void _v0A8(); virtual void _v0AC(); virtual void _v0B0(); virtual void _v0B4();
    virtual void _v0B8(); virtual void _v0BC(); virtual void _v0C0();
    virtual int _v0C4(u8* res, u32 bdat, u32 arg);  // vtable offset 0xC4
};

// Object created by func_80489A60 and stored at parent+0x6F8 / +0x6FC:
// vtable at +0x00, flag word at +0x64 (bit 1 tested by func_8018E7E4),
// model flag word at +0x7A4 (bit 6 set after creation).
struct CfResPcResObj {
    u8 field_00[0x64];
    /* 0x64 */ u32 field_64;
    u8 field_68[0x7A4 - 0x68];
    /* 0x7A4 */ u32 field_7A4;
};

// Cast-only vtable view of the +0x6F8 / +0x6FC objects: slot +0x48 (no-arg
// form) and slot +0x9C (model load with two args). Dummies pin the offsets.
struct CfResPcResObjVtIf {
    virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
    virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
    virtual void _v028(); virtual void _v02C(); virtual void _v030(); virtual void _v034();
    virtual void _v038(); virtual void _v03C(); virtual void _v040(); virtual void _v044();
    virtual void _v048();  // vtable offset 0x48
    virtual void _v04C(); virtual void _v050(); virtual void _v054(); virtual void _v058();
    virtual void _v05C(); virtual void _v060(); virtual void _v064(); virtual void _v068();
    virtual void _v06C(); virtual void _v070(); virtual void _v074(); virtual void _v078();
    virtual void _v07C(); virtual void _v080(); virtual void _v084(); virtual void _v088();
    virtual void _v08C(); virtual void _v090(); virtual void _v094(); virtual void _v098();
    virtual void _v09C(int a, int b);  // vtable offset 0x9C
};

// Same interface as CfResPcResObjVtIf but slot +0x48 takes a float (used
// with lbl_eu_80667A60 by the alternate branch of func_8018E7E4).
struct CfResPcResObjVtIf2 {
    virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
    virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
    virtual void _v028(); virtual void _v02C(); virtual void _v030(); virtual void _v034();
    virtual void _v038(); virtual void _v03C(); virtual void _v040(); virtual void _v044();
    virtual void _v048(float f1);  // vtable offset 0x48
    virtual void _v04C(); virtual void _v050(); virtual void _v054(); virtual void _v058();
    virtual void _v05C(); virtual void _v060(); virtual void _v064(); virtual void _v068();
    virtual void _v06C(); virtual void _v070(); virtual void _v074(); virtual void _v078();
    virtual void _v07C(); virtual void _v080(); virtual void _v084(); virtual void _v088();
    virtual void _v08C(); virtual void _v090(); virtual void _v094(); virtual void _v098();
    virtual void _v09C(int a, int b);  // vtable offset 0x9C
};

// Object stored at parent+0x700: the first word is a handle pointer
// (null-checked after the entry-build dispatch by func_8018E7E4).
struct CfResPc700Obj {
    /* 0x00 */ u8* field_00;
};

// Cast-only vtable view of the object stored at parent+0x38: slot +0xB0 is
// dispatched by func_8018E7E4 after the slot cleanup. Dummies pin the
// offsets.
struct CfResPc38ObjIf {
    virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
    virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
    virtual void _v028(); virtual void _v02C(); virtual void _v030(); virtual void _v034();
    virtual void _v038(); virtual void _v03C(); virtual void _v040(); virtual void _v044();
    virtual void _v048(); virtual void _v04C(); virtual void _v050(); virtual void _v054();
    virtual void _v058(); virtual void _v05C(); virtual void _v060(); virtual void _v064();
    virtual void _v068(); virtual void _v06C(); virtual void _v070(); virtual void _v074();
    virtual void _v078(); virtual void _v07C(); virtual void _v080(); virtual void _v084();
    virtual void _v088(); virtual void _v08C(); virtual void _v090(); virtual void _v094();
    virtual void _v098(); virtual void _v09C(); virtual void _v0A0(); virtual void _v0A4();
    virtual void _v0A8(); virtual void _v0AC();
    virtual void _v0B0();  // vtable offset 0xB0
};

// Stack view of the CMcaFile object constructed by __ct__CMcaFile in
// func_8018E7E4: vptr at +0x00, payload pointers at +0x08 / +0x0C (the
// +0x0C word is the data-adjacent pointer passed to func_80495EAC).
struct CfResPcMca {
    u8 field_00[0x8];
    /* 0x08 */ void* field_08;
    /* 0x0C */ void* field_0C;
};

// Entry returned by findResEntry in func_8018D79C: the +0x0C word is
// cleared on a successful install.
struct CfResPcFindEntry {
    u8 field_00[0x0C];
    /* 0x0C */ u32 field_0C;
};

// Stack buffer filled by func_80080F48 (CfGameManager slot query): seven
// words plus four u16 ids.
struct CfResPcLoadBuffer {
    /* 0x00 */ u32 field_00[7];
    /* 0x1C */ u16 field_1C;
    /* 0x1E */ u16 field_1E;
    /* 0x20 */ u16 field_20;
    /* 0x22 */ u16 field_22;
};

// 8-byte entry of the area-id table at lbl_eu_805327F8 (walked by
// func_8018F63C): scene id plus area id/sub-id pair.
struct CfResPcAreaEntry {
    /* 0x00 */ u32 field_00;
    /* 0x04 */ u16 field_04;
    /* 0x06 */ u16 field_06;
};

// 8-byte float pair of the position table copied from lbl_eu_80503B30 by
// func_8018E7E4 (indexed by the slot id).
struct CfResPcPosTableEntry {
    /* 0x00 */ f32 field_00;
    /* 0x04 */ f32 field_04;
};

// 0x50-byte block copied from lbl_eu_80503B30 into table[1..10] by
// func_8018E7E4 (struct copy so MWCC emits the inline lwzu/stwu loop).
struct CfResPcPosTable {
    /* 0x00 */ CfResPcPosTableEntry entries[10];
};

// Parent object referenced at +0x00 of CfResPcImpl (a CfObject-derived
// instance; its vptr sits at +0x00 and is treated as opaque here). Flag
// words at +0x64/+0x68 and the reload slots at +0x6DC/+0x6E0 are
// read/written by this unit. Same offsets as CfResReloadParent in
// CfResReloadImpl.hpp; kept local so this TU does not pull that header's
// getBdatStringColumnValue(int) declaration (CfBdat.hpp declares the s32
// variant, and int/s32 are distinct C++ types here).
struct CfResPcParent {
    u8 field_00[0x38];             // 0x00..0x37 (incl. the vptr)
    /* 0x38 */ CfResPc38ObjIf* field_38;  // object with vtable slot +0xB0 (dispatched by func_8018E7E4)
    u8 field_3C[0x64 - 0x3C];
    /* 0x64 */ u32 field_64;       // flags (bit 1 = 0x2 tested by func_8018D0C4, bit 31 = 0x80000000 tested by func_eu_8018E19C)
    /* 0x68 */ u32 field_68;       // flags (bit 21 = 0x200000 tested by func_8018CF08/CF90)
    /* 0x6C */ u32 field_6C;       // flags (bit 17 = 0x20000 set by func_8018D354 on query success)
    u8 field_70[0x74 - 0x70];      // 0x70..0x73
    /* 0x74 */ u8* field_74;       // sound-related pointer passed to func_801BFE20 (same offset as CfResReloadParent)
    u8 field_78[0x8C - 0x78];      // 0x78..0x8B
    /* 0x8C */ u16 field_8C;       // slot id read by func_8018D00C
    /* 0x8E */ u16 field_8E;       // cleared by the ctor when a parent is present
    /* 0x90 */ u32 field_90;       // cleared by func_8018CB3C
    /* 0x94 */ u32 field_94;       // cleared by func_8018CB3C
    /* 0x98 */ CfResPc98ObjIf* field_98;  // model object (vtable slot +0xC4 dispatched by func_8018E7E4)
    /* 0x9C */ u32 field_9C;       // model handle (func_800584B8 result, written by func_8018DE8C)
    u8 field_A0[0xC4 - 0xA0];      // 0xA0..0xC3
    /* 0xC4 */ CfResPcC4Obj* field_C4;  // sub-object (flag word at +0x4EC folded by func_8018DE8C)
    u8 field_C8[0x6CC - 0xC8];     // 0xC8..0x6CB
    /* 0x6CC */ u16 field_6CC;     // set to 1 by func_8018DE8C when parent != getPlayer(0)
    u8 field_6CE[0x6D4 - 0x6CE];   // 0x6CE..0x6D3
    /* 0x6D4 */ u32 field_6D4;     // model handle (func_800584B8 result for the +0x708 object)
    /* 0x6D8 */ u32 field_6D8;     // loaded model handle (func_80495EAC result)
    /* 0x6DC */ CfResPcLookupEntry* field_6DC;  // active lookup entry
    /* 0x6E0 */ u32 field_6E0;
    u8 field_6E4[0x6F8 - 0x6E4];   // 0x6E4..0x6F7
    /* 0x6F8 */ CfResPcResObj* field_6F8[2];  // model objects (+0x6F8 / +0x6FC, built from entry_d's +0x08 slot)
    /* 0x700 */ CfResPc700Obj* field_700; // entry-build result (handle pointer at +0x00)
    /* 0x704 */ u32 field_704;     // cleared by func_8018CB3C
    /* 0x708 */ u32 field_708;     // resource handle (written by func_8018DE8C)
    /* 0x70C */ u16 field_70C[4];  // +0x70C..+0x712 u16 ids (written by func_8018CBE8)
};

// Per-slot record of the manager slot table: 8-byte stride; the first word
// is the slot handle (null-checked by func_8018F46C).
struct CfResPcSlotEntry {
    /* 0x00 */ u8* field_00;
    /* 0x04 */ u32 field_04;
};

// Slot table anchored at inner+0x834: per-slot records whose handle words
// sit at +0x610 relative to the anchor (absolute inner+0xE44); the u16 slot
// count is at +0x622 (absolute inner+0xE56).
struct CfResPcSlotTable {
    u8 field_00[0x610];
    /* 0x610 */ CfResPcSlotEntry field_610[2];   // records 0..1
    /* 0x620 */ u8 field_620[2];                 // record 2 first word, low half
    /* 0x622 */ u16 field_622;                   // slot count
    /* 0x624 */ u32 field_624;                   // record 2 second word
    u8 field_628[0x40];                          // remaining records
};

// Inner object behind the manager's +0x00 slot (walked by func_8018F46C):
// +0x82C flag word (bit 2 gates the slot-cleanup loop) and the +0x834 slot
// table.
struct CfResPcMgrInner {
    u8 field_00[0x82C];
    /* 0x82C */ u32 field_82C;
    u8 field_830[0x834 - 0x830];
    /* 0x834 */ CfResPcSlotTable field_834;
};

// Per-slot record of the second manager's slot table: 8-byte stride; the
// first word is the slot handle (null-checked by func_8018F368).
struct CfResPcSlotEntry2 {
    /* 0x00 */ u8* field_00;
    /* 0x04 */ u32 field_04;
};

// Slot table walked by func_8018F368 (anchored at inner2+0x834): three
// 8-byte-stride record groups (handles at +0x000/+0x100/+0x200 relative to
// the anchor, i.e. absolute inner2+0x834/+0x934/+0xA34) with u16 counts at
// +0x618/+0x61A/+0x61C (absolute inner2+0xE4C..0xE50).
struct CfResPcSlotTable2 {
    /* 0x000 */ CfResPcSlotEntry2 field_000[32];
    /* 0x100 */ CfResPcSlotEntry2 field_100[32];
    /* 0x200 */ CfResPcSlotEntry2 field_200[32];
    u8 field_300[0x618 - 0x300];
    /* 0x618 */ u16 field_618;
    /* 0x61A */ u16 field_61A;
    /* 0x61C */ u16 field_61C;
};

// Inner object behind the manager's +0x04 slot (walked by func_8018F368):
// +0x82C flag word (bit 2 gates the whole cleanup), the +0x830 word compared
// against the caller's 3rd argument, then the three-group slot table.
struct CfResPcMgrInner2 {
    u8 field_00[0x82C];
    /* 0x82C */ u32 field_82C;
    /* 0x830 */ u32 field_830;
    /* 0x834 */ CfResPcSlotTable2 field_834;
};

// Manager byte-view for func_8018F46C / func_8018F63C / func_8018F368:
// +0x00 points at the first inner object (slot cleanup), +0x04 at the second
// inner object (three-group slot cleanup); the host object stores the
// manager at +0x408.
struct CfResPcMgrView {
    /* 0x00 */ CfResPcMgrInner* field_00;
    /* 0x04 */ CfResPcMgrInner2* field_04;
};

struct CfResPcMgrHost {
    u8 field_00[0x408];
    /* 0x408 */ CfResPcMgrView* field_408;
};

// Byte view of the object returned by func_8009EC9C: +0x18 holds the text
// id (u16) read by func_8018D00C.
struct CfResPcCharData {
    u8 field_00[0x0E];
    /* 0x0E */ u16 field_0E[5];  // s16 id list indexed by func_8018D79C's loop
    /* 0x18 */ u16 field_18;
};

class CfResPcImpl;

// Non-polymorphic base pushing the secondary vtable to +0x10 (double-hop
// thunk pattern, MWCC_CASES): casting self to CfResPcVtIf and calling a
// virtual emits `lwz r12,0x10(r3); lwz r12,slot(r12); mtctr; bctrl` with
// `this` staying at the object base. lbl_eu_80532774 (stored at +0x10 by
// the ctor) is the vtable itself.
struct CfResPcVtShift {
    u8 field_00[0x10];
};

struct CfResPcVtIf : CfResPcVtShift {
    virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
    virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
    virtual void _v028();              // vtable offset 0x28 (dispatched by func_eu_8018E19C)
    virtual void _v02C();
    virtual void _v030(int a, int b);  // vtable offset 0x30 (dispatched by func_8018D290 with (0, arg2))
    virtual int _v034(int arg);        // vtable offset 0x34 (dispatched by func_eu_8018E19C)
    virtual int _v038();               // vtable offset 0x38 (dispatched by func_8018D65C, result to func_80063994)
    virtual void _v03C(int a, int b);  // vtable offset 0x3C (dispatched by func_8018D79C with (j, v))
    virtual void _v040(); virtual void _v044(); virtual void _v048();
    virtual void _v04C(int a);         // vtable offset 0x4C (dispatched by func_8018D79C with r30)
    virtual void _v050(int a);         // vtable offset 0x50 (dispatched by func_8018D79C with the packed token)
};

// 0x44-byte resource object. Layout recovered from the retail ctor
// (__ct__cf_CfResPcImpl: stfs +4, sth +8, sth +0xA, ...) and the getter/
// setter pair func_8018D134 / func_8018D510 (8-slot table at +0x14).
// The +0x00 slot holds the parent object reference (same head layout as
// CfResReloadImpl; func_8018D0C4 / func_8018CF08 / func_8018CF90 deref it).
class CfResPcImpl {
public:
    CfResPcImpl();
    ~CfResPcImpl();

    /* 0x00 */ CfResPcParent* field_00;  // parent/reference object
    /* 0x04 */ f32 field_04;
    /* 0x08 */ u16 field_08;  // handler index (selected by func_8018EEF0)
    /* 0x0A */ s16 field_0A;  // state (-1 = invalid; lha by callers)
    /* 0x0C */ u16 field_0C;
    /* 0x0E */ s16 field_0E;  // state (-1 = invalid; lha by func_8018CF08/CF90)
    /* 0x10 */ CfResPcVtIf* field_10;  // secondary vtable pointer (lbl_eu_80532774)
    /* 0x14 */ u32 field_14[8];  // 8-slot table (get/set by func_8018D134 / func_8018D510)
    /* 0x34 */ u32 field_34;
    /* 0x38 */ s16 field_38;  // state (-1 = invalid; lha by func_eu_8018E19C)
    /* 0x3A */ s16 field_3A;
    /* 0x3C */ s16 field_3C;
    /* 0x3E */ u16 field_3E;
    /* 0x40 */ u16 field_40;
};

// File-event archive pair referenced by func_8018EFD4 through the object at
// +0x408: which slot is used depends on a flag.
struct CfResPcFilePair {
    /* 0x00 */ class CfTFile* field_00;  // used when flag == 0
    /* 0x04 */ class CfTFile* field_04;  // used when flag != 0
};

// Host view for func_8018F018: +0x408 points at the file-event archive pair
// (field_04 selected when arg2 != 0, field_00 otherwise).
struct CfResPcFileHost {
    u8 field_00[0x408];
    /* 0x408 */ CfResPcFilePair* field_408;
};

// Byte view of the object at host+0x408 used by func_8018F63C: the +0x00 and
// +0x08 CfTFile slots are the copy pair for func_80069A78 (the same +0x408
// slot that func_8018F46C views as CfResPcMgrView).
struct CfResPcMgrFileView {
    /* 0x00 */ class CfTFile* field_00;
    u8 field_04[4];
    /* 0x08 */ class CfTFile* field_08;
};

// Host view for func_8018F63C: +0x408 points at the file view above.
struct CfResPcMgrFileHost {
    u8 field_00[0x408];
    /* 0x408 */ CfResPcMgrFileView* field_408;
};

class UnkClass_8018EF3C {
public:
    UnkClass_8018EF3C();
    ~UnkClass_8018EF3C();

    u8 field_00[0x400];       // 0x000..0x3FF (bulk-cleared by the dtor)
    /* 0x400 */ u32 field_400;
    /* 0x404 */ u32 field_404;
    /* 0x408 */ CfResPcFilePair* field_408;  // file-event archive pair
};

// Byte view of the object hosting the CfGameManager reference at +0x408
// (used by func_8018F520; the +0x28 offset into it is the file-event name
// buffer). Same shape as CFuncHost408 in code_8018F8D8.hpp.
struct CfResPcHost408 {
    u8 field_00[0x408];
    u8* field_408;  // manager; +0x28 holds the name buffer
};

// Manager-typed view of the +0x408 slot (used by func_8018F5A4): the same
// object CfResPcHost408 views as bytes, here with the manager typed so
// unk90 (vtable slot +0x74) can be reached.
struct CfResPcHostGM {
    u8 field_00[0x408];
    /* 0x408 */ CfGameManager* field_408;
};

} // namespace cf

// Result of func_8009D5FC lives in kyoshin/cf/CfGameManager.hpp
// (CfFileEventIdsView); declared there - forward view here only.
struct CfFileEventIdsView;

// Minimal nw4r sound-object interface used by func_8018CD9C (sound slot +0x00
// dereferenced to call SetPlayerPriority). Declared here at global scope
// rather than via snd_BasicSound.h; MWCC mangles the member call to the
// retail name SetPlayerPriority__Q44nw4r3snd6detail10BasicSoundFi (same
// pattern as CfResReloadImpl.hpp - a copy inside namespace cf would mangle
// the extra cf scope into the name).
namespace nw4r {
namespace snd {
namespace detail {
class BasicSound {
public:
    void SetPlayerPriority(int priority);
};
} // namespace detail
} // namespace snd
} // namespace nw4r

// Forward decl so the cast-only parent vtable view can return/accept VEC3
// (the real definition comes from <nw4r/math/math_types.h> in the .cpp).
namespace nw4r { namespace math { struct VEC3; } }

namespace cf {

// Slot entry returned by func_801BFAE4 (CfSoundMan slot lookup); +0x00 holds
// the active sound object, which func_8018CD9C calls SetPlayerPriority on.
struct CfResPcSoundSlotEntry {
    /* 0x00 */ nw4r::snd::detail::BasicSound* field_00;
};

// Entry returned by func_80062EC4 in func_8018D570: +0x04 resource id
// (compared against the secondary vtable slot +0x34 result) and s16 counters
// at +0x38/+0x3A (incremented on a hit).
struct CfResPcEntry38 {
    u8 field_00[0x04];
    /* 0x04 */ u32 field_04;
    u8 field_08[0x38 - 0x08];
    /* 0x38 */ s16 field_38;
    /* 0x3A */ s16 field_3A;
};

// Vtable-proxy for the player object that embeds CfResPcParent at +0x3E9C
// (the parent pointer minus 0x3E9C recovers the player base; CfObjectMove
// lives at +0x3E9C inside CfObjectPc). Slot +0x2BC is dispatched by
// func_8018EE18 (nonzero return = resource still in use). Dummy slots pin
// the offsets (-RTTI's two leading vtable slots land the first declared
// virtual at +8).
struct CfResPcPlayerVtIf {
    virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
    virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
    virtual void _v028(); virtual void _v02C(); virtual void _v030(); virtual void _v034();
    virtual void _v038(); virtual void _v03C(); virtual void _v040(); virtual void _v044();
    virtual void _v048(); virtual void _v04C(); virtual void _v050(); virtual void _v054();
    virtual void _v058(); virtual void _v05C(); virtual void _v060(); virtual void _v064();
    virtual void _v068(); virtual void _v06C(); virtual void _v070(); virtual void _v074();
    virtual void _v078(); virtual void _v07C(); virtual void _v080(); virtual void _v084();
    virtual void _v088(); virtual void _v08C(); virtual void _v090(); virtual void _v094();
    virtual void _v098(); virtual void _v09C(); virtual void _v0A0(); virtual void _v0A4();
    virtual void _v0A8(); virtual void _v0AC(); virtual void _v0B0(); virtual void _v0B4();
    virtual void _v0B8(); virtual void _v0BC(); virtual void _v0C0(); virtual void _v0C4();
    virtual void _v0C8(); virtual void _v0CC(); virtual void _v0D0(); virtual void _v0D4();
    virtual void _v0D8(); virtual void _v0DC(); virtual void _v0E0(); virtual void _v0E4();
    virtual void _v0E8(); virtual void _v0EC(); virtual void _v0F0(); virtual void _v0F4();
    virtual void _v0F8(); virtual void _v0FC(); virtual void _v100(); virtual void _v104();
    virtual void _v108(); virtual void _v10C(); virtual void _v110(); virtual void _v114();
    virtual void _v118(); virtual void _v11C(); virtual void _v120(); virtual void _v124();
    virtual void _v128(); virtual void _v12C(); virtual void _v130(); virtual void _v134();
    virtual void _v138(); virtual void _v13C(); virtual void _v140(); virtual void _v144();
    virtual void _v148(); virtual void _v14C(); virtual void _v150(); virtual void _v154();
    virtual void _v158(); virtual void _v15C(); virtual void _v160(); virtual void _v164();
    virtual void _v168(); virtual void _v16C(); virtual void _v170(); virtual void _v174();
    virtual void _v178(); virtual void _v17C(); virtual void _v180(); virtual void _v184();
    virtual void _v188(); virtual void _v18C(); virtual void _v190(); virtual void _v194();
    virtual void _v198(); virtual void _v19C(); virtual void _v1A0(); virtual void _v1A4();
    virtual void _v1A8(); virtual void _v1AC(); virtual void _v1B0(); virtual void _v1B4();
    virtual void _v1B8(); virtual void _v1BC(); virtual void _v1C0(); virtual void _v1C4();
    virtual void _v1C8(); virtual void _v1CC(); virtual void _v1D0(); virtual void _v1D4();
    virtual void _v1D8(); virtual void _v1DC(); virtual void _v1E0(); virtual void _v1E4();
    virtual void _v1E8(); virtual void _v1EC(); virtual void _v1F0(); virtual void _v1F4();
    virtual void _v1F8(); virtual void _v1FC(); virtual void _v200(); virtual void _v204();
    virtual void _v208(); virtual void _v20C(); virtual void _v210(); virtual void _v214();
    virtual void _v218(); virtual void _v21C(); virtual void _v220(); virtual void _v224();
    virtual void _v228(); virtual void _v22C(); virtual void _v230(); virtual void _v234();
    virtual void _v238(); virtual void _v23C(); virtual void _v240(); virtual void _v244();
    virtual void _v248(); virtual void _v24C(); virtual void _v250(); virtual void _v254();
    virtual void _v258(); virtual void _v25C(); virtual void _v260(); virtual void _v264();
    virtual void _v268(); virtual void _v26C(); virtual void _v270(); virtual void _v274();
    virtual void _v278(); virtual void _v27C(); virtual void _v280(); virtual void _v284();
    virtual void _v288(); virtual void _v28C(); virtual void _v290(); virtual void _v294();
    virtual void _v298(); virtual void _v29C(); virtual void _v2A0(); virtual void _v2A4();
    virtual void _v2A8(); virtual void _v2AC(); virtual void _v2B0(); virtual void _v2B4();
    virtual void _v2B8();
    virtual int _v2BC();  // vtable offset 0x2BC (dispatched by func_8018EE18)
};

// Player object that embeds the CfResPcParent at +0x3E9C (the parent
// reference stored in CfResPcImpl::field_00). func_8018EE18 recovers the
// base by subtracting 0x3E9C to reach the player vtable (slot +0x2BC).
struct CfResPcPlayerBase {
    u8 field_00[0x3E9C];
    /* 0x3E9C */ CfResPcParent field_3E9C;
};

// Vtable-proxy for the parent object (a CfObject-derived instance): the
// vptr sits at parent+0x00; slot +0x74 is dispatched by func_8018CF08 /
// func_8018CF90 (nonzero return = live resource). Dummy slots pin the
// offsets (-RTTI's two leading vtable slots land the first declared
// virtual at +8).
struct CfResPcParentVtIf {
    virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
    virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
    virtual void _v028(); virtual void _v02C(); virtual void _v030(); virtual void _v034();
    virtual void _v038(); virtual void _v03C(); virtual void _v040(); virtual void _v044();
    virtual void _v048(); virtual void _v04C(); virtual void _v050(); virtual void _v054();
    virtual void _v058(); virtual void _v05C(); virtual void _v060(); virtual void _v064();
    virtual void _v068(); virtual void _v06C(); virtual void _v070();
    virtual int _v074();  // vtable offset 0x74
    virtual void _v078(); virtual void _v07C(); virtual void _v080(); virtual void _v084();
    virtual void _v088(); virtual void _v08C(); virtual void _v090(); virtual void _v094();
    virtual void _v098(); virtual void _v09C(); virtual void _v0A0(); virtual void _v0A4();
    virtual void _v0A8();
    virtual nw4r::math::VEC3* _v0AC();  // vtable offset 0xAC - returns the player position (3 floats)
    virtual void _v0B0(); virtual void _v0B4();
    virtual void _v0B8(nw4r::math::VEC3* pos, float f1);  // vtable offset 0xB8 - position the player
    virtual void _v0BC(); virtual void _v0C0(); virtual void _v0C4();
    virtual void _v0C8(); virtual void _v0CC(); virtual void _v0D0(); virtual void _v0D4();
    virtual void _v0D8(); virtual void _v0DC(); virtual void _v0E0(); virtual void _v0E4();
    virtual void _v0E8(); virtual void _v0EC(); virtual void _v0F0(); virtual void _v0F4();
    virtual void _v0F8(); virtual void _v0FC(); virtual void _v100(); virtual void _v104();
    virtual void _v108(); virtual void _v10C(); virtual void _v110(); virtual void _v114();
    virtual void _v118(); virtual void _v11C(); virtual void _v120(); virtual void _v124();
    virtual void _v128(); virtual void _v12C(); virtual void _v130();
    virtual void _v134(float value);  // vtable offset 0x134 (dispatched by func_8018DE8C)
    virtual void _v138(); virtual void _v13C(); virtual void _v140(); virtual void _v144();
    virtual void _v148(); virtual void _v14C(); virtual void _v150(); virtual void _v154();
    virtual void _v158(); virtual void _v15C(); virtual void _v160(); virtual void _v164();
    virtual void _v168(float value);  // vtable offset 0x168 (dispatched by func_8018DE8C)
    virtual void _v16C(); virtual void _v170(); virtual void _v174();
    virtual void _v178();  // vtable offset 0x178 (dispatched by func_8018CB3C)
    virtual void _v17C();  // vtable offset 0x17C (dispatched by func_8018CB3C)
    virtual void _v180(); virtual void _v184(); virtual void _v188(); virtual void _v18C();
    virtual void _v190(); virtual void _v194(); virtual void _v198(); virtual void _v19C();
    virtual void _v1A0(); virtual void _v1A4(); virtual void _v1A8(); virtual void _v1AC();
    virtual void _v1B0(); virtual void _v1B4(); virtual void _v1B8(); virtual void _v1BC();
    virtual void _v1C0(); virtual void _v1C4(); virtual void _v1C8();
    virtual void _v1CC();  // vtable offset 0x1CC (dispatched by func_8018CB3C)
    virtual void _v1D0(int a);  // vtable offset 0x1D0 (dispatched by func_8018D3F0 with arg3)
    virtual void _v1D4(float value);  // vtable offset 0x1D4 (dispatched by func_8018DE8C with a scaled float)
    virtual void _v1D8(); virtual void _v1DC(); virtual void _v1E0();
    virtual void _v1E4(); virtual void _v1E8(); virtual void _v1EC(); virtual void _v1F0();
    virtual void _v1F4(); virtual void _v1F8(); virtual void _v1FC(); virtual void _v200();
    virtual void _v204(); virtual void _v208(); virtual void _v20C(); virtual void _v210();
    virtual void _v214();
    virtual void _v218();  // vtable offset 0x218 (dispatched by func_8018E7E4)
};
} // namespace cf

// Forward decl so the C-ABI import below can name the type.
namespace cf { class CfTFile; }

// PMTF dispatch table (4 x 12-byte member pointers) selected by field_08 in
// func_8018EEF0. Declared at global scope so MWCC keeps the retail name
// unmangled; the (self->*table[idx])() call lowers to `bl __ptmf_scall`.
typedef void (cf::CfResPcImpl::*CfResPcImplPMF)();
extern CfResPcImplPMF lbl_eu_80532730[4];

// Secondary vtable for CfResPcImpl, stored at +0x10 by the ctor (.data).
extern cf::CfResPcVtIf lbl_eu_80532774;

// Float seed written to +0x04 by the ctor (.sdata2). Declared const so MWCC
// treats the pool load as a constant and hoists the lfs above the frame
// stores (MWCC_CASES: `extern const float` scheduling fix).
extern const float lbl_eu_80667A40;

// C-ABI import previously pulled in via cf/CfBdat.hpp (not included here;
// see the note in CfResPcImpl.cpp).
extern "C" int func_800AA33C(ml::FixStr<64>& buf, u32 packed, int prefixFlag, int suffixFlag);

// C-ABI imports (defined in CfRes.cpp / IResInfo.cpp / CfScript.cpp /
// CtrlObjectParam.cpp / code_8018F8D8.cpp / CfGameManagerUnityHelpers.hpp).
// extern "C" keeps the call-site relocs at the plain retail names (same
// convention as CfResReloadImpl.hpp / the existing func_80069ACC below).
extern "C" void func_80065CA4(cf::CfResPcEntryObj* child, cf::CfResPcLookupEntry* parent);
extern "C" void func_80066714(cf::CfResPcLookupEntry* entry, bool cleanup);
extern "C" ::CfFileEventIdsView* func_8009D5FC();
extern "C" void func_8009EB2C(int a, int b, u8* c);
extern "C" void func_8009F6D4(void* object);
extern "C" cf::CfResPcCharData* func_8009EC9C(u32 idx);
extern "C" void func_80068AEC(u8* name);
extern "C" void func_8008413C__Q22cf13CfGameManagerFv(u16 a, u32 b);
extern "C" void func_80084AD4__Q22cf13CfGameManagerFv(u32 mask);
extern "C" bool func_8007DA0C__Q22cf13CfGameManagerFv(u8* manager, u32 first, u32 second);
extern "C" void func_801BFF04(int a, int b, int c, int d);
extern "C" int func_80062928(int destination, int source, int size);
extern "C" int func_80062998(int first, int second, int third);
extern "C" int func_801BFA64(int index);
extern "C" int func_800A807C();
extern "C" void func_800A98A8(int index);
extern "C" void func_80062758(u8* obj, int arg);
extern "C" void func_800B1EC8();
extern "C" void func_8007D834__Q22cf13CfGameManagerFv(cf::CfResPcMgrView* manager);
extern "C" void func_800BBB50(cf::CfResPcParent* parent);
extern "C" void func_800BE1A4(cf::CfResPcParent* parent);
extern "C" void func_8018F63C(void* self);
extern "C" cf::CfResPcEntry38* func_80062EC4(int index);
extern "C" int func_801BFE20(int a, int b, u8* c, float f1, float f2);

// C-ABI imports used by func_8018F368 (slot reset) and func_8018D65C /
// func_8018E69C (resource lookup / notify helpers, defined in CfRes.cpp).
extern "C" int func_80062B3C(u8* handle, u32 state);
extern "C" cf::CfResPcLookupEntry* func_80062C88(int state);
extern "C" cf::CfResPcLookupEntry* func_80062D44(int state);
extern "C" cf::CfResPcLookupEntry* func_80062CE4(int state);
extern "C" cf::CfResPcLookupEntry* func_80062DA4(int state);
extern "C" cf::CfResPcLookupEntry* func_80062E04(int state);
extern "C" cf::CfResPcLookupEntry* func_80062E64(int state);
extern "C" void func_800638B4(int state);
extern "C" void func_80063994(int a, int b);
extern "C" cf::CfResPcSoundSlotEntry* func_801BFAE4(u16 handle);
extern "C" void func_800BAB64(cf::CfResPcParent* parent);
extern "C" void* func_80063038();

// C-ABI imports used by func_eu_8018E19C / func_8018CE70 / func_8018F164 /
// func_8018F5A4 (defined in CfRes.cpp / CfSoundMan.cpp /
// CfGameManagerUnityHelpers.hpp). extern "C" keeps the call-site relocs at
// the plain retail names.
extern "C" u8* CfRes_getInstanceField();
extern "C" void func_eu_80063174(int a, u8* b);
extern "C" void func_801BFE8C(int a, int b, int c);
extern "C" int func_800625A0(int a, int b);
extern "C" int func_80061A80(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f);
extern "C" void func_8007E864__Q22cf13CfGameManagerFv(u32 first, u32 second);

// C-ABI imports used by func_8018F1FC (defined in CfGameManager.cpp / the
// CfGameManagerUnityHelpers.hpp wrappers / code_80187F14.cpp). The Fv-mangled
// CfGameManager methods are called with a vestigial int arg here (retail
// `li r3, X` before the bl); extern "C" keeps the call relocs at the retail
// names (same convention as code_8018F8D8.cpp / CPartyStateWin.hpp).
extern "C" void func_8007FE18__Q22cf13CfGameManagerFv(int flag);
extern "C" void func_80085334__Q22cf13CfGameManagerFv(u32 value);
extern "C" void func_80083328__Q22cf13CfGameManagerFv(cf::CfGameManager* object, u32 first, u32 second);
extern "C" void func_8007C140__Q22cf13CfGameManagerFv(int arg);
extern "C" int func_8018892C(int index);
extern "C" void func_8004302C(int a, int b);

// Game-manager flag words read/written by func_8018F2EC / func_8018F520 /
// func_8018F164 / func_8018F1FC.
extern u32 lbl_eu_80663E24;
extern u32 lbl_eu_80663E28;
extern u16 lbl_eu_80663E46;
extern u16 lbl_eu_80663E42;  // current area id (written by func_8018F1FC)
extern u16 lbl_eu_80663E44;  // current area sub-id (written by func_8018F1FC)

// C-ABI import (defined in CfTFile.cpp, retail C name func_80069ACC): loads
// the file-event archive for a packed token. extern "C" keeps the call-site
// reloc at the plain retail name (CfTFile.hpp does not declare it).
extern "C" int func_80069ACC(cf::CfTFile* self, u32 param);

// C-ABI imports used by func_8018F018 / func_8018F63C (defined in
// CfTFile.cpp / CfRes.cpp / CfScript.cpp / CfCamEvent_1.cpp / the model
// layer). extern "C" keeps the call-site relocs at the plain retail names.
extern "C" u32 func_80069C14(u8* self);
extern "C" void func_800699B0(cf::CfTFile* self);
extern "C" void func_80069A78(cf::CfTFile* dst, cf::CfTFile* src);
extern "C" void func_80062600();
extern "C" u32 func_80061870(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f);
extern "C" int func_8007B0A0(int val);
extern "C" void func_800B06C8();
extern "C" void func_80062860(u32 a, u32 b);
extern "C" void func_80068B9C();
extern "C" u32 func_8009CF8C(u32 resourceId);
extern "C" void func_8016E164(u32 a, u32 b);
extern "C" void func_800B7410();

// Stack-buffer slot query (CfGameManager helper): fills CfResPcLoadBuffer.
extern "C" void func_80080F48__Q22cf13CfGameManagerFv(u32 value, cf::CfResPcLoadBuffer* resource, u32 first, u32 second);

// C-ABI imports used by func_8018E7E4 / func_8018D79C (defined in IResInfo.cpp /
// CfGameManager.cpp / the model layer). extern "C" keeps the call-site relocs
// at the plain retail names.
extern "C" void func_80434A4C__Q23mtl10MemManagerFb(bool value);
extern "C" u8* func_80489A60(u8* global, u8* handle, int a, int b, int c, int d);
extern "C" u32 func_804873EC(void* obj, u32 bdat, u32 arg);
extern "C" void func_804875B8(void* obj, u32 bdat, f32 f1, f32 f2);
extern "C" int func_80062BAC(int handle);
extern "C" cf::CfResPcFindEntry* findResEntry(u8* self, u32 id, u32* outIndex, u32* outValue);
extern "C" void* func_800685C8(u8* self, u32 id, u32* outIdx);
extern "C" bool func_8007E908__Q22cf13CfGameManagerFv(u32 value);
extern "C" const void* func_8009E120(cf::CfResPcCharData* object, u16 value);
extern "C" u32 func_80141E90(u32 param1, s16 param2, u32 param3, u32 param4);
extern "C" void func_800BE3E8(void* object, u32 value);
extern "C" void func_800BD644(void* parent);
extern "C" int CfRes_getD80Flag();
class CMcaFile;
extern "C" void __ct__CMcaFile(CMcaFile* self, void* data);
extern "C" void* func_80495EAC(void* global, void* data, void* buf);
extern "C" void func_804838DC(void* obj, u32 arg);

// Area-id table walked by func_8018F63C (8 x 8-byte entries, .data).
extern cf::CfResPcAreaEntry lbl_eu_805327F8[8];

// Position table copied by func_8018E7E4 (.data, 0x50 bytes).
extern cf::CfResPcPosTable lbl_eu_80503B30;

// Float constant passed to the +0x48 slot by func_8018E7E4 (.sdata2).
extern const float lbl_eu_80667A60;

// Model-layer global passed as the first func_80489A60 argument (.sbss).
extern u8* lbl_eu_80663E14;

// C-ABI imports used by func_8018DE8C (defined in CfRes.cpp / IResInfo.cpp /
// the model layer / monolib scn). extern "C" keeps the call-site relocs at
// the plain retail names.
extern "C" cf::CfResPcTableEntry* func_80062C28(int id, int a);
extern "C" void func_800BBADC(cf::CfResPcParent* parent, u8* handle);
extern "C" u8* func_800584B8(u32 global, u32 id, const char* name);
extern "C" void func_8048472C(void* obj, const char* name);
extern "C" void func_804831C4(void* obj, u8* handle);
extern "C" void* func_80495E8C(u32 global, u32 id, int a, int b);
extern "C" char* func_800AA5C0(void* handle);
extern "C" void func_800BB618(cf::CfResPcParent* parent, int arg);
extern "C" void func_800BC3B0(cf::CfObjectMove* player, float value);
extern "C" void func_800BCFA0(cf::CfResPcParent* parent);
extern "C" void func_800BE12C(void* parent, int handle, int a, int b, int c);
extern "C" int func_804BE470(void* a1, void* a2, void* a3, void* a4, void* a5);
extern "C" void func_804BD94C(void* a, void* b, u32 c, u32 d, u32 e, u32 f,
                              float f1, float f2, float f3, float f4, float f5);
extern "C" void* __dynamic_cast(void* obj, long offset, const void* src_type,
                                const void* dst_type, void* src2dst);

// RTTI typeinfo pair for the __dynamic_cast in func_8018DE8C (.sdata).
extern const void* lbl_eu_806624C0;
extern const void* lbl_eu_806624D0;

// Debug/format string used by func_8018DE8C (.rodata).
extern char lbl_eu_80503BC4[];

// .sdata2 float constants used by func_8018DE8C.
extern const float lbl_eu_80667A44;
extern const float lbl_eu_80667A48;
extern const float lbl_eu_80667A4C;
extern const float lbl_eu_80667A50;
extern const float lbl_eu_80667A54;
extern const float lbl_eu_80667A58;
extern const float lbl_eu_80667A5C;
extern const float lbl_eu_80667A64;
extern const float lbl_eu_80667A68;
extern const float lbl_eu_80666B08;
extern f32 lbl_eu_8066AF20; // f32 form: must match CtrlMoveBase.hpp (10197/10505 on mixed spellings)
