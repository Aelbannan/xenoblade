// Opt into the inline-empty ~IWorkEvent (see IWorkEvent.hpp): retail compiles
// this dtor with the empty base-dtor body visible, so the base-dtor call is
// elided and the unit fits its 0x294 split budget (the call would add 0xC).
#define IWORK_EVENT_INLINE_DTOR

#include "monolib/core.hpp"
#include "monolib/device.hpp"
#include "monolib/util.hpp"
#include <revolution/ARC.h>
#include <cstring>

// optimize_for_size on: retail prologue saves r30+r31 via stmw, not stw
// (same pattern as CTaskGameEff / CRsrcData dtor).
#pragma optimize_for_size on
CArcItem::CArcItem(const char* pFilename) :
unk4(),
unk28(nullptr),
unk2C(0),
unk30(0),
unk34(pFilename),
unk38(nullptr){
    unk28 = CDeviceFile::readFile(mtl::MemManager::getHandleMEM2(), pFilename, this, 0, 0);
    ml::FixStr<64> string = ml::FixStr<64>(false);
    ml::CPathUtil::getNoPathExtName(string, pFilename);
    //This line gets included in the DESTROYBASE extab PC action range
    unk4 = string.c_str();
}
#pragma optimize_for_size off

// optimize_for_size on: retail __dt__ saves r30+r31 via stmw r30, not stw
// (same pattern as CTTask/CRsrcData dtors).
#pragma optimize_for_size on
CArcItem::~CArcItem(){
    if(unk28 != nullptr){
        CDeviceFile::cancel(unk28);
    }

    DELETE_OBJ(unk38);
}
#pragma optimize_for_size off

void CArcItem::func_804DEC30(){
    if(unk2C == 0){
        //Won't match with ||. Why do it like this?
        if(unk30 != 0) unk2C = 2;
        else if(unk38 != nullptr) unk2C = 2;
    }
}

// optimize_for_size on: retail saves r29-r31 via stmw, not stw.
#pragma optimize_for_size on
bool CArcItem::func_804DEC6C(const char* pPath, void** pOutStartAddr, u32* pOutLength){
    if(unk38 == 0 || unk30 != 0) return false;
    s32 entryNum = ARCConvertPathToEntrynum(&mArcHandle, pPath);
    //Return if the entry number is invalid
    if(entryNum < 0) return false;

    ARCFileInfo fileInfo;
    if(ARCFastOpen(&mArcHandle, entryNum, &fileInfo) != false){
        *pOutStartAddr = ARCGetStartAddrInMem(&fileInfo);
        *pOutLength = ARCGetLength(&fileInfo);
        ARCClose(&fileInfo);
        return true;
    }
    
    return false;
}
#pragma optimize_for_size off

bool CArcItem::OnFileEvent(CEventFile* pEventFile){
    if(pEventFile->mFileHandle == unk28){
        if(pEventFile->unk0 == true){
            unk38 = unk28->getData();
            if(!ARCInitHandle(unk38, &mArcHandle)){
                unk30 = 1;
            }
        }else{
            unk30 = 1;
        }

        unk28 = 0;
        return true;
    }

    return false;
}
