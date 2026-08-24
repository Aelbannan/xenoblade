// Translation unit: monolib/src/scn/UnkClass_8047E110
// Unknown scene class implementation.
//
// Status: Only sinit_80481E68 is FULL_MATCH (empty .ctors hook, 4 bytes).
//         Remaining 29 functions are NOT_STARTED stubs.

#include "libs/monolib/src/scn/UnkClass_8047E110.hpp"
#include <monolib/math/CVec3.hpp>
#include <math.h>
#include <string.h>

using ml::CVec3;

// scratch bitmap allocator (coli unit); retail symbol is C-linkage
extern "C" void* func_804B5A68();

// stride-8 threshold slot overlaid at 0x24 by func_8047EEB0
struct ScnSlot8 {
    f32 value;
    f32 pad;
};

// Manager field view (retail offsets) used by the accessor methods below.
struct ScnManagerLayout {
    u32 field_0x0;         // 0x00
    ScnWalkNode* nodes;    // 0x04 node pool
    u32 field_0x8;         // 0x08 quantized waypoint pool
    u32 field_0xC;         // 0x0C stride-10 record pool
    u32 field_0x10;        // 0x10 (retail loads edges from 0x14 - keep the hole)
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
    u32 field_0x3C;        // 0x3C bounds block pointer
    u32 field_0x40;        // 0x40 kd-tree node pool
    u32 field_0x44;        // 0x44 projection slot pool
    f32 field_0x48;        // 0x48 box minX
    f32 field_0x4C;        // 0x4C box minY
    f32 field_0x50;        // 0x50 box minZ
    f32 field_0x54;        // 0x54 box maxX
    f32 field_0x58;        // 0x58 box maxY
    f32 field_0x5C;        // 0x5C box maxZ
    CVec3 dir;             // 0x60 travel direction
    f32 field_0x6C;        // 0x6C contact pos x
    f32 field_0x70;        // 0x70 contact pos y
    f32 field_0x74;        // 0x74 contact pos z
    u8 field_0x78[4];      // 0x78
    f32 field_0x7C;        // 0x7C
    f32 field_0x80;        // 0x80
    f32 field_0x84;        // 0x84
    f32 field_0x88;        // 0x88 x-portal threshold scale
    f32 field_0x8C;        // 0x8C z-portal threshold scale
    u16 field_0x90;        // 0x90
    u8 field_0x92[2];      // 0x92
    u32 field_0x94;        // 0x94 waypoint table pointer
    u32 field_0x98;        // 0x98 current table cursor
    ScnPtmf field_0x9C;    // 0x9C..0xA7
};

// Data-global imports referenced by the accessors below (retail names).
// A890/A8A0 are defined in this TU (see tools/coop/lbls_exclusions.json):
// planting the conversion magic under its retail name lets MWCC's literal
// pool unify the builtin int->float conversions onto it.
const f32 lbl_eu_8066A890 = 1.0f; // box half-size / clearance radius
extern const f32 lbl_eu_8066A8B0; // waypoint min-distance epsilon
extern const f32 lbl_eu_8066A8B4; // waypoint table constant
extern const f32 lbl_eu_8066A8BC; // forward-projection threshold
// nw4r db Warning rodata strings
extern const char lbl_eu_80526300[];
extern const char lbl_eu_80526324[];
extern Fe48Table lbl_eu_80658540; // walk-table locator block (+0x8 pointer)
extern u32 lbl_eu_8056DC68[3];    // default reference block (defined below)
// Walk-box waypoint table (3 x 12-byte blocks, initialized on first use by
// func_8047F658) and its init-once flag. Defined in UnkClass_8047CD0C.cpp.
extern u8 lbl_eu_80658608[0x30];
extern s8 lbl_eu_806658D0[8]; // init-once flag (read as signed char)
extern ScnPtmf lbl_eu_8056DC74;   // default 12-byte reference block

// Retail s16/s32->f32 conversion magic (lbl_eu_8066A8A0 = 0x4330000080000000),
// owned by this TU so the builtin conversions resolve to the retail name.
extern const f64 lbl_eu_8066A8A0 = 4503601774854144.0;
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

// Same conversion but kept in double precision so MWCC can fuse the
// following cell-size multiply into a single fmuls/fmadds.
static inline f64 s32ToF64_A8A0(s32 v) {
    F64Conv_A8A0 c;
    c.w[0] = 0x43300000u;
    c.w[1] = (u32)v ^ 0x80000000u;
    return c.d - lbl_eu_8066A8A0;
}

// Scaled node-coordinate conversion: MWCC's builtin int->float path subtracts
// the 0x4330000080000000 magic (unified onto lbl_eu_8066A8A0) with single
// rounding, then the cell-size multiply contracts into one fmuls.
static inline f32 scaleNodeCoord(s32 v) {
    return (f32)(lbl_eu_8066A898 * v);
}

// Real-signature views of the func_8047E1B0 / func_8047E390 member stubs
// (retail Fv mangling; signatures recovered from caller register setup).
extern "C" s32 func_8047E1B0__17UnkClass_8047E110Fv(UnkClass_8047E110* self, const ml::CVec3* pos);
extern "C" s32 func_8047E390__17UnkClass_8047E110Fv(UnkClass_8047E110* self, const ml::CVec3* pos, s32 index, f32 clearance);
extern "C" s32 func_8047EAD4__17UnkClass_8047E110Fv(
    UnkClass_8047E110* self, u16* out, s32 a, s32 b, s32 mode);
extern "C" s32 func_8047F730__17UnkClass_8047E110Fv(
    UnkClass_8047E110* self, u16* out, s32 count1, s32 count2);
extern "C" bool func_8047EEB0__17UnkClass_8047E110Fv(UnkClass_8047E110* self,
    const WalkQueryInfo* info, u16 value, s32 slotSel);

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

// func_8047E110 -- rebase-copy a spawn descriptor into this manager: raw
// offsets in the descriptor are converted to absolute pointers, the two
// distance fields are summed with the clearance radius, the referenced sub-
// block pointer is stored and the null member-pointer slot is filled.
// __ptmf_null viewed as a u32 triplet (see CREvtModelMap ptmf-copy shape):
// the post-increment walk makes MWCC fold the low half into a single
// lwzu @l at first use instead of an eager addi.
extern const u32 __ptmf_null[3];

extern "C" void func_8047E110__17UnkClass_8047E110Fv(UnkClass_8047E110* self,
    WalkSpawnDesc* desc) {
    // null member-pointer slot: triplet copy; elem0 through a temp stored
    // late (retail keeps it across the [4]-slot store), elem1/elem2 direct
    u32 w0;
    const u32* src = __ptmf_null;
    self->field_0x0 = (u32)desc;
    // clearance radius: loaded up front so the f-sum below reuses it
    f32 clearance = lbl_eu_8066A890;
    self->nodes = (ScnWalkNode*)((u32)desc + desc->offNodes);
    self->field_0x8 = (u32)desc + desc->offEdges;
    self->field_0xC = (u32)desc + desc->offField14;
    self->field_0x10 = (u32)desc + desc->offField24;
    self->edges = (u16*)((u32)desc + desc->offField1C);
    self->field_0x18 = desc->field_0x2C;
    self->field_0x20 = self->field_0x1C = clearance + desc->field_0x30;
    const WalkDescRef* ref = (const WalkDescRef*)((u32)desc + desc->offRef);
    self->field_0x3C = (u32)ref;
    self->field_0x40 = (u32)desc + ref->relField1C;
    self->field_0x44 = (u32)desc + ref->relField18;
    w0 = *src++;
    self->callback.field_0x4 = *src++;
    self->callback.field_0x0 = w0;
    u32 w2 = *src++;
    self->callback.field_0x8 = w2;
}

// func_8047E1B0 -- point-in-walk-region lookup: validate the position against
// the bounds block pointed to by 0x3C, then walk the kd-tree (node pool at
// 0x40, projection slots at 0x44, edge lists at 0x14) down to a leaf and scan
// its quantized waypoint rects for one containing the point. Returns the entry
// index or -1.
extern "C" s32 func_8047E1B0__17UnkClass_8047E110Fv(UnkClass_8047E110* self,
    const ml::CVec3* pos) {
    ScnManagerLayout& m = *(ScnManagerLayout*)self;
    const f32* bounds = (const f32*)m.field_0x3C;
    if (!(bounds[1] >= pos->x))
        return -1;
    if (!(pos->x >= bounds[2]))
        return -1;
    if (!(bounds[3] >= pos->y))
        return -1;
    if (!(pos->y >= bounds[4]))
        return -1;

    ScnSlot8* slots = (ScnSlot8*)m.field_0x44;
    const u16* list = m.edges + ((const u32*)m.field_0x3C)[5];
    const ScnKdNode* node = (const ScnKdNode*)m.field_0x40;
    for (;;) {
        const ScnKdNode& nd = *node;
        const ScnSlot8& sa = slots[nd.slotA];
        const ScnSlot8& sb = slots[nd.slotB];
        f32 dx = pos->x - sa.value;
        f32 dz = pos->z - sa.pad;
        s32 ix = (s32)(dx * sb.value);
        s32 iz = (s32)(dz * sb.pad);
        if (ix < 0)
            return -1;
        if (ix >= (nd.dims >> 4))
            return -1;
        if (iz < 0)
            return -1;
        if (iz >= (nd.dims & 0xF))
            return -1;
        s32 cell = ix * (nd.dims & 0xF) + iz;
        u16 e = list[nd.edgeBase + cell];
        if (!((nd.mask >> cell) & 1)) {
            // internal cell: child link packed in the edge-pool word (bit 15
            // is a flag, low 12 bits select the stride-8 child slot)
            if (e == 0)
                return -1;
            node = (const ScnKdNode*)((const u8*)node + (e & 0xFFF) * 8);
            continue;
        }
        // leaf: scan the waypoint rect list for a hit
        u16 cnt = list[nd.edgeBase + cell + 1];
        const u16* p = &list[nd.edgeBase + cell + 2];
        for (u16 k = 0; k < cnt; ++k) {
            u16 ent = p[k];
            const ScnWpQ& wp = ((const ScnWpQ*)m.field_0x8)[ent];
            f32 fy = wp.q0; // GQR5-dequantized halfword reads
            f32 fz = wp.q1;
            f32 c = m.field_0x18;
            if (fy + c > pos->x && fy <= pos->x && fz + c > pos->z
                && fz <= pos->z)
                return ent;
        }
        return -1;
    }
}

// func_8047E390 -- neighbour search around a walk cell: scan the second
// candidate list of record `index` (stride-10 pool at 0x8), apply the box/
// flag visibility gate, expand each node's s16 rect by `clearance` and track
// the closest node to `pos`, with a priority scheme for flagged nodes.
extern "C" s32 func_8047E390__17UnkClass_8047E110Fv(UnkClass_8047E110* self,
    const ml::CVec3* pos, s32 index, f32 clearance) {
    ScnManagerLayout& m = *(ScnManagerLayout*)self;
    f32 posX = pos->x;
    const u16* rec = (const u16*)(m.field_0x8 + index * 12);
    f32 posZ = pos->z;
    f32 pxLo = posX - clearance; // f6
    f32 pzLo = posZ - clearance; // f7
    f32 pxHi = posX + clearance; // f8
    f32 pzHi = posZ + clearance; // f9
    const ScnWalkNode* node = m.nodes;
    s32 count1 = rec[0];
    s32 count2 = rec[1];
    node += count1;
    s32 best = -1;                  // r7
    f32 bestDist = lbl_eu_8066A894; // f5
    s32 haveStrict = 1;             // r8: any strict candidate seen
    s32 haveMargin = 1;             // r9: any margin-only candidate seen
    s32 haveFlagged = 1;            // r10: no flagged strict take yet
    for (s32 i = 0; i < count2; ++i, ++node) {
        // visibility gate: box-active flag plus two manager/node flag pairs
        u32 flags = m.field_0x38;
        if (!(flags & 1)) {
            if (node->reserved8 & 2)
                continue;
        }
        if (!(flags & 2)) {
            if (node->reserved8 & 4)
                continue;
        } else if (!(flags & 4)) {
            if (node->reserved8 & 4)
                continue;
        }
        // rect expanded by clearance must contain pos
        f32 minX = lbl_eu_8066A898 * (f32)node->x;
        f32 minZ = lbl_eu_8066A898 * (f32)node->z;
        if (pxHi < minX)
            continue;
        if (pzHi < minZ)
            continue;
        f32 maxX = lbl_eu_8066A898 * (f32)(node->x + node->width + 1);
        f32 maxZ = lbl_eu_8066A898 * (f32)(node->z + node->depth + 1);
        if (pxLo >= maxX)
            continue;
        if (pzLo >= maxZ)
            continue;
        // vertical distance to the node plane must be within the threshold
        f32 dist = __fabs(pos->y - node->xPos); // f2
        if (dist >= m.field_0x20)
            continue;
        // priority update: strictly-inside candidates beat clearance-margin
        // ones; flagged nodes (bit 2, when both manager bits set) beat all
        if (pos->x >= minX && pos->z >= minZ && pos->x < maxX && pos->z < maxZ) {
            if (haveStrict) {
                bestDist = dist;
                best = i + count1;
                haveStrict = 0;
                haveMargin = 0;
                if ((flags & 6) == 6 && (node->reserved8 & 4))
                    haveFlagged = 0;
            } else if ((flags & 6) != 6 || !(node->reserved8 & 4)) {
                if (haveFlagged) {
                    if (dist < bestDist) {
                        bestDist = dist;
                        best = i + count1;
                    }
                }
            } else if (haveFlagged) {
                bestDist = dist;
                best = i + count1;
                haveFlagged = 0;
            } else if (dist < bestDist) {
                bestDist = dist;
                best = i + count1;
            }
        } else {
            if (!haveStrict)
                continue;
            if (haveMargin) {
                bestDist = dist;
                best = i + count1;
                haveMargin = 0;
            } else if (dist < bestDist) {
                best = i + count1;
                bestDist = dist;
            }
        }
    }
    return best;
}

// Real-signature views of the func_8047E1B0 / func_8047E390 member stubs
// (retail Fv mangling; signatures recovered from caller register setup).
// func_8047E62C -- two-stage segment solve: E1B0 (point-in-box) then E390
// (neighbour search with clearance); both results stored, 1 on success.
extern "C" s32 func_8047E62C__17UnkClass_8047E110Fv(UnkClass_8047E110* self, u32* outSeg,
    u32* outNode, const ml::CVec3* pos, f32 clearance) {
    s32 seg = ::func_8047E1B0__17UnkClass_8047E110Fv(self, pos);
    *outSeg = seg;
    if (seg != -1) {
        s32 node = ::func_8047E390__17UnkClass_8047E110Fv(self, pos, seg, clearance);
        *outNode = node;
        if (node != -1)
            return 1;
    }
    return 0;
}

// func_8047E6C4 -- resolve the walk node under `pos`: test node `idx`'s cell
// (expanded by clearance, then strictly), otherwise scan idx's edge list for
// the closest visible flagged neighbour and re-probe through func_8047E1B0.
extern "C" s32 func_8047E6C4__17UnkClass_8047E110Fv(UnkClass_8047E110* self,
    u32* outValue, u32* outNode, s32 value, s32 idx, const ml::CVec3* pos,
    f32 clearance) {
    ScnManagerLayout& m = *(ScnManagerLayout*)self;
    const ScnWalkNode* nodes = m.nodes;
    const ScnWalkNode& node = nodes[idx];
    f32 posX = pos->x;
    s16 x0 = node.x;
    f32 pxHi = posX + clearance;
    s16 z0 = node.z;
    f32 pzHi = pos->z + clearance;
    f32 minX = lbl_eu_8066A898 * (f32)x0;
    f32 minZ = lbl_eu_8066A898 * (f32)z0;
    f32 maxX = lbl_eu_8066A898 * (f32)(x0 + node.width + 1);
    f32 maxZ = lbl_eu_8066A898 * (f32)(z0 + node.depth + 1);
    f32 dist0 = __fabs(pos->y - node.xPos);
    s32 looseOk = 0;
    // expanded-cell test (clearance margin)
    if (pxHi >= minX && pzHi >= minZ && pxHi < maxX && pzHi < maxZ
        && dist0 <= m.field_0x20) {
        if ((node.reserved8 & 2) && (m.field_0x38 & 1)) {
            *outValue = value;
            *outNode = idx;
            return 1;
        }
        looseOk = 1;
    }
    // strict-cell test (no clearance)
    if (pos->x >= minX && pos->z >= minZ && pos->x < maxX && pos->z < maxZ
        && dist0 <= m.field_0x20) {
        *outValue = value;
        *outNode = idx;
        return 1;
    }
    // neighbour scan: pick the best visible neighbour
    s32 best = -1;
    u16 cnt = m.edges[node.edgeOffset];
    const u16* ep = &m.edges[node.edgeOffset + 1];
    for (u16 k = 0; k < cnt; ++k) {
        const ScnWalkNode& nb = m.nodes[ep[k]];
        u32 flags = m.field_0x38;
        // visibility gate (same shape as func_8047E390)
        if (!(flags & 1)) {
            if (nb.reserved8 & 2)
                continue;
        }
        if (!(flags & 2)) {
            if (nb.reserved8 & 4)
                continue;
        } else if (!(flags & 4)) {
            if (nb.reserved8 & 4)
                continue;
        }
        f32 nMinX = lbl_eu_8066A898 * (f32)nb.x;
        f32 nMinZ = lbl_eu_8066A898 * (f32)nb.z;
        f32 nMaxX = lbl_eu_8066A898 * (f32)(nb.x + nb.width + 1);
        f32 nMaxZ = lbl_eu_8066A898 * (f32)(nb.z + nb.depth + 1);
        if (pxHi < nMinX || pzHi < nMinZ || pxHi >= nMaxX || pzHi >= nMaxZ)
            continue;
        if (__fabs(pos->y - nb.xPos) > m.field_0x20)
            continue;
        if ((nb.reserved8 & 2) && ep[k] != 0) {
            // flagged neighbour: distance-gated toward the reference plane
            f32 diff = __fabs(node.xPos - nb.xPos);
            if (node.xPos >= nb.xPos) {
                if (m.field_0x24 >= diff)
                    best = ep[k];
            } else {
                if (diff <= m.field_0x28)
                    best = ep[k];
            }
        } else {
            if (pos->x >= nMinX && pos->z >= nMinZ && pos->x < nMaxX
                && pos->z < nMaxZ
                && __fabs(pos->y - nb.xPos) <= m.field_0x20)
                best = ep[k];
        }
    }
    if (best != -1) {
        s32 hit = ::func_8047E1B0__17UnkClass_8047E110Fv(self, pos);
        *outValue = hit;
        if (hit != -1) {
            *outNode = best;
            return 1;
        }
    }
    if (looseOk) {
        *outValue = value;
        *outNode = idx;
        return 1;
    }
    return 0;
}

// func_8047EAD4 -- build a candidate node list for record pair (a,b). When
// node b's low flag is set, filter the edge list selected by walk-table record
// a: keep entries whose group byte (found by locating b in the candidate
// slots) matches a's slot for the requested side. Otherwise breadth-first
// expand from b through EEB0-vetted neighbours until a flagged node yields a
// filtered sublist.
extern "C" s32 func_8047EAD4__17UnkClass_8047E110Fv(
    UnkClass_8047E110* self, u16* out, s32 a, s32 b, s32 mode) {
    UnkClass_8047E110& m = *self;
    const ScnWalkNode& nb = m.nodes[b];
    const ScnWalkNode* nodes = m.nodes;
    // locate the edge list via walk-table record a (+4 halfword)
    u16 edgeIdx = ((const u16*)m.field_0x8)[a * 6 / 2 + 2];
    u16 cnt = m.edges[edgeIdx];
    const u16* p = &m.edges[edgeIdx + 1];
    const u8* pool = (const u8*)m.field_0xC;
    if (nb.reserved8 & 1) {
        // direct filter: find which candidate slot holds b, take its group byte
        u8 grp = 0;
        for (u16 k = 0; k < cnt; ++k) {
            const u16* rc = (const u16*)(pool + p[k] * 10);
            if (rc[0] == (u16)b) {
                grp = ((const u8*)rc)[8];
                break;
            }
            if (rc[1] == (u16)b) {
                grp = ((const u8*)rc)[9];
                break;
            }
        }
        s32 count = 0;
        for (u16 k = 0; k < cnt; ++k) {
            u16 e = p[k];
            const u16* rc = (const u16*)(pool + e * 10);
            if ((rc[2] == (u16)a && ((const u8*)rc)[8] == grp)
                || (rc[3] == (u16)a && ((const u8*)rc)[9] == grp)) {
                out[count++] = e;
            }
        }
        return count;
    }
    // BFS path: expand from b, collecting the first flagged node's matches
    Fe48Table& tbl = lbl_eu_80658540;
    u32* bitmap = (u32*)::func_804B5A68();
    memset(bitmap, 0, (((const u16*)m.field_0x8)[a * 3 + 1] >> 5) * 4);
    u16* work = (u16*)tbl.field_0x8;
    u16* frontier = work + cnt;
    u16* other = frontier;
    frontier[0] = (u16)b;
    bitmap[b >> 5] |= 1 << (b & 31);
    s32 curCount = 1;
    while (curCount != 0) {
        const u16* fr = frontier;
        s32 prevCount = curCount;
        curCount = 0;
        for (s32 i = 0; i < prevCount; ++i) {
            u16 id = *fr++;
            const ScnWalkNode& node = nodes[id];
            u16 ecnt = m.edges[node.edgeOffset];
            const u16* ep = &m.edges[node.edgeOffset + 1];
            for (s32 k = 0; k < ecnt; ++k) {
                u16 nEnt = ep[k];
                if (bitmap[nEnt >> 5] & (1 << (nEnt & 31)))
                    continue;
                if (!::func_8047EEB0__17UnkClass_8047E110Fv(self,
                        (const WalkQueryInfo*)&nodes[nEnt], id, mode))
                    continue;
                if (nodes[nEnt].reserved8 & 1) {
                    // flagged: run the group filter for this subtree
                    const u16* rc0 = (const u16*)(pool + nEnt * 10);
                    u8 grp = 0;
                    for (u16 j = 0; j < ecnt; ++j) {
                        const u16* rc = (const u16*)(pool + ep[j] * 10);
                        if (rc[0] == nEnt) {
                            grp = ((const u8*)rc)[8];
                            break;
                        }
                        if (rc[1] == nEnt) {
                            grp = ((const u8*)rc)[9];
                            break;
                        }
                    }
                    s32 count = 0;
                    for (u16 j = 0; j < ecnt; ++j) {
                        u16 e = ep[j];
                        const u16* rc = (const u16*)(pool + e * 10);
                        if ((rc[2] == (u16)a && ((const u8*)rc)[8] == grp)
                            || (rc[3] == (u16)a && ((const u8*)rc)[9] == grp)) {
                            out[count++] = e;
                        }
                    }
                    return count;
                }
                *other++ = nEnt;
                ++curCount;
                bitmap[nEnt >> 5] |= 1 << (nEnt & 31);
            }
        }
        u16* tmp = frontier;
        frontier = other;
        other = tmp;
    }
    return 0;
}

// Stride-8 threshold slot overlay of the region starting at 0x24, indexed by
// func_8047EEB0.
struct ScnSlotView {
    u8 pad[0x24];
    ScnSlot8 slots8[4];
};

// func_8047EEB0 -- neighbour scan with per-slot distance thresholds. When the
// walk box is inactive only the flag bits decide; otherwise every neighbour of
// info's edge list (except `value`) must pass a distance test whose threshold
// slot is selected by `slotSel`.
extern "C" bool func_8047EEB0__17UnkClass_8047E110Fv(UnkClass_8047E110* self,
    const WalkQueryInfo* info, u16 value, s32 slotSel) {
    ScnManagerLayout& m = *(ScnManagerLayout*)self;
    ScnSlotView& sv = *(ScnSlotView*)self;
    if (!(m.field_0x38 & 1)) {
        if (info->field_0x8 & 2)
            return false;
    } else {
        if (!(info->field_0x8 & 2))
            goto common;
        s32 idx = info->field_0x4;
        u16 count = m.edges[idx * 2];
        if (count == 0)
            return false;
        const u16* p = &m.edges[idx * 2 + 1];
        for (u16 i = 0; i < count; ++i, ++p) {
            u16 n = *p;
            if (n == value)
                continue;
            f32 dv = m.nodes[value].xPos;
            f32 dn = m.nodes[n].xPos;
            if (dv >= dn) {
                if (dv == dn)
                    return true;
                // equal distances always pass; otherwise use stride-4 slot
                if ((&m.field_0x24)[slotSel] >= dv - dn)
                    return true;
                return false;
            } else {
                // stride-8 slot table, index derived from the selector mask
                if (sv.slots8[__cntlzw(slotSel) >> 3].value >= dn - dv)
                    return true;
                return false;
            }
        }
        return false;
    }
common:
    if (m.field_0x38 & 2)
        return true;
    if (!(info->field_0x8 & 4))
        return true;
    return false;
}

// func_8047EFBC -- expand a walk path from `seed` in two directions: each
// pass picks the neighbour whose table value (flag bit 15 stripped) beats the
// running threshold, carries the pass flag (0x8000 backwards / 0x4000
// forwards), matches threshold groups and is already marked in the scratch
// visit bitmap, writing ids into consecutive output slots.
extern "C" s32 func_8047EFBC__17UnkClass_8047E110Fv(UnkClass_8047E110* self,
    u16* out, s32 seed) {
    ScnManagerLayout& m = *(ScnManagerLayout*)self;
    s32 dir = 0;
    u32* bitmap = (u32*)::func_804B5A68();
    const u16* tbl = (const u16*)m.field_0x94;
    u16 v = tbl[seed];
    s32 start;
    s32 end;
    u16 thrA;
    u16 thrB;
    if (v & 0x8000) {
        start = dir - 1;
        end = dir;
        thrA = v & 0x3FFF;
        thrB = thrA + 1;
    } else {
        start = dir + 1;
        end = dir - 1;
        thrB = v & 0x3FFF;
        thrA = thrB + 2;
    }
    if ((u32)(start + end + 1)
        >= ((const Fe48TableExt*)&lbl_eu_80658540)->field_0x14)
        return 0;
    out[start] = (u16)seed;
    u16 best = (u16)seed;
    const ScnRec10* recs = (const ScnRec10*)m.field_0xC;
    const u16* thrPtr = &thrA;
    const s32* cursorPtr = &start; // walks start, then end
    for (s32 pass = 0; pass < 2; ++pass) {
        s32 step = (pass == 0) ? -1 : 1;
        u16 flagBit = (pass == 0) ? 0x8000 : 0x4000;
        s32 iters = *cursorPtr++;
        u16 bestThr = *thrPtr++;
        u16* dst = &out[start + step];
        for (s32 it = 0; it < iters; ++it) {
            const ScnRec10& rec = recs[best];
            for (s32 axis = 0; axis < 2; ++axis) {
                const ScnBlockC& blk =
                    ((const ScnBlockC*)m.field_0x8)[rec.coord[axis]];
                u16 cnt = m.edges[blk.edgeBase];
                const u16* p = &m.edges[blk.edgeBase + 1];
                for (u16 k = 0; k < cnt; ++k) {
                    u16 nEnt = p[k];
                    if (nEnt == best)
                        continue;
                    const ScnRec10& rn = recs[nEnt];
                    s32 cc = rec.coord[axis];
                    s32 nc = rn.coord[axis];
                    // sign trick: 1 iff the axis coordinates differ
                    u32 differ = (((u32)(cc - nc)) | ((u32)(nc - cc))) >> 31;
                    if (rec.grp[axis] != rn.grp[differ])
                        continue;
                    // must already be visited (bitmap bit set)
                    if (!(bitmap[(nEnt >> 5) & 0x7FF] & (1u << (nEnt & 31))))
                        continue;
                    u16 tv = tbl[nEnt];
                    if (!(tv & flagBit))
                        continue;
                    u16 tval = tv & 0x3FFF;
                    if (tval >= bestThr)
                        continue;
                    *dst = nEnt;
                    bestThr = tval;
                    best = nEnt;
                    break;
                }
            }
            dst += step;
        }
    }
    return start + end + 1;
}

// func_8047F214 -- compute the walking direction from `pos` toward node idxB
// (relative to node idxA) into `out`, y forced to zero. When idxB is in
// idxA's edge list the direction aims at the midpoint of the shared portal
// edge (whichever side matches), otherwise straight at idxB's center.
extern "C" void func_8047F214__17UnkClass_8047E110Fv(UnkClass_8047E110* self,
    ml::CVec3* out, const ml::CVec3* pos, s32 idxA, s32 idxB) {
    ScnManagerLayout& m = *(ScnManagerLayout*)self;
    const ScnWalkNode& na = m.nodes[idxA];
    const ScnWalkNode& nb = m.nodes[idxB];
    // rect of B (cell size scaled; max = min + cell*(extent+1))
    f32 minXb = scaleNodeCoord(nb.x);
    f32 minZb = scaleNodeCoord(nb.z);
    f32 maxXb = lbl_eu_8066A898 * s32ToF_A8A0(nb.width + 1) + minXb;
    f32 maxZb = lbl_eu_8066A898 * s32ToF_A8A0(nb.depth + 1) + minZb;

    f32 xSlot;
    f32 zSlot;
    // search for B in A's edge list
    u16 cnt = m.edges[na.edgeOffset];
    const u16* ep = &m.edges[na.edgeOffset + 1];
    s32 found = 0;
    for (u16 i = 0; i < cnt; ++i) {
        if (*ep++ == idxB) {
            found = 1;
            break;
        }
    }

    if (!found) {
        // not adjacent: aim at B's center
        xSlot = lbl_eu_8066A898 * (maxXb + minXb);
        zSlot = lbl_eu_8066A898 * (maxZb + minZb);
    } else {
        // rect of A + vertical midpoint term
        f32 minXa = scaleNodeCoord(na.x);
        f32 minZa = scaleNodeCoord(na.z);
        f32 maxXa = lbl_eu_8066A898 * s32ToF_A8A0(na.width + 1) + minXa;
        f32 maxZa = lbl_eu_8066A898 * s32ToF_A8A0(na.depth + 1) + minZa;
        f32 midY = lbl_eu_8066A898 * (na.xPos + nb.xPos);
        (void)midY;

        if (maxXa == minXb) {
            // shared edge on A's +x side: clamp the z overlap of both rects
            f32 hi = (maxZa >= maxZb) ? maxZb : maxZa;
            f32 lo = (minZa >= minZb) ? minZb : minZa;
            zSlot = lbl_eu_8066A898 * (hi + lo);
            if (pos->x > lo && pos->x < hi)
                xSlot = maxXb;
            else
                xSlot = maxXa;
        } else if (maxXb == minXa) {
            // shared edge on B's +x side
            f32 hi = (maxZa >= maxZb) ? maxZb : maxZa;
            f32 lo = (minZa >= minZb) ? minZb : minZa;
            zSlot = lbl_eu_8066A898 * (hi + lo);
            if (pos->x > lo && pos->x < hi)
                xSlot = minXb;
            else
                xSlot = maxXa;
        } else if (maxZa == minZb) {
            // shared edge on A's +z side: clamp the x overlap
            f32 hi = (maxXa >= maxXb) ? maxXb : maxXa;
            f32 lo = (minXa >= minXb) ? minXb : minXa;
            xSlot = lbl_eu_8066A898 * (hi + lo);
            if (pos->x > lo && pos->x < hi)
                zSlot = maxZb;
            else
                zSlot = maxZa;
        } else if (maxZb == minZa) {
            // shared edge on B's +z side
            f32 hi = (maxXa >= maxXb) ? maxXb : maxXa;
            f32 lo = (minXa >= minXb) ? minXb : minXa;
            xSlot = lbl_eu_8066A898 * (hi + lo);
            if (pos->x > lo && pos->x < hi)
                zSlot = minZb;
            else
                zSlot = maxZb;
        } else {
            xSlot = lbl_eu_8066A898 * (maxXb + minZa);
            zSlot = lbl_eu_8066A898 * (maxZb + maxZa);
        }
    }

    out->x = xSlot - pos->x;
    out->y = lbl_eu_8066A8AC;
    out->z = zSlot - pos->z;
    if (out->x == lbl_eu_8066A8AC && out->z == lbl_eu_8066A8AC) {
        *out = CVec3::zero;
    } else {
        f32 lenSq = out->x * out->x + out->y * out->y + out->z * out->z;
        if (lenSq == lbl_eu_8066A8AC)
            *out = CVec3::zero;
        else
            PSVECNormalize(*out, *out);
    }
}

// func_8047F658 -- fetch the walk-target point for node `idx`: validates the
// walk box, the node flag bits and a distance test against node `idx2`, then
// copies the selected 12-byte waypoint block into `out`. The 0x30-byte
// waypoint table is filled in on first use.
extern "C" s32 func_8047F658__17UnkClass_8047E110Fv(UnkClass_8047E110* self, ScnVecWords* out,
    s32 idx, s32 idx2) {
    UnkClass_8047E110& m = *self;
    const ScnWalkNode& node = m.nodes[idx];

    if ((m.field_0x38 & 1) && (node.reserved8 & 2) &&
        node.xPos - m.nodes[idx2].xPos >= lbl_eu_8066A8B0) {
        const ScnWalkNode* nodes = m.nodes;
        u16 flags = node.reserved8;
        if (!(s8)lbl_eu_806658D0[0]) {
            // waypoint table layout (f32 triples):
            //   block 0: {A8AC, A8AC, A8B4}  block 1: {A8AC, A8AC, A890}
            //   block 2: {A8B4, A8AC, A8AC}  block 3: {A890, A8AC, A8AC}
            const f32 vFlat = lbl_eu_8066A8AC;
            f32* wp = (f32*)lbl_eu_80658608;
            const f32 vDepth = lbl_eu_8066A8B4;
            const f32 vHalf = lbl_eu_8066A890;
            wp[0] = vFlat;
            wp[1] = vFlat;
            wp[2] = vDepth;
            wp[3] = vFlat;
            wp[4] = vFlat;
            wp[5] = vHalf;
            wp[6] = vDepth;
            wp[7] = vFlat;
            wp[8] = vFlat;
            wp[9] = vHalf;
            wp[10] = vFlat;
            wp[11] = vFlat;
            lbl_eu_806658D0[0] = 1;
        }

        // high nibble of the flag halfword selects the waypoint block
        s32 sel = (flags >> 12) & 0xF;
        const ScnVecWords* blocks = (const ScnVecWords*)lbl_eu_80658608;
        *out = blocks[sel];
        return 1;
    }
    return 0;
}

// func_8047F730 -- flood-fill the walk table from the FE48 candidate lists:
// the first `count1` entries of the 0x98 buffer are seeded with the 0x8000
// flag and `count2` more with 0x4000 (all marked visited in the scratch
// bitmap). Each pass expands every listed node's edge lists into the swap
// buffer, assigning child distances (parent + 1); when an expansion touches
// a node whose phase flag differs from its parent's, the opposite frontier
// was met and func_8047EFBC backfills the path. Returns its result, or 0.
extern "C" s32 func_8047F730__17UnkClass_8047E110Fv(
    UnkClass_8047E110* self, u16* out, s32 count1, s32 count2) {
    ScnManagerLayout& m = *(ScnManagerLayout*)self;
    u16* dist = (u16*)m.field_0x94;
    u32* bitmap = (u32*)::func_804B5A68();
    const WalkSpawnDesc* desc = (const WalkSpawnDesc*)m.field_0x0;
    u16* buf = (u16*)m.field_0x98;
    u16* other = buf + (desc->field_0x10 >> 1) + 10;
    memset(bitmap, 0, ((desc->field_0x10 >> 5) + 1) * 4);

    // seed phase flags: list one = 0x8000, list two = 0x4000
    u16* sp = buf;
    for (s32 i = 0; i < count1; ++i) {
        u16 v = *sp++;
        bitmap[v >> 5] |= 1 << (v & 31);
        dist[v] = 0x8000;
    }
    for (s32 i = 0; i < count2; ++i) {
        u16 v = *sp++;
        bitmap[v >> 5] |= 1 << (v & 31);
        dist[v] = 0x4000;
    }

    const ScnRec10* recs = (const ScnRec10*)m.field_0xC;
    const ScnBlockC* blocks = (const ScnBlockC*)m.field_0x8;
    s32 remaining = count1 + count2;
    u16* cur = buf;
    u16* nxt = other;
    while (remaining != 0) {
        s32 found = 0;
        u32 orFlags = 0;
        u16* dst = nxt;
        for (s32 j = 0; j < remaining; ++j) {
            u16 id = cur[j];
            const ScnRec10& rec = recs[id];
            for (s32 axis = 0; axis < 2; ++axis) {
                const ScnBlockC& blk = blocks[rec.coord[axis]];
                u16 cnt = m.edges[blk.edgeBase];
                const u16* p = &m.edges[blk.edgeBase + 1];
                for (u16 k = 0; k < cnt; ++k) {
                    u16 nEnt = p[k];
                    const ScnRec10& rn = recs[nEnt];
                    // sign trick: 1 iff the axis coordinates differ
                    s32 cc = rec.coord[axis];
                    s32 nc = rn.coord[axis];
                    u32 differ = (((u32)(cc - nc)) | ((u32)(nc - cc))) >> 31;
                    if (rec.grp[axis] != rn.grp[differ])
                        continue;
                    u32 word = nEnt >> 5;
                    u32 bit = 1u << (nEnt & 31);
                    if (bitmap[word] & bit) {
                        // already visited: extend the wavefront
                        ++found;
                        *dst++ = nEnt;
                        dist[nEnt] = dist[id] + 1;
                        orFlags |= dist[nEnt];
                    } else if ((dist[nEnt] ^ dist[id]) & 0x8000) {
                        // phases met: backfill the path
                        return ::func_8047EFBC__17UnkClass_8047E110Fv(self,
                            out, id);
                    }
                }
            }
        }
        if ((orFlags & 0xC000) != 0xC000)
            return 0;
        u16* tmp = cur;
        cur = nxt;
        nxt = tmp;
        remaining = found;
    }
    return 0;
}

// func_8047FE48 -- query the walk table: fetch two candidate lists via
// func_8047EAD4 (modes 0/1), report the first common entry, falling back to
// func_8047F730 when the lists share nothing.
extern "C" s32 func_8047FE48__17UnkClass_8047E110Fv(UnkClass_8047E110* self,
    u16* outResult, u16* outValue, s32 p1, s32 p2, s32 p3, s32 p4) {
    ScnManagerLayout& m = *(ScnManagerLayout*)self;
    Fe48Table* tbl = &lbl_eu_80658540;
    m.field_0x94 = tbl->field_0x8;
    m.field_0x98 = m.field_0x94 + ((const WalkSpawnDesc*)m.field_0x0)->field_0x10 * 2;
    s32 count1 = ::func_8047EAD4__17UnkClass_8047E110Fv(
        self, (u16*)m.field_0x98, p1, p2, 0);
    if (count1 == 0)
        return 0;
    s32 count2 = ::func_8047EAD4__17UnkClass_8047E110Fv(
        self, (u16*)(m.field_0x98 + count1 * 2), p3, p4, 1);
    if (count2 == 0)
        return 0;
    s32 i = 0;
    do {
        const u16* q = (const u16*)(m.field_0x98 + count1 * 2);
        s32 cnt = count2;
        if (cnt > 0) {
            do {
                u16 n = *q;
                if (((const u16*)m.field_0x98)[i] == n) {
                    *outValue = ((const u16*)m.field_0x98)[i];
                    *outResult = 1;
                    return 1;
                }
                ++q;
            } while (--cnt);
        }
    } while (++i < count1);
    if ((*outResult = (u16)::func_8047F730__17UnkClass_8047E110Fv(self, outValue, count1, count2)) == 0)
        return 0;
    return 1;
}

// func_8047FF9C -- seed a path table from two candidate start nodes: the node
// whose weight is below the 10000 cap seeds out[], its edge list fills the
// preceding slots backwards (lighter-weight neighbours only), then a forward
// walk appends heavy (>=10000) neighbours into successive slots. Edge entries
// carry a flag in bit 15.
extern "C" s32 func_8047FF9C__17UnkClass_8047E110Fv(UnkClass_8047E110* self,
    u16* out, const u16* weights, const u32* bits, s32, s32, s32 startA,
    s32 startB, s32 limit) {
    ScnManagerLayout& m = *(ScnManagerLayout*)self;
    s32 sel;
    s32 cur;
    s32 end;
    if (weights[startA] >= 10000) {
        sel = startB;
        cur = startA;
        end = limit + 1;
    } else {
        sel = startA;
        cur = startB;
        end = limit;
    }
    s32 pos = end - 1;
    out[pos] = (u16)sel;
    // backwards fill: first lighter marked neighbour of `sel`
    for (s32 i = end - 2; i >= 0; --i) {
        const ScnWalkNode& node = m.nodes[sel];
        u16 wsel = weights[sel];
        u16 cnt = m.edges[node.edgeOffset];
        const u16* p = &m.edges[node.edgeOffset + 1];
        if (cnt > 0) {
            for (u16 k = 0; k < cnt; ++k) {
                u16 nEnt = p[k];
                if (bits[nEnt >> 5] & (1u << (nEnt & 31))) {
                    u16 cw = weights[nEnt & 0x7FFF];
                    if (cw < wsel) {
                        out[i] = nEnt;
                        break;
                    }
                }
            }
        }
    }
    out[end] = (u16)cur;
    // forward walk: append heavy neighbours of the running node
    for (s32 j = 1; j < limit; ++j) {
        const ScnWalkNode& node = m.nodes[cur];
        u16 wcur = weights[cur];
        u16 cnt = m.edges[node.edgeOffset];
        const u16* p = &m.edges[node.edgeOffset + 1];
        if (cnt > 0) {
            for (u16 k = 0; k < cnt; ++k) {
                u16 nEnt = p[k];
                if (bits[nEnt >> 5] & (1u << (nEnt & 31))) {
                    u16 cw = weights[nEnt & 0x7FFF];
                    if (cw < wcur && cw >= 10000) {
                        out[end + j] = nEnt;
                        cur = nEnt;
                        break;
                    }
                }
            }
        }
    }
    return end + limit;
}

// func_80480140 -- for each of `count` outputs, walk the edge list of node
// `start` and pick the first neighbour that lies inside the [baseIndex,
// limit) range, has its bit set in `bits`, and whose weight beats the current
// node's weight; that neighbour becomes the next start.
extern "C" void func_80480140__17UnkClass_8047E110Fv(UnkClass_8047E110* self, u16* out,
    const u16* weights, const u32* bits, s32 start, s32 count, const WalkDistInfo& info) {
    // per-output greedy step: take the first lighter flagged neighbour of
    // `start` inside [base, base+limit) as the next node.
    s32 i = 0;
    do {
        s32 base = info.baseIndex;
        u16 w = weights[start - base];
        u32 off = self->nodes[start].edgeOffset;
        u16 cnt = self->edges[off];
        const u16* p = &self->edges[off + 1];
        if (cnt > 0) {
            do {
                u16 n = *p;
                s32 d = n - base;
                if (d >= 0 && d < info.limit && (bits[d >> 5] & (1 << (d & 31)))
                    && (u32)weights[d] < (u32)w) {
                    *out = n;
                    start = *p;
                    break;
                }
                ++p;
            } while (--cnt);
        }
        ++out;
        ++i;
    } while (i < count);
}
// func_8048020C -- BFS path search between two walk records: resolve the start
// node from record `recIdx` (candidate slot matching `key`), then breadth-first
// expand the graph (distance map + visited bitmap in the scratch area, frontier
// queues in the FE48 work buffer) until `goal` is reached; func_80480140
// backfills the path into outBuf.
extern "C" s32 func_8048020C__17UnkClass_8047E110Fv(UnkClass_8047E110* self,
    u16* outFound, u16* outBuf, s32 limit, s32 key, s32 goal, s32 recIdx) {
    UnkClass_8047E110& m = *self;
    u32 recOff = recIdx * 10;
    const u16* rec = (const u16*)(m.field_0xC + recOff);
    u16 start;
    s32 selFlag;
    if (key == rec[2]) {
        selFlag = 1;
        start = rec[0];
    } else {
        selFlag = 0;
        start = rec[1];
    }
    if (start == goal) {
        *outFound = 1;
        *outBuf = rec[2 + selFlag];
        return 1;
    }
    ScnWpKey& wp = ((ScnWpKey*)m.field_0x8)[key];
    Fe48Table& tbl = lbl_eu_80658540;
    u32* bitmap = (u32*)::func_804B5A68();
    memset(bitmap, 0, ((wp.range >> 5) + 1) * 4);
    u16* buf = (u16*)tbl.field_0x8; // distance map, indexed by id-base
    u16* frontier = buf + wp.range; // frontier queue area
    u16* other = frontier;
    frontier[0] = start;
    buf[start - wp.base] = 0;
    bitmap[(start - wp.base) >> 5] |= 1 << ((start - wp.base) & 31);
    s32 curCount = 1;
    s32 dist = 1;
    while (curCount != 0) {
        const u16* fr = frontier;
        s32 prevCount = curCount;
        u16* dst = other;
        curCount = 0;
        for (s32 i = 0; i < prevCount; ++i) {
            u16 id = *fr++;
            const ScnWalkNode& node = m.nodes[id];
            u16 ecnt = m.edges[node.edgeOffset];
            const u16* ep = &m.edges[node.edgeOffset + 1];
            for (s32 k = 0; k < ecnt; ++k) {
                u16 nEnt = ep[k];
                s32 c = nEnt - wp.base;
                if (c < 0 || c >= wp.range)
                    continue;
                if (bitmap[c >> 5] & (1 << (c & 31)))
                    continue;
                if (!::func_8047EEB0__17UnkClass_8047E110Fv(self,
                        (const WalkQueryInfo*)&m.nodes[nEnt], id, 1))
                    continue;
                buf[c] = (u16)dist;
                if (goal == nEnt) {
                    if (dist + 1 >= limit)
                        return 0;
                    ::func_80480140__17UnkClass_8047E110Fv(self, outBuf, buf,
                        bitmap, goal, dist, *(const WalkDistInfo*)&wp);
                    outBuf[dist] = rec[selFlag];
                    *outFound = (u16)(dist + 1);
                    return 1;
                }
                *dst++ = nEnt;
                ++curCount;
                bitmap[c >> 5] |= 1 << (c & 31);
            }
        }
        u16* tmp = frontier;
        frontier = other;
        other = tmp;
        ++dist;
    }
    return 0;
}

// func_804804DC -- bidirectional walk search: seed a combined queue with two
// start nodes in the FE48 work area, expand neighbours of whichever side has
// remaining capacity (dist map capped at 10000), and when both sides added
// nodes, swap the queue halves. func_8047FF9C backfills the path once a
// connection is found.
extern "C" s32 func_804804DC__17UnkClass_8047E110Fv(UnkClass_8047E110* self,
    u16* outPath, u16* outAux, s32 limit, s32 seedA, s32 seedB) {
    UnkClass_8047E110& m = *self;
    const ScnWalkNode* nodes = m.nodes;
    u32 total = ((const WalkSpawnDesc*)m.field_0x0)->field_0x0;
    u32 cap = (total >= 500) ? total >> 1 : total;
    Fe48Table& tbl = lbl_eu_80658540;
    u16* work = (u16*)tbl.field_0x8;
    u16* qA = work + cap; // forward queue half
    u16* qB = qA + cap;   // backward queue half / dist map
    u32* bitmap = (u32*)::func_804B5A68();
    memset(bitmap, 0, ((total >> 5) + 1) * 4);
    work[0] = (u16)seedA;
    work[1] = (u16)seedB;
    bitmap[seedA >> 5] |= 1 << (seedA & 31);
    bitmap[seedB >> 5] |= 1 << (seedB & 31);
    s32 pass = 1;
    s32 count = 2;
    while (count != 0) {
        s32 prevCount = count;
        s32 addedA = 0;
        s32 addedB = 0;
        u16* dst = qA + cap * 0;
        for (s32 i = 0; i < prevCount; ++i) {
            u16 id = work[i];
            s32 nextDist = qB[id] + 1;
            const ScnWalkNode& node = nodes[id];
            u16 ecnt = m.edges[node.edgeOffset];
            const u16* ep = &m.edges[node.edgeOffset + 1];
            s32 sel = (nextDist < 10000) ? 1 : 0;
            for (s32 k = 0; k < ecnt; ++k) {
                u16 nEnt = ep[k];
                if (bitmap[nEnt >> 5] & (1 << (nEnt & 31)))
                    continue;
                if (!::func_8047EEB0__17UnkClass_8047E110Fv(self,
                        (const WalkQueryInfo*)&nodes[nEnt], id, sel))
                    continue;
                if (nextDist < 10000)
                    ++addedA;
                else
                    ++addedB;
                qB[nEnt] = (u16)nextDist;
                *dst++ = nEnt;
                bitmap[nEnt >> 5] |= 1 << (nEnt & 31);
                if (nextDist >= limit)
                    return ::func_8047FF9C__17UnkClass_8047E110Fv(self,
                        outPath, qB, bitmap, 0, 0, seedA, seedB, limit);
            }
        }
        if (addedA != 0 && addedB != 0) {
            u16* tmp = qA;
            qA = qB;
            qB = tmp;
            ++pass;
        }
        count = addedA + addedB;
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
extern "C" s32 func_80480EF0__17UnkClass_8047E110Fv(UnkClass_8047E110* self, u32 nodeIndex, s32 value) {
    ScnManagerLayout& m = *(ScnManagerLayout*)self;
    u16* edges = m.edges;
    s32 count = edges[m.nodes[nodeIndex].edgeOffset];
    const u16* e = &edges[m.nodes[nodeIndex].edgeOffset + 1];
    for (; count > 0; --count) {
        if (value == *e) return 1;
        ++e;
    }
    return 0;
}

// func_80480F48 -- compare two stride-10 records against `value`: locate which
// endpoint slot (+4/+6) of record A holds it, likewise for record B, then
// report whether the two slots' paired ids (+0/+2) agree.
extern "C" bool func_80480F48__17UnkClass_8047E110Fv(UnkClass_8047E110* self, s32 value,
    s32 idxA, s32 idxB) {
    s32 slotA;
    s32 slotB;
    const u16* recs = (const u16*)self->field_0xC;
    const u16* lo = recs + 2; // +4 endpoint slots
    const u16* hi;            // +6 endpoint slots (else arm only)
    if (lo[idxA * 5] == value) {
        slotA = 0;
        if (lo[idxB * 5] == value)
            slotB = 0;
        else if (recs[idxB * 5 + 3] == value)
            slotB = 1;
        else
            return false;
    } else {
        hi = recs + 3;
        if (hi[idxA * 5] == value) {
            slotA = 1;
            if (lo[idxB * 5] == value)
                slotB = 0;
            else if (hi[idxB * 5] == value)
                slotB = 1;
            else
                return false;
        } else {
            return false;
        }
    }
    const u16* ra = recs + idxA * 5;
    const u16* rb = recs + idxB * 5;
    return ra[slotA] == rb[slotB];
}

// func_80481014 -- activate/clear the walk box: a==b==0 clears it (flag bit 0
// off); otherwise write the clearance pair, position words and set the flag.
// Either way, advance the accumulated distance by `c`.
extern "C" void func_80481014__17UnkClass_8047E110Fv(UnkClass_8047E110* self, f32 a, f32 b, const ScnVecWords* v, f32 c) {
    ScnManagerLayout& m = *(ScnManagerLayout*)self;
    if (a == lbl_eu_8066A8AC) {
        if (b == lbl_eu_8066A8AC)
            goto clear;
    }
    // set: write the clearance pair, position words and raise the flag
    m.field_0x24 = b;
    m.field_0x28 = a;
    m.field_0x2C = v->field_0x0;
    m.field_0x30 = v->field_0x4;
    m.field_0x34 = v->field_0x8;
    m.field_0x38 |= 1;
    goto end;
clear:
    m.field_0x38 &= ~1;
end:
    m.field_0x20 = m.field_0x1C + c;
}

// func_80481074 -- walker contact probe: build the world position of the
// queried grid cell, reject it when it leaves the box (radius 0x7C, direction
// dot test); otherwise normalize the offset and record either the farthest
// miss (flags bit 0) or the best forward projection (bit 1).
extern "C" s32 func_80481074__17UnkClass_8047E110Fv(UnkClass_8047E110* self,
    const WalkHitQuery* q) {
    ScnManagerLayout& m = *(ScnManagerLayout*)self;
    f32 wx = lbl_eu_8066A8A8 * (f32)(q->gxCell + 1)
        + lbl_eu_8066A898 * (f32)q->gridX;
    f32 wz = lbl_eu_8066A8A8 * (f32)(q->gzCell + 1)
        + lbl_eu_8066A898 * (f32)q->gridZ;
    f32 py = q->field_0x0;
    f32 dx = wx - m.field_0x54;
    f32 dz = wz - m.field_0x5C;
    f32 dist2 = dx * dx + dz * dz;
    if (m.field_0x7C * m.field_0x7C < dist2
        || m.dir.x * dx + m.dir.z * dz <= lbl_eu_8066A8AC) {
        // outside the box or moving away: track farthest miss only
        u16 flags = m.field_0x90;
        if (!(flags & 2) && m.field_0x80 < dist2) {
            m.field_0x80 = dist2;
            m.field_0x90 = flags | 1;
            m.field_0x6C = wx;
            m.field_0x70 = py;
            m.field_0x74 = wz;
        }
        return 0;
    }
    if (dist2 < 1.0f)
        nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
    f32 d2 = dx * dx + dz * dz;
    f32 len = (d2 > 1.0f) ? d2 * nw4r::math::FrSqrt(d2) : d2;
    f32 inv = lbl_eu_8066A890 / len;
    dx *= inv;
    dz *= inv;
    f32 proj = m.dir.x * dx + m.dir.z * dz;
    if (lbl_eu_8066A8BC < proj) {
        m.field_0x6C = wx;
        m.field_0x70 = py;
        m.field_0x74 = wz;
        return 1;
    }
    u16 flags = m.field_0x90;
    if (flags & 2) {
        if (m.field_0x84 < proj) {
            m.field_0x6C = wx;
            m.field_0x70 = py;
            m.field_0x74 = wz;
            m.field_0x84 = proj;
        }
    } else {
        m.field_0x90 = flags | 2;
        m.field_0x6C = wx;
        m.field_0x70 = py;
        m.field_0x74 = wz;
        m.field_0x84 = proj;
    }
    return 0;
}

// func_804812D8 -- point-in-node test against the active walk box: the node
// cell (expanded one cell up/right) must overlap the box horizontally and in z,
// and the node plane must be within vertical clearance of the box floor.
extern "C" bool func_804812D8__17UnkClass_8047E110Fv(UnkClass_8047E110* self,
    const ScnWalkNode* node) {
    ScnManagerLayout& m = *(ScnManagerLayout*)self;
    if (!(node->reserved8 & 2))
        return false;
    if (!(scaleNodeCoord(node->x) <= m.field_0x54))
        return false;
    if (!(scaleNodeCoord(node->x + node->width + 1) >= m.field_0x48))
        return false;
    if (!(scaleNodeCoord(node->z) <= m.field_0x5C))
        return false;
    if (!(scaleNodeCoord(node->z + node->depth + 1) >= m.field_0x50))
        return false;
    if (!(__fabs(m.field_0x4C - node->xPos) < m.field_0x20))
        return false;
    return true;
}

// func_804813E8 -- point-in-node test around the reference point kept at
// 0x54/0x58/0x5C: the point must lie inside the node cell expanded by one cell
// on every side and within vertical clearance of the node plane.
extern "C" bool func_804813E8__17UnkClass_8047E110Fv(UnkClass_8047E110* self,
    const ScnWalkNode* node) {
    ScnManagerLayout& m = *(ScnManagerLayout*)self;
    if (!(scaleNodeCoord(node->x) <= m.field_0x54))
        goto fail;
    if (!(scaleNodeCoord(node->x + node->width + 1) >= m.field_0x54))
        goto fail;
    if (!(scaleNodeCoord(node->z) <= m.field_0x5C))
        goto fail;
    if (!(scaleNodeCoord(node->z + node->depth + 1) >= m.field_0x5C))
        goto fail;
    if (!(__fabs(m.field_0x58 - node->xPos) < m.field_0x20))
        goto fail;
    return true;
fail:
    return false;
}

// func_804814DC -- rebuild the walk box corners from two points (word-wise
// copies), derive the travel direction min-max (normalized, with degenerate
// handling), reset the visit counter and refresh the reference block at 0x9C.
extern "C" void func_804814DC__17UnkClass_8047E110Fv(UnkClass_8047E110* self,
    const ml::CVec3* cornerA, const ml::CVec3* cornerB, f32 paramF) {
    ScnManagerLayout& m = *(ScnManagerLayout*)self;
    CVec3 dir;
    const CVec3& boxMin = *(const CVec3*)&m.field_0x48;
    CVec3& boxMax = *(CVec3*)&m.field_0x54;
    *(ScnVecWords*)&m.field_0x48 = *(const ScnVecWords*)cornerA;
    *(ScnVecWords*)&m.field_0x54 = *(const ScnVecWords*)cornerB;
    dir = boxMin - boxMax;
    m.dir = dir;
    m.field_0x90 = 0;
    m.field_0x7C = paramF;
    m.field_0x80 = lbl_eu_8066A8AC;
    m.field_0x84 = lbl_eu_8066A8B4;
    if (m.dir.x == lbl_eu_8066A8AC && m.dir.z == lbl_eu_8066A8AC) {
        // degenerate: axis-aligned fallback direction
        m.dir.x = lbl_eu_8066A8AC;
        m.dir.y = lbl_eu_8066A8AC;
        m.dir.z = lbl_eu_8066A890;
    } else {
        f32 lenSq = m.dir.z * m.dir.z
            + (m.dir.x * m.dir.x + m.dir.y * m.dir.y);
        if (lbl_eu_8066A8AC == lenSq) {
            m.dir = CVec3::zero;
        } else {
            PSVECNormalize(m.dir, m.dir);
        }
    }
    u32* ref = (u32*)&lbl_eu_8056DC68;
    u32 wordA = *ref++;
    u32 wordB = *ref++;
    u32 wordC = *ref;
    m.field_0x9C.field_0x4 = wordB;
    m.field_0x9C.field_0x0 = wordA;
    m.field_0x9C.field_0x8 = wordC;
}

// func_8048163C -- build the walk box (half-size lbl_eu_8066A890) around `v`
// into 0x48-0x5C, reset the 0x90 counter and refresh the reference block at
// 0x9C from the data global.
extern "C" void func_8048163C__17UnkClass_8047E110Fv(UnkClass_8047E110* self, const ml::CVec3* v) {
    ScnManagerLayout& m = *(ScnManagerLayout*)self;
    m.field_0x90 = 0;
    m.field_0x4C = v->y;
    m.field_0x58 = v->y;
    m.field_0x48 = v->x - lbl_eu_8066A890;
    m.field_0x50 = v->z - lbl_eu_8066A890;
    m.field_0x54 = lbl_eu_8066A890 + v->x;
    m.field_0x5C = lbl_eu_8066A890 + v->z;
    // interleaved load/store order matters for byte-identity: word A is
    // fetched (lwzu), word B feeds its store before A's, then word C loads.
    u32* ref = (u32*)&lbl_eu_8056DC74;
    u32 wordA = *ref++;
    m.field_0x9C.field_0x4 = *ref++;
    m.field_0x9C.field_0x0 = wordA;
    m.field_0x9C.field_0x8 = *ref;
}

// func_8048169C -- build the walk box from a min/max corner pair: corners go
// to 0x48/0x54, the size delta seeds the travel direction at 0x60-0x68, the
// x/z portal threshold scales start at 1.0 and fall back to the cross-axis
// ratios z/x and x/z whenever the corresponding direction axis isn't 1.0,
// the visit counter clears and the reference block at 0x9C reloads.
extern "C" void func_8048169C__17UnkClass_8047E110Fv(UnkClass_8047E110* self,
    const ml::CVec3* cornerMin, const ml::CVec3* cornerMax) {
    ScnManagerLayout& m = *(ScnManagerLayout*)self;
    CVec3 dir;
    const CVec3& boxMin = *(const CVec3*)&m.field_0x48;
    CVec3& boxMax = *(CVec3*)&m.field_0x54;
    *(ScnVecWords*)&m.field_0x48 = *(const ScnVecWords*)cornerMin;
    *(ScnVecWords*)&m.field_0x54 = *(const ScnVecWords*)cornerMax;
    dir = boxMin - boxMax;
    m.dir = dir;
    m.field_0x90 = 0;
    m.field_0x88 = lbl_eu_8066A8AC;
    m.field_0x8C = lbl_eu_8066A8AC;
    if (m.dir.x != lbl_eu_8066A8AC) {
        m.field_0x88 = m.dir.z / m.dir.x;
    }
    if (m.dir.z != lbl_eu_8066A8AC) {
        m.field_0x8C = m.dir.x / m.dir.z;
    }
    u32* ref = lbl_eu_8056DC80;
    m.field_0x9C.field_0x0 = *ref++;
    m.field_0x9C.field_0x4 = *ref++;
    m.field_0x9C.field_0x8 = *ref;
}

// func_80481790 -- BFS over the walk graph from `startNode`: each pass scans
// the frontier's edge lists, filters unvisited entries through the EEB0
// neighbour test and the step callback (ptmf at 0x9C), marks visited entries
// in a scratch bitmap and swaps double-buffered frontiers (work area comes
// from the FE48 locator block). Returns 1 when the callback accepts a node or
// the finish flags are set.
extern "C" s32 func_80481790__17UnkClass_8047E110Fv(UnkClass_8047E110* self,
    s32 startNode) {
    ScnManagerLayout& m = *(ScnManagerLayout*)self;
    ScnWalkFn walkFn;
    memcpy(&walkFn, &self->callback, sizeof(walkFn));
    if ((self->*walkFn)(&m.nodes[startNode]))
        return 1;
    u32 v = ((const WalkSpawnDesc*)m.field_0x0)->field_0x0;
    u32 cap = v >= 500 ? v >> 1 : v;
    Fe48Table* tbl = &lbl_eu_80658540;
    u32* bits = (u32*)::func_804B5A68();
    memset(bits, 0, ((v >> 5) + 1) * 4);
    u16* work = (u16*)tbl->field_0x8;
    u16* front = work;
    u16* next = work + cap;
    front[0] = (u16)startNode;
    bits[startNode >> 5] |= 1u << (startNode & 31);
    s32 count = 1;
    for (s32 pass = 0; count != 0 && pass < 30; ++pass) {
        s32 prevCount = count;
        s32 added = 0;
        u16* dst = next;
        for (s32 i = 0; i < prevCount; ++i) {
            u16 id = front[i];
            const ScnWalkNode& node = m.nodes[id];
            u16 ecnt = m.edges[node.edgeOffset];
            const u16* ep = &m.edges[node.edgeOffset + 1];
            for (s32 k = 0; k < ecnt; ++k) {
                u16 ent = ep[k];
                if (bits[ent >> 5] & (1u << (ent & 31)))
                    continue;
                if (!::func_8047EEB0__17UnkClass_8047E110Fv(self,
                        (const WalkQueryInfo*)&m.nodes[ent], id, 0))
                    continue;
                if ((self->*walkFn)(&m.nodes[ent]))
                    return 1;
                bits[ent >> 5] |= 1u << (ent & 31);
                *dst++ = ent;
                ++added;
            }
        }
        u16* tmp = front;
        front = next;
        next = tmp;
        count = added;
    }
    return (m.field_0x90 & 3) != 0 ? 1 : 0;
}

// func_804819AC -- test bit 1 of the node flag word (offset 0x8); the walker
// skips nodes whose closed/blocked bit is set.
extern "C" s32 func_804819AC__17UnkClass_8047E110Fv(UnkClass_8047E110* self, u32 nodeIndex) {
    ScnManagerLayout& m = *(ScnManagerLayout*)self;
    return (m.nodes[nodeIndex].reserved8 & 2) != 0;
}

// func_804819C4 -- step the walker from `idx`: run the 0x9C callback on the
// start node, then hop up to 90 times: each pass scans the current node's
// edge list for a neighbour sharing a portal edge (x+, z+ or z- side) that
// passes the per-axis distance threshold (0x88/0x8C scaled, gated by the
// travel direction at 0x60/0x68); the callback runs on each accepted hop and
// ends the walk with 1 when it accepts.
extern "C" s32 func_804819C4__17UnkClass_8047E110Fv(UnkClass_8047E110* self,
    s32 idx) {
    ScnManagerLayout& m = *(ScnManagerLayout*)self;
    // the ptmf slot stays in memory; MWCC lowers calls through it to
    // __ptmf_scall with r12 pointing at the slot
    ScnWalkFn& walkFn = *(ScnWalkFn*)&self->callback;
    if ((self->*walkFn)(&m.nodes[idx]))
        return 1;
    // loop-invariant constants (retail pins these in f29-f31)
    const f32 cell = lbl_eu_8066A898;
    const f32 zeroF = lbl_eu_8066A8AC;
    for (s32 pass = 0; pass < 90; ++pass) {
        const ScnWalkNode& cn = m.nodes[idx];
        f32 curMinX = scaleNodeCoord(cn.x);
        f32 curMinZ = scaleNodeCoord(cn.z);
        f32 curMaxX = (f32)(cell * s32ToF64_A8A0(cn.width + 1)) + curMinX;
        f32 curMaxZ = (f32)(cell * s32ToF64_A8A0(cn.depth + 1)) + curMinZ;

        u16 cnt = m.edges[cn.edgeOffset];
        const u16* p = &m.edges[cn.edgeOffset + 1];
        s32 next = -1;
        for (u16 k = 0; k < cnt; ++k) {
            u16 n = *p++;
            const ScnWalkNode& nn = m.nodes[n];
            if (nn.reserved8 & 2)
                continue;
            f32 nMinX = scaleNodeCoord(nn.x);
            f32 nMinZ = scaleNodeCoord(nn.z);
            f32 nMaxX = (f32)(cell * s32ToF64_A8A0(nn.width + 1)) + nMinX;
            f32 nMaxZ = (f32)(cell * s32ToF64_A8A0(nn.depth + 1)) + nMinZ;

            if (curMaxX == nMinX) {
                // neighbour directly on the +x side: vertical portal
                if (!(m.dir.x > zeroF))
                    continue;
                f32 portalHi = (curMaxZ >= nMaxZ) ? nMaxZ : curMaxZ;
                f32 portalLo = (curMinZ >= nMinZ) ? curMinZ : nMinZ;
                if (m.field_0x88 == zeroF)
                    continue;
                f32 thr = m.field_0x5C + m.field_0x88 * (curMaxX - m.field_0x54);
                if (!(portalHi < thr) || !(portalLo <= thr))
                    continue;
            } else if (curMaxZ == nMinZ) {
                // neighbour above (-z): horizontal portal
                if (!(m.dir.z < zeroF))
                    continue;
                if (m.field_0x8C == zeroF)
                    continue;
                f32 thr = m.field_0x54 + m.field_0x8C * (curMaxZ - m.field_0x5C);
                f32 hiX = (curMaxX >= nMaxX) ? nMaxX : curMaxX;
                f32 loX = (curMinX >= nMinX) ? nMinX : curMinX;
                if (!(hiX >= thr) || !(loX <= thr))
                    continue;
            } else if (nMaxZ == curMinZ) {
                // neighbour below (+z): horizontal portal
                if (!(m.dir.z > zeroF))
                    continue;
                if (m.field_0x8C == zeroF)
                    continue;
                f32 thr = m.field_0x54 + m.field_0x8C * (nMaxZ - m.field_0x5C);
                f32 hiX = (curMaxX >= nMaxX) ? nMaxX : curMaxX;
                f32 loX = (curMinX >= nMinX) ? nMinX : curMinX;
                if (!(hiX >= thr) || !(loX <= thr))
                    continue;
            } else {
                continue;
            }
            if ((self->*walkFn)(&nn))
                return 1;
            next = n;
            break;
        }
        if (next == -1)
            return 0;
        idx = next;
    }
    return 0;
}

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// [.data] 0x8056DC68-0x8056DCD8 (112B): three 12-byte reference blocks, the
// CScnItemCameraNw4r vtable, and two RTTI base-lists.
// The own-member symbols func_80481074/804812D8/804813E8 are the stub member
// functions defined above (retail Fv mangling); the remaining slots are
// foreign functions/locators.
// (func_80481074__17UnkClass_8047E110Fv declared/defined above)
extern "C" bool func_804812D8__17UnkClass_8047E110Fv(UnkClass_8047E110* self,
    const ScnWalkNode* node);
extern "C" bool func_804813E8__17UnkClass_8047E110Fv(UnkClass_8047E110* self,
    const ScnWalkNode* node);
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
