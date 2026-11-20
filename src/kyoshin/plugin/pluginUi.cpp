#include "kyoshin/cf/CfBdat.hpp"
#include "kyoshin/plugin/pluginUi.hpp"
#include "kyoshin/CUIWindowManagerApi.hpp"
#include "kyoshin/CfEnumListHolder.hpp"
#include "kyoshin/cfsys/CfWalkerCore.hpp"
#include "monolib/util.hpp"
#include "monolib/vm/yvm2.h"
#include "kyoshin/code_80135FDC_State.hpp"

using namespace ml;

// ---------------------------------------------------------------------------
// Retail data-slice definitions owned by this TU (splits.txt: .rodata
// 0x804FA9F0-0x804FAC20, .data 0x80525D68-0x80525E98, .sdata2
// 0x80665DB8-0x80665DF8). The command-name strings, slot table and plugin
// table are defined here explicitly in retail order; MWCC's literal pool
// (int->float conversion magic copies) trails the .sdata2 defs and is
// trimmed by UnitRules after its relocs are rebound to the real defs.
// ---------------------------------------------------------------------------

// .sdata2: fade alpha constant + int->float magic + plugin-name strings.
__declspec(section ".sdata2") __attribute__((used))
double lbl_eu_80665DC0 = 4503601774854144.0;
__declspec(section ".sdata2") __attribute__((used))
char lbl_eu_80665DC8[] = "winTalk";
__declspec(section ".sdata2") __attribute__((used))
char lbl_eu_80665DD0[] = "pcTalk";
__declspec(section ".sdata2") __attribute__((used))
char lbl_eu_80665DD8[] = "fadeIn";
__declspec(section ".sdata2") __attribute__((used))
char lbl_eu_80665DE0[] = "fadeOut";
__declspec(section ".sdata2") __attribute__((used))
char lbl_eu_80665DE8[] = "winSys";
__declspec(section ".sdata2") __attribute__((used))
char lbl_eu_80665DF0[] = "save";

// .rodata: effect-slot table + VM command names + shared message strings.
__declspec(section ".rodata") __attribute__((aligned(8))) __attribute__((used))
CfSlotTable pluginTrustEffectSlots = {{0x00000010u, 0x00100000u, 0x00200000u}};

// .data: VM plugin command table (38 name/function pairs + null terminator).
// First table carries aligned(8) for the section alignment.
__declspec(section ".data") __attribute__((aligned(8))) __attribute__((used))
const void* pluginCommandTable[76] = {
    (const void*)lbl_eu_80665DC8, (const void*)winTalk,
    (const void*)lbl_eu_80665DD0, (const void*)pcTalk,
    "winTalkWait", (const void*)winTalkWait,
    "winTalkNoName", (const void*)winTalkNoName,
    (const void*)lbl_eu_80665DD8, (const void*)fadeIn_1,
    (const void*)lbl_eu_80665DE0, (const void*)fadeOut_1,
    "fadeWait", (const void*)fadeWait_1,
    "createCol6Sys", (const void*)createCol6Sys,
    "createCol6Hint", (const void*)createCol6Hint,
    "createCol6Invite", (const void*)createCol6Invite,
    "createCol6Init", (const void*)createCol6Init,
    "checkCol6Bat", (const void*)checkCol6Bat,
    "simpleEventStart", (const void*)simpleEventStart,
    "simpleEventEnd", (const void*)simpleEventEnd,
    "setTrust", (const void*)setTrust,
    "setItemMulti", (const void*)setItemMulti,
    "setKizunaTalk", (const void*)setKizunaTalk,
    (const void*)lbl_eu_80665DE8, (const void*)winSys,
    "winSysSelect", (const void*)winSysSelect,
    "getSelectNum", (const void*)getSelectNum,
    "mesGetArts", (const void*)mesGetArts,
    "mesAddPT", (const void*)mesAddPT,
    "mesSubPT", (const void*)mesSubPT,
    "mesVisionON", (const void*)mesVisionON,
    "mesVisionOFF", (const void*)mesVisionOFF,
    "mesMonadoON", (const void*)mesMonadoON,
    "mesMonadoOFF", (const void*)mesMonadoOFF,
    "ptChangeNotice", (const void*)ptChangeNotice,
    (const void*)lbl_eu_80665DF0, (const void*)save,
    "kizunaTalkStart", (const void*)kizunaTalkStart,
    "kizunaTalkEnd", (const void*)kizunaTalkEnd,
    "isPrioReq", (const void*)isPrioReq,
    "gameClear", (const void*)gameClear,
    "setLastTalkNpc", (const void*)setLastTalkNpc,
    "isSETalkVoiceWait", (const void*)isSETalkVoiceWait,
    "setSETalkVoiceWait", (const void*)uiSetSETalkVoiceWait,
    "clearSETalkVoiceWait", (const void*)uiClearSETalkVoiceWait,
    (const void*)0, (const void*)0
};

void ui_mesGetArts(VMThread* pThread) {
    int num1 = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    int num2 = vmArgIntGet(3, vmArgPtrGet(pThread, 2));
}

extern "C" bool hasAnyFlags(UiFlags* self, u32 mask) {
    return (self->flags & mask) != 0;
}
void pluginUiRegist() {
    vmPluginRegist((const char*)(lbl_eu_804FABF0 + 0x28),
                   (PluginFuncData*)pluginCommandTable);
}

// winTalk: open a talk window. Script args: (window id, text). The window id
// is passed through to the talk-window factory with mode 1.
int winTalk(VMThread* pThread) {
    VMArg* arg = vmArgPtrGet(pThread, 1);
    int winId = vmArgIntGet(2, arg);
    arg = vmArgPtrGet(pThread, 2);
    const char* str = vmArgStringGet(3, arg);
    UIWin_CreateTalkWin(winId, str, 1);
    return 0;
}
// pcTalk: start a party-chat line. Args: (member id). The id is passed to
// the battle sub-object of player 0; flag is set when the id equals 8.
int pcTalk(VMThread* pThread) {
    VMArg* arg = vmArgPtrGet(pThread, 1);
    int id = vmArgIntGet(2, arg);
    void* player = getPlayer__Q22cf13CfGameManagerFi(0);
    PcBattleTalkObj* obj =
        (PcBattleTalkObj*)__dynamic_cast(player, 0, &__RTTI__Q22cf10CfObjectPc,
                                         &__RTTI__Q22cf12CfObjectMove, 0);
    Walker_StepBattleArts(obj->field_3ED4, id, id == 8);
    return 0;
}

// winTalkWait: park the thread while a talk window is open.
int winTalkWait(VMThread* pThread) {
    if (UIWin_HasPendingUpdate(1)) {
        vmWaitModeSet(pThread);
    } else {
        void* player = getPlayer__Q22cf13CfGameManagerFi(0);
        PcBattleTalkObj* obj =
            (PcBattleTalkObj*)__dynamic_cast(player, 0, &__RTTI__Q22cf10CfObjectPc,
                                             &__RTTI__Q22cf12CfObjectMove, 0);
        // Probe vf40 (+0x40) on the +0x3ED4 walker sub-object: nonzero while busy.
        if (obj->field_3ED4->vf40(0x8000) != 0) {
            vmWaitModeSet(pThread);
        }
    }
    return 0;
}
// Talk window without a name plate: open with mode 0.
int winTalkNoName(VMThread* pThread) {
    UIWin_CreateEveTalkWin(0, vmArgStringGet(2, vmArgPtrGet(pThread, 1)));
    return 0;
}
// System message window: text argument, no extra args.
int winSys(VMThread* pThread) {
    UIWin_CreateSysWin0(vmArgStringGet(2, vmArgPtrGet(pThread, 1)), 0, 0);
    return 0;
}

// Fade-in script command: (duration, count?). Same shape as fadeOut_1 but
// mode 2; the duration converts through MWCC's implicit int->float expansion
// (xoris/0x43300000 word-pair + fsubs against the shared sdata2 correction
// constant). NOTE: the pooled magic lands on anon label @2067; the
// pluginUi.o UnitRules exact_renames key in tools/postprocess_reloc_names.py
// still says @2082 and must be refreshed to @2067 for the reloc-name gate.
int fadeIn_1(VMThread* pThread) {
    int v1 = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    int v2;
    if (vmArgOmitChk(pThread, 2) != 0) {
        v2 = 0;
    } else {
        v2 = vmArgIntGet(3, vmArgPtrGet(pThread, 2));
    }
    CUICfManager_queueFadeMenu(2, v2, -1.0f, -1.0f, (float)v1);
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
    CUICfManager_queueFadeMenu(0, v2, -1.0f, (float)(s32)v1, -1.0f);
    return 0;
}
// Fade-wait script command: park the thread while a fade is running.
int fadeWait_1(VMThread* pThread) {
    if (getFadeMenu() && isFadeActive()) {
        vmWaitModeSet(pThread);
    }
    return 0;
}
int createCol6Sys(VMThread* pThread) {
    UIWin_Create602F4Win();
    return 0;
}
int createCol6Hint(VMThread* pThread) {
    UIWin_Create5DCD0Win();
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
    UIWin_CreateCol6Invite((u16)a, (u8)b, (u8)c);
    return 0;
}
int createCol6Init(VMThread* pThread) {
    InitCol6LodTables();
    return 0;
}
int checkCol6Bat(VMThread* pThread) {
    UIWin_CreateCol6Check();
    return 0;
}
int simpleEventStart(VMThread* pThread) {
    extern void MenuStateSet64058();
    MenuStateSet64058();
    return 0;
}
int simpleEventEnd(VMThread* pThread) {
    extern void code80135FDC_clearByte_64058();
    code80135FDC_clearByte_64058();
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
// the appropriate effect on the two player slots from pluginTrustEffectSlots.
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
    int cur = CtrlRemote_TouchBitByArg(arg3 + 0x28);
    int val = cur + arg4;
    if (val < 0) val = 0;
    if (val > 0x1388) {
        saved = arg4;
        arg4 = arg4 - (val - 0x1388);
        if (arg4 < 0) arg4 = 0;
        val = 0x1388;
    }
    CtrlRemote_SetSharedBit(arg3 + 0x28, val);
    if (saved != 0) arg4 = saved;
    if (arg4 == 0) return 0;

    // Effect class depends on the sign of the delta (172/174).
    int state = 0;
    if (arg4 > 0) {
        state = 0xac;
        playUISound(0x34);
    } else if (arg4 < 0) {
        state = 0xae;
        playUISound(0x36);
    }

    // Clamp both slot indices into [0, 2].
    arg1--;
    arg2--;
    if (arg1 < 0) arg1 = 0;
    if (arg1 > 2) arg1 = 2;
    if (arg2 < 0) arg2 = 0;
    if (arg2 > 2) arg2 = 2;

    CfSlotTable tbl = pluginTrustEffectSlots;
    int done = 0;
    CfEnumListHolder holder;

    // Spawn the effect on the arg1 player slot.
    startEnumObjects(holder.get(), tbl.values[arg1], 0);
    if (((CfEnumList*)holder.get())->count >= 1) {
        if (code80135FDC_getByte_64059() == 0) {
            CfEnumListItem* item =
                (CfEnumListItem*)getEntryAt(holder.get(), 0);
            void* cast = __dynamic_cast(item->field_04, 0,
                                        (const void*)&__RTTI__Q22cf12CfObjectMove,
                                        (const void*)&__RTTI__Q22cf8CfObject, 0);
            bindIndexedEffect((u32)state, cast);
        }
        done++;
    }

    // Same for the arg2 slot; both must succeed before notifying.
    startEnumObjects(holder.get(), tbl.values[arg2], 0);
    if (((CfEnumList*)holder.get())->count >= 1) {
        if (code80135FDC_getByte_64059() == 0) {
            CfEnumListItem* item =
                (CfEnumListItem*)getEntryAt(holder.get(), 0);
            void* cast = __dynamic_cast(item->field_04, 0,
                                        (const void*)&__RTTI__Q22cf12CfObjectMove,
                                        (const void*)&__RTTI__Q22cf8CfObject, 0);
            bindIndexedEffect((u32)state, cast);
        }
        done++;
    }

    if (done >= 2) {
        UIWin_CreateMenuUpdate(5, 0, arg3, arg4);
    }
    return 0;
}

// setItemMulti
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

    UIWin_CreateItemMulti(v1, v2, v3, v4, 0, 1, 0, 1, b != 0);
    return 0;
}
// Kizuna-talk command: read the script int argument and hand it to the
// kizuna-talk handler.
int setKizunaTalk(VMThread* pThread) {
    UIWin_CreateKizunaTalk(vmArgIntGet(2, vmArgPtrGet(pThread, 1)));
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
    UIWin_Create25070Win(str1, str2, str3);
    return 0;
}
// Store the select-window's current item index as the script return value.
int getSelectNum(VMThread* pThread) {
    VMArg arg;
    arg.type = 3;
    arg.value.uintVal = UIWin_GetTimer();
    vmRetValSet(pThread, &arg);
    return 1;
}
// mesAddPT/mesSubPT: resolve the party member name for the script int arg
// from the shared character table, then show it in a party-talk box whose
// caption comes from the fixed string-table entry at +0xD (index 11/12).
static void mesPTSet(int id, int captionIdx) {
    char* name = BdatGetPtrDirect(::cf::CfBdat::spBtlPcListFileData, lbl_eu_804FABF0, id);
    UIWin_CreateSysWin1((u32)(uintptr_t)name,
                  (void*)BdatTouchStringCell(&lbl_eu_804FABF0[0xd], lbl_eu_804FABF0,
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
    UIWin_CreateSysWin0(BdatTouchStringCell(&lbl_eu_804FABF0[0xd], lbl_eu_804FABF0, index), 0, 0);
    return 0;
}
int mesVisionON() { return mesVisionSet(0xd); }
int mesVisionOFF() { return mesVisionSet(0xe); }
// Monado activation/deactivation messages: pull the string table entry into
// the scratch area at table+0xd, then open it in a system window.
int mesMonadoON() {
    UIWin_CreateSysWin0(BdatTouchStringCell(lbl_eu_804FABF0 + 0xd, lbl_eu_804FABF0, 0xf), 0, 0);
    return 0;
}
int mesMonadoOFF() {
    UIWin_CreateSysWin0(BdatTouchStringCell(lbl_eu_804FABF0 + 0xd, lbl_eu_804FABF0, 0x10), 0, 0);
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

    char* row = BdatGetPtrDirect(::cf::CfBdat::spBtlPcListFileData, lbl_eu_804FABF0, id);
    char* sIdx = BdatTouchStringCell(&lbl_eu_804FABF0[5], lbl_eu_804FABF0, idx);
    char* sName = BdatTouchStringCell(&lbl_eu_804FABF0[0xd], lbl_eu_804FABF0, 7);
    char* sFoot = BdatTouchStringCell(&lbl_eu_804FABF0[0xd], lbl_eu_804FABF0, 0xa);

    ml::FixStr<64> str;
    str.format(&lbl_eu_804FABF0[0x18], row, sName, sIdx);
    UIWin_CreateSysWin1((u32)(uintptr_t)str.mString, sFoot, 0, 0);
    return 0;
}

// Retail symbol is the .Fb form (bool param) though args pass via r4/r5.
extern "C" void enablePadFlags__Q22cf13CfGameManagerFUlb(int, int);
// Retail symbol is the unmangled .Fv-registered name (args pass via r4).

int ptChangeNotice(){
    enablePadFlags__Q22cf13CfGameManagerFUlb(-1, 1);
    UIWin_CreatePTChange(0);
    return 0;
}
int save() {
    extern u32 gmPauseFlags;
    if (!(gmPauseFlags & 0x40000000)) UIWin_CreateSaveWin();
    return 0;
}
int kizunaTalkStart() {
    extern u32 gmPauseFlags;
    MenuStateSet64059Notify();
    gmPauseFlags |= 0x1000;
    prepareTextBuffer__Q22cf13CfGameManagerFv();
    return 0;
}
// Retail C callee (code80135FDC split unit), extern "C" at file scope (MWCC
// rejects extern "C" inside function bodies).

int kizunaTalkEnd(){
    extern u32 gmPauseFlags;
    gmPauseFlags &= ~0x1000;
    code80135FDC_clearByte_64059();
    return 0;
}
// Report priority-request window state: type 2 when busy, type 1 otherwise.
int isPrioReq(VMThread* pThread) {
    VMArg arg;
    int busy = CUICfManager_isUIBusy();
    arg.type = !busy + 1;
    vmRetValSet(pThread, &arg);
    return 1;
}
int gameClear(VMThread* pThread) {
    CUICfManager_queueGameClearMenu();
    return 0;
}
// setLastTalkNpc: find the character-table row whose key field (table entry
// at +0x1f) equals the script id and record its 1-based ordinal. A non-
// positive id clears the record instead.
int setLastTalkNpc(VMThread* pThread) {
    VMArg* arg = vmArgPtrGet(pThread, 1);
    int id = vmArgIntGet(2, arg);
    if (id <= 0) {
        CtrlObjectParam_SetWorkTailValue(0);
        return 0;
    }
    char* tbl = (char*)::cf::CfBdat::spFldNpcListFileData;
    int count = Bdat_GetMaxRow_B1EC(tbl);
    for (int i = 1; i <= count; i++) {
        // Key compare is on the low 16 bits only.
        if ((u16)BdatGetU16Direct(tbl, &lbl_eu_804FABF0[0x1f], i) == id) {
            CtrlObjectParam_SetWorkTailValue(i);
            break;
        }
    }
    return 0;
}
// isSETalkVoiceWait: report whether an SE-talk voice is still playing;
// result type is TRUE(2)/FALSE(1).
int isSETalkVoiceWait(VMThread* pThread) {
    VMArg arg;
    arg.type = (MenuStateCheck64064or30() == 0) + 1;
    vmRetValSet(pThread, &arg);
    return 1;
}
extern "C" int uiSetSETalkVoiceWait(VMThread* pThread) {
    code80135FDC_setByte_64064();
    return 0;
}
extern "C" int uiClearSETalkVoiceWait(VMThread* pThread) {
    extern void MenuStateClear64064();
    MenuStateClear64064();
    return 0;
}

__declspec(section ".rodata") __attribute__((used))
char lbl_eu_804FABE0[] = "cf::CfObjectPc";

__declspec(section ".rodata") __attribute__((used))
char lbl_eu_804FABF0[] =
"name\0pc_arts\0MNU_sysmes\0%s%s%s\0rlt_meet\0ui\0\0\0\0\0";
