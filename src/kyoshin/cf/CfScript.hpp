#pragma once

#include <types.h>

// Retail data labels (.data/.sdata) used by the cf script manager.  Declared
// at global scope so MWCC keeps the retail names unmangled (no extern "C"
// needed - global-scope data names are not mangled).
extern char lbl_eu_804FB3A4[];   // .data:0x804FB3A4 script-name / extension string table
extern char lbl_eu_805708D0[];   // .data:0x805708D0 current-dir path buffer
extern char lbl_eu_80661AD0[2];  // .sdata:0x80661AD0 extension separator "."
extern char* lbl_eu_80661AC0;    // .sdata:0x80661AC0 current-dir string pointer
extern char lbl_eu_80570918[];   // .bss  manager instance storage
extern s8 lbl_eu_80663D88;       // .sdata singleton-init flag

// Retail CfScript vtable (.data:0x80526DE8, port/data_defs.cpp).  The class is
// __declspec(novtable), so the ctor stores this label explicitly instead of
// the compiler-generated __vt__Q22cf8CfScript (which would add .data/.rodata/
// RTTI to a retail-empty TU and drift the reloc name).
extern void* lbl_eu_80526DE8[];

// C-linkage runtime imports (retail symbol names - keep linkage/signatures verbatim).
extern "C" {
    void vmExec();
    void vmUnlink(void* ctx);
    void pluginRegist__Fv();
    void* func_800A82BC();
    void* func_800A837C();
    void* func_800A843C();
    void* CfRes_readCommonArchive(void* fileHandle, const char* path, void* callback);
    int getFileSize__11CDeviceFileFPCc(const char* path, int flags);
    void cancel__11CDeviceFileFP11CFileHandle(void* fileHandle);
}

namespace cf {

// Forward declarations
class CfScriptManager;

// Minimal CEventFile layout - only the fields CfScript::OnFileEvent consumes.
// CDeviceFile hands a filled CEventFile to the async IWorkEvent callback.
class CEventFile {
public:
    s32 field_00;          // 0x00 event type
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
class __declspec(novtable) CfScript {
    friend class CfScriptManager;
public:
    // 0x00: vtable pointer - assigned explicitly from lbl_eu_80526DE8 in the
    // ctor (novtable: MWCC would otherwise emit __vt__Q22cf8CfScript relocs).
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

    // Dispatched through the retail ptmf table lbl_eu_80526DD0 while the
    // slot is loading (mWaitCount 0/1).
    void waitLoad();
    void update();
};

// CfScriptManager - manages an array of up to 3 CfScript objects.
// The array is embedded at offset 0. Singleton accessed via getInstance().
class CfScriptManager {
public:
    static __declspec(noinline) CfScriptManager* getInstance();
    void init();

    CfScript mScripts[3]; // 0x00, 0x58, 0xB0 (total 0x108)

    // Script loading thunks for each slot.
    // noinline: these are retail `bl`-thunks (forward this+name to func_80068ECC);
    // prevents MWCC -inline auto from inlining them into the wrappers (REF §8616).
    void __declspec(noinline) loadSlotZero(const char* name);  // load slot 0 (this == &mScripts[0])
    void func_80068B20(const char* n) { loadSlotZero(n); } // compat
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

    // Main update and exec
    void __declspec(noinline) updateScript();
    void resetScripts();
    void func_800694B0() { updateScript(); } // compat
    void func_8006953C() { resetScripts(); } // compat
};

} // namespace cf

// PMTF dispatch table (2 x 12-byte member pointers) selected by mWaitCount in
// CfScriptManager::func_800694B0.  Declared at global scope so MWCC keeps the
// retail name unmangled; the (script.*table[idx])() call lowers to
// lis/addi + mulli + `bl __ptmf_scall` against the retail helper.
// Retail OnFileEvent is a GLOBAL-scope CfScript member (unmangled symbol
// OnFileEvent__8CfScriptFP10CEventFile); our CfScript sits in namespace cf,
// so emit it under the pre-mangled retail name with an explicit self
// (same convention as CUICfManager/CSortMenu).
extern "C" bool OnFileEvent__8CfScriptFP10CEventFile(cf::CfScript* self, cf::CEventFile* event);

typedef void (cf::CfScript::*CfScriptPMF)();
extern CfScriptPMF lbl_eu_80526DD0[2];

// Unmangled retail symbols implemented in this TU (C linkage so the emitted
// symbol names match the retail split verbatim).
extern "C" {
    bool func_80068ECC(cf::CfScript* script, const char* name);
    u32 func_80068E7C(cf::CfScriptManager* mgr, int index, int mask);
}
