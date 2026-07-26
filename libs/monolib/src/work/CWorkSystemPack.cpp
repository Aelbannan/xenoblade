// Auto-scaffolded catalog TU for monolib/src/work/CWorkSystemPack
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern "C" void __ct__15CWorkSystemPackFPCcP11CWorkThread() {}

extern "C" void __dt___reslist_base_CPackItem() {}

extern "C" void func_804DDADC() {}

extern "C" void __dt__reslist_CPackItem() {}

extern "C" void __dt___reslist_base_CArcItem() {}

extern "C" void func_804DDBD8(void* self) {
    uint32_t* head = *(uint32_t**)((uint8_t*)self + 4);
    uint32_t* node = (uint32_t*)*head;
    goto loop_cond;
loop_body:
    uint32_t* cur = node;
    node = (uint32_t*)*node;
    *cur = 0;
loop_cond:
    head = *(uint32_t**)((uint8_t*)self + 4);
    if (node != head) goto loop_body;
    *head = (uint32_t)head;
}

extern "C" void __dt__reslist_CArcItem() {}

extern "C" void __dt__15CWorkSystemPackFv() {}

extern "C" void func_804DDCD4() {}

extern "C" void func_804DDD54() {}

extern "C" void func_804DDDF4__15CWorkSystemPackFPCcPvPUl() {}

extern "C" const char* const* lbl_eu_80665A14;

// Store the PKH filenames array pointer for later retrieval by the work system.
extern "C" void SavePkhFilenamesArrayPtr__15CWorkSystemPackFPCPCc(const char* const* pArray) {
    lbl_eu_80665A14 = pArray;
}

extern "C" const char* const* lbl_eu_80665A18;

// Store the static arc filename string pointer for later retrieval by the work system.
extern "C" void SaveStaticArcFilenameStringPtr__15CWorkSystemPackFPCPCc(const char* const* pFilenameStr) {
    lbl_eu_80665A18 = pFilenameStr;
}

extern "C" void func_804DDE3C() {}

extern "C" void func_804DDF00() {}

extern "C" void func_eu_804E2340() {}

extern "C" void func_804DDFBC__15CWorkSystemPackFi() {}

extern "C" void func_804DE010() {}

extern "C" void func_804DE08C__15CWorkSystemPackFv() {}

extern "C" void func_804DE100__15CWorkSystemPackFv() {}

extern "C" void wkUpdate__15CWorkSystemPackFv() {}

extern "C" void wkStandbyLogin__15CWorkSystemPackFv() {}

extern "C" void wkStandbyLogout__15CWorkSystemPackFv() {}
