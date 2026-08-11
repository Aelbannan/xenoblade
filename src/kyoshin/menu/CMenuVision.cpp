#include "kyoshin/menu/CMenuVision.hpp"

#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/device/CDeviceFont.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/lib/CLibStaticData.hpp"

// Globals shared across menu units
extern "C" {
extern u32 lbl_eu_80663E24;
extern u32 lbl_eu_80663E28;
extern const f32 lbl_eu_80667DC0; // 0.0f
extern const f32 lbl_eu_80667DC4; // 1.0f
extern const f32 lbl_eu_80667DC8; // scale factor
extern char lbl_eu_80504268[];    // string table base
extern u32 lbl_eu_805041C0[];     // 6 pane-name pointers
extern CMenuVision* lbl_eu_80664388;
}

namespace cf {
class CBattleManager {
public:
    static CBattleManager* getInstance();
    void* func_800EA444();
};
}

extern u32 func_801355A0();

extern void func_801AFAD0(CMenuVision*, CMenuVisionEntry*);
extern f32 func_800F4424(void*);

extern "C" {
void* func_800B708C__Fi(int);
void* func_800EA444(cf::CBattleManager*);
u32 func_800F4784(void*);
void* func_800F477C(void*);
int func_800F4648(void*);
f32 func_800F42AC(void*);
void func_80138078__FUl(u32);
void func_80139B5C(void*, const char*, u32);
void func_80139BF4(void*, const char*, u32, u32);
void func_80137F88(void*, void*);
u16 func_80136254(void*, const char*, u32);
char* func_80138F78(u32);

extern u32 lbl_eu_80662580;
extern u32 lbl_eu_80662584;
extern u32 lbl_eu_80662588;
extern u32 lbl_eu_8066258C;
extern u32 lbl_eu_80662590;
extern u32 lbl_eu_80662594;
extern u32 lbl_eu_80662598;
extern u32 lbl_eu_8066259C;
extern u32 lbl_eu_806625A0;
extern u32 lbl_eu_806625A4;
extern u32 lbl_eu_806625A8;
extern u32 lbl_eu_806625AC;
extern u32 lbl_eu_806625B0;
extern u32 lbl_eu_806625B4;
extern f32 lbl_eu_80667DD4;
extern u32 lbl_eu_80667DD0;
extern void* lbl_eu_806640E0;
extern const char* lbl_eu_80504238[4];
extern const char* lbl_eu_80504248[4];
}

struct VisionQuad {
    s16 values[4];
};

extern "C" {
extern VisionQuad lbl_eu_80664338;
extern VisionQuad lbl_eu_80664340;
extern VisionQuad lbl_eu_80664348;
extern VisionQuad lbl_eu_80664350;
extern VisionQuad lbl_eu_80664358;
extern VisionQuad lbl_eu_80664360;
extern VisionQuad lbl_eu_80664368;
extern VisionQuad lbl_eu_80664370;
extern VisionQuad lbl_eu_80664378;
extern VisionQuad lbl_eu_80664380;
}

// Pad accessor for pane float fields at known retail offsets
struct PaneTransAccess {
    u8 _pad[0x4C];
    f32 transX;
    f32 transY;
};

struct PaneScaleAccess {
    u8 _pad[0x44];
    f32 scaleX;
    f32 scaleY;
};

struct MenuVisionBattleData {
    int actor;
    int targetActor;
    u8 _pad08[0x84 - 0x08];
    u32 flags84;
    u32 flags88;
    u8 _pad8C[0x824 - 0x8C];
    u32 flags824;
    u32 value828;
    u32 value82C;
};

struct MenuVisionActorInfo {
    u8 _pad00[0x3C];
    u16 mode3C;
    u8 _pad3E[0x42 - 0x3E];
    u8 mode42;
};

#pragma push
#pragma inline_max_size(10000)

static inline void* menuVisionResource(const char* name) {
    return func_801355F4()->GetResource(0x74696D67, name, 0);
}

static inline u32 menuVisionActorKind(void* actor) {
    return ((u32 (*)(void*))(*(void***)actor)[0xE0 / 4])(actor);
}

static inline MenuVisionActorInfo* menuVisionActorInfo(void* actor) {
    void* value = ((void* (*)(void*))(*(void***)actor)[0x298 / 4])(actor);
    return *(MenuVisionActorInfo**)((u8*)value + 0x50);
}

static inline void menuVisionSetVisible(nw4r::lyt::Pane* pane, bool visible) {
    if (pane != 0) {
        PaneVisAccess* access = reinterpret_cast<PaneVisAccess*>(pane);
        access->visByte &= ~1;
        access->visByte |= visible;
    }
}

static inline void menuVisionSetImage(nw4r::lyt::Layout* layout, const char* paneName, void* image) {
    nw4r::lyt::Pane* pane = layout->GetRootPane()->FindPaneByName(paneName, true);
    menuVisionSetVisible(pane, false);
    if (image != 0) {
        func_80137E7C(layout, (void*)paneName, image);
        menuVisionSetVisible(pane, true);
    }
}

static inline void menuVisionBegin(CMenuVisionEntry& entry) {
    entry.mLayout->SetAnimationEnable(entry.mAnim2, false);
    entry.mLayout->SetAnimationEnable(entry.mAnim3, false);
    entry.mLayout->SetAnimationEnable(entry.mAnim4, false);
    entry.mLayout->SetAnimationEnable(entry.mAnim5, false);
    entry.mLayout->SetAnimationEnable(entry.mAnim6, false);
    entry.mLayout->SetAnimationEnable(entry.mAnim7, false);
    entry.mLayout->SetAnimationEnable(entry.mAnim8, false);
    entry.mLayout->SetAnimationEnable(entry.mAnim1, true);
    entry.mAnim1->SetFrame(lbl_eu_80667DC0);
    entry.field_0x28 = 0;
    entry.mState = 1;
    func_80138078__FUl(0x1C3);
}

static inline void menuVisionBeginWithoutSecond(CMenuVisionEntry& entry) {
    entry.mLayout->SetAnimationEnable(entry.mAnim3, false);
    entry.mLayout->SetAnimationEnable(entry.mAnim4, false);
    entry.mLayout->SetAnimationEnable(entry.mAnim5, false);
    entry.mLayout->SetAnimationEnable(entry.mAnim6, false);
    entry.mLayout->SetAnimationEnable(entry.mAnim7, false);
    entry.mLayout->SetAnimationEnable(entry.mAnim8, false);
    entry.mLayout->SetAnimationEnable(entry.mAnim1, true);
    entry.mAnim1->SetFrame(lbl_eu_80667DC0);
    entry.field_0x28 = 0;
    entry.mState = 1;
    func_80138078__FUl(0x1C3);
}

template <typename T>
static inline void* menuVisionEnemyImage(void* actor, T* battle, int specialOffset) {
    void* image = 0;
    actor = actor != 0 ? (u8*)actor - 0x3E9C : 0;
    if (battle->flags824 & 0x20000) {
        image = menuVisionResource(lbl_eu_80504268 + specialOffset);
        return image;
    }

    switch (menuVisionActorKind(actor)) {
    case 1:
        if (*(u16*)((u8*)actor + 0x3F28) == 0x1A1) {
            image = menuVisionResource(lbl_eu_80504268 + 0x247);
        } else {
            image = menuVisionResource(lbl_eu_80504268 + 0x25C);
        }
        break;
    case 2:
        switch (*(u16*)((u8*)actor + 0x3F28)) {
        case 0x3D: case 0x3E: case 0x4B5:
        case 0x5FF: case 0x600: case 0x601: case 0x961:
            image = menuVisionResource(lbl_eu_80504268 + 0x2B0); break;
        case 0x10C: case 0x1A0: case 0x1A1: case 0x602:
        case 0x835: case 0x8A8: case 0x8AC: case 0x962: case 0x963:
            image = menuVisionResource(lbl_eu_80504268 + 0x247); break;
        case 0x77A: case 0x843: case 0x844: case 0x964:
            image = menuVisionResource(lbl_eu_80504268 + 0x2C5); break;
        case 0x656: case 0x84B: case 0x9C5: case 0x9C6: case 0x9C7:
            image = menuVisionResource(lbl_eu_80504268 + 0x2DA); break;
        case 0x654: case 0x655:
            image = menuVisionResource(lbl_eu_80504268 + 0x2EF); break;
        case 0x59C:
            image = menuVisionResource(lbl_eu_80504268 + 0x304); break;
        case 0x845:
            image = menuVisionResource(lbl_eu_80504268 + 0x319); break;
        }
        break;
    case 3: case 4: case 5: case 6: case 8: case 12:
        image = menuVisionResource(lbl_eu_80504268 + 0x232); break;
    case 7:
        image = menuVisionResource(lbl_eu_80504268 + 0x286); break;
    case 9:
        image = menuVisionResource(lbl_eu_80504268 + 0x271); break;
    case 10:
        image = menuVisionResource(lbl_eu_80504268 + 0x29B); break;
    case 11:
        switch (*(u16*)((u8*)actor + 0x3F28)) {
        case 0x965: image = menuVisionResource(lbl_eu_80504268 + 0x271); break;
        case 0x961: image = menuVisionResource(lbl_eu_80504268 + 0x2B0); break;
        case 0x962: case 0x963: image = menuVisionResource(lbl_eu_80504268 + 0x247); break;
        case 0x964: image = menuVisionResource(lbl_eu_80504268 + 0x2C5); break;
        case 0x524: case 0x525: image = menuVisionResource(lbl_eu_80504268 + 0x32E); break;
        case 0x905: image = menuVisionResource(lbl_eu_80504268 + 0x343); break;
        case 0x9C9: image = menuVisionResource(lbl_eu_80504268 + 0x358); break;
        case 0x969: case 0x96A: case 0x96C: image = menuVisionResource(lbl_eu_80504268 + 0x36D); break;
        case 0x96B: image = menuVisionResource(lbl_eu_80504268 + 0x382); break;
        case 0x967: case 0x968: image = menuVisionResource(lbl_eu_80504268 + 0x397); break;
        default: image = menuVisionResource(lbl_eu_80504268 + 0x3AC); break;
        }
        break;
    }
    return image;
}

template <typename T>
static inline void menuVisionTargetImage(void* actor, T* battle, void*& image) {
    u32 flags = *(u32*)((u8*)actor + 0x64);
    if (flags & 4) {
        image = menuVisionEnemyImage(actor, battle, 0x439);
        return;
    }
    if (flags & 2) {
        return;
    }
    actor = actor != 0 ? (u8*)actor - 0x3E9C : 0;
    switch (*(u16*)((u8*)actor + 0x3F28)) {
    case 1: image = menuVisionResource(lbl_eu_80504268 + 0x44E); break;
    case 2: image = menuVisionResource(lbl_eu_80504268 + 0x462); break;
    case 3: image = menuVisionResource(lbl_eu_80504268 + 0x476); break;
    case 4: image = menuVisionResource(lbl_eu_80504268 + 0x48A); break;
    case 5: image = menuVisionResource(lbl_eu_80504268 + 0x49E); break;
    case 6: image = menuVisionResource(lbl_eu_80504268 + 0x4B2); break;
    case 7: image = menuVisionResource(lbl_eu_80504268 + 0x4C6); break;
    case 8: image = menuVisionResource(lbl_eu_80504268 + 0x4DA); break;
    case 9: image = menuVisionResource(lbl_eu_80504268 + 0x4EE); break;
    case 10: image = menuVisionResource(lbl_eu_80504268 + 0x502); break;
    case 11: image = menuVisionResource(lbl_eu_80504268 + 0x516); break;
    }
}

static inline void menuVisionReplacePaneImage(nw4r::lyt::Pane* pane, void* image) {
    if (image != 0) {
        func_80137F88(pane, image);
        menuVisionSetVisible(pane, true);
    }
}

// --- Stubs for non-target functions ---
void __ct__CMenuVision(){}

CMenuVision::~CMenuVision() {}

extern "C" unsigned long func_801AC088() {
    return lbl_eu_80664388 != 0;
}

void func_801AC09C(){}

void func_801AC124(){}

void func_801AC1F8(){}

void CMenuVision::Term() {}

void CMenuVision::cbRenderBefore() {}

void func_801ACCE0(){}

void func_801ACD5C(){}

extern "C" void func_801AD504(int flags) {
    if (lbl_eu_80664388 == 0) {
        return;
    }

    MenuVisionBattleData* battle = static_cast<MenuVisionBattleData*>(func_800EA444(cf::CBattleManager::getInstance()));
    if (battle == 0) {
        return;
    }

    if (flags & 1) {
        CMenuVision* menu = lbl_eu_80664388;
        CMenuVisionEntry& entry = menu->mEntries[0];
        if (entry.mState != 0) {
            return;
        }
        void* actorBase;
        void* image = 0;
        void* actor = func_800B708C__Fi(battle->actor);
        if (actor == 0) {
            return;
        }
        if (*(u32*)((u8*)actor + 0x64) & 4) {
            void* actorBase = actor;
            if (actor != 0) {
                actorBase = (u8*)actor - 0x3E9C;
            }
            if (battle->flags824 & 0x20000) {
                image = menuVisionResource(lbl_eu_80504268 + 0x21D);
            } else {
                switch (menuVisionActorKind(actorBase)) {
                case 1:
                    if (*(u16*)((u8*)actorBase + 0x3F28) == 0x1A1) {
                        image = menuVisionResource(lbl_eu_80504268 + 0x247);
                    } else {
                        image = menuVisionResource(lbl_eu_80504268 + 0x25C);
                    }
                    break;
                case 2:
                    switch (*(u16*)((u8*)actorBase + 0x3F28)) {
                    case 0x3D: case 0x3E: case 0x4B5:
                    case 0x5FF: case 0x600: case 0x601: case 0x961:
                        image = menuVisionResource(lbl_eu_80504268 + 0x2B0); break;
                    case 0x10C: case 0x1A0: case 0x1A1: case 0x602:
                    case 0x835: case 0x8A8: case 0x8AC: case 0x962: case 0x963:
                        image = menuVisionResource(lbl_eu_80504268 + 0x247); break;
                    case 0x77A: case 0x843: case 0x844: case 0x964:
                        image = menuVisionResource(lbl_eu_80504268 + 0x2C5); break;
                    case 0x656: case 0x84B: case 0x9C5: case 0x9C6: case 0x9C7:
                        image = menuVisionResource(lbl_eu_80504268 + 0x2DA); break;
                    case 0x654: case 0x655:
                        image = menuVisionResource(lbl_eu_80504268 + 0x2EF); break;
                    case 0x59C:
                        image = menuVisionResource(lbl_eu_80504268 + 0x304); break;
                    case 0x845:
                        image = menuVisionResource(lbl_eu_80504268 + 0x319); break;
                    }
                    break;
                case 3: case 4: case 5: case 6: case 8: case 12:
                    image = menuVisionResource(lbl_eu_80504268 + 0x232); break;
                case 7:
                    image = menuVisionResource(lbl_eu_80504268 + 0x286); break;
                case 9:
                    image = menuVisionResource(lbl_eu_80504268 + 0x271); break;
                case 10:
                    image = menuVisionResource(lbl_eu_80504268 + 0x29B); break;
                case 11:
                    switch (*(u16*)((u8*)actorBase + 0x3F28)) {
                    case 0x965: image = menuVisionResource(lbl_eu_80504268 + 0x271); break;
                    case 0x961: image = menuVisionResource(lbl_eu_80504268 + 0x2B0); break;
                    case 0x962: case 0x963: image = menuVisionResource(lbl_eu_80504268 + 0x247); break;
                    case 0x964: image = menuVisionResource(lbl_eu_80504268 + 0x2C5); break;
                    case 0x524: case 0x525: image = menuVisionResource(lbl_eu_80504268 + 0x32E); break;
                    case 0x905: image = menuVisionResource(lbl_eu_80504268 + 0x343); break;
                    case 0x9C9: image = menuVisionResource(lbl_eu_80504268 + 0x358); break;
                    case 0x969: case 0x96A: case 0x96C: image = menuVisionResource(lbl_eu_80504268 + 0x36D); break;
                    case 0x96B: image = menuVisionResource(lbl_eu_80504268 + 0x382); break;
                    case 0x967: case 0x968: image = menuVisionResource(lbl_eu_80504268 + 0x397); break;
                    default: image = menuVisionResource(lbl_eu_80504268 + 0x3AC); break;
                    }
                    break;
                }
            }
        }
        menuVisionSetImage(entry.mLayout, lbl_eu_80504268 + 0x3C1, image);
        menuVisionBegin(entry);
    }

    if (flags & 2) {
        CMenuVision* menu = lbl_eu_80664388;
        CMenuVisionEntry& entry = menu->mEntries[1];
        u32 font = func_800F4784(battle);
        func_80136B4C(entry.mLayout, lbl_eu_80504268 + 0x1AB, (char*)font, 0);
        func_80136B4C(entry.mLayout, lbl_eu_80504268 + 0x1B8, (char*)font, 0);
        func_80136B4C(entry.mLayout, lbl_eu_80504268 + 0x1C5, (char*)font, 0);
        func_80136B4C(entry.mLayout, lbl_eu_80504268 + 0x1D2, (char*)font, 0);
        func_80136B4C(entry.mLayout, lbl_eu_80504268 + 0x1DF, (char*)font, 0);
        func_80136B4C(entry.mLayout, lbl_eu_80504268 + 0x1ED, (char*)font, 0);

        u32 color1 = lbl_eu_80662580;
        u32 color2 = lbl_eu_80662584;
        u32 color3 = lbl_eu_80662588;
        void* actor = func_800B708C__Fi(battle->actor);
        actor = actor != 0 ? (u8*)actor - 0x3E9C : 0;
        if (actor == 0) {
            return;
        }
        MenuVisionActorInfo* info = menuVisionActorInfo(actor);
        if (info == 0) {
            return;
        }
        if (info->mode42 == 1) {
            color1 = lbl_eu_80662594;
            color2 = lbl_eu_806625A4;
            color3 = lbl_eu_806625B4;
        } else if (info->mode42 == 10) {
            color1 = lbl_eu_8066258C;
            color2 = lbl_eu_80662598;
            color3 = lbl_eu_806625A8;
        } else if (info->mode3C == 1) {
            color1 = lbl_eu_80662590;
            color2 = lbl_eu_8066259C;
            color3 = lbl_eu_806625AC;
        } else if (info->mode3C == 2 || info->mode3C == 3) {
            color1 = lbl_eu_8066258C;
            color2 = lbl_eu_80662598;
            color3 = lbl_eu_806625A8;
        }
        func_80139B5C(entry.mLayout, lbl_eu_80504268 + 0x1AB, color1);
        func_80139B5C(entry.mLayout, lbl_eu_80504268 + 0x1B8, color1);
        func_80139B5C(entry.mLayout, lbl_eu_80504268 + 0x1C5, color1);
        func_80139B5C(entry.mLayout, lbl_eu_80504268 + 0x1D2, color1);
        func_80139BF4(entry.mLayout, lbl_eu_80504268 + 0x1ED, color2, color3);

        u16 range = *(u16*)((u8*)func_800F477C(battle) + 0x5E);
        int imageOffset;
        switch (range) {
        case 4:
        case 6:
            imageOffset = 0x3D0;
            break;
        case 1:
            imageOffset = 0x3E5;
            break;
        case 5:
            imageOffset = 0x3FA;
            break;
        default:
            imageOffset = 0x40F;
            break;
        }
        menuVisionSetImage(entry.mLayout, lbl_eu_80504268 + 0x422,
                           menuVisionResource(lbl_eu_80504268 + imageOffset));
        menuVisionBeginWithoutSecond(entry);
    }

    if (flags & 4) {
        CMenuVision* menu = lbl_eu_80664388;
        CMenuVisionEntry& entry = menu->mEntries[2];
        menuVisionSetVisible(entry.mLayout->GetRootPane()->FindPaneByName(lbl_eu_80504268 + 0x42A, true), true);
        void* image = 0;
        void* actor = func_800B708C__Fi(battle->targetActor);
        if (actor == 0) {
            return;
        }
        menuVisionTargetImage(actor, battle, image);
        menuVisionSetImage(entry.mLayout, lbl_eu_80504268 + 0x42A, image);
        menuVisionBegin(entry);
    }

    if (flags & 8) {
        CMenuVision* menu = lbl_eu_80664388;
        CMenuVisionEntry& entry = menu->mEntries[3];
        nw4r::lyt::Pane* status = entry.mLayout->GetRootPane()->FindPaneByName(lbl_eu_80504268 + 0x52A, true);
        nw4r::lyt::Pane* statusAlt = entry.mLayout->GetRootPane()->FindPaneByName(lbl_eu_80504268 + 0x537, true);
        nw4r::lyt::Pane* panic = entry.mLayout->GetRootPane()->FindPaneByName(lbl_eu_80504268 + 0x546, true);
        nw4r::lyt::Pane* damage = entry.mLayout->GetRootPane()->FindPaneByName(lbl_eu_80504268 + 0x1FB, true);
        menuVisionSetVisible(damage, false);
        menuVisionSetVisible(entry.mLayout->GetRootPane()->FindPaneByName(lbl_eu_80504268 + 0x550, true), false);
        menuVisionSetVisible(entry.mLayout->GetRootPane()->FindPaneByName(lbl_eu_80504268 + 0x55D, true), false);
        menuVisionSetVisible(status, false);
        menuVisionSetVisible(statusAlt, false);
        menuVisionSetVisible(panic, false);

        if (battle->flags824 & 0x20000) {
            menuVisionSetVisible(entry.mLayout->GetRootPane()->FindPaneByName(lbl_eu_80504268 + 0x550, true), true);
        } else {
            menuVisionSetVisible(damage, true);
            if ((battle->flags84 & 1) || (battle->flags88 & 0x100) || func_800F4648(battle) > 0) {
                func_80136910__FPQ34nw4r3lyt6LayoutPcUc(entry.mLayout, lbl_eu_80504268 + 0x1FB,
                                                       (u8)func_800F4648(battle));
                nw4r::math::VEC2 scale;
                scale.x = lbl_eu_80667DD4;
                scale.y = lbl_eu_80667DD4;
                PaneScaleAccess* pane = reinterpret_cast<PaneScaleAccess*>(
                    entry.mLayout->GetRootPane()->FindPaneByName(lbl_eu_80504268 + 0x1FB, true));
                pane->scaleX = scale.x;
                pane->scaleY = scale.y;
            } else {
                char* text = func_80136190(lbl_eu_80504268 + 0x56B, lbl_eu_80504268 + 0x576, 11);
                func_80136B4C(entry.mLayout, lbl_eu_80504268 + 0x1FB, text, 0);
                nw4r::math::VEC2 scale;
                scale.x = lbl_eu_80667DC4;
                scale.y = lbl_eu_80667DC4;
                PaneScaleAccess* pane = reinterpret_cast<PaneScaleAccess*>(
                    entry.mLayout->GetRootPane()->FindPaneByName(lbl_eu_80504268 + 0x1FB, true));
                pane->scaleX = scale.x;
                pane->scaleY = scale.y;
            }

            VisionQuad first = lbl_eu_80664338;
            VisionQuad second = lbl_eu_80664340;
            u32 color1 = lbl_eu_80662584;
            u32 color2 = lbl_eu_80662588;
            void* actor = func_800B708C__Fi(battle->actor);
            actor = actor != 0 ? (u8*)actor - 0x3E9C : 0;
            if (actor == 0) {
                return;
            }
            MenuVisionActorInfo* info = menuVisionActorInfo(actor);
            if (info == 0) {
                return;
            }
            if (info->mode42 == 1) {
                first = lbl_eu_80664378;
                second = lbl_eu_80664380;
                color1 = lbl_eu_806625A4;
                color2 = lbl_eu_806625B4;
            } else if (info->mode42 == 10) {
                first = lbl_eu_80664368;
                second = lbl_eu_80664370;
                color1 = lbl_eu_806625A0;
                color2 = lbl_eu_806625B0;
            } else if (info->mode3C == 1) {
                first = lbl_eu_80664358;
                second = lbl_eu_80664360;
                color1 = lbl_eu_8066259C;
                color2 = lbl_eu_806625AC;
            } else if (info->mode3C == 2) {
                first = lbl_eu_80664348;
                second = lbl_eu_80664350;
                color1 = lbl_eu_80662598;
                color2 = lbl_eu_806625A8;
            }
            func_80139BF4(entry.mLayout, lbl_eu_80504268 + 0x1FB, color1, color2);
            func_80139A18(entry.mLayout, lbl_eu_80504268 + 0x1FB, &first, &second);

            u32 current = battle->value828;
            u32 previous = battle->value82C;
            if (current == previous) {
                previous = 0;
            }
            u8 ids[4];
            *(u32*)ids = lbl_eu_80667DD0;
            bool known = previous == ids[0] || previous == ids[1] || previous == ids[2] || previous == ids[3];
            if (known) {
                if (previous != 0) {
                    const char* images[4] = {lbl_eu_80504238[0], lbl_eu_80504238[1],
                                             lbl_eu_80504238[2], lbl_eu_80504238[3]};
                    for (u8 i = 0; i < 4; i++) {
                        if (previous == ids[i]) {
                            menuVisionReplacePaneImage(panic, menuVisionResource(images[i]));
                            break;
                        }
                    }
                    if (!(reinterpret_cast<PaneVisAccess*>(panic)->visByte & 1)) {
                        u16 id = func_80136254(lbl_eu_806640E0, lbl_eu_80504268 + 0x57B, previous);
                        if (id != 0) {
                            menuVisionReplacePaneImage(status, menuVisionResource(func_80138F78(id)));
                        }
                    }
                }
                if (current != 0) {
                    u16 id = func_80136254(lbl_eu_806640E0, lbl_eu_80504268 + 0x57B, current);
                    if (id != 0) {
                        menuVisionReplacePaneImage(statusAlt, menuVisionResource(func_80138F78(id)));
                    }
                }
            } else {
                if (current != 0) {
                    const char* images[4] = {lbl_eu_80504248[0], lbl_eu_80504248[1],
                                             lbl_eu_80504248[2], lbl_eu_80504248[3]};
                    for (u8 i = 0; i < 4; i++) {
                        if (current == ids[i]) {
                            menuVisionReplacePaneImage(panic, menuVisionResource(images[i]));
                            break;
                        }
                    }
                    if (!(reinterpret_cast<PaneVisAccess*>(panic)->visByte & 1)) {
                        u16 id = func_80136254(lbl_eu_806640E0, lbl_eu_80504268 + 0x57B, current);
                        if (id != 0) {
                            menuVisionReplacePaneImage(status, menuVisionResource(func_80138F78(id)));
                        }
                    }
                }
                if (previous != 0) {
                    u16 id = func_80136254(lbl_eu_806640E0, lbl_eu_80504268 + 0x57B, previous);
                    if (id != 0) {
                        menuVisionReplacePaneImage(statusAlt, menuVisionResource(func_80138F78(id)));
                    }
                }
            }

            if (battle->targetActor != 0) {
                void* target = func_800B708C__Fi(battle->targetActor);
                target = target != 0 ? (u8*)target - 0x3E9C : 0;
                if (target != 0 && (battle->flags824 & 0x400)) {
                    menuVisionSetVisible(entry.mLayout->GetRootPane()->FindPaneByName(lbl_eu_80504268 + 0x55D, true), true);
                }
            }
        }
        menuVisionBegin(entry);

        CMenuVisionEntry& barEntry = menu->mEntries[4];
        nw4r::lyt::Pane* bar = barEntry.mLayout->GetRootPane()->FindPaneByName(lbl_eu_80504268 + 0x215, true);
        PaneTransAccess* trans = reinterpret_cast<PaneTransAccess*>(bar);
        nw4r::math::VEC2 position;
        position.x = trans->transX;
        position.y = trans->transY;
        position.x = lbl_eu_80667DC8 * func_800F4424(battle);
        bar = barEntry.mLayout->GetRootPane()->FindPaneByName(lbl_eu_80504268 + 0x215, true);
        trans = reinterpret_cast<PaneTransAccess*>(bar);
        trans->transX = position.x;
        trans->transY = position.y;
        menuVisionBegin(barEntry);
    }

    if (flags & 0x10) {
        CMenuVision* menu = lbl_eu_80664388;
        CMenuVisionEntry& entry = menu->mEntries[5];
        int value = (int)func_800F42AC(battle);
        if (value > 999) {
            value = 999;
        } else if (value < 0) {
            value = 0;
        }
        func_80136910__FPQ34nw4r3lyt6LayoutPcUc(entry.mLayout, lbl_eu_80504268 + 0x208, value);
        menuVisionBegin(entry);
    }
}

#pragma pop

void func_801AF934(){}

void func_801AFAD0(CMenuVision*, CMenuVisionEntry*){}

// Mangled linker names used by adjustor thunks below

void func_801AFE04(void* self) { ((void(*)(void*))__dt__11CMenuVisionFv)((char*)self - 0x58); }

void func_801AFE0C(void* self) { ((void(*)(void*))cbRenderBefore__11CMenuVisionFv)((char*)self - 0x5c); }

void func_801AFE14(void* self) { ((void(*)(void*))__dt__11CMenuVisionFv)((char*)self - 0x5c); }

void sinit_801AFCE8(){}

// --- CMenuVision::Move ---
void CMenuVision::Move() {
    // Gate: return early if game paused, event busy, or UI suppressed
    if (CTaskGame::getInstance()->func_800426F0()) {
        return;
    }
    if (lbl_eu_80663E28 & 0x400) { // bit 10 (IBM bit 21)
        return;
    }
    if (!func_8013BE50()) {
        return;
    }
    if (lbl_eu_80663E24 & 0x40) { // bit 6 (IBM bit 25)
        return;
    }

    // Check if any entry has non-zero state - if all are zero, skip update
    bool anyActive = false;
    for (int i = 0; i < 6; i++) {
        if (mEntries[i].mState != 0) {
            anyActive = true;
            break;
        }
    }
    if (!anyActive) {
        return;
    }

    f32 scale = lbl_eu_80667DC8;
    f32 zero = lbl_eu_80667DC0;
    const char* strBase = lbl_eu_80504268;

    for (int i = 0; i < 6; i++) {
        CMenuVisionEntry& e = mEntries[i];
        nw4r::lyt::Layout* layout = e.mLayout;

        switch (e.mState) {
        case 1:
            // Play mAnim1 to 1.0f; when done, advance to state 3
            if (func_80137444(e.mAnim1, lbl_eu_80667DC4)) {
                e.mState = 3;
            }
            break;

        case 2: {
            void* bmObj = cf::CBattleManager::getInstance()->func_800EA444();
            if (bmObj != NULL) {
                // Check if the first indicator pane is visible
                nw4r::lyt::Pane* pane1 = layout->GetRootPane()->FindPaneByName(strBase + 0x195, true);
                if (reinterpret_cast<PaneVisAccess*>(pane1)->visByte & 1) {
                    // Read transX/transY from position pane, scale transX
                    nw4r::lyt::Pane* posPane = layout->GetRootPane()->FindPaneByName(strBase + 0x215, true);
                    f32 savedX = reinterpret_cast<PaneTransAccess*>(posPane)->transX;
                    f32 savedY = reinterpret_cast<PaneTransAccess*>(posPane)->transY;
                    savedX = scale * func_800F4424(bmObj);
                    // Re-find and write back
                    posPane = layout->GetRootPane()->FindPaneByName(strBase + 0x215, true);
                    reinterpret_cast<PaneTransAccess*>(posPane)->transX = savedX;
                    reinterpret_cast<PaneTransAccess*>(posPane)->transY = savedY;
                }
            } else {
                // Disable all anims except mAnim2, set mAnim2 to frame 0, advance to state 3
                layout->SetAnimationEnable(e.mAnim3, false);
                layout->SetAnimationEnable(e.mAnim4, false);
                layout->SetAnimationEnable(e.mAnim5, false);
                layout->SetAnimationEnable(e.mAnim6, false);
                layout->SetAnimationEnable(e.mAnim7, false);
                layout->SetAnimationEnable(e.mAnim8, false);
                layout->SetAnimationEnable(e.mAnim1, false);
                layout->SetAnimationEnable(e.mAnim2, true);
                e.mAnim2->SetFrame(zero);
                e.mState = 3;
            }
            break;
        }

        case 3:
            // Play mAnim2 to 1.0f; when done, return to state 0
            if (func_80137444(e.mAnim2, lbl_eu_80667DC4)) {
                e.mState = 0;
            }
            break;

        case 4:
            // Play mAnim4 to 1.0f; when done, return to state 0
            if (func_80137444(e.mAnim4, lbl_eu_80667DC4)) {
                e.mState = 0;
            }
            break;

        case 5:
            func_801AFAD0(this, &e);
            break;
        }

        layout->Animate(0);
    }
}

// --- CMenuVision::Init ---
void CMenuVision::Init() {
    mtl::ALLOC_HANDLE handle = mtl::MemManager::getHandleMEM2();
    mLayoutMem.createRegion(handle, 0x2C000, lbl_eu_80504268, 0);

    Class_8045F858 regionGuard(&mLayoutMem);

    // Copy 6 pane-name pointers from lbl_eu_805041C0 to local array
    const char* paneNames[6];
    paneNames[0] = (const char*)lbl_eu_805041C0[0];
    paneNames[1] = (const char*)lbl_eu_805041C0[1];
    paneNames[2] = (const char*)lbl_eu_805041C0[2];
    paneNames[3] = (const char*)lbl_eu_805041C0[3];
    paneNames[4] = (const char*)lbl_eu_805041C0[4];
    paneNames[5] = (const char*)lbl_eu_805041C0[5];

    nw4r::lyt::ArcResourceAccessor* accessor = func_801355F4();
    const char* strBase = lbl_eu_80504268;
    f32 zero = lbl_eu_80667DC0;

    for (int i = 0; i < 6; i++) {
        CMenuVisionEntry& e = mEntries[i];

        func_80136E84(&e.mLayout, accessor, (char*)strBase + 0xb);
        nw4r::lyt::Layout* layout = e.mLayout;

        func_80136F08(layout, &e.mAnim1, accessor, (char*)strBase + 0x2b);
        func_80136F08(layout, &e.mAnim2, accessor, (char*)strBase + 0x4e);
        func_80136F08(layout, &e.mAnim3, accessor, (char*)strBase + 0x72);
        func_80136F08(layout, &e.mAnim4, accessor, (char*)strBase + 0x9a);
        func_80136F08(layout, &e.mAnim5, accessor, (char*)strBase + 0xc5);
        func_80136F08(layout, &e.mAnim6, accessor, (char*)strBase + 0xef);
        func_80136F08(layout, &e.mAnim7, accessor, (char*)strBase + 0x119);
        func_80136F08(layout, &e.mAnim8, accessor, (char*)strBase + 0x13f);

        // Disable anims 2-8, enable anim1, reset to frame 0
        layout->SetAnimationEnable(e.mAnim2, false);
        layout->SetAnimationEnable(e.mAnim3, false);
        layout->SetAnimationEnable(e.mAnim4, false);
        layout->SetAnimationEnable(e.mAnim5, false);
        layout->SetAnimationEnable(e.mAnim6, false);
        layout->SetAnimationEnable(e.mAnim7, false);
        layout->SetAnimationEnable(e.mAnim8, false);
        layout->SetAnimationEnable(e.mAnim1, true);
        e.mAnim1->SetFrame(zero);
        layout->Animate(0);

        // Hide 6 standard panes by clearing bit 0 of visByte
        nw4r::lyt::Pane* rootPane = layout->GetRootPane();
        nw4r::lyt::Pane* p;

        p = rootPane->FindPaneByName(strBase + 0x169, true);
        reinterpret_cast<PaneVisAccess*>(p)->visByte &= ~1;
        p = rootPane->FindPaneByName(strBase + 0x174, true);
        reinterpret_cast<PaneVisAccess*>(p)->visByte &= ~1;
        p = rootPane->FindPaneByName(strBase + 0x17f, true);
        reinterpret_cast<PaneVisAccess*>(p)->visByte &= ~1;
        p = rootPane->FindPaneByName(strBase + 0x18a, true);
        reinterpret_cast<PaneVisAccess*>(p)->visByte &= ~1;
        p = rootPane->FindPaneByName(strBase + 0x195, true);
        reinterpret_cast<PaneVisAccess*>(p)->visByte &= ~1;
        p = rootPane->FindPaneByName(strBase + 0x1a2, true);
        reinterpret_cast<PaneVisAccess*>(p)->visByte &= ~1;

        // Show the entry-specific pane (clear all bits, then set bit 0)
        p = rootPane->FindPaneByName(paneNames[i], true);
        reinterpret_cast<PaneVisAccess*>(p)->visByte &= ~1;
        reinterpret_cast<PaneVisAccess*>(p)->visByte |= 1;

        // Per-index special setup
        if (i == 1) {
            u8* fontObj = (u8*)CDeviceFont::func_80452C10(1, layout);
            typedef u32 (*FontVFn)(void*);
            u32 fontVal = (*reinterpret_cast<FontVFn**>(fontObj))[0x24 / 4](fontObj);

            func_801368C0(layout, (char*)strBase + 0x1ab, fontVal);
            func_801368C0(layout, (char*)strBase + 0x1b8, fontVal);
            func_801368C0(layout, (char*)strBase + 0x1c5, fontVal);
            func_801368C0(layout, (char*)strBase + 0x1d2, fontVal);
            func_801368C0(layout, (char*)strBase + 0x1df, fontVal);
            func_801368C0(layout, (char*)strBase + 0x1ed, fontVal);
        } else if (i == 3) {
            u32 val = func_801355A0();
            func_801368C0(layout, (char*)strBase + 0x1fb, val);
        } else if (i == 5) {
            u32 val = func_801355A0();
            func_801368C0(layout, (char*)strBase + 0x208, val);
        }
    }

    // Register render-before callback (null-safe IScnRender subobject at +0x5C)
    IScnRender* cb = reinterpret_cast<IScnRender*>(this);
    if (this != NULL) {
        cb = reinterpret_cast<IScnRender*>(&unk00[0x5C]);
    }
    mScn->addRenderCB(cb, 0xF, 0);
}