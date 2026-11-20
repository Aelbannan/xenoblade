#pragma once

#include <types.h>
#include <revolution/MTX.h>
#include <nw4r/math/math_types.h>

// Data owned by libs/monolib/src/coli/code_804B59C8.cpp.  The retail symbol
// sColiWorkArea covers 0x2180 bytes: the 0x2000-byte mover workspace, the
// seen-bit table at +0x2000, the query manager at +0x2028 and the 16-entry
// registered-index list at +0x2160.  The first two views alias the same bytes
// (the bit table overlaps the manager), so they are expressed as members of
// one union.  // name-guess: POD workspace behind the exported sColiWorkArea
// symbol; no RTTI or type name survives in the retail binary.
struct CColiMover;

struct CColiMgr {
    nw4r::math::_VEC3 field_0x00;    // 0x00 - ray offset
    nw4r::math::_VEC3 field_0x0C;    // 0x0C - ray direction
    nw4r::math::_VEC3 field_0x18;    // 0x18 - reference position
    nw4r::math::_VEC3 field_0x24;    // 0x24 - AABB min corner
    nw4r::math::_VEC3 field_0x30;    // 0x30 - AABB max corner
    nw4r::math::_VEC3 field_0x3C[6]; // 0x3C - six OBB plane normals
    f32 field_0x84[6];               // 0x84 - six OBB plane offsets
    int field_0x9C;                  // 0x9C - vector count for the array walk
    void* field_0xA0;
    nw4r::math::_VEC3 field_0xA4[3]; // 0xA4 - per-vertex sampled points
    nw4r::math::_VEC3 field_0xC8;    // 0xC8 - sampled normal
    f32 field_0xD4;                  // 0xD4 - ray t
    f32 field_0xD8;                  // 0xD8 - distance threshold
    f32 field_0xDC;                  // 0xDC - last registered distance
    CColiMover* field_0xE0;          // 0xE0 - mover hook
    u8 field_0xE4[0xE8 - 0xE4];      // 0xE4
    Mtx* field_0xE8;                 // 0xE8 - matrix source pointer
    u32 field_0xEC;                  // 0xEC
    Mtx field_0xF0;                  // 0xF0 - embedded matrix
    f32 field_0x120;                 // 0x120 - acceptance radius
    f32 field_0x124;                 // 0x124 - squared acceptance radius
    u32 field_0x128;                 // 0x128 - registration filter flags
    u32 field_0x12C;                 // 0x12C - second registration filter flags
    u32 field_0x130;                 // 0x130
    u8 field_0x134;                  // 0x134
    u16 field_0x136;                 // 0x136 - registered-index count
};

struct CColiListMgr {
    u8 field_0x00[0x136];
    u16 count;
    u16 list[0x10];
};

struct CColiWorkArea {
    union {
        struct {
            u8 field_0x0000[0x2000];
            u32 bitTable[0x60]; // 0x2000; retail walkers index up to +0x7F
        } b;
        struct {
            u8 field_0x0000[0x2028];
            CColiListMgr listMgr; // 0x2028
        } l;
    };
};

extern "C" CColiWorkArea sColiWorkArea;
