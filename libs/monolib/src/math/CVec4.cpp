#include "monolib/math.hpp"

extern const float lbl_eu_8066A1F0; // 0.0f
extern const float lbl_eu_8066A1F4; // 1.0f

// Pre-declare BSS symbols to force correct relocation names
namespace ml {
    extern CVec4 zero__Q22ml5CVec4;
    extern CVec4 unitX__Q22ml5CVec4;
    extern CVec4 unitY__Q22ml5CVec4;
    extern CVec4 unitZ__Q22ml5CVec4;
    extern CVec4 unit__Q22ml5CVec4;
}

namespace ml{
    CVec4 CVec4::zero = CVec4(lbl_eu_8066A1F0, lbl_eu_8066A1F0, lbl_eu_8066A1F0, lbl_eu_8066A1F0);
    CVec4 CVec4::unitX = CVec4(lbl_eu_8066A1F4, lbl_eu_8066A1F0, lbl_eu_8066A1F0, lbl_eu_8066A1F4);
    CVec4 CVec4::unitY = CVec4(lbl_eu_8066A1F0, lbl_eu_8066A1F4, lbl_eu_8066A1F0, lbl_eu_8066A1F4);
    CVec4 CVec4::unitZ = CVec4(lbl_eu_8066A1F0, lbl_eu_8066A1F0, lbl_eu_8066A1F4, lbl_eu_8066A1F4);
    CVec4 CVec4::unit = CVec4(lbl_eu_8066A1F4, lbl_eu_8066A1F4, lbl_eu_8066A1F4, lbl_eu_8066A1F4);
} //namespace ml
