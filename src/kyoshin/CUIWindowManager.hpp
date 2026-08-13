#pragma once

#include "kyoshin/cf/IFlagEvent.hpp"
#include "monolib/util/reslist.hpp"
#include "monolib/work/CTTask.hpp"

class IUIWindow;
class CTalkWindow;

// Retail-unmangled window-factory imports (owning TUs: CCol6System /
// CSysWinSave / CMenuItemExchange / CMenuKizunaTalk / CSysWinBuff). Each
// creates a window under the manager's parent/scene and returns it (or 0).
// C linkage so call relocs bind to the literal retail names - a C++
// declaration would mangle them (e.g. func_8015DCD0FP8CProcessP4CScn) and
// drift at the reloc gate.
extern "C" IUIWindow* func_8015DCD0(CProcess* pParent, CScn* pScene);
extern "C" IUIWindow* __ct__802944D8(CProcess* pParent, CScn* pScene);
extern "C" IUIWindow* func_801BEDE0(CProcess* pParent, CScn* pScene, u32 id);
extern "C" IUIWindow* func_801BCEBC(CProcess* pParent, CScn* pScene, u32 charId);
extern "C" IUIWindow* create__11CSysWinBuffFv(CProcess* pParent, CScn* pScene, u16 id);

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
void* func_801355F4();
void func_8003AA34();
u32 func_8003B1EC(void* fp);

// Quest-entry index lookup (owning TU: CUICfManager). Full-width arg/result
// (retail passes/returns the register unmasked).
int func_80138138(int idx);

// Multi-item entry guard (owning TU: CUICfManager): 5 u16 args, nonzero
// blocks the window creation.
int func_80135654(u16 a, u16 b, u16 c, u16 d, u16 e);

// Item-multi window factory (owning TU: CMenuGetItemMulti). 8 reg args + a
// byte flag on the stack (retail forwards the 9th arg's low byte unmasked).
IUIWindow* func_801B46E4(CProcess* pParent, CScn* pScene, u32 a, u32 b, u32 c,
                          u32 d, u32 e, u32 f, u8 g);

// Talk-window factory (owning TU: CTalkWindow) - creates the window under
// the caller's parent and registers it.
CTalkWindow* func_8012CC78(CProcess* parent, u32 arg1, u32 arg2,
                            const u8* msgSrc, u32 arg3, u32 arg4, u32 arg5);

// bdat column read (owning TU: code_8003B148 family): returns the column
// value at the given row index.
u32 getBdatStringColumnValue(void* bdat, const char* col, int index);

// CMenuUpdate window factory (owning TU: kyoshin/menu/CMenuUpdate).
IUIWindow* func_80142B4C(CProcess* self, CScn* pScene, int r5, int r6, int r7,
                          int r8);

// Quest text lookup: returns a byte value for the entry/text/row.
u8 func_801361E8(u32 entry, const char* text, u32 row);

// Flag-buffer helpers (owning TU: this unit).
int func_8013F3F0(u8* flagBuf);
void func_8013FFF8(void* flagBuf, void* entry, u32 value);
}

// Global data used by func_801412D0 (retail BSS/rodata in other splits).
extern u8 lbl_eu_80573C50[0xC8];     // flag buffer (0xC8 bytes, byte flags)
extern void* lbl_eu_80573D18[0x1C];  // per-table entry pointers (filled at runtime)
extern const u8 lbl_804FC1D0[0x70];  // per-table base offsets (rodata)

// Shared rodata string blobs (other splits).
extern char lbl_eu_8050097C[];  // quest window text (func_801361E8 source)
extern char lbl_eu_80500A50[];  // bdat column-name blob (getBdatStringColumnValue)

class CUIWindowManager : public CTTask<CUIWindowManager>, public cf::IFlagEvent{
public:
    static CUIWindowManager* getInstance();
    static CUIWindowManager* create(CProcess* pParent, CScnNw4r* pScene, mtl::ALLOC_HANDLE mHandle);

    void Term();
    void Move();

    // Fork helper for presentation gating (coop::ShouldRenderSplitScreen).
    bool hasOpenWindows() const {
        return !mWindowList1.empty() || !mWindowList2.empty();
    }

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
    CUIWindowManager();
    virtual ~CUIWindowManager();
    void Init();

}; //size = 0xA4

class CTTask_CUIWindowManager {
public:
    virtual ~CTTask_CUIWindowManager();

    // TODO: add fields
};

