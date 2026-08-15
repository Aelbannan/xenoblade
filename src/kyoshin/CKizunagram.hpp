#pragma once

#include <types.h>
#include <nw4r/lyt.h>
#include "monolib/lib/UnkClass_8045F564.hpp"
#include <revolution/gx/GXTypes.h>

struct CFileHandle;  // monolib/device/CFileHandle.hpp

class CKizunaRadar {
public:
    CKizunaRadar(void* arg);
    virtual ~CKizunaRadar();

    // +0x00: implicit vptr (set by MWCC from the virtual dtor)
    void* mArg;      // 0x04
    u32 mField08;    // 0x08
    u32 mField0C;    // 0x0C
    u32 mField10;    // 0x10
    f32 mField14;    // 0x14
    f32 mField18;    // 0x18
};

class CKizunaCur {
public:
    CKizunaCur(nw4r::lyt::ArcResourceAccessor* accessor);
    ~CKizunaCur();  // non-virtual: retail stores the manual lbl_eu_805375FC vtable label at +0 (no compiler vptr)

    void* mVtable;                               // 0x00
    nw4r::lyt::ArcResourceAccessor* mArcResAcc;  // 0x04
    u32 mField08;                                 // 0x08
    u32 mField0C;                                 // 0x0C
    u8 mField10;                                  // 0x10
    u8 mField11;                                  // 0x11
    u8 mField12;                                  // 0x12
};

class CKizunaLine {
public:
    CKizunaLine(u32 arg4, u32 arg8, u8 arg3D);
    virtual ~CKizunaLine();

    void* mVtable;      // 0x00
    u32 field4;         // 0x04
    u32 field8;         // 0x08
    u32 field0C;        // 0x0C
    u32 field10;        // 0x10
    u8 field14;         // 0x14
    u8 field15;         // 0x15
    u8 field16;         // 0x16
    u8 field17;         // 0x17
    u32 field18;        // 0x18 (pad word; func_8025CF40 copies it as a color word)
    u32 field1C;        // 0x1C
    u32 field20;        // 0x20
    u8 field24;         // 0x24
    u8 _25[0x26-0x25];
    u16 field26;        // 0x26
    f32 field28;        // 0x28
    f32 field2C;        // 0x2C
    f32 field30;        // 0x30
    u8 field34;         // 0x34
    u8 _35[0x36-0x35];
    u16 field36;        // 0x36
    f32 field38;        // 0x38
    u8 field3C;         // 0x3C
    u8 field3D;         // 0x3D
    u8 _3E[0x40-0x3E];
    f32 field40;        // 0x40
};

class CKizunaInfo {
public:
    CKizunaInfo(nw4r::lyt::ArcResourceAccessor* accessor);
    ~CKizunaInfo();

    void* mVtable;                               // 0x00
    nw4r::lyt::ArcResourceAccessor* mArcResAcc;  // 0x04
    u32 mField08;                                 // 0x08
    u32 mField0C;                                 // 0x0C
    u32 mField10;                                 // 0x10
    u8 mField14;                                  // 0x14
    u8 mField15;                                  // 0x15
    u8 mField16;                                  // 0x16
    u16 mField18;                                 // 0x18
};

class CKizunagram {
public:
    CKizunagram();
    virtual ~CKizunagram();
    void OnFileEvent();

    u8 _04[0x04];                      // 0x04
    UnkClass_8045F564 mMemRegionA;      // 0x08
    UnkClass_8045F564 mMemRegionB;      // 0x18
    u8 _28[0x8C-0x28];                 // 0x28
    u8 field_8C;                       // 0x8C
};

// ---------------------------------------------------------------------------
// Target 7 (func_80259344) support types.
// ---------------------------------------------------------------------------

// Two-word (8-byte) struct return building block for func_80259344.
struct UnkKizunaPair {
    u32 field_00;
    u32 field_04;
};

struct UnkKizunaObj59344;
struct UnkKizunaRes59344;

// Intermediate node at +0x10 of the object hanging off offset +0x0C.
struct UnkKizunaMid59344 {
    u8 _00[0x10];
    UnkKizunaObj59344* field10; // 0x10
};

// Object whose vfunc at vtable slot 15 (+0x3C) produces an UnkKizunaRes59344*.
// MWCC prepends two implicit vtable slots, so the target is the 14th declared
// virtual (declared index 13) to land at retail slot 15.
struct UnkKizunaObj59344 {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual UnkKizunaRes59344* target(int a, int b); // vtable slot 15 (+0x3C)
};;

struct UnkKizunaRes59344 {
    u8 _00[0x44];
    u32 field44; // 0x44
    u32 field48; // 0x48
};

// func_80259344 receives: +0x0C -> UnkKizunaMid59344.
struct UnkKizunaSelf59344 {
    u8 _00[0x0C];
    UnkKizunaMid59344* field0C; // 0x0C
};

// ---------------------------------------------------------------------------
// Targets 9/10 (func_80257D90 / func_80257F44) support types.
// ---------------------------------------------------------------------------

// Object with a virtual method at vtable slot 2 (+0x08) taking one int.
// MWCC prepends two implicit vtable slots, so target2 is the first declared
// virtual (declared index 0) to land at retail slot 2.
struct UnkKizunaObj57D90 {
    virtual void target2(int a); // vtable slot 2 (+0x08)
};

struct UnkKizunaSelf57D90 {
    u8 _00[0x08];
    UnkKizunaObj57D90* field8; // 0x08
};

// ---------------------------------------------------------------------------
// Targets 1/2 (func_80259228 / func_80259280) support types.
// ---------------------------------------------------------------------------

// Object whose vfunc at vtable slot 8 (+0x20) takes one u32.
// MWCC prepends two implicit vtable slots, so target8 is the 6th declared
// virtual (declared index 6) to land at retail slot 8.
struct UnkKizunaObj59228 {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void target8(u32 a); // vtable slot 8 (+0x20)
};

// Self layout for func_80259228 / func_80259280 (sets byte flag + anim child).
struct UnkKizunaSelf59228 {
    u8 _00[0x0C];
    UnkKizunaObj59228* field0C; // 0x0C
    u32 field10;                 // 0x10
    u8 field14;                  // 0x14
    u8 field15;                  // 0x15
    u8 _16[0x40-0x16];
    f32 field40;                 // 0x40
};

// ---------------------------------------------------------------------------
// Target 5 (func_80257EE0) support types.
// ---------------------------------------------------------------------------

// Object with a virtual method at vtable slot 14 (+0x38) taking one int.
// MWCC prepends two implicit slots, so callSlot14 is declared index 12.
struct UnkKizunaObjSlot14 {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9();
    virtual void v10();
    virtual void v11();
    virtual void callSlot14(u32 a); // vtable slot 14 (+0x38)
};

struct UnkKizunaSelf57EE0 {
    u8 _00[0x08];
    UnkKizunaObjSlot14* field8;        // 0x08
    nw4r::lyt::AnimTransform* field0C; // 0x0C
    u8 field10;                        // 0x10
};

// ---------------------------------------------------------------------------
// Target 7 (func_802592D8) support types.
// ---------------------------------------------------------------------------

struct UnkKizunaSelf592D8 {
    u8 _00[0x10];
    nw4r::lyt::AnimTransform* field10; // 0x10
};

// ---------------------------------------------------------------------------
// Targets 8/9 (func_8025CAE4 / func_8025CB50) support types.
// ---------------------------------------------------------------------------

// Opaque first argument of func_80231848 (object whose +0x44/+0x48 receive
// the copied pair).
struct UnkKizunaFunc31848Obj;

// Object stored at +0x10 of the +0xB4 holder; passed to func_80231848.
struct UnkKizunaFieldB4 {
    u8 _00[0x10];
    UnkKizunaFunc31848Obj* field10; // 0x10
};

// func_8025CAE4/CB50 self: sub (at +0x68) is the func_80259344 argument whose
// +0x0C field (at +0x74) is the slot-14-callable child object.
struct UnkKizunaSelfCAE4 {
    u8 _00[0x68];
    UnkKizunaSelf59344 sub;              // 0x68
    nw4r::lyt::AnimTransform* field78;   // 0x78
    u8 _7C[0xB4-0x7C];
    UnkKizunaFieldB4* fieldB4;           // 0xB4
};

// ---------------------------------------------------------------------------
// Target 1 (func_80257B6C) support type.
// ---------------------------------------------------------------------------

// func_80257B6C self: the line layout at +0x08 (Animate + root pane), two
// reference panes at +0x0C/+0x10, and the +0x14 Vec2 filled by func_80257AFC.
struct UnkKizunaSelf57B6C {
    u8 _00[0x08];
    nw4r::lyt::Layout* field8;   // 0x08
    nw4r::lyt::Pane* field0C;    // 0x0C
    nw4r::lyt::Pane* field10;    // 0x10
    f32 field14[2];              // 0x14
};

// ---------------------------------------------------------------------------
// Target 10 (func_80257AFC) support types.
// ---------------------------------------------------------------------------

// Result of the slot-15 virtual; copyVEC2 source lives at +0x4C.
struct UnkKizunaLCBRes57AFC {
    u8 _00[0x4C];
    f32 vec[2]; // 0x4C
};

// Object whose vfunc at vtable slot 15 (+0x3C) takes (const char*, u32) and
// returns an UnkKizunaLCBRes57AFC*. Declared index 13.
struct UnkKizunaObjSlot15 {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual UnkKizunaLCBRes57AFC* slot15(const char* s, u32 n); // vtable slot 15 (+0x3C)
};

// Layout holder: the slot-15 object is at +0x10 of the layout pointer.
struct UnkKizunaLayoutSub57AFC {
    u8 _00[0x10];
    UnkKizunaObjSlot15* field10; // 0x10
};

struct UnkKizunaSelfAFC {
    u8 _00[0x04];
    nw4r::lyt::ArcResourceAccessor* field4; // 0x04
    nw4r::lyt::Layout* field8;               // 0x08
    u8 _0C[0x14-0x0C];
    f32 field14[2];                          // 0x14
};

// ---------------------------------------------------------------------------
// Targets 1/2 (func_8025B958 / func_8025B9C8) support types.
// ---------------------------------------------------------------------------

// Child object whose vfuncs at retail slots 7 (+0x1C) and 11 (+0x2C) receive
// the cur value / the (cur, flag) pair. MWCC prepends two implicit vtable
// slots, so slot7 = declared idx 5 and slot11 = declared idx 9.
struct UnkKizunaObjB958 {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void slot7(u32 a);             // retail slot 7 (+0x1C)
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void slot11(u32 a, u32 b);     // retail slot 11 (+0x2C)
};

// Self layout for func_8025B958 / func_8025B9C8 (mode byte at +0x14, child at
// +0x08, value at +0x0C / +0x10).
struct UnkKizunaSelfB958 {
    u8 _00[0x08];
    UnkKizunaObjB958* field8;  // 0x08
    u32 field0C;               // 0x0C
    u32 field10;               // 0x10
    u8 field14;                // 0x14
    u8 field15;                // 0x15
    u8 field16;                // 0x16
};

// ---------------------------------------------------------------------------
// Target 3 (func_8025C510) support type: two file handles (0x28 / 0x2C).
// ---------------------------------------------------------------------------
struct UnkKizunaSelfC510 {
    u8 _00[0x28];
    CFileHandle* field28;      // 0x28
    CFileHandle* field2C;      // 0x2C
};

// ---------------------------------------------------------------------------
// Target 4 (func_8025C7FC) support type.
// ---------------------------------------------------------------------------
struct UnkKizunaSelfC7FC {
    u8 _00[0x39];
    u8 field39;                // 0x39
    u8 _3A[0x3C-0x3A];
    u8 field3C;                // 0x3C
    u8 _3D[0x4C-0x3D];
    UnkKizunaSelfB958 sub4C;   // 0x4C
    u8 _64[0x68-0x64];
    UnkKizunaSelf59228 sub68;  // 0x68
    u8 _7C[0xAC-0x7C];
    UnkKizunaSelf57D90 subAC;  // 0xAC
};

// ---------------------------------------------------------------------------
// Target 5 (func_8025CE00) support type.
// ---------------------------------------------------------------------------
struct UnkKizunaSelfCE00 {
    u8 _00[0x3A];
    u8 field3A;                // 0x3A
    u8 _3B[0x40-0x3B];
    f32 field40;               // 0x40
    f32 field44;               // 0x44
    f32 field48;               // 0x48
    u8 _4C[0x68-0x4C];
    UnkKizunaSelf57D90 sub68;  // 0x68
    u8 _74[0x80-0x74];
    u32 field80;               // 0x80
    u32 field84;               // 0x84
    u32 field88;               // 0x88
};

// ---------------------------------------------------------------------------
// Target 6 (func_8025C21C) support types.
// ---------------------------------------------------------------------------

// Child object with retail slots 8 (+0x20) and 14 (+0x38); declared idx 6 / 12.
struct UnkKizunaObjC21C {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void target8(u32 a);     // retail slot 8 (+0x20)
    virtual void v7();
    virtual void v8();
    virtual void v9();
    virtual void v10();
    virtual void v11();
    virtual void target14(u32 a);    // retail slot 14 (+0x38)
};

struct UnkKizunaSelfC21C {
    u8 _00[0x08];
    UnkKizunaObjC21C* field8;        // 0x08
    u32 field0C;                     // 0x0C
    nw4r::lyt::AnimTransform* field10; // 0x10
    u8 field14;                      // 0x14
    u8 field15;                      // 0x15
};

// ---------------------------------------------------------------------------
// Targets 7/8 (func_8025AB04 / func_8025AB84) support type.
// ---------------------------------------------------------------------------
struct UnkKizunaSelfAB {
    u8 _00[0x26];
    u16 field26;               // 0x26
    u8 _28[0x34-0x28];
    u8 field34;                // 0x34
    u8 _35[0x38-0x35];
    f32 field38;               // 0x38
};

// ---------------------------------------------------------------------------
// Target 9 (func_8025C6F0) support type.
// ---------------------------------------------------------------------------
struct UnkKizunaSelfC6F0 {
    u8 _00[0x08];
    UnkClass_8045F564 mRegA;                 // 0x08
    UnkClass_8045F564 mRegB;                 // 0x18
    CFileHandle* field28;                    // 0x28
    CFileHandle* field2C;                    // 0x2C
    nw4r::lyt::ArcResourceAccessor* field30; // 0x30
    nw4r::lyt::ArcResourceAccessor* field34; // 0x34
    u8 field38;                              // 0x38
    u8 _39[0x4C-0x39];
    UnkKizunaSelf57D90 sub4C;                // 0x4C
    u8 _58[0x68-0x58];
    UnkKizunaSelf57D90 sub68;                // 0x68
    u8 _74[0xAC-0x74];
    UnkKizunaSelf57D90 subAC;                // 0xAC
    u8 _B8[0xC0-0xB8];
    UnkKizunaSelf57D90 subC0;                // 0xC0
};

// ---------------------------------------------------------------------------
// Target 10 (func_80257E58) support types.
// ---------------------------------------------------------------------------

// Child layout object with retail slots 11 (+0x2C) and 14 (+0x38); declared idx 9 / 12.
struct UnkKizunaObj57E58 {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void slot11(u32 a, u32 b);  // retail slot 11 (+0x2C)
    virtual void v10();
    virtual void v11();
    virtual void slot14(u32 a);         // retail slot 14 (+0x38)
};

struct UnkKizunaSelf57E58 {
    u8 _00[0x04];
    nw4r::lyt::ArcResourceAccessor* field4; // 0x04
    UnkKizunaObj57E58* field8;              // 0x08
    nw4r::lyt::AnimTransform* field0C;      // 0x0C
};

// ---------------------------------------------------------------------------
// Targets 11/12/13 (func_8025C874 / func_8025C904 / func_8025C994) types.
// ---------------------------------------------------------------------------

// 3-float vector used with the code80135FDC_setVec3 C-ABI helper. The helper
// writes x/y/z at the passed pointer and leaves the pointer in r3, so callers
// copy the returned struct by value (retail emits lwz/stw word copies).
struct UnkKizunaVec3 {
    f32 x;
    f32 y;
    f32 z;
};

// func_8025C874/C904/C994 self: mode byte at +0x3A, sub-objects at +0x4C /
// +0x68 / +0xAC, and a u16 counter at +0x8E.
struct UnkKizunaSelfC874 {
    u8 _00[0x3A];
    u8 field3A;                   // 0x3A
    u8 _3B[0x4C - 0x3B];
    UnkKizunaSelf57D90 sub4C;     // 0x4C
    u8 _58[0x68 - 0x58];
    UnkKizunaSelf57D90 sub68;     // 0x68
    u8 _74[0x8E - 0x74];
    u16 field8E;                  // 0x8E
    u8 _90[0xAC - 0x90];
    UnkKizunaSelf57D90 subAC;     // 0xAC
};

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------

// Writes 3 floats at dst, returns dst in r3 (C ABI helper from
// code_80135FDC.cpp).
extern "C" UnkKizunaVec3* code80135FDC_setVec3(float* dst, float x, float y, float z);

// Same-TU display-state helpers called by func_8025B870 / func_8025C874
// family (retail links these unmangled; extern "C" keeps the bl relocs
// binding to the unmangled names).
extern "C" void func_8025C16C(UnkKizunaSelfC21C* self);
extern "C" void func_8025C21C(UnkKizunaSelfC21C* self);
extern "C" void func_8025C298(UnkKizunaSelfC21C* self);
extern "C" void func_8025C348(UnkKizunaSelfC21C* self);
extern "C" void func_80259394(UnkKizunaSelf57D90* self, const UnkKizunaVec3* v);
extern "C" void func_8025BA38(UnkKizunaSelf57D90* self, u16 v);
extern "C" void func_80259820(UnkKizunaSelf57D90* self);


// Vtable symbols
// (relocated from top of header: same order, now grouped with the other imports)
extern "C" void* lbl_eu_805375FC[];
extern "C" void* lbl_eu_805375E4[];
extern "C" void* lbl_eu_805375F0[];
extern "C" void func_80124270(nw4r::lyt::Pane* pane, u32 a);
extern "C" void func_80127BC4(float* dst, const float* src); // copy 2 floats

// Position constants used by func_80257B6C (sda2 floats).
extern const float lbl_eu_8066882C;
extern const float lbl_eu_80668830;
extern "C" u32 func_80137510(nw4r::lyt::AnimTransform* anim, float frame);
extern "C" void func_80231848(UnkKizunaFunc31848Obj* self, const UnkKizunaPair* src);
extern "C" void copyVEC2(float* dst, const float* src);
extern "C" void copyVEC3(float* dst, const float* src);
extern "C" void func_8025AC1C(UnkKizunaSelfAB* self, u32 a);
extern "C" void func_8025CE00(UnkKizunaSelfCE00* self);

// ---------------------------------------------------------------------------
// Targets 15/16 (func_80259B18 / func_80259098) support types.
// ---------------------------------------------------------------------------

// Self for func_80259B18 / func_80259098: shared arc layout at +0x0C whose
// +0x10 field is the slot-15-callable root pane.
struct UnkKizunaSelf59B18 {
    u8 _00[0x0C];
    nw4r::lyt::Layout* field0C; // 0x0C
};

// View of a layout's +0x10 field (root pane) used by func_8025B670.
struct UnkKizunaLayoutView {
    u8 _00[0x10];
    nw4r::lyt::Pane* field10; // 0x10
};

// func_80259098's slot-15 result: a pane with a tag-context embedded at +0x10
// driving the pane-name iteration, and a slot-15 virtual at +0x3C returning
// child panes. MWCC prepends two implicit vtable slots, so slot15 is declared
// index 13.
struct UnkKizunaTagCtx59098 {
    u8 _00[0x04];
    const wchar_t* field4; // 0x04
};

struct UnkKizunaCtxPane59098 {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual nw4r::lyt::Pane* slot15(const char* s, u32 n); // vtable slot 15 (+0x3C)
    u8 _04[0x10 - 0x04];
    UnkKizunaTagCtx59098 ctx; // 0x10
};

// Object finalized via vtable slot 2 (+0x08) with a 1 flag (release/null
// helpers and the func_80259098 cleanup tail).
struct UnkKizunaPaneFinalize {
    virtual void target2(int a); // vtable slot 2 (+0x08)
};

// ---------------------------------------------------------------------------
// Target 2 (func_8025CF40) support types.
// ---------------------------------------------------------------------------

// Embedded line state at +0x68 of the func_8025CF40 self: layout matches
// CKizunaLine except +0x18..+0x20 are u32 color words (the source CKizunaLine
// copies its pad bytes there) and the vtable word is not overwritten.
struct UnkKizunaLineState {
    u8 _00[0x04];     // vtable slot, not copied
    u32 field4;       // 0x04
    u32 field8;       // 0x08
    u32 field0C;      // 0x0C
    u32 field10;      // 0x10
    u8 field14;       // 0x14
    u8 field15;       // 0x15
    u8 field16;       // 0x16
    u8 field17;       // 0x17
    u32 field18;      // 0x18
    u32 field1C;      // 0x1C
    u32 field20;      // 0x20
    u8 field24;       // 0x24
    u8 _25[0x26 - 0x25];
    u16 field26;      // 0x26
    f32 field28;      // 0x28
    f32 field2C;      // 0x2C
    f32 field30;      // 0x30
    u8 field34;       // 0x34
    u8 _35[0x36 - 0x35];
    u16 field36;      // 0x36
    f32 field38;      // 0x38
    u8 field3C;       // 0x3C
    u8 field3D;       // 0x3D
    u8 _3E[0x40 - 0x3E];
    f32 field40;      // 0x40
};

// func_8025CF40 self: two pointers gating the rebuild, a flag byte, the
// embedded line state at +0x68, the two slot-15 results, and the +0xDE byte.
struct UnkKizunaSelfCF40 {
    u8 _00[0x30];
    u32 field30;                  // 0x30
    u32 field34;                  // 0x34
    u8 _38[0x3B - 0x38];
    u8 field3B;                   // 0x3B
    u8 _3C[0x68 - 0x3C];
    UnkKizunaLineState lineState; // 0x68
    u8 _AC[0xCC - 0xAC];
    u32 fieldCC;                  // 0xCC
    u32 fieldD0;                  // 0xD0
    u8 _D4[0xDE - 0xD4];
    u8 fieldDE;                   // 0xDE
};

// ---------------------------------------------------------------------------
// Target 5 (func_8025B670) support type.
// ---------------------------------------------------------------------------

struct UnkKizunaSelfB670 {
    u8 _00[0x04];
    nw4r::lyt::ArcResourceAccessor* field4; // 0x04
    nw4r::lyt::Layout* field8;               // 0x08
    nw4r::lyt::AnimTransform* field0C;       // 0x0C
    nw4r::lyt::AnimTransform* field10;       // 0x10
    u8 _14[0x18 - 0x14];
    u16 field18;                             // 0x18
};

// View into the object returned by func_80452C10: vtable slot 9 (+0x24)
// yields the u32 bound into the layout's font pane by func_8013676C.
// All-pure so no vtable is emitted.
struct UnkKizunaFontView {
    virtual void vf0() = 0; // +0x08
    virtual void vf1() = 0; // +0x0C
    virtual void vf2() = 0; // +0x10
    virtual void vf3() = 0; // +0x14
    virtual void vf4() = 0; // +0x18
    virtual void vf5() = 0; // +0x1C
    virtual void vf6() = 0; // +0x20
    virtual u32 vf7() = 0;  // +0x24
};

// ---------------------------------------------------------------------------
// C-linkage imports for the current targets (retail names unmangled).
// ---------------------------------------------------------------------------

// Pane visibility check (CTitleAHelp.cpp) and GX color setter.
extern "C" bool func_801C4648(nw4r::lyt::Pane* pane);
extern "C" void func_801C4B60(GXColorS10* color, s16 r, s16 g, s16 b, s16 a);

// sinit_8025D304 color table (14 GXColorS10 entries, 8-byte stride).
extern GXColorS10 lbl_eu_806647E8;
extern GXColorS10 lbl_eu_806647F0;
extern GXColorS10 lbl_eu_806647F8;
extern GXColorS10 lbl_eu_80664800;
extern GXColorS10 lbl_eu_80664808;
extern GXColorS10 lbl_eu_80664810;
extern GXColorS10 lbl_eu_80664818;
extern GXColorS10 lbl_eu_80664820;
extern GXColorS10 lbl_eu_80664828;
extern GXColorS10 lbl_eu_80664830;
extern GXColorS10 lbl_eu_80664838;
extern GXColorS10 lbl_eu_80664840;
extern GXColorS10 lbl_eu_80664848;
extern GXColorS10 lbl_eu_80664850;

// Tag-context string helpers (CTagProcessor.cpp) and func_8025B670's font
// result source.
extern "C" const wchar_t* getContextStr(u8* self);
extern "C" const wchar_t** getContextStrPtr(u8* self);
extern "C" void* func_801276E0(const wchar_t** self, u32 arg);
extern "C" const wchar_t* func_80127670(const wchar_t** self);
extern "C" int func_801276C8(const wchar_t** a, const wchar_t** b);
extern "C" u32 func_801355BC(void);

// sprintf for func_80259B18's pane-name formatting.
extern "C" int sprintf(char* str, const char* fmt, ...);

// CDeviceFont font-object getter (retail symbol is the mangled C++ name).
extern "C" void* func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(
    u32 arg, nw4r::lyt::Layout* layout);
