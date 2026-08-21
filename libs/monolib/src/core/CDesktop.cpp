#include "monolib/core.hpp"
#include "monolib/device.hpp"
#include "monolib/lib.hpp"
#include "functions.hpp"  // func_800407C8 (vec4 setter)

// CViewRoot::getRootView is defined in CViewRoot.cpp as a C-linkage free
// function (retail symbol getRootView__9CViewRootFP5CView); the read-only
// CViewRoot.hpp only stubs a no-arg member form.
extern "C" CView* getRootView__9CViewRootFP5CView(CView* view);
// CGXCache ring command-2 cache update (retail func_8044A578__8CGXCacheFv);
// the read-only CGXCache.hpp omits it.
extern "C" void func_8044A578__8CGXCacheFv(CGXCache* self, func_800407C8_tmp* color, int flag);
// CDeviceFile work-thread detach (retail func_8044E770__11CDeviceFileFP11CWorkThread);
// the read-only CDeviceFile.hpp only stubs a no-arg member form.
extern "C" void func_8044E770__11CDeviceFileFP11CWorkThread(CWorkThread* parent);
// CDeviceFile instance-field clear (retail func_8044E780__11CDeviceFileFv);
// the read-only CDeviceFile.hpp omits it entirely.
extern "C" void func_8044E780__11CDeviceFileFv(CDeviceFile* self);

// Retail .rodata string pool (0x80522F44): "CDesktopBackGround\0CDesktopException\0".
extern const char lbl_eu_80522F44[];
// Retail .sdata const-char* (0x80663760) pointing at the "Desktop" name string.
extern const char* lbl_eu_80663760;

DesktopIcon* CDesktop::spIcon;
CDesktop* CDesktop::spInstance;
bool CDesktop::sIconInitialized = false;
CDesktopBackGround* CDesktopBackGround::spInstance;
CDesktopException* CDesktopException::spInstance;

// Retail sbss singletons for the anonymous-namespace work threads. The
// include/ header (which declares lbl_eu_806656AC) is outside the writable
// scope, so these live here; plain extern is enough (MWCC does not mangle
// global-scope variable names, so the emitted symbol is the retail name).
extern CDesktopBackGround* lbl_eu_806656B4;
extern CDesktopException* lbl_eu_806656B8;

// WorkEvent3 payload (event id at +0x00; 0x15 = camera select).
struct CWorkEvent3Param {
    s32 field_0;   // +0x00 event id
};

CDesktop::CDesktop(const char* pName, CWorkThread* pParent) :
CProc(pName, pParent, MAX_CHILD),
mView(nullptr) {
    spInstance = this;
    mType = THREAD_CDESKTOP;
    unk1EC.set(0.5f,0.5f,0.5f,1);
    CView::setDefaultFrameColor(unk1EC);
}

// optimize_for_size on: retail saves r30+r31 via stmw, not stw (same pattern
// as the CArcItem/CDrawGX dtors).
#pragma optimize_for_size on
CDesktop::~CDesktop(){
    // Retail writes the sbss singleton under its linker name.
    lbl_eu_806656AC = nullptr;
}
#pragma optimize_for_size off

CDesktop* CDesktop::getInstance(){
    return lbl_eu_806656AC;
}

// noinline: retail wkStandbyLogin calls getView out-of-line (bl getView__8CDesktopFv)
// rather than inlining the singleton read.
__declspec(noinline) CView* CDesktop::getView(){
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

void CDesktop::wkUpdate() {
    // NOTE: retail keeps a full frame (bl, not tail-call b). A bare
    // `isHbmControlInitialized();` statement makes MWCC -O4,p tail-call-fold
    // the whole body to a 4-byte `b`; the conditional form (both paths
    // return/fall through) keeps the 0x20 frame while emitting the same
    // single `bl` (MWCC drops the dead compare). Verified via scratch probe.
    if (CLibHbm::isHbmControlInitialized()) {
        return;
    }
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
    if(CDeviceFile::getInstance() == nullptr || CDeviceFile::isInitialized() == false){
        return false;
    }

    // Seed the CGXCache color cache with a transparent-black CCol4, then tell
    // CDeviceFile that the desktop owns the file work threads.
    func_800407C8_tmp col1;
    func_800407C8(&col1, 0.0f, 0.0f, 0.0f, 1.0f);
    func_8044A578__8CGXCacheFv(CDeviceGX::getCacheInstance(), &col1, 1);
    func_8044E770__11CDeviceFileFP11CWorkThread(this);

    // Create the desktop's view (named after the desktop itself), then give
    // it a transparent-black clear color.
    CView* view = this->pssCreateView(this->mName.c_str(), CViewRoot::getInstance(), 0x80);
    mView = view;

    ml::CVec4 col2;
    func_800407C8(reinterpret_cast<func_800407C8_tmp*>(&col2), 0.0f, 0.0f, 0.0f, 1.0f);
    view->unk444 = col2;

    // Push a tag-3 context message (the drainer ORs unk278 with 0x3). Retail
    // copies the raw bytes of an uninitialized 0x24-byte entry local; the
    // payload is ignored by the tag-3 consumer.
    u32 sum = view->unk3F0 + view->mContextRingWriteIndex;
    u32 slot = sum % view->mContextRingCapacity;
    CViewContextRingEntry* entry = reinterpret_cast<CViewContextRingEntry*>(
        view->mContextRingBase + slot * 0x24);
    CViewContextRingEntry ctx;
    *entry = ctx;
    entry->tag = 3;
    entry->pad = 0;
    view->mContextRingWriteIndex += 1;
    view->unk3FC = view->mContextRingWriteIndex - 1;
    view->unk460 = 0x13;

    // Name the view after the "Desktop" string (both the view-local name and
    // the inherited CWorkThread name; the base name only while still empty).
    const char* desktop = lbl_eu_80663760;
    view->mName = desktop;
    if(view->CWorkThread::mName.mLength == 0){
        view->CWorkThread::mName = desktop;
    }

    // Spawn the background and exception work threads; their names come from
    // the shared string pool ("CDesktopBackGround" / "CDesktopException").
    CProcRoot* root = CProcRoot::getInstance();
    CDesktopBackGround* background = CDesktopBackGround::create(lbl_eu_80522F44, root);
    background->unk1E4 = getView()->mWorkID;

    CDesktopException* exception = CDesktopException::create(lbl_eu_80522F44 + 0x13, root);
    exception->unk1E4 = getView()->mWorkID;

    //Call the desktop icon's start function if set (CGame::GameMain)
    if(sIconInitialized){
        spIcon->startFunc();
    }

    CDeviceVI::setFlag0(false);
    CDeviceVI::setFlag4(false);
    return CProc::wkStandbyLogin();
}

// Logout: once the desktop has no children and the background work thread is
// gone, clear the VI flags, detach the desktop from CDeviceFile, and run the
// base CProc logout.
bool CDesktop::wkStandbyLogout(){
    if(hasChild(this) == false && CProcRoot::getInstance()->getWorkThread(lbl_eu_80522F44) == nullptr){
        CDeviceVI::setFlag0(true);
        func_8044E780__11CDeviceFileFv(reinterpret_cast<CDeviceFile*>(this));
        return CProc::wkStandbyLogout();
    }

    return false;
}

// Walk the current root view's attached-view list and return the first view
// whose work thread is a CProc with a non-null pssGetRoot (this is unused in
// the retail body, hence the free-function form).
// optimize_for_size on: retail saves r30+r31 via stmw, not stw.
#pragma optimize_for_size on
extern "C" __declspec(noinline) CProc* getCurrentProc__8CDesktopFv(){
    CView* view = CViewRoot::getCurrent();
    if(view == nullptr){
        return nullptr;
    }

    CView* rootView = getRootView__9CViewRootFP5CView(view);
    if(rootView == nullptr){
        return nullptr;
    }

    reslist<WORK_ID>& list = *reinterpret_cast<reslist<WORK_ID>*>(&rootView->unk238);
    for(reslist<WORK_ID>::iterator it = list.begin(); it != list.end(); ++it){
        CWorkThread* thread = CWorkUtil::getWorkThread(*it);
        CProc* proc = static_cast<CProc*>(thread);
        if(thread == nullptr){
            proc = nullptr;
        }else{
            int type = thread->mType;
            if(CWorkThread::THREAD_CPROC > type || type >= CWorkThread::THREAD_CPROC_MAX){
                proc = nullptr;
            }
        }
        CProc* root = CProc::pssGetRoot(proc);
        if(root != nullptr){
            return root;
        }
    }

    return nullptr;
}
#pragma optimize_for_size off

void CDesktop::entryTable(DesktopIcon* pIcon, bool state){
    spIcon = pIcon;
    sIconInitialized = state;
}

void CDesktop::setAppException(int r3){
    CDesktop* desktop = getInstance();

    if(desktop == nullptr) return;

    desktop->wkSetEvent(r3 != 0 ? EVT_APPEXCEPTION_ON : EVT_APPEXCEPTION_OFF);
}

// optimize_for_size on: retail saves r30+r31 via stmw, not stw.
#pragma optimize_for_size on
namespace{
    CDesktopBackGround::~CDesktopBackGround(){
        lbl_eu_806656B4 = nullptr;
    }
    CDesktopException::~CDesktopException(){
        lbl_eu_806656B8 = nullptr;
    }
}
#pragma optimize_for_size off

bool CDesktop::OnFileEvent(CEventFile* pFile){
    if(pFile->unk0 == 2){
        CProc* proc = getCurrentProc__8CDesktopFv();
        if(proc != nullptr){
            return proc->OnFileEvent(pFile);
        }
    }
    return false;
}

bool CDesktop::WorkEvent3(void* pThing){
    CWorkEvent3Param* pParam = (CWorkEvent3Param*)pThing;
    if(pParam->field_0 == 0x15){
        CProc* proc = getCurrentProc__8CDesktopFv();
        if(proc != nullptr){
            return proc->WorkEvent3(pThing);
        }
    }
    return false;
}

// dissolved monolibdata2 - core/CDesktop data now provided via retail copy (additive edit)
