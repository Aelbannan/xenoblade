#pragma once

#include <types.h>
#include "monolib/work/IWorkEvent.hpp"
#include "kyoshin/CEquipItemBox.hpp"

namespace nw4r {
namespace lyt {
    class AnimTransform;
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
    int func_80203138();
    void func_8020397C();
    void func_80203984();
    void func_8020398C();

    // 0x04: UnkClass_8045F564 (size 0x10)
    u8 _pad04[0x10];
    // 0x14: UnkClass_8045F564 (size 0x3C)
    u8 _pad14[0x10];                       // 0x14..0x24
    u32 field_24;                          // 0x24 - file handle 1
    u32 field_28;                          // 0x28 - file handle 2
    u8 _pad2C[0x04];                       // 0x2C..0x30
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


// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// Retail emits these as C-style (unmangled) symbols, so reference them with C
// linkage. func_801D2ED8/CEquipItemBox gates take the object and return status.
extern "C" u32 func_80137510(nw4r::lyt::AnimTransform*, float);
extern "C" int func_801D2ED8(CBaseCur*);
extern "C" int func_802865A0(CEquipItemBox* box);
extern "C" int func_802865A8(CEquipItemBox* box);
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

// Sub-object destructors referenced by ~CEquipChange. Defined in CCur.cpp /
// CItemBoxInfo.cpp / CEquipItemBox.cpp.
extern "C" void* __dt__7CSubCurFv(CBaseCur* _this, int flags);
extern "C" void* __dt__6CCur14Fv(CBaseCur* _this, int flags);
extern "C" void* __dt__6CCur15Fv(CBaseCur* _this, int flags);
extern "C" void __dt__17UnkClass_8045F564Fv(void* _this, int flags);
extern "C" void* __dt__12CItemBoxInfoFv(CItemBoxInfo* _this, int flags);
extern "C" void* __dt__13CEquipItemBoxFv(CEquipItemBox* _this, int flags);
