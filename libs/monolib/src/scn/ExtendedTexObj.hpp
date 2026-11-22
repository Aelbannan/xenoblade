#pragma once

#include <types.h>
#include <revolution/gx.h>

// name-guess: ExtendedTexObj. 0x2C-byte texture-work object: allocated by
// TexMan_AllocBuffer_0208 (MemManager::allocate(0x2C)), initialised by
// TexObj_InitExt, released by TexMan_ReleaseArg_02D8 via its deleting
// destructor (defined in code_8049431C.cpp).
struct ExtendedTexObj {
    ~ExtendedTexObj();

    GXTexObj mTexObj; // +0x00 (GXInitTexObj target)
    u32 mField20;   // +0x20 (image pointer)
    u32 mField24;   // +0x24 (buffer size)
    u32 mField28;   // +0x28 (allocation cursor snapshot)
};
