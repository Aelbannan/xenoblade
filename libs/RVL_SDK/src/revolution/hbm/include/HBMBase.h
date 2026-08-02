#ifndef HOME_BUTTON_MINI_LIB_BASE_H
#define HOME_BUTTON_MINI_LIB_BASE_H
#include <HBMGUIManager.h>
#include <HBMRes.h>
#include <revolution/HBM/HBMTypes.h>

#include <nw4hbm/lyt.h>
#include <nw4hbm/ut.h>

#include <revolution/AXFX.h>
#include <revolution/AX.h>
#include <revolution/WPAD.h>

#define HBM_MAX_VOLUME 10

namespace homebutton {

// Forward declarations
class HomeButton;
class Controller;
class GroupAnmController;
class RemoteSpk;

/******************************************************************************
 *
 * HomeButtonEventHandler
 *
 ******************************************************************************/
class HomeButtonEventHandler : public gui::EventHandler {
public:
    HomeButtonEventHandler(homebutton::HomeButton* pHomeButton)
        : mpHomeButton(pHomeButton) {}

    virtual void onEvent(u32 ud, u32 event, void* pData); // at 0x8

    homebutton::HomeButton* getHomeButton() {
        return mpHomeButton;
    }

private:
    HomeButton* mpHomeButton; // at 0x8
};

/******************************************************************************
 *
 * HomeButton
 *
 ******************************************************************************/
class HomeButton {
public:
    enum eSyncType {
        eSyncType_Start,
        eSyncType_Stop,
    };

private:
    enum eSeq {
        eSeq_Normal,
        eSeq_Control,
        eSeq_Cmn,
    };

private:
    class BlackFader {
    public:
        BlackFader(int maxFrame) {
            init(maxFrame);
            setColor(0, 0, 0);
        }

        int getFrame() const {
            return frame_;
        }
        int getMaxFrame() const {
            return maxFrame_;
        }

        void setColor(u8 r, u8 g, u8 b) {
            red_ = r;
            green_ = g;
            blue_ = b;
        }

        bool isDone();

        void init(int maxFrame);
        void calc();
        void draw();

        void start() {
            state_ = 1;
        }

    private:
        int frame_;    // at 0x0
        int maxFrame_; // at 0x4
        int state_;    // at 0x8
        u8 red_;       // at 0xD
        u8 green_;     // at 0xE
        u8 blue_;      // at 0xF
    };

public:
    static void createInstance(const HBMDataInfo* pDataInfo);
    static HomeButton* getInstance();
    static void deleteInstance();

    HomeButton(const HBMDataInfo* pDataInfo);
    ~HomeButton();

    const HBMDataInfo* getHBMDataInfo() {
        return mpHBInfo;
    }
    Controller* getController(int chan) {
        return mpController[chan];
    }
    int getVolume();
    HBMSelectBtnNum getSelectBtnNum();
    const char* getFuncPaneName(int no) {
        return scFuncTouchPaneName[no];
    }
    const char* getPaneName(int no) {
        return scBtnName[no];
    }
    bool getReassignedFlag() const {
        return mReassignedFlag;
    }
    HomeButtonEventHandler* getEventHandler() const {
        return mpHomeButtonEventHandler;
    }
    bool isActive() const;
    bool isUpBarActive() const;
    bool isDownBarActive();

    void setAdjustFlag(int flag);
    void setEndSimpleSyncFlag(bool flag) {
        mEndSimpleSyncFlag = flag;
    }
    void setForcusSE();
    void setReassignedFlag(bool flag) {
        mReassignedFlag = flag;
    }
    /*
    nw4hbm::snd::SoundArchivePlayer* getSoundArchivePlayer() const {
        return mpSoundArchivePlayer;
    }
    */
    void setSimpleSyncAlarm(int type);
    void setSimpleSyncFlag(bool flag) {
        mSimpleSyncFlag = flag;
    }
    void setSpeakerAlarm(int chan, int msec);
    void setVolume(int vol);

    bool getVibFlag();
    int getPaneNo(const nw4hbm::lyt::Pane* pPane);

    void setVibFlag(bool flag);

    void create();
    void init();
    void calc(const HBMControllerData* pController);
    void draw();
    void update(const HBMControllerData* pController);
    void updateTrigPane();

    void startPointEvent(const nw4hbm::lyt::Pane* pPane, void* pData);
    void startLeftEvent(const nw4hbm::lyt::Pane* pPane);
    void startTrigEvent(const nw4hbm::lyt::Pane* pPane);

    int findAnimator(int pane, int anm);
    int findGroupAnimator(int grPane, int grAnm);

    void callSimpleSyncCallback(s32 result, s32 num) {
        if (mSimpleSyncCallback != NULL) {
            mSimpleSyncCallback(result, num);
        }
    }

    void startBlackOut();
    void setBlackOutColor(u8 r, u8 g, u8 b) {
        mFader.setColor(r, g, b);
    }

private:
    void init_battery(const HBMControllerData* pController);
    void calc_battery(int chan);
    void reset_battery();

    void init_sound();
    void play_sound(int id);
    void fadeout_sound(f32 gain);

    void init_msg();
    void init_vib();
    void init_volume();

    void set_config();
    void set_text();

    void calc_fadeoutAnm();
    void calc_letter();

    void update_controller(int id);
    void update_posController(int id);

    void reset_btn();
    void reset_control();
    void reset_guiManager(int num);
    void reset_window();

public:
    void update_sound();

private:
    static HomeButton* spHomeButtonObj;
    static OSMutex sMutex;

    // TODO(kiwi) Document states
    // Member offsets below are the retail layout (verified against the
    // retail HBMBase object: ctor/calc/update/set_text/setAdjustFlag).
    eSeq mSequence;                                            // at 0x0
    const HBMDataInfo* mpHBInfo;                               // at 0x4
    int unk08;                                                 // at 0x8 (retail; zeroed in ctor, unused in this TU)
    int mButtonNum;                                            // at 0xC
    int mAnmNum;                                               // at 0x10
    int mState;                                                // at 0x14
    int mSelectAnmNum;                                         // at 0x18
    int mMsgCount;                                             // at 0x1C
    int mPaneCounter[res::eBtn_Max + res::eFuncTouchPane_Max]; // at 0x20
    int mPadDrawTime[WPAD_MAX_CONTROLLERS];                    // at 0x58
    int mForcusSEWaitTime;                                     // at 0x68
    int mBar0AnmRev;                                           // at 0x6C
    int mBar1AnmRev;                                           // at 0x70
    int mBar0AnmRevHold;                                       // at 0x74
    int mBar1AnmRevHold;                                       // at 0x78
    int mGetPadInfoTime;                                       // at 0x7C
    bool mControllerFlag[WPAD_MAX_CONTROLLERS];                // at 0x80
    int mVolumeNum;                                            // at 0x84
    int unk88;                                                 // at 0x88 (retail; connect-window channel, update()/calc())
    bool mVibFlag;                                             // at 0x8C
    bool mControlFlag;                                         // at 0x8D
    bool mLetterFlag;                                          // at 0x8E
    bool mAdjustFlag;                                          // at 0x8F
    bool mReassignedFlag;                                      // at 0x90
    bool mSimpleSyncFlag;                                      // at 0x91
    bool mEndSimpleSyncFlag;                                   // at 0x92
    bool unk93;                                                // at 0x93 (retail; "all controllers connected" latch)
    bool mInitFlag;                                            // at 0x94
    bool mForceSttInitProcFlag;                                // at 0x95
    bool mForceSttFadeInProcFlag;                              // at 0x96
    bool mEndInitSoundFlag;                                    // at 0x97
    bool mForceStopSyncFlag;                                   // at 0x98
    bool mForceEndMsgAnmFlag;                                  // at 0x99
    bool mStartBlackOutFlag;                                   // at 0x9A
    int mSoundRetryCnt;                                        // at 0x9C
    int mDialogFlag[4];                                        // at 0xA0
    char* mpLayoutName;                                        // at 0xB0
    char* mpAnmName;                                           // at 0xB4
    HBMSelectBtnNum mSelectBtnNum;                             // at 0xB8
    wchar_t* mpText[6][6];                                     // at 0xBC (retail set_text writes rows up to [9][5])
    WPADInfo mWpadInfo[WPAD_MAX_CONTROLLERS];                  // at 0x14C (retail keeps this as a static sWpadInfo)
    WPADSyncDeviceCallback mSimpleSyncCallback;                // at 0x1AC
    f32 mOnPaneVibFrame[WPAD_MAX_CONTROLLERS];                 // at 0x1B0
    f32 mOnPaneVibWaitFrame[WPAD_MAX_CONTROLLERS];             // at 0x1C0
    int mWaitStopMotorCount;                                   // at 0x1D0
    int mDisConnectCount;                                      // at 0x1D4
    nw4hbm::lyt::Layout* mpLayout;                             // at 0x1D8
    nw4hbm::lyt::Layout* mpCursorLayout[res::eCursorLyt_Max];  // at 0x1DC
    nw4hbm::lyt::ArcResourceAccessor* mpResAccessor;           // at 0x1EC
    gui::PaneManager* mpPaneManager;                           // at 0x1F0
    HomeButtonEventHandler* mpHomeButtonEventHandler;          // at 0x1F4
    nw4hbm::lyt::DrawInfo mDrawInfo;                           // at 0x1F8
    Controller* mpController[WPAD_MAX_CONTROLLERS];            // at 0x24C
    RemoteSpk* mpRemoteSpk;                                    // at 0x25C

    GroupAnmController* mpAnmController[res::eAnimator_Max];         // at 0x260
    GroupAnmController* mpGroupAnmController[res::eGrAnimator_Max];  // at 0x290
    GroupAnmController* mpPairGroupAnmController[res::ePairAnm_Max]; // at 0x3B8

    BlackFader mFader;                                     // at 0x3F4
    OSAlarm mAlarm[WPAD_MAX_CONTROLLERS];                  // at 0x408
    OSAlarm mSpeakerAlarm[WPAD_MAX_CONTROLLERS];           // at 0x4C8
    OSAlarm mSimpleSyncAlarm;                              // at 0x588
    int iReConnectTime;                                    // at 0x5B8
    int iReConnectTime2;                                   // at 0x5BC
    int mSoundRetryCount;                                  // at 0x5C0 (retail; sound-retry limit, init()/calc())
    u16 mAppVolume[4];                                     // at 0x5C4 (retail; AUX return volumes + 4th latch)
    AXFXAllocHook mAxFxAlloc;                              // at 0x5CC
    AXFXFreeHook mAxFxFree;                                // at 0x5D0
    AXFX_REVERBHI mAxFxReverb;                             // at 0x5D4
    AXAuxCallback mAuxCallback;                            // at 0x734
    void* mpAuxContext;                                    // at 0x738
    f32 mFadeOutSeTime;                                    // at 0x73C

private:
    static const int scSoundHeapSize_but2;
    static const int scSoundHeapSize_but3;
    static const int scSoundThreadPrio;
    static const int scDvdThreadPrio;

    static const int scReConnectTime;
    static const int scReConnectTime2;
    static const int scPadDrawWaitTime;
    static const int scGetPadInfoTime;
    static const int scForcusSEWaitTime;
    static const f32 scOnPaneVibTime;
    static const f32 scOnPaneVibWaitTime;
    static const int scWaitStopMotorTime;
    static const int scWaitDisConnectTime;

    static const char* scCursorLytName[res::eCursorLyt_Max];
    static const char* scCursorPaneName;
    static const char* scCursorRotPaneName;
    static const char* scCursorSRotPaneName;

    static const char* scBtnName[res::eBtn_Max];
    static const char* scTxtName[res::eTxt_Max];
    static const char* scGrName[res::eGroup_Max];
    static const char* scAnimName[res::eAnim_Max];

    static const char* scPairGroupAnimName[res::ePairAnm_Max];
    static const char* scPairGroupName[res::ePairAnm_Max];

    static const char* scGroupAnimName[res::eGrAnim_Max];
    static const char* scGroupName[res::eGrPane_Max];

    static const char* scFuncPaneName[res::eFuncPane_Max];
    static const char* scFuncTouchPaneName[res::eFuncTouchPane_Max];
    static const char* scFuncTextPaneName[res::eFuncTextPane_Max];

    static const char* scBatteryPaneName[WPAD_MAX_CONTROLLERS]
                                        [res::eBatteryPane_Max];
};

} // namespace homebutton

#endif
