// Auto-scaffolded catalog TU for kyoshin/code_801A929C
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/plugin/ocBdat.hpp"  // getBdatStringColumnValue
#include "monolib/math/CVec3.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)
#include <math.h>

// SDA/sdata2 float pool labels used by the direction helpers below.
extern const f32 lbl_eu_80667D60; // 0.0f
extern const f32 lbl_eu_80667D64; // 1.0f
extern const f32 lbl_eu_80667D68;
extern const f32 lbl_eu_80667D6C; // rad -> FIdx degrees
extern const f32 lbl_eu_80667D70; // FIdx degrees multiplier for SinFIdx
extern const f32 lbl_eu_80667D78; // bdat column float scale
extern const f64 lbl_eu_80667D80; // 0x4330 double-conversion bias A
extern const f64 lbl_eu_80667D88; // 0x4330 double-conversion bias B
extern const f32 lbl_eu_8066A1F8; // pi
extern const f32 lbl_eu_8066A1FC; // 2*pi

// Global runtime-flag word (bit 0x2000 = bit 13 gates target selection).

// Shared ctrl-state singleton pointer (retail .sdata).
extern unsigned char lbl_eu_80664330;

// Retail C-symbol import: camera/scene gate query.
extern "C" int func_8007BAE4();

extern "C" void __dl__FPv(void*);

void func_801A9338(void* self) {
    *(unsigned short*)((char*)self + 0x2c) = 0xFFFF;
}

// Retail exports the blend helper under a plain C symbol, so the definition
// keeps C linkage for call-site reloc parity.

// ----------------------------------------------------------------------------
// func_801A9348 - compute a blend weight and attack direction toward a target.
// Weight falls linearly from 1 to 0 between the outer/inner radii; direction
// comes either from the source matrix transform or, when the shared target
// singleton is active, from wrapped atan2 angle differences.
// ----------------------------------------------------------------------------
// Source object carrying a transform matrix (+0xCC) and position (+0x10C).
struct DirSrc {
    u8 _00[0xCC];
    u8 mMatrix[0x40]; // +0xCC, Mtx44-style 3x4 matrix
    ml::CVec3 mPos;   // +0x10C
};

// Helper: view a raw byte block as an SDK matrix.
inline Mtx* mtxOf(u8* p) { return (Mtx*)p; }

// Shared target singleton view: current + previous positions and active flag.
struct DirTargetInfo {
    ml::CVec3 m00;  // +0x00
    ml::CVec3 m0C;  // +0x0C
    u8 mFlag18;     // +0x18
};

extern "C" void func_801A9348(ml::CVec3* outDir, float* outWeight, DirSrc* src,
                   ml::CVec3* posB, ml::CVec3* targetPos,
                   float innerRadius, float outerRadius) {

    // Choose the aim point: when the presentation flag is clear and the camera
    // gate query passes, blend the source position with posB; otherwise aim at
    // the shared target singleton (when active) or the source itself.
    // NOTE: retail re-reads the singleton global at each use (no caching).
    s32 useTargetObj = 0;
    ml::CVec3 aim;
    if ((lbl_eu_80663E24 & 0x40000) != 0 || func_8007BAE4() != 0) {
        DirTargetInfo* pTarget = *(DirTargetInfo**)(&lbl_eu_80664330);
        if (pTarget != 0 && pTarget->mFlag18 != 0) {
            useTargetObj = 1;
            aim = pTarget->m00;
        } else {
            aim = src->mPos;
        }
    } else {
        aim = (src->mPos + *posB) * lbl_eu_80667D68;
    }

    float mag = PSVECMag(*targetPos - aim);

    // Linear falloff: 0 at/below innerRadius, 1 at/beyond outerRadius.
    *outWeight = lbl_eu_80667D64;
    if (mag < outerRadius) {
        if (mag < innerRadius) {
            *outWeight = lbl_eu_80667D60;
        } else {
            *outWeight =
                lbl_eu_80667D60 - (mag - innerRadius) / (outerRadius - innerRadius);
        }
    }

    if (useTargetObj != 0) {
        // Direction from the signed difference of two heading angles around
        // the target object, wrapped into [-pi, pi]. Each block re-reads the
        // singleton global, matching retail's separate loads around the calls.
        float angSelf;
        {
            DirTargetInfo* t = *(DirTargetInfo**)(&lbl_eu_80664330);
            ml::CVec3 v1 = t->m0C - t->m00;
            angSelf = lbl_eu_80667D6C * nw4r::math::Atan2FIdx(v1.x, v1.z);
        }
        float delta;
        {
            DirTargetInfo* t = *(DirTargetInfo**)(&lbl_eu_80664330);
            ml::CVec3 v2 = *targetPos - t->m0C;
            delta = lbl_eu_80667D6C * nw4r::math::Atan2FIdx(v2.x, v2.z) - angSelf;
        }
        while (lbl_eu_8066A1F8 <= delta) delta -= lbl_eu_8066A1FC;
        while (delta < -lbl_eu_8066A1F8) delta += lbl_eu_8066A1FC;
        outDir->x = -nw4r::math::SinFIdx(lbl_eu_80667D70 * delta);
    } else {
        // Transform the target through the source matrix and normalize; fall
        // back to the zero vector when the length squared matches exactly.
        ml::CVec3 dir;
        PSMTXMultVec(*mtxOf(src->mMatrix), (Vec*)targetPos, (Vec*)&dir);
        float lenSq = dir.y * dir.y + (dir.x * dir.x + dir.z * dir.z);
        if (lenSq == lbl_eu_80667D64) {
            dir = ml::CVec3::zero;
        } else {
            PSVECNormalize((Vec*)&dir, (Vec*)&dir);
        }
        outDir->x = dir.x;
    }
}

// ----------------------------------------------------------------------------
// func_801A96A0 - per-frame update of one ctrl-sound state object.
// Picks the listener aim point (game-manager position, overridden by the
// shared ctrl-state singleton when presentation bit 13 / camera gate allow),
// optionally syncs the entry position from a map object, then either scans
// bdat rows for the nearest matching position (mode 2), stops the entry sound
// when close enough, or (re)starts it and applies distance/flag volume.
// ----------------------------------------------------------------------------

// One 0x38-byte ctrl-sound state object (the unit's per-entry record).
struct SndCtrlObj {
    ml::CVec3 mVec0; // +0x00 current position
    ml::CVec3 mVec1; // +0x0C previous/best position
    f32 m18;         // +0x18 distance gate
    f32 m1C;         // +0x1C blend weight / volume factor
    f32 m20;         // +0x20 direction (written through func_801A9348)
    f32 m24;         // +0x24 base volume
    f32 m28;         // +0x28 stored volume scale
    u16 m2C;         // +0x2C sound handle (0xFFFF = stopped)
    u16 m2E;         // +0x2E sound id
    u16 m30;         // +0x30 bdat row index
    u8 m32;          // +0x32 fade counter
    u8 m33;          // +0x33 mode (0=volume path, 2=nearest search)
    u8 m34;          // +0x34 flag: alternate play entry
    u8 m35;          // +0x35 map-object id for position sync
    u8 pad36;
    u8 m37;          // +0x37 bit0: far-volume attenuation flag
};

// Game-manager query chain: root -> +0xC node -> position at +0x10C.
struct GmPosNode {
    u8 _00[0x10C];
    ml::CVec3 mPos;
};
struct GmRoot {
    u8 _00[0xC];
    GmPosNode* field_C;
};

// Map object returned by func_80186BC8: only the virtual at vtable offset
// 0xB0 is invoked; it returns the object's current position triplet. With the
// RTTI header word the 43rd declared virtual sits at +0xB0.
struct SndMapObjVt {
    virtual void vf00() = 0;
    virtual void vf01() = 0;
    virtual void vf02() = 0;
    virtual void vf03() = 0;
    virtual void vf04() = 0;
    virtual void vf05() = 0;
    virtual void vf06() = 0;
    virtual void vf07() = 0;
    virtual void vf08() = 0;
    virtual void vf09() = 0;
    virtual void vf10() = 0;
    virtual void vf11() = 0;
    virtual void vf12() = 0;
    virtual void vf13() = 0;
    virtual void vf14() = 0;
    virtual void vf15() = 0;
    virtual void vf16() = 0;
    virtual void vf17() = 0;
    virtual void vf18() = 0;
    virtual void vf19() = 0;
    virtual void vf20() = 0;
    virtual void vf21() = 0;
    virtual void vf22() = 0;
    virtual void vf23() = 0;
    virtual void vf24() = 0;
    virtual void vf25() = 0;
    virtual void vf26() = 0;
    virtual void vf27() = 0;
    virtual void vf28() = 0;
    virtual void vf29() = 0;
    virtual void vf30() = 0;
    virtual void vf31() = 0;
    virtual void vf32() = 0;
    virtual void vf33() = 0;
    virtual void vf34() = 0;
    virtual void vf35() = 0;
    virtual void vf36() = 0;
    virtual void vf37() = 0;
    virtual void vf38() = 0;
    virtual void vf39() = 0;
    virtual void vf40() = 0;
    virtual void vf41() = 0;
    virtual ml::CVec3* vf42_getPos() = 0; // vtable +0xB0
};

// Scene pose block carrying a D80-flagged volume object pointer.
struct D80VolObj {
    u8 _00[0xC];
    f32 field_0xC;
};

// Minimal BasicSound interface: only SetPlayerPriority is dereferenced.
namespace nw4r {
namespace snd {
namespace detail {
class BasicSound {
public:
    void SetPlayerPriority(int priority);
};
} // namespace detail
} // namespace snd
} // namespace nw4r

// Sound-slot record returned by func_801BFAE4 (+0x00 active sound object).
struct SndSlotRef {
    nw4r::snd::detail::BasicSound* mSound;
};

union SndConv {
    u32 w[2];
    f64 d;
};

void* func_80186BC8(int id);
extern "C" u32 func_800821F8__Q22cf13CfGameManagerFv();
int CfRes_getD80Flag();
D80VolObj* func_8049603C();
extern "C" float func_800A47C8(const ml::CVec3& a, const ml::CVec3& b,
                               const ml::CVec3& c, float* outT, ml::CVec3* out);
SndSlotRef* func_801BFAE4(u16 handle);
extern "C" void func_801BFED0(int a, u16 b, int c);
// func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf is declared by the catalog
// (CCol6System.hpp): returns the sound handle.
extern "C" void func_801BFE58(s32 idx, u32 handle, u32 ticks, float volume);
extern "C" void func_801BFF44(s32 idx, u16 handle, float value);
extern const char lbl_eu_80526324[]; // nw4r::db::Warning source string
extern const char lbl_eu_80526300[]; // nw4r::db::Warning format string
extern char lbl_eu_80503FA0[];       // bdat column-name blob
struct BdatTable;
extern BdatTable* lbl_eu_806640B8;
extern u32 lbl_eu_8057164C[3];       // reference aim-point vector
extern f32 lbl_eu_80667D74;          // initial best-distance sentinel
extern f32 lbl_eu_8066A208;          // column-match threshold

extern "C" void func_801A96A0(SndCtrlObj* self, int unk4, int farArg,
                               int entryBase) {
    GmRoot* root = (GmRoot*)func_800821F8__Q22cf13CfGameManagerFv();
    if (root == NULL) {
        return;
    }
    ml::CVec3 aim = root->field_C->mPos;

    if (self->m35 != 0) {
        SndMapObjVt* obj = (SndMapObjVt*)func_80186BC8(self->m35);
        if (obj == NULL) {
            return;
        }
        self->mVec0 = *obj->vf42_getPos();
    }

    // Presentation bit 13 or the camera gate enables the shared-singleton
    // aim override.
    if ((lbl_eu_80663E24 & 0x40000) != 0 || func_8007BAE4() != 0) {
        DirTargetInfo* tgt = *(DirTargetInfo**)(&lbl_eu_80664330);
        if (tgt != NULL && tgt->mFlag18 != 0) {
            aim = tgt->m00;
        }
    }

    self->mVec1 = self->mVec0;

    if (self->m33 != 0) {
        if (self->m33 == 2) {
            // Nearest-position scan: rows self->m30 of the shared bdat table,
            // columns built from the name blob + index suffix (signed columns
            // converted through the 0x43300000 double trick).
            BdatTable* tbl = lbl_eu_806640B8;
            f32 best = lbl_eu_80667D74;
            f64 bias = lbl_eu_80667D80;
            f32 scale = lbl_eu_80667D78;
            f32 thr = lbl_eu_8066A208;
            ml::CVec3 prev = self->mVec0;
            char colStorage[sizeof(ml::FixStr<32>)];
            ml::FixStr<32>& col = *reinterpret_cast<ml::FixStr<32>*>(colStorage);
            SndConv conv;
            conv.w[0] = 0x43300000;
            for (s32 i = 1; i <= 10; i++) {
                ml::CVec3 cur;
                col.format(&lbl_eu_80503FA0[0], (char)i);
                conv.w[1] = getBdatStringColumnValue(tbl, colStorage, self->m30) ^ 0x8000;
                cur.x = (f32)(conv.d - bias) * scale;

                col.format(&lbl_eu_80503FA0[9], (char)i);
                conv.w[1] = getBdatStringColumnValue(tbl, colStorage, self->m30) ^ 0x8000;
                cur.y = (f32)(conv.d - bias) * scale;

                col.format(&lbl_eu_80503FA0[0x12], (char)i);
                conv.w[1] = getBdatStringColumnValue(tbl, colStorage, self->m30);
                cur.z = (f32)(conv.d - bias) * scale;

                // All three coordinates within threshold: accept the current
                // best and stop scanning.
                int allClose = 0;
                if ((f32)__fabs((f64)cur.x) < thr && (f32)__fabs((f64)cur.y) < thr) {
                    if ((f32)__fabs((f64)cur.z) < thr) {
                        allClose = 1;
                    }
                }
                if (allClose == 0) {
                    float tOut;
                    ml::CVec3 closest;
                    f32 dist = func_800A47C8(prev, cur, aim, &tOut, &closest);
                    if (dist < best) {
                        best = dist;
                        self->mVec1 = closest;
                    }
                }
                prev = cur;
            }
        }

        // Distance gate against the updated position.
        ml::CVec3 diff = self->mVec1 - aim;
        f32 lenSq = diff.x * diff.x + diff.z * diff.z;
        if (lenSq >= lbl_eu_80667D64) {
            nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
        }
        f32 dist;
        if (lenSq < lbl_eu_80667D64) {
            dist = lenSq * nw4r::math::FrSqrt(lenSq);
        } else {
            dist = lbl_eu_80667D64;
        }
        // Close enough (or y-delta small): stop the entry sound and clear it.
        if (dist >= self->m18 || (f32)__fabs((f64)diff.y) < self->m18) {
            if (self->m2C != 0xFFFF) {
                func_801BFED0(1, self->m2C, 0);
            }
            self->m2C = 0xFFFF;
            return;
        }
    }

    // Start the sound if nothing is playing (priority depends on the mode).
    if (self->m2C == 0xFFFF) {
        self->m20 = lbl_eu_80667D64;
        self->m1C = self->m24;
        u32 prio = 0x78;
        if (self->m33 != 0) {
            self->m1C = lbl_eu_80667D64;
            prio = 0;
        }
        u16 h = func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(1, self->m2E, prio, 0,
                                                           self->m1C);
        self->m2C = h;
        SndSlotRef* slot = (SndSlotRef*)func_801BFAE4((u16)h);
        if (slot != NULL && slot->mSound != NULL) {
            slot->mSound->SetPlayerPriority(0x50);
        }
    }

    if (self->m2C == 0xFFFF) {
        return;
    }

    if (self->m33 == 0) {
        // Looping path: plain distance-attenuated volume updates.
        f32 vol = self->m24;
        if (CfRes_getD80Flag()) {
            vol *= lbl_eu_80667D60 - func_8049603C()->field_0xC;
        }
        if ((lbl_eu_80663E24 & 0x40000) != 0) {
            if (self->m32 != 0) {
                self->m32--;
                return;
            }
            vol *= self->m28;
        } else {
            self->m28 = lbl_eu_80667D60;
            self->m32 = 0;
        }
        if (self->m34 != 0) {
            if (farArg == 0) {
                vol *= lbl_eu_80667D68;
            }
        } else {
            if ((self->m37 & 1) != 0 && farArg == 0) {
                vol *= lbl_eu_80667D68;
            }
        }
        func_801BFE58(1, self->m2C, 0x1e, vol);
        return;
    }

    // Directed path: recompute the blend toward the scene pose target, then
    // apply the same D80/presentation-bit volume chain.
    DirSrc* pose = (DirSrc*)func_80496264(lbl_eu_80663E14, -1);
    func_801A9348((ml::CVec3*)&self->m20, &self->m1C, pose,
                  (ml::CVec3*)lbl_eu_8057164C, &self->mVec1,
                  lbl_eu_80667D68, lbl_eu_80667D68 + self->m18);
    f32 vol = self->m1C * self->m24;
    if (CfRes_getD80Flag()) {
        vol *= lbl_eu_80667D60 - func_8049603C()->field_0xC;
    }
    if ((lbl_eu_80663E24 & 0x40000) != 0) {
        if (self->m32 != 0) {
            self->m32--;
        }
        vol *= self->m28;
    } else {
        self->m28 = lbl_eu_80667D60;
        self->m32 = 0;
    }
    if (self->m32 == 0) {
        func_801BFE58(1, self->m2C, 0, vol);
    }
    func_801BFF44(1, self->m2C, self->m20);
}

// ----------------------------------------------------------------------------
// func_801A9CCC - construct the 128-entry state work object: fill the header
// floats/flags, default-initialize every entry, then invoke the shared
// member-function callback (pointer-to-member-function blob in .data) on all
// 128 entries and publish the object as the singleton.
// ----------------------------------------------------------------------------
extern u32 lbl_eu_805333E0[3];

struct CtrlState {
    u8 _00[0x1C];
    f32 m1C; // +0x1C
    f32 m20; // +0x20
    u8 pad24[4];
    f32 m28; // +0x28
    s16 m2C; // +0x2C, init -1
    s16 m2E;
    s16 m30;
    u8 m32;
    u8 pad33[5];
};

class CtrlStateWork {
public:
    f32 m00; // +0x00
    f32 m04; // +0x04
    f32 m08;
    f32 m0C;
    f32 m10;
    f32 m14;
    u8 m18; // +0x18, cleared
    u8 m19;
    u8 m1A;
    u8 m1B;
    u16 m1C; // +0x1C, set to 1
    u8 pad1E[2];
    CtrlState mEntries[128]; // +0x20 .. +0x1C20
};

typedef void (CtrlState::*CtrlStateFn)();

// Word-level view so the three ptmf words load without a memcpy call.
union CtrlStateFnBits {
    CtrlStateFn fn;
    u32 raw[3];
};

CtrlStateWork* func_801A9CCC(CtrlStateWork* work) {
    work->m00 = lbl_eu_80667D64;
    work->m04 = lbl_eu_80667D60;
    work->m08 = lbl_eu_80667D64;
    work->m0C = lbl_eu_80667D64;
    work->m10 = lbl_eu_80667D64;
    work->m14 = lbl_eu_80667D64;
    work->m18 = 0;
    work->m19 = 0;
    work->m1A = 0;
    work->m1B = 0;
    work->m1C = 1;

    // Default-initialize all 128 entries.
    CtrlState* cur = &work->mEntries[0];
    CtrlState* stop = &work->mEntries[128];
    for (; cur < stop; ++cur) {
        cur->m1C = lbl_eu_80667D60;
        cur->m20 = lbl_eu_80667D64;
        cur->m28 = lbl_eu_80667D64;
        cur->m2C = -1;
        cur->m2E = 0;
        cur->m30 = 0;
        cur->m32 = 0;
    }

    work->m1C = 1;
    work->m18 = 0;

    // Load the shared pointer-to-member-function callback and invoke it on
    // every entry.
    CtrlStateFnBits cb;
    cb.raw[0] = lbl_eu_805333E0[0];
    cb.raw[1] = lbl_eu_805333E0[1];
    cb.raw[2] = lbl_eu_805333E0[2];
    CtrlStateFn fn = cb.fn;
    CtrlStateFnBits cbCall;
    cbCall.fn = fn;
    CtrlState* end = &work->mEntries[128];
    for (CtrlState* it = work->mEntries; it != end; ++it) {
        (it->*cbCall.fn)();
    }

    // Singleton pointer (declared as raw bytes elsewhere in this TU).
    extern unsigned char lbl_eu_80664330;
    *(CtrlStateWork**)(&lbl_eu_80664330) = work;
    return work;
}

// C linkage inherited from the CfGameManager.hpp declaration; repeating
// extern "C" here trips MWCC error 10197.
void* __dt__801A9F78(void* self, int dealloc) {
    if (self != nullptr) {
        extern unsigned char lbl_eu_80664330;
        *(void**)(&lbl_eu_80664330) = 0;
        if (dealloc > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

// ----------------------------------------------------------------------------
// func_801A9FC0 - run the shared member-function callback over all 128 work
// entries and reset the entry-count/flag header fields.
// ----------------------------------------------------------------------------
struct CtrlEntry {
    u8 mRaw[0x38];
};

class CtrlWork {
public:
    u8 _00[0x18];
    u8 mField18; // +0x18, cleared
    u8 _19[3];
    u16 mField1C; // +0x1C, set to 1
    u16 _1E; // padding so entries start at +0x20
    CtrlEntry mEntries[128]; // +0x20 .. +0x1C20
};

typedef void (CtrlEntry::*CtrlEntryFn)();

// 12-byte pointer-to-member-function in .data, invoked on every entry.
extern u32 lbl_eu_805333EC[3];

// Word-level view so the three ptmf words load without a memcpy call.
union CtrlEntryFnBits {
    CtrlEntryFn fn;
    u32 raw[3];
};

void func_801A9FC0(CtrlWork* work) {
    work->mField1C = 1;
    work->mField18 = 0;
    CtrlEntryFnBits cb;
    // Single aggregate copy of the 12-byte ptmf from .data.
    cb = *(CtrlEntryFnBits*)lbl_eu_805333EC;
    // Second ptmf copy: __ptmf_scall consumes a stack temp, matching retail.
    CtrlEntryFnBits cbCall;
    cbCall.fn = cb.fn;
    CtrlEntry* end = &work->mEntries[128];
    for (CtrlEntry* it = work->mEntries; it != end; ++it) {
        (it->*cbCall.fn)();
    }
}

// ----------------------------------------------------------------------------
// func_801AA04C - fill the ctrl-state work entries from the shared bdat table:
// for every row of the table's row range, read named columns (rodata name blob
// at lbl_eu_80503FA0) into one 0x38-byte entry slot of the caller's array.
// Integer columns are converted to float through the 0x43300000 double trick:
// the union high words are preset once up front, each value is placed (with
// the sign-bit xor for signed columns) into the low word, the double bias is
// subtracted, and the result scaled by lbl_eu_80667D78.
// ----------------------------------------------------------------------------

// Conversion scratch: w[0] = 0x43300000, w[1] holds the integer payload.
union BdatConv {
    u32 w[2];
    f64 d;
};

// Column-value view: retail spills every narrow-column call result to its own
// stack word, then re-reads it narrowed (stw/lhz or stw/lbz round-trip).
union ColVal {
    u32 v;
    u16 h[2];
    u8 b[4];
};

// Two views of the moving 0x38-byte slot: the three leading floats go through
// the head cursor, everything else through offsets 0x38.. of the base cursor
// (retail keeps both pointers live across the whole loop).
struct CtrlRecHead {
    f32 f00;
    f32 f04;
    f32 f08;
};

struct CtrlRecTail {
    u8 pad00[0x18];
    f32 f38;
    u8 pad3C[8];
    f32 f44;
    u16 f4C;
    u16 f4E;
    u16 f50;
    u8 f53;
    u8 f54;
    u8 f55;
    u8 f56;
    u8 f57;
};

// Bdat column-name blob (split1 rodata), byte-offset indexed.
extern char lbl_eu_80503FA0[];

// Bdat table handle owning the ctrl-state row range (import from split1).
struct BdatTable;
extern BdatTable* lbl_eu_806640B8;

void func_801AA04C(void* param) {
    BdatTable* tbl = lbl_eu_806640B8;
    BdatConv convA;
    BdatConv convB;
    convA.w[0] = 0x43300000;
    convB.w[0] = 0x43300000;
    s32 i = func_8003B41C(tbl);   // first row of the range
    s32 end = i + func_8003B1EC(tbl); // number of rows
    const char* cols = lbl_eu_80503FA0;

    CtrlRecHead* rec = (CtrlRecHead*)((u8*)param + i * 0x38);
    CtrlRecTail* tail = (CtrlRecTail*)((u8*)param + i * 0x38);

    for (; i < end; i++) {
        // Local struct: field stores keep stack homes across the calls
        // (retail stfs to sp), and the final copy is three lwz/stw moves.
        CtrlRecHead h;
        convA.w[1] = getBdatStringColumnValue(tbl, cols + 0x1b, i) ^ 0x8000;
        h.f00 = (f32)(convA.d - lbl_eu_80667D80) * lbl_eu_80667D78;
        convB.w[1] = getBdatStringColumnValue(tbl, cols + 0x20, i) ^ 0x8000;
        h.f04 = (f32)(convB.d - lbl_eu_80667D80) * lbl_eu_80667D78;
        convA.w[1] = getBdatStringColumnValue(tbl, cols + 0x2a, i) ^ 0x8000;
        h.f08 = (f32)(convA.d - lbl_eu_80667D80) * lbl_eu_80667D78;
        *rec = h;

        ColVal t31;
        t31.v = getBdatStringColumnValue(tbl, cols + 0x31, i);
        convB.w[1] = t31.h[0];
        tail->f38 = (f32)(convB.d - lbl_eu_80667D88);
        tail->f50 = (u16)i;
        ColVal t37;
        t37.v = getBdatStringColumnValue(tbl, cols + 0x37, i);
        tail->f4E = t37.h[0];
        ColVal t3d;
        t3d.v = getBdatStringColumnValue(tbl, cols + 0x3d, i);
        tail->f53 = t3d.b[0];
        ColVal t42;
        t42.v = getBdatStringColumnValue(tbl, cols + 0x42, i);
        tail->f56 = t42.b[0];
        ColVal t49;
        t49.v = getBdatStringColumnValue(tbl, cols + 0x49, i);
        convA.w[1] = t49.b[0];
        tail->f44 = (f32)(convA.d - lbl_eu_80667D88) * lbl_eu_80667D78;
        ColVal t50;
        t50.v = getBdatStringColumnValue(tbl, cols + 0x50, i);
        tail->f54 = t50.b[0];
        ColVal t57;
        t57.v = getBdatStringColumnValue(tbl, cols + 0x57, i);
        tail->f55 = t57.b[0];
        tail->f4C = 0xFFFF;

        rec = (CtrlRecHead*)((char*)rec + 0x38);
        tail = (CtrlRecTail*)((char*)tail + 0x38);
    }
}

// ----------------------------------------------------------------------------
// func_801AA2A8 - per-frame ctrl-state update driver.
// Walks the shared bdat table rows, refreshing each ctrl-state entry: reads
// named columns (rodata name blob), gates on play conditions (time window,
// resource counts, far/near flags, presentation bits), then either starts /
// updates the entry sound through func_801A96A0 or stops it.
// ----------------------------------------------------------------------------

// CfGameManager imports (retail mangled symbols).
class CtrlPlayerVt;
extern "C" void* getPlayer__Q22cf13CfGameManagerFi(int index);
extern "C" u32 func_80086DBC__Q22cf13CfGameManagerFv();
extern "C" u32 func_80086DA0__Q22cf13CfGameManagerFv();
extern "C" void func_80086DA4__Q22cf13CfGameManagerFv();
extern "C" u32 func_8008585C__Q22cf13CfGameManagerFv();
extern "C" u8 func_8007F9BC__Q22cf13CfGameManagerFv();
extern "C" u16 func_8016DF2C(void); // chapter/episode clock (CAIAction.hpp canonical form)
extern "C" u32 func_80082354__Q22cf13CfGameManagerFv(u32 resourceId);

// Player object view: field getter at vtable slot 43 (+0xAC).
class CtrlPlayerVt {
public:
    virtual void q00();
    virtual void q01();
    virtual void q02();
    virtual void q03();
    virtual void q04();
    virtual void q05();
    virtual void q06();
    virtual void q07();
    virtual void q08();
    virtual void q09();
    virtual void q0A();
    virtual void q0B();
    virtual void q0C();
    virtual void q0D();
    virtual void q0E();
    virtual void q0F();
    virtual void q10();
    virtual void q11();
    virtual void q12();
    virtual void q13();
    virtual void q14();
    virtual void q15();
    virtual void q16();
    virtual void q17();
    virtual void q18();
    virtual void q19();
    virtual void q1A();
    virtual void q1B();
    virtual void q1C();
    virtual void q1D();
    virtual void q1E();
    virtual void q1F();
    virtual void q20();
    virtual void q21();
    virtual void q22();
    virtual void q23();
    virtual void q24();
    virtual void q25();
    virtual void q26();
    virtual void q27();
    virtual void q28();
    virtual void q29();
    virtual s32 getField(); // +0xAC
};

// One 0x38-byte ctrl-state entry (fields touched by the update driver).
struct UpdEntry {
    u8 _00[0x2E];
    u16 m2E;      // +0x2E sound id / match key
    u8 _30[0x1C];
    u16 mHandle;  // +0x4C active sound handle (0xFFFF = stopped)
};

// Work-object header layout used by the driver (entries at +0x20).
class UpdWork {
public:
    u8 _00[0x19];
    u8 mFlag19;   // +0x19 mirror of the presentation/camera gate
    u8 mFlag1A;   // +0x1A update-disable flag
    u8 mFlag1B;   // +0x1B restrict-to-ids-1..6 flag
    s16 mCount;   // +0x1C iteration cursor
    u8 _1E[2];
    UpdEntry mEntries[128]; // +0x20
};

// Shared bdat column reader with per-entry context (defining TU: ocBdat.cpp;
// retail call passes an extra entry-base argument).
extern "C" u32 func_8003B434(void* table, const char* col, u32 colHandle,
                              s32 row, void* entryBase);
extern "C" int func_801BFABC(int a);

// Defined later in this TU (C linkage so call-site relocs bind to the
// retail-unmangled names).
extern "C" int func_801AA960(int self, int phase, int area, int mode);

// View of one 0x38-byte ctrl-state entry, exposing the match key at +0x2E.
struct ScanEntry {
    u8 _00[0x2E];
    u16 m2E; // +0x2E
};

void func_801AA2A8(UpdWork* self) {
    CtrlPlayerVt* player = (CtrlPlayerVt*)getPlayer__Q22cf13CfGameManagerFi(0);
    if (player == NULL) {
        return;
    }
    if (func_801BFABC(1) == 0) {
        return;
    }
    BdatTable* tbl = lbl_eu_806640B8;
    if (tbl == NULL) {
        return;
    }

    u32 phase = func_80086DBC__Q22cf13CfGameManagerFv();
    u16 area = (u16)func_80086DA0__Q22cf13CfGameManagerFv();
    func_80086DA4__Q22cf13CfGameManagerFv(); // result discarded (retail calls it)

    s32 row = func_8003B41C(tbl);          // first row of the range
    s32 endRow = row + func_8003B1EC(tbl); // one past the last row
    u16 secs = func_8016DF2C();            // chapter/episode clock
    u16 counter = (u16)func_800822F4__Q22cf13CfGameManagerFv();

    // Key scans: is any entry keyed 0x65 (else 0x66) present in the singleton?
    int keyFlag = 0;
    if (func_80082900__Q22cf13CfGameManagerFv() != 0) {
        CtrlStateWork* work = *(CtrlStateWork**)(&lbl_eu_80664330);
        BdatTable* table = lbl_eu_806640B8;
        if (work != NULL && table != NULL) {
            s32 r = func_8003B41C(table);
            s32 e = r + func_8003B1EC(table);
            ScanEntry* it = (ScanEntry*)((char*)work + 0x20 + r * 0x38);
            for (; r < e; r++) {
                if (it->m2E == 0x65) {
                    keyFlag = 1;
                    break;
                }
                it = (ScanEntry*)((char*)it + 0x38);
            }
        }
    }
    if (keyFlag == 0 && func_80082900__Q22cf13CfGameManagerFv() != 0) {
        CtrlStateWork* work = *(CtrlStateWork**)(&lbl_eu_80664330);
        BdatTable* table = lbl_eu_806640B8;
        if (work != NULL && table != NULL) {
            s32 r = func_8003B41C(table);
            s32 e = r + func_8003B1EC(table);
            ScanEntry* it = (ScanEntry*)((char*)work + 0x20 + r * 0x38);
            for (; r < e; r++) {
                if (it->m2E == 0x66) {
                    keyFlag = 1;
                    break;
                }
                it = (ScanEntry*)((char*)it + 0x38);
            }
        }
    }

    // Presentation-gate latch: bit 22 enables, unless bit 14 of the re-read
    // word clears it again; the scene query forces it on.
    int gate14 = 0;
    u32 flagWord = *(volatile u32*)&lbl_eu_80663E24;
    if ((flagWord & 0x400000) != 0) {
        gate14 = 1;
        u32 flagWord2 = *(volatile u32*)&lbl_eu_80663E24;
        if ((flagWord2 & 0x40000) == 0) {
            gate14 = 0;
        }
    }
    if (func_8008585C__Q22cf13CfGameManagerFv()) {
        gate14 = 1;
    }
    if ((u8)gate14 != self->mFlag19) {
        self->mFlag19 = gate14;
    }

    const char* cols = lbl_eu_80503FA0;

    // Resolve all column handles once up front (retail spills these to a
    // contiguous stack block, highest slot first).
    u32 colH[12];
    colH[11] = (u32)func_8003B4B0(tbl, cols + 0x5f);
    colH[10] = (u32)func_8003B4B0(tbl, cols + 0x49);
    colH[9] = (u32)func_8003B4B0(tbl, cols + 0x67);
    colH[8] = (u32)func_8003B4B0(tbl, cols + 0x71);
    colH[7] = (u32)func_8003B4B0(tbl, cols + 0x7b);
    colH[6] = (u32)func_8003B4B0(tbl, cols + 0x85);
    colH[5] = (u32)func_8003B4B0(tbl, cols + 0x91);
    colH[4] = (u32)func_8003B4B0(tbl, cols + 0x9e);
    colH[3] = (u32)func_8003B4B0(tbl, cols + 0xab);
    colH[2] = (u32)func_8003B4B0(tbl, cols + 0xb5);
    colH[1] = (u32)func_8003B4B0(tbl, cols + 0xbf);
    colH[0] = (u32)func_8003B4B0(tbl, cols + 0xc9);

    // Entries visited this frame: full range once the row span exceeds 15,
    // otherwise a third of it.
    s32 limit;
    if (endRow <= 0xf) {
        limit = endRow / 3;
    } else {
        limit = endRow;
    }

    s32 iter = 0;
    while (row < endRow) {
        if (self->mCount >= endRow) {
            self->mCount = 1;
        }
        s16 idx = self->mCount;
        u32 entryOff = idx * 0x38;
        UpdEntry* entry = (UpdEntry*)((char*)self + entryOff);
        u16 sid = entry->m2E;

        // Gate column + clock/phase checks decide whether the entry plays.
        ColVal t5f;
        t5f.v = func_8003B434(tbl, cols + 0x5f, colH[11], idx, entry);
        int gate = 0;
        if (t5f.b[0] == 0 || (u32)t5f.b[0] == (u32)secs) {
            gate = 1;
        }
        if (sid >= 1 && sid <= 3 && keyFlag == 0) {
            gate = 1;
        }

        u8 farGate = func_8007F9BC__Q22cf13CfGameManagerFv();
        ColVal t49;
        t49.v = func_8003B434(tbl, cols + 0x49, colH[10], idx, entry);
        int volOk = 1;
        if (t49.b[0] == 1) {
            if (farGate == 0) {
                volOk = 0;
            }
        } else if (t49.b[0] == 2 && farGate != 0) {
            volOk = 0;
        }
        // Priority override when the entry id sits in the 0x65/0x66 band.
        int prio = 1;
        if ((u32)(sid - 0x65) <= 1 && farGate != 0) {
            prio = 0;
        }

        if (gate != 0 && volOk != 0) {
            // Three mode columns feed the shared direction/mode selector.
            ColVal t67;
            ColVal t71;
            ColVal t7b;
            u16 modes[3];
            t67.v = func_8003B434(tbl, cols + 0x67, colH[9], idx, entry);
            modes[0] = t67.b[0];
            t71.v = func_8003B434(tbl, cols + 0x71, colH[8], idx, entry);
            modes[1] = t71.b[0];
            t7b.v = func_8003B434(tbl, cols + 0x7b, colH[7], idx, entry);
            modes[2] = t7b.b[0];
            // When a later slot is zero it is skipped and the previous
            // selector result stays live (retail reuses the register).
            int v = 0;
            for (int i = 0; i < 3; i++) {
                if (i == 0 || modes[i] != 0) {
                    if (modes[i] == 0) {
                        v = 1;
                    } else {
                        v = func_801AA960((int)self, phase, area, modes[i]);
                    }
                }
                gate |= (v != 0);
            }
        }

        int doPlay = 0;
        if (self->mFlag1A != 0) {
            doPlay = 0;
        } else if (gate != 0) {
            ColVal t85;
            ColVal t91;
            ColVal t9e;
            t85.v = func_8003B434(tbl, cols + 0x85, colH[6], idx, entry);
            u16 resId = t85.h[0];
            t91.v = func_8003B434(tbl, cols + 0x91, colH[5], idx, entry);
            u8 loCnt = t91.b[0];
            t9e.v = func_8003B434(tbl, cols + 0x9e, colH[4], idx, entry);
            u8 hiCnt = t9e.b[0];
            int rangeOk = 1;
            if (resId != 0) {
                u32 resCount = func_80082354__Q22cf13CfGameManagerFv(resId);
                if (!((u32)loCnt <= resCount && resCount <= (u32)hiCnt)) {
                    rangeOk = 0;
                }
            }
            if (rangeOk != 0) {
                // Time-window check against two alternative [lo,hi] pairs.
                ColVal tab;
                ColVal tbb;
                ColVal tbf;
                ColVal tc92;
                tab.v = func_8003B434(tbl, cols + 0xab, colH[3], idx, entry);
                u16 loA = tab.h[0];
                tbb.v = func_8003B434(tbl, cols + 0xb5, colH[2], idx, entry);
                u16 hiB = tbb.h[0];
                tbf.v = func_8003B434(tbl, cols + 0xbf, colH[1], idx, entry);
                u16 loC = tbf.h[0];
                tc92.v = func_8003B434(tbl, cols + 0xc9, colH[0], idx, entry);
                u16 hiD = tc92.h[0];
                int ok = 0;
                if ((u16)counter >= loA && (u16)counter <= hiB && hiB != 0) {
                    ok = 1;
                } else if ((u16)counter >= loC && (u16)counter <= hiD &&
                           hiD != 0) {
                    ok = 1;
                }
                doPlay = ok;
            }
        }
        if (self->mFlag19 != 0) {
            if (self->mFlag1B == 0 || sid < 1 || sid > 6) {
                doPlay = 0;
            }
        }

        if (doPlay != 0) {
            s32 fieldVal = player->getField();
            func_801A96A0((SndCtrlObj*)((char*)entry + 0x20), fieldVal, prio,
                          (int)((char*)entry));
        } else {
            // Not playing this frame: fade out and clear any active handle.
            if (entry->mHandle != 0xFFFF) {
                func_801BFED0(1, entry->mHandle, 0x3c);
            }
            entry->mHandle = 0xFFFF;
        }

        self->mCount = self->mCount + 1;
        iter++;
        if (iter >= limit) {
            break;
        }
        row++;
    }
}

extern "C" int func_801AA960(int self, int b, int c, int d) {
    // Else-if chain over the mode: retail presets r3=0, exits immediately on
    // a match, and funnels the last test through the shared epilogue.
    int result = 0;
    if (d == 1 && b == 1) result = 1;
    else if (d == 2 && b == 2) result = 1;
    else if (d == 3 && b == 3) result = 1;
    else if (d == 4 && b == 4) result = 1;
    else if (d == 5 && (unsigned int)c <= 2) result = 1;
    else if (d == 6 && (unsigned int)(c - 3) <= 2) result = 1;
    else if (d == 7 && (unsigned int)(c - 6) <= 2) result = 1;
    else if (d == 8 && (unsigned int)(c - 9) <= 2) result = 1;
    else if (d == 9 && (unsigned int)(c - 0xc) <= 2) result = 1;
    else if (d == 0xa && (unsigned int)(c - 0xf) <= 2) result = 1;
    else if (d == 0xb && (unsigned int)(c - 0x12) <= 2) result = 1;
    else if (d == 0xc) {
        // Nested form keeps retail's bnelr/bgtlr epilogue funnelling.
        if ((unsigned int)(c - 0x15) <= 2) result = 1;
    }
    return result;
}


// Bdat row queries; canonical extern "C" decls live in CfObjectEne.hpp /
// CItem.hpp, whose full include closure clashes with this TU.
extern "C" u32 func_8003B41C(void* bdat);   // bdat first row
extern "C" u32 func_8003B1EC(void* bdat);   // bdat row count
// Game-manager active gate; canonical member is cf::CfGameManager::func_80082900.
extern "C" u32 func_80082900__Q22cf13CfGameManagerFv();
// Sound-slot play entry (defined in CfSoundMan.cpp); C linkage so the call
// reloc binds to the retail-unmangled name.
extern "C" void func_801BFE58(s32 idx, u32 a, u32 b, float volume);

// Bdat table handle owning the ctrl-state row range (import from split1).
struct BdatTable;
extern BdatTable* lbl_eu_806640B8;

// ----------------------------------------------------------------------------
// func_801AAAA0 - scan the shared ctrl-state singleton's entry table starting
// at the bdat table's first-row index for the table's row count, returning
// whether any entry's key (+0x2E) equals id.
// ----------------------------------------------------------------------------
bool func_801AAAA0(u32 id) {
    if (func_80082900__Q22cf13CfGameManagerFv() == 0) {
        return false;
    }
    extern unsigned char lbl_eu_80664330;
    BdatTable* table = lbl_eu_806640B8;
    if (*(void**)(&lbl_eu_80664330) == nullptr || table == nullptr) {
        return false;
    }
    s32 row = func_8003B41C(table);   // first row of the range
    s32 count = func_8003B1EC(table); // number of rows
    // Re-read the singleton: scan its entry table from 'row' for 'count' rows.
    CtrlStateWork* work2 = *(CtrlStateWork**)(&lbl_eu_80664330);
    s32 endRow = row + count;
    ScanEntry* it = (ScanEntry*)&work2->mEntries[row];
    for (; row < endRow; row++) {
        if (id == it->m2E) {
            return true;
        }
        it = (ScanEntry*)((char*)it + 0x38);
    }
    return false;
}

// ----------------------------------------------------------------------------
// func_801AAB64 - apply a sound-volume update over the shared ctrl-state
// singleton's entry range (bdat table rows). For every entry whose key (+0x30)
// equals id (or when id == 0, all entries) and whose sound index (+0x2C) is
// valid, play index scaled by the entry's volume (+0x24) times scale. When
// store is set, persist scale and kind into the entry.
// The singleton global is re-read before each access, matching retail.
// ----------------------------------------------------------------------------
void func_801AAB64(u32 id, u32 kind, int store, float scale) {
    extern unsigned char lbl_eu_80664330;
    if (func_80082900__Q22cf13CfGameManagerFv() == 0) {
        return;
    }
    BdatTable* table = lbl_eu_806640B8;
    if (*(void**)(&lbl_eu_80664330) == nullptr || table == nullptr) {
        return;
    }
    s32 row = func_8003B41C(table);   // first row of the range
    s32 endRow = row + func_8003B1EC(table); // number of rows
    for (; row < endRow; row++) {
        // The singleton global is re-read for every statement (retail never
        // caches it across statements).
        if (id != 0 &&
            (*(CtrlStateWork**)(&lbl_eu_80664330))->mEntries[row].m30 != id) {
            continue;
        }
        if ((*(CtrlStateWork**)(&lbl_eu_80664330))->mEntries[row].m2C != -1) {
            func_801BFE58(
                1, (*(CtrlStateWork**)(&lbl_eu_80664330))->mEntries[row].m2C,
                kind,
                scale * (*(CtrlStateWork**)(&lbl_eu_80664330))->mEntries[row].m1C);
        }
        if (store != 0) {
            (*(CtrlStateWork**)(&lbl_eu_80664330))->mEntries[row].m28 = scale;
            (*(CtrlStateWork**)(&lbl_eu_80664330))->mEntries[row].m32 = kind;
        }
    }
}

void func_801AAC70(void* self){ float dummy; func_801AAB64(0, (u32)self, 1, dummy); }

extern "C" void func_801AAC78(u8 v) {
    extern unsigned char lbl_eu_80664330;
    void* p = *(void**)(&lbl_eu_80664330);
    if (p == 0) return;
    *(u8*)((u8*)p + 0x1A) = v;
    if (v == 0) return;
    func_801AAB64(0, 0, 0, lbl_eu_80667D64);
}

extern "C" void func_801AACA8(unsigned char v) {
    extern unsigned char lbl_eu_80664330;
    void* p = *(void**)(&lbl_eu_80664330);
    if (p) *(unsigned char*)((u8*)p + 0x1b) = v;
}

void func_801AACBC(unsigned int* r3, unsigned int* r4) {
    extern unsigned char lbl_eu_80664330;
    unsigned int *p = (unsigned int *)&lbl_eu_80664330;
    if (!*p) return;
    unsigned int *dst = (unsigned int *)*p;
    unsigned int *src1 = r3;
    unsigned int *src2 = r4;
    dst[0] = src1[0];
    dst[1] = src1[1];
    dst[2] = src1[2];
    dst[3] = src2[0];
    dst[4] = src2[1];
    dst[5] = src2[2];
    ((unsigned char *)dst)[0x18] = 1;
}

extern "C" void func_801AAD08() {
    extern unsigned char lbl_eu_80664330;
    void* p = *(void**)(&lbl_eu_80664330);
    if (p) *(unsigned char*)((u8*)p + 0x18) = 0;
}
