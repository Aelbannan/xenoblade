#pragma once

#include <types.h>
#include "monolib/device/CFileHandle.hpp"

namespace cf { class CActParamAnimGame; }

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
    u32 field_0x374;  // +0x374 anim state (func_80200CE8 compares vs 0 / 0xb)
    u8 _378[0x13C];
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

// Sub-object at CModelDispEquip+0x10: 3 vtable words + act-param objects.
// The animModelPtrs array sits at the holder's tail (+0xFC8) - retail code
// addresses it through the holder base (e.g. func_80200E94's lwz 0xfc8(r31)).
struct CActParamHolder {
    void* field_0x00; // +0x00 object pointer (vtable dispatch at 0xC4/0xC8)
    u32 field_0x04;   // +0x04 second vtable
    u32 field_0x08;   // +0x08 third vtable
    CActParamAnimView actParam;    // +0x0C (0x53C bytes)
    s32 timer;        // +0x548
    u32 unk_55C;      // +0x54C
    CActParamAnimView actParams[2]; // +0x550 (0xA78 bytes)
    void* animModelPtrs[2]; // +0xFC8 animation model slot pointers (indexed by r5/i)
};

// Flat view of the memory past the embedded holder (0xFD0): the animPtrs
// array at holder+0xFF4 (== CModelDispEquip+0x1004). func_801FFADC reads the
// animation-model color slots through this so MWCC keeps the holder base and
// emits 0xff4(rX) like retail.
struct CActParamHolderTail {
    u8 _00[0xFF4];      // 0x00..0xFF4
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
    CActParamHolder actParamHolder; // 0x10
    u32 equipPtrs[8]; // 0xFE0
    u32 currentModelPtr; // 0x1000
    u32 animPtrs[2]; // 0x1004
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
extern "C" void func_8004B60C(void*, f32, f32, f32);
// Forward decl so the -4/-8 thunks can reference the dtor symbol.
extern "C" void* __dt__15CModelDispEquipFv(CModelDispEquip*, int);

// Data written by sinit_802019F8 (set-vector helper args); read back as raw
// 32-bit words by func_801FF7B0 (lwzu/lwz word copies into scale1/scale2).
extern u32 lbl_eu_80576550[3];
extern u32 lbl_eu_8057655C[3];
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
extern const u32 lbl_eu_80662738[3];
// Imports used by this unit's functions. Retail C symbols: extern "C" keeps
// MWCC from C++-mangling the call relocs (plain decls emit func_8004CF00__FP..).
extern "C" void func_8004CF00(CActParamAnimView* self);
extern "C" bool func_8004B354(CActParamAnimView* self, const f32* value);
extern "C" CModelDispEffectView* func_804CC1F4(const u32* mgr, u8* bdat, u32 global, int r6, int r7, int r8);
extern "C" void func_804E3D0C(CModelDispEffectView* effect, CModelDispEffectView* parent);
extern "C" s32 getInstance__Q22ml6MTRandFv();
extern "C" u32 rand31__Q22ml6MTRandFv();
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
extern "C" void func_804CC1D8(void* arg);
extern "C" void waitForDrawDone__9CDeviceVIFv();