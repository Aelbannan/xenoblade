#include "kyoshin/plugin/pluginMath.hpp"
#include "monolib/math.hpp"

extern "C" {
    // Retail data sections (all defined by this TU):
    // .rodata "math" (plugin name) + .sdata2 "random" (entry name) +
    // trailing sdata2 float (retail 0x411CCCCD) + .data PluginFuncData
    // table {name, func} terminated by {NULL, NULL}. The float is not read
    // by this TU's code; it rides in this TU's sdata2 pool (retail layout)
    // and is referenced from another unit (see link frontier).
    __declspec(section ".rodata") __attribute__((used)) char lbl_eu_804FB050[0x8] = "math";
    __declspec(section ".sdata2") __attribute__((used)) char lbl_eu_80665E90[0x8] = "random";
    __declspec(section ".data") __attribute__((used)) PluginFuncData lbl_eu_805261B8[2] = {
        { lbl_eu_80665E90, math_random },
        { 0, 0 },
    };
}

/// Generates a random integer in [min, max] and returns it to the YGG script.
/// @param pThread  Current VM thread (provides argument access and return value storage).
/// @return Always 1 (YGG plugin convention for success).
int math_random(VMThread* pThread) {
    int min = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    int max = vmArgIntGet(3, vmArgPtrGet(pThread, 2));

    VMArg result;
    result.value.intVal = ml::math::mtRand(min, max);
    result.type = VM_TYPE_INT;
    vmRetValSet(pThread, &result);
    return 1;
}

/// Registers the math plugin with the YGG VM so scripts can call its functions.
void pluginMathRegist() {
    vmPluginRegist(lbl_eu_804FB050, lbl_eu_805261B8);
}
