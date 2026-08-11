#pragma once

#include "monolib/device/CFileHandle.hpp"
#include "monolib/util/reslist.hpp"
#include "monolib/work.hpp"
#include <nw4r/lyt/lyt_arcResourceAccessor.h>

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void* func_8016FE34(void* r3);

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

// Minimal IUIBattle interface tag. Retail instantiates reslist<IUIBattle>
// (dtors __dt___reslist_base_IUIBattle / __dt__reslist_IUIBattle) whose nodes
// are POD (the array delete emits a bare __dla__ with no per-element dtor
// loop), so a trivially-destructible empty class matches the node layout.
class IUIBattle {};

// reslist<IUIBattle> / _reslist_base<IUIBattle> layout mirror (reslist.hpp).
// The retail dtor symbols use the old flat template mangling, so the deleting
// destructors are emitted as plain global functions over this layout instead
// of the modern-mangled member dtors MWCC 1.1 would emit.
struct ResListIUIBattle {
    void* mVtable;                        // 0x0 - lbl_eu_8052E1B4
    _reslist_node<IUIBattle>* mHead;      // 0x4 - &mStartNode
    _reslist_node<IUIBattle> mStartNode;  // 0x8
    _reslist_node<IUIBattle>* mList;      // 0x14 - slot array
    int mCapacity;                        // 0x18
    u8 mOwnsList;                         // 0x1C
};

class CUIBattleManager : public CTTask<CUIBattleManager>, public IWorkEvent {
public:
    static CUIBattleManager* create(CProcess* pParent, CScnNw4r* pScene, mtl::ALLOC_HANDLE mHandle);

    static void func_8012F87C(u32);

    void Init();
    void Move();
    void func_8012F29C();
    IWorkEvent* battleWorkEvent() { return static_cast<IWorkEvent*>(this); }

    // Battle-UI child task type (retail nested class). The CTTask<CTest>
    // out-of-line Move/Draw specializations are emitted in CUIBattleManager.cpp
    // (mangling Q216CUIBattleManager5CTest); CTest's own vtable entries are
    // still the extern "C" stubs in the cpp.
    class CTest : public CTTask<CTest> {
    public:
        ~CTest();
    };

private:
    // 0x00-0x54 CTTask
    // 0x54-0x58 IWorkEvent
    u8* unk58;                           // 0x58 - scene / create arg
    reslist<CUIBattleChild*> mChildList;   // 0x5C
    CProcess* unk7C;                       // 0x7C - Init child process
    u8 unk80;                              // 0x80 - remove-all request
    u8 unk81;                              // 0x81 - mark-all request
    u8 unk82;                              // 0x82 - create/bind request bits
    u8 unk83;                              // 0x83
    CFileHandle* mFileArtsElem;                        // 0x84 - raw arc handle (cleared once accessor is built)
    nw4r::lyt::ArcResourceAccessor* mFileArtsElemDone; // 0x88 - elem arc accessor (set by OnFileEvent)
    CFileHandle* mFileArtsSys;                         // 0x8C - raw arc handle
    nw4r::lyt::ArcResourceAccessor* mFileArtsSysDone;  // 0x90 - sys arc accessor (set by OnFileEvent)
    CFileHandle* mFileArtsPc[3];                       // 0x94 - raw per-party arc handles
    nw4r::lyt::ArcResourceAccessor* mFileArtsPcBusy[3]; // 0xA0 - per-party accessors (set by OnFileEvent)
    CFileHandle* mFileFacePc[3];                       // 0xAC - raw face arc handles
    void* mFaceTex[3][3];                              // 0xB8 - 3 'timg' textures per party face
    nw4r::lyt::ArcResourceAccessor* mFaceAccessor[3];  // 0xDC - face arc accessors (set by OnFileEvent)
    u8 unkE8;                              // 0xE8 - assets ready
    u8 unkE9;                              // 0xE9 - assets requested
    u8 unkEA[0xEC - 0xEA];                 // 0xEA
    mtl::ALLOC_HANDLE mHeap;               // 0xEC

    // Retail free functions func_8012FD04 / func_8012FD60 / func_8012FC74
    // (texture lookup helpers over the private arc-accessor fields).
    friend void* func_8012FD04(const char* name);
    friend void* func_8012FD60(const char* name);
    friend void* func_8012FC74(const char* name);
    // Retail file-cancel / busy-flag free functions over the private handles.
    friend void func_8012E630(CUIBattleManager* self);
    friend int func_8012E6DC();
    CUIBattleManager();
    virtual ~CUIBattleManager();
    void OnFileEvent();
}; // size = 0xF0

// 3-word member-function-pointer pool in retail .data (split1 unit).
// func_8012F29C copies this pmf ({0, -1, func_8012F2BC}) into mMoveFunc.
extern CUIBattleManager::MoveFunc lbl_eu_8052E0B4;

// _reslist_base<IUIBattle> vtable (split1 .data). The reslist deleting
// destructors store this into the object header (retail lbl_eu_8052E1B4).
extern void* lbl_eu_8052E1B4;
