#pragma once

#include <harness_catalog.h>

// Minimal layout for the global data at lbl_eu_8065F32C.
// Field offsets 0x60 and 0x78 are used by func_804B80A4 / func_804B8078.
struct ScnResData {
    u8 field_0x00[0x60];
    void* field_0x60; // array of 0xE0-byte elements
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
void func_804B7DD4(ScnResData*);
}
