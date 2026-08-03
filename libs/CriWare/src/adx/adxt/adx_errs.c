// Decompiled ADX error handling module.
// Matches retail ADXERR_Init, ADXERR_Finish, ADXERR_CallErrFunc1_,
// ADXERR_CallErrFunc2_, ADXERR_ItoA, ADXERR_ItoA2.

#include <harness_catalog.h>
#include <string.h>

extern void* memset(void* s, int c, size_t n);
extern size_t strlen(const char* s);
extern void SVM_CallErr(const char* msg, ...);
extern char* CRICRW_Strncpy(char* dst, void* ignored, const char* src, size_t n);
extern char* CRICRW_Strncat(char* dst, void* ignored, const char* src, size_t n);

extern char lbl_eu_80517498[];
extern char lbl_eu_805E6488[0x20];
extern u8 lbl_eu_805E6380[0x108];

// ADXERR work area at lbl_eu_805E6380 (size 0x108):
//   offset 0x00: void (*callback)(void* arg, char* msg)
//   offset 0x04: void* arg
//   offset 0x08: char buf[0x100]
struct ADXERR_Work {
    void (*callback)(void* arg, char* msg);
    void* arg;
    char buf[0x100];
};

#define s_work ((struct ADXERR_Work*)&lbl_eu_805E6380)

void ADXERR_Init(void) {
    struct ADXERR_Work* w = (struct ADXERR_Work*)&lbl_eu_805E6380;
    memset(w->buf, 0, 0x100);
    w->callback = ((void (*)(void*, char*))0);
    w->arg = ((void*)0);
}

void ADXERR_Finish(void) {
    struct ADXERR_Work* w = (struct ADXERR_Work*)&lbl_eu_805E6380;
    memset(w->buf, 0, 0x100);
    w->callback = ((void (*)(void*, char*))0);
    w->arg = ((void*)0);
}

void ADXERR_CallErrFunc1_(const char* msg) {
    struct ADXERR_Work* w = s_work;
    if (!msg) {
        SVM_CallErr(lbl_eu_80517498);
        return;
    }

    CRICRW_Strncpy(w->buf, (void*)0x100, msg, 0xFF);

    if (w->callback) {
        w->callback(w->arg, w->buf);
    }

    SVM_CallErr(w->buf);
}

void ADXERR_CallErrFunc2_(const char* a, const char* b) {
    struct ADXERR_Work* w = s_work;
    if (!a || !b) {
        SVM_CallErr(lbl_eu_80517498);
        return;
    }

    CRICRW_Strncpy(w->buf, (void*)0x100, a, 0xFF);
    CRICRW_Strncat(w->buf, (void*)0x100, b, 0xFF);

    if (w->callback) {
        w->callback(w->arg, w->buf);
    }

    SVM_CallErr(w->buf);
}

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
        buf[n] = '\0';
    }
}

void ADXERR_ItoA2(s32 a, s32 b, char* buf, s32 buf_size) {
    ADXERR_ItoA(a, buf, buf_size);

    CRICRW_Strncat(buf, (void*)buf_size, lbl_eu_80517498 + 6, buf_size - (s32)strlen(buf) - 1);

    ADXERR_ItoA(b, buf + (s32)strlen(buf), 4 - (s32)strlen(buf));
}
