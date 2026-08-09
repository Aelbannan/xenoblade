// Auto-scaffolded catalog TU for kyoshin/cf/CfGimmick
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/cf/CfGimmick.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/object/CfObjectMove.hpp"
#include <nw4r/math.h>

// Forward declarations for cross-TU callees (resolved via the retail symbol map).
class UnkClass_805764CC;
UnkClass_805764CC* func_800B07E8();  // MWCC free-fn mangling -> func_800B07E8__Fv

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

    if (self->field_30 == zero && self->field_34 == zero && self->field_3C == zero) {
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

    nw4r::math::MTX34Inv((nw4r::math::MTX34*)self, (nw4r::math::MTX34*)self);
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

void func_80208CC0(){}

void func_80208E98() {
    lbl_eu_80662784 = lbl_eu_80668358;
    lbl_eu_806646C0 = 0;
    for (int i = 0; i < 10; i++) lbl_eu_805765B0[i] = -1;
}

// retail: stw r3, lbl_eu_806646B8; blr — store arg to global
extern unsigned long lbl_eu_806646B8;
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
    out->field_30 = lbl_eu_80668364 * (f32)(u16)getBdatStringColumnValue(
        *holder, *(char**)(lbl_eu_805357E8 + 0x0C) + 2, self->field_64);
    out->field_34 = lbl_eu_80668364 * (f32)(u16)getBdatStringColumnValue(
        *holder, *(char**)(lbl_eu_805357E8 + 0x10) + 2, self->field_64);
    out->field_38 = lbl_eu_80668364 * (f32)(u16)getBdatStringColumnValue(
        *holder, *(char**)(lbl_eu_805357E8 + 0x14) + 2, self->field_64);
    out->field_3C = lbl_eu_80668364 * (f32)(u16)getBdatStringColumnValue(
        *holder, *(char**)(lbl_eu_805357E8 + 0x18) + 2, self->field_64);
}

void func_8020915C(CfGimmick* self, CfGimmick* out, void* unused, void** holder) {
    out->field_30 = lbl_eu_80668364 * (f32)(u16)getBdatStringColumnValue(
        *holder, lbl_eu_80508634 + 0x00, self->field_64);
    out->field_34 = lbl_eu_80668364 * (f32)(u16)getBdatStringColumnValue(
        *holder, lbl_eu_80508634 + 0x0A, self->field_64);
    out->field_38 = lbl_eu_80668364 * (f32)(u16)getBdatStringColumnValue(
        *holder, lbl_eu_80508634 + 0x15, self->field_64);
    out->field_3C = lbl_eu_80668364 * (f32)(u16)getBdatStringColumnValue(
        *holder, lbl_eu_80508634 + 0x20, self->field_64);
}

void func_80209288(CfGimmick* self, f32* out, void* bdat, void** table) {
    u32 value;
    value = (u32)getBdatStringColumnValue(
        *table, *(char**)(lbl_eu_805357E8 + 0x1C) + 2, self->field_64);
    out[0] = lbl_eu_8066A210 * (f32)(s16)value;
    value = (u32)getBdatStringColumnValue(
        *table, *(char**)(lbl_eu_805357E8 + 0x20) + 2, self->field_64);
    out[1] = lbl_eu_8066A210 * (f32)(s16)value;
    value = (u32)getBdatStringColumnValue(
        *table, *(char**)(lbl_eu_805357E8 + 0x24) + 2, self->field_64);
    out[2] = lbl_eu_8066A210 * (f32)(s16)value;
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
    int c = v + 0x41;
    *(u8*)(*(u8* *)(lbl_eu_805357E8 + 0x18)) = (u8)c;
    *(u8*)(*(u8* *)(lbl_eu_805357E8 + 0x14)) = (u8)c;
    *(u8*)(*(u8* *)(lbl_eu_805357E8 + 0x10)) = (u8)c;
    *(u8*)(*(u8* *)(lbl_eu_805357E8 + 0x0C)) = (u8)c;
    out->field_30 = lbl_eu_80668364 * (f32)(u16)getBdatStringColumnValue(
        *holder, *(char**)(lbl_eu_805357E8 + 0x0C), self->field_64);
    out->field_34 = lbl_eu_80668364 * (f32)(u16)getBdatStringColumnValue(
        *holder, *(char**)(lbl_eu_805357E8 + 0x10), self->field_64);
    out->field_38 = lbl_eu_80668364 * (f32)(u16)getBdatStringColumnValue(
        *holder, *(char**)(lbl_eu_805357E8 + 0x14), self->field_64);
    out->field_3C = lbl_eu_80668364 * (f32)(u16)getBdatStringColumnValue(
        *holder, *(char**)(lbl_eu_805357E8 + 0x18), self->field_64);
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

void func_80209754(){}

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
    float hp = ((float (*)(void*))base->vtable[0x128 >> 2])(base);
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
    for (s32 i = 0; i < 3; ++i) {
        CfObjectMove* player = CfGameManager::getPlayer(i);
        if (player != nullptr) {
            if (value != lbl_eu_80668350) {
                player->CfObject_UnkVirtualFunc70(lbl_eu_80668350);
                func_800BC3D8(player, value);
            } else {
                player->CfObject_UnkVirtualFunc70(lbl_eu_80668358);
            }
        }
    }
}

void func_8020A1DC(float value) {
    for (s32 i = 0; i < 3; ++i) {
        CfObjectMove* player = CfGameManager::getPlayer(i);
        if (player != nullptr) {
            if (value != lbl_eu_80668350) {
                player->CfObject_UnkVirtualFunc70(lbl_eu_80668358);
                func_800BC3B0(player, value);
            } else {
                player->CfObject_UnkVirtualFunc70(lbl_eu_80668350);
            }
        }
    }
}

void func_8020A294(){}

void func_8020A35C(){}

void func_8020A434(CfGimmickReg* self) {
    if (self->field_00) {
        func_800B3A88(func_800B07E8(), self->field_00);
        self->field_00 = 0;
    }
}

void func_8020A484(){}

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

// Create/attach gimmick object and name it (resolved via retail symbol map).
CfGimmickObject* func_800B20B4(int a, int b, int c);

void func_8020A6B0(CfGimmickReg* self, const CfGimmickVec3* point, f32 radius,
                   int index, int mod, int arg7) {
    // Distance from the fixed reference point to the requested position.
    f32 d[3];
    d[0] = lbl_eu_805765A0.x - point->x;
    d[1] = lbl_eu_805765A0.y - point->y;
    d[2] = lbl_eu_805765A0.z - point->z;
    f32 distSq = d[0] * d[0] + d[1] * d[1] + d[2] * d[2];

    // If a gimmick object is currently registered, remove it once the target
    // point leaves the radius; otherwise leave it alone.
    if (self->field_00 != 0) {
        if (distSq > radius * radius) {
            func_800B3A88(func_800B07E8(), self->field_00);
            self->field_00 = 0;
        }
        return;
    }

    // No object yet: spawn one only when the target is within range.
    if (distSq > radius * radius)
        return;

    const char* name;
    if (index != 0) {
        void* bdat = lbl_eu_80664148;
        if (bdat != 0) {
            func_8003AA34();
            if (index < (int)(func_8003B41C(bdat) + func_8003B1EC(bdat))) {
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

    CfGimmickObject* obj = func_800B20B4(0x4000, 0, 0);
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

void func_8020A928(){}

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

void func_8020AA8C(){}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
extern "C" void sinit_8020AB7C() {}

extern "C" void func_8020899C() {}
