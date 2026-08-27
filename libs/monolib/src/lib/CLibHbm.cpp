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
// CLibG3d.o's sdata; spHbmstopTplData/lbl_80667FDC/lbl_80667FDD are this
// TU's own sbss words (defined below).
#define spInstance lbl_eu_806656F8
extern "C" CLibHbm* lbl_eu_806656F8;
#define sCurWpadChannel lbl_eu_806637C0
extern "C" int lbl_eu_806637C0;
#define lbl_80667FD4 lbl_eu_806656FC
extern "C" bool lbl_eu_806656FC;
#define spHbmstopTplData lbl_eu_80665700
#define lbl_80667FDC lbl_eu_80665704
#define lbl_80667FDD lbl_eu_80665705
#define sTplTexObj (*(GXTexObj*)&lbl_eu_80657F90)
extern "C" char lbl_eu_80657F90[0x20];

// [.sbss] 0x80665700-0x80665710 zero-fill owned by this TU (size/align are
// compared, not bytes): lbl_eu_80665700 = spHbmstopTplData,
// lbl_eu_80665704/05 = lbl_80667FDC/lbl_80667FDD, and lbl_eu_80665708 is the
// CLibHbmControl singleton-pointer storage (its TU declares it extern).
TPLPalette* lbl_eu_80665700;
bool lbl_eu_80665704;
bool lbl_eu_80665705;
u32 lbl_eu_80665708[2];

// ===== Dissolved monolibdata2 (.rodata) owned by this TU =====
// The five hbm filenames packed into one retail rodata blob
// (0x80523138-0x80523190, 88B). OnFileEvent indexes it at 0x00/0x10/0x22/
// 0x37/0x44; this is a plain byte blob (no relocs).
extern "C" const char lbl_eu_80523138[88] = {
    0x68,0x62,0x6D,0x2F,0x68,0x6F,0x6D,0x65,0x42,0x74,0x6E,0x2E,0x61,0x72,0x63,0x00, // hbm/homeBtn.arc
    0x68,0x62,0x6D,0x2F,0x53,0x70,0x65,0x61,0x6B,0x65,0x72,0x53,0x65,0x2E,0x61,0x72, // hbm/SpeakerSe.ar
    0x63,0x00,0x68,0x62,0x6D,0x2F,0x48,0x6F,0x6D,0x65,0x42,0x75,0x74,0x74,0x6F,0x6E, // c.hbm/HomeButton
    0x53,0x65,0x2E,0x61,0x72,0x63,0x00,0x68,0x62,0x6D,0x2F,0x68,0x6F,0x6D,0x65,0x2E, // Se.arc.hbm/home.
    0x63,0x73,0x76,0x00,0x68,0x62,0x6D,0x2F,0x63,0x6F,0x6E,0x66,0x69,0x67,0x2E,0x74, // csv.hbm/config.t
    0x78,0x74,0x00,0x00,0x00,0x00,0x00,0x00,                                          // xt. + pad
};

// Forward decl for the manual vtable block at the bottom (CLibG3d pattern).
extern "C" u32 lbl_eu_8056D1A8[];

// Member inits are body assignments (CLibLayout pattern): novtable installs
// the retail vtable by hand right after the base ctor call, and keeping the
// member stores in program order after it lets -O4,p schedule them like
// retail instead of sinking the vptr store below an init list.
CLibHbm::CLibHbm(const char* pName, CWorkThread* pParent) : CWorkThread(pName, pParent, MAX_CHILD){
    u32 zeroBits = 0;
    const u32 invalidHandle = (u32)mtl::INVALID_HANDLE;
    *(u32 volatile*)this = (u32)&lbl_eu_8056D1A8[0];
    mHandle = invalidHandle;
    unk1C8 = zeroBits;
    mpLayoutBuf = nullptr;
    mpSpkSeBuf = nullptr;
    mpHbmSeBuf = nullptr;
    mpMsgBuf = nullptr;
    mpConfigBuf = nullptr;
    mpHbmMem = nullptr;
    mpHbmSndMem = nullptr;
    unk1E8 = nullptr;
    mFlags = 0;
    mpHbmArcFileHandle = nullptr;
    mConfigBufSize = 0;
    unk238.mCount = 0;
    unk25C = 0.0f;
    mState = STATE_NEG1;
    unk264 = 0;
    unk265 = zeroBits;
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

void CLibHbm::setHbmStopFlag(bool r3){
    lbl_80667FDD = r3;
}

bool CLibHbm::isHbmStopPending(){
    return lbl_80667FDC;
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

void CLibHbm::addCallback(IHBMCallback* r3){
    CLibHbm* instance = spInstance;
    if(instance == nullptr) return;

    if(instance->unk238.mCount < 8){
        instance->unk238.push_back(r3);
    }
}

//TODO: same as the erase function in CErrorWii
void CLibHbm::removeCallback(IHBMCallback* r3){
    if(spInstance == nullptr) return;

    for(int i = 0; i < spInstance->unk238.size(); i++){
        if(spInstance->unk238.mArray[i] == r3){
            while(i < spInstance->unk238.mCount - 1){
                spInstance->unk238.mArray[i] = spInstance->unk238.mArray[i + 1];
                i++;
            }

            spInstance->unk238.mCount--;
            return;
        }
    }

}

void CLibHbm::setHbmActiveFlag(bool r3){
    lbl_80667FD4 = r3;
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
    // Reference the retail-named instance global (same storage as spInstance)
    // so the SDA21 relocs name-match.
    extern CLibHbm* lbl_eu_806656F8;
    // Filename globals owned by CLibG3d.cpp (.sdata pointers to "hbm.arc" strings).
    extern u32 lbl_eu_806637B8;
    extern u32 lbl_eu_806637BC;

    lbl_eu_806656F8->mFlags &= ~0x40;

    CDeviceVI::waitForDrawDone();
    updateScnCounter(1);
    lbl_eu_806656F8->mFlags |= 0x20;
    lbl_eu_806656F8->mHandle = getScnCounter();

    if(lbl_eu_806656F8->mHandle == mtl::INVALID_HANDLE){
        lbl_eu_806656F8->mHandle = mtl::MemManager::getHandleMEM2();
    }

    // Pick the archive path by console language (6 = Japanese).
    const char* pArcFile = (const char*)lbl_eu_806637B8;
    if(CDeviceSC::getLanguage() == 6){
        pArcFile = (const char*)lbl_eu_806637BC;
    }

    // Retail passes an extra ignored int arg (r4=1) to the 1-param
    // CDeviceFile::getFileSize; route through a cast function-pointer type.
    typedef int (*GetFileSizeFn)(const char*, int);
    if(((GetFileSizeFn)&CDeviceFile::getFileSize)(pArcFile, 1) > 0){
        CFileHandle* handle = CDeviceFile::readFile(lbl_eu_806656F8->mHandle, pArcFile,
        lbl_eu_806656F8, 0, 0);
        lbl_eu_806656F8->mpHbmArcFileHandle = handle;
        CDeviceFile::func_8044F154(lbl_eu_806656F8->mpHbmArcFileHandle, 0);
    }
}

// Inline copy of CWorkThread::isRunning() visible only in this TU (same trick
// as CLibVM.cpp / CDeviceGX.cpp). Retail also inlines CWorkThread's
// CMsgParam<8>::find(EVT_EXCEPTION) scan here (divwu modulo loop, leaf body),
// so the queue scan is hand-inlined over a layout mirror instead of calling
// the member (a member call adds a frame + bl). CWorkRoot.cpp keeps the
// strong out-of-line definition.
struct CLibHbmMsgQueueData {
    u8 pad[0x1A4];               // CWorkThread prefix + vtable + mEntries[8]
    void* mArrayPtr;             // 0x1A4 (mMsgQueue.mArray)
    u32 mFront;                  // 0x1A8
    u32 mCount;                  // 0x1AC
    u32 mCapacity;               // 0x1B0
};

struct CLibHbmMsgQueueEntry {
    u32 command;                 // EVT_* command word
    u8 pad[0x20];                // sizeof == 0x24 (retail scales index by 36)
};

inline bool CWorkThread::isRunning() const {
    bool busy;
    if (mFlags & THREAD_FLAG_EXCEPTION) {
        busy = true;
    } else {
        const CLibHbmMsgQueueData* q = reinterpret_cast<const CLibHbmMsgQueueData*>(this);
        int i;
        int foundIndex;
        for (i = 0; i < q->mCount; i++) {
            if (((const CLibHbmMsgQueueEntry*)q->mArrayPtr)[(q->mFront + i) % q->mCapacity].command
                == EVT_EXCEPTION) {
                foundIndex = i;
                goto done;
            }
        }
        foundIndex = -1;
    done:
        busy = foundIndex >= 0;
    }

    return !busy
        && (mState == THREAD_STATE_LOGIN || mState == THREAD_STATE_RUN);
}

bool CLibHbm::isInitialized(){
    // Reference the retail-named instance global (same storage as the spInstance
    // static member) so the SDA21 reloc name matches.
    extern CLibHbm* lbl_eu_806656F8;
    return lbl_eu_806656F8->isRunning();
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
            bool criException;
            if(fileCri->mFlags & THREAD_FLAG_EXCEPTION){
                criException = true;
            }else{
                const CLibHbmMsgQueueData* q = reinterpret_cast<const CLibHbmMsgQueueData*>(fileCri);
                int i;
                int foundIndex;
                for(i = 0; i < q->mCount; i++){
                    if(((const CLibHbmMsgQueueEntry*)q->mArrayPtr)[(q->mFront + i) % q->mCapacity].command
                        == EVT_EXCEPTION){
                        foundIndex = i;
                        goto doneA;
                    }
                }
                foundIndex = -1;
            doneA:
                criException = foundIndex >= 0;
            }

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

                if(!lbl_80667FDD){
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
                    bool criException2;
                    if(fileCri2->mFlags & THREAD_FLAG_EXCEPTION){
                        criException2 = true;
                    }else{
                        const CLibHbmMsgQueueData* q = reinterpret_cast<const CLibHbmMsgQueueData*>(fileCri2);
                        int i;
                        int foundIndex;
                        for(i = 0; i < q->mCount; i++){
                            if(((const CLibHbmMsgQueueEntry*)q->mArrayPtr)[(q->mFront + i) % q->mCapacity].command
                                == EVT_EXCEPTION){
                                foundIndex = i;
                                goto doneB;
                            }
                        }
                        foundIndex = -1;
                    doneB:
                        criException2 = foundIndex >= 0;
                    }

                    if(criException2 || lbl_80667FDD){
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
            void* data = pFile->getFileDataPtr();
            unk1E8 = data;
            ARCHandle arcHandle;
            ARCFileInfo fileInfo;

            if(ARCInitHandle(unk1E8, &arcHandle)){
                if(ARCOpen(&arcHandle, &lbl_eu_80523138[0x00], &fileInfo)){
                    mpLayoutBuf = ARCGetStartAddrInMem(&fileInfo);
                }

                if(ARCOpen(&arcHandle, &lbl_eu_80523138[0x10], &fileInfo)){
                    mpSpkSeBuf = ARCGetStartAddrInMem(&fileInfo);
                }

                if(ARCOpen(&arcHandle, &lbl_eu_80523138[0x22], &fileInfo)){
                    mpHbmSeBuf = ARCGetStartAddrInMem(&fileInfo);
                }

                if(ARCOpen(&arcHandle, &lbl_eu_80523138[0x37], &fileInfo)){
                    mpMsgBuf = ARCGetStartAddrInMem(&fileInfo);
                }

                if(ARCOpen(&arcHandle, &lbl_eu_80523138[0x44], &fileInfo)){
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

// ===== Dissolved monolibdata2 (.data/.sdata) owned by this TU =====
// Base class CWorkThread is novtable, so CLibHbm's vtable/RTTI are defined
// manually here (novtable suppresses MWCC's auto __vt__/__RTTI__ emission),
// mirroring retail exactly:
//   .data  lbl_eu_8056D1A8 __vt__7CLibHbm (0xA0)
//          lbl_eu_8056D248 RTTI base-list [IWorkEvent,0, CWorkThread,0, 0,0]
//          jumptable_eu_8056D260 CLibHbmControl::wkUpdate switch table (0x2C;
//          foreign text symbol; its switch compiles in CLibHbmControl.cpp but
//          the retail linker places the table in this unit's .data range)
//   .sdata lbl_eu_806637C8 RTTI locator { "CLibHbm" pool string, base-list }
//
// The IWorkEvent default-handler slots, CWorkThread work-method slots, and
// __RTTI__ symbols come from monolib/data_vtables.hpp (shared declarations for
// all manual-vtable TUs). Only this class's own overrides are declared here.
namespace HbmBlob {
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
extern "C" void wkRender__11CWorkThreadFv();
extern "C" void wkRenderAfter__11CWorkThreadFv();
extern "C" void wkStandbyExceptionRetry__11CWorkThreadFUl();
extern "C" u32 __RTTI__10IWorkEvent;
extern "C" u32 __RTTI__11CWorkThread;
// Own-class virtuals referenced by the manual vtable (retail mangled names;
// resolve to this TU's member definitions at link).
extern "C" void __dt__7CLibHbmFv();
extern "C" void OnFileEvent__7CLibHbmFP10CEventFile();
extern "C" void wkUpdate__7CLibHbmFv();
extern "C" void wkStandbyLogin__7CLibHbmFv();
extern "C" void wkStandbyLogout__7CLibHbmFv();
}
// Foreign switch-table target: CLibHbmControl::wkUpdate body.
// Global-scope variable names are not mangled by MWCC, so no extern "C" needed.
extern char wkUpdate__14CLibHbmControlFv[];
extern "C" char lbl_eu_8066A568[8];   // .sdata2 "CLibHbm" RTTI name (shared pool)
// [.data] 0x8056D1A8 (0xA0): __vt__7CLibHbm.
extern "C" u32 lbl_eu_806637C8[2];
extern "C" u32 lbl_eu_8056D1A8[40] = {
    (u32)&lbl_eu_806637C8, 0x00000000, (u32)&HbmBlob::__dt__7CLibHbmFv,
    (u32)&HbmBlob::WorkEvent1__10IWorkEventFPvPCc, (u32)&HbmBlob::OnFileEvent__7CLibHbmFP10CEventFile,
    (u32)&HbmBlob::WorkEvent3__10IWorkEventFPv, (u32)&HbmBlob::WorkEvent4__10IWorkEventFv,
    (u32)&HbmBlob::OnPauseTrigger__10IWorkEventFb,
    (u32)&HbmBlob::WorkEvent6__10IWorkEventFv, (u32)&HbmBlob::WorkEvent7__10IWorkEventFv,
    (u32)&HbmBlob::WorkEvent8__10IWorkEventFv, (u32)&HbmBlob::WorkEvent9__10IWorkEventFv,
    (u32)&HbmBlob::WorkEvent10__10IWorkEventFv, (u32)&HbmBlob::WorkEvent11__10IWorkEventFv,
    (u32)&HbmBlob::WorkEvent12__10IWorkEventFv, (u32)&HbmBlob::WorkEvent13__10IWorkEventFv,
    (u32)&HbmBlob::WorkEvent14__10IWorkEventFv, (u32)&HbmBlob::WorkEvent15__10IWorkEventFv,
    (u32)&HbmBlob::WorkEvent16__10IWorkEventFv, (u32)&HbmBlob::WorkEvent17__10IWorkEventFv,
    (u32)&HbmBlob::WorkEvent18__10IWorkEventFv, (u32)&HbmBlob::WorkEvent19__10IWorkEventFv,
    (u32)&HbmBlob::WorkEvent20__10IWorkEventFv, (u32)&HbmBlob::WorkEvent21__10IWorkEventFv,
    (u32)&HbmBlob::WorkEvent22__10IWorkEventFv, (u32)&HbmBlob::WorkEvent23__10IWorkEventFv,
    (u32)&HbmBlob::WorkEvent24__10IWorkEventFv, (u32)&HbmBlob::WorkEvent25__10IWorkEventFv,
    (u32)&HbmBlob::WorkEvent26__10IWorkEventFv, (u32)&HbmBlob::WorkEvent27__10IWorkEventFv,
    (u32)&HbmBlob::WorkEvent28__10IWorkEventFv, (u32)&HbmBlob::WorkEvent29__10IWorkEventFv,
    (u32)&HbmBlob::WorkEvent30__10IWorkEventFv, (u32)&HbmBlob::WorkEvent31__10IWorkEventFv,
    (u32)&HbmBlob::wkUpdate__7CLibHbmFv, (u32)&HbmBlob::wkRender__11CWorkThreadFv,
    (u32)&HbmBlob::wkRenderAfter__11CWorkThreadFv,
    (u32)&HbmBlob::wkStandbyLogin__7CLibHbmFv, (u32)&HbmBlob::wkStandbyLogout__7CLibHbmFv,
    (u32)&HbmBlob::wkStandbyExceptionRetry__11CWorkThreadFUl,
};

// [.data] 0x8056D248 (0x18): RTTI base list [IWorkEvent,0, CWorkThread,0, 0,0].
extern "C" u32 lbl_eu_8056D248[6] = {
    (u32)&HbmBlob::__RTTI__10IWorkEvent, 0x00000000,
    (u32)&HbmBlob::__RTTI__11CWorkThread, 0x00000000,
    0x00000000, 0x00000000,
};

// [.sdata] 0x806637C8 (0x8): RTTI locator { class-name string, base-list }.
extern "C" u32 lbl_eu_806637C8[2] = { (u32)&lbl_eu_8066A568, (u32)&lbl_eu_8056D248 };

// [.data] 0x8056D260 (0x2C): CLibHbmControl::wkUpdate state-dispatch table.
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

// NOTE: no DECOMP_FORCEACTIVE here -- the fake emitter would add ~0x20 bytes
// to .text and bust the split budget (now exactly at retail size). MWCC keeps
// externally-visible data definitions without a forced reference.
