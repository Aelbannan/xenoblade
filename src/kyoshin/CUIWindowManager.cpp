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

    for (WindowIter it = inst->mWindowList1.begin(); it != inst->mWindowList1.end(); ++it) {
        IUIWindow* window = *it;
        s32 timer = window->unk5C != NULL ? window->unk5C->unk828 : window->unk60;
        if (timer > 0) {
            lbl_eu_8066408C = (s16)timer;
            break;
        }
    }
    for (WindowIter it = inst->mWindowList2.begin(); it != inst->mWindowList2.end(); ++it) {
        IUIWindow* window = *it;
        s32 timer = window->unk5C != NULL ? window->unk5C->unk828 : window->unk60;
        if (timer > 0) {
            lbl_eu_8066408C = (s16)timer;
            break;
        }
    }

    {
        WindowIter it = mWindowList1.begin();
        for (; it != mWindowList1.end(); ++it) {
            if ((*it)->unk65 != 0 || unkA1 != 0) {
                break;
            }
        }
        for (; it != mWindowList1.end(); ++it) {
            (*it)->unk65 = true;
        }
    }
    {
        WindowIter it = mWindowList2.begin();
        for (; it != mWindowList2.end(); ++it) {
            if ((*it)->unk65 != 0 || unkA1 != 0) {
                break;
            }
        }
        for (; it != mWindowList2.end(); ++it) {
            (*it)->unk65 = true;
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

// LLM-HARNESS-BEGIN: us-8013cf3c
extern "C" u32 func_8013C54C() {
    return (u32)lbl_eu_80664088;
}
// LLM-HARNESS-END: us-8013cf3c
// LLM-HARNESS-BEGIN: us-8013dc5c
extern "C" void func_8013D26C() {}
// LLM-HARNESS-END: us-8013dc5c
// LLM-HARNESS-BEGIN: us-8013f65c
extern "C" unsigned short func_8013EC58()
{
    return lbl_eu_8066408C;
}
// LLM-HARNESS-END: us-8013f65c
// LLM-HARNESS-BEGIN: us-8013f664
extern "C" void func_8013EC60() {}
// LLM-HARNESS-END: us-8013f664
// LLM-HARNESS-BEGIN: us-8013f670
extern "C" void func_8013EC6C() {}
// LLM-HARNESS-END: us-8013f670
// LLM-HARNESS-BEGIN: us-8013faac
extern "C" void Draw__Q216CUIWindowManager5CTestFv() {}
// LLM-HARNESS-END: us-8013faac
// LLM-HARNESS-BEGIN: us-8013fab8
extern "C" void Init__Q216CUIWindowManager5CTestFv() {}
// LLM-HARNESS-END: us-8013fab8
// LLM-HARNESS-BEGIN: us-8013fc38
extern "C" void* func_8013F234(void* self) {
    extern void* func_8013CBB4(void*);
    return func_8013CBB4((char*)self - 0x54);
}
// LLM-HARNESS-END: us-8013fc38
// LLM-HARNESS-BEGIN: us-8013fc40
extern "C" void __dt__16CUIWindowManagerFv(CUIWindowManager*);
extern "C" void func_8013F23C(CUIWindowManager* p) {
    __dt__16CUIWindowManagerFv((CUIWindowManager*)((char*)p - 0x54));
}
// LLM-HARNESS-END: us-8013fc40
// LLM-HARNESS-BEGIN: us-8013fdf0
extern "C" void func_8013F3EC() {}
// LLM-HARNESS-END: us-8013fdf0
// LLM-HARNESS-BEGIN: us-80141de0
extern "C" void func_801413DC() {}
// LLM-HARNESS-END: us-80141de0

// LLM-HARNESS-BEGIN: us-8013ca58
extern "C" void ct_CUIWindowManager() {}
// LLM-HARNESS-END: us-8013ca58
// LLM-HARNESS-BEGIN: us-8013cc2c
extern "C" void dt_CTTask_CUIWindowManager() {}
// LLM-HARNESS-END: us-8013cc2c
// LLM-HARNESS-BEGIN: us-8013d5a4
extern "C" void func_8013CBB4() {}
// LLM-HARNESS-END: us-8013d5a4
// LLM-HARNESS-BEGIN: us-8013d9cc
extern "C" void func_8013CFDC() {}
// LLM-HARNESS-END: us-8013d9cc
// LLM-HARNESS-BEGIN: us-8013da6c
extern "C" void func_8013D07C() {}
// LLM-HARNESS-END: us-8013da6c
// LLM-HARNESS-BEGIN: us-8013dbd8
extern "C" void func_8013D1E8() {}
// LLM-HARNESS-END: us-8013dbd8
// LLM-HARNESS-BEGIN: us-8013de38
extern "C" void func_8013D448() {}
// LLM-HARNESS-END: us-8013de38
// LLM-HARNESS-BEGIN: us-8013df4c
extern "C" void func_8013D55C() {}
// LLM-HARNESS-END: us-8013df4c
// LLM-HARNESS-BEGIN: us-8013e08c
extern "C" void func_8013D688() {}
// LLM-HARNESS-END: us-8013e08c
// LLM-HARNESS-BEGIN: us-8013e1c4
extern "C" void func_8013D7C0() {}
// LLM-HARNESS-END: us-8013e1c4
// LLM-HARNESS-BEGIN: us-8013e2a4
extern "C" void func_8013D8A0() {}
// LLM-HARNESS-END: us-8013e2a4
// LLM-HARNESS-BEGIN: us-8013e37c
extern "C" void func_8013D978() {}
// LLM-HARNESS-END: us-8013e37c
// LLM-HARNESS-BEGIN: us-8013e464
extern "C" void func_8013DA60() {}
// LLM-HARNESS-END: us-8013e464
// LLM-HARNESS-BEGIN: us-8013e570
extern "C" void func_8013DB6C() {}
// LLM-HARNESS-END: us-8013e570
// LLM-HARNESS-BEGIN: us-8013e6b0
extern "C" void func_8013DCAC() {}
// LLM-HARNESS-END: us-8013e6b0
// LLM-HARNESS-BEGIN: us-8013e798
extern "C" void func_8013DD94() {}
// LLM-HARNESS-END: us-8013e798
// LLM-HARNESS-BEGIN: us-8013e870
extern "C" void func_8013DE6C() {}
// LLM-HARNESS-END: us-8013e870
// LLM-HARNESS-BEGIN: us-8013e948
extern "C" void func_8013DF44() {}
// LLM-HARNESS-END: us-8013e948
// LLM-HARNESS-BEGIN: us-8013ea34
extern "C" void func_8013E030() {}
// LLM-HARNESS-END: us-8013ea34
// LLM-HARNESS-BEGIN: us-8013eb08
extern "C" void func_8013E104() {}
// LLM-HARNESS-END: us-8013eb08
// LLM-HARNESS-BEGIN: us-8013ec08
extern "C" void func_8013E204() {}
// LLM-HARNESS-END: us-8013ec08
// LLM-HARNESS-BEGIN: us-8013ece4
extern "C" void func_8013E2E0() {}
// LLM-HARNESS-END: us-8013ece4
// LLM-HARNESS-BEGIN: us-8013ee28
extern "C" void func_8013E424() {}
// LLM-HARNESS-END: us-8013ee28
// LLM-HARNESS-BEGIN: us-8013ef30
extern "C" void func_8013E52C() {}
// LLM-HARNESS-END: us-8013ef30
// LLM-HARNESS-BEGIN: us-8013f00c
extern "C" void func_8013E608() {}
// LLM-HARNESS-END: us-8013f00c
// LLM-HARNESS-BEGIN: us-8013f108
extern "C" void func_8013E704() {}
// LLM-HARNESS-END: us-8013f108
// LLM-HARNESS-BEGIN: us-8013f204
extern "C" void func_8013E800() {}
// LLM-HARNESS-END: us-8013f204
// LLM-HARNESS-BEGIN: us-8013f2e4
extern "C" void func_8013E8E0() {}
// LLM-HARNESS-END: us-8013f2e4
// LLM-HARNESS-BEGIN: us-8013f3dc
extern "C" void func_8013E9D8() {}
// LLM-HARNESS-END: us-8013f3dc
// LLM-HARNESS-BEGIN: us-8013f4b4
extern "C" void func_8013EAB0() {}
// LLM-HARNESS-END: us-8013f4b4
// LLM-HARNESS-BEGIN: us-8013f594
extern "C" void func_8013EB90() {}
// LLM-HARNESS-END: us-8013f594
// LLM-HARNESS-BEGIN: us-8013fbdc
extern "C" void dt_Q216CUIWindowManager5CTestFv() {}
// LLM-HARNESS-END: us-8013fbdc
// LLM-HARNESS-BEGIN: us-8013fc48
extern "C" void func_8013F244() {}
// LLM-HARNESS-END: us-8013fc48
// LLM-HARNESS-BEGIN: us-8013fca4
extern "C" void func_8013F2A0() {}
// LLM-HARNESS-END: us-8013fca4
// LLM-HARNESS-BEGIN: us-8013fd58
extern "C" void func_8013F354() {}
// LLM-HARNESS-END: us-8013fd58
// LLM-HARNESS-BEGIN: us-8013fdf4
extern "C" void func_8013F3F0() {}
// LLM-HARNESS-END: us-8013fdf4
// LLM-HARNESS-BEGIN: us-801400c8
extern "C" void func_8013F6C4() {}
// LLM-HARNESS-END: us-801400c8
// LLM-HARNESS-BEGIN: us-801409fc
extern "C" void func_8013FFF8() {}
// LLM-HARNESS-END: us-801409fc
// LLM-HARNESS-BEGIN: us-80141258
extern "C" void func_80140854() {}
// LLM-HARNESS-END: us-80141258
// LLM-HARNESS-BEGIN: us-80141500
extern "C" void func_80140AFC() {}
// LLM-HARNESS-END: us-80141500
// LLM-HARNESS-BEGIN: us-801416a8
extern "C" void func_80140CA4() {}
// LLM-HARNESS-END: us-801416a8
// LLM-HARNESS-BEGIN: us-80141804
extern "C" void func_80140E00() {}
// LLM-HARNESS-END: us-80141804
// LLM-HARNESS-BEGIN: us-80141c74
extern "C" void func_80141270() {}
// LLM-HARNESS-END: us-80141c74
// LLM-HARNESS-BEGIN: us-80141cd4
extern "C" void func_801412D0() {}
// LLM-HARNESS-END: us-80141cd4
// LLM-HARNESS-BEGIN: us-80141dfc
extern "C" void sinit_801413F8() {}
// LLM-HARNESS-END: us-80141dfc
