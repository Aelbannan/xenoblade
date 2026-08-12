#pragma once

#include <types.h>
#include "monolib/work/IWorkEvent.hpp"
#include "kyoshin/CEquipItemBox.hpp"

namespace nw4r {
namespace lyt {
    class AnimTransform;
    class DrawInfo;
}
}
class CBaseCur;
struct CItemBoxInfo;
class UnkClass_8045F564;

/* Sets mVtbl before members are constructed (retail ctor order).
   Manual vtable base (non-polymorphic) so MWCC does not emit a vptr re-seat
   store in ~CEquipChange (retail shape) - same trick as CBatteryVtblBase. */
struct CEquipChangeVtblBase {
    void* mVtbl; // 0x0
};

/* Layout verified against extab destroy-member order:
   0x00: IWorkEvent base (vtable ptr)
   0x04: UnkClass_8045F564 (0x10 bytes)
   0x14: UnkClass_8045F564 (0x3C bytes)
   0x50: CCur14           (0x18 bytes)
   0x68: CCur15           (0x18 bytes)
   0x80: CSubCur          (0x24 bytes)
   0xA4: CItemBoxInfo     (0x20C bytes)
   0x2B0: CEquipItemBox */
class CEquipChange : public CEquipChangeVtblBase {
public:
    ~CEquipChange();
    bool OnFileEvent(CEventFile*);
    u8 func_802023C0();
    void func_802023C8();
    void func_8020247C();
    void func_8020397C();
    void func_80203984();
    void func_8020398C();

    // 0x04: UnkClass_8045F564 (size 0x10)
    u8 _pad04[0x10];
    // 0x14: UnkClass_8045F564 (size 0x3C)
    u8 _pad14[0x10];                       // 0x14..0x24
    u32 field_24;                          // 0x24 - file handle 1
    u32 field_28;                          // 0x28 - file handle 2
    nw4r::lyt::ArcResourceAccessor* field_2C; // 0x2C - arc resource accessor
    u32 field_30;                          // 0x30
    u32 field_34;                          // 0x34
    nw4r::lyt::AnimTransform* field_38;    // 0x38
    nw4r::lyt::AnimTransform* field_3C;    // 0x3C
    nw4r::lyt::AnimTransform* field_40;    // 0x40
    u8 field_44;                           // 0x44
    u8 _pad45[0x03];                       // 0x45..0x48
    u32 field_48;                          // 0x48
    u8 field_4C;                           // 0x4C
    u8 field_4D;             // 0x4D
    u8 _pad4E[0x02];
    // 0x50: CCur14 (size 0x18)
    u8 _pad50[0x18];
    // 0x68: CCur15 (size 0x18)
    u8 _pad68[0x18];
    // 0x80: CSubCur (size 0x24)
    u8 field_80[0x18];       // 0x80
    s8 field_98;              // 0x98
    u8 field_99;              // 0x99
    u8 _pad9A[0x0A];          // 0x9A..0xA4
    // 0xA4: CItemBoxInfo (size 0x20C)
    u8 _padA4[0x20C];
    // 0x2B0: CEquipItemBox
    CEquipItemBox mEquipItemBox;  // 0x2B0
};

// Retail symbols are unmangled free functions (not class members); MWCC would
// mangle plain C++ declarations, so they carry C linkage to emit the exact
// retail symbols.
extern "C" int func_80203138(CEquipChange* self);
extern "C" int func_802031A0(CEquipChange* self);

// C++ linkage so MWCC mangles to the retail symbol func_80137444__FPQ34nw4r3lyt13AnimTransformf.
u32 func_80137444(nw4r::lyt::AnimTransform*, float);

// Cast-only view of the layout object at field_34 (vtable slot 11 = +0x2C,
// method (arg, mode)). A real virtual call makes MWCC load the vtable into
// r12 (retail's `lwz r12,0(r3); lwz r12,0x2c(r12); bctrl`), where a
// function-pointer temp would allocate it to a scratch register. MWCC reserves
// 2 leading vtable slots, so v9 lands at +0x2C.
struct CLayoutVtbl11 {
    virtual void v0(int arg);      // +0x08 (slot 0 after 2 reserved)
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9(void* arg, int mode);
    virtual void v10();            // +0x30
    virtual void v11();            // +0x34
    virtual void v12(int arg);     // +0x38
};

// Cast-only view of the sub-cursor at self+0x80 (vtable slot 4 = +0x10,
// method (arg)). Real virtual call emits the retail r12 dispatch chain;
// MWCC reserves 2 leading vtable slots, so v2 lands at +0x10.
struct CSubCurVtblView {
    virtual void v0();
    virtual void v1();
    virtual void v2(void* arg);
};

// Cast-only view of the cursors at self+0x50/0x68/0x80 (vtable slot 3 =
// +0x0C, no-arg method). v1 lands at +0x0C (2 reserved leading slots).
struct CCurVtblView {
    virtual void v0();
    virtual void v1();
};

// Color/sound palette entries initialised by sinit_802059E8 (sdata2).
extern void* lbl_eu_80664668;
extern void* lbl_eu_80664670;
extern void* lbl_eu_80664678;
extern void* lbl_eu_80664680;
extern void* lbl_eu_80664688;
extern void* lbl_eu_80664690;

// Global equip-page flag (sdata), cleared by func_8020228C.
extern u32 lbl_eu_80664698;


// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// Retail emits these as C-style (unmangled) symbols, so reference them with C
// linkage. func_801D2ED8/CEquipItemBox gates take the object and return status.
extern "C" u32 func_80137510(nw4r::lyt::AnimTransform*, float);

// CItemBoxInfo helpers used by the equip-change screen (C-ABI retail names).
extern "C" u32 func_801D421C(CItemBoxInfo* info);
extern "C" u32 func_801D4260(CItemBoxInfo* info, u8 arg2);
extern "C" void func_801D47D4(CItemBoxInfo* info, u16 arg2, void* arg3, u16 arg4);
extern "C" void func_801D202C(void* cur);
extern "C" void func_801D40C4(CItemBoxInfo* info);
extern "C" void func_801D4174(CItemBoxInfo* info);

// Free-function form (retail: advanceItemBoxState__FP12CItemBoxInfo).
void advanceItemBoxState(CItemBoxInfo* info);

extern "C" void func_80286264(CEquipItemBox* box);
extern "C" void func_80286454(CEquipItemBox* box);
extern "C" void func_802867E0(CEquipItemBox* box);
extern "C" int func_801D2ED8(CBaseCur*);
extern "C" int func_802865A0(CEquipItemBox* box);
extern "C" int func_802865A8(CEquipItemBox* box);
extern "C" int func_802865B0(CEquipItemBox* box);
extern "C" void func_80286740(CEquipItemBox* box);
extern "C" void func_80287250(CEquipItemBox* box, int arg);
extern "C" void func_801D4B3C(void*, CItemBoxInfo*, u32);
extern "C" int func_80286650(CEquipItemBox* box);
extern "C" int func_80286698(CEquipItemBox* box);
extern "C" int func_802866A0(CEquipItemBox* box);
extern "C" int func_8028652C(CEquipItemBox* box);
extern "C" int func_80287EE8(CEquipItemBox* box);
extern "C" void func_802870DC(CEquipItemBox* box);
extern "C" void func_802040FC(CEquipChange* self);
extern "C" void func_801D4054(void* info);
extern "C" void func_802861A8(CEquipItemBox* box);
extern "C" u8 code80135FDC_getByte_64077();
extern "C" void func_802042C0(CEquipChange* self);
extern "C" void* func_8009EC9C(u32);
extern "C" void* func_80157C4C(u32 index, s16 value);
extern "C" void func_80287EFC(CEquipItemBox* box, u32 val);
extern "C" void func_80287DB4(CEquipItemBox* box, u16 packed, void* arg3, u16 arg4);
extern "C" void func_802866E8(CEquipItemBox* box);
extern "C" int func_8028876C(CEquipItemBox* box);
extern "C" void func_80286340(CEquipItemBox* box, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void* func_802052A8(CEquipChange* self);
extern "C" void func_801D20B0(CBaseCur* cur, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void func_801D4154(CItemBoxInfo* info, nw4r::lyt::DrawInfo* drawInfo);

// Sub-object destructors referenced by ~CEquipChange. Defined in CCur.cpp /
// CItemBoxInfo.cpp / CEquipItemBox.cpp.
extern "C" void* __dt__7CSubCurFv(CBaseCur* _this, int flags);
extern "C" void* __dt__6CCur14Fv(CBaseCur* _this, int flags);
extern "C" void* __dt__6CCur15Fv(CBaseCur* _this, int flags);
extern "C" void __dt__17UnkClass_8045F564Fv(void* _this, int flags);
extern "C" void* __dt__12CItemBoxInfoFv(CItemBoxInfo* _this, int flags);
extern "C" void* __dt__13CEquipItemBoxFv(CEquipItemBox* _this, int flags);
