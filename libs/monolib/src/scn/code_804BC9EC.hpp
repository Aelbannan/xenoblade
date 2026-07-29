#pragma once

#include <harness_catalog.h>

// Resource data parsed by func_804BC9F4 from chunked file data.
// Fields are populated based on entry type in the top-level chunk list.
// Offsets 0x60 and 0x78 are used by func_804B80A4 / func_804B8078.
struct ScnResData {
    void* field_0x00;  // set by func_804BC9B4 (base + src[1])
    void* field_0x04;  // set by func_804BC9B4 (base + src[2])
    void* field_0x08;  // set by func_804BC9B4 (base + src[3])
    u8 field_0x0C[0x14 - 0x0C];
    void* field_0x14;  // base pointer to parsed data
    void* field_0x18;  // entry type 1
    void* field_0x1C;  // entry type 3
    void* field_0x20;  // entry type 2
    void* field_0x24;  // entry type 5
    void* field_0x28;  // entry type 4
    void* field_0x2C;  // entry type 0: data + 0x20
    void* field_0x30;  // entry type 0: data + 0x20 (same as 0x2C)
    void* field_0x34;  // entry type 0: data pointer
    void* field_0x38;  // entry type 6
    void* field_0x3C;  // entry type 7
    void* field_0x40;  // entry type 8
    u8 field_0x44[0x48 - 0x44];
    void* field_0x48;  // entry type 9
    void* field_0x4C;  // entry type 12
    u8 field_0x50[0x60 - 0x50];
    void* field_0x60;  // array of 0xE0-byte elements
    u8 field_0x64[0x6C - 0x64];
    u32 field_0x6C;    // entry type 0: u16 count
    u32 field_0x70;    // entry type 7: u16 value
    u8 field_0x74[0x78 - 0x74];
    int count;         // 0x78
};

extern ScnResData lbl_eu_8065F32C;

// Entry in the top-level chunk list (8 bytes each)
struct ScnResEntry {
    u16 type;
    u16 field_0x2;
    u32 offset;  // relative to data base
};

extern "C" {
ScnResData* func_804B80A4(ScnResData* self, int index);
void func_804B8078(ScnResData* self, int index);
void func_804B80CC(ScnResData* self);
void func_804B7804(ScnResData* self);
void func_804BC9A0(ScnResData*);
void func_804B7D9C(int, int);
void func_804B7DD4(ScnResData*);
void func_804BC9B4(int* dest, int baseOffset, int* src);
void func_804B74F0(ScnResData* res, void* data);
void func_804B7540(ScnResData* res, void* data, u16 param);
}
