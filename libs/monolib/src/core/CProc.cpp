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

CView* CProc::pssCreateView(const char* pName, CWorkThread* pThread, int param3){
    // Retail -0xF0 / FixStr@0x68. Volatile walk homes force stack reloads (~85.3%).
    // Remaining: frame often -0x100 / FixStr@0x80; walks +0x18; this in r29 vs r30.
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

    ml::FixStr<64> viewName = lbl_eu_80522500;
    PssCreateWalkFrame wf;

    viewName += pName;
    CView* view = CView::create(viewName.c_str(), pThread);
    view->wkReplaceHasChild((s16)param3);
    // Retail inlines attach (no separate pssAttachView symbol in this split).
    view->attachRenderWork(this);
    mViewIDList.push_back(view->mWorkID);

    CWorkThread* parent = mParent;

    if (parent->mType == THREAD_CPROC) {
        CProc* parentProc = convertToProc(parent);
        u32 length;
        _reslist_node<WORK_ID>* endNode;
        _reslist_node<WORK_ID>* curNode;

        length = 0;
        endNode = parentProc->mViewIDList.mStartNodePtr;
        wf.size0 = endNode;
        curNode = endNode->mNext;
        wf.size8 = endNode;
        wf.size4 = curNode;
        wf.sizeC = curNode;
        while (wf.sizeC != wf.size8) {
            length++;
            wf.sizeC = ((_reslist_node<WORK_ID>*)wf.sizeC)->mNext;
        }

        if (length == 0) {
            wf.clientRect.set(0, 0, 0, 0);
        } else {
            CView* parentView;
            u32 numChildren;
            _reslist_node<CWorkThread*>* childEnd;
            _reslist_node<CWorkThread*>* childCur;

            length = 0;
            wf.size2_C = endNode;
            wf.size2_4 = endNode;
            wf.size2_8 = curNode;
            wf.size2_0 = curNode;
            while (wf.size2_0 != wf.size2_4) {
                length++;
                wf.size2_0 = ((_reslist_node<WORK_ID>*)wf.size2_0)->mNext;
            }

            if (length == 0) {
                parentView = nullptr;
            } else {
                wf.front1 = curNode;
                parentView = CView::convertToView(
                    CWorkUtil::getWorkThread(curNode->mItem));
                wf.front2 = curNode;
            }

            numChildren = 0;
            childEnd = parentView->mChildren.mStartNodePtr;
            wf.child0 = childEnd;
            childCur = childEnd->mNext;
            wf.child8 = childEnd;
            wf.child4 = childCur;
            wf.childC = childCur;
            while (wf.childC != wf.child8) {
                numChildren++;
                wf.childC = ((_reslist_node<CWorkThread*>*)wf.childC)->mNext;
            }

            getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(
                &wf.frameOffset, &parentView->unk1DC);

            s16 sizeX = parentView->unk1C8.unk0;
            s16 sizeY = parentView->unk1C8.unk2;
            s16 scaledX = (s16)((float)sizeX * lbl_eu_8066A278);
            s16 scaledY = (s16)((float)sizeY * lbl_eu_8066A278);
            s16 anotherX = (scaledX - sizeX) / 2;
            s16 anotherY = (scaledY - sizeY) / 2;
            s16 childOff = (s16)(numChildren * 20);
            s16 posX = (s16)(parentView->unk1DC.unk54 + wf.frameOffset.mPos.x);
            s16 posY = (s16)(parentView->unk1DC.unk56 + wf.frameOffset.mPos.y);

            wf.clientRect.mPos.x = (s16)((posX - anotherX) + childOff);
            wf.clientRect.mPos.y = (s16)((posY - anotherY) + childOff);
            wf.clientRect.mSize.x = (s16)(sizeX + anotherX * 2);
            wf.clientRect.mSize.y = (s16)(sizeY + anotherY * 2);
        }

        view->setRect(wf.clientRect);
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
    mViewIDList.clear();
    CViewRoot::destroyProc(this);
    return CWorkThread::wkStandbyLogout();
}
