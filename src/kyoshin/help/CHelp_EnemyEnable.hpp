#pragma once

#include <types.h>
#include "kyoshin/help/CHelp.hpp"

namespace cf {
// size: 0xC (no extra members beyond CHelp).
class __declspec(novtable) CHelp_EnemyEnable : public CHelp {
public:
    bool areAllEnemiesEnabled();
};

// Virtual-dispatch shim: the object returned by func_800AD860 has a sub-object
// with its own vtable at +0x3E9C, and areAllEnemiesEnabled calls that sub-object's
// vtable slot 0x74. Declaring the slot as a real virtual method makes MWCC emit
// the r12 virtual-call sequence used by retail (a raw slot cast would use r4).
// This class is never instantiated, so none of these need out-of-line
// definitions and no vtable is emitted.
class CHelp_EnemyEnableSub {
public:
    virtual void f00(); // 0x00
    virtual void f04(); // 0x04
    virtual void f08(); // 0x08
    virtual void f0c(); // 0x0C
    virtual void f10(); // 0x10
    virtual void f14(); // 0x14
    virtual void f18(); // 0x18
    virtual void f1c(); // 0x1C
    virtual void f20(); // 0x20
    virtual void f24(); // 0x24
    virtual void f28(); // 0x28
    virtual void f2c(); // 0x2C
    virtual void f30(); // 0x30
    virtual void f34(); // 0x34
    virtual void f38(); // 0x38
    virtual void f3c(); // 0x3C
    virtual void f40(); // 0x40
    virtual void f44(); // 0x44
    virtual void f48(); // 0x48
    virtual void f4c(); // 0x4C
    virtual void f50(); // 0x50
    virtual void f54(); // 0x54
    virtual void f58(); // 0x58
    virtual void f5c(); // 0x5C
    virtual void f60(); // 0x60
    virtual void f64(); // 0x64
    virtual void f68(); // 0x68
    virtual bool func74(); // 0x74 <- the slot this TU calls
};

// View of the object returned by func_800AD860 with the sub-object at +0x3E9C.
struct CHelp_EnemyEnableObj {
    u8 pad[0x3E9C];
    CHelp_EnemyEnableSub mSub; // +0x3E9C
};
}

