#include "kyoshin/plugin/pluginDeb.hpp"

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

// name-guess: retail .data object 0x80524B98 is the VM function table for
// the deb plugin.
static PluginFuncData sDebPluginFuncs[] = {
    { "put", deb_put },
    { "performanceStart", deb_performanceStart },
    { "performanceEnd", deb_performanceEnd },
    { nullptr, nullptr },
};

void pluginDebRegist(){
    vmPluginRegist("deb", sDebPluginFuncs);
}
