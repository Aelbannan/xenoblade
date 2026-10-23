#pragma once

// TU-private imports for kyoshin/cf/chain/CChainActorList.cpp (moved out of the public
// CChainActorList.hpp, which keeps the classes).

#include "kyoshin/cf/chain/CChainActorList.hpp"
#include "kyoshin/cf/chain/CChainEffectImports.hpp"
// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void chainResolveMemberPtr(int, void*);
extern "C" u32 UIWin_GetInstance();
extern "C" void UIWin_Create6F8B0Win(int);
extern "C" int lbl_eu_80662A80;
extern "C" float lbl_eu_8050EDE0[4];
// Retail .sdata2 zero-float pair (this TU defines it); code reads element [0].
extern float lbl_eu_80668A80[2];

// Tuning-pool constants used by Chain_CheckChanceRoll plus its u16 id table.
// Retail object is 16 bytes (8 entries); the bound spells the retail size so
// MWCC emits exactly one 0x10 object into .rodata (see the tables at EOF).
extern unsigned short lbl_eu_8050EDD0[8];
extern const float lbl_eu_80668A68;
extern const float lbl_eu_80668A6C;
extern const float lbl_eu_80668A70;
extern const double lbl_eu_80668A78;
extern "C" int lookupWorkAtAddr(void* addr);
extern "C" int isBattleStatusActive(void*, int);
extern "C" void addTableValueWithClamp__Q22cf13CfGameManagerFv(int, int, int);
// CfGameManager pair-value query (retail Fv-form symbol; call sites pass
// two ids even though the suffix does not reflect them) and the tuning-value
// accessor (owner: code_8025FB10.cpp). Single winning decls: extern "C" s32
// form matches CfGameManagerUnityHelpers.hpp; int(void*,u32) form matches
// CChainTimer.hpp so both headers co-exist without rename guards.
namespace cf { class CChainBattleObj; }
extern "C" s32 getTableValueByPair__Q22cf13CfGameManagerFv(s32 firstId, s32 secondId);
extern "C" int IdTable_SumValues(void* data, u32 flag);
extern "C" int GetZealObjPtr(void);
extern "C" int GetZealResult(int);
extern "C" int isClassicController__Q22cf13CfGameManagerFv(int arg);
extern "C" int getArtsSlotRC(const void* arts, short index, short subindex);
extern "C" void* getArtsParamRC2(const void* arts, int index, int subindex);
extern "C" void SysWinLog_QueueEvent(int);
extern "C" u32 SysWinLog_BumpEventValue(int, int);
// Chain-activation dice roll (defined in this TU). Retail callers emit a
// real bl to the unmangled label Chain_CheckChanceRoll (MWCC_CASES CBattleMan_OnActorsEmpty
// record), so it keeps C linkage here like the other imports above; the
// definition in the .cpp inherits it from this declaration.
extern "C" int Chain_CheckChanceRoll(cf::CChainChanceS* self,
                             cf::CChainBattleObj* objA,
                             cf::CChainBattleObj* objB);
extern "C" __declspec(noinline) void ScnLog_ClearStateWords(cf::CChainActorList* self);
extern "C" cf::CChainActor* SysWinLog_AllocFreeSlot(cf::CChainActorList* self);
extern "C" void Chain_InsertOrdered(cf::CChainActorList* self, cf::CChainActor* actor);
extern "C" u32 getAnimModelId(void* battleObj);

// Tail view of the battle object (shared with CChain.cpp / CSuddenCommu):
// an embedded sub-object with a manual vtable sits at +0x3E9C, and +0x3F60
// holds a battle-object pointer queried by getAnimModelId. This TU now calls
// the same slots through cf::CChainBattleObj (CChainTimer.hpp) instead.
class CChainBattleObjTail {
public:
    u8 field_0[0x3E9C];  //0x0
    u32 field_0x3E9C;    //0x3E9C embedded sub-object (vtable at its +0)
    u8 field_0x3EA0[0x3F60 - 0x3EA0];
    u32 field_0x3F60;    //0x3F60
};

// (Former CChainSub4/CChainVoiceSub pad removed: slots are on the owning objects -
// +0x30 is cf::CChainSub4::f30(), +0x4C is cf::CChainVoiceSub::v17()
// in CChainTimer.hpp. Call sites in CChainActorList.cpp now use those.)

// Manual vtable objects stored by the Pc/Ene constructors (retail .data).
// Declared at their full retail sizes (0x78 / 0x10) so MWCC emits the
// lis/addi HA-LO address form; a small (<8B) extern type would otherwise
// use sda21 addressing and break the reloc match (MWCC_CASES sec 833).
struct CChainVtblActor {
    u8 bytes[0x78];
};
struct CChainVtblSub {
    u8 bytes[0x10];
};
extern "C" u32 lbl_eu_80538290[29];  // defined in CChainActorList.cpp (retail 0x78 table, 4-aligned form)
extern CChainVtblActor lbl_eu_805384E0;
extern CChainVtblActor lbl_eu_80538458;
extern CChainVtblSub lbl_eu_80538338;

// _reslist_base<CChainActor*> base vtable (retail .data), restored into the
// mChainActorList member's vptr slot by its inlined base destructor.
extern u8 lbl_eu_8053832C[];

// Explicit call target for the CChainEffect ctor: retail calls it at the END
// of the CChainActorPc/Ene ctor bodies, and MWCC rejects explicit ctor-call
// syntax (10409), so the call is made through this free-function alias. The
// extern "C" keeps the reloc name unmangled (hexdiff reloc-drift fix).
extern "C" void __ct__Q22cf12CChainEffectFv(cf::CChainEffect* p);
