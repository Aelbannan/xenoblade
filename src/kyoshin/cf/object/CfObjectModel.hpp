#pragma once

#include <types.h>
#include "kyoshin/cf/object/CfObject.hpp"
#include "monolib/math/CVec3.hpp"

// Retail data labels referenced by this unit.
extern const float lbl_eu_80666A68;   // CfObject_UnkVirtualFunc20 constant / CfObject_UnkVirtualFunc56 fallback
extern float lbl_eu_80666A6C;   // func_800BBA08 position constant (stack-vector fill)
extern const float lbl_eu_80666A70;   // UnkVirtualFunc25 first probe FP arg
extern const float lbl_eu_80666A74;   // UnkVirtualFunc25 probe second FP arg
extern const float lbl_eu_80666A78;   // UnkVirtualFunc25 lift-offset Y element
extern const float lbl_eu_80666A7C;   // UnkVirtualFunc25 lifted re-probe FP arg
extern const float lbl_eu_80666A80;   // CfObject_UnkVirtualFunc20 (model) stack-vector middle element
extern const float lbl_eu_80666A84;   // CfObject_UnkVirtualFunc20 (model) slot +0xB4 scale argument
extern const float lbl_eu_8066A210;   // CfObject_UnkVirtualFunc32 scale factor
extern const float lbl_eu_8066A20C;   // CfObject_UnkVirtualFunc34 scale factor
extern u8 lbl_eu_804FC548[];   // CfObjectModel_UnkVirtualFunc3 null placeholder (rodata, 8 bytes)
extern u8 lbl_eu_80529318[];   // cf::CfObject vtable (retail .data, stored by the model dtor)
extern u8 lbl_eu_805294E0[];   // cf::CfObjectModel vtable (retail .data)
extern const char lbl_eu_80529678[];   // Panic file (node-lookup assert, line 0x53)
extern const char lbl_eu_80529658[];   // Panic fmt (CfObject_UnkVirtualFunc52/53)

namespace nw4r {
namespace g3d {
struct ResMdlData;  // forward decl (full type in nw4r/g3d/res/g3d_resmdl.h)
}
}

// Retail ml static zero vector (unmangled symbol; ml::CVec3 comes from
// monolib/math/CVec3.hpp, already included transitively).
extern ml::CVec3 zero__Q22ml5CVec3;

namespace cf {
    // Sub-object at CfObjectModel+0x98: flag words read by func_800BB934
    // (bit 0 of field_7A8) and CfObject_UnkVirtualFunc69 (bit 1 of field_7A4).
    // Per-node matrix table reached through CfObjectModelSub98+0x147C: its
    // +0xEC word is the base of 0x30-byte matrix slots (CfObject_UnkVirtualFunc52/53).
    struct CfObjectModelActTable {
        u8 _padEC[0xEC];
        u8* field_EC;  // 0xEC
    };
    struct CfObjectModelSub98 {
        u8 _pad00[0x7A4];             // 0x00-0x7A3
        u32 field_7A4;                // 0x7A4
        u32 field_7A8;                // 0x7A8
        u8 _pad7AC[0x146C - 0x7AC];   // 0x7AC-0x146B
        nw4r::g3d::ResMdlData* field_146C;  // 0x146C (model resource data)
        u8 _pad1470[0x147C - 0x1470]; // 0x1470-0x147B
        CfObjectModelActTable* field_147C;  // 0x147C
        u8 _pad1480[0x14AC - 0x1480]; // 0x1480-0x14AB
        u32 field_14AC;               // 0x14AC (read by CfObject_UnkVirtualFunc54)
    };
    // vtable proxy for calling a cf-chain vtable slot +0x44 with an explicit
    // int argument (retail CfObject_UnkVirtualFunc55 forwards its flag
    // through; the base CObjectParam header declares the slot without args).
    // Dummy slots pin the offset (same shape as CfObjectModelSub98vt18).
    class CfObjectParamReal {
    public:
        virtual void m08(); virtual void m0C(); virtual void m10(); virtual void m14();
        virtual void m18(); virtual void m1C(); virtual void m20(); virtual void m24();
        virtual void m28(); virtual void m2C(); virtual void m30(); virtual void m34();
        virtual void m38(); virtual void m3C(); virtual void m40();
        virtual int m44(int arg);  // vtable +0x44
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
        virtual void* CfObjectModel_UnkVirtualFunc4();  //0x184
        virtual u32 CfObjectModel_UnkVirtualFunc5();  //0x188
        virtual void* CfObjectModel_UnkVirtualFunc6(void* arg);  //0x18C (retail returns a pointer)
        virtual void CfObjectModel_UnkVirtualFunc7();  //0x190
        virtual u32 CfObjectModel_UnkVirtualFunc8();   //0x194 (retail returns a bit value)
        virtual void CfObjectModel_UnkVirtualFunc9();  //0x198
        virtual void CfObjectModel_UnkVirtualFunc10(); //0x19C
        virtual void CfObjectModel_UnkVirtualFunc11(); //0x1A0
        virtual void CfObjectModel_UnkVirtualFunc12(float value); //0x1A4
        virtual void CfObjectModel_UnkVirtualFunc13(const ml::CVec3* pos); //0x1A8 (retail passes a stack vector)
        virtual void CfObjectModel_UnkVirtualFunc14(cf::CfObject* other, const char* name); //0x1AC
        virtual void CfObjectModel_UnkVirtualFunc15(); //0x1B0
        virtual void CfObjectModel_UnkVirtualFunc16(); //0x1B4
        virtual void CfObjectModel_UnkVirtualFunc17(); //0x1B8
        virtual void CfObjectModel_UnkVirtualFunc18(); //0x1BC
        virtual void CfObjectModel_UnkVirtualFunc19(int flag); //0x1C0
        virtual void CfObjectModel_UnkVirtualFunc20(); //0x1C4

        //0x0: vtable
        // CfObject ends at 0x70.
        void* mTarget70;              // 0x70 (first word of field_0x70)
        u8 field_0x74[0x1C - 4];      // 0x74-0x8F
        u16 unk8C_3;
        u16 field_0x8E;
        u8 field_0x90[0x8];          // 0x90-0x97
        CfObjectModelSub98* mSubObj98; // 0x98-0x9B
        u8 field_0x9C[0x4];           // 0x9C-0x9F
        float field_A0;               // 0xA0-0xA3
        u8 field_0xA4[0xC];           // 0xA4-0xAF
        void* mSubObjB0;      // 0xB0-0xB3
        cf::CfObjectModel* field_B4; // 0xB4 (source object stored by UnkVirtualFunc14)
        const char* field_B8;        // 0xB8 (name/fallback label stored by UnkVirtualFunc14)
        u8 field_BC;          // 0xBC
        u8 field_BD;          // 0xBD
    CfObjectModel();
    void CfObject_UnkVirtualFunc2();
    void CfObject_UnkVirtualFunc6();
    void CfObject_UnkVirtualFunc8();
    virtual u32 CfObject_UnkVirtualFunc63() override;
    void CfObject_UnkVirtualFunc19();
    CfObjectModel* CfObject_UnkVirtualFunc22();  // covariant (retail callers read r3 after dispatch)
    void CfObject_UnkVirtualFunc20();
    ml::CVec3* CfObject_UnkVirtualFunc23();
    void CfObject_UnkVirtualFunc27(void* src);
    void CfObject_UnkVirtualFunc29();
    void CfObject_UnkVirtualFunc32();
    float CfObject_UnkVirtualFunc34();
    void CfObject_UnkVirtualFunc33(float amount);
    void CfObject_UnkVirtualFunc30();
    float CfObject_UnkVirtualFunc56();
    void CfObject_UnkVirtualFunc52();
    CfObject* CfObject_UnkVirtualFunc53();
    u32 CfObject_UnkVirtualFunc54();
    void* CfObject_UnkVirtualFunc55(int arg);
    void* CObjectParam_UnkVirtualFunc2();
    void CfObject_UnkVirtualFunc66(int);
    void CfObject_UnkVirtualFunc67();
    void CfObject_UnkVirtualFunc70(float value);
    void CfObject_UnkVirtualFunc69();
    int CfObject_UnkVirtualFunc68(const ml::CVec3* vec) override;
    void CfObject_UnkVirtualFunc24();
    void CfObject_UnkVirtualFunc28();
    float CfObject_UnkVirtualFunc31();
    void CfObject_UnkVirtualFunc35();
    float CfObject_UnkVirtualFunc36();
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

// Imports (retail uses unmangled names; extern "C" so the linkage matches
// the CfObjectMap.hpp declarations of the same helpers).
// Releases a sub-object (used by CfObjectModel_UnkVirtualFunc1).
extern "C" void func_80495E60(void* ptr);
// Fills the two output vectors for the model sub-object (used by
// CfObject_UnkVirtualFunc56).
extern "C" void func_80490A44(cf::CfObjectModelSub98* obj, CfObjectModelVec3* outA, CfObjectModelVec3* outB);

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// Function is defined in CfBdat.cpp with C linkage (retail uses unmangled name)
extern "C" void func_80142428();

// C-linkage import from libs/monolib/src/scn/CScnItemModel.cpp (retail uses
// the unmangled name); tail-called by func_800BB618 with the model sub-object.
extern "C" void func_804838DC(cf::CfObjectModelSub98* model, int flag);

// operator delete (retail symbol __dl__FPv is the unmangled C name; declare
// extern "C" so the bl reloc references exactly __dl__FPv).
extern "C" void __dl__FPv(void* object);

// Imports for CfObjectModel_UnkVirtualFunc13 (sets sub-object +0x7A8 flag
// bits; defined in libs/monolib CScnItemModel.cpp) and func_800BBADC
// (reattaches a detached effect target; defined in CfObjectEff.cpp).
// extern "C" so the bl relocs reference the unmangled retail names (a plain
// C++ header declaration makes MWCC emit a mangled __F<params> name).
extern "C" void func_80484E10(void* self, u32 a, u32 b);
// Registers the caller with the effect chain (defined in CfObjectEff.cpp;
// called by CfObject_UnkVirtualFunc6).
extern "C" void func_800AD4B0(void* self);
extern "C" void func_800AD5EC(void* arg0, void* arg1);
// Effect reattachment helper (defined in CfObjectEff.cpp). C linkage keeps
// the call-site reloc at the plain retail name (a C++ declaration makes
// MWCC emit the mangled __FPUc reference).
extern "C" void func_800AD558(u8* arg);
extern "C" void notifyDetach_(void* self);
extern "C" void reattachTrg__(void* a, void* b);
extern "C" void detachTrgPrt_(u8* arg);

