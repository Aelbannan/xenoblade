// Auto-scaffolded catalog TU for kyoshin/CSaveLoad
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CSaveLoad.hpp"

u8 CSaveLoad::func_8028F664() { return mField123; }

void CSLCur::func_8028EA74() {}

void func_8028EAF8(){}

void func_8028EB70(){}

void func_8028EB9C(){}

void func_8028EC04(void* arg1, const void* arg2) {
    void* ptr1 = *(void**)((char*)arg1 + 8);
    void* ptr2 = *(void**)((char*)ptr1 + 0x10);
    const float* src = (const float*)arg2;
    float* dst = (float*)((char*)ptr2 + 0x2c);
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
}

void func_8028EC28(){}

void func_8028EC74(){}

void func_8028ED0C(CSLCur* data, int r4) {
    data->mField0 = r4;
    data->mField4 = 0;
    data->mField8 = 0;
    data->mFieldC = 0;
    data->mFieldD = 0;
    data->mFieldE = 1;
}

void __dt__8028ED30(){}

void func_8028ED70(){}

void func_8028EDF8(){}

void func_8028EE68(){}

void CSLCur::func_8028EEC0() {
    mFieldC = 1;
    mFieldD = 1;
    mFieldE = 0;
}

void func_8028EED8(){}

void func_8028EF24(){}

void func_8028EF74(){}

void __ct__CSaveLoad(){}

CSaveLoad::~CSaveLoad() {}

void CSaveLoad::func_8028F23C() {}

void func_8028F2CC(){}

void func_8028F3D4(){}

void func_8028F4AC(){}

void func_8028F5C4(){}


void func_8028F66C(){}

void func_8028F6DC(){}

void func_8028F774(){}

void func_8028F7D0(){}

void func_8028F904(){}

void func_8028FA54(){}

void func_8028FB20(){}

void func_8028FC18(){}

void func_8028FE50(){}

u8 CSaveLoad::func_8028FEC4() { return mField12A; }

void func_8028FECC(){}

void func_8028FFD4(){}

void func_80290094(){}

void func_802900E0(){}

void func_80290140(){}

void func_8029018C(){}

void func_802901D8(){}

void func_8029022C(){}

void func_8029040C(){}

void func_8029049C(CSaveLoad* p) {
    if (p->mField11E != 0) {
        p->mField121 = 3;
    }
}

void func_802904B4(){}

void func_8029078C(){}

void func_802907E4(){}

void func_80290844(){}

void func_802908A4(){}

void func_80290994(){}

void func_802910D4(){}

void func_80291204(){}

void CSaveLoad::OnFileEvent() {}

u32 func_8029183C(void) {
    extern u32 lbl_eu_80662AD0;
    return lbl_eu_80662AD0;
}

cf::CfAward::~CfAward() {}

void func_802918AC(){}

void func_8029194C(){}

void func_802919A0(){}

void func_80291A04(){}

void cf::CfAward::func_80291B18() {}

void func_80291B30(){}

void func_80291BF8(){}

void func_80291C60(){}

void func_80291D98(){}

void func_80291EF0(){}

void func_80292000(){}

void func_80292418(){}

void func_802929C8(){}

void func_80292EC0(){}

void sinit_802930E0(){}
