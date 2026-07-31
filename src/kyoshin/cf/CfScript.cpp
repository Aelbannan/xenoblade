// Auto-scaffolded catalog TU for kyoshin/cf/CfScript
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/CfScript.hpp"

void func_80068A20() {
    extern void func_80068A30(void*, unsigned long);
    extern char lbl_eu_805708D0[];
    extern unsigned long lbl_eu_80661AC0;
    func_80068A30((void*)lbl_eu_805708D0, lbl_eu_80661AC0);
}

void func_80068A30(){}

void func_80068A80() {
    cf::CfScriptManager::getInstance()->init();
}

void func_80068AA4(){}

void func_80068AC8(){}

void func_80068AEC(){}

void cf::CfScriptManager::func_80068B20(void) {}

extern "C" void func_80068B58(cf::CfScriptManager* manager, const char* name);
extern "C" void func_80068B24(const char* name) {
    func_80068B58(cf::CfScriptManager::getInstance(), name);
}

void func_80068ECC(void* subObj, const char* name);
void cf::CfScriptManager::func_80068B58(const char* name) {
    func_80068ECC(&mScripts[1], name);
}

void func_80068B60(){}

void cf::CfScriptManager::func_80068B94(const char* name) {
    func_80068ECC(&mScripts[2], name);
}

void func_80068B9C(){}

void cf::CfScriptManager::func_80068BC0() {
    mScripts[0].mFlags |= 4;
}

extern "C" void func_80068BF4(cf::CfScriptManager* manager);
extern "C" void func_80068BD0() {
    func_80068BF4(cf::CfScriptManager::getInstance());
}

void cf::CfScriptManager::func_80068BF4() {
    mScripts[1].mFlags |= 4;
}

void func_80068C04(){}

void cf::CfScriptManager::func_80068C28() {
    mScripts[2].mFlags |= 4;
}

void func_80068C38(){}

void func_80068C5C(){}

void func_80068C7C(){}

void func_80068CA0(){}

void func_80068D14(){}

void func_80068D38(){}

void func_80068DAC(){}

void func_80068DD0(){}

void func_80068E44(){}

void func_80068E7C(){}

void func_80068E9C(){}

void cf::CfScript::waitLoad() {}

void cf::CfScript::update() {
    mFlags |= 0x20;
}

void cf::CfScript::OnFileEvent() {}

cf::CfScriptManager* cf::CfScriptManager::getInstance() { return nullptr; }

void __ct__cf_CfScript(){}

cf::CfScript::~CfScript() {}

void cf::CfScriptManager::init() {}

void cf::CfScriptManager::func_800694B0() {}

void cf::CfScriptManager::func_8006953C() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
namespace ml {
template <int N> class FixStr { public: void format(const char*, ...); };
template <> void FixStr<128>::format(const char*, ...) {}
}
void sinit_800696C8(){}
