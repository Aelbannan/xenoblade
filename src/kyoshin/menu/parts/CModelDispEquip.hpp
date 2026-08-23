#pragma once

#include <types.h>
#include "monolib/device/CFileHandle.hpp"

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

// 8-byte buffer-budget context built by func_801F981C / func_801F9894
// (retail: two remaining-size words at +0/+4, decremented by func_801F9894
// when it picks the MEM1/MEM2 alloc handle for the file size).
struct CModelDispFileCtx {
    u32 field_0x00; // +0 remaining MEM1 budget
    u32 field_0x04; // +4 remaining MEM2 budget
};

// Object pointed to by animPtrs[i] in func_801FFADC: four color words at +0x40.
struct CModelDispAnimColor {
    u8 _00[0x40];
    u32 field_0x40; // +0x40
    u32 field_0x44; // +0x44
    u32 field_0x48; // +0x48
    u32 field_0x4C; // +0x4C
};

// View of the 0x53C-byte act-param object. The retail cf::CActParamAnimGame is
// larger in the shared header; this TU only needs the flag word at +0x0C and
// the model-pointer slot at +0x4B4 (func_8004B9B8 result).
struct CActParamAnimView {
    u8 _00[0x0C];
    u32 field_0x0C;   // +0x0C busy flags (bit 0x20 toggled around func_8004B52C)
    u8 _10[0x364];
    s32 field_0x374;  // +0x374 anim state, signed so == compares emit cmpwi
    u32 field_0x378;  // +0x378 slot index (func_801FFDC4 stores i)
    u8 _37C[0x13C - 0x4];
    void* field_0x4B4; // +0x4B4 model/next-chain-obj pointer (func_8004B9B8 result)
    u8 _4B8[0x84];
};

// Effect-instance view written by func_8020131C: word at +0x14 (result of the
// vtable+0xA8 call) and a byte at +0x59 (active flag, set in the cmd==3 case).
struct CModelDispEffectView {
    u8 _00[0x14];
    u32 field_0x14; // +0x14
    u8 _18[0x41];
    u8 field_0x59;  // +0x59
};

// Command buffer passed to func_8020131C: byte +0xA selects the command
// (2 = load model slot, 3 = mark active); +0x1C holds an optional numeric
// string that is atoi'd when its first byte is a digit.
struct CModelDispEquipCmd {
    u8 _00[0x0A];
    u8 field_0x0A;  // +0x0A command
    u8 _0B[0x11];   // +0x0B..0x1B
    char field_0x1C; // +0x1C string start
};

// Fake SI interface for virtual dispatch at vtable offset +0x48 (index 18),
// taking one float arg (the act-param object's alpha/color setter). Cast-only
// usage: never constructed, so no vtable is emitted - MWCC emits the retail
// r12 dispatch (lwz r12,0(r3); lwz r12,0x48(r12); mtctr; bctrl).
// MWCC reserves 2 leading vtable slots, so 16 filler virtuals + m18 land the
// setter at slot 18 (0x48).
struct CModelDispVt48 {
    virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03();
    virtual void m04(); virtual void m05(); virtual void m06(); virtual void m07();
    virtual void m08(); virtual void m09(); virtual void m0A(); virtual void m0B();
    virtual void m0C(); virtual void m0D(); virtual void m0E(); virtual void m0F();
    virtual void m18(float value); // slot 18 => +0x48
};

// Fake SI interface for virtual dispatch at vtable offset +0xA8 (index 42),
// taking no args and returning a pointer (the anim model's chain-obj getter).
// 40 filler virtuals + m2A land the method at slot 42 (0xA8) with the 2
// reserved leading slots.
struct CModelDispVtA8 {
    virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03();
    virtual void m04(); virtual void m05(); virtual void m06(); virtual void m07();
    virtual void m08(); virtual void m09(); virtual void m0A(); virtual void m0B();
    virtual void m0C(); virtual void m0D(); virtual void m0E(); virtual void m0F();
    virtual void m10(); virtual void m11(); virtual void m12(); virtual void m13();
    virtual void m14(); virtual void m15(); virtual void m16(); virtual void m17();
    virtual void m18(); virtual void m19(); virtual void m1A(); virtual void m1B();
    virtual void m1C(); virtual void m1D(); virtual void m1E(); virtual void m1F();
    virtual void m20(); virtual void m21(); virtual void m22(); virtual void m23();
    virtual void m24(); virtual void m25(); virtual void m26(); virtual void m27();
    virtual void* m2A(); // slot 42 => +0xA8
};

// Opaque object returned by the Bdat/equip lookups in this TU.
struct CModelDispObj {
    u8 _00[4];
};

// Name/param record returned by func_800BEDC4 / func_800BED80 / func_800BBC08.
struct CModelDispNameParam {
    u8 _00[4];
};

// Character/equip record returned by func_8009EC9C (func_80200394): the five
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

// Vtable dispatch at +0x08 (slot 2) on the equipment param-slot objects
// (param[idx].field_2C): takes the owning slot and the weapon id.
struct CModelDispVt8 {
    virtual CModelDispNameParam* m00(CModelDispParamSlot* owner, u32 weaponId); // +0x8
};

// Per-equipment param slot (stride 0x3C); +0x2C holds a vtable-slot-0x8 obj.
struct CModelDispParamSlot {
    u8 _00[0x2C];
    CModelDispVt8* field_2C; // +0x2C
    u8 _30[0x3C - 0x30];
};

// Vtable dispatch on the built display model (holder->field_0x00):
// +0x48 (float), +0x64 (int), +0x9C (2 ints), +0xC4 (3 args), +0xC8 (1 arg).
// 2 reserved slots; target slot = declared index + 1.
// (m9C sits after 37 fillers incl. m24.)
struct CModelDispModelVt {
    virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03();
    virtual void m04(); virtual void m05(); virtual void m06(); virtual void m07();
    virtual void m08(); virtual void m09(); virtual void m0A(); virtual void m0B();
    virtual void m0C(); virtual void m0D(); virtual void m0E(); virtual void m0F();
    virtual void m48(f32 value);        // #17 => +0x48
    virtual void m10(); virtual void m11(); virtual void m12(); virtual void m13();
    virtual void m14(); virtual void m15(); virtual void m16();
    virtual void m64(u32 arg);          // #24 => +0x64
    virtual void m17(); virtual void m18(); virtual void m19(); virtual void m1A();
    virtual void m1B(); virtual void m1C(); virtual void m1D(); virtual void m1E();
    virtual void m1F(); virtual void m20(); virtual void m21(); virtual void m22();
    virtual void m23();
    virtual void m9C(u32 a, u32 b);     // #38 => +0x9C
    virtual void m25(); virtual void m26(); virtual void m27();
    virtual void m28(); virtual void m29(); virtual void m2A(); virtual void m2B();
    virtual void m2C();
    virtual void mC4(CModelDispModelVt* animModel, CModelDispNameParam* name, u32 flag); // #48 => +0xC4
    virtual void mC8(CModelDispModelVt* animModel); // #49 => +0xC8
};

// Vtable dispatch on the actor's embedded CfObjectMove (actor+0x3E9C):
// +0x74 (no args), +0x148 (int arg), +0x184 / +0x188 (no args).
struct CModelDispMoveVt {
    virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03();
    virtual void m04(); virtual void m05(); virtual void m06(); virtual void m07();
    virtual void m08(); virtual void m09(); virtual void m0A(); virtual void m0B();
    virtual void m0C(); virtual void m0D(); virtual void m0E(); virtual void m0F();
    virtual void m10(); virtual void m11(); virtual void m12(); virtual void m13();
    virtual void m14(); virtual void m15(); virtual void m16(); virtual void m17();
    virtual void m18(); virtual void m19(); virtual void m1A();
    virtual int m74();                  // #28 => +0x74
    virtual void m1B(); virtual void m1C(); virtual void m1D(); virtual void m1E();
    virtual void m1F(); virtual void m20(); virtual void m21(); virtual void m22();
    virtual void m23(); virtual void m24(); virtual void m25(); virtual void m26();
    virtual void m27(); virtual void m28(); virtual void m29(); virtual void m2A();
    virtual void m2B(); virtual void m2C(); virtual void m2D(); virtual void m2E();
    virtual void m2F(); virtual void m30(); virtual void m31(); virtual void m32();
    virtual void m33(); virtual void m34(); virtual void m35(); virtual void m36();
    virtual void m37(); virtual void m38(); virtual void m39(); virtual void m3A();
    virtual void m3B(); virtual void m3C(); virtual void m3D(); virtual void m3E();
    virtual void m3F(); virtual void m40(); virtual void m41(); virtual void m42();
    virtual void m43(); virtual void m44(); virtual void m45(); virtual void m46();
    virtual void m47(); virtual void m48(); virtual void m49(); virtual void m4A();
    virtual void m4B(); virtual void m4C(); virtual void m4D(); virtual void m4E();
    virtual int m148(u32 arg);          // #81 => +0x148
    virtual void m4F(); virtual void m50(); virtual void m51(); virtual void m52();
    virtual void m53(); virtual void m54(); virtual void m55(); virtual void m56();
    virtual void m57(); virtual void m58(); virtual void m59(); virtual void m5A();
    virtual void m5B(); virtual void m5C();
    virtual int m184();                 // #96 => +0x184
    virtual int m188();                 // #97 => +0x188
};

// Vtable dispatch at +0xE0 (slot 0x38) on the act-param objects
// (holder+0x0C / +0x550): the flush method. 54 fillers + m38 land it.
struct CModelDispVtE0 {
    virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03();
    virtual void m04(); virtual void m05(); virtual void m06(); virtual void m07();
    virtual void m08(); virtual void m09(); virtual void m0A(); virtual void m0B();
    virtual void m0C(); virtual void m0D(); virtual void m0E(); virtual void m0F();
    virtual void m10(); virtual void m11(); virtual void m12(); virtual void m13();
    virtual void m14(); virtual void m15(); virtual void m16(); virtual void m17();
    virtual void m18(); virtual void m19(); virtual void m1A(); virtual void m1B();
    virtual void m1C(); virtual void m1D(); virtual void m1E(); virtual void m1F();
    virtual void m20(); virtual void m21(); virtual void m22(); virtual void m23();
    virtual void m24();
    virtual void m25(); virtual void m26(); virtual void m27();
    virtual void m28(); virtual void m29(); virtual void m2A(); virtual void m2B();
    virtual void m2C(); virtual void m2D(); virtual void m2E(); virtual void m2F();
    virtual void m30(); virtual void m31(); virtual void m32(); virtual void m33();
    virtual void m34(); virtual void m35();
    virtual void m38();                 // 55th declared => +0xE0
};

// Vtable dispatch at +0x28 (slot 10) on actor->field_3F34.
struct CModelDispVt28 {
    virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03();
    virtual void m04(); virtual void m05(); virtual void m06(); virtual void m07();
    virtual void m28(const char* str, u32 flag); // #9 => +0x28
};

// View of the CMcaFile constructed at func_801FFDC4's stack frame.
struct CModelDispMca {
    u32 field_00;  // +0x00 vtable
    u8 field_04;   // +0x04 flag
    u8 _05[3];
    u8* field_08;  // +0x08
    u8* mDataAdj;  // +0x0C (passed to func_80495EAC)
};

// Parent object whose +0x3A0 field must equal the holder's model pointer
// (func_80200FB0's early guard).
struct CModelDispParent {
    u8 _00[0x3A0];
    CModelDispObj* field_0x3A0; // +0x3A0
};

// Actor container (func_800BFC68 result): CfObjectMove embedded at +0x3E9C.
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

// 8-byte enum-list holder (func_80043D90 / __dt__80043E88).
struct CModelDispListHolder {
    u32 field_00;
    u32 field_04;
};

// Enum list returned by func_80043F18: element count at +0x620.
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
// addresses it through the holder base (e.g. func_80200E94's lwz 0xfc8(r31)).
struct CActParamHolder {
    void* field_0x00; // +0x00 object pointer (vtable dispatch at 0xC4/0xC8)
    CModelDispObj* field_0x04; // +0x04 anim object (func_800584B8 result)
    CModelDispObj* field_0x08; // +0x08 anim object (func_800584B8 result)
    CActParamAnimView actParam;    // +0x0C (0x53C bytes)
    s32 timer;        // +0x548
    CModelDispObj* unk_55C; // +0x54C loaded-model record (func_80495EAC result)
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
    CModelDispEquip();
    ~CModelDispEquip();
    int OnFileEvent(CEventFile* event);

    void resetBase();
    u8 getState20();
    u8 getState21();
    void setState14();
    void setState14_3();
    static void storeFloats(float* dest, float a, float b, float c, float d);

    void func_80201298();
    void func_8020131C();

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
extern "C" void* func_8004B60C(void*, f32, f32, f32);
// Forward decl so the -4/-8 thunks can reference the dtor symbol.
extern "C" void* __dt__15CModelDispEquipFv(CModelDispEquip*, int);

// Data written by sinit_802019F8 (set-vector helper args); read back as raw
// 32-bit words by func_801FF7B0 (lwzu/lwz word copies into scale1/scale2).
extern u32 lbl_eu_80576550[3];
extern u32 lbl_eu_8057655C[3];
// Vtable base written by the ctor into _vtable/_vtable2/_vtable3
// (offsets +0x0 / +0x88 / +0xB4 within the vtable block).
extern u32 lbl_eu_805354C8[];
// 3 state filter values selected by equipSlot (func_801FFDC4 / func_80200FB0).
extern u32 lbl_eu_80507FD0[];
extern u32 lbl_eu_80507FDC[];
// Equip-name string table (accessed at +0x0/+0x4/+0x8/+0xC/+0x58/+0x62).
extern char lbl_eu_80507FF8[];
// Effect-singleton manager object (array forces @ha/@l at the call sites in
// OnFileEvent / func_801FF874).
extern u32 lbl_eu_8065FC18[];
extern const f32 lbl_eu_80668274;
extern const f32 lbl_eu_8066829C;
extern const f32 lbl_eu_80668278;
extern const f32 lbl_eu_80668270;
extern const f32 lbl_eu_8066827C; // alpha step for func_801FFADC
// Step / clamp constants for func_80201570/15D4/1740/17A4 and 1638/16BC.
extern const f32 lbl_eu_80668280;
extern const f32 lbl_eu_80668284; // scale1[1] upper clamp
extern const f32 lbl_eu_80668288; // scale2[1] upper clamp
extern const f32 lbl_eu_8066828C; // scale1[1] lower clamp
extern const f32 lbl_eu_80668290; // scale2[1] lower clamp
extern const f32 lbl_eu_80668294; // scale1[2] lower clamp
extern const f32 lbl_eu_80668298; // scale1[2] upper clamp
// 3-word anim-id table read by func_80200CE8: the random sign (-1/0/+1)
// indexes the array, so the middle entry sits at the label (sdata2 -> @sda21).
// Declared as [2] (8 bytes) so MWCC keeps it small-data eligible and emits
// li r3, lbl@sda21 like retail (a 12-byte extern falls back to lis/addi).
extern const u32 lbl_eu_80662738[2];
// Imports used by this unit's functions. Retail C symbols: extern "C" keeps
// MWCC from C++-mangling the call relocs (plain decls emit func_8004CF00__FP..).
extern "C" void func_8004CF00(CActParamAnimView* self);
extern "C" bool func_8004B354(CActParamAnimView* self, const f32* value);
extern "C" CModelDispEffectView* func_804CC1F4(const u32* mgr, u8* bdat, u32 global, int r6, int r7, int r8);
extern "C" void func_804E3D0C(CModelDispEffectView* effect, CModelDispEffectView* parent);
extern "C" s32 getInstance__Q22ml6MTRandFv();
extern "C" u32 rand31__Q22ml6MTRandFv();
extern "C" int atoi(const char* str);
extern "C" bool func_8004B52C(void*, f32);
extern "C" void* func_80496264(void*, int);
extern "C" void func_8049EFF8(void*, f32, void*, void*);
extern "C" void* func_8004B9B8(void* self);
extern "C" void func_8004B9D4(void* self, void* arg, u32, s32, u32);
extern "C" void __destroy_arr(void*, void*, int, int);
// cf::CActParamAnimGame destructor address (defined in kyoshin/cf/CActParamAnimGame.cpp).
extern "C" void __dt__Q22cf17CActParamAnimGameFv(cf::CActParamAnimGame*, int);

// ---- Cross-TU imports (retail C/verbatim-mangled symbol names) ----
extern "C" int func_800BBC04(void* arg);
extern "C" void* func_8007DE94__Q22cf13CfGameManagerFv(u32 type, int slot);
extern "C" int func_800AA33C(ml::FixStr<64>& buf, u32 packed, int prefixFlag, int suffixFlag);
extern "C" void func_801F981C(CModelDispFileCtx* ctx);
extern "C" u32 func_801F9894(CModelDispFileCtx* ctx, u32 size);
extern "C" int getFileSize__11CDeviceFileFPCc(const char* path, int flags);
extern "C" void* readFile__11CDeviceFileFUlPCcP10IWorkEventii(u32 allocHandle, const char* path, void* workEvent, int, int);
extern "C" void setHandleFlag1__11CDeviceFileFP11CFileHandle(CFileHandle* fh);
extern "C" void func_801390E0__FPP11CFileHandle(CFileHandle** handlePtr);
extern "C" void func_804CC1BC(void* arg);
extern "C" void func_804CC1D8(void* arg, void* data); // (manager, buffer): the
// buffer rides in r4 from the null-check load - keeps the check color r4.
extern "C" void waitForDrawDone__9CDeviceVIFv();

// ---- Enum-list / actor imports (func_801FFBC4 / 801FFDC4 / 80200FB0) ----
extern "C" CModelDispEnumList* func_80043D90(CModelDispListHolder* holder);
extern "C" CModelDispEnumList* func_80043F18(CModelDispListHolder* holder);
extern "C" void __dt__80043E88(CModelDispListHolder* holder, int flag);
extern "C" void func_800F4A98(CModelDispEnumList* list, u32 type, u32 filter);
extern "C" CModelDispSlot* func_800F6EC0(CModelDispEnumList* list, u32 index);
extern "C" void func_804E3CCC(CModelDispEffectView* effect);
extern "C" void func_804E3D48(CModelDispEffectView* effect, CModelDispEffectView* parent);
extern "C" CModelDispNameParam* func_800BEDC4(CModelDispMoveVt* move, int index);
extern "C" CModelDispNameParam* func_800BED80(CModelDispMoveVt* move, int index);
extern "C" CModelDispNameParam* func_800BBC08(u8 value);
extern "C" u32 func_8014235C(u32 param1, const char* column, u32 param3);
extern "C" int sprintf(char* str, const char* fmt, ...);
extern "C" int* func_8009ECB0();
extern "C" u8 func_8013600C(const char*, const char*, u32);
extern "C" u32 func_80141E90(u32 param1, s16 param2, u32 param3, u32 param4);
extern "C" u32 func_80141FE0(int index);
extern "C" int func_80062A00();
extern "C" void* func_80062AD8(u32 id, u32* outType);
extern "C" void func_8007E038__Q22cf13CfGameManagerFv(u32 value, bool searchEntries);
extern "C" void func_80485684(void* model, int flag);
extern "C" void func_80482DF4(void* model, int flag);
extern "C" CModelDispObj* func_800584B8(u32 global, u32 id, const char* name);
extern "C" CModelDispModelVt* func_80495E8C(u32 global, u32 id, int a, int b);
extern "C" CModelDispModelVt* func_80495E94(u32 global, CModelDispNameParam* param);
extern "C" CModelDispObj* func_80495EAC(u32 global, u8* mDataAdj, const char* name);
extern "C" void func_80495E60(CModelDispObj* obj);
extern "C" s16 func_800BE954(CModelDispMoveVt* move);
extern "C" CModelDispParamSlot* func_80062C28(s16 id, int a);
extern "C" CModelDispParamSlot* func_80062DA4(s16 id);
extern "C" void func_8004B6BC(CActParamAnimView* self, CModelDispObj* obj);
extern "C" void func_8004B6A4(CActParamAnimView* self, CModelDispObj* obj, u32 param);
extern "C" void func_8004B624(CActParamAnimView* self, CModelDispModelVt* model, CModelDispObj* anim, u32 param);
extern "C" CModelDispObj* func_8004C5EC(CActParamAnimView* self);
extern "C" void func_8005A594(CActParamAnimView* self);
extern "C" void func_804831C4(CModelDispModelVt* model, CModelDispNameParam* param);
extern "C" void __ct__CMcaFile(CModelDispMca* self, u32 anim);
extern "C" void __construct_array(void*, void*, void*, int, int);
extern "C" void __ct__Q22cf17CActParamAnimGameFv(cf::CActParamAnimGame* self);
extern "C" cf::CfObjectMove* getPlayer__Q22cf13CfGameManagerFi(s32 index);
extern "C" u32 func_800822F4__Q22cf13CfGameManagerFv();
// C++-mangled import: func_800BFC68(cf::CfObjectMove*) => func_800BFC68__FPQ22cf12CfObjectMove.
CModelDispActor* func_800BFC68(cf::CfObjectMove* move);