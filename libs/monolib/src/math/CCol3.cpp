#include "monolib/math.hpp"

extern const float lbl_eu_8066A220; // 1.0f
extern const float lbl_eu_8066A224; // 0.5f
extern const float lbl_eu_8066A228; // 0.0f

namespace ml{
    CCol3 CCol3::white = CCol3(lbl_eu_8066A220, lbl_eu_8066A220, lbl_eu_8066A220);
    CCol3 CCol3::gray = CCol3(lbl_eu_8066A224, lbl_eu_8066A224, lbl_eu_8066A224);
    CCol3 CCol3::black = CCol3(lbl_eu_8066A228, lbl_eu_8066A228, lbl_eu_8066A228);
    CCol3 CCol3::red = CCol3(lbl_eu_8066A220, lbl_eu_8066A228, lbl_eu_8066A228);
    CCol3 CCol3::green = CCol3(lbl_eu_8066A228, lbl_eu_8066A220, lbl_eu_8066A228);
    CCol3 CCol3::blue = CCol3(lbl_eu_8066A228, lbl_eu_8066A228, lbl_eu_8066A220);
    CCol3 CCol3::yellow = CCol3(lbl_eu_8066A220, lbl_eu_8066A220, lbl_eu_8066A228);
    CCol3 CCol3::cyan = CCol3(lbl_eu_8066A228, lbl_eu_8066A220, lbl_eu_8066A220);
    CCol3 CCol3::magenta = CCol3(lbl_eu_8066A220, lbl_eu_8066A228, lbl_eu_8066A220);
    CCol3 CCol3::salmon = CCol3(lbl_eu_8066A220, lbl_eu_8066A224, lbl_eu_8066A224);
    CCol3 CCol3::orange = CCol3(lbl_eu_8066A220, lbl_eu_8066A224, lbl_eu_8066A228);
} //namespace ml
