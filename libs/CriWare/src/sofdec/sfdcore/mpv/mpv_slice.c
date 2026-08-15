// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/mpv/mpv_slice
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

typedef struct MpvSjChunk {
    const u8* p;
    s32 size;
} MpvSjChunk;

/* DecSliceOne output: per-slice statistics written into the caller's struct. */
typedef struct MpvSliceOut {
    s32 field_0x00;  /* 0x00 */
    s32 field_0x04;  /* 0x04 */
    s32 field_0x08;  /* 0x08 */
    s32 field_0x0C;  /* 0x0C */
    s32 field_0x10;  /* 0x10 */
    s32 field_0x14;  /* 0x14 */
} MpvSliceOut;

/* MPV slice-decode context (retail layout, offsets from self). */
typedef struct MpvSlice {
    u8  pad0[0xB50];
    s32 b50;                    /* 0xB50 */
    u8  padB54[0xB64 - 0xB54];
    s32 b64;                    /* 0xB64 */
    u8  padB68[0xBE8 - 0xB68];
    s32 be8;                    /* 0xBE8 */
    s32 bec;                    /* 0xBEC */
    u8  padBF0[0xC30 - 0xBF0];
    s16 c30;                    /* 0xC30 */
    u8  padC32[0xC60 - 0xC32];
    void (*c60)(void*, void*);  /* 0xC60 */
    u8  padC64[0xC8C - 0xC64];
    s32 c8c;                    /* 0xC8C */
    u8  padC90[0xCB4 - 0xC90];
    s32 cb4[4];                 /* 0xCB4 */
    u8  padCC4[0xCD8 - 0xCC4];
    s32 cd8;                    /* 0xCD8 */
    s32 cdc;                    /* 0xCDC */
    s32 ce0;                    /* 0xCE0 */
    s32 ce4;                    /* 0xCE4 */
    u8  padCE8[0xD2C - 0xCE8];
    MpvSjChunk chunk;           /* 0xD2C */
    s32 d34;                    /* 0xD34 */
    u8  padD38[0xD48 - 0xD38];
    s32 d48;                    /* 0xD48 */
    u8  padD4C[0xDA4 - 0xD4C];
    s32 da4;                    /* 0xDA4 */
    s32 da8;                    /* 0xDA8 */
} MpvSlice;

extern s32 MPV_CheckDelim(const u8* p);
extern s32 MPVHDEC_RecoverSj(void* self, s32 mask, void* sj);
extern s32 MPVERR_SetCode(void* self, u32 code);
extern void MPVDEC_ResetMv(void* self);
extern void MPVDEC_ResetDc(void* self);
extern void SJ_SplitChunk(const MpvSjChunk* src, s32 size, MpvSjChunk* dst1,
                          MpvSjChunk* dst2);

void MPVSL_DecSliceOne(MpvSlice* self, void* sj, MpvSliceOut* out);

s32 MPVSL_DecPicture(MpvSlice* self, void* sj) {
    MpvSjChunk st;
    MpvSliceOut out;
    s32 total = 0;
    u32 maxCode = 257;
    s32 ret;

    self->c30 = 0;
    for (;;) {
        if (self->da8 != 0)
            return 0;
        ret = MPVHDEC_RecoverSj(self, -1, sj);
        if (ret != 0)
            return MPVERR_SetCode(self, ret);
        ((void (*)(void*, s32, s32, MpvSjChunk*))*(void**)((char*)*(void**)sj + 0x18))(
            sj, 1, 0x7FFFFFFF, &st);
        ((void (*)(void*, s32, MpvSjChunk*))*(void**)((char*)*(void**)sj + 0x1C))(
            sj, 1, &st);
        if (st.size >= 4 && (MPV_CheckDelim(st.p) & 1)) {
            u32 code = (u32)st.p[3] | 0x100;
            if (maxCode <= code) {
                maxCode = code;
            } else {
                self->be8++;
                break;
            }
            self->c30++;
            MPVSL_DecSliceOne(self, sj, &out);
            total += out.field_0x08;
            if (out.field_0x14 == 0)
                continue;
            break;
        }
        break;
    }
    if (self->cd8 != self->ce4)
        self->be8++;
    if (total != 0)
        self->be8 += total;
    return 0;
}

void MPVSL_DecSliceOne(MpvSlice* self, void* sj, MpvSliceOut* out) {
    MpvSjChunk rest;
    u32 buf;
    u32 cur;
    s32 bitoff;
    u8* wp;
    s32 be8;
    s32 bec;

    out->field_0x00 = 0;
    out->field_0x04 = 0;
    out->field_0x08 = 0;
    out->field_0x0C = -1;
    out->field_0x10 = -1;
    out->field_0x14 = 0;
    self->da4 = 0;
    self->d48 = self->b50;
    be8 = self->be8;
    bec = self->bec;

    ((void (*)(void*, s32, s32, MpvSjChunk*))*(void**)((char*)*(void**)sj + 0x18))(
        sj, 1, 0x7FFFFFFF, &self->chunk);

    {
        u32 p = (u32)(uintptr_t)self->chunk.p;
        const u32* q = (const u32*)(uintptr_t)(p & ~3u);
        u32 w0 = q[0];
        bitoff = (s32)((p - (u32)(uintptr_t)q) << 3);
        u32 w1 = q[1];
        u32 hi;
        u8 byte0;

        hi = w0 << bitoff;
        if (bitoff != 0) {
            buf = w1 << bitoff;
            hi |= w1 >> (32 - bitoff);
        } else {
            buf = w1;
        }
        byte0 = (u8)hi;
        cur = q[2];
        self->ce0 = -1;
        self->cdc = (s32)byte0 - 1;
        self->cd8 = ((s32)byte0 - 1) * self->b64 - 1;
        wp = (u8*)(uintptr_t)q + 0xC;
    }

    /* Read the next 5 bits into c8c (dc/chroma size prefix). */
    if (bitoff >= 0x1B) {
        bitoff -= 0x1B;
        if (bitoff != 0) {
            buf |= cur >> (5 - bitoff);
            self->c8c = (s32)(buf >> 27);
            buf = cur << bitoff;
        } else {
            self->c8c = (s32)(buf >> 27);
            buf = cur;
        }
        cur = *(u32*)(uintptr_t)wp;
        wp += 4;
    } else {
        self->c8c = (s32)(buf >> 27);
        buf <<= 5;
        bitoff += 5;
    }

    MPVDEC_ResetMv((u8*)self + 0xC90);
    MPVDEC_ResetMv((u8*)self + 0xCB4);
    MPVDEC_ResetDc(self);

    /* Scan the slice payload: while the MSB is 1 consume 9 bits; a 0 bit
     * marks the slice-end code. Exhausting the chunk stops without a split. */
    for (;;) {
        if ((buf >> 31) == 0) {
            bitoff += 1;
            if (bitoff >= 0x20) {
                bitoff -= 0x20;
                wp += 4;
            }
            break;
        }
        bitoff += 9;
        if (bitoff >= 0x20) {
            bitoff -= 0x20;
            buf = cur << bitoff;
            cur = *(u32*)(uintptr_t)wp;
            wp += 4;
        } else {
            buf <<= 9;
        }
        if (self->chunk.size <=
            (s32)(wp + ((bitoff + 7) >> 3) - 8) - (s32)(uintptr_t)self->chunk.p)
            goto done;
    }

    /* Slice end: split the remainder, hand it back to the job, decode a row. */
    {
        s32 byte_off = bitoff & 7;
        s32 used = (bitoff - byte_off + 7) >> 3;
        self->d34 = byte_off;
        SJ_SplitChunk(&self->chunk,
                      (s32)(wp + used - 8) - (s32)(uintptr_t)self->chunk.p,
                      &self->chunk, &rest);
        ((void (*)(void*, s32, MpvSjChunk*))*(void**)((char*)*(void**)sj + 0x20))(
            sj, 0, &self->chunk);
        ((void (*)(void*, s32, MpvSjChunk*))*(void**)((char*)*(void**)sj + 0x1C))(
            sj, 1, &rest);
        ((void (*)(void*, void*))*(void**)((char*)self + 0xC60))(self, sj);
        if (self->cd8 >= self->ce4)
            out->field_0x14 = 1;
        out->field_0x00 = self->be8 - be8;
        out->field_0x04 = self->bec - bec;
        out->field_0x08 = self->da4;
    }
done:
    return;
}
