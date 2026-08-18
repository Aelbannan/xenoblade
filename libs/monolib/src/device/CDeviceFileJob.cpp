#include "monolib/device.hpp"

CDeviceFileJob::CDeviceFileJob(const char* pName, CWorkThread* pParent):
CWorkThread(pName, pParent, 0),
mHandle(nullptr),
unk1C8(0),
unk208(0),
unk20C(0),
unk210(0){
    mType = THREAD_CDEVICEFILEJOB;
}

// CDeviceFileJob::cancel(CDeviceFileJob_UnkStruct1*) - returns false


// extern "C" free-function form (CFontLayer pattern): no non-inline virtual
// member is defined in this TU, so MWCC does not auto-emit __vt__14CDeviceFileJob
// here (the retail vtable lives in the dissolved .data blob below).
extern "C" bool cancel__14CDeviceFileJobFPCc(CDeviceFileJob* self, const char* pFilename){
    return false;
}
extern "C" int CDeviceFileJob_UnkVirtualFunc1__14CDeviceFileJobFv() { return 0; }

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
namespace FJBlob {
extern "C" void __dt__14CDeviceFileJobFv();
extern "C" void WorkEvent1__10IWorkEventFPvPCc();
extern "C" void OnFileEvent__10IWorkEventFP10CEventFile();
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
extern "C" void wkUpdate__11CWorkThreadFv();
extern "C" void wkRender__11CWorkThreadFv();
extern "C" void wkRenderAfter__11CWorkThreadFv();
extern "C" void wkStandbyLogin__11CWorkThreadFv();
extern "C" void wkStandbyLogout__11CWorkThreadFv();
extern "C" void wkStandbyExceptionRetry__11CWorkThreadFUl();
extern "C" void CDeviceFileJob_UnkVirtualFunc1__14CDeviceFileJobFv();
extern "C" void cancel__14CDeviceFileJobFPCc();
extern "C" void cancel__14CDeviceFileJobFP25CDeviceFileJob_UnkStruct1();
extern "C" u32 __RTTI__10IWorkEvent;
extern "C" u32 __RTTI__11CWorkThread;
}
extern "C" u32 lbl_eu_806636E8[2]; // this unit's sdata
extern "C" u32 lbl_eu_8056C584[5]; // .data second symbol (sdata reloc target)

// [.data] 0x8056C4D8-0x8056C598 (192B): CDeviceFileJob vtable
extern "C" u32 lbl_eu_8056C4D8[43] = {
    (u32)&lbl_eu_806636E8, 0x00000000,
    (u32)&FJBlob::__dt__14CDeviceFileJobFv,
    (u32)&FJBlob::WorkEvent1__10IWorkEventFPvPCc,
    (u32)&FJBlob::OnFileEvent__10IWorkEventFP10CEventFile,
    (u32)&FJBlob::WorkEvent3__10IWorkEventFPv,
    (u32)&FJBlob::WorkEvent4__10IWorkEventFv,
    (u32)&FJBlob::OnPauseTrigger__10IWorkEventFb,
    (u32)&FJBlob::WorkEvent6__10IWorkEventFv,
    (u32)&FJBlob::WorkEvent7__10IWorkEventFv,
    (u32)&FJBlob::WorkEvent8__10IWorkEventFv,
    (u32)&FJBlob::WorkEvent9__10IWorkEventFv,
    (u32)&FJBlob::WorkEvent10__10IWorkEventFv,
    (u32)&FJBlob::WorkEvent11__10IWorkEventFv,
    (u32)&FJBlob::WorkEvent12__10IWorkEventFv,
    (u32)&FJBlob::WorkEvent13__10IWorkEventFv,
    (u32)&FJBlob::WorkEvent14__10IWorkEventFv,
    (u32)&FJBlob::WorkEvent15__10IWorkEventFv,
    (u32)&FJBlob::WorkEvent16__10IWorkEventFv,
    (u32)&FJBlob::WorkEvent17__10IWorkEventFv,
    (u32)&FJBlob::WorkEvent18__10IWorkEventFv,
    (u32)&FJBlob::WorkEvent19__10IWorkEventFv,
    (u32)&FJBlob::WorkEvent20__10IWorkEventFv,
    (u32)&FJBlob::WorkEvent21__10IWorkEventFv,
    (u32)&FJBlob::WorkEvent22__10IWorkEventFv,
    (u32)&FJBlob::WorkEvent23__10IWorkEventFv,
    (u32)&FJBlob::WorkEvent24__10IWorkEventFv,
    (u32)&FJBlob::WorkEvent25__10IWorkEventFv,
    (u32)&FJBlob::WorkEvent26__10IWorkEventFv,
    (u32)&FJBlob::WorkEvent27__10IWorkEventFv,
    (u32)&FJBlob::WorkEvent28__10IWorkEventFv,
    (u32)&FJBlob::WorkEvent29__10IWorkEventFv,
    (u32)&FJBlob::WorkEvent30__10IWorkEventFv,
    (u32)&FJBlob::WorkEvent31__10IWorkEventFv,
    (u32)&FJBlob::wkUpdate__11CWorkThreadFv,
    (u32)&FJBlob::wkRender__11CWorkThreadFv,
    (u32)&FJBlob::wkRenderAfter__11CWorkThreadFv,
    (u32)&FJBlob::wkStandbyLogin__11CWorkThreadFv,
    (u32)&FJBlob::wkStandbyLogout__11CWorkThreadFv,
    (u32)&FJBlob::wkStandbyExceptionRetry__11CWorkThreadFUl,
    (u32)&FJBlob::CDeviceFileJob_UnkVirtualFunc1__14CDeviceFileJobFv,
    (u32)&FJBlob::cancel__14CDeviceFileJobFPCc,
    (u32)&FJBlob::cancel__14CDeviceFileJobFP25CDeviceFileJob_UnkStruct1,
};
extern "C" u32 lbl_eu_8056C584[5] = {
    (u32)&FJBlob::__RTTI__10IWorkEvent, 0x00000000,
    (u32)&FJBlob::__RTTI__11CWorkThread, 0x00000000, 0x00000000,
};

// [.rodata] 0x80522D40-0x80522D50 (16B)
extern "C" __declspec(align(4)) const char lbl_eu_80522D40[0x10] = {
    0x43,0x44,0x65,0x76,0x69,0x63,0x65,0x46,0x69,0x6C,0x65,0x4A,0x6F,0x62,0x00,0x00,
};

// [.sdata] 0x806636E8-0x806636F0 (8B)
extern "C" u32 lbl_eu_806636E8[2] = { (u32)&lbl_eu_80522D40, (u32)&lbl_eu_8056C584 };
