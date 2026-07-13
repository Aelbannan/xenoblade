#include <types.h>

#include "cviewroot_mock.h"

extern CViewRootMockRoot* rb_lbl_eu_806655D0;

/*
 * Retail getView semantics for PPC harness (full CViewRoot.o crashes mwldeppc).
 * Matches host oracle in host/cviewroot_getview.cpp.
 */
void* rb_getView__9CViewRootFUl(u32 id) {
    CViewRootMockRoot* root;
    CViewRootMockNode* sentinel;
    CViewRootMockNode* node;
    CViewRootMockThread* thread;

    root = rb_lbl_eu_806655D0;
    if (root == 0) {
        return 0;
    }

    sentinel = root->mStartNodePtr;
    node = sentinel->mNext;
    thread = 0;

    while (node != sentinel) {
        thread = cviewroot_mock_get_work_thread(id, 0);
        if (thread != 0) {
            break;
        }
        node = node->mNext;
    }

    if (thread == 0) {
        return 0;
    }

    if (thread->mType >= CVIEWROOT_THREAD_CVIEW) {
        if (thread->mType >= CVIEWROOT_THREAD_CVIEW_MAX) {
            thread = 0;
        }
    } else {
        thread = 0;
    }

    return thread;
}
