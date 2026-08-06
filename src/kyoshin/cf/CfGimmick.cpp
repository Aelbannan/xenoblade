// Auto-scaffolded catalog TU for kyoshin/cf/CfGimmick
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/cf/CfGimmick.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/object/CfObjectMove.hpp"

// Forward declarations for cross-TU callees (resolved via the retail symbol map).
class UnkClass_805764CC;
UnkClass_805764CC* func_800B07E8();  // MWCC free-fn mangling -> func_800B07E8__Fv
extern "C" void func_800B3A88(UnkClass_805764CC* self, void* target);
extern "C" void func_801BFED0(int a, u16 b, int c);
extern "C" int func_8009CF8C(void*);

using namespace cf;

// Small-data globals accessed by CfGimmick-region helpers (@sda21 loads/stores).
extern "C" u32 lbl_eu_806646B4;
extern "C" u32 lbl_eu_806646BC;
extern "C" float lbl_eu_80662784;
extern "C" u16 lbl_eu_806646C0;
extern "C" int lbl_eu_805765B0[10];

// Shared singleton accessor; refs resolve to the unmangled retail name.
extern "C" CfGimmickGlobal* getUnk80664658();

// Bdat table data used by func_8020A608 / func_80208F34 columns.
extern "C" u8 lbl_eu_805357E8[];
extern "C" void* lbl_eu_80664148;   // .sbss - current bdat file pointer
// Returned when func_8020A608 cannot fetch a column row.
extern "C" void* lbl_eu_80662788;

// Column-capacity helpers (CBdat row begin/count).
extern "C" void* func_8003AA34();
extern "C" u32 func_8003B41C(void* bdat);
extern "C" u32 func_8003B1EC(void* bdat);

// Player per-heal helpers paired with CfObject_UnkVirtualFunc70.
extern "C" void func_800BC3B0(cf::CfObjectMove* player, float value);
extern "C" void func_800BC3D8(cf::CfObjectMove* player, float value);

// Scale factor for bdat int -> float position conversion.
extern "C" float lbl_eu_80668364;
// Sentinel used by the player loops (func_8020A124 / func_8020A1DC).
extern "C" float lbl_eu_80668350;

// Bdat column/row reader (returns the raw cell value as a 32-bit word).
extern "C" u32 getBdatStringColumnValue(void* bdat, const char* column, int index);

namespace cf {
    void CfGimmick::func_8020896C(void* other) {
        void* cur = *(void**)((char*)this + 0x78);
        if (cur != other) return;
        *(void**)((char*)cur + 0xB0) = 0;
        *(void**)((char*)this + 0x78) = 0;
    }

    void CfGimmick::func_80208988() {
        extern void func_802089BC(void*, void*, void*);
        func_802089BC((char*)this + 0x1c, (char*)this + 4, (char*)this + 0x10);
    }

    int CfGimmick::func_8020A8AC() { return 1; }
}

cf::CfGimmick::CfGimmick() {
    field_64 = 0;
    field_80 = 0;
    field_66 = 0;
    field_68 = 0;
    field_6A = 0;
    field_6C = 0;
    field_6E = 0;
    field_78 = 0;
    field_7C = 0;
    field_73 = 0;
    field_72 = 0;
    field_71 = 0;
    field_70 = 0;
    field_74 = 0;
}

cf::CfGimmick::~CfGimmick() {}

// func_801BFDE8(u32 mode, u32 value, u32 playerValue, float first, float second)
extern "C" void func_801BFDE8(unsigned int mode, unsigned int value,
                               unsigned int playerValue, float first, float second);
// Sound constants loaded in func_80208C48 / func_80208C60 (retail .sda21 loads)
extern "C" float lbl_eu_80668358;
extern "C" float lbl_eu_8066835C;
extern "C" unsigned int func_80124B78();
void func_802089BC(){}

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

bool func_80208EDC() { return false; }

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

void func_80209020(){}

void func_8020915C(){}

void func_80209288(){}

void func_8020938C(){}

void func_80209488(){}

void func_802095D8(){}

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

void func_802098EC(){}

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
    void* bdat = lbl_eu_80664148;
    if (index != 0 && bdat != 0) {
        func_8003AA34();
        if (index < (int)(func_8003B41C(bdat) + func_8003B1EC(bdat))) {
            // Patch the column-name prefix, then read the requested row.
            u8* col = *(u8**)(lbl_eu_805357E8 + 0x44);
            col[4] = (u8)(mod + 0x31);
            return (void*)getBdatStringColumnValue(
                bdat, *(char**)(lbl_eu_805357E8 + 0x44), index);
        }
    }
    return lbl_eu_80662788;
}

void func_8020A6B0(){}

int func_8020A87C(u32 arg) {
    u32 a = lbl_eu_806646B4;
    if (a == 0) return 0;
    if ((lbl_eu_806646BC & 1) == 0) return 0;
    return (a == arg) ? 1 : 0;
}

int func_8020A8B4(CfGimmick* self, const CfGimmickVec3* point, const CfGimmickVec3* center) {
    float dx = center->x - point->x;
    float dz = center->z - point->z;
    float r = self->field_30;
    if (dx * dx + dz * dz <= r * r) {
        float y = center->y + self->field_34;
        if (y <= point->y && center->y + self->field_38 >= point->y) {
            return 1;
        }
    }
    return 0;
}

void func_8020A928(){}

int func_8020A9F4(CfGimmick* self, const CfGimmickVec3* point, const CfGimmickVec3* center) {
    float r = self->field_30;
    if (center->x + r <= point->x && center->x - r >= point->x &&
        center->z + self->field_3C <= point->z && center->z - self->field_3C >= point->z &&
        center->y + self->field_34 <= point->y && center->y + self->field_38 >= point->y) {
        return 1;
    }
    return 0;
}

void func_8020AA8C(){}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
extern "C" void sinit_8020AB7C() {}
