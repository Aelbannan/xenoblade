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
    virtual ~BiquadFilterLpf() {}

    virtual void GetCoef(int order, f32 freq, BiquadCoef* pCoef) const;
    static const BiquadCoef coefTable[];
};

class BiquadFilterHpf : public BiquadFilterCallback {
public:
    virtual ~BiquadFilterHpf() {}

    virtual void GetCoef(int order, f32 freq, BiquadCoef* pCoef) const;
    static const BiquadCoef coefTable[];
};

class BiquadFilterBpf512 : public BiquadFilterCallback {
public:
    virtual ~BiquadFilterBpf512() {}

    virtual void GetCoef(int order, f32 freq, BiquadCoef* pCoef) const;
    static const BiquadCoef coefTable[];
};

class BiquadFilterBpf1024 : public BiquadFilterCallback {
public:
    virtual ~BiquadFilterBpf1024() {}

    virtual void GetCoef(int order, f32 freq, BiquadCoef* pCoef) const;
    static const BiquadCoef coefTable[];
};

class BiquadFilterBpf2048 : public BiquadFilterCallback {
public:
    virtual ~BiquadFilterBpf2048() {}

    virtual void GetCoef(int order, f32 freq, BiquadCoef* pCoef) const;
    static const BiquadCoef coefTable[];
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
