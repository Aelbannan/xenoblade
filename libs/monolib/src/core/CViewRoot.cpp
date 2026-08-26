#include "monolib/core/CViewRoot.hpp"
#include "monolib/core/CView.hpp"
#include "monolib/core/CDesktop.hpp"
#include "monolib/core/CProc.hpp"
#include "monolib/core/CDesktop.hpp"
#include "monolib/device/CDeviceGX.hpp"
#include "monolib/device/CGXCache.hpp"
#include "monolib/device/CDevice.hpp"
#include "monolib/core/CProcRoot.hpp"
#include "monolib/lib/CLib.hpp"
#include "monolib/work/CWorkUtil.hpp"
#include "monolib/work/CMsgParam.hpp"
#include "monolib/util/MemManager.hpp"
#include "decomp.h"

// NOTE: this TU cannot include monolib/data_vtables.hpp -- its 0-arg
// `extern "C" void __dt__9CViewRootFv();` declaration collides under C
// linkage with the freestanding destructor definition below (MWCC 10197,
// same class of conflict as the __dt__11CWorkThreadFv note in that header).
// The symbols this TU needs are therefore declared here directly.
extern "C" {
extern void* __RTTI__10IWorkEvent;
extern void* __RTTI__11CWorkThread;
extern int WorkEvent1__10IWorkEventFPvPCc(void*, const char*);
extern int OnFileEvent__10IWorkEventFP10CEventFile(void*);
extern int WorkEvent3__10IWorkEventFPv(void*);
extern int WorkEvent4__10IWorkEventFv();
extern void OnPauseTrigger__10IWorkEventFb(int);
extern int WorkEvent6__10IWorkEventFv();
extern int WorkEvent7__10IWorkEventFv();
extern int WorkEvent8__10IWorkEventFv();
extern int WorkEvent9__10IWorkEventFv();
extern int WorkEvent10__10IWorkEventFv();
extern int WorkEvent11__10IWorkEventFv();
extern int WorkEvent12__10IWorkEventFv();
extern int WorkEvent13__10IWorkEventFv();
extern int WorkEvent14__10IWorkEventFv();
extern int WorkEvent15__10IWorkEventFv();
extern int WorkEvent16__10IWorkEventFv();
extern int WorkEvent17__10IWorkEventFv();
extern int WorkEvent18__10IWorkEventFv();
extern int WorkEvent19__10IWorkEventFv();
extern int WorkEvent20__10IWorkEventFv();
extern int WorkEvent21__10IWorkEventFv();
extern int WorkEvent22__10IWorkEventFv();
extern int WorkEvent23__10IWorkEventFv();
extern int WorkEvent24__10IWorkEventFv();
extern int WorkEvent25__10IWorkEventFv();
extern int WorkEvent26__10IWorkEventFv();
extern int WorkEvent27__10IWorkEventFv();
extern int WorkEvent28__10IWorkEventFv();
extern int WorkEvent29__10IWorkEventFv();
extern int WorkEvent30__10IWorkEventFv();
extern void WorkEvent31__10IWorkEventFv();
extern void wkUpdate__11CWorkThreadFv();
extern void wkRender__11CWorkThreadFv();
extern void wkRenderAfter__11CWorkThreadFv();
extern void wkStandbyExceptionRetry__11CWorkThreadFUl(unsigned int);
extern void wkStandbyLogin__9CViewRootFv();
extern void wkStandbyLogout__9CViewRootFv();
extern void __dt__11CWorkThreadFv(void*, int);
extern void* __dt__9CViewRootFv(void*, int);
extern void __dl__FPv(void*);
extern void __dla__FPv(void*);
}

// === Blob monolibdata1/1d dissolve: CViewRoot.cpp owns .rodata 0x80522660-
// 0x80522678, .sdata 0x806635A8-0x806635B0, .data 0x8056B710-0x8056B7C8,
// .sbss 0x806655D0-0x806655D8. ===
// RTTI name strings (.rodata): two 12-byte "CViewRoot" objects at 0x80522660 /
// 0x8052266C (lbl_eu_8052266C is the create() view name). Declared with
// __declspec(align(4)) so MWCC packs them 4-aligned (a plain char[12] would
// get 16-byte 8-aligned slots and overshoot the retail 0x18 range).
__declspec(align(4)) const char lbl_eu_80522660[12] = {0x43,0x56,0x69,0x65,0x77,0x52,0x6F,0x6F,0x74,0x00,0x00,0x00};  /* "CViewRoot\0\0\0" */
__declspec(align(4)) const char lbl_eu_8052266C[12] = {0x43,0x56,0x69,0x65,0x77,0x52,0x6F,0x6F,0x74,0x00,0x00,0x00};  /* "CViewRoot\0\0\0" */
extern u32 lbl_eu_8056B7B0[6];  // class-info, defined below
// RTTI locator (.sdata, 8): { name, class-info }.
extern "C" u32 lbl_eu_806635A8[2] = { (u32)&lbl_eu_80522660, (u32)&lbl_eu_8056B7B0 };
// CViewRoot vtable (.data, 0xA0). Defined BEFORE the class-info so the .data
// emission order matches retail (vtable @0x8056B710, class-info @0x8056B7B0).
u32 lbl_eu_8056B710[0xA0 / 4] = {
    (u32)&lbl_eu_806635A8, 0, (u32)&__dt__9CViewRootFv,
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
    (u32)&wkRenderAfter__11CWorkThreadFv, (u32)&wkStandbyLogin__9CViewRootFv,
    (u32)&wkStandbyLogout__9CViewRootFv, (u32)&wkStandbyExceptionRetry__11CWorkThreadFUl,
};
// CViewRoot class-info (.data, 0x18): [RTTI(IWorkEvent), 0, RTTI(CWorkThread),
// 0, 0, 0].
u32 lbl_eu_8056B7B0[6] = {
    (u32)&__RTTI__10IWorkEvent, 0, (u32)&__RTTI__11CWorkThread, 0, 0, 0,
};

// sbss data owned by this TU (blob monolibdata1d dissolve):
//   lbl_eu_806655D0 (4 bytes) = current root view pointer
//   lbl_eu_806655D4 (1 byte)
//   lbl_eu_806655D5/806655D6/806655D7 (3 bytes) = retail zero-fill pad to the
//   0x806655D8 boundary (the 8-byte retail .sbss slot ends there).
CViewRoot* lbl_eu_806655D0;
u8 lbl_eu_806655D4;
u8 lbl_eu_806655D5;
u8 lbl_eu_806655D6;
u8 lbl_eu_806655D7;

// NOTE: no CViewRoot ctor is defined here -- the retail split has no
// __ct__9CViewRoot symbol (create() constructs the object manually and the
// member-init list never runs the reslist<WORK_ID> default ctor, which would
// pull the reslist<_reslist_base<Ul>> vtables/RTTI into this TU's data
// sections; the retail linker GC'd those weak emissions).
//
// The destructor follows the freestanding-D1 recipe (MWCC_CASES:
// CMenuBattleDamage / CDeviceFontLoader): writing it as a C++ member dtor
// makes MWCC append the implicit reslist<WORK_ID> member destruction AFTER
// the user body (duplicate block + wrong order vs retail's member-first
// layout). The freestanding form emits each sub-object cleanup exactly once,
// in retail order: history reslist, then pools pool2/pool1/pool0, then the
// CWorkThread base dtor.
extern "C" void* __dt__9CViewRootFv(void* selfPtr, int flag) {
    CViewRoot* self = (CViewRoot*)selfPtr;
    if (self != nullptr) {
        lbl_eu_806655D0 = nullptr;

        // Inlined _reslist_base<WORK_ID> dtor: retag the vtable, unlink every
        // node (clearing its mNext), close the sentinel ring, free the array.
        // Nested null-guards reproduce retail's double-beq D2-into-D1 shape.
        reslist<WORK_ID>* hist = &self->mViewHistory;
        if (hist != nullptr) {
            if (hist != nullptr) {
                *reinterpret_cast<void**>(hist) = lbl_eu_8056B298;
                _reslist_node<WORK_ID>* sentinel = hist->mStartNodePtr;
                _reslist_node<WORK_ID>* node = sentinel->mNext;
                while (node != hist->mStartNodePtr) {
                    _reslist_node<WORK_ID>* cur = node;
                    node = node->mNext;
                    cur->mNext = nullptr;
                }
                hist->mStartNodePtr->mNext = hist->mStartNodePtr;
                hist->mStartNodePtr->mPrev = hist->mStartNodePtr;
                if (hist->unk1C == false) {
                    DELETE_ARRAY(hist->mList);
                }
            }
        }

        // Reset the three render pools (retail order: pool2, pool1, pool0).
        if (&self->mPool2 != nullptr) {
            self->mPool2.mUsed = 0;
            self->mPool2.mList = nullptr;
        }
        if (&self->mPool1 != nullptr) {
            self->mPool1.mUsed = 0;
            self->mPool1.mList = nullptr;
        }
        if (&self->mPool0 != nullptr) {
            self->mPool0.mUsed = 0;
            self->mPool0.mList = nullptr;
        }

        __dt__11CWorkThreadFv(self, 0);
        if (flag > 0) {
            ::operator delete(self);
        }
    }
    return self;
}

// Retail emits the CViewRootPool destructor at the head of this TU's split
// (0x80444A1C). It resets the pool ring (mUsed/mList) and frees the object
// when the delete flag is set; the CViewRoot destructor inlines the same
// logic, which is why this symbol only appears once as an out-of-line copy.
extern "C" void* __dt__80442084(CViewRootPool* pThis, int flag) {
    if (pThis != nullptr) {
        pThis->mUsed = 0;
        pThis->mList = 0;

        if (flag > 0) {
            ::operator delete(pThis);
        }
    }
    return pThis;
}

CViewRoot* CViewRoot::getInstance() {
    return lbl_eu_806655D0;
}

CView* CViewRoot::getCurrent() {
    CViewRoot* root = getInstance();

    if(root == nullptr){
        return nullptr;
    }

    return root->mCurrentView;
}

bool CViewRoot::isCurrent(const CView* view) {
    CViewRoot* root = lbl_eu_806655D0;
    if (root == nullptr) {
        return false;
    }

    CView* current = root->mCurrentView;
    int result = 0;
    if (current != nullptr) {
        if (current == view) {
            result = 1;
        }
    }
    return (bool)result;
}

// Walk view's child tree looking for `current`. Retail unrolls three levels
// then recurses. Mid-level gates are lbl/current only (bne + found=0 + b);
// no child/grand null checks before descending.
bool CViewRoot::isCurrentChild(const CView* view, const CView* current) {
    _reslist_node<CWorkThread*>* node;
    _reslist_node<CWorkThread*>* gnode;
    _reslist_node<CWorkThread*>* hnode;
    CView* child;
    CView* grand;
    CView* great;
    int found;

    if (lbl_eu_806655D0 == nullptr) {
        return false;
    }
    if (current == nullptr) {
        return false;
    }

    node = view->mChildren.mStartNodePtr->mNext;
    while (node != view->mChildren.mStartNodePtr) {
        child = CView::convertToView(node->mItem);
        if (current == child) {
            return true;
        }

        if (lbl_eu_806655D0 == nullptr) {
            found = 0;
            goto after_l1;
        }
        if (current == nullptr) {
            found = 0;
            goto after_l1;
        }

        gnode = child->mChildren.mStartNodePtr->mNext;
        while (gnode != child->mChildren.mStartNodePtr) {
            grand = CView::convertToView(gnode->mItem);
            if (current == grand) {
                found = 1;
                goto after_l1;
            }

            if (lbl_eu_806655D0 == nullptr) {
                found = 0;
                goto after_l2;
            }
            if (current == nullptr) {
                found = 0;
                goto after_l2;
            }

            hnode = grand->mChildren.mStartNodePtr->mNext;
            while (hnode != grand->mChildren.mStartNodePtr) {
                great = CView::convertToView(hnode->mItem);
                if (current == great) {
                    found = 1;
                    goto after_l2;
                }
                if (isCurrentChild(great, current)) {
                    found = 1;
                    goto after_l2;
                }
                hnode = hnode->mNext;
            }
            found = 0;
        after_l2:
            if (found != 0) {
                found = 1;
                goto after_l1;
            }
            gnode = gnode->mNext;
        }
        found = 0;
    after_l1:
        if (found != 0) {
            return true;
        }
        node = node->mNext;
    }
    return false;
}

void CViewRoot::destroyProc(CProc* proc) {
    CViewRoot* root = getInstance();

    if(root == nullptr){
        return;
    }

    if(root->mAttachedProc1 != proc && root->mAttachedProc0 != proc){
        return;
    }

    root->mAttachedProc1 = nullptr;
    getInstance()->mAttachedProc0 = nullptr;
}

CView* CViewRoot::getView(WORK_ID id) {
    CViewRoot* root = lbl_eu_806655D0;

    if(root == nullptr){
        return nullptr;
    }

    _reslist_node<CWorkThread*>* sentinel = root->mChildren.mStartNodePtr;
    _reslist_node<CWorkThread*>* node = sentinel->mNext;
    CWorkThread* thread = nullptr;

    while(node != lbl_eu_806655D0->mChildren.mStartNodePtr){
        thread = CWorkUtil::getWorkThread(id);

        if(thread != nullptr){
            break;
        }

        node = node->mNext;
    }

    return CView::convertToView(thread);
}

bool CViewRoot::isInitialized() {
    // Mirror of the CWorkThread message ring (CMsgParam<8> at 0x80:
    // mArrayPtr/mFront/mSize/mCapacity land at 0x1A4/0x1A8/0x1AC/0x1B0,
    // entry stride 0x24).
    struct StatusEntry {
        u32 field_0x00;
        u8 pad[0x24 - 4];
    };
    struct StatusRing {
        u8 pad[0x1A4];
        StatusEntry* mEntries;
        u32 mHead;
        u32 mCount;
        u32 mCapacity;
    };

    int i;
    int foundIndex;
    const StatusRing* ring;

    ring = reinterpret_cast<const StatusRing*>(lbl_eu_806655D0);
    if (ring == nullptr) {
        return false;
    }

    // Inlined CMsgParam<8>::find(EVT_EXCEPTION) scan (same idiom as the
    // matched CLibHbm/CDeviceSC copies).
    bool exceptionOrFound;
    if ((((const CViewRoot*)ring)->mFlags & THREAD_FLAG_EXCEPTION) != 0) {
        exceptionOrFound = true;
    } else {
        for (i = 0; i < ring->mCount; i++) {
            if (ring->mEntries[(ring->mHead + i) % ring->mCapacity].field_0x00 == EVT_EXCEPTION) {
                foundIndex = i;
                goto done;
            }
        }
        foundIndex = -1;
    done:
        exceptionOrFound = foundIndex >= 0;
    }

    return !exceptionOrFound
        && (((const CViewRoot*)ring)->mState == THREAD_STATE_LOGIN
            || ((const CViewRoot*)ring)->mState == THREAD_STATE_RUN);
}

struct PoolPair { u32 w0; u32 w1; };

// Retail symbol is FPvPv but call sites pass three rect pointers (r3/r4/r5);
// defined under its retail mangled name so parameters land in r3/r4/r5.
// Three inline signed ring pushes (divw/mullw/subf shape); fields are always
// re-read through the freshly loaded instance so each used++ reads the
// reloaded instance, matching retail.
void func_80442B54__9CViewRootFPvPv(void* a, void* b, void* c) {
    CViewRoot* root;
    s32 sum;
    s32 q;
    u32* base;
    u32* dst;

    root = lbl_eu_806655D0;
    if (root == nullptr) {
        return;
    }

    // Per-push uniquely-named word pairs keep each live range short.
    {
        u32 w00;
        u32 w01;
        sum = *(s32*)&root->mPool0.mList + root->mPool0.mUsed;
        q = sum / root->mPool0.mCapacity;
        base = reinterpret_cast<u32*>(root->mPool0.mStartNodePtr);
        dst = base + (sum - q * root->mPool0.mCapacity) * 2;
        w01 = static_cast<PoolPair*>(a)->w1;
        w00 = static_cast<PoolPair*>(a)->w0;
        dst[0] = w00;
        dst[1] = w01;
    }
    root->mPool0.mUsed = root->mPool0.mUsed + 1;

    root = lbl_eu_806655D0;
    {
        u32 w10;
        u32 w11;
        sum = *(s32*)&root->mPool1.mList + root->mPool1.mUsed;
        q = sum / root->mPool1.mCapacity;
        base = reinterpret_cast<u32*>(root->mPool1.mStartNodePtr);
        dst = base + (sum - q * root->mPool1.mCapacity) * 2;
        w11 = static_cast<PoolPair*>(b)->w1;
        w10 = static_cast<PoolPair*>(b)->w0;
        dst[0] = w10;
        dst[1] = w11;
    }
    root->mPool1.mUsed = root->mPool1.mUsed + 1;

    root = lbl_eu_806655D0;
    root = lbl_eu_806655D0;
    {
        u32 w20;
        u32 w21;
        sum = *(s32*)&root->mPool2.mList + root->mPool2.mUsed;
        q = sum / root->mPool2.mCapacity;
        base = reinterpret_cast<u32*>(root->mPool2.mStartNodePtr);
        dst = base + (sum - q * root->mPool2.mCapacity) * 2;
        w21 = static_cast<PoolPair*>(c)->w1;
        w20 = static_cast<PoolPair*>(c)->w0;
        dst[0] = w20;
        dst[1] = w21;
    }
    root->mPool2.mUsed = root->mPool2.mUsed + 1;

    func_8044B298__8CGXCacheFv(CDeviceGX::getCacheInstance(), a, b, c);
}

static PoolPair* poolPairAt(CViewRootPool* pool, u32 logicalIndex) {
    PoolPair* base = reinterpret_cast<PoolPair*>(pool->mStartNodePtr);
    u32 slot = (*(u32*)&pool->mList + logicalIndex) % (u32)pool->mCapacity;
    return &base[slot];
}

void CViewRoot::func_80442C68() {
    s32 sum0;
    s32 slot0;
    s32 sum1;
    s32 slot1;
    s32 sum2;
    s32 slot2;

    // Each section uses its own singleton-pointer local so every live range
    // stays short (retail keeps these in volatile regs r4/r7).
    CViewRoot* rootA = lbl_eu_806655D0;
    if (rootA == nullptr) {
        return;
    }

    // Pop one entry off each pool ring.
    if (rootA->mPool0.mUsed != 0) {
        rootA->mPool0.mUsed = rootA->mPool0.mUsed - 1;
    }
    CViewRoot* rootB = lbl_eu_806655D0;
    if (rootB->mPool2.mUsed != 0) {
        rootB->mPool2.mUsed = rootB->mPool2.mUsed - 1;
    }
    CViewRoot* rootC = lbl_eu_806655D0;
    if (rootC->mPool1.mUsed != 0) {
        rootC->mPool1.mUsed = rootC->mPool1.mUsed - 1;
    }

    CViewRoot* root = lbl_eu_806655D0;
    if (root->mPool0.mUsed == 0) {
        return;
    }
    if (root->mPool1.mUsed == 0) {
        return;
    }
    if (root->mPool2.mUsed == 0) {
        return;
    }

    // Ring-slot addresses of the newest entry in each pool.
    sum0 = *(u32*)&root->mPool0.mList + root->mPool0.mUsed;
    slot0 = (sum0 - 1) % (s32)root->mPool0.mCapacity;
    sum1 = *(u32*)&root->mPool1.mList + root->mPool1.mUsed;
    slot1 = (sum1 - 1) % (s32)root->mPool1.mCapacity;
    sum2 = *(u32*)&root->mPool2.mList + root->mPool2.mUsed;
    slot2 = (sum2 - 1) % (s32)root->mPool2.mCapacity;

    func_8044B298__8CGXCacheFv(
        CDeviceGX::getCacheInstance(),
        &reinterpret_cast<PoolPair*>(root->mPool0.mStartNodePtr)[slot0],
        &reinterpret_cast<PoolPair*>(root->mPool1.mStartNodePtr)[slot1],
        &reinterpret_cast<PoolPair*>(root->mPool2.mStartNodePtr)[slot2]);
}

void CViewRoot::func_80442DA8() {
    s32 sum0;
    s32 slot0;
    s32 sum1;
    s32 slot1;
    s32 sum2;
    s32 slot2;
    CViewRoot* root;

    root = lbl_eu_806655D0;
    if (root == nullptr) {
        return;
    }
    if (*(volatile u32*)&root->mPool0.mUsed == 0) {
        return;
    }

    // Ring-slot addresses: (list + used - 1) % capacity * sizeof(Pair)
    // past each pool's node array start.
    sum0 = *(u32*)&root->mPool0.mList + root->mPool0.mUsed;
    slot0 = (sum0 - 1) % (s32)root->mPool0.mCapacity;
    sum1 = *(u32*)&root->mPool1.mList + root->mPool1.mUsed;
    slot1 = (sum1 - 1) % (s32)root->mPool1.mCapacity;
    sum2 = *(u32*)&root->mPool2.mList + root->mPool2.mUsed;
    slot2 = (sum2 - 1) % (s32)root->mPool2.mCapacity;

    func_8044B298__8CGXCacheFv(
        CDeviceGX::getCacheInstance(),
        &reinterpret_cast<PoolPair*>(root->mPool0.mStartNodePtr)[slot0],
        &reinterpret_cast<PoolPair*>(root->mPool1.mStartNodePtr)[slot1],
        &reinterpret_cast<PoolPair*>(root->mPool2.mStartNodePtr)[slot2]);
}

void CViewRoot::setCurrent(CView* view) {
    int length = 0;
    _reslist_node<WORK_ID>* volatile endCopy;
    _reslist_node<WORK_ID>* volatile endNode;
    _reslist_node<WORK_ID>* volatile beginSave;
    _reslist_node<WORK_ID>* volatile curNode;
    CViewRoot* root;
    _reslist_node<WORK_ID>* sentinel;
    _reslist_node<WORK_ID>* frontNode;
    _reslist_node<WORK_ID>* volatile node;
    CWorkThread* thread;
    CProc* proc;
    CProc* rootProc;
    WORK_ID workId;
    CViewRoot* stateRoot;
    CViewRoot* histRoot;
    _reslist_node<WORK_ID>* historySentinel;
    _reslist_node<WORK_ID>* historyNode;

    root = getInstance();

    if (getInstance() == nullptr) {
        return;
    }

    if ((view->mFlags & THREAD_FLAG_NO_EVENT) != 0) {
        return;
    }

    // Count the view's child-list entries (inlined reslist::size walk over
    // the unk238 POD twin of reslist<WORK_ID>). Retail spills the sentinel
    // and the first node twice each; volatile copies force those stores.
    sentinel = (_reslist_node<WORK_ID>*)view->unk238.mStartNodePtr;
    endCopy = sentinel;
    curNode = sentinel->mNext;
    endNode = sentinel;
    beginSave = curNode;

    while (curNode != endNode) {
        length++;
        curNode = curNode->mNext;
    }

    if (length == 0) {
        return;
    }

    if (getInstance()->mCurrentView == view) {
        return;
    }

    // Convert the first child entry to a CProc via the thread-type range
    // check (retail inlines convertToProc); pssGetRoot runs even on a miss.
    // Retail spills the front node twice (sp+8/sp+0x1c) around the mItem load.
    frontNode = sentinel->mNext;
    node = frontNode;
    thread = CWorkUtil::getWorkThread(frontNode->mItem);
    proc = CProc::convertToProc(thread);
    rootProc = CProc::pssGetRoot(proc);

    if (proc == nullptr) {
        return;
    }

    stateRoot = lbl_eu_806655D0;

    // Early-out only when the previous attach state is fully live and clean.
    if (stateRoot->mCurrentView != nullptr && stateRoot->mAttachedProc0 != nullptr &&
        stateRoot->mAttachedProc1 == rootProc && stateRoot->mAttachedProc0->unk1E8 == 0 &&
        proc->unk1E8 != 0) {
        return;
    }

    stateRoot->mCurrentView = view;
    getInstance()->mAttachedProc1 = rootProc;
    getInstance()->mAttachedProc0 = proc;

    workId = view->mWorkID;
    histRoot = lbl_eu_806655D0;
    historySentinel = histRoot->mViewHistory.mStartNodePtr;
    historyNode = historySentinel->mNext;

    while (historyNode != historySentinel && workId != historyNode->mItem) {
        historyNode = historyNode->mNext;
    }

    if (historyNode != historySentinel) {
        return;
    }

    // Inlined reslist<WORK_ID>::push_back(workId) exactly as retail expands
    // it: free-slot scan, setItem's try/catch guarded item store (which emits
    // the exception-frame sp-save), then the splice onto the sentinel.
    _reslist_node<WORK_ID>* startNode = histRoot->mViewHistory.mStartNodePtr;
    _reslist_node<WORK_ID>* temp;
    int capacity = histRoot->mViewHistory.mCapacity;
    int i = 0;

    // The scan reuses the count variable (retail colors them as one web).
    length = 0;

    goto queue_check;
queue_body:
    if (*(u32*)((u8*)histRoot->mViewHistory.mList + length) == 0) {
        goto queue_found;
    }
    length += 0xc;
    i++;
queue_check:
    if (i < capacity) {
        goto queue_body;
    }
queue_found:
    temp = (_reslist_node<WORK_ID>*)((u8*)histRoot->mViewHistory.mList + i * 0xc);
    {
        WORK_ID* ptr = &temp->mItem;
        if (ptr != 0) {
            try {
                *ptr = workId;
            } catch (...) {
                throw;
            }
        }
    }
    temp->mNext = startNode;
    temp->mPrev = startNode->mPrev;
    startNode->mPrev->mNext = temp;
    startNode->mPrev = temp;
}
// Remove the current view's history entry, then promote the history entry
// whose view has the lowest unk460 value to be the new current view. The
// per-entry view lookup is CViewRoot::getView inlined by retail (children
// scan calling CWorkUtil::getWorkThread until it hits, then convertToView).
// workId is assigned before the history-sentinel load so MWCC colors it
// ahead of the walk node (retail: id=r3, sentinel=r4, node=r5).
void CViewRoot::invalidCurrent(CView* view) {
    WORK_ID id;
    u32 count;
    _reslist_node<WORK_ID>* sentinel;
    _reslist_node<WORK_ID>* node;
    _reslist_node<WORK_ID>* prev;
    _reslist_node<WORK_ID>* next;
    _reslist_node<WORK_ID>* best;
    CView* curView;
    CView* bestView;

    if (lbl_eu_806655D0 == nullptr) {
        return;
    }

    // Find-and-unlink (retail stops at the first match, unlike reslist::remove).
    id = view->mWorkID;
    sentinel = lbl_eu_806655D0->mViewHistory.mStartNodePtr;
    node = sentinel->mNext;
    while (node != sentinel && id != node->mItem) {
        node = node->mNext;
    }
    if (node != sentinel) {
        // Unlink: prev<->next splice, then clear the removed node's mNext.
        prev = node->mPrev;
        next = node->mNext;
        prev->mNext = next;
        next->mPrev = prev;
        node->mNext = nullptr;
    }

    // Count remaining entries. Retail holds the singleton in a register
    // across this whole stretch (count loop, empty-store, best init), then
    // switches to per-iteration singleton reloads inside the selection walk.
    CViewRoot* root = lbl_eu_806655D0;
    count = 0;
    node = root->mViewHistory.mStartNodePtr->mNext;
    while (node != root->mViewHistory.mStartNodePtr) {
        node = node->mNext;
        count++;
    }
    if (count == 0) {
        root->mCurrentView = nullptr;
    } else {
        // Promote the entry whose view has the lowest unk460 value.
        best = root->mViewHistory.mStartNodePtr->mNext;
        node = best;
        while (node != lbl_eu_806655D0->mViewHistory.mStartNodePtr) {
            curView = getView((WORK_ID)node->mItem);
            bestView = getView((WORK_ID)best->mItem);
            if ((s32)bestView->unk460 >= (s32)curView->unk460) {
                best = node;
            }
            node = node->mNext;
        }

        lbl_eu_806655D0->mCurrentView = getView((WORK_ID)best->mItem);
    }
}

CView* CViewRoot::getFullScreenView() {
    s16 viewWidth;
    s16 viewHeight;
    CView* childView;
    _reslist_node<CWorkThread*>* walkNode;
    int foundIndex;

    int i;

    u32 viewFlags;
    u32 msgQualified;
    u32 keepGoing;
    bool loginRunKeep;
    int rState;
    ml::CRect16 frameOffset;
    s16 posSumY;
    s16 posSumX;
    GXRenderModeObj* renderMode;
    CView* desktopView;

    // Inlined CMsgParam<8>::find(EVT_EXCEPTION) scan over the child's
    // message ring; overlay is based at &mMsgQueue (0x80), so the entry
    // array lands at +0x124 (= absolute 0x1A4).
    struct StatusEntry {
        u32 field_0x00;
        u8 pad[0x24 - 4];
    };
    struct StatusRing {
        u8 pad[0x124];
        StatusEntry* mEntries;
        u32 mFront;
        u32 mCount;
        u32 mCapacity;
    };

    if (lbl_eu_806655D0 == nullptr) {
        return nullptr;
    }

    desktopView = getView__8CDesktopFv();

    if (desktopView == nullptr) {
        return nullptr;
    }

    walkNode = desktopView->mChildren.mStartNodePtr->mNext;

    while (walkNode != desktopView->mChildren.mStartNodePtr) {
        childView = CView::convertToView((CWorkThread*)walkNode->mItem);

        // One flags load into viewFlags (retail r7) for EXCEPTION + later NO_EVENT.
        viewFlags = childView->mFlags;
        if (viewFlags & THREAD_FLAG_EXCEPTION) {
            msgQualified = true;
        } else {
            const StatusRing* ring =
                reinterpret_cast<const StatusRing*>(&childView->mMsgQueue);
            for (i = 0; i < ring->mCount; i++) {
                if (ring->mEntries[(ring->mFront + i) % ring->mCapacity].field_0x00 ==
                    EVT_EXCEPTION) {
                    foundIndex = i;
                    goto done_find;
                }
            }
            foundIndex = -1;
        done_find:
            msgQualified = (foundIndex >= 0);
        }

        // keepGoing=0 before the branch: retail cmpwi/li/bne shape.
        keepGoing = 0;
        if (msgQualified != 0) {
        } else {
            loginRunKeep = 1;
            rState = childView->mState;
            if (rState == THREAD_STATE_LOGIN) {
            } else if (rState == THREAD_STATE_RUN) {
            } else {
                loginRunKeep = 0;
            }
            if (loginRunKeep != 0) {
                keepGoing = 1;
            }
        }
        if (keepGoing == 0) {
            goto getFullScreenView_next;
        }

        if ((viewFlags & THREAD_FLAG_NO_EVENT) != 0) {
            goto getFullScreenView_next;
        }

        if ((childView->unk278 & 1) != 0) {
            return childView;
        }

        getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(&frameOffset, &childView->mFrame);

        posSumY = childView->mFrame.mContentX + frameOffset.mPos.x;
        posSumX = childView->mFrame.mContentY + frameOffset.mPos.y;
        viewWidth = childView->mRectData.mViewSize.x;
        viewHeight = childView->mRectData.mViewSize.y;

        if ((childView->unk278 & 4) != 0) {
            goto getFullScreenView_next;
        }

        if (posSumY != 0) {
            goto getFullScreenView_next;
        }

        if (posSumX != 0) {
            goto getFullScreenView_next;
        }

        renderMode = getRenderModeObj__9CDeviceVIFv();

        if (viewWidth != renderMode->fbWidth) {
            goto getFullScreenView_next;
        }

        renderMode = getRenderModeObj__9CDeviceVIFv();

        if (viewHeight != renderMode->efbHeight) {
            goto getFullScreenView_next;
        }

        return childView;

getFullScreenView_next:
        walkNode = walkNode->mNext;
    }

    return desktopView;
}

void CViewRoot::renderView() {
    if (lbl_eu_806655D0 == nullptr) {
        return;
    }

    if (lbl_eu_806655D4 != 0) {
        return;
    }

    if ((lbl_eu_806655D0->mFlags & THREAD_FLAG_NO_EVENT) != 0) {
        return;
    }

    if (getInstance__8CDesktopFv() == nullptr) {
        return;
    }

    _reslist_node<CWorkThread*>* walkNode = lbl_eu_806655D0->mChildren.mStartNodePtr->mNext;

    // Inlined CMsgParam<8>::find(EVT_EXCEPTION) scan over the child's message
    // ring (same overlay idiom as getFullScreenView: entries at +0x124 within
    // mMsgQueue, stride 0x24).
    struct StatusEntry {
        u32 field_0x00;
        u8 pad[0x24 - 4];
    };
    struct StatusRing {
        u8 pad[0x124];
        StatusEntry* mEntries;
        u32 mFront;
        u32 mCount;
        u32 mCapacity;
    };

    while (walkNode != lbl_eu_806655D0->mChildren.mStartNodePtr) {
        CView* childView = CView::convertToView(walkNode->mItem);

        // Inlined isException(): EXCEPTION flag or an EVT_EXCEPTION message.
        int exception;
        if ((childView->mFlags & THREAD_FLAG_EXCEPTION) != 0) {
            exception = 1;
        } else {
            const StatusRing* ring = reinterpret_cast<const StatusRing*>(&childView->mMsgQueue);
            int i;
            int foundIndex;
            for (i = 0; i < ring->mCount; i++) {
                if (ring->mEntries[(ring->mFront + i) % ring->mCapacity].field_0x00 ==
                    EVT_EXCEPTION) {
                    foundIndex = i;
                    goto done_find;
                }
            }
            foundIndex = -1;
        done_find:
            exception = (foundIndex >= 0);
        }

        // Render non-exceptional children that are logged-in/running.
        int doRender =
            exception == 0 &&
            (childView->mState == THREAD_STATE_LOGIN || childView->mState == THREAD_STATE_RUN);

        if (doRender != 0) {
            childView->renderView();
        }
        walkNode = walkNode->mNext;
    }
}

CViewRoot* CViewRoot::create(CWorkThread* pParent) {
    const char* name;
    CViewRoot* root;
    u32 historyIndex;
    u32 historyNode;

    name = lbl_eu_8052266C;
    root = (CViewRoot*)allocate__Q23mtl10MemManagerFUlUl(
        0x520, getWorkMem__17CWorkThreadSystemFv());

    if (root == nullptr) {
        goto create_entry_work;
    }

    __ct__11CWorkThreadFPCcP11CWorkThreadi(root, name, pParent, 0x80);

    // No retail CViewRoot ctor exists: lay the subobject down manually,
    // following the retail store order (pools, then the history reslist).
    *(void**)root = lbl_eu_8056B710;

    root->mPool0.mCapacity = 0x20;
    _reslist_node<CWorkThread*>* sentinel0 = &root->mPool0.mSentinel;
    _reslist_node<CWorkThread*>* sentinel1 = &root->mPool1.mSentinel;
    root->mPool0.mStartNodePtr = sentinel0;
    _reslist_node<CWorkThread*>* sentinel2 = &root->mPool2.mSentinel;
    _reslist_node<WORK_ID>* histSentinel = &root->mViewHistory.mStartNode;
    root->mPool0.mUsed = 0;
    root->mPool0.mList = NULL;

    root->mPool1.mCapacity = 0x20;
    root->mPool1.mStartNodePtr = sentinel1;
    root->mPool1.mUsed = 0;
    root->mPool1.mList = NULL;

    root->mPool2.mCapacity = 0x20;
    root->mPool2.mStartNodePtr = sentinel2;
    root->mPool2.mUsed = 0;
    root->mPool2.mList = NULL;

    // History reslist gets the transient _reslist_base vtable first, then
    // the final reslist<WORK_ID> vtable once the sentinel ring is closed.
    *reinterpret_cast<void**>(&root->mViewHistory) = lbl_eu_8056B298;
    root->mViewHistory.mList = NULL;
    root->mViewHistory.mCapacity = 0;
    root->mViewHistory.unk1C = false;
    root->mViewHistory.mStartNodePtr = &root->mViewHistory.mStartNode;
    root->mViewHistory.mStartNodePtr->mNext = root->mViewHistory.mStartNodePtr;
    root->mViewHistory.mStartNodePtr->mPrev = root->mViewHistory.mStartNodePtr;
    *reinterpret_cast<void**>(&root->mViewHistory) = lbl_eu_8056B280;

    root->mCurrentView = NULL;
    root->mAttachedProc0 = NULL;
    root->mAttachedProc1 = NULL;

    lbl_eu_806655D0 = root;
    root->mType = THREAD_CVIEWROOT;

    // Inlined reslist<WORK_ID>::reserve: array-new the node pool, clear every
    // node's mNext link, then publish the capacity.
    root->mViewHistory.mList =
        (_reslist_node<WORK_ID>*)allocate_array__Q23mtl10MemManagerFUlUl(0x600, root->mAllocHandle);
    for (historyIndex = 0; historyIndex < 8; historyIndex++) {
        for (historyNode = 0; historyNode < 16; historyNode++) {
            root->mViewHistory.mList[historyIndex * 16 + historyNode].mNext = NULL;
        }
    }
    root->mViewHistory.mCapacity = 0x80;
    lbl_eu_806655D4 = 0;

create_entry_work:
    entryWork__9CWorkUtilFP11CWorkThreadP11CWorkThreadb(root, pParent, false);
    root->func_804385CC(0);

    return root;
}

bool CViewRoot::wkStandbyLogin() {
    s16 height;
    s16 width;
    ml::CRect16 rect1;
    ml::CRect16 rect2;
    s32 sum;
    s32 q;
    PoolPair* dst;
    u32 w1;
    u32 w0;
    CViewRoot* root;

    // Positive-form guard: both tests branch-equal to the single failure
    // tail, matching retail's two beq -> li r3,0 layout.
    if (CDevice::isAllReady() && CLib::isInitialized()) {
    height = getRenderModeObj__9CDeviceVIFv()->efbHeight;
    width = getRenderModeObj__9CDeviceVIFv()->fbWidth;

    // Retail loads the root pointer before emitting the rect stores.
    root = lbl_eu_806655D0;

    // Retail emits rect1's size.x store before its pos stores.
    rect1.mSize.x = width;
    rect1.mPos.x = 0;
    rect1.mPos.y = 0;
    rect1.mSize.y = height;

    rect2.mPos.x = 0;
    rect2.mPos.y = 0;
    rect2.mSize.x = width;
    rect2.mSize.y = height;

    if (root != nullptr) {
        // Pool0 and pool1 both receive rect1; pool2 receives rect2.
        // Signed sum/quotient decomposition gives retail's divw/mullw/subf.
        sum = *(s32*)&root->mPool0.mList + root->mPool0.mUsed;
        q = sum / root->mPool0.mCapacity;
        dst = reinterpret_cast<PoolPair*>(root->mPool0.mStartNodePtr) +
              (sum - q * root->mPool0.mCapacity);
        w1 = *(u32*)&rect1.mSize;
        w0 = *(u32*)&rect1.mPos;
        dst[0].w0 = w0;
        dst[0].w1 = w1;
        root->mPool0.mUsed = root->mPool0.mUsed + 1;

        root = lbl_eu_806655D0;
        sum = *(s32*)&root->mPool1.mList + root->mPool1.mUsed;
        q = sum / root->mPool1.mCapacity;
        dst = reinterpret_cast<PoolPair*>(root->mPool1.mStartNodePtr) +
              (sum - q * root->mPool1.mCapacity);
        w1 = *(u32*)&rect1.mSize;
        w0 = *(u32*)&rect1.mPos;
        dst[0].w0 = w0;
        dst[0].w1 = w1;
        root->mPool1.mUsed = root->mPool1.mUsed + 1;

        root = lbl_eu_806655D0;
        sum = *(s32*)&root->mPool2.mList + root->mPool2.mUsed;
        q = sum / root->mPool2.mCapacity;
        dst = reinterpret_cast<PoolPair*>(root->mPool2.mStartNodePtr) +
              (sum - q * root->mPool2.mCapacity);
        w1 = *(u32*)&rect2.mSize;
        w0 = *(u32*)&rect2.mPos;
        dst[0].w0 = w0;
        dst[0].w1 = w1;
        root->mPool2.mUsed = root->mPool2.mUsed + 1;

        func_8044B298__8CGXCacheFv(CDeviceGX::getCacheInstance(), &rect1, &rect1, &rect2);
    }

    return CWorkThread::wkStandbyLogin();
    }

    return false;
}

bool CViewRoot::wkStandbyLogout() {
    // Combined guard: both failure paths fall into one trailing 'return false'
    // block (retail's shared li r3,0 tail).
    if (mChildren.empty() && CProcRoot::getInstance() == nullptr) {
        CViewRoot* root = lbl_eu_806655D0;
        if (root != nullptr) {
            // Pop one entry off each pool ring (order: pool0, pool2, pool1);
            // the singleton pointer is reloaded before every block and each
            // mUsed access is re-read (retail lwz/cmpwi/beq + lwz/subi/stw).
            if (*(volatile u32*)&root->mPool0.mUsed != 0) {
                *(volatile u32*)&root->mPool0.mUsed = *(volatile u32*)&root->mPool0.mUsed - 1;
            }
            root = lbl_eu_806655D0;
            if (*(volatile u32*)&root->mPool2.mUsed != 0) {
                *(volatile u32*)&root->mPool2.mUsed = *(volatile u32*)&root->mPool2.mUsed - 1;
            }
            root = lbl_eu_806655D0;
            if (*(volatile u32*)&root->mPool1.mUsed != 0) {
                *(volatile u32*)&root->mPool1.mUsed = *(volatile u32*)&root->mPool1.mUsed - 1;
            }
            root = lbl_eu_806655D0;
            if (*(volatile u32*)&root->mPool0.mUsed == 0 ||
                *(volatile u32*)&root->mPool1.mUsed == 0 ||
                *(volatile u32*)&root->mPool2.mUsed == 0) {
                goto wkStandbyLogout_base;
            }

            // Ring-slot addresses of the newest entry in each pool
            // ((list + used - 1) % capacity), computed inline in the call.
            func_8044B298__8CGXCacheFv(
                CDeviceGX::getCacheInstance(),
                &reinterpret_cast<PoolPair*>(root->mPool0.mStartNodePtr)[(*(s32*)&root->mPool0.mList + (s32)root->mPool0.mUsed - 1) % (s32)root->mPool0.mCapacity],
                &reinterpret_cast<PoolPair*>(root->mPool1.mStartNodePtr)[(*(s32*)&root->mPool1.mList + (s32)root->mPool1.mUsed - 1) % (s32)root->mPool1.mCapacity],
                &reinterpret_cast<PoolPair*>(root->mPool2.mStartNodePtr)[(*(s32*)&root->mPool2.mList + (s32)root->mPool2.mUsed - 1) % (s32)root->mPool2.mCapacity]);
        }

wkStandbyLogout_base:
        return CWorkThread::wkStandbyLogout();
    }

    return false;
}


// Retail emits the out-of-line copy of the header-inline CView::convertToView
// in this TU (0x804452B0); call sites keep inlining the class-body version.
extern "C" CView* convertToView__5CViewFP11CWorkThread(CWorkThread* pThread) {
    if (pThread == nullptr) {
        return nullptr;
    }

    int type = pThread->mType;

    // Check if the thread's type is in the CView range.
    if (CWorkThread::THREAD_CVIEW > type || type >= CWorkThread::THREAD_CVIEW_MAX) {
        return nullptr;
    }
    return static_cast<CView*>(pThread);
}

// Walk up view's parent chain (retail unrolls the walk 3x then recurses) to
// find a "root" view that is neither the CViewRoot singleton nor the desktop.
// Level 0 tests the raw mParent; each later level converts the candidate
// through the inlined CView::convertToView range check first.
// Walk up view's parent chain (retail unrolls the walk 3x then recurses) to
// find a "root" view that is neither the CViewRoot singleton nor the desktop.
// Level 0 tests the raw mParent; each later level converts the candidate
// through the inlined CView::convertToView range check first. Keeping the
// preamble reads in a named `parent` local while re-reading self->mParent
// bare at the conversion reproduces retail's cached-load/post-call-reload
// pairing (a fully-bare form over-CSEs into a single load).
// KNOWN GAP vs retail: MWCC colors {param-copy, parent} as {r31, r30}
// (retail: {r30, r31}) and stages iteration null-returns in r3 instead of
// r31; the final convertToView/recursion pair is also inlined where retail
// keeps out-of-line bl calls.
extern "C" CView* getRootView__9CViewRootFP5CView(CView* self) {
    CWorkThread* parent;
    CView* cur;
    CView* cand;
    CViewRoot* root = lbl_eu_806655D0;

    if (root == nullptr) {
        return nullptr;
    }
    parent = self->mParent;
    if (root == (CViewRoot*)parent) {
        return nullptr;
    }
    if (getView__8CDesktopFv() == (CView*)parent) {
        return self;
    }

    cur = CView::convertToView(self->mParent);

    if (lbl_eu_806655D0 == nullptr) {
        return nullptr;
    }
    if (lbl_eu_806655D0 == (CViewRoot*)(cur->mParent)) {
        return nullptr;
    }
    if (getView__8CDesktopFv() == (CView*)(cur->mParent)) {
        return cur;
    }
    cand = CView::convertToView(cur->mParent);

    if (lbl_eu_806655D0 == nullptr) {
        return nullptr;
    }
    if (lbl_eu_806655D0 == (CViewRoot*)(cand->mParent)) {
        return nullptr;
    }
    if (getView__8CDesktopFv() == (CView*)(cand->mParent)) {
        return cand;
    }
    cur = cand;
    cand = CView::convertToView(cand->mParent);

    if (lbl_eu_806655D0 == nullptr) {
        return nullptr;
    }
    if (lbl_eu_806655D0 == (CViewRoot*)(cand->mParent)) {
        return nullptr;
    }
    if (getView__8CDesktopFv() == (CView*)(cand->mParent)) {
        return cand;
    }
    cur = cand;

    return getRootView__9CViewRootFP5CView(
        (CView*)convertToView__5CViewFP11CWorkThread(cur->mParent));
}
