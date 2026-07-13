#include <cstdio>
#include <cstdint>

#include <types.h>

// MT19937 constants (retail MTRand uses this algorithm).
static constexpr int N = 624;
static constexpr int M = 397;

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

    u32 rand31() { return rand32() >> 1; }
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

    u32 rand31() { return rand32() >> 1; }
};

static void run_scenario(const char* name, u32 seed, int draws) {
    RetailMTRand retail{};
    DecompMTRand decomp{};
    retail.srand(seed);
    decomp.srand(seed);
    for (int i = 0; i < draws; ++i) {
        u32 r = retail.rand31();
        u32 d = decomp.rand31();
        if (r != d) {
            std::printf("FAIL %s at draw %d: %u vs %u\n", name, i, r, d);
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
    for (u32 seed = 2; seed < 18; ++seed) {
        char name[32];
        std::snprintf(name, sizeof(name), "seed_%u", seed);
        run_scenario(name, seed, 40);
    }
    std::printf("host mtrand_integer: all scenarios passed\n");
    return 0;
}
