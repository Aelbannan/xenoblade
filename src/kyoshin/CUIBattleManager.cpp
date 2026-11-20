#define IWORK_EVENT_INLINE_DTOR
#include "kyoshin/CUIBattleManager.hpp"
#include "kyoshin/menu/CMenuBattleDamageSymbols.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "kyoshin/cf/CfMapItemManager.hpp"
#include "kyoshin/CUICfManager_Res.hpp"

// --- CTTask<CUIBattleManager> out-of-line specializations ---
// The canonical declared-only template emits no bodies; these explicit
// specializations produce the retail standalone Move/Draw/dtor symbols.
template<>
void CTTask<CUIBattleManager>::Move() {
    if (mMoveFunc) {
        (static_cast<CUIBattleManager*>(this)->*mMoveFunc)();
    }
}

template<>
void CTTask<CUIBattleManager>::Draw() {
    if (mDrawFunc) {
        (static_cast<CUIBattleManager*>(this)->*mDrawFunc)();
    }
}

template<>
CTTask<CUIBattleManager>::~CTTask() {}

// Retail __dt__34CTTask<Q216CUIBattleManager5CTest>Fv: empty body; MWCC emits
// the CProcess dtor call + deleting-flag delete (0x58-byte shape).
template<>
CTTask<CUIBattleManager::CTest>::~CTTask() {}

// Retail __dt__Q216CUIBattleManager5CTestFv. Empty body; MWCC inlines the
// CTTask<CTest> dtor above (CProcess dtor + delete), reusing the outer null
// check's CR0 for the inlined guard -> the retail double-beq shape.
CUIBattleManager::CTest::~CTest() {}

template<>
void CTTask<CUIBattleManager::CTest>::Move() {
    if (mMoveFunc) {
        (static_cast<CUIBattleManager::CTest*>(this)->*mMoveFunc)();
    }
}

template<>
void CTTask<CUIBattleManager::CTest>::Draw() {
    if (mDrawFunc) {
        (static_cast<CUIBattleManager::CTest*>(this)->*mDrawFunc)();
    }
}

// CfGameManager.hpp and CAIAction.hpp (via CfObjectPc.hpp) declare clashing
// return types for this import; the single canonical copy now lives in
// kyoshin/cf/CBattleManager.hpp (pulled in by both headers).
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/CTaskGameImports.hpp"
#include "kyoshin/menu/CMenuBattleCommu.hpp"
// CfObjectPc.hpp is NOT included here: its chain (CfObjectActor.hpp ->
// CAIAction.hpp vs ocBdat.hpp) currently carries divergent extern "C"
// declarations of getBdatStringColumnValue (MWCC 10197). Only opaque pointer
// casts on these types are needed, so forward declarations suffice.
namespace cf {
class CfObjectMove;
class CfObjectActor;
class CfObjectPc;
}
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/util/MemManager.hpp"

#include "decomp.h"
#include "functions.hpp"
#include <cstdio>
#include <new>

// Initial Move-callback pmf pool (retail .data 0x8052E0A8, the first user
// object of this unit's slice): the {delta = 0, index = -1, fn} record MWCC
// emits for a constant pointer-to-member-function. CUIBattleManager's
// constructor copies it into CTTask<CUIBattleManager>::mMoveFunc.
CUIBattleManager::MoveFunc UIBattleInitialMovePmf = &CUIBattleManager::parkMoveHandler;
CUIBattleManager::MoveFunc UIBattleParkMovePmf = &CUIBattleManager::UIBattleMoveNoopMember;

struct CUIBattleInitProcess {
    u8 unk00[0x3C];
    u32 callbacks[6]; // 0x3C-0x53 - two null-pmf triples
};

extern "C" {
void* UIBattleArtsSelectHandle;
char UIBattleArcPathStrings[];
u32 __ptmf_null[3];
u32 gmPauseFlags;

CUIBattleManager* CUIBattleManager::spInstance;

void __ct__8CProcessFv(CProcess*);
void* createVisionMenu(void* proc, void* scn);
class CMenuPTGauge;
CMenuPTGauge* PTGaugeCreate(CProcess* proc, CProcess* scn);
void* CMenuArtsSelect_CreateMenu(void* proc, void* scn);
void* BpsStateSingletonToInt();
void BpsStateFlag7C9Set();

int evtIsActiveSeq();
int func_801042A4();
int func_801042B4();
int CMainMenu_IsOpen();
// Family-canonical form - must match CfObjectPc.hpp / CAIAction.hpp /
// code_802B8A3C.hpp (see the 10197 note in CTaskGame.hpp).
bool isGlobalCamFlagSet__Fi(UNKWORD flag);
}

// Defined in CMenuBattlePlayerState.cpp with C++ linkage.
class CMenuBattlePlayerState;
class CProcess;
class CScn;
CMenuBattlePlayerState* BpsStateCreateSingleton(CProcess* proc, CScn* scn);

extern cf::CfObjectPc* getCfObjectPc(cf::CfObjectMove* objMove);
// C++-mangled retail helper findObjectById__Fi (actor id -> action source).
void* findObjectById(int id);
// Resource getter renamed by the parallel Track-B pass
// (CtrlRemote_TouchBitByArg -> CtrlRemote_TouchBitByArg); plain C++ decl mirrors the
// pre-rename TU state (cf. CMiniMap.cpp) so call-site codegen is unchanged.
u32 CtrlRemote_TouchBitByArg(u32 resourceId);
typedef u32* (*GetU32Fn)(void*);
typedef int (*GetIntFn)(void*);

template <typename Fn>
static inline Fn vslot(void* obj, u32 offset) {
    return reinterpret_cast<Fn>((*reinterpret_cast<void***>(obj))[offset / 4]);
}

static IWorkEvent* battleWorkEvent(CUIBattleManager* self) {
    if (self == NULL) return NULL;
    return static_cast<IWorkEvent*>(self);
}

void CUIBattleManager::Init() {
    CUIBattleInitProcess* process;

    process = static_cast<CUIBattleInitProcess*>(
        mtl::MemManager::allocate(0x54, CWorkThreadSystem::getWorkMem()));
    new (process) CUIBattleManager::CTest();
    unk7C = reinterpret_cast<CProcess*>(process);
    reinterpret_cast<CProcess*>(process)->Regist(CUIBattleManager::spInstance, false);
}

// Insert helper shape used six times below (retail inlines it each site):
// find the first free child-pool slot, link the created child into the list.
void CUIBattleManager::Move() {
    char pathBuf178[0x8C];
    char pathBufF8[0x80];
    _reslist_node<IUIBattle*>* pending[8];
    void* retCreate96B8;
    void* retCreateACCE0;
    void* retCreate87694;
    void* retCreateB0E0C;
    u32 vcallRes;
    void* retMarkA;
    void* retMarkB;
    CUIBattleManager* inst;
    _reslist_node<IUIBattle*>* temp;
    _reslist_node<IUIBattle*>* node;
    _reslist_node<IUIBattle*>* prev;
    _reslist_node<IUIBattle*>* next;
    _reslist_node<IUIBattle*>* it;
    cf::CfObjectMove* player;
    CUIBattleActor* actor;
    cf::CfObjectPc* objPc;
    void* obj;
    void* flagObj;
    int handle;
    int partyId;
    u8 faceId;
    int i;
    int count;

    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set()) {
        return;
    }
    if ((gmPauseFlags & (1u << 21)) != 0) {
        return;
    }

    mtl::MemManager::getHandleMEM2();

    if ((unk82 & 1) != 0 && getPackedFont() != 0) {
        unk82 &= ~1;
        UIBattleCreatePlayerState();
    }

    if ((unk82 & 2) != 0 && getPackedFont() != 0) {
        unk82 &= 0xfd;
        if (CUIBattleManager::spInstance != NULL) {
            if (CUICfManager_getArcResourceAccessor() == NULL) {
                CUIBattleManager::spInstance->unk82 |= 2;
            } else {
                CUIBattleManager::spInstance->unk82 &= 0xfd;
                retCreate96B8 =
                    BtlDmg_Create((CProcess*)CUIBattleManager::spInstance->unk7C, (CScn*)CUIBattleManager::spInstance->unk58);
                if (retCreate96B8 != NULL) {
                    inst = CUIBattleManager::spInstance;
                    i = 0;
                    while (i < inst->mChildList.mCapacity &&
                           inst->mChildList.mList[i].mItem == NULL) {
                        i++;
                    }
                    // First free pool slot; prepend to the active list.
                    temp = &inst->mChildList.mList[i];
                    temp->setItem((IUIBattle*)retCreate96B8);
                    temp->mNext = inst->mChildList.mStartNodePtr;
                    temp->mPrev = inst->mChildList.mStartNodePtr->mPrev;
                    inst->mChildList.mStartNodePtr->mPrev->mNext = temp;
                    inst->mChildList.mStartNodePtr->mPrev = temp;
                }
            }
        }
    }

    if ((unk82 & 8) != 0 && getPackedFont() != 0) {
        unk82 &= 0xf7;
        if (CUIBattleManager::spInstance != NULL) {
            if (CUICfManager_getArcResourceAccessor() == NULL) {
                CUIBattleManager::spInstance->unk82 |= 8;
            } else {
                CUIBattleManager::spInstance->unk82 &= 0xf7;
                retCreateACCE0 =
                    createVisionMenu(CUIBattleManager::spInstance->unk7C, CUIBattleManager::spInstance->unk58);
                if (retCreateACCE0 != NULL) {
                    inst = CUIBattleManager::spInstance;
                    i = 0;
                    while (i < inst->mChildList.mCapacity &&
                           inst->mChildList.mList[i].mItem == NULL) {
                        i++;
                    }
                    temp = &inst->mChildList.mList[i];
                    temp->setItem((IUIBattle*)retCreateACCE0);
                    temp->mNext = inst->mChildList.mStartNodePtr;
                    temp->mPrev = inst->mChildList.mStartNodePtr->mPrev;
                    inst->mChildList.mStartNodePtr->mPrev->mNext = temp;
                    inst->mChildList.mStartNodePtr->mPrev = temp;
                }
            }
        }
    }

    if ((unk82 & 0x10) != 0 && getPackedFont() != 0) {
        unk82 &= 0xef;
        if (CUIBattleManager::spInstance != NULL) {
            // Retail: cntlzw/srwi zero-test on CtrlRemote_TouchBitByArg(0x3357).
            if (__cntlzw((u32)CtrlRemote_TouchBitByArg(0x3357)) >> 5 != 0 ||
                CUICfManager_getArcResourceAccessor() == NULL) {
                CUIBattleManager::spInstance->unk82 |= 0x10;
            } else {
                CUIBattleManager::spInstance->unk82 &= 0xef;
                retCreate87694 =
                    PTGaugeCreate(CUIBattleManager::spInstance->unk7C,
                                  reinterpret_cast<CProcess*>(CUIBattleManager::spInstance->unk58));
                if (retCreate87694 != NULL) {
                    inst = CUIBattleManager::spInstance;
                    i = 0;
                    while (i < inst->mChildList.mCapacity &&
                           inst->mChildList.mList[i].mItem == NULL) {
                        i++;
                    }
                    temp = &inst->mChildList.mList[i];
                    temp->setItem((IUIBattle*)retCreate87694);
                    temp->mNext = inst->mChildList.mStartNodePtr;
                    temp->mPrev = inst->mChildList.mStartNodePtr->mPrev;
                    inst->mChildList.mStartNodePtr->mPrev->mNext = temp;
                    inst->mChildList.mStartNodePtr->mPrev = temp;
                }
            }
        }
    }

    if ((unk82 & 0x20) != 0 && getPackedFont() != 0) {
        unk82 &= 0xdf;
        if (CUIBattleManager::spInstance != NULL) {
            if (CUICfManager_getArcResourceAccessor() == NULL) {
                CUIBattleManager::spInstance->unk82 |= 0x20;
            } else {
                CUIBattleManager::spInstance->unk82 &= 0xdf;
                retCreateB0E0C =
                    battleCommuCreateRegister((CProcess*)CUIBattleManager::spInstance->unk7C, (CScn*)CUIBattleManager::spInstance->unk58);
                if (retCreateB0E0C != NULL) {
                    inst = CUIBattleManager::spInstance;
                    i = 0;
                    while (i < inst->mChildList.mCapacity &&
                           inst->mChildList.mList[i].mItem == NULL) {
                        i++;
                    }
                    temp = &inst->mChildList.mList[i];
                    temp->setItem((IUIBattle*)retCreateB0E0C);
                    temp->mNext = inst->mChildList.mStartNodePtr;
                    temp->mPrev = inst->mChildList.mStartNodePtr->mPrev;
                    inst->mChildList.mStartNodePtr->mPrev->mNext = temp;
                    inst->mChildList.mStartNodePtr->mPrev = temp;
                }
            }
        }
    }

    if (unkE9 == 0 && evtIsActiveSeq() == 0) {
        if (cf::CfGameManager::getPlayer(0) != NULL) {
            if (mFileArtsElemDone == NULL) {
                mFileArtsElem = CDeviceFile::readFile(
                    mHeap, UIBattleArcPathStrings + 0xA, battleWorkEvent(), 0, 0);
            }
            if (mFileArtsSysDone == NULL) {
                mFileArtsSys = CDeviceFile::readFile(
                    mHeap, UIBattleArcPathStrings + 0x24, battleWorkEvent(), 0, 0);
            }
            for (i = 0; i < 3; i++) {
                partyId = CtrlObjectParam_GetSlotTableBase()[i + 1];
                if (partyId > 0) {
                    if (mFileArtsPcBusy[i] == NULL) {
                        faceId = (u8)partyId;
                        // Party slot 4 remaps to slot 12 while the 0x20
                        // system flag is under 0x21 and 0x3508 is unset.
                        if (faceId == 4 && (int)CtrlRemote_TouchBitByArg(0x20) < 0x21 &&
                            (int)CtrlRemote_TouchBitByArg(0x3508) == 0) {
                            faceId = 0xC;
                        }
                        sprintf(pathBuf178, UIBattleArcPathStrings + 0x3D, faceId);
                        mFileArtsPc[i] = CDeviceFile::readFile(
                            mHeap, pathBuf178, battleWorkEvent(), 0, 0);
                    }
                    sprintf(pathBufF8, UIBattleArcPathStrings + 0x59, partyId);
                    mFileFacePc[i] = CDeviceFile::readFile(
                        mHeap, pathBufF8, battleWorkEvent(), 0, 0);
                }
            }
            unkE9 = 1;
        }
    } else {
        // A request pass ran on an earlier frame: fold load completion into
        // unkE8 (all handles cleared -> assets ready).
        if (unkE8 == 0) {
            unkE8 = 1;
            if (mFileArtsElem != NULL) {
                unkE8 = 0;
            }
            if (mFileArtsSys != NULL) {
                unkE8 = 0;
            }
            for (i = 0; i < 3; i++) {
                if (mFileArtsPc[i] != NULL) {
                    unkE8 = 0;
                    break;
                }
                if (mFileFacePc[i] != NULL) {
                    unkE8 = 0;
                    break;
                }
            }
        }
    }

    inst = CUIBattleManager::spInstance;
    if (inst != NULL && inst->unkE8 != 0 &&
        CMenuArtsSelect_isFinished() == 0) {
        cf::CfGameManager::getInstance();
        if (isGlobalCamFlagSet__Fi(0x10000000) == 0) {
            player = cf::CfGameManager::getPlayer(0);
            if (player != NULL) {
                actor = static_cast<CUIBattleActor*>(MapFx_ResolveOwner(NULL));
                vcallRes = *vslot<GetU32Fn>(actor->field_04, 0x30)(actor->field_04);
                if (testActorStatus(actor, &vcallRes, 0x803) != 0) {
                    if (CMenuArtsSelect_isCreated() == 0) {
                        handle = vslot<GetIntFn>(actor->field_3E9C, 0x4C)(
                            actor->field_3E9C);
                        UIBattleArtsSelectHandle = (void*)handle;
                        if (CUIBattleManager::spInstance != NULL) {
                            retMarkB = CMenuArtsSelect_CreateMenu(CUIBattleManager::spInstance->unk7C,
                                                     CUIBattleManager::spInstance->unk58);
                            if (retMarkB != NULL) {
                                inst = CUIBattleManager::spInstance;
                                i = 0;
                                while (i < inst->mChildList.mCapacity &&
                                       inst->mChildList.mList[i].mItem == NULL) {
                                    i++;
                                }
                                temp = &inst->mChildList.mList[i];
                                temp->setItem((IUIBattle*)retMarkB);
                                temp->mNext = inst->mChildList.mStartNodePtr;
                                temp->mPrev = inst->mChildList.mStartNodePtr->mPrev;
                                inst->mChildList.mStartNodePtr->mPrev->mNext = temp;
                                inst->mChildList.mStartNodePtr->mPrev = temp;
                            }
                        }
                    }
                } else {
                    flagObj = actor->field_3F60;
                    if (flagObj != NULL &&
                        (((CUIBattleFlagObj*)flagObj)->field_530 & 1) == 0) {
                        objPc = getCfObjectPc(player);
                        handle = vslot<GetIntFn>(objPc, 0x4C)(objPc);
                        if (handle == 0) {
                            UIBattleArtsSelectHandle = NULL;
                        } else {
                            obj = findObjectById(handle);
                            if (obj != NULL &&
                                (((CUIBattleAction*)obj)->field_64 & 4) != 0) {
                                cf::CfGameManager::getInstance();
                                if (isGlobalCamFlagSet__Fi(0x1000000) == 0 &&
                                    CMainMenu_IsOpen() == 0) {
                                    if (UIBattleArtsSelectHandle != (void*)handle ||
                                        CMenuArtsSelect_isCreated() == 0) {
                                        UIBattleArtsSelectHandle = (void*)handle;
                                        if (CUIBattleManager::spInstance != NULL) {
                                            retMarkA = CMenuArtsSelect_CreateMenu(
                                                CUIBattleManager::spInstance->unk7C,
                                                CUIBattleManager::spInstance->unk58);
                                            if (retMarkA != NULL) {
                                                inst = CUIBattleManager::spInstance;
                                                i = 0;
                                                while (i < inst->mChildList.mCapacity &&
                                                       inst->mChildList.mList[i].mItem ==
                                                           NULL) {
                                                    i++;
                                                }
                                                temp = &inst->mChildList.mList[i];
                                                temp->setItem(
                                                    (IUIBattle*)retMarkA);
                                                temp->mNext =
                                                    inst->mChildList.mStartNodePtr;
                                                temp->mPrev =
                                                    inst->mChildList.mStartNodePtr
                                                        ->mPrev;
                                                inst->mChildList.mStartNodePtr
                                                    ->mPrev->mNext = temp;
                                                inst->mChildList.mStartNodePtr
                                                    ->mPrev = temp;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Propagate update marks: once any child requests an update (or a global
    // mark-all is armed), stamp every remaining child from the front.
    node = mChildList.mStartNodePtr->mNext;
    for (it = node; it != mChildList.mStartNodePtr; it = it->mNext) {
        if (it->mItem->unk55 != 0 || unk81 != 0) {
            for (; node != mChildList.mStartNodePtr; node = node->mNext) {
                node->mItem->unk55 = 1;
            }
            break;
        }
    }

    // Collect pending removals, then unlink them (MWCC unrolls x8).
    count = 0;
    for (node = mChildList.mStartNodePtr->mNext;
         node != mChildList.mStartNodePtr; node = node->mNext) {
        if (node->mItem->unk54 != 0 || unk80 != 0) {
            node->mItem->SetRemove();
            pending[count] = node;
            count++;
        }
    }
    for (i = 0; i < count; i++) {
        temp = pending[i];
        prev = temp->mPrev;
        next = temp->mNext;
        prev->mNext = next;
        next->mPrev = prev;
        temp->mNext = NULL;
    }

    unk80 = 0;
    unk81 = 0;
}

int UIBattleHasPendingLoads() {
    CUIBattleManager* self = CUIBattleManager::spInstance;
    int i;

    if (self == NULL) return 0;
    if (self->mFileArtsElem != NULL) return 1;
    if (self->mFileArtsSys != NULL) return 1;
    // Retail unrolls this loop, folding the last iteration's i*4 into the base.
    for (i = 0; i < 3; i++) {
        if (self->mFileArtsPc[i] != NULL) return 1;
        if (self->mFileFacePc[i] != NULL) return 1;
    }
    return 0;
}
void CUIBattleManager::parkMoveHandler() {
    // Swap the Move callback to the retail no-op pmf {0, -1, fn}.
    // (Reached via the pmf installed by __ct__CUIBattleManager.)
    mMoveFunc = UIBattleParkMovePmf;
}
void CUIBattleManager::UIBattleMoveNoopMember() {}
extern "C" void UIBattleMarkFlags82() {
    if (CUIBattleManager::spInstance != 0) {
        reinterpret_cast<unsigned char *>(CUIBattleManager::spInstance)[0x82] |= 0x3b;
    }
}
// Retail UIBattleRemoveAll: remove-request processor. arg != 0 defers removal to
// a mark-all (unk81); arg == 0 unlinks every child from the list immediately.
// Do NOT cache the singleton in a local: retail re-spells CUIBattleManager::spInstance for
// the null check, every walk-condition test, and the final clear, and a cached
// local collapses those into one SDA21 load (reloc drift).
extern "C" void UIBattleRemoveAll(u32 arg) {
    _reslist_node<IUIBattle*>* removed[16];
    int count;
    _reslist_node<IUIBattle*>* node;

    if (CUIBattleManager::spInstance == NULL) {
        return;
    }
    if (arg != 0) {
        CUIBattleManager::spInstance->unk81 = 1;
        return;
    }
    count = 0;
    node = CUIBattleManager::spInstance->mChildList.mStartNodePtr->mNext;
    while (node != CUIBattleManager::spInstance->mChildList.mStartNodePtr) {
        node->mItem->SetRemove();
        removed[count++] = node;
        node = node->mNext;
    }
    for (int i = 0; i < count; i++) {
        _reslist_node<IUIBattle*>* cur = removed[i];
        _reslist_node<IUIBattle*>* prev = cur->mPrev;
        _reslist_node<IUIBattle*>* next = cur->mNext;
        prev->mNext = next;
        next->mPrev = prev;
        cur->mNext = NULL;
    }
    // Final request-bit clear re-reads the singleton in retail.
    CUIBattleManager::spInstance->unk82 = 0;
}
extern "C" unsigned char UIBattleGetFlagE8() { return CUIBattleManager::spInstance == 0 ? 0 : *(unsigned char *)((char *)CUIBattleManager::spInstance + 0xe8); }
extern "C" void* UIBattleGetTableB8() { void* p = CUIBattleManager::spInstance; if (p == 0) return 0; if (*(unsigned char*)((char*)p + 0xe8) != 0) return (char*)p + 0xb8; return 0; }
extern "C" int UIBattleGetWordEC() { if (CUIBattleManager::spInstance == 0) return -1; return *(int*)((char*)CUIBattleManager::spInstance + 0xec); }
void CUIBattleManager::CTest::Draw() {}
void CUIBattleManager::CTest::Move() {}
void CUIBattleManager::CTest::Term() {}
void CUIBattleManager::CTest::Init() {}
void __dt__16CUIBattleManagerFv(void*);
extern "C" void UIBattleDelete(void* p) {
    __dt__16CUIBattleManagerFv(reinterpret_cast<CUIBattleManager*>((char*)p - 0x54));
}

// The real constructor lets MWCC emit the CTTask/base and CUIBattleManager
// vtables (including the IWorkEvent secondary vptr) in retail order.
// The body mirrors the retail store order: null move/draw PTMF triples into
// 0x3C..0x50 (each triple stores its middle word first), child reslist at
// 0x5C with the sentinel node self-looped at 0x64, per-party resource slots
// zeroed in the retail interleaved pass order, then the node-pool allocation
// and the battle heap install.
CUIBattleManager::CUIBattleManager(CScnNw4r* pScene, mtl::ALLOC_HANDLE handle)
    : unk58((u8*)pScene) {
    u32 movePmf0;
    u32 movePmf1;
    u32 drawPmf2;
    u32 hookPmf1;
    u32 hookPmf2;
    u32 zero;

    u32* const self = reinterpret_cast<u32*>(this);
    zero = 0;
    {
        // Null move/draw PTMF copies from __ptmf_null (first word read with
        // update addressing, as in retail).
        u32* np = __ptmf_null;
        movePmf0 = *np++;
        movePmf1 = *np;
        drawPmf2 = np[1];
    }
    reinterpret_cast<u32*>(self)[0x10] = movePmf1;
    reinterpret_cast<u32*>(self)[0xF] = movePmf0;
    reinterpret_cast<u32*>(self)[0x11] = drawPmf2;
    reinterpret_cast<u32*>(self)[0x13] = movePmf1;
    reinterpret_cast<u32*>(self)[0x12] = movePmf0;
    reinterpret_cast<u32*>(self)[0x14] = drawPmf2;
    // Child reslist at 0x5C: empty slot array/capacity and sentinel node
    // self-looped at 0x64 (the reslist base ctor installs the vptr).
    reinterpret_cast<u32*>(self)[0x1C] = zero;
    reinterpret_cast<u32*>(self)[0x1D] = zero;
    reinterpret_cast<u8*>(self)[0x78] = (u8)zero;
    {
        u32* sentinel = reinterpret_cast<u32*>(reinterpret_cast<char*>(self) + 0x64);
        reinterpret_cast<u32*>(self)[0x18] = reinterpret_cast<u32>(sentinel);
        sentinel[0] = reinterpret_cast<u32>(sentinel);
        sentinel[1] = reinterpret_cast<u32>(sentinel);
    }
    reinterpret_cast<u32*>(self)[0x1F] = zero;
    reinterpret_cast<u8*>(self)[0x80] = (u8)zero;
    reinterpret_cast<u8*>(self)[0x81] = (u8)zero;
    // Arc-handle/accessor slots.
    mFileArtsElem = NULL;
    mFileArtsElemDone = NULL;
    mFileArtsSys = NULL;
    mFileArtsSysDone = NULL;
    unkE8 = 0;
    unkE9 = 0;
    mHeap = static_cast<mtl::ALLOC_HANDLE>(-1);
    // Per-party resource slots; MWCC unrolls this into the retail
    // three-pass interleaved store order (arts arc, busy accessor, face arc,
    // three face textures, face accessor per party).
    {
        int i;
        for (i = zero; i < 3; i++) {
            mFileArtsPc[i] = NULL;
            mFileArtsPcBusy[i] = NULL;
            mFileFacePc[i] = NULL;
            mFaceTex[i][0] = NULL;
            mFaceTex[i][1] = NULL;
            mFaceTex[i][2] = NULL;
            mFaceAccessor[i] = NULL;
        }
    }
    unk82 = (u8)(zero | 8);
    {
        // Child-node slot array (8 x 0xC nodes) from the scene's alloc region.
        u32* list = reinterpret_cast<u32*>(
            mtl::MemManager::allocate_array(0x60, Scn_CallUnk8C_V10(pScene)));
        int i;
        mChildList.mList = reinterpret_cast<_reslist_node<IUIBattle*>*>(list);
        for (i = 0; i < 8; i++) {
            list[i * 3] = zero;
        }
        mChildList.mCapacity = 8;
    }
    // Initial move-hook pmf from the split1 pool (w1 word stored first).
    {
        u32* ip = reinterpret_cast<u32*>(&UIBattleInitialMovePmf);
        movePmf0 = *ip++;   // w0
        hookPmf1 = *ip;     // w1
        hookPmf2 = *(ip + 1); // w2
    }
    reinterpret_cast<u32*>(self)[0x10] = hookPmf1;
    reinterpret_cast<u32*>(self)[0xF] = movePmf0;
    reinterpret_cast<u32*>(self)[0x11] = hookPmf2;
    mtl::MemManager::getHandleMEM2();
    reinterpret_cast<u32*>(self)[0x3B] = mtl::MemManager::create(
        mtl::MemManager::getHandleMEM1(), 0x44800, UIBattleArcPathStrings);
    CMenuArtsSelect_clearArtsRef();
    UIBattleArtsSelectHandle = reinterpret_cast<void*>(zero);
}

// Retail __dt__8012E534: battle-UI teardown helper (the Term virtual).
// Cancels pending file loads, destroys every arc accessor, marks the init
// child for removal, clears the child list, and releases the manager heap.
void CUIBattleManager::Term() {
    int i;

    UIBattleCancelPendingLoads(this);
    for (i = 0; i < 3; i++) {
        mFaceTex[i][0] = NULL;
        mFaceTex[i][1] = NULL;
        mFaceTex[i][2] = NULL;
        releaseArcResourceAccessor(mFaceAccessor[i]);
        releaseArcResourceAccessor(mFileArtsPcBusy[i]);
    }
    releaseArcResourceAccessor(mFileArtsSysDone);
    releaseArcResourceAccessor(mFileArtsElemDone);
    reinterpret_cast<CUIBattleChild*>(unk7C)->SetRemove();
    // Unlink every child node without destroying items, then reset the list.
    // Retail re-spells mStartNodePtr on every access (no caching).
    {
        _reslist_node<IUIBattle*>* node =
            mChildList.mStartNodePtr->mNext;
        while (node != mChildList.mStartNodePtr) {
            _reslist_node<IUIBattle*>* prev = node;
            node = node->mNext;
            prev->mNext = NULL;
        }
        mChildList.mStartNodePtr->mNext =
            mChildList.mStartNodePtr;
        mChildList.mStartNodePtr->mPrev =
            mChildList.mStartNodePtr;
    }
    if (mChildList.unk1C == false && mChildList.mList != NULL) {
        delete[] mChildList.mList;
        mChildList.mList = NULL;
    }
    mChildList.mCapacity = 0;
    mtl::MemManager::erase(mHeap);
    mHeap = mtl::INVALID_HANDLE;
    CUIBattleManager::spInstance = NULL;
}

extern "C" void UIBattleCancelPendingLoads(CUIBattleManager* self) {
    int i;

    if (self->mFileArtsElem != NULL) {
        CDeviceFile::cancel(self->mFileArtsElem);
        self->mFileArtsElem = NULL;
    }
    if (self->mFileArtsSys != NULL) {
        CDeviceFile::cancel(self->mFileArtsSys);
        self->mFileArtsSys = NULL;
    }
    for (i = 0; i < 3; i++) {
        if (self->mFileArtsPc[i] != NULL) {
            CDeviceFile::cancel(self->mFileArtsPc[i]);
            self->mFileArtsPc[i] = NULL;
        }
        if (self->mFileFacePc[i] != NULL) {
            CDeviceFile::cancel(self->mFileFacePc[i]);
            self->mFileFacePc[i] = NULL;
        }
    }
}

// File-event descriptor passed to UIBattleOnFileEvent (OnFileEvent worker).
struct CUIBattleFileEvt {
    s32 unk00;              // 0x0 - completion kind (1 = party arts arc; retail compares SIGNED cmpi)
    CFileHandle* unk04;     // 0x4 - completed file handle
};

// CUIBattleManager::OnFileEvent: dispatches a completed file load. Elem/sys arcs get an
// ArcResourceAccessor attached immediately; per-party arts arcs (kind 1) also
// pull the three face 'timg' textures out of the face accessor. getData()
// detaches the arc buffer (reads +4, stores NULL back).
int CUIBattleManager::OnFileEvent(CUIBattleFileEvt* evt) {
    int i;
    u32 partyId;
    void* tex;
    nw4r::lyt::ArcResourceAccessor* acc;
    CFileHandle* fh;

    if (mFileArtsElem == evt->unk04) {
        void* buf = mFileArtsElem->getData();
        acc = CLibLayout::createArcResourceAccessor();
        mFileArtsElemDone = acc;
        acc->Attach(buf, &UIBattleArcPathStrings[0x75]);
        mFileArtsElem = NULL;
        return 1;
    }
    if (mFileArtsSys == evt->unk04) {
        void* buf = mFileArtsSys->getData();
        acc = CLibLayout::createArcResourceAccessor();
        mFileArtsSysDone = acc;
        acc->Attach(buf, &UIBattleArcPathStrings[0x75]);
        mFileArtsSys = NULL;
        return 1;
    }
    for (i = 0; i < 3; i++) {
        if (mFileArtsPc[i] == evt->unk04) {
            if (evt->unk00 == 1) {
                void* buf = mFileArtsPc[i]->getData();
                acc = CLibLayout::createArcResourceAccessor();
                mFileArtsPcBusy[i] = acc;
                acc->Attach(buf, &UIBattleArcPathStrings[0x75]);
            }
            mFileArtsPc[i] = NULL;
            return 1;
        }
        if (mFileFacePc[i] != evt->unk04) {
            continue;
        }
        void* buf = mFileFacePc[i]->getData();
        mFileFacePc[i] = NULL;
        acc = CLibLayout::createArcResourceAccessor();
        mFaceAccessor[i] = acc;
        acc->Attach(buf, &UIBattleArcPathStrings[0x75]);
        // Pointer-arithmetic spelling: reduces to retail's bare i*4
        // induction (add r3,base,i*4 / lwz r0,4(r3)) instead of an indexed
        // load.
        partyId = *(CtrlObjectParam_GetSlotTableBase() + 1 + i);
        partyId &= 0xFF;
        if (partyId == 0) {
            continue;
        }
        tex = acc->GetResource(
            0x74696D67,
            MakeTplNameSysFile(BdatGetU16Direct(lbl_eu_80664090, &UIBattleArcPathStrings[0x79],
                                        (int)(u8)partyId)),
            0);
        if (tex != NULL) {
            mFaceTex[i][0] = tex;
        }
        tex = acc->GetResource(
            0x74696D67,
            MakeTplNameSysFile(BdatGetU16Direct(lbl_eu_80664090, &UIBattleArcPathStrings[0x84],
                                        (int)(u8)partyId)),
            0);
        if (tex != NULL) {
            mFaceTex[i][1] = tex;
        }
        tex = acc->GetResource(
            0x74696D67,
            MakeTplNameSysFile(BdatGetU16Direct(lbl_eu_80664090, &UIBattleArcPathStrings[0x8f],
                                        (int)(u8)partyId)),
            0);
        if (tex != NULL) {
            mFaceTex[i][2] = tex;
        }
        return 1;
    }
    return 0;
}
CUIBattleManager* UIBattleGetOrCreate(CProcess* pParent, CScnNw4r* pScene,
                                mtl::ALLOC_HANDLE handle) {
    CUIBattleManager* mgr;

    if (CUIBattleManager::spInstance != NULL) {
        return CUIBattleManager::spInstance;
    }
    if (handle == mtl::INVALID_HANDLE) {
        handle = mtl::MemManager::getHandleMEM2();
    }
    mgr = static_cast<CUIBattleManager*>(
        mtl::MemManager::allocate(sizeof(CUIBattleManager), CWorkThreadSystem::getWorkMem()));
    mgr = new (mgr) CUIBattleManager(pScene, handle);
    CUIBattleManager::spInstance = mgr;
    mgr->Regist(pParent, false);
    return CUIBattleManager::spInstance;
}
// Retail UIBattleCreatePlayerState: create/bind request for the main battle window.
// Returns the created child, or NULL (re-arming the unk82 bit 1 request on
// any failure path). Same savedRet stack-home idiom as UIBattleCreateVisionMenu.
extern "C" CUIBattleChild* UIBattleCreatePlayerState() {
    void* savedRet;
    int i;
    int byteOff;
    int capacity;
    _reslist_node<IUIBattle*>* startNode;
    _reslist_node<IUIBattle*>* temp;
    CUIBattleManager* inst;

    if (CUIBattleManager::spInstance == NULL) {
        return NULL;
    }
    if (CUICfManager_getArcResourceAccessor() == NULL) {
        CUIBattleManager::spInstance->unk82 |= 1;
        return NULL;
    }
    if (CUIBattleManager::spInstance->unkE8 == 0) {
        CUIBattleManager::spInstance->unk82 |= 1;
        return NULL;
    }
    if (evtIsActiveSeq() != 0) {
        CUIBattleManager::spInstance->unk82 |= 1;
        return NULL;
    }
    CUIBattleManager::spInstance->unk82 &= ~1u;
    savedRet = BpsStateCreateSingleton((CProcess*)CUIBattleManager::spInstance->unk7C,
                                       (CScn*)CUIBattleManager::spInstance->unk58);
    if (savedRet == NULL) {
        return NULL;
    }
    inst = CUIBattleManager::spInstance;
    i = 0;
    byteOff = 0;
    startNode = inst->mChildList.mStartNodePtr;
    capacity = inst->mChildList.mCapacity;
    while (i < capacity) {
        if (*(u32*)((u8*)inst->mChildList.mList + byteOff) == 0) {
            break;
        }
        byteOff += 0xc;
        i++;
    }
    temp = (_reslist_node<IUIBattle*>*)((u8*)inst->mChildList.mList + i * 0xc);
    temp->setItem((IUIBattle*)savedRet);
    temp->mNext = startNode;
    temp->mPrev = startNode->mPrev;
    startNode->mPrev->mNext = temp;
    startNode->mPrev = temp;
    return (IUIBattle*)savedRet;
}

// Retail UIBattleCreateVisionMenu: create/bind request variant using bit 3 of unk82 and
// the createVisionMenu factory (no assets-ready gating, unlike UIBattleCreatePlayerState).
// Same shape as the bit-2/bit-8 blocks in Move(): the call result lives in a
// function-top savedRet local so MWCC stack-homes it across the setItem
// try/catch region, matching the retail spill/reload/EH-save block.
extern "C" CUIBattleChild* UIBattleCreateVisionMenu(u32 arg) {
    void* savedRet;
    int i;
    int byteOff;
    int capacity;
    _reslist_node<IUIBattle*>* startNode;
    _reslist_node<IUIBattle*>* temp;
    CUIBattleManager* inst;

    if (CUIBattleManager::spInstance == NULL) {
        return NULL;
    }
    if (CUICfManager_getArcResourceAccessor() == NULL) {
        CUIBattleManager::spInstance->unk82 |= 8;
        return NULL;
    }
    CUIBattleManager::spInstance->unk82 &= ~8u;
    savedRet = createVisionMenu(CUIBattleManager::spInstance->unk7C, CUIBattleManager::spInstance->unk58);
    if (savedRet == NULL) {
        return NULL;
    }
    inst = CUIBattleManager::spInstance;
    i = 0;
    byteOff = 0;
    startNode = inst->mChildList.mStartNodePtr;
    capacity = inst->mChildList.mCapacity;
    while (i < capacity) {
        if (*(u32*)((u8*)inst->mChildList.mList + byteOff) == 0) {
            break;
        }
        byteOff += 0xc;
        i++;
    }
    temp = (_reslist_node<IUIBattle*>*)((u8*)inst->mChildList.mList + i * 0xc);
    temp->setItem((IUIBattle*)savedRet);
    temp->mNext = startNode;
    temp->mPrev = startNode->mPrev;
    startNode->mPrev->mNext = temp;
    startNode->mPrev = temp;
    return (IUIBattle*)savedRet;
}

// Retail UIBattleReleaseAll: full battle-UI release. Waits for draw done, cancels
// pending reads, removes the active move's mark window, then destroys the
// face/party accessors and re-arms the create request.
extern "C" void UIBattleReleaseAll() {
    CDeviceVI::waitForDrawDone();
    if (CUIBattleManager::spInstance == NULL) {
        return;
    }
    UIBattleCancelPendingLoads(CUIBattleManager::spInstance);
    {
        CUIBattleChild* matchItem = static_cast<CUIBattleChild*>(BpsStateSingletonToInt());
        if (matchItem != NULL) {
            BpsStateFlag7C9Set();
            {
                // Retail walks the child reslist with the condition checked
                // first, re-reading the singleton (and thus the sentinel)
                // every iteration.
                _reslist_node<IUIBattle*>* node =
                    CUIBattleManager::spInstance->mChildList.mStartNodePtr->mNext;
                while (node != CUIBattleManager::spInstance->mChildList.mStartNodePtr) {
                    if (node->mItem == matchItem) {
                        node->mItem->SetRemove();
                        _reslist_node<IUIBattle*>* prev = node->mPrev;
                        _reslist_node<IUIBattle*>* next = node->mNext;
                        prev->mNext = next;
                        next->mPrev = prev;
                        node->mNext = NULL;
                        node = next;
                    } else {
                        node = node->mNext;
                    }
                }
            }
        }
    }
    {
        int i;
        for (i = 0; i < 3; i++) {
            int j;
            for (j = 0; j < 3; j++) {
                CUIBattleManager::spInstance->mFaceTex[i][j] = NULL;
            }
            // Detach returns the arc buffer; deleting dtor frees the object,
            // then the detached buffer is released back to the heap. Retail
            // re-spells the singleton for every access so MWCC reloads the
            // SDA21 base each time.
            if (CUIBattleManager::spInstance->mFaceAccessor[i] != NULL) {
                void* arcBuf = CUIBattleManager::spInstance->mFaceAccessor[i]->Detach();
                if (CUIBattleManager::spInstance->mFaceAccessor[i] != NULL) {
                    delete CUIBattleManager::spInstance->mFaceAccessor[i];
                }
                CUIBattleManager::spInstance->mFaceAccessor[i] = NULL;
                if (arcBuf != NULL) {
                    mtl::MemManager::deallocate(arcBuf);
                }
            }
            if (CUIBattleManager::spInstance->mFileArtsPcBusy[i] != NULL) {
                void* arcBuf = CUIBattleManager::spInstance->mFileArtsPcBusy[i]->Detach();
                if (CUIBattleManager::spInstance->mFileArtsPcBusy[i] != NULL) {
                    delete CUIBattleManager::spInstance->mFileArtsPcBusy[i];
                }
                CUIBattleManager::spInstance->mFileArtsPcBusy[i] = NULL;
                if (arcBuf != NULL) {
                    mtl::MemManager::deallocate(arcBuf);
                }
            }
        }
    }
    CUIBattleManager::spInstance->unkE9 = 0;
    CUIBattleManager::spInstance->unkE8 = 0;
    UIBattleCreatePlayerState();
}
// Check the arts-sys arc accessor (0x90) for a named 'timg' texture resource.
// Returns the resource pointer, or NULL if the accessor is not ready / missing.
void* UIBattleFindSysTimg(const char* name) {
    nw4r::lyt::ArcResourceAccessor* acc = CUIBattleManager::spInstance->mFileArtsSysDone;
    if (acc != NULL) {
        void* result = acc->GetResource(0x74696D67 /* 'timg' */, name, 0);
        if (result != NULL) {
            return result;
        }
    }
    return NULL;
}

// Same as UIBattleFindSysTimg but for the arts-elem arc accessor (0x88).
void* UIBattleFindElemTimg(const char* name) {
    nw4r::lyt::ArcResourceAccessor* acc = CUIBattleManager::spInstance->mFileArtsElemDone;
    if (acc != NULL) {
        void* result = acc->GetResource(0x74696D67 /* 'timg' */, name, 0);
        if (result != NULL) {
            return result;
        }
    }
    return NULL;
}

// Search the three per-party arts accessors (0xA0 + i*4) for a named 'timg'
// texture resource. Returns the first hit, or NULL.
void* UIBattleFindPartyTimg(const char* name) {
    u8 i;
    for (i = 0; i < 3; i++) {
        nw4r::lyt::ArcResourceAccessor* acc = CUIBattleManager::spInstance->mFileArtsPcBusy[i];
        if (acc != NULL) {
            void* result = acc->GetResource(0x74696D67 /* 'timg' */, name, 0);
            if (result != NULL) {
                return result;
            }
        }
    }
    return NULL;
}

// Retail __dt__16CUIBattleManagerFv. Empty user body; MWCC generates the
// implicit teardown: inlined ~reslist/~_reslist_base for mChildList followed by
// the inlined empty CTTask<CUIBattleManager> dtor's __dt__8CProcessFv call.
CUIBattleManager::~CUIBattleManager() {}

// This-adjusting thunk: subi r3, r3, 0x54; b OnFileEvent member
// MWCC does not generate this automatically for non-dtor virtual overrides.
// A real member override emits a 0x2C-byte body (11 structural diffs, 0%) instead.
extern "C" void OnFileEvent__16CUIBattleManagerFP10CEventFile(void* self, void* file) {
    reinterpret_cast<CUIBattleManager*>((char*)self - 0x54)->CUIBattleManager::OnFileEvent(
        static_cast<CUIBattleFileEvt*>(file));
}
