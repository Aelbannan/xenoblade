#include <revolution/AX.h>
#include <revolution/AXFX.h>
#include <revolution/OS.h>

#include <string.h>

// Float/double constants in .sdata2 (named after their retail .sdata2 slots).
// volatile keeps MWCC from constant-folding the references so the emitted
// relocations keep the retail symbol names.
static const volatile f32 float_8066BED0 = 32.0f;
static const volatile f32 float_8066BED4 = 0.0f;
static const volatile f32 float_8066BEE0 = 0.1f;
static const volatile f32 float_8066BEE4 = 50.0f;
static const volatile f32 float_8066BEE8 = 1.0f;
static const volatile f32 float_8066BEEC = 2.0f;
static const volatile f32 float_8066BEF0 = 65536.0f;
static const volatile f32 float_8066BEF4 = 256.0f;
static const volatile f32 float_8066BEF8 = 32000.0f;
static const volatile f32 float_8066BEFC = 0.00390625f;
