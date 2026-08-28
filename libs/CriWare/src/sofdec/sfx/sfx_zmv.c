// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfx/sfx_zmv
#include <harness_catalog.h>
#include "libs/CriWare/src/sofdec/sfx/sfx_types.h"

void* memset(void* s, int c, size_t n);

u8 lbl_eu_8061A260[0x270]; /* .bss 0x8061A260 size 0x270 */
const float lbl_eu_8051D218 = 0.0f;
const float lbl_eu_8051D21C = 1.164f;
extern char lbl_eu_8051D220[52];
extern char lbl_eu_8051D254[500];

void SFXZ_Init(void) {
    memset(lbl_eu_8061A260, 0, 0x26c);
    *(u32*)(lbl_eu_8061A260 + 8) = 8;
    *(u32*)(lbl_eu_8061A260 + 4) = 0;
}

void* SFXZ_Create(void) {
    u8* base = lbl_eu_8061A260;
    u8* p = base + 12;
    s32 cnt = *(s32*)(base + 8);
    s32 i;
    for (i = 0; i < cnt; i++) {
        if (*(u32*)p == 0)
            goto found;
        p += 0x4C;
    }
    p = NULL;
found:
    if (p == NULL)
        return p;
    {
        float f0 = lbl_eu_8051D218;
        *(float*)(p + 0x3C) = f0;
        *(float*)(p + 0x40) = f0;
    }
    *(u32*)(p + 0x44) = 0;
    *(u32*)(p + 0x48) = 0;
    *(u32*)(p + 0x04) = 0;
    (*(u32*)lbl_eu_8061A260)++;
    *(u32*)p = 1;
    return p;
}

void SFXZ_Destroy(void* self) {
    if (self == NULL) return;
    *(u32*)((u8*)self) = 0;
    *(s32*)lbl_eu_8061A260 -= 1;
}
void sfxzmv_SetTagGrp();
struct SFXZTagInf { u32 unk8; u32 tag; u32 a; u32 b; };
void SFXZ_SetTagInf(void* self, u32 a, u32 b) {
    *(u32*)((u8*)self + 8) = 1;
    *(u32*)((u8*)self + 12) = a;
    *(u32*)((u8*)self + 16) = b;
    sfxzmv_SetTagGrp();
}
extern int SJ_SearchTag(u32* v, const char* a, const char* b, u32* out);

void sfxzmv_SetTagGrp(void* self) {
    struct { u32 a; u32 b; } v;
    u32 out[2];
    if (*(s32*)((u8*)self + 0xC) == 0) {
        *(s32*)((u8*)self + 0x18) = 1;
        *(s32*)((u8*)self + 0x1C) = 0;
        *(s32*)((u8*)self + 0x20) = 0;
        *(s32*)((u8*)self + 0x28) = 1;
        *(s32*)((u8*)self + 0x2C) = 0;
        *(s32*)((u8*)self + 0x30) = 0;
        return;
    }
    v.a = *(u32*)((u8*)self + 0xC);
    v.b = *(u32*)((u8*)self + 0x10);
    SJ_SearchTag((u32*)&v, lbl_eu_8051D254, lbl_eu_8051D254 + 6, &out[0]);
    *(s32*)((u8*)self + 0x18) = 1;
    *(s32*)((u8*)self + 0x1C) = out[0];
    *(s32*)((u8*)self + 0x20) = out[1];
    SJ_SearchTag((u32*)&v, lbl_eu_8051D254 + 14, lbl_eu_8051D254 + 6, &out[0]);
    *(s32*)((u8*)self + 0x28) = 1;
    *(s32*)((u8*)self + 0x2C) = out[0];
    *(s32*)((u8*)self + 0x30) = out[1];
}
extern int sscanf(const char* str, const char* fmt, ...);

// Looks up the "zfrm" tag string for this object, parses the frame offset
// with sscanf, then seeks to (base + offset*a) and parses "min,max" from it.
// On any lookup failure: *out1 = 0 / *out2 = 0x7FFFFFFF (empty range).
typedef struct SFXZObj {
    u8 pad[0x28];
    s32 flag;   // 0x28: tag table valid flag (must be 1)
    s32 tag;    // 0x2C: tag group id
    s32 unk;    // 0x30
} SFXZObj;

// tag-search key pair (group id + sub-key)
typedef struct SFXZKey {
    u32 grp;
    u32 key;
} SFXZKey;

void SFXZ_GetZfrmRange(SFXZObj* self, s32 a, s32* out1, s32* out2) {
    // tag payload format strings; precomputed so the address setup is hoisted
    // above the flag checks (matches retail scheduling)
    char* fmtB = lbl_eu_8051D254 + 6;
    char* fmtA = lbl_eu_8051D254 + 0x15;
    s32 p;
    u32 found1;
    SFXZKey key1;
    u32 found2;
    SFXZKey key2;
    int base;
    int v0;
    int lo;
    int hi;

    if (self->flag != 1 || self->tag == 0) {
        p = 0;
    } else {
        key1.grp = self->tag;
        key1.key = self->unk;
        SJ_SearchTag((u32*)&key1, fmtA, fmtB, &found1);
        p = found1;
    }
    if (p == 0) {
        *out1 = 0;
        *out2 = 0x7FFFFFFF;
    } else {
        sscanf((char*)p, lbl_eu_8051D254 + 0x1D, &base);
        if (self->flag != 1 || self->tag == 0) {
            p = 0;
        } else {
            key2.grp = self->tag;
            key2.key = self->unk;
            SJ_SearchTag((u32*)&key2, lbl_eu_8051D254 + 0x21, lbl_eu_8051D254 + 6, &found2);
            p = found2;
        }
        if (p == 0) {
            *out1 = 0x7FFFFFFF;
        } else {
            sscanf((char*)(p + base * a), lbl_eu_8051D254 + 0x29, &v0, &lo, &hi);
            *out1 = lo;
            *out2 = hi;
        }
    }
}
extern int sscanf(const char* str, const char* fmt, ...);
extern s32 SFX_SetCcirFx(void);
void sfxzmv_MakeOrgZ32TblByDirect(void* self, u32 f1, u32 f0, u32* dst);
void sfxzmv_MakeOrgZ32TblByCCIR(void* self, u32 f1, u32 f0, u32* dst);
void sfxzmv_MakeZ16TblFromOrgZ32(void* self, u16* dst, u32* src);
void sfxzmv_MakeZ32TblFromOrgZ32(void* self, u32* dst, u32* src);

void SFXZ_MakeCnvZTbl(void* self, s32 a, void* buf) {
    u8* dst;
    s32 o1, o0;
    s32 f0, f1;
    SFXZ_GetZfrmRange(self, a, &o1, &o0);
    dst = (u8*)buf + 0x400;
    f0 = o0;
    f1 = o1;
    memset(buf, 0, 0x400);
    if (SFX_SetCcirFx() == 1) {
        sfxzmv_MakeOrgZ32TblByCCIR(self, f1, f0, (u32*)dst);
    } else {
        sfxzmv_MakeOrgZ32TblByDirect(self, f1, f0, (u32*)dst);
    }
    if (*(void**)((u8*)self + 0x44) == NULL) {
        if (*(s32*)((u8*)self + 4) == 16) {
            sfxzmv_MakeZ16TblFromOrgZ32(self, (u16*)buf, (u32*)dst);
        } else {
            sfxzmv_MakeZ32TblFromOrgZ32(self, (u32*)buf, (u32*)dst);
        }
    } else {
        ((void (*)(void*, void*, f32, f32))*(void**)((u8*)self + 0x44))(
            dst, buf, *(f32*)((u8*)self + 0x3C), *(f32*)((u8*)self + 0x40));
    }
}
// Fill dst[0..255] with a quantizer ramp: 9 zeros, 8 entries of f1,
// then a linear ramp from f1 to f0 over entries 17..222 (step = range/207).
// Clamps are unsigned compares against 0x7FFFFFFF (matches retail codegen).
// Textual macro: retail inlines this ramp into both callers with no call.
#define SFXZ_FILL_Z32_RAMP(dPtr) \
    do { \
        s32 i_; \
        u32 step_; \
        if (f0 >= 0x10000U) { \
            f0 = 0x7FFFFFFFU; \
        } \
        if (f1 >= 0x10000U) { \
            f1 = 0x7FFFFFFFU; \
        } \
        for (i_ = 0; i_ < 9; i_++) { \
            (dPtr)[i_] = 0; \
        } \
        for (i_ = 9; i_ < 17; i_++) { \
            (dPtr)[i_] = f1; \
        } \
        if (f1 == f0) { \
            for (i_ = 17; i_ < 223; i_++) { \
                (dPtr)[i_] = f1; \
            } \
        } else { \
            step_ = (f0 - f1) / 207U; \
            for (i_ = 17; i_ < 223; i_++) { \
                (dPtr)[i_] = f1 + (u32)(i_ - 17) * step_; \
            } \
        } \
        for (i_ = 224; i_ < 240; i_++) { \
            (dPtr)[i_] = f0; \
        } \
        for (i_ = 240; i_ < 256; i_++) { \
            (dPtr)[i_] = 0x7FFFFFFFU; \
        } \
    } while (0)

void sfxzmv_MakeOrgZ32TblByDirect(void* self, u32 f1, u32 f0, u32* dst) {
    SFXZ_FILL_Z32_RAMP(dst);
}
// Build a 256-byte CCIR (1.164 scaling) level table at dst+2048, fill the
// same quantizer ramp at dst+256, then remap the 256 linear entries through
// the CCIR table into dst[0..255].
void sfxzmv_MakeOrgZ32TblByCCIR(void* self, u32 f1, u32 f0, u32* dst) {
    s32 i;
    u8* tbl = (u8*)dst + 2048;
    u32* d = dst + 256;

    for (i = 16; i < 236; i++) {
        tbl[i] = (u8)(lbl_eu_8051D21C * (double)(i - 16));
    }
    for (i = 236; i < 256; i++) {
        tbl[i] = 0xFF;
    }
    SFXZ_FILL_Z32_RAMP(d);
    for (i = 0; i < 256; i++) {
        dst[i] = d[tbl[i]];
    }
}
void sfxzmv_MakeZ32TblFromOrgZ32(void* self, u32* dst, u32* src) {
    if (*(u32*)((u8*)lbl_eu_8061A260 + 4) == 1) {
        u32 i, j;
        for (i = 0; i < 16; i++) {
            for (j = 0; j < 16; j++) {
                dst[j] = (src[j] << 1) & 0xFFFFFF00;
            }
            src += 16;
            dst += 16;
        }
    } else {
        f32 zminf = *(f32*)((u8*)self + 0x3C);
        f32 zmaxf = *(f32*)((u8*)self + 0x40);
        double zmax = (double)zmaxf;
        double b = 16777215.0;
        double b1 = b * 1.0;
        double b2 = b * b1;
        double b3 = b1 * b1;
        double b4 = b2 * b3;
        double zmaxb = zmax * b3;
        double two52 = 4503599627370496.0;
        double two31m1 = 2147483647.0;
        u32 i;
        for (i = 0; i < 256; i++) {
            u32 value = *src;
            if (value == 0) {
                *src = 1;
                value = *src;
            }
            {
                union {
                    f64 d;
                    u32 u[2];
                } cv;
                cv.u[0] = 0x43300000;
                cv.u[1] = value;
                {
                    double v = cv.d - two31m1;
                    double t = zmax * v;
                    double u = t / two52;
                    double w = zmaxb / u;
                    double r = b4 - w;
                    *dst = (u32)r;
                }
            }
            src++;
            dst++;
        }
    }
}

// Double-constant pool shared by the Z16/Z32 converters (lbl_eu_8051D218).
typedef struct SFXZDblTbl {
    u8 pad[0x10];
    double base;    // +0x10
    double scale;   // +0x18
    double two52;   // +0x20
    double two31m1; // +0x28
} SFXZDblTbl;

// zmin/zmax are loaded before the mode test in retail (hoisted lfs), so the
// field reads sit above the branch here too.
void sfxzmv_MakeZ16TblFromOrgZ32(void* self, u16* dst, u32* src) {
    f32 zmin = *(f32*)((u8*)self + 0x3C);
    f32 zmax = *(f32*)((u8*)self + 0x40);
    if (*(u32*)((u8*)lbl_eu_8061A260 + 4) == 1) {
        u32 i, j;
        for (i = 0; i < 16; i++) {
            for (j = 0; j < 16; j++) {
                dst[j] = (u16)(src[j] >> 15);
            }
            src += 16;
            dst += 16;
        }
    } else {
        SFXZDblTbl* ct = (SFXZDblTbl*)(void*)&lbl_eu_8051D218;
        double zd = (double)zmax;
        double range = zd - (double)zmin;
        double f6 = ct->base / range;
        double k0 = ct->scale * f6 * (double)zmin;
        double k1 = ct->scale * zd * f6;
        double num = zd * k0;
        u32 i;
        for (i = 0; i < 256; i++) {
            u32 value = *src;
            if (value == 0) {
                *src = 1;
                value = *src;
            }
            {
                union {
                    f64 d;
                    u32 u[2];
                } cv;
                cv.u[0] = 0x43300000;
                cv.u[1] = value;
                {
                    double v = cv.d - ct->two31m1;
                    double t = zd * v;
                    double u = t / ct->two52;
                    double w = num / u;
                    double r = k1 - w;
                    *dst++ = (u16)(s32)r;
                }
            }
            src++;
        }
    }
}

__declspec(section ".rodata") char lbl_eu_8051D220[52] = {
    0x43,0x30,0x00,0x00,0x80,0x00,0x00,0x00,0x3F,0xF0,0x00,0x00,
    0x00,0x00,0x00,0x00,0x40,0xEF,0xFF,0xE0,0x00,0x00,0x00,0x00,
    0x41,0xDF,0xFF,0xFF,0xFF,0xC0,0x00,0x00,0x43,0x30,0x00,0x00,
    0x00,0x00,0x00,0x00,0x41,0x6F,0xFF,0xFF,0xE0,0x00,0x00,0x00,
    0xC1,0x60,0x00,0x00,
};

__declspec(section ".rodata") char lbl_eu_8051D254[500] = {
    0x5A,0x4D,0x48,0x44,0x52,0x00,0x53,0x46,0x58,0x49,0x4E,0x46,
    0x45,0x00,0x5A,0x4D,0x56,0x46,0x52,0x4D,0x00,0x5A,0x4D,0x46,
    0x53,0x49,0x5A,0x45,0x00,0x25,0x6C,0x78,0x00,0x5A,0x4D,0x46,
    0x44,0x41,0x54,0x41,0x00,0x25,0x6C,0x78,0x20,0x25,0x6C,0x78,
    0x20,0x25,0x6C,0x78,0x00,0x45,0x32,0x30,0x31,0x33,0x31,0x34,
    0x3A,0x20,0x53,0x46,0x58,0x5A,0x5F,0x53,0x65,0x74,0x5A,0x63,
    0x6C,0x69,0x70,0x20,0x3A,0x20,0x7A,0x63,0x6C,0x69,0x70,0x20,
    0x69,0x73,0x20,0x69,0x6E,0x76,0x61,0x6C,0x69,0x64,0x2E,0x00,
    0x43,0x4F,0x4D,0x50,0x4F,0x00,0x45,0x32,0x30,0x31,0x33,0x31,
    0x35,0x3A,0x20,0x73,0x66,0x78,0x63,0x6E,0x76,0x5F,0x43,0x6E,
    0x76,0x46,0x72,0x6D,0x5A,0x63,0x6D,0x6E,0x20,0x3A,0x20,0x7A,
    0x63,0x6C,0x69,0x70,0x20,0x69,0x73,0x20,0x6E,0x6F,0x74,0x20,
    0x73,0x65,0x74,0x2E,0x00,0x45,0x32,0x30,0x31,0x31,0x39,0x31,
    0x3A,0x20,0x53,0x46,0x58,0x5F,0x43,0x6E,0x76,0x46,0x72,0x6D,
    0x5A,0x63,0x6D,0x6E,0x20,0x3A,0x20,0x66,0x72,0x6D,0x66,0x6D,
    0x74,0x20,0x69,0x73,0x20,0x6E,0x6F,0x74,0x20,0x73,0x75,0x70,
    0x70,0x6F,0x72,0x74,0x2E,0x00,0x45,0x30,0x35,0x30,0x37,0x30,
    0x38,0x31,0x30,0x3A,0x20,0x70,0x6C,0x65,0x61,0x73,0x65,0x20,
    0x73,0x65,0x74,0x20,0x70,0x69,0x74,0x63,0x68,0x20,0x62,0x79,
    0x20,0x6D,0x77,0x50,0x6C,0x79,0x46,0x78,0x53,0x65,0x74,0x4F,
    0x75,0x74,0x42,0x75,0x66,0x50,0x69,0x74,0x63,0x68,0x48,0x65,
    0x69,0x67,0x68,0x74,0x28,0x29,0x2E,0x00,0x45,0x30,0x35,0x30,
    0x37,0x30,0x38,0x31,0x31,0x3A,0x20,0x70,0x6C,0x65,0x61,0x73,
    0x65,0x20,0x73,0x65,0x74,0x20,0x70,0x69,0x74,0x63,0x68,0x20,
    0x62,0x79,0x20,0x6D,0x77,0x50,0x6C,0x79,0x46,0x78,0x53,0x65,
    0x74,0x4F,0x75,0x74,0x42,0x75,0x66,0x50,0x69,0x74,0x63,0x68,
    0x48,0x65,0x69,0x67,0x68,0x74,0x28,0x29,0x2E,0x00,0x45,0x32,
    0x30,0x31,0x33,0x31,0x33,0x3A,0x20,0x73,0x66,0x78,0x63,0x6E,
    0x76,0x5F,0x43,0x6E,0x76,0x5A,0x62,0x69,0x74,0x54,0x6F,0x43,
    0x66,0x74,0x20,0x3A,0x20,0x7A,0x62,0x69,0x74,0x20,0x69,0x73,
    0x20,0x69,0x6E,0x76,0x61,0x6C,0x69,0x64,0x2E,0x00,0x45,0x30,
    0x35,0x30,0x37,0x30,0x38,0x31,0x32,0x3A,0x20,0x70,0x6C,0x65,
    0x61,0x73,0x65,0x20,0x73,0x65,0x74,0x20,0x70,0x69,0x74,0x63,
    0x68,0x20,0x62,0x79,0x20,0x6D,0x77,0x50,0x6C,0x79,0x46,0x78,
    0x53,0x65,0x74,0x4F,0x75,0x74,0x42,0x75,0x66,0x50,0x69,0x74,
    0x63,0x68,0x48,0x65,0x69,0x67,0x68,0x74,0x28,0x29,0x2E,0x00,
    0x45,0x30,0x35,0x30,0x37,0x30,0x38,0x31,0x33,0x3A,0x20,0x70,
    0x6C,0x65,0x61,0x73,0x65,0x20,0x73,0x65,0x74,0x20,0x70,0x69,
    0x74,0x63,0x68,0x20,0x62,0x79,0x20,0x6D,0x77,0x50,0x6C,0x79,
    0x46,0x78,0x53,0x65,0x74,0x4F,0x75,0x74,0x42,0x75,0x66,0x50,
    0x69,0x74,0x63,0x68,0x48,0x65,0x69,0x67,0x68,0x74,0x28,0x29,
    0x2E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

