#include "kyoshin/plugin/pluginGame.hpp"

// name-guess: retail .data:0x80533490 is an anonymous PluginFuncData array
// {{"cmd", game_cmd}, {0, 0}}; no name exists for it in any region's symbols.txt.
static PluginFuncData sGameFuncData[2] = {
    { "cmd", game_cmd },
    { 0, 0 },
};

// Stubbed. This might've handled debug commands? (the ones listed in the bdat)
int game_cmd(_sVMThread* pThread) {
    return 0;
}

void pluginGameRegist() {
    vmPluginRegist("game", sGameFuncData);
}
