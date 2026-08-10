#pragma once

#include <types.h>

namespace cf {

// Forward declarations
class CfScriptManager;

// Minimal CEventFile layout - only the fields CfScript::OnFileEvent consumes.
// CDeviceFile hands a filled CEventFile to the async IWorkEvent callback.
class CEventFile {
public:
    u32 field_00;          // 0x00 event type
    void* field_04;        // 0x04 owning CFileHandle*
    u8 _08[0x0C - 0x08];
    const char* field_0C;  // 0x0C path string
    u8 _10[0x14 - 0x10];
    u32 field_14;          // 0x14 flag
};

// FixStr<64>-compatible extension buffer WITHOUT a constructor.  Retail does
// not zero it before getNoPathExtName fills it (a FixStr ctor would emit extra
// stb/stw init stores in the prologue).
struct CfScriptNameBuffer {
    char mString[0x40];   // 0x00
    u32 mLength;          // 0x40
};

// CfScript - per-script state, 0x58 bytes each.
class CfScript {
    friend class CfScriptManager;
public:
    // 0x00: vtable pointer
    CfScript();
    virtual ~CfScript();

    // 0x04: u8 name buffer (inline, up to 0x40 bytes)
    char mName[0x40];      // 0x04 - 0x43

    // 0x44: name length
    u32 mNameLen;          // 0x44

    // 0x48: file handle for pending load
    void* mFileHandle;     // 0x48 (CFileHandle*)

    // 0x4C: flags
    u32 mFlags;            // 0x4C

    // 0x50: VM context/thread
    void* mVmContext;      // 0x50

    // 0x54: wait counter (s16)
    s16 mWaitCount;        // 0x54

    // 0x56: script index (u16)
    u16 mIndex;            // 0x56

    void waitLoad();
    bool OnFileEvent(CEventFile* event);
    void update();

    // Reset this script slot
    void reset();
};

// CfScriptManager - manages an array of up to 3 CfScript objects.
// The array is embedded at offset 0. Singleton accessed via getInstance().
class CfScriptManager {
public:
    static __declspec(noinline) CfScriptManager* getInstance();
    void init();

    CfScript mScripts[3]; // 0x00, 0x58, 0xB0 (total 0x108)

    // Script loading functions for each slot
    // noinline: these are retail `bl`-thunks (forward this+name to func_80068ECC);
    // prevents MWCC -inline auto from inlining them into the wrappers (REF §8616).
    void __declspec(noinline) func_80068B20(const char* name);  // load slot 0 (this == &mScripts[0])
    void func_80068B58(const char* name); // load slot 1 (offset 0x58)
    void __declspec(noinline) func_80068B94(const char* name);  // load slot 2 (offset 0xB0)

    // Set "ready" flag on each slot
    void func_80068BC0();  // set flag on slot 0
    void func_80068BF4();  // set flag on slot 1
    void func_80068C28();  // set flag on slot 2

    // Sleep VM thread for slot 2 if flag set
    void func_80068C5C();

    // Reset/cleanup functions for each slot
    void func_80068CA0();  // reset slot 0
    void func_80068D38();  // reset slot 1
    void func_80068DD0();  // reset slot 2

    // Check script state
    void func_80068E44();
    void func_80068E7C();
    void func_80068E9C();

    // Main update and exec
    void func_800694B0();
    void func_8006953C();
};

} // namespace cf
