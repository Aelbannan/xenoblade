// Auto-scaffolded catalog TU for kyoshin/cf/CCharEffectEne
// High-level C++ reconstruction from retail ASM.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/CCharEffectEne.hpp"

// -- CCharEffectEne class layout --
// Inherits from CCharEffect. vtable: lbl_eu_8052FDD8.
struct CCharEffectEne {
    u8 pad_000[0x26C];       // 0x000..0x26B: inherited from CCharEffect
    u16 mFlags;              // 0x26C
    u8 pad_26E[0x2E];        // 0x26E..0x29B: padding/inherited
    void* mManager;          // 0x29C: CfObjectMove* or similar
};

// Cast-only SI interface for the virtual slots used here. The two RTTI
// slots are omitted so the declared methods retain their retail offsets.
struct CCharEffectEneVTableIf {
    virtual void _v008();
    virtual void _v00C();
    virtual void _v010();
    virtual void _v014();
    virtual void _v018();
    virtual void _v01C();
    virtual void _v020();
    virtual void _v024();
    virtual void _v028();
    virtual void _v02C();
    virtual void _v030();
    virtual void _v034();
    virtual void _v038();
    virtual void _v03C();
    virtual void _v040();
    virtual void _v044();
    virtual void _v048();
    virtual void _v04C();
    virtual void _v050();
    virtual void _v054();
    virtual void _v058();
    virtual void _v05C();
    virtual void _v060();
    virtual void _v064();
    virtual void _v068();
    virtual void _v06C();
    virtual void _v070();
    virtual void _v074();
    virtual void _v078();
    virtual void _v07C();
    virtual void _v080();
    virtual void _v084();
    virtual void _v088();
    virtual void _v08C();
    virtual void _v090();
    virtual void _v094();
    virtual void _v098();
    virtual void _v09C();
    virtual void _v0A0();
    virtual void _v0A4();
    virtual void* _v0A8();
    virtual void _v0AC();
    virtual void _v0B0();
    virtual void _v0B4();
    virtual void _v0B8();
    virtual void _v0BC();
    virtual void _v0C0();
    virtual void _v0C4();
    virtual void _v0C8();
    virtual void _v0CC();
    virtual void _v0D0();
    virtual void _v0D4();
    virtual void _v0D8();
    virtual void _v0DC();
    virtual void _v0E0();
    virtual void _v0E4();
    virtual void _v0E8();
    virtual void _v0EC();
    virtual void _v0F0();
    virtual void _v0F4();
    virtual void _v0F8();
    virtual void _v0FC();
    virtual void _v100();
    virtual void _v104();
    virtual void _v108();
    virtual void _v10C();
    virtual void _v110();
    virtual void _v114();
    virtual void _v118();
    virtual void _v11C();
    virtual void _v120();
    virtual void _v124();
    virtual void _v128();
    virtual void _v12C();
    virtual void _v130();
    virtual void _v134();
    virtual void _v138();
    virtual void _v13C();
    virtual void _v140();
    virtual void _v144();
    virtual void _v148();
    virtual void _v14C();
    virtual void _v150();
    virtual void _v154();
    virtual void _v158();
    virtual void _v15C();
    virtual void _v160();
    virtual void _v164();
    virtual void _v168();
    virtual void _v16C();
    virtual void _v170();
    virtual void _v174();
    virtual void _v178();
    virtual void _v17C();
    virtual void _v180();
    virtual void _v184();
    virtual void _v188();
    virtual void _v18C();
    virtual void _v190();
    virtual void _v194(u32 value);
    virtual void _v198();
    virtual void _v19C();
    virtual void _v1A0();
    virtual void _v1A4();
    virtual void _v1A8();
    virtual void _v1AC();
    virtual void _v1B0();
    virtual void _v1B4();
    virtual void _v1B8();
    virtual void _v1BC();
    virtual void _v1C0();
    virtual void _v1C4();
    virtual void _v1C8();
    virtual void _v1CC();
    virtual void _v1D0();
    virtual void _v1D4();
    virtual void _v1D8();
    virtual void _v1DC();
    virtual void _v1E0();
    virtual void _v1E4();
    virtual void _v1E8();
    virtual void _v1EC();
    virtual void _v1F0();
    virtual void _v1F4();
    virtual void _v1F8();
    virtual void _v1FC();
    virtual void _v200();
    virtual void _v204();
    virtual void _v208();
    virtual void _v20C();
    virtual void _v210();
    virtual void _v214();
    virtual void _v218();
    virtual void _v21C();
    virtual void* _v220(u32 value);
};

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
                result = reinterpret_cast<CCharEffectEneVTableIf*>(self->mManager)->_v220(entry - 0x200);
            }
        }
    } else {
        result = reinterpret_cast<CCharEffectEneVTableIf*>(self->mManager)->_v220((u32)mode);
    }

    return result;
}

// func_8015CD9C: resolve object ID and attach to manager.
extern "C" void* func_8015CD9C(CCharEffectEne* self, u32 index, s32 mode) {
    void* obj = nullptr;

    if (mode < 0) {
        u16 entry = lbl_eu_80501EB8[index];
        if (entry != 0) {
            obj = reinterpret_cast<CCharEffectEneVTableIf*>(self->mManager)->_v220(entry);
        }
    } else {
        obj = reinterpret_cast<CCharEffectEneVTableIf*>(self->mManager)->_v220((u32)mode);
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
                obj = reinterpret_cast<CCharEffectEneVTableIf*>(self->mManager)->_v220(entry - 0x200);
            }
        }
    } else {
        obj = reinterpret_cast<CCharEffectEneVTableIf*>(self->mManager)->_v220((u32)mode);
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
        dataSrc = reinterpret_cast<CCharEffectEneVTableIf*>(data)->_v0A8();
    }

    func_800ACF78(target, self->mManager, (u32)dataSrc);

    reinterpret_cast<CCharEffectEneVTableIf*>(target)->_v194(bitFlag);

    func_800ACEF8(target, (void*)((u8*)(*(void**)((u8*)self->mManager + 0x98)) + 0x304));

    func_80484EB0(*(void**)((u8*)self->mManager + 0x98));

    ((void(*)(void*))func_800ACC50)(target);
}
