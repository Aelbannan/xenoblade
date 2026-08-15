#include "monolib/core.hpp"
#include "monolib/work.hpp"
#include "monolib/math.hpp"
#include "monolib/util.hpp"
#include "monolib/device.hpp"
#include "monolib/lib.hpp"
#include <revolution/OS.h>
#include <revolution/VI.h>

#pragma push
#pragma auto_inline off
// Emit the standalone reslist<CWorkThread*> constructor for the retail symbol
// __ct__23reslist<P11CWorkThread>Fv. With -inline auto MWCC else folds the
// template default-ctor into every call site and never emits a global body.
template reslist<CWorkThread*>::reslist();

namespace {
    class CWorkRootThread : public CWorkThread {
    public:
        friend class CWorkRoot;

        CWorkRootThread(const char* pName, CWorkThread* pThread) : CWorkThread(pName, pThread, 32) {
            mThreadList1.reserve(mAllocHandle, 16);
            mThreadList2.reserve(mAllocHandle, 16);
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

    CWorkRootThread* CWorkRootThread::spInstance;
}

//Exit mode value that determines what to do when exit is called
CWorkRoot::ExitMode CWorkRoot::sExitMode;
CException* CWorkRoot::sException;
//Unused in release
CErrorWii CWorkRoot::sErrorWii;

void CWorkRoot::initialize(){
    sExitMode = EXIT_PROG_END;
    //Initialize the math library
    ml::math::initialize();
    //Initialize VI
    VIInit();
    //Create root thread, allocated from work memory
    CWorkRootThread* thread = new (CWorkThreadSystem::getWorkMem()) CWorkRootThread("CWorkRoot", nullptr);
    CWorkUtil::entryWork(thread, nullptr, false);
    CWorkRootThread::spInstance = thread;
}

void CWorkRoot::destroy(){
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

// The -ipa file pass inlines every header-inline helper into callers, but
// retail keeps call-containing helpers (isEvent3, isRunning, reslist members)
// as real bls and only inlines tiny leaf helpers (flag checks, iterator ops).
// dont_inline on the caller reproduces that split: leaf logic is written as
// raw field access below, everything else stays a bl (MWCC_REFERENCE 11272).
#pragma push
#pragma dont_inline on
#pragma optimize_for_size on
void CWorkRoot::standbyWork(CWorkThread* pThread, bool arg1){
    if(!(arg1 ^ pThread->isEvent3())){
        pThread->wkStandby();

        //Recursively call standbyWork on this thread's children. Retail keeps
        //the list sentinel in a volatile register and reloads it each
        //iteration (it is not live across the recursive call), so re-read it
        //in the loop condition instead of caching it in a local.
        _reslist_node<CWorkThread*>* node = pThread->mChildren.mStartNodePtr->mNext;
        while (node != pThread->mChildren.mStartNodePtr) {
            standbyWork(node->mItem, arg1);
            node = node->mNext;
        }
    }

    // Remove all child threads that are in the shutdown state;
    // restart the scan from the beginning after each removal.
    do {
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
#pragma pop

#pragma push
#pragma dont_inline on
#pragma optimize_for_size on
void CWorkRoot::updateWork(CWorkThread* pThread, bool arg1){
    if(!(arg1 ^ pThread->isEvent3())){
        if(pThread->isRunning()){
            // Raw flag reads matching the retail's inlined leaf checks. The
            // declaration order mirrors the retail register allocation (temp
            // r3, clean r4, intermediate r5, flags r6).
            bool temp;
            bool clean;
            bool intermediate;
            u32 flags;

            flags = pThread->mFlags;
            clean = true;
            intermediate = true;
            temp = (flags & CWorkThread::THREAD_FLAG_PAUSE) != 0 && (flags & CWorkThread::THREAD_FLAG_EVT4) != 0;
            if(!temp){
                temp = (flags & CWorkThread::THREAD_FLAG_EVT7) != 0 && (flags & CWorkThread::THREAD_FLAG_EVT9) == 0;
                if(!temp){
                    intermediate = false;
                    if((flags & CWorkThread::THREAD_FLAG_APPEXCEPTION) == 0){
                        clean = false;
                    }
                }
            }

            if(clean || (flags & CWorkThread::THREAD_FLAG_NO_EVENT) != 0){
                pThread->wkUpdate();
            }
        }

        //Recursively call updateWork on this thread's children (raw node walk).
        _reslist_node<CWorkThread*>* node = pThread->mChildren.mStartNodePtr->mNext;
        while (node != pThread->mChildren.mStartNodePtr) {
            updateWork(node->mItem, arg1);
            node = node->mNext;
        }
    }
}
#pragma pop

void CWorkRoot::standbyWork(){
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

void CWorkRoot::renderWork(){
    if(CDeviceGX::getInstance() != nullptr){
        CDeviceGX::onRenderWork();
    }

    if(!CWorkSystem::isOff()){
        if(CViewRoot::getInstance() != nullptr){
            CViewRoot::renderView();
        }

        if(sException != nullptr){
            sException->wkRender();
        }
    }

    CDeviceFontLayer::func_80454E6C();
}

bool CWorkRoot::isShutdownAll(){
    return hasChild(CWorkRootThread::spInstance) ? false : true;
}

bool CWorkRoot::runSingle(){
    //Trigger the start frame event in CDeviceClock
    if(CDeviceClock::getInstance() != nullptr){
        CDeviceClock::onStartFrame();
    }

    if(CDeviceVI::getInstance() != nullptr){
        CDeviceVI::beginFrame();
    }

    standbyWork();
    //Update all threads, starting from the root thread
    updateWork(CWorkRootThread::spInstance, false);
    renderWork();

    if(CDeviceVI::getInstance() != nullptr){
        CDeviceVI::endFrame();
    }

    return isShutdownAll() ? false : true;
}

void CWorkRoot::exit(){
    //Check the current exit mode to determine how to handle the program stopping
    if(sExitMode == EXIT_WII_MENU){
        //Exit to Wii menu
        returnToWiiMenu(true);
    }else if(sExitMode == EXIT_RESTART){
        //Restart the game
        resetGame(true);
    }else if(sExitMode == EXIT_SHUTDOWN){
        //Shutdown the console
        shutdownGame(true);
    }else{
        /* If still set to the default mode, just exit without doing anything.
        This probably was what was used during debugging. */
        OSReport("exit prog end\n");
    }
}

inline void CWorkRoot::initializeComponents(){
    mtl::MemManager::initialize();
    CErrorWii::initialize();
    CDevice::createRegions();
    CWorkThreadSystem::initialize();
    CWorkRoot::initialize();
    CWorkControl::create(CWorkRootThread::spInstance);
    CDevice::create();
    CLib::create();
    CWorkSystem::create();
}

inline void CWorkRoot::destroyComponents(){
    CWorkRoot::destroy();
    CWorkThreadSystem::destroy();
    CDevice::deleteRegions();
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
    } while(CDevice::getInstance() == nullptr || CDevice::isColdStartReady());

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
    CDeviceVI::onPreRetrace();
}

void CWorkRoot::setException(CException* pException){
    //Why not just = pException??
    sException = pException != nullptr ? pException : nullptr;
}

CException* CWorkRoot::getException(){
    return sException;
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

#pragma pop
