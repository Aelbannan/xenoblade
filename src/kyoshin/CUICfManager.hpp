#pragma once

#include "kyoshin/cf/IFlagEvent.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/work.hpp"

#include <nw4r/lyt.h>
#include <types.h>
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/CUICfManager_Res.hpp"

namespace nw4r {
namespace ut {

// Stub: retail size 0x3C; Destroy returns buffer for MemManager::deallocate.
// ~PackedFont is declared (defined in nw4r's ut_PackedFont TU) so the
// CUICfManager dtor emits the retail member-destruction calls.
class PackedFont {
public:
    // Declared-only: the retail CUICfManager ctor emits the member
    // destruction... construction calls to __ct__Q34nw4r2ut10PackedFontFv;
    // the body lives in nw4r's ut_PackedFont TU.
    PackedFont();
    void* Destroy();
    ~PackedFont();
    bool Construct(void* pBuffer, u32 bufferSize, const void* pArchive,
                   const char* pName);
    static u32 GetRequireBufferSize(const void* pResource, const char* pName,
                                    f32 scale);

private:
    u8 unk[0x3C];
};

} // namespace ut
} // namespace nw4r

// reslist<...>-shaped event queue vtable (retail .data, 0xC bytes). Restored
// by the queue clear in CUICfManager::~CUICfManager.
extern u8 lbl_eu_8052E3B0[];
// Derived event-queue vtable (retail .data) installed by the ctor after the
// reslist base vtable.
extern u8 lbl_eu_8052E398[];
// CTTask vtable / CProcess vtable constants used by the ctor's base setup.
extern u8 lbl_eu_8052E2A0[];
extern u8 lbl_eu_8052E444[];

// --- imports for the free menu-create / probe helpers (CUICfManager.cpp) ---
// Signature choices match co-included headers (CTaskGame.hpp, CMenuQstCnt.hpp,
// CMainMenu.hpp) so TUs that include several of these headers stay legal.
class IUIWindow;

extern "C" int CMainMenu_IsOpen();  // matches CTaskGame.hpp
extern "C" u32 GetItemMulti_IsActiveFlag();  // canonical u32 form (per CMenuGetItemMulti.cpp def / CVision.hpp)
extern "C" u32 PTNotice_IsActive_3C10();  // definition: CMenuPTChangeNotice.cpp
extern "C" u32 MenuTutorialIsCreated();  // definition: CMenuTutorial.cpp (u32)
class CSysWinBuff;
extern "C" CSysWinBuff* getInstance__11CSysWinBuffFv(); // matches CMainMenu.hpp
#include "kyoshin/cf/CtrlRemoteApi.hpp"      // resource getter (CMiniMap.cpp)
extern "C" bool hasQuestWindow();                    // close-quest-menu gate (CQuestWindow.cpp)
extern char lbl_8066DCF8[4];     // .sbss2 font-name blob (address used as pName)
// C-library delete operators (flat retail symbols; shared runtime helpers).
// Canonical void-return form - MUST match the ~100 other __dl__FPv decls
// (mixed void*/void spellings trip MWCC 10505 when headers co-occur).
// C-library vararg formatter (flat retail symbol; used by the slot sprintf
// fills). Matches the per-TU declaration used by CMapSel.hpp etc.
extern "C" int sprintf(char* str, const char* fmt, ...);
// Memory-handle getter for the ctor's node-pool allocation (CUIWindowManager.hpp).
class CScn;
extern "C" mtl::ALLOC_HANDLE Scn_CallUnk8C_V10(void* ptr); // void* param: must match CUIBattleManager.hpp (10197)

// This unit's own slot helpers (flat retail names; bodies live in the .cpp).
extern "C" int CUICfManager_checkMenuOpenGate(u8* singleton);
extern "C" int CUICfManager_claimFreeSlotByIndex(u8* base, u8 index);
extern "C" void CUICfManager_sortSlotsByFlag(u8* base);          // slot-rebuild helper (this unit)
extern "C" void CUICfManager_sortSlotList(u8* base);          // slot re-sort helper (this unit)
extern "C" void CUICfManager_dispatchFreeSlot(u8* base);          // slot dispatch + rebuild (this unit)
extern "C" int CUICfManager_prepareMenus(); // flat helper using the real manager fields
// --- flat imports for the free-menu helpers (CUICfManager_dispatchFreeSlot family) ---
// Caller-shape CTaskGame imports (the full CTaskGame.hpp conflicts with this
// TU's local flat decls for Scn_QueryUnk80State / gmScene; declared with
// the verbatim retail names per the CLibCri caller-shape scheme in CTaskGame.hpp).
class CTaskGame;
// Sound/battle-state gates (member-shaped C-ABI imports; CfSoundMan.hpp and
// CMainMenu.hpp declare the same flat names but pull nw4r/snd.h / CMainMenu
// classes into this TU).
extern "C" bool isSceneActive__Q22cf13CfGameManagerFv();
extern "C" int isAnyFieldFlagSet__Q22cf13CfGameManagerFv();
extern "C" int isQuestLogMenuActive();
extern "C" bool CMenuArtsSelect_isCreated();
// Window/IUI helpers (flat retail names; defining TU is CUIWindowManager.cpp).
extern "C" IUIWindow* UIWin_CreatePTChange(u32 flag);
extern "C" IUIWindow* UIWin_CreateB4790Win(u32 arg1, u32 arg2);
extern "C" IUIWindow* UIWin_CreateItemMulti(u32, u32, u32, u32, u32, u32, u32, u32, u8);
extern "C" IUIWindow* UIWin_CreateSysWin0(const char* str, void* arg1, u32 arg2);
extern "C" IUIWindow* UIWin_CreateSysWin1(u32 arg0, void* arg1, void* arg2, u32 arg3);
// Enum-list holder helpers for CUICfManager_checkMenuOpenGate (defining TU: code_80043E08 TU).
// __ct__16CfEnumListHolderFUlUl takes the holder itself (declared in the .cpp next to the
// CUICfEnumListHolder type).
extern "C" u8* __ct__800FC32C(u8* list, int a, int b, int c);
// .sdata slot-list format strings (4-byte entries, sda21 li loads).
extern char lbl_eu_806621EC[4];
extern char lbl_eu_806621E8[4];

// 0x168-stride slot-flag array at manager +0x14C. RETAIL SHAPE: MWCC needs
// the 0x168-stride element type to reproduce the unrolled li/mulli/lbzx scan
// in CUICfManager_hasInUseSlot (a u8 pointer folds to direct lbz offsets).
struct CUICfSlotFlagView {
    u8 flag;                     // mInitSlots[i].unk04 (0x14c + i*0x168)
    u8 pad[0x168 - 0x01];
};

struct CUICfManagerSlotView {
    u8 pad[0x14C];
    CUICfSlotFlagView slots[8];  // +0x14C, 0x168 stride
};

// 0x40-byte string buffer inside a swap record (struct assignment makes

struct CUICfSwapStr {
    char b[0x40];
};

// CUICfManager_checkMenuOpenGate now reaches both actors through real
// classes. The player's embedded CfObjectMove is at actor +0x3E9C; the
// canonical static_cast in CfActorMoveLayout.hpp emits that same subi.
// CfObjectMove::mTargetC4 is the +0x3F60 battle-object pointer, and the
// pointee is a real cf::CActParamAnimGame (declared in CActParamAnimGame.hpp):
//   field530 (+0x530) bit 0, field4EC (+0x4EC) bits 1/8/15.
// The two former overlay structs duplicated exactly those three offsets
// and were removed without changing any call-site instruction.
// The vtable slot-0x128 call still takes the actor-base pointer, which
// the layout cast recovers from getPlayer(0)'s CfObjectMove*.
// Keeping these declarations as comments preserves -ipa-file line numbers
// for the other matched functions in this TU.
// No data members were lost: the overlays carried no objects of their own.
// CfActorMoveLayout's pad reproduces the actor's CAIAction trailer.
// The flag meanings are documented on CActParamAnimGame's fields.

// CBattleManager's battle-object circular list (head pointer at +0x8; nodes
// are single-linked {next@0}).
struct CUICfListLink {
    CUICfListLink* next;         // +0x0
};
struct CBattleListHead {
    u8 pad[0x8];
    CUICfListLink* head;         // +0x8
};

// Enum-list view for CUICfManager_checkMenuOpenGate's population check (count at +0x620).
struct CUICfEnumList620 {
    u8 pad[0x620];
    u32 count;                   // +0x620
};

// View returned by Scn_QueryUnk80State (f32 at +0/+4/+8/+C).
struct CUICfCamView {
    f32 field_0;
    f32 field_4;
    f32 field_8;
    f32 field_C;
};


struct CUICfUnk144 {
    u8 unk00[0x39];
    u8 unk39; // 0x39
};

// Queue item type for CUICfManager::Move list walks (CProcess-sized prefix).
struct CUICfMenuItem {
    u8 unk00[0x39];
    u8 unk39; // 0x39 - remove / SetRemove
    u8 unk3A[0x54 - 0x3A];
    u8 unk54; // 0x54
    u8 unk55; // 0x55
};

struct CUICfInitBlock {
    u32 unk00;
    u16 unk04;
    u8 unk06[0x34 - 6];
};

// Full slot record used by the CUICfManager_sortSlotsByFlag/CUICfManager_sortSlotList swap helpers
// (same 0x168 layout as the init slots at base+i*0x168).
struct CUICfSwapSlot {
    u8 field_0;                  // +0x00 flag
    u8 field_1;                  // +0x01 state
    u8 pad[2];
    CUICfInitBlock block1;       // +0x04
    CUICfInitBlock block2;       // +0x38
    CUICfInitBlock block3;       // +0x6C
    CUICfInitBlock block4;       // +0xA0
    u32 wordD4;                  // +0xD4
    u16 ids[6];                  // +0xD8..+0xE2
    u8 byteE4;                   // +0xE4
    CUICfSwapStr strE6;          // +0xE6
    CUICfSwapStr str126;         // +0x126
}; // size 0x166

struct CUICfInitState {
    u8 mode;
    u8 state;
    u8 unk02[2];
};

// Retail copy: lwz +0; paired +8/+4; lhz +0xC; lone lwz +0x0E; paired words from +0x12.
// Trailing bytes split so MWCC pair-unrolls (one big u8[] → lwzu). Zeros via u16* overlay.
#pragma pack(push, 1)
struct CUICfInitTailChunk8 {
    u8 b[8];
};
struct CUICfInitTailChunk40 {
    u8 b[0x40];
};
struct CUICfInitTailChunk3E {
    u8 b[0x3E];
};
struct CUICfInitTailChunk40View {
    u8 b[0x40]; // assign view: 2-byte overhang past Tail (retail last lwz pair)
};
struct CUICfInitTail {
    u32 unk00;                  // +0x00
    CUICfInitTailChunk8 mid;    // +0x04..+0x0B (memcpy pair → +8/+4 loads)
    u16 unk0C;                  // +0x0C
    u32 unk0E;                  // +0x0E
    CUICfInitTailChunk40 rest0; // +0x12
    CUICfInitTailChunk3E rest1; // +0x52
}; // size = 0x90
#pragma pack(pop)

// Forces retail stack contiguity: state, block0, blocks[3], tail.
struct CUICfInitTemplates {
    CUICfInitState state;
    CUICfInitBlock block0;
    CUICfInitBlock blocks[3];
    CUICfInitTail tail;
};

// 0xC-byte pool node for eventDispatch's event queue - same layout as
// `_reslist_node<u32>` (mNext@0, mPrev@4, mItem@8). Empty slots have mNext==0.
struct CUICfListNode {
    CUICfListNode* next; // 0x0
    CUICfListNode* prev; // 0x4
    u32 item;            // 0x8
};

// reslist<...>-shaped event queue view at CUICfManager +0x124 (same layout as
// ResListIUIBattle: vtable, sentinel-head ptr, embedded sentinel node, slot
// array, capacity, owns-flag). The dtor inlines the reslist clear+free.
struct CUICfEventQueue {
    u8* vtable;             // +0x00 (0x124)
    CUICfListNode* head;    // +0x04 (0x128)
    CUICfListNode startNode;// +0x08 (0x12C)
    CUICfListNode* nodes;   // +0x14 (0x138)
    int count;              // +0x18 (0x13C)
    u8 ownsList;            // +0x1C (0x140)
};

// 27-entry, 0-terminated id table copied onto the stack by eventDispatch
// (retail: sp+0x28..0x5D, matches lbl_eu_804FFFDC minus its trailing entry).
struct CUICfIdTable {
    u16 ids[27];
};

class CUICfManager;

// Retail flat (unmangled) ctor symbol; the placement-new shim in the .cpp.
extern "C" void* __ct__CUICfManager(CUICfManager* self, class CScnNw4r* pScene,
                                    unsigned long mHandle);

// Same hidden-arg ABI: the retail OnFileEvent symbol is a no-arg member
// (`Fv`) but the body reads r4 as the CEventFile* from the IWorkEvent
// dispatcher (see the onFileEventThunk thunk). Declared extern "C" before the
// class so the in-class friend binds to this same C linkage.
extern "C" bool OnFileEvent__12CUICfManagerFv(CUICfManager* self, CEventFile* evt);

// Flat retail helper that fills the CTTask move-function words; it needs
// member access to the protected CTTask base storage.
extern "C" void CUICfManager_initEventTriplet(CUICfManager* self);

struct CUICfInitSlot {
    u8 unk00[4];
    u8 unk04;
    u8 unk05;
    u8 unk06[2];
    CUICfInitBlock unk08;
    CUICfInitBlock unk3C;
    CUICfInitBlock unk70;
    CUICfInitBlock unkA4;
    CUICfInitTail unkD8;
}; // size = 0x168


struct CUICfSlotTemplate {
    CUICfInitBlock block1;         // +0x00
    CUICfInitBlock block2;         // +0x34
    CUICfInitBlock block3;         // +0x68
    CUICfInitBlock block4;         // +0x9C
    u32 field_D0;                  // +0xD0
};

// novtable: retail's dtor emits NO class-vtable store (the queue's
// lbl_eu_8052E3B0 store is explicit); without it MWCC emits the
// lis/addi/stw __vt__12CUICfManager reset (+3 insns) before the body.
class __declspec(novtable) CUICfManager : public CTTask<CUICfManager>, public IWorkEvent, public cf::IFlagEvent {
public:
    // Retail instantiates CTTask<CUICfManager::CTest> too (Move/Draw/dtor
    // at the 30-char mangling); the bodies match CTTask<CUICfManager>.
    class CTest : public CTTask<CTest> {
    public:
        ~CTest();
        void Init();
        void Term();
        void Move();
        void Draw();
    };

    static CUICfManager* spInstance;

    static CUICfManager* getInstance() {
        return spInstance;
    }
    static CUICfManager* create(CProcess* pParent, CScnNw4r* pScene, mtl::ALLOC_HANDLE mHandle);

    void Init();
    void Term();
    void Move();
    void eventDispatch(int id, int a1, int a2);

    void setTimeout30() { unk120 = 30; }
    void setFlagState(bool value) {
        if (value) {
            mInitSlots[0].unk00[1] = 1;
        } else {
            mInitSlots[0].unk00[0] = 1;
        }
        mFlags = 0;
    }
    void* getArcResourceAccessor() const {
        return mArcResourceAccessor;
    }
    void setFieldC8C(u8 value) {
        unkC88[4] = value;
    }
    nw4r::ut::PackedFont* getPackedFont60() { return &mPackedFont60; }
    nw4r::ut::PackedFont* getPackedFont9C() { return &mPackedFont9C; }
    nw4r::ut::PackedFont* getPackedFontD8() { return &mPackedFontD8; }
    int prepareMenus() {
        if (mArcResourceAccessor == 0) {
            mFlags |= 0x4;
            return 0;
        }
        mFlags &= 0xfffb;
        mFlags |= 0x8 | 0x10 | 0x20 | 0x40 | 0x80;
        return 0;
    }

private:
    // placement-new ctor shim (retail flat symbol) in the .cpp
    friend void* __ct__CUICfManager(CUICfManager*, CScnNw4r*,
                                    mtl::ALLOC_HANDLE);
    friend int CUICfManager_prepareMenus();
    friend void CUICfManager_initEventTriplet(CUICfManager*);

    // 0x054-0x058 IWorkEvent
    // 0x058-0x05C cf::IFlagEvent
public:
    nw4r::lyt::ArcResourceAccessor* mArcResourceAccessor; // 0x05C
    nw4r::ut::PackedFont mPackedFont60;                  // 0x060
    nw4r::ut::PackedFont mPackedFont9C;                  // 0x09C
    nw4r::ut::PackedFont mPackedFontD8;                  // 0x0D8
    CFileHandle* mFileHandle;                            // 0x114
    int unk118;                                          // 0x118
    u32 unk11C;                                          // 0x11C
    u32 unk120;                                          // 0x120 - Move countdown
    u8 unk124[0x128 - 0x124];                            // 0x124
    CUICfListNode* unk128;                               // 0x128 (event queue head)
    u8 unk12C[0x138 - 0x12C];                            // 0x12C
    CUICfListNode* unk138;                               // 0x138 (event node array)
    int unk13C;                                          // 0x13C (event node array count)
    u8 unk140[0x144 - 0x140];                            // 0x140
    CUICfUnk144* unk144;                                 // 0x144
    CUICfInitSlot mInitSlots[8];                         // 0x148
    // Slot0.unk00[0]/[1] are manager-wide clear/mark flags (0x148/0x149).
    u8 unkC88[8];                                        // 0xC88
    u16 mFlags;                                          // 0xC90 - Move bitflags
    u8 unkC92[2];                                        // 0xC92

private:
    CUICfManager(CScnNw4r* pScene, mtl::ALLOC_HANDLE mHandle);
    virtual ~CUICfManager();
    void OnFileEvent();
    void onFileEventThunk();
    void dtorThunk54();
    // Retail symbol mangles `Fv` but the vtable thunk passes its three hidden
    // args (r4/r5/r6) through to eventDispatch after adjusting `this`; body
    // is emitted under the extern "C" flat name (see .cpp).
    void eventDispatchThunk();
    void dtorThunk58();
    IWorkEvent* cfWorkEvent();

    // The retail OnFileEvent body is emitted under this extern "C" free
    // function (Fv name + real CEventFile* arg); grant it member access.
    friend bool OnFileEvent__12CUICfManagerFv(CUICfManager* self, CEventFile* evt);
}; // size = 0xC94

// Canonical C++ declarations for this unit's exported free functions.
u32 CUICfManager_queueQuestLogMenu(u32 arg);
u32 CUICfManager_queueBaseMenuItem();
u32 CUICfManager_queueFactoryMenu(u32 param, f32 value);
u32 CUICfManager_queueShopBuyMenu(u32 param);
u32 CUICfManager_queueShopSellMenu();
u32 CUICfManager_queuePTStateMenu();
u32 CUICfManager_queueMakeCrystalMenu();
u32 CUICfManager_queueArtsSetMenu();
u32 CUICfManager_queueMapSelectMenu();
u32 CUICfManager_queueWorldMapMenu();
u32 CUICfManager_queuePauseMenu();
u32 CUICfManager_queueCollepediaMenu();
u32 CUICfManager_queueKizunagramMenu(u32 arg);
u32 CUICfManager_queuePassiveSkillMenu();
u32 CUICfManager_queuePlayAwardMenu();
u32 CUICfManager_queueKizunaTalkMenu();
u32 CUICfManager_queueSaveMenu(u32 arg0, u32 arg1);
u32 CUICfManager_queueOptionMenu(u32 arg);
u32 CUICfManager_queueTutorialListMenu(u32 arg);
u32 CUICfManager_queueGameClearMenu();
u32 CUICfManager_queueGetItemMenuNarrowed(u32 arg);
u32 CUICfManager_queueGetItemMenuById(u32 arg);
u32 CUICfManager_queueMultiGetItemMenu(u32 arg);
int CUICfManager_tryResetSlots();
