// sj_mem.c - SJ memory management unit
// CriWare ADX SJ linear memory allocator

#include <harness_catalog.h>

extern char lbl_eu_80518A68[];
extern u32 lbl_eu_805ECE48;  /* init counter */
extern u8  lbl_eu_805ECE50[]; /* work area (0x480 bytes) */

extern char lbl_eu_80518A58[]; /* uuid */
extern u8 lbl_eu_80565C00[];  /* vtable */

extern void SJCRS_Init(void);
extern void SJCRS_Finish(void);
extern void SJCRS_Lock(void);
extern void SJCRS_Unlock(void);
extern void SJERR_CallErr(const char *msg);
extern void CRICRW_Strcpy(char *dst, int maxlen, const char *src);
extern void CRICRW_Strcat(char *dst, int maxlen, const char *src);
extern void SJMEM_Error(void);

typedef struct {
    void *vtable;      /* +0x00 */
    u32 valid;         /* +0x04 */
    void *uuid;        /* +0x08 */
    u32 avail;         /* +0x0C */
    u32 offset;        /* +0x10 */
    u8 *pool_mem;      /* +0x14 */
    u32 buf_size;      /* +0x18 */
    void (*err_func)(void *, int); /* +0x1C */
    void *err_arg;     /* +0x20 */
} SJMEM; /* size 0x24 */

typedef struct {
    unsigned char *ptr;
    int size;
} SJ_CHUNK;

/* --- Internal functions (forward declarations) --- */
void *sjmem_Create(void *pool_mem, u32 flags);
int   sjmem_GetChunk(SJMEM *self, int mode, int size, SJ_CHUNK *out);
int   sjmem_PutChunk(SJMEM *self, int mode, SJ_CHUNK *chunk);
int   sjmem_UngetChunk(SJMEM *self, int mode, SJ_CHUNK *chunk);
int   sjmem_IsGetChunk(SJMEM *self, int mode, int size, int *out);

/* --- SJMEM_Error --- */
void SJMEM_Error(void) {
    SJERR_CallErr(lbl_eu_80518A68);
}

/* --- SJMEM_Init --- */
void SJMEM_Init(void) {
    SJCRS_Init();
    SJCRS_Lock();
    if (lbl_eu_805ECE48 == 0) {
        __builtin_memset(lbl_eu_805ECE50, 0, 0x480);
    }
    lbl_eu_805ECE48++;
    SJCRS_Unlock();
}

/* --- SJMEM_Finish --- */
void SJMEM_Finish(void) {
    SJCRS_Lock();
    if (--lbl_eu_805ECE48 == 0) {
        __builtin_memset(lbl_eu_805ECE50, 0, 0x480);
    }
    SJCRS_Unlock();
    SJCRS_Finish();
}

/* --- SJMEM_Create --- */
void *SJMEM_Create(void *pool_mem, u32 flags) {
    void *r;
    SJCRS_Lock();
    r = sjmem_Create(pool_mem, flags);
    SJCRS_Unlock();
    return r;
}

/* --- sjmem_Create (internal, 0x1B4) --- */
void *sjmem_Create(void *pool_mem, u32 flags) {
    SJMEM *instances = (SJMEM *)lbl_eu_805ECE50;
    int i;

    for (i = 0; i < 0x20; i++) {
        if (instances[i].valid == 0) break;
    }
    if (i == 0x20) return NULL;

    SJMEM *self = &instances[i];
    self->valid = 1;
    self->vtable = lbl_eu_80565C00;
    self->pool_mem = (u8 *)pool_mem;
    self->buf_size = flags;
    self->uuid = lbl_eu_80518A58;
    self->err_func = (void (*)(void *, int))SJMEM_Error;
    self->err_arg = self;

    if (pool_mem == NULL) {
        char buf[64];
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518A68 + 0x0C);
        CRICRW_Strcat(buf, 0x40, lbl_eu_80518A68 + 0x27);
        SJERR_CallErr(buf);
    } else if (self->valid == 0) {
        char buf[64];
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518A68 + 0x33);
        CRICRW_Strcat(buf, 0x40, lbl_eu_80518A68 + 0x53);
        SJERR_CallErr(buf);
    } else {
        self->avail = self->buf_size;
        self->offset = 0;
    }
    return self;
}

/* --- SJMEM_Destroy --- */
void SJMEM_Destroy(void *self_ptr) {
    SJMEM *self = (SJMEM *)self_ptr;
    SJCRS_Lock();
    if (self == NULL) {
        char buf1[64];
        CRICRW_Strcpy(buf1, 0x40, lbl_eu_80518A68 + 0x0C);
        CRICRW_Strcat(buf1, 0x40, lbl_eu_80518A68 + 0x5F);
        SJERR_CallErr(buf1);
    } else if (self->valid == 0) {
        char buf2[64];
        CRICRW_Strcpy(buf2, 0x40, lbl_eu_80518A68 + 0x33);
        CRICRW_Strcat(buf2, 0x40, lbl_eu_80518A68 + 0x6B);
        SJERR_CallErr(buf2);
    } else {
        __builtin_memset(self, 0, 0x24);
        self->valid = 0;
    }
    SJCRS_Unlock();
}

/* --- SJMEM_GetUuid --- */
void *SJMEM_GetUuid(void *self_ptr) {
    SJMEM *self = (SJMEM *)self_ptr;
    void *r;
    SJCRS_Lock();
    if (self == NULL) {
        char buf1[64];
        CRICRW_Strcpy(buf1, 0x40, lbl_eu_80518A68 + 0x0C);
        CRICRW_Strcat(buf1, 0x40, lbl_eu_80518A68 + 0x77);
        SJERR_CallErr(buf1);
        r = NULL;
    } else if (self->valid == 0) {
        char buf2[64];
        CRICRW_Strcpy(buf2, 0x40, lbl_eu_80518A68 + 0x33);
        CRICRW_Strcat(buf2, 0x40, lbl_eu_80518A68 + 0x83);
        SJERR_CallErr(buf2);
        r = NULL;
    } else {
        r = self->uuid;
    }
    SJCRS_Unlock();
    return r;
}

/* --- SJMEM_EntryErrFunc --- */
void SJMEM_EntryErrFunc(void *self_ptr, void *cbfunc, void *cbarg) {
    SJMEM *self = (SJMEM *)self_ptr;
    SJCRS_Lock();
    if (self == NULL) {
        char buf1[64];
        CRICRW_Strcpy(buf1, 0x40, lbl_eu_80518A68 + 0x0C);
        CRICRW_Strcat(buf1, 0x40, lbl_eu_80518A68 + 0x8F);
        SJERR_CallErr(buf1);
    } else if (self->valid == 0) {
        char buf2[64];
        CRICRW_Strcpy(buf2, 0x40, lbl_eu_80518A68 + 0x33);
        CRICRW_Strcat(buf2, 0x40, lbl_eu_80518A68 + 0x9B);
        SJERR_CallErr(buf2);
    } else {
        self->err_func = (void (*)(void *, int))cbfunc;
        self->err_arg = cbarg;
    }
    SJCRS_Unlock();
}

/* --- SJMEM_Reset --- */
void SJMEM_Reset(void *self_ptr) {
    SJMEM *self = (SJMEM *)self_ptr;
    SJCRS_Lock();
    if (self == NULL) {
        char buf1[64];
        CRICRW_Strcpy(buf1, 0x40, lbl_eu_80518A68 + 0x0C);
        CRICRW_Strcat(buf1, 0x40, lbl_eu_80518A68 + 0x27);
        SJERR_CallErr(buf1);
    } else if (self->valid == 0) {
        char buf2[64];
        CRICRW_Strcpy(buf2, 0x40, lbl_eu_80518A68 + 0x33);
        CRICRW_Strcat(buf2, 0x40, lbl_eu_80518A68 + 0x53);
        SJERR_CallErr(buf2);
    } else {
        self->avail = self->buf_size;
        self->offset = 0;
    }
    SJCRS_Unlock();
}

/* --- SJMEM_GetNumData --- */
int SJMEM_GetNumData(void *self_ptr, int mode) {
    SJMEM *self = (SJMEM *)self_ptr;
    int r;
    SJCRS_Lock();
    if (self == NULL) {
        char buf1[64];
        CRICRW_Strcpy(buf1, 0x40, lbl_eu_80518A68 + 0x0C);
        CRICRW_Strcat(buf1, 0x40, lbl_eu_80518A68 + 0xA7);
        SJERR_CallErr(buf1);
        r = 0;
    } else if (self->valid == 0) {
        char buf2[64];
        CRICRW_Strcpy(buf2, 0x40, lbl_eu_80518A68 + 0x33);
        CRICRW_Strcat(buf2, 0x40, lbl_eu_80518A68 + 0xB3);
        SJERR_CallErr(buf2);
        r = 0;
    } else if (mode == 1) {
        r = self->avail;
    } else if (mode == 0) {
        r = 0;
    } else {
        if (self->err_func) self->err_func(self->err_arg, -3);
        r = 0;
    }
    SJCRS_Unlock();
    return r;
}

/* --- SJMEM_GetChunk --- */
int SJMEM_GetChunk(void *self, int mode, int size, SJ_CHUNK *out) {
    SJCRS_Lock();
    sjmem_GetChunk((SJMEM *)self, mode, size, out);
    SJCRS_Unlock();
}

/* --- sjmem_GetChunk (internal, 0x134) --- */
int sjmem_GetChunk(SJMEM *self, int mode, int size, SJ_CHUNK *out) {
    if (self == NULL) {
        char buf[64];
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518A68 + 0x0C);
        CRICRW_Strcat(buf, 0x40, lbl_eu_80518A68 + 0xBF);
        SJERR_CallErr(buf);
    } else if (self->valid == 0) {
        char buf[64];
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518A68 + 0x33);
        CRICRW_Strcat(buf, 0x40, lbl_eu_80518A68 + 0xCB);
        SJERR_CallErr(buf);
    } else if (mode == 0) {
        out->size = 0;
        out->ptr = NULL;
    } else if (mode == 1) {
        if ((int)self->avail < size) size = self->avail;
        out->size = size;
        out->ptr = self->pool_mem + self->offset;
        self->offset += size;
        self->avail -= out->size;
    } else {
        out->size = 0;
        out->ptr = NULL;
        if (self->err_func) self->err_func(self->err_arg, -3);
    }
    return 0;
}

/* --- SJMEM_PutChunk --- */
int SJMEM_PutChunk(void *self, int mode, SJ_CHUNK *chunk) {
    SJCRS_Lock();
    sjmem_PutChunk((SJMEM *)self, mode, chunk);
    SJCRS_Unlock();
}

/* --- sjmem_PutChunk (internal, 0xFC) --- */
int sjmem_PutChunk(SJMEM *self, int mode, SJ_CHUNK *chunk) {
    if (self == NULL) {
        char buf[64];
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518A68 + 0x0C);
        CRICRW_Strcat(buf, 0x40, lbl_eu_80518A68 + 0xD7);
        SJERR_CallErr(buf);
    } else if (self->valid == 0) {
        char buf[64];
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518A68 + 0x33);
        CRICRW_Strcat(buf, 0x40, lbl_eu_80518A68 + 0xE3);
        SJERR_CallErr(buf);
    } else if (chunk->size > 0 && chunk->ptr != NULL) {
        if (mode == 0) {
            /* no-op */
        } else if (mode == 1) {
            /* no-op */
        } else {
            chunk->size = 0;
            chunk->ptr = NULL;
            if (self->err_func) self->err_func(self->err_arg, -3);
        }
    }
    return 0;
}

/* --- SJMEM_UngetChunk --- */
int SJMEM_UngetChunk(void *self, int mode, SJ_CHUNK *chunk) {
    SJCRS_Lock();
    sjmem_UngetChunk((SJMEM *)self, mode, chunk);
    SJCRS_Unlock();
}

/* --- sjmem_UngetChunk (internal, 0x18C) --- */
int sjmem_UngetChunk(SJMEM *self, int mode, SJ_CHUNK *chunk) {
    if (self == NULL) {
        char buf[64];
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518A68 + 0x0C);
        CRICRW_Strcat(buf, 0x40, lbl_eu_80518A68 + 0xEF);
        SJERR_CallErr(buf);
    } else if (self->valid == 0) {
        char buf[64];
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518A68 + 0x33);
        CRICRW_Strcat(buf, 0x40, lbl_eu_80518A68 + 0xFB);
        SJERR_CallErr(buf);
    } else if (chunk->size > 0 && chunk->ptr != NULL) {
        if (mode == 0) {
            if (self->err_func) self->err_func(self->err_arg, -3);
        } else if (mode == 1) {
            int new_offset = self->offset - chunk->size;
            if (new_offset < 0) new_offset = 0;
            self->offset = new_offset;
            {
                int new_avail = self->avail + chunk->size;
                if (new_avail > (int)self->buf_size) new_avail = self->buf_size;
                self->avail = new_avail;
            }
            if (new_offset != (int)(chunk->ptr - self->pool_mem)) {
                if (self->err_func) self->err_func(self->err_arg, -3);
            }
        } else {
            chunk->size = 0;
            chunk->ptr = NULL;
            if (self->err_func) self->err_func(self->err_arg, -3);
        }
    }
    return 0;
}

/* --- SJMEM_IsGetChunk --- */
int SJMEM_IsGetChunk(void *self, int mode, int size, int *out) {
    int r;
    SJCRS_Lock();
    r = sjmem_IsGetChunk((SJMEM *)self, mode, size, out);
    SJCRS_Unlock();
    return r;
}

/* --- sjmem_IsGetChunk (internal, 0x128) --- */
int sjmem_IsGetChunk(SJMEM *self, int mode, int size, int *out) {
    int result;
    if (self == NULL) {
        char buf[64];
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518A68 + 0x0C);
        CRICRW_Strcat(buf, 0x40, lbl_eu_80518A68 + 0x107);
        SJERR_CallErr(buf);
        return 0;
    }
    if (self->valid == 0) {
        char buf[64];
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518A68 + 0x33);
        CRICRW_Strcat(buf, 0x40, lbl_eu_80518A68 + 0x113);
        SJERR_CallErr(buf);
        return 0;
    }
    if (mode == 0) {
        result = 0;
    } else if (mode == 1) {
        result = size;
        if ((int)self->avail < size) result = self->avail;
    } else {
        result = 0;
        if (self->err_func) self->err_func(self->err_arg, -3);
    }
    *out = result;
    return (result == size) ? 1 : 0;
}

/* --- SJMEM_GetBufSize --- */
int SJMEM_GetBufSize(void *self_ptr) {
    SJMEM *self = (SJMEM *)self_ptr;
    int r;
    SJCRS_Lock();
    if (self == NULL) {
        char buf1[64];
        CRICRW_Strcpy(buf1, 0x40, lbl_eu_80518A68 + 0x0C);
        CRICRW_Strcat(buf1, 0x40, lbl_eu_80518A68 + 0x137);
        SJERR_CallErr(buf1);
        r = 0;
    } else if (self->valid == 0) {
        char buf2[64];
        CRICRW_Strcpy(buf2, 0x40, lbl_eu_80518A68 + 0x33);
        CRICRW_Strcat(buf2, 0x40, lbl_eu_80518A68 + 0x143);
        SJERR_CallErr(buf2);
        r = 0;
    } else {
        r = self->buf_size;
    }
    SJCRS_Unlock();
    return r;
}
