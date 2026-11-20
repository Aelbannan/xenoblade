#pragma once

#include <types.h>
#include <nw4r/g3d/res/g3d_resmat.h>

// Node user-data key defined by CMdlMaterial.cpp.
extern const char* sMatCullModeKey;

/**
 * CMdlMaterial - model material descriptor.
 *
 * Layout derived from constructor (__ct__CMdlMaterial at 0x804E98E0)
 * and vtable at CMdlMaterialVtable (single virtual: destructor).
 * Size: 0x38 bytes (inferred from spacing before CMdlMouth at 0x1700
 * vs CMdlMaterial at 0x16C8 in CScnItemModelNw4r).
 */
class __declspec(novtable) CMdlMaterial {
public:
    virtual ~CMdlMaterial();

    /* 0x04 */ void* field_0x04;    // Pointer to owner/context object (opaque)
    /* 0x08 */ void* buffer;        // GXColor array (written from scnImN4PoolAlloc/allocate_array void*)
    /* 0x0C */ u32 field_0x0C;      // Write index for GXColor buffer
    /* 0x10 */ u8 flag_0x10;        // Controls buffer ownership: 0 = owned (delete[]), non-0 = unowned
    /* 0x11 */ u8 pad_0x11[3];
    /* 0x14 */ u32 field_0x14;      // Initialized to -1
    /* 0x18 */ u16 field_0x18[8];   // Accumulated material values
    /* 0x28 */ u8 field_0x28[8];    // Per-material flag bytes
    /* 0x30 */ u32 field_0x30;
    /* 0x34 */ s32 field_0x34;      // Write index for field_0x18 array
};
