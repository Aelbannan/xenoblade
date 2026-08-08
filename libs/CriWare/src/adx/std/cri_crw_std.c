// Auto-scaffolded catalog TU for CriWare/src/adx/std/cri_crw_std
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern char lbl_eu_80519750[];
extern void* lbl_eu_805FDDA0;
char* criCrw_GetVersion(void) {
    return lbl_eu_805FDDA0 = lbl_eu_80519750;
}

char* strcpy(char*, const char*);
char* CRICRW_Strcpy(char* dst, const char* ignored, const char* src) {
    return strcpy(dst, src);
}

char* strncpy(char* dest, const char* src, size_t n);
char* CRICRW_Strncpy(char* dst, const char* ignored, const char* src, size_t n) {
    return strncpy(dst, src, n);
}

// mr r4, r5; b strcat - copies 3rd arg to 2nd and calls strcat
char* strcat(char*, const char*);
char* CRICRW_Strcat(char* dst, const char* src1, const char* src2) {
    return strcat(dst, src2);
}

char* strncat(char* dest, const char* src, size_t n);
char* CRICRW_Strncat(char* dest, void* ignored, const char* src, size_t n) {
    return strncat(dest, src, n);
}

int CRICRW_Sprintf(char* s, void* ignored, const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    return vsprintf(s, fmt, ap);
}

int vsprintf(char* s, const char* fmt, va_list ap);
int CRICRW_Vsprintf(char* s, void* ignored, const char* fmt, va_list ap) {
    return vsprintf(s, fmt, ap);
}

static s16 clamp_s16(s32 val) {
    if (val > 0x7FFF) {
        return 0x7FFF;
    }
    if (val < -0x8000) {
        return -0x8000;
    }
    return (s16)val;
}

/* Time-stretch/resample via overlap-add. */
void criware_8039B4E0(s16* src, s32 src_len, s16* dst, s32 dst_len) {
    s32 step;
    s32 overlap;
    s32 i;
    s32 w;

    if (src_len >= dst_len) {
        if (src_len == dst_len) {
            memcpy(dst, src, (size_t)dst_len * sizeof(s16));
        } else {
            /* src_len > dst_len: crossfade decimation */
            step = src_len - dst_len;
            s16* sp = src + step;
            for (i = 0; i < dst_len; i++) {
                s32 a = i * sp[i];
                s32 b = (dst_len - i) * src[i];
                w = a / dst_len + b / dst_len;
                dst[i] = clamp_s16(w);
            }
        }
        return;
    }

    /* src_len < dst_len: overlap-add stretch */
    step = dst_len - src_len;
    overlap = src_len - step;

    /* Phase 1: copy first 'step' samples */
    for (i = 0; i < step; i++) {
        dst[i] = src[i];
    }

    /* Phase 2: crossfade overlap region */
    for (i = 0; i < overlap; i++) {
        s32 a = i * src[i];
        s32 b = (overlap - i) * src[step + i];
        w = a / overlap + b / overlap;
        dst[step + i] = clamp_s16(w);
    }

    /* Phase 3: copy remaining samples */
    for (i = 0; i < step; i++) {
        dst[src_len + i] = src[overlap + i];
    }
}
