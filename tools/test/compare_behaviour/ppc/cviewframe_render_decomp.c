#include "cviewframe_render_mock.h"

int dc_render__10CViewFrameFv(void* frame) {
    (void)frame;
    cviewframe_render_eval(1);
    return g_dc_cfr_out.returnedFalse ? 0 : 1;
}
