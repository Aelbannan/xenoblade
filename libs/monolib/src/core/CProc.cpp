#include "monolib/work.hpp"
#include "monolib/core.hpp"
#include "monolib/util.hpp"
#include "monolib/data_vtables.hpp"
extern double lbl_eu_8066A300; // 0x4330000080000000 (s16->f32 conversion magic, FloatUtils-owned)
// s16 -> f32 conversion matching retail: build the 2^52+x double on the stack
// (low word = x ^ 0x80000000, high word = 0x43300000) and subtract the shared
// magic double (MWCC_CASES 7i; statement order matters).
inline float convF32(s32 v) {
    union { double d; u32 w[2]; } u;
    u.w[1] = (u32)v ^ 0x80000000;
    u.w[0] = 0x43300000;
    return (float)(u.d - lbl_eu_8066A300);
}


extern "C" {
extern const char lbl_eu_80522500[]; // "(View)" - retail .rodata pool string
extern float lbl_eu_8066A278; // 0.6f
void getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(ml::CRect16* rect,
                                                                   const CViewFrame* frame);
// Vtable/RTTI targets referenced by the CProc data definitions below.
// (extern is required inside extern "C": a bare declaration is a tentative
// DEFINITION and would emit a duplicate sbss symbol.)
extern void* __RTTI__5CProc;          // defined by kyoshin/CGame.cpp (.sdata 0x80661898)
extern void __dt__5CProcFv(void* self, int flags);
extern void wkStandbyLogin__5CProcFv();
extern void wkStandbyLogout__5CProcFv();
}

// --- Blob monolibdata1.s dissolve: this TU owns .data 0x8056B1E0-0x8056B2A8,
// .rodata 0x805224C8-0x80522508, .sdata 0x80663538-0x80663548. ---
//
// NOTE: the reslist<Ul> / _reslist_base<Ul> vtables (retail lbl_eu_8056B280 /
// lbl_eu_8056B298) are emitted by MWCC itself from the reslist<u32> member
// instantiation below: their dtor slots reference the template-mangled symbols
// __dt__11reslist<Ul>Fv / __dt__17_reslist_base<Ul>Fv, which cannot be spelled
// in C++ source (MWCC_CASES "What NOT to do": < and > are not valid C
// identifiers). The compiler emits them as the pre-existing weak symbols
// __vt__11reslist<Ul> / __vt__17_reslist_base<Ul> (0xC each), which carry the
// exact retail bytes at the range positions 0x8056B280 / 0x8056B298 (the
// retail 4th word of lbl_eu_8056B298 is the splitter's 8-byte alignment pad).
// The remaining range symbols are defined here with the retail names.

// RTTI name strings (.rodata).
extern const char lbl_eu_805224C8[];
extern const char lbl_eu_805224E0[];
// reslist<Ul> base-subobject RTTI list (.data, 0xC): [RTTI(_reslist_base<Ul>), 0, 0].
extern u32 lbl_eu_8056B28C[3];
// RTTI locators (.sdata, 8 bytes): { name, base-list }.
extern void* lbl_eu_80663538[2];
extern void* lbl_eu_80663540[2];

// CProc vtable (.data, 0xA0): [rtti, 0, dtor, IWorkEvent 1-31, wkUpdate/wkRender/
// wkRenderAfter (CWorkThread slots), wkStandbyLogin/Logout (CProc), wkStandbyExceptionRetry].
u32 lbl_eu_8056B1E0[0xA0 / 4] = {
    (u32)&__RTTI__5CProc, 0, (u32)&__dt__5CProcFv,
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
    (u32)&wkStandbyLogout__5CProcFv, (u32)&wkStandbyExceptionRetry__11CWorkThreadFUl,
};

u32 lbl_eu_8056B28C[3] = { (u32)&lbl_eu_80663540, 0, 0 };

void* lbl_eu_80663538[2] = { (void*)lbl_eu_805224C8, (void*)lbl_eu_8056B28C };
void* lbl_eu_80663540[2] = { (void*)lbl_eu_805224E0, 0 };

const char lbl_eu_805224C8[24] = {0x72,0x65,0x73,0x6C,0x69,0x73,0x74,0x3C,0x75,0x6E,0x73,0x69,0x67,0x6E,0x65,0x64,0x20,0x6C,0x6F,0x6E,0x67,0x3E,0x00,0x00};
const char lbl_eu_805224E0[32] = {0x5F,0x72,0x65,0x73,0x6C,0x69,0x73,0x74,0x5F,0x62,0x61,0x73,0x65,0x3C,0x75,0x6E,0x73,0x69,0x67,0x6E,0x65,0x64,0x20,0x6C,0x6F,0x6E,0x67,0x3E,0x00,0x00,0x00,0x00};

// lbl_eu_80522500 ("(View)", .rodata 0x7) is a pooled string-literal entry
// emitted from pssCreateView's FixStr init below (retail is a pool entry too);
// a named const definition would land in .sdata2 (7 bytes <= small-data limit)
// instead of .rodata, so the pool entry is left as the definition.

CProc::CProc(const char* pName, CWorkThread* pParent, s16 capacity) :
CWorkThread(pName, pParent, capacity),
unk1E4(mtl::INVALID_HANDLE){
    *(char**)this = (char*)&lbl_eu_8056B1E0;
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
    // Keep the two list walks typed; these are iterator state, not untyped storage.
    _reslist_node<WORK_ID>* volatile size0;
    _reslist_node<WORK_ID>* volatile size4;
    _reslist_node<WORK_ID>* volatile size8;
    _reslist_node<WORK_ID>* volatile sizeC;
    _reslist_node<WORK_ID>* volatile front1;
    _reslist_node<WORK_ID>* volatile size2_0;
    _reslist_node<WORK_ID>* volatile size2_4;
    _reslist_node<WORK_ID>* volatile size2_8;
    _reslist_node<WORK_ID>* volatile size2_C;
    _reslist_node<WORK_ID>* volatile front2;
    _reslist_node<CWorkThread*>* volatile child0;
    _reslist_node<CWorkThread*>* volatile child4;
    _reslist_node<CWorkThread*>* volatile child8;
    _reslist_node<CWorkThread*>* volatile childC;
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
    s16 scaledX = (s16)(convF32(sizeX) * lbl_eu_8066A278);
    s16 scaledY = (s16)(convF32(sizeY) * lbl_eu_8066A278);
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
    ml::FixStr<64> viewName = "(View)\0";
    PssCreateWalkFrame wf;
    viewName += pName;
    CView* view = CView::create(viewName.c_str(), pThread);
    view->wkReplaceHasChild((s16)param3);
    view->attachRenderWork(this);
    // Hoist mWorkID so the inlined free-slot walk matches retail preload
    // schedule (loop offsets / beq 0x0c). Remaining r5/r8 and stack-home
    // coloring remains a Chaitin near-miss -- keep iterating in high-level C.
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
