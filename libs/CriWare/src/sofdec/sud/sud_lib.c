// Auto-scaffolded catalog TU for CriWare/src/sofdec/sud/sud_lib
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern s32 lbl_eu_8061A4D0;
extern void* lbl_eu_8061A4D4;
extern const char lbl_eu_8051D448[];

void SUD_Init(void) {
    s32 count = lbl_eu_8061A4D0;
    if (count >= 1)
        return;
    lbl_eu_8061A4D4 = (void*)lbl_eu_8051D448;
    lbl_eu_8061A4D0 = count + 1;
}

int strncmp(const char* s1, const char* s2, size_t n);
extern const char lbl_eu_8051D47C[];
s32 SUD_AnalyTypeDivField(const char* buf, s32 len) {
    if (!buf || len < 0) return 0;
    return strncmp(buf + 0x12, lbl_eu_8051D47C + 0x15, 1) == 0 ? 1 : 0;
}

int strncmp(const char* s1, const char* s2, size_t n);
extern const char lbl_eu_8051D47C[];
s32 SUD_AnalyTypeCcs(const char* buf, s32 len) {
    if (!buf || len < 0) return 0;
    return strncmp(buf + 0x13, lbl_eu_8051D47C + 0x17, 1) == 0 ? 1 : 0;
}

int memcmp(const void* s1, const void* s2, size_t n);
void SUD_SearchSudDat(char* data, s32 id, s32* out1, s32* out2) {
    s32 i;
    char* p = data;
    *out1 = 0;
    *out2 = 0;
    if (data == NULL)
        return;
    i = 0;
    if (id > 0)
        goto L;
    return;
L:
    while (i < id) {
        if (memcmp(p, lbl_eu_8051D47C + 0x1f, 1) == 0 &&
            memcmp(p, lbl_eu_8051D47C, 8) == 0) {
            *out1 = (s32)p;
            *out2 = ((__cntlzw((u32)p) >> 5) & 1) ? 0 : 35;
        }
        p++;
        i++;
    }
}
