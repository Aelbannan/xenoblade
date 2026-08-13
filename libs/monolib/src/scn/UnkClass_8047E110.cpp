// Translation unit: monolib/src/scn/UnkClass_8047E110
// Unknown scene class implementation.
//
// Status: Only sinit_80481E68 is FULL_MATCH (empty .ctors hook, 4 bytes).
//         Remaining 29 functions are NOT_STARTED stubs.

#include <monolib/scn/UnkClass_8047E110.hpp>
#include <monolib/math/CVec3.hpp>
#include <nw4r/math/math_types.h>
#include <string.h>

using ml::CVec3;

struct ScnWalkNode {
    f32 yPos;         // 0x00 world-space height
    u32 edgeOffset;   // 0x04 edge-list offset (u16 pairs)
    u16 reserved8;    // 0x08 flags
    u8 width;         // 0x0A
    u8 depth;         // 0x0B
    s16 x;            // 0x0C grid X
    s16 z;            // 0x0E grid Z
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

// 12-byte float triple used for the walk box / direction vector storage.
// nw4r::math::VEC3/_VEC3 forces word-based struct copies (lwz/stw).
struct ScnBox {
    f32 x;
    f32 y;
    f32 z;
};

// 12-byte reference block stored at 0x9C (pointer-to-member style triple).
struct ScnPtmf {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
};

// 12-byte per-node reference record (manager +0x08 pool): neighbour run start
// (pool index) + count, an edge-list index, then two spare ids.
struct ScnNodeRef {
    u16 edgeStart;   // 0x00 first neighbour pool index
    u16 edgeCount;   // 0x02 neighbour count
    u32 field_0x4;   // 0x04 edge-list index
    u16 field_0x8;   // 0x08
    u16 field_0xa;   // 0x0a
};

// 10-byte per-node lookup record (manager +0x0C pool): two alternate ids, a
// value, and two flag bytes.
struct ScnNodeMap {
    u16 field_0x0;   // 0x00
    u16 field_0x2;   // 0x02
    u16 field_0x4;   // 0x04
    u16 field_0x6;   // 0x06
    u8 field_0x8;    // 0x08
    u8 field_0x9;    // 0x09
};

// Manager field view (retail offsets) used by the accessor methods below.
struct ScnManagerLayout {
    u32* field_0x0;        // 0x00 node-count pointer
    ScnWalkNode* nodes;    // 0x04 node pool
    ScnNodeRef* field_0x8; // 0x08 per-node refs (12-byte stride)
    ScnNodeMap* field_0xc; // 0x0C per-node lookup (10-byte stride)
    u32 field_0x10;        // 0x10
    u16* edges;            // 0x14 edge pool
    f32 field_0x18;        // 0x18
    f32 field_0x1C;        // 0x1C
    f32 field_0x20;        // 0x20
    f32 field_0x24[2];     // 0x24..0x2B per-axis clearance pair
    u32 field_0x2C;        // 0x2C
    u32 field_0x30;        // 0x30
    u32 field_0x34;        // 0x34
    u32 field_0x38;        // 0x38 flags (bit 0 = box active)
    u8 field_0x3C[12];     // 0x3C..0x47
    ml::CVec3 boxMin;    // 0x48..0x53 box min corner
    ml::CVec3 boxMax;    // 0x54..0x5F box max corner
    ml::CVec3 diag;      // 0x60..0x6B box diagonal (max-min)
    f32 field_0x6C;        // 0x6C
    f32 field_0x70;        // 0x70
    f32 field_0x74;        // 0x74
    f32 field_0x78;        // 0x78
    f32 field_0x7C;        // 0x7C
    f32 field_0x80;        // 0x80
    f32 field_0x84;        // 0x84
    f32 field_0x88;        // 0x88 slope dz/dx
    f32 field_0x8C;        // 0x8C slope dx/dz
    u16 field_0x90;        // 0x90
    u8 field_0x92[10];     // 0x92..0x9B
    ScnPtmf field_0x9C;    // 0x9C..0xA7
};

// Data-global imports referenced by the accessors below (retail names).
extern const f32 lbl_eu_8066A890; // box half-size / clearance radius
extern const f32 lbl_eu_8066A894; // initial best-distance (large)
extern const f64 lbl_eu_8066A8A0; // 2^52 double for the s32->f32 trick
extern ScnPtmf lbl_eu_8056DC74;   // default 12-byte reference block
extern ScnPtmf lbl_eu_8056DC80;   // default 12-byte reference block (alt)

// Walk-search global: the traversal buffer pointer lives at +0x08.
struct ScnSearchGlob {
    u32 field_0x0;
    u32 field_0x4;
    u16* buffer;   // 0x08
};
extern ScnSearchGlob lbl_eu_80658540;

// C-ABI allocator getter (retail func_804B5A68, no-arg, returns the bitset
// scratch pointer).
extern "C" u32 func_804B5A68(void);

// func_80480140 -- recursive BFS step (declared here; defined in this TU).
extern "C" s32 func_80480140__17UnkClass_8047E110Fv(UnkClass_8047E110* self, u16* out2, u16* buffer, u32* bits, s32 value2, s32 count, ScnNodeRef* ref);

// func_8047FF9C -- recursive neighbour resolve (declared here; defined in this
// TU). Takes the walk-search context plus the current node and BFS depth.
extern "C" s32 func_8047FF9C__17UnkClass_8047E110Fv(UnkClass_8047E110* self, s32 arg5, u16* bufB, u32* bits, s32 arg7, s32 arg8, u16 nodeId, s32 depth);

// func_8047EEB0 -- neighbour-walk clearance test (defined below in this TU).
extern "C" s32 func_8047EEB0__17UnkClass_8047E110Fv(UnkClass_8047E110* self, const ScnWalkNode* node, s32 value, u32 dir);

// Real-signature views of the func_8047E1B0 / func_8047E390 member stubs
// (retail Fv mangling; signatures recovered from caller register setup).
extern "C" s32 func_8047E1B0__17UnkClass_8047E110Fv(UnkClass_8047E110* self, const ml::CVec3* pos);
extern "C" s32 func_8047E390__17UnkClass_8047E110Fv(UnkClass_8047E110* self, const ml::CVec3* pos, s32 index, f32 clearance);

static inline void setWalkRect(CVec3& min, CVec3& max, const ScnWalkNode& node) {
    min.x = lbl_eu_8066A898 * node.x;
    max.x = min.x + lbl_eu_8066A898 * (node.width + 1);
    min.z = lbl_eu_8066A898 * node.z;
    max.z = min.z + lbl_eu_8066A898 * (node.depth + 1);
}

// s32 -> f32 through the shared signed magic double (lbl_eu_8066A8A0,
// 0x4330000080000000 = 2^52 + 2^31): the value is stored as the low word of
// a 0x43300000 double with the sign bit flipped, then the magic is subtracted.
// Writing the words by hand keeps the named reloc (MWCC would otherwise emit
// its own anonymous 2^52 literal).
union ConvF64 {
    f64 d;
    u32 w[2];
};

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


// func_8047E390 -- pick the best neighbour of `index` near `pos` within
// `clearance`. A node whose grid cell contains pos wins over a merely-nearby
// one; the score is |pos.y - node.yPos| and the result is the pool index of
// the winner (-1 if none). The head flag test ("can use node") is shared with
// the other walk-graph scans.
extern "C" s32 func_8047E390__17UnkClass_8047E110Fv(UnkClass_8047E110* self, const ml::CVec3* pos, s32 index, f32 clearance) {
    ScnManagerLayout& m = *(ScnManagerLayout*)self;
    f32 minX = pos->x - clearance;
    f32 minZ = pos->z - clearance;
    f32 bestDist = lbl_eu_8066A894;
    f32 maxX = pos->x + clearance;
    f32 maxZ = pos->z + clearance;
    const f64 magic = lbl_eu_8066A8A0; // 2^52 + 2^31 signed-conversion magic
    const ScnNodeRef& ref = m.field_0x8[index];
    u16 start = ref.edgeStart;
    u16 count = ref.edgeCount;
    ScnWalkNode* node = &m.nodes[start];
    // Persistent conversion doubles (retail stack 0x8/0x10): the 0x4330 high
    // words are written once here and hoisted to the prologue.
    ConvF64 cx, cz;
    cx.w[0] = 0x43300000u;
    cz.w[0] = 0x43300000u;
    s32 best = -1;
    f32 scale = lbl_eu_8066A898;
    s32 insideFound = 1;
    s32 firstTaken = 1;
    s32 prioFlag = 1;

    for (s32 i = 0; i < count; ++i, ++node) {
        u32 flags = m.field_0x38;
        s32 pass;
        if (!(flags & 1) && (node->reserved8 & 2)) {
            pass = 0;
        } else if (!(flags & 2)) {
            pass = (node->reserved8 & 4) == 0;
        } else if (!(flags & 4)) {
            pass = (node->reserved8 & 4) == 0;
        } else {
            pass = 1;
        }
        if (!pass) {
            continue;
        }

        s16 nx = node->x;
        s16 nz = node->z;
        cx.w[1] = (u32)nx ^ 0x80000000u;
        f32 gx = scale * (f32)(cx.d - magic);
        cz.w[1] = (u32)nz ^ 0x80000000u;
        f32 gz = scale * (f32)(cz.d - magic);
        if (maxX < gx || maxZ < gz) {
            continue;
        }
        cx.w[1] = (u32)(nx + node->width + 1) ^ 0x80000000u;
        f32 ex = scale * (f32)(cx.d - magic);
        cz.w[1] = (u32)(nz + node->depth + 1) ^ 0x80000000u;
        f32 ez = scale * (f32)(cz.d - magic);
        if (minX >= ex || minZ >= ez) {
            continue;
        }
        f32 dy = ml::math::abs(pos->y - node->yPos);
        if (dy >= m.field_0x20) {
            continue;
        }

        if (pos->x >= gx && pos->z >= gz && pos->x < ex && pos->z < ez) {
            if (insideFound) {
                bestDist = dy;
                best = start + i;
                insideFound = 0;
                firstTaken = 0;
                if ((flags & 4) && (node->reserved8 & 4)) {
                    prioFlag = 0;
                }
            } else if (flags & 4) {
                if (node->reserved8 & 4) {
                    if (prioFlag) {
                        bestDist = dy;
                        best = start + i;
                        prioFlag = 0;
                    } else {
                        goto compareDy;
                    }
                } else if (prioFlag) {
                    goto compareDy;
                } else {
                    continue;
                }
            } else {
                goto compareDy;
            }
            continue;
compareDy:
            if (dy < bestDist) {
                bestDist = dy;
                best = start + i;
            }
        } else if (insideFound) {
            if (firstTaken) {
                bestDist = dy;
                best = start + i;
                firstTaken = 0;
            } else if (dy < bestDist) {
                bestDist = dy;
                best = start + i;
            }
        }
    }
    return best;
}

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

// func_8047E6C4 -- resolve the node `nodeIndex` against `pos` + clearance.
// If the target node's cell intersects the clearance box and the walker flags
// allow it, return it directly; otherwise scan its neighbours for one whose
// cell contains pos (or overlaps the box with matching height), preferring a
// flagged node whose Y is within the per-axis clearance of the target.
// Writes the found node id and the E1B0 point-in-box result.
extern "C" s32 func_8047E6C4__17UnkClass_8047E110Fv(UnkClass_8047E110* self, u32* out1, u32* out2, u32 arg1, s32 nodeIndex, const ml::CVec3* pos, f32 clearance) {
    ScnManagerLayout& m = *(ScnManagerLayout*)self;
    const ScnWalkNode& target = m.nodes[nodeIndex];
    f32 maxX = pos->x + clearance;
    f32 maxZ = pos->z + clearance;
    s16 tx = target.x;
    s16 tz = target.z;
    ConvF64 cx, cz;
    cx.w[0] = 0x43300000u;
    cz.w[0] = 0x43300000u;
    f64 magic = lbl_eu_8066A8A0;
    f32 scale = lbl_eu_8066A898;
    f32 minX = pos->x - clearance;
    f32 minZ = pos->z - clearance;
    s32 flag12 = 0;

    cx.w[1] = (u32)tx ^ 0x80000000u;
    cz.w[1] = (u32)tz ^ 0x80000000u;
    f32 gx = scale * (f32)(cx.d - magic);
    f32 gz = scale * (f32)(cz.d - magic);
    cx.w[1] = (u32)(tx + target.width + 1) ^ 0x80000000u;
    cz.w[1] = (u32)(tz + target.depth + 1) ^ 0x80000000u;
    f32 ex = scale * (f32)(cx.d - magic);
    f32 ez = scale * (f32)(cz.d - magic);
    f32 dy = ml::math::abs(pos->y - target.yPos);

    if (maxX >= gx && maxZ >= gz && minX <= ex && minZ <= ez && dy <= m.field_0x20) {
        if ((target.reserved8 & 2) && (m.field_0x38 & 1)) {
            *out1 = arg1;
            *out2 = nodeIndex;
            return 1;
        }
        flag12 = 1;
    }
    if (pos->x >= gx && pos->z >= gz && pos->x <= ex && pos->z <= ez
        && ml::math::abs(pos->y - target.yPos) <= m.field_0x20) {
        *out1 = arg1;
        *out2 = nodeIndex;
        return 1;
    }

    s32 result = -1;
    u16 edgeCount = m.edges[target.edgeOffset];
    const u16* neighbors = &m.edges[target.edgeOffset + 1];
    for (s32 i = 0; i < edgeCount; ++i) {
        u16 id = neighbors[i];
        const ScnWalkNode& node = m.nodes[id];
        u32 flags = m.field_0x38;
        u32 boxActive = flags & 1;
        u32 pass;
        if (!boxActive && (node.reserved8 & 2)) {
            pass = 0;
        } else if (!(flags & 2)) {
            pass = (node.reserved8 & 4) == 0;
        } else if (!(flags & 4)) {
            pass = (node.reserved8 & 4) == 0;
        } else {
            pass = 1;
        }
        if (!pass) {
            continue;
        }

        s16 nx = node.x;
        s16 nz = node.z;
        cx.w[1] = (u32)nx ^ 0x80000000u;
        f32 ngx = scale * (f32)(cx.d - magic);
        cz.w[1] = (u32)nz ^ 0x80000000u;
        f32 ngz = scale * (f32)(cz.d - magic);
        if (maxX < ngx || maxZ < ngz) {
            continue;
        }
        cx.w[1] = (u32)(nx + node.width + 1) ^ 0x80000000u;
        f32 nex = scale * (f32)(cx.d - magic);
        cz.w[1] = (u32)(nz + node.depth + 1) ^ 0x80000000u;
        f32 nez = scale * (f32)(cz.d - magic);
        if (minX > nex || minZ > nez) {
            continue;
        }
        f32 ndy = ml::math::abs(pos->y - node.yPos);
        if (ndy > m.field_0x20) {
            continue;
        }

        if (node.reserved8 & 2) {
            if (!boxActive) {
                continue;
            }
            if (node.yPos < target.yPos) {
                if (m.field_0x24[0] < target.yPos - node.yPos) {
                    continue;
                }
            } else if (node.yPos > target.yPos) {
                if (m.field_0x24[1] < node.yPos - target.yPos) {
                    continue;
                }
            }
            result = id;
            break;
        }
        if (pos->x >= ngx && pos->z >= ngz && pos->x <= nex && pos->z <= nez) {
            result = id;
            break;
        }
    }

    if (result != -1) {
        s32 r = func_8047E1B0__17UnkClass_8047E110Fv(self, pos);
        *out1 = r;
        if (r == -1) {
            return 0;
        }
        *out2 = result;
        return 1;
    }
    if (flag12) {
        *out1 = arg1;
        *out2 = nodeIndex;
        return 1;
    }
    return 0;
}

// func_8047EAD4 -- walk-graph node set query. When the target node's flag bit
// 0 is set, scan the reference edge list for the neighbour whose lookup id
// matches `arg6`, then append every neighbour whose lookup value is `arg5` and
// whose lookup flag byte equals the match; return the count. Otherwise run a
// bounded BFS from `arg6` (frontier = out1 itself) and, on the first flagged
// neighbour, perform the same lookup scan into out1 and return its count.
extern "C" s32 func_8047EAD4__17UnkClass_8047E110Fv(UnkClass_8047E110* self, u16* out1, s32 arg5, s32 arg6, s32 arg7) {
    ScnManagerLayout& m = *(ScnManagerLayout*)self;
    const ScnWalkNode& target = m.nodes[arg6];
    const ScnNodeRef& ref = m.field_0x8[arg5];
    u16 edgeCount = m.edges[ref.field_0x4];
    const u16* neighbors = &m.edges[ref.field_0x4 + 1];

    if (target.reserved8 & 1) {
        u8 match;
        for (s32 i = 0; i < edgeCount; ++i) {
            const ScnNodeMap& lk = m.field_0xc[neighbors[i]];
            if (arg6 == lk.field_0x0) {
                match = lk.field_0x8;
                break;
            }
            if (arg6 == lk.field_0x2) {
                match = lk.field_0x9;
                break;
            }
        }
        s32 count = 0;
        for (s32 i = 0; i < edgeCount; ++i) {
            const ScnNodeMap& lk = m.field_0xc[neighbors[i]];
            if (arg5 == lk.field_0x4) {
                if (match == lk.field_0x8) {
                    out1[count++] = neighbors[i];
                }
            } else if (match == lk.field_0x9) {
                out1[count++] = neighbors[i];
            }
        }
        return count;
    }

    u32* bits = (u32*)func_804B5A68();
    memset(bits, 0, (ref.edgeCount / 32 + 1) * 4);
    u16* frontier = out1;
    u16* next = out1 + ref.edgeCount;
    *frontier = arg6;
    bits[(arg6 - ref.edgeStart) / 32] |= 1 << ((arg6 - ref.edgeStart) & 31);

    s32 nodeCount = 1;
    while (nodeCount != 0) {
        s32 todo = nodeCount;
        u16* cur = frontier;
        u16* nxt = next;
        nodeCount = 0;
        for (s32 o = 0; o < todo; ++o) {
            u16 nodeId = *cur++;
            const ScnWalkNode& node = m.nodes[nodeId];
            u16 cnt = m.edges[node.edgeOffset];
            const u16* nbs = &m.edges[node.edgeOffset + 1];
            for (s32 e = 0; e < cnt; ++e) {
                u16 nb = nbs[e];
                s32 idx = nb - ref.edgeStart;
                if (idx < 0 || idx >= ref.edgeCount) {
                    continue;
                }
                if (bits[idx >> 5] & (1 << (idx & 31))) {
                    continue;
                }
                if (!func_8047EEB0__17UnkClass_8047E110Fv(self, &m.nodes[nb], nodeId, arg7)) {
                    continue;
                }
                if (m.nodes[nb].reserved8 & 1) {
                    u8 match;
                    for (s32 i = 0; i < edgeCount; ++i) {
                        const ScnNodeMap& lk = m.field_0xc[neighbors[i]];
                        if (nb == lk.field_0x0) {
                            match = lk.field_0x8;
                            break;
                        }
                        if (nb == lk.field_0x2) {
                            match = lk.field_0x9;
                            break;
                        }
                    }
                    s32 count = 0;
                    for (s32 i = 0; i < edgeCount; ++i) {
                        const ScnNodeMap& lk = m.field_0xc[neighbors[i]];
                        if (arg5 == lk.field_0x4) {
                            if (match == lk.field_0x8) {
                                out1[count++] = neighbors[i];
                            }
                        } else if (match == lk.field_0x9) {
                            out1[count++] = neighbors[i];
                        }
                    }
                    return count;
                }
                *nxt++ = nb;
                nodeCount++;
                bits[idx >> 5] |= 1 << (idx & 31);
            }
        }
        u16* swap = frontier;
        frontier = next;
        next = swap;
    }
    return 0;
}

// func_8047EEB0 -- neighbour-walk clearance test. With the walk box active
// (flag bit 0), scan the node's edge list; a neighbour whose X position
// differs from `value` must be reachable within the per-axis clearance
// (0x24/0x28, selected by `dir`). Without the box, fall back to the flag test.
extern "C" s32 func_8047EEB0__17UnkClass_8047E110Fv(UnkClass_8047E110* self, const ScnWalkNode* node, s32 value, u32 dir) {
    ScnManagerLayout& m = *(ScnManagerLayout*)self;

    if (m.field_0x38 & 1) {
        if (node->reserved8 & 2) {
            u16* edges = m.edges;
            const u16* neighbors = &edges[node->edgeOffset + 1];
            s32 edgeCount = edges[node->edgeOffset];
            for (s32 i = 0; i < edgeCount; ++i) {
                u16 nb = neighbors[i];
                if (nb == value) continue;
                f32 nx = m.nodes[nb].yPos;
                f32 vx = m.nodes[value].yPos;
                if (vx < nx) {
                    f32 delta = nx - vx;
                    f32 clearance = m.field_0x24[dir == 0 ? 1 : 0];
                    if (clearance >= delta) return 1;
                } else if (vx > nx) {
                    f32 delta = vx - nx;
                    f32 clearance = m.field_0x24[dir];
                    if (clearance >= delta) return 1;
                } else {
                    return 1;
                }
                return 0;
            }
            return 0;
        }
    } else {
        if (node->reserved8 & 2) return 0;
    }
    if ((m.field_0x38 & 2) == 0 && (node->reserved8 & 4)) return 0;
    return 1;
}

void UnkClass_8047E110::func_8047EFBC() {}

// func_8047F214 -- compute the horizontal direction from `pos` toward the
// portal between nodes `nodeA` and `nodeB`. When the two cells share an edge
// (one of the four adjacency tests), the portal is the midpoint of the shared
// segment, or the far edge of nodeA when pos is already inside the overlap
// band; otherwise the portal is nodeA's cell centre. The result is normalized
// into `out` (zero when degenerate).
extern "C" void func_8047F214__17UnkClass_8047E110Fv(
    UnkClass_8047E110* self, ml::CVec3* out, const ml::CVec3* pos,
    u32 nodeB, u32 nodeA) {
    ScnManagerLayout& m = *(ScnManagerLayout*)self;
    const ScnWalkNode& nodeA_ = m.nodes[nodeA];
    const ScnWalkNode& nodeB_ = m.nodes[nodeB];
    // Cell rect corners (12-byte each; y unused) and the portal point.
    ScnBox minA, minB, maxA, maxB, portal;

    // nodeA cell rect: minA = min corner, maxA = max corner.
    // Direct (f32)(u32)s16 casts make MWCC emit the 2^52 stw/lfd/fsubs trick.
    minA.x = lbl_eu_8066A898 * (f32)(u32)nodeA_.x;
    minA.z = lbl_eu_8066A898 * (f32)(u32)nodeA_.z;
    maxA.x = minA.x + lbl_eu_8066A898 * (f32)(u32)(nodeA_.width + 1);
    maxA.z = minA.z + lbl_eu_8066A898 * (f32)(u32)(nodeA_.depth + 1);

    // is nodeA a direct neighbour of nodeB?
    u16 edgeCount = m.edges[nodeB_.edgeOffset];
    const u16* neighbors = &m.edges[nodeB_.edgeOffset + 1];
    s32 found = 0;
    for (s32 i = 0; i < edgeCount; ++i) {
        if (neighbors[i] == nodeA) {
            found = 1;
            break;
        }
    }

    if (found) {
        // nodeB cell rect
        minB.x = lbl_eu_8066A898 * (f32)(u32)nodeB_.x;
        maxB.x = minB.x + lbl_eu_8066A898 * (f32)(u32)(nodeB_.width + 1);
        minB.z = lbl_eu_8066A898 * (f32)(u32)nodeB_.z;
        maxB.z = minB.z + lbl_eu_8066A898 * (f32)(u32)(nodeB_.depth + 1);

        // portal height: midpoint of the two cells (kept for symmetry)
        portal.y = lbl_eu_8066A898 * (nodeB_.yPos + nodeA_.yPos);

        if (maxB.x == minA.x) {
            // nodeB left of nodeA: portal Z is the shared-segment midpoint
            f32 zEnd = (maxB.z >= maxA.z) ? maxA.z : maxB.z;
            f32 zStart = (minB.z <= minA.z) ? minA.z : minB.z;
            portal.z = lbl_eu_8066A898 * (zEnd + zStart);
            if (zStart < pos->z && zEnd > pos->z) portal.x = maxA.x;
            else portal.x = maxB.x;
        } else if (maxA.x == minB.x) {
            // nodeA left of nodeB
            f32 zEnd = (maxB.z >= maxA.z) ? maxA.z : maxB.z;
            f32 zStart = (minB.z <= minA.z) ? minA.z : minB.z;
            portal.z = lbl_eu_8066A898 * (zEnd + zStart);
            if (zStart < pos->z && zEnd > pos->z) portal.x = minA.x;
            else portal.x = maxA.x;
        } else if (maxB.z == minA.z) {
            // nodeB in front of nodeA (Z): portal X is the midpoint
            f32 xEnd = (maxB.x >= maxA.x) ? maxA.x : maxB.x;
            f32 xStart = (minB.x <= minA.x) ? minA.x : minB.x;
            portal.x = lbl_eu_8066A898 * (xEnd + xStart);
            if (xStart < pos->x && xEnd > pos->x) portal.z = maxA.z;
            else portal.z = maxB.z;
        } else if (maxA.z == minB.z) {
            // nodeA in front of nodeB
            f32 xEnd = (maxB.x >= maxA.x) ? maxA.x : maxB.x;
            f32 xStart = (minB.x <= minA.x) ? minA.x : minB.x;
            portal.x = lbl_eu_8066A898 * (xEnd + xStart);
            if (xStart < pos->x && xEnd > pos->x) portal.z = minA.z;
            else portal.z = maxA.z;
        } else {
            // cells do not share an edge: aim at nodeA's centre
            portal.x = lbl_eu_8066A898 * (maxA.x + minA.x);
            portal.z = lbl_eu_8066A898 * (maxA.z + minA.z);
        }
    } else {
        portal.x = lbl_eu_8066A898 * (maxA.x + minA.x);
        portal.z = lbl_eu_8066A898 * (maxA.z + minA.z);
    }

    out->x = portal.x - pos->x;
    out->y = lbl_eu_8066A8AC;
    out->z = portal.z - pos->z;
    if (out->x == lbl_eu_8066A8AC && out->z == lbl_eu_8066A8AC) {
        out->x = lbl_eu_8066A8AC;
        out->y = lbl_eu_8066A8AC;
        out->z = lbl_eu_8066A8AC;
    } else {
        if (out->y * out->y + out->x * out->x + out->z * out->z
            == lbl_eu_8066A8AC) {
            *out = ml::CVec3::zero;
        } else {
            PSVECNormalize(*out, *out);
        }
    }
}

void UnkClass_8047E110::func_8047F658() {}

void UnkClass_8047E110::func_8047F730() {}

void UnkClass_8047E110::func_8047FE48() {}

void UnkClass_8047E110::func_8047FF9C() {}

void UnkClass_8047E110::func_80480140() {}

// func_8048020C -- bounded BFS over the walk graph from node `value` toward
// `value2`. The lookup table picks the starting id (primary/secondary), the
// frontier ping-pongs inside the shared traversal buffer, and each visited
// node is stamped with its BFS depth. Returns the depth (+1) via *out1 and
// appends the path to *out2, or 0 when the limit is hit.
extern "C" s32 func_8048020C__17UnkClass_8047E110Fv(UnkClass_8047E110* self, u16* out1, u16* out2, s32 limit, s32 value, s32 value2, s32 index) {
    ScnManagerLayout& m = *(ScnManagerLayout*)self;
    const ScnNodeMap& lookup = m.field_0xc[index];
    u16 id;
    s32 flag;
    if (value == lookup.field_0x4) {
        id = lookup.field_0x0;
        flag = 1;
    } else {
        id = lookup.field_0x2;
        flag = 0;
    }
    if (id == value2) {
        *out1 = 1;
        *out2 = flag ? lookup.field_0x2 : lookup.field_0x0;
        return 1;
    }

    ScnNodeRef& ref = m.field_0x8[value];
    u16 count = ref.edgeCount;
    u16* buffer = lbl_eu_80658540.buffer;
    u16* frontier = buffer + count;
    u16* next = frontier + count;
    u32* bits = (u32*)func_804B5A68();
    memset(bits, 0, (count / 32 + 1) * 4);
    *frontier = id;
    buffer[id - ref.edgeStart] = 0;
    bits[(id - ref.edgeStart) / 32] |= 1 << ((id - ref.edgeStart) & 31);

    s32 nodeCount = 1;
    s32 curCount = 1;
    while (nodeCount != 0) {
        s32 todo = nodeCount;
        u16* cur = frontier;
        u16* nxt = next;
        nodeCount = 0;
        for (s32 o = 0; o < todo; ++o) {
            u16 nodeId = *cur++;
            const ScnWalkNode& node = m.nodes[nodeId];
            u16 edgeCount = m.edges[node.edgeOffset];
            const u16* neighbors = &m.edges[node.edgeOffset + 1];
            for (s32 e = 0; e < edgeCount; ++e) {
                u16 nb = neighbors[e];
                s32 idx = nb - ref.edgeStart;
                if (idx < 0 || idx >= ref.edgeCount) {
                    continue;
                }
                if (bits[idx >> 5] & (1 << (idx & 31))) {
                    continue;
                }
                if (!func_8047EEB0__17UnkClass_8047E110Fv(self, &m.nodes[nb], nodeId, 1)) {
                    continue;
                }
                buffer[idx] = curCount;
                if (nb == value2) {
                    if (curCount + 1 >= limit) {
                        return 0;
                    }
                    func_80480140__17UnkClass_8047E110Fv(self, out2, buffer, bits, value2, curCount, &ref);
                    out2[curCount] = flag ? lookup.field_0x2 : lookup.field_0x0;
                    *out1 = curCount + 1;
                    return 1;
                }
                *nxt++ = nb;
                nodeCount++;
                bits[idx >> 5] |= 1 << (idx & 31);
            }
        }
        curCount++;
        u16* swap = frontier;
        frontier = next;
        next = swap;
    }
    return 0;
}

// func_804804DC -- bidirectional-ish BFS between seed nodes `arg7` and `arg8`
// over the walk graph. The traversal buffer is split into two frontiers
// (buffer / buffer+cap) with a depth-stamp array at buffer+2*cap; each
// neighbour is accepted through the EEB0 clearance test (dir depends on the
// parent's stamp). Stops and recurses into FF9C when a node with a conflicting
// stamp is reached, or returns 0 when the depth limit is exceeded.
extern "C" s32 func_804804DC__17UnkClass_8047E110Fv(UnkClass_8047E110* self, u16* out, s32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    ScnManagerLayout& m = *(ScnManagerLayout*)self;
    u32 count = *m.field_0x0;
    u32 cap;
    if (count >= 0x1F4) {
        cap = count >> 1;
    } else {
        cap = count;
    }
    u16* buffer = lbl_eu_80658540.buffer;
    u16* cur = buffer;
    u16* next = buffer + cap;
    u16* bufB = buffer + 2 * cap;
    u32* bits = (u32*)func_804B5A68();
    memset(bits, 0, (count / 32 + 1) * 4);

    bits[arg8 >> 5] |= 1 << (arg8 & 31);
    bits[arg7 >> 5] |= 1 << (arg7 & 31);
    buffer[0] = arg7;
    buffer[1] = arg8;
    bufB[arg7] = 0;
    bufB[arg8] = 0x2710;

    s32 nodeCount = 2;
    s32 depth = 1;
    while (nodeCount != 0) {
        s32 todo = nodeCount;
        u16* f = cur;
        u16* nxt = next;
        s32 foundA = 0;
        s32 foundB = 0;
        nodeCount = 0;
        for (s32 o = 0; o < todo; ++o) {
            u16 nodeId = f[o];
            const ScnWalkNode& node = m.nodes[nodeId];
            u16 nextStamp = bufB[nodeId] + 1;
            u16 cnt = m.edges[node.edgeOffset];
            const u16* nbs = &m.edges[node.edgeOffset + 1];
            if (nextStamp >= 0x2710) {
                for (s32 e = 0; e < cnt; ++e) {
                    u16 nb = nbs[e];
                    if (!(bits[nb >> 5] & (1 << (nb & 31)))) {
                        if (func_8047EEB0__17UnkClass_8047E110Fv(self, &m.nodes[nb], nodeId, 1)) {
                            nodeCount++;
                            foundA++;
                            bufB[nb] = nextStamp;
                            *nxt++ = nb;
                            bits[nb >> 5] |= 1 << (nb & 31);
                        }
                    } else if (bufB[nb] < 0x2710) {
                        if (depth * 2 + 1 >= arg6) {
                            return 0;
                        }
                        *out = func_8047FF9C__17UnkClass_8047E110Fv(self, arg5, bufB, bits, arg7, arg8, nodeId, depth);
                        return 1;
                    }
                }
            } else {
                for (s32 e = 0; e < cnt; ++e) {
                    u16 nb = nbs[e];
                    if (!(bits[nb >> 5] & (1 << (nb & 31)))) {
                        if (func_8047EEB0__17UnkClass_8047E110Fv(self, &m.nodes[nb], nodeId, 0)) {
                            nodeCount++;
                            foundB++;
                            bufB[nb] = nextStamp;
                            *nxt++ = nb;
                            bits[nb >> 5] |= 1 << (nb & 31);
                        }
                    } else if (bufB[nb] >= 0x2710) {
                        if (depth * 2 + 1 >= arg6) {
                            return 0;
                        }
                        *out = func_8047FF9C__17UnkClass_8047E110Fv(self, arg5, bufB, bits, arg7, arg8, nodeId, depth);
                        return 1;
                    }
                }
            }
        }
        if (foundA == 0) {
            return 0;
        }
        if (foundB == 0) {
            return 0;
        }
        u16* swap = cur;
        cur = next;
        next = swap;
        depth++;
    }
    return 0;
}

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

// func_80480EF0 -- scan a node's neighbour list for `value` (direct edge test).
// Retail compares SIGNED (cmp, not cmpl): the u16 neighbor and the value are
// compared as promoted ints, so negative-represented words (>= 0x8000) match.
extern "C" s32 func_80480EF0__17UnkClass_8047E110Fv(UnkClass_8047E110* self, u32 nodeIndex, int value) {
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
    if (a != lbl_eu_8066A8AC || b != lbl_eu_8066A8AC) {
        m.field_0x24[0] = b;
        m.field_0x24[1] = a;
        m.field_0x2C = v->field_0x0;
        m.field_0x30 = v->field_0x4;
        m.field_0x34 = v->field_0x8;
        m.field_0x38 |= 1;
    } else {
        m.field_0x38 &= ~1;
    }
    m.field_0x20 = m.field_0x1C + c;
}

void UnkClass_8047E110::func_80481074() {}

// func_804812D8 -- node-vs-box test: the node's grid cell (scaled by
// lbl_eu_8066A898) must lie inside the walk box XZ range, and its height
// must be within the 0x20 clearance of the box min-Y.
extern "C" s32 func_804812D8__17UnkClass_8047E110Fv(UnkClass_8047E110* self, const ScnWalkNode* node) {
    ScnManagerLayout& m = *(ScnManagerLayout*)self;
    if (node->reserved8 & 2) return 0;

    f32 gx = lbl_eu_8066A898 * (f32)node->x;
    if (gx > m.boxMax.x) return 0;
    f32 gx2 = lbl_eu_8066A898 * (f32)(node->x + node->width + 1);
    if (gx2 < m.boxMin.x) return 0;
    f32 gz = lbl_eu_8066A898 * (f32)node->z;
    if (gz > m.boxMax.z) return 0;
    f32 gz2 = lbl_eu_8066A898 * (f32)(node->z + node->depth + 1);
    if (gz2 < m.boxMin.z) return 0;

    if (ml::math::abs(m.boxMin.y - node->yPos) >= m.field_0x20) return 0;
    return 1;
}

void UnkClass_8047E110::func_804813E8() {}

void UnkClass_8047E110::func_804814DC() {}

// func_8048163C -- build the walk box (half-size lbl_eu_8066A890) around `v`
// into 0x48-0x5C, reset the 0x90 counter and refresh the reference block at
// 0x9C from the data global.
extern "C" void func_8048163C__17UnkClass_8047E110Fv(UnkClass_8047E110* self, const ml::CVec3* v) {
    ScnManagerLayout& m = *(ScnManagerLayout*)self;
    m.field_0x90 = 0;
    m.boxMin.y = v->y;
    m.boxMax.y = v->y;
    m.boxMin.x = v->x - lbl_eu_8066A890;
    m.boxMin.z = v->z - lbl_eu_8066A890;
    m.boxMax.x = lbl_eu_8066A890 + v->x;
    m.boxMax.z = lbl_eu_8066A890 + v->z;
    // Reference block copy: pointer-increment form lets MWCC fold the address
    // addi into the first load (retail lwzu); load/store order follows retail.
    u32 v0;
    const u32* src = &lbl_eu_8056DC74.field_0x0;
    v0 = *src++;
    m.field_0x9C.field_0x4 = *src++;
    m.field_0x9C.field_0x0 = v0;
    m.field_0x9C.field_0x8 = *src;
}

// func_8048169C -- build the walk box from two corner points (min/max), then
// compute the box diagonal (0x60-0x68) and its XZ slopes (0x88 = dz/dx,
// 0x8C = dx/dz), reset the 0x90 counter and refresh the reference block at
// 0x9C from the data global.
extern "C" void func_8048169C__17UnkClass_8047E110Fv(UnkClass_8047E110* self, const ml::CVec3* min, const ml::CVec3* max) {
    ScnManagerLayout& m = *(ScnManagerLayout*)self;

    m.field_0x90 = 0;

    // Copy of the two 12-byte corners (retail lwz/stw pairs).
    m.boxMin = *min;
    m.boxMax = *max;

    // Paired-single vector difference (ml::CVec3 operator-), then the result
    // words go to 0x60-0x68.
    ml::CVec3 diff = m.boxMax - m.boxMin;
    *(ScnVecWords*)&m.diag = *(const ScnVecWords*)&diff;

    m.field_0x8C = lbl_eu_8066A8AC;
    m.field_0x88 = lbl_eu_8066A8AC;
    if (m.diag.x != lbl_eu_8066A8AC) m.field_0x88 = m.diag.z / m.diag.x;
    if (m.diag.z != lbl_eu_8066A8AC) m.field_0x8C = m.diag.x / m.diag.z;
    // Reference block copy: pointer-increment form lets MWCC fold the address
    // addi into the first load (retail lwzu); load/store order follows retail.
    u32 v0;
    const u32* src = &lbl_eu_8056DC80.field_0x0;
    v0 = *src++;
    m.field_0x9C.field_0x4 = *src++;
    m.field_0x9C.field_0x0 = v0;
    m.field_0x9C.field_0x8 = *src;
}

void UnkClass_8047E110::func_80481790() {}

// func_804819AC -- test bit 1 of the node flag word (offset 0x8); the walker
// skips nodes whose closed/blocked bit is set.
extern "C" s32 func_804819AC__17UnkClass_8047E110Fv(UnkClass_8047E110* self, u32 nodeIndex) {
    ScnManagerLayout& m = *(ScnManagerLayout*)self;
    return (m.nodes[nodeIndex].reserved8 & 2) != 0;
}

void UnkClass_8047E110::func_804819C4() {}
