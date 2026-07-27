// Auto-scaffolded catalog TU for kyoshin/plugin/ocCfp
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/plugin/ocCfp.hpp"

void func_80045560(){}

void func_8004559C(){}

void func_800455E8(){}

void func_80045634(){}

int func_80045694(void* self){
    extern void* vmOCPropertyGet(void*);
    extern void func_80086D90__Q22cf13CfGameManagerFv(unsigned short);
    void* prop = vmOCPropertyGet(self);
    func_80086D90__Q22cf13CfGameManagerFv(*(unsigned short*)((char*)prop + 4));
    return 0;
}

int func_800456C4(void* self){
    extern void* vmOCPropertyGet(void*);
    extern void func_80086D94__Q22cf13CfGameManagerFv(unsigned short);
    void* prop = vmOCPropertyGet(self);
    func_80086D94__Q22cf13CfGameManagerFv(*(unsigned short*)((char*)prop + 4));
    return 0;
}

void getTimeIdxMin(){}

void getTimeIdxMax(){}

extern "C" {
BOOL vmOCRegist(OCData* pOC);
}
extern OCData lbl_eu_80525D58[];
void ocCfpRegist() {
    vmOCRegist(lbl_eu_80525D58);
}
