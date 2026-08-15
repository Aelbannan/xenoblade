#include "monolib/device.hpp"
#include "monolib/core.hpp"
#include "monolib/lib.hpp"

// ============================================================================
// Blob monolibdata1d/monolibdata1 dissolve - CDeviceRemotePad-owned data
// ranges. Retail bytes/reloc names below are the DOL + retail blob ground
// truth. The RTTI base-list .sdata objects, the RTTI name .rodata strings,
// the class vtables and the sbss singleton are defined here with the retail
// names. NOTE: this TU needs effective `-RTTI off` (see report) - with RTTI
// on, MWCC auto-generates __RTTI__* symbols that clash with the extern "C"
// declarations of the retail RTTI objects.
//
// The vtable/RTTI function addresses are referenced through C-linkage extern
// declarations of the retail (mangled) names; they coexist with the real
// class member declarations (probe-verified) as long as the auto-generated
// RTTI symbols stay off.
// ============================================================================

// RTTI data labels (.sdata 0x806635E8-0x806635F8, defined below) and the
// base-class RTTI objects (defined in the TUs owning their key functions).
extern "C" void* __RTTI__10IWorkEvent;
extern "C" void* __RTTI__11CWorkThread;

// .data RTTI base lists (defined below; forward decls for the .sdata refs).
extern "C" u32 lbl_eu_8056BC60[8];
extern "C" u32 lbl_eu_8056BD20[6];

// Destructors (retail C-linkage names; the class members coexist).
extern "C" void __dt__16CDeviceRemotePadFv();
extern "C" void __dt__11CDeviceBaseFv();

// IWorkEvent virtual handlers (weak defaults live in kyoshin/CGame.cpp).
extern "C" int WorkEvent1__10IWorkEventFPvPCc(void*, const char*);
extern "C" int OnFileEvent__10IWorkEventFP10CEventFile(void*);
extern "C" int WorkEvent3__10IWorkEventFPv(void*);
extern "C" int WorkEvent4__10IWorkEventFv();
extern "C" void OnPauseTrigger__10IWorkEventFb(int);
extern "C" int WorkEvent6__10IWorkEventFv();
extern "C" int WorkEvent7__10IWorkEventFv();
extern "C" int WorkEvent8__10IWorkEventFv();
extern "C" int WorkEvent9__10IWorkEventFv();
extern "C" int WorkEvent10__10IWorkEventFv();
extern "C" int WorkEvent11__10IWorkEventFv();
extern "C" int WorkEvent12__10IWorkEventFv();
extern "C" int WorkEvent13__10IWorkEventFv();
extern "C" int WorkEvent14__10IWorkEventFv();
extern "C" int WorkEvent15__10IWorkEventFv();
extern "C" int WorkEvent16__10IWorkEventFv();
extern "C" int WorkEvent17__10IWorkEventFv();
extern "C" int WorkEvent18__10IWorkEventFv();
extern "C" int WorkEvent19__10IWorkEventFv();
extern "C" int WorkEvent20__10IWorkEventFv();
extern "C" int WorkEvent21__10IWorkEventFv();
extern "C" int WorkEvent22__10IWorkEventFv();
extern "C" int WorkEvent23__10IWorkEventFv();
extern "C" int WorkEvent24__10IWorkEventFv();
extern "C" int WorkEvent25__10IWorkEventFv();
extern "C" int WorkEvent26__10IWorkEventFv();
extern "C" int WorkEvent27__10IWorkEventFv();
extern "C" int WorkEvent28__10IWorkEventFv();
extern "C" int WorkEvent29__10IWorkEventFv();
extern "C" int WorkEvent30__10IWorkEventFv();
extern "C" void WorkEvent31__10IWorkEventFv();

// CWorkThread work-method vtable slots.
extern "C" void wkUpdate__11CWorkThreadFv();
extern "C" void wkRender__11CWorkThreadFv();
extern "C" void wkRenderAfter__11CWorkThreadFv();
extern "C" void wkStandbyLogin__11CWorkThreadFv();
extern "C" void wkStandbyLogout__11CWorkThreadFv();
extern "C" void wkStandbyExceptionRetry__11CWorkThreadFUl(unsigned int);

// CDeviceRemotePad work-method overrides (defined below).
extern "C" void wkUpdate__16CDeviceRemotePadFv();
extern "C" void wkStandbyLogin__16CDeviceRemotePadFv();
extern "C" void wkStandbyLogout__16CDeviceRemotePadFv();

// .rodata 0x80522960-0x80522980: RTTI name strings.
extern "C" const char lbl_eu_80522960[0x11] = "CDeviceRemotePad";
extern "C" const char lbl_eu_80522974[0xC] = "CDeviceBase";

// .sdata 0x806635E8-0x806635F8: RTTI base lists {&name, &baseList}.
extern "C" u32 lbl_eu_806635E8[2] = { (u32)&lbl_eu_80522960[0], (u32)&lbl_eu_8056BC60[0] };
extern "C" u32 lbl_eu_806635F0[2] = { (u32)&lbl_eu_80522974[0], (u32)&lbl_eu_8056BD20[0] };

// .data 0x8056BBC0: CDeviceRemotePad vtable (0xA0).
extern "C" u32 lbl_eu_8056BBC0[0xA0 / 4] = {
    (u32)&lbl_eu_806635E8[0], 0, (u32)&__dt__16CDeviceRemotePadFv,
    (u32)&WorkEvent1__10IWorkEventFPvPCc, (u32)&OnFileEvent__10IWorkEventFP10CEventFile,
    (u32)&WorkEvent3__10IWorkEventFPv, (u32)&WorkEvent4__10IWorkEventFv,
    (u32)&OnPauseTrigger__10IWorkEventFb,
    (u32)&WorkEvent6__10IWorkEventFv, (u32)&WorkEvent7__10IWorkEventFv,
    (u32)&WorkEvent8__10IWorkEventFv, (u32)&WorkEvent9__10IWorkEventFv,
    (u32)&WorkEvent10__10IWorkEventFv, (u32)&WorkEvent11__10IWorkEventFv,
    (u32)&WorkEvent12__10IWorkEventFv, (u32)&WorkEvent13__10IWorkEventFv,
    (u32)&WorkEvent14__10IWorkEventFv, (u32)&WorkEvent15__10IWorkEventFv,
    (u32)&WorkEvent16__10IWorkEventFv, (u32)&WorkEvent17__10IWorkEventFv,
    (u32)&WorkEvent18__10IWorkEventFv, (u32)&WorkEvent19__10IWorkEventFv,
    (u32)&WorkEvent20__10IWorkEventFv, (u32)&WorkEvent21__10IWorkEventFv,
    (u32)&WorkEvent22__10IWorkEventFv, (u32)&WorkEvent23__10IWorkEventFv,
    (u32)&WorkEvent24__10IWorkEventFv, (u32)&WorkEvent25__10IWorkEventFv,
    (u32)&WorkEvent26__10IWorkEventFv, (u32)&WorkEvent27__10IWorkEventFv,
    (u32)&WorkEvent28__10IWorkEventFv, (u32)&WorkEvent29__10IWorkEventFv,
    (u32)&WorkEvent30__10IWorkEventFv, (u32)&WorkEvent31__10IWorkEventFv,
    (u32)&wkUpdate__16CDeviceRemotePadFv, (u32)&wkRender__11CWorkThreadFv,
    (u32)&wkRenderAfter__11CWorkThreadFv, (u32)&wkStandbyLogin__16CDeviceRemotePadFv,
    (u32)&wkStandbyLogout__16CDeviceRemotePadFv, (u32)&wkStandbyExceptionRetry__11CWorkThreadFUl,
};

// .data 0x8056BC60: CDeviceRemotePad RTTI base list (0x20).
extern "C" u32 lbl_eu_8056BC60[] = {
    (u32)&__RTTI__10IWorkEvent, 0, (u32)&__RTTI__11CWorkThread, 0,
    (u32)&lbl_eu_806635F0[0], 0, 0, 0,
};

// .data 0x8056BC80: CDeviceBase vtable (0xA0) - base-class slots are the
// CWorkThread implementations.
extern "C" u32 __vt__11CDeviceBase[0xA0 / 4] = {
    (u32)&lbl_eu_806635F0[0], 0, (u32)&__dt__11CDeviceBaseFv,
    (u32)&WorkEvent1__10IWorkEventFPvPCc, (u32)&OnFileEvent__10IWorkEventFP10CEventFile,
    (u32)&WorkEvent3__10IWorkEventFPv, (u32)&WorkEvent4__10IWorkEventFv,
    (u32)&OnPauseTrigger__10IWorkEventFb,
    (u32)&WorkEvent6__10IWorkEventFv, (u32)&WorkEvent7__10IWorkEventFv,
    (u32)&WorkEvent8__10IWorkEventFv, (u32)&WorkEvent9__10IWorkEventFv,
    (u32)&WorkEvent10__10IWorkEventFv, (u32)&WorkEvent11__10IWorkEventFv,
    (u32)&WorkEvent12__10IWorkEventFv, (u32)&WorkEvent13__10IWorkEventFv,
    (u32)&WorkEvent14__10IWorkEventFv, (u32)&WorkEvent15__10IWorkEventFv,
    (u32)&WorkEvent16__10IWorkEventFv, (u32)&WorkEvent17__10IWorkEventFv,
    (u32)&WorkEvent18__10IWorkEventFv, (u32)&WorkEvent19__10IWorkEventFv,
    (u32)&WorkEvent20__10IWorkEventFv, (u32)&WorkEvent21__10IWorkEventFv,
    (u32)&WorkEvent22__10IWorkEventFv, (u32)&WorkEvent23__10IWorkEventFv,
    (u32)&WorkEvent24__10IWorkEventFv, (u32)&WorkEvent25__10IWorkEventFv,
    (u32)&WorkEvent26__10IWorkEventFv, (u32)&WorkEvent27__10IWorkEventFv,
    (u32)&WorkEvent28__10IWorkEventFv, (u32)&WorkEvent29__10IWorkEventFv,
    (u32)&WorkEvent30__10IWorkEventFv, (u32)&WorkEvent31__10IWorkEventFv,
    (u32)&wkUpdate__11CWorkThreadFv, (u32)&wkRender__11CWorkThreadFv,
    (u32)&wkRenderAfter__11CWorkThreadFv, (u32)&wkStandbyLogin__11CWorkThreadFv,
    (u32)&wkStandbyLogout__11CWorkThreadFv, (u32)&wkStandbyExceptionRetry__11CWorkThreadFUl,
};

// .data 0x8056BD20: CDeviceBase RTTI base list (0x18).
extern "C" u32 lbl_eu_8056BD20[] = {
    (u32)&__RTTI__10IWorkEvent, 0, (u32)&__RTTI__11CWorkThread, 0, 0, 0,
};

// .sbss 0x80665638: CDeviceRemotePad singleton (8 bytes; word 0 in use).
// (tentative definition with C linkage; header declares it extern "C")
extern "C" {
CDeviceRemotePad* lbl_eu_80665638[2];
}

// Own TU owns CDeviceBase's dtor: retail places it (0x8044A2AC) at the head
// of this unit's slice, right before CDeviceRemotePad's dtor. The header
// defines it inline, so the odr-use from __vt__11CDeviceBase above emits the
// weak __dt__11CDeviceBaseFv here with the retail body (base-dtor + delete).

CDeviceRemotePad::CDeviceRemotePad(const char* pName, CWorkThread* pParent) :
CDeviceBase(pName, pParent, MAX_CHILD){
    // Retail stores the manual vtable label (novtable class, no auto store).
    *(void**)this = (void*)&lbl_eu_8056BBC0[0];
    mPadUpdateFunc = nullptr;
    lbl_eu_80665638[0] = this;

    for(int i = 0; i < TOTAL_CONTROLLERS; i++){
        mpPads[i] = nullptr;
    }
}

CDeviceRemotePad::~CDeviceRemotePad(){
    lbl_eu_80665638[0] = nullptr;
}

CDeviceRemotePad* CDeviceRemotePad::getInstance(){
    return lbl_eu_80665638[0];
}

bool CDeviceRemotePad::isConnected(u32 index){
    return lbl_eu_80665638[0]->mpPads[index]->mConnected;
}

u32 CDeviceRemotePad::getHeldButtonFlags(u32 index){
    return lbl_eu_80665638[0]->mpPads[index]->mHeldButtonFlags;
}

u32 CDeviceRemotePad::getPressedButtonFlags(u32 index){
    return lbl_eu_80665638[0]->mpPads[index]->mPressedButtonFlags;
}

CPad* CDeviceRemotePad::getMainGCPad(){
    return CPadManager::getMainGCPad();
}

//0-3: Wii controllers, 4-7: GC controllers
CPad* CDeviceRemotePad::getPadData(u32 index){
    return CPadManager::getPadData(index >= WPAD_MAX_CONTROLLERS ? PAD_SYSTEM_GC : PAD_SYSTEM_WII,
    index % WPAD_MAX_CONTROLLERS);
}

CWpadStatus* CDeviceRemotePad::getWpadStatus(u32 index){
    return CPadManager::getWpadStatus(index);
}

void CDeviceRemotePad::wkUpdate(){
    mPadUpdateFunc();
}

bool CDeviceRemotePad::wkStandbyLogin(){
    if(CDevice::isColdStartReady()){
        PadUpdateFunc func = CPadManager::initialize(mtl::MemManager::getHandleMEM2());
        mPadUpdateFunc = func;

        for(u32 i = 0; i < TOTAL_CONTROLLERS; i++){
            mpPads[i] = getPadData(i);
        }

        return CWorkThread::wkStandbyLogin();
    }

    return false;
}

bool CDeviceRemotePad::wkStandbyLogout(){
    if(mChildren.empty() && CWorkSystem::getInstance() == nullptr && CLib::getInstance() == nullptr){
        CPadManager::destroy();
        mPadUpdateFunc = nullptr;
        return CWorkThread::wkStandbyLogout();
    }

    return false;
}
