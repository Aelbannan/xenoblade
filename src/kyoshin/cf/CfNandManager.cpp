// Auto-scaffolded catalog TU for kyoshin/cf/CfNandManager
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/cf/CfNandManager.hpp"
#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CSaveLoad.hpp"

// forward declarations for scaffold thunk references
void func_8023E4D4__Q22cf13CfNandManagerFv(void*);
void __dt__Q22cf13CfNandManagerFv(void*);
void cbRenderBefore__Q22cf13CfNandManagerFv(void*);

void __ct__cf_CfNandManager(){}

// Destructor of the CfNandEventQueue member embedded in CfNandManager at +0x60
// (retail placeholder name __dt__8023BFCC): zeroes the queue head/tail
// counters, then frees the block when the deleting flag is set.
extern "C" void* __dt__8023BFCC(CfNandEventQueue* self, int dealloc_flag) {
    if (self != nullptr) {
        self->mTail = 0;
        self->mHead = 0;
        if (dealloc_flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

cf::CfNandManager::~CfNandManager() {}

void Init__Q22cf13CfNandManagerFv(void) {
    extern void func_804DA1B4(void*);
    func_804DA1B4(&lbl_eu_8065FD00);
}

void cf::CfNandManager::Term() {
    // Free the heap buffer, then clear the singleton pointer.
    if (field_0x5C != nullptr) {
        __dl__FPv(field_0x5C);
        field_0x5C = nullptr;
    }
    lbl_eu_80664768 = nullptr;
}

void cf::CfNandManager::Move() {
    // Skip the NAND completion pump while the global mode-flag bit is set.
    if (lbl_eu_80663E28 & 0x200000) {
        return;
    }
    func_804DA1CC(&lbl_eu_8065FD00);
    // Dispatch the +0x188 move hook through __ptmf_test/__ptmf_scall when set.
    if (__ptmf_test(&mMoveHook)) {
        (this->*mMoveHook)();
    }
}

extern char lbl_eu_80576CC0[];
extern "C" void* func_8023C1B4() { return lbl_eu_80576CC0; }

// Returns 1 while the NAND request queue is busy (head != tail) or the async
// flag is set; the result variable shape reproduces the retail li r3,0 / beqlr.
u32 func_8023C1C0() {
    u32 result = 0;
    if (lbl_eu_8065FD00.mHead != lbl_eu_8065FD00.mTail || lbl_eu_8066476C != 0) {
        result = 1;
    }
    return result;
}

void func_8023C1F0(){}

void func_8023C2E4(){}

void func_8023C68C(){}

void func_8023C7C4(){}

void func_8023C93C(){}

void func_8023CD9C(){}

void* func_8023D3D8() { return nullptr; }

// NAND teardown: guards on the singleton's heap buffer. The `buf ? buf : 0`
// self-assignment reproduces the retail materialized-bool diamond (the buffer
// word is re-tested with its own cr0 before the main path).
void* __dt__8023E448() {
    u8* buf = lbl_eu_80664768->field_0x5C;
    buf = buf != 0 ? buf : 0;
    if (buf == 0) {
        return 0;
    }
    lbl_eu_80664772 = 1;
    void* result = func_8023D3D8();
    lbl_eu_80664772 = 0;
    if (lbl_eu_80664768->field_0x5C != 0) {
        __dl__FPv(lbl_eu_80664768->field_0x5C);
        lbl_eu_80664768->field_0x5C = 0;
    }
    return result;
}

u32 cf::CfNandManager::func_8023E4D4() {
    lbl_eu_8066476C = 0;
    // The list-node words (mPrev at +0x00, mParent at +0x14) are repurposed as
    // event state: 1 -> run the game-manager handler, 4 -> drop the buffer.
    if (mPrev == (CDoubleListNode*)1 && mParent != 0) {
        func_80084F50__Q22cf13CfGameManagerFv();
    } else if (mPrev == (CDoubleListNode*)4) {
        if (lbl_eu_80664768 != 0 && lbl_eu_80664768->field_0x5C != 0) {
            __dl__FPv(lbl_eu_80664768->field_0x5C);
            lbl_eu_80664768->field_0x5C = 0;
        }
    }
    return 1;
}

void func_8023E544(){}

extern "C" void __dt__8023E63C(u8* self) {}

void func_8023EABC(){}

extern "C" void func_8023EB78(u8* self) {}

extern "C" void func_8023F288(u8* self) {
    (void)self;
    __dt__8023E63C(self);
}

void func_8023F2F4(){}

void func_8023F3C0(){}

void func_8023F5CC(){}

void func_8023F690(){}

void func_8023F860(){}

void func_8023FA64(){}

// Push an event {word, 0x28000, 0, 1, byte, 0} onto the manager's ring.
// Returns 1 on success, 0 when the singleton is not set up.
u32 func_8023FB28(u8 byte, u32 word) {
    u32 result;
    if (lbl_eu_80664768 != 0) {
        cf::CfNandManager* mgr = lbl_eu_80664768;
        CfNandEvent* entry =
            &mgr->mEventQueue.mRingBase[(mgr->mEventQueue.mHead + mgr->mEventQueue.mTail) %
                                        mgr->mEventQueue.mSize];
        entry->mWord = word;
        entry->mTag = 0x28000;
        entry->mField8 = 0;
        entry->mFieldC = 1;
        entry->mFieldE = byte;
        entry->mFieldF = 0;
        mgr->mEventQueue.mTail++;
        result = 1;
    } else {
        result = 0;
    }
    return result;
}

// Push an event {word, 0x28000, 0, 0x10, 0, 0} onto the manager's ring.
// Returns 1 on success, 0 when the singleton is not set up.
u32 func_8023FBA0(u32 word) {
    if (lbl_eu_80664768 == 0) {
        return 0;
    }
    if (lbl_eu_80664768 != 0) {
        cf::CfNandManager* mgr = lbl_eu_80664768;
        CfNandEvent* entry =
            &mgr->mEventQueue.mRingBase[(mgr->mEventQueue.mHead + mgr->mEventQueue.mTail) %
                                        mgr->mEventQueue.mSize];
        entry->mWord = word;
        entry->mTag = 0x28000;
        entry->mField8 = 0;
        entry->mFieldC = 0x10;
        entry->mFieldE = 0;
        entry->mFieldF = 0;
        mgr->mEventQueue.mTail++;
    }
    return 1;
}

void func_8023FC18(){}

// Push an event {word, 0x10000, 0, 9, 0, 0} onto the manager's ring.
// Returns 1 on success, 0 when the singleton is not set up.
u32 func_8023FCCC(u32 word) {
    u32 result;
    if (lbl_eu_80664768 == 0) {
        return 0;
    }
    if (lbl_eu_80664768 != 0) {
        cf::CfNandManager* mgr = lbl_eu_80664768;
        CfNandEvent* entry =
            &mgr->mEventQueue.mRingBase[(mgr->mEventQueue.mHead + mgr->mEventQueue.mTail) %
                                        mgr->mEventQueue.mSize];
        entry->mWord = word;
        entry->mTag = 0x10000;
        entry->mField8 = 0;
        entry->mFieldC = 9;
        entry->mFieldE = 0;
        entry->mFieldF = 0;
        mgr->mEventQueue.mTail++;
        result = 1;
    } else {
        result = 0;
    }
    return result;
}

void func_8023FD4C(){}

void func_8023FEDC(){}

void cf::CfNandManager::create() {}

// Returns whether a NAND file operation is in flight: reads the heap-buffer
// pointer at +0x5C of the CfNandManager singleton (null singleton -> 0).
u32 func_8024005C() {
    if (lbl_eu_80664768 != nullptr) {
        return lbl_eu_80664768->field_0x5C != 0;
    }
    return 0;
}

void func_80240084(){}

void func_80240360(){}

void func_80240420() {
    // Teardown: drop the NAND manager's render callback from the D80 scene,
    // free the heap buffer, and clear the u16 counter.
    CScn* scene = (CScn*)CfRes_getD80Flag();
    if (scene != nullptr) {
        scene->removeRenderCB(static_cast<IScnRender*>(lbl_eu_80664768));
    }
    if (lbl_eu_8066477C != nullptr) {
        mtl::MemManager::deallocate(lbl_eu_8066477C);
        lbl_eu_8066477C = nullptr;
    }
    lbl_eu_80664770 = 0;
}

void cf::CfNandManager::cbRenderBefore() {}

void OnFileEvent__Q22cf13CfNandManagerFv(void* self) { ((void(*)(void*))func_8023E4D4__Q22cf13CfNandManagerFv)((char*)self - 0x54); }

void func_802405FC__Q22cf13CfNandManagerFv(void* self) { ((void(*)(void*))__dt__Q22cf13CfNandManagerFv)((char*)self - 0x54); }

void func_80240604__Q22cf13CfNandManagerFv(void* self) { ((void(*)(void*))cbRenderBefore__Q22cf13CfNandManagerFv)((char*)self - 0x58); }

extern "C" void func_8024060C__Q22cf13CfNandManagerFv(cf::CfNandManager* self) { ((void(*)(void*))__dt__Q22cf13CfNandManagerFv)((char*)self - 0x58); }

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// CTTask<T> is declared in kyoshin/CTaskGameEff.hpp (via harness_catalog.hpp);
// specializations below emit the retail Move/Draw/dtor symbols.
namespace cf { class CfNandManager; }
template<> CTTask<cf::CfNandManager>::~CTTask() {}

// Dispatch the +0x3C move callback through __ptmf_test/__ptmf_scall when set.
template<>
void CTTask<cf::CfNandManager>::Move() {
    if (__ptmf_test(&mMoveFunc)) {
        (static_cast<cf::CfNandManager*>(this)->*mMoveFunc)();
    }
}

// Dispatch the +0x48 draw callback through __ptmf_test/__ptmf_scall when set.
template<>
void CTTask<cf::CfNandManager>::Draw() {
    if (__ptmf_test(&mDrawFunc)) {
        (static_cast<cf::CfNandManager*>(this)->*mDrawFunc)();
    }
}
extern "C" void sinit_802405F0(void) {}
