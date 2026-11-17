// TU: kyoshin/realtimeevt/CREvtModelPc
// Class: cf::CREvtModelPc (public cf::CREvtModel, private IWorkEvent)

#define IWORK_EVENT_INLINE_DTOR
#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/realtimeevt/CREvtModelPc.hpp"

#include <cstring>
#include "kyoshin/cf/CfGameManagerData.hpp"  // lbl_eu_80663E14 (CScn)
#include "kyoshin/cf/object/CfObject.hpp"

// Per-character body variant used in co-op mode (indexed by character id).
static const u32 sCoopVariant[12] = {0, 2, 3, 2, 2, 2, 3, 2, 4};

#include "kyoshin/cf/CTaskREvtSequence.hpp"

extern "C" {
    // Task helpers
    extern int EvtSeqGetWalkIndex();
    extern int EvtSeqGetStateBit14();
    extern int EvtSeqGetStateBit12();
    extern u32 evtLookupCmdWord(void* parent, u32 type, int slot);

    // CfGameManager
    extern void syncFieldData__Q22cf13CfGameManagerFv(u32 type, int flag);

    // Resource helpers
    extern void* CfRes_tryResolveToken(u32 handle, int* outType);
    extern void KyoshinHeap_Free78(void* p, int type);
    // Packed-token helpers (retail C-symbol names)
    extern char* Tok_FormatRes(u32 handle);
    extern void Tok_Unpack(u32 packed, u32* out0, u32* out1, u32* out2, u32* out3);
    extern int Tok_FormatStr(ml::FixStr<64>& buf, u32 packed, int prefixFlag, int suffixFlag);
    extern u32 Tok_Pack10(u32 a, u32 b, u32 c);
    // Archive
    extern int KyoshinHeap_AllocNode(u32 value, int enable);

    // Model helpers
    extern void* Scn_SetupAnim(void* a, void* b, int c, int d);
    extern void simSetLeafDist7B0(void* model, f32 val);
    extern void simSetFlag2000Chain(void* model, int flag);
    extern void simSetLeafFlag4000(void* model, int flag);
    extern void simAttachModel(void* model, void* texName);
    extern void simCoupleModel(void* model, void* texName);
    extern void simRefreshFlag8(void* model, f32 val);

    extern void setHandleParam__11CDeviceFileFP11CFileHandleUl(CFileHandle* pFileHandle, u32 val);
    // CDeviceFile::getFileSize: retail passes a second (flag) argument the
    // monolib header does not declare yet.
    extern int getFileSize__11CDeviceFileFPCc(const char* pPath, int flag);
}

// Event data block at CREvtModel::mPtr1C as read by this TU.
struct CREvtModelPcEvtData {
    /* 0x00 */ u8 _00[0x20];
    /* 0x20 */ u32 mToken;          // packed character token
    /* 0x24 */ u8 _24[0x30 - 0x24];
    /* 0x30 */ s32 mField30;        // -1: model only needs updating
    /* 0x34 */ u8 _34[0x48 - 0x34];
    /* 0x48 */ s8 mCmd48;           // command-word block (evtLookupCmdWord)
    /* 0x49 */ u8 _49[0x58 - 0x49];
    /* 0x58 */ u32 mLoadFlags;      // bit0: handle flag 1, bit1: swap heaps
};

// Gimmick list (circular; head at CfGameManager +0x04). Each node holds the
// CfObject subobject embedded at +0x3E9C of a character actor.
struct CREvtModelPcActor {
    /* 0x0000 */ u8 _00[0x3E9C];
    /* 0x3E9C */ u8 mObject[0x3F28 - 0x3E9C];
    /* 0x3F28 */ u16 mCharaId;
};
struct CREvtModelPcNode {
    /* 0x00 */ CREvtModelPcNode* next;
    /* 0x04 */ u8 _04[4];
    /* 0x08 */ cf::CfObject* obj;
};
struct CREvtModelPcList {
    /* 0x00 */ u8 _00[4];
    /* 0x04 */ CREvtModelPcNode* head;
};

static inline CREvtModelPcActor* actorOf(cf::CfObject* obj) {
    return obj != 0 ? (CREvtModelPcActor*)((u8*)obj - 0x3E9C) : (CREvtModelPcActor*)obj;
}
static inline u32 objToken(cf::CfObject* obj) {
    return *(u32*)((u8*)obj + 0x70);
}

// Packed resource token: kind:5 | chara:7 | variant:10 | slot:10.
static inline u32 packToken(u32 kind, u32 chara, u32 variant, u32 slot) {
    return (kind << 27) | (chara << 20) | (variant << 10) | slot;
}

typedef void (cf::CREvtObj::*CREvtObjCallback)();

namespace cf {

CREvtModelPc::CREvtModelPc(void* pData) : CREvtModel(pData, 2) {
    mSpeed = 0.0f;
    mLoaded = 0;
    mCounter = -1;

    for (int i = 0; i < 6; i++) {
        mFileHandle[i] = 0;
        mFileReq[i] = 0;
        mData[i] = 0;
        mStatus[i] = 0;
    }

    mCallback = static_cast<CREvtObjCallback>(&CREvtModelPc::initCounter);
    insertRtEvent();

    u32 field30 = ((CREvtModelPcEvtData*)mPtr1C)->mField30;
    if (field30 == 0xFFFFFFFF) {
        mFlags |= 0x50;
        mCallback = static_cast<CREvtObjCallback>(&CREvtModelPc::updatePos);
    } else if (field30 > 1) {
        mFlags |= 0x30;
    }
}

CREvtModelPc::~CREvtModelPc() {
    reset();
}

bool CREvtModelPc::isBusy() {
    if (mCounter != EvtSeqGetWalkIndex() + 1) {
        return false;
    }

    bool result;
    if ((u32)((CREvtModelPcEvtData*)mPtr1C)->mField30 <= 1) {
        result = false;
    } else {
        result = false;
        if (mCallback == static_cast<CREvtObjCallback>(&CREvtModelPc::loadFiles) ||
            mCallback == static_cast<CREvtObjCallback>(&CREvtModelPc::checkLoaded)) {
            result = true;
        }
    }
    return result;
}

int CREvtModelPc::reset() {
    if (mFlags & 0x80) {
        return 0;
    }

    CREvtModel::reset();

    for (u32 i = 0; i < 6; i++) {
        if (mStatus[i] == 3) {
            if (mData[i] != 0) {
                if (mData[i] != 0) {
                    mtl::MemManager::deallocate(mData[i]);
                    mData[i] = 0;
                }
            }
        } else if (mStatus[i] == 2) {
            if (mData[i] != 0) {
                KyoshinHeap_Free78(mData[i], 0);
            }
        }

        if (mFileReq[i] != 0) {
            CDeviceFile::cancel(mFileReq[i]);
            mFileReq[i] = 0;
        }
    }

    return 1;
}

void CREvtModelPc::releaseAnims() {
    if ((u32)((CREvtModelPcEvtData*)mPtr1C)->mField30 == 0xFFFFFFFF) {
        mFlags |= 0x50;
        mCallback = static_cast<CREvtObjCallback>(&CREvtModelPc::updatePos);
        return;
    }

    for (int i = 0; i < 6; i++) {
        if (mStatus[i] == 1) {
            return;
        }
    }

    if (isFlag(0x100)) {
        return;
    }
    setFlag(0x100);

    CREvtModel::releaseAnims();

    for (u32 i = 0; i < 6; i++) {
        if (mStatus[i] == 3) {
            if (mData[i] != 0) {
                if (mData[i] != 0) {
                    mtl::MemManager::deallocate(mData[i]);
                    mData[i] = 0;
                }
            }
        } else if (mStatus[i] == 2) {
            if (mData[i] != 0) {
                KyoshinHeap_Free78(mData[i], 0);
            }
        }
    }

    mCallback = static_cast<CREvtObjCallback>(&CREvtModelPc::initCounter);
    mFlags = (mFlags | 0x30) & ~0x41;

    for (int i = 0; i < 6; i++) {
        mFileHandle[i] = 0;
        mFileReq[i] = 0;
        mData[i] = 0;
        mStatus[i] = 0;
    }
    mLoaded = 0;
}

void CREvtModelPc::initCounter() {
    mCounter = -1;

    if (probeEvtSlot((u32*)&mCounter) != 0) {
        if (isFlag(0x100)) {
            clrFlag(0x100);
        }
        mCallback = static_cast<CREvtObjCallback>(&CREvtModelPc::loadFiles);
    }
}

void CREvtModelPc::loadFiles() {
    u32 decA, decB, decC, decD;
    u32 objA, objB, objC, objD;
    int typeOut;

    mFlags |= 0x40;
    CREvtModelPcEvtData* evt = (CREvtModelPcEvtData*)mPtr1C;
    Tok_Unpack(evt->mToken, &decA, &decB, &decC, &decD);

    // Characters 3 and 8 share a model: force a fresh load when the other one
    // is already on the field.
    CREvtModelPcNode* node;
    int forceLoad = 0;
    if (decB == 3 || decB == 8) {
        forceLoad = reinterpret_cast<UnkEvtListEntry*>(this)->SeqCheckRunGate();
        if (forceLoad == 0) {
            node = ((CREvtModelPcList*)getGimmickListHead__Q22cf13CfGameManagerFv())->head->next;
            while (node != ((CREvtModelPcList*)getGimmickListHead__Q22cf13CfGameManagerFv())->head) {
                u16 id = actorOf(node->obj)->mCharaId;
                if ((id == 8 && decB == 3) || (id == 3 && decB == 8)) {
                    forceLoad = 1;
                    break;
                }
                node = node->next;
            }
        }
    }

    if (decB <= 10) {
        // Find the live party character this model mirrors.
        CREvtModelPcActor* live = 0;
        node = ((CREvtModelPcList*)getGimmickListHead__Q22cf13CfGameManagerFv())->head->next;
        while (node != ((CREvtModelPcList*)getGimmickListHead__Q22cf13CfGameManagerFv())->head) {
            Tok_Unpack(objToken(node->obj), &objA, &objB, &objC, &objD);
            if (decB == objB && decC == 1) {
                live = actorOf(node->obj);
                break;
            }
            node = node->next;
        }

        int haveTokens = 0;
        if (((CREvtModelPcEvtData*)mPtr1C)->mCmd48 != 0 && !EvtSeqGetStateBit12()) {
            mFileHandle[0] = evtLookupCmdWord(&((CREvtModelPcEvtData*)mPtr1C)->mCmd48, decB, 1);
            mFileHandle[1] = evtLookupCmdWord(&((CREvtModelPcEvtData*)mPtr1C)->mCmd48, decB, 2);
            mFileHandle[2] = evtLookupCmdWord(&((CREvtModelPcEvtData*)mPtr1C)->mCmd48, decB, 3);
            mFileHandle[3] = evtLookupCmdWord(&((CREvtModelPcEvtData*)mPtr1C)->mCmd48, decB, 4);
            mFileHandle[4] = evtLookupCmdWord(&((CREvtModelPcEvtData*)mPtr1C)->mCmd48, decB, 5);
            if (mFileHandle[0] != 0 && mFileHandle[1] != 0 && mFileHandle[2] != 0 &&
                mFileHandle[3] != 0 && mFileHandle[4] != 0) {
                haveTokens = 1;
            }
        }

        int skipLoad = 0;
        if (!haveTokens) {
            if (live != 0 && !EvtSeqGetStateBit12()) {
                mFileHandle[0] = ((cf::CfObject*)live->mObject)->CfObject_getSlotBits(1);
                mFileHandle[1] = ((cf::CfObject*)live->mObject)->CfObject_getSlotBits(2);
                mFileHandle[2] = ((cf::CfObject*)live->mObject)->CfObject_getSlotBits(3);
                mFileHandle[3] = ((cf::CfObject*)live->mObject)->CfObject_getSlotBits(4);
                mFileHandle[4] = ((cf::CfObject*)live->mObject)->CfObject_getSlotBits(5);
            } else if (decC == 1 && !EvtSeqGetStateBit12()) {
                if (forceLoad == 0) {
                    syncFieldData__Q22cf13CfGameManagerFv(decB, 0);
                    skipLoad = 1;
                }
                mFileHandle[0] = getBdatEntryColumn__Q22cf13CfGameManagerFv(decB, 0);
                mFileHandle[1] = getBdatEntryColumn__Q22cf13CfGameManagerFv(decB, 1);
                mFileHandle[2] = getBdatEntryColumn__Q22cf13CfGameManagerFv(decB, 2);
                mFileHandle[3] = getBdatEntryColumn__Q22cf13CfGameManagerFv(decB, 3);
                mFileHandle[4] = getBdatEntryColumn__Q22cf13CfGameManagerFv(decB, 4);
            } else {
                u32 idx0 = decC;
                if (EvtSeqGetStateBit12()) {
                    idx0 = sCoopVariant[decB];
                }
                u32 idx1 = idx0;
                u32 idx2 = idx0;
                if (decB == 8 && idx0 == 1) {
                    idx2 = 4;
                    idx0 = 4;
                    idx1 = 4;
                }
                if (EvtSeqGetStateBit12()) {
                    if (decB == 1 && idx0 == 2) idx1 = 1;
                    if (decB == 2 && idx0 == 3) idx1 = 1;
                    if (decB == 5 && idx0 == 2) idx1 = 1;
                    if (decB == 6 && idx0 == 3) idx2 = 1;
                }
                mFileHandle[0] = packToken(decA, decB, idx1, 1);
                mFileHandle[1] = packToken(decA, decB, idx0, 2);
                mFileHandle[2] = packToken(decA, decB, idx2, 3);
                mFileHandle[3] = packToken(decA, decB, idx0, 4);
                mFileHandle[4] = packToken(decA, decB, idx0, 5);
            }
        }

        if (!skipLoad) {
            for (u32 i = 0; i <= 4; i++) {
                if (mFileHandle[i] == 0) {
                    continue;
                }

                ml::FixStr<32> name = Tok_FormatRes(mFileHandle[i]);
                CfRes_tryResolveToken(mFileHandle[i], &typeOut);
                if (forceLoad != 0) {
                    typeOut = -1;
                }
                if (typeOut != -1) {
                    continue;
                }

                ml::FixStr<64> path(true);
                Tok_FormatStr(path, mFileHandle[i], 1, 1);
                u32 size = getFileSize__11CDeviceFileFPCc(path.c_str(), 1);
                int mem = (int)allocEvtMemAligned(size, 0x20, 0);
                if (mem != 0) {
                    mStatus[i] = 1;
                } else {
                    mem = KyoshinHeap_AllocNode(size, 1) != 0 ? KyoshinHeap_AllocNode(size, 0) : 0;
                    if (mem != 0) {
                        mStatus[i] = 2;
                    }
                }

                if (mem != 0) {
                    mFileReq[i] = CDeviceFile::readCommonArchiveFile(mem, path.c_str(), this, 0, 0);
                } else {
                    mtl::ALLOC_HANDLE heap = mtl::MemManager::getHandleMEM2();
                    mtl::ALLOC_HANDLE param = lbl_eu_80663E14->getAllocHandle();
                    if (((CREvtModelPcEvtData*)mPtr1C)->mLoadFlags & 2) {
                        heap = lbl_eu_80663E14->getAllocHandle();
                        param = mtl::MemManager::getHandleMEM2();
                    }
                    mFileReq[i] = CDeviceFile::readFile(heap, path.c_str(), this, 0, 0);
                    setHandleParam__11CDeviceFileFP11CFileHandleUl(mFileReq[i], param);
                    if (((CREvtModelPcEvtData*)mPtr1C)->mLoadFlags & 1) {
                        CDeviceFile::setHandleFlag1(mFileReq[i]);
                    }
                    if (EvtSeqGetStateBit14()) {
                        CDeviceFile::setHandleFlag2(mFileReq[i]);
                    }
                    mStatus[i] = 3;
                }
            }
        }
    }

    // No body token: load the plain character model.
    if (mFileHandle[0] == 0) {
        mFileHandle[0] = Tok_Pack10(decB, decC, decD);
        ml::FixStr<64> path0(true);
        Tok_FormatStr(path0, mFileHandle[0], 1, 1);
        u32 size = getFileSize__11CDeviceFileFPCc(path0.c_str(), 1);
        int mem = (int)allocEvtMemAligned(size, 0x20, 0);
        if (mem != 0) {
            mStatus[0] = 1;
        } else {
            mem = KyoshinHeap_AllocNode(size, 1) != 0 ? KyoshinHeap_AllocNode(size, 0) : 0;
            if (mem != 0) {
                mStatus[0] = 2;
            }
        }

        if (mem != 0) {
            mFileReq[0] = CDeviceFile::readCommonArchiveFile(mem, path0.c_str(), this, 0, 0);
        } else {
            mtl::ALLOC_HANDLE heap = mtl::MemManager::getHandleMEM2();
            mtl::ALLOC_HANDLE param = lbl_eu_80663E14->getAllocHandle();
            if (((CREvtModelPcEvtData*)mPtr1C)->mLoadFlags & 2) {
                heap = lbl_eu_80663E14->getAllocHandle();
                param = mtl::MemManager::getHandleMEM2();
            }
            mFileReq[0] = CDeviceFile::readFile(heap, path0.c_str(), this, 0, 0);
            setHandleParam__11CDeviceFileFP11CFileHandleUl(mFileReq[0], param);
            if (((CREvtModelPcEvtData*)mPtr1C)->mLoadFlags & 1) {
                CDeviceFile::setHandleFlag1(mFileReq[0]);
            }
            if (EvtSeqGetStateBit14()) {
                CDeviceFile::setHandleFlag2(mFileReq[0]);
            }
            mStatus[0] = 3;
        }
    }

    // Special id 9 also loads the extra slot 5.
    if (decD == 9) {
        mFileHandle[5] = Tok_Pack10(decB, 1, decD);
        ml::FixStr<64> path5(true);
        Tok_FormatStr(path5, mFileHandle[5], 1, 1);
        u32 size = getFileSize__11CDeviceFileFPCc(path5.c_str(), 1);
        int mem = (int)allocEvtMemAligned(size, 0x20, 0);
        if (mem != 0) {
            mStatus[5] = 1;
        } else {
            mem = KyoshinHeap_AllocNode(size, 0);
            if (mem != 0) {
                mStatus[5] = 2;
            }
        }

        if (mem != 0) {
            mFileReq[5] = CDeviceFile::readCommonArchiveFile(mem, path5.c_str(), this, 0, 0);
        } else {
            mtl::ALLOC_HANDLE heap = mtl::MemManager::getHandleMEM2();
            mtl::ALLOC_HANDLE param = lbl_eu_80663E14->getAllocHandle();
            if (((CREvtModelPcEvtData*)mPtr1C)->mLoadFlags & 2) {
                heap = lbl_eu_80663E14->getAllocHandle();
                param = mtl::MemManager::getHandleMEM2();
            }
            mFileReq[5] = CDeviceFile::readFile(heap, path5.c_str(), this, 0, 0);
            setHandleParam__11CDeviceFileFP11CFileHandleUl(mFileReq[5], param);
            if (((CREvtModelPcEvtData*)mPtr1C)->mLoadFlags & 1) {
                CDeviceFile::setHandleFlag1(mFileReq[5]);
            }
            if (EvtSeqGetStateBit14()) {
                CDeviceFile::setHandleFlag2(mFileReq[5]);
            }
            mStatus[5] = 3;
        }
    }

    mCallback = static_cast<CREvtObjCallback>(&CREvtModelPc::checkLoaded);
}

void CREvtModelPc::checkLoaded() {
    int type;

    void* data0 = mData[0] != 0 ? mData[0] : CfRes_tryResolveToken(mFileHandle[0], &type);

    bool allLoaded = true;
    for (u32 i = 1; i <= 5; i++) {
        if (mFileHandle[i] != 0) {
            void* data = mData[i] != 0 ? mData[i] : CfRes_tryResolveToken(mFileHandle[i], &type);
            if (data == 0) {
                allLoaded = false;
            }
        }
    }

    if (data0 == 0 || !allLoaded) {
        return;
    }

    mFlags |= 0x11;
    mCallback = static_cast<CREvtObjCallback>(&CREvtModelPc::updatePos);
}

void CREvtModelPc::updatePos() {
    if (mModel != 0) {
        simRefreshFlag8(mModel, (int)EvtSeqGetCounter100());
    }

    if (mFlags & 0x800) {
        checkRunGuard();
    }
}

int CREvtModelPc::vfunc_0C() {
    return (int)mModel;
}

void CREvtModelPc::createModel() {
    int type;

    if (!(mFlags & 0x10)) {
        return;
    }

    void* data0 = mData[0] != 0 ? mData[0] : CfRes_tryResolveToken(mFileHandle[0], &type);

    int allLoaded = 1;
    for (u32 i = 1; i <= 5; i++) {
        if (mFileHandle[i] != 0) {
            void* data = mData[i] != 0 ? mData[i] : CfRes_tryResolveToken(mFileHandle[i], &type);
            if (data == 0) {
                allLoaded = 0;
            }
        }
    }

    if (data0 != 0 && allLoaded) {
        mModel = (CScnItemModel*)Scn_SetupAnim(lbl_eu_80663E14, data0, -1, 1);
        simSetLeafDist7B0(mModel, 0.0f);
        simSetFlag2000Chain(mModel, 1);
        simSetLeafFlag4000(mModel, 1);
        mModel->flags7A8 |= 4;

        for (u32 i = 1; i <= 5; i++) {
            if (mFileHandle[i] != 0) {
                ml::FixStr<32> name = Tok_FormatRes(mFileHandle[i]);
                void* data = mData[i] != 0 ? mData[i] : CfRes_tryResolveToken(mFileHandle[i], &type);
                if (i != 5) {
                    simAttachModel(mModel, data);
                } else {
                    simCoupleModel(mModel, data);
                }
            }
        }

        mModel->vfunc64(1);
        mModel->vfunc9C(0, 0);
        mLoaded = 1;
        mFlags |= 0x800;
    }
}

void CREvtModelPc::setEventData(void* pData, void* pModelData) {
    if (mFileHandle[0] == 0) {
        return;
    }

    if (pData != 0 && mLoaded == 0) {
        createModel();
    }

    if (mLoaded != 0) {
        CREvtModel::setEventData(pData, pModelData);
    }
}

void CREvtModelPc::vfunc_24() {
    mFlags &= ~0x20;
}

int CREvtModelPc::vfunc_20() {
    u32 entryId, param1, param2, param3;
    Tok_Unpack(((CREvtModelPcEvtData*)mPtr1C)->mToken, &entryId, &param1, &param2, &param3);

    CREvtModelPcNode* node = ((CREvtModelPcList*)getGimmickListHead__Q22cf13CfGameManagerFv())->head->next;
    while (node != ((CREvtModelPcList*)getGimmickListHead__Q22cf13CfGameManagerFv())->head) {
        u32 objEntryId, objParam1, objParam2, objParam3;
        Tok_Unpack(objToken(node->obj), &objEntryId, &objParam1, &objParam2, &objParam3);

        // Already loaded if a live character object matches this request.
        if (param1 == objParam1 && param2 == 1) {
            return 1;
        }
        node = node->next;
    }

    return 6;
}

bool CREvtModelPc::OnFileEvent(CEventFile* ev) {
    // For each file slot, if the completed request matches the event's
    // handle, record the loaded data (or drop the token on error).
    for (int i = 0; i < 6; i++) {
        if (mFileReq[i] == ev->mFileHandle) {
            if (ev->unk0 == 1) {
                mData[i] = ev->mFileHandle->getData();
            } else {
                mFileHandle[i] = 0;
            }
            mFileReq[i] = NULL;
        }
    }

    return false;
}

int CREvtModelPc::vfunc_14() {
    u32 flags = mFlags;
    int result = 0;
    if (flags & 1) {
        if (flags & 0x40) {
            result = 1;
        }
    }
    return result;
}

} // namespace cf
