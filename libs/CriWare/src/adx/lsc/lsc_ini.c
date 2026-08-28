#include <harness_catalog.h>
#include <string.h>

extern const char lbl_eu_805183E8[];

__declspec(section ".rodata") __attribute__((aligned(8))) const void *lbl_eu_80518418[2] = {
    lbl_eu_805183E8, 0
};

u32 lbl_eu_805E7D38[2];
u8 lbl_eu_805E7D40[0x4700];

struct LSC_CriticalSection {
    volatile u32 field_0x0;
    u32 field_0x4;
};

struct LSC_StmEntry {
    s8 field_0x0;
    u8 field_0x1[0x237];
};

void LSC_LockCrs(struct LSC_CriticalSection* cs);
void LSC_UnlockCrs(struct LSC_CriticalSection* cs);
void LSC_Destroy(struct LSC_StmEntry* stm);
void LSC_EntryErrFunc(u32 param1, u32 param2);

void LSC_Init(void) {
    struct LSC_CriticalSection cs;
    (void)lbl_eu_80518418[0];
    LSC_LockCrs(&cs);
    if (lbl_eu_805E7D38[0] == 0) {
        memset(lbl_eu_805E7D40, 0, 0x4700);
        LSC_EntryErrFunc(0, 0);
    }
    lbl_eu_805E7D38[0]++;
    LSC_UnlockCrs(&cs);
}

void LSC_Finish(void) {
    struct LSC_CriticalSection cs;
    struct LSC_StmEntry* entry;
    s32 i;
    LSC_LockCrs(&cs);
    if (--lbl_eu_805E7D38[0] == 0) {
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
