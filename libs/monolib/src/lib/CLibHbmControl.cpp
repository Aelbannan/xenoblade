#include "monolib/lib/CLibHbmControl.hpp"
#include "monolib/lib/CLibHbm.hpp"
#include "monolib/device.hpp"
#include "monolib/core.hpp"
#include "monolib/work.hpp"

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
#define spInstance lbl_eu_80665708
extern "C" CLibHbmControl* lbl_eu_80665708;
extern const float lbl_eu_8066A578;  // 0.0f (retail wkStandbyLogin zero)
extern void* __dt__5CProcFv(void* self, int flag);
extern bool wkStandbyLogin__5CProcFv(void* self);
extern bool wkStandbyLogout__5CProcFv(void* self);
extern void callExitFunc__11CWorkSystemFv();
extern void __dl__FPv(void*);
namespace HBMBlob {
extern "C" void* __dt__14CLibHbmControlFv();
extern "C" void wkUpdate__14CLibHbmControlFv();
extern "C" void wkRender__14CLibHbmControlFv();
extern "C" bool wkStandbyLogin__14CLibHbmControlFv();
extern "C" bool wkStandbyLogout__14CLibHbmControlFv();
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
extern "C" void wkRenderAfter__11CWorkThreadFv();
extern "C" void wkStandbyExceptionRetry__11CWorkThreadFUl();
extern "C" u32 __RTTI__10IWorkEvent;
extern "C" u32 __RTTI__11CWorkThread;
extern "C" u32 __RTTI__5CProc;
}

// [.rodata] 0x80523190-0x805231B0 (0x20 = 32B): two copies of the class name.
extern "C" __declspec(align(8)) const char lbl_eu_80523190[0x20] = {
    0x43,0x4C,0x69,0x62,0x48,0x62,0x6D,0x43,0x6F,0x6E,0x74,0x72,0x6F,0x6C,0x00,0x00,
    0x43,0x4C,0x69,0x62,0x48,0x62,0x6D,0x43,0x6F,0x6E,0x74,0x72,0x6F,0x6C,0x00,0x00,
};
extern "C" u32 lbl_eu_806637D0[2];
extern "C" u32 lbl_eu_8056D290[40] = {
    (u32)&lbl_eu_806637D0, 0x00000000,
    (u32)&HBMBlob::__dt__14CLibHbmControlFv,
    (u32)&HBMBlob::WorkEvent1__10IWorkEventFPvPCc, (u32)&HBMBlob::OnFileEvent__10IWorkEventFP10CEventFile,
    (u32)&HBMBlob::WorkEvent3__10IWorkEventFPv, (u32)&HBMBlob::WorkEvent4__10IWorkEventFv,
    (u32)&HBMBlob::OnPauseTrigger__10IWorkEventFb,
    (u32)&HBMBlob::WorkEvent6__10IWorkEventFv, (u32)&HBMBlob::WorkEvent7__10IWorkEventFv,
    (u32)&HBMBlob::WorkEvent8__10IWorkEventFv, (u32)&HBMBlob::WorkEvent9__10IWorkEventFv,
    (u32)&HBMBlob::WorkEvent10__10IWorkEventFv, (u32)&HBMBlob::WorkEvent11__10IWorkEventFv,
    (u32)&HBMBlob::WorkEvent12__10IWorkEventFv, (u32)&HBMBlob::WorkEvent13__10IWorkEventFv,
    (u32)&HBMBlob::WorkEvent14__10IWorkEventFv, (u32)&HBMBlob::WorkEvent15__10IWorkEventFv,
    (u32)&HBMBlob::WorkEvent16__10IWorkEventFv, (u32)&HBMBlob::WorkEvent17__10IWorkEventFv,
    (u32)&HBMBlob::WorkEvent18__10IWorkEventFv, (u32)&HBMBlob::WorkEvent19__10IWorkEventFv,
    (u32)&HBMBlob::WorkEvent20__10IWorkEventFv, (u32)&HBMBlob::WorkEvent21__10IWorkEventFv,
    (u32)&HBMBlob::WorkEvent22__10IWorkEventFv, (u32)&HBMBlob::WorkEvent23__10IWorkEventFv,
    (u32)&HBMBlob::WorkEvent24__10IWorkEventFv, (u32)&HBMBlob::WorkEvent25__10IWorkEventFv,
    (u32)&HBMBlob::WorkEvent26__10IWorkEventFv, (u32)&HBMBlob::WorkEvent27__10IWorkEventFv,
    (u32)&HBMBlob::WorkEvent28__10IWorkEventFv, (u32)&HBMBlob::WorkEvent29__10IWorkEventFv,
    (u32)&HBMBlob::WorkEvent30__10IWorkEventFv, (u32)&HBMBlob::WorkEvent31__10IWorkEventFv,
    (u32)&HBMBlob::wkUpdate__14CLibHbmControlFv, (u32)&HBMBlob::wkRender__14CLibHbmControlFv,
    (u32)&HBMBlob::wkRenderAfter__11CWorkThreadFv,
    (u32)&HBMBlob::wkStandbyLogin__14CLibHbmControlFv, (u32)&HBMBlob::wkStandbyLogout__14CLibHbmControlFv,
    (u32)&HBMBlob::wkStandbyExceptionRetry__11CWorkThreadFUl,
};
extern "C" u32 lbl_eu_8056D330[8] = {
    (u32)&HBMBlob::__RTTI__10IWorkEvent, 0x00000000,
    (u32)&HBMBlob::__RTTI__11CWorkThread, 0x00000000,
    (u32)&HBMBlob::__RTTI__5CProc, 0x00000000,
    0x00000000, 0x00000000,
};
extern "C" u32 lbl_eu_806637D0[2] = { (u32)&lbl_eu_80523190, (u32)&lbl_eu_8056D330 };
DECOMP_FORCEACTIVE(CLibHbmControl_cpp, lbl_eu_8056D290);
DECOMP_FORCEACTIVE(CLibHbmControl_cpp, lbl_eu_8056D330);
DECOMP_FORCEACTIVE(CLibHbmControl_cpp, lbl_eu_80523190);
DECOMP_FORCEACTIVE(CLibHbmControl_cpp, lbl_eu_806637D0);

// Own-class virtuals as plain free functions (retail names).
CLibHbmControl::CLibHbmControl(const char* pName, CWorkThread* pParent) : CProc(pName, pParent, MAX_CHILD),
mHbmPhase(0),
mWaitTimer(0) {
    spInstance = this;
    mType = THREAD_CLIBHBMCONTROL;
    std::memset(&mHBMControllerData, 0, sizeof(HBMControllerData));
}

void* __dt__14CLibHbmControlFv(CLibHbmControl* self, int flag) {
    if (self != nullptr) {
        spInstance = nullptr;
        __dt__5CProcFv(self, 0);
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

CLibHbmControl* CLibHbmControl::getInstance(){
    return spInstance;
}

bool CLibHbmControl::isActive(){
    CLibHbmControl* hbmControl = spInstance;

    if(hbmControl == nullptr) return false;
    return hbmControl->mHbmPhase == 3;
}

// Advances the HBM (Home Button Menu) state machine each frame.
// Phase 0: load arc file   → 1
// Phase 1: wait for file I/O, countdown timer, then → 2 (or back to 0 on error)
// Phase 2: init HBM system  → 3
// Phase 3: active - polls WPAD controllers, processes HBM button selections
void wkUpdate__14CLibHbmControlFv(CLibHbmControl* self){
    switch(self->mHbmPhase){
        case 0:
            CLibHbm::loadHbmArcFile();
            self->mWaitTimer = 0xD2;
            self->mHbmPhase++;
            break;
        case 1:
            if(!CDeviceFileCri::getInstance()->isException()){
                self->mWaitTimer--;
                if(self->mWaitTimer <= 0){
                    self->wkSetEvent(CWorkThread::EVT_NONE);
                }

                break;
            }

            self->wkSetEvent(CWorkThread::EVT_NONE);

            if(CLibHbm::checkFlag6()){
                self->mHbmPhase = 0;
            }else if(CLibHbm::isHbmMemPointerValid()){
                self->mHbmPhase++;
            }
            break;
        case 2:
            CLibHbm::initHbm();
            self->mHbmPhase++;
            break;
        case 3:
            for(int i = 0; i < WPAD_MAX_CONTROLLERS; i++){
                CWpadStatus* wpadStatus = CDeviceRemotePad::getWpadStatus(i);

            }
            break;
        default:
            break;
    }

    HBMUpdateSound();

    HBMSelectBtnNum selectBtnNum = HBMCalc(&self->mHBMControllerData);

    switch(selectBtnNum){
        case HBM_SELECT_HOMEBTN:
            self->wkSetEvent(CWorkThread::EVT_NONE);
            break;
        case HBM_SELECT_BTN1:
            callExitFunc__11CWorkSystemFv();
            VIWaitForRetrace();
            VIWaitForRetrace();
            VIWaitForRetrace();
            VIWaitForRetrace();
            OSReturnToMenu();
            break;
        case HBM_SELECT_BTN2:
            callExitFunc__11CWorkSystemFv();
            VISetBlack(1);
            VIFlush();
            VIWaitForRetrace();
            VIWaitForRetrace();
            OSRestart(0);
            break;
        default:
            break;
    }
}

void wkRender__14CLibHbmControlFv(CLibHbmControl* self){
    
}



/* create disabled */

bool wkStandbyLogin__14CLibHbmControlFv(CLibHbmControl* self) {
    // Locals corresponding to callee-saved registers used in the context ring push.
    // Declared early so the compiler assigns them to r25-r31 and emits _savegpr_25.
    s32 saved_r25, saved_r26, saved_r27, saved_r28;
    s32 saved_r29, saved_r30, saved_r31;

    CView* view = CDesktop::getView();
    CView* subView = self->pssCreateView(self->mName.c_str(), view, 0);

    // Context ring push: compute entry position
    s32 sum = subView->unk3F0 + subView->mContextRingWriteIndex;
    s32 product = (sum / (s32)subView->mContextRingCapacity) * (s32)subView->mContextRingCapacity;
    s32 remainder = sum - product;
    s32 offset = remainder * (s32)sizeof(CViewContextRingEntry);

    // Write context ring entry with saved register state
    CViewContextRingEntry* entry = (CViewContextRingEntry*)((u8*)subView->mContextRingBase + offset);
    entry->tag = 4;
    ((u32*)entry->payload)[0] = saved_r25;
    ((u32*)entry->payload)[1] = saved_r26;
    ((u32*)entry->payload)[2] = saved_r27;
    ((u32*)entry->payload)[3] = saved_r28;
    ((u32*)entry->payload)[4] = saved_r29;
    ((u32*)entry->payload)[5] = saved_r30;
    ((u32*)entry->payload)[6] = saved_r31;
    entry->unk54 = 0;
    entry->unk56Hi = 0;
    entry->pad = 0;

    subView->mContextRingWriteIndex++;
    subView->unk3FC = subView->mContextRingWriteIndex - 1;
    subView->unk278 |= 8;
    subView->unk460 = 2;

    // Copy this->mName to subView's own mName (CView::mName at 0x400)
    subView->mName.mLength = strlen(self->mName.c_str());
    strcpy(subView->mName.mString, self->mName.c_str());

    // If the inherited CWorkThread::mName is empty, copy to it too
    if (subView->CWorkThread::mName.mLength == 0) {
        subView->CWorkThread::mName.mLength = strlen(self->mName.c_str());
        strcpy(subView->CWorkThread::mName.mString, self->mName.c_str());
    }

    // Set view rect to full screen
    GXRenderModeObj* rmode = CDeviceVI::getRenderModeObj();
    s16 fbWidth = rmode->fbWidth;
    s16 efbHeight = rmode->efbHeight;

    ml::CRect16 rect;
    rect.mPos.x = 0;
    rect.mPos.y = 0;
    rect.mSize.x = fbWidth;
    rect.mSize.y = efbHeight;
    subView->setRect(rect);

    // Set position to zero (four floats loaded from sdata)
    float zero = lbl_eu_8066A578;
    subView->unk444.x = zero;
    subView->unk444.y = zero;
    subView->unk444.z = zero;
    subView->unk444.w = zero;

    CWorkControl::pause(true);
    return wkStandbyLogin__5CProcFv(self);
}


bool wkStandbyLogout__14CLibHbmControlFv(CLibHbmControl* self) {
    if (self->mChildren.empty()) {
        CLibHbm::deleteHbm();
        CWorkControl::pause(false);
        return wkStandbyLogout__5CProcFv(self);
    }
    return false;
}
