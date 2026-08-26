// The shared reslist.hpp defines every method inline, which -ipa folds into
// every call site; retail monolib instead keeps these bodies out-of-line
// (call sites bl pop_front__23reslist / clearList__29_reslist_base /
// __dt__29_reslist_base). Include the header once under aliased names to
// satisfy its #pragma once, then define canonical-named mirrors below whose
// key methods stay out-of-line.
#define _reslist_node _hdr_reslist_node
#define _reslist_iterator _hdr_reslist_iterator
#define _reslist_base _hdr_reslist_base
#define reslist _hdr_reslist
#include "monolib/util/reslist.hpp"
#undef _reslist_node
#undef _reslist_iterator
#undef _reslist_base
#undef reslist

// Canonical mirrors (same names/mangling as the shared templates).
template <typename T>
struct _reslist_node{
    void setItem(const T& value){
        T* ptr = &mItem;
        if(ptr != nullptr) {
            try{
                *ptr = value;
            }catch(...){
                throw;
            }
        }
    }

    _reslist_node<T>* mNext; //0x0
    _reslist_node<T>* mPrev; //0x4
    T mItem; //0x8
};

template <typename T>
class _reslist_base{
public:
    _reslist_base(){
        mList = nullptr;
        mCapacity = 0;
        unk1C = false;
        mStartNodePtr = &mStartNode;
        mStartNodePtr->mNext = &mStartNode;
        mStartNodePtr->mPrev = mStartNode.mNext;
    }

    // Out-of-line (defined below): retail destroys these lists via a bare
    // "li r4, 0; bl __dt__29_reslist_base<Fv>" call.
    virtual ~_reslist_base();

    void func_8049CB6C(T* item){}

    void func_8049CB70(_reslist_node<T>* r4){
        r4->mNext = nullptr;
    }

    int findFirstEmptySlotIndex(){
        int i = 0;
        
        while(i < mCapacity){
            if(mList[i].mNext == nullptr) break;
            i++;
        }

        return i;
    }

    // Out-of-line (defined below); retail call sites bl this symbol.
    void clearList();

    _reslist_node<T>* mStartNodePtr; //0x4
    _reslist_node<T> mStartNode; //0x8
    _reslist_node<T>* mList; //0x14
    int mCapacity; //0x18
    bool unk1C; //0x1C
};

template <typename T>
class reslist;

template <typename T, typename Ref, typename Ptr>
class _reslist_iterator {
    friend class reslist<T>;
public:
    _reslist_iterator() : mNode(nullptr){}
    explicit _reslist_iterator(_reslist_node<T>* node) : mNode(node){}

    _reslist_iterator& operator++(){
        mNode = mNode->mNext;
        return *this;
    }

    _reslist_iterator operator++(int){
        _reslist_iterator temp = *this;
        mNode = mNode->mNext;
        return temp;
    }

    _reslist_iterator& operator--(){
        mNode = mNode->mPrev;
        return *this;
    }

    _reslist_iterator operator--(int){
        _reslist_iterator temp = *this;
        mNode = mNode->mPrev;
        return temp;
    }

    Ref operator*() const {
        return mNode->mItem;
    }

    Ptr operator->() const {
        return &operator*();
    }

    bool operator==(_reslist_iterator const& rhs) const {
        return mNode == rhs.mNode;
    }

    bool operator!=(_reslist_iterator const& rhs) const {
        return mNode != rhs.mNode;
    }

private:
    _reslist_node<T>* mNode; //0x0
};

// NOTE: unlike the shared header, this mirror declares NO reslist destructor:
// retail's reslist has none, so destroying a reslist member folds straight
// into the _reslist_base destructor call seen in retail bytecode.
template <typename T>
class reslist : public _reslist_base<T> {
public:
    typedef _reslist_iterator<T, T&, T*> iterator;

    // Out-of-line (defined below): retail call sites bl the standalone
    // __ct__23reslist<P11CWorkThread>Fv symbol instead of letting -ipa fold
    // the constructor into enclosing constructors.
    reslist();

    iterator begin() const {
        return iterator(this->mStartNodePtr->mNext);
    }
    iterator end() const {
        return iterator(this->mStartNodePtr);
    }

    T& front() {
        return *begin();
    }
    T& back() {
        return *begin();
    }

    const T& front() const {
        return *begin();
    }
    const T& back() const {
        return *begin();
    }

    u32 size() const {
        _reslist_node<T>* curNode;
        _reslist_node<T>* endNode;
        
        u32 length = 0;
        endNode = this->mStartNodePtr;
        curNode = this->mStartNodePtr->mNext;

        while(curNode != endNode){
            length++;
            curNode = curNode->mNext;
        }
        
        return length;
    }

    bool empty() const {
        return this->mStartNodePtr->mNext == this->mStartNodePtr;
    }

    // Out-of-line (defined below): retail keeps reserve as a standalone
    // symbol that call sites bl.
    void reserve(mtl::ALLOC_HANDLE handle, int capacity);

    iterator find(const T& item){
        return std::find(begin(), end(), item);
    }

    void push_front(const T& item){
        _reslist_node<T>* startNode = this->mStartNodePtr->mNext;
        int i = this->findFirstEmptySlotIndex();

        _reslist_node<T>* temp = &this->mList[i];
        temp->setItem(item);
        temp->mNext = startNode;
        temp->mPrev = startNode->mPrev;
        startNode->mPrev->mNext = temp;
        startNode->mPrev = temp;
    }

    void push_back(const T& item){
        _reslist_node<T>* startNode = this->mStartNodePtr;
        int i = this->findFirstEmptySlotIndex();

        _reslist_node<T>* temp = &this->mList[i];

        temp->setItem(item);
        temp->mNext = startNode;
        temp->mPrev = startNode->mPrev;
        startNode->mPrev->mNext = temp;
        startNode->mPrev = temp;
    }

    void remove(const T& item){
        _reslist_node<T>* curr;
        _reslist_node<T>* next;
        _reslist_node<T>* head;

        head = this->mStartNodePtr;
        curr = head->mNext;
        
        while(curr != head){
            next = curr->mNext;

            if(curr->mItem == item){
                _reslist_node<T>* prev = curr->mPrev;
                prev->mNext = next;
                next->mPrev = prev;
                curr->mNext = nullptr;
            }

            curr = next;
        }
    }

    // Out-of-line (defined below); retail call sites bl this symbol.
    void pop_front();

    void erase(iterator& it){
        _reslist_node<T>* r5;
        _reslist_node<T>* r4;
        _reslist_node<T>* node = it.mNode;
        
        r4 = node->mPrev;
        r5 = node->mNext;
        r4->mNext = r5;
        r5->mPrev = r4;
        node->mNext = nullptr;
    }

    void clear(){
        this->clearList();
    }

    void destroyList(){
        this->clearList();
        if (this->unk1C == false && this->mList != nullptr) {
            delete[](this->mList);
            this->mList = nullptr;
        }
        this->mCapacity = 0;
    }
};

// size: 0x20

template <typename T>
__declspec(noinline) void _reslist_base<T>::clearList(){
    _reslist_node<T>* r5 = mStartNodePtr->mNext;
    
    while (r5 != mStartNodePtr) {
        _reslist_node<T>* r4 = r5;
        r5 = r5->mNext;
        func_8049CB6C(&r4->mItem);
        func_8049CB70(r4);
    }

    mStartNodePtr->mNext = mStartNodePtr;
    mStartNodePtr->mPrev = mStartNodePtr;
}

template <typename T>
__declspec(noinline) _reslist_base<T>::~_reslist_base(){
    clearList();

    if (unk1C == false) DELETE_ARRAY(mList);
}

template <typename T>
__declspec(noinline) reslist<T>::reslist() : _reslist_base<T>() {}

template <typename T>
__declspec(noinline) void reslist<T>::reserve(mtl::ALLOC_HANDLE handle, int capacity) {
    this->mList = new (handle) _reslist_node<T>[capacity];

    for(int i = 0; i < capacity; i++){
        this->mList[i].mNext = nullptr;
    }

    this->mCapacity = capacity;
}

template <typename T>
__declspec(noinline) void reslist<T>::pop_front(){
    _reslist_node<T>* prevNode;
    _reslist_node<T>* nextNode;
    _reslist_node<T>* frontNode;

    frontNode = this->mStartNodePtr->mNext;
    nextNode = frontNode->mNext;
    prevNode = frontNode->mPrev;
    prevNode->mNext = nextNode;
    nextNode->mPrev = prevNode;
    frontNode->mNext = nullptr;
}

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
extern "C" bool isRunning__11CWorkThreadCFv(CWorkThread* thread);
extern "C" bool isEvent3__11CWorkThreadCFv(CWorkThread* thread);
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
// optimize_for_size on the class region: the generated ~CWorkRootThread()
// needs the retail stmw/lmw r30 save/restore prologue style.
namespace CWorkRootThreadNS {
    // Empty helper base: runs right after the CWorkThread base ctor but
    // BEFORE the reslist members are constructed, which reproduces the
    // retail constructor's vtable-store placement (novtable suppresses the
    // compiler's own vtable init).
    struct WorkRootVtableInit {
        WorkRootVtableInit(void* obj, u32 vtbl){
            *(u32*)obj = vtbl;
        }
    };

    class __declspec(novtable) CWorkRootThread : public CWorkThread, public WorkRootVtableInit {
    public:
        friend class CWorkRoot;

        CWorkRootThread(const char* pName, CWorkThread* pThread)
            : CWorkThread(pName, pThread, 32),
              WorkRootVtableInit(this, (u32)&lbl_eu_8056B938) {
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

        ~CWorkRootThread();

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

// Out-of-line definition inside optimize_for_size: gives the generated
// dtor the retail stmw/lmw save/restore style.
#pragma push
#pragma optimize_for_size on
CWorkRootThreadNS::CWorkRootThread::~CWorkRootThread(){}
#pragma pop

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

// optimize_for_size: gives initialize the retail stmw/lmw r30 save/restore
// prologue-epilogue style.
//
// OPEN ITEM (mirrors CWorkControl::create, same fixed-codegen shape): the
// body reaches structural 0 / size-exact / clean relocs, but retail colors
// the early string-pool pointer r31 and the new-expression result r30 while
// decomp is reversed (13 pure reg swaps). Ruled out here: declaration order
// (both ways), declare-then-assign split, const pointer, inline literal arg
// (sinks lis/addi past bl getWorkMem), explicit MemManager::allocate +
// pointer placement-new (extra EH null-path branch). Per the CWorkControl
// investigation, MWCC rematerializes the constant name web so its callee-
// saved color cannot be forced from source; needs witness-era EQUIVALENT_MATCH.
#pragma push
#pragma optimize_for_size on
__declspec(noinline) void CWorkRoot::initialize(){
    lbl_eu_8066560C = EXIT_PROG_END;
    //Initialize the math library
    ml::math::initialize();
    //Initialize VI
    VIInit();

    /* Retail folds CWorkRootThread::create into this function (handle
    operator new inlines down to a direct MemManager::allocate call whose
    result gets the standard new-expression null check). Spell it as the
    create() call over the string LITERAL - the literal lets -ipa fold the
    parameter through the inline and reproduce retail's scheduling exactly;
    the pooled @stringBase0 copy is paired to the retail pool label
    lbl_eu_80522744 via UNIT_RULES data_pool_patterns. */
    CWorkRootThread::create("CWorkRoot", nullptr);
}
#pragma pop

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

#pragma push
#pragma optimize_for_size on
__declspec(noinline) void CWorkRoot::standbyWork(CWorkThread* pThread, bool arg1){
    reslist<CWorkThread*>* children = &pThread->mChildren;
    
    //Something is sus here
    // Empty-then layout: makes MWCC emit "bne past the standby block" like
    // retail instead of beq-into-it.
    if(!(arg1 ^ isEvent3__11CWorkThreadCFv(pThread))){
        pThread->wkStandby();

        //Recursively call standbyWork on this thread's children
        for(reslist<CWorkThread*>::iterator it = children->begin(); it != children->end(); it++){
            CWorkThread* childThread = *it;
            standbyWork(childThread, arg1);
        }
    }

    // Remove all child threads that are in the shutdown state;
    // restart the scan from the beginning after each removal.
    // Declaration order controls register allocation:
    // sentinel(r3), node(r4), foundShutdownThread(r5).
    _reslist_node<CWorkThread*>* sentinel;
    _reslist_node<CWorkThread*>* node;
    bool foundShutdownThread;
    do {
        sentinel = pThread->mChildren.mStartNodePtr;
        node = sentinel->mNext;
        foundShutdownThread = false;
        
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
    } while (foundShutdownThread);
}
#pragma pop

// Uses the out-of-line extern forms of isEvent3/isRunning (like standbyWork):
// the header/TU bodies would fold under -ipa instead of emitting retail's bl.
// optimize_for_size: reproduces the retail stmw/lmw r29 save/restore style.
#pragma push
#pragma optimize_for_size on
__declspec(noinline) void CWorkRoot::updateWork(CWorkThread* pThread, bool arg1){
    // Early-return form: makes MWCC emit retail's xor./bne exit test.
    if(arg1 ^ isEvent3__11CWorkThreadCFv(pThread)){
        return;
    }
    if(isRunning__11CWorkThreadCFv(pThread)){
            // Idle threads and no-event-flagged threads tick themselves.
            bool busy = pThread->isPaused() || pThread->isEvent7() || pThread->isAppException();

            if(!busy || pThread->checkFlag(CWorkThread::THREAD_FLAG_NO_EVENT)){
                pThread->wkUpdate();
            }
    }

    //Recursively call updateWork on this thread's children
    for(reslist<CWorkThread*>::iterator it = pThread->mChildren.begin(); it != pThread->mChildren.end(); it++){
        CWorkThread* childThread = *it;
        updateWork(childThread, arg1);
    }
}
#pragma pop

#pragma push
#pragma optimize_for_size on
__declspec(noinline) void CWorkRoot::standbyWork(){
    CWorkRootThread* thread = CWorkRootThread::getInstance();
    _reslist_node<CWorkThread*>* iter;
    int count;
    _reslist_node<CWorkThread*>* sentinel;
    _reslist_node<CWorkThread*>* firstNode;

    // Multi-exit loop laid out like retail: entry jumps to the emptiness
    // probe below the handoff body; the probe counts list1's nodes by
    // walking a copy of the first-node pointer to the sentinel.
    goto check;

body:
    {
        CWorkThread* list2Front = thread->mThreadList2.mStartNodePtr->mNext->mItem;
        CWorkThread* list1Front = firstNode->mItem;

        thread->mThreadList1.pop_front();
        thread->mThreadList2.pop_front();

        if (isRunning__11CWorkThreadCFv(list1Front)) {
            if (isRunning__11CWorkThreadCFv(list2Front)) {
                list1Front->mParent->wkRemoveChild(list1Front);
                list2Front->mParent = list1Front;
            }
        }
    }

check:
    sentinel = thread->mThreadList1.mStartNodePtr;
    count = 0;
    firstNode = sentinel->mNext;
    iter = firstNode;
    while (iter != sentinel) {
        iter = iter->mNext;
        count++;
    }
    if (count != 0) goto body;

    thread->mThreadList1.clearList();
    thread->mThreadList2.clearList();
    standbyWork(CWorkRootThread::getInstance(), false);
}
#pragma pop

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
// Calls the standalone CMsgParam<8>::find symbol (like isEvent3) instead of
// letting -ipa fold the find loop into this body.
bool CWorkThread::isRunning() const {
    bool exception;
    if (mFlags & THREAD_FLAG_EXCEPTION) {
        exception = true;
    } else {
        exception = (find__12CMsgParamILi8ECFUl(&mMsgQueue, EVT_EXCEPTION) >= 0);
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
