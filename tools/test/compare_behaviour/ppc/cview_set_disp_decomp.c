#include <types.h>

#include "cview_mock.h"

extern CView* dc_getCurrent__9CViewRootFv(void);
extern void dc_invalidCurrent__9CViewRootFP5CView(CView* view);

/* Mirrors libs/monolib/src/core/CView.cpp CView::setDisp(). */
void dc_setDisp__5CViewFbb(CView* view, int disp, int invalidate) {
    u32 flags;

    flags = cview_get_unk278(view);
    if (!disp) {
        flags |= 0x40u;
    } else {
        flags &= ~0x40u;
    }
    cview_set_unk278(view, flags);

    if (invalidate && !disp) {
        if (dc_getCurrent__9CViewRootFv() == view) {
            dc_invalidCurrent__9CViewRootFP5CView(view);
        }
    }
}
