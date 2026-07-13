#include "monolib/core/CViewRoot.hpp"
#include "monolib/core/CView.hpp"
#include "monolib/core/CDesktop.hpp"
#include "monolib/core/CProc.hpp"
#include "monolib/core/CDesktop.hpp"
#include "monolib/work/CWorkUtil.hpp"
#include "monolib/work/CMsgParam.hpp"
#include "monolib/util/MemManager.hpp"
#include "decomp.h"

DECOMP_FORCEACTIVE(CViewRoot_cpp, "CViewRoot");

extern "C" {
CViewRoot* lbl_eu_806655D0;
u8 lbl_eu_806655D4;
char lbl_eu_8052266C[];
void* lbl_eu_8056B710;
void* lbl_eu_8056B280;
void* lbl_eu_8056B298;
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
    CViewRoot* root = getInstance();

    if(root == nullptr){
        return nullptr;
    }

    _reslist_node<CWorkThread*>* sentinel = root->mChildren.mStartNodePtr;
    _reslist_node<CWorkThread*>* node = sentinel->mNext;
    CWorkThread* thread = nullptr;

    while(node != getInstance()->mChildren.mStartNodePtr){
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

void CViewRoot::func_80442DA8() {
}

void CViewRoot::setCurrent(CView* view) {
    CViewRoot* root;
    u32 renderWorkCount;
    _reslist_node<CWorkThread*>* walkSentinel;
    _reslist_node<CWorkThread*>* walkNode;
    WORK_ID renderWorkId;
    CWorkThread* renderThread;
    CProc* proc;
    CProc* rootProc;
    _reslist_node<WORK_ID>* historySentinel;
    _reslist_node<WORK_ID>* historyNode;
    u32 historyUsed;
    int historyIndex;
    _reslist_node<WORK_ID>* historyEntry;

    root = getInstance();

    if (root == nullptr) {
        return;
    }

    if ((view->mFlags & THREAD_FLAG_NO_EVENT) != 0) {
        return;
    }

    renderWorkCount = 0;
    walkSentinel = view->unk238.mStartNodePtr;
    walkNode = walkSentinel->mNext;

    while (walkNode != walkSentinel) {
        walkNode = walkNode->mNext;
        renderWorkCount++;
    }

    if (renderWorkCount == 0) {
        return;
    }

    if (root->mCurrentView == view) {
        return;
    }

    walkSentinel = view->unk238.mStartNodePtr;
    walkNode = walkSentinel->mNext;
    renderWorkId = *(WORK_ID*)&walkNode->mItem;
    renderThread = getWorkThread__9CWorkUtilFUl(renderWorkId);

    if (renderThread == nullptr) {
        proc = nullptr;
        goto setCurrent_after_proc;
    }

    if (renderThread->mType < THREAD_CPROC) {
        proc = nullptr;
        goto setCurrent_after_proc;
    }

    if (renderThread->mType >= THREAD_CPROC_MAX) {
        proc = nullptr;
        goto setCurrent_after_proc;
    }

    proc = static_cast<CProc*>(renderThread);

setCurrent_after_proc:
    rootProc = pssGetRoot__5CProcFP5CProc(proc);

    if (proc == nullptr) {
        return;
    }

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
    getInstance()->mAttachedProc1 = rootProc;
    getInstance()->mAttachedProc0 = proc;

    historySentinel = getInstance()->mViewHistory.mStartNodePtr;
    historyNode = historySentinel->mNext;

    while (historyNode != historySentinel) {
        if (historyNode->mItem == view->mWorkID) {
            return;
        }
        historyNode = historyNode->mNext;
    }

    historyUsed = 0;
    historyIndex = 0;

    while (historyUsed < (u32)getInstance()->mViewHistory.mCapacity) {
        if (getInstance()->mViewHistory.mList[historyIndex].mNext == nullptr) {
            goto setCurrent_push_slot;
        }
        historyIndex++;
        historyUsed++;
    }

setCurrent_push_slot:
    historyEntry = &getInstance()->mViewHistory.mList[historyIndex];
    historySentinel = getInstance()->mViewHistory.mStartNodePtr;
    historyEntry->mItem = view->mWorkID;
    historyEntry->mNext = historySentinel;
    historyEntry->mPrev = historySentinel->mPrev;
    historySentinel->mPrev->mNext = historyEntry;
    historySentinel->mPrev = historyEntry;
}

void CViewRoot::invalidCurrent(CView* view) {
}

CView* CViewRoot::getFullScreenView() {
    s16 viewWidth;
    s16 viewHeight;
    CView* childView;
    _reslist_node<CWorkThread*>* walkNode;
    u32 viewFlags;
    u32 msgIndex;
    u32 msgFront;
    u32 msgModulus;
    u32 msgRemainder;
    u32 msgEntryOffset;
    CMsgParamEntry* msgArray;
    u32 msgCount;
    u32 msgQualified;
    u32 loginRunKeep;
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

        viewFlags = childView->mFlags;

        if ((viewFlags & THREAD_FLAG_EXCEPTION) != 0) {
            msgQualified = 1;
            goto getFullScreenView_after_msg_eval;
        }

        msgCount = *(u32*)((u8*)childView + 0x1AC);

        for (msgIndex = 0; msgIndex < msgCount; msgIndex++) {
            msgFront = *(u32*)((u8*)childView + 0x1A8);
            msgModulus = *(u32*)((u8*)childView + 0x1B0);
            msgArray = *(CMsgParamEntry**)((u8*)childView + 0x1A4);
            msgRemainder = (msgFront + msgIndex) / msgModulus;
            msgRemainder = msgRemainder * msgModulus;
            msgRemainder = (msgFront + msgIndex) - msgRemainder;
            msgEntryOffset = msgRemainder * 0x24;

            if (*(u32*)((u8*)msgArray + msgEntryOffset) != 2) {
                continue;
            }

            goto getFullScreenView_msg_eval;
        }

        msgIndex = -1;

getFullScreenView_msg_eval:
        msgQualified = (msgIndex >> 31) ^ 1;

getFullScreenView_after_msg_eval:
        // Reuse msgQualified as keepGoing (retail: cmpwi r0 / li r0,0 / bne).
        if (msgQualified != 0) {
            msgQualified = 0;
            goto getFullScreenView_after_state;
        }

        loginRunKeep = 1;

        if (childView->mState == THREAD_STATE_LOGIN) {
            goto getFullScreenView_state_ready;
        }

        if (childView->mState == THREAD_STATE_RUN) {
            goto getFullScreenView_state_ready;
        }

        loginRunKeep = 0;

getFullScreenView_state_ready:
        if (loginRunKeep == 0) {
            goto getFullScreenView_after_state;
        }

        msgQualified = 1;

getFullScreenView_after_state:
        if (msgQualified == 0) {
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
    u32 shouldRender;
    u32 stateReady;
    CView* childView;
    _reslist_node<CWorkThread*>* walkNode;
    u32 viewFlags;
    u32 msgIndex;
    u32 msgFront;
    u32 msgModulus;
    u32 msgRemainder;
    u32 msgEntryOffset;
    CMsgParamEntry* msgArray;
    u32 msgCount;
    u32 msgCounter;
    u32 msgQualified;

    if (getInstance() == nullptr) {
        return;
    }

    if (lbl_eu_806655D4 != 0) {
        return;
    }

    if ((getInstance()->mFlags & THREAD_FLAG_NO_EVENT) != 0) {
        return;
    }

    if (getInstance__8CDesktopFv() == nullptr) {
        return;
    }

    walkNode = getInstance()->mChildren.mStartNodePtr->mNext;
    goto renderView_loop_check;

renderView_loop_body:
    childView = (CView*)walkNode->mItem;

    if (childView != nullptr) {
        if (childView->mType < THREAD_CVIEW) {
            childView = nullptr;
        } else if (childView->mType >= THREAD_CVIEW_MAX) {
            childView = nullptr;
        }
    }

    viewFlags = childView->mFlags;

    if ((viewFlags & THREAD_FLAG_EXCEPTION) != 0) {
        shouldRender = 1;
        goto renderView_qualified;
    }

    msgCount = *(u32*)((u8*)childView + 0x1AC);
    msgIndex = 0;

    if ((s32)msgCount <= 0) {
        msgIndex = -1;
        goto renderView_msg_eval;
    }

    msgCounter = msgCount;

    while (true) {
        msgFront = *(u32*)((u8*)childView + 0x1A8);
        msgModulus = *(u32*)((u8*)childView + 0x1B0);
        msgArray = *(CMsgParamEntry**)((u8*)childView + 0x1A4);
        msgRemainder = (msgFront + msgIndex) / msgModulus;
        msgRemainder = msgRemainder * msgModulus;
        msgRemainder = (msgFront + msgIndex) - msgRemainder;
        msgEntryOffset = msgRemainder * 0x24;

        if (*(u32*)((u8*)msgArray + msgEntryOffset) == 2) {
            goto renderView_msg_eval;
        }

        msgIndex++;
        msgCounter--;

        if (msgCounter != 0) {
            continue;
        }

        msgIndex = -1;
        goto renderView_msg_eval;
    }

renderView_msg_eval:
    msgQualified = (msgIndex >> 31) ^ 1;
    shouldRender = msgQualified;

renderView_qualified:
    if (shouldRender != 0) {
        shouldRender = 0;
        goto renderView_state_check_end;
    }

    stateReady = 1;

    if (childView->mState == THREAD_STATE_LOGIN) {
        goto renderView_state_ready;
    }

    if (childView->mState == THREAD_STATE_RUN) {
        goto renderView_state_ready;
    }

    stateReady = 0;

renderView_state_ready:
    if (stateReady == 0) {
        goto renderView_state_check_end;
    }

    shouldRender = 1;

renderView_state_check_end:
    if (shouldRender != 0) {
        renderView__5CViewFv(childView);
    }

    walkNode = walkNode->mNext;

renderView_loop_check:
    if (walkNode != getInstance()->mChildren.mStartNodePtr) {
        goto renderView_loop_body;
    }
}

CViewRoot* CViewRoot::create(CWorkThread* pParent) {
    const char* name;
    CWorkThread* parent;
    mtl::ALLOC_HANDLE handle;
    CViewRoot* root;
    u32 zero;
    u32 poolCapacity;
    _reslist_node<CWorkThread*>* pool0Sentinel;
    _reslist_node<CWorkThread*>* pool1Sentinel;
    _reslist_node<CWorkThread*>* pool2Sentinel;
    _reslist_node<WORK_ID>* historySentinel;
    u8* poolArray;
    u32 addrOffset;
    u32 loopCount;

    name = lbl_eu_8052266C;
    parent = pParent;
    handle = getWorkMem__17CWorkThreadSystemFv();
    root = (CViewRoot*)allocate__Q23mtl10MemManagerFUlUl(0x520, handle);

    if (root == nullptr) {
        goto create_entry_work;
    }

    __ct__11CWorkThreadFPCcP11CWorkThreadi(root, name, parent, 128);

    zero = 0;
    poolCapacity = 0x20;

    *(void**)root = lbl_eu_8056B710;

    pool0Sentinel = &root->mPool0.mSentinel;
    pool1Sentinel = &root->mPool1.mSentinel;
    pool2Sentinel = &root->mPool2.mSentinel;
    historySentinel = &root->mViewHistory.mStartNode;

    root->mPool0.mStartNodePtr = pool0Sentinel;
    root->mPool0.mCapacity = poolCapacity;
    root->mPool0.mList = nullptr;
    root->mPool0.mUsed = 0;

    root->mPool1.mStartNodePtr = pool1Sentinel;
    root->mPool1.mCapacity = poolCapacity;
    root->mPool1.mList = nullptr;
    root->mPool1.mUsed = 0;

    root->mPool2.mStartNodePtr = pool2Sentinel;
    root->mPool2.mCapacity = poolCapacity;
    root->mPool2.mList = nullptr;
    root->mPool2.mUsed = 0;

    *(void**)&root->mViewHistory = lbl_eu_8056B298;
    root->mViewHistory.mStartNodePtr = historySentinel;
    root->mViewHistory.mList = nullptr;
    root->mViewHistory.mCapacity = 0;
    root->mViewHistory.unk1C = 0;

    historySentinel->mNext = historySentinel;
    historySentinel = root->mViewHistory.mStartNodePtr;
    historySentinel->mPrev = historySentinel;

    *(void**)&root->mViewHistory = lbl_eu_8056B280;

    root->mCurrentView = nullptr;
    root->mAttachedProc0 = nullptr;
    root->mAttachedProc1 = nullptr;

    lbl_eu_806655D0 = root;
    root->mType = THREAD_CVIEWROOT;

    poolArray = (u8*)allocate_array__Q23mtl10MemManagerFUlUl(0x600, root->mAllocHandle);
    root->mViewHistory.mList = (_reslist_node<WORK_ID>*)poolArray;

    addrOffset = 0;
    loopCount = 8;
    goto create_clear_test;

create_clear_loop:
    poolArray = (u8*)root->mViewHistory.mList;
    *(u32*)&poolArray[addrOffset] = zero;
    poolArray = (u8*)root->mViewHistory.mList;
    *(u32*)&poolArray[addrOffset + 0xC] = zero;
    poolArray = (u8*)root->mViewHistory.mList;
    *(u32*)&poolArray[addrOffset + 0x18] = zero;
    poolArray = (u8*)root->mViewHistory.mList;
    *(u32*)&poolArray[addrOffset + 0x24] = zero;
    poolArray = (u8*)root->mViewHistory.mList;
    *(u32*)&poolArray[addrOffset + 0x30] = zero;
    poolArray = (u8*)root->mViewHistory.mList;
    *(u32*)&poolArray[addrOffset + 0x3C] = zero;
    poolArray = (u8*)root->mViewHistory.mList;
    *(u32*)&poolArray[addrOffset + 0x48] = zero;
    poolArray = (u8*)root->mViewHistory.mList;
    *(u32*)&poolArray[addrOffset + 0x54] = zero;
    addrOffset += 0x60;
    poolArray = (u8*)root->mViewHistory.mList;
    *(u32*)&poolArray[addrOffset] = zero;
    poolArray = (u8*)root->mViewHistory.mList;
    *(u32*)&poolArray[addrOffset + 0xC] = zero;
    poolArray = (u8*)root->mViewHistory.mList;
    *(u32*)&poolArray[addrOffset + 0x18] = zero;
    poolArray = (u8*)root->mViewHistory.mList;
    *(u32*)&poolArray[addrOffset + 0x24] = zero;
    poolArray = (u8*)root->mViewHistory.mList;
    *(u32*)&poolArray[addrOffset + 0x30] = zero;
    poolArray = (u8*)root->mViewHistory.mList;
    *(u32*)&poolArray[addrOffset + 0x3C] = zero;
    poolArray = (u8*)root->mViewHistory.mList;
    *(u32*)&poolArray[addrOffset + 0x48] = zero;
    poolArray = (u8*)root->mViewHistory.mList;
    *(u32*)&poolArray[addrOffset + 0x54] = zero;
    addrOffset += 0x60;
    loopCount--;

create_clear_test:
    if (loopCount != 0) {
        goto create_clear_loop;
    }

    root->mViewHistory.mCapacity = 0x80;
    lbl_eu_806655D4 = 0;

create_entry_work:
    entryWork__9CWorkUtilFP11CWorkThreadP11CWorkThreadb(root, parent, false);

    if (root != nullptr) {
        root->func_804385CC(0);
    }

    return root;
}

bool CViewRoot::wkStandbyLogin() {
    return false;
}

bool CViewRoot::wkStandbyLogout() {
    return false;
}
