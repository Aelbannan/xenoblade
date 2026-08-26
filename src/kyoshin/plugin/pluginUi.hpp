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

// Opaque enum-list holder (func_80043D90 ctor / __dt__80043E88 dtor /
// func_80043F18 accessor); the list it yields has an element count at +0x620.
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
// sdata2 int->double conversion magic (0x4330000080000000), referenced by
// the fadeIn/fadeOut int-to-float conversions.
extern double lbl_eu_80665DC0;

// Message string tables / buffers used by the mesVision commands.
extern char lbl_eu_804FABF0[];
extern char lbl_eu_80525D68[];

// Player RTTI source type / pointer table entry used by the talk commands.
// Complete scalar type so MWCC emits EMB_SDA21 addressing (retail `li @sda21`),
// not lis/addi (incomplete array types are not SDA-eligible).
extern u32 lbl_eu_806619A0;

// Shared BDAT character-name table pointer (.sbss); resolved by
// func_8013639C against lbl_eu_804FABF0 keys.
extern char* lbl_eu_80664090;

// Sub-object hanging off the player at +0x3ED4: carries its own vtable with
// a state probe at +0x40 (winTalkWait) and is handed to func_800C4244
// (pcTalk).
struct PcTalkSub;
// Fake interface whose vtable slot 16 (+0x40) is the talk-busy probe; casting
// the sub-object to this makes MWCC emit the retail folded r12 virtual-call
// chain instead of a staged function-pointer temp.
struct PcTalkProbeIf {
    virtual void _v00();
    virtual void _v01();
    virtual void _v02();
    virtual void _v03();
    virtual void _v04();
    virtual void _v05();
    virtual void _v06();
    virtual void _v07();
    virtual void _v08();
    virtual void _v09();
    virtual void _v10();
    virtual void _v11();
    virtual void _v12();
    virtual void _v13();
    virtual int probe(int arg); // +0x40: nonzero while the talk window is busy
};
struct PcTalkSub {
    PcTalkProbeIf* vtable; // 0x0
};
struct PcBattleTalkObj {
    u8 _00[0x3ED4];
    PcTalkSub* field_3ED4; // +0x3ED4
};

extern "C" {

void pluginUiRegist();

// Fade controller hook: (int mode, int arg, float a, float b, float c).
// The last argument is an int->float cast: MWCC's inline expansion emits
// xoris/0x43300000 word-pair + fsubs against the shared sdata2 magic
// (renamed to lbl_eu_80665DC0 by the unit's postprocess pool rule).
void func_80135464(int r3, int r4, float f1, float f2, float f3);

// Item-grant helper: 8 register args + 1 stack arg.
void func_8013E2E0(u32 a1, u32 a2, u32 a3, u32 a4, u32 a5, u32 a6, u32 a7,
                   u32 a8, u32 a9);

// Talk-window factory (window id, text, mode); used by winTalk.
void func_8013D07C(u32 obj, const char* str, int mode);

void func_80043D90(CfEnumListHolder* holder);
CfEnumList* func_80043F18(CfEnumListHolder* holder);
void __dt__80043E88(CfEnumListHolder* holder, int tags);
void func_800F4A98(void* list, int type, int value);
void* func_800F6EC0(void* list, int index);
void* func_800451D8(u32 cls, void* param);
u8 code80135FDC_getByte_64059();
void func_80138078__FUl(u32 op);
void func_8013DB6C(u32 first, u32 second, s32 third, s32 fourth);
int func_8009CF8C(int index);
void func_8009D018(int index, int value);
// Copies an entry out of a script string table into the given buffer.
char* func_80136190(char* buf, const char* table, int index);
// BDAT row-name lookup: resolve key row in the character table.
char* func_8013639C(const void* tbl, const void* key, int id);
// Open a party-talk window over an existing message box.
void func_8013D688(char* msg, char* name, int c, int d);
// Talk-state probe (window manager): nonzero when a talk is active.
int func_8013EB90(int v);
// SE-talk voice busy probe (code80135FDC split unit).
int func_eu_8013C8F4();
// Player accessor on the game manager (retail mangled global).
void* getPlayer__Q22cf13CfGameManagerFi(int index);
// Start/queue a party-chat line on the battle sub-object.
int func_800C4244(PcTalkSub* sub, u32 id, u32 flag);
void func_8013E52C(int id);
void func_8013D448(int mode, const char* str);
// Opens a system window with the given text.
char* func_8013D55C(const char* str, int r4, int r5);
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
int func_8013EC58();
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
void func_8013D978(const char* a, const char* b, const char* c);
// Colosseum 6 invite: (u16 mode, u8 a, u8 b).
void func_8013DF44(int a, int b, int c);
// Record the last-talk NPC ordinal.
void func_8009ECD0(int id);
// Character-table row count / keyed lookup helpers (setLastTalkNpc).
int func_8003B1EC(char* tbl);
u32 func_80136254(char* tbl, const char* key, int idx);
// Character-table object used by setLastTalkNpc (.sbss pointer).
extern char* lbl_eu_80664098;

}
