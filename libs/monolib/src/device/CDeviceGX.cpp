#include "monolib/device.hpp"
#include "monolib/lib.hpp"
#include "monolib/math.hpp"
#include "monolib/work.hpp"
#include "monolib/core.hpp"
#include "monolib/util.hpp"

using namespace ml;

#include <decomp.h>
#include <revolution/GX.h>
// Dissolved minimal
extern "C" {
    extern const char lbl_eu_8066A428[];
    extern u32 lbl_eu_806635F0[];
    extern u32 lbl_eu_80663618[];
    extern u32 __RTTI__10IWorkEvent[];
    extern u32 __RTTI__11CWorkThread[];
    extern void __dt__9CDeviceGXFv();
    extern void WorkEvent1__10IWorkEventFPvPCc();
    extern void OnFileEvent__10IWorkEventFP10CEventFile();
    extern void WorkEvent3__10IWorkEventFPv();
    extern void WorkEvent4__10IWorkEventFv();
    extern void OnPauseTrigger__10IWorkEventFb();
    extern void WorkEvent6__10IWorkEventFv();
    extern void WorkEvent7__10IWorkEventFv();
    extern void WorkEvent8__10IWorkEventFv();
    extern void WorkEvent9__10IWorkEventFv();
    extern void WorkEvent10__10IWorkEventFv();
    extern void WorkEvent11__10IWorkEventFv();
    extern void WorkEvent12__10IWorkEventFv();
    extern void WorkEvent13__10IWorkEventFv();
    extern void WorkEvent14__10IWorkEventFv();
    extern void WorkEvent15__10IWorkEventFv();
    extern void WorkEvent16__10IWorkEventFv();
    extern void WorkEvent17__10IWorkEventFv();
    extern void WorkEvent18__10IWorkEventFv();
    extern void WorkEvent19__10IWorkEventFv();
    extern void WorkEvent20__10IWorkEventFv();
    extern void WorkEvent21__10IWorkEventFv();
    extern void WorkEvent22__10IWorkEventFv();
    extern void WorkEvent23__10IWorkEventFv();
    extern void WorkEvent24__10IWorkEventFv();
    extern void WorkEvent25__10IWorkEventFv();
    extern void WorkEvent26__10IWorkEventFv();
    extern void WorkEvent27__10IWorkEventFv();
    extern void WorkEvent28__10IWorkEventFv();
    extern void WorkEvent29__10IWorkEventFv();
    extern void WorkEvent30__10IWorkEventFv();
    extern void WorkEvent31__10IWorkEventFv();
    extern void wkUpdate__11CWorkThreadFv();
    extern void wkRender__11CWorkThreadFv();
    extern void wkRenderAfter__11CWorkThreadFv();
    extern void wkStandbyLogin__9CDeviceGXFv();
    extern void wkStandbyLogout__9CDeviceGXFv();
    extern void wkStandbyExceptionRetry__11CWorkThreadFUl();
    extern void viBeforeDrawDone__11CDeviceVICbFv();
    extern void viAfterDrawDone__9CDeviceGXFv();
    extern void viBeginFrame__9CDeviceGXFv();
    extern void thunk_456_dt_9CDeviceGX();
    extern void thunk_456_viAfter_9CDeviceGX();
    extern void thunk_456_viBegin_9CDeviceGX();
    extern u32 lbl_eu_80522ED0[];
    extern u32 lbl_eu_8056CA20[];
    extern u32 lbl_eu_80663758[];
}
extern "C" __declspec(section ".rodata") u32 lbl_eu_80522ED0[4] = { 0x43446576, 0x69636547, 0x58000000, 0x00000000 };
extern "C" __declspec(section ".sdata") int gxHeapSize__9CDeviceGX = 0x00200000;
extern "C" __declspec(section ".sdata") const char* someString__9CDeviceGX = lbl_eu_8066A428;
extern "C" __declspec(section ".sdata") u32 lbl_eu_80663758[2] = { (u32)&lbl_eu_80522ED0, (u32)&lbl_eu_8056CA20 };
extern "C" u32 lbl_eu_8056C960[48] = {
    (u32)&lbl_eu_80663758, 0, (u32)&__dt__9CDeviceGXFv,
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
    (u32)&wkRenderAfter__11CWorkThreadFv, (u32)&wkStandbyLogin__9CDeviceGXFv,
    (u32)&wkStandbyLogout__9CDeviceGXFv, (u32)&wkStandbyExceptionRetry__11CWorkThreadFUl,
    (u32)&lbl_eu_80663758, 0xFFFFFE38, (u32)&thunk_456_dt_9CDeviceGX,
    (u32)&viBeforeDrawDone__11CDeviceVICbFv, (u32)&thunk_456_viAfter_9CDeviceGX,
    (u32)&thunk_456_viBegin_9CDeviceGX, (u32)&viAfterDrawDone__9CDeviceGXFv,
    (u32)&viBeginFrame__9CDeviceGXFv
};
extern "C" u32 lbl_eu_8056CA20[10] = {
    (u32)&lbl_eu_80663618, 0x000001C8, (u32)&__RTTI__10IWorkEvent, 0,
    (u32)&__RTTI__11CWorkThread, 0, (u32)&lbl_eu_806635F0, 0,
    0, 0
};
extern "C" CGXCache* cacheInstance__9CDeviceGX = nullptr;
extern "C" u32 lbl_eu_806656A8 = 0;
extern "C" CDesktop* lbl_eu_806656AC = nullptr;
extern "C" u8 lbl_eu_806656B0 = 0;
extern "C" u8 gap_10_806656B1_sbss[3];
DECOMP_FORCEACTIVE(CDeviceGX_cpp, lbl_eu_80522ED0, lbl_eu_8056C960, lbl_eu_8056CA20, lbl_eu_80663758, gxHeapSize__9CDeviceGX, someString__9CDeviceGX, cacheInstance__9CDeviceGX);
asm void thunk_456_dt_9CDeviceGX(void) {
    nofralloc
    subi r3, r3, 0x1C8
    b __dt__9CDeviceGXFv
}
asm void thunk_456_viBegin_9CDeviceGX(void) {
    nofralloc
    subi r3, r3, 0x1C8
    b viBeginFrame__9CDeviceGXFv
}
asm void thunk_456_viAfter_9CDeviceGX(void) {
    nofralloc
    subi r3, r3, 0x1C8
    b viAfterDrawDone__9CDeviceGXFv
}


// Inline copy of CWorkThread::isRunning() visible only in this TU so the retail
// inline shape (member call, this-arg bound to the instance) reproduces in
// isInitialized. CWorkRoot.cpp keeps the strong out-of-line definition.
inline bool CWorkThread::isRunning() const {
    bool exception;
    if(mFlags & THREAD_FLAG_EXCEPTION){
        exception = true;
    }else{
        exception = mMsgQueue.find(EVT_EXCEPTION) >= 0;
    }

    bool result = false;
    if(!exception){
        bool stateOK = mState == THREAD_STATE_LOGIN || mState == THREAD_STATE_RUN;
        if(stateOK){
            result = true;
        }
    }
    return result;
}

extern "C" {
extern CDeviceGX* lbl_eu_806656A0;
extern const f64 lbl_eu_8066A440;  // int→double magic (unsigned)
extern const f64 lbl_eu_8066A448;  // int→double magic (signed)
extern const f32 lbl_eu_8066A438;
extern f32 lbl_eu_80665698;
}

float CDeviceGX::sCostTime;
GXPixelFmt CDeviceGX::pixelFormat;
CDeviceGX* CDeviceGX::spInstance;
// cacheInstance/gxHeapSize/someString are provided by the retail data object
// (monolibdata2) until this unit's .sdata/.sbss is data-matched.

CDeviceGX::CDeviceGX(const char* pName, CWorkThread* pParent) :
CDeviceBase(pName, pParent, 0),
CDeviceVICb(),
mDevicesInitialized(false),
mGxHeap(nullptr),
mGxHeapEndAddr(nullptr),
unk264(0),
unk26C(0),
unk270(0),
unk274(1),
mFilter(VFILTER_NONE){
    spInstance = this;
    cacheInstance = &unk27C;
    mGxHeap = new (CDevice::getDevSys1Handle(), 32) u8[gxHeapSize];
    mGxHeapEndAddr = mGxHeap + gxHeapSize;
    cacheInstance->unk50C = 0;
    updateVerticalFilter(VFILTER_NONE);
    cacheInstance->func_8044B294(0);
    setUnk260(1);
}

CDeviceGX::~CDeviceGX(){
    DELETE_ARRAY(mGxHeap);

    spInstance = nullptr;
}

CDeviceGX* CDeviceGX::getInstance(){
    // Retail SDA reloc is lbl_eu_806656A0@sda21, not spInstance__9CDeviceGX.
    return lbl_eu_806656A0;
}

bool CDeviceGX::isInitialized(){
    // Retail inlines CWorkThread::isRunning() here (no bl): the device counts as
    // initialized when it has no pending exception and its state is LOGIN or RUN.
    return lbl_eu_806656A0->isRunning();
}

void CDeviceGX::setDevicesInitializedFlag(bool state){
    spInstance->mDevicesInitialized = state;
}

bool CDeviceGX::devicesInitialized(){
    return spInstance->mDevicesInitialized == true;
}

void CDeviceGX::updateVerticalFilter(EVerticalFilter filter){
    //Default to filter 0
    spInstance->mFilter = filter;
    
    //The game only ever calls this function with VFILTER_0, so these other filters go unused.
    if(spInstance->mFilter == VFILTER_1){
        spInstance->mVFilter[0] = 0;
        spInstance->mVFilter[1] = 3;
        spInstance->mVFilter[2] = 19;
        spInstance->mVFilter[3] = 20;
        spInstance->mVFilter[4] = 19;
        spInstance->mVFilter[5] = 3;
        spInstance->mVFilter[7] = 0;
        spInstance->mVFilter[6] = 0;
    }else if(spInstance->mFilter == VFILTER_2){
        spInstance->mVFilter[0] = 4;
        spInstance->mVFilter[1] = 4;
        spInstance->mVFilter[2] = 15;
        spInstance->mVFilter[3] = 18;
        spInstance->mVFilter[4] = 15;
        spInstance->mVFilter[5] = 4;
        spInstance->mVFilter[6] = 4;
        spInstance->mVFilter[7] = 0;
    }else if(spInstance->mFilter == VFILTER_3){
        spInstance->mVFilter[0] = 8;
        spInstance->mVFilter[1] = 8;
        spInstance->mVFilter[2] = 10;
        spInstance->mVFilter[3] = 12;
        spInstance->mVFilter[4] = 10;
        spInstance->mVFilter[5] = 8;
        spInstance->mVFilter[6] = 8;
        spInstance->mVFilter[7] = 0;
    }
}

void CDeviceGX::viAfterDrawDone(){
    GXFifoObj fifoTemp;
    void* readPtr;
    void* writePtr;

    GXFlush();
    GXGetCPUFifo(&fifoTemp);
    GXGetFifoPtrs(&fifoTemp, &readPtr, &writePtr);
    
    u32 temp1 = unk26C;
    u32 temp = (u32)writePtr;
    u32 temp2;

    if(temp >= temp1){
        temp2 = temp - temp1;
    }else{
        temp1 -= temp;
        temp2 = gxHeapSize - temp1;
    }
    unk264 = ((f32)temp2 / (f32)gxHeapSize) * lbl_eu_8066A438;
}

void CDeviceGX::viBeginFrame(){
    if(lbl_eu_806656A0->mDevicesInitialized == 1){
        return;
    }
    cacheInstance->func_8044BE38();
}

void CDeviceGX::drawFrame(){
    if(lbl_eu_806656A0->mDevicesInitialized == 1){
        GXFlush();

        GXFifoObj fifoTemp;
        void* readPtr;
        void* writePtr;

        GXGetCPUFifo(&fifoTemp);
        GXGetFifoPtrs(&fifoTemp, &readPtr, &writePtr);
        lbl_eu_806656A0->unk26C = (u32)writePtr;
        lbl_eu_806656A0->unk270 = (u32)readPtr;
        GXEnableBreakPt(writePtr);

        GXSetDrawSync(token1);
        cacheInstance->func_8044BE38();

        if(lbl_eu_806656A0->unk274 == 0){
            CDrawGX something;
            something.setCol(*cacheInstance->func_8044B5B4());
            s16 efbHeight = CDeviceVI::getRenderModeObj()->efbHeight;
            s16 fbWidth = CDeviceVI::getRenderModeObj()->fbWidth;
            CRect16 rect = CRect16(0,0,fbWidth,efbHeight);
            something.renderRect(rect);
        }
    }else{
        CStopwatchUtil::entry(someString);
    }
}


//Copies the EFB to the destination external framebuffer.
void CDeviceGX::copyEfbToXfb(void* pDestFrameBuffer){
    CDeviceGX* gx = lbl_eu_806656A0;
    GXBool vf = gx->mFilter != VFILTER_NONE;
    GXRenderModeObj* rmode = CDeviceVI::getRenderModeObj();
    GXBool aa = CDeviceVI::getRenderModeObj()->aa;
    u8* vfilter = gx->mVFilter;
    GXSetCopyFilter(aa, rmode->sample_pattern, vf, vfilter);
    GXCopyDisp(pDestFrameBuffer, lbl_eu_806656A0->unk274);
}

void CDeviceGX::calculateCost(){
    CStopwatchUtil::updateCostTime(someString);
    f32 visPerFrame = (f32)CDeviceVI::getVisPerFrame();
    f32 costTime = CStopwatchUtil::getCostTime(someString);
    lbl_eu_80665698 = costTime / visPerFrame;
}


void CDeviceGX::copyEfb(void* pDestFrameBuffer){
    if(lbl_eu_806656A0->mDevicesInitialized == true){
        GXSetDrawSync(token2);
        copyEfbToXfb(pDestFrameBuffer);
        while(GXReadDrawSync() != token2){}
    }else{
        copyEfbToXfb(pDestFrameBuffer);
        GXDrawDone();
        calculateCost();
    }
}

/* Stubbed for release. This gets called every frame before view rendering happens, so it might've handled
drawing debug overlay stuff. */
void CDeviceGX::onRenderWork(){
}

int CDeviceGX::getHeapSize(){
    return gxHeapSize;
}

bool CDeviceGX::wkStandbyLogin(){
    if(CDeviceVI::func_804482DC()){
        GXInit(mGxHeap, gxHeapSize);

        if(spInstance->mDevicesInitialized == true){
            GXSetDrawDone();
            GXInitFifoBase(&mFifo, mGxHeap, gxHeapSize);
            GXSetCPUFifo(&mFifo);
            GXSetGPFifo(&mFifo);
            GXSetDrawSync(token2);
        }

        cacheInstance->unk50C = 0;
        updateVerticalFilter(VFILTER_NONE);

        GXRenderModeObj* renderMode = CDeviceVI::getRenderModeObj();

        if(renderMode->aa != 0){
            GXSetPixelFmt(GX_PF_RGBA565_Z16, GX_ZC_LINEAR);
        }else{
            GXSetPixelFmt(pixelFormat, GX_ZC_LINEAR);
        }

        cacheInstance->func_8044BE38();
        GXSetDither(GX_DISABLE);

        if(spInstance->mDevicesInitialized == true){
            GXSetDrawSyncCallback(drawSyncCallback);
        }

        return CWorkThread::wkStandbyLogin();
    }

    return false;
}

bool CDeviceGX::wkStandbyLogout(){
    if(spInstance->mDevicesInitialized == true){
        GXSetDrawSyncCallback(nullptr);
    }

    if(mChildren.empty() && CDevice::isInitialized() && CWorkSystem::getInstance() == nullptr
    && CLib::getInstance() == nullptr){
        return CWorkThread::wkStandbyLogout();
    }

    return false;
}

void CDeviceGX::drawSyncCallback(u16 token){
    if(token == token1){
        CStopwatchUtil::entry(someString);
    }else if(token == token2){
        calculateCost();
    }
}

void CDeviceGX::setValues(GXPixelFmt format, u32 heapSize){
    pixelFormat = format;
    gxHeapSize = heapSize;
}

// dissolved monolibdata2 - device/CDeviceGX data now provided via retail copy (additive edit)
