#ifndef CVIEW_MOCK_H
#define CVIEW_MOCK_H

#include <types.h>

typedef struct CView CView;
typedef struct CWorkThread CWorkThread;

#define CVIEW_OFF_UNK278 0x278u
#define CVIEW_OFF_UNK45C 0x45Cu

typedef struct CViewMockState {
    void* splitFrame;
    CView* view1;
    CView* view2;
    s16 splitLine;
    int splitActive;
    CView* rootCurrent;
    void* rootInstance;
    CView* invalidArg;
    int invalidCount;
    int lastWkEvent;
} CViewMockState;

extern CViewMockState g_rb_cview_mock;
extern CViewMockState g_dc_cview_mock;

void cview_mock_reset(void);
u32 cview_get_unk278(CView* view);
void cview_set_unk278(CView* view, u32 value);
void cview_set_unk45c(CView* view, void* splitFrame);

CView* cview_mock_get_view1(void* splitFrame, int decomp);
CView* cview_mock_get_view2(void* splitFrame, int decomp);
s16 cview_mock_get_split_line(void* splitFrame, int decomp);
void cview_mock_set_split_line(void* splitFrame, s16 line, int decomp);
int cview_mock_is_active(void* splitFrame, int decomp);
void cview_mock_apply(void* splitFrame, int decomp);
CView* cview_mock_get_current(int decomp);
void* cview_mock_get_instance(int decomp);
void cview_mock_invalid_current(CView* view, int decomp);
void cview_mock_wk_set_event(CWorkThread* thread, int evt, int decomp);

#endif
