#include "monolib/core/CViewRoot.hpp"
#include "monolib/core/CException.hpp"
#include "monolib/work.hpp"
#include "monolib/math.hpp"
#include "monolib/util.hpp"
#include "monolib/lib.hpp"
#include "monolib/data_vtables.hpp"
#include <revolution/OS.h>
#include <revolution/VI.h>

// Device methods referenced by this TU, declared by retail mangled name
// instead of including monolib/device.hpp: device.hpp pulls in
// CDeviceBase.hpp whose inline virtual dtor makes MWCC declare the __RTTI__
// chain internally, which collides with the extern "C" void* __RTTI__
// declarations this TU needs for its manual RTTI base list (MWCC "illegal
// name overloading").

// Out-of-line isEvent3: retail calls mMsgQueue.find (noinline chain) rather
// than inlining it; the header body would fold the find loop into callers.
extern "C" int find__12CMsgParamILi8ECFUl(const void* queue, u32 msg);
bool CWorkThread::isEvent3() const {
    if (mFlags & THREAD_FLAG_EVT3)
        return true;
    return find__12CMsgParamILi8ECFUl(&mMsgQueue, EVT_3) >= 0;
}


// Out-of-line CMsgParam<8>::find — the retail keeps a standalone symbol
// (find__12CMsgParam<8>CFUl) that callers bl to; the header body would
// inline it under -ipa.
template <int N>
__declspec(noinline) int CMsgParam<N>::find(u32 msg) const {
    for (int i = 0; i < mSize; i++) {
        if (mArrayPtr[(mFront + i) % mCapacity].command == msg) {
            return i;
        }
    }
    return -1;
}
template int CMsgParam<8>::find(u32) const;

class CDevice;
class CDeviceClock;
class CDeviceVI;
class CDeviceGX;
class CDeviceFontLayer;
extern "C" CDeviceClock* getInstance__12CDeviceClockFv();
extern "C" void onStartFrame__12CDeviceClockFv();
extern "C" CDeviceVI* getInstance__9CDeviceVIFv();
extern "C" void beginFrame__9CDeviceVIFv();
extern "C" void endFrame__9CDeviceVIFv();
extern "C" void onPreRetrace__9CDeviceVIFv();
extern "C" CDeviceGX* getInstance__9CDeviceGXFv();
extern "C" void onRenderWork__9CDeviceGXFv();
extern "C" void func_80454E6C__16CDeviceFontLayerFv();
extern "C" CDevice* getInstance__7CDeviceFv();
extern "C" bool isColdStartReady__7CDeviceFv();
extern "C" void createRegions__7CDeviceFv();
extern "C" void create__7CDeviceFv();
extern "C" void deleteRegions__7CDeviceFv();

extern IWorkEventVtbl lbl_eu_8056B938;

// CWorkThread's own virtuals referenced by the manual vtable (declared here,
// not in data_vtables.hpp: that header is included by TUs that also include
// device.hpp/CDeviceBase.hpp, where these would collide with the members).
extern "C" void wkStandbyLogin__11CWorkThreadFv();

// CWorkRootThread's generated deleting-dtor and virtual override, declared by
// mangled name so the manual vtable below can reference them.
extern "C" void __dt__Q217CWorkRootThreadNS15CWorkRootThreadFv();
extern "C" bool wkStandbyLogout__Q217CWorkRootThreadNS15CWorkRootThreadFv();

#pragma push
#pragma auto_inline off
// Emit the standalone reslist<CWorkThread*> constructor for the retail symbol
// __ct__23reslist<P11CWorkThread>Fv. With -inline auto MWCC else folds the
// template default-ctor into every call site and never emits a global body;
// instantiate BEFORE any use so the out-of-line copy is authoritative.
template reslist<CWorkThread*>::reslist();
#pragma pop

// CWorkRootThread is kept in a named namespace (retail mangles it as
// @unnamed@CWorkRoot_cpp@) because MWCC cannot spell a destructor address for
// an anonymous-namespace class in a manual vtable initializer; the coordinator
// renames the three member symbols below back to their @unnamed@ retail names
// (same mechanism as the existing CWorkRoot.o exact_renames).
namespace CWorkRootThreadNS {
    class __declspec(novtable) CWorkRootThread : public CWorkThread {
    public:
        friend class CWorkRoot;

        CWorkRootThread(const char* pName, CWorkThread* pThread) : CWorkThread(pName, pThread, 32) {
            *(void**)this = &lbl_eu_8056B938;
            mThreadList1.reserve(mAllocHandle, 16);
            mThreadList2.reserve(mAllocHandle, 16);
        }

        static void create(const char* pName, CWorkThread* pThread){
            CWorkRootThread* thread = new (CWorkThreadSystem::getWorkMem()) CWorkRootThread(pName, pThread);
            CWorkUtil::entryWork(thread, nullptr, false);
            spInstance = thread;
        }

        static CWorkRootThread* getInstance(){
            return spInstance;
        }

        virtual bool wkStandbyLogout(){
            if(hasChild(this)) return false;
            return CWorkThread::wkStandbyLogout(); //Call base
        }

        //0x0: vtable
        //0x0-1C4: CWorkThread
        reslist<CWorkThread*> mThreadList1; //0x1C4
        reslist<CWorkThread*> mThreadList2; //0x1E4
        u32 unk204;

    private:
        static CWorkRootThread* spInstance;
    };
}  // namespace CWorkRootThreadNS

// spInstance static (retail 0x80665608): the CWorkRoot .sbss split range now
// includes 0x80665608-0x80665618 (16B) so the singleton storage is emitted;
// UNIT_RULES["CWorkRoot.o"] renames the NS-mangled symbol to the retail
// @unnamed@ name (spInstance__Q223@unnamed@CWorkRoot_cpp@15CWorkRootThread),
// resolving the 13 .text references at link.
using CWorkRootThreadNS::CWorkRootThread;

CWorkRootThread* CWorkRootThread::spInstance;

// Data owned by this TU (blob monolibdata1d/monolibdata1 dissolve):
//   lbl_eu_80522718 (.rodata) = CWorkRootThread RTTI name string
//   lbl_eu_80522744 (.rodata) = exit-string pool (retail bytes, 0x4C)
//   lbl_eu_806635C0 (.sdata)  = __RTTI__CWorkRootThread {name, base-list}
//   lbl_eu_8056B938 (.data)   = CWorkRootThread vtable
//   lbl_eu_8056B9D8 (.data)   = RTTI base list {IWE,0,CWT,0,0,0}
//   lbl_eu_8066560C (.sbss)   = CWorkRoot::sExitMode (exit-mode u32)
//   lbl_eu_80665610 (.sbss)   = CWorkRoot::sException (CException*; 8 bytes, word 0 used)
// RTTI name string (.rodata 0x80522718, 0x2C bytes: 34 content + 10 pad).
// align(4) so the object is not padded to a 16-byte slot; the retail object
// ends at 0x80522744 where the exit-string pool starts.
extern "C" __declspec(align(4)) const char lbl_eu_80522718[0x2C] =
    {0x40,0x75,0x6E,0x6E,0x61,0x6D,0x65,0x64,0x40,0x43,0x57,0x6F,0x72,0x6B,0x52,0x6F,0x6F,0x74,0x5F,0x63,0x70,0x70,0x40,0x3A,0x3A,0x43,0x57,0x6F,0x72,0x6B,0x52,0x6F,0x6F,0x74,0x54,0x68,0x72,0x65,0x61,0x64,0x00,0x00,0x00,0x00};  /* "@unnamed@CWorkRoot_cpp@::CWorkRootThread\0\0\0\0" */
extern "C" const char lbl_eu_80522744[0x4C] =
    "CWorkRoot\0"
    "exit wii menu\n\0"
    "exit wii reset\n\0"
    "exit wii power off\n\0"
    "exit prog end\n";

extern IWorkEventVtbl lbl_eu_8056B938;
extern RttiBaseList2 lbl_eu_8056B9D8;

// __RTTI__CWorkRootThread = {name, base-list} (.sdata).
u32 lbl_eu_806635C0[2] = {(u32)lbl_eu_80522718, (u32)&lbl_eu_8056B9D8};

IWorkEventVtbl lbl_eu_8056B938 = {
    (u32)&lbl_eu_806635C0, 0, (u32)&__dt__Q217CWorkRootThreadNS15CWorkRootThreadFv,
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
    (u32)&wkRenderAfter__11CWorkThreadFv, (u32)&wkStandbyLogin__11CWorkThreadFv,
    (u32)&wkStandbyLogout__Q217CWorkRootThreadNS15CWorkRootThreadFv,
    (u32)&wkStandbyExceptionRetry__11CWorkThreadFUl,
};
RttiBaseList2 lbl_eu_8056B9D8 = {
    (u32)&__RTTI__10IWorkEvent, 0, (u32)&__RTTI__11CWorkThread, 0, 0, 0,
};

// CWorkRoot exit-mode / exception statics with the retail sda21 labels (blob
// monolibdata1d dissolve; CWorkRoot.hpp still declares the class statics but
// the retail addresses carry the lbl_eu_ names).
int lbl_eu_8066560C;
CException* lbl_eu_80665610[2];

//Exit mode value that determines what to do when exit is called
//Unused in release
CErrorWii CWorkRoot::sErrorWii;

__declspec(noinline) void CWorkRoot::initialize(){
    lbl_eu_8066560C = EXIT_PROG_END;
    //Initialize the math library
    ml::math::initialize();
    //Initialize VI
    VIInit();
    //Create root thread
    CWorkRootThread::create("CWorkRoot", nullptr);
}

__declspec(noinline) void CWorkRoot::destroy(){
    VISetPreRetraceCallback(nullptr);
    if(CWorkRootThread::spInstance != nullptr){
        delete CWorkRootThread::spInstance;
        CWorkRootThread::spInstance = nullptr;
    }
}

/* Adds a thread as a child to another thread. If the parent thread is null, it is instead added
as a child to the root thread as long as it isn't null. */
void CWorkRoot::entryWork(CWorkThread* pChild, CWorkThread* pParent, bool prepend){
    if(pParent != nullptr){
        pParent->wkEntryChild(pChild, prepend);
    }else if(CWorkRootThread::spInstance != nullptr){
        CWorkRootThread::spInstance->wkEntryChild(pChild, prepend);
    }
}

//Forces isRunning inline to emit here
bool CWorkRoot::dummy1(CWorkThread* pThread){
    return pThread->isRunning();
}

__declspec(noinline) void CWorkRoot::standbyWork(CWorkThread* pThread, bool arg1){
    reslist<CWorkThread*>* children = &pThread->mChildren;
    
    //Something is sus here
    if(!(arg1 ^ pThread->isEvent3())){
        pThread->wkStandby();

        //Recursively call standbyWork on this thread's children
        for(reslist<CWorkThread*>::iterator it = children->begin(); it != children->end(); it++){
            CWorkThread* childThread = *it;
            standbyWork(childThread, arg1);
        }
    }

    // Remove all child threads that are in the shutdown state;
    // restart the scan from the beginning after each removal.
    do {
        // Declaration order controls register allocation:
        // sentinel(r3), node(r4), foundShutdownThread(r5).
        _reslist_node<CWorkThread*>* sentinel = pThread->mChildren.mStartNodePtr;
        _reslist_node<CWorkThread*>* node = sentinel->mNext;
        bool foundShutdownThread = false;
        
        while (node != sentinel) {
            CWorkThread* childThread = node->mItem;
            if (childThread->mState == CWorkThread::THREAD_STATE_SHUTDOWN) {
                pThread->wkRemoveChild(childThread);
                if (childThread != nullptr) {
                    delete childThread;
                }
                foundShutdownThread = true;
                break;
            }
            node = node->mNext;
        }
        
        if (!foundShutdownThread) break;
    } while (true);
}

__declspec(noinline) void CWorkRoot::updateWork(CWorkThread* pThread, bool arg1){
    if(!(arg1 ^ pThread->isEvent3())){
        if(pThread->isRunning()){
            bool r4 = !(pThread->isPaused() || pThread->isEvent7() || pThread->isAppException());

            if(r4 || pThread->isNoEvent()){
                pThread->wkUpdate();
            }
        }

        reslist<CWorkThread*>* children = &pThread->mChildren;

        //Recursively call updateWork on this thread's children
        for(reslist<CWorkThread*>::iterator it = children->begin(); it != children->end(); it++){
            CWorkThread* childThread = *it;
            updateWork(childThread, arg1);
        }
    }
}

__declspec(noinline) void CWorkRoot::standbyWork(){
    CWorkRootThread* thread = CWorkRootThread::getInstance();
    _reslist_node<CWorkThread*>* iter;
    int count;
    _reslist_node<CWorkThread*>* sentinel;
    _reslist_node<CWorkThread*>* firstNode;
    
    // Multi-exit loop: count items at the bottom, process at the top.
    // Entry jumps to the count check; body falls through to recount.
    goto check;
    
body:
    {
        // Declare list2Front first (gets r29) and list1Front second (gets r30)
        // to match retail register allocation.
        CWorkThread* list2Front = thread->mThreadList2.mStartNodePtr->mNext->mItem;
        CWorkThread* list1Front = firstNode->mItem;
        
        thread->mThreadList1.pop_front();
        thread->mThreadList2.pop_front();
        
        if (list1Front->isRunning() && list2Front->isRunning()) {
            list1Front->mParent->wkRemoveChild(list1Front);
            list2Front->mParent = list1Front;
        }
    }
    
check:
    sentinel = thread->mThreadList1.mStartNodePtr;
    firstNode = sentinel->mNext;
    iter = firstNode;
    count = 0;
    while (iter != sentinel) {
        iter = iter->mNext;
        count++;
    }
    if (count != 0) goto body;
    
    thread->mThreadList1.clearList();
    thread->mThreadList2.clearList();
    standbyWork(CWorkRootThread::getInstance(), false);
}

__declspec(noinline) void CWorkRoot::renderWork(){
    if(getInstance__9CDeviceGXFv() != nullptr){
        onRenderWork__9CDeviceGXFv();
    }

    if(!CWorkSystem::isOff()){
        if(CViewRoot::getInstance() != nullptr){
            CViewRoot::renderView();
        }

        if(lbl_eu_80665610[0] != nullptr){
            lbl_eu_80665610[0]->wkRender();
        }
    }

    func_80454E6C__16CDeviceFontLayerFv();
}

bool CWorkRoot::isShutdownAll(){
    return hasChild(CWorkRootThread::spInstance) ? false : true;
}

__declspec(noinline) bool CWorkRoot::runSingle(){
    //Trigger the start frame event in CDeviceClock
    if(getInstance__12CDeviceClockFv() != nullptr){
        onStartFrame__12CDeviceClockFv();
    }

    if(getInstance__9CDeviceVIFv() != nullptr){
        beginFrame__9CDeviceVIFv();
    }

    standbyWork();
    //Update all threads, starting from the root thread
    updateWork(CWorkRootThread::spInstance, false);
    renderWork();

    if(getInstance__9CDeviceVIFv() != nullptr){
        endFrame__9CDeviceVIFv();
    }

    return isShutdownAll() ? false : true;
}

__declspec(noinline) void CWorkRoot::exit(){
    //Check the current exit mode to determine how to handle the program stopping
    if(lbl_eu_8066560C == EXIT_WII_MENU){
        //Exit to Wii menu
        OSReport(&lbl_eu_80522744[0xA]);
        OSReturnToMenu();
    }else if(lbl_eu_8066560C == EXIT_RESTART){
        //Restart the game
        OSReport(&lbl_eu_80522744[0x19]);
        OSRestart(0);
    }else if(lbl_eu_8066560C == EXIT_SHUTDOWN){
        //Shutdown the console
        OSReport(&lbl_eu_80522744[0x29]);
        OSShutdownSystem();
    }else{
        /* If still set to the default mode, just exit without doing anything.
        This probably was what was used during debugging. */
        OSReport(&lbl_eu_80522744[0x3D]);
    }
}

inline void CWorkRoot::initializeComponents(){
    mtl::MemManager::initialize();
    CErrorWii::initialize();
    createRegions__7CDeviceFv();
    CWorkThreadSystem::initialize();
    CWorkRoot::initialize();
    CWorkControl::create(CWorkRootThread::spInstance);
    create__7CDeviceFv();
    CLib::create();
    CWorkSystem::create();
}

inline void CWorkRoot::destroyComponents(){
    CWorkRoot::destroy();
    CWorkThreadSystem::destroy();
    deleteRegions__7CDeviceFv();
    CErrorWii::destroy();
    mtl::MemManager::finalize();
}

//Main function
void CWorkRoot::run(){
    //Initialize components
    initializeComponents();
    
    //Wait for all devices to be initialized?
    do {
        standbyWork();
    } while(getInstance__7CDeviceFv() == nullptr || isColdStartReady__7CDeviceFv());

    //Set pre retrace callback
    VISetPreRetraceCallback(preRetraceCallback);

    //Run the main update function
    while(runSingle() != false){
    }

    //Cleanup components
    destroyComponents();

    //Stop the program
    exit();
}

void CWorkRoot::preRetraceCallback(u32 retraceCount){
    /* This function is stubbed, so there's no way of knowing if the parameter got passed in
    or not */
    onPreRetrace__9CDeviceVIFv();
}

void CWorkRoot::setException(CException* pException){
    //Why not just = pException??
    lbl_eu_80665610[0] = pException != nullptr ? pException : nullptr;
}

CException* CWorkRoot::getException(){
    return lbl_eu_80665610[0];
}

// Out-of-line definition to match retail bytecode.
// Inlines the exception check from isException() rather than calling it.
bool CWorkThread::isRunning() const {
    bool exception;
    if (mFlags & THREAD_FLAG_EXCEPTION) {
        exception = true;
    } else {
        exception = (mMsgQueue.find(EVT_EXCEPTION) >= 0);
    }

    bool result = false;
    if (!exception) {
        bool stateOK = (mState == THREAD_STATE_LOGIN || mState == THREAD_STATE_RUN);
        if (stateOK) {
            result = true;
        }
    }
    return result;
}
