// Auto-scaffolded catalog TU for kyoshin/code_802405F4
// Replace stubs with high-level C/C++ during decomp.

// CMapSel.hpp carries a legacy (void*, float) pseudo-import for func_80137510;
// code_80135FDC.hpp (included below) declares the canonical
// (nw4r::lyt::AnimTransform*, float). Skip the legacy decl so the two
// extern "C" declarations don't clash (10197). Must be defined before any
// header that pulls in CMapSel.hpp (code_802405F4.hpp includes it).
#define KYOSHIN_SKIP_CMAPSEL_LEGACY_LAYOUT_IMPORTS
// code_80135FDC.hpp declares lbl_eu_8066A208 as u32; we need the float
// (.sdata2 epsilon) view, so rename that declaration away.
#define lbl_eu_8066A208 menuMapSelEpsilonU32Unused
#include <types.h>
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

#undef lbl_eu_8066A208
extern const float lbl_eu_8066A208;

// Copy helpers for member sub-object initialization
// func_801BE108: copies CBgTex field-by-field from src to dest
extern void func_801BE108(CBgTex* dest, CBgTex* src);
// func_801BE16C: copies CTitleAHelp field-by-field from src to dest
extern void func_801BE16C(CTitleAHelp* dest, CTitleAHelp* src);

// Init helpers for sub-objects (defined later in this TU)
void func_80241640(CMapSel* dest, CMapSel* src);
void func_8024189C(CFade* dest, CFade* src);
void func_80241920(CFloorMap* dest, CFloorMap* src);
extern void func_8024343C(CMapSel* mapSel);
extern void func_8024439C(CFade* fade);

// Camera/transform position setters
extern void func_8049F168(void* obj, ml::CVec3* pos);
extern void func_8049F204(void* obj, ml::CVec3* pos);

// String table base for MNU_item / MNU_kyeassign lookups
extern "C" {
extern char lbl_eu_8050B498[];
// CfGameManager unity helpers bridge
extern void func_8008294C__Q22cf13CfGameManagerFv(bool enable);

// Term-time helpers (declared here; retail unmangled call relocs).
extern "C" void waitForDrawDone__9CDeviceVIFv();
extern "C" void func_804962A0(CScn* scn, int flag);
extern "C" void func_801C3D9C(CBgTex* self);
extern "C" void func_801C40A0(CTitleAHelp* self);
extern "C" void func_802435CC(CMapSel* self);
extern "C" void func_8024448C(CFade* self);
extern "C" void func_8024CB94(CFloorMap* self);
extern u32 lbl_eu_80664790;
}

// Entry within an array of map landmark positions (stride 0x188, 8 entries)
// One 0x3C status/value slot per landmark; value is a live counter cleared
// when it drops back to the epsilon floor.
struct MapPointSlot {
    u8 flag;            // +0x00 - enabled byte
    u8 _01[0x13];
    float value;        // +0x14
    u8 _18[0x24];
};

struct MapPointEntry {
    u8 flag;            // +0x000 - active flag (non-zero = enabled)
    u8 _01[3];
    u32 id;             // +0x004 - entry id
    u8 _08[0x18];       // +0x008..0x01F
    MapPointSlot slots[6]; // +0x020..0x16F
    ml::CVec3 pos;      // +0x170 - primary position
    ml::CVec3 pos2;     // +0x17C - secondary position
};

/* Per-landmark pulse channel step. Advances the playback time and computes
   the vertical offset (field_10) from a damped sine wobble with lead-in /
   tail-out fading. Returns whether the channel is still alive. */
bool func_80240614(MapFxChannel* fx, float delta) {
    const float eps = lbl_eu_8066A208;
    if (fx->field_2c <= eps) return false;
    if (delta <= eps) return true;

    float dur = fx->field_2c;
    float t = fx->field_00 + delta;
    if (fx->field_18 == 0 || t < dur - fx->field_34) {
        fx->field_00 = t;
    }
    t = fx->field_00;

    if (t < dur) {
        // Channel was cancelled mid-flight: clear and report inactive
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
    if (t < fx->field_28) return true;

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
    if (fx->field_1c > eps) {
        ph = fx->field_04 / fx->field_1c;
    } else {
        ph = lbl_eu_80668718;
    }
    float s = nw4r::math::SinFIdx(lbl_eu_80668724 *
                                  ((ph + fx->field_14) * lbl_eu_8066A200));
    float y = (fx->field_08 - fx->field_0c) * s + fx->field_0c;
    fx->field_10 = y;

    if (t < dur) {
        fx->field_10 = lbl_eu_80668708;
        return true;
    }
    if (fx->field_30 > eps) {
        float lim = fx->field_28 + fx->field_30;
        if (t < lim) {
            fx->field_10 = y * (lbl_eu_80668718 - (lim - t) / fx->field_30);
            return true;
        }
    }
    if (fx->field_34 > eps) {
        float lim = dur - fx->field_34;
        if (t > lim) {
            fx->field_10 = y * (lbl_eu_80668718 -
                (fx->field_34 - (dur - t)) / fx->field_34);
        }
    }
    return true;
}

void func_80240878(){}

// Steps the menu landmark FX channels and folds their offsets into the
// object's two output positions. If nothing is alive, clears everything.
void func_802408D4(MenuFxObj* obj) {
    CfRes_getD80Flag();
    float delta = func_80496288();

    obj->pos = ml::CVec3::zero;
    obj->pos2 = ml::CVec3::zero;

    bool mainHit = false;
    float* dst = &obj->pos.x;
    for (int i = 0; i < 2; i++) {
        MapFxChannel* fx = &obj->fx[i];
        if (fx->field_2c > lbl_eu_8066A208) {
            func_80240614(fx, delta);
            *dst++ = fx->field_10;
            mainHit = true;
        }
    }

    bool subHit = false;
    dst = &obj->pos2.x;
    for (int i = 0; i < 2; i++) {
        MapFxChannel* fx = &obj->fx2[i];
        if (fx->field_2c > lbl_eu_8066A208) {
            func_80240614(fx, delta);
            *dst++ = fx->field_10;
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

void func_80240A64(u8* base) {
    base[0x0] = 0;
    *(int *)(base + 0x4) = 0;
    base[0x188] = 0;
    *(int *)(base + 0x18c) = 0;
    base[0x310] = 0;
    *(int *)(base + 0x314) = 0;
    base[0x498] = 0;
    *(int *)(base + 0x49c) = 0;
    base[0x620] = 0;
    *(int *)(base + 0x624) = 0;
    base[0x7a8] = 0;
    *(int *)(base + 0x7ac) = 0;
    base[0x930] = 0;
    *(int *)(base + 0x934) = 0;
    base[0xab8] = 0;
    *(int *)(base + 0xabc) = 0;
}

void func_80240AAC(){}

// Accumulates positions from active map point entries and applies them
// to a camera or transform object via func_8049F168 / func_8049F204.
void func_80240B10(MapPointEntry* entries, void* target) {
    if (cf::CfGameManager::func_800829B8()) {
        return;
    }

    ml::CVec3 accumPos = ml::CVec3::zero;
    ml::CVec3 accumPos2 = ml::CVec3::zero;
    bool anyActive = false;

    // 8 entries, processed as 4 pairs
    for (int i = 0; i < 4; i++) {
        MapPointEntry* entryA = &entries[i * 2];
        MapPointEntry* entryB = &entries[i * 2 + 1];

        if (entryA->flag != 0) {
            accumPos += entryA->pos;
            accumPos2 += entryA->pos2;
            anyActive = true;
        }
        if (entryB->flag != 0) {
            accumPos += entryB->pos;
            accumPos2 += entryB->pos2;
            anyActive = true;
        }
    }

    if (anyActive) {
        func_8049F168(target, &accumPos);
        func_8049F204(target, &accumPos2);
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
s32 func_80240C98(MenuFxObj* entries, FxParams* params, u8 flagVal) {
    MenuFxObj* obj = entries;
    for (int i = 0; i < 8; i++, obj++) {
        if (obj->field_00 != 0) continue;

        InitFxChannel(&obj->fx[0], params, params->field_04, params->field_2c);
        InitFxChannel(&obj->fx[1], params, params->field_08, params->field_2c);
        InitFxChannel(&obj->fx[2], params, params->field_0c, params->field_2c);

        // Channels 3-5 take pi/2-scaled widths from the reserved param block.
        float pi2 = lbl_eu_8066A210;
        float w3 = params->field_14 * pi2;
        float w4 = params->field_10 * pi2;
        float w5 = params->field_18 * pi2;
        InitFxChannel(&obj->fx2[0], params, w3, params->field_30);
        InitFxChannel(&obj->fx2[1], params, w4, params->field_30);
        InitFxChannel(&obj->fx2[2], params, w5, params->field_30);

        obj->field_00 = 1;

        const float eps = lbl_eu_8066A208;
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
    }
    return 0;
}

void func_8024125C(){}

// Resets active landmark entries matching id (or all when id == 0):
// clears each live slot counter and zeroes the stored positions.
void func_80241344(MapPointEntry* entries, u32 id) {
    MapPointEntry* e = entries;
    for (int i = 0; i < 8; i++, e++) {
        if (e->flag == 0) continue;
        if (!(id == e->id || id == 0)) continue;
        MapPointSlot* s = e->slots;
        for (int j = 0; j < 6; j++, s++) {
            u8 stop = s->value <= lbl_eu_8066A208;
            if (!stop) s->flag = 0;
        }
        e->pos = ml::CVec3::zero;
        e->pos2 = ml::CVec3::zero;
        e->flag = 0;
        e->id = 0;
    }
}

// CMenuMapSelect::~CMenuMapSelect() - virtual destructor (D1/D2 merged; the
// class header declares the virtual dtor, the vtable entry binds to this
// extern "C" symbol). Destroys subobjects in reverse declaration order:
// CFloorMap at +0x194, CFade at +0x16C, CMapSel at +0xB8, CTitleAHelp at +0x80,
// CBgTex at +0x60, then the CProcess base, then conditionally frees memory.
// The nested CProcess null-checks reproduce MWCC's D2-inlined-into-D1
// artifacts (same shape as __dt__9CMainMenuFv in CMainMenu.cpp).
extern "C" void __dt__9CFloorMapFv(void*, int);
extern "C" void __dt__5CFadeFv(void*, int);
extern "C" void __dt__7CMapSelFv(void*, int);
extern "C" void __dt__11CTitleAHelpFv(void*, int);
extern "C" void __dt__6CBgTexFv(void*, int);
extern "C" void __dt__8CProcessFv(void*, int);
extern "C" void* __dt__14CMenuMapSelectFv(CMenuMapSelect* _this, int flags) {
    if (_this) {
        __dt__9CFloorMapFv((char*)_this + 0x194, -1);
        __dt__5CFadeFv((char*)_this + 0x16C, -1);
        __dt__7CMapSelFv((char*)_this + 0xB8, -1);
        __dt__11CTitleAHelpFv((char*)_this + 0x80, -1);
        __dt__6CBgTexFv((char*)_this + 0x60, -1);
        if (_this) {
            if (_this) {
                __dt__8CProcessFv(_this, 0);
            }
        }
        if (flags > 0) {
            __dl__FPv(_this);
        }
    }
    return _this;
}

void CMenuMapSelect::Init() {
    func_8008294C__Q22cf13CfGameManagerFv(1);

    {
        CBgTex bgTex(0);
        func_801BE108(&mBgTex, &bgTex);
    } // bgTex destructor runs here
    mBgTex.func_801C3C14();

    char* helpStr = (char*)func_80136190(lbl_eu_8050B498, lbl_eu_8050B498 + 8, 1);
    {
        CTitleAHelp titleHelp(helpStr, 0x4a);
        func_801BE16C(&mTitleHelp, &titleHelp);
    } // titleHelp destructor runs here
    mTitleHelp.CTitleAHelp_load();

    {
        CMapSel mapSel;
        func_80241640(&mMapSel, &mapSel);
    } // mapSel destructor runs here
    func_8024343C(&mMapSel);

    {
        CFade fade;
        func_8024189C(&mFade, &fade);
    } // fade destructor runs here
    func_8024439C(&mFade);

    {
        CFloorMap floorMap;
        func_80241920(&mFloorMap, &floorMap);
    } // floorMap destructor runs here

    char* cb = (char*)this;
    if (this != 0) cb = (char*)this + 0x58;
    mScn->addRenderCB((IScnRender*)cb, 0xd, 1);
}

// Copy helpers for member sub-object initialization (retail calls these from
// Init; empty stubs must not be inlined away at -O4,p, so disable auto-inline
// for their definitions).
#pragma push
#pragma auto_inline off
void func_80241640(CMapSel* dest, CMapSel* src) {
    // Memberwise copy matching retail's per-field widths
    CMapSelCopyView& d = *reinterpret_cast<CMapSelCopyView*>(dest);
    const CMapSelCopyView& s = *reinterpret_cast<const CMapSelCopyView*>(src);

    // Read every field into locals first (retail hoists all loads above the
    // stores, spilling to the stack under register pressure).
    const u8 b5B = s.f5B;
    const u8 b70 = s.f70;
    const u8 b59 = s.f59;
    const u32 w780 = s.f78[0];
    const u32 w781 = s.f78[1];
    const u32 w782 = s.f78[2];
    const u32 w783 = s.f78[3];
    const u8 b88 = s.f88;
    const u8 b89 = s.f89;
    const u8 b8C = s.f8C;
    const u8 b8D = s.f8D;
    const u32 w8E0 = s.f8E[0];
    const u32 w8E1 = s.f8E[1];
    const u32 w8E2 = s.f8E[2];
    const u32 w8E3 = s.f8E[3];
    const u32 w8E4 = s.f8E[4];
    const u32 w8E5 = s.f8E[5];
    const u32 wAA = s.fAA;
    const u8 bAE = s.fAE;
    const u8 bAF = s.fAF;
    const f32 fB0v = s.fB0;

    const u32 w04_0 = s.f04[0];
    const u32 w04_1 = s.f04[1];
    const u32 w04_2 = s.f04[2];
    const u32 w04_3 = s.f04[3];
    const u32 w04_4 = s.f04[4];
    const u32 w04_5 = s.f04[5];
    const u32 w04_6 = s.f04[6];
    const u32 w04_7 = s.f04[7];
    const u32 w04_8 = s.f04[8];
    const u32 w04_9 = s.f04[9];
    const u32 w04_10 = s.f04[10];
    const u8 b30 = s.f30;
    const u8 b31 = s.f31;
    const u8 b32 = s.f32;
    const u8 b33 = s.f33;
    const u32 w380 = s.f38[0];
    const u32 w381 = s.f38[1];
    const u32 w382 = s.f38[2];
    const u32 w383 = s.f38[3];
    const u32 w384 = s.f38[4];
    const u32 w385 = s.f38[5];
    const u32 w386 = s.f38[6];
    const u32 w387 = s.f38[7];
    const u8 b58 = s.f58;
    const u8 b5A = s.f5A;
    const f32 f5Cv = s.f5C;
    const f32 f60v = s.f60;
    const f32 f64v = s.f64;
    const f32 f68v = s.f68;
    const f32 f6Cv = s.f6C;

    d.f5B = b5B;
    d.f70 = b70;
    d.f59 = b59;
    d.f78[0] = w780;
    d.f78[1] = w781;
    d.f78[2] = w782;
    d.f78[3] = w783;
    d.f88 = b88;
    d.f89 = b89;
    d.f8C = b8C;
    d.f8D = b8D;
    d.f8E[0] = w8E0;
    d.f8E[1] = w8E1;
    d.f8E[2] = w8E2;
    d.f8E[3] = w8E3;
    d.f8E[4] = w8E4;
    d.f8E[5] = w8E5;
    d.fAA = wAA;
    d.fAE = bAE;
    d.fAF = bAF;
    d.fB0 = fB0v;

    d.f04[0] = w04_0;
    d.f04[1] = w04_1;
    d.f04[2] = w04_2;
    d.f04[3] = w04_3;
    d.f04[4] = w04_4;
    d.f04[5] = w04_5;
    d.f04[6] = w04_6;
    d.f04[7] = w04_7;
    d.f04[8] = w04_8;
    d.f04[9] = w04_9;
    d.f04[10] = w04_10;
    d.f30 = b30;
    d.f31 = b31;
    d.f32 = b32;
    d.f33 = b33;
    d.f38[0] = w380;
    d.f38[1] = w381;
    d.f38[2] = w382;
    d.f38[3] = w383;
    d.f38[4] = w384;
    d.f38[5] = w385;
    d.f38[6] = w386;
    d.f38[7] = w387;
    d.f58 = b58;
    d.f5A = b5A;
    d.f5C = f5Cv;
    d.f60 = f60v;
    d.f64 = f64v;
    d.f68 = f68v;
    d.f6C = f6Cv;
}

void func_8024189C(CFade* dest, CFade* src){}

// Memberwise copy of a fully constructed CFloorMap into the process member
// (retail copy helper used by Init). Field widths follow the retail copy; the
// two large pair arrays use loop copies and the trailing word pair at
// +0x3340/+0x3344 is stored in reverse order.
void func_80241920(CFloorMap* dest, CFloorMap* src) {
    CFloorMapCopyView& d = *reinterpret_cast<CFloorMapCopyView*>(dest);
    const CFloorMapCopyView& s = *reinterpret_cast<const CFloorMapCopyView*>(src);

    d.f04[0] = s.f04[0];
    d.f04[1] = s.f04[1];
    d.f04[2] = s.f04[2];
    d.f04[3] = s.f04[3];
    d.f04[4] = s.f04[4];
    d.f04[5] = s.f04[5];
    d.f04[6] = s.f04[6];
    d.f04[7] = s.f04[7];
    d.f04[8] = s.f04[8];
    d.f04[9] = s.f04[9];
    d.f04[10] = s.f04[10];
    d.f04[11] = s.f04[11];
    d.f04[12] = s.f04[12];
    d.f04[13] = s.f04[13];
    d.f04[14] = s.f04[14];
    d.f40 = s.f40;
    d.f41 = s.f41;
    d.f42 = s.f42;
    d.f43 = s.f43;
    d.f44[0] = s.f44[0];
    d.f44[1] = s.f44[1];
    d.f44[2] = s.f44[2];
    d.f44[3] = s.f44[3];
    d.f44[4] = s.f44[4];
    d.f58 = s.f58;
    d.f5A = s.f5A;
    d.f5C = s.f5C;
    d.f5D = s.f5D;
    d.f64[0] = s.f64[0];
    d.f64[1] = s.f64[1];
    d.f64[2] = s.f64[2];
    d.f64[3] = s.f64[3];
    d.f64[4] = s.f64[4];
    d.f64[5] = s.f64[5];
    d.f64[6] = s.f64[6];
    d.f64[7] = s.f64[7];
    d.f84 = s.f84;
    d.f85 = s.f85;
    d.f86 = s.f86;
    d.f87 = s.f87;
    d.f88[0] = s.f88[0];
    d.f88[1] = s.f88[1];
    d.f88[2] = s.f88[2];
    d.f88[3] = s.f88[3];
    d.f88[4] = s.f88[4];
    d.f9C = s.f9C;
    d.fA4[0] = s.fA4[0];
    d.fA4[1] = s.fA4[1];
    d.fA4[2] = s.fA4[2];
    u32 b0v = s.fB0;
    int n1 = 20;   // lands between the +0xB0 load and store, as in retail
    d.fB0 = b0v;
    d.fB4 = s.fB4;
    d.fB5 = s.fB5;
    d.fBC[0] = s.fBC[0];
    d.fBC[1] = s.fBC[1];
    d.fBC[2] = s.fBC[2];
    d.fBC[3] = s.fBC[3];
    d.fBC[4] = s.fBC[4];
    d.fBC[5] = s.fBC[5];
    d.fBC[6] = s.fBC[6];
    d.fBC[7] = s.fBC[7];
    d.fBC[8] = s.fBC[8];
    d.fE0 = s.fE0;
    d.fE4[0] = s.fE4[0];
    d.fE4[1] = s.fE4[1];
    d.fEC[0] = s.fEC[0];
    d.fEC[1] = s.fEC[1];
    d.fEC[2] = s.fEC[2];
    d.fEC[3] = s.fEC[3];
    d.fEC[4] = s.fEC[4];
    d.fEC[5] = s.fEC[5];
    d.fF8[0] = s.fF8[0];
    d.fF8[1] = s.fF8[1];
    d.fF8[2] = s.fF8[2];
    d.fF8[3] = s.fF8[3];
    d.fF8[4] = s.fF8[4];
    d.fF8[5] = s.fF8[5];
    d.fF8[6] = s.fF8[6];
    d.fF8[7] = s.fF8[7];
    d.fF8[8] = s.fF8[8];
    d.f11C = s.f11C;
    d.f120[0] = s.f120[0];
    d.f120[1] = s.f120[1];
    d.f128[0] = s.f128[0];
    d.f128[1] = s.f128[1];
    d.f128[2] = s.f128[2];
    d.f128[3] = s.f128[3];
    d.f128[4] = s.f128[4];
    d.f128[5] = s.f128[5];
    d.f130[0] = s.f130[0];
    d.f130[1] = s.f130[1];
    d.f130[2] = s.f130[2];
    d.f130[3] = s.f130[3];
    d.f130[4] = s.f130[4];
    d.f130[5] = s.f130[5];
    d.f130[6] = s.f130[6];
    d.f14C = s.f14C;

    // Retail keeps these as ctr loops over 8-byte records; the countdown
    // do-while form avoids MWCC's unroller.
    const CFloorMapCopyPair* sp = s.loop1;
    CFloorMapCopyPair* dp = d.loop1;
    do {
        dp->a = sp->a;
        dp->b = sp->b;
        dp++;
        sp++;
    } while (--n1);

    d.f1F0 = s.f1F0;
    d.f1F4 = s.f1F4;
    d.f1F8 = s.f1F8;
    d.f1FC = s.f1FC;
    d.f200 = s.f200;
    d.f204 = s.f204;
    d.f205 = s.f205;
    d.f206 = s.f206;
    d.f207 = s.f207;
    d.f208 = s.f208;

    const CFloorMapCopyPair* sp2 = s.loop2;
    CFloorMapCopyPair* dp2 = d.loop2;
    int n2 = 0x618;
    do {
        dp2->a = sp2->a;
        dp2->b = sp2->b;
        dp2++;
        sp2++;
    } while (--n2);

    d.f32CC = s.f32CC;
    d.f32D4[0] = s.f32D4[0];
    d.f32D4[1] = s.f32D4[1];
    d.f32D4[2] = s.f32D4[2];
    d.f32D4[3] = s.f32D4[3];
    d.f32E4 = s.f32E4;
    d.f32E5 = s.f32E5;
    d.f32EC[0] = s.f32EC[0];
    d.f32EC[1] = s.f32EC[1];
    d.f32EC[2] = s.f32EC[2];
    d.f32EC[3] = s.f32EC[3];
    d.f32FC = s.f32FC;
    d.f32FD = s.f32FD;
    d.f3304[0] = s.f3304[0];
    d.f3304[1] = s.f3304[1];
    d.f3304[2] = s.f3304[2];
    d.f3304[3] = s.f3304[3];
    d.f3314 = s.f3314;
    d.f3315 = s.f3315;
    d.f331C[0] = s.f331C[0];
    d.f331C[1] = s.f331C[1];
    d.f331C[2] = s.f331C[2];
    d.f331C[3] = s.f331C[3];
    d.f332C = s.f332C;
    d.f332D = s.f332D;
    d.f3330[0] = s.f3330[0];
    d.f3330[1] = s.f3330[1];
    d.f3330[2] = s.f3330[2];
    d.f333C = s.f333C;

    // Retail stores +0x3344 before +0x3340.
    u32 v3344 = s.f3344;
    u32 v3340 = s.f3340;
    d.f3344 = v3344;
    d.f3340 = v3340;
}
#pragma pop

void CMenuMapSelect::Term() {
    waitForDrawDone__9CDeviceVIFv();
    func_804962A0(mScn, 1);

    IScnRender* cb = reinterpret_cast<IScnRender*>(this);
    if (this != 0) {
        cb = reinterpret_cast<IScnRender*>((char*)this + 0x58);
    }
    mScn->removeRenderCB(cb);

    func_801C3D9C(&mBgTex);
    func_801C40A0(&mTitleHelp);
    func_802435CC(&mMapSel);
    func_8024448C(&mFade);
    func_8024CB94(&mFloorMap);

    lbl_eu_80664790 = 0;
    func_8008294C__Q22cf13CfGameManagerFv(0);
}

// CMenuMapSelect state machine. States: 0 waits for bg/help/map resources,
// 1 waits for help idle then arms the map, 2 runs the world map handler,
// 3 waits help idle to raise mField54, 4 world map phase 2, 5 starts fade-in
// for the floor map, 6 waits for the fade, 7 floor map handler, 8 starts
// fade-out back to the world map, 9 waits for that fade, 10 exits the scene,
// 11 marks completion in mField54.
void CMenuMapSelect::Move() {
    if (CTaskGame::getInstance()->func_800426F0() || (lbl_eu_80663E28 & 0x200000)) {
        return;
    }

    switch (mState) {
    case 0:
        if (func_801C3E34(&mBgTex) == 0) break;
        if (func_801C4114(&mTitleHelp) == 0) break;
        if (func_80243680(&mMapSel) == 0) break;
        func_801C412C(&mTitleHelp);
        func_802436CC(&mMapSel);
        mState = 1;
        func_80138078__FUl(0x6d);
        func_8013EC6C(1, 0);
        func_804962A0(mScn, 0);
        break;

    case 1:
        if (isIdle__11CTitleAHelpFv(&mTitleHelp) == 0) break;
        if (func_802436C4(&mMapSel) == 0) break;
        mState = 2;
        break;

    case 2:
        func_80242368(this);
        break;

    case 3:
        if (isIdle__11CTitleAHelpFv(&mTitleHelp) == 0) break;
        if (func_802436C4(&mMapSel) == 0) break;
        mField54 = 1;
        break;

    case 4:
        func_80242524(this);
        break;

    case 5:
        if (func_8024CE1C(&mFloorMap) == 0) break;
        func_801C4654(&mTitleHelp, 0);
        func_801C46B4(&mTitleHelp, func_80136190(lbl_eu_8050B498, lbl_eu_8050B498 + 8, 3));
        if (func_8009CF8C((u32)0x3212) != 0) {
            func_801C46DC(&mTitleHelp, 1);
        }
        func_801C41E8(&mTitleHelp, 0);
        func_8024BE1C(&mFloorMap);
        func_80138078__FUl(0x6d);
        func_80244538(&mFade);
        mState = 6;
        break;

    case 6:
        if (func_80244510(&mFade) == 0) break;
        mState = 7;
        break;

    case 7:
        func_80242A28(this);
        break;

    case 8:
        if (func_80244510(&mFade) == 0) break;
        func_8024CB94(&mFloorMap);
        func_801C4654(&mTitleHelp, 1);
        func_801C46B4(&mTitleHelp, func_80136190(lbl_eu_8050B498, lbl_eu_8050B498 + 8, 3));
        func_801C46DC(&mTitleHelp, 0);
        func_801C41E8(&mTitleHelp, 0x4a);
        func_80244538(&mFade);
        mState = 9;
        break;

    case 9:
        if (func_80244510(&mFade) == 0) break;
        mState = 2;
        break;

    case 10:
        if (func_80244510(&mFade) == 0) break;
        func_8008413C__Q22cf13CfGameManagerFv(func_8024F54C(&mFloorMap), 0);
        mState = 0xb;
        if (func_800FF738()) {
            func_800FEDF8();
            func_800FF914();
        }
        break;

    case 11:
        mField54 = 1;
        break;
    }

    if (func_8024CE60(&mFloorMap) == 0) {
        func_801C3D54(&mBgTex);
        func_802434A0(&mMapSel);
    }
    func_801C3FF0(&mTitleHelp);
    func_802443E8(&mFade);
    func_8024C1FC(&mFloorMap);
}

void CMenuMapSelect::cbRenderBefore() {}
