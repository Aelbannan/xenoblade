#include "kyoshin/cf/CfCam.hpp"
#include "kyoshin/cf/CfMapItemManager.hpp"
#include "monolib/scn/CScnTimeApi.hpp"

#include "kyoshin/cf/CfGameManagerData.hpp"
#include "kyoshin/cf/object/CfObject.hpp"
#include "kyoshin/code_80296898.hpp"
#include <nw4r/math.h>
#include <math.h>
#include <string.h>
#include "monolib/math.hpp"
#include "kyoshin/cf/CfCam_ps.inl"
#include "monolib/math/FloatUtils.hpp"
// Scn_QueryUnk80State: retail branches here WITHOUT setting up r3 (stale-register
// call), so this TU keeps a private no-arg declaration and must not include
// libs/monolib/src/scn/CScn_8049603C.hpp (owner: monolib CScn).
extern "C" void* Scn_QueryUnk80State();
// TEMP unblock: monolib/core/CPadManager.hpp currently fails under MWCC
// (C++11 static_assert / illegal offsetof constant expressions) and
// CDeviceRemotePad.hpp pulls it in. This TU only needs CDeviceRemotePad::
// getMainGCPad(), so declare that locally; restore the real includes once the
// header compiles again.
class CPad {
public:
    u8 field_0x0[0xE8];
    u32 mPadType; // 0xE8
};
enum PadType { PAD_TYPE_NONE, PAD_TYPE_GC, PAD_TYPE_CORE, PAD_TYPE_FS,
               PAD_TYPE_CLASSIC, PAD_TYPE_5, PAD_TYPE_6, PAD_TYPE_7 };
class CDeviceRemotePad {
public:
    static CPad* getMainGCPad();
};
// MWCC 10140 rejects CfGameManager.hpp's qualified cf::CfObjectMove uses
// inside class CfGameManager even though the fwd decl sits in the same
// namespace. Rename the type for the duration of the include so the lookup
// hits a name this TU pre-declares; pointer params/members keep their layout,
// and the only affected mangled symbol (member getPlayerSlotPtr) is never called
// here.
#define CfObjectMove cfCamGameManagerObjMoveFwd
namespace cf {
class cfCamGameManagerObjMoveFwd;
}
#include "kyoshin/cf/CfGameManager.hpp"
#undef CfObjectMove
#include <revolution/mtx/quat.h>
#include <revolution/mtx/mtxvec.h>
// Runtime RTTI helper + typeinfo pair for cfCam_copyCamState's dynamic_cast
// (extern "C" keeps the retail unmangled __dynamic_cast reloc; fixed-size
// 8-byte typeinfo decls keep them sdata-eligible -> li @sda21).
extern "C" void* __dynamic_cast(void* obj, long offset, const void* src_type,
                                const void* dst_type, void* src2dst);
extern const void* lbl_eu_80661B28[2];
extern const void* lbl_eu_80661B30[2];
// C++-linkage global; MWCC mangles it to isGlobalCamFlagSet__Fi (the retail name).
int isGlobalCamFlagSet(int gate);
// Cross-TU imports (flat retail names).
// ScnRes_VertRayForward_E398's retail ABI passes two FP args after the four GPR args
// (same shape as CtrlMoveBase.hpp / CfCamEvent.hpp).
extern "C" int ScnRes_VertRayForward_E398(void* vec, u32 a, u32 b, u32 c, f32 d, f32 e);
extern "C" int cfCam_isVecNear0(void* pos);
// C++ free function; MWCC mangles it to the retail getFP__FPCc.
void* getFP(const char* name);
// In-TU definitions further down this file.
int func_80071D50(ml::CVec3* out, u32 probeArg, float f);
int cfCam_maintainFlg(cf::CfCamFollow* self, float val);
void func_8006BFDC(cf::CfCamFollow* self);
void cfCam_updateFrame(cf::CfCamFollow* self);
void func_8006EFA0(cf::CfCamFollow* self, float argF, ml::CVec3* outPos,
                   ml::CVec3* target, int flag);
int func_8006F5C8(cf::CfCamFollow* self, void* arg, float* out1, float* out2);
void cfCam_syncFollowSnapshot(cf::CfCamFollow* self, void* arg, void* srcPos);
void func_8006FD3C(cf::CfCamFollow* self, const nw4r::math::VEC3* arg, float input);
void func_80070088(cf::CfCamFollow* self, const ml::CVec3* dir, float input);
void cfCam_rampFollowY(cf::CfCamFollow* self, int flag);
void func_80071754(cf::CfCamFollow* self, ml::CVec3* arg2);
void cfCam_syncSelFlags(cf::CfCamFollow* self);
void func_8006D8D0(cf::CfCamFollow* self, void* arg);
void func_8006DD58(cf::CfCamFollow* self, void* arg);
void cfCam_copyBlock48(CfCamWordBlock48* dst, const CfCamWordBlock48* src);
void func_80072194(cf::CfCamFollow* self, void* arg);
// extern "C": retail symbol is flat func_80074090 (CfCamEvent.hpp declares
// it with C linkage; keep this TU's decl/def consistent so call relocs and
// the definition use the unmangled name).
extern "C" void func_80074090(ml::CVec3* out, cf::CfObject* src, ml::CVec3* pos, int sel);
void cfCam_applyBasis(nw4r::math::VEC3* a, nw4r::math::VEC3* b, nw4r::math::VEC3* c, float f);
// In-TU definitions further down this file.
__declspec(noinline) void cfCam_posFromLookat(ml::CVec3* out, ml::CVec3* lookat, const ml::CVec3* dir, f32 speed);
extern "C" __declspec(noinline) void func_80074534(ml::CMat33* out, const ml::CVec3* dir);

// cf::CfCam base ctor, C linkage so the retail symbol name is emitted verbatim.
// noinline keeps the `bl __ct__cf_CfCam` in the follow ctor (body is in-TU).
extern "C" __declspec(noinline) void* __ct__cf_CfCam(void* self, void* arg);
// In-TU CVec3::zero copy; noinline keeps the retail `bl cfCam_zeroVec3` sites.
extern "C" __declspec(noinline) void cfCam_zeroVec3(void* self);
extern "C" {
void noopAnimVec3(void* self);
void* writeVec3f(void* self, f32 x, f32 y, f32 z);
void* memset(void* dest, int val, u32 count);

char lbl_eu_80527260[];
char lbl_eu_805272E8[];
/* (80663DEC moved to the retail-ordered .sbss block below) */

/* Step-1 nobits for copy_data_sections removal (cannot affect codegen:
 * zero-init nobits emit no pool/values, only symbols).
 * .bss vec3s owned by this split (hpp extern-declares them). */
float lbl_eu_80570A38[3];
float lbl_eu_80570A44[3];
float lbl_eu_80570A50[3];
float lbl_eu_80570A5C[6];
/* .sbss owned words missing from this TU's emission. */
/* (moved to the retail-ordered .sbss block below) */

/* (sdata/sdata2 labels now come from the typed pool structs below via
 * macros; the old extern decls are removed so no dormant symbols linger.) */

// NOTE: this declaration went missing from the include closure (concurrent
// edit); restored TU-locally so the TU keeps compiling. Definition is at the
// bottom of this file (_declspec(noinline) bool cfCam_isClassicPad).
extern "C" bool cfCam_isClassicPad(void* self, f32 argF);
}

/* Step-1 nobits, C++ linkage (match the hpp/cpp decls exactly). */
/* .bss fallback buffer: hpp declares incomplete u8[]; complete to retail. */
u8 lbl_eu_80570A8C[0x204];
/* Retail-ordered .sbss true-zero block (retail 0x80663DC8-0x80663DF8).
 * Declaration order sets emission order; explicit = 0 forces emission.
 * Adjacent u8 DE0/DE1 are declared together to pack like retail. */
float lbl_eu_80663DC8 = 0;
float lbl_eu_80663DCC = 0;
float lbl_eu_80663DD0 = 0;
float lbl_eu_80663DD4 = 0;
float lbl_eu_80663DD8 = 0;
float lbl_eu_80663DDC = 0;
extern "C" u8 lbl_eu_80663DE0 = 0;
u8 lbl_eu_80663DE1 = 0;
extern "C" float lbl_eu_80663DE4 = 0;
extern "C" float lbl_eu_80663DE8 = 0;
extern "C" cf::CfCamFollow* lbl_eu_80663DEC = 0;
int lbl_eu_80663DF0 = 0;
int cfcam_sbss_pad_DF4 = 0;

// ----- typified retail data (replaces copy_data_sections) -----
// .rodata RTTI class names (exact sizes; 804FB4E0's 6-byte pad gap to
// 804FB4F0 is filled by MWCC's alignment of the blob array).
__declspec(section ".rodata") __attribute__((used, aligned(8)))
char lbl_eu_804FB4D0[0x10] = "cf::CfCamFollow";
__declspec(section ".rodata") __attribute__((used, aligned(8)))
char lbl_eu_804FB4E0[0x0A] = "cf::CfCam";
// .rodata camera message-format pool. Offsets match every
// lbl_eu_804FB4F0+N site in code; tail zero-pads to 0xE0.
__declspec(section ".rodata") __attribute__((used, aligned(8)))
char lbl_eu_804FB4F0[0xE0] =
    "opt_camlist\0"
    "at\0"
    "pitch\0"
    "CROSS WATER\0"
    "WATER AT\0"
    "water high wa:%.3f map:%.3f %.2f %.2f\n\0"
    "blend:%.2f l:%.2f\0"
    "water:%.2f y:%.3f\0"
    "blend:%.2f %.1f\0"
    "cliff adj:%d\0"
    "water\0"
    "[%.2f %.2f %.2f] [%.2f %.2f %.2f] l:%.3f %.3f\0"
    "WALL [%.2f %.2f %.2f]\0";
// .sdata typeinfo pairs (Select precedent). B28/B30 point at this TU's
// rodata names above; the dispatch halves ship from other TUs.
extern char lbl_eu_80527154[];
__declspec(section ".sdata") __attribute__((used, aligned(8)))
const void* lbl_eu_80661B28[2] = { lbl_eu_804FB4D0, lbl_eu_80527154 };
__declspec(section ".sdata") __attribute__((used, aligned(8)))
const void* lbl_eu_80661B30[2] = { lbl_eu_804FB4E0, 0 };
// .sdata scalar pool as a struct (Eve/Item recipe): individual scalars with
// duplicated values would merge, and the code writes several members, so the
// struct is volatile (loads/stores stay real pool accesses, Item-proven).
// Only this TU uses these names (verified), so retail names are macros.
// .sdata scalar pool as MUTABLE individuals (NOT a struct: struct-member
// accesses compile to base+offset (extra address insn per access), while
// direct globals use single SDA loads/stores like the retail shape.
// Mutable (never const): same-valued objects must keep distinct addresses
// (no merging) and visible values must not fold code loads. Only this TU
// uses these names (verified), and sdata holds no zeros (no exile risk).
__declspec(section ".sdata") __attribute__((used, aligned(4)))
float lbl_eu_80661B38 = 1.25f;
__declspec(section ".sdata") __attribute__((used, aligned(4)))
float lbl_eu_80661B3C = 0.07f;
__declspec(section ".sdata") __attribute__((used, aligned(4)))
float lbl_eu_80661B40 = 0.3f;
__declspec(section ".sdata") __attribute__((used, aligned(4)))
float lbl_eu_80661B44 = 0.3f;
__declspec(section ".sdata") __attribute__((used, aligned(4)))
float lbl_eu_80661B48 = 0.65f;
__declspec(section ".sdata") __attribute__((used, aligned(4)))
float lbl_eu_80661B4C = 7.5f;
__declspec(section ".sdata") __attribute__((used, aligned(4)))
float lbl_eu_80661B50 = 40.0f;
__declspec(section ".sdata") __attribute__((used, aligned(4)))
float lbl_eu_80661B54 = 0.5f;
__declspec(section ".sdata") __attribute__((used, aligned(4)))
float lbl_eu_80661B58 = 0.5f;
__declspec(section ".sdata") __attribute__((used, aligned(4)))
float lbl_eu_80661B5C = 1.0f;
__declspec(section ".sdata") __attribute__((used, aligned(4)))
float lbl_eu_80661B60 = 8.0f;
__declspec(section ".sdata") __attribute__((used, aligned(4)))
float lbl_eu_80661B64 = 2.0f;
__declspec(section ".sdata") __attribute__((used, aligned(4)))
float lbl_eu_80661B68 = 20.0f;
__declspec(section ".sdata") __attribute__((used, aligned(4)))
float lbl_eu_80661B6C = 20.0f;
__declspec(section ".sdata") __attribute__((used, aligned(4)))
float lbl_eu_80661B70 = 0.25f;
__declspec(section ".sdata") __attribute__((used, aligned(4)))
float lbl_eu_80661B74 = 0.07f;
__declspec(section ".sdata") __attribute__((used, aligned(4)))
float lbl_eu_80661B78 = 24.0f;
__declspec(section ".sdata") __attribute__((used, aligned(4)))
u32 lbl_eu_80661B7C = 4;
__declspec(section ".sdata") __attribute__((used, aligned(4)))
float lbl_eu_80661B80 = 0.3f;
__declspec(section ".sdata") __attribute__((used, aligned(4)))
float lbl_eu_80661B84 = 0.25f;
__declspec(section ".sdata") __attribute__((used, aligned(4)))
float lbl_eu_80661B88 = 0.5f;
__declspec(section ".sdata") __attribute__((used, aligned(4)))
float lbl_eu_80661B8C = 12.0f;
__declspec(section ".sdata") __attribute__((used, aligned(4)))
float lbl_eu_80661B90 = 18.0f;
__declspec(section ".sdata") __attribute__((used, aligned(4)))
float lbl_eu_80661B94 = 30.0f;
__declspec(section ".sdata") __attribute__((used, aligned(4)))
float lbl_eu_80661B98 = 20.0f;
__declspec(section ".sdata") __attribute__((used, aligned(4)))
float lbl_eu_80661B9C = 30.0f;
__declspec(section ".sdata") __attribute__((used, aligned(4)))
float lbl_eu_80661BA0 = 45.0f;
__declspec(section ".sdata") __attribute__((used, aligned(4)))
u32 lbl_eu_80661BA4 = 1;
__declspec(section ".sdata") __attribute__((used, aligned(4)))
float lbl_eu_80661BA8 = 0.13f;
__declspec(section ".sdata") __attribute__((used, aligned(4)))
float lbl_eu_80661BAC = 30.0f;
__declspec(section ".sdata") __attribute__((used, aligned(4)))
u8 lbl_eu_80661BB0 = 1;
__declspec(section ".sdata") __attribute__((used, aligned(4)))
float lbl_eu_80661BB4 = 0.4f;
// .sdata2 storage (const cam_-named; unread by code so values never
// fold; code uses the added lbl_ symbols via extern decls, compiling to
// single SDA loads with CSE like retail). Zero words use const-float
// zeros; magic words use const-u32 bits. Gaps hold cam_374_5 (u32[5])
// and the joint-strings struct (kept as-is below).
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_000 = 0.01f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_004 = 0.1f;
struct Sdata2_CfCamMagic { u32 pad; u32 hi; u32 lo; };
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
Sdata2_CfCamMagic sdata2_CfCamMagic = {0, 0x43300000, 0x80000000};
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_014 = 0.045f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_018 = 0.65f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_01C = 8.0f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_020 = 20.0f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_024 = 30.0f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_028 = 12.0f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_02C = 18.0f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_030 = 1.25f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_034 = 1.0f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_038 = -40.0f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_03C = 0.75f;
struct Sdata2_DCE0 { float dc; float e0; };
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
Sdata2_DCE0 sdata2_DCE0 = {0.0f, 0.25f};
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_048 = 40.743664f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
u32 cam_s2_04C = 0x3f847ae1;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
u32 cam_s2_050 = 0x40000000;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_054 = 0.4f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_058 = 0.07f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_05C = 0.3f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_060 = 7.5f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_064 = 40.0f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_068 = 0.5f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_06C = 2.0f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_070 = 24.0f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_074 = 45.0f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_078 = 0.13f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_07C = 0.8f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_080 = 0.52f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_084 = 6.0f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_088 = 90.0f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_08C = 4.0f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_090 = 0.024543693f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_094 = 10.0f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_098 = 5.0f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_09C = -0.6f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_0A0 = 0.0001f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_0A4 = -0.17f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_0A8 = 0.001f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_0AC = 0.95f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_0B0 = 0.2f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_0B4 = 0.6f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_0B8 = 0.7f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_0BC = -1.0f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_0C0 = 65.0f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_0C4 = 1.3f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_0C8 = 0.15f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_0CC = 0.007f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_0D0 = 0.05f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_0D4 = 9.0f;
// f374 + f378 + pad word + dead double-zero as exact u32 words (float
// arrays pad starts/ends to 8; u32 arrays pack; the nonzero f374/f378 words
// keep the object in .sdata2; only f374 is code-read, via plain deref).
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
u32 cam_374_5[5] = {0x3727C5AC, 0x41800000, 0x00000000, 0x00000000, 0x00000000};
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_0EC = 0.0625f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_0F0 = 0.9f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_0F4 = 15.0f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_0F8 = 0.09f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_0FC = 0.06f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_100 = 1.1f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_104 = -5.0f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_108 = 0.96f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_10C = 1.8f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_110 = 7.0f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_114 = 3.0f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_118 = 35.0f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_11C = -0.3f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_120 = 0.005f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_124 = 1.2f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_128 = -2.0f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_12C = 0.02f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_130 = -0.9f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_134 = -0.999f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_138 = 0.999f;
// Joint-name strings + zero word as one plain struct (address-taken only;
// never value-read).
struct Sdata2_CfCamStr { u32 m3D8; char sDC[8]; char sE4[8]; char sEC[8]; char sF4[8]; char sFC[8]; char s404[8]; };
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
Sdata2_CfCamStr sdata2_CfCamStr = {0, "JUhead", "JUhd_L", "JUhd_R", "JLhip", "JLft_L", "JLft_R"};
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_170 = -4.5f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_174 = 70.0f;
__declspec(section ".sdata2") __attribute__((used, aligned(4)))
float cam_s2_178 = -80.0f;

// .data float lookup tables as individual scalars (NOT float[5] arrays:
// MWCC pads each 20-byte array to 24, breaking the pack). Code indexes
// them dynamically; array-view macros preserve the indexing (and the hpp
// array decls stay dormant). Only this TU uses these names (verified).
// Scalars are small-data-eligible, so each is pinned to .data explicitly.
__declspec(section ".data") __attribute__((used, aligned(4))) float cam_t208_0 = 0.1f;
__declspec(section ".data") __attribute__((used, aligned(4))) float cam_t208_1 = 0.08f;
__declspec(section ".data") __attribute__((used, aligned(4))) float cam_t208_2 = 0.06f;
__declspec(section ".data") __attribute__((used, aligned(4))) float cam_t208_3 = 0.045f;
__declspec(section ".data") __attribute__((used, aligned(4))) float cam_t208_4 = 0.03f;
#define lbl_eu_80527208 (*((float(*)[5])&cam_t208_0))
__declspec(section ".data") __attribute__((used, aligned(4))) float cam_t21C_0 = 0.07f;
__declspec(section ".data") __attribute__((used, aligned(4))) float cam_t21C_1 = 0.05f;
__declspec(section ".data") __attribute__((used, aligned(4))) float cam_t21C_2 = 0.03f;
__declspec(section ".data") __attribute__((used, aligned(4))) float cam_t21C_3 = 0.02f;
__declspec(section ".data") __attribute__((used, aligned(4))) float cam_t21C_4 = 0.01f;
#define lbl_eu_8052721C (*((float(*)[5])&cam_t21C_0))
__declspec(section ".data") __attribute__((used, aligned(4))) float cam_t230_0 = 0.27f;
__declspec(section ".data") __attribute__((used, aligned(4))) float cam_t230_1 = 0.2f;
__declspec(section ".data") __attribute__((used, aligned(4))) float cam_t230_2 = 0.13f;
__declspec(section ".data") __attribute__((used, aligned(4))) float cam_t230_3 = 0.1f;
__declspec(section ".data") __attribute__((used, aligned(4))) float cam_t230_4 = 0.07f;
#define lbl_eu_80527230 (*((float(*)[5])&cam_t230_0))
// .data joint-name pointer table (all slots are relocs, zero in-file; a real
// pointer table keeps MWCC from exiling an all-zero blob to .bss).
__declspec(section ".data") __attribute__((used, aligned(8)))
const void* lbl_eu_80527244[7] = {
    &sdata2_CfCamStr.m3D8, &sdata2_CfCamStr.sDC, &sdata2_CfCamStr.sE4,
    &sdata2_CfCamStr.sEC, &sdata2_CfCamStr.sF4, &sdata2_CfCamStr.sFC,
    &sdata2_CfCamStr.s404,
};


extern "C" cf::CfCamFollow* __ct__cf_CfCamFollow(cf::CfCamFollow* self, void* arg1,
                                                 void* arg2) {
    __ct__cf_CfCam(self, arg2);
    self->vtbl() = lbl_eu_80527260;

    noopAnimVec3(self->unk1C);
    writeVec3f(self->unk1C + 12, lbl_eu_806662DC, lbl_eu_806662DC, lbl_eu_806662DC);
    writeVec3f(self->unk1C + 24, lbl_eu_806662DC, lbl_eu_806662DC, lbl_eu_806662DC);
    noopAnimVec3(self->unk1C + 36);
    noopAnimVec3(self->unk1C + 48);
    noopAnimVec3(self->unk1C + 60);
    writeVec3f(self->unk1C + 72, lbl_eu_806662DC, lbl_eu_806662DC, lbl_eu_806662DC);
    cfCam_nopBef8(self->unk70);
    cfCam_nopCtorSlot(self->unk130);

    self->unk164 = static_cast<cf::CfObject*>(arg1);

    noopAnimVec3(self->unk168);
    noopAnimVec3(self->unk168 + 12);
    writeVec3f(self->unk168 + 24, lbl_eu_806662DC, lbl_eu_806662DC, lbl_eu_806662DC);
    writeVec3f(self->unk168 + 36, lbl_eu_806662DC, lbl_eu_806662DC, lbl_eu_806662DC);
    noopAnimVec3(self->unk168 + 48);
    noopAnimVec3(self->unk168 + 60);
    noopAnimVec3(self->unk168 + 72);
    noopAnimVec3(self->unk168 + 84);
    writeVec3f(self->unk168 + 96, lbl_eu_806662DC, lbl_eu_806662DC, lbl_eu_806662DC);
    cfCam_storeZeroU32(&self->unk1D4);
    cfCam_storeZeroU16(&self->unk1D8);

    self->unk1DC = lbl_eu_806662DC;
    self->unk1E0 = lbl_eu_80661B50;
    self->unk1E4 = lbl_eu_806662DC;
    self->unk1E8 = lbl_eu_806662DC;
    self->unk1EC = lbl_eu_806662DC;
    self->unk1F0 = lbl_eu_806662DC;
    self->unk1F4 = lbl_eu_806662DC;
    self->unk1F8 = lbl_eu_806662DC;
    self->unk1FC = lbl_eu_806662B8;
    self->unk200 = lbl_eu_806662DC;
    self->unk204 = lbl_eu_806662A0;
    self->unk21C = lbl_eu_806662DC;
    self->unk220 = lbl_eu_806662DC;
    self->unk224 = lbl_eu_806662DC;
    self->unk228 = lbl_eu_806662F0;
    self->unk234 = lbl_eu_806662DC;
    self->unk238 = lbl_eu_806662DC;
    self->unk24C = 0;
    self->unk252 = 0;
    self->unk254 = 0;
    self->unk256 = 0;
    self->unk258 = 0;

    lbl_eu_80663DEC = self;
    self->unk160 = arg2;

    cfCam_zeroVec3(self->unk10);
    cfCam_zeroVec3(self->unk168 + 60);
    cfCam_zeroVec3(self->unk168 + 72);
    cfCam_zeroVec3(self->unk168 + 84);
    cfCam_zeroUnk04(self);
    memset(cfCam_vec3AtIndex(self->unk70, 0), 0, 0xC0);

    self->unk250 = 0;
    self->unk22C = lbl_eu_806662D0;
    self->unk230 = lbl_eu_806662D0;

    // Real virtual calls: MWCC loads the vtable into r12 (lwz r12, 0(r29);
    // lwz r12, slot(r12)) right before each bctrl, matching retail's epilogue.
    self->cfCam_storeUnk08(1);
    self->func_8006BFDC();

    return self;
}

extern "C" void* cfCam_getUnk10Ptr(cf::CfCamFollow* self) { return self->unk10; }
extern "C" u32 cfCam_getUnk164(const cf::CfCamFollow* self) { return reinterpret_cast<u32>(self->unk164); }
extern "C" void* cfCam_getUnk40Ptr(cf::CfCamFollow* ptr) { return ptr->unk1C + 0x24; }
extern "C" void* cfCam_getUnk1CPtr(cf::CfCamFollow* self) { return self->unk1C; }
extern "C" bool cfCam_getTrue() { return true; }
// cfCam_isVecNear0: zero-vector test - true when every component magnitude is
// within the shared 0x8066A208 epsilon. Two-stage boolean structure mirrors
// retail (|x|,|y| checked first, then |z| behind a temp flag).
// noinline keeps the retail `bl cfCam_isVecNear0` inside func_80074090.
__declspec(noinline) int cfCam_isVecNear0(const ml::CVec3* v) {
    int result = 0;
    int temp = 0;
    if (ml::math::abs(v->x) <= lbl_eu_8066A208 && ml::math::abs(v->y) <= lbl_eu_8066A208) {
        temp = 1;
    }
    if (temp != 0 && ml::math::abs(v->z) <= lbl_eu_8066A208) {
        result = 1;
    }
    return result;
}
// cfCam_absFloat: |*self| (single-precision abs of a float). Defined under the
// C++-mangled name so the extern "C" declaration in CfCam.hpp has no in-TU
// body - MWCC then keeps a real `bl cfCam_absFloat` at call sites (retail
// keeps the call across every caller).
float cfCam_absFloat(u8* self) {
    double x = *(float*)self;
    x = __fabs(x);
    return (float)x;
}
extern "C" __declspec(noinline) float cfCam_vecLength(const void* v) {
    return PSVECMag(reinterpret_cast<const Vec*>(v));
}
extern "C" float cfCam_vecMag(const float* v) {
    return PSVECMag(reinterpret_cast<const Vec*>(v));
}
extern "C" void cfCam_setOrClearUnk04Bits(cf::CfCamFollow* r3, int r4, int r5) {
    cf::CfCamFollow* self = static_cast<cf::CfCamFollow*>(r3);
    if (r5) {
        self->field_0x04 |= r4;
    } else {
        self->field_0x04 &= ~r4;
    }
}
void cf::CfCamFollow::clearUnk04Bits(unsigned int mask) {
    field_0x04 &= ~mask;
}
// cf::CfCam base ctor: stores the retail vtable manually, zeroes +0x04 and
// saves the caller argument at +0x0C (matches CfCamFollow's flat-layout view).
extern "C" __declspec(noinline) void* __ct__cf_CfCam(void* self, void* arg) {
    cf::CfCam* cam = static_cast<cf::CfCam*>(self);
    cam->vtbl() = lbl_eu_805272E8;
    cam->field_0x04 = 0;
    cam->field_0x0C = arg;
    return self;
}
// cf::CfCam dtor: empty body - MWCC still emits the null-check + delete-flag
// machinery (cmpwi r3,0 / cmpwi r4,0; ble / bl __dl__FPv).
// noinline keeps the dtor a real symbol so CfCamFollow's dtor emits the
// retail `bl __dt__Q22cf5CfCamFv` instead of inlining the empty body.
__declspec(noinline) cf::CfCam::~CfCam() {}
// cf::CfCamFollow dtor: restores the follow-camera vtable, saves the last
// follow distance into the shared vec3 when a pad is still attached, clears
// the global active-camera pointer; base dtor + delete machinery follows.
cf::CfCamFollow::~CfCamFollow() {
    vtbl() = lbl_eu_80527260;
    if (cfCam_loadUnk08(this) == 1 && cfCam_testUnk04M(reinterpret_cast<int>(this), 0x4000) != 0) {
        lbl_eu_80570A38[0] = unk1F8;
    }
    lbl_eu_80663DEC = 0;
}
// cf::CfCamEvent dtor: empty body - MWCC emits the null-check, the base
// CfCamFollow dtor call (flag 0) and the delete-flag machinery.
// NOTE: retail __dt__Q22cf10CfCamEventFv cannot be defined here - CfCamEvent.hpp
// (read-only) does not declare ~CfCamEvent().
extern "C" __declspec(noinline) void cfCam_nopCtorSlot(void* self) {}
// Copy ml::CVec3::zero into a caller-provided CVec3 (retail: lwzu+lwz/stw
// struct-copy triple from the static zero vector). noinline keeps the bl.
extern "C" __declspec(noinline) void cfCam_zeroVec3(void* self) {
    *static_cast<ml::CVec3*>(self) = ml::CVec3::zero;
}
extern "C" __declspec(noinline) void cfCam_zeroUnk04(cf::CfCamFollow* p) {
    p->field_0x04 = 0;
}
extern "C" void cfCam_setUnk08(cf::CfCamFollow* self, int val) { self->field_0x08 = val; }
extern "C" __declspec(noinline) void cfCam_nopBef8(void* self) {}
__declspec(noinline) void cfCam_storeZeroU32(void* self) { *(u32*)self = 0; }
extern "C" u32 cfCam_getUnk08(cf::CfCamFollow* self) { return self->field_0x08; }
extern "C" bool cfCam_testUnk04Bits(const cf::CfCamFollow* self, unsigned int mask) { return (self->field_0x04 & mask) != 0; }
extern "C" int cfCam_getBit1_0x64(void* p) {
    return (*(int*)((char*)p + 0x64) >> 1) & 1;
}
extern "C" void cfCam_setField1E0AndGlobal(cf::CfCamFollow* ptr, float f) {
    lbl_eu_80661B50 = f;
    ptr->unk1E0 = f;
}
// Retail cfCam_setClear04: set/clear the shared flag word at +4.
extern "C" __declspec(noinline) void cfCam_setClear04(void* obj, u32 mask, int flag) {
    if (flag != 0)
        *reinterpret_cast<unsigned int*>(reinterpret_cast<char*>(obj) + 4) |= mask;
    else
        *reinterpret_cast<unsigned int*>(reinterpret_cast<char*>(obj) + 4) &= ~mask;
}
extern "C" int cfCam_getUnkC4(void* self) { return *(int*)((char*)self + 0xc4); }
extern "C" int cfCam_getBit0_0x530(void* p) {
    return *(unsigned short*)((unsigned char*)p + 0x530) & 1;
}
extern "C" bool cfCam_testBits2_3_0x530(const unsigned char* this_) { return (*(const unsigned short*)(this_ + 0x530) & 0x000C) != 0; }
extern "C" bool cfCam_testBits4_5_0x530(const u8* self) { return (*(const unsigned short*)((const unsigned char*)self + 0x530) & 0x30) != 0; }
extern "C" bool cfCam_testUnk1D4Bits(cf::CfCamFollow* self, unsigned int mask) { return (self->unk1D4 & mask) != 0; }
// cfCam_mtxGetTrans: copy the translation column of a 3x4 matrix (row-major) into a vec3.
// Declare x/y/z first (FPR coloring x->f0, y->f1, z->f2), assign in reverse
// (loads descend), store forward (stores ascend) - MWCC_CASES §10158 shape.
extern "C" void cfCam_mtxGetTrans(float* dst, const float* src) {
    float x, y, z;
    z = src[11];
    y = src[7];
    x = src[3];
    dst[0] = x;
    dst[1] = y;
    dst[2] = z;
}
extern "C" void cfCam_clearUnk1D4Bits(cf::CfCamFollow* _this, unsigned int mask) {
    _this->unk1D4 &= ~mask;
}
extern "C" int cfCam_getBit1_0x4EC(void* _this) {
    return (*(int*)((char*)_this + 0x4EC) >> 1) & 1;
}
// Store the shared follow-distance float into *out, then re-init the global
// camera-relative position vec3.
extern "C" void cfCam_pullFollowD(u8* unused, f32* out) {
    *out = lbl_eu_80663DE4;
    copyVec3Words(unused, lbl_eu_80570A38);
}
// cfCam_storeFollowD: store the follow distance into both .sbss mirrors and the
// first component of the shared camera-relative position into both .bss vec3s.
extern "C" void cfCam_storeFollowD(const ml::CVec3* v, f32 f) {
    float x = v->x;
    lbl_eu_80663DE8 = f;
    lbl_eu_80663DE4 = f;
    lbl_eu_80570A44[0] = x;
    lbl_eu_80570A38[0] = x;
}
// cfCam_syncFollowD: copy the shared follow-distance float into the .sbss mirror,
// then re-init the first component of the global camera-relative vec3.
extern "C" void cfCam_syncFollowD() {
    lbl_eu_80663DE8 = lbl_eu_80663DE4;
    lbl_eu_80570A44[0] = lbl_eu_80570A38[0];
}
extern "C" void cfCam_zeroUnk1D4(cf::CfCamFollow* arg0) {
    arg0->unk1D4 = 0;
}
extern "C" u32 cfCam_getSignBitUnk04(cf::CfCamFollow* p) {
    return p->field_0x04 >> 31;
}
extern "C" u32 cfCam_getUnk04(cf::CfCamFollow* self) { return self->field_0x04; }
extern "C" float cfCam_getField1E0(cf::CfCamFollow* self) { return self->unk1E0; }
extern "C" float cfCam_scaleByConst(float f) {
    extern float lbl_eu_8066A20C;
    return f * lbl_eu_8066A20C;
}
extern "C" void cfCam_setBit9_Unk04(unsigned* p) {
    p[1] |= 0x200;
}
extern "C" void* cfCam_getUnk9CPtr(void* ptr) { return (char*)ptr + 0x9c; }
extern "C" bool cfCam_testUnk04Bit8(void* param_1) {
    return ((bool (*)(void*, int))cfCam_testUnk04Bits)(param_1, 256);
}
unsigned int cf::CfCamFollow::getBit26_0x4EC() {
    return (*(unsigned int*)((u8*)this + 0x4ec) >> 26) & 1;
}
extern "C" int cfCam_getBit24_0x4EC(void* p) {
    return (*(unsigned int*)((char*)p + 0x4EC) >> 24) & 1;
}
extern "C" int cfCam_testUnk4ECBits(void* self, unsigned int mask) { return ((*((unsigned int*)((char*)self + 0x4ec)) & mask) != 0); }
extern "C" uint32_t cfCam_getBit25_0x4EC(void* self) {
    return (*(uint32_t*)((char*)self + 0x4ec) >> 25) & 1;
}
extern "C" int cfCam_getBit23_0x4EC(void* self) {
    return (*(const unsigned int*)((const char*)self + 0x4ec) >> 23) & 1;
}
float cf::CfCam::getUnk4FC() {
    return *(float *)((char *)this + 0x4FC);
}
extern "C" float cfCam_getUnk4F8(void* self) { return *(float*)((char*)self + 0x4f8); }
extern "C" float cfCam_getUnk508(const void* self) {
    return *(const float*)((const char*)self + 0x508);
}
extern "C" int cfCam_getBit13_0x4EC(void* self) { return (*(uint32_t*)((char*)self + 0x4ec) >> 13) & 1; }
extern "C" unsigned int cfCam_getBit2_0x64(void* self) {
    unsigned int val = *(unsigned int*)((unsigned char*)self + 0x64);
    return (val >> 2) & 1U;
}
extern "C" int cfCam_getUnk90E4(u8* self) { return *(int *)((char *)self + 0x90e4); }
extern "C" unsigned int cfCam_getBit27_0x64(u8* self) {
    unsigned int word = *(unsigned int *)((char *)self + 0x64);
    return (word >> 27) & 1U;
}
extern "C" f32 cfCam_vecLenSq(const nw4r::math::VEC3* vector) {
    return nw4r::math::VEC3LenSq(vector);
}
extern int lbl_eu_80663DF0;

extern "C" int cfCam_getGlobal_80663DF0()
{
    return lbl_eu_80663DF0;
}
extern "C" bool cfCam_callCheckFlag(void* self) { return CamEvtFetchSlotPtr(self, 0); }
extern "C" void cfCam_copyInt(int* dst, int* src) {
    *dst = *src;
}
extern "C" int cfCam_getConst16() { return 0x10; }
extern "C" void cfCam_copyU32(unsigned* dst, const unsigned* src) {
    *dst = *src;
}
extern "C" void cfCam_copyU16(u16* dest, const u16* src) {
    *dest = *src;
}
extern unsigned int lbl_eu_80663E28;

extern "C" int cfCam_getBit24_GlobalE28() {
    return (lbl_eu_80663E28 >> 24) & 1;
}
extern "C" void* cfCam_getUnk3D8Ptr(void* self) { return static_cast<char*>(self) + 0x3d8; }
extern "C" int cfCam_getBit9_Arg1(unsigned int* arg0) {
    return (arg0[1] >> 9) & 1;
}
// cfCam_copyBlock48: whole-block copy of the 48-byte (12-word) camera data block.
__declspec(noinline) void cfCam_copyBlock48(CfCamWordBlock48* dst, const CfCamWordBlock48* src) {
    *dst = *src;
}
// cfCam_testGlobalE24: flag-bit probe on lbl_eu_80663E24 with a fixed mask.
// Branchless idiom: r0 = flag; r3 = r0 & mask; r3 = -r3; r0 = r0 | r3; return r0 & 1.
extern "C" bool cfCam_testGlobalE24() {
    unsigned int flag = lbl_eu_80663E24;
    unsigned int x = flag & 0xAFA40000u;
    return ((((unsigned int)-(int)x | x) >> 31) & 1) != 0;
}
// func_80071694: quaternion -> 3x3 rotation matrix.
extern "C" __declspec(noinline) void func_80071694(ml::CMat33* out, const ml::CQuat* q) {
    float one = lbl_eu_806662D0;
    float w = q->w;
    float y = q->y;
    float z = q->z;
    float x = q->x;
    float twoX = x + x;
    float twoY = y + y;
    float twoZ = z + z;

    float YY = y * twoY;
    float ZZ = z * twoZ;
    float XX = x * twoX;
    float XY = x * twoY;
    float ZW = w * twoZ;

    out->m[0][0] = one - (YY + ZZ);

    out->m[1][1] = one - (XX + ZZ);
    out->m[2][2] = one - (XX + YY);

    float XZ = x * twoZ;
    float YW = w * twoY;

    out->m[0][1] = XY - ZW;
    out->m[1][0] = XY + ZW;
    out->m[0][2] = XZ + YW;
    out->m[2][0] = XZ - YW;

    float YZ = y * twoZ;
    float XW = w * twoX;

    out->m[1][2] = YZ - XW;
    out->m[2][1] = YZ + XW;
}
extern "C" void cfCam_copy4Words(void *r3, const void *r4) {
    const unsigned int *src = (const unsigned int *)r4;
    unsigned int *dst = (unsigned int *)r3;
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
    dst[3] = src[3];
}
extern "C" int cfCam_getBit12_Unk04(void* ptr) {
    return (*(unsigned int*)((char*)ptr + 4) >> 12) & 1;
}
extern "C" void cfCam_clearUnk04Bits_EDFF(cf::CfCamFollow* self) { self->field_0x04 &= 0xFFFFEDFFu; }

extern "C" bool cfCam_setUnk1E0AndValidate(void* obj, float val) {
    *(float*)((char*)obj + 0x1e0) = val;
    return func_8049EB60(obj);
}
extern "C" void cfCam_stub_80071B74() {}
extern "C" void cfCam_stub_80071B78() {}
extern "C" int cfCam_getGlobal_80665958() {
    extern int lbl_eu_80665958;
    return lbl_eu_80665958;
}
extern "C" void* cfCam_getUnk60Ptr(void* self) { return static_cast<char*>(self) + 0x60; }
extern "C" void* cfCam_getUnk6CPtr(void* ptr) {
    return (char*)ptr + 0x6c;
}
extern "C" int cfCam_getBit19_0x0C(void* _this) {
    return ((*(unsigned*)((char*)_this + 0xc)) >> 19) & 1;
}
// isGlobalCamFlagSet__Fi: flag-bit probe.  Retail codegen is the branchless idiom
// r0 = flag; r3 = r0 & mask; r3 = -r3; r0 = r0 | r3; return r0 & 1.
extern "C" bool isGlobalCamFlagSet__Fi(s32 mask) {
    unsigned int flag = lbl_eu_80663E24;
    unsigned int x = flag & (unsigned int)mask;
    return ((((unsigned int)-(int)x | x) >> 31) & 1) != 0;
}

extern "C" int cfCam_getBit6_GlobalE24() {
    return (lbl_eu_80663E24 >> 6) & 1;
}
extern "C" float cfCam_getUnk14Float(const void* arg) { return *(const float*)((const char*)arg + 0x14); }
extern "C" void cfCam_resetUnk10(u8* p, void* src) {
    copyVec3Words(p + 0x10, src);
}
// cfCam_extractTrans: copy the translation column of a 3x4 matrix (row-major) into a vec3.
// Declare x/y/z first (FPR coloring x->f0, y->f1, z->f2), assign in reverse
// (loads descend), store forward (stores ascend) - MWCC_CASES §10158 shape.
// noinline keeps the retail `bl cfCam_extractTrans` inside func_80074090.
extern "C" __declspec(noinline) void cfCam_extractTrans(float* dst, const float* src) {
    float x, y, z;
    z = src[11];
    y = src[7];
    x = src[3];
    dst[0] = x;
    dst[1] = y;
    dst[2] = z;
}
extern "C" void cfCam_setMtxTranslation(float* dst, const float* src) {
    dst[3] = src[0];
    dst[7] = src[1];
    dst[11] = src[2];
}
// func_800743C0: in-place inverse of a rigid-transform 3x4 matrix: transpose
// the rotation part and replace the translation with -(R^T * t). The three
// negated translation components are named locals first (retail loads them
// before the nine rotation entries), then the three products are computed in
// _13/_03/_23 order into locals so all translation stores land after the
// transpose stores, like retail.
void func_800743C0(nw4r::math::MTX34* m) {
    f32 n13 = -m->_13;
    f32 n03 = -m->_03;
    f32 n23 = -m->_23;
    f32 r10 = m->_10;
    f32 r01 = m->_01;
    f32 r12 = m->_12;
    f32 r11 = m->_11;
    f32 r21 = m->_21;
    f32 r02 = m->_02;
    f32 r00 = m->_00;
    f32 r20 = m->_20;
    f32 r22 = m->_22;

    f32 t13 = r01 * n03 + r11 * n13 + r21 * n23;
    f32 t03 = r00 * n03 + r10 * n13 + r20 * n23;
    f32 t23 = r02 * n03 + r12 * n13 + r22 * n23;

    m->_01 = r10;
    m->_10 = r01;
    m->_02 = r20;
    m->_20 = r02;
    m->_12 = r21;
    m->_21 = r12;

    m->_03 = t03;
    m->_13 = t13;
    m->_23 = t23;
}
extern "C" bool cfCam_testBits(const unsigned* value, unsigned mask) { return (*value & mask) != 0; }
extern "C" int cfCam_getZero(void* self) { return 0x0; }
extern "C" void* cfCam_getNestedPtr118(cf::CfCamFollow* p) {
    return static_cast<char*>(p->field_0x0C) + 0x118;
}
extern "C" float cfCam_getFloat1E0(cf::CfCamFollow* self) { return self->unk1E0; }
extern "C" void cfCam_copyVec3U32(u32* self, const u32* src)
{
    self[10] = src[0];
    self[11] = src[1];
    self[12] = src[2];
}
// sinit_80074D7C: static-initializer for the camera default tables. Fills
// nine direction/default vec3 slots at 0x80570A20 (several derived from the
// .sdata2 tuning constants: a degree-to-radian product family and one ratio),
// zeroes the trailing state words, and seeds the per-camera-id defaults
// (stride-0x34 CamDefEntry records) at 0x80527160. The local vec3 scratch
// slots are copied into the default table entry-by-entry, mirroring retail's
// dead stack temporaries.
void sinit_80074D7C() {
    f32 one = lbl_eu_806662DC;
    f32 cD0 = lbl_eu_806662D0;
    f32 den = lbl_eu_80661B38;
    f32 ratio = cD0 / den;

    volatile ml::CVec3 t[6];

    f32 b60 = lbl_eu_80661B60;
    f32 c39C = lbl_eu_8066639C;
    f32 radScale = lbl_eu_8066A210;
    f32 prod40C = lbl_eu_8066640C * radScale;
    f32 prodBC = lbl_eu_806662BC * radScale;
    f32 prod410 = lbl_eu_80666410 * radScale;
    f32 prodB4C = lbl_eu_80661B4C * radScale;
    f32 prod414 = lbl_eu_80666414 * radScale;

    lbl_eu_80570A20.dir[0].x = one;
    lbl_eu_80570A20.dir[0].y = c39C;
    lbl_eu_80570A20.dir[0].z = one;
    lbl_eu_80663DC8 = prod40C;
    lbl_eu_80663DCC = prodBC;
    lbl_eu_80663DD0 = ratio;
    lbl_eu_80663DD4 = prod410;
    lbl_eu_80663DD8 = prod414;
    lbl_eu_80570A20.dir[1].x = one;
    lbl_eu_80570A20.dir[1].y = c39C;
    lbl_eu_80570A20.dir[1].z = one;
    lbl_eu_80663DE4 = b60;
    lbl_eu_80570A20.dir[2].x = prodB4C;
    lbl_eu_80570A20.dir[2].y = one;
    lbl_eu_80570A20.dir[2].z = one;
    lbl_eu_80663DE8 = b60;
    lbl_eu_80570A20.dir[3].x = prodB4C;
    lbl_eu_80570A20.dir[3].y = one;
    lbl_eu_80570A20.dir[3].z = one;
    lbl_eu_80570A20.dir[4].x = one;
    lbl_eu_80570A20.dir[4].y = one;
    lbl_eu_80570A20.dir[4].z = one;
    lbl_eu_80570A20.dir[5].x = one;
    lbl_eu_80570A20.dir[5].y = cD0;
    lbl_eu_80570A20.dir[5].z = one;
    lbl_eu_80570A20.dir[6].x = one;
    lbl_eu_80570A20.dir[6].y = one;
    lbl_eu_80570A20.dir[6].z = one;
    lbl_eu_80570A20.dir[7].x = one;
    lbl_eu_80570A20.dir[7].y = one;
    lbl_eu_80570A20.dir[7].z = one;

    lbl_eu_80570A20.dir[8].x = one;
    lbl_eu_80570A20.dir[8].y = one;
    lbl_eu_80570A20.dir[8].z = one;
    u32* tail = reinterpret_cast<u32*>(&lbl_eu_80570A20.rest[0x114]);
    tail[0] = 0;
    tail[1] = 0;
    *reinterpret_cast<u16*>(tail + 2) = 0;

    f32 c29C = lbl_eu_8066629C;
    f32 c36C = lbl_eu_8066636C;
    f32 c3C8 = lbl_eu_806663C8;
    f32 c2A0 = lbl_eu_806662A0;
    f32 c304 = lbl_eu_80666304;

    t[5].x = prodB4C;
    t[5].y = prodB4C;
    t[5].z = one;
    lbl_eu_80527160.entries[0].unk04.x = prodB4C;
    lbl_eu_80527160.entries[0].unk04.y = prodB4C;
    lbl_eu_80527160.entries[0].unk04.z = one;
    t[4].x = c36C;
    t[4].y = c36C;
    t[4].z = one;
    lbl_eu_80527160.entries[0].unk10.x = c36C;
    lbl_eu_80527160.entries[0].unk10.y = c36C;
    lbl_eu_80527160.entries[0].unk10.z = one;
    t[3].x = c3C8;
    t[3].y = c2A0;
    t[3].z = one;
    lbl_eu_80527160.entries[1].unk04.x = c3C8;
    lbl_eu_80527160.entries[1].unk04.y = c2A0;
    lbl_eu_80527160.entries[1].unk04.z = one;
    t[2].x = one;
    t[2].y = cD0;
    t[2].z = one;
    lbl_eu_80527160.entries[1].unk10.x = one;
    lbl_eu_80527160.entries[1].unk10.y = cD0;
    lbl_eu_80527160.entries[1].unk10.z = one;
    t[1].x = c29C;
    t[1].y = c36C;
    t[1].z = one;
    lbl_eu_80527160.entries[2].unk04.x = c29C;
    lbl_eu_80527160.entries[2].unk04.y = c36C;
    lbl_eu_80527160.entries[2].unk04.z = one;
    t[0].x = one;
    t[0].y = c304;
    t[0].z = one;
    lbl_eu_80527160.entries[2].unk10.x = one;
    lbl_eu_80527160.entries[2].unk10.y = c304;
    lbl_eu_80527160.entries[2].unk10.z = one;
}

// Class_80296898 is owned by kyoshin/code_80296898.hpp (real singleton
// class); this TU uses it only through getInstance() with explicit byte
// casts, so no local struct is needed.

// cfCam_loadTuneTbl: refresh the shared camera tables from the Class_80296898
// instance's selection bytes. inst[3] picks the 0x80527208 table entry (copied
// func_8006B720: camera-default reload for camera id arg1. Builds two bdat
// column names from the message string table, resolves the active game-manager
// camera row (vtable slot 0x60 gate supplies the row override at +0x8C), reads
// both cells and scales them into lbl_eu_80570A2C[1] / lbl_eu_80661B4C. When
// arg2 is set, flags word +0x4 of the state object gets bit 0x20000000.
extern "C" void func_8006B720(int arg1, int arg2) {
    lbl_eu_80661BA4 = arg1;
    int col = 1;
    // Two separate getter calls (retail emits both; no CSE).
    if (getActiveCameraObject__Q22cf13CfGameManagerFv() != 0) {
        cf::CfCamFollow* cam = static_cast<cf::CfCamFollow*>(
            getActiveCameraObject__Q22cf13CfGameManagerFv());
        void* ret = cam->cfCam_loadUnk164();
        if (ret != 0) {
            col = reinterpret_cast<cf::CfCamRowView*>(ret)->rowId;
        }
    }

    void* fp = getFP(lbl_eu_804FB4F0);
    const char* base = lbl_eu_804FB4F0;
    struct NameBuf {
        char str[0x20];
        int len;
    };
    // Declared in retail stack-slot order: cellB, cellA, nameB, nameA,
    // convA, convB (execution order below still matches retail's).
    short* cellB;
    short* cellA;
    NameBuf nameB;
    NameBuf nameA;
    volatile union {
        double d;
        u32 w[2];
    } convA;
    volatile union {
        double d;
        u32 w[2];
    } convB;
    const char* srcA = base + 0xC;
    nameA.len = strlen(srcA);
    strcpy(nameA.str, srcA);
    const char* srcB = base + 0xF;
    nameB.len = strlen(srcB);
    strcpy(nameB.str, srcB);
    const char* entry = lbl_eu_805271FC[arg1];
    int entryLen = strlen(entry);
    strcat(nameA.str, entry);
    nameA.len += entryLen;
    entry = lbl_eu_805271FC[arg1]; // retail reloads the table entry here
    entryLen = strlen(entry);
    strcat(nameB.str, entry);
    nameB.len += entryLen;

    // s16 -> f32 via the 0x43300000 double trick; subtracting the named retail
    // blob keeps the lfd reloc on lbl_eu_806662A8 instead of a TU-local pool.
    cellA = reinterpret_cast<short*>(getBdatStringColumnValue(fp, nameA.str, col));
    convA.w[1] = (u32)(s32)*cellA ^ 0x80000000u;
    convA.w[0] = 0x43300000;
    f32 scaledA = lbl_eu_8066629C * (f32)(convA.d - lbl_eu_806662A8);
    cellB = reinterpret_cast<short*>(getBdatStringColumnValue(fp, nameB.str, col));
    lbl_eu_80570A2C[1] = scaledA;
    convB.w[1] = (u32)(s32)*cellB ^ 0x80000000u;
    convB.w[0] = 0x43300000;
    lbl_eu_80661B4C = lbl_eu_806662A0 * (f32)(convB.d - lbl_eu_806662A8);

    if (arg2 != 0) {
        UnkClass_800821F8* state = getCameraDataBlock__Q22cf13CfGameManagerFv();
        if (state != 0) {
            reinterpret_cast<cf::UnkClass800821F8FlagView*>(state)->flags |= 0x20000000;
        }
    }
}

// into the .sdata float 0x80661B3C); inst[2] picks the 0x8052721C table entry
// and the +0x40/+0x44/+0x48/+0x74 .sdata floats. The two .sdata2 constants
// stay in FPRs (f1/f0).
void cfCam_loadTuneTbl() {
    const unsigned char* inst = reinterpret_cast<const unsigned char*>(Class_80296898::getInstance());
    unsigned int idx3 = inst[3];
    lbl_eu_80661B3C = lbl_eu_80527208[idx3];
    unsigned int idx2 = inst[2];
    lbl_eu_80661B74 = lbl_eu_8052721C[idx2];
    lbl_eu_80661B40 = lbl_eu_806662B0;
    lbl_eu_80661B44 = lbl_eu_806662B0;
    lbl_eu_80661B48 = lbl_eu_806662B4;
}

extern "C" void cfCam_updateLbl61BA8() {
    unsigned char idx = reinterpret_cast<unsigned char*>(Class_80296898::getInstance())[4];
    lbl_eu_80661BA8 = lbl_eu_80527230[idx];
}
// func_8006B980: camera-constant init + selection-byte table refresh. Stores
// the initial .sdata camera globals (positions, follow distance, flag word,
// selection byte) from .sdata2 constants, then mirrors the three
// Class_80296898 selection bytes through the 0x80527208 / 0x8052721C /
// 0x80527230 tables (cfCam_loadTuneTbl plus the +0x4 entry) and hands both args
// to func_8006B720.
extern "C" __declspec(noinline) void func_8006B980(int arg1, int arg2) {
    lbl_eu_80661B60 = lbl_eu_806662B8;
    lbl_eu_80661B98 = lbl_eu_806662BC;
    lbl_eu_80661B9C = lbl_eu_806662C0;
    lbl_eu_80661B7C = 4;
    lbl_eu_80661B8C = lbl_eu_806662C4;
    lbl_eu_80661B90 = lbl_eu_806662C8;
    lbl_eu_80663DE1 = 0;
    lbl_eu_80661B38 = lbl_eu_806662CC;
    lbl_eu_80663DD0 = lbl_eu_806662D0 / lbl_eu_806662CC;
    const_cast<float&>(lbl_eu_80663DD8) = lbl_eu_8066A210 * lbl_eu_806662D4;
    const u8* inst = reinterpret_cast<const u8*>(Class_80296898::getInstance());
    lbl_eu_80661B3C = lbl_eu_80527208[inst[3]];
    unsigned int idx2 = inst[2];
    lbl_eu_80661B74 = lbl_eu_8052721C[idx2];
    lbl_eu_80661B40 = lbl_eu_806662B0;
    lbl_eu_80661B44 = lbl_eu_806662B0;
    lbl_eu_80661B48 = lbl_eu_806662B4;
    const u8* inst2 = reinterpret_cast<const u8*>(Class_80296898::getInstance());
    lbl_eu_80661BA8 = lbl_eu_80527230[inst2[4]];
    func_8006B720(arg1, arg2);
}
// func_8006BB20: sine-table blend factor. Offsets the input FIdx angle by
// lbl_eu_806662D8; when positive, a (1.0 - x / lbl_eu_806662E0) factor is
// clamped into [0,1], scaled by pi/2 * lbl_eu_806662E4 and fed through the
// nw4r sine table; otherwise the initial 1.0 falls through unchanged (retail
// returns f31 which was preloaded with 1.0 before the branch). A result above
// 0.1 is wrapped back into [0, 2pi) with fmod (arg selects whether the wrap
// runs at all).
float func_8006BB20(void* arg, float angle) {
    // Offset the FIdx angle, then fold it into [0,1] as a sine-table phase
    f32 s = lbl_eu_806662DC; // 0.0f - sin-input accumulator (retail f2)
    f32 v = lbl_eu_806662D0; // 1.0f - return value / clamp cap (retail f31)
    f32 x = angle - lbl_eu_806662D8;
    if (x > s) {
        f32 t = v - x / lbl_eu_806662E0;
        if (t >= s) {
            if (t <= v) {
                v = t;
            }
            s = v;
        }
        v = nw4r::math::SinFIdx(s * lbl_eu_8066A200 * lbl_eu_806662E4);
        if (arg != 0 && v > lbl_eu_806662A0) {
            f32 m = (f32)fmod((double)v, lbl_eu_806662E8);
            if (v >= lbl_eu_806662DC) {
                v -= m;
            } else {
                v += m;
            }
        }
    }
    return v;
}
// func_8006BFDC: reset the camera constants block - refreshes the shared
// direction vec3 (0x80570A20 -> 0x80570A2C), stores the tuning constants into
// the .sdata globals, claims the 0x4 / 0x8 / 0x200 flags, reinitializes the
// selection tables (func_8006B980) and resets the object through vtable slot
// 0xC.
void func_8006BFDC(cf::CfCamFollow* self) {
    copyVec3Words(lbl_eu_80570A2C, &lbl_eu_80570A20);
    f32 cF8 = lbl_eu_806662F8;
    f32 c304 = lbl_eu_80666304;
    f32 cBC = lbl_eu_806662BC;
    f32 cC0 = lbl_eu_806662C0;
    f32 cF4 = lbl_eu_806662F4;
    f32 cE0 = lbl_eu_806662E0;
    f32 cB4 = lbl_eu_806662B4;
    f32 cFC = lbl_eu_806662FC;
    f32 c300 = lbl_eu_80666300;
    f32 cDC = lbl_eu_806662DC;
    f32 cD0 = lbl_eu_806662D0;
    f32 cB8 = lbl_eu_806662B8;
    f32 c308 = lbl_eu_80666308;
    f32 c30C = lbl_eu_8066630C;
    f32 cC4 = lbl_eu_806662C4;
    f32 cC8 = lbl_eu_806662C8;
    f32 c310 = lbl_eu_80666310;
    f32 c314 = lbl_eu_80666314;
    lbl_eu_80661B3C = cF4;
    lbl_eu_80661B74 = cF4;
    lbl_eu_80661B40 = cF8;
    lbl_eu_80661B44 = cF8;
    lbl_eu_80661B48 = cB4;
    lbl_eu_80661B4C = cFC;
    lbl_eu_80661B50 = c300;
    lbl_eu_80663DDC = cDC;
    lbl_eu_80661B54 = c304;
    lbl_eu_80661B58 = c304;
    lbl_eu_80661B5C = cD0;
    lbl_eu_80661B60 = cB8;
    lbl_eu_80661B64 = c308;
    lbl_eu_80661B68 = cBC;
    lbl_eu_80661B6C = cBC;
    lbl_eu_80661B70 = cE0;
    lbl_eu_80661B78 = c30C;
    lbl_eu_80661B88 = c304;
    lbl_eu_80661B7C = 4;
    lbl_eu_80661B80 = cF8;
    lbl_eu_80661B84 = cE0;
    lbl_eu_80661B8C = cC4;
    lbl_eu_80661B90 = cC8;
    lbl_eu_80661B94 = cC0;
    lbl_eu_80661B98 = cBC;
    lbl_eu_80661B9C = cC0;
    lbl_eu_80661BA0 = c310;
    lbl_eu_80661BA8 = c314;
    lbl_eu_80661BAC = cC0;
    cfCam_setClear04(self, 0x4, 1);
    cfCam_setClear04(self, 0x8, 1);
    func_8006B980(lbl_eu_80661BA4, 0);
    cfCam_setClear04(self, 0x200, 1);
    self->func_8006CC68(0);
}
// cfCam_setFollowTg: stash the parent handle at +0x164, then - when a handle is
// present and the sibling check cfCam_bit1At0x64 accepts it - run the global
// 0x80661BA4 state routine.
void cfCam_setFollowTg(cf::CfCamFollow* self, void* arg) {
    self->unk164 = static_cast<cf::CfObject*>(arg);
    if (arg != 0 && cfCam_bit1At0x64(arg) != 0) {
        func_8006B720(lbl_eu_80661BA4, 0);
    }
}
__declspec(noinline) void func_8006C1C8(cf::CfCamFollow* self) {}
// cfCam_andcUnk04: clear the flag word at +4 (retail: lwz/andc/stw +0x4(r3)).
__declspec(noinline) void cfCam_andcUnk04(void* self, int mask) {
    *reinterpret_cast<unsigned int*>(reinterpret_cast<char*>(self) + 4) &= ~static_cast<unsigned int>(mask);
}
// Retail cfCam_bit1At0x64: return bit 1 of the word at +100.
__declspec(noinline) int cfCam_bit1At0x64(void* self) {
    return (*reinterpret_cast<unsigned int*>(reinterpret_cast<char*>(self) + 100) >> 1) & 1;
}
// Tiny accessors (retail thunks).
extern "C" __declspec(noinline) void* cfCam_getPlus10(void* self) { return static_cast<char*>(self) + 16; }
extern "C" __declspec(noinline) void* cfCam_loadUnk164(void* self) { return *reinterpret_cast<void**>(reinterpret_cast<char*>(self) + 356); }
extern "C" __declspec(noinline) void* cfCam_getPlus40(void* self) { return static_cast<char*>(self) + 64; }
extern "C" __declspec(noinline) void* cfCam_getPlus1C(void* self) { return static_cast<char*>(self) + 28; }
extern "C" __declspec(noinline) void cfCam_storeUnk08(void* self, void* v) { *reinterpret_cast<void**>(reinterpret_cast<char*>(self) + 8) = v; }
__declspec(noinline) u32 cfCam_loadUnk08(void* self) { return *reinterpret_cast<u32*>(reinterpret_cast<char*>(self) + 8); }
__declspec(noinline) int cfCam_getSignBit04(void* self) {
    return (*reinterpret_cast<unsigned int*>(reinterpret_cast<char*>(self) + 4) >> 31) & 1;
}
__declspec(noinline) cf::CfCamFollow* cfCam_getActiveCam() { extern int lbl_eu_80665958; return (cf::CfCamFollow*)lbl_eu_80665958; }
__declspec(noinline) ml::CVec3* cfCam_getPlus60(cf::CfCamFollow* self) { return reinterpret_cast<ml::CVec3*>(reinterpret_cast<char*>(self) + 96); }
extern "C" __declspec(noinline) void cfCam_nopVirt24() {}
extern "C" __declspec(noinline) unsigned int cfCam_returnZero() { return 0; }
// More tiny accessors (retail thunks).
extern "C" __declspec(noinline) void* cfCam_loadPlus4(void* self) { return *reinterpret_cast<void**>(reinterpret_cast<char*>(self) + 4); }
extern "C" __declspec(noinline) f32 cfCam_getFloat1E0F(void* self) { return *reinterpret_cast<f32*>(reinterpret_cast<char*>(self) + 480); }
extern "C" __declspec(noinline) f32 cfCam_get1E0Again(void* self) { return *reinterpret_cast<f32*>(reinterpret_cast<char*>(self) + 480); }
extern "C" __declspec(noinline) void* cfCam_getPlus9C(void* self) { return static_cast<char*>(self) + 156; }
extern "C" __declspec(noinline) void* cfCam_nestedPlus118(void* self) {
    return reinterpret_cast<char*>(*reinterpret_cast<void**>(reinterpret_cast<char*>(self) + 12)) + 280;
}
extern "C" __declspec(noinline) void cfCam_copyToPlus28(void* self, const void* src) {
    const u32* s = static_cast<const u32*>(src);
    u32* d = reinterpret_cast<u32*>(reinterpret_cast<char*>(self) + 40);
    d[0] = s[0];
    d[1] = s[1];
    d[2] = s[2];
}
extern "C" __declspec(noinline) void cfCam_copyToPlus10(void* dst, const void* src) {
    copyVec3Words(static_cast<char*>(dst) + 16, const_cast<void*>(src));
}
extern "C" __declspec(noinline) int cfCam_getBit19_0C(void* self) {
    return (*reinterpret_cast<u32*>(reinterpret_cast<char*>(self) + 12) >> 19) & 1;
}
extern "C" __declspec(noinline) int cfCam_getE24Bit6() {
    return (lbl_eu_80663E24 >> 6) & 1;
}
extern "C" __declspec(noinline) f32 cfCam_getFloat14(void* self) { return *reinterpret_cast<f32*>(reinterpret_cast<char*>(self) + 20); }
// func_8006DC* probe series: flag bits of the word at +0x4EC / floats at +0x4F8..+0x508.
extern "C" __declspec(noinline) int cfCam_getBit26_4EC(void* self) { return (*reinterpret_cast<u32*>(reinterpret_cast<char*>(self) + 1260) >> 26) & 1; }
extern "C" __declspec(noinline) int cfCam_getBit24_4EC(void* self) { return (*reinterpret_cast<u32*>(reinterpret_cast<char*>(self) + 1260) >> 24) & 1; }
extern "C" __declspec(noinline) int cfCam_getBit25_4EC(void* self) { return (*reinterpret_cast<u32*>(reinterpret_cast<char*>(self) + 1260) >> 25) & 1; }
extern "C" __declspec(noinline) int cfCam_getBit23_4EC(void* self) { return (*reinterpret_cast<u32*>(reinterpret_cast<char*>(self) + 1260) >> 23) & 1; }
extern "C" __declspec(noinline) int cfCam_getBit13_4EC(void* self) { return (*reinterpret_cast<u32*>(reinterpret_cast<char*>(self) + 1260) >> 13) & 1; }
extern "C" __declspec(noinline) f32 cfCam_getFloat4FC(void* self) { return *reinterpret_cast<f32*>(reinterpret_cast<char*>(self) + 1276); }
extern "C" __declspec(noinline) f32 cfCam_getFloat4F8(void* self) { return *reinterpret_cast<f32*>(reinterpret_cast<char*>(self) + 1272); }
extern "C" __declspec(noinline) f32 cfCam_getFloat508(void* self) { return *reinterpret_cast<f32*>(reinterpret_cast<char*>(self) + 1288); }
// More tiny accessors (retail thunks, second batch).
extern "C" __declspec(noinline) CfCamEventManager* cfCam_getEventMgr() { return reinterpret_cast<CfCamEventManager*>(lbl_eu_80663DF0); }
__declspec(noinline) int cfCam_getE28Bit24() { return (lbl_eu_80663E28 >> 24) & 1; }
extern "C" __declspec(noinline) void* cfCam_getPlus3D8(void* self) { return static_cast<char*>(self) + 984; }
extern "C" __declspec(noinline) int cfCam_getBit9_04(void* self) { return (*reinterpret_cast<u32*>(reinterpret_cast<char*>(self) + 4) >> 9) & 1; }
__declspec(noinline) int cfCam_getInt90E4(int self) { return *reinterpret_cast<int*>(self + 0x90E4); }
// Retail cfCam_setClear1D4: set/clear the 0x1D4 flag word by flag argument.
__declspec(noinline) void cfCam_setClear1D4(cf::CfCamFollow* self, u32 mask, int flag) {
    u32* word = reinterpret_cast<u32*>(reinterpret_cast<char*>(self) + 0x1D4);
    if (flag != 0)
        *word |= mask;
    else
        *word &= ~mask;
}
extern "C" __declspec(noinline) void* cfCam_loadUnkC4(void* self) { return *reinterpret_cast<void**>(reinterpret_cast<char*>(self) + 196); }
extern "C" __declspec(noinline) int cfCam_testBit0530(void* self) { return *reinterpret_cast<unsigned short*>(reinterpret_cast<char*>(self) + 1328) & 1; }
extern "C" __declspec(noinline) int cfCam_testBitsC530(void* self) {
    unsigned short v = *reinterpret_cast<unsigned short*>(reinterpret_cast<char*>(self) + 1328);
    return (v & 0xC) != 0;
}
extern "C" __declspec(noinline) int cfCam_testBits30530(void* self) {
    unsigned short v = *reinterpret_cast<unsigned short*>(reinterpret_cast<char*>(self) + 1328);
    return (v & 0x30) != 0;
}
// Retail cfCam_maskUnk1D4: test the 0x1D4 flag word against a mask.
extern "C" __declspec(noinline) int cfCam_maskUnk1D4(int self, int mask) {
    return (*reinterpret_cast<u32*>(static_cast<char*>(reinterpret_cast<void*>(self)) + 0x1D4) & static_cast<u32>(mask)) != 0 ? 1 : 0;
}
// Retail cfCam_andcUnk1D4: clear bits of the 0x1D4 flag word.
__declspec(noinline) void cfCam_andcUnk1D4(cf::CfCamFollow* self, u32 mask) {
    *reinterpret_cast<u32*>(reinterpret_cast<char*>(self) + 0x1D4) &= ~mask;
}
// Retail cfCam_getBit12_04: flag bit of the +4 word.
extern "C" __declspec(noinline) int cfCam_getBit12_04(void* self) {
    return (*reinterpret_cast<u32*>(reinterpret_cast<char*>(self) + 4) >> 12) & 1;
}
// Retail cfCam_clearBits1200: clear the 0x1200-bit pair in the +4 word.
extern "C" __declspec(noinline) void cfCam_clearBits1200(void* self) {
    *reinterpret_cast<u32*>(reinterpret_cast<char*>(self) + 4) &= 0xFFFFEDFFu;
}
// Retail cfCam_copyBlock16: copy 16 bytes.
// Retail cfCam_getActiveObj: tail-call the event-manager check with flag 0.
cf::CfObject* cfCam_getActiveObj(CfCamEventManager* mgr) {
    return reinterpret_cast<cf::CfObject*>(CamEvtFetchSlotPtr(mgr, 0));
}
// Retail cfCam_testMask4EC: boolean-test the +0x4EC flag word against a mask.
extern "C" __declspec(noinline) int cfCam_testMask4EC(void* x, u32 mask) {
    return (*reinterpret_cast<u32*>(reinterpret_cast<char*>(x) + 1260) & mask) != 0 ? 1 : 0;
}
// Retail cfCam_testUnk04M: boolean-test the +4 flag word against a mask.
extern "C" __declspec(noinline) int cfCam_testUnk04M(int self, int mask) {
    return (*reinterpret_cast<u32*>(self + 4) & static_cast<u32>(mask)) != 0 ? 1 : 0;
}
// Retail cfCam_zero1D4Word: zero the 0x1D4 flag word.
__declspec(noinline) void cfCam_zero1D4Word(cf::CfCamFollow* self) {
    *reinterpret_cast<u32*>(reinterpret_cast<char*>(self) + 0x1D4) = 0;
}
// Retail cfCam_refreshBA8: refresh lbl_eu_80661BA8 from the 0x80527230 table
// using Class_80296898 selection byte 4.
extern "C" __declspec(noinline) void cfCam_refreshBA8() {
    const unsigned char* inst = reinterpret_cast<const unsigned char*>(Class_80296898::getInstance());
    unsigned char idx = inst[4];
    lbl_eu_80661BA8 = lbl_eu_80527230[idx];
}
// Retail cfCam_testPad100: tail-call the pad gate with mask 0x100.
extern "C" __declspec(noinline) int cfCam_testPad100(void* self) { return cfCam_testUnk04M(reinterpret_cast<int>(self), 0x100); }
// Retail cfCam_mulSda2Const: multiply by the .sda2 constant at lbl_eu_8066A20C.
__declspec(noinline) f32 cfCam_mulSda2Const(f32 x) { extern float lbl_eu_8066A20C; return x * lbl_eu_8066A20C; }
// Retail cfCam_setBit9_04: set bit 0x200 in the +4 word.
extern "C" __declspec(noinline) void cfCam_setBit9_04(void* self) {
    *reinterpret_cast<u32*>(reinterpret_cast<char*>(self) + 4) |= 0x200;
}
// Retail cfCam_nopVirtFloat: empty body.
extern "C" __declspec(noinline) void cfCam_nopVirtFloat(void* self, float f) { (void)self; (void)f; }
// Retail cfCam_testWordMask: test the word at +0 against a mask.
__declspec(noinline) int cfCam_testWordMask(void* obj, u32 mask) {
    return (*reinterpret_cast<u32*>(obj) & mask) != 0 ? 1 : 0;
}
// Retail cfCam_setTransCol: fan a vec3 out to +0xC/+0x1C/+0x2C.
extern "C" __declspec(noinline) void cfCam_setTransCol(float* mtx, const float* v) {
    mtx[3] = v[0];
    mtx[7] = v[1];
    mtx[11] = v[2];
}
__declspec(noinline) void cfCam_copyBlock16(void* dst, void* src) {
    const u32* s = static_cast<const u32*>(src);
    u32* d = static_cast<u32*>(dst);
    d[0] = s[0];
    d[1] = s[1];
    d[2] = s[2];
    d[3] = s[3];
}
__declspec(noinline) int cfCam_getBit27_64(void* self) { return (*reinterpret_cast<u32*>(reinterpret_cast<char*>(self) + 100) >> 27) & 1; }
__declspec(noinline) void cfCam_copyWordA(void* dst, void* src) { *static_cast<u32*>(dst) = *static_cast<u32*>(src); }
__declspec(noinline) void cfCam_copyWordB(void* dst, void* src) { *static_cast<u32*>(dst) = *static_cast<u32*>(src); }
__declspec(noinline) void cfCam_copyHalf(void* dst, void* src) { *static_cast<unsigned short*>(dst) = *static_cast<unsigned short*>(src); }
// cfCam_updFollowPos: follow-cam position update. When the follow target handle
// (+0x164) reports 0 from its vtable slot 0x74, re-arm the 0x40 camera flag.
// cfCam_getFollowSrc derives a source vector, cfCam_offsetFromDir transforms it, the
// result is fanned out to the +0x28/+0x34/+0x1C vec3 slots (copyVec3Words
// returns its dst, feeding the next copy), and the +0x20 heading float
// accumulates the +0x1DC follow distance.
void cfCam_updFollowPos(cf::CfCamFollow* self, void* arg2, float f1) {
    if (self->unk164 != 0) {
        if (self->unk164->CfObject_isMoveActiveNow() == 0) {
            cfCam_setClear04(self, 0x40, 1);
        }
    }
    ml::CVec3 local2;
    ml::CVec3 local1;
    cfCam_getFollowSrc(&local1, self);
    cfCam_offsetFromDir(&local2, self, &local1, arg2, f1);
    copyVec3Words(self->unk1C,
                  copyVec3Words(self->unk1C + 24, copyVec3Words(self->unk1C + 12, &local2)));
    *(reinterpret_cast<f32*>(self->unk1C) + 1) += self->unk1DC;
}
// cfCam_getFollowSrc: follow-camera source-vector derivation. When the follow
// target handle at +0x164 is present (and passes the lookupWorkAtAddr /
// func_800BBC0C liveness probes) the vtable-slot-0xAC aim vector is copied
// into local_2c (overridden by self+0x1BC when the 0x2000 flag is set and
// cfCam_loadUnkC4 reports a child), then offset by lbl_eu_80570A2C. A probe
// point is built 0x806662F8 ahead and collision-tested (mask 0x4); on a hit
// a height clamp (lbl_eu_8066631C) lifts the camera and the 0x800 flag is
// set, else a second probe at +0x80666320 runs when the child exists. The
// 0x2 flag is set with the hit bit and the result vector is copied out.
void cfCam_getFollowSrc(ml::CVec3* out, cf::CfCamFollow* self) {
    if (self->unk164 != 0) {
        if (lookupWorkAtAddr(self->unk164) == 0) {
            self->unk164 = 0;
            copyVec3f(out, reinterpret_cast<const ml::CVec3*>(self->unk10));
            return;
        }
        if (func_800BBC0C(self->unk164) == 0) {
            self->unk164 = 0;
            copyVec3f(out, reinterpret_cast<const ml::CVec3*>(self->unk10));
            return;
        }
        ml::CVec3 local_2c;
    ml::CVec3 local_20;
    ml::CVec3 local_14;
    ml::CVec3 local_8;
    copyVec3f(&local_2c, self->unk164->CfObject_getPosVector());
    void* f = cfCam_loadUnkC4(self->unk164);
    if (f != 0 && cfCam_maskUnk1D4(reinterpret_cast<int>(self), 0x2000) != 0) {
        copyVec3Words(&local_2c, self->unk168 + 0x54);
    }
    addVec3f(reinterpret_cast<float*>(&local_2c), lbl_eu_80570A2C);
    int flag = 0;
    noopAnimVec3(&local_20);
    noopAnimVec3(&local_14);
    copyVec3f(&local_8, &local_2c);
    local_8.y += lbl_eu_806662F8;
    if (ScnRes_SegQueryForward_E348(self->unk164->CfObject_getPosVector(), &local_8, 0,
                      0x40000, 0) != 0) {
        func_804BE4B4(&local_20, 0);
        f32 h = lbl_eu_8066631C + local_20.y;
        if (local_2c.y < h) {
            local_2c.y = h;
            if (getNullPtrC__Q22cf13CfGameManagerFv(8) != 0) {
                CmText_VariadicNoop(0x140, 0x70, lbl_eu_804FB4F0 + 0x15);
            }
            cfCam_setClear1D4(self, 0x8000000, 1);
            flag = 1;
        }
    } else if (f != 0) {
        if (cfCam_getBit1_4EC(f) != 0 || cfCam_maskUnk1D4(reinterpret_cast<int>(self), 0x40000) != 0) {
            copyVec3Words(&local_8, self->unk164->CfObject_getPosVector());
            local_8.y += lbl_eu_80666320;
            if (ScnRes_SegQueryForward_E348(self->unk164->CfObject_getPosVector(), &local_8,
                              0, 0x40000, 0) != 0) {
                func_804BE4B4(&local_20, 0);
                f32 h = lbl_eu_8066631C + local_20.y;
                if (local_2c.y <= h) {
                    local_2c.y = h;
                    if (getNullPtrC__Q22cf13CfGameManagerFv(8) != 0) {
                        CmText_VariadicNoop(0x140, 0x70, lbl_eu_804FB4F0 + 0x21);
                    }
                    flag = 1;
                }
            }
        }
    }
        cfCam_setClear1D4(self, 0x20000, flag);
        copyVec3f(out, &local_2c);
    } else {
        copyVec3f(out, reinterpret_cast<const ml::CVec3*>(self->unk10));
    }
}
// cfCam_prepFollowVec: follow-cam vector prep. Reads the follow target handle at
// +0x164; when present, its vtable slot 0xCC supplies a float (else 0). arg4
// selects scaleByGlobal(lbl_eu_80661B4C) vs the shared camera-relative vec3's
// x; writeVec3f packs (s, f, 0) into a temp vec3 passed to cfCam_offsetFromDir
// along with the first three args.
void cfCam_prepFollowVec(void* arg1, cf::CfCamFollow* self, void* arg3, void* arg4) {
    f32 f;
    if (self->unk164 != 0) {
        f = self->unk164->CfObject_getMoveHeadAngle();
    } else {
        f = lbl_eu_806662DC;
    }
    f32 s;
    if (arg4 != 0) {
        s = scaleByGlobal(lbl_eu_80661B4C);
    } else {
        s = lbl_eu_80570A38[0];
    }
    f32 local[3];
    writeVec3f(local, s, f, lbl_eu_806662DC);
    cfCam_offsetFromDir(arg1, self, arg3, local, lbl_eu_80661B60);
}
void cfCam_seedFollowD() {
    lbl_eu_80663DE8 = lbl_eu_80661B60;
    lbl_eu_80663DE4 = lbl_eu_80661B60;
    f32 s = scaleByGlobal(lbl_eu_80661B4C);
    lbl_eu_80570A44[0] = s;
    lbl_eu_80570A38[0] = s;
}
void func_8006CC68(cf::CfCamFollow* self, int flag) {
    (void)self;
    (void)flag;
}
// noinline keeps the retail `bl func_8006CE24` call sites.
// Definition sits under the mangled name so callers use the flat extern-C
// view and keep the unmangled retail reloc.
//
// func_8006CE24: follow-camera per-frame update (retail 0x8006CE24). Picks a
// desired camera position based on the mode flags (cutscene source / stick
// aim / follow-target heading), then recomputes yaw/pitch state, blend speed
// and the decay/zoom flags from the resulting vectors.
__declspec(noinline) void
func_8006CE24__FPvPviiiii(cf::CfCamFollow* self, void* arg, int a, int b,
                          int c, int d, int e) {
    if (cfCam_getSignBit04(self) == 0) {
        return;
    }
    if (cfCam_testUnk04M(reinterpret_cast<int>(self), 0x800) != 0) {
        return;
    }
    // Declared in descending stack-address order so MWCC's reverse-order slot
    // assignment yields the retail frame layout. nw4r VEC3 (plain POD, size
    // 12) keeps MWCC's slot packing identical to the retail frame.
    nw4r::math::VEC3 lBC;
    nw4r::math::VEC3 lB0;
    nw4r::math::VEC3 lA4;
    nw4r::math::VEC3 l98;
    nw4r::math::VEC3 l8C;
    nw4r::math::VEC3 l80;
    nw4r::math::VEC3 l74;
    nw4r::math::VEC3 l68;
    nw4r::math::VEC3 l5C;
    nw4r::math::VEC3 l50;
    nw4r::math::VEC3 l44;
    nw4r::math::VEC3 l38;
    nw4r::math::VEC3 l2C;
    nw4r::math::VEC3 l20;
    // All scalar temps share one contiguous 24-byte block (retail stack
    // 0x8..0x20): [0],[1] = cfCam_updateHead outs, [2],[3] = cfCam_updateYaw
    // outs, [4] = e-branch BAF0 temp, [5] = c-branch BAF0 temp.
    // tmp packs the follow-flag and scene-pose pointer into one aggregate
    // so MWCC reserves a single aligned home for both.
    f32 sc[6];
    struct {
        void* pose;
        int flag;
    } tmp;
    copyVec3f(reinterpret_cast<ml::CVec3*>(&lBC),
                  static_cast<const ml::CVec3*>(arg));
    subVec3f(&lB0.x, reinterpret_cast<const f32*>(&self->field_0x28),
                  &lBC.x);
    f32 scale = lbl_eu_80661B60;
    noopAnimVec3(&lA4.x);
    tmp.flag = 0;
    if (e != 0) {
        // Scripted/cutscene camera: derive the position from the blended
        // heading pair snapshot and override the zoom scale with unk1FC.
        scale = self->unk1FC;
        writeVec3f(&l98.x, self->unk1F8, self->field_0x184, lbl_eu_806662DC);
        c = 1;
        cfCam_offsetFromDir(&l5C, self, arg, &l98, scale);
        copyVec3Words(&lA4, &l5C);
        lA4.x = self->field_0x28;
        lA4.z = self->field_0x30;
    } else if (a != 0 && d != 0) {
        // Stick-aim + follow-target: full heading update path.
        cfCam_prepFollowVec(&l50, self, arg, reinterpret_cast<void*>(1));
        copyVec3Words(&lA4, &l50);
        if (cfCam_testUnk04M(reinterpret_cast<int>(self), 0x4000) != 0) {
            if (cfCam_maskUnk1D4(reinterpret_cast<int>(self), 0x2000) != 0) {
                cfCam_updateHead(static_cast<void*>(self),
                              static_cast<const void*>(arg), &sc[0], &sc[1],
                              1, lbl_eu_80661B60);
                self->unk1F0 = sc[0];
                self->unk1F4 = wrapAnglePi(sc[1]);
                writeVec3f(&l8C.x, sc[0], sc[1], lbl_eu_806662DC);
                cfCam_offsetFromDir(&l44, self, arg, &l8C, self->unk23C);
                copyVec3Words(&lA4, &l44);
            } else {
                cfCam_updateYaw(static_cast<void*>(self),
                              static_cast<const void*>(arg), &sc[2], &sc[3], 1);
                self->unk1F0 = sc[2];
                self->unk1F4 = wrapAnglePi(sc[3]);
                writeVec3f(&l80.x, sc[2], sc[3], lbl_eu_806662DC);
                cfCam_offsetFromDir(&l38, self, arg, &l80, self->unk23C);
                copyVec3Words(&lA4, &l38);
            }
        }
    } else if (a != 0) {
        cfCam_prepFollowVec(&l2C, self, arg, reinterpret_cast<void*>(1));
        copyVec3Words(&lA4, &l2C);
        lA4.x = self->field_0x28;
        lA4.z = self->field_0x30;
        tmp.flag = 1;
    } else if (d != 0) {
        // Follow-target only: aim vector comes from the target's vtable
        // slot 0xCC scale accessor; fall back to zero when absent.
        writeVec3f(
            &l74.x, self->field_0x180,
            self->unk164 != 0
                ? self->unk164->CfObject_getMoveHeadAngle()
                : lbl_eu_806662DC,
            lbl_eu_806662DC);
        cfCam_offsetFromDir(&l20, self, arg, &l74, self->unk244);
        copyVec3Words(&lA4, &l20);
    }
    subVec3f(&l68.x, &lA4.x, &lBC.x);
    *reinterpret_cast<f32*>(self->unk168 + 0x00) =
        cfCam_pitchFromVec(static_cast<const void*>(&lB0));
    *reinterpret_cast<f32*>(self->unk168 + 0x04) =
        cfCam_yawFromVec(static_cast<const void*>(&lB0));
    *reinterpret_cast<f32*>(self->unk168 + 0x08) = lbl_eu_806662DC;
    *reinterpret_cast<f32*>(self->unk168 + 0x0C) =
        cfCam_pitchFromVec(static_cast<const void*>(&l68));
    *reinterpret_cast<f32*>(self->unk168 + 0x10) =
        cfCam_yawFromVec(static_cast<const void*>(&l68));
    *reinterpret_cast<f32*>(self->unk168 + 0x14) = lbl_eu_806662DC;
    // Scene pose lookup feeds the default follow distance.
    tmp.pose = Scn_FindCamItem(cfCam_loadPlus4(self->unk160), 0);
    if (tmp.pose != 0) {
        self->unk210 = cfCam_getFloat1E0F(tmp.pose);
    } else {
        self->unk210 = lbl_eu_80661B50;
    }
    VecMath_NormalizeInPlace(reinterpret_cast<ml::CVec3*>(&lB0));
    VecMath_NormalizeInPlace(reinterpret_cast<ml::CVec3*>(&l68));
    f32 dot = dotVec3f(&lB0.x, &l68.x);
    f32 t = cfCam_acosF32(
        static_cast<double>((lbl_eu_806662D0 + dot) * lbl_eu_80666304));
    t = cfCam_mulSda2Const(t);
    if (t != t) {
        t = lbl_eu_806662DC; // NaN guard
    }
    f32 blend =
        lbl_eu_80661B8C + t * (lbl_eu_80661B90 - lbl_eu_80661B8C) / lbl_eu_80666324;
    if (blend < lbl_eu_806662D0) {
        blend = lbl_eu_80661B8C * lbl_eu_80666304;
    }
    if (tmp.flag != 0) {
        blend = lbl_eu_80666328;
    } else if (cfCam_testUnk04M(reinterpret_cast<int>(self), 0x4000) != 0) {
        blend = lbl_eu_80666328 + t * lbl_eu_806662C4 / lbl_eu_80666324;
        if (blend < lbl_eu_806662D0) {
            blend = lbl_eu_80666328;
        }
    } else if (e != 0) {
        sc[4] = self->unk1F8 - self->field_0x180;
        t = cfCam_absFloat(&sc[4]);
        f32 ratio = t / scaleByGlobal(lbl_eu_80666328);
        blend = lbl_eu_80666328;
        if (ratio >= blend && ratio <= lbl_eu_806662BC) {
            blend = ratio;
        }
    }
    self->unk204 = lbl_eu_806662D0 / blend;
    if (t > lbl_eu_8066A208) {
        self->unk204 = lbl_eu_80666304;
    }
    if (b != 0) {
        if (cfCam_absFloat(&self->unk21C) > lbl_eu_8066A208) {
            cfCam_setClear04(self, 0x1000, 1);
        }
    }
    self->unk218 = lbl_eu_806662DC;
    if (c != 0) {
        self->unk208 = self->unk23C;
        self->unk20C = scale;
        sc[5] = self->unk23C - scale;
        if (cfCam_absFloat(&sc[5]) > lbl_eu_8066A208) {
            cfCam_setClear04(self, 0x400, 1);
            self->unk218 = self->unk23C - scale;
        }
    }
    self->unk200 = lbl_eu_806662DC;
    self->unk238 = lbl_eu_806662DC;
    cfCam_setBit9_04(self);
}
extern "C" __declspec(noinline) void
func_8006CE24(void* self, void* arg, int a, int b, int c, int d, int e);
// cfCam_pitchFromVec: spherical azimuth - atan2(y, sqrt(x^2+z^2)) scaled by the
// same fixed FIdx factor as cfCam_yawFromVec. MWCC computes the fmadds addend
// (z*z) first, so the sum is written x*x + z*z to get the retail load order
// (z at +8 before x at +0).
float cfCam_pitchFromVec(const ml::CVec3* v) {
    return nw4r::math::Atan2FIdx(v->y, (float)sqrt((double)(v->x * v->x + v->z * v->z))) *
           lbl_eu_8066632C;
}
// cfCam_yawFromVec: yaw from the x/z components of a vector, scaled by a fixed
// lookup-index factor (nw4r Atan2FIdx returns FIdx units). z is read first so
// MWCC colors it to f2 and loads it before f1 (retail load order). Defined
// under the C++-mangled name so callers (extern "C" decl in CfCam.hpp, no
// in-TU body) keep a real `bl cfCam_yawFromVec` instead of an inline.
float cfCam_yawFromVec(const ml::CVec3* v) {
    float z = v->z;
    float x = v->x;
    return nw4r::math::Atan2FIdx(x, z) * lbl_eu_8066632C;
}
// cfCam_acosF32: acos wrapper; retail converts the double result back to f32.
// Defined with a float param; callers go through the body-less double overload
// below so MWCC keeps the retail bl instead of inlining.
float cfCam_acosF32(float x) {
    return (float)acos(x);
}
// cfCam_recomputeEyeTarget: follow-camera state recompute from a new eye/target pair.
// Builds two direction vectors (target-eye through the normalized mid vector
// and the follow-cam source), stores the yaw/pitch/magnitude fields, then
// derives the angular speed (unk204) and resets the 0x800 flag.
void cfCam_recomputeEyeTarget(cf::CfCamFollow* self, void* argA, void* argB, void* argC,
                   int modeA, int modeB, float dist) {
    if (cfCam_getSignBit04(self) == 0) {
        return;
    }
    // Declared so MWCC's reverse-order stack slot assignment yields the
    // retail layout l8 < l14 < l20 < l2c < l38 < l44.
    ml::CVec3 l44;
    ml::CVec3 l38;
    ml::CVec3 l2c;
    ml::CVec3 l20;
    ml::CVec3 l14;
    ml::CVec3 l8;
    subVec3f(&l44.x, reinterpret_cast<const f32*>(argA),
                  reinterpret_cast<const f32*>(argB));
    lbl_eu_80570A38[0] = *reinterpret_cast<const f32*>(self->unk168 + 0x18);
    cfCam_getFollowSrc(&l38, self);
    const f32* mid;
    if (argC != 0) {
        // (CfCamFollow* self keeps the in-TU def's signature.)
        cfCam_prepFollowVec(static_cast<void*>(&l14.x), self,
                      static_cast<void*>(&l38.x), static_cast<void*>(0));
        mid = &l14.x;
    } else {
        cfCam_buildOffset(&l8, self, &l38,
                      reinterpret_cast<const f32*>(self->unk168 + 0x18),
                      self->unk244);
        mid = &l8.x;
    }
    copyVec3f(&l2c, reinterpret_cast<const ml::CVec3*>(mid));
    subVec3f(&l20.x, &l2c.x, &l38.x);
    copyVec3Words(self->unk168 + 0x30, argB);
    if (modeA == 0) {
        copyVec3Words(&l44, &l20);
        copyVec3Words(self->unk168 + 0x30, &l38);
    }
    // Store each result immediately: retail keeps no float live across these
    // calls except dist (f31), so batching temporaries would force extra
    // callee-saved fpr saves.
    *reinterpret_cast<f32*>(self->unk168 + 0x00) = cfCam_pitchFromVec(static_cast<const void*>(&l44));
    *reinterpret_cast<f32*>(self->unk168 + 0x04) = cfCam_yawFromVec(static_cast<const void*>(&l44));
    *reinterpret_cast<f32*>(self->unk168 + 0x08) = lbl_eu_806662DC;
    *reinterpret_cast<f32*>(self->unk168 + 0x0C) = cfCam_pitchFromVec(static_cast<const void*>(&l20));
    *reinterpret_cast<f32*>(self->unk168 + 0x10) = cfCam_yawFromVec(static_cast<const void*>(&l20));
    *reinterpret_cast<f32*>(self->unk168 + 0x14) = lbl_eu_806662DC;
    self->unk208 = cfCam_vecLength(static_cast<const void*>(&l44));
    self->unk20C = cfCam_vecLength(static_cast<const void*>(&l20));
    self->unk210 = dist;
    VecMath_NormalizeInPlace(&l44);
    VecMath_NormalizeInPlace(&l20);
    f32 dot = dotVec3f(&l44.x, &l20.x);
    f32 t = cfCam_acosF32(static_cast<double>((lbl_eu_806662D0 + dot) * lbl_eu_80666304));
    t = cfCam_mulSda2Const(t);
    if (t != t) {
        t = lbl_eu_806662DC; // NaN guard
    }
    f32 blend =
        lbl_eu_80661B8C + t * (lbl_eu_80661B90 - lbl_eu_80661B8C) / lbl_eu_80666324;
    if (blend < lbl_eu_806662D0) {
        blend = lbl_eu_80661B8C;
    }
    f32 denom = blend;
    if (modeB != 0) {
        denom = static_cast<f32>(modeB); // s32->f32 idiom emits retail xoris path
    }
    if (modeA == 0) {
        denom = lbl_eu_806662D0;
    }
    self->unk204 = lbl_eu_806662D0 / denom;
    if (modeA == 0) {
        self->unk204 = lbl_eu_806662D0;
    } else if (t <= lbl_eu_8066A208) {
        self->unk204 = lbl_eu_80666304;
    }
    self->unk200 = lbl_eu_806662DC;
    self->unk238 = lbl_eu_806662DC;
    cfCam_setClear04(self, 0x800, 1);
}
// cfCam_multMtxVec: transform src by mtx into a stack temp, then copy the result
// out component-wise (lfs/stfs triple, f0 reused).
void cfCam_multMtxVec(Vec* dst, const Mtx mtx, const Vec* src) {
    Vec tmp;
    PSMTXMultVec(mtx, src, &tmp);
    dst->x = tmp.x;
    dst->y = tmp.y;
    dst->z = tmp.z;
}
// cfCam_padAbsent: remote-pad availability query; forwards the controller id to
// the pad-state check and returns whether it reported "not present" (== 0).
extern "C" __declspec(noinline) bool cfCam_padAbsent(int controllerId) {
    return cfCam_testUnk04M(controllerId, 3) == 0;
}
// cfCam_initFollowS: init the follow-cam defaults (cfCam_resetHist), then fan the
// source vec3 out to the +0x40 / +0x4C / +0x58 slots. cfCam_resetHist itself is
// declared extern (no in-TU body) so MWCC keeps the bl instead of inlining.
extern "C" void cfCam_initFollowS(cf::CfCamFollow* self, ml::CVec3* src) {
    cfCam_resetHist(self, src);
    copyVec3Words(self->unk1C + 0x30, src);
    copyVec3Words(self->unk1C + 0x24, src);
    copyVec3Words(self->unk1C + 0x3C, src);
}
// cfCam_resetHist: reset the follow-cam slots - zero the +0x250 halfword, copy
// src into all 16 stride-12 vec3 slots at +0x70 (element base via
// cfCam_vec3AtIndex, which is only declared here so the loop keeps its bl), and
// set the +0x22C/+0x230 distances to 1.0.
void cfCam_resetHist(cf::CfCamFollow* self, u8* src) {
    self->unk250 = 0;
    for (unsigned int i = 0; i < 16; i++) {
        copyVec3Words(cfCam_vec3AtIndex(self->unk70, i), src);
    }
    self->unk22C = lbl_eu_806662D0;
    self->unk230 = lbl_eu_806662D0;
}
// cfCam_syncSelFlags: follow-cam flag sync from the Class_80296898 selection
// bytes. Bytes 0/1 toggle the 0x8/0x4 flags; byte 6 clears the 0x40000 flag
// and zeroes the +0x21C/+0x220 distances. When the 0x80663DE0 gate is clear,
// refresh the shared tables (cfCam_loadTuneTbl) and - if selection byte 5 differs
// from the 0x80661BA4 state - run the state routine with flag 1.
__declspec(noinline) void cfCam_syncSelFlags(cf::CfCamFollow* self) {
    const u8* inst = reinterpret_cast<const u8*>(Class_80296898::getInstance());
    cfCam_setClear04(self, 8, inst[0] != 0);
    cfCam_setClear04(self, 4, inst[1] != 0);
    cfCam_setClear04(self, 0x40000, inst[6] == 0);
    if (inst[6] == 0) {
        self->unk21C = lbl_eu_806662DC;
        self->unk220 = lbl_eu_806662DC;
    }
    if (lbl_eu_80663DE0 == 0) {
        cfCam_loadTuneTbl();
        cfCam_refreshBA8();
        if ((unsigned int)inst[5] != (unsigned int)lbl_eu_80661BA4) {
            func_8006B720(inst[5], 1);
        }
    }
}
// func_8006D8D0: follow-camera flag sync from the target object at `arg`.
// func_800BBC0C(arg) yields the object; when cfCam_loadUnkC4 reports a child,
// a battery of func_8006DC*/probe results is fanned into the 0x8 / 0x1 / 0x2
// / 0x4 / 0x80 / 0x40 / 0x200 / 0x20 / 0x8000 / 0x10000 flag bits. The
// +0x18C vec3 magnitude below lbl_eu_8066629C decays +0x238 (toward 0) or
// resets it to lbl_eu_80666334; the +0x80000 flag tracks +0x238 > 0. The
// cfCam_getFloat4FC/DC84/DC8C probes decide the 0x40000 flag and the
// cfCam_getBit13_4EC probe feeds the 0x100000 bit.
//
// Regalloc residual: retail colors obj->r31, child->r30, magFlag/f2/f3->r31;
// our build recycles r31 for the child instead of taking virgin r30 (and the
// flag group into r30), leaving a pure 27-instruction r30<->r31 swap with
// structural=0 and clean relocs. Ruled out: decl-order permutations,
// declaration-at-top vs block scope, init-at-decl x=0, assignment-in-guard,
// nested-if temp form, typed (cf::CfObject*) obj, `register`/`const` quals,
// merged actor+child variable (live-range split), ternary flag arg,
// early-return guard form (breaks branch polarity), identical-arms ternary
// keep-alive, clamp `>` vs `<=` polarity (the latter
// reschedules the whole tail; `<=` + `<` posFlag is the byte-matching shape).
__declspec(noinline) void func_8006D8D0(cf::CfCamFollow* self, void* arg) {
    // CfGameManager.hpp declares CfT_PlayRateGet as void, but the retail call
    // consumes its f1 return (CfTFile.cpp defines it as `float`) - call it
    // through a float-returning cast at the use site.
    void* obj = func_800BBC0C(arg);
    int flag;
    void* x;
    int magFlag;
    void* a = func_8016FE34(obj);
    if (a != 0) {
        flag = cfCam_queryVoxArts(static_cast<cf::CVoiceRec*>(a), 4);
    } else {
        flag = 0;
    }
    cfCam_setClear1D4(self, 0x10, flag);
    if (obj != 0 && cfCam_loadUnkC4(obj) != 0) {
        x = cfCam_loadUnkC4(obj);
        if (cfCam_getBit26_4EC(x) != 0) {
            if (cfCam_maskUnk1D4(reinterpret_cast<int>(self), 1) == 0) {
                self->unk224 = lbl_eu_80666330;
            }
        }
        cfCam_maskUnk1D4(reinterpret_cast<int>(self), 0x10000);
        cfCam_setClear1D4(self, 8, cfCam_getBit24_4EC(x));
        cfCam_setClear1D4(self, 1, cfCam_getBit26_4EC(x));
        cfCam_setClear1D4(self, 2, cfCam_testMask4EC(x, 0x40000));
        cfCam_setClear1D4(self, 4, cfCam_testMask4EC(x, 0x20000));
        cfCam_setClear1D4(self, 0x80, testAnimMoveBit(x));
        cfCam_setClear1D4(self, 0x40, cfCam_getBit1_4EC(x));
        cfCam_setClear1D4(self, 0x200, cfCam_getBit25_4EC(x));
        cfCam_setClear1D4(self, 0x20, cfCam_getBit23_4EC(x));
        cfCam_setClear1D4(self, 0x8000, cfCam_testMask4EC(x, 0x400000));
        cfCam_setClear1D4(self, 0x10000, cfCam_testMask4EC(x, 0x40000000));
        magFlag = cfCam_vecLength(&self->unk168[0x24]) < lbl_eu_8066629C;
        cfCam_setClear1D4(self, 0x100, magFlag);
        if (magFlag != 0) {
            self->unk238 -= ((float (*)())CfT_PlayRateGet)();
            if (self->unk238 <= lbl_eu_806662DC) {
                self->unk238 = lbl_eu_806662DC;
            }
        } else {
            self->unk238 = lbl_eu_80666334;
        }
        cfCam_setClear1D4(self, 0x80000, self->unk238 < lbl_eu_806662DC);
        int f2 = 0;
        f32 c = cfCam_getFloat4FC(x);
        if (cfCam_absFloat(&c) > lbl_eu_8066A208) {
            f32 b = cfCam_getFloat4F8(x);
            if (cfCam_absFloat(&b) > lbl_eu_8066A208) {
                f2 = 1;
            }
        }
        int f3 = 0;
        if (f2 != 0) {
            f32 d8c = cfCam_getFloat508(x);
            if (cfCam_getFloat4F8(x) - d8c >= lbl_eu_80666338) {
                f3 = 1;
            }
        }
        cfCam_setClear1D4(self, 0x40000, f3);
        cfCam_setClear1D4(self, 0x100000, cfCam_getBit13_4EC(x));
    }
}
// cfCam_queryVoxArts: query the +0x4 sub-object's slot-0x30 state word (owner view:
// CVoiceSub30/CVoiceRec in kyoshin/help/CHelp_Talk.hpp - same shape as
// CHelp_Talk::func_802B86F0) and feed it to the func_80174C98 arts-state
// gate. noinline keeps the retail `bl cfCam_queryVoxArts` at call sites.
extern "C" __declspec(noinline) int cfCam_queryVoxArts(cf::CVoiceRec* self, int flags) {
    u32 val = *self->field_04->vf30();
    return func_80174C98(self, &val, flags);
}
// cfCam_artsGateClr: follow-target arts-state gate. Resolves the actor state via
// func_8016FE34, requires the CfGameManager gate, and returns true only when
// NONE of the 3/4/1 state flags and ALL... i.e. the 8 flag probe reports clear.
extern "C" __declspec(noinline) int cfCam_artsGateClr(void* obj) {
    cf::CVoiceRec* a = static_cast<cf::CVoiceRec*>(func_8016FE34(obj));
    if (a == 0) {
        return 0;
    }
    if (isTimerActive__Q22cf13CfGameManagerFv() == 0) {
        return 0;
    }
    if (cfCam_queryVoxArts(a, 3) != 0) {
        return 0;
    }
    if (cfCam_queryVoxArts(a, 4) != 0) {
        return 0;
    }
    if (cfCam_queryVoxArts(a, 1) != 0) {
        return 0;
    }
    return cfCam_queryVoxArts(a, 8) == 0;
}
// func_8006DD58: follow-cam per-frame update driver. Resolves the follow
// target handle (+0x164) into its manager block (base = target - 0x3E9C) when
// the gate probes pass, then runs the actor-param heading selection and
// refreshes the +0x1C8 camera vector toward the selected angle. Finally
// applies the 0x40-flag magnitude decay/clear and clears the 0x1000 flag.
__declspec(noinline) void func_8006DD58(cf::CfCamFollow* self, void* arg) {
    (void)arg;
    u8* base = 0;
    // Retail re-reads the +0x164 handle from memory at each probe.
    if (self->unk164 != 0) {
        if (cfCam_bit1At0x64(self->unk164) || cfCam_getBit2_64(self->unk164)) {
            // field re-read after the probes; gate keeps the raw pointer
                base = reinterpret_cast<u8*>(self->unk164);
            if (base != 0) {
                base -= 0x3E9C;
            }
        }
    }
    cfCam_setClear1D4(self, 0x20000000, 0);
    const u8* inst = reinterpret_cast<const u8*>(Class_80296898::getInstance());
    if (inst[7] != 0) {
        // game-manager probe via vtable slot 0x60; its result is the
        // follow-target handle fed to the arts-state gate
        if (cfCam_artsGateClr(self->cfCam_loadUnk164()) != 0 &&
            base != 0 &&
            testResInfoFlag(0x80000) == 0 &&
            cfCam_testUnk04M(reinterpret_cast<int>(self), 0x20000) == 0 &&
            Selector_GetInstance() != 0) {
            Selector_GetInstance();
            int sel = findObjectById__Fi(cfCam_getInt90E4(Selector_GetInstance()));
            // angle lands in tmp2c.y (retail stores it at tmp+4)
            ml::CVec3 tmp2c;
            if (sel != 0) {
                int flag = 1;
                noopAnimVec3(&tmp2c);
                cfCam_zeroVec3(&tmp2c);
                void* actor = func_8016FE34(reinterpret_cast<void*>(sel));
                if (actor != 0) {
                    switch (CActorParam_UnkVirtualFunc22__Q22cf11CActorParamFv(actor)) {
                    case 3:
                        tmp2c.y = lbl_eu_806662E0;
                        flag = 0;
                        break;
                    case 4:
                    case 5:
                        tmp2c.y = lbl_eu_806662D8;
                        break;
                    case 6:
                        tmp2c.y = lbl_eu_806662D0;
                        break;
                    default:
                        flag = 0;
                        break;
                    }
                    if (cfCam_getBit27_64(static_cast<u8*>(actor) + 0x3E9C)) {
                        tmp2c.y += lbl_eu_80666304;
                    }
                    cfCam_setClear1D4(self, 0x2000, flag);
                }
            }
            f32 sp8[3];
            f32 sp14[3];
            f32 sp20[3];
            subVec3f(sp8, &tmp2c.x,
                          reinterpret_cast<const f32*>(self->unk168 + 0x60));
            scaleVec3f(sp14, sp8, lbl_eu_806662F8);
            cfCam_psAddVec3(reinterpret_cast<nw4r::math::VEC3*>(sp20),
                          reinterpret_cast<const nw4r::math::VEC3*>(
                              self->unk168 + 0x60),
                          reinterpret_cast<const nw4r::math::VEC3*>(sp14));
            copyVec3Words(self->unk168 + 0x60, sp20);
        }
    }
    if (cfCam_testUnk04M(reinterpret_cast<int>(self), 0x400000) == 0) {
        cfCam_psScaleVec(reinterpret_cast<nw4r::math::VEC3*>(self->unk168 + 0x60),
                      lbl_eu_80666318);
        f32 lenSq = cfCam_vecLenSq(
            reinterpret_cast<const nw4r::math::VEC3*>(self->unk168 + 0x60));
        if (lenSq < lbl_eu_8066633C) {
            cfCam_zeroVec3(self->unk168 + 0x60);
        }
        cfCam_setClear1D4(self, 0x10000000, 0);
    }
}
// cfCam_updateHead: follow-camera heading/distance update. Two direction vectors
// are derived (arg2 - (self+0x28), arg2 - (self+0x10)); out1 gets the
// sin-dist product (atan2AnimFIdx of 8.0 * SinFIdx(lbl_eu_80663DC8) with the
// distance float); out2 gets pi + yaw(tmp2) - or pi + yaw(tmp2) - yaw(tmp1)
// when flag == 0 - offset by lbl_eu_80663DCC and wrapped by wrapAnglePi.
__declspec(noinline) void cfCam_updateHead(cf::CfCamFollow* self, const ml::CVec3* arg2, float* out1,
                   float* out2, int flag, float dist) {
    ml::CVec3 tmp1;
    ml::CVec3 tmp2;
    subVec3f(&tmp1.x, &arg2->x, reinterpret_cast<const f32*>(&self->unk1C[12]));
    subVec3f(&tmp2.x, &arg2->x, reinterpret_cast<const f32*>(self->unk10));
    *out1 = atan2AnimFIdx(lbl_eu_806662B8 * sinAnimFIdx(lbl_eu_80663DC8), dist);
    if (flag != 0) {
        *out2 = lbl_eu_8066A1F8 + cfCam_yawFromVec(static_cast<const void*>(&tmp2));
    } else {
        f32 yaw1 = cfCam_yawFromVec(static_cast<const void*>(&tmp1));
        *out2 = lbl_eu_8066A1F8 + (cfCam_yawFromVec(static_cast<const void*>(&tmp2)) - yaw1);
    }
    *out2 += lbl_eu_80663DCC;
    *out2 = wrapAnglePi(*out2);
}
// cfCam_updateYaw: follow-camera heading update. Two direction vectors are
// derived (arg2 minus the +0x28 / +0x10 positions); out1 gets the +0x1F8
// distance; out2 gets pi plus the yaw of v1 (or pi + yaw(v1) - yaw(v2) when
// flag == 0), wrapped into [-pi, pi) by wrapAnglePi.
void cfCam_updateYaw(cf::CfCamFollow* self, const ml::CVec3* arg2, float* out1,
                   float* out2, int flag) {
    ml::CVec3 v2;
    ml::CVec3 v1;
    subVec3f(&v2.x, &arg2->x, reinterpret_cast<const f32*>(&self->unk1C[12]));
    subVec3f(&v1.x, &arg2->x, reinterpret_cast<const f32*>(&self->unk10[0]));
    *out1 = self->unk1F8;
    if (flag != 0) {
        *out2 = lbl_eu_8066A1F8 + cfCam_yawFromVec(static_cast<const void*>(&v1));
    } else {
        *out2 = lbl_eu_8066A1F8 + (cfCam_yawFromVec(static_cast<const void*>(&v1)) -
                                   cfCam_yawFromVec(static_cast<const void*>(&v2)));
    }
    *out2 = wrapAnglePi(*out2);
}
// cfCam_maintainFlg: follow-flag maintenance driven by the config singleton run
// byte (+7). While running: claims the 0x4000 flag when absent (clearing stale
// 0x800000 / 0x8000 bits and snapshotting +0x180/+0x23C into +0x1F8/+0x1FC on
// first entry), and syncs the 0x400000 bit against the 0x20000000 follow flag,
// storing the float arg into +0x1F0 while engaged. Returns whether the
// manager is running.
int cfCam_maintainFlg(cf::CfCamFollow* self, float val) {
    const unsigned char* inst =
        reinterpret_cast<const unsigned char*>(Class_80296898::getInstance());
    if (inst[7] != 0) {
        if (cfCam_testUnk04M(reinterpret_cast<int>(self), 0x4000) == 0) {
            cfCam_setClear04(self, 0x4000, 1);
            if (cfCam_testUnk04M(reinterpret_cast<int>(self), 0x800000) != 0) {
                cfCam_andcUnk04(self, 0x800000);
            }
            // result deliberately discarded in retail
            cfCam_maskUnk1D4(reinterpret_cast<int>(self), 0x200000);
            if (cfCam_testUnk04M(reinterpret_cast<int>(self), 0x8000) != 0) {
                cfCam_andcUnk04(self, 0x8000);
            }
            if (self->unk256 == 0) {
                self->unk1F8 = self->field_0x180;
                self->unk1FC = self->unk23C;
            }
            self->unk256 = 8;
        }
        if (cfCam_testUnk04M(reinterpret_cast<int>(self), 0x400000) == 0) {
            if (cfCam_maskUnk1D4(reinterpret_cast<int>(self), 0x20000000) != 0) {
                cfCam_setClear04(self, 0x400000, 1);
                self->unk1F0 = val;
                self->unk256 = 8;
            }
        } else if (cfCam_maskUnk1D4(reinterpret_cast<int>(self), 0x20000000) == 0) {
            cfCam_andcUnk04(self, 0x400000);
        }
    }
    return inst[7] != 0 ? 1 : 0;
}
// func_8006E2FC: copy a saved camera-state block (or id's object) into the
// active camera-state buffer. Gates on the manager and the active object,
// then fans the vec3 slots, the 9-word +0x114 block, the sub-object copies
// and the scalar tail across, finally setting the source's 0x20 flag.
void func_8006E2FC(int id) {
    u8* dst = reinterpret_cast<u8*>(id);
    if (id == 0) {
        dst = lbl_eu_80570A8C;
    }
    CfCamEventManager* mgr = cfCam_getEventMgr();
    if (mgr == 0) {
        return;
    }
    u8* src = reinterpret_cast<u8*>(cfCam_getActiveObj(mgr));
    if (src == 0) {
        return;
    }
    copyVec3Words(dst + 0x00, src + 0x10);
    copyVec3Words(dst + 0x0C, src + 0x1C);
    copyVec3Words(dst + 0x18, src + 0x28);
    copyVec3Words(dst + 0x24, src + 0x34);
    copyVec3Words(dst + 0x30, src + 0x40);
    copyVec3Words(dst + 0x3C, src + 0x4C);
    copyVec3Words(dst + 0x48, src + 0x58);
    for (int i = 0; i < cfCam_histCount(src + 0x70); i++) {
        copyVec3Words(cfCam_vec3AtIndex(dst + 0x54, i), cfCam_vec3AtIndex(src + 0x70, i));
    }
    copyVec3Words(dst + 0x114, src + 0x168);
    copyVec3Words(dst + 0x120, src + 0x174);
    copyVec3Words(dst + 0x12C, src + 0x180);
    copyVec3Words(dst + 0x138, src + 0x18C);
    copyVec3Words(dst + 0x144, src + 0x198);
    copyVec3Words(dst + 0x150, src + 0x1A4);
    copyVec3Words(dst + 0x15C, src + 0x1B0);
    copyVec3Words(dst + 0x168, src + 0x1BC);
    copyVec3Words(dst + 0x174, src + 0x1C8);
    cfCam_copyWordA(dst + 0x180, src + 0x04);
    cfCam_copyWordB(dst + 0x184, src + 0x1D4);
    cfCam_copyHalf(dst + 0x188, src + 0x1D8);
    *reinterpret_cast<f32*>(dst + 0x18C) = *(const f32*)(src + 0x1DC);
    cfCam_setClear1D4(reinterpret_cast<cf::CfCamFollow*>(src), 0x20, 1);
    *reinterpret_cast<f32*>(dst + 0x190) = *(const f32*)(src + 0x1E0);
    *reinterpret_cast<f32*>(dst + 0x194) = *(const f32*)(src + 0x1E4);
    *reinterpret_cast<f32*>(dst + 0x198) = *(const f32*)(src + 0x1E8);
    *reinterpret_cast<f32*>(dst + 0x19C) = *(const f32*)(src + 0x1EC);
    *reinterpret_cast<f32*>(dst + 0x1A0) = *(const f32*)(src + 0x1F0);
    *reinterpret_cast<f32*>(dst + 0x1A4) = *(const f32*)(src + 0x1F4);
    *reinterpret_cast<f32*>(dst + 0x1A8) = *(const f32*)(src + 0x1F8);
    *reinterpret_cast<f32*>(dst + 0x1AC) = *(const f32*)(src + 0x1FC);
    *reinterpret_cast<f32*>(dst + 0x1B0) = *(const f32*)(src + 0x200);
    *reinterpret_cast<f32*>(dst + 0x1B4) = *(const f32*)(src + 0x204);
    *reinterpret_cast<f32*>(dst + 0x1B8) = *(const f32*)(src + 0x208);
    *reinterpret_cast<f32*>(dst + 0x1BC) = *(const f32*)(src + 0x20C);
    *reinterpret_cast<f32*>(dst + 0x1C0) = *(const f32*)(src + 0x210);
    *reinterpret_cast<u32*>(dst + 0x1C4) = *(const u32*)(src + 0x214);
    *reinterpret_cast<f32*>(dst + 0x1C8) = *(const f32*)(src + 0x218);
    *reinterpret_cast<f32*>(dst + 0x1CC) = *(const f32*)(src + 0x21C);
    *reinterpret_cast<f32*>(dst + 0x1D0) = *(const f32*)(src + 0x220);
    *reinterpret_cast<f32*>(dst + 0x1D4) = *(const f32*)(src + 0x224);
    *reinterpret_cast<f32*>(dst + 0x1D8) = *(const f32*)(src + 0x228);
    *reinterpret_cast<f32*>(dst + 0x1DC) = *(const f32*)(src + 0x22C);
    *reinterpret_cast<f32*>(dst + 0x1E0) = *(const f32*)(src + 0x230);
    *reinterpret_cast<f32*>(dst + 0x1E4) = *(const f32*)(src + 0x234);
    *reinterpret_cast<f32*>(dst + 0x1E8) = *(const f32*)(src + 0x238);
    *reinterpret_cast<f32*>(dst + 0x1EC) = *(const f32*)(src + 0x23C);
    *reinterpret_cast<f32*>(dst + 0x1F0) = *(const f32*)(src + 0x240);
    *reinterpret_cast<f32*>(dst + 0x1F4) = *(const f32*)(src + 0x244);
    *reinterpret_cast<f32*>(dst + 0x1F8) = *(const f32*)(src + 0x248);
    *reinterpret_cast<s16*>(dst + 0x1FC) = *(const s16*)(src + 0x250);
    *reinterpret_cast<u16*>(dst + 0x1FE) = *(const u16*)(src + 0x254);
    *reinterpret_cast<s16*>(dst + 0x200) = *(const s16*)(src + 0x256);
    *reinterpret_cast<s16*>(dst + 0x202) = *(const s16*)(src + 0x258);
}
// cfCam_pushStateToActive: copy the follow-camera state from `self` (or the shared
// fallback buffer lbl_eu_80570A8C when self == 0) into the active camera
// object. Gates: the camera manager must exist (cfCam_getEventMgr), the active
// object must come back non-null from cfCam_getActiveObj and carry the 0x20 flag.
// Copies the 16 stride-12 vec3 slots at +0x70, the +0x114..0x178 block, then
// the float/word/halfword tail through cfCam_copyWordA/C0/CC.
void cfCam_pushStateToActive(cf::CfCamFollow* self) {
    u8* src = reinterpret_cast<u8*>(self);
    if (self == 0) {
        src = lbl_eu_80570A8C;
    }
    CfCamEventManager* mgr = cfCam_getEventMgr();
    if (mgr == 0) {
        return;
    }
    cf::CfCamFollow* dst = reinterpret_cast<cf::CfCamFollow*>(cfCam_getActiveObj(mgr));
    if (dst == 0) {
        return;
    }
    if (cfCam_maskUnk1D4(reinterpret_cast<int>(dst), 0x200000) == 0) {
        return;
    }
    copyVec3Words(dst->unk10, src + 0x00);
    copyVec3Words(dst->unk1C, src + 0x0C);
    copyVec3Words(dst->unk1C + 12, src + 0x18);
    copyVec3Words(dst->unk1C + 24, src + 0x24);
    copyVec3Words(dst->unk1C + 36, src + 0x30);
    copyVec3Words(dst->unk1C + 48, src + 0x3C);
    copyVec3Words(dst->unk1C + 60, src + 0x48);
    for (int i = 0; i < cfCam_histCount(src + 0x54); i++) {
        copyVec3Words(cfCam_vec3AtIndex(dst->unk70, i), cfCam_vec3AtIndex(src + 0x54, i));
    }
    copyVec3Words(dst->unk168, src + 0x114);
    copyVec3Words(dst->unk168 + 12, src + 0x120);
    copyVec3Words(dst->unk168 + 24, src + 0x12C);
    copyVec3Words(dst->unk168 + 36, src + 0x138);
    copyVec3Words(dst->unk168 + 48, src + 0x144);
    copyVec3Words(dst->unk168 + 60, src + 0x150);
    copyVec3Words(dst->unk168 + 72, src + 0x15C);
    copyVec3Words(dst->unk168 + 84, src + 0x168);
    copyVec3Words(dst->unk168 + 96, src + 0x174);
    cfCam_copyWordA(&dst->field_0x04, src + 0x180);
    cfCam_copyWordB(&dst->unk1D4, src + 0x184);
    cfCam_copyHalf(&dst->unk1D8, src + 0x188);
    dst->unk1DC = *(const f32*)(src + 0x18C);
    dst->unk1E0 = *(const f32*)(src + 0x190);
    dst->unk1E4 = *(const f32*)(src + 0x194);
    dst->unk1E8 = *(const f32*)(src + 0x198);
    dst->unk1EC = *(const f32*)(src + 0x19C);
    dst->unk1F0 = *(const f32*)(src + 0x1A0);
    dst->unk1F4 = *(const f32*)(src + 0x1A4);
    dst->unk1F8 = *(const f32*)(src + 0x1A8);
    dst->unk1FC = *(const f32*)(src + 0x1AC);
    dst->unk200 = *(const f32*)(src + 0x1B0);
    dst->unk204 = *(const f32*)(src + 0x1B4);
    dst->unk208 = *(const f32*)(src + 0x1B8);
    dst->unk20C = *(const f32*)(src + 0x1BC);
    dst->unk210 = *(const f32*)(src + 0x1C0);
    dst->unk214 = *(const u32*)(src + 0x1C4);
    dst->unk218 = *(const f32*)(src + 0x1C8);
    dst->unk21C = *(const f32*)(src + 0x1CC);
    dst->unk220 = *(const f32*)(src + 0x1D0);
    dst->unk224 = *(const f32*)(src + 0x1D4);
    dst->unk228 = *(const f32*)(src + 0x1D8);
    dst->unk22C = *(const f32*)(src + 0x1DC);
    dst->unk230 = *(const f32*)(src + 0x1E0);
    dst->unk234 = *(const f32*)(src + 0x1E4);
    dst->unk238 = *(const f32*)(src + 0x1E8);
    dst->unk23C = *(const f32*)(src + 0x1EC);
    dst->unk240 = *(const f32*)(src + 0x1F0);
    dst->unk244 = *(const f32*)(src + 0x1F4);
    dst->unk248 = *(const f32*)(src + 0x1F8);
    dst->unk250 = *(const s16*)(src + 0x1FC);
    dst->unk254 = *(const u16*)(src + 0x1FE);
    dst->unk256 = *(const s16*)(src + 0x200);
    dst->unk258 = *(const s16*)(src + 0x202);
    cfCam_andcUnk1D4(dst, 0x200000);
}
// func_8006E884: follow-camera per-frame driver. Early-outs when the 0x400
// pad gate is set (refreshing the +0x1e0 blend), handles the D80-flag snap
// path, then runs the full update chain: flag sync, talk-source resolve,
// history/position blend (func_8006EFA0), target selection, heading updates
// and the shared-global mirror block.
void func_8006E884(cf::CfCamFollow* self, float argF) {
    // Locals in retail frame-slot order.
    f32 out8;                 // 0x08 func_8006FFA8 outB
    f32 outC;                 // 0x0c func_8006FFA8 outA
    f32 out10;                // 0x10 cfCam_updateHead/E0C8 out2
    f32 out14;                // 0x14 cfCam_updateHead/E0C8 out1
    f32 out18;                // 0x18 func_8006F5C8 out2
    f32 out1C;                // 0x1c func_8006F5C8 out1
    nw4r::math::VEC3 stack20; // 0x20
    ml::CVec3 stack2C;        // 0x2c
    ml::CVec3 stack38;        // 0x38
    f32 stack44[3];           // 0x44
    nw4r::math::VEC3 stack50; // 0x50 (early-out path)
    ml::CVec3 stack5C;        // 0x5c
    f32 stack68[3];           // 0x68
    ml::CVec3 stack74;        // 0x74
    ml::CVec3 stack80;        // 0x80

    if (cfCam_testUnk04M(reinterpret_cast<int>(self), 0x400000) != 0) {
        if (func_8007560C() == 0) {
            cfCam_nopVirtFloat(self, self->unk1E0);
        }
        return;
    }
    // Note: retail compares the incoming float (f1) against 0.0 AFTER the two
    // gate calls without spilling it - keep this operand order when iterating.
    if (CfRes_getD80Flag() != 0 && Scn_GetFrameDelta(lbl_eu_80663E14) != 0 && argF == lbl_eu_806662DC) {
        // Snap: blend straight from the +0x40 vector into the +0x34 slot.
        cfCam_psAddVec3(&stack50, reinterpret_cast<const nw4r::math::VEC3*>(self->unk10 + 0x30),
                      reinterpret_cast<const nw4r::math::VEC3*>(self->unk188 + 0x40));
        cfCam_applyRelPos(self, self->unk1C + 0x18, &stack50, 1, lbl_eu_80661B50,
                      lbl_eu_806662DC);
        return;
    }
    if (cfCam_getE28Bit24() != 0) {
        return;
    }
    cfCam_andcUnk1D4(self, 0x800);
    cfCam_syncSelFlags(self);
    func_8006C1C8(self);
    if (self->unk164 != 0 && cfCam_bit1At0x64(self->unk164) == 0) {
        cfCam_getBit2_64(self->unk164);
    }

    noopAnimVec3(&stack80);
    cfCam_getFollowSrc(&stack74, self);
    // Both subtractions use the same operands (retail recomputes them).
    subVec3f(stack68, reinterpret_cast<const f32*>(&stack74.x),
                  reinterpret_cast<const f32*>(self->unk1C + 0x3C));
    f32 f31 = self->unk22C;
    subVec3f(stack44, reinterpret_cast<const f32*>(&stack74.x),
                  reinterpret_cast<const f32*>(self->unk1C + 0x3C));
    copyVec3Words(self->unk188 + 4, stack44);
    if (!reinterpret_cast<ml::CVec3*>(self->unk188 + 4)->isErr()) {
        cfCam_zeroVec3(self->unk188 + 4);
    }

    void* talkSrc = func_800BBC0C(self->unk164);
    func_8006D8D0(self, talkSrc);
    int selFlag = 0;
    if (cfCam_maskUnk1D4(reinterpret_cast<int>(self), 0x10) != 0) {
        selFlag = stack68[2] < lbl_eu_80666340;
    }
    if (cfCam_testUnk04M(reinterpret_cast<int>(self), 0x800) != 0) {
        cfCam_updateFrame(self);
        return;
    }
    if (talkSrc != 0 && cfCam_loadUnkC4(talkSrc) != 0) {
        talkSrc = cfCam_loadUnkC4(talkSrc);
        if (testAnimMoveBit(talkSrc) != 0) {
            f31 = cfCam_absFloat(reinterpret_cast<u8*>(cfCam_getPlus3D8(talkSrc)) + 4);
        } else {
            f31 = self->unk22C;
        }
    }

    func_8006EFA0(self, f31, &stack80, &stack74,
                  cfCam_maskUnk1D4(reinterpret_cast<int>(self), 0x10));
    func_8006F5C8(self, &stack80, &out1C, &out18);
    self->unk244 = self->unk23C + self->unk240;
    cfCam_buildOffset(&stack38, self, &stack80,
                  reinterpret_cast<const f32*>(&self->field_0x180), self->unk244);
    copyVec3Words(&self->field_0x28, &stack38);
    cfCam_syncFollowSnapshot(self, &stack80, &stack74);
    func_8006DD58(self, &stack80);

    // GameManager probe through the main vtable slot 0x60 decides which of
    // the two update routes runs this frame.
    void* gmRet = self->cfCam_loadUnk164();
    int route = 0;
    if (cfCam_artsGateClr(gmRet) != 0 || cfCam_testUnk04M(reinterpret_cast<int>(self), 0x10) != 0 ||
        cfCam_testUnk04M(reinterpret_cast<int>(self), 0x400000) != 0 ||
        cfCam_maskUnk1D4(reinterpret_cast<int>(self), 0x20000000) != 0) {
        route = 1;
    }
    if (route == 0) {
        // Route A: zoom/aim reset driven by the game-manager state.
        int r29 = 1;
        if (cfCam_testUnk04M(reinterpret_cast<int>(self), 0x404000) != 0) {
            if (cfCam_artsGateClr(self->cfCam_loadUnk164()) != 0) {
                r29 = 0;
            }
        }
        if (r29 != 0) {
            if (cfCam_testUnk04M(reinterpret_cast<int>(self), 0x404000) == 0) {
                if (cfCam_testUnk04M(reinterpret_cast<int>(self), 0x800000) == 0 &&
                    cfCam_maskUnk1D4(reinterpret_cast<int>(self), 0x200000) != 0) {
                    cfCam_andcUnk1D4(self, 0x200000);
                }
                cfCam_setClear04(self, 0x800000, 1);
            }
            cfCam_andcUnk04(self, 0x404000);
        }
    } else {
        // Route B: stick-driven heading via DFE0/E0C8.
        if (cfCam_getBit9_04(self) == 0) {
            if (cfCam_maskUnk1D4(reinterpret_cast<int>(self), 0x2000) != 0) {
                cfCam_updateHead(self, &stack80, &out14, &out10, 0, self->unk23C);
            } else {
                cfCam_updateYaw(self, &stack80, &out14, &out10, 0);
            }
            cfCam_maintainFlg(self, out14);
            if (cfCam_testUnk04M(reinterpret_cast<int>(self), 0x10) != 0) {
                self->unk1F4 = out10;
            }
        }
    }

    if (cfCam_testUnk04M(reinterpret_cast<int>(self), 0x10) == 0) {
        self->unk1F4 = lbl_eu_806662DC;
    }
    func_80071754(self, &stack80);
    func_80072194(self, &stack80);
    isGlobalCamFlagSet(0x200);
    if (cfCam_testUnk04M(reinterpret_cast<int>(self), 0x1000000) != 0) {
        cfCam_isClassicPad(self, lbl_eu_806662DC);
        if (func_802753B8(self) != 0 &&
            cfCam_maskUnk1D4(reinterpret_cast<int>(self), 0x20) == 0) {
            // Decay +0x21c to zero while ramping +0x180, then clamp +0x180
            // into the range reported by func_8006FFA8.
            f32 scaled = scaleByGlobal(self->unk21C);
            self->unk21C = lbl_eu_806662DC;
            self->field_0x180 = self->field_0x180 + scaled;
            func_8006FFA8(self, &outC, &out8);
            f32 v = self->field_0x180;
            if (v >= outC) {
                if (v > out8) {
                    v = out8;
                }
            } else {
                v = outC;
            }
            self->field_0x180 = v;
        }
    }
    func_8006FD3C(self, reinterpret_cast<const nw4r::math::VEC3*>(&stack80), out1C);
    func_80070088(self, &stack80, out18);
    cfCam_rampFollowY(self, selFlag);

    copyVec3f(&stack5C, reinterpret_cast<const ml::CVec3*>(&self->field_0x28));
    cfCam_psAddVec3(reinterpret_cast<nw4r::math::VEC3*>(&stack2C.x),
                  reinterpret_cast<const nw4r::math::VEC3*>(&stack74.x),
                  reinterpret_cast<const nw4r::math::VEC3*>(self->unk188 + 0x40));
    func_800707C0(self, &stack5C, &stack2C);
    cfCam_psAddVec3(&stack20, reinterpret_cast<const nw4r::math::VEC3*>(&stack80.x),
                  reinterpret_cast<const nw4r::math::VEC3*>(self->unk188 + 0x40));
    cfCam_applyRelPos(self, self->unk1C + 0x18, reinterpret_cast<void*>(&stack20), 1,
                  lbl_eu_80661B50, lbl_eu_806662DC);
    self->unk1E0 = lbl_eu_80661B50;
    cfCam_copyBlock48(reinterpret_cast<CfCamWordBlock48*>(self->unk130),
                  reinterpret_cast<CfCamWordBlock48*>(cfCam_getPlus9C(self->unk160)));
    copyVec3Words(self->unk10 + 0x30, &stack80);
    copyVec3Words(self->unk1C + 0x3C, &stack74);

    if (cfCam_testUnk04M(reinterpret_cast<int>(self), 0x404000) == 0) {
        // Mirror state into the shared globals for other cameras.
        f32 sel;
        if (self->unk256 == 0) {
            sel = self->unk1F8;
        } else {
            sel = self->field_0x180;
        }
        float* shared = lbl_eu_80570A38;
        shared[0] = sel;
        lbl_eu_80663DE4 = self->unk23C;
        shared[1] = self->field_0x184;
    }

    // Advance the history ring counter (saturating at 15, negative wraps to
    // 15) and store the blended position into the new slot.
    self->unk250 = self->unk250 + 1;
    int cnt;
    if (self->unk250 < 0) {
        cnt = 15;
    } else {
        int raw = self->unk250;
        cnt = (raw > 15) ? 15 : raw;
    }
    self->unk250 = cnt;
    u8* hist = cfCam_vec3AtIndex(self->unk70, cnt);
    copyVec3Words(hist, &stack74);
    self->unk22C = f31;
}
// func_8006EFA0: follow-camera per-frame position update. Blends the camera
// position (+0x4c vec) toward the target using the averaged history samples at
// +0x70, with per-axis smoothing gains derived from the +0x18c vector
// magnitude and the smoothed heading distance at +0x230.
void func_8006EFA0(cf::CfCamFollow* self, float argF, ml::CVec3* outPos,
                   ml::CVec3* target, int flag) {
    // Derived vec3 addresses (+0x4c / +0x34 / +0x58) are re-materialized at
    // each use - retail never keeps them in registers.
#define CAMPOS (reinterpret_cast<f32*>(self->unk1C + 0x30)) // +0x4c vec
#define VEC34 (reinterpret_cast<f32*>(self->unk1C + 0x18))  // +0x34 vec
#define VEC58 (reinterpret_cast<f32*>(self->unk1C + 0x3C))  // +0x58 vec

    // Locals declared in retail frame-slot order: address-taken scalars at
    // +0x08/+0x0c, then the 12-byte work arrays ascending to +0x64.
    f32 delta;   // 0x08 dot difference (address taken by cfCam_absFloat)
    f32 d;       // 0x0c heading-distance delta (address taken)
    f32 diff10[3]; // 0x10 target - vec58 diff
    f32 work[3];   // 0x1c blended-position scratch
    ml::CVec3 sum; // 0x28 history accumulator
    f32 dirC[3];   // 0x34 normalized cam-fwd vs history direction
    f32 histV[3];  // 0x40 history sample copy
    f32 dirB[3];   // 0x4c target direction
    f32 dirA[3];   // 0x58 camera direction
    f32 diff64[3]; // 0x64 target - campos diff (later holds x/z averages)

    // diff between target and the +0x58 vector, stored into +0x18c.
    subVec3f(diff10, reinterpret_cast<const f32*>(target), VEC58);
    copyVec3Words(self->unk188 + 4, diff10);
    if (!reinterpret_cast<ml::CVec3*>(self->unk188 + 4)->isErr()) {
        cfCam_zeroVec3(self->unk188 + 4);
    }

    u32 mode = self->field_0x08;
    if (mode == 0 || (mode == 1 && lbl_eu_80661B7C == 0)) {
        // Snap: copy the target out and straight into the camera position.
        copyVec3Words(CAMPOS, copyVec3Words(outPos, target));
        cfCam_resetHist(self, target);
        return;
    }
    if (mode != 1) {
        return;
    }

    subVec3f(diff64, reinterpret_cast<const f32*>(target), CAMPOS);
    if (cfCam_vecLenSq(reinterpret_cast<nw4r::math::VEC3*>(diff64)) <
            lbl_eu_80661B6C * lbl_eu_80661B6C && flag == 0) {
        // Close enough with no flag: snap and reset the history ring.
        copyVec3Words(CAMPOS, target);
        cfCam_resetHist(self, target);
    } else {
        // Remote-pad active or gate set -> ramp the step counter up, else decay it.
        if (cfCam_padAbsent(reinterpret_cast<int>(self)) || cfCam_testPad100(self) ||
            cfCam_maskUnk1D4(reinterpret_cast<int>(self), 0x10)) {
            if ((int)self->unk214 < (int)lbl_eu_80661B94) {
                self->unk214 = self->unk214 + 1;
            }
        } else {
            if ((int)self->unk214 > 0) {
                self->unk214 = self->unk214 - 1;
            }
        }
        // (retail recomputes the counter and the cap constant at every use)

        f32 scaleDist = lbl_eu_806662D0; // 1.0f
        if (lbl_eu_80661BB0 != 0 && cfCam_maskUnk1D4(reinterpret_cast<int>(self), 0x80)) {
            // Horizontal aim check: compare the normalized camera->target
            // direction against both the camera forward (+0x34) and the last
            // history sample; when they diverge past the threshold, flatten
            // the whole history ring onto the target x/z.
            subVec3f(dirA, CAMPOS, VEC34);
            subVec3f(dirB, reinterpret_cast<const f32*>(target), VEC34);
            dirA[1] = lbl_eu_806662DC;
            dirB[1] = lbl_eu_806662DC;
            VecMath_NormalizeInPlace(reinterpret_cast<ml::CVec3*>(dirA));
            VecMath_NormalizeInPlace(reinterpret_cast<ml::CVec3*>(dirB));
            f32* hist = reinterpret_cast<f32*>(
                cfCam_vec3AtIndex(self->unk70, self->unk250));
            copyVec3f(reinterpret_cast<ml::CVec3*>(histV),
                          reinterpret_cast<const ml::CVec3*>(hist));
            subVec3f(dirC, histV, VEC34);
            dirC[1] = lbl_eu_806662DC;
            VecMath_NormalizeInPlace(reinterpret_cast<ml::CVec3*>(dirC));
            f32 dotA = dotVec3f(dirA, dirB);
            f32 dotB = dotVec3f(dirA, dirC);
            delta = dotA - dotB;
            if (dotA > dotB && cfCam_absFloat(&delta) < lbl_eu_8066633C) {
                for (unsigned int i = 0; i < 16; i++) {
                    f32 vx = target->x;
                    f32 vz = target->z;
                    f32* slotA = reinterpret_cast<f32*>(cfCam_vec3AtIndex(self->unk70, i));
                    slotA[0] = vx;
                    f32* slotB = reinterpret_cast<f32*>(cfCam_vec3AtIndex(self->unk70, i));
                    slotB[2] = vz;
                }
            }
        }

        // Average the newest `count` history samples around the current
        // camera position to get a blended follow offset.
        u32 count;
        if (cfCam_maskUnk1D4(reinterpret_cast<int>(self), 0x40)) {
            count = 2;
        } else {
            count = lbl_eu_80661B7C;
        }
        f32 mag18c = cfCam_vecLenSq(reinterpret_cast<nw4r::math::VEC3*>(self->unk188 + 4));
        noopAnimVec3(&sum);
        sum.x = target->x - CAMPOS[0];
        sum.y = lbl_eu_806662DC;
        sum.z = target->z - CAMPOS[2];
        for (int i = 0; i < (int)count; i++) {
            int idx = self->unk250 - i;
            if (idx < 0) {
                idx += 16;
            }
            f32* slotA = reinterpret_cast<f32*>(cfCam_vec3AtIndex(self->unk70, idx));
            sum.x += slotA[0] - CAMPOS[0];
            f32* slotB = reinterpret_cast<f32*>(cfCam_vec3AtIndex(self->unk70, idx));
            sum.z += slotB[2] - CAMPOS[2];
        }
        float inv = lbl_eu_806662D0 / (count + 1);
        // Averages overwrite the x/z slots of diff64 (y keeps target-cam).
        diff64[0] = sum.x * inv;
        diff64[2] = sum.z * inv;

        if (self->unk164 != 0 && mag18c < lbl_eu_80666344 && flag == 0) {
            scaleDist = scaleDist * lbl_eu_80661B84;
        } else {
            // Smoothly approach the requested heading-distance in +0x230.
            d = argF - self->unk230;
            if (cfCam_absFloat(&d) < lbl_eu_80666348) {
                self->unk230 = lbl_eu_806662A0 * d + self->unk230;
            } else {
                self->unk230 = lbl_eu_8066634C * d + self->unk230;
            }
            f32 gain = lbl_eu_80661B80;
            if (mag18c > lbl_eu_80666304) {
                gain = lbl_eu_80666350 * (mag18c - lbl_eu_80666304) + gain;
                if (gain > lbl_eu_806662D0) {
                    gain = lbl_eu_806662D0;
                }
            }
            scaleDist *= gain;
        }

        // Final blend weight: max of scaleDist and the floor constant,
        // overridden by flag probes on this camera.
        f32 w = lbl_eu_80666354;
        if (scaleDist >= lbl_eu_80666354) {
            w = scaleDist;
        }
        if (cfCam_maskUnk1D4(reinterpret_cast<int>(self), 0x800)) {
            w = lbl_eu_80666354;
        }
        if (cfCam_maskUnk1D4(reinterpret_cast<int>(self), 8)) {
            w = lbl_eu_806662D0;
        }

        if ((int)self->unk214 > 0 && (int)self->unk214 < (int)lbl_eu_80661B94) {
            // Transition window: lerp from the raw target toward the blended
            // position over the remaining step count.
            float t = (float)((int)lbl_eu_80661B94 - (int)self->unk214) / lbl_eu_80661B94;
            noopAnimVec3(work);
            work[0] = diff64[0] * scaleDist + CAMPOS[0];
            work[2] = diff64[2] * scaleDist + CAMPOS[2];
            work[1] = diff64[1] * w + CAMPOS[1];
            CAMPOS[0] = target->x * (lbl_eu_806662D0 - t) + work[0] * t;
            CAMPOS[2] = target->z * (lbl_eu_806662D0 - t) + work[2] * t;
            CAMPOS[1] = diff64[1] * w + CAMPOS[1];
        } else if (cfCam_padAbsent(reinterpret_cast<int>(self)) || cfCam_testPad100(self) ||
                   cfCam_maskUnk1D4(reinterpret_cast<int>(self), 0x10)) {
            // Remote pad active: track the target directly on x/z.
            CAMPOS[0] = target->x;
            CAMPOS[2] = target->z;
            CAMPOS[1] = diff64[1] * w + CAMPOS[1];
        } else {
            CAMPOS[0] = diff64[0] * scaleDist + CAMPOS[0];
            CAMPOS[2] = diff64[2] * scaleDist + CAMPOS[2];
            CAMPOS[1] = diff64[1] * w + CAMPOS[1];
        }
    }

    // Hand the updated camera position back to the caller.
    copyVec3Words(outPos, CAMPOS);
#undef CAMPOS
#undef VEC34
#undef VEC58
}
// func_8006F5C8: follow-camera per-frame target/heading selection. Zeroes the
// two output floats, then (unless both the 0x20000 flag probe and the
// func_80075640 gate reject) runs the +0x70 vtable sub-object path: clears
// the 0x10000 flag, reads the remote-pad availability (its result becomes the
// return value), and - when the cfCam_testPad100 gate is clear - picks out1 from
// the 0x1/0x4 flag + camera-register matrix (scaled by |sub+0x68| on a classic
// pad) and out2 from the 0x2/0x8 flag + move-register matrix (scaled by
// |sub+0x6C|). The flag-word OR of CfRes_getE24Bit22/cfCam_testGlobalE24/
// cfCam_getE28Bit24 drives a delayed reset path (0x254 countdown, isSceneActive /
// cfCam_tuneZoomDist) and finally zeroes both outputs when set.
int func_8006F5C8(cf::CfCamFollow* self, void* arg, float* out1, float* out2) {
    *out1 = lbl_eu_806662DC;
    *out2 = lbl_eu_806662DC;
    if (cfCam_testUnk04M(reinterpret_cast<int>(self), 0x20000) == 0 &&
        func_80075640() == 0) {
        return 0;
    }
    void* sub = self->cfCam_getActivePad();
    cfCam_andcUnk04(self, 0x10000);
    int result = 0;
    if (*(reinterpret_cast<const u8*>(sub) + 0xED) != 0) {
        result = cfCam_padAbsent(reinterpret_cast<int>(self));
        if (result != 0) {
            cfCam_setClear04(self, 0x10000, 1);
        } else {
            cfCam_andcUnk04(self, 0x10000);
        }
        if (cfCam_testPad100(self) == 0) {
            if (cfCam_testUnk04M(reinterpret_cast<int>(self), 1) == 0) {
                if (func_80275378(self) != 0) {
                    *out1 = cfCam_testUnk04M(reinterpret_cast<int>(self), 4) != 0
                                ? lbl_eu_80666358
                                : lbl_eu_806662D0;
                } else if (func_80275338(self) != 0) {
                    *out1 = cfCam_testUnk04M(reinterpret_cast<int>(self), 4) != 0
                                ? lbl_eu_806662D0
                                : lbl_eu_80666358;
                }
                if (cfCam_isClassicPad(self, lbl_eu_806662DC) != 0) {
                    *out1 *= cfCam_absFloat(reinterpret_cast<u8*>(sub) + 0x68);
                }
            }
            if (cfCam_testUnk04M(reinterpret_cast<int>(self), 2) == 0) {
                if (func_802752B8(self) != 0) {
                    *out2 = cfCam_testUnk04M(reinterpret_cast<int>(self), 8) != 0
                                ? lbl_eu_806662D0
                                : lbl_eu_80666358;
                } else if (func_802752F8(self) != 0) {
                    *out2 = cfCam_testUnk04M(reinterpret_cast<int>(self), 8) != 0
                                ? lbl_eu_80666358
                                : lbl_eu_806662D0;
                }
                if (cfCam_isClassicPad(self, lbl_eu_806662DC) != 0) {
                    *out2 *= cfCam_absFloat(reinterpret_cast<u8*>(sub) + 0x6C);
                }
            }
        }
    }
    int flags = CfRes_getE24Bit22() | (int)cfCam_testGlobalE24() |
                (int)cfCam_getE28Bit24();
    int r31 = 1;
    if (battleCommuIsActive() == 0 && func_8017FD44() == 0) {
        r31 = 0;
    }
    if (cfCam_testUnk04M(reinterpret_cast<int>(self), 0x400) == 0 && flags == 0 &&
        r31 == 0) {
        if (func_802751F8(self) != 0 && cfCam_getBit9_04(self) == 0 &&
            cfCam_testUnk04M(reinterpret_cast<int>(self), 0x800) == 0) {
            self->unk254 = 15;
            self->unk248 = lbl_eu_806662DC;
            cfCam_setClear04(self, 0x8000, 1);
            self->cfCam_loadUnk164();
            int dca0 = cfCam_artsGateClr(arg);
            if (dca0 != 0) {
                if (cfCam_testUnk04M(reinterpret_cast<int>(self), 0x10) == 0) {
                    cfCam_setClear04(self, 0x10, 1);
                }
            }
            func_8006CE24(self, arg, 1, 1, 1, 1, 0);
            if (dca0 != 0 &&
                cfCam_testUnk04M(reinterpret_cast<int>(self), 0x10) != 0) {
                cfCam_andcUnk04(self, 0x10);
            }
        }
    }
    int flags2 = CfRes_getE24Bit22() | (int)isGlobalCamFlagSet__Fi(0xABA40000) |
                 (int)cfCam_getE28Bit24();
    if (cfCam_testUnk04M(reinterpret_cast<int>(self), 0xC10) == 0 && r31 == 0) {
        if (self->unk254 != 0) {
            self->unk254 -= 1;
        } else if (flags2 == 0) {
            if (isSceneActive__Q22cf13CfGameManagerFv() == 0) {
                cfCam_tuneZoomDist(self);
            }
        }
    }
    if (flags2 != 0) {
        *out2 = lbl_eu_806662DC;
        *out1 = lbl_eu_806662DC;
    }
    return result;
}
// cfCam_syncFollowSnapshot: follow-cam state snapshot/restore driver. With the 0x40
// flag: claim 0x20, clear 0x40, chain-copy the position vec3 slots from
// srcPos through +0x58/+0x64/+0x4C/+0x40 into arg, then feed the source's
// scale and the shared vec3 into the slot-0x18 virtual. The 0x2000 path
// clears the follow state; the 0x30000020 path re-runs the pad-action selection.
void cfCam_syncFollowSnapshot(cf::CfCamFollow* self, void* arg, void* srcPos) {
    int bit1000;
    int bit2000;
    if (cfCam_testUnk04M(reinterpret_cast<int>(self), 0x40) != 0 &&
        self->unk164 != 0) {
        cfCam_setClear04(self, 0x20, 1);
        cfCam_andcUnk04(self, 0x40);
        void* p = copyVec3Words(self->unk1C + 0x3C, srcPos);
        p = copyVec3Words(self->unk1C + 0x48, p);
        p = copyVec3Words(self->unk1C + 0x30, p);
        p = copyVec3Words(self->unk1C + 0x24, p);
        copyVec3Words(arg, p);
        f32 scale = self->unk164->CfObject_getMoveHeadAngle();
        nw4r::math::VEC3 v;
        writeVec3f(&v, lbl_eu_80570A38[0], scale, lbl_eu_806662DC);
        self->cfCam_updFollowPos(&v, lbl_eu_80661B60);
    }
    if (cfCam_testUnk04M(reinterpret_cast<int>(self), 0x2000) != 0) {
        cfCam_setClear04(self, 0x20, 1);
        cfCam_andcUnk04(self, 0x2000);
        self->unk252 = 0;
    }
    if (cfCam_testUnk04M(reinterpret_cast<int>(self), 0x20) != 0 &&
        cfCam_testUnk04M(reinterpret_cast<int>(self), 0x404000) == 0 &&
        (s16)self->unk256 > 0) {
        cfCam_andcUnk04(self, 0x20);
    }
    // 0x30000020 = follow-reset mask. Held in a local so MWCC hoists the
    // 0x30000000 high half into a register across both probes (retail
    // lis/addi pair) instead of rematerializing per use.
    int resetMask = 0x30000020;
    if (cfCam_testUnk04M(reinterpret_cast<int>(self), resetMask) != 0) {
        bit1000 = cfCam_testUnk04M(reinterpret_cast<int>(self), 0x10000000);
        bit2000 = cfCam_testUnk04M(reinterpret_cast<int>(self), 0x20000000);
        cfCam_andcUnk04(self, resetMask);
        self->unk248 = lbl_eu_806662DC;
        if (cfCam_getBit9_04(self) == 0 &&
            cfCam_testUnk04M(reinterpret_cast<int>(self), 0x800) == 0) {
            if (bit1000 != 0) {
                func_8006CE24(self, arg, 1, 1, 1, 1, 0);
            } else if (bit2000 != 0) {
                func_8006CE24(self, arg, 1, 0, 0, 0, 0);
            } else {
                func_8006CE24(self, arg, 0, 0, 0, 1, 0);
            }
        }
    }
}
// cfCam_clampHeading: clamp a heading angle toward the follow-cam's current facing.
// When the 0x400 flag is set and the +0x164 source object exists, the source's
// vtable slot 0xCC facing is wrapped into [-pi, pi) (wrapAnglePi); the
// +/-span window around it (span = lbl_eu_8066635C * pi/2 / lbl_eu_80666324)
// is snapped by lbl_eu_8066A1FC steps and finally clamped into [lo, hi].
// Constants are inlined (not named locals) so MWCC picks retail's FPR set.
// Returns whether the clamp changed *angle.
int cfCam_clampHeading(cf::CfCamFollow* self, float* angle) {
    int result = 0;
    if (cfCam_maskUnk1D4((int)self, 0x400) != 0 && self->unk164 != 0) {
        f32 w = wrapAnglePi(lbl_eu_8066A1F8 +
                              self->unk164->CfObject_getMoveHeadAngle());
        f32 lo = w - lbl_eu_8066635C * lbl_eu_8066A200 / lbl_eu_80666324;
        f32 hi = w + lbl_eu_8066635C * lbl_eu_8066A200 / lbl_eu_80666324;
        if (*angle < lo - lbl_eu_8066A200) {
            *angle += lbl_eu_8066A1FC;
        } else if (*angle > hi + lbl_eu_8066A200) {
            *angle -= lbl_eu_8066A1FC;
        }
        if (*angle > hi) {
            *angle = hi;
            result = 1;
        } else if (*angle < lo) {
            *angle = lo;
            result = 1;
        }
    }
    return result;
}
// func_8006FD3C: follow-camera heading update from a stick input value.
// Nonzero input ramps unk1E4 by input*scale (clamped to +/-|input| when the
// classic-pad gate is set); zero input decays unk1E4 toward 0. When either
// decay float is still live, rotates the +0x28/+0x30 camera position around
// the target by the blended heading angle.
void func_8006FD3C(cf::CfCamFollow* self, const nw4r::math::VEC3* arg,
                   float input) {
    f32 lim;
    if (input != lbl_eu_806662DC) {
        f32 scale;
        if (cfCam_maskUnk1D4(reinterpret_cast<int>(self), 0x100) != 0) {
            scale = lbl_eu_80666360 * lbl_eu_80661B40;
        } else {
            scale = lbl_eu_80661B40;
        }
        // assigned here (not before the branch) so MWCC keeps the load inside
        // the branch like retail instead of hoisting it to the prologue
        // read input through its address so MWCC spills it to its stack slot
        lim = lbl_eu_806662D0;
        self->unk1E4 = input * scale + self->unk1E4;
        if (cfCam_isClassicPad(self, input)) {
            lim = cfCam_absFloat(&input);
        }
        // clamp unk1E4 into [-lim, lim]
        if (self->unk1E4 < -lim) {
            self->unk1E4 = -lim;
        } else if (self->unk1E4 > lim) {
            self->unk1E4 = lim;
        }
    } else {
        self->unk1E4 *= lbl_eu_80661B48;
        f32 mag = cfCam_absFloat(&self->unk1E4);
        if (mag <= lbl_eu_8066A208) {
            self->unk1E4 = lbl_eu_806662DC;
        }
    }
    if (cfCam_absFloat(&self->unk1F4) > lbl_eu_8066A208 ||
        cfCam_absFloat(&self->unk1E4) > lbl_eu_8066A208) {
        f32 l10[3];
        subVec3f(l10, reinterpret_cast<const f32*>(&self->field_0x28),
                      reinterpret_cast<const f32*>(arg));
        f32 mag0 = self->unk244;
        f32 camY = self->field_0x180;
        f32 yaw = cfCam_yawFromVec(l10);
        f32 angle = yaw;
        if (cfCam_absFloat(&self->unk1F4) > lbl_eu_8066A208) {
            f32 step = lbl_eu_8066634C * self->unk1F4;
            // limit is the scaled constant; step is clamped to +/-limit
            f32 limit = scaleByGlobal(lbl_eu_80661B78);
            if (self->unk1F4 >= lbl_eu_806662DC) {
                if (step > limit) {
                    step = limit;
                }
            } else {
                if (step < -limit) {
                    step = -limit;
                }
            }
            angle = yaw + step;
            self->unk1F4 -= step;
        } else {
            angle = self->unk1E4 * lbl_eu_80661B3C + yaw;
            cfCam_clampHeading(self, &angle);
        }
        f32 wrappedCamY = wrapAnglePi(camY);
        angle = wrapAnglePi(angle);
        f32 mag = mag0 * cosAnimFIdx(wrappedCamY);
        f32 sn = sinAnimFIdx(angle);
        self->field_0x28 = mag * sn + arg->x;
        self->field_0x30 = mag * cosAnimFIdx(angle) + arg->z;
        self->field_0x184 = angle;
    }
}
// func_8006FFA8: follow-distance output selector. Defaults two output floats,
// then overrides outA from the 0x1D4 flag word: masks 0x40 / 0x20000 / 0x40000
// all map to scaleByGlobal(-d); 0x400 maps to scaleByGlobal(lbl_eu_806662D4).
// d is a named local so MWCC keeps lbl_eu_80666330 in a saved FPR (f31)
// across the scaleByGlobal calls, like retail.
void func_8006FFA8(cf::CfCamFollow* self, float* outA, float* outB) {
    *outB = lbl_eu_80663DD4;
    *outA = lbl_eu_80663DD8;
    f32 d = lbl_eu_80666330;
    if (cfCam_maskUnk1D4((int)self, 0x40) != 0) {
        *outA = scaleByGlobal(-d);
    } else if (cfCam_maskUnk1D4((int)self, 0x20000) != 0) {
        *outA = scaleByGlobal(-d);
    } else if (cfCam_maskUnk1D4((int)self, 0x40000) != 0) {
        *outA = scaleByGlobal(-d);
    } else if (cfCam_maskUnk1D4((int)self, 0x400) != 0) {
        *outA = scaleByGlobal(lbl_eu_806662D4);
    }
}
// func_80070088: follow-heading update. Decays unk1E8 from the stick input,
// nudges unk21C toward 0, then blends field_0x180 (heading) toward the target
// with event/pad-flag overrides and writes the blended vec via cfCam_buildOffset.
void func_80070088(cf::CfCamFollow* self, const ml::CVec3* dir, float input) {
    if (cfCam_testUnk04M(reinterpret_cast<int>(self), 0x800000) != 0) {
        input = lbl_eu_806662DC;
    }
    if (input != lbl_eu_806662DC) {
        // decay rate, doubled under the 0x100 state flag
        f32 rate;
        if (cfCam_maskUnk1D4(reinterpret_cast<int>(self), 0x100) != 0) {
            rate = lbl_eu_80666360 * lbl_eu_80661B44;
        } else {
            rate = lbl_eu_80661B44;
        }
        self->unk1E8 -= input * rate;
        f32 clampLim = lbl_eu_806662D0;
        if (cfCam_isClassicPad(self, input)) {
            clampLim = cfCam_absFloat(&input);
        }
        // clamp the accumulated decay into [-clampLim, clampLim]
        f32 v = self->unk1E8;
        if (v < -clampLim) {
            v = -clampLim;
        } else if (v > clampLim) {
            v = clampLim;
        }
        self->unk1E8 = v;

        if (cfCam_absFloat(&self->unk1E8) > lbl_eu_8066A208) {
            f32 step = lbl_eu_806662D0;
            if (cfCam_testUnk04M(reinterpret_cast<int>(self), 0x10000) == 0 &&
                self->unk21C < lbl_eu_806662BC) {
                step = lbl_eu_806662D0 * lbl_eu_80666308;
            }
            // pull unk21C back toward zero by `step`, clamping at 0
            f32 t = self->unk21C;
            if (t > lbl_eu_806662DC) {
                t -= step;
                self->unk21C = t;
                if (t < lbl_eu_806662DC) {
                    t = lbl_eu_806662DC;
                    self->unk21C = t;
                }
            } else if (!(t >= lbl_eu_806662DC)) {
                t += step;
                self->unk21C = t;
                if (t > lbl_eu_806662DC) {
                    t = lbl_eu_806662DC;
                    self->unk21C = t;
                }
            }
        }
    } else {
        self->unk1E8 *= lbl_eu_80661B48;
    }

    f32 minAng;
    f32 maxAng;
    func_8006FFA8(self, &maxAng, &minAng);
    if (self->field_0x180 > maxAng) {
        self->field_0x180 = maxAng;
    }
    if (self->field_0x180 < minAng) {
        f32 raised = self->field_0x180 + lbl_eu_8066629C;
        self->field_0x180 = raised;
        if (raised > minAng) {
            self->field_0x180 = minAng;
        }
        minAng = self->field_0x180; // retail stores the adjusted value back
    }

    f32 prevAng = self->field_0x180;
    int snap = 0;
    if (cfCam_testUnk04M(reinterpret_cast<int>(self), 0x40000000) != 0 &&
        cfCam_maskUnk1D4(reinterpret_cast<int>(self), 0x200000) == 0) {
        snap = 1;
    }

    if (cfCam_absFloat(&self->unk1E8) > lbl_eu_8066A208) {
        f32 wrapped = wrapAnglePi(self->field_0x180);
        f32 delta = self->unk1E8 * lbl_eu_80661B74;
        if (cfCam_maskUnk1D4(reinterpret_cast<int>(self), 0x40000000) != 0 &&
            cfCam_maskUnk1D4(reinterpret_cast<int>(self), 0x400000) != 0) {
            delta = delta * lbl_eu_8066634C;
        }
        f32 turned = self->field_0x180 + delta;
        if (turned < minAng) {
            turned = minAng;
        } else if (turned > maxAng) {
            turned = maxAng;
        }
        self->field_0x180 = wrapAnglePi(turned);
        if (snap) {
            self->unk1F0 += self->field_0x180 - prevAng;
        }
    } else {
        self->unk1E8 = lbl_eu_806662DC;
    }

    if (cfCam_maskUnk1D4(reinterpret_cast<int>(self), 0x20) == 0) {
        if (snap) {
            f32 d = self->unk1F0 - self->field_0x180;
            self->unk256 = 8;
            self->field_0x180 = self->field_0x180 + lbl_eu_806662F8 * d;
        } else if (cfCam_testUnk04M(reinterpret_cast<int>(self), 0x40000000) != 0) {
            self->unk256 = 8;
        } else if ((s16)self->unk256 > 0) {
            s16 n = (s16)self->unk256 - 1;
            f32 d = self->unk1F8 - self->field_0x180;
            self->unk256 = n;
            self->field_0x180 = self->field_0x180 + lbl_eu_80666364 * d;
            if ((s16)n <= 0) {
                // countdown finished: snap to unk1F8 once both error terms are tiny
                f32 mag23C = self->unk23C - self->unk23C;
                snap = 1;
                if (cfCam_absFloat(&mag23C) > lbl_eu_806662A0) {
                    snap = 0;
                }
                f32 err = self->unk1F8 - self->field_0x180;
                if (cfCam_absFloat(&err) > lbl_eu_806662A0) {
                    snap = 0;
                }
                if (snap) {
                    self->field_0x180 = self->unk1F8;
                    cfCam_andcUnk04(self, 0x800000);
                } else {
                    self->unk256 = self->unk256 + 1;
                }
            }
        }
    }

    f32 ang = self->field_0x180;
    if (ang > lbl_eu_80663DD4) {
        ang = lbl_eu_80663DD4;
    } else if (ang < minAng) {
        ang = minAng;
    }
    f32 heading = wrapAnglePi(ang);

    ml::CVec3 angles;
    ml::CVec3 out;
    copyVec3f(&angles, reinterpret_cast<const ml::CVec3*>(&self->field_0x180));
    angles.x = wrapAnglePi(heading + scaleByGlobal(self->unk21C));

    cfCam_buildOffset(&out, self, dir, reinterpret_cast<const f32*>(&angles), self->unk244);
    copyVec3Words(reinterpret_cast<u8*>(self) + 0x28, &out);
}
// cfCam_rampFollowY: follow-distance ramp. flag != 0 ramps unk1DC up by
// lbl_eu_80666368 toward the lbl_eu_806662F8 cap; flag == 0 decays toward 0
// (snap to 0 when |unk1DC| < epsilon, else step lbl_eu_8066636C toward zero,
// clamped at 0 - >= tests so MWCC emits the cror eq,gt,eq + bne shape).
void cfCam_rampFollowY(cf::CfCamFollow* self, int flag) {
    if (flag != 0) {
        f32 cap;
        f32 inc;
        f32 d;
        f32 sum;
        d = self->unk1DC;
        inc = lbl_eu_80666368;
        cap = lbl_eu_806662F8;
        sum = d + inc;
        self->unk1DC = sum;
        if (sum > cap) {
            self->unk1DC = cap;
        }
    } else if (cfCam_absFloat(&self->unk1DC) < lbl_eu_8066A208) {
        self->unk1DC = lbl_eu_806662DC;
    } else {
        if (self->unk1DC >= lbl_eu_806662DC) {
            self->unk1DC -= lbl_eu_8066636C;
            if (self->unk1DC < lbl_eu_806662DC) {
                self->unk1DC = lbl_eu_806662DC;
            }
        } else {
            self->unk1DC += lbl_eu_8066636C;
            if (self->unk1DC >= lbl_eu_806662DC) {
                self->unk1DC = lbl_eu_806662DC;
            }
        }
    }
}
// cfCam_accumOffset: builds a scaled offset from (src - dst) and accumulates it
// back into dst via the PS add kernel; when flag is set, src[1] is copied
// straight into dst[1] afterwards.
void cfCam_accumOffset(f32* dst, const f32* src, int flag, f32 scale) {
    f32 tmp20[3];
    f32 tmp14[3];
    f32 tmp8[3];
    subVec3f(tmp14, src, dst);
    scaleVec3f(tmp20, tmp14, scale);
    cfCam_psAddVec3(reinterpret_cast<nw4r::math::VEC3*>(tmp8),
                  reinterpret_cast<const nw4r::math::VEC3*>(tmp20),
                  reinterpret_cast<const nw4r::math::VEC3*>(dst));
    copyVec3Words(dst, tmp8);
    if (flag != 0) {
        dst[1] = src[1];
    }
}
// cfCam_aimOnCollide: distance-gated aim adjustment. Computes arg3 - arg2; when its
// squared length is inside (c70, c74] a collision probe (ScnRes_SegQueryForward_E348, mask
// 0x4a03) runs, and on a hit arg3 is replaced: cfCam_seedProbeVec seeds two temp
// vectors, the normalized difference is scaled by 0.1 and accumulated into v1,
// then arg3 is overwritten with v1. Returns whether the probe hit.
int cfCam_aimOnCollide(cf::CfCamFollow* arg1, float* arg2, float* arg3) {
    ml::CVec3 tmp;
    int hit;
    subVec3f(&tmp.x, arg3, arg2);
    f32 lenSq = cfCam_vecLenSq(reinterpret_cast<nw4r::math::VEC3*>(&tmp));
    if (lenSq > lbl_eu_80666370 || lenSq < lbl_eu_80666374) {
        return 0;
    }
    hit = ScnRes_SegQueryForward_E348(arg3, arg2, 0x4a03, 0, 0) != 0;
    if (hit != 0) {
        ml::CVec3 v1;
        ml::CVec3 v2;
        ml::CVec3 v3;
        noopAnimVec3(&v1);
        noopAnimVec3(&v2);
        cfCam_seedProbeVec(&v1, &v2);
        normVec3(&tmp);
        scaleVec3f(&v3.x, &v2.x, lbl_eu_806662A0);
        addVec3f(&v1.x, &v3.x);
        copyVec3Words(arg3, &v1);
    }
    return hit;
}
__declspec(noinline) void cfCam_seedProbeVec(ml::CVec3* a, ml::CVec3* b) {
    func_804BE4B4(a, 0);
    func_804BE4E0(b, 0);
}
__declspec(noinline) void func_800707C0(void* self, void* a, void* b){}
// cfCam_tuneZoomDist: decay/accumulate the camera's +0x23C heading value. When the
// remote pad is absent (cfCam_testUnk04M 0x800000 == 0): the camera-control
// register (func_80275238) decays +0x23C by lbl_eu_80661BA8 clamped at
// lbl_eu_80666308; the movement register (func_80275278) then derives a
// normalized direction (self+0x34 minus self+0x4C), probes it against
// self+0x34 (ScnRes_SegQueryForward_E348, mask 0x4004a03) and - when clear - accumulates
// +0x23C by lbl_eu_80661BA8 clamped at lbl_eu_80666390. The +0x23C value is
// cached in a local so the clamp compares the register value (retail shape).
void cfCam_tuneZoomDist(cf::CfCamFollow* self) {
    if (cfCam_testUnk04M((int)self, 0x800000) != 0) {
        return;
    }
    if (func_80275238(self) != 0) {
        f32 v = self->unk23C;
        f32 r = v - lbl_eu_80661BA8;
        self->unk23C = r;
        if (r < lbl_eu_80666308) {
            self->unk23C = lbl_eu_80666308;
        }
    }
    if (func_80275278(self) != 0) {
        ml::CVec3 tmp1;
        ml::CVec3 tmp3;
        ml::CVec3 tmp2;
        subVec3f(&tmp1.x, reinterpret_cast<const f32*>(&self->unk1C[0x18]),
                      reinterpret_cast<const f32*>(&self->unk1C[0x30]));
        VecMath_NormalizeInPlace(&tmp1);
        scaleVec3f(&tmp2.x, &tmp1.x, lbl_eu_806662D0);
        cfCam_psAddVec3(reinterpret_cast<nw4r::math::VEC3*>(&tmp3),
                      reinterpret_cast<nw4r::math::VEC3*>(&self->unk1C[0x18]),
                      reinterpret_cast<nw4r::math::VEC3*>(&tmp2));
        int hit = ScnRes_SegQueryForward_E348(&tmp3, &self->unk1C[0x18], 0x44a03, 0, 0) != 0;
        if (hit != 0) {
            return;
        }
        f32 v = self->unk23C;
        f32 r = v + lbl_eu_80661BA8;
        self->unk23C = r;
        if (r > lbl_eu_80666390) {
            self->unk23C = lbl_eu_80666390;
        }
    }
}
// cfCam_updateFrame: follow-camera per-frame update (main body). Ramps +0x200 by
// +0x204 toward 1.0 (clearing the 0x800 flag at the cap), slerps between the
// +0x168/+0x174 rotation quats by +0x200, derives the follow-vector set
// (cfCam_getFollowSrc -> subtract/scale/add), drives the +0x244 heading through
// cfCam_dirToAngles / func_8006FFA8 / cfCam_buildOffset, fans the +0x90 vector out to
// the +0x4C/+0x40/+0x58/+0x64 slots, and mirrors the +0x23C/+0x180/+0x184
// values into the shared 0x80663DE4 / 0x80570A38 globals.
void cfCam_updateFrame(cf::CfCamFollow* self) {
    self->unk200 += self->unk204;
    if (self->unk200 >= lbl_eu_806662D0) {
        self->unk200 = lbl_eu_806662D0;
        cfCam_andcUnk04(self, 0x800);
    }
    f32 t = self->unk200;

    // Written by func_80071694, read as a matrix by func_800598A8 - retail
    // allocates the full CMat33 (36B), which sets the caller's frame size.
    ml::CMat33 stackD8;
    ml::CQuat stackC8;
    ml::CQuat stackB8;
    ml::CQuat stackA8;
    nopFunc(&stackD8);
    noopAnimQuat(&stackC8);
    noopAnimQuat(&stackB8);
    stackC8.setRotZXY(reinterpret_cast<const ml::CVec3*>(self->unk168));
    cfCam_normQuat(&stackC8);
    stackB8.setRotZXY(reinterpret_cast<const ml::CVec3*>(self->unk168 + 0xC));
    cfCam_normQuat(&stackB8);
    func_80071398(&stackA8, &stackC8, &stackB8, t);
    cfCam_normQuat(&stackA8);
    func_80071694(&stackD8, &stackA8);

    ml::CVec3 stack9C;
    ml::CVec3 stack90;
    ml::CVec3 stack84;
    ml::CVec3 stack78;
    ml::CQuat stack68;
    ml::CVec3 stack5C;
    ml::CVec3 stack50;
    ml::CVec3 stack44;
    ml::CVec3 stack38;
    ml::CVec3 stack2C;
    ml::CVec3 stack20;
    ml::CVec3 stack14;
    f32 outA;
    f32 outB;
    f32 stack8;

    f32 f31 = self->unk200 * (self->unk20C - self->unk208) + self->unk208;
    cfCam_getFollowSrc(&stack9C, self);
    subVec3f(&stack44.x, &stack9C.x, reinterpret_cast<const f32*>(self->unk168 + 0x30));
    scaleVec3f(&stack50.x, &stack44.x, self->unk200);
    cfCam_psAddVec3(reinterpret_cast<nw4r::math::VEC3*>(&stack90),
                  reinterpret_cast<nw4r::math::VEC3*>(&stack50),
                  reinterpret_cast<nw4r::math::VEC3*>(self->unk168 + 0x30));
    self->cfCam_setFollowD(
        self->unk200 * (lbl_eu_80666300 - self->unk210) + self->unk210);

    noopAnimVec3(&stack84);
    func_800598A8(&stack38, &stackD8,
                  writeVec3f(&stack2C, lbl_eu_806662DC, lbl_eu_806662DC, -f31));
    copyVec3Words(&stack84, &stack38);

    *(f32*)(self->unk1C + 0x0C) = stack90.x - stack84.x;
    *(f32*)(self->unk1C + 0x14) = stack90.z - stack84.z;
    *(f32*)(self->unk1C + 0x10) = stack90.y + stack84.y;
    self->unk1F4 = lbl_eu_806662DC;
    self->unk1DC = lbl_eu_806662DC;
    cfCam_testUnk04M(reinterpret_cast<int>(self), 0x800);

    self->unk244 = self->unk23C + self->unk240;
    stack8 = *(f32*)(self->unk168) - *(f32*)(self->unk168 + 0xC);
    int big = cfCam_absFloat(&stack8) > lbl_eu_8066629C;
    cfCam_dirToAngles(&stack90, reinterpret_cast<const ml::CVec3*>(self->unk1C + 0x0C),
                  reinterpret_cast<f32*>(self->unk168 + 0x18), big, 0, self->unk244);

    copyVec3f(&stack78, reinterpret_cast<const ml::CVec3*>(self->unk168 + 0x18));
    func_8006FFA8(self, &outA, &outB);
    f32 sum = stack78.x + scaleByGlobal(self->unk21C);
    stack78.x = sum;
    if (sum > lbl_eu_80663DD4) {
        stack78.x = lbl_eu_80663DD4;
    } else if (sum < outA) {
        stack78.x = outA;
    }
    stack78.x = wrapAnglePi(stack78.x);

    cfCam_buildOffset(&stack20, self, &stack90, reinterpret_cast<const f32*>(&stack78),
                  self->unk244);
    copyVec3Words(self->unk1C, copyVec3Words(self->unk1C + 0x0C, &stack20));
    cfCam_resetHist(self, &stack90);
    copyVec3Words(self->unk1C + 0x30, &stack90);
    copyVec3Words(self->unk1C + 0x24, &stack90);
    copyVec3Words(self->unk1C + 0x3C, &stack90);
    copyVec3Words(self->unk1C + 0x48, &stack90);
    self->unk228 = lbl_eu_806662A0;
    int r31 = 0;
    if (CfRes_getD80Flag() != 0) {
        cfCam_copyBlock16(&stack68, Scn_QueryUnk80State());
        if (stack68.w >= lbl_eu_8066638C) {
            r31 = 1;
        }
    }
    if (r31 == 0) {
        copyVec3f(&stack5C, reinterpret_cast<const ml::CVec3*>(self->unk1C + 0x0C));
        func_800707C0(self, &stack5C, &stack90);
    } else {
        copyVec3Words(self->unk1C + 0x18, self->unk1C + 0x0C);
    }
    cfCam_psAddVec3(reinterpret_cast<nw4r::math::VEC3*>(&stack14),
                  reinterpret_cast<nw4r::math::VEC3*>(&stack90),
                  reinterpret_cast<nw4r::math::VEC3*>(self->unk168 + 0x60));
    cfCam_applyRelPos(self, self->unk1C + 0x18, &stack14, 0, lbl_eu_80661B50,
                  lbl_eu_806662DC);
    lbl_eu_80663DE4 = self->unk23C;
    lbl_eu_80570A38[0] = *(f32*)(self->unk168 + 0x18);
    lbl_eu_80570A38[1] = *(f32*)(self->unk168 + 0x1C);
}
// cfCam_normQuat: normalize a quaternion in place and hand the pointer back.
// noinline: retail callers keep the `bl cfCam_normQuat` instead of an inlined
// PSQUATNormalize.
__declspec(noinline) Quaternion* cfCam_normQuat(Quaternion* q) {
    PSQUATNormalize(q, q);
    return q;
}
// Tiny helpers MWCC inlines into func_80071398; their by-value Quaternion
// returns keep retail's aggregate-return temporaries (word copy + memberwise
// reload) alive at each call site.
static Quaternion ScaledQuat(const Quaternion* src, float scale) {
    Quaternion ret;
    PSQUATScale(src, &ret, scale);
    // Copy-ctor wrap defeats NRVO so the by-value return temp survives.
    return Quaternion(ret);
}
static Quaternion AddedQuat(const Quaternion* lhs, const Quaternion* rhs) {
    Quaternion sum;
    PSQUATAdd(lhs, rhs, &sum);
    return Quaternion(sum);
}
// func_80071398: quaternion slerp - out = a * sin((1-t)θ)/sin(θ) + q1 *
// sin(tθ)/sin(θ) where θ = acos(clamp(dot(a, b))) and q1 = -b when the dot
// product is negative. θ near zero (|θ| < epsilon) copies q1 straight out.
// The dot is clamped to [-1, 1] with an nw4r Warning on an out-of-range
// input, mirroring the retail assert block.
void func_80071398(Quaternion* out, const Quaternion* a, const Quaternion* b, float t) {
    float dot = PSQUATDotProduct(a, b);
    Quaternion q1;
    Quaternion neg;
    if (dot < lbl_eu_806662DC) {
        // Shortest-path: flip one quaternion and the angle when dot < 0.
        neg.x = -b->x;
        neg.y = -b->y;
        neg.z = -b->z;
        neg.w = -b->w;
        dot = -dot;
        q1 = neg;
    } else {
        q1 = *b;
    }
    if (dot >= lbl_eu_806662D0) {
        dot = lbl_eu_806662D0;
    }
    int valid = 0;
    if (dot <= lbl_eu_806662D0 && dot >= lbl_eu_80666358) {
        valid = 1;
    }
    if (valid == 0) {
        nw4r::db::Warning(lbl_eu_805262F0, 0xef, lbl_eu_805262C8);
    }
    float theta = (float)acos(dot);
    if (fabsf(theta) < lbl_eu_8066A208) {
        // theta ~ 0: quaternions are (anti-)parallel, just copy.
        *out = q1;
    } else {
        float sinTheta = nw4r::math::SinFIdx(lbl_eu_806662E4 * theta);
        float invSin = lbl_eu_806662D0 / sinTheta;
        float w1 = invSin * nw4r::math::SinFIdx(lbl_eu_806662E4 * ((lbl_eu_806662D0 - t) * theta));
        float w2 = invSin * nw4r::math::SinFIdx(lbl_eu_806662E4 * (t * theta));
        Quaternion c1 = ScaledQuat(&q1, w2);
        Quaternion c2 = ScaledQuat(a, w1);
        *out = AddedQuat(&c2, &c1);
    }
}
// func_80071754: follow-camera per-frame update. The cfCam_getBit9_04 gate ramps
// +0x200 by +0x204 and - once it hits 1.0 - clears the 0x8000 pad flag
// (cfCam_andcUnk04). The 0x400 pad flag drives +0x23C toward the
// (1.0 - +0x200) * +0x218 + +0x20C blend, reset to +0x20C at the cap. The
// cfCam_getBit12_04 gate decays +0x21C (multipliers chosen by the 0x1000 flag
// and the |+0x21C| magnitude) and snaps it to 0 when small. With the gate
// again, the +0x168/+0x174 rotation vectors build two quats (setRotZXY +
// normalize), slerped by +0x200 (func_80071398) and applied to +0x70 via
// func_80071694; the +0x23C + +0x240 offset is then written into +0x28,
// +0x244 and +0x1F4, and the result is pushed through cfCam_dirToAngles /
// cfCam_buildOffset before the vtable-slot-0x3C blend update.
void func_80071754(cf::CfCamFollow* self, ml::CVec3* arg2) {
    if (cfCam_getBit9_04(self) != 0) {
        self->unk200 += self->unk204;
        if (self->unk200 >= lbl_eu_806662D0) {
            self->unk200 = lbl_eu_806662D0;
            if (cfCam_testUnk04M(reinterpret_cast<int>(self), 0x8000) != 0) {
                cfCam_andcUnk04(self, 0x8000);
            }
        }
    }
    if (cfCam_testUnk04M(reinterpret_cast<int>(self), 0x400) != 0) {
        self->unk23C = self->unk218 * (lbl_eu_806662D0 - self->unk200) + self->unk20C;
        if (self->unk200 >= lbl_eu_806662D0) {
            cfCam_andcUnk04(self, 0x400);
            self->unk23C = self->unk20C;
        }
    }
    if (cfCam_getBit12_04(self) != 0) {
        if (cfCam_maskUnk1D4(reinterpret_cast<int>(self), 0x1000) != 0) {
            self->unk21C *= lbl_eu_80666318;
        } else if (cfCam_absFloat(&self->unk21C) > lbl_eu_8066636C) {
            self->unk21C *= lbl_eu_806662D8;
        } else {
            self->unk21C *= lbl_eu_806662F0;
        }
        if (cfCam_absFloat(&self->unk21C) < lbl_eu_8066A208) {
            self->unk21C = lbl_eu_806662DC;
            cfCam_andcUnk04(self, 0x1000);
        }
    }
    if (cfCam_getBit9_04(self) != 0) {
        if (self->unk200 >= lbl_eu_806662D0) {
            cfCam_clearBits1200(self);
        }
        ml::CMat33 stack70; // written by func_80071694, read as a matrix by func_800598A8
        ml::CQuat stack60;
        ml::CQuat stack50;
        ml::CQuat stack40;
        ml::CVec3 stack30;
        ml::CVec3 stack24;
        ml::CVec3 stack18;
        ml::CVec3 stackC;
        // Only the x lane is live when cfCam_absFloat reads this temp; retail
        // allocates a single float at sp+0x8 (the dead y/z lanes alias the
        // already-consumed stackC above it).
        f32 stack8;
        f32 t = self->unk200;
        f32 f30 = self->unk23C + self->unk240;
        nopFunc(&stack70);
        noopAnimQuat(&stack60);
        noopAnimQuat(&stack50);
        stack60.setRotZXY(reinterpret_cast<const ml::CVec3*>(self->unk168));
        cfCam_normQuat(&stack60);
        stack50.setRotZXY(reinterpret_cast<const ml::CVec3*>(self->unk168 + 0xC));
        cfCam_normQuat(&stack50);
        func_80071398(&stack40, &stack60, &stack50, t);
        cfCam_normQuat(&stack40);
        func_80071694(&stack70, &stack40);
        noopAnimVec3(&stack30);
        func_800598A8(&stack24, &stack70,
                      writeVec3f(&stack18, lbl_eu_806662DC, lbl_eu_806662DC, -f30));
        copyVec3Words(&stack30, &stack24);
        *(f32*)(self->unk1C + 0x0C) = arg2->x - stack30.x;
        *(f32*)(self->unk1C + 0x14) = arg2->z - stack30.z;
        *(f32*)(self->unk1C + 0x10) = arg2->y + stack30.y;
        self->unk1F4 = lbl_eu_806662DC;
        self->unk244 = f30;
        stack8 = *(f32*)(self->unk168) - *(f32*)(self->unk168 + 0xC);
        int flag = cfCam_absFloat(reinterpret_cast<ml::CVec3*>(&stack8)) > lbl_eu_8066629C;
        // Retail loads self->unk244 into f1 for the trailing (unused) float
        // param of cfCam_dirToAngles, then reloads it for cfCam_buildOffset after the
        // call clobbers f1 - the double lfs at sp+... in retail comes from this.
        cfCam_dirToAngles(arg2, reinterpret_cast<const ml::CVec3*>(self->unk1C + 0x0C),
                      reinterpret_cast<f32*>(self->unk168 + 0x18), flag, 0,
                      self->unk244);
        cfCam_buildOffset(&stackC, self, arg2, reinterpret_cast<const f32*>(self->unk168 + 0x18),
                      self->unk244);
        copyVec3Words(self->unk1C + 0x0C, &stackC);
        f32 v = self->unk200 * (lbl_eu_80666300 - self->unk210) + self->unk210;
        self->cfCam_setFollowD(v);
    }
}
// In-TU definition (bottom of file); noinline keeps the retail bl from
// cfCam_applyRelPos.
extern "C" __declspec(noinline) bool cfCam_set1E0Check(cf::CfCamFollow* self, float val);
// cfCam_applyRelPos: camera-relative position update. Copies src into the +0x1C
// vec3, accumulates the +0x1DC follow distance into its y (offset 0x20),
// mirrors the +0x180/+0x184 floats into the shared lbl_eu_80570A38 vec3, then
// - when the sibling gate cfCam_getSignBit04 passes - applies f2 through
// func_8049EFF8 on the follow target handle (+0x164) and stores f1 into the
// target's +0x1E0 via cfCam_set1E0Check.
// noinline: retail cfCam_updateFrame keeps a `bl cfCam_applyRelPos` instead of the
// inlined body (which would also drag its tail globals into the caller).
__declspec(noinline) void cfCam_applyRelPos(cf::CfCamFollow* self, void* src, void* arg, int c, float f1,
                                        float f2) {
    copyVec3Words(self->unk1C, src);
    *(f32*)(self->unk1C + 4) += self->unk1DC;
    lbl_eu_80570A38[0] = *(f32*)(self->unk168 + 0x18);
    lbl_eu_80570A38[1] = *(f32*)(self->unk168 + 0x1C);
    if (cfCam_getSignBit04(self) != 0) {
        func_8049EFF8(self->unk160, f2, self->unk1C, arg);
        cfCam_set1E0Check(static_cast<cf::CfCamFollow*>(self->unk160), f1);
    }
}
// cfCam_probeCamPos: camera-position update. Probes arg1 against arg2
// (ScnRes_SegQueryForward_E348, mask arg3); on a hit the shared 0x80570A50 / 0x80570A5C
// vec3s are seeded by cfCam_seedProbeVec. Then the per-frame object
// (cfCam_getActiveCam) is enumerated with func_804B5088(obj, arg1, arg2, 5, 0);
// when it returns non-null, v1/v2 snapshot the object's position/camera
// slots (cfCam_getPlus60 / cfCam_getPlus6C) and - if the probe hit and the new
// distance (cfCam_distSqVec3 squared length) is closer than the stored one -
// the shared vec3s are updated. Returns whether the probe hit or the
// enumeration produced a result.
int cfCam_probeCamPos(ml::CVec3* arg1, ml::CVec3* arg2, int arg3) {
    int ret = ScnRes_SegQueryForward_E348(arg1, arg2, arg3, 0, 0);
    int probe = (int)(((unsigned int)(-(int)ret) | (unsigned int)ret) >> 31);
    if (probe != 0) {
        cfCam_seedProbeVec(reinterpret_cast<ml::CVec3*>(lbl_eu_80570A50),
                      reinterpret_cast<ml::CVec3*>(lbl_eu_80570A5C));
    }
    cf::CfCamFollow* obj = cfCam_getActiveCam();
    int r = func_804B5088(obj, arg1, arg2, 5, 0);
    if (r != 0) {
        ml::CVec3 v1;
        ml::CVec3 v2;
        noopAnimVec3(&v1);
        noopAnimVec3(&v2);
        copyVec3Words(&v1, cfCam_getPlus60(cfCam_getActiveCam()));
        copyVec3Words(&v2, cfCam_getPlus6C(cfCam_getActiveCam()));
        if (probe != 0) {
            f32 f = cfCam_distSqVec3(reinterpret_cast<const nw4r::math::VEC3*>(arg1),
                                  reinterpret_cast<const nw4r::math::VEC3*>(lbl_eu_80570A50));
            if (cfCam_distSqVec3(reinterpret_cast<const nw4r::math::VEC3*>(arg1),
                              reinterpret_cast<const nw4r::math::VEC3*>(&v1)) < f) {
                copyVec3Words(lbl_eu_80570A50, &v1);
                copyVec3Words(lbl_eu_80570A5C, &v2);
            }
        } else {
            copyVec3Words(lbl_eu_80570A50, &v1);
            copyVec3Words(lbl_eu_80570A5C, &v2);
        }
    }
    int result = 0;
    if (probe != 0 || r != 0) {
        result = 1;
    }
    return result;
}
int func_80071D50(ml::CVec3* out, u32 probeArg, float f) {
    // ScnRes_VertRayForward_E398's retail ABI here passes only four GPR args.
    typedef int (*BE398Fn4)(void*, u32, u32, u32);
    int probe = ((BE398Fn4)ScnRes_VertRayForward_E398)(out, probeArg, 0, 0);
    if (probe != 0) {
        cfCam_seedProbeVec(reinterpret_cast<ml::CVec3*>(lbl_eu_80570A50),
                      reinterpret_cast<ml::CVec3*>(lbl_eu_80570A5C));
    }
    ml::CVec3 tmp;
    copyVec3f(&tmp, out);
    tmp.y += f;
    cf::CfCamFollow* cam = cfCam_getActiveCam();
    int hit = func_804B5088(cam, out, &tmp, 5, 0);
    if (hit != 0) {
        ml::CVec3 posA;
        ml::CVec3 posB;
        noopAnimVec3(&posA);
        noopAnimVec3(&posB);
        copyVec3Words(&posA, cfCam_getPlus60(cfCam_getActiveCam()));
        copyVec3Words(&posB, cfCam_getPlus6C(cfCam_getActiveCam()));
        if (probe != 0) {
            // keep the shared globals only when the new camera is closer
            f32 best = cfCam_distSqVec3(reinterpret_cast<const nw4r::math::VEC3*>(out),
                                     reinterpret_cast<const nw4r::math::VEC3*>(lbl_eu_80570A50));
            f32 d = cfCam_distSqVec3(reinterpret_cast<const nw4r::math::VEC3*>(out),
                                  reinterpret_cast<const nw4r::math::VEC3*>(&posA));
            if (d < best) {
                copyVec3Words(lbl_eu_80570A50, &posA);
                copyVec3Words(lbl_eu_80570A5C, &posB);
            }
        } else {
            copyVec3Words(lbl_eu_80570A50, &posA);
            copyVec3Words(lbl_eu_80570A5C, &posB);
        }
    }
    return probe != 0 || hit != 0 ? 1 : 0;
}
// cfCam_copyProbeHit: copy the two shared camera-relative position globals
// (lbl_eu_80570A50 / lbl_eu_80570A5C) into the caller's vectors.
void cfCam_copyProbeHit(ml::CVec3* a, ml::CVec3* b) {
    copyVec3Words(a, lbl_eu_80570A50);
    copyVec3Words(b, lbl_eu_80570A5C);
}
// cfCam_canAimFree: usable when neither the 0x40000 pad-state bit nor the
// 0x10000000 follow-cam flag bit is set.
extern "C" int cfCam_canAimFree(cf::CfCamFollow* self) {
    int result = 0;
    if (cfCam_testUnk04M((int)self, 0x40000) == 0 && cfCam_maskUnk1D4((int)self, 0x10000000) == 0) {
        result = 1;
    }
    return result;
}
// func_80071F74: scale-and-clamp of *out driven by a*c. Small steps just
// apply the lbl_eu_806662F8 gain; larger steps pick a division ratio from
// distance bands, then blend *out toward that ratio while keeping its sign
// relative to b and clamping magnitude to |b|.
// b arrives as a double (retail passes it in f2 after promoting); frsp at the
// multiply confirms the widened parameter.
void func_80071F74(f32* out, double a, f32 b, f32 c) {
    f32 ratio;
    f32 t;
    if (c > lbl_eu_80666308) {
        c = lbl_eu_80666308;
    }
    t = a * c;
    if (cfCam_absFloat(&t) <= lbl_eu_806662A0) {
        *out = lbl_eu_806662F8 * t;
    } else {
        f32 g = lbl_eu_806662F8;
        f32 mag = cfCam_absFloat(&t);
        int overCap = mag >= lbl_eu_806662C8;
        if (overCap) {
            g = lbl_eu_80666354;
            ratio = t / lbl_eu_80666390;
        } else if (mag >= lbl_eu_80666330) {
            g = lbl_eu_80666304;
            ratio = t / lbl_eu_806662BC;
        } else if (mag >= lbl_eu_80666334) {
            g = lbl_eu_80666304;
            ratio = t / lbl_eu_806662C0;
        } else {
            ratio = t / lbl_eu_80666300;
        }
        f32 magRatio = cfCam_absFloat(&ratio);
        f32 magOut = cfCam_absFloat(out);
        if (b * *out < lbl_eu_806662DC) {
            ratio = t / lbl_eu_806662C0;
            magRatio = cfCam_absFloat(&ratio);
            *out = -*out;
            if (g < lbl_eu_80666304) {
                g = lbl_eu_80666304;
            }
        }
        if (magOut < lbl_eu_8066629C) {
            *out = ratio;
        } else if (magRatio > magOut) {
            *out = ratio + g * (ratio - *out);
        }
    }
    if (b * *out < lbl_eu_806662DC) {
        *out = -*out;
    }
    if (cfCam_absFloat(out) > cfCam_absFloat(&b)) {
        *out = b;
    }
}
__declspec(noinline) void func_80072194(cf::CfCamFollow* self, void* arg){}
// cfCam_buildOffset: follow-camera offset vector builder. With the 0x200 script
// gate set (nonzero), out just copies the +0x34 position. Otherwise dir is blended
// toward self+0x28 by a yaw-scaled normalized XZ direction, lifted by the
// pitch sine term clamped by the sign-gated constants.
__declspec(noinline) void cfCam_buildOffset(ml::CVec3* out, cf::CfCamFollow* self,
                                        const ml::CVec3* dir, const f32* angles,
                                        float f) {
    if (isGlobalCamFlagSet(0x200) != 0) {
        copyVec3f(out, reinterpret_cast<const ml::CVec3*>(self->unk1C + 24));
        return;
    }
    f32 lift = f * sinAnimFIdx(angles[0]);
    f32 push = f * cosAnimFIdx(angles[0]);
    if (lbl_eu_80663DE1 != 0) {
        if (angles[0] < lbl_eu_806662DC) {
            lift = lbl_eu_80666318 * sinAnimFIdx(angles[0]);
        } else {
            lift *= lbl_eu_806662CC;
        }
    } else {
        if (angles[0] < lbl_eu_806662DC) {
            lift *= lbl_eu_80661B38;
        } else {
            lift *= lbl_eu_806662CC;
        }
    }
    ml::CVec3 blend;
    subVec3f(&blend.x, reinterpret_cast<const f32*>(&self->unk1C[12]),
                  reinterpret_cast<const f32*>(dir));
    blend.y = lbl_eu_806662DC;
    VecMath_NormalizeInPlace(&blend);
    ml::CVec3 res;
    noopAnimVec3(&res);
    res.x = blend.x * push + dir->x;
    res.z = blend.z * push + dir->z;
    res.y = dir->y + lift;
    copyVec3f(out, &res);
}
// func_80074090: pick a follow aim vector. src == 0 copies pos verbatim.
// Otherwise the source's aim body (slot 0xAC) seeds the working vector and
// sel picks a replacement: >0 uses the indexed position sample
// (CfObject_getPosSample, slot 0x12C); <0 indexes the lbl_eu_80527244
// joint-name table into the named node matrix slot (CfObject_findNodeMatrix,
// slot 0x120); sel == 0 probes pos (cfCam_isVecNear0) and falls back to the
// sub-object position word (CfObject_getSubPosWord, slot 0x128). The chosen
// vector feeds the scaled camera basis transform (cfCam_applyBasis) using the
// slot-0xCC scale factor.
extern "C" void func_80074090(ml::CVec3* out, cf::CfObject* src, ml::CVec3* pos, int sel) {
    if (src == 0) {
        copyVec3f(out, pos);
        return;
    }
    nw4r::math::MTX33 pad; // retail reserves this unused stack space
    nopFunc(&pad);
    ml::CVec3 cur;
    copyVec3f(&cur, src->CfObject_getPosVector());
    if (sel > 0) {
        ml::CVec3 v;
        cfCam_extractTrans(&v.x, reinterpret_cast<const float*>(src->CfObject_getPosSample(sel)));
        copyVec3Words(&cur, &v);
    } else if (sel < 0) {
        ml::CVec3 v;
        cfCam_extractTrans(&v.x, reinterpret_cast<const float*>(
                                 src->CfObject_findNodeMatrix(
                                     reinterpret_cast<const char*>(lbl_eu_80527244[-sel]))));
        copyVec3Words(&cur, &v);
    } else {
        if (cfCam_isVecNear0(reinterpret_cast<void*>(pos)) != 0) {
            const void* p = reinterpret_cast<const void*>(src->CfObject_getSubPosWord());
            if (p != 0) {
                ml::CVec3 v;
                cfCam_extractTrans(&v.x, static_cast<const float*>(p));
                copyVec3Words(&cur, &v);
            }
        }
    }
    cfCam_applyBasis(reinterpret_cast<nw4r::math::VEC3*>(out),
                  reinterpret_cast<nw4r::math::VEC3*>(&cur),
                  reinterpret_cast<nw4r::math::VEC3*>(pos), src->CfObject_getMoveHeadAngle());
}
// cfCam_groundReset: collision-adjusted camera reset helper. Probes arg3 against
// a copy of itself lowered by lbl_eu_80661B58 (ScnRes_SegQueryForward_E348, mask
// 0x40000). On a hit: v1/v2 are seeded by cfCam_seedProbeVec, arg4 takes v1 and
// its y bumps by lbl_eu_80661B58, the 0x80570A74 / 0x80570A80 globals are
// refreshed, and when the CfGameManager state gate (8) is set the 0x90/0x120
// message at lbl_eu_804FB4F0+0x92 is emitted. Otherwise arg4 just copies
// arg2. The 0x4000 flag is set on arg1 via cfCam_setClear1D4 with the hit bit.
int cfCam_groundReset(cf::CfCamFollow* arg1, ml::CVec3* arg2, ml::CVec3* arg3,
                  ml::CVec3* arg4) {
    int result = 0;
    ml::CVec3 tmp;
    copyVec3f(&tmp, arg2);
    tmp.y = tmp.y - lbl_eu_80661B58;
    if (ScnRes_SegQueryForward_E348(arg3, &tmp, 0, 0x40000, 0) != 0) {
        ml::CVec3 v1;
        ml::CVec3 v2;
        noopAnimVec3(&v1);
        noopAnimVec3(&v2);
        cfCam_seedProbeVec(&v1, &v2);
        copyVec3Words(arg4, &v1);
        arg4->y = arg4->y + lbl_eu_80661B58;
        copyVec3Words(lbl_eu_80570A74, arg4);
        copyVec3Words(lbl_eu_80570A80, &v2);
        result = 1;
        if (getNullPtrC__Q22cf13CfGameManagerFv(8) != 0) {
            CmText_VariadicNoop(0x90, 0x120, lbl_eu_804FB4F0 + 0x92);
        }
    } else {
        copyVec3Words(arg4, arg2);
    }
    cfCam_setClear1D4(arg1, 0x4000, result);
    return result;
}
// func_80073640: follow-camera ground correction. Probes arg2 against the
// copied target (ScnRes_SegQueryForward_E348, mask 0x4a03); on a hit it scans the probe
// result list (ScnRes_GetEntryPtr_E50C/520), picks blended positions, and rewrites the
// out vector via copyVec3Words. Later blocks handle the 0x40-flag snap and
// the final downward ground clamp (ScnRes_VertRayForward_E398).
int func_80073640(cf::CfCamFollow* self, ml::CVec3* arg2, ml::CVec3* out) {
    ml::CVec3 copyE0;   // 0xE0: verbatim copy of out
    ml::CVec3 vD4;      // 0xD4: seeded by cfCam_seedProbeVec / follow-source pos
    ml::CVec3 vC8;      // 0xC8: seeded by cfCam_seedProbeVec / follow-source aim
    ml::CVec3 vBC;      // 0xBC: working blend result fed to func_804B5088
    ml::CVec3 dirB0;    // 0xB0
    ml::CVec3 snapA4;   // 0xA4: 0x40-flag snap source
    ml::CVec3 snap98;   // 0x98: 0x40-flag snap ground point
    ml::CVec3 clamp8C;  // 0x8C
    ml::CVec3 clamp80;  // 0x80: final downward-clamp ground point
    ml::CVec3 diff74;   // 0x74
    ml::CVec3 diff68;   // 0x68
    ml::CVec3 tmp5C;    // 0x5C
    ml::CVec3 tmp50;    // 0x50
    ml::CVec3 tmp44;    // 0x44
    ml::CVec3 tmp38;    // 0x38
    ml::CVec3 tmp2C;    // 0x2C
    ml::CVec3 tmp20;    // 0x20
    ml::CVec3 tmp14;    // 0x14
    ml::CVec3 tmp8;     // 0x08

    cfCam_andcUnk04(self, 0x80000);
    copyVec3f(&copyE0, out);
    noopAnimVec3(&vD4);
    noopAnimVec3(&vC8);
    copyVec3f(&vBC, out);
    f32 dy = out->y - arg2->y;
    int gt = dy > lbl_eu_806662DC ? 1 : 0;
    // (retail neg/or/srwi idiom)
    int probe = ScnRes_SegQueryForward_E348(arg2, &copyE0, 0x4a03, 0, 0);
    int hit = (int)(((unsigned int)(-(int)probe) | (unsigned int)probe) >> 31);
    int useBlend = 1;
    int allowSnap = 0;
    int result = 0;

    if (hit != 0) {
        cfCam_seedProbeVec(&vD4, &vC8);
        ScnRes_SegQueryForward_E348(arg2, &copyE0, 0x4a03, 0, 1);
        int count = ScnRes_GetEntryCount_E4AC();
        int rowId = 0x80;
        for (int i = 0; i < count; i++) {
            ml::CVec3* pos = ScnRes_GetEntryPtr_E50C(i);
            ml::CVec3* info = ScnRes_GetEntryHead2_E520(i);
            if (gt != 0 && info->y < lbl_eu_806663CC) {
                useBlend = 0;
            }
            subVec3f(&diff74.x, &pos->x, &arg2->x);
            f32 distA = cfCam_vecLength(&diff74);
            subVec3f(&diff68.x, &pos->x, &copyE0.x);
            f32 distB = cfCam_vecLength(&diff68);
            if (getNullPtrC__Q22cf13CfGameManagerFv(8) != 0) {
                CmText_VariadicNoop(0x30, rowId, lbl_eu_804FB4F0 + 0x98, pos->x,
                              pos->y, pos->z, info->x, info->y, info->z, distA,
                              distB);
            }
            rowId += 0x10;
        }
    }

    if (hit != 0 && cfCam_maskUnk1D4(reinterpret_cast<int>(self), 0x4040) != 0) {
        if (ScnRes_SegQueryForward_E348(arg2, &copyE0, 0x404a03, 0, 0) == 0
            && self->field_0x180 >= lbl_eu_806662DC) {
            void* actor = func_800BBC0C(self->unk164);
            if (actor != 0 && cfCam_loadUnkC4(self->unk164) != 0) {
                cfCam_loadUnkC4(self->unk164);
                cfCam_getFloat4F8(actor);
            }
            if (out->y > vD4.y + lbl_eu_80661B54) {
                hit = 0;
            } else if (cfCam_maskUnk1D4(reinterpret_cast<int>(self), 0x10002)
                       == 0) {
                hit = 0;
            }
        }
    }

    if (hit != 0) {
        scaleVec3f(&tmp50.x, &vC8.x, lbl_eu_80661B54);
        cfCam_psAddVec3(reinterpret_cast<nw4r::math::VEC3*>(&tmp5C),
                      reinterpret_cast<const nw4r::math::VEC3*>(&vD4),
                      reinterpret_cast<const nw4r::math::VEC3*>(&tmp50));
        copyVec3Words(&vBC, &tmp5C);
    }

    cf::CfCamFollow* cam = cfCam_getActiveCam();
    int enumRet = func_804B5088(cam, arg2, &vBC, 5, 0);
    f32 scale = lbl_eu_80661B54;
    if (hit != 0 || enumRet != 0) {
        if (enumRet != 0) {
            copyVec3Words(&vD4, cfCam_getPlus60(cfCam_getActiveCam()));
            copyVec3Words(&vC8, cfCam_getPlus6C(cfCam_getActiveCam()));
            useBlend = 1;
        }
        if (useBlend != 0) {
            scaleVec3f(&tmp38.x, &vC8.x, lbl_eu_80661B54);
            cfCam_psAddVec3(reinterpret_cast<nw4r::math::VEC3*>(&tmp44),
                          reinterpret_cast<const nw4r::math::VEC3*>(&vD4),
                          reinterpret_cast<const nw4r::math::VEC3*>(&tmp38));
            copyVec3Words(out, &tmp44);
        } else {
            // No usable blend source: fall back to the normalized
            // arg2 - copy direction scaled by lbl_eu_80661B54.
            subVec3f(&dirB0.x, &arg2->x, &copyE0.x);
            VecMath_NormalizeInPlace(&dirB0);
            scaleVec3f(&tmp20.x, &dirB0.x, lbl_eu_80661B54);
            cfCam_psAddVec3(reinterpret_cast<nw4r::math::VEC3*>(&tmp2C),
                          reinterpret_cast<const nw4r::math::VEC3*>(&vD4),
                          reinterpret_cast<const nw4r::math::VEC3*>(&tmp20));
            copyVec3Words(out, &tmp2C);
        }
        result = 1;
        cfCam_setClear04(self, 8, 1);
        if (cfCam_maskUnk1D4(reinterpret_cast<int>(self), 0x100) != 0
            && cfCam_maskUnk1D4(reinterpret_cast<int>(self), 0x4000) != 0) {
            allowSnap = 1;
        }
    }

    if (hit != 0 || allowSnap != 0) {
        if (ScnRes_RegPassSelect_E2E8(out, 0x404a03, 0, 1) != 0) {
            noopAnimVec3(&snapA4);
            noopAnimVec3(&snap98);
            func_804BE4B4(&snapA4, 0);
            func_804BE4E0(&snap98, 0);
            int ok = 1;
            if (cfCam_maskUnk1D4(reinterpret_cast<int>(self), 0x40) == 0) {
                if (gt != 0) {
                    if (snap98.y < lbl_eu_806663D0) {
                        ok = 0;
                    }
                } else if (ScnRes_GetEntryCount_E4AC() >= 2
                           && cfCam_absFloat(&snap98) < lbl_eu_806663D4) {
                    ok = 0;
                }
            }
            if (ok != 0) {
                scaleVec3f(&tmp8.x, &snap98.x, scale);
                cfCam_psAddVec3(reinterpret_cast<nw4r::math::VEC3*>(&tmp14),
                              reinterpret_cast<const nw4r::math::VEC3*>(&snapA4),
                              reinterpret_cast<const nw4r::math::VEC3*>(&tmp8));
                copyVec3Words(out, &tmp14);
                result = 1;
                if (getNullPtrC__Q22cf13CfGameManagerFv(8) != 0) {
                    CmText_VariadicNoop(0x10, 0x170, lbl_eu_804FB4F0 + 0xc6,
                                  snap98.x, snap98.y, snap98.z);
                }
            }
        }
    }

    // Final downward ground clamp: lower the out height when the probe
    // reports ground above it.
    f32 lift = lbl_eu_80661B54 - lbl_eu_806663BC;
    if (ScnRes_VertRayForward_E398(out, 0x404a03, 0, 0, -lift, lbl_eu_8066AF20) != 0) {
        noopAnimVec3(&clamp8C);
        noopAnimVec3(&clamp80);
        func_804BE4B4(&clamp8C, 0);
        func_804BE4E0(&clamp80, 0);
        if (clamp80.y > lbl_eu_80666318 && clamp80.z + lift > out->y) {
            out->y = clamp80.z + lift;
        }
    }
    return result;
}
// func_80073D8C: compute a follow-parameter vec3 via func_80074090 and
// propagate it through the camera body: the vec3 at +0x28 (position copy)
// gets the fresh value, the vec3 at +0x34 (source pos) mirrors it.
// copyVec3Words returns its dst, feeding the second copy's source.
void func_80073D8C(cf::CfCamFollow* self, int cond) {
    if (cond != 0) {
        ml::CVec3 local;
        // full 4-arg retail shape; this caller only needs the out vector
        func_80074090(&local, 0, &local, 0);
        copyVec3Words(self->unk1C + 24, copyVec3Words(self->unk1C + 12, &local));
    }
}
// cfCam_copyCamState: dynamic_cast the source object to the camera-state type and,
// on success, copy its state block (positions + the 0x1E0 float) into the
// follow camera. The final copy reuses the +0x40 vector as the source for
// self's +0x10 slot.
void cfCam_copyCamState(cf::CfCamFollow* self, cf::CfObject* src) {
    cf::CfCamStateView* state = reinterpret_cast<cf::CfCamStateView*>(
        __dynamic_cast(src, 0, &lbl_eu_80661B28, &lbl_eu_80661B30, 0));
    if (state != NULL) {
        cf::CfCamStateView* dst = reinterpret_cast<cf::CfCamStateView*>(self);
        copyVec3Words(&dst->field34, &state->field34);
        copyVec3Words(&dst->field28, &state->field28);
        copyVec3Words(&dst->field58, &state->field58);
        copyVec3Words(&dst->field40, &state->field40);
        dst->field1E0 = state->field1E0;
        copyVec3Words(&dst->field10, &state->field40);
    }
}
// cfCam_offsetFromDir: build the follow-camera offset vector from a direction
// vec3 `d` and a distance `f`. The result temp starts zeroed, then:
//   tmp[0] = (f*cos(d[0])) * -sin(d[4]);  tmp[2] = (f*cos(d[0])) * -cos(d[4])
//   tmp[1] = f*sin(d[0]), clamped by sign: >= 0 scaled by 0x806662CC;
//   negative values scale by 0x80661B38, or by 0x80666318*sin(d[0]) when the
//   0x80663DE1 gate is set. `c` + tmp is summed into `a` (cfCam_psAddVec3).
void cfCam_offsetFromDir(ml::CVec3* a, cf::CfCamFollow* b, ml::CVec3* c, const f32* d, float f) {
    f32 tmp[3];
    noopAnimVec3(tmp);
    f32 s = f * cosAnimFIdx(d[0]);
    tmp[0] = s * -sinAnimFIdx(d[1]);
    tmp[2] = s * -cosAnimFIdx(d[1]);
    f32 t = f * sinAnimFIdx(d[0]);
    tmp[1] = t;
    if (t >= lbl_eu_806662DC) {
        tmp[1] = t * lbl_eu_806662CC;
    } else if (lbl_eu_80663DE1 != 0) {
        tmp[1] = sinAnimFIdx(d[0]) * lbl_eu_80666318;
    } else {
        tmp[1] = t * lbl_eu_80661B38;
    }
    cfCam_psAddVec3(reinterpret_cast<nw4r::math::VEC3*>(a),
                  reinterpret_cast<const nw4r::math::VEC3*>(c),
                  reinterpret_cast<const nw4r::math::VEC3*>(tmp));
}
// cfCam_dirToAngles: direction/angle follow adjustment. tmp = arg2 - arg1 (XZ
// plane work), len = XZ length (cfCam_xzLength), out[1] = yaw(tmp). Then
// atan2AnimFIdx fills out[0] = sin-scale with the scale/span chosen by the
// 0x80663DE1 gate, flag2 and flag3:
//   gate && flag2: tmp[1] >= 0 -> scale 0x80666318, span len; else scale
//   0x80663DD0, span 1.0
//   !gate && flag2: flag3 == 0 additionally scales tmp[1] by 0x80666318,
//   span is always len
void cfCam_dirToAngles(const ml::CVec3* arg1, const ml::CVec3* arg2, f32* out, int flag2, int flag3,
                   f32 heading) {
    // heading is an unused 6th parameter: retail callers load self->unk244 into
    // f1 before calling, but the body never reads it.
    (void)heading;
    f32 tmp[3];
    subVec3f(tmp, reinterpret_cast<const f32*>(arg2), reinterpret_cast<const f32*>(arg1));
    f32 len = cfCam_xzLength(tmp);
    out[1] = cfCam_yawFromVec(static_cast<const void*>(tmp));
    if (lbl_eu_80663DE1 != 0) {
        if (flag2 != 0) {
            if (tmp[1] >= lbl_eu_806662DC) {
                tmp[1] *= lbl_eu_80666318;
                out[0] = atan2AnimFIdx(tmp[1], len);
            } else {
                tmp[1] *= lbl_eu_80663DD0;
                out[0] = atan2AnimFIdx(tmp[1], lbl_eu_806662D0);
            }
        }
    } else if (flag2 != 0) {
        if (flag3 != 0) {
            out[0] = atan2AnimFIdx(tmp[1], len);
        } else {
            tmp[1] *= lbl_eu_80666318;
            out[0] = atan2AnimFIdx(tmp[1], len);
        }
    }
}
// cfCam_xzLength: XZ-plane length sqrt (len2 = x*x + z*z) with the nw4r FSqrt
// assert path (Warning on negative/NaN squared length) and 0 for non-positive
// len2. Written x*x + z*z so MWCC computes the fmadds addend (z*z) first and
// loads z (+8) before x (+0), matching retail.
float cfCam_xzLength(const ml::CVec3* v) {
    float len2 = v->x * v->x + v->z * v->z;
    if (!(len2 >= lbl_eu_806662DC)) {
        nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
    }
    return len2 <= lbl_eu_806662DC ? lbl_eu_806662DC : len2 * nw4r::math::FrSqrt(len2);
}
// cfCam_applyBasis: build a follow-parameter vec3 - init a stack temp, scale it
// by the float arg via func_80059610, derive the follow offset with
// func_800598A8, then hand the result to cfCam_psAddVec3 (PS kernel in the .inl).
// noinline keeps the retail `bl cfCam_applyBasis` inside func_80074090.
__declspec(noinline) void cfCam_applyBasis(nw4r::math::VEC3* a, nw4r::math::VEC3* b, nw4r::math::VEC3* c, float f) {
    nw4r::math::VEC3 tmp2;
    nw4r::math::MTX33 tmp1; // larger stack slot - retail reserves extra frame space here
    nopFunc(&tmp1);
    func_80059610(&tmp1, f);
    func_800598A8(&tmp2, &tmp1, c);
    cfCam_psAddVec3(a, b, &tmp2);
}
// cfCam_makeYRotMtx: build a Y-rotation 3x4 matrix scaled by the angle argument.
// The lbl_eu_806662E4 factor converts the angle to the fixed-point index
// expected by the nw4r SinFIdx/CosFIdx kernels; translation column stays zero.
// Returns mtx like the other nw4r-style matrix builders.
float* cfCam_makeYRotMtx(float* mtx, float scale) {
    // Constant columns first: lets MWCC issue the SDA loads with the prologue
    // cluster and sink the stores after the trig calls like retail.
    float sinV = nw4r::math::SinFIdx(lbl_eu_806662E4 * scale);
    float cosV = nw4r::math::CosFIdx(lbl_eu_806662E4 * scale);
    mtx[0] = cosV;
    mtx[1] = lbl_eu_806662DC;
    mtx[2] = sinV;
    mtx[3] = lbl_eu_806662DC;
    mtx[4] = lbl_eu_806662DC;
    mtx[5] = lbl_eu_806662D0;
    mtx[6] = lbl_eu_806662DC;
    mtx[7] = lbl_eu_806662DC;
    mtx[8] = -sinV;
    mtx[9] = lbl_eu_806662DC;
    mtx[10] = cosV;
    mtx[11] = lbl_eu_806662DC;
    return mtx;
}
// func_80074230: transform two aim vectors by the inverse camera matrix of a
// camera-position source. Builds an MTX34 on the stack, scales it by the
// source's vtable-slot-0xCC factor (cfCam_makeYRotMtx), stamps the slot-0xAC aim
// vector into the translation column (cfCam_setTransCol), inverts in place
// (func_800743C0), then maps dst1/dst2 through it via cfCam_multMtxVec + copy
// back. The source pointer is a cf::CfObject (slots 0xCC/0xAC).
void func_80074230(cf::CfObject* src, ml::CVec3* dst1, ml::CVec3* dst2) {
    if (src == 0) {
        return;
    }
    nw4r::math::MTX34 mtx;
    cfCam_nopCtorSlot(&mtx);
    cfCam_makeYRotMtx(mtx.a, src->CfObject_getMoveHeadAngle());
    cfCam_setTransCol(mtx.a, reinterpret_cast<const float*>(src->CfObject_getPosVector()));
    func_800743C0(static_cast<void*>(&mtx));
    ml::CVec3 tmp1;
    cfCam_multMtxVec(&tmp1, mtx.mtx, dst1);
    copyVec3Words(dst1, &tmp1);
    ml::CVec3 tmp2;
    cfCam_multMtxVec(&tmp2, mtx.mtx, dst2);
    copyVec3Words(dst2, &tmp2);
}
// cfCam_posFromLookat: compute the base camera position for a direction camera.
// Builds a direction frame from `dir` (func_80074534), clamps the speed
// parameter against the .sdata threshold (floor: lbl_eu_80661B60), packs a
// (0, 0, speed) offset via writeVec3f, transforms it into world space
// (func_800598A8) and adds it to the lookat point (cfCam_psAddVec3).
__declspec(noinline) void cfCam_posFromLookat(ml::CVec3* out, ml::CVec3* lookat, const ml::CVec3* dir, f32 speed) {
    ml::CMat33 dirFrame;
    ml::CVec3 scaled;
    ml::CVec3 offset;

    nopFunc(&dirFrame);
    func_80074534(&dirFrame, dir);

    // Clamp speed to its configured minimum.
    if (speed < lbl_eu_80661B64) {
        speed = lbl_eu_80661B60;
    }

    void* packed = writeVec3f(&offset, lbl_eu_806662DC, lbl_eu_806662DC, speed);
    func_800598A8(&scaled, &dirFrame, packed);
    cfCam_psAddVec3(reinterpret_cast<nw4r::math::VEC3*>(out),
                  reinterpret_cast<const nw4r::math::VEC3*>(lookat),
                  reinterpret_cast<const nw4r::math::VEC3*>(&scaled));
}
// func_80074534: direction frame builder. Builds the base block
// [c,-s,A / s,c,A / A,A,B] from sin/cos of (K * dir->z), then twice
// left-multiplies the running block by an angle matrix whose junk row/column
// entries come from the .sdata constants (X-step then Y-step). Each step is
// evaluated bottom row first, matching retail's product ordering.
// noinline keeps the retail `bl func_80074534` inside cfCam_posFromLookat; C linkage
// emits the flat retail symbol name.
extern "C" __declspec(noinline) void func_80074534(ml::CMat33* out, const ml::CVec3* dir) {
    f32 s1 = nw4r::math::SinFIdx(lbl_eu_806662E4 * dir->z);
    f32 c1 = nw4r::math::CosFIdx(lbl_eu_806662E4 * dir->z);
    out->m[0][0] = c1;
    out->m[0][1] = -s1;
    out->m[0][2] = lbl_eu_806662DC;
    out->m[1][0] = s1;
    out->m[1][1] = c1;
    out->m[1][2] = lbl_eu_806662DC;
    out->m[2][0] = lbl_eu_806662DC;
    out->m[2][1] = lbl_eu_806662DC;
    out->m[2][2] = lbl_eu_806662D0;

    // Step 1 (dir->x): LHS rows (B,A,A / A,c2,-s2 / A,s2,c2).
    f32 s2 = nw4r::math::SinFIdx(lbl_eu_806662E4 * dir->x);
    f32 c2 = nw4r::math::CosFIdx(lbl_eu_806662E4 * dir->x);
    ml::CMat33 n;
    n.m[2][0] = lbl_eu_806662DC * out->m[0][0] + s2 * out->m[1][0]
        + c2 * out->m[2][0];
    n.m[2][1] = lbl_eu_806662DC * out->m[0][1] + s2 * out->m[1][1]
        + c2 * out->m[2][1];
    n.m[2][2] = lbl_eu_806662DC * out->m[0][2] + s2 * out->m[1][2]
        + c2 * out->m[2][2];
    n.m[1][0] = lbl_eu_806662DC * out->m[0][0] + c2 * out->m[1][0]
        - s2 * out->m[2][0];
    n.m[1][1] = lbl_eu_806662DC * out->m[0][1] + c2 * out->m[1][1]
        - s2 * out->m[2][1];
    n.m[1][2] = lbl_eu_806662DC * out->m[0][2] + c2 * out->m[1][2]
        - s2 * out->m[2][2];
    n.m[0][0] = lbl_eu_806662D0 * out->m[0][0] + lbl_eu_806662DC * out->m[1][0]
        + lbl_eu_806662DC * out->m[2][0];
    n.m[0][1] = lbl_eu_806662D0 * out->m[0][1] + lbl_eu_806662DC * out->m[1][1]
        + lbl_eu_806662DC * out->m[2][1];
    n.m[0][2] = lbl_eu_806662D0 * out->m[0][2] + lbl_eu_806662DC * out->m[1][2]
        + lbl_eu_806662DC * out->m[2][2];
    *out = n;

    // Step 2 (dir->y): LHS rows (c3,A,s3 / A,B,A / -s3,A,c3).
    f32 s3 = nw4r::math::SinFIdx(lbl_eu_806662E4 * dir->y);
    f32 c3 = nw4r::math::CosFIdx(lbl_eu_806662E4 * dir->y);
    ml::CMat33 n2;
    n2.m[2][0] = -s3 * out->m[0][0] + lbl_eu_806662DC * out->m[1][0]
        + c3 * out->m[2][0];
    n2.m[2][1] = -s3 * out->m[0][1] + lbl_eu_806662DC * out->m[1][1]
        + c3 * out->m[2][1];
    n2.m[2][2] = -s3 * out->m[0][2] + lbl_eu_806662DC * out->m[1][2]
        + c3 * out->m[2][2];
    n2.m[1][0] = lbl_eu_806662DC * out->m[0][0] + lbl_eu_806662D0 * out->m[1][0]
        + lbl_eu_806662DC * out->m[2][0];
    n2.m[1][1] = lbl_eu_806662DC * out->m[0][1] + lbl_eu_806662D0 * out->m[1][1]
        + lbl_eu_806662DC * out->m[2][1];
    n2.m[1][2] = lbl_eu_806662DC * out->m[0][2] + lbl_eu_806662D0 * out->m[1][2]
        + lbl_eu_806662DC * out->m[2][2];
    n2.m[0][0] = c3 * out->m[0][0] + lbl_eu_806662DC * out->m[1][0]
        + s3 * out->m[2][0];
    n2.m[0][1] = c3 * out->m[0][1] + lbl_eu_806662DC * out->m[1][1]
        + s3 * out->m[2][1];
    n2.m[0][2] = c3 * out->m[0][2] + lbl_eu_806662DC * out->m[1][2]
        + s3 * out->m[2][2];
    *out = n2;
}
// cfCam_posMinusOff: like cfCam_posFromLookat, but subtracts the transformed offset
// from `pos` (out = pos - R*offset) instead of adding it to a lookat point.
void cfCam_posMinusOff(ml::CVec3* out, ml::CVec3* pos, const ml::CVec3* dir, f32 speed) {
    ml::CMat33 dirFrame;
    ml::CVec3 scaled;
    ml::CVec3 offset;

    nopFunc(&dirFrame);
    func_80074534(&dirFrame, dir);

    // Clamp speed to its configured minimum.
    if (speed < lbl_eu_80661B64) {
        speed = lbl_eu_80661B60;
    }

    void* packed = writeVec3f(&offset, lbl_eu_806662DC, lbl_eu_806662DC, speed);
    func_800598A8(&scaled, &dirFrame, packed);
    subVec3f(&out->x, &pos->x, &scaled.x);
}
// cfCam_vecToAngles: direction vector from `from` to `to` (tmp = to - from); the
// tmp length is returned, and the caller's vec gets a pitch/yaw pair via the
// sin-wrapper atan2AnimFIdx (out[1] negated).
float cfCam_vecToAngles(const ml::CVec3* from, const ml::CVec3* to, float* out) {
    ml::CVec3 tmp;
    subVec3f(&tmp.x, &to->x, &from->x);
    float dist = cfCam_vecLength(&tmp);
    float r = VecMath_SafeSqrtF(tmp.x * tmp.x + tmp.z * tmp.z);
    out[0] = atan2AnimFIdx(tmp.y, r);
    out[1] = -atan2AnimFIdx(tmp.x, -tmp.z);
    return dist;
}
// cfCam_getActivePad: pick the active pad source - the remote GC pad when the
// 0x8000000 availability bit is set, otherwise the CfGameManager pad.
CPad* cfCam_getActivePad(int controllerId) {
    if (cfCam_testUnk04M(controllerId, 0x8000000) != 0) {
        return CDeviceRemotePad::getMainGCPad();
    }
    return cf::CfGameManager::getCurrentPad();
}
// cfCam_isClassicPad: whether the current pad is a classic controller (type 4).
// The self/input params are unused by the body but are part of the retail
// calling convention (call sites set up r3/f1 before the bl).
__declspec(noinline) bool cfCam_isClassicPad(void* self, float input) {
    (void)self;
    (void)input;
    return cf::CfGameManager::getCurrentPad()->mPadType == PAD_TYPE_CLASSIC;
}
// func_80074AA4: run camera action `id` (0..5) on the follow camera. First
// probes the 0x8000000 remote-pad availability bit: when set,
// each action dispatches cfCam_testWordMask with a <<16-shifted action mask;
// otherwise the plain 1<<id mask. Every action also fires the slot-0x70
// virtual (CfCamFollow::cfCam_getActivePad); its returned object is forwarded to
// cfCam_testWordMask. Unknown ids yield 0.
int func_80074AA4(cf::CfCamFollow* self, int id) {
    if (cfCam_testUnk04M(reinterpret_cast<int>(self), 0x8000000) != 0) {
        switch (id) {
        case 0:
            return cfCam_testWordMask(self->cfCam_getActivePad(), 0x20000);
        case 1:
            return cfCam_testWordMask(self->cfCam_getActivePad(), 0x40000);
        case 2:
            return cfCam_testWordMask(self->cfCam_getActivePad(), 0x80000);
        case 3:
            return cfCam_testWordMask(self->cfCam_getActivePad(), 0x100000);
        case 4:
            return cfCam_testWordMask(self->cfCam_getActivePad(), 0x10);
        case 5:
            return cfCam_testWordMask(self->cfCam_getActivePad(), 0x20);
        }
    } else {
        switch (id) {
        case 0:
            return cfCam_testWordMask(self->cfCam_getActivePad(), 0x1);
        case 1:
            return cfCam_testWordMask(self->cfCam_getActivePad(), 0x2);
        case 2:
            return cfCam_testWordMask(self->cfCam_getActivePad(), 0x4);
        case 3:
            return cfCam_testWordMask(self->cfCam_getActivePad(), 0x8);
        case 4:
            return cfCam_testWordMask(self->cfCam_getActivePad(), 0x10);
        case 5:
            return cfCam_testWordMask(self->cfCam_getActivePad(), 0x20);
        }
    }
    return 0;
}
// cfCam_newFollowCam: allocate a 0x25c-byte CfCamFollow block on the mtl heap and
// construct it with (self, arg2).
extern "C" void cfCam_newFollowCam(void* self, void* arg2) {
    u32 heap = CfRes_getHeapHandle();
    cf::CfCamFollow* cam = (cf::CfCamFollow*)allocate__Q23mtl10MemManagerFUlUl(0x25c, heap);
    if (cam != 0) {
        __ct__cf_CfCamFollow(cam, self, arg2);
    }
}
extern "C" int CfObject_getCurrentTarget__Q22cf8CfObjectFv() { return 0; }
extern "C" int CfObject_UnkVirtualFunc55__Q22cf8CfObjectFv() { return 0; }
extern "C" int CfObject_UnkVirtualFunc52__Q22cf8CfObjectFv() { return 0; }
// cf::CfObject base virtuals (retail: li r3,0; blr / li r3,1; blr)
extern "C" u32 CfObject_UnkVirtualFunc54__Q22cf8CfObjectFv(void* self) { return 0; }
extern "C" u32 CfObject_isMoveActiveNow__Q22cf8CfObjectFv(void* self) { return 1; }
// cf::CActorParam base virtual (retail: lwz r3,0x15e4(r3); blr)
// noinline keeps the retail `bl CActorParam_UnkVirtualFunc22...` in func_8006DD58
// instead of inlining the field load at the call site.
extern "C" __declspec(noinline) int CActorParam_UnkVirtualFunc22__Q22cf11CActorParamFv(void* self) { return *(u32*)((char*)self + 0x15e4); }

extern "C" bool cfCam_alwaysTrue() { return true; }
extern "C" __declspec(noinline) void cfCam_storeZeroU16(u16* p) { *p = 0; }
// Return a pointer to element `index` of a stride-12 (Vec3) array at `self`.
// noinline keeps the retail `bl cfCam_vec3AtIndex` at call sites.
extern "C" __declspec(noinline) u8* cfCam_vec3AtIndex(u8* self, int index) {
    return self + index * 12;
}
// Store the follow distance into both the shared global and the object field.
extern "C" void cfCam_setFollowD(cf::CfCamFollow* self, f32 value) {
    lbl_eu_80661B50 = value;
    self->unk1E0 = value;
}
// Bit 1 of the 0x4EC flag word. C++ linkage (mangled) so the extern "C"
// (const void*) declaration in CfCam.hpp stays a distinct signature and call
// sites keep the unmangled `bl cfCam_getBit1_4EC`.
unsigned int cfCam_getBit1_4EC(const cf::CfCamFollow* self) {
    return (self->field_0x4EC >> 1) & 1;
}
// Bit 2 of the 0x64 flag word. (void* ABI to match CfCam.hpp's decl.)
extern "C" __declspec(noinline) int cfCam_getBit2_64(void* obj) {
    const cf::CfCamFollow* self = static_cast<const cf::CfCamFollow*>(obj);
    return (self->field_0x64 >> 2) & 1;
}
__declspec(noinline) int cfCam_histCount(void* self) { return 0x10; }
__declspec(noinline) bool cfCam_set1E0Check(cf::CfCamFollow* self, float val) {
    self->unk1E0 = val;
    return func_8049EB60(self);
}
extern "C" u8* cfCam_getPlus6C(cf::CfCamFollow* self) { return self->unk1C + 0x50; }
