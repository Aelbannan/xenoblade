// Auto-scaffolded catalog TU for CriWare/src/adx/lsc/lsc_ini
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// Forward declarations from other CriWare TUs
struct LSC_CriticalSection;
extern struct LSC_CriticalSection lbl_eu_80518418;
void LSC_LockCrs(struct LSC_CriticalSection* cs);
void LSC_UnlockCrs(struct LSC_CriticalSection* cs);

struct LSC_StmEntry;
void LSC_Destroy(struct LSC_StmEntry* stm);

void LSC_EntryErrFunc(u32 param1, u32 param2);

// Global state
extern u32 lbl_eu_805E7D38;
extern u8 lbl_eu_805E7D40[0x4700];

// Opaque critical section type (8 bytes, allocated on stack).
// field_0x0 is volatile so its read carries a side effect.
struct LSC_CriticalSection {
    volatile u32 field_0x0;
    u32 field_0x4;
};

// Stream entry struct (size 0x238, used in LSC_Finish loop)
struct LSC_StmEntry {
    s8 field_0x0;
    u8 field_0x1[0x237];
};

void LSC_Init(void) {
    struct LSC_CriticalSection cs;

    // Retail carries a dead read of the global critical section here.
    (void)lbl_eu_80518418.field_0x0;

    LSC_LockCrs(&cs);
    if (lbl_eu_805E7D38 == 0) {
        memset(lbl_eu_805E7D40, 0, 0x4700);
        LSC_EntryErrFunc(0, 0);
    }
    lbl_eu_805E7D38++;
    LSC_UnlockCrs(&cs);
}void LSC_Finish() {
    struct LSC_CriticalSection cs;
    struct LSC_StmEntry* entry;
    s32 i;

    LSC_LockCrs(&cs);
    if (--lbl_eu_805E7D38 == 0) {
        entry = (struct LSC_StmEntry*)lbl_eu_805E7D40;
        for (i = 0; i < 0x20; i++) {
            if (entry->field_0x0 == 1) {
                LSC_Destroy(entry);
            }
            entry++;
        }
        memset(lbl_eu_805E7D40, 0, 0x4700);
        LSC_EntryErrFunc(0, 0);
    }
    LSC_UnlockCrs(&cs);
}
