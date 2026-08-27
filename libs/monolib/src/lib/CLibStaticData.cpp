// Opt into the inline-empty ~IWorkEvent (see IWorkEvent.hpp): retail's
// CItem dtor elides the empty base-dtor call (0xC4, not 0xD0). Must be
// defined before any monolib header pull-in.
#define IWORK_EVENT_INLINE_DTOR

#include "monolib/lib.hpp"
#include "monolib/work.hpp"
#include "monolib/device.hpp"
#include "monolib/core.hpp"
#include <cstring>

using namespace mtl;

// Retail sbss singletons live in the dissolved monolibdata2 blob; the
// definitions are owned by CLibLayout.cpp (lbl_eu_80665718 = spInstance,
// lbl_eu_8066571C = sStaticArcFileListPtr). Declare them extern so this TU
// emits no .sbss storage.
extern CLibStaticData* lbl_eu_80665718;
#define spInstance lbl_eu_80665718
extern StaticArcFileData* lbl_eu_8066571C;
#include <decomp.h>

CLibStaticData::CLibStaticData(const char* pName, CWorkThread* pParent) :
CWorkThread(pName, pParent, MAX_CHILD),
mState(STATE_0),
mItems(){
    spInstance = this;
    mType = THREAD_CLIBSTATICDATA;
    mItems.reserve(mAllocHandle, MAX_ITEMS);
}

CLibStaticData::~CLibStaticData(){
    spInstance = nullptr;
}

CLibStaticData* CLibStaticData::getInstance(){
    return spInstance;
}

// Read-only view over the private CMsgParam<8> tail inside
// CWorkThread::mMsgQueue. The mEntries array ends at 0x1A4 (0x80 queue base +
// vtable + 8 * 0x24-byte entries); retail folds CMsgParam<8>::find()'s ring
// walk straight into isInitialized's body, so we scan the queue directly
// instead of calling the out-of-line find().
class CMsgQueueView {
public:
    u8 field_0x0[0x1A4];             //0x0..0x1A4 (CWorkThread head + queue vtable + entries)
    CMsgParamEntry* mArrayPtr;       //0x1A4 (CMsgParam::mArrayPtr)
    u32 mFront;                      //0x1A8 (CMsgParam::mFront)
    u32 mSize;                       //0x1AC (CMsgParam::mSize)
    u32 mCapacity;                   //0x1B0 (CMsgParam::mCapacity)
};

// True once the singleton thread has logged in and started running and no
// EVT_EXCEPTION is pending -- either flagged in mFlags or still queued in the
// message ring. The member isRunning() is inlined with 'this' bound to the
// singleton global.
bool CLibStaticData::isInitialized(){
    const CLibStaticData* inst = lbl_eu_80665718;

    bool exception;
    if (inst->mFlags & THREAD_FLAG_EXCEPTION) {
        exception = true;
    } else {
        // Inlined CMsgParam<8>::find(EVT_EXCEPTION): ring walk over the queue.
        const CMsgQueueView* queue = (const CMsgQueueView*)inst;
        int found;
        u32 i = 0;
        for (; i < queue->mSize; i++) {
            if (queue->mArrayPtr[(queue->mFront + i) % queue->mCapacity].command == EVT_EXCEPTION) {
                found = (int)i;
                goto merged;
            }
        }
        found = -1;
merged:
        exception = found >= 0;
    }

    // Running once the thread has logged in and started running. Note this
    // reads CWorkThread::mState (0x48), not the derived-class CLibStaticData
    // state machine field that shadows it.
    bool result = false;
    if (!exception) {
        bool stateOK = true;
        int state = inst->CWorkThread::mState;
        if (state != THREAD_STATE_LOGIN && state != THREAD_STATE_RUN) {
            stateOK = false;
        }
        if (stateOK) {
            result = true;
        }
    }
    return result;
}

void CLibStaticData::saveStaticFileArray(StaticArcFileData* pList){
    lbl_eu_8066571C = pList;
}

bool CLibStaticData::getStaticFileData(const char* pName, StaticDataHandle* pHandle, u32* r5){
    if(r5 != nullptr){
        *r5 = 0;
    }

    pHandle->data = nullptr;
    CLibStaticData* instance = spInstance;
    
    //Check if the item list contains an item with a matching name
    CItem* item;
    for(CItem** it = instance->mItems.begin(); it != instance->mItems.end(); it++){
        item = *it;
        if(std::strcmp(item->mFileData->mName, pName) == 0){
            pHandle->data = item->mData;

            if(r5 != nullptr){
                *r5 = (*it)->mLength;
            }

            bool result = false;

            if(pHandle != nullptr){
                CFileHandle* fileHandle = (*it)->mFileHandle;
                if(fileHandle == nullptr) result = true;
            }

            return result;
        }
    }

    return false;
}

bool CLibStaticData::wkStandbyLogin(){
    if(isNoEvent()) return CWorkThread::wkStandbyLogin();
    if(!CWorkSystemPack::arePacksLoaded()) return false;

    switch(mState){
        case STATE_0:
            if(!CDeviceGX::isInitialized()){
                return false;
            }

            //If the static arc file list is valid, create an entry for each file
            if(lbl_eu_8066571C != nullptr){
                StaticArcFileData* staticArcFileData = lbl_eu_8066571C;
                
                for(StaticArcFileData* it = lbl_eu_8066571C; it->mName != nullptr; it++){
                    CItem* item = new (CWorkThreadSystem::getWorkMem()) CItem(it);
                    mItems.push_back(item);
                }
            }

            mState++;
            //Fallthrough
        case STATE_1:
            //Check whether each file has been loaded. If one hasn't been loaded yet, exit early
            for(CItem** it = mItems.begin(); it != mItems.end(); it++){
                CItem* item = *it;
                if(item->mData == nullptr) return false;
            }

            mState++;
            //Fallthrough
        default:
            return CWorkThread::wkStandbyLogin();
    }

    return true;
}

bool CLibStaticData::wkStandbyLogout(){
    if(mChildren.empty() && CProcRoot::getInstance() == nullptr){
        for(CItem** it = mItems.begin(); it != mItems.end(); it++){
            if(*it != nullptr){
                delete *it;
                *it = nullptr;
            }
        }

        mItems.destroy();

        return CWorkThread::wkStandbyLogout();
    }

    return false;
}

CLibStaticData::CItem::CItem(StaticArcFileData* arcFileData) :
mFileData(arcFileData),
mFileHandle(nullptr),
mData(nullptr),
mLength(0),
unk14(false) {
    //Determine the handle to use for opening the file
    //TODO: This is likely an inline
    ALLOC_HANDLE handle = MemManager::getHandleMEM2();
    MemHandleType unk8 = arcFileData->mHandleType;
    if(unk8 == HANDLE_MEM1){
        handle = MemManager::getHandleMEM1();
    }else if(unk8 == HANDLE_STATIC){
        handle = MemManager::getHandleStatic();
    }

    //Try loading the file through CWorkSystemPack
    if(CWorkSystemPack::findPackResource(arcFileData->mPath, &mData, &mLength)){
        //If successful, call the loaded callback
        unk14 = true;
        if(mFileData->mFileLoadedCallback != nullptr){
            mFileData->mFileLoadedCallback(mData, mLength);
        }

    }else{
        //If it failed, read the file like normal
        mFileHandle = CDeviceFile::readFile(handle, arcFileData->mPath, this, 0, 0);
    }
}

CLibStaticData::CItem::~CItem(){
    if(mFileHandle != nullptr){
        CDeviceFile::cancel(mFileHandle);
        mFileHandle = nullptr;
    }

    if(mFileData->mFileUnloadedCallback != nullptr){
        mFileData->mFileUnloadedCallback(mData, mLength);
    }

    if(unk14){
        mData = nullptr;
    }

    DELETE_OBJ(mData);
}

bool CLibStaticData::CItem::OnFileEvent(CEventFile* pEventFile){
    if(pEventFile->unk0 == true){
        CFileHandle* fileHandle = pEventFile->mFileHandle;
        if(pEventFile->mFileHandle == mFileHandle){
            mData = mFileHandle->getData();
            mLength = mFileHandle->getLength();
            mFileHandle = nullptr;

            //Call the loaded callback
            if(mFileData->mFileLoadedCallback != nullptr){
                mFileData->mFileLoadedCallback(mData, mLength);
            }
        }

        return true;
    }

    return false;
}
