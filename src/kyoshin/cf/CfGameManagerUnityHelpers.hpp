// Typed helpers recovered from the original CfGameManager unity translation unit.

namespace ml {
#pragma dont_inline on
template <>
FixStr<64>::FixStr() {
    clear();
}
#pragma dont_inline reset
}

extern "C" void func_8016EC58(u32 object);
extern "C" bool func_8007CBD4__Q22cf13CfGameManagerFv(u32 mask);
extern "C" void func_8007CE94__Q22cf13CfGameManagerFv() {
    if (!lbl_eu_80663E70) {
        __ct__Q22cf13CfGameManagerFv(&lbl_eu_80571758);
        __register_global_object(&lbl_eu_80571758,
                                 __dt__Q22cf13CfGameManagerFv,
                                 lbl_eu_80571748);
        lbl_eu_80663E70 = 1;
    }
    cf::CfGameManager* manager = &lbl_eu_80571758;
    if (manager != nullptr) {
        if (!lbl_eu_80663E70) {
            __ct__Q22cf13CfGameManagerFv(manager);
            __register_global_object(manager, __dt__Q22cf13CfGameManagerFv,
                                     lbl_eu_80571748);
            lbl_eu_80663E70 = 1;
        }
        if (func_8007CBD4__Q22cf13CfGameManagerFv(0x1000)) {
            cf::CfObjectMove* player = cf::CfGameManager::getPlayer(0);
            if (player != nullptr) {
                func_8016EC58(player->CfObject_UnkVirtualFunc23());
            }
        }
    }
}

extern "C" void func_80086E6C__Q22cf13CfGameManagerFv(float amount) {
    cf::CfPadTask::func_801C1BD8(amount);
    if (lbl_eu_80663E0C != nullptr && amount > lbl_eu_80666498) {
        u32 enabledFlags = lbl_eu_80663DF8;
        enabledFlags &= ~0x6006F0;
        lbl_eu_80663E0C->mPressedButtonFlags &= enabledFlags;
        lbl_eu_80663E0C->mTurboPressButtonFlags &= enabledFlags;
        lbl_eu_80663E0C->mReleasedButtonFlags &= enabledFlags;
        lbl_eu_80663E0C->mHeldButtonFlags &= enabledFlags;
        lbl_eu_80663E0C->mLongHoldButtonFlags &= enabledFlags;
        lbl_eu_80663E0C->mShortPressButtonFlags &= enabledFlags;
        lbl_eu_80571500.mPressedButtonFlags &= enabledFlags;
        lbl_eu_80571500.mTurboPressButtonFlags &= enabledFlags;
    }
}

extern "C" u32 func_800822F4__Q22cf13CfGameManagerFv();

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

struct ItemContainerPrefix {
    u8 field_0x0[0x3E9C];
};

struct ItemListSubobject {
    u32 field_0x0;
};

struct ItemListObject : public ItemContainerPrefix, public ItemListSubobject {
    u8 field_0x3EA0[0x88];
    u16 itemId_0x3F28;
};

struct ItemListEntryView {
    u8 field_0x0[0x8C];
    u16 itemId_0x8C;
};

struct ItemListNode {
    ItemListNode* next;
    u32 field_0x4;
    ItemListSubobject* object;
};

struct ItemListManager {
    u32 field_0x0;
    ItemListNode* sentinel;
};

struct UnkCharEffect304 {
    void* vtable_0x0;
    u8 field_0x4[0x2FC];
    u32 field_0x300;
};

struct Unk80338Object {
    void* vtable_0x0;
    u32 field_0x4;
    u32 field_0x8;
    void* vtable_0xC;
    void* vtable_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u32 field_0x1C;
    u8 field_0x20[8];
    CCharVoice voice_0x28;
    UnkCharEffect304 effect_0x68;
    u32 field_0x36C;
    u32 field_0x370;
    u32 field_0x374;
    u32 field_0x378;
    u32 field_0x37C;
};

struct Unk814BCObject {
    void* vtable_0x0;
    u32 field_0x4;
    u32 field_0x8;
    void* vtable_0xC;
    void* vtable_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u32 field_0x1C;
    u8 field_0x20[8];
    CCharVoice voice_0x28;
    UnkCharEffect304 effect_0x68;
};

struct Unk81B80Object {
    void* vtable_0x0;
    u32 field_0x4;
    u32 field_0x8;
    void* vtable_0xC;
    void* vtable_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u32 field_0x1C;
    u8 field_0x20[8];
    CCharVoice voice_0x28;
    void* vtable_0x68;
    u8 effect_0x6C[0x300];
    u32 field_0x36C;
};

struct UnkReset28Data {
    u8 field_0x0[0x28];
};

class Unk80EE4Data {
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
    virtual void vfunc_0x2C();
    virtual void vfunc_0x30();
    virtual void vfunc_0x34();
    virtual void vfunc_0x38();
    virtual void vfunc_0x3C();
    virtual void vfunc_0x40();
    virtual void vfunc_0x44();
    virtual void vfunc_0x48();
    virtual void vfunc_0x4C();
    virtual void vfunc_0x50();
    virtual void vfunc_0x54();
    virtual void vfunc_0x58();
    virtual void vfunc_0x5C();
    virtual void vfunc_0x60();
    virtual void vfunc_0x64();
    virtual void vfunc_0x68();
    virtual void vfunc_0x6C();
    virtual void vfunc_0x70(void* object);
    virtual void vfunc_0x74();
    virtual void vfunc_0x78();
    virtual void vfunc_0x7C();
    virtual void vfunc_0x80();
    virtual void vfunc_0x84();
    virtual void vfunc_0x88();
    virtual void vfunc_0x8C();
    virtual void vfunc_0x90();
    virtual void vfunc_0x94();
    virtual void vfunc_0x98();
    virtual void vfunc_0x9C();
    virtual void vfunc_0xA0();
    virtual void vfunc_0xA4();
    virtual void vfunc_0xA8();
    virtual void vfunc_0xAC();
    virtual void vfunc_0xB0();
    virtual void vfunc_0xB4();
    virtual void vfunc_0xB8();
    virtual void vfunc_0xBC();
    virtual void vfunc_0xC0();
    virtual void vfunc_0xC4();
    virtual void vfunc_0xC8();
    virtual void vfunc_0xCC();
    virtual void vfunc_0xD0();
    virtual void vfunc_0xD4();
    virtual void vfunc_0xD8();
    virtual void vfunc_0xDC();
    virtual void vfunc_0xE0();
    virtual void vfunc_0xE4();
    virtual void vfunc_0xE8();
    virtual void vfunc_0xEC();
    virtual void vfunc_0xF0();
    virtual void vfunc_0xF4();
    virtual void vfunc_0xF8();
    virtual void vfunc_0xFC();
    virtual void vfunc_0x100();
    virtual void vfunc_0x104();
    virtual void vfunc_0x108();
    virtual void vfunc_0x10C(u32 mode);

    u8 field_0x4[0x74];
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

class VoiceAction {
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
    virtual void vfunc_0x2C();
    virtual void vfunc_0x30();
    virtual void vfunc_0x34();
    virtual void vfunc_0x38();
    virtual void vfunc_0x3C();
    virtual void vfunc_0x40();
    virtual void vfunc_0x44();
    virtual void vfunc_0x48();
    virtual void vfunc_0x4C();
    virtual void vfunc_0x50();
    virtual void vfunc_0x54();
    virtual void vfunc_0x58();
    virtual void vfunc_0x5C();
    virtual void vfunc_0x60();
    virtual void vfunc_0x64();
    virtual void vfunc_0x68();
    virtual void vfunc_0x6C();
    virtual void vfunc_0x70();
    virtual void vfunc_0x74();
    virtual void vfunc_0x78();
    virtual void vfunc_0x7C();
    virtual void vfunc_0x80();
    virtual void vfunc_0x84();
    virtual void vfunc_0x88();
    virtual void vfunc_0x8C();
    virtual void vfunc_0x90();
    virtual void vfunc_0x94();
    virtual void vfunc_0x98();
    virtual void vfunc_0x9C();
    virtual void vfunc_0xA0();
    virtual void vfunc_0xA4();
    virtual void vfunc_0xA8(bool enable);
};

struct VoiceSource {
    u8 field_0x0[0x64];
    u32 flags_0x64;
};

struct VoiceListNode {
    VoiceListNode* next;
    VoiceListNode* previous;
    VoiceSource* value;
};

struct VoiceList {
    VoiceListNode* field_0x0;
    VoiceListNode* end;
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

class UnkClass_8009EC9C {
public:
    u8 field_0x0[0x18];
    u16 textId_0x18;
    u8 field_0x1A[0x3534 - 0x1A];
    UnkF044Data itemCounts_0x3534;
};
class UnkClass_800AA714;
class UnkClass_8009ECB0 {
public:
    u32 field_0x0;
    union {
        s32 entries_0x4[7];
        struct {
            u8 field_0x4[0x14];
            u16 textId_0x18;
            u8 field_0x1A[6];
        };
    };
};
class UnkClass_800821F8 {
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
    virtual void vfunc_0x2C();
    virtual void vfunc_0x30();
    virtual void vfunc_0x34();
    virtual void vfunc_0x38();
    virtual void vfunc_0x3C();
    virtual void vfunc_0x40(u32 mode);
};

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
    virtual UnkF0ACData* vfunc_0x2C(u32 first, u32 second);
    virtual u16 vfunc_0x30(u32 value);
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

extern "C" void func_8007F930__Q22cf13CfGameManagerFv(bool enabled) {
    if (enabled) {
        lbl_eu_80663E24 |= 0x20000;
    } else {
        lbl_eu_80663E24 &= ~0x20000;
    }

    if (enabled) {
        lbl_eu_80663E28 |= 0x1000000;
    } else {
        lbl_eu_80663E28 &= ~0x1000000;
    }

    if (enabled) {
        lbl_eu_80663DF8 = 0x200310;
    } else {
        lbl_eu_80663DF8 |= 0xFFFFFFFF;
    }
}

#pragma dont_inline on
extern "C" void func_8007F8DC__Q22cf13CfGameManagerFv(
    UnkF8C0Node* destination, UnkF8C0Node* source, u32 unused) {
    destination->field_0x0 = source->field_0x0;
    ItemListNode* node = reinterpret_cast<ItemListNode*>(source->field_0x0);
    source->field_0x0 = reinterpret_cast<u32>(node->next);
}
#pragma dont_inline reset

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

#pragma dont_inline on
extern "C" void func_80080E30__Q22cf13CfGameManagerFv(cf::CfObject* data, u32 mask) {
    data->mFlags68 &= ~(mask & ~0x40000000);
}
#pragma dont_inline reset

extern const float lbl_eu_80666538;
extern "C" void func_800BC3B0(cf::CfObjectMove* player, float value);
extern "C" bool func_8007FD00__Q22cf13CfGameManagerFv(u32 value);
class UnkClass_800817BC {
public:
    u8 field_0x0[0x64];
    u32 flags_0x64;
    u8 field_0x68[0xA8 - 0x68];
    u32 field_0xA8;
};
typedef UnkClass_800817BC Unk8187CData;
extern const float lbl_eu_806664A0;
extern "C" UnkClass_800817BC* func_800817BC__Q22cf13CfGameManagerFv(u32 value,
                                                                      u32 index);
extern "C" void func_800ACF78(UnkClass_800817BC* object, cf::CfObjectMove* player,
                                u32 value);
extern "C" void func_801BFDE8(u32 mode, u32 value, u32 playerValue, float first,
                                float second);
extern "C" void func_800BC3D8(cf::CfObjectMove* player, float value);
extern const float lbl_eu_80666564;
extern "C" void func_800ACC28(UnkClass_800817BC* object, float first, float second);
extern "C" void func_800BC4A0(cf::CfObjectMove* player);
extern "C" void func_8008402C__Q22cf13CfGameManagerFv(u32 objectValue,
                                                        bool triggerFirstPlayer,
                                                        float value) {
    for (s32 i = 0; i < 3; ++i) {
        cf::CfObjectMove* player = cf::CfGameManager::getPlayer(i);
        if (player != nullptr) {
            if (objectValue != 0) {
                UnkClass_800817BC* object =
                    func_800817BC__Q22cf13CfGameManagerFv(objectValue, 0);
                if (object != nullptr) {
                    func_800ACC28(object, lbl_eu_80666564, lbl_eu_8066649C);
                    func_800ACF78(object, player, 0);
                }
            }
            if (triggerFirstPlayer && i == 0) {
                u32 playerValue = player->CfObject_UnkVirtualFunc23();
                func_801BFDE8(1, triggerFirstPlayer, playerValue,
                              lbl_eu_8066649C, lbl_eu_806664A0);
            }
            func_800BC4A0(player);
            player->CfObject_UnkVirtualFunc70(lbl_eu_8066649C);
            func_800BC3B0(player, value);
        }
    }
}

extern "C" void func_80083F28__Q22cf13CfGameManagerFv(u32 objectValue,
                                                        u32 triggerFirstPlayer,
                                                        float value) {
    for (s32 i = 0; i < 3; ++i) {
        cf::CfObjectMove* player = cf::CfGameManager::getPlayer(i);
        if (player != nullptr) {
            if (objectValue != 0) {
                UnkClass_800817BC* object =
                    func_800817BC__Q22cf13CfGameManagerFv(objectValue, 0);
                if (object != nullptr) {
                    func_800ACF78(object, player, 0);
                }
            }
            if (triggerFirstPlayer && i == 0) {
                u32 playerValue = player->CfObject_UnkVirtualFunc23();
                func_801BFDE8(1, triggerFirstPlayer, playerValue,
                              lbl_eu_8066649C, lbl_eu_806664A0);
            }
            player->CfObject_UnkVirtualFunc70(lbl_eu_80666498);
            func_800BC3D8(player, value);
        }
    }
}

extern "C" void func_80083EA4__Q22cf13CfGameManagerFv() {
    for (s32 i = 0; i < 3; ++i) {
        cf::CfObjectMove* player = cf::CfGameManager::getPlayer(i);
        if (player != nullptr) {
            func_80080E30__Q22cf13CfGameManagerFv(player, 0x10000000);
            player->CfObject_UnkVirtualFunc70(lbl_eu_8066649C);
            func_800BC3B0(player, lbl_eu_80666538);
        }
    }
    func_8007FD00__Q22cf13CfGameManagerFv(28);
}

extern "C" void func_800838F4__Q22cf13CfGameManagerFv(u32 mode, u32 first, u32 second,
                                                        u32 third, u32 fourth, float value);
extern "C" void func_80135568(u32 value);
extern "C" void func_80083D70__Q22cf13CfGameManagerFv(u32 first, u32 second,
                                                        u32 third, u32 fourth,
                                                        float value) {
    func_80135568(0);
    func_800838F4__Q22cf13CfGameManagerFv(1, first, second, third, fourth, value);
}
extern "C" void func_80083D50__Q22cf13CfGameManagerFv(u32 first, u32 second, u32 third,
                                                        u32 fourth, float value) {
    func_800838F4__Q22cf13CfGameManagerFv(0, first, second, third, fourth, value);
}
extern "C" void func_8007C188__Q22cf13CfGameManagerFv(u32 mask);
extern "C" float lbl_eu_8066653C;
extern "C" u16 lbl_eu_80663E48[2];
extern "C" u16 lbl_eu_80663E4C[2];
extern "C" void func_80083DEC__Q22cf13CfGameManagerFv(
    u32 first, u32 second, u32 third, u32 fourth, u32 fifth, u32 sixth,
    u32 seventh, u32 eighth, float value) {
    func_80135568(0);
    cf::CfGameManager::getInstance();
    lbl_eu_80663E48[1] = seventh;
    lbl_eu_80663E48[0] = fifth;
    lbl_eu_80663E4C[0] = sixth;
    lbl_eu_80663E4C[1] = eighth;
    func_800838F4__Q22cf13CfGameManagerFv(2, first, second, third, fourth, value);
    func_8007C188__Q22cf13CfGameManagerFv(0x80000);
    func_80083F28__Q22cf13CfGameManagerFv(fifth, sixth, lbl_eu_8066653C);
}

extern "C" UnkClass_8009EC9C* func_8009EC9C(u16 index);
extern "C" const void* func_8009E120(UnkClass_8009EC9C* object, u16 value);
extern "C" void func_8007DE94__Q22cf13CfGameManagerFv(u32 index, u32 value) {
    func_8009E120(func_8009EC9C(index), value);
}

#pragma dont_inline on
extern "C" void func_800827A8__Q22cf13CfGameManagerFv(cf::CfGameManager* manager,
                                                        const u16* value) {
    s32 index = (manager->field_0x44 + manager->field_0x48) % manager->field_0x4C;
    manager->field_0x40[index] = *value;
    manager->field_0x48++;
}
#pragma dont_inline reset

#pragma dont_inline on
extern "C" BdatTextEntry* func_8007C2F4__Q22cf13CfGameManagerFv(BdatTextEntry* entry,
                                                                  const char* text) {
    entry->textLength = strlen(text);
    strcpy(entry->text, text);
    return entry;
}
#pragma dont_inline reset

extern "C" BdatTextEntry* func_80083CD8__Q22cf13CfGameManagerFv(
    BdatTextEntry* destination, const BdatTextEntry* source) {
    destination->textLength = strlen(source->text);
    strcpy(destination->text, source->text);
    destination->secondaryTextLength = strlen(source->secondaryText);
    strcpy(destination->secondaryText, source->secondaryText);
    destination->value = source->value;
    destination->enabled = source->enabled;
    return destination;
}

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
extern "C" u32 func_eu_80065640(u32 first, u32 second, u32 third, u32 fourth);
extern "C" Unk8187CData* func_80080F40__Q22cf13CfGameManagerFv(u32 first, u32 second,
                                                                 u32 third);
extern "C" void func_80081874__Q22cf13CfGameManagerFv(Unk8187CData* data, u32 flags);
class Unk817A8Object;
extern "C" Unk817A8Object* func_800817A8__Q22cf13CfGameManagerFv();
extern "C" bool func_800817B0__Q22cf13CfGameManagerFv();
extern "C" bool func_80069C28(Unk817A8Object* object);
extern "C" u32 CfRes_packThreeFields(u32 first, u32 second, u32 third);
extern "C" UnkClass_800817BC* func_800817BC__Q22cf13CfGameManagerFv(
    u32 value, u32 unused) {
    cf::CfGameManager::getInstance();
    Unk817A8Object* object = func_800817A8__Q22cf13CfGameManagerFv();
    UnkClass_800817BC* result = nullptr;
    if (!func_800817B0__Q22cf13CfGameManagerFv() && func_80069C28(object)) {
        u32 packed = CfRes_packThreeFields(12, lbl_eu_80663E42,
                                           lbl_eu_80663E44);
        u32 resource = func_eu_80065640(12, lbl_eu_80663E42,
                                        lbl_eu_80663E44, value);
        result = func_80080F40__Q22cf13CfGameManagerFv(packed, resource, 0);
        if (result != nullptr) {
            func_80081874__Q22cf13CfGameManagerFv(result, 0x100020);
        }
    }
    return result;
}

extern "C" Unk8187CData* func_8008187C__Q22cf13CfGameManagerFv(u32 value) {
    cf::CfGameManager::getInstance();
    u32 packedId = func_8007DCA8__Q22cf13CfGameManagerFv(14, 1);
    u32 resource = func_eu_80065640(14, 1, 0, value);
    Unk8187CData* result =
        func_80080F40__Q22cf13CfGameManagerFv(packedId, resource, 0);
    if (result != nullptr) {
        func_80081874__Q22cf13CfGameManagerFv(result, 0x400020);
    }
    return result;
}

#pragma dont_inline on
extern "C" void func_80081988__Q22cf13CfGameManagerFv(Unk8187CData* data, u32 value) {
    data->field_0xA8 = value;
}
#pragma dont_inline reset

extern "C" Unk8187CData* func_80081900__Q22cf13CfGameManagerFv(u32 first,
                                                                 u32 value,
                                                                 u32 resource) {
    Unk8187CData* result = nullptr;
    if (resource != 0 && value != 0) {
        cf::CfGameManager::getInstance();
        result = func_80080F40__Q22cf13CfGameManagerFv(resource, first, 0);
        if (result != nullptr) {
            func_80081988__Q22cf13CfGameManagerFv(result, value);
        }
    }
    return result;
}

extern "C" void func_80081358__Q22cf13CfGameManagerFv(const char* name, u32 value,
                                                        u32 third, u32 fourth);
extern "C" void func_80081A24__Q22cf13CfGameManagerFv(u32 value) {
    func_80081358__Q22cf13CfGameManagerFv(&lbl_eu_804FB824[0x6F], value, 0, 0);
}

extern "C" UnkClass_800821F8* func_80078B60(CfCamEventManager* manager, u32 mode,
                                             u32 value);
#pragma dont_inline on
extern "C" void func_80081D8C__Q22cf13CfGameManagerFv(u32 value) {
    cf::CfGameManager* manager = cf::CfGameManager::getInstance();
    manager->unkB0 = func_80078B60(manager->unkB4, 0, value);
}
#pragma dont_inline reset

extern "C" float lbl_eu_8066654C;
extern "C" u16 lbl_eu_80663E40;
extern "C" cf::CfObjectMove* func_8007FF6C__Q22cf13CfGameManagerFv(
    u16 objectId, u32 value, u32 unused, float amount);
extern "C" u32 func_8007FC5C__Q22cf13CfGameManagerFv(
    u32 first, u32 second, u32 third, u32 fourth, u32 fifth);
extern "C" cf::CfObjectMove* func_800807BC__Q22cf13CfGameManagerFv(
    u32 value, float amount) {
    cf::CfGameManager::getInstance();
    cf::CfObjectMove* player = cf::CfGameManager::getPlayer(0);
    if (player != nullptr) {
        player->CfObject_UnkVirtualFunc26(value, lbl_eu_8066654C);
        player->CfObject_UnkVirtualFunc33(amount);
        return player;
    }
    cf::CfObjectMove* object = func_8007FF6C__Q22cf13CfGameManagerFv(
        lbl_eu_80663E40, value, 0, amount);
    func_80081D8C__Q22cf13CfGameManagerFv(reinterpret_cast<u32>(object));
    func_8007FC5C__Q22cf13CfGameManagerFv(4, 0, 0, 0, 0);
    return object;
}

extern "C" CItemImplInstances* CItem_initItemImplInstances();
#pragma dont_inline on
extern "C" u16 func_8007EF04__Q22cf13CfGameManagerFv(u32 value) {
    return CItem_initItemImplInstances()->vfunc_0x30(value);
}
extern "C" UnkF0ACData* func_8007F054__Q22cf13CfGameManagerFv(u32 first, u32 second) {
    return CItem_initItemImplInstances()->vfunc_0x2C(first, second);
}
#pragma dont_inline reset
#pragma dont_inline on
extern "C" void func_8007F0C4__Q22cf13CfGameManagerFv(u32 first, u32 second) {
    CItem_initItemImplInstances()->vfunc_0x40(first, second);
}
#pragma dont_inline reset
extern "C" void func_801421C4(u16 value);
extern "C" CfGameManagerData1C* func_8007EEF0__Q22cf13CfGameManagerFv(
    UnkClass_8009EC9C* data, u32 index);
extern "C" bool func_8007F0AC__Q22cf13CfGameManagerFv(const UnkF0ACData* data);
extern "C" void func_8007EF4C__Q22cf13CfGameManagerFv(u16 dataId) {
    UnkClass_8009EC9C* data = func_8009EC9C(dataId);
    u32 total = 0;
    for (s32 i = 0; i < 5; ++i) {
        total += func_8007F044__Q22cf13CfGameManagerFv(&data->itemCounts_0x3534, i);
    }
    for (s32 i = 0; i <= 5; ++i) {
        u32 value = func_8007EEE0__Q22cf13CfGameManagerFv(
            reinterpret_cast<const UnkEEE0Data*>(data), static_cast<u16>(i));
        if (value == 0) {
            continue;
        }
        func_801421C4(value);
        CfGameManagerData1C* group =
            func_8007EEF0__Q22cf13CfGameManagerFv(data, i);
        if (group == nullptr) {
            continue;
        }
        for (s32 j = 0; j < func_8007EF04__Q22cf13CfGameManagerFv(
                                    reinterpret_cast<u32>(group));
             ++j) {
            UnkF0ACData* item = func_8007F054__Q22cf13CfGameManagerFv(
                reinterpret_cast<u32>(group), j);
            if (item != nullptr && !func_8007F0AC__Q22cf13CfGameManagerFv(item)) {
                func_8007F0C4__Q22cf13CfGameManagerFv(
                    reinterpret_cast<u32>(group), j);
            }
        }
    }
}

union ResourceDestination {
    u32 id;
    const void* pointer;
};
extern "C" u32 func_8009CF8C(u32 resourceId);
extern "C" void func_8009D018(u32 destination, u32 value);
struct ResourceIndexTable {
    s8 values[8][8];
};
extern "C" const ResourceIndexTable lbl_eu_804FB7B0;
#pragma dont_inline on
extern "C" s32 func_80082418__Q22cf13CfGameManagerFv(s32 first, s32 second) {
    ResourceIndexTable table = lbl_eu_804FB7B0;
    if (first < 1) {
        return -1;
    }
    if (first > 8) {
        return -1;
    }
    if (second < 1) {
        return -1;
    }
    if (second > 8) {
        return -1;
    }
    return table.values[first - 1][second - 1];
}
#pragma dont_inline reset
extern "C" s32 func_800824FC__Q22cf13CfGameManagerFv(s32 first, s32 second);
extern "C" void func_8013DB6C(u32 first, u32 second, s32 third, s32 fourth);
extern "C" void func_80082568__Q22cf13CfGameManagerFv(
    s32 first, s32 second, s32 increment) {
    s32 value = func_800824FC__Q22cf13CfGameManagerFv(first, second);
    if (value == -1) {
        return;
    }
    s32 index = func_80082418__Q22cf13CfGameManagerFv(first, second);
    if (index == -1) {
        return;
    }
    if (index >= 22) {
        return;
    }
    value += increment;
    func_80082544__Q22cf13CfGameManagerFv(0, &value, 5000);
    func_8009D018(index + 0x28, value);
    func_8013DB6C(5, 0, index, increment);
}

#pragma dont_inline on
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
#pragma dont_inline reset

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
extern "C" bool func_80087250__Q22cf13CfGameManagerFv();
extern "C" bool func_80082680__Q22cf13CfGameManagerFv();
extern "C" void func_8008261C__Q22cf13CfGameManagerFv(u32 value, bool enable) {
    if (func_800822F4__Q22cf13CfGameManagerFv() > 3 ||
        func_80087250__Q22cf13CfGameManagerFv()) {
        if (!func_80082680__Q22cf13CfGameManagerFv()) {
            func_8009D018(value + 0x312C, enable);
        }
    }
}

extern "C" void func_8008269C__Q22cf13CfGameManagerFv(
    cf::CfGameManager* manager, u32 value) {
    if (value <= 0xFFFF && !func_80082680__Q22cf13CfGameManagerFv()) {
        func_8009D018(reinterpret_cast<u32>(&manager->field_0x40), value);
    }
}

extern "C" u16 func_80082770__Q22cf13CfGameManagerFv(cf::CfGameManager* queue);
extern "C" cf::CfGameManager lbl_eu_80570CF0;
extern "C" bool func_80082614__Q22cf13CfGameManagerFv(u32 value);
extern "C" void func_800826F0__Q22cf13CfGameManagerFv(u32 value) {
    if (!func_80082614__Q22cf13CfGameManagerFv(value)) {
        if (static_cast<u32>(func_80082768__Q22cf13CfGameManagerFv(&lbl_eu_80570CF0)) >= 32) {
            u16 removed = func_80082770__Q22cf13CfGameManagerFv(&lbl_eu_80570CF0);
            func_8008261C__Q22cf13CfGameManagerFv(removed, true);
        }
        u16 queued = static_cast<u16>(value);
        func_800827A8__Q22cf13CfGameManagerFv(&lbl_eu_80570CF0, &queued);
    }
}

extern "C" void func_80082834__Q22cf13CfGameManagerFv(u16* values) {
    u16* output = values;
    s32 count = 0;
    while (func_80082768__Q22cf13CfGameManagerFv(&lbl_eu_80570CF0) != 0) {
        *output = func_80082770__Q22cf13CfGameManagerFv(&lbl_eu_80570CF0);
        ++output;
        ++count;
    }
    values[count] = 0;
    for (s32 i = 0; i < count; ++values, ++i) {
        func_800827A8__Q22cf13CfGameManagerFv(&lbl_eu_80570CF0, values);
    }
}

extern "C" void func_800827E4__Q22cf13CfGameManagerFv() {
    while (func_80082768__Q22cf13CfGameManagerFv(&lbl_eu_80570CF0) != 0) {
        u16 value = func_80082770__Q22cf13CfGameManagerFv(&lbl_eu_80570CF0);
        func_8008261C__Q22cf13CfGameManagerFv(value, true);
    }
}

#pragma dont_inline on
extern "C" void func_80080EE4__Q22cf13CfGameManagerFv(Unk80EE4Data* data,
                                                        const char* text, u16 value) {
    data->textLength_0x88 = strlen(text);
    strcpy(data->text_0x78, text);
    data->value_0x8C = value;
}
#pragma dont_inline reset

extern "C" Unk80EE4Data* func_800B9548();
extern "C" Unk80EE4Data* func_80081CBC__Q22cf13CfGameManagerFv(const char* text,
                                                                 u16 value) {
    Unk80EE4Data* data = func_800B9548();
    if (data != nullptr) {
        func_80080EE4__Q22cf13CfGameManagerFv(data, text, value);
        data->vfunc_0x48();
    }
    return data;
}

extern "C" UnkClass_800AA714* func_800AA714(u32 value);
extern "C" Unk80EE4Data* func_800B985C(UnkClass_800AA714* first,
                                          UnkClass_800AA714* second, u32 value);
#pragma dont_inline on
extern "C" Unk80EE4Data* func_80081D2C__Q22cf13CfGameManagerFv(
    u32 first, u32 second, u32 value) {
    UnkClass_800AA714* firstObject = func_800AA714(first);
    UnkClass_800AA714* secondObject = func_800AA714(second);
    return func_800B985C(firstObject, secondObject, value);
}
#pragma dont_inline reset

class BdatFilePointer;
extern "C" void func_8003AA34();
extern "C" BdatFilePointer* getFP__FPCc(const char* name);
extern "C" const char* getBdatStringColumnValue(BdatFilePointer* file,
                                                  const char* column, u16 index);
extern "C" void func_8003B1EC(BdatFilePointer* file);
extern "C" void func_8003B41C(BdatFilePointer* file);
#pragma dont_inline on
extern "C" Unk80EE4Data* func_80080E44__Q22cf13CfGameManagerFv(
    const char* name, u16 index) {
    func_8003AA34();
    const char* value;
    BdatFilePointer* file = getFP__FPCc(name);
    value = getBdatStringColumnValue(file, &lbl_eu_804FB824[0x84], index);
    func_8003B1EC(file);
    func_8003B41C(file);
    Unk80EE4Data* data = func_80081D2C__Q22cf13CfGameManagerFv(
        reinterpret_cast<u32>(value), 0, 0);
    func_80080EE4__Q22cf13CfGameManagerFv(data, name, index);
    return data;
}
#pragma dont_inline reset

extern "C" u8 lbl_eu_8052AC98[];
extern "C" u8 lbl_eu_8052ADC0[];
extern "C" void __ct__CCharVoice(CCharVoice* voice);
extern "C" void __ct__cf_CCharEffectEne(void* effect);
extern "C" Unk81B80Object* __ct__80081B80(Unk81B80Object* self) {
    self->field_0x4 = 0;
    self->field_0x8 = 0;
    self->vtable_0x0 = lbl_eu_8052AC98;
    self->vtable_0xC = &lbl_eu_8052AC98[0xB4];
    self->vtable_0x10 = &lbl_eu_8052AC98[0xC4];
    self->field_0x14 = 0;
    self->field_0x18 = 0;
    self->field_0x1C = 0;
    __ct__CCharVoice(&self->voice_0x28);
    self->vtable_0x0 = lbl_eu_8052ADC0;
    self->vtable_0xC = &lbl_eu_8052ADC0[0xB4];
    self->vtable_0x10 = &lbl_eu_8052ADC0[0xC4];
    self->vtable_0x68 = &lbl_eu_8052ADC0[0xF0];
    __ct__cf_CCharEffectEne(&self->effect_0x6C);
    self->field_0x36C = 0;
    return self;
}

extern "C" void func_8007F8C0__Q22cf13CfGameManagerFv(
    UnkF8C0Node* destination, const UnkF8C0Source* source);
extern "C" ItemListSubobject** func_8007F8D0__Q22cf13CfGameManagerFv(
    UnkF8C0Node* iterator);
extern "C" u16 func_8007F8B8__Q22cf13CfGameManagerFv(ItemListSubobject* object);
#pragma dont_inline on
extern "C" void func_8007F8F4__Q22cf13CfGameManagerFv(
    UnkF8C0Node* destination, const ItemListManager* source) {
    destination->field_0x0 = reinterpret_cast<u32>(source->sentinel);
}
#pragma dont_inline reset
extern "C" u16 lbl_eu_80663E40;
extern "C" ItemListManager* func_800B6BA4__Fv();
extern "C" void func_800C01D4(ItemListObject* object, void* destination,
                                u16 itemId);
extern "C" void __ct__8009ED08(void* destination, u16 itemId);
extern "C" void func_8007F830__Q22cf13CfGameManagerFv(void* destination,
                                                        u16 itemId) {
    ItemListManager* manager = func_800B6BA4__Fv();
    ItemListNode* sentinel = manager->sentinel;
    ItemListNode* node = sentinel->next;
    while (node != sentinel) {
        ItemListObject* object = static_cast<ItemListObject*>(node->object);
        if (itemId == object->itemId_0x3F28) {
            func_800C01D4(object, destination, itemId);
            return;
        }
        node = node->next;
    }
    __ct__8009ED08(destination, itemId);
}

extern "C" cf::UnkClass_80082D90* func_80082D90__Q22cf13CfGameManagerFv() {
    cf::CfGameManager* gameManager = cf::CfGameManager::getInstance();
    ItemListManager* itemManager = func_800B6BA4__Fv();
    UnkF8C0Node iterator;
    func_8007F8C0__Q22cf13CfGameManagerFv(
        &iterator, reinterpret_cast<const UnkF8C0Source*>(itemManager));
    UnkF8C0Node previous;
    UnkF8C0Node end;
    while ((func_8007F8F4__Q22cf13CfGameManagerFv(&end, itemManager),
            func_8007F900__Q22cf13CfGameManagerFv(
                &iterator.field_0x0, &end.field_0x0))) {
        ItemListSubobject* object =
            *func_8007F8D0__Q22cf13CfGameManagerFv(&iterator);
        if (lbl_eu_80663E40 ==
            func_8007F8B8__Q22cf13CfGameManagerFv(object)) {
            return reinterpret_cast<cf::UnkClass_80082D90*>(object);
        }
        func_8007F8DC__Q22cf13CfGameManagerFv(&previous, &iterator, 0);
    }
    return reinterpret_cast<cf::UnkClass_80082D90*>(
        *func_8007C6B4__Q22cf13CfGameManagerFv(gameManager->unk94, 0));
}

extern "C" u8 lbl_eu_8052A7E8[];
extern "C" u8 lbl_eu_8052FE68[];
extern "C" void __ct__CCharEffect(void* effect);
#pragma dont_inline on
extern "C" Unk80338Object* __ct__80080338(Unk80338Object* self) {
    Unk80338Object* result = self;
    Unk80338Object* base = result;
    u32 zero = 0;
    base->field_0x4 = zero;
    base->field_0x8 = zero;
    base->vtable_0x0 = lbl_eu_8052AC98;
    base->vtable_0xC = &lbl_eu_8052AC98[0xB4];
    base->vtable_0x10 = &lbl_eu_8052AC98[0xC4];
    base->field_0x14 = zero;
    base->field_0x18 = zero;
    base->field_0x1C = zero;
    __ct__CCharVoice(&base->voice_0x28);
    result->vtable_0x0 = lbl_eu_8052A7E8;
    result->vtable_0xC = &lbl_eu_8052A7E8[0xB4];
    result->vtable_0x10 = &lbl_eu_8052A7E8[0xC4];
    UnkCharEffect304* effect = &result->effect_0x68;
    __ct__CCharEffect(effect);
    result->field_0x36C = zero;
    effect->vtable_0x0 = lbl_eu_8052FE68;
    result->field_0x370 = zero;
    result->field_0x374 = zero;
    effect->field_0x300 = zero;
    result->field_0x378 = zero;
    result->field_0x37C = zero;
    return result;
}
#pragma dont_inline reset

extern "C" u8 lbl_eu_8052AA28[];
extern "C" u8 lbl_eu_8052FE08[];
extern "C" void __ct__CCharEffect(void* effect);
static inline Unk814BCObject* initializeUnk814Base(Unk814BCObject* base) {
    base->field_0x4 = 0;
    base->field_0x8 = 0;
    base->vtable_0x0 = lbl_eu_8052AC98;
    base->vtable_0xC = &lbl_eu_8052AC98[0xB4];
    base->vtable_0x10 = &lbl_eu_8052AC98[0xC4];
    base->field_0x14 = 0;
    base->field_0x18 = 0;
    base->field_0x1C = 0;
    __ct__CCharVoice(&base->voice_0x28);
    return base;
}
#pragma dont_inline on
extern "C" Unk814BCObject* __ct__800814BC(Unk814BCObject* self) {
    Unk814BCObject* result = self;
    self->field_0x4 = 0;
    self->field_0x8 = 0;
    self->vtable_0x0 = lbl_eu_8052AC98;
    self->vtable_0xC = &lbl_eu_8052AC98[0xB4];
    self->vtable_0x10 = &lbl_eu_8052AC98[0xC4];
    self->field_0x14 = 0;
    self->field_0x18 = 0;
    self->field_0x1C = 0;
    __ct__CCharVoice(&self->voice_0x28);
    result->vtable_0x0 = lbl_eu_8052AA28;
    result->vtable_0xC = &lbl_eu_8052AA28[0xB4];
    result->vtable_0x10 = &lbl_eu_8052AA28[0xC4];
    UnkCharEffect304* effect = &result->effect_0x68;
    __ct__CCharEffect(effect);
    effect->vtable_0x0 = lbl_eu_8052FE08;
    return result;
}
#pragma dont_inline reset

extern "C" u32 func_80061FE8();
extern "C" void* allocate__Q23mtl10MemManagerFUlUl(u32 size, u32 heap);
extern "C" Unk80EE4Data* func_80081990__Q22cf13CfGameManagerFv(
    const char* name, u16 index) {
    Unk80EE4Data* data = func_80080E44__Q22cf13CfGameManagerFv(name, index);
    u32 heap = func_80061FE8();
    void* memory = allocate__Q23mtl10MemManagerFUlUl(0x36C, heap);
    void* object = memory;
    if (memory != nullptr) {
        object = __ct__800814BC(static_cast<Unk814BCObject*>(memory));
    }
    data->vfunc_0x70(object);
    data->vfunc_0x10C(9);
    data->vfunc_0x48();
    return data;
}

extern "C" u32 func_80078C08(CfCamEventManager* manager, u32 first, u32 second,
                               u32 third, u32 fourth, u32 fifth);
extern "C" u32 func_80078D08(CfCamEventManager* manager, u32 first, u32 second,
                               u32 third, u32 fourth, u32 fifth, float value);
extern "C" u32 func_80082008__Q22cf13CfGameManagerFv(u32 first, u32 second, u32 third,
                                                       u32 fourth, u32 fifth) {
    return func_80078C08(cf::CfGameManager::getInstance()->unkB4, first, second, third,
                         fourth, fifth);
}

extern "C" u32 func_80082088__Q22cf13CfGameManagerFv(u32 first, u32 second, u32 third,
                                                       u32 fourth, float value) {
    return func_80078D08(cf::CfGameManager::getInstance()->unkB4, first, second, third,
                         fourth, 0, value);
}

extern "C" UnkClass_800821F8* func_800784A0(u32 first, cf::CfObjectMove* second,
                                             u32 third, u32 fourth, u32 fifth,
                                             u32 sixth, u32 seventh);
extern "C" void func_80075540(CfCamEventManager* manager, u32 flag);
extern "C" void func_80081DD8__Q22cf13CfGameManagerFv(bool special, u32 value);
extern "C" UnkClass_800821F8* func_800821F8__Q22cf13CfGameManagerFv();
extern "C" void func_8008212C__Q22cf13CfGameManagerFv(u32 mode) {
    if (mode >= 16 && mode <= 43) {
        goto direct;
    }
    if (mode >= 8 && mode <= 10) {
        goto enabled;
    }
    if (mode == 0) {
        goto disabled;
    }
    if (mode == 11) {
        goto enabledSingle;
    }
    goto done;

enabled:
    func_80081DD8__Q22cf13CfGameManagerFv(true, 0);
    func_800821F8__Q22cf13CfGameManagerFv()->vfunc_0x40(mode);
    goto done;

enabledSingle:
    func_80081DD8__Q22cf13CfGameManagerFv(true, 0);
    func_800821F8__Q22cf13CfGameManagerFv()->vfunc_0x40(mode);
    goto done;

direct:
    func_800821F8__Q22cf13CfGameManagerFv()->vfunc_0x40(mode);
    goto done;

disabled:
    func_80081DD8__Q22cf13CfGameManagerFv(false, 0);

done:
    return;
}

#pragma dont_inline on
extern "C" void func_80081DD8__Q22cf13CfGameManagerFv(bool special,
                                                        u32 value) {
    cf::CfGameManager* manager = cf::CfGameManager::getInstance();
    if (special) {
        manager->unkB0 = func_800784A0(
            8, *func_8007C6B4__Q22cf13CfGameManagerFv(manager->unk94, 0),
            0, 0, 0, 0, 0);
    } else {
        cf::CfObjectMove* player =
            *func_8007C6B4__Q22cf13CfGameManagerFv(manager->unk94, 0);
        manager->unkB0 = func_800784A0(0, player, value, 0, 0, 0, 0);
        func_80075540(manager->unkB4, 8);
    }
}
#pragma dont_inline reset

extern "C" void func_80081E90__Q22cf13CfGameManagerFv(u32 third, u32 fourth,
                                                       u32 seventh) {
    cf::CfGameManager* manager = cf::CfGameManager::getInstance();
    cf::CfObjectMove** slot =
        func_8007C6B4__Q22cf13CfGameManagerFv(manager->unk94, 0);
    cf::CfObjectMove* player = *slot;
    manager->unkB0 = func_800784A0(0, player, third, fourth, 0, 0, seventh);
    func_80075540(manager->unkB4, 8);
    func_80075540(manager->unkB4, 16);
}

extern "C" void func_80081F28__Q22cf13CfGameManagerFv(u32 first,
                                                       cf::CfObjectMove* second) {
    cf::CfGameManager* manager = cf::CfGameManager::getInstance();
    manager->unkB0 = func_800784A0(first, second, 0, 0, 0, 0, 0);
}

extern "C" void func_80081F90__Q22cf13CfGameManagerFv(u32 first, u32 third) {
    cf::CfGameManager* manager = cf::CfGameManager::getInstance();
    cf::CfObjectMove** slot =
        func_8007C6B4__Q22cf13CfGameManagerFv(manager->unk94, 0);
    cf::CfObjectMove* player = *slot;
    manager->unkB0 = func_800784A0(first, player, third, 0, 0, 0, 0);
}

extern "C" void loadMapBdatFileDataPointers__Q22cf6CfBdatFii(u32 first, u32 second);
extern "C" cf::CfGameManager* CfRes_stub_63ACC(ml::FixStr<64>* text);
extern "C" void func_8008228C__Q22cf13CfGameManagerFv(u32 first, u32 second) {
    ml::FixStr<64> text;
    text.format(&lbl_eu_804FB824[0x98], first, second);
    cf::CfGameManager* manager = CfRes_stub_63ACC(&text);
    manager->func_80082258();
}
extern "C" void func_801AA04C(UnkClass_8007E864* object);
extern "C" void func_8007E864__Q22cf13CfGameManagerFv(u32 first, u32 second) {
    if (!lbl_eu_80663E70) {
        __ct__Q22cf13CfGameManagerFv(&lbl_eu_80571758);
        __register_global_object(&lbl_eu_80571758, __dt__Q22cf13CfGameManagerFv,
                                 lbl_eu_80571748);
        lbl_eu_80663E70 = 1;
    }
    cf::CfGameManager* manager = &lbl_eu_80571758;
    loadMapBdatFileDataPointers__Q22cf6CfBdatFii(first, second);
    func_8008228C__Q22cf13CfGameManagerFv(first, second);
    if (manager->unkA8 != nullptr) {
        func_801AA04C(manager->unkA8);
    }
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
extern "C" s32 func_80063560(s32 value, u32 second, u32 third);
extern "C" void func_80062928(s32 destination, const void* source, u32 size);
extern "C" const void* func_801422A8__Q22cf6CfBdatFUl(u32 textId);
extern "C" void func_8006398C(u32 value);
extern "C" bool func_8007DECC__Q22cf13CfGameManagerFv(s32 value, s32* current,
                                                        u32 size) {
    bool changed = false;
    if (value > 0) {
        UnkClass_8009EC9C* data = func_8009EC9C(static_cast<u16>(value));
        s32 destination = func_80063560(value, 0, 0);
        if (destination >= 0 && destination != *current) {
            func_8009E120(func_8009EC9C(static_cast<u16>(value)), 0);
            func_80062928(destination,
                          func_8009E120(func_8009EC9C(static_cast<u16>(value)), 0),
                          size);
            func_80062928(destination,
                          func_8009E120(func_8009EC9C(static_cast<u16>(value)), 1),
                          size);
            func_80062928(destination,
                          func_8009E120(func_8009EC9C(static_cast<u16>(value)), 2),
                          size);
            func_80062928(destination,
                          func_8009E120(func_8009EC9C(static_cast<u16>(value)), 3),
                          size);
            func_80062928(destination,
                          func_8009E120(func_8009EC9C(static_cast<u16>(value)), 4),
                          size);
            func_80062928(destination,
                          func_801422A8__Q22cf6CfBdatFUl(data->textId_0x18), size);
            *current = destination;
            changed = true;
        } else if (destination >= 0 && destination == *current) {
            func_8006398C(0);
        }
    }
    return changed;
}
extern "C" u32 func_8007E038__Q22cf13CfGameManagerFv(u32 value, bool searchEntries) {
    if (searchEntries) {
        UnkClass_8009ECB0* data = func_8009ECB0();
        s32* entry = &data->entries_0x4[0];
        for (u32 i = 0; i < 7; ++i, ++entry) {
            if (*entry == static_cast<s32>(value)) {
                s32 fallback = -1;
                return func_8007DECC__Q22cf13CfGameManagerFv(
                    value, &fallback, sizeof(fallback));
            }
        }
    } else {
        s32 fallback = -1;
        return func_8007DECC__Q22cf13CfGameManagerFv(value, &fallback,
                                                     sizeof(fallback));
    }
    return 0;
}

extern "C" u32 func_800822F4__Q22cf13CfGameManagerFv();
extern "C" void func_8007E0D0__Q22cf13CfGameManagerFv(bool alternate) {
    UnkClass_8009ECB0* data = func_8009ECB0();
    s32* entry = &data->entries_0x4[0];
    s32 start = 0;
    u32 size = 4;
    if (alternate || (lbl_eu_80663E24 & 0x400000) != 0) {
        start = 3;
        size = 6;
    }
    lbl_eu_80663E28 &= ~0x8000;
    s32 fallback = -1;
    u32 count = func_800822F4__Q22cf13CfGameManagerFv();
    entry += start;
    for (s32 i = start; i < 7; ++i, ++entry) {
        s32 value = *entry;
        if (value > 0) {
            if (value <= 13) {
                if (value == 3 && count >= 42) {
                    continue;
                }
                if (value == 8 && count < 42) {
                    continue;
                }
                func_8007DECC__Q22cf13CfGameManagerFv(value, &fallback, size);
            }
        }
    }
}

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
class UnkClass_80113E1C;
extern "C" UnkClass_80113E1C* func_80113E1C();
extern "C" void func_80113E2C(UnkClass_80113E1C* object);
extern "C" void func_80164A50(ml::FixStr<64>* text, u32 second, u32 third);
extern "C" void func_800863F4__Q22cf13CfGameManagerFv(const char* name) {
    lbl_eu_80571628.textLength = std::strlen(name);
    std::strcpy(lbl_eu_80571628.text, name);
    if ((lbl_eu_80663E28 & 0x100000) == 0) {
        if (func_80113E1C() != nullptr) {
            func_80113E2C(func_80113E1C());
        }
        ml::FixStr<64> text(false);
        text.clear();
        text.format(&lbl_eu_804FB824[0xE9], name);
        func_80164A50(&text, 0, 0);
    }
}

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
extern "C" u32 func_80061870(UnkClass_80085334* object, u32 mode, u32 value,
                                u32 fourth, u32 fifth, u32 sixth);
extern "C" VoiceSource* func_800B76F4();
extern "C" VoiceSource* func_800B7854(VoiceSource* source);
extern "C" void func_800BEE1C(VoiceSource* source, bool enabled);
extern "C" void func_8007FE2C__Q22cf13CfGameManagerFv() {
    if (!lbl_eu_80663E70) {
        __ct__Q22cf13CfGameManagerFv(&lbl_eu_80571758);
        __register_global_object(&lbl_eu_80571758, __dt__Q22cf13CfGameManagerFv,
                                 lbl_eu_80571748);
        lbl_eu_80663E70 = 1;
    }
    lbl_eu_80663E28 |= 4;
    VoiceSource* source = func_800B76F4();
    while (source != nullptr) {
        if ((source->flags_0x64 & 0x80000000) != 0) {
            func_800BEE1C(source, true);
        }
        source = func_800B7854(source);
    }
}

extern "C" void func_8007FECC__Q22cf13CfGameManagerFv() {
    if (!lbl_eu_80663E70) {
        __ct__Q22cf13CfGameManagerFv(&lbl_eu_80571758);
        __register_global_object(&lbl_eu_80571758, __dt__Q22cf13CfGameManagerFv,
                                 lbl_eu_80571748);
        lbl_eu_80663E70 = 1;
    }
    lbl_eu_80663E28 &= ~4;
    VoiceSource* source = func_800B76F4();
    while (source != nullptr) {
        if ((source->flags_0x64 & 0x80000000) != 0) {
            func_800BEE1C(source, false);
        }
        source = func_800B7854(source);
    }
}

#pragma dont_inline on
extern "C" u32 func_8007FC5C__Q22cf13CfGameManagerFv(
    u32 first, u32 second, u32 third, u32 fourth, u32 fifth) {
    if (!lbl_eu_80663E70) {
        __ct__Q22cf13CfGameManagerFv(&lbl_eu_80571758);
        __register_global_object(&lbl_eu_80571758, __dt__Q22cf13CfGameManagerFv,
                                 lbl_eu_80571748);
        lbl_eu_80663E70 = 1;
    }
    u32 result;
    if (lbl_eu_80571758.unkAC == nullptr) {
        result = 0;
    } else {
        result = func_80061870(lbl_eu_80571758.unkAC, first, second, third,
                              fourth, fifth);
    }
    return result;
}
#pragma dont_inline reset

extern "C" bool func_80061D2C(UnkClass_80085334* object, u32 mode);
extern "C" bool func_80061E8C(UnkClass_80085334* object, u32 mode);
extern "C" bool func_8007FD00__Q22cf13CfGameManagerFv(u32 mode) {
    if (!lbl_eu_80663E70) {
        __ct__Q22cf13CfGameManagerFv(&lbl_eu_80571758);
        __register_global_object(&lbl_eu_80571758, __dt__Q22cf13CfGameManagerFv,
                                 lbl_eu_80571748);
        lbl_eu_80663E70 = 1;
    }
    return lbl_eu_80571758.unkAC != nullptr
               ? func_80061D2C(lbl_eu_80571758.unkAC, mode)
               : false;
}

extern "C" bool func_8007FD8C__Q22cf13CfGameManagerFv(u32 mode) {
    if (!lbl_eu_80663E70) {
        __ct__Q22cf13CfGameManagerFv(&lbl_eu_80571758);
        __register_global_object(&lbl_eu_80571758, __dt__Q22cf13CfGameManagerFv,
                                 lbl_eu_80571748);
        lbl_eu_80663E70 = 1;
    }
    return lbl_eu_80571758.unkAC != nullptr
               ? func_80061E8C(lbl_eu_80571758.unkAC, mode)
               : false;
}

extern "C" VoiceList* func_800B6BA0();
extern "C" VoiceAction* func_8016FE34(VoiceSource* source);
extern "C" void func_80084AD4__Q22cf13CfGameManagerFv(u32 mask) {
    VoiceList* list = func_800B6BA0();
    VoiceListNode* node = list->end->next;
    while (node != list->end) {
        VoiceSource* source = node->value;
        u32 flags = source->flags_0x64;
        if ((flags & 0x18E) != 0 && (mask & flags) != 0) {
            VoiceAction* action = func_8016FE34(source);
            if (action != nullptr) {
                action->vfunc_0xA8(true);
            }
        }
        node = node->next;
    }
}

extern "C" void func_8008566C__Q22cf13CfGameManagerFv(u32 mode,
                                                        const UnkFloat4* value,
                                                        u32 third);
extern "C" void func_80084A00__Q22cf13CfGameManagerFv() {
    if (!lbl_eu_80663E70) {
        __ct__Q22cf13CfGameManagerFv(&lbl_eu_80571758);
        __register_global_object(&lbl_eu_80571758, __dt__Q22cf13CfGameManagerFv,
                                 lbl_eu_80571748);
        lbl_eu_80663E70 = 1;
    }
    bool active = lbl_eu_80571758.unkAC != nullptr
                      ? func_80061D2C(lbl_eu_80571758.unkAC, 21)
                      : false;
    if (active) {
        UnkFloat4 value;
        value.field_0x0 = lbl_eu_80666498;
        value.field_0x4 = lbl_eu_80666498;
        value.field_0x8 = lbl_eu_80666498;
        value.field_0xC = lbl_eu_80666498;
        func_8008566C__Q22cf13CfGameManagerFv(15, &value, 0);
    }
    lbl_eu_80663E24 &= ~0x200;
    lbl_eu_80663E28 &= ~0x800;
    lbl_eu_80663DF8 |= 0xFFFFFFFF;
}

extern "C" void func_80085248__Q22cf13CfGameManagerFv() {
    if (!lbl_eu_80663E70) {
        __ct__Q22cf13CfGameManagerFv(&lbl_eu_80571758);
        __register_global_object(&lbl_eu_80571758, __dt__Q22cf13CfGameManagerFv,
                                 lbl_eu_80571748);
        lbl_eu_80663E70 = 1;
    }
    if (lbl_eu_80571758.unkAC != nullptr) {
        func_80061870(lbl_eu_80571758.unkAC, 12, 0, 0, 0, 0);
    }
    if (!lbl_eu_80663E70) {
        __ct__Q22cf13CfGameManagerFv(&lbl_eu_80571758);
        __register_global_object(&lbl_eu_80571758, __dt__Q22cf13CfGameManagerFv,
                                 lbl_eu_80571748);
        lbl_eu_80663E70 = 1;
    }
    if (lbl_eu_80571758.unkAC != nullptr) {
        func_80061870(lbl_eu_80571758.unkAC, 4, 0, 0, 0, 0);
    }
}

extern "C" void func_80085334__Q22cf13CfGameManagerFv(u32 value) {
    if (!lbl_eu_80663E70) {
        __ct__Q22cf13CfGameManagerFv(&lbl_eu_80571758);
        __register_global_object(&lbl_eu_80571758, __dt__Q22cf13CfGameManagerFv,
                                 lbl_eu_80571748);
        lbl_eu_80663E70 = 1;
    }
    cf::CfGameManager* manager = &lbl_eu_80571758;
    if (manager->unkAC != nullptr) {
        func_80061870(manager->unkAC, 6, static_cast<u16>(value), 0, 0, 0);
    }
}

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

extern "C" u32 lbl_eu_80663E30;
extern "C" bool CfRes_getE24Bit18();
extern "C" bool CfRes_checkFlags_2000400();
extern "C" bool func_80083538__Q22cf13CfGameManagerFv();
extern "C" bool func_80083544__Q22cf13CfGameManagerFv();
extern "C" void func_80083550__Q22cf13CfGameManagerFv();
extern "C" void func_80084CA4__Q22cf13CfGameManagerFv(u32 first, u32 second,
                                                        u32 third, bool enable);
extern "C" u32 CfRes_packThreeFields(u32 first, u32 second, u32 third);
extern "C" u32 CfRes_callFunc_67E78(u32 value);
extern "C" void func_800620F0(u32 value);
extern "C" void func_800835FC__Q22cf13CfGameManagerFv();
extern "C" void func_80083470__Q22cf13CfGameManagerFv(u32 first, u32 second,
                                                        bool special) {
    cf::CfGameManager::getInstance();
    if (CfRes_getE24Bit18() || CfRes_checkFlags_2000400() ||
        func_80083538__Q22cf13CfGameManagerFv()) {
        return;
    }
    if (special && lbl_eu_80663E42 == 24 && lbl_eu_80663E44 == 1) {
        return;
    }
    bool active = func_80083544__Q22cf13CfGameManagerFv();
    if (!active || special) {
        CfRes_callFunc_67E78(active);
        func_80084CA4__Q22cf13CfGameManagerFv(first, second, 1, false);
        func_8007F990__Q22cf13CfGameManagerFv(0x10000, special);
    }
    func_80083550__Q22cf13CfGameManagerFv();
}

extern "C" void func_80083560__Q22cf13CfGameManagerFv(u32 first, u32 second,
                                                        u32 third) {
    cf::CfGameManager::getInstance();
    if (!func_80083538__Q22cf13CfGameManagerFv()) {
        func_80084CA4__Q22cf13CfGameManagerFv(first, second, third, true);
    }
    u32 packed = CfRes_packThreeFields(first, second, third);
    if (lbl_eu_80663E30 != 0 && lbl_eu_80663E30 != packed) {
        func_800620F0(CfRes_callFunc_67E78(packed));
    }
    lbl_eu_80663E30 = packed;
    func_800835FC__Q22cf13CfGameManagerFv();
}

struct CfPlayerExtraData {
    u8 field_0x0[0xB14];
};

class CfPlayerComposite : public cf::CActorParam,
                          public CfPlayerExtraData,
                          public cf::CfObjectMove {};

extern "C" u32 func_80082EC0__Q22cf13CfGameManagerFv(
    cf::UnkClass_CActorParam15E0* data, u32 value);
extern "C" u32 func_8025FB10(cf::UnkClass_CActorParam15E0* data, u32 value);
extern "C" u32 func_80082E50__Q22cf13CfGameManagerFv(s32 playerIndex, u32 value) {
    cf::CfObjectMove* player = cf::CfGameManager::getPlayer(playerIndex);
    CfPlayerComposite* composite = static_cast<CfPlayerComposite*>(player);
    if (composite != nullptr) {
        cf::UnkClass_CActorParam15E0* data =
            composite->CActorParam_UnkVirtualFunc127();
        if (data != nullptr) {
            return func_80082EC0__Q22cf13CfGameManagerFv(data, value);
        }
    }
    return 0;
}

extern "C" u32 func_80082EC4__Q22cf13CfGameManagerFv(s32 playerIndex, u32 value) {
    cf::CfObjectMove* player = cf::CfGameManager::getPlayer(playerIndex);
    CfPlayerComposite* composite = static_cast<CfPlayerComposite*>(player);
    if (composite != nullptr) {
        cf::UnkClass_CActorParam15E0* data =
            composite->CActorParam_UnkVirtualFunc127();
        if (data != nullptr) {
            return func_8025FB10(data, value);
        }
    }
    return 0;
}

extern "C" bool func_8006C670(cf::CfObjectMove* player);
extern "C" bool func_80082F2C__Q22cf13CfGameManagerFv(s32 playerIndex,
                                                        bool requireFlag) {
    bool result = false;
    cf::CfObjectMove* player = cf::CfGameManager::getPlayer(playerIndex);
    if (player != nullptr && player->CfObject_UnkVirtualFunc9()) {
        if (requireFlag &&
            !func_80082FCC__Q22cf13CfGameManagerFv(player, 0x100000)) {
            return false;
        }
        if (func_8006C670(player)) {
            result = true;
        }
    }
    return result;
}

extern "C" void func_80082A7C__Q22cf13CfGameManagerFv(cf::CfObjectMove* object) {
    cf::CfGameManager* manager = cf::CfGameManager::getInstance();
    cf::CfObjectMove* oldFirst =
        *func_8007C6B4__Q22cf13CfGameManagerFv(manager->unk94, 0);
    if (*func_8007C6B4__Q22cf13CfGameManagerFv(manager->unk94, 0) != object) {
        *func_8007C6B4__Q22cf13CfGameManagerFv(manager->unk94, 0) = object;
        for (s32 i = 1; i < 3; ++i) {
            if (object ==
                *func_8007C6B4__Q22cf13CfGameManagerFv(manager->unk94, i)) {
                *func_8007C6B4__Q22cf13CfGameManagerFv(manager->unk94, i) = oldFirst;
                break;
            }
        }
    }
}

extern "C" CfCamEventManager* func_8006E59C();
extern "C" cf::UnkClass_80082D90* func_80082D90__Q22cf13CfGameManagerFv();
extern "C" void func_80082A0C__Q22cf13CfGameManagerFv() {
    CfCamEventManager* cameraManager = func_8006E59C();
    if (cameraManager != nullptr) {
        cf::UnkClass_80082D90* data = func_80082D90__Q22cf13CfGameManagerFv();
        if (data != nullptr) {
            cf::CfObject* object = func_8006E5A4(cameraManager);
            if (object != nullptr) {
                object->CfObject_UnkVirtualFunc3(data);
            }
        }
    }
}

class UnkClass_800B07E8;
extern "C" void func_80068D14();
extern "C" UnkClass_800B07E8* func_800B07E8__Fv();
extern "C" void func_800B4278(UnkClass_800B07E8* object, u32 mask);
extern "C" u32 func_80084C10__Q22cf13CfGameManagerFv() {
    if (!lbl_eu_80663E70) {
        __ct__Q22cf13CfGameManagerFv(&lbl_eu_80571758);
        __register_global_object(&lbl_eu_80571758, __dt__Q22cf13CfGameManagerFv,
                                 lbl_eu_80571748);
        lbl_eu_80663E70 = 1;
    }
    cf::CfGameManager* manager = &lbl_eu_80571758;
    if (lbl_eu_80663E24 & 0x40) {
        func_80068D14();
        func_800B4278(func_800B07E8__Fv(), 0x10000000);
        u32 flags = lbl_eu_80663E24;
        manager->field_0x86 = 0;
        lbl_eu_80663E24 = flags & ~0x40;
        return flags;
    }
    return lbl_eu_80663E24;
}

extern "C" bool func_8010CE48();
extern "C" bool func_8012E6DC();
extern "C" void func_8012FAA8();
extern "C" void func_80087280__Q22cf13CfGameManagerFv(cf::CfGameManager* manager) {
    if (!func_8010CE48() && !func_8012E6DC() &&
        (lbl_eu_80663E28 & 0x01000000) == 0) {
        u32 flags = lbl_eu_80663E24;
        if ((flags & 0x31880) == 0x31880 && (flags & 0xAFA40704) == 0) {
            ++manager->unk8C;
            if (manager->unk8C >= 60) {
                func_8012FAA8();
                manager->unk8C = 0;
            }
        } else {
            manager->unk8C = 0;
        }
    } else {
        manager->unk8C = 0;
    }
}

extern "C" bool func_80164C28();
extern "C" void* func_80496034(CScn* scene);
extern "C" void* func_80496264(CScn* scene, s32 index);
extern "C" void func_80075674(CfCamEventManager* manager, void* object);
extern "C" bool func_800865E8__Q22cf13CfGameManagerFv() {
    bool result = func_80164C28();
    if (!lbl_eu_80663E70) {
        __ct__Q22cf13CfGameManagerFv(&lbl_eu_80571758);
        __register_global_object(&lbl_eu_80571758, __dt__Q22cf13CfGameManagerFv,
                                 lbl_eu_80571748);
        lbl_eu_80663E70 = 1;
    }
    cf::CfGameManager* manager = &lbl_eu_80571758;
    CScn* scene = lbl_eu_80663E14;
    if (result && manager->unkB4 != nullptr) {
        void* object = func_80496264(scene, -1);
        func_80075674(manager->unkB4, object);
    }
    if (func_80496034(lbl_eu_80663E14) == nullptr) {
        result = true;
    }
    return result;
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
