// sfd_buf.c - CriWare Sofdec buffer management unit
// Manages ring buffers, video frame buffers, and audio ring buffers
// for the Sofdec video playback system.

#include <harness_catalog.h>
#include "libs/CriWare/src/sofdec/sfdcore/sfd/sfd_types.h"

// Absorbed from criware_data (retail bytes) - generated
__attribute__((aligned(8))) void * lbl_eu_80606E10; // bss off 0x0 (scalar, spec 0x4)
void * lbl_eu_80606E14; // bss off 0x4 (scalar, spec 0x4)

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

/* Forward declarations for internal functions */
static int sfbuf_InitRingSj(SfdBuf *area, u32 *cumulative, u32 *sizes,
                              int idx, u32 extraSize);
void sfbuf_InitVfrmBuf(void *hn, void *area, u32 *cumulative,
                        u32 *sizes, int idx);
void sfbuf_InitAringBuf(void *self, u32 *cumulative, u32 *sizes, int idx);
s32 sfbuf_RingGetSub(void *self, int idx, u32 *out, int mode);
s32 sfbuf_RingAddSub(void *self, int idx, u32 size, int mode);

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

/* SJ object vtable - the first word of every SJRBF/SJMEM object points to
 * this function table (retail data at lbl_eu_80565C30 / lbl_eu_80565C00).
 * Slot names come from the retail vtable relocations: slot 3 = Destroy,
 * 4 = GetUuid, 5 = Reset, 6 = GetChunk, 7 = UngetChunk, 8 = PutChunk,
 * 9 = get-avail (SJRBF fn_80397A74 / SJMEM GetNumData), 10 = IsGetChunk,
 * 11 = EntryErrFunc. Slots 0-2 are NULL in retail (reserved base-object
 * methods). The transport supply objects (ringObj) share the layout.
 * Slots 6/7/8/9 are also invoked with narrower arg lists at some call
 * sites (retail passes fewer args there). */
struct SjObjVtbl {
    void (*reserved0)(void *self);                               /* 0x00 NULL in retail */
    void (*reserved1)(void *self);                               /* 0x04 NULL in retail */
    void (*reserved2)(void *self);                               /* 0x08 NULL in retail */
    void (*destroy)(void *self);                                 /* 0x0C SJRBF_Destroy */
    void *(*getUuid)(void *self);                                /* 0x10 SJRBF_GetUuid */
    void (*reset)(void *self);                                   /* 0x14 SJRBF_Reset */
    int (*getChunk)(void *self, int mode, int size, void *out);  /* 0x18 SJRBF_GetChunk */
    int (*ungetChunk)(void *self, int mode, void *chunk);        /* 0x1C SJRBF_UngetChunk */
    int (*putChunk)(void *self, int mode, void *chunk);          /* 0x20 SJRBF_PutChunk */
    int (*getAvail)(void *self, int mode);                       /* 0x24 SJRBF fn_80397A74 */
    int (*isGetChunk)(void *self, int mode, int size, int *out); /* 0x28 SJRBF_IsGetChunk */
    void (*entryErrFunc)(void *self, void *cb, void *arg);       /* 0x2C SJRBF_EntryErrFunc */
};

/* Fetch an SJ object's method table (its first word). */
#define SJ_VT(obj) (*(struct SjObjVtbl **)(obj))

/* Helper: get one buffer slot of the player handle */
static inline SfdBuf *sfbuf_slot(void *self, int idx) {
    return &((SfdHandle *)self)->buf[idx];
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

    /* Create ring buffer and get its handle */
    obj = SJRBF_Create(pool, 8, 0);
    lbl_eu_80606E10 = SJ_VT(obj)->getUuid(obj);
    SJ_VT(obj)->destroy(obj);

    /* Create memory buffer and get its handle */
    obj = SJMEM_Create(pool, 8);
    lbl_eu_80606E14 = SJ_VT(obj)->getUuid(obj);
    SJ_VT(obj)->destroy(obj);
}

/*
 * sfbuf_InitRingSj - Initialize a single ring buffer slot
 *
 * Sets up the ring buffer at the given index with the specified
 * buffer size, extra size, and PTS queue initialization.
 */
static int sfbuf_InitRingSj(SfdBuf *area, u32 *cumulative, u32 *sizes,
                              int idx, u32 extraSize) {
    u32 err;
    SfdBuf *b;
    int active;
    int state;
    u32 bufSize;
    struct RingHead {
        u32 f10, f14, f18, f1c, f20, f24;
    } head;
    u32 cs;
    void *ringObj;

    b = &area[idx];
    bufSize = sizes[idx];

    if (bufSize == 0) {
        /* Buffer not allocated - mark inactive */
        active = 0;
        state = 4; /* not active */
        goto store;
    }

    err = 0;

    head.f10 = err;
    active = 1;
    state = 5; /* active */
    head.f18 = cumulative[idx]; /* buffer memory pointer */
    head.f1c = bufSize - extraSize;

    /* Allocate ring buffer */
    if ((s32)(bufSize - extraSize) <= 0) {
        err = SFLIB_SetErr(NULL, SFBUF_ERR_BASE + 0x0C);
    } else {
        head.f20 = extraSize;
        head.f24 = err;
        ringObj = SJRBF_Create((void *)cumulative[idx], bufSize - extraSize, extraSize);
        head.f14 = (u32)ringObj;
        if (ringObj == NULL) {
            err = SFLIB_SetErr(NULL, SFBUF_ERR_BASE + 0x0A);
        } else {
            err = 0;
        }
    }

    if (err != 0) {
        return err;
    }

    /* Lock and initialize buffer fields */
    SFLIB_LockCs(&cs);
    b->valid = 1;
    *(struct RingHead *)&b->sup.memPtr = head;
    b->sup.dlmPos = 0;
    b->sup.dlmSize = 0;
    b->sup.wTot = 0;
    b->sup.rTot = 0;
    SFLIB_UnlockCs(&cs);

    /* Initialize PTS queue */
    SFPTS_InitPtsQue(&b->pts);

store:
    /* Set buffer metadata */
    b->state = state;
    b->valid = active;
    b->prepFlg = 0;
    b->termFlg = 0;
    b->inTrn = 9;
    b->outTrn = 9;
    return 0;
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
int SFBUF_InitHn(void *hn, void *area, u32 *cfg) {
    /* Base of the buffer area; kept live through the final state stores */
    SfdBuf *base = (SfdBuf *)area;
    u32 cumulative[8];
    /* Size table starts at cfg[2]: sizes[i] == cfg[2 + i] */
    u32 *sizes = cfg + 2;
    s32 extraSize;
    s32 r;
    u32 sum;

    /* Build cumulative offset array: starts at cfg[1], each entry adds
     * the previous size from the size table. */
    sum = cfg[1];
    cumulative[0] = sum;
    sum += sizes[0]; cumulative[1] = sum;
    sum += sizes[1]; cumulative[2] = sum;
    sum += sizes[2]; cumulative[3] = sum;
    sum += sizes[3]; cumulative[4] = sum;
    sum += sizes[4]; cumulative[5] = sum;
    sum += sizes[5]; cumulative[6] = sum;
    sum += sizes[6]; cumulative[7] = sum;

    /* Extra per-slot size = sizes[0] modulo the slot-count field */
    extraSize = (s32)sizes[0] % (s32)cfg[11];

    /* Initialize the three ring buffer slots, returning on first failure.
     * Slot 1 uses a fixed 0x800 extra size. */
    r = sfbuf_InitRingSj(base, cumulative, sizes, 0, extraSize);
    if (r != 0) return r;

    r = sfbuf_InitRingSj(base, cumulative, sizes, 1, 0x800);
    if (r != 0) return r;

    r = sfbuf_InitRingSj(base, cumulative, sizes, 2, 0);
    if (r != 0) return r;

    /* Video/audio buffer pairs for both transports */
    sfbuf_InitVfrmBuf(hn, base, cumulative, sizes, 3);
    sfbuf_InitAringBuf(base, cumulative, sizes, 4);
    sfbuf_InitVfrmBuf(hn, base, cumulative, sizes, 5);
    sfbuf_InitAringBuf(base, cumulative, sizes, 6);

    /* Set initial handle state: slot 7 is the user output channel record */
    base[7].state = 3;
    base[7].valid = 1;
    base[7].prepFlg = 0;
    base[7].termFlg = 0;
    base[7].inTrn = 9;
    base[7].outTrn = 9;
    base[7].sup.memPtr = 0;
    base[7].sup.sj = NULL;
    base[7].sup.base = 0;
    base[7].sup.size = 0;
    base[7].sup.extra = 0;
    base[7].sup.field_0x14 = 0;
    base[7].sup.dlmPos = 0;
    base[7].sup.dlmSize = 0;
    base[7].sup.wTot = 0;
    base[7].sup.rTot = 0;
    base[7].pts.entries = NULL;
    base[7].pts.maxIdx = 0;
    return 0;
}

/*
 * SFBUF_DestroySj - Destroy all active SJ buffer objects
 *
 * Iterates through the three primary buffer types (ring, vfrm, aring)
 * and destroys their associated SJ objects if they are in the active state.
 */
/* Destroys the SJ object of one buffer slot and clears it. Kept out of
 * SFBUF_DestroySj's dataflow so the record address stays materialized
 * across the virtual call. */
static void sfbuf_DestroySlot(SfdSupply *slot) {
    if (slot->sj != NULL) {
        SJ_VT(slot->sj)->destroy(slot->sj);
        slot->sj = NULL;
    }
}

/* The supply record address is kept in its own local: retail materialises the
 * slot base once and indexes the SJ object off it (a directly derived
 * member pointer loses that shape and costs the match). */
void SFBUF_DestroySj(void *self) {
    u32 *rec = (u32 *)((u8 *)self + 0x13C8);
    u8 *base = (u8 *)self;

    /* Ring buffer */
    if (*(s32 *)(base + 0x13B8) == SFBUF_STATE_ACTIVE) {
        sfbuf_DestroySlot((SfdSupply *)rec);
    }

    /* Vfrm buffer */
    rec = (u32 *)(base + 0x143C);
    if (*(s32 *)(base + 0x142C) == SFBUF_STATE_ACTIVE) {
        sfbuf_DestroySlot((SfdSupply *)rec);
    }

    /* Aring buffer */
    rec = (u32 *)(base + 0x14B0);
    if (*(s32 *)(base + 0x14A0) == SFBUF_STATE_ACTIVE) {
        sfbuf_DestroySlot((SfdSupply *)rec);
    }
}

/*
 * sfbuf_InitVfrmBuf - Initialize video frame buffer
 *
 * Sets up a video frame buffer at the given index with the specified
 * sizes and pointers. Initializes the VFRM frame queue structure.
 */
void sfbuf_InitVfrmBuf(void *hn, void *area, u32 *cumulative,
                        u32 *sizes, int idx) {
    u8 *p = (u8 *)area + idx * SFBUF_BUF_STRIDE;
    u32 ptrVal = sizes[idx];
    u32 valid = (u32)(-(s32)ptrVal | ptrVal) >> 31; /* non-zero check */
    SfdVfrm *vfrm = ((SfdHandle *)hn)->vfrm;
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

    /* Clear the 16 frame slots of the VFRM queue (stride 0x88) */
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
    u32 valid = (u32)(sizes[idx] != 0); /* non-zero check - local forces early load */

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
s32 SFBUF_SetSupplySj(void *self, u32 *supply) {
    SfdHandle *hn = (SfdHandle *)self;
    SfdBuf *b;
    s32 res;
    int idx;
    u32 cs;
    u32 flag;

    /* Validate supply configuration */
    res = 0;
    if (supply[1] == 0) {
        res = -1;
    } else if (supply[0] == 0) {
        if (supply[2] == 0) {
            res = -1;
        } else if ((s32)supply[3] <= 0) {
            res = -1;
        } else if ((s32)supply[5] > 0) {
            res = -1;
        }
    }

    if (res != 0) {
        return SFLIB_SetErr(self, SFBUF_ERR_BASE + 0x08);
    }

    /* Determine buffer index from transport type */
    if (SFTRN_IsSetup(self, 1)) {
        idx = 0;
    } else if (SFTRN_IsSetup(self, 2)) {
        idx = 1;
    } else {
        /* No video/audio transport: a set-up subtitle transport selects
         * buffer 2, otherwise the default index 0 is used. */
        s32 sub = SFTRN_IsSetup(self, 3);
        idx = 0;
        if (sub) {
            idx = 2;
        }
    }

    b = &hn->buf[idx];

    /* Check that buffer is in ready state */
    if (b->state != SFBUF_STATE_READY) {
        return SFLIB_SetErr(self, SFBUF_ERR_BASE + 0x09);
    }

    /* Compute non-zero flag for supply size */
    flag = (u32)(-(s32)supply[1] | supply[1]) >> 31;

    SFLIB_LockCs(&cs);

    /* Store active flag */
    b->valid = flag;

    /* Copy the supply block (memory, SJ object, ring geometry) */
    b->sup.memPtr = supply[0];
    b->sup.sj = (void *)supply[1];
    b->sup.base = supply[2];
    b->sup.size = supply[3];
    b->sup.extra = supply[4];
    b->sup.field_0x14 = supply[5];

    /* Clear counters and state */
    b->sup.dlmPos = 0;
    b->sup.dlmSize = 0;
    b->sup.wTot = 0;
    b->sup.rTot = 0;

    SFLIB_UnlockCs(&cs);

    /* Reset PTS queue */
    SFPTS_ResetPtsQue(&b->pts);

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
    SfdBuf *slot;

    if (SFTRN_IsSetup(self, 1)) {
        bufIdx = 0;
    } else if (SFTRN_IsSetup(self, 2)) {
        bufIdx = 1;
    } else {
        /* No video/audio transport: a set-up subtitle transport selects
         * buffer 2, otherwise the default index 0 is used. */
        s32 sub = SFTRN_IsSetup(self, 3);
        bufIdx = 0;
        if (sub) {
            bufIdx = 2;
        }
    }

    slot = sfbuf_slot(self, bufIdx);

    if (slot->valid == 0 || slot->sup.sj == 0) {
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
typedef struct SfbufUoch {
    u32 w[4];
} SfbufUoch;

void SFBUF_SetUoch(void *self, int idx, int sub_idx, u32 *src) {
    ((SfbufUoch *)((u8 *)self + idx * SFBUF_BUF_STRIDE + 0x13C8))[sub_idx] = *(SfbufUoch *)src;
}

/*
 * SFBUF_GetUoch - Get UOCH (user operation channel) data
 *
 * Reads a 4-field UOCH structure from the buffer's UOCH slot.
 */
void SFBUF_GetUoch(void *self, int idx, int sub_idx, u32 *dst) {
    *(SfbufUoch *)dst = ((SfbufUoch *)((u8 *)self + idx * SFBUF_BUF_STRIDE + 0x13C8))[sub_idx];
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
s32 sfbuf_RingGetSub(void *self, int idx, u32 *out, int mode) {
    SfdBuf *b = sfbuf_slot(self, idx);
    u32 zero = 0;
    u32 maxCnt = 0x7FFFFFFF;
    u32 avail;
    u32 info[2];
    u32 chunk[2];
    u32 active;
    void *ringObj;
    s32 i;

    /* Clear output structure */
    for (i = 0; i < 7; i++) {
        out[i] = zero;
    }

    /* Check if buffer is active */
    active = b->valid;
    ringObj = b->sup.sj;
    if (active == 0 || ringObj == NULL) {
        return 0;
    }

    /* Query available size and take a chunk covering the whole ring */
    avail = (u32)SJ_VT(ringObj)->getAvail(ringObj, mode);
    SJ_VT(ringObj)->getChunk(ringObj, mode, maxCnt, info);

    if ((s32)info[1] < (s32)avail) {
        /* Data wraps: take and return the second (pre-wrap) chunk */
        SJ_VT(ringObj)->getChunk(ringObj, mode, maxCnt, chunk);
        SJ_VT(ringObj)->ungetChunk(ringObj, mode, chunk);
    } else {
        chunk[0] = zero;
        chunk[1] = zero;
    }

    /* Undo the initial chunk reservation */
    SJ_VT(ringObj)->ungetChunk(ringObj, mode, info);

    out[0] = info[0];
    out[1] = info[1];
    out[2] = chunk[0];
    out[3] = chunk[1];
    return 0;
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
s32 sfbuf_RingAddSub(void *self, int idx, u32 size, int mode) {
    SfdBuf *b = sfbuf_slot(self, idx);
    s32 result = 0;
    void *rb = b->sup.sj;

    if (size == 0) {
        return 0;
    }
    if (b->valid == 0 || rb == NULL) {
        return 0;
    }

    {
        u32 info[2];

        /* Reserve space for the incoming data. Note: retail passes the raw
         * access-mode word as the size argument of this first call. */
        SJ_VT(rb)->getChunk(rb, mode, size, info);
        SJ_VT(rb)->putChunk(rb, (mode == 0), info);

        if ((s32)info[1] < (s32)size) {
            /* Ring wraps: reserve the pre-wrap remainder too */
            u32 chunk[2];
            u32 remain = size - info[1];

            SJ_VT(rb)->getChunk(rb, mode, remain, chunk);
            SJ_VT(rb)->putChunk(rb, (mode == 0), chunk);

            if ((s32)chunk[1] < (s32)remain) {
                result = SFLIB_SetErr(self, SFBUF_ERR_BASE + 0x0B);
            }
        }

        /* Track DLM (decode latency marker) position validity for reads
         * on the video ring only. */
        if (mode == 1 && idx == 1) {
            void *sj = b->sup.sj;
            u32 avail;
            u32 buf2[2];
            u32 buf[2];
            u32 pos;

            avail = SJ_VT(sj)->getAvail(sj, 1);
            SJ_VT(sj)->getChunk(sj, 1, 0x7FFFFFFF, buf);

            if ((s32)buf[1] < (s32)avail) {
                /* Data wraps: fetch and inspect the second segment */
                SJ_VT(sj)->getChunk(sj, 1, 0x7FFFFFFF, buf2);
                SJ_VT(sj)->ungetChunk(sj, 1, buf2);
            } else {
                buf2[0] = 0;
                buf2[1] = 0;
            }

            SJ_VT(sj)->ungetChunk(sj, 1, buf);

            /* If the recorded DLM position lies in neither segment,
             * it no longer points at live data - clear it. */
            pos = b->sup.dlmPos;
            if (!((pos >= buf[0] && pos < buf[0] + buf[1]) ||
                  (pos >= buf2[0] && pos < buf2[0] + buf2[1]))) {
                b->sup.dlmPos = 0;
                b->sup.dlmSize = 0;
            }
        }

        /* Advance the running total (read total for reads, write total
         * for writes) unless it is disabled (negative). */
        if (mode == 1) {
            if ((s32)b->sup.rTot >= 0) {
                b->sup.rTot += size;
            }
        } else {
            if ((s32)b->sup.wTot >= 0) {
                b->sup.wTot += size;
            }
        }
    }

    /* Mark handle as updated */
    ((SfdHandle *)self)->field_0x0050 = 1;
    return result;
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
    SfdBuf *b = sfbuf_slot(self, idx);
    u32 cs;

    SFLIB_LockCs(&cs);
    *dlm = b->sup.dlmPos;
    *size = b->sup.dlmSize;
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
    SfdBuf *b = sfbuf_slot(self, idx);
    u32 cs;

    SFLIB_LockCs(&cs);
    b->sup.dlmPos = dlm;
    b->sup.dlmSize = size;
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
    return sfbuf_slot(self, idx)->sup.size;
}

/*
 * SFBUF_GetRTot - Get read total counter
 *
 * @param self  Buffer handle
 * @param idx   Buffer index
 * @return Read total
 */
u32 SFBUF_GetRTot(void *self, u32 idx) {
    return sfbuf_slot(self, idx)->sup.rTot;
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
    SfdBuf *b = sfbuf_slot(self, idx);
    u32 cs;
    u32 wtot;
    u32 rtot;

    SFLIB_LockCs(&cs);

    wtot = b->sup.wTot;
    rtot = b->sup.rTot;

    if (wtot == 0 && rtot != 0) {
        void *ringObj = b->sup.sj;
        wtot = rtot + (u32)SJ_VT(ringObj)->getAvail(ringObj, 1);
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
    SfdBuf *b = sfbuf_slot(self, idx);

    *out = NULL;

    /* Check if buffer is active */
    if (b->valid == 0) {
        return SFLIB_SetErr(self, SFBUF_ERR_BASE + 0x01);
    }

    /* Return the SJ object */
    *out = b->sup.sj;
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
    u32 *ptr = &sfbuf_slot(self, idx)->sup.rTot;
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
    SfdBuf *b = sfbuf_slot(self, idx);

    if (b->valid == 0) {
        return SFTRN_CallTrtTrif(self, b->inTrn, 0x0B, out, size);
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
int SFBUF_VfrmAddRead(void *self, int idx, u32 size, u32 flags) {
    SfdBuf *b = sfbuf_slot(self, idx);
    u32 savedFlags = flags;
    s32 result = 0;

    if (b->valid == 0) {
        result = SFTRN_CallTrtTrif(self, b->inTrn, 0x0C,
                                   (int *)(u32)size, (int)savedFlags);
    }

    ((SfdHandle *)self)->field_0x0050 = 1;
    return result;
}

/*
 * SFBUF_SetPrepFlg - Set preparation flag
 *
 * @param self  Buffer handle
 * @param idx   Buffer index (8 = skip)
 * @param val   Flag value
 */
void SFBUF_SetPrepFlg(void *self, s32 idx, u32 val) {
    if (idx == SFBUF_MAX_IDX) return;
    sfbuf_slot(self, idx)->prepFlg = val;
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
    return sfbuf_slot(self, idx)->prepFlg;
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
    sfbuf_slot(self, idx)->termFlg = val;
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
    return sfbuf_slot(self, idx)->termFlg;
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
    u32 *w = writeCnt;
    u32 *r = readCnt;
    int flag;

    /* Identify the buffer object by its UUID (virtual slot 0x10) and
     * query flow counters through the matching concrete API. */
    flag = (SJ_VT(self)->getUuid(self) == lbl_eu_80606E10);
    if (flag) {
        *r = SJRBF_GetFlowCnt(self, 1, 1);
        *w = SJRBF_GetFlowCnt(self, 0, 1);
    } else {
        flag = (SJ_VT(self)->getUuid(self) == lbl_eu_80606E14);
        if (flag) {
            *r = SJMEM_GetBufSize(self);
            *w = *r - (u32)SJ_VT(self)->getAvail(self, 1);
        } else {
            *r = 0;
            *w = 0;
        }
    }
}

/*
 * SFBUF_UpdateFlowCnt - Update flow control counter
 *
 * Advances the 64-bit "supplied bytes" counter: the low word is re-synced to
 * the library count x, and the high word accumulates one carry per wrap. The
 * carry is the bit of the previous low word that differs from x. Retail builds
 * this as a *signed* 64-bit add whose high half is then discarded (the two
 * dead `srawi` sign-extensions in front of the addc are that add's remains),
 * which is why the definition returns u64: only the carry crosses into the
 * high word, but the ABI result is a two-word value. */
u64 SFBUF_UpdateFlowCnt(u32 hi, u32 lo, u32 x) {
    s32 carry = (lo << __cntlzw(lo ^ x)) >> 31;
    s64 v = carry + (s64)(s32)hi;

    return ((u64)v << 32) | x;
}
