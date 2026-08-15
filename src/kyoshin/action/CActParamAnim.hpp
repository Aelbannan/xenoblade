#pragma once

#include <types.h>

#include "kyoshin/action/CActParamData.hpp"
#include "monolib/math/CVec3.hpp"  // ml::CVec3 for func_80053490 (same-TU helper)

// Full object layout for C-linkage accessors
struct CActParamAnimFull {
    u8 _00[0x10];
    u8 mChildData[0x4AE]; // from +0x10 to +0x4BE
    u8 mEffByte0;          // +0x4BE
    u8 mEffByte1;          // +0x4BF
    u8 _4C0[4];            // +0x4C0: eff obj pointer
};

// Cast-only view of the anim-state region (+0x394..+0x440) plus the flags
// word at +0x0C. Used by the C-linkage state helpers below.
// (Forward-declared owner type: CActParamAnimOwnerIf is defined below.)
struct CActParamAnimOwnerIf;
struct CActParamAnimStateView {
    u8 _pad_00[0x08];           // +0x00..+0x07
    CActParamAnimOwnerIf* owner08;  // +0x08: owner object
    u32 field0C;                // +0x0C: flags
    u8 mChildData10[0x24 - 0x10];   // +0x10..+0x23 (embedded CActParamData base, start)
    f32 field24;                // +0x24 (anim scale, func_8004CC8C)
    u8 _pad_28[0x30 - 0x28];    // +0x28..+0x2F
    s32 field30;                // +0x30 (copied to field374 in func_8004D194)
    u8 _pad_34[0x270 - 0x34];   // +0x34..+0x26F (rest of embedded CActParamData base)
    u32 field270;               // +0x270
    u8 _pad_274[0x278 - 0x274]; // +0x274..+0x277
    u32 field278;               // +0x278
    u32 field27C;               // +0x27C
    u8 _pad_280[0x28C - 0x280]; // +0x280..+0x28B
    u32 field28C;               // +0x28C (anim id used by func_8004E9EC dispatch)
    u8 _pad_290[0x2A4 - 0x290]; // +0x290..+0x2A3
    u32 field2A4;               // +0x2A4
    u8 _pad_2A8[0x2BC - 0x2A8]; // +0x2A8..+0x2BB
    u32 field2BC;               // +0x2BC
    u8 _pad_2C0[0x2FC - 0x2C0]; // +0x2C0..+0x2FB
    u32 field2FC;               // +0x2FC
    u8 _pad_300[0x304 - 0x300]; // +0x300..+0x303
    f32 field304;               // +0x304 (sub-object anim value, func_8004CC8C)
    u8 _pad_308[0x33C - 0x308]; // +0x308..+0x33B
    u32 field33C;               // +0x33C
    u8 _pad_340[0x370 - 0x340]; // +0x340..+0x36F
    s32 field370;               // +0x370
    s32 field374;               // +0x374: anim counter (setAnimCounter)
    s32 field378;               // +0x378
    s32 field37C;               // +0x37C (guard counter, func_8004F484)
    f32 field380;               // +0x380
    f32 field384;               // +0x384 (anim blend, func_8004FE58)
    f32 field388;               // +0x388 (anim speed scale, func_8004CF00)
    f32 field38C;               // +0x38C (func_8004CC8C)
    f32 field390;               // +0x390: anim speed (func_8004CF00 stores here)
    f32 field394;               // +0x394: anim speed
    f32 field398;               // +0x398
    f32 field39C;               // +0x39C
    u8* object3A0;              // +0x3A0: attached sub-object
    u32 state3A4;               // +0x3A4: sub-object state
    u32 field3A8;               // +0x3A8
    u32 field3AC;               // +0x3AC
    u32 field3B0;               // +0x3B0
    u32 field3B4;               // +0x3B4
    u32 field3B8;               // +0x3B8
    u32 field3BC;               // +0x3BC
    u8 _pad_3C0[4];             // +0x3C0
    f32 field3C4;               // +0x3C4
    u8 _pad_3C8[0x3CC - 0x3C8]; // +0x3C8..+0x3CB
    f32 field3CC;               // +0x3CC (anim direction x, func_8004E9EC)
    u8 _pad_3D0[0x3D4 - 0x3D0]; // +0x3D0..+0x3D3
    f32 field3D4;               // +0x3D4 (anim direction z, func_8004E9EC)
    u8 _pad_3D8[0x430 - 0x3D8]; // +0x3D8..+0x42F
    f32 field430;               // +0x430
    f32 field434;               // +0x434
    u8 _pad_438[0x440 - 0x438]; // +0x438..+0x43F
    f32 field440;               // +0x440
    f32 field444;               // +0x444
    f32 field448;               // +0x448 (func_8004CC8C: anim blend accumulator)
    f32 field44C;               // +0x44C
    f32 field450;               // +0x450
    f32 field454;               // +0x454
    f32 field458;               // +0x458
    u8 _pad_45C[0x460 - 0x45C]; // +0x45C..+0x45F
    f32 field460;               // +0x460
    f32 field464;               // +0x464
    f32 field468;               // +0x468
    f32 field46C;               // +0x46C
    f32 field470;               // +0x470
    f32 field474;               // +0x474
    f32 field478;               // +0x478
    f32 field47C;               // +0x47C
    f32 field480;               // +0x480
    f32 field484;               // +0x484
    f32 field488;               // +0x488: last anim value (stored as f32)
    u8 _pad_48C[0x49C - 0x48C]; // +0x48C..+0x49B
    f32 field49C;               // +0x49C (snap position, func_8004E9EC)
    f32 field4A0;               // +0x4A0
    u8 _pad_4A4[0x4B0 - 0x4A4]; // +0x4A4..+0x4AF
    u32 field4B0;               // +0x4B0 (func_8004CF00: cleared each frame)
    u8 _pad_4B4[0x4BD - 0x4B4]; // +0x4B4..+0x4BC
    u8 field4BD;                // +0x4BD
    u8 field4BE;                // +0x4BE
    u8 _pad_4BF[0x4C4 - 0x4BF]; // +0x4BF..+0x4C3
    u32 field4C4;               // +0x4C4
    s32 field4C8;               // +0x4C8 (anim request/owner id, -1 = none)
    f32 field4CC;               // +0x4CC
    f32 field4D0;               // +0x4D0
    s16 field4D4;               // +0x4D4 (signed anim guard, lha in retail)
    s16 field4D6;               // +0x4D6 (frame counter)
    s16 field4D8;               // +0x4D8 (anim-stay counter, func_8004CF00)
    u8 field4DA;                // +0x4DA
    u8 _pad_4DB;                // +0x4DB
    u16 field4DC;               // +0x4DC
    u8 _pad_4DE[0x4E4 - 0x4DE]; // +0x4DE..+0x4E3
    u32 field4E4;               // +0x4E4
};

// Word-copy view of the sub-object returned by func_8048315C: the anim
// position triple lives at +0xC..+0x14 (func_8004B52C copies it as words).
struct CActParamAnimObjVec {
    u8 _pad_00[0x0C];
    u32 x;   // +0xC
    u32 y;   // +0x10
    u32 z;   // +0x14
};

// Raw 32-bit word view of a position/scale triple. func_8004B354/B40C copy
// these as plain words (lwz/stw in retail), while field488 in the state view
// reinterprets the y word as f32.
struct CActParamAnimData3 {
    u32 x;
    u32 y;
    u32 z;
};

// Word view of the position triple at +0x3C0..+0x3C8. func_8004FAB4/F884
// store the rotated anim-axis result here as plain words (lwz/stw in retail).
struct CActParamAnimPosView {
    u8 _pad_000[0x3C0];
    u32 x;   // +0x3C0
    u32 y;   // +0x3C4
    u32 z;   // +0x3C8
};

// Float-typed view of the anim-state region used by the func_8004B114
// initializer: offsets that CActParamAnimStateView declares as u32 (word
// copies in func_8004B354/B40C) are written/read as floats here (lfs/stfs
// in retail), and the tail region +0x3C8..+0x4E4 is exposed as named fields.
struct CActParamAnimInitView {
    u8 _pad_00[0x04];
    u32 field04;            // +0x04
    u32 field08;            // +0x08
    u32 field0C;            // +0x0C
    u8 _pad_10[0x370 - 0x10];
    s32 field370;           // +0x370
    s32 field374;           // +0x374
    s32 field378;           // +0x378
    s32 field37C;           // +0x37C
    f32 field380;           // +0x380
    f32 field384;           // +0x384
    f32 field388;           // +0x388
    f32 field38C;           // +0x38C
    f32 field390;           // +0x390
    f32 field394;           // +0x394
    f32 field398;           // +0x398
    f32 field39C;           // +0x39C
    u8* object3A0;          // +0x3A0
    u32 state3A4;           // +0x3A4
    f32 field3A8;           // +0x3A8 (float view)
    f32 field3AC;           // +0x3AC (float view)
    f32 field3B0;           // +0x3B0 (float view)
    f32 field3B4;           // +0x3B4 (float view)
    f32 field3B8;           // +0x3B8 (float view)
    f32 field3BC;           // +0x3BC (float view)
    f32 field3C0;           // +0x3C0
    f32 field3C4;           // +0x3C4
    f32 field3C8;           // +0x3C8
    f32 field3CC;           // +0x3CC
    f32 field3D0;           // +0x3D0
    f32 field3D4;           // +0x3D4
    f32 field3D8;           // +0x3D8
    f32 field3DC;           // +0x3DC
    f32 field3E0;           // +0x3E0
    f32 field3E4;           // +0x3E4
    f32 field3E8;           // +0x3E8
    f32 field3EC;           // +0x3EC
    f32 field3F0;           // +0x3F0
    f32 field3F4;           // +0x3F4
    f32 field3F8;           // +0x3F8
    f32 field3FC;           // +0x3FC
    f32 field400;           // +0x400
    f32 field404;           // +0x404
    f32 field408;           // +0x408
    f32 field40C;           // +0x40C
    f32 field410;           // +0x410
    f32 field414;           // +0x414
    f32 field418;           // +0x418
    f32 field41C;           // +0x41C
    f32 field420;           // +0x420
    f32 field424;           // +0x424
    f32 field428;           // +0x428
    f32 field42C;           // +0x42C
    f32 field430;           // +0x430
    f32 field434;           // +0x434
    f32 field438;           // +0x438
    f32 field43C;           // +0x43C
    f32 field440;           // +0x440
    f32 field444;           // +0x444
    f32 field448;           // +0x448
    f32 field44C;           // +0x44C
    f32 field450;           // +0x450
    f32 field454;           // +0x454
    f32 field458;           // +0x458
    f32 field45C;           // +0x45C
    f32 field460;           // +0x460
    f32 field464;           // +0x464
    f32 field468;           // +0x468
    f32 field46C;           // +0x46C
    f32 field470;           // +0x470
    f32 field474;           // +0x474
    f32 field478;           // +0x478
    f32 field47C;           // +0x47C
    f32 field480;           // +0x480
    f32 field484;           // +0x484
    f32 field488;           // +0x488
    f32 field48C;           // +0x48C
    f32 field490;           // +0x490
    f32 field494;           // +0x494
    u8 field498;            // +0x498
    u8 _pad_499[0x49C - 0x499];
    f32 field49C;           // +0x49C
    f32 field4A0;           // +0x4A0
    f32 field4A4;           // +0x4A4
    u32 field4A8;           // +0x4A8
    u32 field4AC;           // +0x4AC
    u32 field4B0;           // +0x4B0
    u32 field4B4;           // +0x4B4
    u32 field4B8;           // +0x4B8
    u8 field4BC;            // +0x4BC
    u8 field4BD;            // +0x4BD
    u8 field4BE;            // +0x4BE
    u8 field4BF;            // +0x4BF
    u32 field4C0;           // +0x4C0
    u32 field4C4;           // +0x4C4
    s32 field4C8;           // +0x4C8
    f32 field4CC;           // +0x4CC
    f32 field4D0;           // +0x4D0
    s16 field4D4;           // +0x4D4
    s16 field4D6;           // +0x4D6
    s16 field4D8;           // +0x4D8
    u8 field4DA;            // +0x4DA
    u8 _pad_4DB;            // +0x4DB
    u16 field4DC;           // +0x4DC
    u8 _pad_4DE[0x4E4 - 0x4DE];
    u32 field4E4;           // +0x4E4
};

// Cast-only view of the attached sub-object region read by func_8004CC8C:
// the anim divisor value at +0x304.
struct CActParamAnimSubObjView {
    u8 _pad_000[0x304];
    f32 field304;   // +0x304
};

// C-linkage callees in sibling units (retail names, keep verbatim).
extern "C" float func_80484F18(u8* object);
extern "C" int func_8049798C(u8* object);
extern "C" float func_80485174(u8* object);
extern "C" int func_80485244(u8* object);
extern "C" int func_80485464(u8* object, u32 param);
extern "C" u32 func_80054170(u8* data, u32* out, u32 param, u32 byte, u32 flag);
extern "C" u32 func_80054614(u8* data, u32* out, u32 param, u32 flag, u32 zero);
extern "C" int func_80054A24(u8* data, u32 param);
// C-linkage callees for func_8004CF00 (retail names, keep verbatim).
extern "C" void func_80053F7C(void* data, f32 a, f32 b);
extern "C" void func_80055EE4(void* data);
extern "C" void func_80054A3C(void* data);
extern "C" void func_80055F08(void* data);
extern "C" void func_8004C608(void* self);
extern "C" void func_8004CC8C(void* self);
extern "C" f32 func_80496288(void* obj);
extern "C" void func_80484E5C(void* self, f32 value);
extern "C" int func_804BE398(void* vec, u32 a, u32 b, u32 c, f32 d, f32 e);
extern "C" void func_804BE4B4(void* out, int a);
extern "C" void func_804BE4E0(void* out, int a);

// Cast-only interface for the owner object at +0x08: MWCC places the first
// user virtual at vt+0x08 (two RTTI entries at vt+0x00/0x04), so the 4th
// virtual lands at vt+0x14, the slot func_8004B4A4 dispatches through.
struct CActParamAnimOwnerIf {
    virtual int v0();
    virtual int v1();
    virtual int v2();
    virtual int v3();
    u32 field04;    // +0x04 (bit 3 tested in func_8004D7EC)
    u8 _pad[0x0C];  // +0x08..+0x13
    f32 field14;    // +0x14
    u8 _pad_18[0x24 - 0x18]; // +0x18..+0x23
    u32 field24;    // +0x24 (func_8004D950 stores it to field4C8)
};

// Vtable-view for the +0x14 dispatch in func_8004FFBC: with two hidden RTTI
// slots the 3rd user virtual (index 3) sits at vt+0x14 and returns a float.
struct CActParamAnimVt14 {
    virtual int f00();
    virtual int f01();
    virtual int f02();
    virtual f32 f03();  // +0x14
};

// Vtable-view for the +0x0C dispatch in func_8004FCE0: the second user
// virtual (vt+0x0C after two RTTI slots) returns a float that is compared
// against lbl_eu_80665EF0.
struct CActParamAnimVt0C {
    virtual int f00();
    virtual f32 f01();  // +0x0C
};

// Vtable-view for the +0xE4 dispatch in func_8004CF00 (55 filler user
// virtuals, then the dispatch target at vt+0xE4).
struct CActParamAnimVtE4 {
    virtual int f00();
    virtual int f01();
    virtual int f02();
    virtual int f03();
    virtual int f04();
    virtual int f05();
    virtual int f06();
    virtual int f07();
    virtual int f08();
    virtual int f09();
    virtual int f10();
    virtual int f11();
    virtual int f12();
    virtual int f13();
    virtual int f14();
    virtual int f15();
    virtual int f16();
    virtual int f17();
    virtual int f18();
    virtual int f19();
    virtual int f20();
    virtual int f21();
    virtual int f22();
    virtual int f23();
    virtual int f24();
    virtual int f25();
    virtual int f26();
    virtual int f27();
    virtual int f28();
    virtual int f29();
    virtual int f30();
    virtual int f31();
    virtual int f32();
    virtual int f33();
    virtual int f34();
    virtual int f35();
    virtual int f36();
    virtual int f37();
    virtual int f38();
    virtual int f39();
    virtual int f40();
    virtual int f41();
    virtual int f42();
    virtual int f43();
    virtual int f44();
    virtual int f45();
    virtual int f46();
    virtual int f47();
    virtual int f48();
    virtual int f49();
    virtual int f50();
    virtual int f51();
    virtual int f52();
    virtual int f53();
    virtual int f54();
    virtual int dispatchE4();  // +0xE4
};

// Vtable-view for the +0xEC dispatch in func_8004FE58 (57 filler user
// virtuals, then the dispatch target at vt+0xEC).
struct CActParamAnimVtEC {
    virtual int f00();
    virtual int f01();
    virtual int f02();
    virtual int f03();
    virtual int f04();
    virtual int f05();
    virtual int f06();
    virtual int f07();
    virtual int f08();
    virtual int f09();
    virtual int f10();
    virtual int f11();
    virtual int f12();
    virtual int f13();
    virtual int f14();
    virtual int f15();
    virtual int f16();
    virtual int f17();
    virtual int f18();
    virtual int f19();
    virtual int f20();
    virtual int f21();
    virtual int f22();
    virtual int f23();
    virtual int f24();
    virtual int f25();
    virtual int f26();
    virtual int f27();
    virtual int f28();
    virtual int f29();
    virtual int f30();
    virtual int f31();
    virtual int f32();
    virtual int f33();
    virtual int f34();
    virtual int f35();
    virtual int f36();
    virtual int f37();
    virtual int f38();
    virtual int f39();
    virtual int f40();
    virtual int f41();
    virtual int f42();
    virtual int f43();
    virtual int f44();
    virtual int f45();
    virtual int f46();
    virtual int f47();
    virtual int f48();
    virtual int f49();
    virtual int f50();
    virtual int f51();
    virtual int f52();
    virtual int f53();
    virtual int f54();
    virtual int f55();
    virtual int f56();
    virtual int dispatchEC();  // +0xEC
};

// Vtable-view for the +0x80 dispatch in func_8004D194. With two hidden RTTI
// slots at vt+0x00/0x04 the first user virtual sits at vt+0x08, so slot 0x80
// is the 30th user virtual (30 filler virtuals, then the dispatch target). A
// real virtual call through the cast emits retail's lwz r12,0 / lwz r12,0x80
// / mtctr / bcctrl (manual ((void**)vt)[0x80/4] indexing emits a scratch
// register instead - MWCC_REF vtable dispatch).
struct CActParamAnimVt80 {
    virtual int f00();
    virtual int f01();
    virtual int f02();
    virtual int f03();
    virtual int f04();
    virtual int f05();
    virtual int f06();
    virtual int f07();
    virtual int f08();
    virtual int f09();
    virtual int f10();
    virtual int f11();
    virtual int f12();
    virtual int f13();
    virtual int f14();
    virtual int f15();
    virtual int f16();
    virtual int f17();
    virtual int f18();
    virtual int f19();
    virtual int f20();
    virtual int f21();
    virtual int f22();
    virtual int f23();
    virtual int f24();
    virtual int f25();
    virtual int f26();
    virtual int f27();
    virtual int f28();
    virtual int f29();
    virtual int dispatch80(u32 arg);  // +0x80
};

class CActParamAnim {
public:
    CActParamAnim();
    virtual ~CActParamAnim();
    void func_8004B114();
    void func_8004D2F8();
    void func_8004D4AC();
    void func_8004D950();
    void func_8004DAE0();
    void func_8004DDD0();
    void func_8004DF9C();
    void func_8004E168();
    void func_8004E334();
    void func_8004E828();
    void func_8004E9EC();
    void func_8004ECF4() const;
    void func_8004F484();
    void func_8004F5FC();
    void func_8004F884();
    void func_8004FAB4();
    void func_8004FCE0();
    void func_8004FE58();
    void func_8004FFBC();
    void func_80050890();
    void func_80050DB0();
    void func_800510FC();
    void func_80052934();

    // Converted free functions → members
    void* getChild();
    float getBlendWeight();
    int checkFlag(int mask);
    void* getOwner();
    float getScale();
    void updateSubAnim();
    void setOwner(int val);
    float getAnimSpeed() const;
    void copyVec3To3C0(const float* src);
    void setVec3Y(float val);
    int checkAnimFlag();
    float func_8004B854();
    float func_8004B86C();
    void callObjUpdate();
    float func_8004B898();
    int callObjFunc(void* param);
    void* getNextChainObj();
    void setBlendFlag(int param);
    void* getModelObj();
    int checkRenderFlag();
    void setAnimCounter(int value);
    void* getEffObj();
    void clearEffObj();
    void func_8004C5E8();
    void resetVec3Y();
    void setActiveFlag(s32 param);
    void startAnimA();
    void startAnimA2();
    void startAnimB();
    void startAnimC();
    void stopAnim();
    u32 checkFlag13();
    float getParamFloat();
    float getField10();
    u32 checkFlag16();
    u32 testAndClearFlag18();
    void mulVec3Y(float param_2);
    int getChainInt();
    bool func_80053990();
    bool func_80053998();
    bool func_800539A0();
    bool func_800539A8();
    bool func_800539B0();
    bool func_800539B8();
    bool func_800539C0();
    bool func_800539C8();
    bool func_800539D0();
    bool func_800539D8();

private:
    u8 _pad_04[0x08];      // +0x04..+0x0B
    u32 mField0C;          // +0x0C: zero-initialized in ctor
    CActParamData mChildData; // +0x10: embedded CActParamData sub-object
};

// Same-TU helper (target us-80053ac8), forward-declared for func_8004FFBC.
// extern "C" so the call-site reloc keeps the retail name verbatim.
extern "C" int func_80053490(CActParamAnim* self, const ml::CVec3* dirParam);

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void __dl__FPv(void* object);
extern "C" void* func_8048315C(void* object);
extern "C" void func_8004B9D4(CActParamAnim* self, u32, u32, s32, u32);
extern "C" void func_8004BDCC(CActParamAnim* self, u32 a, u32 b, u32 c, u32 d);
extern "C" void func_80053B24(void* a, void* b, void* c, void* d, void* e);
extern "C" bool func_80055B88(void* data);
extern "C" void func_80055AC4(void* data);
extern "C" u32 func_804BD94C(void* a, void* b, u32 c, u32 d, u32 e, u32 f,
                              f32 g, f32 h, f32 i, f32 j, f32 k);
extern "C" {
extern float lbl_eu_80665ECC;
}
extern "C" u8 lbl_eu_80663D4C;
// sdata2 float constants used by the anim start/setup helpers (f32, loaded
// via lfs in retail).
extern float lbl_eu_80665EB8;
extern const float lbl_eu_80665E9C;
extern double lbl_eu_80665EE8; // 2^52 conversion magic (u32->f32 trick)
extern const float lbl_eu_80665EA0; // const: lets MWCC hoist the sdata2 load like a pool constant (MWCC_REF §SDA hoist)
extern const float lbl_eu_80665ED8; // fidx scale (func_800526C0 / func_8004CC68)
extern const float lbl_eu_80665F00; // half-angle (func_800526C0)
extern float lbl_eu_80665F18;
extern const float lbl_eu_80665E98; // (func_8004B114 init)
extern const float lbl_eu_80665EA4; // (func_8004B114 init)
extern const float lbl_eu_80665EA8; // (func_8004B114 init)
extern const float lbl_eu_80665EAC; // (func_8004B114 init)
extern const float lbl_eu_80665EB0; // (func_8004B114 init / func_800504DC clamp)
extern const float lbl_eu_80665EB4; // (func_8004B114 init / func_800504DC)
extern const float lbl_eu_80665EBC; // (func_8004B114 init: pre-multiplied with 8066A210)
extern const float lbl_eu_8066A210; // (func_8004B114 init: pre-multiplied with 80665EBC)
extern const float lbl_eu_80665EC0; // (func_8004B9D4 angle normalization divisor)
extern const float lbl_eu_80665F04; // (func_800504DC rand scale)
extern const float lbl_eu_80665F1C; // (func_800504DC angle threshold)
extern const float lbl_eu_80665F20; // (func_800504DC angle threshold)
extern const float lbl_eu_80665F24; // (func_800504DC angle threshold)
extern const float lbl_eu_80665F28; // (func_800504DC clamp bound)
extern const float lbl_eu_80665F2C; // (func_800504DC clamp scale)
extern const float lbl_eu_80665F30; // (func_800504DC clamp bound)
extern const float lbl_eu_80665F34; // (func_800504DC clamp bound)
extern const float lbl_eu_80665F38; // (func_800504DC clamp bound)
extern const float lbl_eu_80665F40; // (func_800504DC clamp bound)
extern const float lbl_eu_80665F44; // (func_800504DC clamp scale)
extern const float lbl_eu_80665F48; // (func_800504DC clamp bound)
extern const float lbl_eu_80665EF8; // (func_800504DC clamp bound / func_8004ECF4 gate)
extern const float lbl_eu_80665EFC; // (func_800504DC angle threshold / func_8004ECF4 gate)
extern const float lbl_eu_80665EF0; // 10.0f (anim blend threshold, func_8004FE58/FCE0)
extern const float lbl_eu_80665F08; // (func_8004ECF4 anim clamp bound)
extern const float lbl_eu_80665F10; // (func_8004FFBC anim angle bound)
extern const float lbl_eu_80665F0C; // (func_8004ECF4/FFBC speed limit scale)
extern float lbl_eu_80663D48; // (sdata anim gate, sinit_800539E0 + func_8004ECF4)
extern const float lbl_eu_80665EC4; // (func_8004CC8C)
extern const float lbl_eu_80665EC8; // (func_8004CC8C)
extern const float lbl_eu_80665EDC; // (func_8004ECF4/FFBC speed limit scale)
extern const float lbl_eu_80665EE0; // (func_8004CC8C: anim blend threshold)
extern float lbl_eu_80665F3C;
extern float lbl_eu_80665F50;
extern float lbl_eu_80665F54;
extern const float lbl_eu_80665F14; // (func_80053490 acos-angle epsilon)
extern const float lbl_eu_80665F5C; // (func_80053490 dot threshold)
extern float lbl_eu_80665F6C;
extern float lbl_eu_80665F70;
extern float lbl_eu_8066A1F8; // pi (angle wrap, func_8004BC28)
extern float lbl_eu_8066A1FC; // two*pi (angle wrap, func_8004BC28)
extern const float lbl_eu_8066A20C; // angle gate scale (func_80050F5C / func_800512A8) - const to match CfObjectModel.hpp's declaration (MWCC rejects const/non-const redeclaration)

// nw4r debug strings used by the FSqrt-style assert (file/msg pair).
extern const char lbl_eu_80526324[];
extern const char lbl_eu_80526300[];
// nw4r acos-domain assert strings (func_80053490, line 0xef).
extern const char lbl_eu_805262F0[];
extern const char lbl_eu_805262C8[];

