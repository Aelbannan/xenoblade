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

#include <types.h>
#include <string.h>

// Batch 2026-07-14g: window-mgr-move owns Move() exclusively in this TU.

extern "C" {
CUIWindowManager* lbl_eu_80664088;
s16 lbl_eu_8066408C;

void func_8009D0B4();
void func_8009D514(cf::IFlagEvent*);
}

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
    bool unk65;              //0x65 - pending update-mark flag
    u8 pad66[0x68 - 0x66];   //0x66
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
            if (search->mItem->unk65 != 0 || unkA1 != 0) {
                bool flag = true;
                for (; mark != mWindowList1.mStartNodePtr; mark = mark->mNext) {
                    mark->mItem->unk65 = flag;
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
            if (search->mItem->unk65 != 0 || unkA1 != 0) {
                bool flag = true;
                for (; mark != mWindowList2.mStartNodePtr; mark = mark->mNext) {
                    mark->mItem->unk65 = flag;
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
void func_8013D26C(){}
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

void __ct__CUIWindowManager(){}
void __dt__CTTask_CUIWindowManager(){}
void func_8013CBB4(){}
void func_8013CFDC(){}
// Window factory: create a talk window on the secondary queue, removing any
// existing window with the same id first (retail splices the node out of the
// reslist). Returns 0 when the singleton or layout manager is absent.
extern "C" IUIWindow* func_8013D07C(u32 a1, u32 a2, u32 a3) {
    if (lbl_eu_80664088 == NULL) {
        return NULL;
    }
    if (func_801355F4() == NULL) {
        return NULL;
    }

    bool found = false;
    {
        // Retail unlinks the first match only: advance-at-top, head compare,
        // then item compare (goto shape reproduces the retail loop layout).
        WindowNode* sentinel = lbl_eu_80664088->mWindowList2.mStartNodePtr;
        WindowNode* n = sentinel->mNext;
        goto check;
    body:
        IUIWindow* item = n->mItem;
        if (item->field_0x68 == a1) {
            item->SetRemove();
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

    CTalkWindow* win = func_8012CC78((CProcess*)lbl_eu_80664088->unk9C,
                                     (u32)lbl_eu_80664088->unk58, a1,
                                     (const u8*)a2, (u32)found, 0, a3);
    if (win == NULL) {
        return NULL;
    }

    lbl_eu_80664088->mWindowList2.push_back((IUIWindow*)win);
    return (IUIWindow*)win;
}
void func_8013D1E8(){}
// Window factory: create the simple eve-talk window on the secondary queue.
// Returns 0 when the manager singleton is absent, the layout manager is not
// ready, or the factory declined (message handed to the existing window).
extern "C" IUIWindow* func_8013D448(u32 text, const u8* msgSrc) {
    if (lbl_eu_80664088 == NULL) {
        return NULL;
    }
    if (func_801355F4() == NULL) {
        return NULL;
    }

    // Retail reloads the singleton for the factory args (its live range ends
    // at the guard check) instead of keeping it in a saved register.
    IUIWindow* window = func_801A20DC((CProcess*)lbl_eu_80664088->unk9C,
                                      lbl_eu_80664088->unk58, text, msgSrc, 0);
    if (window == NULL) {
        return NULL;
    }

    lbl_eu_80664088->mWindowList2.push_back(window);
    return window;
}

// Window factory: create a system window on the primary queue. `str` is a
// non-empty string (the window title); returns 0 when the singleton is absent,
// the string is empty, or the entry check rejects the request.
extern "C" IUIWindow* func_8013D55C(const char* str, void* arg1, u32 arg2) {
    if (lbl_eu_80664088 == NULL) {
        return NULL;
    }
    if (str[0] == 0) {
        return NULL;
    }
    if (arg2 == 0 && func_801356BC() != 0) {
        return NULL;
    }

    IUIWindow* window = func_80124AEC((CProcess*)lbl_eu_80664088->unk9C,
                                      arg1 != NULL ? arg1 : lbl_eu_80664088->unk58,
                                      0, (u32)str, 0);
    if (window == NULL) {
        return NULL;
    }

    lbl_eu_80664088->mWindowList1.push_back(window);
    return window;
}

// Window factory: create a system window on the primary queue (flag = 1
// variant). Returns 0 when the singleton is absent or the entry check rejects.
extern "C" IUIWindow* func_8013D688(u32 arg0, void* arg1, void* arg2, u32 arg3) {
    if (lbl_eu_80664088 == NULL) {
        return NULL;
    }
    if (arg3 == 0 && func_801356E0() != 0) {
        return NULL;
    }

    IUIWindow* window = func_80124AEC((CProcess*)lbl_eu_80664088->unk9C,
                                      arg2 != NULL ? arg2 : lbl_eu_80664088->unk58,
                                      1, arg0, (u32)arg1);
    if (window == NULL) {
        return NULL;
    }

    lbl_eu_80664088->mWindowList1.push_back(window);
    return window;
}
// Retail window creator: create a CSysWinBuff window under the manager's
// parent/scene and queue it on the primary window list.
extern "C" IUIWindow* func_8013D7C0(u32 id) {
    CUIWindowManager* inst = lbl_eu_80664088;
    if (inst == NULL) {
        return NULL;
    }

    IUIWindow* window = create__11CSysWinBuffFv((CProcess*)inst->unk9C, inst->unk58, (u16)id);
    if (window == NULL) {
        return NULL;
    }

    // Re-read the singleton: retail keeps `inst` in a volatile register (its
    // live range ends at the call) and reloads the global for the push_back.
    lbl_eu_80664088->mWindowList1.push_back(window);
    return window;
}

void func_8013D8A0(){}
void func_8013D978(){}
// Window factory: create the quest window on the primary queue. Returns 0
// when the manager singleton is absent or the entry check rejects the id;
// note this creator does NOT null-check the factory result before queueing.
extern "C" IUIWindow* func_8013DA60(u32 id, void* arg1, u32 arg2) {
    if (lbl_eu_80664088 == NULL) {
        return NULL;
    }
    if (arg2 == 0 && func_80135694((u16)id) != 0) {
        return NULL;
    }

    // Retail reloads the singleton for the factory args (its live range ends
    // at the guard check) instead of keeping it in a saved register.
    IUIWindow* window = func_80122B2C((CProcess*)lbl_eu_80664088->unk9C,
                                      lbl_eu_80664088->unk58, id, (u32)arg1);

    // Re-read the singleton: retail keeps `inst` in a volatile register (its
    // live range ends at the call) and reloads the global for the push_back.
    lbl_eu_80664088->mWindowList1.push_back(window);
    return window;
}
// Window factory: create a window through the CMenuUpdate factory and queue
// it on the primary window list. Type 1 (quest) windows are rejected when the
// quest table already reports the row as shown. Retail inlines
// reslist::push_back (find-first-empty-slot + splice onto the sentinel).
extern "C" IUIWindow* func_8013DB6C(int first, u32 second, s32 third,
                                    s32 fourth) {
    if (lbl_eu_80664088 == NULL) {
        return NULL;
    }
    if (first == 1) {
        int idx = func_80138138((int)second);
        if (func_801361E8((u32)lbl_eu_80573D18[idx], lbl_eu_8050097C,
                          second) == 2) {
            return NULL;
        }
    }

    IUIWindow* win = func_80142B4C((CProcess*)lbl_eu_80664088->unk9C,
                                   lbl_eu_80664088->unk58, (int)first,
                                   (int)second, (int)third, (int)fourth);
    if (win == NULL) {
        return NULL;
    }

    lbl_eu_80664088->mWindowList1.push_back(win);
    return win;
}
void func_8013DCAC(){}
void func_8013DD94(){}
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

    // Re-read the singleton: retail keeps `inst` in a volatile register (its
    // live range ends at the call) and reloads the global for the push_back.
    lbl_eu_80664088->mWindowList1.push_back(window);
    return window;
}

void func_8013DF44(){}
void func_8013E030(){}
void func_8013E104(){}
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

    // Re-read the singleton: retail keeps `inst` in a volatile register (its
    // live range ends at the call) and reloads the global for the push_back.
    lbl_eu_80664088->mWindowList1.push_back(window);
    return window;
}

// Window factory: create an item-multi window on the primary queue. The
// entry guard only runs when arg7 is clear. The 9th argument is a byte flag
// forwarded to the factory (retail reads it from the stack slot's low byte).
extern "C" IUIWindow* func_8013E2E0(u32 a1, u32 a2, u32 a3, u32 a4, u32 a5,
                                    u32 a6, u32 a7, u32 a8, u8 a9) {
    if (lbl_eu_80664088 == NULL) {
        return NULL;
    }
    if (a7 == 0 && func_80135654(a1, a2, a3, a4, a5)) {
        return NULL;
    }

    IUIWindow* win = func_801B46E4((CProcess*)lbl_eu_80664088->unk9C,
                                   lbl_eu_80664088->unk58, a1, a2, a3, a4,
                                   a6, a8, a9);
    if (win == NULL) {
        return NULL;
    }

    lbl_eu_80664088->mWindowList1.push_back(win);
    return win;
}
void func_8013E424(){}
// Retail window creator: create the kizuna-talk window and queue it on the
// primary window list.
extern "C" IUIWindow* func_8013E52C(u32 charId) {
    CUIWindowManager* inst = lbl_eu_80664088;
    if (inst == NULL) {
        return NULL;
    }

    IUIWindow* window = func_801BCEBC((CProcess*)inst->unk9C, inst->unk58, charId);
    if (window == NULL) {
        return NULL;
    }

    // Re-read the singleton: retail keeps `inst` in a volatile register (its
    // live range ends at the call) and reloads the global for the push_back.
    lbl_eu_80664088->mWindowList1.push_back(window);
    return window;
}

void func_8013E608(){}
void func_8013E704(){}
void func_8013E800(){}
void func_8013E8E0(){}
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

    // Re-read the singleton: retail keeps `inst` in a volatile register (its
    // live range ends at the call) and reloads the global for the push_back.
    lbl_eu_80664088->mWindowList1.push_back(window);
    return window;
}

void func_8013EAB0(){}
void func_8013EB90(){}
void __dt__Q216CUIWindowManager5CTestFv(){}
void func_8013F244(){}
void func_8013F2A0(){}
void func_8013F354(){}
extern "C" int func_8013F3F0(u8* flagBuf) { return 0; }
void func_8013F6C4(){}
extern "C" void func_8013FFF8(void* flagBuf, void* entry, u32 value){}
void func_80140854(){}
void func_80140AFC(){}
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
                if (flagBuf[9] == 0 && func_8013F3F0(flagBuf) != 0) {
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
void func_80141270(){}
// Flag-buffer build: zero the 0xC8-byte flag buffer, mark it active, refresh
// the per-table entry pointers (func_8003AA34), then scan every table's rows
// for `row + baseOffset[i] == target`; on a hit set the flag for that table
// (func_8013FFF8) and return the buffer, otherwise 0.
extern "C" u8* func_801412D0(u32 target) {
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
        void* entry = lbl_eu_80573D18[i];
        int count = (int)func_8003B1EC(entry);
        for (int j = 0; j < count; j++) {
            u32 tmp[28];
            for (int k = 0; k < 14; k++) {
                ((u64*)tmp)[k] = ((u64*)work)[k];
            }
            if (j + tmp[i] == target) {
                func_8013FFF8(flagBuf, entry, (u32)(j + tmp[i]));
                return flagBuf;
            }
        }
    }
    return NULL;
}
void sinit_801413F8(){}
