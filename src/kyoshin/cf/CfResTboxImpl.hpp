#pragma once

#include <types.h>
#include "kyoshin/cf/CfGameManagerData.hpp"
#include "kyoshin/cf/object/CfObject.hpp"
#include "kyoshin/cf/object/CfObjectModel.hpp"

class CScnItemModel; // fwd; real owner of parent+0x98 model (hot header)

struct ResInfoEntry;
class CScn;

namespace cf {

class CfObjectMove;

struct CfResTboxImpl;
struct CfResTboxParent;

class CfObject; // real owner of +0x38 sub-object (CfObject_UnkVirtualFunc24)

// Prefix pushing vptr to +0x10 (same as CfResPcImpl / CfResReloadImpl)
struct CfResTboxImplPrefix {
    /* 0x00 */ CfResTboxParent* field_00;
    /* 0x04 */ f32 field_04;
    /* 0x08 */ u16 field_08;
    /* 0x0A */ s16 field_0A;
    /* 0x0C */ u16 field_0C;
    /* 0x0E */ s16 field_0E;
};

struct CfResTboxImplVtbl {
    void* slots[27];
};

// Real class tree for cf::CfResTboxImpl (retail lbl_eu_80535204, 0x6C: RTTI + 0 + 25 slots). novtable.
class __declspec(novtable) CfResTboxImpl : public CfResTboxImplPrefix {
public:
    virtual ~CfResTboxImpl();                // 0x08
    virtual void func_801F91B4();            // 0x0C
    virtual void func_800BEA34();            // 0x10
    virtual int func_8016C860();             // 0x14
    virtual void func_800BE9AC();            // 0x18
    virtual void func_8016CD64();            // 0x1C
    virtual int func_801F8E70();             // 0x20
    virtual void func_800BC2DC();            // 0x24
    virtual void func_800BF2F4();            // 0x28
    virtual void func_eu_800BFC78();         // 0x2C
    virtual void func_8016CD68(int idx, int value); // 0x30
    virtual u32 func_8016CCBC(int idx);      // 0x34
    virtual void func_800BEA38();            // 0x38
    virtual void func_800BED64();            // 0x3C
    virtual void func_800BEE30();            // 0x40
    virtual void func_800BC3AC();            // 0x44
    virtual void func_800BEC44();            // 0x48
    virtual void func_800BED5C();            // 0x4C
    virtual void func_8016CD5C();            // 0x50
    virtual void func_8016C888(int a, int b, int c, float f1, float f2); // 0x54
    virtual void func_800BF2C4();            // 0x58
    virtual void func_800BF2C8();            // 0x5C
    virtual int func_801F8E78();             // 0x60
    virtual int func_8016CD54();             // 0x64
    virtual int func_800BF30C();             // 0x68

    void*& vtbl() { return *reinterpret_cast<void**>(reinterpret_cast<u8*>(this) + 0x10); }

    /* 0x14 */ u32 field_14[2];
};

// Parent object referenced at +0x00 of CfResTboxImpl (a CfObject-derived
// instance whose vptr sits at +0x00). Only the fields this unit touches are
// declared.
struct CfResTboxParent {
    u8 field_00[0x38];
    /* 0x38 */ cf::CfObject* field_38;       // sub-object (vtable slot 0xB0 -> CfObject_UnkVirtualFunc24)
    u8 field_3C[0x6C - 0x3C];
    /* 0x6C */ u32 field_6C;
    u8 field_70[0x90 - 0x70];
    /* 0x90 */ u8* field_90;
    /* 0x94 */ u8* field_94;
    /* 0x98 */ ::CScnItemModel* field_98;  // model object (vtable slots 0x78(void*)/0x88(int) -> CScnItemModel::vfunc78/88)
    /* 0x9C */ u8* field_9C;
};

// Foreign 0x88 iface deleted: real CScnItemModel::vfunc88(int) now owned by
// CScnItemModel header (hot header, widened to retail arity).

} // namespace cf

struct CfPmf3 {
    u32 w0;
    u32 w1;
    u32 w2;
};

typedef void (cf::CfResTboxImpl::*CfResTboxImplPMF)();
extern CfResTboxImplPMF lbl_eu_805351E0[3];
extern char lbl_eu_80664660;
extern u32 __ptmf_null[3];
extern u8 lbl_eu_80535204[];
extern float lbl_eu_806681D0;
extern float lbl_eu_806681D4;
void func_801F92B0(u8* base, int idx1, int idx2, int idx3);
extern "C" bool isSceneActive__Q22cf13CfGameManagerFv();
extern "C" char* func_80063080();
extern "C" u8* func_80066E7C(ResInfoEntry* self, u32 id);
extern "C" void* func_80062114(char* self, int index, void** out);
extern "C" u8* func_80489A60(u8* global, u8* handle, int a, int b, int c, int d);
extern "C" void func_800BBADC(cf::CfResTboxParent* parent, u8* handle);
extern "C" u8* func_800584B8(u32 global, u32 id, const char* name);
extern "C" int CfRes_getD80Flag();
extern "C" void func_800BC3B0(cf::CfObjectMove* player, float value);
