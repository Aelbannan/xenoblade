#pragma once

/**
 * @file CREvtModel.hpp
 * CREvtModel class definition.
 *
 * CREvtModel is the base class for realtime event model objects.
 * It inherits from cf::CREvtObj and manages model data, animations,
 * and rendering for realtime event sequences.
 */

#include <types.h>
#include "kyoshin/realtimeevt/CREvtObj.hpp"

class CScnItemModel;

class CREvtModel : public cf::CREvtObj {
public:
    CREvtModel(void* pData, void* pArg);
    /* vtable 0x00 */ virtual ~CREvtModel();
    /* vtable 0x04 */ virtual void vfunc_04();           // called from func_801726DC
    /* vtable 0x08 */ virtual void vfunc_08();
    /* vtable 0x0C */ virtual void vfunc_0C();
    /* vtable 0x10 */ virtual void vfunc_10();           // cleanup virtual
    /* vtable 0x14 */ virtual void vfunc_14();
    /* vtable 0x18 */ virtual void vfunc_18();
    /* vtable 0x1C */ virtual void vfunc_1C();
    /* vtable 0x20 */ virtual void vfunc_20();
    /* vtable 0x24 */ virtual void vfunc_24();
    /* vtable 0x28 */ virtual void vfunc_28();
    /* vtable 0x2C */ virtual void vfunc_2C();
    /* vtable 0x30 */ virtual void vfunc_30(int r4);     // called from func_801727DC
    /* vtable 0x34 */ virtual void vfunc_34();
    /* vtable 0x38 */ virtual void vfunc_38(int r4);     // called from func_801729F0
    /* vtable 0x3C */ virtual void vfunc_3C();           // cleanup for destructor

    // Non-virtual methods
    int func_801726DC();
    void func_80172768();
    void* func_801727D0();
    void func_801727DC();
    void func_801728F8();
    void func_8017298C();
    int func_801729D0();
    void func_801729F0(void* pData, void* pModelData);
    void func_80172CC0();
    void func_80172CC4();
    void func_80172CE4(CScnItemModel* pModel);
    void func_80172EA4(void* pData, CScnItemModel* pModel, int flag);
    void* func_801730D0();
    int func_80173194();
    int func_801731A0();
    int func_801731AC();

    // Fields (CREvtObj base: 0x00-0x13)
    /* 0x14 */ u32 mField14;
    /* 0x18 */ u32 mFlags;
    /* 0x1C */ void* mPtr1C;
    /* 0x20 */ CScnItemModel* mModel;  // model pointer
    /* 0x24 */ void* mData;             // animation data
    /* 0x28 */ void* mPtr28;
    /* 0x2C */ int mField2C;
    /* 0x30 */ int mField30;
    /* 0x34 */ int mField34;            // animation index counter
};