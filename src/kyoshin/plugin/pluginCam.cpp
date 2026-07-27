// Auto-scaffolded catalog TU for kyoshin/plugin/pluginCam
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

void select(){}

void restore(){}

void setPos(){}

void setLookat(){}

extern "C" void func_800605D0() {}

void setDir(){}

extern "C" void func_800606AC() {}

void setRotX(){}

extern "C" void func_80060738() {}

void setRotY(){}

extern "C" void func_800607C4() {}

void setFov(){}

void setTarget(){}

void setPosOfs(){}

extern "C" void func_80060A08() {}

void setLookatOfs(){}

extern "C" void func_80060B84() {}

void getPos(){}

void getRot(){}

void keyBegin(){}

void keyEnd(){}

void keyAdd(){}

void isKeyMove(){}

void waitKeyMove(){}

void shake(){}

void stopShake(){}

extern "C" void pluginCamRegist() {
    extern void vmPluginRegist(void*, void*);
    extern char lbl_eu_804FB1A4[];
    extern char lbl_eu_80526560[];
    vmPluginRegist((void*)lbl_eu_804FB1A4, (void*)lbl_eu_80526560);
}
