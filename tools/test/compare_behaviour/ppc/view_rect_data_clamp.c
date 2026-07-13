#include <string.h>

#include <types.h>

#include "behaviour_result.h"

typedef struct CViewRectDataCore {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
} CViewRectDataCore;

typedef struct CPnt16 {
    s16 x;
    s16 y;
} CPnt16;

extern void behaviour_emit_summary(const char* test_id, volatile BehaviourResult* result);
extern void OSReport(const char* fmt, ...);

extern void rb_func_804592F0__17CViewRectDataCoreFRCQ22ml6CPnt16(CViewRectDataCore* self, const CPnt16* size);
extern void dc_func_804592F0__17CViewRectDataCoreFRCQ22ml6CPnt16(CViewRectDataCore* self, const CPnt16* size);

static int compare_state(const CViewRectDataCore* a, const CViewRectDataCore* b) {
    const u8* left = (const u8*)a;
    const u8* right = (const u8*)b;
    u32 i;

    for (i = 0; i < sizeof(CViewRectDataCore); ++i) {
        if (left[i] != right[i]) {
            return 1;
        }
    }
    return 0;
}

static void log_state_diff(const char* name, const CViewRectDataCore* retail, const CViewRectDataCore* decomp) {
    char line[160];
    u32 len = 0;
    const char* prefix = "BEHAVIOUR_DIFF ";
    const s16* r = (const s16*)retail;
    const s16* d = (const s16*)decomp;
    int i;

    for (i = 0; prefix[i] != '\0'; ++i) {
        line[len++] = prefix[i];
    }
    for (i = 0; name[i] != '\0' && len < 40; ++i) {
        line[len++] = name[i];
    }
    line[len++] = ' ';
    for (i = 0; i < 7; ++i) {
        char digits[8];
        int pos = 0;
        s16 rv = r[i];
        s16 dv = d[i];
        u32 n;
        int j;

        if (rv < 0) {
            digits[pos++] = '-';
            n = (u32)(-(s32)rv);
        } else {
            n = (u32)rv;
        }
        if (n == 0) {
            digits[pos++] = '0';
        } else {
            while (n > 0 && pos < 7) {
                digits[pos++] = (char)('0' + (n % 10));
                n /= 10;
            }
        }
        while (pos > 0) {
            line[len++] = digits[--pos];
        }
        line[len++] = '/';
        pos = 0;
        if (dv < 0) {
            digits[pos++] = '-';
            n = (u32)(-(s32)dv);
        } else {
            n = (u32)dv;
        }
        if (n == 0) {
            digits[pos++] = '0';
        } else {
            while (n > 0 && pos < 7) {
                digits[pos++] = (char)('0' + (n % 10));
                n /= 10;
            }
        }
        while (pos > 0) {
            line[len++] = digits[--pos];
        }
        if (i + 1 < 7) {
            line[len++] = ',';
        }
    }
    line[len] = '\0';
    OSReport("%s", line);
}

static void run_scenario(volatile BehaviourResult* result, const char* name, const CViewRectDataCore* seed, s16 sx,
                         s16 sy) {
    CViewRectDataCore retail;
    CViewRectDataCore decomp;
    CPnt16 size;
    int i;

    size.x = sx;
    size.y = sy;
    memcpy(&retail, seed, sizeof(CViewRectDataCore));
    memcpy(&decomp, seed, sizeof(CViewRectDataCore));
    rb_func_804592F0__17CViewRectDataCoreFRCQ22ml6CPnt16(&retail, &size);
    dc_func_804592F0__17CViewRectDataCoreFRCQ22ml6CPnt16(&decomp, &size);

    if (compare_state(&retail, &decomp) != 0) {
        result->failed++;
        behaviour_log_fail(name);
        log_state_diff(name, &retail, &decomp);
        for (i = 0; i < 63 && name[i] != '\0'; ++i) {
            result->last_fail[i] = name[i];
        }
        result->last_fail[63] = '\0';
        return;
    }
    result->passed++;
}

static CViewRectDataCore make_base(void) {
    CViewRectDataCore base;

    memset(&base, 0, sizeof(base));
    base.unk4 = 640;
    base.unk6 = 480;
    base.unk8 = 400;
    base.unkA = 300;
    return base;
}

static CViewRectDataCore with_neg_unk8(const CViewRectDataCore* base) {
    CViewRectDataCore s = *base;
    s.unk8 = -5;
    return s;
}

static CViewRectDataCore with_neg_unkA(const CViewRectDataCore* base) {
    CViewRectDataCore s = *base;
    s.unkA = -3;
    return s;
}

static CViewRectDataCore with_large_margins(const CViewRectDataCore* base) {
    CViewRectDataCore s = *base;
    s.unkC = 200;
    s.unkE = 150;
    s.unk10 = 100;
    s.unk12 = 80;
    return s;
}

static CViewRectDataCore with_tight_width(const CViewRectDataCore* base) {
    CViewRectDataCore s = *base;
    s.unk8 = 600;
    s.unk4 = 500;
    return s;
}

static CViewRectDataCore with_tight_height(const CViewRectDataCore* base) {
    CViewRectDataCore s = *base;
    s.unkA = 450;
    s.unk6 = 400;
    return s;
}

static CViewRectDataCore with_both_negative(const CViewRectDataCore* base) {
    CViewRectDataCore s = *base;
    s.unk8 = -20;
    s.unkA = -15;
    return s;
}

int behaviour_main(void) {
    volatile BehaviourResult* result = &g_behaviour_result;
    CViewRectDataCore base;
    CViewRectDataCore inset;
    int i;
    int w;
    static const char* grid_names[] = {
        "grid_0", "grid_1", "grid_2", "grid_3", "grid_4", "grid_5", "grid_6", "grid_7",
    };
    static const char* frame_names[] = {
        "frame_0", "frame_1", "frame_2", "frame_3", "frame_4", "frame_5",
    };

    behaviour_result_init(result);
    base = make_base();

    run_scenario(result, "within_bounds", &base, 320, 240);
    run_scenario(result, "overflow_both_axes", &base, 800, 600);
    run_scenario(result, "negative_size", &base, -10, -20);
    run_scenario(result, "zero_size", &base, 0, 0);
    run_scenario(result, "width_only_overflow", &base, 900, 240);
    run_scenario(result, "height_only_overflow", &base, 320, 900);
    run_scenario(result, "negative_unk8", &with_neg_unk8(&base), 100, 100);
    run_scenario(result, "negative_unkA", &with_neg_unkA(&base), 100, 100);

    inset = base;
    inset.unkC = 16;
    inset.unkE = 32;
    inset.unk10 = 8;
    inset.unk12 = 16;
    run_scenario(result, "with_insets", &inset, 700, 500);
    run_scenario(result, "insets_small_input", &inset, 50, 40);
    run_scenario(result, "insets_overflow", &inset, 1000, 800);

    for (i = 0; i < 8; ++i) {
        CViewRectDataCore s = base;
        s.unk8 = (s16)(100 + i * 50);
        s.unkA = (s16)(80 + i * 40);
        s.unkC = (s16)(i * 4);
        s.unk10 = (s16)(i * 2);
        run_scenario(result, grid_names[i], &s, (s16)(200 + i * 30), (s16)(150 + i * 20));
    }

    for (w = 0; w < 6; ++w) {
        CViewRectDataCore s = base;
        s.unk4 = (s16)(320 + w * 64);
        s.unk6 = (s16)(240 + w * 48);
        run_scenario(result, frame_names[w], &s, (s16)(s.unk4 - 1), (s16)(s.unk6 - 1));
    }

    run_scenario(result, "max_s16_size", &base, 32767, 32767);
    run_scenario(result, "min_s16_size", &base, -32768, -32768);
    run_scenario(result, "large_margins", &with_large_margins(&base), 640, 480);
    run_scenario(result, "tight_width", &with_tight_width(&base), 400, 300);
    run_scenario(result, "tight_height", &with_tight_height(&base), 300, 350);
    run_scenario(result, "both_negative_outputs", &with_both_negative(&base), -5, -8);
    run_scenario(result, "symmetric_clamp", &base, 640, 480);

    result->exit_code = result->failed == 0 ? 0 : 1;
    behaviour_emit_summary("view-rect-data-clamp", result);
    return (int)result->exit_code;
}
