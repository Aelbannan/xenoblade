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
    vmPluginRegist((void*)((char*)lbl_eu_804FABF0 + 0x28), (void*)lbl_eu_80525D68);
}

// winTalk: open a talk window. Script args: (window id, text). The window id
// is passed through to the talk-window factory with mode 1.
int winTalk(VMThread* pThread) {
    VMArg* arg = vmArgPtrGet(pThread, 1);
    int winId = vmArgIntGet(2, arg);
    arg = vmArgPtrGet(pThread, 2);
    const char* str = vmArgStringGet(3, arg);
    func_8013D07C(winId, str, 1);
    return 0;
}
// pcTalk: start a party-chat line. Args: (member id). The id is passed to
// the battle sub-object of player 0; flag is set when the id equals 8.
int pcTalk(VMThread* pThread) {
    VMArg* arg = vmArgPtrGet(pThread, 1);
    int id = vmArgIntGet(2, arg);
    void* player = getPlayer__Q22cf13CfGameManagerFi(0);
    PcBattleTalkObj* obj =
        (PcBattleTalkObj*)__dynamic_cast(player, 0, &lbl_eu_806619A0,
                                         &lbl_eu_806618D8, 0);
    func_800C4244(obj->field_3ED4, id, id == 8);
    return 0;
}

// winTalkWait: park the thread while a talk window is open.
int winTalkWait(VMThread* pThread) {
    if (func_8013EB90(1)) {
        vmWaitModeSet(pThread);
    } else {
        void* player = getPlayer__Q22cf13CfGameManagerFi(0);
        PcBattleTalkObj* obj =
            (PcBattleTalkObj*)__dynamic_cast(player, 0, &lbl_eu_806619A0,
                                             &lbl_eu_806618D8, 0);
        // Probe vtable+0x40 on the +0x3ED4 sub-object: nonzero while busy.
        if (((PcTalkProbeIf*)obj->field_3ED4)->probe(0x8000) != 0) {
            vmWaitModeSet(pThread);
        }
    }
    return 0;
}
// Talk window without a name plate: open with mode 0.
int winTalkNoName(VMThread* pThread) {
    func_8013D448(0, vmArgStringGet(2, vmArgPtrGet(pThread, 1)));
    return 0;
}
// System message window: text argument, no extra args.
int winSys(VMThread* pThread) {
    func_8013D55C(vmArgStringGet(2, vmArgPtrGet(pThread, 1)), 0, 0);
    return 0;
}
// Manual signed-int -> float conversion (docs/MWCC_PATTERNS.md 7i): build
// the 0x4330000080000000 bit pattern and subtract the shared sdata2 magic so
// the lfd references lbl_eu_80665DC0 instead of a TU-local pool label.
static float ConvS32ToF32(s32 x) {
    union {
        double d;
        u32 w[2];
    } u;
    // xoris word first, then 0x43300000, or MWCC hoists the lis out of order.
    u.w[1] = (u32)x ^ 0x80000000;
    u.w[0] = 0x43300000;
    return u.d - lbl_eu_80665DC0;
}

// Fade-in script command: (duration, count?). Same shape as fadeOut_1 but
// mode 2 and the converted duration goes in the last float slot.
int fadeIn_1(VMThread* pThread) {
    int v1 = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    int v2;
    if (vmArgOmitChk(pThread, 2) != 0) {
        v2 = 0;
    } else {
        v2 = vmArgIntGet(3, vmArgPtrGet(pThread, 2));
    }
    // Manual int->float conversion (docs/MWCC_PATTERNS.md 7i), written inline
    // with the stores interleaved so MWCC schedules the retail sequence:
    // xoris word, alpha load, 0x43300000 word, then the fsubs against the
    // shared sdata2 magic lbl_eu_80665DC0.
    union {
        double d;
        u32 w[2];
    } u;
    // Stores sequenced inside the conversion argument so MWCC walks the
    // subtraction right-to-left like retail (magic constant lands in the
    // destination register f3).
    func_80135464(2, v2, lbl_eu_80665DB8, lbl_eu_80665DB8,
                  (u.w[1] = (u32)v1 ^ 0x80000000, u.w[0] = 0x43300000,
                   u.d - lbl_eu_80665DC0));
    return 0;
}
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
// Fade-wait script command: park the thread while a fade is running.
int fadeWait_1(VMThread* pThread) {
    if (func_80113E1C() && func_80113E24()) {
        vmWaitModeSet(pThread);
    }
    return 0;
}
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
// Colosseum 6 invite: three int args, narrowed to u16/u8/u8 by the callee
// prototype.
int createCol6Invite(VMThread* pThread) {
    VMArg* arg = vmArgPtrGet(pThread, 1);
    int a = vmArgIntGet(2, arg);
    arg = vmArgPtrGet(pThread, 2);
    int b = vmArgIntGet(3, arg);
    arg = vmArgPtrGet(pThread, 3);
    int c = vmArgIntGet(4, arg);
    func_8013DF44((u16)a, (u8)b, (u8)c);
    return 0;
}
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
    VMArg* p1 = vmArgPtrGet(pThread, 1);
    int arg1 = vmArgIntGet(2, p1);
    VMArg* p2 = vmArgPtrGet(pThread, 2);
    int arg2 = vmArgIntGet(3, p2);
    VMArg* p3 = vmArgPtrGet(pThread, 3);
    int arg3 = vmArgIntGet(4, p3);
    VMArg* p4 = vmArgPtrGet(pThread, 4);
    int arg4 = vmArgIntGet(5, p4);

    // Clamp the new trust value into [0, 5000]; remember the original delta.
    int saved = 0;
    int cur = func_8009CF8C(arg3 + 0x28);
    int val = cur + arg4;
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
    int done = 0;
    CfEnumListHolder holder;
    func_80043D90(&holder);

    // Spawn the effect on the arg1 player slot.
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
        done++;
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
    int v1, v2, v3, v4, b;
    // idx is the current optional-arg slot; each consumed arg advances it.
    int idx = 1;

    // Arg 1: optional.
    if (vmArgOmitChk(pThread, idx)) {
        v1 = 0;
        idx++;
    } else {
        VMArg* arg = vmArgPtrGet(pThread, idx++);
        v1 = vmArgIntGet(idx, arg);
    }

    // Args 2-4: optional ints; the pointer fetch consumes the current index
    // and advances it before the int fetch reads the next slot.
    if (vmArgOmitChk(pThread, idx)) {
        v2 = 0;
        idx++;
    } else {
        VMArg* arg = vmArgPtrGet(pThread, idx++);
        v2 = vmArgIntGet(idx, arg);
    }

    if (vmArgOmitChk(pThread, idx)) {
        v3 = 0;
        idx++;
    } else {
        VMArg* arg = vmArgPtrGet(pThread, idx++);
        v3 = vmArgIntGet(idx, arg);
    }

    if (vmArgOmitChk(pThread, idx)) {
        v4 = 0;
        idx++;
    } else {
        VMArg* arg = vmArgPtrGet(pThread, idx++);
        v4 = vmArgIntGet(idx, arg);
    }

    // Arg 5: optional bool.
    if (vmArgOmitChk(pThread, idx)) {
        b = 0;
    } else {
        VMArg* arg = vmArgPtrGet(pThread, idx++);
        b = vmArgBoolGet(idx, arg);
    }

    func_8013E2E0(v1, v2, v3, v4, 0, 1, 0, 1, b != 0);
    return 0;
}
// Kizuna-talk command: read the script int argument and hand it to the
// kizuna-talk handler.
int setKizunaTalk(VMThread* pThread) {
    func_8013E52C(vmArgIntGet(2, vmArgPtrGet(pThread, 1)));
    return 0;
}
// System select window: three string args handed to the window factory.
int winSysSelect(VMThread* pThread) {
    VMArg* arg = vmArgPtrGet(pThread, 1);
    const char* str1 = vmArgStringGet(2, arg);
    arg = vmArgPtrGet(pThread, 2);
    const char* str2 = vmArgStringGet(3, arg);
    arg = vmArgPtrGet(pThread, 3);
    const char* str3 = vmArgStringGet(4, arg);
    func_8013D978(str1, str2, str3);
    return 0;
}
// Store the select-window's current item index as the script return value.
int getSelectNum(VMThread* pThread) {
    VMArg arg;
    arg.type = 3;
    arg.value.uintVal = func_8013EC58();
    vmRetValSet(pThread, &arg);
    return 1;
}
// mesAddPT/mesSubPT: resolve the party member name for the script int arg
// from the shared character table, then show it in a party-talk box whose
// caption comes from the fixed string-table entry at +0xD (index 11/12).
static void mesPTSet(int id, int captionIdx) {
    char* name = func_8013639C(lbl_eu_80664090, lbl_eu_804FABF0, id);
    func_8013D688(name,
                  func_80136190(&lbl_eu_804FABF0[0xd], lbl_eu_804FABF0,
                                captionIdx),
                  0, 0);
}
int mesAddPT(VMThread* pThread) {
    VMArg* arg = vmArgPtrGet(pThread, 1);
    mesPTSet(vmArgIntGet(2, arg), 0xb);
    return 0;
}
int mesSubPT(VMThread* pThread) {
    VMArg* arg = vmArgPtrGet(pThread, 1);
    mesPTSet(vmArgIntGet(2, arg), 0xc);
    return 0;
}
// Show/hide the vision message: fetch the fixed entry from the message table
// (entry at offset 13/14) and open a system window with it.
static int mesVisionSet(int index) {
    // Retail always reads the entry at +0xD; only the table index differs.
    func_8013D55C(func_80136190(&lbl_eu_804FABF0[0xd], lbl_eu_804FABF0, index), 0, 0);
    return 0;
}
int mesVisionON() { return mesVisionSet(0xd); }
int mesVisionOFF() { return mesVisionSet(0xe); }
// Monado activation/deactivation messages: pull the string table entry into
// the scratch area at table+0xd, then open it in a system window.
int mesMonadoON() {
    func_8013D55C(func_80136190(lbl_eu_804FABF0 + 0xd, lbl_eu_804FABF0, 0xf), 0, 0);
    return 0;
}
int mesMonadoOFF() {
    func_8013D55C(func_80136190(lbl_eu_804FABF0 + 0xd, lbl_eu_804FABF0, 0x10), 0, 0);
    return 0;
}
// mesGetArts: build the arts description message. Args: (id, index). The
// character row is looked up by id; three fixed string-table entries supply
// the name/caption pieces, and the row is formatted into a 64-byte string
// shown in a party-talk box whose footer is the index-10 entry.
int mesGetArts(VMThread* pThread) {
    VMArg* arg = vmArgPtrGet(pThread, 1);
    int id = vmArgIntGet(2, arg);
    arg = vmArgPtrGet(pThread, 2);
    int idx = vmArgIntGet(3, arg);

    char* row = func_8013639C(lbl_eu_80664090, lbl_eu_804FABF0, id);
    char* sIdx = func_80136190(&lbl_eu_804FABF0[5], lbl_eu_804FABF0, idx);
    char* sName = func_80136190(&lbl_eu_804FABF0[0xd], lbl_eu_804FABF0, 7);
    char* sFoot = func_80136190(&lbl_eu_804FABF0[0xd], lbl_eu_804FABF0, 0xa);

    ml::FixStr<64> str;
    str.format(&lbl_eu_804FABF0[0x18], row, sName, sIdx);
    func_8013D688(str.mString, sFoot, 0, 0);
    return 0;
}

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
// Report priority-request window state: type 2 when busy, type 1 otherwise.
int isPrioReq(VMThread* pThread) {
    VMArg arg;
    int busy = func_80135708();
    arg.type = !busy + 1;
    vmRetValSet(pThread, &arg);
    return 1;
}
int gameClear(VMThread* pThread) {
    extern void func_8013500C();
    func_8013500C();
    return 0;
}
// setLastTalkNpc: find the character-table row whose key field (table entry
// at +0x1f) equals the script id and record its 1-based ordinal. A non-
// positive id clears the record instead.
int setLastTalkNpc(VMThread* pThread) {
    VMArg* arg = vmArgPtrGet(pThread, 1);
    int id = vmArgIntGet(2, arg);
    if (id <= 0) {
        func_8009ECD0(0);
        return 0;
    }
    char* tbl = lbl_eu_80664098;
    int count = func_8003B1EC(tbl);
    for (int i = 1; i <= count; i++) {
        // Key compare is on the low 16 bits only.
        if ((u16)func_80136254(tbl, &lbl_eu_804FABF0[0x1f], i) == id) {
            func_8009ECD0(i);
            break;
        }
    }
    return 0;
}
// isSETalkVoiceWait: report whether an SE-talk voice is still playing;
// result type is TRUE(2)/FALSE(1).
int isSETalkVoiceWait(VMThread* pThread) {
    VMArg arg;
    arg.type = (func_eu_8013C8F4() == 0) + 1;
    vmRetValSet(pThread, &arg);
    return 1;
}
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
