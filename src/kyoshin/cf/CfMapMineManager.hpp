#pragma once

#include "kyoshin/cf/CfBdat.hpp"

#include <types.h>
#include "kyoshin/code_8025FB10.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (gmScene; gmGameFlags)
#include "kyoshin/cf/object/CfObject.hpp"
#include "monolib/math/CVec3.hpp"
class UnkClass_800821F8;

#include "kyoshin/cf/CItemImpl.hpp"  // item-impl virtual interface (owner)

// ---------------------------------------------------------------------------
// C-linkage imports
// ---------------------------------------------------------------------------

class BdatFilePointer;
struct CfMapMineManager;

// External runtime / game functions (retail-unmangled symbols)
#include "kyoshin/plugin/ocBdat.hpp"  // getBdatStringColumnValue (owner)
#include "kyoshin/CUICfManager.hpp"
extern "C" u32 Bdat_GetMaxRow_B1EC(void* file);
extern "C" u32 Bdat_GetRowBase_B41C(void* file);
extern "C" int CfRes_getD80Flag();
extern "C" u32 CfRes_getAllocHandle();
#include "monolib/util/MemManager.hpp"
extern "C" void* __construct_new_array(void* block, void* ctor, void* dtor,
                                       u32 size, u32 n);
#include "monolib/math/Random.hpp"
#include "monolib/core/CPadManager.hpp"
extern "C" int lookupWorkAtAddr(void* obj);
extern "C" void gmFileObject(void* obj);
extern "C" void* ArtsSelect_GetEntry(int id);
// CItem_initItemImplInstances: canonical decl from CfGameManager.hpp.
extern "C" void CItemData_enableFamFlag(void* item);
extern "C" void CItemData_callVf0C(void* item, u16 v);
extern "C" void CItem_resolveFamilyBdat(u16 v, void* outA, void* outB);
extern "C" int* CtrlObjectParam_GetSlotTableBase();
extern "C" void* CtrlObjectParam_GetCharEntry(u32 index);
#include "kyoshin/cf/CtrlRemoteApi.hpp"
extern "C" void* CItemData_lookupOrAlloc(void* item, int v);
extern "C" void* getInstance__Fv();
extern "C" void* spawnMaskedObject(void* a, u32 b, u32 c, u32 d);
extern "C" void ObjPoint_SetName_13FC(void* obj, void* name, u32 len);
extern "C" void setChildB59__(void* obj, s8 val);

extern "C" int UIWin_HasPendingUpdate(int v);
extern "C" int CMainMenu_IsOpen();
extern "C" void* createNpcActor__Q22cf13CfGameManagerFv(u32 value);

extern "C" UnkClass_800821F8* getCameraDataBlock__Q22cf13CfGameManagerFv();
extern "C" int isSceneReadyForInput__Q22cf13CfGameManagerFv();
extern "C" int isAnyFieldFlagSet__Q22cf13CfGameManagerFv();
extern "C" u32 getEventValue40__Q22cf13CfGameManagerFv(u32 id);
extern "C" void setEventManagerValue__Q22cf13CfGameManagerFv(u32 id, u32 value);
extern "C" void queueEventId__Q22cf13CfGameManagerFv(u32 id);

// External data (retail linker symbols)
extern "C" CfMapMineManager* sMineManagerInstance;   // sbss: singleton instance
extern "C" u16 gmGlobalFlag42;                 // sbss: current area id
extern "C" u16 gmGlobalFlag44;                 // sbss: current area sub-id
extern "C" s32 sMineNameLength;                 // sbss: cached name length
extern "C" s8 sMineNameInitialized;                  // sbss: one-time init flag
extern "C" char sMineItemNameScratch[8];            // sdata: scratch name buffer
extern "C" u8 sMineRangeBounds[8];              // sdata: range-pair table
extern "C" u32 sMineLotteryBias[2];             // sdata: lottery bias table
extern "C" char lbl_eu_80508424[];              // rodata: bdat column names
extern "C" char lbl_eu_80535720[];              // data: scratch name buffer 2
extern "C" u8 lbl_eu_8053572C[];                // data: reslist vtable
extern "C" u8 lbl_eu_80535744[];                // data: _reslist_base vtable
extern "C" const f64 lbl_eu_806682D8;           // rodata: 0.0 double literal
