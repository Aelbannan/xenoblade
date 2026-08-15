#include "monolib/math.hpp"

extern const float lbl_eu_8066A1F0; // 0.0f
extern const float lbl_eu_8066A1F4; // 1.0f

namespace ml{
    CVec4 CVec4::zero = CVec4(lbl_eu_8066A1F0, lbl_eu_8066A1F0, lbl_eu_8066A1F0, lbl_eu_8066A1F0);
    CVec4 CVec4::unitX = CVec4(lbl_eu_8066A1F4, lbl_eu_8066A1F0, lbl_eu_8066A1F0, lbl_eu_8066A1F4);
    CVec4 CVec4::unitY = CVec4(lbl_eu_8066A1F0, lbl_eu_8066A1F4, lbl_eu_8066A1F0, lbl_eu_8066A1F4);
    CVec4 CVec4::unitZ = CVec4(lbl_eu_8066A1F0, lbl_eu_8066A1F0, lbl_eu_8066A1F4, lbl_eu_8066A1F4);
    CVec4 CVec4::unit = CVec4(lbl_eu_8066A1F4, lbl_eu_8066A1F4, lbl_eu_8066A1F4, lbl_eu_8066A1F4);
} //namespace ml
