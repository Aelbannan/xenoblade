#pragma once

#include <types.h>
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)
class UnkClass_800821F8;
class CItemImplInstances;

// Cast-only vtable interfaces for the CItemImplInstances object returned by
// CItem_initItemImplInstances(item). MWCC prepends 2 RTTI header entries, so the
// first virtual sits at vtable+0x08.
struct CItemInstVt1C {
    virtual void _v08();
    virtual void _v0C();
    virtual void _v10();
    virtual void _v14();
    virtual void _v18();
    virtual void _v1C(void* item);  // vtable+0x1C
};
struct CItemInstVt50 {
    virtual u32 _v08(void* item);
    virtual void _v0C();
    virtual void _v10();
    virtual void _v14();
    virtual void _v18();
    virtual void _v1C();
    virtual char* _v20(void* item);  // item name getter
    virtual void _v24();
    virtual void _v28();
    virtual void _v2C();
    virtual void _v30();
    virtual void _v34();
    virtual void _v38();
    virtual void _v3C();
    virtual void _v40();
    virtual void _v44();
    virtual void _v48();
    virtual void _v4C();
    virtual void _v50(void* item, int slot, int value);  // vtable+0x50
};
struct CItemInstVt68 {
    virtual void _v08();
    virtual void _v0C();
    virtual void _v10();
    virtual void _v14();
    virtual void _v18();
    virtual void _v1C();
    virtual void _v20();
    virtual void _v24();
    virtual void _v28();
    virtual void _v2C();
    virtual void _v30();
    virtual void _v34();
    virtual void _v38();
    virtual void _v3C();
    virtual void _v40();
    virtual void _v44();
    virtual void _v48();
    virtual void _v4C();
    virtual void _v50();
    virtual void _v54();
    virtual void _v58();
    virtual void _v5C();
    virtual void _v60();
    virtual void _v64();
    virtual void _v68(void* item, int slot, int value);  // vtable+0x68
};

// ---------------------------------------------------------------------------
// C-linkage imports
// ---------------------------------------------------------------------------

class BdatFilePointer;
struct CfMapMineManager;

// External runtime / game functions (retail-unmangled symbols)
#include "kyoshin/plugin/ocBdat.hpp"  // getBdatStringColumnValue (owner)
extern "C" u32 func_8003B1EC(void* file);
extern "C" u32 func_8003B41C(void* file);
extern "C" int CfRes_getD80Flag();
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
extern "C" void setLODParam__8CTaskLODFv(u32 a, u32 b, f64 c);
extern "C" u16 playActorSound__Q22cf10CfSoundManFUlUlUlUlf(u32 a, u32 b, u32 c,
                                                          u32 d, f32 e);
extern "C" void* CItem_initItemImplInstances(void* item);
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
extern "C" void* getInstance__Fv();
extern "C" void* func_800B20B4(void* a, u32 b, u32 c, u32 d);
extern "C" void func_800C13FC(void* obj, void* name, u32 len);
extern "C" void setChildB59__(void* obj, int mode);
extern "C" void bindPartnerO_(void* obj, void* player, u32 v);
extern "C" int func_8013EB90(int v);
extern "C" int func_800FF738();
extern "C" void func_8013D55C(void* a, u32 b, u32 c);
extern "C" void* func_800FE68C();
extern "C" void* createNpcActor__Q22cf13CfGameManagerFv(u32 value);
extern "C" void* isTimerActive__Q22cf13CfGameManagerFv();
extern "C" int isAnyFieldFlagSet__Q22cf13CfGameManagerFv();
extern "C" UnkClass_800821F8* getCameraDataBlock__Q22cf13CfGameManagerFv();
extern "C" int isSceneReadyForInput__Q22cf13CfGameManagerFv();
extern "C" u32 getEventValue40__Q22cf13CfGameManagerFv(u32 id);
extern "C" void setEventManagerValue__Q22cf13CfGameManagerFv(u32 id, u32 value);
extern "C" void queueEventId__Q22cf13CfGameManagerFv(u32 id);
extern "C" void* getFP__FPCc(const char* name);

// External data (retail linker symbols)
extern "C" CfMapMineManager* lbl_eu_806646A0;   // sbss: singleton instance
extern "C" u16 lbl_eu_80663E42;                 // sbss: current area id
extern "C" u16 lbl_eu_80663E44;                 // sbss: current area sub-id
extern "C" BdatFilePointer* lbl_eu_806640C8;    // sbss: bdat file handle
extern "C" s32 lbl_eu_806646A4;                 // sbss: cached name length
extern "C" s8 lbl_eu_806646A8;                  // sbss: one-time init flag
extern "C" char lbl_eu_80662758[8];            // sdata: scratch name buffer
extern "C" u8 lbl_eu_80662750[8];              // sdata: range-pair table
extern "C" u32 lbl_eu_80662760[2];             // sdata: lottery bias table
extern "C" char lbl_eu_80508424[];              // rodata: bdat column names
extern "C" char lbl_eu_80535720[];              // data: scratch name buffer 2
extern "C" u8 lbl_eu_8053572C[];                // data: reslist vtable
extern "C" u8 lbl_eu_80535744[];                // data: _reslist_base vtable
extern "C" const f32 lbl_eu_806682B0;           // rodata: 0.0f literal
extern const f32 lbl_eu_806682B4;               // sdata2: 1.0e-4f
extern const f64 lbl_eu_806682B8;               // sdata2: 2^52 + 2^31 (int->double fixup)
extern const f32 lbl_eu_806682C8;               // rodata: 50.0f
extern const f32 lbl_eu_806682CC;               // rodata: 40000.0f
extern const f32 lbl_eu_806682D0;               // rodata: 2.5f
extern const f32 lbl_eu_806682D4;               // rodata: 6.25f
extern const f32 lbl_eu_806682E0;               // rodata: sound timer reset value
extern const f32 lbl_eu_806682E4;               // rodata: 0.6f
extern const f32 lbl_eu_806682E8;               // rodata: small y offset
extern const f32 lbl_eu_806682EC;               // rodata: respawn timer scale
extern const f32 lbl_eu_806682F0;               // rodata: respawn timer scale 2
extern const f32 lbl_eu_806682F4;               // rodata: message display time
extern "C" const f64 lbl_eu_806682D8;           // rodata: 0.0 double literal
extern "C" const f64 lbl_eu_806682C0;               // rodata: 160.0 double literal
extern "C" const f32 lbl_eu_8066A208;           // monolibdata1 sdata2: 1.0e-6f timer epsilon
