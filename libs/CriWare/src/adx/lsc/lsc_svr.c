// LSC stream server entry: state machine that advances/binds stream entries.
// The LSC handle holds a fixed ring of up-to-16 stat entries. The "entry" for
// the active slot is address h + (index << 5); the index loops modulo 16.
// Entry fields live at +0x3C..+0x54 inside each fixed-size slot.

#include <harness_catalog.h>
#include <string.h>

extern void ADXSTM_StopNw(void *);
extern void ADXSTM_ReleaseFileNw(void *);
extern int ADXSTM_BindFileNw(void *, const char *, s32, s32, s64);
extern void ADXSTM_SetEos(void *, s32);
extern void ADXSTM_SetBufSize(void *, s32, s32);
extern void ADXSTM_Seek(void *, s32);
extern void ADXSTM_Start(void *);
extern int ADXSTM_GetStat(void *);
extern int ADXSTM_Tell(void *);
extern void LSC_CallErrFunc_(const char *, ...);
extern void LSC_CallStatFunc(void *);
extern int LSC_EntryFileRange(void *, const char *, s32, s32, s32);

extern char lbl_eu_80518420[];

/* One stream slot. Only the fields below +0x3C are referenced by this server. */
struct LSC_Entry {
    u8 pad[0x3C];      /* unreferenced slot header */
    const char *fname; /* +0x3C */
    u32 checksum;      /* +0x40 */
    s32 offLo;         /* +0x44 */
    s32 offHi;         /* +0x48 */
    s32 size;          /* +0x4C */
    s32 state;         /* +0x50: 0 idle, 1 binding, 2 done */
    s32 pos;           /* +0x54 */
};

/* LSC stream-server handle. Header fields, then the ring of slots. */
struct LSC_Hndl {
    u8  pad0;      /* +0x00 */
    s8  st;        /* +0x01 status */
    u8  busy;      /* +0x02 */
    s8  mode3;     /* +0x03 */
    s8  paused;    /* +0x04 */
    u8  pad1[0x0F];/* +0x05..0x13 */
    s32 bufLo;     /* +0x14 */
    s32 bufHi;     /* +0x18 */
    u8  pad2[0x04];/* +0x1C */
    s32 index;     /* +0x20 active slot index */
    s32 count;     /* +0x24 remaining count */
    void *stream;  /* +0x28 */
    s32 curpos;    /* +0x2C */
};

/* Resolve the active slot: entry = h + (index << 5). */
static inline struct LSC_Entry *lsc_entry(struct LSC_Hndl *h) {
    return (struct LSC_Entry *)((char *)h + (h->index << 5));
}

void lsc_StatWait(struct LSC_Hndl *h);
void lsc_StatEnd(struct LSC_Hndl *h);

/* Wait for a stream operation, then bind (or re-bind) the active file entry. */
void lsc_StatWait(struct LSC_Hndl *h) {
    struct LSC_Entry *e = lsc_entry(h);
    const char *fname;
    s32 flen, sum, i, cnt;

    if (h->count <= 0) {
        return;
    }

    ADXSTM_StopNw(h->stream);
    ADXSTM_ReleaseFileNw(h->stream);

    fname = e->fname;
    flen = (s32)strlen(fname);
    sum = 0;
    i = 0;
    if (flen > 0) {
        cnt = (flen - 1) >> 3;
        if (flen - 8 > 0) {
            /* Unrolled checksum pass, 8 bytes per iteration. */
            for (; cnt > 0; cnt--) {
                sum += (u8)fname[i] + (u8)fname[i + 1] + (u8)fname[i + 2]
                     + (u8)fname[i + 3] + (u8)fname[i + 4] + (u8)fname[i + 5]
                     + (u8)fname[i + 6] + (u8)fname[i + 7];
                i += 8;
            }
        }
        /* Remainder bytes. */
        for (; i < flen; i++) {
            sum += (u8)fname[i];
        }
    }

    if ((u32)sum != e->checksum) {
        LSC_CallErrFunc_(lbl_eu_80518420, fname);
        return;
    }

    ADXSTM_BindFileNw(h->stream, fname, e->offLo, e->offHi, (s64)e->size << 11);
    ADXSTM_SetEos(h->stream, e->size);

    h->curpos = e->size;
    e->pos = 0;
    h->busy = 0;

    ADXSTM_SetBufSize(h->stream, h->bufLo, h->bufHi);
    ADXSTM_Seek(h->stream, 0);
    ADXSTM_Start(h->stream);

    h->busy = 1;
    e->state = 1;
}

/* End a stat operation: advance to the next slot and start it if eligible. */
void lsc_StatEnd(struct LSC_Hndl *h) {
    const char *fname = NULL;
    s32 offLo = 0, offHi = 0, size = 0, cnt, idx, hb, rot;

    if (h->stream == NULL) {
        return;
    }

    if (h->mode3 == 1) {
        struct LSC_Entry *e = lsc_entry(h);
        fname = e->fname;
        offLo = e->offLo;
        offHi = e->offHi;
        size = e->size;
    }

    /* Advance slot index (circular over 16) and decrement the count. */
    idx = h->index + 1;
    cnt = h->count - 1;
    rot = (idx << 28) & 0xF0000000;
    h->count = cnt;
    hb = idx >> 31;
    rot = rot - hb;
    h->index = ((rot << 4) | ((u32)rot >> 28)) + hb;

    if (cnt <= 0) {
        LSC_CallStatFunc(h);
        h->st = 1;
    }

    if (h->mode3 == 1) {
        LSC_EntryFileRange(h, fname, offLo, offHi, size);
    }
}

/* Handle status of the active stream and drive the state machine. */
void lsc_ExecHndl(struct LSC_Hndl *h) {
    struct LSC_Entry *e;
    void *stream;

    if (h->paused == 1) {
        return;
    }
    if (h->st != 2) {
        return;
    }
    if (h->count <= 0) {
        return;
    }

    e = lsc_entry(h);
    if (e->state == 1) {
        stream = h->stream;
        if (stream == NULL) {
            LSC_CallErrFunc_(&lbl_eu_80518420[0x40]);
        } else {
            int stat = ADXSTM_GetStat(stream);
            if (stat == 4) {
                h->st = 3;
            } else if (stat == 2) {
                e->pos = ADXSTM_Tell(h->stream);
            } else if (stat == 3) {
                e->pos = h->curpos;
                e->state = 2;
            }
        }
    }

    e = lsc_entry(h);
    if (e->state == 2) {
        lsc_StatEnd(h);
    }

    e = lsc_entry(h);
    if (e->state == 0) {
        lsc_StatWait(h);
    }
}