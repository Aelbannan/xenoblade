#pragma once

#include "monolib/work.hpp"

class CUIBattleManager : public CTTask<CUIBattleManager>, public IWorkEvent {
public:
    static CUIBattleManager* create(CProcess* pParent, CScnNw4r* pScene, mtl::ALLOC_HANDLE mHandle);

    static void func_8012F87C(u32);

    void Init();

private:
    // 0x00-0x54 CTTask
    // 0x54-0x58 IWorkEvent
    u8 unk58[0x7C - 0x58]; // 0x58
    CProcess* unk7C;       // 0x7C
    u8 unk80[0xF0 - 0x80]; // 0x80
}; // size = 0xF0
