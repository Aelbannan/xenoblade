#pragma once

#define FLOAT_LARGE 1e14f
#define FLOAT_SMALL -1e14f
#define FLOAT_MAX_HEX 0xD3D3D3D3
#define FLOAT_MAX_HEX_NEG 0xF3F3F3F3
#define FLOAT_NAN 0xFFFFFFFF
#define FLOAT_TO_HEX(f) (*(u32*)&f)

// ml::epsilon (.sdata2, 0x358637BD); defined in src/math/FloatUtils.cpp.
// Single canonical declaration for the whole tree (was re-declared ad hoc in
// dozens of TUs, some wrongly as u32).
extern "C" const float lbl_eu_8066A208;

namespace ml{
    namespace math{
        bool isErrFloat(float f);
    } //namespace math
} //namespace ml
