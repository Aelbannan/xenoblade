#include "kyoshin/plugin/pluginDeb.hpp"

extern "C" {
    extern char lbl_eu_804FA434[];
    extern PluginFuncData lbl_eu_80524B98[];
}

//This was probably used to print debug messages, but is stubbed in the release version.
int deb_put(VMThread* pThread){
    return 0;
}

//These two functions would've printed out performance info?
int deb_performanceStart(VMThread* pThread) {
    vmArgStringGet(2, vmArgPtrGet(pThread, 1));
    return 0;
}

int deb_performanceEnd(VMThread* pThread) {
    vmArgStringGet(2, vmArgPtrGet(pThread, 1));
    return 0;
}

void pluginDebRegist(){
    vmPluginRegist(lbl_eu_804FA434, lbl_eu_80524B98);
}
