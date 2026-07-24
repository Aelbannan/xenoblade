#include "kyoshin/plugin/plugins.hpp"
#include "kyoshin/plugin/pluginMain.hpp"

// Registers all SB script plugin modules and OC (object command) handlers
// into the VM. Each plugin provides script-callable function modules for
// a specific subsystem (game, debug, battle, units, UI, events, camera,
// math, input, timing, audio, voice, help) and OC handlers provide core
// object-command operations (builtins, bdat data access, threads, messages,
// units, cfp).
void pluginRegist() {
    pluginGameRegist();
    pluginDebRegist();
    pluginBtlRegist();
    pluginUnitRegist();
    pluginWaitRegist();
    pluginUiRegist();
    pluginEveRegist();
    pluginCfsRegist();
    pluginCamRegist();
    pluginMathRegist();
    pluginPadRegist();
    pluginTimeRegist();
    pluginSndRegist();
    pluginVoiceRegist();
    pluginHelpRegist();
    ocBuiltinRegist();
    ocBdatRegist();
    ocThreadRegist();
    ocMsgRegist();
    ocUnitRegist();
    ocCfpRegist();
}
