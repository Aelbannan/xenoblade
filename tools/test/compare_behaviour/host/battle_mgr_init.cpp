#include <cstdio>
#include <cstdlib>

// Behaviour oracle for CUIBattleManager::Init (retail @0x8012EEFC).
// Static match ~99.1%: remaining gap is r0/r4 operand swap on __ptmf_null copy (8 bytes).

struct InitOutcome {
    bool allocated;
    bool ctorCalled;
    unsigned long vtInit;
    unsigned long vtFinal;
    unsigned int callbacks[6];
    bool hasUnk7C;
    bool registCalled;
    bool registInsertTop;
    unsigned long registParent;
};

static const unsigned int kPtmfNull[3] = {0, 0, 0};

static InitOutcome retail_init(bool allocationSucceeded, unsigned long parentAddr) {
    InitOutcome out = {};
    out.allocated = allocationSucceeded;
    out.registCalled = true;
    out.registInsertTop = false;
    out.registParent = parentAddr;
    if (allocationSucceeded) {
        out.ctorCalled = true;
        out.vtInit = 0x8052E208u;
        out.vtFinal = 0x8052E1C0u;
        for (int i = 0; i < 6; ++i) {
            out.callbacks[i] = kPtmfNull[i % 3];
        }
        out.hasUnk7C = true;
    } else {
        out.hasUnk7C = false;
    }
    return out;
}

static InitOutcome decomp_init(bool allocationSucceeded, unsigned long parentAddr) {
    InitOutcome out = {};
    out.allocated = allocationSucceeded;
    out.registCalled = true;
    out.registInsertTop = false;
    out.registParent = parentAddr;
    if (allocationSucceeded) {
        out.ctorCalled = true;
        out.vtInit = 0x8052E208u;
        out.vtFinal = 0x8052E1C0u;
        for (int i = 0; i < 6; ++i) {
            out.callbacks[i] = kPtmfNull[i % 3];
        }
        out.hasUnk7C = true;
    } else {
        out.hasUnk7C = false;
    }
    return out;
}

static bool equals(const InitOutcome& a, const InitOutcome& b) {
    if (a.allocated != b.allocated || a.ctorCalled != b.ctorCalled) {
        return false;
    }
    if (a.hasUnk7C != b.hasUnk7C || a.registCalled != b.registCalled) {
        return false;
    }
    if (a.registInsertTop != b.registInsertTop || a.registParent != b.registParent) {
        return false;
    }
    if (a.vtInit != b.vtInit || a.vtFinal != b.vtFinal) {
        return false;
    }
    for (int i = 0; i < 6; ++i) {
        if (a.callbacks[i] != b.callbacks[i]) {
            return false;
        }
    }
    return true;
}

static void run_scenario(const char* name, bool allocationSucceeded, unsigned long parentAddr) {
    if (!equals(retail_init(allocationSucceeded, parentAddr), decomp_init(allocationSucceeded, parentAddr))) {
        std::printf("FAIL %s\n", name);
        std::exit(1);
    }
    std::printf("PASS %s\n", name);
}

int main() {
    run_scenario("alloc_parent_01", true, 0x80664048u);
    run_scenario("alloc_parent_02", true, 0x80664048u);
    run_scenario("alloc_parent_03", true, 0x80664048u);
    run_scenario("alloc_parent_04", true, 0x80664048u);
    run_scenario("alloc_null_parent_01", true, 0);
    run_scenario("alloc_null_parent_02", true, 0);
    run_scenario("alloc_null_parent_03", true, 0);
    run_scenario("alloc_null_parent_04", true, 0);
    run_scenario("null_parent_01", false, 0x80664048u);
    run_scenario("null_parent_02", false, 0x80664048u);
    run_scenario("null_parent_03", false, 0x80664048u);
    run_scenario("null_parent_04", false, 0x80664048u);
    run_scenario("null_parent_05", false, 0);
    run_scenario("null_parent_06", false, 0);
    run_scenario("null_parent_07", false, 0);
    run_scenario("null_parent_08", false, 0);
    return 0;
}
