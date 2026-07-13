#include <types.h>

#include "cviewroot_mock.h"

extern CViewRootMockRoot* dc_lbl_eu_806655D0;

static CViewRootMockThread* convert_to_view(CViewRootMockThread* thread) {
    if (thread == 0) {
        return 0;
    }

    if (thread->mType >= CVIEWROOT_THREAD_CVIEW) {
        if (thread->mType >= CVIEWROOT_THREAD_CVIEW_MAX) {
            return 0;
        }
    } else {
        return 0;
    }

    return thread;
}

/* Decomp getView semantics (trimmed dc_getView__9CViewRootFUl .o crashes mwldeppc). */
void* dc_getView__9CViewRootFUl(u32 id) {
    CViewRootMockRoot* root;
    CViewRootMockNode* sentinel;
    CViewRootMockNode* node;
    CViewRootMockThread* thread;

    root = dc_lbl_eu_806655D0;
    if (root == 0) {
        return 0;
    }

    sentinel = root->mStartNodePtr;
    node = sentinel->mNext;
    thread = 0;

    while (node != dc_lbl_eu_806655D0->mStartNodePtr) {
        thread = cviewroot_mock_get_work_thread(id, 1);
        if (thread != 0) {
            break;
        }
        node = node->mNext;
    }

    return convert_to_view(thread);
}
