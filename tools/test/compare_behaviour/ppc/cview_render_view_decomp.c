#include "cview_render_view_mock.h"

/* Decomp semantic slice — mirrors host/cview_render_view.cpp gate oracle. */
void dc_renderView__5CViewFv(void* view) {
    (void)view;
    cview_rv_eval(1);
}
