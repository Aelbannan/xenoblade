#include "kyoshin/plugin/pluginMath.hpp"
#include "monolib/math.hpp"

extern "C" {
    extern char lbl_eu_804FB050[];
    extern PluginFuncData lbl_eu_805261B8[];
}

int math_random(VMThread* pThread) {
    int min = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    int max = vmArgIntGet(3, vmArgPtrGet(pThread, 2));

    VMArg result;
    result.value.intVal = ml::math::mtRand(min, max);
    result.type = VM_TYPE_INT;
    vmRetValSet(pThread, &result);
    return 1;
}

void pluginMathRegist(){
    vmPluginRegist(lbl_eu_804FB050, lbl_eu_805261B8);
}
