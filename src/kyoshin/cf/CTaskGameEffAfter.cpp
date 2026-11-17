#include "kyoshin/cf/CTaskGameEffAfter.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"

// The optimize_for_size pragma stays OPEN for the whole TU: the factory (with the
// inlined ctor) and both dtors need the size-priority frame (retail stmw r30 prologues),
// and the Move/Draw dispatchers + empty overrides compile identically under
// either priority. Definition order follows retail .text order.
#pragma optimize_for_size on

CTaskGameEffAfter* CTaskGameEffAfter::create(CProcess* parent) {
    CTaskGameEffAfter* obj = new (CWorkThreadSystem::getWorkMem()) CTaskGameEffAfter();
    obj->Regist(parent, false);
    return obj;
}


// Base destructor: destruct CProcess and free the block if the delete flag is set.
// Retail __dt__27CTTask<17CTTaskGameEffAfter>Fv is 0x50 (stmw r30 frame).
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
