#pragma once

#include <types.h>

namespace cf {

// Retail __vt__Q22cf5CHelp (US/EU symbols.txt: lbl_eu_8053B3A0, size 0x20).
// The object lives in retail .data, not in CHelp.cpp (that TU has no .data).
struct CHelpVtbl {
    void* mSlots[8]; // +0x00..+0x1C
};

// owner@0, param@4. CHelp's C++ vptr follows at +8.
struct CHelpPrefix {
    void* mOwner; // 0x0
    u32 mParam; // 0x4
};

// novtable: do not emit __vt__ from this TU. The ctor writes the symbols.txt
// label (lbl_eu_8053B3A0) at +8, same pattern as CToken / CHelpManager.
class __declspec(novtable) CHelp : public CHelpPrefix {
public:
    virtual void CHelp_UnkVirtualFunc1(); // vtable 0x08
    virtual void CHelp_UnkVirtualFunc2(); // vtable 0x0C
    virtual UNKWORD f10(); // vtable 0x10 (null in the base table)
    virtual UNKWORD CHelp_UnkVirtualFunc4(); // vtable 0x14
    virtual UNKWORD CHelp_UnkVirtualFunc5(); // vtable 0x18
    // No +0x1C on CHelp: that slot belongs to CHelpSwitch. Direct leaves
    // (Target, Sp, ArtsSet) stop at +0x18.

    CHelp(void* owner, u32 param);
    void func_802B7C68();

    // Overlay on the vptr at +8 so CHelpManager can swap retail tables.
    CHelpVtbl*& vtbl() {
        return *reinterpret_cast<CHelpVtbl**>(reinterpret_cast<u8*>(this) + 8);
    }
};

class __declspec(novtable) CHelpSwitch : public CHelp {
public:
    // Non-virtual: a new virtual here would append, not override Unk1.
    // Retail leaf tables still store this at +0x08; we do not emit them.
    void func_802B7CB0();

    virtual u32 func_802B7CBC(u32 flag); // vtable 0x1C (Fv linker name)
    virtual u32 func_802B7CE4(u8 flag); // vtable 0x20 (Fv linker name)

    u8 mFlag; // 0xC
};

} // namespace cf

// C-linkage imports - retail symbols are unmangled (free functions), so no
// C++ mangled declaration exists; C linkage is the repo-wide pattern for
// these (see include/functions.hpp, CHelp_ArtsSet.hpp). Signatures match
// the retail call sites verbatim.
extern "C" void func_80134D18(u32 param0, UNKWORD param1, UNKWORD param2);
extern "C" void func_8009D018(u32, u32);
extern "C" void* func_8013DB6C(int, u32, s32, s32);
extern "C" void func_8029A658();
// US symbols.txt name for __vt__Q22cf5CHelp. Plain extern (not mangled).
extern cf::CHelpVtbl lbl_eu_8053B3A0;
