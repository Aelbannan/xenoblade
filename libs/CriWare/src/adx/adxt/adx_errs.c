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
    if (!msg) {
        SVM_CallErr(lbl_eu_80517498);
        return;
    }

    CRICRW_Strncpy(s_work->buf, ((void*)0), msg, 0xFF);

    if (s_work->callback) {
        s_work->callback(s_work->arg, s_work->buf);
    }

    SVM_CallErr(s_work->buf);
}

void ADXERR_CallErrFunc2_(const char* a, const char* b) {
    if (!a || !b) {
        SVM_CallErr(lbl_eu_80517498);
        return;
    }

    CRICRW_Strncpy(s_work->buf, ((void*)0), a, 0xFF);
    CRICRW_Strncat(s_work->buf, ((void*)0), b, 0xFF);

    if (s_work->callback) {
        s_work->callback(s_work->arg, s_work->buf);
    }

    SVM_CallErr(s_work->buf);
}

void ADXERR_ItoA(s32 value, char* buf, s32 buf_size) {
    // Convert integer to ASCII digits (least-significant first) into a temp buffer
    char tmp[32];
    int i = 0;
    int negative = 0;
    u32 v;

    if (value < 0) {
        negative = 1;
        v = (u32)(-value);
    } else {
        v = (u32)value;
    }

    if (v == 0) {
        tmp[i++] = '0';
    } else {
        while (v > 0 && i < 31) {
            tmp[i++] = (char)('0' + (v % 10));
            v /= 10;
        }
    }

    if (negative && i < 31) {
        tmp[i++] = '-';
    }

    // Copy digits into output buffer in reverse order (most-significant first)
    s32 copy_len = i < buf_size - 1 ? i : buf_size - 1;
    for (s32 j = 0; j < copy_len; j++) {
        buf[j] = tmp[i - 1 - j];
    }
    if (copy_len >= 0) {
        buf[copy_len] = '\0';
    }
}

void ADXERR_ItoA2(s32 a, s32 b, char* buf, s32 buf_size) {
    s32 len;

    ADXERR_ItoA(a, buf, buf_size);

    len = (s32)strlen(buf);
    CRICRW_Strncat(buf, ((void*)0), lbl_eu_80517498 + 6, buf_size - len - 1);

    len = (s32)strlen(buf);
    ADXERR_ItoA(b, buf + len, 4 - len);
}
