// Auto-scaffolded catalog TU for kyoshin/plugin/pluginBtl
// Plugin functions for the YGG VM "btl" subsystem (battle management).
// These are called from script via the function table registered in
// pluginBtlRegist.

#include "kyoshin/harness_catalog.hpp"

// Retail symbol has C linkage (plain reloc name); declared here per the
// reloc-name fix rather than including the heavy menu header.
extern "C" bool CMenuArtsSelect_isCreated();

namespace cf {
// Intrusive ring-list node used by CBattleManager's unit list (sentinel at +0x8).
struct BtlRingNode {
    BtlRingNode* next; // 0x00
};

class CBattleManager {
public:
    static CBattleManager* getInstance();
    // vtable slots 0x14/0x18 are the implicit __dt/__dl destructor entries
    // MWCC emits for a polymorphic class; the five stubs pad up to them.
    virtual void v000() = 0;
    virtual void v001() = 0;
    virtual void v002() = 0;
    virtual void v003() = 0;
    virtual void v004() = 0;
    virtual void func_80085220(u32 r4, u32 r5) = 0;   // slot 0x1C
    virtual void func_800E2584(u32 mask) = 0;         // slot 0x20
    // +0x194: UnkClass_8018C5FC
    // +0x219C: CVision
};
}

// Actor vtable interface: dispatches slot 0x304 (#191 under the kyoshin
// -RTTI (k+2)*4 layout). Only used as a dispatch view, never instantiated.
struct BtlSetTensionVt {
    virtual void v000() = 0; virtual void v001() = 0; virtual void v002() = 0;
    virtual void v003() = 0; virtual void v004() = 0; virtual void v005() = 0;
    virtual void v006() = 0; virtual void v007() = 0; virtual void v008() = 0;
    virtual void v009() = 0; virtual void v010() = 0; virtual void v011() = 0;
    virtual void v012() = 0; virtual void v013() = 0; virtual void v014() = 0;
    virtual void v015() = 0; virtual void v016() = 0; virtual void v017() = 0;
    virtual void v018() = 0; virtual void v019() = 0; virtual void v020() = 0;
    virtual void v021() = 0; virtual void v022() = 0; virtual void v023() = 0;
    virtual void v024() = 0; virtual void v025() = 0; virtual void v026() = 0;
    virtual void v027() = 0; virtual void v028() = 0; virtual void v029() = 0;
    virtual void v030() = 0; virtual void v031() = 0; virtual void v032() = 0;
    virtual void v033() = 0; virtual void v034() = 0; virtual void v035() = 0;
    virtual void v036() = 0; virtual void v037() = 0; virtual void v038() = 0;
    virtual void v039() = 0; virtual void v040() = 0; virtual void v041() = 0;
    virtual void v042() = 0; virtual void v043() = 0; virtual void v044() = 0;
    virtual void v045() = 0; virtual void v046() = 0; virtual void v047() = 0;
    virtual void v048() = 0; virtual void v049() = 0; virtual void v050() = 0;
    virtual void v051() = 0; virtual void v052() = 0; virtual void v053() = 0;
    virtual void v054() = 0; virtual void v055() = 0; virtual void v056() = 0;
    virtual void v057() = 0; virtual void v058() = 0; virtual void v059() = 0;
    virtual void v060() = 0; virtual void v061() = 0; virtual void v062() = 0;
    virtual void v063() = 0; virtual void v064() = 0; virtual void v065() = 0;
    virtual void v066() = 0; virtual void v067() = 0; virtual void v068() = 0;
    virtual void v069() = 0; virtual void v070() = 0; virtual void v071() = 0;
    virtual void v072() = 0; virtual void v073() = 0; virtual void v074() = 0;
    virtual void v075() = 0; virtual void v076() = 0; virtual void v077() = 0;
    virtual void v078() = 0; virtual void v079() = 0; virtual void v080() = 0;
    virtual void v081() = 0; virtual void v082() = 0; virtual void v083() = 0;
    virtual void v084() = 0; virtual void v085() = 0; virtual void v086() = 0;
    virtual void v087() = 0; virtual void v088() = 0; virtual void v089() = 0;
    virtual void v090() = 0; virtual void v091() = 0; virtual void v092() = 0;
    virtual void v093() = 0; virtual void v094() = 0; virtual void v095() = 0;
    virtual void v096() = 0; virtual void v097() = 0; virtual void v098() = 0;
    virtual void v099() = 0; virtual void v100() = 0; virtual void v101() = 0;
    virtual void v102() = 0; virtual void v103() = 0; virtual void v104() = 0;
    virtual void v105() = 0; virtual void v106() = 0; virtual void v107() = 0;
    virtual void v108() = 0; virtual void v109() = 0; virtual void v110() = 0;
    virtual void v111() = 0; virtual void v112() = 0; virtual void v113() = 0;
    virtual void v114() = 0; virtual void v115() = 0; virtual void v116() = 0;
    virtual void v117() = 0; virtual void v118() = 0; virtual void v119() = 0;
    virtual void v120() = 0; virtual void v121() = 0; virtual void v122() = 0;
    virtual void v123() = 0; virtual void v124() = 0; virtual void v125() = 0;
    virtual void v126() = 0; virtual void v127() = 0; virtual void v128() = 0;
    virtual void v129() = 0; virtual void v130() = 0; virtual void v131() = 0;
    virtual void v132() = 0; virtual void v133() = 0; virtual void v134() = 0;
    virtual void v135() = 0; virtual void v136() = 0; virtual void v137() = 0;
    virtual void v138() = 0; virtual void v139() = 0; virtual void v140() = 0;
    virtual void v141() = 0; virtual void v142() = 0; virtual void v143() = 0;
    virtual void v144() = 0; virtual void v145() = 0; virtual void v146() = 0;
    virtual void v147() = 0; virtual void v148() = 0; virtual void v149() = 0;
    virtual void v150() = 0; virtual void v151() = 0; virtual void v152() = 0;
    virtual void v153() = 0; virtual void v154() = 0; virtual void v155() = 0;
    virtual void v156() = 0; virtual void v157() = 0; virtual void v158() = 0;
    virtual void v159() = 0; virtual void v160() = 0; virtual void v161() = 0;
    virtual void v162() = 0; virtual void v163() = 0; virtual void v164() = 0;
    virtual void v165() = 0; virtual void v166() = 0; virtual void v167() = 0;
    virtual void v168() = 0; virtual void v169() = 0; virtual void v170() = 0;
    virtual void v171() = 0; virtual void v172() = 0; virtual void v173() = 0;
    virtual void v174() = 0; virtual void v175() = 0; virtual void v176() = 0;
    virtual void v177() = 0; virtual void v178() = 0; virtual void v179() = 0;
    virtual void v180() = 0; virtual void v181() = 0; virtual void v182() = 0;
    virtual void v183() = 0; virtual void v184() = 0; virtual void v185() = 0;
    virtual void v186() = 0; virtual void v187() = 0; virtual void v188() = 0;
    virtual void v189() = 0; virtual void v190() = 0;         // pads #0..#190
    virtual void vf304(s32 lv) = 0;                            // #191 -> 0x304
};

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
    void* func_8009EC9C(u16 index);
    void func_801F9288(void* self, int, int, int);
    void func_801F92B0(void* self, int, int, int);

    // Game state / flags
    bool func_8006EF04__Fi(s32 mask);
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

// Dispatch view onto the battle actor's embedded sub-object at +0x3E9C:
// slots 2 (vtable +0x08) and 4 (+0x10) set/clear the attack-art flag bits.
class BtlAttackView {
public:
    // MWCC prepends the implicit __dt/__dl pair to the vtable, so these land
    // at +0x08 / +0x10 like retail.
    virtual void setFlagA(u32 flag) {} // vtable +0x08
    virtual void v00C(u32 flag) {}
    virtual void setFlagB(u32 flag) {} // vtable +0x10
};

// Player actor layout: embedded battle sub-object at +0x3E9C.
struct BtlActorLayout {
    u8 _00[0x3E9C];
    BtlAttackView attackView;
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
                actor->attackView.setFlagA(0x100);
                actor->attackView.setFlagB(0x200);
            } else {
                actor->attackView.setFlagA(0x200);
                actor->attackView.setFlagB(0x100);
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

// Virtual dispatch view for the player's target-getter at vtable+0xAC.
class BtlGetTargetView {
public:
    virtual void v000() = 0;
    virtual void v001() = 0;
    virtual void v002() = 0;
    virtual void v003() = 0;
    virtual void v004() = 0;
    virtual void v005() = 0;
    virtual void v006() = 0;
    virtual void v007() = 0;
    virtual void v008() = 0;
    virtual void v009() = 0;
    virtual void v010() = 0;
    virtual void v011() = 0;
    virtual void v012() = 0;
    virtual void v013() = 0;
    virtual void v014() = 0;
    virtual void v015() = 0;
    virtual void v016() = 0;
    virtual void v017() = 0;
    virtual void v018() = 0;
    virtual void v019() = 0;
    virtual void v020() = 0;
    virtual void v021() = 0;
    virtual void v022() = 0;
    virtual void v023() = 0;
    virtual void v024() = 0;
    virtual void v025() = 0;
    virtual void v026() = 0;
    virtual void v027() = 0;
    virtual void v028() = 0;
    virtual void v029() = 0;
    virtual void v030() = 0;
    virtual void v031() = 0;
    virtual void v032() = 0;
    virtual void v033() = 0;
    virtual void v034() = 0;
    virtual void v035() = 0;
    virtual void v036() = 0;
    virtual void v037() = 0;
    virtual void v038() = 0;
    virtual void v039() = 0;
    virtual void v040() = 0;
    virtual void* getTarget() = 0; // vtable +0xAC
};

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
        // Virtual call at vtable+0xAC (slot #41 after MWCC's 2 hidden
        // __dt/__dl entries) on the player object.
        void* target = ((BtlGetTargetView*)player)->getTarget();
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
        bm->func_80085220(0x200, 1);
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
    if (func_8006EF04__Fi(0x10000000) != 0) {
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
            ((BtlSetTensionVt*)action)->vf304(level - 1);
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
                void** vtable = *(void***)action;
                typedef void (*SetTPFunc)(void*, f32);
                ((SetTPFunc)vtable[0x54])(action, (f32)(conv.d - cvtBias));
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
        void* actor = func_800B708C__Fi(*(u32*)((u8*)visionList + 0));
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
