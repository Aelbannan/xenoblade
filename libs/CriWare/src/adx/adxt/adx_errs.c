// Decompiled ADX error-reporting module (ADXERR_*).
// Byte-identical to retail: ADXERR_Init, ADXERR_Finish, ADXERR_CallErrFunc1_,
// ADXERR_CallErrFunc2_, ADXERR_ItoA, ADXERR_ItoA2.

#include <harness_catalog.h>
#include <string.h>
#include "lbls_criware.hpp"

// Cross-TU imports (C linkage). No shared CriWare header declares these yet;
// sibling units (svm.c, adx_fs.c, adx_tlk2.c) use the same local-extern style.
extern void SVM_CallErr(const char* msg, ...);
extern char* CRICRW_Strncpy(char* dst, void* ignored, const char* src, size_t n);
extern char* CRICRW_Strncat(char* dst, void* ignored, const char* src, size_t n);

// ADX error work area, stored at lbl_eu_805E6380 (0x108 bytes of bss).
// Opaque context type for the user error callback (layout unknown; never
// dereferenced -- only stored and passed back).
typedef struct ADXERR_Context ADXERR_Context;

typedef struct ADXERR_Work {
    void (*callback)(ADXERR_Context* object, const char* msg); // error callback (NULL when unset)
    ADXERR_Context* object;                                    // opaque context passed to callback
    char buf[0x100];                                           // formatted error-message buffer
} ADXERR_Work;

// Overlay the retail bss block with the typed work-area view.
#define ADXERR_WORK ((ADXERR_Work*)&lbl_eu_805E6380)

// Reset the work area: clear the message buffer and unset the error callback.
void ADXERR_Init(void) {
    ADXERR_Work* w = ADXERR_WORK;
    memset(w->buf, 0, 0x100);
    w->callback = NULL;
    w->object = NULL;
}

// Mirror of ADXERR_Init (retail emits a separate, identical body).
void ADXERR_Finish(void) {
    ADXERR_Work* w = ADXERR_WORK;
    memset(w->buf, 0, 0x100);
    w->callback = NULL;
    w->object = NULL;
}

// Report one error message: copy it into the work-area buffer, pass it to the
// user error callback if one is installed, then hand it to SVM_CallErr.
// A NULL message reports the generic "Error" string (lbl_eu_80517498).
void ADXERR_CallErrFunc1_(const char* msg) {
    ADXERR_Work* w = ADXERR_WORK;
    if (!msg) {
        SVM_CallErr(lbl_eu_80517498);
        return;
    }

    CRICRW_Strncpy(w->buf, (void*)0x100, msg, 0xFF);

    if (w->callback) {
        w->callback(w->object, w->buf);
    }

    SVM_CallErr(w->buf);
}

// Report a two-part error message (a and b concatenated into the work buffer).
// Same dispatch as ADXERR_CallErrFunc1_; NULL in either part yields "Error".
void ADXERR_CallErrFunc2_(const char* a, const char* b) {
    ADXERR_Work* w = ADXERR_WORK;
    if (!a || !b) {
        SVM_CallErr(lbl_eu_80517498);
        return;
    }

    CRICRW_Strncpy(w->buf, (void*)0x100, a, 0xFF);
    CRICRW_Strncat(w->buf, (void*)0x100, b, 0xFF);

    if (w->callback) {
        w->callback(w->object, w->buf);
    }

    SVM_CallErr(w->buf);
}

// Format an integer into buf.
// Retail quirk (kept for byte-identity): the digit loop writes RAW digit
// values (0-9, not ASCII '0'+d) and the inline terminator overwrites the last
// digit; the result is then replaced by a reversal copy that re-reads the
// global scratch lbl_eu_805E6488 (bss, zero-initialized in retail) and writes
// its reversed prefix into buf. Faithful to the retail bytes -- do not "fix".
void ADXERR_ItoA(s32 value, char* buf, s32 buf_size) {
    s32 i;
    for (i = 0; i < 32; i++) {
        buf[i] = (char)(value % 10);
        value /= 10;
        if (value == 0) {
            buf[i] = '\0';
            break;
        }
    }

    {
        s32 len = (s32)strlen(lbl_eu_805E6488);
        s32 n = len < buf_size - 1 ? len : buf_size - 1;
        s32 j;
        for (j = 0; j < n; j++) {
            buf[j] = lbl_eu_805E6488[n - 1 - j];
        }
        buf[j] = '\0'; // j == n here; retail uses the loop counter
    }
}

// Format two integers into buf, separated by a space (lbl_eu_80517498 + 6 is
// the " " after "Error\0"). ADXERR_ItoA is applied to each value in turn.
void ADXERR_ItoA2(s32 a, s32 b, char* buf, s32 buf_size) {
    ADXERR_ItoA(a, buf, buf_size);

    CRICRW_Strncat(buf, (void*)buf_size, lbl_eu_80517498 + 6, buf_size - (s32)strlen(buf) - 1);

    ADXERR_ItoA(b, buf + (s32)strlen(buf), 4 - (s32)strlen(buf));
}
