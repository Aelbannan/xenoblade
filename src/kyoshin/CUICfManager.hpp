#pragma once

#include "kyoshin/cf/IFlagEvent.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/work.hpp"

#include <nw4r/lyt.h>
#include <types.h>

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
// Move/draw PTMF table re-installed at the end of the ctor.
extern u32 lbl_eu_8052E288[3];
// CTTask vtable / CProcess vtable constants used by the ctor's base setup.
extern u8 lbl_eu_8052E2A0[];
extern u8 lbl_eu_8052E444[];

// --- imports for the free menu-create / probe helpers (CUICfManager.cpp) ---
// Signature choices match co-included headers (CTaskGame.hpp, CMenuQstCnt.hpp,
// CMainMenu.hpp) so TUs that include several of these headers stay legal.
extern u32 lbl_eu_80664184;      // scene-mode global (u8-narrowed reads)
extern const f32 lbl_eu_806672C8; // proximity float constant (.sdata2, r2-sda21)
extern "C" int func_800FF738();  // matches CTaskGame.hpp
extern "C" u32 func_801B481C();  // canonical u32 form (per CMenuGetItemMulti.cpp def / CVision.hpp)
extern "C" int func_80293C10();  // matches CMenuQstCnt.hpp (int)
extern "C" int func_8029A658();  // matches CMainMenu.hpp / CMenuQstCnt.hpp
void func_80138078(u32);         // mangles to func_80138078__FUl
class CSysWinBuff;
extern "C" CSysWinBuff* getInstance__11CSysWinBuffFv(); // matches CMainMenu.hpp
extern "C" u32 func_8009CF8C(u32 resourceId);      // resource getter (CMiniMap.cpp)
extern "C" u32 func_800FF6BC(u8* ctx, u32 scene);  // menu factory (CMainMenu.cpp)
extern "C" u32 func_80122450();                    // close-quest-menu gate (CMainMenu.hpp)
extern char lbl_8066DCF8[4];     // .sbss2 font-name blob (address used as pName)
// C-library delete operators (flat retail symbols; shared runtime helpers).
// Canonical void-return form - MUST match the ~100 other __dl__FPv decls
// (mixed void*/void spellings trip MWCC 10505 when headers co-occur).
extern "C" void __dl__FPv(void* p);
extern "C" void __dla__FPv(void* p);
// C-library vararg formatter (flat retail symbol; used by the slot sprintf
// fills). Matches the per-TU declaration used by CMapSel.hpp etc.
extern "C" int sprintf(char* str, const char* fmt, ...);
// Memory-handle getter for the ctor's node-pool allocation (CUIWindowManager.hpp).
class CScn;
extern "C" mtl::ALLOC_HANDLE func_80496004(void* ptr); // void* param: must match CUIBattleManager.hpp (10197)

// This unit's own slot helpers (flat retail names; bodies live in the .cpp).
extern "C" int func_801359AC(u8* singleton);
extern "C" int func_8013042C(u8* base, u8 index);
extern "C" void func_801311B8(u8* base);          // slot-rebuild helper (this unit)
extern "C" void func_80131820(u8* base);          // slot re-sort helper (this unit)
extern "C" void func_8012FFB4(u8* base);          // slot dispatch + rebuild (this unit)

// --- flat imports for the free-menu helpers (func_8012FFB4 family) ---
// Caller-shape CTaskGame imports (the full CTaskGame.hpp conflicts with this
// TU's local flat decls for func_8049603C / lbl_eu_80663E14; declared with
// the verbatim retail names per the CLibCri caller-shape scheme in CTaskGame.hpp).
class CTaskGame;
extern "C" CTaskGame* getInstance__9CTaskGameFv();
extern "C" bool isFlag01Set__9CTaskGameFv(CTaskGame* self);
// Sound/battle-state gates (member-shaped C-ABI imports; CfSoundMan.hpp and
// CMainMenu.hpp declare the same flat names but pull nw4r/snd.h / CMainMenu
// classes into this TU).
extern "C" bool isSceneActive__Q22cf13CfGameManagerFv();
extern "C" int isAnyFieldFlagSet__Q22cf13CfGameManagerFv();
extern "C" int func_8011CD5C();
extern "C" bool CMenuArtsSelect_isCreated();
// Window/IUI helpers (flat retail names; defining TU is CUIWindowManager.cpp).
extern "C" void func_8013E8E0(int mode);
extern "C" void func_8013E424(u8* str, int flags);
extern "C" void func_8013E2E0(u32, u32, u32, u32, u32, u32, u32, u32, u32);
extern "C" void func_8013D55C(char* msg, int a, int b);
extern "C" void func_8013D688(char* a, char* b, int c, int d);
// Enum-list holder helpers for func_801359AC (defining TU: code_80043E08 TU).
// func_80043E08 takes the holder itself (declared in the .cpp next to the
// CUICfEnumListHolder type).
extern "C" u8* __ct__800FC32C(u8* list, int a, int b, int c);
// .sdata2 float constants (r2-sda21 loads; const routes them into .sdata2).
extern const f32 lbl_eu_806672C8;      // proximity constant
extern const f32 lbl_eu_806672D0;      // player-pose constant
// .sdata slot-list format strings (4-byte entries, sda21 li loads).
extern char lbl_eu_806621EC[4];
extern char lbl_eu_806621E8[4];

// Read-only queue-helper view over the lbl_eu_80664054 singleton: factory
// ctx at +0x144, scene id at +0x11C, reslist head at +0x128, free-slot array
// at +0x138 with count at +0x13C (used by the func_80133CA0 family).
struct CUICfQueueMgrView {
    u8 pad0[0x11C];
    u32 field_11C;
    u8 pad1[0x128 - 0x120];
    void* field_128;             // reslist head node
    u8 pad2[0x138 - 0x12C];
    void* field_138;             // free-slot array base (0xC stride)
    int field_13C;               // slot capacity
    u8 pad3[0x144 - 0x140];
    u32 field_144;               // menu-factory ctx
};

// 0x168-stride view over mInitSlots[].unk04 (func_80135898's flag scan).
struct CUICfSlotFlagView {
    u8 flag;                     // mInitSlots[i].unk04 (0x14c + i*0x168)
    u8 pad[0x168 - 0x01];
};

// Manager view offsetting the slot-flag array to +0x14C (avoids raw pointer
// arithmetic at func_801359AC's flag==4 scan).
struct CUICfManagerSlotView {
    u8 pad[0x14C];
    CUICfSlotFlagView slots[8];  // +0x14C, 0x168 stride
};

// 0x34-byte block used by the slot-swap records (same layout as
// CUICfInitBlock; field_0xNN names avoid the repo lint's unkN rule).
struct CUICfSwapBlock {
    u32 field_0x00;
    u16 field_0x04;
    u8 field_0x06[0x34 - 6];
};

// Zero-view over CUICfSwapBlock for the swap clears (names the two zeroed
// fields; the block copies go through func_8015704C with CUICfInitBlock*).
struct CUICfBlockZeroView {
    u32 field_0x00;
    u16 field_0x04;
};

// 0x40-byte string buffer inside a swap record (struct assignment makes
// MWCC emit the retail lwz/stw word-pair copies).
struct CUICfSwapStr {
    char b[0x40];
};

// Player-wrapper view for func_801359AC's pose/state checks (CfObjectMove is
// at +0x3E9C inside the wrapper; +0x3F60 points at the battle object).
struct CUICfPlayerView {
    u8 pad[0x3F60];
    u8* field_3F60;              // +0x3F60
};

// Battle-object view for func_801359AC's flag checks (+0x530 halfword bit 0,
// +0x4EC word bits 1/8/15).
struct CUICfObj3F60View {
    u8 pad[0x4EC];
    u32 field_4EC;               // +0x4EC
    u8 pad2[0x530 - 0x4F0];
    u16 field_530;               // +0x530
};

// CBattleManager's battle-object circular list (head pointer at +0x8; nodes
// are single-linked {next@0}).
struct CUICfListLink {
    CUICfListLink* next;         // +0x0
};
struct CBattleListHead {
    u8 pad[0x8];
    CUICfListLink* head;         // +0x8
};

// Enum-list view for func_801359AC's population check (count at +0x620).
struct CUICfEnumList620 {
    u8 pad[0x620];
    u32 count;                   // +0x620
};

// View returned by func_8049603C (f32 at +0/+4/+8/+C).
struct CUICfCamView {
    f32 field_0;
    f32 field_4;
    f32 field_8;
    f32 field_C;
};

// Layout view of the object returned by getUnk80664658 (flag word at 0x214).
struct CUICfGlobalSettings {
    u8 field_0x00[0x214];
    u32 field_214;               // 0x214 - flag bits
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

// Full slot record used by the func_801311B8/func_80131820 swap helpers
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

// Dispatch view over one init-slot region for func_8012FFB4 (base =
// &mInitSlots[0].unk04). Flag/state bytes at +0/+1, block data at +4,
// five u16 ids at +0xD8..+0xE0, u16 id at +0xE2, u8 state at +0xE4, and the
// two sprintf buffers at +0xE6 / +0x126.
struct CUICfSlotDispView {
    u8 field_0x00;               // +0x00 slot flag
    u8 field_0x01;               // +0x01 slot state
    u8 pad[0x04 - 0x02];
    CUICfInitBlock block_04;     // +0x04
    u8 pad2[0xD8 - 0x38];
    u16 field_D8;                // +0xD8
    u16 field_DA;                // +0xDA
    u16 field_DC;                // +0xDC
    u16 field_DE;                // +0xDE
    u16 field_E0;                // +0xE0
    u16 field_E2;                // +0xE2
    u8 field_E4;                 // +0xE4
    u8 pad3;                     // +0xE5
    char str_E6[0x40];           // +0xE6
    char str_126[0x80];          // +0x126
};

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

// 0xC-byte pool node for func_80133324's event queue - same layout as
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

// Public-layout mirror of the CUICfManager fields the free menu-create
// helpers (func_80133A08 family) touch; the real fields are private.
struct CUICfManagerCreateView {
    u8 field_0x00[0x5C];
    void* field_0x5C;              // 0x5C - resource accessor (mArcResourceAccessor)
    u8 field_0x60[0x11C - 0x60];
    u32 field_0x11C;               // 0x11C - owning scene
    int field_0x120;               // 0x120 - Move countdown (signed)
    u8 field_0x124[0x128 - 0x124];
    CUICfListNode* field_0x128;    // 0x128 - event queue head
    u8 field_0x12C[0x138 - 0x12C];
    CUICfListNode* field_0x138;    // 0x138 - event node array
    int field_0x13C;               // 0x13C - node array count
    u8 field_0x140[0x144 - 0x140];
    CUICfUnk144* field_0x144;      // 0x144 - menu context (CProcess-ish)
    u8 field_0x148[0xC90 - 0x148];
    u16 field_0xC90;               // 0xC90 - Move bitflags (mFlags)
    u8 field_0xC92[0xC94 - 0xC92];
};

// 27-entry, 0-terminated id table copied onto the stack by func_80133324
// (retail: sp+0x28..0x5D, matches lbl_eu_804FFFDC minus its trailing entry).
struct CUICfIdTable {
    u16 ids[27];
};

class CUICfManager;

// Retail flat (unmangled) ctor symbol; the placement-new shim in the .cpp.
extern "C" void* __ct__CUICfManager(CUICfManager* self, class CScnNw4r* pScene,
                                    unsigned long mHandle);

// The body reads r4/r5/r6 as real event-dispatch arguments and never touches
// `self` (all state comes from the lbl_eu_80664054 singleton); declared
// extern "C" here (before the class, so the in-class friend declaration
// below binds to this same linkage) so its ABI is r3=<unused self>, r4=id,
// r5=a1, r6=a2. Named with the retail mangling (`Fiii`).
extern "C" void func_80133324__12CUICfManagerFiii(CUICfManager* self, int id, int a1, int a2);

// Same hidden-arg ABI: the retail OnFileEvent symbol is a no-arg member
// (`Fv`) but the body reads r4 as the CEventFile* from the IWorkEvent
// dispatcher (see the onFileEventThunk thunk). Declared extern "C" before the
// class so the in-class friend binds to this same C linkage.
extern "C" bool OnFileEvent__12CUICfManagerFv(CUICfManager* self, CEventFile* evt);

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

// Write view over one init-slot region for func_80130B74's slot fill
// (flag byte at +0, u16 id at +0xE2, u8 state at +0xE4).
struct CUICfSlotWriteView {
    u8 field_0x00;             // +0x00 slot flag (4 = in use)
    u8 pad[0xE2 - 0x01];
    u16 field_0xE2;            // +0xE2
    u8 field_0xE4;             // +0xE4
};

// Write view over one init-slot region for func_80130960's 5-id claim
// (flag byte at +0, five u16 ids at +0xD8..+0xE0).
struct CUICfSlotIdWriteView {
    u8 field_0x00;             // +0x00 slot flag (2 = claiming)
    u8 pad[0xD8 - 0x01];
    u16 field_D8;              // +0xD8
    u16 field_DA;              // +0xDA
    u16 field_DC;              // +0xDC
    u16 field_DE;              // +0xDE
    u16 field_E0;              // +0xE0
};

// Write view over one init-slot region for func_80130D80/func_80130F98's
// sprintf fills (flag byte at +0, char buffer at +0xE6).
struct CUICfSlotStrView {
    u8 field_0x00;             // +0x00 slot flag
    u8 pad[0xE6 - 0x01];
    char str[0x80];            // +0xE6 sprintf target
};

// 0x168-stride slot write view for func_80130720's block-copy claim
// (flag byte at +0, four 0x34-byte blocks at +4/+0x38/+0x6C/+0xA0,
// u32 at +0xD4).
struct CUICfSlotCopyView {
    u8 field_0x00;                 // +0x00 slot flag (3 = claiming)
    u8 pad[0x04 - 0x01];
    CUICfInitBlock block1;         // +0x04
    CUICfInitBlock block2;         // +0x38
    CUICfInitBlock block3;         // +0x6C
    CUICfInitBlock block4;         // +0xA0
    u32 field_D4;                  // +0xD4
};

// Source-side view of the template arg for func_80130720's block copy.
struct CUICfSrcCopyView {
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
    };

    static CUICfManager* getInstance() {
        return spInstance;
    }
    static CUICfManager* create(CProcess* pParent, CScnNw4r* pScene, mtl::ALLOC_HANDLE mHandle);
    static nw4r::lyt::ArcResourceAccessor* func_801355F4();
    static int func_80135FDC();

    void Init();
    void Term();
    void Move();

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
    void* getPackedFont9C() { return &mPackedFont9C; }
    void* getPackedFontD8() { return &mPackedFontD8; }
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
    // 0x000-0x054 CTTask
    // 0x054-0x058 IWorkEvent
    // 0x058-0x05C cf::IFlagEvent
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

    static CUICfManager* spInstance;
    CUICfManager(CScnNw4r* pScene, mtl::ALLOC_HANDLE mHandle);
    virtual ~CUICfManager();
    void OnFileEvent();
    void onFileEventThunk();
    void dtorThunk54();
    // Retail symbol mangles `Fv` but the vtable thunk passes its three hidden
    // args (r4/r5/r6) through to func_80133324 after adjusting `this`; body
    // is emitted under the extern "C" flat name (see .cpp).
    void eventDispatchThunk();
    void dtorThunk58();
    void func_80133324(int id, int a1, int a2);
    IWorkEvent* cfWorkEvent();

    // The retail OnFileEvent body is emitted under this extern "C" free
    // function (Fv name + real CEventFile* arg); grant it member access.
    friend bool OnFileEvent__12CUICfManagerFv(CUICfManager* self, CEventFile* evt);
}; // size = 0xC94
