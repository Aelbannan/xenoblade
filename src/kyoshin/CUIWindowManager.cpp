#include "kyoshin/CUIWindowManager.hpp"

#include <types.h>

// Batch 2026-07-14g: window-mgr-move owns Move() exclusively in this TU.

extern "C" {
CUIWindowManager* lbl_eu_80664088;
s16 lbl_eu_8066408C;
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
public:
    void SetRemove() { mIsRemove = true; }

    IUIWindowSubView* unk5C; //0x5C - non-null: timer lives at unk5C->unk828
    s32 unk60;               //0x60 - fallback timer when unk5C is null
    bool unk64;              //0x64 - pending removal flag
    bool unk65;              //0x65 - pending update-mark flag

private:
    u8 unk00[0x39];
    bool mIsRemove; //0x39 (CProcess::mIsRemove)
    u8 unk3A[0x5C - 0x3A];
};

typedef reslist<IUIWindow*>::iterator WindowIter;

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

    // If any window in a queue already needs an update (or a mark-all
    // request is pending), force the flag on every window in that queue.
    {
        bool markAll = false;
        for (WindowIter it = mWindowList1.begin(); it != mWindowList1.end(); ++it) {
            if ((*it)->unk65 != 0 || unkA1 != 0) {
                markAll = true;
                break;
            }
        }
        if (markAll) {
            for (WindowIter it = mWindowList1.begin(); it != mWindowList1.end(); ++it) {
                (*it)->unk65 = true;
            }
        }
    }
    {
        bool markAll = false;
        for (WindowIter it = mWindowList2.begin(); it != mWindowList2.end(); ++it) {
            if ((*it)->unk65 != 0 || unkA1 != 0) {
                markAll = true;
                break;
            }
        }
        if (markAll) {
            for (WindowIter it = mWindowList2.begin(); it != mWindowList2.end(); ++it) {
                (*it)->unk65 = true;
            }
        }
    }

    // Remove every window flagged for closure (or all of them, if a
    // close-all request is pending) from each queue.
    WindowIter pending[18];
    int pendingCount;
    int i;

    pendingCount = 0;
    for (WindowIter it = mWindowList1.begin(); it != mWindowList1.end(); ++it) {
        IUIWindow* window = *it;
        if (window->unk64 != 0 || unkA0 != 0) {
            window->SetRemove();
            pending[pendingCount++] = it;
        }
    }
    for (i = 0; i < pendingCount; i++) {
        mWindowList1.erase(pending[i]);
    }

    pendingCount = 0;
    for (WindowIter it = mWindowList2.begin(); it != mWindowList2.end(); ++it) {
        IUIWindow* window = *it;
        if (window->unk64 != 0 || unkA0 != 0) {
            window->SetRemove();
            pending[pendingCount++] = it;
        }
    }
    for (i = 0; i < pendingCount; i++) {
        mWindowList2.erase(pending[i]);
    }

    unkA0 = false;
    unkA1 = false;
}
