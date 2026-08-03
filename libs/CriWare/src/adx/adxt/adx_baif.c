// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_baif
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

static inline u32 AiffLE32(const u8* p) {
    return (u32)p[0] | ((u32)p[1] << 8) | ((u32)p[2] << 16) | ((u32)p[3] << 24);
}

static inline u32 AiffSwap32(u32 x) {
    return (((x >> 8) & 0xFF) << 16) | (((x >> 16) & 0xFF) << 8) | ((x & 0xFF) << 24) | ((x >> 24) & 0xFF);
}

static inline u32 AiffLE16(const u8* p) {
    return (u32)p[0] | ((u32)p[1] << 8);
}

static inline u32 AiffSwap16(u32 x) {
    return ((x >> 8) & 0xFF) | ((x & 0xFF) << 8);
}

int AIFF_GetInfo(u8* self, u32* out1, u32* out2, u32* out3, u32* out4) {
    u8* p = self + 12;
    u32 comm = 0;
    if (AiffLE32(self) != 0x4D524F46) return 0;
    if (AiffLE32(self + 8) != 0x46464941) return 0;
    {
        u8* end = p + AiffSwap32(AiffLE32(self + 4)) - 4;
        u32 ssnd = 0;
        while (p < end) {
        u32 type = AiffLE32(p);
        u32 size = AiffLE32(p + 4);
        p += 8;
        if (type == 0x4D4D4F43) {
            if (comm == 0) {
                if (AiffSwap32(size) <= 18) return 0;
                *out2 = AiffSwap16(AiffLE16(p));
                *out4 = AiffSwap32(AiffLE32(p + 2));
                *out3 = AiffSwap16(AiffLE16(p + 6));
                *out1 = AiffSwap32(AiffLE32(p + 8));
                comm = 1;
            }
            if (ssnd != 0) goto out;
        } else if (type == 0x444E5353) {
            if (ssnd == 0) {
                u32 off = AiffSwap32(AiffLE32(p));
                ssnd = 1;
                p += 4;
                if (comm != 0) return (int)(p + off);
            }
        } else {
            p += (AiffSwap32(size) + 1) & ~1;
        }
        }
    }
out:
    ;
}

extern char lbl_eu_80516B20[];

int ADXB_CheckAiff(void* self) {
    if (memcmp(self, lbl_eu_80516B20, 4) == 0 && memcmp((u8*)self + 8, lbl_eu_80516B20 + 5, 4) == 0)
        return 1;
    return 0;
}

extern int AIFF_GetInfo(u8* self, u32* out1, u32* out2, u32* out3, u32* out4);

s32 ADX_DecodeInfoAiff(u8* src, s32 size, s16* outA, s8* outB, s8* outE, u8* outD, s8* outF, s32* outG, s32* outH, s32* outC) {
    s32 v1 = 0, v2 = 0, v3 = 0, v4 = 0;
    s32 r;
    if (size < 4096) {
        *outA = 0;
        return -1;
    }
    r = AIFF_GetInfo(src, (u32*)&v1, (u32*)&v2, (u32*)&v3, (u32*)&v4);
    if (r == 0)
        return -1;
    {
        s32 off = r - (s32)src;
        s16 s = (s16)off;
        *outA = s;
        if (s <= 0)
            return -1;
    }
    *outH = v1;
    *outF = (u8)v2;
    *outE = (u8)v3;
    *outG = v4;
    *outB = -1;
    *outD = (s8)((s8)*outF * (s8)*outE / 8);
    *outC = 1;
    return 0;
}

void ADXB_DecodeHeaderAiff() {}

void ADXB_ExecOneAiff16() {}

void ADXB_ExecOneAiff8() {}

void ADXB_ExecOneAiff(void* self) {
    if ((int)*(s16*)((u8*)self + 0x9c) == 1) {
        ((void(*)(void*))ADXB_ExecOneAiff8)(self);
    } else {
        ((void(*)(void*))ADXB_ExecOneAiff16)(self);
    }
}
