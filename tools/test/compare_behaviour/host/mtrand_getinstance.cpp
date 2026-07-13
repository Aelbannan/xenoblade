#include <cstdio>
#include <cstdint>
#include <cstring>

#include <types.h>

static constexpr int N = 624;

static u32 retail_u32_mul_add(u32 a, u32 b, u32 add) {
    return static_cast<u32>(static_cast<uint64_t>(a) * static_cast<uint64_t>(b) + add);
}

static void retail_srand(u32* state, int* left, BOOL* initialized, u32 seed) {
    if (seed == 0) {
        seed = 0x012BD6AAu;
    }
    state[0] = seed;
    for (int i = 1; i < N; ++i) {
        u32 mixed = state[i - 1] ^ (state[i - 1] >> 30);
        state[i] = retail_u32_mul_add(0x6C078965u, mixed, static_cast<u32>(i));
    }
    *left = 1;
    *initialized = true;
}

struct RetailMTRand {
    u32 state[N];
    int left;
    BOOL initialized;
    u32* pNext;
    u32 unk9D0;
};

static RetailMTRand s_retail_instance{};
static s8 s_retail_guard = 0;

static RetailMTRand* retail_getInstance() {
    if (!s_retail_guard) {
        retail_srand(s_retail_instance.state, &s_retail_instance.left, &s_retail_instance.initialized,
                     0x012BD6AAu);
        s_retail_instance.pNext = s_retail_instance.state;
        s_retail_guard = 1;
    }
    return &s_retail_instance;
}

struct DecompMTRand {
    u32 state[N];
    int left;
    BOOL initialized;
    u32* pNext;
    u32 unk9D0;
};

static DecompMTRand s_decomp_instance{};
static s8 s_decomp_guard = 0;

static DecompMTRand* decomp_getInstance() {
    if (!s_decomp_guard) {
        retail_srand(s_decomp_instance.state, &s_decomp_instance.left, &s_decomp_instance.initialized,
                     0x012BD6AAu);
        s_decomp_instance.pNext = s_decomp_instance.state;
        s_decomp_guard = 1;
    }
    return &s_decomp_instance;
}

static void reset_singletons() {
    std::memset(&s_retail_instance, 0, sizeof(s_retail_instance));
    std::memset(&s_decomp_instance, 0, sizeof(s_decomp_instance));
    s_retail_guard = 0;
    s_decomp_guard = 0;
}

static void run_scenario(const char* name) {
    reset_singletons();
    RetailMTRand* retail = retail_getInstance();
    DecompMTRand* decomp = decomp_getInstance();

    if (retail->state[0] != decomp->state[0] || retail->state[0] != 0x012BD6AAu) {
        std::printf("FAIL %s state0\n", name);
        std::exit(1);
    }
    if (retail->left != decomp->left || retail->left != 1) {
        std::printf("FAIL %s left\n", name);
        std::exit(1);
    }
    if (!retail->initialized || !decomp->initialized) {
        std::printf("FAIL %s initialized\n", name);
        std::exit(1);
    }
    if (retail->pNext != retail->state || decomp->pNext != decomp->state) {
        std::printf("FAIL %s pNext\n", name);
        std::exit(1);
    }
    if (s_retail_guard != 1 || s_decomp_guard != 1) {
        std::printf("FAIL %s guard\n", name);
        std::exit(1);
    }
    std::printf("PASS %s\n", name);
}

static void run_scenario_pointer_stable(const char* name, int extra_calls) {
    reset_singletons();
    RetailMTRand* first_retail = retail_getInstance();
    DecompMTRand* first_decomp = decomp_getInstance();
    RetailMTRand* last_retail = first_retail;
    DecompMTRand* last_decomp = first_decomp;

    for (int i = 0; i < extra_calls; ++i) {
        last_retail = retail_getInstance();
        last_decomp = decomp_getInstance();
    }

    if (first_retail != last_retail || first_decomp != last_decomp) {
        std::printf("FAIL %s pointer\n", name);
        std::exit(1);
    }
    if (first_retail->state[1] != last_retail->state[1] || first_decomp->state[1] != last_decomp->state[1]) {
        std::printf("FAIL %s state preserved\n", name);
        std::exit(1);
    }
    std::printf("PASS %s\n", name);
}

static void run_scenario_no_reinit(const char* name) {
    reset_singletons();
    retail_getInstance();
    decomp_getInstance();
    s_retail_instance.state[0] = 0xCAFEBABEu;
    s_decomp_instance.state[0] = 0xCAFEBABEu;
    retail_getInstance();
    decomp_getInstance();
    if (s_retail_instance.state[0] != 0xCAFEBABEu || s_decomp_instance.state[0] != 0xCAFEBABEu) {
        std::printf("FAIL %s reinit\n", name);
        std::exit(1);
    }
    std::printf("PASS %s\n", name);
}

static void run_scenario_state_tail(const char* name, u32 expected) {
    reset_singletons();
    RetailMTRand* retail = retail_getInstance();
    DecompMTRand* decomp = decomp_getInstance();
    u32 retailState1 = static_cast<u32>(static_cast<uint32_t>(retail->state[1]));
    u32 decompState1 = static_cast<u32>(static_cast<uint32_t>(decomp->state[1]));
    if (retailState1 != decompState1 || retailState1 != expected) {
        std::printf("FAIL %s state1 expected=%lu retail=%lu decomp=%lu\n", name,
                    static_cast<unsigned long>(expected), static_cast<unsigned long>(retailState1),
                    static_cast<unsigned long>(decompState1));
        std::exit(1);
    }
    std::printf("PASS %s\n", name);
}

int main() {
    run_scenario("first_call_init");
    run_scenario_pointer_stable("second_call_same_ptr", 1);
    run_scenario_pointer_stable("five_calls_same_ptr", 4);
    run_scenario_pointer_stable("ten_calls_same_ptr", 9);
    run_scenario_no_reinit("no_reinit_on_repeat");
    run_scenario_state_tail("state1_after_seed", 0x82D2AB13u);
    run_scenario("fresh_reset_init");
    run_scenario_pointer_stable("repeat_after_reset", 2);
    std::printf("host mtrand_getinstance: all scenarios passed\n");
    return 0;
}
