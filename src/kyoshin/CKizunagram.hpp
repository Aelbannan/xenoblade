#pragma once

#include <types.h>
#include <nw4r/lyt.h>
#include "monolib/lib/UnkClass_8045F564.hpp"

// Vtable symbols
extern "C" void* lbl_eu_805375FC[];
extern "C" void* lbl_eu_805375E4[];
extern "C" void* lbl_eu_805375F0[];

class CKizunaRadar {
public:
    CKizunaRadar();
    virtual ~CKizunaRadar();

    // TODO: add fields
};

class CKizunaCur {
public:
    CKizunaCur(nw4r::lyt::ArcResourceAccessor* accessor);
    virtual ~CKizunaCur();

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
    u8 _18[0x24-0x18];
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
    virtual ~CKizunaInfo();

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

