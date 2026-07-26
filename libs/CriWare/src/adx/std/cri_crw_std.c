// Auto-scaffolded catalog TU for CriWare/src/adx/std/cri_crw_std
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// LLM-HARNESS-BEGIN: us-8039e76c
extern char lbl_eu_80519750[];
extern void* lbl_eu_805FDDA0;
char* criCrw_GetVersion(void) {
    return lbl_eu_805FDDA0 = lbl_eu_80519750;
}
// LLM-HARNESS-END: us-8039e76c

// LLM-HARNESS-BEGIN: us-8039e780
void CRICRW_Strcpy(void) {}
// LLM-HARNESS-END: us-8039e780

// LLM-HARNESS-BEGIN: us-8039e788
void CRICRW_Strncpy(void) {}
// LLM-HARNESS-END: us-8039e788

// LLM-HARNESS-BEGIN: us-8039e794
// mr r4, r5; b strcat — copies 3rd arg to 2nd and calls strcat
char* strcat(char*, const char*);
char* CRICRW_Strcat(char* dst, const char* src1, const char* src2) {
    return strcat(dst, src2);
}
// LLM-HARNESS-END: us-8039e794

// LLM-HARNESS-BEGIN: us-8039e79c
char* strncat(char* dest, const char* src, size_t n);
char* CRICRW_Strncat(char* dest, void* ignored, const char* src, size_t n) {
    return strncat(dest, src, n);
}
// LLM-HARNESS-END: us-8039e79c

// LLM-HARNESS-BEGIN: us-8039e7a8
void CRICRW_Sprintf() {}
// LLM-HARNESS-END: us-8039e7a8

// LLM-HARNESS-BEGIN: us-8039e838
int vsprintf(char* s, const char* fmt, va_list ap);
int CRICRW_Vsprintf(char* s, void* ignored, const char* fmt, va_list ap) {
    return vsprintf(s, fmt, ap);
}
// LLM-HARNESS-END: us-8039e838

// LLM-HARNESS-BEGIN: us-8039e844
void criware_8039B4E0() {}
// LLM-HARNESS-END: us-8039e844
