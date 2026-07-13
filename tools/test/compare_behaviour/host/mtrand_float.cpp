#include <cmath>
#include <cstdio>
#include <cstring>

#include <types.h>

// MT19937 + retail randFloat conversion (804384F4 int-to-double trick).

static constexpr int N = 624;
static constexpr int M = 397;
static constexpr f32 kScale = 2.32830643653869628906e-10f; // lbl_eu_8066A1D0

// Gekko 0x43300000 int-to-double trick: lfd after (high=0x43300000, low=bits) then
// fsubs lbl_eu_8066A1D8 yields (f64)bits exactly; fmuls lbl_eu_8066A1D0 finishes.
static f32 retail_float_from_bits(u32 bits) {
    return (f32)((f64)bits * (f64)kScale);
}

static inline u32 twist(u32 o, u32 m, u32 k) {
    return (((o & 0x80000000u) | (m & 0x7fffffffu)) >> 1) ^ (k & 1 ? 0x9908b0dfu : 0);
}

struct RetailMTRand {
    u32 state[N];
    int left;
    u32* pNext;
    bool initialized;

    void srand(u32 seed) {
        if (seed == 0) {
            seed = 0x012BD6AAu;
        }
        state[0] = seed;
        for (int i = 1; i < N; ++i) {
            state[i] = 0x6C078965u * (state[i - 1] ^ (state[i - 1] >> 30)) + (u32)i;
        }
        left = 1;
        initialized = true;
    }

    void nextMt() {
        if (!initialized) {
            srand(0x1571u);
        }
        left = N;
        pNext = state;
        int i;
        u32* p = state;
        for (i = N - M + 1; --i; ++p) {
            *p = twist(p[M], p[0], p[1]);
        }
        for (i = M; --i; ++p) {
            *p = twist(p[M - N], p[0], p[1]);
        }
        *p = twist(p[M - N], p[0], state[0]);
    }

    u32 rand32() {
        left--;
        if (left <= 0) {
            nextMt();
        }
        u32 r4 = *pNext++;
        r4 ^= (r4 >> 0xB);
        r4 ^= (r4 << 7) & 0x9D2C5680u;
        r4 ^= (r4 << 0xF) & 0xEFC60000u;
        r4 ^= r4 >> 0x12;
        return r4;
    }

    f32 randFloat() { return retail_float_from_bits(rand32()); }
};

struct DecompMTRand {
    u32 state[N];
    int left;
    u32* pNext;
    bool initialized;

    void srand(u32 seed) {
        if (seed == 0) {
            seed = 0x012BD6AAu;
        }
        state[0] = seed;
        for (int i = 1; i < N; ++i) {
            state[i] = 0x6C078965u * (state[i - 1] ^ (state[i - 1] >> 30)) + (u32)i;
        }
        left = 1;
        initialized = true;
    }

    void nextMt() {
        if (!initialized) {
            srand(0x1571u);
        }
        left = N;
        pNext = state;
        int i;
        u32* p = state;
        for (i = N - M + 1; --i; ++p) {
            *p = twist(p[M], p[0], p[1]);
        }
        for (i = M; --i; ++p) {
            *p = twist(p[M - N], p[0], p[1]);
        }
        *p = twist(p[M - N], p[0], state[0]);
    }

    u32 rand32() {
        left--;
        if (left <= 0) {
            nextMt();
        }
        u32 r4 = *pNext++;
        r4 ^= (r4 >> 0xB);
        r4 ^= (r4 << 7) & 0x9D2C5680u;
        r4 ^= (r4 << 0xF) & 0xEFC60000u;
        r4 ^= r4 >> 0x12;
        return r4;
    }

    // Keep in sync with libs/monolib/src/math/MTRand.cpp::randFloat().
    f32 randFloat() { return (f32)rand32() * kScale; }
};

static void run_scenario(const char* name, u32 seed, int draws) {
    RetailMTRand retail{};
    DecompMTRand decomp{};
    retail.srand(seed);
    decomp.srand(seed);
    for (int i = 0; i < draws; ++i) {
        const f32 r = retail.randFloat();
        const f32 d = decomp.randFloat();
        if (r != d) {
            std::printf("FAIL %s at draw %d: retail=%a decomp=%a\n", name, i, r, d);
            std::exit(1);
        }
        if (!(r >= 0.0f && r < 1.0f)) {
            std::printf("FAIL %s range at draw %d: %a\n", name, i, r);
            std::exit(1);
        }
    }
    std::printf("PASS %s\n", name);
}

int main() {
    run_scenario("seed_default", 0x012BD6AAu, 32);
    run_scenario("seed_zero_maps", 0u, 16);
    run_scenario("seed_one", 1u, 64);
    run_scenario("seed_1571", 0x1571u, 64);
    run_scenario("seed_deadbeef", 0xDEADBEEFu, 128);
    run_scenario("long_run", 0x12345678u, 700);
    run_scenario("twist_boundary", 0xABCDEF01u, 624);
    run_scenario("seed_18", 18u, 20);
    run_scenario("seed_19", 19u, 20);
    run_scenario("seed_20", 20u, 20);
    run_scenario("seed_21", 21u, 20);
    run_scenario("seed_22", 22u, 20);
    run_scenario("seed_cafe", 0xCAFEu, 32);
    run_scenario("seed_beef", 0xBEEFu, 32);
    run_scenario("seed_face", 0xFACEu, 32);
    run_scenario("seed_9999", 0x9999u, 48);
    run_scenario("seed_abc", 0xABCDEFu, 48);
    run_scenario("seed_ff00", 0xFF00FF00u, 48);
    for (u32 seed = 2; seed < 14; ++seed) {
        char name[32];
        std::snprintf(name, sizeof(name), "seed_%u", seed);
        run_scenario(name, seed, 40);
    }
    for (u32 seed = 100; seed < 108; ++seed) {
        char name[32];
        std::snprintf(name, sizeof(name), "seed_high_%u", seed);
        run_scenario(name, seed, 24);
    }
    std::printf("host mtrand_float: all scenarios passed\n");
    return 0;
}
