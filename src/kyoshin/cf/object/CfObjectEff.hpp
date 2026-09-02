#pragma once

#include <types.h>
#include "kyoshin/cf/object/CfObject.hpp"

// Retail data symbols (linker-provided)
// Retail data symbols (linker-provided)
extern const float lbl_eu_8066A210;  // final position scale (pi/2)
extern const float lbl_eu_8066A20C;  // effect position scale
extern const float lbl_eu_80666960;  // X component baked into the move vector (moveEffOfs___)
extern const float lbl_eu_80666964;  // scale passed to the vtable 0xB4 slot (moveEffOfs___)
extern u8 lbl_eu_80528870[];         // CfObjectEff vtable (retail data, 0x1A8 bytes)
extern u8 lbl_eu_805294E0[];         // CfObject vtable (retail .data at 0x805294E0) -
                                     // stored by the inlined base ctor
extern u8 __vt__Q22cf8CfObject[];    // compiler-generated name for the same CfObject
                                     // vtable; the devirtualized slot-0x5C call in the
                                     // ctor references it
extern const float lbl_eu_80666958;  // default position/scale fill (CfObjectEff ctor)
extern const float lbl_eu_8066695C;  // default mFloat60 fill (CfObjectEff ctor)

// C-ABI imports used by createEffect_. These retail symbols are unmangled
// C names, so they are declared extern "C" (as CREvtEffect.hpp does) - a
// plain C++ declaration would make MWCC append a mangling suffix to the
// reloc. isGlobalCamFlagSet__Fi is declared in kyoshin/cf/code_800F42AC.hpp
// (extern "C") and included by the cpp.
extern "C" {
    void* func_800630C8();
    void* func_80063038();
    void* func_80066E7C(void* self, u32 id);
    void* func_80066CF8(void* self);
    void* func_800BB340(void* obj);
    void* func_804E3CFC(void* effect);
    int CfRes_getD80Flag();
}

// RTTI typeinfos + C-ABI cast helper for notifyDetach_ (same pair as
// CTaskGameEff/CBattleManager: cast the chain object from the type at
// lbl_eu_80661970 down to its base at lbl_eu_806618F0).
extern "C" const void* lbl_eu_80661970;
extern "C" const void* lbl_eu_806618F0;
extern "C" void* __dynamic_cast(void* obj, long offset, const void* src_type,
                                const void* dst_type, void* src2dst);
// C-linkage validity check used by updateEffect_/teardownEff__ (returns the
// object when it is still live, 0 when it has been released).
extern "C" void* func_800B8920(void* obj);

namespace cf {
class CfObjectEff;
}

// Pointer-to-member-function dispatch table used by updateEffect_: indexed by
// the u16 counter at +0x8E, each entry is a 12-byte ptmf (retail .data at
// lbl_eu_80528858, 0x18 bytes before the CfObjectEff vtable). Declared at
// global scope so MWCC keeps the C symbol name unmangled.
extern void (cf::CfObjectEff::*const lbl_eu_80528858[])();

namespace cf {

// Minimal view of the argument passed to teardownEff__: only the +0x14 word
// (validity flag) is read.
struct CfObjectEffArg14View {
    u8 _pad00[0x14];
    u32 field_14;  // 0x14
};

// Raw u16 view of CfObjectEffChild's flag word at +0x0: updateEffect_ reads
// and writes the whole halfword (bit 14), not individual bitfields.
struct CfObjectEffChildFlagsView {
    u16 field_00;  // 0x00
};

// Minimal 3-component float vector used for the position/rotation copies.
struct CfObjectEffVec3 {
    float x;  // 0x00
    float y;  // 0x04
    float z;  // 0x08
};

// 2-word view for paired word copies (func_800ACCE4 copies x/y as one
// 8-byte struct assignment, producing lwz/lwz/stw/stw).
struct CfObjectEffU32Vec2 {
    u32 x;  // 0x00
    u32 y;  // 0x04
};

// 3-word vector used for raw word copies (setEffPosVec_/setEffRotVec_): the
// destination float fields are copied as words, so MWCC emits lwz/stw.
struct CfObjectEffU32Vec3 {
    u32 x;  // 0x00
    u32 y;  // 0x04
    u32 z;  // 0x08
};

// Slot +0xB4 is CfObject_UnkVirtualFunc25 - now fixed in CfObject.hpp
// to (const float*, float) so moveEffOfs___'s call is this->method(...).

// View of the CfObjectEff tail used by setEffRotVec_: the 0x48-0x50 region
// (spanning mField4C) is one 12-byte vector copied as a struct.
struct CfObjectEffVec48View {
    u8 _pad00[0x48];
    CfObjectEffU32Vec3 vec48;  // 0x48
};

struct CfObjectEffChild {
    u16 unk0HighTop : 1; // bit 15
    u16 flag4000 : 1;    // bit 14 (0x4000) - synced with slot-0x160 status
    u16 unk0High : 3;    // bits 13-11
    u16 flag400 : 1;     // bit 10 (0x400) - assigned from a flag in setChildFlag_
    u16 unk0Low : 10;    // bits 9-0
    u8 _pad02[0x14 - 0x02];
    u8* field_14;        // 0x14 - partner object (bindPartnerO_)
    u8* field_18;        // 0x18 - resolved partner object (setTargetObj_)
    u32 field_1C[3];     // 0x1C-0x27 - word-copied from a position vector (setEffPosVec_)
    float field_28;      // 0x28
    float unk2C;         // 0x2C
    float field_30;      // 0x30
    u32 unk34[3];
    u32 unk40[4];
    u8 _pad50[0x59 - 0x50];
    u8 unk59;
    u8 _pad5A[0x5C - 0x5A];
    int unk5C;
};

// View of the CfObjectEffChild 0x34-0x3C region used by setEffScale__: the
// same float bits are word-copied into all three slots.
struct CfObjectEffChild34View {
    u8 _pad00[0x34];
    u32 field_34[3];  // 0x34-0x3C
};

// Minimal view of the CfObject-family sub-object checked by getEffOwner__:
// the owner object starts 0x3E9C bytes before it, and only the +0x64 flag
// word (CfObject::unk64) is read (bit 4 = alive/enabled).
struct CfObjectEffSubView {
    u8 _pad64[0x64];
    u32 field_0x64;
};

// Byte-exact view of the CfObjectEff base region as the retail ctor chain
// clears it: 0x10 is a byte store, 0x30/0x34 words, the 0x3C-0x60 float
// block, and the 0x68-0x8E tail words/bytes. 0x64 is deliberately skipped.
struct CfObjectEffCtorView {
    u8 _pad00[0x04];
    u32 field_04;      // 0x04
    u32 field_08;      // 0x08
    u32 field_0C;      // 0x0C
    u8 field_10;       // 0x10 (byte store)
    u8 _pad11[0x30 - 0x11];
    u32 field_30;      // 0x30
    u32 field_34;      // 0x34
    u32 field_38;      // 0x38 (mSubObj38)
    float field_3C;    // 0x3C
    float field_40;    // 0x40
    float field_44;    // 0x44
    float field_48;    // 0x48
    float field_4C;    // 0x4C
    float field_50;    // 0x50
    float field_54;    // 0x54
    float field_58;    // 0x58
    float field_5C;    // 0x5C
    float field_60;    // 0x60 (mFloat60)
    u32 field_64;      // 0x64 (not cleared by the ctor)
    u32 field_68;      // 0x68 (mFlags68)
    u32 field_6C;      // 0x6C
    u32 field_70;      // 0x70
    u32 field_74;      // 0x74
    u8 field_78;       // 0x78 (byte store)
    u8 _pad79[0x88 - 0x79];
    u32 field_88;      // 0x88
    u16 field_8C;      // 0x8C
    u16 field_8E;      // 0x8E
};

// Slot +0xBC (CfObject_UnkVirtualFunc27) is base-name: void* arg matches retail.
// Slots +0x3C/+0x9C/+0xB4/+0xDC now take retail arity on CObjectParam/CfObject
// (const char* / const ml::CVec3* / (ml::CVec3*, float) / float), so Owner
// and Move pads were deleted; call sites use this->method(...).

// Minimal view of the effect-source objects (BDAT/CfRes entries) used by
// createEffect_: the +0xC word gates the 0x10/0x11 types, and +0x2C is a
// sub-object whose vtable slot 0x40 validates the entry.
struct CfObjectEffSourceView {
    u8 _pad00[0xC];
    u32 field_0C;      // 0x0C
    u8 _pad10[0x2C - 0x10];
    void* field_2C;    // 0x2C
};

// Foreign helpers for mFieldB0 (slot 0xC) and source validation (slot 0x40).
// These are tiny interfaces on the owning objects, not pads named after the caller.
struct CfB0Helper {
    virtual void f08();
    virtual void func0C(CfObjectEff* owner); // slot 0xC
};
struct CfSourceHelper {
    virtual void f08();
    virtual void f0C();
    virtual void f10();
    virtual void f14();
    virtual void f18();
    virtual void f1C();
    virtual void f20();
    virtual void f24();
    virtual void f28();
    virtual void f2C();
    virtual void f30();
    virtual void f34();
    virtual void f38();
    virtual void f3C();
    virtual u32 validate(void* owner); // slot 0x40
};

class __declspec(novtable) CfObjectEff : public CfObject {
public:
    u32 mField70;                // 0x70 - packed type/id (top 5 bits = type, createEffect_)
    u8 _pad74[0x8C - 0x74];
    u16 mCount8C;                // 0x8C - effect count (createEffect_ passes count-1)
    u16 mCount8E;                // 0x8E - ptmf-table dispatch counter (updateEffect_)
    u8* mSubObj90;               // 0x90 - secondary vtable (CfObjectEff vtable + 0x178)
    CfObjectEffChild* mChildEff;  // 0x94
    u8* mField98;            // 0x98 - object (cleared with mField9C in detachBoundO_)
    u8* mField9C;            // 0x9C - object (bindPartnerO_)
    u8* mFieldA0;            // 0xA0 - target (setTargetObj_)
    u16 mFlagsA4;
    u16 mCountA6;
    u8* mFieldA8;            // 0xA8 - source object (createEffect_)
    u8* mFieldAC;            // 0xAC - partner (bindPartnerO_)
    u8* mFieldB0;            // 0xB0 - object cleared by teardownEff__ (slot-0xC dispatch)
    u8 mFieldB4;             // 0xB4 - effect bound flag (createEffect_)
    u8 _padB5[0xB8 - 0xB5];
    u8* mFieldB8;            // 0xB8 - detached target (detachTrgPrt_)
    u8* mFieldBC;            // 0xBC - effect manager override (createEffect_)

    CfObjectEff();
    bool initEffState_();
    void cleanupEffct_();
    void notifySubA08_();
    void setSubObject_();
    void setEffEnable_();
    void setChildF50__();
    void setEffPosVec_();
    void callVirt19___();
    void func_800ACCE4();
    void callVirt25___();
    void moveEffOfs___();
    void setEffRotVec_();
    u8* getEffPosPtr_();
    void setEffYPos___();
    void scaleVirt29__();
    float getScaledVal_();
    void setEffScale__();
    void setEffLockFg_() const;
    void updateEffect_();
    int checkStsFlag_();
    virtual ~CfObjectEff();
    // Retail vtable lbl_eu_80528870 beyond CfObject's last slot (0x174) is
    // a secondary group at +0x178: header {RTTI, -0x90}, thunks
    // thunkDtor90__ (-0x90 -> dtor) and thunkTear90__ (-0x90 ->
    // teardownEff__) at +0x180/+0x184, then CfObjectEff's own appended
    // virtuals at +0x188..+0x1A4. The dummies at 0x178-0x184 cover the header/
    // thunk words; the appended reals are the MI analysis from the probe
    // (header+2 thunks + 8 appended = retail 0x188-0x1A4 exactly).
    virtual void dummy178();                      // +0x178 dummy (RTTI word)
    virtual void dummy17C();                      // +0x17C dummy (-0x90 word)
    virtual void dummy180();                      // +0x180 dummy (dtor-thunk slot)
    virtual void dummy184();                      // +0x184 dummy (AD68C-thunk slot)
    virtual u32 hasChildEffs_();               // +0x188 - active-sub-object check
    virtual void teardownEff__(u8* arg);       // +0x18C - teardown (Sub iface op)
    virtual void detachChildEf();              // +0x190 - detach child effect
    virtual void setEffLockFg_(bool flag);     // +0x194
    virtual bool detachPrtnrA_(u8* obj);       // +0x198 - detach partner
    virtual bool detachBoundO_(u8* arg);       // +0x19C - detach bound object
    virtual void setChildFlag_(int flag);      // +0x1A0
    virtual bool testFlagA4___(u16 flags);     // +0x1A4 - flag test
    void callVirt32___();
    void callVirtC4___();
    void getEffYPos___();
    void getScaleComp_() const;
    void thunkTear90__();
    void thunkDtor90__();
};

} // namespace cf
