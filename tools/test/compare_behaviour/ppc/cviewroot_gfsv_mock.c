#include <string.h>

#include "cviewroot_gfsv_mock.h"

CViewRootGfsvState g_rb_gfsv_mock;
CViewRootGfsvState g_dc_gfsv_mock;

void cviewroot_gfsv_mock_reset(void) {
    memset(&g_rb_gfsv_mock, 0, sizeof(g_rb_gfsv_mock));
    memset(&g_dc_gfsv_mock, 0, sizeof(g_dc_gfsv_mock));
    g_rb_gfsv_mock.fbWidth = 640;
    g_rb_gfsv_mock.efbHeight = 480;
    g_dc_gfsv_mock.fbWidth = 640;
    g_dc_gfsv_mock.efbHeight = 480;
}

CViewRootGfsvChild* cviewroot_gfsv_add_child(int decomp) {
    CViewRootGfsvState* mock = decomp ? &g_dc_gfsv_mock : &g_rb_gfsv_mock;
    CViewRootGfsvChild* child;

    if (mock->childCount >= GFSV_MAX_CHILDREN) {
        return 0;
    }

    child = &mock->children[mock->childCount++];
    memset(child, 0, sizeof(*child));
    child->isView = 1;
    child->mState = GFSV_THREAD_STATE_RUN;
    child->viewW = 640;
    child->viewH = 480;
    return child;
}

static int msg_has_exception(const CViewRootGfsvChild* child) {
    int i;

    for (i = 0; i < child->msgCount; ++i) {
        if (child->msgs[i] == GFSV_EVT_EXCEPTION) {
            return 1;
        }
    }
    return 0;
}

/*
 * Shared gate matching retail asm @0x80445314 / host oracle.
 * Return codes: 0 = null, 1..N = child index+1, 0xFFFFFFFE = desktop (-2).
 * Encoded as pointers so harness can compare without hardcoding layouts.
 */
void* cviewroot_gfsv_select(int decomp) {
    CViewRootGfsvState* mock = decomp ? &g_dc_gfsv_mock : &g_rb_gfsv_mock;
    int i;

    if (!mock->hasRoot) {
        return 0;
    }
    if (!mock->hasDesktop) {
        return 0;
    }

    for (i = 0; i < mock->childCount; ++i) {
        CViewRootGfsvChild* child = &mock->children[i];
        u32 viewFlags;
        u32 msgQualified;
        u32 keepGoing;
        u32 loginRunKeep;
        s16 posSumY;
        s16 posSumX;

        if (!child->isView) {
            continue;
        }

        viewFlags = child->mFlags;
        msgQualified = (viewFlags & GFSV_THREAD_FLAG_EXCEPTION) ? 1u : (msg_has_exception(child) ? 1u : 0u);

        keepGoing = 0;
        if (msgQualified != 0) {
        } else {
            loginRunKeep = 1;
            if (child->mState == GFSV_THREAD_STATE_LOGIN) {
            } else if (child->mState == GFSV_THREAD_STATE_RUN) {
            } else {
                loginRunKeep = 0;
            }
            if (loginRunKeep != 0) {
                keepGoing = 1;
            }
        }
        if (keepGoing == 0) {
            continue;
        }

        if ((viewFlags & GFSV_THREAD_FLAG_NO_EVENT) != 0) {
            continue;
        }

        if ((child->unk278 & 1u) != 0) {
            return (void*)(u32)(i + 1);
        }

        posSumY = (s16)(child->posX + child->frameOffX);
        posSumX = (s16)(child->posY + child->frameOffY);

        if ((child->unk278 & 4u) != 0) {
            continue;
        }
        if (posSumY != 0) {
            continue;
        }
        if (posSumX != 0) {
            continue;
        }
        if (child->viewW != mock->fbWidth) {
            continue;
        }
        if (child->viewH != mock->efbHeight) {
            continue;
        }
        return (void*)(u32)(i + 1);
    }

    /* desktop fallback */
    return (void*)0xFFFFFFFEu;
}
