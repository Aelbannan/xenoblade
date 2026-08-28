#include <harness_catalog.h>
#include <string.h>

__declspec(section ".rodata") __attribute__((aligned(8))) const char lbl_eu_80519750[0x60] = {
    0x0A, 0x43, 0x52, 0x49, 0x20, 0x43, 0x52, 0x57, 0x3A, 0x53, 0x54, 0x44, 0x2F, 0x57, 0x49, 0x49, 0x20, 0x56, 0x65, 0x72, 0x2E, 0x30, 0x2E, 0x38, 0x33, 0x20, 0x42, 0x75, 0x69, 0x6C, 0x64, 0x3A, 0x4F, 0x63, 0x74, 0x20, 0x32, 0x38, 0x20, 0x32, 0x30, 0x30, 0x39, 0x20, 0x32, 0x31, 0x3A, 0x31, 0x31, 0x3A, 0x32, 0x35, 0x0A, 0x00, 0x41, 0x70, 0x70, 0x65, 0x6E, 0x64, 0x3A, 0x20, 0x4D, 0x57, 0x34, 0x33, 0x30, 0x32, 0x20, 0x57, 0x49, 0x49, 0x33, 0x30, 0x4A, 0x75, 0x6C, 0x32, 0x30, 0x30, 0x38, 0x50, 0x61, 0x74, 0x63, 0x68, 0x30, 0x34, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void *lbl_eu_805FDDA0[8];

char* criCrw_GetVersion(void) {
    return lbl_eu_805FDDA0[0] = (void*)lbl_eu_80519750;
}

char* strcpy(char*, const char*);
char* CRICRW_Strcpy(char* dst, const char* ignored, const char* src) {
    return strcpy(dst, src);
}

char* strncpy(char* dest, const char* src, size_t n);
char* CRICRW_Strncpy(char* dst, const char* ignored, const char* src, size_t n) {
    return strncpy(dst, src, n);
}

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

static s32 clamp_s16(s32 val) {
    if (val > 0x7FFF) {
        return 0x7FFF;
    }
    if (val < -0x8000) {
        return -0x8000;
    }
    return val;
}

void criware_8039B4E0(s16* src, s32 src_len, s16* dst, s32 dst_len) {
    s32 step;
    s32 overlap;
    s32 i;

    if (src_len < dst_len) {
        step = dst_len - src_len;
        overlap = src_len - step;

        for (i = 0; i < step; i++) {
            dst[i] = src[i];
        }

        for (i = 0; i < overlap; i++) {
            s32 b = (overlap - i) * src[step + i];
            s32 a = i * src[i];
            dst[step + i] = clamp_s16((s16)(b / overlap) + (s16)(a / overlap));
        }

        for (i = 0; i < step; i++) {
            dst[src_len + i] = src[overlap + i];
        }
    } else if (src_len == dst_len) {
        memcpy(dst, src, dst_len * sizeof(s16));
    } else if (src_len > dst_len) {
        step = src_len - dst_len;
        for (i = 0; i < dst_len; i++) {
            s32 b = src[i] * (dst_len - i);
            s32 a = i * src[step + i];
            dst[i] = clamp_s16((s16)(b / dst_len) + (s16)(a / dst_len));
        }
    }
}
