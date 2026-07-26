// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfx/sfx_alp
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void* memset(void* s, int c, size_t n);
extern u8 lbl_eu_8061A138[0x128];
void SFXA_Init(void) {
    memset(lbl_eu_8061A138, 0, 0x128);
    *(u32*)(lbl_eu_8061A138 + 4) = 8;
}

void SFXA_Create() {}

void SFXA_Destroy(void* self) {
    if (self == NULL) return;
    *(u32*)self = 0;
}

void SFXA_MakeAlpLumiTbl(void* self, u32 a, u32 b, u32 c) {
    void (*cb)(u32, u32, u32, u32) = *(void (**)(u32, u32, u32, u32))((u8*)self + 0x18);
    if (cb) cb(*(u32*)((u8*)self + 8), *(u32*)((u8*)self + 0xc), *(u32*)((u8*)self + 0x10), c);
    *(u32*)((u8*)self + 4) = 0;
}

void SFXA_MakeAlp3110Tbl() {}

void SFXA_MakeAlp3211Tbl(void* obj, int a, int b)
{
    unsigned char* base = (unsigned char*)obj;
    void (*func)(int, int, int, int) = *(void (**)(int, int, int, int))&base[0x20];
    int byte14 = base[0x14];
    int byte15 = base[0x15];
    int byte16 = base[0x16];
    if (func != NULL)
    {
        func(b, byte14, byte15, byte16);
    }
}

u32 SFXA_IsNeedUpdateLumiTbl(void* self) { return *(u32*)((u8*)self + 0x4); }
