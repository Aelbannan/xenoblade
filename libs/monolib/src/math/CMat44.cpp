#include "monolib/math.hpp"

extern const float lbl_eu_8066A258; // 0.0f
extern const float lbl_eu_8066A25C; // 1.0f

namespace ml{
    // The retail split keeps CMat44::zero/identity in the shared monolibdata1f
    // .bss (0x80656D88/0x80656DB8); this TU only *references* them.  Defining
    // them here would create a local .bss (data diff FAIL) and duplicate the
    // symbol at link time.  The retail __sinit_\CMat44_cpp still lives in this
    // TU's .text: it is reproduced by the auto-generated __sinit_ from the
    // CMat44Sinit global below (same store sequence, byte-identical).
    extern CMat44 CMat44::zero;
    extern CMat44 CMat44::identity;

    // Auto-__sinit_ trigger: a global object with an inlined constructor that
    // writes the retail values into the extern statics.  MWCC inlines this
    // ctor into __sinit_\CMat44_cpp, emitting the exact retail store sequence
    // (zero block first, then identity) with the sdata2 constants referenced
    // via the named lbl_eu_8066A258/25C symbols.
    struct CMat44Sinit {
        CMat44Sinit() {
            CMat44::zero.m[0][0] = lbl_eu_8066A258;
            CMat44::zero.m[0][1] = lbl_eu_8066A258;
            CMat44::zero.m[0][2] = lbl_eu_8066A258;
            CMat44::zero.m[0][3] = lbl_eu_8066A258;
            CMat44::zero.m[1][0] = lbl_eu_8066A258;
            CMat44::zero.m[1][1] = lbl_eu_8066A258;
            CMat44::zero.m[1][2] = lbl_eu_8066A258;
            CMat44::zero.m[1][3] = lbl_eu_8066A258;
            CMat44::zero.m[2][0] = lbl_eu_8066A258;
            CMat44::zero.m[2][1] = lbl_eu_8066A258;
            CMat44::zero.m[2][2] = lbl_eu_8066A258;
            CMat44::zero.m[2][3] = lbl_eu_8066A258;
            CMat44::zero.m[3][0] = lbl_eu_8066A258;
            CMat44::zero.m[3][1] = lbl_eu_8066A258;
            CMat44::zero.m[3][2] = lbl_eu_8066A258;
            CMat44::zero.m[3][3] = lbl_eu_8066A258;

            CMat44::identity.m[0][0] = lbl_eu_8066A25C;
            CMat44::identity.m[0][1] = lbl_eu_8066A258;
            CMat44::identity.m[0][2] = lbl_eu_8066A258;
            CMat44::identity.m[0][3] = lbl_eu_8066A258;
            CMat44::identity.m[1][0] = lbl_eu_8066A258;
            CMat44::identity.m[1][1] = lbl_eu_8066A25C;
            CMat44::identity.m[1][2] = lbl_eu_8066A258;
            CMat44::identity.m[1][3] = lbl_eu_8066A258;
            CMat44::identity.m[2][0] = lbl_eu_8066A258;
            CMat44::identity.m[2][1] = lbl_eu_8066A258;
            CMat44::identity.m[2][2] = lbl_eu_8066A25C;
            CMat44::identity.m[2][3] = lbl_eu_8066A258;
            CMat44::identity.m[3][0] = lbl_eu_8066A258;
            CMat44::identity.m[3][1] = lbl_eu_8066A258;
            CMat44::identity.m[3][2] = lbl_eu_8066A258;
            CMat44::identity.m[3][3] = lbl_eu_8066A25C;
        }
    };

    static CMat44Sinit g_CMat44Sinit;
} //namespace ml
