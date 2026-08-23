// Auto-scaffolded catalog TU for kyoshin/cf/CfGimmick
// Replace stubs with high-level C/C++ during decomp.

#include <cstdio>
#include <cstring>

#include "kyoshin/cf/CfGimmick.hpp"
#include "kyoshin/CTaskGame.hpp"   // CTaskGameCamView / func_8049603C (extern "C")
#include "kyoshin/cf/object/CfObjectMove.hpp"
#include <nw4r/math.h>

// Forward declarations for cross-TU callees (resolved via the retail symbol map).
class UnkClass_805764CC;

namespace cf {
// Minimal view of cf::CfGameManager for this TU (CfGameManager.hpp is not
// included - its func_8007F91C is declared non-static, but retail callers
// (func_80208CC0) invoke the Fv symbol with a dead r3, i.e. a static call).
// Static members here emit the same pre-mangled retail symbols without a
// `this` load.  getPlayer() coexists with CfObjectMove.hpp's extern "C"
// declaration of the same symbol (different scopes - no overload).
class CfGameManager {
public:
    static CfObjectMove* getPlayer(int playerIndex);
    static bool func_8007F91C();
    u8 pad[0xB0];               // 0x00..0xAF
    UnkClass_800821F8* unkB0;   // 0xB0 - cleared by func_80208EE4
    void func_80080F44();
};
} // namespace cf

using namespace cf;

namespace cf {
    void CfGimmick::func_8020896C(void* other) {
        void* cur = *(void**)((char*)this + 0x78);
        if (cur != other) return;
        *(void**)((char*)cur + 0xB0) = 0;
        *(void**)((char*)this + 0x78) = 0;
    }

    void CfGimmick::func_80208988() {
        extern void func_802089BC(CfGimmick*, const f32*, const CfGimmickVec3*);
        func_802089BC((CfGimmick*)((char*)this + 0x1c), (const f32*)((char*)this + 4),
                      (const CfGimmickVec3*)((char*)this + 0x10));
    }

    int CfGimmick::func_8020A8AC() { return 1; }
}

// Constructor.  The retail symbol __ct__cf_CfGimmick is a C-linkage name (no
// C++ mangling markers), so it must be emitted as an extern "C" free function
// rather than a C++ member constructor.  Subclass constructors (CfGimmickElv,
// CfGimmickWarp, ...) call this to init the shared CfGimmick base fields.
extern "C" void __ct__cf_CfGimmick(CfGimmick* self) {
    self->vtable = (void*)lbl_eu_80535844;
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
void func_802089BC(CfGimmick* self, const f32* basis, const CfGimmickVec3* point) {
    f32 zero = lbl_eu_80668350;
    f32* m = ((nw4r::math::MTX34*)self)->m[0];

    if (self->field_38 > zero)
        self->field_38 = -self->field_38;

    // Retail compares (zero == field), so keep the constant on the left.
    if (zero == self->field_30 && zero == self->field_34 && zero == self->field_3C) {
        self->field_44 = 0;
        return;
    }

    if (self->field_3C == zero) {
        // No depth: horizontal-only placement.
        if (point->x == zero && point->z == zero) {
            self->field_44 = 1;
            f32 one = lbl_eu_80668358;
            m[0] = one; m[1] = zero; m[2] = zero; m[3] = basis[0];
            m[4] = zero; m[5] = one; m[6] = zero; m[7] = basis[1];
            m[8] = zero; m[9] = zero; m[10] = one; m[11] = basis[2];
        } else {
            f32 v[3];
            v[0] = self->field_30;
            v[1] = self->field_34;
            v[2] = self->field_3C;
            if (v[1] < -self->field_38)
                v[1] = -self->field_38;
            self->field_44 = 2;
            self->field_40 = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
            nw4r::math::MTX34RotXYZFIdx((nw4r::math::MTX34*)self, lbl_eu_80668354 * point->x,
                                        lbl_eu_80668354 * point->y, lbl_eu_80668354 * point->z);
            m[3] = basis[0];
            m[7] = basis[1];
            m[11] = basis[2];
        }
    } else {
        // With depth: full placement.
        f32 v[3];
        v[0] = self->field_30;
        v[1] = self->field_34;
        v[2] = self->field_3C;
        if (v[1] < -self->field_38)
            v[1] = -self->field_38;
        self->field_40 = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
        if (point->x == zero && point->y == zero && point->z == zero) {
            self->field_44 = 3;
            f32 one = lbl_eu_80668358;
            m[0] = one; m[1] = zero; m[2] = zero; m[3] = basis[0];
            m[4] = zero; m[5] = one; m[6] = zero; m[7] = basis[1];
            m[8] = zero; m[9] = zero; m[10] = one; m[11] = basis[2];
        } else {
            self->field_44 = 4;
            nw4r::math::MTX34RotXYZFIdx((nw4r::math::MTX34*)self, lbl_eu_80668354 * point->x,
                                        lbl_eu_80668354 * point->y, lbl_eu_80668354 * point->z);
            m[3] = basis[0];
            m[7] = basis[1];
            m[11] = basis[2];
        }
    }

    // Invert the rotation/translation matrix in place (retail PSMTXInverse).
    PSMTXInverse((const f32 (*)[4])self, (f32 (*)[4])self);
}

void func_80208C48(void* self, void* arg) {
    func_801BFDE8(1, (unsigned int)self, (unsigned int)arg,
                  lbl_eu_80668358, lbl_eu_8066835C);
}

void func_80208C60(void* self, void* arg, float second) {
    func_801BFDE8(1, (unsigned int)self, (unsigned int)arg,
                  lbl_eu_80668358, second);
}

void func_80208C78(cf::CfGimmick* self) {
    if (self->field_80 != 0) {
        func_801BFED0(1, self->field_80, 0xa);
        self->field_80 = 0;
    }
}

// Party-feature state update: re-anchor the gimmick reference point to the
// player's target-map position (or clear it when no player is loaded), and
// set the party/kicking flag bits.  Called on party joins/leaves.
void func_80208CC0(void* partyId, s32 flagA, s32 flagB) {
    lbl_eu_806646BC = 0;

    // Ground-height gate: while the stage base height is above zero, record
    // the downward drop value and flag it (bit 0x8).  The scene-camera height
    // query is re-run inside the block (retail calls func_8049603C twice).
    f32 rem = lbl_eu_80668358 - func_8049603C(lbl_eu_80663E14)->field_C;
    if (rem < lbl_eu_80668358) {
        lbl_eu_806646BC |= 0x8;
        lbl_eu_80662780 = lbl_eu_80668358 - func_8049603C(lbl_eu_80663E14)->field_C;
    }

    if (cf::CfGameManager::getPlayer(0) != 0) {
        CfGimmickPlayerFace* player = (CfGimmickPlayerFace*)cf::CfGameManager::getPlayer(0);
        // Integer-copy the target position (retail lwz/stw, no f32 path):
        // loads are hoisted x,y then stored y,x, then z - mirror with temps.
        CfGimmickVec3u* target = (CfGimmickVec3u*)player->d41();
        u32 tx = target->x;
        u32 ty = target->y;
        CfGimmickVec3u& dst = (CfGimmickVec3u&)lbl_eu_805765A0;
        dst.y = ty;
        dst.x = tx;
        dst.z = target->z;
        lbl_eu_806646B0 = player->d49();
        lbl_eu_806646B4 = (u32)partyId;
        if (flagA) lbl_eu_806646BC |= 0x20;
        if (flagB) lbl_eu_806646BC |= 0x40;
        if (player->field_C4 != 0 && player->d27() && player->d86() &&
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
    if (!cf::CfGameManager::func_8007F91C())
        lbl_eu_806646BC |= 0x1;
}

void func_80208E98() {
    lbl_eu_80662784 = lbl_eu_80668358;
    lbl_eu_806646C0 = 0;
    for (int i = 0; i < 10; i++) lbl_eu_805765B0[i] = -1;
}

// retail: stw r3, lbl_eu_806646B8; blr - store arg to global
extern unsigned long lbl_eu_806646B8;
extern const f64 lbl_eu_80668368;
extern "C" void func_80208EDC(u32 value) { lbl_eu_806646B8 = value; }

void func_80208EE4(cf::CfGimmick* self) {
    if (self->field_78) {
        self->field_78->unkB0 = 0;
        self->field_78->func_80080F44();
        self->field_78 = 0;
    }
}

void func_80208F34(CfGimmick* self, float* out, void* unused, void** holder) {
    // Read three adjacent bdat columns starting at each +2 prefix, converting
    // each integer cell to a scaled float written into the output vector.
    out[0] = lbl_eu_80668364 * (f32)(s32)getBdatStringColumnValue(
        *holder, *(char**)(lbl_eu_805357E8 + 0x00) + 2, self->field_64);
    out[1] = lbl_eu_80668364 * (f32)(s32)getBdatStringColumnValue(
        *holder, *(char**)(lbl_eu_805357E8 + 0x04) + 2, self->field_64);
    out[2] = lbl_eu_80668364 * (f32)(s32)getBdatStringColumnValue(
        *holder, *(char**)(lbl_eu_805357E8 + 0x08) + 2, self->field_64);
}

void func_80209020(CfGimmick* self, CfGimmick* out, void* unused, void** holder) {
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

void func_8020915C(CfGimmick* self, CfGimmick* out, void* unused, void** holder) {
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

void func_80209288(CfGimmick* self, f32* out, void* bdat, void** table) {
    // Address-taking forces the call result to spill; the s16 deref reload
    // emits retail's stw + lha memory truncation idiom.
    s32 rawA = getBdatStringColumnValue(
        *table, *(char**)(lbl_eu_805357E8 + 0x1C) + 2, self->field_64);
    out[0] = lbl_eu_8066A210 * (f32)*(const s16*)&rawA;
    s32 rawB = getBdatStringColumnValue(
        *table, *(char**)(lbl_eu_805357E8 + 0x20) + 2, self->field_64);
    out[1] = lbl_eu_8066A210 * (f32)*(const s16*)&rawB;
    s32 rawC = getBdatStringColumnValue(
        *table, *(char**)(lbl_eu_805357E8 + 0x24) + 2, self->field_64);
    out[2] = lbl_eu_8066A210 * (f32)*(const s16*)&rawC;
}

void func_8020938C(CfGimmick* self, f32* out, void* unused, void** holder, int v) {
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

void func_80209488(CfGimmick* self, CfGimmick* out, void* unused, void** holder, int v) {
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

void func_802095D8(CfGimmick* self, f32* out, void* unused, void** holder, int v) {
    u8 c = (u8)(v + 0x41);
    *(u8*)(*(u8* *)(lbl_eu_805357E8 + 0x24)) = c;
    *(u8*)(*(u8* *)(lbl_eu_805357E8 + 0x20)) = c;
    *(u8*)(*(u8* *)(lbl_eu_805357E8 + 0x1C)) = c;
    out[0] = lbl_eu_8066A210 * (f32)(s16)getBdatStringColumnValue(
        *holder, *(char**)(lbl_eu_805357E8 + 0x1C) + 2, self->field_64);
    out[1] = lbl_eu_8066A210 * (f32)(s16)getBdatStringColumnValue(
        *holder, *(char**)(lbl_eu_805357E8 + 0x20) + 2, self->field_64);
    out[2] = lbl_eu_8066A210 * (f32)(s16)getBdatStringColumnValue(
        *holder, *(char**)(lbl_eu_805357E8 + 0x24) + 2, self->field_64);
}

int func_802096EC(void* obj) {
    int v = func_8009CF8C((void*)((u32)obj + 0x1d44));
    return (v == 1) ? 1 : 0;
}

int func_8020971C(void* obj) {
    int v = func_8009CF8C((void*)((u32)obj + 0x2cc8));
    return (v == 1) ? 1 : 0;
}

void func_8009D018(void* self);
void func_8020974C(void* self) { ((void(*)(void*))func_8009D018)((char*)self + 0x2cc8); }

// Party/rotation-gated checker dispatch: when mask has the party bits (0x21)
// the loaded-party flag and matching party id must hold; when mask has the
// rotation bit (0x4) the heading dot-product and side tests must pass.  Only
// then is the per-gimmick jumptable checker invoked (mirrors func_802098EC's
// guarded section).  The scaled angle is recomputed per Sin/Cos call (no
// local) to match retail's caller-saved FPR budget (f29-f31 only).
int func_80209754(u32 mask, CfGimmick* gimmick, const CfGimmickVec3* point,
                  const f32* ang, void* partyId) {
    // Party-scoped guard.  Retail keeps the (bc&1)==0 return-0 inline (block
    // A) but shares ONE return-0 block for the b4==0 / id-mismatch fails;
    // the pass path jumps to the rotation check (beq .L_8020B5A8).
    if (mask & 0x21) {
        if ((lbl_eu_806646BC & 1) == 0)
            return 0;
        if (lbl_eu_806646B4 != 0 && partyId == (void*)(u32)lbl_eu_806646B4)
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

// Resolve each list node to the object handed to the checkers by calling its
// +0xAC vtable slot, then run every checker against the circular list until
// one passes.
static bool checkGimmickList(CfGimmickList* list, CfGimmick* gimmick,
                             const CfGimmickVec3* point) {
    CfGimmickListNode* n = list->head->next;
    while (n != list->head) {
        CfGimmickObject* obj = (CfGimmickObject*)n->object;
        void* target = ((void* (*)(CfGimmickObject*))obj->vtable[0xAC >> 2])(obj);
        if (jumptable_eu_80535830[gimmick->field_44](gimmick, target, point))
            return true;
        n = n->next;
    }
    return false;
}

// Enumerate the player whose +0x3E9C slot-object must be repelled, checking
// the live-HP / state-condition gate before dispatching the checker.
static bool checkGimmickPlayer(CfGimmick* gimmick, const CfGimmickVec3* point, int i) {
    void* pmv = cf::CfGameManager::getPlayer(i);
    if (pmv == 0)
        return false;
    CfPlayerBase* base = (CfPlayerBase*)((char*)pmv - 0x3E9C);
    float hp = base->getHP();
    if (hp <= 0.0f)
        return false;
    void* sub = base->subField3F60;
    if (sub != 0 && (((CfPlayerSub3F60*)sub)->field_4EC & (1u << 20)))
        return false;
    void* target = ((void* (*)(CfPlayerSpot*))base->spot.vtable[0xAC >> 2])(&base->spot);
    return jumptable_eu_80535830[gimmick->field_44](gimmick, target, point) != 0;
}

// Scan-all-players variant of checkGimmickPlayer, skipping any player whose
// per-player presence bit is set in lbl_eu_806646BC (0x10 << i).
static bool checkGimmickAllPlayers(CfGimmick* gimmick, const CfGimmickVec3* point) {
    for (int i = 0; i < 3; ++i) {
        if (lbl_eu_806646BC & (0x10 << i))
            continue;
        if (checkGimmickPlayer(gimmick, point, i))
            return true;
    }
    return false;
}

int func_802098EC(u32 mask, CfGimmick* gimmick, const CfGimmickVec3* point,
                  const f32* ang, void* partyId) {
    int partyValid = 0;
    if (mask & 0x21) {
        // Party-scoped queries require the loaded-party flag and a matching ID.
        if ((lbl_eu_806646BC & 1) == 0)
            return 0;
        if (lbl_eu_806646B4 == 0)
            return 0;
        if (partyId != (void*)(u32)lbl_eu_806646B4)
            return 0;
        partyValid = 1;
    }

    if (mask & 0x4) {
        // Rotation-gated test: compare the relative bearing to a fixed angle.
        // The scaled angle is recomputed for each call (not held in a local),
        // matching the retail caller-saved FPR budget (f29-f31 only).
        f32 sa = nw4r::math::SinFIdx(lbl_eu_80668354 * ang[1]);
        f32 ca = nw4r::math::CosFIdx(lbl_eu_80668354 * ang[1]);
        f32 sb = nw4r::math::SinFIdx(lbl_eu_80668354 * lbl_eu_806646B0);
        f32 cb = nw4r::math::CosFIdx(lbl_eu_80668354 * lbl_eu_806646B0);
        if (ca * cb + sa * sb <= 0.0f)
            return 0;
        if (ca * (lbl_eu_805765A0.z - point->z) + sa * (lbl_eu_805765A0.x - point->x) >= 0.0f)
            return 0;
    }

    if (partyValid) {
        if (jumptable_eu_80535830[gimmick->field_44](gimmick, (void*)&lbl_eu_805765A0, point))
            return 1;
        return 0;
    }

    if (mask & 0xC0) {
        // Scan the fight-list; then optionally the player slots.
        if (checkGimmickList(func_800B6BC8(), gimmick, point))
            return 1;
        if (mask & 0x80) {
            if (mask & 0x10) {
                if (checkGimmickPlayer(gimmick, point, 0))
                    return 1;
            } else if (checkGimmickAllPlayers(gimmick, point)) {
                return 1;
            }
        }
        return 0;
    }

    // No fight-list flag: enemy list first, then player slots.
    if (lbl_eu_806646BC & 4)
        return checkGimmickList(func_800B6BEC(), gimmick, point) ? 1 : 0;
    if (mask & 0x10) {
        if (checkGimmickPlayer(gimmick, point, 0))
            return 1;
    } else if (checkGimmickAllPlayers(gimmick, point)) {
        return 1;
    }
    return 0;
}

void func_80209F2C() {
    getUnk80664658()->field_214 |= 0x000C0042;
}

void func_80209F5C() {
    void* p = getUnk80664658();
    *(volatile unsigned int*)((unsigned char*)p + 0x214) |= 0x000C0002;
}

void func_80209F8C() {
    void* p = getUnk80664658();
    *(unsigned int*)((char*)p + 0x214) |= 0x8008;
}

void func_80209FB8() {
    void* p = getUnk80664658();
    *(unsigned int*)((char*)p + 0x214) |= 0xD0000;
}

void func_80209FE4() {
    void* p = getUnk80664658();
    *(unsigned int*)((char*)p + 0x214) |= 0x40000;
}

void func_8020A010() {
    void* p = getUnk80664658();
    *(unsigned int*)((char*)p + 0x214) |= 0x80000;
}

void func_8020A03C() {
    void* p = getUnk80664658();
    *(unsigned int*)((char*)p + 0x214) |= 0x200000;
}

void func_8020A068(int arg0, int flag, u32 value) {
    CfGimmickGlobal* p = getUnk80664658();
    if (flag != 0) {
        p->field_214 |= 0x80;
    } else {
        p->field_214 &= ~0x80;
    }
    getUnk80664658()->field_210 = value;
}

void func_8020A0CC() {
    void* p = getUnk80664658();
    *(unsigned int*)((char*)p + 0x214) |= 0x8;
}

void func_8020A0F8() {
    void* p = getUnk80664658();
    *(unsigned int*)((char*)p + 0x214) |= 0x400000;
}

void func_8020A124(float value) {
    // Sentinel 0.0 is hoisted into a callee-saved FPR (f31) at entry and
    // reused for both the equality test and the +0x168 dispatch argument.
    float zero = lbl_eu_80668350;
    for (s32 i = 0; i < 3; ++i) {
        CfObjectMove* player = CfGameManager::getPlayer(i);
        if (player != nullptr) {
            if (value != zero) {
                // +0x168 = CfObject_UnkVirtualFunc70 (dispatched through the
                // CfObject view since CfObjectMove redeclares it as a member).
                ((cf::CfObject*)player)->CfObject_UnkVirtualFunc70(zero);
                func_800BC3D8(player, value);
            } else {
                ((cf::CfObject*)player)->CfObject_UnkVirtualFunc70(lbl_eu_80668358);
            }
        }
    }
}

void func_8020A1DC(float value) {
    float zero = lbl_eu_80668350;
    for (s32 i = 0; i < 3; ++i) {
        CfObjectMove* player = CfGameManager::getPlayer(i);
        if (player != nullptr) {
            if (value != zero) {
                ((cf::CfObject*)player)->CfObject_UnkVirtualFunc70(lbl_eu_80668358);
                func_800BC3B0(player, value);
            } else {
                ((cf::CfObject*)player)->CfObject_UnkVirtualFunc70(zero);
            }
        }
    }
}

// Scans the fight list for a player whose (id >> 4) matches playerId and is
// still alive (vtable +0x128 HP query > 0); returns 0 in that case.
int func_8020A294(u32 playerId) {
    CfGimmickList* list = func_800B6BC8();
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
        float hp;
        if ((int)((CfPlayerIdView*)base)->id456C >> 4 == playerId) {
            float hp = base->getHP();
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
CfGimmickObject* func_8020A35C(const char* name, int other, const CfGimmickVec3* point) {
    CfGimmickVec3 pos;
    char buf[0x40];
    CfGimmickObject* obj = func_800B20B4((UnkClass_805764CC*)func_800B07E8(), 0x4000, 0, 0);
    if (obj != 0) {
        if (strlen(name) >= 0x20) {
            strcpy(buf, name);
            buf[0x1F] = 0;
            func_800C13FC(obj, buf, other);
        } else {
            func_800C13FC(obj, name, other);
        }

        // Activate, then reposition the object above the requested point.
        ((CfGimmickSpawnIf*)obj)->activate(1);
        // FPR temp order x,y,z drives the retail f0/f1/f2 colors; reversed
        // assignment keeps retail's load order, forward stores the struct.
        f32 x, y, z;
        z = point->z;
        y = lbl_eu_80668378 + point->y;
        x = point->x;
        pos.x = x;
        pos.y = y;
        pos.z = z;
        ((CfGimmickSpawnIf*)obj)->setPos(&pos);
        obj->field_90 = 1;
        return obj;
    }
    return 0;
}

void func_8020A434(CfGimmickReg* self) {
    if (self->field_00) {
        func_800B3A88((UnkClass_805764CC*)func_800B07E8(), self->field_00);
        self->field_00 = 0;
    }
}

// Look up a gimmick name from the bdat table: resolve the column for the
// requested row (prefixing the column string with '3'), format it into the
// shared message buffer and post the message.  The write-format path is
// gated on func_8013C54C() (message system loaded) and func_80124B78() being
// zero (via the inlined func_8020A5DC boolean).  When the row is out of range
// (or the bdat file isn't loaded), the fallback name lbl_eu_80662788 is used.
unsigned int func_8020A5DC();

int func_8020A484(int index) {
    // One reused `name` local: retail colors the bdat/column-string web and
    // the cap/default-name web into r31/r30 respectively.
    void* bdat;
    s32 cap;
    s32 cnt;
    const char* name;
    if (index != 0) {
        bdat = lbl_eu_80664148;
        if (bdat != 0) {
            func_8003AA34();
            // Named cap/cnt like the matched func_8020A608: B41C's result
            // stays callee-saved across the B1EC call.
            s32 cap = func_8003B41C(bdat);
            s32 cnt = func_8003B1EC(bdat);
            if (cap + cnt > index) {
                u8* col = *(u8**)(lbl_eu_805357E8 + 0x44);
                col[4] = 0x33;
                name = (const char*)getBdatStringColumnValue(
                    bdat, *(char**)(lbl_eu_805357E8 + 0x44), index);
                // Retail inlines the func_8020A5DC boolean (neg/or/srwi) here.
                if (func_8013C54C() != 0) {
                    if (func_8020A5DC() == 0) {
                        sprintf(lbl_eu_805765D8, lbl_eu_80508634 + 0x2A, name);
                        func_8013D55C(lbl_eu_805765D8, 0, 0);
                        return 1;
                    }
                }
                return 0;
            }
        }
    }
    name = (const char*)lbl_eu_80662788;
    if (func_8013C54C() != 0 && func_8020A5DC() == 0) {
        sprintf(lbl_eu_805765D8, lbl_eu_80508634 + 0x2A, name);
        func_8013D55C(lbl_eu_805765D8, 0, 0);
        return 1;
    }
    return 0;
}

// Returns 1 if func_80124B78() is non-zero, else 0 (retail: neg/or/srwi 31 idiom).
unsigned int func_8020A5DC() {
    unsigned int x = func_80124B78();
    /* (-x | x) >> 31: nonzero => 1, zero => 0 */
    return ((unsigned int)-(int)x | x) >> 31;
}

void* func_8020A608(int index, int mod) {
    if (index != 0) {
        void* bdat = lbl_eu_80664148;
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
                   int index, int mod, int arg7) {
    // Distance from the fixed reference point to the requested position.  The
    // nw4r VEC3 helper kernels reproduce retail's psq_l/ps_sub/psq_st block,
    // and VEC3LenSq the ps_mul/ps_madd/ps_sum0 distSq per path.
    nw4r::math::VEC3 diff;
    VEC3Sub(&diff, (const nw4r::math::VEC3*)&lbl_eu_805765A0,
            (const nw4r::math::VEC3*)point);
    f32 radiusSq = radius * radius;

    if (self->field_00 != 0) {
        // Object registered: remove it once the target leaves the radius.
        // The inner re-test of field_00 reuses the entry cr1 compare (retail
        // beq cr1) and gates the unregister call.
        if (VEC3LenSq(&diff) > radiusSq && self->field_00 != 0) {
            func_800B3A88((UnkClass_805764CC*)func_800B07E8(), self->field_00);
            self->field_00 = 0;
        }
        return;
    }

    // No object yet: spawn one only when the target is within range.
    if (VEC3LenSq(&diff) <= radiusSq) {
    const char* name;
    if (index != 0) {
        void* bdat = lbl_eu_80664148;
        if (bdat != 0) {
            func_8003AA34();
            // Sequence the row-capacity calls so B41C runs first (retail
            // keeps its result in a callee-saved reg across the B1EC call).
            u32 cap = func_8003B41C(bdat);
            u32 cnt = func_8003B1EC(bdat);
            if (index < (int)(cap + cnt)) {
                // Patch the requested column's prefix, then read the row value.
                u8* col = *(u8**)(lbl_eu_805357E8 + 0x44);
                col[4] = (u8)(mod + 0x31);
                name = (const char*)getBdatStringColumnValue(
                    bdat, *(char**)(lbl_eu_805357E8 + 0x44), index);
            } else {
                name = (const char*)lbl_eu_80662788;
            }
        } else {
            name = (const char*)lbl_eu_80662788;
        }
    } else {
        name = (const char*)lbl_eu_80662788;
    }

    UnkClass_805764CC* mgr = (UnkClass_805764CC*)func_800B07E8();
    CfGimmickObject* obj = func_800B20B4(mgr, 0x4000, 0, 0);
    if (obj == 0) {
        self->field_00 = 0;
        return;
    }

    if (strlen(name) >= 0x20) {
        // Truncate over-long names into a fixed 0x20 buffer.
        char buf[0x20];
        strcpy(buf, name);
        buf[0x1F] = 0;
        func_800C13FC(obj, buf, arg7);
    } else {
        func_800C13FC(obj, name, arg7);
    }

    // Activate, then reposition the object right above the requested point.
    ((void (*)(CfGimmickObject*, int))obj->vtable[0x158 >> 2])(obj, 1);
    CfGimmickVec3 pos;
    pos.x = point->x;
    pos.y = point->y + lbl_eu_80668378;
    pos.z = point->z;
    ((void (*)(CfGimmickObject*, const CfGimmickVec3*))obj->vtable[0x9C >> 2])(obj, &pos);
    obj->field_90 = 1;
    self->field_00 = (void*)obj;
    }
}

// func_8020A87C(dirID-cached check) - takes (this, arg); retail keeps the loaded
// party-id table value in r3 and the caller arg in r4, and shares one return-0
// epilogue for both guard failures (beq to a common block).
int func_8020A87C(void* self, u32 arg) {
    u32 a = lbl_eu_806646B4;
    if (a != 0 && (lbl_eu_806646BC & 1u) != 0) {
        return arg == a;
    }
    return 0;
}

int func_8020A8B4(CfGimmick* self, const CfGimmickVec3* point, const CfGimmickVec3* center) {
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

int func_8020A9F4(CfGimmick* self, const CfGimmickVec3* point, const CfGimmickVec3* center) {
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
    // SDK VEC3Sub + VEC3LenSq kernels, same idiom as func_8020A928.
    nw4r::math::VEC3 diff;
    VEC3Sub(&diff, (const nw4r::math::VEC3*)point, (const nw4r::math::VEC3*)center);
    if (VEC3LenSq(&diff) > self->field_40)
        return 0;

    Vec tmp;
    PSMTXMultVec((const f32 (*)[4])self, (const Vec*)point, (Vec*)&tmp);
    if (self->field_30 >= tmp.x && -self->field_30 <= tmp.x &&
        self->field_3C >= tmp.z && -self->field_3C <= tmp.z &&
        self->field_34 >= tmp.y && self->field_38 <= tmp.y)
        return 1;
    return 0;
}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
extern "C" void sinit_8020AB7C() {}

// Vfunc7 of the CfGimmick vtable: forward to the linked field_78
// game-manager object's vtable slot +0x88 when it is present.  The dispatch
// is the function's last statement, so MWCC emits it as a tail call (bctr).
void func_8020899C(cf::CfGimmick* self) {
    if (self->field_78 == 0)
        return;
    ((CfGimmickMgr88*)self->field_78)->m88();
}
