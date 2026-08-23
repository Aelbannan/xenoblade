// Translation unit: monolib/src/effect/CERand
// CERandomizer / CERandomizerSimple random generators + CERand statics.
//
// Implemented as extern "C" fragments + hand-written vtables/RTTI so this TU
// emits exactly the retail data (vtables + RTTI name strings + locators) and
// no compiler-generated __vt__/__RTTI__/vtable .data of its own. The two
// randomizer objects live in foreign .bss/.sbss ranges in the retail DOL
// (unlabeled globals at lbl_eu_80660028 / lbl_eu_80665A08), so they are
// referenced by address instead of being defined here (retail CERand.o has
// an empty .bss/.sbss).
//
// All float/double constants are referenced by their retail sdata2 names so
// no local .sdata2 pool is emitted (retail CERand.o .sdata2 is empty).

#include "monolib/effect/CERand.hpp"

using namespace ml;

// ---- Named sdata2 constants (retail pool) ----
extern double lbl_eu_8066B240; // 0x4330000080000000 (s32->f32 magic, CERandomizer)
extern double lbl_eu_8066B258; // 0x4330000080000000 (s32->f32 magic, Simple)
extern float lbl_eu_8066B234;  // 2147483648.0f (2^31, randF divisor)
extern float lbl_eu_8066B250;  // 10006.0f (Simple randF divisor)
extern float lbl_eu_8066B238;  // 0.5f
extern float lbl_eu_8066B248;  // 1.0f
extern float lbl_eu_8066B24C;  // -1.0f
extern float lbl_eu_8066B230[];  // g_fSeed1Table

// Local layout mirror of CERandomizerSimple (the real class keeps its fields
// private; the extern "C" fragments below address the object through this
// view, which matches the retail layout exactly: vptr 0x0, seed1 0x4, seed2
// 0x6, age 0x8).
struct CERandomizerSimpleLay {
    void* vptr;  // 0x00
    u16 seed1;   // 0x04
    u16 seed2;   // 0x06
    float age;   // 0x08
};

// Retail storage of the randomizer globals (foreign .bss/.sbss ranges). The
// Simple object is viewed through the layout mirror above (its fields are
// private in the real class).
extern CERandomizerSimpleLay lbl_eu_80660028;
extern CERandomizer lbl_eu_80665A08;

// ---- s32 -> f32 via the signed magic double (0x43300000 trick) ----
// Each caller builds the double 0x43300000_(x ^ 0x80000000) on the stack,
// subtracts the retail magic double, and divides - all kept in fp registers
// so the intermediate double never touches memory (retail fsubs + fdivs).
// The x ^ 0x80000000 word is assigned first, then the 0x43300000 word, so
// MWCC does not hoist the magic `lis` above the extern load (see MWCC_PATTERNS).

// Retail loads the object vptr, indexes entry 2 (rand), and calls it through
// ctr, the same shape the compiler emits for a virtual call.
static inline u32 simpleRand(CERandomizerSimpleLay* self) {
    typedef u32 (*Fn)(CERandomizerSimpleLay*);
    Fn* vtable = (Fn*)*(void**)self;
    return vtable[2](self);
}

// Forward declarations for the data block at the bottom (vtable/RTTI
// cross-references from the function fragments above). 
extern "C" u32 lbl_eu_8056FE08[];
extern "C" u32 lbl_eu_80663BB0[];
extern "C" u32 lbl_eu_80663BB8[];
extern "C" u32 lbl_eu_80663BC0[];
extern "C" u32 lbl_eu_8056FE20[];
extern "C" u32 lbl_eu_8056FE48[];

// ---- Function fragments (retail mangled names) ----

extern "C" void __ct__18CERandomizerSimpleFv(CERandomizerSimpleLay* self) {
    // seed1 = seed2 = 14992, age = 0.0f, vptr = CERandomizerSimple vtable.
    // age assigned first so its load hoists to the top of the block.
    self->age = lbl_eu_8066B230[0];
    self->seed1 = (u16)CERand::defaultSeed;
    self->seed2 = (u16)CERand::defaultSeed;
    *(void**)self = (void*)&lbl_eu_8056FE08;
}

extern "C" void create__18CERandomizerSimpleFi(CERandomizerSimpleLay* self, int seed) {
    if (seed < 0) {
        self->seed1 = lbl_eu_80660028.seed1;
    } else {
        self->seed1 = (u16)seed;
    }
    self->seed2 = self->seed1;
    self->age = lbl_eu_8066B230[self->seed1];
}

extern "C" void execute__18CERandomizerSimpleFf(CERandomizerSimpleLay* self, float time) {
    float prevAge = self->age;
    self->age += time;
    if ((int)prevAge == (int)self->age) {
        self->seed1 = self->seed2;
    } else {
        self->seed2 = self->seed1;
        if (((int)self->age & 0xE0000000u) == 0) {
            simpleRand(self);
        }
    }
}

extern "C" u32 rand__18CERandomizerSimpleFv(CERandomizerSimpleLay* self) {
    u32 temp = (u32)self->seed1 * 673 + 945;
    self->seed1 = (u16)((temp / 10) % 100003);
    return temp % 10007;
}

extern "C" float randF__18CERandomizerSimpleFv(CERandomizerSimpleLay* self) {
    return (float)simpleRand(self) / lbl_eu_8066B250;
}

extern "C" float randFHalf__18CERandomizerSimpleFv(CERandomizerSimpleLay* self) {
    return (float)simpleRand(self) / lbl_eu_8066B250 - lbl_eu_8066B238;
}

extern "C" float randSign__18CERandomizerSimpleFv(CERandomizerSimpleLay* self) {
    return (simpleRand(self) % 2 != 0) ? lbl_eu_8066B248 : lbl_eu_8066B24C;
}

extern "C" u32 rand__12CERandomizerFv(CERandomizer* self) {
    return ml::math::mtRand();
}

extern "C" float randF__12CERandomizerFv(CERandomizer* self) {
    return (float)ml::math::mtRand() / lbl_eu_8066B234;
}

extern "C" float randFHalf__12CERandomizerFv(CERandomizer* self) {
    return (float)ml::math::mtRand() / lbl_eu_8066B234 - lbl_eu_8066B238;
}

extern "C" float randSign__12CERandomizerFv(CERandomizer* self) {
    return (ml::math::mtRand() % 2 != 0) ? lbl_eu_8066B248 : lbl_eu_8066B24C;
}

extern "C" void init__6CERandFv() {
    create__18CERandomizerSimpleFi(&lbl_eu_80660028, CERand::defaultSeed);
}

extern "C" void execute__6CERandFf(float time) {
    float prevAge = lbl_eu_80660028.age;
    lbl_eu_80660028.age += time;
    if ((int)prevAge == (int)lbl_eu_80660028.age) {
        lbl_eu_80660028.seed1 = lbl_eu_80660028.seed2;
    } else {
        lbl_eu_80660028.seed2 = lbl_eu_80660028.seed1;
        if (((int)lbl_eu_80660028.age & 0xE0000000u) == 0) {
            simpleRand(&lbl_eu_80660028);
        }
    }
}

extern "C" void randVec__6CERandFPQ22ml5CVec3(ml::CVec3* v) {
    v->x = (float)ml::math::mtRand() / lbl_eu_8066B234 - lbl_eu_8066B238;
    v->y = (float)ml::math::mtRand() / lbl_eu_8066B234 - lbl_eu_8066B238;
    v->z = (float)ml::math::mtRand() / lbl_eu_8066B234 - lbl_eu_8066B238;
}

extern "C" void randSignVec__6CERandFPQ22ml5CVec3(ml::CVec3* v) {
    v->x *= randSign__12CERandomizerFv(&lbl_eu_80665A08);
    v->y *= randSign__12CERandomizerFv(&lbl_eu_80665A08);
    v->z *= randSign__12CERandomizerFv(&lbl_eu_80665A08);
}

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// [.rodata] 0x80524658-0x80524688 (48B): RTTI name strings.
extern "C" __declspec(align(8)) const char lbl_eu_80524658[20] = {
    0x43,0x45,0x52,0x61,0x6E,0x64,0x6F,0x6D,0x69,0x7A,0x65,0x72,0x53,0x69,0x6D,0x70,
    0x6C,0x65,0x00,0x00,
};
extern "C" __declspec(align(4)) const char lbl_eu_8052466C[12] = {
    0x49,0x52,0x61,0x6E,0x64,0x6F,0x6D,0x69,0x7A,0x65,0x72,0x00,
};
extern "C" __declspec(align(4)) const char lbl_eu_80524678[16] = {
    0x43,0x45,0x52,0x61,0x6E,0x64,0x6F,0x6D,0x69,0x7A,0x65,0x72,0x00,0x00,0x00,0x00,
};
DECOMP_FORCEACTIVE(CERand_cpp, lbl_eu_80524658);

// [.data] 0x8056FE08-0x8056FE58 (80B): vtables + RTTI base-lists.
extern "C" u32 lbl_eu_8056FE08[6] = {
    (u32)&lbl_eu_80663BB0, 0x00000000,
    (u32)&rand__18CERandomizerSimpleFv, (u32)&randF__18CERandomizerSimpleFv,
    (u32)&randFHalf__18CERandomizerSimpleFv, (u32)&randSign__18CERandomizerSimpleFv,
};
extern "C" u32 lbl_eu_8056FE20[4] = { (u32)&lbl_eu_80663BB8, 0x00000000, 0x00000000, 0x00000000 };
extern "C" u32 lbl_eu_8056FE30[6] = {
    (u32)&lbl_eu_80663BC0, 0x00000000,
    (u32)&rand__12CERandomizerFv, (u32)&randF__12CERandomizerFv,
    (u32)&randFHalf__12CERandomizerFv, (u32)&randSign__12CERandomizerFv,
};
extern "C" u32 lbl_eu_8056FE48[4] = { (u32)&lbl_eu_80663BB8, 0x00000000, 0x00000000, 0x00000000 };
DECOMP_FORCEACTIVE(CERand_cpp, lbl_eu_8056FE08);
DECOMP_FORCEACTIVE(CERand_cpp, lbl_eu_8056FE30);

// [.sdata] 0x80663BB0-0x80663BC8 (24B): RTTI locators.
extern "C" u32 lbl_eu_80663BB0[2] = { (u32)&lbl_eu_80524658, (u32)&lbl_eu_8056FE20 };
extern "C" u32 lbl_eu_80663BB8[2] = { (u32)&lbl_eu_8052466C, 0x00000000 };
extern "C" u32 lbl_eu_80663BC0[2] = { (u32)&lbl_eu_80524678, (u32)&lbl_eu_8056FE48 };
DECOMP_FORCEACTIVE(CERand_cpp, lbl_eu_80663BB0);
