#include "monolib/math.hpp"

extern const float lbl_eu_8066A258; // 0.0f
extern const float lbl_eu_8066A25C; // 1.0f

namespace ml{
    CMat44 CMat44::zero = CMat44(
    lbl_eu_8066A258,lbl_eu_8066A258,lbl_eu_8066A258,lbl_eu_8066A258,
    lbl_eu_8066A258,lbl_eu_8066A258,lbl_eu_8066A258,lbl_eu_8066A258,
    lbl_eu_8066A258,lbl_eu_8066A258,lbl_eu_8066A258,lbl_eu_8066A258,
    lbl_eu_8066A258,lbl_eu_8066A258,lbl_eu_8066A258,lbl_eu_8066A258
    );

    CMat44 CMat44::identity = CMat44(
    lbl_eu_8066A25C,lbl_eu_8066A258,lbl_eu_8066A258,lbl_eu_8066A258,
    lbl_eu_8066A258,lbl_eu_8066A25C,lbl_eu_8066A258,lbl_eu_8066A258,
    lbl_eu_8066A258,lbl_eu_8066A258,lbl_eu_8066A25C,lbl_eu_8066A258,
    lbl_eu_8066A258,lbl_eu_8066A258,lbl_eu_8066A258,lbl_eu_8066A25C
    );
} //namespace ml
