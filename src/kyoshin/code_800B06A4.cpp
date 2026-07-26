// Auto-scaffolded catalog TU for kyoshin/code_800B06A4
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

// Forward declarations for functions called from extern "C" wrappers
void* func_800B07E8();
extern "C" u32 func_800AA2BC(u32 a, u32 b);

// LLM-HARNESS-BEGIN: us-800b0f70
extern "C" void func_800B06A4(float a) {
    extern float lbl_eu_80661CCC;
    extern float lbl_eu_80661CD0;
    extern float lbl_eu_80663EC8;
    extern float lbl_eu_80663ECC;
    extern float lbl_eu_80663ED0;
    extern float lbl_eu_80663ED4;

    double ad = a;
    float sq = (float)ad;
    float c = lbl_eu_80661CD0;
    lbl_eu_80661CCC = a;
    sq = sq * sq;
    lbl_eu_80663EC8 = a;
    lbl_eu_80663ECC = sq;
    lbl_eu_80663ED0 = c;
    lbl_eu_80663ED4 = sq;
}
// LLM-HARNESS-END: us-800b0f70






// LLM-HARNESS-BEGIN: us-800b135c
extern "C" void func_800B0A90(void) {}
// LLM-HARNESS-END: us-800b135c




// LLM-HARNESS-BEGIN: us-800b140c
extern "C" void func_800B0B40(void* self) {
    *(unsigned long*)((char*)self + 0xb00) = 0;
    *(unsigned long*)((char*)self + 0xafc) = 0;
}
// LLM-HARNESS-END: us-800b140c

// LLM-HARNESS-BEGIN: us-800b141c
extern "C" void __ct__reslist_cf_TboxInfo() {}
// LLM-HARNESS-END: us-800b141c


// LLM-HARNESS-BEGIN: us-800b15d8
extern "C" void __ct__reslist_cf_IFactoryEvent() {}
// LLM-HARNESS-END: us-800b15d8

// LLM-HARNESS-BEGIN: us-800b1780
extern "C" void __dt__17UnkClass_805764CCFv() {}
// LLM-HARNESS-END: us-800b1780

// LLM-HARNESS-BEGIN: us-800b186c
extern "C" void func_800B0FA0() {}
// LLM-HARNESS-END: us-800b186c

// LLM-HARNESS-BEGIN: us-800b18b8
extern "C" u32 func_800B0FEC(void* self) { return *(u32*)((u8*)self + 0x18); }
// LLM-HARNESS-END: us-800b18b8

// LLM-HARNESS-BEGIN: us-800b18c0
extern "C" void func_800B0FF4() {}
// LLM-HARNESS-END: us-800b18c0

// LLM-HARNESS-BEGIN: us-800b19ec
extern "C" void func_800B1120() {}
// LLM-HARNESS-END: us-800b19ec

// LLM-HARNESS-BEGIN: us-800b1bf8
extern "C" void __ct__cf_CfValueItemManager() {}
// LLM-HARNESS-END: us-800b1bf8

// LLM-HARNESS-BEGIN: us-800b1c34
extern "C" void func_800B1368(unsigned long flags) {
    extern unsigned long lbl_eu_80663EE0;
    lbl_eu_80663EE0 |= flags;
}
// LLM-HARNESS-END: us-800b1c34

// LLM-HARNESS-BEGIN: us-800b1c44
extern "C" void FactoryEvent3__Q22cf13IFactoryEventFv() {}
// LLM-HARNESS-END: us-800b1c44

// LLM-HARNESS-BEGIN: us-800b1c48
extern "C" void func_800B137C() {}
// LLM-HARNESS-END: us-800b1c48

// LLM-HARNESS-BEGIN: us-800b1d90
extern "C" void func_800B14C4(void* dst, void* src) {
    void* ptr = *(void**)((char*)src + 4);
    *(unsigned long*)dst = *(unsigned long*)ptr;
}
// LLM-HARNESS-END: us-800b1d90

// LLM-HARNESS-BEGIN: us-800b1da0
extern "C" void* func_800B14D4(void* self) {
    return (char*)(*(void**)self) + 8;
}
// LLM-HARNESS-END: us-800b1da0

// LLM-HARNESS-BEGIN: us-800b1dac
extern "C" void func_800B14E0() {}
// LLM-HARNESS-END: us-800b1dac

// LLM-HARNESS-BEGIN: us-800b1dbc
extern "C" void func_800B14F0(void* dst, void* src) {
    *(unsigned long*)dst = *(unsigned long*)((char*)src + 4);
}
// LLM-HARNESS-END: us-800b1dbc

// LLM-HARNESS-BEGIN: us-800b1dc8
extern "C" void func_800B14FC() {}
// LLM-HARNESS-END: us-800b1dc8

// LLM-HARNESS-BEGIN: us-800b1de4
extern "C" void func_800B1518(void) {}
// LLM-HARNESS-END: us-800b1de4

// LLM-HARNESS-BEGIN: us-800b1de8
extern "C" void __dt__800B151C() {}
// LLM-HARNESS-END: us-800b1de8

// LLM-HARNESS-BEGIN: us-800b1e70
extern "C" void func_800B15A4() {}
// LLM-HARNESS-END: us-800b1e70

extern u32 lbl_eu_80663F24;
// LLM-HARNESS-BEGIN: us-800b20d4
extern "C" void func_800B1808() { lbl_eu_80663F24 = 0; }
// LLM-HARNESS-END: us-800b20d4

// LLM-HARNESS-BEGIN: us-800b20e0
extern "C" void FactoryEvent4__Q22cf13IFactoryEventFv() {}
// LLM-HARNESS-END: us-800b20e0

// LLM-HARNESS-BEGIN: us-800b20e4
extern "C" void func_800B1818(void* self, u32 val) { *(u32*)((u8*)self + 0x0) = val; }
// LLM-HARNESS-END: us-800b20e4

// LLM-HARNESS-BEGIN: us-800b20ec
extern "C" void func_800B1820(int* dst, int* src) {
    *dst = *src;
}
// LLM-HARNESS-END: us-800b20ec

// LLM-HARNESS-BEGIN: us-800b20f8
extern "C" void func_800B182C() {}
// LLM-HARNESS-END: us-800b20f8

// LLM-HARNESS-BEGIN: us-800b2108
extern "C" void __dt__800B183C() {}
// LLM-HARNESS-END: us-800b2108

// LLM-HARNESS-BEGIN: us-800b2198
extern "C" void __dt__800B18CC() {}
// LLM-HARNESS-END: us-800b2198

// LLM-HARNESS-BEGIN: us-800b2220
extern "C" void func_800B1954() {}
// LLM-HARNESS-END: us-800b2220

// LLM-HARNESS-BEGIN: us-800b2328
extern "C" void func_800B1A5C() {}
// LLM-HARNESS-END: us-800b2328

// LLM-HARNESS-BEGIN: us-800b2358
extern "C" void func_800B1A8C() {}
// LLM-HARNESS-END: us-800b2358

// LLM-HARNESS-BEGIN: us-800b2368
extern "C" void* func_800B1A9C(void* self) {
    return (char*)(*(void**)self) + 8;
}
// LLM-HARNESS-END: us-800b2368

// LLM-HARNESS-BEGIN: us-800b2374
extern "C" void func_800B1AA8() {}
// LLM-HARNESS-END: us-800b2374

// LLM-HARNESS-BEGIN: us-800b238c
extern "C" void func_800B1AC0(int* dst, int* src) {
    *dst = *src;
}
// LLM-HARNESS-END: us-800b238c

// LLM-HARNESS-BEGIN: us-800b2398
extern "C" void func_800B1ACC(void* dst, void* src) {
    *(unsigned long*)dst = *(unsigned long*)((char*)src + 4);
}
// LLM-HARNESS-END: us-800b2398

// LLM-HARNESS-BEGIN: us-800b23a4
extern "C" void func_800B1AD8() {}
// LLM-HARNESS-END: us-800b23a4

// LLM-HARNESS-BEGIN: us-800b23c0
extern "C" void func_800B1AF4() {}
// LLM-HARNESS-END: us-800b23c0

// LLM-HARNESS-BEGIN: us-800b23f8
extern "C" void func_800B1B2C() {}
// LLM-HARNESS-END: us-800b23f8

// LLM-HARNESS-BEGIN: us-800b2488
extern "C" void func_800B1BBC() {}
// LLM-HARNESS-END: us-800b2488

// LLM-HARNESS-BEGIN: us-800b24cc
extern "C" unsigned long func_800B1C00() {
    extern unsigned long lbl_eu_80663EE0;
    return (lbl_eu_80663EE0 >> 25) & 1;
}
// LLM-HARNESS-END: us-800b24cc

// LLM-HARNESS-BEGIN: us-800b24d8
extern "C" void func_800B1C0C() {}
// LLM-HARNESS-END: us-800b24d8

// LLM-HARNESS-BEGIN: us-800b24f0
extern "C" void func_800B1C24() {}
// LLM-HARNESS-END: us-800b24f0

// LLM-HARNESS-BEGIN: us-800b250c
extern "C" void func_800B1C40() {}
// LLM-HARNESS-END: us-800b250c

// LLM-HARNESS-BEGIN: us-800b2544
extern "C" void func_800B1C78() {}
// LLM-HARNESS-END: us-800b2544

// LLM-HARNESS-BEGIN: us-800b25a8
extern "C" void func_800B1CDC() {}
// LLM-HARNESS-END: us-800b25a8

// LLM-HARNESS-BEGIN: us-800b26e4
extern "C" void func_800B1E18() {}
// LLM-HARNESS-END: us-800b26e4

// LLM-HARNESS-BEGIN: us-800b26f8
extern "C" void func_800B1E2C() {}
// LLM-HARNESS-END: us-800b26f8

// LLM-HARNESS-BEGIN: us-800b2784
extern "C" void func_800B1EB8() {}
// LLM-HARNESS-END: us-800b2784

// LLM-HARNESS-BEGIN: us-800b2794
extern "C" void func_800B1EC8() {}
// LLM-HARNESS-END: us-800b2794

// LLM-HARNESS-BEGIN: us-800b27f8
extern "C" void func_800B1F2C() {}
// LLM-HARNESS-END: us-800b27f8

// LLM-HARNESS-BEGIN: us-800b280c
extern "C" void func_800B1F40() {}
// LLM-HARNESS-END: us-800b280c

// LLM-HARNESS-BEGIN: us-800b2820
extern "C" void func_800B1F54() {}
// LLM-HARNESS-END: us-800b2820

// LLM-HARNESS-BEGIN: us-800b2838
extern "C" void func_800B1F6C() {}
// LLM-HARNESS-END: us-800b2838

// LLM-HARNESS-BEGIN: us-800b28a4
extern "C" void func_800B1FD8() {}
// LLM-HARNESS-END: us-800b28a4

// LLM-HARNESS-BEGIN: us-800b2900
extern "C" void func_800B2034() {}
// LLM-HARNESS-END: us-800b2900

// LLM-HARNESS-BEGIN: us-800b2914
extern "C" void func_800B2048() {}
// LLM-HARNESS-END: us-800b2914

// LLM-HARNESS-BEGIN: us-800b2928
extern "C" void func_800B205C() {}
// LLM-HARNESS-END: us-800b2928

// LLM-HARNESS-BEGIN: us-800b296c
extern "C" void func_800B20A0() {}
// LLM-HARNESS-END: us-800b296c

// LLM-HARNESS-BEGIN: us-800b2980
extern "C" void func_800B20B4() {}
// LLM-HARNESS-END: us-800b2980

// LLM-HARNESS-BEGIN: us-800b35f4
extern "C" u32 func_800B2D28(void* self) { return *(u32*)((u8*)self + 0x4); }
// LLM-HARNESS-END: us-800b35f4

// LLM-HARNESS-BEGIN: us-800b35fc
extern "C" void __dt__800B2D30() {}
// LLM-HARNESS-END: us-800b35fc

// LLM-HARNESS-BEGIN: us-800b3650
extern "C" void FactoryEvent1__Q22cf13IFactoryEventFv() {}
// LLM-HARNESS-END: us-800b3650

// LLM-HARNESS-BEGIN: us-800b3654
extern "C" void func_800B2D88(void* self, int param) {
    extern void func_800B2DB0(void*, void*);
    func_800B2DB0(self, &param);
}
// LLM-HARNESS-END: us-800b3654

// LLM-HARNESS-BEGIN: us-800b367c
extern "C" void func_800B2DB0() {}
// LLM-HARNESS-END: us-800b367c

// LLM-HARNESS-BEGIN: us-800b3704
extern "C" void func_800B2E38() {}
// LLM-HARNESS-END: us-800b3704

// LLM-HARNESS-BEGIN: us-800b379c
extern "C" void func_800B2ED0() {}
// LLM-HARNESS-END: us-800b379c

// LLM-HARNESS-BEGIN: us-800b3a7c
extern "C" int func_800B31B0(void* p) {
    return (*(int*)((char*)p + 0x64) >> 1) & 1;
}
// LLM-HARNESS-END: us-800b3a7c

// LLM-HARNESS-BEGIN: us-800b3a88
extern "C" int func_800B31BC(void* p) {
    return (*(int*)((char*)p + 0x64) >> 1) & 1;
}
// LLM-HARNESS-END: us-800b3a88

// LLM-HARNESS-BEGIN: us-800b3a94
extern "C" int func_800B31C8(void* p) {
    return (*(int*)((char*)p + 0x64) >> 1) & 1;
}
// LLM-HARNESS-END: us-800b3a94

// LLM-HARNESS-BEGIN: us-800b3aa0
extern "C" int func_800B31D4(void* p) {
    return (*(int*)((char*)p + 0x64) >> 1) & 1;
}
// LLM-HARNESS-END: us-800b3aa0

// LLM-HARNESS-BEGIN: us-800b3aac
extern "C" int func_800B31E0(void* p) {
    return (*(int*)((char*)p + 0x64) >> 1) & 1;
}
// LLM-HARNESS-END: us-800b3aac

// LLM-HARNESS-BEGIN: us-800b3ab8
extern "C" int func_800B31EC(void* p) {
    return (*(int*)((char*)p + 0x64) >> 1) & 1;
}
// LLM-HARNESS-END: us-800b3ab8

// LLM-HARNESS-BEGIN: us-800b3ac4
extern "C" int func_800B31F8(void* p) {
    return (*(int*)((char*)p + 0x64) >> 1) & 1;
}
// LLM-HARNESS-END: us-800b3ac4

// LLM-HARNESS-BEGIN: us-800b3ad0
extern "C" u32 func_800B3204(u32 val) { return (val >> 16) & 0xFF; }
// LLM-HARNESS-END: us-800b3ad0

// LLM-HARNESS-BEGIN: us-800b3adc
extern "C" void func_800B3210() {}
// LLM-HARNESS-END: us-800b3adc

// LLM-HARNESS-BEGIN: us-800b428c
extern "C" u32 func_800B39C0(void* self) { return *(u32*)((u8*)self + 0x74); }
// LLM-HARNESS-END: us-800b428c

// LLM-HARNESS-BEGIN: us-800b4294
extern "C" void func_800B39C8() {}
// LLM-HARNESS-END: us-800b4294

// LLM-HARNESS-BEGIN: us-800b4320
extern "C" void func_800B3A54(int* dst, int* src) {
    *dst = *src;
}
// LLM-HARNESS-END: us-800b4320

// LLM-HARNESS-BEGIN: us-800b432c
extern "C" void func_800B3A60(void* self, int param) {
    extern void func_800B99EC(void*, void*);
    func_800B99EC(self, &param);
}
// LLM-HARNESS-END: us-800b432c

// LLM-HARNESS-BEGIN: us-800b4354
extern "C" void func_800B3A88() {}
// LLM-HARNESS-END: us-800b4354

// LLM-HARNESS-BEGIN: us-800b4630
extern "C" u32 func_800B3D34(u32 val) { return val >> 24; }
// LLM-HARNESS-END: us-800b4630

// LLM-HARNESS-BEGIN: us-800b463c
extern "C" u32 func_800B3D40(u32 val) { return (val >> 16) & 0xFF; }
// LLM-HARNESS-END: us-800b463c

// LLM-HARNESS-BEGIN: us-800b4648
extern "C" void func_800B3D4C() {}
// LLM-HARNESS-END: us-800b4648

// LLM-HARNESS-BEGIN: us-800b4660
extern "C" void func_800B3D64() {}
// LLM-HARNESS-END: us-800b4660

// LLM-HARNESS-BEGIN: us-800b4a1c
extern "C" void func_800B4120() {}
// LLM-HARNESS-END: us-800b4a1c

// LLM-HARNESS-BEGIN: us-800b4b74
extern "C" void func_800B4278() {}
// LLM-HARNESS-END: us-800b4b74

// LLM-HARNESS-BEGIN: us-800b4be4
extern "C" void func_800B42E8() {}
// LLM-HARNESS-END: us-800b4be4

// LLM-HARNESS-BEGIN: us-800b4c64
extern "C" void func_800B4368() {}
// LLM-HARNESS-END: us-800b4c64

// LLM-HARNESS-BEGIN: us-800b4cfc
extern "C" void func_800B4400() {}
// LLM-HARNESS-END: us-800b4cfc

// LLM-HARNESS-BEGIN: us-800b4d9c
extern "C" void func_800B44A0() {}
// LLM-HARNESS-END: us-800b4d9c

// LLM-HARNESS-BEGIN: us-800b4e50
extern "C" void func_800B4554() {}
// LLM-HARNESS-END: us-800b4e50

// LLM-HARNESS-BEGIN: us-800b4e84
extern "C" void func_800B4588(u32 val) {}
// LLM-HARNESS-END: us-800b4e84

// LLM-HARNESS-BEGIN: us-800b4e90
extern "C" u32 func_800B4594(u32 val) { return (val >> 16) & 0xFF; }
// LLM-HARNESS-END: us-800b4e90

// LLM-HARNESS-BEGIN: us-800b4e9c
extern "C" void func_800B45A0() {}
// LLM-HARNESS-END: us-800b4e9c

// LLM-HARNESS-BEGIN: us-800b50a4
extern "C" void func_800B47A8() {}
// LLM-HARNESS-END: us-800b50a4

// LLM-HARNESS-BEGIN: us-800b5320
extern "C" void func_800B4A24() {}
// LLM-HARNESS-END: us-800b5320

// LLM-HARNESS-BEGIN: us-800b5400
extern "C" int CfObjectMove_UnkVirtualFunc15__Q22cf12CfObjectMoveFv(void* self) { return 0; }
// LLM-HARNESS-END: us-800b5400

// LLM-HARNESS-BEGIN: us-800b5408
extern "C" void func_800B4B0C() {}
// LLM-HARNESS-END: us-800b5408

// LLM-HARNESS-BEGIN: us-800b5470
extern "C" void func_800B4B74() {}
// LLM-HARNESS-END: us-800b5470

// LLM-HARNESS-BEGIN: us-800b5484
extern "C" void func_800B4B88() {}
// LLM-HARNESS-END: us-800b5484

// LLM-HARNESS-BEGIN: us-800b559c
extern "C" void func_800B4CA0() {}
// LLM-HARNESS-END: us-800b559c

// LLM-HARNESS-BEGIN: us-800b5680
extern "C" void func_800B4D84() {}
// LLM-HARNESS-END: us-800b5680

// LLM-HARNESS-BEGIN: us-800b5854
extern "C" u32 func_800B4F58(u32 val) { return (val >> 16) & 0xFF; }
// LLM-HARNESS-END: us-800b5854

extern u16 lbl_eu_8066408C;
// LLM-HARNESS-BEGIN: us-800b5860
extern "C" unsigned short func_800B4F64()
{
    return lbl_eu_8066408C;
}
// LLM-HARNESS-END: us-800b5860

// LLM-HARNESS-BEGIN: us-800b5868
extern "C" void func_800B4F6C() {}
// LLM-HARNESS-END: us-800b5868

// LLM-HARNESS-BEGIN: us-800b587c
extern "C" void func_800B4F80() {}
// LLM-HARNESS-END: us-800b587c

// LLM-HARNESS-BEGIN: us-800b588c
extern "C" void func_800B4F90() {}
// LLM-HARNESS-END: us-800b588c

// LLM-HARNESS-BEGIN: us-800b58a0
extern "C" u32 func_800B4FA4(void* self) { return *(u32*)((u8*)self + 0x70); }
// LLM-HARNESS-END: us-800b58a0

// LLM-HARNESS-BEGIN: us-800b58a8
extern "C" void func_800B4FAC() {}
// LLM-HARNESS-END: us-800b58a8

// LLM-HARNESS-BEGIN: us-800b5c58
extern "C" void func_800B535C() {}
// LLM-HARNESS-END: us-800b5c58

// LLM-HARNESS-BEGIN: us-800b6008
extern "C" void func_800B570C() {}
// LLM-HARNESS-END: us-800b6008

// LLM-HARNESS-BEGIN: us-800b6178
extern "C" void func_800B587C() {}
// LLM-HARNESS-END: us-800b6178

// LLM-HARNESS-BEGIN: us-800b6240
extern "C" void func_800B5944() {}
// LLM-HARNESS-END: us-800b6240

// LLM-HARNESS-BEGIN: us-800b6244
extern "C" void func_800B5948() {}
// LLM-HARNESS-END: us-800b6244

// LLM-HARNESS-BEGIN: us-800b6254
extern "C" void func_800B5958(void* self) { *(u32*)((u8*)self + 896) = 0; }
// LLM-HARNESS-END: us-800b6254

// LLM-HARNESS-BEGIN: us-800b6260
extern "C" void* func_800B5964(void* self) { return 0; }
// LLM-HARNESS-END: us-800b6260

// LLM-HARNESS-BEGIN: us-800b626c
extern "C" u32 func_800B5970(void* self) { return *(u32*)((u8*)self + 0x380); }
// LLM-HARNESS-END: us-800b626c

// LLM-HARNESS-BEGIN: us-800b6274
extern "C" void func_800B5978() {}
// LLM-HARNESS-END: us-800b6274

// LLM-HARNESS-BEGIN: us-800b6290
extern "C" void func_800B5994() {}
// LLM-HARNESS-END: us-800b6290

// LLM-HARNESS-BEGIN: us-800b6d80
extern "C" void func_800B6484() {}
// LLM-HARNESS-END: us-800b6d80

// LLM-HARNESS-BEGIN: us-800b6d90
extern "C" void func_800B6494() {}
// LLM-HARNESS-END: us-800b6d90

// LLM-HARNESS-BEGIN: us-800b6da8
extern "C" u32 func_800B64AC(u32 val) { return (val >> 16) & 0xFF; }
// LLM-HARNESS-END: us-800b6da8

// LLM-HARNESS-BEGIN: us-800b6db4
extern "C" void func_800B64B8() {}
// LLM-HARNESS-END: us-800b6db4

// LLM-HARNESS-BEGIN: us-800b6dcc
extern "C" u32 func_800B64D0(u32 a, u32 b) { return a & b; }
// LLM-HARNESS-END: us-800b6dcc

// LLM-HARNESS-BEGIN: us-800b6dd8
extern "C" u32 func_800B64DC(void* self) { return *(u32*)((u8*)self + 0x98); }
// LLM-HARNESS-END: us-800b6dd8

// LLM-HARNESS-BEGIN: us-800b6de0
extern "C" u16 func_800B64E4(void* self) { return *(u16*)((u8*)self + 0x45C0); }
// LLM-HARNESS-END: us-800b6de0

// LLM-HARNESS-BEGIN: us-800b6de8
extern "C" u32 func_800B64EC(u32 val) { return (val >> 16) & 0xFF; }
// LLM-HARNESS-END: us-800b6de8

// LLM-HARNESS-BEGIN: us-800b6df4
extern "C" void func_800B64F8() {}
// LLM-HARNESS-END: us-800b6df4

// LLM-HARNESS-BEGIN: us-800b6e04
extern "C" void func_800B6508() {}
// LLM-HARNESS-END: us-800b6e04

// LLM-HARNESS-BEGIN: us-800b6e1c
extern "C" void func_800B6520(void* self, void* other) {
    unsigned int count = *(unsigned int*)((unsigned char*)self + 0x700);
    unsigned int val = *(unsigned int*)other;
    unsigned int newCount = count + 1;
    *(unsigned int*)((unsigned char*)self + 0x700) = newCount;
    float fval = *(float*)((unsigned char*)other + 4);
    void* dest = (unsigned char*)self + count * 8;
    *(unsigned int*)dest = val;
    *(float*)((unsigned char*)dest + 4) = fval;
}
// LLM-HARNESS-END: us-800b6e1c

// LLM-HARNESS-BEGIN: us-800b6e40
extern "C" void func_800B6544() {}
// LLM-HARNESS-END: us-800b6e40

// LLM-HARNESS-BEGIN: us-800b6e58
extern "C" void func_800B655C() {}
// LLM-HARNESS-END: us-800b6e58

// LLM-HARNESS-BEGIN: us-800b6fa8
extern "C" void func_800B66AC() {}
// LLM-HARNESS-END: us-800b6fa8

// LLM-HARNESS-BEGIN: us-800b6fb8
extern "C" void func_800B66BC() {}
// LLM-HARNESS-END: us-800b6fb8

// LLM-HARNESS-BEGIN: us-800b70c8
extern "C" void func_800B67CC() {}
// LLM-HARNESS-END: us-800b70c8

// LLM-HARNESS-BEGIN: us-800b70e8
extern "C" void* func_800B67EC(void* self) { return (void*)((u8*)self + 0x1a8); }
// LLM-HARNESS-END: us-800b70e8

// LLM-HARNESS-BEGIN: us-800b70f0
extern "C" void func_800B67F4(void* self) { *(u32*)((u8*)self + 1792) = 0; }
// LLM-HARNESS-END: us-800b70f0

// LLM-HARNESS-BEGIN: us-800b70fc
extern "C" void func_800B6800() {}
// LLM-HARNESS-END: us-800b70fc

// LLM-HARNESS-BEGIN: us-800b71a4
extern "C" void func_800B68A8() {}
// LLM-HARNESS-END: us-800b71a4

// LLM-HARNESS-BEGIN: us-800b7414
extern "C" void func_800B6AF4() {}
// LLM-HARNESS-END: us-800b7414

// LLM-HARNESS-BEGIN: us-800b74c0
extern "C" void func_800B6BA0(void) {}
// LLM-HARNESS-END: us-800b74c0

// LLM-HARNESS-BEGIN: us-800b74c4
extern "C" void* func_800B6BA4__Fv() {
    extern void* func_800B07E8__Fv();
    return (unsigned char*)func_800B07E8__Fv() + 0xB28;
}
// LLM-HARNESS-END: us-800b74c4

// LLM-HARNESS-BEGIN: us-800b74e8
extern "C" void* func_800B6BC8() {
    extern void* func_800B07E8__Fv();
    return (unsigned char*)func_800B07E8__Fv() + 0xB48;
}
// LLM-HARNESS-END: us-800b74e8

// LLM-HARNESS-BEGIN: us-800b750c
extern "C" void* func_800B6BEC() {
    extern void* func_800B07E8__Fv();
    return (unsigned char*)func_800B07E8__Fv() + 0xB68;
}
// LLM-HARNESS-END: us-800b750c

// LLM-HARNESS-BEGIN: us-800b7530
extern "C" void* func_800B6C10() {
    extern void* func_800B07E8__Fv();
    return (unsigned char*)func_800B07E8__Fv() + 0xB88;
}
// LLM-HARNESS-END: us-800b7530

// LLM-HARNESS-BEGIN: us-800b7554
extern "C" void* func_800B6C34() {
    extern void* func_800B07E8__Fv();
    return (unsigned char*)func_800B07E8__Fv() + 0xBE8;
}
// LLM-HARNESS-END: us-800b7554

// LLM-HARNESS-BEGIN: us-800b7578
extern "C" void* func_800B6C58() {
    extern void* func_800B07E8__Fv();
    return (unsigned char*)func_800B07E8__Fv() + 0xBC8;
}
// LLM-HARNESS-END: us-800b7578

// LLM-HARNESS-BEGIN: us-800b759c
extern "C" void* func_800B6C7C() {
    extern void* func_800B07E8__Fv();
    return (unsigned char*)func_800B07E8__Fv() + 0xC08;
}
// LLM-HARNESS-END: us-800b759c

// LLM-HARNESS-BEGIN: us-800b75c0
extern "C" void* func_800B6CA0() {
    extern void* func_800B07E8__Fv();
    return (unsigned char*)func_800B07E8__Fv() + 0xC48;
}
// LLM-HARNESS-END: us-800b75c0

// LLM-HARNESS-BEGIN: us-800b75e4
extern "C" void func_800B6CC4() {}
// LLM-HARNESS-END: us-800b75e4

// LLM-HARNESS-BEGIN: us-800b7618
extern "C" void func_800B6CF8() {}
// LLM-HARNESS-END: us-800b7618

// LLM-HARNESS-BEGIN: us-800b765c
extern "C" void* func_800B6D3C(void*);
// LLM-HARNESS-END: us-800b765c

// LLM-HARNESS-BEGIN: us-800b76f0
extern "C" void func_800B6DD0() {}
// LLM-HARNESS-END: us-800b76f0

// LLM-HARNESS-BEGIN: us-800b77e0
extern "C" void func_800B6EC0() {}
// LLM-HARNESS-END: us-800b77e0

// LLM-HARNESS-BEGIN: us-800b791c
extern "C" u32 func_800B6FFC(void* self) { return *(u32*)((u8*)self + 0xB00); }
// LLM-HARNESS-END: us-800b791c

// LLM-HARNESS-BEGIN: us-800b7924
extern "C" u32 func_800B7004(void* self) { return *(u32*)((u8*)self + 0xB04); }
// LLM-HARNESS-END: us-800b7924

// LLM-HARNESS-BEGIN: us-800b792c
extern "C" void* func_800B700C(void* self, u32 index) {
    u32 start = *(u32*)((u8*)self + 0xAFC);
    u32 count = *(u32*)((u8*)self + 0xB04);
    u32* base = *(u32**)((u8*)self + 0xAF8);
    return &base[(start + index) % count];
}
// LLM-HARNESS-END: us-800b792c

// LLM-HARNESS-BEGIN: us-800b7954
extern "C" void func_800B7034() {
    func_800B6D3C(func_800B07E8());
}
// LLM-HARNESS-END: us-800b7954

// LLM-HARNESS-BEGIN: us-800b7978
extern "C" void func_800B7058() {}
// LLM-HARNESS-END: us-800b7978

// LLM-HARNESS-BEGIN: us-800b79ac
extern "C" void func_800B708C__Fi() {}
// LLM-HARNESS-END: us-800b79ac

// LLM-HARNESS-BEGIN: us-800b7a1c
extern "C" void func_800B70FC() {}
// LLM-HARNESS-END: us-800b7a1c

// LLM-HARNESS-BEGIN: us-800b7ae4
extern "C" u32 func_800B71C4(void) {
    extern u32 lbl_eu_806640F4;
    return lbl_eu_806640F4;
}
// LLM-HARNESS-END: us-800b7ae4

// LLM-HARNESS-BEGIN: us-800b7aec
extern "C" void func_800B71CC() {}
// LLM-HARNESS-END: us-800b7aec

// LLM-HARNESS-BEGIN: us-800b7b2c
extern "C" void* func_800B720C(void* self) { return (void*)((u8*)self + 0x720); }
// LLM-HARNESS-END: us-800b7b2c

// LLM-HARNESS-BEGIN: us-800b7b34
extern "C" void func_800B7214() {}
// LLM-HARNESS-END: us-800b7b34

// LLM-HARNESS-BEGIN: us-800b7bfc
extern "C" void func_800B72DC() {}
// LLM-HARNESS-END: us-800b7bfc

// LLM-HARNESS-BEGIN: us-800b7c40
extern "C" void func_800B7320() {}
// LLM-HARNESS-END: us-800b7c40

// LLM-HARNESS-BEGIN: us-800b7d08
extern "C" void func_800B73E8(void* out_prev, void* unused, void* list_ptr) {
    (void)unused;
    void* entry = *(void**)list_ptr;
    void* prev = *(void**)entry;
    void* next = *(void**)((char*)entry + 4);
    *(void**)next = prev;
    *(void**)((char*)prev + 4) = next;
    entry = *(void**)list_ptr;
    *(void**)entry = NULL;
    *(void**)out_prev = prev;
}
// LLM-HARNESS-END: us-800b7d08

// LLM-HARNESS-BEGIN: us-800b7d30
extern "C" void func_800B7410() {}
// LLM-HARNESS-END: us-800b7d30

// LLM-HARNESS-BEGIN: us-800b7ed0
extern "C" unsigned short func_800B75B4()
{
    return lbl_eu_8066408C;
}
// LLM-HARNESS-END: us-800b7ed0

// LLM-HARNESS-BEGIN: us-800b7ed8
extern "C" unsigned short func_800B75BC()
{
    return lbl_eu_8066408C;
}
// LLM-HARNESS-END: us-800b7ed8

// LLM-HARNESS-BEGIN: us-800b7ee0
extern "C" void func_800B75C4(void* self, u32 mask, int enable) {
    u32* field = (u32*)((u8*)self + 0x6C);
    if (enable) {
        *field |= mask;
    } else {
        *field &= ~mask;
    }
}
// LLM-HARNESS-END: us-800b7ee0

// LLM-HARNESS-BEGIN: us-800b7f08
extern "C" void func_800B75EC() {}
// LLM-HARNESS-END: us-800b7f08

// LLM-HARNESS-BEGIN: us-800b7f9c
extern "C" int func_800B7680(void* self) {
    void* head = *(void**)((char*)self + 4);
    void* cur = *(void**)head;
    int count = 0;
    while (cur != head) {
        cur = *(void**)cur;
        count++;
    }
    return count;
}
// LLM-HARNESS-END: us-800b7f9c

// LLM-HARNESS-BEGIN: us-800b7fc0
extern "C" void func_800B76A4(void* self) {
    void* ptr = func_800B07E8();
    func_800B6D3C((void*)((u8*)ptr + 0xB28));
}
// LLM-HARNESS-END: us-800b7fc0

// LLM-HARNESS-BEGIN: us-800b7fe8
extern "C" void func_800B76CC(void* self) {
    void* ptr = func_800B07E8();
    func_800B6D3C((void*)((u8*)ptr + 0xB48));
}
// LLM-HARNESS-END: us-800b7fe8

// LLM-HARNESS-BEGIN: us-800b8010
extern "C" void func_800B76F4(void* self) {
    void* ptr = func_800B07E8();
    func_800B6D3C((void*)((u8*)ptr + 0xB68));
}
// LLM-HARNESS-END: us-800b8010

// LLM-HARNESS-BEGIN: us-800b8038
extern "C" void* func_800B771C(void* self) {
    extern void* func_800B07E8__Fv();
    return func_800B6D3C((char*)func_800B07E8__Fv() + 0xB88);
}
// LLM-HARNESS-END: us-800b8038

// LLM-HARNESS-BEGIN: us-800b8060
extern "C" void* func_800B7744(void* self) {
    extern void* func_800B07E8__Fv();
    return func_800B6D3C((char*)func_800B07E8__Fv() + 0xBE8);
}
// LLM-HARNESS-END: us-800b8060

// LLM-HARNESS-BEGIN: us-800b8088
extern "C" void* func_800B776C(void* self) {
    extern void* func_800B07E8__Fv();
    return func_800B6D3C((char*)func_800B07E8__Fv() + 0xBC8);
}
// LLM-HARNESS-END: us-800b8088

// LLM-HARNESS-BEGIN: us-800b80b0
extern "C" void* func_800B7794(void* self) {
    extern void* func_800B07E8__Fv();
    return func_800B6D3C((char*)func_800B07E8__Fv() + 0xC08);
}
// LLM-HARNESS-END: us-800b80b0

// LLM-HARNESS-BEGIN: us-800b80d8
extern "C" void* func_800B77BC(void* self) {
    extern void* func_800B07E8__Fv();
    return func_800B6D3C((char*)func_800B07E8__Fv() + 0xBA8);
}
// LLM-HARNESS-END: us-800b80d8

// LLM-HARNESS-BEGIN: us-800b8100
extern "C" void func_800B77E4() {}
// LLM-HARNESS-END: us-800b8100

// LLM-HARNESS-BEGIN: us-800b8138
extern "C" void func_800B781C() {}
// LLM-HARNESS-END: us-800b8138

// LLM-HARNESS-BEGIN: us-800b8170
extern "C" void func_800B7854() {}
// LLM-HARNESS-END: us-800b8170

// LLM-HARNESS-BEGIN: us-800b81a8
extern "C" void func_800B788C(void* param_1) { extern void* func_800B07E8(); extern void func_800B6DD0(void*, void*); void* ptr = func_800B07E8(); func_800B6DD0((char*)ptr + 0xb88, param_1); }
// LLM-HARNESS-END: us-800b81a8

// LLM-HARNESS-BEGIN: us-800b81e0
extern "C" void func_800B78C4() {}
// LLM-HARNESS-END: us-800b81e0

// LLM-HARNESS-BEGIN: us-800b8218
extern "C" void func_800B78FC() {}
// LLM-HARNESS-END: us-800b8218

// LLM-HARNESS-BEGIN: us-800b8250
extern "C" void func_800B7934() {}
// LLM-HARNESS-END: us-800b8250

// LLM-HARNESS-BEGIN: us-800b8288
extern "C" void func_800B796C() {}
// LLM-HARNESS-END: us-800b8288

// LLM-HARNESS-BEGIN: us-800b82c0
extern "C" void func_800B79A4() {}
// LLM-HARNESS-END: us-800b82c0

// LLM-HARNESS-BEGIN: us-800b8334
extern "C" void func_800B7A18() {}
// LLM-HARNESS-END: us-800b8334

// LLM-HARNESS-BEGIN: us-800b840c
extern "C" void func_800B7AF0() {}
// LLM-HARNESS-END: us-800b840c

// LLM-HARNESS-BEGIN: us-800b8cc8
extern "C" void func_800B83AC() {}
// LLM-HARNESS-END: us-800b8cc8

// LLM-HARNESS-BEGIN: us-800b8e40
extern "C" void func_800B8524() {}
// LLM-HARNESS-END: us-800b8e40

// LLM-HARNESS-BEGIN: us-800b9118
extern "C" void func_800B87FC() {}
// LLM-HARNESS-END: us-800b9118

// LLM-HARNESS-BEGIN: us-800b911c
extern "C" void func_800B8800() {}
// LLM-HARNESS-END: us-800b911c

// LLM-HARNESS-BEGIN: us-800b9120
extern "C" void func_800B8804__FPvPQ22cf13IFactoryEvent() {}
// LLM-HARNESS-END: us-800b9120

// LLM-HARNESS-BEGIN: us-800b91fc
extern "C" void func_800B88E0() {}
// LLM-HARNESS-END: us-800b91fc

// LLM-HARNESS-BEGIN: us-800b923c
extern "C" void func_800B8920() {}
// LLM-HARNESS-END: us-800b923c

// LLM-HARNESS-BEGIN: us-800b92e8
extern "C" void func_800B89CC() {}
// LLM-HARNESS-END: us-800b92e8

// LLM-HARNESS-BEGIN: us-800b9380
extern "C" void func_800B8A64() {}
// LLM-HARNESS-END: us-800b9380

// LLM-HARNESS-BEGIN: us-800b9418
extern "C" void func_800B8AFC() {}
// LLM-HARNESS-END: us-800b9418

// LLM-HARNESS-BEGIN: us-800b94b0
extern "C" void func_800B8B94() {}
// LLM-HARNESS-END: us-800b94b0

// LLM-HARNESS-BEGIN: us-800b9594
extern "C" void func_800B8C78() {}
// LLM-HARNESS-END: us-800b9594

// LLM-HARNESS-BEGIN: us-800b9678
extern "C" void func_800B8D5C() {}
// LLM-HARNESS-END: us-800b9678

// LLM-HARNESS-BEGIN: us-800b98e0
extern "C" void func_800B8FC4() {}
// LLM-HARNESS-END: us-800b98e0

// LLM-HARNESS-BEGIN: us-800b9bd0
extern "C" void func_800B92B4() {
    func_800B07E8();
    ((void(*)())func_800B1120)();
}
// LLM-HARNESS-END: us-800b9bd0

// LLM-HARNESS-BEGIN: us-800b9bf4
extern "C" void func_800B92D8() {
    func_800B07E8();
    ((void(*)())func_800B15A4)();
}
// LLM-HARNESS-END: us-800b9bf4

// LLM-HARNESS-BEGIN: us-800b9c18
extern "C" void func_800B92FC() {}
// LLM-HARNESS-END: us-800b9c18

// LLM-HARNESS-BEGIN: us-800b9c5c
extern "C" void func_800B9340() {
    func_800B07E8();
    ((void(*)())func_800B87FC)();
}
// LLM-HARNESS-END: us-800b9c5c

// LLM-HARNESS-BEGIN: us-800b9c80
extern "C" void func_800B9364() {
    func_800B07E8();
    ((void(*)())func_800B8800)();
}
// LLM-HARNESS-END: us-800b9c80

// LLM-HARNESS-BEGIN: us-800b9ca4
extern "C" void func_800B9388() {
    func_800B07E8();
    ((void(*)())func_800B1954)();
}
// LLM-HARNESS-END: us-800b9ca4

// LLM-HARNESS-BEGIN: us-800b9cc8
extern "C" void func_800B93AC() {
    func_800B07E8();
    ((void(*)())func_800B1B2C)();
}
// LLM-HARNESS-END: us-800b9cc8

// LLM-HARNESS-BEGIN: us-800b9cec
extern "C" void func_800B93D0() {}
// LLM-HARNESS-END: us-800b9cec

// LLM-HARNESS-BEGIN: us-800b9d20
extern "C" void func_800B9404() {}
// LLM-HARNESS-END: us-800b9d20

// LLM-HARNESS-BEGIN: us-800b9d54
extern "C" void func_800B9438() {}
// LLM-HARNESS-END: us-800b9d54

// LLM-HARNESS-BEGIN: us-800b9d88
extern "C" void func_800B946C() {}
// LLM-HARNESS-END: us-800b9d88

// LLM-HARNESS-BEGIN: us-800b9dbc
extern "C" void func_800B94A0() {}
// LLM-HARNESS-END: us-800b9dbc

// LLM-HARNESS-BEGIN: us-800b9df0
extern "C" void func_800B94D4() {}
// LLM-HARNESS-END: us-800b9df0

// LLM-HARNESS-BEGIN: us-800b9e40
extern "C" void func_800B9524(u32 a, u32 b) {
    func_800AA2BC(a, b);
    ((void(*)())func_800B94D4)();
}
// LLM-HARNESS-END: us-800b9e40

// LLM-HARNESS-BEGIN: us-800b9e64
extern "C" void func_800B9548() {}
// LLM-HARNESS-END: us-800b9e64

// LLM-HARNESS-BEGIN: us-800b9e98
extern "C" void func_800B957C() {}
// LLM-HARNESS-END: us-800b9e98

// LLM-HARNESS-BEGIN: us-800b9fe0
extern "C" void func_800B96C4(void* self, u32 val) { *(u32*)((u8*)self + 0x734) = val; }
// LLM-HARNESS-END: us-800b9fe0

// LLM-HARNESS-BEGIN: us-800b9fe8
extern "C" void func_800B96CC(void* self, u8 a, u8 b) { ((u8*)self)[1850] = a; ((u8*)self)[1851] = b; }
// LLM-HARNESS-END: us-800b9fe8

// LLM-HARNESS-BEGIN: us-800b9ff4
extern "C" void func_800B96D8(void* self, u32 val) { *(u32*)((u8*)self + 0x720) = val; }
// LLM-HARNESS-END: us-800b9ff4

// LLM-HARNESS-BEGIN: us-800b9ffc
extern "C" void func_800B96E0(void* self, u16 val) { *(u16*)((u8*)self + 0x738) = val; }
// LLM-HARNESS-END: us-800b9ffc

// LLM-HARNESS-BEGIN: us-800ba004
extern "C" void func_800B96E8() {}
// LLM-HARNESS-END: us-800ba004

// LLM-HARNESS-BEGIN: us-800ba020
extern "C" void func_800B9704(void* self, float val) { *(float*)((u8*)self + 0x730) = val; }
// LLM-HARNESS-END: us-800ba020

// LLM-HARNESS-BEGIN: us-800ba028
extern "C" void __ct__800B970C() {}
// LLM-HARNESS-END: us-800ba028

// LLM-HARNESS-BEGIN: us-800ba0bc
extern "C" void func_800B97A0() {}
// LLM-HARNESS-END: us-800ba0bc

// LLM-HARNESS-BEGIN: us-800ba178
extern "C" void func_800B985C() {}
// LLM-HARNESS-END: us-800ba178

// LLM-HARNESS-BEGIN: us-800ba1e4
extern "C" void func_800B98C8() {}
// LLM-HARNESS-END: us-800ba1e4

// LLM-HARNESS-BEGIN: us-800ba2a0
extern "C" u32 func_800B9984(void* self) { return *(u32*)((u8*)self + 0x620); }
// LLM-HARNESS-END: us-800ba2a0

// LLM-HARNESS-BEGIN: us-800ba2a8
extern "C" void func_800B998C() {}
// LLM-HARNESS-END: us-800ba2a8

// LLM-HARNESS-BEGIN: us-800ba2d8
extern "C" void func_800B99BC() {}
// LLM-HARNESS-END: us-800ba2d8

// LLM-HARNESS-BEGIN: us-800ba308
extern "C" void func_800B99EC() {}
// LLM-HARNESS-END: us-800ba308

// LLM-HARNESS-BEGIN: us-800ba34c
extern "C" void func_800B9A30() {}
// LLM-HARNESS-END: us-800ba34c

// --- hard-symbol stubs (scaffold_hard_symbols) ---
namespace cf { struct TboxInfo { char _pad; }; }
namespace cf { struct IFactoryEvent { char _pad; }; }
template <typename T> class _reslist_base { public: ~_reslist_base(); };
template <typename T> class reslist { public: ~reslist(); };
// LLM-HARNESS-BEGIN: us-800b145c
template <> _reslist_base<cf::TboxInfo>::~_reslist_base() {}
// LLM-HARNESS-END: us-800b145c
// LLM-HARNESS-BEGIN: us-800b1518
template <> reslist<cf::TboxInfo>::~reslist() {}
// LLM-HARNESS-END: us-800b1518
// LLM-HARNESS-BEGIN: us-800b1614
template <> _reslist_base<cf::IFactoryEvent*>::~_reslist_base() {}
// LLM-HARNESS-END: us-800b1614
// LLM-HARNESS-BEGIN: us-800b16c8
template <> reslist<cf::IFactoryEvent*>::~reslist() {}
// LLM-HARNESS-END: us-800b16c8
// LLM-HARNESS-BEGIN: us-800ba35c
extern "C" void sinit_800B9A40() {}
// LLM-HARNESS-END: us-800ba35c
