// Auto-scaffolded catalog TU for kyoshin/cf/ICamControlClassic
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/cf/ICamControlClassic.hpp"
#include "kyoshin/cf/CfGameManager.hpp"

void func_80274EB0() {}
void func_80274EE4() {}
void func_80274F34() {}
void func_80274F94() {}
void func_80274FF4() {}
void func_80275028() {}
void func_8027505C() {}
void func_80275090() {}
void func_802750C4() {}
extern "C" {
// Returns the pad object for the main controller (index 0).
CPad* func_80274EA8() {
    return cf::CfGameManager::getPad(0);
}
}
