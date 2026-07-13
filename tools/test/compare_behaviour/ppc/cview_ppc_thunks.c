#include <types.h>

/*
 * MWCC treats __5CView* names at call sites as CView member functions.
 * Call through explicit symbol addresses instead.
 */
extern char rb_getCurrentView__5CViewFv[];
extern char dc_getCurrentView__5CViewFv[];
extern char rb_getSplitLine__5CViewFv[];
extern char dc_getSplitLine__5CViewFv[];
extern char rb_setSplitLine__5CViewFs[];
extern char dc_setSplitLine__5CViewFs[];
extern char rb_setDisp__5CViewFbb[];
extern char dc_setDisp__5CViewFbb[];

typedef void* (*CViewGetCurrentFn)(void);
typedef s16 (*CViewGetSplitLineFn)(void* view);
typedef void (*CViewSetSplitLineFn)(void* view, s16 line);
typedef void (*CViewSetDispFn)(void* view, int disp, int invalidate);

void* ppc_rb_getCurrentView(void) {
    return ((CViewGetCurrentFn)&rb_getCurrentView__5CViewFv)();
}

void* ppc_dc_getCurrentView(void) {
    return ((CViewGetCurrentFn)&dc_getCurrentView__5CViewFv)();
}

s16 ppc_rb_getSplitLine(void* view) {
    return ((CViewGetSplitLineFn)&rb_getSplitLine__5CViewFv)(view);
}

s16 ppc_dc_getSplitLine(void* view) {
    return ((CViewGetSplitLineFn)&dc_getSplitLine__5CViewFv)(view);
}

void ppc_rb_setSplitLine(void* view, s16 line) {
    ((CViewSetSplitLineFn)&rb_setSplitLine__5CViewFs)(view, line);
}

void ppc_dc_setSplitLine(void* view, s16 line) {
    ((CViewSetSplitLineFn)&dc_setSplitLine__5CViewFs)(view, line);
}

void ppc_rb_setDisp(void* view, int disp, int invalidate) {
    ((CViewSetDispFn)&rb_setDisp__5CViewFbb)(view, disp, invalidate);
}

void ppc_dc_setDisp(void* view, int disp, int invalidate) {
    ((CViewSetDispFn)&dc_setDisp__5CViewFbb)(view, disp, invalidate);
}
