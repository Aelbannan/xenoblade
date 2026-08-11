// Catalog TU for monolib/src/lod/CTaskLOD
// Replace stubs with high-level C/C++ during decomp.

#include <types.h>
#include "monolib/work/CTTask.hpp"

class CTaskLOD;

// Minimal LOD::LODMemMan declaration for this TU.  Only the member functions
// really called here are declared (with the exact signatures the retail
// callers use; several take a trailing argument passed via r4 that the
// read-only LODMemMan.hpp header omits).  Declared locally because the shared
// header is outside this session's writable scope.
namespace LOD {
class LODMemMan {
public:
    void func_8046E6DC();
    void func_8046E594(int enable);
    void func_8046E5BC(CTaskLOD* source);
    void func_8046F010();
    void func_8046F088(CTaskLOD* task);
    void func_8046E920(CTaskLOD* task);
    void func_8046EB50(CTaskLOD* task);
    void func_8046EBEC(CTaskLOD* task);
    void func_8046EEE8(CTaskLOD* task);
    void* func_8046E8C8(CTaskLOD* task);
    float func_8046EC88(CTaskLOD* task);
    void func_8046EAE8(CTaskLOD* task, void* a);
    void func_8046ECD4(CTaskLOD* task, void* a);
    void func_8046ED68(CTaskLOD* task, void* a);
    void func_8046EDD0(CTaskLOD* task, void* a);
    void func_8046E770(CTaskLOD* task, void* a, void* b);
    void* func_8046EE9C(CTaskLOD* task);
    float func_8046EF30(CTaskLOD* task);
    void* func_8046EF7C(CTaskLOD* task);
    void func_8046DBC8();
    float func_8046F01C();
    void func_8046F024(int a, int b);

private:
    u8 _00[0x6c]; // unknown leading fields

public:
    u32 field_0x6c; // offset 0x6c (flag bits used by func_80462C48)
};
} // namespace LOD

// CTaskLOD wraps a LODMemMan and manages its lifecycle.
// Inherits from CTTask<CTaskLOD> (0x54: CDoubleListNode + vtable + CProcess +
// Move/Draw member-pointer slots).
// Layout (0x1D44 total):
//   0x00-0x53 : CTTask<CTaskLOD> base
//   0x54 ..    : unknown state fields (not used by the matched delegates)
//   0x1D40    : mpActiveLOD (points to the LODMemMan to delegate to)
class CTaskLOD : public CTTask<CTaskLOD> {
public:
    virtual ~CTaskLOD();
    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void Draw();
    void create();
    void func_80462A08();
    void func_80462AC0();
    void func_80462B30();
    void func_80462B4C();
    void func_80462B68();
    void func_80462BC8();
    void func_80462BE4();
    void func_80462BFC();
    float func_80462C14();
    void func_80462C48();
    void func_80462C80();
    void func_80462CBC();
    void func_80462CD8(void* a, void* b, void* c);
    void func_80462D04();
    void func_80462D5C();
    void func_80462DB4();
    void* func_80462E1C();
    void func_80462E3C();
    void func_80462E58();
    void func_80462ED0(void* a, void* b);
    void func_80462EF4();
    void func_80462F10();
    float func_80462F2C();
    void func_80462F4C(void* a, void* b);
    void func_80462F70(void* a, void* b);
    void func_80462F94(void* a, void* b);
    void* func_80462FB8();
    void func_80462FD8();
    float func_80462FF4();
    void* func_80463014();

private:
    u8 _54[0x1D40 - 0x54];       // 0x54 .. 0x1D40

public:
    LOD::LODMemMan* mpActiveLOD; // 0x1D40
};

// Global instance pointer referenced by the delegate wrappers.  Declared at
// global scope (MWCC does not mangle global data names) - no `extern "C"`
// needed.
extern CTaskLOD* lbl_eu_80665730;

// --- FULL_MATCH functions ---

// Delegate wrappers.  Each checks the singleton instance pointer
// `lbl_eu_80665730` and, if non-null, forwards to its active LODMemMan
// (mpActiveLOD at 0x1D40).  The forwarding call is tail-call optimised by
// MWCC into a `b` branch.
//
// func_8046E6DC / func_8046F010 are genuinely no-arg (`.Fv`), matching
// byte-for-byte.  func_8046E594 and func_8046E5BC take a real argument in
// retail (passed via r4: `1`/`0`/`this`), but the shared symbol map labels
// them `.Fv` (no-arg); MWCC refuses to pass an arg to a no-arg-declared
// method, so these bodies are byte-identical with a branch-reloc NAME drift
// (`.Fi`/`.FP8CTaskLOD` vs retail `.Fv`).

void CTaskLOD::func_80462BE4() {
    if (lbl_eu_80665730) {
        lbl_eu_80665730->mpActiveLOD->func_8046E6DC();
    }
}

void CTaskLOD::func_80462BFC() {
    if (lbl_eu_80665730) {
        lbl_eu_80665730->mpActiveLOD->func_8046F010();
    }
}

void CTaskLOD::func_80462B30() {
    if (lbl_eu_80665730) {
        lbl_eu_80665730->mpActiveLOD->func_8046E594(1);
    }
}

void CTaskLOD::func_80462B4C() {
    if (lbl_eu_80665730) {
        lbl_eu_80665730->mpActiveLOD->func_8046E594(0);
    }
}

void CTaskLOD::func_80462BC8() {
    if (lbl_eu_80665730) {
        lbl_eu_80665730->mpActiveLOD->func_8046E5BC(this);
    }
}

// --- Remaining harness stubs (empty bodies) ---

CTaskLOD::~CTaskLOD() {}

void CTaskLOD::Init() {}
void CTaskLOD::Term() {}

void CTaskLOD::Move() { mpActiveLOD->func_8046DBC8(); }

void CTaskLOD::Draw() {}
void CTaskLOD::create() {}

void CTaskLOD::func_80462A08() {}
void CTaskLOD::func_80462AC0() {}
void CTaskLOD::func_80462B68() {}

// Calls the active LODMemMan's getter for its side-effect-free float access,
// then unconditionally returns the constant 1.0f (retail loads the 1.0f pool
// entry at 0x8066A5B8 after the call).
float CTaskLOD::func_80462C14() {
    if (lbl_eu_80665730) {
        lbl_eu_80665730->mpActiveLOD->func_8046F01C();
    }
    return 1.0f;
}

// Toggle the 0x40 flag bit on the active LODMemMan at offset 0x6c: set when
// `this` is non-null, clear when null.
void CTaskLOD::func_80462C48() {
    if (lbl_eu_80665730) {
        LOD::LODMemMan* lod = lbl_eu_80665730->mpActiveLOD;
        if (this) {
            lod->field_0x6c |= 0x40;
        } else {
            lod->field_0x6c &= ~0x40;
        }
    }
}

// Forwards the tail call to the active LODMemMan's func_8046F024 with args
// (1, this - 1) when `this` is non-null, otherwise (0, 0).
void CTaskLOD::func_80462C80() {
    if (lbl_eu_80665730) {
        if (this) {
            lbl_eu_80665730->mpActiveLOD->func_8046F024(1, (int)this - 1);
        } else {
            lbl_eu_80665730->mpActiveLOD->func_8046F024(0, 0);
        }
    }
}

void CTaskLOD::func_80462CBC() {
    if (lbl_eu_80665730) {
        lbl_eu_80665730->mpActiveLOD->func_8046F088(this);
    }
}

// Fetches the active LODMemMan and forwards task + first two args (a, b) to it
// (retail `mr r6, r5` sets up b in r6 for the delegate).  c is unused (dropped).
// The retail symbol map labels this `.Fv`, but the real retail body reads r4/r5/r6
// (see shared header).
void CTaskLOD::func_80462CD8(void* a, void* b, void* c) {
    if (lbl_eu_80665730) {
        lbl_eu_80665730->mpActiveLOD->func_8046E770(this, a, b);
    }
}
void CTaskLOD::func_80462D04() {}
void CTaskLOD::func_80462D5C() {}
void CTaskLOD::func_80462DB4() {}
// Value-delegates.  Like the void wrappers above, each forwards to the
// active LODMemMan when the singleton `lbl_eu_80665730` is set, passing
// `this` as the trailing CTaskLOD* argument (retail `mr r4, r3`).  The
// pointer/float returns come straight back from the delegate.
void* CTaskLOD::func_80462E1C() {
    if (lbl_eu_80665730) {
        return lbl_eu_80665730->mpActiveLOD->func_8046E8C8(this);
    }
    return 0;
}

void CTaskLOD::func_80462E3C() {
    if (lbl_eu_80665730) {
        lbl_eu_80665730->mpActiveLOD->func_8046E920(this);
    }
}

void CTaskLOD::func_80462E58() {}

// Same delegate-shape as func_80462CD8: forwards `this` + first arg `a` to the
// active LODMemMan (tail-call `b`).  `b` is unused in retail.
void CTaskLOD::func_80462ED0(void* a, void* b) {
    if (lbl_eu_80665730) {
        lbl_eu_80665730->mpActiveLOD->func_8046EAE8(this, a);
    }
}

void CTaskLOD::func_80462EF4() {
    if (lbl_eu_80665730) {
        lbl_eu_80665730->mpActiveLOD->func_8046EB50(this);
    }
}

void CTaskLOD::func_80462F10() {
    if (lbl_eu_80665730) {
        lbl_eu_80665730->mpActiveLOD->func_8046EBEC(this);
    }
}

float CTaskLOD::func_80462F2C() {
    if (lbl_eu_80665730) {
        return lbl_eu_80665730->mpActiveLOD->func_8046EC88(this);
    }
    return 0.0f;
}
void CTaskLOD::func_80462F4C(void* a, void* b) {
    if (lbl_eu_80665730) {
        lbl_eu_80665730->mpActiveLOD->func_8046ECD4(this, a);
    }
}

void CTaskLOD::func_80462F70(void* a, void* b) {
    if (lbl_eu_80665730) {
        lbl_eu_80665730->mpActiveLOD->func_8046ED68(this, a);
    }
}

void CTaskLOD::func_80462F94(void* a, void* b) {
    if (lbl_eu_80665730) {
        lbl_eu_80665730->mpActiveLOD->func_8046EDD0(this, a);
    }
}
void* CTaskLOD::func_80462FB8() {
    if (lbl_eu_80665730) {
        return lbl_eu_80665730->mpActiveLOD->func_8046EE9C(this);
    }
    return 0;
}

void CTaskLOD::func_80462FD8() {
    if (lbl_eu_80665730) {
        lbl_eu_80665730->mpActiveLOD->func_8046EEE8(this);
    }
}

float CTaskLOD::func_80462FF4() {
    if (lbl_eu_80665730) {
        return lbl_eu_80665730->mpActiveLOD->func_8046EF30(this);
    }
    return 0.0f;
}

// func_8046EF7C returns a pointer; when there is no active LOD the retail
// body returns `this` unchanged (beqlr with r3 still holding `this`).
void* CTaskLOD::func_80463014() {
    if (lbl_eu_80665730) {
        return lbl_eu_80665730->mpActiveLOD->func_8046EF7C(this);
    }
    return this;
}

// Out-of-line specialisations of the CTTask<CTaskLOD> Move/Draw slots.  These
// emit the retail `Move__17CTTask<8CTaskLOD>Fv` / `Draw__17CTTask<8CTaskLOD>Fv`
// bodies: __ptmf_test on the member-ptr slot (0x3C / 0x48) then __ptmf_scall.
template<>
void CTTask<CTaskLOD>::Move() {
    if (mMoveFunc) {
        (static_cast<CTaskLOD*>(this)->*mMoveFunc)();
    }
}

template<>
void CTTask<CTaskLOD>::Draw() {
    if (mDrawFunc) {
        (static_cast<CTaskLOD*>(this)->*mDrawFunc)();
    }
}

// Out-of-line dtor: emits the retail `__dt__17CTTask<8CTaskLOD>Fv` body
// (base CProcess dtor call + conditional operator delete).
template<>
CTTask<CTaskLOD>::~CTTask() {}