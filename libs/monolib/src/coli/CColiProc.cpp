// Translation unit: monolib/src/coli/CColiProc
// Contains CColiProc constructor and field-management helpers.

#include <types.h>
#include <nw4r/math/math_types.h>
#include <cstring>
#include "monolib/coli/CColiProc.hpp"
#include "monolib/util/MemManager.hpp"

// CColiProcTarget/CColiProc definitions live in CColiProc.hpp (single source).

// Context/link struct passed to ColiProc_ProcessSegments; only 0x3C is referenced here.
struct CColiLinkObj {
    u8 field_0x0[0x3C];
    CColiProc* field_0x3C;
};

// The single global CColiResCall instance (retail bss, size 0x50) and the
// CColiResCall vtable installed into it at static-init time (both owned by
// code_804A6C60.cpp; see the class hierarchy note there).
extern "C" u8 lbl_eu_8065D0E8[0x50];
extern "C" u32 __vt__12CColiResCall[];
extern "C" const f32 lbl_eu_8066AEA8;
extern "C" void ColiProc_ProcessSegments();
extern "C" void __ct__CColiProc(CColiProc* self);

void CColiProc::InitTriple(CColiProcTarget* target, u32 a) {
    this->field_0x0 = target;
    this->field_0x4 = a;
    this->field_0xC = 0;
}

void CColiProc::InitQuad(CColiProcTarget* target, u32 a, u32 b) {
    this->field_0x0 = target;
    this->field_0x4 = a;
    this->field_0x8 = b;
    this->field_0xC = 0;
}

// Returns true if field_0x0 is non-null and bit 0 of its u16 at offset 6 is set.
// Both failure paths share a single return-0 tail to match retail branch layout.
bool CColiProc::IsTargetFlagged() {
    if (this->field_0x0 != nullptr && (this->field_0x0->field_0x6 & 1)) {
        return true;
    }
    return false;
}

// Returns bit 18 of field_0xC (extrwi extraction).
bool CColiProc::TestBit18State() {
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

// Variable-length entry chained via lbl_eu_8056F3C0[type]+16; data follows the
// 0x10-byte header (handler receives a pointer to field_0x10).
struct CColiEntry {
    u16 field_0x0; // type index into the exported tables
    u16 field_0x2; // flags: bit0 / bit1 / bit4
    u32 field_0x4; // absolute-ish data offset (added to the table base)
    u8 field_0x8[0x8];
    u8 field_0x10; // raw data blob
};

// Per-type metadata and handler vectors (retail .data tables).
// lbl_eu_8056F3C0      : s16 per-entry step size (+16 byte header), index by type
// lbl_eu_8056F3D4      : s16 per-entry data size, index by type
// lbl_eu_8056F3FC      : handler function-pointer vector, index by type

extern s16 lbl_eu_8056F3C0[];
extern s16 lbl_eu_8056F3D4[];
typedef void (*CColiDataFn)(u8* dst, u32 src, u32 data, u8* raw);
extern CColiDataFn lbl_eu_8056F3FC[];

// The generic object the collision work is run against.
struct CColiWork {
    CColiTable* field_0x0; // target table
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xC; // flags (bit18 set below => 0x40000)
};

// Owner whose +0x7C holds the ALLOC_HANDLE used for the output buffer.
struct CColiAllocOwner {
    u8 field_0x0[0x7C];
    mtl::ALLOC_HANDLE field_0x7C;
};

// Shared bss globals (set/read by this routine and by siblings).
extern u32 lbl_eu_80665940;
extern u32 lbl_eu_8066594C;
extern s16 lbl_eu_80665950;
extern CColiAllocOwner* lbl_eu_80665958;

// Compute the packed buffer size for every entry, allocate it, then fill it
// by dispatching each entry's data through lbl_eu_8056F3FC[type]. Returns an
// index/count code: 0 when no selection was made yet, else 2 (0 selected) or 1.
extern "C" int ColiProc_ClassifyEntries(CColiWork* work, u16* outIndex, void** outBuf,
                             u32 arg4) {
    CColiTable* table = work->field_0x0;
    lbl_eu_80665940 = (u32)table;
    lbl_eu_8066594C = arg4;

    // Pass 1: accumulate the total byte size of all entries.
    s32 total = 0;
    CColiEntry* e = (CColiEntry*)((u8*)table + table->field_0x8);
    u32 entryCount = table->field_0xC;
    for (u32 i = 0; i < entryCount; i++) {
        u16 type = e->field_0x0;
        u16 flags = e->field_0x2;
        // Advance to the next entry first (chained by C0[type]+16).
        e = (CColiEntry*)((u8*)e + lbl_eu_8056F3C0[type] + 16);
        if ((flags & 2) && type != 8) {
            total += 2 * lbl_eu_8056F3D4[type] + 0xc0;
        } else {
            total += lbl_eu_8056F3D4[type];
        }
    }

    u8* buf = nullptr;
    if (total != 0) {
        buf = (u8*)mtl::MemManager::allocate_head(
            lbl_eu_80665958->field_0x7C, (u32)total, 0x20);
    }

    // Pass 2: dispatch each entry's handler and pack its data into buf.
    u32 cntBit0 = 0;
    u32 offset = 0;
    CColiEntry* q = (CColiEntry*)((u8*)work->field_0x0 + table->field_0x8);
    lbl_eu_80665950 = -1;
    for (u32 i = 0; i < table->field_0xC; i++) {
        if (q->field_0x2 & 1) {
            cntBit0++;
        }
        if (q->field_0x2 & 0x10) {
            work->field_0xC |= 0x40000;
        }
        if ((q->field_0x2 & 2) && q->field_0x0 != 8) {
            u8* dst = buf + offset;
            lbl_eu_8056F3FC[q->field_0x0](
                dst, (u32)work->field_0x0 + q->field_0x4, work->field_0x4,
                &q->field_0x10);
            memcpy(dst + (s32)lbl_eu_8056F3D4[q->field_0x0], dst,
                   (size_t)lbl_eu_8056F3D4[q->field_0x0]);
            offset += 2 * lbl_eu_8056F3D4[q->field_0x0] + 0xc0;
        } else {
            u8* dst = buf + offset;
            lbl_eu_8056F3FC[q->field_0x0](
                dst, (u32)work->field_0x0 + q->field_0x4, work->field_0x4,
                &q->field_0x10);
            offset += lbl_eu_8056F3D4[q->field_0x0];
        }
        q = (CColiEntry*)((u8*)q + lbl_eu_8056F3C0[q->field_0x0] + 16);
    }

    *outBuf = buf;
    if (lbl_eu_80665950 == -1) {
        return 0;
    }
    s32 sign = -(s32)cntBit0 | (s32)cntBit0;
    *outIndex = lbl_eu_80665950;
    return (sign >> 31) + 2;
}

extern "C" void ColiProc_RefreshAxes(){}
// Prepare the shared CColiProc instance against a work object, pass it to the
// generic position update, then (on success) add the instance's offet vector.
extern "C" u32 ColiProc_RunCollideScan(CColiProc* self, nw4r::math::VEC3* pVec,
                             CColiLinkObj* pLink, s32 flag) {
    CColiProc* pColi = (CColiProc*)lbl_eu_8065D0E8;
    f32 zero = lbl_eu_8066AEA8;

    pColi->field_0x44 = 0;
    pColi->field_0x38.x = zero;
    pColi->field_0x38.y = zero;
    pColi->field_0x38.z = zero;
    pColi->field_0x48 = pLink;

    if (pColi != 0) {
        pLink->field_0x3C = pColi;
    } else {
        pLink->field_0x3C = (CColiProc*)((u8*)pLink + 4);
    }

    if (flag != 0) {
        self->field_0xC |= 1;
    }

    typedef int (*F2AA4)(CColiProc*, CColiLinkObj*);
    if (((F2AA4)ColiProc_ProcessSegments)(self, pLink) != 0) {
        nw4r::math::VEC3* pG =
            &((CColiProc*)lbl_eu_8065D0E8)->field_0x38;
        nw4r::math::VEC3Add(pVec, pVec, pG);
        return 1;
    }
    return 0;
}
extern "C" void ColiProc_ProcessSegments(){}
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
extern s16 lbl_eu_8056F3E8[];

// Per-type spec record sizes: step (next-entry advance), payload size and
// dispatch class. Accessors keep the offset tables spelled once.
static inline s32 colistep(u32 type) { return lbl_eu_8056F3C0[type]; }
static inline s32 colisize(u32 type) { return lbl_eu_8056F3D4[type]; }
static inline s32 coliclass(u32 type) { return lbl_eu_8056F3E8[type]; }

// Sibling helpers (defined in the code_804A6C60 unit).
extern "C" void ColiRegisterSubObjects(void* unused, CColiSpecHead* spec);
extern "C" u32 ColiSetDispatchIndex(void* unused, u32 val);
extern "C" int ColiDispatchByIndex(CColiDispatchTarget* self, u32 val);

// Same spec walk as ColiProc_DispatchSpec, but the *matched* entries are the
// ones the walk passes over: an entry is skipped when its +0x2 bits 2-3
// intersect the work object's selection state (+0xC bit 0 against entry bit 2,
// state bit 2 against entry bit 3); every other entry goes to the method table.
extern "C" int ColiProc_CheckEntries(CColiWork* work, CColiObjDispatch* obj) {
    obj->field_0x210 = 0;
    obj->field_0x2d4 = 0;
    CColiTable* table = work->field_0x0;
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
// dispatch class (lbl_eu_8056F3E8) and the running data offset is the
// dispatch key. Returns 1 as soon as a handler reports a hit.
extern "C" int ColiProc_DispatchSpec(CColiWork* work,
                                     CColiObjDispatch* obj) {
    obj->field_0x210 = 0;
    obj->field_0x2d4 = 0;
    CColiTable* table = work->field_0x0;
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

// --- hard-symbol stubs (scaffold_hard_symbols) ---

// Static initializer for the global CColiProc instance: default-construct it,
// then install the alternate default target-data pointer into field_0x0.
void sinit_804B2FB4() {
    typedef void (*CtorFn)(CColiProc*);
    // Called through a cast so MWCC emits an out-of-line call to the retail
    // ctor symbol instead of inlining the same-TU body.
    ((CtorFn)__ct__CColiProc)((CColiProc*)lbl_eu_8065D0E8);
    ((CColiProc*)lbl_eu_8065D0E8)->field_0x0 = (CColiProcTarget*)__vt__12CColiResCall;
}

// Defined after sinit so the static-init call stays an out-of-line call
// rather than being inlined by MWCC.
extern "C" void __ct__CColiProc(CColiProc* self) {
    self->field_0x0 = (CColiProcTarget*)__vt__9CColiProc;
    self->field_0x4 = 0;
}
