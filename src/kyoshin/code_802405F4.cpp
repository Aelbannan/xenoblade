// Auto-scaffolded catalog TU for kyoshin/code_802405F4
// Replace stubs with high-level C/C++ during decomp.

// CMapSelImports.hpp carries a legacy (void*, float) pseudo-import for AnimRewindFrame;
// code_80135FDC.hpp (included below) declares the canonical
// (nw4r::lyt::AnimTransform*, float). Skip the legacy decl so the two
// extern "C" declarations don't clash (10197). Must be defined before any
// header that pulls in CMapSelImports.hpp (code_802405F4.hpp includes it).
#define KYOSHIN_SKIP_CMAPSEL_LEGACY_LAYOUT_IMPORTS
// code_80135FDC.hpp declares ml::epsilon as u32; we need the float
// (.sdata2 epsilon) view, so rename that declaration away.
// (BdatGetU16ByTableKey: CFloorMap.hpp's legacy u32-returning copy moved TU-local
// into CFloorMap.cpp; the canonical u16 decl on code_80135FDC.hpp is now the
// only shared one - no rename guard needed here.)
#include <types.h>
#include "monolib/scn/CScnTimeApi.hpp"
#include <monolib/math/CVec3.hpp>
#include <monolib/math/MTRand.hpp>

#include "kyoshin/code_802405F4.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include <monolib/scn/CScn.hpp>
#include <monolib/math/CVec3.hpp>
#include "kyoshin/CBgTex.hpp"
#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/CMapSel.hpp"
#include "kyoshin/CFade.hpp"
#include <monolib/math/MTRand.hpp>
#include "monolib/math/FloatUtils.hpp"  // H3 label-owner decl (ml::epsilon)
#include "kyoshin/CTitleAHelpImports.hpp"
#include "kyoshin/code_80135FDC_State.hpp"


// Entry within an array of map landmark positions (stride 0x188, 8 entries)
// One 0x3C status/value slot per landmark; value is a live counter cleared
// when it drops back to the epsilon floor.
struct MapPointSlot {
    u8 _00[0x18];       // +0x00 - lead padding (array starts at entry +0x08)
    u8 flag;            // +0x18 - enabled byte
    u8 _19[0x13];
    float value;        // +0x2C
    u8 _30[0x0C];
};

struct MapPointEntry {
    u8 flag;            // +0x000 - active flag (non-zero = enabled)
    u8 _01[3];
    u32 id;             // +0x004 - entry id
    MapPointSlot slots[6]; // +0x008..0x16F (stride 0x3C, flag/value live at slot+0x18/+0x2C)
    ml::CVec3 pos;      // +0x170 - primary position
    ml::CVec3 pos2;     // +0x17C - secondary position
};

// Retail keeps this helper as an unmangled symbol.
/* Per-landmark pulse channel step. Advances the playback time and computes
   the vertical offset (field_10) from a damped sine wobble with lead-in /
   tail-out fading. Returns whether the channel is still alive. */
extern "C" bool MapFxStepPulseChannel(MapFxChannel* fx, float delta) {
    if (fx->field_2c <= ml::epsilon) return false;
    if (delta <= ml::epsilon) return true;

    float t = fx->field_00 + delta;
    if (fx->field_18 == 0 || t < fx->field_2c - fx->field_34) {
        fx->field_00 = t;
    }

    float dur = fx->field_2c;
    if (fx->field_00 >= dur) {
        // Playback reached the duration: reset the whole channel.
        const float z = lbl_eu_80668708;
        fx->field_00 = z;
        fx->field_2c = z;
        fx->field_04 = z;
        fx->field_08 = z;
        fx->field_0c = z;
        fx->field_10 = z;
        fx->field_14 = z;
        fx->field_18 = 0;
        return false;
    }
    if (fx->field_00 < fx->field_28) return true;

    // Wobble phase: accumulate time; when the accumulator wraps past its
    // limit, pick a fresh random amplitude/period pair.
    float acc = fx->field_04 + delta;
    fx->field_04 = acc;
    if (acc < fx->field_1c) {
        fx->field_0c = fx->field_08;
        float rnd1 = ml::MTRand::getInstance()->randFloat1();
        float amp = fx->field_20 * rnd1;
        fx->field_04 = lbl_eu_80668708;
        float lim = lbl_eu_80668714 * amp + fx->field_20;
        float scale = lbl_eu_80668718;
        if (fx->field_08 > lbl_eu_80668708) scale = lbl_eu_8066871C;
        fx->field_1c = lim;
        float rnd2 = ml::MTRand::getInstance()->randFloat1();
        float w = (lbl_eu_80668720 * rnd2 - lbl_eu_80668718) * fx->field_24;
        w = (fx->field_38 * w + fx->field_24) * scale;
        fx->field_08 = w;
    }

    float ph;
    if (fx->field_1c > ml::epsilon) {
        ph = fx->field_04 / fx->field_1c;
    } else {
        ph = lbl_eu_80668718;
    }
    float s = nw4r::math::SinFIdx(lbl_eu_80668724 *
                                  ((ph + fx->field_14) * ml::halfpi));
    float y = (fx->field_08 - fx->field_0c) * s + fx->field_0c;
    fx->field_10 = y;

    dur = fx->field_2c;
    if (fx->field_00 >= dur) {
        // Finished during this frame: flatten the output offset.
        fx->field_10 = lbl_eu_80668708;
        return true;
    }
    if (fx->field_30 > ml::epsilon) {
        float lim = fx->field_28 + fx->field_30;
        if (fx->field_00 < lim) {
            fx->field_10 = y * (lbl_eu_80668718 -
                (lim - fx->field_00) / fx->field_30);
            return true;
        }
    }
    if (fx->field_34 > ml::epsilon) {
        float lim = fx->field_2c - fx->field_34;
        if (fx->field_00 > lim) {
            fx->field_10 = y * (lbl_eu_80668718 -
                (fx->field_34 - (fx->field_2c - fx->field_00)) /
                fx->field_34);
        }
    }
    return true;
}

void func_80240878(){}

// Retail keeps these helpers as unmangled symbols.

// Steps the menu landmark FX channels and folds their offsets into the
// object's two output positions. If nothing is alive, clears everything.
// The scene-delta fetch consumes CfRes_getD80Flag()'s r3 return as its own
// argument (retail chains the two bl calls with no intervening arg setup).
// Indexed position writes keep the store anchored at +0x170 off the
// object base (retail strength-reduces them to a +4 byte walker); the
// !(x <= eps) form reproduces retail's cror eq,lt,eq branch fusion.
extern "C" void MapFxStepMenuChannels(MenuFxObj* obj) {
    float delta = Scn_GetFrameDelta((void*)CfRes_getD80Flag());

    obj->pos = ml::CVec3::zero;
    obj->pos2 = ml::CVec3::zero;

    // Channels are contiguous (fx[3] == fx2[0]); retail walks indices 0-2
    // into pos and 3-5 into pos2 via one shared counter. The destination is
    // a float view of the object indexed 0x5C+i, which keeps the +0x170 in
    // the store displacement while the walker starts at obj+4i.
    float* posOut = reinterpret_cast<float*>(obj);
    bool mainHit = false;
    bool done;
    for (int i = 0; i <= 2; i++) {
        MapFxChannel* fx = &obj->fx[i];
        // Materializing the le-test (CfGimmick.cpp pattern) yields retail's
        // fcmpo + cror eq,lt,eq + mfcr/extrwi sequence.
        done = fx->field_2c <= ml::epsilon;
        if (!done) {
            MapFxStepPulseChannel(fx, delta);
            posOut[0x5C + i] = fx->field_10;
            mainHit = true;
        }
    }

    bool subHit = false;
    for (int j = 3; j <= 5; j++) {
        MapFxChannel* fx = &obj->fx[j];
        done = fx->field_2c <= ml::epsilon;
        if (!done) {
            MapFxStepPulseChannel(fx, delta);
            posOut[0x5C + j] = fx->field_10;
            subHit = true;
        }
    }

    if (!mainHit && !subHit) {
        obj->pos = ml::CVec3::zero;
        obj->pos2 = ml::CVec3::zero;
        obj->field_00 = 0;
        obj->field_04 = 0;
    }
}

void MenuFx_ClearSlotFlags(u8* base) {
    MenuFxObj* entries = (MenuFxObj*)base;
    entries[0].field_00 = 0;
    entries[0].field_04 = 0;
    entries[1].field_00 = 0;
    entries[1].field_04 = 0;
    entries[2].field_00 = 0;
    entries[2].field_04 = 0;
    entries[3].field_00 = 0;
    entries[3].field_04 = 0;
    entries[4].field_00 = 0;
    entries[4].field_04 = 0;
    entries[5].field_00 = 0;
    entries[5].field_04 = 0;
    entries[6].field_00 = 0;
    entries[6].field_04 = 0;
    entries[7].field_00 = 0;
    entries[7].field_04 = 0;
}

// Advances the landmark FX for every active map point entry.
extern "C" void MapFxStepAllEntries(MenuFxObj* entries) {
    u32 base = (u32)entries;
    u32 off = 0;
    for (u32 i = 0; i < 8; i++) {
        if (*(u8*)(base + off) != 0)
            MapFxStepMenuChannels((MenuFxObj*)(base + off));
        off += sizeof(MenuFxObj);
    }
}

// Accumulates positions from active map point entries and applies them
// to a camera or transform object via ScnCamShiftBy / ScnCamTurnAim.
// Retail keeps four zero-assigned locals (two are dead after optimization)
// and walks the 8 entries as an unrolled pair inside a 4-iteration ctr loop;
// the sums are in-place nw4r VEC3Add paired-single operations.
void MapFxApplyEntryPositions(MapPointEntry* entries, void* target) {
    if (cf::CfGameManager::isSceneLoading() != 0) {
        return;
    }

    // Retail retains all four slots; the array keeps the unused elements'
    // initialization stores alive (MWCC DCEs standalone dead locals).
    ml::CVec3 acc[4];
    acc[0] = ml::CVec3::zero;
    acc[1] = ml::CVec3::zero;
    acc[2] = ml::CVec3::zero;
    acc[3] = ml::CVec3::zero;

    bool anyActive = false;

    // Walk by byte offset so MWCC strength-reduces to a base+offset counter
    // (lbzx/add r4 form) instead of a dedicated cursor register.
    u32 off = 0;
    for (int i = 0; i < 4; i++) {
        MapPointEntry* e = reinterpret_cast<MapPointEntry*>(reinterpret_cast<u8*>(entries) + off);
        if (e->flag != 0) {
            nw4r::math::VEC3Add(reinterpret_cast<nw4r::math::VEC3*>(&acc[0]),
                                reinterpret_cast<const nw4r::math::VEC3*>(&acc[0]),
                                reinterpret_cast<const nw4r::math::VEC3*>(&e->pos));
            nw4r::math::VEC3Add(reinterpret_cast<nw4r::math::VEC3*>(&acc[1]),
                                reinterpret_cast<const nw4r::math::VEC3*>(&acc[1]),
                                reinterpret_cast<const nw4r::math::VEC3*>(&e->pos2));
            anyActive = true;
        }
        off += sizeof(MapPointEntry);
        e = reinterpret_cast<MapPointEntry*>(reinterpret_cast<u8*>(entries) + off);
        if (e->flag != 0) {
            nw4r::math::VEC3Add(reinterpret_cast<nw4r::math::VEC3*>(&acc[0]),
                                reinterpret_cast<const nw4r::math::VEC3*>(&acc[0]),
                                reinterpret_cast<const nw4r::math::VEC3*>(&e->pos));
            nw4r::math::VEC3Add(reinterpret_cast<nw4r::math::VEC3*>(&acc[1]),
                                reinterpret_cast<const nw4r::math::VEC3*>(&acc[1]),
                                reinterpret_cast<const nw4r::math::VEC3*>(&e->pos2));
            anyActive = true;
        }
        off += sizeof(MapPointEntry);
    }

    if (anyActive) {
        ScnCamShiftBy(target, &acc[0]);
        ScnCamTurnAim(target, &acc[1]);
    }
}

// Initializes one landmark pulse channel from the shared parameter block.
// Zeroes the channel first; when both duration reference and width are
// non-zero, populates timing/amplitude fields and draws two random values
// (phase base and wobble accumulator limit). Retail inlines this at every
// call site, hence the forced inline.
static inline void InitFxChannel(MapFxChannel* fx, const FxParams* p, float width,
                                 float endScale) {
    // Zero-fill order mirrors retail's scheduled stores; constants are read
    // from sdata2 at every use (retail does not cache them in registers).
    fx->field_00 = lbl_eu_80668708;
    fx->field_2c = lbl_eu_80668708;
    fx->field_04 = lbl_eu_80668708;
    fx->field_08 = lbl_eu_80668708;
    fx->field_0c = lbl_eu_80668708;
    fx->field_10 = lbl_eu_80668708;
    fx->field_14 = lbl_eu_80668708;
    fx->field_18 = 0;

    if (p->field_20 == lbl_eu_80668708 || width == lbl_eu_80668708) return;

    fx->field_24 = width;
    fx->field_20 = p->field_00;
    fx->field_28 = lbl_eu_8066870C * p->field_1c;
    fx->field_30 = lbl_eu_8066870C * p->field_24;
    fx->field_34 = lbl_eu_8066870C * p->field_28;
    fx->field_38 = endScale;
    fx->field_2c = lbl_eu_8066870C * (p->field_1c + p->field_20);

    float rnd1 = ml::MTRand::getInstance()->randFloat1();
    fx->field_14 = rnd1 * lbl_eu_80668710;

    float rnd2 = ml::MTRand::getInstance()->randFloat1();
    float amp = fx->field_20;
    float lim = lbl_eu_80668714 * (amp * rnd2) + amp;
    fx->field_1c = lim;
    fx->field_04 = lim;
}

// Resets/initializes all 8 landmark FX entries. Entries already flagged active
// are skipped. Each fresh entry gets its 6 pulse channels initialized (widths
// for channels 3-5 come pre-scaled by pi/2), the active flag is set, live
// channels inherit the caller's enable byte, and a shared counter value is
// stamped into the entry id. Always returns 0.
s32 MapFxAllocEntry(MenuFxObj* entries, FxParams* params, u8 flagVal) {
    MenuFxObj* obj = entries;
    for (int i = 0; i < 8; i++, obj++) {
        if (obj->field_00 != 0) continue;
        // NOTE: retail returns immediately after initializing the first
        // inactive entry (returns the new counter value); 0 only when all
        // 8 entries were already active.

        InitFxChannel(&obj->fx[0], params, params->field_04, params->field_2c);
        InitFxChannel(&obj->fx[1], params, params->field_08, params->field_2c);
        InitFxChannel(&obj->fx[2], params, params->field_0c, params->field_2c);

        // Channels 3-5 take pi/2-scaled widths from the reserved param block.
        // volatile pins the pi/2-scaled widths to stack slots across the
        // intervening randFloat1 calls (retail keeps no f14+ live ranges).
        float pi2 = ml::deg2rad;
        volatile float w3 = params->field_10 * pi2;
        volatile float w4 = params->field_14 * pi2;
        volatile float w5 = params->field_18 * pi2;
        InitFxChannel(&obj->fx2[0], params, w3, params->field_30);
        InitFxChannel(&obj->fx2[1], params, w4, params->field_30);
        InitFxChannel(&obj->fx2[2], params, w5, params->field_30);

        obj->field_00 = 1;

        const float eps = ml::epsilon;
        if (obj->fx[0].field_2c > eps) obj->fx[0].field_18 = flagVal;
        if (obj->fx[1].field_2c > eps) obj->fx[1].field_18 = flagVal;
        if (obj->fx[2].field_2c > eps) obj->fx[2].field_18 = flagVal;
        if (obj->fx2[0].field_2c > eps) obj->fx2[0].field_18 = flagVal;
        if (obj->fx2[1].field_2c > eps) obj->fx2[1].field_18 = flagVal;
        if (obj->fx2[2].field_2c > eps) obj->fx2[2].field_18 = flagVal;

        // Shared landmark counter with skip-zero wrap handling.
        u32 count = lbl_eu_80664788 + 1;
        lbl_eu_80664788 = count;
        if (count == 0) {
            count = count + 1;
            lbl_eu_80664788 = count;
        }
        obj->field_04 = count;
        return count;
    }
    return 0;
}

// Clears each landmark slot flag whose live counter has risen above the
// epsilon floor, for active entries matching id (or all when id == 0).
void MapFxClearSlotFlags(MapPointEntry* entries, u32 id) {
    MapPointEntry* e = entries;
    for (int i = 0; i < 8; i++, e++) {
        if (e->flag == 0) continue;
        if (!(id == e->id || id == 0)) continue;
        MapPointSlot* s = e->slots;
        for (int j = 6; j != 0; j--) {
            u8 stop = s->value <= ml::epsilon;
            if (!stop) s->flag = 0;
            s++;
        }
    }
}

// Resets active landmark entries matching id (or all when id == 0):
// clears each live slot counter and zeroes the stored positions.
void MapFxResetEntries(MapPointEntry* entries, u32 id) {
    MapPointEntry* e = entries;
    for (int i = 0; i < 8; i++, e++) {
        if (e->flag == 0) continue;
        if (!(id == e->id || id == 0)) continue;
        MapPointSlot* s = e->slots;
        for (int j = 0; j < 6; j++, s++) {
            u8 stop = s->value <= ml::epsilon;
            if (!stop) s->flag = 0;
        }
        e->pos = ml::CVec3::zero;
        e->pos2 = ml::CVec3::zero;
        e->flag = 0;
        e->id = 0;
    }
}
