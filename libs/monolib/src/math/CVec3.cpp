#include "monolib/math.hpp"

extern const float lbl_eu_8066A1E8; // 0.0f
extern const float lbl_eu_8066A1EC; // 1.0f

namespace ml{
    CVec3 CVec3::zero = CVec3(lbl_eu_8066A1E8, lbl_eu_8066A1E8, lbl_eu_8066A1E8);
    CVec3 CVec3::unitX = CVec3(lbl_eu_8066A1EC, lbl_eu_8066A1E8, lbl_eu_8066A1E8);
    CVec3 CVec3::unitY = CVec3(lbl_eu_8066A1E8, lbl_eu_8066A1EC, lbl_eu_8066A1E8);
    CVec3 CVec3::unitZ = CVec3(lbl_eu_8066A1E8, lbl_eu_8066A1E8, lbl_eu_8066A1EC);
    CVec3 CVec3::unit = CVec3(lbl_eu_8066A1EC, lbl_eu_8066A1EC, lbl_eu_8066A1EC);

    //Checks whether any values in the vector are invalid.
    bool CVec3::isErr() const {
        return math::isErrFloat(x) || math::isErrFloat(y) || math::isErrFloat(z);
    }

} //namespace ml
