#ifndef CVIEW_RENDER_VIEW_MOCK_H
#define CVIEW_RENDER_VIEW_MOCK_H

#include <types.h>

typedef struct CViewRenderViewMock {
    int clearEntered;
    int colorUpdateOff;
    int selfWkRender;
    int frameRenderCalled;
    int earlyReturn;
    int tookTail;
    u32 crossRootFlag;
} CViewRenderViewMock;

typedef struct CViewRVScenario {
    u32 unk278;
    int fsvBit1;
    int selfIsFsv;
    int thisListEmpty;
    int fsvListEmpty;
    int sameRoot;
    int thisRootNull;
    s32 unk460;
    s16 sizeX;
    s16 sizeY;
    s16 maxX;
    s16 maxY;
    int splitBothViews;
    u32 mFlags;
    u32 mState;
    int msgHasException;
} CViewRVScenario;

extern CViewRenderViewMock g_rb_rv_mock;
extern CViewRenderViewMock g_dc_rv_mock;

void cview_rv_mock_reset(void);
void cview_rv_set_scenario(int decomp, CViewRVScenario* in);
void cview_rv_eval(int decomp);

#endif
