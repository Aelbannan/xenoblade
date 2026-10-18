#include "kyoshin/cf/CfTaskMain.hpp"
#include "monolib/work/CProcess.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/util/MemManager.hpp"

// --- CTTask<cf::CfTaskMain> out-of-line specializations ---
// The canonical declared-only template emits no bodies; these explicit
// specializations produce the retail standalone Move/Draw/dtor symbols.
template<>
void CTTask<cf::CfTaskMain>::Move() {
    if (mMoveFunc) {
        (static_cast<cf::CfTaskMain*>(this)->*mMoveFunc)();
    }
}

template<>
void CTTask<cf::CfTaskMain>::Draw() {
    if (mDrawFunc) {
        (static_cast<cf::CfTaskMain*>(this)->*mDrawFunc)();
    }
}

template<>
CTTask<cf::CfTaskMain>::~CTTask() {}

// CfGameManager methods and raw CProcess ctor are declared in CfTaskMain.hpp
// with C linkage to match the retail symbol names exactly.

// Global null pointer-to-member-function constant (3 words), used to zero-fill
// the CTTask move/draw callback slots. Non-const: the object stores made during
// construction may alias it, which forces MWCC to reload each word per slot.
extern u32 __ptmf_null[3];

// Retail vtables in .data. lbl_eu_80526F58 is the CTTask<CfTaskMain> vtable;
// lbl_eu_80526E80 is the CfTaskMain primary vtable (IWorkEvent sub-vtable at +0x24).
extern char lbl_eu_80526F58[];
extern char lbl_eu_80526E80[];

// The complete-object destructor is declared in CfTaskMain.hpp (tail-called
// by the IWorkEvent dtor thunk).

namespace cf {

// Complete object destructor.
CfTaskMain::~CfTaskMain() {}

void CfTaskMain::Init() {}
void CfTaskMain::Term() {}

void CfTaskMain::Move() {
    if ((lbl_eu_80663E28 & 0x200000) != 0) return;
    func_8007F9C4__Q22cf13CfGameManagerFv();
}

void CfTaskMain::Draw() {
    refreshCameraIfCurrentView__Q22cf13CfGameManagerFv();
}

void CfTaskMain::Tail() {
    resetCameraIfCurrentView__Q22cf13CfGameManagerFv();
}

} // namespace cf

// Byte-range shim over the CProcess header + owned fields so the factory
// can write the vtable (+0x10), the __ptmf_null callback slots
// (+0x3C..0x50), the IWorkEvent secondary vtable (+0x54) and the trailing
// scalar (+0x58) with typed members. Mirrors the CUICfManager
// CUICfInitProcess pattern (interim vtable, vtFinal between the PTMF
// loads, per-slot temps).
struct CfTaskMainCtorShim {
    u8 _00[0x10];
    void* vtable;         // 0x10 - CProcess vtable, overwritten below
    u8 _14[0x28];         // 0x14-0x3B - rest of CProcess
    u32 moveCallbacks[3]; // 0x3C-0x44 - __ptmf_null move-callback slots
    u32 drawCallbacks[3]; // 0x48-0x50 - __ptmf_null draw-callback slots
    void* evtVtable;      // 0x54 - IWorkEvent secondary vtable
    u32 field58;          // 0x58
};

// Factory function: allocates, constructs, and registers a CfTaskMain.
//   r3 = parent CProcess* (for Regist)
//   r4 = u32 value stored at field_0x58
// Returns new CfTaskMain* (null if allocation failed).
//
// Construction is done in place (calling the raw CProcess ctor and writing the
// CfTaskMain fields directly) so that no separate CfTaskMain ctor symbol is
// emitted; retail keeps the whole body inline in this creator.
void* __ct__800697E8(CProcess* parent, u32 field58val) {
    cf::CfTaskMain* mem = static_cast<cf::CfTaskMain*>(
        mtl::MemManager::allocate(sizeof(cf::CfTaskMain),
                                  CWorkThreadSystem::getWorkMem()));
    cf::CfTaskMain* obj = static_cast<cf::CfTaskMain*>(mem);
    if (obj) {
        __ct__8CProcessFv(obj);
        CfTaskMainCtorShim* s = reinterpret_cast<CfTaskMainCtorShim*>(obj);
        // Single address materialization reused for the final vtable store
        // and the secondary-subobject slot.
        s->vtable = lbl_eu_80526F58;
        // The null-PTMF words are read through a pointer (round-tripped
        // through u32 so the base materializes once with lis/addi): direct
        // __ptmf_null[i] indexing lets MWCC fold the @l part into the
        // offset-0 loads and split the base across two registers, while
        // retail issues one addi and plain loads off a single base.
        volatile u32* ptmf =
            reinterpret_cast<volatile u32*>(reinterpret_cast<u32>(__ptmf_null));
        u32 ptmfWord1 = ptmf[1];
        char* vtFinal = lbl_eu_80526E80;
        u32 ptmfWord0 = ptmf[0];
        s->moveCallbacks[0] = ptmfWord0;
        s->moveCallbacks[1] = ptmfWord1;
        u32 ptmfWord2 = ptmf[2];
        s->moveCallbacks[2] = ptmfWord2;
        ptmfWord1 = ptmf[1];
        ptmfWord0 = ptmf[0];
        s->drawCallbacks[0] = ptmfWord0;
        s->drawCallbacks[1] = ptmfWord1;
        ptmfWord2 = ptmf[2];
        s->drawCallbacks[2] = ptmfWord2;
        s->vtable = vtFinal;
        s->evtVtable = vtFinal + 0x24;
        s->field58 = field58val;
    }
    // Regist runs even when the allocation failed (retail behaviour).
    obj->Regist(parent, false);
    return obj;
}

// IWorkEvent dtor adjusting thunk: subtracts 0x54 from IWorkEvent* to recover
// CfTaskMain*, then tail-calls the complete-object destructor.
// Retail: subi r3, r3, 0x54; b __dt__Q22cf10CfTaskMainFv
void CfTaskMain_dtorAdj54(void* self) {
    __dt__Q22cf10CfTaskMainFv(static_cast<char*>(self) - 0x54);
}