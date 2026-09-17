#pragma once

#include <types.h>
#include "monolib/device/CFileHandle.hpp"

// Owner-class definitions for the recovered virtual calls in this TU.
// The three hidden free-function names keep this TU's retail-arity
// prototypes (same scheme as CActParamAnim.cpp's func_80055B88_typed_hidden):
// the canonical headers declare these symbols with a different arity, which
// would collide with the byte-exact decls further down. The class
// definitions (CActParamAnim, CScnItemModel) come through intact.
#define writeVec3f writeVec3f_void_hidden
#define func_8004B9D4 func_8004B9D4_hidden
#define Scn_SetupAnimDefault Scn_SetupAnimDefault_1arg_hidden
#define Scn_FindCamItem Scn_FindCamItem_s32_hidden
#define Scn_IsAnimActiveOrNull Scn_IsAnimActiveOrNull_void_hidden
#define lbl_eu_805262F0 lbl_eu_805262F0_constchar_hidden
#define lbl_eu_805262C8 lbl_eu_805262C8_constchar_hidden
#define zero__Q22ml5CVec3 zero__Q22ml5CVec3_ml_hidden
#define func_804BE398 func_804BE398_u32_hidden
#define simGetLeafActData simGetLeafActData_void_hidden
#define lbl_eu_8066A200 lbl_eu_8066A200_float_hidden
#include "kyoshin/action/CActParamAnim.hpp"
#include "libs/monolib/src/scn/CScnItemModel.hpp"
#include "kyoshin/cf/object/CfObject.hpp"
#include "kyoshin/cf/object/CfObjectModel.hpp"
#include "kyoshin/cf/IResInfo.hpp"
#undef writeVec3f
#undef func_8004B9D4
#undef Scn_SetupAnimDefault
#undef Scn_FindCamItem
#undef Scn_IsAnimActiveOrNull
#undef lbl_eu_805262F0
#undef lbl_eu_805262C8
#undef zero__Q22ml5CVec3
#undef func_804BE398
#undef simGetLeafActData
#undef lbl_eu_8066A200

namespace cf { class CActParamAnimGame; class CfObjectMove; }

// Async file event handed to IWorkEvent callbacks by CDeviceFile. The
// cf::CEventFile in CfScript.hpp is a different type; this TU uses the global
// ::CEventFile (retail OnFileEvent__15CModelDispEquipFP10CEventFile).
struct CEventFile {
    s32 field_00;          // 0x00 event type (cmpwi in OnFileEvent)
    CFileHandle* field_04; // 0x04 file handle
};

// File slot: 12 bytes per entry
struct FileSlot {
    CFileHandle* handle; // 0x00 file handle (CDeviceFile::readFile result)
    u8* data;            // 0x04 loaded file buffer (CFileHandle::mData)
    u8 flag;             // 0x08
    u8 _pad[3];
};

// 8-byte buffer-budget context built by PartyStateWin_InitMemCounters / PartyStateWin_PickAllocHandle
// (retail: two remaining-size words at +0/+4, decremented by PartyStateWin_PickAllocHandle
// when it picks the MEM1/MEM2 alloc handle for the file size).
struct CModelDispFileCtx {
    u32 field_0x00; // +0 remaining MEM1 budget
    u32 field_0x04; // +4 remaining MEM2 budget
};

// Object pointed to by animPtrs[i] in ModelDispEquip_StepFadeIn: four color words at +0x40.
struct CModelDispAnimColor {
    u8 _00[0x40];
    u32 field_0x40; // +0x40
    u32 field_0x44; // +0x44
    u32 field_0x48; // +0x48
    u32 field_0x4C; // +0x4C
};

// View of the 0x53C-byte act-param object. The retail cf::CActParamAnimGame is
// larger in the shared header; this TU only needs the flag word at +0x0C and
// the model-pointer slot at +0x4B4 (getAnimChain result).
struct CActParamAnimView {
    u8 _00[0x0C];
    u32 field_0x0C;   // +0x0C busy flags (bit 0x20 toggled around setTurnScale)
    u8 _10[0x364];
    s32 field_0x374;  // +0x374 anim state, signed so == compares emit cmpwi
    u32 field_0x378;  // +0x378 slot index (ModelDispEquip_BuildPartyModel stores i)
    u8 _37C[0x13C - 0x4];
    void* field_0x4B4; // +0x4B4 model/next-chain-obj pointer (getAnimChain result)
    u8 _4B8[0x84];
};

// Effect-instance view written by ModelDispEquip_HandleSlotCmd: word at +0x14 (result of the
// vtable+0xA8 call) and a byte at +0x59 (active flag, set in the cmd==3 case).
struct CModelDispEffectView {
    u8 _00[0x14];
    u32 field_0x14; // +0x14
    u8 _18[0x41];
    u8 field_0x59;  // +0x59
};

// Command buffer passed to ModelDispEquip_HandleSlotCmd: byte +0xA selects the command
// (2 = load model slot, 3 = mark active); +0x1C holds an optional numeric
// string that is atoi'd when its first byte is a digit.
struct CModelDispEquipCmd {
    u8 _00[0x0A];
    u8 field_0x0A;  // +0x0A command
    u8 _0B[0x11];   // +0x0B..0x1B
    char field_0x1C; // +0x1C string start
};

// (was a +0x48 float-setter pad here: virtual dispatch at vtable offset +0x48 taking one
// float arg. Retail table lbl_eu_8056DD70 word 18 is CScnItemModel::vfunc48;
// call sites now use the owning class directly.)

// (was a +0xA8 chain-getter pad here: virtual dispatch at vtable offset +0xA8 returning a
// pointer. Retail table lbl_eu_8056DD70 word 42 is CScnItemModel::vfuncA8;
// call sites now use the owning class directly.)

// Opaque object returned by the Bdat/equip lookups in this TU.
struct CModelDispObj {
    u8 _00[4];
};

// Name/param record returned by CfObjectMove_getBdatNameCol11 / CfObjectMove_getBdatNameCol7 / func_800BBC08.
struct CModelDispNameParam {
    u8 _00[4];
};

// Character/equip record returned by func_8009EC9C (ModelDispEquip_BuildEquipModel): the five
// equip ids at +0x0E..0x16 and the u16 weapon id at +0x18. The equip ids are
// u16 (retail lhz + extsh pairs at the call sites) - callers sign-extend.
struct CModelDispCharRecord {
    u8 _00[0x0E];
    u16 field_0x0E; // equip slot 1 id
    u16 field_0x10; // equip slot 2 id
    u16 field_0x12; // equip slot 3 id
    u16 field_0x14; // equip slot 4 id
    u16 field_0x16; // equip slot 5 id
    u16 weaponId;   // +0x18
};

struct CModelDispParamSlot;

// (was a +0x08 lookup pad here: virtual dispatch at +0x08 on the equipment param-slot
// objects (param[idx].field_2C), taking the owning slot and the weapon id.
// The +0x2C word is a ::CResLookup (cf. CfResPcTableEntry in CfResPcImpl.hpp),
// whose first virtual is getResourceBase(void*, int) at +0x08; call sites now
// use the owning class directly.)

// Per-equipment param slot (stride 0x3C); +0x2C holds a vtable-slot-0x8 obj.
struct CModelDispParamSlot {
    u8 _00[0x2C];
    CResLookup* field_2C; // +0x2C
    u8 _30[0x3C - 0x30];
};

// (was a display-model dispatch pad here: virtual dispatch on the built display model
// (holder->field_0x00) at +0x48 (float), +0x64 (int), +0x9C (2 ints),
// +0xC4 (3 args), +0xC8 (1 arg). Retail table lbl_eu_8056DD70 words 18/25/39
// /48/49 are CScnItemModel::vfunc48/64/9C/C4/C8; call sites now use the
// owning class directly.)

// (was a move-dispatch pad here: virtual dispatch on the actor's embedded CfObjectMove
// (actor+0x3E9C) at +0x74 (no args), +0x148 (index arg), +0x184 / +0x188
// (no args). These are cf::CfObject::CfObject_isMoveActiveNow (bool, +0x74),
// cf::CfObject::CfObject_getSlotBits (u32, +0x148, widened to the retail
// index arity), cf::CfObjectModel::CfObjectModel_getAnimState (void*,
// +0x184) and CfObjectModel_getAnimFlags (u32, +0x188); call sites now use
// the owning classes directly.)

// (was a +0xE0 flush pad here: virtual dispatch at vtable offset +0xE0, the flush
// method on the act-param objects. CActParamAnim declares 59 virtuals with
// MWCC's 2 leading slots, so index 54 lands at +0xE0 (the __vt_48/__vt_78/
// __vt_F0 names confirm the (N+2)*4 layout): CActParamAnim::func_8004B114.
// Call sites now use the owning class directly.)

// (was a +0x28 notify pad here: virtual dispatch at vtable offset +0x28 taking a
// string and a flag. The field_3F34 target carries the +0x7A4 word of
// CScnItemModel, and retail table lbl_eu_8056DD70 word 10 is
// CScnItemModel::vfunc28; call sites now use the owning class directly.)

// View of the CMcaFile constructed at ModelDispEquip_BuildPartyModel's stack frame.
struct CModelDispMca {
    u32 field_00;  // +0x00 vtable
    u8 field_04;   // +0x04 flag
    u8 _05[3];
    u8* field_08;  // +0x08
    u8* mDataAdj;  // +0x0C (passed to Scn_InitGlobalA)
};

// Parent object whose +0x3A0 field must equal the holder's model pointer
// (ModelDispEquip_SwapPartyEquip's early guard).
struct CModelDispParent {
    u8 _00[0x3A0];
    CModelDispObj* field_0x3A0; // +0x3A0
};

// 12-byte filter triple copied by value out of lbl_eu_80507FDC in
// ModelDispEquip_SwapPartyEquip (retail loads it as one struct: lwzu/lwz/lwz).
struct CModelDispFilterTbl {
    u32 slot[3];
};

// Actor container (getCfObjectPc result): CfObjectMove embedded at +0x3E9C.
struct CModelDispActor {
    u8 _00[0x3E9C];
    u8 move[0x3F08 - 0x3E9C]; // embedded CfObjectMove (vtable at +0)
    u32 field_3F08;           // +0x3F08 flags (bits 12/13/17 tested)
    u8 _3F0C[0x3F28 - 0x3F0C];
    u16 field_3F28;           // +0x3F28
    u32 field_3F2C;           // +0x3F2C
    u32 field_3F30;           // +0x3F30
    u32 field_3F34;           // +0x3F34
};

// 8-byte enum-list holder (CTaskGame_enumListCtor / __dt__80043E88).
struct CModelDispListHolder {
    u32 field_00;
    u32 field_04;
};

// Enum list returned by CTaskGame_enumListGet: element count at +0x620.
struct CModelDispEnumList {
    u8 _pad00[0x620];
    u32 field_620; // +0x620
};

// Enum-list slot from func_800F6EC0: move object pointer at +0x04.
struct CModelDispSlot {
    u32 field_00;
    cf::CfObjectMove* field_04; // +0x04
};

// Sub-object at CModelDispEquip+0x10: 3 vtable words + act-param objects.
// The animModelPtrs array sits at the holder's tail (+0xFC8) - retail code
// addresses it through the holder base (e.g. ModelDispEquip_RearmAnimSlot's lwz 0xfc8(r31)).
struct CActParamHolder {
    void* field_0x00; // +0x00 object pointer (vtable dispatch at 0xC4/0xC8)
    CModelDispObj* field_0x04; // +0x04 anim object (func_800584B8 result)
    CModelDispObj* field_0x08; // +0x08 anim object (func_800584B8 result)
    CActParamAnimView actParam;    // +0x0C (0x53C bytes)
    s32 timer;        // +0x548
    CModelDispObj* unk_55C; // +0x54C loaded-model record (Scn_InitGlobalA result)
    CActParamAnimView actParams[2]; // +0x550 (0xA78 bytes)
    void* animModelPtrs[2]; // +0xFC8 animation model slot pointers (indexed by r5/i)
    u32 equipPtrs[8];       // +0xFD0
    u32 currentModelPtr;    // +0xFF0
    void* animPtrs[2];      // +0xFF4 effect-slot color targets (== equip+0x1004)
};

// Flat view of the memory past the embedded holder (0xFD0): alias of the
// CActParamHolder tail (equipPtrs/currentModelPtr/animPtrs) kept for functions
// that address the region through the tail type.
struct CActParamHolderTail {
    u8 _00[0xFD0];      // 0x00..0xFD0
    u32 equipPtrs[8];   // 0xFD0 = CModelDispEquip+0xFE0
    u32 currentModelPtr; // 0xFF0 = CModelDispEquip+0x1000
    void* animPtrs[2];  // 0xFF4 = CModelDispEquip+0x1004
};

class CModelDispEquip {
public:
    CModelDispEquip(u32 somePtr, s32 equipSlot);
    ~CModelDispEquip();
    int OnFileEvent(CEventFile* event);

    void resetBase();
    u8 getState20();
    u8 getState21();
    void setState14();
    void setState14_3();
    static void storeFloats(float* dest, float a, float b, float c, float d);

    void ModelDispEquip_ReplaySlotAnims();
    void ModelDispEquip_HandleSlotCmd();

    void vfunc18();
    void vfunc40();

    void thunk4_1298();
    void thunk4_1440();
    void thunk4_131C();
    void thunk4_1318();
    void thunk4_F9C();
    void thunk4_dtor();
    void thunk8_1444();
    void thunk8_dtor();

    // ---- Layout ----
    // The retail has 3 vtable pointers (multiple inheritance), assigned
    // manually - the class has no C++ virtuals, so the dtor does not
    // auto-write a vtable. All 3 are plain words.
    u32 _vtable;  // 0x00 - first vtable pointer
    u32 _vtable2; // 0x04 - second vtable
    u32 _vtable3; // 0x08 - third vtable
    u32 somePtr;  // 0x0C
    CActParamHolder actParamHolder; // 0x10 (extends to 0x100C, covers the
                                    // equipPtrs/currentModelPtr/animPtrs tail)
    u32 weaponId;  // 0x100C
    s32 equipSlot; // 0x1010
    u8 state;      // 0x1014
    u8 _pad1015[3];
    f32 alpha;     // 0x1018
    f32 alphaTimer; // 0x101C
    u8 state20;    // 0x1020
    u8 state21;    // 0x1021
    u8 _pad1022[2];
    FileSlot fileSlots[9]; // 0x1024-0x108F
    CFileHandle* modelFileHandle; // 0x1090
    u8* modelData;  // 0x1094
    f32 colorR;     // 0x1098
    f32 colorG;     // 0x109C
    f32 colorB;     // 0x10A0
    f32 colorA;     // 0x10A4
    f32 scale1[3];  // 0x10A8
    f32 scale2[3];  // 0x10B4
};

// C-linkage imports (retail unmangled symbols).
extern "C" void* writeVec3f(void*, f32, f32, f32);
// Forward decl so the -4/-8 thunks can reference the dtor symbol.
extern "C" void* __dt__15CModelDispEquipFv(CModelDispEquip*, int);

// Data written by sinit_802019F8 (set-vector helper args); read back as raw
// 32-bit words by ModelDispEquip_SyncScalePose (lwzu/lwz word copies into scale1/scale2).
extern u32 lbl_eu_80576550[3];
extern u32 lbl_eu_8057655C[3];
// Vtable base written by the ctor into _vtable/_vtable2/_vtable3
// (offsets +0x0 / +0x88 / +0xB4 within the vtable block).
extern u32 lbl_eu_805354C8[];
// 3 state filter values selected by equipSlot (ModelDispEquip_BuildPartyModel / ModelDispEquip_SwapPartyEquip).
extern u32 lbl_eu_80507FD0[];
extern u32 lbl_eu_80507FDC[];
// Equip-name string table (accessed at +0x0/+0x4/+0x8/+0xC/+0x58/+0x62).
extern char lbl_eu_80507FF8[];
// Effect-singleton manager object (array forces @ha/@l at the call sites in
// OnFileEvent / ModelDispEquip_ResetDisplay).
extern u32 lbl_eu_8065FC18[];
extern const f32 lbl_eu_80668274;
extern const f32 lbl_eu_8066829C;
extern const f32 lbl_eu_80668278;
extern const f32 lbl_eu_80668270;
extern const f32 lbl_eu_8066827C; // alpha step for ModelDispEquip_StepFadeIn
// Step / clamp constants for ModelDispEquip_FadeAlphaUp/15D4/1740/17A4 and 1638/16BC.
extern const f32 lbl_eu_80668280;
extern const f32 lbl_eu_80668284; // scale1[1] upper clamp
extern const f32 lbl_eu_80668288; // scale2[1] upper clamp
extern const f32 lbl_eu_8066828C; // scale1[1] lower clamp
extern const f32 lbl_eu_80668290; // scale2[1] lower clamp
extern const f32 lbl_eu_80668294; // scale1[2] lower clamp
extern const f32 lbl_eu_80668298; // scale1[2] upper clamp
// 3-word anim-id table read by ModelDispEquip_TickAnim: the random sign (-1/0/+1)
// indexes the array, so the middle entry sits at the label (sdata2 -> @sda21).
// Declared as [2] (8 bytes) so MWCC keeps it small-data eligible and emits
// li r3, lbl@sda21 like retail (a 12-byte extern falls back to lis/addi).
extern const u32 lbl_eu_80662738[2];
// Imports used by this unit's functions. Retail C symbols: extern "C" keeps
// MWCC from C++-mangling the call relocs (plain decls emit tickAnimFrame__FP..).
extern "C" void tickAnimFrame(CActParamAnimView* self);
extern "C" bool setAnimSubPos(CActParamAnimView* self, const f32* value);
extern "C" CModelDispEffectView* func_804CC1F4(const u32* mgr, u8* bdat, u32 global, int r6, int r7, int r8);
extern "C" void func_804E3D0C(CModelDispEffectView* effect, CModelDispEffectView* parent);
extern "C" s32 getInstance__Q22ml6MTRandFv();
extern "C" u32 rand31__Q22ml6MTRandFv();
extern "C" int atoi(const char* str);
extern "C" bool setTurnScale(void*, f32);
extern "C" void* Scn_FindCamItem(void*, int);
extern "C" void func_8049EFF8(void*, f32, void*, void*);
extern "C" void* getAnimChain(void* self);
extern "C" void func_8004B9D4(void* self, void* arg, u32, s32, u32);
extern "C" void __destroy_arr(void*, void*, int, int);
// cf::CActParamAnimGame destructor address (defined in kyoshin/cf/CActParamAnimGame.cpp).
extern "C" void __dt__Q22cf17CActParamAnimGameFv(cf::CActParamAnimGame*, int);

// ---- Cross-TU imports (retail C/verbatim-mangled symbol names) ----
extern "C" int func_800BBC04(void* arg);
extern "C" void* getBdatEntryColumn__Q22cf13CfGameManagerFv(u32 type, int slot);
extern "C" int func_800AA33C(ml::FixStr<64>& buf, u32 packed, int prefixFlag, int suffixFlag);
extern "C" void PartyStateWin_InitMemCounters(CModelDispFileCtx* ctx);
extern "C" u32 PartyStateWin_PickAllocHandle(CModelDispFileCtx* ctx, u32 size);
extern "C" int getFileSize__11CDeviceFileFPCc(const char* path, int flags);
extern "C" void* readFile__11CDeviceFileFUlPCcP10IWorkEventii(u32 allocHandle, const char* path, void* workEvent, int, int);
extern "C" void setHandleFlag1__11CDeviceFileFP11CFileHandle(CFileHandle* fh);
extern "C" void closeFileHandle__FPP11CFileHandle(CFileHandle** handlePtr);
extern "C" void func_804CC1BC(void* mgr, void* data);
extern "C" void func_804CC1D8(void* arg, void* data); // (manager, buffer): the
// buffer rides in r4 from the null-check load - keeps the check color r4.
extern "C" void waitForDrawDone__9CDeviceVIFv();

// ---- Enum-list / actor imports (ModelDispEquip_TeardownHolder / 801FFDC4 / 80200FB0) ----
extern "C" CModelDispEnumList* CTaskGame_enumListCtor(CModelDispListHolder* holder);
extern "C" CModelDispEnumList* CTaskGame_enumListGet(CModelDispListHolder* holder);
extern "C" void __dt__80043E88(CModelDispListHolder* holder, int flag);
extern "C" void func_800F4A98(CModelDispEnumList* list, u32 type, u32 filter);
extern "C" CModelDispSlot* func_800F6EC0(CModelDispEnumList* list, u32 index);
extern "C" void func_804E3CCC(CModelDispEffectView* effect);
extern "C" void func_804E3D48(CModelDispEffectView* effect, CModelDispEffectView* parent);
extern "C" CModelDispNameParam* CfObjectMove_getBdatNameCol11(cf::CfObjectMove* move, int index);
extern "C" CModelDispNameParam* CfObjectMove_getBdatNameCol7(cf::CfObjectMove* move, int index);
extern "C" CModelDispNameParam* func_800BBC08(u8 value);
extern "C" u32 func_8014235C(u32 param1, const char* column, u32 param3);
extern "C" int sprintf(char* str, const char* fmt, ...);
extern "C" int* CtrlObjectParam_GetSlotTableBase();
extern "C" u8 BdatGetU8ByTableKey(const void*, const void*, u32);
extern "C" u32 func_80141E90(u32 param1, s16 param2, u32 param3, u32 param4);
extern "C" u32 func_80141FE0(int index);
extern "C" int CfRes_isGridLoadIdle();
extern "C" void* CfRes_tryResolveToken(u32 id, u32* outType);
extern "C" void syncFieldData__Q22cf13CfGameManagerFv(u32 value, bool searchEntries);
extern "C" void simSetLeafFlag4000(void* model, int flag);
extern "C" void func_80482DF4(void* model, int flag);
extern "C" CModelDispObj* func_800584B8(u32 global, u32 id, const char* name);
extern "C" CScnItemModel* Scn_SetupAnim(u32 global, u32 id, int a, int b);
extern "C" CScnItemModel* Scn_SetupAnimDefault(u32 global, CModelDispNameParam* param);
extern "C" CModelDispObj* Scn_InitGlobalA(u32 global, u8* mDataAdj, const char* name);
extern "C" void Scn_IsAnimActiveOrNull(CModelDispObj* obj);
extern "C" int CfObjectMove_getSubB0FieldA(cf::CfObjectMove* move); // int return: callers store
// into an s16 local, forcing the extsh into the home register at assignment.
extern "C" CModelDispParamSlot* CfRes_getPcGridEntry(s16 id, int a);
extern "C" CModelDispParamSlot* CfRes_getArrayElem18Idx(s16 id);
extern "C" void releaseAnimObj(CActParamAnimView* self, CModelDispObj* obj);
extern "C" void pushAnimNode(CActParamAnimView* self, CModelDispObj* obj, u32 param);
extern "C" void attachAnimObj(CActParamAnimView* self, CScnItemModel* model, CModelDispObj* anim, u32 param);
extern "C" CModelDispObj* getAnimModelId(CActParamAnimView* self);
extern "C" void func_8005A594(CActParamAnimView* self);
extern "C" void func_804831C4(CScnItemModel* model, CModelDispNameParam* param);
extern "C" void __ct__CMcaFile(CModelDispMca* self, u32 anim);
extern "C" void __construct_array(void*, void*, void*, int, int);
extern "C" void __ct__Q22cf17CActParamAnimGameFv(cf::CActParamAnimGame* self);
extern "C" cf::CfObjectMove* getPlayer__Q22cf13CfGameManagerFi(s32 index);
extern "C" u32 getQueuedFileEventCount__Q22cf13CfGameManagerFv();
// C++-mangled import: getCfObjectPc(cf::CfObjectMove*) => getCfObjectPc__FPQ22cf12CfObjectMove.
CModelDispActor* getCfObjectPc(cf::CfObjectMove* move);