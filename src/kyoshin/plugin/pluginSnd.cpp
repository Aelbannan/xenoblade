// Auto-scaffolded catalog TU for kyoshin/plugin/pluginSnd
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

// ---------------------------------------------------------------------------
// C-ABI sound-system imports (retail symbols are unmangled; no proper C++
// header declares these yet -- kept as extern "C" like the sibling plugin
// TUs, e.g. pluginHelp.cpp).
// ---------------------------------------------------------------------------
extern "C" {
    extern float lbl_eu_80667D90;   // SE volume scale (float)
    extern float lbl_eu_80667D94;   // BGM fade time (float)
    extern double lbl_eu_80667D98;  // signed int->float magic double (2^52 + 2^31)
    extern float lbl_eu_80667DA0;   // voice volume (float)
    extern char lbl_eu_80504148[];  // voice file-name format string

    extern s32 func_80189A04(s32 index);                        // sound-slot busy check
    extern s32 func_801897A0(const char* name, float vol, s32 flag);  // play archive voice
    extern "C" void func_801BFE8C(u32 a, u32 b, u32 c);        // CfSoundMan stop helper (matches CfObjectImplMove.hpp)
    extern void func_80189318(s32 clearName, float fadeTime);   // stop BGM slot (menu sound system)
    extern void func_8018986C(const char* name, float fadeTime); // stop voice by name
    extern void func_80188D34(const char* name, bool enable, float value, float fadeTime); // play BGM
    extern void func_8007C344__Q22cf13CfGameManagerFv(u32 first, u32 second, u8 enabled, float value); // field BGM state
    extern void func_8007C374__Q22cf13CfGameManagerFv(u32 first, u32 second, float value, u8 enabled); // town BGM state
}

// Entry returned by func_801BFAE4 (CfSoundMan slot lookup); only the +0x2A
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
    extern void func_801896A8(s32 index, float f1, float f2);              // master SE volume (menu sound system)
    extern void func_801AAC70(u32 id, u32 vol, float fade);                // map SE volume (tail-call stub)
    extern u16 func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(u32 r3, u32 r4, u32 r5, u32 r6, float f1); // play SE
    extern SoundSlotEntry* func_801BFAE4(u16 handle);                      // SE slot lookup (tail-call stub)
    extern void func_801AACBC(SndVec3* pos, SndVec3* target);              // set map camera position
}

extern u8 lbl_eu_8066443A;   // camera-position registered flag (.sbss)

// Resolve ml::FixStr<128>::format calls to the explicit specialization that
// CfScript.cpp defines (retail symbol format__Q22ml10FixStr<128>FPCce).  The
// generic-template call mangles to ...11FixStr... otherwise (reloc-name drift).
template <> void ml::FixStr<128>::format(const char* fmt, ...);

// Script command: play a BGM track.  Arg 1 is the file name, arg 2 a fixed-
// point volume (scaled by lbl_eu_80667D90), arg 3 an optional loop/clear
// flag that defaults to false when omitted.
int playBgm(VMThread* pThread) {
    const char* name = vmArgStringGet(2, vmArgPtrGet(pThread, 1));
    int fixed = vmArgFixedGet(3, vmArgPtrGet(pThread, 2));
    int enable;
    if (vmArgOmitChk(pThread, 3) != 0) {
        enable = 0;
    } else {
        enable = vmArgBoolGet(4, vmArgPtrGet(pThread, 3));
    }
    float value = (float)fixed / lbl_eu_80667D90;
    func_80188D34(name, enable != 0, value, lbl_eu_80667D94);
    return 0;
}

// Script command: stop the currently playing BGM.  Arg 1 is an optional
// fixed-point fade volume; when omitted a default volume constant is used.
int stopBgm(VMThread* pThread) {
    float vol;
    if (vmArgOmitChk(pThread, 1) != 0) {
        vol = lbl_eu_80667DA0;
    } else {
        float converted = vmArgFixedGet(2, vmArgPtrGet(pThread, 1));
        vol = converted;
    }
    func_80189318(1, (float)(int)vol / lbl_eu_80667D90);
    return 0;
}

// Script command: set the field BGM.  Args 1-2 are int state selectors
// (default 0), arg 3 a fixed-point volume (default lbl_eu_80667DA0),
// arg 4 an enable flag (default 0).
int setFieldBgm(VMThread* pThread) {
    int first;
    int second;
    int idx;
    int volInt;
    if (vmArgOmitChk(pThread, 1) != 0) {
        first = 0;
        idx = 2;
    } else {
        idx = 2;
        first = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    }
    if (vmArgOmitChk(pThread, idx) != 0) {
        second = 0;
        idx++;
    } else {
        VMArg* arg = vmArgPtrGet(pThread, idx++);
        second = vmArgIntGet(idx, arg);
    }
    float vol;
    if (vmArgOmitChk(pThread, idx) != 0) {
        vol = lbl_eu_80667DA0;
        idx++;
    } else {
        VMArg* arg = vmArgPtrGet(pThread, idx++);
        vol = (float)vmArgFixedGet(idx, arg);
    }
    volInt = (int)vol;
    int enable;
    if (vmArgOmitChk(pThread, idx) != 0) {
        enable = 0;
    } else {
        VMArg* arg = vmArgPtrGet(pThread, idx++);
        enable = vmArgBoolGet(idx, arg);
    }
    func_8007C344__Q22cf13CfGameManagerFv(first, second, enable != 0,
                                          (float)volInt / lbl_eu_80667D90);
    return 0;
}

// Script command: set the town BGM.  Same argument layout as setFieldBgm,
// routed to the town BGM state entry instead.
int setTownBgm(VMThread* pThread) {
    int first;
    int second;
    int idx;
    int volInt;
    if (vmArgOmitChk(pThread, 1) != 0) {
        first = 0;
        idx = 2;
    } else {
        VMArg* arg = vmArgPtrGet(pThread, 1);
        idx = 2;
        first = vmArgIntGet(2, arg);
    }
    if (vmArgOmitChk(pThread, idx) != 0) {
        second = 0;
        idx++;
    } else {
        VMArg* arg = vmArgPtrGet(pThread, idx++);
        second = vmArgIntGet(idx, arg);
    }
    float vol;
    if (vmArgOmitChk(pThread, idx) != 0) {
        vol = lbl_eu_80667DA0;
        idx++;
    } else {
        VMArg* arg = vmArgPtrGet(pThread, idx++);
        vol = (float)vmArgFixedGet(idx, arg);
    }
    volInt = (int)vol;
    int enable;
    if (vmArgOmitChk(pThread, idx) != 0) {
        enable = 0;
    } else {
        VMArg* arg = vmArgPtrGet(pThread, idx++);
        enable = vmArgBoolGet(idx, arg);
    }
    func_8007C374__Q22cf13CfGameManagerFv(first, second,
                                          (float)volInt / lbl_eu_80667D90,
                                          enable != 0);
    return 0;
}

// Script command: stop the field BGM (clears the field BGM state entry and
// stops the menu BGM slot).  Same optional fade argument as stopBgm.
int stopFieldBgm(VMThread* pThread) {
    float vol;
    if (vmArgOmitChk(pThread, 1) != 0) {
        vol = lbl_eu_80667DA0;
    } else {
        float converted = vmArgFixedGet(2, vmArgPtrGet(pThread, 1));
        vol = converted;
    }
    float fade = (float)(int)vol / lbl_eu_80667D90;
    func_8007C344__Q22cf13CfGameManagerFv(0, 0, 1, fade);
    func_80189318(1, fade);
    return 0;
}

// Script command: stop the town BGM (clears the town BGM state entry and
// stops the menu BGM slot).  Same optional fade argument as stopBgm.
int stopTownBgm(VMThread* pThread) {
    float vol;
    if (vmArgOmitChk(pThread, 1) != 0) {
        vol = lbl_eu_80667DA0;
    } else {
        float converted = vmArgFixedGet(2, vmArgPtrGet(pThread, 1));
        vol = converted;
    }
    float fade = (float)(int)vol / lbl_eu_80667D90;
    func_8007C374__Q22cf13CfGameManagerFv(0, 0, fade, 1);
    func_80189318(1, fade);
    return 0;
}

// Script command: force the field BGM.  Calls CfGameManager::func_8007C140
// with a null this (the callee handles this == nullptr internally).
int forceFieldBgm(VMThread* pThread) {
    ((cf::CfGameManager*)nullptr)->func_8007C140();
    return 0;
}

// Script command: set the master SE volume.  Arg 1 is a fixed-point volume
// (scaled by lbl_eu_80667D90), arg 2 an optional fixed-point fade time that
// defaults to lbl_eu_80667DA0 when omitted.
int setVolBgm(VMThread* pThread) {
    float fade;
    int vol = vmArgFixedGet(2, vmArgPtrGet(pThread, 1));
    if (vmArgOmitChk(pThread, 2) != 0) {
        fade = lbl_eu_80667DA0;
    } else {
        float converted = vmArgFixedGet(3, vmArgPtrGet(pThread, 2));
        fade = converted;
    }
    int fadeInt = fade;
    func_801896A8(0, (float)vol / lbl_eu_80667D90,
                  (float)fadeInt / lbl_eu_80667D90);
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
    func_801897A0(buf.mString, lbl_eu_80667DA0, 1);
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
        func_8018986C(buf.mString, lbl_eu_80667DA0);
    } else {
        func_8018986C(nullptr, lbl_eu_80667DA0);
    }
    return 0;
}

// Script command: wait until the currently playing voice has stopped.
int waitVoice(VMThread* pThread) {
    if (func_80189A04(0) == 0) {
        vmWaitModeSet(pThread);
    }
    return 0;
}

// Script command: play a common SE.  Arg 1 is the sound id, arg 2 an
// optional fixed-point fade time (default lbl_eu_80667D94), arg 3 an
// optional fixed-point volume (default lbl_eu_80667DA0).  Skipped while the
// presentation/event bit 0x100 in lbl_eu_80663E24 is set.
int playSeCommon(VMThread* pThread) {
    int idx;
    int id = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    float fade;
    if (vmArgOmitChk(pThread, 2) != 0) {
        idx = 3;
        fade = lbl_eu_80667D94;
    } else {
        idx = 3;
        fade = (float)vmArgFixedGet(3, vmArgPtrGet(pThread, 2));
    }
    int fadeInt = (int)fade;
    float vol;
    if (vmArgOmitChk(pThread, idx) != 0) {
        vol = lbl_eu_80667DA0;
    } else {
        VMArg* arg = vmArgPtrGet(pThread, idx++);
        vol = (float)vmArgFixedGet(idx, arg);
    }
    int volInt = (int)vol;
    if ((lbl_eu_80663E24 & 0x100) == 0) {
        u16 handle = func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(
            0, id, (int)((float)volInt / lbl_eu_80667D90), 1,
            (float)fadeInt / lbl_eu_80667D90);
        SoundSlotEntry* entry = func_801BFAE4(handle);
        if (entry != nullptr) {
            entry->field_0x2A |= 0x20;
        }
    }
    return 0;
}

// Script command: play a map SE.  Arg 1 is the sound id, arg 2 an optional
// fixed-point fade time (default lbl_eu_80667D94), arg 3 an optional
// fixed-point volume (default lbl_eu_80667DA0).  Skipped while the
// presentation/event bit 0x100 in lbl_eu_80663E24 is set.  Same layout as
// playSeCommon but routed to the map-SE channel (first call arg 1).
int playSeMap(VMThread* pThread) {
    int argPos = 2;
    int id = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    float fade;
    if (vmArgOmitChk(pThread, argPos) != 0) {
        argPos++;
        fade = lbl_eu_80667D94;
    } else {
        VMArg* arg = vmArgPtrGet(pThread, argPos++);
        fade = (float)vmArgFixedGet(argPos, arg);
    }
    int fadeInt = (int)fade;
    float vol;
    if (vmArgOmitChk(pThread, argPos) != 0) {
        vol = lbl_eu_80667DA0;
    } else {
        VMArg* arg = vmArgPtrGet(pThread, argPos++);
        vol = (float)vmArgFixedGet(argPos, arg);
    }
    int volInt = (int)vol;
    if ((lbl_eu_80663E24 & 0x100) == 0) {
        // fadeF must be computed before volScaled: MWCC allocates the
        // int->float conversion slots in this order (fade -> 0x8/0xc,
        // vol -> 0x10/0x14), matching the retail schedule.
        float fadeF = (float)fadeInt / lbl_eu_80667D90;
        int volScaled = (int)((float)volInt / lbl_eu_80667D90);
        u16 handle = func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(
            1, id, volScaled, 1, fadeF);
        SoundSlotEntry* entry = func_801BFAE4(handle);
        if (entry != nullptr) {
            entry->field_0x2A |= 0x20;
        }
    }
    return 0;
}

// Script command: set a map SE slot's volume.  Arg 1 is the slot id, arg 2
// an optional fixed-point fade time (default lbl_eu_80667D94), arg 3 the
// new volume (default 0 when omitted).
int volSeMap(VMThread* pThread) {
    int id;
    int idx;
    int vol;
    float fade;
    id = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    if (vmArgOmitChk(pThread, 2) != 0) {
        fade = lbl_eu_80667D94;
        idx = 3;
    } else {
        idx = 3;
        VMArg* arg = vmArgPtrGet(pThread, 2);
        fade = (float)vmArgFixedGet(idx, arg);
    }
    int fadeInt = (int)fade;
    if (vmArgOmitChk(pThread, idx) == 0) {
        VMArg* arg = vmArgPtrGet(pThread, idx++);
        vol = vmArgIntGet(idx, arg);
    } else {
        vol = 0;
    }
    func_801AAC70(id, vol, (float)fadeInt / lbl_eu_80667D90);
    return 0;
}

// Script command: stop a common SE slot.  Arg 2 is a fixed-point volume that
// is scaled by lbl_eu_80667D90.  The int->float conversion is written
// manually (MWCC_CASES 7i) so the magic double reloc targets the retail
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

// Script command: register the map camera position.  Args 1-3 are the
// fixed-point position components (scaled by lbl_eu_80667D90); args 4-6 are
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
        (float)x / lbl_eu_80667D90,
        (float)y / lbl_eu_80667D90,
        (float)z / lbl_eu_80667D90,
    };
    SndVec3 target = {
        (float)x / lbl_eu_80667D90,
        (float)y / lbl_eu_80667D90,
        (float)z / lbl_eu_80667D90,
    };
    func_801AACBC(&pos, &target);
    lbl_eu_8066443A = 1;
    return 0;
}

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
