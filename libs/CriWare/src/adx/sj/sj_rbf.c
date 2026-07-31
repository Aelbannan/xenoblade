// sj_rbf.c - SJ ring buffer management unit
// CriWare ADX SJ ring buffer allocator

#include <harness_catalog.h>

extern char lbl_eu_80518BC8[];

extern void *memset(void *, int, unsigned long);

extern u32 lbl_eu_805ED2D0;   /* init counter */
extern u8  lbl_eu_805ED2D8[]; /* work area (0x4800 bytes) */

extern char lbl_eu_80518BB8[]; /* uuid */
extern u8 lbl_eu_80565C30[];  /* vtable */

extern void SJCRS_Init(void);
extern void SJCRS_Finish(void);
extern void SJCRS_Lock(void);
extern void SJCRS_Unlock(void);
extern void SJERR_CallErr(const char *msg);
extern void CRICRW_Strcpy(char *dst, int maxlen, const char *src);
extern void CRICRW_Strcat(char *dst, int maxlen, const char *src);
extern void SJRBF_Error(void);

typedef struct {
    void *vtable;      /* +0x00 */
    u32 valid;         /* +0x04 */
    void *uuid;        /* +0x08 */
    u32 put_avail;     /* +0x0C */
    u32 get_avail;     /* +0x10 */
    u32 get_pos;       /* +0x14 */
    u32 put_pos;       /* +0x18 */
    u8 *pool_mem;      /* +0x1C */
    u32 buf_size;      /* +0x20 */
    u32 xtr_size;      /* +0x24 */
    u32 flow_cnt[4];   /* +0x28 */
    void (*err_func)(void *, int); /* +0x38 */
    void *err_arg;     /* +0x3C */
    void (*put_func)(void *, void *); /* +0x40 */
    void *put_arg;     /* +0x44 */
} SJRBF; /* size 0x48 */

typedef struct {
    unsigned char *ptr;
    int size;
} SJ_CHUNK;

/* --- Internal forward declarations --- */
void *sjrbf_Create(void *pool_mem, u32 buf_size, u32 xtr_size);
void  sjrbf_Reset(SJRBF *self);
int   sjrbf_GetChunk(SJRBF *self, int mode, int size, SJ_CHUNK *out);
int   sjrbf_PutChunk(SJRBF *self, int mode, SJ_CHUNK *chunk);
int   sjrbf_UngetChunk(SJRBF *self, int mode, SJ_CHUNK *chunk);
int   sjrbf_IsGetChunk(SJRBF *self, int mode, int size, int *out);

/* --- SJRBF_Error --- */
void SJRBF_Error(void) {
    SJERR_CallErr(lbl_eu_80518BC8);
}

/* --- SJRBF_Init --- */
void SJRBF_Init(void) {
    SJCRS_Init();
    SJCRS_Lock();
    if (lbl_eu_805ED2D0 == 0) {
        memset(lbl_eu_805ED2D8, 0, 0x4800);
    }
    lbl_eu_805ED2D0++;
    SJCRS_Unlock();
}

/* --- SJRBF_Finish --- */
void SJRBF_Finish(void) {
    SJCRS_Lock();
    if (--lbl_eu_805ED2D0 == 0) {
        memset(lbl_eu_805ED2D8, 0, 0x4800);
    }
    SJCRS_Unlock();
    SJCRS_Finish();
}

/* --- SJRBF_Create --- */
void *SJRBF_Create(void *pool_mem, u32 buf_size, u32 xtr_size) {
    void *r;
    SJCRS_Lock();
    r = sjrbf_Create(pool_mem, buf_size, xtr_size);
    SJCRS_Unlock();
    return r;
}

/* --- sjrbf_Create (internal) --- */
void *sjrbf_Create(void *pool_mem, u32 buf_size, u32 xtr_size) {
    SJRBF *instances = (SJRBF *)lbl_eu_805ED2D8;
    int i;

    for (i = 0; i < 0x100; i++) {
        if (instances[i].valid == 0) break;
    }
    if (i == 0x100) return NULL;

    SJRBF *self = &instances[i];
    self->valid = 1;
    self->vtable = lbl_eu_80565C30;
    self->pool_mem = (u8 *)pool_mem;
    self->buf_size = buf_size;
    self->xtr_size = xtr_size;
    self->uuid = lbl_eu_80518BB8;
    self->err_func = (void (*)(void *, int))SJRBF_Error;
    self->err_arg = self;
    self->put_func = NULL;
    self->put_arg = NULL;
    sjrbf_Reset(self);
    return self;
}

/* --- SJRBF_Destroy --- */
void SJRBF_Destroy(void *self_ptr) {
    SJRBF *self = (SJRBF *)self_ptr;
    char buf2[64];
    char buf1[64];
    SJCRS_Lock();
    if (self == NULL) {
        CRICRW_Strcpy(buf1, 0x40, lbl_eu_80518BC8 + 0x6B);
        CRICRW_Strcat(buf1, 0x40, lbl_eu_80518BC8 + 0x0C);
        SJERR_CallErr(buf1);
    } else if (self->valid == 0) {
        CRICRW_Strcpy(buf2, 0x40, lbl_eu_80518BC8 + 0x97);
        CRICRW_Strcat(buf2, 0x40, lbl_eu_80518BC8 + 0x77);
        SJERR_CallErr(buf2);
    } else {
        memset(self, 0, 0x48);
        self->valid = 0;
    }
    SJCRS_Unlock();
}

/* --- SJRBF_GetUuid --- */
void *SJRBF_GetUuid(void *self_ptr) {
    SJRBF *self = (SJRBF *)self_ptr;
    char buf2[64];
    char buf1[64];
    void *r;
    SJCRS_Lock();
    if (self == NULL) {
        CRICRW_Strcpy(buf1, 0x40, lbl_eu_80518BC8 + 0xA3);
        CRICRW_Strcat(buf1, 0x40, lbl_eu_80518BC8 + 0x0C);
        SJERR_CallErr(buf1);
        r = NULL;
    } else if (self->valid == 0) {
        CRICRW_Strcpy(buf2, 0x40, lbl_eu_80518BC8 + 0xAF);
        CRICRW_Strcat(buf2, 0x40, lbl_eu_80518BC8 + 0x77);
        SJERR_CallErr(buf2);
        r = NULL;
    } else {
        r = self->uuid;
    }
    SJCRS_Unlock();
    return r;
}

/* --- SJRBF_EntryErrFunc --- */
void SJRBF_EntryErrFunc(void *self_ptr, void *cbfunc, void *cbarg) {
    SJRBF *self = (SJRBF *)self_ptr;
    char buf2[64];
    char buf1[64];
    SJCRS_Lock();
    if (self == NULL) {
        CRICRW_Strcpy(buf1, 0x40, lbl_eu_80518BC8 + 0xBB);
        CRICRW_Strcat(buf1, 0x40, lbl_eu_80518BC8 + 0x0C);
        SJERR_CallErr(buf1);
    } else if (self->valid == 0) {
        CRICRW_Strcpy(buf2, 0x40, lbl_eu_80518BC8 + 0xC7);
        CRICRW_Strcat(buf2, 0x40, lbl_eu_80518BC8 + 0x77);
        SJERR_CallErr(buf2);
    } else {
        self->err_func = (void (*)(void *, int))cbfunc;
        self->err_arg = cbarg;
    }
    SJCRS_Unlock();
}

/* --- SJRBF_Reset --- */
void SJRBF_Reset(void *self) {
    SJCRS_Lock();
    sjrbf_Reset((SJRBF *)self);
    SJCRS_Unlock();
}

/* --- sjrbf_Reset (internal) --- */
void sjrbf_Reset(SJRBF *self) {
    if (self == NULL) {
        char buf[64];
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518BC8 + 0xEB);
        CRICRW_Strcat(buf, 0x40, lbl_eu_80518BC8 + 0x0C);
        SJERR_CallErr(buf);
    } else if (self->valid == 0) {
        char buf[64];
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518BC8 + 0xF7);
        CRICRW_Strcat(buf, 0x40, lbl_eu_80518BC8 + 0x77);
        SJERR_CallErr(buf);
    } else {
        self->put_avail = 0;
        self->get_avail = self->buf_size;
        self->get_pos = 0;
        self->put_pos = 0;
        self->flow_cnt[0] = 0;
        self->flow_cnt[1] = 0;
        self->flow_cnt[2] = 0;
        self->flow_cnt[3] = 0;
    }
}

/* --- fn_80397A74 --- */
int fn_80397A74(void *self_ptr, int mode) {
    SJRBF *self = (SJRBF *)self_ptr;
    int r;
    SJCRS_Lock();
    if (self == NULL) {
        char buf1[64];
        CRICRW_Strcpy(buf1, 0x40, lbl_eu_80518BC8 + 0x103);
        CRICRW_Strcat(buf1, 0x40, lbl_eu_80518BC8 + 0x0C);
        SJERR_CallErr(buf1);
        r = 0;
    } else if (self->valid == 0) {
        char buf2[64];
        CRICRW_Strcpy(buf2, 0x40, lbl_eu_80518BC8 + 0x10F);
        CRICRW_Strcat(buf2, 0x40, lbl_eu_80518BC8 + 0x77);
        SJERR_CallErr(buf2);
        r = 0;
    } else if (mode == 1) {
        r = self->put_avail;
    } else if (mode == 0) {
        r = self->get_avail;
    } else {
        if (self->err_func) self->err_func(self->err_arg, -3);
        r = 0;
    }
    SJCRS_Unlock();
    return r;
}

/* --- SJRBF_GetChunk --- */
int SJRBF_GetChunk(void *self, int mode, int size, SJ_CHUNK *out) {
    SJCRS_Lock();
    sjrbf_GetChunk((SJRBF *)self, mode, size, out);
    SJCRS_Unlock();
}

/* --- sjrbf_GetChunk (internal) --- */
int sjrbf_GetChunk(SJRBF *self, int mode, int size, SJ_CHUNK *out) {
    if (self == NULL) {
        char buf[64];
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518BC8 + 0x11B);
        CRICRW_Strcat(buf, 0x40, lbl_eu_80518BC8 + 0x0C);
        SJERR_CallErr(buf);
    } else if (self->valid == 0) {
        char buf[64];
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518BC8 + 0x127);
        CRICRW_Strcat(buf, 0x40, lbl_eu_80518BC8 + 0x77);
        SJERR_CallErr(buf);
    } else if (self->buf_size == 0) {
        char buf[64];
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518BC8 + 0x14B);
        CRICRW_Strcat(buf, 0x40, lbl_eu_80518BC8 + 0x133);
        SJERR_CallErr(buf);
    } else if (mode == 0) {
        int avail = self->xtr_size + self->buf_size - self->get_pos;
        if ((int)self->get_avail < avail) avail = self->get_avail;
        out->size = avail;
        if (avail < size) size = avail;
        out->size = size;
        out->ptr = self->pool_mem + self->get_pos;
        {
            int new_pos = self->get_pos + size;
            self->get_pos = new_pos % self->buf_size;
        }
        self->get_avail -= out->size;
        self->flow_cnt[0] += out->size;
    } else if (mode == 1) {
        int avail = self->xtr_size + self->buf_size - self->put_pos;
        if ((int)self->put_avail < avail) avail = self->put_avail;
        out->size = avail;
        if (avail < size) size = avail;
        out->size = size;
        out->ptr = self->pool_mem + self->put_pos;
        {
            int new_pos = self->put_pos + size;
            self->put_pos = new_pos % self->buf_size;
        }
        self->put_avail -= out->size;
        self->flow_cnt[2] += out->size;
    } else {
        out->size = 0;
        out->ptr = NULL;
        if (self->err_func) self->err_func(self->err_arg, -3);
    }
    return 0;
}

/* --- SJRBF_PutChunk --- */
int SJRBF_PutChunk(void *self, int mode, SJ_CHUNK *chunk) {
    SJCRS_Lock();
    sjrbf_PutChunk((SJRBF *)self, mode, chunk);
    SJCRS_Unlock();
}

/* --- sjrbf_PutChunk (internal) --- */
int sjrbf_PutChunk(SJRBF *self, int mode, SJ_CHUNK *chunk) {
    if (self == NULL) {
        char buf[64];
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518BC8 + 0x157);
        CRICRW_Strcat(buf, 0x40, lbl_eu_80518BC8 + 0x0C);
        SJERR_CallErr(buf);
    } else if (self->valid == 0) {
        char buf[64];
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518BC8 + 0x163);
        CRICRW_Strcat(buf, 0x40, lbl_eu_80518BC8 + 0x77);
        SJERR_CallErr(buf);
    } else if (chunk->size > 0 && chunk->ptr != NULL) {
        if (mode == 1) {
            if (self->put_func) self->put_func(self->put_arg, chunk);
            {
                int offset = chunk->ptr - self->pool_mem;
                if (offset < (int)self->xtr_size) {
                    int copy_len = self->xtr_size - offset;
                    if (chunk->size < copy_len) copy_len = chunk->size;
                    memcpy(chunk->ptr + self->buf_size, chunk->ptr, copy_len);
                }
            }
            {
                int end_offset = chunk->size + (chunk->ptr - self->pool_mem);
                if (end_offset > (int)self->buf_size) {
                    int copy_len = end_offset - self->buf_size;
                    if (chunk->size < copy_len) copy_len = chunk->size;
                    memcpy(self->pool_mem, chunk->ptr + (end_offset - copy_len), copy_len);
                }
            }
            self->put_avail += chunk->size;
            self->flow_cnt[3] += chunk->size;
        } else if (mode == 0) {
            self->get_avail += chunk->size;
            self->flow_cnt[1] += chunk->size;
        } else {
            chunk->size = 0;
            chunk->ptr = NULL;
            if (self->err_func) self->err_func(self->err_arg, -3);
        }
    }
    return 0;
}

/* --- SJRBF_UngetChunk --- */
int SJRBF_UngetChunk(void *self, int mode, SJ_CHUNK *chunk) {
    SJCRS_Lock();
    sjrbf_UngetChunk((SJRBF *)self, mode, chunk);
    SJCRS_Unlock();
}

/* --- sjrbf_UngetChunk (internal) --- */
int sjrbf_UngetChunk(SJRBF *self, int mode, SJ_CHUNK *chunk) {
    if (self == NULL) {
        char buf[64];
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518BC8 + 0x16F);
        CRICRW_Strcat(buf, 0x40, lbl_eu_80518BC8 + 0x0C);
        SJERR_CallErr(buf);
    } else if (self->valid == 0) {
        char buf[64];
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518BC8 + 0x17B);
        CRICRW_Strcat(buf, 0x40, lbl_eu_80518BC8 + 0x77);
        SJERR_CallErr(buf);
    } else if (self->buf_size == 0) {
        char buf[64];
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518BC8 + 0x187);
        CRICRW_Strcat(buf, 0x40, lbl_eu_80518BC8 + 0x133);
        SJERR_CallErr(buf);
    } else if (chunk->size > 0 && chunk->ptr != NULL) {
        if (mode == 0) {
            int new_pos = (self->get_pos + self->buf_size - chunk->size) % self->buf_size;
            int expected = ((int)(self->pool_mem - chunk->ptr)) % self->buf_size;
            if (new_pos == expected) {
                self->get_pos = new_pos;
                self->get_avail += chunk->size;
            } else {
                if (self->err_func) self->err_func(self->err_arg, -3);
            }
            self->flow_cnt[0] -= chunk->size;
        } else if (mode == 1) {
            int new_pos = (self->put_pos + self->buf_size - chunk->size) % self->buf_size;
            int expected = ((int)(self->pool_mem - chunk->ptr)) % self->buf_size;
            if (new_pos == expected) {
                self->put_pos = new_pos;
                self->put_avail += chunk->size;
            } else {
                if (self->err_func) self->err_func(self->err_arg, -3);
            }
            self->flow_cnt[2] -= chunk->size;
        } else {
            chunk->size = 0;
            chunk->ptr = NULL;
            if (self->err_func) self->err_func(self->err_arg, -3);
        }
    }
    return 0;
}

/* --- SJRBF_IsGetChunk --- */
int SJRBF_IsGetChunk(void *self, int mode, int size, int *out) {
    int r;
    SJCRS_Lock();
    r = sjrbf_IsGetChunk((SJRBF *)self, mode, size, out);
    SJCRS_Unlock();
    return r;
}

/* --- sjrbf_IsGetChunk (internal) --- */
int sjrbf_IsGetChunk(SJRBF *self, int mode, int size, int *out) {
    int result;
    if (self == NULL) {
        char buf[64];
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518BC8 + 0x193);
        CRICRW_Strcat(buf, 0x40, lbl_eu_80518BC8 + 0x0C);
        SJERR_CallErr(buf);
        return 0;
    }
    if (self->valid == 0) {
        char buf[64];
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518BC8 + 0x19F);
        CRICRW_Strcat(buf, 0x40, lbl_eu_80518BC8 + 0x77);
        SJERR_CallErr(buf);
        return 0;
    }
    if (mode == 0) {
        int avail = self->xtr_size + self->buf_size - self->get_pos;
        if ((int)self->get_avail < avail) avail = self->get_avail;
        result = size;
        if (avail < size) result = avail;
    } else if (mode == 1) {
        int avail = self->xtr_size + self->buf_size - self->put_pos;
        if ((int)self->put_avail < avail) avail = self->put_avail;
        result = size;
        if (avail < size) result = avail;
    } else {
        result = 0;
        if (self->err_func) self->err_func(self->err_arg, -3);
    }
    *out = result;
    return (result == size) ? 1 : 0;
}

/* --- SJRBF_GetBufPtr --- */
void *SJRBF_GetBufPtr(void *self_ptr) {
    SJRBF *self = (SJRBF *)self_ptr;
    char buf2[64];
    char buf1[64];
    void *r;
    SJCRS_Lock();
    if (self == NULL) {
        CRICRW_Strcpy(buf1, 0x40, lbl_eu_80518BC8 + 0x1AB);
        CRICRW_Strcat(buf1, 0x40, lbl_eu_80518BC8 + 0x0C);
        SJERR_CallErr(buf1);
        r = NULL;
    } else if (self->valid == 0) {
        CRICRW_Strcpy(buf2, 0x40, lbl_eu_80518BC8 + 0x1B7);
        CRICRW_Strcat(buf2, 0x40, lbl_eu_80518BC8 + 0x77);
        SJERR_CallErr(buf2);
        r = NULL;
    } else {
        r = self->pool_mem;
    }
    SJCRS_Unlock();
    return r;
}

/* --- SJRBF_GetBufSize --- */
int SJRBF_GetBufSize(void *self_ptr) {
    SJRBF *self = (SJRBF *)self_ptr;
    char buf2[64];
    char buf1[64];
    int r;
    SJCRS_Lock();
    if (self == NULL) {
        CRICRW_Strcpy(buf1, 0x40, lbl_eu_80518BC8 + 0x1C3);
        CRICRW_Strcat(buf1, 0x40, lbl_eu_80518BC8 + 0x0C);
        SJERR_CallErr(buf1);
        r = 0;
    } else if (self->valid == 0) {
        CRICRW_Strcpy(buf2, 0x40, lbl_eu_80518BC8 + 0x1CF);
        CRICRW_Strcat(buf2, 0x40, lbl_eu_80518BC8 + 0x77);
        SJERR_CallErr(buf2);
        r = 0;
    } else {
        r = self->buf_size;
    }
    SJCRS_Unlock();
    return r;
}

/* --- SJRBF_GetXtrSize --- */
int SJRBF_GetXtrSize(void *self_ptr) {
    SJRBF *self = (SJRBF *)self_ptr;
    char buf2[64];
    char buf1[64];
    int r;
    SJCRS_Lock();
    if (self == NULL) {
        CRICRW_Strcpy(buf1, 0x40, lbl_eu_80518BC8 + 0x1DB);
        CRICRW_Strcat(buf1, 0x40, lbl_eu_80518BC8 + 0x0C);
        SJERR_CallErr(buf1);
        r = 0;
    } else if (self->valid == 0) {
        CRICRW_Strcpy(buf2, 0x40, lbl_eu_80518BC8 + 0x1E7);
        CRICRW_Strcat(buf2, 0x40, lbl_eu_80518BC8 + 0x77);
        SJERR_CallErr(buf2);
        r = 0;
    } else {
        r = self->xtr_size;
    }
    SJCRS_Unlock();
    return r;
}

/* --- SJRBF_GetFlowCnt --- */
int SJRBF_GetFlowCnt(void *self_ptr, int a, int b) {
    SJRBF *self = (SJRBF *)self_ptr;
    char buf2[64];
    char buf1[64];
    int r;
    SJCRS_Lock();
    if (self == NULL) {
        CRICRW_Strcpy(buf1, 0x40, lbl_eu_80518BC8 + 0x20B);
        CRICRW_Strcat(buf1, 0x40, lbl_eu_80518BC8 + 0x0C);
        SJERR_CallErr(buf1);
        r = 0;
    } else if (self->valid == 0) {
        CRICRW_Strcpy(buf2, 0x40, lbl_eu_80518BC8 + 0x217);
        CRICRW_Strcat(buf2, 0x40, lbl_eu_80518BC8 + 0x77);
        SJERR_CallErr(buf2);
        r = 0;
    } else {
        r = self->flow_cnt[a * 2 + b];
    }
    SJCRS_Unlock();
    return r;
}
