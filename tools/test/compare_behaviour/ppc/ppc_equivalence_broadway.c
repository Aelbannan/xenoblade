#include <string.h>

#include <types.h>

#include "behaviour_result.h"
#include "ppc_equivalence_oracle.h"

extern void behaviour_emit_summary(const char* test_id, volatile BehaviourResult* result);
extern void OSReport(const char* fmt, ...);

static const char* const k_names[PPC_EQ_ORACLE_CASES] = {
    "addc", "addeo", "addmeo", "subfc", "subfeo", "nego",
    "mulhw", "mulhwu", "mullwo", "divw", "divwu",
    "slw31", "slw32", "srw", "sraw", "srawi",
    "rlwinm", "rlwimi", "rlwnm",
    "andc", "eqv", "nand", "nor", "orc", "xor",
    "extsb", "extsh", "cntlzw", "cmpw", "cmplw",
    "cr-logical", "xer-transfer",
    "stw-lbz-be", "lhz-be", "lha", "byte-reverse", "stwu", "lmw-stmw",
    "bc-cr", "bdnz"
};

#define T(result, cr, xer) result, cr, xer

static const u32 k_expected[PPC_EQ_ORACLE_CASES * PPC_EQ_WORDS_PER_CASE] = {
    T(0x00000000, 0x20000000, 0x20000000),
    T(0x80000000, 0x90000000, 0xC0000000),
    T(0x7FFFFFFF, 0x50000000, 0xE0000000),
    T(0xFFFFFFFF, 0x80000000, 0x00000000),
    T(0xFFFFFFFF, 0x80000000, 0x00000000),
    T(0x80000000, 0x90000000, 0xE0000000),
    T(0xFFFFFFFF, 0x80000000, 0x00000000),
    T(0x00000001, 0x40000000, 0x00000000),
    T(0x00000000, 0x30000000, 0xC0000000),
    T(0xFFFFFFFE, 0x80000000, 0x00000000),
    T(0x7FFFFFFF, 0x40000000, 0x00000000),
    T(0x80000000, 0x80000000, 0x00000000),
    T(0x00000000, 0x20000000, 0x00000000),
    T(0x00000001, 0x40000000, 0x00000000),
    T(0xFFFFFFFF, 0x80000000, 0x20000000),
    T(0xC0000000, 0x80000000, 0x20000000),
    T(0x0000000C, 0x40000000, 0x00000000),
    T(0xAA56CCDD, 0x80000000, 0x00000000),
    T(0x00000003, 0x40000000, 0x00000000),
    T(0x00F000F0, 0x40000000, 0x00000000),
    T(0x5A5A5A5A, 0x40000000, 0x00000000),
    T(0xF0F0FFFF, 0x80000000, 0x00000000),
    T(0x00FFFFFF, 0x40000000, 0x00000000),
    T(0x1234FFFF, 0x40000000, 0x00000000),
    T(0x55555555, 0x40000000, 0x00000000),
    T(0xFFFFFF80, 0x80000000, 0x00000000),
    T(0xFFFF8001, 0x80000000, 0x00000000),
    T(0x0000000F, 0x40000000, 0x00000000),
    T(0x00000000, 0x80000000, 0x00000000),
    T(0x00000000, 0x40000000, 0x00000000),
    T(0x54000000, 0x54000000, 0x00000000),
    T(0xE0000000, 0x00000000, 0xE0000000),
    T(0x00000011, 0x00000000, 0x00000000),
    T(0x00003344, 0x00000000, 0x00000000),
    T(0xFFFF8001, 0x00000000, 0x00000000),
    T(0x11223344, 0x00000000, 0x00000000),
    T(0x0000000C, 0x00000000, 0x00000000),
    T(0x00000004, 0x00000000, 0x00000000),
    T(0x00000000, 0x80000000, 0x00000000),
    T(0x00000001, 0x00000000, 0x00000000),
};

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

static void log_case_diff(const char* name, const u32* actual, const u32* expected) {
    char line[160];
    const char* text;
    u32 length = 0;
    int i;

#define APPEND_TEXT(value) \
    do { \
        text = (value); \
        while (*text != '\0') line[length++] = *text++; \
    } while (0)

    APPEND_TEXT("BEHAVIOUR_DIFF ");
    APPEND_TEXT(name);
    APPEND_TEXT(" actual=");
    for (i = 0; i < PPC_EQ_WORDS_PER_CASE; ++i) {
        if (i != 0) line[length++] = ',';
        append_hex_u32(line, &length, actual[i]);
    }
    APPEND_TEXT(" expected=");
    for (i = 0; i < PPC_EQ_WORDS_PER_CASE; ++i) {
        if (i != 0) line[length++] = ',';
        append_hex_u32(line, &length, expected[i]);
    }
    line[length] = '\0';
    OSReport("%s", line);

#undef APPEND_TEXT
}

int behaviour_main(void) {
    volatile BehaviourResult* result = &g_behaviour_result;
    u32 output[PPC_EQ_ORACLE_CASES * PPC_EQ_WORDS_PER_CASE];
    u32 memory[32];
    int case_index;
    int word;

    behaviour_result_init(result);
    memset(output, 0xA5, sizeof(output));
    memset(memory, 0, sizeof(memory));
    memory[4] = 1;
    memory[5] = 2;
    memory[6] = 3;
    memory[7] = 4;

    ppc_equivalence_oracle(output, memory);

    for (case_index = 0; case_index < PPC_EQ_ORACLE_CASES; ++case_index) {
        int mismatch = 0;
        for (word = 0; word < PPC_EQ_WORDS_PER_CASE; ++word) {
            int index = case_index * PPC_EQ_WORDS_PER_CASE + word;
            if (output[index] != k_expected[index]) {
                mismatch = 1;
            }
        }
        if (mismatch) {
            result->failed++;
            behaviour_log_fail(k_names[case_index]);
            log_case_diff(k_names[case_index], &output[case_index * PPC_EQ_WORDS_PER_CASE],
                          &k_expected[case_index * PPC_EQ_WORDS_PER_CASE]);
            set_last_fail(result, k_names[case_index]);
        } else {
            result->passed++;
        }
    }

    /* stwbrx byte order and the lmw/stmw destination block are also observed. */
    if (memory[2] != 0x44332211u || memory[12] != 1 || memory[13] != 2 ||
        memory[14] != 3 || memory[15] != 4) {
        result->failed++;
        behaviour_log_fail("memory-layout");
        set_last_fail(result, "memory-layout");
    } else {
        result->passed++;
    }

    result->exit_code = result->failed == 0 ? 0 : 1;
    behaviour_emit_summary("ppc-equivalence-broadway", result);
    return (int)result->exit_code;
}
