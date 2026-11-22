// Auto-scaffolded catalog TU for kyoshin/cf/CActParamAnimGame
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/cf/CActParamAnimGame.hpp"
#include "kyoshin/action/CActParamAnim.hpp"
#include "monolib/scn/code_804BD8E8.hpp"
#include "kyoshin/action/CActParamAnimForeign.hpp"
#include "monolib/coli/code_804B2FF0.hpp"
#include <nw4r/math/math_arithmetic.h>
#include <nw4r/math/math_triangular.h>
#include <revolution/mtx/vec.h>
#include "monolib/math/CVec3.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (gmScene; gmGameFlags)
#include "monolib/math/FloatUtils.hpp"  // H3 label-owner decl (ml::epsilon)
#include "kyoshin/cf/object/CfObject.hpp"  // real owner of the +0x4C/+0xAC/+0xC4 link slots
#include "kyoshin/cf/object/CBattleState.hpp"
namespace nw4r { namespace math { f32 FrSqrt(f32 x); } }
#include "monolib/scn/code_804BC9EC.hpp"
// CBattleState +0x10 (__vt__Q22cf12CBattleState / JP __vt__Q22cf12CBattleState word 3):
// retail symbol remains CBattleState_UnkVirtualFunc3. Owner TU is out of scope;
// no already-real same-slot alias exists (unlike getOwner for +0x08). Call sites
// below keep the real virtual; this is a placeholder name, not a pad/If.

// Retail constructor(C) at 0x8005AA64 (unmangled symbol): base-constructs
// the ::CActParamAnim subobject, stores the retail vtable manually
// (novtable class), zeroes the flag fields, invokes the +0xE0 virtual
// (CActParamAnim::initAnimState, overridden in the retail table by
// resetAnimGame), then parks the link argument at +0x4E8 and returns this.
cf::CActParamAnimGame* __ct__8005A3FC(cf::CActParamAnimGame* self, cf::CActParamAnimGameLink* arg) {
    __ct__13CActParamAnimFv(self);
    *(void**)self = (void*)lbl_eu_80526458;
    ((cf::CActParamAnimGame*)self)->field4EC = 0;
    ((cf::CActParamAnimGame*)self)->field4F0 = 0;
    ((cf::CActParamAnimGame*)self)->field530 = 0;
    self->initAnimState();
    ((cf::CActParamAnimGame*)self)->field4E8 = arg;
    return self;
}

cf::CActParamAnimGame::CActParamAnimGame() {
    // Base ctor (__ct__13CActParamAnimFv) runs first; the retail vtable is
    // stored manually because the class is __declspec(novtable).
    *(void**)this = (void*)lbl_eu_80526458;
    ((CActParamAnimGame*)this)->field4EC = 0;
    ((CActParamAnimGame*)this)->field4F0 = 0;
    ((CActParamAnimGame*)this)->field530 = 0;
    this->initAnimState();
}

cf::CActParamAnimGame::~CActParamAnimGame() {}

// Retail C-ABI sibling (unmangled name; defined in kyoshin/action/CActParamAnim.cpp)
extern "C" void startAnimDual(void*);

// Forward decls for the state-gate probes defined below.
// Retail symbols are unmangled C-ABI globals; extern "C" keeps the call-site
// relocs on the retail names (same precedent as trackGroundAt below).
extern "C" bool probeStandGnd(cf::CActParamAnimGame* self);
extern "C" bool snapActorDown(cf::CActParamAnimGame* self);
bool probeGroundAt(cf::CActParamAnimGame* self);
bool refineHitNorm(cf::CActParamAnimGame* self);
bool probeWallPair(cf::CActParamAnimGame* self);

void cf::CActParamAnimGame::resetAnimGame() {
    CActParamAnim::initAnimState();
    // Retail-offset stores through the absolute view (the ::CActParamAnim base
    // compiles 0x36C larger, so class-member displacements shift; per-statement
    // view casts fold each retail offset into its store exactly like the raw
    // form did).
    CActParamAnimGame* v = (CActParamAnimGame*)this;
    v->field4E8 = 0;
    v->field4F4 = 0;
    v->field4F8 = lbl_eu_80666040;
    v->field500 = lbl_eu_80666044;
    v->field508 = lbl_eu_80666044;
    v->field50C = lbl_eu_80666048;
    v->field52C = 0;
    v->field504 = lbl_eu_80666048;
    v->field532 = 0;
    v->field534 = 0;
    v->field536 = 2;
    v->field538 = 0;
}

// Stubs for retail C-linkage functions that func_8005D2C4 tail-calls;
// extern "C" so the call-site reloc keeps the retail unmangled name (same precedent as runBattleMove in this file).
// trackGroundAt is defined below (after the collision-query typedefs).
extern "C" void trackGroundAt(cf::CActParamAnimGame* self);

// Forward decls for applyFallMove below (C-linkage retail names; most are
// also declared in the shared block further down).
extern "C" {
void addAnimHeight(void*, f32);
int shiftAnimPos(void*);
void mulAnimHeight(void*, f32);
void clampAnimMove(void*);
void steerAnimHead(void*);
void setActiveBit(void*, int);
void stopAnimMove(void*);
f32 getSecPerFrame__9CDeviceVIFv();
}

// Fall/air update: runs while airborne. Refreshes the fall scale pair,
// applies the fall-timer penalty, integrates velocity/move over the frame
// time into a drift delta, then either dispatches the paused vt+0xE8 probe
// or lands via setActiveBit/clrAnimHeight.
// Retail symbol is an unmangled C-ABI global; extern "C" keeps the call-site
// reloc on the retail name (approved fix, PLAN.md §17.6).
extern "C" void applyFallMove(cf::CActParamAnimGame* selfV) {
    cf::CActParamAnimGame* self = (cf::CActParamAnimGame*)selfV;
    shiftAnimPos(self);
    self->field394 = 0.75f;
    self->field398 = 0.09375f;
    if ((self->field0C & 0x100) == 0) {
        ml::CVec3 pos;
        pos.x = self->field3A8;
        pos.y = self->field3AC;
        pos.z = self->field3B0;
        // Mirror (inverted) the air/fall bit from the snapshot flag word.
        if (!(self->field4F0 & 2)) {
            self->field4EC |= 0x400000;
        } else {
            self->field4EC &= ~0x400000;
        }
        // Fall-timer overshoot penalty: past the cap the penalty saturates.
        f32 drop = self->field4FC - (self->field508 + self->field3AC);
        if (drop > lbl_eu_80666040) {
            f32 scale = 15.0f;
            if (!(drop >= 0.4f)) {
                scale = 2.5f * drop * 15.0f;
            }
            addAnimHeight(self, scale);
        }
        addAnimHeight(self, -self->field380);
        mulAnimHeight(self, 0.9f);
        clampAnimMove(self);
        // Advance velocity and move vec over the frame time; the accumulated
        // drift is (vel*dt + face dir) + move*dt. The intermediate copies are
        // kept so each stage lands in its own stack temporary (retail layout).
        f32 speed = self->field390;
        f32 dt = speed * getSecPerFrame__9CDeviceVIFv();
        ml::CVec3 velStep = self->vec3C0() * dt;
        ml::CVec3 moveStep = self->vec3F0() * dt;
        ml::CVec3 faceSum = velStep + self->vec3CC();
        ml::CVec3 sumA = faceSum;
        ml::CVec3 delta = sumA + moveStep;
        // Fold in the +0x3A0 chain's airborne accumulator.
        if (self->object3A0 != 0) {
            cf::CActParamAnimGameObj3A0* obj = (cf::CActParamAnimGameObj3A0*)self->object3A0;
            delta += obj->pos7CC;
        }
        // Ground clamp: pull y toward the ground height once past the bound.
        {
            f32 t = self->field4FC - (delta.y + pos.y) - 5.0f;
            if (t > lbl_eu_80666040) {
                pos.y += t;
            }
        }
        {
            f32 t = self->field4FC - (delta.y + pos.y) - 2.0f;
            if (t > lbl_eu_80666040) {
                self->field394 = lbl_eu_80666040;
                self->field4EC |= 0x40000000;
            }
        }
        // Normalize the horizontal speed into +0x448.
        f32 sp2 = delta.z * delta.z + delta.x * delta.x;
        if (sp2 < lbl_eu_80666040) {
            Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273, lbl_eu_80526300);
        }
        f32 len = lbl_eu_80666040;
        if (sp2 > lbl_eu_80666040) {
            len = sp2 * nw4r::math::FrSqrt(sp2);
        }
        self->field448 = len;
        getScnHandle();
        if (ScnData_FwdBC9A0() != 0) {
            // Paused: probe the +0xE8 hook (initAnimBlendVectors) with the
            // position/delta pair; land only when the vertical speed is
            // positive or the hook reports failure.
            int ret = selfV->initAnimBlendVectors(&pos, &delta);
            f32 vy = self->field3C4;
            if (vy > lbl_eu_80666040 || ret == 0) {
                setActiveBit(self, 0);
            }
        } else {
            setActiveBit(self, 1);
            clrAnimHeight((CActParamAnim*)self);
            pos.x += delta.x;
            pos.z += delta.z;
        }
        setAnimSubPos((CActParamAnim*)self, (const CActParamAnimData3*)&pos);
    }
    if ((self->field0C & 0x40) == 0) {
        steerAnimHead(self);
    }
    stopAnimMove(self);
    self->field0C &= ~0x80;
}

typedef unsigned char u8;
typedef signed char s8;

extern "C" {
f32 SinFIdx__Q24nw4r4mathFf(f32);
f32 CosFIdx__Q24nw4r4mathFf(f32);
f32 Atan2FIdx__Q24nw4r4mathFff(f32, f32);
void Warning__Q24nw4r2dbFPCciPCce(const char*, int, const char*, ...);
void PSVECCrossProduct(const Vec*, const Vec*, Vec*);
void PSVECNormalize(const Vec*, Vec*);
f32 PSVECMag(const Vec*);
bool isTvFormatPal__9CDeviceVIFv();
f32 getSecPerFrame__9CDeviceVIFv();
extern "C" f32 simGetTvFadeConst();
void setAnimPosVec(void*, const ml::CVec3*);
void addAnimHeight(void*, f32);
int shiftAnimPos(void*);
void mulAnimHeight(void*, f32);
void clampAnimMove(void*);
void steerAnimHead(void*);
void setActiveBit(void*, int);
int isSpeedBelow(void*);
void stopAnimMove(void*);
f32 getAnimWalkKmh(void*);
f32 getAnimMinSpeed();
u32 ScnRes_SegQueryForward_E348(void*, void*, u32, u32, u32);
u32 ScnRes_EntryFlagThunk_E5A4(u32, u32);
u32 ScnRes_GetEntryCount_E4AC(void);
int ScnRes_Notify5Ptr_E470(void*, void*, void*, void*, void*, f32, f32);
int ScnRes_TableByteThunk_E5A8(void*, u32, u32, u32);
void* ScnRes_GetEntryHead2_E520(int);
void* ScnRes_GetEntryPtr_E50C(u32);
u32 getTargetFramerate__9CDeviceVIFv(void);
extern "C" void scnImN4DynStop(void*, void*);
int ColiResolveNodeMove(void*, ml::CVec3*, ml::CVec3*, ml::CVec3*);
int Coli_WalkReport(void*, ml::CVec3*, ml::CVec3*, ml::CVec3*, ml::CVec3*);
void setAnimHeight(void*, f32);
void ScnRes_XCrossCbThunk_E628(int);
u32 getQueuedFileEventCount__Q22cf13CfGameManagerFv(void);
extern "C" void* getGimmickListHead__Q22cf13CfGameManagerFv(void);
int getEventValue40__Q22cf13CfGameManagerFv(int);
void setEventManagerValue__Q22cf13CfGameManagerFv(int, int);
void queueEventId__Q22cf13CfGameManagerFv(int);
void ColiSetAxisBlockInverse(void*, const ml::CVec3*);
void ColiResolveMoveDefault(void*, void*, void*, void*, void*);
s8 sAnimGameYawUpperInit; // name-guess: one-shot flag for the +0xC owner-yaw upper bound (runBattleMove)
s8 sAnimGameYawLowerInit; // name-guess: one-shot flag for the +0xC owner-yaw lower bound (runBattleMove)
s8 sAnimGameQuarterPiInit; // name-guess: one-shot flag for the pi/4 turn-scale bound (runBattleMove)
f32 sAnimGameYawUpperRad; // name-guess: cached +60 deg * 0.017453292 used as owner-yaw upper bound (runBattleMove)
f32 sAnimGameYawLowerRad; // name-guess: cached -60 deg * 0.017453292 used as owner-yaw lower bound (runBattleMove)
f32 sAnimGameQuarterPiRad; // name-guess: cached ml::pi * 0.25 used as turn-scale bound (runBattleMove)
const char lbl_eu_80526324[];
const char lbl_eu_80526300[];
ml::CVec3 zero__Q22ml5CVec3;
}

// ScnRes_VertRayForward_E398's retail ABI passes two FP args (f1, f2) after the four GPR
// args; the header now owns the extern "C" decl (6 GPR+FP args), and the
// already-matched runBattleMove 4-arg call site casts to a 4-arg fn type.
typedef int (*BE398Fn4)(void*, u32, u32, u32);
// ScnRes_ResolveGroundHeight (11-arg; header-owned extern "C") cast helpers.
typedef u32 (*BD94CFn)(void*, void*, u32, u32, u32, u32, f32, f32, f32, f32, f32);
typedef u32 (*BD94CFn6)(void*, void*, u32, u32, u32, u32);

// Collision-query result object returned by ScnRes_GetEntryHead2_E520 / ScnRes_GetEntryPtr_E50C:
// position triple with the height at +4.
struct CActParamAnimGameBeObj {
    f32 x, y, z;
};

// Main per-frame actor update: resolves the ground/obstacle probes, runs the
// obstacle-scan loop (nearest/tallest hits), refreshes the facing vector, and
// feeds the move vec into the +0x3A0 chain. C-linkage retail symbol; called
// by func_8005D2C4. Goes through the absolute-offset view: the embedded base
// compiles 0x36C larger than retail, so direct member access shifts every
// displacement.
extern "C" void trackGroundAt(cf::CActParamAnimGame* selfV) {
    cf::CActParamAnimGame* self = (cf::CActParamAnimGame*)selfV;
    self->field47C = -10000.0f;
    self->field0C &= ~0x800;
    self->field4AC = 0;
    self->vec520() = zero__Q22ml5CVec3;

    if (self->field4EC & 0x40) {
        self->field4EC &= ~0x40;
        goto shared;
    }

    if (self->mData.mA260 & 0x20) {
        // Ground contact already tracked: probe straight down, then commit.
        ml::CVec3 vecA(lbl_eu_80666040, 2.0f, lbl_eu_80666040);
        ml::CVec3 vec220 = self->vec3A8() + vecA;
        if (ScnRes_Notify5Ptr_E470(&vec220, &self->vec520(), 0, 0, 0, -8.0f, lbl_eu_80666040)) {
            CActParamAnimGameBeObj* obj = (CActParamAnimGameBeObj*)ScnRes_GetEntryHead2_E520(0);
            if (obj->y < 0.0f) {
                self->field4EC |= 0x20000000;
            } else {
                ml::CVec3 vec88 = self->vec3A8() + self->vec520();
                self->field488 += self->field524;
                setAnimSubPos((CActParamAnim*)self, (const CActParamAnimData3*)&vec88);
                if (self->object3A0) scnImN4DynStop(self->object3A0, &self->vec520());
            }
        }
        if ((self->field0C & 0x80) == 0) {
            if (self->field488 - self->field3AC > 5000.0f) {
                self->field4EC |= 0x80;
            }
        }
        goto shared;
    }

    {
        // Probe down; the f2-height path re-aims the actor when the probe
        // point lies beyond the facing half-plane.
        ml::CVec3 vecA(lbl_eu_80666040, 2.0f, lbl_eu_80666040);
        ml::CVec3 vec214 = self->vec3A8() + vecA;
        f32 localF = 0.0f;
        if (self->field3CC == 0.0f && self->field3D4 == 0.0f) {
            if (ScnRes_Notify5Ptr_E470(&vec214, &self->vec520(), &localF, 0, 0, -8.0f, lbl_eu_80666040)) {
                CActParamAnimGameBeObj* obj = (CActParamAnimGameBeObj*)ScnRes_GetEntryHead2_E520(0);
                if (obj->y < 0.0f) {
                    self->field4EC |= 0x20000000;
                } else {
                    ml::CVec3 vec = self->vec3A8() + self->vec520();
                    self->field488 += self->field524;
                    setAnimSubPos((CActParamAnim*)self, (const CActParamAnimData3*)&vec);
                    if (localF != 0.0f) setTurnScale((CActParamAnim*)self, localF + self->field444);
                    if (self->object3A0) scnImN4DynStop(self->object3A0, &self->vec520());
                }
            }
        } else {
            if (ScnRes_Notify5Ptr_E470(&vec214, &self->vec520(), 0, 0, 0, -8.0f, lbl_eu_80666040)) {
                CActParamAnimGameBeObj* obj = (CActParamAnimGameBeObj*)ScnRes_GetEntryHead2_E520(0);
                if (obj->y < 0.0f) {
                    self->field4EC |= 0x20000000;
                } else {
                    ml::CVec3 vec = self->vec3A8() + self->vec520();
                    self->field488 += self->field524;
                    setAnimSubPos((CActParamAnim*)self, (const CActParamAnimData3*)&vec);
                    if (self->object3A0) scnImN4DynStop(self->object3A0, &self->vec520());
                }
            }
        }
        self->field4EC &= ~0x40000;
        goto shared;
    }

shared:
    {
        ml::CVec3 vecA(lbl_eu_80666040, -30.0f, lbl_eu_80666040);
        ml::CVec3 vec58 = self->vec3A8() + vecA;
        *(u32*)&self->field3E4 = *(u32*)&vec58.x;
        *(u32*)&self->field3E8 = *(u32*)&vec58.y;
        *(u32*)&self->field3EC = *(u32*)&vec58.z;
        f32 f31v = 0.8f;
        ml::CVec3 vecB(lbl_eu_80666040, 13.0f, lbl_eu_80666040);
        ml::CVec3 vec4C = self->vec3A8() + vecB;
        ml::CVec3 vec124 = vec4C;
        int needDefault = 1;
        if (ScnRes_VertRayForward_E398((float*)&vec124, 0xa04, 0, 1, -30.0f, lbl_eu_80666040)) {
            int count = ScnRes_GetEntryCount_E4AC();
            f32 f30 = 0.3f + self->field3AC;
            int lowerIdx = -1;
            f32 f29 = 0.01f + self->field3AC;
            int upperIdx = -1;
            f32 f28 = f30 - 1000.0f;
            f32 f24 = 2.0f;
            int selIdx = -1;
            f32 f27 = 10000.0f + f30;
            int refIdx = -1;
            f32 f26 = f30 - f24;
            int secIdx = -1;
            f32 f23 = f28;
            int faceFlag;
            for (faceFlag = 0; faceFlag < count; faceFlag++) {
                ml::CVec3 vec208;
                ScnRes_CopyEntryHead(&vec208, faceFlag);
                if (ScnRes_EntryFlagThunk_E5A4(0x40000, faceFlag)) {
                    f32 y = vec208.y;
                    if (y >= f29) {
                        if (f27 > y) { f27 = y; upperIdx = faceFlag; }
                    } else {
                        if (f24 + self->field3AC > y) {
                            if (f26 < y) { f26 = y; refIdx = faceFlag; }
                            if (f23 < y) { f23 = y; secIdx = faceFlag; }
                        }
                    }
                } else {
                    CActParamAnimGameBeObj* obj = (CActParamAnimGameBeObj*)ScnRes_GetEntryHead2_E520(faceFlag);
                    f32 y = vec208.y;
                    if (obj->y > 0.0f) {
                        if (f30 > y) {
                            if (f28 < y) { f28 = y; lowerIdx = faceFlag; }
                            if (f23 < y) { f23 = y; secIdx = -1; }
                        }
                    } else {
                        if (f29 > y) {
                            if (f28 < y) { f28 = y; lowerIdx = faceFlag; }
                            if (f23 < y) { f23 = y; secIdx = -1; }
                        }
                    }
                }
            }
            if (lowerIdx != -1) {
                selIdx = lowerIdx;
                if (self->field3C4 <= 0.0f) {
                    if (ml::math::abs(self->field3AC - f28) <= 0.01f) {
                        if ((self->field4EC & 4) == 0) {
                            ml::CVec3 vec1fc;
                            if (ScnRes_FetchScaleRow_E53C((f32*)&vec1fc, (s32)lowerIdx)) {
                                f32 fr = (f32)getTargetFramerate__9CDeviceVIFv();
                                vec1fc *= fr;
                                *(u32*)&self->field3F0 = *(u32*)&vec1fc.x;
                                *(u32*)&self->field3F4 = *(u32*)&vec1fc.y;
                                *(u32*)&self->field3F8 = *(u32*)&vec1fc.z;
                                needDefault = 0;
                            }
                        }
                    }
                }
                self->field47C = f28;
            }
            if (upperIdx != -1) {
                faceFlag = 0;
                self->field4FC = f27;
                if (lowerIdx != -1) self->field4F8 = f27 - f28;
                selIdx = upperIdx;
                if ((self->field4EC & 4) == 0) {
                    ml::CVec3 vec1f0;
                    if (ScnRes_FetchScaleRow_E53C((f32*)&vec1f0, (s32)upperIdx)) {
                        f32 fr = (f32)getTargetFramerate__9CDeviceVIFv();
                        vec1f0 *= fr;
                        *(u32*)&self->field3F0 = *(u32*)&vec1f0.x;
                        *(u32*)&self->field3F4 = *(u32*)&vec1f0.y;
                        *(u32*)&self->field3F8 = *(u32*)&vec1f0.z;
                        needDefault = 0;
                    }
                }
                if (self->field4F8 > self->field508) {
                    f23 = lbl_eu_80666044 + self->field3AC;
                    int bandIdx = -1;
                    f27 = -0.95f;
                    f29 = lbl_eu_80666040;
                    f24 = 1.3f + f23;
                    int cand;
                    for (cand = 0; cand < count; cand++) {
                        if (((CActParamAnimGameBeObj*)ScnRes_GetEntryHead2_E520(cand))->y < 0.0f
                            && ((CActParamAnimGameBeObj*)ScnRes_GetEntryHead2_E520(cand))->y >= f27) {
                            f32 g = ((CActParamAnimGameBeObj*)ScnRes_GetEntryPtr_E50C(cand))->y;
                            if (f23 < g && f24 > g) { f24 = g; bandIdx = cand; }
                        }
                    }
                    if (bandIdx != -1) {
                        ml::CVec3 vec1e4;
                        ScnRes_CopyEntryMid(&vec1e4, bandIdx);
                        if (vec1e4.x * self->field3CC + vec1e4.z * self->field3D4 <= 0.0f) {
                            ml::CVec3 vec1d8;
                            vec1d8.x = vec1e4.x;
                            vec1d8.y = 0.0f;
                            vec1d8.z = vec1e4.z;
                            f32 len2 = vec1d8.y * vec1d8.y + vec1d8.x * vec1d8.x + vec1d8.z * vec1d8.z;
                            if (len2 == 0.0f) {
                                vec1d8 = zero__Q22ml5CVec3;
                            } else {
                                PSVECNormalize((const Vec*)&vec1d8, (Vec*)&vec1d8);
                            }
                            ml::CVec3 up(0.0f, 1.0f, 0.0f);
                            ml::CVec3 cross;
                            PSVECCrossProduct((const Vec*)&vec1d8, (const Vec*)&up, (Vec*)&cross);
                            f32 clen2 = cross.y * cross.y + cross.x * cross.x + cross.z * cross.z;
                            if (clen2 == 0.0f) {
                                cross = zero__Q22ml5CVec3;
                            } else {
                                PSVECNormalize((const Vec*)&cross, (Vec*)&cross);
                            }
                            f32 dot = cross.x * self->field3CC + cross.z * self->field3D4;
                            f32 fz = 0.0f;
                            f32 fx = cross.x * dot;
                            f32 fz2 = cross.z * dot;
                            *(u32*)&self->field3D0 = *(u32*)&fz;
                            *(u32*)&self->field3CC = *(u32*)&fx;
                            *(u32*)&self->field3D4 = *(u32*)&fz2;
                            *(u32*)&self->field3F0 = *(u32*)&vec1d8.x;
                            *(u32*)&self->field3F4 = *(u32*)&vec1d8.y;
                            *(u32*)&self->field3F8 = *(u32*)&vec1d8.z;
                            faceFlag = 1;
                            needDefault = 0;
                        }
                    }
                }
            }
            if (selIdx != -1) {
                CActParamAnimGameBeObj* o = (CActParamAnimGameBeObj*)ScnRes_GetEntryPtr_E50C(selIdx);
                *(u32*)&self->field3E4 = *(u32*)&o->x;
                *(u32*)&self->field3E8 = *(u32*)&o->y;
                *(u32*)&self->field3EC = *(u32*)&o->z;
                CActParamAnimGameBeObj* o2 = (CActParamAnimGameBeObj*)ScnRes_GetEntryHead2_E520(selIdx);
                *(u32*)&self->field3D8 = *(u32*)&o2->x;
                *(u32*)&self->field3DC = *(u32*)&o2->y;
                *(u32*)&self->field3E0 = *(u32*)&o2->z;
                void* sub = ((cf::CActParamAnimGameObj3A0*)self->object3A0)->sub7EC;
                if (sub != 0) {
                    ml::CVec3 vec;
                    vec.x = self->field3E4;
                    vec.y = self->field3E8;
                    vec.z = self->field3EC;
                    ((CActParam7ECTarget*)sub)->func14(&vec);
                }
                if ((self->field4EC & 0x20000000) && faceFlag == 0) {
                    f32 dot = self->field3D8 * self->field3CC + self->field3EC * self->field3D4;
                    if (dot <= 0.0f) {
                        ml::CVec3 v1c0(self->field3D8, 0.0f, self->field3EC);
                        ml::CVec3 up2(0.0f, 1.0f, 0.0f);
                        ml::CVec3 cross2;
                        PSVECCrossProduct((const Vec*)&v1c0, (const Vec*)&up2, (Vec*)&cross2);
                        f32 clen2b = cross2.y * cross2.y + cross2.x * cross2.x + cross2.z * cross2.z;
                        if (clen2b == 0.0f) {
                            cross2 = zero__Q22ml5CVec3;
                        } else {
                            PSVECNormalize((const Vec*)&cross2, (Vec*)&cross2);
                        }
                        f32 dot2 = cross2.x * self->field3CC + cross2.z * self->field3D4;
                        f32 fz = 0.0f;
                        f32 fx = cross2.x * dot2;
                        f32 fz2 = cross2.z * dot2;
                        *(u32*)&self->field3D0 = *(u32*)&fz;
                        *(u32*)&self->field3CC = *(u32*)&fx;
                        *(u32*)&self->field3D4 = *(u32*)&fz2;
                    }
                }
                u32 local8 = 0;
                if (lowerIdx != -1) {
                    if (ScnRes_EntryFlagThunk_E5A4(0x10000, lowerIdx)) {
                        ScnRes_TableByteThunk_E5A8(&local8, 0x10000, lowerIdx, 0);
                    }
                } else if (upperIdx != -1) {
                    if (ScnRes_EntryFlagThunk_E5A4(0x10000, upperIdx)) {
                        ScnRes_TableByteThunk_E5A8(&local8, 0x10000, upperIdx, 0);
                    }
                }
                self->field4AC = local8;
                if (ScnRes_IsEntryValued_E604((int)selIdx)) {
                    self->field4EC |= 0x1000000;
                }
                if (ScnRes_EntryFlagThunk_E5A4(0x4000, selIdx)) {
                    if (ml::math::abs(self->field3AC - f28) <= 2.0f) {
                        self->field4EC |= 0x80;
                    }
                }
                if (refIdx != -1 && upperIdx == -1) {
                    if (f26 < f28) refIdx = selIdx;
                } else {
                    refIdx = selIdx;
                }
                if (ScnRes_EntryFlagThunk_E5A4(0x20000, refIdx)) {
                    f32 objY = ((CActParamAnimGameBeObj*)ScnRes_GetEntryPtr_E50C(refIdx))->y;
                    if (self->field3AC - objY <= lbl_eu_80666044) {
                        self->field4EC |= 0x1000000;
                        ScnRes_TableByteThunk_E5A8(&local8, 0x20000, refIdx, 1);
                        self->field4B0 = local8;
                    }
                }
                if (ScnRes_EntryFlagThunk_E5A4(0x8000, selIdx)) {
                    self->field4EC |= 0x800000;
                } else {
                    self->field4EC &= ~0x800000;
                }
                if (ScnRes_EntryFlagThunk_E5A4(0x400, selIdx)) {
                    if ((self->field0C & 2) != 0 && (self->mData.mA260 & 0x20) == 0) {
                        ml::CVec3 vec1a8;
                        ScnRes_CopyEntryMid(&vec1a8, selIdx);
                        ml::CVec3 vec19c = self->vec3F0();
                        f32 f5 = self->field390 * getSecPerFrame__9CDeviceVIFv();
                        ml::CVec3 tmp = self->vec3C0() * f5;
                        ml::CVec3 vec190 = tmp;
                        vec190 += self->vec3CC();
                        if (vec190.x == 0.0f && vec190.z == 0.0f) {
                            f31v = 0.98f;
                            if (vec1a8.y <= f31v) {
                                vec19c.x += vec1a8.x * 1.5f;
                                vec19c.z += vec1a8.z * 1.5f;
                            }
                        } else {
                            if (f5 != 0.0f) f5 = 1.0f / f5;
                            if (self->field0C & 0x200000) {
                                vec19c += vec190 * (0.04f * f5);
                            } else {
                                vec19c += vec190 * (0.5f * f5);
                            }
                            f32 len2 = vec19c.x * vec19c.x + vec19c.z * vec19c.z;
                            if (len2 >= 4225.0f) {
                                if (!(len2 >= 0.0f)) {
                                    Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273, lbl_eu_80526300);
                                }
                                f32 len = len2 <= 0.0f ? 0.0f : len2 * nw4r::math::FrSqrt(len2);
                                f32 inv = 1.0f / len;
                                vec19c.x *= 48.0f * inv;
                                vec19c.z *= 48.0f * inv;
                            }
                            if (vec1a8.y <= 0.98f) {
                                f32 len2b = vec190.x * vec190.x + vec190.z * vec190.z;
                                if (!(len2b >= 0.0f)) {
                                    Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273, lbl_eu_80526300);
                                }
                                f32 lenb = len2b <= 0.0f ? 0.0f : len2b * nw4r::math::FrSqrt(len2b);
                                f32 invb = 1.0f / lenb;
                                f32 nvx = vec190.x * invb;
                                f32 nvz = vec190.z * invb;
                                f32 len2c = vec1a8.x * vec1a8.x + vec1a8.z * vec1a8.z;
                                if (!(len2c >= 0.0f)) {
                                    Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273, lbl_eu_80526300);
                                }
                                f32 lenc = len2c <= 0.0f ? 0.0f : len2c * nw4r::math::FrSqrt(len2c);
                                f32 invc = 1.0f / lenc;
                                f32 dot = nvz * (vec1a8.z * invc) + nvx * (vec1a8.x * invc);
                                if (dot < 0.0f) dot = -dot;
                                dot *= 1.5f;
                                vec19c += ml::CVec3(vec1a8.x * dot, 0.0f, vec1a8.z * dot);
                            }
                            f31v = 0.99f;
                        }
                        *(u32*)&self->field3F0 = *(u32*)&vec19c.x;
                        *(u32*)&self->field3F4 = *(u32*)&vec19c.y;
                        *(u32*)&self->field3F8 = *(u32*)&vec19c.z;
                        self->field0C |= 0x30;
                    } else {
                        self->field0C &= ~0x40000;
                    }
                } else {
                    self->field0C &= ~0x40000;
                }
                if (secIdx != -1) {
                    self->field4EC |= 0x8000000;
                }
                if (self->field530 & 0x400) {
                    if (self->mData.mA260 & 0x20) {
                        ml::CVec3 diff = self->vec3A8() - self->vec3B4();
                        f32 len2 = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
                        if (len2 <= 64.0f) {
                            if (ScnRes_SegQueryForward_E348(&self->vec3A8(), &self->vec3B4(), 0, 0x4000, 0)) {
                                self->field4EC |= 0x80;
                            }
                        }
                    }
                }
                if (needDefault != 0) {
                    ml::CVec3 v = self->vec3F0() * f31v;
                    *(u32*)&self->field3F0 = *(u32*)&v.x;
                    *(u32*)&self->field3F4 = *(u32*)&v.y;
                    *(u32*)&self->field3F8 = *(u32*)&v.z;
                }
            }
        }
    }
}




// Sets flag bits on the actor object: 0x160 in the +0xC word and bit 0 in
// the +0x4EC word (retail: two independent read-modify-write pairs).
void AnimGame_SetStateFlags(cf::CActParamAnimGame* self) {
    self->field0C |= 0x160;
    self->field4EC |= 1;
}

extern "C" void runBattleMove(void* selfV) {
    cf::CActParamAnimGame* self = (cf::CActParamAnimGame*)selfV;
    s32 moved = 0;
    u32 flags;
    f32 f28v = 0.0f;
    f32 f29v;
    f32 f31v;
    f32 f30v;
    ml::CVec3 pos;
    ml::CVec3 up;
    ml::CVec3 dir;
    ml::CVec3 up2;
    ml::CVec3 dir2;
    ml::CVec3 accel;
    ml::CVec3 target;
    ml::CVec3 dirM;

    self->field530 &= 0xFF3F;
    self->field52C = 0;
    self->field4EC &= ~0xC0000;
    self->field394 = 1.0f;
    self->field398 = 1.0f;
    self->vec3F0() = zero__Q22ml5CVec3;
    setAnimPosVec(self, &zero__Q22ml5CVec3);
    shiftAnimPos(self);

    if ((self->field0C & 0x100) != 0) goto exit;
    if (self->object3A0 == 0) goto exit;

    moved = 1;
    flags = self->field530 | 0x800;
    self->field530 = (u16)flags;
    pos.x = self->field3A8;
    pos.y = self->field3AC;
    pos.z = self->field3B0;
    if ((flags & 0xC) == 0) goto mode0;

    self->field530 |= 0x80;
    if ((self->field530 & 2) == 0) goto bit1clear;

    if (self->field530 & 4) pos.y += 0.5f;
    if (self->field530 & 8) {
        pos.y -= 2.0f;
        pos.x = self->field514 - 2.0f * SinFIdx__Q24nw4r4mathFf(40.743663f * self->field444);
        pos.z = self->field51C - 2.0f * CosFIdx__Q24nw4r4mathFf(40.743663f * self->field444);
    }
    self->field530 &= 0xFFF1;
    goto commit;

bit1clear:
    {
        f32 f31v = isTvFormatPal__9CDeviceVIFv() ? 1.2f : 1.0f;
        f32 f29v = simGetTvFadeConst();
        f32 f30v = ((CActParamAnim*)self)->getAttachedAnimRate() - f29v;
        f32 f29b = ((CActParamAnim*)self)->getAttachedAnimTime();
        if (((CActParamAnim*)self)->getAttachedAnimScale() * f31v + f29b >= f30v) {
        } else {
            self->field530 |= 2;
        }
    }
    moved = 0;
    pos.x += 0.1f * (self->field514 - pos.x);
    pos.z += 0.1f * (self->field51C - pos.z);
    {
        dir = ml::CVec3(SinFIdx__Q24nw4r4mathFf(40.743663f * self->field444), 0.0f, CosFIdx__Q24nw4r4mathFf(40.743663f * self->field444));
        up = pos + ((self->field530 & 4) ? ml::CVec3(0.0f, 1.0f, 0.0f) : ml::CVec3(0.0f, -1.5f, 0.0f));
        ml::CVec3 t3 = up + dir * 3.0f;
        if (ScnRes_SegQueryForward_E348(&up, &t3, self->field4A8, 0, 0) != 0
            && ScnRes_EntryFlagThunk_E5A4(8192, 0) != 0) {
            ml::CVec3 v;
            ScnRes_CopyEntryMid(&v, 0);
            if (v.y <= 0.7071f) {
                setTurnScale((CActParamAnim*)self, 0.024543693f * Atan2FIdx__Q24nw4r4mathFff(-v.x, -v.z));
            }
        }
    }
commit:
    if (self->field4E8 != 0) ColiSetAxisBlockInverse((u8*)self->field4E8 + 0x60C, &pos);
    setAnimSubPos((CActParamAnim*)self, (const CActParamAnimData3*)&pos);
    goto next;

mode0:
    if ((flags & 0x30) != 0) {
        up2 = pos + ml::CVec3(0.0f, 1.0f, 0.0f);
        dir2 = ml::CVec3(SinFIdx__Q24nw4r4mathFf(40.743663f * self->field444), 0.0f, CosFIdx__Q24nw4r4mathFf(40.743663f * self->field444));
        ml::CVec3 t3b = up2 + dir2 * 2.0f;
        if (ScnRes_SegQueryForward_E348(&up2, &t3b, self->field4A8, 0, 0) != 0
            && ScnRes_EntryFlagThunk_E5A4(8192, 0) != 0) {
            ml::CVec3 v;
            ScnRes_CopyEntryMid(&v, 0);
            if (v.y <= 0.7071f) {
                setTurnScale((CActParamAnim*)self, 0.024543693f * Atan2FIdx__Q24nw4r4mathFff(-v.x, -v.z));
            }
        }
        if (self->field530 & 0x20) {
            pos.y += 0.1f * (self->field518 - pos.y);
            setAnimSubPos((CActParamAnim*)self, (const CActParamAnimData3*)&pos);
        }
        moved = 0;
        goto next;
    }
    if ((self->field530 & 0x100) != 0) {
        f32 dx = self->field514 - pos.x;
        f32 dz = self->field51C - pos.z;
        if (dx * dx + dz * dz < 0.0001f) {
            self->field530 &= 0xFEFF;
        } else {
            pos.x += 0.3f * dx;
            pos.z += 0.3f * dz;
            setAnimSubPos((CActParamAnim*)self, (const CActParamAnimData3*)&pos);
            moved = 0;
        }
    }
next:
    if (self->field530 & 0x200) {
        if (((cf::CBattleState*)self->owner08)->CObjectParam_getParamPtr() != 0 || isSpeedBelow(self) != 0) {
            self->field530 &= 0xFDFF;
        } else {
            if (sAnimGameYawUpperInit == 0) { sAnimGameYawUpperRad = 60.0f * 0.017453292f; sAnimGameYawUpperInit = 1; }
            if (sAnimGameYawLowerInit == 0) { sAnimGameYawLowerRad = -60.0f * 0.017453292f; sAnimGameYawLowerInit = 1; }
            f32 ang = *(f32*)((u8*)self->owner08 + 0xC);
            if (!(ang <= sAnimGameYawUpperRad) || ang <= sAnimGameYawLowerRad) {
                self->field530 &= 0xFDFF;
            }
        }
    }
    if (moved == 0) goto exit;

    f28v = 0.0f;
    accel = ml::CVec3(0.0f, 0.0f, 0.0f);
    if ((self->field530 & 0x200) == 0) {
        f29v = *(f32*)((u8*)self->owner08 + 0x14);
        if (f29v * self->field430 >= getAnimMinSpeed()) {
            f28v = *(f32*)((u8*)self->owner08 + 0x14) * self->field394;
        }
        f31v = f28v * (1000.0f * getAnimWalkKmh(self) / 3600.0f) * (self->field390 * getSecPerFrame__9CDeviceVIFv());
        if (self->field530 & 0x400) {
            f32 a = *(f32*)((u8*)self->owner08 + 0xC);
            f30v = fabsf(f31v * SinFIdx__Q24nw4r4mathFf(40.743663f * a));
            accel.y = f31v * CosFIdx__Q24nw4r4mathFf(40.743663f * a);
            ml::CVec3 d2 = ml::CVec3(SinFIdx__Q24nw4r4mathFf(40.743663f * self->field444), 0.0f, CosFIdx__Q24nw4r4mathFf(40.743663f * self->field444));
            ml::CVec3 d3 = ml::CVec3(SinFIdx__Q24nw4r4mathFf(40.743663f * self->field440), 0.0f, CosFIdx__Q24nw4r4mathFf(40.743663f * self->field440));
            ml::CVec3 upv = ml::CVec3(0.0f, 1.0f, 0.0f);
            ml::CVec3 cross;
            PSVECCrossProduct((const Vec*)&d2, (const Vec*)&upv, (Vec*)&cross);
            f32 len2 = cross.x * cross.x + cross.y * cross.y + cross.z * cross.z;
            if (len2 == 0.0f) {
                cross = zero__Q22ml5CVec3;
            } else {
                PSVECNormalize((const Vec*)&cross, (Vec*)&cross);
            }
            f32 dot = nw4r::math::VEC3Dot((const nw4r::math::VEC3*)&cross, (const nw4r::math::VEC3*)&d3);
            if (dot <= 0.0f) f30v = -f30v;
            accel.x = cross.x * f30v;
            accel.z = cross.z * f30v;
        } else {
            accel.x = f31v * SinFIdx__Q24nw4r4mathFf(40.743663f * self->field440);
            accel.z = f31v * CosFIdx__Q24nw4r4mathFf(40.743663f * self->field440);
            ml::CVec3 d4 = ml::CVec3(SinFIdx__Q24nw4r4mathFf(40.743663f * self->field444), 0.0f, CosFIdx__Q24nw4r4mathFf(40.743663f * self->field444));
            ml::CVec3 d5 = ml::CVec3(SinFIdx__Q24nw4r4mathFf(40.743663f * self->field440), 0.0f, CosFIdx__Q24nw4r4mathFf(40.743663f * self->field440));
            f32 dot = nw4r::math::VEC3Dot((const nw4r::math::VEC3*)&d4, (const nw4r::math::VEC3*)&d5);
            accel.y = f31v * dot;
            f32 scale = 1.0f - fabsf(dot);
            accel.x *= scale;
            accel.z *= scale;
        }
    }
    {
        target.x = self->field3A8;
        target.y = self->field3AC;
        target.z = self->field3B0;
        dirM = ml::CVec3(SinFIdx__Q24nw4r4mathFf(40.743663f * self->field444), 0.0f, CosFIdx__Q24nw4r4mathFf(40.743663f * self->field444));
        moved = 1;
        u32 hit;
        if (accel.x == 0.0f && accel.y == 0.0f && accel.z == 0.0f) {
            ml::CVec3 tpd = target + dirM * 2.0f;
            hit = ScnRes_SegQueryForward_E348(&target, &tpd, 0, 8192, 0) != 0;
        } else {
            ml::CVec3 va = target + accel;
            ml::CVec3 vb = dirM * 3.0f;
            ml::CVec3 vc = va - dirM;
            ml::CVec3 vd = vc + vb;
            hit = ScnRes_SegQueryForward_E348(&vc, &vd, 0, 8192, 0) != 0;
            if (hit == 0) {
                ml::CVec3 t2 = target + dirM * 2.0f;
                hit = ScnRes_SegQueryForward_E348(&target, &t2, 0, 8192, 0) != 0;
            }
        }
        if (hit == 0) {
            target += ml::CVec3(0.01f, 0.01f, 0.01f);
            ml::CVec3 tpd = target + dirM * 2.0f;
            hit = ScnRes_SegQueryForward_E348(&target, &tpd, 0, 8192, 0) != 0;
        }
        if (hit == 0) goto after_hit;
        f32 nx;
        f32 nz;
        ml::CVec3 v;
        ScnRes_CopyEntryMid(&v, 0);
        if (v.y > 0.7071f) goto after_hit;
        nx = -v.x;
        nz = -v.z;
        f32 len2 = nx * nx + nz * nz;
        if (!(len2 >= 0.0f)) {
            Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 627, lbl_eu_80526300);
        }
        f32 len;
        if (len2 <= 0.0f) {
            len = 0.0f;
        } else {
            len = len2 * nw4r::math::FrSqrt(len2);
        }
        if (len > 0.001f) {
            nx *= 1.0f / len;
            nz *= 1.0f / len;
            nx = 0.3f * (nx - dirM.x) + dirM.x;
            nz = 0.3f * (nz - dirM.z) + dirM.z;
            len2 = nx * nx + nz * nz;
            if (!(len2 >= 0.0f)) {
                Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 627, lbl_eu_80526300);
            }
            f32 lenb;
            if (len2 <= 0.0f) {
                lenb = 0.0f;
            } else {
                lenb = len2 * nw4r::math::FrSqrt(len2);
            }
            if (lenb != 0.0f) {
                nx = nx / lenb;
                nz = nz / lenb;
            } else {
                nx = -v.x;
                nz = -v.z;
            }
        }
        f28v = 0.024543693f * Atan2FIdx__Q24nw4r4mathFff(nx, nz);
        setTurnScale((CActParamAnim*)self, f28v);
        dirM.x = SinFIdx__Q24nw4r4mathFf(40.743663f * f28v);
        dirM.z = CosFIdx__Q24nw4r4mathFf(40.743663f * f28v);
        if (accel.x == 0.0f && accel.y == 0.0f && accel.z == 0.0f) goto ground;
        {
            f32 f2v = 0.024543693f * Atan2FIdx__Q24nw4r4mathFff(accel.z * dirM.x - accel.x * dirM.z, accel.y);
            if (sAnimGameQuarterPiInit == 0) { sAnimGameQuarterPiRad = ml::pi * 0.25f; sAnimGameQuarterPiInit = 1; }
            f32 p4 = sAnimGameQuarterPiRad;
            if (p4 >= f2v && f2v >= -p4) {
                self->field52C = 1;
            } else if (ml::pi - p4 <= f2v || f2v <= -(ml::pi - p4)) {
                self->field52C = 2;
            } else if (p4 < f2v && f2v < 3.0f * p4) {
                self->field52C = 4;
            } else if (-p4 > f2v && f2v > 3.0f * -p4) {
                self->field52C = 3;
            }
        }
ground:
        {
            ml::CVec3 v2;
            ScnRes_CopyEntryHead(&v2, 0);
            f32 ddx = pos.y - v2.y;
            f32 ddz = pos.x - v2.x;
            if (ddx * ddx + ddz * ddz < 0.0064f) {
                pos.x = v2.x - 0.08f * nx;
                pos.z = v2.z - 0.08f * nz;
            }
        }
        if (self->mData.mA260 & 0x100) {
            accel *= 0.2f;
            ml::CVec3 v2;
            ScnRes_CopyEntryHead(&v2, 0);
            if (self->field4E8 != 0) {
                ml::CVec3 out;
                ml::CVec3 sum = v2 + accel;
                ColiResolveMoveDefault((u8*)self->field4E8 + 0x60C, &out, &sum, &sum, &v2);
                accel += out;
            }
            {
                ml::CVec3 target2;
                target2.x = pos.x;
                target2.y = pos.y + 0.1f;
                target2.z = pos.z;
                if (accel.x != 0.0f || accel.y != 0.0f || accel.z != 0.0f) {
                    if (accel.x != 0.0f || accel.z != 0.0f) {
                        ml::CVec3 v2c0 = ml::CVec3(accel.x, 0.0f, accel.z);
                        f32 mag = PSVECMag((const Vec*)&v2c0);
                        v2c0 *= 1.0f / mag * (0.4f + mag);
                        ml::CVec3 tp = target2 + v2c0;
                        if (ScnRes_SegQueryForward_E348(&target2, &tp, self->field4A8, 0, 0) != 0) {
                            ScnRes_CopyEntryHead(&v2c0, 0);
                            accel.x = v2c0.x - target2.x;
                            accel.z = v2c0.z - target2.z;
                        }
                    }
                    ((BD94CFn6)ScnRes_ResolveGroundHeight)(&target2, &accel, self->field4A8, 0, (self->field0C & 0x100) != 0, 0);  // DEBT(callcast): C symbol; this TU's retail calls omit the trailing FP args of the canonical 11-arg decl
                }
                target2.y += 0.9f;
                if (((BE398Fn4)ScnRes_VertRayForward_E398)(&target2, self->field4A8, 0, 0) != 0) {  // DEBT(callcast): C symbol; retail callers use several incompatible arities / GPR-vs-FPR arg views
                    void* e = ScnRes_GetEntryPtr_E50C(0);
                    target2.y -= 1.4f - (((CActParamAnimGameBeObj*)e)->y - target2.y);
                }
                {
                    ml::CVec3 d2 = dirM * 2.0f;
                    ml::CVec3 tmd = target2 - dirM;
                    ml::CVec3 tpd = target2 + d2;
                    u32 hit2 = ScnRes_SegQueryForward_E348(&tmd, &tpd, 0, 8192, 0) != 0;
                    if (hit2 == 0) {
                        target2 += ml::CVec3(0.01f, 0.01f, 0.01f);
                        ml::CVec3 tmd2 = target2 - dirM;
                        ml::CVec3 tpd2 = target2 + dirM * 2.0f;
                        hit2 = ScnRes_SegQueryForward_E348(&tmd2, &tpd2, 0, 8192, 0) != 0;
                    }
                    if (hit2 != 0 && v.y <= 0.7071f) {
                        ml::CVec3 v3;
                        ScnRes_CopyEntryHead(&v3, 0);
                        pos.x = v3.x - 0.08f * nx;
                        pos.y = v3.y - 1.0f;
                        pos.z = v3.z - 0.08f * nz;
                        moved = 0;
                    }
                }
            }
        }
    }
after_hit:
    setAnimSubPos((CActParamAnim*)self, (const CActParamAnimData3*)&pos);
    if (moved == 0) goto exit;
    self->field530 &= 0xF7FF;
    self->field4EC |= 0x40000;
    // func_8004B9D4 is declared in kyoshin/action/CActParamAnim.hpp
    // (CActParamAnim* first arg); cast the view pointer at the call site.
    func_8004B9D4((CActParamAnim*)self, 6, 0, -1, 0);
    setTurnScale((CActParamAnim*)self, self->field444);
    {
        ml::CVec3 zero2 = ml::CVec3(0.0f, 0.0f, 0.0f);
        setAnimPosVec(self, &zero2);
        ml::CVec3 d = ml::CVec3(0.2f * SinFIdx__Q24nw4r4mathFf(40.743663f * self->field444), 0.0f, 0.2f * CosFIdx__Q24nw4r4mathFf(40.743663f * self->field444));
        pos = pos - d;
        setAnimSubPos((CActParamAnim*)self, (const CActParamAnimData3*)&pos);
    }
exit:
    self->field488 = self->field47C = self->field3AC;
    stopAnimMove(self);
}

void cf::CActParamAnimGame::dispatchState() {
    CActParamAnimGame* v = (CActParamAnimGame*)this;
    // Snapshot the flag word, clear it to the run mask, and reset the
    // air/height state; the +0x530 flags lose their low bit. The first
    v->field4F0 = v->getFlags4EC();
    v->field4F8 = lbl_eu_80666040;
    v->field4FC = lbl_eu_80666040;
    v->field4EC &= 0x14AE5A7D;
    v->field530 &= 0xFFFE;

    // Child-data timers: keep the current value only when it is non-negative.
    f32 timerA = v->mData.getParamFloat0C();
    if (timerA >= lbl_eu_80666040) v->field500 = timerA;
    if (v->field504 >= lbl_eu_80666040) v->field500 = v->field504;
    f32 timerB = v->mData.getParamFloat08();
    if (timerB >= lbl_eu_80666040) v->field508 = timerB;
    if (v->field50C >= lbl_eu_80666040) v->field508 = v->field50C;

    getScnHandle();
    if (ScnData_FwdBC9A0() != 0 && (v->field4EC & 1) == 0) {
        trackGroundAt(this);
    }

    if (v->field374 == 2) {
        // In-battle mode: mark ready and run the full movement/step logic.
        // Load +0x530 first (colors r4 at retail), then +0xC.
        u16 field530 = v->field530;
        u32 field0C = v->field0C;
        v->field530 = field530 | 1;
        v->field0C = field0C & ~0x80000;
        runBattleMove(this);
    } else {
        bool over = v->field4F8 > v->field508;
        if (over && (v->field0C & 0x10000) == 0) {
            // Air timer exceeded the fall timer: switch to falling state.
            v->field4EC |= 2;
            v->field0C &= ~0x80000;
            applyFallMove(this);
            goto d2c4_fall_done;
        }
        {
            u16 flags = v->field530;
            if (flags & 0x30) {
                v->field530 &= 0xF5CF;
                setAnimSubPos((CActParamAnim*)this, (const CActParamAnimData3*)&v->field514);
            } else if (flags & 0x800) {
                // Small lateral pull toward the facing direction. Build the
                // position member-wise, clear the flag between the position
                // setup and the trig calls (retail interleave), and phrase
                // each offset as const*trig so fmuls colors f0=const,f1=field444.
                ml::CVec3 p;
                p.x = v->field3A8;
                p.y = v->field3AC;
                p.z = v->field3B0;
                v->field530 &= 0xF7FF;
                p.x -= 0.3f * nw4r::math::SinFIdx(v->field444 * 40.743664f);
                p.z -= 0.3f * nw4r::math::CosFIdx(v->field444 * 40.743664f);
                setAnimSubPos((CActParamAnim*)this, (const CActParamAnimData3*)&p);
            }
            if (v->field4F8 > lbl_eu_80666040) {
                // Airborne: pick the ground-following scale pair from the
                // height delta below the actor.
                f32 drop = v->field4FC - v->field3AC;
                if (v->field4EC & 0x100) {
                    v->field394 = 0.5f;
                    v->field398 = 0.125f;
                } else if (drop < 0.1f) {
                    v->field394 = lbl_eu_80666044;
                    v->field398 = lbl_eu_80666044;
                } else if (drop < 0.7f) {
                    v->field394 = lbl_eu_80666044;
                    v->field398 = 0.1875f;
                } else {
                    v->field394 = 0.7f;
                    v->field398 = 0.175f;
                }
            } else {
                v->field394 = lbl_eu_80666044;
                v->field398 = lbl_eu_80666044;
            }
            ((CActParamAnim*)this)->stepAnimFrame(nullptr);
        }
    d2c4_fall_done:;
    }

    // Notify the +0x3A0 chain's +0x7EC sub-object when the fall state and
    // distance from ground allow it. The +0x4AC word is read before the
    // sub-object null test (retail interleave).
    void* obj = v->object3A0;
    if (obj != 0) {
        void* sub = ((CActParamAnimGameObj3A0*)obj)->sub7EC;
        u32 ac = v->field4AC;
        if (sub != 0) {
            // Forward field4F8 as the float arg so f1 stays live from the
            // compare into CActParam7ECTarget::func08 (+0x08), matching
            // retail (no second lfs before the bctrl).
            f32 air = v->field4F8;
            int arg = 0;
            if (ac < 5 && air < 0.1f && (v->field4EC & 0x8000000) == 0) {
                arg = 1;
            }
            ((CActParam7ECTarget*)sub)->func08(arg, air);
        }
    }

    // Link handoff: when the falling flag was newly set, detach the linked
    // region and reset its two virtual state slots.
    if ((v->field4EC & 2) != 0 && (v->field4F0 & 2) == 0) {
        void* link = MapFx_ResolveOwner(v->field4E8);
        if (link != 0) {
            ((cf::CActorParam*)link)->CActorParam_clearSecondGauge();
            ((cf::CActorParam*)link)->CActorParam_clearGauge();
        }
    }
}

// Typed view for isActionReady: keeps the +0x8 link as a typed pointer.


// Action-readiness query: requires a +0x8 link object, the collision filter
// inside the [0x4,0x4A09] window, the move vec NOT past the fall vec (the
// dead mfcr/extrwi GT capture of the fcmpo), and bit 2 set in the link's
// +0x4 flags; then notifies the region via startAnimDual. The type arg is
// forwarded to startAnimDual untouched (retail leaves r4 as-is), so keeping
// it a live parameter pins r4 and colors the locals into retail's r5/r6.
bool cf::CActParamAnimGame::isActionReady(u32 type) {
    cf::CActParamAnimGame* self = reinterpret_cast<cf::CActParamAnimGame*>(this);
    CActParamAnimOwner* owner = self->owner08;
    if (owner == 0) return false;
    // Filter gate: retail folds the compare against 0x44a05 (too big for a
    // cmpli immediate) into addis(-0x40000)+cmpli(0x4a05).
    if ((u32)(self->field4A8 - 0x40000) != 0x4a05) goto flags;
    {
        // Explicit bool keeps MWCC's dead mfcr/extrwi GT capture of the fcmpo.
        bool over = self->field4F8 > self->field508;
        if (over) goto fail;
    }
flags:
    if ((owner->field04 & 4) == 0) goto fail;
    ((void (*)(void*, u32))startAnimDual)(this, type);
    return true;
fail:
    return false;
}

// C-ABI sibling (retail unmangled name; defined in kyoshin/action/CActParamAnim.cpp)

// Returns true when the +0x4F8 float exceeds the +0x508 float; the linked
// region is then notified via startAnimDual. The explicit bool local keeps
// MWCC's dead mfcr/rlwinm capture of the fcmpo result (retail `extrwi.` shape).
bool cf::CActParamAnimGame::checkHeightThreshold() {
    cf::CActParamAnimGame* self = reinterpret_cast<cf::CActParamAnimGame*>(this);
    bool cond = self->field4F8 > self->field508;
    if (cond) {
        startAnimDual(this);
        return true;
    }
    return false;
}

// Mirror of checkHeightThreshold phrased with <= (retail `cntlzw`/`srwi.` shape).
// The negated comparison keeps MWCC's dead mfcr/cntlzw capture chain.
bool cf::CActParamAnimGame::checkHeightBelow() {
    cf::CActParamAnimGame* self = reinterpret_cast<cf::CActParamAnimGame*>(this);
    bool cond = !(self->field4F8 > self->field508);
    if (cond) {
        startAnimDual(this);
        return true;
    }
    return false;
}

bool stepAnimState__13CActParamAnimFv(void* self);
bool checkNotFalling__Q22cf17CActParamAnimGame(void* self) {
    if (((cf::CActParamAnimGame*)self)->field4EC & 2) {
        return false;
    }
    return stepAnimState__13CActParamAnimFv(self);
}

// Clears the +0x4EC bit-12 (0x80000) flag and notifies the linked region.
bool cf::CActParamAnimGame::clearFlag80000() {
    cf::CActParamAnimGame* self = reinterpret_cast<cf::CActParamAnimGame*>(this);
    if (self->hasFlag80000()) {
        self->field4EC &= ~0x80000;
        startAnimDual(this);
        return true;
    }
    return false;
}

// State-0/1 movement start: state 1 additionally dispatches the +0x8 link's
// CBattleState_UnkVirtualFunc3 status; only when that status AND isSpeedBelow
// both clear is
// the 0x200 flag raised before notifying the region.
bool cf::CActParamAnimGame::startMoveGate(u32 type, u32 state) {
    cf::CActParamAnimGame* self = reinterpret_cast<cf::CActParamAnimGame*>(this);
    if (state == 0) {
        if (probeGroundAt(this)) {
            field530 = (field530 | 4) & ~0x100;
            ((void (*)(void*, u32))startAnimDual)(this, type);
            return true;
        }
    } else if (state == 1) {
        if (refineHitNorm(this)) {
            field530 = (field530 | 8) & ~0x100;
            if (((cf::CBattleState*)self->owner08)->CObjectParam_getParamPtr() == 0 && isSpeedBelow(this) == 0) {
                field530 |= 0x200;
            }
            ((void (*)(void*, u32))startAnimDual)(this, type);
            return true;
        }
    }
    return false;
}

// Sets the +0x530 0x40 flag and notifies the linked region (startAnimDual,
// forwarded the passed type) when the actor is actionable and the current
// +0x52C state matches the requested one. r4 = type forwarded to
// startAnimDual, r5 = requested state (0..3).
bool cf::CActParamAnimGame::fireStateGate(u32 type, u32 state) {
    if (field530 & 0x1C0) return false;
    if (isSpeedAbove(this) != 0 || isSpeedInRange(this) != 0) {
        if (state == 0) {
            if (field52C == 1 && isFiringAllowed()) {
                field530 |= 0x40;
                ((void (*)(void*, u32))startAnimDual)(this, type);
                return true;
            }
        } else if (state == 1) {
            if (field52C == 2) {
                field530 |= 0x40;
                ((void (*)(void*, u32))startAnimDual)(this, type);
                return true;
            }
        } else if (state == 2) {
            if (field52C == 4) {
                field530 |= 0x40;
                ((void (*)(void*, u32))startAnimDual)(this, type);
                return true;
            }
        } else if (state == 3) {
            if (field52C == 3) {
                field530 |= 0x40;
                ((void (*)(void*, u32))startAnimDual)(this, type);
                return true;
            }
        }
    }
    return false;
}

// State-0/1 gate: state 1 uses the facing-offset ground probe
// (snapActorDown), state 0 the waiting probe (probeStandGnd); on success the
// matching 0x20/0x10 bit goes up and the 0x100 bit comes down.
bool cf::CActParamAnimGame::tryGroundStep(u32 type, u32 state) {
    cf::CActParamAnimGame* view = reinterpret_cast<cf::CActParamAnimGame*>(this);
    if (state == 1) {
        if (snapActorDown(this)) {
            u16 flags = view->field530;
            flags |= 0x20;
            flags &= ~0x100;
            view->field530 = flags;
            ((void (*)(void*, u32))startAnimDual)(this, type);
            return true;
        }
    } else if (state == 0) {
        if (probeStandGnd(this)) {
            u16 flags = view->field530;
            flags |= 0x10;
            flags &= ~0x100;
            view->field530 = flags;
            ((void (*)(void*, u32))startAnimDual)(this, type);
            return true;
        }
    }
    return false;
}

// Dash-start gate: needs the base 0x40 flag and the action-source probe
// (probeWallPair), raises the 0x100 flag, zeroes the move vec, resets the
// move-vec accumulator, requests mode 2, then notifies the region.
bool cf::CActParamAnimGame::dashStartGate(u32 type) {
    if (!(mData.mA260 & 0x40)) return false;
    if (probeWallPair(this) == false) return false;
    field530 |= 0x100;
    vec3F0() = zero__Q22ml5CVec3;
    setAnimPosVec(this, &zero__Q22ml5CVec3);
    setAnimCount(this, 2);
    ((void (*)(void*, u32))startAnimDual)(this, type);
    return true;
}

bool cf::CActParamAnimGame::isAnimChainActive() {
    if (this->field4EC & 0x00040000) {
        startAnimDual(this);
        return true;
    }
    return false;
}

// Walks the game-manager linked list: for each node resolves its action
// source via MapFx_ResolveOwner and compares scale * CActorParam_getDamageScale
// against CActorParam_getHp; when
// any source passes, notifies the region via startAnimDual. The list head is
// re-fetched from the manager every iteration (retail reloads it at the loop
// bottom).
bool cf::CActParamAnimGame::scanActorList(u32 type) {
    CActParamAnimGameObj3A0* obj = (CActParamAnimGameObj3A0*)object3A0;
    if (obj == 0) return false;
    if (*(u32*)obj->_00 != (u32)gmScene) return false;

    CActParamAnimGameListNode* node =
        (CActParamAnimGameListNode*)((CActParamAnimGameMgr*)getGimmickListHead__Q22cf13CfGameManagerFv())->head;
    f32 scale = 0.2f;
    bool found = false;
    while (node !=
           (CActParamAnimGameListNode*)((CActParamAnimGameMgr*)getGimmickListHead__Q22cf13CfGameManagerFv())->head) {
        void* item = MapFx_ResolveOwner(node->f08);
        f32 limit = ((cf::CActorParam*)item)->CActorParam_getHp();
        f32 value = ((cf::CActorParam*)item)->CActorParam_getDamageScale();
        if (scale * value >= limit) {
            found = true;
            break;
        }
        node = (CActParamAnimGameListNode*)node->next;
    }
    if (found) {
        ((void (*)(void*, u32))startAnimDual)(this, type);
        return true;
    }
    return false;
}

float cf::CActorParam::CActorParam_getDamageScale() { return unk17E4.field_10; }

// Returns true (and notifies the linked region) when the +0x4F4 link is
// active; otherwise falls back to the +0x4E8 link when +0x4F4 is null.
// startAnimDual takes (self, type) in retail; the type slot (r4) is a
// leftover register at these call sites. Declaring it as a (live) second
// parameter keeps r4 busy as a call-arg register so the CSE'd link pointer
// lands in r5 like retail (__wpadInfoCallback precedent).
bool cf::CActParamAnimGame::tryLinkActive(u32 type) {
    CActParamAnimGameLink* link = field4F4;
    if (link != 0 && link->active != 0) {
        ((void (*)(void*, u32))startAnimDual)(this, type);
        return true;
    }
    if (link == 0) {
        link = field4E8;
        if (link != 0 && link->active != 0) {
            ((void (*)(void*, u32))startAnimDual)(this, type);
            return true;
        }
    }
    return false;
}

// Trivial leaf: always false.
bool cf::CActParamAnimGame::isAnimBlendActive() { return false; }

// Rotation-matrix facing update (state 2 only): builds a 3x4 matrix from the
// cached yaw (sin/cos of 40.743663f * field_444), rotates the yaw basis
// through its first row, then walks the actor back along the new facing by
// 104 units and sets the +0x4EC 0x40000 flag / clears the +0x0C 0x80 flag.
// The position call is retail's 4-arg setAnimSubPos(self, out, pos, delta).
// The yaw is snapshotted into a local for the first sin/cos pair (keeps two
// callee-saved FPRs live like retail); the second pair reads field_444
// directly so MWCC reloads it fresh each time.
void cf::CActParamAnimGame::snapFacingYaw() {
    CActParamAnimGame* v = (CActParamAnimGame*)this;
    if (v->field374 != 2) {
        solveHeadRate();
        return;
    }
    f32 ang = v->field444;
    f32 m[3][4];
    ml::CVec3 d;
    m[0][0] = lbl_eu_80666040;
    m[0][1] = lbl_eu_80666040;
    m[0][2] = 0.8333333f;
    f32 sinv = nw4r::math::SinFIdx(40.743664f * ang);
    f32 cosv = nw4r::math::CosFIdx(40.743664f * ang);
    m[0][3] = cosv;
    m[1][0] = lbl_eu_80666040;
    m[1][1] = sinv;
    m[1][2] = lbl_eu_80666040;
    m[1][3] = lbl_eu_80666044;
    m[2][0] = lbl_eu_80666040;
    m[2][1] = -sinv;
    m[2][2] = lbl_eu_80666040;
    m[2][3] = cosv;
    // Retail feeds the rotation math through double round-trips (three frsp)
    // while the matrix keeps raw float copies.
    ml::CVec3 rot;
    rot.x = m[0][0] * (f32)-(double)sinv + m[0][1] * lbl_eu_80666040 + m[0][2] * (f32)(double)cosv;
    rot.y = m[0][0] * lbl_eu_80666040 + m[0][1] * lbl_eu_80666044 + m[0][2] * lbl_eu_80666040;
    rot.z = m[0][0] * (f32)(double)cosv + m[0][1] * lbl_eu_80666040 + m[0][2] * (f32)(double)sinv;
    setAnimPosVec(this, &rot);
    setAnimCount(this, 0);
    d.x = 0.2f * nw4r::math::SinFIdx(v->field444 * 40.743664f);
    d.y = lbl_eu_80666040;
    d.z = 0.2f * nw4r::math::CosFIdx(v->field444 * 40.743664f);
    ml::CVec3 delta(v->field3A8 - d.x, v->field3AC - d.y, v->field3B0 - d.z);
    ml::CVec3 target;
    target.x = delta.x;
    target.y = delta.y;
    target.z = delta.z;
    ((bool (*)(void*, ml::CVec3*, const ml::CVec3*, const ml::CVec3*))setAnimSubPos)(this, &target, &delta, &d);
    // Interleaved RMW pair: compute both masked words, then store (retail shape).
    u32 flags4ec = v->field4EC | 0x40000;
    u32 flags0c = v->field0C & ~0x80;
    v->field4EC = flags4ec;
    v->field0C = flags0c;
}

// Ground-hit record written by ScnRes_CopyEntryMid/ScnRes_CopyEntryHead: contact x/z plus
// extra floats consumed by the facing-vector refinement in the step probes.
struct CActParamAnimGameHitRec {
    f32 x;   // +0x00
    f32 y;   // +0x04
    f32 z;   // +0x08
    f32 w;   // +0x0C
    u8 _10[4];
    f32 t;   // +0x14
};

// Facing-dir ground-step probe (variant of refineHitNorm used while moving):
// lifts pos by (0,1,0), requires the facing dir to point along the movement
// cone (dot >= lbl_8066605C), probes ahead, refines the yaw against the hit
// normal, and records the ground point at +0x514.
bool probeGroundAt(cf::CActParamAnimGame* selfV) {
    cf::CActParamAnimGame* self = (cf::CActParamAnimGame*)selfV;
    ml::CVec3 vecUp;
    vecUp.x = lbl_eu_80666040;
    vecUp.y = lbl_eu_80666044;
    vecUp.z = lbl_eu_80666040;
    ml::CVec3 vec50 = self->vec3A8() + vecUp;
    ml::CVec3 vecEC(vec50);
    ml::CVec3 sincos;
    sincos.x = nw4r::math::SinFIdx(40.743664f * self->field444);
    sincos.y = lbl_eu_80666040;
    sincos.z = nw4r::math::CosFIdx(40.743664f * self->field444);
    // Normalize the facing dir; a zero-length dir stays zero.
    ml::CVec3 dir;
    dir.x = self->field3CC;
    dir.y = self->field3D0;
    f32 len2 = self->field3D0 * self->field3D0;
    dir.z = self->field3D4;
    len2 = dir.x * dir.x + len2;
    len2 = dir.z * dir.z + len2;
    if (len2 == lbl_eu_80666040) {
        dir = zero__Q22ml5CVec3;
    } else {
        PSVECNormalize((const Vec*)&dir, (Vec*)&dir);
    }
    // Movement-cone gate: facing must align with the anim direction.
    if (!(dir.x * sincos.x + dir.z * sincos.z >= 0.8f)) {
        return false;
    }
    ml::CVec3 vec44;
    nw4r::math::VEC3Sub((nw4r::math::VEC3*)&vec44, vecEC, sincos);
    ml::CVec3 vec38;
    nw4r::math::VEC3Scale((nw4r::math::VEC3*)&vec38, sincos, 2.0f);
    ml::CVec3 vec80 = vec38;
    ml::CVec3 vec2C = vecEC + vec80;
    ml::CVec3 vec8C(vec2C);
    ml::CVec3 vec98(vec44);
    // Wall sweep ahead of the actor (wall mask 0x2000, no region filter).
    if (ScnRes_SegQueryForward_E348(&vec98, &vec8C, 0, 0x2000, 0) == 0) {
        return false;
    }
    ml::CVec3 hit;
    ScnRes_CopyEntryHead(&hit, 0);
    // Landing-point radius gate: contact must stay within lbl_619F8 of
    // the lifted position (x/z only).
    if (!((hit.x - vecEC.x) * (hit.x - vecEC.x) +
              (hit.z - vecEC.z) * (hit.z - vecEC.z) <=
          lbl_eu_806619F8 * lbl_eu_806619F8)) {
        return false;
    }
    // Fetch the wall normal, then re-probe along the facing dir with the
    // region filter enabled to find the true step-up ground point.
    ml::CVec3 norm;
    ScnRes_CopyEntryMid(&norm, 0);
    ml::CVec3 vec20;
    nw4r::math::VEC3Sub((nw4r::math::VEC3*)&vec20, vecEC, sincos);
    ml::CVec3 vec14;
    nw4r::math::VEC3Scale((nw4r::math::VEC3*)&vec14, sincos, 2.0f);
    ml::CVec3 vec5C = vec14;
    ml::CVec3 vec08 = vecEC + vec5C;
    ml::CVec3 vec68(vec08);
    ml::CVec3 vec74(vec20);
    if (ScnRes_SegQueryForward_E348(&vec74, &vec68, self->field4A8, 0, 0) == 0) {
        return false;
    }
    ml::CVec3 hit2;
    ScnRes_CopyEntryHead(&hit2, 0);
    // Both contacts must agree within lbl_6120.
    if ((hit2.x - hit.x) * (hit2.x - hit.x) +
            (hit2.y - hit.y) * (hit2.y - hit.y) +
            (hit2.z - hit.z) * (hit2.z - hit.z) >
        0.010000001f) {
        return false;
    }
    // Refine the ground point along the wall normal, re-aim the actor,
    // then record the validated step destination at +0x514.
    vecEC.x = -norm.x * lbl_eu_806619F8 + hit.x;
    vecEC.y = self->field3AC + 0.5f;
    vecEC.z = -norm.z * lbl_eu_806619F8 + hit.z;
    setTurnScale((CActParamAnim*)selfV, 0.024543693f * Atan2FIdx__Q24nw4r4mathFff(-norm.x, -norm.z));
    if (ScnRes_VertRayForward_E398((float*)&vecEC, self->field4A8, 0, 0,
                                 -2.0f, lbl_eu_8066AF20) != 0) {
        ScnRes_CopyEntryHead(&self->field514, 0);
    } else {
        self->field514 = vecEC.x;
        self->field518 = vecEC.y;
        self->field51C = vecEC.z;
    }
    return true;
}


// Ground-step probe: casts a ray from pos+(0,1,0) along the normalized
// facing dir, validates the landing point stays within radius of the actor,
// then refines the facing vector against the hit normal and records the new
// ground point at +0x514. Returns false when any probe misses.
bool refineHitNorm(cf::CActParamAnimGame* selfV) {
    cf::CActParamAnimGame* self = (cf::CActParamAnimGame*)selfV;
    ml::CVec3 up(lbl_eu_80666040, lbl_eu_80666044, lbl_eu_80666040);
    ml::CVec3 start = self->vec3A8() + up;
    ml::CVec3 base(start);
    // Normalize the facing dir; a zero-length dir stays zero.
    ml::CVec3 dir(self->vec3CC());
    f32 len2 = dir.y * dir.y;
    len2 = dir.x * dir.x + len2;
    len2 = dir.z * dir.z + len2;
    if (len2 == lbl_eu_80666040) {
        dir = zero__Q22ml5CVec3;
    } else {
        PSVECNormalize((const Vec*)&dir, (Vec*)&dir);
    }
    ml::CVec3 ahead = dir * 1.3f;
    ml::CVec3 aheadC(ahead);
    ml::CVec3 dest = aheadC + base;
    ml::CVec3 destC(dest);
    if (ScnRes_SegQueryForward_E348(&base, &destC, self->field4A8, 0, 0) != 0) {
        return false;
    }
    if (ScnRes_VertRayForward_E398((float*)&destC, self->field4A8, 0, 0,
                                 -2.0f, lbl_eu_8066AF20) != 0) {
        return false;
    }
    ml::CVec3 step = dir * 2.0f;
    ml::CVec3 stepC(step);
    destC.y -= 2.0f;
    ml::CVec3 delta = destC - stepC;
    ml::CVec3 deltaC(delta);
    // Second sweep drops the region filter and uses the wall mask 0x2000.
    if (ScnRes_SegQueryForward_E348(&destC, &deltaC, 0, 0x2000, 0) == 0) {
        return false;
    }
    // Landing point must stay within lbl_806619FC of the actor (x/z only).
    ml::CVec3 land;
    ScnRes_CopyEntryHead(&land, 0);
    f32 dx = land.x - self->field3A8;
    f32 dz = land.z - self->field3B0;
    f32 d2 = dz * dz;
    d2 = dx * dx + d2;
    if (!(d2 <= lbl_eu_806619FC * lbl_eu_806619FC)) {
        return false;
    }
    ml::CVec3 norm;
    ScnRes_CopyEntryMid(&norm, 0);
    // Re-aim along the ground normal's yaw.
    setTurnScale((CActParamAnim*)selfV, 0.024543693f * Atan2FIdx__Q24nw4r4mathFff(-norm.x, -norm.z));
    // Retail reuses the dead 'base' slot for the retarget point.
    base.x = -norm.x * lbl_eu_806619FC + land.x;
    base.y = self->field3AC + lbl_eu_80666044;
    base.z = -norm.z * lbl_eu_806619FC + land.z;
    if (ScnRes_VertRayForward_E398((float*)&base, self->field4A8, 0, 0,
                                 -2.0f, lbl_eu_8066AF20) != 0) {
        ScnRes_CopyEntryHead(&self->field514, 0);
    } else {
        self->field514 = base.x;
        self->field518 = base.y;
        self->field51C = base.z;
    }
    return true;
}

// Movement-grounding probe used by the "waiting" anim states (1/3/4): lifts
// the position by lbl_806660BC, casts a probe along the facing dir (scaled/
// offset), and when a ground hit is found within lbl_80666050 of the current
// height, snaps the actor to it and records the ground point.
extern "C" bool probeStandGnd(cf::CActParamAnimGame* selfV) {
    cf::CActParamAnimGame* self = (cf::CActParamAnimGame*)selfV;
    if (self->field52C == 1 || (u32)(self->field52C - 3) <= 1) {
        ml::CVec3 vec(self->field3A8, self->field3AC, self->field3B0);
        if (ScnRes_VertRayForward_E398((float*)&vec, self->field4A8, 0, 0, 2.5f, lbl_eu_80666040) == 0) {
            vec.y += 2.5f;
            f32 sinv = nw4r::math::SinFIdx(40.743664f * self->field444);
            ml::CVec3 dir;
            dir.x = sinv;
            f32 cosv = nw4r::math::CosFIdx(40.743664f * self->field444);
            dir.y = lbl_eu_80666040;
            dir.z = cosv;
            dir.x = dir.x * lbl_eu_80666044 + vec.x;
            dir.y = dir.y * lbl_eu_80666044 + vec.y;
            dir.z = dir.z * lbl_eu_80666044 + vec.z;
            if (ScnRes_SegQueryForward_E348(&vec, &dir, self->field4A8, 0, 0) == 0) {
                if (ScnRes_VertRayForward_E398((float*)&dir, self->field4A8, 0, 0, -2.0f, lbl_eu_8066AF20) != 0) {
                    ml::CVec3 g;
                    ScnRes_CopyEntryHead(&g, 0);
                    if (g.y - self->field3AC <= 2.0f) {
                        ml::CVec3 t(self->field3A8, self->field3AC, self->field3B0);
                        t.y = g.y - 2.0f;
                        setAnimSubPos((CActParamAnim*)selfV, (const CActParamAnimData3*)&t);
                        self->field514 = g.x;
                        self->field518 = g.y;
                        self->field51C = g.z;
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

// Ground-probe variant for states 2-4: offsets the position along the facing
// direction, probes down, and when a ground hit is found, records the ground
// point, re-probes, and snaps the actor to it.
extern "C" bool snapActorDown(cf::CActParamAnimGame* selfV) {
    cf::CActParamAnimGame* self = (cf::CActParamAnimGame*)selfV;
    ml::CVec3 vec;
    if ((u32)(self->field52C - 2) <= 2u) {
        vec.x = self->field3A8;
        vec.y = self->field3AC;
        vec.z = self->field3B0;
        f32 sinv = nw4r::math::SinFIdx(self->field444 * 40.743664f);
        vec.x = vec.x + 0.1f * (-sinv);
        f32 cosv = nw4r::math::CosFIdx(self->field444 * 40.743664f);
        vec.z = vec.z + 0.1f * (-cosv);
        vec.y = vec.y + lbl_eu_80666044;
        if (ScnRes_VertRayForward_E398((float*)&vec, self->field4A8, 0, 0, -1.0f, lbl_eu_80666040) != 0) {
            ml::CVec3 g;
            ScnRes_CopyEntryHead(&g, 0);
            if (self->field3AC - g.y <= 0.5f) {
                f32 sinv2 = nw4r::math::SinFIdx(self->field444 * 40.743664f);
                self->field514 = self->field3A8 - sinv2;
                f32 cosv2 = nw4r::math::CosFIdx(self->field444 * 40.743664f);
                self->field51C = self->field3B0 - cosv2;
                self->field518 = g.y;
                *(u32*)&vec.y = *(u32*)&self->field518;
                *(u32*)&vec.x = *(u32*)&self->field514;
                *(u32*)&vec.z = *(u32*)&self->field51C;
                vec.y = lbl_eu_80666044 + self->field3AC;
                if (ScnRes_VertRayForward_E398((float*)&vec, self->field4A8, 0, 0, -1.5f, lbl_eu_80666040) != 0) {
                    ScnRes_CopyEntryHead(&g, 0);
                    self->field518 = g.y;
                }
                *(u32*)&vec.y = *(u32*)&self->field3AC;
                *(u32*)&vec.x = *(u32*)&self->vec3A8();
                *(u32*)&vec.z = *(u32*)&self->field3B0;
                vec.y = self->field518;
                setAnimSubPos((CActParamAnim*)selfV, (const CActParamAnimData3*)&vec);
                return true;
            }
        }
    }
    return false;
}

// Facing-dir step probe with movement-cone and landing-radius gates; on
// success re-aims the actor along the ground normal's yaw and writes the
// refined ground point to +0x514/+0x518/+0x51C. Three sweeps share the same
// ray shape (lifted anchor +/- facing dir): wall probes without the region
// filter first, then the filtered probe whose contact pair must agree.
bool probeWallPair(cf::CActParamAnimGame* selfV) {
    cf::CActParamAnimGame* self = (cf::CActParamAnimGame*)selfV;
    // Copy the facing dir once; normalization below happens in place.
    ml::CVec3 dir = self->vec3CC();
    // Degenerate facing (no x/z components): nothing to probe against.
    if (dir.x == lbl_eu_80666040 && dir.z == lbl_eu_80666040) {
        return false;
    }
    ml::CVec3 sincos;
    // The trig results live only inside this struct (retail reloads them
    // from its stack slot around each probe).
    sincos.x = nw4r::math::SinFIdx(40.743664f * self->field444);
    sincos.z = nw4r::math::CosFIdx(40.743664f * self->field444);
    // Normalize the facing dir; a zero-length dir stays zero.
    f32 len2 = dir.y * dir.y;
    len2 = dir.x * dir.x + len2;
    len2 = dir.z * dir.z + len2;
    if (len2 == lbl_eu_80666040) {
        dir = zero__Q22ml5CVec3;
    } else {
        PSVECNormalize((const Vec*)&dir, (Vec*)&dir);
    }
    // Movement-cone gate.
    if (!(dir.x * sincos.x + dir.z * sincos.z >= 0.8f)) {
        return false;
    }
    sincos.y = lbl_eu_80666040;

    // Sweep 1: unfiltered wall probe around pos+(0,lbl_80666050).
    ml::CVec3 lift1(lbl_eu_80666040, 2.0f, lbl_eu_80666040);
    ml::CVec3 vec80 = self->vec3A8() + lift1;
    ml::CVec3 anchorC(vec80);
    ml::CVec3 vec74;
    nw4r::math::VEC3Sub((nw4r::math::VEC3*)&vec74, anchorC, sincos);
    ml::CVec3 vec68;
    nw4r::math::VEC3Scale((nw4r::math::VEC3*)&vec68, sincos, 2.0f);
    ml::CVec3 vecEC(vec68);
    ml::CVec3 vec5C = vec74 + vecEC;
    ml::CVec3 vec98(vec74);
    ml::CVec3 vecAC(vec5C);
    if (ScnRes_SegQueryForward_E348(&vec98, &vecAC, 0, 0x2000, 0) == 0) {
        return false;
    }
    ml::CVec3 hit;
    ScnRes_CopyEntryHead(&hit, 0);
    // Landing point must stay within the radius of the lifted anchor (xz).
    f32 dx = hit.x - anchorC.x;
    f32 dz = hit.z - anchorC.z;
    if (dx * dx + dz * dz > 0.010000001f * 0.010000001f) {
        return false;
    }

    // Sweep 2: second unfiltered wall probe around pos+(0,lbl_80666080).
    ml::CVec3 lift2(lbl_eu_80666040, 0.5f, lbl_eu_80666040);
    ml::CVec3 vec50 = self->vec3A8() + lift2;
    anchorC = vec50;
    ml::CVec3 vec44;
    nw4r::math::VEC3Sub((nw4r::math::VEC3*)&vec44, anchorC, sincos);
    ml::CVec3 vec38;
    nw4r::math::VEC3Scale((nw4r::math::VEC3*)&vec38, sincos, 2.0f);
    ml::CVec3 vecB0(vec38);
    ml::CVec3 vec2C = vec44 + vecB0;
    ml::CVec3 vecBC(vec44);
    ml::CVec3 vecC8(vec2C);
    if (ScnRes_SegQueryForward_E348(&vecC8, &vecBC, 0, 0x2000, 0) == 0) {
        return false;
    }
    ml::CVec3 norm;
    ScnRes_CopyEntryMid(&norm, 0);

    // Sweep 3: the same ray as sweep 2 but with the region filter enabled.
    ml::CVec3 vec14;
    nw4r::math::VEC3Scale((nw4r::math::VEC3*)&vec14, sincos, 2.0f);
    ml::CVec3 vec20;
    nw4r::math::VEC3Sub((nw4r::math::VEC3*)&vec20, anchorC, sincos);
    ml::CVec3 vec8 = anchorC + vec14;
    ml::CVec3 vec98b(vec20);
    ml::CVec3 vecA4(vec8);
    if (ScnRes_SegQueryForward_E348(&vecA4, &vec98b, self->field4A8, 0, 0) == 0) {
        return false;
    }
    ml::CVec3 land;
    ScnRes_CopyEntryHead(&land, 0);
    // Both contacts must agree within the radius (full xyz); the difference
    // is folded back into the landing-point buffer like retail.
    nw4r::math::VEC3Sub((nw4r::math::VEC3*)&land,
                        reinterpret_cast<const nw4r::math::VEC3*>(&land),
                        reinterpret_cast<const nw4r::math::VEC3*>(&hit));
    if (land.x * land.x + land.y * land.y + land.z * land.z > 0.010000001f) {
        return false;
    }
    // Re-aim along the ground normal's yaw.
    setTurnScale((CActParamAnim*)selfV, 0.024543693f * Atan2FIdx__Q24nw4r4mathFff(-norm.x, -norm.z));
    // Commit the refined ground point.
    self->field514 = 0.08f * norm.x + hit.x;
    self->field518 = self->field3AC;
    self->field51C = 0.08f * norm.z + hit.z;
    return true;
}

// Obstacle/facing update: adjusts the move vector against the linked
// region's probe results and refines it via the two scan loops, then runs
// the ground-follow / fallback logic. Returns a bool-ish status (r31).
int cf::CActParamAnimGame::steerForObsts(ml::CVec3* pos, ml::CVec3* move) {
    ml::CVec3 moveV(*move);
    ml::CVec3 v134(0.0f, 0.0f, 0.0f);
    int groundOk = 0;

    if (field4E8 != 0) {
        if (field4EC & 0x8) {
            if (move->x != 0.0f || move->y != 0.0f) {
                ml::CVec3 v128(*pos);
                ml::CVec3 v11c(*move);
                ml::CVec3 v110(0.0f, 0.0f, 0.0f);
                v128.y = field510;
                v11c.y = 0.0f;
                u16 saved = field4E8->f6BE;
                field4E8->f6BE = 0x1f4;
                ColiResolveNodeMove(&field4E8->f60C, &v128, &v11c, &v110);
                field4E8->f6BE = saved;
                if (v110.x != 0.0f || v110.z != 0.0f) {
                    v128.x += v110.x;
                    v128.z += v110.z;
                    v128.x -= pos->x;
                    v128.z -= pos->z;
                    f32 len2 = v128.x * v128.x + v128.z * v128.z;
                    if (len2 != 0.0f) {
                        f32 len2b = move->x * move->x + move->z * move->z;
                        if (len2b != 0.0f) {
                            if (!(len2 >= 0.0f)) {
                                Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273, lbl_eu_80526300);
                            }
                            f32 len = len2 <= 0.0f ? 0.0f : len2 * nw4r::math::FrSqrt(len2);
                            f32 inv = 1.0f / len;
                            f32 nx = v128.x * inv;
                            f32 nz = v128.z * inv;
                            if (!(len2b >= 0.0f)) {
                                Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273, lbl_eu_80526300);
                            }
                            f32 lenb = len2b <= 0.0f ? 0.0f : len2b * nw4r::math::FrSqrt(len2b);
                            f32 invb = 1.0f / lenb;
                            f32 dot = nz * (move->z * invb) + nx * (move->x * invb);
                            if (dot > 0.0f) {
                                move->x = move->x * dot;
                                move->z = move->z * dot;
                            } else {
                                move->x = 0.0f;
                                move->z = 0.0f;
                            }
                        }
                    }
                }
            }
        }
        if (ColiResolveNodeMove(&field4E8->f60C, &v134, pos, move)) {
            if (v134.y > 0.01f) {
                field47C = pos->y + v134.y;
                groundOk = 1;
            }
        } else {
            if (v134.y > 0.01f) {
                if (field3C4 < 0.0f && field3C4 <= -15.0f) {
                    setAnimHeight(this, field3C4 * 0.95f);
                }
            }
        }
        if (v134.x != 0.0f || v134.y != 0.0f || v134.z != 0.0f) {
            field4EC |= 0x20000000;
        }
        moveV += v134;
    }

    if (field4EC & 0x28) {
        ml::CVec3 v38 = moveV * 1.5f;
        ml::CVec3 v8c = v38;
        ml::CVec3 v2c = *pos + v8c;
        ml::CVec3 v98 = v2c;
        ml::CVec3 v80(0.0f, 2.5f, 0.0f);
        ml::CVec3 v20 = v98 + v80;
        ml::CVec3 va4 = v20;
        if (ScnRes_VertRayForward_E398((float*)&va4, field4A8 & ~0x2000, 0, 1, -5.0f, lbl_eu_8066AF20)) {
            if (field4EC & 0x8) {
                f32 f29v = -1e+06f;
                int topIdx = -1;
                f32 f31v = 0.0f;
                int idx;
                for (idx = 0; idx < ScnRes_GetEntryCount_E4AC(); idx++) {
                    if (((CActParamAnimGameBeObj*)ScnRes_GetEntryHead2_E520(idx))->y >= f31v) {
                        if (f29v < ((CActParamAnimGameBeObj*)ScnRes_GetEntryPtr_E50C(idx))->y) {
                            f29v = ((CActParamAnimGameBeObj*)ScnRes_GetEntryPtr_E50C(idx))->y;
                            topIdx = idx;
                        }
                    }
                }
                if (topIdx >= 0) {
                    if (ScnRes_EntryFlagThunk_E5A4(0x40000, topIdx)) {
                        f32 f30v = f29v - 100.0f;
                        int alt;
                        for (alt = 0; alt < ScnRes_GetEntryCount_E4AC(); alt++) {
                            if (topIdx != alt) {
                                f32 g = ((CActParamAnimGameBeObj*)ScnRes_GetEntryPtr_E50C(alt))->y;
                                if (f29v > g && f30v < g) {
                                    f30v = ((CActParamAnimGameBeObj*)ScnRes_GetEntryPtr_E50C(alt))->y;
                                }
                            }
                        }
                        if (f29v - f30v > field508) {
                            moveV = ml::CVec3(0.0f, 0.0f, 0.0f);
                        }
                    }
                }
            } else {
                if (field4EC & 0x20) {
                    if (v134.x != 0.0f || v134.z != 0.0f) {
                        moveV = zero__Q22ml5CVec3;
                    }
                }
            }
        }
    }

    if (field4EC & 0x8) {
        ml::CVec3 v104(*pos);
        ml::CVec3 vF8(moveV.x, 0.0f, moveV.z);
        v104.y = field510;
        ((BD94CFn)ScnRes_ResolveGroundHeight)(&v104, &vF8, field4A8, 0, (field0C & 0x10000) != 0, 1,  // DEBT(callcast): C symbol; this TU's retail calls omit the trailing FP args of the canonical 11-arg decl
                                 field494, field490, -1.0f, lbl_eu_8066AF20, field484);
        field510 = v104.y;
        v104.x -= pos->x;
        v104.z -= pos->z;
        f32 len2 = v104.x * v104.x + v104.z * v104.z;
        if (len2 != 0.0f) {
            f32 len2b = moveV.x * moveV.x + moveV.z * moveV.z;
            if (len2b != 0.0f) {
                if (!(len2 >= 0.0f)) {
                    Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273, lbl_eu_80526300);
                }
                f32 len = len2 <= 0.0f ? 0.0f : len2 * nw4r::math::FrSqrt(len2);
                f32 inv = 1.0f / len;
                f32 nx = v104.x * inv;
                f32 nz = v104.z * inv;
                if (!(len2b >= 0.0f)) {
                    Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273, lbl_eu_80526300);
                }
                f32 lenb = len2b <= 0.0f ? 0.0f : len2b * nw4r::math::FrSqrt(len2b);
                f32 invb = 1.0f / lenb;
                f32 dot = nz * (moveV.z * invb) + nx * (moveV.x * invb);
                if (dot > 0.0f) {
                    moveV.x = moveV.x * dot;
                    moveV.z = moveV.z * dot;
                } else {
                    moveV.x = 0.0f;
                    moveV.z = 0.0f;
                }
            }
        } else {
            moveV.x = 0.0f;
            moveV.z = 0.0f;
        }
    }

    if (getQueuedFileEventCount__Q22cf13CfGameManagerFv() == 0x31) {
        if (field4E8 != 0 && (field4E8->f64 & 4)) {
            if (PSVECMag((const Vec*)&moveV) > 0.2f) {
                f32 len2 = moveV.x * moveV.x + moveV.y * moveV.y + moveV.z * moveV.z;
                if (len2 == 0.0f) {
                    moveV = zero__Q22ml5CVec3;
                } else {
                    PSVECNormalize((const Vec*)&moveV, (Vec*)&moveV);
                }
                moveV *= 0.2f;
            }
        }
    }

    if (field4EC & 0x1000) {
        field488 = pos->y;
        groundOk = 1;
        field4EC &= ~0x1000;
        ml::CVec3 zz(0.0f, 0.0f, 0.0f);
        setAnimPosVec(this, &zz);
        goto l60418;
    }

    if (mData.mA260 & 0x20) {
        f32 f31v = pos->y + moveV.y;
        f32 f29v;
        if (field3DC <= 0.9f) {
            f29v = -0.1f;
        } else if (field3DC <= 0.8f) {
            f29v = -0.15f;
        } else {
            f29v = -0.25f;
        }
        if (v134.y > 0.01f) f29v = 0.0f;
        if (field4EC & 0x20000000) {
            moveV.x = 0.0f;
            moveV.z = 0.0f;
        }
        if (field530 & 0x400) {
            ScnRes_XCrossCbThunk_E628(1);
            groundOk = (groundOk | ((BD94CFn)ScnRes_ResolveGroundHeight)(pos, &moveV, field4A8, 0,  // DEBT(callcast): C symbol; this TU's retail calls omit the trailing FP args of the canonical 11-arg decl
                        (field0C & 0x10000) != 0, 1,
                        field494, field490, f29v, lbl_eu_8066AF20, field484)) != 0;
            ScnRes_XCrossCbThunk_E628(0);
        } else {
            groundOk = (groundOk | ((BD94CFn)ScnRes_ResolveGroundHeight)(pos, &moveV, field4A8, 0,  // DEBT(callcast): C symbol; this TU's retail calls omit the trailing FP args of the canonical 11-arg decl
                        (field0C & 0x10000) != 0, 1,
                        field494, field490, f29v, lbl_eu_8066AF20, field484)) != 0;
        }
        if (ScnRes_GetCrossFlagA_E5B8()) {
            field4EC |= 0x2000000;
            ml::CVec3 v68 = vec3C0() * 0.99f;
            setAnimPosVec(this, &v68);
            if (0.98f + field464 < pos->y && 0.02f + f31v < pos->y) {
                if (move->x != 0.0f || move->z != 0.0f) {
                    ml::CVec3 vEC(move->x, 0.0f, move->z);
                    f32 clen2 = vEC.x * vEC.x + vEC.y * vEC.y + vEC.z * vEC.z;
                    if (clen2 == 0.0f) {
                        vEC = zero__Q22ml5CVec3;
                    } else {
                        PSVECNormalize((const Vec*)&vEC, (Vec*)&vEC);
                    }
                    field0C |= 0x1000;
                    *(u32*)&field3FC = *(u32*)&vEC.x;
                    *(u32*)&field400 = *(u32*)&vEC.y;
                    *(u32*)&field404 = *(u32*)&vEC.z;
                }
            }
        }
        if (moveV.y < 0.0f && pos->y - f31v <= 0.1f && field3C4 <= -15.0f) {
            setAnimHeight(this, field3C4 * 0.95f);
        }
        if (pos->y - field464 <= 1.5f && field3C4 > 0.0f && !(field4EC & 0x1000000)) {
            groundOk = 0;
        }
        field534 = 0;
        if (field4EC & 2) groundOk = 1;
        goto l60418;
    }

    {
        f32 f29v = pos->y + moveV.y;
        f32 f30v;
        if (field4EC & 2) {
            f30v = 0.0f;
            groundOk = 1;
        } else if (field0C & 0x10000) {
            f30v = 0.0f;
        } else {
            f32 len2 = move->x * move->x + move->z * move->z;
            if (len2 < 0.1024f) {
                f30v = -0.3f;
            } else {
                if (!(len2 >= 0.0f)) {
                    Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273, lbl_eu_80526300);
                }
                f32 len = len2 <= 0.0f ? 0.0f : len2 * nw4r::math::FrSqrt(len2);
                f30v = -len;
                if (field0C & 0x200000) f30v *= 2.0f;
            }
        }
        if (v134.y > 0.01f) f30v = 0.0f;
        ml::CVec3 vE0 = *pos;
        if (field530 & 0x400) {
            ScnRes_XCrossCbThunk_E628(1);
            groundOk = (groundOk | ((BD94CFn)ScnRes_ResolveGroundHeight)(pos, &moveV, field4A8, 0,  // DEBT(callcast): C symbol; this TU's retail calls omit the trailing FP args of the canonical 11-arg decl
                        (field0C & 0x10000) != 0, 1,
                        field494, field490, f30v, lbl_eu_8066AF20, field484)) != 0;
            ScnRes_XCrossCbThunk_E628(0);
        } else {
            groundOk = (groundOk | ((BD94CFn)ScnRes_ResolveGroundHeight)(pos, &moveV, field4A8, 0,  // DEBT(callcast): C symbol; this TU's retail calls omit the trailing FP args of the canonical 11-arg decl
                        (field0C & 0x10000) != 0, 1,
                        field494, field490, f30v, lbl_eu_8066AF20, field484)) != 0;
        }
        if (groundOk != 0) {
            field4EC |= 0x8000;
        } else {
            field4EC &= ~0x8000;
        }
        if (ScnRes_GetCrossFlagA_E5B8()) {
            field4EC |= 0x200000;
        } else {
            field4EC &= ~0x200000;
        }
        if (moveV.y < 0.0f && pos->y - f29v <= 0.1f && field3C4 <= -15.0f) {
            setAnimHeight(this, field3C4 * 0.95f);
        }
        if (groundOk == 0) {
            if (field4EC & 0x40000) {
                if (field4EC & 0x1000) {
                    if (field4EC & 0x800) {
                        if (v134.x == 0.0f && v134.z == 0.0f) {
                            if (vE0.y + moveV.y < pos->y) {
                                field534 = 0;
                                groundOk = 1;
                            } else {
                                s16 a = (s16)field534;
                                s16 b = (s16)field536;
                                field534 = (u16)(a + 1);
                                if (a > b) {
                                    field534 = (u16)(b + 1);
                                } else {
                                    groundOk = 1;
                                }
                            }
                        } else {
                            *pos = vE0;
                            field534 = 0;
                            field4EC |= 0x400;
                            groundOk = 1;
                        }
                    } else {
                        s16 a = (s16)field534;
                        s16 b = (s16)field536;
                        field534 = (u16)(a + 1);
                        if (a > b) {
                            field534 = (u16)(b + 1);
                        } else {
                            groundOk = 1;
                        }
                    }
                } else {
                    field534 = 0;
                }
            } else {
                field534 = 0;
            }
        } else {
            field534 = 0;
        }
        if (field3C4 > 0.0f) groundOk = 0;
    }

l60418:
    if (ScnRes_GetCrossFlagB_E5C0()) {
        field4EC |= 0x80000000;
    } else {
        field4EC &= ~0x80000000;
    }
    if (groundOk == 0) {
        if (!(field0C & 0x10000)) {
            if (field4EC & 0x40000) {
                field4EC = (field4EC & 0xFFF5FFFF) | 0x40000;
            }
            if (field488 < pos->y || field3B8 - field3AC > 5.0f) {
                field488 = pos->y;
            } else {
                f32 d = field488 - pos->y;
                if (d > 10.0f || (field0C & 0x80)) {
                } else {
                    field4EC |= 0x100000;
                    if (field488 - pos->y <= 10.0f) {
                        int n = (int)(field488 - pos->y);
                        int q = n / 10;
                        int res = q * 20;
                        if (res >= 100) res = 100;
                        field532 = (u16)res;
                    }
                }
            }
        }
    } else {
        if (field4EC & 0x40000) {
            if (!(field0C & 0x80) && field488 - pos->y <= 10.0f) {
                if (field4EC & 2) {
                    if ((field530 & 0x400) && field488 - pos->y <= 200.0f) {
                        if (getEventValue40__Q22cf13CfGameManagerFv(0x83) == 0) {
                            setEventManagerValue__Q22cf13CfGameManagerFv(0x83, 1);
                            queueEventId__Q22cf13CfGameManagerFv(0x83);
                        }
                    }
                } else {
                    int n = (int)(field488 - pos->y);
                    int q = n / 10;
                    int res = q * 20;
                    if (res >= 100) res = 100;
                    field532 = (u16)res;
                    field538 = 0;
                    field4EC |= 0x90000;
                }
            }
            field4EC &= ~0x40000;
        } else if (field4EC & 0x80000) {
            if (this->getAttachedAnimScale() > 0.0f) {
                s16 c = (s16)field538;
                field538 = (u16)(c + 1);
                if (c > 6) field4EC &= ~0x80000;
            }
        }
    }
    field488 = pos->y;
    field4EC |= 0x20000;
    if (field47C <= pos->y) field47C = pos->y;
    if (field4EC & 2) {
        field4EC &= 0xFFE2FFFF;
    }
    if (field4EC & 0x4000) {
        if (object3A0 != 0) {
            void* sub = ((cf::CActParamAnimGameObj3A0*)object3A0)->sub7EC;
            if (sub != 0) {
                ml::CVec3 v14 = *pos + ml::CVec3(0.0f, 1.0f, 0.0f);
                ml::CVec3 vBC = v14;
                ml::CVec3 v8 = *pos + ml::CVec3(0.0f, -3.0f, 0.0f);
                ml::CVec3 vB0 = v8;
                ml::CVec3 vD4;
                ml::CVec3 vC8;
if (Coli_WalkReport(sEneColiProbe, &vD4, &vC8, &vBC, &vB0)) {
                    if (vD4.y > field3E8 - 0.05f) {
                        ml::CVec3 v44 = vD4;
                        ((CActParam7ECTarget*)sub)->func14(&v44);
                        ((CActParam7ECTarget*)sub)->func18(1);
                    }
                }
            }
        }
    }
    return groundOk;
}


// Actor-facing position/heading update: reads the +0x4E8 link's current
// position (CObjectParam_getSelfObjectId id gate, then findObjectById),
// diffs the action source's CfObject_getPosVector position vector against
// this object's position, and when the delta is non-trivial, orients the
// link toward the actor (CfObject_setMoveHeadAngle) with the heading from
// Atan2FIdx.
void cf::CActParamAnimGame::aimAtActorPos() {
    CActParamAnimGame* v = (CActParamAnimGame*)this;
    // Retail reloads the +0x4E8 link for every dispatch; keep the loads
    // independent so MWCC colors them like retail.
    if (v->field4E8 == 0) return;
    if (((cf::CfObject*)v->field4E8)->CObjectParam_getSelfObjectId() == 0) return;
    void* src = findObjectById((int)((cf::CfObject*)v->field4E8)->CObjectParam_getSelfObjectId());
    if (src == 0) return;
    ml::CVec3* p = ((cf::CfObject*)src)->CfObject_getPosVector();
    ml::CVec3 d;
    nw4r::math::VEC3Sub(reinterpret_cast<nw4r::math::VEC3*>(&d),
                         reinterpret_cast<const nw4r::math::VEC3*>(p),
                         reinterpret_cast<const nw4r::math::VEC3*>(&v->field3A8));
    f32 dx = d.x;
    f32 dy = d.y;
    f32 dz = d.z;
    bool b1 = nw4r::math::FAbs(dx) <= ml::epsilon;
    bool b2 = nw4r::math::FAbs(dy) <= ml::epsilon;
    bool nearXY = b1 && b2;
    bool b3 = nw4r::math::FAbs(dz) <= ml::epsilon;
    bool nearAll = nearXY && b3;
    if (nearAll) return;
    v->field0C |= 0x1000000;
    ((cf::CfObject*)v->field4E8)->CfObject_setMoveHeadAngle(0.024543693f * nw4r::math::Atan2FIdx(dx, dz));
}

// Forwards the +0x444 scalar to the +0xC4 virtual slot of the object at
// +0x4E8 (tail call; retail keeps no stack frame). Real virtual dispatch
// (CfObject_setMoveHeadAngle) so MWCC emits the r12 vtable load.
void cf::CActParamAnimGame::syncYawToLink() {
    cf::CActParamAnimGame* self = reinterpret_cast<cf::CActParamAnimGame*>(this);
    cf::CfObject* region = (cf::CfObject*)self->field4E8;
    if (region == 0) return;
    f32 value = self->field444;
    region->CfObject_setMoveHeadAngle(value);
}

extern "C" bool pushYawToLink(::CActParamData* data) {
    int v0 = data->mA270;
    bool f = false;
    if (((v0 & 0x800000) != 0) || ((data->mField2F0 & 0x800000) != 0))
        f = true;
    return f;
}
