#include "cviewframe_render_mock.h"

#include <string.h>

CViewFrameRenderOut g_rb_cfr_out;
CViewFrameRenderOut g_dc_cfr_out;

static CViewFrameScenario s_rb_in;
static CViewFrameScenario s_dc_in;

void cviewframe_render_mock_reset(void) {
    memset(&g_rb_cfr_out, 0, sizeof(g_rb_cfr_out));
    memset(&g_dc_cfr_out, 0, sizeof(g_dc_cfr_out));
    memset(&s_rb_in, 0, sizeof(s_rb_in));
    memset(&s_dc_in, 0, sizeof(s_dc_in));
}

void cviewframe_render_set(int decomp, CViewFrameScenario* in) {
    if (decomp) {
        s_dc_in = *in;
    } else {
        s_rb_in = *in;
    }
}

void cviewframe_render_eval(int decomp) {
    CViewFrameScenario* in = decomp ? &s_dc_in : &s_rb_in;
    CViewFrameRenderOut* out = decomp ? &g_dc_cfr_out : &g_rb_cfr_out;
    s16 w;
    s16 h;
    int expand;
    int expandApplied;
    int expand2;
    int badSize;

    memset(out, 0, sizeof(*out));

    if (in->ownerNull) {
        out->returnedFalse = 1;
        return;
    }

    w = in->sizeX;
    h = in->sizeY;
    expand = 0;
    if ((in->unk27C & 1u) != 0) {
        if ((in->unk278 & 1u) == 0 && (in->unk278 & 2u) == 0) {
            expand = 1;
        }
    }

    expandApplied = 0;
    if (expand != 0) {
        w = (s16)(w + (s16)(in->border * 2));
        expand2 = 0;
        if ((in->unk27C & 2u) != 0) {
            if ((in->unk278 & 1u) == 0 && (in->unk278 & 2u) == 0) {
                expand2 = 1;
            }
        }
        if (expand2 != 0) {
            h = (s16)(h + (s16)(in->border * 3 + 0x16));
            expandApplied = 2;
        } else {
            h = (s16)(h + (s16)(in->border * 2));
            expandApplied = 1;
        }
    }

    badSize = 0;
    if (w <= 0) {
        badSize = 1;
    } else if (h <= 0) {
        badSize = 1;
    }
    if (badSize != 0) {
        out->returnedFalse = 1;
        out->outW = w;
        out->outH = h;
        out->expandApplied = expandApplied;
        return;
    }

    out->outW = w;
    out->outH = h;
    out->expandApplied = expandApplied;
}
