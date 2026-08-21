#include "monolib/device.hpp"
#include "monolib/lib.hpp"
#include "monolib/math.hpp"
#include "monolib/work.hpp"
#include "monolib/core.hpp"
#include "monolib/util.hpp"

using namespace ml;

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
