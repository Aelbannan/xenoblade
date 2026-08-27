#pragma once

#include <types.h>
#include "monolib/lib/UnkClass_8045F564.hpp"

class CEventFile;
class CFileHandle;
class CItemImplInstances;

namespace nw4r { namespace lyt { class DrawInfo; class AnimTransform; class ArcResourceAccessor; class Layout; class Pane; } }
namespace nw4r { namespace math { struct VEC3; } }

// C++-mangled imports from code_80135FDC.cpp: the unmangled identifiers mangle
// to the retail symbols func_801390E0__FPP11CFileHandle and
// func_80139124__FPQ34nw4r3lyt19ArcResourceAccessor (see CItemBoxLine.hpp).
void func_801390E0(CFileHandle**);
void func_80139124(nw4r::lyt::ArcResourceAccessor*);

// Fake SI interface for the nw4r::lyt::Layout deleting-destructor dispatch at
// vtable slot 2 (2 hidden RTTI prefix slots, so the first declared virtual is
// at +0x8). Real virtual dispatch reproduces the retail `lwz r12,0(r3);
// lwz r12,8(r12); mtctr; bcctrl` sequence; the manual `(*(void***)layout)[2]`
// cast emits a scratch r5 vtable load (see MWCC_CASES: manual vtable cast
// -> real virtual dispatch, r12 ABI register). CCur.cpp CBaseCurVt5 precedent.
struct CItemBoxLayoutDtorVt {
    virtual void destroy(u32 flags);  // slot 2 => +0x8, arg in r4
};

// Opaque item sub-record returned by CItemImplVt::_v2C: a u32 at +0 and a
// u16 flag field at +4 whose bits [16..27] select the equip state.
struct CItemBoxSubRecord {
    u32 _00;
    u16 field_04;
    u8 _06[2];
};

// Cast-only vtable interface for the object returned by
// CItem_initItemImplInstances: with -RTTI on, MWCC prepends 2 hidden RTTI
// header entries, so the first declared virtual lands at raw vtable slot 2
// (+0x08). Real virtual dispatch reproduces the retail `lwz r12,0(r3);
// lwz r12,<off>(r12); mtctr; bcctrl` sequence; the manual `(*(void***)x)[N]`
// casts color a scratch r5 instead of r12 (see CItemBoxGrid.hpp CItemInstVt08).
struct CItemImplVt {
    virtual u32 _v08(void* item);  // vtable+0x08 (raw slot 2)
    virtual void _v0C();
    virtual void _v10();
    virtual void _v14();
    virtual void _v18();
    virtual void _v1C();
    virtual void* _v20(void* item);  // vtable+0x20 (raw slot 8)
    virtual void _v24();
    virtual void _v28();
    virtual CItemBoxSubRecord* _v2C(void* item, u32 i);  // vtable+0x2C (raw slot 11)
    virtual u8 _v30(void* item);                         // vtable+0x30 (raw slot 12)
    virtual void _v34();
    virtual void _v38();
    virtual void _v3C();
    virtual s16 _v40(void* item, u32 i);                 // vtable+0x40 (raw slot 16)
    virtual void _v44();
    virtual void _v48();
    virtual u16 _v4C(void* item, u32 i);  // vtable+0x4C (raw slot 19)
    virtual void _v50();
    virtual u16 _v54(void* item);    // vtable+0x54 (raw slot 21) - equip category
    virtual void _v58();
    virtual void _v5C();
    virtual void _v60();
    virtual u8 _v64(void* item, u32 i);   // vtable+0x64 (raw slot 25)
    virtual void _v68();
    virtual void _v6C();
    virtual void _v70();
    virtual void _v74();
    virtual void _v78();
    virtual void _v7C();
    virtual void _v80();
    virtual void _v84();
    virtual void _v88();
    virtual void _v8C();
    virtual u32 _v90(void* item);    // vtable+0x90 (raw slot 36)
};

// Cast-only vtable interface for nw4r::lyt::Pane: method at vtable+0x68
// (raw slot 26) returns the vertex-colour object consumed by
// func_801D62F8. With -RTTI on, 2 hidden slots shift the first declared
// virtual to +0x08; real virtual dispatch keeps the ABI r12 vtable load.
struct CItemBoxPaneVt {
    virtual void _v08();
    virtual void _v0C();
    virtual void _v10();
    virtual void _v14();
    virtual void _v18();
    virtual void _v1C();
    virtual void _v20();
    virtual void _v24();
    virtual void _v28();
    virtual void _v2C();
    virtual void _v30();
    virtual void _v34();
    virtual void _v38();
    virtual void _v3C();
    virtual void _v40();
    virtual void _v44();
    virtual void _v48();
    virtual void _v4C();
    virtual void _v50();
    virtual void _v54();
    virtual void _v58();
    virtual void _v5C();
    virtual void _v60();
    virtual void _v64();
    virtual void* getColorObject();  // vtable+0x68 (raw slot 26)
};

// Cast-only vtable for the object returned by func_801355F4 (item-name
// system): the tag lookup method is at vtable+0x0C (raw slot 3). Real virtual
// dispatch reproduces the retail `lwz r12,0(r3); lwz r12,0xc(r12); mtctr;
// bcctrl` sequence; a manual (*(void***)obj)[3] cast colors a scratch
// register instead of the ABI r12.
struct CItemNameLookupVt {
    virtual void _v00();
    virtual u32 findName(u32 tag, u32 itemId, u32 unk);  // vtable+0x0C
};

struct CItemBoxInfoState {
    u8 _00[0x04];
    UnkClass_8045F564 memRegion1;   // 0x04
    UnkClass_8045F564 memRegion2;   // 0x14
    CFileHandle* fileHandle1;       // 0x24
    CFileHandle* fileHandle2;       // 0x28
    nw4r::lyt::ArcResourceAccessor* arcResourceAccessor;     // 0x2C - nw4r::lyt::ArcResourceAccessor
    nw4r::lyt::ArcResourceAccessor* resource;                // 0x30 - second ArcResourceAccessor (released by func_80139124)
    nw4r::lyt::Layout* layout;
    nw4r::lyt::AnimTransform* animTransform1;   // 0x38 - nw4r::lyt::AnimTransform
    nw4r::lyt::AnimTransform* animTransform2;   // 0x3C - nw4r::lyt::AnimTransform
    nw4r::lyt::Pane* slotPanes[20];  // 0x40..0x8C - cached selection-row panes
                                     // (filled by CItemBoxInfo2::OnFileEvent)
    u8 active;
    u8 _91[3];
    s32 state;
    u8 current;
    u8 visible;
    u8 _9A[0x26];
    s16 values[12];
};

struct CItemBoxInfo {
    CItemBoxInfoState state;
    CItemBoxInfo(u8 arg2, u16 arg3);
    ~CItemBoxInfo();
    bool OnFileEvent(CEventFile*);

    u8 getItemBoxState();
    void advanceItemBoxState();
    void setItemBoxIndex(unsigned char index, short value);
};

// Free-function helper (retail: tryActivateItemBox__FP12CItemBoxInfo)
void tryActivateItemBox(CItemBoxInfo* info);

// Recovered free functions (retail: getItemBoxState__FP12CItemBoxInfo)
u8 getItemBoxState(CItemBoxInfo* self);

struct CItemBoxInfo2 {
    CItemBoxInfoState state;
    ~CItemBoxInfo2();
    bool OnFileEvent(CEventFile*);

    void drawItemBox2Layout(nw4r::lyt::DrawInfo* drawInfo);
    void advanceItemBox2State();
};

u8 getItemBox2State(CItemBoxInfo2* self);

struct CItemBoxInfoEntry {
    u16 itemId;
    u16 _02;
    u32 value;
    u8 state;

};

// 8-byte color (4 s16 channels) used by the item-box layout renderer.
// Colour quad used by the item-box layout renderer. The default ctor is
// zero-initializing; stack locals are built member-wise at their use sites.
struct CItemBoxQuadColor {
    s16 r, g, b, a;
    CItemBoxQuadColor() : r(0), g(0), b(0), a(0) {}
    CItemBoxQuadColor(s16 _r, s16 _g, s16 _b, s16 _a) : r(_r), g(_g), b(_b), a(_a) {}
};

// 4 selection/vertex colours (0x20 bytes) built per-slot in func_801E2928.
struct CItemBoxQuad {
    CItemBoxQuadColor col[4];
};

// POD views without ctors: func_801D6394 builds its colour pair member-wise
// at the use site (retail shows bare sth/stw sequences, no ctor calls).
struct CItemBoxColorPOD {
    s16 r, g, b, a;
};
struct CItemBoxQuadPOD {
    CItemBoxColorPOD col[4];
};

// 6-byte slot table: a u32 (bytes 0..3) + u16 (bytes 4..5), read from two
// consecutive .sdata2 constants (lbl_eu_8066805C + 80668060, etc.).
union CItemBoxSlotBytes {
    struct { u32 a; u16 b; } ab;
    u8 bytes[6];
};

// 0x1C-byte per-slot comparison record built by func_801D5274: three s16
// labels (read sign-extended by lha) plus a flat 22-byte flag region written
// at i+0 / i+7 / i+14 (the three per-slot flag banks overlap at their
// boundary bytes), copied out as one unit.
struct CItemBoxSlotFlags {
    s16 v[3];        // 0x00
    u8 flags[22];    // 0x06 (f1 @ +0, f2 @ +7, f3 @ +14)
};

// Word-array view: assigning the union copies the whole 0x1C-byte object as
// one block, reproducing the retail mtctr 8-byte-pair copy loop.
union CItemBoxSlotFlagsAny {
    CItemBoxSlotFlags rec;
    u32 w[7];
};

// 0x34-byte per-slot comparison record built by func_801D4E2C / func_801E197C
// and copied out as one unit. The three flag banks at +0x1D/+0x24/+0x2B hold
// per-slot {1,2,0} comparison results (candidate vs current item values); the
// word0 float is the func_80139C98 result and word4 the name ratio.
struct CItemBoxCompRecord {
    f32 f28;       // 0x00 (func_80139C98 result)
    s16 s2C;       // 0x04
    s16 s2E;       // 0x06
    s16 s30;       // 0x08
    s16 s32;       // 0x0A
    s16 s34;       // 0x0C
    u8 _0E[2];     // 0x0E
    f32 f38;       // 0x10
    s16 s3C;       // 0x14
    u8 _16[7];     // 0x16..0x1C
    u8 flags1[8];  // 0x1D..0x24
    u8 flags2[8];  // 0x24..0x2B
    u8 flags3[8];  // 0x2B..0x32
    u8 _32[2];     // 0x32..0x33
};


// 0xA4-byte item-name record written by func_801D5564 (copied out as one
// unit): header word, count byte, label string pointer, 0x10-byte name
// buffer, two state bytes, an equip-state colour and a 0x20-byte text buffer
// used by the SJIS-aware width scan; the 0xA4 copy drags in the unused tail.
struct CItemBoxNameRecord {
    u32 _00;        // 0x00
    u8 count;       // 0x04
    u8 _05[3];      // 0x05
    u32 str;        // 0x08
    char name[0x10];// 0x0C..0x1B
    u8 e0;          // 0x1C
    u8 e1;          // 0x1D
    u8 _1E[2];      // 0x1E
    u32 color;      // 0x20
    char text[0x20];// 0x24..0x43
    u8 _44[0x60];   // 0x44..0xA3
};

// Stack copy of the static label-pointer table (lbl_eu_8050634C): a header
// word precedes the four char* entries; padded so the 0x1C-byte move stays a
// 3-pair mtctr loop + 4-byte tail.
struct CItemBoxLabelTable {
    u32 hdr;         // 0x00
    char* labels[4]; // 0x04
    u32 tail[2];     // 0x14..0x1B
};

// u32-returning view of CItemImplVt::_v54: retail keeps the raw call result
// live in a register and narrows at each use site, so the interface must not
// force a u16 narrowing at the call.
struct CItemImplVt54 {
    virtual void _v08();
    virtual void _v0C();
    virtual void _v10();
    virtual void _v14();
    virtual void _v18();
    virtual void _v1C();
    virtual void* _v20(void* item);
    virtual void _v24();
    virtual void _v28();
    virtual CItemBoxSubRecord* _v2C(void* item, u32 i);
    virtual u8 _v30(void* item);
    virtual void _v34();
    virtual void _v38();
    virtual void _v3C();
    virtual s16 _v40(void* item, u32 i);
    virtual void _v44();
    virtual void _v48();
    virtual u16 _v4C(void* item, u32 i);
    virtual void _v50();
    virtual u32 _v54(void* item);  // vtable+0x54 (raw slot 21)
};

// 0xA4-byte item-name record written by func_801E20FC (ItemBox2 twin of
// func_801D5564, copied out as one unit): unlike CItemBoxNameRecord the
// count byte sits at +0 (no u32 prefix), so str is at +4 and the name buffer
// at +8. The two state bytes at +0x18/+0x19 and the equip colour at +0x1C
// sit inside the tail of the name buffer (retail overwrites them after the
// name sprintf), and the text buffer is at +0x20. The remaining +0x40..0xA3
// bytes are copied out but unused.
struct CItemBoxNameRecord2 {
    u8 count;        // 0x00
    u8 _01[3];       // 0x01
    u32 str;         // 0x04
    char name[0x10]; // 0x08..0x17
    u8 e0;           // 0x18
    u8 e1;           // 0x19
    u8 _1A[2];       // 0x1A
    u32 color;       // 0x1C
    char text[0x20]; // 0x20
    u8 _40[0x64];    // 0x40..0xA3
};

// 9-byte per-slot flag record written by func_801D5AA0 (u32 + u32 + u8);
// bytes [1..8] are per-slot flags for slots 0..7 and byte [8] also selects
// the item-box row-height label in func_801D77BC / func_801E2FEC. The type is
// padded to 16 bytes (retail stack slots step by 16); only 9 bytes are ever
// copied/read.
struct CItemBoxSlotFlagRecord {
    u32 a;       // 0x00
    u32 b;       // 0x04
    u8 c;        // 0x08
    u8 pad[7];   // 0x09
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// Out-record built by func_801D59C0 / func_801E2558: {u8, ptr, u8, ptr}.
// The mixed-width field stores (stb/stw) plus one whole-struct copy reproduce
// the retail four-word block copy into the caller's u32 out-buffer.
struct CItemBoxLabelRec {
    u8 a;
    u8 pad0[3];
    char* b;
    u8 c;
    u8 pad1[3];
    char* d;
};

extern "C" char lbl_eu_805063BC[];
extern "C" char lbl_eu_8050634C[];
extern "C" char lbl_eu_80506380[];extern "C" void func_80136B4C(nw4r::lyt::Layout*, const char*, const char*, u32);
extern "C" int func_8026178C(void*, u32);extern "C" u32 func_8025FB10(void*, u32);
extern "C" u32 func_80137510(nw4r::lyt::AnimTransform*, float);
extern "C" void* func_8009EC9C(u32);
extern "C" void func_80124270(void*, u32);
extern "C" u32 func_801361E8(u32, const char*, u32);
extern "C" u16 func_80139358(u32);
extern "C" u32 func_801392E4(u32);
extern "C" u32 func_801393CC(void*);
extern "C" void func_801D8B08(CItemBoxInfo*);
extern "C" void func_801D85D8(CItemBoxInfo*);
extern "C" void func_801D59C0(u32*, void*, void*);
extern "C" void func_801D1F9C(void*, u32);
extern "C" void func_801C4B60(void*, u32, u32, u32, u32);
extern "C" void __as__11_GXColorS10FRC11_GXColorS10(void* dst, const void* src);
extern "C" void Panic__Q24nw4r2dbFPCciPCce(const char*, int, const char*, ...);
extern "C" char lbl_eu_8052E590[];
extern "C" char lbl_eu_8052E568[];
extern "C" void func_801D4C9C(CItemBoxInfo*);
extern "C" void func_801D4D64(CItemBoxInfo*);
extern "C" void func_801E17EC(CItemBoxInfo2*);
extern "C" void func_801E4090(CItemBoxInfo2*);
extern "C" void func_801E3B9C(CItemBoxInfo2*);
extern "C" void func_801E2558(u32*, void*, void*);
extern "C" u32 func_801E92B8(void*, void*);
extern "C" u32 func_801DFD60(void*, void*, u32);
extern "C" u32 func_801DF610(void*, void*, u32, void*);
extern "C" double lbl_eu_80668028;
extern "C" void func_801E1868(CItemBoxInfo2*);
extern "C" void func_801E18B4(CItemBoxInfo2*);
extern "C" void func_801E1930(CItemBoxInfo2*);
extern "C" void __dt__17UnkClass_8045F564Fv(void*, int);
extern "C" void __ct__17UnkClass_8045F564Fv(void*);
extern "C" void* lbl_eu_80534B28[];
extern "C" void* lbl_eu_80534A90[];
extern "C" u8 code80135FDC_getByte_64077();
extern "C" void copyVEC3(void*, const void*);
// C-linkage position-set helper (retail-unmangled func_80137924; same
// signature as CSysWin.hpp / CMenuPlayAward.hpp).
extern "C" void func_80137924(void*, void*, void*, void*);
// More flat retail (unmangled) helpers used by the item-box renderers.
extern "C" void func_80137B44(nw4r::lyt::Layout*, const char*, u32);
extern "C" char* func_80136190(char*, char*, u32);
extern "C" void func_80139A18(nw4r::lyt::Layout*, char*, void*, void*);
extern "C" void func_80137E7C(nw4r::lyt::Layout*, const char*, u32);
extern "C" u32 func_801392C0();
extern "C" int func_801392B4(u8);
extern "C" f32 func_80139C98(u32 a, u32 b, u32 c, f32 d);
extern "C" void func_801E40E8(CItemBoxInfo2*);
extern "C" void func_801E43BC(CItemBoxInfo2*, u16, void*, u16, u32);
extern "C" void func_801E27D0(u8*, void*, void*, void*);
// cf/CfGameManager.hpp (pulled in via harness_catalog) already declares the
// singleton no-arg form of this import at global scope; MWCC rejects a second,
// differently-prototyped extern "C" declaration as an illegal overload. Keep
// the item-pointer form (same unmangled symbol) in a namespace and shim the
// call sites through it.
namespace itemimplshim {
extern "C" void* CItem_initItemImplInstances(void*);
}
#define CItem_initItemImplInstances(item) itemimplshim::CItem_initItemImplInstances(item)
extern "C" void func_801D62F8(void*, u32, const void*);
extern "C" void func_801D8930(CItemBoxInfo*);
// 9-byte flag-record builder: (out, unused, data) - param2 is never read in
// retail (callers pass the item-box pointer through it).
extern "C" void func_801D5AA0(CItemBoxInfo* out, void* unused, void* data);
extern "C" void func_801E37C4(CItemBoxInfo2*, void*, void*);
extern "C" u8 func_8013600C(const void*, const void*, u32);
extern "C" u32 func_800A32BC();
extern "C" void func_801D4A2C(void*);
extern "C" int func_801C6E90(void*);
extern "C" u32 func_801D4AB0(void*);
extern "C" void func_801D6394(CItemBoxInfo*, u32, void*, u32);
extern "C" void func_801D5DA4(CItemBoxInfo*, u16, void*, u16);
extern "C" void func_801D79F8(CItemBoxInfo*, u16, void*, u32);

// 0xA8-byte detail record built by func_801D5564 for func_801D79F8; only the
// 0xA4-byte body (offset 4..0xA7) is copied out whole by the callers (retail
// mtctr 8-byte-pair copy loop over 20 iterations).
struct CItemBoxDetailBody {
    char* text;      // 0x00 - description text pointer
    u8 kind;         // 0x04 - compared against 0x30
    u8 _05[3];
    u8 _08[0x10];
    u8 flag18;       // 0x18 - caption selector (3/4 select the count label)
    u8 _19[0x8B];    // pad to 0xA4
};
struct CItemBoxDetailRec {
    u32 _00;
    CItemBoxDetailBody body;
};

// Six per-slot equip s16 values staged from the lookup record inside the
// func_801D79F8 slot loop ([0] = record+0x26 field, [1..5] = +0x1c..+0x24).
union CItemBoxSlotVals {
    struct {
        s16 v26;
        s16 v1C;
        s16 v1E;
        s16 v20;
        s16 v22;
        s16 v24;
    };
    s16 s[6];
};

// .sdata2 table-handle pair staged once before the func_801D79F8 slot loop.
extern const u32 lbl_eu_80668030;
extern const u16 lbl_eu_80668034;
extern "C" void func_801D8058(CItemBoxInfo*, u32);
extern "C" void func_801D77BC(CItemBoxInfo*, u16);
extern "C" void func_801D8318(CItemBoxInfo*);
extern "C" void func_801D885C(CItemBoxInfo*);
extern "C" void func_801E1E0C(CItemBoxSlotFlags*, void*, void*);
extern "C" void func_801D8A88(CItemBoxInfo*);
extern "C" void func_801E2928(CItemBoxInfo2*, u16, void*, u16);
extern "C" void func_801E3228(CItemBoxInfo2*, u16, void*, u16);
extern "C" void func_801E2C5C(CItemBoxInfo2*, u16, void*, u16);
extern "C" void func_801E3730(CItemBoxInfo2*, u32);
extern "C" void func_801E2FEC(CItemBoxInfo2*, u16);
extern "C" void func_801E3918(CItemBoxInfo2*);
extern "C" void func_801E3DE4(CItemBoxInfo2*);
extern "C" void func_801E3EB8(CItemBoxInfo2*);
extern "C" void func_801E4010(CItemBoxInfo2*);
extern "C" void func_801D5C38(void*, void*, void*, void*);
extern "C" void func_801D8B60(CItemBoxInfo*);
extern "C" void func_801D8C0C(CItemBoxInfo*);
extern "C" void func_801E4194(CItemBoxInfo2*);
extern "C" void func_801E14DC(CItemBoxInfo2*, u16, void*, u16, u32);
extern "C" void func_801D80EC(CItemBoxInfo*, u16, void*);
// Slot-selection table filled by func_801E2638: arr[0] is the selected-count
// category flag, arr[1+i] the per-slot "selected" flag consumed by
// func_801E2FEC; flag2 is the extra category-gate byte.
struct CItemBoxSlotSelTable {
    u32 selWords[2];   // byte [0] = category flag, bytes [1..7] = per-slot flags
    u8 flag2;          // also selects the row-height label size (0x2c vs 0x2b)
};

// 0xA0-byte label record written by func_801E20FC and consumed by
// func_801E3228 (CItemBoxNameRecord2 minus its unused 4-byte tail).
struct CItemBoxInfo2LabelRec {
    u32 count;       // 0x00
    char* str;       // 0x04 - pane text pointer for the fixed label
    char name[0x10]; // 0x08..0x17 (name[0] checked against '0', used as %s)
    u8 e0;           // 0x18 equip-state flag
    u8 e1;           // 0x19
    u8 _1A[2];
    u32 color;       // 0x1C
    char text[0x20]; // 0x20
    u8 _40[0x60];    // 0x40..0x9F
};

// Per-slot equip-value entry staged in func_801E3228: the table handle pair
// comes from two .sdata2 constants, the six s16 values from the slot's item
// table record.
struct CItemBoxSlotEntry {
    u32 tbl;       // 0x00
    u16 h;         // 0x04
    u8 _06[2];
    s16 vals[6];   // 0x08..0x13 ([0] = +0x26 field, [1..5] = +0x1c..+0x24)
};

// Hoisted table-handle pair (lbl_eu_80668064 / lbl_eu_80668068) staged once
// before the per-slot loop in func_801E3228.
struct CItemBoxTagStage {
    u32 tbl;
    u16 h;
};

// POD (ctor-less) 8-byte colours used by func_801E2FEC: the s16 view builds
// the alpha-only defaults with individual halfword stores, the u32 view lets
// the selected-item pair move as word loads/stores (retail stw/lwz shape).
struct CItemBoxColorRaw {
    s16 r, g, b, a;
};
struct CItemBoxColorBlock {
    u32 w[2];
};

// Halfword/word dual view of one 8-byte colour: built via the s16 members,
// moved as whole words (retail lwz/stw shape).
union CItemBoxColorAny {
    CItemBoxColorRaw c;
    CItemBoxColorBlock w;
};
// .sdata2 table-handle pair staged by func_801E3228.
extern const u32 lbl_eu_80668064;
extern const u16 lbl_eu_80668068;
// 0x10-byte POD vertex-colour quad built per-slot in func_801D79F8 from
// info+0x9C..0xA8 and consumed pairwise by func_801D62F8.
struct CItemBoxVertexColors {
    u32 w[4];
};

// Unmangled retail imports (plain symbol names in the retail binary).
extern "C" void func_801E20FC(void*, void*, void*, void*);
extern "C" void func_80136A1C(nw4r::lyt::Layout*, const char*, const char*, u32);
extern "C" char* func_801394D4(u32);
extern "C" void func_801E2638(CItemBoxSlotSelTable*, CItemBoxInfo2*, void*);
extern "C" void* func_801571FC();
extern "C" void func_80137F88(void*, u32);
extern "C" void func_80137C1C(void*, u32);
extern "C" u16 func_80136254(const void*, const void*, int);
extern "C" char* func_80138F78(u32);
extern "C" nw4r::lyt::ArcResourceAccessor* func_801355F4();

// Layout-build helpers for OnFileEvent (retail mangled C++ symbols; keep
// signature shapes verbatim so the mangled names line up).
extern "C" nw4r::lyt::ArcResourceAccessor* createArcResourceAccessor__10CLibLayoutFv();
extern "C" void* func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32, nw4r::lyt::Layout*);
extern "C" void func_8013676C(nw4r::lyt::Pane*, void*);
extern "C" char* func_801355BC(void);
extern "C" char* func_80138DA4(const char* msg);
void func_80136E84(nw4r::lyt::Layout**, nw4r::lyt::ArcResourceAccessor*, const char*);
void func_80136F08(nw4r::lyt::Layout*, nw4r::lyt::AnimTransform**, nw4r::lyt::ArcResourceAccessor*, char*);
void func_801368C0(nw4r::lyt::Layout*, char*, u32);

extern "C" u32 func_80138E90(u16);
extern u32 lbl_eu_80664098;

// 8-byte GXColorS10 returned by value in r3:r4 from func_801397AC.
struct CItemBoxGXColor {
    s16 r, g, b, a;
};
// Pane/material vertex-colour getter (flat retail symbol; returns in r3:r4).
extern "C" CItemBoxGXColor func_801397AC(void* obj, int index);

// Vtable view of the font-info object from CDeviceFont::func_80452C10:
// virtual index 7 (vtable+0x24) returns the pane data bound via func_8013676C.
struct CItemBoxFontInfoVt {
    virtual void _v00();
    virtual void _v04();
    virtual void _v08();
    virtual void _v0C();
    virtual void _v10();
    virtual void _v14();
    virtual void _v18();
    virtual void* fontData();  // vtable+0x24 (raw slot 9)
};

// 0x24-byte item-box slot record (built by func_801E27D0 and copied by the
// renderers): count byte, string pointer, per-slot text pointers, a 4-byte
// gap, per-slot byte values, and a tail whose byte 1 doubles as the build
// counter. Whole-struct copies of this size compile to the retail mtctr
// 8-byte-pair copy loop.
struct CItemBoxSlotRecord {
    u8 count;      // 0x00
    u8 _01[3];     // 0x01
    u32 str;       // 0x04
    u32 text[4];   // 0x08
    u8 _18[4];     // 0x18
    u8 vals[4];    // 0x1C
    u8 tail[4];    // 0x20 (tail[1] = counter at 0x21)
};

// 0x2C-byte item-box slot record with per-slot pair counts (built by
// func_801D5C38, copied out as one unit): count byte, string pointer,
// per-slot text pointers, per-slot byte values, a counter byte at 0x21, and
// two u16 pair counts at 0x22.
struct CItemBoxSlotRecord1 {
    u8 count;      // 0x00
    u8 _01[3];     // 0x01
    u32 str;       // 0x04
    u32 text[4];   // 0x08
    u8 _18[4];     // 0x18
    u8 vals[4];    // 0x1C
    u8 _20;        // 0x20
    u8 counter;    // 0x21 (build counter)
    u16 counts[4]; // 0x22 (indexed up to counter-1)
    u8 _26[2];     // 0x2A - pad to 0x2C
};

// Word-array view: assigning the union copies the whole object as one block
// (reproduces the retail mtctr lwzu/stwu pair loop) instead of expanding
// field-by-field.
union CItemBoxSlotRecAny1 {
    CItemBoxSlotRecord1 rec;
    u32 w[11];
};

