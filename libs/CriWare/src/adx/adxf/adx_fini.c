// CriWare ADX: ADXF subsystem init/finalize (libs/CriWare/src/adx/adxf/adx_fini.c)
//
// Matches retail ADXF_Init / ADXF_Finish. Absorbed from criware_data:
//   .rodata lbl_eu_80515788 (0x58) + .bss lbl_eu_805DFDA8 (0x8 head).
// The rest of the ADXF work image (from +0x8) lives in contiguous .bss
// owned by adx_fs.c; we overlay the full struct for field access.

#include <harness_catalog.h>
#include <string.h>

// Version / build banner (.rodata 0x80515788, 0x58 bytes, align 8).
// Plain `const unsigned int[]` lands in .data under these flags; force
// .rodata like the monolib dissolve recipe.
__declspec(section ".rodata") __attribute__((aligned(8)))
const unsigned char lbl_eu_80515788[0x58] = {
    0x0A,0x41,0x44,0x58,0x46,0x2F,0x57,0x49,0x49,0x20,0x56,0x65,0x72,0x2E,0x37,0x2E,
    0x37,0x31,0x20,0x42,0x75,0x69,0x6C,0x64,0x3A,0x4F,0x63,0x74,0x20,0x32,0x38,0x20,
    0x32,0x30,0x30,0x39,0x20,0x32,0x31,0x3A,0x31,0x31,0x3A,0x31,0x39,0x0A,0x00,0x41,
    0x70,0x70,0x65,0x6E,0x64,0x3A,0x20,0x4D,0x57,0x34,0x33,0x30,0x32,0x20,0x57,0x49,
    0x49,0x33,0x30,0x4A,0x75,0x6C,0x32,0x30,0x30,0x38,0x50,0x61,0x74,0x63,0x68,0x30,
    0x34,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,
};

// Full ADXF work layout (runtime overlay across fini head + adx_fs .bss).
struct ADXF_Work {
    s32 refcount;           // 0x00
    u8* field_0x04;         // 0x04 -> lbl_eu_80515788
    u8 field_0x08[0x340];   // 0x08  (adx_fs .bss)
    u8 field_0x348[0x400];  // 0x348
    u8 field_0x748[0x100];  // 0x748
    u8 field_0x848[0x20];   // 0x848
    u32 field_0x868;        // 0x868
    u32 field_0x86C;        // 0x86C
    u32 field_0x870;        // 0x870
    u32 field_0x874;        // 0x874
    s32 field_0x878;        // 0x878
    u32 field_0x87C;        // 0x87C
};

// Retail symbol is only the 8-byte head at 0x805DFDA8.
struct {
    s32 refcount;
    u8* field_0x04;
} lbl_eu_805DFDA8;

// Forward declarations for ADXF functions defined in adx_fs.c
void ADXF_CloseAll(void);

void ADXF_Init(void) {
    struct ADXF_Work* work = (struct ADXF_Work*)&lbl_eu_805DFDA8;
    work->field_0x04 = (u8*)lbl_eu_80515788;

    if (work->refcount == 0) {
        memset(work->field_0x08, 0, 0x340);
        memset(work->field_0x348, 0, 0x400);
        memset(work->field_0x748, 0xFF, 0x100);
        memset(work->field_0x848, 0, 0x20);
        work->field_0x868 = 0;
        work->field_0x86C = 0;
        work->field_0x870 = 0;
        work->field_0x874 = 0;
        work->field_0x878 = -1;
        work->field_0x87C = 1;
    }

    work->refcount++;
}

void ADXF_Finish(void) {
    // Decrement the init refcount; on the last release, reset all ADXF state.
    // NOTE: retail forms the work base with a single lis/addi pair in r31 after
    // the prologue stores; MWCC always hoists the address computation into an
    // arg register first (lis rX before the stw spills) and keeps a second copy
    // for the refcount access. This hoist is a catalogued static cap
    // (docs/MWCC_CASES.md "adx_fini - unattainable lis-hoist schedule"):
    // pointer const-ness, scoping, direct member access and register hints
    // were all ruled out.
    struct ADXF_Work* work = (struct ADXF_Work*)&lbl_eu_805DFDA8;
    if (--work->refcount == 0) {
        ADXF_CloseAll();
        work->field_0x87C = 1;
        work->field_0x878 = -1;
        work->field_0x874 = 0;
        work->field_0x870 = 0;
        work->field_0x86C = 0;
        work->field_0x868 = 0;
        memset(work->field_0x848, 0, 0x20);
        memset(work->field_0x748, 0xFF, 0x100);
        memset(work->field_0x348, 0, 0x400);
        memset(work->field_0x08, 0, 0x340);
    }
}
