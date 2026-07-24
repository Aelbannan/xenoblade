#include "kyoshin/plugin/pluginGame.hpp"

extern "C" {
    extern char lbl_eu_80504168[];
    extern PluginFuncData lbl_eu_80533490[];
}

// Stubbed. This might've handled debug commands? (the ones listed in the bdat)
int game_cmd(_sVMThread* pThread) {
    return 0;
}

void pluginGameRegist() {
    vmPluginRegist(lbl_eu_80504168, lbl_eu_80533490);
}
