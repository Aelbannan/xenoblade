// sfd_buf.c - CriWare Sofdec buffer management unit
// Manages ring buffers, video frame buffers, and audio ring buffers
// for the Sofdec video playback system.

#include <harness_catalog.h>

/* External function declarations */
extern void *SJRBF_Create(void *pool_mem, u32 buf_size, u32 xtr_size);
extern void *SJMEM_Create(void *pool_mem, u32 flags);
extern int SJRBF_GetFlowCnt(void *self, int mode, int reset);
extern int SJMEM_GetBufSize(void *self);
extern u32 SFTRN_IsSetup(void *self, u32 idx);
extern void SFPTS_InitPtsQue(void *self);
extern void SFPTS_ResetPtsQue(void *self);
extern s32 SFLIB_SetErr(void *h, u32 err_code);
extern void SFLIB_LockCs(void *cs);
extern void SFLIB_UnlockCs(void *cs);
extern s32 SFTRN_CallTrtTrif(void *self, int idx, int funcIdx, int *arg4, int arg5);

/* Global buffer object pointers (set by SFBUF_Init) */
extern void *lbl_eu_80606E10; /* ring buffer object */
extern void *lbl_eu_80606E14; /* memory buffer object */

/* Forward declarations for internal functions */
static void sfbuf_InitRingSj(void *self, u32 *cumulative, u32 *sizes,
                              int idx, u32 extraSize);
void sfbuf_InitVfrmBuf(void *self, void *handle, u32 *cumulative,
                        u32 *sizes, int idx);
void sfbuf_InitAringBuf(void *self, u32 *cumulative, u32 *sizes, int idx);
void sfbuf_RingGetSub(void *self, int idx, u32 *out, int mode);
void sfbuf_RingAddSub(void *self, int idx, u32 size, int mode);

/* Buffer constants */
#define SFBUF_MAX_IDX   8
#define SFBUF_BUF_STRIDE 0x74
#define SFBUF_STATE_ACTIVE 5
#define SFBUF_STATE_READY  4
#define SFBUF_ERR_BASE     0xFF000400

/* Per-buffer field offsets (relative to buffer base + idx*0x74) */
#define OFF_STATE       0x00
#define OFF_ACTIVE      0x04
#define OFF_BUFOBJ      0x1C
#define OFF_BUFSIZ      0x18
#define OFF_WTOT        0x20
#define OFF_RTOT        0x24
#define OFF_DLM_PTR     0x28
#define OFF_DLM_SIZE    0x2C

/* Helper: get per-buffer base pointer */
static inline u8 *sfbuf_base(void *self, int idx) {
    return (u8 *)self + idx * SFBUF_BUF_STRIDE;
}

/*
 * SFBUF_Init - Initialize global buffer allocators
 *
 * Creates temporary SJRBF and SJMEM objects to obtain their
 * internal buffer handles, stores them in globals, then destroys
 * the temporary objects.
 */
void SFBUF_Init(void) {
    void *pool[2];
    void *obj;
    void *vtable;

    /* Create ring buffer and get its handle */
    obj = SJRBF_Create(pool, 8, 0);
    vtable = *(void **)obj;
    lbl_eu_80606E10 = ((void *(*)(void *))((void **)vtable)[4])(obj);
    vtable = *(void **)obj;
    ((void (*)(void *))((void **)vtable)[3])(obj);

    /* Create memory buffer and get its handle */
    obj = SJMEM_Create(pool, 8);
    vtable = *(void **)obj;
    lbl_eu_80606E14 = ((void *(*)(void *))((void **)vtable)[4])(obj);
    vtable = *(void **)obj;
    ((void (*)(void *))((void **)vtable)[3])(obj);
}

/*
 * sfbuf_InitRingSj - Initialize a single ring buffer slot
 *
 * Sets up the ring buffer at the given index with the specified
 * buffer size, extra size, and PTS queue initialization.
 */
static void sfbuf_InitRingSj(void *self, u32 *cumulative, u32 *sizes,
                              int idx, u32 extraSize) {
    u8 *p = sfbuf_base(self, idx);
    u32 bufSize = cumulative[idx];
    u32 allocSize;
    void *ringObj;
    int active;
    int state;

    if (bufSize == 0) {
        /* Buffer not allocated - mark inactive */
        active = 0;
        state = 4; /* not active */
    } else {
        u32 actualSize = bufSize - extraSize;
        void *memObj = (void *)cumulative[idx]; /* buffer memory pointer */

        active = 1;
        state = 5; /* active */

        /* Allocate ring buffer */
        if (actualSize <= 0) {
            SFLIB_SetErr(NULL, SFBUF_ERR_BASE + 0x0C);
            allocSize = 0;
        } else {
            ringObj = SJRBF_Create(memObj, actualSize, extraSize);
            if (ringObj == NULL) {
                SFLIB_SetErr(NULL, SFBUF_ERR_BASE + 0x0A);
                allocSize = 0;
            } else {
                allocSize = (u32)ringObj;
            }
        }

        if (allocSize != 0) {
            /* Lock and initialize buffer fields */
            u32 cs;
            SFLIB_LockCs(&cs);

            *(u32 *)(p + OFF_BUFOBJ) = allocSize;
            *(u32 *)(p + OFF_BUFSIZ) = actualSize;
            *(u32 *)(p + OFF_WTOT) = 0;
            *(u32 *)(p + OFF_RTOT) = 0;
            *(u32 *)(p + OFF_DLM_PTR) = 0;
            *(u32 *)(p + OFF_DLM_SIZE) = 0;

            SFLIB_UnlockCs(&cs);

            /* Initialize PTS queue */
            SFPTS_InitPtsQue(p + 0x38);
        }
    }

    /* Set buffer metadata */
    *(u32 *)(p + OFF_STATE) = state;
    *(u32 *)(p + OFF_ACTIVE) = active;
    *(u32 *)(p + 0x08) = 0;
    *(u32 *)(p + 0x0C) = 0;
    *(u32 *)(p + 0x4C) = 9;
    *(u32 *)(p + 0x50) = 9;
}

/*
 * SFBUF_InitHn - Initialize buffer handle with all ring buffers
 *
 * Sets up the complete buffer system: initializes ring buffers,
 * configures video frame buffers and audio ring buffers, and
 * sets initial state.
 *
 * @param self   Buffer handle pointer
 * @param idx    Buffer group index
 * @param config Configuration array with sizes and metadata
 */
void SFBUF_InitHn(void *self, int idx, u32 *config) {
    u32 cumulative[8];
    u32 bufSize = config[0];
    u32 divisor = config[11]; /* config[0x2C/4] */
    u32 extraSize;
    int i;

    /* Build cumulative offset array from config sizes */
    cumulative[0] = bufSize;
    for (i = 1; i < 7; i++) {
        cumulative[i] = cumulative[i - 1] + config[i];
    }

    /* Compute extra size as remainder of bufSize / divisor */
    extraSize = bufSize - (bufSize / divisor) * divisor;

    /* Initialize ring buffer (index 0) */
    sfbuf_InitRingSj(self, cumulative, config, 0, extraSize);
    if (*(u32 *)sfbuf_base(self, 0) != 0) return;

    /* Initialize video frame buffer 0 (index 1) */
    sfbuf_InitRingSj(self, cumulative, config, 1, 0x800);
    if (*(u32 *)sfbuf_base(self, 1) != 0) return;

    /* Initialize audio ring buffer 0 (index 2) */
    sfbuf_InitRingSj(self, cumulative, config, 2, 0);
    if (*(u32 *)sfbuf_base(self, 2) != 0) return;

    /* Initialize video frame buffer 1 (index 3) */
    sfbuf_InitVfrmBuf(self, self, cumulative, config, 3);

    /* Initialize audio ring buffer 1 (index 4) */
    sfbuf_InitAringBuf(self, cumulative, config, 4);

    /* Initialize video frame buffer 2 (index 5) */
    sfbuf_InitVfrmBuf(self, self, cumulative, config, 5);

    /* Initialize audio ring buffer 2 (index 6) */
    sfbuf_InitAringBuf(self, cumulative, config, 6);

    /* Set initial handle state */
    *(u32 *)((u8 *)self + 0x32C) = 3;
    *(u32 *)((u8 *)self + 0x330) = 1;
    *(u32 *)((u8 *)self + 0x334) = 0;
    *(u32 *)((u8 *)self + 0x338) = 0;
    *(u32 *)((u8 *)self + 0x378) = 9;
    *(u32 *)((u8 *)self + 0x37C) = 9;
    *(u32 *)((u8 *)self + 0x33C) = 0;
    *(u32 *)((u8 *)self + 0x340) = 0;
    *(u32 *)((u8 *)self + 0x344) = 0;
    *(u32 *)((u8 *)self + 0x348) = 0;
    *(u32 *)((u8 *)self + 0x34C) = 0;
    *(u32 *)((u8 *)self + 0x350) = 0;
    *(u32 *)((u8 *)self + 0x354) = 0;
    *(u32 *)((u8 *)self + 0x358) = 0;
    *(u32 *)((u8 *)self + 0x35C) = 0;
    *(u32 *)((u8 *)self + 0x360) = 0;
    *(u32 *)((u8 *)self + 0x364) = 0;
    *(u32 *)((u8 *)self + 0x368) = 0;
}

/*
 * SFBUF_DestroySj - Destroy all active SJ buffer objects
 *
 * Iterates through the three primary buffer types (ring, vfrm, aring)
 * and destroys their associated SJ objects if they are in the active state.
 */
void SFBUF_DestroySj(void *self) {
    u8 *base = (u8 *)self;
    u8 *supply = base + 0x13C8;
    void *obj;

    /* Buffer 0: ring buffer */
    if (*(u32 *)(base + 0x13B8) == SFBUF_STATE_ACTIVE) {
        obj = *(void **)(supply + 0x04);
        if (obj != NULL) {
            void *vtable = *(void **)obj;
            ((void (*)(void *))((void **)vtable)[3])(obj);
            *(u32 *)(supply + 0x04) = 0;
        }
    }

    /* Buffer 1: vfrm buffer */
    supply = base + 0x143C;
    if (*(u32 *)(base + 0x142C) == SFBUF_STATE_ACTIVE) {
        obj = *(void **)(supply + 0x04);
        if (obj != NULL) {
            void *vtable = *(void **)obj;
            ((void (*)(void *))((void **)vtable)[3])(obj);
            *(u32 *)(supply + 0x04) = 0;
        }
    }

    /* Buffer 2: aring buffer */
    supply = base + 0x14B0;
    if (*(u32 *)(base + 0x14A0) == SFBUF_STATE_ACTIVE) {
        obj = *(void **)(supply + 0x04);
        if (obj != NULL) {
            void *vtable = *(void **)obj;
            ((void (*)(void *))((void **)vtable)[3])(obj);
            *(u32 *)(supply + 0x04) = 0;
        }
    }
}

/*
 * sfbuf_InitVfrmBuf - Initialize video frame buffer
 *
 * Sets up a video frame buffer at the given index with the specified
 * sizes and pointers. Initializes the VFRM frame queue structure.
 */
void sfbuf_InitVfrmBuf(void *self, void *handle, u32 *cumulative,
                        u32 *sizes, int idx) {
    u8 *p = (u8 *)self + idx * SFBUF_BUF_STRIDE;
    u32 ptrVal = sizes[idx];
    u32 valid = (u32)(-(s32)ptrVal | ptrVal) >> 31; /* non-zero check */
    u8 *vfrm = (u8 *)handle + 0x1758;
    int i;

    *(u32 *)(p + 0x00) = 1; /* VFRM type */
    *(u32 *)(p + 0x04) = valid;
    *(u32 *)(p + 0x08) = 0;
    *(u32 *)(p + 0x0C) = 0;
    *(u32 *)(p + 0x4C) = 9;
    *(u32 *)(p + 0x50) = 9;
    *(u32 *)(p + 0x10) = cumulative[idx];
    *(u32 *)(p + 0x14) = sizes[idx];
    *(u32 *)(p + 0x18) = 0;
    *(u32 *)(p + 0x1C) = 0;

    /* Set up VFRM frame queue at offset 0x20 */
    *(u32 *)(p + 0x20) = (u32)vfrm;
    *(u32 *)vfrm = 0;

    /* Initialize 16 frame slots in the VFRM queue (stride 0x88) */
    for (i = 0; i < 16; i++) {
        *(u32 *)(*(u32 *)(p + 0x20) + i * 0x88) = 0;
    }
}

/*
 * sfbuf_InitAringBuf - Initialize audio ring buffer
 *
 * Sets up an audio ring buffer at the given index with the specified
 * sizes and pointers. Clears all ring control fields.
 */
void sfbuf_InitAringBuf(void *self, u32 *cumulative, u32 *sizes, int idx) {
    u8 *p = (u8 *)self + idx * SFBUF_BUF_STRIDE;
    u32 ptrVal = sizes[idx];
    u32 valid = (u32)(-(s32)ptrVal | ptrVal) >> 31; /* non-zero check */

    *(u32 *)(p + 0x00) = 2; /* ARING type */
    *(u32 *)(p + 0x04) = valid;
    *(u32 *)(p + 0x08) = 0;
    *(u32 *)(p + 0x0C) = 0;
    *(u32 *)(p + 0x4C) = 9;
    *(u32 *)(p + 0x50) = 9;
    *(u32 *)(p + 0x10) = cumulative[idx];
    *(u32 *)(p + 0x14) = sizes[idx];
    *(u32 *)(p + 0x18) = 0;
    *(u32 *)(p + 0x1C) = 0;
    *(u32 *)(p + 0x20) = 0;
    *(u32 *)(p + 0x24) = 0;
    *(u32 *)(p + 0x28) = 0;
    *(u32 *)(p + 0x2C) = 0;
    *(u32 *)(p + 0x30) = 0;
    *(u32 *)(p + 0x34) = 0;
    *(u32 *)(p + 0x38) = 0;
    *(u32 *)(p + 0x3C) = 0;
}

/*
 * SFBUF_SetSupplySj - Set supply buffer configuration
 *
 * Validates the supply configuration and copies it into the
 * appropriate ring buffer's supply section. Also resets the PTS queue.
 *
 * @param self    Buffer handle
 * @param idx     Transport index (1=video, 2=audio, 3=subtitle)
 * @param supply  Supply configuration (ptr, size, readLen, writeLen, ...)
 * @return 0 on success, -1 on error
 */
s32 SFBUF_SetSupplySj(void *self, int idx, u32 *supply) {
    u32 bufBase;
    int bufIdx;
    u32 cs;

    /* Validate supply configuration */
    if (supply[1] == 0) {
        return -1;
    }
    if (supply[0] != 0) {
        /* Check additional fields */
        if (supply[2] == 0) {
            return -1;
        }
        if ((s32)supply[3] <= 0) {
            return -1;
        }
        if ((s32)supply[5] > 0) {
            return -1;
        }
    }

    /* Determine buffer index from transport type */
    if (SFTRN_IsSetup(self, 1)) {
        bufIdx = 0;
    } else if (SFTRN_IsSetup(self, 2)) {
        bufIdx = 1;
    } else if (SFTRN_IsSetup(self, 3)) {
        bufIdx = 2;
    } else {
        bufIdx = 0;
    }

    bufBase = bufIdx * SFBUF_BUF_STRIDE;

    /* Check that buffer is in ready state */
    if (*(u32 *)((u8 *)self + bufBase + 0x13B8) != SFBUF_STATE_READY) {
        SFLIB_SetErr(self, SFBUF_ERR_BASE + 0x09);
        return -1;
    }

    /* Compute non-zero flag for supply pointer */
    {
        u32 ptr = supply[1];
        u32 flag = (u32)(-(s32)ptr | ptr) >> 31;
        u8 *dst = (u8 *)self + bufBase + 0x13C8;

        /* Lock critical section */
        SFLIB_LockCs(&cs);

        /* Store active flag */
        *(u32 *)((u8 *)self + bufBase + 0x13BC) = flag;

        /* Copy supply fields (ptr, size, readLen, writeLen) */
        *(u32 *)(dst + 0x00) = supply[0];
        *(u32 *)(dst + 0x04) = supply[1];
        *(u32 *)(dst + 0x08) = supply[2];
        *(u32 *)(dst + 0x0C) = supply[3];
        *(u32 *)(dst + 0x10) = supply[4];
        *(u32 *)(dst + 0x14) = supply[5];

        /* Clear counters and state */
        *(u32 *)(dst + 0x18) = 0;
        *(u32 *)(dst + 0x1C) = 0;
        *(u32 *)(dst + 0x20) = 0;
        *(u32 *)(dst + 0x24) = 0;

        SFLIB_UnlockCs(&cs);

        /* Reset PTS queue */
        SFPTS_ResetPtsQue(dst + 0x28);
    }

    return 0;
}

/*
 * fn_803C1CAC - Check transport setup status and supply availability
 *
 * Checks if any transport (video/audio/subtitle) is set up and has
 * a valid supply buffer configured.
 *
 * @param self  Buffer handle
 * @return 1 if supply available, 0 otherwise
 */
int fn_803C1CAC(void *self) {
    int bufIdx;
    u32 *fields;

    /* Determine buffer index from transport type */
    if (SFTRN_IsSetup(self, 1)) {
        bufIdx = 0;
    } else if (SFTRN_IsSetup(self, 2)) {
        bufIdx = 1;
    } else if (SFTRN_IsSetup(self, 3)) {
        bufIdx = 2;
    } else {
        bufIdx = 0;
    }

    /* Check if buffer has supply configured */
    fields = (u32 *)((u8 *)self + bufIdx * 0x74 + 0x13B8);

    if (fields[1] == 0 || fields[5] == 0) { /* active flag or ring obj */
        return 0;
    }
    return 1;
}

/*
 * SFBUF_SetUoch - Set UOCH (user operation channel) data
 *
 * Copies a 4-field UOCH structure into the buffer's UOCH slot.
 *
 * @param self    Buffer handle
 * @param idx     Buffer index
 * @param sub_idx Sub-index (UOCH slot)
 * @param src     Source UOCH data (4 x u32)
 */
void SFBUF_SetUoch(void *self, int idx, int sub_idx, u32 *src) {
    u32 s0 = src[0];
    u32 s1 = src[1];
    u8 *dst = (u8 *)self + idx * SFBUF_BUF_STRIDE + (sub_idx << 4) + 0x13C8;
    *(u32 *)(dst + 0) = s0;
    *(u32 *)(dst + 4) = s1;
    s0 = src[2];
    s1 = src[3];
    *(u32 *)(dst + 8) = s0;
    *(u32 *)(dst + 12) = s1;
}

/*
 * SFBUF_GetUoch - Get UOCH (user operation channel) data
 *
 * Reads a 4-field UOCH structure from the buffer's UOCH slot.
 *
 * @param self    Buffer handle
 * @param idx     Buffer index
 * @param sub_idx Sub-index (UOCH slot)
 * @param dst     Destination buffer (4 x u32)
 */
void SFBUF_GetUoch(void *self, int idx, int sub_idx, u32 *dst) {
    u32 *src = (u32 *)((u8 *)self + idx * SFBUF_BUF_STRIDE +
                        (sub_idx << 4) + 0x13C8);
    u32 s0 = src[0];
    u32 s1 = src[1];
    dst[1] = s1;
    dst[0] = s0;
    s0 = src[2];
    s1 = src[3];
    dst[3] = s1;
    dst[2] = s0;
}

/*
 * sfbuf_RingGetSub - Internal ring buffer get operation
 *
 * Retrieves data from the ring buffer at the specified index.
 * Mode 0 = write access, Mode 1 = read access.
 *
 * @param self  Buffer handle
 * @param idx   Buffer index (passed as pointer value)
 * @param out   Output data structure
 * @param mode  Access mode (0=write, 1=read)
 */
void sfbuf_RingGetSub(void *self, int idx, u32 *out, int mode) {
    u8 *p = sfbuf_base(self, idx);
    u32 active = *(u32 *)(p + 0x13BC);
    void *ringObj = *(void **)(p + 0x13CC);

    /* Clear output structure */
    out[0] = 0;
    out[1] = 0;
    out[2] = 0;
    out[3] = 0;
    out[4] = 0;
    out[5] = 0;
    out[6] = 0;

    /* Check if buffer is active */
    if (active == 0 || ringObj == NULL) {
        return;
    }

    /* Call virtual methods on the ring buffer object */
    {
        void *vtable = *(void **)ringObj;
        u32 (*getFunc)(void *, int) = (u32 (*)(void *, int))((void **)vtable)[9];
        u32 (*infoFunc)(void *, int, u32, u32 *) =
            (u32 (*)(void *, int, u32, u32 *))((void **)vtable)[6];
        u32 (*readFunc)(void *, int, u32 *) =
            (u32 (*)(void *, int, u32 *))((void **)vtable)[7];

        u32 dataAvail = getFunc(ringObj, mode);
        u32 info[2];
        u32 chunk[2];

        infoFunc(ringObj, mode, 0x7FFFFFFF, info);

        if (info[1] < dataAvail) {
            /* Need second chunk */
            infoFunc(ringObj, mode, 0x7FFFFFFF, chunk);
            readFunc(ringObj, mode, chunk);
        } else {
            chunk[0] = 0;
            chunk[1] = 0;
        }

        /* Get final data */
        infoFunc(ringObj, mode, 0x7FFFFFFF, info);
        readFunc(ringObj, mode, info);

        /* Copy results to output */
        out[0] = info[0];
        out[1] = info[1];
        out[2] = chunk[0];
        out[3] = chunk[1];
    }
}

/*
 * SFBUF_RingGetWrite - Get write access to ring buffer
 */
void SFBUF_RingGetWrite(void *self, int idx, u32 *out) {
    sfbuf_RingGetSub(self, idx, out, 0);
}

/*
 * SFBUF_RingGetRead - Get read access to ring buffer
 */
void SFBUF_RingGetRead(void *self, int idx, u32 *out) {
    sfbuf_RingGetSub(self, idx, out, 1);
}

/*
 * sfbuf_RingAddSub - Internal ring buffer add operation
 *
 * Adds data to the ring buffer at the specified index.
 * Handles write pointer advancement and PTS tracking.
 *
 * @param self   Buffer handle
 * @param idx    Buffer index
 * @param size   Data size to add
 * @param mode   Access mode (0=write, 1=read)
 * @return 0 on success, error code on failure
 */
void sfbuf_RingAddSub(void *self, int idx, u32 size, int mode) {
    u8 *p = sfbuf_base(self, idx);
    u8 *supply = p + 0x13C8;
    u32 active = *(u32 *)(p + 0x13BC);
    void *ringObj = *(void **)(p + 0x13CC);
    u32 result = 0;

    /* Check inputs */
    if (size == 0) return;
    if (active == 0 || ringObj == NULL) return;

    /* Call virtual methods on the ring buffer object */
    {
        void *vtable = *(void **)ringObj;
        u32 (*infoFunc)(void *, int, u32, u32 *) =
            (u32 (*)(void *, int, u32, u32 *))((void **)vtable)[6];
        u32 (*addFunc)(void *, int, u32 *) =
            (u32 (*)(void *, int, u32 *))((void **)vtable)[8];
        u32 (*getFunc)(void *, int) = (u32 (*)(void *, int))((void **)vtable)[9];

        u32 isRead = (mode == 1) ? 1 : 0;
        u32 info[2];

        /* Get available space */
        infoFunc(ringObj, isRead, 0x7FFFFFFF, info);
        addFunc(ringObj, isRead, info);

        if (info[1] < size) {
            /* Not enough space - try second chunk */
            u32 remaining = size - info[1];
            u32 chunk[2];

            infoFunc(ringObj, isRead, remaining, chunk);
            addFunc(ringObj, isRead, chunk);

            if (chunk[1] < remaining) {
                SFLIB_SetErr(self, SFBUF_ERR_BASE + 0x0B);
                result = 1;
            }
        }

        /* Update PTS tracking for read mode */
        if (mode == 1 && idx == 1) {
            u32 *dlmPtr = (u32 *)(p + 0x13CC);
            u32 *ptsPtr = (u32 *)(p + 0x13E0);
            u32 dlm[2];
            u32 pts[2];
            u32 (*dlmFunc)(void *, int, u32 *) =
                (u32 (*)(void *, int, u32 *))((void **)vtable)[6];
            u32 (*ptsFunc)(void *, int, u32 *) =
                (u32 (*)(void *, int, u32 *))((void **)vtable)[7];

            dlmFunc(ringObj, 1, dlm);
            ptsFunc(ringObj, 1, pts);

            /* Check PTS validity */
            if (dlm[0] < pts[0] || dlm[0] >= pts[0] + pts[1]) {
                /* PTS out of range - reset */
                ptsFunc(ringObj, 1, pts);
                if (dlm[0] >= pts[0] && dlm[0] < pts[0] + pts[1]) {
                    /* Still valid */
                } else {
                    *(u32 *)(p + 0x13E0) = 0;
                    *(u32 *)(p + 0x13E4) = 0;
                }
            }
        }

        /* Update counters */
        if (mode == 1) {
            u32 *cntPtr = (u32 *)(p + 0x13E4);
            if ((s32)*cntPtr >= 0) {
                *cntPtr += size;
            }
        } else {
            u32 *cntPtr = (u32 *)(p + 0x13E0);
            if ((s32)*cntPtr >= 0) {
                *cntPtr += size;
            }
        }
    }

    /* Mark buffer as updated */
    *(u32 *)((u8 *)self + 0x50) = 1;
}

/*
 * SFBUF_RingAddWrite - Add data to ring buffer (write mode)
 */
void SFBUF_RingAddWrite(void *self, int idx, u32 size) {
    sfbuf_RingAddSub(self, idx, size, 0);
}

/*
 * SFBUF_RingAddRead - Add data to ring buffer (read mode)
 */
void SFBUF_RingAddRead(void *self, int idx, u32 size) {
    sfbuf_RingAddSub(self, idx, size, 1);
}

/*
 * SFBUF_RingGetDlm - Get DLM (decode latency marker) values
 *
 * Thread-safe read of the DLM pointer and size from the ring buffer.
 *
 * @param self  Buffer handle
 * @param idx   Buffer index
 * @param dlm   Output: DLM pointer
 * @param size  Output: DLM size
 */
void SFBUF_RingGetDlm(void *self, int idx, u32 *dlm, u32 *size) {
    u8 *p = (u8 *)self + idx * 0x74;
    u32 cs;

    SFLIB_LockCs(&cs);
    *dlm = *(u32 *)(p + 0x13E0);
    *size = *(u32 *)(p + 0x13E4);
    SFLIB_UnlockCs(&cs);
}

/*
 * SFBUF_RingSetDlm - Set DLM (decode latency marker) values
 *
 * Thread-safe write of the DLM pointer and size to the ring buffer.
 *
 * @param self  Buffer handle
 * @param idx   Buffer index
 * @param dlm   DLM pointer value
 * @param size  DLM size value
 */
void SFBUF_RingSetDlm(void *self, int idx, u32 dlm, u32 size) {
    u8 *p = (u8 *)self + idx * 0x74;
    u32 cs;

    SFLIB_LockCs(&cs);
    *(u32 *)(p + 0x13E0) = dlm;
    *(u32 *)(p + 0x13E4) = size;
    SFLIB_UnlockCs(&cs);
}

/*
 * SFBUF_GetRingBufSiz - Get ring buffer size
 *
 * @param self  Buffer handle
 * @param idx   Buffer index
 * @return Buffer size in bytes
 */
u32 SFBUF_GetRingBufSiz(void *self, u32 idx) {
    return *(u32 *)((u8 *)self + 0x13D4 + idx * SFBUF_BUF_STRIDE);
}

/*
 * SFBUF_GetRTot - Get read total counter
 *
 * @param self  Buffer handle
 * @param idx   Buffer index
 * @return Read total
 */
u32 SFBUF_GetRTot(void *self, u32 idx) {
    return *(u32 *)((u8 *)self + 0x13EC + idx * SFBUF_BUF_STRIDE);
}

/*
 * SFBUF_GetWTot - Get write total counter (thread-safe)
 *
 * Computes the write total by combining the stored write total
 * with the current ring buffer write position when needed.
 *
 * @param self  Buffer handle
 * @param idx   Buffer index
 * @return Write total (clamped to 0x7FFFFFFF)
 */
u32 SFBUF_GetWTot(void *self, int idx) {
    u8 *p = (u8 *)self + idx * 0x74;
    u32 cs;
    u32 wtot;
    u32 rtot;

    SFLIB_LockCs(&cs);

    wtot = *(u32 *)(p + 0x13E8);
    rtot = *(u32 *)(p + 0x13EC);

    if (wtot == 0 && rtot != 0) {
        void *ringObj = *(void **)(p + 0x13CC);
        void *vtable = *(void **)ringObj;
        u32 (*getFunc)(void *, int) = (u32 (*)(void *, int))((void **)vtable)[9];
        wtot = rtot + getFunc(ringObj, 1);
    }

    if ((s32)wtot < 0) {
        wtot = 0x7FFFFFFF;
    }

    SFLIB_UnlockCs(&cs);

    return wtot;
}

/*
 * SFBUF_RingGetSj - Get ring buffer SJ (supply) object
 *
 * @param self  Buffer handle
 * @param idx   Buffer index
 * @param out   Output: pointer to store SJ object
 * @return 0 on success, error code on failure
 */
s32 SFBUF_RingGetSj(void *self, s32 idx, void **out) {
    u8 *p = sfbuf_base(self, idx);

    *out = NULL;

    /* Check if buffer is active */
    if (*(u32 *)(p + 0x13BC) == 0) {
        return SFLIB_SetErr(self, SFBUF_ERR_BASE + 0x01);
    }

    /* Return the SJ object */
    *out = *(void **)(p + 0x13CC);
    return 0;
}

/*
 * SFBUF_AddRtotSj - Add to read total counter
 *
 * Increments the read total for the specified buffer.
 * Skips the update if the counter is negative (disabled).
 *
 * @param self    Buffer handle
 * @param idx     Buffer index
 * @param addend  Value to add to read total
 */
void SFBUF_AddRtotSj(void *self, int idx, int addend) {
    u32 *ptr = (u32 *)((u8 *)self + idx * SFBUF_BUF_STRIDE + 0x13EC);
    if ((s32)*ptr < 0) return;
    *ptr += addend;
}

/*
 * SFBUF_VfrmGetRead - Get video frame read data
 *
 * If the buffer is not active, delegates to the transport
 * layer to handle the video frame read.
 *
 * @param self  Buffer handle
 * @param idx   Buffer index
 * @param out   Output data pointer
 * @param size  Requested size
 * @return Result from transport layer, or 0 if active
 */
int SFBUF_VfrmGetRead(void *self, int idx, int *out, int size) {
    u8 *p = sfbuf_base(self, idx);

    if (*(u32 *)(p + 0x13BC) == 0) {
        return SFTRN_CallTrtTrif(self, *(int *)(p + 0x1404), 0x0B, out, size);
    }
    return 0;
}

/*
 * SFBUF_VfrmAddRead - Add to video frame read counter
 *
 * Updates the video frame read position and marks buffer as updated.
 * Delegates to transport layer if buffer is not active.
 *
 * @param self  Buffer handle
 * @param idx   Buffer index
 * @param size  Data size to add
 * @param flags Additional flags
 */
void SFBUF_VfrmAddRead(void *self, int idx, u32 size, u32 flags) {
    u8 *p = (u8 *)self + idx * 0x74;
    s32 result;
    u32 savedFlags;

    result = 0;
    savedFlags = flags;

    if (*(u32 *)(p + 0x13BC) == 0) {
        result = SFTRN_CallTrtTrif(self, *(int *)(p + 0x1404), 0x0C,
                                   (int *)(u32)size, (int)savedFlags);
    }

    *(u32 *)((u8 *)self + 0x50) = 1;
}

/*
 * SFBUF_SetPrepFlg - Set preparation flag
 *
 * @param self  Buffer handle
 * @param idx   Buffer index (8 = skip)
 * @param val   Flag value
 */
void SFBUF_SetPrepFlg(void *self, u32 idx, u32 val) {
    if (idx == SFBUF_MAX_IDX) return;
    *(u32 *)((u8 *)self + idx * SFBUF_BUF_STRIDE + 0x13C0) = val;
}

/*
 * SFBUF_GetPrepFlg - Get preparation flag
 *
 * @param self  Buffer handle
 * @param idx   Buffer index (8 = return 0)
 * @return Flag value
 */
int SFBUF_GetPrepFlg(void *self, int idx) {
    if (idx == SFBUF_MAX_IDX) return 0;
    return *(u32 *)((u8 *)self + idx * SFBUF_BUF_STRIDE + 0x13C0);
}

/*
 * SFBUF_SetTermFlg - Set termination flag
 *
 * @param self  Buffer handle
 * @param idx   Buffer index (8 = skip)
 * @param val   Flag value
 */
void SFBUF_SetTermFlg(void *self, s32 idx, u32 val) {
    if (idx == SFBUF_MAX_IDX) return;
    *(u32 *)((u8 *)self + idx * SFBUF_BUF_STRIDE + 0x13C4) = val;
}

/*
 * SFBUF_GetTermFlg - Get termination flag
 *
 * @param self  Buffer handle
 * @param idx   Buffer index (8 = return 1)
 * @return Flag value
 */
int SFBUF_GetTermFlg(void *self, int idx) {
    if (idx == SFBUF_MAX_IDX) return 1;
    return *(u32 *)((u8 *)self + idx * SFBUF_BUF_STRIDE + 0x13C4);
}

/*
 * SFBUF_RingGetDataSiz - Get total data size in ring buffer
 *
 * Retrieves the current read buffer information and returns
 * the sum of both data segments (for circular buffer wrap).
 *
 * @param self  Buffer handle
 * @param idx   Buffer index
 * @return Total data size available
 */
s32 SFBUF_RingGetDataSiz(void *self, int idx) {
    u32 result[7];
    sfbuf_RingGetSub(self, idx, result, 1);
    return (s32)(result[1] + result[3]);
}

/*
 * SFBUF_GetFlowCnt - Get flow control counters
 *
 * Queries the flow control state from the appropriate buffer
 * object (ring buffer or memory buffer) and stores the results.
 *
 * @param self      Buffer handle
 * @param readCnt   Output: read flow count
 * @param writeCnt  Output: write flow count
 */
void SFBUF_GetFlowCnt(void *self, u32 *readCnt, u32 *writeCnt) {
    void *vtable = *(void **)self;
    u32 (*initFunc)(void *) = (u32 (*)(void *))((void **)vtable)[4];
    u32 initResult = initFunc(self);
    u32 *w = writeCnt;
    u32 *r = readCnt;

    if (initResult == (u32)lbl_eu_80606E10) {
        *w = SJRBF_GetFlowCnt(self, 0, 1);
        *r = SJRBF_GetFlowCnt(self, 1, 1);
    } else if (initResult == (u32)lbl_eu_80606E14) {
        u32 bufSize = SJMEM_GetBufSize(self);
        *r = bufSize;

        vtable = *(void **)self;
        initFunc = (u32 (*)(void *))((void **)vtable)[9];
        *w = bufSize - initFunc(self);
    } else {
        *w = 0;
        *r = 0;
    }
}

/*
 * SFBUF_UpdateFlowCnt - Update flow control counter
 *
 * Computes a new flow count based on bit changes between
 * old and new values. Uses leading zero count to determine
 * which bit changed.
 *
 * @param count   Current flow count
 * @param newVal  New value
 * @param oldVal  Old value
 * @return Updated flow count
 */
int SFBUF_UpdateFlowCnt(int count, int newVal, int oldVal) {
    u32 diff = newVal ^ oldVal;
    u32 leading = __cntlzw(diff);
    u32 shifted = newVal << leading;
    u32 bit = shifted >> 31;
    return count + (int)bit;
}
