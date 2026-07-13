#ifndef CVIEWROOT_MOCK_H
#define CVIEWROOT_MOCK_H

#include <types.h>

#define CVIEWROOT_THREAD_CVIEW 48
#define CVIEWROOT_THREAD_CVIEW_MAX 53
#define CVIEWROOT_INVALID_WORK_ID 0xFFFFFFFFu

typedef struct CViewRootMockThread {
    u8 pad[0x50];
    s32 mType;
} CViewRootMockThread;

typedef struct CViewRootMockNode {
    struct CViewRootMockNode* mNext;
    struct CViewRootMockNode* mPrev;
    CViewRootMockThread* mItem;
} CViewRootMockNode;

typedef struct CViewRootMockRoot {
    u8 pad[0x5C];
    CViewRootMockNode* mStartNodePtr;
    CViewRootMockNode sentinel;
    CViewRootMockNode nodes[4];
} CViewRootMockRoot;

typedef struct CViewRootMockState {
    CViewRootMockRoot* singleton;
    CViewRootMockThread* workTable[8];
} CViewRootMockState;

extern CViewRootMockThread g_cviewroot_threads[8];
extern CViewRootMockState g_rb_cviewroot_mock;
extern CViewRootMockState g_dc_cviewroot_mock;

void cviewroot_mock_reset(void);
void cviewroot_mock_set_children(int decomp, int count);
void cviewroot_mock_bind_work(u32 id, int decomp, int tableIndex, int threadType);
CViewRootMockThread* cviewroot_mock_get_work_thread(u32 id, int decomp);

#endif
