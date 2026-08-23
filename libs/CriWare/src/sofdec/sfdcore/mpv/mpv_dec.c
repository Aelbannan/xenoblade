// CriWare Sofdec MPEG-2 picture-layer macroblock decoder (mpv_dec).
// Reconstructed from retail (Sony SDL-derived "mpv" decoder).
//
// All three slice decoders (I/P/B) share the same bit-buffer machine:
//   hi/lo = 64-bit bit accumulator, bc = bit count, ptr = word cursor.
// VLC tables are pointer variables in .bss filled at MPVVLC_Init time.

#include <harness_catalog.h>

typedef struct MpvSjChunk {
    const u8* p;    /* 0x00 */
    s32 size;       /* 0x04 */
} MpvSjChunk;

/* Stream job object: first member is a function table (slots 6/7/8 used). */
typedef struct SjVtbl {
    void (*f[9])(void);
} SjVtbl;

typedef struct SjObj {
    SjVtbl* vtbl;   /* 0x00 */
} SjObj;

/* MPV decoder context (retail layout, offsets from self). */
typedef struct MpvDec {
    u8  pad0[0x980];
    u32 hi;                    /* 0x980 bit accumulator high word */
    u32 lo;                    /* 0x984 bit accumulator low word */
    s32 bc;                    /* 0x988 bit count */
    u8* ptr;                   /* 0x98c word cursor */
    u8  pad990[0xb50 - 0x990];
    u32 b50;                   /* 0xb50 */
    void (*b54)(void*);        /* 0xb54 */
    void* b58;                 /* 0xb58 */
    u8  padB5C[0xb64 - 0xb5c];
    s32 b64;                   /* 0xb64 */
    u8  padB68[0xc64 - 0xb68];
    void (*c64)(void*);        /* 0xc64 */
    void (*c68)(void*);        /* 0xc68 */
    void (*c6c)(void*);        /* 0xc6c */
    void (*c70)(void*);        /* 0xc70 */
    void (*c74[4])(void*);     /* 0xc74 (0xc7c = c74[2]) */
    u8  padC84[0xc8c - 0xc84];
    s32 c8c;                   /* 0xc8c */
    s32 c90[4];                /* 0xc90 forward motion params */
    s32 ca0;                   /* 0xca0 */
    s32 ca4;                   /* 0xca4 */
    s32 ca8;                   /* 0xca8 */
    s32 cac;                   /* 0xcac */
    u8  padCB0[0xcb4 - 0xcb0];
    s32 cb4[4];                /* 0xcb4 backward motion params */
    s32 cc4;                   /* 0xcc4 */
    s32 cc8;                   /* 0xcc8 */
    s32 ccc;                   /* 0xccc */
    s32 cd0;                   /* 0xcd0 */
    u8  padCD4[0xcd8 - 0xcd4];
    s32 cd8;                   /* 0xcd8 macroblock address */
    s32 cdc;                   /* 0xcdc */
    s32 ce0;                   /* 0xce0 */
    s32 ce4;                   /* 0xce4 */
    s32 ce8;                   /* 0xce8 */
    s32 cec;                   /* 0xcec */
    s32 cf0;                   /* 0xcf0 */
    s32 cf4;                   /* 0xcf4 */
    s32 cf8;                   /* 0xcf8 */
    u8  padCFC[0xd2c - 0xcfc];
    MpvSjChunk chunk;          /* 0xd2c */
    s32 d34;                   /* 0xd34 */
    u8  padD38[0xd48 - 0xd38];
    s32 d48;                   /* 0xd48 */
    u8  padD4C[0xda8 - 0xd4c];
    s32 da8;                   /* 0xda8 */
} MpvDec;

extern char lbl_eu_8051C088[];
extern int strcmp(const char* a, const char* b);

/* VLC table pointer variables (filled by mpv_vlc.c init code). */
extern s16* lbl_eu_80603508;   /* motion code small table */
extern s16* lbl_eu_80603610;   /* motion code big table */
extern s16* lbl_eu_80604628;   /* I-pic mb_type small table */
extern s16* lbl_eu_8060462C;   /* I-pic mb_type big table */
extern s16* lbl_eu_80604630;   /* P-pic mb_type small table */
extern s16* lbl_eu_80604634;   /* P-pic mb_type big table */
extern s16* lbl_eu_80604638;   /* B-pic mb_type small table */
extern s16* lbl_eu_8060463C;   /* B-pic mb_type big table */
extern s16* lbl_eu_80604640;   /* dct_type table */
extern s16* lbl_eu_80603658;   /* P-pic dc_size table */
extern s16* lbl_eu_806036A0;   /* B-pic dc_size table */

extern s32 MPV_GoNextDelimSj(void* sj);
extern void SJ_SplitChunk(const MpvSjChunk* src, int size, MpvSjChunk* dst1, MpvSjChunk* dst2);

s32 mpvdec_MotionSub(MpvDec* self, s32* mv, s32* out1, s32* out2);

s32 MPVDEC_CheckVersion(const char* version, unsigned int size, s32 v) {
    // Sequential early-out ifs match the retail dispatch:
    //   beq cont / li r3,-1 / b end / cont: ...
    // Known wall: switch form gives the exact retail dispatch
    // (beq body / li -1 / b end twice) but lowers the size compare to
    // signed cmpi under GC/3.0a5.2 (retail cmplwi). Every if/else/ternary
    // variant emits unsigned cmpli but CSE-merges the two `li r3,-1`
    // blocks into one (116B vs 120B). No shape found emits both together.
    if (strcmp(lbl_eu_8051C088, version) != 0)
        return -1;
    switch ((unsigned long)size) {
    default:
        return -1;
    case 0xDAC:
        return ((v - 128) | (128 - v)) >> 31;
    }
}

/* Refill the 64-bit bit accumulator from the chunk; extra = initial bit offset. */
static inline void mpvdec_Refill(MpvDec* self, s32 extra, u32* hi, u32* lo, s32* bc, u8** ptr) {
    u32 p = (u32)(uintptr_t)self->chunk.p;
    u32 aligned = p & ~3u;
    u32 off = p - aligned;
    u32 word = *(u32*)aligned;
    s32 bitoff = (s32)(off << 3);
    *lo = *(u32*)(aligned + 4);
    *hi = word << bitoff;
    *ptr = (u8*)(aligned + 8);
    *bc = bitoff + extra;
    if (*bc >= 0x20) {
        *bc -= 0x20;
        *hi = *lo << *bc;
        *lo = *(u32*)*ptr;
        *ptr += 4;
    } else {
        *hi <<= extra;
    }
}

/* Read 5 bits into c8c (dc_size chroma) when the ce8 bit-4 flag is set. */
static inline void mpvdec_ReadDcSizeC(MpvDec* self, u32* hi, u32* lo, s32* bc, u8** ptr) {
    u32 v5 = *hi >> 27;
    if (*bc < 0x1b) {
        *hi <<= 5;
        *bc += 5;
    } else {
        *bc -= 0x1b;
        if (*bc != 0) {
            *hi |= *lo >> (5 - *bc);
            v5 = *hi >> 27;
            *hi = *lo << *bc;
            *lo = *(u32*)*ptr;
            *ptr += 4;
        } else {
            v5 = *hi >> 27;
            *hi = *lo;
            *lo = *(u32*)*ptr;
            *ptr += 4;
        }
    }
    self->c8c = (s32)v5;
}

s32 MPVDEC_DecIpicMb(MpvDec* self, SjObj* sj) {
    MpvSjChunk rest_end;   /* slice-end split remainder */
    MpvSjChunk rest_loop;  /* mid-slice split remainder */
    s32 bc;
    u32 hi;
    u32 lo;
    u8* ptr;
    s32 type;

    ((void (*)(void*, s32, s32, MpvSjChunk*))sj->vtbl->f[6])(sj, 1, 0x7fffffff, &self->chunk);

    mpvdec_Refill(self, self->d34, &hi, &lo, &bc, &ptr);

    for (;;) {
        /* macroblock_address_increment VLC (9 bits); 0 ends the slice */
        u32 inc = hi >> 9;
        if (bc > 9)
            inc |= lo >> (0x29 - bc);
        if (inc == 0)
            break;

        s32 old_cd8 = self->cd8;
        for (;;) {
            /* macroblock_type VLC (12 bits) */
            u32 mb = hi >> 20;
            if (bc > 0x14)
                mb |= lo >> (0x34 - bc);

            s16 t;
            if ((mb >> 8) == 0)
                t = lbl_eu_80604628[mb];
            else
                t = lbl_eu_8060462C[mb >> 6];

            u32 len = (u32)t & 0xF;
            bc += len;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *(u32*)ptr;
                ptr += 4;
            } else {
                hi <<= len;
            }

            type = (s32)(((u32)t >> 2) & 0xFF) >> 2;
            if (type == 0x22)
                continue;
            if (type == 0x23) {
                self->cd8 += 0x21;
                continue;
            }
            if (type == 0x24) {
                type = -2;
                break;
            }
            {
                s32 new_cd8 = self->cd8 + type;
                self->cd8 = new_cd8;
                self->ce8 = (u32)t >> 10;
                if (new_cd8 > self->ce4) {
                    type = -2;
                    break;
                }
                type = new_cd8 - old_cd8;
                self->ce0 += type;
                while (self->ce0 >= self->b64) {
                    self->ce0 -= new_cd8;
                    self->cdc++;
                }
            }
            break;
        }
        if (type == -2)
            break;

        if ((self->ce8 & 0x10) != 0)
            mpvdec_ReadDcSizeC(self, &hi, &lo, &bc, &ptr);

        self->hi = hi;
        self->lo = lo;
        self->bc = bc;
        self->ptr = ptr;
        self->c68(self);
        self->c70(self);
        hi = self->hi;
        lo = self->lo;
        bc = self->bc;
        ptr = self->ptr;

        if (self->da8 != 0)
            break;
        self->d48--;
        if (self->d48 <= 0) {
            self->d48 = self->b50;
            self->b54(self->b58);
        }

        /* When few bytes remain in the chunk, split it and refill from the job. */
        s32 byte_off = bc & 7;
        s32 used = (bc - byte_off + 7) >> 3;
        s32 rem = self->chunk.size - ((s32)(ptr + used - 8) - (s32)(uintptr_t)self->chunk.p);
        if (rem <= 0x800) {
            SJ_SplitChunk(&self->chunk, rem, &self->chunk, &rest_loop);
            ((void (*)(void*, s32, MpvSjChunk*))sj->vtbl->f[8])(sj, 0, &self->chunk);
            ((void (*)(void*, s32, MpvSjChunk*))sj->vtbl->f[7])(sj, 1, &rest_loop);
            ((void (*)(void*, s32, s32, MpvSjChunk*))sj->vtbl->f[6])(sj, 1, 0x7fffffff, &self->chunk);
            mpvdec_Refill(self, byte_off, &hi, &lo, &bc, &ptr);
        }
    }

    /* Slice end: hand the remaining chunk back and advance to the next delim. */
    SJ_SplitChunk(&self->chunk,
                  (s32)(ptr + ((bc + 7) >> 3) - 8) - (s32)(uintptr_t)self->chunk.p,
                  &self->chunk, &rest_end);
    ((void (*)(void*, s32, MpvSjChunk*))sj->vtbl->f[8])(sj, 0, &self->chunk);
    ((void (*)(void*, s32, MpvSjChunk*))sj->vtbl->f[7])(sj, 1, &rest_end);
    return MPV_GoNextDelimSj(sj);
}

s32 MPVDEC_DecPpicMb(MpvDec* self, SjObj* sj) {
    MpvSjChunk rest_end;
    MpvSjChunk rest_loop;
    s32 bc;
    u32 hi;
    u32 lo;
    u8* ptr;
    s32 type;
    s32 first = 1;

    ((void (*)(void*, s32, s32, MpvSjChunk*))sj->vtbl->f[6])(sj, 1, 0x7fffffff, &self->chunk);

    mpvdec_Refill(self, self->d34, &hi, &lo, &bc, &ptr);

    for (;;) {
        u32 inc = hi >> 9;
        if (bc > 9)
            inc |= lo >> (0x29 - bc);
        if (inc == 0)
            break;

        s32 old_cd8 = self->cd8;
        for (;;) {
            /* macroblock_type VLC (11 bits) */
            u32 mb = hi >> 21;
            if (bc > 0x15)
                mb |= lo >> (0x35 - bc);

            s16 t;
            if ((mb >> 7) == 0)
                t = lbl_eu_80604630[mb];
            else
                t = lbl_eu_80604634[mb >> 6];

            u32 len = (u32)t & 0xF;
            bc += len;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *(u32*)ptr;
                ptr += 4;
            } else {
                hi <<= len;
            }

            type = (s32)(((u32)t >> 2) & 0xFF) >> 2;
            if (type == 0x22)
                continue;
            if (type == 0x23) {
                self->cd8 += 0x21;
                continue;
            }
            if (type == 0x24) {
                type = -2;
                break;
            }
            {
                s32 new_cd8 = self->cd8 + type;
                self->cd8 = new_cd8;
                self->ce8 = (u32)t >> 10;
                if (new_cd8 > self->ce4) {
                    type = -2;
                    break;
                }
                type = new_cd8 - old_cd8;
                self->ce0 += type;
                while (self->ce0 >= self->b64) {
                    self->ce0 -= new_cd8;
                    self->cdc++;
                }
            }
            break;
        }
        if (type == -2)
            break;

        /* First macroblock of a slice with a non-skipped type: reset predictors. */
        if (first == 0 && (u32)type > 1) {
            self->c64(self);
            self->ca0 = self->ca4 = self->ca8 = self->cac = 0;
            self->cf4 = self->cf8 = self->cf0 = 0x400;
        }

        /* dc_size luma (5 bits) unless the flag is already set. */
        if ((self->ce8 & 0x20) == 0) {
            u32 code5 = hi >> 27;
            if (bc > 0x1b)
                code5 |= lo >> (0x3b - bc);
            s16 v = lbl_eu_80603658[code5];
            u32 len = (u32)v & 0xFF;
            bc += len;
            self->ce8 = (u32)v >> 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *(u32*)ptr;
                ptr += 4;
            } else {
                hi <<= len;
            }
        }

        if ((self->ce8 & 0x10) != 0)
            mpvdec_ReadDcSizeC(self, &hi, &lo, &bc, &ptr);

        /* Forward motion vectors. */
        if ((self->ce8 & 0x8) != 0) {
            s32 m;
            self->hi = hi;
            self->lo = lo;
            self->bc = bc;
            self->ptr = ptr;
            m = mpvdec_MotionSub(self, &self->c90[0], &self->ca8, &self->ca0);
            m |= mpvdec_MotionSub(self, &self->c90[0], &self->cac, &self->ca4);
            hi = self->hi;
            lo = self->lo;
            bc = self->bc;
            ptr = self->ptr;
            if (m != 0)
                break;
        } else {
            self->ca0 = self->ca4 = self->ca8 = self->cac = 0;
        }

        /* dct_type (9 bits). */
        if ((self->ce8 & 0x2) != 0) {
            u32 code9 = hi >> 23;
            if (bc > 0x17)
                code9 |= lo >> (0x37 - bc);
            s16 v = lbl_eu_80604640[code9];
            u32 len = (u32)v & 0xFF;
            bc += len;
            self->cec = ((u32)v >> 4) & 0xFFF;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *(u32*)ptr;
                ptr += 4;
            } else {
                hi <<= len;
            }
        } else {
            self->cec = 0;
        }

        self->hi = hi;
        self->lo = lo;
        self->bc = bc;
        self->ptr = ptr;
        if ((self->ce8 & 1) != 0) {
            self->c68(self);
            self->c70(self);
        } else {
            if (self->cec != 0)
                self->c6c(self);
            self->c74[2](self);
            self->cf4 = self->cf8 = self->cf0 = 0x400;
        }
        hi = self->hi;
        lo = self->lo;
        bc = self->bc;
        ptr = self->ptr;

        if (self->da8 != 0)
            break;
        self->d48--;
        if (self->d48 <= 0) {
            self->d48 = self->b50;
            self->b54(self->b58);
        }

        s32 byte_off = bc & 7;
        s32 used = (bc - byte_off + 7) >> 3;
        s32 rem = self->chunk.size - ((s32)(ptr + used - 8) - (s32)(uintptr_t)self->chunk.p);
        if (rem <= 0x800) {
            SJ_SplitChunk(&self->chunk, rem, &self->chunk, &rest_loop);
            ((void (*)(void*, s32, MpvSjChunk*))sj->vtbl->f[8])(sj, 0, &self->chunk);
            ((void (*)(void*, s32, MpvSjChunk*))sj->vtbl->f[7])(sj, 1, &rest_loop);
            ((void (*)(void*, s32, s32, MpvSjChunk*))sj->vtbl->f[6])(sj, 1, 0x7fffffff, &self->chunk);
            mpvdec_Refill(self, byte_off, &hi, &lo, &bc, &ptr);
        }
        first = 0;
    }

    SJ_SplitChunk(&self->chunk,
                  (s32)(ptr + ((bc + 7) >> 3) - 8) - (s32)(uintptr_t)self->chunk.p,
                  &self->chunk, &rest_end);
    ((void (*)(void*, s32, MpvSjChunk*))sj->vtbl->f[8])(sj, 0, &self->chunk);
    ((void (*)(void*, s32, MpvSjChunk*))sj->vtbl->f[7])(sj, 1, &rest_end);
    return MPV_GoNextDelimSj(sj);
}

void MPVDEC_ResetMv(void* self) {
    *(u32*)((u8*)self + 0x10) = 0;
    *(u32*)((u8*)self + 0x14) = 0;
    *(u32*)((u8*)self + 0x18) = 0;
    *(u32*)((u8*)self + 0x1c) = 0;
}

void MPVDEC_ResetDc(void* self) {
    *(u32*)((u8*)self + 0xcf4) = 0x400;
    *(u32*)((u8*)self + 0xcf8) = 0x400;
    *(u32*)((u8*)self + 0xcf0) = 0x400;
}

s32 mpvdec_MotionSub(MpvDec* self, s32* mv, s32* out1, s32* out2) {
    s32 bc = self->bc;
    u32 hi = self->hi;
    u32 lo = self->lo;
    u8* ptr = self->ptr;
    /* 9-bit motion code. */
    u32 code = hi >> 21;
    s32 f_code = mv[1];
    s32 ret = 0;
    u32 mvscale = (u32)mv[2];
    s16 v16;
    s16* tbl;
    if (bc > 0x15)
        code |= lo >> (0x35 - bc);

    if ((code >> 7) == 0) {
        tbl = lbl_eu_80603508;
        v16 = tbl[code];
    } else {
        tbl = lbl_eu_80603610;
        v16 = tbl[code >> 6];
    }

    s32 v = (s8)v16;
    if (v == 0x7f) {
        ret = -1;
    } else {
        u32 len = (u16)v16 >> 8;
        bc += len;
        if (bc >= 0x20) {
            bc -= 0x20;
            hi = lo << bc;
            lo = *(u32*)ptr;
            ptr += 4;
        } else {
            hi <<= len;
        }
        if (v == 0) {
            *out1 = *out2;
        } else {
            s32 vr;
            if (f_code != 0) {
                /* Read f_code residual bits. */
                s32 residual;
                if (bc >= 0x20 - f_code) {
                    bc -= 0x20 - f_code;
                    if (bc != 0) {
                        hi |= lo >> (f_code - bc);
                        residual = (s32)(hi >> (0x20 - f_code));
                        hi = lo << bc;
                    } else {
                        residual = (s32)(hi >> (0x20 - f_code));
                        hi = lo;
                    }
                    lo = *(u32*)ptr;
                    ptr += 4;
                } else {
                    residual = (s32)(hi >> (0x20 - f_code));
                    bc += f_code;
                    hi <<= f_code;
                }
                {
                    /* sign-adjusted motion residual */
                    s32 shifted = v << f_code;
                    s32 t = mv[3] - 1 - residual;
                    vr = shifted + t;
                    if (shifted > 0)
                        vr = shifted - t;
                }
            } else {
                vr = v;
            }
            {
                s32 acc = *out2 + vr;
                s32 res = (s32)((u32)acc << mvscale) >> mvscale;
                *out1 = res;
                *out2 = res;
            }
        }
        if (mv[0] != 0)
            *out1 <<= 1;
    }

    self->hi = hi;
    self->lo = lo;
    self->bc = bc;
    self->ptr = ptr;
    return ret;
}

s32 MPVDEC_DecBpicMb(MpvDec* self, SjObj* sj) {
    MpvSjChunk rest_end;
    MpvSjChunk rest_loop;
    s32 bc;
    u32 hi;
    u32 lo;
    u8* ptr;
    s32 type;
    s32 first = 1;

    ((void (*)(void*, s32, s32, MpvSjChunk*))sj->vtbl->f[6])(sj, 1, 0x7fffffff, &self->chunk);

    mpvdec_Refill(self, self->d34, &hi, &lo, &bc, &ptr);

    for (;;) {
        u32 inc = hi >> 9;
        if (bc > 9)
            inc |= lo >> (0x29 - bc);
        if (inc == 0)
            break;

        s32 old_cd8 = self->cd8;
        for (;;) {
            /* macroblock_type VLC (11 bits) */
            u32 mb = hi >> 21;
            if (bc > 0x15)
                mb |= lo >> (0x35 - bc);

            s16 t;
            if ((mb >> 7) == 0)
                t = lbl_eu_80604638[mb];
            else
                t = lbl_eu_8060463C[mb >> 6];

            u32 len = (u32)t & 0xF;
            bc += len;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *(u32*)ptr;
                ptr += 4;
            } else {
                hi <<= len;
            }

            type = (s32)(((u32)t >> 2) & 0xFF) >> 2;
            if (type == 0x22)
                continue;
            if (type == 0x23) {
                self->cd8 += 0x21;
                continue;
            }
            if (type == 0x24) {
                type = -2;
                break;
            }
            {
                s32 new_cd8 = self->cd8 + type;
                self->cd8 = new_cd8;
                self->ce8 = (u32)t >> 10;
                if (new_cd8 > self->ce4) {
                    type = -2;
                    break;
                }
                type = new_cd8 - old_cd8;
                self->ce0 += type;
                while (self->ce0 >= self->b64) {
                    self->ce0 -= new_cd8;
                    self->cdc++;
                }
            }
            break;
        }
        if (type == -2)
            break;

        if (first == 0 && (u32)type > 1) {
            self->c64(self);
            self->cf4 = self->cf8 = self->cf0 = 0x400;
        }

        /* dc_size (6 bits) unless the flag is already set. */
        if ((self->ce8 & 0x20) == 0) {
            u32 code6 = hi >> 26;
            if (bc > 0x1a)
                code6 |= lo >> (0x3a - bc);
            s16 v = lbl_eu_806036A0[code6];
            u32 len = (u32)v & 0xFF;
            bc += len;
            self->ce8 = (u32)v >> 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *(u32*)ptr;
                ptr += 4;
            } else {
                hi <<= len;
            }
        }

        if ((self->ce8 & 0x10) != 0)
            mpvdec_ReadDcSizeC(self, &hi, &lo, &bc, &ptr);

        /* Forward motion vectors. */
        if ((self->ce8 & 0x8) != 0) {
            s32 m;
            self->hi = hi;
            self->lo = lo;
            self->bc = bc;
            self->ptr = ptr;
            m = mpvdec_MotionSub(self, &self->c90[0], &self->ca8, &self->ca0);
            m |= mpvdec_MotionSub(self, &self->c90[0], &self->cac, &self->ca4);
            hi = self->hi;
            lo = self->lo;
            bc = self->bc;
            ptr = self->ptr;
            if (m != 0)
                break;
        }

        /* Backward motion vectors. */
        if ((self->ce8 & 0x4) != 0) {
            s32 m;
            self->hi = hi;
            self->lo = lo;
            self->bc = bc;
            self->ptr = ptr;
            m = mpvdec_MotionSub(self, &self->cb4[0], &self->ccc, &self->cc4);
            m |= mpvdec_MotionSub(self, &self->cb4[0], &self->cd0, &self->cc8);
            hi = self->hi;
            lo = self->lo;
            bc = self->bc;
            ptr = self->ptr;
            if (m != 0)
                break;
        }

        /* dct_type (9 bits). */
        if ((self->ce8 & 0x2) != 0) {
            u32 code9 = hi >> 23;
            if (bc > 0x17)
                code9 |= lo >> (0x37 - bc);
            s16 v = lbl_eu_80604640[code9];
            u32 len = (u32)v & 0xFF;
            bc += len;
            self->cec = ((u32)v >> 4) & 0xFFF;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *(u32*)ptr;
                ptr += 4;
            } else {
                hi <<= len;
            }
        } else {
            self->cec = 0;
        }

        self->hi = hi;
        self->lo = lo;
        self->bc = bc;
        self->ptr = ptr;
        if ((self->ce8 & 1) != 0) {
            self->c68(self);
            self->c70(self);
            self->ca0 = self->ca4 = self->ca8 = self->cac = 0;
            self->cc4 = self->cc8 = self->ccc = self->cd0 = 0;
        } else {
            /* Pick the reconstruction callback by the ce8 low bits. */
            void (*fn)(void*) = self->c74[(self->ce8 & 0xC) >> 2];
            self->c74[0] = fn;
            if (self->cec != 0)
                self->c6c(self);
            fn(self);
            self->cf4 = self->cf8 = self->cf0 = 0x400;
        }
        hi = self->hi;
        lo = self->lo;
        bc = self->bc;
        ptr = self->ptr;

        if (self->da8 != 0)
            break;
        self->d48--;
        if (self->d48 <= 0) {
            self->d48 = self->b50;
            self->b54(self->b58);
        }

        s32 byte_off = bc & 7;
        s32 used = (bc - byte_off + 7) >> 3;
        s32 rem = self->chunk.size - ((s32)(ptr + used - 8) - (s32)(uintptr_t)self->chunk.p);
        if (rem <= 0x800) {
            SJ_SplitChunk(&self->chunk, rem, &self->chunk, &rest_loop);
            ((void (*)(void*, s32, MpvSjChunk*))sj->vtbl->f[8])(sj, 0, &self->chunk);
            ((void (*)(void*, s32, MpvSjChunk*))sj->vtbl->f[7])(sj, 1, &rest_loop);
            ((void (*)(void*, s32, s32, MpvSjChunk*))sj->vtbl->f[6])(sj, 1, 0x7fffffff, &self->chunk);
            mpvdec_Refill(self, byte_off, &hi, &lo, &bc, &ptr);
        }
        first = 0;
    }

    SJ_SplitChunk(&self->chunk,
                  (s32)(ptr + ((bc + 7) >> 3) - 8) - (s32)(uintptr_t)self->chunk.p,
                  &self->chunk, &rest_end);
    ((void (*)(void*, s32, MpvSjChunk*))sj->vtbl->f[8])(sj, 0, &self->chunk);
    ((void (*)(void*, s32, MpvSjChunk*))sj->vtbl->f[7])(sj, 1, &rest_end);
    return MPV_GoNextDelimSj(sj);
}
