// TU: kyoshin/cf/CTaskGameEffAfter
//
// CTaskGameEffAfter derives from CTTask<CTaskGameEffAfter>, the CRTP task base.
// It stores two pointer-to-member-function callbacks (Move at +0x3C, Draw at
// +0x48) that the base CTTask::Move/Draw dispatch through __ptmf_test/__ptmf_scall.
// The object is not built through a normal member ctor: the retail ctor is a
// self-allocating factory that raw-initialises the memory and registers the new
// task under a parent.

#include "kyoshin/cf/CTaskGameEffAfter.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/util/MemManager.hpp"

// ============================================================================
// CTTask<CTaskGameEffAfter> template specializations (out-of-line).
// ============================================================================

// Dispatch the move callback if it is non-null.
template<>
void CTTask<CTaskGameEffAfter>::Move() {
    if (__ptmf_test(&mMoveFunc)) {
        (static_cast<CTaskGameEffAfter*>(this)->*mMoveFunc)();
    }
}

// Dispatch the draw callback if it is non-null.
template<>
void CTTask<CTaskGameEffAfter>::Draw() {
    if (__ptmf_test(&mDrawFunc)) {
        (static_cast<CTaskGameEffAfter*>(this)->*mDrawFunc)();
    }
}

// Base destructor: destruct CProcess and free the block if the delete flag is set.
template<>
CTTask<CTaskGameEffAfter>::~CTTask() {}

// ============================================================================
// CTaskGameEffAfter methods.
// ============================================================================

void CTaskGameEffAfter::Draw() {}
void CTaskGameEffAfter::Term() {}
void CTaskGameEffAfter::Init() {}

CTaskGameEffAfter::~CTaskGameEffAfter() {}

// ============================================================================
// Constructor (self-allocating factory). Retail symbol: __ct__CTaskGameEffAfter.
//   r3 = parent CProcess* (used for Regist)
// Allocates a 0x54 block, constructs the CProcess base in place, fills the
// CTTask move/draw callback slots with __ptmf_null, registers the new task
// under `parent`, and returns it.
// ============================================================================
void* __ct__CTaskGameEffAfter(CProcess* parent) {
    CTaskGameEffAfter* mem = static_cast<CTaskGameEffAfter*>(
        mtl::MemManager::allocate(sizeof(CTaskGameEffAfter),
                                  CWorkThreadSystem::getWorkMem()));
    CTaskGameEffAfter* obj = static_cast<CTaskGameEffAfter*>(mem);
    if (obj) {
        __ct__8CProcessFv(obj);

        // In-place construction of CTTask<CTaskGameEffAfter>, using u32 word
        // indexing: reassigning w0/w1 from __ptmf_null between the two callback
        // slots forces MWCC to reload each null PTMF word (retail issues six
        // separate loads, not CSE'd).
        u32* p = reinterpret_cast<u32*>(obj);
        const u32* nullPt = &__ptmf_null[0];
        p[4] = reinterpret_cast<u32>(lbl_eu_80526650);   // interim CTTask vtable
        u32 ptmf1 = nullPt[1];
        u32 ptmf0 = nullPt[0];
        p[0xF] = ptmf0;       // 0x3C mMoveFunc[0]
        p[0x10] = ptmf1;      // 0x40 mMoveFunc[1]
        p[0x11] = nullPt[2];  // 0x44 mMoveFunc[2]
        ptmf1 = nullPt[1];
        ptmf0 = nullPt[0];
        p[0x12] = ptmf0;      // 0x48 mDrawFunc[0]
        p[0x13] = ptmf1;      // 0x4C mDrawFunc[1]
        p[0x14] = nullPt[2];  // 0x50 mDrawFunc[2]
        p[4] = reinterpret_cast<u32>(lbl_eu_80526608);   // CTaskGameEffAfter vtable
    }
    // Regist runs even when allocation failed (retail behaviour).
    obj->Regist(parent, false);
    return obj;
}