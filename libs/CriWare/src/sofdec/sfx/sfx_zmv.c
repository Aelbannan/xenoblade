// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfx/sfx_zmv
#include <harness_catalog.h>
#include "libs/CriWare/src/sofdec/sfx/sfx_types.h"

void* memset(void* s, int c, size_t n);

SFXZmvGlobal sfxz_work_area; /* .bss 0x8061A260 size 0x270 */ // name-guess: TU-owned 0x270 bss arena indexed as 8 x 0x4C SFXZ objects
extern void MWSFSVM_Error(const char* message, ...);

void SFXZ_Init(void) {
    memset(&sfxz_work_area, 0, 0x26c);
    sfxz_work_area.maxCount = 8;
    sfxz_work_area._04 = 0;
}

void* SFXZ_Create(void) {
    SFXZmvSlot* p = sfxz_work_area.slots;
    s32 cnt = sfxz_work_area.maxCount;
    s32 i;
    for (i = 0; i < cnt; i++) {
        if (p->active == 0)
            goto found;
        p++;
    }
    p = NULL;
found:
    if (p == NULL)
        return p;
    {
        float f0 = 0.0f;
        p->_3C = f0;
        p->_40 = f0;
    }
    p->_44 = 0;
    p->_48 = 0;
    p->_04 = 0;
    sfxz_work_area.count++;
    p->active = 1;
    return p;
}

void SFXZ_Destroy(SFXZmvSlot* self) {
    if (self == NULL) return;
    self->active = 0;
    sfxz_work_area.count -= 1;
}
void sfxzmv_SetTagGrp();
void SFXZ_SetTagInf(SFXZmvSlot* self, u32 a, u32 b) {
    self->_08 = 1;
    self->_0C = a;
    self->_10 = b;
    sfxzmv_SetTagGrp();
}
extern int SJ_SearchTag(u32* v, const char* a, const char* b, u32* out);

void sfxzmv_SetTagGrp(SFXZmvSlot* self) {
    struct { u32 a; u32 b; } v;
    u32 out[2];
    if (self->_0C == 0) {
        self->_18 = 1;
        self->_1C = 0;
        self->_20 = 0;
        self->_28 = 1;
        self->_2C = 0;
        self->_30 = 0;
        return;
    }
    v.a = self->_0C;
    v.b = self->_10;
    SJ_SearchTag((u32*)&v, "ZMHDR", "SFXINFE", &out[0]);
    self->_18 = 1;
    self->_1C = out[0];
    self->_20 = out[1];
    SJ_SearchTag((u32*)&v, "ZMVFRM", "SFXINFE", &out[0]);
    self->_28 = 1;
    self->_2C = out[0];
    self->_30 = out[1];
}
extern int sscanf(const char* str, const char* fmt, ...);

// Looks up the "zfrm" tag string for this object, parses the frame offset
// with sscanf, then seeks to (base + offset*a) and parses "min,max" from it.
// On any lookup failure: *out1 = 0 / *out2 = 0x7FFFFFFF (empty range).

// tag-search key pair (group id + sub-key)
typedef struct SFXZKey {
    u32 grp;
    u32 key;
} SFXZKey;

void SFXZ_GetZfrmRange(SFXZmvSlot* self, s32 a, s32* out1, s32* out2) {
    // tag payload format strings; precomputed so the address setup is hoisted
    // above the flag checks (matches retail scheduling)
    char* fmtB = "SFXINFE";
    char* fmtA = "ZMFSIZE";
    s32 p;
    u32 found1;
    SFXZKey key1;
    u32 found2;
    SFXZKey key2;
    int base;
    int v0;
    int lo;
    int hi;

    if (self->_28 != 1 || self->_2C == 0) {
        p = 0;
    } else {
        key1.grp = self->_2C;
        key1.key = self->_30;
        SJ_SearchTag((u32*)&key1, fmtA, fmtB, &found1);
        p = found1;
    }
    if (p == 0) {
        *out1 = 0;
        *out2 = 0x7FFFFFFF;
    } else {
        sscanf((char*)p, "%lx", &base);
        if (self->_28 != 1 || self->_2C == 0) {
            p = 0;
        } else {
            key2.grp = self->_2C;
            key2.key = self->_30;
            SJ_SearchTag((u32*)&key2, "ZMFDATA", "SFXINFE", &found2);
            p = found2;
        }
        if (p == 0) {
            *out1 = 0x7FFFFFFF;
        } else {
            sscanf((char*)(p + base * a), "%lx %lx %lx", &v0, &lo, &hi);
            *out1 = lo;
            *out2 = hi;
        }
    }
}
extern int sscanf(const char* str, const char* fmt, ...);
extern s32 SFX_SetCcirFx(void);
void sfxzmv_MakeOrgZ32TblByDirect(SFXZmvSlot* self, u32 f1, u32 f0, u32* dst);
void sfxzmv_MakeOrgZ32TblByCCIR(SFXZmvSlot* self, u32 f1, u32 f0, u32* dst);
void sfxzmv_MakeZ16TblFromOrgZ32(SFXZmvSlot* self, u32* src, u16* dst);
void sfxzmv_MakeZ32TblFromOrgZ32(SFXZmvSlot* self, u32* src, u32* dst);

void SFXZ_MakeCnvZTbl(void* self, s32 a, void* buf) {
    SFXZmvSlot* slot = self;
    s32 f1, f0;
    s32 o1, o0;
    u8* org;
    SFXZ_GetZfrmRange(self, a, &o1, &o0);
    org = (u8*)buf + 0x400;
    f0 = o0;
    f1 = o1;
    memset(buf, 0, 0x400);
    if (SFX_SetCcirFx() == 1) {
        sfxzmv_MakeOrgZ32TblByCCIR(self, f1, f0, (u32*)org);
    } else {
        sfxzmv_MakeOrgZ32TblByDirect(self, f1, f0, (u32*)org);
    }
    if (slot->_44 == NULL) {
        if (slot->_04 == 16) {
            sfxzmv_MakeZ16TblFromOrgZ32(self, (u32*)org, (u16*)buf);
        } else {
            sfxzmv_MakeZ32TblFromOrgZ32(self, (u32*)org, (u32*)buf);
        }
    } else {
        slot->_44(org, buf, slot->_3C, slot->_40);
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

void sfxzmv_MakeOrgZ32TblByDirect(SFXZmvSlot* self, u32 f1, u32 f0, u32* dst) {
    SFXZ_FILL_Z32_RAMP(dst);
}
// Build a 256-byte CCIR (1.164 scaling) level table at dst+2048, fill the
// same quantizer ramp at dst+256, then remap the 256 linear entries through
// the CCIR table into dst[0..255].
void sfxzmv_MakeOrgZ32TblByCCIR(SFXZmvSlot* self, u32 f1, u32 f0, u32* dst) {
    s32 i;
    f32 scale = 1.164f;
    u8* tbl = (u8*)dst + 2048;
    u32* d = dst + 256;

    for (i = 16; i < 236; i++) {
        tbl[i] = (u8)(scale * (double)(i - 16));
    }
    for (i = 236; i < 256; i++) {
        tbl[i] = 0xFF;
    }
    SFXZ_FILL_Z32_RAMP(d);
    for (i = 0; i < 256; i++) {
        dst[i] = d[tbl[i]];
    }
}
// zmin/zmax are loaded before the mode test in retail (hoisted lfs), so the
// field reads sit above the branch here too.
void sfxzmv_MakeZ16TblFromOrgZ32(SFXZmvSlot* self, u32* src, u16* dst) {
    f32 zmin = self->_3C;
    f32 zmax = self->_40;
    if (sfxz_work_area._04 == 1) {
        u32 i, j;
        for (i = 0; i < 16; i++) {
            for (j = 0; j < 16; j++) {
                dst[j] = (u16)(src[j] >> 15);
            }
            src += 16;
            dst += 16;
        }
    } else {
        double zd = (double)zmax;
        double range = zd - (double)zmin;
        double f6 = 1.0 / range;
        double k0 = 65535.0 * f6 * (double)zmin;
        double k1 = 65535.0 * zd * f6;
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
                    double v = cv.d - 2147483647.0;
                    double t = zd * v;
                    double u = t / 4503599627370496.0;
                    double w = num / u;
                    double r = k1 - w;
                    *dst++ = (u16)(s32)r;
                }
            }
            src++;
        }
    }
}



void sfxzmv_MakeZ32TblFromOrgZ32(SFXZmvSlot* self, u32* src, u32* dst) {
    if (sfxz_work_area._04 == 1) {
        u32 i, j;
        for (i = 0; i < 16; i++) {
            for (j = 0; j < 16; j++) {
                dst[j] = (src[j] << 1) & 0xFFFFFF00;
            }
            src += 16;
            dst += 16;
        }
    } else {
        f32 zminf = self->_3C;
        f32 zmaxf = self->_40;
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

// name-guess: these non-retail-named functions are kept out of the linked
// game; their error strings are the orphan tail of this TU's retail string
// pool (RECIPE D / dead-stripped functions).
// name-guess: SFXZ_SetZclip is named by the retail E201314 error string; the
// -14.0f zclip bound is the orphan float at retail .rodata+0x38.
void SFXZ_SetZclip(f32 zclip) {
    if (zclip < -14.0f) {
        MWSFSVM_Error("E201314: SFXZ_SetZclip : zclip is invalid.");
    }
}

const char* sfxz_GetCompoTagName(void) {
    return "COMPO";
}

void sfxz_ReportZclipNotSet(void) {
    MWSFSVM_Error("E201315: sfxcnv_CnvFrmZcmn : zclip is not set.");
}

void sfxz_ReportFrmfmtUnsupported(void) {
    MWSFSVM_Error("E201191: SFX_CnvFrmZcmn : frmfmt is not support.");
}

void sfxz_ReportPitchHeight10(void) {
    MWSFSVM_Error("E05070810: please set pitch by mwPlyFxSetOutBufPitchHeight().");
    MWSFSVM_Error("E05070811: please set pitch by mwPlyFxSetOutBufPitchHeight().");
}

void sfxz_ReportInvalidZbit(void) {
    MWSFSVM_Error("E201313: sfxcnv_CnvZbitToCft : zbit is invalid.");
}

void sfxz_ReportPitchHeight12(void) {
    MWSFSVM_Error("E05070812: please set pitch by mwPlyFxSetOutBufPitchHeight().");
    MWSFSVM_Error("E05070813: please set pitch by mwPlyFxSetOutBufPitchHeight().");
}
