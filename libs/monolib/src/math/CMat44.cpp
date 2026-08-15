#include "monolib/math.hpp"

extern const float lbl_eu_8066A258; // 0.0f
extern const float lbl_eu_8066A25C; // 1.0f

namespace ml{
    // Retail values recovered from __sinit_\CMat44_cpp: zero = all 0.0f,
    // identity = diagonal 1.0f.  Storage lives in this TU's .bss
    // (split range 0x80656DE8..0x80656E68) after the monolibdata1f re-split.
    CMat44 CMat44::zero = CMat44(
        0,0,0,0,
        0,0,0,0,
        0,0,0,0,
        0,0,0,0
    );

    CMat44 CMat44::identity = CMat44(
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    );
} //namespace ml
