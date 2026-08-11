#pragma once

#include <types.h>
class UnkClass_800821F8;
class CItemImplInstances;

// ---------------------------------------------------------------------------
// C-linkage imports
// ---------------------------------------------------------------------------

class BdatFilePointer;
struct CfMapMineManager;

// External runtime / game functions (retail-unmangled symbols)
extern "C" u32 getBdatStringColumnValue(void* file,
                                                const char* column, s32 index);
extern "C" u32 func_8003B1EC(void* file);
extern "C" u32 func_8003B41C(void* file);
extern "C" int CfRes_getD80Flag();
extern "C" f32 func_80496288(void*);
extern "C" u32 func_80061FFC();
extern "C" const f32 lbl_eu_806682B0;
extern "C" void* allocate_array__Q23mtl10MemManagerFUlUl(u32 size, u32 handle);
extern "C" void* __construct_new_array(void* block, void* ctor, void* dtor,
                                       u32 size, u32 n);
extern "C" void __dl__FPv(void* ptr);
extern "C" void __dla__FPv(void* ptr);
extern "C" int mtRand__Q22ml4mathFi(int n);
extern "C" int mtRand__Q22ml4mathFii(int lo, int hi);
extern "C" void* func_800B8920(void* obj);
extern "C" void func_800B9404(void* obj);
extern "C" void* func_80186BC8(int id);
extern "C" void func_800BFBF4(u16 id, int mode);
extern "C" void func_80462E58__8CTaskLODFv(u32 a, u32 b, f32 c);
extern "C" void func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(u32 a, u32 b, u32 c,
                                                           u32 d, f32 e);
extern "C" CItemImplInstances* CItem_initItemImplInstances();
extern "C" void func_801583E0(void* item);
extern "C" void func_801570A0(void* item, u16 v);
extern "C" void func_80157F04(u16 v, void* outA, void* outB);
extern "C" int* func_8009ECB0();
extern "C" void* func_8009EC9C(u32 index);
extern "C" u32 func_8009CF8C(u32 v);
extern "C" int func_8026178C(void* data, u32 flag);
extern "C" u32 func_8025FB10(void* data, u32 flag);
extern "C" void* func_801599D4(void* item, int v);
extern "C" void func_801352A4(void* mgr);
extern "C" void* func_800B07E8__Fv();
extern "C" void* func_800B20B4(void* a, u32 b, u32 c, u32 d);
extern "C" void func_800C13FC(void* obj, void* name, u32 len);
extern "C" void func_800ACC14(void* obj, int mode);
extern "C" void func_800ACF78(void* obj, void* player, u32 v);
extern "C" int func_8013EB90(int v);
extern "C" int func_800FF738();
extern "C" void func_8013D55C(void* a, u32 b, u32 c);
extern "C" void* func_800FE68C();
extern "C" void* func_8008187C__Q22cf13CfGameManagerFv(u32 value);
extern "C" void* func_8007F91C__Q22cf13CfGameManagerFv();
extern "C" int func_80084BF4__Q22cf13CfGameManagerFv();
extern "C" UnkClass_800821F8* func_800821F8__Q22cf13CfGameManagerFv();
extern "C" int func_80085840__Q22cf13CfGameManagerFv();
extern "C" u32 func_80082694__Q22cf13CfGameManagerFv(u32 id);
extern "C" void func_8008269C__Q22cf13CfGameManagerFv(u32 id, u32 value);
extern "C" void func_800826F0__Q22cf13CfGameManagerFv(u32 id);
extern "C" void* getFP__FPCc(const char* name);

// External data (retail linker symbols)
extern "C" CfMapMineManager* lbl_eu_806646A0;   // sbss: singleton instance
extern "C" u32 lbl_eu_80663E24;                 // sbss: game state flags
extern "C" u16 lbl_eu_80663E42;                 // sbss: current area id
extern "C" u16 lbl_eu_80663E44;                 // sbss: current area sub-id
extern "C" BdatFilePointer* lbl_eu_806640C8;    // sbss: bdat file handle
extern "C" s32 lbl_eu_806646A4;                 // sbss: cached name length
extern "C" s8 lbl_eu_806646A8;                  // sbss: one-time init flag
extern "C" char lbl_eu_80662758[];              // sdata: scratch name buffer
extern "C" u8 lbl_eu_80662750[];                // sdata: range-pair table
extern "C" u32 lbl_eu_80662760[];               // sdata: lottery bias table
extern "C" char lbl_eu_80508424[];              // rodata: bdat column names
extern "C" char lbl_eu_80535720[];              // data: scratch name buffer 2
extern "C" u8 lbl_eu_8053572C[];                // data: reslist vtable
extern "C" u8 lbl_eu_80535744[];                // data: _reslist_base vtable
