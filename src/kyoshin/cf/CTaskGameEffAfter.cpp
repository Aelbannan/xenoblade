// TU: kyoshin/cf/CTaskGameEffAfter
//
// CTaskGameEffAfter derives from CTTask<CTaskGameEffAfter>, the CRTP task base.
// It stores two pointer-to-member-function callbacks (Move at +0x3C, Draw at
// +0x48) that the base CTTask::Move/Draw dispatch through __ptmf_test/__ptmf_scall.
// The object is not built through a normal member ctor: the retail ctor is a
// self-allocating factory that raw-initialises the memory, registers the new
// task under a parent, and returns the fresh object.
//
// The factory is a C++-linkage free function, so MWCC emits it under the
// mangled name __ct__CTaskGameEffAfter__FP8CProcess; the retail split carries
// the same annotation (the earlier unmangled __ct__CTaskGameEffAfter form was
// a recovery placeholder). The retail split object carries NO data: the two
// vtables (lbl_eu_80526650 interim CTTask base, lbl_eu_80526608 final) and the
// RTTI descriptors ship from the shared retail data object.

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
// Retail __dt__27CTTask<17CTTaskGameEffAfter>Fv is 0x50 (stmw r30 frame). The
// optimize_for_size pragma stays OPEN for the rest of the TU on purpose: the
// derived dtor and the self-allocating ctor need the same size-priority frame
// (retail stmw r30 prologues), and the Move/Draw dispatchers + empty overrides
// compile identically under either priority. (An unclosed pragma leaks forward
// in MWCC - see docs/MWCC_CASES.md "pragma leaks".)
#pragma optimize_for_size on
template<>
CTTask<CTaskGameEffAfter>::~CTTask() {}

// ============================================================================
// CTaskGameEffAfter methods.
// ============================================================================

void CTaskGameEffAfter::Draw() {}
void CTaskGameEffAfter::Term() {}
void CTaskGameEffAfter::Init() {}

// Empty derived dtor: the compiler inlines the (empty) CTTask base dtor, which
// itself null-checks `this` again - reproducing retail's dead second beq.
CTaskGameEffAfter::~CTaskGameEffAfter() {}

// ============================================================================
// Constructor (self-allocating factory). Retail symbol:
// __ct__CTaskGameEffAfter__FP8CProcess (MWCC mangling of the free function;
// the retail split annotation matches).
//   r3 = parent CProcess* (used for Regist)
// Allocates a 0x54 block, constructs the CProcess base in place, fills the
// CTTask move/draw callback slots with __ptmf_null, registers the new task
// under `parent`, and returns it.
// ============================================================================
CTaskGameEffAfter* __ct__CTaskGameEffAfter(CProcess* parent) {
    CTaskGameEffAfter* mem = static_cast<CTaskGameEffAfter*>(
        mtl::MemManager::allocate(sizeof(CTaskGameEffAfter),
                                  CWorkThreadSystem::getWorkMem()));
    CTaskGameEffAfter* obj = mem;
    if (obj) {
        __ct__8CProcessFv(obj);

        // In-place construction of CTTask<CTaskGameEffAfter>, using u32 word
        // indexing: reassigning w0/w1 from __ptmf_null between the two callback
        // slots forces MWCC to reload each null PTMF word (retail issues six
        // separate loads, not CSE'd). Round-trip the base through a u32 value so
        // MWCC materializes it as a register value (lis/addi into ONE register)
        // and every load - including the offset-0 word - uses that base instead
        // of folding @l into the lwz.
        u32* p = reinterpret_cast<u32*>(obj);
        p[4] = reinterpret_cast<u32>(lbl_eu_80526650);   // interim CTTask vtable
        const volatile u32* ptmf = reinterpret_cast<const volatile u32*>(reinterpret_cast<u32>(__ptmf_null));
        u32 ptmf1 = ptmf[1];
        u32 ptmf0 = ptmf[0];
        p[0xF] = ptmf0;       // 0x3C mMoveFunc[0]
        p[0x10] = ptmf1;      // 0x40 mMoveFunc[1]
        p[0x11] = ptmf[2];    // 0x44 mMoveFunc[2]
        ptmf1 = ptmf[1];
        ptmf0 = ptmf[0];
        p[0x12] = ptmf0;      // 0x48 mDrawFunc[0]
        p[0x13] = ptmf1;      // 0x4C mDrawFunc[1]
        p[0x14] = ptmf[2];    // 0x50 mDrawFunc[2]
        p[4] = reinterpret_cast<u32>(lbl_eu_80526608);   // CTaskGameEffAfter vtable
    }
    // Regist runs even when allocation failed (retail behaviour).
    obj->Regist(parent, false);
    return obj;
}
