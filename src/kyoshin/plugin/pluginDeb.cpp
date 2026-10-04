#include "kyoshin/plugin/pluginDeb.hpp"

// Retail data sections (all defined by this TU):
// .rodata entry names "performanceStart"/"performanceEnd" + plugin name
// "deb" + .sdata2 entry name "put" + .data PluginFuncData table
// {name, func} terminated by {NULL, NULL}. The rodata strings are UNSIZED
// (CMCEffStart recipe): MWCC packs unsized string pieces at 4-byte
// boundaries, so 17/15/4-byte strings land at 0x00/0x14/0x24 with retail
// zero padding and the section totals 0x28. (Sized .rodata string arrays
// over-align the following piece to 8 and break the layout.)
__declspec(section ".rodata") __attribute__((used))
char lbl_eu_804FA410[] = "performanceStart";
__declspec(section ".rodata") __attribute__((used))
char lbl_eu_804FA424[] = "performanceEnd";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_804FA434[] = "deb";
__declspec(section ".sdata2") __attribute__((used)) char lbl_eu_80665B80[0x8] = "put";
__declspec(section ".data") __attribute__((used)) PluginFuncData lbl_eu_80524B98[4] = {
    { lbl_eu_80665B80, deb_put },
    { lbl_eu_804FA410, deb_performanceStart },
    { lbl_eu_804FA424, deb_performanceEnd },
    { 0, 0 },
};

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
