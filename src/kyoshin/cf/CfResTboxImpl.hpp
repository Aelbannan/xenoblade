#pragma once

#include <types.h>
#include "kyoshin/cf/CfGameManagerData.hpp"
#include "kyoshin/cf/object/CfObject.hpp"
#include "kyoshin/cf/object/CfObjectModel.hpp"

#include "kyoshin/cf/CfResObjImpl.hpp"

namespace cf {

// Real class tree for cf::CfResTboxImpl (retail vtable lbl_eu_80535204).
// Same 0x1C layout as cf::CfResObjImpl (shared prefix: parent at +0x00, vptr
// at +0x10), and its ctor runs __ct__cf_CfResObjImpl first, but it is NOT
// declared as a C++ subclass: retail ~CfResTboxImpl does not chain to
// ~CfResObjImpl (deriving adds the base-dtor call and breaks the match).
class __declspec(novtable) CfResTboxImpl : public CfResObjImplPrefix {
public:
    virtual ~CfResTboxImpl();                // 0x08
    virtual void ResTbox_CacheNullPtmfOnce();            // 0x0C
    virtual void CfResObj_noop10();            // 0x10
    virtual int ResObj_IsInUse_C860();             // 0x14
    virtual void CfObjectMove_relaySubB0Slot14();            // 0x18
    virtual void ResObj_Noop_CD64();            // 0x1C
    virtual int ResTbox_GetFixedSize256();             // 0x20
    virtual void CfResObj_noop24();            // 0x24
    virtual void CfResObj_noop28();            // 0x28
    virtual void CfResObj_noop2C();         // 0x2C
    virtual void ResObj_SetWork_CD68(int idx, int value); // 0x30
    virtual u32 ResObj_GetWork_CCBC(int idx);      // 0x34
    virtual void CfResObj_false38();            // 0x38
    virtual void CfResObj_unk3C();            // 0x3C
    virtual void CfResObj_noop40();            // 0x40
    virtual void CfResObj_noop44();            // 0x44
    virtual void CfResObj_unk48();            // 0x48
    virtual void CfResObj_unk4C();            // 0x4C
    virtual void ResObj_ZeroStubB_CD5C();            // 0x50
    virtual void ResObj_NotifySound_C888(int a, int b, int c, float f1, float f2); // 0x54
    virtual void CfResObj_noop58();            // 0x58
    virtual void CfResObj_unk5C();            // 0x5C
    virtual int ResTbox_NegInUseState();             // 0x60
    virtual int ResObj_ZeroStubA_CD54();             // 0x64
    virtual int CfResObj_true68();             // 0x68

    CfResObjImplVtbl*& vtbl() {
        return *reinterpret_cast<CfResObjImplVtbl**>(reinterpret_cast<u8*>(this) + 0x10);
    }

    /* 0x14 */ u32 field_14[2];
};

} // namespace cf

typedef void (cf::CfResTboxImpl::*CfResTboxImplPMF)();
extern CfPmf3 lbl_eu_805351E0[3];
extern char lbl_eu_80664660[8];
extern cf::CfResObjImplVtbl lbl_eu_80535204;
extern float lbl_eu_806681D0;
extern float lbl_eu_806681D4;
extern "C" void ResTbox_SetCellBit40(u8* base, int idx1, int idx2, int idx3);
extern "C" bool isSceneActive__Q22cf13CfGameManagerFv();
extern "C" char* CfRes_getInstPtr29C();
extern "C" u8* CfRes_lookupStrTable(char* key, int index, u32** out);
extern "C" void CfObjectMove_setMoveSpeedGated(cf::CfObjectMove* player, float value);
