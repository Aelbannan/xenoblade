#include "kyoshin/cf/CArtsSet.hpp"
#include "kyoshin/cf/CfMapItemManager.hpp"
#include <monolib/device/CDeviceVI.hpp>

#include "kyoshin/cf/CArtsParam.hpp"
#include <nw4r/math/math_triangular.h>
#include <nw4r/math/math_arithmetic.h>
#include <nw4r/db/db_assert.h>

// Minimal local view of cf::CBattleManager. The full CBattleManager.hpp pulls
// in the chain sub-object headers (CChain.hpp etc.), which are currently
// mid-refactor and do not parse in this TU. Only the members used by
// func_80154280 are declared; offsets match the full header layout
// (vtable at +0, unk20C8 at +0x20C8). getInstance() resolves to the real
// mangled symbol getInstance__Q22cf14CBattleManagerFv at link time.
namespace cf {
class CBattleManager {
public:
    static CBattleManager* getInstance();
    u8 _pad20C8[0x20C8];  // 0x00-0x20C7
    u32 unk20C8;          // 0x20C8
};
}

// Free-function imports previously supplied by the trimmed chain headers
// (CChainTimer.hpp / CfObjectImplEne.hpp / CBattleManager.hpp). extern "C"
// keeps the call-site relocs at the retail unmangled names.
extern "C" int func_80154280(void* param, void* actor, int flags);
extern "C" int func_80148778(void* obj, int id);
extern "C" void func_80043D90(void* holder);
extern "C" void* func_80043F18(void* holder);
extern "C" void __dt__80043E88(void* holder, int tags);
extern "C" void func_800F4A98(void* list, u32 type, u32 filter);
extern "C" void* func_800F6EAC(void* list, u32 idx);
extern "C" void* func_8016FE34(void* src);

// float constants (retail .sdata2 pool) used by the effect-dispatch paths
extern const float lbl_eu_805019A8[6];  // arts-speed table (func_801540C0)
extern const float lbl_eu_80667460;
extern const float lbl_eu_80667464;
extern const float lbl_eu_80667468;
extern const float lbl_eu_8066746C;
extern const float lbl_eu_80667470;
extern const float lbl_eu_80667474;
extern const float lbl_eu_80667478;
extern const float lbl_eu_8066747C;
extern const float lbl_eu_80667480;
extern const float lbl_eu_80667484;
extern const float lbl_eu_80667488;
extern const float lbl_eu_8066748C;
extern const float lbl_eu_8066A1F8;
extern const float lbl_eu_8066A1FC;
extern const float lbl_eu_8066A210;

namespace cf {
    // Manual dispatch tables installed at CAttackParam+0x84 by the ctor
    // (retail sinit/ctor store these blob labels there and call slot +0x8
    // through them via bctrl - the class's virtual-dispatch scheme).
    extern "C" void* lbl_eu_8052F610[];
    extern "C" void* lbl_eu_8052F5E8[];

    typedef void (*AttackParamSlot)(CAttackParam* self);

    CArtsParam lbl_80577580;

    CAttackParam::CAttackParam(){
        unk84 = lbl_eu_8052F610;
        unk0 = 0;
        unk20 = 0;
        unk78 = 0;
        ((AttackParamSlot*)unk84)[2](this);
    }

    CArtsParam::CArtsParam(){
        unk84 = lbl_eu_8052F5E8;
        ((AttackParamSlot*)unk84)[2](this);
    }

    void CArtsParam::CArtsParam_UnkVirtualFunc1(){
        CAttackParam::CAttackParam_UnkVirtualFunc1();
        unk88 = 0;
    }

    void CArtsParam::CArtsParam_UnkVirtualFunc3(u8 r4){
        if(unk88 != nullptr){
            unk0 = r4;
        }
    }

void cf::CArtsParam::CArtsParam_UnkVirtualFunc2(){
    // NOTE: retail returns *(u8*)unk88 (or unk2A) in r3, but the
    // (read-only) CArtsSet.hpp declares this slot as returning void,
    // so the loaded byte cannot be returned here.
}
}

extern "C" int func_8015403C(int x) {
    // Returns 2 when (unsigned)(x-4) <= 5, else 1.
    // Retail shape: subi/cmplwi/ble with two separate li/blr blocks; a
    // dense-case switch is the only form that survives MWCC's branchless
    // range-check fold.
    // Wall: retail keeps two li/blr blocks after subi/cmplwi/ble; every
    // high-level form either folds branchless or fuses into a conditional
    // return. This variant is the closest (6 insn diff).
    int ret = 1;
    if ((unsigned)(x - 4) <= 5) {
        ret = 2;
    }
    return ret;
}
float func_80154058(const u8* this_) { unsigned short raw = *(const unsigned short*)(this_ + 0x5a); int value = raw; if (value < 0) value = 0; else if (value > 4) value = 4; extern const float lbl_eu_80501978[]; return lbl_eu_80501978[value]; }
void func_8015408C(){}
extern const float lbl_eu_805019C0[];
float func_80154134(const u8* self) {
    int index = *reinterpret_cast<const unsigned short*>(self + 0x5a);
    if (index < 0)
        index = 0;
    else if (index > 5)
        index = 5;
    return lbl_eu_805019C0[index];
}
int func_80154168(const u8* self) { const unsigned short value = *reinterpret_cast<const unsigned short*>(self + 0x5c); return value == 3 || value == 4 || value == 11; }
bool func_8015419C(u8* self) {
    return *(unsigned short*)(self + 0x5C) == 11;
}

// func_801540C0: per-frame-scaled entry of the arts-speed table @0x805019A8.
// The scale chain is applied before the index clamp so MWCC keeps the
// constant loads/multiplies ahead of the branch (matching retail order).
float func_801540C0(const u8* self) {
    int idx = *(const unsigned short*)(self + 0x5a);
    float scale = CDeviceVI::getSecPerFrame() * lbl_eu_80667468;
    scale *= lbl_eu_8066746C;
    scale = lbl_eu_80667464 * scale;
    if (idx < 0) {
        idx = 0;
    } else if (idx > 5) {
        idx = 5;
    }
    return lbl_eu_805019A8[idx] * scale;
}

// func_8015408C: clamp the u16 at +0x5a to [0,5] (retail keeps the dead
// signed <0 test: lhz + cmpi/bge), then index the arts table.
extern "C" float func_8015408C(const u8* self) {
    int value = *(const u16*)(self + 0x5a);
    if (value < 0) {
        value = 0;
    } else if (value > 5) {
        value = 5;
    }
    extern const float lbl_eu_80501990[];
    return lbl_eu_80501990[value];
}

// ---------------------------------------------------------------------------
// func_801541B0 - arts usability gate.
// Runs the full arts check (func_80154280) and rejects the arts unless the
// caller explicitly allowed every modifier flag the check may have set.
// ---------------------------------------------------------------------------
int func_801541B0(void* param, void* actor, int flags) {
    int result = func_80154280(param, actor, flags);
    if (result & 1) return 0;
    if (!(flags & 0x200) && (result & 0x80)) return 0;
    if (!(flags & 0x20) && (result & 0x20)) return 0;
    if (!(flags & 0x2) && (result & 0x2)) return 0;
    if (!(flags & 0x4) && (result & 0x4)) return 0;
    if (!(flags & 0x8) && (result & 0x8)) return 0;
    if (!(flags & 0x10) && (result & 0x10)) return 0;
    return 1;
}

// func_801554DC - inverted-flags arts gate: mask to 0xFFF bits, invert, run
// the full arts check, and return 1 only when the check returned 0.
extern "C" int func_801554DC(void* param, void* actor, int flags) {
    int result = func_80154280(param, actor, ~(flags & 0xFFF));
    return result == 0;
}

// ---------------------------------------------------------------------------
// Data imports for func_80154280 (global scope: MWCC does not mangle these).
// ---------------------------------------------------------------------------
extern const u64 lbl_eu_805019D8[12];     // arts-select battle table (0x60 B)
extern const char lbl_eu_80526324[];      // warning file name
extern const char lbl_eu_80526300[];      // warning message
extern f64 lbl_eu_80667490;               // 2^52 (u32 -> float magic)
extern f64 lbl_eu_80667498;               // 2^31 magic (s32 -> float)
extern u16 lbl_eu_80663E42;               // current area id
// lbl_eu_8066746C / 80667470 / 80667474 / 80667478 / 8066747C / 80667480 /
// 80667484 / 80667488 / 8066748C and lbl_eu_8066A1F8 / 8066A1FC / 8066A210
// are declared by headers already in the include chain (CfObjectModel.hpp /
// CfCamEvent_1.hpp etc.).

// C-linkage-less imports (retail symbols are unmangled but no header declares
// them with C linkage; declared here with C++ linkage for compilation - reloc
// names at these call sites drift until the headers are updated).
extern int func_801B202C();
extern int func_801B1D4C();
extern int func_801B1FA4();
extern int func_801B1CB0();
extern int func_80260264(void* self, int id, void* out);
extern void* func_801491A4(void* self, unsigned int id);
extern void* func_800F6D50(void* list, int index);
extern int func_8004DAC4(void* obj);
extern int func_8009CF8C(u32 resource);
extern void func_800FB270(void* list, void* vec, f32 a, f32 b, f32 c, int d);
extern void func_800FB5AC(void* list, void* vec, f32 a, f32 b, f32 c, f32 d, f32 e, int f);
extern void* __ct__800FB044(void* list, f32 a, void* vec, int d);
extern void* __ct__800FAE3C(void* list, void* vec, int d);
extern void* __ct__800FBA18(void* list, f32 a, f32 b, f32 c, int d);
extern int func_804B19CC(void* self, void* src, int a, int b);

// ---------------------------------------------------------------------------
// func_80154280 - arts usability check. Accumulates a flag word describing
// which gates the arts fails (bit 1 = out of range, 2 = ?, 8 = ?, 0x10 = ?,
// 0x20 = no valid target, 0x40 = ?, 0x80 = ?, 0x100 = ?). Returns the raw
// flag word; callers (func_801541B0 / func_801554DC) interpret it.
// ---------------------------------------------------------------------------
int func_80154280(void* param, void* actor, int flags) {
    cf::CArtsParam* p = (cf::CArtsParam*)param;            // r24
    cf::CArtsParamActor* a = (cf::CArtsParamActor*)actor;  // r25
    cf::CArtsParamActor* obj = (cf::CArtsParamActor*)a->v38();                   // r30
    int result = 0;                                        // r31
    if (p->unk20 == 0) return 1;
    if (a == 0) return 0xfff;

    // Gate: arts-hold flag (0x40) - weapon/arts list checks.
    if (!(flags & 0x40)) {
        if (p->unk78 & 2) {
            if (((p->unk78 & 0x4000) != 0 || (p->unk78 & ~1u) != 0) ||
                func_80148778((u8*)a + 8, 0xc) == 0) {
                if (func_80148778((u8*)a + 8, 9) != 0 || func_80148778((u8*)a + 8, 0xb) != 0) {
                    result |= 0x40;
                }
            } else {
                result |= 0x40;
            }
        }
    }

    // Gate: arts-hold flag (0x80) - attack type / voice-action checks.
    if (!(flags & 0x80)) {
        if (p->unk78 & 2) {
            if (func_80148778((u8*)a + 8, 0xcb) == 0 &&
                func_80148778((u8*)a + 8, 0xf8) == 0 &&
                func_80148778((u8*)a + 8, 0xf) == 0 &&
                func_80148778((u8*)a + 8, 0x10) == 0) {
                u32 val = *(u32*)a->field_04->f30();
                if (func_80174C98(a, (int*)&val, 0x18) == 0 && a->v173() != 0) {
                    result |= 0x80;
                }
            } else {
                result |= 0x80;
            }
        }
    }

    // Gate: arts-hold flag (0x20) - target / battle-state checks.
    if (!(flags & 0x20)) {
        if (obj->field_3F00 & 2) {
            if (obj->field_3F28 == 7) {
                if ((p->unk78 & 1) && p->unk77 == 0x10) {
                    if (func_801B202C() == 0) {
                        result |= 0x80;
                    } else {
                        int chain = func_801B1D4C();
                        if (chain == -1) {
                            result |= 0x20;
                        } else {
                            for (int i = 1; i < 8; i++) {
                                u8* rec = (u8*)obj->v156() + 0x8f8 + i * 0x8c;
                                if (chain == *(u16*)(rec + 0x40)) {
                                    result |= func_80154280(rec, obj, -0x21);
                                    break;
                                }
                            }
                        }
                    }
                }
            } else {
                // Battle-manager / arts-select table path.
                cf::CBattleManager* bm = cf::CBattleManager::getInstance();
                if (*(u32*)&bm->unk20C8 == 0) {
                    u64 table[12];
                    for (int i = 0; i < 12; i++) table[i] = lbl_eu_805019D8[i];
                    if (p->unk5C != 2) {
                        f32 f30;
                        f32 f31 = p->unk60;
                        int r28 = p->unk5E;
                        f32 f27 = 0.0f;
                        u8 holder[8];
                        func_80043D90(holder);
                        f30 = (f32)p->unk64 * lbl_eu_8066A210;
                        int idx = (obj->field_3F00 & 2) ? 0 : 4;
                        int r29 = *(u32*)((u8*)table + 4 + p->unk5C * 8 + idx);
                        if (obj->v163() != 0) {
                            int sv;
                            if (func_80260264(obj->v163(), 0x77, &sv) != 0) {
                                if (p->unk78 & 2) {
                                    if (f31 <= lbl_eu_80667470) {
                                        f31 += (f32)(s32)sv;
                                        if (f31 > lbl_eu_80667474) f31 = lbl_eu_80667474;
                                    }
                                }
                            }
                        }
                        if (!(p->unk78 & 2)) {
                            void* entry = func_801491A4((u8*)obj + 8, 0x95);
                            if (entry != 0) {
                                r28 = 1;
                                f30 = (f32)*(u32*)((u8*)entry + 0x10) * lbl_eu_8066A210;
                            }
                        }
                        u32 sv2;
                        if (func_80174C98(obj, (int*)&sv2, 0x806) != 0) {
                            f31 *= lbl_eu_80667478;
                        }
                        if (!(p->unk78 & 2)) {
                            f31 = obj->v110()[0];
                        }
                        f32 vec70[3];
                        void* vpos = obj->mSub.s73();
                        if (vpos != 0) {
                            vec70[0] = *(f32*)((u8*)vpos + 0xc);
                            vec70[1] = *(f32*)((u8*)vpos + 0x1c);
                            vec70[2] = *(f32*)((u8*)vpos + 0x2c);
                        } else {
                            vec70[0] = ((f32*)obj->mSub.s41())[0];
                            vec70[1] = ((f32*)obj->mSub.s41())[4];
                            vec70[2] = ((f32*)obj->mSub.s41())[8];
                        }
                        vec70[1] = vec70[1] - (vec70[1] - ((f32*)obj->mSub.s41())[4]) * lbl_eu_8066746C;
                        f27 = vec70[1] - ((f32*)obj->mSub.s41())[4];
                        if (f27 != 0.0f) {
                            f32 len2 = f27 * f27 + f31 * f31;
                            if (len2 <= 0.0f) {
                                nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
                            }
                            if (f27 * f27 + f31 * f31 <= 0.0f) {
                                f31 = 0.0f;
                            } else {
                                f31 = (f27 * f27 + f31 * f31) * nw4r::math::FrSqrt(f27 * f27 + f31 * f31);
                            }
                        }
                        cf::CBattleManager* bm2 = cf::CBattleManager::getInstance();
                        u8 chainByte = ((u8*)bm2)[0x1AA];
                        if (chainByte >= 1 && chainByte <= 0x18) {
                            vec70[0] = ((f32*)obj->mSub.s41())[0];
                            vec70[1] = ((f32*)obj->mSub.s41())[4];
                            vec70[2] = ((f32*)obj->mSub.s41())[8];
                        }
                        int r27 = 8;
                        if (!(obj->field_3F00 & 2)) r27 = 0;
                        if (obj->field_3F00 & 4) {
                            switch (obj->field_3F28) {
                            case 0x8b3: case 0x8b4: case 0x8a0: case 0x6c5:
                            case 0x5f2: case 0x607: case 0x60b: case 0x11c:
                            case 0x9c5:
                                r27 = 0;
                                break;
                            default:
                                break;
                            }
                        }
                        switch (r28) {
                        case 1: {
                            void* list = func_80043F18(holder);
                            func_800F4A98(list, r29, 0x1000);
                            f32 f28 = obj->v364();
                            list = func_80043F18(holder);
                            func_800FB270(list, vec70, f31, f30, f28, r27);
                            list = func_80043F18(holder);
                            __ct__800FB044(list, f31, vec70, 0);
                            break;
                        }
                        case 2: {
                            void* list = func_80043F18(holder);
                            func_800F4A98(list, r29, 0x1000);
                            list = func_80043F18(holder);
                            __ct__800FB044(list, f31, vec70, 0);
                            f32 f28 = obj->v364() + lbl_eu_8066A1F8;
                            list = func_80043F18(holder);
                            func_800FB270(list, vec70, f31, f30, f28, r27);
                            break;
                        }
                        case 3: {
                            void* list = func_80043F18(holder);
                            func_800F4A98(list, r29, 0x1000);
                            f32 v1 = obj->v364();
                            f32 f28 = v1 + lbl_eu_8066A1F8 * lbl_eu_8066746C;
                            f32 f29 = v1 - lbl_eu_8066A1F8 * lbl_eu_8066746C;
                            list = func_80043F18(holder);
                            func_800FB5AC(list, vec70, f31, f30, f28, f30, f29, r27);
                            list = func_80043F18(holder);
                            __ct__800FB044(list, f31, vec70, 0);
                            break;
                        }
                        case 4: {
                            void* list = func_80043F18(holder);
                            func_800F4A98(list, r29, 0x1000);
                            list = func_80043F18(holder);
                            __ct__800FB044(list, f31, vec70, r27);
                            break;
                        }
                        case 5: {
                            void* list = func_80043F18(holder);
                            func_800F4A98(list, r29, 0x1000);
                            f32 f27b = (f32)p->unk64 / lbl_eu_8066747C;
                            f32 f28 = obj->v364();
                            list = func_80043F18(holder);
                            __ct__800FBA18(list, f31, f27b, f28, r27);
                            list = func_80043F18(holder);
                            __ct__800FB044(list, f31, vec70, 0);
                            break;
                        }
                        case 6: {
                            if (p->unk5C == 3 || p->unk5C == 0xb) {
                                void* list = func_80043F18(holder);
                                func_800F4A98(list, r29, 0x1000);
                            } else {
                                void* r28b = obj->mSub.s17();
                                void* list = func_80043F18(holder);
                                func_800F6D50(list, (int)r28b);
                            }
                            void* list = func_80043F18(holder);
                            __ct__800FB044(list, f31, vec70, r27);
                            break;
                        }
                        case 7: {
                            void* list = func_80043F18(holder);
                            func_800F4A98(list, r29, 0x1000);
                            break;
                        }
                        default: {
                            if (p->unk5C == 3 || p->unk5C == 0xb) {
                                void* list = func_80043F18(holder);
                                func_800F4A98(list, r29, 0x1000);
                            } else {
                                void* r28b = obj->mSub.s17();
                                void* list = func_80043F18(holder);
                                func_800F6D50(list, (int)r28b);
                            }
                            void* list = func_80043F18(holder);
                            __ct__800FB044(list, f31, vec70, r27);
                            break;
                        }
                        }
                        if (p->unk78 & 0x8000) {
                            // positional offset effect (paired-single math)
                            f32 base[3];
                            base[0] = lbl_eu_80667460;
                            base[1] = lbl_eu_80667480;
                            base[2] = lbl_eu_80667460;
                            f32* off = (f32*)obj->mSub.s41();
                            f32 sum[3];
                            sum[0] = off[0] + base[0];
                            sum[1] = off[4] + base[4];
                            sum[2] = off[8] + base[8];
                            void* list = func_80043F18(holder);
                            __ct__800FAE3C(list, sum, 0);
                        }
                        int r29b = 0;
                        int r23 = 0;
                        int r22 = 0;
                        void* list = func_80043F18(holder);
                        while (r22 < *(u32*)((u8*)list + 0x620)) {
                            void* list2 = func_80043F18(holder);
                            void* target = func_800F6EAC(list2, r22);
                            if (target != 0 && func_8016FE34(target) != 0) {
                                if (!(p->unk78 & 2)) {
                                    f32 f27b = nw4r::math::Atan2FIdx(
                                        *(f32*)((u8*)target + 0x3e9c + 0x28 + 0x4) - ((f32*)obj->mSub.s41())[4],
                                        *(f32*)((u8*)target + 0x3e9c + 0x28) - ((f32*)obj->mSub.s41())[0]) * lbl_eu_80667484;
                                    f27b -= obj->v363();
                                    while (lbl_eu_8066A1F8 <= f27b) f27b -= lbl_eu_8066A1FC;
                                    while (f27b < -lbl_eu_8066A1F8) f27b += lbl_eu_8066A1FC;
                                    f32* tpos = (f32*)obj->mSub.s41();
                                    if (func_804B19CC((u8*)target + 0x44a8, tpos, 0, 1) == 0) {
                                        f32 lim = obj->v111()[0];
                                        if (-(lim * lbl_eu_8066746C) <= f27b && f27b <= lim * lbl_eu_8066746C) {
                                            // in range
                                        } else {
                                            r23 = 1;
                                        }
                                    }
                                }
                                if (p->unk5C == 0xb) {
                                    if (target != obj && a->v173() == 0) r29b++;
                                } else if (p->unk5C == 3) {
                                    if (a->v173() == 0) {
                                        r23 = 1;
                                        r29b++;
                                    }
                                } else {
                                    if (obj->mSub.s17() == *(void**)((u8*)target + 0x3f10)) r23 = 1;
                                    r29b++;
                                }
                            }
                            r22++;
                        }
                        if ((obj->field_3F00 & 4) && r23 == 0 && p->unk5C != 0xb) {
                            result |= 0x20;
                        }
                        if (r29b == 0) result |= 0x20;
                        __dt__80043E88(holder, -1);
                    }
                }
            }
        }
    }

    // Gate: range flag (0x2) - arts range vs distance.
    if (!(flags & 0x2)) {
        if (p->unk80 > lbl_eu_80667460) result |= 0x2;
    }
    // Gate: range flag (0x4).
    if (!(flags & 0x4)) {
        if (p->unk7C > lbl_eu_80667460) result |= 0x80;
    }
    // Gate: range flag (0x8).
    if (!(flags & 0x8)) {
        if (func_80148778((u8*)a + 8, 0x30) == 0 && lbl_eu_80667460 > a->v78()) {
            result |= 0x8;
        }
    }
    // Gate: range flag (0x10).
    if (!(flags & 0x10)) {
        if (func_80148778((u8*)a + 8, 0x31) == 0) {
            int dmg = (int)(s16)p->unk34;
            if (dmg > 0 && (f32)dmg > a->v84()) {
                result |= 0x10;
            } else if (p->unk78 & 0x40) {
                if (a->v84() < lbl_eu_80667488) result |= 0x10;
            }
        }
    }
    // Battle-object sub-state gate (0x100).
    if (obj->field_3F60 != 0 && func_8004DAC4(obj->field_3F60) != 0) {
        int st = func_8004DAC4(obj->field_3F60);
        if (p->unk78 & 2) {
            if (st >= 0x10 && st <= 0x1f && st - 0x10 == (int)p->unk76) {
                result |= 0x100;
            }
        } else if ((p->unk78 & 1) && st >= 7 && st <= 0xe && st - 7 == (int)p->unk76) {
            result |= 0x100;
        }
    }

    // Gate: arts-hold flag (0x200) - arts-id specific checks.
    if (!(flags & 0x200)) {
        if ((obj->field_3F00 & 2) && obj->field_3F28 == 1) {
            if (p->unk78 & 1) {
                if (p->unk48 == 0xea) {
                    if (a->v84() < lbl_eu_8066747C) result |= 0x80;
                    int kind = func_8009CF8C(0x20);
                    if (kind > 0x108 && kind < 0x116 && (lbl_eu_80663E42 & 0x1F000) != 0) {
                        result |= 0x80;
                    }
                }
            } else if (p->unk78 & 2) {
                if (p->unk78 & 0x4000) {
                    int kind = func_8009CF8C(0x20);
                    if (kind > 0x108 && kind < 0x116 && (lbl_eu_80663E42 & 0x1F000) != 0) {
                        result |= 0x80;
                    }
                } else {
                    if (p->unk48 == 0x10d && a->v72() < lbl_eu_80667478) result |= 0x80;
                }
            }
        }
        if ((obj->field_3F00 & 2) && obj->field_3F28 == 2) {
            if (p->unk48 == 0xff && *(u32*)((u8*)obj + 0x1530) == 0) result |= 0x80;
            if (func_80148778((u8*)obj + 8, 0xeb) != 0 && p->unk48 != 0xeb) result |= 0x80;
        }
        if ((obj->field_3F00 & 2) && obj->field_3F28 == 5) {
            int st = obj->field_3F60 ? *(u32*)((u8*)obj->field_3F60 + 0x374) : 0;
            if (st == 0xe) result |= 0x80;
            int st2 = obj->field_3F60 ? *(u32*)((u8*)obj->field_3F60 + 0x374) : 0;
            if (st2 == 0xd) result |= 0x80;
            if (a->v84() >= lbl_eu_8066747C) result |= 0x80;
        }
        if ((obj->field_3F00 & 2) && obj->field_3F28 == 6) {
            if (p->unk48 == 0xf8) {
                u8 holder2[8];
                func_80043D90(holder2);
                void* list = func_80043F18(holder2);
                func_800F4A98(list, 0x20, 0x800);
                list = func_80043F18(holder2);
                if (*(u32*)((u8*)list + 0x620) <= 1) result |= 0x80;
                __dt__80043E88(holder2, -1);
            }
            if (p->unk48 == 0x119 && obj->v192() < 3) result |= 0x80;
        }
        if ((obj->field_3F00 & 2) && obj->field_3F28 == 7) {
            if (p->unk78 & 1) {
                u32 sv3;
                if (func_80174C98(obj, (int*)&sv3, 0xb) != 0) result |= 0x80;
                if (func_801B202C() == 0) result |= 0x80;
            } else if (p->unk78 & 2) {
                if (p->unk3C == 4) {
                    if (func_801B1FA4() == -1) result |= 0x80;
                } else if (p->unk77 == 0xb) {
                    if (a->v74() <= lbl_eu_8066748C) result |= 0x80;
                } else if (p->unk77 >= 0xe && p->unk77 <= 0xf) {
                    if (func_80148778((u8*)obj + 8, 0x10a) == 0) result |= 0x80;
                } else if (p->unk77 == 6) {
                    if (func_801B1CB0() == 0 || func_801B1FA4() == -1) {
                        result |= 0x80;
                    } else {
                        int chain2 = func_801B1CB0();
                        int found = 0;
                        u8* base2 = (u8*)obj->v156();
                        for (int row = 0; row < 2; row++) {
                            u8* rowp = base2 + row * 0x460;
                            for (int col = 0; col < 8; col++) {
                                u8* rec = rowp + 0x38 + col * 0x8c;
                                if (*(u16*)(rec + 0x3c) == 4 && *(u16*)(rec + 0x40) == (u16)chain2) {
                                    found = 1;
                                }
                            }
                        }
                        if (found == 0) result |= 0x80;
                    }
                }
            }
        }
        if ((obj->field_3F00 & 2) && obj->field_3F28 == 8) {
            if (p->unk77 == 0xf && obj->v192() != 4) result |= 0x80;
        }
    }

    return result;
}

// cf::CActorParam base virtual (retail: li r3,0; blr)
extern "C" u32 CActorParam_UnkVirtualFunc3__Q22cf11CActorParamFv() { return 0; }

void cf::CArtsParam::vtableFunc3(u8 val) {
    u8* o = *(u8**)((u8*)this + 0x88);
    if (o) *(u8*)((u8*)o + 0) = val;
}
