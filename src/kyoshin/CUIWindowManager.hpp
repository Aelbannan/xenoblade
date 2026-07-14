#pragma once

#include "kyoshin/cf/IFlagEvent.hpp"
#include "monolib/util/reslist.hpp"
#include "monolib/work/CTTask.hpp"

class IUIWindow;

class CUIWindowManager : public CTTask<CUIWindowManager>, public cf::IFlagEvent{
public:
    static CUIWindowManager* getInstance();
    static CUIWindowManager* create(CProcess* pParent, CScnNw4r* pScene, mtl::ALLOC_HANDLE mHandle);

    void Term();
    void Move();

private:
    //0x00-0x54 CTTask
    //0x54-0x58 cf::IFlagEvent
    char unk58[0x5C - 0x58]; //0x58
    reslist<IUIWindow*> mWindowList1; //0x5C - primary window queue
    reslist<IUIWindow*> mWindowList2; //0x7C - secondary window queue
    IUIWindow* unk9C; //0x9C - child window flagged for removal on Term
    bool unkA0; //0xA0 - request: remove flagged/all windows in both queues
    bool unkA1; //0xA1 - request: force update-mark on all windows in both queues
    char unkA2[0xA4 - 0xA2]; //0xA2

    static CUIWindowManager* spInstance;

}; //size = 0xA4
