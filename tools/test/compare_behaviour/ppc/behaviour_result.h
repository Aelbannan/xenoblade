#ifndef BEHAVIOUR_RESULT_H
#define BEHAVIOUR_RESULT_H

#include <types.h>

#define BEHAVIOUR_RESULT_MAGIC 0xBEEFCAFEu
#define BEHAVIOUR_LOG_ADDR  ((volatile char*)0x80003800)

typedef struct BehaviourResult {
    u32 magic;
    u32 passed;
    u32 failed;
    u32 exit_code;
    char last_fail[64];
} BehaviourResult;

extern volatile BehaviourResult g_behaviour_result;

void behaviour_log_fail(const char* scenario);
void behaviour_emit_summary(const char* test_id, volatile BehaviourResult* result);

static inline void behaviour_result_init(volatile BehaviourResult* result) {
    result->magic = BEHAVIOUR_RESULT_MAGIC;
    result->passed = 0;
    result->failed = 0;
    result->exit_code = 0;
    result->last_fail[0] = '\0';
}

#endif
