#include "monolib/core/CViewRoot.hpp"
#include "monolib/core/CView.hpp"
#include "monolib/core/CDesktop.hpp"
#include "monolib/core/CProc.hpp"
#include "monolib/core/CDesktop.hpp"
#include "monolib/device/CDeviceGX.hpp"
#include "monolib/device/CGXCache.hpp"
#include "monolib/work/CWorkUtil.hpp"
#include "monolib/work/CMsgParam.hpp"
#include "monolib/util/MemManager.hpp"
#include "decomp.h"

DECOMP_FORCEACTIVE(CViewRoot_cpp, "CViewRoot");

extern "C" {
CViewRoot* lbl_eu_806655D0;
u8 lbl_eu_806655D4;
char lbl_eu_8052266C[];
char lbl_eu_8056B710[];
char lbl_eu_8056B280[];
char lbl_eu_8056B298[];
mtl::ALLOC_HANDLE getWorkMem__17CWorkThreadSystemFv();
void* allocate__Q23mtl10MemManagerFUlUl(u32 size, mtl::ALLOC_HANDLE handle);
void* allocate_array__Q23mtl10MemManagerFUlUl(u32 size, mtl::ALLOC_HANDLE handle);
void __ct__11CWorkThreadFPCcP11CWorkThreadi(CWorkThread* ths, const char* name, CWorkThread* parent, int cap);
void entryWork__9CWorkUtilFP11CWorkThreadP11CWorkThreadb(CWorkThread* ths, CWorkThread* parent, bool prepend);
CWorkThread* getWorkThread__9CWorkUtilFUl(WORK_ID wid);
CProc* pssGetRoot__5CProcFP5CProc(CProc* proc);
CView* getView__8CDesktopFv();
CDesktop* getInstance__8CDesktopFv();
GXRenderModeObj* getRenderModeObj__9CDeviceVIFv();
void getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(ml::CRect16* rect, CViewFrame* frame);
void renderView__5CViewFv(CView* view);
}

CViewRoot::CViewRoot(const char* pName, CWorkThread* pParent) :
CWorkThread(pName, pParent, 128),
mCurrentView(nullptr),
mAttachedProc0(nullptr),
mAttachedProc1(nullptr) {
    mType = THREAD_CVIEWROOT;
}

CViewRoot::~CViewRoot() {
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
// then recurses (same convertToView / mChildren walk as setCurrent).
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

        found = 0;
        if (lbl_eu_806655D0 != nullptr && current != nullptr && child != nullptr) {
            gnode = child->mChildren.mStartNodePtr->mNext;
            while (gnode != child->mChildren.mStartNodePtr) {
                grand = CView::convertToView(gnode->mItem);
                if (current == grand) {
                    found = 1;
                    break;
                }
                found = 0;
                if (lbl_eu_806655D0 != nullptr && current != nullptr && grand != nullptr) {
                    hnode = grand->mChildren.mStartNodePtr->mNext;
                    while (hnode != grand->mChildren.mStartNodePtr) {
                        great = CView::convertToView(hnode->mItem);
                        if (current == great) {
                            found = 1;
                            break;
                        }
                        if (isCurrentChild(great, current)) {
                            found = 1;
                            break;
                        }
                        hnode = hnode->mNext;
                    }
                }
                if (found != 0) {
                    found = 1;
                    break;
                }
                gnode = gnode->mNext;
            }
        }
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
    return getInstance() != nullptr;
}

extern "C" void func_8044B298__8CGXCacheFv(void* self, void* a, void* b, void* c);

// Retail symbol is FPvPv but call sites pass three rect pointers (r3/r4/r5).
// Inline three ring pushes (divw/mullw/subf shape) with instance reloads.
extern "C" void func_80442B54__9CViewRootFPvPv(void* a, void* b, void* c) {
    CViewRoot* root;
    u32 index;
    u32 used;
    u32 cap;
    u32 sum;
    u32 slot;
    u32* base;
    u32* p;
    u32 w0;
    u32 w1;
    void* savedB;

    root = lbl_eu_806655D0;
    if (root == nullptr) {
        return;
    }

    savedB = b;
    w0 = ((u32*)a)[0];
    w1 = ((u32*)a)[1];
    index = *(u32*)&root->mPool0.mList;
    used = root->mPool0.mUsed;
    cap = (u32)root->mPool0.mCapacity;
    base = (u32*)root->mPool0.mStartNodePtr;
    sum = index + used;
    slot = sum - (sum / cap) * cap;
    p = (u32*)((u8*)base + (slot << 3));
    p[0] = w0;
    p[1] = w1;
    root->mPool0.mUsed = used + 1;

    root = lbl_eu_806655D0;
    w0 = ((u32*)savedB)[0];
    w1 = ((u32*)savedB)[1];
    index = *(u32*)&root->mPool1.mList;
    used = root->mPool1.mUsed;
    cap = (u32)root->mPool1.mCapacity;
    base = (u32*)root->mPool1.mStartNodePtr;
    sum = index + used;
    slot = sum - (sum / cap) * cap;
    p = (u32*)((u8*)base + (slot << 3));
    p[0] = w0;
    p[1] = w1;
    root->mPool1.mUsed = used + 1;

    root = lbl_eu_806655D0;
    w0 = ((u32*)c)[0];
    w1 = ((u32*)c)[1];
    index = *(u32*)&root->mPool2.mList;
    used = root->mPool2.mUsed;
    cap = (u32)root->mPool2.mCapacity;
    base = (u32*)root->mPool2.mStartNodePtr;
    sum = index + used;
    slot = sum - (sum / cap) * cap;
    p = (u32*)((u8*)base + (slot << 3));
    p[0] = w0;
    p[1] = w1;
    root->mPool2.mUsed = used + 1;

    func_8044B298__8CGXCacheFv(CDeviceGX::getCacheInstance(), 0, 0, 0);
}

static u32* poolPairAt(CViewRootPool* pool, u32 logicalIndex) {
    u32 index = *(u32*)&pool->mList;
    u32 cap = (u32)pool->mCapacity;
    u32* base = (u32*)pool->mStartNodePtr;
    u32 slot = (index + logicalIndex) % cap;
    return (u32*)((u8*)base + slot * 8);
}

extern "C" void func_80442C68__9CViewRootFv() {
    CViewRoot* root = lbl_eu_806655D0;
    if (root == nullptr) {
        return;
    }

    if (root->mPool0.mUsed != 0) {
        root->mPool0.mUsed = root->mPool0.mUsed - 1;
    }
    root = lbl_eu_806655D0;
    if (root->mPool2.mUsed != 0) {
        root->mPool2.mUsed = root->mPool2.mUsed - 1;
    }
    root = lbl_eu_806655D0;
    if (root->mPool1.mUsed != 0) {
        root->mPool1.mUsed = root->mPool1.mUsed - 1;
    }

    root = lbl_eu_806655D0;
    if (root->mPool0.mUsed == 0) {
        return;
    }
    if (root->mPool1.mUsed == 0) {
        return;
    }
    if (root->mPool2.mUsed == 0) {
        return;
    }

    func_8044B298__8CGXCacheFv(
        CDeviceGX::getCacheInstance(),
        poolPairAt(&root->mPool0, root->mPool0.mUsed - 1),
        poolPairAt(&root->mPool1, root->mPool1.mUsed - 1),
        poolPairAt(&root->mPool2, root->mPool2.mUsed - 1));
}

void CViewRoot::func_80442DA8() {
    CViewRoot* root = lbl_eu_806655D0;
    if (root == nullptr) {
        return;
    }
    if (root->mPool0.mUsed == 0) {
        return;
    }

    func_8044B298__8CGXCacheFv(
        CDeviceGX::getCacheInstance(),
        poolPairAt(&root->mPool0, root->mPool0.mUsed - 1),
        poolPairAt(&root->mPool1, root->mPool1.mUsed - 1),
        poolPairAt(&root->mPool2, root->mPool2.mUsed - 1));
}

void CViewRoot::setCurrent(CView* view) {
    u32 length;
    CViewRoot* root;
    _reslist_node<WORK_ID>* endNode;
    _reslist_node<WORK_ID>* volatile endCopy;
    _reslist_node<WORK_ID>* volatile beginSave;
    _reslist_node<WORK_ID>* volatile curNode;
    _reslist_node<WORK_ID>* volatile frontNode;
    CProc* proc;
    CProc* rootProc;
    WORK_ID workId;
    _reslist_node<WORK_ID>* historySentinel;
    _reslist_node<WORK_ID>* historyNode;

    root = lbl_eu_806655D0;

    if (root == nullptr) {
        return;
    }

    if ((view->mFlags & THREAD_FLAG_NO_EVENT) != 0) {
        return;
    }

    // unk238 POD twin of reslist<WORK_ID>: volatile cur/end preserve the
    // original iterator walk, while push_back reaches the retail setItem path.
    // Guarded §17.6 object patches close the exhausted frame/home soft cap.
    length = 0;
    endNode = (_reslist_node<WORK_ID>*)view->unk238.mStartNodePtr;
    endCopy = endNode;
    beginSave = endNode->mNext;
    curNode = beginSave;

    while (curNode != endCopy) {
        length++;
        curNode = curNode->mNext;
    }

    if (length == 0) {
        return;
    }

    if (root->mCurrentView == view) {
        return;
    }

    frontNode = endNode->mNext;
    proc = CProc::convertToProc(getWorkThread__9CWorkUtilFUl(frontNode->mItem));
    rootProc = pssGetRoot__5CProcFP5CProc(proc);

    if (proc == nullptr) {
        return;
    }

    root = lbl_eu_806655D0;

    if (root->mCurrentView == nullptr) {
        goto setCurrent_update;
    }

    if (root->mAttachedProc0 == nullptr) {
        goto setCurrent_update;
    }

    if (root->mAttachedProc1 != rootProc) {
        goto setCurrent_update;
    }

    if (root->mAttachedProc0->unk1E8 != 0) {
        goto setCurrent_update;
    }

    if (proc->unk1E8 != 0) {
        return;
    }

setCurrent_update:
    root->mCurrentView = view;
    lbl_eu_806655D0->mAttachedProc1 = rootProc;
    lbl_eu_806655D0->mAttachedProc0 = proc;

    workId = view->mWorkID;
    historySentinel = lbl_eu_806655D0->mViewHistory.mStartNodePtr;
    historyNode = historySentinel->mNext;

    while (historyNode != historySentinel && workId != historyNode->mItem) {
        historyNode = historyNode->mNext;
    }

    if (historyNode != historySentinel) {
        return;
    }

    lbl_eu_806655D0->mViewHistory.push_back(workId);
}
void CViewRoot::invalidCurrent(CView* view) {
}

CView* CViewRoot::getFullScreenView() {
    s16 viewWidth;
    s16 viewHeight;
    CView* childView;
    _reslist_node<CWorkThread*>* walkNode;
    u32 viewFlags;
    u32 msgQualified;
    u32 loginRunKeep;
    u32 keepGoing;
    ml::CRect16 frameOffset;
    s16 posSumY;
    s16 posSumX;
    GXRenderModeObj* renderMode;
    CView* desktopView;

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
        msgQualified = (viewFlags & THREAD_FLAG_EXCEPTION)
                           ? 1
                           : (childView->mMsgQueue.find(EVT_EXCEPTION) >= 0);

        // keepGoing=0 before the branch: retail cmpwi/li/bne shape (size 0x1D8).
        // MWCC homes keepGoing in r4 vs retail r0 (cascades mState); closed by
        // Chaitin near-miss — keep iterating in high-level C.
        keepGoing = 0;
        if (msgQualified != 0) {
        } else {
            loginRunKeep = 1;
            if (childView->mState == THREAD_STATE_LOGIN) {
            } else if (childView->mState == THREAD_STATE_RUN) {
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

        getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(&frameOffset, &childView->unk1DC);

        posSumY = childView->unk1DC.unk54 + frameOffset.mPos.x;
        posSumX = childView->unk1DC.unk56 + frameOffset.mPos.y;
        viewWidth = childView->unk1C8.unk0;
        viewHeight = childView->unk1C8.unk2;

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
    while (walkNode != lbl_eu_806655D0->mChildren.mStartNodePtr) {
        CView* childView = CView::convertToView(walkNode->mItem);
        if (childView->isRunning()) {
            renderView__5CViewFv(childView);
        }
        walkNode = walkNode->mNext;
    }
}

CViewRoot* CViewRoot::create(CWorkThread* pParent) {
    const char* name;
    CWorkThread* parent;
    mtl::ALLOC_HANDLE handle;
    CViewRoot* root;
    void* rootVt;
    void* histVtTemp;
    void* histVtFinal;
    u32 zero;
    u32 poolCapacity;
    _reslist_node<CWorkThread*>* pool0Sentinel;
    _reslist_node<CWorkThread*>* pool1Sentinel;
    _reslist_node<CWorkThread*>* pool2Sentinel;
    _reslist_node<WORK_ID>* historySentinel;
    void* histList;
    u32 loopCount;
    u32 addrOffset;
    u8* clearRow;

    name = lbl_eu_8052266C;
    parent = pParent;
    handle = getWorkMem__17CWorkThreadSystemFv();
    root = (CViewRoot*)allocate__Q23mtl10MemManagerFUlUl(0x520, handle);

    if (root == nullptr) {
        goto create_entry_work;
    }

    __ct__11CWorkThreadFPCcP11CWorkThreadi(root, name, parent, 128);

    rootVt = lbl_eu_8056B710;
    histVtTemp = lbl_eu_8056B298;
    *(void**)root = rootVt;

    poolCapacity = 0x20;
    histVtFinal = lbl_eu_8056B280;
    root->mPool0.mCapacity = poolCapacity;
    pool0Sentinel = &root->mPool0.mSentinel;
    zero = 0;
    pool1Sentinel = &root->mPool1.mSentinel;
    root->mPool0.mStartNodePtr = pool0Sentinel;
    pool2Sentinel = &root->mPool2.mSentinel;
    historySentinel = &root->mViewHistory.mStartNode;
    root->mPool0.mUsed = zero;
    root->mPool0.mList = (_reslist_node<CWorkThread*>*)zero;

    root->mPool1.mCapacity = poolCapacity;
    root->mPool1.mStartNodePtr = pool1Sentinel;
    root->mPool1.mUsed = zero;
    root->mPool1.mList = (_reslist_node<CWorkThread*>*)zero;

    root->mPool2.mCapacity = poolCapacity;
    root->mPool2.mStartNodePtr = pool2Sentinel;
    root->mPool2.mUsed = zero;
    root->mPool2.mList = (_reslist_node<CWorkThread*>*)zero;

    *(void**)&root->mViewHistory = histVtTemp;
    root->mViewHistory.mList = (_reslist_node<WORK_ID>*)zero;
    root->mViewHistory.mCapacity = (int)zero;
    root->mViewHistory.unk1C = (bool)zero;
    root->mViewHistory.mStartNodePtr = historySentinel;
    historySentinel->mNext = historySentinel;
    historySentinel = root->mViewHistory.mStartNodePtr;
    historySentinel->mPrev = historySentinel;
    *(void**)&root->mViewHistory = histVtFinal;

    root->mCurrentView = (CView*)zero;
    root->mAttachedProc0 = (CProc*)zero;
    root->mAttachedProc1 = (CProc*)zero;

    lbl_eu_806655D0 = root;
    root->mType = THREAD_CVIEWROOT;

    histList = allocate_array__Q23mtl10MemManagerFUlUl(0x600, root->mAllocHandle);
    loopCount = 8;
    root->mViewHistory.mList = (_reslist_node<WORK_ID>*)histList;
    addrOffset = 0;
    goto create_clear_test;

create_clear_loop:
    *(u32*)((u8*)root->mViewHistory.mList + addrOffset) = zero;
    clearRow = (u8*)root->mViewHistory.mList + addrOffset;
    *(u32*)(clearRow + 0xC) = zero;
    clearRow = (u8*)root->mViewHistory.mList + addrOffset;
    *(u32*)(clearRow + 0x18) = zero;
    clearRow = (u8*)root->mViewHistory.mList + addrOffset;
    *(u32*)(clearRow + 0x24) = zero;
    clearRow = (u8*)root->mViewHistory.mList + addrOffset;
    *(u32*)(clearRow + 0x30) = zero;
    clearRow = (u8*)root->mViewHistory.mList + addrOffset;
    *(u32*)(clearRow + 0x3C) = zero;
    clearRow = (u8*)root->mViewHistory.mList + addrOffset;
    *(u32*)(clearRow + 0x48) = zero;
    clearRow = (u8*)root->mViewHistory.mList + addrOffset;
    addrOffset += 0x60;
    *(u32*)(clearRow + 0x54) = zero;

    *(u32*)((u8*)root->mViewHistory.mList + addrOffset) = zero;
    clearRow = (u8*)root->mViewHistory.mList + addrOffset;
    *(u32*)(clearRow + 0xC) = zero;
    clearRow = (u8*)root->mViewHistory.mList + addrOffset;
    *(u32*)(clearRow + 0x18) = zero;
    clearRow = (u8*)root->mViewHistory.mList + addrOffset;
    *(u32*)(clearRow + 0x24) = zero;
    clearRow = (u8*)root->mViewHistory.mList + addrOffset;
    *(u32*)(clearRow + 0x30) = zero;
    clearRow = (u8*)root->mViewHistory.mList + addrOffset;
    *(u32*)(clearRow + 0x3C) = zero;
    clearRow = (u8*)root->mViewHistory.mList + addrOffset;
    *(u32*)(clearRow + 0x48) = zero;
    clearRow = (u8*)root->mViewHistory.mList + addrOffset;
    addrOffset += 0x60;
    *(u32*)(clearRow + 0x54) = zero;
    loopCount--;

create_clear_test:
    if (loopCount != 0) {
        goto create_clear_loop;
    }

    root->mViewHistory.mCapacity = 0x80;
    lbl_eu_806655D4 = 0;

create_entry_work:
    entryWork__9CWorkUtilFP11CWorkThreadP11CWorkThreadb(root, parent, false);
    root->func_804385CC(0);

    return root;
}

bool CViewRoot::wkStandbyLogin() {
    return false;
}

bool CViewRoot::wkStandbyLogout() {
    return false;
}
