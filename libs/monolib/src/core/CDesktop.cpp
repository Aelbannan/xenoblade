#include "monolib/core.hpp"
#include <decomp.h>
#include <typeinfo>
#include "monolib/device.hpp"
#include "monolib/lib.hpp"
#include "functions.hpp"  // CTaskGame_setVec4 (vec4 setter)
#include "monolib/util/MemManager.hpp"  // canonical mtl::MemManager::allocate
// CViewRoot::getRootView is defined in CViewRoot.cpp as a C-linkage free
// function (retail symbol getRootView__9CViewRootFP5CView); the read-only
// CViewRoot.hpp only stubs a no-arg member form.
extern "C" CView* getRootView__9CViewRootFP5CView(CView* view);
// CGXCache ring command-2 cache update (retail updateCmd2Color__8CGXCacheFv);
// the read-only CGXCache.hpp omits it.
extern "C" void updateCmd2Color__8CGXCacheFv(CGXCache* self, CTaskGame_setVec4_tmp* color, int flag);
// CDeviceFile work-thread detach (retail setParent__11CDeviceFileFP11CWorkThread);
// the read-only CDeviceFile.hpp only stubs a no-arg member form.
extern "C" void setParent__11CDeviceFileFP11CWorkThread(CWorkThread* parent);
// CDeviceFile instance-field clear (retail clearParent__11CDeviceFileFv);
// the read-only CDeviceFile.hpp omits it entirely.
extern "C" void clearParent__11CDeviceFileFv(CDeviceFile* self);

// Type tag whose RTTI locator is the .sdata descriptor at 0x80663760
// ({name, 0}); typeid(Desktop).name() is the view name.
class Desktop {};

// Retail sbss singletons. CDesktop::spIcon/spInstance/sIconInitialized live in
// the CDeviceGX retail sbss range (lbl_eu_806656A8/AC/B0, defined there); the
// anonymous-namespace thread singletons are real class static members defined
// at the bottom of this TU. Their names in symbols.txt are the MWCC manglings
// of CDesktopBackGround::spInstance and CDesktopException::spInstance.
extern "C" u32 lbl_eu_806656A8;   // DesktopIcon* CDesktop::spIcon (CDeviceGX TU)
extern u8 lbl_eu_806656B0;     // bool CDesktop::sIconInitialized (CDeviceGX TU)


// WorkEvent3 payload (event id at +0x00; 0x15 = camera select).
struct CWorkEvent3Param {
    s32 field_0;   // +0x00 event id
};

CDesktop::CDesktop(const char* pName, CWorkThread* pParent) :
CProc(pName, pParent, MAX_CHILD),
mView(nullptr) {
    mType = THREAD_CDESKTOP;
    lbl_eu_806656AC = this;
    unk1EC.set(0.5f, 0.5f, 0.5f, 1.0f);
    CView::setDefaultFrameColor(unk1EC);
}

// Retail saves r30+r31 via stmw (-O4,s unit).
CDesktop::~CDesktop(){
    // Retail writes the sbss singleton under its linker name.
    lbl_eu_806656AC = nullptr;
}

CDesktop* CDesktop::getInstance(){
    return lbl_eu_806656AC;
}

// retail wkStandbyLogin calls getView out-of-line (bl getView__8CDesktopFv).
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

    // Keep the singleton load below the null check (retail order);
    // the instruction emitted is the same plain lwz.
    return reinterpret_cast<CDesktopException*>(*reinterpret_cast<u32*>(&CDesktopException::spInstance));
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
    // Positive-condition nesting: retail branches beq->shared-fail-tail for
    // both guards and emits the `return false` at the end of the function.
    if(CDeviceFile::getInstance() != nullptr && CDeviceFile::isInitialized()){
    // Seed the CGXCache color cache with a transparent-black CCol4, then tell
    // CDeviceFile that the desktop owns the file work threads.
    CGXCache* cache = CDeviceGX::getCacheInstance();
    CTaskGame_setVec4_tmp col1;
    updateCmd2Color__8CGXCacheFv(cache, CTaskGame_setVec4(&col1, 0.0f, 0.0f, 0.0f, 1.0f), 1);
    setParent__11CDeviceFileFP11CWorkThread(this);

    // Create the desktop's view (named after the desktop itself), then give
    // it a transparent-black clear color.
    mView = this->pssCreateView(this->mName.c_str(), CViewRoot::getInstance(), 0x80);
    setParent__11CDeviceFileFP11CWorkThread(this);

    // Create the desktop's view (named after the desktop itself), then give
    // it a transparent-black clear color.
    mView = this->pssCreateView(this->mName.c_str(), CViewRoot::getInstance(), 0x80);

    ml::CVec4 col2;
    CTaskGame_setVec4(reinterpret_cast<CTaskGame_setVec4_tmp*>(&col2), 0.0f, 0.0f, 0.0f, 1.0f);
    mView->unk444 = col2;

    // Push a tag-3 context message (the drainer ORs unk278 with 0x3). Retail
    // copies the raw bytes of an uninitialized 0x24-byte entry local; the
    // payload is ignored by the tag-3 consumer.
    CView* view = mView;
    u32 sum = view->mContextMsg.mRing.mFront + view->mContextMsg.mRing.mSize;
    u32 slot = sum % view->mContextMsg.mRing.mCapacity;
    CViewContextRingEntry* entry = reinterpret_cast<CViewContextRingEntry*>(
        (u8*)view->mContextMsg.mRing.mArrayPtr + slot * 0x24);
    CViewContextRingEntry ctx;
    *entry = ctx;
    entry->tag = 3;
    entry->pad = 0;
    view->mContextMsg.mRing.mSize += 1;
    view->mContextMsg.mRing.field6 = view->mContextMsg.mRing.mSize - 1;
    mView->unk460 = 0x13;

    // Name the view after the desktop type's RTTI name (both the view-local
    // name and the inherited CWorkThread name; the base name only while
    // still empty).
    const char* desktop = typeid(Desktop).name();
    mView->mName.mLength = strlen(desktop);
    strcpy(mView->mName.mString, desktop);
    if(mView->CWorkThread::mName.mLength == 0){
        mView->CWorkThread::mName.mLength = strlen(desktop);
        strcpy(mView->CWorkThread::mName.mString, desktop);
    }

    // Spawn the background and exception work threads; their names come
    // directly from the shared string pool ("CDesktopBackGround" at offset 0,
    // "CDesktopException" at +0x13). Retail placement-constructs the raw
    // CProc subobject in work memory (no derived ctor exists), installs the
    // dissolved vtable by hand, records the singleton, then registers the
    // thread.
    CProcRoot* root = CProcRoot::getInstance();

    void* bgMem = mtl::MemManager::allocate(0x1F0, getWorkMem__17CWorkThreadSystemFv());
    if(bgMem != nullptr){
        CDesktopBackGround* bg = new (bgMem) CDesktopBackGround("CDesktopBackGround", root);
    }
    entryWork__9CWorkUtilFP11CWorkThreadP11CWorkThreadb(static_cast<CWorkThread*>(bgMem), root, false);
    *reinterpret_cast<u32*>(static_cast<u8*>(bgMem) + 0x1E4) = getView()->mWorkID;

    void* exMem = mtl::MemManager::allocate(0x1F0, getWorkMem__17CWorkThreadSystemFv());
    if(exMem != nullptr){
        CDesktopException* ex = new (exMem) CDesktopException("CDesktopException", root);
    }
    entryWork__9CWorkUtilFP11CWorkThreadP11CWorkThreadb(static_cast<CWorkThread*>(exMem), root, false);
    *reinterpret_cast<u32*>(static_cast<u8*>(exMem) + 0x1E4) = getView()->mWorkID;

    //Call the desktop icon's start function if set (CGame::GameMain)
    if(lbl_eu_806656B0 != 0){
        reinterpret_cast<DesktopIcon*>(lbl_eu_806656A8)->startFunc();
    }

    CDeviceVI::setFlag0(false);
    CDeviceVI::setFlag4(false);
    return CProc::wkStandbyLogin();
    }

    return false;
}

// Logout: once the desktop has no children and the background work thread is
// gone, clear the VI flags, detach the desktop from CDeviceFile, and run the
// base CProc logout.
bool CDesktop::wkStandbyLogout(){
    if(hasChild(this) == false && CProcRoot::getInstance()->getWorkThread("CDesktopBackGround") == nullptr){
        CDeviceVI::setFlag0(true);
        clearParent__11CDeviceFileFv(reinterpret_cast<CDeviceFile*>(this));
        return CProc::wkStandbyLogout();
    }

    return false;
}

// Walk the current root view's attached-view list and return the first view
// whose work thread is a CProc with a non-null pssGetRoot (this is unused in
// the retail body, hence the free-function form).
extern "C" CProc* getCurrentProc__8CDesktopFv(){
    CView* view = CViewRoot::getCurrent();
    if(view == nullptr){
        return nullptr;
    }

    CView* rootView = getRootView__9CViewRootFP5CView(view);
    if(rootView == nullptr){
        return nullptr;
    }

    reslist<WORK_ID>& list = rootView->mWorkIDList;
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

void CDesktop::entryTable(DesktopIcon* pIcon, bool state){
    lbl_eu_806656A8 = reinterpret_cast<u32>(pIcon);
    lbl_eu_806656B0 = state;
}

void CDesktop::setAppException(int enable){
    CDesktop* desktop = getInstance();

    if(desktop == nullptr) return;

    desktop->wkSetEvent(enable != 0 ? EVT_APPEXCEPTION_ON : EVT_APPEXCEPTION_OFF);
}

namespace{
    CDesktopBackGround::~CDesktopBackGround(){
        CDesktopBackGround::spInstance = nullptr;
    }
    CDesktopException::~CDesktopException(){
        CDesktopException::spInstance = nullptr;
    }
}

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

// .sbss 0x806656B4..0x806656C0: the anonymous-namespace thread singletons.
// The Exception slot has an 8-byte retail extent: word 0 = the static member,
// word 1 = retail tail padding (never read).
CDesktopBackGround* CDesktopBackGround::spInstance = nullptr;
CDesktopException* CDesktopException::spInstance = nullptr;
extern "C" u32 gap_20_806656BC_sbss = 0;
