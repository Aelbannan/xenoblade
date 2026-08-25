// monolib/src/lod/CTaskLOD -- COMPLETE (Matching TU).
// 38/38 functions FULL byte-match; data gate MATCH; split budget exact.
// Retail data (vtables/RTTI/strings/singleton) is defined at the bottom of
// this file. See docs/MWCC_CASES.md "CTaskLOD" for the matching notes.

#include <types.h>
#include "monolib/work/CTTask.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/util/MemManager.hpp"

class CTaskLOD;

// Minimal LOD::LODMemMan declaration for this TU.  Only the member functions
// really called here are declared (with the exact signatures the retail
// callers use; several take a trailing argument passed via r4 that the
// read-only LODMemMan.hpp header omits).  Declared locally because the shared
// header is outside this session's writable scope.
namespace LOD {
class LODMemMan {
public:
    LODMemMan();
    void func_8046E6DC();
    void func_8046E594(int enable);
    void func_8046E5BC(CTaskLOD* source);
    void func_8046F010(float a);
    void func_8046F088(CTaskLOD* task);
    void func_8046E920(CTaskLOD* task);
    void func_8046EB50(CTaskLOD* task);
    void func_8046EBEC(CTaskLOD* task);
    void func_8046EEE8(CTaskLOD* task);
    void* func_8046E8C8(CTaskLOD* task);
    float func_8046EC88(CTaskLOD* task);
    void* func_8046EE9C(CTaskLOD* task);
    float func_8046EF30(CTaskLOD* task);
    void* func_8046EF7C(CTaskLOD* task);
    void func_8046DA64();
    void func_8046DBC8();
    float func_8046F01C();
    void func_8046F024(int a, int b);

private:
    u8 _00[0x6c]; // unknown leading fields

public:
    u32 field_0x6c; // offset 0x6c (flag bits used by func_80462C48)
};
} // namespace LOD

struct CScnEnvLgtCtrl;  // scene-light control ring (CScnEnvLgtCtrl.hpp)

// Pointee of CTaskLOD::mParam1 (offset 0x54): an opaque manager object whose
// +0x7C slot holds the scene-light control list head handed to func_804C2014
// by the func_80462D04/D5C/DB4 wrappers.
struct LODParam1Obj {
    u8 _00[0x7C];
    CScnEnvLgtCtrl* field_0x7C;  // +0x7C light-control list head
};

// CTaskLOD wraps a LODMemMan and manages its lifecycle.
// Inherits from CTTask<CTaskLOD> (0x54: CDoubleListNode + vtable + CProcess +
// Move/Draw member-pointer slots).
// Layout (0x1D44 total):
//   0x00-0x53 : CTTask<CTaskLOD> base
//   0x54 ..    : unknown state fields (not used by the matched delegates)
//   0x1D40    : mpActiveLOD (points to the LODMemMan to delegate to)
//
// novtable: retail emits NO implicit vptr stores in the dtor (and the ctor
// chain is inlined into create with the vptr written by explicit stores), so
// the compiler-generated vptr init must be suppressed.
class __declspec(novtable) CTaskLOD : public CTTask<CTaskLOD> {
public:
    virtual ~CTaskLOD();
    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void Draw();
    void func_80462AC0();
    void func_80462B30();
    // Inlined into create(): nulls the CTTask<CTaskLOD> Move/Draw member
    // pointers (emits the retail __ptmf_null loads at the call site).
    void initMemberPointers() {
        mMoveFunc = nullptr;
        mDrawFunc = nullptr;
    }
    void func_80462B4C();
    void func_80462B68();
    void func_80462BC8();
    void func_80462BE4();
    void func_80462BFC();
    float func_80462C14();
    void func_80462C48();
    void func_80462C80();
    void func_80462CBC();
    void func_80462D04();
    void func_80462D5C();
    void* func_80462E1C();
    void func_80462E3C();
    void func_80462EF4();
    void func_80462F10();
    float func_80462F2C();
    void* func_80462FB8();
    void func_80462FD8();
    float func_80462FF4();
    void* func_80463014();

    LODParam1Obj* mParam1;              // 0x54
    void* mParam2;                      // 0x58
    LOD::LODMemMan mLODMemMan;          // 0x5C (local class size 0x70)
    u8 _5C_pad[0x1C70];                 // 0xCC .. 0x1D3C
    LOD::LODMemMan* mpSecondaryLOD;     // 0x1D3C
    LOD::LODMemMan* mpActiveLOD;        // 0x1D40
};

// Global instance pointer referenced by the delegate wrappers.  Retail owns a
// single 8-byte, 8-aligned .sbss slot here (the pointer plus a 4-byte
// anonymous tail); define it as a 2-element array so the object reproduces
// the retail size and alignment.  All accesses go through element 0 (MWCC
// emits byte-identical code for arr[0] and the scalar form).
// (plain definition: global data names are never mangled by MWCC, and an
// `extern "C"` qualified declaration would be taken as extern storage class,
// emitting no storage)
__attribute__((aligned(8))) CTaskLOD* lbl_eu_80665730[2];

// Retail vtable data (defined at the bottom of this file; declared here so
// create()'s vptr stores carry the exact retail reloc names).
extern "C" u32 lbl_eu_8056D6C0[9];
extern "C" u32 lbl_eu_8056D678[9];

// LODMemMan delegate entry points used by the wrappers below.  The retail
// symbol map labels them `.Fv` (no-arg) even though the retail bodies take
// real arguments (r4/r5/...).  A C++ declaration cannot express that shape
// (any arg list mangles the name away from the retail symbol), so they are
// declared with C linkage: MWCC emits the identifier verbatim, making both
// the wrapper symbols and the tail-call relocs carry the exact retail names
// (no reloc drift).  Same pattern as the accepted LODMemMan.cpp TU.
extern "C" void func_8046EAE8__Q23LOD9LODMemManFv(LOD::LODMemMan* self, CTaskLOD* task, void* flag);
extern "C" void func_8046ECD4__Q23LOD9LODMemManFv(LOD::LODMemMan* self, CTaskLOD* task, void* flag);
extern "C" void func_8046ED68__Q23LOD9LODMemManFv(LOD::LODMemMan* self, CTaskLOD* task, void* flag);
extern "C" void func_8046EDD0__Q23LOD9LODMemManFv(LOD::LODMemMan* self, CTaskLOD* task, void* value);
extern "C" void func_8046E770__Q23LOD9LODMemManFv(LOD::LODMemMan* self, CTaskLOD* task, void* a, void* b);
extern "C" void func_8046E780__Q23LOD9LODMemManFv(LOD::LODMemMan* self, CTaskLOD* task);
extern "C" void func_8046E7D0__Q23LOD9LODMemManFv(LOD::LODMemMan* self, CTaskLOD* task);
extern "C" void func_8046E820__Q23LOD9LODMemManFv(LOD::LODMemMan* self, CTaskLOD* task, int flag);
extern "C" void func_8046E594__Q23LOD9LODMemManFv(LOD::LODMemMan* self, int param);
extern "C" void func_8046E5BC__Q23LOD9LODMemManFv(LOD::LODMemMan* self, CTaskLOD* task);
extern "C" void* func_8046E8C8__Q23LOD9LODMemManFv(LOD::LODMemMan* self, CTaskLOD* task);
extern "C" void func_8046F024__Q23LOD9LODMemManFv(LOD::LODMemMan* self, int a, int b);
extern "C" void func_8046F088__Q23LOD9LODMemManFv(LOD::LODMemMan* self, CTaskLOD* task);
extern "C" void func_8046E920__Q23LOD9LODMemManFv(LOD::LODMemMan* self, CTaskLOD* task);
extern "C" void func_8046EB50__Q23LOD9LODMemManFv(LOD::LODMemMan* self, CTaskLOD* task);
extern "C" void func_8046EBEC__Q23LOD9LODMemManFv(LOD::LODMemMan* self, CTaskLOD* task);
extern "C" float func_8046EC88__Q23LOD9LODMemManFv(LOD::LODMemMan* self, CTaskLOD* task);
extern "C" void* func_8046EE9C__Q23LOD9LODMemManFv(LOD::LODMemMan* self, CTaskLOD* task);
extern "C" void func_8046EEE8__Q23LOD9LODMemManFv(LOD::LODMemMan* self, CTaskLOD* task);
extern "C" float func_8046EF30__Q23LOD9LODMemManFv(LOD::LODMemMan* self, CTaskLOD* task);
extern "C" void* func_8046EF7C__Q23LOD9LODMemManFv(LOD::LODMemMan* self, CTaskLOD* task);
extern "C" void func_804717FC__Q23LOD9LODMemManFv();
extern "C" void __dt__8046D144(void* self, int flag);
extern "C" void __ct__Q23LOD9LODMemManFv(void* self);
extern "C" void __ct__8CProcessFv(CProcess* self);
extern "C" void func_804C2014(CScnEnvLgtCtrl* self, void* a, int b);
extern "C" void func_8046DAC0__Q23LOD9LODMemManFv(LOD::LODMemMan* self, int param);
extern "C" void func_8046E988__Q23LOD9LODMemManFv(LOD::LODMemMan* self, CTaskLOD* task, void* a);
extern "C" void* func_8046D898__Q23LOD9LODMemManFv(LOD::LODMemMan* self, CTaskLOD* task, LODParam1Obj* p1);
extern "C" void func_80471794__Q23LOD9LODMemManFv(u32 handle, u32 size);
extern "C" void func_8046CFD8__Q23LOD17UnkClass_8046A530Fv(LOD::LODMemMan* self, u32 a, u32 b);
extern "C" void func_804C2094(CScnEnvLgtCtrl* self, float f, CTaskLOD* task, void* a);

// Overlay for the compiler-placed vptr slot at +0x10 (the CDoubleListNode
// base occupies 0x00-0x10).  create() installs the retail vtable pointers
// through this typed view instead of raw pointer arithmetic.
struct CTaskLODVptrSlot {
    u8 _00[0x10];
    void* vtable;  // +0x10
};

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
    if (lbl_eu_80665730[0]) {
        lbl_eu_80665730[0]->mpActiveLOD->func_8046E6DC();
    }
}

// The uninitialized `float d` out-arg is deliberate: retail discards the
// delegate's float return the same way, and MWCC warning 10185 on this line
// is expected -- do not "fix" it (any change here breaks the byte match).
void CTaskLOD::func_80462BFC() {
    if (lbl_eu_80665730[0]) {
        float d; lbl_eu_80665730[0]->mpActiveLOD->func_8046F010(d);
    }
}

void CTaskLOD::func_80462B30() {
    if (lbl_eu_80665730[0]) {
        // Retail symbol is .Fv though r4 carries the arg; route through the
        // extern "C" retail-named wrapper so the reloc name matches.
        func_8046E594__Q23LOD9LODMemManFv(lbl_eu_80665730[0]->mpActiveLOD, 1);
    }
}

void CTaskLOD::func_80462B4C() {
    if (lbl_eu_80665730[0]) {
        func_8046E594__Q23LOD9LODMemManFv(lbl_eu_80665730[0]->mpActiveLOD, 0);
    }
}

void CTaskLOD::func_80462BC8() {
    if (lbl_eu_80665730[0]) {
        func_8046E5BC__Q23LOD9LODMemManFv(lbl_eu_80665730[0]->mpActiveLOD, this);
    }
}

// --- Remaining harness stubs (empty bodies) ---

// Destroy the embedded LODMemMan member (retail calls its address-named dtor
// __dt__8046D144 with the member-dtor flag -1); the base CProcess dtor call
// (__dt__8CProcessFv) and the deleting-flag operator delete are emitted
// automatically by MWCC.
CTaskLOD::~CTaskLOD() {
    __dt__8046D144(&mLODMemMan, -1);
}

void CTaskLOD::Init() {}

// Teardown: release the secondary LOD instance (if any), then stop the active
// LODMemMan and release the shared persistent LOD buffer.  The redundant
// inner null-check reproduces retail's doubled beq (MWCC keeps the delete's
// own guard when the guarded store follows); the dtor is called explicitly
// because the retail reloc name (__dt__8046D144) cannot be produced by a C++
// class dtor.
void CTaskLOD::Term() {
    if (mpSecondaryLOD) {
        if (mpSecondaryLOD) {
            __dt__8046D144(mpSecondaryLOD, 1);
            mpSecondaryLOD = 0;
        }
    }
    mpActiveLOD->func_8046DA64();
    func_804717FC__Q23LOD9LODMemManFv();
}

void CTaskLOD::Move() { mpActiveLOD->func_8046DBC8(); }

// Draw is NOT defined here: retail's split has no Draw__8CTaskLODFv body
// (the vtable slot targets CTTask<CTaskLOD>::Draw) and nothing calls it, so
// defining the empty override would add a 4-byte function retail lacks.
// (declaration stays in the class; the virtual is never odr-used)

// Factory: allocate a 0x1D44 CTaskLOD from work memory and fully initialise
// it (the retail symbol is mislabeled `.Fv` but the body reads r4-r7).
extern "C" CTaskLOD* create__8CTaskLODFv(CTaskLOD* parent, LODParam1Obj* p1,
                                          void* p2, u32 handle, u32 size) {
    CTaskLOD* t = (CTaskLOD*)mtl::MemManager::allocate(
        0x1d44, CWorkThreadSystem::getWorkMem());
    if (t) {
        // CProcess base ctor (out-of-line, abstract class so it cannot be
        // placement-newed) plus the inlined CTTask<CTaskLOD> / CTaskLOD vptr
        // and Move/Draw member-pointer stores (the retail ctor-chain
        // sequence).  The vtable labels are referenced directly so the vptr
        // stores carry the retail reloc names.
        __ct__8CProcessFv(t);
        ((CTaskLODVptrSlot*)t)->vtable = (void*)&lbl_eu_8056D6C0;
        t->initMemberPointers();
        ((CTaskLODVptrSlot*)t)->vtable = (void*)&lbl_eu_8056D678;
        t->mParam1 = p1;
        t->mParam2 = p2;
        // Direct ctor-symbol call (same recipe as the __dt__8046D144 dtor
        // calls in this TU): placement-new here makes MWCC emit its intrinsic
        // null guard + addic. on &t->mLODMemMan, which retail does not have
        // (the compiler there proved the address non-null via the t != 0
        // check above).
        __ct__Q23LOD9LODMemManFv(&t->mLODMemMan);
        t->mpSecondaryLOD = 0;
        t->mpActiveLOD = &t->mLODMemMan;
        if (size) {
            func_80471794__Q23LOD9LODMemManFv(handle, size);
        }
        func_8046CFD8__Q23LOD17UnkClass_8046A530Fv(t->mpActiveLOD, handle, size);
    }
    t->Regist(parent, false);
    lbl_eu_80665730[0] = t;
    return t;
}

// Allocate/replace the secondary LODMemMan when `enable` is set, then return
// the active LODMemMan's func_8046D898 result (0 when there is no singleton).
extern "C" void* func_80462A08__8CTaskLODFv(CTaskLOD* self, bool enable) {
    if (lbl_eu_80665730[0]) {
        if (enable) {
            CTaskLOD* t = lbl_eu_80665730[0];
            if (t->mpSecondaryLOD) {
                if (t->mpSecondaryLOD) {
                    __dt__8046D144(t->mpSecondaryLOD, 1);
                    t->mpSecondaryLOD = 0;
                }
            }
            LOD::LODMemMan* lod = (LOD::LODMemMan*)mtl::MemManager::allocate(
                0x1ce0, mtl::MemManager::getHandleMEM1());
            // No explicit null guard: MWCC's placement-new emits its own
            // (retail has exactly one beq here).  Re-assigning the result
            // keeps `lod` re-derived from the ctor return (retail keeps it
            // in r0 across the call instead of a saved register).
            lod = new (lod) LOD::LODMemMan();
            t->mpSecondaryLOD = lod;
            t->mpActiveLOD = lod;
            func_8046E594__Q23LOD9LODMemManFv(&t->mLODMemMan, 0);
        }
        return func_8046D898__Q23LOD9LODMemManFv(
            lbl_eu_80665730[0]->mpActiveLOD, self, lbl_eu_80665730[0]->mParam1);
    }
    return 0;
}

// Teardown + re-bind: release the active LODMemMan's g3d objects, destroy the
// secondary LOD instance, then point mpActiveLOD back at the embedded
// mLODMemMan and enable the LOD system.  The whole body works on the global
// instance (retail never touches `this` except via the delegate call).
void CTaskLOD::func_80462AC0() {
    if (lbl_eu_80665730[0]) {
        func_8046DAC0__Q23LOD9LODMemManFv(lbl_eu_80665730[0]->mpActiveLOD, (int)this);
        CTaskLOD* t = lbl_eu_80665730[0];
        if (t->mpSecondaryLOD) {
            if (t->mpSecondaryLOD) {
                __dt__8046D144(t->mpSecondaryLOD, 1);
                t->mpSecondaryLOD = 0;
            }
        }
        t->mpActiveLOD = &t->mLODMemMan;
        func_8046E594__Q23LOD9LODMemManFv(&t->mLODMemMan, 1);
    }
}

// (Re)bind the singleton's active LOD: destroy the secondary instance, point
// mpActiveLOD at the embedded mLODMemMan, and enable the LOD system.  The
// whole body works on the global instance (retail never touches `this`).
void CTaskLOD::func_80462B68() {
    if (lbl_eu_80665730[0]) {
        CTaskLOD* t = lbl_eu_80665730[0];
        if (t->mpSecondaryLOD) {
            if (t->mpSecondaryLOD) {
                __dt__8046D144(t->mpSecondaryLOD, 1);
                t->mpSecondaryLOD = 0;
            }
        }
        t->mpActiveLOD = &t->mLODMemMan;
        func_8046E594__Q23LOD9LODMemManFv(&t->mLODMemMan, 1);
    }
}

// Calls the active LODMemMan's getter for its side-effect-free float access,
// then unconditionally returns the constant 1.0f (retail loads the 1.0f pool
// entry at 0x8066A5B8 after the call).
float CTaskLOD::func_80462C14() {
    if (lbl_eu_80665730[0]) {
        lbl_eu_80665730[0]->mpActiveLOD->func_8046F01C();
    }
    return 1.0f;
}

// Toggle the 0x40 flag bit on the active LODMemMan at offset 0x6c: set when
// `this` is non-null, clear when null.
void CTaskLOD::func_80462C48() {
    if (lbl_eu_80665730[0]) {
        LOD::LODMemMan* lod = lbl_eu_80665730[0]->mpActiveLOD;
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
    if (lbl_eu_80665730[0]) {
        if (this) {
            func_8046F024__Q23LOD9LODMemManFv(lbl_eu_80665730[0]->mpActiveLOD, 1, (int)this - 1);
        } else {
            func_8046F024__Q23LOD9LODMemManFv(lbl_eu_80665730[0]->mpActiveLOD, 0, 0);
        }
    }
}

void CTaskLOD::func_80462CBC() {
    if (lbl_eu_80665730[0]) {
        func_8046F088__Q23LOD9LODMemManFv(lbl_eu_80665730[0]->mpActiveLOD, this);
    }
}

// Fetches the active LODMemMan and forwards task + args (a, b) to it
// (retail `mr r8,r3; mr r0,r4; mr r6,r5` sets up this/a/b for the delegate).
// c is unused (dropped).  Defined as a global function carrying the retail
// `.Fv` symbol name (see note above).
void func_80462CD8__8CTaskLODFv(CTaskLOD* self, void* a, void* b, void* c) {
    if (lbl_eu_80665730[0]) {
        func_8046E770__Q23LOD9LODMemManFv(lbl_eu_80665730[0]->mpActiveLOD, self, a, b);
    }
}

// LOD activation wrappers: enable/disable LOD for `this` task on the active
// manager, then dispatch the same task + flag through the singleton's
// scene-light control list (func_804C2014 ring walk).
void CTaskLOD::func_80462D04() {
    if (lbl_eu_80665730[0]) {
        func_8046E780__Q23LOD9LODMemManFv(lbl_eu_80665730[0]->mpActiveLOD, this);
        func_804C2014(lbl_eu_80665730[0]->mParam1->field_0x7C, this, 1);
    }
}

void CTaskLOD::func_80462D5C() {
    if (lbl_eu_80665730[0]) {
        func_8046E7D0__Q23LOD9LODMemManFv(lbl_eu_80665730[0]->mpActiveLOD, this);
        func_804C2014(lbl_eu_80665730[0]->mParam1->field_0x7C, this, 0);
    }
}

// Setter-style wrapper: forwards the task + flag pair through the active
// manager and the scene-light ring.  Defined as a global carrying the retail
// `.Fv` symbol name because the body reads a real r4 argument (see the note
// on the func_80462CD8 family above).
extern "C" void func_80462DB4__8CTaskLODFv(CTaskLOD* self, int flag) {
    if (lbl_eu_80665730[0]) {
        func_8046E820__Q23LOD9LODMemManFv(lbl_eu_80665730[0]->mpActiveLOD, self, flag);
        func_804C2014(lbl_eu_80665730[0]->mParam1->field_0x7C, self, flag);
    }
}
// Value-delegates.  Like the void wrappers above, each forwards to the
// active LODMemMan when the singleton `lbl_eu_80665730` is set, passing
// `this` as the trailing CTaskLOD* argument (retail `mr r4, r3`).  The
// pointer/float returns come straight back from the delegate.
void* CTaskLOD::func_80462E1C() {
    if (lbl_eu_80665730[0]) {
        return func_8046E8C8__Q23LOD9LODMemManFv(lbl_eu_80665730[0]->mpActiveLOD, this);
    }
    return 0;
}

void CTaskLOD::func_80462E3C() {
    if (lbl_eu_80665730[0]) {
        func_8046E920__Q23LOD9LODMemManFv(lbl_eu_80665730[0]->mpActiveLOD, this);
    }
}

// Forward the task + arg pair to the active LODMemMan (func_8046E988), then
// walk the singleton's scene-light control ring (func_804C2094) with the same
// task/arg and the float value.  `b` is unused in retail.
extern "C" void func_80462E58__8CTaskLODFv(CTaskLOD* self, float val, void* a, void* b) {
    if (lbl_eu_80665730[0]) {
        func_8046E988__Q23LOD9LODMemManFv(lbl_eu_80665730[0]->mpActiveLOD, self, a);
        func_804C2094(lbl_eu_80665730[0]->mParam1->field_0x7C, val, self, a);
    }
}

// Same delegate-shape as func_80462CD8: forwards `self` + first arg `a` to the
// active LODMemMan (tail-call `b`).  `b` is unused in retail.
void func_80462ED0__8CTaskLODFv(CTaskLOD* self, void* a, void* b) {
    if (lbl_eu_80665730[0]) {
        func_8046EAE8__Q23LOD9LODMemManFv(lbl_eu_80665730[0]->mpActiveLOD, self, a);
    }
}

void CTaskLOD::func_80462EF4() {
    if (lbl_eu_80665730[0]) {
        func_8046EB50__Q23LOD9LODMemManFv(lbl_eu_80665730[0]->mpActiveLOD, this);
    }
}

void CTaskLOD::func_80462F10() {
    if (lbl_eu_80665730[0]) {
        func_8046EBEC__Q23LOD9LODMemManFv(lbl_eu_80665730[0]->mpActiveLOD, this);
    }
}

float CTaskLOD::func_80462F2C() {
    if (lbl_eu_80665730[0]) {
        return func_8046EC88__Q23LOD9LODMemManFv(lbl_eu_80665730[0]->mpActiveLOD, this);
    }
    return 0.0f;
}
void func_80462F4C__8CTaskLODFv(CTaskLOD* self, void* a, void* b) {
    if (lbl_eu_80665730[0]) {
        func_8046ECD4__Q23LOD9LODMemManFv(lbl_eu_80665730[0]->mpActiveLOD, self, a);
    }
}

void func_80462F70__8CTaskLODFv(CTaskLOD* self, void* a, void* b) {
    if (lbl_eu_80665730[0]) {
        func_8046ED68__Q23LOD9LODMemManFv(lbl_eu_80665730[0]->mpActiveLOD, self, a);
    }
}

void func_80462F94__8CTaskLODFv(CTaskLOD* self, void* a, void* b) {
    if (lbl_eu_80665730[0]) {
        func_8046EDD0__Q23LOD9LODMemManFv(lbl_eu_80665730[0]->mpActiveLOD, self, a);
    }
}
void* CTaskLOD::func_80462FB8() {
    if (lbl_eu_80665730[0]) {
        return func_8046EE9C__Q23LOD9LODMemManFv(lbl_eu_80665730[0]->mpActiveLOD, this);
    }
    return 0;
}

void CTaskLOD::func_80462FD8() {
    if (lbl_eu_80665730[0]) {
        func_8046EEE8__Q23LOD9LODMemManFv(lbl_eu_80665730[0]->mpActiveLOD, this);
    }
}

float CTaskLOD::func_80462FF4() {
    if (lbl_eu_80665730[0]) {
        return func_8046EF30__Q23LOD9LODMemManFv(lbl_eu_80665730[0]->mpActiveLOD, this);
    }
    return 0.0f;
}

// func_8046EF7C returns a pointer; when there is no active LOD the retail
// body returns `this` unchanged (beqlr with r3 still holding `this`).
void* CTaskLOD::func_80463014() {
    if (lbl_eu_80665730[0]) {
        return func_8046EF7C__Q23LOD9LODMemManFv(lbl_eu_80665730[0]->mpActiveLOD, this);
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

// ==== retail data owned by this TU (data-gate dissolve) ====
// The retail split object carries this class's data: RTTI class-name strings
// (.rodata), the CTaskLOD / CTTask<CTaskLOD> vtables and RTTI base tables
// (.data), the typeinfo descriptors and pooled "ref" strings (.sdata), and
// the singleton instance pointer (.sbss).  Each object is defined as an
// explicit array (same recipe as CDesktop.cpp / CWorkSystem.cpp) so the
// compiled sections reproduce retail bytes, relocation names, packing, and
// alignment exactly.
//
// The CTTask<CTaskLOD> template members referenced by lbl_eu_8056D678 /
// lbl_eu_8056D6C0 cannot be spelled in source (their mangled names contain
// '<'), so those slots carry a stand-in Tail__8CProcessFv reloc that the
// Section 17.6 UNIT_RULES retarget at the local .text definitions
// (retarget_relocs_local).

// Defined by this TU's member/destructor definitions under these exact
// mangled assembler names (extern "C" keeps MWCC from appending parameter
// encodings; only one definition exists at link time).
extern "C" void __dt__8CTaskLODFv();
extern "C" void Init__8CTaskLODFv();
extern "C" void Term__8CTaskLODFv();
extern "C" void Move__8CTaskLODFv();

// External vtable slots + RTTI base descriptors (owned by other TUs).
extern "C" void Reset__14CChildListNodeFv();
extern "C" void Tail__8CProcessFv();
extern "C" u32 lbl_eu_80661948[];  // RTTI CProcess descriptor
extern "C" u32 lbl_eu_80661950[];  // RTTI CChildListNode descriptor
extern "C" u32 lbl_eu_80661958[];  // RTTI CDoubleListNode descriptor
extern "C" u32 lbl_eu_806637F8[2]; // RTTI CTaskLOD descriptor (.sdata, below)
extern "C" u32 lbl_eu_80663800[2]; // RTTI CTTask<CTaskLOD> descriptor (.sdata, below)

// .rodata 0x80523D60..0x80523D80: RTTI class-name strings (each sized to its
// retail extent incl. tail padding).
extern "C" __declspec(section ".rodata") __attribute__((aligned(8))) const char lbl_eu_80523D60[12] = "CTaskLOD";
extern "C" __declspec(section ".rodata") const char lbl_eu_80523D6C[20] = "CTTask<CTaskLOD>";

// .data 0x8056D678..0x8056D700: vtables + RTTI base tables.
//   lbl_eu_8056D678: __vt__8CTaskLOD          {ti, 0, dtor, Reset, Init, Term,
//                                               Move, Draw17, Tail}
//   lbl_eu_8056D69C: CTaskLOD base table      {CDoubleListNode, CChildListNode,
//                                               CProcess, CTTask<CTaskLOD>}
//   lbl_eu_8056D6C0: __vt__17CTTask<8CTaskLOD>{ti, 0, dtor17, Reset, 0, 0,
//                                               Move17, Draw17, Tail}
//   lbl_eu_8056D6E4: CTTask<CTaskLOD> bases   {CDoubleListNode, CChildListNode,
//                                               CProcess}
extern "C" __declspec(section ".data") __attribute__((aligned(8))) u32 lbl_eu_8056D678[9] = {
    (u32)&lbl_eu_806637F8, 0,
    (u32)&__dt__8CTaskLODFv, (u32)&Reset__14CChildListNodeFv,
    (u32)&Init__8CTaskLODFv, (u32)&Term__8CTaskLODFv,
    (u32)&Move__8CTaskLODFv,
    (u32)&Tail__8CProcessFv,  // Draw__17CTTask<8CTaskLOD>Fv (retargeted)
    (u32)&Tail__8CProcessFv,
};
extern "C" __declspec(section ".data") u32 lbl_eu_8056D69C[9] = {
    (u32)&lbl_eu_80661958, 0, (u32)&lbl_eu_80661950, 0,
    (u32)&lbl_eu_80661948, 0, (u32)&lbl_eu_80663800, 0, 0,
};
extern "C" __declspec(section ".data") u32 lbl_eu_8056D6C0[9] = {
    (u32)&lbl_eu_80663800, 0,
    (u32)&Tail__8CProcessFv,  // __dt__17CTTask<8CTaskLOD>Fv (retargeted)
    (u32)&Reset__14CChildListNodeFv,
    0, 0,
    (u32)&Tail__8CProcessFv,  // Move__17CTTask<8CTaskLOD>Fv (retargeted)
    (u32)&Tail__8CProcessFv,  // Draw__17CTTask<8CTaskLOD>Fv (retargeted)
    (u32)&Tail__8CProcessFv,
};
extern "C" __declspec(section ".data") u32 lbl_eu_8056D6E4[7] = {
    (u32)&lbl_eu_80661958, 0, (u32)&lbl_eu_80661950, 0,
    (u32)&lbl_eu_80661948, 0, 0,
};

// .sdata 0x806637F8..0x80663820: RTTI type descriptors {name, base table},
// the pooled "ref" strings (referenced by code_804645CC.cpp), and the retail
// 4-byte section tail pad (folded into the last string's extent).
extern "C" __declspec(section ".sdata") __attribute__((aligned(8))) u32 lbl_eu_806637F8[2] = {
    (u32)&lbl_eu_80523D60, (u32)&lbl_eu_8056D69C,
};
extern "C" __declspec(section ".sdata") u32 lbl_eu_80663800[2] = {
    (u32)&lbl_eu_80523D6C, (u32)&lbl_eu_8056D6E4,
};
extern "C" __declspec(section ".sdata") char lbl_eu_80663808[4] = "ref";
extern "C" __declspec(section ".sdata") char lbl_eu_8066380C[4] = "ref";
extern "C" __declspec(section ".sdata") char lbl_eu_80663810[8] = "ref";
