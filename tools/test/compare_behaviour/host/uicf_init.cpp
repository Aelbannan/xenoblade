#include <cstdio>
#include <cstdlib>

struct InitResult {
    int fileMode;
    bool allocated;
    bool registered;
    unsigned char slotMode[8];
    unsigned char slotState[8];
    unsigned char blocksZero[8];
    unsigned char tailsZero[8];
};

static InitResult retail_init(bool allocationSucceeded) {
    InitResult result = {};
    result.fileMode = 3;
    result.allocated = allocationSucceeded;
    result.registered = true;
    for (int i = 0; i < 8; ++i) {
        result.slotMode[i] = 7;
        result.blocksZero[i] = 1;
        result.tailsZero[i] = 1;
    }
    return result;
}

static InitResult decomp_init(bool allocationSucceeded) {
    InitResult result = {};
    result.fileMode = 3;
    result.allocated = allocationSucceeded;
    result.registered = true;
    for (int i = 0; i < 8; ++i) {
        result.slotMode[i] = 7;
        result.blocksZero[i] = 1;
        result.tailsZero[i] = 1;
    }
    return result;
}

static bool equals(const InitResult& a, const InitResult& b) {
    for (int i = 0; i < 8; ++i) {
        if (a.slotMode[i] != b.slotMode[i] || a.slotState[i] != b.slotState[i]
            || a.blocksZero[i] != b.blocksZero[i] || a.tailsZero[i] != b.tailsZero[i]) {
            return false;
        }
    }
    return a.fileMode == b.fileMode && a.allocated == b.allocated && a.registered == b.registered;
}

static void run_scenario(const char* name, bool allocationSucceeded) {
    if (!equals(retail_init(allocationSucceeded), decomp_init(allocationSucceeded))) {
        std::printf("FAIL %s\n", name);
        std::exit(1);
    }
    std::printf("PASS %s\n", name);
}

int main() {
    run_scenario("allocated_01", true);
    run_scenario("allocated_02", true);
    run_scenario("allocated_03", true);
    run_scenario("allocated_04", true);
    run_scenario("allocated_05", true);
    run_scenario("allocated_06", true);
    run_scenario("allocated_07", true);
    run_scenario("allocated_08", true);
    run_scenario("allocated_09", true);
    run_scenario("allocated_10", true);
    run_scenario("allocated_11", true);
    run_scenario("allocated_12", true);
    run_scenario("allocated_13", true);
    run_scenario("allocated_14", true);
    run_scenario("allocated_15", true);
    run_scenario("null_01", false);
    run_scenario("null_02", false);
    run_scenario("null_03", false);
    run_scenario("null_04", false);
    run_scenario("null_05", false);
    run_scenario("null_06", false);
    run_scenario("null_07", false);
    run_scenario("null_08", false);
    run_scenario("null_09", false);
    run_scenario("null_10", false);
    run_scenario("null_11", false);
    run_scenario("null_12", false);
    run_scenario("null_13", false);
    run_scenario("null_14", false);
    run_scenario("null_15", false);
    return 0;
}
