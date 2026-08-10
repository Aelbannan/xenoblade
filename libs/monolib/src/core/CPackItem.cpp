#include "monolib/core.hpp"
#include "monolib/work.hpp"
#include "monolib/util.hpp"
#include "monolib/device.hpp"
#include <criware/cri_adxf.h>
#include <cstring>

extern const char lbl_eu_80524714[];
extern u32 lbl_eu_80663BC8;
extern const char lbl_eu_806623C0[];
CPackItem::CPackItem(const char* name, int partitionId) :
mBaseName(),
mPkbFilename(),
mFileHandle(nullptr),
mPackHeader(nullptr),
mArchiveName(name),
mFilePath(name),
mFileHashTable(nullptr),
mFileIds(nullptr),
mFileDataOffsets(nullptr),
mAdxPartitionId(partitionId),
field_0x68(0),
mAhxAdxBuffer(nullptr),
mLoadState(LOAD_STATE_NOT_LOADED),
mFileReadFailed(0),
mPackHeaderExternal(0),
mIsAhxAdxFile(false),
mWorkPackDataPtr(0),
mWorkPackDataSize(0) {
    // Check if the file is an adx/ahx pack file from the filename.
    // Retail uses a shared string table at lbl_eu_80524714 + 0x8 ("adx") and +0xC ("ahx").
    if(std::strstr(name, lbl_eu_80524714 + 8) != nullptr || std::strstr(name, lbl_eu_80524714 + 0xC) != nullptr){
        mIsAhxAdxFile = true;
    }
}

CPackItem::~CPackItem(){
    if(mFileHandle != nullptr){
        CDeviceFile::cancel(mFileHandle);
    }

    if(mPackHeaderExternal != 0){
        mPackHeader = nullptr;
    }

    if(mPackHeader != nullptr){
        mtl::MemManager::deallocate(mPackHeader);
        mPackHeader = nullptr;
    }

    if(mAhxAdxBuffer != nullptr){
        mtl::MemManager::deallocate(mAhxAdxBuffer);
        mAhxAdxBuffer = nullptr;
    }
}

/* Main update tick for pack file loading state machine.
   Transitions through: NOT_LOADED → OPENED_PKH_FILE → (LOADING_AHX_ADX_FILE) → LOADED */
void CPackItem::update(){
    ml::FixStr<32> localStr(false);
    int dotPos;

    if(mLoadState == LOAD_STATE_NOT_LOADED){
        if(CWorkSystemPack::func_804DE100() == 0) return;

        if(CWorkSystemPack::func_804DDDF4(mFilePath, &mWorkPackDataPtr, &mWorkPackDataSize) != 0){
            mPackHeaderExternal = 1;
            mPackHeader = (PackHeader*)mWorkPackDataPtr;
            setupHashTable();
        }else{
            if((s32)field_0x68 >= 0){
                func_eu_804521A8(field_0x68);
            }
            
            mFileHandle = CDeviceFile::readFile(lbl_eu_80663BC8, mFilePath, this, 0, 0);
            func_8044F400__11CDeviceFileFP11CFileHandleUl(mFileHandle, mtl::MemManager::getHandleMEM2());
            
            if((s32)field_0x68 >= 0){
                func_eu_804521B0();
            }
        }

        // Extract base name (without path or extension)
        ml::FixStr<64> tempString = ml::FixStr<64>(false);
        ml::CPathUtil::getNoPathExtName(tempString, mFilePath);
        mBaseName = tempString.c_str();

        // Copy full path, strip extension, then append ".pkb"
        mPkbFilename = mFilePath;

        // Find last '.' in the filename
        dotPos = mPkbFilename.rfind(lbl_eu_806623C0, -1);

        // Truncate at extension if found
        if((u32)(dotPos + 1) > 1){
            localStr.clear();

            if(mPkbFilename.mLength != 0){
                if(dotPos == -1){
                    dotPos = mPkbFilename.mLength;
                }
                strncpy(localStr.mString, mPkbFilename.mString, dotPos);
                localStr.mString[dotPos] = '\0';
                localStr.mLength = strlen(localStr.mString);
            }

            mPkbFilename = localStr;
        }

        mPkbFilename += lbl_eu_80524714 + 0x10;
        func_eu_804520D0(mPkbFilename.mString);

        mLoadState = LOAD_STATE_OPENED_PKH_FILE;
    }else if(mLoadState == LOAD_STATE_OPENED_PKH_FILE){
        if(mFileReadFailed != 0){
            mLoadState = LOAD_STATE_LOADED;
            return;
        }

        if(mPackHeader == nullptr) return;

        if(mIsAhxAdxFile){
            if(CWorkSystemPack::func_804DDFBC((u32)this) == 0) return;
            u32 bufferSize = ((mPackHeader->mFiles + 1) * 2 + 0x11a) & ~3;
            mAhxAdxBuffer = (u8*)mtl::MemManager::allocate_head(CWorkThreadSystem::getWorkMem(), bufferSize, 4);
            ADXF_LoadPartitionNw(mAdxPartitionId, mPkbFilename.c_str(), nullptr, mAhxAdxBuffer);
            mLoadState = LOAD_STATE_LOADING_AHX_ADX_FILE;
        }else{
            mLoadState = LOAD_STATE_LOADED;
        }
    }else if(mLoadState == LOAD_STATE_LOADING_AHX_ADX_FILE){
        if(ADXF_GetPtStat(mAdxPartitionId) == ADXF_STAT_READEND){
            mLoadState = LOAD_STATE_LOADED;
        }
    }
}

/* Looks up a file by name in the pack's hash table.
   On success, returns true and fills outPkbPath, outEntryId, outIndex, and outFileId. */
bool CPackItem::lookupFile(const char* filename, char** outPkbPath, u32* outEntryId, u32* outIndex, u32* outFileId){
    if(mPackHeader == nullptr){
        return false;
    }

    if(calculatePackFileHash(filename) == false){
        return false;
    }

    int hashIndex = findHashIndex(0, mPackHeader->mFiles);
    
    if(hashIndex < 0){
        return false;
    }

    *outPkbPath = (char*)mPkbFilename.c_str();

    if(mFileDataOffsets != nullptr){
        *outEntryId = mFileDataOffsets[hashIndex];
    }else{
        *outEntryId = mAdxPartitionId;
    }

    *outIndex = hashIndex;
    *outFileId = mFileIds[hashIndex];
    return true;
}

/* Tries to locate the hash of this item in the hash table. If successful,
returns the corresponding index. If not successful, returns -1.
   Each hash table entry is two 32-bit words (hash lower half, then upper
   half) packed into one 8-byte slot, so the table is walked as u32 pairs.
   If the table is small (< 16 entries) a linear scan is used; otherwise a
   binary search descending from the mid index. */
int CPackItem::findHashIndex(int startIndex, int endIndex){
    int length = endIndex - startIndex;

    if(length < 16){
        // Linear scan over the small range.
        u32* entry = (u32*)(mFileHashTable + startIndex);
        int i;
        for(i = startIndex; i < endIndex; i++){
            if(entry[0] == mHashLowerHalf && entry[1] == mHashUpperHalf){
                return i;
            }
            entry += 2;
        }

        //If the hash couldn't be found, return -1
        return -1;
    }

    // Binary search: narrow down the search space toward the mid index.
    int midIndex = (endIndex + startIndex) / 2;
    u32* entry = (u32*)(mFileHashTable + midIndex);

    /* If the entry at the middle index happens to be the right one, return the index. If not,
    call the function again, choosing the left portion if the value is larger,
    and the right portion otherwise. */
    if(entry[1] == mHashUpperHalf){
        if(entry[0] == mHashLowerHalf){
            //The middle entry matches the values of this item, return the index
            return midIndex;
        }else if(entry[0] > mHashLowerHalf){
            return findHashIndex(startIndex, midIndex);
        }else{
            return findHashIndex(midIndex, endIndex);
        }
    }else if(entry[1] > mHashUpperHalf){
        return findHashIndex(startIndex, midIndex);
    }else{
        return findHashIndex(midIndex, endIndex);
    }
}

bool CPackItem::isNotLoaded(){
    return mLoadState != LOAD_STATE_LOADED && !mIsAhxAdxFile;
}

/* Handles async file read completion events.
   On success, takes ownership of the file data as a PackHeader and sets up the hash table.
   On failure, sets mFileReadFailed to move the state machine past the file-open phase. */
bool CPackItem::OnFileEvent(CEventFile* pEventFile){
    if(pEventFile->mFileHandle == mFileHandle){
        if(pEventFile->unk0 == 1){
            // Local void* is required for matching (R7 overrides R1/R3)
            void* data = mFileHandle->mData;
            mFileHandle->mData = nullptr;
            mPackHeader = static_cast<PackHeader*>(data);
            setupHashTable();
        }else{
            mFileReadFailed = 1;
        }

        mFileHandle = nullptr;
        return true;
    }

    return false;
}

/* Calculates the hash of the given file name string. This is used to
find where the file is located in the pkb archive using the hash table
in the pkh file. */
bool CPackItem::calculatePackFileHash(const char* filename){
    if(mPackHeader == nullptr){
        return false;
    }

    mHashLowerHalf = 0;
    mHashUpperHalf = 0;
    
    int length = std::strlen(filename);

    //Return if the string is empty
    if(length == 0){
        return false;
    }

    u32 hashValTableLength = mPackHeader->mHashValTableLength;

    for(u32 i = 0; i < hashValTableLength; i++){
        u8 val = mPackHeader->mHashValTable[i];
        u32 byteIndex = val / 8;         // which byte of the filename this bit tests
        u8 mask = (u8)(1u << (val % 8)); // bit position within that byte
        if((u32)(length - 1) >= byteIndex){
            u8 ch = (u8)filename[(length - 1) - byteIndex];
            bool bit = (ch & mask) != 0;
            if(i < 32){
                mHashLowerHalf |= (1u << i) * bit;
            }else{
                //The index is more than 32, write to the high 32 bit variable
                mHashUpperHalf |= (1u << (i - 32)) * bit;
            }
        }
    }

    return true;
}
