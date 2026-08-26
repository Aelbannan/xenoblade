#pragma once

#include <types.h>
#include "kyoshin/cf/CtrlMoveBase.hpp"
#include "kyoshin/cf/voice/CCharVoice.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

// Forward decls: full player-object layouts are declared after the cf
// namespace (global scope) - see CtrlPlayerObj below.
class CtrlPlayerObj;
class CtrlPlayerSub3ED4;

// ---------------------------------------------------------------------------
// Small layout views shared by the pad-handler functions (defined before the
// cf namespace so CtrlPc/CtrlPlayerObj can use them as member/return types).
// ---------------------------------------------------------------------------

// self->vf37() (vtable slot 0x9C) result: flag words at 0x00/0x04/0x10/0x14.
struct CtrlPcSub37 {
    u32 mField0;   // 0x00
    u32 mField4;   // 0x04
    u8 _8[0x10 - 0x8];
    u32 mField10;  // 0x10
    u32 mField14;  // 0x14
};

// player->vf164() (vtable slot 0x298) result: combo/state word at +0x48 and
// a clearable word at +0x7C.
struct CtrlPlayerSub298 {
    u8 _00[0x48];
    int mField48;        // 0x48 (signed range checks)
    u8 _4C[0x7C - 0x4C];
    u32 mField7C;        // 0x7C
};

// 0x20-byte AI-action parameter slot (same layout as CVision.cpp's
// CVisionFxParam); installed on player->mField3380 via func_8014AC38.
struct CVisionFxParam {
    u8 unk0[0x6];        // 0x00
    u8 b_06;             // 0x06
    u8 unk07[0xD - 0x07];
    u8 b_0D;             // 0x0D
    u8 unk0E[0x10 - 0x0E];
    u16 h_10;            // 0x10
    u16 h_12;            // 0x12
    f32 f_14;            // 0x14
    u8 unk18[0x20 - 0x18];
};

// +0x3E9C voice-owner interface view (same vtable layout as CVoiceOwnerIntf,
// getPosition at raw slot 0xAC) with typed slots used by this TU:
// 0x08/0x0C state probes (int), 0x10 flag setter, 0x4C handle getter,
// 0x50 source setter, 0xC4 angle/position feed (f32).
class CVoiceOwnerIntfPc {
public:
    virtual int v00(int arg);              // 0x08
    virtual int v01(int arg);              // 0x0C
    virtual void v02(int arg);             // 0x10
    virtual void v03();  virtual void v04();  virtual void v05();
    virtual void v06();  virtual void v07();  virtual void v08();
    virtual void v09();  virtual void v10();  virtual void v11();
    virtual void v12();  virtual void v13();  virtual void v14();
    virtual void v15();  virtual void v16();
    virtual void* v17();                   // 0x4C
    virtual void v18(void* arg);           // 0x50
    virtual void v19();  virtual void v20();  virtual void v21();
    virtual void v22();  virtual void v23();  virtual void v24();
    virtual void v25();  virtual void v26();  virtual void v27();
    virtual void v28();  virtual void v29();  virtual void v30();
    virtual void v31();  virtual void v32();  virtual void v33();
    virtual void v34();  virtual void v35();  virtual void v36();
    virtual void v37();  virtual void v38();  virtual void v39();
    virtual void v40();
    virtual CVoicePos* getPosition();      // 0xAC
    virtual void v42();  virtual void v43();  virtual void v44();
    virtual void v45();  virtual void v46();
    virtual void v47(f32 arg);             // 0xC4
    // Padding out to retail slot 0x220 (used by func_80097A5C).
    virtual void v48();  virtual void v49();  virtual void v50();
    virtual void v51();  virtual void v52();  virtual void v53();
    virtual void v54();  virtual void v55();  virtual void v56();
    virtual void v57();  virtual void v58();  virtual void v59();
    virtual void v60();  virtual void v61();  virtual void v62();
    virtual void v63();  virtual void v64();  virtual void v65();
    virtual void v66();  virtual void v67();  virtual void v68();
    virtual void v69();  virtual void v70();  virtual void v71();
    virtual void v72();  virtual void v73();  virtual void v74();
    virtual void v75();  virtual void v76();  virtual void v77();
    virtual void v78();  virtual void v79();  virtual void v80();
    virtual void v81();  virtual void v82();  virtual void v83();
    virtual void v84();  virtual void v85();  virtual void v86();
    virtual void v87();  virtual void v88();  virtual void v89();
    virtual void v90();  virtual void v91();  virtual void v92();
    virtual void v93();  virtual void v94();  virtual void v95();
    virtual void v96();  virtual void v97();  virtual void v98();
    virtual void v99();  virtual void v100(); virtual void v101();
    virtual void v102(); virtual void v103(); virtual void v104();
    virtual void v105(); virtual void v106(); virtual void v107();
    virtual void v108(); virtual void v109(); virtual void v110();
    virtual void v111(); virtual void v112(); virtual void v113();
    virtual void v114(); virtual void v115(); virtual void v116();
    virtual void v117(); virtual void v118(); virtual void v119();
    virtual void v120(); virtual void v121(); virtual void v122();
    virtual void v123(); virtual void v124(); virtual void v125();
    virtual void v126(); virtual void v127(); virtual void v128();
    virtual void v129(); virtual void v130(); virtual void v131();
    virtual void v132(); virtual void v133();
    virtual void* v134(int arg);           // 0x220
};

namespace cf {
class __declspec(novtable) CtrlRemote {
public:
    virtual ~CtrlRemote();
};

class __declspec(novtable) CtrlPad {
public:
    virtual ~CtrlPad();
};

// novtable like the siblings: the retail dtor/ctor store lbl_eu_80527DB0
// explicitly; suppressing MWCC's own __vt__Q22cf6CtrlPc keeps the reloc
// name byte-identical.
class __declspec(novtable) CtrlPc {
public:
    CtrlPc();
    virtual ~CtrlPc();

    // Pad-handler virtuals (retail vtable lbl_eu_80527DB0; the Nth declared
    // virtual sits at vtable offset (N+2)*4). Only slots this TU calls get
    // real names; the rest are ordering padding.
    virtual void vf01();  virtual void vf02();  virtual void vf03();
    virtual void vf04();  virtual void vf05();  virtual void vf06();
    virtual void vf07();  virtual void vf08();  virtual void vf09();
    virtual void vf10();  virtual void vf11();  virtual void vf12();
    virtual void vf13();  virtual void vf14();  virtual void vf15();
    virtual void vf16();  virtual void vf17();  virtual void vf18();
    virtual void vf19();
    virtual void vf20();  // 0x58
    virtual void vf21();  // 0x5C
    virtual void vf22();  // 0x60
    virtual void vf23();  virtual void vf24();  virtual void vf25();
    virtual void vf26();  virtual void vf27();  virtual void vf28();
    virtual void vf29();  virtual void vf30();  virtual void vf31();
    virtual int vf32();  // 0x88
    virtual void vf33();  // 0x8C
    virtual void vf34();  // 0x90
    virtual void vf35();  // 0x94
    virtual void vf36();  // 0x98
    virtual CtrlPcSub37* vf37();  // 0x9C
    virtual CtrlPcSub37* vf38();  // 0xA0

    // bit testers
    u32 testBit20();
    u32 testBit16();
    u32 testBit15();
    u32 testBit14();
    u32 testBit13();
    u32 testBit12();
    u32 testBit11();
    u32 testBit10();
    u32 testBit9();
    u32 testBit8();

    // setters/getters
    u32 getField1D4();

    // delegate helpers
    void delegateTo899C0();
    void delegateTo89E88();
    void delegateTo89F68();

    int returnTrue();

    // +0x00: implicit vptr (novtable: stored manually as lbl_eu_80527DB0)
    u32 mField4;                 // 0x04 (pad/battle flags: bits 2/3 set by func_80097134/97598)
    char _pad_08[0x0C - 0x08];   // 0x08-0x0B
    f32 mFieldC;                 // 0x0C (target angle/position state)
    f32 mField10;                // 0x10 (computed facing angle)
    f32 mField14;                // 0x14 (aim/fx state flag value)
    u32 mField18;                // 0x18 (combo step count, written by func_80098810)
    char _pad_1C[0x24 - 0x1C];   // 0x1C-0x23
    u32 mField24;                // 0x24 (pad-state written by func_80098A04: -1/0/1)
    char _pad_28[0x2C - 0x28];   // 0x28-0x2B
    u32 mPadFlags;                // 0x2C (bit flags: bits 8-20 tested)
    char _pad_30[0x5C - 0x30];   // 0x30-0x5B
    CtrlPlayerObj* mField5C;     // 0x5C (player actor object)
    char _pad_60[0x7C - 0x60];   // 0x60-0x7B
    ml::CVec3 mPos7C;            // 0x7C-0x87 (position snapshot taken in ctor)
    char _pad_88[0x8C - 0x88];   // 0x88-0x8B
    char mSubObj8C[0x1D4 - 0x8C];// 0x8C-0x1D3 (CCtrlMovePC subobject)
    u32 mField1D4;                // 0x1D4
    u32 mField1D8;                // 0x1D8
    char _pad_1DC[0x380 - 0x1DC];// 0x1DC-0x37F
    u32 mField380;                // 0x380
};

} // namespace cf

// 8-byte stack holder around a CfObjEnumList* (func_80043D90 ctor /
// __dt__80043E88 dtor; func_80043F18 reads the wrapped list pointer).
struct CfEnumListHolder {
    void* list;  // 0x0
    u32 handle;  // 0x4
};

// Enum list returned by func_80043F18: element count at +0x620.
struct CfEnumList {
    u8 _00[0x620];
    u32 count;  // 0x620
};

// CBattleManager view for the fields this TU touches. Named distinctly from
// include/kyoshin/cf/CfGameManager.hpp's CBattleManagerView (different field
// set); cast the shared getInstance result to this at the use sites.
struct CBattleManagerViewPc {
    u8 _00[0x1A8];
    u8 mField1A8;            // 0x1A8 chain region (handed to func_8027936C)
    u8 _1A9[0x1AA - 0x1A9];
    u8 mField1AA;            // 0x1AA battle-state byte
    u8 _1AB[0x20C8 - 0x1AB];
    s16 mField20C8;          // 0x20C8 chain/timer state (signed)
};

// Object behind func_800FE68C (lbl_eu_80663F14): actor-id word at +0x90E4.
struct CfObj90E4 {
    u8 _00[0x90E4];
    u32 mField90E4;  // 0x90E4
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void func_80089F68(void* self);
extern "C" int func_80089B24(cf::CCtrlMoveBase* self, ml::CVec3* out);
extern "C" void func_80089398(cf::CCtrlMoveBase* self, ml::CVec3* dst,
                              const ml::CVec3* src, int flag);
extern "C" int func_800890A8(cf::CCtrlMoveBase* self, ml::CVec3* out, u8* outFlag,
                             const ml::CVec3* src, int flag);
extern "C" int func_80086F9C__Q22cf13CfGameManagerFv(int arg);
// Direct (non-inlinable) call to the CtrlPc destructor (retail symbol name).
extern "C" void* __dt__Q22cf6CtrlPcFv(cf::CtrlPc* obj, int flags);
// CfObjEnumList stack-holder helpers (retail C-ABI names).
extern "C" void func_80043D90(void* holder);
extern "C" void* func_80043F18(void* holder);
extern "C" void func_800F4A98(void* list, u32 type, u32 filter);
extern "C" void* __ct__800FB044(void* list, f32 radius, void* pos, int arg);
extern "C" void __dt__80043E88(void* holder, int flags);
extern "C" void* func_800F6E98(void* list, int index);
// CBattleManager singleton + actor-id query helper (retail C-ABI names).
// Voice/AI-action helpers + C-ABI imports used by the pad-handler funcs.
// (func_80174C98 comes from CtrlMovePC.hpp.)
class UnkClass_800821F8View;   // defined below (func_800821F8 result view)
extern "C" void func_8004DACC(void* obj);
extern "C" void func_8014AC38(void* a, void* b);
extern "C" void func_8027936C(void* a, int b);
extern "C" void* __dynamic_cast(void* src, long offset, const void* src_type,
                                const void* dst_type, void* src2dst);
extern "C" void* func_800FEDF8(void);
extern "C" int func_800DA06C(void* bm, void* obj);
extern "C" void func_8006BC1C(void* obj, u32 mask);
extern "C" void func_8006BBF4(void* obj, u32 mask, int flag);
extern "C" CfObj90E4* func_800FE68C(void);
// (func_800821F8__Q22cf13CfGameManagerFv: single winning decl on
// CfGameManagerApi.hpp - canonical UnkClass_800821F8* view; call sites cast
// to the UnkClass_800821F8View vtable proxy defined below.)
#include "kyoshin/cf/CfGameManagerApi.hpp"
extern "C" const void* lbl_eu_80661C60;   // __dynamic_cast src typeinfo
extern "C" const void* lbl_eu_80661BE8;   // __dynamic_cast dst typeinfo
// In-TU function, C-linkage so the call reloc is the retail name.
extern "C" void* func_80098694(cf::CtrlPc* self);
extern "C" u32 func_80098B74(int index);
// func_800BFC68(CfObjectMove*) (retail mangled name
// func_800BFC68__FPQ22cf12CfObjectMove); proper C++ decl so MWCC emits the
// retail reloc. Returns the player object (CtrlPlayerObj layout at +0x4).
namespace cf {
class CfObjectMove;
}
CtrlPlayerObj* func_800BFC68(cf::CfObjectMove* objMove);

// Global-scope data arrays (MWCC does not mangle global variable names).
extern u32 lbl_eu_80527E98[];
extern u32 lbl_eu_80527F10[];
// sdata2 float constants used by func_80098694 / func_80098194. const
// declarations so MWCC treats the loads as constants and hoists them into
// the prologue (see MWCC_CASES §extern const float hoist).
extern const f32 lbl_eu_80666720;
extern const f32 lbl_eu_80666724;
extern const f32 lbl_eu_80666718;   // facing-angle scale used by func_80096974
// Retail CtrlPc vtable data (.data at 0x80527DB0). Stored manually because
// the class is __declspec(novtable).
extern u8 lbl_eu_80527DB0[];
// Retail CtrlPad vtable data (.data at 0x80527C80), stored manually (novtable).
extern u8 lbl_eu_80527C80[];
// CtrlPad ctor pad-config table: 30 u32 entries (0x78 bytes) fed to
// func_80098BD0(i, tbl[i]) in __ct__cf_CtrlPad.
extern u32 lbl_eu_80527C08[];

// ---------------------------------------------------------------------------
// Player actor object at CtrlPc+0x5C (CfObjectPc-derived). Only the fields /
// vtable slots this TU touches are declared; MWCC emits two leading vtable
// entries (RTTI + offset-to-top), so the Nth declared virtual sits at vtable
// offset (N-1+2)*4. vf14 -> 0x40, vf167 -> 0x2A4.
// ---------------------------------------------------------------------------
class CtrlPlayerSub3ED4 {        // target of player->mSub3ED4
public:
    virtual void vf00();
    virtual void vf01();
    virtual void vf02();
    virtual void vf03();
    virtual void vf04();
    virtual void vf05();
    virtual void vf06();
    virtual void vf07();
    virtual void vf08();
    virtual void vf09();
    virtual void vf10(int a, int b);  // vtable slot 0x30
    virtual void vf11(int arg);       // vtable slot 0x34
    virtual void vf12();
    virtual void vf13();
    virtual int vf14(int flag);       // vtable slot 0x40
};

// player->mField4: sub-object whose vtable slot 0x30 returns a u32 word
// holder (first word read as the actor id fed to func_80174C98).
class CtrlPlayerSub4 {
public:
    virtual void vf00();
    virtual void vf01();
    virtual void vf02();
    virtual void vf03();
    virtual void vf04();
    virtual void vf05();
    virtual void vf06();
    virtual void vf07();
    virtual void vf08();
    virtual void vf09();
    virtual u32* vf30();  // vtable slot 0x30
};

// player->mField3F60 (voice/battle target): flag word at +0x4EC (bits 2/3)
// and a status word at +0x0C (bit 2 tested).
struct CtrlPlayerSub3F60 {
    u8 _00[0xC];
    u32 mFieldC;             // 0x0C (bit 1 tested)
    u8 _10[0x4EC - 0x10];
    u32 mField4EC;           // 0x4EC (bits 1/4 set/clear)
    u8 _4F0[0x510 - 0x4F0];
    f32 mField510;           // 0x510 (target position y snapshot)
};

// CBattleManager view for the fields this TU touches.

struct CtrlPlayerSub50 {         // target of vf2A4-result->mField50
    u8 _0[0x77];
    s8 mField77;                 // 0x77 (pad-handler selector byte, signed cmps)
};

// Retail func_80097A5C / func_80098194 imports.
// func_8017FD44 result view: selector word at +0xB8.
struct Fd44State {
    u8 _00[0xB8];
    u32 mFieldB8;
};
extern "C" Fd44State* func_8017FD44(void);
extern "C" int func_8017FD4C(Fd44State* state);
struct ArtsSelStateViewPc {
    s8 byte0;   // 0x00
    s8 byte1;   // 0x01
    s8 byte2;   // 0x02
};
extern "C" ArtsSelStateViewPc* CMenuArtsSelect_getSelectState(void);
extern "C" void func_800ACF78(void* obj, void* target, u32 child);
extern "C" void func_800BE12C(void* owner, int a, int b, int c, int d);
extern "C" void func_800F6D50(CfEnumList* list, u32 val);
// void* parameter form matches CAIAction.hpp's declaration (two extern "C"
// overloads of the same name are illegal).
extern "C" void* func_800F6E08(void* list);
// Enum list with the extra word at +0x3030 cleared by func_80098194.
struct CfListBig : CfEnumList {
    u8 _624[0x3030 - 0x624];
    u32 mField3030;  // 0x3030
};
// C-linkage so call relocs keep the retail unmangled names (and MWCC does
// not inline the in-TU bodies).
extern "C" void func_80098194(cf::CtrlPc* self, char arg1, char arg2);
extern "C" int func_80097E00(cf::CtrlPc* self);
extern "C" void func_800983B8(cf::CtrlPc* self, char arg);

namespace cf { class CAttackParam; }

struct CtrlPlayerSub2A4 {        // vf2A4() result (vptr at 0)
    void* mVtbl;                 // 0x00
    u32 mField4;                 // 0x04
    u8 _8[0x48 - 0x8];
    u32 mField48;                // 0x48 (combo step count copy)
    u32 mField4C;                // 0x4C
    cf::CAttackParam* mField50;  // 0x50 (selected arts/combo param)
    u8 _54[0x78 - 0x54];
    u32 mField78;                // 0x78 (flag word, bits 0x400/0x800 tested)
};

class CtrlPlayerObj {            // CtrlPc+0x5C player actor
public:
    virtual void vf00();
    virtual void vf01();
    virtual void vf02();
    virtual void vf03();
    virtual void vf04();
    virtual void vf05();
    virtual void vf06();
    virtual void vf07();
    virtual void vf08();
    virtual void vf09();
    virtual void vf10();
    virtual void vf11();
    virtual void vf12();
    virtual void vf13();
    virtual void vf14();
    virtual void vf15();
    virtual void vf16();
    virtual void vf17();
    virtual void vf18();
    virtual void vf19();
    virtual void vf20();
    virtual void vf21();
    virtual void vf22();
    virtual void vf23();
    virtual void vf24();
    virtual void vf25();
    virtual void vf26();
    virtual void vf27();
    virtual void vf28();
    virtual void vf29();
    virtual void vf30();
    virtual void vf31();
    virtual void vf32();
    virtual void vf33();
    virtual void vf34();
    virtual void vf35();
    virtual void vf36();
    virtual void vf37();
    virtual void vf38();
    virtual void vf39();
    virtual void vf40();
    virtual void vf41();
    virtual void vf42();
    virtual void vf43();
    virtual void vf44();
    virtual void vf45();
    virtual void vf46();
    virtual void vf47();
    virtual void vf48();
    virtual void vf49();
    virtual void vf50();
    virtual void vf51();
    virtual void vf52();
    virtual void vf53();
    virtual void vf54();
    virtual void vf55();
    virtual void vf56();
    virtual void vf57();
    virtual void vf58();
    virtual void vf59();
    virtual void vf60();
    virtual void vf61();
    virtual void vf62();
    virtual void vf63();
    virtual void vf64();
    virtual void vf65();
    virtual void vf66();
    virtual void vf67();
    virtual void vf68();
    virtual void vf69();
    virtual void vf70();
    virtual void vf71();
    virtual void vf72();
    virtual void vf73();
    virtual void vf74();
    virtual void vf75();
    virtual void vf76();
    virtual void vf77();
    virtual void vf78();
    virtual void vf79();
    virtual void vf80();
    virtual void vf81();
    virtual void vf82();
    virtual void vf83();
    virtual void vf84();
    virtual void vf85();
    virtual void vf86();
    virtual void vf87();
    virtual void vf88();
    virtual void vf89();
    virtual void vf90();
    virtual void vf91();
    virtual void vf92();
    virtual void vf93();
    virtual void vf94();
    virtual void vf95();
    virtual void vf96();
    virtual void vf97();
    virtual void vf98();
    virtual void vf99();
    virtual void vf100();
    virtual void vf101();
    virtual void vf102();
    virtual void vf103();
    virtual void vf104();
    virtual void vf105();
    virtual void vf106();
    virtual void vf107();
    virtual void vf108();
    virtual void vf109();
    virtual void vf110();
    virtual void vf111();
    virtual void vf112();
    virtual void vf113();
    virtual void vf114();
    virtual void vf115();
    virtual void vf116();
    virtual void vf117();
    virtual void vf118();
    virtual void vf119();
    virtual void vf120();
    virtual void vf121();
    virtual void vf122();
    virtual void vf123();
    virtual void vf124();
    virtual void vf125();
    virtual void vf126();
    virtual void vf127();
    virtual void vf128();
    virtual void vf129();
    virtual void vf130();
    virtual void vf131();
    virtual void vf132();
    virtual void vf133();
    virtual void vf134();
    virtual void vf135();
    virtual void vf136();
    virtual void vf137();
    virtual void vf138();
    virtual void vf139();
    virtual void vf140();
    virtual void vf141();
    virtual void vf142();
    virtual void vf143();
    virtual void vf144();
    virtual void vf145();
    virtual void vf146();
    virtual void vf147();
    virtual void vf148();
    virtual void vf149();
    virtual void vf150();
    virtual void vf151();
    virtual void vf152();
    virtual void vf153();
    virtual void vf154();
    virtual void vf155();
    virtual void vf156();
    virtual void* vf157();               // vtable slot 0x27C (CArtsSet holder)
    virtual void vf158();
    virtual void vf159();
    virtual void vf160();
    virtual void vf161();
    virtual void vf162();
    virtual void vf163();
    virtual CtrlPlayerSub298* vf164();  // vtable slot 0x298
    virtual void vf165();
    virtual void vf166();
    virtual CtrlPlayerSub2A4* vf167();  // vtable slot 0x2A4
    virtual void vf168();
    virtual void vf169();
    virtual void vf170();
    virtual void vf171();
    virtual void vf172();
    virtual int vf173();                // vtable slot 0x2BC
    // Padding to the slots used by func_80097A5C / func_80098194.
    virtual void vf174(); virtual void vf175(); virtual void vf176();
    virtual void vf177(); virtual void vf178(); virtual void vf179();
    virtual void vf180(); virtual void vf181(); virtual void vf182();
    virtual void vf183(); virtual void vf184(); virtual void vf185();
    virtual void vf186(); virtual void vf187();
    virtual void vf2F8(int arg);        // vtable slot 0x2F8
    virtual void vf189(); virtual void vf190(); virtual void vf191();
    virtual void vf192(); virtual void vf193(); virtual void vf194();
    virtual void vf195(); virtual void vf196(); virtual void vf197();
    virtual void vf198(); virtual void vf199(); virtual void vf200();
    virtual void vf201(); virtual void vf202(); virtual void vf203();
    virtual void vf204(); virtual void vf205(); virtual void vf206();
    virtual void vf207(); virtual void vf208(); virtual void vf209();
    virtual void vf210(); virtual void vf211(); virtual void vf212();
    virtual void vf213(); virtual void vf214(); virtual void vf215();
    virtual void vf216(); virtual void vf217(); virtual void vf218();
    virtual void vf219(); virtual void vf220(); virtual void vf221();
    virtual void vf222(); virtual void vf223(); virtual void vf224();
    virtual void vf225(); virtual void vf226(); virtual void vf227();
    virtual void vf228(); virtual void vf229(); virtual void vf230();
    virtual void vf231(); virtual void vf232(); virtual void vf233();
    virtual void vf234(); virtual void vf235(); virtual void vf236();
    virtual void vf237(); virtual void vf238(); virtual void vf239();
    virtual void vf240(); virtual void vf241(); virtual void vf242();
    virtual void vf243(); virtual void vf244(); virtual void vf245();
    virtual void vf246(); virtual void vf247(); virtual void vf248();
    virtual void vf249(); virtual void vf250(); virtual void vf251();
    virtual void vf252(); virtual void vf253(); virtual void vf254();
    virtual void vf255(); virtual void vf256(); virtual void vf257();
    virtual void vf258(); virtual void vf259(); virtual void vf260();
    virtual void vf261(); virtual void vf262(); virtual void vf263();
    virtual void vf264(); virtual void vf265(); virtual void vf266();
    virtual void vf267(); virtual void vf268(); virtual void vf269();
    virtual void vf270(); virtual void vf271(); virtual void vf272();
    virtual void vf273(); virtual void vf274(); virtual void vf275();
    virtual void vf276(); virtual void vf277(); virtual void vf278();
    virtual void vf279(); virtual void vf280(); virtual void vf281();
    virtual void vf282(); virtual void vf283(); virtual void vf284();
    virtual void vf285(); virtual void vf286(); virtual void vf287();
    virtual void vf288(); virtual void vf289(); virtual void vf290();
    virtual void vf291(); virtual void vf292(); virtual void vf293();
    virtual void vf294(); virtual void vf295(); virtual void vf296();
    virtual void vf297(); virtual void vf298(); virtual void vf299();
    virtual void vf300(); virtual void vf301(); virtual void vf302();
    virtual void vf303(); virtual void vf304(); virtual void vf305();
    virtual void vf306(); virtual void vf307(); virtual void vf308();
    virtual void vf309(); virtual void vf310(); virtual void vf311();
    virtual void vf312(); virtual void vf313(); virtual void vf314();
    virtual void vf315(); virtual void vf316(); virtual void vf317();
    virtual void vf318(); virtual void vf319(); virtual void vf320();
    virtual void vf321(); virtual void vf322(); virtual void vf323();
    virtual void vf324(); virtual void vf325(); virtual void vf326();
    virtual void vf327(); virtual void vf328(); virtual void vf329();
    virtual void vf330(); virtual void vf331(); virtual void vf332();
    virtual void vf333(); virtual void vf334(); virtual void vf335();
    virtual void vf336(); virtual void vf337(); virtual void vf338();
    virtual void vf339(); virtual void vf340(); virtual void vf341();
    virtual void vf342(); virtual void vf343(); virtual void vf344();
    virtual void vf345(); virtual void vf346(); virtual void vf347();
    virtual void vf348(); virtual void vf349(); virtual void vf350();
    virtual void vf351(); virtual void vf352(); virtual void vf353();
    virtual void vf354(); virtual void vf355(); virtual void vf356();
    virtual void vf357(); virtual void vf358(); virtual void vf359();
    virtual void vf360(); virtual void vf361(); virtual void vf362();
    virtual void vf363(); virtual void vf364(); virtual void vf365();
    virtual void* vf5C0();              // vtable slot 0x5C0

    CtrlPlayerSub4* mField4;      // 0x04 (sub-object, vf30 -> u32* word holder)
    u8 mField8;                   // 0x08 (state region handed to func_80148778)
    u8 _9[0x1530 - 0x9];
    u32 mField1530;               // 0x1530
    u8 _1534[0x3380 - 0x1534];
    CVisionFxParam mField3380;    // 0x3380 (AI-action slot installed via func_8014AC38)
    u8 _33A0[0x3E9C - 0x33A0];
    CVoiceOwnerIntfPc mSub3E9C;   // 0x3E9C embedded voice-owner interface (getPosition @ 0xAC)
    u8 _3EA0[0x3ED4 - 0x3EA0];
    CtrlPlayerSub3ED4* mSub3ED4;  // 0x3ED4
    u8 _3ED8[0x3F10 - 0x3ED8];
    u32 mField3F10;               // 0x3F10
    u8 _3F14[0x3F28 - 0x3F14];
    u16 mField3F28;               // 0x3F28
    u8 _3F2A[0x3F60 - 0x3F2A];
    CtrlPlayerSub3F60* mField3F60;// 0x3F60 (voice/battle target)
};

// ---------------------------------------------------------------------------
// Helper views used by the pad-handler functions in this TU.
// ---------------------------------------------------------------------------

// Result of __dynamic_cast on player->mSub3ED4 in func_800983B8; slot 0xF8.
class CtrlPlayerSub3ED4Cast {
public:
    virtual void vf00();  virtual void vf01();  virtual void vf02();
    virtual void vf03();  virtual void vf04();  virtual void vf05();
    virtual void vf06();  virtual void vf07();  virtual void vf08();
    virtual void vf09();  virtual void vf10();  virtual void vf11();
    virtual void vf12();  virtual void vf13();  virtual void vf14();
    virtual void vf15();  virtual void vf16();  virtual void vf17();
    virtual void vf18();  virtual void vf19();  virtual void vf20();
    virtual void vf21();  virtual void vf22();  virtual void vf23();
    virtual void vf24();  virtual void vf25();  virtual void vf26();
    virtual void vf27();  virtual void vf28();  virtual void vf29();
    virtual void vf30();  virtual void vf31();  virtual void vf32();
    virtual void vf33();  virtual void vf34();  virtual void vf35();
    virtual void vf36();  virtual void vf37();  virtual void vf38();
    virtual void vf39();  virtual void vf40();  virtual void vf41();
    virtual void vf42();  virtual void vf43();  virtual void vf44();
    virtual void vf45();  virtual void vf46();  virtual void vf47();
    virtual void vf48();  virtual void vf49();  virtual void vf50();
    virtual void vf51();  virtual void vf52();  virtual void vf53();
    virtual void vf54();  virtual void vf55();  virtual void vf56();
    virtual void vf57();  virtual void vf58();  virtual void vf59();
    virtual void vf60(void* arg);          // 0xF8
};

// func_800B708C result (voice handle); slot 0xAC = vf41.
class CtrlVoiceHandle {
public:
    virtual void vf00();  virtual void vf01();  virtual void vf02();
    virtual void vf03();  virtual void vf04();  virtual void vf05();
    virtual void vf06();  virtual void vf07();  virtual void vf08();
    virtual void vf09();  virtual void vf10();  virtual void vf11();
    virtual void vf12();  virtual void vf13();  virtual void vf14();
    virtual void vf15();  virtual void vf16();  virtual void vf17();
    virtual void vf18();  virtual void vf19();  virtual void vf20();
    virtual void vf21();  virtual void vf22();  virtual void vf23();
    virtual void vf24();  virtual void vf25();  virtual void vf26();
    virtual void vf27();  virtual void vf28();  virtual void vf29();
    virtual void vf30();  virtual void vf31();  virtual void vf32();
    virtual void vf33();  virtual void vf34();  virtual void vf35();
    virtual void vf36();  virtual void vf37();  virtual void vf38();
    virtual void vf39();  virtual void vf40();
    virtual void* vf41();                  // 0xAC
};

// func_800821F8 result (game-manager sub-object): flag word at +0x04 and a
// slot-0x2C dispatcher.
class UnkClass_800821F8View {
public:
    virtual void vf00();  virtual void vf01();  virtual void vf02();
    virtual void vf03();  virtual void vf04();  virtual void vf05();
    virtual void vf06();  virtual void vf07();  virtual void vf08();
    virtual void vf09(void* arg);          // 0x2C
    u32 mField4;                           // 0x04 (after implicit vptr)
};

// C-linkage base/sub ctors (retail short-form names; extern "C" keeps call
// relocs matching the retail symbols).
extern "C" void __ct__CtrlRemote(void* obj, void* posObj, int arg5);
extern "C" void __ct__800D10DC(void* obj, void* posObj, void* arg5);
// Pad-config writer used by the __ct__cf_CtrlPad loop (retail 0x800995A8).
extern "C" void func_80098BD0(int index, u32 value);
// Arts/combo helpers used by func_80098810 (retail short-form names).
struct CAttackParamVt {
    virtual void v00();
    virtual int getComboId();        // vtable slot 0x0C
    virtual void applyCombo(int);    // vtable slot 0x10
};
// Polymorphic view of cf::CAttackParam with the vptr at +0x84 (retail
// layout): lets func_80098810 issue true virtual dispatches through the
// record's +0x84 table exactly as the retail code does.
struct CAttackParam84Head {
    u8 _00[0x84];
};
class CAttackParamVtObj : private CAttackParam84Head {
public:
    virtual void vt00();            // +0x08
    virtual int getComboId();       // +0x0C
    virtual void applyCombo(int);   // +0x10
};
extern "C" void* getArtsParamRC2(void* artsSet, int row, int col);
extern "C" int func_801B202C(void);
extern "C" int func_801B1D4C(int arg);
extern "C" int func_801B1FA4(void);
