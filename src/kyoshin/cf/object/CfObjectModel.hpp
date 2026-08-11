#pragma once

#include <types.h>
#include "kyoshin/cf/object/CfObject.hpp"

// Retail data labels referenced by this unit.
extern float lbl_eu_80666A68;   // CfObject_UnkVirtualFunc20 constant / CfObject_UnkVirtualFunc56 fallback
extern float lbl_eu_80666A80;   // CfObject_UnkVirtualFunc20 (model) stack-vector middle element
extern float lbl_eu_80666A84;   // CfObject_UnkVirtualFunc20 (model) slot +0xB4 scale argument
extern float lbl_eu_8066A210;   // CfObject_UnkVirtualFunc32 scale factor
extern float lbl_eu_8066A20C;   // CfObject_UnkVirtualFunc34 scale factor
extern u8 lbl_eu_804FC548[];   // CfObjectModel_UnkVirtualFunc3 null placeholder (rodata, 8 bytes)

namespace cf {
    // Sub-object at CfObjectModel+0x98: flag words read by func_800BB934
    // (bit 0 of field_7A8) and CfObject_UnkVirtualFunc69 (bit 1 of field_7A4).
    struct CfObjectModelSub98 {
        u8 _pad00[0x7A4];             // 0x00-0x7A3
        u32 field_7A4;                // 0x7A4
        u32 field_7A8;                // 0x7A8
        u8 _pad7AC[0x14AC - 0x7AC];   // 0x7AC-0x14AB
        u32 field_14AC;               // 0x14AC (read by CfObject_UnkVirtualFunc54)
    };
    // Vtable-layout proxy for the model sub-object (CfObjectModel+0x98): its
    // retail vtable is cf-chain layout, and slot +0xA8 returns a u32 even
    // though the equivalent CfObject slot is declared void in the base
    // header. The inherited CObjectParam base occupies +0x00..+0x50, so the
    // 22nd new virtual lands at +0xA8 (dummy slots pin the offsets).
    class CfObjectModelSub98Vt : public CObjectParam {
    public:
        virtual void m54(); virtual void m58(); virtual void m5C(); virtual void m60();
        virtual void m64(); virtual void m68(); virtual void m6C(); virtual void m70();
        virtual void m74(); virtual void m78(); virtual void m7C(); virtual void m80();
        virtual void m84(); virtual void m88(); virtual void m8C(); virtual void m90();
        virtual void m94(); virtual void m98(); virtual void m9C(); virtual void mA0();
        virtual void mA4();
        virtual u32 mA8();  // vtable +0xA8 (returns a value in retail)
    };
    // Vtable proxy for the model sub-object's vtable slot +0x18: the retail
    // sub-object vtable is cf-chain layout, and slot +0x18 returns a pointer
    // even though the base CObjectState slot is declared void. Dummy slots
    // pin the offset.
    class CfObjectModelSub98Vt18 {
    public:
        virtual void m08(); virtual void m0C(); virtual void m10(); virtual void m14();
        virtual void* m18();  // vtable +0x18 (returns a value in retail)
    };
    // Vtable proxy for calling a cf-chain vtable slot +0x14C as a
    // u32-returning virtual (retail CfObject_UnkVirtualFunc63 returns a flag
    // word; the base header declares it void). Dummy slots pin the offset.
    // Vtable proxy for calling a cf-chain vtable slot +0xB4 with hidden
    // (const float* vec, float scale) args (retail CfObject_UnkVirtualFunc25
    // takes them; the base header declares it void). Dummy slots pin the
    // offset (same shape as CfObjectEffMoveIf in CfObjectEff.hpp).
    class CfObjectVtB4 : public CfObjectModelSub98Vt {
    public:
        virtual void mAC(); virtual void mB0();
        virtual void mB4(const float* vec, float scale);  // vtable +0xB4
    };
    class CfObjectVt14C : public CfObjectModelSub98Vt {
    public:
        virtual void mAC(); virtual void mB0(); virtual void mB4(); virtual void mB8();
        virtual void mBC(); virtual void mC0(); virtual void mC4(); virtual void mC8();
        virtual void mCC(); virtual void mD0(); virtual void mD4(); virtual void mD8();
        virtual void mDC(); virtual void mE0(); virtual void mE4(); virtual void mE8();
        virtual void mEC(); virtual void mF0(); virtual void mF4(); virtual void mF8();
        virtual void mFC(); virtual void m100(); virtual void m104(); virtual void m108();
        virtual void m10C(); virtual void m110(); virtual void m114(); virtual void m118();
        virtual void m11C(); virtual void m120(); virtual void m124(); virtual void m128();
        virtual void m12C(); virtual void m130(); virtual void m134(); virtual void m138();
        virtual void m13C(); virtual void m140(); virtual void m144(); virtual void m148();
        virtual u32 m14C();  // vtable +0x14C (returns a flag word in retail)
    };
    //min size: 0xbe
    class CfObjectModel : public CfObject {
    public:
        //vtable 1 (CfObject)
        virtual ~CfObjectModel();
        //vtable 1 (CfObjectModel)
        virtual void CfObjectModel_UnkVirtualFunc1();  //0x178
        virtual void CfObjectModel_UnkVirtualFunc2();  //0x17C
        virtual void* CfObjectModel_UnkVirtualFunc3();  //0x180 (retail returns a pointer)
        virtual void CfObjectModel_UnkVirtualFunc4();  //0x184
        virtual void CfObjectModel_UnkVirtualFunc5();  //0x188
        virtual void CfObjectModel_UnkVirtualFunc6();  //0x18C
        virtual void CfObjectModel_UnkVirtualFunc7();  //0x190
        virtual u32 CfObjectModel_UnkVirtualFunc8();   //0x194 (retail returns a bit value)
        virtual void CfObjectModel_UnkVirtualFunc9();  //0x198
        virtual void CfObjectModel_UnkVirtualFunc10(); //0x19C
        virtual void CfObjectModel_UnkVirtualFunc11(); //0x1A0
        virtual void CfObjectModel_UnkVirtualFunc12(); //0x1A4
        virtual void CfObjectModel_UnkVirtualFunc13(); //0x1A8
        virtual void CfObjectModel_UnkVirtualFunc14(); //0x1AC
        virtual void CfObjectModel_UnkVirtualFunc15(); //0x1B0
        virtual void CfObjectModel_UnkVirtualFunc16(); //0x1B4
        virtual void CfObjectModel_UnkVirtualFunc17(); //0x1B8
        virtual void CfObjectModel_UnkVirtualFunc18(); //0x1BC
        virtual void CfObjectModel_UnkVirtualFunc19(); //0x1C0
        virtual void CfObjectModel_UnkVirtualFunc20(); //0x1C4

        //0x0: vtable
        // CfObject ends at 0x70.
        u8 field_0x70[0x1C];
        u16 unk8C_3;
        u16 field_0x8E;
        u8 field_0x90[0x8];          // 0x90-0x97
        CfObjectModelSub98* mSubObj98; // 0x98-0x9B
        u8 field_0x9C[0x4];           // 0x9C-0x9F
        float field_A0;               // 0xA0-0xA3
        u8 field_0xA4[0xC];           // 0xA4-0xAF
        void* mSubObjB0;      // 0xB0-0xB3
        u8 unkB4[0xBC - 0xB4]; // 0xB4-0xBB
        u8 field_BC;          // 0xBC
        u8 field_BD;          // 0xBD
    CfObjectModel();
    void CfObject_UnkVirtualFunc2();
    void CfObject_UnkVirtualFunc6();
    void CfObject_UnkVirtualFunc8();
    void CfObject_UnkVirtualFunc63();
    void CfObject_UnkVirtualFunc19();
    void CfObject_UnkVirtualFunc22();
    void CfObject_UnkVirtualFunc20();
    u32 CfObject_UnkVirtualFunc23();
    void CfObject_UnkVirtualFunc27(void* src);
    void CfObject_UnkVirtualFunc29();
    void CfObject_UnkVirtualFunc32();
    float CfObject_UnkVirtualFunc34();
    void CfObject_UnkVirtualFunc33(float amount);
    void CfObject_UnkVirtualFunc30();
    float CfObject_UnkVirtualFunc56();
    void CfObject_UnkVirtualFunc52();
    CfObject* CfObject_UnkVirtualFunc53();
    void CfObject_UnkVirtualFunc54();
    void CfObject_UnkVirtualFunc55();
    void* CObjectParam_UnkVirtualFunc2();
    void CfObject_UnkVirtualFunc66(int);
    void CfObject_UnkVirtualFunc67();
    void CfObject_UnkVirtualFunc70(float value);
    void CfObject_UnkVirtualFunc69();
    void CfObject_UnkVirtualFunc68();
    void CfObject_UnkVirtualFunc24();
    void CfObject_UnkVirtualFunc28();
    float CfObject_UnkVirtualFunc31();
    void CfObject_UnkVirtualFunc35();
    void CfObject_UnkVirtualFunc36();
    void CfObject_UnkVirtualFunc72();
    };
}

// Output vector pair filled by func_80490A44 (monolib scene helper used by
// CfObject_UnkVirtualFunc56: the two positions' Y difference is returned).
struct CfObjectModelVec3 {
    float x;  // 0x00
    float y;  // 0x04
    float z;  // 0x08
};

// Imports (retail uses unmangled names; global scope so MWCC emits them
// as-is, no extern "C" needed).
// Releases a sub-object (used by CfObjectModel_UnkVirtualFunc1).
void func_80495E60(void* ptr);
// Fills the two output vectors for the model sub-object (used by
// CfObject_UnkVirtualFunc56).
void func_80490A44(cf::CfObjectModelSub98* obj, CfObjectModelVec3* outA, CfObjectModelVec3* outB);

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// Function is defined in CfBdat.cpp with C linkage (retail uses unmangled name)
extern "C" void func_80142428();

// C-linkage import from libs/monolib/src/scn/CScnItemModel.cpp (retail uses
// the unmangled name); tail-called by func_800BB618 with the model sub-object.
extern "C" void func_804838DC(cf::CfObjectModelSub98* model, int flag);

// operator delete (retail symbol __dl__FPv is the unmangled C name; declare
// extern "C" so the bl reloc references exactly __dl__FPv).
extern "C" void __dl__FPv(u8* object);

