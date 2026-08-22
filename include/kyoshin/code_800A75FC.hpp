#pragma once

#include <types.h>

namespace cf {
struct CtrlObjectParamArtsList;  // defined in src/kyoshin/cf/CtrlObjectParam.hpp
}

// ---------------------------------------------------------------------------
// C-linkage imports
// ---------------------------------------------------------------------------

struct GlobalStruct_80572B94;

// ---------------------------------------------------------------------------
// Retail linker labels referenced from this TU (unmangled global data names;
// plain extern at global scope -- MWCC does not mangle global variable names)
// ---------------------------------------------------------------------------
extern char lbl_eu_804FBF60[];
extern char lbl_eu_80572AC8[];
extern char lbl_eu_80572B78[];
extern char lbl_eu_80572B38[];
// Each entry points at a table of up to 14 pointers (indexed by the callers).
struct LblPtrTable {
    char* slots[14];
};
extern LblPtrTable* lbl_eu_805282A0[6];
extern LblPtrTable* lbl_eu_80528398[6];
extern char lbl_eu_80572B88[];   // .bss: global-object registration node
extern char lbl_eu_80572B58[];   // .bss
extern s8 lbl_eu_80663E98;       // .sbss: singleton-initialized flag

// External runtime / game functions (retail-unmangled symbols)
extern "C" void func_800A37CC(void*, void*);
extern "C" int func_800A3594(void*, int, int);
extern "C" int func_800A36A4(void*, int, int);
extern "C" u32 func_800A3998(void*);
extern "C" void func_800A76EC(GlobalStruct_80572B94*);
extern "C" int func_800A39E8(void*, void*, void*, void*);
extern "C" void func_800A3520(void*, int, int, int);
extern "C" void* __register_global_object(void* object, void* destructor, void* registration);
extern "C" void func_800A34C8(void*);
u32 func_800A3940(cf::CtrlObjectParamArtsList* list);
extern "C" void __dt__800A34E0();
