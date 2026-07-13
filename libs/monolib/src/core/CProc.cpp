#include "monolib/work.hpp"
#include "monolib/core.hpp"
#include "monolib/util.hpp"

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

CView* CProc::pssGetView(WORK_ID id) const {
    //Use the given id if valid
    if(id != INVALID_WORK_ID){
        CWorkThread* thread = CWorkUtil::getWorkThread(id);
        return CView::convertToView(thread);
    }

    //Otherwise, get the view corresponding to the first id in the list
    if(mViewIDList.size() == 0){
        return nullptr;
    }else{
        CWorkThread* thread = CWorkUtil::getWorkThread(getFirstViewID());
        return CView::convertToView(thread);
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
    CView* view = pssGetView(INVALID_WORK_ID);

    if(view != nullptr){
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

void CProc::pssDetachView(){
    for(reslist<u32>::iterator it = mViewIDList.begin(); it != mViewIDList.end(); it++){
        u32 value = *it;
        CView* view = CViewRoot::getView(value);
        view->detachRenderWork(this);
    }

    mViewIDList.clear();
}

void CProc::pssAttachView(CView* pView){
    pView->attachRenderWork(this);

    //Add the new view's id to the list
    WORK_ID viewID = pView->mWorkID;
    mViewIDList.push_back(viewID);
}

//TODO: According to XCX's symbol, this shouldn't have a parameter, but XC1 and XCX's code suggest otherwise
ml::CRect16& CProc::pssMakeClientRect(ml::CRect16& rect) const {
    if(mViewIDList.size() == 0){
        rect.set(0, 0, 0, 0);
        return rect;
    }

    CView* view = pssGetView(INVALID_WORK_ID);
    u32 numChildren = view->mChildren.size();

    ml::CRect16 rect_40;
    view->getRect(rect_40);

    float someX = rect_40.mSize.x;
    float someY = rect_40.mSize.y;
    s16 anotherX = (someX*0.6f - someX)/2;
    s16 anotherY = (someY*0.6f - someY)/2;
    rect.mPos.x = (rect_40.mPos.x - anotherX) + numChildren * 20;
    rect.mPos.y = (rect_40.mPos.y - anotherY) + numChildren * 20;
    rect.mSize.x = rect_40.mSize.x + anotherX*2;
    rect.mSize.y = rect_40.mSize.y + anotherY*2;

    return rect;
}

CView* CProc::pssCreateView(const char* pName, CWorkThread* pThread, int param3){
    ml::FixStr<64> viewName = "(View)";
    viewName += pName;
    CView* view = CView::create(viewName.c_str(), pThread);
    view->wkReplaceHasChild((s16)param3);
    pssAttachView(view);

    CWorkThread* parent = mParent;

    if(parent->mType == THREAD_CPROC){
        CProc* parentProc = convertToProc(parent);
        ml::CRect16 rect;
        view->setRect(parentProc->pssMakeClientRect(rect));
    }

    return view;
}

bool CProc::wkStandbyLogin(){
    //Kind of strange that it just calls this function. Maybe there was code here that got stubbed?
    return CWorkThread::wkStandbyLogin();
}

bool CProc::wkStandbyLogout(){
    pssDetachView();
    CViewRoot::destroyProc(this);
    return CWorkThread::wkStandbyLogout();
}
