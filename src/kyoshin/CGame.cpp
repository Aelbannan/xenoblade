#include "kyoshin/CGame.hpp"
#include "kyoshin/cf/CTaskREvent.hpp"
#include "kyoshin/cf/CBattleManager.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/lib.hpp"
#include "monolib/core.hpp"
#include "monolib/device.hpp"
#include "monolib/work.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "monolib/lib/CLibStaticData.hpp"
#include "decomp.h"
#include <cstring>
#include <revolution/GX.h>
#include <revolution/VI.h>
#include <revolution/WPAD.h>

using namespace ml;

extern void func_801BF93C();
extern float func_801C0014();
extern void func_801BFFAC(float f1, float f2);
extern void func_801644BC(u32 value);
extern void func_80044FBC(u32 value);

// Non-vararg sink avoids crclr (varargs float ABI) so five pool strings fit in
// 0x1C. Postprocess drops this FORCEACTIVE from .text (CGame.o drop_text_symbols)
// after it packs @stringBase0 — stubs already fill the split to 0xD08.
void force_cgame_strings(const char*, const char*, const char*, const char*, const char*);
void FORCEACTIVECGame_cpp_wkStandbyLogin(void);
void FORCEACTIVECGame_cpp_wkStandbyLogin(void) {
    force_cgame_strings("CGameRestart", "", "43", "arc", "4_3mode.brlyt");
}


CGame* CGame::spInstance;
static FixStr<64> lbl_80573C80;
nw4r::lyt::Layout* CGame::lbl_80666604;
nw4r::lyt::ArcResourceAccessor* CGame::sArcResourceAccessor;
const char* CGame::scViewName = "巨神"; //"Bionis"
CGameRestart* CGameRestart::spInstance;

CGame::CGame(const char* pName, CWorkThread* pParent) :
    CProc(pName, pParent, MAX_CHILD),
    mView(nullptr),
    mShutdownState(SHUTDOWN_STATE_0),
    mSceneReqId(-1),
    mSceneReqId2(-1),
    mSceneReqFlag(0),
    unk1FC(),
    mTaskManUpdateCount(1),
    mPrevBgmSpeed(1.0f),
    mPauseRefCount(0) {
    spInstance = this;
    CLibHbm::func_8045D5C8(1);
    CWorkSystem::setExitFunc(&onExit);
    wkSetEvent(EVT_4);
    CDeviceVI::isTvFormatPal();
    mLetterboxBorder = 57;
}

CGame::~CGame() {
    CWorkSystem::setExitFunc(0);
    CLibHbm::func_8045D5C8(0);
    spInstance = 0;
}

CGame* CGame::getInstance() {
    return spInstance;
}

bool CGame::func_8003933C() {
    return func_80164910() == 0;
}

void CGame::func_80039364() {
    if (spInstance == nullptr) {
        GameMain();
    } else if (CGameRestart::spInstance == nullptr) {
        CGameRestart* gameRestart = CGameRestart::create("CGameRestart", CDesktop::getInstance());

        if (gameRestart != nullptr) {
            gameRestart->mHandle = spInstance->mWorkID;
            spInstance->wkSetEvent(EVT_NONE);
        }
    }
}

void CGame::setTaskManagerUpdateCount(u32 count) {
    if (spInstance != nullptr) {
        spInstance->mTaskManUpdateCount = count;
    }
}

// Dispatch pending scene transitions through CTaskGame, then run the task
// manager update loop mTaskManUpdateCount times per frame.
// Scene requests arrive via external setters (mSceneReqId/mSceneReqId2).
void CGame::wkUpdate() {
    if ((s16)mSceneReqId >= 0 && CTaskGame::getInstance() != nullptr) {
        if (unk1FC.size() == 0) {
            CTaskGame::getInstance()->func_80040A3C(mSceneReqId, mSceneReqId2, nullptr, mSceneReqFlag);
        } else {
            CTaskGame::getInstance()->func_80040A3C(mSceneReqId, mSceneReqId2, unk1FC.c_str(), mSceneReqFlag);
        }

        mSceneReqId = -1;
        mSceneReqId2 = -1;
        // Preserve the retail string-pool reference to the terminator after
        // "CGameRestart"; a plain "" resolves to a different pool entry.
        unk1FC = "CGameRestart" + 13;
        mSceneReqFlag = 0;
    }

    if (isNoEvent() && CTaskGame::getInstance() != nullptr) {
        // The repeated singleton lookup is part of the retail call schedule.
        (void)CTaskGame::getInstance();
        if (CTaskGame::func_800426F0() == false) {
            CTaskGame::getInstance()->func_80042720();
        }
    }

    for (u32 i = 0; i < mTaskManUpdateCount; i++) {
        CTaskManager::Move();
    }
}

// Animate and draw the 4:3 letterbox overlay (when active), then dispatch
// per-task rendering. The overlay is only rendered in non-widescreen mode.
void CGame::wkRender() {
    if (lbl_80666604 != nullptr) {
        lbl_80666604->Animate(0);
    }

    // Draw the 4:3 presentation overlay before dispatching task rendering.
    if (lbl_80666604 != nullptr) {
        if (!CDeviceSC::isWideAspectRatio()) {
            CDeviceGX::getCacheInstance()->func_8044BE38();
            GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
            nw4r::lyt::DrawInfo drawInfo;
            func_80137250(&drawInfo);
            func_80137038(lbl_80666604, &drawInfo, 0, 1);
            CViewRoot::func_80442DA8();
        }
    }

    CTaskManager::Draw();
}

// Reconfigure the rendering viewport for the current aspect ratio.
// In 4:3 (wide==false), letterbox the viewport by subtracting 2*mLetterboxBorder
// scanlines from efbHeight and offsetting Y by mLetterboxBorder-1.
// In 16:9 (wide==true), use the full framebuffer dimensions.
// @param wide true = 16:9 content, false = 4:3 content with letterbox borders
void CGame::func_800395F4(bool wide) {
    CGame* self;

    self = spInstance;
    if (self == nullptr) {
        return;
    }
    if (self->mView == nullptr) {
        return;
    }

    if (!wide) {
        // Letterbox: visible height = efbHeight - 2*mLetterboxBorder.
        // mLetterboxBorder is shifted left by 1 (= *2) to account for both top and bottom.
        s32 height = (s16)((u16)CDeviceVI::getRenderModeObj()->efbHeight
            - ((u32)(u16)spInstance->mLetterboxBorder << 1));
        setViewRect(self->mView, 0, (s16)((u16)self->mLetterboxBorder - 1),
            CDeviceVI::getRenderModeObj()->fbWidth, (s16)height);
    } else {
        // Full 16:9 viewport; using spInstance directly to match retail regalloc.
        s16 height = CDeviceVI::getRenderModeObj()->efbHeight;
        setViewRect(spInstance->mView, 0, 0,
            CDeviceVI::getRenderModeObj()->fbWidth, height);
    }
}

void CGame::setViewRect(CView* view, s16 x, s16 y, s16 width, s16 height) {
    view->setRect(ml::CRect16(x, y, width, height));
}

// Initialize the full CGame presentation layer during standby login.
// Creates the Bionis view, configures letterboxing/4:3 overlay,
// bootstraps CTaskGame, arms controller dimming, and loads the
// 4:3 border layout from the static file archive.
// @return true on success, false if CLibStaticData not yet initialized
bool CGame::wkStandbyLogin() {
    StaticDataHandle handle;

    if (!CLibStaticData::isInitialized()) {
        return false;
    }

    {
        CView* view = pssCreateView(scViewName, CDesktop::getView(), 0);
        mView = view;
        const char* viewName = scViewName;
        view->mName = viewName;
        if (static_cast<CWorkThread*>(view)->mName.size() == 0) {
            static_cast<CWorkThread*>(view)->mName = viewName;
        }
    }

    if (CDeviceSC::isWideAspectRatio()) {
        setViewRect(mView, 0, 0,
            CDeviceVI::getRenderModeObj()->fbWidth,
            CDeviceVI::getRenderModeObj()->efbHeight);
    } else {
        setViewRect(mView, 0,
            (s16)((u16)mLetterboxBorder - 1),
            CDeviceVI::getRenderModeObj()->fbWidth,
            (s16)((u16)CDeviceVI::getRenderModeObj()->efbHeight
                - ((u32)(u16)mLetterboxBorder << 1)));
    }

    mView->unk444 = CVec4(0.0f, 0.0f, 0.0f, 1.0f);

    func_800395F4(CDeviceSC::isWideAspectRatio());
    CDeviceGX::updateVerticalFilter(VFILTER_NONE);
    CTaskManager::Reset();
    CTaskGame::create(mView, this, 1);
    WPADSetAutoSleepTime(5); // auto-sleep after 5 min of inactivity
    VIEnableDimming(1); // enable VI dimming for controller disconnect
    VISetTimeToDimming(1); // dim after 1 frame of no input

    // +14: "CGameRestart" after null → same string reused as resource filename
    if (CLibStaticData::getStaticFileData("CGameRestart" + 14, &handle, nullptr)) {
        sArcResourceAccessor = CLibLayout::createArcResourceAccessor();
        // +17: points past "CGameRestart".\0" → archive key within the ARC
        sArcResourceAccessor->Attach(handle.data, "CGameRestart" + 17);
        // +21: points past "CGameRestart".\0"arc" → "4_3mode.brlyt" layout name
        func_80136E84(&lbl_80666604, sArcResourceAccessor, "CGameRestart" + 21);
    }

    return CProc::wkStandbyLogin();
}

// Phased shutdown of the CGame subsystem.
// Phase 0: signal CTaskGame to begin teardown.
// Phase 1: wait for CTaskGame's unk68 bit 4 (0x10) to confirm readiness.
// Phase 2: reset task manager and free overlay resources once children exit.
// @return true when fully torn down, false while waiting for phases
bool CGame::wkStandbyLogout() {
    if (mShutdownState == SHUTDOWN_STATE_0) {
        CTaskGame::getInstance()->func_80042710();
        mShutdownState = SHUTDOWN_STATE_1;
    }

    if (mShutdownState == SHUTDOWN_STATE_1) {
        if (!(CTaskGame::getInstance()->unk68 & 0x10)) {
            return false;
        }
        mShutdownState = SHUTDOWN_STATE_2;
    }

    // All child work threads must have exited before freeing resources.
    if (mChildren.empty()) {
        CTaskManager::Reset();

        if (sArcResourceAccessor != nullptr) {
            delete sArcResourceAccessor;
            sArcResourceAccessor = nullptr;
        }

        if (lbl_80666604 != nullptr) {
            delete lbl_80666604;
            lbl_80666604 = nullptr;
        }

        return CProc::wkStandbyLogout();
    }

    return false;
}

// Keep the ArcResourceAccessor destructor instantiated in this translation unit.
static void dummy() {
    nw4r::lyt::ArcResourceAccessor* accessor = new nw4r::lyt::ArcResourceAccessor();
    accessor->Attach(0, 0);
    delete accessor;
}

void CGame::GameMain() {
    if (spInstance != nullptr) {
        spInstance->pssSetFocus();
    } else {
        create("CGame", CDesktop::getInstance(), CDesktop::getView()->mWorkID);
    }
}

// Register an exception entry for a controller-related error (e.g. disconnect).
// Creates a CException attached to the CGame work thread if the game is in a
// state that can handle it (not in a no-event state, no existing exception lock).
// @param message  Wide-character error description
// @param handler  Callback interface invoked during exception retry
// @param param    Opaque parameter forwarded to the handler
void CGame::registerControllerErrorEntry(const wchar_t* message, IGameException* handler, u32 param) {
    if (spInstance != nullptr && CTaskGame::func_800426F0() == nullptr && !spInstance->isNoEvent()) {
        CException* exception = CException::func_80457CA4(spInstance, message, 5);
        if (exception != nullptr) {
            exception->mException = handler;
            exception->unk204 = param;
        }
    }
}

// Retry handler for controller exceptions (e.g. reconnection after disconnect).
// Resolves the exception by work thread ID, checks retry readiness via the
// exception state machine, and dispatches to the stored handler callback.
// @param wid  Work thread ID of the exception source
// @return  true if exception resolved or no retry needed, false if still pending
bool CGame::wkStandbyExceptionRetry(u32 wid) {
    if (isNoEvent()) {
        return true;
    }
    if (CLibHbm::func_8045DE00()) {
        return false;
    }

    CWorkThread* workThread = CWorkThread::getWorkThread(wid);
    CException* exception = CException::convertToException(workThread);

    if (exception == nullptr) {
        return true;
    }
    if (exception->func_80457C8C() == false) {
        return false;
    }
    if (exception->mException == nullptr) {
        return true;
    }

    IGameException* handler = exception->mException;
    return handler->gameExceptionCB(exception->unk204);
}

// Handle game pause/resume triggered by controller events (Start button).
// On first pause: snap BGM speed, mute audio, disable battle vision effects.
// On last resume: restore BGM speed, re-enable audio and vision.
// Uses reference counting to correctly handle nested pause/resume sequences.
// @param paused  true = enter pause, false = exit pause
void CGame::OnPauseTrigger(bool paused) {
    if (cf::CfGameManager::func_8007E1B4()) {
        if (paused) {
            if (mPauseRefCount == 0) {
                mPrevBgmSpeed = func_801C0014();
                func_801BFFAC(0, 0);
                func_801644BC(1);

                if (cf::CBattleManager::getInstance() != nullptr) {
                    cf::CBattleManager* battleManager = cf::CBattleManager::getInstance();
                    battleManager->mVision.func_801A929C(1);
                }

                func_80044FBC(1);
            }

            mPauseRefCount++;
        } else {
            // <=1 catches the last resume (count drops to 0 after decrement)
            if (mPauseRefCount <= 1) {
                func_801BFFAC(mPrevBgmSpeed, 0);
                func_801644BC(0);

                if (cf::CBattleManager::getInstance() != nullptr) {
                    cf::CBattleManager* battleManager = cf::CBattleManager::getInstance();
                    battleManager->mVision.func_801A929C(0);
                }

                func_80044FBC(0);
            }

            mPauseRefCount--;
            if (mPauseRefCount < 0) {
                mPauseRefCount = 0;
            }
        }
    }
}

void CGame::onExit() {
    if (spInstance != nullptr) {
        if (cf::CfGameManager::func_8007E1B4()) {
            func_801BF93C();
        }
    }
}

// Retail emits IWorkEvent defaults as weak into CGame.s (descending WorkEvent31..1
// after onExit). IWorkEvent.cpp is not a link unit for these.
void IWorkEvent::WorkEvent31() {}

bool IWorkEvent::WorkEvent30() {
    return false;
}

bool IWorkEvent::WorkEvent29() {
    return false;
}

bool IWorkEvent::WorkEvent28() {
    return false;
}

bool IWorkEvent::WorkEvent27() {
    return false;
}

bool IWorkEvent::WorkEvent26() {
    return false;
}

bool IWorkEvent::WorkEvent25() {
    return false;
}

bool IWorkEvent::WorkEvent24() {
    return false;
}

bool IWorkEvent::WorkEvent23() {
    return false;
}

bool IWorkEvent::WorkEvent22() {
    return false;
}

bool IWorkEvent::WorkEvent21() {
    return false;
}

bool IWorkEvent::WorkEvent20() {
    return false;
}

bool IWorkEvent::WorkEvent19() {
    return false;
}

bool IWorkEvent::WorkEvent18() {
    return false;
}

bool IWorkEvent::WorkEvent17() {
    return false;
}

bool IWorkEvent::WorkEvent16() {
    return false;
}

bool IWorkEvent::WorkEvent15() {
    return false;
}

bool IWorkEvent::WorkEvent14() {
    return false;
}

bool IWorkEvent::WorkEvent13() {
    return false;
}

bool IWorkEvent::WorkEvent12() {
    return false;
}

bool IWorkEvent::WorkEvent11() {
    return false;
}

bool IWorkEvent::WorkEvent10() {
    return false;
}

bool IWorkEvent::WorkEvent9() {
    return false;
}

bool IWorkEvent::WorkEvent8() {
    return false;
}

bool IWorkEvent::WorkEvent7() {
    return false;
}

bool IWorkEvent::WorkEvent6() {
    return false;
}

void IWorkEvent::OnPauseTrigger(bool /*paused*/) {}

bool IWorkEvent::WorkEvent4() {
    return false;
}

bool IWorkEvent::WorkEvent3(UNKTYPE* /*r4*/) {
    return false;
}

bool IWorkEvent::OnFileEvent(CEventFile* /*pEventFile*/) {
    return false;
}

bool IWorkEvent::WorkEvent1(UNKTYPE* /*r4*/, const char* /*r5*/) {
    return false;
}