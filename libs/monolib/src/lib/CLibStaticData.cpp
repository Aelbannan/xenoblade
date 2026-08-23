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

// Read-only view over the singleton's thread flag / message-queue tail fields
struct CMsgParamEntryView {
    u32 command; //0x0
    u32 wid; //0x4
    u32 unk8;
    u32 unkC;
    u32 unk10;
    u32 unk14;
    u32 unk18;
    u32 unk1C;
    u16 unk20;
    u8 unk22;
    u8 unk23;
};

// (the private CMsgParam<8> members mArrayPtr/mFront/mSize/mCapacity sit at
// 0x1A4..0x1B0 inside CWorkThread::mMsgQueue). The retail inlines
// CMsgParam<8>::find(EVT_EXCEPTION) here, so we scan the queue directly.
class CWorkThreadFieldsView {
public:
    u8 field_0x0[0x48];              //0x0
    int mState;                      //0x48 (CWorkThread::ThreadState)
    u8 field_0x4C[0x7C - 0x4C];      //0x4C
    u32 mThreadFlags;                //0x7C (CWorkThread::ThreadFlags)
    u8 field_0x80[0x1A4 - 0x80];     //0x80..0x1A4 (queue vtable + entries)
    CMsgParamEntryView* mMsgArray;   //0x1A4 (CMsgParam::mArrayPtr)
    u32 mMsgFront;                   //0x1A8 (CMsgParam::mFront)
    u32 mMsgSize;                    //0x1AC (CMsgParam::mSize)
    u32 mMsgCapacity;                //0x1B0 (CMsgParam::mCapacity)
};

// Retail folds CMsgParam<8>::find(EVT_EXCEPTION)'s ring walk straight into
// this call site, with the this-arg bound to the instance: view-cast THIS
// (born at entry, ahead of the loop index -> inst claims the lower volatile)
// rather than re-loading the singleton global.
inline bool CWorkThread::isRunning() const {
    // Loop index declared first: first-declared claims the higher volatile,
    // so i lands in r7 and the instance view in r6, matching retail.
    u32 i = 0;
    const CWorkThreadFieldsView* inst = (const CWorkThreadFieldsView*)this;

    bool exception;
    if (inst->mThreadFlags & THREAD_FLAG_EXCEPTION) {
        exception = true;
    } else {
        // Inlined queue scan for a queued EVT_EXCEPTION event.
        int found;
        for (; i < inst->mMsgSize; i++) {
            if (inst->mMsgArray[(inst->mMsgFront + i) % inst->mMsgCapacity].command == EVT_EXCEPTION) {
                found = (int)i;
                goto merged;
            }
        }
        found = -1;
merged:
        exception = found >= 0;
    }

    // Running once the thread has logged in and started running.
    if (exception) {
        return false;
    }
    return inst->mState == THREAD_STATE_LOGIN || inst->mState == THREAD_STATE_RUN;
}
bool CLibStaticData::isInitialized(){
    extern CLibStaticData* lbl_eu_80665718;
    return lbl_eu_80665718->isRunning();
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
    if(!CWorkSystemPack::func_804DE08C()) return false;

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
    if(CWorkSystemPack::func_804DDDF4(arcFileData->mPath, &mData, &mLength)){
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

void __ct__Q214CLibStaticData5CItemFP17StaticArcFileData(){}
void __dt__Q214CLibStaticData5CItemFv(){}
