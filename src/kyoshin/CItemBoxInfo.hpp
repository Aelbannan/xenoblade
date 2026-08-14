#pragma once

#include <types.h>
#include "monolib/lib/UnkClass_8045F564.hpp"

class CEventFile;
class CFileHandle;

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
// cast emits a scratch r5 vtable load (see MWCC_REFERENCE: manual vtable cast
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

// Cast-only vtable for the object returned by func_801355F4 (item-name
// system): the tag lookup method is at vtable+0x0C (raw slot 3). Real virtual
// dispatch reproduces the retail `lwz r12,0(r3); lwz r12,0xc(r12); mtctr;
// bcctrl` sequence; a manual (*(void***)obj)[3] cast colors a scratch
// register instead of the ABI r12.
struct CItemNameLookupVt {
    virtual void _v00();
    virtual void _v04();
    virtual void _v08();
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
    u8 _40[0x50];
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
struct CItemBoxQuadColor {
    s16 r, g, b, a;
    CItemBoxQuadColor() : r(0), g(0), b(0), a(0) {}
    CItemBoxQuadColor(s16 _r, s16 _g, s16 _b, s16 _a) : r(_r), g(_g), b(_b), a(_a) {}
};

// 4 selection/vertex colors (0x20 bytes) built per-slot in func_801E2928.
struct CItemBoxQuad {
    CItemBoxQuadColor col[4];
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
// unit): count byte, label string pointer, name buffer, two state bytes, an
// equip-state colour and a 0x20-byte text buffer used by the SJIS-aware
// width scan.
struct CItemBoxNameRecord {
    u32 _00;        // 0x00
    u8 count;       // 0x04
    u8 _05[3];      // 0x05
    u32 str;        // 0x08
    char name[0x20];// 0x0C
    u8 e0;          // 0x2C
    u8 e1;          // 0x2D
    u8 _2E[2];      // 0x2E
    u32 color;      // 0x30
    char text[0x20];// 0x34
    u8 _54[0x50];   // 0x54..0xA3
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
extern "C" char lbl_eu_805063BC[];
extern "C" char lbl_eu_80506380[];extern "C" void func_80136B4C(nw4r::lyt::Layout*, const char*, const char*, u32);
extern "C" int func_8026178C(void*, u32);extern "C" u32 func_8025FB10(void*, u32);
extern "C" u32 func_80137510(nw4r::lyt::AnimTransform*, float);
extern "C" void* func_8009EC9C(u32);
extern "C" void func_80124270(void*, u32);
extern "C" u32 func_801361E8(u32, const char*, u32);
extern "C" u32 func_80139358(u32);
extern "C" u32 func_801392E4(void*);
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
extern "C" void copyVEC3(void*, void*);
// C-linkage position-set helper (retail-unmangled func_80137924; same
// signature as CSysWin.hpp / CMenuPlayAward.hpp).
extern "C" void func_80137924(nw4r::math::VEC3*, nw4r::lyt::Pane*, nw4r::lyt::Pane*, nw4r::lyt::Pane*);
// More flat retail (unmangled) helpers used by the item-box renderers.
extern "C" void func_80137B44(nw4r::lyt::Layout*, const char*, u32);
extern "C" char* func_80136190(char*, char*, u32);
extern "C" char* func_8013639C(void*, char*);
extern "C" void func_80139A18(nw4r::lyt::Layout*, char*, void*, void*);
extern "C" void func_80137E7C(nw4r::lyt::Layout*, const char*, u32);
extern "C" u32 func_801392C0();
extern "C" u32 func_801392B4(u32);
extern "C" f32 func_80139C98(u32 a, u32 b, u32 c, f32 d);
extern "C" void func_801E40E8(CItemBoxInfo2*);
extern "C" void func_801E43BC(CItemBoxInfo2*, u16, void*, u16, u32);
extern "C" void func_801E27D0(u8*, void*, void*, void*);
extern "C" void* CItem_initItemImplInstances(void*);
extern "C" void func_801D62F8(void*, u32, const void*);
extern "C" void func_801D8930(CItemBoxInfo*);
// 9-byte flag-record builder: (out, unused, data) — param2 is never read in
// retail (callers pass the item-box pointer through it).
extern "C" void func_801D5AA0(CItemBoxInfo* out, void* unused, void* data);
extern "C" void func_801E37C4(CItemBoxInfo2*, void*, void*);
extern "C" u32 func_8013600C(void*, const char*, u32);
extern "C" u32 func_800A32BC();
extern "C" void func_801D4A2C(void*);
extern "C" u32 func_801C6E90(void*);
extern "C" u32 func_801D4AB0(void*);
extern "C" void func_801D6394(CItemBoxInfo*, u32, void*, u32);
extern "C" void func_801D5DA4(CItemBoxInfo*, u16, void*, u16);
extern "C" void func_801D79F8(CItemBoxInfo*, u16, void*, u16);
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
extern "C" void func_801E2638(CItemBoxInfo2*, u16, void*);
extern "C" void* func_801571FC();
extern "C" void func_80137F88(void*, u16);
extern "C" void func_80137C1C(void*, u32);
extern "C" u32 func_80136254(void*, const char*, u32);
extern "C" u32 func_80138F78(u16);
extern "C" void* func_801355F4();

// Layout-build helpers for OnFileEvent (retail mangled C++ symbols; keep
// signature shapes verbatim so the mangled names line up).
extern "C" void* createArcResourceAccessor__10CLibLayoutFv(void);
extern "C" void* func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32, nw4r::lyt::Layout*);
extern "C" void func_8013676C(nw4r::lyt::Pane*, void*);
extern "C" char* func_801355BC(void);
extern "C" char* func_80138DA4(const char* msg);
extern "C" u32 func_80138E90(u16);
extern "C" CItemBoxQuadColor func_801397AC(void*, u32);
extern u32 lbl_eu_80664098;
void func_80136E84(nw4r::lyt::Layout**, nw4r::lyt::ArcResourceAccessor*, const char*);
void func_80136F08(nw4r::lyt::Layout*, nw4r::lyt::AnimTransform**, nw4r::lyt::ArcResourceAccessor*, char*);
void func_801368C0(nw4r::lyt::Layout*, char*, u32);

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
    u16 counts[2]; // 0x22
    u8 _26[6];     // 0x26 - pad to 0x2C
};

