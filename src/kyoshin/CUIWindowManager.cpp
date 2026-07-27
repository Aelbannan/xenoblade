#include "kyoshin/CUIWindowManager.hpp"

#include <types.h>

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
};

typedef reslist<IUIWindow*>::iterator WindowIter;
typedef _reslist_node<IUIWindow*> WindowNode;

void Term__Q216CUIWindowManager5CTestFv(){}

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
void func_8013F3EC(void){}
extern "C" int func_801413DC(unsigned int arg0, int arg1) { unsigned int low = arg0 & 0xffff; unsigned int high = arg0 >> 16; if (arg1 >= (int)low) return -1; return (int)(high + arg1); }

void __ct__CUIWindowManager(){}
void __dt__CTTask_CUIWindowManager(){}
void func_8013CBB4(){}
void func_8013CFDC(){}
void func_8013D07C(){}
void func_8013D1E8(){}
void func_8013D448(){}
void func_8013D55C(){}
void func_8013D688(){}
void func_8013D7C0(){}
void func_8013D8A0(){}
void func_8013D978(){}
void func_8013DA60(){}
void func_8013DB6C(){}
void func_8013DCAC(){}
void func_8013DD94(){}
void func_8013DE6C(){}
void func_8013DF44(){}
void func_8013E030(){}
void func_8013E104(){}
void func_8013E204(){}
void func_8013E2E0(){}
void func_8013E424(){}
void func_8013E52C(){}
void func_8013E608(){}
void func_8013E704(){}
void func_8013E800(){}
void func_8013E8E0(){}
void func_8013E9D8(){}
void func_8013EAB0(){}
void func_8013EB90(){}
void __dt__Q216CUIWindowManager5CTestFv(){}
void func_8013F244(){}
void func_8013F2A0(){}
void func_8013F354(){}
void func_8013F3F0(){}
void func_8013F6C4(){}
void func_8013FFF8(){}
void func_80140854(){}
void func_80140AFC(){}
void func_80140CA4(){}
void func_80140E00(){}
void func_80141270(){}
void func_801412D0(){}
void sinit_801413F8(){}
