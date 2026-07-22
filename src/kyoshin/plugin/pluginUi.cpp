#include "monolib/util.hpp"
#include "monolib/vm/yvm2.h"

using namespace ml;

void ui_mesGetArts(VMThread* pThread) {
    int num1 = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    int num2 = vmArgIntGet(3, vmArgPtrGet(pThread, 2));
}

// LLM-HARNESS-BEGIN: us-80045f9c
extern "C" bool func_800459FC(const unsigned int* self, unsigned int mask) { return (self[2] & mask) != 0; }
// LLM-HARNESS-END: us-80045f9c
// LLM-HARNESS-BEGIN: us-80046de8
extern "C" void pluginUiRegist() {}
// LLM-HARNESS-END: us-80046de8

// LLM-HARNESS-BEGIN: us-80045e24
extern "C" void winTalk() {}
// LLM-HARNESS-END: us-80045e24
// LLM-HARNESS-BEGIN: us-80045e98
extern "C" void pcTalk() {}
// LLM-HARNESS-END: us-80045e98
// LLM-HARNESS-BEGIN: us-80045f0c
extern "C" void winTalkWait() {}
// LLM-HARNESS-END: us-80045f0c
// LLM-HARNESS-BEGIN: us-80045fb4
extern "C" void winTalkNoName() {}
// LLM-HARNESS-END: us-80045fb4
// LLM-HARNESS-BEGIN: us-80045ff4
extern "C" void fadeIn_1() {}
// LLM-HARNESS-END: us-80045ff4
// LLM-HARNESS-BEGIN: us-800460a4
extern "C" void fadeOut_1() {}
// LLM-HARNESS-END: us-800460a4
// LLM-HARNESS-BEGIN: us-80046154
extern "C" void fadeWait_1() {}
// LLM-HARNESS-END: us-80046154
// LLM-HARNESS-BEGIN: us-800461a0
extern "C" void createCol6Sys() {}
// LLM-HARNESS-END: us-800461a0
// LLM-HARNESS-BEGIN: us-800461c4
extern "C" void createCol6Hint() {}
// LLM-HARNESS-END: us-800461c4
// LLM-HARNESS-BEGIN: us-800461e8
extern "C" void createCol6Invite() {}
// LLM-HARNESS-END: us-800461e8
// LLM-HARNESS-BEGIN: us-80046284
extern "C" void createCol6Init() {}
// LLM-HARNESS-END: us-80046284
// LLM-HARNESS-BEGIN: us-800462a8
extern "C" void checkCol6Bat() {}
// LLM-HARNESS-END: us-800462a8
// LLM-HARNESS-BEGIN: us-800462cc
extern "C" void simpleEventStart() {}
// LLM-HARNESS-END: us-800462cc
// LLM-HARNESS-BEGIN: us-800462f0
extern "C" void simpleEventEnd() {}
// LLM-HARNESS-END: us-800462f0
// LLM-HARNESS-BEGIN: us-80046314
extern "C" void setTrust() {}
// LLM-HARNESS-END: us-80046314
// LLM-HARNESS-BEGIN: us-8004659c
extern "C" void setItemMulti() {}
// LLM-HARNESS-END: us-8004659c
// LLM-HARNESS-BEGIN: us-80046734
extern "C" void setKizunaTalk() {}
// LLM-HARNESS-END: us-80046734
// LLM-HARNESS-BEGIN: us-8004676c
extern "C" void winSys() {}
// LLM-HARNESS-END: us-8004676c
// LLM-HARNESS-BEGIN: us-800467ac
extern "C" void winSysSelect() {}
// LLM-HARNESS-END: us-800467ac
// LLM-HARNESS-BEGIN: us-80046844
extern "C" void getSelectNum() {}
// LLM-HARNESS-END: us-80046844
// LLM-HARNESS-BEGIN: us-80046980
extern "C" void mesAddPT() {}
// LLM-HARNESS-END: us-80046980
// LLM-HARNESS-BEGIN: us-800469f8
extern "C" void mesSubPT() {}
// LLM-HARNESS-END: us-800469f8
// LLM-HARNESS-BEGIN: us-80046a70
extern "C" void mesVisionON() {}
// LLM-HARNESS-END: us-80046a70
// LLM-HARNESS-BEGIN: us-80046ab0
extern "C" void mesVisionOFF() {}
// LLM-HARNESS-END: us-80046ab0
// LLM-HARNESS-BEGIN: us-80046af0
extern "C" void mesMonadoON() {}
// LLM-HARNESS-END: us-80046af0
// LLM-HARNESS-BEGIN: us-80046b30
extern "C" void mesMonadoOFF() {}
// LLM-HARNESS-END: us-80046b30
// LLM-HARNESS-BEGIN: us-80046b70
extern "C" void ptChangeNotice() {}
// LLM-HARNESS-END: us-80046b70
// LLM-HARNESS-BEGIN: us-80046ba4
extern "C" void save() {}
// LLM-HARNESS-END: us-80046ba4
// LLM-HARNESS-BEGIN: us-80046bd4
extern "C" void kizunaTalkStart() {}
// LLM-HARNESS-END: us-80046bd4
// LLM-HARNESS-BEGIN: us-80046c08
extern "C" void kizunaTalkEnd() {}
// LLM-HARNESS-END: us-80046c08
// LLM-HARNESS-BEGIN: us-80046c38
extern "C" void isPrioReq() {}
// LLM-HARNESS-END: us-80046c38
// LLM-HARNESS-BEGIN: us-80046c84
extern "C" void gameClear() {}
// LLM-HARNESS-END: us-80046c84
// LLM-HARNESS-BEGIN: us-80046ca8
extern "C" void setLastTalkNpc() {}
// LLM-HARNESS-END: us-80046ca8
// LLM-HARNESS-BEGIN: us-80046d54
extern "C" void isSETalkVoiceWait() {}
// LLM-HARNESS-END: us-80046d54
// LLM-HARNESS-BEGIN: us-80046da0
extern "C" void func_eu_80046DA0() {}
// LLM-HARNESS-END: us-80046da0
// LLM-HARNESS-BEGIN: us-80046dc4
extern "C" void func_eu_80046DC4() {}
// LLM-HARNESS-END: us-80046dc4
