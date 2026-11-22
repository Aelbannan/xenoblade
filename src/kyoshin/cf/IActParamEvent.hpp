#pragma once

struct CModelDispParent;

// Retail interface at the +4 base of CModelDispMakeCrystal / CModelDispEquip /
// CModelDisp, and the primary base of the cf::CfObjectImpl* classes. Its RTTI
// name string lives in CfGameManager.rodata and the weak RTTI locator
// (__RTTI__14IActParamEvent) in CtrlEnemy.sdata. CModelDisp's base list names
// it and its DESTROYBASE record names the 0x40 weak destructor emitted in
// CfGameManager.o. The eight virtuals are listed in retail vtable order; the
// no-op default bodies (retail actParamNop0..7) are defined in
// CActParamData.cpp and appear unoverridden in the derived vtables
// (CModelDispMakeCrystal uses all eight).
//
// Single inline-empty destructor: MWCC elides the base-dtor call in derived
// destructors. __declspec(novtable) matches the interface: no retail standalone
// vtable for IActParamEvent exists, only its derived classes' compiler-emitted
// tables.
class __declspec(novtable) IActParamEvent {
public:
    virtual ~IActParamEvent() {}
    // name-guess: slot names/params are inferred from the overriders.
    // Slot 0 is overridden by CModelDisp (scan/rebind) and CModelDispEquip
    // (swap party equip), both taking (parent, kind, sub-index).
    virtual void actParamNop0(CModelDispParent* parent, int kind, int index);
    virtual void actParamNop1();
    virtual void actParamNop2();
    virtual void actParamNop3();
    virtual void actParamNop4();
    virtual void actParamNop7();
    virtual void actParamNop5();
    // Last slot is overridden by CModelDisp (dispatch chain) and
    // CModelDispEquip (replay slot anims), both taking (parent, anim arg).
    virtual void actParamNop6(CModelDispParent* parent, void* arg);
};
