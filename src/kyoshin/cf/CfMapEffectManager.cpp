// Decompiled unit for kyoshin/cf/CfMapEffectManager (retail .text
// 0x80170104-0x80171264). The constructor is written as a free function
// named __ct__cf_CfMapEffectManager because that is the retail symbol for
// this unit (the harness matches it by name); the vtable is stored manually
// to reproduce the retail reloc to lbl_eu_80531080.

#include "kyoshin/cf/CfMapEffectManager.hpp"
#include "kyoshin/cf/object/CfObject.hpp"
#include "kyoshin/cf/object/CfObjectEff.hpp"
#include "kyoshin/cf/CfMapMineManager.hpp" // extern "C" func_800B8920/800B9404/8003B1EC/8003B41C
#include "monolib/device/CDeviceSC.hpp"   // CDeviceSC::getLanguage
#include <string.h>

namespace cf {
// Local minimal view of cf::CfGameManager used by func_8016F9D4. The full
// class in include/kyoshin/cf/CfGameManager.hpp declares func_80086DA4 as
// void, but retail returns a value (its body is a tail call to
// func_8006A37C) and this caller truncates it to u16.
class CfGameManager {
public:
    static u32 func_80086DBC();
    static u32 func_80086DA0();
    static u32 func_80086DA4();
    static u32 func_80086B1C();
    static u32 func_80086B2C();
    static void* func_80083298();
};
} // namespace cf

using namespace cf;

// retail: __ct__cf_CfMapEffectManager (0x80170104)
CfMapEffectManager* __ct__cf_CfMapEffectManager(CfMapEffectManager* pSelf) {
    pSelf->field_0x0 = lbl_eu_80531080;
    pSelf->field_0x4 = -1;
    pSelf->field_0x6 = -1;
    pSelf->field_0x8 = -1;
    pSelf->field_0xA = 0xF;
    memset(pSelf->field_0x0C, 0, sizeof(pSelf->field_0x0C));
    memset(pSelf->field_0x9C, 0, sizeof(pSelf->field_0x9C));
    memset(pSelf->field_0xE4, 0, sizeof(pSelf->field_0xE4));
    pSelf->field_0x12C = -1;
    pSelf->field_0x12E = -1;
    pSelf->field_0x130 = -1;
    pSelf->field_0x134 = 0;
    pSelf->field_0x138 = 1;
    lbl_eu_80664290 = pSelf;
    return pSelf;
}

// retail: __dt__Q22cf18CfMapEffectManagerFv (0x801701AC). The vtable store
// and the singleton clear are manual (lbl_eu_80531080 / lbl_eu_80664290); the
// null-check and delete-flag handling are the MWCC destructor skeleton.
cf::CfMapEffectManager::~CfMapEffectManager() {
    field_0x0 = lbl_eu_80531080;
    func_8016EF2C(this);
    CfMapEffectHandle* obj = field_0x134;
    field_0x12C = -1;
    field_0x12E = -1;
    field_0x130 = -1;
    if (obj != 0) {
        if (func_800B8920(obj) != 0) {
            field_0x134->field_0xB0 = 0;
            func_800B9404(field_0x134);
            field_0x134 = 0;
        }
        field_0x134 = 0;
    }
    lbl_eu_80664290 = 0;
}

// retail: func_8016EEB0 (0x80170264)
extern "C" void func_8016EEB0(CfMapEffectManager* pSelf) {
    func_8016EF2C(pSelf);
    CfMapEffectHandle* obj = pSelf->field_0x134;
    pSelf->field_0x12C = -1;
    pSelf->field_0x12E = -1;
    pSelf->field_0x130 = -1;
    if (obj != 0) {
        if (func_800B8920(obj) != 0) {
            pSelf->field_0x134->field_0xB0 = 0;
            func_800B9404(pSelf->field_0x134);
            pSelf->field_0x134 = 0;
        }
        pSelf->field_0x134 = 0;
    }
}

// retail: func_8016EF2C (0x801702E0) — reset the effect-object slots and the
// two per-slot s16 counter banks. Same-TU definition; linkage (C) is inherited
// from the extern "C" declaration in the header so calls keep the retail name.
void func_8016EF2C(CfMapEffectManager* pSelf) {
    pSelf->field_0x4 = -1;
    pSelf->field_0x6 = -1;
    pSelf->field_0xA = 0xF;
    for (int i = 0; i < 0x24; i++) {
        CfObject* obj = pSelf->field_0x0C[i];
        if (obj != 0) {
            if (func_800B8920(obj) != 0) {
                reinterpret_cast<CfMapEffectObject*>(obj)->field_0xB0 = 0;
                func_800B9404(pSelf->field_0x0C[i]);
                pSelf->field_0x0C[i] = 0;
            }
            pSelf->field_0x0C[i] = 0;
        }
        reinterpret_cast<s16*>(pSelf->field_0x9C)[i] = 0;
        reinterpret_cast<s16*>(pSelf->field_0xE4)[i] = 0;
    }
}

// retail: func_8016EFD8 (0x8017038C) — BDAT range check against the current
// chapter/event counters reported by CfGameManager.
int func_8016EFD8(int unused, int index) {
    u8* bdat = lbl_eu_806640AC;
    u32 v0 = getBdatStringColumnValue(bdat, lbl_eu_80503248, index);
    if ((u16)v0 != 0) {
        u32 cur = (u16)func_80082354__Q22cf13CfGameManagerFv((u16)v0);
        u32 v1 = getBdatStringColumnValue(bdat, lbl_eu_80503248 + 8, index);
        u32 v2 = getBdatStringColumnValue(bdat, lbl_eu_80503248 + 0x11, index);
        if (cur < (u8)v1 || cur > (u8)v2) {
            return 0;
        }
    }
    u32 cur2 = (u16)func_800822F4__Q22cf13CfGameManagerFv();
    u32 v3 = getBdatStringColumnValue(bdat, lbl_eu_80503248 + 0x1A, index);
    u32 v4 = getBdatStringColumnValue(bdat, lbl_eu_80503248 + 0x20, index);
    u32 v5 = getBdatStringColumnValue(bdat, lbl_eu_80503248 + 0x26, index);
    u32 v6 = getBdatStringColumnValue(bdat, lbl_eu_80503248 + 0x2C, index);
    if (cur2 >= (u16)v3 && cur2 <= (u16)v4 && (u16)v4 != 0) {
        return 1;
    }
    if (cur2 >= (u16)v5 && cur2 <= (u16)v6 && (u16)v6 != 0) {
        return 1;
    }
    return 0;
}

extern "C" void func_8016F9D4(CfMapEffectManager* pSelf);
extern "C" void func_8016F140(CfMapEffectManager* self) { func_8016F9D4(self); }

// retail: func_8016F144 (0x801704F8) - scene-attached effect manager. When
// the chapter/event counters change, release the current scene handle and
// create the one matching the new state (BGM/ambient effect ids are
// language-dependent). The 0x16/1 area-id pair disables the feature entirely.
void func_8016F144(CfMapEffectManager* self) {
    cf::CfGameManager::func_80086DBC();
    u16 cur0 = (u16)cf::CfGameManager::func_80086DA0();
    u16 cur1 = (u16)cf::CfGameManager::func_80086DA4();
    int type = 0;
    if (lbl_eu_80663E42 != 0x16 || lbl_eu_80663E44 != 1) {
        cf::CfGameManager::func_80086B2C();
        cf::CfGameManager::func_80086B1C();
        if (self->field_0x12E != cur0 || self->field_0x130 != cur1) {
            if ((int)cur0 == 4 && (int)cur1 == 0x3b) {
                type = 0xb6;
                if (CDeviceSC::getLanguage() >= 1) {
                    type = (CDeviceSC::getLanguage() - 1) * 2 + 0xcf;
                }
                u16 bgm = lbl_eu_80661AF4;
                if (bgm != func_80082694__Q22cf13CfGameManagerFv(0x6f)) {
                    u32 next = func_80082694__Q22cf13CfGameManagerFv(0x81) + 1;
                    func_8008269C__Q22cf13CfGameManagerFv(0x81, next);
                    if (next == 0x16e) {
                        func_800826F0__Q22cf13CfGameManagerFv(0x81);
                    }
                    func_8008269C__Q22cf13CfGameManagerFv(0x6f, lbl_eu_80661AF4);
                }
            } else if ((int)cur0 == 0x12 && (int)cur1 == 0x3b) {
                type = 0xb7;
                if (CDeviceSC::getLanguage() >= 1) {
                    type = (CDeviceSC::getLanguage() - 1) * 2 + 0xd0;
                }
            }
        }
        if (type != 0 && func_8007F91C__Q22cf13CfGameManagerFv() == 0) {
            CfMapEffectHandle* obj = self->field_0x134;
            self->field_0x12C = -1;
            self->field_0x12E = -1;
            self->field_0x130 = -1;
            if (obj != 0) {
                if (func_800B8920(obj) != 0) {
                    self->field_0x134->field_0xB0 = 0;
                    func_800B9404(self->field_0x134);
                    self->field_0x134 = 0;
                }
                self->field_0x134 = 0;
            }
            CfMapEffectHandle* h =
                (CfMapEffectHandle*)func_8008187C__Q22cf13CfGameManagerFv(type);
            self->field_0x134 = h;
            h->field_0xB0 = (u32)self;
        }
        self->field_0x12E = cur0;
        self->field_0x130 = cur1;
    }
}

// retail: func_8016F2A4 (0x801706A0) - per-row map-effect update driven by
// bdat columns: reconcile the effect object at field_0x0C[index] with the
// current event state, rebuild it when the state is "active" and the slot is
// empty, then push the per-frame column data through the effect virtuals.
void func_8016F2A4(CfMapEffectManager* self, int index, u8* bdat) {
    u32 flags = lbl_eu_80663E24;
    F64Conv convLo, convHi;
    convLo.w[0] = 0x43300000;
    convHi.w[0] = 0x43300000;
    // Fast path: event bits set + +0x32 column enabled -> poke the 0x158
    // virtual on the existing effect object and stop.
    if ((flags & 0x2000000) != 0 || (flags & 0x400) != 0) {
        u32 v32 = getBdatStringColumnValue(bdat, lbl_eu_80503248 + 0x32, index);
        if ((u8)v32 != 0) {
            CfObject* obj = self->field_0x0C[index];
            if (obj != 0) {
                obj->CfObject_UnkVirtualFunc66(0);
            }
        }
        return;
    }

    u32 phase = cf::CfGameManager::func_80086DBC();
    u16 area = (u16)cf::CfGameManager::func_80086DA0();
    cf::CfGameManager::func_80086DA4(); // result discarded (retail calls it)
    u16 secs = func_8016DF2C();

    u32 modelCol = getBdatStringColumnValue(bdat, lbl_eu_80503248 + 0x38, index);
    u8 timeCol = (u8)getBdatStringColumnValue(bdat, lbl_eu_80503248 + 0x40, index);
    u8 onceCol = (u8)getBdatStringColumnValue(bdat, lbl_eu_80503248 + 0x48, index);

    CfObject** slot = &self->field_0x0C[index];
    if (*slot != 0 && func_800B8920(*slot) == 0) {
        // Stale handle: drop the object and clear both counter banks.
        *slot = 0;
        self->field_0xE4[index] = 0;
        self->field_0x9C[index] = 0;
    }

    u16 gate = (u16)getBdatStringColumnValue(bdat, lbl_eu_80503248 + 0x4d, index);
    if (gate != 0 && func_8009CF8C(gate + 0x278a) == 0) {
        // Gated off: force idle state (1) unless already applied.
        if (*slot != 0 && self->field_0xE4[index] != 1) {
            func_800ACC14(*slot, 1);
            self->field_0xE4[index] = 1;
            self->field_0x9C[index] = 0;
        }
        return;
    }

    int inRange = func_8016EFD8((int)self, index);
    int timeOk = 1;
    if (timeCol != 0 && (u32)timeCol != (u32)secs) {
        timeOk = 0;
    }

    u16 subs[3];
    subs[0] = (u8)getBdatStringColumnValue(bdat, lbl_eu_80503248 + 0x55, index);
    subs[1] = (u8)getBdatStringColumnValue(bdat, lbl_eu_80503248 + 0x5f, index);
    subs[2] = (u8)getBdatStringColumnValue(bdat, lbl_eu_80503248 + 0x69, index);
    func_8006A6D0();
    int anySub = 0;
    for (int i = 0; i < 3; i++) {
        if (i != 0 && subs[i] == 0) {
            continue;
        }
        int r =
            (subs[i] == 0) ? 1 : func_8016FA68((int)self, (int)phase, (int)area, (int)subs[i]);
        anySub = (anySub | r) != 0;
    }

    int spawn = 0;
    if (inRange != 0 && timeOk != 0 && anySub != 0) {
        spawn = 1;
    }
    if (spawn == 0) {
        if (*slot != 0 && self->field_0xE4[index] != 1) {
            func_800ACC14(*slot, 1);
            self->field_0xE4[index] = 1;
            self->field_0x9C[index] = 0;
        }
        if (onceCol == 0) {
            self->field_0x9C[index] = 0;
        }
        return;
    }

    // Active path: bounce a live object to state 2, then (re)create it.
    if (*slot != 0 && self->field_0xE4[index] == 1) {
        func_800ACC14(*slot, 2);
        self->field_0xE4[index] = 2;
    }
    if (onceCol == 0 && self->field_0x9C[index] != 0) {
        spawn = 0;
    }

    if (*slot == 0 && spawn != 0) {
        u16 flashCol = (u16)getBdatStringColumnValue(bdat, lbl_eu_80503248 + 0x73, index);
        u32 attachCol = getBdatStringColumnValue(bdat, lbl_eu_80503248 + 0x7d, index);
        CfObject* extra = 0;
        if ((u8)attachCol != 0) {
            extra = (CfObject*)func_80186BC8((u8)attachCol);
            if (extra == 0) {
                return;
            }
        }
        float flashArg = lbl_eu_80667710;
        CfObjectEff* newObj =
            (CfObjectEff*)func_800817BC__Q22cf13CfGameManagerFv((u16)modelCol, 0);
        if (newObj != 0) {
            *slot = newObj;
            if (onceCol != 0) {
                self->field_0x9C[index] = area;
            } else {
                self->field_0x9C[index] = 1;
            }
            reinterpret_cast<CfMapEffectObject*>(newObj)->field_0xB0 = (u32)self;

            // Columns +0x84/+0x89/+0x8e: unsigned column -> position floats.
            float pos[3];
            convLo.w[1] =
                getBdatStringColumnValue(bdat, lbl_eu_80503248 + 0x84, index) ^ 0x80000000;
            pos[0] = lbl_eu_80667714 * (float)(convLo.d - lbl_eu_80667720);
            convHi.w[1] =
                getBdatStringColumnValue(bdat, lbl_eu_80503248 + 0x89, index) ^ 0x80000000;
            pos[1] = lbl_eu_80667714 * (float)(convHi.d - lbl_eu_80667720);
            convLo.w[1] =
                getBdatStringColumnValue(bdat, lbl_eu_80503248 + 0x8e, index) ^ 0x80000000;
            pos[2] = lbl_eu_80667714 * (float)(convLo.d - lbl_eu_80667720);

            // Columns +0x93/+0x98/+0x9d: signed column -> floats. Kept as an
            // array so MWCC spills them; the third converts into the arg block.
            float vals[3];
            convHi.w[1] = (u32)(s16)getBdatStringColumnValue(bdat, lbl_eu_80503248 + 0x93,
                                                             index) ^ 0x80000000;
            vals[0] = lbl_eu_80667718 * (float)(convHi.d - lbl_eu_80667720);
            convLo.w[1] = (u32)(s16)getBdatStringColumnValue(bdat, lbl_eu_80503248 + 0x98,
                                                             index) ^ 0x80000000;
            vals[1] = lbl_eu_80667718 * (float)(convLo.d - lbl_eu_80667720);

            float args[3];
            args[0] = vals[0] * lbl_eu_8066A210;
            args[1] = vals[1] * lbl_eu_8066A210;
            convHi.w[1] = (u32)(s16)getBdatStringColumnValue(bdat, lbl_eu_80503248 + 0x9d,
                                                             index) ^ 0x80000000;
            vals[2] = lbl_eu_80667718 * (float)(convHi.d - lbl_eu_80667720);
            args[2] = vals[2] * lbl_eu_8066A210;
            (*(CfObjectEff_VTable39**)newObj)->fn(newObj, pos);
            (*(CfObjectEff_VTable47**)newObj)->fn(newObj, args);
            if (extra != 0) {
                func_800ACF78(newObj, extra, 0);
            }
            void* cam = cf::CfGameManager::func_80083298();
            int camFlag = (*(CfGameManager_VTable101**)cam)->fn(cam);
            func_800AD040((char*)newObj, camFlag);
            newObj->CfObject_UnkVirtualFunc66(self->field_0x138);

            if (flashCol != 0) {
                u32 wa2 = getBdatStringColumnValue(bdat, lbl_eu_80503248 + 0xa2, index);
                if ((u8)wa2 != 0) {
                    u32 wa7 = getBdatStringColumnValue(bdat, lbl_eu_80503248 + 0xa7, index);
                    convLo.w[1] = (u16)wa7;
                    flashArg = (float)(convLo.d - lbl_eu_80667728);
                }
                convHi.w[1] = (u16)flashCol;
                func_800ACC28(newObj, (float)(convHi.d - lbl_eu_80667728), flashArg);
            }
        }
    }

    // Per-frame column push (columns 0x30+i in the scratch-name buffer)
    // while the slot is live. Counter compare is signed in retail.
    if (*slot != 0) {
        for (int i = 1; i <= 8; i++) {
            lbl_eu_806623EC[6] = (u8)(i + 0x30);
            u32 vv = getBdatStringColumnValue(bdat, (const char*)lbl_eu_806623EC, index);
            int r;
            if ((u8)vv == 0) {
                r = 0;
            } else {
                r = func_8016FA68((int)self, (int)phase, (int)area, (int)(u8)vv);
            }
            if (r != 0 && (s16)self->field_0xE4[index] != (s16)i) {
                func_800ACC14(self->field_0x0C[index], (s8)i);
                self->field_0xE4[index] = (u16)i;
            }
        }
    }
}

// retail: func_8016F9D4 (0x80170DD0)
extern "C" void func_8016F9D4(CfMapEffectManager* pSelf) {
    u8* bdat = lbl_eu_806640AC;
    u32 v = cf::CfGameManager::func_80086DBC();
    u16 a = (u16)cf::CfGameManager::func_80086DA0();
    u16 b = (u16)cf::CfGameManager::func_80086DA4();
    u16 c = func_8016DF2C();
    int end;
    int i = (int)func_8003B41C(bdat);
    end = i + (int)func_8003B1EC(bdat);
    for (; i < end; i++) {
        func_8016F2A4(pSelf, i, bdat);
    }
    pSelf->field_0x4 = v;
    pSelf->field_0x6 = a;
    pSelf->field_0x8 = b;
    pSelf->field_0xA = c;
}

extern "C" int func_8016FA68(int, int b, int c, int d) {
    if (d == 1 && b == 1) return 1;
    if (d == 2 && b == 2) return 1;
    if (d == 3 && b == 3) return 1;
    if (d == 4 && b == 4) return 1;
    if (d == 5 && (unsigned int)c <= 2) return 1;
    if (d == 6 && (unsigned int)(c - 3) <= 2) return 1;
    if (d == 7 && (unsigned int)(c - 6) <= 2) return 1;
    if (d == 8 && (unsigned int)(c - 9) <= 2) return 1;
    if (d == 9 && (unsigned int)(c - 0xc) <= 2) return 1;
    if (d == 0xa && (unsigned int)(c - 0xf) <= 2) return 1;
    if (d == 0xb && (unsigned int)(c - 0x12) <= 2) return 1;
    if (d == 0xc && (unsigned int)(c - 0x15) <= 2) return 1;
    return 0;
}

// retail: func_8016FBA8 (0x80170FA4)
void func_8016FBA8(CfMapEffectManager* pSelf, int flag) {
    for (int i = 0; i < 0x24; i++) {
        CfObject* obj = pSelf->field_0x0C[i];
        if (obj != 0) {
            func_800AD040((char*)obj, flag);
        }
    }
}

// retail: func_8016FC0C (0x80171008)
void func_8016FC0C(u8 arg) {
    if (lbl_eu_80664290 == 0) return;
    for (int i = 0; i < 0x24; i++) {
        CfObject* obj = lbl_eu_80664290->field_0x0C[i];
        if (obj != 0) {
            obj->CfObject_UnkVirtualFunc66(arg);
        }
    }
    lbl_eu_80664290->field_0x138 = arg;
}

// retail: func_8016FC98 (0x80171094) — release a registered effect object
// (or the scene handle at 0x134) by pointer identity.
void func_8016FC98(CfMapEffectManager* pSelf, CfObject* target) {
    if (reinterpret_cast<CfObject*>(pSelf->field_0x134) == target) {
        pSelf->field_0x134->field_0xB0 = 0;
        pSelf->field_0x134 = 0;
    }
    // 36 slots walked in 4 groups of 9, checking 8 per group (the 9th slot
    // of each group is skipped in retail).
    for (int i = 0; i < 0x24; i += 9) {
        for (int j = 0; j < 8; j++) {
            CfObject* obj = pSelf->field_0x0C[i + j];
            if (obj == target) {
                reinterpret_cast<CfMapEffectObject*>(obj)->field_0xB0 = 0;
                pSelf->field_0x0C[i + j] = 0;
            }
        }
    }
}

// retail: func_8016FD84 (0x80171180) — push a clamped flash value to every
// active effect object.
void func_8016FD84(float first, float second) {
    float value = second;
    if (lbl_eu_80664290 != 0) {
        if (first > lbl_eu_80667730) {
            first = lbl_eu_806623E8;
        }
        for (int i = 0; i < 0x24; i++) {
            CfObject* obj = lbl_eu_80664290->field_0x0C[i];
            if (obj != 0) {
                func_800ACC28(obj, first, value);
            }
        }
    }
}

extern "C" void func_8016FE2C(float val) { lbl_eu_806623E8 = val; }

extern "C" void* func_8016FE34(void* r3) {
    if (r3 == 0) return 0;
    int word = *(int*)((char*)r3 + 0x64);
    if ((word & 0x2) != 0 || (word & 0x4) != 0) {
        if (r3 != 0)
            return (char*)r3 - 0x3e9c;
    }
    return 0;
}
