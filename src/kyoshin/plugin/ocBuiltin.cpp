#include "kyoshin/plugin/ocBuiltin.hpp"

extern "C" {
void vmBuiltinOCRegist(OCData* pOC);
// Built-in OC data block registered by ocBuiltinRegist (0x80524BF8, size 0x10)
extern OCData lbl_eu_80524BF8;
}

int isExistProperty(VMThread* pThread){ return 0; }

int isExistSelector(VMThread* pThread){ return 0; }

int getOCName(VMThread* pThread){ return 0; }

extern "C" void ocBuiltinRegist() {
    vmBuiltinOCRegist(&lbl_eu_80524BF8);
}
