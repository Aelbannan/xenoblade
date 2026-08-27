#include <homebuttonLib.h>
#include <HBMRes.h>

#include <revolution/hbm/HBMBase.hpp>

#include <nw4hbm/lyt.h>
#include <nw4hbm/ut.h>

#include <revolution/AX.h>
#include <revolution/AXFX.h>
#include <revolution/GX.h>
#include <revolution/OS.h>
#include <revolution/PAD.h>
#include <revolution/VI.h>
#include <revolution/WPAD.h>
#include <revolution/version.h>

#include <cstring>

/* First .rodata object, exactly like the retail pool (GXColor{0,0,0,255}
 * plus a 0.0f word, one 8-byte object): naming it gives MWCC the retail base
 * symbol (lbl_805186C8) for the r27 constant-pool base instead of a section
 * reloc. */
extern "C" const u32 lbl_805186C8[2] = {0x000000FF, 0x00000000};

/* Retail names the pooled "bar_00" pane string lbl_eu_80549D1C (symbols.txt
 * .data:0x80549D1C); update_controller's FindPaneByName("bar_00") emits a
 * reloc against it. Pin the name via an undefined extern declaration (resolved
 * at link time from symbols.txt) so the reloc matches without adding a .data
 * object that would shift the TU string pool. */
namespace homebutton {
void AxSoundMain(); // defined in HBMAxSound.cpp
void SetSoundMode(u32 mode); // defined in HBMAxSound.cpp
void PlaySeq(int seqId); // defined in HBMAxSound.cpp
void StopAllSeq(); // defined in HBMAxSound.cpp
void SetVolumeAllSeq(float volume); // defined in HBMAxSound.cpp
}

/******************************************************************************
 *
 * Implementation details
 *
 ******************************************************************************/

/* sAllocator/spAllocator are defined in HBMCommon.cpp (retail TU boundary);
 * this unit only references spAllocator (see create's inlined PaneManager
 * ctor). HBMAllocMem/HBMFreeMem are also defined in HBMCommon.cpp. */
extern MEMAllocator* spAllocator;
extern void* HBMAllocMem(u32 size);
extern void HBMFreeMem(void* pBlock);

/* Layout view for free-function private-member access (retail offsets:
   mSpeakerAlarm at 0x4C8). */
struct HBMBaseAlarmView {
    u8 _pad0[0x4C8];
    OSAlarm mSpeakerAlarm[WPAD_MAX_CONTROLLERS];
};

/* Retail keeps the Wiimote battery-info array as a static bss object
 * (sWpadInfo__Q22cf9CfPadTask__Q210homebutton10HomeButton, 0x60 bytes) rather
 * than an instance member; calc_battery/update/init_battery all reference this
 * static. The two-Q mangled name cannot be produced by a normal C++ member
 * declaration (cf::CfPadTask is not nested in HomeButton), so the exact retail
 * linker name is pinned via an extern "C" definition. The HomeButton layout
 * member at 0x14C is retained solely to preserve the retail object layout. */
extern "C" WPADInfo
    sWpadInfo__Q22cf9CfPadTask__Q210homebutton10HomeButton[WPAD_MAX_CONTROLLERS] =
        {};

namespace homebutton {

// Prototypes
void SimpleSyncCallback(s32 result, s32 num);
static void initgx();
static void drawBlackPlate(f32 left, f32 top, f32 right, f32 bottom,
                           GXColor clr);

struct AnmControllerTable {
    int pane; // at 0x0
    int anm;  // at 0x4
};

#define X(Y, Z) {res::eGroup_##Y, res::eAnim_##Z},
static const AnmControllerTable scAnmTable[] = {HBM_ANIM_TABLE};
#undef X

#define X(Y, Z) {res::eGrPane_##Y, res::eGrAnim_##Z},
static const AnmControllerTable scGroupAnmTable[] = {HBM_GROUP_ANIM_TABLE};
#undef X

/* Retail keeps all of these helpers inlined (no out-of-line symbols exist in
 * the retail HBMBase object); `inline` forces MWCC to inline them here and
 * suppresses the standalone copies. */
inline void HomeButton::play_sound(int id) {
    int ret = 0;

    if (mpHBInfo->sound_callback != NULL) {
        ret = mpHBInfo->sound_callback(HBM_SOUND_PLAY, id);
    }

    if (ret == 0) {
        PlaySeq(id);
    }
}

inline int HomeButton::findAnimator(int pane, int anm) {
    for (int i = 0; i < mAnmNum; i++) {
        if (scAnmTable[i].pane == pane && scAnmTable[i].anm == anm) {
            return i;
        }
    }

    return -1;
}

inline int HomeButton::findGroupAnimator(int grPane, int grAnm) {
    for (int i = 0; i < res::eGrAnimator_Max; i++) {
        if (scGroupAnmTable[i].pane == grPane &&
            scGroupAnmTable[i].anm == grAnm) {

            return i;
        }
    }

    return -1;
}

inline void HomeButton::BlackFader::init(int maxFrame) {
    frame_ = 0;
    maxFrame_ = maxFrame;
    state_ = 0;
}

inline bool HomeButton::isActive() const {
    return mState == 2;
}

HomeButton* HomeButton::spHomeButtonObj = NULL;
OSMutex HomeButton::sMutex;

const int HomeButton::scReConnectTime = 3600;
const int HomeButton::scReConnectTime2 = 3570;
const int HomeButton::scPadDrawWaitTime = 5;
const int HomeButton::scGetPadInfoTime = 100;
const int HomeButton::scForcusSEWaitTime = 2;
const f32 HomeButton::scOnPaneVibTime = 3.0f;
const f32 HomeButton::scOnPaneVibWaitTime = 9.0f;
const int HomeButton::scWaitStopMotorTime = 30;
const int HomeButton::scWaitDisConnectTime = 180;

#define X(Y) #Y ".brlyt",
const char* HomeButton::scCursorLytName[res::eCursorLyt_Max] = {
    HBM_CURSOR_LYT_NAME_LIST //
};
#undef X

const char* HomeButton::scCursorPaneName = HBM_CURSOR_PANE_NAME;
const char* HomeButton::scCursorRotPaneName = HBM_CURSOR_ROT_PANE_NAME;
const char* HomeButton::scCursorSRotPaneName = HBM_CURSOR_SROT_PANE_NAME;

#define X(Y) #Y,
/* Retail only emits three btn/txt strings (B_btnL_00/01/10,
 * T_btnL_00/01/10); the fourth array entry stays NULL. */
const char* HomeButton::scBtnName[res::eBtn_Max] = {"B_btnL_00", "B_btnL_01",
                                                    "B_btnL_10", NULL};
#undef X

#define X(Y) #Y,
const char* HomeButton::scTxtName[res::eTxt_Max] = {"T_btnL_00", "T_btnL_01",
                                                    "T_btnL_10", NULL};
#undef X

#define X(Y) #Y,
const char* HomeButton::scGrName[res::eGroup_Max] = {HBM_GROUP_LIST};
#undef X

#define X(Y) "_" #Y ".brlan",
const char* HomeButton::scAnimName[res::eAnim_Max] = {HBM_ANIM_NAME_LIST};
#undef X

#define X(Y) "_" #Y ".brlan",
const char* HomeButton::scPairGroupAnimName[res::ePairAnm_Max] = {
    HBM_PAIR_ANM_NAME_LIST};
#undef X

#define X(Y) #Y,
const char* HomeButton::scPairGroupName[res::ePairAnm_Max] = {
    HBM_PAIR_ANM_NAME_LIST};
#undef X

#define X(Y) "_" #Y ".brlan",
const char* HomeButton::scGroupAnimName[res::eGrAnim_Max] = {
    HBM_GROUP_ANIM_NAME_LIST};
#undef X

#define X(Y) #Y,
const char* HomeButton::scGroupName[res::eGrPane_Max] = {
    HBM_GROUP_PANE_NAME_LIST};
#undef X

#define X(Y) #Y,
const char* HomeButton::scFuncPaneName[res::eFuncPane_Max] = {
    HBM_FUNC_PANE_NAME_LIST};
#undef X

#define X(Y) #Y,
const char* HomeButton::scFuncTouchPaneName[res::eFuncTouchPane_Max] = {
    HBM_FUNC_TOUCH_PANE_NAME_LIST};
#undef X

#define X(Y) #Y,
const char* HomeButton::scFuncTextPaneName[res::eFuncTextPane_Max] = {
    HBM_FUNC_TEXT_PANE_NAME_LIST};
#undef X

#define X(Y) #Y,
const char* HomeButton::scBatteryPaneName //
    [WPAD_MAX_CONTROLLERS][res::eBatteryPane_Max] = {
        HBM_BATTERY_PANE_NAME_LIST(00_) //
        HBM_BATTERY_PANE_NAME_LIST(01_) //
        HBM_BATTERY_PANE_NAME_LIST(02_) //
        HBM_BATTERY_PANE_NAME_LIST(03_) //
};
#undef X

/* Minimal class with the vtable shape the HomeButton destructor calls through
 * for the lyt/GroupAnmController objects (deleting-destructor virtual at
 * vtbl[2]).  Casting to this local type keeps the retail byte-identical
 * virtual dispatch while avoiding standalone emission of the SDK classes'
 * inline virtuals. */
struct VtblObj {
    virtual ~VtblObj();
    virtual void v1();
    virtual void v2();
};

HomeButton::HomeButton(const HBMDataInfo* pDataInfo)
    : mpHBInfo(pDataInfo), unk08(NULL), mpLayout(NULL), mpPaneManager(NULL),
      mFader(30) {

    unk5B8 = 1;
    mState = 2;
    mSelectBtnNum = HBM_SELECT_NULL;
    mSelectAnmNum = -1;
    mMsgCount = 0;
    mSequence = eSeq_Normal;
    mForcusSEWaitTime = 0;
    mLetterFlag = false;
    mBar0AnmRev = 0;
    mBar1AnmRev = 0;
    mBar0AnmRevHold = 0;
    mBar1AnmRevHold = 0;
    mAdjustFlag = false;
    mReassignedFlag = false;
    mEndInitSoundFlag = false;

    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        OSCreateAlarm(&mAlarm[i]);
        OSCreateAlarm(&mSpeakerAlarm[i]);
    }

    OSCreateAlarm(&mSimpleSyncAlarm);
    OSInitMutex(&sMutex);

    /*
    mpSoundArchivePlayer = NULL;
    mpDvdSoundArchive = NULL;
    mpMemorySoundArchive = NULL;
    mpNandSoundArchive = NULL;
    mpSoundHeap = NULL;
    mpSoundHandle = NULL;
    */
}

HomeButton::~HomeButton() {
    int i;

    ((VtblObj*)mpResAccessor)->~VtblObj();
    HBMFreeMem(mpResAccessor);

    ((VtblObj*)mpLayout)->~VtblObj();
    HBMFreeMem(mpLayout);

    if (mpHBInfo->cursor == 0) {
        for (i = 0; i < res::eCursorLyt_Max; i++) {
            ((VtblObj*)mpCursorLayout[i])->~VtblObj();
            HBMFreeMem(mpCursorLayout[i]);
        }
    }

    for (i = 0; i < mAnmNum; i++) {
        ((VtblObj*)mpAnmController[i])->~VtblObj();
        HBMFreeMem(mpAnmController[i]);
    }

    for (i = 0; i < res::ePairAnm_Max; i++) {
        ((VtblObj*)mpPairGroupAnmController[i])->~VtblObj();
        HBMFreeMem(mpPairGroupAnmController[i]);
    }

    for (i = 0; i < res::eGrAnimator_Max; i++) {
        ((VtblObj*)mpGroupAnmController[i])->~VtblObj();
        HBMFreeMem(mpGroupAnmController[i]);
    }

    HBMFreeMem(mpHomeButtonEventHandler);

    ((gui::Manager*)mpPaneManager)->~Manager();
    HBMFreeMem(mpPaneManager);

    for (i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        if (mInitFlag) {
            mpController[i]->clearCallback();
        }
        mpController[i]->~Controller();
        HBMFreeMem(mpController[i]);
    }

        mpRemoteSpk->~RemoteSpk();
    HBMFreeMem(mpRemoteSpk);
    mpRemoteSpk = NULL;

    WPADSetSimpleSyncCallback(mSimpleSyncCallback);

    HBMFreeMem(mpLayoutName);
    HBMFreeMem(mpAnmName);

    for (i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        OSCancelAlarm(&mAlarm[i]);
        OSCancelAlarm(&mSpeakerAlarm[i]);
    }

    OSCancelAlarm(&mSimpleSyncAlarm);
}


void HomeButton::createInstance(const HBMDataInfo* pDataInfo) {
    if (void* pMem = HBMAllocMem(sizeof(HomeButton))) {
        spHomeButtonObj = new (pMem) HomeButton(pDataInfo);
    }
}

void HomeButton::deleteInstance() {
    spHomeButtonObj->~HomeButton();
    HBMFreeMem(spHomeButtonObj);
    spHomeButtonObj = NULL;
}

HomeButton* HomeButton::getInstance() {
    return spHomeButtonObj;
}

void HomeButton::create() {
    int i;
    char anmNameBuf[64];

    mInitFlag = false;
    mForceSttInitProcFlag = false;
    mForceSttFadeInProcFlag = false;

    set_config();
    set_text();

    if (void* pMem = HBMAllocMem(sizeof(nw4hbm::lyt::ArcResourceAccessor))) {
        mpResAccessor = new (pMem) nw4hbm::lyt::ArcResourceAccessor();
    }

    mpResAccessor->Attach(mpHBInfo->layoutBuf, "arc");

    if (!mpHBInfo->cursor) {
        for (i = 0; i < res::eCursorLyt_Max; i++) {
            if (void* pMem = HBMAllocMem(sizeof(nw4hbm::lyt::Layout))) {
                mpCursorLayout[i] = new (pMem) nw4hbm::lyt::Layout();
            }

            void* pBinary = mpResAccessor->GetResource(
                nw4hbm::lyt::ArcResourceAccessor::RES_TYPE_NAME,
                scCursorLytName[i], NULL);

            mpCursorLayout[i]->Build(pBinary, mpResAccessor);
        }
    }

    if (void* pMem = HBMAllocMem(sizeof(nw4hbm::lyt::Layout))) {
        mpLayout = new (pMem) nw4hbm::lyt::Layout();
    }

    {
        void* pBinary = mpResAccessor->GetResource(
            nw4hbm::lyt::ArcResourceAccessor::RES_TYPE_NAME, mpLayoutName, NULL);

        mpLayout->Build(pBinary, mpResAccessor);
    }

    nw4hbm::lyt::Pane* pRootPane = mpLayout->GetRootPane();

    NW4R_UT_LINKLIST_FOREACH(it, pRootPane->GetChildList(), {
        if (std::strcmp(it->GetName(), "back_00") == 0 ||
            std::strcmp(it->GetName(), "back_01") == 0 ||
            std::strcmp(it->GetName(), "back_02") == 0) {

            it->SetScale(nw4hbm::math::VEC2(1.5f, 1.5f));
        }
    })

    mpLayout->GetRootPane()
        ->FindPaneByName("bar_line_00", true)
        ->SetScale(nw4hbm::math::VEC2(1.5f, 1.0f));
    mpLayout->GetRootPane()
        ->FindPaneByName("bar_line_10", true)
        ->SetScale(nw4hbm::math::VEC2(1.5f, 1.0f));

    for (i = 0; i < mAnmNum; i++) {
        std::strcpy(anmNameBuf, mpAnmName);
        std::strcat(anmNameBuf, scAnimName[scAnmTable[i].anm]);

        void* pBinary = mpResAccessor->GetResource(
            nw4hbm::lyt::ArcResourceAccessor::RES_TYPE_NAME, anmNameBuf, NULL);

        if (void* pMem = HBMAllocMem(sizeof(GroupAnmController))) {
            mpAnmController[i] = new (pMem) GroupAnmController();
        }

        mpAnmController[i]->mpAnimGroup =
            mpLayout->CreateAnimTransform(pBinary, mpResAccessor);

        mpAnmController[i]->mpGroup =
            mpLayout->GetGroupContainer()->FindGroupByName(
                scGrName[scAnmTable[i].pane]);

        nw4hbm::lyt::detail::PaneLinkList& rList =
            mpAnmController[i]->mpGroup->GetPaneList();

        NW4R_UT_LINKLIST_FOREACH(it, rList, {
            it->mTarget->BindAnimation(mpAnmController[i]->mpAnimGroup, false);
        })

        mpAnmController[i]->init(FrameController::eAnmType_Forward,
                                 mpAnmController[i]->mpAnimGroup->GetFrameMax(),
                                 0.0f, mpHBInfo->frameDelta);
    }

    for (i = 0; i < res::eGrAnimator_Max; i++) {
        std::strcpy(anmNameBuf, mpAnmName);
        std::strcat(anmNameBuf, scGroupAnimName[scGroupAnmTable[i].anm]);

        void* pBinary = mpResAccessor->GetResource(
            nw4hbm::lyt::ArcResourceAccessor::RES_TYPE_NAME, anmNameBuf, NULL);

        if (void* pMem = HBMAllocMem(sizeof(GroupAnmController))) {
            mpGroupAnmController[i] = new (pMem) GroupAnmController();
        }

        mpGroupAnmController[i]->mpAnimGroup =
            mpLayout->CreateAnimTransform(pBinary, mpResAccessor);

        mpGroupAnmController[i]->mpGroup =
            mpLayout->GetGroupContainer()->FindGroupByName(
                scGroupName[scGroupAnmTable[i].pane]);

        nw4hbm::lyt::detail::PaneLinkList& rList =
            mpGroupAnmController[i]->mpGroup->GetPaneList();

        NW4R_UT_LINKLIST_FOREACH(it, rList, {
             it->mTarget->BindAnimation(mpGroupAnmController[i]->mpAnimGroup,
                                       false);
        })

        mpGroupAnmController[i]->init(
            FrameController::eAnmType_Forward,
            mpGroupAnmController[i]->mpAnimGroup->GetFrameMax(), 0.0f,
            mpHBInfo->frameDelta);
    }

    for (i = 0; i < res::ePairAnm_Max; i++) {
        std::strcpy(anmNameBuf, mpAnmName);
        std::strcat(anmNameBuf, scPairGroupAnimName[i]);

        void* pBinary = mpResAccessor->GetResource(
            nw4hbm::lyt::ArcResourceAccessor::RES_TYPE_NAME, anmNameBuf, NULL);

        if (void* pMem = HBMAllocMem(sizeof(GroupAnmController))) {
            mpPairGroupAnmController[i] = new (pMem) GroupAnmController();
        }

        mpPairGroupAnmController[i]->mpAnimGroup =
            mpLayout->CreateAnimTransform(pBinary, mpResAccessor);

        mpPairGroupAnmController[i]->mpGroup =
            mpLayout->GetGroupContainer()->FindGroupByName(scPairGroupName[i]);

        nw4hbm::lyt::detail::PaneLinkList& rList =
            mpPairGroupAnmController[i]->mpGroup->GetPaneList();

        NW4R_UT_LINKLIST_FOREACH(it, rList, {
            it->mTarget->BindAnimation(mpPairGroupAnmController[i]->mpAnimGroup,
                false);
        })

        mpPairGroupAnmController[i]->init(
            FrameController::eAnmType_Forward,
            mpPairGroupAnmController[i]->mpAnimGroup->GetFrameMax(), 0.0f,
            mpHBInfo->frameDelta);
    }

    if (void* pMem = HBMAllocMem(sizeof(HomeButtonEventHandler))) {
        mpHomeButtonEventHandler = new (pMem) HomeButtonEventHandler(this);
    }

    if (void* pMem = HBMAllocMem(sizeof(gui::PaneManager))) {
        mpPaneManager = new (pMem)
            gui::PaneManager(mpHomeButtonEventHandler, NULL, spAllocator);
    }

    mpPaneManager->createLayoutScene(*mpLayout);

    if (void* pMem = HBMAllocMem(sizeof(RemoteSpk))) {
        mpRemoteSpk = new (pMem) RemoteSpk(mpHBInfo->spkSeBuf);
    }

    for (i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        if (void* pMem = HBMAllocMem(sizeof(Controller))) {
            mpController[i] = new (pMem) Controller(i, mpRemoteSpk);
        }
    }

    mpPaneManager->setDrawInfo(&mDrawInfo);

    nw4hbm::math::VEC2 adjScale(1.0f / mpHBInfo->adjust.x, 1.0f);
    mDrawInfo.SetLocationAdjustScale(adjScale);
    mDrawInfo.SetLocationAdjust(mAdjustFlag);

    nw4hbm::math::MTX34 viewMtx;
    nw4hbm::math::MTX34Identity(&viewMtx);
    mDrawInfo.SetViewMtx(viewMtx);

    init_msg();
}

static u32 get_comma_length(char* pBuf) {
    u32 len;

    for (len = 0; pBuf[len] != '\0'; len++) {
        if (pBuf[len] == ',') {
            break;
        }
    }

    return len;
}

void HomeButton::set_config() {
    int i = 0, j = 0;

    char* pConfig = static_cast<char*>(mpHBInfo->configBuf);
    char* pEnd =
        static_cast<char*>(mpHBInfo->configBuf) + mpHBInfo->configBufSize;

    u32 len = get_comma_length(pConfig);
    mpLayoutName = static_cast<char*>(HBMAllocMem(len + 1));

    std::strncpy(mpLayoutName, pConfig, len);
    mpLayoutName[len] = '\0';

    pConfig += len + 1;

    len = get_comma_length(pConfig);
    mpAnmName = static_cast<char*>(HBMAllocMem(len + 1));

    std::strncpy(mpAnmName, pConfig, len);
    mpAnmName[len] = '\0';

    pConfig += len;

    i = 0;
    j = 0;
    for (; pConfig[i] != '\0' && pConfig + i < pEnd; i++) {
        if (pConfig[i] == ',') {
            if (pConfig[i + 1] == '1') {
                mDialogFlag[j] = true;
            } else {
                mDialogFlag[j] = false;
            }

            j++;
        }
    }

    mButtonNum = j;
    mAnmNum = mButtonNum * res::eAnim_Max;
}

void HomeButton::set_text() {
    int i = 0, j = 0, k = 0;
    bool flag = false;
    wchar_t* message = static_cast<wchar_t*>(mpHBInfo->msgBuf);

    for (; message[i] != '\0'; i++) {
        if (message[i] == L'"') {
            if (!flag) {
                flag = true;
                mpText[j][k] = &message[i + 1];
                j++;

                if (j == 10) {
                    j = 0;
                    k++;
                }
            } else {
                flag = false;
            }
        }
    }
}

void HomeButton::init() {
    int i;

    if (mInitFlag) {
        return;
    }

    mInitFlag = true;
    mAppVolume[3] = 1; // "home button sound active" latch (retail 0x5CA)

    WPADStopSimpleSync();

    mForceSttInitProcFlag = false;
    mForceSttFadeInProcFlag = false;
    mStartBlackOutFlag = false;
    mForceStopSyncFlag = false;
    mSimpleSyncCallback = NULL;

    iReConnectTime = static_cast<int>(
        3600.0f / getInstance()->getHBMDataInfo()->frameDelta);
    iReConnectTime2 = static_cast<int>(
        3570.0f / getInstance()->getHBMDataInfo()->frameDelta);

    if (mEndInitSoundFlag) {
        AXFXReverbHiShutdown(&mAxFxReverb);
        AXRegisterAuxACallback(mAuxCallback, mpAuxContext);
        AXFXSetHooks(mAxFxAlloc, mAxFxFree);
        AXSetAuxAReturnVolume(mAppVolume[0]);
        AXSetAuxBReturnVolume(mAppVolume[1]);
        AXSetAuxCReturnVolume(mAppVolume[2]);
        mEndInitSoundFlag = false;
    }

    for (i = 0; i < res::eBtn_Max + res::eFuncTouchPane_Max; i++) {
        mPaneCounter[i] = 0;
    }

    mState = 0;
    mSequence = eSeq_Normal;
    mReassignedFlag = false;

    mpPaneManager->init();
    mpPaneManager->setAllComponentTriggerTarget(false);

    for (i = 0; i < mButtonNum; i++) {
        mpPaneManager->getPaneComponentByPane(
            mpLayout->GetRootPane()->FindPaneByName(scBtnName[i], true))
            ->setTriggerTarget(true);
    }

    updateTrigPane();

    nw4hbm::ut::Rect layoutRect = mpLayout->GetLayoutRect();
    mDrawInfo.SetViewRect(layoutRect);
    mpLayout->CalculateMtx(mDrawInfo);

    nw4hbm::math::VEC2 cursorPos(-1000.0f, -1000.0f);

    for (i = 0; i < res::eCursorLyt_Max; i++) {
        mpCursorLayout[i]->CalculateMtx(mDrawInfo);
        mpCursorLayout[i]->GetRootPane()
            ->FindPaneByName(scCursorPaneName, true)
            ->SetTranslate(cursorPos);
    }

    for (i = 0; i < 8; i++) {
        mpPaneManager->update(i, -10000.0f, -10000.0f, 0, 0, 0, NULL);
    }

    for (i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        mPadDrawTime[i] = 0;

        mpController[i]->setInValidPos();
        mpController[i]->clrKpadButton();
        mpController[i]->disconnect();
        mpController[i]->clrBatteryFlag();
        mpController[i]->initCallback();
        mpController[i]->initSound();

        mOnPaneVibFrame[i] = 0.0f;
        mOnPaneVibWaitFrame[i] = 0.0f;
    }

    mpLayout->GetRootPane()
        ->FindPaneByName(scFuncPaneName[res::eFuncPane_let_icn_00], true)
        ->SetVisible(false);
    mpLayout->GetRootPane()->FindPaneByName("N_cntrl_01", true)
        ->SetVisible(true);
    mpLayout->GetRootPane()->FindPaneByName("bar_00", true)
        ->SetVisible(true);
    mpLayout->GetRootPane()->FindPaneByName("bar_10", true)
        ->SetVisible(true);

    for (i = res::eFuncTouchPane_B_optnBtn_00;
         i < res::eFuncTouchPane_B_optnBtn_20 + 1; i++) {

        mpLayout->GetRootPane()
            ->FindPaneByName(scFuncTouchPaneName[i], true)
            ->SetVisible(false);
    }

    for (i = 0; i < res::eFuncTextPane_Max; i++) {
        mpLayout->GetRootPane()
            ->FindPaneByName(scFuncTextPaneName[i], true)
            ->SetVisible(false);
    }

    mpRemoteSpk->Start();

    calc(NULL);

    mFader.init(static_cast<int>(
        30.0f / getInstance()->getHBMDataInfo()->frameDelta));
    mFader.mBlackOutFlag = true;
}

inline void HomeButton::init_msg() {
    int i, len;

    for (i = 0; i < res::eFuncTextPane_Max; i++) {
        nw4hbm::lyt::Pane* pPane = mpLayout->GetRootPane()->FindPaneByName(
            scFuncTextPaneName[i], true);

        nw4hbm::lyt::TextBox* pTextBox =
            nw4hbm::ut::DynamicCast<nw4hbm::lyt::TextBox*>(pPane);

        for (len = 0;; len++) {
            if (mpText[mpHBInfo->region][i][len] == L'\"') {
                break;
            }
        }

        pTextBox->SetString(mpText[mpHBInfo->region][i], 0, len);
    }
}

void HomeButton::init_volume() {
    int i, idx;

    mVolumeNum = getVolume();
    setVolume(HBM_MAX_VOLUME);

    for (i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        getController(i)->setSpeakerVol(mVolumeNum /
                                        static_cast<f32>(HBM_MAX_VOLUME));

        getController(i)->connect();
    }

    for (i = 0; i < res::eGrPane_vol_09 - res::eGrPane_vol_00 + 1; i++) {
        if (i < mVolumeNum) {
            idx = findGroupAnimator(i + res::eGrPane_vol_00,
                                    res::eGrAnim_sound_ylw);
            mpGroupAnmController[idx]->start();
        } else {
            idx = findGroupAnimator(i + res::eGrPane_vol_00,
                                    res::eGrAnim_sound_gry);
            mpGroupAnmController[idx]->start();
        }
    }
}

inline void HomeButton::init_vib() {
    int idx;

    mVibFlag = getVibFlag();

    if (mVibFlag) {
        idx = findGroupAnimator(res::eGrPane_optnBtn_10_psh,
                                res::eGrAnim_vb_btn_wht_psh);
        mpGroupAnmController[idx]->start();

        idx = findGroupAnimator(res::eGrPane_optnBtn_11_psh,
                                res::eGrAnim_vb_btn_ylw_psh);
        mpGroupAnmController[idx]->start();
    } else {
        idx = findGroupAnimator(res::eGrPane_optnBtn_10_psh,
                                res::eGrAnim_vb_btn_ylw_psh);
        mpGroupAnmController[idx]->start();

        idx = findGroupAnimator(res::eGrPane_optnBtn_11_psh,
                                res::eGrAnim_vb_btn_wht_psh);
        mpGroupAnmController[idx]->start();
    }
}

void HomeButton::init_sound() {
    if (mpHBInfo->sound_callback != NULL) {
        mpHBInfo->sound_callback(HBM_SOUND_INIT, 0);
    }

    mAppVolume[0] = AXGetAuxAReturnVolume(); // AUX_A
    mAppVolume[1] = AXGetAuxBReturnVolume(); // AUX_B
    mAppVolume[2] = AXGetAuxCReturnVolume(); // AUX_C

    AXFXGetHooks(&mAxFxAlloc, &mAxFxFree);
    AXGetAuxACallback(&mAuxCallback, &mpAuxContext);
    AXFXSetHooks(&HBMAllocMem, &HBMFreeMem);

    mAxFxReverb.preDelay = 0.0f;
    mAxFxReverb.time = 2.5f;
    mAxFxReverb.coloration = 0.5f;
    mAxFxReverb.damping = 0.0f;
    mAxFxReverb.crosstalk = 0.0f;
    mAxFxReverb.mix = 1.0f;

    AXFXReverbHiInit(&mAxFxReverb);
    AXRegisterAuxACallback(&AXFXReverbHiCallback, &mAxFxReverb);

    AXSetAuxAReturnVolume(AX_MAX_VOLUME);
    AXSetAuxBReturnVolume(0);
    AXSetAuxCReturnVolume(0);

    if (mpHBInfo->sound_callback != NULL) {
        mpHBInfo->sound_callback(HBM_SOUND_POST_INIT, 0);
    }

    SetSoundMode(SCGetSoundMode());
    mEndInitSoundFlag = true;
}
void HomeButton::init_battery(const HBMControllerData* pController) {
    int idx;

    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        if (pController->wiiCon[i].kpad) {
            if (!mpHBInfo->cursor) {
                mpCursorLayout[i]
                    ->GetRootPane()
                    ->FindPaneByName(scCursorPaneName, true)
                    ->SetVisible(true);
            }

            idx = findGroupAnimator(i + res::eGrPane_plyr_00,
                                    res::eGrAnim_btry_wht);
            mpGroupAnmController[idx]->start();

            mControllerFlag[i] = true;
            getController(i)->getInfoAsync(
                &sWpadInfo__Q22cf9CfPadTask__Q210homebutton10HomeButton[i]);
        } else {
            if (!mpHBInfo->cursor) {
                mpCursorLayout[i]
                    ->GetRootPane()
                    ->FindPaneByName(scCursorPaneName, true)
                    ->SetVisible(false);
            }

            idx = findGroupAnimator(i + res::eGrPane_plyr_00,
                                    res::eGrAnim_btry_gry);
            mpGroupAnmController[idx]->start();

            mControllerFlag[i] = false;
        }
    }

    reset_battery();
    mGetPadInfoTime = 0;
}

void HomeButton::calc(const HBMControllerData* pController) {
    int i;
    GroupAnmController* pAnim;

    mpPaneManager->calc();

    for (i = 0; i < mAnmNum; i++) {
        mpAnmController[i]->do_calc();
    }

    for (i = 0; i < res::ePairAnm_Max; i++) {
        mpPairGroupAnmController[i]->do_calc();
    }

    for (i = 0; i < res::eGrAnimator_Max; i++) {
        mpGroupAnmController[i]->do_calc();
    }

    for (i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        if (mOnPaneVibFrame[i] > 0.0f) {
            if (!mControllerFlag[i] || !getController(i)->isRumbling()) {
                mOnPaneVibFrame[i] = 0.0f;
                mOnPaneVibWaitFrame[i] = 0.0f;

                if (getController(i)->isRumbling()) {
                    getController(i)->stopMotor();
                }
            } else {
                mOnPaneVibFrame[i] -= mpHBInfo->frameDelta;

                if (mOnPaneVibFrame[i] <= 0.0f || mState == 17) {
                    getController(i)->stopMotor();
                    mOnPaneVibFrame[i] = 0.0f;
                    mOnPaneVibWaitFrame[i] = scOnPaneVibWaitTime;
                }
            }
        } else if (mOnPaneVibWaitFrame[i] > 0.0f) {
            mOnPaneVibWaitFrame[i] -= mpHBInfo->frameDelta;

            if (mOnPaneVibWaitFrame[i] <= 0.0f) {
                mOnPaneVibWaitFrame[i] = 0.0f;
            }
        }
    }

    switch (mState) {
    case 0: {
        if (mpHBInfo->backFlag) {
            mSelectAnmNum = findGroupAnimator(res::eGrPane_hmMenuBck_strt,
                                              res::eGrAnim_hmMenu_strt);

            mpLayout->GetRootPane()
                ->FindPaneByName("back_00", true)
                ->SetVisible(false);

            mpLayout->GetRootPane()
                ->FindPaneByName("back_02", true)
                ->SetVisible(true);
        } else {
            mSelectAnmNum = findGroupAnimator(res::eGrPane_hmMenu_strt,
                                              res::eGrAnim_hmMenu_strt);

            mpLayout->GetRootPane()
                ->FindPaneByName("back_00", true)
                ->SetVisible(true);

            mpLayout->GetRootPane()
                ->FindPaneByName("back_02", true)
                ->SetVisible(false);
        }

        mpGroupAnmController[mSelectAnmNum]->start();

        if (pController != NULL) {
            mState = 1;
            init_battery(pController);
        }

        break;
    }

    case 1: {
        if (!mpGroupAnmController[mSelectAnmNum]->isPlaying()) {
            init_volume();
            init_vib();
            init_sound();
            play_sound(HBM_SE_HOME_BUTTON);

            mState = 2;
        }

        break;
    }

    case 3: {
        if (!mpGroupAnmController[mSelectAnmNum]->isPlaying() &&
            mSelectAnmNum != res::eGrAnimator_optn_bar_in__close_bar_in) {

            reset_battery(true);
        }

        if (--mWaitStopMotorCount <= 0) {
            for (i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
                WPADDisconnect(i);
            }

            mState = 4;

            mDisConnectCount = scWaitDisConnectTime /
                               getInstance()->getHBMDataInfo()->frameDelta;
        }

        break;
    }

    case 4: {
        if (mpGroupAnmController[mSelectAnmNum]->isPlaying()) {
            break;
        }

        if (mDisConnectCount > 0) {
            for (i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
                s32 type;
                if (WPADProbe(i, &type) != WPAD_ERR_NO_CONTROLLER) {
                    break;
                }
            }

            if (i < WPAD_MAX_CONTROLLERS) {
                --mDisConnectCount;
                break;
            }

            mDisConnectCount = 0;
        }

        mState = 5;
        mMsgCount = 0;
        mSoundRetryCnt = 0;
        mSimpleSyncCallback = WPADSetSimpleSyncCallback(&SimpleSyncCallback);
        mEndSimpleSyncFlag = false;
        unk93 = false;

        for (i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
            getController(i)->setEnableRumble(true);
        }

        mSimpleSyncFlag = WPADStartFastSimpleSync();

        if (!mSimpleSyncFlag) {
            setSimpleSyncAlarm(eSyncType_Start);
        }

        break;
    }

    case 5:
    case 6: {
        if (!mSimpleSyncFlag ||
            mpPairGroupAnmController[mSelectAnmNum]->isPlaying()) {
            break;
        }

        if (mMsgCount == 0) {
            int j;
            GroupAnmController* animator;

            for (j = 0; j < 5; j++) {
                int idx = findGroupAnimator(
                    res::eGrPane_optnBtn_00_inOut + j,
                    res::eGrAnim_optn_btn_out);

                animator = mpGroupAnmController[idx];
                animator->start();
            }

            reset_btn();

            mpPairGroupAnmController[res::ePairAnm_12btn_on]->setAnmType(
                FrameController::eAnmType_Wrap);

            mpPairGroupAnmController[res::ePairAnm_12btn_on]->start();
        }

        for (i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
            if (!mControllerFlag[i]) {
                break;
            }
        }

        if (i >= WPAD_MAX_CONTROLLERS) {
            unk93 = true;
        }

        if (unk93) {
            if (mState != 6) {
                if (!getController(unk88)->isPlayReady() ||
                    getController(unk88)
                        ->isPlayingSoundId(HBM_SE_SELECT)) {

                    mState = 6;
                    mMsgCount = iReConnectTime2;
                }

                if (++mSoundRetryCnt <= iReConnectTime2) {
                    break;
                }

                mState = 6;
                mMsgCount = iReConnectTime2;
            } else {
                if (++mMsgCount <= iReConnectTime) {
                    break;
                }

                mState = 7;

                if (!WPADStopSimpleSync()) {
                    setSimpleSyncAlarm(eSyncType_Stop);
                }

                mEndSimpleSyncFlag = true;
            }
        } else {
            if (++mMsgCount <= iReConnectTime) {
                break;
            }

            mState = 7;

            if (!WPADStopSimpleSync()) {
                setSimpleSyncAlarm(eSyncType_Stop);
            }

            mEndSimpleSyncFlag = true;
        }

        break;
    }

    case 7: {
        if (!mEndSimpleSyncFlag) {
            break;
        }

        WPADSetSimpleSyncCallback(mSimpleSyncCallback);
        mpRemoteSpk->ClearPcm();

        for (i = 0; i < 8; i++) {
            mpPaneManager->update(i, -10000.0f, -10000.0f, 0, 0, 0, NULL);
        }

        mSelectAnmNum = res::ePairAnm_link_msg_out;
        mpPairGroupAnmController[mSelectAnmNum]->start();

        mState = 8;
        mpPairGroupAnmController[res::ePairAnm_12btn_on]->setAnmType(
            FrameController::eAnmType_Forward);

        play_sound(HBM_SE_END_CONNECT_WINDOW);

        break;
    }

    case 8: {
        if (mpPairGroupAnmController[mSelectAnmNum]->isPlaying()) {
            break;
        }

        if (mSelectAnmNum == res::ePairAnm_cmn_msg_rtrn) {
            int idx = findGroupAnimator(res::eGrPane_msgBtn_00_inOut,
                                        res::eGrAnim_cmn_msg_btn_out);
            mpGroupAnmController[idx]->start();

            idx = findGroupAnimator(res::eGrPane_msgBtn_01_inOut,
                                    res::eGrAnim_cmn_msg_btn_out);
            mpGroupAnmController[idx]->start();

            mpLayout->GetRootPane()
                ->FindPaneByName(
                    scFuncTextPaneName[res::eFuncTextPane_T_Dialog], true)
                ->SetVisible(false);

        } else if (mSelectAnmNum == res::ePairAnm_link_msg_out) {
            mpLayout->GetRootPane()
                ->FindPaneByName(
                    scFuncTextPaneName[res::eFuncTextPane_T_msg_00], true)
                ->SetVisible(false);

            mpLayout->GetRootPane()
                ->FindPaneByName(
                    scFuncTextPaneName[res::eFuncTextPane_T_msg_01], true)
                ->SetVisible(false);
        }

        mState = 2;

        break;
    }

    case 9: {
        if (mpGroupAnmController[mSelectAnmNum]->isPlaying()) {
            break;
        }

        if (mVibFlag) {
            for (i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
                getController(i)->stopMotor();
            }
        }

        mState = 2;

        break;
    }

    case 10: {
        if (mpPairGroupAnmController[mSelectAnmNum]->isPlaying()) {
            break;
        }

        mBar0AnmRev = 0;
        mBar1AnmRev = 0;
        mBar0AnmRevHold = 0;
        mBar1AnmRevHold = 0;

        if (mSequence != eSeq_Control) {
            for (i = res::eFuncTouchPane_B_optnBtn_00;
                 i < res::eFuncTouchPane_B_optnBtn_20 + 1; i++) {

                mpLayout->GetRootPane()
                    ->FindPaneByName(scFuncTouchPaneName[i], true)
                    ->SetVisible(false);
            }

            mState = 2;
        } else if (mSequence == eSeq_Control) {
            mpLayout->GetRootPane()
                ->FindPaneByName("bar_00", true)
                ->SetVisible(false);

            mSelectAnmNum = res::ePairAnm_cntrl_wndw_opn;
            mpPairGroupAnmController[mSelectAnmNum]->start();

            mSelectAnmNum = res::ePairAnm_optn_bar_psh;
            play_sound(HBM_SE_OPEN_CONTROLLER);

            mState = 8;
        }

        updateTrigPane();

        break;
    }

    case 11: {
        if (mpAnmController[mSelectAnmNum]->isPlaying()) {
            break;
        }

        mSelectAnmNum = res::ePairAnm_cmn_msg_in;
        mpPairGroupAnmController[mSelectAnmNum]->start();

        mState = 12;

        break;
    }

    case 12: {
        if (mpPairGroupAnmController[mSelectAnmNum]->isPlaying()) {
            break;
        }

        updateTrigPane();
        reset_btn();
        mState = 2;

        break;
    }

    case 13: {
        if (mpGroupAnmController[mSelectAnmNum]->isPlaying()) {
            break;
        }

        if (mSelectBtnNum >= 0) {
            mFader.start();
            mState = 19;
            mFadeOutSeTime = mFader.getMaxFrame();

            if (mSelectBtnNum != HBM_SELECT_BTN3 &&
                mpHBInfo->sound_callback != NULL) {
                mpHBInfo->sound_callback(
                    HBM_SOUND_RETURN_APP, static_cast<int>(mFadeOutSeTime));
            }
        } else {
            updateTrigPane();
            mSelectAnmNum = res::ePairAnm_cmn_msg_rtrn;
            mpPairGroupAnmController[mSelectAnmNum]->start();
            mState = 8;
        }

        reset_guiManager(-1);

        break;
    }

    case 14: {
        if (mpPairGroupAnmController[mSelectAnmNum]->isPlaying()) {
            break;
        }

        calc_fadeoutAnm();
        break;
    }

    case 15: {
        if (mpAnmController[mSelectAnmNum]->isPlaying()) {
            break;
        }

        mFader.start();
        mStartBlackOutFlag = true;
        mState = 19;
        mFadeOutSeTime = mFader.getMaxFrame();

        if (mSelectBtnNum != HBM_SELECT_BTN3 &&
            mpHBInfo->sound_callback != NULL) {
            mpHBInfo->sound_callback(
                HBM_SOUND_RETURN_APP, static_cast<int>(mFadeOutSeTime));
        }

        break;
    }

    case 16: {
        mAppVolume[3] = 0;

        if (mSequence <= 2) {
            pAnim = mpGroupAnmController[mSelectAnmNum];
        }

        if (!pAnim->isPlaying()) {
            mState = 17;
            fadeout_sound(0.0f);
        } else {
            f32 restFrame = pAnim->getMaxFrame() - pAnim->getCurrentFrame();
            fadeout_sound(restFrame / mFadeOutSeTime);
        }

        break;
    }

    case 17: {
        mState = 18;

        if (mSelectBtnNum != HBM_SELECT_BTN3) {
            StopAllSeq();

            if (mEndInitSoundFlag) {
                AXFXReverbHiShutdown(&mAxFxReverb);
                AXRegisterAuxACallback(mAuxCallback, mpAuxContext);
                AXFXSetHooks(mAxFxAlloc, mAxFxFree);

                AXSetAuxAReturnVolume(mAppVolume[0]);
                AXSetAuxBReturnVolume(mAppVolume[1]);
                AXSetAuxCReturnVolume(mAppVolume[2]);
            }

            mEndInitSoundFlag = false;
        }

        WPADSetSpeakerVolume(static_cast<u8>(12.7f * mVolumeNum));
        WPADSaveConfig(NULL);

        mpRemoteSpk->ClearPcm();
        mpRemoteSpk->Stop();
        for (i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
            getController(i)->stopMotor();
            mpController[i]->clearCallback();
        }

        if (mSelectBtnNum != HBM_SELECT_BTN3 &&
            mpHBInfo->sound_callback != NULL) {
            mpHBInfo->sound_callback(HBM_SOUND_STOP, 0);
        }

        for (i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
            mpGroupAnmController[findGroupAnimator(
                res::eGrPane_plyr_00 + i, res::eGrAnim_btry_wht)]
                ->stop();
            mpGroupAnmController[findGroupAnimator(
                res::eGrPane_plyr_00 + i, res::eGrAnim_btry_wink_gry)]
                ->stop();
            mpGroupAnmController[findGroupAnimator(
                res::eGrPane_plyr_00 + i, res::eGrAnim_btry_wink)]
                ->stop();
        }

        mInitFlag = false;

        break;
    }

    case 18: {
        mState = 2;

        break;
    }

    case 19: {
        if (mForceSttInitProcFlag) {
            init_battery(pController);
            mForceSttInitProcFlag = false;
        }

        if (mForceSttFadeInProcFlag) {
            init_volume();
            init_vib();
            mForceSttFadeInProcFlag = false;
        }

        if (mFader.isDone()) {
            if (mForceStopSyncFlag) {
                if (!mEndSimpleSyncFlag) {
                    break;
                }

                WPADSetSimpleSyncCallback(mSimpleSyncCallback);
                mForceStopSyncFlag = false;
            }

            if (mForceEndMsgAnmFlag) {
                int idx;

                idx = res::ePairAnm_link_msg_in;
                mpPairGroupAnmController[idx]->initFrame();
                mpPairGroupAnmController[idx]->stop();

                mpLayout->GetRootPane()
                    ->FindPaneByName(
                        scFuncTextPaneName[res::eFuncTextPane_T_msg_00], true)
                    ->SetVisible(false);

                mpLayout->GetRootPane()
                    ->FindPaneByName(
                        scFuncTextPaneName[res::eFuncTextPane_T_msg_01], true)
                    ->SetVisible(false);

                idx = res::ePairAnm_12btn_on;
                if (mpPairGroupAnmController[idx]->isPlaying()) {
                    mpPairGroupAnmController[idx]->initFrame();
                    mpPairGroupAnmController[idx]->stop();
                }
            }

            mState = 17;

            VISetBlack(unk5B8 != 0 || mFader.mBlackOutFlag == 0);
            VIFlush();

            fadeout_sound(0.0f);
        } else {
            f32 restFrame = mFader.getMaxFrame() - mFader.getFrame();
            fadeout_sound(restFrame / mFadeOutSeTime);
        }

        break;
    }

    default: {
        break;
    }

    case 2: {
        break;
    }
    }

    if (mBar0AnmRev && isUpBarActive()) {
        if (mBar0AnmRev != 0 && mBar0AnmRev != mBar0AnmRevHold) {
            mpPairGroupAnmController[mBar0AnmRev]->start();
            mBar0AnmRevHold = mBar0AnmRev;
        }

        mBar0AnmRev = 0;
    }

    if (mBar1AnmRev && isDownBarActive()) {
        if (mBar1AnmRev != 0 && mBar1AnmRev != mBar1AnmRevHold) {
            mpGroupAnmController[mBar1AnmRev]->start();
            mBar1AnmRevHold = mBar1AnmRev;
        }

        mBar1AnmRev = 0;
    }

    if (pController != NULL && mAppVolume[3] != 0) {
        update(pController);
    }

    mpLayout->Animate();
    mpLayout->CalculateMtx(mDrawInfo);

    if (!mpHBInfo->cursor) {
        for (i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
            mpCursorLayout[i]->CalculateMtx(mDrawInfo);
        }
    }

    if (mForcusSEWaitTime <= 2) {
        mForcusSEWaitTime++;
    }
}

inline void HomeButton::calc_fadeoutAnm() {
    mpLayout->GetRootPane()
        ->FindPaneByName(scFuncTextPaneName[res::eFuncTextPane_T_Dialog], true)
        ->SetVisible(false);

    if (mpHBInfo->backFlag) {
        mSelectAnmNum = findGroupAnimator(res::eGrPane_hmMenuBck_fnsh,
                                          res::eGrAnim_hmMenu_fnsh);
    } else {
        mSelectAnmNum = findGroupAnimator(res::eGrPane_hmMenu_fnsh,
                                          res::eGrAnim_hmMenu_fnsh);
    }

    mpGroupAnmController[mSelectAnmNum]->start();
    mState = 16;
    mFadeOutSeTime = mpGroupAnmController[mSelectAnmNum]->getMaxFrame();

    if (mpHBInfo->sound_callback != NULL) {
        mpHBInfo->sound_callback(HBM_SOUND_GOTO_MENU, mFadeOutSeTime);
    }
}

void HomeButton::calc_battery(int chan) {
    for (int i = 0; i < res::eBatteryPane_Max; i++) {
        if (i < sWpadInfo__Q22cf9CfPadTask__Q210homebutton10HomeButton[chan]
                .battery) {
            mpLayout->GetRootPane()
                ->FindPaneByName(scBatteryPaneName[chan][i], true)
                ->SetVisible(true);
        } else {
            mpLayout->GetRootPane()
                ->FindPaneByName(scBatteryPaneName[chan][i], true)
                ->SetVisible(false);
        }
    }

    if (sWpadInfo__Q22cf9CfPadTask__Q210homebutton10HomeButton[chan].battery <
        2) {
        int idx = findGroupAnimator(chan + res::eGrPane_plyr_00,
                                    res::eGrAnim_btry_red);

        mpGroupAnmController[idx]->start();
    } else {
        int idx = findGroupAnimator(chan + res::eGrPane_plyr_00,
                                    res::eGrAnim_btry_wht);

        mpGroupAnmController[idx]->start();
    }

    if (mGetPadInfoTime < scGetPadInfoTime) {
        mGetPadInfoTime = 0;
    }

    getController(chan)->clrBatteryFlag();
}

void HomeButton::draw() {
    GXSetFog(GX_FOG_NONE, (GXColor){0, 0, 0, 255}, 0.0f, 0.0f, 0.0f, 0.0f);

    GXSetTexCoordScaleManually(GX_TEXCOORD0, GX_FALSE, 0, 0);
    GXSetTexCoordCylWrap(GX_TEXCOORD0, GX_FALSE, GX_FALSE);
    GXSetTexCoordScaleManually(GX_TEXCOORD1, GX_FALSE, 0, 0);
    GXSetTexCoordCylWrap(GX_TEXCOORD1, GX_FALSE, GX_FALSE);

    initgx();

    GXSetTevColor(GX_TEVREG0, (GXColor){0, 0, 0, 255});
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition2f32(-500.0f, 228.0f);
    GXPosition2f32(-500.0f, 500.0f);
    GXPosition2f32(500.0f, 500.0f);
    GXPosition2f32(500.0f, 228.0f);

    GXSetTevColor(GX_TEVREG0, (GXColor){0, 0, 0, 255});
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition2f32(-500.0f, -500.0f);
    GXPosition2f32(-500.0f, -228.0f);
    GXPosition2f32(500.0f, -228.0f);
    GXPosition2f32(500.0f, -500.0f);

    initgx();

    mpLayout->Draw(mDrawInfo);

    if (!mpHBInfo->cursor) {
        for (int i = WPAD_MAX_CONTROLLERS - 1; i >= 0; i--) {
            mpCursorLayout[i]->Draw(mDrawInfo);
        }
    }

    mFader.draw();
}

static void SpeakerCallback(OSAlarm* pAlarm, OSContext* /* pContext */) {
    u32 data = reinterpret_cast<u32>(OSGetAlarmUserData(pAlarm));
    int chan = data >> 16;
    int id = data & 0xFFFF;

    HomeButton* pHBObj = HomeButton::getInstance();

    if (!WPADIsSpeakerEnabled(chan) ||
        !pHBObj->getController(chan)->isPlayReady()) {

        OSSetAlarmUserData(
            &reinterpret_cast<HBMBaseAlarmView*>(pHBObj)->mSpeakerAlarm[chan],
            reinterpret_cast<void*>((chan << 16) | (chan + 2)));
        OSCancelAlarm(
            &reinterpret_cast<HBMBaseAlarmView*>(pHBObj)->mSpeakerAlarm[chan]);
        OSSetAlarm(
            &reinterpret_cast<HBMBaseAlarmView*>(pHBObj)->mSpeakerAlarm[chan],
            OS_MSEC_TO_TICKS(50), &SpeakerCallback);
    } else {
        pHBObj->getController(chan)->playSound(id);
    }
}

static void MotorCallback(OSAlarm* pAlarm, OSContext* /* pContext */) {
    Controller* pController =
        reinterpret_cast<Controller*>(OSGetAlarmUserData(pAlarm));

    pController->stopMotor();
}

inline void HomeButton::setSpeakerAlarm(int chan, int msec) {
    OSSetAlarmUserData(&mSpeakerAlarm[chan],
                       reinterpret_cast<void*>((chan << 16) | (chan + 2)));

    OSCancelAlarm(&mSpeakerAlarm[chan]);
    OSSetAlarm(&mSpeakerAlarm[chan], OS_MSEC_TO_TICKS(msec), &SpeakerCallback);
}

static void RetrySimpleSyncCallback(OSAlarm* pAlarm,
                                    OSContext* /* pContext */) {
    HomeButton* pHBObj = HomeButton::getInstance();
    int type = reinterpret_cast<int>(OSGetAlarmUserData(pAlarm));
    bool retrySuccessFlag = false;

    if (type == HomeButton::eSyncType_Start) {
        if (WPADStartFastSimpleSync()) {
            pHBObj->setSimpleSyncFlag(true);
            retrySuccessFlag = true;
        }
    } else /* HomeButton::eSyncType_Stop */ {
        if (WPADStopSimpleSync()) {
            retrySuccessFlag = true;
        }
    }

    if (!retrySuccessFlag) {
        pHBObj->setSimpleSyncAlarm(type);
    }
}

void SimpleSyncCallback(s32 result, s32 num) {
    if (result == WPAD_SYNC_DONE) {
        HomeButton::getInstance()->setEndSimpleSyncFlag(true);
    }

    HomeButton::getInstance()->callSimpleSyncCallback(result, num);
}

inline void HomeButton::setSimpleSyncAlarm(int type) {
    OSCancelAlarm(&mSimpleSyncAlarm);
    OSSetAlarmUserData(&mSimpleSyncAlarm, reinterpret_cast<void*>(type));
    OSSetAlarm(&mSimpleSyncAlarm, OS_MSEC_TO_TICKS(100),
               &RetrySimpleSyncCallback);
}

void HomeButton::update(const HBMControllerData* pController) {
    int i, idx;

    mFader.calc();

    for (i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        if (pController->wiiCon[i].kpad != NULL) {

            if (pController->wiiCon[i].kpad->wpad_err !=
                WPAD_ERR_NO_CONTROLLER) {

                // clang-format off
                if (mPadDrawTime[i] >
                    static_cast<int>(scPadDrawWaitTime
                        / getInstance()->getHBMDataInfo()->frameDelta + 0.5f)) {
                    // clang-format on

                    if (pController->wiiCon[i].kpad->wpad_err == WPAD_ERR_OK ||
                        pController->wiiCon[i].kpad->wpad_err ==
                            WPAD_ERR_CORRUPTED) {
                        bool pointerEnableFlag;

                        if ((pController->wiiCon[i].use_devtype !=
                                 WPAD_DEV_CLASSIC ||
                             pController->wiiCon[i].kpad->dev_type !=
                                 WPAD_DEV_CLASSIC) &&
                            (pController->wiiCon[i].use_devtype !=
                                 WPAD_DEV_NUNCHUK ||
                             pController->wiiCon[i].kpad->dev_type !=
                                 WPAD_DEV_NUNCHUK)) {

                            if (pController->wiiCon[i].kpad->dpd_valid_fg >
                                0) {
                                pointerEnableFlag = true;
                            } else {
                                pointerEnableFlag = false;
                            }
                        } else {
                            pointerEnableFlag = true;
                        }

                        mpController[i]->setKpad(&pController->wiiCon[i],
                                                 pointerEnableFlag);

                        if (!mpHBInfo->cursor) {
                            mpCursorLayout[i]
                                ->GetRootPane()
                                ->FindPaneByName(scCursorPaneName, true)
                                ->SetVisible(true);
                        }
                    }
                } else {
                    mPadDrawTime[i]++;
                }

                if (!((pController->wiiCon[i].use_devtype ==
                           WPAD_DEV_CLASSIC &&
                       pController->wiiCon[i].kpad->dev_type ==
                           WPAD_DEV_CLASSIC) ||
                      (pController->wiiCon[i].use_devtype ==
                           WPAD_DEV_NUNCHUK &&
                       pController->wiiCon[i].kpad->dev_type ==
                           WPAD_DEV_NUNCHUK))) {

                    if (pController->wiiCon[i].kpad->dpd_valid_fg <= 0) {
                        s32 result;
                        s32 type;
                        result = WPADProbe(i, &type);

                        if (pController->wiiCon[i].kpad->wpad_err !=
                                WPAD_ERR_COMMUNICATION_ERROR &&
                            result != WPAD_ERR_COMMUNICATION_ERROR) {

                            mpController[i]->setInValidPos();
                        }
                    }
                }
            } else {
                mpController[i]->setInValidPos();

                if (!mpHBInfo->cursor) {
                    mpCursorLayout[i]
                        ->GetRootPane()
                        ->FindPaneByName(scCursorPaneName, true)
                        ->SetVisible(false);
                }
            }

            if (!mControllerFlag[i]) {
                unk88 = i;
                mControllerFlag[i] = true;

                getController(i)->getInfoAsync(
                    &sWpadInfo__Q22cf9CfPadTask__Q210homebutton10HomeButton[i]);

                idx = findGroupAnimator(i + res::eGrPane_plyr_00,
                                        res::eGrAnim_btry_wht);
                mpGroupAnmController[idx]->start();

                idx = findGroupAnimator(i + res::eGrPane_plyr_00,
                                        res::eGrAnim_btry_wink_gry);
                mpGroupAnmController[idx]->stop();

                idx = findGroupAnimator(i + res::eGrPane_plyr_00,
                                        res::eGrAnim_btry_wink);
                mpGroupAnmController[idx]->start();

                play_sound(i + HBM_SE_CONNECTED);
                getController(i)->connect();
                getController(i)->startMotor();

                OSSetAlarmUserData(&mAlarm[i],
                                   reinterpret_cast<void*>(getController(i)));

                OSCancelAlarm(&mAlarm[i]);
                OSSetAlarm(&mAlarm[i], OS_MSEC_TO_TICKS(300), &MotorCallback);

                setSpeakerAlarm(i, 400);
            }

            if (pController->wiiCon[i].kpad->wpad_err == WPAD_ERR_OK ||
                pController->wiiCon[i].kpad->wpad_err == WPAD_ERR_CORRUPTED) {
                nw4hbm::math::VEC3 vec;

                if ((pController->wiiCon[i].use_devtype ==
                         WPAD_DEV_CLASSIC &&
                     pController->wiiCon[i].kpad->dev_type ==
                         WPAD_DEV_CLASSIC) ||
                    (pController->wiiCon[i].use_devtype ==
                         WPAD_DEV_NUNCHUK &&
                     pController->wiiCon[i].kpad->dev_type ==
                         WPAD_DEV_NUNCHUK)) {
                    vec = nw4hbm::math::VEC3(0.0f, 0.0f, 15.0f);
                } else {
                    Vec2 v = pController->wiiCon[i].kpad->horizon;
                    f32 rad = nw4hbm::math::Atan2Deg(-v.y, v.x);

                    vec = nw4hbm::math::VEC3(0.0f, 0.0f, rad);
                }

                if (!mpHBInfo->cursor) {
                    mpCursorLayout[i]
                        ->GetRootPane()
                        ->FindPaneByName(scCursorRotPaneName, true)
                        ->SetRotate(vec);

                    mpCursorLayout[i]
                        ->GetRootPane()
                        ->FindPaneByName(scCursorSRotPaneName, true)
                        ->SetRotate(vec);
                }

                if (mGetPadInfoTime > scGetPadInfoTime) {
                    getController(i)->getInfoAsync(
                        &sWpadInfo__Q22cf9CfPadTask__Q210homebutton10HomeButton[i]);
                }

                update_controller(i);

                if (!mpHBInfo->cursor) {
                    update_posController(i);
                }
            }

            if (getController(i)->getBatteryFlag()) {
                calc_battery(i);
            }

        } else {
            if (mControllerFlag[i]) {
                idx = findGroupAnimator(i + res::eGrPane_plyr_00,
                                        res::eGrAnim_btry_wht);
                mpGroupAnmController[idx]->start();

                idx = findGroupAnimator(i + res::eGrPane_plyr_00,
                                        res::eGrAnim_btry_wink_gry);
                mpGroupAnmController[idx]->start();

                for (int j = 0; j < res::eBatteryPane_Max; j++) {
                    mpLayout->GetRootPane()
                        ->FindPaneByName(scBatteryPaneName[i][j], true)
                        ->SetVisible(false);
                }

                if (!mpHBInfo->cursor) {
                    mpCursorLayout[i]
                        ->GetRootPane()
                        ->FindPaneByName(scCursorPaneName, true)
                        ->SetVisible(false);
                }

                mPadDrawTime[i] = 0;
                mControllerFlag[i] = false;

                mpController[i]->setInValidPos();
                mpController[i]->clrKpadButton();
                mpController[i]->disconnect();
            }

            reset_guiManager(i);
        }
    }

    if (mGetPadInfoTime > scGetPadInfoTime) {
        mGetPadInfoTime = 0;
    } else {
        mGetPadInfoTime++;
    }
}

void HomeButton::update_controller(int id) {
    if (isActive()) {
        HBController* pController = mpController[id]->getController();

        f32 x = pController->x * 608.0f / 2.0f;
        f32 y = pController->y * 456.0f / 2.0f;

        if (mAdjustFlag) {
            x *= mpHBInfo->adjust.x;
            y *= mpHBInfo->adjust.y;
        }

        mpPaneManager->update(id, x, -y, pController->trig, pController->hold,
                              pController->release, pController);

        if ((pController->trig & WPAD_BUTTON_HOME) && isActive()) {
            if (mSequence == eSeq_Control) {
                mpLayout->GetRootPane()
                    ->FindPaneByName(lbl_eu_80549D1C, true)
                    ->SetVisible(true);

                mpPaneManager->update(id, 0.0f, -180.0f, 0, 0, 0, NULL);

                mpPairGroupAnmController[res::ePairAnm_hmMenu_bar_psh]
                    ->start();

                mSelectAnmNum = res::ePairAnm_close_bar_psh;
                mpPairGroupAnmController[mSelectAnmNum]->start();

                mpPairGroupAnmController[res::ePairAnm_cntrl_dwn]->start();

                mState = 10;
                mSequence = eSeq_Normal;

                play_sound(HBM_SE_CLOSE_CONTROLLER);
            } else if (mSequence == eSeq_Normal) {
                if (mpPairGroupAnmController[res::ePairAnm_hmMenu_bar_in]
                        ->isPlaying()) {

                    mpPairGroupAnmController[res::ePairAnm_hmMenu_bar_in]
                        ->stop();
                }

                if (mpPairGroupAnmController[res::ePairAnm_hmMenu_bar_out]
                        ->isPlaying()) {

                    mpPairGroupAnmController[res::ePairAnm_hmMenu_bar_out]
                        ->stop();
                }

                mSelectBtnNum = HBM_SELECT_HOMEBTN;

                mSelectAnmNum = res::ePairAnm_hmMenu_bar_psh;
                mpPairGroupAnmController[mSelectAnmNum]->start();

                mState = 14;
                play_sound(HBM_SE_RETURN_APP);
            }
        } else if (mSequence == eSeq_Control && isActive()) {
            if (pController->trig & WPAD_BUTTON_MINUS) {
                if (mVolumeNum > 0) {
                    mVolumeNum--;

                    int idx =
                        findGroupAnimator(mVolumeNum + res::eGrPane_vol_00,
                                          res::eGrAnim_sound_ylw);
                    mpGroupAnmController[idx]->stop();

                    idx = findGroupAnimator(mVolumeNum + res::eGrPane_vol_00,
                                            res::eGrAnim_sound_gry);
                    mpGroupAnmController[idx]->start();

                    idx = findGroupAnimator(res::eGrPane_optnBtn_00_psh,
                                            res::eGrAnim_optn_btn_psh);
                    mpGroupAnmController[idx]->start();

                    if (mVolumeNum == 0) {
                        play_sound(HBM_SE_VOLUME_MINUS_LIMIT);

                        for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
                            getController(i)->setSpeakerVol(
                                mVolumeNum / static_cast<f32>(HBM_MAX_VOLUME));

                            getController(i)->playSound(HBM_SPK_SE_CONNECT1);
                        }
                    } else {
                        play_sound(HBM_SE_VOLUME_MINUS);

                        for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
                            getController(i)->setSpeakerVol(
                                mVolumeNum / static_cast<f32>(HBM_MAX_VOLUME));

                            getController(i)->playSound(HBM_SPK_SE_CONNECT1);
                        }
                    }
                } else {
                    play_sound(HBM_SE_NOTHING_DONE);
                }
            } else if (pController->trig & WPAD_BUTTON_PLUS) {
                if (mVolumeNum < HBM_MAX_VOLUME) {
                    int idx =
                        findGroupAnimator(mVolumeNum + res::eGrPane_vol_00,
                                          res::eGrAnim_sound_gry);
                    mpGroupAnmController[idx]->stop();

                    idx = findGroupAnimator(mVolumeNum + res::eGrPane_vol_00,
                                            res::eGrAnim_sound_ylw);
                    mpGroupAnmController[idx]->start();

                    mVolumeNum++;

                    idx = findGroupAnimator(res::eGrPane_optnBtn_01_psh,
                                            res::eGrAnim_optn_btn_psh);
                    mpGroupAnmController[idx]->start();

                    if (mVolumeNum == HBM_MAX_VOLUME) {
                        play_sound(HBM_SE_VOLUME_PLUS_LIMIT);

                        for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
                            getController(i)->setSpeakerVol(
                                mVolumeNum / static_cast<f32>(HBM_MAX_VOLUME));

                            getController(i)->playSound(HBM_SPK_SE_CONNECT1);
                        }
                    } else {
                        play_sound(HBM_SE_VOLUME_PLUS);

                        for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
                            getController(i)->setSpeakerVol(
                                mVolumeNum / static_cast<f32>(HBM_MAX_VOLUME));

                            getController(i)->playSound(HBM_SPK_SE_CONNECT1);
                        }
                    }
                } else {
                    play_sound(HBM_SE_NOTHING_DONE);
                }
            }
        }
    } else if (mSequence == eSeq_Control && mState == 5 &&
               !mpPairGroupAnmController[mSelectAnmNum]->isPlaying()) {

        HBController* pController = mpController[id]->getController();
        if (pController->trig != 0) {
            mMsgCount = iReConnectTime;
        }
    }
}

inline void HomeButton::update_posController(int id) {
    HBController* pController = mpController[id]->getController();
    nw4hbm::ut::Rect layoutRect = mpLayout->GetLayoutRect();

    f32 x = pController->x * layoutRect.right;
    f32 y = pController->y * layoutRect.bottom;
    nw4hbm::math::VEC2 pos(x, y);

    mpCursorLayout[id]
        ->GetRootPane()
        ->FindPaneByName(scCursorPaneName, true)
        ->SetTranslate(pos);
}

void HomeButton::updateTrigPane() {
    int i;

    switch (mSequence) {
    case eSeq_Normal: {
        for (i = 0; i < res::eFuncTouchPane_Max; i++) {

            if (i < res::eFuncTouchPane_B_optnBtn_00 ||
                i == res::eFuncTouchPane_cntrl_00) {

                nw4hbm::lyt::Pane* pTouchPane =
                    mpLayout->GetRootPane()->FindPaneByName(
                        scFuncTouchPaneName[i], true);

                mpPaneManager->getPaneComponentByPane(pTouchPane)
                    ->setTriggerTarget(true);

            } else {
                nw4hbm::lyt::Pane* pTouchPane =
                    mpLayout->GetRootPane()->FindPaneByName(
                        scFuncTouchPaneName[i], true);

                mpPaneManager->getPaneComponentByPane(pTouchPane)
                    ->setTriggerTarget(false);
            }
        }

        break;
    }

    case eSeq_Control: {
        for (i = 0; i < res::eFuncTouchPane_Max; i++) {

            if ((i > res::eFuncTouchPane_B_btn_00 &&
                 i < res::eFuncTouchPane_B_BtnA) ||
                i == res::eFuncTouchPane_cntrl_00) {

                nw4hbm::lyt::Pane* pTouchPane =
                    mpLayout->GetRootPane()->FindPaneByName(
                        scFuncTouchPaneName[i], true);

                mpPaneManager->getPaneComponentByPane(pTouchPane)
                    ->setTriggerTarget(true);

            } else {
                nw4hbm::lyt::Pane* pTouchPane =
                    mpLayout->GetRootPane()->FindPaneByName(
                        scFuncTouchPaneName[i], true);

                mpPaneManager->getPaneComponentByPane(pTouchPane)
                    ->setTriggerTarget(false);
            }
        }

        break;
    }

    case eSeq_Cmn: {
        for (i = 0; i < res::eFuncTouchPane_Max; i++) {

            if (i < res::eFuncTouchPane_B_BtnA ||
                i == res::eFuncTouchPane_cntrl_00) {

                nw4hbm::lyt::Pane* pTouchPane =
                    mpLayout->GetRootPane()->FindPaneByName(
                        scFuncTouchPaneName[i], true);

                mpPaneManager->getPaneComponentByPane(pTouchPane)
                    ->setTriggerTarget(false);

            } else {
                nw4hbm::lyt::Pane* pTouchPane =
                    mpLayout->GetRootPane()->FindPaneByName(
                        scFuncTouchPaneName[i], true);

                mpPaneManager->getPaneComponentByPane(pTouchPane)
                    ->setTriggerTarget(true);
            }
        }

        break;
    }
    }
}

void HomeButton::startPointEvent(const nw4hbm::lyt::Pane* pPane, void* pData) {
    int idx;
    int btnNo = getPaneNo(pPane);
    HBController* pController = static_cast<HBController*>(pData);
    bool onFlag = false;

    if (isActive() && btnNo != -1 && mPaneCounter[btnNo] == 0) {
        if (mSequence != eSeq_Cmn && btnNo < mButtonNum) {
            idx = findAnimator(btnNo, res::eAnim_cntBtn_in);
            mpAnmController[idx]->start();

            if (mSequence == eSeq_Normal) {
                setForcusSE();
                onFlag = true;
            }

        } else {
            switch (btnNo - mButtonNum) {
            case res::eFuncTouchPane_B_btn_00: {
                if (mSequence == eSeq_Normal) {
                    if (isUpBarActive()) {
                        mpPairGroupAnmController[res::ePairAnm_hmMenu_bar_in]
                            ->start();

                        mBar0AnmRevHold = res::ePairAnm_hmMenu_bar_in;
                        mBar0AnmRev = 0;

                        setForcusSE();
                        onFlag = true;
                    } else {
                        mBar0AnmRev = res::ePairAnm_hmMenu_bar_in;
                    }
                }

                break;
            }

            case res::eFuncTouchPane_B_bar_10:
            case res::eFuncTouchPane_cntrl_00: {
                if (mSequence == eSeq_Normal) {
                    idx = findGroupAnimator(res::eGrPane_optn_bar_in,
                                            res::eGrAnim_optn_bar_in);

                    if (isDownBarActive()) {
                        mpGroupAnmController[idx]->start();
                        mBar1AnmRevHold = idx;
                        mBar1AnmRev = 0;

                        setForcusSE();
                        onFlag = true;
                    } else {
                        mBar1AnmRev = idx;
                    }

                } else if (mSequence == eSeq_Control) {
                    idx = findGroupAnimator(res::eGrPane_optn_bar_in,
                                            res::eGrAnim_close_bar_in);

                    if (isDownBarActive()) {
                        mpGroupAnmController[idx]->start();
                        mBar1AnmRevHold = idx;
                        mBar1AnmRev = 0;

                        setForcusSE();
                        onFlag = true;
                    } else {
                        mBar1AnmRev = idx;
                    }
                }

                break;
            }

            case res::eFuncTouchPane_B_optnBtn_00: {
                if (mSequence == eSeq_Control) {
                    idx = findGroupAnimator(res::eGrPane_optnBtn_00_inOut,
                                            res::eGrAnim_optn_btn_in);

                    mpGroupAnmController[idx]->start();

                    setForcusSE();
                    onFlag = true;
                }

                break;
            }

            case res::eFuncTouchPane_B_optnBtn_01: {
                if (mSequence == eSeq_Control) {
                    idx = findGroupAnimator(res::eGrPane_optnBtn_01_inOut,
                                            res::eGrAnim_optn_btn_in);
                    mpGroupAnmController[idx]->start();

                    setForcusSE();
                    onFlag = true;
                }

                break;
            }

            case res::eFuncTouchPane_B_optnBtn_10: {
                if (mSequence == eSeq_Control) {
                    idx = findGroupAnimator(res::eGrPane_optnBtn_10_inOut,
                                            res::eGrAnim_optn_btn_in);
                    mpGroupAnmController[idx]->start();

                    setForcusSE();
                    onFlag = true;
                }

                break;
            }

            case res::eFuncTouchPane_B_optnBtn_11: {
                if (mSequence == eSeq_Control) {
                    idx = findGroupAnimator(res::eGrPane_optnBtn_11_inOut,
                                            res::eGrAnim_optn_btn_in);
                    mpGroupAnmController[idx]->start();

                    setForcusSE();
                    onFlag = true;
                }

                break;
            }

            case res::eFuncTouchPane_B_optnBtn_20: {
                if (mSequence == eSeq_Control) {
                    idx = findGroupAnimator(res::eGrPane_optnBtn_20_inOut,
                                            res::eGrAnim_optn_btn_in);
                    mpGroupAnmController[idx]->start();

                    setForcusSE();
                    onFlag = true;
                }

                break;
            }

            case res::eFuncTouchPane_B_BtnA: {
                if (mSequence == eSeq_Cmn) {
                    idx = findGroupAnimator(res::eGrPane_msgBtn_00_inOut,
                                            res::eGrAnim_cmn_msg_btn_in);
                    mpGroupAnmController[idx]->start();

                    setForcusSE();
                    onFlag = true;
                }

                break;
            }

            case res::eFuncTouchPane_B_BtnB: {
                if (mSequence == eSeq_Cmn) {
                    idx = findGroupAnimator(res::eGrPane_msgBtn_01_inOut,
                                            res::eGrAnim_cmn_msg_btn_in);
                    mpGroupAnmController[idx]->start();

                    setForcusSE();
                    onFlag = true;
                }

                break;
            }
            }
        }
    }

    if (btnNo == mButtonNum + res::eFuncTouchPane_B_bar_10 ||
        btnNo == mButtonNum + res::eFuncTouchPane_cntrl_00) {
        mPaneCounter[mButtonNum + res::eFuncTouchPane_B_bar_10]++;
        mPaneCounter[mButtonNum + res::eFuncTouchPane_cntrl_00]++;
    } else {
        mPaneCounter[btnNo]++;
    }

    if (onFlag && pController != NULL) {
        if (!getController(pController->chan)->isRumbling() &&
            mOnPaneVibWaitFrame[pController->chan] <= 0.0f) {

            mOnPaneVibFrame[pController->chan] = scOnPaneVibTime;
            getController(pController->chan)->startMotor();
        }
    }
}

void HomeButton::startLeftEvent(const nw4hbm::lyt::Pane* pPane) {
    int idx;
    int btnNo = getPaneNo(pPane);

    if (0 < mPaneCounter[btnNo]) {
        if (btnNo == mButtonNum + res::eFuncTouchPane_B_bar_10 ||
            btnNo == mButtonNum + res::eFuncTouchPane_cntrl_00) {
            mPaneCounter[mButtonNum + res::eFuncTouchPane_B_bar_10]--;
            mPaneCounter[mButtonNum + res::eFuncTouchPane_cntrl_00]--;
        } else {
            mPaneCounter[btnNo]--;
        }
    }

    if (isActive() && btnNo != -1 && !mPaneCounter[btnNo]) {
        if (mSequence != eSeq_Cmn && btnNo < mButtonNum) {
            idx = findAnimator(btnNo, res::eAnim_cntBtn_out);
            mpAnmController[idx]->start();

        } else {
            switch (btnNo - mButtonNum) {
            case res::eFuncTouchPane_B_btn_00: {
                if (mSequence == eSeq_Normal) {
                    if (isUpBarActive()) {
                        mpPairGroupAnmController[res::ePairAnm_hmMenu_bar_out]
                            ->start();

                        mBar0AnmRevHold = res::ePairAnm_hmMenu_bar_out;
                        mBar0AnmRev = 0;
                    } else {
                        mBar0AnmRev = res::ePairAnm_hmMenu_bar_out;
                    }
                }

                break;
            }

            case res::eFuncTouchPane_B_bar_10:
            case res::eFuncTouchPane_cntrl_00: {
                if (mSequence == eSeq_Normal) {
                    idx = findGroupAnimator(res::eGrPane_optn_bar_out,
                                            res::eGrAnim_optn_bar_out);

                    if (isDownBarActive()) {
                        mpGroupAnmController[idx]->start();
                        mBar1AnmRevHold = idx;
                        mBar1AnmRev = 0;
                    } else {
                        mBar1AnmRev = idx;
                    }
                } else if (mSequence == eSeq_Control) {
                    idx = findGroupAnimator(res::eGrPane_optn_bar_out,
                                            res::eGrAnim_close_bar_out);

                    if (isDownBarActive()) {
                        mpGroupAnmController[idx]->start();
                        mBar1AnmRevHold = idx;
                        mBar1AnmRev = 0;
                    } else {
                        mBar1AnmRev = idx;
                    }
                }

                break;
            }

            case res::eFuncTouchPane_B_optnBtn_00: {
                if (mSequence == eSeq_Control) {
                    idx = findGroupAnimator(res::eGrPane_optnBtn_00_inOut,
                                            res::eGrAnim_optn_btn_out);
                    mpGroupAnmController[idx]->start();
                }

                break;
            }
            case res::eFuncTouchPane_B_optnBtn_01: {
                if (mSequence == eSeq_Control) {
                    idx = findGroupAnimator(res::eGrPane_optnBtn_01_inOut,
                                            res::eGrAnim_optn_btn_out);
                    mpGroupAnmController[idx]->start();
                }

                break;
            }

            case res::eFuncTouchPane_B_optnBtn_10: {
                if (mSequence == eSeq_Control) {
                    idx = findGroupAnimator(res::eGrPane_optnBtn_10_inOut,
                                            res::eGrAnim_optn_btn_out);
                    mpGroupAnmController[idx]->start();
                }

                break;
            }

            case res::eFuncTouchPane_B_optnBtn_11: {
                if (mSequence == eSeq_Control) {
                    idx = findGroupAnimator(res::eGrPane_optnBtn_11_inOut,
                                            res::eGrAnim_optn_btn_out);
                    mpGroupAnmController[idx]->start();
                }

                break;
            }

            case res::eFuncTouchPane_B_optnBtn_20: {
                if (mSequence == eSeq_Control) {
                    idx = findGroupAnimator(res::eGrPane_optnBtn_20_inOut,
                                            res::eGrAnim_optn_btn_out);
                    mpGroupAnmController[idx]->start();
                }

                break;
            }

            case res::eFuncTouchPane_B_BtnA: {
                if (mSequence == eSeq_Cmn) {
                    idx = findGroupAnimator(res::eGrPane_msgBtn_00_inOut,
                                            res::eGrAnim_cmn_msg_btn_out);
                    mpGroupAnmController[idx]->start();
                }

                break;
            }

            case res::eFuncTouchPane_B_BtnB: {
                if (mSequence == eSeq_Cmn) {
                    idx = findGroupAnimator(res::eGrPane_msgBtn_01_inOut,
                                            res::eGrAnim_cmn_msg_btn_out);
                    mpGroupAnmController[idx]->start();
                }

                break;
            }
            }
        }
    }
}

void HomeButton::startTrigEvent(const nw4hbm::lyt::Pane* pPane) {
    int idx;
    int btnNo;

    btnNo = getPaneNo(pPane);

    if (isActive() && btnNo != -1) {
        if (mSequence == eSeq_Normal && btnNo < mButtonNum) {
            mSelectBtnNum =
                static_cast<HBMSelectBtnNum>(btnNo + HBM_SELECT_BTN1);

            mSelectAnmNum = findAnimator(btnNo + res::eGroup_btnL_00_psh,
                                         res::eAnim_cntBtn_psh);
            mpAnmController[mSelectAnmNum]->start();

            play_sound(HBM_SE_SELECT);

            if (mDialogFlag[btnNo]) {
                mState = 11;
                mSequence = eSeq_Cmn;

                nw4hbm::lyt::Pane* pPane =
                    mpLayout->GetRootPane()->FindPaneByName(
                        scFuncTextPaneName[res::eFuncTextPane_T_Dialog], true);

                nw4hbm::lyt::TextBox* pTextBox =
                    nw4hbm::ut::DynamicCast<nw4hbm::lyt::TextBox*>(pPane);

                u16 len;
                if (mpHBInfo->messageFlag & (btnNo + 1)) {
                    for (len = 0;; len++) {
                        // U+FF1F FULLWIDTH QUESTION MARK
                        if (mpText[mpHBInfo->region][btnNo + 2][len] == L'？') {
                            break;
                        }

                        // U+003F QUESTION MARK
                        if (mpText[mpHBInfo->region][btnNo + 2][len] == L'?') {
                            break;
                        }
                    }
                } else {
                    for (len = 0;; len++) {
                        // U+0022 QUOTATION MARK
                        if (mpText[mpHBInfo->region][btnNo + 2][len + 1] ==
                            L'"') {
                            break;
                        }
                    }
                }

                pTextBox->SetString(mpText[mpHBInfo->region][btnNo + 2], 0,
                                    ++len);

                mpLayout->GetRootPane()
                    ->FindPaneByName(
                        scFuncTextPaneName[res::eFuncTextPane_T_Dialog], true)
                    ->SetVisible(true);
            } else {
                mState = 15;
            }

        } else
            switch (btnNo - mButtonNum) {
            case res::eFuncTouchPane_B_btn_00: {
                if (mpPairGroupAnmController[res::ePairAnm_hmMenu_bar_out]
                        ->isPlaying()) {

                    mpPairGroupAnmController[res::ePairAnm_hmMenu_bar_out]
                        ->stop();
                }

                if (mpPairGroupAnmController[res::ePairAnm_hmMenu_bar_in]
                        ->isPlaying()) {

                    mpPairGroupAnmController[res::ePairAnm_hmMenu_bar_in]
                        ->stop();
                }

                mSelectBtnNum = HBM_SELECT_HOMEBTN;

                mSelectAnmNum = res::ePairAnm_hmMenu_bar_psh;
                mpPairGroupAnmController[mSelectAnmNum]->start();

                mState = 14;
                play_sound(HBM_SE_RETURN_APP);

                break;
            }

            case res::eFuncTouchPane_B_bar_10:
            case res::eFuncTouchPane_cntrl_00: {
                if (mSequence == eSeq_Control) {
                    mpLayout->GetRootPane()
                        ->FindPaneByName("bar_00", true)
                        ->SetVisible(true);

                    mpPairGroupAnmController[res::ePairAnm_hmMenu_bar_psh]
                        ->start();

                    mpPairGroupAnmController[res::ePairAnm_cntrl_dwn]->start();

                    mSelectAnmNum = res::ePairAnm_close_bar_psh;
                    mpPairGroupAnmController[mSelectAnmNum]->start();

                    mState = 10;
                    mSequence = eSeq_Normal;
                    play_sound(HBM_SE_CLOSE_CONTROLLER);

                } else if (mSequence == eSeq_Normal) {
                    mpPairGroupAnmController[res::ePairAnm_optn_bar_psh]
                        ->start();

                    mSelectAnmNum = res::ePairAnm_cntrl_up;
                    mpPairGroupAnmController[mSelectAnmNum]->start();

                    mState = 10;

                    for (int i = res::eFuncTouchPane_B_optnBtn_00;
                         i < res::eFuncTouchPane_B_optnBtn_20 + 1; i++) {

                        mpLayout->GetRootPane()
                            ->FindPaneByName(scFuncTouchPaneName[i], true)
                            ->SetVisible(true);
                    }

                    mSequence = eSeq_Control;
                    play_sound(HBM_SE_SELECT);
                }

                break;
            }

            case res::eFuncTouchPane_B_optnBtn_00: {
                if (mVolumeNum > 0) {
                    mVolumeNum--;

                    idx = findGroupAnimator(mVolumeNum + res::eGrPane_vol_00,
                                            res::eGrAnim_sound_ylw);
                    mpGroupAnmController[idx]->stop();

                    idx = findGroupAnimator(mVolumeNum + res::eGrPane_vol_00,
                                            res::eGrAnim_sound_gry);
                    mpGroupAnmController[idx]->start();

                    if (mVolumeNum == 0) {
                        play_sound(HBM_SE_VOLUME_MINUS_LIMIT);

                        for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
                            getController(i)->setSpeakerVol(
                                mVolumeNum / static_cast<f32>(HBM_MAX_VOLUME));

                            getController(i)->playSound(HBM_SPK_SE_CONNECT1);
                        }
                    } else {
                        play_sound(HBM_SE_VOLUME_MINUS);

                        for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
                            getController(i)->setSpeakerVol(
                                mVolumeNum / static_cast<f32>(HBM_MAX_VOLUME));

                            getController(i)->playSound(HBM_SPK_SE_CONNECT1);
                        }
                    }

                    idx = findGroupAnimator(res::eGrPane_optnBtn_00_psh,
                                            res::eGrAnim_optn_btn_psh);
                    mpGroupAnmController[idx]->start();

                } else {
                    play_sound(HBM_SE_NOTHING_DONE);
                }

                break;
            }

            case res::eFuncTouchPane_B_optnBtn_01: {
                if (mVolumeNum < HBM_MAX_VOLUME) {
                    idx = findGroupAnimator(mVolumeNum + res::eGrPane_vol_00,
                                            res::eGrAnim_sound_gry);
                    mpGroupAnmController[idx]->stop();

                    idx = findGroupAnimator(mVolumeNum + res::eGrPane_vol_00,
                                            res::eGrAnim_sound_ylw);
                    mpGroupAnmController[idx]->start();

                    mVolumeNum++;

                    if (mVolumeNum == HBM_MAX_VOLUME) {
                        play_sound(HBM_SE_VOLUME_PLUS_LIMIT);

                        for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
                            getController(i)->setSpeakerVol(
                                mVolumeNum / static_cast<f32>(HBM_MAX_VOLUME));

                            getController(i)->playSound(HBM_SPK_SE_CONNECT1);
                        }
                    } else {
                        play_sound(HBM_SE_VOLUME_PLUS);

                        for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
                            getController(i)->setSpeakerVol(
                                mVolumeNum / static_cast<f32>(HBM_MAX_VOLUME));

                            getController(i)->playSound(HBM_SPK_SE_CONNECT1);
                        }
                    }

                    idx = findGroupAnimator(res::eGrPane_optnBtn_01_psh,
                                            res::eGrAnim_optn_btn_psh);
                    mpGroupAnmController[idx]->start();

                } else {
                    play_sound(HBM_SE_NOTHING_DONE);
                }

                break;
            }

            case res::eFuncTouchPane_B_optnBtn_10: {
                if (!mVibFlag) {
                    mVibFlag = true;

                    setVibFlag(mVibFlag);

                    idx = findGroupAnimator(res::eGrPane_optnBtn_11_psh,
                                            res::eGrAnim_vb_btn_ylw_psh);
                    mpGroupAnmController[idx]->start();

                    mSelectAnmNum =
                        findGroupAnimator(res::eGrPane_optnBtn_10_cntrl,
                                          res::eGrAnim_vb_btn_wht_psh);
                    mpGroupAnmController[mSelectAnmNum]->start();

                    play_sound(HBM_SE_VIBE_ON);
                } else {
                    mSelectAnmNum =
                        findGroupAnimator(res::eGrPane_optnBtn_10_cntrl,
                                          res::eGrAnim_vb_btn_ylw_ylw);
                    mpGroupAnmController[mSelectAnmNum]->start();

                    play_sound(HBM_SE_NOTHING_DONE);
                }

                for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
                    getController(i)->startMotor();
                    mOnPaneVibFrame[i] = 0.0f;
                    mOnPaneVibWaitFrame[i] = 0.0f;
                }

                mState = 9;

                break;
            }

            case res::eFuncTouchPane_B_optnBtn_11: {
                if (mVibFlag) {
                    mVibFlag = false;
                    setVibFlag(mVibFlag);

                    idx = findGroupAnimator(res::eGrPane_optnBtn_11_psh,
                                            res::eGrAnim_vb_btn_wht_psh);
                    mpGroupAnmController[idx]->start();

                    mSelectAnmNum =
                        findGroupAnimator(res::eGrPane_optnBtn_10_psh,
                                          res::eGrAnim_vb_btn_ylw_psh);
                    mpGroupAnmController[mSelectAnmNum]->start();

                    play_sound(HBM_SE_VIBE_OFF);
                } else {
                    play_sound(HBM_SE_NOTHING_DONE);
                }

                mState = 9;

                break;
            }

            case res::eFuncTouchPane_B_optnBtn_20: {
                mSelectAnmNum = findGroupAnimator(res::eGrPane_optnBtn_20_psh,
                                                  res::eGrAnim_optn_btn_psh);
                mpGroupAnmController[mSelectAnmNum]->start();

                mState = 3;
                setReassignedFlag(true);

                for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
                    mOnPaneVibFrame[i] = 0.0f;
                    mOnPaneVibWaitFrame[i] = 0.0f;
                    getController(i)->stopMotor();
                    getController(i)->setEnableRumble(false);
                }

                mWaitStopMotorCount =
                    scWaitStopMotorTime /
                    getInstance()->getHBMDataInfo()->frameDelta;

                mpLayout->GetRootPane()
                    ->FindPaneByName(
                        scFuncTextPaneName[res::eFuncTextPane_T_msg_00], true)
                    ->SetVisible(true);

                mpLayout->GetRootPane()
                    ->FindPaneByName(
                        scFuncTextPaneName[res::eFuncTextPane_T_msg_01], true)
                    ->SetVisible(true);

                play_sound(HBM_SE_SELECT);
                play_sound(HBM_SE_START_CONNECT_WINDOW);

                break;
            }

            case res::eFuncTouchPane_B_BtnA: {
                mSelectAnmNum = findGroupAnimator(res::eGrPane_msgBtn_00_psh,
                                                  res::eGrAnim_cmn_msg_btn_psh);
                mpGroupAnmController[mSelectAnmNum]->start();

                mState = 13;
                mSequence = eSeq_Normal;

                if (mSelectBtnNum == HBM_SELECT_BTN1) {
                    play_sound(HBM_SE_GOTO_MENU);
                    mFader.mBlackOutFlag = false;
                } else if (mSelectBtnNum == HBM_SELECT_BTN2) {
                    play_sound(HBM_SE_RESET_APP);
                    mFader.mBlackOutFlag = true;
                } else if (mSelectBtnNum == HBM_SELECT_BTN4) {
                    play_sound(HBM_SE_RESET_APP);
                    mFader.mBlackOutFlag = false;
                }

                break;
            }

            case res::eFuncTouchPane_B_BtnB: {
                mSelectAnmNum = findGroupAnimator(res::eGrPane_msgBtn_01_psh,
                                                  res::eGrAnim_cmn_msg_btn_psh);
                mpGroupAnmController[mSelectAnmNum]->start();

                mState = 13;
                mSelectBtnNum = HBM_SELECT_NULL;

                mSequence = eSeq_Normal;
                play_sound(HBM_SE_CANCEL);

                break;
            }
            }
    }
}

void HomeButton::reset_btn() {
    int idx;

    for (int i = 0; i < mButtonNum; i++) {
        if (mPaneCounter[i] == 0) {
            continue;
        }

        idx = findAnimator(i, res::eAnim_cntBtn_out);
        mpAnmController[idx]->start();

        mPaneCounter[i] = 0;
    }

    if (mPaneCounter[mButtonNum + res::eFuncTouchPane_B_btn_00] != 0) {
        mpPairGroupAnmController[res::ePairAnm_hmMenu_bar_out]->start();
        mPaneCounter[mButtonNum + res::eFuncTouchPane_B_bar_10] = 0;
    }

    if (mPaneCounter[mButtonNum + res::eFuncTouchPane_B_bar_10] != 0) {
        if (mSequence == eSeq_Control) {
            if (mMsgCount == 0) {
                idx = findGroupAnimator(res::eGrPane_optn_bar_out,
                                        res::eGrAnim_close_bar_out);
                mpGroupAnmController[idx]->start();

                mPaneCounter[mButtonNum + res::eFuncTouchPane_B_btn_00] = 0;
            }
        } else {
            idx = findGroupAnimator(res::eGrPane_optn_bar_out,
                                    res::eGrAnim_optn_bar_out);
            mpGroupAnmController[idx]->start();

            mPaneCounter[mButtonNum + res::eFuncTouchPane_B_btn_00] = 0;
        }
    }
}

inline void HomeButton::reset_control() {
    int idx;

    for (int i = 0;
         i < res::eGrPane_optnBtn_20_inOut - res::eGrPane_optnBtn_00_inOut + 1;
         i++) {

        idx = findGroupAnimator(i + res::eGrPane_optnBtn_00_inOut,
                                res::eGrAnim_optn_btn_out);
        mpGroupAnmController[idx]->start();
    }
}

inline void HomeButton::reset_window() {
    int idx;

    idx = findGroupAnimator(res::eGrPane_msgBtn_00_inOut,
                            res::eGrAnim_cmn_msg_btn_out);
    mpGroupAnmController[idx]->start();

    idx = findGroupAnimator(res::eGrPane_msgBtn_01_inOut,
                            res::eGrAnim_cmn_msg_btn_out);
    mpGroupAnmController[idx]->start();
}

inline void HomeButton::reset_battery(bool startPair) {
    const char* const* pane;
    const char* const* row = &scBatteryPaneName[0][0];

    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        pane = row;

        for (int j = 0; j < res::eBatteryPane_Max; j++) {
            mpLayout->GetRootPane()
                ->FindPaneByName(*pane, true)
                ->SetVisible(false);
            pane++;
        }

        row += res::eBatteryPane_Max;
    }

    if (startPair) {
        mSelectAnmNum = res::ePairAnm_link_msg_in;
        mpPairGroupAnmController[mSelectAnmNum]->start();
    }
}

inline void HomeButton::reset_guiManager(int num) {
    if (num < 0) {
        for (int i = 0; i < 8; i++) {
            mpPaneManager->update(i, -10000.0f, -10000.0f, 0, 0, 0, NULL);
        }
    } else {
        mpPaneManager->update(num, -10000.0f, -10000.0f, 0, 0, 0, NULL);
    }
}

inline bool HomeButton::isUpBarActive() const {
    bool flag = true;

    if (!isActive() ||
        mpPairGroupAnmController[res::ePairAnm_hmMenu_bar_out]->isPlaying() ||
        mpPairGroupAnmController[res::ePairAnm_hmMenu_bar_in]->isPlaying()) {

        flag = false;
    }

    return flag;
}

inline bool HomeButton::isDownBarActive() {
    bool flag = true;

    // clang-format off
    int idx[4];
    idx[0] = findGroupAnimator(res::eGrPane_optn_bar_in,  res::eGrAnim_optn_bar_in);
    idx[1] = findGroupAnimator(res::eGrPane_optn_bar_out, res::eGrAnim_optn_bar_out);
    idx[2] = findGroupAnimator(res::eGrPane_optn_bar_in,  res::eGrAnim_close_bar_in);
    idx[3] = findGroupAnimator(res::eGrPane_optn_bar_out, res::eGrAnim_close_bar_out);
    // clang-format on

    if (!isActive() || //
        mpGroupAnmController[idx[0]]->isPlaying() ||
        mpGroupAnmController[idx[1]]->isPlaying() ||
        mpGroupAnmController[idx[2]]->isPlaying() ||
        mpGroupAnmController[idx[3]]->isPlaying()) {

        flag = false;
    }

    return flag;
}

int HomeButton::getPaneNo(const nw4hbm::lyt::Pane* pPane) {
    int ret = -1;
    const char* pName = pPane->GetName();

    for (int i = 0; i < mButtonNum; i++) {
        if (std::strcmp(pName, getPaneName(i)) == 0) {
            ret = i;
            break;
        }
    }

    for (int i = 0; i < res::eFuncTouchPane_Max; i++) {
        if (std::strcmp(pName, getFuncPaneName(i)) == 0) {
            ret = i + mButtonNum;
            break;
        }
    }

    return ret;
}

HBMSelectBtnNum HomeButton::getSelectBtnNum() {
    if (mState != 18) {
        return HBM_SELECT_NULL;
    }

    return mSelectBtnNum;
}

inline void HomeButton::setForcusSE() {
    if (mForcusSEWaitTime <= scForcusSEWaitTime) {
        return;
    }

    play_sound(HBM_SE_FOCUS);
    mForcusSEWaitTime = 0;
}

void HomeButton::setAdjustFlag(int flag) {
    nw4hbm::math::VEC2 scale;

    mAdjustFlag = static_cast<bool>(flag);
    mDrawInfo.SetLocationAdjust(mAdjustFlag);

    if (mAdjustFlag) {
        scale = nw4hbm::math::VEC2(mpHBInfo->adjust.x, mpHBInfo->adjust.y);
        mpLayout->GetRootPane()->SetScale(scale);

        if (!mpHBInfo->cursor) {
            for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
                mpCursorLayout[i]->GetRootPane()->SetScale(scale);
            }
        }
    } else {
        scale = nw4hbm::math::VEC2(1.0f, 1.0f);
        mpLayout->GetRootPane()->SetScale(scale);

        if (!mpHBInfo->cursor) {
            for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
                mpCursorLayout[i]->GetRootPane()->SetScale(scale);
            }
        }
    }
}

inline void HomeButton::setVolume(int vol) {
    // clang-format off
    WPADSetSpeakerVolume(vol *
        (WPAD_MAX_SPEAKER_VOLUME / static_cast<f32>(HBM_MAX_VOLUME)));
    // clang-format on
}

inline int HomeButton::getVolume() {
    // clang-format off
    return WPADGetSpeakerVolume() * (1.0f /
        (WPAD_MAX_SPEAKER_VOLUME / static_cast<f32>(HBM_MAX_VOLUME))) + 0.9f;
    // clang-format on
}

inline void HomeButton::setVibFlag(bool flag) {
    WPADEnableMotor(flag);
}

inline bool HomeButton::getVibFlag() {
    return WPADIsMotorEnabled();
}

void HomeButtonEventHandler::onEvent(u32 id, u32 event, void* pData) {
    gui::PaneComponent* pComponent =
        static_cast<gui::PaneComponent*>(mpManager->getComponent(id));

    const nw4hbm::lyt::Pane* pPane = pComponent->getPane();
    HomeButton* pHBObj = getHomeButton();
    HBController* pController = static_cast<HBController*>(pData);

    switch (event) {
    case gui::HBM_EVENT_ON_POINT: {
        pHBObj->startPointEvent(pPane, pData);
        break;
    }

    case gui::HBM_EVENT_OFF_POINT: {
        pHBObj->startLeftEvent(pPane);
        break;
    }

    case gui::HBM_EVENT_TRIG: {
        if (pController->trig & WPAD_BUTTON_A) {
            pHBObj->startTrigEvent(pPane);
        }

        break;
    }
    }
}

inline void HomeButton::startBlackOut() {
    if (mStartBlackOutFlag) {
        return;
    }

    mStartBlackOutFlag = true;

    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        mpController[i]->stopMotor();
    }

    mForceSttInitProcFlag = false;
    mForceSttFadeInProcFlag = false;
    mForceStopSyncFlag = false;
    mForceEndMsgAnmFlag = false;

    switch (mState) {
    case 0: {
        mForceSttInitProcFlag = true;
        // FALLTHROUGH
    }

    case 1: {
        mForceSttFadeInProcFlag = true;
        break;
    }

    case 3:
    case 5:
    case 6:
    case 7: {
        if ((mState == 3 && mSelectAnmNum == 5) ||
            (mState == 5 && !mSimpleSyncFlag)) {
            OSCancelAlarm(&mSimpleSyncAlarm);
            WPADSetSimpleSyncCallback(mSimpleSyncCallback);

        } else if (!mEndSimpleSyncFlag && mState > 3) {
            mForceStopSyncFlag = true;

            if (!WPADStopSimpleSync()) {
                OSCancelAlarm(&mSimpleSyncAlarm);

                OSSetAlarmUserData(&mSimpleSyncAlarm,
                                   reinterpret_cast<void*>(eSyncType_Stop));

                OSSetAlarm(&mSimpleSyncAlarm, OS_MSEC_TO_TICKS(100),
                           &RetrySimpleSyncCallback);
            }
        } else {
            WPADSetSimpleSyncCallback(mSimpleSyncCallback);
        }

        mForceEndMsgAnmFlag = true;
        break;
    }

    default: {
        break;
    }
    }

    mState = 19;
    mFader.start();

    mSelectBtnNum = HBM_SELECT_BTN2;

    f32 maxFrame = mFader.getMaxFrame();
    mFadeOutSeTime = maxFrame;

    if (mpHBInfo->sound_callback != NULL) {
        mpHBInfo->sound_callback(HBM_SOUND_RETURN_APP, maxFrame);
    }
}

inline void HomeButton::BlackFader::calc() {
    if (state_ == 1)
        frame_++;
    else if (state_ == 2)
        frame_--;

    if (frame_ < 0)
        frame_ = 0;
    else if (frame_ > maxFrame_)
        frame_ = maxFrame_;
}

inline bool HomeButton::BlackFader::isDone() {
    if (state_ == 1) {
        if (frame_ == maxFrame_)
            return true;
    }

    if (state_ == 2) {
        if (frame_ == 0)
            return true;
    }

    return false;
}

static void initgx() {
    Mtx view_mtx;

    GXSetCullMode(GX_CULL_NONE);

    PSMTXIdentity(view_mtx);
    GXLoadPosMtxImm(view_mtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);

    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGB, GX_F32, 0);

    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL,
                  GX_DF_NONE, GX_AF_NONE);
    GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL,
                  GX_DF_NONE, GX_AF_NONE);

    GXSetNumTexGens(0);

    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
                  GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
                    GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
                    GX_TRUE, GX_TEVPREV);

    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetAlphaUpdate(GX_FALSE);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetDispCopyGamma(0);

    GXSetNumIndStages(0);
    GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE,
                          GX_CH_ALPHA);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
}

static void drawBlackPlate(f32 left, f32 top, f32 right, f32 bottom,
                           GXColor clr) {
    GXSetTevColor(GX_TEVREG0, clr);

    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    {
        GXPosition2f32(left, top);
        GXPosition2f32(left, bottom);
        GXPosition2f32(right, bottom);
        GXPosition2f32(right, top);
    }
    GXEnd();
}

#pragma push
#pragma inline_max_size(10000)
#pragma inline_max_total_size(10000)
__inline void HomeButton::BlackFader::draw() {
    u8 alpha = frame_ * 255 / maxFrame_;

    initgx();
    drawBlackPlate(-1000.0f, -1000.0f, 1000.0f, 1000.0f,
                   mBlackOutFlag
                       ? (GXColor){red_, green_, blue_, alpha}
                       : (GXColor){0, 0, 0, alpha});
}
#pragma pop

const int HomeButton::scSoundHeapSize_but2 = 0x60000;
const int HomeButton::scSoundHeapSize_but3 = 0x6f800;
const int HomeButton::scSoundThreadPrio = 4;
const int HomeButton::scDvdThreadPrio = 3;

void HomeButton::update_sound() {
    AxSoundMain();

    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++)
        mpController[i]->updateSound();
}

inline void HomeButton::fadeout_sound(f32 gain) {
    if (mSelectBtnNum != HBM_SELECT_BTN3) {
        if (mEndInitSoundFlag) {
            AXSetAuxAReturnVolume(static_cast<u16>(gain * AX_MAX_VOLUME));
        }

        SetVolumeAllSeq(gain);
    }

    /*
    if (mpSoundArchivePlayer != NULL) {
        nw4hbm::ut::detail::AutoLock<OSMutex> lock(sMutex);

        for (int i = 0; i < mpSoundArchivePlayer->GetSoundPlayerCount(); i++) {
            mpSoundArchivePlayer->GetSoundPlayer(i).SetVolume(gain);
        }
    }
    */
}

} // namespace homebutton

/******************************************************************************
 *
 * nw4hbm::lyt::ArcResourceAccessor
 *
 * The destructor is inline in lyt_arcResourceAccessor.h (retail keeps a
 * strong out-of-line copy in this TU, which the linker prefers over the
 * weak copy emitted with the vtable in lyt_arcResourceAccessor.o).
 *
 ******************************************************************************/

namespace nw4hbm {
namespace lyt {

} // namespace lyt
} // namespace nw4hbm

/******************************************************************************
 * Retail .data tail (+0xCD0): 0x20 zero pad followed by the gui::Interface
 * typeinfo name string (27 chars + NUL, zero-filled to 0x28). Defined here so
 * the bytes land at the end of this TU's .data exactly like retail.
 ******************************************************************************/
// Retail tail from 0xC50 onward: 6 symbols that were previously mis-ordered.
// Decomp had pad(0x20) + gui::Interface(0x28) at 0xC50/0xC70 with extra vtables
// at 0xC98/0xCE0, while retail has the sequence below at 0xC50..0xD18.
// Defining them explicitly in retail order makes the bytes and symbols match.
extern "C" __declspec(section ".data") const unsigned char lbl_8054D4F0[0x10] = {0};
extern "C" __declspec(section ".data") char lbl_8054D500[0x23] = "homebutton::HomeButtonEventHandler";
extern "C" __declspec(section ".data") const unsigned char lbl_8054D524[0xC] = {0};
extern "C" __declspec(section ".data") const unsigned char lbl_8054D530[0x18] = {0};
extern "C" __declspec(section ".data") char lbl_8054D548[0x1E] = "homebutton::gui::EventHandler";
extern "C" __declspec(section ".data") const unsigned char lbl_8054D568[0x50] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x68, 0x6F, 0x6D, 0x65, 0x62, 0x75, 0x74, 0x74,
    0x6F, 0x6E, 0x3A, 0x3A, 0x67, 0x75, 0x69, 0x3A,
    0x3A, 0x49, 0x6E, 0x74, 0x65, 0x72, 0x66, 0x61,
    0x63, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
