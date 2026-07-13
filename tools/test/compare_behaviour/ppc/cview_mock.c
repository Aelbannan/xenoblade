#include <string.h>

#include "cview_mock.h"

CViewMockState g_rb_cview_mock;
CViewMockState g_dc_cview_mock;

static CViewMockState* mock_for(int decomp) {
    return decomp ? &g_dc_cview_mock : &g_rb_cview_mock;
}

void cview_mock_reset(void) {
    memset(&g_rb_cview_mock, 0, sizeof(g_rb_cview_mock));
    memset(&g_dc_cview_mock, 0, sizeof(g_dc_cview_mock));
}

u32 cview_get_unk278(CView* view) {
    u32 value;
    memcpy(&value, (u8*)view + CVIEW_OFF_UNK278, sizeof(value));
    return value;
}

void cview_set_unk278(CView* view, u32 value) {
    memcpy((u8*)view + CVIEW_OFF_UNK278, &value, sizeof(value));
}

void cview_set_unk45c(CView* view, void* splitFrame) {
    memcpy((u8*)view + CVIEW_OFF_UNK45C, &splitFrame, sizeof(splitFrame));
}

CView* cview_mock_get_view1(void* splitFrame, int decomp) {
    CViewMockState* mock = mock_for(decomp);
    if (splitFrame != mock->splitFrame) {
        return 0;
    }
    return mock->view1;
}

CView* cview_mock_get_view2(void* splitFrame, int decomp) {
    CViewMockState* mock = mock_for(decomp);
    if (splitFrame != mock->splitFrame) {
        return 0;
    }
    return mock->view2;
}

s16 cview_mock_get_split_line(void* splitFrame, int decomp) {
    CViewMockState* mock = mock_for(decomp);
    if (splitFrame != mock->splitFrame) {
        return 0;
    }
    return mock->splitLine;
}

void cview_mock_set_split_line(void* splitFrame, s16 line, int decomp) {
    CViewMockState* mock = mock_for(decomp);
    if (splitFrame != mock->splitFrame) {
        return;
    }
    mock->splitLine = line;
}

int cview_mock_is_active(void* splitFrame, int decomp) {
    CViewMockState* mock = mock_for(decomp);
    if (splitFrame != mock->splitFrame) {
        return 0;
    }
    return mock->splitActive;
}

void cview_mock_apply(void* splitFrame, int decomp) {
    (void)splitFrame;
    (void)decomp;
}

CView* cview_mock_get_current(int decomp) {
    return mock_for(decomp)->rootCurrent;
}

void* cview_mock_get_instance(int decomp) {
    return mock_for(decomp)->rootInstance;
}

void cview_mock_invalid_current(CView* view, int decomp) {
    CViewMockState* mock = mock_for(decomp);
    mock->invalidArg = view;
    mock->invalidCount++;
}

void cview_mock_wk_set_event(CWorkThread* thread, int evt, int decomp) {
    (void)thread;
    mock_for(decomp)->lastWkEvent = evt;
}
