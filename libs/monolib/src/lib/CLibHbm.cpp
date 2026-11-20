#include "monolib/lib/CLibHbm.hpp"
#include "monolib/lib/CLibHbmControl.hpp"
#include "monolib/core.hpp"
#include "monolib/device.hpp"
#include "monolib/scn.hpp"
#include <revolution/ARC.h>
#include <cstring>

//I don't wanna type these several times
#define HOME_BUTTON_PRESSED(channel) (CDeviceRemotePad::getPressedButtonFlags(channel) & PAD_INPUT_FLAG_HOME)
#define HOME_BUTTON_HELD(channel) (CDeviceRemotePad::getHeldButtonFlags(channel) & PAD_INPUT_FLAG_HOME)

// ===== Dissolved monolibdata2: static-member storage at retail addresses =====
// spInstance -> lbl_eu_806656F8 and lbl_80667FD4 -> lbl_eu_806656FC live in
// CLibG3d.o's sbss span; sCurWpadChannel -> lbl_eu_806637C0 lives in
// CLibG3d.o's sdata; spHbmstopTplData/sHbmStopPending/sHbmStopFlag are this
// TU's own sbss words (defined below).
#define spInstance lbl_eu_806656F8
extern "C" CLibHbm* lbl_eu_806656F8;
#define sCurWpadChannel lbl_eu_806637C0
extern "C" int lbl_eu_806637C0;
#define lbl_80667FD4 lbl_eu_806656FC
extern "C" bool lbl_eu_806656FC;
#define spHbmstopTplData lbl_eu_80665700
#define sTplTexObj lbl_eu_80657F90
extern "C" GXTexObj lbl_eu_80657F90; // storage defined in CException.cpp's .bss slice

// [.sbss] 0x80665700-0x80665710 zero-fill owned by this TU (size/align are
// compared, not bytes): lbl_eu_80665700 = spHbmstopTplData,
// lbl_eu_80665704/05 = sHbmStopPending/sHbmStopFlag, and lbl_eu_80665708 is the
// CLibHbmControl singleton-pointer storage (its TU declares it extern).
TPLPalette* lbl_eu_80665700;
// name-guess: isHbmStopPending() reads this word; set by the HBM stop release path
bool sHbmStopPending;
// name-guess: setHbmStopFlag() writes this word; wkUpdate tests it before starting HBM control
bool sHbmStopFlag;
u32 lbl_eu_80665708[2];

// Initialize the derived fields in declaration order; MWCC emits the real
// derived vptr store followed by these stores.
CLibHbm::CLibHbm(const char* pName, CWorkThread* pParent)
    : CWorkThread(pName, pParent, MAX_CHILD),
      mHandle(mtl::INVALID_HANDLE), unk1C8(0),
      mpLayoutBuf(nullptr), mpSpkSeBuf(nullptr), mpHbmSeBuf(nullptr),
      mpMsgBuf(nullptr), mpConfigBuf(nullptr), mpHbmMem(nullptr),
      mpHbmSndMem(nullptr), unk1E8(nullptr), mFlags(0),
      mpHbmArcFileHandle(nullptr), mConfigBufSize(0), unk25C(0.0f),
      mState(STATE_NEG1), unk264(0), unk265(0) {
    spInstance = this;
    mType = THREAD_CLIBHBM;
}

CLibHbm::~CLibHbm(){
    spInstance = nullptr;
}

CLibHbm* CLibHbm::getInstance(){
    return spInstance;
}

void CLibHbm::setCurrentWpadChannel(int channel){
    if(channel >= WPAD_CHAN_INVALID && channel < WPAD_MAX_CONTROLLERS) sCurWpadChannel = channel;
}

void CLibHbm::setHbmStopFlag(bool flag){
    sHbmStopFlag = flag;
}

bool CLibHbm::isHbmStopPending(){
    return sHbmStopPending;
}

void CLibHbm::loadTplImage(void* pTplData){
    if(spHbmstopTplData == nullptr){
        TPLPalette* tpl = (TPLPalette*)pTplData;
        spHbmstopTplData = tpl;

        if(tpl != nullptr){
            TPLBind(tpl);
            TPLGetGXTexObjFromPalette(spHbmstopTplData, &sTplTexObj, 0);
            GXInitTexObjLOD(&sTplTexObj, GX_LINEAR, GX_LINEAR, 0, 0, 0,
            GX_FALSE, GX_FALSE, GX_ANISO_1);
        }
    }
}

void CLibHbm::removeTplImage(){
    CDeviceVI::waitForDrawDone();
    spHbmstopTplData = nullptr;
}

void CLibHbm::addCallback(IHBMCallback* callback){
    CLibHbm* instance = spInstance;
    if(instance == nullptr) return;

    if(instance->unk238.mCount < 8){
        instance->unk238.push_back(callback);
    }
}

//TODO: same as the erase function in CErrorWii
void CLibHbm::removeCallback(IHBMCallback* callback){
    if(spInstance == nullptr) return;

    for(int i = 0; i < spInstance->unk238.size(); i++){
        if(spInstance->unk238.mArray[i] == callback){
            while(i < spInstance->unk238.mCount - 1){
                spInstance->unk238.mArray[i] = spInstance->unk238.mArray[i + 1];
                i++;
            }

            spInstance->unk238.mCount--;
            return;
        }
    }

}

void CLibHbm::setHbmActiveFlag(bool flag){
    lbl_80667FD4 = flag;
}

void CLibHbm::destroy(){
    CDeviceVI::waitForDrawDone();

    //TODO: custom delete operator?
    if(mpHbmArcFileHandle != nullptr){
        CDeviceFile::cancel(mpHbmArcFileHandle);
        mpHbmArcFileHandle = nullptr;
    }

    DELETE_OBJ(mpHbmSndMem);
    DELETE_OBJ(unk1E8);
    mpSpkSeBuf = 0;
    mpMsgBuf = 0;
    mpConfigBuf = 0;
    mpLayoutBuf = 0;
    mpHbmSeBuf = 0;
    DELETE_OBJ(mpHbmMem);

    mHandle = mtl::INVALID_HANDLE;

    if(mFlags & 0x20){
        updateScnCounter(0);
        mFlags &= ~0x20;
    }

    mFlags &= ~0xC0;
}

bool CLibHbm::isHbmMemPointerValid(){
    return spInstance->mpHbmMem != nullptr;
}

bool CLibHbm::checkFlag6(){
    return (spInstance->mFlags >> 6) & 1;
}

void CLibHbm::loadHbmArcFile(){
    // Filename globals owned by CLibG3d.cpp (.sdata pointers to "hbm.arc" strings).
    extern const char* lbl_eu_806637B8;
    extern const char* lbl_eu_806637BC;

    spInstance->mFlags &= ~0x40;

    CDeviceVI::waitForDrawDone();
    updateScnCounter(1);
    spInstance->mFlags |= 0x20;
    spInstance->mHandle = getScnCounter();

    if(spInstance->mHandle == mtl::INVALID_HANDLE){
        spInstance->mHandle = mtl::MemManager::getHandleMEM2();
    }

    // Pick the archive path by console language (6 = Japanese).
    const char* pArcFile = lbl_eu_806637B8;
    if(CDeviceSC::getLanguage() == 6){
        pArcFile = lbl_eu_806637BC;
    }

    // Retail passes an extra ignored int arg (r4=1) to the 1-param
    // CDeviceFile::getFileSize; route through a cast function-pointer type.
    typedef int (*GetFileSizeFn)(const char*, int);
    if(((GetFileSizeFn)&CDeviceFile::getFileSize)(pArcFile, 1) > 0){  // DEBT(callcast): getFileSize__11CDeviceFileFPCc has 1 param; retail leaves r4=1 stale
        CFileHandle* handle = CDeviceFile::readFile(spInstance->mHandle, pArcFile,
        spInstance, 0, 0);
        spInstance->mpHbmArcFileHandle = handle;
        CDeviceFile::tryUpdateJobPriority(spInstance->mpHbmArcFileHandle, 0);
    }
}

// Retail inlines CMsgParam<8>::find (and through it CWorkThread::isException /
// isRunning) into this TU, while CWorkRoot.cpp keeps the strong out-of-line
// find. A TU-local inline specialization reproduces that without changing
// the shared header for every other TU (same trick as CLibVM.cpp / CDeviceGX.cpp
// for isRunning).
template <>
inline int CMsgParam<8>::find(u32 msg) const {
    for (int i = 0; i < mRing.mSize; i++) {
        if (mRing.mArrayPtr[(mRing.mFront + i) % mRing.mCapacity].command == msg) {
            return i;
        }
    }
    return -1;
}

inline bool CWorkThread::isRunning() const {
    return !isException() && (mState == THREAD_STATE_LOGIN || mState == THREAD_STATE_RUN);
}

bool CLibHbm::isInitialized(){
    return spInstance->isRunning();
}

void CLibHbm::wkUpdate(){
    if(CDesktop::getInstance() != nullptr){
        int channel = sCurWpadChannel >= 0 ? sCurWpadChannel : CDeviceRemotePad::getFirstConnectedWpadPort();

        if(CLibHbmControl::getInstance() != nullptr){
            if(CLibHbmControl::isActive()) return;

            // Retail inlines CWorkThread::isException()'s flag test + queued
            // EVT scan straight into wkUpdate (CMsgParam::find lives
            // out-of-line in another TU, so walk the queue via the mirror).
            CDeviceFileCri* fileCri = CDeviceFileCri::getInstance();
            bool criException = fileCri->isException();

            if(criException){
                if(!CLibHbmControl::getInstance()->isNoEvent() && mState < STATE_0 && !unk264){
                    if(mState == STATE_NEG1){
                        setState(STATE_0);
                    }
                    unk264 = true;
                }

                if(channel >= 0 && !lbl_80667FD4 && HOME_BUTTON_PRESSED(channel) && !CDeviceVI::checkFlag0()
                && mState == STATE_NEG1){
                    mState = STATE_0;

                    if(spHbmstopTplData == nullptr){
                        mState = STATE_NEG1;
                    }
                }
            }else{
                if(unk264){
                    CLibHbmControl::getInstance()->wkSetEvent(EVT_NONE);
                }

                unk264 = false;

                if(!sHbmStopFlag){
                    mState = STATE_3;
                    if(spHbmstopTplData == nullptr){
                        mState = STATE_NEG1;
                    }
                }
            }
        }else{
            if(unk265 && channel >= 0 && !HOME_BUTTON_HELD(channel)){
                unk265 = false;
            }

            if(!unk265 && channel >= 0 && !lbl_80667FD4){
                if((HOME_BUTTON_PRESSED(channel) || HOME_BUTTON_HELD(channel)) && !CDeviceVI::checkFlag0()){
                    if(HOME_BUTTON_HELD(channel)){
                        unk265 = true;
                    }

                    CDeviceFileCri* fileCri2 = CDeviceFileCri::getInstance();
                    bool criException2 = fileCri2->isException();

                    if(criException2 || sHbmStopFlag){
                        if(mState == STATE_NEG1){
                            setState(STATE_0);
                        }
                    }else{
                        CLibHbmControl::create();
                    }
                }
            }
        }
    }
}


void CLibHbm::initHbmInfoStruct(){
    std::memset(&spInstance->unk1EC, 0, sizeof(HBMDataInfo));
    spInstance->unk1EC.region = CDeviceSC::getLanguage();
    spInstance->unk1EC.layoutBuf = spInstance->mpLayoutBuf;
    spInstance->unk1EC.spkSeBuf = spInstance->mpSpkSeBuf;
    spInstance->unk1EC.msgBuf = spInstance->mpMsgBuf;
    spInstance->unk1EC.configBuf = spInstance->mpConfigBuf;
    spInstance->unk1EC.configBufSize = spInstance->mConfigBufSize;

    spInstance->unk1EC.sound_callback = nullptr;
    spInstance->unk1EC.backFlag = false;
    spInstance->unk1EC.cursor = false;

    spInstance->unk1EC.adjust.x = CDeviceVI::isWideAspectRatio() ? 1.3684211f : 1;
    spInstance->unk1EC.adjust.y = 1;
    spInstance->unk1EC.frameDelta = CDeviceVI::getVisPerFrame() * (CDeviceVI::isTvFormatPal() ? 60.0f/50.0f : 1);

    spInstance->unk1EC.mem = spInstance->mpHbmMem;
    spInstance->unk1EC.memSize = HBM_MEM_SIZE;
    spInstance->unk1EC.pAllocator = nullptr;
    spInstance->unk1EC.messageFlag = false;
}

void CLibHbm::initHbm(){
    //Initialize the HBM info struct
    initHbmInfoStruct();

    HBMCreate(&spInstance->unk1EC);
    HBMCreateSound(spInstance->mpHbmSeBuf, spInstance->mpHbmSndMem, HBM_SND_MEM_SIZE);
    HBMInit();
    HBMSetAdjustFlag(true);

    spInstance->mFlags |= 0x80;

    //TODO: probably fakematch
    for(u32 i = 0; (int)i < spInstance->unk238.size(); i++){
        spInstance->unk238[i]->onInitHbm();
    }
}

void CLibHbm::deleteHbm(){
    if(spInstance->mFlags & 0x80){
        HBMDeleteSound();
        HBMDelete();
    }

    spInstance->destroy();

    //TODO: probably fakematch
    for(u32 i = 0; (int)i < spInstance->unk238.size(); i++){
        spInstance->unk238[i]->onDeleteHbm();
    }
}

bool CLibHbm::isHbmControlInitialized(){
    return CLibHbmControl::getInstance() != nullptr;
}

bool CLibHbm::isHbmActive(){
    return CLibHbmControl::isActive();
}

bool CLibHbm::wkStandbyLogin(){
    if(CDeviceGX::isInitialized()){
        unk1C8++;
        if(unk1C8 <= 1) return false;
        return CWorkThread::wkStandbyLogin();
    }

    return false;
}

bool CLibHbm::wkStandbyLogout(){
    if(mChildren.empty() && CProcRoot::getInstance() == nullptr){
        CLibHbm::destroy();
        return CWorkThread::wkStandbyLogout();
    }

    return false;
}

bool CLibHbm::OnFileEvent(CEventFile* pFile){

    if(mpHbmArcFileHandle == pFile->mFileHandle){
        if(pFile->unk0 == true){
            unk1E8 = static_cast<UNKTYPE*>(pFile->getFileDataPtr());
            ARCHandle arcHandle;
            ARCFileInfo fileInfo;

            if(ARCInitHandle(unk1E8, &arcHandle)){
                if(ARCOpen(&arcHandle, "hbm/homeBtn.arc", &fileInfo)){
                    mpLayoutBuf = ARCGetStartAddrInMem(&fileInfo);
                }

                if(ARCOpen(&arcHandle, "hbm/SpeakerSe.arc", &fileInfo)){
                    mpSpkSeBuf = ARCGetStartAddrInMem(&fileInfo);
                }

                if(ARCOpen(&arcHandle, "hbm/HomeButtonSe.arc", &fileInfo)){
                    mpHbmSeBuf = ARCGetStartAddrInMem(&fileInfo);
                }

                if(ARCOpen(&arcHandle, "hbm/home.csv", &fileInfo)){
                    mpMsgBuf = ARCGetStartAddrInMem(&fileInfo);
                }

                if(ARCOpen(&arcHandle, "hbm/config.txt", &fileInfo)){
                    mpConfigBuf = ARCGetStartAddrInMem(&fileInfo);
                    mConfigBufSize = ARCGetLength(&fileInfo);
                }
            }
        }else{
            mFlags |= 0x40;

            if(CLibHbmControl::getInstance() != nullptr){
                if(!CLibHbmControl::getInstance()->isNoEvent()){
                    CLibHbmControl::getInstance()->wkSetEvent(EVT_NONE);
                }
            }
        }

        mpHbmArcFileHandle = nullptr;
    } else return false;

    if(mpHbmMem == nullptr && mpLayoutBuf != nullptr && mpSpkSeBuf != nullptr && mpHbmSeBuf != nullptr && mpMsgBuf != nullptr
    && mpConfigBuf != nullptr){
        mpHbmMem = mtl::MemManager::allocate_head(mHandle, HBM_MEM_SIZE, 32);
        mpHbmSndMem = mtl::MemManager::allocate_head(mHandle, HBM_SND_MEM_SIZE, 32);
    }

    return true;
}

//Doesn't work as an inline :/
#define ADD_CENTERED_TEX(draw, x, y, width, height, texWidth, texHeight) \
draw.add((CDeviceVI::getRenderModeObj()->fbWidth - width + x*2)/2, (CDeviceVI::getRenderModeObj()->efbHeight - height + y*2)/2, 0, 0);                \
draw.add((CDeviceVI::getRenderModeObj()->fbWidth + width + x*2)/2, (CDeviceVI::getRenderModeObj()->efbHeight - height + y*2)/2, texWidth,0);          \
draw.add((CDeviceVI::getRenderModeObj()->fbWidth - width + x*2)/2, (CDeviceVI::getRenderModeObj()->efbHeight + height + y*2)/2, 0, texHeight);        \
draw.add((CDeviceVI::getRenderModeObj()->fbWidth + width + x*2)/2, (CDeviceVI::getRenderModeObj()->efbHeight + height + y*2)/2, texWidth, texHeight); \

void CLibHbm::renderHbmstopIcon(){
    if(spInstance == nullptr) return;

    if(spInstance->mState < STATE_0) return;

    switch(spInstance->mState){
        case STATE_0:
            spInstance->unk25C = 0;
            spInstance->mState++;
            //Fallthrough
        case STATE_1:
            spInstance->unk25C += CDeviceVI::getSecPerFrame();
            if(spInstance->unk25C >= 0.25f){
                spInstance->unk25C = 0.25f;
                spInstance->mState++;
            }
            break;
        case STATE_2:
            spInstance->unk25C += CDeviceVI::getSecPerFrame();
            if(spInstance->unk25C >= 1.25f){
                spInstance->unk25C = 0.25f;
                spInstance->setState(3);
            }
            break;
        case STATE_3:
            spInstance->unk25C -= CDeviceVI::getSecPerFrame();
            if(spInstance->unk25C <= 0){
                spInstance->unk25C = 0;
                spInstance->mState = -1;
                return;
            }
            break;
        default:
            break;
    }

    CDeviceGX::getCacheInstance()->resetGXStateA();
    //Cast to a custom struct for easier use
    ml::CTPLData* tplData = reinterpret_cast<ml::CTPLData*>(spHbmstopTplData); //r31
    CDrawGX draw;
    draw.setZCompare(0);
    draw.setZWriteEnable(0);

    float smth = spInstance->unk25C * 4;

    if(smth > 1){
        smth = 1;
    }

    draw.setCol(ml::CCol4(1, 1, 1, smth));
    draw.setFlag4();
    draw.setTex(&sTplTexObj, tplData->imageHeader.width, tplData->imageHeader.height);

    draw.begin(PRIM_TRIANGLESTRIP, 4);

    float scale;

    if (CDeviceVI::isWideAspectRatio()) {
        scale = (tplData->imageHeader.width/CDeviceVI::getWidthScale() * CDeviceVI::VI_WIDTH_16_9) / CDeviceVI::SCREEN_WIDTH;
    }else{
        scale = (tplData->imageHeader.width * CDeviceVI::VI_WIDTH_16_9) / CDeviceVI::SCREEN_WIDTH;
    }

    int width = scale;

    ADD_CENTERED_TEX(draw, 0, -110, width, tplData->imageHeader.height, tplData->imageHeader.width, tplData->imageHeader.height);

    draw.end();

    CDeviceGX::getCacheInstance()->resetGXStateA();
    CViewRoot::updateViewRoot();
}

// [.data] 0x8056D260 (0x2C): CLibHbmControl::wkUpdate state-dispatch table.
// Foreign switch-table target: CLibHbmControl::wkUpdate body.
// Global-scope variable names are not mangled by MWCC, so no extern "C" needed.
extern char wkUpdate__14CLibHbmControlFv[];
extern "C" u32 jumptable_eu_8056D260[11] = {
    (u32)&wkUpdate__14CLibHbmControlFv[0x270],
    (u32)&wkUpdate__14CLibHbmControlFv[0x224],
    (u32)&wkUpdate__14CLibHbmControlFv[0x22C],
    (u32)&wkUpdate__14CLibHbmControlFv[0x270],
    (u32)&wkUpdate__14CLibHbmControlFv[0x234],
    (u32)&wkUpdate__14CLibHbmControlFv[0x244],
    (u32)&wkUpdate__14CLibHbmControlFv[0x250],
    (u32)&wkUpdate__14CLibHbmControlFv[0x270],
    (u32)&wkUpdate__14CLibHbmControlFv[0x23C],
    (u32)&wkUpdate__14CLibHbmControlFv[0x25C],
    (u32)&wkUpdate__14CLibHbmControlFv[0x268],
};
