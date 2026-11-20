#pragma once

// Retail interface at the +4 base of CModelDispMakeCrystal / CModelDispEquip /
// CModelDisp. Its RTTI name string lives in CfGameManager.rodata and the weak
// RTTI locator (__RTTI__14IActParamEvent) in CtrlEnemy.sdata. CModelDisp's
// base list names it and its DESTROYBASE record names the 0x40 destructor
// mapped as __dt__80080400. The eight virtuals are the actParamNop slots in
// retail vtable order.
//
// Single inline-empty destructor: MWCC elides the base-dtor call in derived
// destructors. __declspec(novtable) matches the interface: no retail standalone
// vtable for IActParamEvent exists, only its derived classes' compiler-emitted
// tables.
class __declspec(novtable) IActParamEvent {
public:
    virtual ~IActParamEvent() {}
    virtual void actParamNop0() = 0;
    virtual void actParamNop1() = 0;
    virtual void actParamNop2() = 0;
    virtual void actParamNop3() = 0;
    virtual void actParamNop4() = 0;
    virtual void actParamNop7() = 0;
    virtual void actParamNop5() = 0;
    virtual void actParamNop6() = 0;
};
