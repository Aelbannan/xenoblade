#include "cviewframe_render_mock.h"

int rb_render__10CViewFrameFv(void* frame) {
    (void)frame;
    cviewframe_render_eval(0);
    return g_rb_cfr_out.returnedFalse ? 0 : 1;
}
