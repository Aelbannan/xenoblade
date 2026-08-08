// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_seeki
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void SFSEEKI_InitHn(void* self) {
    *(u32*)((u8*)self + 0x00) = 0;
    *(u32*)((u8*)self + 0x18) = 0;
    *(u32*)((u8*)self + 0x04) = 0;
    *(u32*)((u8*)self + 0x08) = 0;
    *(u32*)((u8*)self + 0x0C) = 0;
    *(u32*)((u8*)self + 0x10) = 0;
    *(u32*)((u8*)self + 0x14) = 0;
}

typedef struct SfseekiHn {
    u8 pad[0x960];
    u32 field_0x960;     /* 0x960 */
    u32 field_0x964;     /* 0x964 */
    u8 pad2[0x1D18];
    u32 skipCount;       /* 0x2680 */
    u8 pad3[0x4];
    u32 field_0x2688;    /* 0x2688 */
    u8 pad4[0xC];
    u32 field_0x2698;    /* 0x2698 */
} SfseekiHn;

/* Seek-callback argument object at lbl_eu_80568CF0. */
typedef struct SfseekiCbArg {
    u32 field_0x0;       /* 0x00 */
    u32 base;            /* 0x04 - passed as arg1 to the callback */
    u32 field_0x8;       /* 0x08 */
    SfseekiHn* hn;       /* 0x0C */
    u32 field_0x10;      /* 0x10 */
    u32 field_0x14;      /* 0x14 */
    char* str;           /* 0x18 - seek label to report */
} SfseekiCbArg;

extern void* lbl_eu_80606E34;
extern char lbl_eu_8051CEC0[];
extern SfseekiCbArg lbl_eu_80568CF0;

typedef void (*SfseekiCallback)(void*, void*);

/*
 * SFSEEKI_IsSkip
 * If seeking is pending, dispatch through the registered seek callback
 * (lbl_eu_80606E34) so the consumer can perform the seek.
 */
int SFSEEKI_IsSkip(SfseekiHn* hn, u32* outSkipCount) {
    SfseekiCallback fn;

    *outSkipCount = hn->skipCount;
    if (hn->skipCount == 0) {
        return 0;
    }

    if (hn->field_0x964 < hn->field_0x2698) {
        if (lbl_eu_80606E34 != NULL) {
            SfseekiCbArg* cb = &lbl_eu_80568CF0;
            cb->hn = hn;
            cb->str = &lbl_eu_8051CEC0[0];
            fn = *(SfseekiCallback*)((u8*)*(u32*)lbl_eu_80606E34 + 0x24);
            fn(lbl_eu_80606E34, &cb->base);
        }
        return 1;
    }

    if (hn->field_0x960 >= hn->field_0x2688) {
        if (lbl_eu_80606E34 != NULL) {
            SfseekiCbArg* cb = &lbl_eu_80568CF0;
            cb->hn = hn;
            cb->str = &lbl_eu_8051CEC0[0x0D];
            fn = *(SfseekiCallback*)((u8*)*(u32*)lbl_eu_80606E34 + 0x24);
            fn(lbl_eu_80606E34, &cb->base);
        }
        return 1;
    }

    return 0;
}