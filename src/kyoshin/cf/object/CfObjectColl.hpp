#pragma once

#include <types.h>
#include <revolution/MTX.h>
#include "monolib/util/FixStr.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

// Retail data symbols referenced from this unit.
extern const float lbl_eu_80666910;
extern const float lbl_eu_80666914;
extern const float lbl_eu_80666928;
extern const float lbl_eu_80666930;
extern const float lbl_eu_8066694C;
extern const float lbl_eu_80666950;
extern char lbl_eu_804FC134[];

namespace ml { struct CVec3; }

// Presentation/event bitfield owned by CUICfManager (.sbss).

// Shared distance helper (defined in code_800A3B24.cpp); retail symbol is flat.
extern "C" float func_800A4B5C(const ml::CVec3& a, const ml::CVec3& b, const ml::CVec3& c);

namespace cf {

// ---------------------------------------------------------------------------
// Real class tree, recovered from the retail vtable lbl_eu_80528600
// (= split1.s __vt__Q22cf12CfObjectColl, 0x178 bytes: RTTI, 0, 92 slots).
//
// Slot ownership follows the linker symbols that fill each entry in the
// retail table (JP __RTTI__ / *_typestr confirm the classes):
//
//   cf::CObjectState   +0x08..+0x38 (UnkVirtualFunc1..13)
//   cf::CObjectParam   +0x3C..+0x50 (UnkVirtualFunc1..6)
//   cf::CfObject       +0x54..+0x174 (dtor slot + the UnkVirtualFunc zoo)
//   cf::CfObjectPoint  overrides +0x48/+0x60/+0x68/+0x70/+0x9C/+0xA0/
//                                 +0x158/+0x160 (loadPointData..AC604)
//   cf::CfObjectColl   overrides +0x54/+0x58/+0x5C/+0x6C/+0xA8/+0xB8
//
// Intermediate tables are never constructed as most-derived in this TU and no
// compiler __vt__ survives the build (postprocess strips .data and points the
// dtor's lis/addi vptr store at the retail label), so only displacement
// positions and the definitions compiled here matter.
// ---------------------------------------------------------------------------

class CObjectState {
public:
    virtual void CObjectState_UnkVirtualFunc1();    // +0x08
    virtual void CObjectState_UnkVirtualFunc2();    // +0x0C
    virtual void CObjectState_UnkVirtualFunc3();    // +0x10
    virtual void CObjectState_UnkVirtualFunc4();    // +0x14
    virtual void CObjectState_UnkVirtualFunc5();    // +0x18
    virtual void CObjectState_UnkVirtualFunc6();    // +0x1C
    virtual void CObjectState_UnkVirtualFunc7();    // +0x20
    virtual void CObjectState_UnkVirtualFunc8();    // +0x24
    virtual void CObjectState_UnkVirtualFunc9();    // +0x28
    virtual void CObjectState_UnkVirtualFunc10();   // +0x2C
    virtual void* CObjectState_UnkVirtualFunc11();   // +0x30
    virtual void CObjectState_UnkVirtualFunc12();   // +0x34
    virtual void CObjectState_UnkVirtualFunc13();   // +0x38
};

class CObjectParam : public CObjectState {
public:
    virtual void CObjectParam_UnkVirtualFunc1();    // +0x3C
    virtual void CObjectParam_UnkVirtualFunc2();    // +0x40
    virtual void CObjectParam_UnkVirtualFunc3();    // +0x44
    virtual void loadPointData();                   // +0x48 (overridden by CfObjectPoint)
    virtual void CObjectParam_UnkVirtualFunc5();    // +0x4C
    virtual void CObjectParam_UnkVirtualFunc6();    // +0x50
};

class CfObject : public CObjectParam {
public:
    virtual ~CfObject() {}                                          // +0x54 (empty; not in retail D1/D2 chain)

    virtual int resetCollTimer();                    // +0x58 (overridden by CfObjectColl)
    virtual void enableCollision();                   // +0x5C (overridden by CfObjectColl)
    virtual void updatePointState();                   // +0x60 (overridden by CfObjectPoint)
    virtual void CfObject_UnkVirtualFunc5();        // +0x64
    virtual void releasePointLink();                   // +0x68 (overridden by CfObjectPoint)
    virtual void refreshCollLink();                   // +0x6C (overridden by CfObjectColl)
    virtual void setChildPoint();                   // +0x70 (overridden by CfObjectPoint)
    virtual int CfObject_UnkVirtualFunc9();         // +0x74
    virtual void CfObject_UnkVirtualFunc10();       // +0x78
    virtual int CfObject_UnkVirtualFunc11();        // +0x7C
    virtual void CfObject_UnkVirtualFunc12();       // +0x80
    virtual void CfObject_UnkVirtualFunc13();       // +0x84
    virtual void* CfObject_UnkVirtualFunc14(float value); // +0x88
    virtual float CfObject_UnkVirtualFunc15();      // +0x8C
    virtual void* CfObject_UnkVirtualFunc16();      // +0x90
    virtual float CfObject_UnkVirtualFunc17();      // +0x94
    virtual int CfObject_UnkVirtualFunc18();        // +0x98
    virtual void func_80047814(const void* pos);          // +0x9C (overridden by CfObjectPoint)
    virtual void setPointPosition();                   // +0xA0 (overridden by CfObjectPoint)
    virtual void CfObject_UnkVirtualFunc21();       // +0xA4
    virtual void syncCollVectors(const void* pos);          // +0xA8 (see spell-out definition in the .cpp)
    virtual ml::CVec3* CfObject_UnkVirtualFunc23(); // +0xAC
    virtual void* CfObject_UnkVirtualFunc24();      // +0xB0
    virtual void CfObject_UnkVirtualFunc25();       // +0xB4
    virtual void copyCollPosition();                   // +0xB8 (overridden by CfObjectColl)
    virtual void* CfObject_UnkVirtualFunc27(void* param); // +0xBC
    virtual void* CfObject_UnkVirtualFunc28();      // +0xC0
    virtual float CfObject_UnkVirtualFunc29(float value); // +0xC4
    virtual void CfObject_UnkVirtualFunc30();       // +0xC8
    virtual void CfObject_UnkVirtualFunc31();       // +0xCC
    virtual void CfObject_UnkVirtualFunc32();       // +0xD0
    virtual void CfObject_UnkVirtualFunc33();       // +0xD4
    virtual void CfObject_UnkVirtualFunc34();       // +0xD8
    virtual void CfObject_UnkVirtualFunc35(float value); // +0xDC
    virtual float CfObject_UnkVirtualFunc36();      // +0xE0
    virtual void CfObject_UnkVirtualFunc37();       // +0xE4
    virtual int CfObject_UnkVirtualFunc38();        // +0xE8
    virtual int CfObject_UnkVirtualFunc39();        // +0xEC
    virtual int CfObject_UnkVirtualFunc40();        // +0xF0
    virtual int CfObject_UnkVirtualFunc41();        // +0xF4
    virtual int CfObject_UnkVirtualFunc42();        // +0xF8
    virtual int CfObject_UnkVirtualFunc43();        // +0xFC
    virtual int CfObject_UnkVirtualFunc44();        // +0x100
    virtual void CfObject_UnkVirtualFunc45();       // +0x104
    virtual void CfObject_UnkVirtualFunc46(void*);       // +0x108
    virtual void CfObject_UnkVirtualFunc47();       // +0x10C
    virtual void* CfObject_UnkVirtualFunc48();       // +0x110
    virtual void CfObject_UnkVirtualFunc49();       // +0x114
    virtual void CfObject_UnkVirtualFunc50();       // +0x118
    virtual int CfObject_UnkVirtualFunc51();        // +0x11C
    virtual void CfObject_UnkVirtualFunc52();       // +0x120
    virtual void CfObject_UnkVirtualFunc53();       // +0x124
    virtual void CfObject_UnkVirtualFunc54();       // +0x128
    virtual void CfObject_UnkVirtualFunc55();       // +0x12C
    virtual void CfObject_UnkVirtualFunc56();       // +0x130
    virtual void CfObject_UnkVirtualFunc57();       // +0x134
    virtual void CfObject_UnkVirtualFunc58();       // +0x138
    virtual void CfObject_UnkVirtualFunc59(float value); // +0x13C
    virtual float CfObject_UnkVirtualFunc60();      // +0x140
    virtual void CfObject_UnkVirtualFunc61();       // +0x144
    virtual int CfObject_UnkVirtualFunc62();        // +0x148
    virtual void CfObject_UnkVirtualFunc63();       // +0x14C
    virtual int CfObject_UnkVirtualFunc64(int id);  // +0x150
    virtual void CfObject_UnkVirtualFunc65(int flag); // +0x154
    virtual void setPointEnabled(u32 flag);           // +0x158 (overridden by CfObjectPoint)
    virtual void CfObject_UnkVirtualFunc67(int flag); // +0x15C
    virtual int isCollEnabled();                    // +0x160 (defined by CfObjectPoint)
    virtual int CfObject_UnkVirtualFunc69();        // +0x164
    virtual void CfObject_UnkVirtualFunc70();       // +0x168
    virtual float CfObject_UnkVirtualFunc71();      // +0x16C
    virtual void CfObject_UnkVirtualFunc72();       // +0x170
    virtual float CfObject_UnkVirtualFunc73();      // +0x174

    u8 pad04[0x68 - 0x04];
    u32 field_0x68;                    // shared flag word
    u32 field_0x6C;
};

class CfObjectPoint : public CfObject {
public:
    virtual ~CfObjectPoint();          // +0x54 override; defined in CfObjectPoint.cpp

    void notifyChildUpdate();              // non-virtual helper (external)

    virtual void loadPointData();      // +0x48
    virtual void updatePointState();      // +0x60
    virtual void releasePointLink();      // +0x68
    virtual void setChildPoint();      // +0x70
    virtual void func_80047814(const void* pos); // +0x9C: copies pos[0..2] to this+0x3C
    virtual void setPointPosition();      // +0xA0
    virtual void setPointEnabled(u32 flag); // +0x158
    virtual int isCollEnabled();       // +0x160
};

class __declspec(novtable) CfObjectColl : public CfObjectPoint {
public:
    ~CfObjectColl();                   // +0x54 most-derived destructor
    int resetCollTimer();               // +0x58
    void enableCollision();              // +0x5C
    void refreshCollLink();              // +0x6C
    void copyCollPosition();              // +0xB8

    u8 pad70[0x94 - 0x70];
    u32 field_0x94;
    u32 field_0x98;
    u32 field_0x9C;                    // packed resource id
    u32 field_0xA0;                    // vec A word 0
    u32 field_0xA4;                    // vec A word 1
    u32 field_0xA8;                    // vec A word 2
    u32 field_0xAC;                    // vec B word 0
    u32 field_0xB0;                    // vec B word 1
    u32 field_0xB4;                    // vec B word 2
    float field_0xB8;
    u32 field_0xBC;                    // vec A' word 0
    u32 field_0xC0;                    // vec A' word 1
    u32 field_0xC4;                    // vec A' word 2
    u32 field_0xC8;                    // vec B' word 0
    u32 field_0xCC;                    // vec B' word 1
    u32 field_0xD0;                    // vec B' word 2
    float field_0xD4;
    float field_0xD8;
    float field_0xDC;
    float field_0xE0;
    float field_0xE4;
    float field_0xE8;
    float field_0xEC;
    Mtx field_0xF0;
    ml::FixStr<16> fieldName120;       // formatted display name (0x14-byte entry)
    u8 pad134[0x148 - 0x134];
    float field_0x148;
    float field_0x14C;
    float field_0x150;
    float field_0x154;
    u16 field_0x158;
    u16 field_0x15A;
    u16 field_0x15C;
    u16 field_0x15E;
    u16 field_0x160;
};

// 12-byte packed vector block copied around by the copy helpers
struct CollVec {
    u32 w0;
    u32 w4;
    u32 w8;
};

// Stateless collision-implementation interface.
// (CfCollCapsuleImpl / CfCollCylinderImpl / CfCollAABBImpl / CfCollSphereImpl /
// CfCollCircleImpl) carry no data members: each instance is just its vptr
// storage, and the registry slots below double as those instances.
class CfCollImpl {
public:
    virtual void CfCollImpl_UnkVirtualFunc1(); // +0x08
    virtual int load(CfObjectColl* self, CfObject* obj, ml::CVec3* out,
                     float angle);             // +0x0C
};

} // namespace cf

// Static coll-impl instance registry (lazily initialized singletons). Each
// lbl_eu_80663Exx word is the instance itself; its content becomes the vptr
// (one of the five leaf vtables below) on first use.
extern s8 lbl_eu_80663EA0;
extern s8 lbl_eu_80663EA8;
extern s8 lbl_eu_80663EB0;
extern s8 lbl_eu_80663EB8;
extern s8 lbl_eu_80663EC0;
extern cf::CfCollImpl lbl_eu_80663EA4;
extern cf::CfCollImpl lbl_eu_80663EAC;
extern cf::CfCollImpl lbl_eu_80663EB4;
extern cf::CfCollImpl lbl_eu_80663EBC;
extern cf::CfCollImpl lbl_eu_80663EC4;
extern cf::CfCollImpl* lbl_eu_805285E8[6];
// Leaf vtables: CfCollCircleImpl / SphereImpl / AABBImpl / CylinderImpl /
// CapsuleImpl (split1.s lbl_eu_80528820..87A0, RTTI typestrs confirmed).
extern u8 lbl_eu_80528820[];
extern u8 lbl_eu_80528800[];
extern u8 lbl_eu_805287E0[];
extern u8 lbl_eu_805287C0[];
extern u8 lbl_eu_805287A0[];
