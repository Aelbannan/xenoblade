// Decompiled CREvtModelMap - realtime event model map
#define IWORK_EVENT_INLINE_DTOR
// High-level C++ reconstruction.

#include <string.h>
#include "kyoshin/cf/CfMapEffectManager.hpp"
#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/realtimeevt/CREvtModelMap.hpp"
#include <decomp.h>
#include "kyoshin/realtimeevt/CREvtLight.hpp"
// The +0x2F3C game-manager sub-object is a CScnEnvLgtCtrlListItem (real
// owner in libs/monolib/src/scn/CScnEnvLgtCtrl.hpp, which cannot be
// included here over this TU's conflicting decls). TU-local view under the
// real owner name with retail arities (CTaskREvtSequence.cpp idiom); this
// TU dispatches its v24 slot (0x68) with the retail int flag.
class CScnEnvLgtCtrlListItem {
public:
    virtual void v0(int flag);
    virtual void v1();
    virtual void v2(void* arg);
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8(void* arg);
    virtual void v9(void* arg);
    virtual void v10(void* arg, void* data);
    virtual void v11(void* arg, u8 byte);
    virtual int v12(void* vec);
    virtual int v13(void* vec);
    virtual void v14(void* data);
    virtual int v15(float f, void* data, void* arg, u32 count);
    virtual void v16(void* arg);
    virtual void v17(void* arg);
    virtual void v18(void* arg, float f);
    virtual int v19(void* bits, int flag);
    virtual int v20(void* base, int flag);
    virtual void v21();
    virtual void v22(float f, s32 mode, s32 submode);
    virtual int v23();
    virtual void v24(int flag);  // slot 0x68 - retail passes r4
    virtual void v25(int flag);
    virtual void v26(void* a, void* b);
    virtual void v27(void* a, void* b, float f);
    virtual int v28(void* out, int flag);
    virtual int v29();
    virtual int v30();
    virtual void v31(u32 a, u32 b);
    virtual void v32(u32 a, u32 b, void* c, void* d);
    virtual void v33(u32 a, u32 b, void* c);
    virtual void v34();
    virtual void v35(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g);
    virtual void v36();
    virtual void v37(int flag);
};
#include "kyoshin/cf/object/CfObject.hpp"
#include "kyoshin/cf/CfGimmick.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/util/FixStr.hpp"
#include "include/kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"
#include "libs/monolib/src/scn/CScnItemModel.hpp"

#include "monolib/work/IWorkEvent.hpp"
#include "monolib/work/CEventFile.hpp"
#include "kyoshin/cf/CTaskREvtSequence.hpp"

extern "C" {
void Tok_FormatStr(ml::FixStr<64>& buf, u32 packed, int prefixFlag, int suffixFlag);
u32 EvtSeqGetCounter100();
u32 EvtSeqGetStateBit5();
u32 REvtCam_IsField98Set();
s32 REvtCam_IsMode2B0Eq1();
s32 REvtCam_IsMode2B0Eq2();
void enableLOD__8CTaskLODFv();
void restorePrimaryLOD__8CTaskLODFv();
void disableLOD__8CTaskLODFv();
void acquireLODResource__8CTaskLODFv(void* task, u32 value);
bool Scn_IsAnimActiveOrNull(void* pEmote);
void* Scn_SetupAnim(void* a, void* b, int c, int d);
void simSetFlag2OnTree(void* pEmote, int flag);
void simSetLeafDist7B0(void* pEmote, float scale);
void simRefreshFlag8(void* pEmote, float time);
void scnLgtRemoveLgtItem(void* pData);
void* scnLgtCreateEnvController(void* pData, void* pFile, int flags);
mtl::ALLOC_HANDLE getScnCounter__Fv();
extern void* getMapEffectManager__Q22cf13CfGameManagerFv();
extern void setHandleParam__11CDeviceFileFP11CFileHandleUl(CFileHandle*, u32);
extern void Tok_Unpack(u32 packed, u32* out0, u32* out1, u32* out2, u32* out3);
}

namespace cf {

CREvtModelMap* CREvtModelMap::sLoadingGuest;
CREvtModelMap* CREvtModelMap::sVisibleModel;

CREvtModelMap::CREvtModelMap(void* pData)
    : CREvtModel(pData, 1),
      mVisible(1),
      mIsGuest(0),
      mFileHandle1(0),
      mFileData1(0),
      mFileHandle2(0),
      mFileData2(0),
      mFileHandle3(0),
      mFileData3(0),
      mLoadedModelData(0),
      mEmoteModel(0),
      mCreatureCount(0),
      mCreatureId(-1)
{
    memset(mCreatureState, 0, sizeof(mCreatureState));
    u32 out0, out1, out2, out3;
    Tok_Unpack(*(u32*)((char*)mPtr1C + 0x20), &out0, &out1, &out2, &out3);
    if (out0 == 1) {
        mIsGuest = 1;
        mCallback = static_cast<void (CREvtObj::*)()>(&CREvtModelMap::loadFiles);
        mFlags |= 0x10;
        sVisibleModel = this;
    } else {
        mCallback = static_cast<void (CREvtObj::*)()>(&CREvtModelMap::probeSlot);
        insertRtEvent();
        if (*(s32*)((char*)mPtr1C + 0x30) != 0) {
            mFlags |= 0x30;
        }
    }
}

CREvtModelMap::~CREvtModelMap()
{
    if (mIsGuest) {
        setVisible(0);
        if (cf::CfGameManager::getGameSubManager()) {
            reinterpret_cast<cf::CfObject*>(cf::CfGameManager::getGameSubManager())->setPointEnabled(1);
        }
        enableLOD__8CTaskLODFv();
        if (mCreatureCount > 0) {
            CCreatureNode* it;
            s32 i = 0;
            for (it = ((CCreatureNode*)((cf::CfGameManager*)spawnGimmickEntity__Q22cf13CfGameManagerFv())->field_0x4)->next;
                 it != (CCreatureNode*)((cf::CfGameManager*)spawnGimmickEntity__Q22cf13CfGameManagerFv())->field_0x4;
                 it = it->next, i++) {
                reinterpret_cast<cf::CfObject*>(it->obj)->setPointEnabled(mCreatureState[i] != 0);
            }
        }
        sLoadingGuest = 0;
        if (getMapEffectManager__Q22cf13CfGameManagerFv()) {
            getMapEffectManager__Q22cf13CfGameManagerFv();
            cf::CfMapEffectManager::setPointEnabled(1);
        }
    }
    reset();
    restorePrimaryLOD__8CTaskLODFv();
}

int CREvtModelMap::isMapModel() {
    s8* info = (s8*)mPtr1C;
    return (info[0x10] == 0x6D && info[0x11] == 0x61) ? 1 : 0;
}

bool CREvtModelMap::isBusy()
{
    return mFileHandle1 || mFileHandle2 || mFileHandle3;
}

int CREvtModelMap::reset()
{
    if (mFlags & 0x80) {
        return 0;
    }
    CREvtModel::reset();
    if (mEmoteModel) {
        Scn_IsAnimActiveOrNull(mEmoteModel);
        mEmoteModel = nullptr;
    }
    if (mLoadedModelData) {
        scnLgtRemoveLgtItem(*(void**)((u8*)gmScene + 0x7C));
        mLoadedModelData = nullptr;
    }
    void* p1 = mFileData1;
    if (p1) {
        if (p1) {
            mtl::MemManager::deallocate(p1);
            mFileData1 = 0;
        }
    }
    void* p2 = mFileData2;
    if (p2) {
        if (p2) {
            mtl::MemManager::deallocate(p2);
            mFileData2 = 0;
        }
    }
    void* p3 = mFileData3;
    if (p3) {
        if (p3) {
            mtl::MemManager::deallocate(p3);
            mFileData3 = 0;
        }
    }
    if (mFileHandle1) {
        CDeviceFile::cancel(mFileHandle1);
    }
    if (mFileHandle2) {
        CDeviceFile::cancel(mFileHandle2);
    }
    if (mFileHandle3) {
        CDeviceFile::cancel(mFileHandle3);
    }
    mFileHandle1 = 0;
    mFileHandle2 = 0;
    mFileHandle3 = 0;
    return 1;
}

void CREvtModelMap::disablePoints()
{
    if (CREvtModelMap::sLoadingGuest) {
        return;
    }
    if (cf::CfGameManager::getGameSubManager()) {
        cf::CfGameManager* mgr = (cf::CfGameManager*)cf::CfGameManager::getGameSubManager();
        reinterpret_cast<cf::CfObject*>(mgr)->setPointEnabled(0);
        disableLOD__8CTaskLODFv();
    }
}

void CREvtModelMap::enablePoints()
{
    if (CREvtModelMap::sLoadingGuest) {
        return;
    }
    cf::CfGameManager* mgr = (cf::CfGameManager*)cf::CfGameManager::getGameSubManager();
    reinterpret_cast<cf::CfObject*>(mgr)->setPointEnabled(1);
    enableLOD__8CTaskLODFv();
}

void CREvtModelMap::probeSlot()
{
    s32* pId = &mCreatureId;
    *pId = -1;
    if (probeEvtSlot(reinterpret_cast<u32*>(pId))) {
        mCallback = static_cast<void (CREvtObj::*)()>(&CREvtModelMap::loadFiles);
    }
}

void CREvtModelMap::loadFiles()
{
    u32 flags = mFlags | 0x40;
    if (mIsGuest) {
        sLoadingGuest = this;
        flags |= 0x11;
        mCallback = static_cast<void (CREvtObj::*)()>(&CREvtModelMap::update);
        mFlags = flags;
        cf::CfGameManager* mgr = (cf::CfGameManager*)cf::CfGameManager::getGameSubManager();
        Tok_FormatStr(mBasePath, mgr->unk70, 1, 0);
        mBasePath += ".lod";
    } else {
        ml::FixStr<32> baseBuf;
        ml::FixStr<32> filePath;
        baseBuf = "/obj/";
        filePath = baseBuf;
        filePath += ((CREvtModelMapNameInfo*)mPtr1C)->mName;
        filePath += ".map";
        if (CDeviceFile::getFileSize(filePath.mString) > 0) {
            u32 innerFlags = ((CREvtModelMapNameInfo*)mPtr1C)->field_0x58;
            if ((innerFlags & 2) && !(innerFlags & 0x40)) {
                mFileHandle1 = CDeviceFile::readFile(
                    gmScene->getAllocHandle(), filePath.mString, this, 0, 0);
                setHandleParam__11CDeviceFileFP11CFileHandleUl(mFileHandle1, mtl::MemManager::getHandleMEM2());
            } else {
                mFileHandle1 = CDeviceFile::readFile(
                    mtl::MemManager::getHandleMEM2(), filePath.mString, this, 0, 0);
                setHandleParam__11CDeviceFileFP11CFileHandleUl(mFileHandle1, gmScene->getAllocHandle());
            }
            if (EvtSeqGetStateBit5()) {
                setHandleParam__11CDeviceFileFP11CFileHandleUl(mFileHandle1, getScnCounter__Fv());
            }
            setHandleParam__11CDeviceFileFP11CFileHandleUl(mFileHandle1, mtl::MemManager::getHandleMEM1());
            if (((CREvtModelMapNameInfo*)mPtr1C)->field_0x58 & 1) {
                CDeviceFile::setHandleFlag1(mFileHandle1);
            }
            filePath = baseBuf;
            filePath += ((CREvtModelMapNameInfo*)mPtr1C)->mName;
            filePath += ".lod";
            if (CDeviceFile::getFileSize(filePath.mString) > 0) {
                u32 innerFlags = ((CREvtModelMapNameInfo*)mPtr1C)->field_0x58;
                if ((innerFlags & 2) && !(innerFlags & 0x40)) {
                    mFileHandle2 = CDeviceFile::readFile(
                        gmScene->getAllocHandle(), filePath.mString, this, 0, 0);
                    setHandleParam__11CDeviceFileFP11CFileHandleUl(mFileHandle2, mtl::MemManager::getHandleMEM2());
                } else {
                    mFileHandle2 = CDeviceFile::readFile(
                        mtl::MemManager::getHandleMEM2(), filePath.mString, this, 0, 0);
                    setHandleParam__11CDeviceFileFP11CFileHandleUl(mFileHandle2, gmScene->getAllocHandle());
                }
                if (EvtSeqGetStateBit5()) {
                    setHandleParam__11CDeviceFileFP11CFileHandleUl(mFileHandle2, getScnCounter__Fv());
                }
                setHandleParam__11CDeviceFileFP11CFileHandleUl(mFileHandle2, mtl::MemManager::getHandleMEM1());
                if (((CREvtModelMapNameInfo*)mPtr1C)->field_0x58 & 1) {
                    CDeviceFile::setHandleFlag1(mFileHandle2);
                }
            }
            filePath = baseBuf;
            filePath += ((CREvtModelMapNameInfo*)mPtr1C)->mName;
            filePath += ".lgt";
            if (CDeviceFile::getFileSize(filePath.mString) > 0) {
                u32 innerFlags = ((CREvtModelMapNameInfo*)mPtr1C)->field_0x58;
                if ((innerFlags & 2) && !(innerFlags & 0x40)) {
                    mFileHandle3 = CDeviceFile::readFile(
                        gmScene->getAllocHandle(), filePath.mString, this, 0, 0);
                    setHandleParam__11CDeviceFileFP11CFileHandleUl(mFileHandle3, mtl::MemManager::getHandleMEM2());
                } else {
                    mFileHandle3 = CDeviceFile::readFile(
                        mtl::MemManager::getHandleMEM2(), filePath.mString, this, 0, 0);
                    setHandleParam__11CDeviceFileFP11CFileHandleUl(mFileHandle3, gmScene->getAllocHandle());
                }
                if (EvtSeqGetStateBit5()) {
                    setHandleParam__11CDeviceFileFP11CFileHandleUl(mFileHandle2, getScnCounter__Fv());
                }
                setHandleParam__11CDeviceFileFP11CFileHandleUl(mFileHandle3, mtl::MemManager::getHandleMEM1());
                if (((CREvtModelMapNameInfo*)mPtr1C)->field_0x58 & 1) {
                    CDeviceFile::setHandleFlag1(mFileHandle3);
                }
            }
            mFlags |= 0x2;
            mCallback = static_cast<void (CREvtObj::*)()>(&CREvtModelMap::finishLoad);
        } else {
            mFlags |= 0x11;
            mCallback = static_cast<void (CREvtObj::*)()>(&CREvtModelMap::update);
        }
    }
}

void CREvtModelMap::finishLoad()
{
    if (mFlags & 2) {
        if (!mFileHandle1 && mFileData1 && !mEmoteModel) {
            mEmoteModel = (CScnItemModel*)Scn_SetupAnim(gmScene, mFileData1, 7, 1);
            simSetFlag2OnTree(mEmoteModel, 0);
            simSetLeafDist7B0(mEmoteModel, 0.0f);
            mEmoteModel->flags7A8 |= 4;
            mVisible = 0;
            if (sVisibleModel == this) {
                sVisibleModel = 0;
            }
        }
        if (mFileHandle2 || mFileHandle3 || mFileHandle1) {
            if (!(mFlags & 4)) {
                return;
            }
        }
        mFlags |= 0x11;
        mCallback = static_cast<void (CREvtObj::*)()>(&CREvtModelMap::update);
    } else {
        mCallback = static_cast<void (CREvtObj::*)()>(&CREvtModelMap::update);
    }
}

void CREvtModelMap::update()
{
    if (mEmoteModel) {
        f32 fTime = (f32)(s32)EvtSeqGetCounter100();
        simRefreshFlag8(mEmoteModel, fTime);
    }
    setVisible(REvtCam_IsField98Set());
    checkRunGuard();
}

void CREvtModelMap::onEvent(u32 visible)
{
    if (mIsGuest) {
        cf::CfGameManager* mgr = (cf::CfGameManager*)cf::CfGameManager::getGameSubManager();
        if (mgr) {
            if (REvtCam_IsMode2B0Eq1()) {
                cf::CfGameManager::getGameSubManager()->field_0x100 &= ~4;
            } else if (visible) {
                cf::CfGameManager::getGameSubManager()->field_0x100 |= 4;
            }
        }
    }
    if (mVisible == visible) {
        goto set_guest_flags;
    }
    if (mIsGuest) {
        cf::CfGameManager* mgr = (cf::CfGameManager*)cf::CfGameManager::getGameSubManager();
        if (mgr) {
            reinterpret_cast<cf::CfObject*>(mgr)->setPointEnabled(visible);
        }
        if (visible) {
            restorePrimaryLOD__8CTaskLODFv();
            sVisibleModel = this;
        } else {
            if (sVisibleModel == this) {
                disableLOD__8CTaskLODFv();
                sVisibleModel = 0;
            }
        }
        if (getMapEffectManager__Q22cf13CfGameManagerFv()) {
            getMapEffectManager__Q22cf13CfGameManagerFv();
            cf::CfMapEffectManager::setPointEnabled(visible);
        }
        s32 i = 0;
        CCreatureNode* head = (CCreatureNode*)((cf::CfGameManager*)spawnGimmickEntity__Q22cf13CfGameManagerFv())->field_0x4;
        CCreatureNode* it = head->next;
        while (it != (CCreatureNode*)((cf::CfGameManager*)spawnGimmickEntity__Q22cf13CfGameManagerFv())->field_0x4) {
            if (!visible) {
                int v = reinterpret_cast<cf::CfObject*>(it->obj)->CfObject_checkSubReady();
                mCreatureState[i] = (u8)v;
                mCreatureCount++;
                reinterpret_cast<cf::CfObject*>(it->obj)->setPointEnabled(0);
            } else {
                reinterpret_cast<cf::CfObject*>(it->obj)->setPointEnabled(mCreatureState[i] != 0);
            }
            it = it->next;
            i++;
        }
        if (visible) {
            mCreatureCount = 0;
        }
        goto visible_store;
    }
    if (mEmoteModel) {
        if (visible) {
            if (mFileData2) {
                acquireLODResource__8CTaskLODFv(mFileData2, 1);
            }
            if (mFileData3) {
                mLoadedModelData = scnLgtCreateEnvController(*(void**)((u8*)gmScene + 0x7C), mFileData3, 7);
                cf::CfGameManager* m2 = (cf::CfGameManager*)cf::CfGameManager::getGameSubManager();
                if (m2 && *(void**)((u8*)m2 + 0x2F3C)) {
                    CScnEnvLgtCtrlListItem* fx = (CScnEnvLgtCtrlListItem*)(*(void**)((u8*)cf::CfGameManager::getGameSubManager() + 0x2F3C));
                    fx->v24(0);
                }
            }
            sVisibleModel = this;
        } else {
            if (sVisibleModel == this) {
                disableLOD__8CTaskLODFv();
                sVisibleModel = 0;
            }
            if (mLoadedModelData) {
                scnLgtRemoveLgtItem(*(void**)((u8*)gmScene + 0x7C));
                mLoadedModelData = 0;
                cf::CfGameManager* m2 = (cf::CfGameManager*)cf::CfGameManager::getGameSubManager();
                if (m2 && *(void**)((u8*)m2 + 0x2F3C)) {
                    CScnEnvLgtCtrlListItem* fx = (CScnEnvLgtCtrlListItem*)(*(void**)((u8*)cf::CfGameManager::getGameSubManager() + 0x2F3C));
                    fx->v24(1);
                }
            }
        }
        simSetFlag2OnTree(mEmoteModel, visible);
    }
visible_store:
    mVisible = (u8)visible;
set_guest_flags:
    if (mIsGuest) {
        s32 isZero = (REvtCam_IsMode2B0Eq2() == 0);
        UnkClass_80083298* m3 = cf::CfGameManager::getGameSubManager();
        if (isZero) {
            m3->field_0x100 |= 0x10;
        } else {
            m3->field_0x100 &= ~0x10;
        }
    }
}

void CREvtModelMap::setVisible(int visible)
{
    if (mIsGuest) {
        if (cf::CfGameManager::getGameSubManager()) {
            cf::CfGameManager* mgr = (cf::CfGameManager*)cf::CfGameManager::getGameSubManager();
            reinterpret_cast<cf::CfObject*>(mgr)->setPointEnabled(visible);
        }
        if (getGlobalSda()) {
            *(u8*)((u8*)getGlobalSda() + 0xA8) = visible;
        }
        u8* taskState = (u8*)(*(void**)((u8*)gmScene + 0x78));
        taskState[0x28] = visible;
    } else if (mEmoteModel) {
        mEmoteModel->vfuncB4(REvtCam_IsField98Set());
    }
}

int CREvtModelMap::vfunc_20() { return mIsGuest != 0 ? 0 : 3; }

bool CREvtModelMap::OnFileEvent(CEventFile* pEvent)
{
    if (pEvent->mFileHandle == mFileHandle1) {
        if (pEvent->unk0 == 1) {
            mFileData1 = mFileHandle1->getData();
        } else {
            mFlags |= 4;
        }
        mFileHandle1 = 0;
        return 1;
    }
    if (pEvent->mFileHandle == mFileHandle2) {
        if (pEvent->unk0 == 1) {
            mFileData2 = mFileHandle2->getData();
        }
        mFileHandle2 = 0;
        return 1;
    }
    if (pEvent->mFileHandle == mFileHandle3) {
        if (pEvent->unk0 == 1) {
            mFileData3 = mFileHandle3->getData();
        }
        mFileHandle3 = 0;
        return 1;
    }
    return 0;
}

} // namespace cf
