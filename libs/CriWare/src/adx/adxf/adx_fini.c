// Auto-scaffolded catalog TU for CriWare/src/adx/adxf/adx_fini
// Replace stubs with high-level C/C++ during decompilation.

#include <harness_catalog.h>
#include <string.h>

// Static data pointer stored at field_0x04 during Init (lbl_eu_80515788)
extern u8 lbl_eu_80515788[];

// ADXF global work structure (lbl_eu_805DFDA8)
struct ADXF_Work {
    s32 refcount;           // 0x00: initialization reference count
    u8* field_0x04;         // 0x04: pointer to static data (lbl_eu_80515788)
    u8 field_0x08[0x340];   // 0x08
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

// Global ADXF work structure in BSS
struct ADXF_Work ADXF_Work;

// Forward declarations for ADXF functions defined in adx_fs.c
void ADXF_CloseAll(void);

void ADXF_Init(void) {
    struct ADXF_Work* work = &ADXF_Work;
    work->field_0x04 = lbl_eu_80515788;

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
    struct ADXF_Work* work = &ADXF_Work;
    if (--work->refcount != 0) return;
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
