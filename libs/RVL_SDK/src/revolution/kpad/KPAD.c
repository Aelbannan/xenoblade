// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/kpad/KPAD
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void reset_kpad() {}

void KPADEnableAimingMode(int channel) {
    extern unsigned char inside_kpads[];
    unsigned char *pad = inside_kpads + channel * 0x578;
    pad[0x55c] = 1;
    pad[0x55d] = 1;
}

void calc_button_repeat() {}

void read_kpad_button() {}

void calc_acc() {}

void calc_acc_horizon() {}

void calc_acc_vertical() {}

void read_kpad_acc() {}

void select_2obj_first() {}

void select_2obj_continue() {}

void select_1obj_first() {}

void select_1obj_continue() {}

void calc_dpd_variable() {}

void read_kpad_dpd() {}

void clamp_stick_circle() {}

void clamp_stick_cross() {}

void read_kpad_ext() {}

void KPADReadEx(void) {}

void KPADiRead() {}

void KPADInitEx() {}

void KPADReset() {}

void KPADiConnectCallback() {}

void* KPADSetConnectCallback(int chan, void* callback) {
    extern unsigned char inside_kpads[];
    unsigned int enabled;
    unsigned char* base;
    void* old;

    enabled = OSDisableInterrupts();
    base = inside_kpads + chan * 0x578;
    old = *(void**)(base + 0x550);
    *(void**)(base + 0x550) = callback;
    OSRestoreInterrupts(enabled);
    return old;
}

void KPADiControlWbcCallback() {}

void KPADiUpdateTempWbcCallback() {}

void KPADDisableDPD() {}

void KPADEnableDPD(int channel) {
    extern unsigned char inside_kpads[];
    inside_kpads[channel * 0x578 + 0x556] = 1;
}

void KPADiControlDpdCallback() {}

void KPADiSamplingCallback() {}
