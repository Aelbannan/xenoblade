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
extern const void* lbl_eu_806618D8;
extern const void* lbl_eu_806618F0;

// sdata2 constant for fadeOut_1 (fade alpha value); lbl_eu_80665DC0 is the
// MWCC int->float conversion magic (0x4330000080000000), auto-synthesised.
extern f32 lbl_eu_80665DB8;

extern "C" {

void pluginUiRegist();

// Fade controller hook: (int mode, int arg, float a, float b, float c).
void func_80135464(int r3, int r4, float f1, float f2, float f3);

// Item-grant helper: 8 register args + 1 stack arg.
void func_8013E2E0(u32 a1, u32 a2, u32 a3, u32 a4, u32 a5, u32 a6, u32 a7,
                   u32 a8, u32 a9);

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
void* __dynamic_cast(void* src, long offset, const void* src_type,
                     const void* dst_type, void* src2dst);

void winTalk();
void pcTalk();
void winTalkWait();
void winTalkNoName();
void fadeIn_1();
int fadeOut_1(VMThread* pThread);
void fadeWait_1();
int createCol6Sys(VMThread* pThread);
int createCol6Hint(VMThread* pThread);
void createCol6Invite();
int createCol6Init(VMThread* pThread);
int checkCol6Bat(VMThread* pThread);
int simpleEventStart(VMThread* pThread);
int simpleEventEnd(VMThread* pThread);
int setTrust(VMThread* pThread);
int setItemMulti(VMThread* pThread);
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
