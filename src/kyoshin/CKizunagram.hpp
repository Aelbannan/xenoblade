#pragma once

#include <types.h>
#include <nw4r/lyt.h>

// Vtable symbols
extern "C" void* lbl_eu_805375FC[];
extern "C" void* lbl_eu_805375E4[];

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
    CKizunaLine();
    virtual ~CKizunaLine();

    // TODO: add fields
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

    // TODO: add fields
    u8 field_8C; // 0x8C
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

