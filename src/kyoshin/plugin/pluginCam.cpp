// Auto-scaffolded catalog TU for kyoshin/plugin/pluginCam
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern "C" void select() {}

extern "C" void restore() {}

extern "C" void setPos() {}

extern "C" void setLookat() {}

extern "C" void func_800605D0() {}

extern "C" void setDir() {}

extern "C" void func_800606AC() {}

extern "C" void setRotX() {}

extern "C" void func_80060738() {}

extern "C" void setRotY() {}

extern "C" void func_800607C4() {}

extern "C" void setFov() {}

extern "C" void setTarget() {}

extern "C" void setPosOfs() {}

extern "C" void func_80060A08() {}

extern "C" void setLookatOfs() {}

extern "C" void func_80060B84() {}

extern "C" void getPos() {}

extern "C" void getRot() {}

extern "C" void keyBegin() {}

extern "C" void keyEnd() {}

extern "C" void keyAdd() {}

extern "C" void isKeyMove() {}

extern "C" void waitKeyMove() {}

extern "C" void shake() {}

extern "C" void stopShake() {}

extern "C" void pluginCamRegist() {
    extern void vmPluginRegist(void*, void*);
    extern char lbl_eu_804FB1A4[];
    extern char lbl_eu_80526560[];
    vmPluginRegist((void*)lbl_eu_804FB1A4, (void*)lbl_eu_80526560);
}
