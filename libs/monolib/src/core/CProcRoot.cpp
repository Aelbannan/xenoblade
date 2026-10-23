#include "monolib/work.hpp"
#include "monolib/lib.hpp"
#include "monolib/core.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/data_vtables.hpp"

extern "C" {
CProcRoot* lbl_eu_806655A0;   // defined here (blob monolibdata1d dissolve; 4-byte sbss slot)
// Shared string pool: "CDesktop\0CProcRoot" - create uses +9 ("CProcRoot").
extern const char lbl_eu_80522514[];

// Base ctor called on raw storage: create() hand-inlines CProcRoot's ctor (see below).
extern "C" void __ct__11CWorkThreadFPCcP11CWorkThreadi(CWorkThread* ths, const char* name,
                                                        CWorkThread* parent, int cap);
}

// --- Blob monolibdata1.s / monolibdata1d.s dissolve: this TU owns .data
// 0x8056B2A8-0x8056B360, .rodata 0x80522508-0x80522528, .sdata 0x80663548-
// 0x80663550, .sbss 0x806655A0-0x806655A8. ---

// RTTI name string (.rodata).
extern const char lbl_eu_80522508[];
// RTTI locator (.sdata, 8 bytes): { name, base-list }.
extern u32 lbl_eu_80663548[2];

const char lbl_eu_80522508[] = "CProcRoot";
const char lbl_eu_80522514[] = "CDesktop\0CProcRoot\0";

// CProcRoot vtable (.data, 0xA0): [rtti, 0, dtor, IWorkEvent 1-31, wkUpdate/
// wkRender/wkRenderAfter (CWorkThread slots), wkStandbyLogin/Logout (CProcRoot),
// wkStandbyExceptionRetry].
u32 lbl_eu_8056B2A8[0xA0 / 4] = {
    (u32)&lbl_eu_80663548, 0, (u32)&__dt__9CProcRootFv,
    (u32)&WorkEvent1__10IWorkEventFPvPCc, (u32)&OnFileEvent__10IWorkEventFP10CEventFile,
    (u32)&WorkEvent3__10IWorkEventFPv, (u32)&WorkEvent4__10IWorkEventFv,
    (u32)&OnPauseTrigger__10IWorkEventFb,
    (u32)&WorkEvent6__10IWorkEventFv, (u32)&WorkEvent7__10IWorkEventFv,
    (u32)&WorkEvent8__10IWorkEventFv, (u32)&WorkEvent9__10IWorkEventFv,
    (u32)&WorkEvent10__10IWorkEventFv, (u32)&WorkEvent11__10IWorkEventFv,
    (u32)&WorkEvent12__10IWorkEventFv, (u32)&WorkEvent13__10IWorkEventFv,
    (u32)&WorkEvent14__10IWorkEventFv, (u32)&WorkEvent15__10IWorkEventFv,
    (u32)&WorkEvent16__10IWorkEventFv, (u32)&WorkEvent17__10IWorkEventFv,
    (u32)&WorkEvent18__10IWorkEventFv, (u32)&WorkEvent19__10IWorkEventFv,
    (u32)&WorkEvent20__10IWorkEventFv, (u32)&WorkEvent21__10IWorkEventFv,
    (u32)&WorkEvent22__10IWorkEventFv, (u32)&WorkEvent23__10IWorkEventFv,
    (u32)&WorkEvent24__10IWorkEventFv, (u32)&WorkEvent25__10IWorkEventFv,
    (u32)&WorkEvent26__10IWorkEventFv, (u32)&WorkEvent27__10IWorkEventFv,
    (u32)&WorkEvent28__10IWorkEventFv, (u32)&WorkEvent29__10IWorkEventFv,
    (u32)&WorkEvent30__10IWorkEventFv, (u32)&WorkEvent31__10IWorkEventFv,
    (u32)&wkUpdate__11CWorkThreadFv, (u32)&wkRender__11CWorkThreadFv,
    (u32)&wkRenderAfter__11CWorkThreadFv, (u32)&wkStandbyLogin__9CProcRootFv,
    (u32)&wkStandbyLogout__9CProcRootFv, (u32)&wkStandbyExceptionRetry__11CWorkThreadFUl,
};

// RTTI base list (.data, 0x18): [IWorkEvent, 0, CWorkThread, 0, 0, 0].
u32 lbl_eu_8056B348[6] = {
    (u32)&__RTTI__10IWorkEvent, 0, (u32)&__RTTI__11CWorkThread, 0, 0, 0,
};

u32 lbl_eu_80663548[2] = { (u32)lbl_eu_80522508, (u32)lbl_eu_8056B348 };

// spInstance (sbss lbl_eu_806655A4) - defined here so this TU owns its data
// (blob monolibdata1d dissolve). Retail code accesses the singleton via the
// flat lbl_eu_806655A0 label (4-byte sbss slot).
CProcRoot* CProcRoot::spInstance;

// No out-of-line __ct__9CProcRoot - retail create inlines CWorkThread ctor +
// vtable/singleton/mType stores (same pattern as CViewRoot::create).

CProcRoot::~CProcRoot() {
    lbl_eu_806655A0 = nullptr;
}

CProcRoot* CProcRoot::getInstance() {
    return lbl_eu_806655A0;
}

bool CProcRoot::wkStandbyLogin() {
    const char* name;

    if (isNoEvent()) {
        return CWorkThread::wkStandbyLogin();
    }

    if (CLib::isInitialized() && CViewRoot::isInitialized()) {
        // CREATE_WORKTHREAD(CDesktop, this) with retail string-pool label.
        // Materialize name before create so MWCC homes it in r31 / this in r30.
        name = lbl_eu_80522514;
        CDesktop::create(name, this);
        return CWorkThread::wkStandbyLogin();
    }

    return false;
}

bool CProcRoot::wkStandbyLogout() {
    // TODO: maybe this should be an inline
    return !mChildren.empty() ? false : CWorkThread::wkStandbyLogout();
}

CProcRoot* CProcRoot::create(CWorkThread* pParent) {
    const char* name = lbl_eu_80522514 + 9; // "CProcRoot"
    CWorkThread* parent = pParent;
    mtl::ALLOC_HANDLE handle = CWorkThreadSystem::getWorkMem();
    CProcRoot* procRoot = static_cast<CProcRoot*>(mtl::MemManager::allocate(sizeof(CProcRoot), handle));

    if (procRoot != nullptr) {
        // Inlined CProcRoot(name, parent): base ctor, vptr, singleton, thread type.
        // The vtable is the hand-laid lbl_eu_8056B2A8 (class is novtable), so the
        // vptr store has to be explicit.
        __ct__11CWorkThreadFPCcP11CWorkThreadi(procRoot, name, parent, MAX_CHILD);
        *reinterpret_cast<u32**>(procRoot) = lbl_eu_8056B2A8;
        lbl_eu_806655A0 = procRoot;
        procRoot->mType = THREAD_CPROCROOT;
    }

    CWorkUtil::entryWork(procRoot, parent, false);
    return procRoot;
}
