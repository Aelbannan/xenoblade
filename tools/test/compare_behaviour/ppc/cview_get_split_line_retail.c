#include <types.h>

#include "cview_mock.h"

extern CView* rb_getView1__11CSplitFrameFv(void* splitFrame);
extern CView* rb_getView2__11CSplitFrameFv(void* splitFrame);
extern s16 rb_getSplitLine__11CSplitFrameFv(void* splitFrame);

/*
 * Retail getSplitLine semantics for PPC (full CView.o crashes mwldeppc).
 * Gate shape matches retail: return 0 unless both view1 and view2 exist.
 */
s16 rb_getSplitLine__5CViewFv(CView* view) {
    void* splitFrame;
    u32 hasView2;
    u32 hasView1;

    splitFrame = *(void**)((u8*)view + CVIEW_OFF_UNK45C);
    hasView2 = 0;
    hasView1 = 0;
    if (splitFrame == 0) {
        goto getSplitLine_no_view1;
    }
    if (rb_getView1__11CSplitFrameFv(splitFrame) == 0) {
        goto getSplitLine_no_view1;
    }
    hasView1 = 1;

getSplitLine_no_view1:
    if (hasView1 == 0) {
        goto getSplitLine_no_view2;
    }
    splitFrame = *(void**)((u8*)view + CVIEW_OFF_UNK45C);
    if (rb_getView2__11CSplitFrameFv(splitFrame) == 0) {
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
    return rb_getSplitLine__11CSplitFrameFv(splitFrame);
}
