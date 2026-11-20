// Auto-scaffolded catalog TU for kyoshin/plugin/pluginSnd
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/CfSoundMan.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (gmScene; gmGameFlags)
#include "kyoshin/menu/MenuSndApi.hpp"

// ---------------------------------------------------------------------------
// C-ABI sound-system imports (retail symbols are unmangled; no proper C++
// header declares these yet -- kept as extern "C" like the sibling plugin
// TUs, e.g. pluginHelp.cpp).
// ---------------------------------------------------------------------------
extern "C" {
    extern char lbl_eu_80504148[];  // voice file-name format string

    extern s32 MenuSnd_QuerySlotLive_89A04(s32 index);                        // sound-slot busy check
    extern s32 MenuSnd_TryPlayGated_97A0(const char* name, float vol, s32 flag);  // play archive voice
    extern void MenuSnd_SilenceByName_8986C(const char* name, float fadeTime); // stop voice by name
    extern void MenuSnd_RestartBGM_88D34(const char* name, bool enable, float value, float fadeTime); // play BGM
    extern void queueBdatTextA__Q22cf13CfGameManagerFv(u32 first, u32 second, bool enabled, float value); // field BGM state
    extern void func_8007C374__Q22cf13CfGameManagerFv(u32 first, u32 second, float value, u8 enabled); // town BGM state
}

// Entry returned by CfSoundMan_TouchSlotById (CfSoundMan slot lookup); only the +0x2A
// u16 flag word is touched by playSeCommon.
struct SoundSlotEntry {
    u8 field_0x00[0x2A];
    u16 field_0x2A;
};

// 3-component float vector used by seCamPos for the camera position pair.
struct SndVec3 {
    float x;
    float y;
    float z;
};

extern "C" {
    extern void BattleWork_ApplyVolumeStore(u32 id, u32 vol, float fade);                // map SE volume (tail-call stub)
    extern SoundSlotEntry* CfSoundMan_TouchSlotById(u16 handle);                      // SE slot lookup (tail-call stub)
    extern void BattleWork_CopyPairToWork(SndVec3* pos, SndVec3* target);              // set map camera position
}

extern u8 gSoundSystemInitFlag;   // camera-position registered flag (.sbss)

// Resolve ml::FixStr<128>::format calls to the explicit specialization that
// CfScript.cpp defines (retail symbol format__Q22ml10FixStr<128>FPCce).  The
// generic-template call mangles to ...11FixStr... otherwise (reloc-name drift).
template <> void ml::FixStr<128>::format(const char* fmt, ...);

// Script command: play a BGM track.  Arg 1 is the file name, arg 2 a fixed-
// point volume (scaled by 4096.0f), arg 3 an optional loop/clear
// flag that defaults to false when omitted.
// Retail defines every command with a plain (unmangled) symbol, so the whole
// command family has C linkage. Plain decomp names also let the reloc-map
// miner pair retail/decomp functions; MWCC emits the int->float magic from
// the ordinary casts in this TU's literal pool.
extern "C" {
int playBgm(VMThread* pThread) {
    const char* name = vmArgStringGet(2, vmArgPtrGet(pThread, 1));
    int fixed = vmArgFixedGet(3, vmArgPtrGet(pThread, 2));
    int enable;
    if (vmArgOmitChk(pThread, 3) != 0) {
        enable = 0;
    } else {
        enable = vmArgBoolGet(4, vmArgPtrGet(pThread, 3));
    }
    float value = (float)fixed / 4096.0f;
    MenuSnd_RestartBGM_88D34(name, enable != 0, value, 1.0f);
    return 0;
}

// Script command: stop the currently playing BGM.  Arg 1 is an optional
// fixed-point fade volume; when omitted a default volume constant is used.
int stopBgm(VMThread* pThread) {
    float vol;
    if (vmArgOmitChk(pThread, 1) != 0) {
        vol = 0.0f;
    } else {
        float converted = vmArgFixedGet(2, vmArgPtrGet(pThread, 1));
        vol = converted;
    }
    MenuSnd_StopSlot0Clear_9318(1, (float)(int)vol / 4096.0f);
    return 0;
}

// Script command: set the field BGM.  Args 1-2 are int state selectors
// (default 0), arg 3 a fixed-point volume (default 0.0f),
// arg 4 an enable flag (default 0).
int setFieldBgm(VMThread* pThread) {
    int first;
    int second;
    float vol;
    int idx = 1;

    if (vmArgOmitChk(pThread, 1)) {
        first = 0;
        idx = 2;
    } else {
        VMArg* arg = vmArgPtrGet(pThread, idx++);
        first = vmArgIntGet(idx, arg);
    }
    if (vmArgOmitChk(pThread, idx)) {
        second = 0;
        idx++;
    } else {
        VMArg* arg = vmArgPtrGet(pThread, idx++);
        second = vmArgIntGet(idx, arg);
    }
    if (vmArgOmitChk(pThread, idx)) {
        vol = 0.0f;
        idx++;
    } else {
        VMArg* arg = vmArgPtrGet(pThread, idx++);
        vol = (float)vmArgFixedGet(idx, arg);
    }
    int volInt = (int)vol;
    int enable;
    if (vmArgOmitChk(pThread, idx)) {
        enable = 0;
    } else {
        VMArg* arg = vmArgPtrGet(pThread, idx++);
        enable = vmArgBoolGet(idx, arg);
    }
    queueBdatTextA__Q22cf13CfGameManagerFv(first, second, enable != 0,
                                          (float)volInt / 4096.0f);
    return 0;
}

// Script command: set the town BGM.  Same argument layout as setFieldBgm,
// routed to the town BGM state entry instead.
int setTownBgm(VMThread* pThread) {
    int first;
    int second;
    float vol;
    int idx = 1;

    if (vmArgOmitChk(pThread, 1)) {
        first = 0;
        idx = 2;
    } else {
        VMArg* arg = vmArgPtrGet(pThread, idx++);
        first = vmArgIntGet(idx, arg);
    }
    if (vmArgOmitChk(pThread, idx)) {
        second = 0;
        idx++;
    } else {
        VMArg* arg = vmArgPtrGet(pThread, idx++);
        second = vmArgIntGet(idx, arg);
    }
    if (vmArgOmitChk(pThread, idx)) {
        vol = 0.0f;
        idx++;
    } else {
        VMArg* arg = vmArgPtrGet(pThread, idx++);
        vol = (float)vmArgFixedGet(idx, arg);
    }
    int volInt = (int)vol;
    int enable;
    if (vmArgOmitChk(pThread, idx)) {
        enable = 0;
    } else {
        VMArg* arg = vmArgPtrGet(pThread, idx++);
        enable = vmArgBoolGet(idx, arg);
    }
    func_8007C374__Q22cf13CfGameManagerFv(first, second,
                                          (float)volInt / 4096.0f,
                                          enable != 0);
    return 0;
}

// Script command: stop the field BGM (clears the field BGM state entry and
// stops the menu BGM slot).  Same optional fade argument as stopBgm.
int stopFieldBgm(VMThread* pThread) {
    float vol;
    if (vmArgOmitChk(pThread, 1) != 0) {
        vol = 0.0f;
    } else {
        float converted = vmArgFixedGet(2, vmArgPtrGet(pThread, 1));
        vol = converted;
    }
    float fade = (float)(int)vol / 4096.0f;
    queueBdatTextA__Q22cf13CfGameManagerFv(0, 0, 1, fade);
    MenuSnd_StopSlot0Clear_9318(1, fade);
    return 0;
}

// Script command: stop the town BGM (clears the town BGM state entry and
// stops the menu BGM slot).  Same optional fade argument as stopBgm.
int stopTownBgm(VMThread* pThread) {
    float vol;
    if (vmArgOmitChk(pThread, 1) != 0) {
        vol = 0.0f;
    } else {
        float converted = vmArgFixedGet(2, vmArgPtrGet(pThread, 1));
        vol = converted;
    }
    float fade = (float)(int)vol / 4096.0f;
    func_8007C374__Q22cf13CfGameManagerFv(0, 0, fade, 1);
    MenuSnd_StopSlot0Clear_9318(1, fade);
    return 0;
}

// Script command: force the field BGM.  Calls CfGameManager::resetGameFlags
// with a null this (the callee handles this == nullptr internally).
int forceFieldBgm(VMThread* pThread) {
    ((cf::CfGameManager*)nullptr)->resetGameFlags();
    return 0;
}

// Script command: set the master SE volume.  Arg 1 is a fixed-point volume
// (scaled by 4096.0f), arg 2 an optional fixed-point fade time that
// defaults to 0.0f when omitted.
int setVolBgm(VMThread* pThread) {
    float fade;
    int vol = vmArgFixedGet(2, vmArgPtrGet(pThread, 1));
    if (vmArgOmitChk(pThread, 2) != 0) {
        fade = 0.0f;
    } else {
        float converted = vmArgFixedGet(3, vmArgPtrGet(pThread, 2));
        fade = converted;
    }
    int fadeInt = fade;
    MenuSnd_SetBalancePush_96A8(0, (float)vol / 4096.0f,
                  (float)fadeInt / 4096.0f);
    return 0;
}

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
    MenuSnd_TryPlayGated_97A0(buf.mString, 0.0f, 1);
    return 0;
}

// Script command: stop a character voice.  Arg 1 is the voice name; when
// present it is truncated to 10 chars and recombined via the retail format
// string, otherwise a null name stops any active voice.
int stopVoice(VMThread* pThread) {
    const char* name;
    if (vmArgOmitChk(pThread, 1) != 0) {
        name = nullptr;
    } else {
        name = vmArgStringGet(2, vmArgPtrGet(pThread, 1));
    }
    if (name != nullptr) {
        ml::FixStr<128> buf;
        if (name != nullptr) {
            char shortName[128];
            strncpy(shortName, name, 10);
            shortName[10] = 0;
            buf.format(lbl_eu_80504148, shortName, name);
        } else {
            buf.clear();
        }
        MenuSnd_SilenceByName_8986C(buf.mString, 0.0f);
    } else {
        MenuSnd_SilenceByName_8986C(nullptr, 0.0f);
    }
    return 0;
}

// Script command: wait until the currently playing voice has stopped.
int waitVoice(VMThread* pThread) {
    if (MenuSnd_QuerySlotLive_89A04(0) == 0) {
        vmWaitModeSet(pThread);
    }
    return 0;
}

// Script command: play a common SE.  Arg 1 is the sound id, arg 2 an
// optional fixed-point fade time (default 1.0f), arg 3 an
// optional fixed-point volume (default 0.0f).  Skipped while the
// presentation/event bit 0x100 in gmGameFlags is set.
int playSeCommon(VMThread* pThread) {
    int argPos = 2;
    int id = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    float fade;
    if (vmArgOmitChk(pThread, argPos) != 0) {
        argPos++;
        fade = 1.0f;
    } else {
        VMArg* arg = vmArgPtrGet(pThread, argPos++);
        fade = (float)vmArgFixedGet(argPos, arg);
    }
    int fadeInt = (int)fade;
    float vol;
    if (vmArgOmitChk(pThread, argPos) != 0) {
        vol = 0.0f;
    } else {
        VMArg* arg = vmArgPtrGet(pThread, argPos++);
        vol = (float)vmArgFixedGet(argPos, arg);
    }
    int volInt = (int)vol;
    if ((gmGameFlags & 0x100) == 0) {
        float fadeF = (float)fadeInt / 4096.0f;
        int volScaled = (int)((float)volInt / 4096.0f);
        u16 handle = cf::CfSoundMan::playActorSound(0, id, volScaled, 1, fadeF);
        SoundSlotEntry* entry = CfSoundMan_TouchSlotById(handle);
        if (entry != nullptr) {
            entry->field_0x2A |= 0x20;
        }
    }
    return 0;
}

// Script command: play a map SE.  Arg 1 is the sound id, arg 2 an optional
// fixed-point fade time (default 1.0f), arg 3 an optional
// fixed-point volume (default 0.0f).  Skipped while the
// presentation/event bit 0x100 in gmGameFlags is set.  Same layout as
// playSeCommon but routed to the map-SE channel (first call arg 1).
int playSeMap(VMThread* pThread) {
    int argPos = 2;
    int id = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    float fade;
    if (vmArgOmitChk(pThread, argPos) != 0) {
        argPos++;
        fade = 1.0f;
    } else {
        VMArg* arg = vmArgPtrGet(pThread, argPos++);
        fade = (float)vmArgFixedGet(argPos, arg);
    }
    int fadeInt = (int)fade;
    float vol;
    if (vmArgOmitChk(pThread, argPos) != 0) {
        vol = 0.0f;
    } else {
        VMArg* arg = vmArgPtrGet(pThread, argPos++);
        vol = (float)vmArgFixedGet(argPos, arg);
    }
    int volInt = (int)vol;
    if ((gmGameFlags & 0x100) == 0) {
        // fadeF must be computed before volScaled: MWCC allocates the
        // int->float conversion slots in this order (fade -> 0x8/0xc,
        // vol -> 0x10/0x14), matching the retail schedule.
        float fadeF = (float)fadeInt / 4096.0f;
        int volScaled = (int)((float)volInt / 4096.0f);
        u16 handle = cf::CfSoundMan::playActorSound(1, id, volScaled, 1, fadeF);
        SoundSlotEntry* entry = CfSoundMan_TouchSlotById(handle);
        if (entry != nullptr) {
            entry->field_0x2A |= 0x20;
        }
    }
    return 0;
}

// Script command: set a map SE slot's volume.  Arg 1 is the slot id, arg 2
// an optional fixed-point fade time (default 1.0f), arg 3 the
// new volume (default 0 when omitted).
int volSeMap(VMThread* pThread) {
    int idx;
    int id;
    int vol;
    float fade;
    id = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    if (vmArgOmitChk(pThread, 2) != 0) {
        fade = 1.0f;
        idx = 3;
    } else {
        idx = 3;
        VMArg* arg = vmArgPtrGet(pThread, 2);
        fade = (float)vmArgFixedGet(idx, arg);
    }
    int fadeInt = (int)fade;
    if (vmArgOmitChk(pThread, idx) != 0) {
        vol = 0;
    } else {
        VMArg* arg = vmArgPtrGet(pThread, idx++);
        vol = vmArgIntGet(idx, arg);
    }
    float fadeF = (float)fadeInt / 4096.0f;
    BattleWork_ApplyVolumeStore(id, vol, fadeF);
    return 0;
}

// Script command: stop a common SE slot.
// is scaled by 4096.0f.  The int->float conversion comes from the plain
// (float)(s32) cast, with the conversion magic emitted in this TU's own
// literal pool.
int stopSeCommon(VMThread* pThread) {
    int id = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    int vol = vmArgFixedGet(3, vmArgPtrGet(pThread, 2));
    float f = (float)(s32)vol;
    CfSoundMan_StopSlotByMode(0, id, (int)(f / 4096.0f));
    return 0;
}

// Script command: stop a map SE slot (same as stopSeCommon but channel 1).
int stopSeMap(VMThread* pThread) {
    int id = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    int vol = vmArgFixedGet(3, vmArgPtrGet(pThread, 2));
    float f = (float)(s32)vol;
    CfSoundMan_StopSlotByMode(1, id, (int)(f / 4096.0f));
    return 0;
}

// Script command: register the map camera position.  Args 1-3 are the
// fixed-point position components (scaled by 4096.0f); args 4-6 are
// optional fixed-point values that are only validated (read and discarded).
// Both output vectors are built from args 1-3 and the registered flag is set.
int seCamPos(VMThread* pThread) {
    int x = vmArgFixedGet(2, vmArgPtrGet(pThread, 1));
    int y = vmArgFixedGet(3, vmArgPtrGet(pThread, 2));
    int z = vmArgFixedGet(4, vmArgPtrGet(pThread, 3));
    int argPos = 4;
    if (vmArgOmitChk(pThread, argPos) != 0) {
        argPos++;
    } else {
        VMArg* arg = vmArgPtrGet(pThread, argPos++);
        vmArgFixedGet(argPos, arg);
    }
    if (vmArgOmitChk(pThread, argPos) != 0) {
        argPos++;
    } else {
        VMArg* arg = vmArgPtrGet(pThread, argPos++);
        vmArgFixedGet(argPos, arg);
    }
    if (vmArgOmitChk(pThread, argPos) == 0) {
        VMArg* arg = vmArgPtrGet(pThread, argPos++);
        vmArgFixedGet(argPos, arg);
    }
    SndVec3 pos = {
        (float)x / 4096.0f,
        (float)y / 4096.0f,
        (float)z / 4096.0f,
    };
    SndVec3 target = {
        (float)x / 4096.0f,
        (float)y / 4096.0f,
        (float)z / 4096.0f,
    };
    BattleWork_CopyPairToWork(&pos, &target);
    gSoundSystemInitFlag = 1;
    return 0;
}

int clearCamPos() {
    extern unsigned char gSoundSystemInitFlag;
    extern void func_801AAD08();
    func_801AAD08();
    gSoundSystemInitFlag = 0;
    return 0;
}

void pluginSndRegist() {
    extern void vmPluginRegist(void*, void*);
    extern char lbl_eu_80504148[];
    extern char lbl_eu_805333F8[];
    vmPluginRegist((void*)((char*)lbl_eu_80504148 + 0x15), (void*)lbl_eu_805333F8);
}
} // extern "C"
