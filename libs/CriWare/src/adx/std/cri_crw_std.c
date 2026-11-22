#include <harness_catalog.h>
#include <string.h>

/* Real CriWare rodata object (JP map: cricrwstd_build, global linkage):
 * version stamp + append line in one string-table object (same shape as
 * cft_ver_str / DCT_version_str); retail 0x60 slot = 0x5A + link tail pad. */
const char cricrwstd_build[] =
    "\nCRI CRW:STD/WII Ver.0.83 Build:Oct 28 2009 21:11:25\n\0"
    "Append: MW4302 WII30Jul2008Patch04\n";

// name-guess: no retail name in config/{us,eu,jp}/symbols.txt (JP: local
// lbl_80601360, size 0x20); slot 0 receives the version string.
static void* sCriCrwVersionSlots[8];

char* criCrw_GetVersion(void) {
    return sCriCrwVersionSlots[0] = (void*)cricrwstd_build;
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

static inline s32 clamp_s16(s32 val) {
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
