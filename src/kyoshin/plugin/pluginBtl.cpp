// Auto-scaffolded catalog TU for kyoshin/plugin/pluginBtl
// Plugin functions for the YGG VM "btl" subsystem (battle management).
// These are called from script via the function table registered in
// pluginBtlRegist.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/IFactoryEvent.hpp"
#include "kyoshin/cfsys/CfWalkerCore.hpp"
#include "kyoshin/cf/object/CfObject.hpp"
#include "kyoshin/cf/object/CActorParam.hpp"

// Retail symbol has C linkage (plain reloc name); declared here per the
// reloc-name fix rather than including the heavy menu header.
extern "C" bool CMenuArtsSelect_isCreated();

namespace cf {
// Intrusive ring-list node used by CBattleManager's unit list (sentinel at +0x8).
struct BtlRingNode {
    BtlRingNode* next; // 0x00
};

// Battle manager (real prefix: IFactoryEvent handlers at +0x08..+0x18, see
// retail table lbl_eu_8052BCE0; setPartyMaskFlag/func_800E2584 at +0x1C/+0x20).
// Local decl (never constructed here, so no vtable is emitted).
class CBattleManager : public IFactoryEvent {
public:
    static CBattleManager* getInstance();
    virtual void setPartyMaskFlag(u32 r4, u32 r5); // +0x1C
    virtual void func_800E2584(u32 mask);          // +0x20
};
}

// (Actor slots now fold onto their owners: tension 0x304 ->
// cf::CActorParam::CActorParam_UnkVirtualFunc156, TP 0x150 ->
// CActorParam_UnkVirtualFunc47, attack flags +0x08/+0x10 ->
// cf::CfWalkMoveSub::m08/m10, target getter +0xAC ->
// cf::CfObject::CfObject_UnkVirtualFunc23.)

// --- C-linkage retail helpers ---
// (Declarations already provided by included headers: func_800F3C08,
// func_800EA444, func_800D9978 and the CfMoveEnumHolder/CfMoveEnumList
// family live in cfsys/CfObjectImplMove.hpp / cf/CfGameManager.hpp;
// func_801862C0/func_801864DC/func_8003AA34/func_8003B1EC in
// kyoshin/code_801862C0.hpp.)
extern "C" {
    // CBattleManager helpers
    int func_800F3DC8(cf::CBattleManager*, int);
    void func_800F4034(cf::CBattleManager*);
    void func_800F3F8C(cf::CBattleManager*);
    void func_800F3FC8(cf::CBattleManager*);

    // Enum list holder ctor (dtor/accessors come from CfObjectImplMove.hpp)
    void func_80043E08(void* holder, int type, int subtype);

    // Enum list operations
    void __ct__800FC32C(void* list, int a, int b, int c);

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
    void* func_801862C0();
    void* func_801864DC(void* mgr, int slot);

    // Monado arts unlock
    void* func_8009EC9C(u32 index);
    void func_801F9288(void* self, int, int, int);
    void func_801F92B0(void* self, int, int, int);

    // Game state / flags
    bool isGlobalCamFlagSet__Fi(s32 mask);
    u32 func_80192BD0();

    // bdat helpers
    void* getFP__FPCc(const char* name);

    // Shared int->float conversion magic double (0x43300000_80000000), .sdata2.
    extern double lbl_eu_80667978;

    // RTTI type info for dynamic_cast (attack function).
    // Declared as u32 so MWCC sdata-addresses them (EMB_SDA21) like retail.
    extern u32 lbl_eu_806618D8;
    extern u32 lbl_eu_806619A0;
    void* __dynamic_cast(void*, long, const void*, const void*, void*);
    void* findObjectById__Fi(int);
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
    VMArg arg;
    u8 holder[8];
    int result = 0;
    cf::CBattleManager* bm = cf::CBattleManager::getInstance();
    // Intrusive ring list: bm+8 points at a sentinel node whose `next`
    // wraps back to itself when empty. Declaration order mirrors the matched
    // func_8027F0B8 loop so MWCC colors the node r3 / sentinel r5.
    cf::BtlRingNode* node;
    int count;
    cf::BtlRingNode* sentinel = *(cf::BtlRingNode**)((u8*)bm + 8);
    count = 0;
    node = sentinel->next;
    while (node != sentinel) {
        node = node->next;
        ++count;
    }
    if (count == 0) {
        func_80043E08(&holder, 0x20, 0x800);
        void* list = func_80043F18((CfMoveEnumHolder*)holder);
        __ct__800FC32C(list, 3, 4, 0);
        list = func_80043F18((CfMoveEnumHolder*)holder);
        if (*(u32*)((u8*)list + 0x620) != 0) {
            result = 1;
        }
        __dt__80043E88((CfMoveEnumHolder*)holder, -1);
    }
    // unsigned cast forces MWCC's rotate-form shift (rlwinm), matching retail
    arg.type = ((unsigned)__cntlzw(result) >> 5) + 1;
    vmRetValSet(pThread, &arg);
    return 1;
}

int end(VMThread* pThread) {
    cf::CBattleManager* bm = cf::CBattleManager::getInstance();
    func_800F4034(bm);
    return 0;
}


// Player actor layout: embedded battle sub-object at +0x3E9C (real owner:
// cf::CfWalkMoveSub; m08/m10 at +0x08/+0x10 set the attack-art flag bits).
struct BtlActorLayout {
    u8 _00[0x3E9C];
    cf::CfWalkMoveSub attackView;
};

int attack(VMThread* pThread) {
    int showArt;
    if (vmArgOmitChk(pThread, 1) != 0) {
        showArt = 1;
    } else {
        showArt = vmArgBoolGet(2, vmArgPtrGet(pThread, 1));
    }
    for (int i = 0; i < 3; i++) {
        BtlActorLayout* actor = (BtlActorLayout*)__dynamic_cast(
            cf::CfGameManager::getPlayer(i), 0, &lbl_eu_806619A0,
            &lbl_eu_806618D8, 0);
        if (actor != 0) {
            // showArt set: player-side arts on / enemy-side off; cleared: swapped.
            if (showArt != 0) {
                actor->attackView.m08(0x100);
                actor->attackView.m10(0x200);
            } else {
                actor->attackView.m08(0x200);
                actor->attackView.m10(0x100);
            }
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
    func_80043D90((CfMoveEnumHolder*)holder);
    func_800F4A98(func_80043F18((CfMoveEnumHolder*)holder), 0x100, 0);
    // Count is re-read via the holder accessor every iteration (retail shape).
    for (u32 i = 0; i < *(u32*)((u8*)func_80043F18((CfMoveEnumHolder*)holder) + 0x620); i++) {
        void* unit = func_8016FE34(func_800F6EAC((CfMoveEnumList*)func_80043F18((CfMoveEnumHolder*)holder), i));
        if (targetId == *(u16*)((u8*)unit + 0x3F28)) {
            func_800D9978(cf::CBattleManager::getInstance(), unit);
        }
    }
    __dt__80043E88((CfMoveEnumHolder*)holder, -1);
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
        func_80043D90((CfMoveEnumHolder*)holder);
        void* list = func_80043F18((CfMoveEnumHolder*)holder);
        func_800F4A98((CfMoveEnumList*)list, 0x100, 0x802);
        void* player = cf::CfGameManager::getPlayer(0);
        // Target getter at vtable+0xAC is
        // cf::CfObject::CfObject_UnkVirtualFunc23 on the player object.
        void* target = (void*)((cf::CfObject*)player)->CfObject_UnkVirtualFunc23();
        list = func_80043F18((CfMoveEnumHolder*)holder);
        func_800F6ED0((CfMoveEnumList*)list, target);
        void* cam = func_800FE68C();
        func_800FE950(cam, 0x80000003, 0, 0);
        void* val = func_800F6E08(func_80043F18((CfMoveEnumHolder*)holder));
        cam = func_800FE68C();
        func_800FE96C(cam, val);
        __dt__80043E88((CfMoveEnumHolder*)holder, -1);
    }
    return 0;
}

int vision(VMThread* pThread) {
    int enable;
    if (vmArgOmitChk(pThread, 1) != 0) {
        enable = 1;
    } else {
        enable = vmArgBoolGet(2, vmArgPtrGet(pThread, 1));
    }
    if (enable) {
        cf::CBattleManager* bm = cf::CBattleManager::getInstance();
        bm->func_800E2584(0x200);
    } else {
        cf::CBattleManager* bm = cf::CBattleManager::getInstance();
        bm->setPartyMaskFlag(0x200, 1);
    }
    return 0;
}

int voiceEvent(VMThread* pThread) {
    // Ternary keeps the omit-check/arg-read result in r3 across the merge,
    // matching retail's register allocation.
    if (vmArgOmitChk(pThread, 1) ? 1 : vmArgBoolGet(2, vmArgPtrGet(pThread, 1))) {
        cf::CBattleManager* bm = cf::CBattleManager::getInstance();
        func_800F3F8C(bm);
    } else {
        cf::CBattleManager* bm = cf::CBattleManager::getInstance();
        func_800F3FC8(bm);
    }
    return 0;
}

int isVoiceEvent(VMThread* pThread) {
    int result = 0;
    cf::CfGameManager::getInstance();
    if (isGlobalCamFlagSet__Fi(0x10000000) != 0) {
        if (!CMenuArtsSelect_isCreated() && func_80192BD0() == 0) {
            result = 1;
        }
    }
    // Only the bool type byte is written; the value field stays uninitialized
    // in retail (cntlzw/srwi bool-to-0-1 idiom).
    VMArg arg;
    arg.type = (((u32)__cntlzw(result)) >> 5) + 1;
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
            ((cf::CActorParam*)action)->CActorParam_UnkVirtualFunc156(level - 1);
        }
    }
    return 0;
}

int setTP(VMThread* pThread) {
    int idx;
    int tp;
    if (vmArgOmitChk(pThread, 1) != 0) {
        tp = 100;
        idx = 2;
    } else {
        // Nested assignment colors idx into the same register phase as the
        // default branch in retail.
        tp = vmArgIntGet(idx = 2, vmArgPtrGet(pThread, 1));
    }
    // Single-def form keeps idx's two phases in one register web.
    if (vmArgOmitChk(pThread, idx) != 0) {
        idx = 0;
    } else {
        // nextIdx defined first so MWCC schedules the addi before the
        // ptrGet call, like retail.
        int nextIdx = idx + 1;
        VMArg* argPtr = vmArgPtrGet(pThread, idx);
        idx = vmArgIntGet(nextIdx, argPtr);
    }
    // Shared sdata2 magic double (0x43300000_80000000). MWCC hoists the load
    // into f31 across the loop.
    const double cvtBias = lbl_eu_80667978;
    for (int i = 0; i < 3; i++) {
        void* action = func_8016FE34(cf::CfGameManager::getPlayer(i));
        if (action != 0) {
            if (idx == 0 || *(u16*)((u8*)action + 0x3F28) == idx) {
                // Built per-iteration so the words round-trip through the
                // stack frame like retail (stw/stw/lfd/fsubs).
                union {
                    double d;
                    struct { u32 hi, lo; } w;
                } conv;
                conv.w.hi = 0x43300000;
                conv.w.lo = (u32)tp ^ 0x80000000;
                // TP setter at vtable+0x150 takes the converted float:
                // cf::CActorParam::CActorParam_UnkVirtualFunc47.
                ((cf::CActorParam*)action)->CActorParam_UnkVirtualFunc47((f32)(conv.d - cvtBias));
            }
        }
    }
    return 0;
}

int breakVision(VMThread* pThread) {
    if (vmArgOmitChk(pThread, 1) == 0) {
        vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    }
    cf::CBattleManager* bm = cf::CBattleManager::getInstance();
    void* visionList = func_800EA444(bm);
    if (visionList != 0) {
        void* actor = findObjectById__Fi(*(u32*)((u8*)visionList + 0));
        void* voiceAction = func_8016FE34(actor);
        // Retail re-fetches the manager here instead of caching it across the
        // intervening calls (keeps the frame small).
        bm = cf::CBattleManager::getInstance();
        func_801A8244((u8*)bm + 0x219C, visionList, 5, 1, 0);
        if (voiceAction != 0) {
            // Different syntax families stop MWCC from CSE-ing the address
            // into an in-place update of voiceAction (retail recomputes the
            // addi per call).
            func_801537F0((u8*)voiceAction + 0x3380);
            func_801537E0(&((u8*)voiceAction)[0x3380]);
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
    // Retail reloc names carry the eu_ prefix.
    extern char lbl_eu_805038B0[];
    extern char lbl_eu_80532360[];
    vmPluginRegist((void*)((char*)lbl_eu_805038B0 + 0xe), (void*)lbl_eu_80532360);
}
