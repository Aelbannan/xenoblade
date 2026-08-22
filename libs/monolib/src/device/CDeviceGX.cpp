#include "monolib/device.hpp"
#include "monolib/lib.hpp"
#include "monolib/math.hpp"
#include "monolib/work.hpp"
#include "monolib/core.hpp"
#include "monolib/util.hpp"

using namespace ml;

#include <decomp.h>
#include <revolution/GX.h>
class CDesktop;
extern "C" {
    extern const char lbl_eu_8066A428[];
    extern u32 lbl_eu_806635F0[];
    extern u32 lbl_eu_80663618[];
    extern void __dt__9CDeviceGXFv();
    extern void thunk_456_dt_9CDeviceGX();
    extern void thunk_456_viAfter_9CDeviceGX();
    extern void thunk_456_viBegin_9CDeviceGX();
}
extern "C" __declspec(section ".rodata") u32 lbl_eu_80522ED0[4] = { 0x43446576, 0x69636547, 0x58000000, 0x00000000 };
extern "C" __declspec(section ".sdata") int gxHeapSize__9CDeviceGX = 0x00200000;
extern "C" __declspec(section ".sdata") const char* someString__9CDeviceGX = lbl_eu_8066A428;
extern "C" u32 lbl_eu_8056CA20[];
extern "C" __declspec(section ".sdata") u32 lbl_eu_80663758[2] = { (u32)&lbl_eu_80522ED0, (u32)&lbl_eu_8056CA20 };
extern "C" u32 lbl_eu_8056C960[48] = {0};
extern "C" u32 lbl_eu_8056CA20[10] = {0};
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

extern "C" f32 lbl_eu_80665698 = 0;
extern "C" GXPixelFmt lbl_eu_8066569C = (GXPixelFmt)0;
extern "C" CDeviceGX* lbl_eu_806656A0 = nullptr;
#define sCostTime lbl_eu_80665698
#define pixelFormat lbl_eu_8066569C
#define spInstance lbl_eu_806656A0
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
    mGxHeap = new (CDevice::getDevSys1Handle(), 32) u8[gxHeapSize__9CDeviceGX];
    mGxHeapEndAddr = mGxHeap + gxHeapSize__9CDeviceGX;
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

struct MyMsgEntry {
    u32 command;
    u32 wid;
    u32 unk8;
    u32 unkC;
    u32 unk10;
    u32 unk14;
    u32 unk18;
    u32 unk1C;
    u16 unk20;
    u8 unk22;
    u8 unk23;
};
struct MyQueueData {
    u8 pad[0x1A4];
    MyMsgEntry* mArrayPtr;
    u32 mFront;
    u32 mSize;
    u32 mCapacity;
};
bool CDeviceGX::isInitialized(){
    CDeviceGX* inst = lbl_eu_806656A0;
    bool exception;
    u32 flags = *(u32*)((u8*)inst + 0x7C);
    if (flags & 0x10) {
        exception = true;
    } else {
        MyQueueData* q = (MyQueueData*)inst;
        int found = -1;
        for (u32 i = 0; i < q->mSize; i++) {
            u32 idx = (q->mFront + i) % q->mCapacity;
            MyMsgEntry* e = &q->mArrayPtr[idx];
            if (e->command == 2) {
                found = (int)i;
                break;
            }
        }
        exception = found >= 0;
    }
    bool result = false;
    if (!exception) {
        u32 state = *(u32*)((u8*)inst + 0x48);
        if (state == 2 || state == 3) {
            result = true;
        }
    }
    return result;
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
        spInstance->mVFilter[6] = 0;
        spInstance->mVFilter[7] = 0;
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
    sCostTime = costTime / visPerFrame;
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
    return gxHeapSize__9CDeviceGX;
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
    gxHeapSize__9CDeviceGX = heapSize;
}

// dissolved monolibdata2 - device/CDeviceGX data now provided via retail copy (additive edit)
