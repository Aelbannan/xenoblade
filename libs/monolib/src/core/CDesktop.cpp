// Suppress MWCC auto-vtable/typeinfo emissions: all class data ships from the
// dissolved retail blocks at the bottom of this TU.
class __declspec(novtable) CDesktop;
namespace {
    class __declspec(novtable) CDesktopBackGround;
    class __declspec(novtable) CDesktopException;
}

#include "monolib/core.hpp"
#include <decomp.h>
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
// Work-memory allocation path used by wkStandbyLogin: retail placement-
// constructs the raw CProc subobject in work memory (no derived ctor).
// getWorkMem/allocate/entryWork are declared in CViewRoot.hpp.
extern "C" void __ct__5CProcFPCcP11CWorkThreads(void* self, const char* pName, CWorkThread* pParent, int maxChild);

// Retail .rodata string pool (0x80522F44): "CDesktopBackGround\0CDesktopException\0".
extern const char lbl_eu_80522F44[];
// Retail .sdata2 color constants used by the ctor frame color and the
// wkStandbyLogin clear-color fills.
extern const f32 lbl_eu_8066A450[];
extern const f32 lbl_eu_8066A458;
extern const f32 lbl_eu_8066A460;
extern const f32 lbl_eu_8066A45C;
// Dissolved CDesktop vtable (ctor installs it by hand; novtable TU) plus the
// two anonymous-namespace thread vtables used by wkStandbyLogin.
extern "C" u32 lbl_eu_8056CBC8[40];
extern "C" u32 lbl_eu_8056CB08[40];
extern "C" u32 lbl_eu_8056CA48[40];
// Dissolved .sdata type descriptor (word 0 = "Desktop" string pointer).
extern "C" u32 lbl_eu_80663760[2];

// Retail sbss singletons. CDesktop::spIcon/spInstance/sIconInitialized live in
// the CDeviceGX retail sbss range (lbl_eu_806656A8/AC/B0, defined there); the
// anonymous-namespace thread singletons (lbl_eu_806656B4/B8) are defined in
// the dissolved .sbss block at the bottom of this TU. MWCC does not mangle
// global-scope variable names, so the emitted symbols are the retail names.
extern "C" u32 lbl_eu_806656A8;   // DesktopIcon* CDesktop::spIcon (CDeviceGX TU)
extern "C" CDesktop* lbl_eu_806656AC; // CDesktop::spInstance (CDeviceGX TU)
extern u8 lbl_eu_806656B0;     // bool CDesktop::sIconInitialized (CDeviceGX TU)
extern "C" CDesktopBackGround* lbl_eu_806656B4;
extern "C" u32 lbl_eu_806656B8;

// WorkEvent3 payload (event id at +0x00; 0x15 = camera select).
struct CWorkEvent3Param {
    s32 field_0;   // +0x00 event id
};

// Retail installs the dissolved vtable by hand (novtable TU) and reads the
// frame-color constants from the shared .sdata2 pool, not literals.
CDesktop::CDesktop(const char* pName, CWorkThread* pParent) :
CProc(pName, pParent, MAX_CHILD),
mView(nullptr) {
    lbl_eu_806656AC = this;
    *(u32**)this = (u32*)lbl_eu_8056CBC8;
    mType = THREAD_CDESKTOP;
    unk1EC.set(lbl_eu_8066A458, lbl_eu_8066A458, lbl_eu_8066A458, lbl_eu_8066A45C);
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

    // volatile keeps the singleton load below the null check (retail order);
    // the instruction emitted is the same plain lwz.
    return reinterpret_cast<CDesktopException*>(*reinterpret_cast<volatile u32*>(&lbl_eu_806656B8));
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
    func_800407C8_tmp col1;
    func_8044A578__8CGXCacheFv(cache, func_800407C8(&col1, lbl_eu_8066A460, lbl_eu_8066A460, lbl_eu_8066A460, lbl_eu_8066A45C), 1);
    func_8044E770__11CDeviceFileFP11CWorkThread(this);

    // Create the desktop's view (named after the desktop itself), then give
    // it a transparent-black clear color.
    mView = this->pssCreateView(this->mName.c_str(), CViewRoot::getInstance(), 0x80);
    func_8044E770__11CDeviceFileFP11CWorkThread(this);

    // Create the desktop's view (named after the desktop itself), then give
    // it a transparent-black clear color.
    mView = this->pssCreateView(this->mName.c_str(), CViewRoot::getInstance(), 0x80);

    ml::CVec4 col2;
    func_800407C8(reinterpret_cast<func_800407C8_tmp*>(&col2), lbl_eu_8066A460, lbl_eu_8066A460, lbl_eu_8066A460, lbl_eu_8066A45C);
    mView->unk444 = col2;

    // Push a tag-3 context message (the drainer ORs unk278 with 0x3). Retail
    // copies the raw bytes of an uninitialized 0x24-byte entry local; the
    // payload is ignored by the tag-3 consumer.
    CView* view = mView;
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
    mView->unk460 = 0x13;

    // Name the view after the "Desktop" string (both the view-local name and
    // the inherited CWorkThread name; the base name only while still empty).
    // lbl_eu_80663760[0] holds the "Desktop" string pointer (sdata2 pool).
    const char* desktop = reinterpret_cast<const char*>(lbl_eu_80663760[0]);
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

    void* bgMem = allocate__Q23mtl10MemManagerFUlUl(0x1F0, getWorkMem__17CWorkThreadSystemFv());
    if(bgMem != nullptr){
        __ct__5CProcFPCcP11CWorkThreads(bgMem, lbl_eu_80522F44, root, 8);
        *(u32**)bgMem = (u32*)lbl_eu_8056CB08;
        lbl_eu_806656B4 = static_cast<CDesktopBackGround*>(bgMem);
    }
    entryWork__9CWorkUtilFP11CWorkThreadP11CWorkThreadb(static_cast<CWorkThread*>(bgMem), root, false);
    *reinterpret_cast<u32*>(static_cast<u8*>(bgMem) + 0x1E4) = getView()->mWorkID;

    void* exMem = allocate__Q23mtl10MemManagerFUlUl(0x1F0, getWorkMem__17CWorkThreadSystemFv());
    if(exMem != nullptr){
        __ct__5CProcFPCcP11CWorkThreads(exMem, lbl_eu_80522F44 + 0x13, root, 0x40);
        *(u32**)exMem = (u32*)lbl_eu_8056CA48;
        lbl_eu_806656B8 = reinterpret_cast<u32>(exMem);
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
    lbl_eu_806656A8 = reinterpret_cast<u32>(pIcon);
    lbl_eu_806656B0 = state;
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
        lbl_eu_806656B8 = 0;
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

// ===== Dissolved retail data (CDesktop TU) =====
// Retail bytes/relocs dumped from build/us/asm/monolib/src/core/CDesktop.s
// .rodata 0x80522EE0 size 0x90 align 8, .data 0x8056CA48 size 0x240 align 8,
// .sdata 0x80663760 size 0x20 align 8, .sbss 0x806656B4 size 0xC align 4.
// Emitted as extern "C" u32 arrays with (u32)&extern, rodata align,
// DECOMP_FORCEACTIVE, novtable.
//
// The anonymous-namespace thread-class methods are defined above under their
// retail @unnamed@ mangling; the vtables below reference legal stand-in
// spellings and the §17.6 UNIT_RULES retarget those four slots (see
// tools/postprocess_reloc_names.py "CDesktop.o").
extern "C" void __dt__17CDesktopExceptionFv();
extern "C" void wkStandbyLogout__17CDesktopExceptionFv();
extern "C" void __dt__18CDesktopBackGroundFv();
extern "C" void wkStandbyLogout__18CDesktopBackGroundFv();

extern "C" {
    // Shared base-class virtuals (defined in other monolib TUs).
    extern void WorkEvent1__10IWorkEventFPvPCc();
    extern void OnFileEvent__10IWorkEventFP10CEventFile();
    extern void WorkEvent3__10IWorkEventFPv();
    extern void WorkEvent4__10IWorkEventFv();
    extern void OnPauseTrigger__10IWorkEventFb();
    extern void WorkEvent6__10IWorkEventFv();
    extern void WorkEvent7__10IWorkEventFv();
    extern void WorkEvent8__10IWorkEventFv();
    extern void WorkEvent9__10IWorkEventFv();
    extern void WorkEvent10__10IWorkEventFv();
    extern void WorkEvent11__10IWorkEventFv();
    extern void WorkEvent12__10IWorkEventFv();
    extern void WorkEvent13__10IWorkEventFv();
    extern void WorkEvent14__10IWorkEventFv();
    extern void WorkEvent15__10IWorkEventFv();
    extern void WorkEvent16__10IWorkEventFv();
    extern void WorkEvent17__10IWorkEventFv();
    extern void WorkEvent18__10IWorkEventFv();
    extern void WorkEvent19__10IWorkEventFv();
    extern void WorkEvent20__10IWorkEventFv();
    extern void WorkEvent21__10IWorkEventFv();
    extern void WorkEvent22__10IWorkEventFv();
    extern void WorkEvent23__10IWorkEventFv();
    extern void WorkEvent24__10IWorkEventFv();
    extern void WorkEvent25__10IWorkEventFv();
    extern void WorkEvent26__10IWorkEventFv();
    extern void WorkEvent27__10IWorkEventFv();
    extern void WorkEvent28__10IWorkEventFv();
    extern void WorkEvent29__10IWorkEventFv();
    extern void WorkEvent30__10IWorkEventFv();
    extern void WorkEvent31__10IWorkEventFv();
    extern void wkUpdate__11CWorkThreadFv();
    extern void wkRender__11CWorkThreadFv();
    extern void wkRenderAfter__11CWorkThreadFv();
    extern void wkStandbyLogin__5CProcFv();
    extern void wkStandbyExceptionRetry__11CWorkThreadFUl();
    // NOTE: the base typeinfo objects (__RTTI__10IWorkEvent / __RTTI__11CWorkThread
    // / __RTTI__5CProc) cannot be spelled here: declaring an __RTTI__* name in a TU
    // with novtable-predeclared anonymous-namespace classes trips an MWCC -ipa file
    // ICE ("illegal name overloading"). The hierarchy tables below reference legal
    // stand-in spellings; the §17.6 UNIT_RULES retarget those nine slots to the
    // retail typeinfo symbols.
    extern u32 rtti_10IWorkEvent[];
    extern u32 rtti_11CWorkThread[];
    extern u32 rtti_5CProc[];
    // Own members (defined in this TU; mangled spellings).
    extern void __dt__8CDesktopFv();
    extern void wkUpdate__8CDesktopFv();
    extern void wkRender__8CDesktopFv();
    extern void wkRenderAfter__8CDesktopFv();
    extern void wkStandbyLogin__8CDesktopFv();
    extern void wkStandbyLogout__8CDesktopFv();
    extern void OnFileEvent__8CDesktopFP10CEventFile();
    extern void WorkEvent3__8CDesktopFPv();
    extern const char lbl_eu_80522EE0[];
    extern const char lbl_eu_80522F0C[];
    extern const char lbl_eu_80522F38[];
    extern u32 lbl_eu_8056CAE8[];
    extern u32 lbl_eu_8056CBA8[];
    extern u32 lbl_eu_8056CC68[];
    extern u32 lbl_eu_8056CA48[];
    extern u32 lbl_eu_8056CB08[];
}

// .rodata 0x80522EE0..0x80522F70: typeinfo name strings + shared name pool.
// Per-object alignment reproduces the retail packing (8/4/8/4).
extern "C" __declspec(section ".rodata") __attribute__((aligned(8))) const char lbl_eu_80522EE0[44] = "@unnamed@CDesktop_cpp@::CDesktopException";
extern "C" __declspec(section ".rodata") __attribute__((aligned(4))) const char lbl_eu_80522F0C[44] = "@unnamed@CDesktop_cpp@::CDesktopBackGround";
extern "C" __declspec(section ".rodata") __attribute__((aligned(8))) const char lbl_eu_80522F38[12] = "CDesktop";
extern "C" __declspec(section ".rodata") __attribute__((aligned(4))) const char lbl_eu_80522F44[44] = "CDesktopBackGround\0CDesktopException";

// .sdata 0x80663760..0x80663780: monolib type descriptors {name, table}.
extern "C" __declspec(section ".sdata") __attribute__((aligned(8))) u32 lbl_eu_80663760[2] = { (u32)&lbl_eu_8066A450, 0 };
extern "C" __declspec(section ".sdata") __attribute__((aligned(8))) u32 lbl_eu_80663768[2] = { (u32)&lbl_eu_80522EE0, (u32)&lbl_eu_8056CAE8 };
extern "C" __declspec(section ".sdata") __attribute__((aligned(8))) u32 lbl_eu_80663770[2] = { (u32)&lbl_eu_80522F0C, (u32)&lbl_eu_8056CBA8 };
extern "C" __declspec(section ".sdata") __attribute__((aligned(8))) u32 lbl_eu_80663778[2] = { (u32)&lbl_eu_80522F38, (u32)&lbl_eu_8056CC68 };

// .data 0x8056CA48..0x8056CC88: CDesktopException / CDesktopBackGround /
// CDesktop vtables (40 words each) + monolib RTTI hierarchy tables (8 words).
extern "C" u32 lbl_eu_8056CA48[40] __attribute__((aligned(8))) = {
    (u32)&lbl_eu_80663768, 0,
    (u32)&__dt__17CDesktopExceptionFv,
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
    (u32)&wkRenderAfter__11CWorkThreadFv, (u32)&wkStandbyLogin__5CProcFv,
    (u32)&wkStandbyLogout__17CDesktopExceptionFv,
    (u32)&wkStandbyExceptionRetry__11CWorkThreadFUl
};
extern "C" u32 lbl_eu_8056CAE8[8] __attribute__((aligned(8))) = {
    (u32)&rtti_10IWorkEvent, 0, (u32)&rtti_11CWorkThread, 0,
    (u32)&rtti_5CProc, 0, 0, 0
};
extern "C" u32 lbl_eu_8056CB08[40] __attribute__((aligned(8))) = {
    (u32)&lbl_eu_80663770, 0,
    (u32)&__dt__18CDesktopBackGroundFv,
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
    (u32)&wkRenderAfter__11CWorkThreadFv, (u32)&wkStandbyLogin__5CProcFv,
    (u32)&wkStandbyLogout__18CDesktopBackGroundFv,
    (u32)&wkStandbyExceptionRetry__11CWorkThreadFUl
};
extern "C" u32 lbl_eu_8056CBA8[8] __attribute__((aligned(8))) = {
    (u32)&rtti_10IWorkEvent, 0, (u32)&rtti_11CWorkThread, 0,
    (u32)&rtti_5CProc, 0, 0, 0
};
extern "C" u32 lbl_eu_8056CBC8[40] __attribute__((aligned(8))) = {
    (u32)&lbl_eu_80663778, 0,
    (u32)&__dt__8CDesktopFv,
    (u32)&WorkEvent1__10IWorkEventFPvPCc, (u32)&OnFileEvent__8CDesktopFP10CEventFile,
    (u32)&WorkEvent3__8CDesktopFPv, (u32)&WorkEvent4__10IWorkEventFv,
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
    (u32)&wkUpdate__8CDesktopFv, (u32)&wkRender__8CDesktopFv,
    (u32)&wkRenderAfter__8CDesktopFv, (u32)&wkStandbyLogin__8CDesktopFv,
    (u32)&wkStandbyLogout__8CDesktopFv,
    (u32)&wkStandbyExceptionRetry__11CWorkThreadFUl
};
extern "C" u32 lbl_eu_8056CC68[8] __attribute__((aligned(8))) = {
    (u32)&rtti_10IWorkEvent, 0, (u32)&rtti_11CWorkThread, 0,
    (u32)&rtti_5CProc, 0, 0, 0
};

// .sbss 0x806656B4..0x806656C0: the anonymous-namespace thread singletons.
// lbl_eu_806656B8 is an 8-byte retail slot: word 0 = Exception singleton,
// word 1 = retail tail padding (never read).
extern "C" CDesktopBackGround* lbl_eu_806656B4 = nullptr;
extern "C" u32 lbl_eu_806656B8 = 0;
extern "C" u32 gap_20_806656BC_sbss = 0;
