#include <revolution/GX.h>

#include <string.h>

#define GX_PHY_ADDR(a) ((u32)(a) & 0x3FFFFFFF)

#define TX_MIN_NEAREST 0
#define TX_MIN_NEAREST_MIPMAP_NEAREST 1
#define TX_MIN_NEAREST_MIPMAP_LINEAR 2
#define TX_MIN_LINEAR 4
#define TX_MIN_LINEAR_MIPMAP_NEAREST 5
#define TX_MIN_LINEAR_MIPMAP_LINEAR 6

#define TX_LOADBLOCK3_COUNT_MASK 0x00007FFF

#define TX_SETMODE0_I0_ID 0x80
#define TX_SETMODE0_I1_ID 0x81
#define TX_SETMODE0_I2_ID 0x82
#define TX_SETMODE0_I3_ID 0x83
#define TX_SETMODE1_I0_ID 0x84
#define TX_SETMODE1_I1_ID 0x85
#define TX_SETMODE1_I2_ID 0x86
#define TX_SETMODE1_I3_ID 0x87
#define TX_SETMODE0_I4_ID 0xA0
#define TX_SETMODE0_I5_ID 0xA1
#define TX_SETMODE0_I6_ID 0xA2
#define TX_SETMODE0_I7_ID 0xA3
#define TX_SETMODE1_I4_ID 0xA4
#define TX_SETMODE1_I5_ID 0xA5
#define TX_SETMODE1_I6_ID 0xA6
#define TX_SETMODE1_I7_ID 0xA7
#define TX_SETIMAGE0_I0_ID 0x88
#define TX_SETIMAGE0_I1_ID 0x89
#define TX_SETIMAGE0_I2_ID 0x8A
#define TX_SETIMAGE0_I3_ID 0x8B
#define TX_SETIMAGE1_I0_ID 0x8C
#define TX_SETIMAGE1_I1_ID 0x8D
#define TX_SETIMAGE1_I2_ID 0x8E
#define TX_SETIMAGE1_I3_ID 0x8F
#define TX_SETIMAGE2_I0_ID 0x90
#define TX_SETIMAGE2_I1_ID 0x91
#define TX_SETIMAGE2_I2_ID 0x92
#define TX_SETIMAGE2_I3_ID 0x93
#define TX_SETIMAGE3_I0_ID 0x94
#define TX_SETIMAGE3_I1_ID 0x95
#define TX_SETIMAGE3_I2_ID 0x96
#define TX_SETIMAGE3_I3_ID 0x97
#define TX_SETIMAGE0_I4_ID 0xA8
#define TX_SETIMAGE0_I5_ID 0xA9
#define TX_SETIMAGE0_I6_ID 0xAA
#define TX_SETIMAGE0_I7_ID 0xAB
#define TX_SETIMAGE1_I4_ID 0xAC
#define TX_SETIMAGE1_I5_ID 0xAD
#define TX_SETIMAGE1_I6_ID 0xAE
#define TX_SETIMAGE1_I7_ID 0xAF
#define TX_SETIMAGE2_I4_ID 0xB0
#define TX_SETIMAGE2_I5_ID 0xB1
#define TX_SETIMAGE2_I6_ID 0xB2
#define TX_SETIMAGE2_I7_ID 0xB3
#define TX_SETIMAGE3_I4_ID 0xB4
#define TX_SETIMAGE3_I5_ID 0xB5
#define TX_SETIMAGE3_I6_ID 0xB6
#define TX_SETIMAGE3_I7_ID 0xB7
#define TX_SETTLUT_I0_ID 0x98
#define TX_SETTLUT_I1_ID 0x99
#define TX_SETTLUT_I2_ID 0x9A
#define TX_SETTLUT_I3_ID 0x9B
#define TX_SETTLUT_I4_ID 0xB8
#define TX_SETTLUT_I5_ID 0xB9
#define TX_SETTLUT_I6_ID 0xBA
#define TX_SETTLUT_I7_ID 0xBB

#define SC_TX_SETMODE0_SET_WRAP_S(reg, v) \
    ((reg) = (((reg) & ~0x3u) | (u32)(v)))
#define SC_TX_SETMODE0_SET_WRAP_T(reg, v) \
    ((reg) = (((reg) & ~0xCu) | ((u32)(v) << 2)))
#define SC_TX_SETMODE0_SET_MAG_FILTER(reg, v) \
    ((reg) = (((reg) & ~0x10u) | ((u32)(v) << 4)))
#define SC_TX_SETMODE0_SET_MIN_FILTER(reg, v) \
    ((reg) = (((reg) & ~0xE0u) | ((u32)(v) << 5)))
#define SC_TX_SETMODE0_SET_LODBIAS(reg, v) \
    ((reg) = (((reg) & ~0x0001FE00u) | ((u32)(v) << 9)))
#define SC_TX_SETMODE0_SET_DIAGLOD_ENABLE(reg, v) \
    ((reg) = (((reg) & ~0x00000100u) | ((u32)(v) << 8)))
#define SC_TX_SETMODE0_SET_ROUND(reg, v) \
    ((reg) = (((reg) & ~0x00020000u) | ((u32)(v) << 17)))
#define SC_TX_SETMODE0_SET_FIELD_PREDICT(reg, v) \
    ((reg) = (((reg) & ~0x00040000u) | ((u32)(v) << 18)))
#define SC_TX_SETMODE0_SET_MAXANISO(reg, v) \
    ((reg) = (((reg) & ~0x00180000u) | ((u32)(v) << 19)))
#define SC_TX_SETMODE0_SET_LODCLAMP(reg, v) \
    ((reg) = (((reg) & ~0x00200000u) | ((u32)(v) << 21)))
#define SC_TX_SETMODE0_SET_RID(reg, v) \
    ((reg) = (((reg) & ~0xFF000000u) | ((u32)(v) << 24)))

#define SC_TX_SETMODE1_SET_MINLOD(reg, v) \
    ((reg) = (((reg) & ~0x000000FFu) | (u32)(v)))
#define SC_TX_SETMODE1_SET_MAXLOD(reg, v) \
    ((reg) = (((reg) & ~0x0000FF00u) | ((u32)(v) << 8)))
#define SC_TX_SETMODE1_SET_RID(reg, v) \
    ((reg) = (((reg) & ~0xFF000000u) | ((u32)(v) << 24)))

#define SC_TX_SETIMAGE0_SET_IMAGE_WIDTH(reg, v) \
    ((reg) = (((reg) & ~0x000003FFu) | (u32)(v)))
#define SC_TX_SETIMAGE0_SET_IMAGE_HEIGHT(reg, v) \
    ((reg) = (((reg) & ~0x000FFC00u) | ((u32)(v) << 10)))
#define SC_TX_SETIMAGE0_SET_FORMAT(reg, v) \
    ((reg) = (((reg) & ~0x00F00000u) | ((u32)(v) << 20)))
#define SC_TX_SETIMAGE0_SET_RID(reg, v) \
    ((reg) = (((reg) & ~0xFF000000u) | ((u32)(v) << 24)))

#define SC_TX_SETIMAGE1_SET_TMEM_OFFSET(reg, v) \
    ((reg) = (((reg) & ~0x007FE000u) | ((u32)(v) << 13)))
#define SC_TX_SETIMAGE1_SET_CACHE_WIDTH(reg, v) \
    ((reg) = (((reg) & ~0x00001800u) | ((u32)(v) << 11)))
#define SC_TX_SETIMAGE1_SET_CACHE_HEIGHT(reg, v) \
    ((reg) = (((reg) & ~0x00000600u) | ((u32)(v) << 9)))
#define SC_TX_SETIMAGE1_SET_IMAGE_TYPE(reg, v) \
    ((reg) = (((reg) & ~0x00000100u) | ((u32)(v) << 8)))
#define SC_TX_SETIMAGE1_SET_RID(reg, v) \
    ((reg) = (((reg) & ~0xFF000000u) | ((u32)(v) << 24)))

#define SC_TX_SETIMAGE2_SET_TMEM_OFFSET(reg, v) \
    ((reg) = (((reg) & ~0x007FE000u) | ((u32)(v) << 13)))
#define SC_TX_SETIMAGE2_SET_CACHE_WIDTH(reg, v) \
    ((reg) = (((reg) & ~0x00001800u) | ((u32)(v) << 11)))
#define SC_TX_SETIMAGE2_SET_CACHE_HEIGHT(reg, v) \
    ((reg) = (((reg) & ~0x00000600u) | ((u32)(v) << 9)))
#define SC_TX_SETIMAGE2_SET_RID(reg, v) \
    ((reg) = (((reg) & ~0xFF000000u) | ((u32)(v) << 24)))

#define SC_TX_SETIMAGE3_SET_IMAGE_BASE(reg, v) \
    ((reg) = (((reg) & ~0x00FFFFFFu) | (u32)(v)))
#define SC_TX_SETIMAGE3_SET_RID(reg, v) \
    ((reg) = (((reg) & ~0xFF000000u) | ((u32)(v) << 24)))

#define SC_TX_SETTLUT_SET_FORMAT(reg, v) \
    ((reg) = (((reg) & ~0x000C0000u) | ((u32)(v) << 18)))
#define SC_TX_SETTLUT_SET_TMEM_OFFSET(reg, v) \
    ((reg) = (((reg) & ~0x0003FC00u) | ((u32)(v) << 10)))
#define SC_TX_SETTLUT_SET_RID(reg, v) \
    ((reg) = (((reg) & ~0xFF000000u) | ((u32)(v) << 24)))

#define SC_TX_LOADTLUT0_SET_TLUT_BASE(reg, v) \
    ((reg) = (((reg) & ~0x00FFFFE0u) | ((u32)(v) << 5)))
#define SC_TX_LOADTLUT0_SET_RID(reg, v) \
    ((reg) = (((reg) & ~0xFF000000u) | ((u32)(v) << 24)))

#define SC_TX_LOADTLUT1_SET_TMEM_OFFSET(reg, v) \
    ((reg) = (((reg) & ~0x000001FFu) | (u32)(v)))
#define SC_TX_LOADTLUT1_SET_COUNT(reg, v) \
    ((reg) = (((reg) & ~0x001FFC00u) | ((u32)(v) << 10)))
#define SC_TX_LOADTLUT1_SET_RID(reg, v) \
    ((reg) = (((reg) & ~0xFF000000u) | ((u32)(v) << 24)))

#define TX_SETMODE0_GET_WRAP_S(reg) ((reg) & 0x3u)
#define TX_SETMODE0_GET_WRAP_T(reg) (((reg) & 0xCu) >> 2)
#define TX_SETMODE0_GET_MAG_FILTER(reg) (((reg) & 0x10u) >> 4)
#define TX_SETMODE0_GET_MIN_FILTER(reg) (((reg) & 0xE0u) >> 5)
#define TX_SETMODE0_GET_LODBIAS(reg) (((reg) & 0x0001FE00u) >> 9)
#define TX_SETMODE0_GET_DIAGLOD_ENABLE(reg) (((reg) & 0x00000100u) >> 8)
#define TX_SETMODE0_GET_LODCLAMP(reg) (((reg) & 0x00200000u) >> 21)
#define TX_SETMODE0_GET_MAXANISO(reg) (((reg) & 0x00180000u) >> 19)
#define TX_SETMODE1_GET_MINLOD(reg) ((reg) & 0xFFu)
#define TX_SETMODE1_GET_MAXLOD(reg) (((reg) & 0x0000FF00u) >> 8)
#define TX_SETIMAGE0_GET_IMAGE_WIDTH(reg) ((reg) & 0x3FFu)
#define TX_SETIMAGE0_GET_IMAGE_HEIGHT(reg) (((reg) & 0x000FFC00u) >> 10)
#define TX_SETIMAGE3_GET_IMAGE_BASE(reg) ((reg) & 0x00FFFFFFu)
#define TX_LOADTLUT1_GET_TMEM_OFFSET(reg) ((reg) & 0x1FFu)

#define TX_SETIMAGE1(tmem_offset, cache_width, cache_height, image_type, rid) \
    (((u32)(tmem_offset) << 13) | ((u32)(cache_width) << 11) | \
     ((u32)(cache_height) << 9) | ((u32)(image_type) << 8) | ((u32)(rid) << 24))

#define TX_SETIMAGE2(tmem_offset, cache_width, cache_height, rid) \
    (((u32)(tmem_offset) << 13) | ((u32)(cache_width) << 11) | \
     ((u32)(cache_height) << 9) | ((u32)(rid) << 24))

#define TX_INVALTAGS(tmem_offset, count, rid) \
    (((u32)(tmem_offset)) | ((u32)(count) << 9) | ((u32)(rid) << 24))

#define IsCITexFmt(fmt) \
    ((fmt) == GX_TF_C4 || (fmt) == GX_TF_C8 || (fmt) == GX_TF_C14X2)

static u32 lbl_80665A08[2] = { 0x80818283, 0xA0A1A2A3 };
static u32 lbl_80665A10[2] = { 0x84858687, 0xA4A5A6A7 };
static u32 lbl_80665A18[2] = { 0x88898A8B, 0xA8A9AAAB };
static u32 lbl_80665A20[2] = { 0x8C8D8E8F, 0xACADAEAF };
static u32 lbl_80665A28[2] = { 0x90919293, 0xB0B1B2B3 };
static u32 lbl_80665A30[2] = { 0x94959697, 0xB4B5B6B7 };
static u32 lbl_80665A38[2] = { 0x98999A9B, 0xB8B9BABB };
static u8 lbl_80665A40[] = { 0x00, 0x04, 0x01, 0x05, 0x02, 0x06, 0x00, 0x00 };
static u32 lbl_80665A48[2] = { 0x00020400, 0x01030500 };

static const f32 float_8066C010[2] = { 16.0f, 0.0f };
static const f64 double_8066C018 = 4503599627370496.0;
static const f32 float_8066C020 = -4.0f;
static const f32 float_8066C024 = 3.99f;
static const f32 float_8066C028 = 4.0f;
static const f32 float_8066C02C = 32.0f;
static const f32 float_8066C030 = 0.0f;
static const f32 float_8066C034 = 10.0f;
static const f32 float_8066C038 = 0.0625f;
static const f32 float_8066C03C = 0.03125f;
static const f64 double_8066C040 = 4503601774854144.0;

static const u8 GXTexTileRowShift[61] = {
    3, 3, 3, 2, 2, 2, 2, 0, 3, 3, 2, 0, 0, 0, 3, 0, 0, 3, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 3, 2, 0, 0, 0, 3, 3, 3, 3, 2, 2, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 0, 2,
};

static const u8 GXTexTileColShift[61] = {
    3, 2, 2, 2, 2, 2, 2, 0, 3, 2, 2, 0, 0, 0, 3, 0, 0, 2, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 2, 2, 0, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 2,
};

u32 GXGetTexBufferSize(u16 width, u16 height, u32 format, GXBool mipmap, u8 max_lod) {
    u32 tileShiftX, tileShiftY, tileBytes, bufferSize;
    u32 nx, ny;
    u32 level;

    switch (format) {
    case GX_TF_I4:
    case GX_TF_C4:
    case GX_TF_CMPR:
    case GX_CTF_R4:
    case GX_CTF_Z4:
        tileShiftX = 3;
        tileShiftY = 3;
        break;

    case GX_TF_I8:
    case GX_TF_IA4:
    case GX_TF_C8:
    case GX_TF_Z8:
    case GX_CTF_RA4:
    case GX_CTF_A8:
    case GX_CTF_R8:
    case GX_CTF_G8:
    case GX_CTF_B8:
    case GX_CTF_Z8M:
    case GX_CTF_Z8L:
        tileShiftX = 3;
        tileShiftY = 2;
        break;

    case GX_TF_IA8:
    case GX_TF_RGB565:
    case GX_TF_RGB5A3:
    case GX_TF_RGBA8:
    case GX_TF_C14X2:
    case GX_TF_Z16:
    case GX_TF_Z24X8:
    case GX_CTF_RA8:
    case GX_CTF_RG8:
    case GX_CTF_GB8:
    case GX_CTF_Z16L:
        tileShiftX = 2;
        tileShiftY = 2;
        break;

    default:
        tileShiftY = 0;
        tileShiftX = 0;
        break;
    }

    if (format == GX_TF_RGBA8 || format == GX_TF_Z24X8) {
        tileBytes = 64;
    } else {
        tileBytes = 32;
    }

    if (mipmap == GX_TRUE) {
        bufferSize = 0;

        for (level = 0; level < max_lod; level++) {
            nx = (u32)((width + (1 << tileShiftX) - 1) >> tileShiftX);
            ny = (u32)((height + (1 << tileShiftY) - 1) >> tileShiftY);

            bufferSize += nx * ny * tileBytes;

            if (width == 1 && height == 1) {
                break;
            }

            width = (width > 1) ? (width >> 1) : 1;
            height = (height > 1) ? (height >> 1) : 1;
        }
    } else {
        nx = (u32)((width + (1 << tileShiftX) - 1) >> tileShiftX);
        ny = (u32)((height + (1 << tileShiftY) - 1) >> tileShiftY);

        bufferSize = nx * ny * tileBytes;
    }

    return bufferSize;
}

void __GetImageTileCount(GXTexFmt fmt, u16 wd, u16 ht, u32* rowTiles, u32* colTiles,
                         u32* cmpTiles) {
    u32 texRowShift, texColShift;

    switch (fmt) {
    case GX_TF_I4:
    case GX_TF_C4:
    case GX_TF_CMPR:
    case GX_CTF_R4:
    case GX_CTF_Z4:
        texRowShift = 3;
        texColShift = 3;
        break;

    case GX_TF_I8:
    case GX_TF_IA4:
    case GX_TF_C8:
    case GX_TF_Z8:
    case GX_CTF_RA4:
    case GX_CTF_A8:
    case GX_CTF_R8:
    case GX_CTF_G8:
    case GX_CTF_B8:
    case GX_CTF_Z8M:
    case GX_CTF_Z8L:
        texRowShift = 3;
        texColShift = 2;
        break;

    case GX_TF_IA8:
    case GX_TF_RGB565:
    case GX_TF_RGB5A3:
    case GX_TF_RGBA8:
    case GX_TF_C14X2:
    case GX_TF_Z16:
    case GX_TF_Z24X8:
    case GX_CTF_RA8:
    case GX_CTF_RG8:
    case GX_CTF_GB8:
    case GX_CTF_Z16L:
        texRowShift = 2;
        texColShift = 2;
        break;

    default:
        texColShift = 0;
        texRowShift = 0;
        break;
    }

    if (wd == 0) {
        wd = 1;
    }
    if (ht == 0) {
        ht = 1;
    }
    *rowTiles = (u32)((wd + (1 << texRowShift) - 1) >> texRowShift);
    *colTiles = (u32)((ht + (1 << texColShift) - 1) >> texColShift);
    *cmpTiles = (fmt == GX_TF_RGBA8 || fmt == GX_TF_Z24X8) ? 2U : 1U;
}

void GXInitTexObj(GXTexObj* obj, void* image_ptr, u16 width, u16 height, GXTexFmt format,
                  GXTexWrapMode wrap_s, GXTexWrapMode wrap_t, GXBool mipmap) {
    u32 imageBase;
    u32 maxLOD;
    u32 mode0;
    u32 mode1;
    u32 image0;
    u32 image3;
    u32 fmt;
    u16 rowT, colT;
    u32 rowC, colC;
    GXTexObjImpl* t = (GXTexObjImpl*)obj;

    memset(t, 0, sizeof(GXTexObjImpl));

    mode0 = t->mode0;
    mode0 = __rlwimi(mode0, wrap_s, 0, 30, 31);
    mode0 = __rlwimi(mode0, wrap_t, 2, 28, 29);
    mode0 |= 0x10;
    t->mode0 = mode0;

    if (mipmap) {
        t->flags |= 1;
        if ((u32)(format - 8) <= 2) {
            mode0 = __rlwimi(mode0, 5, 5, 24, 26);
            t->mode0 = mode0;
        } else {
            mode0 = __rlwimi(mode0, 6, 5, 24, 26);
            t->mode0 = mode0;
        }

        if (width > height) {
            maxLOD = (u32)(31 - __cntlzw(width));
        } else {
            maxLOD = (u32)(31 - __cntlzw(height));
        }

        mode1 = t->mode1;
        mode1 = __rlwimi(mode1, (s32)(f32)(float_8066C010[0] * (f32)maxLOD), 8, 16, 23);
        t->mode1 = mode1;
    } else {
        mode0 = __rlwimi(mode0, 4, 5, 24, 26);
        t->mode0 = mode0;
    }

    image0 = t->image0;
    image0 = __rlwimi(image0, width - 1, 0, 22, 31);

    image3 = t->image3;
    image0 = __rlwimi(image0, height - 1, 10, 12, 21);
    image3 = __rlwimi(image3, (GX_PHY_ADDR(image_ptr)) >> 5, 0, 8, 31);

    fmt = format & 0xF;
    image0 = __rlwimi(image0, fmt, 20, 8, 11);
    t->fmt = format;
    t->image0 = image0;
    t->image3 = image3;

    switch (fmt) {
    case GX_TF_I4:
    case GX_TF_C4:
        t->loadFmt = 1;
        rowT = 3;
        colT = 3;
        break;

    case GX_TF_I8:
    case GX_TF_IA4:
    case GX_TF_C8:
        t->loadFmt = 2;
        rowT = 3;
        colT = 2;
        break;

    case GX_TF_IA8:
    case GX_TF_C14X2:
    case GX_TF_RGB565:
    case GX_TF_RGB5A3:
        t->loadFmt = 2;
        rowT = 2;
        colT = 2;
        break;

    case GX_TF_RGBA8:
        t->loadFmt = 3;
        rowT = 2;
        colT = 2;
        break;

    case GX_TF_CMPR:
        t->loadFmt = 0;
        rowT = 3;
        colT = 3;
        break;

    default:
        t->loadFmt = 2;
        rowT = 2;
        colT = 2;
        break;
    }

    rowC = (u32)((width + (1 << rowT) - 1) >> rowT);
    colC = (u32)((height + (1 << colT) - 1) >> colT);
    t->loadCnt = (u16)((rowC * colC) & TX_LOADBLOCK3_COUNT_MASK);
    t->flags |= 2;
}

void GXInitTexObjCI(GXTexObj* obj, void* image_ptr, u16 width, u16 height, GXTexFmt format,
                    GXTexWrapMode wrap_s, GXTexWrapMode wrap_t, GXBool mipmap, u32 tlut_name) {
    /* Retail: snap stack arg into saved GPR before bl; store tlutName before
       clearing flags bit1 (lbz / stw / rlwinm / stb). */
    GXTexObjImpl* t = (GXTexObjImpl*)obj;
    u32 name = tlut_name;
    u8 flags;

    GXInitTexObj(obj, image_ptr, width, height, format, wrap_s, wrap_t, mipmap);

    flags = t->flags;
    t->tlutName = name;
    t->flags = (u8)(flags & ~2);
}

void GXInitTexObjLOD(GXTexObj* obj, GXTexFilter min_filt, GXTexFilter mag_filt, f32 min_lod,
                     f32 max_lod, f32 lod_bias, GXBool bias_clamp, GXBool do_edge_lod,
                     GXAnisotropy max_aniso) {
    s32 lmin;
    s32 lmax;
    u32 mode0;
    u32 mode1;
    GXTexObjImpl* t = (GXTexObjImpl*)obj;

    if (lod_bias < float_8066C020) {
        lod_bias = float_8066C020;
    } else if (lod_bias >= float_8066C028) {
        lod_bias = float_8066C024;
    }

    mode0 = __rlwimi(t->mode0, (s32)(f32)(float_8066C02C * lod_bias), 9, 15, 22);
    mode0 = __rlwimi(mode0, __cntlzw((u32)(mag_filt - 1)), 31, 27, 27);
    t->mode0 = mode0;

    mode0 = __rlwimi(mode0, lbl_80665A40[min_filt], 5, 24, 26);
    mode0 = __rlwimi(mode0, __cntlzw((u32)do_edge_lod), 3, 23, 23);
    mode0 = __rlwinm(mode0, 0, 15, 12);
    mode0 = __rlwimi(mode0, max_aniso, 19, 11, 12);
    mode0 = __rlwimi(mode0, bias_clamp, 21, 10, 10);
    t->mode0 = mode0;

    if (min_lod < float_8066C030) {
        min_lod = float_8066C030;
    } else if (min_lod > float_8066C034) {
        min_lod = float_8066C034;
    }
    lmin = (s32)(f32)(float_8066C010[0] * min_lod);

    if (max_lod < float_8066C030) {
        max_lod = float_8066C030;
    } else if (max_lod > float_8066C034) {
        max_lod = float_8066C034;
    }
    lmax = (s32)(f32)(float_8066C010[0] * max_lod);

    mode1 = t->mode1;
    mode1 = __rlwimi(mode1, (u32)lmin, 0, 24, 31);
    mode1 = __rlwimi(mode1, (u32)lmax, 8, 16, 23);
    t->mode1 = mode1;
}

void GXInitTexObjWrapMode(GXTexObj* obj, GXTexWrapMode sm, GXTexWrapMode tm) {
    GXTexObjImpl* t = (GXTexObjImpl*)obj;
    u32 mode0 = t->mode0;

    mode0 = __rlwimi(mode0, sm, 0, 30, 31);
    mode0 = __rlwimi(mode0, tm, 2, 28, 29);
    t->mode0 = mode0;
}

void GXInitTexObjTlut(GXTexObj* obj, u32 tlut_name) {
    GX_SETUP_TEXOBJ(t, obj)
    t->tlutName = tlut_name;
}

void GXInitTexObjFilter(GXTexObj* obj, GXTexFilter min_filt, GXTexFilter mag_filt) {
    u32 mode0;
    u32 magBits;
    u8 minHw;
    GXTexObjImpl* t = (GXTexObjImpl*)obj;

    mode0 = t->mode0;
    magBits = __cntlzw((u32)(mag_filt - 1));
    mode0 = __rlwimi(mode0, magBits, 31, 27, 27);
    t->mode0 = mode0;

    minHw = lbl_80665A40[min_filt];
    mode0 = __rlwimi(mode0, minHw, 5, 24, 26);
    t->mode0 = mode0;
}

void GXInitTexObjUserData(GXTexObj* obj, void* user_data) {
    GX_SETUP_TEXOBJ(t, obj)
    t->userData = user_data;
}

void* GXGetTexObjUserData(GXTexObj* obj) {
    GX_SETUP_TEXOBJ(t, obj)
    return t->userData;
}

u16 GXGetTexObjWidth(const GXTexObj* to) {
    GX_SETUP_TEXOBJ(t, to)
    return (u16)(TX_SETIMAGE0_GET_IMAGE_WIDTH(t->image0) + 1);
}

u16 GXGetTexObjHeight(const GXTexObj* to) {
    GX_SETUP_TEXOBJ(t, to)
    return (u16)(TX_SETIMAGE0_GET_IMAGE_HEIGHT(t->image0) + 1);
}

GXTexFmt GXGetTexObjFmt(const GXTexObj* to) {
    GX_SETUP_TEXOBJ(t, to)
    return t->fmt;
}

GXTexWrapMode GXGetTexObjWrapS(GXTexObj* to) {
    GX_SETUP_TEXOBJ(t, to)
    return (GXTexWrapMode)TX_SETMODE0_GET_WRAP_S(t->mode0);
}

GXTexWrapMode GXGetTexObjWrapT(GXTexObj* to) {
    GX_SETUP_TEXOBJ(t, to)
    return (GXTexWrapMode)TX_SETMODE0_GET_WRAP_T(t->mode0);
}

GXBool GXGetTexObjMipMap(const GXTexObj* to) {
    GX_SETUP_TEXOBJ(t, to)
    return (GXBool)((t->flags & 1) == 1);
}

void GXGetTexObjLODAll(GXTexObj* tex_obj, GXTexFilter* min_filt, GXTexFilter* mag_filt,
                       f32* min_lod, f32* max_lod, f32* lod_bias, GXBool* bias_clamp,
                       GXBool* do_edge_lod, GXAnisotropy* max_aniso) {
    u32 mode0;
    u32 mode1;
    u8 minLodByte;
    u8 maxLodByte;
    s16 lodBiasRaw;
    const u8* filtConv;
    GXTexObjImpl* t = (GXTexObjImpl*)tex_obj;

    mode1 = t->mode1;
    mode0 = t->mode0;
    filtConv = (const u8*)lbl_80665A48;

    *min_filt = (GXTexFilter)filtConv[TX_SETMODE0_GET_MIN_FILTER(mode0)];
    *mag_filt = (GXTexFilter)TX_SETMODE0_GET_MAG_FILTER(mode0);

    minLodByte = TX_SETMODE1_GET_MINLOD(mode1);
    *min_lod = (f32)(u32)minLodByte * float_8066C038;

    maxLodByte = TX_SETMODE1_GET_MAXLOD(mode1);
    *max_lod = (f32)(u32)maxLodByte * float_8066C038;

    lodBiasRaw = (s16)TX_SETMODE0_GET_LODBIAS(mode0);
    *lod_bias = (f32)(s8)lodBiasRaw * float_8066C03C;

    *bias_clamp = (GXBool)TX_SETMODE0_GET_LODCLAMP(mode0);
    *do_edge_lod = (GXBool)!TX_SETMODE0_GET_DIAGLOD_ENABLE(mode0);
    *max_aniso = (GXAnisotropy)TX_SETMODE0_GET_MAXANISO(mode0);
}

u32 GXGetTexObjTlut(GXTexObj* tex_obj) {
    GX_SETUP_TEXOBJ(t, tex_obj)
    return t->tlutName;
}

#pragma dont_inline on
void GXLoadTexObjPreLoaded(GXTexObj* obj, GXTexRegion* region, GXTexMapID id) {
    volatile void* pipe;
    GXTexObjImpl* t = (GXTexObjImpl*)obj;
    GXTexRegionImpl* r = (GXTexRegionImpl*)region;
    u32 mode0Val;
    u32 mode1Val;
    u32 image0Val;
    u32 image1Val;
    u32 image2Val;
    u32 image3Val;
    u8 mode0Id;
    u8 mode1Id;
    u8 image0Id;
    u8 image1Id;
    u8 image2Id;
    u8 image3Id;
    u8 flags;
    const u8* tblMode0 = (const u8*)lbl_80665A08;
    const u8* tblMode1 = (const u8*)lbl_80665A10;
    const u8* tblImage0 = (const u8*)lbl_80665A18;
    const u8* tblImage1 = (const u8*)lbl_80665A20;
    const u8* tblImage2 = (const u8*)lbl_80665A28;
    const u8* tblImage3 = (const u8*)lbl_80665A30;

    /* Retail lbzx order: mode0, mode1, image0, image2, image1, image3. */
    mode0Val = t->mode0;
    mode0Id = tblMode0[id];
    mode0Val = __rlwimi(mode0Val, mode0Id, 24, 0, 7);

    mode1Val = t->mode1;
    mode1Id = tblMode1[id];
    image0Id = tblImage0[id];
    mode1Val = __rlwimi(mode1Val, mode1Id, 24, 0, 7);
    image2Id = tblImage2[id];
    image1Id = tblImage1[id];
    image3Id = tblImage3[id];

    image0Val = t->image0;
    image0Val = __rlwimi(image0Val, image0Id, 24, 0, 7);

    /* Initialized at first BP use; keeps params colored first. */
    pipe = (volatile void*)&WGPIPE;
    *(volatile u8*)pipe = GX_FIFO_CMD_LOAD_BP_REG;
    image1Val = r->image1;
    image1Val = __rlwimi(image1Val, image1Id, 24, 0, 7);
    *(volatile u32*)pipe = mode0Val;

    image2Val = r->image2;
    image2Val = __rlwimi(image2Val, image2Id, 24, 0, 7);
    *(volatile u8*)pipe = GX_FIFO_CMD_LOAD_BP_REG;
    flags = t->flags;
    *(volatile u32*)pipe = mode1Val;

    image3Val = t->image3;
    *(volatile u8*)pipe = GX_FIFO_CMD_LOAD_BP_REG;
    image3Val = __rlwimi(image3Val, image3Id, 24, 0, 7);
    *(volatile u32*)pipe = image0Val;

    *(volatile u8*)pipe = GX_FIFO_CMD_LOAD_BP_REG;
    *(volatile u32*)pipe = image1Val;
    *(volatile u8*)pipe = GX_FIFO_CMD_LOAD_BP_REG;
    *(volatile u32*)pipe = image2Val;

    /* stb for image3 lands before these stores; stw follows. */
    *(volatile u8*)pipe = GX_FIFO_CMD_LOAD_BP_REG;
    t->mode0 = mode0Val;
    t->mode1 = mode1Val;
    t->image0 = image0Val;
    r->image1 = image1Val;
    r->image2 = image2Val;
    t->image3 = image3Val;
    *(volatile u32*)pipe = image3Val;

    /* rlwinm. flags bit1 - skip tlut when set. */
    if ((flags & 2) == 0) {
        GXTlutRegionImpl* tlr =
            (GXTlutRegionImpl*)(gxdt->tlutRegionCallback)(t->tlutName);
        const u8* tblTlut = (const u8*)lbl_80665A38;
        u32 tlutVal = tlr->tlutObj.tlut;
        u8 tlutId = tblTlut[id];
        tlutVal = __rlwimi(tlutVal, tlutId, 24, 0, 7);
        tlr->tlutObj.tlut = tlutVal;
        *(volatile u8*)pipe = GX_FIFO_CMD_LOAD_BP_REG;
        *(volatile u32*)pipe = tlr->tlutObj.tlut;
    }

    {
        GXData* gx = gxdt;
        gx->tImage0[id] = t->image0;
        gx->tMode0[id] = t->mode0;
        gx->gxDirtyFlags |= GX_DIRTY_SU_TEX;
        gx->lastWriteWasXF = 0;
    }
}
#pragma dont_inline off

void GXLoadTexObj(const GXTexObj* obj, GXTexMapID id) {
    GXTexRegion* r;
    r = (gxdt->texRegionCallback)(obj, id);
    GXLoadTexObjPreLoaded((GXTexObj*)obj, r, id);
}

void GXInitTlutObj(GXTlutObj* tlut_obj, void* lut, GXTlutFmt fmt, u16 n_entries) {
    u32 tlut;
    u32 loadTlut0;
    GX_SETUP_TLUTOBJ(t, tlut_obj);

    tlut = 0;
    tlut = __rlwimi(tlut, fmt, 10, 20, 21);
    t->tlut = tlut;

    loadTlut0 = t->loadTlut0;
    loadTlut0 = __rlwimi(loadTlut0, (GX_PHY_ADDR(lut) >> 5), 0, 8, 31);
    loadTlut0 = __rlwimi(loadTlut0, 0x64, 24, 0, 7);
    t->loadTlut0 = loadTlut0;

    t->numEntries = n_entries;
}

void GXLoadTlut(GXTlutObj* tlut_obj, u32 tlut_name) {
    GXTlutRegionImpl* r;
    u32 merged;
    GX_SETUP_TLUTOBJ(t, tlut_obj);

    r = (GXTlutRegionImpl*)(gxdt->tlutRegionCallback)(tlut_name);

    __GXFlushTextureState();

    GX_BP_LOAD_REG(t->loadTlut0);
    GX_BP_LOAD_REG(r->loadTlut1);

    __GXFlushTextureState();

    merged = __rlwimi(t->tlut, r->loadTlut1, 0, 22, 31);
    r->tlutObj.tlut = merged;
    r->tlutObj.loadTlut0 = t->loadTlut0;
    ((u32*)&r->tlutObj)[2] = ((u32*)&t->tlut)[2];
    t->tlut = merged;
}

void GXInitTexCacheRegion(GXTexRegion* region, GXBool is_32b_mipmap, u32 tmem_even,
                          u32 size_even, u32 tmem_odd, u32 size_odd) {
    u32 WidthExp2;
    u32 image1;
    u32 image2;
    GX_SETUP_TREGOBJ(t, region)

    switch ((s32)size_even) {
    case 0:
        WidthExp2 = 3;
        break;
    case 1:
        WidthExp2 = 4;
        break;
    case 2:
        WidthExp2 = 5;
        break;
    default:
        break;
    }

    image1 = 0;
    image1 = __rlwimi(image1, tmem_even, 27, 17, 31);
    image1 = __rlwimi(image1, WidthExp2, 15, 14, 16);
    image1 = __rlwimi(image1, WidthExp2, 18, 11, 13);
    t->image1 = image1;

    switch ((s32)size_odd) {
    case 0:
        WidthExp2 = 3;
        break;
    case 1:
        WidthExp2 = 4;
        break;
    case 2:
        WidthExp2 = 5;
        break;
    case 3:
        WidthExp2 = 0;
        break;
    default:
        break;
    }

    image2 = 0;
    image2 = __rlwimi(image2, tmem_odd, 27, 17, 31);
    image2 = __rlwimi(image2, WidthExp2, 15, 14, 16);
    image2 = __rlwimi(image2, WidthExp2, 18, 11, 13);
    t->image2 = image2;

    t->is32bMipmap = (u8)is_32b_mipmap;
    t->isCached = GX_TRUE;
}

void GXInitTlutRegion(GXTlutRegion* region, u32 tmem_addr, u32 tlut_size) {
    GX_SETUP_TLUTREGOBJ(t, region)

    t->loadTlut1 = 0;
    tmem_addr -= 0x80000;
    t->loadTlut1 = __rlwimi(t->loadTlut1, tmem_addr, 23, 22, 31);
    t->loadTlut1 = __rlwimi(t->loadTlut1, tlut_size, 10, 11, 21);
    t->loadTlut1 = __rlwimi(t->loadTlut1, 0x65, 24, 0, 7);
}

void GXInvalidateTexAll(void) {
    u32 reg0, reg1;
    reg0 = TX_INVALTAGS(0, 8, 0x66);
    reg1 = TX_INVALTAGS(256, 8, 0x66);
    __GXFlushTextureState();
    GX_BP_LOAD_REG(reg0);
    GX_BP_LOAD_REG(reg1);
    __GXFlushTextureState();
}

GXTexRegionCallback GXSetTexRegionCallback(GXTexRegionCallback f) {
    GXTexRegionCallback oldcb = gxdt->texRegionCallback;
    gxdt->texRegionCallback = f;
    return oldcb;
}

GXTlutRegionCallback GXSetTlutRegionCallback(GXTlutRegionCallback f) {
    GXTlutRegionCallback oldcb = gxdt->tlutRegionCallback;
    gxdt->tlutRegionCallback = f;
    return oldcb;
}

void GXSetTexCoordScaleManually(GXTexCoordID coord, GXBool enable, u16 ss, u16 ts) {
    gxdt->tcsManEnab = gxdt->tcsManEnab & ~(1 << coord) | (enable << coord);

    if (enable) {
        GX_BP_SET_SU_SIZE_SCALE(gxdt->suTs0[coord], (u16)(ss - 1));
        GX_BP_SET_SU_SIZE_SCALE(gxdt->suTs1[coord], (u16)(ts - 1));

        GX_BP_LOAD_REG(gxdt->suTs0[coord]);
        GX_BP_LOAD_REG(gxdt->suTs1[coord]);
        gxdt->lastWriteWasXF = FALSE;
    }
}

#pragma dont_inline on
void __SetSURegs(u32 tmap, u32 tcoord) {
    u32 mode0;
    u32 suTs0;
    u32 suTs1;
    u32 w;
    u32 h;
    u32 wrapS;
    u32 wrapT;
    u32 sBias;
    u32 tBias;

    w = gxdt->tImage0[tmap] & 0x3FF;
    h = (gxdt->tImage0[tmap] >> 10) & 0x3FF;

    suTs0 = gxdt->suTs0[tcoord];
    suTs0 = __rlwimi(suTs0, w, 0, 16, 31);
    gxdt->suTs0[tcoord] = suTs0;

    suTs1 = gxdt->suTs1[tcoord];
    suTs1 = __rlwimi(suTs1, h, 0, 16, 31);
    gxdt->suTs1[tcoord] = suTs1;

    mode0 = gxdt->tMode0[tmap];

    wrapS = mode0 & 3;
    wrapT = (mode0 >> 2) & 3;
    wrapS = wrapS - 1;
    wrapT = wrapT - 1;
    sBias = __cntlzw(wrapS);
    tBias = __cntlzw(wrapT);

    suTs0 = __rlwimi(suTs0, sBias, 11, 15, 15);
    gxdt->suTs0[tcoord] = suTs0;

    suTs1 = __rlwimi(suTs1, tBias, 11, 15, 15);
    gxdt->suTs1[tcoord] = suTs1;

    GX_BP_LOAD_REG(gxdt->suTs0[tcoord]);
    GX_BP_LOAD_REG(gxdt->suTs1[tcoord]);
    gxdt->lastWriteWasXF = FALSE;
}
#pragma dont_inline off

void __GXSetSUTexRegs(void) {
    u32 nStages, nIndStages, i, map, tmap, coord;
    u32* ptref;

    if (gxdt->tcsManEnab == 0xFF) {
        return;
    }

    nStages = ((gxdt->genMode & 0x3C00) >> 10) + 1;
    nIndStages = ((gxdt->genMode & 0x70000) >> 16);

    for (i = 0; i < nIndStages; i++) {
        switch (i) {
        case GX_INDTEXSTAGE0:
            tmap = (gxdt->ras1_iref) & 0x7;
            coord = ((gxdt->ras1_iref & 0x38) >> 3);
            break;
        case GX_INDTEXSTAGE1:
            tmap = ((gxdt->ras1_iref & 0x1C0) >> 6);
            coord = ((gxdt->ras1_iref & 0xE00) >> 9);
            break;
        case GX_INDTEXSTAGE2:
            tmap = ((gxdt->ras1_iref & 0x7000) >> 12);
            coord = ((gxdt->ras1_iref & 0x38000) >> 15);
            break;
        case GX_INDTEXSTAGE3:
            tmap = ((gxdt->ras1_iref & 0x1C0000) >> 18);
            coord = ((gxdt->ras1_iref & 0xE00000) >> 21);
            break;
        }

        if (!(gxdt->tcsManEnab & (1 << coord))) {
            __SetSURegs(tmap, coord);
        }
    }

    for (i = 0; i < nStages; i++) {
        ptref = &gxdt->tref[i / 2];
        map = gxdt->texmapId[i];
        tmap = map & ~256;

        if (i & 1) {
            coord = ((*ptref & 0x38000) >> 15);
        } else {
            coord = ((*ptref & 0x38) >> 3);
        }

        if ((tmap != GX_TEXMAP_NULL) && !(gxdt->tcsManEnab & (1 << coord)) &&
            (gxdt->tevTcEnab & (1 << i))) {
            __SetSURegs(tmap, coord);
        }
    }
}

void __GXSetTmemConfig(u32 config) {
    /* Retail BP payloads (SETIMAGE1/2 even+odd banks). Do not rebuild via
       TX_SETIMAGE*(addr>>5, 3, 3, …) - that overflows tmem into the RID byte
       and emits 0xBB001E00-style junk vs retail 0xB30DDC00. */
    switch (config) {
    case 2:
        GX_BP_LOAD_REG(0x8C0D8000);
        GX_BP_LOAD_REG(0x900DC000);
        GX_BP_LOAD_REG(0x8D0D8800);
        GX_BP_LOAD_REG(0x910DC800);
        GX_BP_LOAD_REG(0x8E0D9000);
        GX_BP_LOAD_REG(0x920DD000);
        GX_BP_LOAD_REG(0x8F0D9800);
        GX_BP_LOAD_REG(0x930DD800);
        GX_BP_LOAD_REG(0xAC0DA000);
        GX_BP_LOAD_REG(0xB00DC400);
        GX_BP_LOAD_REG(0xAD0DA800);
        GX_BP_LOAD_REG(0xB10DCC00);
        GX_BP_LOAD_REG(0xAE0DB000);
        GX_BP_LOAD_REG(0xB20DD400);
        GX_BP_LOAD_REG(0xAF0DB800);
        GX_BP_LOAD_REG(0xB30DDC00);
        break;
    case 1:
        GX_BP_LOAD_REG(0x8C0D8000);
        GX_BP_LOAD_REG(0x900DC000);
        GX_BP_LOAD_REG(0x8D0D8800);
        GX_BP_LOAD_REG(0x910DC800);
        GX_BP_LOAD_REG(0x8E0D9000);
        GX_BP_LOAD_REG(0x920DD000);
        GX_BP_LOAD_REG(0x8F0D9800);
        GX_BP_LOAD_REG(0x930DD800);
        GX_BP_LOAD_REG(0xAC0DA000);
        GX_BP_LOAD_REG(0xB00DE000);
        GX_BP_LOAD_REG(0xAD0DA800);
        GX_BP_LOAD_REG(0xB10DE800);
        GX_BP_LOAD_REG(0xAE0DB000);
        GX_BP_LOAD_REG(0xB20DF000);
        GX_BP_LOAD_REG(0xAF0DB800);
        GX_BP_LOAD_REG(0xB30DF800);
        break;

    default:
    case 0:
        GX_BP_LOAD_REG(0x8C0D8000);
        GX_BP_LOAD_REG(0x900DC000);
        GX_BP_LOAD_REG(0x8D0D8400);
        GX_BP_LOAD_REG(0x910DC400);
        GX_BP_LOAD_REG(0x8E0D8800);
        GX_BP_LOAD_REG(0x920DC800);
        GX_BP_LOAD_REG(0x8F0D8C00);
        GX_BP_LOAD_REG(0x930DCC00);
        GX_BP_LOAD_REG(0xAC0D9000);
        GX_BP_LOAD_REG(0xB00DD000);
        GX_BP_LOAD_REG(0xAD0D9400);
        GX_BP_LOAD_REG(0xB10DD400);
        GX_BP_LOAD_REG(0xAE0D9800);
        GX_BP_LOAD_REG(0xB20DD800);
        GX_BP_LOAD_REG(0xAF0D9C00);
        GX_BP_LOAD_REG(0xB30DDC00);
        break;
    }
}

void GXSetTexCoordCylWrap(GXTexCoordID coord, GXBool s_enable, GXBool t_enable) {
    GX_BP_SET_SU_SIZE_CYLINDRICWRAP(gxdt->suTs0[coord], s_enable);
    GX_BP_SET_SU_SIZE_CYLINDRICWRAP(gxdt->suTs1[coord], t_enable);

    /* Retail beqlr after and.: flush only when the coord is manually enabled. */
    if (gxdt->tcsManEnab & (1 << coord)) {
        GX_BP_LOAD_REG(gxdt->suTs0[coord]);
        GX_BP_LOAD_REG(gxdt->suTs1[coord]);
        gxdt->lastWriteWasXF = FALSE;
    }
}
