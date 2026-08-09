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
extern const u8 lbl_eu_80526F58[];
extern const u8 lbl_eu_80526E80[];

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
    func_8007FBFC__Q22cf13CfGameManagerFv();
}

void CfTaskMain::Tail() {
    func_8007FC2C__Q22cf13CfGameManagerFv();
}

} // namespace cf

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

        // In-place construction of CTTask<CfTaskMain> + CfTaskMain, using
        // u32 word indexing (see CTaskGameCf): reassigning w0/w1 from
        // __ptmf_null between the two callback slots forces MWCC to reload
        // each null PTMF word (retail issues six separate loads, not CSE'd).
        u32* p = reinterpret_cast<u32*>(obj);
        const u32* nullPt = &__ptmf_null[0];
        p[4] = reinterpret_cast<u32>(lbl_eu_80526F58);   // interim CTTask vtable
        u32 ptmf1 = nullPt[1];
        u32 ptmf0 = nullPt[0];
        p[0xF] = ptmf0;        // 0x3C mMoveFunc[0]
        p[0x10] = ptmf1;       // 0x40 mMoveFunc[1]
        p[0x11] = nullPt[2];   // 0x44 mMoveFunc[2]
        ptmf1 = nullPt[1];
        ptmf0 = nullPt[0];
        p[0x12] = ptmf0;       // 0x48 mDrawFunc[0]
        p[0x13] = ptmf1;       // 0x4C mDrawFunc[1]
        p[0x14] = nullPt[2];   // 0x50 mDrawFunc[2]
        p[4] = reinterpret_cast<u32>(lbl_eu_80526E80);    // CfTaskMain vtable
        *reinterpret_cast<u32*>(reinterpret_cast<u8*>(obj) + 0x54) =
            reinterpret_cast<u32>(lbl_eu_80526E80 + 0x24); // IWorkEvent vtable
        *reinterpret_cast<u32*>(reinterpret_cast<u8*>(obj) + 0x58) = field58val;
    }
    // Regist runs even when the allocation failed (retail behaviour).
    obj->Regist(parent, false);
    return obj;
}

// IWorkEvent dtor adjusting thunk: subtracts 0x54 from IWorkEvent* to recover
// CfTaskMain*, then tail-calls the complete-object destructor.
// Retail: subi r3, r3, 0x54; b __dt__Q22cf10CfTaskMainFv
void func_80069944(void* self) {
    __dt__Q22cf10CfTaskMainFv(static_cast<char*>(self) - 0x54);
}