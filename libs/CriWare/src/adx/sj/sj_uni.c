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
    u32 num_chunks;       /* +0x10 */
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
void *sjuni_Create(int index, void *pool_mem, u32 pool_size);
void  sjuni_Reset(SJUNI *self);
int   sjuni_GetNumData(SJUNI *self, int mode);
int   sjuni_GetChunk(SJUNI *self, int mode, int size, SJ_CHUNK *out);
int   sjuni_PutChunk(SJUNI *self, int mode, SJ_CHUNK *chunk);
int   sjuni_UngetChunk(SJUNI *self, int mode, SJ_CHUNK *chunk);
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
        __builtin_memset(lbl_eu_805F1AE0, 0, 0xC00);
    }
    lbl_eu_805F1AD8++;
    SJCRS_Unlock();
}

/* --- SJUNI_Finish --- */
void SJUNI_Finish(void) {
    SJCRS_Lock();
    if (--lbl_eu_805F1AD8 == 0) {
        __builtin_memset(lbl_eu_805F1AE0, 0, 0xC00);
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
void *sjuni_Create(int index, void *pool_mem, u32 pool_size) {
    SJUNI *instances = (SJUNI *)lbl_eu_805F1AE0;
    int i;

    for (i = 0; i < 0x40; i++) {
        if (instances[i].valid == 0) break;
    }
    if (i == 0x40) return NULL;

    SJUNI *self = &instances[i];
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
void SJUNI_Destroy(void *self_ptr) {
    SJUNI *self = (SJUNI *)self_ptr;
    SJCRS_Lock();
    if (self == NULL) {
        char buf1[64];
        CRICRW_Strcpy(buf1, 0x40, lbl_eu_80518E00 + 0xC);
        CRICRW_Strcat(buf1, 0x40, lbl_eu_80518E00 + 0x27);
        SJERR_CallErr(buf1);
    } else if (self->valid == 0) {
        char buf2[64];
        CRICRW_Strcpy(buf2, 0x40, lbl_eu_80518E00 + 0x33);
        CRICRW_Strcat(buf2, 0x40, lbl_eu_80518E00 + 0x53);
        SJERR_CallErr(buf2);
    } else {
        __builtin_memset(self, 0, 0x30);
        self->valid = 0;
    }
    SJCRS_Unlock();
}

/* --- SJUNI_GetUuid --- */
void *SJUNI_GetUuid(void *self_ptr) {
    SJUNI *self = (SJUNI *)self_ptr;
    void *r;
    SJCRS_Lock();
    if (self == NULL) {
        char buf1[64];
        CRICRW_Strcpy(buf1, 0x40, lbl_eu_80518E00 + 0xC);
        CRICRW_Strcat(buf1, 0x40, lbl_eu_80518E00 + 0x5F);
        SJERR_CallErr(buf1);
        r = NULL;
    } else if (self->valid == 0) {
        char buf2[64];
        CRICRW_Strcpy(buf2, 0x40, lbl_eu_80518E00 + 0x33);
        CRICRW_Strcat(buf2, 0x40, lbl_eu_80518E00 + 0x6B);
        SJERR_CallErr(buf2);
        r = NULL;
    } else {
        r = self->uuid;
    }
    SJCRS_Unlock();
    return r;
}

/* --- SJUNI_EntryErrFunc --- */
void SJUNI_EntryErrFunc(void *self_ptr, void *cbfunc, void *cbarg) {
    SJUNI *self = (SJUNI *)self_ptr;
    SJCRS_Lock();
    if (self == NULL) {
        char buf1[64];
        CRICRW_Strcpy(buf1, 0x40, lbl_eu_80518E00 + 0xC);
        CRICRW_Strcat(buf1, 0x40, lbl_eu_80518E00 + 0x77);
        SJERR_CallErr(buf1);
    } else if (self->valid == 0) {
        char buf2[64];
        CRICRW_Strcpy(buf2, 0x40, lbl_eu_80518E00 + 0x33);
        CRICRW_Strcat(buf2, 0x40, lbl_eu_80518E00 + 0x83);
        SJERR_CallErr(buf2);
    } else {
        self->err_func = (void (*)(void *, int))cbfunc;
        self->err_arg = cbarg;
    }
    SJCRS_Unlock();
}

/* --- SJUNI_Reset --- */
void SJUNI_Reset(void *self) {
    SJCRS_Lock();
    sjuni_Reset((SJUNI *)self);
    SJCRS_Unlock();
}

/* --- sjuni_Reset (internal) --- */
void sjuni_Reset(SJUNI *self) {
    if (self == NULL) {
        char buf[64];
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518E00 + 0xC);
        CRICRW_Strcat(buf, 0x40, lbl_eu_80518E00 + 0x8F);
        SJERR_CallErr(buf);
    } else if (self->valid == 0) {
        char buf[64];
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518E00 + 0x33);
        CRICRW_Strcat(buf, 0x40, lbl_eu_80518E00 + 0x9B);
        SJERR_CallErr(buf);
    } else {
        SJUNI_CHUNK *chunks = (SJUNI_CHUNK *)self->pool_mem;
        u32 i;
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

/* --- SJUNI_GetNumData --- */
int SJUNI_GetNumData(void *self, int mode) {
    int r;
    SJCRS_Lock();
    r = sjuni_GetNumData((SJUNI *)self, mode);
    SJCRS_Unlock();
    return r;
}

/* --- sjuni_GetNumData (internal) --- */
int sjuni_GetNumData(SJUNI *self, int mode) {
    if (self == NULL) {
        char buf[64];
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518E00 + 0xC);
        CRICRW_Strcat(buf, 0x40, lbl_eu_80518E00 + 0xA7);
        SJERR_CallErr(buf);
        return 0;
    }
    if (self->valid == 0) {
        char buf[64];
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518E00 + 0x33);
        CRICRW_Strcat(buf, 0x40, lbl_eu_80518E00 + 0xB3);
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

/* --- SJUNI_GetChunk --- */
int SJUNI_GetChunk(void *self, int mode, int size, SJ_CHUNK *out) {
    SJCRS_Lock();
    sjuni_GetChunk((SJUNI *)self, mode, size, out);
    SJCRS_Unlock();
}

/* --- sjuni_GetChunk (internal) --- */
int sjuni_GetChunk(SJUNI *self, int mode, int size, SJ_CHUNK *out) {
    if (self == NULL) {
        char buf[64];
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518E00 + 0xC);
        CRICRW_Strcat(buf, 0x40, lbl_eu_80518E00 + 0xBF);
        SJERR_CallErr(buf);
    } else if (self->valid == 0) {
        char buf[64];
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518E00 + 0x33);
        CRICRW_Strcat(buf, 0x40, lbl_eu_80518E00 + 0xCB);
        SJERR_CallErr(buf);
    } else if ((u32)mode > 3) {
        if (self->err_func) self->err_func(self->err_arg, -3);
        out->ptr = NULL;
        out->size = 0;
    } else {
        SJUNI_CHUNK *chunk = self->queue[mode];
        if (chunk == NULL) {
            out->ptr = NULL;
            out->size = 0;
        } else {
            SJ_CHUNK info;
            SJ_CHUNK remainder;
            info.size = chunk->size;
            info.ptr = chunk->ptr;
            if (info.size <= size) {
                out->ptr = chunk->ptr;
                out->size = chunk->size;
                self->queue[mode] = chunk->next;
                chunk->next = self->free_head;
                self->free_head = chunk;
            } else if (self->index == 1) {
                SJ_SplitChunk(&info, size, &info, &remainder);
                out->ptr = info.ptr;
                out->size = info.size;
                chunk->ptr = remainder.ptr;
                chunk->size = remainder.size;
            } else {
                out->ptr = NULL;
                out->size = 0;
            }
        }
    }
    return 0;
}

/* --- SJUNI_PutChunk --- */
int SJUNI_PutChunk(void *self, int mode, SJ_CHUNK *chunk) {
    SJCRS_Lock();
    sjuni_PutChunk((SJUNI *)self, mode, chunk);
    SJCRS_Unlock();
}

/* --- sjuni_PutChunk (internal) --- */
int sjuni_PutChunk(SJUNI *self, int mode, SJ_CHUNK *chunk) {
    if (self == NULL) {
        char buf[64];
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518E00 + 0xC);
        CRICRW_Strcat(buf, 0x40, lbl_eu_80518E00 + 0xD7);
        SJERR_CallErr(buf);
    } else if (self->valid == 0) {
        char buf[64];
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518E00 + 0x33);
        CRICRW_Strcat(buf, 0x40, lbl_eu_80518E00 + 0xE3);
        SJERR_CallErr(buf);
    } else if ((u32)mode > 3) {
        if (self->err_func) self->err_func(self->err_arg, -3);
    } else if (chunk->size > 0 && chunk->ptr != NULL) {
        SJUNI_CHUNK **tail = &self->queue[mode];
        SJUNI_CHUNK *prev = NULL;
        while (*tail != NULL) {
            prev = *tail;
            tail = &(*tail)->next;
        }
        if (self->index == 1 && prev != NULL) {
            if (prev->ptr + prev->size == chunk->ptr) {
                prev->size += chunk->size;
                return 0;
            }
        }
        {
            SJUNI_CHUNK *free_chunk = self->free_head;
            if (free_chunk == NULL) {
                if (self->err_func) self->err_func(self->err_arg, -3);
            } else {
                self->free_head = free_chunk->next;
                free_chunk->next = NULL;
                free_chunk->ptr = chunk->ptr;
                free_chunk->size = chunk->size;
                *tail = free_chunk;
            }
        }
    }
    return 0;
}

/* --- SJUNI_UngetChunk --- */
int SJUNI_UngetChunk(void *self, int mode, SJ_CHUNK *chunk) {
    SJCRS_Lock();
    sjuni_UngetChunk((SJUNI *)self, mode, chunk);
    SJCRS_Unlock();
}

/* --- sjuni_UngetChunk (internal) --- */
int sjuni_UngetChunk(SJUNI *self, int mode, SJ_CHUNK *chunk) {
    if (self == NULL) {
        char buf[64];
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518E00 + 0xC);
        CRICRW_Strcat(buf, 0x40, lbl_eu_80518E00 + 0xEF);
        SJERR_CallErr(buf);
    } else if (self->valid == 0) {
        char buf[64];
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518E00 + 0x33);
        CRICRW_Strcat(buf, 0x40, lbl_eu_80518E00 + 0xFB);
        SJERR_CallErr(buf);
    } else if ((u32)mode > 3) {
        if (self->err_func) self->err_func(self->err_arg, -3);
    } else if (chunk->size > 0 && chunk->ptr != NULL) {
        if (self->index == 1) {
            SJUNI_CHUNK *head = self->queue[mode];
            if (head != NULL && chunk->ptr + chunk->size == head->ptr) {
                head->ptr = chunk->ptr;
                head->size += chunk->size;
                return 0;
            }
        }
        {
            SJUNI_CHUNK *free_chunk = self->free_head;
            if (free_chunk == NULL) {
                if (self->err_func) self->err_func(self->err_arg, -3);
            } else {
                self->free_head = free_chunk->next;
                free_chunk->next = NULL;
                free_chunk->ptr = chunk->ptr;
                free_chunk->size = chunk->size;
                free_chunk->next = self->queue[mode];
                self->queue[mode] = free_chunk;
            }
        }
    }
    return 0;
}

/* --- SJUNI_IsGetChunk --- */
int SJUNI_IsGetChunk(void *self, int mode, int size, int *out) {
    int r;
    SJCRS_Lock();
    r = sjuni_IsGetChunk((SJUNI *)self, mode, size, out);
    SJCRS_Unlock();
    return r;
}

/* --- sjuni_IsGetChunk (internal) --- */
int sjuni_IsGetChunk(SJUNI *self, int mode, int size, int *out) {
    if (self == NULL) {
        char buf[64];
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518E00 + 0x107);
        CRICRW_Strcat(buf, 0x40, lbl_eu_80518E00 + 0xC);
        SJERR_CallErr(buf);
        return 0;
    }
    if (self->valid == 0) {
        char buf[64];
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518E00 + 0x113);
        CRICRW_Strcat(buf, 0x40, lbl_eu_80518E00 + 0x33);
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
            ck.ptr = head->ptr;
            ck.size = head->size;
            *out = ck.size;
            if (self->index == 1) {
                return (ck.size >= size) ? 1 : 0;
            } else {
                return (ck.size == size) ? 1 : 0;
            }
        }
    }
}

/* --- SJUNI_GetNumChunk --- */
int SJUNI_GetNumChunk(void *self_ptr, int mode) {
    SJUNI *self = (SJUNI *)self_ptr;
    int r;
    SJCRS_Lock();
    if (self == NULL) {
        char buf1[64];
        CRICRW_Strcpy(buf1, 0x40, lbl_eu_80518E00 + 0xC);
        CRICRW_Strcat(buf1, 0x40, lbl_eu_80518E00 + 0x11F);
        SJERR_CallErr(buf1);
        r = 0;
    } else if (self->valid == 0) {
        char buf2[64];
        CRICRW_Strcpy(buf2, 0x40, lbl_eu_80518E00 + 0x33);
        CRICRW_Strcat(buf2, 0x40, lbl_eu_80518E00 + 0x12B);
        SJERR_CallErr(buf2);
        r = 0;
    } else {
        int count = 0;
        SJUNI_CHUNK *cur = self->queue[mode];
        while (cur != NULL) {
            count++;
            cur = cur->next;
        }
        r = count;
    }
    SJCRS_Unlock();
    return r;
}
