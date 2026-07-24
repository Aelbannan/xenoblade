#include "kyoshin/plugin/pluginMath.hpp"
#include "monolib/math.hpp"

extern "C" {
    /// Plugin name string (const data).
    extern const char lbl_eu_804FB050[];
    /// Plugin function table, terminated by {NULL, NULL}.
    extern PluginFuncData lbl_eu_805261B8[];
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
