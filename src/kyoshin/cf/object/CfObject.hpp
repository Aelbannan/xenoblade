#pragma once

#include <types.h>
#include "kyoshin/cf/object/CObjectParam.hpp"
#include "monolib/math/CVec3.hpp"

namespace cf {
    class UnkClass_80082D90;
    struct CfObjectSub54 {
        u8 field_0x0[0xC];
    };

    //min size: 0x70
    class CfObject : public CObjectParam {
    public:
        //vtable 1 (CfObject)
        virtual ~CfObject();                      //0x54
        virtual void CfObject_UnkVirtualFunc2() = 0;  //0x58
        virtual void CfObject_UnkVirtualFunc3(); //0x5C
        virtual void CfObject_UnkVirtualFunc4() = 0;  //0x60
        virtual void CfObject_UnkVirtualFunc5();      //0x64
        virtual void CfObject_UnkVirtualFunc6();      //0x68
        virtual void CfObject_UnkVirtualFunc7() = 0;  //0x6C
        virtual void CfObject_UnkVirtualFunc8() = 0;  //0x70
        virtual bool CfObject_UnkVirtualFunc9();      //0x74
        virtual void CfObject_UnkVirtualFunc10();     //0x78
        virtual void CfObject_UnkVirtualFunc11();     //0x7C
        virtual void CfObject_UnkVirtualFunc12();     //0x80
        virtual void CfObject_UnkVirtualFunc13();     //0x84
        virtual void* CfObject_UnkVirtualFunc14(float value);     //0x88
        virtual void CfObject_UnkVirtualFunc15();     //0x8C
        virtual void* CfObject_UnkVirtualFunc16(float value);     //0x90
        virtual void CfObject_UnkVirtualFunc17();     //0x94
        virtual void CfObject_UnkVirtualFunc18();     //0x98
        // Retail bodies read r4 as a position vector (setEffPosVec_ /
        // Model/Move/ocUnit UVF19). Fv linker names are uneducated.
        virtual void CfObject_UnkVirtualFunc19(const ml::CVec3* vec); //0x9C
        virtual void CfObject_UnkVirtualFunc20(float a, float b);     //0xA0
        virtual void CfObject_UnkVirtualFunc21();     //0xA4
        // Same ABI as UVF19 (copy vec into +0x3C). Eff's override is a pure
        // forward to UVF19 (callVirt19); Model/Move override with extra work.
        virtual void CfObject_UnkVirtualFunc22(const ml::CVec3* vec); //0xA8
        virtual ml::CVec3* CfObject_UnkVirtualFunc23();      //0xAC
        virtual void CfObject_UnkVirtualFunc24();     //0xB0
        // Retail ground-snap / moveEffOfs: (pos, scale) in r4/f1.
        virtual void CfObject_UnkVirtualFunc25(ml::CVec3* pos, float scale); //0xB4
        // Forwards to UVF19 (base) or UVF25 (Eff callVirt25). First arg is a
        // position vector (u32 casts at call sites are pointers).
        virtual void CfObject_UnkVirtualFunc26(const ml::CVec3* vec, float amount); //0xB8
        virtual void CfObject_UnkVirtualFunc27(void* src);     //0xBC
        virtual void CfObject_UnkVirtualFunc28();     //0xC0
        virtual void CfObject_UnkVirtualFunc29(float value);     //0xC4
        virtual void CfObject_UnkVirtualFunc30(float value);     //0xC8
        virtual float CfObject_UnkVirtualFunc31();     //0xCC
        virtual void CfObject_UnkVirtualFunc32();     //0xD0
        virtual void CfObject_UnkVirtualFunc33(float amount); //0xD4
        virtual float CfObject_UnkVirtualFunc34();     //0xD8
        // Retail setEffScale_ / Coll UVF35: float in f1 -> +0x60.
        virtual void CfObject_UnkVirtualFunc35(float value); //0xDC
        // Retail: lfs f1,0x60(r3); blr - scale getter used to feed UVF35.
        virtual float CfObject_UnkVirtualFunc36();     //0xE0
        virtual void CfObject_UnkVirtualFunc37();     //0xE4
        virtual void CfObject_UnkVirtualFunc38();     //0xE8
        virtual void CfObject_UnkVirtualFunc39();     //0xEC
        virtual float CfObject_UnkVirtualFunc40();     //0xF0
        virtual void CfObject_UnkVirtualFunc41();     //0xF4
        virtual void CfObject_UnkVirtualFunc42();     //0xF8
        virtual void CfObject_UnkVirtualFunc43();     //0xFC
        virtual void CfObject_UnkVirtualFunc44();     //0x100
        virtual void CfObject_UnkVirtualFunc45();     //0x104
        virtual void CfObject_UnkVirtualFunc46();     //0x108
        virtual void CfObject_UnkVirtualFunc47();     //0x10C
        virtual void CfObject_UnkVirtualFunc48();     //0x110
        virtual void CfObject_UnkVirtualFunc49();     //0x114
        virtual int CfObject_UnkVirtualFunc50();     //0x118
        virtual int CfObject_UnkVirtualFunc51();     //0x11C
        virtual void CfObject_UnkVirtualFunc52();     //0x120
        virtual CfObject* CfObject_UnkVirtualFunc53(); //0x124
        virtual void CfObject_UnkVirtualFunc54();     //0x128
        virtual void CfObject_UnkVirtualFunc55();     //0x12C
        virtual float CfObject_UnkVirtualFunc56();     //0x130
        virtual void CfObject_UnkVirtualFunc57(float value);     //0x134
        virtual u32* CfObject_UnkVirtualFunc58();     //0x138
        virtual void CfObject_UnkVirtualFunc59();     //0x13C
        virtual void CfObject_UnkVirtualFunc60();     //0x140
        virtual void CfObject_UnkVirtualFunc61();     //0x144
        virtual void CfObject_UnkVirtualFunc62();     //0x148
        virtual void CfObject_UnkVirtualFunc63();     //0x14C
        virtual void CfObject_UnkVirtualFunc64(int flag);     //0x150
        virtual void CfObject_UnkVirtualFunc65();     //0x154
        virtual void CfObject_UnkVirtualFunc66(int) = 0; //0x158
        void func_800BFB90();
        virtual void CfObject_UnkVirtualFunc67();     //0x15C
        virtual int CfObject_UnkVirtualFunc68(); //0x160
        virtual void CfObject_UnkVirtualFunc69();     //0x164
        virtual void CfObject_UnkVirtualFunc70(float value); //0x168
        // Retail: return float in f1 (Model reads +0xA0; Coll returns const).
        // CfResPcImpl forwards that into CScnItemModel::vfunc48.
        virtual float CfObject_UnkVirtualFunc71();     //0x16C
        virtual void CfObject_UnkVirtualFunc72();     //0x170
        virtual void CfObject_UnkVirtualFunc73();     //0x174

        //not sure if belongs here? (can be in any class from CObjectState to CfObjectMove)
        void requestVoice(int, u32, float, float);


        //0x0: vtable
        // CObjectParam currently ends at 0x28.
        u8 field_0x28[0x10];
        void* mSubObj38;          // 0x38-0x3B
        float mPos3C;           // 0x3C-0x3F
        float mPos40;           // 0x40-0x43
        float mPos44;           // 0x44-0x47
        u8 _pad48[0x4C - 0x48];   // 0x48-0x4B
        float mField4C;           // 0x4C-0x4F
        u8 _pad50[0x54 - 0x50];   // 0x50-0x53
        CfObjectSub54 mSubObj54;     // 0x54-0x5F
        float mFloat60;            // 0x60-0x63
        u32 unk64;                  // 0x64-0x67
        u32 mFlags68;               // 0x68-0x6B
        u8 _pad6C[0x70 - 0x6C];    // 0x6C-0x6F
    };
}
