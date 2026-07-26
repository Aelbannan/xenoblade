// Auto-scaffolded catalog TU for kyoshin/cf/CfScript
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/CfScript.hpp"

extern "C" void func_80068A20() {
    extern void func_80068A30(void*, unsigned long);
    extern char lbl_eu_805708D0[];
    extern unsigned long lbl_eu_80661AC0;
    func_80068A30((void*)lbl_eu_805708D0, lbl_eu_80661AC0);
}

extern "C" void func_80068A30() {}

extern "C" void func_80068A80() {
    cf::CfScriptManager::getInstance()->init();
}

extern "C" void func_80068AA4() {}

extern "C" void func_80068AC8() {}

extern "C" void func_80068AEC() {}

extern "C" void func_80068B20__Q22cf15CfScriptManagerFv(void) {}

extern "C" void func_80068B24() {}

extern "C" void func_80068ECC(void* subObj, const char* name);

// Thunk: forward to func_80068ECC with &manager->mScripts[1] and the script name.
extern "C" void func_80068B58(cf::CfScriptManager* manager, const char* name) {
    func_80068ECC(&manager->mScripts[1], name);
}

extern "C" void func_80068B60() {}

// Thunk: forward to func_80068ECC with &manager->mScripts[2] and the script name.
extern "C" void func_80068B94(cf::CfScriptManager* manager, const char* name) {
    func_80068ECC(&manager->mScripts[2], name);
}

extern "C" void func_80068B9C() {}

extern "C" void func_80068BC0(void* self) {
    *(unsigned long*)((char*)self + 0x4c) |= 4;
}

extern "C" void func_80068BD0() {}

extern "C" void func_80068BF4(void* self) {
    *(unsigned long*)((char*)self + 0xa4) |= 4;
}

extern "C" void func_80068C04() {}

extern "C" void func_80068C28(void* self) {
    *(unsigned long*)((char*)self + 0xfc) |= 4;
}

extern "C" void func_80068C38() {}

extern "C" void func_80068C5C() {}

extern "C" void func_80068C7C() {}

extern "C" void func_80068CA0() {}

extern "C" void func_80068D14() {}

extern "C" void func_80068D38() {}

extern "C" void func_80068DAC() {}

extern "C" void func_80068DD0() {}

extern "C" void func_80068E44() {}

extern "C" void func_80068E7C() {}

extern "C" void func_80068E9C() {}


extern "C" void waitLoad__Q22cf8CfScriptFv() {}

extern "C" void update__Q22cf8CfScriptFv(void* self) {
    *(unsigned long*)((char*)self + 0x4c) |= 0x20;
}

extern "C" void OnFileEvent__8CfScriptFP10CEventFile() {}

extern "C" void getInstance__Q22cf15CfScriptManagerFv() {}

extern "C" void __ct__cf_CfScript() {}

extern "C" void __dt__Q22cf8CfScriptFv() {}

extern "C" void init__Q22cf15CfScriptManagerFv() {}

extern "C" void func_800694B0__Q22cf15CfScriptManagerFv() {}

extern "C" void func_8006953C__Q22cf15CfScriptManagerFv() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
namespace ml {
template <int N> class FixStr { public: void format(const char*, ...); };
template <> void FixStr<128>::format(const char*, ...) {}
}
extern "C" void sinit_800696C8() {}
