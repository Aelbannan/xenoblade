// Auto-scaffolded catalog TU for kyoshin/cf/CCharEffectEne
// High-level C++ reconstruction from retail ASM.

#include "kyoshin/harness_catalog.hpp"

// -- External data tables (u16 ID lookup arrays, 44 entries each) --
extern "C" u16 lbl_eu_80501E48[];
extern "C" u16 lbl_eu_80501EB8[];
extern "C" u16 lbl_eu_80501F28[];
extern "C" u16 lbl_eu_80501F98[];

// -- External functions called by this TU --
// CfGameManager utilities
extern "C" void* func_8008187C__Q22cf13CfGameManagerFv(u32 value);
extern "C" void* func_800817BC__Q22cf13CfGameManagerFv(u32 value, u32 unused);
extern "C" void func_800ACF78(void* obj, void* mgr, u32 val);
extern "C" void func_800ACEF8(void* obj, void* ptr);
extern "C" void func_800ACC50(void* obj);
extern "C" void func_80484EB0(void* ptr);
extern "C" void func_800AD844(void* obj, void* mgr, u32 val);

// vtable for CCharEffectEne
extern "C" u8 lbl_eu_8052FDD8[];

// Base constructor
extern "C" void __ct__CCharEffect(void* self);

// -- CCharEffectEne class layout --
// Inherits from CCharEffect. vtable: lbl_eu_8052FDD8.
struct CCharEffectEne {
    u8 pad_000[0x26C];       // 0x000..0x26B: inherited from CCharEffect
    u16 mFlags;              // 0x26C
    u8 pad_26E[0x2E];        // 0x26E..0x29B: padding/inherited
    void* mManager;          // 0x29C: CfObjectMove* or similar
};

// -- VTable dispatch helpers --
typedef void* (*MgrVFunc_0x220)(void*, u32);
typedef void* (*VFunc_0xA8)(void*);
typedef void (*TargetVFunc_0x194)(void*, u32);

// __ct__cf_CCharEffectEne: Constructor.
extern "C" CCharEffectEne* __ct__cf_CCharEffectEne(CCharEffectEne* self) {
    __ct__CCharEffect(self);
    *(void**)self = lbl_eu_8052FDD8;
    self->mFlags |= 1;
    return self;
}

// func_8015CD04: resolve an object ID to a pointer.
extern "C" void* func_8015CD04(CCharEffectEne* self, u32 index, s32 mode) {
    void* result = nullptr;

    if (mode < 0) {
        u16 entry = lbl_eu_80501E48[index];
        if (entry != 0) {
            if (entry < 0x200) {
                result = func_8008187C__Q22cf13CfGameManagerFv(entry);
            } else {
                result = ((MgrVFunc_0x220)((*(void***)self->mManager)[0x220 / 4]))(
                    self->mManager, entry - 0x200);
            }
        }
    } else {
        result = ((MgrVFunc_0x220)((*(void***)self->mManager)[0x220 / 4]))(
            self->mManager, (u32)mode);
    }

    return result;
}

// func_8015CD9C: resolve object ID and attach to manager.
extern "C" void* func_8015CD9C(CCharEffectEne* self, u32 index, s32 mode) {
    void* obj = nullptr;

    if (mode < 0) {
        u16 entry = lbl_eu_80501EB8[index];
        if (entry != 0) {
            obj = ((MgrVFunc_0x220)((*(void***)self->mManager)[0x220 / 4]))(
                self->mManager, entry);
        }
    } else {
        obj = ((MgrVFunc_0x220)((*(void***)self->mManager)[0x220 / 4]))(
            self->mManager, (u32)mode);
    }

    if (obj != nullptr) {
        func_800ACF78(obj, self->mManager, 0);
    }

    return obj;
}

// func_8015CE44: resolve via CfGameManager lookup, attach to manager.
extern "C" void* func_8015CE44(CCharEffectEne* self, u32 index, s32 mode) {
    void* obj = nullptr;

    if (mode < 0) {
        u16 entry = lbl_eu_80501F28[index];
        if (entry != 0) {
            obj = func_8008187C__Q22cf13CfGameManagerFv(entry);
        }
    } else {
        obj = func_800817BC__Q22cf13CfGameManagerFv((u32)mode, 0);
    }

    if (obj != nullptr) {
        func_800ACF78(obj, self->mManager, 0);
    }

    return obj;
}

// func_8015CED0: resolve object ID with vfunc, attach to manager.
extern "C" void* func_8015CED0(CCharEffectEne* self, u32 index, s32 mode) {
    void* obj = nullptr;

    if (mode < 0) {
        u16 entry = lbl_eu_80501F98[index];
        if (entry != 0) {
            if (entry < 0x200) {
                obj = func_8008187C__Q22cf13CfGameManagerFv(entry);
                if (obj != nullptr) {
                    func_800ACF78(obj, self->mManager, 0);
                }
            } else {
                obj = ((MgrVFunc_0x220)((*(void***)self->mManager)[0x220 / 4]))(
                    self->mManager, entry - 0x200);
            }
        }
    } else {
        obj = ((MgrVFunc_0x220)((*(void***)self->mManager)[0x220 / 4]))(
            self->mManager, (u32)mode);
    }

    return obj;
}

// func_8015CF90: apply effect with type-specific data source.
// r4 = target, r5 = type, r6 = flags.
extern "C" void func_8015CF90(CCharEffectEne* self, void* target, u32 type, u32 flags) {
    void* mgr;
    void* data;
    void* dataSrc;
    u32 bitFlag, subtype;

    mgr = self->mManager;
    dataSrc = *(void**)((u8*)mgr + 0x98);
    if (dataSrc == nullptr) return;

    if (mgr != nullptr) {
        mgr = (void*)((u8*)mgr - 0x3E9C);
    }

    bitFlag = (flags >> 7) & 1;
    subtype = flags & 0x7F;
    data = nullptr;

    if (type == 0xA || type == 0xC || type == 0xE || type == 0x19 || subtype == 2) {
        data = *(void**)((u8*)mgr + 0x4594);
    } else if (type == 0xB || type == 0xD || type == 0xF || subtype == 3) {
        data = *(void**)((u8*)mgr + 0x4598);
    }

    if (data != nullptr) {
        dataSrc = ((VFunc_0xA8)((*(void***)data)[0xA8 / 4]))(data);
    } else {
        dataSrc = nullptr;
    }

    func_800ACF78(target, self->mManager, (u32)dataSrc);

    ((TargetVFunc_0x194)((*(void***)target)[0x194 / 4]))(target, bitFlag);

    func_800ACEF8(target, (void*)((u8*)(*(void**)((u8*)self->mManager + 0x98)) + 0x304));

    func_80484EB0(*(void**)((u8*)self->mManager + 0x98));

    func_800ACC50(target);
}
