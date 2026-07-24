#include "monolib/work.hpp"
#include "monolib/core.hpp"
#include "monolib/util.hpp"

extern "C" {
extern const char lbl_eu_80522500[]; // "(View)"
extern float lbl_eu_8066A278; // 0.6f
void getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(ml::CRect16* rect, CViewFrame* frame);
}

CProc::CProc(const char* pName, CWorkThread* pParent, s16 capacity) :
CWorkThread(pName, pParent, capacity),
unk1E4(mtl::INVALID_HANDLE){
    unk1E8 = 2;
    mType = THREAD_CPROC;
    mViewIDList.reserve(mAllocHandle, 16);
}

CProc::~CProc(){
    //Empty loop. Maybe had stubbed code?
    for(reslist<u32>::iterator it = mViewIDList.begin(); it != mViewIDList.end(); it++){
    }
}

//Finds the root CProc instance for the given instance
CProc* CProc::pssGetRoot(CProc* pProc) {
    if (pProc == nullptr) {
        return nullptr;
    }

    //If the parent is CProcRoot, return null
    if (pProc->pssGetParent() == CProcRoot::getInstance()){
        return nullptr;
    }

    //If the parent is CDesktop, return this instance as the root
    if (pProc->pssGetParent() == CDesktop::getInstance()){
        return pProc;
    }

    CProc* parent = convertToProc(pProc->pssGetParent());
    return pssGetRoot(parent);
}

void CProc::pssSetFocus(){
    _reslist_node<WORK_ID>* curNode;
    u32 length;
    _reslist_node<WORK_ID>* endNode;
    CView* view;

    length = 0;
    endNode = mViewIDList.mStartNodePtr;
    curNode = endNode->mNext;

    while (curNode != endNode) {
        length++;
        curNode = curNode->mNext;
    }

    if (length == 0) {
        view = nullptr;
    } else {
        view = CView::convertToView(CWorkUtil::getWorkThread(getFirstViewID()));
    }

    if (view != nullptr) {
        view->setDisp(true, true);
        view->setCurrent();
    }
}

bool CProc::pssDetachView(WORK_ID id){
    reslist<WORK_ID>::iterator it = mViewIDList.find(id);

    if(it != mViewIDList.end()){
        CView* view = CViewRoot::getView(*it);
        view->detachRenderWork(this);
        mViewIDList.erase(it);
        return true;
    }

    return false;
}

struct PssCreateWalkFrame {
    void* volatile size0;
    void* volatile size4;
    void* volatile size8;
    void* volatile sizeC;
    void* volatile front1;
    void* volatile size2_0;
    void* volatile size2_4;
    void* volatile size2_8;
    void* volatile size2_C;
    void* volatile front2;
    void* volatile child0;
    void* volatile child4;
    void* volatile child8;
    void* volatile childC;
    ml::CRect16 frameOffset;
    ml::CRect16 clientRect;
};

static inline ml::CRect16& pssMakeClientRectInline(
    const CProc* proc, PssCreateWalkFrame& wf) {
    u32 length = 0;
    _reslist_node<WORK_ID>* endNode = proc->mViewIDList.mStartNodePtr;
    wf.size0 = endNode;
    _reslist_node<WORK_ID>* curNode = endNode->mNext;
    wf.size8 = endNode;
    wf.size4 = curNode;
    wf.sizeC = curNode;
    while (wf.sizeC != wf.size8) {
        ++length;
        wf.sizeC = ((_reslist_node<WORK_ID>*)wf.sizeC)->mNext;
    }

    if (length == 0) {
        wf.clientRect.set(0, 0, 0, 0);
        return wf.clientRect;
    }

    length = 0;
    wf.size2_C = endNode;
    wf.size2_4 = endNode;
    wf.size2_8 = curNode;
    wf.size2_0 = curNode;
    while (wf.size2_0 != wf.size2_4) {
        ++length;
        wf.size2_0 = ((_reslist_node<WORK_ID>*)wf.size2_0)->mNext;
    }

    CView* parentView;
    if (length == 0) {
        parentView = nullptr;
    } else {
        wf.front1 = curNode;
        parentView = CView::convertToView(
            CWorkUtil::getWorkThread(curNode->mItem));
        wf.front2 = curNode;
    }

    u32 numChildren = 0;
    _reslist_node<CWorkThread*>* childEnd =
        parentView->mChildren.mStartNodePtr;
    wf.child0 = childEnd;
    _reslist_node<CWorkThread*>* childCur = childEnd->mNext;
    wf.child8 = childEnd;
    wf.child4 = childCur;
    wf.childC = childCur;
    while (wf.childC != wf.child8) {
        ++numChildren;
        wf.childC = ((_reslist_node<CWorkThread*>*)wf.childC)->mNext;
    }

    getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(
        &wf.frameOffset, &parentView->mFrame);

    s16 sizeX = parentView->mRectData.mViewSize.x;
    s16 sizeY = parentView->mRectData.mViewSize.y;
    s16 scaledX = (s16)((float)sizeX * lbl_eu_8066A278);
    s16 scaledY = (s16)((float)sizeY * lbl_eu_8066A278);
    s16 anotherX = (scaledX - sizeX) / 2;
    s16 anotherY = (scaledY - sizeY) / 2;
    s16 childOff = (s16)(numChildren * 20);
    s16 posX = (s16)(parentView->mFrame.mContentX + wf.frameOffset.mPos.x);
    s16 posY = (s16)(parentView->mFrame.mContentY + wf.frameOffset.mPos.y);

    wf.clientRect.mPos.x = (s16)((posX - anotherX) + childOff);
    wf.clientRect.mPos.y = (s16)((posY - anotherY) + childOff);
    wf.clientRect.mSize.x = (s16)(sizeX + anotherX * 2);
    wf.clientRect.mSize.y = (s16)(sizeY + anotherY * 2);
    return wf.clientRect;
}

CView* CProc::pssCreateView(const char* pName, CWorkThread* pThread, int param3){
    ml::FixStr<64> viewName = "(View)";
    PssCreateWalkFrame wf;
    viewName += pName;
    CView* view = CView::create(viewName.c_str(), pThread);
    view->wkReplaceHasChild((s16)param3);
    view->attachRenderWork(this);
    // Hoist mWorkID so the inlined free-slot walk matches retail preload
    // schedule (loop offsets / beq 0x0c). Remaining r5/r8 and stack-home
    // coloring remains a Chaitin near-miss — keep iterating in high-level C.
    WORK_ID workId = view->mWorkID;
    mViewIDList.push_back(workId);

    CWorkThread* parent = mParent;
    if (parent->mType == THREAD_CPROC) {
        view->setRect(pssMakeClientRectInline(
            convertToProc(parent), wf));
    }
    return view;
}

bool CProc::wkStandbyLogin(){
    //Kind of strange that it just calls this function. Maybe there was code here that got stubbed?
    return CWorkThread::wkStandbyLogin();
}

bool CProc::wkStandbyLogout(){
    // Retail inlines detach-all (no separate pssDetachView() symbol in this split).
    for (reslist<u32>::iterator it = mViewIDList.begin(); it != mViewIDList.end();
         it++) {
        u32 value = *it;
        CView* view = CViewRoot::getView(value);
        view->detachRenderWork(this);
    }
    {
        _reslist_node<u32>* endNode = mViewIDList.mStartNodePtr;
        _reslist_node<u32>* curNode = endNode->mNext;

        while (curNode != mViewIDList.mStartNodePtr) {
            _reslist_node<u32>* oldNode = curNode;
            curNode = curNode->mNext;
            oldNode->mNext = nullptr;
        }

        mViewIDList.mStartNodePtr->mNext = mViewIDList.mStartNodePtr;
        mViewIDList.mStartNodePtr->mPrev = mViewIDList.mStartNodePtr;
    }
    CViewRoot::destroyProc(this);
    return CWorkThread::wkStandbyLogout();
}
