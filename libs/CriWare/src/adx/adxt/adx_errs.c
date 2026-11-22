// Decompiled ADX error-reporting module (ADXERR_*).
// Byte-identical to retail: ADXERR_Init, ADXERR_Finish, ADXERR_CallErrFunc1_,
// ADXERR_CallErrFunc2_, ADXERR_ItoA, ADXERR_ItoA2.

#include <harness_catalog.h>
#include <string.h>

extern const char adxerr_default_message[];

// Cross-TU imports (C linkage). No shared CriWare header declares these yet;
// sibling units (svm.c, adx_fs.c, adx_tlk2.c) use the same local-extern style.
extern void SVM_CallErr(const char* msg, ...);
extern char* CRICRW_Strncpy(char* dst, void* ignored, const char* src, size_t n);
extern char* CRICRW_Strncat(char* dst, void* ignored, const char* src, size_t n);

// Opaque context type for the user error callback (layout unknown; never
// dereferenced -- only stored and passed back).
typedef struct ADXERR_Context ADXERR_Context;

// Real CriWare work-area objects. Explicit zero initializers keep them in BSS.
void (*adxerr_func)(ADXERR_Context* object, const char* msg) = NULL;
ADXERR_Context* adxerr_obj = NULL;
char adxerr_msg[0x100] = { 0 };

// Reset the work area: clear the message buffer and unset the error callback.
void ADXERR_Init(void) {
    memset(adxerr_msg, 0, 0x100);
    adxerr_func = NULL;
    adxerr_obj = NULL;
}

// Mirror of ADXERR_Init (retail emits a separate, identical body).
void ADXERR_Finish(void) {
    memset(adxerr_msg, 0, 0x100);
    adxerr_func = NULL;
    adxerr_obj = NULL;
}

// Report one error message: copy it into the work-area buffer, pass it to the
// user error callback if one is installed, then hand it to SVM_CallErr.
// A NULL message reports the generic "Error" string (adxerr_default_message).
void ADXERR_CallErrFunc1_(const char* msg) {
    if (!msg) {
        SVM_CallErr(adxerr_default_message);
        return;
    }

    CRICRW_Strncpy(adxerr_msg, (void*)0x100, msg, 0xFF);

    if (adxerr_func) {
        adxerr_func(adxerr_obj, adxerr_msg);
    }

    SVM_CallErr(adxerr_msg);
}

// Report a two-part error message (a and b concatenated into the work buffer).
// Same dispatch as ADXERR_CallErrFunc1_; NULL in either part yields "Error".
void ADXERR_CallErrFunc2_(const char* a, const char* b) {
    if (!a || !b) {
        SVM_CallErr(adxerr_default_message);
        return;
    }

    CRICRW_Strncpy(adxerr_msg, (void*)0x100, a, 0xFF);
    CRICRW_Strncat(adxerr_msg, (void*)0x100, b, 0xFF);

    if (adxerr_func) {
        adxerr_func(adxerr_obj, adxerr_msg);
    }

    SVM_CallErr(adxerr_msg);
}

// Format an integer into buf.
// Retail quirk (kept for byte-identity): the digit loop writes RAW digit
// values (0-9, not ASCII '0'+d) and the inline terminator overwrites the last
// digit; the result is then replaced by a reversal copy that re-reads the
// static scratch buf (bss, zero-initialized in retail) and writes its reversed
// prefix into dst. Faithful to the retail bytes -- do not "fix".
void ADXERR_ItoA(s32 value, char* dst, s32 buf_size) {
    static char buf[0x20];
    s32 i;
    for (i = 0; i < 32; i++) {
        dst[i] = (char)(value % 10);
        value /= 10;
        if (value == 0) {
            dst[i] = '\0';
            break;
        }
    }

    {
        s32 len = (s32)strlen(buf);
        s32 n = len < buf_size - 1 ? len : buf_size - 1;
        s32 j;
        for (j = 0; j < n; j++) {
            dst[j] = buf[n - 1 - j];
        }
        dst[j] = '\0'; // j == n here; retail uses the loop counter
    }
}

// Format two integers into buf, separated by a space (adxerr_default_message + 6 is
// the " " after "Error\0"). ADXERR_ItoA is applied to each value in turn.
void ADXERR_ItoA2(s32 a, s32 b, char* buf, s32 buf_size) {
    ADXERR_ItoA(a, buf, buf_size);

    CRICRW_Strncat(buf, (void*)buf_size, adxerr_default_message + 6, buf_size - (s32)strlen(buf) - 1);

    ADXERR_ItoA(b, buf + (s32)strlen(buf), 4 - (s32)strlen(buf));
}

/* Generic ADXERR message ("Error", plus the " " separator). */
const char adxerr_default_message[8] = {
    0x45,0x72,0x72,0x6F,0x72,0x00,0x20,0x00
};
