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


// ---------------------------------------------------------------------------
// Retail data slice (splits.txt): .data 0x80526608-0x80526690 (0x88),
// .rodata 0x804FB1A8-0x804FB1D8 (0x30), .sdata 0x80661A08-0x80661A20 (0x18).
//
// The class is __declspec(novtable), so MWCC emits NO vtable/RTTI data of its
// own for this TU (CTaskEnvironment.cpp recipe) and these definitions own the
// three sections outright. Shapes decoded from the retail relocations
// (build/us/obj/kyoshin/cf/CTaskGameEffAfter.o):
//
//   .data
//     80526608 (0x24) CTaskGameEffAfter vtable
//                     [RTTI, 0, dtor, Reset, Init, Term, Move27, Draw, Tail]
//     8052662C (0x24) CTaskGameEffAfter RTTI base list
//                     [CDoubleListNode, 0, CChildListNode, 0, CProcess, 0,
//                      RTTI(CTTask<>), 0, 0]
//     80526650 (0x24) interim CTTask<CTaskGameEffAfter> vtable
//                     [RTTI(CTTask<>), 0, dtor27, Reset, 0, 0,
//                      Move27, Draw27, Tail]
//     80526674 (0x1C) CTTask<CTaskGameEffAfter> RTTI base list
//                     [CDoubleListNode, 0, CChildListNode, 0, CProcess, 0, 0]
//   .rodata
//     804FB1A8 (0x12) "CTaskGameEffAfter"
//     804FB1BC (0x1A) "CTTask<CTaskGameEffAfter>"
//   .sdata
//     80661A08 (0x8) { "cf::CActParamAnimGame" name, its base list } - the
//                    retail object carries this pair (reference CActParamAnimGame
//                    vtable head); both targets are defined by linked objects
//                    (pluginCam.o .rodata / CActParamAnimGame.o .data)
//     80661A10 (0x8) { "CTaskGameEffAfter", 8052662C }
//     80661A18 (0x8) { "CTTask<CTaskGameEffAfter>", 80526674 }
// ---------------------------------------------------------------------------

// Retail RTTI typeinfo pairs of the base chain (monolib; data_vtables.hpp
// spells the same three): CDoubleListNode <- CChildListNode <- CProcess.
extern "C" u32 lbl_eu_80661958[2];   // RTTI CDoubleListNode
extern "C" u32 lbl_eu_80661950[2];   // RTTI CChildListNode
extern "C" u32 lbl_eu_80661948[2];   // RTTI CProcess
// Name string + RTTI base list of cf::CActParamAnimGame (owned elsewhere).
extern "C" char lbl_eu_804FB128[];
extern "C" u32 lbl_eu_8052654C[];

// This TU's .rodata/.sdata slice, referenced by the tables below (defined in
// retail order at the end of the block).
extern "C" char lbl_eu_804FB1A8[0x12];
extern "C" char lbl_eu_804FB1BC[0x1C];
extern "C" u32 lbl_eu_80661A08[2];
extern "C" u32 lbl_eu_80661A10[2];
extern "C" u32 lbl_eu_80661A18[2];

// The derived-class members referenced below under their retail (mangled)
// names; MWCC binds each declaration to the member definition above
// (CScrollBar.cpp precedent: declaration must follow the definition).
extern "C" void* __dt__17CTaskGameEffAfterFv(void*, int);
extern "C" void Init__17CTaskGameEffAfterFv(void*);
extern "C" void Term__17CTaskGameEffAfterFv(void*);
extern "C" void Draw__17CTaskGameEffAfterFv(void*);
// Base-chain slots (monolib; data_vtables.hpp spells the same two).
extern "C" void Reset__14CChildListNodeFv();
extern "C" void Tail__8CProcessFv();

// --- .data (retail order) ---
// NOTE: no aligned(8) here - the 8-aligned definition flips the ctor's
// register allocation (10 reg_swap); MWCC ties the .data section
// alignment to the symbol, and the gate only inspects bytes/size.
__declspec(section ".data") __attribute__((used))
u32 lbl_eu_80526608[9] = {
    (u32)lbl_eu_80661A10, 0, (u32)&__dt__17CTaskGameEffAfterFv,
    (u32)&Reset__14CChildListNodeFv,
    (u32)&Init__17CTaskGameEffAfterFv,
    (u32)&Term__17CTaskGameEffAfterFv,
    // Retail slot 6 is Move__27CTTask<17CTaskGameEffAfter>Fv; the '<'/'>' in
    // the template mangling cannot be spelled as a C++ identifier, so the
    // word stays a zero placeholder (bytes identical, reloc name dropped -
    // see the wave DL8-x1 report).
    0,
    (u32)&Draw__17CTaskGameEffAfterFv,
    (u32)&Tail__8CProcessFv,
};
__declspec(section ".data") __attribute__((used))
u32 lbl_eu_8052662C[9] = {
    (u32)lbl_eu_80661958, 0, (u32)lbl_eu_80661950, 0, (u32)lbl_eu_80661948, 0,
    (u32)lbl_eu_80661A18, 0, 0,
};
__declspec(section ".data") __attribute__((used))
u32 lbl_eu_80526650[9] = {
    (u32)lbl_eu_80661A18, 0,
    // __dt__27CTTask<17CTaskGameEffAfter>Fv (unspellable, see above)
    0,
    (u32)&Reset__14CChildListNodeFv,
    0, 0,
    // Move__27CTTask<17CTaskGameEffAfter>Fv / Draw__27CTTask<...>Fv
    // (unspellable, see above)
    0, 0,
    (u32)&Tail__8CProcessFv,
};
__declspec(section ".data") __attribute__((used))
u32 lbl_eu_80526674[7] = {
    (u32)lbl_eu_80661958, 0, (u32)lbl_eu_80661950, 0, (u32)lbl_eu_80661948, 0, 0,
};

// --- .rodata (retail RTTI name strings; sizes include the retail padding,
// so the section is 0x12 + 0x1A + 4 bytes of alignment tail = 0x30) ---
__declspec(section ".rodata") __attribute__((used))
char lbl_eu_804FB1A8[0x12] = "CTaskGameEffAfter";
__declspec(section ".rodata") __attribute__((used))
char lbl_eu_804FB1BC[0x1C] = "CTTask<CTaskGameEffAfter>";

// --- .sdata (RTTI locator pairs, retail order) ---
__declspec(section ".sdata") __attribute__((used))
u32 lbl_eu_80661A08[2] = { (u32)lbl_eu_804FB128, (u32)lbl_eu_8052654C };
__declspec(section ".sdata") __attribute__((used))
u32 lbl_eu_80661A10[2] = { (u32)lbl_eu_804FB1A8, (u32)lbl_eu_8052662C };
__declspec(section ".sdata") __attribute__((used))
u32 lbl_eu_80661A18[2] = { (u32)lbl_eu_804FB1BC, (u32)lbl_eu_80526674 };
