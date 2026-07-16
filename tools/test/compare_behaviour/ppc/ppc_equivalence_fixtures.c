#include <string.h>

#include <types.h>

#include "behaviour_result.h"
#include "ppc_fixture_exec.h"

extern void behaviour_emit_summary(const char* test_id, volatile BehaviourResult* result);
extern void OSReport(const char* fmt, ...);

#define SANDBOX_MEM1_ADDR 0x80004000u
#define PYTHON_SANDBOX_BASE 0x00001000u
#define PYTHON_SANDBOX_LIMIT (PYTHON_SANDBOX_BASE + (PPC_FIXTURE_MEM_WORDS * 4u))

static void set_last_fail(volatile BehaviourResult* result, const char* name) {
    int i;
    for (i = 0; i < 63 && name[i] != '\0'; ++i) {
        result->last_fail[i] = name[i];
    }
    result->last_fail[i] = '\0';
}

static void append_hex_u32(char* line, u32* length, u32 value) {
    static const char digits[] = "0123456789ABCDEF";
    int shift;

    for (shift = 28; shift >= 0; shift -= 4) {
        line[(*length)++] = digits[(value >> shift) & 0xFu];
    }
}

static void log_case_diff(const char* name, u32 ar, u32 ac, u32 ax, u32 er, u32 ec, u32 ex) {
    char line[192];
    const char* text;
    u32 length = 0;

#define APPEND_TEXT(value) \
    do { \
        text = (value); \
        while (*text != '\0') line[length++] = *text++; \
    } while (0)

    APPEND_TEXT("BEHAVIOUR_DIFF ");
    APPEND_TEXT(name);
    APPEND_TEXT(" actual=");
    append_hex_u32(line, &length, ar);
    line[length++] = ',';
    append_hex_u32(line, &length, ac);
    line[length++] = ',';
    append_hex_u32(line, &length, ax);
    APPEND_TEXT(" expected=");
    append_hex_u32(line, &length, er);
    line[length++] = ',';
    append_hex_u32(line, &length, ec);
    line[length++] = ',';
    append_hex_u32(line, &length, ex);
    line[length] = '\0';
    OSReport("%s", line);

#undef APPEND_TEXT
}

static void log_fp_diff(const char* name, u32 index, u64 actual, u64 expected) {
    OSReport(
        "BEHAVIOUR_FP_DIFF %s f%u actual=%08X%08X expected=%08X%08X",
        name,
        index,
        (u32)(actual >> 32),
        (u32)actual,
        (u32)(expected >> 32),
        (u32)expected
    );
}

int behaviour_main(void) {
    volatile BehaviourResult* result = &g_behaviour_result;
    u32* sandbox = (u32*)SANDBOX_MEM1_ADDR;
    PpcFixtureActual actual;
    int case_index;
    u32 mem_index;
    u32 fpr_index;
    u32 gqr_index;
    u32 gpr_index;
    int mismatch;

    behaviour_result_init(result);

    for (case_index = 0; case_index < PPC_FIXTURE_COUNT; ++case_index) {
        const PpcFixtureCase* fixture = &g_ppc_fixtures[case_index];

        memset(&actual, 0xA5, sizeof(actual));
        ppc_fixture_run_case(fixture, sandbox, &actual);

        mismatch = 0;
        if (actual.result != fixture->expected_result || actual.cr != fixture->expected_cr ||
            actual.xer != fixture->expected_xer) {
            mismatch = 1;
        }
        for (mem_index = 0; mem_index < fixture->expected_mem_count; ++mem_index) {
            u32 offset = fixture->expected_mem_off[mem_index];
            u32 word_index = offset / 4u;
            if (word_index >= PPC_FIXTURE_MEM_WORDS || actual.mem[word_index] != fixture->expected_mem_val[mem_index]) {
                mismatch = 1;
            }
        }
        for (fpr_index = 0; fpr_index < fixture->expected_fpr_count; ++fpr_index) {
            u32 index = fixture->expected_fpr_index[fpr_index];
            if (index >= 32u || actual.fpr[index] != fixture->expected_fpr_value[fpr_index]) {
                mismatch = 1;
                if (index < 32u) {
                    log_fp_diff(fixture->name, index, actual.fpr[index], fixture->expected_fpr_value[fpr_index]);
                    log_case_diff(
                        fixture->name,
                        (u32)(actual.fpr[index] >> 32),
                        (u32)actual.fpr[index],
                        actual.fpscr,
                        (u32)(fixture->expected_fpr_value[fpr_index] >> 32),
                        (u32)fixture->expected_fpr_value[fpr_index],
                        fixture->expected_fpscr
                    );
                }
            }
        }
        if (fixture->observe_fpscr && actual.fpscr != fixture->expected_fpscr) {
            mismatch = 1;
            OSReport(
                "BEHAVIOUR_FPSCR_DIFF %s actual=%08X expected=%08X",
                fixture->name,
                actual.fpscr,
                fixture->expected_fpscr
            );
            log_case_diff(
                fixture->name,
                0,
                0,
                actual.fpscr,
                0,
                0,
                fixture->expected_fpscr
            );
        }
        for (gqr_index = 0; gqr_index < fixture->expected_gqr_count; ++gqr_index) {
            u32 idx = fixture->expected_gqr_index[gqr_index];
            if (actual.gqr[idx] != fixture->expected_gqr_value[gqr_index]) {
                mismatch = 1;
                OSReport(
                    "BEHAVIOUR_GQR_DIFF %s gqr%u actual=%08X expected=%08X",
                    fixture->name,
                    idx,
                    actual.gqr[idx],
                    fixture->expected_gqr_value[gqr_index]
                );
            }
        }
        for (gpr_index = 0; gpr_index < fixture->expected_gpr_count; ++gpr_index) {
            u32 idx = fixture->expected_gpr_index[gpr_index];
            u32 expected = fixture->expected_gpr_value[gpr_index];
            if (idx == 1 || idx == 2 || idx == 13 || idx == 14 || idx == 15) {
                continue;
            }
            if (expected >= PYTHON_SANDBOX_BASE && expected < PYTHON_SANDBOX_LIMIT) {
                expected = (expected - PYTHON_SANDBOX_BASE) + SANDBOX_MEM1_ADDR;
            }
            if (actual.gpr[idx] != expected) {
                mismatch = 1;
                OSReport(
                    "BEHAVIOUR_GPR_DIFF %s r%u actual=%08X expected=%08X",
                    fixture->name,
                    idx,
                    actual.gpr[idx],
                    expected
                );
            }
        }
        if (fixture->observe_lr && actual.lr != fixture->expected_lr) {
            mismatch = 1;
            OSReport(
                "BEHAVIOUR_LR_DIFF %s actual=%08X expected=%08X",
                fixture->name,
                actual.lr,
                fixture->expected_lr
            );
        }
        if (fixture->observe_ctr && actual.ctr != fixture->expected_ctr) {
            mismatch = 1;
            OSReport(
                "BEHAVIOUR_CTR_DIFF %s actual=%08X expected=%08X",
                fixture->name,
                actual.ctr,
                fixture->expected_ctr
            );
        }

        if (mismatch) {
            result->failed++;
            behaviour_log_fail(fixture->name);
            log_case_diff(
                fixture->name,
                actual.result,
                actual.cr,
                actual.xer,
                fixture->expected_result,
                fixture->expected_cr,
                fixture->expected_xer
            );
            set_last_fail(result, fixture->name);
        } else {
            result->passed++;
        }
    }

    /* Require every fixture to report a pass; zero executed cases is a hard fail. */
    result->exit_code =
        (result->failed == 0 && result->passed == (u32)PPC_FIXTURE_COUNT) ? 0 : 1;
    behaviour_emit_summary("ppc-equivalence-fixtures", result);
    return (int)result->exit_code;
}
