#pragma once

// pluginUi -- UI-related plugin commands for the VM scripting system.
// All functions have C linkage because they are called by name from function
// pointer tables registered with the VM plugin system.

#include <types.h>
#include "monolib/vm/yvm2.h"

// Flag context block passed to some plugin callbacks; flags at offset 8.
struct UiFlags {
    u32 _00; // 0x0
    u32 _04; // 0x4
    u32 flags; // 0x8

    bool func_800459FC(u32 mask) const;
};

extern "C" {

void pluginUiRegist();

void winTalk();
void pcTalk();
void winTalkWait();
void winTalkNoName();
void fadeIn_1();
void fadeOut_1();
void fadeWait_1();
int createCol6Sys(VMThread* pThread);
int createCol6Hint(VMThread* pThread);
void createCol6Invite();
int createCol6Init(VMThread* pThread);
int checkCol6Bat(VMThread* pThread);
int simpleEventStart(VMThread* pThread);
int simpleEventEnd(VMThread* pThread);
void setTrust();
void setItemMulti();
void setKizunaTalk();
void winSys();
void winSysSelect();
void getSelectNum();
void mesAddPT();
void mesSubPT();
void mesVisionON();
void mesVisionOFF();
void mesMonadoON();
void mesMonadoOFF();
int ptChangeNotice();
int save();
int kizunaTalkStart();
int kizunaTalkEnd();
void isPrioReq();
int gameClear(VMThread* pThread);
void setLastTalkNpc();
void isSETalkVoiceWait();
int func_eu_80046DA0(VMThread* pThread);
int func_eu_80046DC4(VMThread* pThread);

}
