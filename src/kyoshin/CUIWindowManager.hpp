#pragma once

#include "kyoshin/cf/IFlagEvent.hpp"
#include "monolib/util/reslist.hpp"
#include "monolib/work/CTTask.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "kyoshin/plugin/ocBdat.hpp"
#include <new>
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

class IUIWindow;
class IUIWindowSubView; // timer view behind IUIWindow::unk5C (defined in CUIWindowManager.cpp)
class CTalkWindow;
class CCol6Invite;

// Item-availability query row (0x2A bytes), indexed by arg1 in func_80140854.
struct CItemQueryRow {
    u8  field_0x00;      //+0x00
    u8  field_0x01;      //+0x01 - nonzero enables the flag-table lookup
    u16 field_0x02[4];   //+0x02 - per-arg2 entry type
    u16 field_0x0A[4];   //+0x0A - per-arg2 value
    u8  field_0x12[24];  //+0x12 - per-arg2 byte
}; //size 0x2A

// 0xC8-byte flag buffer (retail BSS at lbl_eu_80573C50): a build/active flag,
// per-table byte flags, and the quest/mission slot ids consumed by the
// flag-buffer helpers func_8013F3F0 / func_80140AFC in this unit.
struct CFlagBuffer {
    u8  field_0x00;              //0x00 - bit0: active/build flag
    u8  field_0x01[3];           //0x01-0x03
    u32 field_0x04;              //0x04 - accepted flag-memory result
    u8  field_0x08;              //0x08
    u8  field_0x09;              //0x09 - window-open guard
    u8  field_0x0A[2];           //0x0A-0x0B
    u8  field_0x0C[0x4C - 0x0C]; //0x0C-0x4B
    u32 field_0x4C;              //0x4C
    u16 field_0x50;              //0x50 - category id matched against arg2/arg4
    u16 field_0x52;              //0x52 - quest id
    u16 field_0x54;              //0x54
    u16 field_0x56;              //0x56
    u16 field_0x58;              //0x58 - slot 1 id
    u16 field_0x5A;              //0x5A - slot 2 id
    u16 field_0x5C;              //0x5C - slot 3 id
    u16 field_0x5E;              //0x5E - slot 3 gate value
    u16 field_0x60;              //0x60 - slot 4 bdat row index
    u16 field_0x62;              //0x62 - slot 5 bdat row index
    u16 field_0x64;              //0x64 - slot 6 id
    u8  field_0x66;              //0x66 - slot 6 expected flag-memory byte
    u8  field_0x67[0x68 - 0x67]; //0x67
    u16 field_0x68;              //0x68
    u16 field_0x6A;              //0x6A
    u8  field_0x6C[0x6E - 0x6C]; //0x6C-0x6D
    CItemQueryRow field_0x6E[2]; //0x6E,0x98 - per-item rows
    u8  field_0xC2[2];           //0xC2-0xC3
    u8* field_0xC4;              //0xC4 - per-item byte table
};

// Item-availability query table (retail func_80140854): two per-item rows at
// 0x6E/0x98 and a signed byte table behind field_0xC4.
struct CItemQuery {
    u8  field_0x00;               //0x00 - bit0: active flag
    u8  field_0x01[3];            //0x01-0x03
    u32 field_0x04;               //0x04 - item id
    u8  field_0x08[0x6E - 0x08];  //0x08-0x6D
    CItemQueryRow field_0x6E[2];  //0x6E,0x98 - per-item rows
    u8  field_0xC2[2];            //0xC2-0xC3
    u8* field_0xC4;               //0xC4 - per-item byte table
};

// Retail-unmangled window-factory imports (owning TUs: CCol6System /
// CSysWinSave / CMenuItemExchange / CMenuKizunaTalk / CSysWinBuff). Each
// creates a window under the manager's parent/scene and returns it (or 0).
// C linkage so call relocs bind to the literal retail names - a C++
// declaration would mangle them (e.g. func_8015DCD0FP8CProcessP4CScn) and
// drift at the reloc gate.
extern "C" IUIWindow* func_8015DCD0(CProcess* pParent, CScn* pScene);
extern "C" IUIWindow* func_801602F4(CProcess* pParent, CScn* pScene);
extern "C" IUIWindow* __ct__802944D8(CProcess* pParent, CScn* pScene);
extern "C" IUIWindow* func_801BEDE0(CProcess* pParent, CScn* pScene, u32 id);
extern "C" IUIWindow* func_801BCEBC(CProcess* pParent, CScn* pScene, u32 charId);
extern "C" IUIWindow* create__11CSysWinBuffFv(CProcess* pParent, CScn* pScene, u16 id);
extern "C" IUIWindow* func_8026F8B0(CProcess* pParent, CScn* pScene, u32 type, u32 id);
extern "C" IUIWindow* func_802AA2A0(CProcess* pParent, CScn* pScene, u8 id);
extern "C" IUIWindow* func_80125070(CProcess* pParent, CScn* pScene, u32 a2, u32 a3, u32 a4);
extern "C" IUIWindow* func_80144EE4(CProcess* pParent, CScn* pScene, u16 a3, u8 a4);

// C-ABI factory/guard imports used by the remaining window creators in this
// TU (retail unmangled symbols; owning TUs: CQuestWindow / CSystemWindow /
// CSimpleEveTalkWin / code_80135FDC). C linkage so call relocs bind to the
// literal retail names.
extern "C" {
IUIWindow* func_80122B2C(CProcess* pParent, CScn* pScene, u32 a2, u32 a3);
IUIWindow* func_801A20DC(CProcess* pParent, CScn* pScene, u32 text, const u8* msgSrc, u8 flag);
IUIWindow* func_80124AEC(CProcess* pParent, void* pSceneOrWin, u32 flag, u32 a3, u32 a4);
int func_80135694(u16 arg);
int func_801356BC();
int func_801356E0();
namespace nw4r { namespace lyt { class ArcResourceAccessor; } }
nw4r::lyt::ArcResourceAccessor* func_801355F4();
void* func_8003AA34();
u32 func_8003B1EC(void* fp);

// Quest-entry index lookup (owning TU: CUICfManager). Full-width arg/result
// (retail passes/returns the register unmasked).
u32 func_80138138(u32 idx);                 // quest id -> name-table index

// Multi-item entry guard (owning TU: CUICfManager): 5 u16 args, nonzero
// blocks the window creation.
int func_80135654(u16 a, u16 b, u16 c, u16 d, u16 e);

// Entry guard (owning TU: code_80135FDC family): nonzero blocks creation.
int func_80135610();
int func_80135630();

// Party-change-notice factory (owning TU: CMenuPTChangeNotice).
IUIWindow* func_80293B9C(CProcess* pParent, CScn* pScene);

// Quest-log menu factory (owning TU: CMenuQstCnt). The trailing entry bytes
// come from the caller's (masked) args plus a literal.
IUIWindow* func_802269D8(CProcess* pParent, CScn* pScene, u16 a3, u16 a4,
                          u8 a5, u8 a6, u8 a7);

// Quest-menu open guard (owning TU: CMenuQstCnt): nonzero blocks creation.
int func_80226B94();

// Shop window factory (owning TU: CMenuSelectShop).
IUIWindow* func_8018A58C(CProcess* pParent, CScn* pScene, u32 a4);

// CCol6Invite constructor (owning TU: CCol6System; unmangled retail symbol).
CCol6Invite* __ct__CCol6Invite(CCol6Invite* self, u16 arg2, u8 arg3, u8 arg4);

// Item-multi window factory (owning TU: CMenuGetItemMulti). 8 reg args + a
// byte flag on the stack (the caller forwards the 9th arg's low byte
// unmasked; declared u32 so no re-mask is emitted at the call site).
IUIWindow* func_801B46E4(CProcess* pParent, CScn* pScene, u32 a, u32 b, u32 c,
                          u32 d, u32 e, u32 f, u32 g);

// Related item-multi window factory (owning TU: CMenuGetItemMulti).
IUIWindow* func_801B4790(CProcess* pParent, CScn* pScene, u32 a2);

// Talk-window factory (owning TU: CTalkWindow) - creates the window under
// the caller's parent and registers it.
CTalkWindow* func_8012CC78(CProcess* parent, u32 arg1, u32 arg2,
                            const u8* msgSrc, u32 arg3, u32 arg4, u32 arg5);

// CMenuUpdate window factory (owning TU: kyoshin/menu/CMenuUpdate).
IUIWindow* func_80142B4C(CProcess* self, CScn* pScene, int r5, int r6, int r7,
                          int r8);

// Quest text lookup: returns a byte value for the entry/text/row.
u32 func_801361E8(u32 entry, const char* text, u32 row);

// Flag-buffer helpers (owning TU: this unit).
u8* func_80140AFC(u32 target);
void func_8013D26C(int mode);
int func_80140854(CItemQuery* self, u32 arg1, u32 arg2);
int func_8013F3F0(CFlagBuffer* flagBuf);
void func_8013FFF8(void* flagBuf, void* entry, u32 value);

// C-ABI imports used by the window-manager ctor and the flag-buffer helpers
// (owning TUs: CScn / CCol6System / CQuestWindow / CMenuGetItemMulti /
// CTalkWindow / code_8003B148 family). C linkage so call relocs bind to the
// literal retail names.
mtl::ALLOC_HANDLE func_80496004(void* ptr); // void* param: must match CUIBattleManager.hpp (10197)
void func_8015D0B8();
void func_80122460();
void func_801B29E0();
void func_8012BDD0();
u32 func_8009CF8C(u32 resourceId);
u32 func_80158068(u16 value);

// Flag-memory setter (owning TU outside this unit): writes `val` into the
// flag-memory slot for page id `idx`.
void func_8009D018(u32 idx, u32 val); // (u32,u32) form: must match CfGameManager.hpp:767 (10197)

// Consumable-count absorb helper (owning TU: kyoshin/cf/CItem; retail
// unmangled symbol).
void func_80159C04(unsigned int family, int count);

// CProcess::Regist (C-ABI import).
void Regist__8CProcessFP8CProcessb(CProcess* self, CProcess* parent,
                                   bool insertTop);

// array delete (C-ABI import).
void __dla__FPv(void* p);
}

// reslist_base vtable (retail data, other split). Sized >= 8 bytes so MWCC
// addresses it via lis/addi (retail form), not small-data SDA21.
extern const u8 lbl_eu_8052E61C[0x10];

// Presentation/event bitfield (.sbss, other splits) - talk-event bit gates
// the 0x608 window path of func_8013CBB4.

// Actor id -> actor object lookup (plain decl so the call reloc binds to the
// C++-mangled retail symbol findObjectById__Fi).
void* findObjectById(int id);

// C-ABI imports used by func_8013CBB4.
extern "C" {
void* getPlayer__Q22cf13CfGameManagerFi(int index);
void func_8009ECD0(u32 id);
u8 func_8013600C(const void* tbl, const void* key, u32 idx);
void func_8013B88C(u8 v);
}

// findObjectById result view used by the 0x608 window path: +0x64 flags word
// (bit 28 = show-talk-target) and +0x8C id handed to func_8009ECD0.
struct CActorFlagsView {
    u8 _00[0x64];
    u32 mFlags64;              //+0x64
    u8 _68[0x8C - 0x68];
    u16 mId8C;                 //+0x8C
};

// Global data used by func_801412D0 (retail BSS/rodata in other splits).
extern u8 lbl_eu_80573C50[0xC8];     // flag buffer (0xC8 bytes, byte flags)
extern void* lbl_eu_80573D18[0x1C];  // per-table entry pointers (filled at runtime)
extern const u8 lbl_804FC1D0[0x70];  // per-table base offsets (rodata)

// Flag-buffer builder (defined in this TU): zeroes the 0xC8-byte flag
// buffer, marks it active, refreshes per-table entry pointers, scans every
// table's rows for `row + baseOffset[i] == target`.
extern "C" u8* func_801412D0(u32 target);

// Pair of consecutive per-table base offsets; the flag-buffer builders copy
// the 28-entry offset table as 14 x 8-byte chunks.
struct CFlagOffsetPair {
    u32 field_0x00;
    u32 field_0x04;
};

// The full 28-entry per-table base-offset block (112 bytes, rodata at
// lbl_804FC1D0); copied wholesale by the flag-buffer builders.
struct CFlagOffsets {
    u32 v[28];
};

// Shared rodata string blobs (other splits).
extern char lbl_eu_8050097C[];  // quest window text (func_801361E8 source)
extern char lbl_eu_80500A50[];  // bdat column-name blob (getBdatStringColumnValue)

// bdat pointer used by the flag-buffer column reads (.sdata, other split).
extern u8* lbl_eu_80664098;
// rodata flag table (other split), byte-offset indexed by func_80140854.
extern u8 lbl_804FC260[];

// Per-entry time thresholds (rodata lbl_804FC240): two slots per entry,
// selected by the query slot id. Values gate per-entry updates in
// func_8013F6C4 and double as blocked flag-memory states.
struct CQuestTimeEntry {
    u32 v[2];
};
extern CQuestTimeEntry lbl_804FC240[4];

// bdat item/enum table handle (.sbss, other split); source of the column
// reads func_8013F6C4 issues after absorbing an entry.
extern void* lbl_eu_806640EC;

class CUIWindowManager : public CTTask<CUIWindowManager>, public cf::IFlagEvent{
public:
    // Retail instantiates CTTask<CUIWindowManager::CTest> too (34-char
    // mangling, same bodies as the 26-char one).
    class CTest : public CTTask<CTest> {
    public:
        ~CTest();
    };

    static CUIWindowManager* getInstance();
    static CUIWindowManager* create(CProcess* pParent, CScnNw4r* pScene, mtl::ALLOC_HANDLE mHandle);

    // Retail creator func_8013CFDC is an unmangled free function, so the ctor
    // must be reachable from it.
    CUIWindowManager(CScn* pScene, mtl::ALLOC_HANDLE mHandle);

    void Term();
    void Move();

public:
    //0x00-0x54 CTTask
    //0x54-0x58 cf::IFlagEvent
    CScn* unk58; //0x58 - scene passed to the window factories
    reslist<IUIWindow*> mWindowList1; //0x5C - primary window queue
    reslist<IUIWindow*> mWindowList2; //0x7C - secondary window queue
    IUIWindow* unk9C; //0x9C - child window flagged for removal on Term
    bool unkA0; //0xA0 - request: remove flagged/all windows in both queues
    bool unkA1; //0xA1 - request: force update-mark on all windows in both queues
    char unkA2[0xA4 - 0xA2]; //0xA2

    // The retail window creators func_8013DE6C / func_8013E9D8 / func_8013E204
    // / func_8013E52C / func_8013D7C0 are free functions (unmangled retail
    // symbols) that push newly created windows onto mWindowList1, so the data
    // members above are public for them. The singleton ctor/dtor stay private.

private:
    static CUIWindowManager* spInstance;
    virtual ~CUIWindowManager();
    void Init();

}; //size = 0xA4

class CTTask_CUIWindowManager {
public:
    virtual ~CTTask_CUIWindowManager();

    // TODO: add fields
};

