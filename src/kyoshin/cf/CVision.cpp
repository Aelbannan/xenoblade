// Auto-scaffolded catalog TU for kyoshin/cf/CVision
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/cf/CBattleManagerApi.hpp"
#include "kyoshin/cf/CfMapItemManager.hpp"
#include <types.h>
#include "monolib/scn/CScnTimeApi.hpp"
#include <PowerPC_EABI_Support/Runtime/MWCPlusLib.h>
#include <new>
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)
// Forward decl: CfGameManager.hpp's `static cf::CfGameManager* init(...)` relies
// on the class name being declared before its include chain is entered.
namespace cf { class CfGameManager; }
#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CTaskGameApi.hpp"
// (CUIWindowManagerApi.hpp omitted: conflicts with this TU's closure.)
extern "C" void* func_801412D0(u32 target);

#include "kyoshin/cf/CVision.hpp"
#include "kyoshin/cf/CArtsSet.hpp"
#include "kyoshin/cf/object/CBattleState.hpp"

// Forward decls for the slot-array construction in CVision::CVision(). The
// retail names are address-mangled (__ct__801A33AC / __dt__801A36D0), so they
// are kept as C-linkage free functions with typed `self` params (same idiom
// as __dt__801A3728 below) to keep the __construct_array reloc names exact.
extern "C" void* __ct__801A33AC(cf::CVisionSlot* self);
extern "C" void* __dt__801A36D0(cf::UnkClass_801A36D0* self, int deleting);

// ---------------------------------------------------------------------------
// Include-order workarounds (CMenuArtsSelect.cpp idiom: rename the later
// header's symbols so its C-linkage decls don't overload-conflict with the
// chain headers pulled in by CBattleManager.hpp).
//
// CSuddenCommu.hpp's imports now match the chain headers (see CSuddenCommu.hpp);
// the symbols this TU uses (lbl_eu_80663E24,
// func_80496288, func_8006EF04__Fi, func_80260264) resolve to the
// chain-header / local declarations instead.
//
// func_8016FE34 is declared `void* (void*)` by CAIAction.hpp and
// CChainTimer.hpp but takes an int id at the retail call sites here, so those
// two headers are pre-included under a rename and CVision.cpp keeps its
// (int id) declaration.
#include "kyoshin/cf/CSuddenCommu.hpp"
#define func_8016FE34 visionCppChainActorLookup
// CAIAction.hpp recently switched func_8009EC9C to (u16); CVision.hpp's
// (u32) import is what this TU uses (identical call-site codegen). Rename
// the CAIAction form out of the way (MWCC 10197).
#define func_8009EC9C visionCppAiActionEc9cUnused
// CAIAction.hpp also gained a 2-param void* getBdatStringColumnValue; the
// canonical 3-param u32 decl comes from plugin/ocBdat.hpp (via
// harness_catalog.hpp). Rename theirs out of the way too.
#define getBdatStringColumnValue visionCppAiActionBdatColUnused
// ...and a third: u16 getArtsSlotRC(void*,int,int) vs CChainActorList.hpp's
// int(const void*,short,short). Unused here; rename past the include.
#define getArtsSlotRC visionCppAiActionArtsSlotRcUnused
// ...and getArtsParamRC2 (const void* vs void* first param - illegal
// overloading under C linkage).
#define getArtsParamRC2 visionCppAiActionArtsParamRc2Unused
// CAIAction.hpp declares func_80174C98 as void(const void*,void*,int), which
// conflicts with CSuddenCommu.hpp's int(void*,int*,int) form (MWCC 10197).
// This TU uses the CSuddenCommu form; rename the other out of the way.
#include "kyoshin/cf/object/CAIAction.hpp"
#undef func_8009EC9C
#undef getBdatStringColumnValue
#undef getArtsSlotRC
#undef getArtsParamRC2
#undef func_8016FE34
#define func_8016FE34 visionCppChainTimerActorLookup
#define func_8025FB10 visionCppChainTimer25fb10Unused
#include "kyoshin/cf/chain/CChainTimer.hpp"
#undef func_8025FB10
#undef func_8016FE34
#include "kyoshin/cf/CBattleManager.hpp"

using namespace cf;

// Named .sdata2 conversion magics: defining them lets MWCC's constant pool
// reuse the retail symbols for the (double) casts instead of emitting TU-local
// @N labels (CMiniMap / CfResReloadImpl idiom). 80667D28 is the signed
// int->float magic (2^52 + 2^31); 80667D58 the unsigned one (2^52).
extern "C" const double lbl_eu_80667D28 = 0x4330000080000000ll;
extern "C" const double lbl_eu_80667D58 = 0x4330000000000000ll;

// Most C-linkage pseudo-imports for this TU now live in the "C-linkage
// imports" section of kyoshin/cf/CVision.hpp (real imports). The few kept
// inline below stay because sibling TUs that include this header declare the
// same retail symbols with different signatures or stub definitions -- moving
// them here would break those TUs' builds (see the task report).

// Fragment-anchored destructors for the UnkClass_801A36D0 / UnkClass_801A3728
// sub-objects are defined below and passed by address to __destroy_arr.

// Kept inline (not moved to CVision.hpp): sibling TUs that include this
// header declare the same retail symbols with conflicting signatures/stubs.
extern "C" void func_800EA484(cf::CBattleManager* bm, f32 volume, int id); // CBattleManager.cpp re-declares (float,u32) + defines stub
extern "C" int func_80148778(void* obj, int id);   // CBattleManager.cpp declares with C++ linkage
extern "C" void func_801AD504(int id);             // CMenuVision.cpp defines an empty stub
extern "C" void* func_8016FE34(int id);            // 4 TUs declare a void* -param version
extern "C" void func_802A1DF0(u32 a);              // CCharVoiceMan.cpp defines an empty stub
extern "C" int func_80260264(void* self, int id, void* result); // matches CSuddenCommu.hpp / CBattleManager.cpp

// Kept inline (not moved to CVision.hpp): sibling TUs declare/stub these.
// func_8009ECB0 / func_8009E344 / func_800A32C4 / func_8009EC9C are declared
// in the imports section of kyoshin/cf/CVision.hpp -- sibling TUs use those
// symbols with different signatures or define stubs, so the shared decls live
// in the common header instead of being repeated inline here.
extern "C" void func_800E921C(void* a, void* b, void* c, void* d, void* e); // CBattleManager.cpp defines an empty stub

// Per-battle-slot object iterated in func_801A897C (stride 0x834).
struct CVisionBtlSlot {
    u32 w_00;       // 0x00
    u32 w_04;       // 0x04
    u8 unk08[0x88 - 0x08];
    u32 w_88;       // 0x88
    u8 unk8C[0x824 - 0x8C];
    u32 w_824;      // 0x824
    u32 w_828;      // 0x828
    u32 w_82C;      // 0x82C
};

// Parameter block used by func_801A6540's FX-setup path. The mid member
// (+0x04..+0x11) is re-cleared by its own memset in retail.
struct CVisionFxParamMid {
    u8 pad0[2];     // +0x04
    u8 b_06;        // +0x06
    u8 pad07[6];
    u8 b_0D;        // +0x0D
    u8 pad0E[2];
    u16 h_10;       // +0x10
};                  // 0xE bytes, ends +0x12
struct CVisionFxParam {
    u32 w_00;               // 0x00 (filled by func_8014B8BC)
    CVisionFxParamMid mid;  // 0x04
    u16 h_12;               // 0x12
    f32 f_14;               // 0x14
    CVisionSubRef* obj18;   // 0x18 (filled by func_8014B8BC)
};

// Kept inline (not moved to CVision.hpp): sibling TUs stub these.
extern "C" void func_802A1FB4(void* a, void* b); // CCharVoiceMan.cpp defines an empty stub
extern "C" void func_801AF934(u32 a);            // CMenuVision.cpp defines an empty stub

// Object handed to func_801A5E58 by func_800F477C (reads byte at 0x42).
struct CVisionRefObj {
    u8 unk0[0x42];  // 0x00
    s8 b_42;         // 0x42 (signed: retail compares with cmpi)
    u8 unk43[0x78 - 0x43];
    u32 w_78;        // 0x78
};
// Kept inline (not moved to CVision.hpp): sibling TUs declare these.
extern "C" CVisionRefObj* func_800F477C(void* self); // pluginUnit.cpp declares CfUnknownSub* version
extern "C" bool func_8006EF04__Fi(int mask);         // CfObjectActor.hpp/CChain.hpp declare bool(int); s32=long would clash

// Object passed to func_801A6A7C / func_801A380C-family (battle object).
struct CVisionObj {
    u8 unk00[0x08];
    u8 field_08;           //0x08 (region handed to func_80148778)
    u8 unk09[0x3F00 - 0x09];
    u32 field_3F00;        //0x3F00 flag bits
    u8 unk3F04[0x3F10 - 0x3F04];
    u32 field_3F10;        //0x3F10 id/ptr
    u8 unk3F14[0x3F28 - 0x3F14];
    u16 field_3F28;        //0x3F28
    u8 unk3F2A[0x4FFC - 0x3F2A];
};

// Parameter objects traversed by func_801A5BA8 via the fusion vtable.
class CVisionPrmHolder;
class CVisionPrm;

// Scan cursor for func_801A70DC's free-slot search: walks from the CVision
// base (so the per-slot id lands at +0x04) with the full 0x834 slot stride.
class CVisionSlotScan {
public:
    u8 pad0[0x4];
    u32 id;                 // 0x04 (= CVisionSub::field_00)
    u8 pad8[0x834 - 0x8];
};

// Typed view of CBattleManager's vtable slot 0x34 so MWCC emits the standard
// r12 virtual dispatch instead of a raw function-pointer call.
class CVisionBMView {
public:
    virtual void f000();
    virtual void f004();
    virtual void f008();
    virtual void f00C();
    virtual void f010();
    virtual void f014();
    virtual void f018();
    virtual void f01C();
    virtual void f020();
    virtual void f024();
    virtual void f028();
    virtual void f02C();
    virtual void vt30(u32 id, f32 a, f32 b);   // index 11 -> 0x30
    virtual void vt34(u32 id, f32 a, f32 b);   // index 12 -> 0x34
};

// Result of func_8016FE34 (per-model battle state). The retail v20 call
// dispatches at vtable offset 0x20; the vtable carries 2 leading slots plus
// 7 declared virtuals (v20 = 7th, index 8).
class CVisionFusionSub {
public:
    virtual void v08();
    virtual void v0C();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1C();
    virtual void v20(u32 r4);   // 0x20
    // Filler slots keep the named entry at its retail vtable offset
    // (declared index N -> vtable offset (N+2)*4).
    virtual void f024(); virtual void f028(); virtual void f02C(); virtual void f030();
    virtual void f034(); virtual void f038(); virtual void f03C(); virtual void f040();
    virtual void f044(); virtual void f048(); virtual void f04C(); virtual void f050();
    virtual void f054(); virtual void f058(); virtual void f05C(); virtual void f060();
    virtual void f064(); virtual void f068(); virtual void f06C(); virtual void f070();
    virtual void f074(); virtual void f078(); virtual void f07C(); virtual void f080();
    virtual void f084(); virtual void f088(); virtual void f08C(); virtual void f090();
    virtual void f094(); virtual void f098(); virtual void f09C(); virtual void f0A0();
    virtual void f0A4(); virtual void f0A8(); virtual void f0AC(); virtual void f0B0();
    virtual void f0B4(); virtual void f0B8(); virtual void f0BC(); virtual void f0C0();
    virtual void f0C4(); virtual void f0C8(); virtual void f0CC(); virtual void f0D0();
    virtual void f0D4(); virtual void f0D8(); virtual void f0DC(); virtual void f0E0();
    virtual void f0E4(); virtual void f0E8(); virtual void f0EC(); virtual void f0F0();
    virtual void f0F4(); virtual void f0F8(); virtual void f0FC(); virtual void f100();
    virtual void f104(); virtual void f108(); virtual void f10C(); virtual void f110();
    virtual void f114(); virtual void f118(); virtual void f11C(); virtual void f120();
    virtual void f124(); virtual void f128(); virtual void f12C(); virtual void f130();
    virtual void f134(); virtual void f138(); virtual void f13C(); virtual void f140();
    virtual void f144(); virtual void f148(); virtual void f14C(); virtual void f150();
    virtual void f154(); virtual void f158(); virtual void f15C(); virtual void f160();
    virtual void f164(); virtual void f168(); virtual void f16C(); virtual void f170();
    virtual void f174(); virtual void f178(); virtual void f17C(); virtual void f180();
    virtual void f184(); virtual void f188(); virtual void f18C(); virtual void f190();
    virtual void f194(); virtual void f198(); virtual void f19C(); virtual void f1A0();
    virtual void f1A4(); virtual void f1A8(); virtual void f1AC(); virtual void f1B0();
    virtual void f1B4(); virtual void f1B8(); virtual void f1BC(); virtual void f1C0();
    virtual void f1C4(); virtual void f1C8(); virtual void f1CC(); virtual void f1D0();
    virtual void f1D4(); virtual void f1D8(); virtual void f1DC(); virtual void f1E0();
    virtual void f1E4(); virtual void f1E8(); virtual void f1EC(); virtual void f1F0();
    virtual void f1F4(); virtual void f1F8(); virtual void f1FC(); virtual void f200();
    virtual void f204(); virtual void f208(); virtual void f20C(); virtual void f210();
    virtual void f214(); virtual void f218(); virtual void f21C(); virtual void f220();
    virtual void f224(); virtual void f228(); virtual void f22C(); virtual void f230();
    virtual void f234(); virtual void f238(); virtual void f23C(); virtual void f240();
    virtual void f244(); virtual void f248(); virtual void f24C(); virtual void f250();
    virtual void f254(); virtual void f258(); virtual void f25C(); virtual void f260();
    virtual void f264(); virtual void f268(); virtual void f26C(); virtual void f270();
    virtual void f274(); virtual void f278(); virtual void f27C(); virtual void f280();
    virtual void f284(); virtual void f288(); virtual void f28C();
    virtual void* vf290();      // 0x290
    virtual void f294(); virtual void f298(); virtual void f29C(); virtual void f2A0();
    // Named slots used by func_801A5BA8 (declared index N -> vtable (N+2)*4).
    virtual CVisionPrmHolder* vf2A4();  // 0x2A4 -> index 167
    virtual s32 vf2A8(CVisionPrm* prm); // 0x2A8 -> index 169
    virtual void f2AC(); virtual void f2B0(); virtual void f2B4(); virtual void f2B8();
    virtual s32 vf2BC();        // 0x2BC -> index 175
    // Fillers up to the slots used by func_801A4CF8.
    virtual void f2C0(); virtual void f2C4(); virtual void f2C8(); virtual void f2CC();
    virtual void f2D0(); virtual void f2D4(); virtual void f2D8(); virtual void f2DC();
    virtual void f2E0(); virtual void f2E4(); virtual void f2E8(); virtual void f2EC();
    virtual void f2F0(); virtual void f2F4(); virtual void f2F8(); virtual void f2FC();
    virtual void f300(); virtual void f304(); virtual void f308(); virtual void f30C();
    virtual void f310(); virtual void f314(); virtual void f318(); virtual void f31C();
    virtual void f320(); virtual void f324(); virtual void f328(); virtual void f32C();
    virtual void f330(); virtual void f334(); virtual void f338(); virtual void f33C();
    virtual void f340(); virtual void f344(); virtual void f348(); virtual void f34C();
    virtual void f350(); virtual void f354(); virtual void f358(); virtual void f35C();
    virtual void f360(); virtual void f364(); virtual void f368(); virtual void f36C();
    virtual void f370(); virtual void f374(); virtual void f378(); virtual void f37C();
    virtual void f380(); virtual void f384(); virtual void f388(); virtual void f38C();
    virtual void f390(); virtual void f394(); virtual void f398(); virtual void f39C();
    virtual void f3A0(); virtual void f3A4(); virtual void f3A8(); virtual void f3AC();
    virtual void f3B0(); virtual void f3B4(); virtual void f3B8(); virtual void f3BC();
    virtual void f3C0(); virtual void f3C4(); virtual void f3C8(); virtual void f3CC();
    virtual void f3D0(); virtual void f3D4(); virtual void f3D8(); virtual void f3DC();
    virtual void f3E0(); virtual void f3E4(); virtual void f3E8(); virtual void f3EC();
    virtual void f3F0(); virtual void f3F4(); virtual void f3F8(); virtual void f3FC();
    virtual void f400(); virtual void f404(); virtual void f408(); virtual void f40C();
    virtual void f410(); virtual void f414(); virtual void f418(); virtual void f41C();
    virtual void f420(); virtual void f424(); virtual void f428(); virtual void f42C();
    virtual void f430(); virtual void f434(); virtual void f438(); virtual void f43C();
    virtual void f440(); virtual void f444(); virtual void f448(); virtual void f44C();
    virtual void f450(); virtual void f454(); virtual void f458(); virtual void f45C();
    virtual void f460(); virtual void f464(); virtual void f468(); virtual void f46C();
    virtual void f470(); virtual void f474(); virtual void f478(); virtual void f47C();
    virtual void f480(); virtual void f484(); virtual void f488(); virtual void f48C();
    virtual void f490(); virtual void f494(); virtual void f498(); virtual void f49C();
    virtual void f4A0(); virtual void f4A4(); virtual void f4A8(); virtual void f4AC();
    virtual void f4B0(); virtual void f4B4(); virtual void f4B8(); virtual void f4BC();
    virtual void f4C0(); virtual void f4C4(); virtual void f4C8(); virtual void f4CC();
    virtual void f4D0(); virtual void f4D4(); virtual void f4D8(); virtual void f4DC();
    virtual void f4E0(); virtual void f4E4(); virtual void f4E8(); virtual void f4EC();
    virtual void f4F0(); virtual void f4F4(); virtual void f4F8(); virtual void f4FC();
    virtual void f500(); virtual void f504(); virtual void f508(); virtual void f50C();
    virtual void f510(); virtual void f514(); virtual void f518(); virtual void f51C();
    virtual void f520(); virtual void f524(); virtual void f528(); virtual void f52C();
    virtual void f530(); virtual void f534(); virtual void f538(); virtual void f53C();
    virtual void f540(); virtual void f544(); virtual void f548(); virtual void f54C();
    virtual void f550(); virtual void f554(); virtual void f558(); virtual void f55C();
    virtual void f560(); virtual void f564(); virtual void f568(); virtual void f56C();
    virtual void f570(); virtual void f574(); virtual void f578(); virtual void f57C();
    virtual void f580(); virtual void f584(); virtual void f588(); virtual void f58C();
    virtual void f590(); virtual void f594(); virtual void f598(); virtual void f59C();
    virtual void f5A0(); virtual void f5A4(); virtual void f5A8(); virtual void f5AC();
    virtual void f5B0(); virtual void f5B4(); virtual void f5B8();
    virtual void f5B8_2();
    // Slots dispatched by func_801A4CF8.
    virtual f32 vf5BC();        // 0x5BC
    virtual void vf5C4(f32 a);  // 0x5C4
    virtual void vf5C8(u32 a);  // 0x5C8
};

// Result of the polymorphic sub-object's 0xAC vtable slot (dispatched by
// func_801A4CF8): the caller reads a remaining-time f32 at +0x04.
struct CVisionAcResult {
    u8 pad0[4];
    f32 time;       // +0x04
};

// Typed view of the polymorphic sub-object embedded at CVisionFusion+0x3E9C
// (slot 0xAC dispatched by func_801A4CF8).
class CVisionFusionSubObj {
public:
    virtual void f000(); virtual void f004(); virtual void f008(); virtual void f00C();
    virtual void f010(); virtual void f014(); virtual void f018(); virtual void f01C();
    virtual void f020(); virtual void f024(); virtual void f028(); virtual void f02C();
    virtual void f030(); virtual void f034(); virtual void f038(); virtual void f03C();
    virtual void f040(); virtual void f044(); virtual void f048(); virtual void f04C();
    virtual void f050(); virtual void f054(); virtual void f058(); virtual void f05C();
    virtual void f060(); virtual void f064(); virtual void f068(); virtual void f06C();
    virtual void f070(); virtual void f074(); virtual void f078(); virtual void f07C();
    virtual void f080(); virtual void f084(); virtual void f088(); virtual void f08C();
    virtual void f090(); virtual void f094(); virtual void f098();
    virtual CVisionAcResult* vfAC();    // 0xAC
};

struct CVisionFusion {
    u8 unk0[0x4];
    CVisionFusionSub* field_04; //0x4 vtable-holder
    u8 unk08[0x3380 - 0x08];
    void* field_3380;           //0x3380
    u8 unk3384[0x3388 - 0x3384];
    u16 field_3388;             //0x3388
    u8 unk338A[0x3E9C - 0x338A];
    void* field_3E9C;           //0x3E9C vtable
    u8 unk3EA0[0x3F00 - 0x3EA0];
    u32 field_3F00;             //0x3F00 flags
    u8 unk3F04[0x3F60 - 0x3F04];
    void* field_3F60;           //0x3F60
    u8 unk3F64[0x4FFC - 0x3F64];
};


extern "C" u32 lbl_eu_805332D0[];

cf::CVision::CVision() {
    // novtable class: store the retail vtable manually (MWCC's auto store
    // resolves to the IObjectInfo base vtable -> reloc name drift).
    *(u32**)this = lbl_eu_805332D0;
    // Initialise the four CVisionSub battle slots (0x4..0x20D4, 0x834 stride):
    // field stores, two memset regions, the 10 x 0xBC item array, then the
    // per-slot release callback. Retail hoists one sdata2 zero into a saved FPR.
    f32 zero = lbl_eu_80667CD4;
    CVisionSub* p = &sub;
    CVisionSub* pEnd = p + 4;
    do {
        p->field_84 = 0;
        for (u32* wp = &p->field_88; wp < &p->field_8C; wp++) *wp = 0;
        p->field_10 = 0;
        p->field_14 = 0;
        p->field_58 = 0;
        p->field_5C = -1;
        p->field_60 = 0;
        p->f_64 = zero; p->f_68 = zero; p->f_6C = zero; p->f_70 = zero; p->f_74 = zero;
        p->field_8C = 0;
        p->field_90 = 0;
        p->field_C8 = 0;
        p->f_78 = zero; p->f_7C = zero;
        p->h_80 = 0; p->h_82 = 0;
        memset(p->unk18, 0, 0x40);
        memset(p->unk94, 0, 0x34);
        p->field_84 = 0;
        p->field_88 = 0;
        CVisionInitBlock* ip = p->items;
        CVisionInitBlock* ipEnd = ip + 10;
        do {
            CVisionInitBlock& it = *ip;
            it.w_74pair[0] = 0;
            for (u32* wp = &it.w_74pair[1]; wp < &it.w_7C; wp++) *wp = 0;
            it.w_00 = 0;
            it.w_04 = 0;
            it.w_48 = 0;
            it.w_4C = -1;
            it.w_50 = 0;
            it.f_54 = zero; it.f_58 = zero; it.f_5C = zero; it.f_60 = zero; it.f_64 = zero;
            it.w_7C = 0;
            it.h_80 = 0;
            it.w_B8 = 0;
            it.f_68 = zero; it.f_6C = zero;
            it.h_70 = 0; it.h_72 = 0;
            memset(it.unk_08, 0, 0x40);
            memset(it.unk_84, 0, 0x34);
            it.w_74pair[0] = 0;
            it.w_74pair[1] = 0;
            ip++;
        } while (ip < ipEnd);
        p->field_824 = 0;
        func_800F449C(p);
        p++;
    } while (p < pEnd);

    // The eight 0x4818-byte vision slots, via the retail slot ctor/dtor.
    __construct_array(&unk20D4[0], (void*)&__ct__801A33AC,
                      (void*)&__dt__801A36D0, sizeof(CVisionSlot), 8);

    // unk261C4 head (u32/f32/u32 triplet) plus seven more at 0xC stride.
    // The entry-testing for-loop matches the retail cmplw/bge + mtctr/bdnz.
    CVisionU32F32U32* upEnd = (CVisionU32F32U32*)((u8*)this + 0x26224);
    f32 zero261 = lbl_eu_80667CD4;
    unk261C4.w0 = 0;
    unk261C4.f4 = zero261;
    unk261C4.w8 = 0;
    for (CVisionU32F32U32* up = unk261C4.arr; up != upEnd; up++) {
        up->a = 0;
        up->b = zero261;
        up->c = 0;
    }
    // NOTE (repo-proven): a `<` pointer-walk over this member array makes
    // MWCC emit its unknown-count memset template WITH an 8x-unroll fast path
    // (+0xF8 bytes, retail has none); an `!=` walk emits the plain loop that
    // matches retail's shape much more closely (26% vs 19.8%).

    // unk261C4 tail (self pointer / count) then the vision field resets.
    f32 zeroFields = lbl_eu_80667CD4;
    unk261C4.w6C = 8;
    *(u32*)upEnd = (u32)&unk261C4; // w60: retail reuses the loop-end pointer
    unk261C4.field_68 = 0;
    unk261C4.field_64 = 0;
    field_26194 = zeroFields;
    field_26198 = zeroFields;
    field_2619C = zeroFields;
    field_261A0 = zeroFields;
    field_261A4 = 0;
    memset(effectArray, 0, sizeof(effectArray));

    // Install the null ptmf callback and hand it to __ptmf_test. The fader
    // reads/writes are interleaved with the callback copy in retail.
    CVisionPtmf cb;
    u32* np = __ptmf_null;
    cb.mPfn = *np++;
    cb.mObj = *np++;
    cb.mDelta = *np;
    f32 f194 = field_26194;
    mPtmf.mPfn = cb.mPfn;
    mPtmf.mObj = cb.mObj;
    mPtmf.mDelta = cb.mDelta;
    f32 zTail = lbl_eu_80667CD4;
    field_2619C = zTail;
    field_261A0 = f194;
    __ptmf_test(&cb);
}
// ---------------------------------------------------------------------------
// us-801a4acc: CVisionSlot constructor (retail __ct__801A33AC).
// Each 0x4818-byte vision slot carries a CBattleState, two func_801751DC
// sub-objects, a CArtsSet (vtable at +0x34) and CAttackSet (vtable at +0x0),
// plus the same 0xBC-byte init blocks as CVisionSub. Member construction is
// manual (placement new / direct calls) so the retail call order is kept.
// ---------------------------------------------------------------------------
extern "C" void* __ct__801A33AC(CVisionSlot* self) {
    self->field_00 = (u32)&lbl_eu_8052813C[0];
    self->field_04 = 0;
    self->field_08 = 0;
    self->field_0C = 0;
    __ct__Q22cf12CBattleStateFv(&self->unk10[0]);
    func_801751DC(self->unk15F0);
    func_801751DC(self->unk1668);
    memset(self->unk16E0, 0, 0x1520);
    // One cursor/end pair drives every sub-object loop (retail reuses the
    // same register pair throughout).
    u8* ptr;
    u8* end;
    end = self->blocks2C00 + 0xC0;
    ptr = self->blocks2C00;
    do {
        memset(ptr, 0, 0x18);
        ptr += 0x18;
    } while (ptr < end);
    __ct__Q22cf9_sArtsSetFv(&self->unk2CC8[0]);
    self->field_2CFC = (u32)&lbl_eu_8052F5C0[0];
    ptr = self->unk2D00;
    end = self->unk2D00 + 0xD20;
    do {
        __ct__Q22cf10CArtsParamFv((cf::CArtsParam*)ptr);
        ptr += 0x8C;
    } while (ptr < end);
    self->field_3A2C = (u32)&lbl_eu_8052F5A8[0];
    ptr = self->unk3A30;
    end = self->unk3A30 + 0x330;
    do {
        __ct__Q22cf12CAttackParamFv((cf::CAttackParam*)ptr);
        ptr += 0x88;
    } while (ptr < end);
    // One zero variable spans the tail; MWCC splits its live range like
    // retail (r28 for the item array loop, r30 from block4720 on).
    u32 zero = 0;
    u8* q = self->unk3D8C, *qEnd = self->unk3D8C + 0x200;
    do {
        memset(q + 4, 0, 0xe);
        memset(q, 0, 0x20);
        q += 0x20;
    } while (q < qEnd);
    self->field_3F98 = 0x10;
    *(u32*)qEnd = (u32)self->unk3D8C; // qEnd == &field_3F8C; retail reuses the loop-end reg
    self->field_3F94 = zero;
    self->field_3F90 = zero;
    memset(self->unk3FA0, 0, 0xe);
    memset(self->unk3F9C, 0, 0x20);
    // Item array: retail hoists one sdata2 zero load into a saved FPR.
    f32 zeroItems = lbl_eu_80667CD4;
    ptr = (u8*)self->items, end = (u8*)self->items + 10 * 0xBC;
    do {
        CVisionInitBlock& it = *(CVisionInitBlock*)ptr;
        it.w_74pair[0] = zero;
        for (u32* wp = &it.w_74pair[1]; wp < &it.w_7C; wp++) *wp = 0;
        it.w_00 = 0;
        it.w_04 = 0;
        it.w_48 = 0;
        it.w_4C = -1;
        it.w_50 = 0;
        it.f_54 = zeroItems; it.f_58 = zeroItems; it.f_5C = zeroItems;
        it.f_60 = zeroItems; it.f_64 = zeroItems;
        it.w_7C = 0;
        it.h_80 = 0;
        it.w_B8 = 0;
        it.f_68 = zeroItems; it.f_6C = zeroItems;
        it.h_70 = 0; it.h_72 = 0;
        memset(it.unk_08, 0, 0x40);
        memset(it.unk_84, 0, 0x34);
        it.w_74pair[0] = zero;
        it.w_74pair[1] = 0;
        ptr += 0xBC;
    } while (ptr < end);
    CVisionInitBlock& b = self->block4720;
    // Fresh literal zero here: retail rematerialises (r3) after the loop.
    b.w_74pair[0] = 0;
    for (u32* wp = &b.w_74pair[1]; wp < &b.w_7C; wp++) *wp = 0;
    // Fresh sdata2 load (volatile f0 in retail); zt is the tail zero (r30).
    f32 zeroBlock = lbl_eu_80667CD4;
    b.w_00 = zero;
    b.w_04 = zero;
    b.w_48 = zero;
    b.w_4C = -1;
    b.w_50 = zero;
    b.f_54 = zeroBlock; b.f_58 = zeroBlock; b.f_5C = zeroBlock;
    b.f_60 = zeroBlock; b.f_64 = zeroBlock;
    b.w_7C = zero;
    b.h_80 = 0;
    b.w_B8 = zero;
    b.f_68 = zeroBlock; b.f_6C = zeroBlock;
    b.h_70 = 0; b.h_72 = 0;
    memset(&b.unk_08, 0, 0x40);
    memset(&b.unk_84, 0, 0x34);
    b.w_74pair[0] = zero;
    b.w_74pair[1] = zero;
    func_80174658(self->unk47EC);
    // Slot tail resets (retail reloads the sdata2 zero after the call and
    // stores f_15E8, h_15EE, h_15EC, ... in that order).
    f32 zeroTail = lbl_eu_80667CD4;
    self->f_15E8 = zeroTail;
    self->h_15EE = zero;
    self->h_15EC = zero;
    self->w_2CC4 = zero;
    self->w_2CC0 = zero;
    self->f_3D6C = zeroTail;
    self->w_3FBC = zero;
    self->f_3FC0 = zeroTail;
    self->w_4800 = zero;
    self->f_4808 = zeroTail;
    self->w_4810 = zero;
    self->w_4804 = zero;
    self->f_480C = zeroTail;
    self->w_4814 = zero;
    return self;
}

// ---------------------------------------------------------------------------
// us-801a4f2c: Initialise all vision slots / effects (retail func_801A380C).
// Retail keeps self+0x20000 hoisted into a saved GPR for the tail fields and
// resets the eight vision slots via a 2-iteration loop whose body covers four
// slots each (trio + mid-field group per slot, in ctor store order).
// ---------------------------------------------------------------------------
void func_801A380C(CVision* self) {
    f32 zero = lbl_eu_80667CD4;
    CVisionRingBase* R = (CVisionRingBase*)((u8*)self + 0x20000);
    R->f_6194 = zero;
    R->f_6198 = zero;
    R->f_619C = zero;
    R->f_61A0 = zero;
    R->b_61A4 = 0;

    // Install the null ptmf callback: copied through a stack local first.
    CVisionPtmf cb;
    u32* np = __ptmf_null;
    R->mPtmf.mPfn = cb.mPfn = *np++;
    cb.mObj = *np++;
    cb.mDelta = *np;
    R->mPtmf.mObj = cb.mObj;
    R->mPtmf.mDelta = cb.mDelta;
    R->f_619C = zero;
    R->f_61A0 = zero;
    __ptmf_test(&cb);

    self->vt_34();

    // Reset the four CVisionSub battle slots (0x834 stride).
    CVisionSub* pSub = &self->sub;
    for (int i = 0; i < 4; i++) {
        func_800F449C(pSub);
        pSub++;
    }

    // Reset the eight vision slots: two iterations x four slots.
    f32 zSlot = lbl_eu_80667CD4;
    CVisionSlot* s = self->unk20D4;
    for (int k = 0; k < 2; k++) {
        // Four slots per iteration, written out so MWCC emits the same
        // straight-line store run (moving base pointer) as retail.
        CVisionSlot* c = s;
        c->f_15E8 = zSlot;
        c->h_15EE = 0;
        c->h_15EC = 0;
        c->w_2CC4 = 0;
        c->w_2CC0 = 0;
        c->f_3D6C = zSlot;
        c->w_3FBC = 0;
        c->f_3FC0 = zSlot;
        c->w_4800 = 0;
        c->f_4808 = zSlot;
        c->w_4810 = 0;
        c->w_4804 = 0;
        c->f_480C = zSlot;
        c->w_4814 = 0;
        c++;
        c->f_15E8 = zSlot;
        c->h_15EE = 0;
        c->h_15EC = 0;
        c->w_2CC4 = 0;
        c->w_2CC0 = 0;
        c->f_3D6C = zSlot;
        c->w_3FBC = 0;
        c->f_3FC0 = zSlot;
        c->w_4800 = 0;
        c->f_4808 = zSlot;
        c->w_4810 = 0;
        c->w_4804 = 0;
        c->f_480C = zSlot;
        c->w_4814 = 0;
        c++;
        c->f_15E8 = zSlot;
        c->h_15EE = 0;
        c->h_15EC = 0;
        c->w_2CC4 = 0;
        c->w_2CC0 = 0;
        c->f_3D6C = zSlot;
        c->w_3FBC = 0;
        c->f_3FC0 = zSlot;
        c->w_4800 = 0;
        c->f_4808 = zSlot;
        c->w_4810 = 0;
        c->w_4804 = 0;
        c->f_480C = zSlot;
        c->w_4814 = 0;
        c++;
        c->f_15E8 = zSlot;
        c->h_15EE = 0;
        c->h_15EC = 0;
        c->w_2CC4 = 0;
        c->w_2CC0 = 0;
        c->f_3D6C = zSlot;
        c->w_3FBC = 0;
        c->f_3FC0 = zSlot;
        c->w_4800 = 0;
        c->f_4808 = zSlot;
        c->w_4810 = 0;
        c->w_4804 = 0;
        c->f_480C = zSlot;
        c->w_4814 = 0;
        s = c + 1;
    }
}

// ---------------------------------------------------------------------------
// us-801a50f8: Drive the vision-slot FX update loop from the ring element
// array (retail func_801A39D8). Each queued element decays by the per-frame
// step; while it is still above the threshold the matching flag bit is raised
// on the vision sub-object and the element is removed from the circular ring
// (shifting whichever half of the ring is shorter), then the installed
// callback is dispatched and its fade progress advanced.
// ---------------------------------------------------------------------------
void func_801A39D8(CVision* self) {
    CVisionSub* sub = (self->sub.field_00 == 0) ? 0 : &self->sub;
    if (sub == 0) {
        // Null path goes through CVision's own members (each address is
        // recomputed from self, matching retail's addis/addi pairs).
        if (self->unk261C4.field_68 != 0) {
            self->unk261C4.field_68 = 0;
            self->unk261C4.field_64 = 0;
        }
        if (__ptmf_test(&self->mPtmf) != 0) {
            self->vt_1C();
        }
        self->field_261A4 = 0;
        return;
    }
    getInstance__Q22cf13CfGameManagerFv();
    if (func_8006EF04__Fi(0x40000) != 0) {
        return;
    }
    getInstance__Q22cf13CfGameManagerFv();
    switch (CfRes_getE24Bit22()) {
    case 0: {
    // All tail-block state is addressed as direct CVision members; MWCC hoists
    // the shared high base (self+0x20000) into a register itself.
    // Per-frame constants cached in registers across the update loop.
    f32 decay = lbl_eu_80667CDC;
    f32 zero = lbl_eu_80667CD4;
    u32 i = 0;
    while (i < self->unk261C4.field_68) {
        // Ring slot for element i, offset by the write cursor field_64.
        CVisionU32F32U32* el =
            &((CVisionU32F32U32*)self->unk261C4.w60)[(self->unk261C4.field_64 + i) % self->unk261C4.w6C];
        el->b -= decay * func_80496288(lbl_eu_80663E14);
        if (!(zero < el->b)) {
            i++;
            continue;
        }
        switch (el->a) {
        case 1: func_801AD504(1); sub->field_824 |= 0x20; break;
        case 2: func_801AD504(2); sub->field_824 |= 0x40; break;
        case 3: func_801AD504(4); sub->field_824 |= 0x80; break;
        case 4: func_801AD504(8); sub->field_824 |= 0x100; break;
        case 5: func_801AD504(0x10); sub->field_824 |= 0x200; break;
        }
        // Re-read the ring state after the calls above (they may repoint it).
        CVisionU32F32U32* es = (CVisionU32F32U32*)self->unk261C4.w60;
        int cnt = self->unk261C4.field_68;
        int n = self->unk261C4.w6C;
        int cursor = self->unk261C4.field_64;
        CVisionU32F32U32 removed = es[(cursor + i) % n];
        self->unk261C4.field_68 = cnt - 1;
        // Remove element i by shifting the shorter side of the ring over it:
        // past the midpoint shift the tail up (gap falls off the end), before
        // it shift the head up and advance the cursor instead.
        if ((int)i >= (cnt - 1) / 2) {
            for (int j = i; j < cnt - 1; j++) {
                es[(cursor + j) % n] = es[((cursor + j) % n + 1) % n];
            }
        } else {
            for (int j = i - 1; j >= 0; j--) {
                es[(cursor + j) % n] = es[((cursor + j) % n + 1) % n];
            }
            self->unk261C4.field_64 = (cursor + 1) % n;
        }
    }

    if (__ptmf_test(&self->mPtmf) != 0) {
        f32 step = lbl_eu_80667CDC * func_80496288(lbl_eu_80663E14);
        self->field_26198 = self->field_26194;
        self->field_26194 += step;

        // Dispatch the installed member callback (retail __ptmf_scall).
        typedef void (CVision::*VisionCb)();
        (self->*(*reinterpret_cast<const VisionCb*>(&self->mPtmf)))();

        // When the callback is the fade-out entry, play a sound each time the
        // fade timer crosses an integer boundary.
        CVisionPtmf chk = *(CVisionPtmf*)&lbl_eu_80533128[0];
        if (__ptmf_cmpr(&self->mPtmf, &chk) == 0) {
            if ((int)self->field_26198 != (int)self->field_26194) {
                func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(0, 0x1c1, 0, 0,
                                                           lbl_eu_80667CE0);
            }
        }
        if (self->field_261A0 != self->field_26194) {
            self->field_2619C += step;
        }
    }

    CVisionSub* subEnd = (self->sub.field_00 != 0) ? &self->sub : 0;
    if (subEnd == 0) {
        for (int pi = 0; pi < 3; pi++) {
            CVisionFusion* p =
                (CVisionFusion*)func_8016FE34(
                    (int)getPlayer__Q22cf13CfGameManagerFi(pi));
            if (p != 0) {
                p->field_3388 &= ~0x8u;
            }
        }
        CVisionPtmf cb = *(CVisionPtmf*)&__ptmf_null[0];
        self->mPtmf = cb;
        self->field_2619C = lbl_eu_80667CD4;
        self->field_261A0 = self->field_26194;
        __ptmf_test(&cb);
    }
    }
    break;
    }
}

void func_801A4194(CVision* self) {
    CVisionSub* sub;
    if (self->sub.field_00 == 0) {
        sub = 0;
    } else {
        sub = &self->sub;
    }
    CVisionFusion* p1 = (CVisionFusion*)func_8016FE34(func_800B708C__Fi(sub->field_00));
    CVisionFusion* p2 = (CVisionFusion*)func_8016FE34(func_800B708C__Fi(sub->field_04));
    if (p1 == 0 || p2 == 0) {
        self->vt_20(1);
        return;
    }
    // Retail materializes the equality test via mfcr/extrwi (value context).
    bool atBase = (lbl_eu_80667CD4 == self->field_2619C);
    if (atBase) {
        func_8006E2FC(0);
        CBattleManager* bm = CBattleManager::getInstance();
        ((void (*)(void*))(*(void***)bm)[0x38 / 4])(bm);
    }
    // Materialized timer-window checks (retail evaluates each into r0).
    int w1;
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CD4 &&
        lbl_eu_80667CD4 < self->field_26194 - self->field_261A0) {
        w1 = 1;
    } else {
        w1 = 0;
    }
    if (w1) {
        func_800EA484(CBattleManager::getInstance(), lbl_eu_80667CE4, 3);
    }
    int w2;
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CE4 &&
        lbl_eu_80667CE4 < self->field_26194 - self->field_261A0) {
        w2 = 1;
    } else {
        w2 = 0;
    }
    if (w2) {
        func_800EA484(CBattleManager::getInstance(), lbl_eu_80667CD4, 3);
    }
    int w3;
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CE4 &&
        lbl_eu_80667CE4 < self->field_26194 - self->field_261A0) {
        w3 = 1;
    } else {
        w3 = 0;
    }
    if (w3) {
        self->vt_2C(0);
        func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(0, 0x1be, 0, 0, lbl_eu_80667CE0);
    }
    int w4;
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CE8 &&
        lbl_eu_80667CE8 < self->field_26194 - self->field_261A0) {
        w4 = 1;
    } else {
        w4 = 0;
    }
    if (w4) {
        u8 vdlist[8];
        func_80043D90(vdlist);
        func_800F4A98(func_80043F18(vdlist), 0x328, 0);
        u32 i = 0;
        while (i < *(u32*)((u8*)func_80043F18(vdlist) + 0x620)) {
            void* elem = func_800F6EAC(func_80043F18(vdlist), i);
            void* obj = __dynamic_cast(elem, 0, &lbl_eu_806618E8, &lbl_eu_806618F0, 0);
            if (obj == 0) {
                self->vt_20(1);
                __dt__80043E88(vdlist, -1);
                return;
            }
            u32 id = *(u32*)((u8*)obj + 0x74);
            if (id != sub->field_00 && id != sub->field_04) {
                ((void (*)(void*, u32))(*(void***)obj)[0x190 / 4])(obj, 1);
            }
            i++;
        }
        __dt__80043E88(vdlist, -1);
        if (getGlobalSda() != 0) {
            getGlobalSda()[0xa8] = 1;
        }
        self->vt_2C(1);
        func_801C01A8(0, 0x2, lbl_eu_80667CE4);
        func_801C01A8(1, 0x0, lbl_eu_80667CE4);
        func_802A1DF0(1);
        CVisionPtmf cb;
        cb.mPfn = lbl_eu_80533134[0];
        cb.mObj = lbl_eu_80533134[1];
        cb.mDelta = lbl_eu_80533134[2];
        self->mPtmf = cb;
        self->field_2619C = lbl_eu_80667CD4;
        self->field_261A0 = self->field_26194;
        __ptmf_test(&cb);
    }
    if (((void* (*)(void*))(*(void***)p1)[0x2BC / 4])(p1) != 0 ||
        ((void* (*)(void*))(*(void***)p2)[0x2BC / 4])(p2) != 0) {
        self->vt_1C();
    }
}

// ---------------------------------------------------------------------------
// us-801a5c98: Per-player vision callback management (retail func_801A4578).
// ---------------------------------------------------------------------------
void func_801A4578(CVision* self) {
    CVisionFusion* p1;
    CVisionFusion* p2;
    CVisionSub* sub;
    if (self->sub.field_00 == 0) {
        sub = 0;
    } else {
        sub = &self->sub;
    }
    if (sub->field_824 & 0x40000) {
        p1 = (CVisionFusion*)func_8016FE34(func_800B708C__Fi(sub->field_00));
        p2 = (CVisionFusion*)func_8016FE34(func_800B708C__Fi(sub->field_04));
        if (p1 == 0 || p2 == 0) {
            self->vt_20(1);
            return;
        }
        // Retail materializes the equality test via mfcr/extrwi (value context).
        bool atBase = (lbl_eu_80667CD4 == self->field_2619C);
        if (atBase) {
            func_800EA484(CBattleManager::getInstance(), lbl_eu_80667CEC, 3);
        }
        // Materialized timer-window checks (retail evaluates both into r0).
        int w1;
        if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CF0 &&
            lbl_eu_80667CF0 < self->field_26194 - self->field_261A0) {
            w1 = 1;
        } else {
            w1 = 0;
        }
        if (w1) {
            func_80133F48(5, lbl_eu_80667CF0);
        }
        if (sub->field_824 & 0x80000) {
            u32* src = &lbl_eu_80533140.mPfn;
            u32 dlt, obj, pfn;
            pfn = *src++;
            obj = *src++;
            dlt = *src++;
            CVisionPtmf cb;
            cb.mPfn = pfn;
            cb.mObj = obj;
            cb.mDelta = dlt;
            f32 f_6194 = self->field_26194;
            f32 f_cd4 = lbl_eu_80667CD4;
            self->mPtmf.mPfn = pfn;
            self->mPtmf.mObj = obj;
            self->mPtmf.mDelta = cb.mDelta;
            self->field_2619C = f_cd4;
            self->field_261A0 = f_6194;
            __ptmf_test(&cb);
        }
        int w2;
        if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CF4 &&
            lbl_eu_80667CF4 < self->field_26194 - self->field_261A0) {
            w2 = 1;
        } else {
            w2 = 0;
        }
        if (w2) {
        u32* src = &lbl_eu_8053314C.mPfn;
        u32 dlt, obj, pfn;
        pfn = *src++;
        obj = *src++;
        dlt = *src++;
        CVisionPtmf cb;
        cb.mPfn = pfn;
        cb.mObj = obj;
        cb.mDelta = dlt;
        f32 f_6194 = self->field_26194;
        f32 f_cd4 = lbl_eu_80667CD4;
        self->mPtmf.mPfn = pfn;
        self->mPtmf.mObj = obj;
        self->mPtmf.mDelta = cb.mDelta;
        self->field_2619C = f_cd4;
        self->field_261A0 = f_6194;
        __ptmf_test(&cb);
    }
        if (((void* (*)(void*))(*(void***)p1)[0x2BC / 4])(p1) != 0 ||
            ((void* (*)(void*))(*(void***)p2)[0x2BC / 4])(p2) != 0) {
            self->vt_1C();
        }
    }
}

void func_801A47D0(CVision* self) {
    CVisionFusion* p2 = 0;
    CVisionFusion* p1 = 0;
    CVisionSub* sub;
    if (self->sub.field_00 == 0) {
        sub = 0;
    } else {
        sub = &self->sub;
    }
    p1 = (CVisionFusion*)func_8016FE34(func_800B708C__Fi(sub->field_00));
    p2 = (CVisionFusion*)func_8016FE34(func_800B708C__Fi(sub->field_04));
    if (p1 == 0 || p2 == 0) {
        self->vt_20(1);
        return;
    }
    // Retail materializes the equality test via mfcr/extrwi (value context).
    bool atBase = (lbl_eu_80667CD4 == self->field_2619C);
    if (atBase) {
        void* mem = getHandleMEM2__Q23mtl10MemManagerFv();
        func_8004392C(1, 0x12f, (u32)mem, 2, 1, lbl_eu_80667CF8);
    }
    // Materialized timer-window checks (retail evaluates each into r0).
    int w1;
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CD4 &&
        lbl_eu_80667CD4 < self->field_26194 - self->field_261A0) {
        w1 = 1;
    } else {
        w1 = 0;
    }
    if (w1) {
        func_800EA484(CBattleManager::getInstance(), lbl_eu_80667CE4, 3);
    }
    int w2;
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CE4 &&
        lbl_eu_80667CE4 < self->field_26194 - self->field_261A0) {
        w2 = 1;
    } else {
        w2 = 0;
    }
    if (w2) {
        func_800EA484(CBattleManager::getInstance(), lbl_eu_80667CD4, 3);
    }
    int w3;
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CE4 &&
        lbl_eu_80667CE4 < self->field_26194 - self->field_261A0) {
        w3 = 1;
    } else {
        w3 = 0;
    }
    if (w3) {
        self->vt_2C(0);
        func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(0, 0x1be, 0, 0, lbl_eu_80667CE0);
        func_80081E90__Q22cf13CfGameManagerFv(0, 0, 0);
    }
    int w4;
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CE8 &&
        lbl_eu_80667CE8 < self->field_26194 - self->field_261A0) {
        w4 = 1;
    } else {
        w4 = 0;
    }
    if (w4) {
        u8 vdlist[8];
        func_80043D90(vdlist);
        func_800F4A98(func_80043F18(vdlist), 0x328, 0x1);
        u32 i = 0;
        while (i < *(u32*)((u8*)func_80043F18(vdlist) + 0x620)) {
            void* elem = func_800F6EAC(func_80043F18(vdlist), i);
            void* obj = __dynamic_cast(elem, 0, &lbl_eu_806618E8, &lbl_eu_806618F0, 0);
            if (obj == 0) {
                self->vt_20(1);
                __dt__80043E88(vdlist, -1);
                return;
            }
            u32 id = *(u32*)((u8*)obj + 0x74);
            if (sub->field_00 != id && sub->field_04 != id) {
                ((CVisionEnumElem*)obj)->vf190(1);
            }
            i++;
        }
        __dt__80043E88(vdlist, -1);
        if (getGlobalSda() != 0) {
            u8* sda = getGlobalSda();
            sda[0xa8] = 1;
        }
        self->vt_2C(1);
        func_801C01A8(0, 0x2, lbl_eu_80667CE4);
        func_801C01A8(1, 0x0, lbl_eu_80667CE4);
        func_802A1DF0(1);
        CVisionPtmf cb;
        // Retail walks the triplet with an updating load (lwzu).
        u32* src = &lbl_eu_80533158[0];
        u32 pfn = *src++;
        u32 obj = *src++;
        u32 dlt = *src;
        cb.mPfn = pfn;
        cb.mObj = obj;
        cb.mDelta = dlt;
        f32 f_6194 = self->field_26194;
        f32 f_cd4 = lbl_eu_80667CD4;
        self->mPtmf.mPfn = pfn;
        self->mPtmf.mObj = obj;
        // Third word is re-read from the stack copy in retail.
        self->mPtmf.mDelta = cb.mDelta;
        self->field_2619C = f_cd4;
        self->field_261A0 = f_6194;
        __ptmf_test(&cb);
    }
    if (((CVisionFusionSub*)p1)->vf2BC() != 0 ||
        ((CVisionFusionSub*)p2)->vf2BC() != 0) {
        self->vt_1C();
    }
}

void func_801A4BC8(CVision* self) {
    // Retail materializes the equality test via mfcr/extrwi (value context).
    bool atBase = (lbl_eu_80667CD4 == self->field_2619C);
    if (atBase) {
        self->vt_2C(3);
        void* mem = getHandleMEM2__Q23mtl10MemManagerFv();
        func_8004392C(1, 0x130, (u32)mem, 2, 1, lbl_eu_80667CF8);
    }
    // Materialized timer-window checks (retail evaluates both into r0).
    int w1;
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CE4 &&
        lbl_eu_80667CE4 < self->field_26194 - self->field_261A0) {
        w1 = 1;
    } else {
        w1 = 0;
    }
    if (w1) {
        func_80081E90__Q22cf13CfGameManagerFv(0, 0, 0);
        func_8006E5D8();
    }
    int w2;
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CFC &&
        lbl_eu_80667CFC < self->field_26194 - self->field_261A0) {
        w2 = 1;
    } else {
        w2 = 0;
    }
    if (w2) {
        self->vt_20(0);
    }
}

void func_801A4CF8(CVision* self) {
    CVisionSub* sub = (self->sub.field_00 == 0) ? 0 : &self->sub;
    if (sub == 0) {
        self->vt_20(1);
        return;
    }
    CVisionFusion* p1 = (CVisionFusion*)func_8016FE34(func_800B708C__Fi(sub->field_00));
    CVisionFusion* p2 = (CVisionFusion*)func_8016FE34(func_800B708C__Fi(sub->field_04));
    if (p1 == 0 || p2 == 0) {
        self->vt_20(1);
        return;
    }
    // Retail materializes the equality test via mfcr/extrwi (value context).
    bool atBase = (lbl_eu_80667CD4 == self->field_2619C);
    if (atBase) {
        // Resume callback clear: restore the sub fader, play a sound.
        if (sub->field_824 & 0x20000) {
            sub->field_0C->field_7C = lbl_eu_80667D00;
        } else {
            sub->field_0C->field_7C = lbl_eu_80667CF4;
        }
        if (sub->field_824 & 0x20000) {
            func_80081F90__Q22cf13CfGameManagerFv(0x26, 0);
        }
        func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(0, 0x1bf, 0, 0, lbl_eu_80667CE0);
    }
    // Materialized timer-window check (retail evaluates into r0).
    int w1;
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CD4 &&
        lbl_eu_80667CD4 < self->field_26194 - self->field_261A0) {
        w1 = 1;
    } else {
        w1 = 0;
    }
    if (w1) {
        // Virtual dispatches through the retail vtable slots (typed casts so
        // MWCC emits the standard r12 mtctr/bctrl member-call sequence).
        ((CVisionFusionSub*)p1)->vf5C4(lbl_eu_80667CF0);
        if (sub->field_824 & 0x20000) {
            ((CVisionFusionSub*)p1)->vf5C8(1);
            func_800BE12C((u8*)p1 + 0x3e9c, 0x11, 1, -1, 1);
        } else {
            func_800BE12C((u8*)p1 + 0x3e9c, 0x1, 1, -1, 1);
        }
        func_80174B4C(p1, 0x4000);
        if (p1 != p2) {
            ((CVisionFusionSub*)p2)->vf5C4(lbl_eu_80667CF0);
            func_800BE12C((u8*)p2 + 0x3e9c, 0x1, 1, -1, 1);
            func_80174B4C(p2, 0x4000);
            p2->field_3388 |= 0x2;
        }
        if (p2->field_3F00 & 0x2) {
            // Single r0 result across both paths (retail mfcr/srwi shape).
            int cancel;
            if (p2 == 0 || p2->field_3F60 == 0) {
                cancel = 1;
            } else {
                void* f60 = p2->field_3F60;
                CVisionAcResult* r =
                    ((CVisionFusionSubObj*)((u8*)p2 + 0x3E9C))->vfAC();
                // Retail loads the call-result float first.
                f32 diff = r->time;
                diff = *(f32*)((u8*)f60 + 0x3e8) - diff;
                cancel = diff > lbl_eu_80667CD0;
            }
            if (cancel != 0) {
                self->vt_20(1);
            }
        }
    }
    // Materialized timer-window check (retail evaluates into r0).
    int w2;
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667D04 &&
        lbl_eu_80667D04 < self->field_26194 - self->field_261A0) {
        w2 = 1;
    } else {
        w2 = 0;
    }
    if (w2) {
        func_801AD504(1);
    }
    f32 f = ((CVisionFusionSub*)p1)->vf5BC();
    if (f == lbl_eu_80667CD4) {
        CVisionPtmf cb;
        // Retail walks the triplet with an updating load (lwzu).
        u32* src = &lbl_eu_80533164[0];
        cb.mPfn = *src++;
        cb.mObj = *src++;
        cb.mDelta = *src;
        self->mPtmf = cb;
        self->field_2619C = lbl_eu_80667CD4;
        self->field_261A0 = self->field_26194;
        __ptmf_test(&cb);
    }
}

// ---------------------------------------------------------------------------
// us-801a678c: Second-fusion vision check. When the fusion's 0x2 flag is up,
// cancel vision if the remaining time is short, then run the D08/CE4 timer
// windows (retail func_801A506C).
// ---------------------------------------------------------------------------
void func_801A506C(CVision* self) {
    CVisionSub* sub;
    if (self->sub.field_00 == 0) {
        sub = 0;
    } else {
        sub = &self->sub;
    }
    CVisionFusion* p2 = (CVisionFusion*)func_8016FE34(func_800B708C__Fi(sub->field_04));
    if (p2->field_3F00 & 0x2) {
        bool cancel;
        if (p2 == 0 || p2->field_3F60 == 0) {
            cancel = true;
        } else {
            void* f60 = p2->field_3F60;
            void* sub = (u8*)p2 + 0x3e9c;
            void* r = ((void* (*)(void*))((void**)*(void**)sub)[0xAC / 4])(sub);
            f32 f_ret = *(f32*)((u8*)r + 4);
            f32 diff = *(f32*)((u8*)f60 + 0x3e8) - f_ret;
            cancel = diff < lbl_eu_80667CD0;
        }
        if (cancel) {
            self->vt_20(1);
        }
    }
    // Materialized timer-window checks (retail evaluates both into r0).
    int w1;
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667D08 &&
        lbl_eu_80667D08 < self->field_26194 - self->field_261A0) {
        w1 = 1;
    } else {
        w1 = 0;
    }
    if (w1) {
        func_800EA484(CBattleManager::getInstance(), lbl_eu_80667CD4, 0x24);
    }
    int w2;
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667D08 &&
        lbl_eu_80667D08 < self->field_26194 - self->field_261A0) {
        w2 = 1;
    } else {
        w2 = 0;
    }
    if (w2) {
        func_801AD504(2);
    }
    int w3;
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CE4 &&
        lbl_eu_80667CE4 < self->field_26194 - self->field_261A0) {
        w3 = 1;
    } else {
        w3 = 0;
    }
    if (w3) {
        u32* src = &lbl_eu_80533170.mPfn;
        u32 dlt, obj, pfn;
        pfn = *src++;
        obj = *src++;
        dlt = *src++;
        CVisionPtmf cb;
        cb.mPfn = pfn;
        cb.mObj = obj;
        cb.mDelta = dlt;
        f32 f_6194 = self->field_26194;
        f32 f_cd4 = lbl_eu_80667CD4;
        self->mPtmf.mPfn = pfn;
        self->mPtmf.mObj = obj;
        self->mPtmf.mDelta = cb.mDelta;
        self->field_2619C = f_cd4;
        self->field_261A0 = f_6194;
        __ptmf_test(&cb);
    }
}

void func_801A5260(CVision* self) {
    // Materialized timer-window checks (retail evaluates each into r0).
    int w1;
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667D04 &&
        lbl_eu_80667D04 < self->field_26194 - self->field_261A0) {
        w1 = 1;
    } else {
        w1 = 0;
    }
    if (w1) {
        func_801AD504(4);
    }
    int w2;
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667D0C &&
        lbl_eu_80667D0C < self->field_26194 - self->field_261A0) {
        w2 = 1;
    } else {
        w2 = 0;
    }
    if (w2) {
        // Retail walks the triplet with an updating load (lwzu).
        CVisionPtmf cb;
        u32* src = &lbl_eu_8053317C[0];
        cb.mPfn = *src++;
        cb.mObj = *src++;
        cb.mDelta = *src;
        self->mPtmf = cb;
        self->field_2619C = lbl_eu_80667CD4;
        self->field_261A0 = self->field_26194;
        __ptmf_test(&cb);
    }
}

void func_801A5360(CVision* self) {
    // Materialized timer-window checks (retail evaluates each into r0).
    int w1;
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CD4 &&
        lbl_eu_80667CD4 < self->field_26194 - self->field_261A0) {
        w1 = 1;
    } else {
        w1 = 0;
    }
    if (w1) {
        func_800EA484((CBattleManager*)getInstance__Q22cf14CBattleManagerFv(),
                      lbl_eu_80667CF0, 0x10);
        func_800EA484((CBattleManager*)getInstance__Q22cf14CBattleManagerFv(),
                      lbl_eu_80667CF0, 0x24);
    }
    int w2;
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667D10 &&
        lbl_eu_80667D10 < self->field_26194 - self->field_261A0) {
        w2 = 1;
    } else {
        w2 = 0;
    }
    if (w2) {
        self->vt_1C();
    }
}

// us-801a6b64: Match the vision sub-object's model against `obj`; when it
// matches one of the callback identities, install the associated vision FX
// callback on the vision slots.
void func_801A5444(CVision* self, CVisionBattleObj* obj, CVisionBattleObj* r5) {
    // Identity/callback table base (retail anchors it in a saved GPR).
    u32* tbl = lbl_eu_80533128;
    CVisionPtmf cur = self->mPtmf;
    if (__ptmf_test(&cur) == 0) {
        return;
    }
    CVisionSub* sub;
    if (self->sub.field_00 == 0) {
        sub = 0;
    } else {
        sub = &self->sub;
    }
    CVisionBattleObj* fu = (CVisionBattleObj*)func_8016FE34(func_800B708C__Fi(sub->field_00));
    if (fu == 0) {
        self->vt_20(1);
        return;
    }
    if (obj != fu) {
        return;
    }

    // The callback must equal one of the two identity triplets at
    // lbl_eu_80533128+0x60 / +0x6C for the FX-install path to run.
    int ok = 1;
    CVisionPtmf t1 = *(CVisionPtmf*)&tbl[0x60 / 4];
    if (__ptmf_cmpr(&self->mPtmf, &t1) == 0) {
        CVisionPtmf t2 = *(CVisionPtmf*)&tbl[0x6C / 4];
        if (__ptmf_cmpr(&self->mPtmf, &t2) == 0) {
            ok = 0;
        }
    }
    if (ok == 0) {
        // Not one of the two FX identities: the +0x84 triplet releases the
        // slot, the +0x90 triplet is checked and ignored.
        CVisionPtmf t4 = *(CVisionPtmf*)&tbl[0x84 / 4];
        if (__ptmf_cmpr(&self->mPtmf, &t4) == 0) {
            func_800F449C(sub);
        } else {
            CVisionPtmf t5 = *(CVisionPtmf*)&tbl[0x90 / 4];
            __ptmf_cmpr(&self->mPtmf, &t5);
        }
        return;
    }

    // FX-install path: pull the item block and the mid block from `obj`.
    CVisionInitBlock* src = obj->vf298();
    sub->items[obj->vf2A8()] = *src;
    if (obj->vf2A8() == 0) {
        CVisionInitBlock* mid = obj->vf298();
        *(CVisionSubMid*)&sub->field_10 = *(CVisionSubMid*)mid;
        f32 sum = mid->f_5C + mid->f_60;
        int r0;
        if (sum >= lbl_eu_80667CD4) {
            if (sub->field_84 & 0x80) {
                r0 = 0;
            } else {
                f64 adj = (sum > lbl_eu_80667CD4) ? lbl_eu_80667D18 : lbl_eu_80667D20;
                r0 = (int)(sum + adj);
            }
        } else {
            r0 = 0;
        }
        if (r0 <= 0) {
            sub->f_6C = lbl_eu_80667CD4;
        }
    } else {
        CVisionInitBlock* d = obj->vf298();
        if (d->w_74pair[0] & 1) {
            sub->field_84 |= d->w_74pair[0];
        } else {
            sub->field_88 |= d->w_74pair[0];
        }
    }
    if (obj->vf2A8() > 0) {
        CVisionInitBlock* d2 = obj->vf298();
        f32 sum2 = d2->f_5C + d2->f_60;
        int r0b;
        if (sum2 >= lbl_eu_80667CD4) {
            if (d2->w_74pair[0] & 0x80) {
                r0b = 0;
            } else {
                f64 adj2 = (sum2 > lbl_eu_80667CD4) ? lbl_eu_80667D18 : lbl_eu_80667D20;
                r0b = (int)(sum2 + adj2);
            }
        } else {
            r0b = 0;
        }
        if (r0b > 0) {
            CVisionInitBlock* d3 = obj->vf298();
            f32 sum3 = d3->f_5C + d3->f_60;
            int r0c;
            if (sum3 >= lbl_eu_80667CD4) {
                if (d3->w_74pair[0] & 0x80) {
                    r0c = 0;
                } else {
                    f64 adj3 = (sum3 > lbl_eu_80667CD4) ? lbl_eu_80667D18 : lbl_eu_80667D20;
                    r0c = (int)(sum3 + adj3);
                }
            } else {
                r0c = 0;
            }
            // Retail converts the int through the 0x4330 double magic
            // (signed int -> double) before the single-precision add.
            if (r0c > 0) {
                sub->f_6C += r0c;
            }
        }
    }
    if (r5->vf2BC() != 0 || (obj->vf298()->w_74pair[1] & 0x1000)) {
        sub->field_824 |= 0x400;
    }

    // Install the +0x78 identity callback with the fader fields.
    u32* np = &tbl[0x78 / 4];
    u32 pfn = *np++;
    u32 cbObj = *np++;
    u32 dlt = *np;
    CVisionPtmf cb;
    cb.mPfn = pfn;
    cb.mObj = cbObj;
    cb.mDelta = dlt;
    f32 f_6194 = self->field_26194;
    f32 f_cd4 = lbl_eu_80667CD4;
    self->mPtmf.mPfn = pfn;
    self->mPtmf.mObj = cbObj;
    self->mPtmf.mDelta = dlt;
    self->field_2619C = f_cd4;
    self->field_261A0 = f_6194;
    __ptmf_test(&cb);
}

// Parameter objects traversed by func_801A5BA8 via the fusion vtable.
class CVisionPrmHolder {
public:
    u8 unk0[0x50];
    CVisionPrm* field_50;   // 0x50
};

class CVisionPrm {
public:
    u8 unk0[0x44];
    u8 b_44;                // 0x44 (retail lbz: unsigned byte)
};

void func_801A5BA8(CVision* self) {
    // Reverse-declaration coloring (see func_801A5E58): the parameter is
    // consumed before the first calls and dies into cv, which is assigned
    // only after them - so no callee-saved home is needed for the parameter
    // and the locals take p=r31, cv=r30, byte/changed=r29 as in retail.
    CVisionFusion* p;
    CVision* cv;
    u8 byte;
    int changed;
    CVisionSub* sub;
    if (self->sub.field_00 == 0) {
        sub = 0;
    } else {
        sub = &self->sub;
    }
    // Retail dereferences the conditional sub pointer unconditionally
    // (field_00 != 0 guarantees it is valid in practice).
    p = (CVisionFusion*)func_8016FE34(func_800B708C__Fi(sub->field_00));
    cv = self;
    if (p == 0) {
        cv->vt_20(1);
        return;
    }
    // Retail materializes the equality test via mfcr/extrwi (value context).
    bool atBase = (lbl_eu_80667CD4 == cv->field_2619C);
    if (atBase) {
        p->field_3388 |= 0x2;
    }
    // The func_8016FE34 object carries its vtable at +0; dispatch through
    // the typed sub view while keeping CVisionFusion for the data fields.
    CVisionPrmHolder* a = ((CVisionFusionSub*)p)->vf2A4();
    CVisionPrm* prm = a->field_50;
    byte = prm->b_44;
    s32 result = ((CVisionFusionSub*)p)->vf2A8(prm);
    // Retail r29 flag: subfc/adde idiom for `byte <= result` (byte is the
    // zero-extended lbz value, so the compare is signed vs [0,255]).
    changed = byte <= result;

    // Materialized timer-window checks (retail evaluates each into r0).
    int w1;
    if (cv->field_26198 - cv->field_261A0 <= lbl_eu_80667CD4 &&
        lbl_eu_80667CD4 < cv->field_26194 - cv->field_261A0) {
        w1 = 1;
    } else {
        w1 = 0;
    }
    if (w1) {
        if (changed) {
            ((CVisionBMView*)CBattleManager::getInstance())->vt34(8,
                                                                  lbl_eu_80667D30,
                                                                  lbl_eu_80667D34);
        }
    }
    int w2;
    if (cv->field_26198 - cv->field_261A0 <= lbl_eu_80667CD4 &&
        lbl_eu_80667CD4 < cv->field_26194 - cv->field_261A0) {
        w2 = 1;
    } else {
        w2 = 0;
    }
    if (w2) {
        func_801AD504(8);
    }
    int w3;
    if (cv->field_26198 - cv->field_261A0 <= lbl_eu_80667D34 &&
        lbl_eu_80667D34 < cv->field_26194 - cv->field_261A0) {
        w3 = 1;
    } else {
        w3 = 0;
    }
    if (w3) {
        if (changed) {
            func_800EA484(CBattleManager::getInstance(), lbl_eu_80667CD4, 3);
        }
    }
    int w4;
    if (cv->field_26198 - cv->field_261A0 <= lbl_eu_80667D34 &&
        lbl_eu_80667D34 < cv->field_26194 - cv->field_261A0) {
        w4 = 1;
    } else {
        w4 = 0;
    }
    if (w4) {
        // Without the flag, bail out early unless vt2BC says otherwise.
        if (!changed) {
            if (((CVisionFusionSub*)p)->vf2BC() == 0) {
                return;
            }
        }
        // Retail walks the triplet with updating loads (lwzu), builds the
        // stack copy first, then installs it (mDelta re-read from the copy).
        u32* src = &lbl_eu_805331C4.mPfn;
        u32 dlt, obj, pfn;
        pfn = *src++;
        obj = *src++;
        dlt = *src++;
        CVisionPtmf cb;
        cb.mPfn = pfn;
        cb.mObj = obj;
        // Retail schedules the sdata2 zero load here (between the triplet
        // stores), so materialize it at this point.
        f32 f_cd4 = lbl_eu_80667CD4;
        cb.mDelta = dlt;
        cv->mPtmf.mPfn = pfn;
        cv->mPtmf.mObj = obj;
        cv->mPtmf.mDelta = cb.mDelta;
        cv->field_2619C = f_cd4;
        cv->field_261A0 = cv->field_26194;
        __ptmf_test(&cb);
    }
}

// ---------------------------------------------------------------------------
// us-801a7578: When the vision field is active, ask each player's battle
// state whether it should show, and forward it to func_80133F48 (retail
// func_801A5E58).
// ---------------------------------------------------------------------------
void func_801A5E58(CVision* self) {
    // MWCC colors callee-saved GPRs in reverse declaration order (locals), so
    // the declaration sequence below reproduces the retail mapping
    // (convHi=r31, sub=r30, i=r29, p=r28, cv=r27); the parameter dies into the
    // trailing local copy and is never spilled.
    u32 convHi = 0x43300000u;
    CVisionSub* sub;
    int i;
    CVisionFusionSub* p;
    CVision* cv = self;
    if (cv->sub.field_00 == 0) {
        sub = 0;
    } else {
        sub = &cv->sub;
    }
    // Materialized timer-window check (retail evaluates the condition into r0).
    int w1;
    if (cv->field_26198 - cv->field_261A0 <= lbl_eu_80667CD4 &&
        lbl_eu_80667CD4 < cv->field_26194 - cv->field_261A0) {
        w1 = 1;
    } else {
        w1 = 0;
    }
    if (w1) {
        f32 scl = lbl_eu_80667D38;
        if (func_800F477C(sub) != 0) {
            if (func_800F477C(sub)->b_42 == 1) {
                scl = lbl_eu_80667D3C;
            }
        }
        // Per-player offset accumulation. The signed int result goes through
        // the 0x4330/xoris double trick against the NAMED .sdata2 magic
        // lbl_eu_80667D28 (hoisted into a saved FPR), then folds into the
        // running scale.
        for (i = 0; i < 3; i++) {
            p = (CVisionFusionSub*)func_8016FE34((int)getPlayer__Q22cf13CfGameManagerFi(i));
            if (p != 0) {
                if (p->vf290() != 0) {
                    s32 out;
                    if (func_80260264(p->vf290(), 0x6c, &out) != 0) {
                        union {
                            u32 w[2];
                            double d;
                        } conv;
                        conv.w[0] = convHi;
                        conv.w[1] = (u32)out ^ 0x80000000u;
                        scl += (f32)(conv.d - lbl_eu_80667D28);
                    }
                }
            }
        }
        if (sub->field_824 & 0x20000) {
            scl = sub->field_0C->field_2C;
        }
        if (((BMVtIf828*)CBattleManager::getInstance())->v008(0x400) != 0) {
            scl = lbl_eu_80667D00;
        }
        if (((BMVtIf828*)CBattleManager::getInstance())->v008(0x800) != 0) {
            scl = lbl_eu_80667D40;
        }
        sub->field_830 = scl;
        func_801AD504(0x10);
    }
    int w2;
    if (cv->field_26198 - cv->field_261A0 <= lbl_eu_80667D04 &&
        lbl_eu_80667D04 < cv->field_26194 - cv->field_261A0) {
        w2 = 1;
    } else {
        w2 = 0;
    }
    if (w2) {
        // Retail copies the 12-byte callback triplet as a struct.
        CVisionPtmf cb = lbl_eu_805331D0;
        cv->mPtmf = cb;
        cv->field_2619C = lbl_eu_80667CD4;
        cv->field_261A0 = cv->field_26194;
        __ptmf_test(&cb);
    }
}

void func_801A60B0(CVision* self) {
    // Retail hoists the first threshold into an FPR before anything else.
    f32 t1 = lbl_eu_80667CD4;
    // Materialized timer-window check (retail evaluates the condition into r0).
    int w1;
    if (self->field_26198 - self->field_261A0 <= t1 &&
        t1 < self->field_26194 - self->field_261A0) {
        w1 = 1;
    } else {
        w1 = 0;
    }
    if (w1) {
        self->vt_2C(2);
        func_801BFE8C(0, 0x1bf, 0);
        func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(0, 0x1c0, 0, 0, lbl_eu_80667CE0);
    }
    f32 t2 = lbl_eu_80667D04;
    int w2;
    if (self->field_26198 - self->field_261A0 <= t2 &&
        t2 < self->field_26194 - self->field_261A0) {
        w2 = 1;
    } else {
        w2 = 0;
    }
    if (w2) {
        u8 vdlist[8];
        func_80043D90(vdlist);
        func_800F4A98(func_80043F18(vdlist), 0x328, 0);
        // Flag every matching player object; bail out with vt_20(1) if the
        // cast fails mid-iteration.
        u32 i = 0;
        while (i < *(u32*)((u8*)func_80043F18(vdlist) + 0x620)) {
            void* elem = func_800F6EAC(func_80043F18(vdlist), i);
            void* obj = __dynamic_cast(elem, 0, &lbl_eu_806618E8, &lbl_eu_806618F0, 0);
            if (obj == 0) {
                self->vt_20(1);
                __dt__80043E88(vdlist, -1);
                return;
            }
            ((void (*)(void*, u32))((void**)obj)[0x190 / 4])(obj, 0);
            i++;
        }
        __dt__80043E88(vdlist, -1);
        if (getGlobalSda() != 0) {
            getGlobalSda()[0xa8] = 0;
        }
        self->vt_18();
        self->vt_30(1);
        func_80081E90__Q22cf13CfGameManagerFv(0, 0, 0);
        func_8006E5D8();
    }
    f32 t3 = lbl_eu_80667D44;
    int w3;
    if (self->field_26198 - self->field_261A0 <= t3 &&
        t3 < self->field_26194 - self->field_261A0) {
        w3 = 1;
    } else {
        w3 = 0;
    }
    if (w3) {
        // Retail walks the callback triplet with an updating pointer load,
        // then copies it to the member as a struct.
        CVisionPtmf cb;
        u32* src = &lbl_eu_805331DC.mPfn;
        cb.mPfn = *src++;
        cb.mObj = *src++;
        cb.mDelta = *src;
        self->mPtmf = cb;
        self->field_2619C = lbl_eu_80667CD4;
        self->field_261A0 = self->field_26194;
        __ptmf_test(&cb);
    }
}

void func_801A6340(CVision* self) {
    CVisionSub* sub = (self->sub.field_00 == 0) ? 0 : &self->sub;
    CVisionFusion* fr = (CVisionFusion*)func_8016FE34(func_800B708C__Fi(sub->field_00));
    CVisionFusion* fr2 = (CVisionFusion*)func_8016FE34(func_800B708C__Fi(sub->field_04));
    // Retail materializes the equality test via mfcr/extrwi (value context).
    bool atBase = (lbl_eu_80667CD4 == self->field_2619C);
    if (atBase) {
        func_801537E0(&fr->field_3380);
    }
    // Materialized timer-window checks (retail evaluates both into r0).
    int w1;
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CD4 &&
        lbl_eu_80667CD4 < self->field_26194 - self->field_261A0) {
        w1 = 1;
    } else {
        w1 = 0;
    }
    if (w1) {
        func_800EA484(CBattleManager::getInstance(), lbl_eu_80667CD4, 0x3);
    }
    int w2;
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CD4 &&
        lbl_eu_80667CD4 < self->field_26194 - self->field_261A0) {
        w2 = 1;
    } else {
        w2 = 0;
    }
    if (w2) {
        func_800EA484(CBattleManager::getInstance(), lbl_eu_80667CF0, 0x13);
        fr->field_3388 &= 0xFFFD;
        fr->field_04->v20(0x4000);
        if (fr != fr2) {
            fr2->field_3388 &= 0xFFFD;
            fr2->field_04->v20(0x4000);
        }
        sub->field_0C->field_7C = func_800F42AC(sub);
        self->vt_1C();
    }
}

// us-801aa0b0: Manage per-battle-slot vision FX. With `slot` set, validate
// the matching battle slot (timing/rounding/state checks) and re-engage FX via
// func_801A8244; with `slot` null, walk the four 0x834-strided vision slots,
// run the gate/flag machinery, and refresh or clear each slot's effect.
void func_801A897C(CVision* self, void* slot, void* r28) {
    if (slot != 0) {
        getInstance__Q22cf13CfGameManagerFv();
        if (func_8006EF04__Fi(0x4000000) != 0) {
            return;
        }
        CVisionBtlSlot* bs = (CVisionBtlSlot*)func_800EA444(CBattleManager::getInstance());
        if (bs == 0) {
            return;
        }
        if (bs->w_824 & 0x10000) {
            return;
        }
        if (func_800F4730() > 2) {
            return;
        }
        u32 x = *(u32*)((u8*)slot + 0x3f10);
        if (bs->w_00 != x && bs->w_04 != x) {
            return;
        }
        CVisionBattleObj* fu = (CVisionBattleObj*)func_8016FE34(func_800B708C__Fi(bs->w_00));
        if (fu == 0) {
            return;
        }
        if (fu->vf298()->w_04 != 0) {
            u32 v26 = bs->w_828;
            u32 v25 = bs->w_82C;
            CVision* r27 = (CVision*)func_800F4648(bs);
            // Out-params land in stack locals (retail reads sp+0x50 back).
            f32 outF;
            s32 outI;
            func_800E921C(CBattleManager::getInstance(), bs, fu, &outF, &outI);
            if (func_800F46C0(bs, fu) != 0) {
                return;
            }
            f32 f1 = outF;
            f64 adj = (f1 > lbl_eu_80667CD4) ? lbl_eu_80667D18 : lbl_eu_80667D20;
            int r = (int)(f1 + adj);
            if ((u32)r27 != (u32)r || ((bs->w_824 >> 10) & 1) != ((bs->w_88 >> 12) & 1) ||
                v26 != bs->w_828 || v25 != bs->w_82C) {
                // Retail recomputes the rounded value and derives the rounding
                // direction via the sign-bit abs-diff idiom.
                int rc = (int)outF;
                u32 xorv = (u32)r27 ^ (u32)rc;
                u32 r25v = ((u32)((s32)xorv >> 1) - (xorv & (u32)r27)) >> 31;
                func_801A8244(&CBattleManager::getInstance()->mVision, bs, 2, r25v, 0);
                return;
            }
            if ((void*)fu != slot) {
                return;
            }
            if (r28 == 0) {
                return;
            }
            if (*(u16*)((u8*)r28 + 0xc) == 0x10) {
                if (func_80148778((u8*)slot + 8, 0x10) != 0) {
                    func_801A8244(&CBattleManager::getInstance()->mVision, bs, 1, 1, 0);
                    return;
                }
                if (*(f32*)((u8*)r28 + 0x20) > lbl_eu_80667CD4) {
                    func_801A8244(&CBattleManager::getInstance()->mVision, bs, 1, 1, 0);
                    return;
                }
            }
        } else if ((void*)fu == slot && r28 != 0 &&
                   *(u16*)((u8*)r28 + 0xc) == 0x9 &&
                   func_80148778((u8*)slot + 8, 0x9) != 0) {
            func_801A8244(&CBattleManager::getInstance()->mVision, bs, 4, 1, 0);
            return;
        }
        return;
    }

    // r4 == 0 path: walk the four 0x834-strided vision slots. The zero
    // constant stays hoisted in a saved FPR across the whole loop.
    f32 f31 = lbl_eu_80667CD4;
    CVisionBtlSlot* p = (CVisionBtlSlot*)&self->sub;
    for (u32 i = 0; i < 4; i++, p++) {
        if (p->w_00 == 0) {
            continue;
        }
#define VISION_VT30(subPtr) (*(u32*)((void* (*)(void*))(*(void***)(void*)(subPtr))[0x30 / 4])((void*)(subPtr)))
        // Retail reuses the incoming argument registers for the per-slot
        // working values (slot -> object, r28 -> effect kind).
        slot = func_8016FE34(func_800B708C__Fi(p->w_00));
        // Second id lookup result is discarded (presence check only).
        func_8016FE34(func_800B708C__Fi(p->w_04));
        if (slot == 0) {
            self->vt_20(1);
            continue;
        }
        u32 local0 = VISION_VT30(((CVisionBattleObj*)slot)->field_04);
        if (func_80174C98((CVisionBattleObj*)slot, (int*)&local0, 0x802) != 0) {
            self->vt_20(1);
            continue;
        }
        if (p->w_824 & 0x10000) {
            // Five independent gates; passing all of them cancels the vision.
            u32 m0 = VISION_VT30(((CVisionBattleObj*)slot)->field_04);
            if (func_80174C98((CVisionBattleObj*)slot, (int*)&m0, 0x801) == 0) {
                u32 m1 = VISION_VT30(((CVisionBattleObj*)slot)->field_04);
                if (func_80174C98((CVisionBattleObj*)slot, (int*)&m1, 0x13) == 0) {
                    u32 m2 = VISION_VT30(((CVisionBattleObj*)slot)->field_04);
                    if (func_80174C98((CVisionBattleObj*)slot, (int*)&m2, 0x12) == 0) {
                        u32 m3 = VISION_VT30(((CVisionBattleObj*)slot)->field_04);
                        if (func_80174C98((CVisionBattleObj*)slot, (int*)&m3, 0x800) == 0) {
                            u32 m4 = VISION_VT30(((CVisionBattleObj*)slot)->field_04);
                            if (func_80174C98((CVisionBattleObj*)slot, (int*)&m4, 0x6) == 0) {
                                self->vt_20(1);
                                continue;
                            }
                        }
                    }
                }
            }
        }
        if (func_800F4424(p) == f31) {
            // Timer expired: re-arm unless a pair of progress bits is half-set
            // (started but not finished) or the effect flag is already up.
            u32 flags = p->w_824;
            if ((((flags & 1) && !(flags & 0x20)) || ((flags & 2) && !(flags & 0x40)) ||
                 ((flags & 4) && !(flags & 0x80)) || ((flags & 8) && !(flags & 0x100)) ||
                 ((flags & 0x10) && !(flags & 0x200)))) {
                // fall through to the refresh flow below
            } else if (!(flags & 0x10000)) {
                func_801AF934(0);
                p->w_824 |= 0x10000;
                if (p->w_824 & 0x20000) {
                    func_8009D018(0x30e3, 0);
                    func_801412D0(0x375);
                    func_8013F244();
                }
                continue;
            }
        }
        // Effect-kind accumulator reuses the third argument's register in
        // retail (the slot path's dataArg is dead by now).
        r28 = (void*)0;
        if (!(p->w_824 & 1) && ((CVisionBattleObj*)slot)->vf2BC() != 0) {
            r28 = (void*)5;
        }
        if (!(p->w_824 & 2) && r28 == 0) {
            // Nine gate reads, each fetched and tested immediately (each
            // result lives in its own stack word); all passing selects 4.
#define VOBJ ((CVisionBattleObj*)slot)
            u32 n0 = VISION_VT30(VOBJ->field_04);
            if (func_80174C98(VOBJ, (int*)&n0, 0x806) == 0) {
                u32 n1 = VISION_VT30(VOBJ->field_04);
                if (func_80174C98(VOBJ, (int*)&n1, 0x800) == 0) {
                    u32 n2 = VISION_VT30(VOBJ->field_04);
                    if (func_80174C98(VOBJ, (int*)&n2, 0x805) == 0) {
                        u32 n3 = VISION_VT30(VOBJ->field_04);
                        if (func_80174C98(VOBJ, (int*)&n3, 0x18) == 0) {
                            u32 n4 = VISION_VT30(VOBJ->field_04);
                            if (func_80174C98(VOBJ, (int*)&n4, 0x6) == 0) {
                                u32 n5 = VISION_VT30(VOBJ->field_04);
                                if (func_80174C98(VOBJ, (int*)&n5, 0x801) == 0) {
                                    u32 n6 = VISION_VT30(VOBJ->field_04);
                                    if (func_80174C98(VOBJ, (int*)&n6, 0x13) == 0) {
                                        u32 n7 = VISION_VT30(VOBJ->field_04);
                                        if (func_80174C98(VOBJ, (int*)&n7, 0x12) == 0) {
                                            u32 n8 = VISION_VT30(VOBJ->field_04);
                                            if (func_80174C98(VOBJ, (int*)&n8, 0x19) == 0) {
                                                r28 = (void*)4;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
#undef VOBJ
        }
        if (func_80148778((u8*)slot + 8, 0xf) != 0 ||
            func_80148778((u8*)slot + 8, 0x9) != 0 ||
            func_80148778((u8*)slot + 8, 0xb) != 0) {
            r28 = (void*)4;
        }
        if (func_80148778((u8*)slot + 8, 0xc) != 0 &&
            (func_800F477C(p) == 0 || !(func_800F477C(p)->w_78 & 1))) {
            r28 = (void*)4;
        }
        if (!(p->w_824 & 4) && r28 == 0) {
            // Fusion hand-off: run the embedded sub-object's slot-0x4C call
            // and feed the result plus both actors to the battle manager.
            if (func_800F46C0(p, (CVisionBattleObj*)slot) != 0 && !(p->w_824 & 0x20000)) {
                r28 = (void*)3;
                void* emb = *(void**)((u8*)slot + 0x3E9C);
                u32 res = ((u32 (*)(void*))(*(void***)emb)[0x4C / 4])(emb);
                p->w_04 = res;
                f32 outF2;
                s32 outI2;
                func_800E921C(CBattleManager::getInstance(), p, slot, &outF2, &outI2);
            }
        } else if ((p->w_824 & 4) && (p->w_824 & 0x40)) {
            p->w_824 &= 0x1CE;
        }
        // Each finished progress pair collapses its mask down (the st guard
        // only suppresses the collapse while a refresh is pending).
        if ((p->w_824 & 2) && (p->w_824 & 0x40)) {
            p->w_824 &= 0xE7;
        }
        if ((p->w_824 & 8) && r28 != 0 && (p->w_824 & 0x100)) {
            p->w_824 &= 0x39C;
        }
        if ((p->w_824 & 0x10) && r28 != 0 && (p->w_824 & 0x200)) {
            p->w_824 &= 0x738;
        }
#undef VISION_VT30
        if (r28 != 0) {
            func_801A8244(self, p, (u32)(uintptr_t)r28, 1, 0);
        }
    }
}


// us-801a7c60: Manage vision FX on the sub-object. When the fade timer is at
// baseline (field_2619C == 0) only the D48 release-window check runs;
// otherwise the FX setup path drives the fusion's parameter block and either
// finishes immediately (B8BC == 0) or programs the battle object state and
// queues ring events 1..5 before falling into the same window check.
void func_801A6540(CVision* self) {
    CVisionSub* sub = (self->sub.field_00 != 0) ? &self->sub : 0;
    if (sub == 0) {
        self->vt_20(1);
        return;
    }
    CVisionBattleObj* fu = (CVisionBattleObj*)func_8016FE34(func_800B708C__Fi(sub->field_00));
    if (fu == 0) {
        self->vt_20(1);
        return;
    }

    if (lbl_eu_80667CD4 != self->field_2619C) {
        // Setup path: begin driving the FX on the fusion parameter block.
        func_800EA484(CBattleManager::getInstance(), lbl_eu_80667CD4, 0x13);
        sub->field_824 |= 0x100000;
        func_801537F0(&((CVisionFusion*)fu)->field_3380);
        CVisionFxParam p;
        memset(&p.mid, 0, sizeof(p.mid));
        memset(&p, 0, sizeof(p));
        if (sub->field_0C != 0) {
            p.mid.b_0D = 0x5a;
            p.h_12 = *((u8*)sub->field_0C + 0x77);
        } else {
            p.mid.b_0D = 0xd;
            p.h_12 = -1;
        }
        p.f_14 = lbl_eu_80667CD4;
        p.mid.h_10 = 0;
        p.mid.b_06 = 6;
        func_8014AC38(&((CVisionFusion*)fu)->field_3380, &p);
        func_800BE12C((u8*)fu + 0x3e9c, 1, 0, -1, 1);
        if (func_8014B8BC(&((CVisionFusion*)fu)->field_3380, &p) == 0) {
            func_801537E0(&((CVisionFusion*)fu)->field_3380);
            func_800EA484(CBattleManager::getInstance(), lbl_eu_80667CF0, 0x13);
            // Install the lbl_eu_805331E8 callback with the fader fields reset.
            CVisionPtmf cb;
            cb.mPfn = lbl_eu_805331E8[0];
            cb.mObj = lbl_eu_805331E8[1];
            cb.mDelta = lbl_eu_805331E8[2];
            f32 f_6194 = self->field_26194;
            f32 f_cd4 = lbl_eu_80667CD4;
            self->mPtmf.mPfn = cb.mPfn;
            self->mPtmf.mObj = cb.mObj;
            self->mPtmf.mDelta = cb.mDelta;
            self->field_2619C = f_cd4;
            self->field_261A0 = f_6194;
            __ptmf_test(&cb);
            return;
        }

        // Program the battle object from the filled-in parameter block.
        fu->vf298()->w_04 = p.w_00;
        fu->vf298()->w_50 = (u32)p.obj18;
        fu->vf298()->w_74pair[1] &= ~0xC00u;
        if (p.obj18->field_78 & 1) {
            fu->vf298()->w_74pair[1] |= 0x40000000 | 0x800;
        } else {
            fu->vf298()->w_74pair[1] |= 0x40000000 | 0x400;
        }
        sub->field_04 = p.w_00;
        sub->field_0C = p.obj18;

        func_801537E0(&((CVisionFusion*)fu)->field_3380);
        u32 outC;
        u32 out8;
        func_800E921C(CBattleManager::getInstance(), sub, fu, &outC, &out8);
        sub->field_830 = lbl_eu_80667CD4;

        // Queue FX events 1..5 on the vision ring at step-increasing times.
        // The time accumulator deliberately lives across the func_801AC09C
        // call (retail keeps it in f31).
        f32 t = lbl_eu_80667CE4;
        if (func_801AC09C(1) == 0) {
            // Build the five ring events (ids 1..5 at step-increasing times),
            // then enqueue them; MWCC fuses the two loops. Ring fields are
            // reached as far-offset CVision members (retail emits a fresh
            // addis/lwz pair per access, no cached base pointer).
            CVisionU32F32U32 ev[5];
            int k;
            for (k = 0; k < 5; k++) {
                ev[k].a = k + 1;
                ev[k].b = t;
                ev[k].c = 0;
                t += lbl_eu_80667CD4;
            }
            for (k = 0; k < 5; k++) {
                CVisionU32F32U32* el = &self->unk261C4.arr[
                    (self->unk261C4.field_64 + self->unk261C4.field_68) % self->unk261C4.w6C];
                *el = ev[k];
                self->unk261C4.field_68 = self->unk261C4.field_68 + 1;
            }
        }
    }

    // Release window check shared by the baseline path and post-setup.
    int w;
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667D48 &&
        lbl_eu_80667D48 < self->field_26194 - self->field_261A0) {
        w = 1;
    } else {
        w = 0;
    }
    if (w) {
        func_800EA484(CBattleManager::getInstance(), lbl_eu_80667CF0, 0x13);
        // Install the lbl_eu_805331F4 callback with the fader fields reset.
        CVisionPtmf cb;
        cb.mPfn = lbl_eu_805331F4[0];
        cb.mObj = lbl_eu_805331F4[1];
        cb.mDelta = lbl_eu_805331F4[2];
        f32 f_6194 = self->field_26194;
        f32 f_cd4 = lbl_eu_80667CD4;
        self->mPtmf.mPfn = cb.mPfn;
        self->mPtmf.mObj = cb.mObj;
        self->mPtmf.mDelta = cb.mDelta;
        self->field_2619C = f_cd4;
        self->field_261A0 = f_6194;
        __ptmf_test(&cb);
    }
}

// ---------------------------------------------------------------------------
// us-801a819c: Validate a battle object / slot and, when it passes, install
// the state-change callback (retail func_801A6A7C).
// ---------------------------------------------------------------------------
int func_801A6A7C(CVision* self, CVisionObj* obj) {
    if (__ptmf_test(&self->mPtmf) == 0) {
        return 0;
    }
    if ((obj->field_3F00 & 0x4) == 0) {
        return 0;
    }
    CVisionSub* sub = (self->sub.field_00 == 0) ? 0 : &self->sub;
    if (sub == 0) {
        return 0;
    }
    if (obj->field_3F10 != sub->field_00) {
        return 0;
    }
    if (func_80148778(&obj->field_08, 0x113) == 0 &&
        func_80148778(&obj->field_08, 0x114) == 0 &&
        func_80148778(&obj->field_08, 0x115) == 0) {
        return 0;
    }
    if ((sub->field_824 & 0x4000) == 0) {
        return 0;
    }
    CVisionPtmf cb;
    u32* src = lbl_eu_80533200;
    u32 t1 = *src++;
    u32 t2 = *src++;
    u32 dlt = *src++;
    cb.mPfn = t1;
    cb.mObj = t2;
    cb.mDelta = dlt;
    f32 f_6194 = self->field_26194;
    f32 f_cd4 = lbl_eu_80667CD4;
    self->mPtmf.mPfn = t1;
    self->mPtmf.mObj = t2;
    self->mPtmf.mDelta = cb.mDelta;
    self->field_2619C = f_cd4;
    self->field_261A0 = f_6194;
    __ptmf_test(&cb);
    return 1;
}

// Typed view of CBattleManager's vtable slot 0x28 (declared index 8).
class CVisionBM28View {
public:
    virtual void f000(); virtual void f004(); virtual void f008(); virtual void f00C();
    virtual void f010(); virtual void f014(); virtual void f018(); virtual void f01C();
    virtual s32 vt28(u32 r4);   // index 8 -> vtable 0x28
};

// us-801a82ec: Guard chain deciding whether a vision cutscene may engage on
// `obj` (with `r5` supplying the target actor), then HP/damage timing checks
// and the vision-callback install via vt_14. Returns 1 on success.
int func_801A6BCC(CVision* self, CVisionObjV* obj, CVisionObjV* r5) {
    if ((obj->field_3F00 & 0x4) == 0) {
        return 0;
    }
    if (obj->vt2BC() != 0) {
        return 0;
    }
    if (func_eu_80053FD4() != 0) {
        return 0;
    }
    u32 nc = *((u8*)CBattleManager::getInstance() + 0x1aa);
    // Bitwise AND of bools keeps the closest-to-retail two-compare shape.
    bool hi = nc <= 0x18;
    bool outOfRange = (nc >= 1) & hi;
    if (outOfRange) {
        return 0;
    }
    if (*(s16*)((u8*)CBattleManager::getInstance() + 0x20c8) != 0) {
        return 0;
    }
    getInstance__Q22cf13CfGameManagerFv();
    if (func_8006EF04__Fi(0x40000)) {
        return 0;
    }
    getInstance__Q22cf13CfGameManagerFv();
    if (CfRes_getE24Bit22() != 0) {
        return 0;
    }
    getInstance__Q22cf13CfGameManagerFv();
    if (func_8006EF04__Fi(0x200) != 0) {
        return 0;
    }
    if (((CVisionBM28View*)CBattleManager::getInstance())->vt28(0x200) != 0) {
        return 0;
    }
    if (func_801B481C() != 0) {
        return 0;
    }

    // Active-callback gate: retail normalizes each runtime-helper result via
    // '!' (cntlzw), and rejects only callbacks matching NEITHER known identity.
    CVisionPtmf cur = self->mPtmf;
    if (!__ptmf_test(&cur)) {
        // No callback installed: proceed.
    } else {
        CVisionPtmf t1;
        u32* src1 = lbl_eu_8053320C;
        u32 s11 = *src1++;
        u32 s12 = *src1++;
        u32 s13 = *src1++;
        t1.mPfn = s11;
        t1.mObj = s12;
        t1.mDelta = s13;
        if (!__ptmf_cmpr(&self->mPtmf, &t1)) {
            // lbl_eu_8053320C identity: allowed.
        } else {
            CVisionPtmf t2;
            u32* src2 = lbl_eu_80533218;
            u32 s21 = *src2++;
            u32 s22 = *src2++;
            u32 s23 = *src2++;
            t2.mPfn = s21;
            t2.mObj = s22;
            t2.mDelta = s23;
            if (!__ptmf_cmpr(&self->mPtmf, &t2)) {
                // lbl_eu_80533218 terminal identity: allowed.
            } else {
                return 0;
            }
        }
    }

    // Target fusion from `r5` (+4 id, +0x50 actor requirement).
    CVisionActor50* actor = r5->field_50;
    CVisionFusionV* target = (CVisionFusionV*)func_8016FE34(func_800B708C__Fi(r5->field_04));
    if (target == NULL || actor == NULL) {
        return 0;
    }
    if (func_8016FE34((int)getPlayer__Q22cf13CfGameManagerFi(0)) == NULL) {
        return 0;
    }

    int engage = 0;
    if (((CVisionBM28View*)CBattleManager::getInstance())->vt28(0x100) == 0) {
        // Movie/dialogue mode gates: the pending-event id must be clear and the
        // current event must be an accepted type (or the damage preview failed).
        if (func_8009CF8C(0x335e) != 0) {
            return 0;
        }
        s32 mode = func_8009CF8C(0x20);
        f32 outDamage;
        u32 outCount;
        s32 ok = func_8009E344(func_8009ECB0(), 1, (int*)&outDamage, (int*)&outCount);
        if (ok == 0 && mode != 0x166) {
            return 0;
        }
        void* row = func_8009EC9C(1);
        if (func_800A32C4(row) != 0) {
            return 0;
        }
        if (mode <= 0x1f) {
            return 0;
        }
        if (mode > 0x16a && mode < 0x178) {
            return 0;
        }
    } else {
        if (actor->field_3C != 0) {
            return 0;
        }
    }

    // Cancel while the target's remaining-HP delta falls below the threshold.
    if (target->field_3F00 & 0x2) {
        bool cancel;
        if (target == NULL || target->field_3F60 == NULL) {
            cancel = true;
        } else {
            f32 f_ret = *(f32*)((u8*)target->emb.vfAC() + 4);
            f32 diff = target->field_3F60->f_3E8 - f_ret;
            cancel = diff < lbl_eu_80667CD0;
        }
        if (cancel) {
            return 0;
        }
    }

    if ((actor->field_78 & 0x1000) != 0) {
        engage = 1;
    } else if (((CVisionBM28View*)CBattleManager::getInstance())->vt28(0x100) != 0) {
        engage = 1;
    } else {
        // The active vision slot (if any) must already hold this model id.
        CVisionSub* active = self->sub.field_00 != 0 ? &self->sub : NULL;
        if (active != NULL && active->field_00 == obj->field_3F10) {
            engage = 1;
        }
    }

    // Damage timing: engage when incoming damage would exceed max HP or drop
    // the HP fraction below the lbl_eu_80667D4C threshold.
    if (obj != (CVisionObjV*)target) {
        f32 total = lbl_eu_80667CD4;
        u32 hits = 0;
        CBattleManager_preCalcTotalDamage(getInstance__Q22cf14CBattleManagerFv(), obj, &total, &hits);
        if (hits != 0 && total > lbl_eu_80667CD4) {
            f32 hpMax = target->vt128();
            if (total >= hpMax) {
                engage = 1;
            }
            f32 hpDen = target->vt12C();
            f32 hpCur = target->vt128();
            f32 frac = (hpCur - total) / hpDen;
            if (frac <= lbl_eu_80667D4C) {
                engage = 1;
            }
        }
    }
    if (engage != 0) {
        if (self->vt_14(obj, target) == 0) {
            return 0;
        }
    }
    return 1;
}

int func_801A70DC(CVision* self, void* obj, void* obj2) {
    if (obj == 0) {
        self->vt_20(1);
        return 0;
    }
    CVisionObj* o = (CVisionObj*)obj;
    CVisionSubRef* eff = *(CVisionSubRef**)((u8*)obj + 0x3e74);
    if (eff == 0) {
        self->vt_20(1);
        return 0;
    }
    // Active vision slot (or none): if one exists it must either be fading
    // out (0x8000) or already own this model id.
    CVisionSub* sub;
    if (self->sub.field_00 == 0) {
        sub = 0;
    } else {
        sub = &self->sub;
    }
    if (sub != 0) {
        if (eff->field_78 & 0x8000) {
            self->vt_1C();
        } else if (sub->field_00 != o->field_3F10) {
            return 0;
        }
    }
    if (sub == 0) {
        // Claim a free slot. A slot already holding this id cancels instead.
        // The scans walk a cursor anchored at `self' (id sits at +0x04) while
        // the hit is materialised from the live index.
        u32 want = o->field_3F10;
        CVisionSub* dup;
        if (want == 0) {
            dup = 0;
        } else {
            CVisionSlotScan* walk = (CVisionSlotScan*)self;
            int i = 0;
            while (1) {
                if (walk->id == want) {
                    dup = &self->sub + i;
                    break;
                }
                walk++;
                i++;
                if (i == 4) {
                    dup = 0;
                    break;
                }
            }
        }
        if (dup != 0) {
            sub = 0;
        } else {
            CVisionSlotScan* walk = (CVisionSlotScan*)self;
            int j = 0;
            while (1) {
                if (walk->id == 0) {
                    sub = &self->sub + j;
                    break;
                }
                walk++;
                j++;
                if (j == 4) {
                    sub = 0;
                    break;
                }
            }
        }    }
    if (sub == 0) {
        return 0;
    }
    // Remaining-vision ratio; zero denominator means the full default.
    f32 denom = eff->field_2C;
    f32 ratio;
    if (denom == lbl_eu_80667CD4) {
        ratio = lbl_eu_80667CD8;
    } else {
        ratio = eff->field_7C / denom;
    }
    if (lbl_eu_80667CD4 == ratio) {
        return 0;
    }
    func_800F449C(sub);
    sub->field_00 = o->field_3F10;
    sub->field_04 = ((CVisionObj*)obj2)->field_3F10;
    sub->field_0C = eff;
    if (eff->field_78 & 0x8000) {
        sub->field_824 |= 0x20000;
        func_8009D018(0x30e3, 0);
        func_801412D0(0x375);
        func_8013F244();
    }
    if (o->field_3F00 & 0x4) {
        u16 h = o->field_3F28;
        if (h == 0x967 || h == 0x96b || h == 0x96c || h == 0x969) {
            sub->field_824 |= 0x40000;
        }
    }
    eff->field_7C = eff->field_2C;
    func_8016FF14(obj, (u8*)self + 0x20d4);
    if (obj2 != 0 && obj2 != obj) {
        func_8016FF14(obj2, (u8*)self + 0x68ec);
    }
    // Clear the engaged flag on every other battle actor and refresh its
    // per-slot vision parameter block.
    u8 vdlist[8];
    func_80043D90(vdlist);
    func_800F4A98(func_80043F18(vdlist), 0x20, 0);
    for (u32 i = 0; i < ((CVisionEnumList*)func_80043F18(vdlist))->count; i++) {
        void* elem = func_800F6EAC(func_80043F18(vdlist), i);
        void* actor = __dynamic_cast(elem, 0, &__RTTI__Q22cf13CfObjectActor, &lbl_eu_806618F0, 0);
        if (actor != obj && actor != obj2) {
            CVisionActor3388* a = (CVisionActor3388*)actor;
            a->field_3388 &= ~0x8u;
            func_8016FF14(actor, (u8*)self + i * 0x4818 + 0xB104);
        }
    }
    __dt__80043E88(vdlist, -1);
    eff->field_7C = lbl_eu_80667D10;
    self->field_26194 = lbl_eu_80667CD4;
    self->field_26198 = lbl_eu_80667CD4;
    lbl_eu_80663DA0 |= 0x1;
    lbl_eu_80663E24 |= 0x400;
    func_802A1FB4(obj, obj2);
    // Install the fade-in / fade-out resume callback depending on the
    // auto-vision flag captured above.
    if (sub->field_824 & 0x40000) {
        CVisionPtmf cb;
        u32* src = lbl_eu_80533224;
        cb.mPfn = *src++;
        cb.mObj = *src++;
        cb.mDelta = *src;
        f32 f194 = self->field_26194;
        self->mPtmf.mPfn = cb.mPfn;
        self->mPtmf.mObj = cb.mObj;
        self->mPtmf.mDelta = cb.mDelta;
        self->field_2619C = lbl_eu_80667CD4;
        self->field_261A0 = f194;
        __ptmf_test(&cb);
    } else {
        CVisionPtmf cb;
        u32* src = lbl_eu_80533230;
        cb.mPfn = *src++;
        cb.mObj = *src++;
        cb.mDelta = *src;
        f32 f194 = self->field_26194;
        self->mPtmf.mPfn = cb.mPfn;
        self->mPtmf.mObj = cb.mObj;
        self->mPtmf.mDelta = cb.mDelta;
        self->field_2619C = lbl_eu_80667CD4;
        self->field_261A0 = f194;
        __ptmf_test(&cb);
    }
    return 1;
}

// ---------------------------------------------------------------------------
// us-801a8c10: Vision state check. If the installed callback matches the
// 0x114 table entry, clear the vision field; otherwise branch on the 0x120
// entry (retail func_801A74DC).
// ---------------------------------------------------------------------------
void func_801A74DC(CVision* self) {
    u32* tbl = &lbl_eu_80533128[0];
    CVisionPtmf cb114;
    cb114.mPfn = tbl[0x45];
    cb114.mObj = tbl[0x46];
    cb114.mDelta = tbl[0x47];
    if (__ptmf_cmpr(&self->mPtmf, &cb114) != 0) {
        // Not the 0x114 entry: branch on the 0x120 entry.
        CVisionPtmf cb120;
        cb120.mPfn = tbl[0x48];
        cb120.mObj = tbl[0x49];
        cb120.mDelta = tbl[0x4A];
        if (__ptmf_cmpr(&self->mPtmf, &cb120) == 0) {
            self->vt_20(0);
        } else {
            self->vt_20(1);
        }
        return;
    }
    // 0x114 entry matched: clear the vision field.
    self->field_26194 = lbl_eu_80667CD4;
        // Clear the two global state bytes/words (bit-range masks).
        u8 da0 = lbl_eu_80663DA0;
        lbl_eu_80663DA0 = da0 & 0x7E;
        u32 e24 = lbl_eu_80663E24;
        lbl_eu_80663E24 = e24 & 0xFBFFFFFF;
        CVisionSub* sub;
        if (self->sub.field_00 == 0) {
            sub = 0;
        } else {
            sub = &self->sub;
        }
        if (sub->field_824 & 0x20000) {
            func_8009D018(0x30e3, 1);
        }
        // Per-player: the fusion-state query runs twice (test then fetch);
        // the stat is passed as a plain float (MWCC's int->float magic).
        int i = 0;
        do {
            void* p = func_8016FE34((int)getPlayer__Q22cf13CfGameManagerFi(i));
            if (p != 0) {
                if (((s32 (*)(void*))((void**)p)[0x290 / 4])(p) != 0) {
                    void* r = ((void* (*)(void*))((void**)p)[0x290 / 4])(p);
                    s32 out;
                    if (func_80260264(r, 0x39, &out) != 0) {
                        ((void (*)(void*, f32))((void**)r)[0x154 / 4])(r, (f32)out);
                    }
                }
            }
            i++;
        } while (i < 3);
        CVisionPtmf cb;
        cb.mPfn = tbl[0x4B];
        cb.mObj = tbl[0x4C];
        cb.mDelta = tbl[0x4D];
        self->mPtmf = cb;
        self->field_2619C = lbl_eu_80667CD4;
        self->field_261A0 = self->field_26194;
        __ptmf_test(&cb);
}

// us-801a8e38: Drive vision FX updates on the two sub-object fusions and on
// every other in-play actor, then run the per-slot FX / flag management.
void func_801A7704(CVision* self) {
    CVisionSub* sub;
    if (self->sub.field_00 == 0) {
        sub = 0;
    } else {
        sub = &self->sub;
    }
    CVisionBattleObj* p1 = (CVisionBattleObj*)func_8016FE34(func_800B708C__Fi(sub->field_00));
    CVisionBattleObj* p2 = (CVisionBattleObj*)func_8016FE34(func_800B708C__Fi(sub->field_04));
    if (p1 == 0) {
        self->vt_20(1);
        return;
    }
    if (p2 == 0) {
        self->vt_20(1);
        return;
    }
    p1->sub8.s30(0x10);
    func_80170AB0(p1, &self->unk20D4[0]);
    if (p2 != 0 && p1 != p2) {
        p2->sub8.s30(0x10);
        func_80170AB0(p2, &self->unk20D4[1]);
        func_80174C24(p2, 0x40);
    }

    u8 vdlist[8];
    func_80043D90(vdlist);
    func_800F4A98(func_80043F18(vdlist), 0x20, 0);
    u32 off = 0, i = 0;
    for (; i < ((CVisionEnumList*)func_80043F18(vdlist))->count; off += 0x4818, i++) {
        void* elem = func_800F6EAC(func_80043F18(vdlist), i);
        void* obj = __dynamic_cast(elem, 0, &__RTTI__Q22cf13CfObjectActor, &lbl_eu_806618F0, 0);
        if (obj == (void*)p1 || obj == (void*)p2) {
            continue;
        }
        func_80170AB0(obj, (u8*)self + (off + 0x10000) - 0x4efc);
        func_80174C24(obj, 0x40);
    }
    __dt__80043E88(vdlist, -1);
    func_800EA484(CBattleManager::getInstance(), lbl_eu_80667CD4, 3);

    // Push the five sub-item blocks onto the fusion, then copy the first
    // block back into the fusion's own second block.
    CVisionInitBlock* src = sub->items;
    int n = 0;
    do {
        *p1->vf29C(n) = *src;
        n++;
        src++;
    } while (n < 5);
    *p1->vf2A4() = *p1->vf29C(0);
    if (p1 != p2) {
        p1->vf2C4(p2, lbl_eu_80667CD4, lbl_eu_80667D50, lbl_eu_80667CD4);
    }
    sub->field_0C->field_7C = lbl_eu_80667CFC;
    func_801C028C(0, 0);
    func_801C028C(1, 0);
    func_802A1DF0(0);

    // Enumerate the 0x8000000-tagged list; raise a state flag on every
    // actor whose battle-state id is outside [0xa1,0xa4] and != 0xb8.
    u8 vdlist2[8];
    func_80043D90(vdlist2);
    func_800F4A98(func_80043F18(vdlist2), 0x80000, 0);
    for (u32 i = 0; i < ((CVisionEnumList*)func_80043F18(vdlist2))->count; i++) {
        void* elem = func_800F6EAC(func_80043F18(vdlist2), i);
        void* obj = __dynamic_cast(elem, 0, &lbl_eu_80661970, &lbl_eu_806618F0, 0);
        if (obj == 0) {
            self->vt_20(1);
            __dt__80043E88(vdlist2, -1);
            return;
        }
        CVisionActorRef* act = (CVisionActorRef*)obj;
        if (((CVisionBattleObj*)obj)->vf1A4(2) != 0 && act->field_8C != 0xb8) {
            if (act->field_8C < 0xa1 || act->field_8C > 0xa4) {
                act->field_68 |= 0x40;
            }
        }
    }
    __dt__80043E88(vdlist2, -1);
}


void func_801A7D6C(CVision* self, void* r4) {
    CVisionSub* sub;
    if (self->sub.field_00 == 0) {
        sub = 0;
    } else {
        sub = &self->sub;
    }
    u8 vdlist[8];
    func_80043D90(vdlist);
    func_800F4A98(func_80043F18(vdlist), 0x328, 0);
    for (u32 i = 0; i < ((CVisionEnumList*)func_80043F18(vdlist))->count; i++) {
        void* elem = func_800F6EAC(func_80043F18(vdlist), i);
        void* obj = __dynamic_cast(elem, 0, &lbl_eu_806618E8, &lbl_eu_806618F0, 0);
        if (obj == 0) {
            self->vt_20(1);
            __dt__80043E88(vdlist, -1);
            return;
        }
        // Virtual dispatch through vtable slot 0x190.
        ((CVisionEnumElem*)obj)->vf190(0);
    }
    if (getGlobalSda() != 0) {
        getGlobalSda()[0xa8] = 0;
    }
    self->vt_30(1);
    if (r4 != 0) {
        func_80081E90__Q22cf13CfGameManagerFv(1, 0, 0);
        func_8006E5D8();
    }
    func_800EA484(CBattleManager::getInstance(), lbl_eu_80667CF0, 0x13);
    func_801AF934(0);
    if (sub != 0) {
        CVisionFusion* p1 = (CVisionFusion*)func_8016FE34(func_800B708C__Fi(sub->field_00));
        CVisionFusion* p2 = (CVisionFusion*)func_8016FE34(func_800B708C__Fi(sub->field_04));
        if (p1 != 0) {
            p1->field_3388 &= 0xFFFD;
            p1->field_04->v20(0x4000);
        }
        if (p2 != 0) {
            p2->field_3388 &= 0xFFFD;
            p2->field_04->v20(0x4000);
        }
        func_800F449C(sub);
    }
    func_800F4A98(func_80043F18(vdlist), 0x20, 0);
    for (u32 i = 0; i < *(u32*)((u8*)func_80043F18(vdlist) + 0x620); i++) {
        void* elem = func_800F6EAC(func_80043F18(vdlist), i);
        CVisionFusion* actor = (CVisionFusion*)__dynamic_cast(elem, 0, &__RTTI__Q22cf13CfObjectActor, &lbl_eu_806618F0, 0);
        actor->field_3388 &= 0xFFFD;
        actor->field_04->v20(0x4000);
    }
    self->field_26194 = lbl_eu_80667CD4;
    func_801C028C(0, 0);
    func_801C028C(1, 0);
    func_802A1DF0(0);
    CVisionPtmf cb;
    cb.mPfn = __ptmf_null[0];
    cb.mObj = __ptmf_null[1];
    cb.mDelta = __ptmf_null[2];
    lbl_eu_80663E24 &= 0xFFFFFFC0;
    lbl_eu_80663DA0 &= 0x7F;
    self->mPtmf = cb;
    self->field_2619C = lbl_eu_80667CD4;
    self->field_261A0 = self->field_26194;
    __ptmf_test(&cb);
    __dt__80043E88(vdlist, -1);
}

int* func_801A8070(int* param) {
    if (param[1] == 0) {
        return 0;
    }
    return &param[1];
}

// ---------------------------------------------------------------------------
// us-801a986c: Clear all four vision effect slots (retail func_801A8138).
// ---------------------------------------------------------------------------
void func_801A8138(CVision* self) {
    for (int i = 0; i < 4; i++) {
        if (self->effectArray[i] != 0) {
            self->effectArray[i]->field_B0 = 0;
            self->effectArray[i]->field_68 |= 0x40;
        }
        self->effectArray[i] = 0;
    }
}

// us-801a9978: Validate a battle slot against the vision state and, per the
// mode `r26`, either route its FX type or engage per-player FX on it.
// r26 is the vision mode: 0 = refresh only, 1..5 = escalating FX tiers with
// fall-through (tier N also re-arms every lower tier). `stage` tracks how
// many element-array pushes have happened so far; each push appends a
// {tag, scaled stage, 0} entry to the ring at unk261C4.w60.
void func_801A8244(CVision* self, void* r25, int r26, int r27, int r28) {
    CVisionBattleObj* fu = (CVisionBattleObj*)func_8016FE34(func_800B708C__Fi(((CVisionBtlSlot*)r25)->w_00));
    func_8016FE34(func_800B708C__Fi(((CVisionBtlSlot*)r25)->w_04));
    if (fu == 0) {
        self->vt_20(1);
        return;
    }
    int engaged = 0;
    int stage = 0;
    CVisionBtlSlot* slot = (CVisionBtlSlot*)r25;
    // Tier gate: modes 1-3 refuse to re-engage while their flag bit is set.
    switch (r26) {
    case 1:
        if (slot->w_824 & 0x8) return;
        break;
    case 2:
        if (slot->w_824 & 0x4) return;
        break;
    case 3:
        if (slot->w_824 & 0x40) return;
        break;
    }
    if (r26 != 0) {
        self->unk261C4.field_68 = 0;
        self->unk261C4.field_64 = 0;
    }
    if (r26 != 0) {
        u8 vdlist[8];
        func_80043D90(vdlist);
        func_800F4A98(func_80043F18(vdlist), 0x20, 0);
        for (u32 i = 0; i < ((CVisionEnumList*)func_80043F18(vdlist))->count; i++) {
            CVisionBattleObj* obj = (CVisionBattleObj*)func_8016FE34((int)func_800F6EAC(func_80043F18(vdlist), i));
            if (obj->vf308() == 3) {
                obj->vf304(4);
            } else if (obj->vf308() == 0) {
                obj->vf304(2);
            }
        }
        __dt__80043E88(vdlist, -1);
    }
    if (r26 != 0) {
        if (r27 != 0) {
            func_802A2078(r26, r28, fu);
        }
        func_80280D04(r26);
    }

    // Per-mode FX routing. Cases fall through; each block sets its flag bit
    // (and clears the bit the next tier owns), lazily engages the gauge once,
    // then pushes a ring entry while r26 < 4.
    switch (r26) {
    case 5:
        slot->w_824 |= 0x1;
        func_801ACD5C(1);
        func_800F449C(slot);
        func_8018C820((u8*)CBattleManager::getInstance() + 0x194, 0);
        func_800F3970(CBattleManager::getInstance(), fu, 0, 0x17, 0);
        engaged = 1;
    case 4:
        slot->w_824 = (slot->w_824 | 0x2) & ~0x40;
        func_801ACD5C(2);
        if (engaged == 0) {
            func_8018C820((u8*)CBattleManager::getInstance() + 0x194, 0);
            func_800F3970(CBattleManager::getInstance(), fu, 0, 0x18, 0);
            engaged = 1;
        }
    case 3:
        slot->w_824 = (slot->w_824 | 0x4) & ~0x80;
        func_801ACD5C(4);
        if (engaged == 0) {
            func_8018C820((u8*)CBattleManager::getInstance() + 0x194, 0);
            func_800F3970(CBattleManager::getInstance(), fu, 0, 0x19, 0);
            engaged = 1;
        }
        if (r26 < 4) {
            u32 sum = self->unk261C4.field_64 + self->unk261C4.field_68;
            u32 idx = sum % self->unk261C4.w6C;
            CVisionU32F32U32* arr = (CVisionU32F32U32*)self->unk261C4.w60;
            CVisionU32F32U32 e;
            e.a = 3;
            e.b = lbl_eu_80667CE4 * (double)stage + lbl_eu_80667CF0;
            e.c = 0;
            arr[idx] = e;
            self->unk261C4.field_68++;
        }
        stage = 1;
    case 2:
        slot->w_824 = (slot->w_824 | 0x8) & ~0x100;
        func_801ACD5C(8);
        // Track the actor's fusion-readiness bit in the tier-2 flag word.
        if (slot->w_88 & 0x1000) {
            slot->w_824 |= 0x400;
        } else {
            slot->w_824 &= ~0x400;
        }
        if (engaged == 0) {
            func_8018C820((u8*)CBattleManager::getInstance() + 0x194, 0);
            func_800F3970(CBattleManager::getInstance(), fu, 0, 0x1a, 0);
            engaged = 1;
        }
        if (r26 < 4) {
            u32 sum = self->unk261C4.field_64 + self->unk261C4.field_68;
            u32 idx = sum % self->unk261C4.w6C;
            CVisionU32F32U32* arr = (CVisionU32F32U32*)self->unk261C4.w60;
            CVisionU32F32U32 e;
            e.a = 4;
            e.b = lbl_eu_80667CE4 * (double)stage + lbl_eu_80667CF0;
            e.c = 0;
            arr[idx] = e;
            self->unk261C4.field_68++;
        }
        stage = stage + 1;
    case 1:
        slot->w_824 = ((slot->w_824 | 0x10) & ~0x200) | 0x4000;
        func_801ACD5C(0x10);
        if (engaged == 0) {
            func_8018C820((u8*)CBattleManager::getInstance() + 0x194, 0);
            func_800F3970(CBattleManager::getInstance(), fu, 0, 0x1b, 0);
        }
        {
            CBattleManager* bm = CBattleManager::getInstance();
            // vtable +0x34: fade the vision gauge by mode/8 seconds.
            ((void (*)(void*, u32, f32, f32))(*(void***)bm)[0x34 / 4])(
                bm, 0x13, lbl_eu_80667D54,
                lbl_eu_80667CE4 * (double)r26);
        }
        if (r26 < 4) {
            u32 sum = self->unk261C4.field_64 + self->unk261C4.field_68;
            u32 idx = sum % self->unk261C4.w6C;
            CVisionU32F32U32* arr = (CVisionU32F32U32*)self->unk261C4.w60;
            CVisionU32F32U32 e;
            e.a = 5;
            e.b = lbl_eu_80667CE4 * (double)stage + lbl_eu_80667CF0;
            e.c = 0;
            arr[idx] = e;
            self->unk261C4.field_68++;
        }
    default:
        break;
    }

    lbl_eu_80663E28 |= 0x8;
    if (r26 != 5) {
        return;
    }
    // Full-tier engage: fire vf1E8 on the slot and spawn a vision effect for
    // each player whose fusion state passes the func_8026178C probe.
    {
        CVisionArg5 data = *(CVisionArg5*)lbl_eu_80503F80;
        for (int i = 0; i < 3; i++) {
            CVisionBattleObj* pl = (CVisionBattleObj*)func_8016FE34((int)getPlayer__Q22cf13CfGameManagerFi(i));
            if (pl == 0) continue;
            if (pl->vf290() == 0) continue;
            if (func_8026178C((void*)pl->vf290(), 0x8c) == 0) continue;
            u16 objId = ((CVisionObjV*)pl)->field_3F28;
            u32 rv = (u32)fu->vf1E8(data);
            s32 n = (s32)(data.f[4] * (double)rv);
            void* handle = func_8009EC9C(objId);
            func_800A26A4(handle, n, 0, (int)fu->vf108(), 0, 0, 1);
        }
    }
}

// ---------------------------------------------------------------------------
// us-801a4df0: UnkClass_801A36D0 deleting destructor (__dt__801A36D0).
// Clears the sub-object at +0x3d8c (fields at +0x3f90/+0x3f94), then frees.
// ---------------------------------------------------------------------------
extern "C" void* __dt__801A36D0(UnkClass_801A36D0* self, int deleting) {
    if (self != 0) {
        if ((char*)self + 0x3d8c != 0) {
            *(u32*)((char*)self + 0x3f94) = 0;
            *(u32*)((char*)self + 0x3f90) = 0;
        }
        if (deleting > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

// ---------------------------------------------------------------------------
// us-801a4e48: UnkClass_801A3728 deleting destructor (__dt__801A3728).
// Clears field_68/field_64, then frees when the deleting flag is set.
// ---------------------------------------------------------------------------
extern "C" void* __dt__801A3728(UnkClass_801A3728* self, int deleting) {
    if (self != 0) {
        self->field_68 = 0;
        self->field_64 = 0;
        if (deleting > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

cf::CVision::~CVision() {
    *(u32**)this = lbl_eu_805332D0;
    vt_34();
    if (&this->unk261C4 != NULL) {
        this->unk261C4.field_68 = 0;
        this->unk261C4.field_64 = 0;
    }
    __destroy_arr(&this->unk20D4[0],
                  reinterpret_cast<ConstructorDestructor*>(
                      const_cast<void*>(reinterpret_cast<const void*>(&__dt__801A36D0))),
                  sizeof(CVisionSlot), 8);
}

// ---------------------------------------------------------------------------
// us-801a7c18: Release the zero/zero slot of the battle manager's vision
// sub-object (retail func_801A64F8).
// ---------------------------------------------------------------------------
void func_801A64F8() {
    if (CBattleManager::getInstance() &&
        &CBattleManager::getInstance()->mVision) {
        func_801A897C(&CBattleManager::getInstance()->mVision, 0, 0);
    }
}

// ---------------------------------------------------------------------------
// us-801aa050: Release an arbitrary slot of the battle manager's vision
// sub-object (retail func_801A891C). Args are forwarded untouched.
// ---------------------------------------------------------------------------
void func_801A891C(int a, int b) {
    if (CBattleManager::getInstance() &&
        &CBattleManager::getInstance()->mVision) {
        func_801A897C(&CBattleManager::getInstance()->mVision, a, b);
    }
}

// ---------------------------------------------------------------------------
// us-801a97c0: Lazily create the vision effect at `index` and record the
// owning CVision back-pointer (retail func_801A808C).
// ---------------------------------------------------------------------------
void func_801A808C(CVision* self, int index) {
    // Scaled index materialized before the object base is referenced: MWCC
    // colors/creates the value nodes in statement order, which reproduces the
    // retail rlwinm-before-addis schedule.
    u32 addr = (u32)index * 4;
    CVisionEffect** slot =
        (CVisionEffect**)(addr + (u32)self->effectArray);
    if (*slot != 0) {
        return;
    }
    CVisionEffect* eff =
        (CVisionEffect*)func_800451D8(lbl_eu_80503F60[index].field_00, 0);
    *slot = eff;
    if (eff != 0) {
        eff->field_B0 = (u32)self;
    }
}

// ---------------------------------------------------------------------------
// us-801a9830: Clear the vision effect at `index` (retail func_801A80FC).
// ---------------------------------------------------------------------------
void func_801A80FC(CVision* self, int index) {
    if (self->effectArray[index] != 0) {
        self->effectArray[index]->field_B0 = 0;
        self->effectArray[index]->field_68 |= 0x40;
    }
    self->effectArray[index] = 0;
}

// ---------------------------------------------------------------------------
// us-801a9930: Null out the first effect whose object identity matches
// `value` (retail func_801A81FC).
// ---------------------------------------------------------------------------
void func_801A81FC(CVision* self, u32 value) {
    for (int i = 0; i < 4; i++) {
        if (self->effectArray[i] == (CVisionEffect*)value) {
            self->effectArray[i] = 0;
            return;
        }
    }
}

// ---------------------------------------------------------------------------
// us-801aa980: Like func_801A81FC, but also clears the effect's owning
// back-pointer before nulling the slot (retail func_801A924C).
// ---------------------------------------------------------------------------
void func_801A924C(CVision* self, u32 value) {
    for (int i = 0; i < 4; i++) {
        if (self->effectArray[i] == (CVisionEffect*)value) {
            CVisionEffect* e = self->effectArray[i];
            e->field_B0 = 0;
            self->effectArray[i] = 0;
            return;
        }
    }
}

void cf::CVision::func_801A929C(u32 r4) {
    f32 scl = (r4 != 0) ? lbl_eu_80667CD4 : lbl_eu_80667CF0;
    for (int i = 0; i < 4; i++) {
        if (effectArray[i] != 0) {
            func_800ACC50(effectArray[i], scl);
            if (effectArray[i]->mSchedule != 0) {
                func_804E36DC(effectArray[i]->mSchedule, lbl_eu_80667CF0);
            }
        }
    }
}