#pragma once

#include "kyoshin/cf/CfBdat.hpp"

#include <types.h>
#include <monolib/work/CProcess.hpp>
#include <monolib/scn/IScnRender.hpp>
#include <revolution/GX.h>

#include "kyoshin/CBgTex.hpp"
#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/CFade.hpp"

// CFloorMap.hpp and CMapSel.hpp declare overlapping helpers with different
// signatures; rename CFloorMap's versions while its header is processed.
#define Bdat_GetMaxRow_B1EC Bdat_GetMaxRow_B1ECFloorMap
#include "kyoshin/CFloorMap.hpp"
#undef Bdat_GetMaxRow_B1EC

class CScn;

// C-ABI imports shared by the menu map-select FX helpers
extern "C" {
extern "C" int CfRes_getD80Flag();
}

// Imports used by the landmark FX initializer
extern u32 lbl_eu_80664788;      // .sdata landmark request counter
extern const float lbl_eu_8066870C;
extern const float lbl_eu_80668710;

extern "C" {
// Camera/transform position setters (retail keeps unmangled symbols)
void ScnCamShiftBy(void* obj, ml::CVec3* pos);
void ScnCamTurnAim(void* obj, ml::CVec3* pos);
}

// sdata2 float constants used by the FX easing helpers
extern const float lbl_eu_80668708;  // 0.0f
extern const float lbl_eu_80668714;
extern const float lbl_eu_80668718;  // 1.0f
extern const float lbl_eu_8066871C;
extern const float lbl_eu_80668720;
extern const float lbl_eu_80668724;

// One easing/pulse channel driven by MapFxStepPulseChannel (0x3C bytes).
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

// Container updated by MapFxStepMenuChannels: two pairs of channels feeding the
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

/* Per-entry FX parameter block passed to MapFxAllocEntry. Only the fields the
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

struct MapPointEntry;

// C++-linkage helpers owned by kyoshin/code_802405F4.
void MenuFx_ClearSlotFlags(u8* base);
void MapFxClearSlotFlags(MapPointEntry* entries, u32 id);
void MapFxResetEntries(MapPointEntry* entries, u32 id);
