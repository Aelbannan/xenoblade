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
extern "C" __declspec(section ".sdata") int gxHeapSize__9CDeviceGX = 0x00200000;
extern "C" __declspec(section ".sdata") const char* someString__9CDeviceGX = lbl_eu_8066A428;
// Retail .data layout: the compiler emits __vt__9CDeviceGX (0xC0) itself;
// lbl_eu_8056CA20 (the RTTI base-list, 0x28) resolves to the retail data
// object at link -- only its reloc name must match.
extern "C" u32 lbl_eu_8056CA20[];
extern "C" __declspec(section ".sdata") u32 lbl_eu_80663758[2] = { (u32)&lbl_eu_80522ED0, (u32)&lbl_eu_8056CA20 };
extern "C" CGXCache* cacheInstance__9CDeviceGX = nullptr;
extern "C" u32 lbl_eu_806656A8 = 0;
extern "C" CDesktop* lbl_eu_806656AC = nullptr;
extern "C" u8 lbl_eu_806656B0 = 0;
extern "C" u8 gap_10_806656B1_sbss[3];
// (DECOMP_FORCEACTIVE removed: every anchor below is referenced by real
// code/data, and the anchor stub's name embeds a line number, which made
// UNIT_RULES drop matching fragile.)

// Static member macros
#define spInstance lbl_eu_806656A0
#define cacheInstance cacheInstance__9CDeviceGX
#define pixelFormat lbl_eu_8066569C
#define gxHeapSize gxHeapSize__9CDeviceGX
#define sCostTime lbl_eu_80665698
#define someString someString__9CDeviceGX

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
    lbl_eu_806656A0 = this;
    cacheInstance = &unk27C;
    mGxHeap = new (CDevice::getDevSys1Handle(), 32) u8[gxHeapSize__9CDeviceGX];
    mGxHeapEndAddr = mGxHeap + gxHeapSize__9CDeviceGX;
    cacheInstance->unk50C = 0;
    updateVerticalFilter(VFILTER_NONE);
    cacheInstance->clearStubFunc(0);
    setUnk260(1);
}

CDeviceGX::~CDeviceGX(){
    DELETE_ARRAY(mGxHeap);

    lbl_eu_806656A0 = nullptr;
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
static int FindMsgException(const MyQueueData* q, u32 msg);
// Mirrors CMsgParam<8>::find (see CWorkRoot.cpp, FULL_MATCH); -inline auto
// folds this single-call static into isInitialized so the retail-inlined
// scan shape (sunk -1, aliased index) reproduces.
bool CDeviceGX::isInitialized(){
    bool exception;
    if (*(u32*)((u8*)lbl_eu_806656A0 + 0x7C) & THREAD_FLAG_EXCEPTION) {
        exception = true;
    } else {
        exception = FindMsgException((const MyQueueData*)lbl_eu_806656A0, 2) >= 0;
    }
    bool result = false;
    if (!exception) {
        bool stateOK = true;
        ThreadState state = *(ThreadState*)((u8*)lbl_eu_806656A0 + 0x48);
        if (state != THREAD_STATE_LOGIN && state != THREAD_STATE_RUN) {
            stateOK = false;
        }
        if (stateOK) {
            result = true;
        }
    }
    return result;
}

// Mirrors CMsgParam<8>::find body (CWorkRoot.cpp holds the out-of-line
// definition); -inline auto folds this single-call static into isInitialized
// reproducing the retail-inlined scan shape (sunk -1, aliased index, no bl).
static int FindMsgException(const MyQueueData* q, u32 msg) {
    for (int i = 0; i < q->mSize; i++) {
        if (q->mArrayPtr[(q->mFront + i) % q->mCapacity].command == msg) {
            return i;
        }
    }
    return -1;
}

void CDeviceGX::setDevicesInitializedFlag(bool state){
    lbl_eu_806656A0->mDevicesInitialized = state;
}

bool CDeviceGX::devicesInitialized(){
    return lbl_eu_806656A0->mDevicesInitialized == true;
}

void CDeviceGX::updateVerticalFilter(EVerticalFilter filter){
    //Default to filter 0
    lbl_eu_806656A0->mFilter = filter;
    
    //The game only ever calls this function with VFILTER_0, so these other filters go unused.
    if(lbl_eu_806656A0->mFilter == VFILTER_1){
        lbl_eu_806656A0->mVFilter[0] = 0;
        lbl_eu_806656A0->mVFilter[1] = 3;
        lbl_eu_806656A0->mVFilter[2] = 19;
        lbl_eu_806656A0->mVFilter[3] = 20;
        lbl_eu_806656A0->mVFilter[4] = 19;
        lbl_eu_806656A0->mVFilter[5] = 3;
        lbl_eu_806656A0->mVFilter[6] = 0;
        lbl_eu_806656A0->mVFilter[7] = 0;
    }else if(lbl_eu_806656A0->mFilter == VFILTER_2){
        lbl_eu_806656A0->mVFilter[0] = 4;
        lbl_eu_806656A0->mVFilter[1] = 4;
        lbl_eu_806656A0->mVFilter[2] = 15;
        lbl_eu_806656A0->mVFilter[3] = 18;
        lbl_eu_806656A0->mVFilter[4] = 15;
        lbl_eu_806656A0->mVFilter[5] = 4;
        lbl_eu_806656A0->mVFilter[6] = 4;
        lbl_eu_806656A0->mVFilter[7] = 0;
    }else if(lbl_eu_806656A0->mFilter == VFILTER_3){
        lbl_eu_806656A0->mVFilter[0] = 8;
        lbl_eu_806656A0->mVFilter[1] = 8;
        lbl_eu_806656A0->mVFilter[2] = 10;
        lbl_eu_806656A0->mVFilter[3] = 12;
        lbl_eu_806656A0->mVFilter[4] = 10;
        lbl_eu_806656A0->mVFilter[5] = 8;
        lbl_eu_806656A0->mVFilter[6] = 8;
        lbl_eu_806656A0->mVFilter[7] = 0;
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
    cacheInstance->resetGXStateA();
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
        cacheInstance->resetGXStateA();

        if(lbl_eu_806656A0->unk274 == 0){
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
    return gxHeapSize__9CDeviceGX;
}

bool CDeviceGX::wkStandbyLogin(){
    if(CDeviceVI::func_804482DC()){
        GXInit(mGxHeap, gxHeapSize);

        if(lbl_eu_806656A0->mDevicesInitialized == true){
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
            GXSetPixelFmt(lbl_eu_8066569C, GX_ZC_LINEAR);
        }

        cacheInstance->resetGXStateA();
        GXSetDither(GX_DISABLE);

        if(lbl_eu_806656A0->mDevicesInitialized == true){
            GXSetDrawSyncCallback(drawSyncCallback);
        }

        return CWorkThread::wkStandbyLogin();
    }

    return false;
}

bool CDeviceGX::wkStandbyLogout(){
    if(lbl_eu_806656A0->mDevicesInitialized == true){
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
