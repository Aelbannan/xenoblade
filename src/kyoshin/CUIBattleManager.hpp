#pragma once

#include "monolib/device/CFileHandle.hpp"
#include "monolib/util/reslist.hpp"
#include "monolib/work.hpp"

// Minimal battle-UI child type (IUIWindow-like) for Move's mark/remove walk.
class CUIBattleChild {
private:
    u8 unk00[0x39];
    bool mIsRemove; // 0x39
    u8 unk3A[0x54 - 0x3A];

public:
    void SetRemove() { mIsRemove = true; }

    u8 unk54; // 0x54 - pending-remove request
    u8 unk55; // 0x55 - pending update-mark
};

class CUIBattleManager : public CTTask<CUIBattleManager>, public IWorkEvent {
public:
    static CUIBattleManager* create(CProcess* pParent, CScnNw4r* pScene, mtl::ALLOC_HANDLE mHandle);

    static void func_8012F87C(u32);

    void Init();
    void Move();

private:
    // 0x00-0x54 CTTask
    // 0x54-0x58 IWorkEvent
    void* unk58;                           // 0x58 - scene / create arg
    reslist<CUIBattleChild*> mChildList;   // 0x5C
    CProcess* unk7C;                       // 0x7C - Init child process
    u8 unk80;                              // 0x80 - remove-all request
    u8 unk81;                              // 0x81 - mark-all request
    u8 unk82;                              // 0x82 - create/bind request bits
    u8 unk83;                              // 0x83
    CFileHandle* mFileArtsElem;            // 0x84
    CFileHandle* mFileArtsElemDone;        // 0x88
    CFileHandle* mFileArtsSys;             // 0x8C
    CFileHandle* mFileArtsSysDone;         // 0x90
    CFileHandle* mFileArtsPc[3];           // 0x94
    CFileHandle* mFileArtsPcBusy[3];       // 0xA0
    CFileHandle* mFileFacePc[3];           // 0xAC
    u8 unkB8[0xE8 - 0xB8];                 // 0xB8
    u8 unkE8;                              // 0xE8 - assets ready
    u8 unkE9;                              // 0xE9 - assets requested
    u8 unkEA[0xEC - 0xEA];                 // 0xEA
    mtl::ALLOC_HANDLE mHeap;               // 0xEC
}; // size = 0xF0
