#define IWORK_EVENT_INLINE_DTOR
#include "kyoshin/CUIBattleManager.hpp"
#include "kyoshin/cf/CfMapItemManager.hpp"

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
// kyoshin/cf/CBattleManagerApi.hpp (pulled in by both headers).
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/CTaskGame.hpp"
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

// _reslist_base<CUIBattleChild*>::~_reslist_base()
// Implicit-member-destruction body for mChildList; spelled out so the
// base-vtable store references the split1 pool symbol lbl_eu_8052E1B4 instead
// of the compiler's auto-mangled __vt__ name. Must appear before any ODR-use
// of the template so it overrides the inline definition in reslist.hpp.
template <>
_reslist_base<CUIBattleChild*>::~_reslist_base() {
    *(void**)this = (void*)lbl_eu_8052E1B4;
    _reslist_node<CUIBattleChild*>* old;
    _reslist_node<CUIBattleChild*>* cur = mStartNodePtr->mNext;
    while (cur != mStartNodePtr) {
        old = cur;
        cur = cur->mNext;
        old->mNext = nullptr;
    }
    mStartNodePtr->mNext = mStartNodePtr;
    mStartNodePtr->mPrev = mStartNodePtr;

    if (!unk1C && mList != nullptr) {
        __dla__FPv(mList);
        mList = nullptr;
    }
}

// 3-word null-pmf constant mirror (retail __ptmf_null). Struct access keeps
// MWCC on a single materialised base register instead of folding the offset-0
// word into `lwz @l(r5)` (which would add a reloc retail lacks).
struct CUIBattlePtmfWords {
    u32 w0; // 0x0
    u32 w1; // 0x4
    u32 w2; // 0x8
};

struct CUIBattleInitProcess {
    u8 unk00[0x10];
    void* vtable;
    u8 unk14[0x28];
    u32 callbacks[6]; // 0x3C-0x53 - two null-pmf triples
};

extern "C" {
CUIBattleManager* lbl_eu_80664048;
void* lbl_eu_8066404C;
char lbl_eu_8052E208[];
char lbl_eu_8052E1C0[];
char lbl_eu_804FFF2C[];
u32 __ptmf_null[3];
u32 lbl_eu_80663E28;
// Ctor vtable / pmf-pool constants (split1 .data).
void* lbl_eu_8052E248[]; // interim CTTask<CUIBattleManager> vtable
void* lbl_eu_8052E0C0[]; // final CUIBattleManager vtable
void* lbl_eu_8052E19C[]; // reslist<CUIBattleChild*> vtable
void* lbl_eu_8052E0A8[]; // initial mMoveFunc pmf pool
void __ct__8CProcessFv(CProcess*);

void* func_801096B8(void* proc, void* scn);
void* func_801ACCE0(void* proc, void* scn);
void* func_80187694(void* proc, void* scn);
void* func_801B0E0C(void* proc, void* scn);
void* func_80104210(void* proc, void* scn);
void* func_8010CDCC(void* proc, void* scn);
void* func_8010CE48();
void func_8010CF5C();

int func_80164410();
int func_801042A4();
int func_801042B4();
int func_800FF738();
// Family-canonical form - must match CfObjectPc.hpp / CAIAction.hpp /
// code_802B8A3C.hpp (see the 10197 note in CTaskGame.hpp).
bool func_8006EF04__Fi(UNKWORD r3);
void* func_801355F4();
}

extern u32 func_801355A0();
extern cf::CfObjectPc* func_800BFC68(cf::CfObjectMove* objMove);
// C++-mangled retail helper func_800B708C__Fi (actor id -> action source).
void* func_800B708C(int id);typedef u32* (*GetU32Fn)(void*);
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
    const CUIBattlePtmfWords* ptmf;
    u32 ptmfWord1;
    u32 ptmfWord0;
    u32 ptmfWord2;

    process = static_cast<CUIBattleInitProcess*>(
        mtl::MemManager::allocate(0x54, CWorkThreadSystem::getWorkMem()));
    if (process != NULL) {
        __ct__8CProcessFv(reinterpret_cast<CProcess*>(process));
        process->vtable = lbl_eu_8052E208;
        ptmf = reinterpret_cast<const CUIBattlePtmfWords*>(__ptmf_null);
        ptmfWord1 = ptmf->w1;
        ptmfWord0 = ptmf->w0;
        process->callbacks[0] = ptmfWord0;
        process->callbacks[1] = ptmfWord1;
        ptmfWord2 = ptmf->w2;
        process->callbacks[2] = ptmfWord2;
        ptmfWord1 = ptmf->w1;
        ptmfWord0 = ptmf->w0;
        process->callbacks[3] = ptmfWord0;
        process->callbacks[4] = ptmfWord1;
        ptmfWord2 = ptmf->w2;
        process->callbacks[5] = ptmfWord2;
        process->vtable = lbl_eu_8052E1C0;
    }
    unk7C = reinterpret_cast<CProcess*>(process);
    reinterpret_cast<CProcess*>(process)->Regist(lbl_eu_80664048, false);
}

// Insert helper shape used six times below (retail inlines it each site):
// find the first free child-pool slot, link the created child into the list.
void CUIBattleManager::Move() {
    char pathBuf178[0x8C];
    char pathBufF8[0x80];
    _reslist_node<CUIBattleChild*>* pending[8];
    void* retCreate96B8;
    void* retCreateACCE0;
    void* retCreate87694;
    void* retCreateB0E0C;
    u32 vcallRes;
    void* retMarkA;
    void* retMarkB;
    CUIBattleManager* inst;
    _reslist_node<CUIBattleChild*>* temp;
    _reslist_node<CUIBattleChild*>* node;
    _reslist_node<CUIBattleChild*>* prev;
    _reslist_node<CUIBattleChild*>* next;
    _reslist_node<CUIBattleChild*>* it;
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
    if (CTaskGame::func_800426F0()) {
        return;
    }
    if ((lbl_eu_80663E28 & (1u << 21)) != 0) {
        return;
    }

    mtl::MemManager::getHandleMEM2();

    if ((unk82 & 1) != 0 && func_801355A0() != 0) {
        unk82 &= ~1;
        func_8012F5F8();
    }

    if ((unk82 & 2) != 0 && func_801355A0() != 0) {
        unk82 &= 0xfd;
        if (lbl_eu_80664048 != NULL) {
            if (func_801355F4() == NULL) {
                lbl_eu_80664048->unk82 |= 2;
            } else {
                lbl_eu_80664048->unk82 &= 0xfd;
                retCreate96B8 =
                    func_801096B8(lbl_eu_80664048->unk7C, lbl_eu_80664048->unk58);
                if (retCreate96B8 != NULL) {
                    inst = lbl_eu_80664048;
                    i = 0;
                    while (i < inst->mChildList.mCapacity &&
                           inst->mChildList.mList[i].mItem == NULL) {
                        i++;
                    }
                    // First free pool slot; prepend to the active list.
                    temp = &inst->mChildList.mList[i];
                    temp->setItem((CUIBattleChild*)retCreate96B8);
                    temp->mNext = inst->mChildList.mStartNodePtr;
                    temp->mPrev = inst->mChildList.mStartNodePtr->mPrev;
                    inst->mChildList.mStartNodePtr->mPrev->mNext = temp;
                    inst->mChildList.mStartNodePtr->mPrev = temp;
                }
            }
        }
    }

    if ((unk82 & 8) != 0 && func_801355A0() != 0) {
        unk82 &= 0xf7;
        if (lbl_eu_80664048 != NULL) {
            if (func_801355F4() == NULL) {
                lbl_eu_80664048->unk82 |= 8;
            } else {
                lbl_eu_80664048->unk82 &= 0xf7;
                retCreateACCE0 =
                    func_801ACCE0(lbl_eu_80664048->unk7C, lbl_eu_80664048->unk58);
                if (retCreateACCE0 != NULL) {
                    inst = lbl_eu_80664048;
                    i = 0;
                    while (i < inst->mChildList.mCapacity &&
                           inst->mChildList.mList[i].mItem == NULL) {
                        i++;
                    }
                    temp = &inst->mChildList.mList[i];
                    temp->setItem((CUIBattleChild*)retCreateACCE0);
                    temp->mNext = inst->mChildList.mStartNodePtr;
                    temp->mPrev = inst->mChildList.mStartNodePtr->mPrev;
                    inst->mChildList.mStartNodePtr->mPrev->mNext = temp;
                    inst->mChildList.mStartNodePtr->mPrev = temp;
                }
            }
        }
    }

    if ((unk82 & 0x10) != 0 && func_801355A0() != 0) {
        unk82 &= 0xef;
        if (lbl_eu_80664048 != NULL) {
            // Retail: cntlzw/srwi zero-test on func_8009CF8C(0x3357).
            if (__cntlzw((u32)func_8009CF8C(0x3357)) >> 5 != 0 ||
                func_801355F4() == NULL) {
                lbl_eu_80664048->unk82 |= 0x10;
            } else {
                lbl_eu_80664048->unk82 &= 0xef;
                retCreate87694 =
                    func_80187694(lbl_eu_80664048->unk7C, lbl_eu_80664048->unk58);
                if (retCreate87694 != NULL) {
                    inst = lbl_eu_80664048;
                    i = 0;
                    while (i < inst->mChildList.mCapacity &&
                           inst->mChildList.mList[i].mItem == NULL) {
                        i++;
                    }
                    temp = &inst->mChildList.mList[i];
                    temp->setItem((CUIBattleChild*)retCreate87694);
                    temp->mNext = inst->mChildList.mStartNodePtr;
                    temp->mPrev = inst->mChildList.mStartNodePtr->mPrev;
                    inst->mChildList.mStartNodePtr->mPrev->mNext = temp;
                    inst->mChildList.mStartNodePtr->mPrev = temp;
                }
            }
        }
    }

    if ((unk82 & 0x20) != 0 && func_801355A0() != 0) {
        unk82 &= 0xdf;
        if (lbl_eu_80664048 != NULL) {
            if (func_801355F4() == NULL) {
                lbl_eu_80664048->unk82 |= 0x20;
            } else {
                lbl_eu_80664048->unk82 &= 0xdf;
                retCreateB0E0C =
                    func_801B0E0C(lbl_eu_80664048->unk7C, lbl_eu_80664048->unk58);
                if (retCreateB0E0C != NULL) {
                    inst = lbl_eu_80664048;
                    i = 0;
                    while (i < inst->mChildList.mCapacity &&
                           inst->mChildList.mList[i].mItem == NULL) {
                        i++;
                    }
                    temp = &inst->mChildList.mList[i];
                    temp->setItem((CUIBattleChild*)retCreateB0E0C);
                    temp->mNext = inst->mChildList.mStartNodePtr;
                    temp->mPrev = inst->mChildList.mStartNodePtr->mPrev;
                    inst->mChildList.mStartNodePtr->mPrev->mNext = temp;
                    inst->mChildList.mStartNodePtr->mPrev = temp;
                }
            }
        }
    }

    if (unkE9 == 0 && func_80164410() == 0) {
        if (cf::CfGameManager::getPlayer(0) != NULL) {
            if (mFileArtsElemDone == NULL) {
                mFileArtsElem = CDeviceFile::readFile(
                    mHeap, lbl_eu_804FFF2C + 0xA, battleWorkEvent(), 0, 0);
            }
            if (mFileArtsSysDone == NULL) {
                mFileArtsSys = CDeviceFile::readFile(
                    mHeap, lbl_eu_804FFF2C + 0x24, battleWorkEvent(), 0, 0);
            }
            for (i = 0; i < 3; i++) {
                partyId = func_8009ECB0()[i + 1];
                if (partyId > 0) {
                    if (mFileArtsPcBusy[i] == NULL) {
                        faceId = (u8)partyId;
                        // Party slot 4 remaps to slot 12 while the 0x20
                        // system flag is under 0x21 and 0x3508 is unset.
                        if (faceId == 4 && (int)func_8009CF8C(0x20) < 0x21 &&
                            (int)func_8009CF8C(0x3508) == 0) {
                            faceId = 0xC;
                        }
                        sprintf(pathBuf178, lbl_eu_804FFF2C + 0x3D, faceId);
                        mFileArtsPc[i] = CDeviceFile::readFile(
                            mHeap, pathBuf178, battleWorkEvent(), 0, 0);
                    }
                    sprintf(pathBufF8, lbl_eu_804FFF2C + 0x59, partyId);
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

    inst = lbl_eu_80664048;
    if (inst != NULL && inst->unkE8 != 0 &&
        CMenuArtsSelect_isFinished() == 0) {
        cf::CfGameManager::getInstance();
        if (func_8006EF04__Fi(0x10000000) == 0) {
            player = cf::CfGameManager::getPlayer(0);
            if (player != NULL) {
                actor = static_cast<CUIBattleActor*>(func_8016FE34(NULL));
                vcallRes = *vslot<GetU32Fn>(actor->field_04, 0x30)(actor->field_04);
                if (func_80174C98(actor, &vcallRes, 0x803) != 0) {
                    if (CMenuArtsSelect_isCreated() == 0) {
                        handle = vslot<GetIntFn>(actor->field_3E9C, 0x4C)(
                            actor->field_3E9C);
                        lbl_eu_8066404C = (void*)handle;
                        if (lbl_eu_80664048 != NULL) {
                            retMarkB = func_80104210(lbl_eu_80664048->unk7C,
                                                     lbl_eu_80664048->unk58);
                            if (retMarkB != NULL) {
                                inst = lbl_eu_80664048;
                                i = 0;
                                while (i < inst->mChildList.mCapacity &&
                                       inst->mChildList.mList[i].mItem == NULL) {
                                    i++;
                                }
                                temp = &inst->mChildList.mList[i];
                                temp->setItem((CUIBattleChild*)retMarkB);
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
                        objPc = func_800BFC68(player);
                        handle = vslot<GetIntFn>(objPc, 0x4C)(objPc);
                        if (handle == 0) {
                            lbl_eu_8066404C = NULL;
                        } else {
                            obj = func_800B708C(handle);
                            if (obj != NULL &&
                                (((CUIBattleAction*)obj)->field_64 & 4) != 0) {
                                cf::CfGameManager::getInstance();
                                if (func_8006EF04__Fi(0x1000000) == 0 &&
                                    func_800FF738() == 0) {
                                    if (lbl_eu_8066404C != (void*)handle ||
                                        CMenuArtsSelect_isCreated() == 0) {
                                        lbl_eu_8066404C = (void*)handle;
                                        if (lbl_eu_80664048 != NULL) {
                                            retMarkA = func_80104210(
                                                lbl_eu_80664048->unk7C,
                                                lbl_eu_80664048->unk58);
                                            if (retMarkA != NULL) {
                                                inst = lbl_eu_80664048;
                                                i = 0;
                                                while (i < inst->mChildList.mCapacity &&
                                                       inst->mChildList.mList[i].mItem ==
                                                           NULL) {
                                                    i++;
                                                }
                                                temp = &inst->mChildList.mList[i];
                                                temp->setItem(
                                                    (CUIBattleChild*)retMarkA);
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

int func_8012E6DC() {
    CUIBattleManager* self = lbl_eu_80664048;
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
void CUIBattleManager::func_8012F29C() {
    // Swap the Move callback to the retail no-op pmf {0, -1, func_8012F2BC}.
    // (Reached via the pmf installed by __ct__CUIBattleManager.)
    mMoveFunc = lbl_eu_8052E0B4;
}
extern "C" void func_8012F2BC() {}
extern "C" void func_8012F860() {
    if (lbl_eu_80664048 != 0) {
        reinterpret_cast<unsigned char *>(lbl_eu_80664048)[0x82] |= 0x3b;
    }
}
// Retail func_8012F87C: remove-request processor. arg != 0 defers removal to
// a mark-all (unk81); arg == 0 unlinks every child from the list immediately.
// Do NOT cache the singleton in a local: retail re-spells lbl_eu_80664048 for
// the null check, every walk-condition test, and the final clear, and a cached
// local collapses those into one SDA21 load (reloc drift).
extern "C" void func_8012F87C(u32 arg) {
    _reslist_node<CUIBattleChild*>* removed[16];
    int count;
    _reslist_node<CUIBattleChild*>* node;

    if (lbl_eu_80664048 == NULL) {
        return;
    }
    if (arg != 0) {
        lbl_eu_80664048->unk81 = 1;
        return;
    }
    count = 0;
    node = lbl_eu_80664048->mChildList.mStartNodePtr->mNext;
    while (node != lbl_eu_80664048->mChildList.mStartNodePtr) {
        node->mItem->SetRemove();
        removed[count++] = node;
        node = node->mNext;
    }
    for (int i = 0; i < count; i++) {
        _reslist_node<CUIBattleChild*>* cur = removed[i];
        _reslist_node<CUIBattleChild*>* prev = cur->mPrev;
        _reslist_node<CUIBattleChild*>* next = cur->mNext;
        prev->mNext = next;
        next->mPrev = prev;
        cur->mNext = NULL;
    }
    // Final request-bit clear re-reads the singleton in retail.
    lbl_eu_80664048->unk82 = 0;
}
extern "C" unsigned char func_8012FA5C() { return lbl_eu_80664048 == 0 ? 0 : *(unsigned char *)((char *)lbl_eu_80664048 + 0xe8); }
extern "C" void* func_8012FA78() { void* p = lbl_eu_80664048; if (p == 0) return 0; if (*(unsigned char*)((char*)p + 0xe8) != 0) return (char*)p + 0xb8; return 0; }
extern "C" int func_8012FDBC() { if (lbl_eu_80664048 == 0) return -1; return *(int*)((char*)lbl_eu_80664048 + 0xec); }
extern "C" void Draw__Q216CUIBattleManager5CTestFv() {}
extern "C" void Move__Q216CUIBattleManager5CTestFv() {}
extern "C" void Term__Q216CUIBattleManager5CTestFv() {}
extern "C" void Init__Q216CUIBattleManager5CTestFv() {}
void __dt__16CUIBattleManagerFv(void*);
extern "C" void func_8012FF6C(void* p) {
    __dt__16CUIBattleManagerFv((char*)p - 0x54);
}

// noinline: the retail ctor is a real 0x1FC-byte function with a stripped
// (no length/params) symbol; without this the empty same-TU stub is IPA'd away
// and func_8012F558 loses its ctor call.
// Returns this (like retail), so callers can reuse r3 instead of preserving
// the pointer across the call in a callee-saved register.
// Body mirrors the retail store order exactly: interim/final vtables at 0x10,
// null move/draw PTMF triples into 0x3C..0x50 (each triple stores its middle
// word first), secondary vtable slots 0x54-0x58, child reslist at 0x5C with
// the sentinel node self-looped at 0x64, per-party resource slots zeroed in
// the retail interleaved pass order, then the node-pool allocation and the
// battle heap install.
extern "C" __declspec(noinline) CUIBattleManager* __ct__CUIBattleManager(
    CUIBattleManager* self_, CScnNw4r* pScene, mtl::ALLOC_HANDLE handle) {
    u32 movePmf0;
    u32 movePmf1;
    u32 drawPmf2;
    u32 hookPmf1;
    u32 hookPmf2;
    u32 zero;

    u32* const self = reinterpret_cast<u32*>(self_);
    zero = 0;
    __ct__8CProcessFv(reinterpret_cast<CProcess*>(self));
    // Interim CTTask<CUIBattleManager> vtable.
    reinterpret_cast<u32*>(self)[4] = reinterpret_cast<u32>(lbl_eu_8052E248);
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
    // Composite CUIBattleManager vtable + secondary sub-object slots.
    reinterpret_cast<u32*>(self)[4] = reinterpret_cast<u32>(lbl_eu_8052E0C0);
    reinterpret_cast<u32*>(self)[0x15] =
        reinterpret_cast<u32>(lbl_eu_8052E0C0) + 0x24;
    reinterpret_cast<u32*>(self)[0x16] = reinterpret_cast<u32>(pScene);
    // Child reslist at 0x5C: base vtable first, empty slot array/capacity,
    // sentinel node self-looped at 0x64, then the derived reslist vtable.
    reinterpret_cast<u32*>(self)[0x17] = reinterpret_cast<u32>(lbl_eu_8052E1B4);
    reinterpret_cast<u32*>(self)[0x1C] = zero;
    reinterpret_cast<u32*>(self)[0x1D] = zero;
    reinterpret_cast<u8*>(self)[0x78] = (u8)zero;
    {
        u32* sentinel = reinterpret_cast<u32*>(reinterpret_cast<char*>(self) + 0x64);
        reinterpret_cast<u32*>(self)[0x18] = reinterpret_cast<u32>(sentinel);
        sentinel[0] = reinterpret_cast<u32>(sentinel);
        sentinel[1] = reinterpret_cast<u32>(sentinel);
    }
    reinterpret_cast<u32*>(self)[0x17] = reinterpret_cast<u32>(lbl_eu_8052E19C);
    reinterpret_cast<u32*>(self)[0x1F] = zero;
    reinterpret_cast<u8*>(self)[0x80] = (u8)zero;
    reinterpret_cast<u8*>(self)[0x81] = (u8)zero;
    // Arc-handle/accessor slots.
    self_->mFileArtsElem = NULL;
    self_->mFileArtsElemDone = NULL;
    self_->mFileArtsSys = NULL;
    self_->mFileArtsSysDone = NULL;
    self_->unkE8 = 0;
    self_->unkE9 = 0;
    self_->mHeap = static_cast<mtl::ALLOC_HANDLE>(-1);
    // Per-party resource slots; MWCC unrolls this into the retail
    // three-pass interleaved store order (arts arc, busy accessor, face arc,
    // three face textures, face accessor per party).
    {
        int i;
        for (i = zero; i < 3; i++) {
            self_->mFileArtsPc[i] = NULL;
            self_->mFileArtsPcBusy[i] = NULL;
            self_->mFileFacePc[i] = NULL;
            self_->mFaceTex[i][0] = NULL;
            self_->mFaceTex[i][1] = NULL;
            self_->mFaceTex[i][2] = NULL;
            self_->mFaceAccessor[i] = NULL;
        }
    }
    self_->unk82 = (u8)(zero | 8);
    {
        // Child-node slot array (8 x 0xC nodes) from the scene's alloc region.
        u32* list = reinterpret_cast<u32*>(
            mtl::MemManager::allocate_array(0x60, func_80496004(pScene)));
        int i;
        self_->mChildList.mList = reinterpret_cast<_reslist_node<CUIBattleChild*>*>(list);
        for (i = 0; i < 8; i++) {
            list[i * 3] = zero;
        }
        self_->mChildList.mCapacity = 8;
    }
    // Initial move-hook pmf from the split1 pool (w1 word stored first).
    {
        u32* ip = reinterpret_cast<u32*>(lbl_eu_8052E0A8);
        movePmf0 = *ip++;   // w0
        hookPmf1 = *ip;     // w1
        hookPmf2 = *(ip + 1); // w2
    }
    reinterpret_cast<u32*>(self)[0x10] = hookPmf1;
    reinterpret_cast<u32*>(self)[0xF] = movePmf0;
    reinterpret_cast<u32*>(self)[0x11] = hookPmf2;
    mtl::MemManager::getHandleMEM2();
    reinterpret_cast<u32*>(self)[0x3B] = mtl::MemManager::create(
        mtl::MemManager::getHandleMEM1(), 0x44800, lbl_eu_804FFF2C);
    CMenuArtsSelect_clearArtsRef();
    lbl_eu_8066404C = reinterpret_cast<void*>(zero);
    return self_;
}

// Retail __dt__8012E534: battle-UI teardown helper (flat global symbol).
// Cancels pending file loads, destroys every arc accessor, marks the init
// child for removal, clears the child list, and releases the manager heap.
extern "C" void __dt__8012E534(CUIBattleManager* self) {
    int i;

    func_8012E630(self);
    for (i = 0; i < 3; i++) {
        self->mFaceTex[i][0] = NULL;
        self->mFaceTex[i][1] = NULL;
        self->mFaceTex[i][2] = NULL;
        func_80139124(self->mFaceAccessor[i]);
        func_80139124(self->mFileArtsPcBusy[i]);
    }
    func_80139124(self->mFileArtsSysDone);
    func_80139124(self->mFileArtsElemDone);
    reinterpret_cast<CUIBattleChild*>(self->unk7C)->SetRemove();
    // Unlink every child node without destroying items, then reset the list.
    // Retail re-spells mStartNodePtr on every access (no caching).
    {
        _reslist_node<CUIBattleChild*>* node =
            self->mChildList.mStartNodePtr->mNext;
        while (node != self->mChildList.mStartNodePtr) {
            _reslist_node<CUIBattleChild*>* prev = node;
            node = node->mNext;
            prev->mNext = NULL;
        }
        self->mChildList.mStartNodePtr->mNext =
            self->mChildList.mStartNodePtr;
        self->mChildList.mStartNodePtr->mPrev =
            self->mChildList.mStartNodePtr;
    }
    if (self->mChildList.unk1C == false && self->mChildList.mList != NULL) {
        delete[] self->mChildList.mList;
        self->mChildList.mList = NULL;
    }
    self->mChildList.mCapacity = 0;
    mtl::MemManager::erase(self->mHeap);
    self->mHeap = mtl::INVALID_HANDLE;
    lbl_eu_80664048 = NULL;
}

extern "C" void func_8012E630(CUIBattleManager* self) {
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

// ---------------------------------------------------------------------------
// reslist<IUIBattle> deleting destructors (retail __dt___reslist_base_IUIBattle
// / __dt__reslist_IUIBattle). The retail symbols use the old flat template
// mangling, so these are plain global functions (MWCC leaves `__`-prefixed
// names unmangled) over the ResListIUIBattle layout mirror.
// ---------------------------------------------------------------------------
void* __dt___reslist_base_IUIBattle(ResListIUIBattle* self, int mode) {
    if (self != NULL) {
        self->mVtable = (void*)lbl_eu_8052E1B4;
        // Walk the node chain nulling each next pointer (nodes are
        // pool-allocated, not freed here), then reset the sentinel.
        _reslist_node<IUIBattle>* node = self->mHead->mNext;
        while (node != self->mHead) {
            _reslist_node<IUIBattle>* cur = node;
            node = cur->mNext;
            cur->mNext = NULL;
        }
        self->mHead->mNext = self->mHead;
        self->mHead->mPrev = self->mHead;
        if (self->mOwnsList == 0 && self->mList != NULL) {
            __dla__FPv(self->mList);
            self->mList = NULL;
        }
        if (mode > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

void* __dt__reslist_IUIBattle(ResListIUIBattle* self, int mode) {
    // Doubled null check mirrors retail: MWCC keeps the dead second beq.
    if (self != NULL) {
        if (self != NULL) {
            self->mVtable = (void*)lbl_eu_8052E1B4;
            _reslist_node<IUIBattle>* node = self->mHead->mNext;
            while (self->mHead != node) {
                _reslist_node<IUIBattle>* cur = node;
                node = cur->mNext;
                cur->mNext = NULL;
            }
            self->mHead->mNext = self->mHead;
            self->mHead->mPrev = self->mHead;
            if (self->mOwnsList == 0 && self->mList != NULL) {
                __dla__FPv(self->mList);
                self->mList = NULL;
            }
        }
        if (mode > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

// File-event descriptor passed to func_8012F2C0 (OnFileEvent worker).
struct CUIBattleFileEvt {
    s32 unk00;              // 0x0 - completion kind (1 = party arts arc; retail compares SIGNED cmpi)
    CFileHandle* unk04;     // 0x4 - completed file handle
};

// Retail func_8012F2C0: dispatches a completed file load. Elem/sys arcs get an
// ArcResourceAccessor attached immediately; per-party arts arcs (kind 1) also
// pull the three face 'timg' textures out of the face accessor. getData()
// detaches the arc buffer (reads +4, stores NULL back).
int func_8012F2C0(CUIBattleManager* self, CUIBattleFileEvt* evt) {
    int i;
    u32 partyId;
    void* tex;
    nw4r::lyt::ArcResourceAccessor* acc;
    CFileHandle* fh;

    if (self->mFileArtsElem == evt->unk04) {
        void* buf = self->mFileArtsElem->getData();
        acc = createArcResourceAccessor__10CLibLayoutFv();
        self->mFileArtsElemDone = acc;
        acc->Attach(buf, &lbl_eu_804FFF2C[0x75]);
        self->mFileArtsElem = NULL;
        return 1;
    }
    if (self->mFileArtsSys == evt->unk04) {
        void* buf = self->mFileArtsSys->getData();
        acc = createArcResourceAccessor__10CLibLayoutFv();
        self->mFileArtsSysDone = acc;
        acc->Attach(buf, &lbl_eu_804FFF2C[0x75]);
        self->mFileArtsSys = NULL;
        return 1;
    }
    for (i = 0; i < 3; i++) {
        if (self->mFileArtsPc[i] == evt->unk04) {
            if (evt->unk00 == 1) {
                void* buf = self->mFileArtsPc[i]->getData();
                acc = createArcResourceAccessor__10CLibLayoutFv();
                self->mFileArtsPcBusy[i] = acc;
                acc->Attach(buf, &lbl_eu_804FFF2C[0x75]);
            }
            self->mFileArtsPc[i] = NULL;
            return 1;
        }
        if (self->mFileFacePc[i] != evt->unk04) {
            continue;
        }
        void* buf = self->mFileFacePc[i]->getData();
        self->mFileFacePc[i] = NULL;
        acc = createArcResourceAccessor__10CLibLayoutFv();
        self->mFaceAccessor[i] = acc;
        acc->Attach(buf, &lbl_eu_804FFF2C[0x75]);
        // Pointer-arithmetic spelling: reduces to retail's bare i*4
        // induction (add r3,base,i*4 / lwz r0,4(r3)) instead of an indexed
        // load.
        partyId = *(func_8009ECB0() + 1 + i);
        partyId &= 0xFF;
        if (partyId == 0) {
            continue;
        }
        tex = acc->GetResource(
            0x74696D67,
            func_80138F78(func_80136254(lbl_eu_80664090, &lbl_eu_804FFF2C[0x79],
                                        (int)(u8)partyId)),
            0);
        if (tex != NULL) {
            self->mFaceTex[i][0] = tex;
        }
        tex = acc->GetResource(
            0x74696D67,
            func_80138F78(func_80136254(lbl_eu_80664090, &lbl_eu_804FFF2C[0x84],
                                        (int)(u8)partyId)),
            0);
        if (tex != NULL) {
            self->mFaceTex[i][1] = tex;
        }
        tex = acc->GetResource(
            0x74696D67,
            func_80138F78(func_80136254(lbl_eu_80664090, &lbl_eu_804FFF2C[0x8f],
                                        (int)(u8)partyId)),
            0);
        if (tex != NULL) {
            self->mFaceTex[i][2] = tex;
        }
        return 1;
    }
    return 0;
}
CUIBattleManager* func_8012F558(CProcess* pParent, CScnNw4r* pScene,
                                mtl::ALLOC_HANDLE handle) {
    CUIBattleManager* mgr;

    if (lbl_eu_80664048 != NULL) {
        return lbl_eu_80664048;
    }
    if (handle == mtl::INVALID_HANDLE) {
        handle = mtl::MemManager::getHandleMEM2();
    }
    mgr = static_cast<CUIBattleManager*>(
        mtl::MemManager::allocate(sizeof(CUIBattleManager), CWorkThreadSystem::getWorkMem()));
    if (mgr != NULL) {
        mgr = __ct__CUIBattleManager(mgr, pScene, handle);
    }
    lbl_eu_80664048 = mgr;
    mgr->Regist(pParent, false);
    return lbl_eu_80664048;
}
// Retail func_8012F5F8: create/bind request for the main battle window.
// Returns the created child, or NULL (re-arming the unk82 bit 1 request on
// any failure path). Same savedRet stack-home idiom as func_8012F750.
extern "C" CUIBattleChild* func_8012F5F8() {
    void* savedRet;
    int i;
    int byteOff;
    int capacity;
    _reslist_node<CUIBattleChild*>* startNode;
    _reslist_node<CUIBattleChild*>* temp;
    CUIBattleManager* inst;

    if (lbl_eu_80664048 == NULL) {
        return NULL;
    }
    if (func_801355F4() == NULL) {
        lbl_eu_80664048->unk82 |= 1;
        return NULL;
    }
    if (lbl_eu_80664048->unkE8 == 0) {
        lbl_eu_80664048->unk82 |= 1;
        return NULL;
    }
    if (func_80164410() != 0) {
        lbl_eu_80664048->unk82 |= 1;
        return NULL;
    }
    lbl_eu_80664048->unk82 &= ~1u;
    savedRet = func_8010CDCC(lbl_eu_80664048->unk7C, lbl_eu_80664048->unk58);
    if (savedRet == NULL) {
        return NULL;
    }
    inst = lbl_eu_80664048;
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
    temp = (_reslist_node<CUIBattleChild*>*)((u8*)inst->mChildList.mList + i * 0xc);
    temp->setItem((CUIBattleChild*)savedRet);
    temp->mNext = startNode;
    temp->mPrev = startNode->mPrev;
    startNode->mPrev->mNext = temp;
    startNode->mPrev = temp;
    return (CUIBattleChild*)savedRet;
}

// Retail func_8012F750: create/bind request variant using bit 3 of unk82 and
// the func_801ACCE0 factory (no assets-ready gating, unlike func_8012F5F8).
// Same shape as the bit-2/bit-8 blocks in Move(): the call result lives in a
// function-top savedRet local so MWCC stack-homes it across the setItem
// try/catch region, matching the retail spill/reload/EH-save block.
extern "C" CUIBattleChild* func_8012F750(u32 arg) {
    void* savedRet;
    int i;
    int byteOff;
    int capacity;
    _reslist_node<CUIBattleChild*>* startNode;
    _reslist_node<CUIBattleChild*>* temp;
    CUIBattleManager* inst;

    if (lbl_eu_80664048 == NULL) {
        return NULL;
    }
    if (func_801355F4() == NULL) {
        lbl_eu_80664048->unk82 |= 8;
        return NULL;
    }
    lbl_eu_80664048->unk82 &= ~8u;
    savedRet = func_801ACCE0(lbl_eu_80664048->unk7C, lbl_eu_80664048->unk58);
    if (savedRet == NULL) {
        return NULL;
    }
    inst = lbl_eu_80664048;
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
    temp = (_reslist_node<CUIBattleChild*>*)((u8*)inst->mChildList.mList + i * 0xc);
    temp->setItem((CUIBattleChild*)savedRet);
    temp->mNext = startNode;
    temp->mPrev = startNode->mPrev;
    startNode->mPrev->mNext = temp;
    startNode->mPrev = temp;
    return (CUIBattleChild*)savedRet;
}

// Retail func_8012FAA8: full battle-UI release. Waits for draw done, cancels
// pending reads, removes the active move's mark window, then destroys the
// face/party accessors and re-arms the create request.
extern "C" void func_8012FAA8() {
    CDeviceVI::waitForDrawDone();
    if (lbl_eu_80664048 == NULL) {
        return;
    }
    func_8012E630(lbl_eu_80664048);
    {
        CUIBattleChild* matchItem = static_cast<CUIBattleChild*>(func_8010CE48());
        if (matchItem != NULL) {
            func_8010CF5C();
            {
                // Retail walks the child reslist with the condition checked
                // first, re-reading the singleton (and thus the sentinel)
                // every iteration.
                _reslist_node<CUIBattleChild*>* node =
                    lbl_eu_80664048->mChildList.mStartNodePtr->mNext;
                while (node != lbl_eu_80664048->mChildList.mStartNodePtr) {
                    if (node->mItem == matchItem) {
                        node->mItem->SetRemove();
                        _reslist_node<CUIBattleChild*>* prev = node->mPrev;
                        _reslist_node<CUIBattleChild*>* next = node->mNext;
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
                lbl_eu_80664048->mFaceTex[i][j] = NULL;
            }
            // Detach returns the arc buffer; deleting dtor frees the object,
            // then the detached buffer is released back to the heap. Retail
            // re-spells the singleton for every access so MWCC reloads the
            // SDA21 base each time.
            if (lbl_eu_80664048->mFaceAccessor[i] != NULL) {
                void* arcBuf = lbl_eu_80664048->mFaceAccessor[i]->Detach();
                if (lbl_eu_80664048->mFaceAccessor[i] != NULL) {
                    delete lbl_eu_80664048->mFaceAccessor[i];
                }
                lbl_eu_80664048->mFaceAccessor[i] = NULL;
                if (arcBuf != NULL) {
                    mtl::MemManager::deallocate(arcBuf);
                }
            }
            if (lbl_eu_80664048->mFileArtsPcBusy[i] != NULL) {
                void* arcBuf = lbl_eu_80664048->mFileArtsPcBusy[i]->Detach();
                if (lbl_eu_80664048->mFileArtsPcBusy[i] != NULL) {
                    delete lbl_eu_80664048->mFileArtsPcBusy[i];
                }
                lbl_eu_80664048->mFileArtsPcBusy[i] = NULL;
                if (arcBuf != NULL) {
                    mtl::MemManager::deallocate(arcBuf);
                }
            }
        }
    }
    lbl_eu_80664048->unkE9 = 0;
    lbl_eu_80664048->unkE8 = 0;
    func_8012F5F8();
}
// Check the arts-sys arc accessor (0x90) for a named 'timg' texture resource.
// Returns the resource pointer, or NULL if the accessor is not ready / missing.
void* func_8012FD04(const char* name) {
    nw4r::lyt::ArcResourceAccessor* acc = lbl_eu_80664048->mFileArtsSysDone;
    if (acc != NULL) {
        void* result = acc->GetResource(0x74696D67 /* 'timg' */, name, 0);
        if (result != NULL) {
            return result;
        }
    }
    return NULL;
}

// Same as func_8012FD04 but for the arts-elem arc accessor (0x88).
void* func_8012FD60(const char* name) {
    nw4r::lyt::ArcResourceAccessor* acc = lbl_eu_80664048->mFileArtsElemDone;
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
void* func_8012FC74(const char* name) {
    u8 i;
    for (i = 0; i < 3; i++) {
        nw4r::lyt::ArcResourceAccessor* acc = lbl_eu_80664048->mFileArtsPcBusy[i];
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

// This-adjusting thunk: subi r3, r3, 0x54; b func_8012F2C0
// MWCC does not generate this automatically for non-dtor virtual overrides.
// Accepting as-is; may need DECOMP_ASM_INSN for exact match.
extern "C" void OnFileEvent__16CUIBattleManagerFP10CEventFile(void* self, void* file) {
    func_8012F2C0(reinterpret_cast<CUIBattleManager*>((char*)self - 0x54),
                  static_cast<CUIBattleFileEvt*>(file));
}
