// Translation unit: monolib/src/effect/CERand
//
// Real member-function definitions for the effect-system randomizers. The
// class declarations (with inline virtual bodies) live in
// monolib/effect/CERand.hpp; MWCC emits the out-of-line virtual copies and
// the vtables/RTTI in this TU because the constructors store the vptr here.
//
// The two randomizer objects live in foreign .bss/.sbss ranges in the retail
// DOL (dissolved into the CNReqtaskSave.o / CNReqtaskCheck.o blob slices at
// lbl_eu_80660028 / lbl_eu_80665A08), so they are accessed through those
// labels instead of being defined here (retail CERand.o owns no storage).

#include "monolib/effect/CERand.hpp"

using namespace ml;

// ---- Shared monolibdata2 sdata2 pool entries ----
extern double lbl_eu_8066B240; // signed s32->f64 magic (0x43300000_80000000), CERandomizer
extern double lbl_eu_8066B258; // signed s32->f64 magic (0x43300000_80000000), CERandomizerSimple
extern float lbl_eu_8066B234;  // 2147483648.0f (randF divisor)
extern float lbl_eu_8066B250;  // 10006.0f (Simple randF divisor)
extern float lbl_eu_8066B238;  // 0.5f
extern float lbl_eu_8066B248;  // 1.0f
extern float lbl_eu_8066B24C;  // -1.0f
extern float lbl_eu_8066B230;  // 0.0f

// ---- Dissolved storage of the global randomizer instances ----
extern u8 lbl_eu_80660028[16]; // ceRandomizerSimple (.bss, CNReqtaskSave.o slice)
extern u32 lbl_eu_80665A08[2]; // ceRandomizer (.sbss, CNReqtaskCheck.o slice)

// Vtables (retail .data of this TU).
extern u32 lbl_eu_8056FE08[]; // CERandomizerSimple
extern u32 lbl_eu_8056FE30[]; // CERandomizer

// Accessors for the two global randomizer instances, whose storage lives in
// foreign blob slices.
static CERandomizer* ceRandomizerRef() {
    return reinterpret_cast<CERandomizer*>(&lbl_eu_80665A08);
}

static CERandomizerSimple* ceRandomizerSimpleRef() {
    return reinterpret_cast<CERandomizerSimple*>(&lbl_eu_80660028);
}

// ---- CERandomizerSimple ----

CERandomizerSimple::CERandomizerSimple() {
    create(CERand::defaultSeed);
}

void CERandomizerSimple::create(int seed) {
    if (seed < 0) {
        seed1 = ceRandomizerSimpleRef()->seed1;
    } else {
        seed1 = (u16)seed;
    }
    seed2 = seed1;
    age = lbl_eu_8066B230;
}

void CERandomizerSimple::execute(float time) {
    float prevAge = age;
    age += time;
    if ((int)prevAge == (int)age) {
        seed1 = seed2;
    } else {
        seed2 = seed1;
        if (((int)age & 31) == 0) {
            rand();
        }
    }
}

// Never called; constructing the instance makes MWCC emit this TU's
// CERandomizer vtable and the out-of-line copies of its inline virtuals
// (retail keeps them here next to the Simple ones).
void KeepCERandomizerEmission() {
    CERandomizer dummy;
}

// ---- Hand-written static initializer ----

// Static initializer for the two global randomizer instances. Their storage
// is dissolved into foreign blob slices (see top-of-file note), so MWCC will
// not generate an auto-sinit; the retail __sinit_\CERand_cpp body is
// reproduced here and retargeted to the retail symbol by
// postprocess_reloc_names.py.
void CERandSinit() {
    ceRandomizerSimpleRef()->create(CERand::defaultSeed);
    *(void**)ceRandomizerSimpleRef() = (void*)&lbl_eu_8056FE08;
    *(void**)ceRandomizerRef() = (void*)&lbl_eu_8056FE30;
}

// ---- CERand statics ----

void CERand::init() {
    ceRandomizerSimpleRef()->create(CERand::defaultSeed);
}

void CERand::execute(float time) {
    ceRandomizerSimpleRef()->execute(time);
}

// randFHalf/randSign of CERandomizer are inlined into the vector helpers by
// retail; the same expressions against the shared pool constants are spelled
// out here.
void CERand::randVec(ml::CVec3* v) {
    union {
        f64 d;
        u32 w[2];
    } c;
    c.w[0] = 0x43300000u;
    c.w[1] = ml::math::mtRand() ^ 0x80000000u;
    v->x = (f32)(c.d - lbl_eu_8066B240) / lbl_eu_8066B234 - lbl_eu_8066B238;
    c.w[1] = ml::math::mtRand() ^ 0x80000000u;
    v->y = (f32)(c.d - lbl_eu_8066B240) / lbl_eu_8066B234 - lbl_eu_8066B238;
    c.w[1] = ml::math::mtRand() ^ 0x80000000u;
    v->z = (f32)(c.d - lbl_eu_8066B240) / lbl_eu_8066B234 - lbl_eu_8066B238;
}

void CERand::randSignVec(ml::CVec3* v) {
    v->x *= (ml::math::mtRand() % 2 != 0) ? lbl_eu_8066B248 : lbl_eu_8066B24C;
    v->y *= (ml::math::mtRand() % 2 != 0) ? lbl_eu_8066B248 : lbl_eu_8066B24C;
    v->z *= (ml::math::mtRand() % 2 != 0) ? lbl_eu_8066B248 : lbl_eu_8066B24C;
}
