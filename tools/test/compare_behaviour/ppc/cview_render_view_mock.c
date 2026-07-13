#include "cview_render_view_mock.h"

#include <string.h>

CViewRenderViewMock g_rb_rv_mock;
CViewRenderViewMock g_dc_rv_mock;

static CViewRVScenario s_rb_in;
static CViewRVScenario s_dc_in;

void cview_rv_mock_reset(void) {
    memset(&g_rb_rv_mock, 0, sizeof(g_rb_rv_mock));
    memset(&g_dc_rv_mock, 0, sizeof(g_dc_rv_mock));
    memset(&s_rb_in, 0, sizeof(s_rb_in));
    memset(&s_dc_in, 0, sizeof(s_dc_in));
}

void cview_rv_set_scenario(int decomp, CViewRVScenario* in) {
    if (decomp) {
        s_dc_in = *in;
    } else {
        s_rb_in = *in;
    }
}

#define THREAD_FLAG_EXCEPTION (1u << 4)
#define THREAD_STATE_LOGIN 2u
#define THREAD_STATE_RUN 3u

void cview_rv_eval(int decomp) {
    CViewRVScenario* in = decomp ? &s_dc_in : &s_rb_in;
    CViewRenderViewMock* mock = decomp ? &g_dc_rv_mock : &g_rb_rv_mock;
    u32 cross;
    u32 msgQualified;
    u32 stateReady;

    memset(mock, 0, sizeof(*mock));

    if ((in->unk278 & 0x40u) != 0) {
        mock->earlyReturn = 1;
        return;
    }

    cross = 0;
    if (!in->fsvBit1 && !in->selfIsFsv && !in->thisListEmpty && !in->fsvListEmpty) {
        if (!in->sameRoot && !in->thisRootNull && in->unk460 >= 7) {
            cross = 1;
        }
    }
    mock->crossRootFlag = cross;

    if (in->sizeX <= 0 || in->sizeY <= 0) {
        mock->tookTail = 1;
        mock->frameRenderCalled = (cross == 0);
        return;
    }
    if (in->maxX <= 0 && in->maxY <= 0) {
        mock->tookTail = 1;
        mock->frameRenderCalled = (cross == 0);
        return;
    }

    if (cross == 0) {
        if (!in->splitBothViews && (in->unk278 & 1u) == 0) {
            mock->clearEntered = 1;
            mock->colorUpdateOff = ((in->unk278 >> 2) & 1u) != 0;
        }
    }

    msgQualified = 0;
    if ((in->mFlags & THREAD_FLAG_EXCEPTION) != 0) {
        msgQualified = 1;
    } else if (in->msgHasException) {
        msgQualified = 1;
    }

    stateReady = 0;
    if (in->mState == THREAD_STATE_LOGIN || in->mState == THREAD_STATE_RUN) {
        stateReady = 1;
    }
    mock->selfWkRender = (msgQualified == 0 && stateReady != 0);
    mock->frameRenderCalled = (cross == 0);
}
