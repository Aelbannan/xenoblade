// NOTE: this minimal preamble exists ONLY so the tombstone compiles.
// It is superseded by the restored pre-incident source (see runbook below).
#include <types.h>
#include <string.h>
#include "libs/monolib/src/scn/UnkClass_8047E110.hpp"
#include <nw4r/math/math_arithmetic.h>

// Pool allocator provided by monolib coli (returns the arena cursor).
extern "C" u32 func_804B5A68(void);
// Reachability predicate over a walk node (retail member, called with the
// current edge pool and the caller's selector word).
extern "C" s32 func_8047EEB0__17UnkClass_8047E110Fv(UnkClass_8047E110* self,
    ScnWalkNode* node, u16 id, s32 kind);
// Edge-list resolver: maps the queried position to an id in the candidate
// cell's list (returns -1 when nothing matches).
extern "C" s32 func_8047E1B0__17UnkClass_8047E110Fv(UnkClass_8047E110* self,
    const ml::CVec3* pos);
// Frontier-crossing resolver: invoked when the two seeded searches touch the
// same node with mismatching seed markers; its return value becomes ours.
extern "C" s32 func_8047EFBC__17UnkClass_8047E110Fv(UnkClass_8047E110* self,
    u16* arg4, u16 id);

// func_8047EAD4 -- walk-graph connectivity query.
//
// Selects the kd-cell record for startId (stride-12 table at manager+0x8:
// +0 base id, +2 range, +4 edge-list index). When the goal node is flagged
// (bit 0 of its +8 halfword), scans the cell's record list (stride-10 records
// at manager+0xC) for the endpoint paired with goalId, collects every record
// whose matching-side group byte agrees, writing their ids into outIds, and
// returns the count. Otherwise performs a breadth-first flood fill from
// goalId across each node's edge list (bounded to [base, base+range), gated
// by func_8047EEB0 with a pooled visited bitmap); reaching any flagged node
// runs the same collection scan and returns its count, else returns 0.

using ml::CVec3;

// Manager field view (retail offsets).
struct ScnManagerLayout {
    u32 field_0x0;
    ScnWalkNode* nodes;
    ScnNodeRef* field_0x8;
    ScnNodeMap* field_0xC;
    u32 field_0x10;
    u16* edges;
    f32 field_0x18;
    f32 field_0x1C;
    f32 field_0x20;
    f32 field_0x24;
    f32 field_0x28;
    u32 field_0x2C;
    u32 field_0x30;
    u32 field_0x34;
    u32 field_0x38;
    u32 field_0x3C;
    u32 field_0x40;
    u32 field_0x44;
    f32 field_0x48;
    f32 field_0x4C;
    f32 field_0x50;
    f32 field_0x54;
    f32 field_0x58;
    f32 field_0x5C;
    CVec3 dir;
    f32 field_0x6C;
    f32 field_0x70;
    f32 field_0x74;
    u8 field_0x78[4];
    f32 field_0x7C;
    f32 field_0x80;
    f32 field_0x84;
    f32 field_0x88;
    f32 field_0x8C;
    u16 field_0x90;
    u8 field_0x92[2];
    u32 field_0x94;
    u32 field_0x98;
    ScnPtmf field_0x9C;
};

// Float-pool imports; A890/A8A0 are OWNED by this TU per
// tools/coop/lbls_exclusions.json, so their definitions must survive here
// until the full source is restored.
extern const f32 lbl_eu_8066A898;
extern const f32 lbl_eu_8066A8A8;
extern const f32 lbl_eu_8066A8AC;
extern const f32 lbl_eu_8066A8BC;
extern const char lbl_eu_80526300[];
extern const char lbl_eu_80526324[];
const f32 lbl_eu_8066A890 = 1.0f;
extern const f64 lbl_eu_8066A8A0 = 4503601774854144.0;

// func_8047E390 -- neighbour search around a walk cell: scan the second
// candidate list of record `index` (stride-12 cell table at 0x8), apply the
// box/flag visibility gate, expand each node's s16 rect by `clearance` and
// track the closest node to `pos`, with a priority scheme for flagged nodes.
//
// OPEN ITEM PACKET (us-80482360, best mismatch=171 structural=148 reg_swap=23,
// 688B vs 668B): dominant residual is register allocation - decomp emits
// bl _savegpr_26 + mflr/LR spill + frame -64, while retail has NO calls, NO
// LR save, frame -0x30, and saves exactly r29/r30/r31 which are then DEAD
// (never read in the body). Reproducing three phantom callee-saved stores
// plus volatile-only allocation has resisted: bool-vs-s32 flags, hoisted vs
// per-iteration flags reload, do-while vs for loop, helper-union vs plain
// (f32) int casts (plain casts ARE required - they unify onto lbl_eu_8066A8A0),
// statement reordering, u16-vs-s32 counts (u16 made it WORSE: savegpr_27+
// restgpr_27). Root cause is likely wall #6/#13: the mtctr/bdnz loop shape
// needs a per-function opt level the unit lock denies, so count2 stays in a
// GPR and pushes allocation over the volatile budget. KB corroboration:
// __wudSyncDone sibling shows the same split - retail individual r29-r31
// stw saves are an -O4,p-style prologue while our unit emits -O4,s-style
// _savegpr/_restgpr outliner calls; not reachable from high-level source. Next experiments: (a) restore the full pre-incident TU
// first (-ipa file scheduling depends on the whole file); (b) try declaring
// three locals that die immediately after init to lure r29-31 allocation;
// (c) check whether retail used register-keyword locals.
extern "C" s32 func_8047E390__17UnkClass_8047E110Fv(UnkClass_8047E110* self,
    const ml::CVec3* pos, s32 index, f32 clearance) {
    ScnManagerLayout& m = *(ScnManagerLayout*)self;
    f32 pxLo = pos->x - clearance; // f6
    f32 pzLo = pos->z - clearance; // f7
    f32 pxHi = pos->x + clearance; // f8
    f32 pzHi = pos->z + clearance; // f9
    const ScnWalkNode* node = m.nodes;
    const u16* rec = (const u16*)(m.field_0x8 + index * 12);
    s32 count1 = rec[0];
    s32 count2 = rec[1];
    node += count1;
    s32 best = -1;                    // r7
    f32 bestDist = lbl_eu_8066A894;   // f5
    s32 haveStrict = 1;               // r8: any strict candidate seen
    s32 haveMargin = 1;               // r9: any margin-only candidate seen
    s32 haveFlagged = 1;              // r10: no flagged strict take yet
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

// OPEN ITEM PACKET (us-80482aa4, best 936B vs 988B, structural=218,
// mismatch=236): semantics fully decoded and frame now matches (-112, stmw
// r14-r31). Residual is register allocation + scheduling: decomp copies
// params/spills in a different order (retail: self->r15, goalId->r17,
// arg7->r16, then stw out), emits lhzx+copy where retail folds the edge-list
// count load into lhzux (update-form *p++ whose result register feeds the
// neighbors pointer), and still differs by ~52B inside the BFS inner loop.
// Ruled out: ScnRec10 pointer-vs-repeated-index loop shapes, hoisted vs
// branch-local list/count, cached base/range locals vs direct cell reloads
// (direct reloads required for the call-aliasing shape), u16-vs-s32 counts.
// Next experiments: (a) recover the pre-incident TU (see INCIDENT note
// below) since -ipa file scheduling depends on the whole file; (b) try
// update-form idiom variants for the count load; (c) reorder the BFS init
// statements to interleave like retail (nxt computed before alloc call).
extern "C" s32 func_8047EAD4__17UnkClass_8047E110Fv(UnkClass_8047E110* self,
    u16* out1, s32 arg5, s32 arg6, s32 arg7) {
    ScnManagerLayout& m = *(ScnManagerLayout*)self;
    const ScnNodeRef& ref = m.field_0x8[arg5];
    const ScnWalkNode& target = m.nodes[arg6];
    u16* nlist = m.edges + ref.field_0x4;
    s32 edgeCount = *nlist++;

    if (target.reserved8 & 1) {
        // Endpoint collection: find which side of the lookup records pairs
        // with arg6 and remember its flag byte, then emit every record
        // whose value is arg5 with a matching same-side flag byte.
        u8 match;
        for (s32 i = 0; i < edgeCount; ++i) {
            const ScnNodeMap& lk = m.field_0xC[nlist[i]];
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
            const ScnNodeMap& lk = m.field_0xC[nlist[i]];
            if (arg5 == lk.field_0x4) {
                if (match != lk.field_0x8)
                    continue;
            } else if (match != lk.field_0x9) {
                continue;
            }
            out1[count] = nlist[i];
            count++;
        }
        return count;
    }

    // Bounded BFS from arg6 over each node's edge list, restricted to the
    // id window [ref.edgeStart, ref.edgeStart+ref.edgeCount), with one bit
    // per id in a pooled visited map. Reaching a flagged node runs the
    // endpoint-collection scan above into out1 and returns its count.
    // ref fields are re-read inside the loops on purpose: the calls below
    // may alias the manager, forcing retail's reload shape.
    u16* cur = out1;
    u16* other = out1 + ref.edgeCount;
    u32* bits = (u32*)func_804B5A68();
    memset(bits, 0, ((m.field_0x8[arg5].edgeCount >> 5) + 1) * 4);
    out1[0] = arg6;
    s32 level = 1;
    s32 d = arg6 - m.field_0x8[arg5].edgeStart;
    bits[d >> 5] |= 1 << (d & 31);
    while (level != 0) {
        const u16* cin = cur;
        u16* fout = other;
        s32 n = level;
        level = 0;
        for (s32 i = 0; i < n; ++i) {
            u16 id = *cin++;
            ScnWalkNode* node = m.nodes + id;
            u16 ecnt = m.edges[node->edgeOffset];
            const u16* el = &m.edges[node->edgeOffset + 1];
            for (s32 j = 0; j < ecnt; ++j) {
                u16 nb = *el++;
                s32 dd = m.field_0x8[arg5].edgeStart - nb;
                if (dd < 0 || dd >= m.field_0x8[arg5].edgeCount)
                    continue;
                s32 wi = dd >> 5;
                u32 mask = 1 << (dd & 31);
                if (bits[wi] & mask)
                    continue;
                ScnWalkNode* en = m.nodes + nb;
                if (!func_8047EEB0__17UnkClass_8047E110Fv(self, en, id, arg7))
                    continue;
                if (en->reserved8 & 1) {
                    // Reached a flagged node: run the endpoint collection
                    // over the reference list and return its count.
                    u8 match;
                    for (s32 k = 0; k < edgeCount; ++k) {
                        const ScnNodeMap& lk = m.field_0xC[nlist[k]];
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
                    for (s32 k = 0; k < edgeCount; ++k) {
                        const ScnNodeMap& lk = m.field_0xC[nlist[k]];
                        if (arg5 == lk.field_0x4) {
                            if (match != lk.field_0x8)
                                continue;
                        } else if (match != lk.field_0x9) {
                            continue;
                        }
                        out1[count] = nlist[k];
                        count++;
                    }
                    return count;
                }
                *fout++ = nb;
                level++;
                bits[wi] |= mask;
            }
        }
        // ping-pong the frontier halves inside the shared output buffer
        u16* t = cur;
        cur = other;
        other = t;
    }
    return 0;
}

// =====================================================================
// INCIDENT - DO NOT BLINDLY REBUILD FROM THIS FILE
//
// During session batch-16 (2026-08-24 ~17:40 UTC) an agent destroyed this
// translation unit by calling `write` (full-file overwrite) on it instead of
// `edit`. The previous ~1889-line content of UnkClass_8047E110.cpp was lost
// from the working tree. Only the func_80481074 body below survives.
//
// UNAFFECTED: libs/monolib/src/scn/UnkClass_8047E110.hpp (never touched).
// The last good build artifact is build/us/src/monolib/src/scn/
// UnkClass_8047E110.o (compiled from the pre-loss source after the session's
// first edit).
//
// RECOVERY SOURCES (verified to exist):
// - build/pi-harness/sessions/monolib__src__scn__UnkClass_8047E110/*/ *.jsonl
//   Every prior session's tool calls/results are recorded there, including
//   full edit oldText/newText pairs and file reads. Sessions editing this
//   cpp: 08-23 batch-0/5/10/15/20, 08-24 batch-0/1/4/6/9/11/12/14, batch-16.
// - build/pi-harness/sessions/monolib__src__scn__UnkClass_8047E110/batch-20/
//   batch-20-session-1.transcript.md lines ~1963-2082 + ~2103-2235 hold
//   line:crc3|content annotated dumps of cpp lines 1-120 and 1111-1223
//   (state as of 08-23 07:40), plus a full dump of the .hpp as of then.
// - git history / reflog (human with shell access) can restore the last
//   committed version directly; transcripts cover post-commit edits.
//
// RECOMMENDED: restore via git first, then replay recorded edits from the
// 08-24 session jsonls (batch-14 is the last session before the loss).
// Verify with: unit-status monolib/src/scn/UnkClass_8047E110 -> must return
// to "matched: 10/30" before any further matching work in this unit.
// LINK WARNING: blob-surgery .data definitions lost with the file
// (lbl_eu_8056DC68/74/80/90/B8/CCC, RTTI lists, vtable) will fail the final
// link until the restore is done; this preamble only fixes TU compilation.
// =====================================================================

// func_80481074 -- walker contact probe: build the world position of the
// queried grid cell, reject it when it leaves the box (radius 0x7C, direction
// dot test); otherwise normalize the offset and record either the farthest
// miss (flags bit 0) or the best forward projection (bit 1).
// Session batch-16 progress on us-80485044 before the incident:
// structural 153 -> 137 vs retail (len = threshold-default + conditional
// dist2*FrSqrt(dist2) rematerialization shape identified from retail asm).
//
// OPEN ITEM PACKET (us-80485044, best structural=137 mismatch=151, 592B vs
// 612B): remaining divergence is dominated by two fixed-codegen shapes:
// (1) PROLOGUE FPR-SAVE ENCODING: decomp (monolib Wii/1.1 flags) spills
//     callee-saved f30/f31 as stfd + psq_st(...,W=1,qr3) [single-word save];
//     retail uses stfd + psq_st(...,W=0,qr0) [pair save]. Same open item as
//     src/kyoshin/code_800B06A4.cpp func_800B68A8 ("psq_st words differ,
//     W=1,qr3 vs W=0,qr0"). Suspect compiler-config difference (GC/3.0a5.2
//     vs Wii/1.1); not reachable from high-level source. Any function in
//     this unit needing f14-f31 callee saves inherits this wall.
// (2) FRAME/SCHEDULING DRIFT: decomp frame -128 vs retail -80; decomp hoists
//     all four WalkHitQuery loads (lbz/lha) ahead of the lis 0x4330 and
//     interleaves the stfd saves among the int->float conversion buffer
//     writes; retail converts strictly in statement order with two SHARED
//     conversion buffers at 0x18/0x1c and 0x20/0x24. Likely downstream of
//     (1): different save layout shifts the scheduler's slot budget.
// Ruled out: operand order variants (const*val vs val*const), flat-vs-nested
// miss gating, ternary len forms, dir.x-first dot order, self-> vs overlay-
// reference access (byte-identical output). Next experiments: (a) retry under
// mw_version GC/3.0a5.2 in an off-branch build; (b) re-diff after the TU
// restore, since -ipa file scheduling depends on the whole TU.
extern "C" s32 func_80481074__17UnkClass_8047E110Fv(UnkClass_8047E110* self,
    const WalkHitQuery* q) {
    ScnManagerLayout& m = *(ScnManagerLayout*)self;
    // cell position: cell-size * (cell+1) evaluated before grid-coord term
    f32 wx = lbl_eu_8066A8A8 * (q->gxCell + 1) + lbl_eu_8066A898 * q->gridX;
    f32 wz = lbl_eu_8066A8A8 * (q->gzCell + 1) + lbl_eu_8066A898 * q->gridZ;
    f32 py = q->field_0x0;
    f32 dx = wx - m.field_0x54;
    f32 dz = wz - m.field_0x5C;
    f32 dist2 = dx * dx + dz * dz;
    // outside the clearance circle or behind the walk direction: miss path
    if (m.field_0x7C * m.field_0x7C <= dist2) {
        f32 fwd = m.dir.z * dz + m.dir.x * dx;
        if (lbl_eu_8066A8AC < fwd) {
            if (dist2 < lbl_eu_8066A8AC)
                nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
            // normalize; len defaults to the floor and dist2 is
            // rematerialized around the FrSqrt call like retail
            f32 len = lbl_eu_8066A8AC;
            if (dist2 > lbl_eu_8066A8AC)
                len = dist2 * nw4r::math::FrSqrt(dist2);
            f32 inv = lbl_eu_8066A890 / len;
            dz *= inv;
            dx *= inv;
            fwd = m.dir.z * dz + m.dir.x * dx;
            if (lbl_eu_8066A8BC < fwd) {
                m.field_0x6C = wx;
                m.field_0x70 = py;
                m.field_0x74 = wz;
                return 1;
            }
            u16 flags = m.field_0x90;
            if (flags & 2) {
                if (m.field_0x84 < fwd) {
                    m.field_0x6C = wx;
                    m.field_0x70 = py;
                    m.field_0x74 = wz;
                    m.field_0x84 = fwd;
                }
            } else {
                m.field_0x90 = flags | 2;
                m.field_0x6C = wx;
                m.field_0x70 = py;
                m.field_0x74 = wz;
                m.field_0x84 = fwd;
            }
            return 0;
        }
    }
    // farthest-miss tracking (flag bit 0)
    u16 flags = m.field_0x90;
    if (!(flags & 2)) {
        if (m.field_0x80 < dist2) {
            m.field_0x80 = dist2;
            m.field_0x90 = flags | 1;
            m.field_0x6C = wx;
            m.field_0x70 = py;
            m.field_0x74 = wz;
        }
    }
    return 0;
}

// NOTE: retail symbol ends in Fv yet the body reads r4-r7 (Fv-with-hidden-params
// shape, see MWCC_PATTERNS.md) - defined via extern "C" under the exact retail
// mangled name with explicit ABI params.
// OPEN ITEM PACKET (us-804831e4, best mismatch=271 structural=225 reg_swap=46,
// 1056B vs 1092B): semantics fully decoded - adjacency search of idB in
// idA's edge list, then rect-intersection boundary-point selection over the
// two scaled cell rects, then normalize(cx-pos.x, staleY, cz-pos.z).
// Retail symbol ends Fv yet reads r4-r7 (Fv-with-hidden-params shape):
// defined via extern "C" under the exact retail mangled name.
// volatile on the eight rect locals + cy was the key lever so far (size
// 872->1056B, structural 245->225): retail keeps every rect intermediate in
// a frame slot (stfs/lfs pairs, frame 0x60) while plain locals stay in FPRs.
// Residual is pure scheduling/reloc-slot placement: conversion-buffer
// (0x4330 magic double lbl_eu_8066A8A0) and scale (lbl_eu_8066A898) SDA21
// relocs land at shifted offsets, and the epilogue zero-copy/PSVECNormalize
// order differs slightly. Ruled out: bool-vs-s32 adjacent flag, cached nodes
// pointer vs direct self->nodes indexing (direct required - kills the r31
// callee-saved spill), u16-vs-u32 idB (u32 required - no rlwinm mask).
// Also RULED OUT: volatile on cx/cz (overshoots size to 1104B, mismatch 275
// vs 271 - retail keeps the chosen point in FPRs until the epilogue store).
// Next experiments: (a) drop volatile from the phase-A (na) group only;
// (b) reorder cx/cz assignments to interleave like retail epilogue;
// (c) try hi/lo ternaries as fmin/fmax-style if/else statements.
extern "C" void func_8047F214__17UnkClass_8047E110Fv(UnkClass_8047E110* self,
    ml::CVec3* out, ml::CVec3* pos, u32 idA, u32 idB);

// func_8047F214 -- resolve the transition point between walk cells idA and
// idB: confirm idB is listed in idA's edge list, intersect the two cell
// rects (scaled to world units), pick the shared boundary point, and write
// the normalized offset from pos toward it into out. Degenerate offsets
// collapse to epsilon / zero vectors.
extern "C" void func_8047F214__17UnkClass_8047E110Fv(UnkClass_8047E110* self,
    ml::CVec3* out, ml::CVec3* pos, u32 idA, u32 idB) {
    ScnWalkNode& nb = self->nodes[idB];
    const f32 S = lbl_eu_8066A898;
    ScnWalkNode& na = self->nodes[idA];
    u16* it = self->edges + na.edgeOffset;

    // rect of the queried node in world units (volatile: retail keeps these
    // in frame slots across the adjacency search)
    volatile f32 bx0 = S * (f32)nb.x;
    volatile f32 bz0 = S * (f32)nb.z;
    volatile f32 bx1 = S * (f32)(nb.width + 1) + bx0;
    volatile f32 bz1 = S * (f32)(nb.depth + 1) + bz0;

    // adjacency check: does idB appear in idA's edge list?
    s32 adjacent = 0;
    s32 count = *it++;
    for (; count != 0; --count) {
        if (*it == idB) {
            adjacent = 1;
            break;
        }
        ++it;
    }

    f32 cx, cz; // chosen transition point
    if (adjacent == 0) {
        // not connected: fall back to the centre of the queried node's rect
        cx = lbl_eu_8066A898 * (bx0 + bx1);
        cz = lbl_eu_8066A898 * (bz0 + bz1);
    } else {
        volatile f32 ax0 = lbl_eu_8066A898 * (f32)na.x;
        volatile f32 az0 = lbl_eu_8066A898 * (f32)na.z;
        volatile f32 ax1 = lbl_eu_8066A898 * (f32)(na.width + 1) + ax0;
        volatile f32 az1 = lbl_eu_8066A898 * (f32)(na.depth + 1) + az0;
        volatile f32 cy = lbl_eu_8066A898 * (na.xPos + nb.xPos);
        if (ax1 == bx0) {
            f32 hi = (ax1 >= bz1) ? bz1 : ax1;
            f32 lo = (az0 <= bz0) ? bz0 : az0;
            cz = lbl_eu_8066A898 * (hi + lo);
            f32 pz = pos->z;
            cx = (lo < pz && pz < hi) ? bx1 : ax1;
        } else if (bx1 == ax0) {
            f32 hi = (ax1 >= bz1) ? bz1 : ax1;
            f32 lo = (az0 <= bz0) ? bz0 : az0;
            cz = lbl_eu_8066A898 * (hi + lo);
            f32 pz = pos->z;
            cx = (lo < pz && pz < hi) ? bx0 : bx1;
        } else if (ax1 == bz0) {
            f32 hi = (az1 >= bx1) ? bx1 : az1;
            f32 lo = (ax0 <= bx0) ? bx0 : ax0;
            cx = lbl_eu_8066A898 * (hi + lo);
            f32 px = pos->x;
            cz = (lo < px && px < hi) ? bz0 : bz1;
        } else {
            // degenerate corner-touch case
            cx = bx1 + bx0;
            cz = cy + bz0;
        }
    }

    // aim vector from pos toward the chosen point
    out->x = cx - pos->x;
    out->z = cz - pos->z;
    if (out->x != lbl_eu_8066A8AC || out->z != lbl_eu_8066A8AC) {
        // NOTE: reads the stale out->y on purpose (retail behavior)
        if (out->y * out->y + out->x * out->x + out->z * out->z
            == lbl_eu_8066A8AC) {
            *out = ml::CVec3::zero;
        } else {
            PSVECNormalize((const Vec*)out, (Vec*)out);
        }
    } else {
        out->x = lbl_eu_8066A8AC;
        out->y = lbl_eu_8066A8AC;
        out->z = lbl_eu_8066A8AC;
    }
}

// func_804819C4 -- walk-step driver: ask the walker callback whether the
// queried cell is acceptable; otherwise greedily step to the neighbouring
// cell sharing the boundary the travel direction points at (up to 90 steps,
// re-asking the callback at every candidate). Each neighbour is classified
// by which pair of cell rects touch (left/right along x, below/above along
// z) and gated by the corresponding direction component sign.
//
// OPEN ITEM PACKET (us-80485994, best mismatch=274 structural=210 reg_swap=64,
// 1192B vs 1188B): semantics fully decoded (see body); frame/prologue shape
// matches retail (individual r28-r31 + f29-f31 saves, no _savegpr outliner,
// ptmf via __ptmf_scall twice). Residual is frame-slot layout + scheduling:
// retail frame is -0xa0 vs decomp -0x80; retail keeps FOUR extra f32 temps in
// memory slots (overlap-window hi/lo at 0x34/0x28 and the boundary coordinate
// stored TWICE at 0x20/0x2c), with a regular every-other-slot 4-byte gap
// pattern across all 14 float slots. Ruled out: volatile on bxA/bxB (+56B,
// loses constant hoisting), volatile on zHi/zLo only (296 mism.), volatile on
// everything incl. per-case xHi/xLo (+144B, drops f29-f31 callee caching and
// reloads S/EPS/magic inside the loop), cached pmf reference local (extra
// callee-saved GPR -> _savegpr_27 outliner; inline (self->**(ScnWalkFn*)&m
// .field_0x9C) at both call sites instead - REQUIRED). Next experiments:
// (a) recover pre-incident TU then re-diff (-ipa file scheduling);
// (b) try declaring the window/boundary temps as separate variables PER CASE
// (8 distinct names instead of 4 shared) to reproduce retail's slot set;
// (c) try ternaries for zHi/zLo instead of if/else to shift scheduling.
// SESSION 2 ADDITIONS (ruled out): volatile on bxA/bxB+zHi/zLo together
// (+112B, frame -144), per-case distinct names aHi/aLo/bHi/bLo (byte-identical
// to shared - MWCC merges them), double-typed Z-case bounds xHiD/xLoD (+24B,
// 283 mism), hoisting the maxX==cMinX compare into a bool flag ahead of the
// other cand-rect slots (286 mism - MWCC's early first-compare is NOT from a
// source-level flag). Reg-mapping shows decomp int callee-saved assignment is
// shifted +1 vs retail (retail self/best/list/idx = r28/r29/r30/r31, decomp =
// r29/r30/r31+...) - one fewer live int value; retail keeps FOUR extra f32
// temps in memory (window hi/lo + boundary stored twice) that our source
// leaves in FPRs, explaining both the -0x80-vs--0xa0 frame gap and the temp
// spilling. Next experiments: (a) restore pre-incident TU (-ipa scheduling);
// (b) find a source shape that adds exactly one live value / four spilled
// float temps without volatility (e.g. temps live-across-the-callback via a
// restructured hit path); (c) kb search 'spill f32 temps register pressure
// frame layout' patterns.
extern "C" s32 func_804819C4__17UnkClass_8047E110Fv(UnkClass_8047E110* self,
    s32 id) {
    ScnManagerLayout& m = *(ScnManagerLayout*)self;
    if ((self->**(ScnWalkFn*)&m.field_0x9C)(&m.nodes[id]))
        return 1;

    const f32 S = lbl_eu_8066A898;   // cell-size scale
    const f32 EPS = lbl_eu_8066A8AC; // epsilon
    s32 best;
    for (s32 i = 0; i < 90; ++i) {
        ScnWalkNode& nd = m.nodes[id];
        volatile f32 minX = S * (f32)nd.x;
        volatile f32 minZ = S * (f32)nd.z;
        volatile f32 maxX = S * (f32)(nd.width + 1) + minX;
        volatile f32 maxZ = S * (f32)(nd.depth + 1) + minZ;
        u16 cnt = m.edges[nd.edgeOffset];
        u16* rec = &m.edges[nd.edgeOffset + 1];
        best = -1;
        for (; cnt != 0; --cnt) {
            u32 nid = *rec++;
            ScnWalkNode& cn = m.nodes[nid];
            if (cn.reserved8 & 2)
                continue;
            volatile f32 cMinX = S * (f32)cn.x;
            volatile f32 cMinZ = S * (f32)cn.z;
            volatile f32 cMaxX = S * (f32)(cn.width + 1) + cMinX;
            volatile f32 cMaxZ = S * (f32)(cn.depth + 1) + cMinZ;

            f32 bxA, bxB; // boundary coordinate
            f32 zHi, zLo; // shared-overlap window
            if (maxX == cMinX) {
                // neighbour left of us: need forward -x motion
                if (!(m.dir.x > EPS))
                    continue;
                if (maxZ >= cMaxZ)
                    zHi = maxZ;
                else
                    zHi = cMaxZ;
                if (minZ <= cMinZ)
                    zLo = cMinZ;
                else
                    zLo = minZ;
                bxA = maxX;
                bxB = maxX;
                if (m.field_0x88 == EPS)
                    continue;
                f32 y = m.field_0x88 * (bxB - m.field_0x54) + m.field_0x5C;
                if (zHi < y || zLo > y)
                    continue;
            } else if (cMaxX == minX) {
                // neighbour right of us: need forward +x motion
                if (!(m.dir.x < EPS))
                    continue;
                if (maxZ >= cMaxZ)
                    zHi = maxZ;
                else
                    zHi = cMaxZ;
                if (minZ <= cMinZ)
                    zLo = cMinZ;
                else
                    zLo = minZ;
                bxA = cMaxX;
                bxB = cMaxX;
                if (m.field_0x88 == EPS)
                    continue;
                f32 y = m.field_0x88 * (bxB - m.field_0x54) + m.field_0x5C;
                if (zHi < y || zLo > y)
                    continue;
            } else if (cMinZ == maxZ) {
                // neighbour below us: need forward -z motion
                if (!(m.dir.z > EPS))
                    continue;
                f32 xHi = (maxX >= cMaxX) ? maxX : cMaxX;
                f32 xLo = (minX <= cMinX) ? cMinX : minX;
                bxA = xLo;
                bxB = xHi;
                zHi = maxZ;
                zLo = maxZ;
                if (m.field_0x8C == EPS)
                    continue;
                f32 y = m.field_0x8C * (zLo - m.field_0x5C) + m.field_0x54;
                if (bxB < y || bxA > y)
                    continue;
            } else {
                if (cMaxZ != minZ)
                    continue;
                // neighbour above us: need forward +z motion
                if (!(m.dir.z < EPS))
                    continue;
                f32 xHi = (maxX >= cMaxX) ? maxX : cMaxX;
                f32 xLo = (minX <= cMinX) ? cMinX : minX;
                bxA = xLo;
                bxB = xHi;
                zHi = cMaxZ;
                zLo = cMaxZ;
                if (m.field_0x8C == EPS)
                    continue;
                f32 y = m.field_0x8C * (zLo - m.field_0x5C) + m.field_0x54;
                if (bxB < y || bxA > y)
                    continue;
            }

            if ((self->**(ScnWalkFn*)&m.field_0x9C)(&cn))
                return 1;
            best = nid;
            break;
        }
        if (best == -1)
            return 0;
        id = best;
    }
    return 0;
}

// func_804808A0 -- walk search: breadth-first expansion from startId toward
// goalId over axis-aligned adjacent cells. For every edge the shared boundary
// is identified (which pair of cell sides touch), the walker's travel line
// through pos (slope dz/dx or dx/dz) is projected onto that boundary, and the
// projection must fall inside the shared-overlap window (+/- 1 tolerance).
// Among passing neighbours the one whose boundary point is closest to pos is
// kept as the next frontier; reaching goalId returns 1. Returns 0 when the
// frontier dies, a non-axis-aligned edge is met, or 90 levels run out.
// NOTE: retail symbol ends in Fv yet reads r3-r6 (Fv-with-hidden-params
// shape) - defined via extern "C" under the exact retail mangled name.
extern "C" s32 func_804808A0__17UnkClass_8047E110Fv(UnkClass_8047E110* self,
    const ml::CVec3* pos, s32 startId, s32 goalId) {
    ScnManagerLayout& m = *(ScnManagerLayout*)self;
    if (startId == goalId)
        return 1;

    const f32 S = lbl_eu_8066A898;    // sub-cell scale
    const f32 CELL = lbl_eu_8066A8A8; // cell-pitch scale / boundary tolerance
    const f32 EPS = lbl_eu_8066A8AC;  // degenerate-slope epsilon

    // world-space anchor of the goal cell and its offset from pos
    const ScnWalkNode& gn = m.nodes[goalId];
    ml::CVec3 anchor;
    anchor.y = pos->y;
    anchor.z = S * (f32)gn.z + CELL * (f32)(gn.depth + 1);
    anchor.x = S * (f32)gn.x + CELL * (f32)(gn.width + 1);
    ml::CVec3 off = anchor - *pos;

    // travel-line slopes through pos (defaulted to epsilon when axis-degenerate)
    f32 slopeX = EPS; // dz/dx
    if (off.x != EPS)
        slopeX = off.z / off.x;
    f32 slopeZ = EPS; // dx/dz
    if (off.z != EPS)
        slopeZ = off.x / off.z;

    // pos +/- window along each axis
    f32 hiX = anchor.x >= pos->x ? anchor.x : pos->x;
    f32 loX = anchor.x >= pos->x ? pos->x : anchor.x;
    f32 hiZ = anchor.z >= pos->z ? anchor.z : pos->z;
    f32 loZ = anchor.z >= pos->z ? pos->z : anchor.z;

    // ping-pong BFS frontiers
    u32 bufA[10];
    u32 bufB[10];
    bufB[0] = startId;
    u32* cur = bufB;
    u32* nxt = bufA;
    s32 level = 1;
    f32 bestDist2 = lbl_eu_8066A8B8;
    for (s32 n = 0; n < 90; ++n) {
        if (level == 0)
            return 0;
        s32 produced = 0;
        for (s32 i = 0; i < level; ++i) {
            s32 id = *cur++;
            ScnWalkNode& nd = m.nodes[id];
            volatile f32 minX = S * (f32)nd.x;
            volatile f32 minZ = S * (f32)nd.z;
            volatile f32 maxX = S * (f32)(nd.width + 1) + minX;
            volatile f32 maxZ = S * (f32)(nd.depth + 1) + minZ;
            u16 cnt = m.edges[nd.edgeOffset];
            u16* rec = &m.edges[nd.edgeOffset + 1];
            for (; cnt != 0; --cnt) {
                u32 nb = *rec++;
                ScnWalkNode& cn = m.nodes[nb];
                volatile f32 cMinX = S * (f32)cn.x;
                volatile f32 cMinZ = S * (f32)cn.z;
                volatile f32 cMaxX = S * (f32)(cn.width + 1) + cMinX;
                volatile f32 cMaxZ = S * (f32)(cn.depth + 1) + cMinZ;

                f32 bxA, bxB; // boundary point coordinates
                f32 proj;
                if (maxX == cMinX) {
                    // neighbour shares our +x edge; project onto x = maxX
                    if (hiX < maxX - CELL || loX > maxX + CELL)
                        continue;
                    volatile f32 hi = maxZ < cMaxZ ? maxZ : cMaxZ;
                    volatile f32 lo = minZ > cMinZ ? minZ : cMinZ;
                    bxA = maxX;
                    bxB = maxX;
                    if (slopeX == EPS)
                        continue;
                    proj = slopeX * (maxX - anchor.x) + anchor.z;
                    if (hi - CELL < proj || lo + CELL > proj)
                        continue;
                } else if (cMaxX == minX) {
                    // neighbour shares our -x edge; project onto x = cMaxX
                    if (hiX < cMaxX - CELL || loX > cMaxX + CELL)
                        continue;
                    volatile f32 hi = maxZ < cMaxZ ? maxZ : cMaxZ;
                    volatile f32 lo = minZ > cMinZ ? minZ : cMinZ;
                    bxA = cMaxX;
                    bxB = cMaxX;
                    if (slopeX == EPS)
                        continue;
                    proj = slopeX * (cMaxX - anchor.x) + anchor.z;
                    if (hi - CELL < proj || lo + CELL > proj)
                        continue;
                } else if (maxZ == cMinZ) {
                    // neighbour shares our +z edge; project onto z = maxZ
                    if (hiZ < maxZ - CELL || loZ > maxZ + CELL)
                        continue;
                    bxB = loX < cMaxX ? loX : cMaxX;
                    bxA = minX < cMinX ? minX : cMinX;
                    volatile f32 bz = maxZ;
                    if (slopeZ == EPS)
                        continue;
                    proj = slopeZ * (maxZ - anchor.z) + anchor.x;
                    if (bxB - CELL < proj || bxA + CELL > proj)
                        continue;
                } else if (cMaxZ == minZ) {
                    // neighbour shares our -z edge; project onto z = cMaxZ
                    if (hiZ < cMaxZ - CELL || loZ > cMaxZ + CELL)
                        continue;
                    bxB = loX < cMaxX ? loX : cMaxX;
                    bxA = minX < cMinX ? minX : cMinX;
                    volatile f32 bz = cMaxZ;
                    if (slopeZ == EPS)
                        continue;
                    proj = slopeZ * (cMaxZ - anchor.z) + anchor.x;
                    if (bxB - CELL < proj || bxA + CELL > proj)
                        continue;
                } else {
                    // edges only connect axis-aligned neighbours
                    return 0;
                }

                // keep the closest boundary point to pos
                f32 dy2 = (pos->z - proj) * (pos->z - proj);
                f32 dist2 = dy2 + (pos->x - bxA) * (pos->x - bxA);
                if (bestDist2 <= dist2)
                    continue;
                bestDist2 = dist2;
                if (nb == goalId)
                    return 1;
                nxt[produced++] = nb;
            }
        }
        // ping-pong the frontier buffers
        u32* t = cur;
        cur = nxt;
        nxt = t;
        level = produced;
    }
    return 0;
}

// func_8047E6C4 -- reachability query against the walk cell `cellIdx`.
// The margin-offset point must lie fully OUTSIDE the cell rect while the raw
// point lies fully INSIDE it (both with the vertical plane threshold); that
// combination, or the raw point alone when the cell is a portal/box-active,
// is an immediate hit. Otherwise scan the cell's edge list (same visibility
// gate as func_8047E390) for the best adjacent cell and resolve it through
// func_8047E1B0.
extern "C" s32 func_8047E6C4__17UnkClass_8047E110Fv(UnkClass_8047E110* self,
    u32* outId, u32* outCell, u32 arg6, s32 cellIdx, const ml::CVec3* pos,
    f32 margin) {
    ScnManagerLayout& m = *(ScnManagerLayout*)self;
    const ScnWalkNode& nd = m.nodes[cellIdx];
    f32 pxHi = pos->x + margin; // f0
    f32 pzHi = pos->z + margin; // f2
    f32 loX = lbl_eu_8066A898 * (f32)nd.x;
    f32 loZ = lbl_eu_8066A898 * (f32)nd.z;
    f32 hiX = lbl_eu_8066A898 * (f32)(nd.x + nd.width + 1);
    f32 hiZ = lbl_eu_8066A898 * (f32)(nd.z + nd.depth + 1);
    f32 pxLo = pos->x - margin; // f3
    f32 pzLo = pos->z - margin; // f1
    s32 hitCenter = 0;          // r12: center-inside but flags rejected

    // margin-expanded point inside the cell rect, vertical threshold ok
    if (pxHi >= loX && pzHi >= loZ && pxLo <= hiX && pzLo <= hiZ
        && __fabs(pos->y - nd.xPos) <= m.field_0x20) {
        if ((nd.reserved8 & 2) || (m.field_0x38 & 1)) {
            *outId = arg6;
            *outCell = cellIdx;
            return 1;
        }
        hitCenter = 1;
    }

    // raw point inside the rect: unconditional hit
    if (pos->x >= loX && pos->z >= loZ && pos->x <= hiX && pos->z <= hiZ
        && __fabs(pos->y - nd.xPos) <= m.field_0x20) {
        *outId = arg6;
        *outCell = cellIdx;
        return 1;
    }

    // scan the cell's edge list for an adjacent reachable node
    u16* list = m.edges + nd.edgeOffset;
    s32 count = *list++;
    const ScnWalkNode* nodes = m.nodes;
    s32 best = -1; // r31
    for (s32 i = 0; i < count; ++i) {
        u16 id = *list++;
        u32 flags = m.field_0x38;
        const ScnWalkNode& cn = nodes[id];
        // visibility gate (same shape as func_8047E390)
        if (!(flags & 1)) {
            if (cn.reserved8 & 2)
                continue;
        }
        if (!(flags & 2)) {
            if (cn.reserved8 & 4)
                continue;
        } else if (!(flags & 4)) {
            if (cn.reserved8 & 4)
                continue;
        }
        // expanded candidate rect must overlap [point-margin, point+margin]
        f32 minX = lbl_eu_8066A898 * (f32)cn.x;
        f32 minZ = lbl_eu_8066A898 * (f32)cn.z;
        if (pxHi < minX)
            continue;
        if (pzHi < minZ)
            continue;
        f32 maxX = lbl_eu_8066A898 * (f32)(cn.x + cn.width + 1);
        f32 maxZ = lbl_eu_8066A898 * (f32)(cn.z + cn.depth + 1);
        if (pxLo >= maxX)
            continue;
        if (pzLo >= maxZ)
            continue;
        f32 dist = __fabs(pos->y - cn.xPos);
        if (dist >= m.field_0x20)
            continue;
        if (cn.reserved8 & 2) {
            // flagged neighbour: only accept when the vertical step between
            // the two cells stays within the up/down limits
            if (!(flags & 1))
                continue;
            f32 cy = cn.xPos;
            f32 ty = nd.xPos;
            if (cy < ty) {
                if (ty - cy > m.field_0x24)
                    continue;
            } else if (cy > ty) {
                if (cy - ty > m.field_0x28)
                    continue;
            }
            best = id;
            break;
        } else {
            // unflagged neighbour: raw point must be inside its rect
            if (pos->x < minX || pos->z < minZ || pos->x > maxX || pos->z > maxZ)
                continue;
            best = id;
            break;
        }
    }

    if (best != -1) {
        s32 res = func_8047E1B0__17UnkClass_8047E110Fv(self, pos);
        *outId = res;
        if (res == -1)
            return 0;
        *outCell = best;
        return 1;
    }
    if (hitCenter) {
        *outId = arg6;
        *outCell = cellIdx;
        return 1;
    }
    return 0;
}

// func_8047F730 -- bidirectional reachability search over the walk graph.
// The shared cursor buffer holds two seed sets: the first `count` ids belong
// to search A (label-table entries seeded 0x8000) and the following `arg6`
// ids to search B (seeded 0x4000); labels live at manager+0x94, two halfwords
// per node id. A pooled bitmap marks visited ids. Every level expands each
// frontier node through BOTH endpoints of its map record's edge lists,
// appending unvisited neighbours to the opposite half of the cursor buffer.
// When one level produces expansions whose accumulated labels carry BOTH
// seed markers, the searches met: func_8047EFBC resolves the crossing node
// and its result becomes ours. Frontier exhaustion returns 0.
//
// OPEN ITEM PACKET (us-80483700, best mismatch=294 structural=282
// reg_swap=12, 1384B vs 1816B): preamble + phase-1 unrolled seed loop are
// BYTE-IDENTICAL (incl. register allocation). Divergence starts at loop 2:
// decomp allocates the frontier-seed/bound temp (retail r8) into r7 and
// hoists the strength-reduced walker (cur[count]) above the unroll guard
// chain, while retail sinks it below and materializes FOUR guard-flag
// booleans (li r4/r5/r7/r9) fed by a cr1 compare reused between the early
// exit and the overflow-proof chain. Semantics fully decoded (bidirectional
// BFS, labels[v] halfword per id, 0x8000/0x4000 seed markers, meet test
// acc&0xC000==0xC000 -> func_8047EFBC(self, arg4, cur[j]) return-through).
// Ruled out: labels[v*2] indexing (wrong - byte offset is v*2 = labels[v]);
// i-continuation loop 2 (reg_swap explodes to 116); pointer-walk q loop
// (walker hoists even higher); mutating-count walk form; produced==end
// merged variable (byte-identical to separate end); cached cur[j] id local
// (retail reloads lhz every use); ternary grp select (indexed (&f8)[d]
// required). Key insight kept: loop 2 must mutate `count` itself so the
// induction stays in r24 (matches retail addi r24,+8 in unrolled body).
// Next experiments: (a) find a loop-2 bound expression shape that makes MWCC
// reuse the loop-condition cr1 compare inside its overflow-proof chain;
// (b) try declaring the seed constants (0x8000/1) as named locals so they
// stay live across the loop boundary and push allocation to r8;
// (c) re-check after any unit flag negotiation (unroll guard shapes are
// -O4,p sensitive).
extern "C" s32 func_8047F730__17UnkClass_8047E110Fv(UnkClass_8047E110* self,
    u16* arg4, s32 count, s32 arg6) {
    ScnManagerLayout& m = *(ScnManagerLayout*)self;
    u16* labels = (u16*)m.field_0x94;
    u32* bits = (u32*)func_804B5A68();
    u16* cur = (u16*)m.field_0x98;
    u32 n = ((ScnWalkHeader*)m.field_0x0)->field_0x10;
    u16* mid = &cur[(n >> 1) + 10];
    memset(bits, 0, ((n >> 5) + 1) * 4);

    // seed pass: mark every seed id as visited and stamp its marker label
    s32 i;
    for (i = 0; i < count; ++i) {
        u16 v = cur[i];
        bits[v >> 5] |= 1 << (v & 31);
        labels[cur[i]] = 0x8000;
    }
    s32 produced = count + arg6;
    for (; count < produced; ++count) {
        u16 v = cur[count];
        bits[v >> 5] |= 1 << (v & 31);
        labels[cur[count]] = 0x4000;
    }

    while (produced != 0) {
        s32 lvl = produced;
        produced = 0;
        u32 acc = 0; // OR of all newly written labels (marker-bit detector)
        u16* wr = mid;
        for (s32 j = 0; j < lvl; ++j) {
            const ScnNodeMap& rec = m.field_0xC[cur[j]];
            for (s32 ep = 0; ep < 2; ++ep) {
                u8 g = (&rec.field_0x8)[ep];
                u16 coord = (&rec.field_0x4)[ep];
                u32 eo = m.field_0x8[coord].field_0x4;
                u16 ecnt = m.edges[eo];
                const u16* el = m.edges + eo + 1;
                for (s32 k = 0; k < ecnt; ++k) {
                    u16 nb = *el++;
                    const ScnNodeMap& lk = m.field_0xC[nb];
                    // neighbour record side must agree with ours for coord
                    u8 g2 = (&lk.field_0x8)[coord != lk.field_0x4];
                    if (g2 != g)
                        continue;
                    u32 mask = 1 << (nb & 31);
                    if (bits[nb >> 5] & mask) {
                        // already labelled this run: exactly one side carrying
                        // the A-marker means the frontiers crossed here
                        if (labels[nb] & 0x8000) {
                            if (!(labels[cur[j]] & 0x8000))
                                return func_8047EFBC__17UnkClass_8047E110Fv(
                                    self, arg4, cur[j]);
                        } else if (labels[cur[j]] & 0x8000) {
                            return func_8047EFBC__17UnkClass_8047E110Fv(
                                self, arg4, cur[j]);
                        }
                        continue;
                    }
                    bits[nb >> 5] |= mask;
                    wr[produced] = nb;
                    ++produced;
                    labels[nb] = labels[cur[j]] + 1;
                    acc |= labels[nb];
                }
            }
        }
        if ((acc & 0xC000) != 0xC000)
            return 0;
        // ping-pong the frontier halves inside the shared cursor buffer
        u16* t = cur;
        cur = mid;
        mid = t;
    }
    return 0;
}
