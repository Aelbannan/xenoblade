#include "monolib/core.hpp"
#include "monolib/device.hpp"
#include "monolib/lib.hpp"

DesktopIcon* CDesktop::spIcon;
CDesktop* CDesktop::spInstance;
bool CDesktop::sIconInitialized = false;
CDesktopBackGround* CDesktopBackGround::spInstance;
CDesktopException* CDesktopException::spInstance;

CDesktop::CDesktop(const char* pName, CWorkThread* pParent) :
CProc(pName, pParent, MAX_CHILD),
mView(nullptr) {
    spInstance = this;
    mType = THREAD_CDESKTOP;
    unk1EC.set(0.5f,0.5f,0.5f,1);
    CView::setDefaultFrameColor(unk1EC);
}

CDesktop::~CDesktop(){
    spInstance = nullptr;
}

CDesktop* CDesktop::getInstance(){
    return lbl_eu_806656AC;
}

CView* CDesktop::getView(){
    CDesktop* desktop = lbl_eu_806656AC;
    if(desktop == nullptr){
        return nullptr; 
    }

    return desktop->mView;
}

CDesktopException* CDesktop::getException(){
    CDesktop* desktop = getInstance();
    if(desktop == nullptr){
        return nullptr; 
    }

    return CDesktopException::spInstance;
}

void CDesktop::wkUpdate(){
    // NOTE: retail keeps a full frame (bl, not tail-call b). That is the
    // documented per-function -O level conflict (MWCC_REFERENCE __init_user):
    // -O4,p folds void f(){g();} to a 4-byte `b`, only -O1 reproduces the
    // 0x20 frame, and the rest of this unit needs -O4,p. No source bridge.
    CLibHbm::isHbmControlInitialized();
}

void CDesktop::wkRender(){
    CView* view = CViewRoot::getFullScreenView();
    if(view != nullptr){
        CView* current = CView::getCurrentView();
        if(view == current){
            return;
        }
    }
}

void CDesktop::wkRenderAfter(){
    CLibHbm::renderHbmstopIcon();
}

bool CDesktop::wkStandbyLogin(){
    if(CDeviceFile::getInstance() != nullptr && CDeviceFile::isInitialized()){
        CDesktopBackGround* background = CREATE_WORKTHREAD(CDesktopBackGround, CProcRoot::getInstance());
        background->unk1E4 = getView()->mWorkID;
        CDesktopException* exception = CREATE_WORKTHREAD(CDesktopException, CProcRoot::getInstance());
        exception->unk1E4 = getView()->mWorkID;

        //Call the desktop icon's start function if set (CGame::GameMain)
        if(sIconInitialized){
            spIcon->startFunc();
        }

        CDeviceVI::setFlag0(false);
        CDeviceVI::setFlag4(false);
        return CProc::wkStandbyLogin();
    }

    return false;
}

void CDesktop::entryTable(DesktopIcon* pIcon, bool state){
    spIcon = pIcon;
    sIconInitialized = state;
}

void CDesktop::setAppException(int r3){
    CDesktop* desktop = getInstance();

    if(desktop == nullptr) return;

    desktop->wkSetEvent(r3 != 0 ? EVT_APPEXCEPTION_ON : EVT_APPEXCEPTION_OFF);
}

namespace{
    CDesktopBackGround::~CDesktopBackGround(){
        spInstance = nullptr;
    }
    CDesktopException::~CDesktopException(){
        spInstance = nullptr;
    }
}

bool CDesktop::OnFileEvent(CEventFile* pFile) { return false; }
bool CDesktop::WorkEvent3(void* pThing) { return false; }


extern "C" void getCurrentProc__8CDesktopFv() {}
