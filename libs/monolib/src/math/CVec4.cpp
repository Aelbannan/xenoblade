#include "monolib/math.hpp"


namespace ml{
    CVec4 CVec4::zero = CVec4(0.0f, 0.0f, 0.0f, 0.0f);
    CVec4 CVec4::unitX = CVec4(1.0f, 0.0f, 0.0f, 1.0f);
    CVec4 CVec4::unitY = CVec4(0.0f, 1.0f, 0.0f, 1.0f);
    CVec4 CVec4::unitZ = CVec4(0.0f, 0.0f, 1.0f, 1.0f);
    CVec4 CVec4::unit = CVec4(1.0f, 1.0f, 1.0f, 1.0f);
} //namespace ml
