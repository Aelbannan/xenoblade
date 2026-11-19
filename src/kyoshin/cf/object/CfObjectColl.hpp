#pragma once

#include <types.h>
#include <revolution/MTX.h>
#include "monolib/util/FixStr.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)
#include "kyoshin/cf/object/CfObject.hpp"

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
extern "C" float VecMath_SegmentParam(const ml::CVec3& a, const ml::CVec3& b, const ml::CVec3& c);

namespace cf {

// CfObject / CObjectParam / CObjectState virtuals and fields are declared once
// on the real classes in kyoshin/cf/object/CfObject.hpp, in retail vtable order
// (lbl_eu_80528600). This header only adds the Point/Coll family on top.

class CfObjectPoint : public CfObject {
public:
    virtual ~CfObjectPoint();          // +0x54 override; defined in CfObjectPoint.cpp

    void notifyChildUpdate(const ml::CVec3* pos); // non-virtual helper (external)
    int isCollEnabled();                          // +0x160 retail spelling
};

class __declspec(novtable) CfObjectColl : public CfObjectPoint {
public:
    ~CfObjectColl();                   // +0x54 most-derived destructor
    int resetCollTimer();               // +0x58
    void enableCollision();              // +0x5C
    void refreshCollLink(const ml::CVec3* pos); // +0x6C
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
