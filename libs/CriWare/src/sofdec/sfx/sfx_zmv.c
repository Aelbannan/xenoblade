// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfx/sfx_zmv
#include <harness_catalog.h>
#include "libs/CriWare/src/sofdec/sfx/sfx_types.h"

void* memset(void* s, int c, size_t n);

extern u8 lbl_eu_8061A260[];
extern float lbl_eu_8051D218;

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
        if (*(u32*)p != 0)
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
extern char lbl_eu_8051D254[];
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

int SFXZ_GetZfrmRange(void* self, s32 a, s32* out1, s32* out0) {
    const char* tag6a = lbl_eu_8051D254 + 6;
    const char* tag21 = lbl_eu_8051D254 + 21;
    s32 out1v;
    u32 v1[2];
    s32 t;
    s32 out2v;
    u32 v2[2];
    s32 u1, u2, u3;

    char* p;
    if (*(s32*)((u8*)self + 0x28) != 1 || *(s32*)((u8*)self + 0x2C) == 0) {
        out1v = 0;
    } else {
        v1[0] = *(u32*)((u8*)self + 0x2C);
        v1[1] = *(u32*)((u8*)self + 0x30);
        SJ_SearchTag(v1, tag21, tag6a, (u32*)&out1v);
    }
    p = (char*)out1v;
    if (p == 0) {
        *out1 = 0;
        *out0 = 0x7FFFFFFF;
    } else {
        sscanf(p, lbl_eu_8051D254 + 29, &t);
        if (*(s32*)((u8*)self + 0x28) != 1 || *(s32*)((u8*)self + 0x2C) == 0) {
            out2v = 0;
        } else {
            v2[0] = *(u32*)((u8*)self + 0x2C);
            v2[1] = *(u32*)((u8*)self + 0x30);
            SJ_SearchTag(v2, lbl_eu_8051D254 + 33, lbl_eu_8051D254 + 6, (u32*)&out2v);
        }
        if (out2v == 0) {
            *out1 = 0x7FFFFFFF;
        } else {
            sscanf((const char*)(out2v + t * a), lbl_eu_8051D254 + 41, &u1, &u2, &u3);
            *out1 = u2;
            *out0 = u3;
        }
    }
}
extern int sscanf(const char* str, const char* fmt, ...);
extern s32 SFX_SetCcirFx(void);
void sfxzmv_MakeOrgZ32TblByDirect(void* self, s32 f1, s32 f0, s32* dst);
void sfxzmv_MakeOrgZ32TblByCCIR(void* self, s32 f1, s32 f0, s32* dst);
void sfxzmv_MakeZ16TblFromOrgZ32(void* self, u16* dst, u32* src);
void sfxzmv_MakeZ32TblFromOrgZ32(void* self, u32* dst, u32* src);

void SFXZ_MakeCnvZTbl(void* self, s32 a, void* buf) {
    s32 o1, o0;
    u8* dst;
    s32 f0, f1;
    SFXZ_GetZfrmRange(self, a, &o1, &o0);
    dst = (u8*)buf + 0x400;
    f0 = o0;
    f1 = o1;
    memset(buf, 0, 0x400);
    if (SFX_SetCcirFx() == 1) {
        sfxzmv_MakeOrgZ32TblByCCIR(self, f1, f0, (s32*)dst);
    } else {
        sfxzmv_MakeOrgZ32TblByDirect(self, f1, f0, (s32*)dst);
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
void sfxzmv_MakeOrgZ32TblByDirect(void* self, s32 f1, s32 f0, s32* dst) {
    s32 i;
    if (f0 < 0) {
        f0 = 0x7FFFFFFF;
    }
    if (f1 < 0) {
        f1 = 0x7FFFFFFF;
    }
    for (i = 0; i < 9; i++) {
        dst[i] = 0;
    }
    for (i = 9; i < 17; i++) {
        dst[i] = f1;
    }
    if (f1 == f0) {
        for (i = 17; i < 224; i++) {
            dst[i] = f1;
        }
    } else {
        s32 step = (s32)((u32)(f0 - f1) / 207);
        for (i = 17; i < 224; i += 8) {
            dst[i + 0] = f1 + (i - 17) * step;
            dst[i + 1] = f1 + (i - 16) * step;
            dst[i + 2] = f1 + (i - 15) * step;
            dst[i + 3] = f1 + (i - 14) * step;
            dst[i + 4] = f1 + (i - 13) * step;
            dst[i + 5] = f1 + (i - 12) * step;
            dst[i + 6] = f1 + (i - 11) * step;
            dst[i + 7] = f1 + (i - 10) * step;
        }
        dst[217] = f1 + 200 * step;
        dst[218] = f1 + 201 * step;
        dst[219] = f1 + 202 * step;
        dst[220] = f1 + 203 * step;
        dst[221] = f1 + 204 * step;
        dst[222] = f1 + 205 * step;
        dst[223] = f1 + 206 * step;
    }
    for (i = 224; i < 240; i++) {
        dst[i] = f0;
    }
    for (i = 240; i < 256; i++) {
        dst[i] = 0x7FFFFFFF;
    }
}
void sfxzmv_MakeOrgZ32TblByCCIR(void* self, s32 f1, s32 f0, s32* dst) {
    s32 i;
    for (i = 0; i < 16; i++) {
        ((u8*)dst)[2048 + i] = 0;
    }
    for (i = 16; i < 236; i++) {
        ((u8*)dst)[2048 + i] = (u8)(s32)(1.1640000343322754f * (float)(i - 16));
    }
    for (i = 236; i < 256; i++) {
        ((u8*)dst)[2048 + i] = 0xFF;
    }
    {
        s32* d = dst + 256;
        if (f0 < 0) {
            f0 = 0x7FFFFFFF;
        }
        if (f1 < 0) {
            f1 = 0x7FFFFFFF;
        }
        for (i = 0; i < 9; i++) {
            d[i] = 0;
        }
        for (i = 9; i < 17; i++) {
            d[i] = f1;
        }
        if (f1 == f0) {
            for (i = 17; i < 224; i++) {
                d[i] = f1;
            }
        } else {
            s32 step = (s32)((u32)(f0 - f1) / 207);
            for (i = 17; i < 224; i += 8) {
                d[i + 0] = f1 + (i - 17) * step;
                d[i + 1] = f1 + (i - 16) * step;
                d[i + 2] = f1 + (i - 15) * step;
                d[i + 3] = f1 + (i - 14) * step;
                d[i + 4] = f1 + (i - 13) * step;
                d[i + 5] = f1 + (i - 12) * step;
                d[i + 6] = f1 + (i - 11) * step;
                d[i + 7] = f1 + (i - 10) * step;
            }
            d[217] = f1 + 200 * step;
            d[218] = f1 + 201 * step;
            d[219] = f1 + 202 * step;
            d[220] = f1 + 203 * step;
            d[221] = f1 + 204 * step;
            d[222] = f1 + 205 * step;
            d[223] = f1 + 206 * step;
        }
        for (i = 224; i < 240; i++) {
            d[i] = f0;
        }
        for (i = 240; i < 256; i++) {
            d[i] = 0x7FFFFFFF;
        }
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

void sfxzmv_MakeZ16TblFromOrgZ32(void* self, u16* dst, u32* src) {
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
        f32 zmaxf = *(f32*)((u8*)self + 0x40);
        double zmax = (double)zmaxf;
        double b = 65535.0;
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
                    dst[i] = (u16)(s32)r;
                }
            }
            src++;
        }
    }
}
