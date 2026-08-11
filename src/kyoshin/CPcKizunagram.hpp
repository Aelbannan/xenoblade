#pragma once

#include <types.h>

#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/work/CEventFile.hpp"

#include <nw4r/lyt.h>

// Vtable symbol for CPcKizunagram (stored by CPcKizunagramVtblBase).
extern "C" void* lbl_eu_80537618[];

// PC Kizunagram (affinity chart) per-character window.
// Layout-compatible with IWorkEvent (vptr @0) for CDeviceFile::readFile.

// Base that stores the manual vtable pointer before member construction.
// Retail __ct__CPcKizunagram writes lbl_eu_80537618 into +0x00 *before* the
// embedded UnkClass_8045F564 at +0x04 is constructed; inheriting this base
// (whose inline ctor sets mVtbl) preserves that ordering under MWCC.
struct CPcKizunagramVtblBase {
    void* mVtbl; // 0x00 - lbl_eu_80537618
    CPcKizunagramVtblBase() { mVtbl = lbl_eu_80537618; }
};

class CPcKizunagram : public CPcKizunagramVtblBase {
public:
    UnkClass_8045F564 mMemRegion;             // 0x04
    CFileHandle* mFileHandle;                 // 0x14
    nw4r::lyt::ArcResourceAccessor* mArcRes;  // 0x18
    nw4r::lyt::Layout* mLayout;               // 0x1C
    nw4r::lyt::AnimTransform* mAnimTransform; // 0x20
    u8 mStateByte1;                           // 0x24 - active flag
    u8 mStateByte2;                           // 0x25 - sub-state (0=hidden, 1=opening, 2=open, 3=closing)
    u8 mIsHidden;                             // 0x26 - set externally to hide/show
    u8 mIsOpen;                               // 0x27 - 1 when in stable visible state (state 2)
    u8 mField28;                              // 0x28
    u8 _pad29[3];                             // 0x29
    u8 mKizunaCur[0x18];                      // 0x2C - CPcKizunaCur sub-object
    u8 mByte44;                               // 0x44
    u8 _pad45[3];                             // 0x45
    f32 mFloat48;                             // 0x48
    CPcKizunagram();
    ~CPcKizunagram();
    int OnFileEvent(CEventFile* event);
};

// View into the object returned by CDeviceFont::func_80452C10: vtable+0x24
// (index 7, no args) yields the u32 bound into the layout's font pane.
// All-pure so no vtable is emitted; the call keeps the retail r12 dispatch.
class CDeviceFontVtblView {
public:
    virtual void vf0() = 0; // index 0 -> +0x08
    virtual void vf1() = 0; // index 1 -> +0x0C
    virtual void vf2() = 0; // index 2 -> +0x10
    virtual void vf3() = 0; // index 3 -> +0x14
    virtual void vf4() = 0; // index 4 -> +0x18
    virtual void vf5() = 0; // index 5 -> +0x1C
    virtual void vf6() = 0; // index 6 -> +0x20
    virtual u32 vf7() = 0;  // index 7 -> +0x24
};

// C-linkage accessors matching retail unmangled symbols.
extern "C" u8 func_8025DA40(CPcKizunagram* pKizunagram);
extern "C" u8 func_8025DA48(CPcKizunagram* pKizunagram);

// ---------------------------------------------------------------------------
// CPcKizunaCur - the per-character cursor sub-object (0x18 bytes) embedded at
// offset 0x2C of CPcKizunagram. Layout-compatible with CBaseCur; the vtable
// pointer is stored explicitly (lbl_eu_805376AC) by the constructor.
// ---------------------------------------------------------------------------

// Vtable symbol for CPcKizunaCur.
extern "C" void* lbl_eu_805376AC[];

class CPcKizunaCur {
public:
    void* mVtable;                            // 0x00
    nw4r::lyt::ArcResourceAccessor* mAccessor; // 0x04
    nw4r::lyt::Layout* mpLayout;              // 0x08
    nw4r::lyt::AnimTransform* mpAnim0;        // 0x0C
    nw4r::lyt::AnimTransform* mpAnim1;        // 0x10
    u8 mField14;                              // 0x14
    u8 mField15;                              // 0x15
    u8 mField16;                              // 0x16

    ~CPcKizunaCur();
};

// Retail ctor symbol __ct__CPcKizunaCur is UNMANGLED (no class-length prefix),
// so it cannot be expressed as a real member ctor (MWCC would mangle the
// call-site reloc to __ct__12CPcKizunaCurFP...). The C-linkage form below
// reproduces the retail name and keeps the call site a direct bl.
extern "C" void __declspec(noinline) __ct__CPcKizunaCur(CPcKizunaCur* self,
                                                        nw4r::lyt::ArcResourceAccessor* accessor);

// ---------------------------------------------------------------------------
// Support types for the free functions in this TU (not part of CPcKizunagram).
// ---------------------------------------------------------------------------

// Large object func_8025EE7C / func_8025EE94 operate on (u32 field at 0x89C).
// func_8025EDC8 clears an array of 11 slots (stride 0xC4) plus tail fields.

// Per-0x20 block in an affinity slot: only the low byte of word and byte14
// carry state; the rest is memset to zero. Word-granular access at +0x00
// (retail uses lwz/stw on this field) requires a u32 member.
struct CPcKizunaSlotEntry {           // 0x20 bytes
    u32 word;                         // 0x00 (low byte = value)
    u16 field04;                      // 0x04 - per-entry runtime id (BDAT key)
    u8  field06[2];                   // 0x06 .. 0x07
    u32 field08;                      // 0x08 - runtime word (func_8025EE94)
    u16 field0C;                      // 0x0C - runtime halfword (func_8025F2E8)
    u8  field0E[2];                   // 0x0E .. 0x0F
    f32 field10;                      // 0x10 - runtime float (func_8025F2E8)
    u8  byte14;                       // 0x14
    u8  field15[3];                   // 0x15 .. 0x17
    CPcKizunaSlotEntry* pField18;     // 0x18 - runtime linked-list next
    CPcKizunaSlotEntry* pField1C;     // 0x1C - runtime linked-list prev
};

struct CPcKizunaSlot {                // 0xC4 bytes
    CPcKizunaSlotEntry data00;        // 0x00
    CPcKizunaSlotEntry sub[5];        // 0x20 .. 0xBF (5 blocks)
    u8 byteC0;                        // 0xC0
    u8 _padC1[3];                     // 0xC1
};

// Compact per-slot form (stride 0xD) produced by func_8025EC0C from a
// CPcKizunagramBig, inverted by func_8025ECE4. Byte order matches the packing
// loops: [0]=data00.lo, [1]=data00.b14, [2,4,6,8,10]=sub[k].lo,
// [3,5,7,9,11]=sub[k].b14, [0xC]=byteC0.
struct CPcKizunaCompactSlot {         // 0xD bytes
    u8 bytes[0xD];
};

// func_8025EC0C/ECE4 container: 11 compact slots then the reused tail fields.
struct CPcKizunaCompact {             // 0xAD bytes
    CPcKizunaCompactSlot slots[0xb];  // 0x00 .. 0x8E
    u8 _pad8F;                        // 0x8F
    u32 field_0x90;                   // 0x90 (from field_0x884)
    u8 data94[0x14];                  // 0x94 .. 0xA7 (from data888)
    u32 field_0xA8;                   // 0xA8 (from field_0x89C)
};

struct CPcKizunagramBig {
    CPcKizunaSlot slots[0xb];           // 0x000 .. 0x86C
    u32 field_0x86C;                    // 0x86C
    u8 data870[0x14];                   // 0x870 .. 0x884
    u32 field_0x884;                    // 0x884
    u8 data888[0x14];                   // 0x888 .. 0x89C
    int field_0x89C;                    // 0x89C
};

// Chart object shared by func_8025F2E8/F528/F768/F9AC (starts at work+0x3534).
// The per-character working slot array at 0x3D4 overlaps the tail of the total
// slot array (0x000, searched for duplicate ids) and the persistence bitmap
// (0x870) in the retail object, so the two views are a union.
struct CPcKizunaChart {
    union {
        struct {
            CPcKizunaSlot searchSlots[0xb];  // 0x000 - total affinity chart
            u32 field_0x86C;                 // 0x86C
            u8  data870[0x14];               // 0x870 - persistence bitmap
        };
        struct {
            u8  _pad000[0x3D4];              // 0x000 .. 0x3D3
            CPcKizunaSlot workSlots[0xb];    // 0x3D4 - current chart entries
        };
    };
};

// Working-copy entry view: for a total-chart position (slot a, sub b-1), the
// matching working entry sits 0x3D4 bytes past that position. Retail keeps the
// position as the base register and reads the entry fields as +0x3D4
// displacements, so field accesses go through this padded view.
struct CPcKizunaWorkEntryPos {
    u8 _pad[0x3D4];               // 0x000
    CPcKizunaSlotEntry entry;     // 0x3D4
};

// func_8025D6E0: nested pointer chain -> leaf struct holding a Vec3 at 0x2C.
struct CPcKizunaTreeLeaf {
    u8 _00[0x2C];
    f32 x;                                    // 0x2C
    f32 y;                                    // 0x30
    f32 z;                                    // 0x34
};

struct CPcKizunaTreeMid {
    u8 _00[0x10];
    CPcKizunaTreeLeaf* field10;               // 0x10
};

struct CPcKizunaTreeRoot {
    u8 _00[0x08];
    CPcKizunaTreeMid* field8;                 // 0x08
};

struct CPcKizunaVec3 {
    f32 x;
    f32 y;
    f32 z;
};

// ---------------------------------------------------------------------------
// C-linkage imports (relocated from the pseudo-import block in
// CPcKizunagram.cpp). Names are the retail symbol names - do not rename or
// "fix" the mangled forms, and keep extern "C" linkage.
// ---------------------------------------------------------------------------

// Intra-TU callees defined in CPcKizunagram.cpp (free unmangled functions).
extern "C" void func_8025DCFC(CPcKizunagram* self);
extern "C" void func_8025E3A4(CPcKizunagram* self, u32 arg);
extern "C" void func_8025E4A4(CPcKizunagram* self);
extern "C" void func_8025DC08(CPcKizunagram* self);
extern "C" void func_8025DC8C(CPcKizunagram* self);
extern "C" void func_8025DCB0(CPcKizunagram* self);
extern "C" void func_8025E56C(CPcKizunagram* self);
extern "C" void func_8025E5A8(CPcKizunagram* self);
extern "C" void func_8025E5E4(CPcKizunagram* self, u32 value);
extern "C" void func_8025EE94(CPcKizunagramBig* self);
extern "C" void func_8025F9AC(CPcKizunaChart* self, int a, int b);
extern "C" int func_8025E904(CPcKizunagram* self, const void* table, int val);
extern "C" int func_8025E9E4(CPcKizunagram* self, const void* table, int id);
extern "C" int func_8025E960(CPcKizunagram* self, const void* table, int id);
extern "C" void* getFP__FPCc(const char* path);
extern "C" u32 func_8003B1EC(void*);

// Sound / effect helper (C-linkage retail symbol func_80138078__FUl).
extern "C" void func_80138078__FUl(u32 arg);

// Layout animation helpers from code_80135FDC (retail unmangled func_80137510).
extern "C" u32 func_80137510(nw4r::lyt::AnimTransform*, float);

// BDAT table pointer (loaded via sda21 by func_8025EE94 / func_8025F2E8).
extern "C" u32 lbl_eu_8066415C;
extern "C" u32 lbl_eu_80664158;

// Format-string constants blended into a column-name buffer (func_8025EE94).
extern "C" u32 lbl_eu_806688A8;
extern "C" u32 lbl_eu_806688AC;

// BDAT column lookup (unmangled C-linkage retail symbol).
extern "C" u32 getBdatStringColumnValue(void* bdat, const char* col, s32 index);

// nw4r layout/animation loaders used by the cursor init (func_8025D4E4).
extern "C" void func_80136E84__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc(
    nw4r::lyt::Layout** dst, nw4r::lyt::ArcResourceAccessor* acc, const char* name);
extern "C" void func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
    nw4r::lyt::Layout* lay, nw4r::lyt::AnimTransform** dst, nw4r::lyt::ArcResourceAccessor* acc, char* name);
extern "C" void* func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32 arg, nw4r::lyt::Layout* layout);
extern "C" bool Attach__Q34nw4r3lyt19ArcResourceAccessorFPvPCc(
    nw4r::lyt::ArcResourceAccessor* self, void* data, const char* name);
extern "C" nw4r::lyt::ArcResourceAccessor* createArcResourceAccessor__10CLibLayoutFv();
extern "C" void func_80434A4C__Q23mtl10MemManagerFb(bool value);
extern "C" void* getHandleMEM2__Q23mtl10MemManagerFv();
extern "C" void func_8013676C(void*, u32);

// Scratch region guard (RAII Class_8045F858): explicit buffer + C-ABI ctor/dtor
// calls so the retail stack slot and the -1 dealloc flag match exactly.
extern "C" void __ct__14Class_8045F858FP17UnkClass_8045F564(Class_8045F858* self,
                                                            UnkClass_8045F564* base);
extern "C" void __dt__14Class_8045F858Fv(Class_8045F858* self, int dealloc);

// BDAT / random / pane-text helpers with unmangled retail names.
extern "C" u16 func_80136254(const void*, const void*, int);
extern "C" char* func_80136190(const void*, const void*, int);
extern "C" char* func_8013639C(const void*, const void*, int);
extern "C" u8 func_801361E8(u32, const char*, u32);
extern "C" int func_801C4648(void);
extern "C" u16 func_8013606C(const void*, const void*, u32);
extern "C" u32 func_8009CF8C(u32);
extern "C" u32 func_801355BC(void);
extern "C" void func_80137F88(nw4r::lyt::Pane*, void*);
extern "C" void func_80137C1C(void*, u32);
extern "C" void func_8013AB0C(u8*, u8*, int);
extern "C" int sprintf(char*, const char*, ...);
extern "C" u32 lbl_eu_80668898;
extern "C" u32 lbl_eu_8066889C;
extern "C" u32 lbl_eu_806688A0;
extern "C" u32 lbl_eu_806688A4;
extern "C" void func_80136910__FPQ34nw4r3lyt6LayoutPcUc(nw4r::lyt::Layout*, char*, u8);
extern "C" void func_80137924(void*, void*, void*, void*);
extern "C" void func_80124270(void*, u32);

// Cursor destroy helper (external retail symbol, not in this TU).
extern "C" void func_8025D688(CPcKizunaCur* cur);

// Fixed 8-entry cursor-row ordering (signed bytes) kept in .sdata2 as two
// separate u32 words (retail loads lbl_eu_80668888 and lbl_eu_8066888C via
// @sda21 individually).
extern "C" void* func_8003AA34(void);
struct S8Bytes {
    union {
        u32 w[2];   // copied as two words (lwz/stw pair into the local)
        s8  b[8];   // byte access for the order lookups (retail lbzx sign-extends)
    };
};
extern u32 lbl_eu_80668888;
extern u32 lbl_eu_8066888C;
