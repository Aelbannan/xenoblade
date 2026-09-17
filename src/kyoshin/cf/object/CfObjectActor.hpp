#pragma once

#include <types.h>
#include "kyoshin/cf/object/CfObjectMove.hpp"
#include "kyoshin/cf/object/CAIAction.hpp"
#include "kyoshin/cf/object/CActorParam.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

// Retail float constants referenced by CfObjectActor functions (SDA2 access).
extern const float lbl_eu_80667738;   // CActorParam_addHp / CfObjectActor_takeDamageValue threshold

extern const float lbl_eu_8066776C;   // CfObjectActor_readActionScale fallback value

extern const float lbl_eu_8066773C;   // CActorParam_UnkVirtualFunc179 CfObjectMove vf70 arg

extern const float lbl_eu_80667740;   // CActorParam_setHp clamp threshold


extern u32 lbl_eu_80663E28;     // mode bitfield (.sbss; bit 0x800 gates the 33 path)

extern const double lbl_eu_80667748;  // 2^52 + 2^31 magic (CfObjectActor_getAdjustedFacing s16 -> float)

extern const double lbl_eu_80667750;  // +0.5 rounding add (CActorParam_applyDamage gauge)

extern const double lbl_eu_80667758;  // -0.5 rounding add (CActorParam_applyDamage gauge)

extern const float lbl_eu_80667760;         // CActorParam_applyDamage 0xC3 ratio divisor

extern const float lbl_eu_80667764;         // CActorParam_applyDamage 0xFC/0x100 gauge scale

extern const float lbl_eu_80667768;         // CActorParam_applyDamage 0x100 decay add

extern const float lbl_eu_80667770;         // code-orphaned tail float (touch-anchored)

extern const float lbl_eu_80667774;         // code-orphaned tail float (touch-anchored)

extern const float lbl_eu_80667778;         // code-orphaned tail float (touch-anchored)

// ptmf constant (3 words: this-offset / fn / this) compared against
// CBattleManager+0x28354 by CActorParam_applyDamage.
extern const u32 lbl_eu_80531090[3];

// vtable-group symbol the CfObjectActor constructor copies its four vtable
// pointers from (retail label; the implicit __vt symbol sits -32 bytes away).
extern u8 lbl_eu_8053109C[];

// Battle-side initializers called by the CfObjectActor constructor.
// NOTE: func_80174B4C is declared here in its LOCAL 5-arg form inside
// CfObjectActor.cpp (the shared headers carry the common 2-arg form;
// the two extern "C" signatures cannot coexist - error 10197).
extern "C" void func_8014AA10(void* obj, unsigned int value);

// C-linkage imports (retail symbols are unmangled).
extern "C" int getParamSelS16(u8* obj, int arg);   // CActParamData short-value helper

// Retail symbol is Fv but the real ABI passes (self, arg, f1, f2, f3);
// CfObjectActor::CActorParam_addHate tail-calls this base entry
// (defined in CActorParam.cpp).
extern "C" void CActorParam_addHate__Q22cf11CActorParamFv(cf::CActorParam* self, void* arg, int id, f32 a, f32 b, f32 c);

// Retail symbols are Fv; the real ABI passes (self, arg). The CfObjectActor
// overrides of 179/180 forward to these base entries (defined in CActorParam.cpp).
extern "C" void CActorParam_UnkVirtualFunc179__Q22cf11CActorParamFv(cf::CActorParam* self, void* arg);
extern "C" void CActorParam_UnkVirtualFunc180__Q22cf11CActorParamFv(cf::CActorParam* self, void* arg);

// C-ABI imports (retail symbols unmangled).
extern "C" void func_801A891C(void* obj, int arg);   // vision slot release

extern "C" int func_8027990C(void* battleSub, void* actor);  // battle sub-object query

extern "C" void func_8010975C(u8);

extern "C" void func_80109770(u8);

extern "C" void func_80109734(void* obj, u32 value);

extern "C" void func_80109784(void* obj, u32 a, int b);

extern "C" void func_8010989C(u8);

extern "C" void func_80109888(u8);

extern "C" void func_80109874(u8);

extern "C" bool isGlobalCamFlagSet__Fi(int mask);

extern "C" void* getInstance__Q22cf13CfGameManagerFv(void);

extern "C" float func_800D81A8(void* obj, void* target, void* source);

extern "C" void func_8018C820(void* obj, int value);   // party-gauge add

extern "C" void CBattleMan_ClearEventNotify(void* battleMgr, void* actor, int flag);

extern "C" void func_80277A7C(void* battleSub, void* actor, float value);

extern "C" int __ptmf_cmpr(void* a, void* b);

namespace cf {
    //min size: 0x45BC
    class CfObjectActor : public CActorParam, public CAIAction, public CfObjectMove {
    public:
        CfObjectActor();
        //vtable 4 (CfObjectActor)
        virtual ~CfObjectActor();                  //0x5A0
        virtual int CfObjectActor_hasActorScale();  //0x5A4: constant 1 (Ene/Actor scale present)
        // Legacy Unk spelling (non-virtual inline).
        int CfObjectActor_UnkVirtualFunc2() { return CfObjectActor_hasActorScale(); }
        virtual void CfObjectActor_clearStatusPair();  //0x5A8
        virtual void CfObjectActor_flushStatusPair();  //0x5AC
        virtual void CfObjectActor_takeDamageValue(float a, u32 b);  //0x5B0
        virtual float CfObjectActor_readFacingAngle();  //0x5B4
        virtual float CfObjectActor_getAdjustedFacing();  //0x5B8 (retail returns a float in f1)
        virtual float CfObjectActor_readActionScale();  //0x5BC (retail returns a float in f1)
        virtual void* CfObjectActor_sharesMoveFlags(void* arg);  //0x5C0
        virtual void CfObjectActor_pushRefreshValue(float value); //0x5C4
        virtual void CfObjectActor_storeActionSrcId(void* arg); //0x5C8
        virtual u32 CfObjectActor_getActionSrcId(); //0x5CC: read +0x45BC action-src id
        virtual void CfObjectActor_clearActionSrcId(); //0x5D0: clear +0x45BC to -1
        // Legacy Unk spellings (non-virtual inline): same-arity aliases so
        // any leftover callers keep compiling (Wave-61).
        u32 CfObjectActor_UnkVirtualFunc12() { return CfObjectActor_getActionSrcId(); }
        void CfObjectActor_UnkVirtualFunc13() { CfObjectActor_clearActionSrcId(); }

        //0x0: vtable 1
        //0x0-3380: CActorParam
        //0x3380: vtable 2
        //0x3380-3e9c: CAIAction
        //0x3e9c: vtable 3
        //0x3e9c-45b4: CfObjectMove
        u8 unk45B4[0x8];
    const char* CActorParam_getActorName();
    void CActorParam_addHate(void* arg, float x, float y, float z);
    void CActorParam_UnkVirtualFunc179(void* arg);
    void CActorParam_UnkVirtualFunc180(void* arg);
    void CActorParam_setHp(float val);
    void CActorParam_applyDamage(float value, int a, int b, int c);
    void CActorParam_addHp(float val);
    void CActorParam_addGauge(int delta);
    void CActorParam_addSecondGauge(int delta);
    void CActorParam_resetArtsStatus(void* arts);
    virtual void CActorParam_setScale(float val);
    virtual float CActorParam_getScale();
    void destroyActorParam();
    // Caller aliases kept for existing call sites; both forward to the real
    // setScale/getScale overrides (former UnkVirtualFunc21/23).
    void CActorParam_propagateScale(float val) { CActorParam_setScale(val); }
    float CActorParam_getSubScale() { return CActorParam_getScale(); }
    };

    // Status-entry view for the func_80149154 results used by
    // CActorParam_applyDamage: u32 marker at +0x8, s32 value at +0x10,
    // s16 gauge at +0x14.
    struct CfStatusEntry {
        u8 _pad[0x8];
        u32 field_0x8;   // 0x8
        u8 _pad0C[0x4];  // 0xC
        s32 field_0x10;  // 0x10
        s16 field_0x14;  // 0x14
        u8 _pad16[2];    // 0x16-0x17
        s16 field_0x1A;  // 0x1A (E08E8 s1A)
    };

    // 12-byte ptmf copy built by CActorParam_applyDamage from
    // lbl_eu_80531090 and compared against CBattleManager+0x28354.
    struct CfPtmf3 {
        u32 field_0;  // 0x0
        u32 field_4;  // 0x4
        u32 field_8;  // 0x8
    };

    // Object holding a float at +0x7C (target of the CAIAction trailer word
    // read by CfObjectActor_readActionScale).
    struct CfFloat7C {
        u8 _pad[0x7C];
        float field_0x7C;  // 0x7C
    };

    // View of CfObjectActor's word at absolute offset 0x3E74 (CAIAction
    // trailer-area word, used as a pointer by CfObjectActor_readActionScale).
    struct CfActorField3E74 {
        u8 _pad[0x3E74];
        u32 field_0x3E74;  // 0x3E74
    };

    // View of CfObjectActor's word at absolute offset 0x3F10 (the
    // CfObjectModel.field_0x74 slot inside the CfObjectMove subobject) - used
    // by CActorParam_addHate to compare two actors.
    struct CfActorField3F10 {
        u8 _pad[0x3F10];
        u32 field_0x3F10;  // 0x3F10
    };

    // View of CfObjectActor's flag word at absolute offset 0x3F00
    // (CfObjectMove+0x64) - compared between two actors by
    // CfObjectActor_sharesMoveFlags (bits 0x2 / 0x4 select the dispatch).
    struct CfActorField3F00 {
        u8 _pad[0x3F00];
        u32 field_0x3F00;  // 0x3F00
    };

    // Same flag word viewed relative to the CfObjectMove subobject pointer
    // (findObjectById returns one; CfObjectActor_pushRefreshValue reads it).
    struct CfMoveFlags64 {
        u8 _pad[0x64];
        u32 field_0x64;  // 0x64
    };

    // Absolute-offset views for CfObjectMove-subobject fields. The C++ base
    // layout in the headers is 8 bytes off retail (CfObjectMove lands at
    // +0x3EA4 instead of +0x3E9C), so subobject fields are reached through
    // raw views instead of static_cast upcasts.
    struct CfActorField3EE8 {
        u8 _pad[0x3EE8];
        float field_0x3EE8;  // 0x3EE8 (CfObjectMove+0x4C base height)
    };
    struct CfActorField3F60 {
        u8 _pad[0x3F60];
        u8* field_0x3F60;  // 0x3F60 (CfObjectMove+0xC4 move target)
    };
    struct CfActorField45B8 {
        u8 _pad[0x45B8];
        u8* field_0x45B8;  // 0x45B8 (action-source handle id)
        s32 field_0x45BC;  // 0x45BC (ctor seeds -1)
    };

    // vtable-pointer slots written explicitly by the CfObjectActor
    // constructor from the lbl_eu_8053109C group via direct *(void**)
    // stores in the .cpp (offsets 0x0 / 0x8 / 0x3380 / 0x3E9C).

    // View of CfObjectActor's pointer at absolute offset 0x04 (the
    // CActorState sub-object pointer): its vtable slot +0x30 returns a u32*
    // whose first word is the actor id queried by func_80174C98.
    struct CfActorField04 {
        u8 _pad[0x4];
        u8* field_0x04;  // 0x04
    };

    // u16/u32/s16 -> float conversion helper (CfMapEffectManager.hpp
    // convention): the retail builds the 0x43300000-prefixed bit pattern in
    // memory and subtracts the shared sdata2 magic double, so the pool reloc
    // matches the retail constant instead of an MWCC-synthesised @N entry.
    union CfActorF64Conv {
        u32 w[2];
        double d;
    };

    // Argument passed through the CActorParam_UnkVirtualFunc179/180 slots
    // (retail ABI r4): u16 dispatch id at +0xC.
    struct CfActorParamArg {
        u8 _pad[0xC];
        u16 field_0xC;  // 0xC
    };

    // Absolute-offset view of CfObjectActor's CActorParam-region gauge/flag
    // fields (the C++ class layout in the headers is not trusted for these
    // functions; raw views pin the retail offsets).
    struct CfActorParamFields {
        u8 _pad[0x160C];
        s16 field_0x160C;  // 0x160C (arts/tension gauge current)
        s16 field_0x160E;  // 0x160E (gauge max)
        u8 _pad1610[0x1614 - 0x1610];
        s16 field_0x1614;  // 0x1614 (second gauge current)
        s16 field_0x1616;  // 0x1616 (second gauge max)
        u8 _pad1618[0x1634 - 0x1618];
        u32 field_0x1634;  // 0x1634
        u32 field_0x1638;  // 0x1638
        u32 field_0x163C;  // 0x163C
        u32 field_0x1640;  // 0x1640
        u32 field_0x1644;  // 0x1644
        u8 _pad1648[0x17E8 - 0x1648];
        f32 field_0x17E8;  // 0x17E8
        u8 _pad17EC[0x17F4 - 0x17EC];
        f32 field_0x17F4;  // 0x17F4 (gauge max)
        u8 _pad17F8[0x3374 - 0x17F8];
        u32 field_0x3374;  // 0x3374 (flags, bit 0x40000 gates the battle-state path)
    };

    // Stack buffer built by CActorParam_UnkVirtualFunc179 and passed to the
    // CBattleState subobject vtable slot +0x18 (exactly 0x34 bytes, matching
    // the memset size).
    struct CfActor179Buf {
        u32 field_0x00;       // 0x00
        u32 field_0x04;       // 0x04
        u8  field_0x08[4];    // 0x08
        u16 field_0x0C;       // 0x0C
        u32 field_0x10;       // 0x10
        u16 field_0x14;       // 0x14
        u16 field_0x16;       // 0x16
        u16 field_0x18;       // 0x18
        u8  field_0x1C[4];    // 0x1C
        f32 field_0x20;       // 0x20
        u8  field_0x24[0xC];  // 0x24
        u32 field_0x30;       // 0x30
    };

    // CfObjectActor's pointer at absolute offset 0x3ED4 (CfObjectMove+0x38
    // sub-object pointer; vtable slots +0x88/+0x8C take (self, arg) -
    // dispatched as cf::CfObjectSub38::m88/m8C (see CfObject.hpp).
    struct CfActorField3ED4 {
        u8 _pad[0x3ED4];
        u8* field_0x3ED4;  // 0x3ED4
    };
}
