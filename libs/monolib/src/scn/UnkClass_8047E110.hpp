#pragma once

#include <types.h>
#include <monolib/math/CVec3.hpp>

// Walk-graph node pool entry (stride 0x10).
struct ScnWalkNode {
    union {
        u32 reserved; // reinterpreted as f32 by func_8047F658
        f32 xPos;
    };
    u32 edgeOffset;
    u16 reserved8;
    u8 width;
    u8 depth;
    s16 x;
    s16 z;
};

// Walk graph header stored at the manager base.
struct ScnWalkGraph {
    u32 reserved;
    ScnWalkNode* nodes;
    u8 reserved8[12];
    u16* edges;
};

// 12-byte word triple copied verbatim as raw words.
struct ScnVecWords {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
};

// 12-byte reference block / pointer-to-member slot.
struct ScnPtmf {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
};

// Spawn descriptor consumed by func_8047E110 (offsets are rebased against
// the descriptor address itself).
struct WalkSpawnDesc {
    u32 field_0x0;
    u32 offNodes;      // 0x04 relative pointer
    u32 field_0x8;
    u32 offEdges;      // 0x0C relative pointer
    u32 field_0x10;
    u32 offField14;    // 0x14 relative pointer
    u32 field_0x18;
    u32 offField1C;    // 0x1C relative pointer
    u32 field_0x20;
    u32 offField24;    // 0x24 relative pointer
    u32 offRef;        // 0x28 relative pointer to a DescRef block
    f32 field_0x2C;
    f32 field_0x30;
};

// Block referenced by WalkSpawnDesc::offRef.
struct WalkDescRef {
    u8 field_0x0[0x18];
    u32 relField18; // relative pointer
    u32 relField1C; // relative pointer
};

// Per-node distance info used by func_80480140 (overlays 4-byte ScnWpKey).
struct WalkDistInfo {
    u16 baseIndex;
    u16 limit;
};

// kd-tree node (stride 8) used by func_8047E1B0; cell -> edge-list lookup.
struct ScnKdNode {
    u16 edgeBase;
    u16 slotA;
    u16 mask;   // bit per cell: 0 = internal (descend), 1 = leaf
    u8 slotB;
    u8 dims;    // high nibble = x cells, low nibble = z cells
};

// quantized waypoint block (stride 12); the halfwords at +8/+A are read as
// floats through GQR5 dequantization.
struct ScnWpQ {
    u16 pad[4];
    s16 q0; // +8
    s16 q1; // +A
};

// stride-10 per-node record used by func_8047EFBC / func_80480F48.
// (+4/+6) select the record by node id; (+0/+2) hold each endpoint's paired id.
struct ScnRec10 {
    u16 node[2];   // paired ids for the two endpoints (+0/+2)
    u16 coord[2];  // endpoint ids (+4/+6)
    u8 grp[2];
};

// edge-list block (stride 12) referenced by ScnRec10 coordinates.
struct ScnBlockC {
    u8 pad[4];
    u32 edgeBase;
};

// Fe48Table extended view (+0x14 range-limit word read by func_8047EFBC).
struct Fe48TableExt {
    u8 pad[0x14];
    u32 field_0x14;
};

// query record consumed by func_80481074.
struct WalkHitQuery {
    f32 field_0x0;
    u8 pad[6];
    u8 gxCell; // 0xA
    u8 gzCell; // 0xB
    s16 gridX; // 0xC
    s16 gridZ; // 0xE
};

class UnkClass_8047E110 {
public:
    u32 field_0x0;
    ScnWalkNode* nodes;   // 0x04 node pool
    u32 field_0x8;
    u32 field_0xC;
    u32 field_0x10;
    u16* edges;           // 0x14 edge pool
    f32 field_0x18;       // 0x18
    f32 field_0x1C;
    f32 field_0x20;
    f32 field_0x24;
    f32 field_0x28;
    u32 field_0x2C;
    u32 field_0x30;
    u32 field_0x34;
    u32 field_0x38;       // flags (bit 0 = walk box active)
    u32 field_0x3C;
    u32 field_0x40;
    u32 field_0x44;
    u8 field_0x48[0x48];  // 0x48..0x8F
    u16 field_0x90;
    u8 field_0x92[0xA];
    ScnPtmf callback; // 0x9C null pointer-to-member block

    void func_8047E110();
    void func_8047E1B0();
    void func_8047E390();
    void func_8047E62C();
    void func_8047E6C4();
    void func_8047EAD4();
    void func_8047EEB0();
    void func_8047EFBC();
    void func_8047F214();
    void func_8047F658();
    void func_8047F730();
    void func_8047FE48();
    void func_8047FF9C();
    void func_80480140();
    void func_8048020C();
    void func_804804DC();
    void func_804808A0();
    void func_80480EF0();
    void func_80480F48();
    void func_80481014();
    void func_80481074();
    void func_804812D8();
    void func_804813E8();
    void func_804814DC();
    void func_8048163C();
    void func_8048169C();
    void func_80481790();
    void func_804819AC();
    void func_804819C4();
};

// quantized walk-table key record (indexed by coordinate selector): covers
// node ids [base, base+range) for the BFS distance map.
struct ScnWpKey {
    u16 base;
    u16 range;
};

// walker step callback invoked through the ptmf slot at 0x9C
// (retail lowers calls through it to __ptmf_scall)
typedef bool (UnkClass_8047E110::*ScnWalkFn)(const ScnWalkNode*);

namespace nw4r {
namespace db {
void Warning(const char* file, int line, const char* fmt, ...);
}
} // namespace nw4r

// Query descriptor consumed by func_8047EEB0.
struct WalkQueryInfo {
    u32 field_0x0;
    u32 field_0x4;   // edge-list index
    u16 field_0x8;   // status flags
};

// Foreign data table read by func_8047FE48 (+0x8 = waypoint table pointer).
struct Fe48Table {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
};

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------

// Walk-graph float constants (retail .rodata pool)
extern "C" const f32 lbl_eu_8066A898;
extern "C" const f32 lbl_eu_8066A894;
extern "C" const f32 lbl_eu_8066A8A8;
extern "C" const f32 lbl_eu_8066A8AC;
extern "C" const f32 lbl_eu_8066A8B8;
extern "C" u32 lbl_eu_8056DC80[4]; // ptmf reference block copied into the 0x9C slot
