#include "kyoshin/CUIWindowManager.hpp"

// --- CTTask<CUIWindowManager> out-of-line specializations ---
// The canonical declared-only template emits no bodies; these explicit
// specializations produce the retail standalone Move/Draw/dtor symbols.
template<>
void CTTask<CUIWindowManager>::Move() {
    if (mMoveFunc) {
        (static_cast<CUIWindowManager*>(this)->*mMoveFunc)();
    }
}

template<>
void CTTask<CUIWindowManager>::Draw() {
    if (mDrawFunc) {
        (static_cast<CUIWindowManager*>(this)->*mDrawFunc)();
    }
}

template<>
CTTask<CUIWindowManager>::~CTTask() {}

// Retail 34-char CTest instantiation: Move/Draw/dtor bodies identical to
// the CTTask<CUIWindowManager> ones above.
template<>
void CTTask<CUIWindowManager::CTest>::Move() {
    if (mMoveFunc) {
        (static_cast<CUIWindowManager::CTest*>(this)->*mMoveFunc)();
    }
}

template<>
void CTTask<CUIWindowManager::CTest>::Draw() {
    if (mDrawFunc) {
        (static_cast<CUIWindowManager::CTest*>(this)->*mDrawFunc)();
    }
}

template<>
CTTask<CUIWindowManager::CTest>::~CTTask() {}

// CUIWindowManager::CTest::~CTest is emitted below as the retail-mangled
// free function __dt__Q216CUIWindowManager5CTestFv.

#include <types.h>
#include <string.h>

// Batch 2026-07-14g: window-mgr-move owns Move() exclusively in this TU.

extern "C" {
CUIWindowManager* lbl_eu_80664088;
s16 lbl_eu_8066408C;

void func_8009D0B4();
void func_8009D514(cf::IFlagEvent*);

// C-ABI helpers used by the retail dtor/free-function shapes below.
extern void __dt__8CProcessFv(void* self, s32 flags);
extern void* __dl__FPv(void* p);
extern void* __dynamic_cast(void* obj, long offset, const void* srcType,
                            const void* dstType, void* src2dst);
extern const void* lbl_eu_80662170; // __dynamic_cast src typeinfo
extern const void* lbl_eu_80661EC8; // __dynamic_cast dst typeinfo
}

// Object type reached through the dynamic_cast in func_8013D1E8; only the
// +0xAC show flag is written.
struct CTaskWindowFlags {
    u8 unk000[0xAC];
    u32 field_0xAC; //0xAC - show flag
};

// Unrecovered view type referenced through IUIWindow::unk5C; only the +0x828
// timer field is used by CUIWindowManager::Move.
struct IUIWindowSubView {
    u8 unk000[0x828];
    s32 unk828; //0x828 - active transition/close timer
};

// Minimal recovery of the window queue item type (IUIWindow, per retail
// `reslist<P9IWindow>` / `__dt__9IUIWindowFv`). Only the fields touched by
// CUIWindowManager::Move are named; the rest is CTTask<IUIWindow>/CProcess.
class IUIWindow {
private:
    // Field order matters here: this private CTTask/CProcess prefix must
    // come first in declaration order so the public members below land at
    // their retail offsets (MWCC lays out members in declaration order,
    // independent of the access-specifier they fall under).
    u8 unk00[0x39];
    bool mIsRemove; //0x39 (CProcess::mIsRemove)
    u8 unk3A[0x5C - 0x3A];

public:
    void SetRemove() { mIsRemove = true; }

    IUIWindowSubView* unk5C; //0x5C - non-null: timer lives at unk5C->unk828
    s32 unk60;               //0x60 - fallback timer when unk5C is null
    bool unk64;              //0x64 - pending removal flag
    bool field_0x65;         //0x65 - pending update-mark flag
    bool field_0x66;         //0x66 - removal-mark (func_8013D26C)
    u8 pad67[0x68 - 0x67];   //0x67
    u32 field_0x68;          //0x68 - window id (matched by func_8013D07C)
};

typedef reslist<IUIWindow*>::iterator WindowIter;
typedef _reslist_node<IUIWindow*> WindowNode;

extern "C" void Term__Q216CUIWindowManager5CTestFv(){}
extern "C" void Move__Q216CUIWindowManager5CTestFv(){}

void CUIWindowManager::Term() {
    cf::IFlagEvent* flagEvent = this; // implicit MI conversion -- do not static_cast / ternary / if
    func_8009D0B4();
    func_8009D514(flagEvent);

    unk9C->SetRemove();
    lbl_eu_80664088 = NULL;
}

void CUIWindowManager::Move() {
    CUIWindowManager* inst = lbl_eu_80664088;
    if (inst == NULL) {
        return;
    }

    // Raw node walks for timer find (retail r7/r5/r4 shape).
    {
        WindowNode* sentinel = inst->mWindowList1.mStartNodePtr;
        for (WindowNode* n = sentinel->mNext; n != sentinel; n = n->mNext) {
            IUIWindow* window = n->mItem;
            s32 timer;
            if (window->unk5C == NULL) {
                timer = window->unk60;
            } else {
                timer = window->unk5C->unk828;
            }
            if (timer > 0) {
                lbl_eu_8066408C = (s16)timer;
                break;
            }
        }
    }
    {
        WindowNode* sentinel = inst->mWindowList2.mStartNodePtr;
        for (WindowNode* n = sentinel->mNext; n != sentinel; n = n->mNext) {
            IUIWindow* window = n->mItem;
            s32 timer;
            if (window->unk5C == NULL) {
                timer = window->unk60;
            } else {
                timer = window->unk5C->unk828;
            }
            if (timer > 0) {
                lbl_eu_8066408C = (s16)timer;
                break;
            }
        }
    }

    // Retail mark-all: search cursor (r6) walks; mark cursor (r7) stays at
    // list head until a hit, then marks the entire list from the head.
    {
        WindowNode* sentinel = mWindowList1.mStartNodePtr;
        WindowNode* mark = sentinel->mNext;
        WindowNode* search = mark;
        for (; search != sentinel; search = search->mNext) {
            if (search->mItem->field_0x65 != 0 || unkA1 != 0) {
                bool flag = true;
                for (; mark != mWindowList1.mStartNodePtr; mark = mark->mNext) {
                    mark->mItem->field_0x65 = flag;
                }
                break;
            }
        }
    }
    {
        WindowNode* sentinel = mWindowList2.mStartNodePtr;
        WindowNode* mark = sentinel->mNext;
        WindowNode* search = mark;
        for (; search != sentinel; search = search->mNext) {
            if (search->mItem->field_0x65 != 0 || unkA1 != 0) {
                bool flag = true;
                for (; mark != mWindowList2.mStartNodePtr; mark = mark->mNext) {
                    mark->mItem->field_0x65 = flag;
                }
                break;
            }
        }
    }

    WindowNode* node;
    WindowNode* pending[18];
    int pendingCount;
    int i;
    WindowNode* prev;
    WindowNode* next;

    pendingCount = 0;
    for (node = mWindowList1.mStartNodePtr->mNext; node != mWindowList1.mStartNodePtr; node = node->mNext) {
        IUIWindow* window = node->mItem;
        if (window->unk64 != 0 || unkA0 != 0) {
            pending[pendingCount++] = node;
            window->SetRemove();
        }
    }
    for (i = 0; i < pendingCount; i++) {
        prev = pending[i]->mPrev;
        next = pending[i]->mNext;
        prev->mNext = next;
        next->mPrev = prev;
        pending[i]->mNext = NULL;
        pending[i] = (WindowNode*)0;
    }

    pendingCount = 0;
    for (node = mWindowList2.mStartNodePtr->mNext; node != mWindowList2.mStartNodePtr; node = node->mNext) {
        IUIWindow* window = node->mItem;
        if (window->unk64 != 0 || unkA0 != 0) {
            pending[pendingCount++] = node;
            window->SetRemove();
        }
    }
    for (i = 0; i < pendingCount; i++) {
        prev = pending[i]->mPrev;
        next = pending[i]->mNext;
        prev->mNext = next;
        next->mPrev = prev;
        pending[i]->mNext = NULL;
        pending[i] = (WindowNode*)0;
    }

    unkA0 = false;
    unkA1 = false;
}

extern "C" u32 func_8013C54C() {
    return (u32)lbl_eu_80664088;
}
// Window-queue sweep (retail func_8013D26C): walk the secondary queue. With
// mode == 0 mark each window for removal (mIsRemove + field_0x66) and collect
// the nodes; with mode != 0 just set the update-mark (field_0x65). Collected
// nodes are unlinked from the list (MWCC auto-unrolls the unlink loop by 8).
void func_8013D26C(int mode) {
    CUIWindowManager* inst = lbl_eu_80664088;
    if (inst == NULL) {
        return;
    }

    WindowNode* pending[18];
    int count = 0;
    WindowNode* sentinel = inst->mWindowList2.mStartNodePtr;
    for (WindowNode* n = sentinel->mNext; n != sentinel; n = n->mNext) {
        if (mode == 0) {
            pending[count++] = n;
            n->mItem->field_0x66 = 1;
            n->mItem->SetRemove();
        } else {
            n->mItem->field_0x65 = 1;
        }
    }

    if (count > 0) {
        for (int i = 0; i < count; i++) {
            WindowNode* node = pending[i];
            WindowNode* prev = node->mPrev;
            WindowNode* next = node->mNext;
            prev->mNext = next;
            next->mPrev = prev;
            node->mNext = NULL;
        }
    }
}
extern "C" unsigned short func_8013EC58()
{
    return lbl_eu_8066408C;
}
extern "C" void func_8013EC60() {
    lbl_eu_8066408C = 0;
}
void func_8013EC6C(){}
extern "C" void Draw__Q216CUIWindowManager5CTestFv(void* self) {}
extern "C" void Init__Q216CUIWindowManager5CTestFv(void* self) {}
extern "C" void* func_8013F234(void* self) {
    extern void* func_8013CBB4(void*);
    return func_8013CBB4((char*)self - 0x54);
}
void __dt__16CUIWindowManagerFv(CUIWindowManager*);
extern "C" void func_8013F23C(CUIWindowManager* p) {
    __dt__16CUIWindowManagerFv((CUIWindowManager*)((char*)p - 0x54));
}

// Retail dtor: destroys the two reslist window queues (the reslist dtors
// inline to the vtable-store + clear + free shape), then the CProcess base
// and the delete-this guard driven by the r4 delete flag.
// The IFlagEvent base dtor body must be visible in this TU so MWCC inlines
// it (retail emits no IFlagEvent dtor call here).
namespace cf {
IFlagEvent::~IFlagEvent() {}
}
CUIWindowManager::~CUIWindowManager() {}
extern "C" void func_8013F2A0(CUIWindowManager* self);
extern "C" void func_8013F3EC(CUIWindowManager* self) { func_8013F2A0(self); }
extern "C" int func_801413DC(unsigned int arg0, int arg1) { unsigned int low = arg0 & 0xffff; unsigned int high = arg0 >> 16; if (arg1 >= (int)low) return -1; return (int)(high + arg1); }

// Singleton ctor: CProcess base first, then the complete-object vtable, the
// IFlagEvent sub-vtable, the scene, and the two window queues (each reslist
// default-constructs its sentinel, then reserves 8 node slots from the
// scene's alloc handle). Closes with the global timer-id reset.
CUIWindowManager::CUIWindowManager(CScn* pScene, mtl::ALLOC_HANDLE mHandle)
    : unk58(pScene) {
    unk9C = NULL;
    unkA0 = false;
    unkA1 = false;

    mWindowList1.reserve(func_80496004(pScene), 8);
    mWindowList2.reserve(func_80496004(pScene), 8);

    func_8015D0B8();
    func_80122460();
    func_801B29E0();
    func_8012BDD0();
    lbl_eu_8066408C = 0;
}
// Retail dtor of the CTTask<CUIWindowManager> base (flat retail mangling):
// run the CProcess base dtor behind a null guard, then free the object when
// the delete flag is positive.
extern "C" void* __dt__CTTask_CUIWindowManager(void* self, s32 flags) {
    if (self != NULL) {
        __dt__8CProcessFv(self, 0);
        if (flags > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}
void func_8013CBB4(){}
// Retail keeps a flat unmangled ctor symbol (__ct__CUIWindowManager); route
// the creator's call through this shim so the call reloc binds to the literal
// retail name. The recursive null path is unreachable from the creator (the
// allocation is null-checked first) and exists only to keep the shim outlined.
extern "C" void* __ct__CUIWindowManager(CUIWindowManager* self, CScn* pScene,
                                        mtl::ALLOC_HANDLE mHandle) {
    if (self == NULL) {
        return __ct__CUIWindowManager(self, pScene, mHandle);
    }
    return new (self) CUIWindowManager(pScene, mHandle);
}

// Singleton accessor/creator: return the existing manager, or allocate a
// 0xA4-byte instance from the work heap, construct it (falling back to the
// MEM2 handle when the caller passes -1), register it under `pParent`, and
// cache it in the global singleton slot.
extern "C" CUIWindowManager* func_8013CFDC(CProcess* pParent, CScn* pScene,
                                           mtl::ALLOC_HANDLE mHandle) {
    if (lbl_eu_80664088 != NULL) {
        return lbl_eu_80664088;
    }
    if ((u32)mHandle == 0xFFFFFFFF) {
        mHandle = mtl::MemManager::getHandleMEM2();
    }
    void* mem = mtl::MemManager::allocate(0xA4, CWorkThreadSystem::getWorkMem());
    CUIWindowManager* inst = NULL;
    if (mem != NULL) {
        inst = (CUIWindowManager*)__ct__CUIWindowManager(
            (CUIWindowManager*)mem, pScene, mHandle);
    }
    lbl_eu_80664088 = inst;
    Regist__8CProcessFP8CProcessb(pParent, inst, false);
    return lbl_eu_80664088;
}

// reslist_base destructor (retail __dt___reslist_base_IUIWindow): install the
// vtable, walk the node chain nulling each node's next pointer (the nodes are
// pool-allocated, not freed here), reset the sentinel to an empty circular
// list, free the grown node-array buffer when it is not fixed-capacity, then
// delete-this behind the flags guard.
struct ResListNode {
    ResListNode* mNext; //0x00
    ResListNode* mPrev; //0x04
};
struct ResListBase {
    void* mVTable;        //0x00
    ResListNode mSentinel;//0x04 - circular list head
    u8 unk0C[0x14 - 0x0C];
    void* field_0x14;     //0x14 - grown node array (heap)
    u8 unk18[0x1C - 0x18];
    u8 field_0x1C;        //0x1C - nonzero: fixed capacity (no heap buffer)
};
extern "C" void* __dt___reslist_base_IUIWindow(void* self, s32 flags) {
    ResListBase* list = (ResListBase*)self;
    if (list != NULL) {
        list->mVTable = (void*)lbl_eu_8052E61C;
        ResListNode* node = list->mSentinel.mNext;
        while (node != &list->mSentinel) {
            ResListNode* cur = node;
            node = cur->mNext;
            cur->mNext = NULL;
        }
        list->mSentinel.mNext = &list->mSentinel;
        list->mSentinel.mPrev = &list->mSentinel;
        if (list->field_0x1C == 0 && list->field_0x14 != NULL) {
            __dla__FPv(list->field_0x14);
            list->field_0x14 = NULL;
        }
        if (flags > 0) {
            __dl__FPv(list);
        }
    }
    return self;
}
// Window factory: create a talk window on the secondary queue, removing any
// existing window with the same id first (retail splices the node out of the
// reslist). Returns 0 when the singleton or layout manager is absent.
extern "C" IUIWindow* func_8013D07C(u32 id, const u8* msgSrc, u32 a3) {
    volatile u32 savedRet;
    CUIWindowManager* inst = lbl_eu_80664088;
    if (inst == NULL) {
        return NULL;
    }
    if (func_801355F4() == NULL) {
        return NULL;
    }

    bool found = false;
    {
        // Retail unlinks the first match only: test-at-bottom walk through
        // the reloaded singleton (chained loads keep the sentinel in the
        // same register), then mark removal and splice the node out.
        inst = lbl_eu_80664088;
        WindowNode* sentinel = inst->mWindowList2.mStartNodePtr;
        WindowNode* n = sentinel->mNext;
        goto check;
    body:
        if (n->mItem->field_0x68 == id) {
            n->mItem->SetRemove();
            found = true;
            WindowNode* prev = n->mPrev;
            WindowNode* next = n->mNext;
            prev->mNext = next;
            next->mPrev = prev;
            n->mNext = NULL;
            goto done;
        }
    advance:
        n = n->mNext;
    check:
        if (n != sentinel) goto body;
    done:;
    }

    // Reload the singleton for the factory args (its live range ends at the
    // search loop) and again for the inlined push_back below.
    {
        // NOTE: retail does NOT null-check the factory result here - the
        // window (or 0) is queued regardless.
        u32 tempRet = (u32)func_8012CC78((CProcess*)lbl_eu_80664088->unk9C,
                                         (u32)lbl_eu_80664088->unk58, id,
                                         msgSrc, found, 0, a3);
        savedRet = tempRet;
    }
    inst = lbl_eu_80664088;
    int i = 0;
    int byteOff = 0;
    WindowNode* temp;
    int capacity;
    WindowNode* startNode;
    startNode = inst->mWindowList2.mStartNodePtr;
    capacity = inst->mWindowList2.mCapacity;
    goto queue_check;
queue_body:
    if (*(u32*)((u8*)inst->mWindowList2.mList + byteOff) == 0) {
        goto queue_found;
    }
    byteOff += 0xc;
    i++;
queue_check:
    if (i < capacity) {
        goto queue_body;
    }
queue_found:
    temp = (WindowNode*)((u8*)inst->mWindowList2.mList + i * 0xc);
    {
        u32* ptr = (u32*)&temp->mItem;
        if (ptr != 0) {
            try {
                *ptr = savedRet;
            } catch (...) {
                throw;
            }
        }
    }
    temp->mNext = startNode;
    temp->mPrev = startNode->mPrev;
    startNode->mPrev->mNext = temp;
    startNode->mPrev = temp;
    return (IUIWindow*)savedRet;
}
// Window-list lookup: find the secondary-queue window whose id matches,
// then dynamic_cast it to the talk-window type and set its +0xAC show flag.
void func_8013D1E8(u32 id) {
    CUIWindowManager* inst = lbl_eu_80664088;
    if (inst == NULL) {
        return;
    }
    if (id == 0) {
        return;
    }

    WindowIter endIt = inst->mWindowList2.end();
    WindowIter it = inst->mWindowList2.begin();
    for (; it != endIt; ++it) {
        IUIWindow* window = *it;
        if (window->field_0x68 == id) {
            void* cast = __dynamic_cast(window, 0x10, &lbl_eu_80662170,
                                        &lbl_eu_80661EC8, NULL);
            if (cast != NULL) {
                ((CTaskWindowFlags*)cast)->field_0xAC = 1;
            }
            return;
        }
    }
}
// Window factory: create the simple eve-talk window on the secondary queue.
// Returns 0 when the manager singleton is absent, the layout manager is not
// ready, or the factory declined (message handed to the existing window).
extern "C" IUIWindow* func_8013D448(u32 text, const u8* msgSrc) {
    volatile u32 savedRet;
    CUIWindowManager* inst = lbl_eu_80664088;
    if (inst == NULL) {
        return NULL;
    }
    if (func_801355F4() == NULL) {
        return NULL;
    }
    {
        // Retail reloads the singleton for the factory args (its live range
        // ends at the guard check) instead of keeping it in a saved register.
        u32 tempRet = (u32)func_801A20DC((CProcess*)lbl_eu_80664088->unk9C,
                                         lbl_eu_80664088->unk58, text, msgSrc,
                                         0);
        savedRet = tempRet;
        if (tempRet == 0) {
            return NULL;
        }
    }
    inst = lbl_eu_80664088;
    int i = 0;
    int byteOff = 0;
    WindowNode* temp;
    int capacity;
    WindowNode* startNode;
    startNode = inst->mWindowList2.mStartNodePtr;
    capacity = inst->mWindowList2.mCapacity;
    goto queue_check;
queue_body:
    if (*(u32*)((u8*)inst->mWindowList2.mList + byteOff) == 0) {
        goto queue_found;
    }
    byteOff += 0xc;
    i++;
queue_check:
    if (i < capacity) {
        goto queue_body;
    }
queue_found:
    temp = (WindowNode*)((u8*)inst->mWindowList2.mList + i * 0xc);
    {
        // Expanded setItem: reading the volatile savedRet inside the guard
        // keeps the reload after the addic./beq, as in retail.
        u32* ptr = (u32*)&temp->mItem;
        if (ptr != 0) {
            try {
                *ptr = savedRet;
            } catch (...) {
                throw;
            }
        }
    }
    temp->mNext = startNode;
    temp->mPrev = startNode->mPrev;
    startNode->mPrev->mNext = temp;
    startNode->mPrev = temp;
    return (IUIWindow*)savedRet;
}

// Window factory: create a system window on the primary queue. `str` is a
// non-empty string (the window title); returns 0 when the singleton is absent,
// the string is empty, or the entry check rejects the request.
extern "C" IUIWindow* func_8013D55C(const char* str, void* arg1, u32 arg2) {
    volatile u32 savedRet;
    CUIWindowManager* inst = lbl_eu_80664088;
    if (inst == NULL) {
        return NULL;
    }
    if (str[0] == 0) {
        return NULL;
    }
    if (arg2 == 0 && func_801356BC() != 0) {
        return NULL;
    }
    {
        // Volatile stack-homed result: the store lands between the compare
        // and the branch, as in retail.
        u32 tempRet = (u32)func_80124AEC(
            (CProcess*)lbl_eu_80664088->unk9C,
            arg1 != NULL ? arg1 : lbl_eu_80664088->unk58, 0, (u32)str, 0);
        savedRet = tempRet;
        if (tempRet == 0) {
            return NULL;
        }
    }
    inst = lbl_eu_80664088;
    int i = 0;
    int byteOff = 0;
    WindowNode* temp;
    int capacity;
    WindowNode* startNode;
    startNode = inst->mWindowList1.mStartNodePtr;
    capacity = inst->mWindowList1.mCapacity;
    goto queue_check;
queue_body:
    if (*(u32*)((u8*)inst->mWindowList1.mList + byteOff) == 0) {
        goto queue_found;
    }
    byteOff += 0xc;
    i++;
queue_check:
    if (i < capacity) {
        goto queue_body;
    }
queue_found:
    temp = (WindowNode*)((u8*)inst->mWindowList1.mList + i * 0xc);
    {
        // Expanded setItem: reading the volatile savedRet inside the guard
        // keeps the reload after the addic./beq, as in retail.
        u32* ptr = (u32*)&temp->mItem;
        if (ptr != 0) {
            try {
                *ptr = savedRet;
            } catch (...) {
                throw;
            }
        }
    }
    temp->mNext = startNode;
    temp->mPrev = startNode->mPrev;
    startNode->mPrev->mNext = temp;
    startNode->mPrev = temp;
    return (IUIWindow*)savedRet;
}

// Window factory: create a system window on the primary queue (flag = 1
// variant). Returns 0 when the singleton is absent or the entry check rejects.
extern "C" IUIWindow* func_8013D688(u32 arg0, void* arg1, void* arg2, u32 arg3) {
    volatile u32 savedRet;
    CUIWindowManager* inst = lbl_eu_80664088;
    if (inst == NULL) {
        return NULL;
    }
    if (arg3 == 0 && func_801356E0() != 0) {
        return NULL;
    }
    {
        u32 tempRet = (u32)func_80124AEC((CProcess*)lbl_eu_80664088->unk9C,
                                         arg2 != NULL ? arg2
                                                      : lbl_eu_80664088->unk58,
                                         1, arg0, (u32)arg1);
        savedRet = tempRet;
        if (tempRet == 0) {
            return NULL;
        }
    }
    inst = lbl_eu_80664088;
    int i = 0;
    int byteOff = 0;
    WindowNode* temp;
    int capacity;
    WindowNode* startNode;
    startNode = inst->mWindowList1.mStartNodePtr;
    capacity = inst->mWindowList1.mCapacity;
    goto queue_check;
queue_body:
    if (*(u32*)((u8*)inst->mWindowList1.mList + byteOff) == 0) {
        goto queue_found;
    }
    byteOff += 0xc;
    i++;
queue_check:
    if (i < capacity) {
        goto queue_body;
    }
queue_found:
    temp = (WindowNode*)((u8*)inst->mWindowList1.mList + i * 0xc);
    {
        // Expanded setItem: reading the volatile savedRet inside the guard
        // keeps the reload after the addic./beq, as in retail.
        u32* ptr = (u32*)&temp->mItem;
        if (ptr != 0) {
            try {
                *ptr = savedRet;
            } catch (...) {
                throw;
            }
        }
    }
    temp->mNext = startNode;
    temp->mPrev = startNode->mPrev;
    startNode->mPrev->mNext = temp;
    startNode->mPrev = temp;
    return (IUIWindow*)savedRet;
}
// Retail window creator: create a CSysWinBuff window under the manager's
// parent/scene and queue it on the primary window list.
// Same body shape as CUICfManager's CF_QUEUE_MENU creators: save the
// factory result in a volatile stack-homed savedRet, re-read the singleton,
// walk the free-slot list with an i/byteOff cursor pair, then setItem+splice.
extern "C" IUIWindow* func_8013D7C0(u32 id) {
    volatile u32 savedRet;
    CUIWindowManager* inst = lbl_eu_80664088;
    if (inst == NULL) {
        return NULL;
    }
    {
        u32 tempRet = (u32)create__11CSysWinBuffFv((CProcess*)inst->unk9C, inst->unk58, (u16)id);
        savedRet = tempRet;
        if (tempRet == 0) {
            return NULL;
        }
    }
    inst = lbl_eu_80664088;
    int i = 0;
    int byteOff = 0;
    WindowNode* temp;
    int capacity;
    WindowNode* startNode;
    startNode = inst->mWindowList1.mStartNodePtr;
    capacity = inst->mWindowList1.mCapacity;
    goto queue_check;
queue_body:
    if (*(u32*)((u8*)inst->mWindowList1.mList + byteOff) == 0) {
        goto queue_found;
    }
    byteOff += 0xc;
    i++;
queue_check:
    if (i < capacity) {
        goto queue_body;
    }
queue_found:
    temp = (WindowNode*)((u8*)inst->mWindowList1.mList + i * 0xc);
    {
        // Expanded setItem: reading the volatile savedRet inside the guard
        // keeps the reload after the addic./beq, as in retail.
        u32* ptr = (u32*)&temp->mItem;
        if (ptr != 0) {
            try {
                *ptr = savedRet;
            } catch (...) {
                throw;
            }
        }
    }
    temp->mNext = startNode;
    temp->mPrev = startNode->mPrev;
    startNode->mPrev->mNext = temp;
    startNode->mPrev = temp;
    return (IUIWindow*)savedRet;
}

void func_8013D8A0(){}
// Retail window creator (same body shape as func_8013D7C0).
extern "C" IUIWindow* func_8013D978(u32 a1, u32 a2, u32 a3) {
    volatile u32 savedRet;
    CUIWindowManager* inst = lbl_eu_80664088;
    if (inst == NULL) {
        return NULL;
    }
    {
        u32 tempRet = (u32)func_80125070((CProcess*)inst->unk9C, inst->unk58,
                                         a1, a2, a3);
        savedRet = tempRet;
        if (tempRet == 0) {
            return NULL;
        }
    }
    inst = lbl_eu_80664088;
    int i = 0;
    int byteOff = 0;
    WindowNode* temp;
    int capacity;
    WindowNode* startNode;
    startNode = inst->mWindowList1.mStartNodePtr;
    capacity = inst->mWindowList1.mCapacity;
    goto queue_check;
queue_body:
    if (*(u32*)((u8*)inst->mWindowList1.mList + byteOff) == 0) {
        goto queue_found;
    }
    byteOff += 0xc;
    i++;
queue_check:
    if (i < capacity) {
        goto queue_body;
    }
queue_found:
    temp = (WindowNode*)((u8*)inst->mWindowList1.mList + i * 0xc);
    {
        u32* ptr = (u32*)&temp->mItem;
        if (ptr != 0) {
            try {
                *ptr = savedRet;
            } catch (...) {
                throw;
            }
        }
    }
    temp->mNext = startNode;
    temp->mPrev = startNode->mPrev;
    startNode->mPrev->mNext = temp;
    startNode->mPrev = temp;
    return (IUIWindow*)savedRet;
}
// Window factory: create the quest window on the primary queue. Returns 0
// when the manager singleton is absent or the entry check rejects the id;
// note this creator does NOT null-check the factory result before queueing.
extern "C" IUIWindow* func_8013DA60(u32 id, void* arg1, u32 arg2) {
    volatile u32 savedRet;
    CUIWindowManager* inst = lbl_eu_80664088;
    if (inst == NULL) {
        return NULL;
    }
    if (arg2 == 0 && func_80135694((u16)id) != 0) {
        return NULL;
    }
    {
        // Note this creator does NOT null-check the factory result before
        // queueing - retail stores it and walks the list unconditionally.
        u32 tempRet = (u32)func_80122B2C((CProcess*)lbl_eu_80664088->unk9C,
                                         lbl_eu_80664088->unk58, id,
                                         (u32)arg1);
        savedRet = tempRet;
    }
    inst = lbl_eu_80664088;
    int i = 0;
    int byteOff = 0;
    WindowNode* temp;
    int capacity;
    WindowNode* startNode;
    startNode = inst->mWindowList1.mStartNodePtr;
    capacity = inst->mWindowList1.mCapacity;
    goto queue_check;
queue_body:
    if (*(u32*)((u8*)inst->mWindowList1.mList + byteOff) == 0) {
        goto queue_found;
    }
    byteOff += 0xc;
    i++;
queue_check:
    if (i < capacity) {
        goto queue_body;
    }
queue_found:
    temp = (WindowNode*)((u8*)inst->mWindowList1.mList + i * 0xc);
    {
        // Expanded setItem: reading the volatile savedRet inside the guard
        // keeps the reload after the addic./beq, as in retail.
        u32* ptr = (u32*)&temp->mItem;
        if (ptr != 0) {
            try {
                *ptr = savedRet;
            } catch (...) {
                throw;
            }
        }
    }
    temp->mNext = startNode;
    temp->mPrev = startNode->mPrev;
    startNode->mPrev->mNext = temp;
    startNode->mPrev = temp;
    return (IUIWindow*)savedRet;
}
// Window factory: create a window through the CMenuUpdate factory and queue
// it on the primary window list. Type 1 (quest) windows are rejected when the
// quest table already reports the row as shown. Retail inlines
// reslist::push_back (find-first-empty-slot + splice onto the sentinel).
extern "C" IUIWindow* func_8013DB6C(int first, u32 second, s32 third,
                                    s32 fourth) {
    volatile u32 savedRet;
    CUIWindowManager* inst = lbl_eu_80664088;
    if (inst == NULL) {
        return NULL;
    }
    if (first == 1) {
        int idx = func_80138138((int)second);
        if (func_801361E8((u32)lbl_eu_80573D18[idx], lbl_eu_8050097C,
                          second) == 2) {
            return NULL;
        }
    }
    {
        u32 tempRet = (u32)func_80142B4C(
            (CProcess*)lbl_eu_80664088->unk9C, lbl_eu_80664088->unk58,
            (int)first, (int)second, (int)third, (int)fourth);
        savedRet = tempRet;
        if (tempRet == 0) {
            return NULL;
        }
    }
    inst = lbl_eu_80664088;
    int i = 0;
    int byteOff = 0;
    WindowNode* temp;
    int capacity;
    WindowNode* startNode;
    startNode = inst->mWindowList1.mStartNodePtr;
    capacity = inst->mWindowList1.mCapacity;
    goto queue_check;
queue_body:
    if (*(u32*)((u8*)inst->mWindowList1.mList + byteOff) == 0) {
        goto queue_found;
    }
    byteOff += 0xc;
    i++;
queue_check:
    if (i < capacity) {
        goto queue_body;
    }
queue_found:
    temp = (WindowNode*)((u8*)inst->mWindowList1.mList + i * 0xc);
    {
        u32* ptr = (u32*)&temp->mItem;
        if (ptr != 0) {
            try {
                *ptr = savedRet;
            } catch (...) {
                throw;
            }
        }
    }
    temp->mNext = startNode;
    temp->mPrev = startNode->mPrev;
    startNode->mPrev->mNext = temp;
    startNode->mPrev = temp;
    return (IUIWindow*)savedRet;
}
// Retail window creator (same body shape as func_8013D7C0).
extern "C" IUIWindow* func_8013DCAC(u32 a1, u32 a2) {
    volatile u32 savedRet;
    CUIWindowManager* inst = lbl_eu_80664088;
    if (inst == NULL) {
        return NULL;
    }
    {
        u32 tempRet = (u32)func_80144EE4((CProcess*)inst->unk9C, inst->unk58,
                                         (u16)a1, (u8)a2);
        savedRet = tempRet;
        if (tempRet == 0) {
            return NULL;
        }
    }
    inst = lbl_eu_80664088;
    int i = 0;
    int byteOff = 0;
    WindowNode* temp;
    int capacity;
    WindowNode* startNode;
    startNode = inst->mWindowList1.mStartNodePtr;
    capacity = inst->mWindowList1.mCapacity;
    goto queue_check;
queue_body:
    if (*(u32*)((u8*)inst->mWindowList1.mList + byteOff) == 0) {
        goto queue_found;
    }
    byteOff += 0xc;
    i++;
queue_check:
    if (i < capacity) {
        goto queue_body;
    }
queue_found:
    temp = (WindowNode*)((u8*)inst->mWindowList1.mList + i * 0xc);
    {
        u32* ptr = (u32*)&temp->mItem;
        if (ptr != 0) {
            try {
                *ptr = savedRet;
            } catch (...) {
                throw;
            }
        }
    }
    temp->mNext = startNode;
    temp->mPrev = startNode->mPrev;
    startNode->mPrev->mNext = temp;
    startNode->mPrev = temp;
    return (IUIWindow*)savedRet;
}
// reslist<IUIWindow*>::push_back expanded exactly as retail inlines it into
// every window creator below: free-slot scan, setItem (whose try/catch emits
// the exception-frame sp-save), then the four-pointer splice onto the sentinel.
// NOTE: kept duplicated in each creator - a shared helper is not inlined by
// MWCC here and retail has no such call.

// Retail window creator: create the window from func_801602F4 and queue it
// on the primary window list.
extern "C" IUIWindow* func_8013DD94() {
    CUIWindowManager* inst = lbl_eu_80664088;
    if (inst == NULL) {
        return NULL;
    }

    IUIWindow* window = func_801602F4((CProcess*)inst->unk9C, inst->unk58);
    if (window == NULL) {
        return NULL;
    }

    lbl_eu_80664088->mWindowList1.push_back(window);
    return window;
}
// Retail window creator: create the Col6 hint window and queue it on the
// primary window list.
extern "C" IUIWindow* func_8013DE6C() {
    CUIWindowManager* inst = lbl_eu_80664088;
    if (inst == NULL) {
        return NULL;
    }

    IUIWindow* window = func_8015DCD0((CProcess*)inst->unk9C, inst->unk58);
    if (window == NULL) {
        return NULL;
    }

    // Re-read the singleton: its live range ends at the factory call, so
    // retail reloads the global for the inlined push_back.
    lbl_eu_80664088->mWindowList1.push_back(window);
    return window;
}

// NOTE: retail inlines reslist<IUIWindow*>::push_back into every window
// creator below, so the free-slot scan / setItem / splice tail is kept
// duplicated verbatim in each function - a shared helper emits a call
// retail never makes.

// Retail window creator: construct a CCol6Invite subobject on the manager's
// flagged child window and queue the resulting window handle.
extern "C" IUIWindow* func_8013DF44(u32 a1, u32 a2, u32 a3) {
    volatile u32 savedRet;
    CUIWindowManager* inst = lbl_eu_80664088;
    if (inst == NULL) {
        return NULL;
    }
    {
        u32 tempRet =
            (u32)__ct__CCol6Invite((CCol6Invite*)inst->unk9C, (u16)a1, (u8)a2,
                                   (u8)a3);
        savedRet = tempRet;
        if (tempRet == 0) {
            return NULL;
        }
    }
    inst = lbl_eu_80664088;
    int i = 0;
    int byteOff = 0;
    WindowNode* temp;
    int capacity;
    WindowNode* startNode;
    startNode = inst->mWindowList1.mStartNodePtr;
    capacity = inst->mWindowList1.mCapacity;
    goto queue_check;
queue_body:
    if (*(u32*)((u8*)inst->mWindowList1.mList + byteOff) == 0) {
        goto queue_found;
    }
    byteOff += 0xc;
    i++;
queue_check:
    if (i < capacity) {
        goto queue_body;
    }
queue_found:
    temp = (WindowNode*)((u8*)inst->mWindowList1.mList + i * 0xc);
    {
        // Expanded setItem: reading the volatile savedRet inside the guard
        // keeps the reload after the addic./beq, as in retail.
        u32* ptr = (u32*)&temp->mItem;
        if (ptr != 0) {
            try {
                *ptr = savedRet;
            } catch (...) {
                throw;
            }
        }
    }
    temp->mNext = startNode;
    temp->mPrev = startNode->mPrev;
    startNode->mPrev->mNext = temp;
    startNode->mPrev = temp;
    return (IUIWindow*)savedRet;
}
void func_8013E030(){}
// Retail window creator: create the shop-select window and queue it. A
// quest-menu open guard can veto the creation.
extern "C" IUIWindow* func_8013E104(u32 id) {
    volatile u32 savedRet;
    CUIWindowManager* inst = lbl_eu_80664088;
    if (inst == NULL) {
        return NULL;
    }
    if (func_80226B94()) {
        return NULL;
    }
    {
        u32 tempRet = (u32)func_8018A58C((CProcess*)inst->unk9C, inst->unk58,
                                         id);
        savedRet = tempRet;
        if (tempRet == 0) {
            return NULL;
        }
    }
    inst = lbl_eu_80664088;
    int i = 0;
    int byteOff = 0;
    WindowNode* temp;
    int capacity;
    WindowNode* startNode;
    startNode = inst->mWindowList1.mStartNodePtr;
    capacity = inst->mWindowList1.mCapacity;
    goto queue_check;
queue_body:
    if (*(u32*)((u8*)inst->mWindowList1.mList + byteOff) == 0) {
        goto queue_found;
    }
    byteOff += 0xc;
    i++;
queue_check:
    if (i < capacity) {
        goto queue_body;
    }
queue_found:
    temp = (WindowNode*)((u8*)inst->mWindowList1.mList + i * 0xc);
    {
        // Expanded setItem: reading the volatile savedRet inside the guard
        // keeps the reload after the addic./beq, as in retail.
        u32* ptr = (u32*)&temp->mItem;
        if (ptr != 0) {
            try {
                *ptr = savedRet;
            } catch (...) {
                throw;
            }
        }
    }
    temp->mNext = startNode;
    temp->mPrev = startNode->mPrev;
    startNode->mPrev->mNext = temp;
    startNode->mPrev = temp;
    return (IUIWindow*)savedRet;
}
// Retail window creator: create the item-exchange window and queue it on the
// primary window list.
extern "C" IUIWindow* func_8013E204(u32 id) {
    CUIWindowManager* inst = lbl_eu_80664088;
    if (inst == NULL) {
        return NULL;
    }

    IUIWindow* window = func_801BEDE0((CProcess*)inst->unk9C, inst->unk58, id);
    if (window == NULL) {
        return NULL;
    }

    lbl_eu_80664088->mWindowList1.push_back(window);
    return window;
}

// Window factory: create an item-multi window on the primary queue. The
// entry guard only runs when arg7 is clear. The 9th argument is a byte flag
// forwarded to the factory (retail reads it from the stack slot's low byte).
extern "C" IUIWindow* func_8013E2E0(u32 a1, u32 a2, u32 a3, u32 a4, u32 a5,
                                    u32 a6, u32 a7, u32 a8, u8 a9) {
    volatile u32 savedRet;
    // Retail homes the stack byte arg into a callee-saved register with a
    // single lbz at entry and forwards it unmasked.
    u32 flag = a9;
    CUIWindowManager* inst = lbl_eu_80664088;
    if (inst == NULL) {
        return NULL;
    }
    if (a7 == 0 && func_80135654(a1, a2, a3, a4, a5)) {
        return NULL;
    }
    {
        u32 tempRet = (u32)func_801B46E4((CProcess*)lbl_eu_80664088->unk9C,
                                         lbl_eu_80664088->unk58, a1, a2, a3,
                                         a4, a6, a8, flag);
        savedRet = tempRet;
        if (tempRet == 0) {
            return NULL;
        }
    }
    inst = lbl_eu_80664088;
    int i = 0;
    int byteOff = 0;
    WindowNode* temp;
    int capacity;
    WindowNode* startNode;
    startNode = inst->mWindowList1.mStartNodePtr;
    capacity = inst->mWindowList1.mCapacity;
    goto queue_check;
queue_body:
    if (*(u32*)((u8*)inst->mWindowList1.mList + byteOff) == 0) {
        goto queue_found;
    }
    byteOff += 0xc;
    i++;
queue_check:
    if (i < capacity) {
        goto queue_body;
    }
queue_found:
    temp = (WindowNode*)((u8*)inst->mWindowList1.mList + i * 0xc);
    {
        u32* ptr = (u32*)&temp->mItem;
        if (ptr != 0) {
            try {
                *ptr = savedRet;
            } catch (...) {
                throw;
            }
        }
    }
    temp->mNext = startNode;
    temp->mPrev = startNode->mPrev;
    startNode->mPrev->mNext = temp;
    startNode->mPrev = temp;
    return (IUIWindow*)savedRet;
}
// Window factory: create an item-multi related window on the primary queue.
// Returns 0 when the singleton is absent, the entry guard rejects (only
// consulted when arg2 is clear), or the factory returned 0.
extern "C" IUIWindow* func_8013E424(u32 arg1, u32 arg2) {
    volatile u32 savedRet;
    CUIWindowManager* inst = lbl_eu_80664088;
    if (inst == NULL) {
        return NULL;
    }
    if (arg2 == 0 && func_80135630() != 0) {
        return NULL;
    }
    {
        u32 tempRet = (u32)func_801B4790((CProcess*)lbl_eu_80664088->unk9C,
                                         lbl_eu_80664088->unk58, arg1);
        savedRet = tempRet;
        if (tempRet == 0) {
            return NULL;
        }
    }
    inst = lbl_eu_80664088;
    int i = 0;
    int byteOff = 0;
    WindowNode* temp;
    int capacity;
    WindowNode* startNode;
    startNode = inst->mWindowList1.mStartNodePtr;
    capacity = inst->mWindowList1.mCapacity;
    goto queue_check;
queue_body:
    if (*(u32*)((u8*)inst->mWindowList1.mList + byteOff) == 0) {
        goto queue_found;
    }
    byteOff += 0xc;
    i++;
queue_check:
    if (i < capacity) {
        goto queue_body;
    }
queue_found:
    temp = (WindowNode*)((u8*)inst->mWindowList1.mList + i * 0xc);
    {
        // Expanded setItem: reading the volatile savedRet inside the guard
        // keeps the reload after the addic./beq, as in retail.
        u32* ptr = (u32*)&temp->mItem;
        if (ptr != 0) {
            try {
                *ptr = savedRet;
            } catch (...) {
                throw;
            }
        }
    }
    temp->mNext = startNode;
    temp->mPrev = startNode->mPrev;
    startNode->mPrev->mNext = temp;
    startNode->mPrev = temp;
    return (IUIWindow*)savedRet;
}
// Retail window creator: create the kizuna-talk window and queue it on the
// primary window list.
extern "C" IUIWindow* func_8013E52C(u32 charId) {
    CUIWindowManager* inst = lbl_eu_80664088;
    if (inst == NULL) {
        return NULL;
    }

    IUIWindow* window =
        func_801BCEBC((CProcess*)inst->unk9C, inst->unk58, charId);
    if (window == NULL) {
        return NULL;
    }

    lbl_eu_80664088->mWindowList1.push_back(window);
    return window;
}

// Retail window creator: create the quest-log menu window and queue it.
// The trailing entry byte passed to the factory is a literal 0 here.
extern "C" IUIWindow* func_8013E608(u32 a1, u32 a2, u32 a3, u32 a4) {
    volatile u32 savedRet;
    CUIWindowManager* inst = lbl_eu_80664088;
    if (inst == NULL) {
        return NULL;
    }
    {
        u32 tempRet = (u32)func_802269D8(
            (CProcess*)inst->unk9C, inst->unk58,
            (u16)a1, (u16)a2, (u8)a3, (u8)a4, 0);
        savedRet = tempRet;
        if (tempRet == 0) {
            return NULL;
        }
    }
    inst = lbl_eu_80664088;
    int i = 0;
    int byteOff = 0;
    WindowNode* temp;
    int capacity;
    WindowNode* startNode;
    startNode = inst->mWindowList1.mStartNodePtr;
    capacity = inst->mWindowList1.mCapacity;
    goto queue_check;
queue_body:
    if (*(u32*)((u8*)inst->mWindowList1.mList + byteOff) == 0) {
        goto queue_found;
    }
    byteOff += 0xc;
    i++;
queue_check:
    if (i < capacity) {
        goto queue_body;
    }
queue_found:
    temp = (WindowNode*)((u8*)inst->mWindowList1.mList + i * 0xc);
    {
        // Expanded setItem: reading the volatile savedRet inside the guard
        // keeps the reload after the addic./beq, as in retail.
        u32* ptr = (u32*)&temp->mItem;
        if (ptr != 0) {
            try {
                *ptr = savedRet;
            } catch (...) {
                throw;
            }
        }
    }
    temp->mNext = startNode;
    temp->mPrev = startNode->mPrev;
    startNode->mPrev->mNext = temp;
    startNode->mPrev = temp;
    return (IUIWindow*)savedRet;
}

// Same shape as func_8013E608 with the literal entry byte set to 1.
extern "C" IUIWindow* func_8013E704(u32 a1, u32 a2, u32 a3, u32 a4) {
    volatile u32 savedRet;
    CUIWindowManager* inst = lbl_eu_80664088;
    if (inst == NULL) {
        return NULL;
    }
    {
        u32 tempRet = (u32)func_802269D8(
            (CProcess*)inst->unk9C, inst->unk58,
            (u16)a1, (u16)a2, (u8)a3, (u8)a4, 1);
        savedRet = tempRet;
        if (tempRet == 0) {
            return NULL;
        }
    }
    inst = lbl_eu_80664088;
    int i = 0;
    int byteOff = 0;
    WindowNode* temp;
    int capacity;
    WindowNode* startNode;
    startNode = inst->mWindowList1.mStartNodePtr;
    capacity = inst->mWindowList1.mCapacity;
    goto queue_check;
queue_body:
    if (*(u32*)((u8*)inst->mWindowList1.mList + byteOff) == 0) {
        goto queue_found;
    }
    byteOff += 0xc;
    i++;
queue_check:
    if (i < capacity) {
        goto queue_body;
    }
queue_found:
    temp = (WindowNode*)((u8*)inst->mWindowList1.mList + i * 0xc);
    {
        // Expanded setItem: reading the volatile savedRet inside the guard
        // keeps the reload after the addic./beq, as in retail.
        u32* ptr = (u32*)&temp->mItem;
        if (ptr != 0) {
            try {
                *ptr = savedRet;
            } catch (...) {
                throw;
            }
        }
    }
    temp->mNext = startNode;
    temp->mPrev = startNode->mPrev;
    startNode->mPrev->mNext = temp;
    startNode->mPrev = temp;
    return (IUIWindow*)savedRet;
}
// Retail window creator (same body shape as func_8013D7C0): factory call,
// volatile stack-homed savedRet, singleton re-read, free-slot cursor walk,
// expanded setItem + splice.
extern "C" IUIWindow* func_8013E800(u32 id) {
    volatile u32 savedRet;
    CUIWindowManager* inst = lbl_eu_80664088;
    if (inst == NULL) {
        return NULL;
    }
    {
        u32 tempRet =
            (u32)func_8026F8B0((CProcess*)inst->unk9C, inst->unk58, 2, id);
        savedRet = tempRet;
        if (tempRet == 0) {
            return NULL;
        }
    }
    inst = lbl_eu_80664088;
    int i = 0;
    int byteOff = 0;
    WindowNode* temp;
    int capacity;
    WindowNode* startNode;
    startNode = inst->mWindowList1.mStartNodePtr;
    capacity = inst->mWindowList1.mCapacity;
    goto queue_check;
queue_body:
    if (*(u32*)((u8*)inst->mWindowList1.mList + byteOff) == 0) {
        goto queue_found;
    }
    byteOff += 0xc;
    i++;
queue_check:
    if (i < capacity) {
        goto queue_body;
    }
queue_found:
    temp = (WindowNode*)((u8*)inst->mWindowList1.mList + i * 0xc);
    {
        // Expanded setItem: reading the volatile savedRet inside the guard
        // keeps the reload after the addic./beq, as in retail.
        u32* ptr = (u32*)&temp->mItem;
        if (ptr != 0) {
            try {
                *ptr = savedRet;
            } catch (...) {
                throw;
            }
        }
    }
    temp->mNext = startNode;
    temp->mPrev = startNode->mPrev;
    startNode->mPrev->mNext = temp;
    startNode->mPrev = temp;
    return (IUIWindow*)savedRet;
}
// Retail window creator: create the party-change-notice window and queue it.
// When flag is clear, an entry guard can still veto the creation.
extern "C" IUIWindow* func_8013E8E0(u32 flag) {
    volatile u32 savedRet;
    CUIWindowManager* inst = lbl_eu_80664088;
    if (inst == NULL) {
        return NULL;
    }
    if (flag == 0 && func_80135610()) {
        return NULL;
    }
    {
        u32 tempRet =
            (u32)func_80293B9C((CProcess*)inst->unk9C, inst->unk58);
        savedRet = tempRet;
        if (tempRet == 0) {
            return NULL;
        }
    }
    inst = lbl_eu_80664088;
    int i = 0;
    int byteOff = 0;
    WindowNode* temp;
    int capacity;
    WindowNode* startNode;
    startNode = inst->mWindowList1.mStartNodePtr;
    capacity = inst->mWindowList1.mCapacity;
    goto queue_check;
queue_body:
    if (*(u32*)((u8*)inst->mWindowList1.mList + byteOff) == 0) {
        goto queue_found;
    }
    byteOff += 0xc;
    i++;
queue_check:
    if (i < capacity) {
        goto queue_body;
    }
queue_found:
    temp = (WindowNode*)((u8*)inst->mWindowList1.mList + i * 0xc);
    {
        // Expanded setItem: reading the volatile savedRet inside the guard
        // keeps the reload after the addic./beq, as in retail.
        u32* ptr = (u32*)&temp->mItem;
        if (ptr != 0) {
            try {
                *ptr = savedRet;
            } catch (...) {
                throw;
            }
        }
    }
    temp->mNext = startNode;
    temp->mPrev = startNode->mPrev;
    startNode->mPrev->mNext = temp;
    startNode->mPrev = temp;
    return (IUIWindow*)savedRet;
}
// Retail window creator: create the save window (CSysWinSave) and queue it on
// the primary window list.
extern "C" IUIWindow* func_8013E9D8() {
    CUIWindowManager* inst = lbl_eu_80664088;
    if (inst == NULL) {
        return NULL;
    }

    IUIWindow* window = __ct__802944D8((CProcess*)inst->unk9C, inst->unk58);
    if (window == NULL) {
        return NULL;
    }

    lbl_eu_80664088->mWindowList1.push_back(window);
    return window;
}

// Retail window creator (same body shape as func_8013D7C0).
extern "C" IUIWindow* func_8013EAB0(u32 id) {
    volatile u32 savedRet;
    CUIWindowManager* inst = lbl_eu_80664088;
    if (inst == NULL) {
        return NULL;
    }
    {
        u32 tempRet =
            (u32)func_802AA2A0((CProcess*)inst->unk9C, inst->unk58, (u8)id);
        savedRet = tempRet;
        if (tempRet == 0) {
            return NULL;
        }
    }
    inst = lbl_eu_80664088;
    int i = 0;
    int byteOff = 0;
    WindowNode* temp;
    int capacity;
    WindowNode* startNode;
    startNode = inst->mWindowList1.mStartNodePtr;
    capacity = inst->mWindowList1.mCapacity;
    goto queue_check;
queue_body:
    if (*(u32*)((u8*)inst->mWindowList1.mList + byteOff) == 0) {
        goto queue_found;
    }
    byteOff += 0xc;
    i++;
queue_check:
    if (i < capacity) {
        goto queue_body;
    }
queue_found:
    temp = (WindowNode*)((u8*)inst->mWindowList1.mList + i * 0xc);
    {
        u32* ptr = (u32*)&temp->mItem;
        if (ptr != 0) {
            try {
                *ptr = savedRet;
            } catch (...) {
                throw;
            }
        }
    }
    temp->mNext = startNode;
    temp->mPrev = startNode->mPrev;
    startNode->mPrev->mNext = temp;
    startNode->mPrev = temp;
    return (IUIWindow*)savedRet;
}
void func_8013EB90(){}
// Retail dtor of the CTest view (complete-object flavor): run the CProcess
// base dtor behind a null guard (the dead second beq mirrors retail), then
// free the object when the delete flag is positive.
extern "C" void* __dt__Q216CUIWindowManager5CTestFv(void* self, s32 flags) {
    if (self != NULL) {
        if (self != NULL) {
            __dt__8CProcessFv(self, 0);
        }
        if (flags > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}
// Clamp the +0x52 page id into the 544..1543 range, then when the clamp is
// valid run the +0xC4 layout resize (0, 0, 8) and a 4-byte memset.
extern "C" void func_8013F244(void* self) {
    CFlagBuffer* buf = (CFlagBuffer*)self;
    u16 page = buf->field_0x52;
    int idx = ((s32)page < 1000) ? page + 544 : -1;
    if (idx != -1) {
        func_8009D018(idx, 0);
        memset(buf->field_0xC4, 0, 8);
    }
}
// Release any pending flag-memory writes for this buffer's quest page: the
// 0xFC/0xFD deferred states are committed as 0xFE/0xFF.
void func_8013F354(CFlagBuffer* self) {
    CFlagBuffer* global = (CFlagBuffer*)lbl_eu_80573C50;
    if (global->field_0x6C[0] == 0) {
        return;
    }
    int v52 = self->field_0x52;
    int id = (v52 < 0x3E8) ? v52 + 0x220 : -1;
    if (id == -1) {
        return;
    }
    int state = self->field_0x04;
    if (state == 0xFC) {
        func_8009D018(id, 0xFE);
        self->field_0x04 = 0xFE;
    } else if (state == 0xFD) {
        func_8009D018(id, 0xFF);
        self->field_0x04 = 0xFF;
    }
}
// Reset the buffer's layout and mission slots: commit slot 0x52 as value 1,
// clear the 8-byte per-item table, then release slots 0x68/0x6A with 0xC8.
void func_8013F2A0(CFlagBuffer* self) {
    int v52 = self->field_0x52;
    int id = -1;
    if (v52 < 0x3E8) {
        id = v52 + 0x220;
    }
    if (id == -1) {
        return;
    }
    func_8009D018(id, 1);
    memset(self->field_0xC4, 0, 8);

    int v68 = self->field_0x68;
    if (v68 != 0) {
        id = -1;
        if (v68 < 0x3E8) {
            id = v68 + 0x220;
        }
        if (id == -1) {
            return;
        }
        func_8009D018(id, 0xC8);
    }

    int v6A = self->field_0x6A;
    if (v6A != 0) {
        id = -1;
        if (v6A < 0x3E8) {
            id = v6A + 0x220;
        }
        if (id == -1) {
            return;
        }
        func_8009D018(id, 0xC8);
    }
}
// Flag-buffer availability check (retail func_8013F3F0): returns 1 when the
// buffer's window can still be shown, 0 otherwise. Bit0 of field_0x00 is
// cleared by the caller before querying. The quest id (0x52) must not map to
// a blocked flag-memory state, and each nonzero mission slot (0x58-0x64)
// must pass its own flag-memory gate.
int func_8013F3F0(CFlagBuffer* flagBuf) {
    if ((flagBuf->field_0x00 & 1) != 0) {
        return 0;
    }

    int v52 = flagBuf->field_0x52;
    int id = -1;
    if (v52 < 0x3E8) {
        id = v52 + 0x220;
    }
    if (id == -1) {
        return 0;
    }
    int res = (int)func_8009CF8C((u32)id);
    if (res == 1) {
        return 0;
    }
    if (res == 0x6F || res == 0x70 || res == 0x79 || res == 0x7A ||
        res == 0x83 || res == 0x84 || (u32)(res - 0x8D) <= 1) {
        return 0;
    }
    if (res == 0xC8) {
        return 0;
    }
    if ((u32)(res - 0xFE) <= 1) {
        return 0;
    }

    int v58 = flagBuf->field_0x58;
    if (v58 == 0 && flagBuf->field_0x5A == 0 && flagBuf->field_0x5C == 0 &&
        flagBuf->field_0x60 == 0 && flagBuf->field_0x62 == 0 &&
        flagBuf->field_0x64 == 0) {
        return 0;
    }
    if (v58 != 0 && (int)func_8009CF8C(0x20) < flagBuf->field_0x58) {
        return 0;
    }

    int v5a = flagBuf->field_0x5A;
    if (v5a != 0) {
        id = -1;
        if (v5a < 0x3E8) {
            id = v5a + 0x220;
        }
        if (id == -1) {
            return 0;
        }
        int res = (int)func_8009CF8C((u32)id);
        if (res != 0xFE && res != 0xFF) {
            return 0;
        }
    }

    int v5c = flagBuf->field_0x5C;
    if (v5c != 0) {
        id = -1;
        if (v5c < 7) {
            id = v5c + 0x21;
        }
        if (id == -1) {
            return 0;
        }
        int res = (int)func_8009CF8C((u32)id);
        if (res < (int)flagBuf->field_0x5E) {
            return 0;
        }
    }

    int v60 = flagBuf->field_0x60;
    if (v60 != 0) {
        u16 v = (u16)getBdatStringColumnValue(lbl_eu_80664098,
                                              lbl_eu_80500A50, v60);
        id = -1;
        if (v < 0x12C) {
            id = v + 0xA20;
        }
        if (id == -1) {
            return 0;
        }
        if ((int)func_8009CF8C((u32)id) == 0) {
            return 0;
        }
    }

    int v62 = flagBuf->field_0x62;
    if (v62 != 0) {
        u16 v = (u16)getBdatStringColumnValue(lbl_eu_80664098,
                                              lbl_eu_80500A50, v62);
        id = -1;
        if (v < 0x12C) {
            id = v + 0xA20;
        }
        if (id == -1) {
            return 0;
        }
        if ((int)func_8009CF8C((u32)id) == 0) {
            return 0;
        }
    }

    int v64 = flagBuf->field_0x64;
    if (v64 != 0) {
        id = -1;
        if (v64 < 0x190) {
            id = v64 + 0x608;
        }
        if (id == -1) {
            return 0;
        }
        int res = (int)func_8009CF8C((u32)id);
        if (res != (int)flagBuf->field_0x66) {
            return 0;
        }
    }

    return 1;
}
void func_8013F6C4(){}
extern "C" void func_8013FFF8(void* flagBuf, void* entry, u32 value){}
// Item-availability query (retail func_80140854): returns 1 when the item
// can be used/shown, 0 otherwise. Reads the per-row entry type at
// rows[arg1].field_0x02[arg2]; types 2 / 3 / 0x104 / 0x105 gate on the sign
// of the field_0xC4 byte table and a 4-entry rodata flag table; type 4
// accepts when the flag-memory read is 0xFE or 0xFF; the default case does a
// signed range test of the byte-table value against the row byte.
int func_80140854(CItemQuery* self, u32 arg1, u32 arg2) {
    if ((self->field_0x00 & 1) == 0) {
        if ((u32)(self->field_0x04 - 0xFC) > 3) {
            CItemQueryRow* row = &self->field_0x6E[arg1];
            u16 rowType = row->field_0x02[arg2];
            if (rowType == 2) {
                if ((s8)self->field_0xC4[arg1 * 4 + arg2] >= 1) {
                    return 1;
                }
                if ((int)func_80158068(row->field_0x0A[arg2]) >=
                    (int)row->field_0x12[arg2]) {
                    return 1;
                }
                if (row->field_0x01 == 0) {
                    return 0;
                }
                if (*(u32*)&lbl_804FC260[arg1 * 4 + arg2 * 8] >
                    self->field_0x04) {
                    return 0;
                }
                u32 idx = (__cntlzw(arg1) >> 3) & 0x07FFFFFF;
                u32* t = (u32*)&lbl_804FC260[idx];
                if (self->field_0x04 == t[0] || self->field_0x04 == t[2] ||
                    self->field_0x04 == t[4] || self->field_0x04 == t[6]) {
                    return 0;
                }
                return 1;
            }
            if (rowType == 3 || rowType == 0x104 || rowType == 0x105) {
                if ((s8)self->field_0xC4[arg1 * 4 + arg2] >= 1) {
                    return 1;
                }
                if (row->field_0x01 == 0) {
                    return 0;
                }
                if (*(u32*)&lbl_804FC260[arg1 * 4 + arg2 * 8] >
                    self->field_0x04) {
                    return 0;
                }
                u32 idx = (__cntlzw(arg1) >> 3) & 0x07FFFFFF;
                u32* t = (u32*)&lbl_804FC260[idx];
                if (self->field_0x04 == t[0] || self->field_0x04 == t[2] ||
                    self->field_0x04 == t[4] || self->field_0x04 == t[6]) {
                    return 0;
                }
                return 1;
            }
            if (rowType == 4) {
                int id = -1;
                if (row->field_0x0A[arg2] < 0x3E8) {
                    id = row->field_0x0A[arg2] + 0x220;
                }
                if (id == -1) {
                    return 0;
                }
                int res = (int)func_8009CF8C((u32)id);
                return (res == 0xFE || res == 0xFF) ? 1 : 0;
            }
            s8 v = (s8)self->field_0xC4[arg1 * 4 + arg2];
            return v >= (int)row->field_0x12[arg2];
        } else {
            return 1;
        }
    }
    return 0;
}
// Flag-buffer build (bdat-column variant): zero the 0xC8-byte flag buffer,
// mark it active, refresh the per-table entry pointers, then scan every
// table's rows for a row whose bdat column value matches `target`. On a hit
// set the flag for that table (func_8013FFF8) and, when the window-open
// guard is clear (field_0x09 == 0), accept the buffer when the flag-memory
// read of the 0x52 slot id is 1, 2, 0xFC or 0xFD; otherwise 0.
// The 28 base offsets live in a 112-byte block; retail copies it wholesale
// (backend struct-assign copy loop) once into `work`, and again into a
// scratch block per row visited.
u8* func_80140AFC(u32 target) {
    // Declared up-front: retail forms both table addresses and zeroes the
    // index/cursors before the first block copy.
    CFlagOffsets tmp;
    CFlagOffsets work;
    void** table;
    u32* off;
    u8* entry;
    int i;

    memset(lbl_eu_80573C50, 0, 0xC8);
    lbl_eu_80573C50[0] |= 1;
    func_8003AA34();

    table = lbl_eu_80573D18;
    off = tmp.v;
    i = 0;
    work = *(const CFlagOffsets*)lbl_804FC1D0;

    for (; i < 28; ++i) {
        // Advance the table cursor as we read so the entry pointer cannot be
        // re-derived inside the row loop (retail keeps it in a register).
        entry = (u8*)*table++;
        int count = (int)func_8003B1EC(entry);
        for (int j = 0; j < count; ++j) {
            tmp = work;
            int v = j + (int)*off;
            if ((u16)getBdatStringColumnValue(entry, &lbl_eu_80500A50[0x32],
                                              v) == target) {
                func_8013FFF8(lbl_eu_80573C50, entry, (u32)v);
                if (((CFlagBuffer*)lbl_eu_80573C50)->field_0x09 == 0) {
                    int id = -1;
                    if (((CFlagBuffer*)lbl_eu_80573C50)->field_0x52 < 0x3E8) {
                        id =
                            ((CFlagBuffer*)lbl_eu_80573C50)->field_0x52 + 0x220;
                    }
                    if (id == -1) {
                        return NULL;
                    }
                    u32 res = func_8009CF8C((u32)id);
                    ((CFlagBuffer*)lbl_eu_80573C50)->field_0x04 = res;
                    if (res == 1 || res == 2 || res == 0xFC || res == 0xFD) {
                        return (u8*)lbl_eu_80573C50;
                    }
                }
            }
        }
        ++off;
    }
    return NULL;
}
// Flag-buffer build (bdat-column variant): zero the 0xC8-byte flag buffer,
// mark it active, refresh the per-table entry pointers, then scan every
// table's rows for a row whose bdat column value matches `target`. On a hit
// set the flag for that table and, when the window-open guard is clear
// (flagBuf[9] == 0 and func_8013F3F0 accepts), clear the +4 slot and return
// the buffer, otherwise 0.
extern "C" u8* func_80140CA4(u32 target) {
    u8* flagBuf = lbl_eu_80573C50;
    memset(flagBuf, 0, 0xC8);
    flagBuf[0] |= 1;
    func_8003AA34();

    // Working copy of the 28 per-table base offsets (14 x 8-byte chunks).
    u32 work[28];
    for (int i = 0; i < 14; i++) {
        ((u64*)work)[i] = ((u64*)lbl_804FC1D0)[i];
    }

    for (int i = 0; i < 28; i++) {
        u8* entry = (u8*)lbl_eu_80573D18[i];
        int count = (int)func_8003B1EC(entry);
        for (int j = 0; j < count; j++) {
            u32 tmp[28];
            for (int k = 0; k < 14; k++) {
                ((u64*)tmp)[k] = ((u64*)work)[k];
            }
            u32 v = j + tmp[i];
            if ((u16)getBdatStringColumnValue(entry, &lbl_eu_80500A50[0x32],
                                              (int)v) == target) {
                func_8013FFF8(flagBuf, entry, v);
                if (flagBuf[9] == 0 && func_8013F3F0((CFlagBuffer*)flagBuf) != 0) {
                    // Clear the per-table entry slot (retail writes a word).
                    *reinterpret_cast<u32*>(&flagBuf[4]) = 0;
                    return flagBuf;
                }
            }
        }
    }
    return NULL;
}
void func_80140E00(){}
// Flag-slot availability query: map the page id into the flag-memory index
// (1000..1543 -> 544..1087) and report whether the slot is not locked to the
// 0xFE/0xFF pair.
extern "C" int func_80141270(s32 page) {
    int id = -1;
    if (page < 0x3E8) {
        id = page + 0x220;
    }
    if (id == -1) {
        return 0;
    }
    u32 res = func_8009CF8C((u32)id);
    return ((u32)(res - 0xFE) <= 1);
}
// Flag-buffer build: zero the 0xC8-byte flag buffer, mark it active, refresh
// the per-table entry pointers (func_8003AA34), then scan every table's rows
// for `row + baseOffset[i] == target`; on a hit set the flag for that table
// (func_8013FFF8) and return the buffer, otherwise 0.
// The 28 base offsets live in a 112-byte block; retail copies it wholesale
// (backend struct-assign copy loop) once into `work`, and again into a
// scratch block per row visited.
extern "C" u8* func_801412D0(u32 target) {
    // Declared up-front: retail forms both table addresses and zeroes the
    // index/cursors before the first block copy.
    CFlagOffsets tmp;
    CFlagOffsets work;
    u32* off;
    void** table;
    u8* entry;
    int i;

    memset(lbl_eu_80573C50, 0, 0xC8);
    lbl_eu_80573C50[0] |= 1;
    func_8003AA34();

    table = lbl_eu_80573D18;
    off = tmp.v;
    i = 0;
    work = *(const CFlagOffsets*)lbl_804FC1D0;

    for (; i < 28; ++i) {
        // Advance the table cursor as we read so the entry pointer cannot be
        // re-derived inside the row loop (retail keeps it in a register).
        entry = (u8*)*table++;
        int count = (int)func_8003B1EC(entry);
        for (int j = 0; j < count; ++j) {
            tmp = work;
            int v = j + (int)*off;
            if (v == (int)target) {
                func_8013FFF8(lbl_eu_80573C50, entry, (u32)v);
                return lbl_eu_80573C50;
            }
        }
        ++off;
    }
    return NULL;
}
void sinit_801413F8() {
    CFlagBuffer* buf = (CFlagBuffer*)lbl_eu_80573C50;
    CItemQueryRow* rowEnd = buf->field_0x6E + 2;
    CItemQueryRow* row = buf->field_0x6E;
    lbl_eu_80573C50[0] = 0;
    buf->field_0x0C[0] = 0;
    buf->field_0x4C = 0;
    for (; row < rowEnd; ++row) {
        memset(row, 0, sizeof(CItemQueryRow));
    }
    memset(buf, 0, sizeof(CFlagBuffer));
    lbl_eu_80573C50[0] |= 1;
}
