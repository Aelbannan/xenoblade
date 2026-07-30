#ifndef NW4R_SND_BIQUAD_FILTER_PRESET_H
#define NW4R_SND_BIQUAD_FILTER_PRESET_H
#include <nw4r/types_nw4r.h>

namespace nw4r {
namespace snd {

class BiquadFilterCallback {
public:
    struct BiquadCoef {
        u16 b0;
        u16 b1;
        u16 b2;
        u16 a1;
        u16 a2;
    };
};

namespace detail {

class BiquadFilterLpf : public BiquadFilterCallback {
public:
    void GetCoef(int order, f32 freq, BiquadCoef* pCoef) const;
};

class BiquadFilterHpf : public BiquadFilterCallback {
public:
    void GetCoef(int order, f32 freq, BiquadCoef* pCoef) const;
};

class BiquadFilterBpf512 : public BiquadFilterCallback {
public:
    void GetCoef(int order, f32 freq, BiquadCoef* pCoef) const;
};

class BiquadFilterBpf1024 : public BiquadFilterCallback {
public:
    void GetCoef(int order, f32 freq, BiquadCoef* pCoef) const;
};

class BiquadFilterBpf2048 : public BiquadFilterCallback {
public:
    void GetCoef(int order, f32 freq, BiquadCoef* pCoef) const;
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
