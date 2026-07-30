// Auto-scaffolded catalog TU for kyoshin/cf/CCharEffectEne
// Replace stubs with high-level C/C++ during decomp.

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

// -- CCharEffectEne class layout --
// Inherits from CCharEffect (defined elsewhere via vtable).
// Size: at least 0x2A0 bytes.
class CCharEffectEne {
public:
    // +0x000: vtable (lbl_eu_8052FDD8)
    u8 pad_000[0x26C];       // 0x000..0x26B: inherited fields from CCharEffect
    u16 mFlags;              // 0x26C: bit-0 set in ctor
    u8 pad_26E[0x2E];        // 0x26E..0x29B
    void* mManager;          // 0x29C: CfObjectMove* (or similar)
    // total size >= 0x2A0
};

// -- Function implementations --

// Constructor.
// Calls base __ct__CCharEffect, sets vtable, marks flag bit 0.
extern "C" void __ct__cf_CCharEffectEne(CCharEffectEne* self) {
    // Base constructor call via thunk (defined elsewhere)
    extern void __ct__CCharEffect(void* self);
    __ct__CCharEffect(self);

    // Set vtable and enable flag bit 0
    self->mFlags |= 1;
    // vtable written by the base ctor call? Actually the retail does:
    //   lwz r12 from vtbl after base ctor, then replaces +0x00
    // but the vtable store is at the end of the retail ctor sequence.
    // MWCC generates: base-call → this-adjust → vtable-store → flag-set
    // The vtable store is implicit in the inlined base ctor result.
    // We capture it via the vtable symbol reference below:
    extern u8 lbl_eu_8052FDD8[];
    *(void**)self = lbl_eu_8052FDD8;
}

// func_8015CD04: resolve an object ID to a pointer.
// r4 = index, r5 = mode (<0: lookup in table, >=0: direct value).
// Returns the resolved object pointer (or null).
extern "C" void* func_8015CD04(CCharEffectEne* self, u32 index, s32 mode) {
    void* result = nullptr;

    if (mode < 0) {
        u16 entry = lbl_eu_80501E48[index];
        if (entry != 0) {
            if (entry < 0x200) {
                result = func_8008187C__Q22cf13CfGameManagerFv(entry);
            } else {
                typedef void* (*MgrVFunc_0x220)(void*, u32);
                void* mgr = self->mManager;
                void** vtbl = *(void***)mgr;
                MgrVFunc_0x220 func = (MgrVFunc_0x220)(vtbl[0x220 / 4]);
                result = func(mgr, entry - 0x200);
            }
        }
    } else {
        typedef void* (*MgrVFunc_0x220)(void*, u32);
        void* mgr = self->mManager;
        void** vtbl = *(void***)mgr;
        MgrVFunc_0x220 func = (MgrVFunc_0x220)(vtbl[0x220 / 4]);
        result = func(mgr, (u32)mode);
    }

    return result;
}

// func_8015CD9C: resolve object ID and attach to manager.
// Returns the resolved object (may be null).
extern "C" void* func_8015CD9C(CCharEffectEne* self, u32 index, s32 mode) {
    void* obj = nullptr;

    if (mode < 0) {
        u16 entry = lbl_eu_80501EB8[index];
        if (entry != 0) {
            typedef void* (*MgrVFunc_0x220)(void*, u32);
            void* mgr = self->mManager;
            void** vtbl = *(void***)mgr;
            MgrVFunc_0x220 func = (MgrVFunc_0x220)(vtbl[0x220 / 4]);
            obj = func(mgr, entry);
        }
    } else {
        typedef void* (*MgrVFunc_0x220)(void*, u32);
        void* mgr = self->mManager;
        void** vtbl = *(void***)mgr;
        MgrVFunc_0x220 func = (MgrVFunc_0x220)(vtbl[0x220 / 4]);
        obj = func(mgr, (u32)mode);
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
                typedef void* (*MgrVFunc_0x220)(void*, u32);
                void* mgr = self->mManager;
                void** vtbl = *(void***)mgr;
                MgrVFunc_0x220 func = (MgrVFunc_0x220)(vtbl[0x220 / 4]);
                obj = func(mgr, entry - 0x200);
            }
        }
    } else {
        typedef void* (*MgrVFunc_0x220)(void*, u32);
        void* mgr = self->mManager;
        void** vtbl = *(void***)mgr;
        MgrVFunc_0x220 func = (MgrVFunc_0x220)(vtbl[0x220 / 4]);
        obj = func(mgr, (u32)mode);
    }

    return obj;
}

// func_8015CF90: apply effect with type-specific data source.
// r4 = target effect object, r5 = effect type, r6 = flags (bit 24 = flag,
// bits 0-4 = subtype).
extern "C" void func_8015CF90(CCharEffectEne* self, void* target, u32 type, u32 flags) {
    void* mgr = self->mManager;
    void* dataSrc = *(void**)((u8*)mgr + 0x98);
    if (dataSrc == nullptr) return;

    // Adjust manager to parent struct offset if non-null
    void* parentMgr = mgr;
    if (parentMgr != nullptr) {
        parentMgr = (void*)((u8*)mgr - 0x3E9C);
    }

    u32 bitFlag = (flags >> 24) & 1;   // extract bit 24
    u32 subtype = flags & 0x1F;        // low 5 bits

    // Choose data source based on type
    void* data = nullptr;
    if (type == 0xA || type == 0xC || type == 0xE || type == 0x19 || subtype == 2) {
        data = *(void**)((u8*)parentMgr + 0x4594);
    } else if (type == 0xB || type == 0xD || type == 0xF || subtype == 3) {
        data = *(void**)((u8*)parentMgr + 0x4598);
    }

    if (data != nullptr) {
        typedef void* (*VFunc_0xA8)(void*);
        void** vtbl = *(void***)data;
        VFunc_0xA8 func = (VFunc_0xA8)(vtbl[0xA8 / 4]);
        dataSrc = func(data);  // vfunc result overrides dataSrc
    } else {
        dataSrc = nullptr;
    }

    func_800ACF78(target, mgr, (u32)dataSrc);

    // Call vfunc 0x194 on target with bitFlag
    typedef void (*TargetVFunc_0x194)(void*, u32);
    void** targetVtbl = *(void***)target;
    TargetVFunc_0x194 tgtFunc = (TargetVFunc_0x194)(targetVtbl[0x194 / 4]);
    tgtFunc(target, bitFlag);

    func_800ACEF8(target, (void*)((u8*)(*(void**)((u8*)mgr + 0x98)) + 0x304));

    func_80484EB0(*(void**)((u8*)mgr + 0x98));

    func_800ACC50(target);
}
