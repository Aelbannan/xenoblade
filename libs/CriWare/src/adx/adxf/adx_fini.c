// CriWare ADX: ADXF subsystem init/finalize (libs/CriWare/src/adx/adxf/adx_fini.c)
//
// Matches retail ADXF_Init / ADXF_Finish. Absorbed from criware_data:
//   .rodata adxf_build (0x53) + .bss adxf_head (0x8 head).
// The rest of the ADXF work image (from +0x8) lives in contiguous .bss
// owned by adx_fs.c; we overlay the full struct for field access.

#include <harness_catalog.h>
#include <string.h>

// JP name for this CRI version string (.rodata, 0x53 bytes, scope:global,
// data:string_table). The single array, including both embedded terminators,
// is the whole object; the retail US/EU entry has linker tail padding.
const char adxf_build[] = "\nADXF/WII Ver.7.71 Build:Oct 28 2009 21:11:19\n\0Append: MW4302 WII30Jul2008Patch04\n";

// Full ADXF work layout (runtime overlay across fini head + adx_fs .bss).
struct ADXF_Work {
    s32 refcount;           // 0x00
    const char* field_0x04; // 0x04 -> adxf_build
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

// Retail symbol is only the 8-byte head at 0x805DFDA8; the rest of the
// 0x880 block is defined after the code (see the bottom of this file).
extern u64 adxf_head;

// Forward declarations for ADXF functions defined in adx_fs.c
void ADXF_CloseAll(void);

void ADXF_Init(void) {
    struct ADXF_Work* work = (struct ADXF_Work*)&adxf_head;
    work->field_0x04 = adxf_build;

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
    struct ADXF_Work* work = (struct ADXF_Work*)&adxf_head;
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

/* .bss: defined after the code so MWCC addresses every object through its
 * own retail symbol.  Reverse definition order gives the retail layout. */
u32 adxf_pt_stat;
u32 adxf_pt_id;
void* adxf_curpt;
u32 adxf_ftbl_idx;
s32 adxf_obci_flag;
s32 adxf_reqidx;
u16 adxf_reqseq[16];
u8 adxf_reqslot[0x100];
u32 adxf_pttbl[256];
u8 adxf_ptwork[0x340];
u64 adxf_head;
