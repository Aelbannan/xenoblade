#include "kyoshin/cf/CfPadTask.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/CGame.hpp"
#include "monolib/math.hpp"
#include "monolib/lib.hpp"
#include "monolib/work.hpp"
#include "monolib/device.hpp"
#include <revolution/KPAD.h>
#include <revolution/PAD.h>
#include <cstring>

extern "C" {
    extern u32 lbl_eu_80663E28;
    extern const float lbl_eu_80667EB0;
    extern const float lbl_eu_80667EA8; // 0.0f
    extern const float lbl_eu_80667EAC; // 0.3f
    extern const float lbl_eu_80667EB4; // 1.0f
    extern const float lbl_eu_80667EB8; // 0.7f
    const wchar_t* func_eu_802B1384();
    const wchar_t* func_eu_802B139C();
    const wchar_t* func_eu_802B13B4();
    const wchar_t* func_eu_802B13CC();
    // Retail symbols.txt recovers this as Fv (wrong mangling); call by that name for reloc match.
    void setPad__Q22cf13CfGameManagerFv(int r3, CPad* pPad, u32 r5);
    extern const float lbl_eu_8066A208;
    // Retail sbss singleton (config symbols.txt); mangled spInstance fails reloc name match.
    extern cf::CfPadTask* lbl_eu_80664448;
    // Retail sbss battery slot; mangled sWiimoteBattery fails reloc name match.
    extern u32 lbl_eu_80664444;
    // CTaskGame static gate (flat retail name; defined in CTaskGame.cpp). Declared
    // here instead of including CTaskGame.hpp: that header declares a minimal
    // CLibHbm which collides with the full CLibHbm.hpp already pulled in by
    // CfPadTask.hpp -> monolib/lib.hpp.
    s32 func_8004368C__9CTaskGameFv();
}

// The retail CfPadTask.o split object carries NO data sections — the class
// statics live in the retail data object (split1.s .sbss / .bss) and are
// referenced from this TU as plain (undefined) symbols with the exact retail
// mangled names (config symbols.txt). Not defining them here keeps this TU
// free of .sbss/.bss content; the linker resolves the references to the
// retail data object.

// --- CTTask<cf::CfPadTask> out-of-line specializations ---
// The canonical declared-only template emits no bodies; these explicit
// specializations produce the retail standalone Move/Draw/dtor symbols.
template<>
void CTTask<cf::CfPadTask>::Move() {
    if (mMoveFunc) {
        (static_cast<cf::CfPadTask*>(this)->*mMoveFunc)();
    }
}

template<>
void CTTask<cf::CfPadTask>::Draw() {
    if (mDrawFunc) {
        (static_cast<cf::CfPadTask*>(this)->*mDrawFunc)();
    }
}

template<>
CTTask<cf::CfPadTask>::~CTTask() {}

namespace cf{
    u32 CfPadTask::lbl_80666D3C;
    u32 CfPadTask::sMainPadType;
    float CfPadTask::sInputDisableTimer;
    float CfPadTask::sButtonDisableTimer;
    WPADInfo CfPadTask::sWpadInfo;

    CfPadTask::~CfPadTask(){
        lbl_80666D3C = PAD_TYPE_NONE;
        sMainPadType = PAD_TYPE_NONE;
        CLibHbm::removeCallback(this);
    }

void CfPadTask::Init(){
    mErrorFrameCount = 0;
    mNoErrorFrameCount = 0;
    lbl_eu_80664448 = this;
}

    void CfPadTask::Term(){
        CLibHbm::setCurrentWpadChannel(WPAD_CHAN_INVALID);
        lbl_eu_80664448 = nullptr;
    }

void CfPadTask::copyInputFlag(CPad* pPad, u32 srcFlag, u32 dstFlag){
        if(pPad->mPressedButtonFlags & srcFlag) pPad->mPressedButtonFlags |= dstFlag;
        else pPad->mPressedButtonFlags &= ~dstFlag;

        if(pPad->mTurboPressButtonFlags & srcFlag) pPad->mTurboPressButtonFlags |= dstFlag;
        else pPad->mTurboPressButtonFlags &= ~dstFlag;

        if(pPad->mReleasedButtonFlags & srcFlag) pPad->mReleasedButtonFlags |= dstFlag;
        else pPad->mReleasedButtonFlags &= ~dstFlag;

        if(pPad->mHeldButtonFlags & srcFlag) pPad->mHeldButtonFlags |= dstFlag;
        else pPad->mHeldButtonFlags &= ~dstFlag;

        if(pPad->mLongHoldButtonFlags & srcFlag) pPad->mLongHoldButtonFlags |= dstFlag;
        else pPad->mLongHoldButtonFlags &= ~dstFlag;

        if(pPad->mShortPressButtonFlags & srcFlag) pPad->mShortPressButtonFlags |= dstFlag;
        else pPad->mShortPressButtonFlags &= ~dstFlag;
    }

//Set the input disable timer (all inputs ignored until timer expires)
void CfPadTask::func_801C1B94(float f1){
    if(f1 < lbl_eu_8066A208) sInputDisableTimer = 0;
    else if(f1 > sInputDisableTimer) sInputDisableTimer = f1;
}

//Check if input is currently disabled
bool CfPadTask::func_801C1BC0(){
        return sInputDisableTimer > lbl_eu_8066A208;
    }

//Set the button disable timer (A/B/+- buttons ignored until timer expires)
void CfPadTask::func_801C1BD8(float f1) {
    if (f1 < lbl_eu_8066A208) {
        sButtonDisableTimer = lbl_eu_80667EA8;
    } else if (f1 > sButtonDisableTimer) {
        sButtonDisableTimer = f1;
    }
}

    void CfPadTask::updateCfPadData(CfPadData* padData, const CPad* pad){
        u32 prevHoldFlags = padData->mHeldButtonFlags;
        padData->mPrevHeldButtonFlags = prevHoldFlags;
        padData->mHeldButtonFlags = pad->mHeldButtonFlags;
        padData->mPad = *pad;

        //Override the default deadzone check for the left stick for whatever reason
        if(ml::math::abs(padData->mPad.mLStickXRaw) < lbl_eu_80667EB0){
            padData->mHeldButtonFlags &= ~(PAD_INPUT_FLAG_LSTICK_LEFT | PAD_INPUT_FLAG_LSTICK_RIGHT);
        }

        if(ml::math::abs(padData->mPad.mLStickYRaw) < lbl_eu_80667EB0){
            padData->mHeldButtonFlags &= ~(PAD_INPUT_FLAG_LSTICK_UP | PAD_INPUT_FLAG_LSTICK_DOWN);
        }

        //Enforce DPad / left-stick mutual exclusivity:
        //If the user is holding a DPad direction this frame, clear the stick directional flags
        //that were set by CPadManager (and vice versa), so inputs remain mutually exclusive.
        bool wasHoldingDpadButton = prevHoldFlags & PAD_INPUT_FLAG_DPAD;
        bool wasHoldingStickDir = prevHoldFlags & PAD_INPUT_FLAG_LSTICK;

        if(padData->mHeldButtonFlags & PAD_INPUT_FLAG_DPAD){
            if(wasHoldingStickDir) padData->mHeldButtonFlags &= ~PAD_INPUT_FLAG_DPAD;
            else padData->mHeldButtonFlags &= ~PAD_INPUT_FLAG_LSTICK;
        }else{
            if(wasHoldingDpadButton) padData->mHeldButtonFlags &= ~PAD_INPUT_FLAG_LSTICK;
            else padData->mHeldButtonFlags &= ~PAD_INPUT_FLAG_DPAD;
        }

        //Compute pressed/turbo flags from the frame-to-frame change in held state.
        //This duplicates logic already done in CPadManager::updatePadInputs.
        u32 changedFlags = prevHoldFlags ^ padData->mHeldButtonFlags;
        u32 pressedButtonFlags = padData->mHeldButtonFlags & changedFlags;
        padData->mPressedButtonFlags = pressedButtonFlags;
        padData->mTurboPressButtonFlags = pressedButtonFlags;

        u32 bit = 1;

        for(int i = 0; i < MAX_PAD_INPUT_FLAGS; i++){
            if(padData->mHeldButtonFlags & bit){
                if(padData->mPressedButtonFlags & bit){
                    padData->mButtonHoldTimersTurbo[i] = 0;
                }

                padData->mButtonHoldTimersTurbo[i]++;

                if(padData->mButtonHoldTimersTurbo[i] >= TURBO_HOLD_TIMER_THRESHOLD + TURBO_INPUT_FRAMES){
                    padData->mTurboPressButtonFlags |= bit;
                    padData->mButtonHoldTimersTurbo[i] -= TURBO_INPUT_FRAMES;
                }
            }

            bit <<= 1;
        }

        //Mask turbo flags against current held state (turbo should only fire for held buttons)
        padData->mTurboPressButtonFlags &= padData->mHeldButtonFlags;
    }

    void CfPadTask::Move(){
        mFrameCounter++;
        if(mFrameCounter > 0xFFFF) mFrameCounter = 0xFFFF;

        if(lbl_eu_80663E28 & (1u << 21)) return;

        int result = checkForControllerError(update());
        if(result != ERROR_NONE && !CLibHbm::isHbmControlInitialized() && !CWorkSystem::isOff() && !func_8004368C__9CTaskGameFv()
        && mFrameCounter > SECONDS_TO_FRAMES(2)){
            if(result == ERROR_WIIMOTE_DISCONNECTED && !CGame::func_8003933C()) return;

            mErrorFrameCount++;
            
            //If a controller error persists for more than 9 frames, trigger an error
            if(mErrorFrameCount > MAX_ERROR_FRAMES){
                if(result == ERROR_WIIMOTE_DISCONNECTED){
                    CGame::registerControllerErrorEntry(func_eu_802B1384(), this, 0);
                }else if(result == ERROR_NUNCHUCK_DISCONNECTED){
                    CGame::registerControllerErrorEntry(func_eu_802B139C(), this, 0);
                }else if(result == ERROR_CLASSIC_CONTROLLER_DISCONNECTED){
                    CGame::registerControllerErrorEntry(func_eu_802B13B4(), this, 0);
                }else if(result == ERROR_NO_EXTENSION){
                    CGame::registerControllerErrorEntry(func_eu_802B13CC(), this, 0);
                }

                mErrorFrameCount = 0;
            }
        }else{
            mErrorFrameCount = 0;
        }
    }

    bool CfPadTask::update(){
        bool hasActivePad = false;
        u32 systemType = PAD_SYSTEM_WII;
        u32 enabledInputFlags = CfGameManager::getEnabledInputFlags();
        u32 mainPadChannel = CfGameManager::getCurrentPadChannel();
        CDeviceRemotePad* remotePad = CDeviceRemotePad::getInstance();
        CPad* mainPad = CPadManager::getMainPad();

        if(mainPad->mPadType == PAD_TYPE_GC){
            sMainPadIsGCController = true;
            mainPad = nullptr;
        }

        sMainPadChannel = mainPadChannel;

        if(sInputDisableTimer > lbl_eu_80667EA8){
            enabledInputFlags = 0;
            sInputDisableTimer -= lbl_eu_80667EB4;
            if(sInputDisableTimer <= lbl_eu_80667EA8){
                sInputDisableTimer = lbl_eu_80667EA8;
            }
        }

        if(sButtonDisableTimer > lbl_eu_80667EA8){
            enabledInputFlags &= ~(PAD_INPUT_FLAG_CORE_A | PAD_INPUT_FLAG_CORE_B | PAD_INPUT_FLAG_1 | PAD_INPUT_FLAG_2 | PAD_INPUT_FLAG_PLUS
            | PAD_INPUT_FLAG_MINUS | PAD_INPUT_FLAG_CLASSIC_A | PAD_INPUT_FLAG_CLASSIC_B);
            sButtonDisableTimer -= lbl_eu_80667EB4;
            if(sButtonDisableTimer <= lbl_eu_80667EA8){
                sButtonDisableTimer = lbl_eu_80667EA8;
            }
        }

        if(mainPadChannel != WPAD_CHAN_INVALID){
            mainPad = CDeviceRemotePad::getPadData(mainPadChannel);
            //BUG: sWpadDisconnectTimer is not properly initialized beforehand
            if(mainPad != nullptr){ //0x16C
                sWpadDisconnectTimer++;
                if(sWpadDisconnectTimer % 32u == 0){
                    for(u32 i = 1; i < WPAD_MAX_CONTROLLERS; i++){
                        CPad* pad = CDeviceRemotePad::getPadData(i);
                        if(pad != nullptr && pad->mConnected){
                            WPADDisconnect(i);
                        }
                    }
                }

                if(sWpadDisconnectTimer == 0){
                    WPADGetInfoAsync(mainPad->mChannel, &sWpadInfo, &wpadGetInfoCallback);
                }
                
                CLibHbm::setCurrentWpadChannel(mainPadChannel);
            }
        }

        CfGameManager::setCurrentPadPtr(nullptr, WPAD_CHAN_INVALID);

        switch(sMainPadType){
            case PAD_TYPE_NONE:
            case PAD_TYPE_CORE:
                if(mainPad != nullptr && mainPad->mConnected){
                    if(mainPad->mPadType == PAD_TYPE_FS){
                        sMainPadType = PAD_TYPE_FS;
                        CPadManager::setRightStickDeadzoneDefault();
                    }else if(mainPad->mPadType == PAD_TYPE_CLASSIC){
                        sMainPadType = PAD_TYPE_CLASSIC;
                        CPadManager::setRightStickDeadzone(lbl_eu_80667EAC);
                    }else if(mainPad->mPadType == PAD_TYPE_CORE){
                        sMainPadType = PAD_TYPE_CORE;
                        CPadManager::setRightStickDeadzoneDefault();
                    }
                }
                break;
        }

        bool flag24 = lbl_eu_80663E28 & (1u << 24);
        if(flag24 && mainPad != nullptr && (sMainPadType >= PAD_TYPE_CORE && sMainPadType <= PAD_TYPE_CLASSIC)){
                if(mainPad->mConnected && (mainPad->mPadType >= PAD_TYPE_CORE && mainPad->mPadType <= PAD_TYPE_CLASSIC)){
                    sMainPadType = mainPad->mPadType;
                    if(sMainPadType == PAD_TYPE_CLASSIC){
                        CPadManager::setRightStickDeadzone(lbl_eu_80667EAC);
                    }else{
                        CPadManager::setRightStickDeadzoneDefault();
                    }
                }

            if(mainPad->mPadType >= PAD_TYPE_FS && mainPad->mPadType <= PAD_TYPE_CLASSIC){
                lbl_80666D3C = mainPad->mPadType;
            }else{
                lbl_80666D3C = PAD_TYPE_NONE;
            }
        }else if(!flag24 && (sMainPadType >= PAD_TYPE_FS && sMainPadType <= PAD_TYPE_CLASSIC) && lbl_80666D3C == PAD_TYPE_NONE){
            lbl_80666D3C = sMainPadType;
        }

        bool anyPadActive = false;

        for(u32 i = 0; i < TOTAL_CONTROLLERS; i++){
            //Exclude Wii controllers in ports 2-4
            if(i >= MAX_WII_CONTROLLERS && i < WPAD_MAX_CONTROLLERS) continue;
            //Skip over the channel for the current main controller
            if(mainPadChannel != WPAD_CHAN_INVALID && isWpadChannel(i) && mainPadChannel != i) continue;

            CPad newPad;
            CPad* pad = CDeviceRemotePad::getPadData(i);
            if(pad == nullptr) continue;
            newPad = *pad;

            u32 padType = newPad.mPadType;
            u32 result = 0;

            if(padType == sMainPadType || sMainPadType == PAD_TYPE_CORE || (padType == PAD_TYPE_CORE && !(lbl_eu_80663E28 & (1u << 28)))){
                result = 1;
            }

            if(!newPad.mConnected){
                result = 0;
            }

            if(isWpadChannel(i) && newPad.mConnected && padType != PAD_TYPE_NONE){
                systemType = PAD_SYSTEM_GC;
                sMainPadIsGCController = false;
            }
            
            if(result != 0){
                anyPadActive = true;
                //Map the Wii A/B buttons to the classic controller A/B buttons
                if(padType == PAD_TYPE_CLASSIC){
                    copyInputFlag(&newPad, PAD_INPUT_FLAG_CLASSIC_A, PAD_INPUT_FLAG_CORE_A);
                    copyInputFlag(&newPad, PAD_INPUT_FLAG_CLASSIC_B, PAD_INPUT_FLAG_CORE_B);
                }

                //0x554

                newPad.mPressedButtonFlags &= enabledInputFlags;
                newPad.mTurboPressButtonFlags &= enabledInputFlags;
                newPad.mReleasedButtonFlags &= enabledInputFlags;
                newPad.mHeldButtonFlags &= enabledInputFlags;
                newPad.mLongHoldButtonFlags &= enabledInputFlags;
                newPad.mShortPressButtonFlags &= enabledInputFlags;

                if((enabledInputFlags & (PAD_INPUT_FLAG_LSTICK_LEFT | PAD_INPUT_FLAG_LSTICK_RIGHT)) == 0){
                    newPad.mLStickX = 0;
                    newPad.mLStickXRaw = 0;
                    newPad.mLeftTriggerFloat = 0;
                    newPad.mRightTriggerFloat = 0;
                }

                if((enabledInputFlags & (PAD_INPUT_FLAG_LSTICK_UP | PAD_INPUT_FLAG_LSTICK_DOWN)) == 0){
                    newPad.mLStickY = 0;
                    newPad.mLStickYRaw = 0;
                }

                if(newPad.mPadType == PAD_TYPE_CLASSIC){
                    if(!(newPad.mHeldButtonFlags & 0x60000)){
                        newPad.mRStickXRaw = 0;
                    }else{
                        f32 stickX;
                        stickX = newPad.mRStickXRaw;
                        if(stickX >= 0.0f){
                            newPad.mRStickXRaw = (stickX - lbl_eu_80667EAC)/lbl_eu_80667EB8;
                        }else{
                            // Force stack reload before fabs (retail lfs at +0x604).
                            volatile f32* stickXReload = &newPad.mRStickXRaw;
                            newPad.mRStickXRaw = -(ml::math::abs(*stickXReload) - lbl_eu_80667EAC)/lbl_eu_80667EB8;
                        }
                    }

                    //0x620

                    if(!(newPad.mHeldButtonFlags & 0x180000)){
                        newPad.mRStickYRaw = 0;
                    }else{
                        f32 stickY;
                        stickY = newPad.mRStickYRaw;
                        if(stickY >= 0.0f){
                            newPad.mRStickYRaw = (stickY - lbl_eu_80667EAC)/lbl_eu_80667EB8;
                        }else{
                            // Force stack reload before fabs (retail lfs at +0x654).
                            volatile f32* stickYReload = &newPad.mRStickYRaw;
                            newPad.mRStickYRaw = -(ml::math::abs(*stickYReload) - lbl_eu_80667EAC)/lbl_eu_80667EB8;
                        }
                    }
                }

                //0x670

                if(mainPad != nullptr && mainPad == pad && isWpadChannel(i)){
                    CfGameManager::setCurrentPadPtr(CfGameManager::getPad(i), i);
                    hasActivePad = 1;
                    if(newPad.mPadType == PAD_TYPE_CLASSIC) sMainPadExtension = PAD_EXT_CLASSIC_CONTROLLER;
                    if(newPad.mPadType == PAD_TYPE_FS) sMainPadExtension = PAD_EXT_NUNCHUCK;
                    if(newPad.mPadType == PAD_TYPE_CORE) sMainPadExtension = PAD_EXT_NONE;
                }
            }else{
                if(CDeviceRemotePad::getPadData(i) != CDeviceRemotePad::getMainGCPad()){
                    std::memset(&newPad, 0, sizeof(CPad));
                }
            }

            setPad__Q22cf13CfGameManagerFv(i, &newPad, systemType);
        }

        if(anyPadActive){
            updateCfPadData(CfGameManager::getCfPadData(),CfGameManager::getCurrentPad());
        }else{
            std::memset(CfGameManager::getCfPadData(), 0, sizeof(CfPadData));
        }

        //BUG: sDpadDisableTimer is not properly initialized beforehand
        if(systemType == PAD_SYSTEM_WII){
            if(sDpadDisableTimer != 0){
                sMainPadIsGCController = true;
            }
            
            sDpadDisableTimer = 0;
        }else{
            if(sDpadDisableTimer < SECONDS_TO_FRAMES(1)){
                sDpadDisableTimer++;
                if((u8)sDpadDisableTimer == SECONDS_TO_FRAMES(1)){
                    KPADDisableDPD(mainPadChannel);
                }
            }
        }

        return hasActivePad;
    }

    CfPadTask* CfPadTask::create(CProcess* pParent){
        CfPadTask* padTask = new (CWorkThreadSystem::getWorkMem()) CfPadTask();
        padTask->Regist(pParent, false);
        return padTask;
    }
    
    bool CfPadTask::gameExceptionCB(u32 unusedArg){
        if(checkForControllerError(update()) == ERROR_NONE){
            mNoErrorFrameCount++;
            //If there has been no controller error for more than 9 frames, allow the exception message to be disabled
            if(mNoErrorFrameCount >= MAX_ERROR_FRAMES){
                mNoErrorFrameCount = 0;
                return true;
            }
        }

        return false;
    }

int CfPadTask::checkForControllerError(bool noError) {
    if (!noError) {
        if ((sDpadDisableTimer >= SECONDS_TO_FRAMES(1) && sMainPadExtension != PAD_EXT_INVALID) || sMainPadIsGCController) {
            if (sMainPadIsGCController) return ERROR_WIIMOTE_DISCONNECTED;

            if (lbl_eu_80663E28 & (1u << 28)) {
                if (sMainPadExtension == PAD_EXT_NUNCHUCK) return ERROR_NUNCHUCK_DISCONNECTED;
                else if (sMainPadExtension == PAD_EXT_CLASSIC_CONTROLLER) return ERROR_CLASSIC_CONTROLLER_DISCONNECTED;
                else if (sMainPadExtension == PAD_EXT_NONE) return ERROR_NO_EXTENSION;
            }
        }
    } else if (sDpadDisableTimer >= SECONDS_TO_FRAMES(1) && (lbl_eu_80663E28 & (1u << 28))) {
        if (sMainPadExtension == PAD_EXT_NONE) return ERROR_NO_EXTENSION;
    }

    return ERROR_NONE;
}

    void CfPadTask::onInitHbm(){
        u32 channel = cf::CfGameManager::getCurrentPadChannel();
        if(channel != WPAD_CHAN_INVALID){
            KPADEnableDPD(channel);
        }
    }

    void CfPadTask::onDeleteHbm(){
        u32 channel = cf::CfGameManager::getCurrentPadChannel();
        if(channel != WPAD_CHAN_INVALID){
            KPADDisableDPD(channel);
        }
    }

u32 CfPadTask::getWiimoteBattery() {
    return lbl_eu_80664444;
}

void CfPadTask::wpadGetInfoCallback(s32 chan, s32 result){
    if(lbl_eu_80664448 != NULL && result == WPAD_ERR_OK){
        lbl_eu_80664444 = sWpadInfo.battery;
    }
}

}
