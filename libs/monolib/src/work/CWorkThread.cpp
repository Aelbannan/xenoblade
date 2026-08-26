// Inline-empty ~IWorkEvent so MWCC elides the empty base-dtor call inside
// ~CWorkThread (retail has no bl __dt__10IWorkEventFv there).
#define IWORK_EVENT_INLINE_DTOR
#include "monolib/work.hpp"
#include "monolib/data_vtables.hpp"
#include <cstring>

// Manual vtable defined at end of file (so MWCC's jump-table emission for
// wkStandby lands first in .data, matching the retail symbol order); forward
// declared here for the ctor/dtor vptr stores.
extern IWorkEventVtbl lbl_eu_8056B110;
// CWorkThread's own virtuals referenced by the manual vtable (declared here,
// not in data_vtables.hpp: that header is included by TUs that also include
// device.hpp/CDeviceBase.hpp, where these would collide with the members).
extern "C" void wkStandbyLogin__11CWorkThreadFv();
extern "C" void wkStandbyLogout__11CWorkThreadFv();

// CDeviceClock methods referenced by this TU, declared by retail mangled name
// instead of including monolib/device.hpp: device.hpp pulls in CDeviceBase.hpp
// whose inline virtual dtor makes MWCC declare the __RTTI__ chain internally,
// which collides with the extern "C" void* __RTTI__ declarations this TU needs
// for its manual vtable (MWCC "illegal name overloading").
class CDeviceClock;
extern "C" CDeviceClock* getInstance__12CDeviceClockFv();
extern "C" bool isInitialized__12CDeviceClockFv();


// sbss data owned by this TU (blob monolibdata1d dissolve):
//   lbl_eu_80665594 (0x80665594, 4 bytes) = sAllocFlags (u32*)
//   lbl_eu_80665598 (0x80665598, 8 bytes) = sWorkThreads (CWorkThread**; word 0 in use)
// (lbl_eu_80665590 = sMemAvailable is defined in CWorkThreadSystem.cpp, its user.)
u32* lbl_eu_80665594;
CWorkThread** lbl_eu_80665598[2];

// Reslist/CMsgParam subobject vtables (blob monolibdata labels, data in
// port/data_defs.cpp). Referenced by the flat ctor below.
extern u32 lbl_eu_8056B1B0[];
extern u32 lbl_eu_8056B1BC[];
extern u32 lbl_eu_8056B1D4[];

// CWorkThreadSystem::sAllocHandle (sdata; defined at the bottom of this TU).
// Accessed by label because sAllocHandle is private and the flat ctor below
// is not a friend of CWorkThreadSystem.
extern u32 lbl_eu_8066351C;

// Read-write mirror of the private CMsgParam<8> tail (see CDevice.cpp): lets
// wkSetEvent/wkCheckTimeout/wkStandby hand-expand the self enqueue, which
// retail inlines at these sites only (the child sites stay out-of-line bl
// enqueue calls).
struct CWorkThreadQueueLayout {
    u8 pad[0x80];                 //0x00..0x7F
    void* queueVtbl;              //0x80
    CMsgParamEntry mEntries[8];   //0x84
    CMsgParamEntry* mArrayPtr;    //0x1A4
    u32 mFront;                   //0x1A8
    u32 mSize;                    //0x1AC
    u32 mCapacity;                //0x1B0
    u32 field6;                   //0x1B4
    u32 field7;                   //0x1B8
};

// Explicit specialization so the weak __dt__12CMsgParam<8>Fv instantiation is
// emitted here with the retail shape: null-this guard, the addic. address test
// on the inline mEntries array at +0x4, counter clears, then operator delete
// when the deleting-dtor flag is set.
template <>
CMsgParam<8>::~CMsgParam(){
    if (mEntries){
        mSize = 0;
        mFront = 0;
    }
}
// Explicit <8> specialization of enqueue. Structure notes for future matching:
// - entry must stay volatile (non-volatile uninitialized reads are UB-eliminated
//   by MWCC and the loads vanish).
// - The modulo needs signed operands ((int)(mFront + mSize) % (int)mCapacity)
//   or MWCC emits divwu instead of retail's divw.
// - The element reference form gives the retail stwux store-with-update.
// - Remaining residual (24 pure reg swaps) is Chaitin pool-order coloring:
//   retail colors the nine field values [r31,r12,r11,r10,r9,r8,r7,r6,r5] and
//   address temps [r29,r28,r30]; our pool yields the rotated assignment.
// noinline so wkSetEvent/wkSetEventChild emit bl calls to it like retail
// (with the plain body visible, MWCC -ipa inlines it at every call site).
template <>
__declspec(noinline) void CMsgParam<8>::enqueue(u32 msg){
    volatile CMsgParamEntry entry;
    u32 value4 = entry.wid;
    u32 value8 = entry.unk8;
    u32 valueC = entry.unkC;
    u32 value10 = entry.unk10;

    // Ring buffer index: (mFront + mSize) % mCapacity, entry stride 0x24.
    CMsgParamEntry& target = mArrayPtr[(int)(mFront + mSize) % (int)mCapacity];

    u32 value14 = entry.unk14;
    u32 value18 = entry.unk18;
    u32 value1C = entry.unk1C;
    u16 value20 = entry.unk20;
    u8 value22 = entry.unk22;

    target.command = msg;
    target.wid = value4;
    target.unk8 = value8;
    target.unkC = valueC;
    target.unk10 = value10;
    target.unk14 = value14;
    target.unk18 = value18;
    target.unk1C = value1C;
    target.unk20 = value20;
    target.unk22 = value22;
    target.unk23 = 0;

    mSize++;
    field6 = mSize - 1;
}

// Flat constructor under the retail mangled symbol (CRsrcData/CUICfManager
// idiom): a plain member definition makes MWCC emit an implicit IWorkEvent
// base-subobject vptr store (__vt__10IWorkEvent) that retail does not have;
// a free function gets no implicit base construction, so the whole init store
// sequence is spelled by hand in the retail order.
extern "C" void* __ct__11CWorkThreadFPCcP11CWorkThreadi(
    CWorkThread* self, const char* pName, CWorkThread* pParent, int capacity){
    // Vptr first, then FixStr<64> default-init (clear).
    *(void**)self = (void*)&lbl_eu_8056B110;
    self->mName.mString[0] = 0;
    self->mName.mLength = 0;

    self->mState = CWorkThread::THREAD_STATE_NONE;
    self->mWorkID = INVALID_WORK_ID;
    self->mType = CWorkThread::THREAD_CWORKTHREAD;
    self->mParent = pParent;

    // Inlined _reslist_base<CWorkThread*> ctor: base vtable, fields,
    // sentinel node links; then the reslist ctor overrides the vptr.
    *(void**)&self->mChildren = (void*)lbl_eu_8056B1D4;
    self->mChildren.mList = nullptr;
    self->mChildren.mCapacity = 0;
    self->mChildren.unk1C = false;
    self->mChildren.mStartNodePtr = &self->mChildren.mStartNode;
    self->mChildren.mStartNodePtr->mNext = &self->mChildren.mStartNode;
    self->mChildren.mStartNodePtr->mPrev = self->mChildren.mStartNode.mNext;
    *(void**)&self->mChildren = (void*)lbl_eu_8056B1BC;

    self->mFlags = 0;

    // Inlined CMsgParam<8>(0, &unk1BC) ctor.
    CWorkThreadQueueLayout* q = reinterpret_cast<CWorkThreadQueueLayout*>(self);
    q->queueVtbl = lbl_eu_8056B1B0;
    q->mCapacity = 8;
    q->mArrayPtr = q->mEntries;
    q->mSize = 0;
    q->mFront = 0;
    q->field6 = 0;
    self->unk1BC = 0;
    q->field7 = 0;

    self->mExceptionWorkID = INVALID_WORK_ID;

    // Retail defers the sdata load of sAllocHandle to the end of the init
    // sequence (cheap sda21 addressing).
    self->mAllocHandle = lbl_eu_8066351C;

    // FixStr::operator=(const char*): length then copy.
    self->mName = pName;
    self->mWorkID = CWorkThreadSystem::allocWID(self);

    // Hand-expanded mChildren.reserve(mAllocHandle, capacity): retail inlines
    // the array allocation + node-init loop (MWCC unrolls it by 8) here.
    if(capacity > 0){
        self->mChildren.mList = new (self->mAllocHandle) _reslist_node<CWorkThread*>[capacity];
        for(int i = 0; i < capacity; i++){
            self->mChildren.mList[i].mNext = nullptr;
        }
        self->mChildren.mCapacity = capacity;
    }

    // Retail inlines isEvent3 here as a direct EVT3 flag-bit test.
    if(pParent != nullptr && (pParent->mFlags & CWorkThread::THREAD_FLAG_EVT3)){
        self->mFlags |= CWorkThread::THREAD_FLAG_EVT3;
    }

    if(pParent != nullptr && (pParent->mFlags & CWorkThread::THREAD_FLAG_EVT4)){
        self->mFlags |= CWorkThread::THREAD_FLAG_EVT4;
    }

    if(pParent != nullptr && (pParent->mFlags & CWorkThread::THREAD_FLAG_PAUSE)){
        self->mFlags |= CWorkThread::THREAD_FLAG_PAUSE;
    }

    if(pParent != nullptr && (pParent->mFlags & CWorkThread::THREAD_FLAG_EVT7)){
        self->mFlags |= CWorkThread::THREAD_FLAG_EVT7;
    }

    if(pParent != nullptr && (pParent->mFlags & CWorkThread::THREAD_FLAG_EVT9)){
        self->mFlags |= CWorkThread::THREAD_FLAG_EVT9;
    }

    if(pParent != nullptr && (pParent->mFlags & CWorkThread::THREAD_FLAG_APPEXCEPTION)){
        self->mFlags |= CWorkThread::THREAD_FLAG_APPEXCEPTION;
    }

    if(pParent != nullptr && (pParent->mFlags & CWorkThread::THREAD_FLAG_NO_EVENT)){
        self->mFlags |= CWorkThread::THREAD_FLAG_NO_EVENT;
    }

    return self;
}

CWorkThread::~CWorkThread(){
    *(void**)this = &lbl_eu_8056B110;

    // Walk the child list without acting on the items (children are destroyed
    // through CWorkControl; retail leaves this loop empty).
    if(!mChildren.empty()){
        reslist<CWorkThread*>::iterator it = mChildren.begin();
        reslist<CWorkThread*>::iterator endIt = mChildren.end();
        for(; it != endIt; ++it){
            ;
        }
    }

    // Inlined CWorkThreadSystem::freeWID: clear this thread's allocation bit
    // and registration slot (retail references the sbss labels via sda21).
    lbl_eu_80665594[mWorkID / 32] &= ~(1 << (mWorkID % 32));
    ((CWorkThread**)lbl_eu_80665598)[mWorkID] = nullptr;

    // Member destructors (~CMsgParam<8> then ~reslist) are emitted here.
}

void CWorkThread::wkReplaceHasChild(int capacity){
    if(capacity > 0){
        // Inlined reslist::destroyList
        _reslist_node<CWorkThread*>* next = mChildren.mStartNodePtr->mNext;
        while(next != mChildren.mStartNodePtr){
            _reslist_node<CWorkThread*>* curr = next;
            next = curr->mNext;
            curr->mNext = nullptr;
        }
        mChildren.mStartNodePtr->mNext = mChildren.mStartNodePtr;
        mChildren.mStartNodePtr->mPrev = mChildren.mStartNodePtr;

        if(mChildren.unk1C == false && mChildren.mList != nullptr){
            delete[] mChildren.mList;
            mChildren.mList = nullptr;
        }
        mChildren.mCapacity = 0;

        // Inlined reslist::reserve
        mChildren.reserve(mAllocHandle, capacity);
    }
}

void CWorkThread::wkEntryChild(CWorkThread* pChild, bool prepend){
    if(prepend){
        //Add the new child at the start (inlined reslist::push_front)
        _reslist_node<CWorkThread*>* startNode = mChildren.mStartNodePtr->mNext;
        int i = mChildren.findFirstEmptySlotIndex();

        _reslist_node<CWorkThread*>* temp = &mChildren.mList[i];
        temp->setItem(pChild);
        temp->mNext = startNode;
        temp->mPrev = startNode->mPrev;
        startNode->mPrev->mNext = temp;
        startNode->mPrev = temp;
    }else{
        //Add the new child at the end (inlined reslist::push_back)
        _reslist_node<CWorkThread*>* startNode = mChildren.mStartNodePtr;
        int i = mChildren.findFirstEmptySlotIndex();

        _reslist_node<CWorkThread*>* temp = &mChildren.mList[i];
        temp->setItem(pChild);
        temp->mNext = startNode;
        temp->mPrev = startNode->mPrev;
        startNode->mPrev->mNext = temp;
        startNode->mPrev = temp;
    }

    CWorkThread* pParent = this;
    pChild->mParent = pParent;
}

void CWorkThread::wkRemoveChild(CWorkThread* pChild){
    // Retail only unlinks the first matching node (unlike reslist::remove, which walks all).
    // Loop shape matches retail: advance-at-top, head-check, then item compare.
    _reslist_node<CWorkThread*>* head = mChildren.mStartNodePtr;
    _reslist_node<CWorkThread*>* curr = head->mNext;
    goto check;

advance:
    curr = curr->mNext;
check:
    if(curr == head){
        goto done;
    }
    if(curr->mItem != pChild){
        goto advance;
    }

done:
    if(curr == head){
        return;
    }

    _reslist_node<CWorkThread*>* prev = curr->mPrev;
    _reslist_node<CWorkThread*>* next = curr->mNext;
    prev->mNext = next;
    next->mPrev = prev;
    curr->mNext = nullptr;
}

// Retail unrolls three descendant generations of this walk inline and leaves
// the fourth to the recursive call.
void CWorkThread::wkSetEvent(EVT evt){
    if(evt == EVT_NONE){
        mFlags |= THREAD_FLAG_NO_EVENT;
    }else{
        // Hand-expanded mMsgQueue.enqueue(evt) (retail inlines the self call).
        volatile CMsgParamEntry entry;

        // Ring buffer index: (mFront + mSize) % mCapacity, entry stride 0x24.
        CMsgParamEntry& target =
            reinterpret_cast<CWorkThreadQueueLayout*>(this)->mArrayPtr
                [(int)(reinterpret_cast<CWorkThreadQueueLayout*>(this)->mFront +
                       reinterpret_cast<CWorkThreadQueueLayout*>(this)->mSize) %
                 (int)reinterpret_cast<CWorkThreadQueueLayout*>(this)->mCapacity];

        u32 value4 = entry.wid;
        u32 value8 = entry.unk8;
        u32 valueC = entry.unkC;
        u32 value10 = entry.unk10;
        u32 value14 = entry.unk14;
        u32 value18 = entry.unk18;
        u32 value1C = entry.unk1C;
        u16 value20 = entry.unk20;
        u8 value22 = entry.unk22;

        target.command = evt;
        target.wid = value4;
        target.unk8 = value8;
        target.unkC = valueC;
        target.unk10 = value10;
        target.unk14 = value14;
        target.unk18 = value18;
        target.unk1C = value1C;
        target.unk20 = value20;
        target.unk22 = value22;
        target.unk23 = 0;

        reinterpret_cast<CWorkThreadQueueLayout*>(this)->mSize++;
        reinterpret_cast<CWorkThreadQueueLayout*>(this)->field6 =
            reinterpret_cast<CWorkThreadQueueLayout*>(this)->mSize - 1;
    }

    // Walk state declared up front; MWCC colors these like retail's unrolled
    // generations (node/item pointer pairs per generation).
    _reslist_node<CWorkThread*>* n1;
    _reslist_node<CWorkThread*>* n2;
    _reslist_node<CWorkThread*>* n3;
    _reslist_node<CWorkThread*>* n4;
    CWorkThread* c1;
    CWorkThread* c2;
    CWorkThread* c3;

    for(n1 = mChildren.mStartNodePtr->mNext;
        n1 != mChildren.mStartNodePtr; n1 = n1->mNext){
        c1 = n1->mItem;
        if(evt == EVT_NONE){
            c1->mFlags |= THREAD_FLAG_NO_EVENT;
        }else{
            c1->mMsgQueue.enqueue(evt);
        }

        for(n2 = c1->mChildren.mStartNodePtr->mNext;
            n2 != c1->mChildren.mStartNodePtr; n2 = n2->mNext){
            c2 = n2->mItem;
            if(evt == EVT_NONE){
                c2->mFlags |= THREAD_FLAG_NO_EVENT;
            }else{
                c2->mMsgQueue.enqueue(evt);
            }

            for(n3 = c2->mChildren.mStartNodePtr->mNext;
                n3 != c2->mChildren.mStartNodePtr; n3 = n3->mNext){
                c3 = n3->mItem;
                if(evt == EVT_NONE){
                    c3->mFlags |= THREAD_FLAG_NO_EVENT;
                }else{
                    c3->mMsgQueue.enqueue(evt);
                }

                for(n4 = c3->mChildren.mStartNodePtr->mNext;
                    n4 != c3->mChildren.mStartNodePtr; n4 = n4->mNext){
                    n4->mItem->wkSetEvent(evt);
                }
            }
        }
    }
}

// Same walk minus self-processing; retail unrolls three descendant generations
// inline (gen 1 with a hand-expanded enqueue, gens 2-3 with bl enqueue calls)
// and leaves the fourth to the recursive wkSetEvent call.
void CWorkThread::wkSetEventChild(EVT evt){
    volatile CMsgParamEntry entry;
    // Volatile struct copy: batched register copy at definition, memory-resident
    // (volatile) for the gen-1 reloads.
    volatile CMsgParamEntry msg = entry;

    _reslist_node<CWorkThread*>* n1;
    _reslist_node<CWorkThread*>* n2;
    _reslist_node<CWorkThread*>* n3;
    _reslist_node<CWorkThread*>* n4;
    CWorkThread* c1;
    CWorkThread* c2;
    CWorkThread* c3;

    for(n1 = mChildren.mStartNodePtr->mNext;
        n1 != mChildren.mStartNodePtr; n1 = n1->mNext){
        c1 = n1->mItem;
        if(evt == EVT_NONE){
            c1->mFlags |= THREAD_FLAG_NO_EVENT;
        }else{
            // Hand-expanded c1->mMsgQueue.enqueue(evt): retail only inlines the
            // self-style enqueue into the first generation.
            CWorkThreadQueueLayout* q = reinterpret_cast<CWorkThreadQueueLayout*>(c1);

            // Ring buffer index: (mFront + mSize) % mCapacity, entry stride 0x24.
            u32 sum = q->mFront + q->mSize;
            int idx = (int)(sum % q->mCapacity);
            CMsgParamEntry* pTarget = &q->mArrayPtr[idx];

            pTarget->command = evt;
            pTarget->wid = msg.wid;
            pTarget->unk8 = msg.unk8;
            pTarget->unkC = msg.unkC;
            pTarget->unk10 = msg.unk10;
            pTarget->unk14 = msg.unk14;
            pTarget->unk18 = msg.unk18;
            pTarget->unk1C = msg.unk1C;
            pTarget->unk20 = msg.unk20;
            pTarget->unk22 = msg.unk22;
            pTarget->unk23 = 0;

            q->mSize++;
            q->field6 = q->mSize - 1;
        }

        for(n2 = c1->mChildren.mStartNodePtr->mNext;
            n2 != c1->mChildren.mStartNodePtr; n2 = n2->mNext){
            c2 = n2->mItem;
            if(evt == EVT_NONE){
                c2->mFlags |= THREAD_FLAG_NO_EVENT;
            }else{
                c2->mMsgQueue.enqueue(evt);
            }

            for(n3 = c2->mChildren.mStartNodePtr->mNext;
                n3 != c2->mChildren.mStartNodePtr; n3 = n3->mNext){
                c3 = n3->mItem;
                if(evt == EVT_NONE){
                    c3->mFlags |= THREAD_FLAG_NO_EVENT;
                }else{
                    c3->mMsgQueue.enqueue(evt);
                }

                for(n4 = c3->mChildren.mStartNodePtr->mNext;
                    n4 != c3->mChildren.mStartNodePtr; n4 = n4->mNext){
                    CWorkThread* c4 = n4->mItem;
                    if(evt == EVT_NONE){
                        c4->mFlags |= THREAD_FLAG_NO_EVENT;
                    }else{
                        c4->mMsgQueue.enqueue(evt);
                    }

                    // Fifth generation: leaf recursive wkSetEvent calls.
                    for(_reslist_node<CWorkThread*>* n5 = c4->mChildren.mStartNodePtr->mNext;
                        n5 != c4->mChildren.mStartNodePtr; n5 = n5->mNext){
                        n5->mItem->wkSetEvent(evt);
                    }
                }
            }
        }
    }
}

bool CWorkThread::wkCheckTimeout(u32 arg0, bool arg1, const char* pMessage){
    // isInitialized is effectively static (never touches this).
    if(getInstance__12CDeviceClockFv() == nullptr ||
       (!isInitialized__12CDeviceClockFv() && !arg1)){
        return false;
    }

    if(mFlags & THREAD_FLAG_EVT1){
        return true;
    }

    // Only INIT/LOGIN/RUN (states 1..3) are live; anything else counts as timed out.
    if(mState != THREAD_STATE_INIT && mState != THREAD_STATE_LOGIN && mState != THREAD_STATE_RUN){
        return true;
    }

    if(arg0 == 0){
        // Retail inlines a full copy of wkSetEvent(EVT_1) here: hand-expanded self
        // enqueue, then three inlined descendant generations with the fourth left
        // to a real wkSetEvent call.
        CWorkThreadQueueLayout* q = reinterpret_cast<CWorkThreadQueueLayout*>(this);
        volatile CMsgParamEntry entry;

        // Ring buffer index: (mFront + mSize) % mCapacity, entry stride 0x24.
        u32 sum = q->mFront + q->mSize;
        int idx = (int)(sum % q->mCapacity);
        CMsgParamEntry* pTarget = &q->mArrayPtr[idx];

        pTarget->command = EVT_1;
        pTarget->wid = entry.wid;
        pTarget->unk8 = entry.unk8;
        pTarget->unkC = entry.unkC;
        pTarget->unk10 = entry.unk10;
        pTarget->unk14 = entry.unk14;
        pTarget->unk18 = entry.unk18;
        pTarget->unk1C = entry.unk1C;
        pTarget->unk20 = entry.unk20;
        pTarget->unk22 = entry.unk22;
        pTarget->unk23 = 0;

        q->mSize++;
        q->field6 = q->mSize - 1;

    CWorkThread* c3;
    CWorkThread* c2;
    CWorkThread* c1;
    _reslist_node<CWorkThread*>* n1;
    _reslist_node<CWorkThread*>* n2;
    _reslist_node<CWorkThread*>* n3;
    _reslist_node<CWorkThread*>* n4;

    for(n1 = mChildren.mStartNodePtr->mNext;
        n1 != mChildren.mStartNodePtr; n1 = n1->mNext){
        c1 = n1->mItem;
        c1->mMsgQueue.enqueue(EVT_1);

        for(n2 = c1->mChildren.mStartNodePtr->mNext;
            n2 != c1->mChildren.mStartNodePtr; n2 = n2->mNext){
            c2 = n2->mItem;
            c2->mMsgQueue.enqueue(EVT_1);

            for(n3 = c2->mChildren.mStartNodePtr->mNext;
                n3 != c2->mChildren.mStartNodePtr; n3 = n3->mNext){
                c3 = n3->mItem;
                c3->mMsgQueue.enqueue(EVT_1);

                for(n4 = c3->mChildren.mStartNodePtr->mNext;
                    n4 != c3->mChildren.mStartNodePtr; n4 = n4->mNext){
                    n4->mItem->wkSetEvent(EVT_1);
                }
            }
        }
    }

    CWorkUtil::dispTree(CWorkControl::getInstance(), 0);
    return true;
    }

    return arg1;
}

bool CWorkThread::wkIsCurrent() const{
    if(mParent != nullptr){
        return this == mParent->wkGetChild();
    }

    return true;
}

CWorkThread* CWorkThread::getWorkThread(WORK_ID wid){
    if(wid == INVALID_WORK_ID){
        return nullptr;
    }

    return CWorkThreadSystem::sWorkThreads[wid];
}

void CWorkThread::func_804385CC(u32){}

void CWorkThread::wkTimeoutInit(){
    (void)getInstance__12CDeviceClockFv();
}

bool CWorkThread::wkStandbyInit(){
    mState = THREAD_STATE_INIT;
    wkTimeoutInit();
    return true;
}

bool CWorkThread::wkStandbyRun(){
    if(isNoEvent()){
        mState = THREAD_STATE_RUN;
        wkTimeoutInit();
    }

    return mState == THREAD_STATE_RUN;
}

bool CWorkThread::wkStandbyShutdown(){
    mState = THREAD_STATE_SHUTDOWN;
    wkTimeoutInit();
    return true;
}

void CWorkThread::wkStandby(){
    // Hand-expanded queue access (retail inlines front/pop/find here with a
    // signed-div pop modulo; the shared header forms don't match those shapes).
    CWorkThreadQueueLayout* q = reinterpret_cast<CWorkThreadQueueLayout*>(this);

    mFlags &= 0xFFFF;

    while(q->mSize != 0){
        // front().command: ring slot (mFront % mCapacity), entry stride 0x24.
        u32 command = q->mArrayPtr[q->mFront % q->mCapacity].command;

        switch(command){
            case EVT_1:{
                mFlags |= THREAD_FLAG_EVT1;
                break;
            }

            case EVT_EXCEPTION:{
                mFlags |= THREAD_FLAG_EXCEPTION;
                mExceptionWorkID = q->mArrayPtr[q->mFront % q->mCapacity].wid;
                break;
            }

            case EVT_3:{
                mFlags |= THREAD_FLAG_EVT3;
                break;
            }

            case EVT_4:{
                mFlags |= THREAD_FLAG_EVT4;
                break;
            }

            case EVT_PAUSE:{
                mFlags |= THREAD_FLAG_PAUSE;
                OnPauseTrigger(true);
                break;
            }

            case EVT_UNPAUSE:{
                mFlags &= ~THREAD_FLAG_PAUSE;
                OnPauseTrigger(false);
                break;
            }

            case EVT_7:{
                if(!(mFlags & THREAD_FLAG_EVT9)){
                    mFlags |= THREAD_FLAG_EVT7;
                }
                break;
            }

            case EVT_8:{
                if(!(mFlags & THREAD_FLAG_EVT9)){
                    // Clears EVT7|EVT9 (retail mask drops both bits).
                    mFlags &= ~(THREAD_FLAG_EVT7 | THREAD_FLAG_EVT9);
                }
                break;
            }

            case EVT_APPEXCEPTION_ON:{
                mFlags |= THREAD_FLAG_APPEXCEPTION;
                OnPauseTrigger(true);
                break;
            }

            case EVT_APPEXCEPTION_OFF:{
                // Clears APPEXCEPTION|EVT7 (retail mask drops both bits).
                mFlags &= ~(THREAD_FLAG_APPEXCEPTION | THREAD_FLAG_EVT7);
                OnPauseTrigger(false);
                break;
            }

            case EVT_9:{
                mFlags |= THREAD_FLAG_EVT9;
                break;
            }
        }

        // pop(): signed-div modulo, size store between.
        q->mSize = q->mSize - 1;
        q->mFront = (int)(q->mFront + 1) % (int)q->mCapacity;
    }

    // Exception pending either as a live flag or as a queued EVT_EXCEPTION.
    bool exception;
    if(mFlags & THREAD_FLAG_EXCEPTION){
        exception = true;
    }else{
        // Inlined CMsgParam<8>::find(EVT_EXCEPTION) >= 0 ring scan.
        int findIdx = -1;
        for(u32 i = 0; i < q->mSize; i++){
            if(q->mArrayPtr[(q->mFront + i) % q->mCapacity].command == EVT_EXCEPTION){
                findIdx = (int)i;
                break;
            }
        }
        exception = findIdx >= 0;
    }

    if(!exception){
        switch(mState){
            case THREAD_STATE_NONE:{
                wkStandbyInit();

                //FALLTHROUGH
            }

            case THREAD_STATE_INIT:{
                if(isNoEvent()){
                    mState = THREAD_STATE_LOGIN;
                    wkTimeoutInit();
                } else if(!wkStandbyLogin()){
                    break;
                }

                //FALLTHROUGH
            }

            case THREAD_STATE_LOGIN:{
                if(!wkStandbyRun()){
                    break;
                }

                //FALLTHROUGH
            }

            case THREAD_STATE_RUN:{
                if(!wkStandbyLogout()){
                    break;
                }

                //FALLTHROUGH
            }

            case THREAD_STATE_LOGOUT:{
                wkStandbyShutdown();

                //FALLTHROUGH
            }

            case THREAD_STATE_SHUTDOWN:
            default:{
                break;
            }
        }
    }else{
        if(wkStandbyExceptionRetry(mExceptionWorkID)){
            // Inlined getWorkThread(WORK_ID): sbss table lookup.
            CWorkThread* pExceptionThread = nullptr;
            if(mExceptionWorkID != INVALID_WORK_ID){
                pExceptionThread = reinterpret_cast<CWorkThread**>(lbl_eu_80665598)[mExceptionWorkID];
            }

            if(pExceptionThread != nullptr){
                // Retail inlines wkSetEvent(EVT_NONE): self NO_EVENT flag first,
                // then three descendant generations (fourth stays recursive).
                pExceptionThread->mFlags |= THREAD_FLAG_NO_EVENT;

                _reslist_node<CWorkThread*>* n1;
                _reslist_node<CWorkThread*>* n2;
                _reslist_node<CWorkThread*>* n3;
                _reslist_node<CWorkThread*>* n4;
                CWorkThread* c1;
                CWorkThread* c2;
                CWorkThread* c3;

                for(n1 = pExceptionThread->mChildren.mStartNodePtr->mNext;
                    n1 != pExceptionThread->mChildren.mStartNodePtr; n1 = n1->mNext){
                    c1 = n1->mItem;
                    c1->mFlags |= THREAD_FLAG_NO_EVENT;

                    for(n2 = c1->mChildren.mStartNodePtr->mNext;
                        n2 != c1->mChildren.mStartNodePtr; n2 = n2->mNext){
                            c2 = n2->mItem;
                            c2->mFlags |= THREAD_FLAG_NO_EVENT;

                            for(n3 = c2->mChildren.mStartNodePtr->mNext;
                                n3 != c2->mChildren.mStartNodePtr; n3 = n3->mNext){
                                c3 = n3->mItem;
                                c3->mFlags |= THREAD_FLAG_NO_EVENT;

                                for(n4 = c3->mChildren.mStartNodePtr->mNext;
                                    n4 != c3->mChildren.mStartNodePtr; n4 = n4->mNext){
                                    n4->mItem->wkSetEvent(EVT_NONE);
                                }
                            }
                    }
                }
            }

            mExceptionWorkID = INVALID_WORK_ID;
            mFlags &= ~(THREAD_FLAG_EXCEPTION | THREAD_FLAG_3);
        }
    }
}

bool CWorkThread::wkStandbyLogin(){
    mState = THREAD_STATE_LOGIN;
    wkTimeoutInit();
    return true;
}

bool CWorkThread::wkStandbyLogout(){
    mState = THREAD_STATE_LOGOUT;
    wkTimeoutInit();
    return true;
}

void CWorkThread::wkUpdate(){}

#pragma dont_inline on
#pragma optimize_for_size on
CWorkThread* CWorkThread::getWorkThread(const char* name){
    // Retail tolerates a null `this` (checks r3 before any member access).
    if(this == NULL){
        return NULL;
    }

    // Retail: addi r3, this, 4; bl strcmp; cntlzw/srwi bool test.
    bool same = std::strcmp(reinterpret_cast<const char*>(&mName), name) == 0;
    if(same){
        return this;
    }

    // Head pointer is re-read from the member every iteration (retail reloads
    // it at 0x60(this) since the recursive call may invalidate caches).
    for(_reslist_node<CWorkThread*>* node = mChildren.mStartNodePtr->mNext;
        node != mChildren.mStartNodePtr; node = node->mNext){
        CWorkThread* result = node->mItem->getWorkThread(name);

        if(result != NULL && result->mState != THREAD_STATE_SHUTDOWN){
            return result;
        }
    }

    return NULL;
}
#pragma optimize_for_size off
#pragma dont_inline off

// Data owned by this TU (blob monolibdata1 dissolve). The CWorkThread vtable
// (lbl_eu_8056B110) is defined explicitly here because the class is novtable:
// MWCC would otherwise auto-emit __vt__11CWorkThread + the __RTTI__11CWorkThread
// base-list, which the retail does not carry in this split range.
IWorkEventVtbl lbl_eu_8056B110 = {
    (u32)&__RTTI__11CWorkThread, 0, (u32)&__dt__11CWorkThreadFv,
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
    (u32)&wkStandbyLogout__11CWorkThreadFv, (u32)&wkStandbyExceptionRetry__11CWorkThreadFUl,
};

// sAllocHandle (mtl::ALLOC_HANDLE, initialized to INVALID_HANDLE 0xFFFFFFFF)
// - blob monolibdata1 dissolve. CWorkThreadSystem.cpp declares it extern and
// is its user; the definition lives here (sdata lbl_eu_8066351C).
u32 lbl_eu_8066351C = 0xFFFFFFFF;
