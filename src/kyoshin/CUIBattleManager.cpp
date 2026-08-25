#include "kyoshin/CUIBattleManager.hpp"

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
// return types for this import; hide one copy while including each header.
#define getInstance__Q22cf14CBattleManagerFv cuiBattleBmGetInstanceUnused
#include "kyoshin/cf/CfGameManager.hpp"
#undef getInstance__Q22cf14CBattleManagerFv
// CTaskGame.hpp's func_8049603C (CTaskGameCamView* return) clashes with
// CfGameManager.hpp's (UnkScnResult*) above; this TU calls neither - shield
// per the house convention (see CBattleManager.cpp / CfCam.cpp).
#define func_8049603C cuiBattleMgrCtaskGame9603CUnused
#include "kyoshin/CTaskGame.hpp"
#undef func_8049603C
#include "kyoshin/cf/object/CfObjectPc.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/util/MemManager.hpp"

#include "decomp.h"
#include "functions.hpp"
#include <cstdio>

struct CUIBattleInitProcess {
    u8 unk00[0x10];
    void* vtable;
    u8 unk14[0x28];
    u32 callbacks[6];
};

// 3-word null-pmf constant mirror (retail __ptmf_null). Struct access keeps
// MWCC on a single materialised base register instead of folding the offset-0
// word into `lwz @l(r5)` (which would add a reloc retail lacks).
struct CUIBattlePtmfWords {
    u32 w0; // 0x0
    u32 w1; // 0x4
    u32 w2; // 0x8
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
int func_80174C98(void* actor, int* outVal, int flags);
bool func_8006EF04__Fi(UNKWORD r3);
void* func_801355F4();
}

extern u32 func_801355A0();
extern cf::CfObjectPc* func_800BFC68(cf::CfObjectMove* objMove);

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
    u32 vtFinal;
    const CUIBattlePtmfWords* ptmf;
    u32 ptmfWord0;
    u32 ptmfWord1;
    u32 ptmfWord2;

    process = static_cast<CUIBattleInitProcess*>(
        mtl::MemManager::allocate(0x54, CWorkThreadSystem::getWorkMem()));
    if (process != NULL) {
        __ct__8CProcessFv(reinterpret_cast<CProcess*>(process));
        process->vtable = lbl_eu_8052E208;
        ptmf = reinterpret_cast<const CUIBattlePtmfWords*>(__ptmf_null);
        ptmfWord1 = ptmf->w1;
        vtFinal = reinterpret_cast<u32>(lbl_eu_8052E1C0);
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
        process->vtable = reinterpret_cast<void*>(vtFinal);
    }
    unk7C = reinterpret_cast<CProcess*>(process);
    reinterpret_cast<CProcess*>(process)->Regist(lbl_eu_80664048, false);
}

void CUIBattleManager::Move() {
    // Retail frame -0x220 / stmw r25 / mr r31,r1. Per-site savedRet homes at
    // 0x20..0x8 and setItem try/catch force the exception frame.
    void* savedRet20;
    void* savedRet1C;
    void* savedRet18;
    void* savedRet14;
    void* savedRet10;
    void* savedRet0C;
    void* savedRet08;
    int pad0C;
    int pad10;
    int pad14;
    int pad18;
    int pad1C;
    int pad20;
    _reslist_node<CUIBattleChild*>* pending[8];
    char pathBufF8[0x80];
    char pathBuf178[0x8C];
    // Grow frame toward retail -0x220 (spare between pending and paths).
    int framePad[8];
    int localVal;
    int pendingCount;
    int i;
    int byteOff;
    int capacity;
    _reslist_node<CUIBattleChild*>* startNode;
    _reslist_node<CUIBattleChild*>* temp;
    _reslist_node<CUIBattleChild*>* node;
    _reslist_node<CUIBattleChild*>* prev;
    _reslist_node<CUIBattleChild*>* next;
    _reslist_node<CUIBattleChild*>* it;
    _reslist_node<CUIBattleChild*>* walk;
    CUIBattleManager* inst;
    void* actor2;
    cf::CfObjectMove* objMove;
    cf::CfObjectPc* objPc;
    int handle;
    void* subObj;
    int partyId;
    u8 faceId;
    int n;
    void* flagObj;
    void* obj;
    u8 flags;

    CTaskGame::getInstance();
    if (CTaskGame::func_800426F0()) {
        goto done;
    }
    if ((lbl_eu_80663E28 & (1u << 21)) == 0) {
        goto after_bit21;
    }
    DECOMP_ASM_INSN_BEGIN
    asm {
        b done
    }
    DECOMP_ASM_INSN_END
after_bit21:

    mtl::MemManager::getHandleMEM2();

    if ((unk82 & 1) != 0) {
        if (func_801355A0() != 0) {
            flags = unk82;
            flags &= 0xfe;
            unk82 = flags;
            {
                void (*const callF5F8)(void) =
                    reinterpret_cast<void (*)(void)>(&func_8012F5F8);
                callF5F8();
            }
        }
    }

    if ((unk82 & 2) != 0) {
        if (func_801355A0() != 0) {
            flags = unk82;
            flags &= 0xfd;
            unk82 = flags;
            if (lbl_eu_80664048 != NULL) {
                if (func_801355F4() == NULL) {
                    lbl_eu_80664048->unk82 |= 2;
                } else {
                    flags = lbl_eu_80664048->unk82;
                    flags &= 0xfd;
                    lbl_eu_80664048->unk82 = flags;
                    savedRet20 = func_801096B8(lbl_eu_80664048->unk7C, lbl_eu_80664048->unk58);
                    if (savedRet20 != NULL) {
                        inst = lbl_eu_80664048;
                        i = 0;
                        byteOff = 0;
                        startNode = inst->mChildList.mStartNodePtr;
                        capacity = inst->mChildList.mCapacity;
                        pad0C = capacity;
                        pad10 = byteOff;
                        pad14 = i;
                        pad18 = pad0C;
                        pad1C = pad10;
                        pad20 = pad14;
                        goto slot_check_2;
                    slot_body_2:
                        if (*(u32*)((u8*)inst->mChildList.mList + byteOff) == 0) {
                            goto slot_found_2;
                        }
                        byteOff += 0xc;
                        i++;
                    slot_check_2:
                        if (i < capacity) {
                            goto slot_body_2;
                        }
                    slot_found_2:
                        temp = (_reslist_node<CUIBattleChild*>*)((u8*)inst->mChildList.mList +
                                                                i * 0xc);
                        temp->setItem((CUIBattleChild*)savedRet20);
                        temp->mNext = startNode;
                        temp->mPrev = startNode->mPrev;
                        startNode->mPrev->mNext = temp;
                        startNode->mPrev = temp;
                    }
                }
            }
        }
    }

    if ((unk82 & 8) != 0) {
        if (func_801355A0() != 0) {
            flags = unk82;
            flags &= 0xf7;
            unk82 = flags;
            if (lbl_eu_80664048 != NULL) {
                if (func_801355F4() == NULL) {
                    lbl_eu_80664048->unk82 |= 8;
                } else {
                    flags = lbl_eu_80664048->unk82;
                    flags &= 0xf7;
                    lbl_eu_80664048->unk82 = flags;
                    savedRet1C = func_801ACCE0(lbl_eu_80664048->unk7C, lbl_eu_80664048->unk58);
                    if (savedRet1C != NULL) {
                        inst = lbl_eu_80664048;
                        i = 0;
                        byteOff = 0;
                        capacity = inst->mChildList.mCapacity;
                        startNode = inst->mChildList.mStartNodePtr;
                        pad0C = capacity;
                        pad10 = byteOff;
                        pad14 = i;
                        pad18 = pad0C;
                        pad1C = pad10;
                        pad20 = pad14;
                        goto slot_check_8;
                    slot_body_8:
                        if (*(u32*)((u8*)inst->mChildList.mList + byteOff) == 0) {
                            goto slot_found_8;
                        }
                        byteOff += 0xc;
                        i++;
                    slot_check_8:
                        if (i < capacity) {
                            goto slot_body_8;
                        }
                    slot_found_8:
                        temp = (_reslist_node<CUIBattleChild*>*)((u8*)inst->mChildList.mList +
                                                                i * 0xc);
                        temp->setItem((CUIBattleChild*)savedRet1C);
                        temp->mNext = startNode;
                        temp->mPrev = startNode->mPrev;
                        startNode->mPrev->mNext = temp;
                        startNode->mPrev = temp;
                    }
                }
            }
        }
    }

    if ((unk82 & 0x10) != 0) {
        if (func_801355A0() != 0) {
            flags = unk82;
            flags &= 0xef;
            unk82 = flags;
            if (lbl_eu_80664048 != NULL) {
                // Retail: cntlzw + srwi zero-test.
#undef __cntlzw
                {
                    u32 cf8cVal = (u32)func_8009CF8C(0x3357);
                    u32 cntlz = (u32)__cntlzw(cf8cVal);
                    if ((cntlz >> 5) != 0) {
                        lbl_eu_80664048->unk82 |= 0x10;
                        goto after_bit10;
                    }
                }
                if (func_801355F4() == NULL) {
                    lbl_eu_80664048->unk82 |= 0x10;
                } else {
                    flags = lbl_eu_80664048->unk82;
                    flags &= 0xef;
                    lbl_eu_80664048->unk82 = flags;
                    savedRet18 = func_80187694(lbl_eu_80664048->unk7C, lbl_eu_80664048->unk58);
                    if (savedRet18 != NULL) {
                        inst = lbl_eu_80664048;
                        i = 0;
                        byteOff = 0;
                        capacity = inst->mChildList.mCapacity;
                        startNode = inst->mChildList.mStartNodePtr;
                        pad0C = capacity;
                        pad10 = byteOff;
                        pad14 = i;
                        pad18 = pad0C;
                        pad1C = pad10;
                        pad20 = pad14;
                        goto slot_check_10;
                    slot_body_10:
                        if (*(u32*)((u8*)inst->mChildList.mList + byteOff) == 0) {
                            goto slot_found_10;
                        }
                        byteOff += 0xc;
                        i++;
                    slot_check_10:
                        if (i < capacity) {
                            goto slot_body_10;
                        }
                    slot_found_10:
                        temp = (_reslist_node<CUIBattleChild*>*)((u8*)inst->mChildList.mList +
                                                                i * 0xc);
                        temp->setItem((CUIBattleChild*)savedRet18);
                        temp->mNext = startNode;
                        temp->mPrev = startNode->mPrev;
                        startNode->mPrev->mNext = temp;
                        startNode->mPrev = temp;
                    }
                }
            }
        }
    }

after_bit10:
    if ((unk82 & 0x20) != 0) {
        if (func_801355A0() != 0) {
            flags = unk82;
            flags &= 0xdf;
            unk82 = flags;
            if (lbl_eu_80664048 != NULL) {
                if (func_801355F4() == NULL) {
                    lbl_eu_80664048->unk82 |= 0x20;
                } else {
                    flags = lbl_eu_80664048->unk82;
                    flags &= 0xdf;
                    lbl_eu_80664048->unk82 = flags;
                    savedRet14 = func_801B0E0C(lbl_eu_80664048->unk7C, lbl_eu_80664048->unk58);
                    if (savedRet14 != NULL) {
                        inst = lbl_eu_80664048;
                        i = 0;
                        byteOff = 0;
                        capacity = inst->mChildList.mCapacity;
                        startNode = inst->mChildList.mStartNodePtr;
                        pad0C = capacity;
                        pad10 = byteOff;
                        pad14 = i;
                        pad18 = pad0C;
                        pad1C = pad10;
                        pad20 = pad14;
                        goto slot_check_20;
                    slot_body_20:
                        if (*(u32*)((u8*)inst->mChildList.mList + byteOff) == 0) {
                            goto slot_found_20;
                        }
                        byteOff += 0xc;
                        i++;
                    slot_check_20:
                        if (i < capacity) {
                            goto slot_body_20;
                        }
                    slot_found_20:
                        temp = (_reslist_node<CUIBattleChild*>*)((u8*)inst->mChildList.mList +
                                                                i * 0xc);
                        temp->setItem((CUIBattleChild*)savedRet14);
                        temp->mNext = startNode;
                        temp->mPrev = startNode->mPrev;
                        startNode->mPrev->mNext = temp;
                        startNode->mPrev = temp;
                    }
                }
            }
        }
    }

    if (unkE9 == 0 && func_80164410() == 0) {
        if (cf::CfGameManager::getPlayer(0) != NULL) {
            if (mFileArtsElemDone == NULL) {
                mFileArtsElem =
                    CDeviceFile::readFile(mHeap, lbl_eu_804FFF2C + 0xa, battleWorkEvent(), 0, 0);
            }
            if (mFileArtsSysDone == NULL) {
                mFileArtsSys =
                    CDeviceFile::readFile(mHeap, lbl_eu_804FFF2C + 0x24, battleWorkEvent(), 0, 0);
            }

            n = 0;
            for (; n < 3; n++) {
                partyId = func_8009ECB0()[n + 1];
                if (partyId > 0) {
                    if (mFileArtsPcBusy[n] == NULL) {
                        faceId = (u8)partyId;
                        if (faceId == 4) {
                            if ((int)(u32)func_8009CF8C(0x20) < 0x21) {
                                if ((int)(u32)func_8009CF8C(0x3508) == 0) {
                                    faceId = 0xc;
                                }
                            }
                        }
                        sprintf(pathBuf178, lbl_eu_804FFF2C + 0x3d, (unsigned)faceId);
                        mFileArtsPc[n] =
                            CDeviceFile::readFile(mHeap, pathBuf178, battleWorkEvent(), 0, 0);
                    }
                    sprintf(pathBufF8, lbl_eu_804FFF2C + 0x59, partyId);
                    mFileFacePc[n] =
                        CDeviceFile::readFile(mHeap, pathBufF8, battleWorkEvent(), 0, 0);
                }
            }
            unkE9 = 1;
            goto after_assets;
        }
    } else {
        if (unkE8 == 0) {
            unkE8 = 1;
            if (mFileArtsElem != NULL) {
                unkE8 = 0;
            }
            if (mFileArtsSys != NULL) {
                unkE8 = 0;
            }
            n = 0;
            for (; n < 3; n++) {
                if (mFileArtsPc[n] != NULL) {
                    unkE8 = 0;
                    goto after_assets;
                }
                if (mFileFacePc[n] != NULL) {
                    unkE8 = 0;
                    goto after_assets;
                }
            }
        }
    }
after_assets:

    if (lbl_eu_80664048->unkE8 != 0) {
        if (func_801042A4() == 0) {
            cf::CfGameManager::getInstance();
            if (func_8006EF04__Fi(0x10000000) == 0) {
                objMove = cf::CfGameManager::getPlayer(0);
                if (objMove != NULL) {
                    actor2 = func_8016FE34(NULL);
                    subObj = *(void**)((u8*)actor2 + 4);
                    {
                        u32* ret = vslot<GetU32Fn>(subObj, 0x30)(subObj);
                        localVal = *ret;
                    }
                    if (func_80174C98(actor2, &localVal, 0x803) == 0) {
                        flagObj = *(void**)((u8*)actor2 + 0x3f60);
                        if (flagObj != NULL) {
                            if ((*(u16*)((u8*)flagObj + 0x530) & 1) == 0) {
                                objPc = func_800BFC68(objMove);
                                {
                                    void* moveBase = static_cast<cf::CfObjectMove*>(static_cast<cf::CfObjectActor*>(objPc));
                                    handle = vslot<GetIntFn>(moveBase, 0x4c)(moveBase);
                                }
                                if (handle != 0) {
                                    obj = func_800B708C((BOOL)handle);
                                    if (obj != NULL) {
                                        if ((*(u32*)((u8*)obj + 0x64) & 4) != 0) {
                                            cf::CfGameManager::getInstance();
                                            if (func_8006EF04__Fi(0x1000000) == 0) {
                                                if (func_800FF738() == 0) {
                                                    if (lbl_eu_8066404C == (void*)handle) {
                                                        if (func_801042B4() != 0) {
                                                            goto mark_remove;
                                                        }
                                                    }
                                                    lbl_eu_8066404C = (void*)handle;
                                                    if (lbl_eu_80664048 != NULL) {
                                                        savedRet0C = func_80104210(
                                                            lbl_eu_80664048->unk7C,
                                                            lbl_eu_80664048->unk58);
                                                        if (savedRet0C != NULL) {
                                                            inst = lbl_eu_80664048;
                                                            i = 0;
                                                            byteOff = 0;
                                                            capacity = inst->mChildList.mCapacity;
                                                            startNode =
                                                                inst->mChildList.mStartNodePtr;
                                                            pad0C = capacity;
                                                            pad10 = byteOff;
                                                            pad14 = i;
                                                            pad18 = pad0C;
                                                            pad1C = pad10;
                                                            pad20 = pad14;
                                                            goto slot_check_a;
                                                        slot_body_a:
                                                            if (*(u32*)((u8*)inst->mChildList.mList +
                                                                        byteOff) == 0) {
                                                                goto slot_found_a;
                                                            }
                                                            byteOff += 0xc;
                                                            i++;
                                                        slot_check_a:
                                                            if (i < capacity) {
                                                                goto slot_body_a;
                                                            }
                                                        slot_found_a:
                                                            temp = (_reslist_node<CUIBattleChild*>*)(
                                                                (u8*)inst->mChildList.mList +
                                                                i * 0xc);
                                                            temp->setItem(
                                                                (CUIBattleChild*)savedRet0C);
                                                            temp->mNext = startNode;
                                                            temp->mPrev = startNode->mPrev;
                                                            startNode->mPrev->mNext = temp;
                                                            startNode->mPrev = temp;
                                                        }
                                                    }
                                                    goto mark_remove;
                                                }
                                            }
                                        }
                                    }
                                    goto mark_remove;
                                }
                                lbl_eu_8066404C = NULL;
                                goto mark_remove;
                            }
                        }
                        goto mark_remove;
                    }
                    if (func_801042B4() == 0) {
                        {
                            void* moveBase = static_cast<cf::CfObjectMove*>(static_cast<cf::CfObjectActor*>((cf::CfObjectActor*)actor2));
                            handle = vslot<GetIntFn>(moveBase, 0x4c)(moveBase);
                        }
                        lbl_eu_8066404C = (void*)handle;
                        if (lbl_eu_80664048 != NULL) {
                            savedRet08 = func_80104210(lbl_eu_80664048->unk7C,
                                                       lbl_eu_80664048->unk58);
                            if (savedRet08 != NULL) {
                                inst = lbl_eu_80664048;
                                i = 0;
                                byteOff = 0;
                                capacity = inst->mChildList.mCapacity;
                                startNode = inst->mChildList.mStartNodePtr;
                                pad0C = capacity;
                                pad10 = byteOff;
                                pad14 = i;
                                pad18 = pad0C;
                                pad1C = pad10;
                                pad20 = pad14;
                                goto slot_check_b;
                            slot_body_b:
                                if (*(u32*)((u8*)inst->mChildList.mList + byteOff) == 0) {
                                    goto slot_found_b;
                                }
                                byteOff += 0xc;
                                i++;
                            slot_check_b:
                                if (i < capacity) {
                                    goto slot_body_b;
                                }
                            slot_found_b:
                                temp = (_reslist_node<CUIBattleChild*>*)(
                                    (u8*)inst->mChildList.mList + i * 0xc);
                                temp->setItem((CUIBattleChild*)savedRet08);
                                temp->mNext = startNode;
                                temp->mPrev = startNode->mPrev;
                                startNode->mPrev->mNext = temp;
                                startNode->mPrev = temp;
                            }
                        }
                    }
                }
            }
        }
    }

mark_remove:
    {
        _reslist_node<CUIBattleChild*>* sentinel = mChildList.mStartNodePtr;
        walk = sentinel->mNext;
        for (it = walk; it != sentinel; it = it->mNext) {
            if (it->mItem->unk55 != 0 || unk81 != 0) {
                for (; walk != sentinel; walk = walk->mNext) {
                    walk->mItem->unk55 = 1;
                }
                break;
            }
        }
    }

    pendingCount = 0;
    for (node = mChildList.mStartNodePtr->mNext; node != mChildList.mStartNodePtr;
         node = node->mNext) {
        CUIBattleChild* child = node->mItem;
        if (child->unk54 != 0 || unk80 != 0) {
            child->SetRemove();
            pending[pendingCount++] = node;
        }
    }
    for (i = 0; i < pendingCount; i++) {
        prev = pending[i]->mPrev;
        next = pending[i]->mNext;
        prev->mNext = next;
        next->mPrev = prev;
        pending[i]->mNext = NULL;
    }

    unk80 = 0;
    unk81 = 0;
    framePad[0] = pendingCount;
done:;
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
extern "C" void func_8012F87C(u32 arg) {
    CUIBattleManager* inst = lbl_eu_80664048;
    _reslist_node<CUIBattleChild*>* removed[16];
    int count = 0;
    int i;

    if (inst == NULL) {
        return;
    }
    if (arg != 0) {
        inst->unk81 = 1;
        return;
    }
    {
        _reslist_node<CUIBattleChild*>* sentinel =
            inst->mChildList.mStartNodePtr;
        _reslist_node<CUIBattleChild*>* node;
        for (node = sentinel->mNext; node != sentinel; node = node->mNext) {
            node->mItem->SetRemove();
            removed[count++] = node;
        }
        if (count > 0) {
            for (i = 0; i < count; i++) {
                removed[i]->mPrev->mNext = removed[i]->mNext;
                removed[i]->mNext->mPrev = removed[i]->mPrev;
                removed[i]->mNext = NULL;
            }
        }
    }
    // Final request-bit clear goes through the global again in retail.
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
extern "C" __declspec(noinline) void __ct__CUIBattleManager(CUIBattleManager* self,
                                                 CScnNw4r* pScene,
                                                 mtl::ALLOC_HANDLE handle) {
    u32* p = reinterpret_cast<u32*>(self);
    const u32* src;

    __ct__8CProcessFv(reinterpret_cast<CProcess*>(self));
    // Interim CTTask<CUIBattleManager> vtable at 0x10, overwritten by the final
    // CUIBattleManager vtable once the base hooks are in place.
    p[4] = reinterpret_cast<u32>(lbl_eu_8052E248);
    // Null move/draw hook pmfs (0x3C..0x50); retail stores the w1 word first.
    src = __ptmf_null;
    p[0x10] = src[1];
    p[0xF] = src[0];
    p[0x11] = src[2];
    p[0x13] = src[4];
    p[0x12] = src[3];
    p[0x14] = src[5];
    p[4] = reinterpret_cast<u32>(lbl_eu_8052E0C0);
    p[0x15] = reinterpret_cast<u32>(lbl_eu_8052E0C0) + 0x24;
    p[0x16] = reinterpret_cast<u32>(pScene);
    // reslist<CUIBattleChild*> head: base vtable, empty slot array, sentinel
    // node self-looped at 0x64, then the derived reslist vtable overwrite.
    p[0x17] = reinterpret_cast<u32>(lbl_eu_8052E1B4);
    p[0x1C] = 0;
    p[0x1D] = 0;
    reinterpret_cast<u8*>(p)[0x78] = 0;
    p[0x18] = reinterpret_cast<u32>(p) + 0x64;
    p[0x19] = p[0x18];
    p[0x1A] = p[0x18];
    p[0x17] = reinterpret_cast<u32>(lbl_eu_8052E19C);
    p[0x1F] = 0;
    reinterpret_cast<u8*>(p)[0x80] = 0;
    reinterpret_cast<u8*>(p)[0x81] = 0;
    p[0x21] = 0;
    p[0x22] = 0;
    p[0x23] = 0;
    p[0x24] = 0;
    reinterpret_cast<u8*>(p)[0xE8] = 0;
    reinterpret_cast<u8*>(p)[0xE9] = 0;
    p[0x3B] = 0xFFFFFFFF;
    // Per-party resource slots, unrolled in retail store order (per index:
    // arts arc, busy accessor, face arc, 3 face textures, face accessor).
    p[0x25] = 0;
    p[0x28] = 0;
    p[0x2B] = 0;
    p[0x2E] = 0;
    p[0x2F] = 0;
    p[0x30] = 0;
    p[0x37] = 0;
    p[0x26] = 0;
    p[0x29] = 0;
    p[0x2C] = 0;
    p[0x31] = 0;
    p[0x32] = 0;
    p[0x33] = 0;
    p[0x38] = 0;
    p[0x27] = 0;
    p[0x2A] = 0;
    p[0x2D] = 0;
    p[0x34] = 0;
    p[0x35] = 0;
    p[0x36] = 0;
    p[0x39] = 0;
    reinterpret_cast<u8*>(p)[0x82] = 8;
    {
        // Child-node slot array (8 x 0xC nodes) from the scene's alloc region.
        u32* list = reinterpret_cast<u32*>(
            mtl::MemManager::allocate_array(0x60, func_80496004(pScene)));
        p[0x1C] = reinterpret_cast<u32>(list);
        list[0] = 0;
        list[3] = 0;
        list[6] = 0;
        list[9] = 0;
        list[12] = 0;
        list[15] = 0;
        list[18] = 0;
        list[21] = 0;
        p[0x1D] = 8;
    }
    // Initial move-hook pmf from the split1 pool (w1 word stored first).
    src = reinterpret_cast<const u32*>(lbl_eu_8052E0A8);
    p[0x10] = src[1];
    p[0xF] = src[0];
    p[0x11] = src[2];
    mtl::MemManager::getHandleMEM2();
    p[0x3B] = mtl::MemManager::create(mtl::MemManager::getHandleMEM1(),
                                      0x44800, lbl_eu_804FFF2C);
    CMenuArtsSelect_clearArtsRef();
    lbl_eu_8066404C = NULL;
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
        self->mVtable = lbl_eu_8052E1B4;
        {
            _reslist_node<IUIBattle>* cur = self->mHead->mNext;
            while (cur != self->mHead) {
                _reslist_node<IUIBattle>* prev = cur;
                cur = cur->mNext;
                prev->mNext = NULL;
            }
            self->mHead->mNext = self->mHead;
            self->mHead->mPrev = self->mHead;
        }
        if (self->mOwnsList == 0 && self->mList != NULL) {
            delete[] self->mList;
            self->mList = NULL;
        }
    }
    if (mode > 0) {
        delete self;
    }
    return self;
}

void* __dt__reslist_IUIBattle(ResListIUIBattle* self, int mode) {
    // Doubled null check mirrors retail: MWCC keeps the dead second beq.
    if (self != NULL) {
        if (self != NULL) {
            self->mVtable = lbl_eu_8052E1B4;
            {
                _reslist_node<IUIBattle>* cur = self->mHead->mNext;
                while (cur != self->mHead) {
                    _reslist_node<IUIBattle>* prev = cur;
                    cur = cur->mNext;
                    prev->mNext = NULL;
                }
                self->mHead->mNext = self->mHead;
                self->mHead->mPrev = self->mHead;
            }
            if (self->mOwnsList == 0 && self->mList != NULL) {
                delete[] self->mList;
                self->mList = NULL;
            }
        }
    }
    if (mode > 0) {
        delete self;
    }
    return self;
}

// File-event descriptor passed to func_8012F2C0 (OnFileEvent worker).
struct CUIBattleFileEvt {
    u32 unk00;              // 0x0 - completion kind (1 = party arts arc)
    CFileHandle* unk04;     // 0x4 - completed file handle
};

// Retail func_8012F2C0: dispatches a completed file load. Elem/sys arcs get an
// ArcResourceAccessor attached immediately; per-party arts arcs (kind 1) also
// pull the three face 'timg' textures out of the face accessor.
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
        partyId = func_8009ECB0()[i + 1];
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
        __ct__CUIBattleManager(mgr, pScene, handle);
    }
    lbl_eu_80664048 = mgr;
    mgr->Regist(pParent, false);
    return lbl_eu_80664048;
}
// Retail func_8012F5F8: create/bind request for the main battle window.
// Returns the created child, or NULL (re-arming the unk82 bit 1 request on
// any failure path).
extern "C" CUIBattleChild* func_8012F5F8() {
    CUIBattleManager* inst;
    CUIBattleChild* child;

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
    inst = lbl_eu_80664048;
    child = static_cast<CUIBattleChild*>(
        func_8010CDCC(inst->unk7C, inst->unk58));
    if (child == NULL) {
        return NULL;
    }
    lbl_eu_80664048->mChildList.push_back(child);
    return child;
}

// Retail func_8012F750: create/bind request variant using bit 3 of unk82 and
// the func_801ACCE0 factory (no assets-ready gating, unlike func_8012F5F8).
extern "C" CUIBattleChild* func_8012F750(u32 arg) {
    CUIBattleManager* inst;
    CUIBattleChild* child;

    if (lbl_eu_80664048 == NULL) {
        return NULL;
    }
    if (func_801355F4() == NULL) {
        lbl_eu_80664048->unk82 |= 8;
        return NULL;
    }
    lbl_eu_80664048->unk82 &= ~8u;
    inst = lbl_eu_80664048;
    child = static_cast<CUIBattleChild*>(
        func_801ACCE0(inst->unk7C, inst->unk58));
    if (child == NULL) {
        return NULL;
    }
    lbl_eu_80664048->mChildList.push_back(child);
    return child;
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
                _reslist_node<CUIBattleChild*>* sentinel =
                    lbl_eu_80664048->mChildList.mStartNodePtr;
                _reslist_node<CUIBattleChild*>* node = sentinel->mNext;
                while (node != sentinel) {
                    if (node->mItem == matchItem) {
                        _reslist_node<CUIBattleChild*>* next = node->mNext;
                        node->mItem->SetRemove();
                        node->mPrev->mNext = next;
                        next->mPrev = node->mPrev;
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

// This-adjusting thunk: subi r3, r3, 0x54; b func_8012F2C0
// MWCC does not generate this automatically for non-dtor virtual overrides.
// Accepting as-is; may need DECOMP_ASM_INSN for exact match.
extern "C" void OnFileEvent__16CUIBattleManagerFP10CEventFile(void* self, void* file) {
    func_8012F2C0(reinterpret_cast<CUIBattleManager*>((char*)self - 0x54),
                  static_cast<CUIBattleFileEvt*>(file));
}
