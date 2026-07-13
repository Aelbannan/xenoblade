#include <types.h>

#include "cview_mock.h"

extern CView* dc_getView1__11CSplitFrameFv(void* splitFrame);
extern CView* dc_getView2__11CSplitFrameFv(void* splitFrame);
extern void dc_setSplitLine__11CSplitFrameFs(void* splitFrame, s16 line);

/* Mirrors libs/monolib/src/core/CView.cpp CView::setSplitLine(). */
void dc_setSplitLine__5CViewFs(CView* view, s16 line) {
    void* splitFrame;
    u32 hasView2;
    u32 hasView1;

    splitFrame = *(void**)((u8*)view + CVIEW_OFF_UNK45C);
    hasView2 = 0;
    hasView1 = 0;
    if (splitFrame == 0) {
        goto setSplitLine_no_view1;
    }
    if (dc_getView1__11CSplitFrameFv(splitFrame) == 0) {
        goto setSplitLine_no_view1;
    }
    hasView1 = 1;

setSplitLine_no_view1:
    if (hasView1 == 0) {
        goto setSplitLine_no_view2;
    }
    splitFrame = *(void**)((u8*)view + CVIEW_OFF_UNK45C);
    if (dc_getView2__11CSplitFrameFv(splitFrame) == 0) {
        goto setSplitLine_no_view2;
    }
    hasView2 = 1;

setSplitLine_no_view2:
    if (hasView2 == 0) {
        return;
    }
    splitFrame = *(void**)((u8*)view + CVIEW_OFF_UNK45C);
    dc_setSplitLine__11CSplitFrameFs(splitFrame, line);
}
