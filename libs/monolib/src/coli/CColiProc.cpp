// Translation unit: monolib/src/coli/CColiProc
// Contains CColiProc constructor and field-management helpers.

#include <types.h>
#include <nw4r/math/math_types.h>
#include <revolution/mtx.h>
#include <revolution/mtx/mtxvec.h>
#include <cstring>
#include "monolib/coli/CColiProc.hpp"
#include "monolib/coli/code_804B2FF0.hpp"
#include "monolib/util/MemManager.hpp"

// CColiProcTarget/CColiProc definitions live in CColiProc.hpp (single source).

// Context/link struct passed to ColiProc_ProcessSegments; only 0x3C is referenced here.
struct CColiLinkObj {
    u8 field_0x0[0x3C];
    CColiProc* field_0x3C;
};

// The single global CColiResCall instance (retail .bss 0x8065D0E8, size 0x50)
// is defined at the end of this file; its static initializer
// (retail __sinit_\CColiProc_cpp, .ctors 0x804FA16C) runs the CColiProc base
// ctor and then installs __vt__12CColiResCall into the instance.
CColiResCall g_coliResCall;
extern "C" const f32 lbl_eu_8066AEA8;
extern "C" int ColiProc_ProcessSegments(CColiWork* work,
                                         struct CColiObjDispatch* obj);

// Object-view types the shared bounds/transform globals point at; defined with
// their real layouts in the code_804B2FF0 unit (see
// monolib/coli/code_804B2FF0.hpp), used here only as opaque pointers.
struct CColiBounds;
struct CColiQx;
struct CColiHx;

void CColiWork::InitTriple(CColiProcTarget* target, u32 a) {
    this->field_0x0 = target;
    this->field_0x4 = a;
    this->field_0xC = 0;
}

void CColiWork::InitQuad(CColiProcTarget* target, u32 a, u32 b) {
    this->field_0x0 = target;
    this->field_0x4 = a;
    this->field_0x8 = b;
    this->field_0xC = 0;
}

// Returns true if field_0x0 is non-null and bit 0 of its u16 at offset 6 is set.
// Both failure paths share a single return-0 tail to match retail branch layout.
bool CColiWork::IsTargetFlagged() {
    if (this->field_0x0 != nullptr && (this->field_0x0->field_0x6 & 1)) {
        return true;
    }
    return false;
}

// Returns bit 18 of field_0xC (extrwi extraction).
bool CColiWork::TestBit18State() {
    return ((this->field_0xC >> 18) & 1) != 0;
}

// --- ColiProc_ClassifyEntries dependencies ---

// Table header stored at the object's field_0x0: entries begin at +field_0x8
// (a byte offset from the table base) and there are field_0xC of them.
struct CColiTable {
    u8 field_0x0[0x8];
    u32 field_0x8; // byte offset to first entry
    u32 field_0xC; // entry count
};

// Variable-length entry chained via ColiSpecEntryStep[type]+16; data follows the
// 0x10-byte header (handler receives a pointer to field_0x10).
struct CColiEntry {
    u16 field_0x0; // type index into the exported tables
    u16 field_0x2; // flags: bit0 / bit1 / bit4
    u32 field_0x4; // absolute-ish data offset (added to the table base)
    u8 field_0x8[0x8];
    u8 field_0x10; // raw data blob
};

// Per-type metadata and handler vectors (retail .data tables).
// ColiSpecEntryStep      : s16 per-entry step size (+16 byte header), index by type
// ColiSpecEntrySize      : s16 per-entry data size, index by type
// ColiSpecDataHandlers      : handler function-pointer vector, index by type

extern s16 ColiSpecEntryStep[];
extern s16 ColiSpecEntrySize[];
typedef void (*CColiDataFn)(u8* dst, u32 src, u32 data, u8* raw);
extern CColiDataFn ColiSpecDataHandlers[];

// The generic work object the collision walkers run against is CColiWork
// (CColiProc.hpp); its +0x00 slot holds the target table.
static inline CColiTable* colitable(CColiWork* work) {
    return (CColiTable*)work->field_0x0;
}


extern u32 g_coliSpecTable;
extern CColiBounds* g_coliBounds;
extern CColiQx* g_coliXformPos;
extern CColiHx* g_coliHalfExtent;
extern s16 g_coliNodeIndex;

// Compute the packed buffer size for every entry, allocate it, then fill it
// by dispatching each entry's data through ColiSpecDataHandlers[type]. Returns an
// index/count code: 0 when no selection was made yet, else 2 (0 selected) or 1.
extern "C" int ColiProc_ClassifyEntries(CColiWork* work, u16* outIndex, void** outBuf,
                             CColiHx* arg4) {
    CColiTable* table = colitable(work);
    g_coliSpecTable = (u32)table;
    g_coliHalfExtent = arg4;

    // Pass 1: accumulate the total byte size of all entries.
    s32 total = 0;
    CColiEntry* e = (CColiEntry*)((u8*)table + table->field_0x8);
    u32 entryCount = table->field_0xC;
    for (u32 i = 0; i < entryCount; i++) {
        u16 type = e->field_0x0;
        u16 flags = e->field_0x2;
        // Advance to the next entry first (chained by C0[type]+16).
        e = (CColiEntry*)((u8*)e + ColiSpecEntryStep[type] + 16);
        if ((flags & 2) && type != 8) {
            total += 2 * ColiSpecEntrySize[type] + 0xc0;
        } else {
            total += ColiSpecEntrySize[type];
        }
    }

    u8* buf = nullptr;
    if (total != 0) {
        buf = (u8*)mtl::MemManager::allocate_head(
            sEneColiProbe->field_0x7C, (u32)total, 0x20);
    }

    // Pass 2: dispatch each entry's handler and pack its data into buf.
    u32 cntBit0 = 0;
    u32 offset = 0;
    CColiEntry* q = (CColiEntry*)((u8*)colitable(work) + table->field_0x8);
    g_coliNodeIndex = -1;
    for (u32 i = 0; i < table->field_0xC; i++) {
        if (q->field_0x2 & 1) {
            cntBit0++;
        }
        if (q->field_0x2 & 0x10) {
            work->field_0xC |= 0x40000;
        }
        if ((q->field_0x2 & 2) && q->field_0x0 != 8) {
            u8* dst = buf + offset;
            ColiSpecDataHandlers[q->field_0x0](
                dst, (u32)colitable(work) + q->field_0x4, work->field_0x4,
                &q->field_0x10);
            memcpy(dst + (s32)ColiSpecEntrySize[q->field_0x0], dst,
                   (size_t)ColiSpecEntrySize[q->field_0x0]);
            offset += 2 * ColiSpecEntrySize[q->field_0x0] + 0xc0;
        } else {
            u8* dst = buf + offset;
            ColiSpecDataHandlers[q->field_0x0](
                dst, (u32)colitable(work) + q->field_0x4, work->field_0x4,
                &q->field_0x10);
            offset += ColiSpecEntrySize[q->field_0x0];
        }
        q = (CColiEntry*)((u8*)q + ColiSpecEntryStep[q->field_0x0] + 16);
    }

    *outBuf = buf;
    if (g_coliNodeIndex == -1) {
        return 0;
    }
    s32 sign = -(s32)cntBit0 | (s32)cntBit0;
    *outIndex = g_coliNodeIndex;
    return (sign >> 31) + 2;
}

// ---------------------------------------------------------------------------
// Sweep-link helper shared with the object units (retail ColiLinkSweepRecord,
// code_804A6C60 unit): records the dispatch class on the object for the entry.
extern "C" void ColiLinkSweepRecord(CColiObjDispatch* obj, u32 cls, u8* entry);

// --- ColiProc_DispatchSpec dependencies ---

// The collision object a spec dispatch runs against: only the two counters it
// clears and the dispatch byte ColiDispatchByIndex consumes are touched here.
struct CColiObjDispatch {
    u8 _00[0x210];
    u32 field_0x210;
    u8 _214[0x2d4 - 0x214];
    u32 field_0x2d4;
};

struct CColiSpecHead;       // spec table handed to ColiRegisterSubObjects
struct CColiDispatchTarget; // object the method-table dispatcher runs on

// Per-type dispatch class (s16 pair table, defined with the other coli tables).
extern s16 ColiSpecDispatchClass[];

// Per-type spec record sizes: step (next-entry advance), payload size and
// dispatch class. Accessors keep the offset tables spelled once.
static inline s32 colistep(u32 type) { return ColiSpecEntryStep[type]; }
static inline s32 colisize(u32 type) { return ColiSpecEntrySize[type]; }
static inline s32 coliclass(u32 type) { return ColiSpecDispatchClass[type]; }

// Sibling helpers (defined in the code_804A6C60 unit).
extern "C" void ColiRegisterSubObjects(void* unused, CColiSpecHead* spec);
extern "C" u32 ColiSetDispatchIndex(void* unused, u32 val);
extern "C" int ColiDispatchByIndex(CColiDispatchTarget* self, u32 val);

// Instance record head at the start of one work-buffer entry
// CColiHead: signed bone index + flag halfword). The matrix handlers take the
// record by pointer, so the head and its payload are addressed separately.
// ---------------------------------------------------------------------------
struct CColiInstHead {
    s16 index; //0x0
    u16 flags; //0x2
};

// Per-type matrix handlers (retail .data ColiSpecMtxHandlers): fill one
// instance record from the node's bone/root matrix. The expander family
// (ColiSpecExpandHandlers) grows a duplicated payload in place.
typedef void (*CColiMtxFn)(u8* inst, void* mgr, u8* data);
typedef void (*CColiExpandFn)(u8* inst);

extern CColiMtxFn ColiSpecMtxHandlers[];
extern CColiExpandFn ColiSpecExpandHandlers[];
extern "C" void Coli_ShiftMtxHist(CColiInstHead* head, void* dst, void* mgr);
extern "C" void Coli_CopyBoneMtx(CColiInstHead* head, void* dst, void* mgr);

// Rebuild the axes/matrices of every entry in the work object's table: seed
// the shared bounds / xform-position / half-extent globals and the node index
// from the caller's object, then walk the entries. The
// dual-buffer entries (flag bit 1, i.e. the types that carry a second copy of
// their payload) duplicate the record, expand the copy and rebuild the matrix
// pair when the instance head asks for it (bit 0), and only copy the bone
// matrix otherwise.
extern "C" void ColiProc_RefreshAxes(CColiWork* work, CColiBounds* bounds,
                                     CColiQx* q, CColiHx* h, s16 index) {
    CColiTable* table = colitable(work);
    CColiEntry* e = (CColiEntry*)((u8*)table + table->field_0x8);
    g_coliBounds = bounds;
    g_coliXformPos = q;
    g_coliHalfExtent = h;
    g_coliNodeIndex = index;
    g_coliSpecTable = (u32)table;
    u32 offset = 0;
    for (u32 i = 0; i < table->field_0xC; i++) {
        if (e->field_0x0 != 8 && (e->field_0x2 & 2)) {
            CColiInstHead* p = (CColiInstHead*)((u8*)work->field_0x8 + offset);
            if (p->flags & 1) {
                memcpy((u8*)(ColiSpecEntrySize[e->field_0x0] + (u32)p), p,
                       (size_t)ColiSpecEntrySize[e->field_0x0]);
                ColiSpecExpandHandlers[e->field_0x0](
                    (u8*)work->field_0x8 + offset +
                    ColiSpecEntrySize[e->field_0x0]);
                ColiSpecMtxHandlers[e->field_0x0](
                    (u8*)work->field_0x8 + offset, (void*)work->field_0x4,
                    &e->field_0x10);
                Coli_ShiftMtxHist(
                    p,
                    (u8*)work->field_0x8 + offset +
                        2 * ColiSpecEntrySize[e->field_0x0],
                    (void*)work->field_0x4);
            } else {
                ColiSpecMtxHandlers[e->field_0x0](
                    (u8*)work->field_0x8 + offset, (void*)work->field_0x4,
                    &e->field_0x10);
                memcpy((u8*)(ColiSpecEntrySize[e->field_0x0] +
                             (u32)(work->field_0x8 + offset)),
                       (u8*)work->field_0x8 + offset,
                       (size_t)ColiSpecEntrySize[e->field_0x0]);
                Coli_CopyBoneMtx(
                    p,
                    (u8*)work->field_0x8 + offset +
                        2 * ColiSpecEntrySize[e->field_0x0],
                    (void*)work->field_0x4);
            }
            offset += 2 * ColiSpecEntrySize[e->field_0x0] + 0xc0;
            e = (CColiEntry*)(colistep(e->field_0x0) + (u32)e + 16);
        } else {
            ColiSpecMtxHandlers[e->field_0x0](
                (u8*)work->field_0x8 + offset, (void*)work->field_0x4,
                &e->field_0x10);
            offset += ColiSpecEntrySize[e->field_0x0];
            e = (CColiEntry*)(colistep(e->field_0x0) + (u32)e + 16);
        }
    }
}
// Prepare the shared CColiProc instance against a work object, pass it to the
// generic position update, then (on success) add the instance's offet vector.
extern "C" u32 ColiProc_RunCollideScan(CColiWork* self, nw4r::math::VEC3* pVec,
                             CColiLinkObj* pLink, s32 flag) {
    CColiResCall* pColi = &g_coliResCall;
    f32 zero = lbl_eu_8066AEA8;

    pColi->src = 0;
    pColi->acc.x = zero;
    pColi->acc.y = zero;
    pColi->acc.z = zero;
    pColi->target = pLink;

    if (pColi != 0) {
        pLink->field_0x3C = pColi;
    } else {
        pLink->field_0x3C = (CColiProc*)((u8*)pLink + 4);
    }

    if (flag != 0) {
        self->field_0xC |= 1;
    }

    typedef int (*F2AA4)(CColiWork*, CColiLinkObj*);
    if (((F2AA4)ColiProc_ProcessSegments)(self, pLink) != 0) {
        nw4r::math::VEC3* pG =
            &g_coliResCall.acc;
        nw4r::math::VEC3Add(pVec, pVec, pG);
        return 1;
    }
    return 0;
}
// Sweep every entry of the work object's table through the object's method
// table: an entry whose selection bits match the work object's state is
// skipped (its payload is only reserve space), every other one first links
// the sweep record for its dispatch class, then runs the object's dispatcher
// at the running data offset. The dual-buffer entries (flag bit 1) dispatch
// the duplicated copy first and only fall back to the primary record when
// that misses; the accumulator makes the walk exhaustive instead of
// first-hit.
extern "C" int ColiProc_ProcessSegments(CColiWork* work, CColiObjDispatch* obj) {
    obj->field_0x210 = 0;
    obj->field_0x2d4 = 0;
    CColiTable* table = colitable(work);
    u32 result = 0;
    u32 offset = 0;
    u32 i;
    CColiEntry* e = (CColiEntry*)((u8*)table + table->field_0x8);
    ColiRegisterSubObjects(obj, (CColiSpecHead*)table);
    for (i = 0; i < table->field_0xC; i++) {
        u32 extra = 0;
        if (e->field_0x0 != 8) {
            bool sel = false;
            if (e->field_0x2 & 0x1c) {
                if (e->field_0x2 & 0x10) {
                    sel = true;
                } else if ((work->field_0xC & 1) && (e->field_0x2 & 4)) {
                    sel = true;
                } else if ((work->field_0xC & 4) && (e->field_0x2 & 8)) {
                    sel = true;
                }
            }
            if (sel) {
                if (e->field_0x2 & 2) {
                    extra = (u32)colisize(e->field_0x0) + 0xc0;
                }
                offset += (u32)colisize(e->field_0x0) + extra;
                e = (CColiEntry*)(colistep(e->field_0x0) + (u32)e + 16);
                continue;
            }
            ColiLinkSweepRecord(obj, (u32)coliclass(e->field_0x0), (u8*)e);
            if (e->field_0x2 & 2) {
                if (work->field_0xC & 2) {
                    g_coliResCall.src = (CColiMtxSrc*)(
                        work->field_0x8 + offset +
                        2 * (u32)colisize(e->field_0x0));
                    if (ColiDispatchByIndex(
                            (CColiDispatchTarget*)obj,
                            work->field_0x8 + offset +
                                (u32)colisize(e->field_0x0)) != 0) {
                        result |= 1;
                        offset += 2 * (u32)colisize(e->field_0x0) + 0xc0;
                        e = (CColiEntry*)(colistep(e->field_0x0) + (u32)e + 16);
                        continue;
                    }
                }
                extra = (u32)colisize(e->field_0x0) + 0xc0;
            }
            g_coliResCall.src = (CColiMtxSrc*)0;
            result |= (u32)ColiDispatchByIndex((CColiDispatchTarget*)obj,
                                               work->field_0x8 + offset);
        }
        offset += (u32)colisize(e->field_0x0) + extra;
        e = (CColiEntry*)(colistep(e->field_0x0) + (u32)e + 16);
    }
    return result != 0;
}
// Same spec walk as ColiProc_DispatchSpec, but the *matched* entries are the
// ones the walk passes over: an entry is skipped when its +0x2 bits 2-3
// intersect the work object's selection state (+0xC bit 0 against entry bit 2,
// state bit 2 against entry bit 3); every other entry goes to the method table.
extern "C" int ColiProc_CheckEntries(CColiWork* work, CColiObjDispatch* obj) {
    obj->field_0x210 = 0;
    obj->field_0x2d4 = 0;
    CColiTable* table = colitable(work);
    CColiEntry* e = (CColiEntry*)((u8*)table + table->field_0x8);
    ColiRegisterSubObjects(obj, (CColiSpecHead*)table);
    u32 offset = 0;
    for (u32 i = 0; i < table->field_0xC; i++) {
        u32 extra = 0;
        if (e->field_0x0 != 8) {
            if (e->field_0x2 & 0xC) {
                u32 state = work->field_0xC;
                bool sel = false;
                if ((state & 1) && (e->field_0x2 & 4)) {
                    sel = true;
                } else if ((state & 4) && (e->field_0x2 & 8)) {
                    sel = true;
                }
                if (sel) {
                    if (e->field_0x2 & 2) {
                        extra = (u32)colisize(e->field_0x0) + 0xc0;
                    }
                    offset += (u32)colisize(e->field_0x0) + extra;
                    e = (CColiEntry*)(colistep(e->field_0x0) + (u32)e + 16);
                    continue;
                }
            }
            ColiSetDispatchIndex(obj, (u32)coliclass(e->field_0x0));
            if (e->field_0x2 & 2) {
                extra = (u32)colisize(e->field_0x0) + 0xc0;
            }
            if (ColiDispatchByIndex((CColiDispatchTarget*)obj,
                                    work->field_0x8 + offset) != 0) {
                return 1;
            }
        }
        offset += (u32)colisize(e->field_0x0) + extra;
        e = (CColiEntry*)(colistep(e->field_0x0) + (u32)e + 16);
    }
    return 0;
}

// Walk the work object's spec table and hand every entry whose +0x2 flags
// carry bit 4 to the object's method table: the entry type selects the
// dispatch class (ColiSpecDispatchClass) and the running data offset is the
// dispatch key. Returns 1 as soon as a handler reports a hit.
extern "C" int ColiProc_DispatchSpec(CColiWork* work,
                                     CColiObjDispatch* obj) {
    obj->field_0x210 = 0;
    obj->field_0x2d4 = 0;
    CColiTable* table = colitable(work);
    CColiEntry* e = (CColiEntry*)((u8*)table + table->field_0x8);
    ColiRegisterSubObjects(obj, (CColiSpecHead*)table);
    u32 offset = 0;
    for (u32 i = 0; i < table->field_0xC; i++) {
        u32 extra = 0;
        if (e->field_0x0 != 8) {
            if (!(e->field_0x2 & 0x10)) {
                if (e->field_0x2 & 2) {
                    extra = (u32)colisize(e->field_0x0) + 0xc0;
                }
                offset += (u32)colisize(e->field_0x0) + extra;
                e = (CColiEntry*)(colistep(e->field_0x0) + (u32)e + 16);
                continue;
            }
            ColiSetDispatchIndex(obj, (u32)coliclass(e->field_0x0));
            if (e->field_0x2 & 2) {
                extra = (u32)colisize(e->field_0x0) + 0xc0;
            }
            if (ColiDispatchByIndex((CColiDispatchTarget*)obj,
                                    work->field_0x8 + offset) != 0) {
                return 1;
            }
        }
        offset += (u32)colisize(e->field_0x0) + extra;
        e = (CColiEntry*)(colistep(e->field_0x0) + (u32)e + 16);
    }
    return 0;
}

// CColiProc's ctor lives in its own unit (libs/monolib/src/coli/CColiProcBase.cpp,
// retail .text 0x804B66D4-0x804B66EC) so this unit's generated static
// initializer keeps calling it out of line, as retail does.

// ---------------------------------------------------------------------------
// CColiResCall's virtual slot 0 (retail ColiMoverUpdate, the first function of
// the unit that follows this one in retail .text): move the tracked position
// through the two bone matrices, fold the delta into the accumulated offset,
// then add the linear offset component. Both stages notify the target object.
// ---------------------------------------------------------------------------
extern "C" void ColiTranslatePoints(void* self);
struct CColiMtxSrc {
    Mtx mtx0;       //0x0
    char pad[0x60]; //0x30
    Mtx mtx1;       //0x90
};

// CColiResCall's virtual slot 0 (retail ColiMoverUpdate): the object itself,
// reached through the CColiProc members pos (+0x0C) / off (+0x24) and the
// CColiResCall members acc (+0x38) / src (+0x44) / target (+0x48).
void CColiResCall::Update() {
    CColiResCall* self = this;
    // Null-check the matrix source first; the vertical-offset gate only
    // evaluates when a source exists (short-circuit, matching retail's
    // early beq to the shared tail).
    if (self->src != NULL) {
        // Retail compares against the shared 0.0f pool entry (@sda21 reloc to
        // lbl_eu_8066AEA8), not a TU-local literal.
        if (self->off.y > lbl_eu_8066AEA8) {
            Vec tmp;
            Vec* pos = (Vec*)&self->pos;
            PSMTXMultVec((const f32 (*)[4])self->src->mtx1, pos, (Vec*)&tmp);
            PSMTXMultVec((const f32 (*)[4])self->src->mtx0, (Vec*)&tmp,
                         (Vec*)&tmp);
            nw4r::math::VEC3Sub((nw4r::math::VEC3*)&tmp,
                                (nw4r::math::VEC3*)&tmp,
                                (nw4r::math::VEC3*)&self->pos);
            nw4r::math::VEC3Add((nw4r::math::VEC3*)&self->acc,
                                (nw4r::math::VEC3*)&self->acc,
                                (nw4r::math::VEC3*)&tmp);
            ColiTranslatePoints(self->target);
        }
    }

    // Tail: acc += off, then notify the target object.
    nw4r::math::VEC3Add((nw4r::math::VEC3*)&self->acc,
                        (nw4r::math::VEC3*)&self->acc,
                        (nw4r::math::VEC3*)&self->off);
    ColiTranslatePoints(self->target)
;}
