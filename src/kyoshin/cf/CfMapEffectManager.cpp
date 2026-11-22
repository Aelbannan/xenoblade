// Decompiled unit for kyoshin/cf/CfMapEffectManager (retail .text
// 0x80170104-0x80171264).

#include "kyoshin/cf/CfMapEffectManager.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/CfTFile.hpp"
#include "kyoshin/cf/object/CfObject.hpp"
#include "kyoshin/cf/object/CfObjectActor.hpp"
#include "kyoshin/cf/object/CfObjectEff.hpp"  // cf::CfObjectEff definition
#include "kyoshin/cf/object/CfObjectEffSymbols.hpp"
#include "kyoshin/cf/CfResReloadImpl.hpp"  // getReloadParam0 (owner)
#include "kyoshin/cf/CfMapMineManager.hpp" // extern "C" lookupWorkAtAddr/800B9404/8003B1EC/8003B41C
#include "monolib/device/CDeviceSC.hpp"   // CDeviceSC::getLanguage
#include <string.h>

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------
extern "C" {

void setChildScl__(cf::CfObject* object, float first, float second);
// CfGameManager phase/counter getters (same signatures as CfGimmickItem.hpp;
// CfGimmickItem.hpp itself cannot be included here - its CfGimmick.hpp
// re-declares CtrlRemote_TouchBitByArg with a conflicting signature).
u32 getQueuedFileEventCount__Q22cf13CfGameManagerFv(void);
u32 getResourceFromTable__Q22cf13CfGameManagerFv(u32 a);
// Retail call shape: pre-mangled member symbol with the object id in r3 and
// a second (unused) GPR left at 0; no owner header carries this spelling yet.
void* createBattleActor__Q22cf13CfGameManagerFv(u32 value, u32 unused);
}

// Retail data symbols (linker-provided)
extern u16 gCfTCalendarDay;                   // sdata: play-clock / BGM frame counter

// name-guess: sdata float written by setGlobalFloat() and used as the clamped
// first argument of setChildScl__() in pushFlash().
float gMapEffectFlashScale = 10.0f;
// name-guess: sdata pointer initialised to "signal1"; byte 6 is rewritten with
// '0'+i to form the per-frame BDAT column name signal1..signal8.
char* gMapEffectColumnName = (char*)"signal1";

#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (gmScene; gmGameFlags)

using namespace cf;

CfMapEffectManager* CfMapEffectManager::sInstance;

CfMapEffectManager::CfMapEffectManager()
    : field_0x4(-1), field_0x6(-1), field_0x8(-1), field_0xA(0xF) {
    memset(field_0x0C, 0, sizeof(field_0x0C));
    memset(field_0x9C, 0, sizeof(field_0x9C));
    memset(field_0xE4, 0, sizeof(field_0xE4));
    field_0x12C = -1;
    field_0x12E = -1;
    field_0x130 = -1;
    field_0x134 = 0;
    field_0x138 = 1;
    sInstance = this;
}

CfMapEffectManager::~CfMapEffectManager() {
    resetSlots();
    CfMapEffectHandle* obj = field_0x134;
    field_0x12C = -1;
    field_0x12E = -1;
    field_0x130 = -1;
    if (obj != 0) {
        if (lookupWorkAtAddr(obj) != 0) {
            field_0x134->field_0xB0 = 0;
            gmFileObject(field_0x134);
            field_0x134 = 0;
        }
        field_0x134 = 0;
    }
    sInstance = 0;
}

void CfMapEffectManager::releaseHandle() {
    CfMapEffectManager* pSelf = this;
    resetSlots();
    CfMapEffectHandle* obj = pSelf->field_0x134;
    pSelf->field_0x12C = -1;
    pSelf->field_0x12E = -1;
    pSelf->field_0x130 = -1;
    if (obj != 0) {
        if (lookupWorkAtAddr(obj) != 0) {
            pSelf->field_0x134->field_0xB0 = 0;
            gmFileObject(pSelf->field_0x134);
            pSelf->field_0x134 = 0;
        }
        pSelf->field_0x134 = 0;
    }
}

// Reset the effect-object slots and the two per-slot s16 counter banks.
void CfMapEffectManager::resetSlots() {
    CfMapEffectManager* pSelf = this;
    pSelf->field_0x4 = -1;
    pSelf->field_0x6 = -1;
    pSelf->field_0xA = 0xF;
    for (int i = 0; i < 0x24; i++) {
        CfObject* obj = pSelf->field_0x0C[i];
        if (obj != 0) {
            if (lookupWorkAtAddr(obj) != 0) {
                reinterpret_cast<CfMapEffectObject*>(obj)->field_0xB0 = 0;
                gmFileObject(pSelf->field_0x0C[i]);
                pSelf->field_0x0C[i] = 0;
            }
            pSelf->field_0x0C[i] = 0;
        }
        reinterpret_cast<s16*>(pSelf->field_0x9C)[i] = 0;
        reinterpret_cast<s16*>(pSelf->field_0xE4)[i] = 0;
    }
}

// BDAT range check against the current chapter/event counters reported by
// CfGameManager.
int CfMapEffectManager::checkRowRange(int index) {
    u8* bdat = (u8*)::cf::CfBdat::spCurMapEffListFileData;
    u32 v0 = getBdatStringColumnValue(bdat, "questID", index);
    if ((u16)v0 != 0) {
        u32 cur = (u16)getResourceFromTable__Q22cf13CfGameManagerFv((u16)v0);
        u32 v1 = getBdatStringColumnValue(bdat, "qst_sflg", index);
        u32 v2 = getBdatStringColumnValue(bdat, "qst_eflg", index);
        if (cur < (u8)v1 || cur > (u8)v2) {
            return 0;
        }
    }
    u32 cur2 = (u16)getQueuedFileEventCount__Q22cf13CfGameManagerFv();
    u32 v3 = getBdatStringColumnValue(bdat, "sflg1", index);
    u32 v4 = getBdatStringColumnValue(bdat, "eflg1", index);
    u32 v5 = getBdatStringColumnValue(bdat, "sflg2", index);
    u32 v6 = getBdatStringColumnValue(bdat, "eflg2", index);
    if (cur2 >= (u16)v3 && cur2 <= (u16)v4 && (u16)v4 != 0) {
        return 1;
    }
    if (cur2 >= (u16)v5 && cur2 <= (u16)v6 && (u16)v6 != 0) {
        return 1;
    }
    return 0;
}

void CfMapEffectManager::forwardReload() { reloadFromBdat(); }

// Scene-attached effect. When
// the chapter/event counters change, release the current scene handle and
// create the one matching the new state (BGM/ambient effect ids are
// language-dependent). The 0x16/1 area-id pair disables the feature entirely.
void CfMapEffectManager::updateSceneFx() {
    CfMapEffectManager* self = this;
    cf::CfGameManager::getCurrentSlotIndex();
    u16 cur0 = (u16)cf::CfGameManager::getControllerWordA33C();
    u16 cur1 = (u16)cf::CfGameManager::getControllerWordA37C();
    int type = 0;
    if (gmGlobalFlag42 != 0x16 || gmGlobalFlag44 != 1) {
        cf::CfGameManager::getGlobalPtr640A8();
        cf::CfGameManager::getGlobalWord64184Dup();
        if (self->field_0x12E != cur0 || self->field_0x130 != cur1) {
            if ((int)cur0 == 4 && (int)cur1 == 0x3b) {
                type = 0xb6;
                if (CDeviceSC::getLanguage() >= 1) {
                    type = (CDeviceSC::getLanguage() - 1) * 2 + 0xcf;
                }
                u16 bgm = gCfTCalendarDay;
                if (bgm != getEventValue40__Q22cf13CfGameManagerFv(0x6f)) {
                    u32 next = getEventValue40__Q22cf13CfGameManagerFv(0x81) + 1;
                    setEventManagerValue__Q22cf13CfGameManagerFv(0x81, next);
                    if (next == 0x16e) {
                        queueEventId__Q22cf13CfGameManagerFv(0x81);
                    }
                    setEventManagerValue__Q22cf13CfGameManagerFv(0x6f, gCfTCalendarDay);
                }
            } else if ((int)cur0 == 0x12 && (int)cur1 == 0x3b) {
                type = 0xb7;
                if (CDeviceSC::getLanguage() >= 1) {
                    type = (CDeviceSC::getLanguage() - 1) * 2 + 0xd0;
                }
            }
        }
        if (type != 0 && cf::CfGameManager::isTimerActive() == 0) {
            CfMapEffectHandle* obj = self->field_0x134;
            self->field_0x12C = -1;
            self->field_0x12E = -1;
            self->field_0x130 = -1;
            if (obj != 0) {
                if (lookupWorkAtAddr(obj) != 0) {
                    self->field_0x134->field_0xB0 = 0;
                    gmFileObject(self->field_0x134);
                    self->field_0x134 = 0;
                }
                self->field_0x134 = 0;
            }
            CfMapEffectHandle* h =
                (CfMapEffectHandle*)createNpcActor__Q22cf13CfGameManagerFv(type);
            self->field_0x134 = h;
            h->field_0xB0 = (u32)self;
        }
        self->field_0x12E = cur0;
        self->field_0x130 = cur1;
    }
}

// Per-row map-effect update driven by
// bdat columns: reconcile the effect object at field_0x0C[index] with the
// current event state, rebuild it when the state is "active" and the slot is
// empty, then push the per-frame column data through the effect virtuals.
void CfMapEffectManager::updateRow(int index, u8* bdat) {
    CfMapEffectManager* self = this;
    u32 flags = gmGameFlags;
    // Fast path: event bits set + +0x32 column enabled -> poke the 0x158
    // virtual on the existing effect object and stop.
    if ((flags & 0x2000000) != 0 || (flags & 0x400) != 0) {
        u32 v32 = getBdatStringColumnValue(bdat, "erase", index);
        if ((u8)v32 != 0) {
            CfObject* obj = self->field_0x0C[index];
            if (obj != 0) {
                obj->setPointEnabled(0);
            }
        }
        return;
    }

    u32 phase = cf::CfGameManager::getCurrentSlotIndex();
    u16 area = (u16)cf::CfGameManager::getControllerWordA33C();
    cf::CfGameManager::getControllerWordA37C(); // result discarded (retail calls it)
    u16 secs = getReloadParam0();

    u32 modelCol = getBdatStringColumnValue(bdat, "pack_no", index);
    u8 timeCol = (u8)getBdatStringColumnValue(bdat, "weather", index);
    u8 onceCol = (u8)getBdatStringColumnValue(bdat, "loop", index);

    CfObject** slot = &self->field_0x0C[index];
    if (*slot != 0 && lookupWorkAtAddr(*slot) == 0) {
        // Stale handle: drop the object and clear both counter banks.
        *slot = 0;
        self->field_0xE4[index] = 0;
        self->field_0x9C[index] = 0;
    }

    u16 gate = (u16)getBdatStringColumnValue(bdat, "flg_eff", index);
    if (gate != 0 && CtrlRemote_TouchBitByArg(gate + 0x278a) == 0) {
        // Gated off: force idle state (1) unless already applied.
        if (*slot != 0 && self->field_0xE4[index] != 1) {
            setChildB59__(*slot, 1);
            self->field_0xE4[index] = 1;
            self->field_0x9C[index] = 0;
        }
        return;
    }

    int inRange = self->checkRowRange(index);
    int timeOk = 1;
    if (timeCol != 0 && (u32)timeCol != (u32)secs) {
        timeOk = 0;
    }

    u16 subs[3];
    subs[0] = (u8)getBdatStringColumnValue(bdat, "playtime1", index);
    subs[1] = (u8)getBdatStringColumnValue(bdat, "playtime2", index);
    subs[2] = (u8)getBdatStringColumnValue(bdat, "playtime3", index);
    CfT_DispatchFrameCounts();
    int anySub = 0;
    for (int i = 0; i < 3; i++) {
        if (i != 0 && subs[i] == 0) {
            continue;
        }
        int r =
            (subs[i] == 0) ? 1 : self->matchKind((int)phase, (int)area, (int)subs[i]);
        anySub = (anySub | r) != 0;
    }

    int spawn = 0;
    if (inRange != 0 && timeOk != 0 && anySub != 0) {
        spawn = 1;
    }
    if (spawn == 0) {
        if (*slot != 0 && self->field_0xE4[index] != 1) {
            setChildB59__(*slot, 1);
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
        setChildB59__(*slot, 2);
        self->field_0xE4[index] = 2;
    }
    if (onceCol == 0 && self->field_0x9C[index] != 0) {
        spawn = 0;
    }

    if (*slot == 0 && spawn != 0) {
        u16 flashCol = (u16)getBdatStringColumnValue(bdat, "progframe", index);
        u32 attachCol = getBdatStringColumnValue(bdat, "caster", index);
        CfObject* extra = 0;
        if ((u8)attachCol != 0) {
            extra = (CfObject*)ArtsSelect_GetEntry((u8)attachCol);
            if (extra == 0) {
                return;
            }
        }
        float flashArg = 1.0f;
        CfObjectEff* newObj =
            (CfObjectEff*)createBattleActor__Q22cf13CfGameManagerFv((u16)modelCol, 0);
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
            pos[0] = 0.0001f * (float)(s32)getBdatStringColumnValue(bdat, "posX", index);
            pos[1] = 0.0001f * (float)(s32)getBdatStringColumnValue(bdat, "posY", index);
            pos[2] = 0.0001f * (float)(s32)getBdatStringColumnValue(bdat, "posZ", index);

            // Columns +0x93/+0x98/+0x9d: signed column -> floats. Kept as an
            // array so MWCC spills them; the third converts into the arg block.
            float vals[3];
            vals[0] = 0.01f * (float)(s16)getBdatStringColumnValue(bdat, "rotX", index);
            vals[1] = 0.01f * (float)(s16)getBdatStringColumnValue(bdat, "rotY", index);

            float args[3];
            args[0] = vals[0] * ml::deg2rad;
            args[1] = vals[1] * ml::deg2rad;
            vals[2] = 0.01f * (float)(s16)getBdatStringColumnValue(bdat, "rotZ", index);
            args[2] = vals[2] * ml::deg2rad;
            newObj->CfObject_setMoveTargetVec((const ml::CVec3*)pos);
            newObj->CfObject_setRotVec((const ml::CVec3*)args);
            if (extra != 0) {
                bindPartnerO_((cf::CfObjectEff*)newObj, (u8*)extra, (u8*)0);
            }
            GameSubManagerCamState* cam =
                (GameSubManagerCamState*)cf::CfGameManager::getGameSubManager();
            int camFlag = cam->getEffectEnableFlag();
            setChild5CFl_((char*)newObj, camFlag);
            newObj->setPointEnabled(self->field_0x138);

            if (flashCol != 0) {
                u32 wa2 = getBdatStringColumnValue(bdat, "skip", index);
                if ((u8)wa2 != 0) {
                    u32 wa7 = getBdatStringColumnValue(bdat, "calc_frame", index);
                    flashArg = (float)(u16)wa7;
                }
                setChildScl__(newObj, (float)(u16)flashCol, flashArg);
            }
        }
    }

    // Per-frame column push (columns 0x30+i in the scratch-name buffer)
    // while the slot is live. Counter compare is signed in retail.
    if (*slot != 0) {
        for (int i = 1; i <= 8; i++) {
            gMapEffectColumnName[6] = (u8)(i + 0x30);
            u32 vv = getBdatStringColumnValue(bdat, gMapEffectColumnName, index);
            int r;
            if ((u8)vv == 0) {
                r = 0;
            } else {
                r = self->matchKind((int)phase, (int)area, (int)(u8)vv);
            }
            if (r != 0 && (s16)self->field_0xE4[index] != (s16)i) {
                setChildB59__(self->field_0x0C[index], (s8)i);
                self->field_0xE4[index] = (u16)i;
            }
        }
    }
}

void CfMapEffectManager::reloadFromBdat() {
    CfMapEffectManager* pSelf = this;
    u8* bdat = (u8*)::cf::CfBdat::spCurMapEffListFileData;
    u32 v = cf::CfGameManager::getCurrentSlotIndex();
    u16 a = (u16)cf::CfGameManager::getControllerWordA33C();
    u16 b = (u16)cf::CfGameManager::getControllerWordA37C();
    u16 c = getReloadParam0();
    int end;
    int i = (int)Bdat_GetRowBase_B41C(bdat);
    end = i + (int)Bdat_GetMaxRow_B1EC(bdat);
    for (; i < end; i++) {
        pSelf->updateRow(i, bdat);
    }
    pSelf->field_0x4 = v;
    pSelf->field_0x6 = a;
    pSelf->field_0x8 = b;
    pSelf->field_0xA = c;
}

int CfMapEffectManager::matchKind(int b, int c, int d) {
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

void CfMapEffectManager::setSlotFlag(int flag) {
    CfMapEffectManager* pSelf = this;
    for (int i = 0; i < 0x24; i++) {
        CfObject* obj = pSelf->field_0x0C[i];
        if (obj != 0) {
            setChild5CFl_((char*)obj, flag);
        }
    }
}

void CfMapEffectManager::setPointEnabled(u8 arg) {
    if (sInstance == 0) return;
    for (int i = 0; i < 0x24; i++) {
        CfObject* obj = sInstance->field_0x0C[i];
        if (obj != 0) {
            obj->setPointEnabled(arg);
        }
    }
    sInstance->field_0x138 = arg;
}

// Release a registered effect object (or the scene handle at 0x134) by
// pointer identity.
void CfMapEffectManager::releaseObject(CfObject* target) {
    CfMapEffectManager* pSelf = this;
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

// Push a clamped flash value to every active effect object.
void CfMapEffectManager::pushFlash(float first, float second) {
    float value = second;
    if (sInstance != 0) {
        if (first > 0.0f) {
            first = gMapEffectFlashScale;
        }
        for (int i = 0; i < 0x24; i++) {
            CfObject* obj = sInstance->field_0x0C[i];
            if (obj != 0) {
                setChildScl__(obj, first, value);
            }
        }
    }
}

void CfMapEffectManager::setGlobalFloat(float val) { gMapEffectFlashScale = val; }

extern "C" void* MapFx_ResolveOwner(void* effectObj) {
    cf::CfObjectMove* move = static_cast<cf::CfObjectMove*>(effectObj);
    if (move != 0) {
        u32 flags = move->unk64;
        if ((flags & 0x2) != 0 || (flags & 0x4) != 0) {
            return static_cast<cf::CfObjectActor*>(move);
        }
    }
    return 0;
}
