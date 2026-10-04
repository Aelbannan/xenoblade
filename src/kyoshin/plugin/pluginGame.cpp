#include "kyoshin/plugin/pluginGame.hpp"

extern "C" {
    // Retail data sections (all defined by this TU):
    // .rodata "game" (plugin name) + .sdata2 "cmd" (entry name) +
    // .data PluginFuncData table {name, func} terminated by {NULL, NULL}.
    __declspec(section ".rodata") __attribute__((used)) char lbl_eu_80504168[0x8] = "game";
    __declspec(section ".sdata2") __attribute__((used)) char lbl_eu_80667DB8[0x8] = "cmd";
    __declspec(section ".data") __attribute__((used)) PluginFuncData lbl_eu_80533490[2] = {
        { lbl_eu_80667DB8, game_cmd },
        { 0, 0 },
    };
}

// Stubbed. This might've handled debug commands? (the ones listed in the bdat)
int game_cmd(_sVMThread* pThread) {
    return 0;
}

void pluginGameRegist() {
    vmPluginRegist(lbl_eu_80504168, lbl_eu_80533490);
}
