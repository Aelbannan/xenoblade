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
    u8 pad2[0x1D1C];
    u32 skipCount;       /* 0x2680 */
    u8 pad3[0x4];
    u32 field_0x2688;    /* 0x2688 */
    u8 pad4[0xC];
    u32 field_0x2698;    /* 0x2698 */
} SfseekiHn;

extern void* lbl_eu_80606E34;
extern char lbl_eu_8051CEC0[];
extern u8 lbl_eu_80568CF0[];

typedef void (*SfseekiCallback)(void*, void*);

int SFSEEKI_IsSkip(SfseekiHn* hn, u32* outSkipCount) {
    void* callback;
    u32* vtable;
    SfseekiCallback vfunc;

    *outSkipCount = hn->skipCount;
    if (hn->skipCount == 0) {
        return 0;
    }

    if (hn->field_0x964 < hn->field_0x2698) {
        callback = lbl_eu_80606E34;
        if (callback != NULL) {
            *(void**)(lbl_eu_80568CF0 + 0x0C) = callback;
            *(void**)(lbl_eu_80568CF0 + 0x04) = hn;
            *(char**)(lbl_eu_80568CF0 + 0x18) = lbl_eu_8051CEC0;
            vtable = *(u32**)callback;
            vfunc = (SfseekiCallback)vtable[9];
            vfunc(callback, lbl_eu_80568CF0 + 0x04);
        }
        return 1;
    }

    if (hn->field_0x960 >= hn->field_0x2688) {
        callback = lbl_eu_80606E34;
        if (callback != NULL) {
            *(void**)(lbl_eu_80568CF0 + 0x0C) = callback;
            *(void**)(lbl_eu_80568CF0 + 0x04) = hn;
            *(char**)(lbl_eu_80568CF0 + 0x18) = lbl_eu_8051CEC0 + 0x0D;
            vtable = *(u32**)callback;
            vfunc = (SfseekiCallback)vtable[9];
            vfunc(callback, lbl_eu_80568CF0 + 0x04);
        }
        return 1;
    }

    return 0;
}
