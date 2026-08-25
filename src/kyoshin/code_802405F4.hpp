#pragma once

#include <types.h>
#include <monolib/work/CProcess.hpp>
#include <monolib/scn/IScnRender.hpp>
#include <revolution/GX.h>

#include "kyoshin/CBgTex.hpp"
#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/CMapSel.hpp"
#include "kyoshin/CFade.hpp"

// CFloorMap.hpp and CMapSel.hpp declare overlapping helpers with different
// signatures; rename CFloorMap's versions while its header is processed.
#define CCur18View CCur18ViewFloorMap
#define CCur18Data CCur18DataFloorMap
#define func_801F3540 func_801F3540FloorMap
#define func_801D202C func_801D202CFloorMap
#define func_8022B748 func_8022B748FloorMap
#define func_801F3850 func_801F3850FloorMap
#define func_8013676C func_8013676CFloorMap
#define func_801F367C func_801F367CFloorMap
#define func_801F3670 func_801F3670FloorMap
#define func_801F36BC func_801F36BCFloorMap
#define func_8003B1EC func_8003B1ECFloorMap
#define lbl_eu_806640A8 lbl_eu_806640A8FloorMap
#define func_80136190 func_80136190FloorMap
#define func_80137E7C func_80137E7CFloorMap
#include "kyoshin/CFloorMap.hpp"
#undef CCur18View
#undef CCur18Data
#undef func_801F3540
#undef func_801D202C
#undef func_8022B748
#undef func_801F3850
#undef func_8013676C
#undef func_801F367C
#undef func_801F3670
#undef func_801F36BC
#undef func_8003B1EC
#undef lbl_eu_806640A8
#undef func_80136190
#undef func_80137E7C

class CScn;

// C-ABI imports shared by the menu map-select FX helpers
extern "C" {
int CfRes_getD80Flag();
}

// Imports used by CMenuMapSelect::Move and the landmark FX initializer
extern u32 lbl_eu_80663E28;      // .sbss global busy-flag word (bit 21)
extern u32 lbl_eu_80664788;      // .sdata landmark request counter
extern const float lbl_eu_8066870C;
extern const float lbl_eu_80668710;
extern const float lbl_eu_8066A210;  // pi/2 variant used by FX init scaling

extern "C" {
void func_8013EC6C(u32, u32);                       // UI mode switch helper
u32 func_8009CF8C(u32 resourceId);                  // resource query
int func_800FF738();                                // scene transition gate
int func_800FEDF8();
void func_800FF914();
// Static-member helper on cf::CfGameManager (retail keeps the mangled name);
// called with the map index and a zero second argument.
void func_8008413C__Q22cf13CfGameManagerFv(u32, u32);
void func_80242368(class CMenuMapSelect* self);     // world map input handler
void func_80242524(class CMenuMapSelect* self);     // world map phase 2 setup
void func_80242A28(class CMenuMapSelect* self);     // floor map input handler
void func_802434A0(class CMapSel* self);
int func_80243680(class CMapSel* self);
int func_802436C4(class CMapSel* self);
void func_802436CC(class CMapSel* self);
// Retail keeps the CTitleAHelp/CBgTex/CFade helpers as unmangled symbols.
int func_801C3E34(class CBgTex* self);
void func_801C3D54(class CBgTex* self);
void func_801C3FF0(class CTitleAHelp* self);
int func_801C4114(class CTitleAHelp* self);
// Mangled-name form so callers compare with cmpwi directly (no byte mask).
extern "C" int isIdle__11CTitleAHelpFv(class CTitleAHelp* self);
void func_801C412C(class CTitleAHelp* self);
void func_801C41E8(class CTitleAHelp* self, u8 arg);
void func_801C4654(class CTitleAHelp* self, u32 arg);
void func_801C46B4(class CTitleAHelp* self, char* text);
void func_801C46DC(class CTitleAHelp* self, u32 arg);
void func_802443E8(class CFade* self);
int func_80244510(class CFade* self);
void func_80244538(class CFade* self);
int func_8024CE1C(class CFloorMap* self);
void func_8024BE1C(class CFloorMap* self);
void func_8024CB94(class CFloorMap* self);
int func_8024CE60(class CFloorMap* self);
void func_8024C1FC(class CFloorMap* self);
u16 func_8024F54C(class CFloorMap* self);
void func_80243560(class CMapSel* self, class nw4r::lyt::DrawInfo* drawInfo);
void func_8024C8F8(class CFloorMap* self, class nw4r::lyt::DrawInfo* drawInfo);
// Retail keeps these draw helpers as unmangled free symbols taking sub-object pointers.
void func_801C3D7C(class CBgTex* self, class nw4r::lyt::DrawInfo* drawInfo);
void func_801C4080(class CTitleAHelp* self, class nw4r::lyt::DrawInfo* drawInfo);
void func_80244460(class CFade* self, class nw4r::lyt::DrawInfo* drawInfo);
// Retail emits direct bl to the DrawInfo ctor/dtor symbols.
void __ct__Q34nw4r3lyt8DrawInfoFv(class nw4r::lyt::DrawInfo* self);
void __dt__Q34nw4r3lyt8DrawInfoFv(class nw4r::lyt::DrawInfo* self, int flags);
int func_8013BE50();
// Camera/transform position setters (retail keeps unmangled symbols)
void func_8049F168(void* obj, ml::CVec3* pos);
void func_8049F204(void* obj, ml::CVec3* pos);
}

// sdata2 float constants used by the FX easing helpers
extern const float lbl_eu_80668708;  // 0.0f
extern const float lbl_eu_80668714;
extern const float lbl_eu_80668718;  // 1.0f
extern const float lbl_eu_8066871C;
extern const float lbl_eu_80668720;
extern const float lbl_eu_80668724;
extern const float lbl_eu_8066A200;  // pi/2 (sdata2)

// One easing/pulse channel driven by func_80240614 (0x3C bytes).
struct MapFxChannel {
    float field_00;   // +0x00 playback time
    float field_04;   // +0x04 wobble accumulator
    float field_08;   // +0x08 amplitude
    float field_0c;   // +0x0c previous amplitude
    float field_10;   // +0x0c+4 computed vertical offset (output)
    float field_14;   // +0x14 phase base
    u8 field_18;      // +0x18 clamping enable
    u8 _pad19[3];
    float field_1c;   // +0x1c accumulator limit
    float field_20;   // +0x20 swing amplitude
    float field_24;   // +0x24 swing width
    float field_28;   // +0x28 hold-in time
    float field_2c;   // +0x2c duration
    float field_30;   // +0x30 lead-in fade length
    float field_34;   // +0x34 tail fade length
    float field_38;   // +0x38 end-scale factor
};

// Container updated by func_802408D4: two pairs of channels feeding the
// position vectors at +0x170/+0x17C.
struct MenuFxObj {
    u8 field_00;           // +0x00 active flag
    u8 _01[3];
    u32 field_04;          // +0x04 live counter (from lbl_eu_80664788)
    MapFxChannel fx[3];    // +0x08 primary channels (8/0x44/0x80)
    MapFxChannel fx2[3];   // +0xBC secondary channels (0xBC/0xF8/0x134)
    ml::CVec3 pos;         // +0x170
    ml::CVec3 pos2;        // +0x17C
};

/* Per-entry FX parameter block passed to func_80240C98. Only the fields the
   initializer reads are named; the rest is padding. */
struct FxParams {
    float field_00;   // +0x00 swing amplitude
    float field_04;   // +0x04 swing width (channel 0)
    float field_08;   // +0x08 swing width (channel 1)
    float field_0c;   // +0x0c swing width (channel 2)
    float field_10;   // +0x10 pi/2-scaled width source (channel 4)
    float field_14;   // +0x14 pi/2-scaled width source (channel 3)
    float field_18;   // +0x18 pi/2-scaled width source (channel 5)
    float field_1c;   // +0x1c hold-in scale factor
    float field_20;   // +0x20 duration reference
    float field_24;   // +0x24 lead-in fade length
    float field_28;   // +0x28 tail fade length
    float field_2c;   // +0x2c end-scale (channels 0-2)
    float field_30;   // +0x30 end-scale (channels 3-5)
};

/* Field-exact view of CFloorMap for the retail memberwise copy helper
   func_80241920. Widths mirror the retail copy: words for data blocks, bytes
   for flags, floats for scalar state, and two pair-array loops (20 x 8 bytes
   at +0x150, 0x618 x 8 bytes at +0x20C). The vtable word (+0x00), the small
   inter-block gaps and the trailing word pair (+0x3340/+0x3344, stored in
   reverse order) are handled explicitly. */
struct CFloorMapCopyPair {
    u32 a;
    u32 b;
};

/* Retail lowers the 0xB4..0x207 and 0x208..0x32CB stretches as single
   struct assignments. Skipped gaps are unnamed storage (anonymous byte
   bitfields), so only named members expand to copies; the pair arrays
   become ctr loops whose walkers start at the substruct base - 4
   (displacement folds the array offset). */
struct CFloorMapCopyBlock1 {
    u8 fB4 : 8;
    u8 fB5 : 8;
    u8 : 8;
    u8 : 8;
    u8 : 8;
    u8 : 8;                 // +0xB6..0xBB skipped
    u32 fBC[9];             // +0xBC..0xDC
    u8 fE0 : 8;
    u8 : 8;
    u8 : 8;
    u8 : 8;
    u32 fE4[2];             // +0xE4..0xE8
    u8 fEC0 : 8;            // +0xEC..0xF1
    u8 fEC1 : 8;
    u8 fEC2 : 8;
    u8 fEC3 : 8;
    u8 fEC4 : 8;
    u8 fEC5 : 8;
    u8 : 8;
    u8 : 8;
    u8 : 8;
    u8 : 8;
    u8 : 8;
    u8 : 8;                 // +0xF2..0xF7 skipped
    u32 fF8[9];             // +0xF8..0x118
    u8 f11C : 8;
    u8 : 8;
    u8 : 8;
    u8 : 8;
    u32 f120[2];            // +0x120..0x124
    u8 f128_0 : 8;          // +0x128..0x12D
    u8 f128_1 : 8;
    u8 f128_2 : 8;
    u8 f128_3 : 8;
    u8 f128_4 : 8;
    u8 f128_5 : 8;
    u8 : 8;
    u8 : 8;
    u32 f130[7];            // +0x130..0x148
    u8 f14C : 8;
    u8 : 8;
    u8 : 8;
    u8 : 8;
    CFloorMapCopyPair loop1[20]; // +0x150..0x1EF
    u8 f1F0 : 8;
    u8 : 8;
    u8 : 8;
    u8 : 8;
    u32 f1F4;               // +0x1F4
    u8 f1F8 : 8;
    u8 : 8;
    u8 : 8;
    u8 : 8;
    u32 f1FC;               // +0x1FC
    u32 f200;               // +0x200
    u8 f204 : 8;
    u8 f205 : 8;
    u8 f206 : 8;
    u8 f207 : 8;
};

struct CFloorMapCopyBlock2 {
    u8 f208 : 8;            // +0x208
    u8 : 8;
    u8 : 8;
    u8 : 8;
    CFloorMapCopyPair loop2[0x618]; // +0x20C..0x32CB
};

struct CFloorMapCopyView {
    u8 _00[4];              // +0x00 vptr (skipped)
    u32 f04[15];            // +0x04..0x3C
    u8 f40, f41, f42, f43;
    float f44[5];           // +0x44..0x54
    u8 f58;
    u8 _59;
    u16 f5A;
    u8 f5C, f5D;
    u8 _5E[6];              // +0x5E..0x63 (skipped)
    u32 f64[8];             // +0x64..0x80
    u8 f84, f85, f86, f87;
    float f88[5];           // +0x88..0x94
    u8 f9C;
    u8 _9D[3];
    u8 _A0[4];              // +0xA0..0xA3 (skipped)
    u32 fA4[3];             // +0xA4..0xAC
    u32 fB0;                // +0xB0
    CFloorMapCopyBlock1 block1;
    CFloorMapCopyBlock2 block2;
    u8 f32CC;
    u8 _32CD[3];
    u32 f32D4[4];           // +0x32D4..0x32E0
    u8 f32E4, f32E5;
    u8 _32E6[6];
    u32 f32EC[4];           // +0x32EC..0x32F8
    u8 f32FC, f32FD;
    u8 _32FE[6];
    u32 f3304[4];           // +0x3304..0x3310
    u8 f3314, f3315;
    u8 _3316[6];
    u32 f331C[4];           // +0x331C..0x3328
    u8 f332C, f332D;
    u8 _332E[2];
    u32 f3330[3];           // +0x3330..0x3338
    u8 f333C;
    u8 _333D[3];
    u32 f3340;
    u32 f3344;
};

/* Tail of the CMapSel copy region starting at the unaligned offset +0x8E.
   Retail copies these fields with unaligned word loads/stores (lwz/stw at
   0x8E, 0x92, ... 0xAA), reproduced here via the packed attribute. */
struct __attribute__((packed)) CMapSelCopyTail {
    u32 w[7];   // +0x8E..0xA8
    u32 aa;     // +0xAA..0xAD
    u8 ae, af;  // +0xAE..0xAF
    float b0;   // +0xB0
};

/* Whole-array word blocks: assigning these by value lets MWCC expand them
   as a single unrolled ascending copy group. */
struct CMapSelCopyW11 { u32 v[11]; };
struct CMapSelCopyW8 { u32 v[8]; };

/* Source-side view (distinct type from CMapSelCopyView so strict aliasing
   lets MWCC hoist loads above stores, as in retail). */
struct __attribute__((packed)) CMapSelCopyViewSrc {
    u8 _00[4];                          // +0x00 vptr (skipped)
    CMapSelCopyW11 f04;                 // +0x04..0x2C
    u8 f30, f31, f32, f33;              // +0x30..0x33
    u8 _34[4];                          // +0x34..0x37 (skipped)
    CMapSelCopyW8 f38;                  // +0x38..0x54
    u8 f58, f59, f5A, f5B;              // +0x58..0x5B
    float f5C, f60, f64, f68, f6C;      // +0x5C..0x6C
    u8 f70;                             // +0x70 (byte)
    u8 _71[7];
    u32 f78[4];                         // +0x78..0x84
    u8 f88, f89, _8A[2], f8C, f8D;      // +0x88..0x8D
    CMapSelCopyTail tail;               // +0x8E..0xB1 (packed, unaligned)
};

/* Field-exact view of CMapSel for the retail memberwise copy helper
   func_80240614-style (func_80241640). Widths follow the retail copy:
   words for 0x04-0x2C / 0x38-0x54 / 0x70-0xAD, bytes for the flag fields,
   floats for 0x5C-0x6C and 0xB0. The vtable word (+0x00) and +0x34-0x37
   are not copied by retail. */
struct CMapSelCopyView {
    u8 _00[4];                          // +0x00 vptr (skipped)
    CMapSelCopyW11 f04;                 // +0x04..0x2C
    u8 f30, f31, f32, f33;              // +0x30..0x33
    u8 _34[4];                          // +0x34..0x37 (skipped)
    CMapSelCopyW8 f38;                  // +0x38..0x54
    u8 f58, f59, f5A, f5B;              // +0x58..0x5B
    float f5C, f60, f64, f68, f6C;      // +0x5C..0x6C
    u8 f70;                             // +0x70 (byte)
    u8 _71[7];
    u32 f78[4];                         // +0x78..0x84
    u8 f88, f89, _8A[2], f8C, f8D;      // +0x88..0x8D
    CMapSelCopyTail tail;               // +0x8E..0xB1 (packed, unaligned)
};

/* Field-exact view of CFade for the retail memberwise copy helper
   func_8024189C: everything from mMemRegion (+0x04) through the four flag
   bytes (+0x24..0x27). The vtable word (+0x00) is not copied. Two
   structurally identical but distinct types are used for the source and
   destination views so strict aliasing lets MWCC hoist every load above
   every store, matching the retail copy sequence. */
struct CFadeCopyView {
    UnkClass_8045F564 mMemRegion;               // +0x04..0x13
    CFileHandle* mFileHandle;                   // +0x14
    nw4r::lyt::ArcResourceAccessor* mArcResAcc; // +0x18
    nw4r::lyt::Layout* mLayout;                 // +0x1C
    nw4r::lyt::AnimTransform* mAnimTrans;       // +0x20
    u8 mIsLoaded, mFadeState, mReady, mVisible; // +0x24..0x27
};

// Distinct-type mirror of CFadeCopyView for the copy source.
struct CFadeMemRegionSrc {
    u32 unk0, unk4, unk8, unkC;
};
struct CFadeCopyViewSrc {
    CFadeMemRegionSrc mMemRegion;
    CFileHandle* mFileHandle;
    nw4r::lyt::ArcResourceAccessor* mArcResAcc;
    nw4r::lyt::Layout* mLayout;
    nw4r::lyt::AnimTransform* mAnimTrans;
    u8 mIsLoaded, mFadeState, mReady, mVisible;
};

class CMenuMapSelect : public CProcess, public IScnRender {
public:
    CMenuMapSelect();
    virtual ~CMenuMapSelect();
    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void cbRenderBefore();

    // Retail layout, recovered from the Init__14CMenuMapSelectFv /
    // __dt__14CMenuMapSelectFv / Move__14CMenuMapSelectFv disassembly:
    //   0x00-0x3C: CProcess base
    //   0x3C-0x40: IScnRender base (vtable ptr)
    //   0x40-0x53: unknown
    //   0x54:      state byte
    //   0x5C:      CScn* (loaded at Init's addRenderCB call)
    //   0x60:      CBgTex      (0x20)
    //   0x80:      CTitleAHelp (0x38)
    //   0xB8:      CMapSel     (0xB4)
    //   0x16C:     CFade       (0x28)
    //   0x194:     CFloorMap   (0x3348)
    //   0x34DC:    state byte (Move state machine)
    u8 mField40[0x14];       // 0x40
    u8 mField54;             // 0x54
    u8 _pad55[7];            // 0x55-0x5B
    CScn* mScn;              // 0x5C
    CBgTex mBgTex;           // 0x60
    CTitleAHelp mTitleHelp;  // 0x80
    CMapSel mMapSel;         // 0xB8
    CFade mFade;             // 0x16C
    CFloorMap mFloorMap;     // 0x194
    u8 mState;               // 0x34DC
};
