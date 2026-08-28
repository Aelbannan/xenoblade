#include <harness_catalog.h>

__declspec(section ".rodata") __attribute__((aligned(8))) const char lbl_eu_8051BF28[0x28] = {
    0x45, 0x32, 0x30, 0x35, 0x33, 0x30, 0x30, 0x31, 0x20, 0x4D, 0x57, 0x53, 0x46, 0x44, 0x5F, 0x4D, 0x61, 0x6C, 0x6C, 0x6F, 0x63, 0x3A, 0x20, 0x63, 0x6E, 0x74, 0x20, 0x6F, 0x76, 0x65, 0x72, 0x2E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

extern char lbl_eu_8051C8B0[];
extern char lbl_eu_8051C8F0[];
extern char lbl_eu_8051C940[];
extern char lbl_eu_8051C4E0[];
extern char lbl_eu_8051CCD8[];
extern char lbl_eu_8051C550[];
extern char lbl_eu_8051CC98[];

__declspec(section ".rodata") __attribute__((aligned(4))) const void *lbl_eu_8051BF50[9] = {
    lbl_eu_8051C8B0,
    lbl_eu_8051C8F0,
    lbl_eu_8051C940,
    lbl_eu_8051C4E0,
    0,0,
    lbl_eu_8051CCD8,
    lbl_eu_8051C550,
    lbl_eu_8051CC98
};

__declspec(section ".rodata") __attribute__((aligned(4))) const void *lbl_eu_8051BF74[9] = {
    lbl_eu_8051C8B0,
    0,
    lbl_eu_8051C940,
    0,0,0,
    lbl_eu_8051CCD8,
    0,0
};

__declspec(section ".rodata") __attribute__((aligned(4))) const void *lbl_eu_8051BF98[10] = {
    lbl_eu_8051C8B0,
    lbl_eu_8051C8F0,
    lbl_eu_8051C940,
    0,0,0,
    lbl_eu_8051CCD8,
    0,
    lbl_eu_8051CC98,
    0
};

extern void MWSFSVM_Error(const char* fmt, ...);
extern u32 criware_803A0BA8(void* self, u32 x);
extern s32 criware_803A0BDC(void* self);
extern void criware_803A0C1C(void* self, u32 x);
extern void criware_803A0C20(void* self);

void criware_803A3A48(void* self) {
    s32* entry;
    s32 i;
    for (i = 0; i < 32; i++) {
        entry = (s32*)((u8*)self + (31 - i) * 4 + 0x558);
        if (*entry != 0) {
            if (*(s32*)((u8*)self + 0x540) != 0)
                criware_803A0C1C(self, (u32)*entry);
            else
                criware_803A0C20((void*)*entry);
            (*(s32*)((u8*)self + 0x554))--;
            *entry = 0;
        }
    }
}

void criware_803A3A48(void* self);

s32 criware_803A3AE4(void* self, s32 x) {
    if (x < 0) return 0;
    if (*(s32*)((u8*)self + 0x554) >= 32) {
        MWSFSVM_Error(lbl_eu_8051BF28);
        return 0;
    }
    {
        s32 r;
        if (*(s32*)((u8*)self + 0x540) != 0) {
            r = (s32)criware_803A0BA8(self, (u32)x);
        } else {
            r = (s32)criware_803A0BDC((void*)x);
        }
        if (r == 0) {
            MWSFSVM_Error(lbl_eu_8051BF28);
            return 0;
        }
        *(s32*)((u8*)self + *(s32*)((u8*)self + 0x554) * 4 + 0x558) = r;
        (*(s32*)((u8*)self + 0x554))++;
        return r;
    }
}

void criware_803A3B98(void* self) { *(u32*)((u8*)self + 0x554) = 0; }

u32 fn_803A7320(void* self) { return *(u32*)((u8*)self + 0x554); }

int fn_803A7328(void) { return 0x0; }
