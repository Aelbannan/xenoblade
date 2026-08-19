#include "monolib/core/CPadManager.hpp"
#include <cstring>

// Float constants: retail CPadManager.o has .sdata2 size 0 -- every float
// constant is referenced from the shared FloatUtils pool (lbl_eu_8066A320..A340)
// instead of a TU-local copy. The header's lbl_eu_8066A320 and the CDeviceVI
// header's lbl_eu_8066A340 keep their declarations (other TUs may use them); this
// TU references the named externs so no copy is emitted here.
extern float lbl_eu_8066A320;  // 0.15f  (lbl_eu_8066A320)
extern float lbl_eu_8066A324;  // 0.0f
extern float lbl_eu_8066A328;  // 56.0f  (lbl_eu_8066A328)
extern float lbl_eu_8066A32C;  // 150.0f (lbl_eu_8066A32C)
extern float lbl_eu_8066A330;  // 320.0f (lbl_eu_8066A330)
extern float lbl_eu_8066A334;  // 210.0f (lbl_eu_8066A334)
extern double lbl_eu_8066A340;  // 1/30  (MS_PER_FRAME)
extern double lbl_eu_8066A338;  // 0x4330000000000000 (2^52, unsigned u8->f32 magic)

// u8 -> f32 conversion matching retail: build the 2^52+x double on the stack
// (low word = x, high word = 0x43300000 -- NO sign flip for unsigned) and
// subtract the shared unsigned magic double lbl_eu_8066A338 (FloatUtils-owned),
// so the TU emits no .sdata2 pool entry (retail CPadManager.o has .sdata2
// size 0). MWCC_CASES 7i. Statement order matters: the value word first,
// then 0x43300000.
inline float u8ToF32(u8 v) {
    union { double d; u32 w[2]; } u;
    u.w[1] = (u32)v;
    u.w[0] = 0x43300000;
    return (float)(u.d - lbl_eu_8066A338);
}

//Total number of buttons for different control styles
const int NUM_BUTTONS_WIIMOTE_NUNCHUCK = 14;
const int NUM_BUTTONS_CLASSIC = 16;

// ---- Retail sbss data (blob monolibdata1 dissolve): 8-byte slot 0x80665630 ----
// Word 0 holds the heap-allocated CPadData pointer (the retail code reads the
// first word of this 8-byte slot; there is no separate lbl_eu_80665630[0] static -- the
// header's private static member is declared but never defined here).
CPadData* lbl_eu_80665630[2];

// ---- Retail rodata data (blob monolibdata1 dissolve) ----
// WPAD/Classic button conversion tables (retail .rodata 0x80522870 / 0x805228E0).
// GLOBAL const (extern "C") so updatePadInputs references the retail names
// instead of TU-local @NNNN pools.
extern "C" const PadButtonMapping lbl_eu_80522870[NUM_BUTTONS_WIIMOTE_NUNCHUCK] = {
    {WPAD_BUTTON_LEFT, PAD_INPUT_FLAG_LEFT},
    {WPAD_BUTTON_RIGHT, PAD_INPUT_FLAG_RIGHT},
    {WPAD_BUTTON_UP, PAD_INPUT_FLAG_UP},
    {WPAD_BUTTON_DOWN, PAD_INPUT_FLAG_DOWN},
    {WPAD_BUTTON_A, PAD_INPUT_FLAG_CORE_A},
    {WPAD_BUTTON_B, PAD_INPUT_FLAG_CORE_B},
    {WPAD_BUTTON_1, PAD_INPUT_FLAG_1},
    {WPAD_BUTTON_2, PAD_INPUT_FLAG_2},
    {WPAD_BUTTON_HOME, PAD_INPUT_FLAG_HOME},
    {WPAD_BUTTON_PLUS, PAD_INPUT_FLAG_PLUS},
    {WPAD_BUTTON_MINUS, PAD_INPUT_FLAG_MINUS},
    {WPAD_BUTTON_FS_Z, PAD_INPUT_FLAG_FS_Z},
    {WPAD_BUTTON_FS_C, PAD_INPUT_FLAG_FS_C},
    {0, 0}
};

extern "C" const PadButtonMapping lbl_eu_805228E0[NUM_BUTTONS_CLASSIC] = {
    {WPAD_BUTTON_CL_LEFT, PAD_INPUT_FLAG_LEFT},
    {WPAD_BUTTON_CL_RIGHT, PAD_INPUT_FLAG_RIGHT},
    {WPAD_BUTTON_CL_UP, PAD_INPUT_FLAG_UP},
    {WPAD_BUTTON_CL_DOWN, PAD_INPUT_FLAG_DOWN},
    {WPAD_BUTTON_CL_HOME, PAD_INPUT_FLAG_HOME},
    {WPAD_BUTTON_CL_PLUS, PAD_INPUT_FLAG_PLUS},
    {WPAD_BUTTON_CL_MINUS, PAD_INPUT_FLAG_MINUS},
    {WPAD_BUTTON_CL_A, PAD_INPUT_FLAG_CLASSIC_A},
    {WPAD_BUTTON_CL_B, PAD_INPUT_FLAG_CLASSIC_B},
    {WPAD_BUTTON_CL_X, PAD_INPUT_FLAG_CLASSIC_X},
    {WPAD_BUTTON_CL_Y, PAD_INPUT_FLAG_CLASSIC_Y},
    {WPAD_BUTTON_CL_L, PAD_INPUT_FLAG_CLASSIC_L},
    {WPAD_BUTTON_CL_R, PAD_INPUT_FLAG_CLASSIC_R},
    {WPAD_BUTTON_CL_ZL, PAD_INPUT_FLAG_CLASSIC_ZL},
    {WPAD_BUTTON_CL_ZR, PAD_INPUT_FLAG_CLASSIC_ZR},
    {0, 0}
};

CWpadStatus* CPadManager::getWpadStatus(int index){
    return &lbl_eu_80665630[0]->mWpadStatuses[index];
}

void* CPadManager::wpadAllocFunc(u32 size){
    //If CPadManager's handle isn't null, use it for allocations. Otherwise, just use the MEM2 handle.
    mtl::ALLOC_HANDLE handle = mtl::MemManager::getHandleMEM2();
    if(lbl_eu_80665630[0] != nullptr) handle = lbl_eu_80665630[0]->mAllocHandle;
    return mtl::MemManager::allocate_head(handle, size, 4);
}

int CPadManager::wpadDeallocFunc(void* pData){
    if(pData != nullptr){
        mtl::MemManager::deallocate(pData);
    }
    return 1;
}

PadUpdateFunc CPadManager::initialize(mtl::ALLOC_HANDLE handle){
    CPadData* padData = static_cast<CPadData*>(mtl::MemManager::allocate_head(handle, sizeof(CPadData), 32));
    lbl_eu_80665630[0] = padData;

    lbl_eu_80665630[0]->unk5118 = 0;
    lbl_eu_80665630[0]->mAllocHandle = handle;

    WPADRegisterAllocator(wpadAllocFunc, wpadDeallocFunc);
    KPADInitEx(&lbl_eu_80665630[0]->mWpadStatus, 128);
    
    std::memset(&lbl_eu_80665630[0]->mPads, 0, sizeof(lbl_eu_80665630[0]->mPads));
    std::memset(&lbl_eu_80665630[0]->mDummyPad, 0, sizeof(CPad));
    
    lbl_eu_80665630[0]->mMainPad = &lbl_eu_80665630[0]->mDummyPad;
    lbl_eu_80665630[0]->mMainGCPad = &lbl_eu_80665630[0]->mDummyPad;

    lbl_eu_80665630[0]->mConfig.turboHoldTimerThreshold = TURBO_HOLD_TIMER_THRESHOLD;
    lbl_eu_80665630[0]->mConfig.turboInputFrames = TURBO_INPUT_FRAMES;
    lbl_eu_80665630[0]->mConfig.longHoldTimerThreshold = LONG_HOLD_TIMER_THRESHOLD;
    lbl_eu_80665630[0]->mConfig.shortPressMaxFrames = SHORT_PRESS_MAX_FRAMES;
    lbl_eu_80665630[0]->mConfig.mLStickDeadzone = lbl_eu_8066A320;
    lbl_eu_80665630[0]->mConfig.mRStickDeadzone = lbl_eu_8066A320;
    lbl_eu_80665630[0]->mConfig.unk0 = nullptr;

    lbl_eu_80665630[0]->unk511C = nullptr;

    //Reset the connected flag for all controllers
    for(int i = 0; i < TOTAL_CONTROLLERS; i++){
        lbl_eu_80665630[0]->mPads[i].mConnected = false;
    }


    lbl_eu_80665630[0]->mConfig.unk1C = 0b11;

    //Wait until the status isn't 3
    while(WPADGetStatus() != WPAD_LIB_STATUS_3){}

    //Setup controller ports

    //Wii controllers
    for(int i = 0; i < WPAD_MAX_CONTROLLERS; i++){
        KPADChannel channel = (KPADChannel)i; //Required for matching
        KPADEnableAimingMode(channel);
        KPADSetConnectCallback(channel, kpadConnectCallback);
        lbl_eu_80665630[0]->mPads[channel].mChannel = channel;
    }

    //GC controllers
    for(int i = 0; i < PAD_CHANMAX; i++){
        lbl_eu_80665630[0]->mPads[WPAD_MAX_CONTROLLERS + i].mChannel = i;
    }

    KPADReset();

    //Why can't this just be called normally??
    return update;
}

void CPadManager::destroy(){
    DELETE_OBJ(lbl_eu_80665630[0]);
    
    //Reset the callbacks for each Wii controller port
    for(int i = 0; i < WPAD_MAX_CONTROLLERS; i++){
        KPADSetConnectCallback(i, nullptr);
        WPADSetExtensionCallback(i, nullptr);
    }
}

void CPadManager::updateLongHoldTimerThreshold(u32 r3){
    lbl_eu_80665630[0]->mConfig.longHoldTimerThreshold = r3;
}

void CPadManager::setRightStickDeadzoneDefault(){
    lbl_eu_80665630[0]->mConfig.mRStickDeadzone = lbl_eu_8066A320;
}

void CPadManager::setRightStickDeadzone(float value){
    lbl_eu_80665630[0]->mConfig.mRStickDeadzone = value;
}


void CPad::setExtensionCB(s32 chan, s32 result){
    //TODO: can this be matched better?
    if(result != KPAD_ERR_OK){
        if(result != -1) return;
    }else{
        WPADSetExtensionCallback(chan, CPadManager::wpadExtensionCallback);
        mConnected = true;
        mPadType = PAD_TYPE_NONE;
        return;
    }

    mConnected = false;
    mPadType = PAD_TYPE_NONE;
}

//Not the same as convertWiiDevType
void CPad::setWiiPadType(s32 dev){
    switch(dev){
        case WPAD_DEV_CORE:
        mPadType = PAD_TYPE_CORE;
        break;
        case WPAD_DEV_FREESTYLE:
        mPadType = PAD_TYPE_FS;
        break;
        case WPAD_DEV_CLASSIC:
        mPadType = PAD_TYPE_CLASSIC;
        break;
        case WPAD_DEV_FUTURE:
        //Unknown device, default to Wiimote
        mPadType = PAD_TYPE_CORE;
        break;
    }
}

void CPad::checkExtension(s32 chan){
    s32 devType;

    if(mConnected && mPadType == PAD_TYPE_NONE){
        if(WPADProbe(chan, &devType) == 0){
            mPadType = CPadManager::convertWiiDevType(devType);
        }
    }
}

void CPadManager::kpadConnectCallback(s32 chan, s32 result){
    if(lbl_eu_80665630[0] == nullptr) return;

    if(lbl_eu_80665630[0]->mConfig.unk1C & 1){
        CPad* r31 = getPadData(0, chan);
        r31->setExtensionCB(chan, result);

        Unk511CFunc func = lbl_eu_80665630[0]->unk511C;
        if(func != nullptr){
            func(chan, result);
        }
    }
}

void CPadManager::wpadExtensionCallback(s32 chan, s32 dev){
    if(lbl_eu_80665630[0] == nullptr) return;

    if(lbl_eu_80665630[0]->mConfig.unk1C & 1){
        CPad* r31 = getPadData(0, chan);
        r31->setWiiPadType(dev);
    }
}

void CPadManager::updatePadExtensions(){
    if(lbl_eu_80665630[0]->mConfig.unk1C & 1){
        for(int i = 0; i < WPAD_MAX_CONTROLLERS; i++){
            KPADChannel channel = (KPADChannel)i; //Required for matching
            WPADSetExtensionCallback(channel, wpadExtensionCallback);
            CPad* r31 = getPadData(0, channel);
            r31->checkExtension(channel);
        }
    }
}


void CPad::setJoystickValues(CWpadStatus* pPadStatus){
    //Update joystick/trigger values
    switch((u32)mPadType){
        case PAD_TYPE_FS:
        case PAD_TYPE_6:
            //Nunchuck
            mLStickXRaw = pPadStatus->ex_status.fs.stick.x;
            mLStickYRaw = pPadStatus->ex_status.fs.stick.y;
            mLStickX = lbl_eu_8066A328 * mLStickXRaw;
            mLStickY = lbl_eu_8066A328 * mLStickYRaw;
            break;
        case PAD_TYPE_CLASSIC:
        case PAD_TYPE_7:
            //Classic controller
            mLStickXRaw = pPadStatus->ex_status.cl.lstick.x;
            mLStickYRaw = pPadStatus->ex_status.cl.lstick.y;
            mRStickXRaw = pPadStatus->ex_status.cl.rstick.x;
            mRStickYRaw = pPadStatus->ex_status.cl.rstick.y;

            mLeftTriggerByte = pPadStatus->ex_status.cl.ltrigger;
            mRightTriggerByte = pPadStatus->ex_status.cl.rtrigger;

            mLStickX = lbl_eu_8066A328 * mLStickXRaw;
            mLStickY = lbl_eu_8066A328 * mLStickYRaw;
            mRStickX = lbl_eu_8066A328 * mRStickXRaw;
            mRStickY = lbl_eu_8066A328 * mRStickYRaw;

            mLeftTriggerFloat = u8ToF32(mLeftTriggerByte)/lbl_eu_8066A32C;
            mRightTriggerFloat = u8ToF32(mRightTriggerByte)/lbl_eu_8066A32C;
            break;
        default:
            break;
    }
}

void CPad::setAcceleration(CWpadStatus* pPadStatus){
    //Update acceleration values
    switch(mPadType){
        case PAD_TYPE_CORE ... PAD_TYPE_7:
            //Wii controllers
            mWiimoteAccelData.mAcc.x = pPadStatus->acc.x;
            mWiimoteAccelData.mAcc.y = pPadStatus->acc.y;
            mWiimoteAccelData.mAcc.z = pPadStatus->acc.z;
            mWiimoteAccelData.mAccValue = pPadStatus->acc_value;
            mWiimoteAccelData.mAccSpeed = pPadStatus->acc_speed;
            break;
        default:
            break;
    }

    //Update nunchuck acceleration values
    switch((u32)mPadType){
        case PAD_TYPE_FS:
        case PAD_TYPE_6:
            mNunchuckAccelData.mAcc.x = pPadStatus->ex_status.fs.acc.x;
            mNunchuckAccelData.mAcc.y = pPadStatus->ex_status.fs.acc.y;
            mNunchuckAccelData.mAcc.z = pPadStatus->ex_status.fs.acc.z;
            mNunchuckAccelData.mAccValue = pPadStatus->ex_status.fs.acc_value;
            mNunchuckAccelData.mAccSpeed = pPadStatus->ex_status.fs.acc_speed;
            break;
        default:
            break;
    }
}

void CPad::setMiscValues(CWpadStatus* pPadStatus){
    switch(mPadType){
        case PAD_TYPE_CORE ... PAD_TYPE_7:
            //Wii controllers
            mWpadData.mDpdValidFg = pPadStatus->dpd_valid_fg;
            if(mWpadData.mDpdValidFg > 0){
                mWpadData.mPos.x = pPadStatus->pos.x * lbl_eu_8066A330;
                mWpadData.mPos.y = pPadStatus->pos.y * lbl_eu_8066A334;
                mWpadData.mPos.z = lbl_eu_8066A324;

                mWpadData.mVec.x = pPadStatus->vec.x;
                mWpadData.mVec.y = pPadStatus->vec.y;
                mWpadData.mVec.z = lbl_eu_8066A324;

                mWpadData.mSpeed = pPadStatus->speed;

                mWpadData.mHorizon.x = pPadStatus->horizon.x;
                mWpadData.mHorizon.y = pPadStatus->horizon.y;
                mWpadData.mHorizon.z = lbl_eu_8066A324;

                mWpadData.mHoriVec.x = pPadStatus->hori_vec.x;
                mWpadData.mHoriVec.y = pPadStatus->hori_vec.y;
                mWpadData.mHoriVec.z = lbl_eu_8066A324;

                mWpadData.mHoriSpeed = pPadStatus->hori_speed;
                mWpadData.mDist = pPadStatus->dist;
                mWpadData.mDistVec = pPadStatus->dist_vec;
                mWpadData.mDistSpeed = pPadStatus->dist_speed;
            }
            break;
        default:
            break;
    }
}

u32 CPad::calculateFlagValue(CWpadStatus* pPadStatus, const PadButtonMapping* pClassicMap, const PadButtonMapping* pWiimoteMap){
    u32 flagValue = 0;

    switch(mPadType){
        case PAD_TYPE_CLASSIC:
        case PAD_TYPE_7:{
            for(int i = 0; pClassicMap[i].wpadValue != 0; i++){
                if(pPadStatus->ex_status.cl.hold & pClassicMap[i].wpadValue){
                    flagValue |= pClassicMap[i].customValue;
                }
            }

            if(pPadStatus->hold & WPAD_BUTTON_CL_RIGHT) flagValue |= PAD_INPUT_FLAG_HOME;
            break;
        }
        case PAD_TYPE_CORE:
        case PAD_TYPE_FS:
        case PAD_TYPE_5:
        case PAD_TYPE_6:{
            for(int i = 0; pWiimoteMap[i].wpadValue != 0; i++){
                if(pPadStatus->hold & pWiimoteMap[i].wpadValue){
                    flagValue |= pWiimoteMap[i].customValue;
                }
            }
            break;
        }
        default:
            break;
    }

    return flagValue;
}

void CPad::updateFlagValues(u32 buttonFlags){
    //Set digital joystick direction inputs using the deadzone for each stick
    if(mLStickXRaw <= -lbl_eu_80665630[0]->mConfig.mLStickDeadzone) buttonFlags |= PAD_INPUT_FLAG_LSTICK_LEFT;
    if(mLStickXRaw > lbl_eu_80665630[0]->mConfig.mLStickDeadzone) buttonFlags |= PAD_INPUT_FLAG_LSTICK_RIGHT;
    if(mLStickYRaw <= -lbl_eu_80665630[0]->mConfig.mLStickDeadzone) buttonFlags |= PAD_INPUT_FLAG_LSTICK_DOWN;
    if(mLStickYRaw > lbl_eu_80665630[0]->mConfig.mLStickDeadzone) buttonFlags |= PAD_INPUT_FLAG_LSTICK_UP;
    if(mRStickXRaw <= -lbl_eu_80665630[0]->mConfig.mRStickDeadzone) buttonFlags |= PAD_INPUT_FLAG_RSTICK_LEFT;
    if(mRStickXRaw > lbl_eu_80665630[0]->mConfig.mRStickDeadzone) buttonFlags |= PAD_INPUT_FLAG_RSTICK_RIGHT;
    if(mRStickYRaw <= -lbl_eu_80665630[0]->mConfig.mRStickDeadzone) buttonFlags |= PAD_INPUT_FLAG_RSTICK_DOWN;
    if(mRStickYRaw > lbl_eu_80665630[0]->mConfig.mRStickDeadzone) buttonFlags |= PAD_INPUT_FLAG_RSTICK_UP;

    //Update the button flag values
    mLongHoldButtonFlags = 0;
    u32 changedButtonMask = mHeldButtonFlags ^ buttonFlags;
    u32 pressedButtons = buttonFlags & changedButtonMask;
    mPressedButtonFlags = pressedButtons;
    mReleasedButtonFlags = changedButtonMask & ~buttonFlags;
    mTurboPressButtonFlags = pressedButtons;
    mHeldButtonFlags = buttonFlags;
    mShortPressButtonFlags = 0;

    //Update the button hold timers, and all the additional button flag values
    u32 bit = PAD_INPUT_FLAG_LEFT;

    for(int i = 0; i < MAX_PAD_INPUT_FLAGS; i++){
        if(buttonFlags & bit){
            if(mPressedButtonFlags & bit){
                mButtonHoldTimersTurbo[i] = 0;
                mButtonHoldTimers[i] = 0;
            }

            mButtonHoldTimersTurbo[i]++;

            //BUG?: This should maybe be < instead? This makes the limit 1 more than the max
            if(mButtonHoldTimers[i] <= lbl_eu_80665630[0]->mConfig.longHoldTimerThreshold){
                mButtonHoldTimers[i]++;
            }

            //Enable the bit in the turbo press flagset on every nth frame as long as the timer has passed the threshold
            if(mButtonHoldTimersTurbo[i] >= lbl_eu_80665630[0]->mConfig.turboHoldTimerThreshold +
            lbl_eu_80665630[0]->mConfig.turboInputFrames){
                mTurboPressButtonFlags |= bit;
                mButtonHoldTimersTurbo[i] -= lbl_eu_80665630[0]->mConfig.turboInputFrames;
            }

            if(mButtonHoldTimers[i] >= lbl_eu_80665630[0]->mConfig.longHoldTimerThreshold){
                mLongHoldButtonFlags |= bit;
            }
        }else if(mReleasedButtonFlags & bit){
            if(lbl_eu_80665630[0]->mConfig.shortPressMaxFrames >= mButtonHoldTimers[i]){
                mShortPressButtonFlags |= bit;
            }
        }

        //Go to the next bit
        bit <<= 1;
    }

    CPadData_UnkStruct2* r30 = lbl_eu_80665630[0]->mConfig.unk0;

    while(r30 != nullptr){
        r30->UnkVirtualFunc1(this);
        r30 = r30->mNext;
    }
}

void CPadManager::updatePadInputs(){
    /*
    WPAD button values -> custom button values conversion tables for
    Wiimote + Nunchuck and Classic Controller (retail rodata lbl_eu_80522870 /
    lbl_eu_805228E0, defined at the bottom of this TU).
    The entry with all zeroes acts as a terminator
    */

    KPADResult result;

    for(int i = 0; i < WPAD_MAX_CONTROLLERS; i++){
        KPADChannel channel = (KPADChannel)i;

        KPADReadEx(channel, &lbl_eu_80665630[0]->mWpadStatuses[i], 16, &result);

        CPad* pad = getPadData(0, channel);

        if(pad->mConnected && result == KPAD_ERR_OK){
            CWpadStatus* padStatus = getWpadStatus(i);

            pad->mWpadErr = padStatus->wpad_err;
            s8 error = padStatus->wpad_err;

            if(error == -7){
                pad->mPadType = PAD_TYPE_CORE;
                padStatus->hold &= ~(WPAD_BUTTON_FS_Z | WPAD_BUTTON_FS_C);
                pad->mLStickYRaw = lbl_eu_8066A324;
                pad->mLStickXRaw = lbl_eu_8066A324;
                pad->mLStickX = lbl_eu_8066A324;
            }else if(error != 0){
                pad->mHeldButtonFlags = 0;
                pad->mPressedButtonFlags = 0;
                pad->mReleasedButtonFlags = 0;
                continue;
            }

            u32 buttonFlags = pad->calculateFlagValue(padStatus, lbl_eu_805228E0, lbl_eu_80522870);
            pad->setJoystickValues(padStatus);
            pad->updateFlagValues(buttonFlags);
            pad->setAcceleration(padStatus);
            pad->setMiscValues(padStatus);
        }
    }

}

void CPad::updateMotor(){
    if(mMotorTimer > lbl_eu_8066A324){
        mMotorTimer -= lbl_eu_8066A340;

        //If the motor timer reached zero, turn off the motor
        if(mMotorTimer <= lbl_eu_8066A324){
            mMotorTimer = lbl_eu_8066A324;

            switch(mPadType){
                case PAD_TYPE_CORE ... PAD_TYPE_7:
                    WPADControlMotor(mChannel, WPAD_MOTOR_STOP);
                    break;
                case PAD_TYPE_GC:
                    //Unused in the release version, but this confirms it was supported :)
                    PADControlMotor(mChannel, PAD_MOTOR_STOP);
                    break;
                default:
                    break;
            }
        }
    }
}

/* Updates the pointers for the current main controllers. This typically will set each
to the entry corresponding to the controllers in the earliest ports, but will instead be
set to the dummy entry if no controller is connected. */
void CPadManager::updateMainControllers(){
    lbl_eu_80665630[0]->mMainPad = &lbl_eu_80665630[0]->mDummyPad;
    
    //Find the first connected controller, starting with Wii controller ports first
    for(int i = 0; i < TOTAL_CONTROLLERS; i++){
        //A connected controller was found, update the pointer
        if(lbl_eu_80665630[0]->mPads[i].mConnected){
            lbl_eu_80665630[0]->mMainPad = &lbl_eu_80665630[0]->mPads[i];
            break;
        }
    }
    
    lbl_eu_80665630[0]->mMainGCPad = &lbl_eu_80665630[0]->mDummyPad;

    //Find the first connected GC controller
    for(int i = 4; i < TOTAL_CONTROLLERS; i++){
        //A connected controller was found, update the pointer
        if(lbl_eu_80665630[0]->mPads[i].mConnected){
            lbl_eu_80665630[0]->mMainGCPad = &lbl_eu_80665630[0]->mPads[i];
            break;
        }
    }
}

void CPadManager::update(){
    updatePadExtensions();
    updatePadInputs();

    lbl_eu_80665630[0]->mMainPad->updateMotor();

    //Update the first controller pointers
    updateMainControllers();
}

CPad* CPadManager::getPadData(s32 type, s32 channel){
    switch(type){
        case PAD_SYSTEM_GC:
        //Gamecube controllers
        if(channel < PAD_CHANMAX) return &lbl_eu_80665630[0]->mPads[WPAD_MAX_CONTROLLERS + channel];
        break;
        case PAD_SYSTEM_WII:
        //Wii controllers
        if(channel < WPAD_MAX_CONTROLLERS) return &lbl_eu_80665630[0]->mPads[channel];
        break;
    }

    return &lbl_eu_80665630[0]->mDummyPad;
}

CPad* CPadManager::getDummyPad(){
    return &lbl_eu_80665630[0]->mDummyPad;
}

//Returns the controller data for the current main controller (controller in earliest port, Wii ports have priority).
CPad* CPadManager::getMainPad(){
    return lbl_eu_80665630[0]->mMainPad;
}

//Returns the controller data for the current main GC controller (controller in earliest port).
CPad* CPadManager::getMainGCPad(){
    return lbl_eu_80665630[0]->mMainGCPad;
}

extern "C" u8* func_eu_80449F30(int index) { return (u8*)lbl_eu_80665630[0] + index * 0xf8 + 0x24; }
