#pragma once

#include <harness_catalog.h>

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// Forward-declared instead of including NMWException.h (which conflicts with
// __ppc_eabi_init.h's _stack_addr declaration).
extern "C" void* __register_global_object(void* object, void* destructor, void* registration);
extern "C" void __dt__804BD8E8();

// Minimal layout for the global data at lbl_eu_8065F32C.
// Field offsets 0x60 and 0x78 are used by func_804B80A4 / func_804B8078.
struct ScnResData {
    u8 field_0x00[0x60];
    u8* field_0x60; // array of 0xE0-byte elements
    u8 field_0x64[0x78 - 0x64];
    int count; // 0x78
};

extern ScnResData lbl_eu_8065F32C;

extern "C" {
ScnResData* func_804B80A4(ScnResData* self, int index);
void func_804B8078(ScnResData* self, int index);
void func_804B80CC(ScnResData* self);
void func_804B7804(ScnResData* self);
void func_804BC9A0(ScnResData*);
void func_804B7D9C(int, int);
void func_804B7DD4(ScnResData*);
}
