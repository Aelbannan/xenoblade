#include <string.h>

#include "cviewroot_mock.h"

CViewRootMockThread g_cviewroot_threads[8];
CViewRootMockState g_rb_cviewroot_mock;
CViewRootMockState g_dc_cviewroot_mock;

void cviewroot_mock_reset(void) {
    memset(&g_rb_cviewroot_mock, 0, sizeof(g_rb_cviewroot_mock));
    memset(&g_dc_cviewroot_mock, 0, sizeof(g_dc_cviewroot_mock));
    memset(g_cviewroot_threads, 0, sizeof(g_cviewroot_threads));
}

void cviewroot_mock_set_children(int decomp, int count) {
    CViewRootMockState* mock = decomp ? &g_dc_cviewroot_mock : &g_rb_cviewroot_mock;
    CViewRootMockRoot* root = mock->singleton;
    int i;

    if (root == 0) {
        return;
    }

    if (count == 0) {
        root->sentinel.mNext = &root->sentinel;
        root->sentinel.mPrev = &root->sentinel;
        return;
    }

    for (i = 0; i < count; ++i) {
        root->nodes[i].mItem = 0;
        root->nodes[i].mNext = (i + 1 < count) ? &root->nodes[i + 1] : &root->sentinel;
        root->nodes[i].mPrev = (i == 0) ? &root->sentinel : &root->nodes[i - 1];
    }
    root->sentinel.mNext = &root->nodes[0];
    root->sentinel.mPrev = &root->nodes[count - 1];
}

void cviewroot_mock_bind_work(u32 id, int decomp, int tableIndex, int threadType) {
    CViewRootMockState* mock = decomp ? &g_dc_cviewroot_mock : &g_rb_cviewroot_mock;

    if (id >= 8u || tableIndex < 0 || tableIndex >= 8) {
        return;
    }

    g_cviewroot_threads[tableIndex].mType = threadType;
    mock->workTable[id] = &g_cviewroot_threads[tableIndex];
}

CViewRootMockThread* cviewroot_mock_get_work_thread(u32 id, int decomp) {
    CViewRootMockState* mock = decomp ? &g_dc_cviewroot_mock : &g_rb_cviewroot_mock;

    if (id == CVIEWROOT_INVALID_WORK_ID) {
        return 0;
    }
    if (id >= 8u) {
        return 0;
    }
    return mock->workTable[id];
}
