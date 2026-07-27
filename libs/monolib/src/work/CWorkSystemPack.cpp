// Auto-scaffolded catalog TU for monolib/src/work/CWorkSystemPack
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

CWorkSystemPack::CWorkSystemPack() {}

void __dt___reslist_base_CPackItem(){}

void func_804DDADC(){}

void __dt__reslist_CPackItem(){}

void __dt___reslist_base_CArcItem(){}

void func_804DDBD8(void* self) {
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

void __dt__reslist_CArcItem(){}

void CWorkSystemPack::~CWorkSystemPack() {}

void func_804DDCD4(){}

void func_804DDD54(){}

void CWorkSystemPack::func_804DDDF4() {}

const char* const* lbl_eu_80665A14;

// Store the PKH filenames array pointer for later retrieval by the work system.
void SavePkhFilenamesArrayPtr__15CWorkSystemPackFPCPCc(const char* const* pArray) {
    lbl_eu_80665A14 = pArray;
}

const char* const* lbl_eu_80665A18;

// Store the static arc filename string pointer for later retrieval by the work system.
void SaveStaticArcFilenameStringPtr__15CWorkSystemPackFPCPCc(const char* const* pFilenameStr) {
    lbl_eu_80665A18 = pFilenameStr;
}

void func_804DDE3C(){}

void func_804DDF00(){}

void func_eu_804E2340(){}

void CWorkSystemPack::func_804DDFBC() {}

void func_804DE010(){}

void CWorkSystemPack::func_804DE08C() {}

void CWorkSystemPack::func_804DE100() {}

void CWorkSystemPack::wkUpdate() {}

void CWorkSystemPack::wkStandbyLogin() {}

void CWorkSystemPack::wkStandbyLogout() {}
