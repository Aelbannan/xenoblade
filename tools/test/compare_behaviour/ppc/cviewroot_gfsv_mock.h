#ifndef CVIEWROOT_GFSV_MOCK_H
#define CVIEWROOT_GFSV_MOCK_H

#include <types.h>

#define GFSV_THREAD_FLAG_NO_EVENT 1u
#define GFSV_THREAD_FLAG_EXCEPTION 0x10u
#define GFSV_THREAD_STATE_LOGIN 2u
#define GFSV_THREAD_STATE_RUN 3u
#define GFSV_EVT_EXCEPTION 2u
#define GFSV_MAX_CHILDREN 4

typedef struct CViewRootGfsvChild {
    u32 mFlags;
    u32 mState;
    u32 unk278;
    s16 frameOffX;
    s16 frameOffY;
    s16 posX;
    s16 posY;
    s16 viewW;
    s16 viewH;
    u32 msgs[4];
    int msgCount;
    int isView;
} CViewRootGfsvChild;

typedef struct CViewRootGfsvState {
    int hasRoot;
    int hasDesktop;
    s16 fbWidth;
    s16 efbHeight;
    int childCount;
    CViewRootGfsvChild children[GFSV_MAX_CHILDREN];
} CViewRootGfsvState;

extern CViewRootGfsvState g_rb_gfsv_mock;
extern CViewRootGfsvState g_dc_gfsv_mock;

void cviewroot_gfsv_mock_reset(void);
CViewRootGfsvChild* cviewroot_gfsv_add_child(int decomp);
void* cviewroot_gfsv_select(int decomp);

#endif
