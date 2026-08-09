#include <nw4r/snd.h>

namespace nw4r {
namespace snd {
namespace detail {

void BiquadFilterLpf::GetCoef(int /*order*/, f32 freq,
                              BiquadFilterCallback::BiquadCoef* pCoef) const {
    int maxIdx = 0x6F;
    int t = (int)((f32)maxIdx * freq);
    int idx;

    if (t > maxIdx) {
        idx = maxIdx; // upper clamp
    } else {
        idx = (t < 0) ? 0 : t;
    }

    pCoef->b0 = lbl_eu_8051E808[idx].b0;
    pCoef->b1 = lbl_eu_8051E808[idx].b1;
    pCoef->b2 = lbl_eu_8051E808[idx].b2;
    pCoef->a1 = lbl_eu_8051E808[idx].a1;
    pCoef->a2 = lbl_eu_8051E808[idx].a2;
}

void BiquadFilterHpf::GetCoef(int /*order*/, f32 freq,
                              BiquadFilterCallback::BiquadCoef* pCoef) const {
    int maxIdx = 0x60;
    int t = (int)((f32)maxIdx * freq);
    int idx;

    if (t > maxIdx) {
        idx = maxIdx; // upper clamp
    } else {
        idx = (t < 0) ? 0 : t;
    }

    pCoef->b0 = lbl_eu_8051EC68[idx].b0;
    pCoef->b1 = lbl_eu_8051EC68[idx].b1;
    pCoef->b2 = lbl_eu_8051EC68[idx].b2;
    pCoef->a1 = lbl_eu_8051EC68[idx].a1;
    pCoef->a2 = lbl_eu_8051EC68[idx].a2;
}

void BiquadFilterBpf512::GetCoef(int /*order*/, f32 freq,
                                 BiquadFilterCallback::BiquadCoef* pCoef) const {
    f32 scale = freq * (1.0f - freq);
    int maxIdx = 0x79;
    int t = (int)((f32)maxIdx * scale);
    int idx;

    if (t > maxIdx) {
        idx = maxIdx; // upper clamp
    } else {
        idx = (t < 0) ? 0 : t;
    }

    pCoef->b0 = lbl_eu_8051F034[idx].b0;
    pCoef->b1 = lbl_eu_8051F034[idx].b1;
    pCoef->b2 = lbl_eu_8051F034[idx].b2;
    pCoef->a1 = lbl_eu_8051F034[idx].a1;
    pCoef->a2 = lbl_eu_8051F034[idx].a2;
}

void BiquadFilterBpf1024::GetCoef(int /*order*/, f32 freq,
                                  BiquadFilterCallback::BiquadCoef* pCoef) const {
    f32 scale = freq * (1.0f - freq);
    int maxIdx = 0x5C;
    int t = (int)((f32)maxIdx * scale);
    int idx;

    if (t > maxIdx) {
        idx = maxIdx; // upper clamp
    } else {
        idx = (t < 0) ? 0 : t;
    }

    pCoef->b0 = lbl_eu_8051F4F8[idx].b0;
    pCoef->b1 = lbl_eu_8051F4F8[idx].b1;
    pCoef->b2 = lbl_eu_8051F4F8[idx].b2;
    pCoef->a1 = lbl_eu_8051F4F8[idx].a1;
    pCoef->a2 = lbl_eu_8051F4F8[idx].a2;
}

void BiquadFilterBpf2048::GetCoef(int /*order*/, f32 freq,
                                  BiquadFilterCallback::BiquadCoef* pCoef) const {
    f32 scale = freq * (1.0f - freq);
    int maxIdx = 0x5C;
    int t = (int)((f32)maxIdx * scale);
    int idx;

    if (t > maxIdx) {
        idx = maxIdx; // upper clamp
    } else {
        idx = (t < 0) ? 0 : t;
    }

    pCoef->b0 = lbl_eu_8051F89C[idx].b0;
    pCoef->b1 = lbl_eu_8051F89C[idx].b1;
    pCoef->b2 = lbl_eu_8051F89C[idx].b2;
    pCoef->a1 = lbl_eu_8051F89C[idx].a1;
    pCoef->a2 = lbl_eu_8051F89C[idx].a2;
}

} // namespace detail
} // namespace snd
} // namespace nw4r
