// Translation unit: monolib/src/scn/UnkClass_8047E110
// Unknown scene class implementation.
//
// Status: Only sinit_80481E68 is FULL_MATCH (empty .ctors hook, 4 bytes).
//         Remaining 29 functions are NOT_STARTED stubs.

#include <monolib/scn/UnkClass_8047E110.hpp>
#include <monolib/math/CVec3.hpp>

using ml::CVec3;

struct ScnWalkNode {
    u32 reserved;
    u32 edgeOffset;
    u16 reserved8;
    u8 width;
    u8 depth;
    s16 x;
    s16 z;
};

struct ScnWalkGraph {
    u32 reserved;
    ScnWalkNode* nodes;
    u8 reserved8[12];
    u16* edges;
};

// 12-byte word triple: a vector/position copied verbatim as raw words.
struct ScnVecWords {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
};

// 12-byte reference block stored at 0x9C (pointer-to-member style triple).
struct ScnPtmf {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
};

// Manager field view (retail offsets) used by the accessor methods below.
struct ScnManagerLayout {
    u32 field_0x0;         // 0x00
    ScnWalkNode* nodes;    // 0x04 node pool
    u8 field_0x8[12];      // 0x08
    u16* edges;            // 0x14 edge pool
    f32 field_0x18;        // 0x18
    f32 field_0x1C;        // 0x1C
    f32 field_0x20;        // 0x20
    f32 field_0x24;        // 0x24
    f32 field_0x28;        // 0x28
    u32 field_0x2C;        // 0x2C
    u32 field_0x30;        // 0x30
    u32 field_0x34;        // 0x34
    u32 field_0x38;        // 0x38 flags (bit 0 = box active)
    u8 field_0x3C[12];     // 0x3C..0x47
    f32 field_0x48;        // 0x48 box minX
    f32 field_0x4C;        // 0x4C box minY
    f32 field_0x50;        // 0x50 box minZ
    f32 field_0x54;        // 0x54 box maxX
    f32 field_0x58;        // 0x58 box maxY
    f32 field_0x5C;        // 0x5C box maxZ
    u8 field_0x60[0x30];   // 0x60..0x8F
    u16 field_0x90;        // 0x90
    u8 field_0x92[10];     // 0x92..0x9B
    ScnPtmf field_0x9C;    // 0x9C..0xA7
};

// Data-global imports referenced by the accessors below (retail names).
extern const f32 lbl_eu_8066A890; // box half-size / clearance radius
extern ScnPtmf lbl_eu_8056DC74;   // default 12-byte reference block

// Retail s16/s32->f32 conversion magic (lbl_eu_8066A8A0 = 0x4330000080000000),
// referenced via the union trick so the TU emits no local .sdata2 pool
// (retail UnkClass_8047E110.o .sdata2 is empty).
extern double lbl_eu_8066A8A0;
union F64Conv_A8A0 {
    f64 d;
    u32 w[2];
};
static inline f32 s32ToF_A8A0(s32 v) {
    F64Conv_A8A0 c;
    c.w[0] = 0x43300000u;
    c.w[1] = (u32)v ^ 0x80000000u;
    return (f32)(c.d - lbl_eu_8066A8A0);
}

// Real-signature views of the func_8047E1B0 / func_8047E390 member stubs
// (retail Fv mangling; signatures recovered from caller register setup).
extern "C" s32 func_8047E1B0__17UnkClass_8047E110Fv(UnkClass_8047E110* self, const ml::CVec3* pos);
extern "C" s32 func_8047E390__17UnkClass_8047E110Fv(UnkClass_8047E110* self, const ml::CVec3* pos, s32 index, f32 clearance);

static inline void setWalkRect(CVec3& min, CVec3& max, const ScnWalkNode& node) {
    min.x = lbl_eu_8066A898 * s32ToF_A8A0(node.x);
    max.x = min.x + lbl_eu_8066A898 * s32ToF_A8A0(node.width + 1);
    min.z = lbl_eu_8066A898 * s32ToF_A8A0(node.z);
    max.z = min.z + lbl_eu_8066A898 * s32ToF_A8A0(node.depth + 1);
}

// ============================================================
// FULL_MATCH functions
// ============================================================

/**
 * sinit_80481E68 -- static initializer registered in .ctors.
 *
 * Empty body (just blr). Referenced from the .ctors section at
 * 0x804FA158 to trigger TU-level construction ordering.
 *
 * Retail: 4E 80 00 20  (blr)
 */
extern "C" void sinit_80481E68() {}

// ============================================================
// NOT_STARTED stubs (to be decompiled)
// ============================================================

void UnkClass_8047E110::func_8047E110() {}

void UnkClass_8047E110::func_8047E1B0() {}

void UnkClass_8047E110::func_8047E390() {}

// func_8047E62C -- two-stage segment solve: E1B0 (point-in-box) then E390
// (neighbour search with clearance); both results stored, 1 on success.
extern "C" s32 func_8047E62C__17UnkClass_8047E110Fv(UnkClass_8047E110* self, u32* out, u32* a, const ml::CVec3* b, f32 f) {
    s32 r = func_8047E1B0__17UnkClass_8047E110Fv(self, b);
    *out = r;
    if (r == -1) return 0;
    s32 r2 = func_8047E390__17UnkClass_8047E110Fv(self, b, r, f);
    *a = r2;
    if (r2 == -1) return 0;
    return 1;
}

void UnkClass_8047E110::func_8047E6C4() {}

void UnkClass_8047E110::func_8047EAD4() {}

void UnkClass_8047E110::func_8047EEB0() {}

void UnkClass_8047E110::func_8047EFBC() {}

void UnkClass_8047E110::func_8047F214() {}

void UnkClass_8047E110::func_8047F658() {}

void UnkClass_8047E110::func_8047F730() {}

void UnkClass_8047E110::func_8047FE48() {}

void UnkClass_8047E110::func_8047FF9C() {}

void UnkClass_8047E110::func_80480140() {}

void UnkClass_8047E110::func_8048020C() {}

void UnkClass_8047E110::func_804804DC() {}

extern "C" bool func_804808A0__17UnkClass_8047E110Fv(
    UnkClass_8047E110* self, const CVec3* position, s32 startNode,
    s32 goalNode, f32 clearance) {
    if (startNode == goalNode) return true;

    ScnWalkGraph& graph = *(ScnWalkGraph*)self;
    const ScnWalkNode& goalNodeData = graph.nodes[goalNode];
    CVec3 goalCenter;
    goalCenter.y = position->y;
    goalCenter.z = lbl_eu_8066A898 * s32ToF_A8A0(goalNodeData.z)
        + lbl_eu_8066A8A8 * s32ToF_A8A0(goalNodeData.depth + 1);
    goalCenter.x = lbl_eu_8066A898 * s32ToF_A8A0(goalNodeData.x)
        + lbl_eu_8066A8A8 * s32ToF_A8A0(goalNodeData.width + 1);
    CVec3 direction;
    direction = goalCenter - *position;

    f32 crossingX;
    f32 crossingZ;
    f32 deltaX;
    f32 deltaZ;
    f32 distance;
    f32 dzPerX;
    f32 dxPerZ;
    f32 maxX;
    f32 maxZ;
    f32 minX;
    f32 minZ;
    dzPerX = lbl_eu_8066A8AC;
    dxPerZ = lbl_eu_8066A8AC;
    if (direction.x != lbl_eu_8066A8AC) dzPerX = direction.z / direction.x;
    if (direction.z != lbl_eu_8066A8AC) dxPerZ = direction.x / direction.z;

    if (goalCenter.x < position->x) {
        maxX = position->x;
        minX = goalCenter.x;
    } else {
        maxX = goalCenter.x;
        minX = position->x;
    }

    if (goalCenter.z < position->z) {
        maxZ = position->z;
        minZ = goalCenter.z;
    } else {
        maxZ = goalCenter.z;
        minZ = position->z;
    }

    s32 frontierA[10];
    s32 frontierB[10];
    s32 pass;
    s32* frontier = frontierA;
    s32* next = frontierB;
    s32 frontierCount = 1;
    frontier[0] = startNode;
    f32 bestDistance = lbl_eu_8066A8B8;

    pass = 0;
    while (frontierCount != 0 && ++pass < 90) {
        s32 nextCount = 0;
        for (s32 i = 0; i < frontierCount; ++i) {
            CVec3 currentMax;
            CVec3 currentMin;
            const ScnWalkNode& currentNode = graph.nodes[frontier[i]];
            u16* edges = graph.edges;
            setWalkRect(currentMin, currentMax, currentNode);
            u32 edgeOffset = currentNode.edgeOffset;
            u32 neighborOffset = edgeOffset + 1;
            u16 edgeCount = edges[edgeOffset];
            const u16* neighbors = edges + neighborOffset;

            for (s32 edge = 0; edge < edgeCount; ++edge) {
                s32 candidateIndex = neighbors[edge];
                const ScnWalkNode& candidateNode = graph.nodes[candidateIndex];
                CVec3 portalMax;
                CVec3 portalMin;
                CVec3 candidateMax;
                CVec3 candidateMin;
                setWalkRect(candidateMin, candidateMax, candidateNode);
                if (currentMax.x == candidateMin.x) {
                    if (maxX < currentMax.x - clearance || minX > currentMax.x + clearance)
                        continue;
                    if (currentMax.z < candidateMax.z)
                        portalMax.z = currentMax.z;
                    else
                        portalMax.z = candidateMax.z;
                    if (currentMin.z > candidateMin.z)
                        portalMin.z = currentMin.z;
                    else
                        portalMin.z = candidateMin.z;
                    portalMax.x = currentMax.x;
                    portalMin.x = currentMax.x;
                    if (dzPerX == lbl_eu_8066A8AC) continue;
                    crossingX = portalMax.x;
                    crossingZ = goalCenter.z + dzPerX * (crossingX - goalCenter.x);
                    if (portalMax.z - clearance < crossingZ) continue;
                    if (portalMin.z + clearance > crossingZ) continue;
                    goto crossingAccepted;
                } else if (candidateMax.x == currentMin.x) {
                    if (maxX < candidateMax.x - clearance || minX > candidateMax.x + clearance)
                        continue;
                    if (currentMax.z < candidateMax.z)
                        portalMax.z = currentMax.z;
                    else
                        portalMax.z = candidateMax.z;
                    if (currentMin.z > candidateMin.z)
                        portalMin.z = currentMin.z;
                    else
                        portalMin.z = candidateMin.z;
                    portalMax.x = candidateMax.x;
                    portalMin.x = candidateMax.x;
                    if (dzPerX == lbl_eu_8066A8AC) continue;
                    crossingX = portalMax.x;
                    crossingZ = goalCenter.z + dzPerX * (crossingX - goalCenter.x);
                    if (portalMax.z - clearance < crossingZ) continue;
                    if (portalMin.z + clearance > crossingZ) continue;
                    goto crossingAccepted;
                } else if (currentMax.z == candidateMin.z) {
                    if (maxZ < currentMax.z - clearance || minZ > currentMax.z + clearance)
                        continue;
                    portalMax.x = currentMax.x < candidateMax.x ? currentMax.x : candidateMax.x;
                    portalMin.x = currentMin.x > candidateMin.x ? currentMin.x : candidateMin.x;
                    portalMax.z = currentMax.z;
                    portalMin.z = currentMax.z;
                    if (dxPerZ == lbl_eu_8066A8AC) continue;
                    crossingZ = portalMax.z;
                    crossingX = goalCenter.x + dxPerZ * (crossingZ - goalCenter.z);
                    if (portalMax.x - clearance < crossingX) continue;
                    if (portalMin.x + clearance > crossingX) continue;
                    goto crossingAccepted;
                } else if (candidateMax.z == currentMin.z) {
                    if (maxZ < candidateMax.z - clearance || minZ > candidateMax.z + clearance)
                        continue;
                    portalMax.x = currentMax.x < candidateMax.x ? currentMax.x : candidateMax.x;
                    portalMin.x = currentMin.x > candidateMin.x ? currentMin.x : candidateMin.x;
                    portalMax.z = candidateMax.z;
                    portalMin.z = candidateMax.z;
                    if (dxPerZ == lbl_eu_8066A8AC) continue;
                    crossingZ = portalMax.z;
                    crossingX = goalCenter.x + dxPerZ * (crossingZ - goalCenter.z);
                    if (portalMax.x - clearance < crossingX) continue;
                    if (portalMin.x + clearance > crossingX) continue;
                    goto crossingAccepted;
                } else {
                    return false;
                }

crossingAccepted:
                deltaX = position->x - crossingX;
                deltaZ = position->z - crossingZ;
                distance = deltaX * deltaX + deltaZ * deltaZ;
                if (bestDistance < distance) {
                    bestDistance = distance;
                    if (goalNode == candidateIndex) return true;
                    next[nextCount++] = candidateIndex;
                }
            }
        }

        s32* swap = next;
        next = frontier;
        frontier = swap;
        frontierCount = nextCount;
    }
    return false;
}

// func_80480EF0 -- scan a node's neighbour list for `value` (direct edge test).
extern "C" s32 func_80480EF0__17UnkClass_8047E110Fv(UnkClass_8047E110* self, u32 nodeIndex, u16 value) {
    ScnManagerLayout& m = *(ScnManagerLayout*)self;
    const ScnWalkNode& node = m.nodes[nodeIndex];
    u16 edgeCount = m.edges[node.edgeOffset];
    const u16* neighbors = &m.edges[node.edgeOffset + 1];
    for (s32 i = 0; i < edgeCount; ++i) {
        if (neighbors[i] == value) return 1;
    }
    return 0;
}

void UnkClass_8047E110::func_80480F48() {}

// func_80481014 -- activate/clear the walk box: a==b==0 clears it (flag bit 0
// off); otherwise write the clearance pair, position words and set the flag.
// Either way, advance the accumulated distance by `c`.
extern "C" void func_80481014__17UnkClass_8047E110Fv(UnkClass_8047E110* self, f32 a, f32 b, const ScnVecWords* v, f32 c) {
    ScnManagerLayout& m = *(ScnManagerLayout*)self;
    if (a == lbl_eu_8066A8AC && b == lbl_eu_8066A8AC) {
        m.field_0x38 &= ~1;
    } else {
        m.field_0x38 |= 1;
        m.field_0x24 = b;
        m.field_0x28 = a;
        m.field_0x2C = v->field_0x0;
        m.field_0x30 = v->field_0x4;
        m.field_0x34 = v->field_0x8;
    }
    m.field_0x20 = m.field_0x1C + c;
}

void UnkClass_8047E110::func_80481074() {}

void UnkClass_8047E110::func_804812D8() {}

void UnkClass_8047E110::func_804813E8() {}

void UnkClass_8047E110::func_804814DC() {}

// func_8048163C -- build the walk box (half-size lbl_eu_8066A890) around `v`
// into 0x48-0x5C, reset the 0x90 counter and refresh the reference block at
// 0x9C from the data global.
extern "C" void func_8048163C__17UnkClass_8047E110Fv(UnkClass_8047E110* self, const ml::CVec3* v) {
    ScnManagerLayout& m = *(ScnManagerLayout*)self;
    f32 c = lbl_eu_8066A890;
    m.field_0x90 = 0;
    m.field_0x4C = v->y;
    m.field_0x58 = v->y;
    m.field_0x48 = v->x - c;
    m.field_0x50 = v->z - c;
    m.field_0x54 = v->x + c;
    m.field_0x5C = v->z + c;
    m.field_0x9C = lbl_eu_8056DC74;
}

void UnkClass_8047E110::func_8048169C() {}

void UnkClass_8047E110::func_80481790() {}

// func_804819AC -- test bit 1 of the node flag word (offset 0x8); the walker
// skips nodes whose closed/blocked bit is set.
extern "C" s32 func_804819AC__17UnkClass_8047E110Fv(UnkClass_8047E110* self, u32 nodeIndex) {
    ScnManagerLayout& m = *(ScnManagerLayout*)self;
    return (m.nodes[nodeIndex].reserved8 & 2) != 0;
}

void UnkClass_8047E110::func_804819C4() {}

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// [.data] 0x8056DC68-0x8056DCD8 (112B): three 12-byte reference blocks, the
// CScnItemCameraNw4r vtable, and two RTTI base-lists.
// The own-member symbols func_80481074/804812D8/804813E8 are the stub member
// functions defined above (retail Fv mangling); the remaining slots are
// foreign functions/locators.
extern "C" void func_80481074__17UnkClass_8047E110Fv();
extern "C" void func_804812D8__17UnkClass_8047E110Fv();
extern "C" void func_804813E8__17UnkClass_8047E110Fv();
extern "C" void __dt__18CScnItemCameraNw4rFv();
extern "C" void func_8049F9A4();
extern "C" void func_80482048();
extern "C" void func_80481F9C();
extern "C" void func_8048204C();
extern "C" void func_80482040();
extern "C" void func_80482038();
extern "C" void func_80481F00();
extern "C" u32 lbl_eu_806638A8;   // .sdata RTTI locator (foreign unit)
extern "C" u32 lbl_eu_806624D8;   // .sdata RTTI locator (foreign unit)
extern "C" u32 lbl_eu_806638B0;   // .sdata RTTI locator (foreign unit)

extern "C" u32 lbl_eu_8056DC68[3] = {
    0x00000000, 0xFFFFFFFF, (u32)&func_80481074__17UnkClass_8047E110Fv,
};
// The default 12-byte reference block (also the retail symbol referenced by
// func_8048163C). Defined as the ScnPtmf struct so the field copy compiles.
// NOTE: declared AFTER lbl_eu_8056DC68 so MWCC emits the .data symbols in
// retail offset order (definition order = section order for .data).
ScnPtmf lbl_eu_8056DC74 = { 0x00000000, 0xFFFFFFFF, (u32)&func_804812D8__17UnkClass_8047E110Fv };
DECOMP_FORCEACTIVE(UnkClass_8047E110_cpp, lbl_eu_8056DC74);
extern "C" u32 lbl_eu_8056DC80[4] = {
    0x00000000, 0xFFFFFFFF, (u32)&func_804813E8__17UnkClass_8047E110Fv,
    0x00000000,
};
extern "C" u32 lbl_eu_8056DC90[10] = {
    (u32)&lbl_eu_806638A8, 0x00000000,
    (u32)&__dt__18CScnItemCameraNw4rFv, (u32)&func_8049F9A4,
    (u32)&func_80482048, (u32)&func_80481F9C,
    (u32)&func_8048204C, (u32)&func_80482040,
    (u32)&func_80482038, (u32)&func_80481F00,
};
extern "C" u32 lbl_eu_8056DCB8[5] = {
    (u32)&lbl_eu_806624D8, 0x00000000, (u32)&lbl_eu_806638B0, 0x00000000,
    0x00000000,
};
extern "C" u32 lbl_eu_8056DCCC[3] = { (u32)&lbl_eu_806624D8, 0x00000000, 0x00000000 };
DECOMP_FORCEACTIVE(UnkClass_8047E110_cpp, lbl_eu_8056DC68);
DECOMP_FORCEACTIVE(UnkClass_8047E110_cpp, lbl_eu_8056DC90);
DECOMP_FORCEACTIVE(UnkClass_8047E110_cpp, lbl_eu_8056DCB8);
