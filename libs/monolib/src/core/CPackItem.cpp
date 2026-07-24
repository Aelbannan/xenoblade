#include "monolib/core.hpp"
#include "monolib/work.hpp"
#include "monolib/util.hpp"
#include "monolib/device.hpp"
#include <criware/cri_adxf.h>
#include <cstring>

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
mAhxAdxBuffer(nullptr),
mLoadState(LOAD_STATE_NOT_LOADED),
mFileReadFailed(0),
mPackHeaderExternal(0),
mIsAhxAdxFile(false),
mWorkPackDataPtr(0),
mWorkPackDataSize(0) {
    //Check if the file is a adx/ahx pack file from the filename
    if(std::strstr(name, "adx") != nullptr || std::strstr(name, "ahx") != nullptr){
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

    DELETE_OBJ(mPackHeader);
    DELETE_OBJ(mAhxAdxBuffer);
}

/* Main update tick for pack file loading state machine.
   Transitions through: NOT_LOADED → OPENED_PKH_FILE → (LOADING_AHX_ADX_FILE) → LOADED */
void CPackItem::update(){
    if(mLoadState == LOAD_STATE_NOT_LOADED){
        if(CWorkSystemPack::func_804DE100() == 0) return;

        if(CWorkSystemPack::func_804DDDF4(mFilePath, &mWorkPackDataPtr, &mWorkPackDataSize) != 0){
            mPackHeaderExternal = 1;
            mPackHeader = (PackHeader*)mWorkPackDataPtr;
            setupHashTable();
        }else{
            mFileHandle = CDeviceFile::readFile(mtl::MemManager::getHandleMEM2(), mFilePath, this, 0, 0);
        }

        //TODO: What did they do here? They likely didnt have a constructor like this to skip
        //initialization, but using a raw buffer feels wrong
        ml::FixStr<64> tempString = ml::FixStr<64>(false); //0x2C

        ml::CPathUtil::getNoPathExtName(tempString, mFilePath);
        mBaseName = tempString.c_str();
        mPkbFilename = mFilePath;
        ml::CPathUtil::removeExt(mPkbFilename);
        mPkbFilename += ".pkb";
        mLoadState = LOAD_STATE_OPENED_PKH_FILE;
    }else if(mLoadState == LOAD_STATE_OPENED_PKH_FILE){
        if(mFileReadFailed != 0){
            mLoadState = LOAD_STATE_LOADED;
            return;
        }

        if(mPackHeader == nullptr) return;

        if(mIsAhxAdxFile){
            if(CWorkSystemPack::func_804DDFBC((u32)this) == false) return;
            u32 bufferSize = ROUND_DOWN((mPackHeader->mFiles + 1)*2 + 0x11a, 4); //TODO: figure out the corresponding Criware struct here.
            mAhxAdxBuffer = (u8*)mtl::MemManager::allocate_head(mtl::MemManager::getHandleMEM2(), bufferSize, 4);
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
   Each hash table entry is two u32s (lower half, upper half) stored
   consecutively. The table is accessed as a flat u32 array for matching. */
int CPackItem::findHashIndex(int startIndex, int endIndex){
    int length = endIndex - startIndex;

    if(length < 16){
        //If the length is less than 16, just search for a match with a for loop
        // Access the hash table as u32* for word-by-word comparison (required for matching)
        u32* pHashTable = (u32*)&mFileHashTable[startIndex];
        int i = startIndex;

        while(i < endIndex){
            if(pHashTable[0] == mHashLowerHalf && pHashTable[1] == mHashUpperHalf){
                return i;
            }

            i++;
            pHashTable += 2; // advance by two u32s per entry
        }
        
        //If the hash couldn't be found, return -1
        return -1;
    }

    //If not, use binary search to narrow down the search space
    int midIndex = (endIndex + startIndex)/2;
    // Access the hash table as u32* for word-by-word comparison (required for matching)
    u32* pHashTable = (u32*)&mFileHashTable[midIndex];

    /* If the entry at the middle index happens to be the right one, return the index. If not,
    call the function again, choosing the left portion if any of the two hash values are larger,
    and the right portion otherwise. */
    if(pHashTable[1] == mHashUpperHalf){
        if(pHashTable[0] == mHashLowerHalf){
            //The middle entry matches the values of this item, return the index
            return midIndex;
        }else if(pHashTable[0] > mHashLowerHalf){
            return findHashIndex(startIndex, midIndex);
        }else{
            return findHashIndex(midIndex, endIndex);
        }
    }else if(pHashTable[1] > mHashUpperHalf){
        return findHashIndex(startIndex, midIndex);
    }else{
        return findHashIndex(midIndex, endIndex);
    }
}

bool CPackItem::isNotLoaded(){
    return mLoadState != LOAD_STATE_LOADED && !mIsAhxAdxFile;
}

/* Sets up the hash table and per-file ID pointers from the pack header.
   The layout after the hash table is: mFileIds (u16 per file), then optionally
   mFileDataOffsets (u32 per file) if the pkh file is large enough. */
void CPackItem::setupHashTable() {
    if(mPackHeader != nullptr){
        mFileHashTable = mPackHeader->mFileHashTable;
        // mFileIds starts right after the hash table entries
        mFileIds = (u16*)&mFileHashTable[mPackHeader->mFiles];

        // Check if there is room for mFileDataOffsets after mFileIds
        u32 fileIdsEnd = (u32)(mFileIds + mPackHeader->mFiles);
        if(mPackHeader->mPkhFilesize > fileIdsEnd - (u32)mPackHeader){
            mFileDataOffsets = (u32*)(mFileIds + mPackHeader->mFiles);
        }
    }
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
        u32 byteIndex = mPackHeader->mHashValTable[i] / 8;
        u32 bitIndex = mPackHeader->mHashValTable[i] % 8;
        u8 mask = 1 << bitIndex;
        if(length - 1 >= byteIndex){
            bool bit = (u8)filename[length - 1 - byteIndex] & mask;
            //If the index is more than 32, write to the high 32 bit variable
            if(i >= 32){
                mHashUpperHalf |= (1 << (i - 32)) * bit;
            }else{
                mHashLowerHalf |= (1 << i) * bit;
            }
        }
    }
    
    return true;
}
