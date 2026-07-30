#include "monolib/math.hpp"

extern const float lbl_eu_8066A230; // 1.0f
extern const float lbl_eu_8066A234; // 0.5f
extern const float lbl_eu_8066A238; // 0.0f

namespace ml{
    CCol4 CCol4::white = CCol4(lbl_eu_8066A230, lbl_eu_8066A230, lbl_eu_8066A230, lbl_eu_8066A230);
    CCol4 CCol4::gray = CCol4(lbl_eu_8066A234, lbl_eu_8066A234, lbl_eu_8066A234, lbl_eu_8066A230);
    CCol4 CCol4::black = CCol4(lbl_eu_8066A238, lbl_eu_8066A238, lbl_eu_8066A238, lbl_eu_8066A230);
    CCol4 CCol4::red = CCol4(lbl_eu_8066A230, lbl_eu_8066A238, lbl_eu_8066A238, lbl_eu_8066A230);
    CCol4 CCol4::green = CCol4(lbl_eu_8066A238, lbl_eu_8066A230, lbl_eu_8066A238, lbl_eu_8066A230);
    CCol4 CCol4::blue = CCol4(lbl_eu_8066A238, lbl_eu_8066A238, lbl_eu_8066A230, lbl_eu_8066A230);
    CCol4 CCol4::yellow = CCol4(lbl_eu_8066A230, lbl_eu_8066A230, lbl_eu_8066A238, lbl_eu_8066A230);
    CCol4 CCol4::cyan = CCol4(lbl_eu_8066A238, lbl_eu_8066A230, lbl_eu_8066A230, lbl_eu_8066A230);
    CCol4 CCol4::magenta = CCol4(lbl_eu_8066A230, lbl_eu_8066A238, lbl_eu_8066A230, lbl_eu_8066A230);
    CCol4 CCol4::salmon = CCol4(lbl_eu_8066A230, lbl_eu_8066A234, lbl_eu_8066A234, lbl_eu_8066A230);
    CCol4 CCol4::orange = CCol4(lbl_eu_8066A230, lbl_eu_8066A234, lbl_eu_8066A238, lbl_eu_8066A230);
    CCol4 CCol4::zero = CCol4(lbl_eu_8066A238, lbl_eu_8066A238, lbl_eu_8066A238, lbl_eu_8066A238);
} //namespace ml
