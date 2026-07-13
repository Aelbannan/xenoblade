#ifndef CVIEWFRAME_RENDER_MOCK_H
#define CVIEWFRAME_RENDER_MOCK_H

#include <types.h>

typedef struct CViewFrameScenario {
    int ownerNull;
    u32 unk27C;
    u32 unk278;
    s16 sizeX;
    s16 sizeY;
    s16 border;
} CViewFrameScenario;

typedef struct CViewFrameRenderOut {
    int returnedFalse;
    s16 outW;
    s16 outH;
    int expandApplied;
} CViewFrameRenderOut;

extern CViewFrameRenderOut g_rb_cfr_out;
extern CViewFrameRenderOut g_dc_cfr_out;

void cviewframe_render_mock_reset(void);
void cviewframe_render_set(int decomp, CViewFrameScenario* in);
void cviewframe_render_eval(int decomp);

#endif
