// Typed helpers recovered from the original CfGameManager unity translation unit.

union UnkWordFloat {
    u32 bits;
    float value;
};

struct UnkFlags8Data {
    u8 field_0x0[8];
    u32 flags_0x8;
};

struct UnkFlags84Data {
    u8 field_0x0[0x84];
    u32 flags_0x84;
};

struct UnkLinkedNode {
    UnkLinkedNode* next;
};

struct UnkReset28Data {
    u8 field_0x0[0x28];
};

struct Unk80EE4Data {
    u8 field_0x0[0x78];
    char text_0x78[0x10];
    u32 textLength_0x88;
    u16 value_0x8C;
};

struct Unk866A0Data {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u8 field_0xC[0x14];
    u32 field_0x20;
    u32 field_0x24;
    u32 field_0x28;
    Unk866A0Data* field_0x2C;
};

struct UnkFloat4 {
    float field_0x0;
    float field_0x4;
    float field_0x8;
    float field_0xC;
};

struct UnkDestructible {
    u8 field_0x0;
};

struct Unk87588TypedData {
    u8 field_0x0;
    u8 field_0x1[0x1F];
    u32 field_0x20;
    u8 field_0x24;
    u8 field_0x25[0x1F];
    u32 field_0x44;
    UnkWordFloat field_0x48;
};

class UnkClass_8009EC9C;
class UnkClass_800AA714;
class UnkClass_8009ECB0;
class UnkClass_800821F8;

class CItemImplInstances {
public:
    virtual void vfunc_0x08();
    virtual void vfunc_0x0C();
    virtual void vfunc_0x10();
    virtual void vfunc_0x14();
    virtual void vfunc_0x18();
    virtual void vfunc_0x1C();
    virtual void vfunc_0x20();
    virtual void vfunc_0x24();
    virtual void vfunc_0x28();
    virtual void vfunc_0x2C(u32 first, u32 second);
    virtual void vfunc_0x30(u32 value);
    virtual void vfunc_0x34();
    virtual void vfunc_0x38();
    virtual void vfunc_0x3C();
    virtual void vfunc_0x40(u32 first, u32 second);
};

extern "C" void CObjectState_UnkVirtualFunc7__Q22cf12CObjectStateFv(UnkFlags8Data* data,
                                                                     u32 mask) {
    data->flags_0x8 &= ~mask;
}

extern "C" void func_8008742C__Q22cf13CfGameManagerFv(UnkFlags8Data* data, u32 mask) {
    data->flags_0x8 &= ~mask;
}

extern "C" void func_80085220__Q22cf13CfGameManagerFv(UnkFlags84Data* data, u32 mask,
                                                        bool enable) {
    if (enable) data->flags_0x84 |= mask;
    else data->flags_0x84 &= ~mask;
}

extern "C" void func_80087448__Q22cf13CfGameManagerFv(UnkFlags8Data* data, u32 mask,
                                                        bool enable) {
    if (enable) data->flags_0x8 |= mask;
    else data->flags_0x8 &= ~mask;
}

extern "C" u64 func_8007F930__Q22cf13CfGameManagerFv(u32 enabled) {
    if (enabled) {
        lbl_eu_80663E24 |= 0x20000;
    } else {
        lbl_eu_80663E24 &= ~0x20000;
    }

    u32 secondaryFlags = lbl_eu_80663E28;
    if (enabled) {
        secondaryFlags |= 0x1000000;
    } else {
        secondaryFlags &= ~0x1000000;
    }
    lbl_eu_80663E28 = secondaryFlags;

    if (enabled) {
        u32 value = 0;
        value |= 0x200000;
        value |= 0x310;
        lbl_eu_80663DF8 = value;
        return (static_cast<u64>(enabled) << 32) | secondaryFlags;
    }

    u32 oldValue = lbl_eu_80663DF8;
    lbl_eu_80663DF8 = oldValue | 0xFFFFFFFF;
    return (static_cast<u64>(oldValue) << 32) | secondaryFlags;
}

extern "C" void func_8007F8DC__Q22cf13CfGameManagerFv(UnkLinkedNode** destination,
                                                        UnkLinkedNode** source) {
    *destination = *source;
    *source = (*source)->next;
}

extern "C" u32 func_80081318__Q22cf13CfGameManagerFv(u32 first, u32 second, u32 third) {
    return 0x38000000 | (first << 20) | (second << 10) | third;
}

extern "C" u32 func_80083718__Q22cf13CfGameManagerFv(u32 first, u32 second) {
    return 0x58000000 | (first << 20) | (second << 10);
}

extern "C" void func_8007C198__Q22cf13CfGameManagerFv(u32 mode, u32 first, u32 second,
                                                        u32 third);
extern "C" void func_8007C344__Q22cf13CfGameManagerFv(u32 first, u32 second, u32 third) {
    func_8007C198__Q22cf13CfGameManagerFv(0, first, second, third);
}
extern "C" void func_8007C360__Q22cf13CfGameManagerFv(u32 first, u32 second) {
    func_8007C198__Q22cf13CfGameManagerFv(1, first, second, 1);
}

extern "C" void func_80081258__Q22cf13CfGameManagerFv(UnkReset28Data* data) {
    memset(data, 0, sizeof(*data));
}

extern const float lbl_eu_8066A210;
extern "C" void CfObject_UnkVirtualFunc32__Q22cf8CfObjectFv(cf::CfObject* object,
                                                             float value) {
    object->mField4C = value * lbl_eu_8066A210;
}

extern "C" void func_80080E30__Q22cf13CfGameManagerFv(Unk82FCCData* data, u32 mask) {
    data->flags_0x68 &= ~(mask & ~0x40000000);
}

extern "C" void func_800838F4__Q22cf13CfGameManagerFv(u32 mode, u32 first, u32 second,
                                                        u32 third, u32 fourth);
extern "C" void func_80083D50__Q22cf13CfGameManagerFv(u32 first, u32 second, u32 third,
                                                        u32 fourth) {
    func_800838F4__Q22cf13CfGameManagerFv(0, first, second, third, fourth);
}

extern "C" UnkClass_8009EC9C* func_8009EC9C(u16 index);
extern "C" void func_8009E120(UnkClass_8009EC9C* object, u16 value);
extern "C" void func_8007DE94__Q22cf13CfGameManagerFv(u32 index, u32 value) {
    func_8009E120(func_8009EC9C(index), value);
}

extern "C" void func_800827A8__Q22cf13CfGameManagerFv(cf::CfGameManager* manager,
                                                        const u16* value) {
    s32 index = (manager->field_0x44 + manager->field_0x48) % manager->field_0x4C;
    manager->field_0x40[index] = *value;
    manager->field_0x48++;
}

#pragma dont_inline on
extern "C" BdatTextEntry* func_8007C2F4__Q22cf13CfGameManagerFv(BdatTextEntry* entry,
                                                                  const char* text) {
    entry->textLength = strlen(text);
    strcpy(entry->text, text);
    return entry;
}
#pragma dont_inline reset

extern "C" bool func_80083C78__Q22cf13CfGameManagerFv(const UnkFloat4* first,
                                                        const UnkFloat4* second) {
    bool equal = false;
    if (first->field_0x0 == second->field_0x0 && first->field_0x4 == second->field_0x4 &&
        first->field_0x8 == second->field_0x8 && first->field_0xC == second->field_0xC) {
        equal = true;
    }
    return equal;
}

extern "C" u16 lbl_eu_80663E42;
extern "C" u16 lbl_eu_80663E44;
extern "C" void func_800832BC__Q22cf13CfGameManagerFv(u16* first, u16* second) {
    cf::CfGameManager::getInstance();
    *first = lbl_eu_80663E42;
    *second = lbl_eu_80663E44;
}

extern "C" char lbl_eu_804FB824[];
extern "C" void func_80081358__Q22cf13CfGameManagerFv(const char* name, u32 value,
                                                        u32 third, u32 fourth);
extern "C" void func_80081A24__Q22cf13CfGameManagerFv(u32 value) {
    func_80081358__Q22cf13CfGameManagerFv(&lbl_eu_804FB824[0x6F], value, 0, 0);
}

extern "C" UnkClass_800821F8* func_80078B60(CfCamEventManager* manager, u32 mode,
                                             u32 value);
extern "C" void func_80081D8C__Q22cf13CfGameManagerFv(u32 value) {
    cf::CfGameManager* manager = cf::CfGameManager::getInstance();
    manager->unkB0 = func_80078B60(manager->unkB4, 0, value);
}

extern "C" CItemImplInstances* CItem_initItemImplInstances();
extern "C" void func_8007EF04__Q22cf13CfGameManagerFv(u32 value) {
    CItem_initItemImplInstances()->vfunc_0x30(value);
}
extern "C" void func_8007F054__Q22cf13CfGameManagerFv(u32 first, u32 second) {
    CItem_initItemImplInstances()->vfunc_0x2C(first, second);
}
extern "C" void func_8007F0C4__Q22cf13CfGameManagerFv(u32 first, u32 second) {
    CItem_initItemImplInstances()->vfunc_0x40(first, second);
}

extern "C" u32 func_8009CF8C(u32 resourceId);
extern "C" void func_8009D018(u32 resourceId, u32 value);
extern "C" s32 func_80082418__Q22cf13CfGameManagerFv(s32 first, s32 second);
extern "C" s32 func_800824FC__Q22cf13CfGameManagerFv(s32 first, s32 second) {
    s32 index = func_80082418__Q22cf13CfGameManagerFv(first, second);
    if (index == -1) {
        return -1;
    }
    if (index >= 22) {
        return -1;
    }
    return func_8009CF8C(index + 40);
}

extern "C" bool func_8008235C__Q22cf13CfGameManagerFv(u32 bit) {
    return (func_8009CF8C(0x108) & (1U << bit)) != 0;
}
extern "C" bool func_800823A4__Q22cf13CfGameManagerFv(u32 bit, bool enable) {
    u32 flags = func_8009CF8C(0x108);
    u32 mask = 1U << bit;
    bool changed = false;
    if (enable) {
        changed = (flags & mask) == 0;
        flags |= mask;
    } else {
        flags &= ~mask;
    }
    func_8009D018(0x108, flags);
    return changed;
}
extern "C" u32 func_80082354__Q22cf13CfGameManagerFv(u32 resourceId) {
    return func_8009CF8C(resourceId + 0x220);
}

extern "C" void __dl__FPv(void* object);
#define DEFINE_POD_DTOR(name) \
extern "C" UnkDestructible* name(UnkDestructible* object, s32 deleteFlag) { \
    UnkDestructible* result = object; \
    if (object != nullptr && deleteFlag > 0) __dl__FPv(object); \
    return result; \
}
DEFINE_POD_DTOR(__dt__80080400)
DEFINE_POD_DTOR(__dt__80080440)
DEFINE_POD_DTOR(__dt__80080480)
DEFINE_POD_DTOR(__dt__Q22cf11IObjectInfo)
DEFINE_POD_DTOR(__dt__Q22cf10CCharVoiceFv)
DEFINE_POD_DTOR(__dt__Q22cf16CfObjectImplMoveFv)
DEFINE_POD_DTOR(__dt__Q22cf11CCharEffectFv)
DEFINE_POD_DTOR(__dt__Q22cf13CCharEffectPcFv)
DEFINE_POD_DTOR(__dt__Q22cf14CfObjectImplPcFv)
DEFINE_POD_DTOR(__dt__Q22cf14CCharEffectNpcFv)
DEFINE_POD_DTOR(__dt__Q22cf14CCharEffectObjFv)
DEFINE_POD_DTOR(__dt__80081C20)
DEFINE_POD_DTOR(__dt__Q22cf14CCharEffectEneFv)
#undef DEFINE_POD_DTOR

extern "C" Unk87588TypedData* __dt__8008753C(Unk87588TypedData* object, s32 deleteFlag) {
    Unk87588TypedData* result = object;
    if (object != nullptr) {
        object->field_0x48.bits = 0;
        object->field_0x44 = 0;
        if (deleteFlag > 0) __dl__FPv(object);
    }
    return result;
}

#pragma dont_inline on
extern "C" s32 func_80082768__Q22cf13CfGameManagerFv(const cf::CfGameManager* queue) {
    return queue->field_0x48;
}
#pragma dont_inline reset
extern "C" u16 func_80082770__Q22cf13CfGameManagerFv(cf::CfGameManager* queue);
extern "C" void func_8008261C__Q22cf13CfGameManagerFv(u32 value, bool enable);
extern "C" cf::CfGameManager lbl_eu_80570CF0;
extern "C" void func_800827E4__Q22cf13CfGameManagerFv() {
    while (func_80082768__Q22cf13CfGameManagerFv(&lbl_eu_80570CF0) != 0) {
        u16 value = func_80082770__Q22cf13CfGameManagerFv(&lbl_eu_80570CF0);
        func_8008261C__Q22cf13CfGameManagerFv(value, true);
    }
}

extern "C" void func_80080EE4__Q22cf13CfGameManagerFv(Unk80EE4Data* data,
                                                        const char* text, u16 value) {
    data->textLength_0x88 = strlen(text);
    strcpy(data->text_0x78, text);
    data->value_0x8C = value;
}

extern "C" UnkClass_800AA714* func_800AA714(u32 value);
extern "C" void func_800B985C(UnkClass_800AA714* first, UnkClass_800AA714* second,
                                u32 value);
extern "C" void func_80081D2C__Q22cf13CfGameManagerFv(u32 first, u32 second, u32 value) {
    UnkClass_800AA714* firstObject = func_800AA714(first);
    UnkClass_800AA714* secondObject = func_800AA714(second);
    func_800B985C(firstObject, secondObject, value);
}

extern "C" u32 func_80078C08(CfCamEventManager* manager, u32 first, u32 second,
                               u32 third, u32 fourth, u32 fifth);
extern "C" u32 func_80082008__Q22cf13CfGameManagerFv(u32 first, u32 second, u32 third,
                                                       u32 fourth, u32 fifth) {
    return func_80078C08(cf::CfGameManager::getInstance()->unkB4, first, second, third,
                         fourth, fifth);
}

extern "C" UnkClass_800821F8* func_800784A0(u32 first, u32 second, u32 third, u32 fourth,
                                             u32 fifth, u32 sixth, u32 seventh);
extern "C" void func_80081F28__Q22cf13CfGameManagerFv(u32 first, u32 second) {
    cf::CfGameManager* manager = cf::CfGameManager::getInstance();
    manager->unkB0 = func_800784A0(first, second, 0, 0, 0, 0, 0);
}

extern "C" u32 func_8007E960__Q22cf13CfGameManagerFv(u32 value) {
    u32 result = 0;
    switch (value) {
    case 0: result = 4; break;
    case 1: result = 5; break;
    case 2: result = 6; break;
    case 3: result = 7; break;
    case 4: result = 8; break;
    case 5: result = 2; break;
    }
    return result;
}

extern "C" UnkClass_8009ECB0* func_8009ECB0();
extern "C" bool func_8009E344(UnkClass_8009ECB0* object, u32 value, s32* firstOut,
                                s32* secondOut);
extern "C" bool func_8007E908__Q22cf13CfGameManagerFv(u32 value) {
    s32 firstOut;
    s32 secondOut;
    UnkClass_8009ECB0* object = func_8009ECB0();
    bool found = func_8009E344(object, value, &firstOut, &secondOut);
    bool result = false;
    if (found && firstOut == 1) result = true;
    return result;
}

extern "C" u32 lbl_eu_80661BC4;
extern "C" void func_8009D018(u32 resourceId, u32 value);
extern "C" bool func_8007E1B4__Q22cf13CfGameManagerFv();
extern "C" void func_800822FC__Q22cf13CfGameManagerFv(u32 value, bool makeCurrent) {
    func_8009D018(0x20, value);
    if (!func_8007E1B4__Q22cf13CfGameManagerFv() || makeCurrent) lbl_eu_80661BC4 = value;
}

extern "C" void func_8007C188__Q22cf13CfGameManagerFv(u32 mask);
extern "C" void func_80496294(CScn* scene, float value);
extern "C" void func_801C011C(u32 value, u32 frames);
extern "C" void func_8008294C__Q22cf13CfGameManagerFv(bool enable) {
    cf::CfGameManager::getInstance();
    if (enable) {
        func_8007C188__Q22cf13CfGameManagerFv(0x40000000);
        func_80496294(lbl_eu_80663E14, lbl_eu_80666498);
    } else {
        func_8007E4CC__Q22cf13CfGameManagerFv(0x40000000);
        func_80496294(lbl_eu_80663E14, lbl_eu_8066649C);
    }
    func_801C011C(enable, 10);
}

extern "C" BdatTextEntry lbl_eu_80571628;
extern "C" bool func_80087244__Q22cf13CfGameManagerFv();
extern "C" bool func_80068B60(const char* text);
extern "C" void func_80068C04();
extern "C" void func_80083888__Q22cf13CfGameManagerFv(const char* text) {
    func_8007C2F4__Q22cf13CfGameManagerFv(&lbl_eu_80571628, text);
    if (!func_80087244__Q22cf13CfGameManagerFv() && text != nullptr && func_80068B60(text)) {
        func_80068C04();
        func_8007C188__Q22cf13CfGameManagerFv(0x8000);
    }
}

extern "C" Unk866A0Data* func_80062F60();
extern "C" void func_80065CA4(Unk866A0Data* child, Unk866A0Data* parent);
extern "C" void func_800A8E6C(u32 value, bool enable);
extern "C" void func_800866A0__Q22cf13CfGameManagerFv() {
    lbl_eu_80663E24 |= 0x400;
    Unk866A0Data* data = func_80062F60();
    func_80065CA4(data->field_0x2C, data);
    data->field_0x4 = 0;
    data->field_0x8 = 0;
    data->field_0x28 = 0;
    data->field_0x0 = 0;
    data->field_0x24 = 0;
    data->field_0x20 = 0;
    func_800A8E6C(0x10, true);
}

extern "C" void func_8006349C();
extern "C" void func_8008670C__Q22cf13CfGameManagerFv() {
    if (!lbl_eu_80663E70) {
        __ct__Q22cf13CfGameManagerFv(&lbl_eu_80571758);
        __register_global_object(&lbl_eu_80571758, __dt__Q22cf13CfGameManagerFv,
                                 lbl_eu_80571748);
        lbl_eu_80663E70 = 1;
    }
    lbl_eu_80663E24 &= ~0x400;
    func_8006349C();
}

extern "C" void func_800B9404(UnkClass_80083298* object);
extern "C" UnkClass_80083298* func_800B9524(u32 first, u32 second);
extern "C" bool func_8007DA0C__Q22cf13CfGameManagerFv(cf::CfGameManager* manager,
                                                       u32 first, u32 second) {
    func_800B9404(manager->unk90);
    manager->unk90 = nullptr;
    manager->unk90 = func_800B9524(first, second);
    return manager->unk90 != nullptr;
}

extern "C" s16 lbl_eu_80663E3E;
extern "C" u32 lbl_eu_80663EE0;
extern const float lbl_eu_80666508;
extern "C" void func_80188D34(u32 first, bool enable, float value, float fadeTime);
extern "C" void func_8007D7A4__Q22cf13CfGameManagerFv() {
    func_80188D34(0, true, lbl_eu_80666508, lbl_eu_8066649C);
    if (lbl_eu_80663E3E != 0) {
        u32 flagsEE0 = lbl_eu_80663EE0;
        s16 resetValue = -1;
        u32 flagsE28 = lbl_eu_80663E28;
        flagsEE0 &= ~0x20;
        lbl_eu_80661BCC = resetValue;
        flagsE28 &= ~8;
        lbl_eu_80663EE0 = flagsEE0;
        lbl_eu_80663E28 = flagsE28;
    }
    lbl_eu_80663E3C = 0;
    lbl_eu_80663E3E = 0;
    lbl_eu_80663E68 = lbl_eu_80666498;
}
