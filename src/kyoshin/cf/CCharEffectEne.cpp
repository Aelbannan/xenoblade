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
extern "C" void setChildF50G_(void* self);
extern "C" void func_80484EB0(void* ptr);
extern "C" u8 lbl_eu_8052FDD8[];
extern "C" void* __ct__CCharEffect(void* self);

// CCharEffectEne layout (inherits CCharEffect, vptr at 0)
struct CCharEffectEne {
    u8 pad_000[0x26C];
    u16 mFlags;
    u8 pad_26E[0x2E];
    void* mManager;
};

// Manager interface at vtable offset 0x220 (retail CfObjectMove slot)
struct MgrIf {
    virtual void d000();
    virtual void d001();
    virtual void d002();
    virtual void d003();
    virtual void d004();
    virtual void d005();
    virtual void d006();
    virtual void d007();
    virtual void d008();
    virtual void d009();
    virtual void d010();
    virtual void d011();
    virtual void d012();
    virtual void d013();
    virtual void d014();
    virtual void d015();
    virtual void d016();
    virtual void d017();
    virtual void d018();
    virtual void d019();
    virtual void d020();
    virtual void d021();
    virtual void d022();
    virtual void d023();
    virtual void d024();
    virtual void d025();
    virtual void d026();
    virtual void d027();
    virtual void d028();
    virtual void d029();
    virtual void d030();
    virtual void d031();
    virtual void d032();
    virtual void d033();
    virtual void d034();
    virtual void d035();
    virtual void d036();
    virtual void d037();
    virtual void d038();
    virtual void d039();
    virtual void d040();
    virtual void d041();
    virtual void d042();
    virtual void d043();
    virtual void d044();
    virtual void d045();
    virtual void d046();
    virtual void d047();
    virtual void d048();
    virtual void d049();
    virtual void d050();
    virtual void d051();
    virtual void d052();
    virtual void d053();
    virtual void d054();
    virtual void d055();
    virtual void d056();
    virtual void d057();
    virtual void d058();
    virtual void d059();
    virtual void d060();
    virtual void d061();
    virtual void d062();
    virtual void d063();
    virtual void d064();
    virtual void d065();
    virtual void d066();
    virtual void d067();
    virtual void d068();
    virtual void d069();
    virtual void d070();
    virtual void d071();
    virtual void d072();
    virtual void d073();
    virtual void d074();
    virtual void d075();
    virtual void d076();
    virtual void d077();
    virtual void d078();
    virtual void d079();
    virtual void d080();
    virtual void d081();
    virtual void d082();
    virtual void d083();
    virtual void d084();
    virtual void d085();
    virtual void d086();
    virtual void d087();
    virtual void d088();
    virtual void d089();
    virtual void d090();
    virtual void d091();
    virtual void d092();
    virtual void d093();
    virtual void d094();
    virtual void d095();
    virtual void d096();
    virtual void d097();
    virtual void d098();
    virtual void d099();
    virtual void d100();
    virtual void d101();
    virtual void d102();
    virtual void d103();
    virtual void d104();
    virtual void d105();
    virtual void d106();
    virtual void d107();
    virtual void d108();
    virtual void d109();
    virtual void d110();
    virtual void d111();
    virtual void d112();
    virtual void d113();
    virtual void d114();
    virtual void d115();
    virtual void d116();
    virtual void d117();
    virtual void d118();
    virtual void d119();
    virtual void d120();
    virtual void d121();
    virtual void d122();
    virtual void d123();
    virtual void d124();
    virtual void d125();
    virtual void d126();
    virtual void d127();
    virtual void d128();
    virtual void d129();
    virtual void d130();
    virtual void d131();
    virtual void d132();
    virtual void d133();
    virtual void* get220(u32 id);
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
                result = reinterpret_cast<MgrIf*>(self->mManager)->get220(entry - 0x200);
            }
        }
    } else {
        result = reinterpret_cast<MgrIf*>(self->mManager)->get220((u32)mode);
    }
    return result;
}

extern "C" void* func_8015CD9C(CCharEffectEne* self, u32 index, s32 mode) {
    void* obj = nullptr;
    if (mode < 0) {
        u16 entry = lbl_eu_80501EB8[index];
        if (entry != 0) {
            obj = reinterpret_cast<MgrIf*>(self->mManager)->get220(entry);
        }
    } else {
        obj = reinterpret_cast<MgrIf*>(self->mManager)->get220((u32)mode);
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
                obj = reinterpret_cast<MgrIf*>(self->mManager)->get220(entry - 0x200);
            }
        }
    } else {
        obj = reinterpret_cast<MgrIf*>(self->mManager)->get220((u32)mode);
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
        dataSrc = reinterpret_cast<void*>(reinterpret_cast<CfObjectImplPcTgt*>(data)->vfA8());
    }
    bindPartnerO_(target, self->mManager, (u32)(uintptr_t)dataSrc);
    reinterpret_cast<cf::CfObjectEff*>(target)->func_800ACF50(bitFlag != 0);
    setChild34Sc_(target, (void*)((u8*)(*(void**)((u8*)self->mManager + 0x98)) + 0x304));
    func_80484EB0(*(void**)((u8*)self->mManager + 0x98));
    setChildF50G_(target);
}
