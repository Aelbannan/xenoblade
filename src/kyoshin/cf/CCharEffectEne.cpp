// Auto-scaffolded catalog TU for kyoshin/cf/CCharEffectEne
// High-level C++ reconstruction from retail ASM.

#include <types.h>
#include "kyoshin/cf/CCharEffectEne.hpp"
#include "kyoshin/cf/CCharEffect.hpp"
#include "kyoshin/cf/object/CfObjectMove.hpp"
#include "kyoshin/cfsys/CfObjectImplPc.hpp"
#include "kyoshin/cf/object/CfObjectEff.hpp"
#include "kyoshin/cf/object/CfObjectEffSymbols.hpp"

// name-guess: 44-entry id table used by ChEff_LoadNpcA_CD04.
static const u16 sNpcEffectIds[44] = {
    0x0079, 0x0051, 0x0065, 0x0201, 0x0202, 0x0203, 0x0204, 0x0205,
    0x0206, 0x020F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x00A7, 0x00A6, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0055, 0x0056, 0x0057, 0x0052,
    0x0059, 0x0058, 0x0246, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x005A, 0x0054, 0x0053, 0x0000,
};

// Retail .rodata interleaves an unreferenced "cf::CCharEffectEne" string after each
// table (0x80501EA0/F10/F80) whose producer is not yet decompiled, so these three
// tables stay retail-label externs until it is (OPUS-T-CCHAREFFECTENE-REGAIN).
extern "C" const u16 lbl_eu_80501EB8[44];
extern "C" const u16 lbl_eu_80501F28[44];
extern "C" const u16 lbl_eu_80501F98[44];

extern "C" void* createNpcActor__Q22cf13CfGameManagerFv(u32 value);
extern "C" void* createBattleActor__Q22cf13CfGameManagerFv(u32 value, u32 unused);

extern "C" void setChild34Sc_(void* obj, const void* src);
// setChildF50G_ / simGetLeafDist7B0 are owned by kyoshin/cf/CCharEffect.hpp
// (canonical u8* form); do not redeclare here (MWCC 10197).
extern "C" u8 lbl_eu_8052FDD8[];
extern "C" void* __ct__CCharEffect(void* self);

// CCharEffectEne layout (inherits CCharEffect, vptr at 0)
struct CCharEffectEne {
    u8 pad_000[0x26C];
    u16 mFlags;
    u8 pad_26E[0x2E];
    cf::CfObjectMove* mManager; // +0x29C: CfObjectMove sub-object (vtable lbl_eu_80529690); +0x220 = CfObjectMove_loadResourceById
};

extern "C" CCharEffectEne* __ct__cf_CCharEffectEne(CCharEffectEne* self) {
    __ct__CCharEffect(self);
    *(void**)self = lbl_eu_8052FDD8;
    self->mFlags |= 1;
    return self;
}

extern "C" void* ChEff_LoadNpcA_CD04(CCharEffectEne* self, u32 index, s32 mode) {
    void* result = nullptr;
    if (mode < 0) {
        u16 entry = sNpcEffectIds[index];
        if (entry != 0) {
            if (entry < 0x200) {
                result = createNpcActor__Q22cf13CfGameManagerFv(entry);
            } else {
                result = self->mManager->CfObjectMove_loadResourceById(entry - 0x200);
            }
        }
    } else {
        result = self->mManager->CfObjectMove_loadResourceById((u32)mode);
    }
    return result;
}

extern "C" void* ChEff_LoadRes_CD9C(CCharEffectEne* self, u32 index, s32 mode) {
    void* obj = nullptr;
    if (mode < 0) {
        u16 entry = lbl_eu_80501EB8[index];
        if (entry != 0) {
            obj = self->mManager->CfObjectMove_loadResourceById(entry);
        }
    } else {
        obj = self->mManager->CfObjectMove_loadResourceById((u32)mode);
    }
    if (obj != nullptr) {
        bindPartnerO_((cf::CfObjectEff*)obj, (u8*)self->mManager, (u8*)0);
    }
    return obj;
}

extern "C" void* ChEff_LoadActorA_CE44(CCharEffectEne* self, u32 index, s32 mode) {
    void* obj = nullptr;
    if (mode < 0) {
        u16 entry = lbl_eu_80501F28[index];
        if (entry != 0) {
            obj = createNpcActor__Q22cf13CfGameManagerFv(entry);
        }
    } else {
        obj = createBattleActor__Q22cf13CfGameManagerFv((u32)mode, 0);
    }
    if (obj != nullptr) {
        bindPartnerO_((cf::CfObjectEff*)obj, (u8*)self->mManager, (u8*)0);
    }
    return obj;
}

extern "C" void* ChEff_LoadNpcBind_CED0(CCharEffectEne* self, u32 index, s32 mode) {
    void* obj = nullptr;
    if (mode < 0) {
        u16 entry = lbl_eu_80501F98[index];
        if (entry != 0) {
            if (entry < 0x200) {
                obj = createNpcActor__Q22cf13CfGameManagerFv(entry);
                if (obj != nullptr) {
                    bindPartnerO_((cf::CfObjectEff*)obj, (u8*)self->mManager, (u8*)0);
                }
            } else {
                obj = self->mManager->CfObjectMove_loadResourceById(entry - 0x200);
            }
        }
    } else {
        obj = self->mManager->CfObjectMove_loadResourceById((u32)mode);
    }
    return obj;
}

extern "C" void ChEff_Dispatch_CF90(CCharEffectEne* self, void* target, u32 type, u32 flags) {
    void* mgr;
    void* data;
    void* dataSrc;
    u32 bitFlag, subtype;
    mgr = self->mManager;
    dataSrc = static_cast<cf::CfObjectMove*>(mgr)->mSubObj98;
    if (dataSrc == nullptr) return;
    bitFlag = (flags >> 7) & 1;
    subtype = flags & 0x7F;
    dataSrc = nullptr;
    if (mgr != nullptr) {
        mgr = (void*)((u8*)mgr - 0x3E9C);
    }
    data = nullptr;
    if (type == 0xA || type == 0xC || type == 0xE || type == 0x19 || subtype == 2) {
        data = static_cast<cf::CfObjectImplPc18*>(mgr)->field_4594;
    } else if (type == 0xB || type == 0xD || type == 0xF || subtype == 3) {
        data = static_cast<cf::CfObjectImplPc18*>(mgr)->field_4598;
    }
    if (data != nullptr) {
        dataSrc = reinterpret_cast<void*>(reinterpret_cast<cf::CfObjectImplPcTgt*>(data)->vfA8());
    }
    bindPartnerO_((cf::CfObjectEff*)target, (u8*)self->mManager, (u8*)(u32)(uintptr_t)dataSrc);
    reinterpret_cast<cf::CfObjectEff*>(target)->setEffLockFg_(bitFlag);
    setChild34Sc_(target, &self->mManager->mSubObj98->mScale304);
    simGetLeafDist7B0(reinterpret_cast<u8*>(self->mManager->mSubObj98));
    setChildF50G_((u8*)target);
}
