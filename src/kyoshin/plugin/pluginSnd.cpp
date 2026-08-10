// Auto-scaffolded catalog TU for kyoshin/plugin/pluginSnd
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

// ---------------------------------------------------------------------------
// C-ABI sound-system imports (retail symbols are unmangled; no proper C++
// header declares these yet -- kept as extern "C" like the sibling plugin
// TUs, e.g. pluginHelp.cpp).
// ---------------------------------------------------------------------------
extern "C" {
    extern float lbl_eu_80667D90;   // SE volume scale (float)
    extern double lbl_eu_80667D98;  // signed int->float magic double (2^52 + 2^31)
    extern float lbl_eu_80667DA0;   // voice volume (float)
    extern char lbl_eu_80504148[];  // voice file-name format string

    extern s32 func_80189A04(s32 index);                        // sound-slot busy check
    extern s32 func_801897A0(const char* name, float vol, s32 flag);  // play archive voice
    extern void func_801BFE8C(int a, int b, int c);             // CfSoundMan stop helper
}

void playBgm(){}

void stopBgm(){}

void setFieldBgm(){}

void setTownBgm(){}

void stopFieldBgm(){}

void stopTownBgm(){}

// Script command: force the field BGM.  Calls CfGameManager::func_8007C140
// with a null this (the callee handles this == nullptr internally).
int forceFieldBgm(VMThread* pThread) {
    ((cf::CfGameManager*)nullptr)->func_8007C140();
    return 0;
}

void setVolBgm(){}

// Script command: play a character voice.  Arg 1 is the voice name; when
// present it is truncated to 10 chars and recombined via the retail format
// string before playback.
int playVoice(VMThread* pThread) {
    const char* str = vmArgStringGet(2, vmArgPtrGet(pThread, 1));
    ml::FixStr<128> buf;
    if (str != nullptr) {
        char name[128];
        strncpy(name, str, 10);
        name[10] = 0;
        buf.format(lbl_eu_80504148, name, str);
    } else {
        buf.clear();
    }
    func_801897A0(buf.mString, lbl_eu_80667DA0, 1);
    return 0;
}

void stopVoice(){}

// Script command: wait until the currently playing voice has stopped.
int waitVoice(VMThread* pThread) {
    if (func_80189A04(0) == 0) {
        vmWaitModeSet(pThread);
    }
    return 0;
}

void playSeCommon(){}

void playSeMap(){}

void volSeMap(){}

// Script command: stop a common SE slot.  Arg 2 is a fixed-point volume that
// is scaled by lbl_eu_80667D90.  The int->float conversion is written
// manually (MWCC_REFERENCE 7i) so the magic double reloc targets the retail
// lbl_eu_80667D98 instead of a TU-local @N pool constant.
int stopSeCommon(VMThread* pThread) {
    int id = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    int vol = vmArgFixedGet(3, vmArgPtrGet(pThread, 2));
    float f = (float)(s32)vol;
    func_801BFE8C(0, id, (int)(f / lbl_eu_80667D90));
    return 0;
}

// Script command: stop a map SE slot (same as stopSeCommon but channel 1).
int stopSeMap(VMThread* pThread) {
    int id = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    int vol = vmArgFixedGet(3, vmArgPtrGet(pThread, 2));
    float f = (float)(s32)vol;
    func_801BFE8C(1, id, (int)(f / lbl_eu_80667D90));
    return 0;
}

void seCamPos(){}

extern "C" int clearCamPos() {
    extern unsigned char lbl_eu_8066443A;
    extern void func_801AAD08();
    func_801AAD08();
    lbl_eu_8066443A = 0;
    return 0;
}

extern "C" void pluginSndRegist() {
    extern void vmPluginRegist(void*, void*);
    extern char lbl_eu_80504148[];
    extern char lbl_eu_805333F8[];
    vmPluginRegist((void*)((char*)lbl_eu_80504148 + 0x15), (void*)lbl_eu_805333F8);
}
