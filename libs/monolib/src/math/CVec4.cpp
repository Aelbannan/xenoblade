#include "monolib/math.hpp"

extern const float lbl_eu_8066A1F0; // 0.0f
extern const float lbl_eu_8066A1F4; // 1.0f

namespace ml{
    // The retail split keeps the CVec4 statics in the shared monolibdata1f
    // .bss (0x80656BF0..0x80656C40); this TU only *references* them.  Defining
    // them here would create a local .bss (data diff FAIL) and duplicate the
    // symbols at link time.  The retail __sinit_\CVec4_cpp still lives in this
    // TU's .text: it is reproduced by the auto-generated __sinit_ from the
    // CVec4Sinit global below (same store sequence, byte-identical).
    extern CVec4 CVec4::zero;
    extern CVec4 CVec4::unitX;
    extern CVec4 CVec4::unitY;
    extern CVec4 CVec4::unitZ;
    extern CVec4 CVec4::unit;

    // Auto-__sinit_ trigger: a global object with an inlined constructor that
    // writes the retail values into the extern statics.  MWCC inlines this
    // ctor into __sinit_\CVec4_cpp, emitting the exact retail store sequence
    // with the sdata2 constants referenced via the named lbl_eu_8066A1F0/1F4.
    struct CVec4Sinit {
        CVec4Sinit() {
            CVec4::zero.x = lbl_eu_8066A1F0;
            CVec4::zero.y = lbl_eu_8066A1F0;
            CVec4::zero.z = lbl_eu_8066A1F0;
            CVec4::zero.w = lbl_eu_8066A1F0;

            CVec4::unitX.x = lbl_eu_8066A1F4;
            CVec4::unitX.y = lbl_eu_8066A1F0;
            CVec4::unitX.z = lbl_eu_8066A1F0;
            CVec4::unitX.w = lbl_eu_8066A1F4;

            CVec4::unitY.x = lbl_eu_8066A1F0;
            CVec4::unitY.y = lbl_eu_8066A1F4;
            CVec4::unitY.z = lbl_eu_8066A1F0;
            CVec4::unitY.w = lbl_eu_8066A1F4;

            CVec4::unitZ.x = lbl_eu_8066A1F0;
            CVec4::unitZ.y = lbl_eu_8066A1F0;
            CVec4::unitZ.z = lbl_eu_8066A1F4;
            CVec4::unitZ.w = lbl_eu_8066A1F4;

            CVec4::unit.x = lbl_eu_8066A1F4;
            CVec4::unit.y = lbl_eu_8066A1F4;
            CVec4::unit.z = lbl_eu_8066A1F4;
            CVec4::unit.w = lbl_eu_8066A1F4;
        }
    };

    static CVec4Sinit g_CVec4Sinit;
} //namespace ml
