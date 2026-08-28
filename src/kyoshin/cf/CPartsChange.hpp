#pragma once

#include <types.h>
#include "monolib/math/CVec3.hpp"
#include "kyoshin/plugin/ocBdat.hpp"  // getBdatStringColumnValue (owner)
// Forward decls for folded fake vtables (real defs included in .cpp)
namespace cf { class CObjectState; class CfObject; class CActorParam; class CfObjectActor; }
struct PcSub4VtIf;
struct CfWordView;
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)
#include "monolib/math/FloatUtils.hpp"  // H3 label-owner decl (lbl_eu_8066A208)

// Resource getter (retail unmangled symbol; defined in kyoshin/CMiniMap.cpp).
extern "C" u32 func_8009CF8C(u32 resourceId);

// BDAT table pointer + column-name blob read by func_80195384.
extern void* lbl_eu_806640CC;
extern char lbl_eu_80503C48[];

// BDAT table pointer + column index read by func_801931D0.
extern void* lbl_eu_806640A8;
extern u32 lbl_eu_80664184;

// Party-change helpers used by func_801952CC (retail unmangled C-ABI
// imports from other TUs; C linkage so the call relocs name the retail
// symbols).
extern "C" void func_8009D018(u32 owner, u32 flag);
// Frame clock (retail C-ABI, no-arg form used by func_8019514C): returns the
// current frame timer value (CfRes_getD80Flag is declared in CfResPcImpl.hpp).
// Presentation-mode getter used by func_801949E0 (retail unmangled C-ABI).
extern "C" u32 func_8016E08C();
extern "C" void func_802918AC(int val);
extern "C" int func_80291C60(int v);
extern "C" void func_800A282C(void* obj, int flag);
extern "C" void func_802761C0(void* self);
// CfGameManager statics (retail pre-mangled names; the extern "C" block
// stops MWCC from re-mangling them - CSelShopWin.hpp pattern).
extern "C" {
u32 isResourceFlagSet__Q22cf13CfGameManagerFv(u32 id);
void* getPlayer__Q22cf13CfGameManagerFi(int index);
// BDAT table pointer (func_80194AFC / func_80195BD4 column lookups) and the
// party-count getters used by the range checks (retail pre-mangled names).
u32 getGlobalPtr6409C__Q22cf13CfGameManagerFv();
u32 getQueuedFileEventCount__Q22cf13CfGameManagerFv();
u32 getResourceFromTable__Q22cf13CfGameManagerFv(u32 a);
// Party-change object resolver used by func_80194610 (retail pre-mangled
// name): column lookup returning the sub-object at +0x3E9C (the caller
// de-biases it).
void* func_80081A40__Q22cf13CfGameManagerFv(const char* col, u32 a, u32 b, u32 c);
}
// Global event/presentation flag word (.sbss); bit 0x200000 gates re-arming.

// Party-change refresh helpers (func_80197538): battle-membership check
// (retail unmangled C-ABI import); the battle-manager instance getter
// getInstance__Q22cf14CBattleManagerFv comes from
// kyoshin/cf/CBattleManagerApi.hpp (via cf/CfGameManager.hpp).
extern "C" int func_800DA06C(void* bm, void* obj);

// CfGameManager statics used by func_80195E5C (retail pre-mangled names).
extern "C" u32 getGlobalWord640A4__Q22cf13CfGameManagerFv();
extern "C" int isFieldBlockedByFlag__Q22cf13CfGameManagerFv();
extern "C" int isSceneLoading__Q22cf13CfGameManagerFv();
// Global settings object returned by getUnk80664658 (func_80195E5C reads the
// flag word at +0x214).
struct CfGlobalGimmickView {
    u8 pad_00[0x214];
    /* 0x214 */ u32 field_214;
};
extern "C" CfGlobalGimmickView* getUnk80664658();
// Vector helpers used by func_801953E8 (retail unmangled C-ABI imports).
extern "C" void* func_8004B79C(void* out, void* v);
extern "C" void* func_8004B0B0(void* out);
extern "C" int func_8006DFBC(void* obj);
// Globals read by func_80195E5C / func_80196E04 / func_801953E8: the
// presentation/flag words, the settings timer, the party-slot speed scale
// constants, and the four shared column-name buffers (their 4th character
// is rewritten per iteration to build column names).
extern u32 lbl_eu_80663EE0;
extern u32 lbl_eu_80663D90;
extern float lbl_eu_80663ED8;
extern const f32 lbl_eu_80667B1C;
extern const f32 lbl_eu_80667B20;
extern char lbl_eu_80662500[4];
extern char lbl_eu_80662508[4];
extern char lbl_eu_80662510[4];
extern char lbl_eu_80662520[4];

// Global parts-manager singletons written by the manager ctor (__ct__80193270)
// and read by the element helpers (lbl_eu_80664308 / lbl_eu_8066430C) plus
// the u8/u16 state words cleared by the same ctor.
extern u32 lbl_eu_80664308;
extern u32 lbl_eu_8066430C;
extern u8 lbl_eu_80664310;
extern u16 lbl_eu_80664312;
extern u16 lbl_eu_80664314;

// Manager ctor helpers (retail unmangled C-ABI imports): MemManager handle
// getter + node-array allocation (size first, then handle).
extern "C" u32 func_80061FFC();
extern "C" void* allocate_array__Q23mtl10MemManagerFUlUl(u32 size, u32 handle);
// Actor-list helpers used by func_80193810 (retail unmangled C-ABI imports).
extern "C" void* func_800B8920(void* obj);
extern "C" void func_800B9404(void* obj);
// Party/actor-list helpers used by func_80194264 / func_80197C6C (retail
// unmangled C-ABI imports; the +4 sentinel / +8 element layout is shared with
// code_802B8A3C's OrderGimmickList). CfActorList / CfActorListNode are defined
// below with the other global-scope views.
struct CfActorList;
struct CfActorListNode;
extern "C" CfActorList* func_800B6C7C();
extern "C" void func_800B75EC();
extern "C" void* func_800B957C(u32 id, u32 arg2, const ml::CVec3* pos, f32 f);
extern "C" void func_800B71CC(void* obj);
// Party-drop helper (retail unmangled C-ABI; defined in kyoshin/cf/CItem.cpp).
extern "C" s32 func_80155660(u32 row, u32 flag);
// Sphere-vs-sphere push-apart helper (retail unmangled C-ABI; defined in
// kyoshin/code_800A3B24.cpp). Fills out when the circles overlap.
extern "C" bool func_800A5488(const ml::CVec3& a, const ml::CVec3& b,
                              ml::CVec3* out, float r1, float r2);
// Party-change helpers used by func_80194610 / func_80194D5C (retail
// unmangled C-ABI imports from other TUs). func_800B99BC / func_800B998C
// resolve a target offset against the passed element (opaque outs); the
// 800BExxx helpers drive the target object's flags / state.
extern "C" int func_800B99BC(void* pos, const ml::CVec3* a, int b, int c,
                             float* out1, void* out2, f32 f);
extern "C" int func_800B998C(void* pos, const void* elem, int b, int c,
                             float* out1, void* out2);
extern "C" void func_800BC4B8(void* obj, f32 f);
extern "C" u32 func_8006A6D0();
// BDAT table size/row accessors used by func_80193D48 (retail unmangled).
extern "C" u32 func_8003B41C(void* bdat);
extern "C" u32 func_8003B1EC(void* bdat);
extern "C" void* func_800AA714(void* bdat, u32 id);
extern "C" u32 func_80063A60(void* obj);
// Scene-probe API used by func_801987A4 (retail unmangled C-ABI; monolib
// scn/coli units). func_804BE398 probes the scene query 0x4a05; the 0x4BExxx
// helpers walk the probe result list.
extern "C" int func_804BE398(void* vec, int a, int b, int c, f32 d, f32 e);  // int spellings match CtrlMoveBase.hpp (u32/int made the two extern "C" decls distinct)
extern "C" void* func_804BE520(int index);
extern "C" void* func_804BE50C(u32 index);
extern "C" u32 func_804BE5A4(u32 a, u32 b);
extern "C" int func_804BE53C(void* out, u32 index);
extern "C" u32 func_804BE4AC();
// Sound-slot id table indexed by the party-drop id (func_80197C6C, .sdata).
// Declared with a size so MWCC addresses it via @sda21 (retail li) instead of
// lis/addi (CSuddenCommu.hpp lbl_eu_80662608 pattern).
extern u16 lbl_eu_80662528[1];
// Party-drop sound play (retail CfSoundMan static, mangled name; CfSoundMan.hpp
// pulls nw4r/snd.h which conflicts with this TU's include chain, so the flat
// C-ABI name is declared here - CUICfManager.hpp / CCol6System.hpp precedent).
extern "C" u16 playActorSound__Q22cf10CfSoundManFUlUlUlUlf(
    u32 soundMan, u32 a, u32 b, u32 c, f32 e);  // single shared flat-name form
// Position-offset added to the spawn position Y by func_80197C6C (.sdata2).
extern const f32 lbl_eu_80667AE8;
// Constant passed to the spawned object's vfDC by func_80197C6C (.sdata2).
extern const f32 lbl_eu_80667B24;
// nw4r Warning() file/message strings referenced by func_8019922C (.rodata).
extern const char lbl_eu_80526324[];  // nw4r::db::Warning source file (const per CfObjectImplMove.hpp)
extern const char lbl_eu_80526300[];  // nw4r::db::Warning message

// Retail CPartsChange vtable (.data:0x80532AA8, split1). The retail ctor
// symbol is the splitter-renamed `__ct__cf_CPartsChange` (not MWCC's
// __ct__Q22cf12CPartsChangeFv), so the ctor is written as a C-style function
// that stores this label explicitly - the reloc must name lbl_eu_80532AA8 to
// match the retail ctor's reloc (compiler __vt__ symbol would drift).
extern u8 lbl_eu_80532AA8[];

// Retail _reslist_base<cf::CfPartyInfo> vtable (.data) stored by the dtor
// (same explicit-label treatment as the ctor above).
extern u8 lbl_eu_80532AE4[];

// Retail reslist<CfPartyInfo> vtable (.data:0x80532ACC) stored by the manager
// ctor (__ct__80193270) after the inlined _reslist_base ctor's vtable store.
extern u8 lbl_eu_80532ACC[];

// Vtable interface of the object stored at CPartsChange::mField08
// (func_80192F94 / func_80192E80): -RTTI's two leading vtable slots land the
// first declared virtual at +8, so the 6 dummies pin vf20 to +0x20 and vf24
// to +0x24 (func_80192E80's count/name queries), the next two dummies keep
// vf30 at +0x30 and vf34 at +0x34 (func_80192F94's apply/accept calls).
struct PartsChangeIf {
    virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
    virtual void _v018(); virtual void _v01C(); virtual u32 vf20();
    virtual const char* vf24(int idx);
    virtual void _v028(); virtual void _v02C(); virtual void vf30(u8 a, u32 b);
    virtual int vf34(u8 a);
};

// Format string used by func_80192E80's FixStr<16>::format call (.rodata:
// 0x80503BFC) and the .sdata prefix-string pointer passed as its first
// vararg (.sdata: 0x806624E8; the value is a const char*).
extern char lbl_eu_80503BFC[];
extern const char* lbl_eu_806624E8;

// Ground-probe helper (retail unmangled C-ABI; defined in
// kyoshin/code_800A3B24.cpp, retail 0x800A795C). Raises arg2, probes the
// scene with the query id c (0x44A11 from func_801943E4), and returns 1 on
// a ground hit. C linkage so the call reloc names the retail symbol.
extern "C" int func_800A7094(ml::CVec3* a, ml::CVec3* b, int c, float f, float g);
// Same-TU helpers (retail unmangled symbols; the extern "C" declarations
// give the definitions C linkage so call relocs name the retail symbols).
extern "C" u32 func_801943E4(ml::CVec3* out, const ml::CVec3* in, int flag, f32 fA, f32 fB);
extern "C" int func_801949E0(u32 a, u8 b);// Average-position helper (same-TU sibling; retail unmangled symbol). The
// extern "C" declaration before the definition gives it C linkage, so the
// call reloc names func_80194264 (the definition below keeps its bytes).
extern "C" int func_80194264(f32 f, ml::CVec3* out, const ml::CVec3* in);

// Random-spawn offset table (.data:0x80532AB8, 5 floats) indexed by
// func_801943E4's retry loop (stride-4 pointer walk, retail addi r28,r28,4).
extern const f32 lbl_eu_80532AB8[5];
// .sdata2 pool entries used by func_801943E4 (retail names; the f64 is the
// 2^52 + 2^31 u32->f32 conversion magic loaded once into f31).
extern const f32 lbl_eu_80667AEC;
extern double lbl_eu_80667AF0;
extern const f32 lbl_eu_80667AF8;
extern const f32 lbl_eu_80667AFC;
extern const f32 lbl_eu_80667B00;
extern const f32 lbl_eu_80667B04;
// .sdata2 pool entries used by func_80194610 / func_80194D5C: the epsilon
// and distance thresholds, the collision-out scalar, and the fallback
// speeds. lbl_eu_80663ED0 is a writable global (defined in
// kyoshin/code_800B06A4.cpp).
// .sdata2 pool entries used by func_80194610 / func_80194D5C (retail names).
extern const f32 lbl_eu_80667B08;
extern const f32 lbl_eu_80667B0C;
extern const f32 lbl_eu_80667B10;
extern const f32 lbl_eu_80667B14;
extern const f32 lbl_eu_80667B18;
extern float lbl_eu_80663ED0;
// .sbss/.sdata words used by func_80194D5C (presentation bit) and
// func_80193D48 (BDAT table pointers + column-name string buffers whose
// 4th character is rewritten per iteration to build column names).
extern u32 lbl_eu_80663E28;
extern u32 lbl_eu_80664094;
extern char* lbl_eu_806624F8;
extern char* lbl_eu_806624FC;
// Pseudo-random pivot state for the func_80196434 / func_80196864
// quicksorts (.sdata; cycles through -4..4).
extern s32 lbl_eu_80662518;
extern s32 lbl_eu_8066251C;

namespace cf {

class CPartsChange {
public:
    virtual ~CPartsChange();

    /* 0x04 */ u8 mField04;
    /* 0x08 */ PartsChangeIf* mField08;
    /* 0x0C */ u8 mData[0x20];
    /* 0x2C */ u8 mField2C;
};

// Struct filled by func_80198710 (likely CfPartyInfo)
struct CfPartyInfo {
    /* 0x00 */ u32 field_00;
    /* 0x04 */ u32 field_04;
    /* 0x08 */ u32 field_08;
    /* 0x0C */ s32 field_0C;
    /* 0x10 */ u32 field_10;
    /* 0x14 */ s32 field_14;
    /* 0x18 */ f32 field_18;
    /* 0x1C */ f32 field_1C;
    /* 0x20 */ f32 field_20;
    /* 0x24 */ u32 field_24;
    /* 0x28 */ f32 field_28;
    /* 0x2C */ u8 field_2C;
    /* 0x2D */ u8 field_2D;
    /* 0x2E */ u8 field_2E;

    void func_80195AFC(u8 val);
    void func_80198710(void* r4, float f1, int r5, int r6, float f2, float f3);
};

// Comparator key struct for func_8019641C
struct CfPartyInfoSortKey {
    /* 0x00 */ u32 field_00;
    /* 0x04 */ f32 sortKey;
};

// Wrapper for CActorParam fields accessed by CPartsChange accessors
struct CfActorAccessors {
    u8 pad_00[0x1E];
    /* 0x1E */ u16 mFlags1E;
    u8 pad_20[0x10];
    /* 0x30 */ void* mField30;
    u8 pad_34[0x58];
    /* 0x8C */ f32 mField8C;
    u8 pad_90[4];
    /* 0x94 */ u32 mField94;
    u8 pad_98[6];
    /* 0x9E */ u16 mField9E;

    void func_80193A88(int enable);
    u32 func_80193B04();
    u16 func_80193CC8();
    u32 func_80195284();
    u32 func_80195AC0();
    void func_80195ACC(float val);
    void* func_80195AEC();
};

// Wrapper for CfObjectPc fields at 0x45C4+
struct CfObjectPcExt {
    u8 pad[0x45C4];
    /* 0x45C4 */ u16 mField45C4;
    /* 0x45C6 */ u16 mField45C6;
    /* 0x45C8 */ u16 mField45C8;

    void func_80195AD4(u16 val);
    void func_80195ADC(u16 val);
    u16 func_80195AE4();
    void* func_80195AF4();
};

} // namespace cf

// 8-byte sort entry swapped by func_80196C94: u32 id at +0, f32 value at +4.
// The swap reads the +4 word as raw bits (MWCC stw+lfs bitcast round-trip)
// so the value travels as f32 on one side and as bits on the other.
struct CfPartsSwapEntry {
    /* 0x00 */ u32 field_00;
    /* 0x04 */ f32 field_04;
};

// Comparator object passed to func_80196C94: raw function pointer at +0
// taking two entries; nonzero return orders the two arguments.
typedef int (*CfPartsSwapCmp)(const CfPartsSwapEntry* a, const CfPartsSwapEntry* b);
struct CfPartsSwapCmpObj {
    /* 0x00 */ CfPartsSwapCmp cmp;
};

// 0xC-byte node of the actor list returned by func_800B6C7C / func_800B6BC8:
// mNext at +0, element pointer at +8, sentinel at container +4.
struct CfActorListNode {
    /* 0x00 */ CfActorListNode* mNext;
    u8 pad_04[4];
    /* 0x08 */ void* mElem;
};
struct CfActorList {
    u8* field_00;          // 0x00
    CfActorListNode* mHead; // 0x04
};

// Object created by func_800B957C (func_80197C6C): dispatched virtual slots at
// 0x9C / 0xC4 / 0xDC and the party id at +0x73C. A real virtual class (rather
// than a fn-pointer table) so MWCC emits the retail vtable dispatch (vtable
// and slot both land in r12). -RTTI's two leading vtable slots land the first
// declared virtual at +8.
struct CfSpawnIf {
    virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
    virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
    virtual void _v028(); virtual void _v02C(); virtual void _v030(); virtual void _v034();
    virtual void _v038(); virtual void _v03C(); virtual void _v040(); virtual void _v044();
    virtual void _v048(); virtual void _v04C(); virtual void _v050(); virtual void _v054();
    virtual void _v058(); virtual void _v05C(); virtual void _v060(); virtual void _v064();
    virtual void _v068(); virtual void _v06C(); virtual void _v070(); virtual void _v074();
    virtual void _v078(); virtual void _v07C(); virtual void _v080(); virtual void _v084();
    virtual void _v088(); virtual void _v08C(); virtual void _v090(); virtual void _v094();
    virtual void _v098(); virtual void vf9C(const f32* v);   // 0x9C (position set)
    virtual void _v0A0(); virtual void _v0A4(); virtual void _v0A8(); virtual void _v0AC();
    virtual void _v0B0(); virtual void _v0B4(); virtual void _v0B8(); virtual void _v0BC();
    virtual void _v0C0(); virtual void vfC4(f32 f);          // 0xC4 (float arg)
    virtual void _v0C8(); virtual void _v0CC(); virtual void _v0D0(); virtual void _v0D4();
    virtual void _v0D8(); virtual void vfDC(f32 f);          // 0xDC (float arg)
};
struct CfSpawnObj {
    CfSpawnIf* vt;        // 0x00
    u8 pad_04[0x73C - 0x04];
    /* 0x73C */ u32 field_73C;
};

// View over the +0x3E9C-debiased actor base written by func_80198524: flags
// at +0x3F00 (bit 0x04000000 cleared/set) and the BDAT id at +0x3F28.
struct CfActorPartsView {
    u8 pad_00[0x3F00];
    /* 0x3F00 */ u32 field_3F00;
    u8 pad_3F04[0x3F28 - 0x3F04];
    /* 0x3F28 */ u16 field_3F28;
};

// 12-byte id/state triple shared by the CPartsChange helpers: copied by
// func_80198AC4; the first word is resolved as an actor id by func_80198310.
// (Global scope - the retail helper symbols are unmangled free functions
// defined at global scope in CPartsChange.cpp.)
struct CfPartsTri {
    /* 0x00 */ u32 field_00;
    /* 0x04 */ u32 field_04;
    /* 0x08 */ u32 field_08;
};

// 8-byte-stride list entries (func_801984E4 / func_801984F0); the first word
// is an actor id fed to findObjectById.
struct CfPartsListEntry {
    /* 0x00 */ u32 field_00;
    /* 0x04 */ u32 field_04;
};

// View over a single actor-id word at +0x00 (func_80198310).
struct CfActorIdRef {
    /* 0x00 */ u32 field_00;
};

// Sub-object embedded at +0x3E9C of the func_80194610 target: a real virtual
// interface so MWCC emits the retail lwz-r12 dispatch chain (-RTTI: two leading
// vtable slots land the first declared virtual at +8).
struct CfPartsChgSub {
    virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
    virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
    virtual void _v028(); virtual void _v02C(); virtual void _v030(); virtual void _v034();
    virtual void _v038(); virtual void _v03C(); virtual void _v040(); virtual void _v044();
    virtual void _v048(); virtual void _v04C(); virtual void _v050(); virtual void _v054();
    virtual void _v058(); virtual void _v05C(); virtual void _v060(); virtual void _v064();
    virtual void _v068(); virtual void _v06C(); virtual void _v070(); virtual void _v074();
    virtual void _v078(); virtual void _v07C(); virtual void _v080(); virtual void _v084();
    virtual void _v088(); virtual void _v08C(); virtual void _v090(); virtual void _v094();
    virtual void _v098(); virtual void vf9C(const f32* v);   // 0x9C (position set)
    virtual void _v0A0(); virtual void _v0A4();
    virtual void vfA8(const ml::CVec3* v);                    // 0xA8
    virtual ml::CVec3* vfAC();                                // 0xAC (position)
    virtual void _v0B0(); virtual void _v0B4();
    virtual void vfB8(const ml::CVec3* v, f32 f);             // 0xB8
    virtual void _v0BC(); virtual void _v0C0(); virtual void _v0C4();
    virtual void _v0C8(); virtual void _v0CC();
    virtual void vfD0(f32 f);                                 // 0xD0
    virtual void _v0D4(); virtual void _v0D8(); virtual void _v0DC(); virtual void _v0E0();
    virtual void _v0E4(); virtual void _v0E8(); virtual void _v0EC(); virtual void _v0F0();
    virtual void _v0F4(); virtual void _v0F8(); virtual void _v0FC(); virtual void _v100();
    virtual void _v104(); virtual void _v108(); virtual void _v10C(); virtual void _v110();
    virtual void _v114(); virtual void _v118(); virtual void _v11C(); virtual void _v120();
    virtual void _v124(); virtual void _v128(); virtual void _v12C(); virtual void _v130();
    virtual void _v134(); virtual void _v138(); virtual void _v13C(); virtual void _v140();
    virtual void _v144(); virtual void _v148();
    virtual void vf14C();                                     // filler to keep vf150 aligned
    virtual void vf150(int a);                                // 0x150
};

// De-biased target view for func_80194610 (func_80081A40 returns base+0x3E9C;
// the caller subtracts 0x3E9C): flag words at +0x3F00/+0x3F04 and the u16 ids
// at +0x456C / +0x45C2 / +0x45CA.
struct CfObjVt5B4;
struct CfPartsChgObj3F04 {
    /* 0x0000 */ CfObjVt5B4* vt;
    u8 pad_04[0x3E9C - 0x04];
    /* 0x3E9C */ CfPartsChgSub mSub;
    u8 pad_3EA0[0x3F00 - 0x3EA0];
    /* 0x3F00 */ u32 field_3F00;
    /* 0x3F04 */ u32 field_3F04;
    u8 pad_3F08[0x44D8 - 0x3F08];
    /* 0x44D8 */ f32 field_44D8;
    u8 pad_44DC[0x456C - 0x44DC];
    /* 0x456C */ u16 field_456C;
    u8 pad_456E[0x45C2 - 0x456E];
    /* 0x45C2 */ u16 field_45C2;
    /* 0x45C4 */ u16 field_45C4;
    /* 0x45C6 */ u16 field_45C6;
    /* 0x45C8 */ u16 field_45C8;
    /* 0x45CA */ u16 field_45CA;
};

// Player object returned by getPlayer__Q22cf13CfGameManagerFi in func_80194610:
// vtable at +0 with the position getter at slot 0xAC.
struct CfPlayerVt {
    u32 _padAC[0xAC / 4];
    ml::CVec3* (*vfAC)(void* self);  // 0xAC
};
struct CfPlayerPosView {
    /* 0x00 */ CfPlayerVt* vt;
};

// Virtual-call form of the same player view (vptr at +0, position getter at
// slot 0xAC) so MWCC emits the retail lwz-r12 dispatch in func_80194610.
struct CfPlayerVtACIf {
    virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
    virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
    virtual void _v028(); virtual void _v02C(); virtual void _v030(); virtual void _v034();
    virtual void _v038(); virtual void _v03C(); virtual void _v040(); virtual void _v044();
    virtual void _v048(); virtual void _v04C(); virtual void _v050(); virtual void _v054();
    virtual void _v058(); virtual void _v05C(); virtual void _v060(); virtual void _v064();
    virtual void _v068(); virtual void _v06C(); virtual void _v070(); virtual void _v074();
    virtual void _v078(); virtual void _v07C(); virtual void _v080(); virtual void _v084();
    virtual void _v088(); virtual void _v08C(); virtual void _v090(); virtual void _v094();
    virtual void _v098(); virtual void _v09C(); virtual void _v0A0(); virtual void _v0A4();
    virtual void _v0A8();
    virtual ml::CVec3* vfAC();   // 0xAC (position)
};

// Opaque collision output written by func_800B99BC / func_800B998C
// (byte at +0 is read back by func_80194D5C).
struct CfCollideOut {
    /* 0x00 */ u8 data[12];
};

// View over the +0x1D44 resource slot queried by func_80195290: the slot
// address itself is passed to func_8009CF8C (no load of its contents).
struct CfResSlot {
    u8 pad_00[0x1D44];
    /* 0x1D44 */ u8 field_1D44;
};

// 0x4C-byte element of the array scanned by func_8019747C (u16 id at +0x1C).
// Carries a spawn/current position (mPos + field_0C), the speed/flags fields
// touched by func_80193678 / func_801931D0, a third float at +0x18, u8 flags
// at +0x25/+0x26, and four 9-byte sub-blocks at +0x27, +0x30, +0x39 and +0x42
// (func_8019397C memsets each separately; func_80193D48 fills the full
// element including mPos/field_0C).
struct CfPartsElem4C {
    /* 0x00 */ ml::CVec3 mPos;
    /* 0x0C */ f32 field_0C;
    /* 0x10 */ f32 field_10;
    /* 0x14 */ f32 field_14;
    /* 0x18 */ f32 field_18;
    /* 0x1C */ u16 field_1C;
    /* 0x1E */ u16 field_1E;
    /* 0x20 */ u16 field_20;
    /* 0x22 */ u16 field_22;
    /* 0x24 */ u8 field_24;
    /* 0x25 */ u8 field_25;
    /* 0x26 */ u8 field_26;
    /* 0x27 */ u8 field_27[9];
    /* 0x30 */ u8 field_30[9];
    /* 0x39 */ u8 field_39[9];
    /* 0x42 */ u8 field_42[9];
    /* 0x4B */ u8 pad_4B;
};

// Array view for func_8019747C: 0x4C-stride elements from +0x00, count at
// +0x9800 (0x200 * 0x4C = 0x9800, so mElems ends exactly at the count).
struct CfPartsElemArray {
    /* 0x00 */ CfPartsElem4C mElems[0x200];
    /* 0x9800 */ u32 mCount;
};

// 0xA4-byte element of the 16-entry table scanned by func_80193AB0; u32
// search key at +0x94, actor id at +0x00 (func_80193CD0 list elements).
// The +0x9E u16 is the flag read through func_80193CC8 by func_80197AA0.
struct CfElemA4 {
    /* 0x00 */ u32 field_00;
    u8 pad_04[0x8C];
    /* 0x90 */ f32 field_90;
    /* 0x94 */ u32 field_94;
    /* 0x98 */ u16 field_98;
    /* 0x9A */ u16 field_9A;
    u8 pad_9C[2];
    /* 0x9E */ u16 field_9E;
    /* 0xA0 */ u16 field_A0;
    /* 0xA2 */ u16 field_A2;
};

// Full 0xA4-byte table element as initialized by func_80193B0C: 16 8-byte
// actor-id sub-entries at +0x00 (zeroed by the 0x80 memset), then the state
// block at +0x90 written in func_80193C74 store order. Bit 0 of +0xA0 is the
// in-use flag scanned by func_80193B0C / func_80193810.
struct CfElemA4Full {
    /* 0x00 */ CfPartsListEntry mEntries[16];
    u8 pad_80[0x8C - 0x80];
    /* 0x8C */ f32 field_8C;
    /* 0x90 */ f32 field_90;
    /* 0x94 */ u32 field_94;
    /* 0x98 */ u16 field_98;
    /* 0x9A */ u16 field_9A;
    /* 0x9C */ u16 field_9C;
    /* 0x9E */ u16 field_9E;
    /* 0xA0 */ u16 field_A0;
    /* 0xA2 */ u16 field_A2;
};

// Flags/state words read by func_801974CC from its second argument
// (bit 2 of +0x3F00 gates the scan; +0x456C supplies the shifted id).
struct CfPartsFlagView {
    u8 pad_00[0x3F00];
    /* 0x3F00 */ u32 field_3F00;
    u8 pad_3F04[0x456C - 0x3F04];
    /* 0x456C */ u16 field_456C;
};

// u16 state word at +0x45C0 read by func_80193CD0 (zero means "no lookup").
struct CfPartsIdView {
    u8 pad_00[0x45C0];
    /* 0x45C0 */ u16 field_45C0;
};

// Node of the singly-linked list walked by func_80193CD0: next link at +0x00,
// element pointer at +0x08; the terminal node links back to the head node.
struct CfPartsNode {
    /* 0x00 */ CfPartsNode* next;
    u8 pad_04[4];
    /* 0x08 */ CfElemA4* data;
};

// Container whose list head node lives at +0xA80C (func_80193CD0).
struct CfPartsListA80C {
    u8 pad_00[0xA80C];
    /* 0xA80C */ CfPartsNode* head;
};

// Table view for func_80193AB0: 16 elements of stride 0xA4 at +0xA828.
struct CfTableA4 {
    u8 pad_00[0xA828];
    /* 0xA828 */ CfElemA4 mElems[16];
};

// Init block written by func_80193C74 (memset 0x80, then explicit fields in
// retail store order).
struct CfPartState90 {
    u8 pad_00[0x90];
    /* 0x90 */ f32 field_90;
    /* 0x94 */ u32 field_94;
    /* 0x98 */ u16 field_98;
    /* 0x9A */ u16 field_9A;
    u8 pad_9C[4];
    /* 0xA0 */ u16 field_A0;
    /* 0xA2 */ u16 field_A2;
};

// Walking view for func_80193B0C's free-slot scan: the loop advances a
// pointer through the manager 0xA4 per iteration and reads the in-use flag
// at +0xA8C8 (= mTable[i].field_A0).
struct CfElemFlagWalk {
    u8 pad_00[0xA8C8];
    /* 0xA8C8 */ u16 field_A8C8;
};

// Float constant written by func_80193C74 to +0x90 (.sdata2, r2-sda21).
// `const` so MWCC treats the load as a constant and hoists it (cf.
// MWCC_CASES.md `extern const float` hoist note).
extern const f32 lbl_eu_80667AD4;

// Default value stored to the manager tail floats (+0xB268/+0xB26C) by the
// manager ctor __ct__80193270 (.sdata2).
extern const f32 lbl_eu_80667AD0;

// Fractional-degree scale for func_80198AE0: converts the % 360 remainder of
// the s32 turn value (1/256 degree units) into degrees (.sdata2).
extern const f32 lbl_eu_8066A210;

// Magnitude coefficient applied to func_80198AE0's % 100 remainder (.sdata2).
extern const f32 lbl_eu_80667B4C;

// Rotation scale read by func_80198D44 (.sdata2, FIdx degrees conversion).
extern const f32 lbl_eu_80667B50;

// Turn-angle offset added to (func_80198E0C) / subtracted from
// (func_80198EE8) the source +0x18 angle before the FIdx sin/cos rotation
// (.sdata2).
extern const f32 lbl_eu_8066A204;

// Turn-angle offset (pi/2) added to the +0x18 angle in func_801990F0's
// odd-turn path (.sdata2; sibling constant of lbl_eu_8066A204).
extern const f32 lbl_eu_8066A200;

// Default speed value written to CfPartyInfo::field_28 (.sdata2); also the
// initial x/z accumulator in func_801990F0.
extern const f32 lbl_eu_80667B28;

// Speed-scale constants read by func_801931D0 (.sdata2).
extern const f32 lbl_eu_80667AB8;
extern const f32 lbl_eu_80667ABC;
extern const f32 lbl_eu_80667AC0;
// 2^52 (0x4330000000000000) u32->double conversion magic (.sdata2, lfd).
// Declared here only; the definition lives in another TU so reads stay
// external references carrying the retail-named sda21 reloc.
extern const double lbl_eu_80667AC8;

// Polymorphic receiver for the party-info state pmf table (lbl_eu_80532AF0).
// Abstract so MWCC emits no vtable; the virtuals force 12-byte
// pointer-to-member records (retail `mulli r0,r0,0xc`). The conceptual vptr
// occupies +0x00, so field_0C lands at byte +0x0C (CfPartyInfo::field_0C).
class CfPartyInfoState {
public:
    virtual void _vf00() = 0;
    virtual void _vf04() = 0;
    virtual void _vf08() = 0;
    virtual void _vf0C() = 0;
    virtual void _vf10() = 0;
    virtual void _vf14() = 0;
    virtual void _vf18() = 0;
    virtual void _vf1C() = 0;
    u8 pad_04[0x8];          // 0x04..0x0B (conceptual vptr + pad)
    /* 0x0C */ u32 field_0C; // state index (CfPartyInfo::field_0C)
    virtual void _vf20() = 0;
    virtual void _vf24() = 0;
};

// Party-info state-dispatch table (.data): 12-byte pmf records indexed by the
// +0x0C state word. MWCC lowers (self->*table[idx])(arg) to lis/addi + mulli
// + `bl __ptmf_scall`.
typedef int (CfPartyInfoState::*CfPartyInfoPMF)(CfPartsTri*);
extern CfPartyInfoPMF lbl_eu_80532AF0[];

// Party-info processor tail-called by func_80198AC4 (retail `b`) and called
// by func_80198D44 (retail `bl`). The retail symbol is unmangled, so C
// linkage is required for the call relocs to name func_801987A4. Returns a
// 0/1 selector result (retail cmpwi on r3 at the tail-call sites).
extern "C" int func_801987A4(CfPartsTri* src, CfPartsTri* dst);

// 0xC-byte reslist node (4-byte item) of the retail
// _reslist_base<cf::CfPartyInfo> instantiation.
struct CfPartyListNode {
    /* 0x00 */ CfPartyListNode* mNext;
    /* 0x04 */ CfPartyListNode* mPrev;
    /* 0x08 */ u32 field_08;
};

// Mirror of _reslist_base<cf::CfPartyInfo> (retail instantiation): the
// sentinel node at +0x08 is 0xC bytes, so mList lands at +0x14 and the
// owns-flag at +0x1C - the reslist.hpp template would embed a full 0x38-byte
// node and push these to +0x40/+0x48.
struct CfPartyListBase {
    /* 0x00 */ void* vtable;
    /* 0x04 */ CfPartyListNode* mStartNodePtr;
    /* 0x08 */ CfPartyListNode mStartNode;
    /* 0x14 */ CfPartyListNode* mList;
    /* 0x18 */ int mCapacity;
    /* 0x1C */ u8 mOwnsList;
};

// 8-byte distance-array entry at manager +0x9804 (func_80194D5C appends the
// processed element pointer and squared distance).
struct CfPartsDistEntry {
    /* 0x00 */ void* elem;
    /* 0x04 */ f32 dist;
};

// Big manager object (func_80197AA0 / func_80193678): 0x4C-element array
// with count at +0x9800, the distance array at +0x9804, a u32 at +0xA804, the
// party-info reslist at +0xA808, the 16-entry 0xA4 table at +0xA828, and the
// tail state words written by the ctor __ct__80193270 / func_80193810.
struct CfPartsManager {
    /* 0x0000 */ CfPartsElemArray mElems;
    /* 0x9804 */ CfPartsDistEntry mDist[0x200];
    /* 0xA804 */ u32 field_A804;
    /* 0xA808 */ CfPartyListBase mPartyList;
    /* 0xA828 */ CfElemA4 mTable[16];
    /* 0xB268 */ f32 field_B268;
    /* 0xB26C */ f32 field_B26C;
    /* 0xB270 */ u16 field_B270;
    /* 0xB272 */ u16 field_B272;
    /* 0xB274 */ u16 field_B274;
    /* 0xB276 */ s16 field_B276;
    /* 0xB278 */ u16 field_B278;
};

// Tail view used by func_80196E04: the distance array and its live count
// (+0xA804, immediately after the last entry) addressed through one base.
struct CfPartsTailView {
    /* 0x9804 */ CfPartsDistEntry mDist[0x200];
    /* 0xA804 */ u32 count;
};

// Stack-local collection list used by func_80193810: 0x60 collected actor
// pointers followed by the count word. Kept as a struct so MWCC addresses
// count through memory (retail lwz/stw @ sp+0x188 per use).
struct CfPartsCollectList {
    /* 0x000 */ u32 arr[0x60];
    /* 0x180 */ u32 count;
};

// View of func_80198138's second argument: the +0x3F10 actor id is written
// into the first free slot; +0x45C0 receives the parts id and +0x45C6 the
// slot index.
struct CfPartsSlotChgView {
    u8 pad_00[0x3F10];
    /* 0x3F10 */ u32 field_3F10;
    u8 pad_3F14[0x45C0 - 0x3F14];
    /* 0x45C0 */ u16 field_45C0;
    u8 pad_45C2[0x45C6 - 0x45C2];
    /* 0x45C6 */ u16 field_45C6;
};

// View of func_80197BA4's self object (a CfObjectPc): flags word at +0x3F08
// (bit 0x08000000 gates the re-arm) and the u16 party id at +0x45C8.
struct CfPartsChangeObj {
    u8 pad_00[0x3F08];
    /* 0x3F08 */ u32 field_3F08;
    u8 pad_3F0C[0x45C8 - 0x3F0C];
    /* 0x45C8 */ u16 field_45C8;
};

// Function-pointer view of the CfPartsChangeObj primary vtable (offset +0x00),
// slot +0x2BC: returns a nonzero busy flag. Same pattern as CfActorVt5C4Table
// (CfObjectActor.hpp) - avoids a 176-slot virtual proxy.
typedef int (*CfPartsVt2BCFn)(void* self);
struct CfPartsVtPtr {
    u32* vt;  // 0x00
};
struct CfPartsVt2BCTable {
    u32 _pad[0x2BC / 4];
    CfPartsVt2BCFn fn2BC;  // 0x2BC
};

// Player object returned by getPlayer__Q22cf13CfGameManagerFi: +0x74 holds
// the CMenuLvUp-ish object passed to func_802761C0 (func_801952CC).
struct CfGamePlayerView {
    u8 pad_00[0x74];
    /* 0x74 */ void* field_74;
};

// Layout for func_80198D44's source triple: 12 copied bytes, then the s32
// turn index at +0x14 and the f32 speed scale at +0x18/+0x1C.
struct CfPartsMoveSrc {
    /* 0x00 */ u32 field_00;
    /* 0x04 */ u32 field_04;
    /* 0x08 */ u32 field_08;
    u8 pad_0C[0x14 - 0x0C];
    /* 0x14 */ s32 field_14;
    /* 0x18 */ f32 field_18;
    /* 0x1C */ f32 field_1C;
    /* 0x20 */ f32 field_20; // added for func_80198AE0's magnitude offset
};

// 16 8-byte slots (+0x00) plus the +0x94/+0xA0/+0xA2 state tail and the
// +0x3F10 actor id scanned by func_80198284. Both function params use this
// type so MWCC cannot prove no-aliasing and re-loads field_3F10 inside the
// loop (retail lwz-per-iteration).
struct CfPartsSlots {
    /* 0x00 */ CfPartsListEntry mEntries[16];
    u8 pad_80[0x8C - 0x80];
    /* 0x8C */ f32 field_8C;
    u8 pad_90[0x94 - 0x90];
    /* 0x94 */ u32 field_94;
    /* 0x98 */ u16 field_98;
    /* 0x9A */ u16 field_9A;
    u8 pad_9C[4];
    /* 0xA0 */ u16 field_A0;
    /* 0xA2 */ s16 field_A2;
    u8 pad_A4[0x3F10 - 0xA4];
    /* 0x3F10 */ u32 field_3F10;
};

// Cast-only vtable view of func_80193710's self object: slot +0xA8 is
// dispatched with (self, 1). Dummies pin the offsets (-RTTI's two leading
// vtable slots land the first declared virtual at +8).
struct CfPartsVtA8If {
    virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
    virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
    virtual void _v028(); virtual void _v02C(); virtual void _v030(); virtual void _v034();
    virtual void _v038(); virtual void _v03C(); virtual void _v040(); virtual void _v044();
    virtual void _v048(); virtual void _v04C(); virtual void _v050(); virtual void _v054();
    virtual void _v058(); virtual void _v05C(); virtual void _v060(); virtual void _v064();
    virtual void _v068(); virtual void _v06C(); virtual void _v070(); virtual void _v074();
    virtual void _v078(); virtual void _v07C(); virtual void _v080(); virtual void _v084();
    virtual void _v088(); virtual void _v08C(); virtual void _v090(); virtual void _v094();
    virtual void _v098(); virtual void _v09C(); virtual void _v0A0(); virtual void _v0A4();
    virtual void _v0A8(int arg);  // vtable offset 0xA8 (dispatched by func_80193710 with 1)
};

// View of func_80193710's self object: primary vtable at +0x00, flags word
// at +0x3F00 (bit 2 gates the scan), u16 part id at +0x456C and the u16 id
// at +0x45C0 (note 0x456C < 0x45C0, so the part id sits below the id).
struct CfPartsChgObj {
    /* 0x00 */ CfPartsVtA8If* vt;
    u8 pad_04[0x3F00 - 0x04];
    /* 0x3F00 */ u32 field_3F00;
    u8 pad_3F04[0x456C - 0x3F04];
    /* 0x456C */ u16 field_456C;
    u8 pad_456E[0x45C0 - 0x456E];
    /* 0x45C0 */ u16 field_45C0;
};

// Raw vtable view of the actor object in func_80197DE8: slot +0x5B4 returns
// a base-facing float (CtrlAct.hpp pattern).
struct CfObjVt5B4 {
    void* p00[0x5B4 / 4];
    f32 (*fn_0x5B4)(void* self);  // vtable offset 0x5B4
};

// Full view of func_80197DE8's actor argument (a CfObjectPc): primary vtable
// at +0, the CfResPcParent sub-object vptr at +0x3E9C, the flags/id words and
// the u16 part id at +0x45C0.
struct CfPartsChgObjFull {
    /* 0x0000 */ CfObjVt5B4* vt;
    u8 pad_04[0x3E9C - 0x04];
    /* 0x3E9C */ void* mSubVt;   // sub-object vtable pointer (CfResPcParent)
    u8 pad_3EA0[0x3F00 - 0x3EA0];
    /* 0x3F00 */ u32 field_3F00;
    /* 0x3F04 */ u32 field_3F04;
    /* 0x3F08 */ u32 field_3F08;
    u8 pad_3F0C[0x3F10 - 0x3F0C];
    /* 0x3F10 */ u32 field_3F10;
    u8 pad_3F14[0x3F28 - 0x3F14];
    /* 0x3F28 */ u16 field_3F28;
    u8 pad_3F2A[0x456C - 0x3F2A];
    /* 0x456C */ u16 field_456C;
    u8 pad_456E[0x45C0 - 0x456E];
    /* 0x45C0 */ u16 field_45C0;
};

// Doubly-linked node of the +0xA80C unlink list (func_80197DE8): next at +0,
// prev at +4, element pointer at +8 (same layout as CfActorListNode + prev).
struct CfPartsDNode {
    /* 0x00 */ CfPartsDNode* next;
    /* 0x04 */ CfPartsDNode* prev;
    /* 0x08 */ CfElemA4* data;
};

// Party-position selector state (func_801987A4): the +0x24/+0x28 floats and
// the +0x2C/+0x2D/+0x2E flag bytes.
struct CfPartyPosSel {
    u8 pad_00[0x24];
    /* 0x24 */ f32 field_24;
    /* 0x28 */ f32 field_28;
    /* 0x2C */ u8 field_2C;
    /* 0x2D */ u8 field_2D;
    /* 0x2E */ u8 field_2E;
};

// .sdata2 pool entries used by func_801987A4 (retail names): the offset
// vector components, the probe bounds, and the distance thresholds.
extern const f32 lbl_eu_80667B2C;
extern const f32 lbl_eu_80667B30;
extern const f32 lbl_eu_80667B34;
extern const f32 lbl_eu_80667B38;
extern const f32 lbl_eu_80667B3C;
extern const f32 lbl_eu_80667B40;
extern const f32 lbl_eu_80667B44;
extern const f32 lbl_eu_80667B48;

// Party-list element scanned by func_80194AFC: flags word at +0x68 (bits
// 0x20/0x40/0x60 set conditionally) and the u16 part id at +0x6D0 shifted
// right 4 (sign-extended) to key the BDAT lookups.
struct CfPartyScanElem {
    u8 pad_00[0x68];
    /* 0x68 */ u32 field_68;
    u8 pad_6C[0x6D0 - 0x6C];
    /* 0x6D0 */ u16 field_6D0;
};

// Cast-only vtable view of the sub-object at func_8016FE34(obj)+4: slot
// +0x30 is dispatched with no arguments and returns a word view (dummies
// pin the offset: -RTTI's two leading vtable slots land the first declared
// virtual at +8, so 10 dummies keep vf30 at +0x30).
struct CfWordView {
    /* 0x00 */ u32 field_00;
};
struct CfVt30If {
    virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
    virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
    virtual void _v028(); virtual void _v02C();
    virtual CfWordView* vf30();  // vtable offset 0x30
};

// Same-TU quicksort/apply helpers (retail unmangled symbols; the extern "C"
// declarations give the definitions C linkage so call relocs name the
// retail symbols). Placed after all type definitions.
extern "C" void func_80196C94(CfPartsSwapEntry* a, CfPartsSwapEntry* b,
                               CfPartsSwapEntry* c, CfPartsSwapCmpObj* f);
extern "C" void func_80196864(CfPartsSwapEntry* a, CfPartsSwapEntry* b,
                               CfPartsSwapCmpObj* f);
extern "C" CfPartsChgObj3F04* func_80194610(CfPartsManager* mgr, u32 arg2, u32 arg3,
                                             u32 arg4, CfPartsElem4C* src);
extern "C" void func_80194D5C(CfPartsManager* mgr, const ml::CVec3* pos, f32 f);

// Party-info builder (retail unmangled import): fills a CfPartyInfo-shaped
// record from the source position triple. The second float argument travels
// as the raw (unrounded) u32->f64 conversion result, so it is spelled double.
extern "C" void func_80198710(void* out, const void* src, f32 a, f64 b,
                               u32 c, u32 d);

// Same-TU party-info helpers with unmangled retail names (func_80195E5C
// calls; the declarations give the definitions below C linkage).
extern "C" int func_80194AFC();
extern "C" void func_8019514C(CfPartsManager* self);
extern "C" void func_80196E04(CfPartsManager* mgr, f32 f);
extern "C" void func_80196434(CfPartsSwapEntry* a, CfPartsSwapEntry* b,
                               CfPartsSwapCmp cmp);
// Party-info sort comparator (func_80195E5C takes its address; the retail
// reloc names the unmangled form, so give the definition C linkage).
extern "C" bool func_8019641C(const cf::CfPartyInfoSortKey* a,
                               const cf::CfPartyInfoSortKey* b);
