#pragma once

// pluginUi -- UI-related plugin commands for the VM scripting system.
// All functions have C linkage because they are called by name from function
// pointer tables registered with the VM plugin system.

#include <types.h>

extern "C" {

bool func_800459FC(const unsigned int* self, unsigned int mask);
void pluginUiRegist();

void winTalk();
void pcTalk();
void winTalkWait();
void winTalkNoName();
void fadeIn_1();
void fadeOut_1();
void fadeWait_1();
void createCol6Sys();
void createCol6Hint();
void createCol6Invite();
void createCol6Init();
void checkCol6Bat();
void simpleEventStart();
void simpleEventEnd();
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
void ptChangeNotice();
void save();
void kizunaTalkStart();
void kizunaTalkEnd();
void isPrioReq();
void gameClear();
void setLastTalkNpc();
void isSETalkVoiceWait();
void func_eu_80046DA0();
void func_eu_80046DC4();

}
