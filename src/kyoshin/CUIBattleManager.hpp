#pragma once

#include "monolib/device/CFileHandle.hpp"
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include "monolib/util/reslist.hpp"
#include "monolib/work.hpp"
#include "monolib/work/CTTask.hpp"
#include <nw4r/lyt/lyt_arcResourceAccessor.h>
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void* MapFx_ResolveOwner(void* r3);

// Accessor teardown helper (code_80135FDC.cpp); retail symbol is the
// Itanium-mangled name, so declare unmangled.

// C-linkage imports used by the battle-manager constructor.
extern "C" mtl::ALLOC_HANDLE Scn_CallUnk8C_V10(void* ptr);
extern "C" void CMenuArtsSelect_clearArtsRef();
// Layout/BDAT helpers used by the file-event handler.

class CProcess;
class CUIBattleManager;
struct CUIBattleFileEvt;

// Real retail IUIBattle base: CTTask<IUIBattle> (0x00-0x53) plus the two
// tail bytes at +0x54/+0x55.  The former byte-array view hid the inheritance
// that CMenuVision's retail vtable and base list prove.
class IUIBattle : public CTTask<IUIBattle> {
public:
    void SetRemove() { CProcess::SetRemove(); }

    u8 unk54; // 0x54 - pending-remove request
    u8 unk55; // 0x55 - pending update-mark
};

typedef IUIBattle CUIBattleChild;

// Opaque battle-side object returned by MapFx_ResolveOwner (touched by Move).
struct CUIBattleActor {
    u8 unk00[0x4];
    void* field_04;                   // 0x04 - queried sub-object (vtable slot 0x30)
    u8 unk08[0x3E9C - 0x8];
    u8 field_3E9C[0x3F60 - 0x3E9C];   // 0x3E9C - move sub-object (vtable slot 0x4C)
    void* field_3F60;                 // 0x3F60 - flag object
};

// Flag object hanging off CUIBattleActor::field_3F60.
struct CUIBattleFlagObj {
    u8 unk00[0x530];
    u16 field_530;                    // bit0 checked by Move
};

// Action-source object returned by findObjectById.
struct CUIBattleAction {
    u8 unk00[0x64];
    u32 field_64;                     // bit2 checked by Move
};

// Battle-UI helpers. Retail binds these by their plain (unmangled) names.
extern "C" bool CMenuArtsSelect_isFinished();
extern "C" void UIBattleCancelPendingLoads(CUIBattleManager* self);
extern "C" CUIBattleChild* UIBattleCreatePlayerState();
extern "C" CUIBattleChild* UIBattleCreateVisionMenu(u32 arg);
extern "C" void UIBattleReleaseAll();
extern "C" void UIBattleRemoveAll(u32 arg);

// Retail texture lookup helpers (unmangled symbols; defined in
// CUIBattleManager.cpp, which accesses private members via friend decls
// inside the class) - declared BEFORE the class with C linkage so the
// friend declarations refer to these same C-linkage functions.
extern "C" void* UIBattleFindSysTimg(const char* name);
extern "C" void* UIBattleFindElemTimg(const char* name);
extern "C" void* UIBattleFindPartyTimg(const char* name);

class CUIBattleManager : public CTTask<CUIBattleManager>, public IWorkEvent {
public:
    static CUIBattleManager* spInstance;

    CUIBattleManager(CScnNw4r* pScene, mtl::ALLOC_HANDLE handle);
    static CUIBattleManager* create(CProcess* pParent, CScnNw4r* pScene, mtl::ALLOC_HANDLE mHandle);

    static void UIBattleRemoveAll(u32);

    virtual void Init();
    virtual void Term();
    virtual int OnFileEvent(CUIBattleFileEvt* evt);
    void Move();
    void parkMoveHandler();
    void UIBattleMoveNoopMember();
    IWorkEvent* battleWorkEvent() { return static_cast<IWorkEvent*>(this); }

    // Battle-UI child task type (retail nested class). The CTTask<CTest>
    // out-of-line Move/Draw specializations are emitted in CUIBattleManager.cpp
    // (mangling Q216CUIBattleManager5CTest); CTest's vtable is emitted from
    // these real member functions.
    class CTest : public CTTask<CTest> {
    public:
        ~CTest();
        void Init();
        void Term();
        void Move();
        void Draw();
    };

private:
    // 0x00-0x54 CTTask
    // 0x54-0x58 IWorkEvent
    u8* unk58;                           // 0x58 - scene / create arg
    reslist<IUIBattle*> mChildList;       // 0x5C
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

    // Retail free functions UIBattleFindSysTimg / UIBattleFindElemTimg / UIBattleFindPartyTimg
    // (texture lookup helpers over the private arc-accessor fields) are
    // declared with C linkage at global scope above; these friends grant
    // them member access.
    friend void* UIBattleFindSysTimg(const char* name);
    friend void* UIBattleFindElemTimg(const char* name);
    friend void* UIBattleFindPartyTimg(const char* name);
    // Retail file-cancel / busy-flag free functions over the private handles.
    friend void UIBattleCancelPendingLoads(CUIBattleManager* self);
    friend int UIBattleHasPendingLoads();
    // Teardown / create-bind / release helpers over the private fields.
    friend CUIBattleChild* UIBattleCreatePlayerState();
    friend CUIBattleChild* UIBattleCreateVisionMenu(u32 arg);
    friend void UIBattleReleaseAll();
    friend void UIBattleRemoveAll(u32 arg);
    virtual ~CUIBattleManager();
}; // size = 0xF0

// Free functions defined in this TU (real C++ linkage, retail-mangled names).
class CScnNw4r;
int UIBattleHasPendingLoads();
CUIBattleManager* UIBattleGetOrCreate(CProcess* pParent, CScnNw4r* pScene,
                                      u32 handle);

// 3-word member-function-pointer pool in retail .data (split1 unit).
// parkMoveHandler copies this pmf ({0, -1, no-op member}) into mMoveFunc.
extern CUIBattleManager::MoveFunc UIBattleParkMovePmf;

// Array-delete / delete helpers (C-ABI imports).

