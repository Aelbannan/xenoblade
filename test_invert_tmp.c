#include <harness_catalog.h>
extern u32 lbl_eu_80602A78[];
int CheckHn(void* h);

s32 f1(void* self, void* cb, void* arg) {
    u32* dst;
    if (self == 0) {
        dst = lbl_eu_80602A78;
    } else {
        if (CheckHn(self) != 0) {
            return 0xFF030203;
        }
        dst = (u32*)((u8*)self + 0xbdc);
    }
    dst[0] = (u32)cb;
    dst[1] = (u32)arg;
    return 0;
}
