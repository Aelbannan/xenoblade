#pragma once

#include <types.h>

// ---------------------------------------------------------------------------
// C-linkage imports
// ---------------------------------------------------------------------------

struct GlobalStruct_80572B94;

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
extern "C" void __dt__800A34E0();
