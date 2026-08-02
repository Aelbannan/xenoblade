#ifndef RVL_SDK_NDEV_EXI2_AD_ODEMU_H
#define RVL_SDK_NDEV_EXI2_AD_ODEMU_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

// Mail descriptor layout: bits 31-24 = 0x1F tag, bits 23-16 = page,
// bits 12-0 = payload size in bytes.
static u32 ODEMUGenMailData(u32 ofs, u32 size) {
    return (ofs & 0xff) << 0x10 | 0x1f000000 | size & 0x1fff;
}

static u32 ODEMUGetPage(u32 mail) {
    return (mail & 0xFF0000) >> 16;
}

// Page 1 maps 0x800 bytes past the page-0 buffer (page stride).
static u32 ODEMUGetPc2NngcOffset(u32 mail) {
    if (!(ODEMUGetPage(mail) & 0x1)) {
        return 0;
    }

    return 0x800;
}

// A mail is valid iff the 0x1F tag is present.
static BOOL ODEMUIsValidMail(u32 mail) {
    return (mail & 0x1F000000) == 0x1F000000;
}

static u32 ODEMUGetSize(u32 mail) {
    return mail & 0x1FFF;
}

#ifdef __cplusplus
}
#endif
#endif
