#include "kyoshin/CUIBattleManager.hpp"

#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/object/CfObjectPc.hpp"
#include "monolib/device/CDeviceFile.hpp"
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

extern "C" {
CUIBattleManager* lbl_eu_80664048;
void* lbl_eu_8066404C;
char lbl_eu_8052E208[];
char lbl_eu_8052E1C0[];
char lbl_eu_804FFF2C[];
u32 __ptmf_null[3];
u32 lbl_eu_80663E28;
void __ct__8CProcessFv(CProcess*);

void* func_801096B8(void* proc, void* scn);
void* func_801ACCE0(void* proc, void* scn);
void* func_80187694(void* proc, void* scn);
void* func_801B0E0C(void* proc, void* scn);
void* func_80104210(void* proc, void* scn);
void func_8012F5F8();
int func_80164410();
int func_801042A4();
int func_801042B4();
int func_800FF738();
void* func_8016FE34();
u32 func_80174C98(void* actor, u32* outVal, u32 flags);
UNKWORD func_8006EF04(UNKWORD r3);
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
    IWorkEvent* workEvent = reinterpret_cast<IWorkEvent*>(self);
    if (self != NULL) {
        workEvent = reinterpret_cast<IWorkEvent*>(reinterpret_cast<u8*>(self) + 0x54);
    }
    return workEvent;
}

void CUIBattleManager::Init() {
    CUIBattleInitProcess* process;
    u8* ptmfBase;
    char* vtFinal;
    u32 ptmfWord1;
    u32 ptmfWord0;
    u32 ptmfWord2;

    process = static_cast<CUIBattleInitProcess*>(
        mtl::MemManager::allocate(0x54, CWorkThreadSystem::getWorkMem()));
    if (process != NULL) {
        __ct__8CProcessFv(reinterpret_cast<CProcess*>(process));
        ptmfBase = (u8*)__ptmf_null;
        process->vtable = lbl_eu_8052E208;
        ptmfWord1 = *(u32*)(ptmfBase + 4);
        vtFinal = lbl_eu_8052E1C0;
        ptmfWord0 = *(u32*)(ptmfBase + 0);
        process->callbacks[0] = ptmfWord0;
        process->callbacks[1] = ptmfWord1;
        ptmfWord2 = *(u32*)(ptmfBase + 8);
        process->callbacks[2] = ptmfWord2;
        ptmfWord1 = *(u32*)(ptmfBase + 4);
        ptmfWord0 = *(u32*)(ptmfBase + 0);
        process->callbacks[3] = ptmfWord0;
        process->callbacks[4] = ptmfWord1;
        ptmfWord2 = *(u32*)(ptmfBase + 8);
        process->callbacks[5] = ptmfWord2;
        process->vtable = vtFinal;
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
    int framePad[4];
    u32 localVal;
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
    u8* slotBase;
    int off;
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
            func_8012F5F8();
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
                        startNode = inst->mChildList.mStartNodePtr;
                        capacity = inst->mChildList.mCapacity;
                        pad18 = capacity;
                        pad1C = byteOff;
                        pad20 = i;
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
                // Retail: cntlzw + srwi. zero-test. compat.h stubs __cntlzw to 0.
#undef __cntlzw
                if (__cntlzw((u32)func_8009CF8C(0x3357)) >> 5) {
                    lbl_eu_80664048->unk82 |= 0x10;
                    goto after_bit10;
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
                        startNode = inst->mChildList.mStartNodePtr;
                        capacity = inst->mChildList.mCapacity;
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
                        startNode = inst->mChildList.mStartNodePtr;
                        capacity = inst->mChildList.mCapacity;
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
        if (cf::CfGameManager::func_80082D54(0) != NULL) {
            if (mFileArtsElemDone == NULL) {
                mFileArtsElem =
                    CDeviceFile::readFile(mHeap, lbl_eu_804FFF2C + 0xa, battleWorkEvent(this), 0, 0);
            }
            if (mFileArtsSysDone == NULL) {
                mFileArtsSys =
                    CDeviceFile::readFile(mHeap, lbl_eu_804FFF2C + 0x24, battleWorkEvent(this), 0, 0);
            }

            slotBase = reinterpret_cast<u8*>(this);
            off = 0;
            n = 0;
            for (; n < 3; n++) {
                partyId = *(int*)((u8*)func_8009ECB0() + off + 4);
                if (partyId > 0) {
                    if (*(CFileHandle**)(slotBase + 0xa0) == NULL) {
                        faceId = (u8)partyId;
                        if (faceId == 4) {
                            if ((int)(u32)func_8009CF8C(0x20) < 0x21) {
                                if ((int)(u32)func_8009CF8C(0x3508) == 0) {
                                    faceId = 0xc;
                                }
                            }
                        }
                        sprintf(pathBuf178, lbl_eu_804FFF2C + 0x3d, (unsigned)faceId);
                        *(CFileHandle**)(slotBase + 0x94) =
                            CDeviceFile::readFile(mHeap, pathBuf178, battleWorkEvent(this), 0, 0);
                    }
                    sprintf(pathBufF8, lbl_eu_804FFF2C + 0x59, partyId);
                    *(CFileHandle**)(slotBase + 0xac) =
                        CDeviceFile::readFile(mHeap, pathBufF8, battleWorkEvent(this), 0, 0);
                }
                off += 4;
                slotBase += 4;
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
            slotBase = reinterpret_cast<u8*>(this);
            n = 0;
            for (; n < 3; n++) {
                if (*(CFileHandle**)(slotBase + 0x94) != NULL) {
                    unkE8 = 0;
                    goto after_assets;
                }
                if (*(CFileHandle**)(slotBase + 0xac) != NULL) {
                    unkE8 = 0;
                    goto after_assets;
                }
                slotBase += 4;
            }
        }
    }
after_assets:

    if (lbl_eu_80664048->unkE8 != 0) {
        if (func_801042A4() == 0) {
            cf::CfGameManager::getInstance();
            if (func_8006EF04(0x10000000) == 0) {
                objMove = cf::CfGameManager::func_80082D54(0);
                if (objMove != NULL) {
                    actor2 = func_8016FE34();
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
                                    void* moveBase = reinterpret_cast<u8*>(objPc) + 0x3e9c;
                                    handle = vslot<GetIntFn>(moveBase, 0x4c)(moveBase);
                                }
                                if (handle != 0) {
                                    obj = func_800B708C((BOOL)handle);
                                    if (obj != NULL) {
                                        if ((*(u32*)((u8*)obj + 0x64) & 4) != 0) {
                                            cf::CfGameManager::getInstance();
                                            if (func_8006EF04(0x1000000) == 0) {
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
                                                            startNode =
                                                                inst->mChildList.mStartNodePtr;
                                                            capacity = inst->mChildList.mCapacity;
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
                            void* moveBase = reinterpret_cast<u8*>(actor2) + 0x3e9c;
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
                                startNode = inst->mChildList.mStartNodePtr;
                                capacity = inst->mChildList.mCapacity;
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

// LLM-HARNESS-BEGIN: us-8012f1b0
extern "C" void func_8012E6DC() {}
// LLM-HARNESS-END: us-8012f1b0
// LLM-HARNESS-BEGIN: us-8012fd70
extern "C" void func_8012F29C() {}
// LLM-HARNESS-END: us-8012fd70
// LLM-HARNESS-BEGIN: us-8012fd90
extern "C" void func_8012F2BC() {}
// LLM-HARNESS-END: us-8012fd90
// LLM-HARNESS-BEGIN: us-80130334
extern "C" void func_8012F860() {
    if (lbl_eu_80664048 != 0) {
        reinterpret_cast<unsigned char *>(lbl_eu_80664048)[0x82] |= 0x3b;
    }
}
// LLM-HARNESS-END: us-80130334
// LLM-HARNESS-BEGIN: us-80130350
extern "C" void func_8012F87C() {}
// LLM-HARNESS-END: us-80130350
// LLM-HARNESS-BEGIN: us-80130530
extern "C" unsigned char func_8012FA5C() { return lbl_eu_80664048 == 0 ? 0 : *(unsigned char *)((char *)lbl_eu_80664048 + 0xe8); }
// LLM-HARNESS-END: us-80130530
// LLM-HARNESS-BEGIN: us-8013054c
extern "C" void* func_8012FA78() { void* p = lbl_eu_80664048; if (p == 0) return 0; if (*(unsigned char*)((char*)p + 0xe8) != 0) return (char*)p + 0xb8; return 0; }
// LLM-HARNESS-END: us-8013054c
// LLM-HARNESS-BEGIN: us-80130890
extern "C" int func_8012FDBC() { if (lbl_eu_80664048 == 0) return -1; return *(int*)((char*)lbl_eu_80664048 + 0xec); }
// LLM-HARNESS-END: us-80130890
// LLM-HARNESS-BEGIN: us-801308ac
extern "C" void Draw__Q216CUIBattleManager5CTestFv() {}
// LLM-HARNESS-END: us-801308ac
// LLM-HARNESS-BEGIN: us-801308b0
extern "C" void Move__Q216CUIBattleManager5CTestFv() {}
// LLM-HARNESS-END: us-801308b0
// LLM-HARNESS-BEGIN: us-801308b4
extern "C" void Term__Q216CUIBattleManager5CTestFv() {}
// LLM-HARNESS-END: us-801308b4
// LLM-HARNESS-BEGIN: us-801308b8
extern "C" void Init__Q216CUIBattleManager5CTestFv() {}
// LLM-HARNESS-END: us-801308b8
// LLM-HARNESS-BEGIN: us-80130a40
extern "C" void __dt__16CUIBattleManagerFv(void*);

extern "C" void func_8012FF6C(void* p) {
    __dt__16CUIBattleManagerFv((char*)p - 0x54);
}
// LLM-HARNESS-END: us-80130a40

// LLM-HARNESS-BEGIN: us-8012ea60
extern "C" void __ct__CUIBattleManager() {}
// LLM-HARNESS-END: us-8012ea60
// LLM-HARNESS-BEGIN: us-8012f104
extern "C" void func_8012E630() {}
// LLM-HARNESS-END: us-8012f104
// LLM-HARNESS-BEGIN: us-8012fd94
extern "C" void func_8012F2C0() {}
// LLM-HARNESS-END: us-8012fd94
// LLM-HARNESS-BEGIN: us-8013002c
extern "C" void func_8012F558() {}
// LLM-HARNESS-END: us-8013002c
// LLM-HARNESS-BEGIN: us-801300cc
extern "C" void func_8012F5F8() {}
// LLM-HARNESS-END: us-801300cc
// LLM-HARNESS-BEGIN: us-80130224
extern "C" void func_8012F750() {}
// LLM-HARNESS-END: us-80130224
// LLM-HARNESS-BEGIN: us-8013057c
extern "C" void func_8012FAA8() {}
// LLM-HARNESS-END: us-8013057c
// LLM-HARNESS-BEGIN: us-80130748
extern "C" void func_8012FC74() {}
// LLM-HARNESS-END: us-80130748
// LLM-HARNESS-BEGIN: us-801307d8
extern "C" void func_8012FD04() {}
// LLM-HARNESS-END: us-801307d8
// LLM-HARNESS-BEGIN: us-80130834
extern "C" void func_8012FD60() {}
// LLM-HARNESS-END: us-80130834
// LLM-HARNESS-BEGIN: us-801309dc
extern "C" void __dt__Q216CUIBattleManager5CTestFv() {}
// LLM-HARNESS-END: us-801309dc
