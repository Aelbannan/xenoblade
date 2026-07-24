#include <revolution/BTE/gki/platform/gki_int.h>

#if (GKI_NUM_TOTAL_BUF_POOLS > 16)
#error Number of pools out of range (16 Max)!
#endif

/* Retail MAGIC_NO check: odd ptr => bad; else (exp-v)|(v-exp)>>31. Must stay a
 * macro so MWCC inlines into each caller (no separate symbol). */
#define gki_magic_corrupted(magic)                                             \
    (((UINT32)(magic) & 1)                                                     \
         ? 1u                                                                  \
         : ((((MAGIC_NO) - *(UINT32*)(magic)) |                                \
             (*(UINT32*)(magic) - (MAGIC_NO))) >>                              \
            31))

void gki_init_free_queue(UINT8 id, UINT16 size, UINT16 total, void* p_mem) {
    UINT16 i;
    UINT16 act_size;
    BUFFER_HDR_T* hdr;
    BUFFER_HDR_T* hdr1 = NULL;
    INT32 tempsize;
    tGKI_COM_CB* p_cb = &gki_cb.com;

    tempsize = (INT32)ALIGN_POOL(size);
    act_size = (UINT16)(tempsize + BUFFER_PADDING_SIZE);

    /* REVOLUTION: always store start/end. */
    p_cb->pool_start[id] = (UINT8*)p_mem;
    p_cb->pool_end[id] = (UINT8*)p_mem + (act_size * total);
    p_cb->pool_size[id] = act_size;
    p_cb->freeq[id].size = (UINT16)tempsize;
    p_cb->freeq[id].total = total;
    p_cb->freeq[id].cur_cnt = 0;
    p_cb->freeq[id].max_cnt = 0;

    hdr = (BUFFER_HDR_T*)p_mem;
    p_cb->freeq[id].p_first = hdr;

    if (total == 0) {
        goto finish;
    }

    /* Retail has an 8x CTR unroll when total>8; MWCC does not emit it from a
     * handwritten 8-body (size blows past 0x220). Keep a single loop. */
    for (i = 0; i < total; i++) {
        hdr->task_id = GKI_INVALID_TASK;
        hdr->q_id = id;
        hdr->status = BUF_STATUS_FREE;
        *(UINT32*)((UINT8*)hdr + tempsize + BUFFER_HDR_SIZE) = MAGIC_NO;
        hdr1 = hdr;
        hdr = (BUFFER_HDR_T*)((UINT8*)hdr + act_size);
        hdr1->p_next = hdr;
    }

finish:
    hdr1->p_next = NULL;
    p_cb->freeq[id].p_last = hdr1;
}

void gki_buffer_init(void) {
    UINT8 i;
    UINT8 tt;
    UINT8 mb;
    tGKI_COM_CB* p_cb = &gki_cb.com;

    for (tt = 0; tt < GKI_MAX_TASKS; tt++) {
        for (mb = 0; mb < NUM_TASK_MBOX; mb++) {
            p_cb->OSTaskQFirst[tt][mb] = NULL;
            p_cb->OSTaskQLast[tt][mb] = NULL;
        }
    }

    for (tt = 0; tt < GKI_NUM_TOTAL_BUF_POOLS; tt++) {
        p_cb->pool_start[tt] = NULL;
        p_cb->pool_end[tt] = NULL;
        p_cb->pool_size[tt] = 0;
        p_cb->freeq[tt].p_first = 0;
        p_cb->freeq[tt].p_last = 0;
        p_cb->freeq[tt].size = 0;
        p_cb->freeq[tt].total = 0;
        p_cb->freeq[tt].cur_cnt = 0;
        p_cb->freeq[tt].max_cnt = 0;
    }

    p_cb->pool_access_mask = GKI_DEF_BUFPOOL_PERM_MASK;

#if (GKI_NUM_FIXED_BUF_POOLS > 0)
    gki_init_free_queue(0, GKI_BUF0_SIZE, GKI_BUF0_MAX, p_cb->bufpool0);
#endif
#if (GKI_NUM_FIXED_BUF_POOLS > 1)
    gki_init_free_queue(1, GKI_BUF1_SIZE, GKI_BUF1_MAX, p_cb->bufpool1);
#endif
#if (GKI_NUM_FIXED_BUF_POOLS > 2)
    gki_init_free_queue(2, GKI_BUF2_SIZE, GKI_BUF2_MAX, p_cb->bufpool2);
#endif
#if (GKI_NUM_FIXED_BUF_POOLS > 3)
    gki_init_free_queue(3, GKI_BUF3_SIZE, GKI_BUF3_MAX, p_cb->bufpool3);
#endif
#if (GKI_NUM_FIXED_BUF_POOLS > 4)
    gki_init_free_queue(4, GKI_BUF4_SIZE, GKI_BUF4_MAX, p_cb->bufpool4);
#endif

    for (i = 0; i < GKI_NUM_FIXED_BUF_POOLS; i++) {
        p_cb->pool_list[i] = i;
    }

    p_cb->curr_total_no_of_pools = GKI_NUM_FIXED_BUF_POOLS;
    return;
}

void GKI_init_q(BUFFER_Q* p_q) {
    p_q->p_first = p_q->p_last = NULL;
    p_q->count = 0;
    return;
}

void* GKI_getbuf(UINT16 size) {
    UINT8 i;
    UINT8 pool;
    FREE_QUEUE_T* Q;
    BUFFER_HDR_T* p_hdr;
    tGKI_COM_CB* p_cb = &gki_cb.com;
    void* ret;

    if (size == 0) {
        GKI_exception(GKI_ERROR_BUF_SIZE_ZERO, "getbuf: Size is zero");
        return NULL;
    }

    /* Bottom-tested scan; join at cmplw i/total (retail bne-to-disable). */
    i = 0;
    goto getbuf_scan_test;
getbuf_scan:
    if (size <= p_cb->freeq[p_cb->pool_list[i]].size) {
        goto getbuf_scan_done;
    }
    i++;
getbuf_scan_test:
    if (i < p_cb->curr_total_no_of_pools) {
        goto getbuf_scan;
    }
getbuf_scan_done:
    if (i == p_cb->curr_total_no_of_pools) {
        GKI_exception(GKI_ERROR_BUF_SIZE_TOOBIG, "getbuf: Size is too big");
        return NULL;
    }

    GKI_disable();

    /* Second phase: access-mask + free slot only (no size re-check). */
    goto getbuf_take_test;
getbuf_take:
    pool = p_cb->pool_list[i];
    if (!(((UINT16)1 << pool) & p_cb->pool_access_mask)) {
        Q = &p_cb->freeq[pool];
        if (Q->cur_cnt < Q->total) {
            p_hdr = Q->p_first;
            Q->p_first = p_hdr->p_next;
            if (!Q->p_first) {
                Q->p_last = NULL;
            }
            if (++Q->cur_cnt > Q->max_cnt) {
                Q->max_cnt = Q->cur_cnt;
            }
            GKI_enable();
            p_hdr->task_id = GKI_get_taskid();
            ret = (void*)((UINT8*)p_hdr + BUFFER_HDR_SIZE);
            p_hdr->status = BUF_STATUS_UNLINKED;
            p_hdr->Type = 0;
            p_hdr->p_next = NULL;
            return ret;
        }
    }
    i++;
getbuf_take_test:
    if (i < p_cb->curr_total_no_of_pools) {
        goto getbuf_take;
    }

    GKI_enable();
    return NULL;
}
void* GKI_getpoolbuf(UINT8 pool_id) {
    FREE_QUEUE_T* Q;
    BUFFER_HDR_T* p_hdr;
    tGKI_COM_CB* p_cb = &gki_cb.com;

    if (pool_id >= GKI_NUM_TOTAL_BUF_POOLS) {
        return NULL;
    }

    GKI_disable();
    Q = &p_cb->freeq[pool_id];

    if (Q->cur_cnt < Q->total) {
        p_hdr = Q->p_first;
        Q->p_first = p_hdr->p_next;

        if (!Q->p_first) {
            Q->p_last = NULL;
        }

        if (++Q->cur_cnt > Q->max_cnt) {
            Q->max_cnt = Q->cur_cnt;
        }

        GKI_enable();
        p_hdr->task_id = GKI_get_taskid();
        p_hdr->status = BUF_STATUS_UNLINKED;
        p_hdr->p_next = NULL;
        p_hdr->Type = 0;
        return (void*)((UINT8*)p_hdr + BUFFER_HDR_SIZE);
    }

    GKI_enable();
    return GKI_getbuf(p_cb->freeq[pool_id].size);
}

void GKI_freebuf(void* p_buf) {
    FREE_QUEUE_T* Q;
    BUFFER_HDR_T* p_hdr;
    UINT32* magic;
    UINT16 buf_size;
    UINT32 v;
    UINT32 exp;
    UINT8 bad;
    /* Retail: string pool base in r5 only at prologue (no early gki_cb → no r30). */
    char* msg = (char*)"getbuf: Size is zero";

#if (GKI_ENABLE_BUF_CORRUPTION_CHECK == TRUE)
    if (!p_buf) {
        goto free_corrupted;
    }

    p_hdr = (BUFFER_HDR_T*)((UINT8*)p_buf - BUFFER_HDR_SIZE);
    /* Odd hdr → size 0; else q_id>=9 → 0; else load (retail bge-skip shape). */
    if ((UINT32)p_hdr & 1) {
        buf_size = 0;
    } else {
        if (p_hdr->q_id >= GKI_NUM_TOTAL_BUF_POOLS) {
            buf_size = 0;
        } else {
            buf_size = gki_cb.com.freeq[p_hdr->q_id].size;
        }
    }

    magic = (UINT32*)((UINT8*)p_buf + buf_size);
    if ((UINT32)magic & 1) {
        bad = 1;
    } else {
        /* Load MAGIC before *magic so MWCC keeps exp in a GPR for dual subf. */
        exp = MAGIC_NO;
        v = *magic;
        bad = (UINT8)(((exp - v) | (v - exp)) >> 31);
    }

    if (bad) {
    free_corrupted:
        GKI_exception(GKI_ERROR_BUF_CORRUPTED, msg + 0x30);
        return;
    }
#else
    p_hdr = (BUFFER_HDR_T*)((UINT8*)p_buf - BUFFER_HDR_SIZE);
#endif

    if (p_hdr->status != BUF_STATUS_UNLINKED) {
        GKI_exception(GKI_ERROR_FREEBUF_BUF_LINKED, msg + 0x48);
        return;
    }

    if (p_hdr->q_id >= GKI_NUM_TOTAL_BUF_POOLS) {
        GKI_exception(GKI_ERROR_FREEBUF_BAD_QID, msg + 0x5c);
        return;
    }

    GKI_disable();

    Q = &gki_cb.com.freeq[p_hdr->q_id];
    if (Q->p_last) {
        Q->p_last->p_next = p_hdr;
    } else {
        Q->p_first = p_hdr;
    }

    Q->p_last = p_hdr;
    p_hdr->p_next = NULL;
    p_hdr->status = BUF_STATUS_FREE;
    p_hdr->task_id = GKI_INVALID_TASK;

    if (Q->cur_cnt > 0) {
        Q->cur_cnt--;
    }

    GKI_enable();
}

UINT16 GKI_get_buf_size(void* p_buf) {
    BUFFER_HDR_T* p_hdr;

    p_hdr = (BUFFER_HDR_T*)((UINT8*)p_buf - BUFFER_HDR_SIZE);
    if ((UINT32)p_hdr & 1) {
        return 0;
    }

    if (p_hdr->q_id < GKI_NUM_TOTAL_BUF_POOLS) {
        return gki_cb.com.freeq[p_hdr->q_id].size;
    }

    return 0;
}

void GKI_send_msg(UINT8 task_id, UINT8 mbox, void* msg) {
    BUFFER_HDR_T* p_hdr;
    UINT32* magic;
    UINT16 buf_size;
    tGKI_COM_CB* p_cb = &gki_cb.com;

    if ((task_id >= GKI_MAX_TASKS) || (mbox >= NUM_TASK_MBOX) ||
        (p_cb->OSRdyTbl[task_id] == TASK_DEAD)) {
        GKI_exception(GKI_ERROR_SEND_MSG_BAD_DEST, "Sending to unknown dest");
        GKI_freebuf(msg);
        return;
    }

#if (GKI_ENABLE_BUF_CORRUPTION_CHECK == TRUE)
    p_hdr = (BUFFER_HDR_T*)((UINT8*)msg - BUFFER_HDR_SIZE);
    buf_size = 0;
    if (!((UINT32)p_hdr & 1)) {
        if (p_hdr->q_id < GKI_NUM_TOTAL_BUF_POOLS) {
            buf_size = p_cb->freeq[p_hdr->q_id].size;
        }
    }

    magic = (UINT32*)((UINT8*)msg + buf_size);
    if (gki_magic_corrupted(magic)) {
        GKI_exception(GKI_ERROR_BUF_CORRUPTED, "Send - Buffer corrupted");
        return;
    }
#else
    p_hdr = (BUFFER_HDR_T*)((UINT8*)msg - BUFFER_HDR_SIZE);
#endif

    if (p_hdr->status != BUF_STATUS_UNLINKED) {
        GKI_exception(GKI_ERROR_SEND_MSG_BUF_LINKED, "Send - buffer linked");
        return;
    }

    GKI_disable();

    if (p_cb->OSTaskQFirst[task_id][mbox]) {
        p_cb->OSTaskQLast[task_id][mbox]->p_next = p_hdr;
    } else {
        p_cb->OSTaskQFirst[task_id][mbox] = p_hdr;
    }

    p_cb->OSTaskQLast[task_id][mbox] = p_hdr;
    p_hdr->p_next = NULL;
    p_hdr->status = BUF_STATUS_QUEUED;
    p_hdr->task_id = task_id;

    GKI_enable();
    GKI_send_event(task_id, (UINT16)EVENT_MASK(mbox));
    return;
}

void* GKI_read_mbox(UINT8 mbox) {
    UINT8 task_id = GKI_get_taskid();
    void* p_buf = NULL;
    BUFFER_HDR_T* p_hdr;

    if ((task_id >= GKI_MAX_TASKS) || (mbox >= NUM_TASK_MBOX)) {
        return NULL;
    }

    GKI_disable();

    if (gki_cb.com.OSTaskQFirst[task_id][mbox]) {
        p_hdr = gki_cb.com.OSTaskQFirst[task_id][mbox];
        gki_cb.com.OSTaskQFirst[task_id][mbox] = p_hdr->p_next;
        p_hdr->p_next = NULL;
        p_hdr->status = BUF_STATUS_UNLINKED;
        p_buf = (UINT8*)p_hdr + BUFFER_HDR_SIZE;
    }

    GKI_enable();
    return p_buf;
}

void GKI_enqueue(BUFFER_Q* p_q, void* p_buf) {
    BUFFER_HDR_T* p_hdr;
    UINT32* magic;
    UINT16 buf_size;

#if (GKI_ENABLE_BUF_CORRUPTION_CHECK == TRUE)
    p_hdr = (BUFFER_HDR_T*)((UINT8*)p_buf - BUFFER_HDR_SIZE);
    buf_size = 0;
    if (!((UINT32)p_hdr & 1)) {
        if (p_hdr->q_id < GKI_NUM_TOTAL_BUF_POOLS) {
            buf_size = gki_cb.com.freeq[p_hdr->q_id].size;
        }
    }

    magic = (UINT32*)((UINT8*)p_buf + buf_size);
    if (gki_magic_corrupted(magic)) {
        GKI_exception(GKI_ERROR_BUF_CORRUPTED, "Enqueue - Buffer corrupted");
        return;
    }
#else
    p_hdr = (BUFFER_HDR_T*)((UINT8*)p_buf - BUFFER_HDR_SIZE);
#endif

    if (p_hdr->status != BUF_STATUS_UNLINKED) {
        GKI_exception(GKI_ERROR_ENQUEUE_BUF_LINKED, "Eneueue - buf already linked");
        return;
    }

    GKI_disable();

    if (p_q->p_last) {
        BUFFER_HDR_T* p_last_hdr = (BUFFER_HDR_T*)((UINT8*)p_q->p_last - BUFFER_HDR_SIZE);
        p_last_hdr->p_next = p_hdr;
    } else {
        p_q->p_first = p_buf;
    }

    p_q->p_last = p_buf;
    p_q->count++;
    p_hdr->p_next = NULL;
    p_hdr->status = BUF_STATUS_QUEUED;

    GKI_enable();
    return;
}

void GKI_enqueue_head(BUFFER_Q* p_q, void* p_buf) {
    BUFFER_HDR_T* p_hdr;
    UINT32* magic;
    UINT16 buf_size;

#if (GKI_ENABLE_BUF_CORRUPTION_CHECK == TRUE)
    p_hdr = (BUFFER_HDR_T*)((UINT8*)p_buf - BUFFER_HDR_SIZE);
    buf_size = 0;
    if (!((UINT32)p_hdr & 1)) {
        if (p_hdr->q_id < GKI_NUM_TOTAL_BUF_POOLS) {
            buf_size = gki_cb.com.freeq[p_hdr->q_id].size;
        }
    }

    magic = (UINT32*)((UINT8*)p_buf + buf_size);
    if (gki_magic_corrupted(magic)) {
        GKI_exception(GKI_ERROR_BUF_CORRUPTED, "Enqueue - Buffer corrupted");
        return;
    }
#else
    p_hdr = (BUFFER_HDR_T*)((UINT8*)p_buf - BUFFER_HDR_SIZE);
#endif

    if (p_hdr->status != BUF_STATUS_UNLINKED) {
        GKI_exception(GKI_ERROR_ENQUEUE_BUF_LINKED, "Eneueue head - buf already linked");
        return;
    }

    GKI_disable();

    if (p_q->p_first) {
        p_hdr->p_next = (BUFFER_HDR_T*)((UINT8*)p_q->p_first - BUFFER_HDR_SIZE);
        p_q->p_first = p_buf;
    } else {
        p_q->p_first = p_buf;
        p_q->p_last = p_buf;
        p_hdr->p_next = NULL;
    }

    p_q->count++;
    p_hdr->status = BUF_STATUS_QUEUED;

    GKI_enable();
    return;
}

void* GKI_dequeue(BUFFER_Q* p_q) {
    BUFFER_HDR_T* p_hdr;

    GKI_disable();

    if (!p_q || !p_q->count) {
        GKI_enable();
        return NULL;
    }

    p_hdr = (BUFFER_HDR_T*)((UINT8*)p_q->p_first - BUFFER_HDR_SIZE);

    if (p_hdr->p_next) {
        p_q->p_first = ((UINT8*)p_hdr->p_next + BUFFER_HDR_SIZE);
    } else {
        p_q->p_first = NULL;
        p_q->p_last = NULL;
    }

    p_q->count--;
    p_hdr->p_next = NULL;
    p_hdr->status = BUF_STATUS_UNLINKED;

    GKI_enable();
    return (UINT8*)p_hdr + BUFFER_HDR_SIZE;
}

void* GKI_remove_from_queue(BUFFER_Q* p_q, void* p_buf) {
    BUFFER_HDR_T* p_prev;
    BUFFER_HDR_T* p_buf_hdr;

    GKI_disable();

    if (p_buf == p_q->p_first) {
        GKI_enable();
        return GKI_dequeue(p_q);
    }

    p_buf_hdr = (BUFFER_HDR_T*)((UINT8*)p_buf - BUFFER_HDR_SIZE);
    p_prev = (BUFFER_HDR_T*)((UINT8*)p_q->p_first - BUFFER_HDR_SIZE);

    for (; p_prev; p_prev = p_prev->p_next) {
        if (p_prev->p_next == p_buf_hdr) {
            p_prev->p_next = p_buf_hdr->p_next;

            if (p_buf == p_q->p_last) {
                p_q->p_last = p_prev + 1;
            }

            p_q->count--;
            p_buf_hdr->p_next = NULL;
            p_buf_hdr->status = BUF_STATUS_UNLINKED;

            GKI_enable();
            return p_buf;
        }
    }

    GKI_enable();
    return NULL;
}

void* GKI_getfirst(BUFFER_Q* p_q) {
    return p_q->p_first;
}

void* GKI_getnext(void* p_buf) {
    BUFFER_HDR_T* p_hdr;

    p_hdr = (BUFFER_HDR_T*)((UINT8*)p_buf - BUFFER_HDR_SIZE);
    if (p_hdr->p_next) {
        return (UINT8*)p_hdr->p_next + BUFFER_HDR_SIZE;
    }

    return NULL;
}

BOOLEAN GKI_queue_is_empty(BUFFER_Q* p_q) {
    return (BOOLEAN)(p_q->count == 0);
}

UINT8 GKI_create_pool(UINT16 size, UINT16 count, UINT8 permission, void* p_mem_pool) {
    UINT8 xx;
    UINT8 groups;
    INT32 i;
    INT32 j;
    INT32 tempsize;
    tGKI_COM_CB* p_cb = &gki_cb.com;

    if (size > MAX_USER_BUF_SIZE) {
        return GKI_INVALID_POOL;
    }

    /* Retail: 3 bodies per CTR trip over 9 total pools. */
    xx = 0;
    groups = 3;
    do {
        if (!p_cb->pool_start[xx]) {
            break;
        }
        xx++;
        if (!p_cb->pool_start[xx]) {
            break;
        }
        xx++;
        if (!p_cb->pool_start[xx]) {
            break;
        }
        xx++;
    } while (--groups != 0);

    if (xx == GKI_NUM_TOTAL_BUF_POOLS) {
        return GKI_INVALID_POOL;
    }

    tempsize = (INT32)ALIGN_POOL(size);

    if (!p_mem_pool) {
        p_mem_pool = GKI_os_malloc((tempsize + BUFFER_PADDING_SIZE) * count);
    }

    if (!p_mem_pool) {
        return GKI_INVALID_POOL;
    }

    gki_init_free_queue(xx, size, count, p_mem_pool);

    for (i = 0; i < p_cb->curr_total_no_of_pools; i++) {
        if (p_cb->freeq[xx].size <= p_cb->freeq[p_cb->pool_list[i]].size) {
            break;
        }
    }

    /* Scalar shift — handwritten 8× CTR body regresses (~48%). */
    for (j = p_cb->curr_total_no_of_pools; j > i; j--) {
        p_cb->pool_list[j] = p_cb->pool_list[j - 1];
    }

    p_cb->pool_list[i] = xx;

    if (permission == GKI_RESTRICTED_POOL) {
        p_cb->pool_access_mask = (UINT16)(p_cb->pool_access_mask | (1 << xx));
    } else {
        p_cb->pool_access_mask = (UINT16)(p_cb->pool_access_mask & ~(1 << xx));
    }

    p_cb->curr_total_no_of_pools++;
    return xx;
}

void GKI_delete_pool(UINT8 pool_id) {
    FREE_QUEUE_T* Q;
    tGKI_COM_CB* p_cb = &gki_cb.com;
    UINT8 i;

    if ((pool_id >= GKI_NUM_TOTAL_BUF_POOLS) || (!p_cb->pool_start[pool_id])) {
        return;
    }

    GKI_disable();
    Q = &p_cb->freeq[pool_id];

    if (!Q->cur_cnt) {
        Q->size = 0;
        Q->total = 0;
        Q->cur_cnt = 0;
        Q->max_cnt = 0;
        Q->p_first = NULL;
        Q->p_last = NULL;

        GKI_os_free(p_cb->pool_start[pool_id]);

        p_cb->pool_start[pool_id] = NULL;
        p_cb->pool_end[pool_id] = NULL;
        p_cb->pool_size[pool_id] = 0;

        for (i = 0; i < p_cb->curr_total_no_of_pools; i++) {
            if (pool_id == p_cb->pool_list[i]) {
                break;
            }
        }

        while (i < (p_cb->curr_total_no_of_pools - 1)) {
            p_cb->pool_list[i] = p_cb->pool_list[i + 1];
            i++;
        }

        p_cb->curr_total_no_of_pools--;
    } else {
        GKI_exception(GKI_ERROR_DELETE_POOL_BAD_QID, "Deleting bad pool");
    }

    GKI_enable();
    return;
}
