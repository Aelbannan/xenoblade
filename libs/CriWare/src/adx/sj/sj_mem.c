#include <harness_catalog.h>

__declspec(section ".rodata") __attribute__((aligned(8))) char lbl_eu_80518A58[0x10] = {
    0xDD, 0x9E, 0xEE, 0x41, 0x16, 0x79, 0x11, 0xD2, 0x93, 0x6C, 0x00, 0x60, 0x08, 0x94, 0x48, 0xBC
};

__declspec(section ".rodata") __attribute__((aligned(8))) char lbl_eu_80518A68[0x150] = {
    0x53, 0x4A, 0x4D, 0x45, 0x4D, 0x20, 0x45, 0x72, 0x72, 0x6F, 0x72, 0x00, 0x20, 0x3A, 0x20, 0x4E, 0x55, 0x4C, 0x4C, 0x20, 0x70, 0x6F, 0x69, 0x6E, 0x74, 0x65, 0x72, 0x20, 0x69, 0x73, 0x20, 0x70, 0x61, 0x73, 0x73, 0x65, 0x64, 0x2E, 0x00, 0x45, 0x32, 0x30, 0x30, 0x34, 0x30, 0x39, 0x30, 0x32, 0x33, 0x37, 0x00, 0x20, 0x3A, 0x20, 0x53, 0x70, 0x65, 0x63, 0x69, 0x66, 0x69, 0x65, 0x64, 0x20, 0x68, 0x61, 0x6E, 0x64, 0x6C, 0x65, 0x20, 0x69, 0x73, 0x20, 0x69, 0x6E, 0x76, 0x61, 0x6C, 0x69, 0x64, 0x2E, 0x00, 0x45, 0x32, 0x30, 0x30, 0x34, 0x30, 0x39, 0x30, 0x32, 0x33, 0x38, 0x00, 0x45, 0x32, 0x30, 0x30, 0x34, 0x30, 0x39, 0x30, 0x32, 0x33, 0x31, 0x00, 0x45, 0x32, 0x30, 0x30, 0x34, 0x30, 0x39, 0x30, 0x32, 0x33, 0x32, 0x00, 0x45, 0x32, 0x30, 0x30, 0x34, 0x30, 0x39, 0x30, 0x32, 0x33, 0x33, 0x00, 0x45, 0x32, 0x30, 0x30, 0x34, 0x30, 0x39, 0x30, 0x32, 0x33, 0x34, 0x00, 0x45, 0x32, 0x30, 0x30, 0x34, 0x30, 0x39, 0x30, 0x32, 0x33, 0x35, 0x00, 0x45, 0x32, 0x30, 0x30, 0x34, 0x30, 0x39, 0x30, 0x32, 0x33, 0x36, 0x00, 0x45, 0x32, 0x30, 0x30, 0x34, 0x30, 0x39, 0x30, 0x32, 0x33, 0x39, 0x00, 0x45, 0x32, 0x30, 0x30, 0x34, 0x30, 0x39, 0x30, 0x32, 0x34, 0x30, 0x00, 0x45, 0x32, 0x30, 0x30, 0x34, 0x30, 0x39, 0x30, 0x32, 0x34, 0x31, 0x00, 0x45, 0x32, 0x30, 0x30, 0x34, 0x30, 0x39, 0x30, 0x32, 0x34, 0x32, 0x00, 0x45, 0x32, 0x30, 0x30, 0x34, 0x30, 0x39, 0x30, 0x32, 0x34, 0x33, 0x00, 0x45, 0x32, 0x30, 0x30, 0x34, 0x30, 0x39, 0x30, 0x32, 0x34, 0x34, 0x00, 0x45, 0x32, 0x30, 0x30, 0x34, 0x30, 0x39, 0x30, 0x32, 0x34, 0x35, 0x00, 0x45, 0x32, 0x30, 0x30, 0x34, 0x30, 0x39, 0x30, 0x32, 0x34, 0x36, 0x00, 0x45, 0x32, 0x30, 0x30, 0x34, 0x30, 0x39, 0x30, 0x32, 0x34, 0x37, 0x00, 0x45, 0x32, 0x30, 0x30, 0x34, 0x30, 0x39, 0x30, 0x32, 0x34, 0x38, 0x00, 0x45, 0x32, 0x30, 0x30, 0x34, 0x30, 0x39, 0x30, 0x32, 0x34, 0x39, 0x00, 0x45, 0x32, 0x30, 0x30, 0x34, 0x30, 0x39, 0x30, 0x32, 0x35, 0x30, 0x00, 0x45, 0x32, 0x30, 0x30, 0x34, 0x30, 0x39, 0x30, 0x32, 0x35, 0x31, 0x00, 0x45, 0x32, 0x30, 0x30, 0x34, 0x30, 0x39, 0x30, 0x32, 0x35, 0x32, 0x00, 0x00
};

extern void SJMEM_Destroy();
extern void *SJMEM_GetUuid();
extern void SJMEM_Reset();
extern int SJMEM_GetChunk();
extern int SJMEM_UngetChunk();
extern int SJMEM_PutChunk();
extern int SJMEM_GetNumData();
extern int SJMEM_IsGetChunk();
extern void SJMEM_EntryErrFunc();

__declspec(section ".data") __attribute__((aligned(8))) void *lbl_eu_80565C00[12] = {
    0, 0, 0,
    SJMEM_Destroy,
    SJMEM_GetUuid,
    SJMEM_Reset,
    SJMEM_GetChunk,
    SJMEM_UngetChunk,
    SJMEM_PutChunk,
    SJMEM_GetNumData,
    SJMEM_IsGetChunk,
    SJMEM_EntryErrFunc
};

u32 lbl_eu_805ECE48[2];
u8 lbl_eu_805ECE50[0x480];

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
void  sjmem_GetChunk(SJMEM *self, int mode, int size, SJ_CHUNK *out);
void  sjmem_PutChunk(SJMEM *self, int mode, SJ_CHUNK *chunk);
void  sjmem_UngetChunk(SJMEM *self, int mode, SJ_CHUNK *chunk);
int   sjmem_IsGetChunk(SJMEM *self, int mode, int size, int *out);

/* --- SJMEM_Error --- */
void SJMEM_Error(void) {
    SJERR_CallErr(lbl_eu_80518A68);
}

/* --- SJMEM_Init --- */
void SJMEM_Init(void) {
    SJCRS_Init();
    SJCRS_Lock();
    if (lbl_eu_805ECE48[0] == 0) {
        memset(lbl_eu_805ECE50, 0, 0x480);
    }
    lbl_eu_805ECE48[0]++;
    SJCRS_Unlock();
}

/* --- SJMEM_Finish --- */
void SJMEM_Finish(void) {
    SJCRS_Lock();
    if (--lbl_eu_805ECE48[0] == 0) {
        memset(lbl_eu_805ECE50, 0, 0x480);
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
#pragma push
#pragma opt_propagation off
void *sjmem_Create(void *pool_mem, u32 flags) {
    const char *suffix;
    SJMEM *self;
    char buf_invalid[64];
    char buf_null[64];
    SJMEM *p = (SJMEM *)lbl_eu_805ECE50;
    int i;

    /* scan the fixed table of 0x20 slots for a free (valid == 0) entry */
    for (i = 0; i < 0x20; i++) {
        if (p->valid == 0) break;
        p++;
    }
    if (i == 0x20) return NULL;

    {
        self = &((SJMEM *)lbl_eu_805ECE50)[i];
        self->valid = 1;
        self->vtable = lbl_eu_80565C00;
        self->pool_mem = (u8 *)pool_mem;
        self->buf_size = flags;
        self->uuid = lbl_eu_80518A58;
        self->err_func = (void (*)(void *, int))SJMEM_Error;
        self->err_arg = self;

        /* defensive NULL/validity re-checks with formatted error messages */
        if (self == NULL) {
            suffix = lbl_eu_80518A68 + 0x0C;
            CRICRW_Strcpy(buf_null, 0x40, lbl_eu_80518A68 + 0x27);
            CRICRW_Strcat(buf_null, 0x40, suffix);
            SJERR_CallErr(buf_null);
        } else if (self->valid == 0) {
            suffix = lbl_eu_80518A68 + 0x33;
            CRICRW_Strcpy(buf_invalid, 0x40, lbl_eu_80518A68 + 0x53);
            CRICRW_Strcat(buf_invalid, 0x40, suffix);
            SJERR_CallErr(buf_invalid);
        } else {
            self->avail = self->buf_size;
            self->offset = 0;
        }
        return self;
    }
}
#pragma pop

/* --- SJMEM_Destroy --- */
#pragma push
#pragma opt_propagation off
void SJMEM_Destroy(void *self_ptr) {
    char buf2[64];
    char buf1[64];
    SJMEM *self = (SJMEM *)self_ptr;
    SJCRS_Lock();
    if (self == NULL) {
        const char *suffix = lbl_eu_80518A68 + 0x0C;
        CRICRW_Strcpy(buf1, 0x40, lbl_eu_80518A68 + 0x5F);
        CRICRW_Strcat(buf1, 0x40, suffix);
        SJERR_CallErr(buf1);
    } else if (self->valid == 0) {
        const char *suffix = lbl_eu_80518A68 + 0x33;
        CRICRW_Strcpy(buf2, 0x40, lbl_eu_80518A68 + 0x6B);
        CRICRW_Strcat(buf2, 0x40, suffix);
        SJERR_CallErr(buf2);
    } else {
        memset(self, 0, 0x24);
        self->valid = 0;
    }
    SJCRS_Unlock();
}
#pragma pop

/* --- SJMEM_GetUuid --- */
#pragma push
#pragma opt_propagation off
void *SJMEM_GetUuid(void *self_ptr) {
    char buf2[64];
    char buf1[64];
    SJMEM *self = (SJMEM *)self_ptr;
    void *r;
    SJCRS_Lock();
    if (self == NULL) {
        const char *suffix = lbl_eu_80518A68 + 0x0C;
        CRICRW_Strcpy(buf1, 0x40, lbl_eu_80518A68 + 0x77);
        CRICRW_Strcat(buf1, 0x40, suffix);
        SJERR_CallErr(buf1);
        r = NULL;
    } else if (self->valid == 0) {
        const char *suffix = lbl_eu_80518A68 + 0x33;
        CRICRW_Strcpy(buf2, 0x40, lbl_eu_80518A68 + 0x83);
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

/* --- SJMEM_EntryErrFunc --- */
#pragma push
#pragma opt_propagation off
void SJMEM_EntryErrFunc(void *self_ptr, void *cbfunc, void *cbarg) {
    char buf2[64];
    char buf1[64];
    SJMEM *self = (SJMEM *)self_ptr;
    SJCRS_Lock();
    if (self == NULL) {
        const char *suffix = lbl_eu_80518A68 + 0x0C;
        CRICRW_Strcpy(buf1, 0x40, lbl_eu_80518A68 + 0x8F);
        CRICRW_Strcat(buf1, 0x40, suffix);
        SJERR_CallErr(buf1);
    } else if (self->valid == 0) {
        const char *suffix = lbl_eu_80518A68 + 0x33;
        CRICRW_Strcpy(buf2, 0x40, lbl_eu_80518A68 + 0x9B);
        CRICRW_Strcat(buf2, 0x40, suffix);
        SJERR_CallErr(buf2);
    } else {
        self->err_func = (void (*)(void *, int))cbfunc;
        self->err_arg = cbarg;
    }
    SJCRS_Unlock();
}
#pragma pop

/* --- SJMEM_Reset --- */
#pragma push
#pragma opt_propagation off
void SJMEM_Reset(void *self_ptr) {
    char buf2[64];
    char buf1[64];
    SJMEM *self = (SJMEM *)self_ptr;
    SJCRS_Lock();
    if (self == NULL) {
        const char *suffix = lbl_eu_80518A68 + 0x0C;
        CRICRW_Strcpy(buf1, 0x40, lbl_eu_80518A68 + 0x27);
        CRICRW_Strcat(buf1, 0x40, suffix);
        SJERR_CallErr(buf1);
    } else if (self->valid == 0) {
        const char *suffix = lbl_eu_80518A68 + 0x33;
        CRICRW_Strcpy(buf2, 0x40, lbl_eu_80518A68 + 0x53);
        CRICRW_Strcat(buf2, 0x40, suffix);
        SJERR_CallErr(buf2);
    } else {
        self->avail = self->buf_size;
        self->offset = 0;
    }
    SJCRS_Unlock();
}
#pragma pop

/* --- SJMEM_GetNumData --- */
#pragma push
#pragma opt_propagation off
int SJMEM_GetNumData(void *self_ptr, int mode) {
    char buf2[64];
    char buf1[64];
    SJMEM *self = (SJMEM *)self_ptr;
    int r;
    SJCRS_Lock();
    if (self == NULL) {
        const char *suffix = lbl_eu_80518A68 + 0x0C;
        CRICRW_Strcpy(buf1, 0x40, lbl_eu_80518A68 + 0xA7);
        CRICRW_Strcat(buf1, 0x40, suffix);
        SJERR_CallErr(buf1);
        r = 0;
    } else if (self->valid == 0) {
        const char *suffix = lbl_eu_80518A68 + 0x33;
        CRICRW_Strcpy(buf2, 0x40, lbl_eu_80518A68 + 0xB3);
        CRICRW_Strcat(buf2, 0x40, suffix);
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
#pragma pop

/* --- SJMEM_GetChunk --- */
int SJMEM_GetChunk(void *self, int mode, int size, SJ_CHUNK *out) {
    SJCRS_Lock();
    sjmem_GetChunk((SJMEM *)self, mode, size, out);
    SJCRS_Unlock();
}

/* --- sjmem_GetChunk (internal, 0x134) --- */
#pragma push
#pragma opt_propagation off
void sjmem_GetChunk(SJMEM *self, int mode, int size, SJ_CHUNK *out) {
    if (self == NULL) {
        char buf[64];
        const char *suffix = lbl_eu_80518A68 + 0x0C;
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518A68 + 0xBF);
        CRICRW_Strcat(buf, 0x40, suffix);
        SJERR_CallErr(buf);
    } else if (self->valid == 0) {
        char buf[64];
        const char *suffix = lbl_eu_80518A68 + 0x33;
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518A68 + 0xCB);
        CRICRW_Strcat(buf, 0x40, suffix);
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
}
#pragma pop

/* --- SJMEM_PutChunk --- */
int SJMEM_PutChunk(void *self, int mode, SJ_CHUNK *chunk) {
    SJCRS_Lock();
    sjmem_PutChunk((SJMEM *)self, mode, chunk);
    SJCRS_Unlock();
}

/* --- sjmem_PutChunk (internal, 0xFC) --- */
#pragma push
#pragma opt_propagation off
void sjmem_PutChunk(SJMEM *self, int mode, SJ_CHUNK *chunk) {
    if (self == NULL) {
        char buf[64];
        const char *suffix = lbl_eu_80518A68 + 0x0C;
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518A68 + 0xD7);
        CRICRW_Strcat(buf, 0x40, suffix);
        SJERR_CallErr(buf);
        goto end;
    }
    if (self->valid == 0) {
        char buf[64];
        const char *suffix = lbl_eu_80518A68 + 0x33;
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518A68 + 0xE3);
        CRICRW_Strcat(buf, 0x40, suffix);
        SJERR_CallErr(buf);
        goto end;
    }
    if (chunk->size > 0 && chunk->ptr != NULL) goto body;
    goto exit;
exit:
    return;
body:
    if (mode == 0) {
        /* no-op */
        return;
    }
    if (mode == 1) {
        /* no-op */
        return;
    }
    chunk->size = 0;
    chunk->ptr = NULL;
    if (self->err_func) self->err_func(self->err_arg, -3);
end:
    return;
}
#pragma pop

/* --- SJMEM_UngetChunk --- */
int SJMEM_UngetChunk(void *self, int mode, SJ_CHUNK *chunk) {
    SJCRS_Lock();
    sjmem_UngetChunk((SJMEM *)self, mode, chunk);
    SJCRS_Unlock();
}

/* --- sjmem_UngetChunk (internal, 0x18C) --- */
#pragma push
#pragma opt_propagation off
void sjmem_UngetChunk(SJMEM *self, int mode, SJ_CHUNK *chunk) {
    char buf_null[64];
    char buf_valid[64];
    if (self == NULL) {
        const char *suffix = lbl_eu_80518A68 + 0x0C;
        CRICRW_Strcpy(buf_null, 0x40, lbl_eu_80518A68 + 0xEF);
        CRICRW_Strcat(buf_null, 0x40, suffix);
        SJERR_CallErr(buf_null);
        goto end;
    }
    if (self->valid == 0) {
        const char *suffix = lbl_eu_80518A68 + 0x33;
        CRICRW_Strcpy(buf_valid, 0x40, lbl_eu_80518A68 + 0xFB);
        CRICRW_Strcat(buf_valid, 0x40, suffix);
        SJERR_CallErr(buf_valid);
        goto end;
    }
    /* unget only applies to a live chunk */
    if (chunk->size > 0 && chunk->ptr != NULL) goto body;
    goto exit;
exit:
    return;
body:
    if (mode == 0) {
        if (self->err_func) self->err_func(self->err_arg, -3);
        return;
    }
    if (mode == 1) {
        /* rewind offset by chunk size (clamped at 0), return the
         * consumed bytes to avail (clamped at buf_size) */
        int new_offset = (int)(self->offset - chunk->size);
        int new_avail;
        int clamped = (new_offset > 0) ? new_offset : 0;
        self->offset = clamped;
        new_avail = self->avail + chunk->size;
        if ((int)self->buf_size < new_avail) new_avail = self->buf_size;
        self->avail = new_avail;
        if (clamped != (int)(chunk->ptr - self->pool_mem)) {
            if (self->err_func) self->err_func(self->err_arg, -3);
        }
        return;
    }
    chunk->size = 0;
    chunk->ptr = NULL;
    if (self->err_func) self->err_func(self->err_arg, -3);
end:
    return;
}
#pragma pop

/* --- SJMEM_IsGetChunk --- */
int SJMEM_IsGetChunk(void *self, int mode, int size, int *out) {
    int r;
    SJCRS_Lock();
    r = sjmem_IsGetChunk((SJMEM *)self, mode, size, out);
    SJCRS_Unlock();
    return r;
}

/* --- sjmem_IsGetChunk (internal, 0x128) --- */
#pragma push
#pragma opt_propagation off
int sjmem_IsGetChunk(SJMEM *self, int mode, int size, int *out) {
    int result;
    if (self == NULL) {
        char buf[64];
        const char *suffix = lbl_eu_80518A68 + 0x0C;
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518A68 + 0x107);
        CRICRW_Strcat(buf, 0x40, suffix);
        SJERR_CallErr(buf);
        return 0;
    }
    if (self->valid == 0) {
        char buf[64];
        const char *suffix = lbl_eu_80518A68 + 0x33;
        CRICRW_Strcpy(buf, 0x40, lbl_eu_80518A68 + 0x113);
        CRICRW_Strcat(buf, 0x40, suffix);
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
#pragma pop

/* --- SJMEM_GetBufSize --- */
#pragma push
#pragma opt_propagation off
int SJMEM_GetBufSize(void *self_ptr) {
    char buf2[64];
    char buf1[64];
    SJMEM *self = (SJMEM *)self_ptr;
    int r;
    SJCRS_Lock();
    if (self == NULL) {
        const char *suffix = lbl_eu_80518A68 + 0x0C;
        CRICRW_Strcpy(buf1, 0x40, lbl_eu_80518A68 + 0x137);
        CRICRW_Strcat(buf1, 0x40, suffix);
        SJERR_CallErr(buf1);
        r = 0;
    } else if (self->valid == 0) {
        const char *suffix = lbl_eu_80518A68 + 0x33;
        CRICRW_Strcpy(buf2, 0x40, lbl_eu_80518A68 + 0x143);
        CRICRW_Strcat(buf2, 0x40, suffix);
        SJERR_CallErr(buf2);
        r = 0;
    } else {
        r = self->buf_size;
    }
    SJCRS_Unlock();
    return r;
}
#pragma pop
