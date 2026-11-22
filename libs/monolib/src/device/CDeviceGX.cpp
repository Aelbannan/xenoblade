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
    // IWorkEvent/CWorkThread weak vtable slots (normally via data_vtables.hpp,
    // which spells __RTTI__* and trips MWCC 10322 under -ipa file in this TU).
    extern int WorkEvent1__10IWorkEventFPvPCc(void*, const char*);
    extern int OnFileEvent__10IWorkEventFP10CEventFile(void*);
    extern int WorkEvent3__10IWorkEventFPv(void*);
    extern int WorkEvent4__10IWorkEventFv();
    extern void OnPauseTrigger__10IWorkEventFb(int);
    extern int WorkEvent6__10IWorkEventFv();
    extern int WorkEvent7__10IWorkEventFv();
    extern int WorkEvent8__10IWorkEventFv();
    extern int WorkEvent9__10IWorkEventFv();
    extern int WorkEvent10__10IWorkEventFv();
    extern int WorkEvent11__10IWorkEventFv();
    extern int WorkEvent12__10IWorkEventFv();
    extern int WorkEvent13__10IWorkEventFv();
    extern int WorkEvent14__10IWorkEventFv();
    extern int WorkEvent15__10IWorkEventFv();
    extern int WorkEvent16__10IWorkEventFv();
    extern int WorkEvent17__10IWorkEventFv();
    extern int WorkEvent18__10IWorkEventFv();
    extern int WorkEvent19__10IWorkEventFv();
    extern int WorkEvent20__10IWorkEventFv();
    extern int WorkEvent21__10IWorkEventFv();
    extern int WorkEvent22__10IWorkEventFv();
    extern int WorkEvent23__10IWorkEventFv();
    extern int WorkEvent24__10IWorkEventFv();
    extern int WorkEvent25__10IWorkEventFv();
    extern int WorkEvent26__10IWorkEventFv();
    extern int WorkEvent27__10IWorkEventFv();
    extern int WorkEvent28__10IWorkEventFv();
    extern int WorkEvent29__10IWorkEventFv();
    extern int WorkEvent30__10IWorkEventFv();
    extern void WorkEvent31__10IWorkEventFv();
    extern void wkUpdate__11CWorkThreadFv();
    extern void wkRender__11CWorkThreadFv();
    extern void wkRenderAfter__11CWorkThreadFv();
    extern void wkStandbyExceptionRetry__11CWorkThreadFUl(unsigned int);
}
extern "C" __declspec(section ".rodata") u32 lbl_eu_80522ED0[4] = { 0x43446576, 0x69636547, 0x58000000, 0x00000000 };
int CDeviceGX::gxHeapSize = 0x00200000;
const char* CDeviceGX::someString = lbl_eu_8066A428;
// Retail .data layout: the compiler emits __vt__9CDeviceGX (0xC0) itself;
// lbl_eu_8056CA20 (the RTTI base-list, 0x28) resolves to the retail data
// object at link -- only its reloc name must match.
extern "C" u32 lbl_eu_8056CA20[];
extern "C" __declspec(section ".sdata") u32 lbl_eu_80663758[2] = { (u32)&lbl_eu_80522ED0, (u32)&lbl_eu_8056CA20 };
extern "C" CGXCache* cacheInstance__9CDeviceGX = nullptr;
extern "C" u32 lbl_eu_806656A8 = 0;
extern "C" CDesktop* lbl_eu_806656AC = nullptr;
extern "C" u8 lbl_eu_806656B0 = 0;
extern u8 gap_10_806656B1_sbss[3];
// (DECOMP_FORCEACTIVE removed: every anchor below is referenced by real
// code/data, and the anchor stub's name embeds a line number, which made
// UNIT_RULES drop matching fragile.)

// Static members are declared once in CDeviceGX.hpp and defined here.

float CDeviceGX::sCostTime = 0.0f;
GXPixelFmt CDeviceGX::pixelFormat = (GXPixelFmt)0;
CDeviceGX* CDeviceGX::spInstance = nullptr;
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
    cacheInstance->clearStubFunc(0);
    setUnk260(1);
}

CDeviceGX::~CDeviceGX(){
    DELETE_ARRAY(mGxHeap);

    spInstance = nullptr;
}

CDeviceGX* CDeviceGX::getInstance(){
    return spInstance;
}

struct DeviceGxMsgQueueData {
    u8 pad[0x1A4];               // CWorkThread prefix through mMsgQueue.mEntries
    CMsgParamEntry* mArrayPtr;   // 0x1A4
    u32 mFront;                  // 0x1A8
    u32 mSize;                   // 0x1AC
    u32 mCapacity;               // 0x1B0
};
// Mirrors CMsgParam<8>::find (see CWorkRoot.cpp, FULL_MATCH); the scan is
// written directly in the body with a goto-gate walk (sunk -1, aliased
// index), reproducing the retail-inlined shape with no bl. Loop index is
// declared before the queue view it dereferences so MWCC colors inst=r6 /
// i=r7 as retail does (isInitialized inlined-find family recipe).
// NOTE: CDeviceBase declares its own mFlags at 0x1C4 shadowing
// CWorkThread::mFlags at 0x7C -- retail tests bit 4 of the 0x7C word, so the
// flag read must qualify CWorkThread:: explicitly. Likewise the state read
// targets CWorkThread::mState (0x48).
bool CDeviceGX::isInitialized(){
    const CDeviceGX* inst = spInstance;

    bool exception;
    if (inst->CWorkThread::mFlags & THREAD_FLAG_EXCEPTION) {
        exception = true;
    } else {
        const DeviceGxMsgQueueData* queue = (const DeviceGxMsgQueueData*)inst;
        int found;
        u32 i = 0;
        for (; i < queue->mSize; i++) {
            if (queue->mArrayPtr[(queue->mFront + i) % queue->mCapacity].command == EVT_EXCEPTION) {
                found = (int)i;
                goto merged;
            }
        }
        found = -1;
merged:
        exception = found >= 0;
    }
    bool result = false;
    if (!exception) {
        bool stateOK = true;
        int state = inst->CWorkThread::mState;
        if (state != THREAD_STATE_LOGIN && state != THREAD_STATE_RUN) {
            stateOK = false;
        }
        if (stateOK) {
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
    u8* readPtr;
    u8* writePtr;

    GXFlush();
    GXGetCPUFifo(&fifoTemp);
    GXGetFifoPtrs(&fifoTemp, (void**)&readPtr, (void**)&writePtr);
    
    u32 temp1 = unk26C;
    u32 temp = (u32)writePtr;
    u32 temp2;

    if(temp >= temp1){
        temp2 = temp - temp1;
    }else{
        temp1 -= temp;
        temp2 = gxHeapSize - temp1;
    }
    unk264 = ((f32)temp2 / (f32)gxHeapSize) * 2.0f;
}

void CDeviceGX::viBeginFrame(){
    if(spInstance->mDevicesInitialized == 1){
        return;
    }
    cacheInstance->resetGXStateA();
}

void CDeviceGX::drawFrame(){
    if(spInstance->mDevicesInitialized == 1){
        GXFlush();

        GXFifoObj fifoTemp;
        u8* readPtr;
        u8* writePtr;

        GXGetCPUFifo(&fifoTemp);
        GXGetFifoPtrs(&fifoTemp, (void**)&readPtr, (void**)&writePtr);
        spInstance->unk26C = (u32)writePtr;
        spInstance->unk270 = (u32)readPtr;
        GXEnableBreakPt(writePtr);

        GXSetDrawSync(token1);
        cacheInstance->resetGXStateA();

        if(spInstance->unk274 == 0){
            CDrawGX something;
            something.setCol(*cacheInstance->getClearColor());
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
    CDeviceGX* gx = spInstance;
    GXBool vf = gx->mFilter != VFILTER_NONE;
    GXRenderModeObj* rmode = CDeviceVI::getRenderModeObj();
    GXBool aa = CDeviceVI::getRenderModeObj()->aa;
    u8* vfilter = gx->mVFilter;
    GXSetCopyFilter(aa, rmode->sample_pattern, vf, vfilter);
    GXCopyDisp(pDestFrameBuffer, spInstance->unk274);
}

void CDeviceGX::calculateCost(){
    CStopwatchUtil::updateCostTime(someString);
    f32 visPerFrame = (f32)CDeviceVI::getVisPerFrame();
    f32 costTime = CStopwatchUtil::getCostTime(someString);
    sCostTime = costTime / visPerFrame;
}


void CDeviceGX::copyEfb(void* pDestFrameBuffer){
    if(spInstance->mDevicesInitialized == true){
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
    if(CDeviceVI::isRunningNoFlag2()){
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

        cacheInstance->resetGXStateA();
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
