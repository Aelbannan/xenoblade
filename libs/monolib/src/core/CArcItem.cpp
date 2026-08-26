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
// operator delete(void*) (retail unmangled C symbol).
extern "C" void __dl__FPv(void*);
// CArcItem vtable, spelled by hand in the dissolved .data blob below.
extern "C" u32 lbl_eu_8056FFE0[];

#pragma optimize_for_size on
// Installing destructor, spelled as an extern "C" free function against the
// dissolved .data blob vtable below (CFontLayer pattern): no member key
// function is defined in this TU, so MWCC does not auto-emit __vt__8CArcItem.
// Retail body: hoist unk28, install the blob vptr, cancel the pending read,
// free+clear the buffer, then operator-delete self when mode > 0.
extern "C" void* __dt__8CArcItemFv(CArcItem* self, int mode) {
    if (self != nullptr) {
        CFileHandle* h = self->unk28;
        *(u32*)self = (u32)&lbl_eu_8056FFE0;
        if (h != nullptr) {
            CDeviceFile::cancel(h);
        }
        if (self->unk38 != nullptr) {
            mtl::MemManager::deallocate(self->unk38);
            self->unk38 = nullptr;
        }
        if (mode > 0) {
            __dl__FPv(self);
        }
    }
    return self;
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

extern "C" bool OnFileEvent__8CArcItemFP10CEventFile(CArcItem* self, CEventFile* pEventFile){
    if(pEventFile->mFileHandle == self->unk28){
        if(pEventFile->unk0 == true){
            self->unk38 = self->unk28->getData();
            if(!ARCInitHandle(self->unk38, &self->mArcHandle)){
                self->unk30 = 1;
            }
        }else{
            self->unk30 = 1;
        }

        self->unk28 = 0;
        return true;
    }

    return false;
}

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// Forward refs: own-class methods (dtor/OnFileEvent) and the base IWorkEvent
// virtual stubs, referenced by retail mangled names (CMdlLook D2Blob pattern).
namespace CArcItemBlob {
extern "C" void __dt__8CArcItemFv();
extern "C" void OnFileEvent__8CArcItemFP10CEventFile();
extern "C" void WorkEvent1__10IWorkEventFPvPCc();
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

extern "C" const char lbl_eu_80524708[0x28];
extern "C" u32 lbl_eu_80663C00[2];

// [.data] 0x8056FFE0-0x80570068 (0x88 = 136B): CArcItem vtable
extern "C" u32 lbl_eu_8056FFE0[34] = {
    (u32)&lbl_eu_80663C00, 0x00000000,
    (u32)&CArcItemBlob::__dt__8CArcItemFv,
    (u32)&CArcItemBlob::WorkEvent1__10IWorkEventFPvPCc,
    (u32)&CArcItemBlob::OnFileEvent__8CArcItemFP10CEventFile,
    (u32)&CArcItemBlob::WorkEvent3__10IWorkEventFPv,
    (u32)&CArcItemBlob::WorkEvent4__10IWorkEventFv,
    (u32)&CArcItemBlob::OnPauseTrigger__10IWorkEventFb,
    (u32)&CArcItemBlob::WorkEvent6__10IWorkEventFv,
    (u32)&CArcItemBlob::WorkEvent7__10IWorkEventFv,
    (u32)&CArcItemBlob::WorkEvent8__10IWorkEventFv,
    (u32)&CArcItemBlob::WorkEvent9__10IWorkEventFv,
    (u32)&CArcItemBlob::WorkEvent10__10IWorkEventFv,
    (u32)&CArcItemBlob::WorkEvent11__10IWorkEventFv,
    (u32)&CArcItemBlob::WorkEvent12__10IWorkEventFv,
    (u32)&CArcItemBlob::WorkEvent13__10IWorkEventFv,
    (u32)&CArcItemBlob::WorkEvent14__10IWorkEventFv,
    (u32)&CArcItemBlob::WorkEvent15__10IWorkEventFv,
    (u32)&CArcItemBlob::WorkEvent16__10IWorkEventFv,
    (u32)&CArcItemBlob::WorkEvent17__10IWorkEventFv,
    (u32)&CArcItemBlob::WorkEvent18__10IWorkEventFv,
    (u32)&CArcItemBlob::WorkEvent19__10IWorkEventFv,
    (u32)&CArcItemBlob::WorkEvent20__10IWorkEventFv,
    (u32)&CArcItemBlob::WorkEvent21__10IWorkEventFv,
    (u32)&CArcItemBlob::WorkEvent22__10IWorkEventFv,
    (u32)&CArcItemBlob::WorkEvent23__10IWorkEventFv,
    (u32)&CArcItemBlob::WorkEvent24__10IWorkEventFv,
    (u32)&CArcItemBlob::WorkEvent25__10IWorkEventFv,
    (u32)&CArcItemBlob::WorkEvent26__10IWorkEventFv,
    (u32)&CArcItemBlob::WorkEvent27__10IWorkEventFv,
    (u32)&CArcItemBlob::WorkEvent28__10IWorkEventFv,
    (u32)&CArcItemBlob::WorkEvent29__10IWorkEventFv,
    (u32)&CArcItemBlob::WorkEvent30__10IWorkEventFv,
    (u32)&CArcItemBlob::WorkEvent31__10IWorkEventFv,
};

// [.rodata] 0x80524708-0x80524730 (0x28 = 40B): "CArcItem\0..." + "AHX_PKH\0adx\0ahx\0.pkb\0"
extern "C" __declspec(align(4)) const char lbl_eu_80524708[0x28] = {
    0x43,0x41,0x72,0x63,0x49,0x74,0x65,0x6D,0x00,0x00,0x00,0x00,
    0x41,0x48,0x58,0x5F,0x50,0x4B,0x48,0x00,0x61,0x64,0x78,0x00,
    0x61,0x68,0x78,0x00,0x2E,0x70,0x6B,0x62,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00
};

// [.sdata] 0x80663C00-0x80663C08 (8B)
extern "C" u32 lbl_eu_80663C00[2] = { (u32)&lbl_eu_80524708, 0x00000000 };
