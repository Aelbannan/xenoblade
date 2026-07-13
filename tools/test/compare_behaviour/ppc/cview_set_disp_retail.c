#include <types.h>

#include "cview_mock.h"

extern CView* rb_getCurrent__9CViewRootFv(void);
extern void rb_invalidCurrent__9CViewRootFP5CView(CView* view);

void rb_setDisp__5CViewFbb(CView* view, int disp, int invalidate) {
    u32 flags;

    flags = cview_get_unk278(view);
    if (!disp) {
        flags |= 0x40u;
    } else {
        flags &= ~0x40u;
    }
    cview_set_unk278(view, flags);

    if (invalidate && !disp) {
        if (rb_getCurrent__9CViewRootFv() == view) {
            rb_invalidCurrent__9CViewRootFP5CView(view);
        }
    }
}
