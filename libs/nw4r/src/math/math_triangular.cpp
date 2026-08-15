#include <nw4r/math.h>

namespace nw4r {
namespace math {

struct SinCosSample {
    f32 sin_val;   // at 0x0
    f32 cos_val;   // at 0x4
    f32 sin_delta; // at 0x8
    f32 cos_delta; // at 0xC
};

struct ArcTanSample {
    f32 atan_val;   // at 0x0
    f32 atan_delta; // at 0x4
};

// Retail tables ship from the nw4r data unit (nw4r_data.s):
//   lbl_eu_8051D7F8 = gSinCosTbl (.rodata, 0x1010)
//   lbl_eu_8056A5E0 = sArcTanTbl (.data, 0x108)
extern "C" const SinCosSample lbl_eu_8051D7F8[];
extern "C" ArcTanSample lbl_eu_8056A5E0[];

// Retail .sdata2 constants referenced by name so no local pool is emitted.
extern "C" {
    extern const f32 lbl_eu_80669E20; // 65536.0f
    extern const f32 lbl_eu_80669E24; // 0.0f
    extern const f32 lbl_eu_80669E28; // 32.0f
    extern const f32 lbl_eu_80669E2C; // 64.0f
    extern const f32 lbl_eu_80669E30; // -64.0f
    extern const f32 lbl_eu_80669E34; // 128.0f
    extern const f32 lbl_eu_80669E38; // -128.0f
}

namespace {

// Retail inlines this into Atan2FIdx and never emits a standalone body.
static f32 AtanFIdx_(f32 x) {
    x *= lbl_eu_80669E28;

    u16 whole = F32ToU16(x);
    f32 frac = x - U16ToF32(whole);

    f32 atan = lbl_eu_8056A5E0[whole].atan_val + frac * lbl_eu_8056A5E0[whole].atan_delta;

    return atan;
}

} // namespace

f32 SinFIdx(f32 fidx) {
    f32 abs_fidx = FAbs(fidx);

    while (abs_fidx > lbl_eu_80669E20) {
        abs_fidx -= lbl_eu_80669E20;
    }

    u16 whole = F32ToU16(abs_fidx);
    f32 frac = abs_fidx - U16ToF32(whole);

    f32 sin = lbl_eu_8051D7F8[whole & 255].sin_val +
              frac * lbl_eu_8051D7F8[whole & 255].sin_delta;

    return (fidx < lbl_eu_80669E24) ? -sin : sin;
}

f32 CosFIdx(f32 fidx) {
    f32 abs_fidx = FAbs(fidx);

    while (abs_fidx > lbl_eu_80669E20) {
        abs_fidx -= lbl_eu_80669E20;
    }

    u16 whole = F32ToU16(abs_fidx);
    f32 frac = abs_fidx - U16ToF32(whole);

    f32 cos = lbl_eu_8051D7F8[whole & 255].cos_val +
              frac * lbl_eu_8051D7F8[whole & 255].cos_delta;

    return cos;
}

void SinCosFIdx(register f32* pSin, register f32* pCos, register f32 fidx) {
    register u32 idx; // r31
    register f32 abs_fidx; // f31
    register f32 r; // f30
    register f32 idxmax; // f29
    register __vec2x32float__ scval, scdel; //f28, f27
    register __vec2x32float__ result; //f26
    register f32 c_zero; // f25
    const register f32* pTbl; // r30
    
    idxmax = lbl_eu_80669E20;
    pTbl = reinterpret_cast<const f32*>(lbl_eu_8051D7F8);

    /*
    abs_fidx = FAbs(fidx);

    while (abs_fidx > idxmax) {
        abs_fidx -= idxmax;
    }

    u16 raw = F32ToU16(abs_fidx);

    idx = raw;
    c_zero = 0; (or idxmax - idxmax bc hudsonsoft silly)
    idx = (idx & 0xFF) << 4;
    pTbl += idx;
    */
    ASM (
        fabs abs_fidx, fidx;
        psq_st abs_fidx, 0(pSin), 1, 3;
        fcmpu cr0, abs_fidx, idxmax;
        ble loc2;
    loc1:
        fsubs abs_fidx, abs_fidx, idxmax;
        fcmpu cr0, abs_fidx, idxmax;
        bgt loc1;
        psq_st abs_fidx, 0(pSin), 1, 3;
    loc2:
        lhz idx, 0(pSin);
        fsubs c_zero, idxmax, idxmax; // you could just... use 0, but sure
        rlwinm idx, idx, 4, 20, 27;
        add pTbl, pTbl, idx;
    )

    /*
    r = abs_fidx - U16ToF32(*reinterpret_cast<const u16*>(pSin));

    scval[0] = pTbl[0];
    scval[1] = pTbl[1];
    scdel[0] = pTbl[2];
    scdel[1] = pTbl[3];

    result[0] = scdel[0] * r + scval[0];
    result[1] = scdel[1] * r + scval[1];

    *pCos = result[1];
    *pSin = fidx < 0 ? -result[0] : result[0];
    */
    ASM (
        psq_l r, 0(pSin), 1, 3
        fsubs r, abs_fidx, r
        psq_l scval, 0(pTbl), 0, 0
        psq_l scdel, 8(pTbl), 0, 0
        ps_madds0 result, scdel, r, scval
        ps_merge10 r, result, result
        psq_st r, 0(pCos), 1, 0
        fcmpu cr0, fidx, c_zero;
        bge positive;
        ps_neg result, result;
    positive:
        psq_st result, 0(pSin), 1, 0;
    )

}

f32 Atan2FIdx(f32 y, f32 x) {
    f32 a, b, c;
    bool minus;

    if (lbl_eu_80669E24 == x && lbl_eu_80669E24 == y) {
        return lbl_eu_80669E24;
    }

    if (x >= lbl_eu_80669E24) {
        if (y >= lbl_eu_80669E24) {
            if (x >= y) {
                a = x;
                b = y;
                c = lbl_eu_80669E24;
                minus = false;
            } else {
                a = y;
                b = x;
                c = lbl_eu_80669E2C;
                minus = true;
            }
        } else {
            if (x >= -y) {
                a = x;
                b = -y;
                c = lbl_eu_80669E24;
                minus = true;
            } else {
                a = -y;
                b = x;
                c = lbl_eu_80669E30;
                minus = false;
            }
        }
    } else {
        if (y >= lbl_eu_80669E24) {
            if (-x >= y) {
                a = -x;
                b = y;
                c = lbl_eu_80669E34;
                minus = true;
            } else {
                a = y;
                b = -x;
                c = lbl_eu_80669E2C;
                minus = false;
            }
        } else {
            if (-x >= -y) {
                a = -x;
                b = -y;
                c = lbl_eu_80669E38;
                minus = false;
            } else {
                a = -y;
                b = -x;
                c = lbl_eu_80669E30;
                minus = true;
            }
        }
    }

    if (minus) {
        return c - AtanFIdx_(b / a);
    } else {
        return c + AtanFIdx_(b / a);
    }
}

} // namespace math
} // namespace nw4r
