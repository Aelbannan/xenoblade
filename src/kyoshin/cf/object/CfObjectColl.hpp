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

// Presentation/event bitfield owned by CUICfManager (.sbss).

// Static coll-impl instance registry (lazily initialized singletons).
extern s8 lbl_eu_80663EA0;
extern s8 lbl_eu_80663EA8;
extern s8 lbl_eu_80663EB0;
extern s8 lbl_eu_80663EB8;
extern s8 lbl_eu_80663EC0;
extern u8* lbl_eu_80663EA4;
extern u8* lbl_eu_80663EAC;
extern u8* lbl_eu_80663EB4;
extern u8* lbl_eu_80663EBC;
extern u8* lbl_eu_80663EC4;
extern u8* lbl_eu_805285E8[5];
extern u8 lbl_eu_80528820[];
extern u8 lbl_eu_80528800[];
extern u8 lbl_eu_805287E0[];
extern u8 lbl_eu_805287C0[];
extern u8 lbl_eu_805287A0[];

// Shared distance helper (defined in code_800A3B24.cpp); retail symbol is flat.
extern "C" float func_800A4B5C(const ml::CVec3& a, const ml::CVec3& b, const ml::CVec3& c);

namespace cf {

class CfObjectPoint {
public:
    virtual ~CfObjectPoint();
    void func_800C1638();
    int func_800AC604();

    u8 pad04[0x68 - 0x4];
    u32 field_0x68;
};

class CfObjectColl : public CfObjectPoint {
public:
    virtual ~CfObjectColl();
    int func_800AB3EC();
    void func_800AB498();
    void func_800AB57C();
    void func_800AB798();
    void func_800AB7A8();

    u8 pad6C[0x94 - 0x6C];
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

namespace ml { struct CVec3; }


class CfObject {
public:
    void CfObject_UnkVirtualFunc67();
    void CfObject_UnkVirtualFunc65();
    int CfObject_UnkVirtualFunc11();
    void CfObject_UnkVirtualFunc10();

    // note: non-polymorphic helper view; offsets are absolute from 0
    u8 pad04[0x68];
    u32 field_0x68; // shared flag word
    u8 pad6C[0x6C - 0x68];
    u32 field_0x6C;
    void CfObject_UnkVirtualFunc73();
    void CfObject_UnkVirtualFunc72();
    void CfObject_UnkVirtualFunc71();
    void CfObject_UnkVirtualFunc70(float value);
    void CfObject_UnkVirtualFunc69();
    void CfObject_UnkVirtualFunc62();
    void CfObject_UnkVirtualFunc61();
    void CfObject_UnkVirtualFunc60();
    void CfObject_UnkVirtualFunc59();
    void CfObject_UnkVirtualFunc57();
    void CfObject_UnkVirtualFunc51();
    void CfObject_UnkVirtualFunc45();
    void CfObject_UnkVirtualFunc42();
    void CfObject_UnkVirtualFunc43();
    void CfObject_UnkVirtualFunc44();
    void CfObject_UnkVirtualFunc39();
    void CfObject_UnkVirtualFunc40();
    void CfObject_UnkVirtualFunc41();
    void CfObject_UnkVirtualFunc38();
    void CfObject_UnkVirtualFunc37();
    void CfObject_UnkVirtualFunc36();
    void CfObject_UnkVirtualFunc35();
    void CfObject_UnkVirtualFunc28();
    void CfObject_UnkVirtualFunc24();
    void CfObject_UnkVirtualFunc21();
    void CfObject_UnkVirtualFunc18();
    void CfObject_UnkVirtualFunc17();
    void* CfObject_UnkVirtualFunc16(float value);
    void CfObject_UnkVirtualFunc15();
    void* CfObject_UnkVirtualFunc14(float value);
    void CfObject_UnkVirtualFunc13();
    void CfObject_UnkVirtualFunc12();
    void CfObject_UnkVirtualFunc5();
};
} // namespace cf
