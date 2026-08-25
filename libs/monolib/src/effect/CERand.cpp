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

// ---- Dissolved retail vtable/RTTI data (hand-built, retail layout) ----
//
// Retail CERand.o owns .rodata 0x80524658 (0x30: RTTI class-name strings),
// .data 0x8056FE08 (0x50: two vtables + two derived-typeinfo base lists) and
// .sdata 0x80663BB0 (0x18: three {name, base-list} type_info locators).
// Byte/reloc ground truth: build/us/asm/monolib/src/effect/CERand.s.
//
// The classes are NOT novtable here: constructing the throwaway CERandomizer
// below keeps MWCC emitting the out-of-line copies of the header-inline
// virtuals that retail keeps in this TU. That construction also makes MWCC
// emit ITS OWN vtable/RTTI/class-name copies (wrong order/names); they trail
// this block in each section and are tail-dropped by the CERand.o UNIT_RULES,
// leaving exactly the retail-named layout below.

extern "C" {
u32 rand__18CERandomizerSimpleFv();
float randF__18CERandomizerSimpleFv();
float randFHalf__18CERandomizerSimpleFv();
float randSign__18CERandomizerSimpleFv();
u32 rand__12CERandomizerFv();
float randF__12CERandomizerFv();
float randFHalf__12CERandomizerFv();
float randSign__12CERandomizerFv();
}

// Forward references (definitions below / cross-referenced).
extern "C" const char lbl_eu_80524658[];
extern "C" const char lbl_eu_8052466C[];
extern "C" const char lbl_eu_80524678[];
extern "C" u32 lbl_eu_8056FE20[4];
extern "C" u32 lbl_eu_8056FE48[4];
extern "C" u32 lbl_eu_80663BB0[2];
extern "C" u32 lbl_eu_80663BB8[2];
extern "C" u32 lbl_eu_80663BC0[2];

// [.rodata] 0x80524658 | sizes include retail align padding.
extern "C" __declspec(section ".rodata") __attribute__((aligned(8))) const char lbl_eu_80524658[0x14] = "CERandomizerSimple";
extern "C" __declspec(section ".rodata") const char lbl_eu_8052466C[0xC] = "IRandomizer";
extern "C" __declspec(section ".rodata") const char lbl_eu_80524678[0x10] = "CERandomizer";

// [.data] 0x8056FE08 | vtables {locator, 0, slots...} + base lists.
extern "C" u32 lbl_eu_8056FE08[6] __attribute__((aligned(8))) = {
    (u32)&lbl_eu_80663BB0, 0x00000000,
    (u32)&rand__18CERandomizerSimpleFv, (u32)&randF__18CERandomizerSimpleFv,
    (u32)&randFHalf__18CERandomizerSimpleFv, (u32)&randSign__18CERandomizerSimpleFv,
};
extern "C" u32 lbl_eu_8056FE20[4] = { (u32)&lbl_eu_80663BB8, 0, 0, 0 };
extern "C" u32 lbl_eu_8056FE30[6] = {
    (u32)&lbl_eu_80663BC0, 0x00000000,
    (u32)&rand__12CERandomizerFv, (u32)&randF__12CERandomizerFv,
    (u32)&randFHalf__12CERandomizerFv, (u32)&randSign__12CERandomizerFv,
};
extern "C" u32 lbl_eu_8056FE48[4] = { (u32)&lbl_eu_80663BB8, 0, 0, 0 };

// [.sdata] 0x80663BB0 | type_info locators {name, base-list}.
extern "C" __declspec(section ".sdata") __attribute__((aligned(8))) u32 lbl_eu_80663BB0[2] = {
    (u32)&lbl_eu_80524658, (u32)&lbl_eu_8056FE20,
};
extern "C" __declspec(section ".sdata") u32 lbl_eu_80663BB8[2] = {
    (u32)&lbl_eu_8052466C, 0,
};
extern "C" __declspec(section ".sdata") u32 lbl_eu_80663BC0[2] = {
    (u32)&lbl_eu_80524678, (u32)&lbl_eu_8056FE48,
};

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
