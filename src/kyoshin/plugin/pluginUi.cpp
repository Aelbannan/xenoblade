#include "kyoshin/plugin/pluginUi.hpp"
#include "monolib/util.hpp"
#include "monolib/vm/yvm2.h"

using namespace ml;

void ui_mesGetArts(VMThread* pThread) {
    int num1 = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    int num2 = vmArgIntGet(3, vmArgPtrGet(pThread, 2));
}

extern "C" bool func_800459FC(const unsigned int* self, unsigned int mask) { return (self[2] & mask) != 0; }
extern "C" void pluginUiRegist() {
    extern void vmPluginRegist(void*, void*);
    extern char lbl_eu_804FABF0[];
    extern char lbl_eu_80525D68[];
    vmPluginRegist((void*)((char*)lbl_eu_804FABF0 + 0x28), (void*)lbl_eu_80525D68);
}

extern "C" void winTalk() {}
extern "C" void pcTalk() {}
extern "C" void winTalkWait() {}
extern "C" void winTalkNoName() {}
extern "C" void fadeIn_1() {}
extern "C" void fadeOut_1() {}
extern "C" void fadeWait_1() {}
extern "C" int createCol6Sys(VMThread* pThread) {
    extern void func_8013DD94();
    func_8013DD94();
    return 0;
}
extern "C" int createCol6Hint(VMThread* pThread) {
    extern void func_8013DE6C();
    func_8013DE6C();
    return 0;
}
extern "C" void createCol6Invite() {}
extern "C" int createCol6Init(VMThread* pThread) {
    extern void func_80139CEC();
    func_80139CEC();
    return 0;
}
extern "C" int checkCol6Bat(VMThread* pThread) {
    extern void func_8013E030();
    func_8013E030();
    return 0;
}
extern "C" int simpleEventStart(VMThread* pThread) {
    extern void func_8013BD9C();
    func_8013BD9C();
    return 0;
}
extern "C" int simpleEventEnd(VMThread* pThread) {
    extern void func_8013BDA8();
    func_8013BDA8();
    return 0;
}
extern "C" void setTrust() {}
extern "C" void setItemMulti() {}
extern "C" void setKizunaTalk() {}
extern "C" void winSys() {}
extern "C" void winSysSelect() {}
extern "C" void getSelectNum() {}
extern "C" void mesAddPT() {}
extern "C" void mesSubPT() {}
extern "C" void mesVisionON() {}
extern "C" void mesVisionOFF() {}
extern "C" void mesMonadoON() {}
extern "C" void mesMonadoOFF() {}
extern "C" int ptChangeNotice() {
    extern void enablePadFlags__Q22cf13CfGameManagerFUlb(int, int);
    extern void func_8013E8E0();
    enablePadFlags__Q22cf13CfGameManagerFUlb(-1, 1);
    func_8013E8E0();
    return 0;
}
extern "C" int save() {
    extern u32 lbl_eu_80663E28;
    extern void func_8013E9D8();
    if (!(lbl_eu_80663E28 & 0x40000000)) func_8013E9D8();
    return 0;
}
extern "C" int kizunaTalkStart() {
    extern void func_8013BDBC();
    extern u32 lbl_eu_80663E28;
    extern void func_8007D7A4__Q22cf13CfGameManagerFv();
    func_8013BDBC();
    lbl_eu_80663E28 |= 0x1000;
    func_8007D7A4__Q22cf13CfGameManagerFv();
    return 0;
}
extern "C" int kizunaTalkEnd() {
    extern u32 lbl_eu_80663E28;
    extern void func_8013BDD0();
    lbl_eu_80663E28 &= ~0x1800;
    func_8013BDD0();
    return 0;
}
extern "C" void isPrioReq() {}
extern "C" int gameClear(VMThread* pThread) {
    extern void func_8013500C();
    func_8013500C();
    return 0;
}
extern "C" void setLastTalkNpc() {}
extern "C" void isSETalkVoiceWait() {}
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
