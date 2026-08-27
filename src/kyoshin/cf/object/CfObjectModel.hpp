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
    // Vtable-layout proxy for the model sub-object (CfObjectModel+0x98): its
    // retail vtable is cf-chain layout, and slot +0xA8 returns a u32 even
    // though the equivalent CfObject slot is declared void in the base
    // header. The inherited CObjectParam base occupies +0x00..+0x50, so the
    // 22nd new virtual lands at +0xA8 (dummy slots pin the offsets).
    class CfObjectModelSub98Vt : public CObjectParam {
    public:
        virtual void m54(); virtual void m58(); virtual void m5C(); virtual void m60();
        virtual void m64(); virtual void m68(); virtual void m6C(u32 arg); virtual void m70();
        virtual void m74(); virtual void m78(); virtual void m7C(); virtual void m80();
        virtual void m84(); virtual void m88(); virtual void m8C(); virtual void m90(const float* vec, u32 arg);
        virtual void m94(); virtual void m98(); virtual void m9C(); virtual void mA0();
        virtual void mA4();
        virtual u32 mA8();  // vtable +0xA8 (returns a value in retail)
        // m6C at +0x6C and m90 at +0x90 take explicit args in retail
        // (CfObjectModel_UnkVirtualFunc13 forwards r4; func_800BBA08 passes a
        // position vector + flags), unlike the base header's no-arg slots.
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
    // Vtable proxy for calling a cf-chain vtable slot +0x44 with an explicit
    // int argument (retail CfObject_UnkVirtualFunc55 forwards its flag
    // through; the base CObjectParam header declares the slot without args).
    // Dummy slots pin the offset (same shape as CfObjectModelSub98Vt18).
    class CfObjectParamVt44 {
    public:
        virtual void m08(); virtual void m0C(); virtual void m10(); virtual void m14();
        virtual void m18(); virtual void m1C(); virtual void m20(); virtual void m24();
        virtual void m28(); virtual void m2C(); virtual void m30(); virtual void m34();
        virtual void m38(); virtual void m3C(); virtual void m40();
        virtual int m44(int arg);  // vtable +0x44
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
    // Vtable proxy for calling cf-chain vtable slots +0x16C/+0x174 as
    // float-returning virtuals (retail CfObject_UnkVirtualFunc68 reads both
    // results; the base header declares the slots void). Extends the +0x14C
    // proxy; dummy slots pin the offsets.
    class CfObjectVt174 : public CfObjectVt14C {
    public:
        virtual void m150(); virtual void m154(); virtual void m158();
        virtual void m15C(); virtual void m160(); virtual void m164(); virtual void m168();
        virtual float m16C();  // vtable +0x16C (CfObject_UnkVirtualFunc71 returns a float in retail)
        virtual void m170();
        virtual float m174();  // vtable +0x174 (CfObject_UnkVirtualFunc73 returns a float in retail)
    };
    // Vtable proxy for calling a cf-chain vtable slot +0x120 with a name-string
    // argument and a u32 result (retail CfObjectModel_UnkVirtualFunc14 checks
    // the return; the base header declares the slot no-arg/void). Dummy slots
    // pin the offset.
    class CfObjectVt120 : public CfObjectModelSub98Vt {
    public:
        virtual void mAC(); virtual void mB0(); virtual void mB4(); virtual void mB8();
        virtual void mBC(); virtual void mC0(); virtual void mC4(); virtual void mC8();
        virtual void mCC(); virtual void mD0(); virtual void mD4(); virtual void mD8();
        virtual void mDC(); virtual void mE0(); virtual void mE4(); virtual void mE8();
        virtual void mEC(); virtual void mF0(); virtual void mF4(); virtual void mF8();
        virtual void mFC(); virtual void m100(); virtual void m104(); virtual void m108();
        virtual void m10C(); virtual void m110(); virtual void m114(); virtual void m118();
        virtual void m11C();
        virtual u32 m120(const char* name);  // vtable +0x120
    };
    // Vtable proxy for calling the model sub-object's vtable slot +0x3C with a
    // name argument; the slot returns a matrix pointer whose translation column
    // (+0xC/+0x1C/+0x2C) feeds the position vector.
    class CfObjectParamVt3C {
    public:
        virtual void m08(); virtual void m0C(); virtual void m10(); virtual void m14();
        virtual void m18(); virtual void m1C(); virtual void m20(); virtual void m24();
        virtual void m28(); virtual void m2C(); virtual void m30(); virtual void m34();
        virtual void m38();
        virtual float* m3C(const char* name);  // vtable +0x3C
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
        virtual void* CfObjectModel_UnkVirtualFunc6(void* arg);  //0x18C (retail returns a pointer)
        virtual void CfObjectModel_UnkVirtualFunc7();  //0x190
        virtual u32 CfObjectModel_UnkVirtualFunc8();   //0x194 (retail returns a bit value)
        virtual void CfObjectModel_UnkVirtualFunc9();  //0x198
        virtual void CfObjectModel_UnkVirtualFunc10(); //0x19C
        virtual void CfObjectModel_UnkVirtualFunc11(); //0x1A0
        virtual void CfObjectModel_UnkVirtualFunc12(); //0x1A4
        virtual void CfObjectModel_UnkVirtualFunc13(const ml::CVec3* pos); //0x1A8 (retail passes a stack vector)
        virtual void CfObjectModel_UnkVirtualFunc14(); //0x1AC
        virtual void CfObjectModel_UnkVirtualFunc15(); //0x1B0
        virtual void CfObjectModel_UnkVirtualFunc16(); //0x1B4
        virtual void CfObjectModel_UnkVirtualFunc17(); //0x1B8
        virtual void CfObjectModel_UnkVirtualFunc18(); //0x1BC
        virtual void CfObjectModel_UnkVirtualFunc19(); //0x1C0
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
    void CfObject_UnkVirtualFunc63();
    void CfObject_UnkVirtualFunc19(const ml::CVec3* vec);
    // UVF22 sets position (same ABI as UVF19); r3 stays self after the call.
    void CfObject_UnkVirtualFunc22(const ml::CVec3* vec);
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
    int CfObject_UnkVirtualFunc68();
    void CfObject_UnkVirtualFunc24();
    void CfObject_UnkVirtualFunc28();
    float CfObject_UnkVirtualFunc31();
    void CfObject_UnkVirtualFunc35(float value);
    void CfObject_UnkVirtualFunc25(ml::CVec3* pos, float scale);
    void CfObject_UnkVirtualFunc26(const ml::CVec3* vec, float amount);
    float CfObject_UnkVirtualFunc36();
    void CfObject_UnkVirtualFunc72();
    };
    // Vtable view of the CfObject-family sub-object at CfObjectModel+0x38
    // (mSubObj38): slots 0x8 (release-with-delete-flag), 0x2C (attach parent),
    // 0xA0 (activate) and 0xAC (release) are called by
    // CfObject_UnkVirtualFunc6 / CfObject_UnkVirtualFunc8. Dummy slots pin
    // the offsets (MWCC puts the Nth declared virtual at (N+1)*4).
    class CfObjectSub38If {
    public:
        virtual void m08(int flag);  // vtable +0x08
        virtual void _f0C(); virtual void _f10(); virtual void _f14(); virtual void _f18();
        virtual void _f1C(); virtual void _f20(); virtual void _f24(); virtual void _f28();
        virtual void m2C(CfObjectModel* parent);  // vtable +0x2C
        virtual void _f30(); virtual void _f34(); virtual void _f38(); virtual void _f3C();
        virtual void _f40(); virtual void _f44(); virtual void _f48(); virtual void _f4C();
        virtual void _f50(); virtual void _f54(); virtual void _f58(); virtual void _f5C();
        virtual void _f60(); virtual void _f64(); virtual void _f68(); virtual void _f6C();
        virtual void _f70(); virtual void _f74(); virtual void _f78(); virtual void _f7C();
        virtual void _f80(); virtual void _f84(); virtual void _f88(); virtual void _f8C();
        virtual void _f90(); virtual void _f94(); virtual void _f98(); virtual void _f9C();
        virtual void mA0();  // vtable +0xA0
        virtual void _fA4(); virtual void _fA8();
        virtual void mAC();  // vtable +0xAC
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
extern "C" void notifyDetach_(void* self);
extern "C" void reattachTrg__(void* arg0, void* arg1);
// Effect reattachment helper (defined in CfObjectEff.cpp). C linkage keeps
// the call-site reloc at the plain retail name (a C++ declaration makes
// MWCC emit the mangled __FPUc reference).
extern "C" void detachTrgPrt_(u8* arg);

