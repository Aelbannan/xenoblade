// Auto-scaffolded catalog TU for CriWare/src/sofdec/sud/sud_lib
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// LLM-HARNESS-BEGIN: us-803d96f0
void SUD_Init() {}
// LLM-HARNESS-END: us-803d96f0

// LLM-HARNESS-BEGIN: us-803d971c
int strncmp(const char* s1, const char* s2, size_t n);
extern const char lbl_eu_8051D47C[];
s32 SUD_AnalyTypeDivField(const char* buf, s32 len) {
    if (!buf || len < 0) return 0;
    return strncmp(buf + 0x12, lbl_eu_8051D47C + 0x15, 1) == 0 ? 1 : 0;
}
// LLM-HARNESS-END: us-803d971c

// LLM-HARNESS-BEGIN: us-803d9770
int strncmp(const char* s1, const char* s2, size_t n);
extern const char lbl_eu_8051D47C[];
s32 SUD_AnalyTypeCcs(const char* buf, s32 len) {
    if (!buf || len < 0) return 0;
    return strncmp(buf + 0x13, lbl_eu_8051D47C + 0x17, 1) == 0 ? 1 : 0;
}
// LLM-HARNESS-END: us-803d9770

// LLM-HARNESS-BEGIN: us-803d97c4
void SUD_SearchSudDat() {}
// LLM-HARNESS-END: us-803d97c4
