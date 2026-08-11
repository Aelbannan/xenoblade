#pragma once

#include <types.h>
#include "kyoshin/cf/CfGimmick.hpp"

namespace cf {

// Region/collision sub-object embedded in CfGimmickLock at 0x8C and 0x140.
// Its methods live in the 0x804B0xxx library (not decompiled in this repo);
// the lock only reads/writes the state word and timer below.
struct CfGimmickLockSub {
    u8 pad_00[0xAC];   // 0x00..0xAB
    u32 state;         // 0xAC
    u8 pad_B0[2];      // 0xB0..0xB1
    u16 timer;         // 0xB2
};

// Player object base (list-object pointer minus 0x3E9C) read by func_8020D204.
struct CfGimmickLockPlayer {
    u8 pad_00[0x63C];
    f32 field_63C;              // 0x63C - read by func_8020C640 (distance addend)
    u8 pad_640[0x3374 - 0x640];
    u32 flags3374;              // 0x3374 - per-player "locked" bit word
    u8 pad_3378[0x456C - 0x3378];
    u16 id456C;                 // 0x456C - lock id (>> 4 compared to lockId[])
};

// Lock gimmick.  Flattened layout (CfGimmick base fields inlined) so the
// region sub-objects land at their retail offsets 0x8C and 0x140.
class __declspec(novtable) CfGimmickLock {
public:
    virtual ~CfGimmickLock();   // vtable slot 0
    virtual void virt01();      // slot 1
    virtual void virt02();      // slot 2
    virtual void virt03();      // slot 3
    virtual void virt04();      // slot 4
    virtual void virt05();      // slot 5
    virtual void vinit();       // declared slot 6, dispatched at offset 0x20

    // vptr at 0x00 (implicit; written directly by the ctor/dtor).
    /* 0x04 */ CfGimmickVec3 position;
    /* 0x10 */ CfGimmickVec3 rotation;
    /* 0x1C */ CfGimmickVec3 scale;
    /* 0x28 */ u8 pad_28[0x24];
    /* 0x4C */ f32 extent0;
    /* 0x50 */ f32 extent1;
    /* 0x54 */ f32 extent2;
    /* 0x58 */ f32 extent3;
    /* 0x5C */ u32 pad_5C;
    /* 0x60 */ u32 stateIndex;
    /* 0x64 */ u16 rowId;
    /* 0x66 */ u16 configFlags;
    /* 0x68 */ u16 field_68;
    /* 0x6A */ u16 resourceId;
    /* 0x6C */ u16 minVal;
    /* 0x6E */ u16 maxVal;
    /* 0x70 */ u8 lod[4];
    /* 0x74 */ u32 flags;
    /* 0x78 */ void* field_78;
    /* 0x7C */ u32 field_7C;
    /* 0x80 */ u16 field_80;
    /* 0x82 */ u16 typeId;
    /* 0x84 */ u16 lockId[4];
    /* 0x8C */ CfGimmickLockSub subA;
    /* 0x140 */ CfGimmickLockSub subB;
    /* 0x1F4 */ f32 field_1F4;
    /* 0x1F8 */ s16 field_1F8;   // unlock timer (signed - read with lha in func_8020C640)
};

// Function-pointer types for the vtable slots the lock invokes on the
// game-manager object stored in field_78 (see func_8020CFD0).
typedef void (*CfGimmickLockSetPosFn)(void*, const f32*);
typedef void (*CfGimmickLockSetYFn)(void*, f32);
typedef void (*CfGimmickLockSetScaleFn)(void*, f32);
typedef void* (*CfGimmickLockGetTargetFn)(void*);

// Cast-only interface for the linked game-manager object (field_78) and the
// fight-list node objects.  MWCC vtable slots are shifted +2 (RTTI headers),
// so a method at declared slot N dispatches at offset (N+2)*4.  Declared
// slots below are chosen so the real dispatch offsets match the retail calls:
//   0x9c setPosition (real slot 39 = declared 37)
//   0xc4 setY        (real slot 49 = declared 47)
//   0xdc setScale    (real slot 55 = declared 53)
//   0x110 getTarget  (real slot 68 = declared 66)
struct CfGimmickLockObj {
    virtual ~CfGimmickLockObj();      // declared 0
    virtual void d01();               // declared 1
    virtual void d02();               // declared 2
    virtual void d03();               // declared 3
    virtual void d04();               // declared 4
    virtual void d05();               // declared 5
    virtual void d06();               // declared 6
    virtual void d07();               // declared 7
    virtual void d08();               // declared 8
    virtual void d09();               // declared 9
    virtual void d10();               // declared 10
    virtual void d11();               // declared 11
    virtual void d12();               // declared 12
    virtual void d13();               // declared 13
    virtual void d14();               // declared 14
    virtual void d15();               // declared 15
    virtual void d16();               // declared 16
    virtual void d17();               // declared 17
    virtual void d18();               // declared 18
    virtual void d19();               // declared 19
    virtual void d20();               // declared 20
    virtual void d21();               // declared 21
    virtual void d22();               // declared 22
    virtual void d23();               // declared 23
    virtual void d24();               // declared 24
    virtual void d25();               // declared 25
    virtual void d26();               // declared 26
    virtual void d27();               // declared 27
    virtual void d28();               // declared 28
    virtual void d29();               // declared 29
    virtual void d30();               // declared 30
    virtual void d31();               // declared 31
    virtual void d32();               // declared 32
    virtual void d33();               // declared 33
    virtual void d34();               // declared 34
    virtual void d35();               // declared 35
    virtual void d36();               // declared 36
    virtual void setPos(const f32* p);   // declared 37 -> 0x9c
    virtual void d38();               // declared 38
    virtual void d39();               // declared 39
    virtual void d40();               // declared 40
    virtual void d41();               // declared 41
    virtual void d42();               // declared 42
    virtual void d43();               // declared 43
    virtual void d44();               // declared 44
    virtual void d45();               // declared 45
    virtual void d46();               // declared 46
    virtual void setY(f32 y);         // declared 47 -> 0xc4
    virtual void d48();               // declared 48
    virtual void d49();               // declared 49
    virtual void d50();               // declared 50
    virtual void d51();               // declared 51
    virtual void d52();               // declared 52
    virtual void setScale(f32 s);     // declared 53 -> 0xdc
    virtual void d54();               // declared 54
    virtual void d55();               // declared 55
    virtual void d56();               // declared 56
    virtual void d57();               // declared 57
    virtual void d58();               // declared 58
    virtual void d59();               // declared 59
    virtual void d60();               // declared 60
    virtual void d61();               // declared 61
    virtual void d62();               // declared 62
    virtual void d63();               // declared 63
    virtual void d64();               // declared 64
    virtual void d65();               // declared 65
    virtual void* getTarget();        // declared 66 -> 0x110
};

} // namespace cf

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names verbatim).  Wrapped in extern "C" so
// MWCC emits unmangled reloc names (repo norm, cf. CfGimmickElv.hpp).
// ---------------------------------------------------------------------------

// Lock vtable (data TU).
extern u8 lbl_eu_80535900[];

// CfGimmick base ctor/dtor + TU helpers (defined in CfGimmick.cpp).
extern "C" void __ct__cf_CfGimmick(void* self);
extern "C" void __dt__Q22cf9CfGimmickFv(void* self, int flag);
extern "C" void func_80208EE4(cf::CfGimmickLock* self);
extern "C" void func_802089BC(void* matrix, const f32* basis, const CfGimmickVec3* point);
extern "C" void func_80208F34(void* self, void* out, void* bdat, void** holder);
extern "C" void func_80209020(void* self, void* out, void* bdat, void** holder);
extern "C" void func_80209288(void* self, void* out, void* bdat, void** holder);

// 0x804B0xxx region-object library (not decompiled here).
extern "C" void func_804B0924(void* region);
extern "C" void func_804B4BDC(void* manager, void* region);
extern "C" void func_804B4C7C(void* manager, void* region);
extern "C" int func_804B192C(void* region, void* target, int arg2, int arg3, f32 dist);
extern "C" void __dt__804B095C(void* self, int flag);
extern "C" void func_804B0B0C(void* region, const f32* vec);
extern "C" void func_804B0AD4(void* region, int flag, f32 a, f32 b);
extern "C" void func_804B0B54(void* region, const f32* vec);
extern "C" void func_804B0C0C(void* region, const f32* vec, const CfGimmickVec3* rot);

// CfGameManager object factory / player accessors.
extern "C" void* func_800817BC__Q22cf13CfGameManagerFv(u32 value, u32 unused);
extern "C" unsigned int func_800822F4__Q22cf13CfGameManagerFv(void);
extern "C" void* getPlayer__Q22cf13CfGameManagerFi(int index);

// CfGimmick-TU helpers (CfGimmick.cpp).
extern "C" int func_8020A294();
extern "C" void func_8020A484(u16 resourceId);
extern "C" unsigned int func_8020A5DC();
extern "C" void func_80209F8C();

// Scene / frame-time helpers.
extern "C" void* func_8049626C(void* a, void* b);
extern "C" f32 func_80496288(void* scene);
extern "C" void func_800ACEF8(void* obj, const void* src);
extern "C" void func_8008B95C(void* target);

// Same-TU functions (C-linkage names, defined in CfGimmickLock.cpp).
extern "C" void* __ct__cf_CfGimmickLock(cf::CfGimmickLock* self, u16 row);
extern "C" void func_8020C640(cf::CfGimmickLock* self);
extern "C" void func_8020CC9C(cf::CfGimmickLock* self);
extern "C" void func_8020CFD0(cf::CfGimmickLock* self);
extern "C" void func_8020D204(cf::CfGimmickLock* self, int flag);

// Small-data constants / globals.
extern const f32 lbl_eu_806683B0;
extern const f32 lbl_eu_806683B4;
extern const f32 lbl_eu_806683B8;
extern const f32 lbl_eu_806683BC;
extern const f32 lbl_eu_806683C0;
extern const f32 lbl_eu_806683C4;
extern void* lbl_eu_80665958;
extern void* lbl_eu_8066412C;    // bdat holder seed (sda21 pointer global)
extern u8* lbl_eu_80662798;      // sda21 pointer to the lock-id column name buffer
extern char lbl_eu_805086EC[];   // column-name string for configFlags
extern void* lbl_eu_80663E10;    // scene/manager global
extern void* lbl_eu_80663E14;    // scene/manager global
extern u32 lbl_eu_806646BC;
extern u32 lbl_eu_80663E24;
