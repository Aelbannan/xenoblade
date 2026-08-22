// Opt into the inline-empty ~IWorkEvent (see IWorkEvent.hpp): retail compiles
// this dtor with the empty base-dtor body visible, so the base-dtor call is
// elided and the unit fits its 0x7C8 split budget.
#define IWORK_EVENT_INLINE_DTOR

#include "monolib/core.hpp"
#include "monolib/work.hpp"
#include "monolib/util.hpp"
#include "monolib/device.hpp"
#include <criware/cri_adxf.h>
#include <cstring>

extern const char lbl_eu_80524714[];
extern u32 lbl_eu_80663BC8;
extern const char lbl_eu_806623C0[];

// optimize_for_size on: retail prologue saves r29-r31 via stmw, not stw
#pragma optimize_for_size on
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
#pragma optimize_for_size off

// extern "C" free-function form (CArcItem pattern): no member key function is
// defined in this TU, so MWCC does not auto-emit __vt__9CPackItem here (the retail
// vtable lives in the dissolved .data blob below).
#pragma optimize_for_size on
extern "C" void __dt__9CPackItemFv(CPackItem* self){
    if(self->mFileHandle != nullptr){
        CDeviceFile::cancel(self->mFileHandle);
    }

    if(self->mPackHeaderExternal != 0){
        self->mPackHeader = nullptr;
    }

    if(self->mPackHeader != nullptr){
        mtl::MemManager::deallocate(self->mPackHeader);
        self->mPackHeader = nullptr;
    }

    if(self->mAhxAdxBuffer != nullptr){
        mtl::MemManager::deallocate(self->mAhxAdxBuffer);
        self->mAhxAdxBuffer = nullptr;
    }

    if((s32)self->field_0x68 >= 0){
        // field_0x68 >=0 triggers delete; retail delete call is via __dl__FPv after inline dtor elision
    }
    if((s32)self->field_0x68 >= 0){
        extern void __dl__FPv(void*);
        __dl__FPv(self);
    }
}
#pragma optimize_for_size off

/* Main update tick for pack file loading state machine.
   Transitions through: NOT_LOADED → OPENED_PKH_FILE → (LOADING_AHX_ADX_FILE) → LOADED */
#pragma optimize_for_size on
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
#pragma optimize_for_size off

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
extern "C" bool OnFileEvent__9CPackItemFP10CEventFile(CPackItem* self, CEventFile* pEventFile){
    if(pEventFile->mFileHandle == self->mFileHandle){
        if(pEventFile->unk0 == 1){
            // Local void* is required for matching (R7 overrides R1/R3)
            void* data = self->mFileHandle->mData;
            self->mFileHandle->mData = nullptr;
            self->mPackHeader = static_cast<PackHeader*>(data);
            self->setupHashTable();
        }else{
            self->mFileReadFailed = 1;
        }

        self->mFileHandle = nullptr;
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

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
namespace CPackItemBlob {
extern "C" void __dt__9CPackItemFv();
extern "C" void WorkEvent1__10IWorkEventFPvPCc();
extern "C" void OnFileEvent__9CPackItemFP10CEventFile();
extern "C" void WorkEvent3__10IWorkEventFPv();
extern "C" void WorkEvent4__10IWorkEventFv();
extern "C" void OnPauseTrigger__10IWorkEventFb();
extern "C" void WorkEvent6__10IWorkEventFv();
extern "C" void WorkEvent7__10IWorkEventFv();
extern "C" void WorkEvent8__10IWorkEventFv();
extern "C" void WorkEvent9__10IWorkEventFv();
extern "C" void WorkEvent10__10IWorkEventFv();
extern "C" void WorkEvent11__10IWorkEventFv();
extern "C" void WorkEvent12__10IWorkEventFv();
extern "C" void WorkEvent13__10IWorkEventFv();
extern "C" void WorkEvent14__10IWorkEventFv();
extern "C" void WorkEvent15__10IWorkEventFv();
extern "C" void WorkEvent16__10IWorkEventFv();
extern "C" void WorkEvent17__10IWorkEventFv();
extern "C" void WorkEvent18__10IWorkEventFv();
extern "C" void WorkEvent19__10IWorkEventFv();
extern "C" void WorkEvent20__10IWorkEventFv();
extern "C" void WorkEvent21__10IWorkEventFv();
extern "C" void WorkEvent22__10IWorkEventFv();
extern "C" void WorkEvent23__10IWorkEventFv();
extern "C" void WorkEvent24__10IWorkEventFv();
extern "C" void WorkEvent25__10IWorkEventFv();
extern "C" void WorkEvent26__10IWorkEventFv();
extern "C" void WorkEvent27__10IWorkEventFv();
extern "C" void WorkEvent28__10IWorkEventFv();
extern "C" void WorkEvent29__10IWorkEventFv();
extern "C" void WorkEvent30__10IWorkEventFv();
extern "C" void WorkEvent31__10IWorkEventFv();
}

extern "C" __declspec(align(4)) const char lbl_eu_805246FC[0xC] = {
    0x43,0x50,0x61,0x63,0x6B,0x49,0x74,0x65,0x6D,0x00,0x00,0x00,
};
extern "C" u32 lbl_eu_80663BF8[2] = { (u32)&lbl_eu_805246FC, 0 };

// [.data] 0x8056FF58-0x8056FFE0 (0x88): CPackItem vtable
extern "C" u32 lbl_eu_8056FF58[34] = {
    (u32)&lbl_eu_80663BF8, 0x00000000,
    (u32)&CPackItemBlob::__dt__9CPackItemFv,
    (u32)&CPackItemBlob::WorkEvent1__10IWorkEventFPvPCc,
    (u32)&CPackItemBlob::OnFileEvent__9CPackItemFP10CEventFile,
    (u32)&CPackItemBlob::WorkEvent3__10IWorkEventFPv,
    (u32)&CPackItemBlob::WorkEvent4__10IWorkEventFv,
    (u32)&CPackItemBlob::OnPauseTrigger__10IWorkEventFb,
    (u32)&CPackItemBlob::WorkEvent6__10IWorkEventFv,
    (u32)&CPackItemBlob::WorkEvent7__10IWorkEventFv,
    (u32)&CPackItemBlob::WorkEvent8__10IWorkEventFv,
    (u32)&CPackItemBlob::WorkEvent9__10IWorkEventFv,
    (u32)&CPackItemBlob::WorkEvent10__10IWorkEventFv,
    (u32)&CPackItemBlob::WorkEvent11__10IWorkEventFv,
    (u32)&CPackItemBlob::WorkEvent12__10IWorkEventFv,
    (u32)&CPackItemBlob::WorkEvent13__10IWorkEventFv,
    (u32)&CPackItemBlob::WorkEvent14__10IWorkEventFv,
    (u32)&CPackItemBlob::WorkEvent15__10IWorkEventFv,
    (u32)&CPackItemBlob::WorkEvent16__10IWorkEventFv,
    (u32)&CPackItemBlob::WorkEvent17__10IWorkEventFv,
    (u32)&CPackItemBlob::WorkEvent18__10IWorkEventFv,
    (u32)&CPackItemBlob::WorkEvent19__10IWorkEventFv,
    (u32)&CPackItemBlob::WorkEvent20__10IWorkEventFv,
    (u32)&CPackItemBlob::WorkEvent21__10IWorkEventFv,
    (u32)&CPackItemBlob::WorkEvent22__10IWorkEventFv,
    (u32)&CPackItemBlob::WorkEvent23__10IWorkEventFv,
    (u32)&CPackItemBlob::WorkEvent24__10IWorkEventFv,
    (u32)&CPackItemBlob::WorkEvent25__10IWorkEventFv,
    (u32)&CPackItemBlob::WorkEvent26__10IWorkEventFv,
    (u32)&CPackItemBlob::WorkEvent27__10IWorkEventFv,
    (u32)&CPackItemBlob::WorkEvent28__10IWorkEventFv,
    (u32)&CPackItemBlob::WorkEvent29__10IWorkEventFv,
    (u32)&CPackItemBlob::WorkEvent30__10IWorkEventFv,
    (u32)&CPackItemBlob::WorkEvent31__10IWorkEventFv,
};

DECOMP_FORCEACTIVE(CPackItem_cpp, lbl_eu_8056FF58);
DECOMP_FORCEACTIVE(CPackItem_cpp, lbl_eu_805246FC);
DECOMP_FORCEACTIVE(CPackItem_cpp, lbl_eu_80663BF8);

// data: retail sections verified via run.py data diff (no bypass)
