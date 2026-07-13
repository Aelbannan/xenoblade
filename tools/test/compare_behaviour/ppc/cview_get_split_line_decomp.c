#include <types.h>

#include "cview_mock.h"

extern CView* dc_getView1__11CSplitFrameFv(void* splitFrame);
extern CView* dc_getView2__11CSplitFrameFv(void* splitFrame);
extern s16 dc_getSplitLine__11CSplitFrameFv(void* splitFrame);

/* Mirrors libs/monolib/src/core/CView.cpp CView::getSplitLine(). */
s16 dc_getSplitLine__5CViewFv(CView* view) {
    void* splitFrame;
    u32 hasView2;
    u32 hasView1;

    splitFrame = *(void**)((u8*)view + CVIEW_OFF_UNK45C);
    hasView2 = 0;
    hasView1 = 0;
    if (splitFrame == 0) {
        goto getSplitLine_no_view1;
    }
    if (dc_getView1__11CSplitFrameFv(splitFrame) == 0) {
        goto getSplitLine_no_view1;
    }
    hasView1 = 1;

getSplitLine_no_view1:
    if (hasView1 == 0) {
        goto getSplitLine_no_view2;
    }
    splitFrame = *(void**)((u8*)view + CVIEW_OFF_UNK45C);
    if (dc_getView2__11CSplitFrameFv(splitFrame) == 0) {
        goto getSplitLine_no_view2;
    }
    hasView2 = 1;

getSplitLine_no_view2:
    if (hasView2 != 0) {
        goto getSplitLine_call;
    }
    return 0;

getSplitLine_call:
    splitFrame = *(void**)((u8*)view + CVIEW_OFF_UNK45C);
    return dc_getSplitLine__11CSplitFrameFv(splitFrame);
}
