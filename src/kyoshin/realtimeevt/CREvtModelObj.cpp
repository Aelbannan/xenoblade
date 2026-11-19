// TU: kyoshin/realtimeevt/CREvtModelObj
// Class: cf::CREvtModelObj (derives from cf::CREvtModel, kind 3) - object
// model of a realtime event. Loads the model archive (".kp"/".chr"), sets
// the scene model up and drives it through the mCallback state ptmfs
// (stepProbe -> stepLoad -> stepSetup -> stepUpdate).

// Retail ~CREvtModelObj elides the empty IWorkEvent base-dtor call.
#define IWORK_EVENT_INLINE_DTOR

#include <string.h>

#include "kyoshin/realtimeevt/CREvtModelObj.hpp"
#include "kyoshin/realtimeevt/CREvtObj.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"
#include "kyoshin/cf/CTaskREvtSequence.hpp"
#include "libs/monolib/src/scn/CScnItemModel.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "monolib/scn/CScn.hpp"
#include "monolib/util/FixStr.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CEventFile.hpp"

namespace nw4r { namespace db { void Panic(const char* file, int line, const char* fmt, ...); } }

extern "C" {
// CTaskREvtSequence.cpp helpers that take the model as `this` (still C-linkage there).
extern cf::CREvtModelObj* EvtSeqFindSameNameType3(void* self);
extern int EvtSeqGetCounter104(void);
extern int EvtSeqGetWalkIndex(void);
extern int EvtSeqGetStateBit14(void);
extern int EvtSeqGetStateBit5(void);
extern int evtLookupEntryWord(const char* key, int slot);

extern void* KyoshinHeap_AllocNode(int size, int enable);
extern void KyoshinHeap_Free78(void* p, int);
extern char* Tok_FormatRes(u32 packed);
extern int Tok_FormatStr(ml::FixStr<64>& buf, u32 packed, int prefixFlag, int suffixFlag);
extern "C" void* CfRes_tryResolveToken(u32 packed, s32* outKind);
extern "C" int CfRes_tryDelegateLoad1(u32 packed, int);
extern int getFileSize__11CDeviceFileFPCc(const char* path, int param);
extern void setHandleParam__11CDeviceFileFP11CFileHandleUl(CFileHandle* handle, u32 param);
extern u32 getScnCounter__Fv(void);


extern CScnItemModel* Scn_SetupAnim(CScn* scn, void* data, int priority, int);
extern void simRefreshFlag8(CScnItemModel* model, float frame);
extern void simSetFlag2OnTree(CScnItemModel* model, int flag);
extern void simCoupleModel(CScnItemModel* model, void* data);
extern int labs(int);

// Shared strings owned by other units.
extern char lbl_eu_806623C0[2];      // directory separator
extern char lbl_eu_8052637C[];   // nw4r assert file
extern char lbl_eu_80526354[];   // nw4r assert message
}

// Archive entry ("KYP" header: count at +0x8, 0x10-byte entries at +0x10).
struct KypEntry {
    /* 0x0 */ u32 mOffset;
    /* 0x4 */ u32 mSize;
    /* 0x8 */ char mName[8];
};

// Model archive section names tried in stepSetup.
static const char* sModelExts[] = {"en", "np", "oj", nullptr};
// Weapon model names; the two digits select the player slot.
static const char* sWeaponNames[] = {"wp42", "wp82", "wp83", "wp84", "np62"};

// Weapon effect file shared by all object models.
static void* sWeaponData;
static CFileHandle* sWeaponFile;
static s32 sWeaponUsers;

namespace cf {

typedef void (CREvtObj::*EvtStep)();

CREvtModelObj::CREvtModelObj(void* pData)
    : CREvtModel(pData, 3), mFileHandle(nullptr), mFileState(0), mAllocData(nullptr), mRefCount(0),
      mModelType(0), mRefOwner(nullptr), mFileId(-1), mResId5C(0), mResId60(0), mFlag64(0),
      mExtractData(nullptr), mFileHandle2(nullptr), mAllocData2(nullptr), mField74(-1),
      mAllocData3(nullptr), mSchedule(nullptr), mCount80(0), mState84(0) {
    mCallback = static_cast<EvtStep>(&CREvtModelObj::stepProbe);
    insertRtEvent();

    u32 mode = evtData()->mField30;
    if (mode == 0xFFFFFFFF) {
        mFlags |= 0x50;
        mCallback = static_cast<EvtStep>(&CREvtModelObj::stepUpdate);
    } else if (mode != 0) {
        mFlags |= 0x30;
    }
}

CREvtModelObj::~CREvtModelObj() {
    if (mSchedule != nullptr) {
        schedClearFlag15Update(mSchedule);
        mSchedule = nullptr;
    }

    if (sWeaponFile != nullptr) {
        CDeviceFile::cancel(sWeaponFile);
        sWeaponFile = nullptr;
    }

    reset();

    if (mCount80 != 0) {
        if (sWeaponUsers > 0) {
            if (--sWeaponUsers == 0) {
                if (sWeaponData != nullptr) {
                    EffSched_LookupB(&lbl_eu_8065FC18, sWeaponData);
                    if (sWeaponData != nullptr) {
                        mtl::MemManager::deallocate(sWeaponData);
                        sWeaponData = nullptr;
                    }
                }
            }
        }
        mCount80 = 0;
    }
}

void CREvtModelObj::setEventData(void* pData, void* pModelData) {
    if (mSchedule != nullptr) {
        schedClearFlag15Update(mSchedule);
        mSchedule = nullptr;
    }

    CREvtModel::setEventData(pData, pModelData);

    if (mState84 == 1) {
        if (sWeaponData != nullptr && mModel != nullptr && pData != nullptr && mSchedule == nullptr &&
            mCount80 != 0) {
            mSchedule = static_cast<CSchedule*>(
            EffRoot_AddSchedReq(&lbl_eu_8065FC18, sWeaponData, gmScene, mCount80 - 1, 1, 0));
            if (mSchedule != nullptr) {
                schedAttachChildSlot(mSchedule, static_cast<CScheduleChild*>(this));
                mSchedule->field_0x14 = mModel->vfuncA8();
                schedSetStepIntervals(mSchedule, 40.0f, 1.0f);
            }
        }
    }

    if ((evtData()->mFlags & 0x80) && mModel != nullptr) {
        if (isVisionPackLoaded__Q22cf13CfGameManagerFv()) {
            simSetFlag2OnTree(mModel, 0);
        }
    }
}

int CREvtModelObj::reset() {
    if (mFlags & 0x80) {
        return 0;
    } else if (mRefCount > 0) {
        return 0;
    }

    if (mSchedule != nullptr) {
        schedClearFlag15Update(mSchedule);
        mSchedule = nullptr;
    }

    CREvtModel::reset();

    if (mFileState == 3) {
        if (mAllocData != nullptr) {
            if (mAllocData != nullptr) {
                mtl::MemManager::deallocate(mAllocData);
                mAllocData = nullptr;
            }
        }
    } else if (mFileState == 2) {
        if (mAllocData3 != nullptr) {
            KyoshinHeap_Free78(mAllocData3, 0);
            mAllocData3 = nullptr;
            mAllocData = nullptr;
        }
    } else {
        mAllocData = nullptr;
    }

    if (mFileHandle != nullptr) {
        CDeviceFile::cancel(mFileHandle);
        mFileHandle = nullptr;
    }

    if (mAllocData2 != nullptr) {
        if (mAllocData2 != nullptr) {
            mtl::MemManager::deallocate(mAllocData2);
            mAllocData2 = nullptr;
        }
    }

    if (mFileHandle2 != nullptr) {
        CDeviceFile::cancel(mFileHandle2);
        mFileHandle2 = nullptr;
    }

    if (mRefOwner != nullptr) {
        if (mRefOwner->mAllocData != nullptr) {
            mRefOwner->mRefCount--;
        }
        mRefOwner = nullptr;
    }

    return 1;
}

void CREvtModelObj::releaseAnims() {
    if (mFileState == 1) return;
    if (isFlag(0x100)) return;
    if (mRefCount > 0) return;

    CSchedule* sched = mSchedule;
    setFlag(0x100);
    if (sched != nullptr) {
        schedClearFlag15Update(sched);
        mSchedule = nullptr;
    }

    CREvtModel::releaseAnims();

    if (mFileState == 3) {
        if (mAllocData != nullptr) {
            if (mAllocData != nullptr) {
                mtl::MemManager::deallocate(mAllocData);
                mAllocData = nullptr;
            }
        }
    } else if (mFileState == 2) {
        if (mAllocData3 != nullptr) {
            KyoshinHeap_Free78(mAllocData3, 0);
            mAllocData3 = nullptr;
            mAllocData = nullptr;
        }
    } else {
        mAllocData = nullptr;
    }

    if (mAllocData2 != nullptr) {
        if (mAllocData2 != nullptr) {
            mtl::MemManager::deallocate(mAllocData2);
            mAllocData2 = nullptr;
        }
    }

    if (mRefOwner != nullptr) {
        if (mRefOwner->mAllocData != nullptr) {
            mRefOwner->mRefCount--;
        }
        mRefOwner = nullptr;
    }

    mExtractData = nullptr;
    mCallback = static_cast<EvtStep>(&CREvtModelObj::stepProbe);
    mFlags = (mFlags | 0x30) & ~0x243;
}

void CREvtModelObj::stepProbe() {
    scanPlayers();
    mField74 = -1;
    if (probeEvtSlot(reinterpret_cast<u32*>(&mField74))) {
        if (isFlag(0x100)) {
            clrFlag(0x100);
        }
        mCallback = static_cast<EvtStep>(&CREvtModelObj::stepLoad);
    }
}

int CREvtModelObj::vfunc_10() {
    int result = 0;
    if (mFlags & 0x10) {
        if (mCallback == static_cast<EvtStep>(&CREvtModelObj::stepProbe) ||
            mCallback == static_cast<EvtStep>(&CREvtModelObj::stepUpdate)) {
            result = 1;
        }
    }
    return result;
}

// Lets the scene allocator take large requests, falling back to MEM2/MEM1.
static inline void setFileHeaps(CFileHandle* handle, u32 flags, u32 scnLimit) {
    if (flags & 2) {
        if (mtl::MemManager::getMaxAllocSize(gmScene->getAllocHandle()) > scnLimit) {
            setHandleParam__11CDeviceFileFP11CFileHandleUl(handle, gmScene->getAllocHandle());
        }
        setHandleParam__11CDeviceFileFP11CFileHandleUl(handle, mtl::MemManager::getHandleMEM2());
    } else if (flags & 0x20) {
        if (mtl::MemManager::getMaxAllocSize(mtl::MemManager::getHandleMEM1()) > 0x40000) {
            setHandleParam__11CDeviceFileFP11CFileHandleUl(handle, mtl::MemManager::getHandleMEM1());
        }
        setHandleParam__11CDeviceFileFP11CFileHandleUl(handle, gmScene->getAllocHandle());
        setHandleParam__11CDeviceFileFP11CFileHandleUl(handle, mtl::MemManager::getHandleMEM2());
    } else {
        setHandleParam__11CDeviceFileFP11CFileHandleUl(handle, mtl::MemManager::getHandleMEM2());
        if (mtl::MemManager::getMaxAllocSize(gmScene->getAllocHandle()) > 0x40000) {
            setHandleParam__11CDeviceFileFP11CFileHandleUl(handle, gmScene->getAllocHandle());
        }
    }
    setHandleParam__11CDeviceFileFP11CFileHandleUl(handle, mtl::MemManager::getHandleMEM1());
}

void CREvtModelObj::stepLoad() {
    scanPlayers();
    mFlags |= 0x40;
    mCallback = static_cast<EvtStep>(&CREvtModelObj::stepSetup);
    mResId5C = evtData()->mResId;

    u32 kind, chara, sub, ext;
    Tok_Unpack(mResId5C, &kind, &chara, &sub, &ext);
    if (kind == 3 && ext == 9) {
        mResId5C = (kind << 27) | (chara << 20) | (sub << 10) | 1;
        mResId60 = evtData()->mResId;
    }

    if (kind == 5) {
        u32 resId = EvtSeqResolvePackedResId(mResId5C);
        if (resId != mResId5C) {
            mResId5C = resId;
        }
        char* suffix = evtData()->mSuffix;
        if (suffix[0] != 0) {
            int word = evtLookupEntryWord(suffix, chara / 10);
            if (word != 0) {
                mResId5C = word;
            }
        }
    }

    ml::FixStr<64> name(true);
    if (Tok_FormatStr(name, mResId5C, 0, 0)) {
        for (u32 i = 0; i < 5; i++) {
            if (strstr(name.c_str(), sWeaponNames[i]) != nullptr) {
                if (mCount80 == 0) {
                    sWeaponUsers++;
                }
                mCount80 = (sWeaponNames[i][2] - '0') * 10 + (sWeaponNames[i][3] - '0');
                if (sWeaponFile == nullptr && sWeaponData == nullptr) {
                    sWeaponFile = CDeviceFile::readFile(mtl::MemManager::getHandleMEM2(), "eff/rec/ev/evwpn01.rec",
                                                        this, 0, 0);
                    CDeviceFile::tryUpdateJobPriority(sWeaponFile, 3);
                    if (EvtSeqGetStateBit14()) {
                        CDeviceFile::setHandleFlag2(sWeaponFile);
                    }
                }
                break;
            }
        }
    }

    int delegated = 0;
    if (evtData()->mKind != 2) {
        s32 resKind;
        delegated = CfRes_tryDelegateLoad1(mResId5C, 4);
        CfRes_tryResolveToken(mResId5C, &resKind);
    }

    if (delegated == 0) {
        ml::FixStr<32> resName(Tok_FormatRes(mResId5C));
        mFlags |= 2;
        ml::FixStr<32> base(true);
        base = resName;
        int sep = base.rfind(lbl_eu_806623C0, -1);
        if (sep != -1 && sep != 0) {
            base = ml::FixStr<32>(base, 0, sep);
        }

        u32 type = evtData()->mKind;
        if (type - 3 <= 3) {
            ml::FixStr<64> path(true);
            if (evtData()->mKind == 4) {
                path = "/chr/jp/en/";
                mModelType = 2;
            } else if (evtData()->mKind == 3) {
                path = "/chr/jp/np/";
                mModelType = 1;
            } else if (evtData()->mKind == 5) {
                path = "/chr/jp/wp/";
                mModelType = 1;
            } else {
                path = "/obj/";
                mModelType = 1;
            }
            path += base.c_str();

            ml::FixStr<64> kpPath = path;
            kpPath += ".kp";
            int kpSize = getFileSize__11CDeviceFileFPCc(kpPath.c_str(), 1);
            ml::FixStr<64> chrPath = path;
            chrPath += ".chr";
            int size = getFileSize__11CDeviceFileFPCc(chrPath.c_str(), 1);
            if (size <= 0) {
                path += ".kp";
                size = kpSize;
            } else {
                path += ".chr";
            }

            if (size < 0) {
                mFlags |= 4;
            } else if (reinterpret_cast<UnkEvtListEntry*>(this)->SeqIsSameNameType3Busy() == 0) {
                void* buf = allocEvtMemAligned(size, 0x20, 0);
                if (buf != nullptr) {
                    mFileState = 1;
                } else {
                    buf = KyoshinHeap_AllocNode(size, 1) != nullptr ? KyoshinHeap_AllocNode(size, 0) : nullptr;
                    if (buf != nullptr) {
                        mAllocData3 = buf;
                        mFileState = 2;
                    }
                }

                if (buf != nullptr) {
                    mFileHandle = CDeviceFile::readCommonArchiveFile((mtl::ALLOC_HANDLE)buf, path.c_str(), this, 0, 0);
                } else {
                    mFileHandle = CDeviceFile::readFile(CLibLayout::getAllocHandle(), path.c_str(), this, 0, 0);
                    if (EvtSeqGetStateBit5()) {
                        setHandleParam__11CDeviceFileFP11CFileHandleUl(mFileHandle, getScnCounter__Fv());
                    }
                    setFileHeaps(mFileHandle, evtData()->mFlags, 0x40000);
                    if (evtData()->mFlags & 1) {
                        CDeviceFile::setHandleFlag1(mFileHandle);
                    }
                    if (EvtSeqGetStateBit14()) {
                        CDeviceFile::setHandleFlag2(mFileHandle);
                    }
                    mFileState = 3;
                }

                if (mFileHandle == nullptr) {
                    mFlags |= 4;
                }
            } else {
                mFlags |= 0x200;
            }
        } else {
            mFlags |= 4;
        }
    }

    if (mResId60 != 0 && evtData()->mKind == 3) {
        ml::FixStr<64> path(true);
        path = "/chr/jp/np/";
        path += evtData()->mName;
        path += ".chr";
        mFileHandle2 = CDeviceFile::readFile(CLibLayout::getAllocHandle(), path.c_str(), this, 0, 0);
        setFileHeaps(mFileHandle2, evtData()->mFlags, 0x20000);
        if (evtData()->mFlags & 1) {
            CDeviceFile::setHandleFlag1(mFileHandle2);
        }
        if (EvtSeqGetStateBit14()) {
            CDeviceFile::setHandleFlag2(mFileHandle2);
        }
    }
}

void CREvtModelObj::stepSetup() {
    scanPlayers();
    if (mFileHandle2 != nullptr) return;

    if (mFlags & 2) {
        if ((mFlags & 0x200) && reinterpret_cast<UnkEvtListEntry*>(this)->SeqIsSameNameType3Busy() == 0) {
            mFlags &= ~0x200;
            mCallback = static_cast<EvtStep>(&CREvtModelObj::stepLoad);
            return;
        }

        CREvtModelObj* other = EvtSeqFindSameNameType3(this);
        u8* otherData = nullptr;
        if (other != nullptr) {
            otherData = other->mAllocData;
        }
        if ((mFileHandle == nullptr && mAllocData != nullptr) || otherData != nullptr) {
            if (mFileHandle2 != nullptr) return;

            u8* data = mAllocData;
            mFlags |= 0x11;
            if (data == nullptr) {
                CREvtModelObj* owner = EvtSeqFindSameNameType3(this);
                mRefOwner = owner;
                if (owner->mAllocData == nullptr) {
                    data = nullptr;
                } else {
                    owner->mRefCount++;
                    data = owner->mAllocData;
                }
            }

            if ((u32)data & 0x1F) {
                nw4r::db::Panic(lbl_eu_8052637C, 0x3C, lbl_eu_80526354);
            }

            if (data[0] == 'K' && data[1] == 'Y' && data[2] == 'P') {
                KypEntry* entry = reinterpret_cast<KypEntry*>(data + 0x10);
                for (u32 i = 0; i < *reinterpret_cast<u32*>(data + 8); i++, entry++) {
                    for (const char** ext = sModelExts; *ext != nullptr; ext++) {
                        if (memcmp(entry->mName, *ext, strlen(*ext)) == 0) {
                            mExtractData = entry->mOffset + data;
                            break;
                        }
                    }
                    if (mExtractData != data) break;
                }
                mFlag64 = 1;
            }

            if (mExtractData == nullptr) {
                mExtractData = data;
            }

            CScn* scn = gmScene;
            u8* modelData = mExtractData;
            int priority = 7;
            if (evtData()->mFlags & 4) {
                priority = 8;
            }
            mModel = Scn_SetupAnim(scn, modelData, priority, 1);
            if (mModel != nullptr) {
                mModel->flags7A4 |= 0x20000000;
                simSetLeafDist7B0(mModel, 0.0f);
                mModel->flags7A8 |= 4;
                CScnItemModel* model = mModel;
                if (evtData()->mFlags & 8) {
                    model->flags7A8 |= 0x80;
                } else {
                    model->flags7A8 &= ~0x80;
                }
                if (evtData()->mKind - 2 <= 1) {
                    if (evtData()->mFlags & 0x400) {
                        mModel->vfunc64(4);
                    } else {
                        mModel->vfunc64(0);
                    }
                }
                if (mModel->flags7A4 & 0x800000) {
                    if (CfGameManager::getGameSubManager() != nullptr &&
                        reinterpret_cast<void**>(CfGameManager::getGameSubManager())[0x98 / 4] != nullptr) {
                        mModel->vfunc78(reinterpret_cast<void**>(CfGameManager::getGameSubManager())[0x98 / 4]);
                    }
                }
                if (mAllocData2 != nullptr) {
                    simCoupleModel(mModel, mAllocData2);
                    mModel->vfunc64(4);
                }
                if (mCount80 == 0 && !(evtData()->mFlags & 8)) {
                    mModel->vfunc84(1);
                }
                simSetFlag2OnTree(mModel, 0);
            }

            u32 kind, chara, sub, ext;
            Tok_Unpack(mResId5C, &kind, &chara, &sub, &ext);
            if (kind - 2 <= 1) {
                mModelType = 1;
            } else if (kind == 4) {
                mModelType = 2;
            } else if (kind == 6) {
                mModelType = 3;
            }
        } else {
            if (!(mFlags & 4)) return;
            mFlags |= 0x11;
        }
    } else {
        s32 resKind;
        u8* data = static_cast<u8*>(CfRes_tryResolveToken(mResId5C, &resKind));
        if (resKind == -1) {
            mFlags |= 0x11;
        } else {
            if (resKind == 0) return;
            mFlags |= 0x11;

            u8* base = data;
            if (data[0] == 'K' && data[1] == 'Y' && data[2] == 'P') {
                u32 count = *reinterpret_cast<u32*>(data + 8);
                KypEntry* entry = reinterpret_cast<KypEntry*>(data + 0x10);
                for (u32 i = 0; i < count; i++, entry++) {
                    for (const char** ext = sModelExts; *ext != nullptr; ext++) {
                        if (memcmp(entry->mName, *ext, strlen(*ext)) == 0) {
                            data = data + entry->mOffset;
                            break;
                        }
                    }
                    if (data != base) break;
                }
                mFlag64 = 1;
            }

            if (mExtractData == nullptr) {
                mExtractData = data;
            }

            mModel = Scn_SetupAnim(gmScene, mExtractData, -1, 1);
            if (mModel != nullptr) {
                simSetLeafDist7B0(mModel, 0.0f);
                mModel->flags7A4 |= 0x20000000;
                mModel->flags7A8 |= 4;
                if (evtData()->mKind - 2 <= 1) {
                    if (evtData()->mFlags & 0x400) {
                        mModel->vfunc64(4);
                    } else {
                        mModel->vfunc64(0);
                    }
                }
                if (mModel->flags7A4 & 0x800000) {
                    if (CfGameManager::getGameSubManager() != nullptr &&
                        reinterpret_cast<void**>(CfGameManager::getGameSubManager())[0x98 / 4] != nullptr) {
                        mModel->vfunc78(reinterpret_cast<void**>(CfGameManager::getGameSubManager())[0x98 / 4]);
                    }
                }
                if (mAllocData2 != nullptr) {
                    simCoupleModel(mModel, mAllocData2);
                    mModel->vfunc64(4);
                }
                if (mCount80 == 0 && !(evtData()->mFlags & 8)) {
                    mModel->vfunc84(1);
                }
                simSetFlag2OnTree(mModel, 0);
            }

            u32 kind, chara, sub, ext;
            Tok_Unpack(mResId5C, &kind, &chara, &sub, &ext);
            if (kind - 2 <= 1) {
                mModelType = 1;
            } else if (kind == 4) {
                mModelType = 2;
            } else if (kind == 6) {
                mModelType = 3;
            }
        }
    }

    if (mModel != nullptr) {
        if (mModelType <= 1) {
            mModel->vfunc9C(0, 0);
        } else if (mModelType == 2) {
            mModel->vfunc9C(1, 0);
        } else if (mModelType == 3) {
            mModel->vfunc9C(2, 0);
        }
    }
    mCallback = static_cast<EvtStep>(&CREvtModelObj::stepUpdate);
}

void CREvtModelObj::stepNone() {}

void CREvtModelObj::stepUpdate() {
    if (mModel != nullptr) {
        simRefreshFlag8(mModel, (int)EvtSeqGetCounter100());
    }
    checkRunGuard();
    scanPlayers();
}

void CREvtModelObj::scanPlayers() {
    if (sWeaponData == nullptr || mCount80 == 0) {
        return;
    }

    // State 0 or 2: scan for a player joining. Any other state: scan for a
    // player leaving. Both scans always run all 4 slots (no early exit).
    if (mState84 == 0 || mState84 == 2) {
        if (mSchedule != nullptr) return;

        int found = 0;
        int peer = -1;
        for (int i = 0; i < 4; i++) {
            s16 start = evtData()->mSlots[i].mStart;
            if (start > 0 && start <= EvtSeqGetCounter104() + 1) {
                found = 1;
                peer = evtData()->mSlots[i].mPeer;
            } else if (start < 0) {
                int count = labs(start);
                if (count <= EvtSeqGetCounter104() + 1) {
                    found = 0;
                }
            }
        }
        if (!found) return;

        mState84 = 1;
        if (mModel == nullptr || mData == nullptr) {
            return;
        }

        mSchedule = static_cast<CSchedule*>(
            EffRoot_AddSchedReq(&lbl_eu_8065FC18, sWeaponData, gmScene, mCount80 - 1, 1, 0));
        if (mSchedule == nullptr) return;

        schedAttachChildSlot(mSchedule, static_cast<CScheduleChild*>(this));
        if ((float)peer > 1.0f) {
            schedSetStepIntervals(mSchedule, (float)peer, 1.0f);
        }
        mSchedule->field_0x14 = mModel->vfuncA8();
        return;
    }

    for (int i = 0; i < 4; i++) {
        s16 start = evtData()->mSlots[i].mStart;
        if (start >= 0) continue;
        int count = labs(start);
        if (count == EvtSeqGetCounter104() + 1) {
            CSchedule* sched = mSchedule;
            mState84 = 2;
            if (sched == nullptr) return;
            if (evtData()->mSlots[i].mPeer != 0) {
                sched->field_0x59 = 1;
            } else {
                schedClearFlag15Update(sched);
                mSchedule = nullptr;
            }
            return;
        }
    }
}

bool CREvtModelObj::isBusy() {
    if (mField74 != EvtSeqGetWalkIndex() + 1) {
        return false;
    }

    bool result;
    // Only active while multiple players are running
    if (evtData()->mField30 <= 1) {
        result = false;
    } else {
        result = mCallback == static_cast<EvtStep>(&CREvtModelObj::stepLoad) ||
                 mCallback == static_cast<EvtStep>(&CREvtModelObj::stepSetup);
    }
    return result;
}

int CREvtModelObj::vfunc_20() { return 1; }

void CREvtModelObj::vfunc_24() {
    mFlags &= ~0x20;
}

bool CREvtModelObj::OnFileEvent(CEventFile* pEvent) {
    if (pEvent->mFileHandle == mFileHandle) {
        if (pEvent->unk0 == 1) {
            mAllocData = static_cast<u8*>(mFileHandle->getData());
            mFileId = pEvent->field_14;
        } else {
            mFlags |= 4;
        }
        mFileHandle = nullptr;
        return true;
    }

    if (pEvent->mFileHandle == mFileHandle2) {
        if (pEvent->unk0 == 1) {
            mAllocData2 = static_cast<u8*>(mFileHandle2->getData());
        } else {
            mFlags |= 4;
        }
        mFileHandle2 = nullptr;
        return true;
    }

    if (pEvent->mFileHandle == sWeaponFile) {
        if (pEvent->unk0 == 1) {
            sWeaponData = sWeaponFile->getData();
            EffSched_LookupA(&lbl_eu_8065FC18, sWeaponData);
        }
        sWeaponFile = nullptr;
    }

    return false;
}

void CREvtModelObj::OnScheduleDelete(CSchedule* pSchedule) {
    if (mSchedule == pSchedule) {
        mSchedule = nullptr;
    }
}

int CREvtModelObj::vfunc_14() {
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
