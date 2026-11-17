// Effect-system randomizers.
//
// The class declarations (with their inline virtuals) live in
// monolib/effect/CERand.hpp; this TU defines the two global instances, so the
// vtables, RTTI and out-of-line copies of the inline virtuals are emitted here.

#include "monolib/effect/CERand.hpp"

using namespace ml;

CERandomizer ceRandomizer;
CERandomizerSimple ceRandomizerSimple;

// ---- CERandomizerSimple ----

CERandomizerSimple::CERandomizerSimple() {
    create(CERand::defaultSeed);
}

void CERandomizerSimple::create(int seed) {
    if (seed < 0) {
        seed1 = ceRandomizerSimple.seed1;
    } else {
        seed1 = (u16)seed;
    }
    seed2 = seed1;
    age = 0.0f;
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

// ---- CERand statics ----

void CERand::init() {
    ceRandomizerSimple.create(CERand::defaultSeed);
}

void CERand::execute(float time) {
    ceRandomizerSimple.execute(time);
}

void CERand::randVec(ml::CVec3* v) {
    v->x = ceRandomizer.randFHalf();
    v->y = ceRandomizer.randFHalf();
    v->z = ceRandomizer.randFHalf();
}

void CERand::randSignVec(ml::CVec3* v) {
    v->x *= ceRandomizer.randSign();
    v->y *= ceRandomizer.randSign();
    v->z *= ceRandomizer.randSign();
}
