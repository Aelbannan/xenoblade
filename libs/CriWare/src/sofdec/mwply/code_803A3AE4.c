// Auto-scaffolded catalog TU for CriWare/src/sofdec/mwply/code_803A3AE4
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>


extern char lbl_eu_8051BF28[];
extern void MWSFSVM_Error(const char* fmt, ...);
extern u32 criware_803A0BA8(void* self, u32 x);
extern s32 criware_803A0BDC(void* self);

void criware_803A3A48(void* self) {
    s32* entry;
    s32 i;
    for (i = 0; i < 32; i++) {
        entry = (s32*)((u8*)self + (31 - i) * 4 + 0x558);
        if (*entry != 0) {
            if (*(s32*)((u8*)self + 0x540) != 0)
                criware_803A0BA8(self, (u32)*entry);
            else
                criware_803A0BDC((void*)*entry);
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
