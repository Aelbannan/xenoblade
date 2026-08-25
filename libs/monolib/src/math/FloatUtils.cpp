#include "monolib/math.hpp"
#include <cmath>
#include "monolib/math/FloatUtils.hpp"  // H3 label-owner decl (lbl_eu_8066A208)

// ============================================================================
// Blob monolibdata1.s dissolve: this TU owns the ENTIRE pooled .sdata2 range
// 0x8066A178-0x8066A360 (retail bytes below, exact). These are the shared
// float/double/string constants pooled by the retail monolib TUs (standard
// math constants, the 2^52 int->double conversion magics, the %-token format
// strings used by CToken's tokenizer and the RTTI name strings "Mem1"/"Mem2"/
// "CToken"/"CView"). They are defined here with the retail names so code in
// other TUs references them via `extern const float lbl_eu_...` instead of
// pooling TU-local @NNNN literals.
// ============================================================================

extern "C" const float lbl_eu_8066A178 = 0.5f;  // 0x3F000000

extern "C" const float lbl_eu_8066A17C = 40.7436637878418f;  // 0x4222F983

extern "C" const float lbl_eu_8066A180 = 1.0f;  // 0x3F800000

extern "C" const float lbl_eu_8066A184 = -1.0f;  // 0xBF800000

extern "C" const float lbl_eu_8066A188 = 0.02454369328916073f;  // 0x3CC90FDB

extern "C" const float lbl_eu_8066A18C = 0.0f;  // 0x00000000

extern "C" const float lbl_eu_8066A190 = 40.7436637878418f;  // 0x4222F983

extern "C" const float lbl_eu_8066A194 = -1.0f;  // 0xBF800000

extern "C" const float lbl_eu_8066A198 = 1.0f;  // 0x3F800000

extern "C" const float lbl_eu_8066A19C = 0.02454369328916073f;  // 0x3CC90FDB

extern "C" const u64 lbl_eu_8066A1A0 = 0;  // 0x0000000000000000  // 0x0000000000000000

extern "C" const char lbl_eu_8066A1A8[5] = "\x4D\x65\x6D\x31";  // 0x4D656D3100

extern "C" const char lbl_eu_8066A1B0[5] = "\x4D\x65\x6D\x32";  // 0x4D656D3200

extern "C" const double lbl_eu_8066A1B8 = 1.0;  // 0x3FF0000000000000

extern "C" const float lbl_eu_8066A1C0 = 0.0f;  // 0x00000000

extern "C" const float lbl_eu_8066A1C4 = 100.0f;  // 0x42C80000

extern "C" const u64 lbl_eu_8066A1C8 = 0x4330000000000000ULL;  // double 0x4330000000000000

extern "C" const float lbl_eu_8066A1D0 = 2.3283064365386963e-10f;  // 0x2F800000

extern "C" const u64 lbl_eu_8066A1D8 = 0x4330000000000000ULL;  // double 0x4330000000000000

extern "C" const float lbl_eu_8066A1E0[2] = { 0.5f, 0.0f };  // 0x3F00000000000000

extern "C" const float lbl_eu_8066A1E8 = 0.0f;  // 0x00000000

extern "C" const float lbl_eu_8066A1EC = 1.0f;  // 0x3F800000

extern "C" const float lbl_eu_8066A1F0 = 0.0f;  // 0x00000000

extern "C" const float lbl_eu_8066A1F4 = 1.0f;  // 0x3F800000

extern "C" const float lbl_eu_8066A1F8 = 3.1415927410125732f;  // 0x40490FDB

extern "C" const float lbl_eu_8066A1FC = 6.2831854820251465f;  // 0x40C90FDB

extern "C" const float lbl_eu_8066A200 = 1.5707963705062866f;  // 0x3FC90FDB

extern "C" const float lbl_eu_8066A204 = 0.7853981852531433f;  // 0x3F490FDB

extern "C" const float lbl_eu_8066A208 = 9.999999974752427e-07f;  // 0x358637BD

extern "C" const float lbl_eu_8066A20C = 57.2957763671875f;  // 0x42652EE0

extern "C" const float lbl_eu_8066A210 = 0.01745329238474369f;  // 0x3C8EFA35

extern "C" const float lbl_eu_8066A214[1] = { 1e14f };  // 0x56B5E621

extern "C" const float lbl_eu_8066A218[2] = { -100000000376832.0f, 0.0f };  // 0xD6B5E62100000000

extern "C" const float lbl_eu_8066A220 = 1.0f;  // 0x3F800000

extern "C" const float lbl_eu_8066A224 = 0.5f;  // 0x3F000000

extern "C" const u64 lbl_eu_8066A228 = 0;  // 0x0000000000000000  // 0x0000000000000000

extern "C" const float lbl_eu_8066A230 = 1.0f;  // 0x3F800000

extern "C" const float lbl_eu_8066A234 = 0.5f;  // 0x3F000000

extern "C" const u64 lbl_eu_8066A238 = 0;  // 0x0000000000000000  // 0x0000000000000000

extern "C" const float lbl_eu_8066A240 = 0.0f;  // 0x00000000

extern "C" const float lbl_eu_8066A244 = 0.5f;  // 0x3F000000

extern "C" const float lbl_eu_8066A248 = 1.0f;  // 0x3F800000

extern "C" const float lbl_eu_8066A24C = -1.0f;  // 0xBF800000

extern "C" const float lbl_eu_8066A250[2] = { 0.02454369328916073f, 0.0f };  // 0x3CC90FDB00000000

extern "C" const float lbl_eu_8066A258 = 0.0f;  // 0x00000000

extern "C" const float lbl_eu_8066A25C = 1.0f;  // 0x3F800000

extern "C" const u64 lbl_eu_8066A260 = 0;  // 0x0000000000000000  // 0x0000000000000000

extern "C" const float lbl_eu_8066A268 = 40.7436637878418f;  // 0x4222F983

extern "C" const float lbl_eu_8066A26C = 0.0f;  // 0x00000000

extern "C" const float lbl_eu_8066A270[2] = { 1.0f, 0.0f };  // 0x3F80000000000000

extern "C" const float lbl_eu_8066A278 = 0.6000000238418579f;  // 0x3F19999A

extern "C" const u64 lbl_eu_8066A280 = 0x4330000080000000ULL;  // double 0x4330000080000000

extern "C" const char lbl_eu_8066A288[5] = "\x3F\x3F\x3F\x3F";  // 0x3F3F3F3F00

extern "C" const char lbl_eu_8066A290[3] = "\x25\x66";  // 0x256600

extern "C" const char lbl_eu_8066A294[3] = "\x25\x64";  // 0x256400

extern "C" const char lbl_eu_8066A298[3] = "\x25\x78";  // 0x257800

extern "C" const char lbl_eu_8066A29C[4] = "\x25\x76\x33";  // 0x25763300

extern "C" const char lbl_eu_8066A2A0[4] = "\x25\x76\x61";  // 0x25766100

extern "C" const char lbl_eu_8066A2A4[4] = "\x25\x76\x34";  // 0x25763400

extern "C" const char lbl_eu_8066A2A8[4] = "\x25\x69\x33";  // 0x25693300

extern "C" const char lbl_eu_8066A2AC[4] = "\x25\x69\x34";  // 0x25693400

extern "C" const char lbl_eu_8066A2B0[6] = "\x25\x72\x65\x63\x74";  // 0x257265637400

extern "C" const char lbl_eu_8066A2B8[3] = "\x25\x73";  // 0x257300

extern "C" const u64 lbl_eu_8066A2C0 = 0x4330000080000000ULL;  // double 0x4330000080000000

extern "C" const char lbl_eu_8066A2C8[7] = "\x43\x54\x6F\x6B\x65\x6E";  // 0x43546F6B656E00

extern "C" const float lbl_eu_8066A2D0 = 1.0f;  // 0x3F800000

extern "C" const float lbl_eu_8066A2D4 = 0.6000000238418579f;  // 0x3F19999A

extern "C" const char lbl_eu_8066A2D8[6] = "\x43\x56\x69\x65\x77";  // 0x435669657700

extern "C" const float lbl_eu_8066A2E0 = 0.4000000059604645f;  // 0x3ECCCCCD

extern "C" const float lbl_eu_8066A2E4 = 0.5f;  // 0x3F000000

extern "C" const float lbl_eu_8066A2E8 = 0.0f;  // 0x00000000

extern "C" const float lbl_eu_8066A2EC = 0.5f;  // 0x3F000000

extern "C" const float lbl_eu_8066A2F0 = 1.0f;  // 0x3F800000

extern "C" const float lbl_eu_8066A2F4 = 0.800000011920929f;  // 0x3F4CCCCD

extern "C" const float lbl_eu_8066A2F8 = 0.4000000059604645f;  // 0x3ECCCCCD

extern "C" const float lbl_eu_8066A2FC = 0.6000000238418579f;  // 0x3F19999A

extern "C" const u64 lbl_eu_8066A300 = 0x4330000080000000ULL;  // double 0x4330000080000000

extern "C" const float lbl_eu_8066A308 = 0.1144770011305809f;  // 0x3DEA72EB

extern "C" const float lbl_eu_8066A30C = 0.298911988735199f;  // 0x3E990AFE

extern "C" const float lbl_eu_8066A310 = 0.5866109728813171f;  // 0x3F162C23

extern "C" const float lbl_eu_8066A314 = 0.699999988079071f;  // 0x3F333333

extern "C" const float lbl_eu_8066A318[2] = { 1.2000000476837158f, 0.0f };  // 0x3F99999A00000000

extern "C" const float lbl_eu_8066A320 = 0.15000000596046448f;  // 0x3E19999A

extern "C" const float lbl_eu_8066A324 = 0.0f;  // 0x00000000

extern "C" const float lbl_eu_8066A328 = 56.0f;  // 0x42600000

extern "C" const float lbl_eu_8066A32C = 150.0f;  // 0x43160000

extern "C" const float lbl_eu_8066A330 = 320.0f;  // 0x43A00000

extern "C" const float lbl_eu_8066A334 = 210.0f;  // 0x43520000

extern "C" const u64 lbl_eu_8066A338 = 0x4330000000000000ULL;  // double 0x4330000000000000

extern "C" const double lbl_eu_8066A340 = 0.03333333333333333;  // 0x3FA1111111111111

extern "C" const float lbl_eu_8066A348 = 16666.666015625f;  // 0x46823555

extern "C" const u64 lbl_eu_8066A350 = 0x4330000000000000ULL;  // double 0x4330000000000000

extern "C" const u64 lbl_eu_8066A358 = 0;  // 0x0000000000000000  // 0x0000000000000000
namespace ml{
    namespace math{
        bool isErrFloat(float f) {
            bool result = false;
            u32 val = FLOAT_TO_HEX(f);

            if (fpclassify(f) == FP_NAN || fpclassify(f) == FP_INFINITE || val == FLOAT_MAX_HEX_NEG ||
            val == FLOAT_MAX_HEX || val == FLOAT_NAN || f > lbl_eu_8066A214[0] || f < lbl_eu_8066A218[0]) {
                result = true;
            }

            return result;
        }
    } //namespace math
} //namespace ml
