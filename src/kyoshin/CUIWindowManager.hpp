#pragma once

#include "kyoshin/cf/IFlagEvent.hpp"
#include "monolib/util/reslist.hpp"
#include "monolib/work/CTTask.hpp"

class IUIWindow;

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

