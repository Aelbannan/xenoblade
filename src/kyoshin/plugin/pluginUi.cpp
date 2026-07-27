#include "kyoshin/plugin/pluginUi.hpp"
#include "monolib/util.hpp"
#include "monolib/vm/yvm2.h"

using namespace ml;

void ui_mesGetArts(VMThread* pThread) {
    int num1 = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    int num2 = vmArgIntGet(3, vmArgPtrGet(pThread, 2));
}

bool func_800459FC(const unsigned int* self, unsigned int mask) { return (self[2] & mask) != 0; }
void pluginUiRegist() {
    extern void vmPluginRegist(void*, void*);
    extern char lbl_eu_804FABF0[];
    extern char lbl_eu_80525D68[];
    vmPluginRegist((void*)((char*)lbl_eu_804FABF0 + 0x28), (void*)lbl_eu_80525D68);
}

void winTalk(){}
void pcTalk(){}
void winTalkWait(){}
void winTalkNoName(){}
void fadeIn_1(){}
void fadeOut_1(){}
void fadeWait_1(){}
int createCol6Sys(VMThread* pThread) {
    extern void func_8013DD94();
    func_8013DD94();
    return 0;
}
int createCol6Hint(VMThread* pThread) {
    extern void func_8013DE6C();
    func_8013DE6C();
    return 0;
}
void createCol6Invite(){}
int createCol6Init(VMThread* pThread) {
    extern void func_80139CEC();
    func_80139CEC();
    return 0;
}
int checkCol6Bat(VMThread* pThread) {
    extern void func_8013E030();
    func_8013E030();
    return 0;
}
int simpleEventStart(VMThread* pThread) {
    extern void func_8013BD9C();
    func_8013BD9C();
    return 0;
}
int simpleEventEnd(VMThread* pThread) {
    extern void func_8013BDA8();
    func_8013BDA8();
    return 0;
}
void setTrust(){}
void setItemMulti(){}
void setKizunaTalk(){}
void winSys(){}
void winSysSelect(){}
void getSelectNum(){}
void mesAddPT(){}
void mesSubPT(){}
void mesVisionON(){}
void mesVisionOFF(){}
void mesMonadoON(){}
void mesMonadoOFF(){}
int ptChangeNotice(){
    extern void enablePadFlags__Q22cf13CfGameManagerFUlb(int, int);
    extern void func_8013E8E0();
    enablePadFlags__Q22cf13CfGameManagerFUlb(-1, 1);
    func_8013E8E0();
    return 0;
}
int save() {
    extern u32 lbl_eu_80663E28;
    extern void func_8013E9D8();
    if (!(lbl_eu_80663E28 & 0x40000000)) func_8013E9D8();
    return 0;
}
int kizunaTalkStart() {
    extern void func_8013BDBC();
    extern u32 lbl_eu_80663E28;
    extern void func_8007D7A4__Q22cf13CfGameManagerFv();
    func_8013BDBC();
    lbl_eu_80663E28 |= 0x1000;
    func_8007D7A4__Q22cf13CfGameManagerFv();
    return 0;
}
int kizunaTalkEnd(){
    extern u32 lbl_eu_80663E28;
    extern void func_8013BDD0();
    lbl_eu_80663E28 &= ~0x1800;
    func_8013BDD0();
    return 0;
}
void isPrioReq(){}
int gameClear(VMThread* pThread) {
    extern void func_8013500C();
    func_8013500C();
    return 0;
}
void setLastTalkNpc(){}
void isSETalkVoiceWait(){}
extern "C" int func_eu_80046DA0(VMThread* pThread) {
    extern void func_eu_8013C8DC();
    func_eu_8013C8DC();
    return 0;
}
extern "C" int func_eu_80046DC4(VMThread* pThread) {
    extern void func_eu_8013C8E8();
    func_eu_8013C8E8();
    return 0;
}
