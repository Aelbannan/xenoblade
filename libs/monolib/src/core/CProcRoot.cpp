#include "monolib/work.hpp"
#include "monolib/lib.hpp"
#include "monolib/core.hpp"
#include "monolib/util/MemManager.hpp"

extern "C" {
CProcRoot* lbl_eu_806655A0;
// Shared string pool: "CDesktop\0CProcRoot" - create uses +9 ("CProcRoot").
char lbl_eu_80522514[];
// Retail CProcRoot vtable (outside this .text-only split).
char lbl_eu_8056B2A8[];

mtl::ALLOC_HANDLE getWorkMem__17CWorkThreadSystemFv();
void* allocate__Q23mtl10MemManagerFUlUl(u32 size, mtl::ALLOC_HANDLE handle);
void __ct__11CWorkThreadFPCcP11CWorkThreadi(CWorkThread* ths, const char* name,
                                            CWorkThread* parent, int cap);
void entryWork__9CWorkUtilFP11CWorkThreadP11CWorkThreadb(CWorkThread* ths,
                                                         CWorkThread* parent,
                                                         bool prepend);
}

// Keep the static for ABI; retail singleton slot is lbl_eu_806655A0.
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
    const char* name;
    CWorkThread* parent;
    mtl::ALLOC_HANDLE handle;
    CProcRoot* procRoot;
    void* rootVt;
    u32 type;

    name = lbl_eu_80522514 + 9;
    parent = pParent;
    handle = getWorkMem__17CWorkThreadSystemFv();
    procRoot = (CProcRoot*)allocate__Q23mtl10MemManagerFUlUl(0x1C8, handle);

    if (procRoot == nullptr) {
        goto create_entry_work;
    }

    __ct__11CWorkThreadFPCcP11CWorkThreadi(procRoot, name, parent, MAX_CHILD);

    rootVt = lbl_eu_8056B2A8;
    type = THREAD_CPROCROOT;
    *(void**)procRoot = rootVt;
    lbl_eu_806655A0 = procRoot;
    procRoot->mType = (CWorkThread::ThreadType)type;

create_entry_work:
    entryWork__9CWorkUtilFP11CWorkThreadP11CWorkThreadb(procRoot, parent, false);
    return procRoot;
}
