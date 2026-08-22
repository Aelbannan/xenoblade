// sj_uni.c - SJ unified chunk management unit
// CriWare ADX SJ chunk-based allocator

#include <harness_catalog.h>

extern char lbl_eu_80518E00[];

extern u32 lbl_eu_805F1AD8;   /* init counter */
extern u8  lbl_eu_805F1AE0[]; /* work area (0xC00 bytes) */

extern char lbl_eu_80518DF0[]; /* uuid */
extern u8 lbl_eu_80565C60[];  /* vtable */

extern void SJCRS_Init(void);
extern void SJCRS_Finish(void);
extern void SJCRS_Lock(void);
extern void SJCRS_Unlock(void);
extern void SJERR_CallErr(const char *msg);
extern void CRICRW_Strcpy(char *dst, int maxlen, const char *src);
extern void CRICRW_Strcat(char *dst, int maxlen, const char *src);
extern void SJUNI_Error(void);

extern void *memset(void *, int, unsigned long);

typedef struct SJUNI_CHUNK {
    struct SJUNI_CHUNK *next; /* +0x00 */
    u32 pad;                  /* +0x04 */
    unsigned char *ptr;       /* +0x08 */
    int size;                 /* +0x0C */
} SJUNI_CHUNK; /* size 0x10 */

typedef struct {
    void *vtable;         /* +0x00 */
    s8 valid;             /* +0x04 */
    s8 index;             /* +0x05 */
    u8 pad06[2];          /* +0x06 */
    void *uuid;           /* +0x08 */
    u8 *pool_mem;         /* +0x0C */
    int num_chunks;       /* +0x10 */
    SJUNI_CHUNK *free_head; /* +0x14 */
    SJUNI_CHUNK *queue[4];  /* +0x18, +0x1C, +0x20, +0x24 */
    void (*err_func)(void *, int); /* +0x28 */
    void *err_arg;        /* +0x2C */
} SJUNI; /* size 0x30 */

typedef struct {
    unsigned char *ptr;
    int size;
} SJ_CHUNK;

extern void SJ_SplitChunk(const SJ_CHUNK *src, int size, SJ_CHUNK *dst1, SJ_CHUNK *dst2);

/* --- Internal forward declarations --- */
void *sjuni_Create(int index, void *pool_mem, int pool_size);
void  sjuni_Reset(SJUNI *self);
int   sjuni_GetNumData(SJUNI *self, int mode);
void  sjuni_GetChunk(SJUNI *self, int mode, int size, SJ_CHUNK *out);
void  sjuni_PutChunk(SJUNI *self, int mode, SJ_CHUNK *chunk);
void  sjuni_UngetChunk(SJUNI *self, int mode, SJ_CHUNK *chunk);
int   sjuni_IsGetChunk(SJUNI *self, int mode, int size, int *out);

/* --- SJUNI_Error --- */
void SJUNI_Error(void) {
    SJERR_CallErr(lbl_eu_80518E00);
}

/* --- SJUNI_Init --- */
void SJUNI_Init(void) {
    SJCRS_Init();
    SJCRS_Lock();
    if (lbl_eu_805F1AD8 == 0) {
        memset(lbl_eu_805F1AE0, 0, 0xC00);
    }
    lbl_eu_805F1AD8++;
    SJCRS_Unlock();
}

/* --- SJUNI_Finish --- */
void SJUNI_Finish(void) {
    SJCRS_Lock();
    if (--lbl_eu_805F1AD8 == 0) {
        memset(lbl_eu_805F1AE0, 0, 0xC00);
    }
    SJCRS_Unlock();
    SJCRS_Finish();
}

/* --- SJUNI_Create --- */
void *SJUNI_Create(int index, void *pool_mem, u32 pool_size) {
    void *r;
    SJCRS_Lock();
    r = sjuni_Create(index, pool_mem, pool_size);
    SJCRS_Unlock();
    return r;
}

/* --- sjuni_Create (internal) --- */
void *sjuni_Create(int index, void *pool_mem, int pool_size) {
    int i;

    for (i = 0; i < 0x40; i++) {
        if (((SJUNI *)lbl_eu_805F1AE0)[i].valid == 0) break;
    }
    if (i == 0x40) return NULL;

    SJUNI *self = &((SJUNI *)lbl_eu_805F1AE0)[i];
    self->valid = 1;
    self->vtable = lbl_eu_80565C60;
    self->index = (s8)index;
    self->uuid = lbl_eu_80518DF0;
    self->pool_mem = (u8 *)pool_mem;
    self->num_chunks = pool_size / 16;
    self->err_func = (void (*)(void *, int))SJUNI_Error;
    self->err_arg = self;
    sjuni_Reset(self);
    return self;
}

/* --- SJUNI_Destroy --- */
#pragma push
#pragma opt_propagation off
void SJUNI_Destroy(void *self_ptr) {
    SJUNI *self = (SJUNI *)self_ptr;
    char buf2[64];
    char buf1[64];
    SJCRS_Lock();
    if (self == NULL) {
        const char *suffix = lbl_eu_80518E00 + 0xC;
        CRICRW_Strcpy(buf1, 0x40, lbl_eu_80518E00 + 0x27);
        CRICRW_Strcat(buf1, 0x40, suffix);
        SJERR_CallErr(buf1);
    } else if (self->valid == 0) {
        const char *suffix = lbl_eu_80518E00 + 0x33;
        CRICRW_Strcpy(buf2, 0x40, lbl_eu_80518E00 + 0x53);
        CRICRW_Strcat(buf2, 0x40, suffix);
        SJERR_CallErr(buf2);
    } else {
        memset(self, 0, 0x30);
        self->valid = 0;
    }
    SJCRS_Unlock();
}
#pragma pop

/* --- SJUNI_GetUuid --- */
#pragma push
#pragma opt_propagation off
void *SJUNI_GetUuid(void *self_ptr) {
    SJUNI *self = (SJUNI *)self_ptr;
    void *r;
    char buf2[64];
    char buf1[64];
    SJCRS_Lock();
    if (self == NULL) {
        const char *suffix = lbl_eu_80518E00 + 0xC;
        CRICRW_Strcpy(buf1, 0x40, lbl_eu_80518E00 + 0x5F);
        CRICRW_Strcat(buf1, 0x40, suffix);
        SJERR_CallErr(buf1);
        r = NULL;
    } else if (self->valid == 0) {
        const char *suffix = lbl_eu_80518E00 + 0x33;
        CRICRW_Strcpy(buf2, 0x40, lbl_eu_80518E00 + 0x6B);
        CRICRW_Strcat(buf2, 0x40, suffix);
        SJERR_CallErr(buf2);
        r = NULL;
    } else {
        r = self->uuid;
    }
    SJCRS_Unlock();
    return r;
}
#pragma pop

/* --- SJUNI_EntryErrFunc --- */
#pragma push
#pragma opt_propagation off
void SJUNI_EntryErrFunc(void *self_ptr, void *cbfunc, void *cbarg) {
    SJUNI *self = (SJUNI *)self_ptr;
    char buf2[64];
    char buf1[64];
    SJCRS_Lock();
    if (self == NULL) {
        const char *suffix = lbl_eu_80518E00 + 0xC;
        CRICRW_Strcpy(buf1, 0x40, lbl_eu_80518E00 + 0x77);
        CRICRW_Strcat(buf1, 0x40, suffix);
        SJERR_CallErr(buf1);
    } else if (self->valid == 0) {
        const char *suffix = lbl_eu_80518E00 + 0x33;
        CRICRW_Strcpy(buf2, 0x40, lbl_eu_80518E00 + 0x83);
        CRICRW_Strcat(buf2, 0x40, suffix);
        SJERR_CallErr(buf2);
    } else {
        self->err_func = (void (*)(void *, int))cbfunc;
        self->err_arg = cbarg;
    }
    SJCRS_Unlock();
}
#pragma pop

/* --- SJUNI_Reset --- */
void SJUNI_Reset(void *self) {
    SJCRS_Lock();
    sjuni_Reset((SJUNI *)self);
    SJCRS_Unlock();
}

/* --- sjuni_Reset (internal) --- */
#pragma push
#pragma opt_propagation off
void sjuni_Reset(SJUNI *self) {
    if (self == NULL) {
        char buf[64];
        const char *suffix = lbl_eu_80518E00 + 0xC;
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518E00 + 0x8F);
        CRICRW_Strcat(buf, 0x40, suffix);
        SJERR_CallErr(buf);
    } else if (self->valid == 0) {
        char buf[64];
        const char *suffix = lbl_eu_80518E00 + 0x33;
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518E00 + 0x9B);
        CRICRW_Strcat(buf, 0x40, suffix);
        SJERR_CallErr(buf);
    } else {
        int i;
        SJUNI_CHUNK *chunks = (SJUNI_CHUNK *)self->pool_mem;
        self->free_head = chunks;
        for (i = 0; i < self->num_chunks - 1; i++) {
            chunks[i].next = &chunks[i + 1];
            chunks[i].ptr = NULL;
            chunks[i].size = 0;
        }
        chunks[i].next = NULL;
        chunks[i].ptr = NULL;
        chunks[i].size = 0;
        self->queue[0] = NULL;
        self->queue[1] = NULL;
        self->queue[2] = NULL;
        self->queue[3] = NULL;
    }
}
#pragma pop

/* --- SJUNI_GetNumData --- */
int SJUNI_GetNumData(void *self, int mode) {
    int r;
    SJCRS_Lock();
    r = sjuni_GetNumData((SJUNI *)self, mode);
    SJCRS_Unlock();
    return r;
}

/* --- sjuni_GetNumData (internal) --- */
#pragma push
#pragma opt_propagation off
int sjuni_GetNumData(SJUNI *self, int mode) {
    if (self == NULL) {
        char buf[64];
        const char *suffix = lbl_eu_80518E00 + 0xC;
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518E00 + 0xA7);
        CRICRW_Strcat(buf, 0x40, suffix);
        SJERR_CallErr(buf);
        return 0;
    }
    if (self->valid == 0) {
        char buf[64];
        const char *suffix = lbl_eu_80518E00 + 0x33;
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518E00 + 0xB3);
        CRICRW_Strcat(buf, 0x40, suffix);
        SJERR_CallErr(buf);
        return 0;
    }
    if ((u32)mode > 3) {
        if (self->err_func) self->err_func(self->err_arg, -3);
        return 0;
    }
    {
        int total = 0;
        SJUNI_CHUNK *cur = self->queue[mode];
        while (cur != NULL) {
            total += cur->size;
            cur = cur->next;
        }
        return total;
    }
}
#pragma pop

/* --- SJUNI_GetChunk --- */
int SJUNI_GetChunk(void *self, int mode, int size, SJ_CHUNK *out) {
    SJCRS_Lock();
    sjuni_GetChunk((SJUNI *)self, mode, size, out);
    SJCRS_Unlock();
}

/* --- sjuni_GetChunk (internal) --- */
#pragma push
#pragma opt_propagation off
void sjuni_GetChunk(SJUNI *self, int mode, int size, SJ_CHUNK *out) {
    if (self == NULL) {
        char buf[64];
        const char *suffix = lbl_eu_80518E00 + 0xC;
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518E00 + 0xBF);
        CRICRW_Strcat(buf, 0x40, suffix);
        SJERR_CallErr(buf);
    } else if (self->valid == 0) {
        char buf[64];
        const char *suffix = lbl_eu_80518E00 + 0x33;
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518E00 + 0xCB);
        CRICRW_Strcat(buf, 0x40, suffix);
        SJERR_CallErr(buf);
    } else if ((u32)mode > 3) {
        if (self->err_func) self->err_func(self->err_arg, -3);
        out->ptr = NULL;
        out->size = 0;
    } else {
        /* hoist &queue[mode]: MWCC keeps this address in a register across the branch */
        SJUNI_CHUNK **qslot;
        SJUNI_CHUNK *chunk;
        qslot = &self->queue[mode];
        chunk = *qslot;
        if (chunk == NULL) {
            out->ptr = NULL;
            out->size = 0;
        } else {
            SJ_CHUNK info;
            SJ_CHUNK remainder;
            int sz = chunk->size;
            unsigned char *p = chunk->ptr;
            info.ptr = p;
            info.size = sz;
            if (info.size <= size) {
                out->ptr = p;
                out->size = sz;
                *qslot = chunk->next;
                chunk->next = self->free_head;
                self->free_head = chunk;
            } else if (self->index == 1) {
                int isz;
                unsigned char *ip;
                int rsz;
                unsigned char *rp;
                SJ_SplitChunk(&info, size, &info, &remainder);
                ip = info.ptr;
                isz = info.size;
                out->size = isz;
                out->ptr = ip;
                rsz = remainder.size;
                rp = remainder.ptr;
                chunk->ptr = rp;
                chunk->size = rsz;
            } else {
                out->ptr = NULL;
                out->size = 0;
            }
        }
    }
}
#pragma pop

/* --- SJUNI_PutChunk --- */
int SJUNI_PutChunk(void *self, int mode, SJ_CHUNK *chunk) {
    SJCRS_Lock();
    sjuni_PutChunk((SJUNI *)self, mode, chunk);
    SJCRS_Unlock();
}

/* --- sjuni_PutChunk (internal) --- */
#pragma push
#pragma opt_propagation off
void sjuni_PutChunk(SJUNI *self, int mode, SJ_CHUNK *chunk) {
    if (self == NULL) {
        char buf[64];
        const char *suffix = lbl_eu_80518E00 + 0xC;
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518E00 + 0xD7);
        CRICRW_Strcat(buf, 0x40, suffix);
        SJERR_CallErr(buf);
        goto exit;
    }
    if (self->valid == 0) {
        char buf[64];
        const char *suffix = lbl_eu_80518E00 + 0x33;
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518E00 + 0xE3);
        CRICRW_Strcat(buf, 0x40, suffix);
        SJERR_CallErr(buf);
        goto exit;
    }
    if ((u32)mode > 3) {
        if (self->err_func) self->err_func(self->err_arg, -3);
        goto exit;
    }
    if (chunk->size > 0 && chunk->ptr != NULL) goto body;
    goto exit;
exit:
    return;
body:;
    {
        SJUNI_CHUNK *prev = NULL;
        SJUNI_CHUNK **tail = &self->queue[mode];
        while (*tail != NULL) {
            prev = *tail;
            tail = &(*tail)->next;
        }
        if (self->index == 1 && prev != NULL &&
            prev->ptr + prev->size == chunk->ptr) {
            prev->size += chunk->size;
            goto exit;
        }
        {
            SJUNI_CHUNK *free_chunk = self->free_head;
            if (free_chunk == NULL) {
                if (self->err_func) self->err_func(self->err_arg, -3);
            } else {
                int sz;
                unsigned char *p;
                self->free_head = free_chunk->next;
                free_chunk->next = NULL;
                sz = chunk->size;
                p = chunk->ptr;
                free_chunk->ptr = p;
                free_chunk->size = sz;
                *tail = free_chunk;
            }
        }
    }
}
#pragma pop

/* --- SJUNI_UngetChunk --- */
int SJUNI_UngetChunk(void *self, int mode, SJ_CHUNK *chunk) {
    SJCRS_Lock();
    sjuni_UngetChunk((SJUNI *)self, mode, chunk);
    SJCRS_Unlock();
}

/* --- sjuni_UngetChunk (internal) --- */
#pragma push
#pragma opt_propagation off
void sjuni_UngetChunk(SJUNI *self, int mode, SJ_CHUNK *chunk) {
    if (self == NULL) {
        char buf[64];
        const char *suffix = lbl_eu_80518E00 + 0xC;
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518E00 + 0xEF);
        CRICRW_Strcat(buf, 0x40, suffix);
        SJERR_CallErr(buf);
        goto exit;
    }
    if (self->valid == 0) {
        char buf[64];
        const char *suffix = lbl_eu_80518E00 + 0x33;
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518E00 + 0xFB);
        CRICRW_Strcat(buf, 0x40, suffix);
        SJERR_CallErr(buf);
        goto exit;
    }
    if ((u32)mode > 3) {
        if (self->err_func) self->err_func(self->err_arg, -3);
        goto exit;
    }
    if (chunk->size > 0 && chunk->ptr != NULL) goto body;
    goto exit;
exit:
    return;
body:;
    {
        SJUNI_CHUNK **queue_ptr = &self->queue[mode];
        SJUNI_CHUNK *head = *queue_ptr;
        if (self->index != 1 || head == NULL ||
            chunk->ptr + chunk->size != head->ptr)
            goto freepath;
        head->ptr = chunk->ptr;
        head->size += chunk->size;
        goto exit;
    freepath:;
        {
            SJUNI_CHUNK *free_chunk = self->free_head;
            if (free_chunk == NULL) {
                if (self->err_func) self->err_func(self->err_arg, -3);
            } else {
                int sz;
                unsigned char *p;
                self->free_head = free_chunk->next;
                free_chunk->next = NULL;
                p = chunk->ptr;
                sz = chunk->size;
                free_chunk->ptr = p;
                free_chunk->size = sz;
                free_chunk->next = *queue_ptr;
                *queue_ptr = free_chunk;
            }
        }
    }
}
#pragma pop

/* --- SJUNI_IsGetChunk --- */
int SJUNI_IsGetChunk(void *self, int mode, int size, int *out) {
    int r;
    SJCRS_Lock();
    r = sjuni_IsGetChunk((SJUNI *)self, mode, size, out);
    SJCRS_Unlock();
    return r;
}

/* --- sjuni_IsGetChunk (internal) --- */
#pragma push
#pragma opt_propagation off
int sjuni_IsGetChunk(SJUNI *self, int mode, int size, int *out) {
    if (self == NULL) {
        char buf[64];
        const char *suffix = lbl_eu_80518E00 + 0xC;
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518E00 + 0x107);
        CRICRW_Strcat(buf, 0x40, suffix);
        SJERR_CallErr(buf);
        return 0;
    }
    if (self->valid == 0) {
        char buf[64];
        const char *suffix = lbl_eu_80518E00 + 0x33;
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518E00 + 0x113);
        CRICRW_Strcat(buf, 0x40, suffix);
        SJERR_CallErr(buf);
        return 0;
    }
    *out = 0;
    if ((u32)mode > 3) {
        if (self->err_func) self->err_func(self->err_arg, -3);
        return 0;
    }
    {
        SJUNI_CHUNK *head = self->queue[mode];
        if (head == NULL) return 0;
        {
            SJ_CHUNK ck;
            SJ_CHUNK *ckp = &ck;
            ckp->ptr = head->ptr;
            ckp->size = head->size;
            *out = ckp->size;
            if (self->index == 1) {
                return (ckp->size >= size) ? 1 : 0;
            } else {
                return (ckp->size == size) ? 1 : 0;
            }
        }
    }
}
#pragma pop

/* --- SJUNI_GetNumChunk --- */
#pragma push
#pragma opt_propagation off
int SJUNI_GetNumChunk(void *self_ptr, int mode) {
    SJUNI *self = (SJUNI *)self_ptr;
    int r;
    char buf2[64];
    char buf1[64];
    SJCRS_Lock();
    if (self == NULL) {
        const char *suffix = lbl_eu_80518E00 + 0xC;
        CRICRW_Strcpy(buf1, 0x40, lbl_eu_80518E00 + 0x11F);
        CRICRW_Strcat(buf1, 0x40, suffix);
        SJERR_CallErr(buf1);
        r = 0;
    } else if (self->valid == 0) {
        const char *suffix = lbl_eu_80518E00 + 0x33;
        CRICRW_Strcpy(buf2, 0x40, lbl_eu_80518E00 + 0x12B);
        CRICRW_Strcat(buf2, 0x40, suffix);
        SJERR_CallErr(buf2);
        r = 0;
    } else {
        int count = 0;
        SJUNI_CHUNK *cur = self->queue[mode];
        while (cur != NULL) {
            cur = cur->next;
            count++;
        }
        r = count;
    }
    SJCRS_Unlock();
    return r;
}
#pragma pop
