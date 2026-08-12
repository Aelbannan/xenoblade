#include "kyoshin/plugin/pluginUi.hpp"
#include "monolib/util.hpp"
#include "monolib/vm/yvm2.h"

using namespace ml;

void ui_mesGetArts(VMThread* pThread) {
    int num1 = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    int num2 = vmArgIntGet(3, vmArgPtrGet(pThread, 2));
}

bool UiFlags::func_800459FC(u32 mask) const {
    return (this->flags & mask) != 0;
}
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
// Fade-out script command: (duration, count?). The duration is cast to float
// (MWCC emits the xoris 0x8000 + fsubs int-via-double conversion) and passed
// with the shared alpha constant to the fade controller.
int fadeOut_1(VMThread* pThread) {
    int v1 = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    int v2;
    if (vmArgOmitChk(pThread, 2) != 0) {
        v2 = 0;
    } else {
        v2 = vmArgIntGet(3, vmArgPtrGet(pThread, 2));
    }
    func_80135464(0, v2, lbl_eu_80665DB8, (float)(s32)v1, lbl_eu_80665DB8);
    return 0;
}
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
// ml::FixStr<64>::format - vsnprintf into a stack buffer, then copy into the
// fixed string (mString at 0x00, mLength at 0x40 for FixStr<64>).
template <>
void ml::FixStr<64>::format(const char* fmt, ...) {
    char buffer[0x100];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    mLength = (int)std::strlen(buffer);
    std::strcpy(mString, buffer);
}

// setTrust: adjust a character's trust value (clamped to [0, 5000]) and spawn
// the appropriate effect on the two player slots from lbl_eu_804FA9F0.
int setTrust(VMThread* pThread) {
    int arg1 = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    int arg2 = vmArgIntGet(3, vmArgPtrGet(pThread, 2));
    int arg3 = vmArgIntGet(4, vmArgPtrGet(pThread, 3));
    int arg4 = vmArgIntGet(5, vmArgPtrGet(pThread, 4));

    // Clamp the new trust value into [0, 5000]; remember the original delta.
    int cur = func_8009CF8C(arg3 + 0x28);
    int val = cur + arg4;
    int saved = 0;
    if (val < 0) val = 0;
    if (val > 0x1388) {
        saved = arg4;
        arg4 = arg4 - (val - 0x1388);
        if (arg4 < 0) arg4 = 0;
        val = 0x1388;
    }
    func_8009D018(arg3 + 0x28, val);
    if (saved != 0) arg4 = saved;
    if (arg4 == 0) return 0;

    // Effect class depends on the sign of the delta (172/174).
    int state = 0;
    if (arg4 > 0) {
        state = 0xac;
        func_80138078__FUl(0x34);
    } else if (arg4 < 0) {
        state = 0xae;
        func_80138078__FUl(0x36);
    }

    arg1--;
    arg2--;
    if (arg1 < 0) arg1 = 0;
    if (arg1 > 2) arg1 = 2;
    if (arg2 < 0) arg2 = 0;
    if (arg2 > 2) arg2 = 2;

    CfSlotTable tbl = lbl_eu_804FA9F0;
    CfEnumListHolder holder;
    func_80043D90(&holder);

    // Spawn the effect on the arg1 player slot.
    int done = 0;
    func_800F4A98(func_80043F18(&holder), tbl.values[arg1], 0);
    if (func_80043F18(&holder)->count >= 1) {
        if (code80135FDC_getByte_64059() == 0) {
            CfEnumListItem* item =
                (CfEnumListItem*)func_800F6EC0(func_80043F18(&holder), 0);
            void* cast = __dynamic_cast(item->field_04, 0,
                                        (const void*)&lbl_eu_806618D8,
                                        (const void*)&lbl_eu_806618F0, 0);
            func_800451D8((u32)state, cast);
        }
        done = 1;
    }

    // Same for the arg2 slot; both must succeed before notifying.
    func_800F4A98(func_80043F18(&holder), tbl.values[arg2], 0);
    if (func_80043F18(&holder)->count >= 1) {
        if (code80135FDC_getByte_64059() == 0) {
            CfEnumListItem* item =
                (CfEnumListItem*)func_800F6EC0(func_80043F18(&holder), 0);
            void* cast = __dynamic_cast(item->field_04, 0,
                                        (const void*)&lbl_eu_806618D8,
                                        (const void*)&lbl_eu_806618F0, 0);
            func_800451D8((u32)state, cast);
        }
        done++;
    }

    if (done >= 2) {
        func_8013DB6C(5, 0, arg3, arg4);
    }
    __dt__80043E88(&holder, -1);
    return 0;
}

// setItemMulti: grant multiple items (up to 4 ids + a bool flag) via the
// shared item-queue helper.
int setItemMulti(VMThread* pThread) {
    int v1, v2, v3, v4;
    int next;

    // Arg 1: optional
    if (vmArgOmitChk(pThread, 1)) { v1 = 0; next = 2; }
    else { next = 2; VMArg* arg = vmArgPtrGet(pThread, 1); v1 = vmArgIntGet(2, arg); }

    // Arg 2: optional
    if (vmArgOmitChk(pThread, next)) { v2 = 0; next++; }
    else { VMArg* arg = vmArgPtrGet(pThread, next); next++; v2 = vmArgIntGet(next, arg); }

    // Arg 3: optional
    if (vmArgOmitChk(pThread, next)) { v3 = 0; next++; }
    else { VMArg* arg = vmArgPtrGet(pThread, next); next++; v3 = vmArgIntGet(next, arg); }

    // Arg 4: optional
    if (vmArgOmitChk(pThread, next)) { v4 = 0; next++; }
    else { VMArg* arg = vmArgPtrGet(pThread, next); next++; v4 = vmArgIntGet(next, arg); }

    // Arg 5: optional bool
    int b;
    if (vmArgOmitChk(pThread, next)) { b = 0; }
    else { VMArg* arg = vmArgPtrGet(pThread, next); next++; b = vmArgBoolGet(next, arg); }

    func_8013E2E0(v1, v2, v3, v4, 0, 1, 0, 1, b != 0);
    return 0;
}
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
    extern void func_8013E8E0(int);
    enablePadFlags__Q22cf13CfGameManagerFUlb(-1, 1);
    func_8013E8E0(0);
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
// Retail C callee (code80135FDC split unit), extern "C" at file scope (MWCC
// rejects extern "C" inside function bodies).
extern "C" void code80135FDC_clearByte_64059();

int kizunaTalkEnd(){
    extern u32 lbl_eu_80663E28;
    lbl_eu_80663E28 &= ~0x1000;
    code80135FDC_clearByte_64059();
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
