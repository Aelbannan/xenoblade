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
// `reslist<P9IUIWindow>` / `__dt__9IUIWindowFv`). Only the fields touched by
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
    cf::IFlagEvent* flagEvent = this; // implicit MI conversion — do not static_cast / ternary / if
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

    // Report the first live transition timer found in either queue.
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

    // Walk each queue looking for the first window that already needs an
    // update (or a global mark-all request); once found, propagate the
    // update flag to that window and every window after it in the queue.
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

    // Remove every window flagged for closure (or all of them, if a
    // close-all request is pending) from each queue. The pending buffer is
    // an array of raw list-node pointers (a trivial type) rather than
    // reslist<T>::iterator, since iterator's non-trivial default ctor would
    // force MWCC to zero-init all 18 slots up front - overhead retail's
    // code never pays.
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
    }

    unkA0 = false;
    unkA1 = false;
}
