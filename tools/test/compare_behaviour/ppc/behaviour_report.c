#include <stdarg.h>
#include <types.h>

#include "behaviour_result.h"

#define BEHAVIOUR_LOG_ADDR ((volatile char*)0x80003800)
#define BEHAVIOUR_LOG_SIZE 2048u

static volatile u32 s_log_len;
volatile BehaviourResult g_behaviour_result;

void OSReport(const char* fmt, ...) {
    va_list args;
    const char* msg;
    u32 i;

    (void)fmt;
    va_start(args, fmt);
    msg = va_arg(args, const char*);
    va_end(args);

    if (msg == NULL) {
        return;
    }

    for (i = 0; msg[i] != '\0' && s_log_len + 1 < BEHAVIOUR_LOG_SIZE; ++i) {
        BEHAVIOUR_LOG_ADDR[s_log_len++] = msg[i];
    }
    if (s_log_len + 1 < BEHAVIOUR_LOG_SIZE) {
        BEHAVIOUR_LOG_ADDR[s_log_len++] = '\n';
    }
    BEHAVIOUR_LOG_ADDR[s_log_len] = '\0';
}

static void write_u32(char* out, u32* len, u32 value) {
    char tmp[16];
    int pos = 0;
    u32 n = value;

    if (n == 0) {
        out[(*len)++] = '0';
        return;
    }

    while (n > 0 && pos < 16) {
        tmp[pos++] = (char)('0' + (n % 10));
        n /= 10;
    }
    while (pos > 0) {
        out[(*len)++] = tmp[--pos];
    }
}

static void write_str(char* out, u32* len, const char* text) {
    while (text && *text) {
        out[(*len)++] = *text++;
    }
}

void behaviour_log_fail(const char* scenario) {
    char line[128];
    u32 len = 0;

    write_str(line, &len, "BEHAVIOUR_FAIL ");
    write_str(line, &len, scenario);
    line[len++] = '\0';
    OSReport("%s", line);
}

void behaviour_emit_summary(const char* test_id, volatile BehaviourResult* result) {
    char line[256];
    u32 len = 0;

    write_str(line, &len, "BEHAVIOUR_SUMMARY test=");
    write_str(line, &len, test_id);
    write_str(line, &len, " passed=");
    write_u32(line, &len, result->passed);
    write_str(line, &len, " failed=");
    write_u32(line, &len, result->failed);
    write_str(line, &len, " exit=");
    write_u32(line, &len, result->exit_code);
    write_str(line, &len, " last=");
    write_str(line, &len, (const char*)result->last_fail);
    line[len++] = '\n';
    line[len] = '\0';

    OSReport("%s", line);
}
