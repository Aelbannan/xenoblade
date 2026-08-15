#pragma once

#include <types.h>
#include <nw4r/g3d/res/g3d_resfile.h>

// Retail vtable for CScnItemAnim (unmapped .data label). Declared as an array
// so MWCC emits the full lis/addi pair with the exact retail reloc name; the
// object vptr is stored manually (novtable).
extern char lbl_eu_8056EC90[];

// Panic file/format strings used by the alignment and chr-anm asserts (retail
// linker names; global-scope names are not mangled by MWCC). The sdata2 arg
// string is sized so MWCC emits sda21 addressing, matching the retail call
// sites.
extern char lbl_eu_8052637C[];          // file (ResFile alignment assert, line 0x3c)
extern char lbl_eu_80526354[];          // fmt
extern const char lbl_eu_8056E9D0[];    // file (chr-anm data assert, line 0x27)
extern const char lbl_eu_8056E9B4[];    // fmt
extern const char lbl_eu_8056E9A8[];    // arg
extern const char lbl_eu_80663A88[4];   // arg (sda2 string)

// Host scene object for the item-anim factory ctor (retail placeholder symbol
// __ct__8049E710): scene-item pool at +0x60.
struct CScnItemAnimHost {
    u8 _00[0x60];           // +0x00
    void* mPool;            // +0x60
};

/**
 * CScnItemAnim - Scene animation item (size: 0x58)
 *
 * Manages a character animation resource within the scene graph.
 * Stores an nw4r::g3d::ResFile reference and the name of the first
 * animation track found in that resource.
 *
 * Vtable: lbl_eu_8056EC90 (0x20 bytes, 8 entries)
 */
class __declspec(novtable) CScnItemAnim {
public:
    virtual ~CScnItemAnim();

    // 0x00: vtable pointer (lbl_eu_8056EC90, stored manually)
    CScnItemAnimHost* mParent;          // 0x04 - parent/owner host
    u16 mType;                          // 0x08 = 2
    u8 pad_0A[2];                       // 0x0A - padding

    nw4r::g3d::ResFile mResFile;        // 0x0C - embedded ResFile handle (4 bytes)
    char mName[0x40];                   // 0x10 - animation name buffer
    u32 mNameLen;                       // 0x50 - strlen of mName
    nw4r::g3d::ResFile mResFileCopy;    // 0x54 - copy of the ResFile handle
}; // size: 0x58

// Scene-item pool helpers (retail reloc names are unmangled short forms, so
// C-linkage declarations reproduce them; the retail map resolves the symbols).
extern "C" void func_8048CBC0(void* self, void* arg);
extern "C" CScnItemAnim* func_8048C400(void* self);
extern "C" u32 func_8048C630(void* pool, void* item, u32 flag);

// Runtime throw helper (NMWException.h is not included: it drags in
// __ppc_eabi_linker.h which conflicts with __ppc_eabi_init.h's _stack_addr).
// noreturn: MWCC elides the __end__catch epilogue of a catch handler that
// ends in a terminal call (retail catch-all handlers end with `bl __throw`).
extern "C" __declspec(noreturn) void __throw(char* throwtype, void* location,
                                             void* dtor);
