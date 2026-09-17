// Auto-scaffolded catalog TU for kyoshin/cf/CfGimmick
// Replace stubs with high-level C/C++ during decomp.

#include <cstdio>
#include <cstring>

#include "kyoshin/cf/CfGimmick.hpp"
#include "kyoshin/CTaskGame.hpp"   // CTaskGameCamView
#include "libs/monolib/src/scn/CScn_8049603C.hpp" // Scn_QueryUnk80State (single owner decl)
#include "kyoshin/cf/object/CfObjectMove.hpp"
#include "kyoshin/cf/object/CActorParam.hpp"
#include <nw4r/math.h>
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

// typed retail data (replaces old byte blobs): declared up top so MWCC emits
// them before its own code-literal pool (else the int->double magics land at
// the sdata2 head and shift the pool). Order within each section matches retail.
__declspec(section ".rodata") __attribute__((used)) const char lbl_eu_80508598[0x0C] = "A_area_x";
__declspec(section ".rodata") __attribute__((used)) const char lbl_eu_805085A4[0x0C] = "A_area_py";
__declspec(section ".rodata") __attribute__((used)) const char lbl_eu_805085B0[0x0C] = "A_area_my";
__declspec(section ".rodata") __attribute__((used)) const char lbl_eu_805085BC[0x0C] = "A_area_z";
__declspec(section ".rodata") __attribute__((used)) const char lbl_eu_805085C8[0x0C] = "A_area_rx";
__declspec(section ".rodata") __attribute__((used)) const char lbl_eu_805085D4[0x0C] = "A_area_ry";
__declspec(section ".rodata") __attribute__((used)) const char lbl_eu_805085E0[0x0C] = "A_area_rz";
__declspec(section ".rodata") __attribute__((used)) const char lbl_eu_805085EC[0x0C] = "A_rectype";
__declspec(section ".rodata") __attribute__((used)) const char lbl_eu_805085F8[0x0C] = "S_FLG_MIN";
__declspec(section ".rodata") __attribute__((used)) const char lbl_eu_80508604[0x0C] = "S_FLG_MAX";
__declspec(section ".rodata") __attribute__((used)) const char lbl_eu_80508610[0x0C] = "quest_FLG";
__declspec(section ".rodata") __attribute__((used)) const char lbl_eu_8050861C[0x0C] = "quest_STFLG";
__declspec(section ".rodata") __attribute__((used)) const char lbl_eu_80508628[0x0C] = "MESS ERR";
__declspec(section ".rodata") __attribute__((used)) const char lbl_eu_80508634[0x34] = "cl_area_x\0cl_area_py\0cl_area_my\0cl_area_z\0%s";
__declspec(section ".sdata") __attribute__((used, aligned(4))) struct Sdata_CfGimmick {
    float f;            // 80662784 1.0 fill (written by CfGimmick_InitPartyGlobals)
    const void* t[2];   // 80662788 fallback message table
} sdata_CfGimmick = { 1.0f, { lbl_eu_80508628, nullptr } };
#define lbl_eu_80662784 sdata_CfGimmick.f
#define lbl_eu_80662788 sdata_CfGimmick.t
__declspec(section ".sdata2") __attribute__((used)) const char lbl_eu_80668320[0x08] = "A_posX";
__declspec(section ".sdata2") __attribute__((used)) const char lbl_eu_80668328[0x08] = "A_posY";
__declspec(section ".sdata2") __attribute__((used)) const char lbl_eu_80668330[0x08] = "A_posZ";
__declspec(section ".sdata2") __attribute__((used)) const char lbl_eu_80668338[0x08] = "gimID";
__declspec(section ".sdata2") __attribute__((used)) const char lbl_eu_80668340[0x08] = "MSGID";
__declspec(section ".sdata2") __attribute__((used)) const char lbl_eu_80668348[0x08] = "name1";
// Individual SDA number labels (retail addressing): each constant is its own
// .sdata2 symbol so MWCC emits SDA21 loads through the small-data base.
// The previous single-struct pool forced absolute addressing (li + lfs via a
// dedicated GPR), which grew every float user's frame by one saved register
// (CfGimmick_ApplyPartyMoveSpeed/A1DC: 0x30 -> 0x40) and drifted reloc names. Order and
// addresses match retail: 68350..68364 floats, 68368/68370 doubles,
// 68378 float pair. Total 0x30 bytes, same as the struct.
extern "C" {
__declspec(section ".sdata2") __attribute__((used)) const float lbl_eu_80668350 = 0.0f;
__declspec(section ".sdata2") __attribute__((used)) const f32 lbl_eu_80668354 = 40.743664f;
__declspec(section ".sdata2") __attribute__((used)) const float lbl_eu_80668358 = 1.0f;
__declspec(section ".sdata2") __attribute__((used)) const float lbl_eu_8066835C = 30.0f;
__declspec(section ".sdata2") __attribute__((used)) const f32 lbl_eu_80668360 = 30000.0f;
__declspec(section ".sdata2") __attribute__((used)) const float lbl_eu_80668364 = 0.01f;
// Tail doubles stay in a struct: as individual labels MWCC literal-dedups
// the 2^52 against the code-gen int->float pool, drops the slot and pulls
// the 68378 pair up to 0x50 (data regression). The aggregate forces the
// retail offsets 0x48/0x50 with the retail values. Neither double is
// referenced by name (code uses pool magics), so no frame impact.
struct Sdata2Tail_CfGimmick {
    double d0;   // 80668368 4503601774854144.0 (signed int->float magic)
    double d1;   // 80668370 4503599627370496.0 (2^52, unsigned magic)
};
__declspec(section ".sdata2") __attribute__((used, aligned(8)))
const Sdata2Tail_CfGimmick sdata2tail_CfGimmick = {
    4503601774854144.0, 4503599627370496.0
};
__declspec(section ".sdata2") __attribute__((used)) const f32 lbl_eu_80668378[2] = {1.5f, 0.0f};
}

// typed .bss/.sbss zero-fill (replaces old nobits blobs): sizes must total
// retail .bss 0xB8 / .sbss 0xC; nobits compare sizes only.
extern "C" {
CfGimmickVec3 lbl_eu_805765A0;
int lbl_eu_805765B0[10];
char lbl_eu_805765D8[128];
f32 lbl_eu_806646B0;
u32 lbl_eu_806646B4;
u32 lbl_eu_806646B8;
}

// (No TU-local vtable pad: the field_78 actor dispatch goes through the
// CfObject view at the CfGimmick_PushRefreshValue definition below.)


struct CfGimmickVec3;

namespace cf {
// Minimal view of cf::CfGameManager for this TU (CfGameManager.hpp is not
// included - its isTimerActive is declared non-static, but retail callers
// (CfGimmick_UpdatePartyAnchorState) invoke the Fv symbol with a dead r3, i.e. a static call).
// Static members here emit the same pre-mangled retail symbols without a
// `this` load.  getPlayer() coexists with CfObjectMove.hpp's extern "C"
// declaration of the same symbol (different scopes - no overload).
class CfGameManager {
public:
    static CfObjectMove* getPlayer(int playerIndex);
    static bool isTimerActive();
    u8 pad[0xB0];               // 0x00..0xAF
    UnkClass_800821F8* unkB0;   // 0xB0 - cleared by CfGimmick_ClearManagerBinding
    void clearPlayerEffect();
};
} // namespace cf

// Builds/inverts the gimmick collider placement matrix (see definition below).
void func_802089BC(cf::CfGimmick* self, const f32* basis, const CfGimmickVec3* point);

using namespace cf;

namespace cf {
    void CfGimmick::CfGimmick_DetachManager(CfGameManager* other) {
        CfGameManager* cur = field_78;
        if (cur != other) return;
        cur->unkB0 = 0;
        field_78 = 0;
    }

    void CfGimmick::CfGimmick_UpdateColliderMatrix() {
        func_802089BC((CfGimmick*)((char*)this + 0x1c), (const f32*)((char*)this + 4),
                      (const CfGimmickVec3*)((char*)this + 0x10));
    }

    int CfGimmick::CfGimmick_CheckerKind0Always() { return 1; }
}

// Constructor.  The retail symbol __ct__cf_CfGimmick is a C-linkage name (no
// C++ mangling markers), so it must be emitted as an extern "C" free function
// rather than a C++ member constructor.  Subclass constructors (CfGimmickElv,
// CfGimmickWarp, ...) call this to init the shared CfGimmick base fields.
extern "C" void __ct__cf_CfGimmick(CfGimmick* self) {
    self->vtable = (u32*)lbl_eu_80535844;
    self->field_64 = 0;
    self->field_80 = 0;
    self->field_66 = 0;
    self->field_68 = 0;
    self->field_6A = 0;
    self->field_6C = 0;
    self->field_6E = 0;
    self->field_78 = 0;
    self->field_7C = 0;
    self->field_73 = 0;
    self->field_72 = 0;
    self->field_71 = 0;
    self->field_70 = 0;
    self->field_74 = 0;
}

cf::CfGimmick::~CfGimmick() {}

// Build a rotation/placement matrix for the gimmick's collider box from its
// axis-extents (field_30/34/38/3C) and the requested point direction, then
// invert it in place.  field_44 records which placement shape was selected.
// Retail never CSEs the float-pool constants: every test reloads lbl_eu_80668350
// via lfs, so the source references the globals directly (no zero/one locals).
void func_802089BC(CfGimmick* self, const f32* basis, const CfGimmickVec3* point) {
    f32* m = ((nw4r::math::MTX34*)self)->m[0];

    if (self->field_38 > lbl_eu_80668350)
        self->field_38 = -self->field_38;

    if (lbl_eu_80668350 == self->field_30 && lbl_eu_80668350 == self->field_34 &&
        lbl_eu_80668350 == self->field_3C) {
        self->field_44 = 0;
        return;
    }

    if (self->field_3C != lbl_eu_80668350) {
        // With depth: full placement.  Extent/clamp/squared-length run before
        // the direction test.
        CfGimmickVec3 v;
        v.x = self->field_30;
        v.y = self->field_34;
        v.z = self->field_3C;
        if (v.y < -self->field_38)
            v.y = -self->field_38;
        // PS-paired kernel reproduces retail's psq_l/ps_mul/ps_madd/ps_sum0.
        self->field_40 = VEC3LenSq((const nw4r::math::VEC3*)&v);
        if (point->x != lbl_eu_80668350 || point->y != lbl_eu_80668350 ||
            point->z != lbl_eu_80668350) {
            self->field_44 = 4;
            nw4r::math::MTX34RotXYZFIdx((nw4r::math::MTX34*)self, lbl_eu_80668354 * point->x,
                                        lbl_eu_80668354 * point->y, lbl_eu_80668354 * point->z);
            m[3] = basis[0];
            m[7] = basis[1];
            m[11] = basis[2];
        } else {
            self->field_44 = 3;
            f32 one = lbl_eu_80668358;
            m[0] = one; m[1] = lbl_eu_80668350; m[2] = lbl_eu_80668350; m[3] = basis[0];
            m[4] = lbl_eu_80668350; m[5] = one; m[6] = lbl_eu_80668350; m[7] = basis[1];
            m[8] = lbl_eu_80668350; m[9] = lbl_eu_80668350; m[10] = one; m[11] = basis[2];
        }
    } else {
        // No depth: horizontal-only placement.  Direction test comes first;
        // the extent/squared-length math only runs on the rotated path.
        if (point->x != lbl_eu_80668350 || point->z != lbl_eu_80668350) {
            CfGimmickVec3 v;
            v.x = self->field_30;
            v.y = self->field_34;
            v.z = self->field_3C;
            if (v.y < -self->field_38)
                v.y = -self->field_38;
            self->field_40 = VEC3LenSq((const nw4r::math::VEC3*)&v);
            self->field_44 = 2;
            nw4r::math::MTX34RotXYZFIdx((nw4r::math::MTX34*)self, lbl_eu_80668354 * point->x,
                                        lbl_eu_80668354 * point->y, lbl_eu_80668354 * point->z);
            m[3] = basis[0];
            m[7] = basis[1];
            m[11] = basis[2];
        } else {
            self->field_44 = 1;
            f32 one = lbl_eu_80668358;
            m[0] = one; m[1] = lbl_eu_80668350; m[2] = lbl_eu_80668350; m[3] = basis[0];
            m[4] = lbl_eu_80668350; m[5] = one; m[6] = lbl_eu_80668350; m[7] = basis[1];
            m[8] = lbl_eu_80668350; m[9] = lbl_eu_80668350; m[10] = one; m[11] = basis[2];
        }
    }

    // Invert the rotation/translation matrix in place (retail PSMTXInverse).
    PSMTXInverse((const f32 (*)[4])self, (f32 (*)[4])self);
}

// Sound-id helpers: first arg is a u16/u32 effect id (Elv val1B6 / Item field_8E),
// second is a position pointer passed through as an integer to func_801BFDE8.
void CfGimmick_PlaySoundAtPos(u32 id, const CfGimmickVec3* pos) {
    func_801BFDE8(1, id, (u32)pos,
                  lbl_eu_80668358, lbl_eu_8066835C);
}

void CfGimmick_PlaySoundAtPosScaled(u32 id, const CfGimmickVec3* pos, float second) {
    func_801BFDE8(1, id, (u32)pos,
                  lbl_eu_80668358, second);
}

void CfGimmick_StopManagedSound(cf::CfGimmick* self) {
    if (self->field_80 != 0) {
        func_801BFED0(1, self->field_80, 0xa);
        self->field_80 = 0;
    }
}

// Party-feature state update: re-anchor the gimmick reference point to the
// player's target-map position (or clear it when no player is loaded), and
// set the party/kicking flag bits.  Called on party joins/leaves.
void CfGimmick_UpdatePartyAnchorState(u32 partyId, s32 flagA, s32 flagB) {
    lbl_eu_806646BC = 0;

    // Ground-height gate: while the stage base height is above zero, record
    // the downward drop value and flag it (bit 0x8).  The scene-camera height
    // query is re-run inside the block (retail calls Scn_QueryUnk80State twice).
    f32 rem = lbl_eu_80668358 - ((CTaskGameCamView*)Scn_QueryUnk80State(lbl_eu_80663E14))->field_C;
    if (rem < lbl_eu_80668358) {
        lbl_eu_806646BC |= 0x8;
        lbl_eu_80662780 = lbl_eu_80668358 - ((CTaskGameCamView*)Scn_QueryUnk80State(lbl_eu_80663E14))->field_C;
    }

    if (cf::CfGameManager::getPlayer(0) != 0) {
        cf::CfObject* player = (cf::CfObject*)cf::CfGameManager::getPlayer(0);
        // Integer-copy the target position (retail lwz/stw, no f32 path):
        // loads are hoisted x,y then stored y,x, then z - mirror with temps.
        CfGimmickVec3u* target = (CfGimmickVec3u*)player->CfObject_getPosVector();
        u32 tx = target->x;
        u32 ty = target->y;
        CfGimmickVec3u& dst = (CfGimmickVec3u&)lbl_eu_805765A0;
        dst.y = ty;
        dst.x = tx;
        dst.z = target->z;
        lbl_eu_806646B0 = player->CfObject_getMoveHeadAngle();
        lbl_eu_806646B4 = partyId;
        if (flagA) lbl_eu_806646BC |= 0x20;
        if (flagB) lbl_eu_806646BC |= 0x40;
        if (((cf::CfObjectMove*)player)->mTargetC4 != 0 && player->CfObject_isMoveActiveNow() && player->CfObject_checkSubReady() &&
            (lbl_eu_80663E24 & 0x80) != 0) {
            lbl_eu_806646BC |= 0x2;
        }
    } else {
        lbl_eu_805765A0.x = lbl_eu_80668360;
        lbl_eu_805765A0.y = lbl_eu_80668360;
        lbl_eu_805765A0.z = lbl_eu_80668360;
        lbl_eu_806646B0 = lbl_eu_80668350;
        lbl_eu_806646B4 = 0;
    }

    // Presentation-event gate (bit 0x22040000) and the CF-mode gate; both
    // guard extra gimmick-party flags.
    if (lbl_eu_80663E24 & 0x22040000u)
        lbl_eu_806646BC |= 0x4;
    if (!cf::CfGameManager::isTimerActive())
        lbl_eu_806646BC |= 0x1;
}

void CfGimmick_InitPartyGlobals() {
    lbl_eu_80662784 = lbl_eu_80668358;
    lbl_eu_806646C0 = 0;
    for (int i = 0; i < 10; i++) lbl_eu_805765B0[i] = -1;
}

// retail: stw r3, lbl_eu_806646B8; blr - store arg to global
void CfGimmick_SetGlobalB8Value(u32 value) { lbl_eu_806646B8 = value; }

void CfGimmick_ClearManagerBinding(cf::CfGimmick* self) {
    if (self->field_78) {
        self->field_78->unkB0 = 0;
        self->field_78->clearPlayerEffect();
        self->field_78 = 0;
    }
}

void CfGimmick_LoadBdatAreaPos(CfGimmick* self, float* out, void* unused, void** holder) {
    // Read three adjacent bdat columns starting at each +2 prefix, converting
    // each integer cell to a scaled float written into the output vector.
    out[0] = lbl_eu_80668364 * (f32)(s32)getBdatStringColumnValue(
        *holder, *(char**)(lbl_eu_805357E8 + 0x00) + 2, self->field_64);
    out[1] = lbl_eu_80668364 * (f32)(s32)getBdatStringColumnValue(
        *holder, *(char**)(lbl_eu_805357E8 + 0x04) + 2, self->field_64);
    out[2] = lbl_eu_80668364 * (f32)(s32)getBdatStringColumnValue(
        *holder, *(char**)(lbl_eu_805357E8 + 0x08) + 2, self->field_64);
}

void CfGimmick_LoadBdatAreaExtents(CfGimmick* self, CfGimmick* out, void* unused, void** holder) {
    u32 rawA = getBdatStringColumnValue(
        *holder, *(char**)(lbl_eu_805357E8 + 0x0C) + 2, self->field_64);
    out->field_30 = lbl_eu_80668364 * (f32)*(const u16*)&rawA;
    u32 rawB = getBdatStringColumnValue(
        *holder, *(char**)(lbl_eu_805357E8 + 0x10) + 2, self->field_64);
    out->field_34 = lbl_eu_80668364 * (f32)*(const u16*)&rawB;
    u32 rawC = getBdatStringColumnValue(
        *holder, *(char**)(lbl_eu_805357E8 + 0x14) + 2, self->field_64);
    out->field_38 = lbl_eu_80668364 * (f32)*(const u16*)&rawC;
    u32 rawD = getBdatStringColumnValue(
        *holder, *(char**)(lbl_eu_805357E8 + 0x18) + 2, self->field_64);
    out->field_3C = lbl_eu_80668364 * (f32)*(const u16*)&rawD;
}

void CfGimmick_LoadBdatClAreaExtents(CfGimmick* self, CfGimmick* out, void* unused, void** holder) {
    // Retail truncates each column cell to u16 through its stack home
    // (stw + lhz, not rlwinm); writing the truncation as a deref of the
    // call result's storage reproduces the memory-load idiom.  The f32 cast
    // then uses MWCC's 2^52 double-trick (lfd + fsubs).
    u32 rawA = getBdatStringColumnValue(*holder, lbl_eu_80508634 + 0x00, self->field_64);
    out->field_30 = lbl_eu_80668364 * (f32)*(const u16*)&rawA;
    u32 rawB = getBdatStringColumnValue(*holder, lbl_eu_80508634 + 0x0A, self->field_64);
    out->field_34 = lbl_eu_80668364 * (f32)*(const u16*)&rawB;
    u32 rawC = getBdatStringColumnValue(*holder, lbl_eu_80508634 + 0x15, self->field_64);
    out->field_38 = lbl_eu_80668364 * (f32)*(const u16*)&rawC;
    u32 rawD = getBdatStringColumnValue(*holder, lbl_eu_80508634 + 0x20, self->field_64);
    out->field_3C = lbl_eu_80668364 * (f32)*(const u16*)&rawD;
}

void CfGimmick_LoadBdatAreaRotation(CfGimmick* self, f32* out, void* bdat, void** table) {
    // Address-taking forces the call result to spill; the s16 deref reload
    // emits retail's stw + lha memory truncation idiom.
    s32 rawA = getBdatStringColumnValue(
        *table, *(char**)(lbl_eu_805357E8 + 0x1C) + 2, self->field_64);
    out[0] = (f32)*(const s16*)&rawA * lbl_eu_8066A210;
    s32 rawB = getBdatStringColumnValue(
        *table, *(char**)(lbl_eu_805357E8 + 0x20) + 2, self->field_64);
    out[1] = (f32)*(const s16*)&rawB * lbl_eu_8066A210;
    s32 rawC = getBdatStringColumnValue(
        *table, *(char**)(lbl_eu_805357E8 + 0x24) + 2, self->field_64);
    out[2] = (f32)*(const s16*)&rawC * lbl_eu_8066A210;
}

void CfGimmick_LoadBdatAreaPosIndexed(CfGimmick* self, f32* out, void* unused, void** holder, int v) {
    int c = v + 0x41;
    *(u8*)(*(u8* *)(lbl_eu_805357E8 + 0x08)) = (u8)c;
    *(u8*)(*(u8* *)(lbl_eu_805357E8 + 0x04)) = (u8)c;
    *(u8*)(*(u8* *)(lbl_eu_805357E8 + 0x00)) = (u8)c;
    out[0] = lbl_eu_80668364 * (f32)(s32)getBdatStringColumnValue(
        *holder, *(char**)(lbl_eu_805357E8 + 0x00), self->field_64);
    out[1] = lbl_eu_80668364 * (f32)(s32)getBdatStringColumnValue(
        *holder, *(char**)(lbl_eu_805357E8 + 0x04), self->field_64);
    out[2] = lbl_eu_80668364 * (f32)(s32)getBdatStringColumnValue(
        *holder, *(char**)(lbl_eu_805357E8 + 0x08), self->field_64);
}

void CfGimmick_LoadBdatAreaExtentsIndexed(CfGimmick* self, CfGimmick* out, void* unused, void** holder, int v) {
    u8 c = (u8)(v + 0x41);
    *(u8*)(*(u8* *)(lbl_eu_805357E8 + 0x18)) = (u8)c;
    *(u8*)(*(u8* *)(lbl_eu_805357E8 + 0x14)) = (u8)c;
    *(u8*)(*(u8* *)(lbl_eu_805357E8 + 0x10)) = (u8)c;
    *(u8*)(*(u8* *)(lbl_eu_805357E8 + 0x0C)) = (u8)c;
    u32 rawA = getBdatStringColumnValue(*holder, *(char**)(lbl_eu_805357E8 + 0x0C), self->field_64);
    out->field_30 = lbl_eu_80668364 * (f32)*(const u16*)&rawA;
    u32 rawB = getBdatStringColumnValue(*holder, *(char**)(lbl_eu_805357E8 + 0x10), self->field_64);
    out->field_34 = lbl_eu_80668364 * (f32)*(const u16*)&rawB;
    u32 rawC = getBdatStringColumnValue(*holder, *(char**)(lbl_eu_805357E8 + 0x14), self->field_64);
    out->field_38 = lbl_eu_80668364 * (f32)*(const u16*)&rawC;
    u32 rawD = getBdatStringColumnValue(*holder, *(char**)(lbl_eu_805357E8 + 0x18), self->field_64);
    out->field_3C = lbl_eu_80668364 * (f32)*(const u16*)&rawD;
}

void CfGimmick_LoadBdatAreaRotationIndexed(CfGimmick* self, f32* out, void* unused, void** holder, int v) {
    u8 c = (u8)(v + 0x41);
    *(u8*)(*(u8* *)(lbl_eu_805357E8 + 0x24)) = c;
    *(u8*)(*(u8* *)(lbl_eu_805357E8 + 0x20)) = c;
    *(u8*)(*(u8* *)(lbl_eu_805357E8 + 0x1C)) = c;
    // Address-taking forces the call result to spill; the s16 deref reload
    // emits retail's stw + lha memory truncation idiom (same as CfGimmick_LoadBdatAreaRotation).
    s32 rawA = getBdatStringColumnValue(
        *holder, *(char**)(lbl_eu_805357E8 + 0x1C), self->field_64);
    out[0] = (f32)*(const s16*)&rawA * lbl_eu_8066A210;
    s32 rawB = getBdatStringColumnValue(
        *holder, *(char**)(lbl_eu_805357E8 + 0x20), self->field_64);
    out[1] = (f32)*(const s16*)&rawB * lbl_eu_8066A210;
    s32 rawC = getBdatStringColumnValue(
        *holder, *(char**)(lbl_eu_805357E8 + 0x24), self->field_64);
    out[2] = (f32)*(const s16*)&rawC * lbl_eu_8066A210;
}

int CfGimmick_CheckStateFlag1D44(u8* obj) {
    int v = func_8009CF8C((u32)(obj + 0x1d44));
    return (v == 1) ? 1 : 0;
}

int CfGimmick_CheckStateFlag2CC8(u8* obj) {
    int v = func_8009CF8C((u32)(obj + 0x2cc8));
    return (v == 1) ? 1 : 0;
}

// Sound trigger helper; retail callers reach it through a single-argument
// view (r4 is never initialised), so only the destination is named here.
void func_8009D018(u32 destination);
// `id` is the gimmick's resource-id base; the sound trigger lives at +0x2CC8.
void CfGimmick_TriggerSound2CC8(u32 id) { func_8009D018(id + 0x2CC8); }

// Party/rotation-gated checker dispatch: when mask has the party bits (0x21)
// the loaded-party flag and matching party id must hold; when mask has the
// rotation bit (0x4) the heading dot-product and side tests must pass.  Only
// then is the per-gimmick jumptable checker invoked (mirrors func_802098EC's
// guarded section).  The scaled angle is recomputed per Sin/Cos call (no
// local) to match retail's caller-saved FPR budget (f29-f31 only).
int CfGimmick_CheckTriggerGated(u32 mask, CfGimmick* gimmick, const CfGimmickVec3* point,
                  const f32* ang, u32 partyId) {
    // Party-scoped guard.  Retail keeps the (bc&1)==0 return-0 inline (block
    // A) but shares ONE return-0 block for the b4==0 / id-mismatch fails;
    // the pass path jumps to the rotation check (beq .L_8020B5A8).
    if (mask & 0x21) {
        if ((lbl_eu_806646BC & 1) == 0)
            return 0;
        if (lbl_eu_806646B4 != 0 && partyId == lbl_eu_806646B4)
            goto rotation;
        return 0;
    }

rotation:
    if (mask & 0x4) {
        // Scoped angle temporaries force retail's load schedule: the raw
        // angle is fetched by its own statement first, then the fixed scale
        // inside each product (lfs var; lfs K; fmuls fD, f0(K), fX(var)).
        f32 sa;
        f32 ca;
        f32 sb;
        f32 cb;
        {
            f32 a = ang[1];
            sa = nw4r::math::SinFIdx(lbl_eu_80668354 * a);
        }
        {
            f32 a = ang[1];
            ca = nw4r::math::CosFIdx(lbl_eu_80668354 * a);
        }
        {
            f32 b = lbl_eu_806646B0;
            sb = nw4r::math::SinFIdx(lbl_eu_80668354 * b);
        }
        {
            f32 b = lbl_eu_806646B0;
            cb = nw4r::math::CosFIdx(lbl_eu_80668354 * b);
        }
        if (sa * sb + ca * cb <= lbl_eu_80668350)
            return 0;
        if (sa * (lbl_eu_805765A0.x - point->x) +
                ca * (lbl_eu_805765A0.z - point->z) >= lbl_eu_80668350)
            return 0;
    }

    if (jumptable_eu_80535830[gimmick->field_44](gimmick, &lbl_eu_805765A0, point))
        return 1;
    return 0;
}

// Gimmick trigger query.  Retail keeps every block fully inlined (the list
// scans, player gates and player loops are duplicated verbatim), so the
// source mirrors that duplication instead of factoring helpers.
// Mask bits: 0x21 party guard, 0x4 rotation occlusion, 0x00C00000 fight-list
// scan, 0x00800000 enable player scan after it, 0x01000000 player 0 only
// (otherwise all players minus the 0x10<<i presence bits).  When neither the
// fight-list nor the bc&4 enemy-list path runs, players are scanned alone.
int func_802098EC(u32 mask, CfGimmick* gimmick, const CfGimmickVec3* point,
                  const f32* ang, u32 partyId) {
    int partyValid = 0;
    if (mask & 0x21) {
        // Party-scoped queries require the loaded-party flag and a matching ID.
        if ((lbl_eu_806646BC & 1) == 0)
            return 0;
        if (lbl_eu_806646B4 != 0) {
            if (partyId == lbl_eu_806646B4) {
                partyValid = 1;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    }

    if (mask & 0x4) {
        // Rotation-gated test: relative bearing dot product plus side test.
        // The scaled angle is recomputed for each call (no shared local).
        // Scoped angle temporaries force retail's load schedule: raw angle
        // fetched first, then the fixed scale inside each product.
        f32 sa;
        f32 ca;
        f32 sb;
        f32 cb;
        {
            f32 a = ang[1];
            sa = nw4r::math::SinFIdx(lbl_eu_80668354 * a);
        }
        {
            f32 a = ang[1];
            ca = nw4r::math::CosFIdx(lbl_eu_80668354 * a);
        }
        {
            f32 b = lbl_eu_806646B0;
            sb = nw4r::math::SinFIdx(lbl_eu_80668354 * b);
        }
        {
            f32 b = lbl_eu_806646B0;
            cb = nw4r::math::CosFIdx(lbl_eu_80668354 * b);
        }
        if (ca * cb + sa * sb <= lbl_eu_80668350)
            return 0;
        if (ca * (lbl_eu_805765A0.z - point->z) +
                sa * (lbl_eu_805765A0.x - point->x) >= lbl_eu_80668350)
            return 0;
    }

    if (partyValid) {
        if (jumptable_eu_80535830[gimmick->field_44](gimmick, &lbl_eu_805765A0, point))
            return 1;
        return 0;
    }

    if (mask & 0x00C00000UL) {
        // Fight-list scan, then optionally the player scan.
        CfGimmickList* list = getReslistB48();
        CfGimmickListNode* n = list->head->next;
        while (n != list->head) {
            CfGimmickVec3* target = (CfGimmickVec3*)((cf::CfObject*)n->object)->CfObject_getPosVector();
            if (jumptable_eu_80535830[gimmick->field_44](gimmick, target, point))
                return 1;
            n = n->next;
        }
        if (mask & 0x00800000UL) {
            if (mask & 0x01000000UL) {
                // Player 0 only.
                CfPlayerBase* base = (CfPlayerBase*)cf::CfGameManager::getPlayer(0);
                if (base != 0)
                    base = (CfPlayerBase*)((char*)base - 0x3E9C);
                int ok;
                if (base != 0 && ((cf::CActorParam*)((char*)base - 0x3E9C))->CActorParam_getHp() > lbl_eu_80668350) {
                    if (base->subField3F60 == 0 ||
                        ((((CfPlayerSub3F60*)base->subField3F60)->field_4EC & 0x800) == 0))
                        ok = 1;
                    else
                        ok = 0;
                } else
                    ok = 0;
                if (ok == 0)
                    return 0;
                CfGimmickVec3* target = (CfGimmickVec3*)((cf::CfObject*)&base->spot)->CfObject_getPosVector();
                if (jumptable_eu_80535830[gimmick->field_44](gimmick, target, point))
                    return 1;
            } else {
                // All players, skipping present ones.  partyValid is dead by
                // here; reusing it as the loop counter mirrors retail's
                // register recycling.
                f32 zero = lbl_eu_80668350;
                for (partyValid = 0; partyValid < 3; ++partyValid) {
                    if (lbl_eu_806646BC & (0x10 << partyValid))
                        continue;
                    CfPlayerBase* base =
                        (CfPlayerBase*)cf::CfGameManager::getPlayer(partyValid);
                    if (base != 0)
                        base = (CfPlayerBase*)((char*)base - 0x3E9C);
                    int ok;
                    if (base != 0 && ((cf::CActorParam*)((char*)base - 0x3E9C))->CActorParam_getHp() > zero) {
                        if (base->subField3F60 == 0 ||
                            ((((CfPlayerSub3F60*)base->subField3F60)->field_4EC & 0x800) == 0))
                            ok = 1;
                        else
                            ok = 0;
                    } else
                        ok = 0;
                    if (ok == 0)
                        continue;
                    CfGimmickVec3* target = (CfGimmickVec3*)((cf::CfObject*)&base->spot)->CfObject_getPosVector();
                    if (jumptable_eu_80535830[gimmick->field_44](gimmick, target, point))
                        return 1;
                }
            }
        }
        return 0;
    }

    // No fight-list flag: enemy list first, then the player scan.
    if (lbl_eu_806646BC & 4) {
        CfGimmickList* list = getReslistB68();
        CfGimmickListNode* n = list->head->next;
        while (n != list->head) {
            CfGimmickVec3* target = (CfGimmickVec3*)((cf::CfObject*)n->object)->CfObject_getPosVector();
            if (jumptable_eu_80535830[gimmick->field_44](gimmick, target, point))
                return 1;
            n = n->next;
        }
        return 0;
    }
    if (mask & 0x01000000UL) {
        // Player 0 only.
        CfPlayerBase* base = (CfPlayerBase*)cf::CfGameManager::getPlayer(0);
        if (base != 0)
            base = (CfPlayerBase*)((char*)base - 0x3E9C);
        int ok;
        if (base != 0 && ((cf::CActorParam*)((char*)base - 0x3E9C))->CActorParam_getHp() > lbl_eu_80668350) {
            if (base->subField3F60 == 0 ||
                ((((CfPlayerSub3F60*)base->subField3F60)->field_4EC & 0x800) == 0))
                ok = 1;
            else
                ok = 0;
        } else
            ok = 0;
        if (ok == 0)
            return 0;
        CfGimmickVec3* target = (CfGimmickVec3*)((cf::CfObject*)&base->spot)->CfObject_getPosVector();
        if (jumptable_eu_80535830[gimmick->field_44](gimmick, target, point))
            return 1;
        return 0;
    }
    // All players, skipping present ones.
    f32 zero = lbl_eu_80668350;
    for (partyValid = 0; partyValid < 3; ++partyValid) {
        if (lbl_eu_806646BC & (0x10 << partyValid))
            continue;
        CfPlayerBase* base = (CfPlayerBase*)cf::CfGameManager::getPlayer(partyValid);
        if (base != 0)
            base = (CfPlayerBase*)((char*)base - 0x3E9C);
        int ok;
        if (base != 0 && ((cf::CActorParam*)((char*)base - 0x3E9C))->CActorParam_getHp() > zero) {
            if (base->subField3F60 == 0 ||
                ((((CfPlayerSub3F60*)base->subField3F60)->field_4EC & 0x800) == 0))
                ok = 1;
            else
                ok = 0;
        } else
            ok = 0;
        if (ok == 0)
            continue;
        CfGimmickVec3* target = (CfGimmickVec3*)((cf::CfObject*)&base->spot)->CfObject_getPosVector();
        if (jumptable_eu_80535830[gimmick->field_44](gimmick, target, point))
            return 1;
    }
    return 0;
}

void CfGimmick_SetGlobalFlagC0042() {
    getUnk80664658()->field_214 |= 0x000C0042;
}

void CfGimmick_SetGlobalFlagC0002() {
    CfGimmickGlobal* p = getUnk80664658();
    *(volatile u32*)&p->field_214 |= 0x000C0002;
}

void CfGimmick_SetGlobalFlag8008() {
    getUnk80664658()->field_214 |= 0x8008;
}

void CfGimmick_SetGlobalFlagD0000() {
    getUnk80664658()->field_214 |= 0xD0000;
}

void CfGimmick_SetGlobalFlag40000() {
    getUnk80664658()->field_214 |= 0x40000;
}

void CfGimmick_SetGlobalFlag80000() {
    getUnk80664658()->field_214 |= 0x80000;
}

void CfGimmick_SetGlobalFlag200000() {
    getUnk80664658()->field_214 |= 0x200000;
}

void CfGimmick_SetGlobalFlag80AndValue(int arg0, int flag, u32 value) {
    CfGimmickGlobal* p = getUnk80664658();
    if (flag != 0) {
        p->field_214 |= 0x80;
    } else {
        p->field_214 &= ~0x80;
    }
    getUnk80664658()->field_210 = value;
}

void CfGimmick_SetGlobalFlag8() {
    getUnk80664658()->field_214 |= 0x8;
}

void CfGimmick_SetGlobalFlag400000() {
    getUnk80664658()->field_214 |= 0x400000;
}

void CfGimmick_ApplyPartyMoveSpeed(float value) {
    // Sentinel 0.0 is hoisted into a callee-saved FPR (f31) at entry and
    // reused for both the equality test and the +0x168 dispatch argument.
    float zero = lbl_eu_80668350;
    for (s32 i = 0; i < 3; ++i) {
        CfObjectMove* player = CfGameManager::getPlayer(i);
        if (player != nullptr) {
            if (value != zero) {
                // +0x168 = CfObject_syncModelRate (CfObjectMove
                // implicitly overrides the slot, so no view cast is needed).
                player->CfObject_syncModelRate(zero);
                CfObjectMove_setMoveSpeed(player, value);
            } else {
                player->CfObject_syncModelRate(lbl_eu_80668358);
            }
        }
    }
}

void CfGimmick_ApplyPartyMoveSpeedGated(float value) {
    float zero = lbl_eu_80668350;
    for (s32 i = 0; i < 3; ++i) {
        CfObjectMove* player = CfGameManager::getPlayer(i);
        if (player != nullptr) {
            if (value != zero) {
                // Same +0x168 slot as CfGimmick_ApplyPartyMoveSpeed (no view cast needed).
                player->CfObject_syncModelRate(lbl_eu_80668358);
                CfObjectMove_setMoveSpeedGated(player, value);
            } else {
                player->CfObject_syncModelRate(zero);
            }
        }
    }
}

// Scans the fight list for a player whose (id >> 4) matches playerId and is
// still alive (vtable +0x128 HP query > 0); returns 0 in that case.
int CfGimmick_CheckFightListPlayerAlive(u32 playerId) {
    CfGimmickList* list = getReslistB48();
    CfGimmickListNode* node = list->head->next;
    int result = 1;
    float zero = lbl_eu_80668350;
    while (node != list->head) {
        // List objects point 0x3E9C into the player base; the HP/id reads and
        // the vtable dispatch all use that base pointer (retail keeps the
        // null case as-is and dereferences the offset anyway).
        CfPlayerBase* base = (CfPlayerBase*)node->object;
        if (base != 0)
            base = (CfPlayerBase*)((char*)base - 0x3E9C);
        if ((int)((CfPlayerIdView*)base)->id456C >> 4 == playerId) {
            float hp = ((cf::CActorParam*)((char*)base - 0x3E9C))->CActorParam_getHp();
            // Retail materializes the le-test through cror/mfcr/extrwi.
            int lowHP = hp <= zero;
            if (!lowHP)
                result = 0;
        }
        node = node->next;
    }
    return result;
}

// Spawn a gimmick object, name it (truncating over-long names into a fixed
// 0x20 buffer), activate it, reposition it above the given point, and return
// it.  Returns 0 if allocation fails.
CfGimmickObject* CfGimmick_SpawnNamedObject(const char* name, int other, const CfGimmickVec3* point) {
    CfGimmickVec3 pos;
    char buf[0x40];
    CfGimmickObject* obj = func_800B20B4(getInstance(), 0x4000, 0, 0);
    if (obj != 0) {
        if (strlen(name) >= 0x20) {
            strcpy(buf, name);
            buf[0x1F] = 0;
            func_800C13FC(obj, buf, other);
        } else {
            func_800C13FC(obj, name, other);
        }

        // Activate, then reposition the object above the requested point.
        obj->activate(1);
        // FPR temp order x,y,z drives the retail f0/f1/f2 colors; reversed
        // assignment keeps retail's load order, forward stores the struct.
        f32 x, y, z;
        z = point->z;
        y = lbl_eu_80668378[0] + point->y;
        x = point->x;
        pos.x = x;
        pos.y = y;
        pos.z = z;
        obj->setPos(&pos);
        obj->field_90 = 1;
        return obj;
    }
    return 0;
}

void CfGimmick_UnregisterSpawnedObject(CfGimmickReg* self) {
    if (self->field_00) {
        func_800B3A88(getInstance(), self->field_00);
        self->field_00 = 0;
    }
}

// Message-format gate shared by func_8020A484 / func_8020A6B0.
unsigned int CfGimmick_IsMessageSystemBusy();

// Look up a gimmick name from the bdat table: resolve the column for the
// requested row (prefixing the column string with '3'), format it into the
// shared message buffer and post the message.  The write-format path is
// gated on UIWin_GetInstance() (message system loaded) and func_80124B78() being
// zero (via the inlined CfGimmick_IsMessageSystemBusy boolean).  When the row is out of range
// (or the bdat file isn't loaded), the fallback name lbl_eu_80662788 is used.
int func_8020A484(int index) {
    // One reused `name` local: retail colors the bdat/column-string web and
    // the cap/default-name web into r31/r30 respectively.
    u8* bdat;
    s32 cap;
    s32 cnt;
    const char* name;
    if (index != 0) {
        if (lbl_eu_80664148 != 0) {
            bdat = (u8*)lbl_eu_80664148;
            func_8003AA34();
            // Named cap/cnt like the matched CfGimmick_LookupBdatGimmickName: B41C's result
            // stays callee-saved across the B1EC call.
            cap = func_8003B41C(bdat);
            cnt = func_8003B1EC(bdat);
            if (cap + cnt > index) {
                u8* col = *(u8**)(lbl_eu_805357E8 + 0x44);
                col[4] = 0x33;
                name = (const char*)getBdatStringColumnValue(
                    bdat, *(char**)(lbl_eu_805357E8 + 0x44), index);
                // Retail inlines the CfGimmick_IsMessageSystemBusy boolean (neg/or/srwi) here.
                if (UIWin_GetInstance() != 0) {
                    if (CfGimmick_IsMessageSystemBusy() == 0) {
                        sprintf(lbl_eu_805765D8, lbl_eu_80508634 + 0x2A, name);
                        UIWin_CreateSysWin0(lbl_eu_805765D8, 0, 0);
                        return 1;
                    }
                }
                return 0;
            }
        }
    }
    name = (const char*)lbl_eu_80662788;
    if (UIWin_GetInstance() != 0 && CfGimmick_IsMessageSystemBusy() == 0) {
        sprintf(lbl_eu_805765D8, lbl_eu_80508634 + 0x2A, name);
        UIWin_CreateSysWin0(lbl_eu_805765D8, 0, 0);
        return 1;
    }
    return 0;
}

// Returns 1 if func_80124B78() is non-zero, else 0 (retail: neg/or/srwi 31 idiom).
unsigned int CfGimmick_IsMessageSystemBusy() {
    unsigned int x = func_80124B78();
    /* (-x | x) >> 31: nonzero => 1, zero => 0 */
    return ((unsigned int)-(int)x | x) >> 31;
}

void* CfGimmick_LookupBdatGimmickName(int index, int mod) {
    if (index != 0) {
        u8* bdat = (u8*)lbl_eu_80664148;
        if (bdat != 0) {
            func_8003AA34();
            // Sequence the two row-capacity calls so the first (B41C) result is
            // kept in a callee-saved register across the second call, matching
            // the retail add r0, r31(b41c), r3(b1ec).
            u32 cap = func_8003B41C(bdat);
            u32 cnt = func_8003B1EC(bdat);
            // bound > index keeps the retail cmp r0(bound), r28(index) / ble form.
            if ((int)(cap + cnt) > index) {
                // Patch the column-name prefix, then read the requested row.
                u8* col = *(u8**)(lbl_eu_805357E8 + 0x44);
                col[4] = (u8)(mod + 0x31);
                return (void*)getBdatStringColumnValue(
                    bdat, *(char**)(lbl_eu_805357E8 + 0x44), index);
            }
        }
    }
    return lbl_eu_80662788;
}

void func_8020A6B0(CfGimmickReg* self, const CfGimmickVec3* point, f32 radius,
                   s32 index, s32 mod, s32 arg7) {
    // Distance from the fixed reference point to the requested position.
    // VEC3Sub's paired-single kernel reproduces retail's psq_l/ps_sub/psq_st
    // block; radiusSq is folded into the same schedule (fmuls between loads).
    // Distance from the fixed reference point to the requested position.
    // The nw4r paired-single kernels reproduce retail's psq_l/ps_sub block.
    nw4r::math::VEC3 diff;
    VEC3Sub(&diff, (const nw4r::math::VEC3*)&lbl_eu_805765A0,
            (const nw4r::math::VEC3*)point);
    f32 radiusSq = radius * radius;

    if (self->field_00 != 0) {
        // Registered object: unregister once the target leaves the radius.
        // Retail re-tests the entry-time null compare (cr1) after the length
        // check, keeping one shared return path for both guard fails.
        f32 lenSq = VEC3LenSq(&diff);
        if (lenSq > radiusSq && self->field_00 != 0) {
            func_800B3A88(getInstance(), self->field_00);
            self->field_00 = 0;
        }
        return;
    }

    // No object yet: spawn one only when the target is within range.  Both
    // paths recompute the length (retail duplicates the PS block per branch).
    if (VEC3LenSq(&diff) <= radiusSq) {
        // Name lookup: flat control flow mirroring retail - one shared
        // default-name load site (.L_8020C5EC), one bdat load held in a
        // callee-saved register through the row-capacity calls.
        const char* name;
        u8* bdat;
        if (index == 0)
            goto defaultName;
        bdat = (u8*)lbl_eu_80664148;
        if (bdat == 0)
            goto defaultName;
        func_8003AA34();
        {
            // B41C's result stays in a callee-saved register across B1EC.
            s32 cap = func_8003B41C(bdat);
            s32 cnt = func_8003B1EC(bdat);
            if ((s32)(cap + cnt) <= index)
                goto defaultName;
            // Patch the requested column's prefix, then read its value.
            u8* col = *(u8**)(lbl_eu_805357E8 + 0x44);
            col[4] = (u8)(mod + 0x31);
            name = (const char*)getBdatStringColumnValue(
                bdat, *(char**)(lbl_eu_805357E8 + 0x44), index);
            goto resolvedName;
        }
defaultName:
        name = (const char*)lbl_eu_80662788;
resolvedName:;

    CfGimmickObject* obj = func_800B20B4(getInstance(), 0x4000, 0, 0);
        CfGimmickObject* result;
        if (obj == 0) {
            result = 0;
        } else {
            if (strlen(name) >= 0x20) {
                // Truncate over-long names into a fixed 0x40 stack buffer.
                char buf[0x40];
                strcpy(buf, name);
                buf[0x3F] = 0;
                func_800C13FC(obj, buf, arg7);
            } else {
                func_800C13FC(obj, name, arg7);
            }

            // Activate, then reposition the object above the requested point.
            // FPR temp order mirrors matched CfGimmick_SpawnNamedObject's tail exactly.
            obj->activate(1);
            CfGimmickVec3 pos;
            f32 x, y, z;
            z = point->z;
            y = lbl_eu_80668378[0] + point->y;
            x = point->x;
            pos.x = x;
            pos.y = y;
            pos.z = z;
            obj->setPos(&pos);
            obj->field_90 = 1;
            result = obj;
        }
        self->field_00 = result;
    }
}

// CfGimmick_CheckPartyIdLoaded(dirID-cached check) - takes (this, arg); retail keeps the loaded
// party-id table value in r3 and the caller arg in r4, and shares one return-0
// epilogue for both guard failures (beq to a common block).
int CfGimmick_CheckPartyIdLoaded(CfGimmick* self, u32 arg) {
    (void)self;
    u32 a = lbl_eu_806646B4;
    if (a != 0 && (lbl_eu_806646BC & 1u) != 0) {
        return arg == a;
    }
    return 0;
}

int CfGimmick_CheckerCylinderXZ(CfGimmick* self, const CfGimmickVec3* point, const CfGimmickVec3* center) {
    float dz = center->z - point->z;
    float dx = center->x - point->x;
    // MWCC evaluates the right addend first: keep the dz diff declared first
    // and give the x-product as the left addend to force the retail
    // z-load-first schedule.
    if (dx * dx + dz * dz <= self->field_30 * self->field_30) {
        if (center->y + self->field_34 >= point->y &&
            center->y + self->field_38 <= point->y) {
            return 1;
        }
    }
    return 0;
}

// Collider hit test: the point must be within field_40 (squared distance) of
// the reference vector, and after transforming it into the gimmick's local
// space it must lie inside the horizontal radius (field_30) and vertical
// band [field_38, field_34].
int func_8020A928(CfGimmick* self, const CfGimmickVec3* point, const CfGimmickVec3* center) {
    // SDK VEC3Sub + VEC3LenSq kernels reproduce the retail paired-single
    // distance block.
    nw4r::math::VEC3 diff;
    VEC3Sub(&diff, (const nw4r::math::VEC3*)point, (const nw4r::math::VEC3*)center);
    if (VEC3LenSq(&diff) > self->field_40)
        return 0;

    Vec tmp;
    PSMTXMultVec((const f32 (*)[4])self, (const Vec*)point, (Vec*)&tmp);
    // Right addend first keeps retail's z-load-first schedule.
    if (tmp.x * tmp.x + tmp.z * tmp.z <= self->field_30 * self->field_30) {
        if (self->field_34 >= tmp.y && self->field_38 <= tmp.y)
            return 1;
    }
    return 0;
}

int CfGimmick_CheckerBoxExtent(CfGimmick* self, const CfGimmickVec3* point, const CfGimmickVec3* center) {
    if (center->x + self->field_30 >= point->x &&
        center->x - self->field_30 <= point->x &&
        center->z + self->field_3C >= point->z &&
        center->z - self->field_3C <= point->z &&
        center->y + self->field_34 >= point->y &&
        center->y + self->field_38 <= point->y) {
        return 1;
    }
    return 0;
}

// PS-optimized collider test: squared distance from the reference vector must
// be within field_40, and after transforming the point into the gimmick's
// local space it must lie inside the horizontal/vertical extent box.  The
// distance math goes through the nw4r VEC3Sub/VEC3LenSq helpers, whose
// inlined paired-single kernels reproduce retail's psq_l/ps_sub/ps_mul/
// ps_madd/ps_sum0 sequence.
int func_8020AA8C(CfGimmick* self, const CfGimmickVec3* point, const CfGimmickVec3* center) {
    nw4r::math::VEC3 diff;
    VEC3Sub(&diff, (const nw4r::math::VEC3*)center, (const nw4r::math::VEC3*)point);
    // Success path nested inside the radius test keeps one shared fail-exit
    // like retail (.L_8020C9BC).
    if (VEC3LenSq(&diff) <= self->field_40) {
        Vec tmp;
        PSMTXMultVec((const f32 (*)[4])self, (const Vec*)point, (Vec*)&tmp);
        // Retail tests the local-space extents in z, x, y order, fneg-ing each
        // half-extent for the lower bound.
        if (self->field_30 >= tmp.z && -self->field_30 <= tmp.z &&
            self->field_3C >= tmp.x && -self->field_3C <= tmp.x &&
            self->field_34 >= tmp.y && self->field_38 <= tmp.y)
            return 1;
    }
    return 0;
}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
extern "C" void sinit_8020AB7C() {}

// Retail free function taking (gimmick, float): the 801F3BE0 callers set f1
// from lbl_eu_80668158/60 before bl CfGimmick_PushRefreshValue, and the body tail-forwards
// (field_78, f1) to vtable slot +0x88 with no f1 setup of its own. Slot and
// arity coincide with the CfObject-layout +0x88 float slot
// (CfObject_pushRefreshValue); the sibling +0x9C (vec) / +0xC4 (float)
// dispatches on the same object in CfGimmickElv match the CfObject-layout
// slots too. The object itself is the createBattleActor product (back-pointer
// at +0xB0, cleared by CfGimmick_DetachManager/CfGimmick_ClearManagerBinding), driven here through the
// CfObject view. The dispatch is the function's last statement, so MWCC
// emits it as a tail call (bctr).
void CfGimmick_PushRefreshValue(cf::CfGimmick* self, float value) {
    if (self->field_78 == 0)
        return;
    reinterpret_cast<cf::CfObject*>(self->field_78)->CfObject_pushRefreshValue(value);
}


// Retail free-function checker for placement kind 0 (always passes). The
// CfGimmick::CfGimmick_CheckerKind0Always method above is its C++-linkage twin kept for
// header compat; this C-linkage twin owns the retail jumptable slot
// (retail .data jumptable_eu_80535830[0] = CfGimmick_CheckerKind0Always).
extern "C" int CfGimmick_CheckerKind0Always(cf::CfGimmick* self, const CfGimmickVec3* point, const CfGimmickVec3* center) {
    (void)self; (void)point; (void)center;
    return 1;
}

// --- typified .data (replaces the old generated byte blob) ---
// lbl_eu_805357E8: bdat column-name pointer table (18 string labels, all
// owned by this TU's .rodata/.sdata2 above). Slots emit zero bytes +
// ADDR32 relocs in the .o, matching retail.
// jumptable_eu_80535830: placement-checker dispatch (5 checkers defined in
// this TU). Layout from build/us/asm/kyoshin/cf/CfGimmick.s.
extern "C" {
__declspec(section ".data") __attribute__((used, aligned(8)))
const void* lbl_eu_805357E8[18] = {
    lbl_eu_80668320, lbl_eu_80668328, lbl_eu_80668330,
    lbl_eu_80508598, lbl_eu_805085A4, lbl_eu_805085B0,
    lbl_eu_805085BC, lbl_eu_805085C8, lbl_eu_805085D4,
    lbl_eu_805085E0, lbl_eu_805085EC,
    lbl_eu_80668338, lbl_eu_80668340,
    lbl_eu_805085F8, lbl_eu_80508604, lbl_eu_80508610,
    lbl_eu_8050861C, lbl_eu_80668348,
};
__declspec(section ".data") __attribute__((used))
CfGimmickChecker jumptable_eu_80535830[5] = {
    (CfGimmickChecker)CfGimmick_CheckerKind0Always,
    (CfGimmickChecker)CfGimmick_CheckerCylinderXZ,
    (CfGimmickChecker)func_8020A928,
    (CfGimmickChecker)CfGimmick_CheckerBoxExtent,
    (CfGimmickChecker)func_8020AA8C,
};
}
