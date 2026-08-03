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

extern "C" const f32 lbl_eu_8066A898;
extern "C" const f32 lbl_eu_8066A8A8;
extern "C" const f32 lbl_eu_8066A8AC;
extern "C" const f32 lbl_eu_8066A8B8;

static inline void setWalkRect(CVec3& min, CVec3& max, const ScnWalkNode& node) {
    min.x = lbl_eu_8066A898 * node.x;
    max.x = min.x + lbl_eu_8066A898 * (node.width + 1);
    min.z = lbl_eu_8066A898 * node.z;
    max.z = min.z + lbl_eu_8066A898 * (node.depth + 1);
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

void UnkClass_8047E110::func_8047E62C() {}

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
    goalCenter.z = lbl_eu_8066A898 * goalNodeData.z
        + lbl_eu_8066A8A8 * (goalNodeData.depth + 1);
    goalCenter.x = lbl_eu_8066A898 * goalNodeData.x
        + lbl_eu_8066A8A8 * (goalNodeData.width + 1);
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

void UnkClass_8047E110::func_80480EF0() {}

void UnkClass_8047E110::func_80480F48() {}

void UnkClass_8047E110::func_80481014() {}

void UnkClass_8047E110::func_80481074() {}

void UnkClass_8047E110::func_804812D8() {}

void UnkClass_8047E110::func_804813E8() {}

void UnkClass_8047E110::func_804814DC() {}

void UnkClass_8047E110::func_8048163C() {}

void UnkClass_8047E110::func_8048169C() {}

void UnkClass_8047E110::func_80481790() {}

void UnkClass_8047E110::func_804819AC() {}

void UnkClass_8047E110::func_804819C4() {}
