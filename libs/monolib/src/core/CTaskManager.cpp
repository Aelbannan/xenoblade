#include "monolib/core.hpp"
#include "monolib/device.hpp"
#include "monolib/work.hpp"
#include "monolib/core/CTaskManager.hpp"

// Blob monolibdata1/1d dissolve: this TU also owns .rodata 0x80522588-
// 0x805225E0 (two RTTI name strings, 40B + 48B), .sdata 0x80663568-
// 0x80663578 (two {name, base-list} RTTI locators), and .data 0x8056B55C-
// 0x8056B5E0 (the CRootProc base list + vtables; the CTTask<CRootProc>
// dtor/Move/Draw slots cannot be spelled in C++ -- MWCC emits the vtables
// from the class definitions below and UNIT_RULES["CTaskManager.o"] reshapes
// them).

// RTTI name strings (.rodata). align(4) so they pack 4-aligned (40B + 48B =
// 0x58, no padding slots).
extern "C" __declspec(align(4)) const char lbl_eu_80522588[40] =
    {0x40,0x75,0x6E,0x6E,0x61,0x6D,0x65,0x64,0x40,0x43,0x54,0x61,0x73,0x6B,0x4D,0x61,0x6E,0x61,0x67,0x65,0x72,0x5F,0x63,0x70,0x70,0x40,0x3A,0x3A,0x43,0x52,0x6F,0x6F,0x74,0x50,0x72,0x6F,0x63,0x00,0x00,0x00};  /* "@unnamed@CTaskManager_cpp@::CRootProc\0\0\0" */
extern "C" __declspec(align(4)) const char lbl_eu_805225B0[48] =
    {0x43,0x54,0x54,0x61,0x73,0x6B,0x3C,0x40,0x75,0x6E,0x6E,0x61,0x6D,0x65,0x64,0x40,0x43,0x54,0x61,0x73,0x6B,0x4D,0x61,0x6E,0x61,0x67,0x65,0x72,0x5F,0x63,0x70,0x70,0x40,0x3A,0x3A,0x43,0x52,0x6F,0x6F,0x74,0x50,0x72,0x6F,0x63,0x3E,0x00,0x00,0x00};  /* "CTTask<@unnamed@CTaskManager_cpp@::CRootProc>\0\0\0" */

// RTTI locators (.sdata, 8 bytes each): { class-name, base-list }. The base
// lists live at the head of this TU's .data (lbl_eu_8056B55C = CRootProc's
// base list, lbl_eu_8056B55C+0x48 = CTTask<CRootProc>'s base list); the .data
// words are provided by MWCC's auto vtable emission + UNIT_RULES reshaping.
// RTTI base structs referenced by the vtable chain (.sdata, other TUs).
extern "C" u32 lbl_eu_80661958;
extern "C" u32 lbl_eu_80661950;
extern "C" u32 lbl_eu_80661948;
extern "C" void Reset__14CChildListNodeFv();
extern "C" void Tail__8CProcessFv();
extern "C" void updateMsg__5CViewFv();

extern "C" u32 lbl_eu_80663570[2];

// .data 0x8056B55C-0x8056B5E0 (132B): CRootProc vtable array (RTTI chain +
// CTTask<CRootProc> segment + secondary chain) + CView::updateMsg jumptable.
// The three CTTask<...> slots (+0x2C/+0x3C/+0x40, '<'-unspellable) are zero
// here and injected by UNIT_RULES["CTaskManager.o"] inject_relocs.
extern "C" u32 lbl_eu_8056B55C[33] = {
    (u32)&lbl_eu_80661958, 0, (u32)&lbl_eu_80661950, 0, (u32)&lbl_eu_80661948, 0,
    (u32)&lbl_eu_80663570, 0, 0, (u32)&lbl_eu_80663570, 0,
    0,                                      /* +0x2C __dt__48CTTask<...>Fv (injected) */
    (u32)&Reset__14CChildListNodeFv,        /* +0x30 */
    0, 0,
    0,                                      /* +0x3C Move__48CTTask<...>Fv (injected) */
    0,                                      /* +0x40 Draw__48CTTask<...>Fv (injected) */
    (u32)&Tail__8CProcessFv,                /* +0x44 */
    (u32)&lbl_eu_80661958, 0, (u32)&lbl_eu_80661950, 0, (u32)&lbl_eu_80661948, 0, 0,
    (u32)&updateMsg__5CViewFv, (u32)&updateMsg__5CViewFv,
    (u32)&updateMsg__5CViewFv, (u32)&updateMsg__5CViewFv,
    (u32)&updateMsg__5CViewFv, (u32)&updateMsg__5CViewFv,
    (u32)&updateMsg__5CViewFv, (u32)&updateMsg__5CViewFv,
};
// .data + 0x48: CTTask<CRootProc> base-list label (defined by
// UNIT_RULES["CTaskManager.o"] add_symbols at offset 0x48; UNDEF here so the
// .sdata locator below relocates to the retail symbol).
extern "C" u32 lbl_eu_8056B5A4[];
extern "C" u32 lbl_eu_80663568[2] = { (u32)&lbl_eu_80522588, (u32)&lbl_eu_8056B55C };
extern "C" u32 lbl_eu_80663570[2] = { (u32)&lbl_eu_805225B0, (u32)&lbl_eu_8056B5A4 };

// Blob monolibdata1d dissolve: sbss slots owned by this TU (retail
// lbl_eu_806655B8/BC/C0/C4). Declared extern "C" in CTaskManager.hpp; the
// initializers make these the definitions (zero-fill -> .sbss).
extern "C" CRootProc* lbl_eu_806655B8 = 0;
extern "C" CRootProc* lbl_eu_806655BC = 0;
extern "C" CRootProc* lbl_eu_806655C0 = 0;
extern "C" CRootProc* lbl_eu_806655C4 = 0;

namespace {

/*
Root (top-level) process.

Serves as an empty parent process for children tasks.
*/
class CRootProc : public CTTask<CRootProc> {
public:
    virtual void Init() {}
    virtual void Term() {}

    virtual void Move() {}
    virtual void Draw() {}
};

// --- CTTask<CRootProc> out-of-line specializations ---
// CRootProc is anonymous-namespace-local (no retail counterpart); the
// canonical declared-only template emits no bodies, so these provide the
// definitions referenced by the emitted interim CTTask<CRootProc> vtable.
// (MWCC accepts these inside the unnamed namespace only because the template
// members are declared-only; inline bodies would raise 10333 redefined.)
template<>
void CTTask<CRootProc>::Move() {
    if (mMoveFunc) {
        (static_cast<CRootProc*>(this)->*mMoveFunc)();
    }
}

template<>
void CTTask<CRootProc>::Draw() {
    if (mDrawFunc) {
        (static_cast<CRootProc*>(this)->*mDrawFunc)();
    }
}

template<>
CTTask<CRootProc>::~CTTask() {}

} // namespace

void CTaskManager::Create() {
    CProcessMan::Init();
    CProcessMan::Reset();

    Start();
}

// Inline: retail inlines Start into Create/Reset (both 0x23C bodies contain
// the full registration sequence) and emits NO standalone Start symbol.
// Non-inline MWCC would emit an out-of-line copy (+0x234 over split budget).
inline void CTaskManager::Start() {
    //All register with NULL parent because they are root-level processes
    lbl_eu_806655BC = new (CWorkThreadSystem::getWorkMem()) CRootProc();
    lbl_eu_806655BC->Regist(nullptr, false);

    lbl_eu_806655B8 = new (CWorkThreadSystem::getWorkMem()) CRootProc();
    lbl_eu_806655B8->Regist(nullptr, false);

    lbl_eu_806655C0 = new (CWorkThreadSystem::getWorkMem()) CRootProc();
    lbl_eu_806655C0->Regist(nullptr, false);

    lbl_eu_806655C4 = new (CWorkThreadSystem::getWorkMem()) CRootProc();
    lbl_eu_806655C4->Regist(nullptr, false);
}

void CTaskManager::Release() {
    CProcessMan::Term();

    lbl_eu_806655B8 = nullptr;
    lbl_eu_806655BC = nullptr;
    lbl_eu_806655C0 = nullptr;
    lbl_eu_806655C4 = nullptr;
}

void CTaskManager::Move() {
    CProcessMan::Move();
    CProcessMan::Delete();
}

void CTaskManager::Draw() {
    CProcessMan::Draw();
}

void CTaskManager::Reset() {
    CDeviceVI::waitForDrawDone();
    CProcessMan::Reset();

    Start();
}

CProcess* CTaskManager::GetRootProcGame() {
    return lbl_eu_806655B8;
}

CProcess* CTaskManager::GetRootProcRealTime() {
    return lbl_eu_806655BC;
}

CProcess* CTaskManager::GetRootProcScn() {
    return lbl_eu_806655C0;
}

