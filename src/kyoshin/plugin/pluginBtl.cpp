// Auto-scaffolded catalog TU for kyoshin/plugin/pluginBtl
// Plugin functions for the YGG VM "btl" subsystem (battle management).
// These are called from script via the function table registered in
// pluginBtlRegist.

#include "kyoshin/harness_catalog.hpp"

// Forward declarations for types used in the extern API below
namespace cf {
class CBattleManager {
public:
    static CBattleManager* getInstance();
    void func_800E2584(u32 mask);
    void func_80085220(u32 r4, u32 r5);
    void* func_800EA444();
    // +0x194: UnkClass_8018C5FC
    // +0x219C: CVision
};
class CfGameManager {
public:
    static void* getPlayer(int index);
};
}

// --- C-linkage retail helpers ---
extern "C" {
    // CBattleManager helpers
    void func_800F3C08(cf::CBattleManager*, int);
    int func_800F3DC8(cf::CBattleManager*, int);
    void func_800F4034(cf::CBattleManager*);
    void func_800F3F8C(cf::CBattleManager*);
    void func_800F3FC8(cf::CBattleManager*);
    void func_800D9978(cf::CBattleManager*, void*);

    // Enum list holder (8-byte stack struct: ptr + handle)
    void func_80043D90(void* holder);
    void func_80043E08(void* holder, int type, int subtype);
    void* func_80043F18(void* holder);
    void __dt__80043E88(void* holder, int);

    // Enum list operations
    void func_800F4A98(void* list, int type, int filter);
    void* func_800F6EAC(void* list, u32 idx);
    void* func_800F6ED0(void* list, void* obj);
    void* func_800F6E08(void* list);

    // List init with filter (isEnd)
    void __ct__800FC32C(void* list, int a, int b, int c);

    // Camera / selector helpers
    void* func_800FE68C();
    void func_800FE96C(void* cam, void* target);
    void func_800FE950(void* cam, u32 a, u32 b, u32 c);

    // Vision
    void func_801A8244(void* vision, void* visionList, int a, int b, int c);

    // Voice / action helpers
    void* func_8016FE34(void* source);
    void func_801537F0(void* self);
    void func_801537E0(void* self);

    // PTG / tension helpers
    void func_8018C8F4(void* self, int ptg);
    void* func_801862C0();
    void* func_801864DC(void* mgr, int slot);

    // Monado arts unlock
    void* func_8009EC9C(u16 index);
    void func_801F9288(void* self, int, int, int);
    void func_801F92B0(void* self, int, int, int);

    // Game state / flags
    bool func_8006EF04__Fi(s32 mask);
    u32 func_80192BD0();

    // bdat helpers
    void* getFP__FPCc(const char* name);
    const char* getBdatStringColumnValue(void* bdat, const char* col, u32 index);
    u32 func_8003B1EC(void* bdat);
    void func_8003AA34();

    // RTTI type info for dynamic_cast (attack function)
    extern char lbl_eu_806618D8[];
    extern char lbl_eu_806619A0[];
    void* __dynamic_cast(void*, int, void*, void*, int);
    void* func_800B708C__Fi(int);
}

// --- pluginBtl function implementations ---

int startObserve(VMThread* pThread) {
    cf::CBattleManager* bm = cf::CBattleManager::getInstance();
    func_800F3C08(bm, 1);
    return 0;
}

int endObserve(VMThread* pThread) {
    cf::CBattleManager* bm = cf::CBattleManager::getInstance();
    func_800F3C08(bm, 0);
    return 0;
}

int defeatingCount(VMThread* pThread) {
    VMArg* argPtr = vmArgPtrGet(pThread, 1);
    int key = vmArgIntGet(2, argPtr);
    VMArg arg;
    arg.type = 3;
    cf::CBattleManager* bm = cf::CBattleManager::getInstance();
    arg.value.uintVal = func_800F3DC8(bm, key);
    vmRetValSet(pThread, &arg);
    return 1;
}

int isEnd(VMThread* pThread) {
    u8 holder[8];
    int result = 0;
    cf::CBattleManager* bm = cf::CBattleManager::getInstance();
    void** sentinelPtr = *(void***)((u8*)bm + 8);
    int count = 0;
    for (void* node = *sentinelPtr; node != sentinelPtr; node = *(void**)node) {
        count++;
    }
    if (count == 0) {
        func_80043E08(holder, 0x20, 0x800);
        void* list = func_80043F18(holder);
        __ct__800FC32C(list, 3, 4, 0);
        list = func_80043F18(holder);
        if (*(u32*)((u8*)list + 0x620) != 0) {
            result = 1;
        }
        __dt__80043E88(holder, -1);
    }
    VMArg arg;
    arg.type = (__cntlzw(result) >> 5) + 1;
    arg.value.uintVal = 0;
    vmRetValSet(pThread, &arg);
    return 1;
}

int end(VMThread* pThread) {
    cf::CBattleManager* bm = cf::CBattleManager::getInstance();
    func_800F4034(bm);
    return 0;
}

int attack(VMThread* pThread) {
    bool showArt;
    if (vmArgOmitChk(pThread, 1) != 0) {
        showArt = true;
    } else {
        showArt = vmArgBoolGet(2, vmArgPtrGet(pThread, 1));
    }
    for (int i = 0; i < 3; i++) {
        void* player = cf::CfGameManager::getPlayer(i);
        void* battleObj = __dynamic_cast(player, 0, &lbl_eu_806619A0, &lbl_eu_806618D8, 0);
        if (battleObj == 0) continue;
        void* subObj = (u8*)battleObj + 0x3E9C;
        void** vtable = *(void***)subObj;
        typedef void (*FlagFunc)(void*, u32);
        if (showArt) {
            ((FlagFunc)vtable[2])(subObj, 0x100);
            ((FlagFunc)vtable[4])(subObj, 0x200);
        } else {
            ((FlagFunc)vtable[2])(subObj, 0x200);
            ((FlagFunc)vtable[4])(subObj, 0x100);
        }
    }
    return 0;
}

int attackEne(VMThread* pThread) {
    int targetId;
    if (vmArgOmitChk(pThread, 1) != 0) {
        targetId = 0;
    } else {
        targetId = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    }
    u8 holder[8];
    func_80043D90(holder);
    func_800F4A98(func_80043F18(holder), 0x100, 0);
    u32 i = 0;
    while (true) {
        void* list2 = func_80043F18(holder);
        if (i >= *(u32*)((u8*)list2 + 0x620)) break;
        void* unit = func_8016FE34(func_800F6EAC(list2, i));
        if (*(u16*)((u8*)unit + 0x3F28) == targetId) {
            func_800D9978(cf::CBattleManager::getInstance(), unit);
        }
        i++;
    }
    __dt__80043E88(holder, -1);
    return 0;
}

int selectTgt(VMThread* pThread) {
    void* ocObj;
    if (vmArgOmitChk(pThread, 1) != 0) {
        ocObj = 0;
    } else {
        ocObj = vmArgOCGet(2, vmArgPtrGet(pThread, 1));
    }
    if (ocObj != 0) {
        void* mgr = func_801862C0();
        void* slot = func_801864DC(mgr, *(u32*)((u8*)ocObj + 4));
        if (slot != 0) {
            void* cam = func_800FE68C();
            func_800FE96C(cam, *(void**)((u8*)slot + 0x74));
        }
    } else {
        u8 holder[8];
        func_80043D90(holder);
        void* list = func_80043F18(holder);
        func_800F4A98(list, 0x100, 0x802);
        void* player = cf::CfGameManager::getPlayer(0);
        void** vtab = *(void***)player;
        typedef void* (*GetTargetFunc)(void*);
        void* target = ((GetTargetFunc)vtab[0x2B])(player);
        list = func_80043F18(holder);
        func_800F6ED0(list, target);
        void* cam = func_800FE68C();
        func_800FE950(cam, 0x80000003, 0, 0);
        void* val = func_800F6E08(func_80043F18(holder));
        cam = func_800FE68C();
        func_800FE96C(cam, val);
        __dt__80043E88(holder, -1);
    }
    return 0;
}

int vision(VMThread* pThread) {
    bool enable;
    if (vmArgOmitChk(pThread, 1) != 0) {
        enable = true;
    } else {
        enable = vmArgBoolGet(2, vmArgPtrGet(pThread, 1));
    }
    cf::CBattleManager* bm = cf::CBattleManager::getInstance();
    if (enable) {
        bm->func_800E2584(0x200);
    } else {
        bm->func_80085220(0x200, 1);
    }
    return 0;
}

int voiceEvent(VMThread* pThread) {
    bool enable;
    if (vmArgOmitChk(pThread, 1) != 0) {
        enable = true;
    } else {
        enable = vmArgBoolGet(2, vmArgPtrGet(pThread, 1));
    }
    cf::CBattleManager* bm = cf::CBattleManager::getInstance();
    if (enable) {
        func_800F3F8C(bm);
    } else {
        func_800F3FC8(bm);
    }
    return 0;
}

int isVoiceEvent(VMThread* pThread) {
    int result = 0;
    cf::CfGameManager::getInstance();
    if (func_8006EF04__Fi(0x10000000) != 0) {
        extern bool CMenuArtsSelect_isCreated();
        if (!CMenuArtsSelect_isCreated() && func_80192BD0() == 0) {
            result = 1;
        }
    }
    VMArg arg;
    arg.type = (__cntlzw(result) >> 5) + 1;
    arg.value.uintVal = 0;
    vmRetValSet(pThread, &arg);
    return 1;
}

int unlockMonadoArts(VMThread* pThread) {
    void* data = func_8009EC9C(1);
    void* ptr = (u8*)data + 0xE8;
    for (int i = 0; i < 8; i++) {
        func_801F9288(ptr, 0, 1, i);
        func_801F92B0(ptr, 0, 1, i);
    }
    return 0;
}

int setTensionLv(VMThread* pThread) {
    int level;
    if (vmArgOmitChk(pThread, 1) != 0) {
        level = 3;
    } else {
        level = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    }
    for (int i = 0; i < 3; i++) {
        void* player = cf::CfGameManager::getPlayer(i);
        void* action = func_8016FE34(player);
        if (action != 0) {
            void** vtable = *(void***)action;
            typedef void (*SetTensionFunc)(void*, int);
            ((SetTensionFunc)vtable[0xC1])(action, level - 1);
        }
    }
    return 0;
}

int setTP(VMThread* pThread) {
    int tp;
    int targetId;
    if (vmArgOmitChk(pThread, 1) != 0) {
        tp = 100;
        targetId = 2;
    } else {
        tp = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
        targetId = 2;
    }
    if (vmArgOmitChk(pThread, targetId) != 0) {
        targetId = 0;
    } else {
        targetId = vmArgIntGet(2, vmArgPtrGet(pThread, targetId + 1));
    }
    for (int i = 0; i < 3; i++) {
        void* player = cf::CfGameManager::getPlayer(i);
        void* action = func_8016FE34(player);
        if (action == 0) continue;
        if (targetId != 0 && *(u16*)((u8*)action + 0x3F28) != targetId) continue;
        void** vtable = *(void***)action;
        typedef void (*SetTPFunc)(void*, f32);
        ((SetTPFunc)vtable[0x54])(action, (f32)tp);
    }
    return 0;
}

int breakVision(VMThread* pThread) {
    if (vmArgOmitChk(pThread, 1) == 0) {
        vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    }
    cf::CBattleManager* bm = cf::CBattleManager::getInstance();
    void* visionList = bm->func_800EA444();
    if (visionList != 0) {
        void* actor = func_800B708C__Fi(*(u32*)((u8*)visionList + 0));
        void* voiceAction = func_8016FE34(actor);
        func_801A8244((u8*)bm + 0x219C, visionList, 5, 1, 0);
        if (voiceAction != 0) {
            func_801537F0((u8*)voiceAction + 0x3380);
            func_801537E0((u8*)voiceAction + 0x3380);
        }
    }
    return 0;
}

int setPTG(VMThread* pThread) {
    int ptg = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    cf::CBattleManager* bm = cf::CBattleManager::getInstance();
    if (bm != 0) {
        bm = cf::CBattleManager::getInstance();
        func_8018C8F4((u8*)bm + 0x194, ptg);
    }
    return 0;
}

int getPTG(VMThread* pThread) {
    u32 ptg = 0;
    cf::CBattleManager* bm = cf::CBattleManager::getInstance();
    if (bm != 0) {
        bm = cf::CBattleManager::getInstance();
        ptg = *(u32*)((u8*)bm + 0x194);
    }
    VMArg arg;
    arg.type = 3;
    arg.value.uintVal = ptg;
    vmRetValSet(pThread, &arg);
    return 1;
}

int test(VMThread* pThread) {
    func_8003AA34();
    func_8003AA34();
    void* bdat = getFP__FPCc("ene_arts");
    u32 count = func_8003B1EC(bdat);
    for (int i = 1; i < (int)count; i++) {
        getBdatStringColumnValue(bdat, "name", i);
    }
    return 0;
}

// --- Plugin registration ---

extern "C" void pluginBtlRegist() {
    extern void vmPluginRegist(void*, void*);
    extern char lbl_805038B0[];
    extern char lbl_80532360[];
    vmPluginRegist((void*)((char*)lbl_805038B0 + 0xe), (void*)lbl_80532360);
}
