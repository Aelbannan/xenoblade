#pragma once

#include <types.h>

namespace cf {

// Forward declarations
class CfScriptManager;

// CfScript - per-script state, 0x58 bytes each.
// Layout hints from disassembly:
//   0x00: vtable (4B)
//   0x04: u8 flag
//   0x44-0x53: data pointers / fields
//   0x54: u16 counter  0x56: u16 index
// TODO: replace placeholder with real members when this class is decompiled.
class CfScript {
    friend class CfScriptManager;
    u8 gap00[0x58];
public:
    // Currently a placeholder - actual fields TBD.
};

// CfScriptManager - manages an array of up to 3 CfScript objects.
// The array is embedded at offset 0. Singleton accessed via getInstance().
class CfScriptManager {
public:
    static CfScriptManager* getInstance();
    void init();

    CfScript mScripts[3]; // 0x00, 0x58, 0xB0
};

} // namespace cf
