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

// ---------------------------------------------------------------------------
// Imports (retail symbol names, C linkage via the extern "C" block below;
// the struct/label externs at global scope keep their plain names since MWCC
// does not mangle global variables).
// ---------------------------------------------------------------------------

// Opaque enum-list holder (CTaskGame_enumListCtor ctor / __dt__80043E88 dtor /
// CTaskGame_enumListGet accessor); the list it yields has an element count at +0x620.
struct CfEnumListHolder {
    void* list;  // 0x0
    u32 handle;  // 0x4
};
struct CfEnumList {
    u8 _00[0x620];
    u32 count;  // 0x620
};

// Item returned by func_800F6EC0; the object pointer for __dynamic_cast sits
// at +0x4.
struct CfEnumListItem {
    u8 _00[0x4];
    void* field_04;  // 0x4
};

// 3-entry u32 table (player-slot effect ids) copied onto the stack by setTrust.
struct CfSlotTable {
    u32 values[3];  // 0x0
};
extern CfSlotTable lbl_eu_804FA9F0;

// RTTI typeinfo pair for __dynamic_cast in setTrust.
// Declared as complete scalar types so MWCC sdata-addresses them
// (EMB_SDA21 `li rX, sym@sda21`) like retail.
extern u32 lbl_eu_806618D8;
extern const void* lbl_eu_806618F0;

// sdata2 constant for fadeOut_1 (fade alpha value).
extern f32 lbl_eu_80665DB8;
// The int->double conversion magic (lbl_eu_80665DC0) is planted as a TU-local
// static const double in pluginUi.cpp so MWCC unifies its implicit-cast magic
// pool entry with it (see the comment there).

// Message string tables / buffers used by the mesVision commands.
extern char lbl_eu_804FABF0[];
// VM plugin command table (retail .data 0x80525D68, 38 name/function pairs).
// Defined in pluginUi.cpp as a typed pointer table; vmPluginRegist takes its
// address (decays identically for any array type, so call code is unchanged).
extern const void* lbl_eu_80525D68[];

// Player RTTI source type / pointer table entry used by the talk commands.
// Complete scalar type so MWCC emits EMB_SDA21 addressing (retail `li @sda21`),
// not lis/addi (incomplete array types are not SDA-eligible).
extern u32 lbl_eu_806619A0;

// Shared BDAT character-name table pointer (.sbss); resolved by
// BdatGetPtrDirect against lbl_eu_804FABF0 keys.
extern char* lbl_eu_80664090;

// Sub-object hanging off the player at +0x3ED4 (real owner:
// cf::CfObjectImplWalker, whose vf40 at +0x40 is the talk-busy probe used
// by winTalkWait); handed to func_800C4244 (pcTalk).
namespace cf { class CfObjectImplWalker; }
struct PcBattleTalkObj {
    u8 _00[0x3ED4];
    cf::CfObjectImplWalker* field_3ED4; // +0x3ED4
};

extern "C" {

void pluginUiRegist();

// Fade controller hook: (int mode, int arg, float a, float b, float c).
// The last argument is an int->float cast: MWCC's inline expansion emits
// xoris/0x43300000 word-pair + fsubs against the shared sdata2 magic
// (renamed to lbl_eu_80665DC0 by the unit's postprocess pool rule).
void CUICfManager_queueFadeMenu(int r3, int r4, float f1, float f2, float f3);

// Item-grant helper: 8 register args + 1 stack arg.
void UIWin_CreateItemMulti(u32 a1, u32 a2, u32 a3, u32 a4, u32 a5, u32 a6, u32 a7,
                   u32 a8, u32 a9);

// Talk-window factory (window id, text, mode); used by winTalk.
void UIWin_CreateTalkWin(u32 obj, const char* str, int mode);

void CTaskGame_enumListCtor(CfEnumListHolder* holder);
CfEnumList* CTaskGame_enumListGet(CfEnumListHolder* holder);
void __dt__80043E88(CfEnumListHolder* holder, int tags);
void func_800F4A98(void* list, int type, int value);
void* func_800F6EC0(void* list, int index);
void* func_800451D8(u32 cls, void* param);
u8 code80135FDC_getByte_64059();
void playUISound__FUl(u32 op);
void UIWin_CreateMenuUpdate(u32 first, u32 second, s32 third, s32 fourth);
int func_8009CF8C(int index);
void func_8009D018(int index, int value);
// Copies an entry out of a script string table into the given buffer.
char* BdatTouchStringCell(char* buf, const char* table, int index);
// BDAT row-name lookup: resolve key row in the character table.
char* BdatGetPtrDirect(const void* tbl, const void* key, int id);
// Open a party-talk window over an existing message box.
void UIWin_CreateSysWin1(char* msg, char* name, int c, int d);
// Talk-state probe (window manager): nonzero when a talk is active.
int func_8013EB90(int v);
// SE-talk voice busy probe (code80135FDC split unit).
int MenuStateCheck64064or30();
// Player accessor on the game manager (retail mangled global).
void* getPlayer__Q22cf13CfGameManagerFi(int index);
// Start/queue a party-chat line on the battle sub-object.
int func_800C4244(cf::CfObjectImplWalker* sub, u32 id, u32 flag);
void UIWin_CreateKizunaTalk(int id);
void UIWin_CreateEveTalkWin(int mode, const char* str);
// Opens a system window with the given text.
char* UIWin_CreateSysWin0(const char* str, int r4, int r5);
void* __dynamic_cast(void* src, long offset, const void* src_type,
                     const void* dst_type, void* src2dst);

int winTalk(VMThread* pThread);
int winTalkNoName(VMThread* pThread);
int fadeIn_1(VMThread* pThread);
int fadeOut_1(VMThread* pThread);
int fadeWait_1(VMThread* pThread);
int createCol6Sys(VMThread* pThread);
int createCol6Hint(VMThread* pThread);
int createCol6Invite(VMThread* pThread);
int createCol6Init(VMThread* pThread);
int checkCol6Bat(VMThread* pThread);
int simpleEventStart(VMThread* pThread);
int simpleEventEnd(VMThread* pThread);
int setTrust(VMThread* pThread);
int setItemMulti(VMThread* pThread);
int setKizunaTalk(VMThread* pThread);
int winSys(VMThread* pThread);
int winSysSelect(VMThread* pThread);
int mesGetArts(VMThread* pThread);
int pcTalk(VMThread* pThread);
int winTalkWait(VMThread* pThread);
int getSelectNum(VMThread* pThread);
int mesAddPT(VMThread* pThread);
int mesSubPT(VMThread* pThread);
int mesVisionON();
int mesVisionOFF();
int mesMonadoON();
int mesMonadoOFF();
int ptChangeNotice();
int save();
int kizunaTalkStart();
int kizunaTalkEnd();
// Select-window result probe (window manager): current select item index.
int UIWin_GetTimer();
// Busy/slot-state probe used by isPrioReq (nonzero when a priority request
// window is active).
int func_80135708();
// Fade-state probes: 1 when a fade is in progress / fade-wait pending.
BOOL func_80113E1C();
BOOL func_80113E24();
int isPrioReq(VMThread* pThread);
int gameClear(VMThread* pThread);
// Set the last-talked NPC by script index: find the row whose key equals
// the id and record its ordinal.
int setLastTalkNpc(VMThread* pThread);
int isSETalkVoiceWait(VMThread* pThread);
int func_eu_80046DA0(VMThread* pThread);
int func_eu_80046DC4(VMThread* pThread);

// Open a system select window from three strings.
void UIWin_Create25070Win(const char* a, const char* b, const char* c);
// Colosseum 6 invite: (u16 mode, u8 a, u8 b).
void UIWin_CreateCol6Invite(int a, int b, int c);
// Record the last-talk NPC ordinal.
void func_8009ECD0(int id);
// Character-table row count / keyed lookup helpers (setLastTalkNpc).
int func_8003B1EC(char* tbl);
u32 BdatGetU16Direct(char* tbl, const char* key, int idx);
// Character-table object used by setLastTalkNpc (.sbss pointer).
extern char* lbl_eu_80664098;

}
