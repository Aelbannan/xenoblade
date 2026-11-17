#pragma once

#include <types.h>

class CFileHandle;

// Lean public header for kyoshin/code_80135FDC: the menu-state flags, the
// collected/slot lists, the item / BDAT-id lookups and the misc. game-state
// helpers of that unit. Single canonical declaration of each helper -- do NOT
// re-declare these anywhere else (C linkage: a second declaration with
// different parameter types is a hard "illegal function overloading" error).
//
// Linkage follows the retail symbol: names that are unmangled in
// config/*/symbols.txt are declared extern "C"; names whose retail symbol is
// an Itanium-mangled C++ name (playUISound__FUl, ...) are declared as plain
// C++ functions below.
//
//   row = BDAT row id (an integer; the item / character tables are keyed by
//         the caller's id, never by a string)
//
// Width rule (same as code_80135FDC_Bdat.hpp): getters return the width of
// the stored value (u8 flags, u16 cells). Retail callers that keep r3
// unextended store the result in a local of the same narrow width; callers
// that extend at receipt convert explicitly.
extern "C" {

// --- menu state flags (lbl_eu_80664058..80664080) ---
void MenuStateInitFlags();
void MenuStateSet64058();
void MenuStateClear6405A();
void MenuStateSet6405A();
void MenuStateSet64059Notify();
void MenuStateClear64064();
int MenuStateCheck64064or30();
int IsMenuState621F0();
int code80135FDC_getByte_64058();
void code80135FDC_clearByte_64058();
int code80135FDC_getByte_64059();
void code80135FDC_clearByte_64059();
u8 code80135FDC_getByte_6405A();
u8 code80135FDC_getByte_6405B();
void code80135FDC_setByte_6405B();
void code80135FDC_setPair_6405C_64060(u32 a, u32 b);
u32 code80135FDC_getWord_6405C();
u32 code80135FDC_getWord_64060();
void code80135FDC_setByte_64064();
u8 code80135FDC_getByte_6407F();
u8 code80135FDC_setByte_6407F(u8 value);
// Menu nesting counter (lbl_eu_80664080): post-increment / clamped
// post-decrement; both return the previous value.
u8 code80135FDC_getByte_64080();
u8 code80135FDC_postIncByte_64080();
u8 DecMenuCounter64080();

// --- system state flags (Class_80296898 instance bytes) ---
bool GetSysStateFlag10();
int GetSysStateFlag11();
int GetSysStateFlag20();
int GetSysStateFlag21();
int GetSysStateFlag22();
int GetSysStateFlag23();
int GetSysStateFlag24();
int GetSysStateFlag25();
int GetSysStateFlag31();
// Tests remote bit (id + 0x2CC8).
int CheckState2CC8Active(u32 id);

// --- party slot lists (lbl_eu_80664070 u8[7] / lbl_eu_80664078 u16[3]) ---
// Rebuilds both lists from the slot table; a non-zero `skipFlagged` skips
// characters whose +0x176C byte is 1.
void BuildSlotItemLists(int skipFlagged);
u8 code80135FDC_getByte_64077(); // u8 list length
u8 code80135FDC_getByte_6407E(); // u16 list length
u8 GetCollectedFlagByte(u32 idx);
u8 GetCollectedFlagWord8(u32 idx);

// --- item / character BDAT lookups ---
char* MakeTplNameSysFile(u32 row);
char* MakeIconTplName(u32 row);
// u16 cells: retail callers keep r3 unextended and mask at use, so callers
// hold the result in a u16 local (a u32 local forces clrlwi at receipt).
u16 BdatGetItemType(u32 row);
u16 BdatGetItemId(u32 row);
// BDAT file of the item's type table (0 when the item table is not loaded).
u32 LookupItemTypeTable(u32 row);
char* BuildItemNameStr(u32 row);
int BdatGetSexFlag(u32 row);
u32 MapSexFlagGroup(u32 row);
u32 BdatGetRankByName(u32 row);
// `str` is a decimal row id; returns the name cell for a positive id,
// otherwise `str` itself.
char* GetBdatNameByNumId(const char* str);
u8 MapValueToRank6(int value);
u32 SearchThresholdIdx(u32 value);
u16 GetU16PairEntry(u32 idx);
f32 GetFloatTableEntry(u32 idx);
f32 BlendFloatAvgScale(u32 a, u32 b, u32 c, f32 scale);
u32 func_80138574(const char* name, u32 id);
u32 CheckEntryConditions(const char* name, u32 id);
void ItemBlockSetDefaultFlags();

// --- affinity (relate) table ---
u16 LookupRelateValue(u16 a, u16 b);
void AdjustRelateValue(u16 a, u16 b, s8 delta);
void FetchRelatePair(u8* outA, u8* outB, int idx);

// --- map reveal ---
int GetMapItemBitFlag(u8 max, u8 count, u32 offset);
void RevealMinimapArea();
void TryRevealMapArea(u8 area);
void code80135FDC_thunk_BFE8C(u8* arg);

} // extern "C"

// --- retail C++-linkage helpers (mangled retail symbols) ---
void playUISound(u32 id);
void incrementEventCounter(u32 value);
void closeFileHandle(CFileHandle** handle);
