// Auto-scaffolded catalog TU for CriWare/src/sofdec/sud/sud_lib
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern s32 lbl_eu_8061A4D0;
extern void* lbl_eu_8061A4D4;
extern const char lbl_eu_8051D448[];

void SUD_Init(void) {
    s32 *pc = &lbl_eu_8061A4D0;
    s32 count = *pc;
    if (count >= 1) return;
    lbl_eu_8061A4D4 = (void*)lbl_eu_8051D448;
    *pc = count + 1;
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

s32 SUD_SearchSudDat(void* data, void* id, s32* out1, s32* out2) {
    const u8* p = (const u8*)lbl_eu_8051D47C;
    s32 i;
    *out1 = 0;
    *out2 = 0;
    if (data == NULL)
        return 0;
    if ((s32)id > 0)
        return 0;
    for (i = 0; i < (s32)data; i++) {
        if (memcmp(p, p + 0x1f, 1) == 0 && memcmp(p, id, 8) == 0) {
            *out1 = i;
            *out2 = (s32)(i + 1) & 1;
        }
        p++;
    }
    return 0;
}
