// Auto-scaffolded catalog TU for kyoshin/cf/CCharEffectEne
// High-level C++ reconstruction from retail ASM.

#include <types.h>
#include "kyoshin/cf/CCharEffect.hpp"
#include "kyoshin/cf/object/CfObjectMove.hpp"
#include "kyoshin/cfsys/CfObjectImplPc.hpp"
#include "kyoshin/cf/object/CfObjectEff.hpp"

// External data tables
extern "C" u16 lbl_eu_80501E48[];
extern "C" u16 lbl_eu_80501EB8[];
extern "C" u16 lbl_eu_80501F28[];
extern "C" u16 lbl_eu_80501F98[];
extern "C" void* createNpcActor__Q22cf13CfGameManagerFv(u32 value);
extern "C" void* createBattleActor__Q22cf13CfGameManagerFv(u32 value, u32 unused);
extern "C" void bindPartnerO_(void* obj, void* mgr, u32 val);
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

extern "C" void* func_8015CD04(CCharEffectEne* self, u32 index, s32 mode) {
    void* result = nullptr;
    if (mode < 0) {
        u16 entry = lbl_eu_80501E48[index];
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

extern "C" void* func_8015CD9C(CCharEffectEne* self, u32 index, s32 mode) {
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
        bindPartnerO_(obj, self->mManager, 0);
    }
    return obj;
}

extern "C" void* func_8015CE44(CCharEffectEne* self, u32 index, s32 mode) {
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
        bindPartnerO_(obj, self->mManager, 0);
    }
    return obj;
}

extern "C" void* func_8015CED0(CCharEffectEne* self, u32 index, s32 mode) {
    void* obj = nullptr;
    if (mode < 0) {
        u16 entry = lbl_eu_80501F98[index];
        if (entry != 0) {
            if (entry < 0x200) {
                obj = createNpcActor__Q22cf13CfGameManagerFv(entry);
                if (obj != nullptr) {
                    bindPartnerO_(obj, self->mManager, 0);
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

extern "C" void func_8015CF90(CCharEffectEne* self, void* target, u32 type, u32 flags) {
    void* mgr;
    void* data;
    void* dataSrc;
    u32 bitFlag, subtype;
    mgr = self->mManager;
    dataSrc = *(void**)((u8*)mgr + 0x98);
    if (dataSrc == nullptr) return;
    bitFlag = (flags >> 7) & 1;
    subtype = flags & 0x7F;
    dataSrc = nullptr;
    if (mgr != nullptr) {
        mgr = (void*)((u8*)mgr - 0x3E9C);
    }
    data = nullptr;
    if (type == 0xA || type == 0xC || type == 0xE || type == 0x19 || subtype == 2) {
        data = *(void**)((u8*)mgr + 0x4594);
    } else if (type == 0xB || type == 0xD || type == 0xF || subtype == 3) {
        data = *(void**)((u8*)mgr + 0x4598);
    }
    if (data != nullptr) {
        dataSrc = reinterpret_cast<void*>(reinterpret_cast<cf::CfObjectImplPcTgt*>(data)->vfA8());
    }
    bindPartnerO_(target, self->mManager, (u32)(uintptr_t)dataSrc);
    reinterpret_cast<cf::CfObjectEff*>(target)->setEffLockFg_(bitFlag);
    setChild34Sc_(target, (void*)((u8*)(*(void**)((u8*)self->mManager + 0x98)) + 0x304));
    simGetLeafDist7B0((u8*)(*(void**)((u8*)self->mManager + 0x98)));
    setChildF50G_((u8*)target);
}
