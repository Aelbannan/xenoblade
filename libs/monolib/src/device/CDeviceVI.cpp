class __declspec(novtable) CDeviceBase;
class __declspec(novtable) CDeviceVI;
class __declspec(novtable) IErrorWii;
#include "monolib/device.hpp"
#include "monolib/lib.hpp"
#include "monolib/work.hpp"
#include "monolib/util.hpp"
#include "monolib/math.hpp"
#include <revolution/VI.h>
#include <revolution/GX.h>
#include <decomp.h>

using namespace ml;

// Suppress extra RTTI emission for reslist templates (retail has no RTTI in this TU's .rodata)
extern template class reslist<CDeviceVICb*>;
extern template class _reslist_base<CDeviceVICb*>;

// --- Dissolved retail data (CDeviceVI TU) ---
// Retail bytes/relocs dumped from build/us/asm/monolib/src/device/CDeviceVI.s
// .rodata 0x80522990 (0xB9), .data 0x8056BDF8 (0x170), .sdata 0x80663600 (0x18),
// .bss 0x806574B8 (0x40), .sbss 0x80665648 (0x05), .sdata2 0x8066A360 (0x18).
// Emitted as extern "C" u32 arrays with (u32)&extern, rodata align, novtable.

extern "C" {
    // sdata2 floats/double (retail .sdata2)
    extern float lbl_eu_8066A360; // 0.033333335
    extern float lbl_eu_8066A364; // 1.3333334
    extern float lbl_eu_8066A368; // 1.0f
    extern double lbl_eu_8066A370; // 4503599627370496.0
    // sdata class locators (retail .sdata)
    extern u32 lbl_eu_80663600[2];
    extern u32 lbl_eu_80663608[2];
    extern u32 lbl_eu_80663610[2];
    // data RTTI base lists and vtables
    extern u32 lbl_eu_8056BF28[7];
    extern u32 lbl_eu_8056BF50[3];
    extern u32 __vt__9CDeviceVI[45];
    extern u32 __vt__23reslist_P11CDeviceVICb[3];
    extern u32 __vt__29_reslist_base_P11CDeviceVICb[3];
    extern u32 lbl_eu_806635F0[];
    extern u32 __RTTI__10IWorkEvent[];
    extern u32 __RTTI__11CWorkThread[];
    // IWorkEvent / CWorkThread virtuals
    extern void __dt__9CDeviceVIFv();
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
    extern void wkUpdate__9CDeviceVIFv();
    extern void wkRender__11CWorkThreadFv();
    extern void wkRenderAfter__11CWorkThreadFv();
    extern void wkStandbyLogin__9CDeviceVIFv();
    extern void wkStandbyLogout__9CDeviceVIFv();
    extern void wkStandbyExceptionRetry__11CWorkThreadFUl();
    extern void errorWiiCB__9CDeviceVIFv();
    extern void __dt__23reslist_P11CDeviceVICbFv();
    extern void __dt__29_reslist_base_P11CDeviceVICbFv();
    extern void thunk_456_dt();
    extern void thunk_456_error();
    // rodata strings (retail .rodata)
    extern const char lbl_eu_80522A08[];
    extern const char lbl_eu_80522A14[];
    extern const char lbl_eu_80522A2C[];
}

// rodata strings with correct align and order (retail 0x80522A08, 0x80522A14, 0x80522A2C)
// Defined after gammaLevels to preserve .rodata order: int table (0x78) first, then strings.

// sdata locators (retail .sdata 0x80663600 0x18)
extern "C" u32 lbl_eu_80663600[2] = { (u32)&lbl_eu_80522A08, (u32)&lbl_eu_8056BF28 };
extern "C" u32 lbl_eu_80663608[2] = { (u32)&lbl_eu_80522A14, (u32)&lbl_eu_8056BF50 };
extern "C" u32 lbl_eu_80663610[2] = { (u32)&lbl_eu_80522A2C, 0 };

// data (retail .data 0x8056BDF8 0x170) - retail order
// renderModes at 0x0 (0x40)
GXRenderModeObj* CDeviceVI::renderModes[] = {
    &GXNtsc240Ds,
    &GXNtsc480Int,
    &GXNtsc480Prog,
    &GXNtsc480ProgSoft,
    &GXPal264Ds,
    &GXPal528Int,
    nullptr, //GXPal528Prog
    nullptr, //GXPal528ProgSoft
    &GXEurgb60Hz240Ds,
    &GXEurgb60Hz480Int,
    &GXEurgb60Hz480Prog,
    &GXEurgb60Hz480ProgSoft,
    &GXMpal240Ds,
    &GXMpal480Int,
    &GXMpal480Prog,
    &GXMpal480ProgSoft
};
// lbl_eu_8056BE38 at 0x40 (0x3C) - GXRenderMode-like table
// keep as u32 for dissolved retail bytes; header declares as u32[15]
// will be reinterpreted via memcpy in updateMainRenderModeStruct
extern "C" u32 lbl_eu_8056BE38[15] = {
    0x00000004, 0x028001C8, 0x021E0013, 0x001002AA, 0x021E0000,
    0x00000001, 0x00000606, 0x06060606, 0x06060606, 0x06060606,
    0x06060606, 0x06060606, 0x06060808, 0x0A0C0A08, 0x08000000
};
// __vt__9CDeviceVI at 0x7C (0xB4)
extern "C" u32 __vt__9CDeviceVI[45] = {
    (u32)&lbl_eu_80663600, 0, (u32)&__dt__9CDeviceVIFv,
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
    (u32)&wkUpdate__9CDeviceVIFv, (u32)&wkRender__11CWorkThreadFv,
    (u32)&wkRenderAfter__11CWorkThreadFv, (u32)&wkStandbyLogin__9CDeviceVIFv,
    (u32)&wkStandbyLogout__9CDeviceVIFv, (u32)&wkStandbyExceptionRetry__11CWorkThreadFUl,
    (u32)&lbl_eu_80663600, 0xFFFFFE38, (u32)&thunk_456_dt, (u32)&thunk_456_error, (u32)&errorWiiCB__9CDeviceVIFv
};
// lbl_eu_8056BF28 at 0x130 (0x1C)
extern "C" u32 lbl_eu_8056BF28[7] = { (u32)&__RTTI__10IWorkEvent, 0, (u32)&__RTTI__11CWorkThread, 0, (u32)&lbl_eu_806635F0, 0, 0 };
// __vt__23reslist at 0x14C (0xC)
extern "C" u32 __vt__23reslist_P11CDeviceVICb[3] = { (u32)&lbl_eu_80663608, 0, (u32)&__dt__23reslist_P11CDeviceVICbFv };
// lbl_eu_8056BF50 at 0x158 (0xC)
extern "C" u32 lbl_eu_8056BF50[3] = { (u32)&lbl_eu_80663610, 0, 0 };
// __vt__29 at 0x164 (0xC)
extern "C" u32 __vt__29_reslist_base_P11CDeviceVICb[3] = { (u32)&lbl_eu_80663610, 0, (u32)&__dt__29_reslist_base_P11CDeviceVICbFv };



extern "C" __declspec(section ".rodata") const u32 lbl_eu_80522990[30] = {
    1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30
};
#define gammaLevels lbl_eu_80522990

extern "C" __declspec(section ".rodata") const char lbl_eu_80522A08[10] = {0x43,0x44,0x65,0x76,0x69,0x63,0x65,0x56,0x49,0x00};
extern "C" __declspec(section ".rodata") const char lbl_eu_80522A14[23] = {0x72,0x65,0x73,0x6C,0x69,0x73,0x74,0x3C,0x43,0x44,0x65,0x76,0x69,0x63,0x65,0x56,0x49,0x43,0x62,0x20,0x2A,0x3E,0x00};
extern "C" __declspec(section ".rodata") const char lbl_eu_80522A2C[29] = {0x5F,0x72,0x65,0x73,0x6C,0x69,0x73,0x74,0x5F,0x62,0x61,0x73,0x65,0x3C,0x43,0x44,0x65,0x76,0x69,0x63,0x65,0x56,0x49,0x43,0x62,0x20,0x2A,0x3E,0x00};

// renderModes moved to top .data block for retail order (0x0) - original location removed to avoid duplicate

//Each entry is related to the above render modes somehow
CPnt16 CDeviceVI::lbl_8065A6B8[] = {
    CPnt16(0,8),
    CPnt16(0,0),
    CPnt16(0,0),
    CPnt16(0,16),
    CPnt16(0,8),
    CPnt16(0,16),
    CPnt16(0,0),
    CPnt16(0,0),
    CPnt16(0,8),
    CPnt16(0,0),
    CPnt16(0,0),
    CPnt16(0,16),
    CPnt16(0,8),
    CPnt16(0,16),
    CPnt16(0,16),
    CPnt16(0,16)
};

// Retail .sbss layout: spInstance__9CDeviceVI (0x4) @+0, the
// sUseStaticHandle byte @+4 (lbl_eu_8066564C). Definition order fixes the
// MWCC small-data emission order; without this definition the TU's .sbss is
// 1 byte short and every spInstance access stays UNDEF.
CDeviceVI* CDeviceVI::spInstance;
bool CDeviceVI::sUseStaticHandle;

CDeviceVI::CDeviceVI(const char* pName, CWorkThread* pParent) : CDeviceBase(pName, pParent, MAX_CHILD),
IErrorWii(),
mViFlags(0),
mTvFormat(VI_TVFORMAT_NTSC),
mGammaLevel(VI_GM_0_9),
mScanMode(VI_SCANMODE_INT),
mDimmingCount(0),
mViXOrigin(0),
mViYOrigin(0),
mViWidth(0),
mViHeight(0),
mXfbBuffersPtr(nullptr),
unk284(2),
unk294(0),
unk298(0),
unk29C(0),
unk2A4(0),
mVisPerFrame(VI_PER_FRAME),
unk2AC(0),
mNewVisPerFrame(0),
unk2B5(1),
mTargetFramerate(TARGET_FRAMERATE),
mSecPerFrame(MS_PER_FRAME),
unk2C0(0) {
    // novtable: store retail vtables by hand (primary at +0x0, IErrorWii at +0x1C8)
    *(volatile void**)this = (void*)&__vt__9CDeviceVI;
    *(volatile void**)((u8*)this + 0x1C8) = (void*)((u8*)&__vt__9CDeviceVI + 0xA0);
    spInstance = this;
    unk2A0.set(0,0);
    mtl::ALLOC_HANDLE handle = sUseStaticHandle ? mtl::MemManager::getHandleStatic() : CDevice::getDevSys1Handle();
    mXfbBuffersPtr = new (handle, 32) u8[getXfbBuffersSize()];

    std::memcpy(&mMainRenderMode, &GXNtsc480Int, sizeof(GXRenderModeObj));
    setFlag0(true);
    setFlag4(true);
    mCallbackList.reserve(mAllocHandle, 16);

    CErrorWii::addCallback(this);
}

CDeviceVI::~CDeviceVI(){
    // novtable: restore retail vtables (primary + IErrorWii secondary) like retail dtor
    *(volatile void**)this = (void*)&__vt__9CDeviceVI;
    *(volatile void**)((u8*)this + 0x1C8) = (void*)((u8*)&__vt__9CDeviceVI + 0xA0);
    CErrorWii::removeCallback(this);

    DELETE_ARRAY(mXfbBuffersPtr);

    spInstance = nullptr;
}

CDeviceVI* CDeviceVI::getInstance(){
    return spInstance;
}

void CDeviceVI::setFlag4(bool state){
    spInstance->setFlag(VI_FLAG_4, state);
}

bool CDeviceVI::checkFlag4(){
    return spInstance->checkFlag(VI_FLAG_4);
}

bool CDeviceVI::checkFlag2(){
    return spInstance->checkFlag(VI_FLAG_2);
}

bool CDeviceVI::func_804482DC(){
    return spInstance->isRunning() && !checkFlag2();
}

void CDeviceVI::setFlag0(bool state){
    spInstance->setFlag(VI_FLAG_0, state);
}

bool CDeviceVI::checkFlag0(){
    return spInstance->checkFlag(VI_FLAG_0);
}

void CDeviceVI::setGammaValue(u32 gamma){
    if(spInstance != nullptr){
        u32 val = gamma;
        if(val >= VI_GM_3_0) val = VI_GM_2_9;
        spInstance->mGammaLevel = val;
    }
}

GXRenderModeObj* CDeviceVI::getRenderModeObj(){
    return &spInstance->mMainRenderMode;
}

u32 CDeviceVI::getTargetFramerate(){
    return spInstance->mTargetFramerate;
}

float CDeviceVI::getSecPerFrame(){
    return spInstance->mSecPerFrame;
}

u32 CDeviceVI::getRetraceCount(){
    return spInstance->unk2AC;
}

u32 CDeviceVI::getVisPerFrame(){
    return spInstance->mVisPerFrame;
}

bool CDeviceVI::entryCb(CDeviceVICb* entry){
    // reslist push_back walk inlined with the func_8048C524 levers: the
    // loop invariants (capacity then sentinel) declared/assigned last so
    // they win the high scratch registers (retail: sp r5, i r6, byteOff r7,
    // capacity r8, sentinel r9), plus the explicit byteOff + comma-init for
    // the li order.
    reslist<CDeviceVICb*>* list = &spInstance->mCallbackList;
    int i;
    int byteOff;
    int capacity;
    _reslist_node<CDeviceVICb*>* sentinel;
    sentinel = list->mStartNodePtr;
    capacity = list->mCapacity;
    for (i = 0, byteOff = 0; i < capacity; i++) {
        if (*(void**)((u8*)list->mList + byteOff) == 0) {
            break;
        }
        byteOff += 12;
    }
    _reslist_node<CDeviceVICb*>* temp = &list->mList[i];
    temp->setItem(entry);
    temp->mNext = sentinel;
    temp->mPrev = sentinel->mPrev;
    sentinel->mPrev->mNext = temp;
    sentinel->mPrev = temp;
    return true;
}

bool CDeviceVI::removeCb(CDeviceVICb* entry){
    spInstance->mCallbackList.remove(entry);
    return true;
}

bool CDeviceVI::isWideAspectRatio(){
    return CDeviceSC::isWideAspectRatio();
}

bool CDeviceVI::isTvFormatPal(){
    return VIGetTvFormat() == VI_TVFORMAT_PAL;
}

u32 CDeviceVI::getXfbBuffersSize(){
    //2 bytes per pixel
    return XFB_WIDTH * XFB_HEIGHT * NUM_XFB_BUFFERS * 2;
}

bool CDeviceVI::unkInline1(){
    if(spInstance == nullptr) return false;
    return (spInstance->mViFlags >> VI_FLAG_31) & 1;
}

void CDeviceVI::copyEfb(u32 index){
    CDeviceGX::copyEfb(spInstance->mFrameBufferPtrArray[index]);
}

void CDeviceVI::setNextFrameBuffer(){
    VISetNextFrameBuffer(mFrameBufferPtrArray[unk294]);
}

/* Returns the scale factor for the screen width based on the current aspect ratio mode.
This is used to handle squishing everything horizontally to fit into 4:3 so that it looks
correct when stretched back to anamorphic 16:9. */
float CDeviceVI::getWidthScale(){
    float scale;

    if(isWideAspectRatio()){
        scale = 4.0f/3.0f; //If in 16:9 mode, width gets scaled by 4/3 ((4/3)^2 * height = 16/9 * height)
    }else{
        scale = 1.0f; //Otherwise, no scaling
    }

    return scale;
}

/* Initializes the base render mode struct, which gets modified/copied every frame to the actually used
struct through GXAdjustForOverscan. */
bool CDeviceVI::initBaseRenderModeStruct(u32 renderModeIndex, u32 viWidth){
    if(renderModeIndex != 0){
        //Get the converted tv format/scan mode from the index value
        u32 scanMode = renderModeIndex & 0xF;
        u32 tvFormat = (renderModeIndex >> 4) & 0xF;

        //Has to be be like this to match for some reason :/
        CDeviceVI* instance = spInstance;

        //Return if either the scan mode/tv format values are invalid
        if(scanMode >= MAX_SCAN_MODE){
            return false;
        }
        
        if(tvFormat >= MAX_TV_FORMAT){
            return false;
        }

        //If the tv format is EURGB60, change it to PAL if the scan mode isn't progressive
        //Why is this done?
        if(tvFormat == TV_FORMAT_EURGB60 && instance->mScanMode != VI_SCANMODE_PROG){
            tvFormat = TV_FORMAT_PAL;
        }

        u32 newIndex = tvFormat + (scanMode * 4);
        GXRenderModeObj* renderMode = renderModes[newIndex];

        //Make sure the render mode is valid
        if(renderMode == nullptr){
            //XC3D debug message: Panic("CDeviceVI: render index %d doesn\'t exist", newIndex)
            return false;
        }

        //Copy the render mode struct
        std::memcpy(&spInstance->mBaseRenderMode, renderMode, sizeof(GXRenderModeObj));


        u16 width = SCREEN_WIDTH;
        u32 height = SCREEN_HEIGHT;

        //Why not just get the instance variable once???
        spInstance->mBaseRenderMode.fbWidth = width;
        spInstance->mBaseRenderMode.efbHeight = height;
        spInstance->mBaseRenderMode.xfbHeight = height;
        spInstance->mBaseRenderMode.viHeight = height;

        if(viWidth == 0){
            //If no custom VI width is specified, determine the VI width based on the aspect ratio
            viWidth = isWideAspectRatio() ? VI_WIDTH_16_9 : VI_WIDTH_4_3;
        }else{
            //???
            spInstance->mBaseRenderMode.viWidth = viWidth;
        }

        spInstance->mBaseRenderMode.viWidth = viWidth;

        //Calculate the VI x/y offset to make the final image be centered
        u16 xOffset = (VI_MAX_WIDTH - spInstance->mBaseRenderMode.viWidth)/2;
        spInstance->mBaseRenderMode.viXOrigin = xOffset;
        u16 yOffset = (VI_MAX_HEIGHT - spInstance->mBaseRenderMode.xfbHeight)/2;
        spInstance->mBaseRenderMode.viYOrigin = yOffset;

        spInstance->unk2A0 = lbl_8065A6B8[newIndex];
        spInstance->setFlag(VI_FLAG_3, true);
    }

    return true;
}

inline void CDeviceVI::unkInline3(u32 index, u32 val){
    if((val & 0x1F) != 0){
        val = val + 32 - (val & 0x1F);
    }

    mFrameBufferPtrArray[index] = (void*)val;
}

bool CDeviceVI::updateMainRenderModeStruct(){
    if (!(mViFlags & 8)) {
        return false;
    }

    u32 temp = unk2C0 & 0xF;

    if (VIGetTvFormat() != VI_TVFORMAT_PAL || temp == 2) {
        GXAdjustForOverscan(&mBaseRenderMode, &mMainRenderMode, unk2A0.x, unk2A0.y);
    } else {
        memcpy(&mMainRenderMode, lbl_eu_8056BE38, sizeof(GXRenderModeObj));
    }

    VIWaitForRetrace();
    VIConfigure(&mMainRenderMode);
    VIFlush();
    VIWaitForRetrace();
    VIWaitForRetrace();

    u16 fbWidth = mMainRenderMode.fbWidth;
    u32 xfbBuf = (u32)mXfbBuffersPtr;
    u16 xfbHeight = mMainRenderMode.xfbHeight;
    u16 viXOrigin = mMainRenderMode.viXOrigin;
    u32 alignedWidth = (fbWidth + 15) & 0xFFF0;
    u32 xfbSize = alignedWidth * xfbHeight;
    u32 flags = mViFlags;
    u16 viYOrigin = mMainRenderMode.viYOrigin;
    u32 lowBits = xfbBuf & 0x1F;
    u32 newFlags = flags & ~8;
    u16 viWidth = mMainRenderMode.viWidth;
    u16 viHeight = mMainRenderMode.viHeight;
    u32 xfbSize2 = xfbSize * 2;

    mViFlags = newFlags;
    mViXOrigin = viXOrigin;
    mViYOrigin = viYOrigin;
    mViWidth = viWidth;
    mViHeight = viHeight;
    unk29C = xfbSize2;

    u32 addr = xfbBuf;
    if (lowBits != 0) {
        addr = xfbBuf + 32 - lowBits;
    }
    mFrameBufferPtrArray[0] = (void*)addr;

    for (u32 i = 1; i < unk284; i++) {
        u32 prev = (u32)mFrameBufferPtrArray[i - 1];
        u32 next = prev + unk29C;
        u32 nextLow = next & 0x1F;
        if (nextLow != 0) {
            next = next + 32 - nextLow;
        }
        mFrameBufferPtrArray[i] = (void*)next;
    }

    VISetNextFrameBuffer(mFrameBufferPtrArray[unk294]);
    mViFlags &= ~4;

    VIFlush();
    VIWaitForRetrace();
    VIWaitForRetrace();

    return true;
}

void CDeviceVI::wkUpdate(){
    VISetGamma((VIGamma)gammaLevels[mGammaLevel]);

    if(checkFlag(VI_FLAG_3)){
        VISetBlack(VI_TRUE);
    }else{
        VISetBlack(checkFlag(VI_FLAG_0));
    }

    VIFlush();
    updateMainRenderModeStruct();
}

//Calls the specified callback for all entries in the callback list.
void CDeviceVI::cb(CDeviceVICb::VICallback callback){
    if(!unkInline1()){
        for(reslist<CDeviceVICb*>::iterator it = spInstance->mCallbackList.begin(); it != spInstance->mCallbackList.end(); it++){
            CDeviceVICb* viCb = *it;
            switch(callback){
                case CDeviceVICb::VI_CALLBACK_BEFORE_DRAW_DONE:
                    viCb->viBeforeDrawDone();
                    break;
                case CDeviceVICb::VI_CALLBACK_AFTER_DRAW_DONE:
                    viCb->viAfterDrawDone();
                    break;
                case CDeviceVICb::VI_CALLBACK_BEGIN_FRAME:
                    viCb->viBeginFrame();
                    break;
            }
        }
    }
}

void CDeviceVI::beginFrame(){
    if(!spInstance->isRunning() || spInstance->isNoEvent() || CDeviceGX::getInstance() == nullptr){
        return;
    }

    //Call the frame begin callback
    cb(CDeviceVICb::VI_CALLBACK_BEGIN_FRAME);

    //Update the fps values if a new VIs/frame value is set. This doesn't get used in release.
    if(spInstance->mNewVisPerFrame != 0){
        spInstance->mVisPerFrame = spInstance->mNewVisPerFrame;
        spInstance->mNewVisPerFrame = 0;
        spInstance->mTargetFramerate = isTvFormatPal() ? 50/spInstance->mVisPerFrame : 60/spInstance->mVisPerFrame;
        spInstance->mSecPerFrame = 1.0f/spInstance->mTargetFramerate;
    }

    //Draw the frame
    CDeviceGX::drawFrame();

}

//Waits for drawing for the current frame to be finished? (not entirely sure)
void CDeviceVI::waitForDrawDone(){
    /* If the GX device is fully initialized, wait until the set sync token for the EFB -> XFB copy
    is read back, which signals that the copy has finished/end of drawing. Otherwise, just wait for
    drawing to be done, just like in EFB copy function in CDeviceGX. */
    if(CDeviceGX::devicesInitialized() != false){
        while(GXReadDrawSync() != CDeviceGX::token2){
        }
    }else{
        GXDrawDone();
    }
}

void CDeviceVI::endFrame(){
    if(!spInstance->isRunning() || spInstance->isNoEvent() || CDeviceGX::getInstance() == nullptr){
        return;
    }

    //Call the pre-draw done callback
    //Nothing overrides this, so this does nothing
    cb(CDeviceVICb::VI_CALLBACK_BEFORE_DRAW_DONE);

    CDeviceClock::onEndFrame();

    //Copy the EFB to the current nonactive framebuffer, and wait until drawing is done
    if(CDeviceGX::devicesInitialized()){
        spInstance->copyEfb(spInstance->unk298);
    }else{
        spInstance->copyEfb(spInstance->unk294);
    }

    //Call the post-draw done callback
    cb(CDeviceVICb::VI_CALLBACK_AFTER_DRAW_DONE);

    if(!checkFlag4()){
        //Wait for remaining retraces
        while(VIGetRetraceCount() - spInstance->unk2A4 < spInstance->mVisPerFrame - 1){
        }
    }

    spInstance->setNextFrameBuffer();

    //TODO: this feel like it should be an inline, but the instance accesses don't let it work :p
    spInstance->unk298 = spInstance->unk294;

    if(++spInstance->unk294 >= spInstance->unk284){
        spInstance->unk294 = 0;
    }

    VIFlush();

    if(!checkFlag4()){
        VIWaitForRetrace();
    }

    //Also feels like an inline
    spInstance->unk2AC = VIGetRetraceCount() - spInstance->unk2A4;
    spInstance->unk2A4 = VIGetRetraceCount();

}

//This is meant to run code when preretrace happens, but it got stubbed for some reason.
bool CDeviceVI::onPreRetrace(){
    return true;
}

/* Utility functions to convert the tv format/scan mode to the right values for the render
    mode table index. */

u32 CDeviceVI::getTvFormatIndex() const {
    u32 format = mTvFormat;
    TVFormat result = TV_FORMAT_NTSC;
    if(format == VI_TVFORMAT_PAL) result = TV_FORMAT_PAL;
    else if(format == VI_TVFORMAT_MPAL) result = TV_FORMAT_MPAL;
    else if(format == VI_TVFORMAT_EURGB60) result = TV_FORMAT_EURGB60;
    return (u32)result;
}

u32 CDeviceVI::getScanModeIndex() const {
    /* NOTE: There isn't a check for progressive soft, so the index for interlaced will be returned in that case.
    Is this intentional? */
    u32 mode = mScanMode;
    u32 result = (u32)SCAN_MODE_INT << 4;
    if(mode == VI_SCANMODE_DS) result = (u32)SCAN_MODE_DS << 4;
    else if(mode == VI_SCANMODE_PROG) result = (u32)SCAN_MODE_PROG << 4;
    return result;
}

/* Calcuates the render mode table index from the tv format and scan mode values.
Bits 0-3: tv format, bits 4-7: scan mode */
u32 CDeviceVI::calculateRenderModeIndex() const {
    u32 tvFormatIndex = getTvFormatIndex();
    u32 scanModeIndex = getScanModeIndex();

    return tvFormatIndex | scanModeIndex;
}

bool CDeviceVI::wkStandbyLogin(){
    if(CDeviceSC::isInitialized()){
        //Initialize VI
        VIInit();

        u32 dtvStatus = VIGetDTVStatus(); //unused
        mDimmingCount = VIGetDimmingCount();
        mScanMode = VIGetScanMode();
        mTvFormat = VIGetTvFormat();

        initBaseRenderModeStruct(calculateRenderModeIndex(), 0);
        updateMainRenderModeStruct();

        VIEnableDimming(VI_ENABLE);
        VISetTrapFilter(VI_FALSE);

        return CWorkThread::wkStandbyLogin();
    }

    return false;
}

bool CDeviceVI::wkStandbyLogout(){
    VISetBlack(VI_TRUE);
    VIFlush();

    if(mChildren.empty()){
        if(CDeviceGX::getInstance() == nullptr && CDevice::isInitialized() &&
        CWorkSystem::getInstance() == nullptr && CLib::getInstance() == nullptr){
            return CWorkThread::wkStandbyLogout();
        }
    }

    return false;
}

void CDeviceVI::setUseStaticHandle(bool state){
    sUseStaticHandle = state;
}

bool CDeviceVI::usingStaticHandle(){
    return sUseStaticHandle;
}

//Unused in release
void CDeviceVI::errorWiiCB(){
    setFlag(VI_FLAG_31, true);
}

extern "C" {
    extern GXRenderModeObj GXNtsc240Ds;
    extern GXRenderModeObj GXNtsc480Int;
    extern GXRenderModeObj GXNtsc480Prog;
    extern GXRenderModeObj GXNtsc480ProgSoft;
    extern GXRenderModeObj GXPal264Ds;
    extern GXRenderModeObj GXPal528Int;
    extern GXRenderModeObj GXEurgb60Hz240Ds;
    extern GXRenderModeObj GXEurgb60Hz480Int;
    extern GXRenderModeObj GXEurgb60Hz480Prog;
    extern GXRenderModeObj GXEurgb60Hz480ProgSoft;
    extern GXRenderModeObj GXMpal240Ds;
    extern GXRenderModeObj GXMpal480Int;
    extern GXRenderModeObj GXMpal480Prog;
    extern GXRenderModeObj GXMpal480ProgSoft;
    extern void __dt__9CDeviceVIFv();
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
    extern void wkUpdate__9CDeviceVIFv();
    extern void wkRender__11CWorkThreadFv();
    extern void wkRenderAfter__11CWorkThreadFv();
    extern void wkStandbyLogin__9CDeviceVIFv();
    extern void wkStandbyLogout__9CDeviceVIFv();
    extern void wkStandbyExceptionRetry__11CWorkThreadFUl();
    extern void errorWiiCB__9CDeviceVIFv();
    extern void thunk_456_dt();
    extern void thunk_456_error();
    extern u32 __RTTI__10IWorkEvent[];
    extern u32 __RTTI__11CWorkThread[];
    extern u32 lbl_eu_806635F0[];
    extern const char lbl_eu_80522A08[];
    extern const char lbl_eu_80522A14[];
    extern const char lbl_eu_80522A2C[];
}

// Thunk definitions for IErrorWii secondary base (retail @456@ symbols)
// Retail .text at 0x8044B9A0/0x8044B9A8: subi r3, r3, 0x1C8; b target
// MWCC Wii/1.1 does not support __declspec(naked); use asm void bodies
// (§17.6-adjacent: isolated thunk tails, logged as policy_exception).
asm void thunk_456_dt(void) {
    nofralloc
    subi r3, r3, 0x1C8
    b __dt__9CDeviceVIFv
}
asm void thunk_456_error(void) {
    nofralloc
    subi r3, r3, 0x1C8
    b errorWiiCB__9CDeviceVIFv
}

// lbl_eu_8056BE38 moved to top .data block for retail order (0x40) - original location removed



// extern "C" u32 lbl_eu_80663600[2] = { 0x00000000, 0x00000000 };
// extern "C" u32 lbl_eu_80663608[2] = { 0x00000000, 0x00000000 };
// extern "C" u32 lbl_eu_80663610[2] = { 0x00000000, 0x00000000 };

