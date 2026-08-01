#include <revolution/version.h>

#include <revolution/BTE.h>
#include <revolution/IPC.h>
#include <revolution/OS.h>
#include <revolution/SC.h>
#include <revolution/WPAD.h>
#include <revolution/WUD.h>

#include <string.h>

#define DPD_CONFIG1_SIZE 9
#define DPD_CONFIG2_SIZE 2
#define SPK_CONFIG_SIZE 7

RVL_LIB_VERSION(WPAD, "May 17 2007", "01:52:03", "0x4199_60831");

void __wpadSendDataSub(s32 chan, WPADCommand command);
void __wpadReceiveCallback(UINT8 devHandle, UINT8* pReport, UINT16 len);
void __wpadCalcControllerData(s32 chan);
void __wpadInfoCallback(s32 chan, s32 status);
void __wpadDpdCallback(s32 chan, s32 status);
void* __wpadNoAlloc(u32 size);
BOOL __wpadNoFree(void* pBlock);

extern const char lbl_80560608[];
extern const char lbl_805607C4[0x9C];
extern const char lbl_80560860[];
extern const char lbl_805606C8[];

extern s32 WPADWriteExtReg(s32 chan, void* pData, u16 len, u32 addr,
                           WPADCallback pCallback);
extern void WPADiClearMemBlock(int index);

OSAlarm _wpadManageAlarm;

WPADCB* __rvl_p_wpadcb[WPAD_MAX_CONTROLLERS];

u8 __wpadManageHandlerStack[0x1000] ALIGN(32);

s8 _wpadHandle2PortTable[WUD_MAX_DEV_ENTRY];

WPADCB __rvl_wpadcb[WPAD_MAX_CONTROLLERS];

u8 _wpadSleepTime;
u8 _wpadDpdSense;
u8 _wpadSensorBarPos;
u32 _wpadRumbleFlag;
u8 _wpadSpeakerVol;
u8 _scFlush;
u8 _wpadGameType;
const char* _wpadGameCode;
u8 _wpadIsUsedChannel[WPAD_MAX_CONTROLLERS];

static u8 _wpadSCSetting;
static u8 _wpadShutdownFlag;
static s8 _wpadAfhChannel;
static BOOL _wpadInitialized;
static u8 _wpadRumbleCnt[WPAD_MAX_CONTROLLERS];
static u8 _wpadExtCnt[WPAD_MAX_CONTROLLERS];
static u16 _wpadAfhCnt;
static u8 _wpadCheckCnt;
static u16 _wpadSenseCnt;
static u8 _wpadRegisterShutdownFunc;

void* _wpadUsedCallback;

s32 _wpadOnReconnect;
s32 _wpadReconnectWait;

void (*_wpadVSMInit)(void);
void (*_wpadTRNInit)(void);
void (*_wpadGTRInit)(void);
void (*_wpadDRMInit)(void);

static u16 _wpad_diff_count_threshold[2] = {6, 4};   // acc, dpd
static u16 _wpad_hyst_count_threshold[2] = {30, 30};  // acc, dpd

void __wpadConnectionCallback(WUDDevInfo* pInfo, u8 open);

static DECOMP_INLINE void WPADiDisconnect(s32 chan, BOOL sleep);

static BOOL __CanPushCmdQueue(const WPADCommandQueue* pQueue, s8 num);
static s8 __GetCmdNumber(const WPADCommandQueue* pQueue);

static void __SetScreenSaverFlag(BOOL disable);
static u8 __GetDpdSensitivity(void);
static u8 __GetSensorBarPosition(void);
static u32 __GetMotorMode(void);
static u8 __ClampSpeakerVolume(u8 volume);
static u8 __GetSpeakerVolume(void);

static BOOL OnShutdown(BOOL final, u32 event) {
    BOOL result = FALSE;
    WUDLibStatus status = WUDGetStatus();

    if (!final) {
        if (status == WUD_LIB_STATUS_3) {
            switch (event) {
            case OS_SD_EVENT_FATAL: {
                DEBUGPrint("Deregister allocators because of fatal error.\n");
                WUDRegisterAllocator(&__wpadNoAlloc, &__wpadNoFree);

                // FALLTHROUGH
            }

            case OS_SD_EVENT_SHUTDOWN:
            case OS_SD_EVENT_3: {
                __WPADShutdown();
                result = FALSE;
                break;
            }

            case OS_SD_EVENT_1:
            case OS_SD_EVENT_RESTART:
            case OS_SD_EVENT_RETURN_TO_MENU:
            case OS_SD_EVENT_LAUNCH_APP: {
                result = FALSE;
                break;
            }
            }

        } else if (status == WUD_LIB_STATUS_5) {
            WPADStopSimpleSync();
            result = FALSE;

        } else if (status == WUD_LIB_STATUS_4 || status == WUD_LIB_STATUS_1 ||
                   status == WUD_LIB_STATUS_2) {
            result = FALSE;

        } else if (status == WUD_LIB_STATUS_0) {
            result = TRUE;
        }
    } else {
        result = TRUE;
    }

_end:
    return result;
}

static OSShutdownFunctionInfo ShutdownFunctionInfo = {OnShutdown, 127};

void __wpadCalcRadioQuality(s32 chan) {
    WPADCB* p = __rvl_p_wpadcb[chan];
    u16 a;

    if (_wpadSenseCnt != 10) {
        return;
    }

    a = p->radioSensitivity * 9;
    a += (u16)((p->copyOutCount * 100) >> 1);
    a /= 10;
    a = a > 100 ? 100 : a;

    p->radioSensitivity = a;
    p->copyOutCount = 0;

    if (p->radioQuality != WPAD_RADIO_QUALITY_GOOD) {
        if (a > 85) {
            p->radioQuality = WPAD_RADIO_QUALITY_GOOD;
            p->radioQualityOkMs = 0;
        } else if (a > 80) {
            p->radioQualityOkMs++;

            if (p->radioQualityOkMs >= 20) {
                p->radioQuality = WPAD_RADIO_QUALITY_GOOD;
                p->radioQualityOkMs = 0;
            }
        }
    } else if (a < 75) {
        p->radioQuality = WPAD_RADIO_QUALITY_BAD;
        p->radioQualityOkMs = 0;
    } else if (a < 80) {
        p->radioQualityOkMs++;

        if (p->radioQualityOkMs >= 1) {
            p->radioQuality = WPAD_RADIO_QUALITY_BAD;
            p->radioQualityOkMs = 0;
        }
    }
}

static u8 IsButtonChanged(u16 lhs, u16 rhs) {
    return lhs != rhs ? TRUE : FALSE;
}

static u8 IsAnalogChanged(s32 lhs, s32 rhs, s32 threshold) {
    s32 diff = lhs - rhs < 0 ? rhs - lhs : lhs - rhs;
    return diff > threshold ? TRUE : FALSE;
}

static DECOMP_INLINE BOOL CalcAccNoise(WPADCB* p, BOOL analogChanged) {
    if (analogChanged) {
        p->filterDiffAcc++;

        if (p->filterDiffAcc > _wpad_diff_count_threshold[0]) {
            p->filterDiffAcc = 0;
            p->filterSameAcc = 0;
            return TRUE;
        }

    } else {
        p->filterSameAcc =
            (p->filterSameAcc + 1) % _wpad_hyst_count_threshold[0];

        if (p->filterSameAcc == _wpad_hyst_count_threshold[0] - 1 &&
            p->filterDiffAcc > 0) {
            p->filterDiffAcc--;
        }
    }

    return FALSE;
}

static DECOMP_INLINE BOOL CalcDpdNoise(WPADCB* p, BOOL analogChanged) {
    if (analogChanged) {
        p->filterDiffDpd++;

        if (p->filterDiffDpd > _wpad_diff_count_threshold[1]) {
            p->filterDiffDpd = 0;
            p->filterSameDpd = 0;
            return TRUE;
        }

    } else {
        p->filterSameDpd =
            (p->filterSameDpd + 1) % _wpad_hyst_count_threshold[1];

        if (p->filterSameDpd == _wpad_hyst_count_threshold[1] - 1 &&
            p->filterDiffDpd > 0) {
            p->filterDiffDpd--;
        }
    }

    return FALSE;
}

static DECOMP_INLINE BOOL CalcExtNoise(WPADCB* p, BOOL analogChanged) {
    if (analogChanged) {
        p->filterDiffExt++;

        if (p->filterDiffExt > _wpad_diff_count_threshold[1]) {
            p->filterDiffExt = 0;
            p->filterSameExt = 0;
            return TRUE;
        }

    } else {
        p->filterSameExt =
            (p->filterSameExt + 1) % _wpad_hyst_count_threshold[1];

        if (p->filterSameExt == _wpad_hyst_count_threshold[1] - 1 &&
            p->filterDiffExt > 0) {
            p->filterDiffExt--;
        }
    }

    return FALSE;
}

BOOL __wpadIsControllerDataChanged(WPADCB* p, void* pLhs, void* pRhs) {
    WPADStatus* pLhsCR = (WPADStatus*)pLhs;
    WPADStatus* pRhsCR = (WPADStatus*)pRhs;
    u8 changed = 0;
    int i;

    // dataFormat 0xE (extended DPD report) skips the core-compare block.
    if (p->dataFormat != 14) {
        if (pLhsCR->err == WPAD_ERR_OK || pLhsCR->err == WPAD_ERR_CORRUPTED) {
            if (pRhsCR->err == WPAD_ERR_OK || pRhsCR->err == WPAD_ERR_CORRUPTED) {
                changed = IsButtonChanged(pLhsCR->button, pRhsCR->button);

                changed |= CalcAccNoise(
                    p,
                    IsAnalogChanged(pLhsCR->accX, pRhsCR->accX, 12) |
                        IsAnalogChanged(pLhsCR->accY, pRhsCR->accY, 12) |
                        IsAnalogChanged(pLhsCR->accZ, pRhsCR->accZ, 12));

                changed |= CalcDpdNoise(
                    p,
                    IsAnalogChanged(pLhsCR->obj[0].x, pRhsCR->obj[0].x, 2) |
                        IsAnalogChanged(pLhsCR->obj[0].y, pRhsCR->obj[0].y, 2) |
                        IsAnalogChanged(pLhsCR->obj[1].x, pRhsCR->obj[1].x, 2) |
                        IsAnalogChanged(pLhsCR->obj[1].y, pRhsCR->obj[1].y, 2) |
                        IsAnalogChanged(pLhsCR->obj[2].x, pRhsCR->obj[2].x, 2) |
                        IsAnalogChanged(pLhsCR->obj[2].y, pRhsCR->obj[2].y, 2) |
                        IsAnalogChanged(pLhsCR->obj[3].x, pRhsCR->obj[3].x, 2) |
                        IsAnalogChanged(pLhsCR->obj[3].y, pRhsCR->obj[3].y, 2));
            }
        }
    }

    if (pLhsCR->err == WPAD_ERR_OK && pRhsCR->err == WPAD_ERR_OK &&
        p->dataFormat - WPAD_FMT_FS_BTN <= 12) {

        switch (p->dataFormat - WPAD_FMT_FS_BTN) {
        case 0:
        case 1:
        case 2: { // FreeStyle (Nunchuk) formats
            WPADFSStatus* pLhsFS = (WPADFSStatus*)pLhs;
            WPADFSStatus* pRhsFS = (WPADFSStatus*)pRhs;

            changed |= CalcExtNoise(
                p,
                IsAnalogChanged(pLhsFS->fsAccX, pRhsFS->fsAccX, 12) |
                    IsAnalogChanged(pLhsFS->fsAccY, pRhsFS->fsAccY, 12) |
                    IsAnalogChanged(pLhsFS->fsAccZ, pRhsFS->fsAccZ, 12));

            changed |= IsAnalogChanged(pLhsFS->fsStickX, pRhsFS->fsStickX, 1);
            changed |= IsAnalogChanged(pLhsFS->fsStickY, pRhsFS->fsStickY, 1);
            break;
        }

        case 3:
        case 4:
        case 5:
        case 8:
        case 12: { // Classic formats
            WPADCLStatus* pLhsCL = (WPADCLStatus*)pLhs;
            WPADCLStatus* pRhsCL = (WPADCLStatus*)pRhs;
            s32 leftStickDiv;
            s32 rightStickDiv;
            s32 triggerDiv;

            switch (p->devMode) {
            case WPAD_DEV_MODE_CLASSIC_REDUCED: {
                leftStickDiv = 16;
                rightStickDiv = 32;
                triggerDiv = 8;
                break;
            }

            case WPAD_DEV_MODE_CLASSIC_STANDARD: {
                leftStickDiv = 4;
                rightStickDiv = 4;
                triggerDiv = 1;
                break;
            }

            default: {
                leftStickDiv = 1;
                rightStickDiv = 1;
                triggerDiv = 1;
                break;
            }
            }

            changed |= IsButtonChanged(pLhsCL->clButton, pRhsCL->clButton);

            changed |= IsAnalogChanged(pLhsCL->clLStickX / leftStickDiv,
                                       pRhsCL->clLStickX / leftStickDiv, 1);
            changed |= IsAnalogChanged(pLhsCL->clLStickY / rightStickDiv,
                                       pRhsCL->clLStickY / rightStickDiv, 1);

            changed |= IsAnalogChanged(pLhsCL->clRStickX / rightStickDiv,
                                       pRhsCL->clRStickX / rightStickDiv, 1);
            changed |= IsAnalogChanged(pLhsCL->clRStickY / rightStickDiv,
                                       pRhsCL->clRStickY / rightStickDiv, 1);

            changed |= IsAnalogChanged(pLhsCL->clTriggerL / triggerDiv,
                                       pRhsCL->clTriggerL / triggerDiv, 1);
            changed |= IsAnalogChanged(pLhsCL->clTriggerR / triggerDiv,
                                       pRhsCL->clTriggerR / triggerDiv, 1);
            break;
        }

        case 7: { // Taiko (TR) format
            WPADTRStatus* pLhsTR = (WPADTRStatus*)pLhs;
            WPADTRStatus* pRhsTR = (WPADTRStatus*)pRhs;

            changed |= IsButtonChanged(pLhsTR->trButton, pRhsTR->trButton);
            changed |= IsAnalogChanged(pLhsTR->brake, pRhsTR->brake, 1);
            changed |= IsAnalogChanged(pLhsTR->mascon, pRhsTR->mascon, 1);
            break;
        }

        case 9: { // Balance Board format
            WPADBLStatus* pLhsBL = (WPADBLStatus*)pLhs;
            WPADBLStatus* pRhsBL = (WPADBLStatus*)pRhs;

            changed |= CalcExtNoise(
                p,
                IsAnalogChanged(pLhsBL->press[0], pRhsBL->press[0], 50) |
                    IsAnalogChanged(pLhsBL->press[1], pRhsBL->press[1], 50) |
                    IsAnalogChanged(pLhsBL->press[2], pRhsBL->press[2], 50) |
                    IsAnalogChanged(pLhsBL->press[3], pRhsBL->press[3], 50));
            break;
        }

        case 10:
        case 11: { // Extended DPD formats
            WPADStatusEx* pLhsEx = (WPADStatusEx*)pLhs;
            WPADStatusEx* pRhsEx = (WPADStatusEx*)pRhs;
            u16* pExt = (u16*)pLhs;

            changed |= IsButtonChanged(*(u8*)&pLhsEx->exp[2].range_x2,
                                       *(u8*)&pRhsEx->exp[2].range_x2);
            changed |= IsAnalogChanged(pLhsEx->exp[2].range_y1,
                                       pRhsEx->exp[2].range_y1, 32);

            for (i = 0; i < 5; i++) {
                changed |= IsAnalogChanged(pExt[0x1B + i], pExt[0x15 + i], 32);
            }
            break;
        }

        case 6: // extended format: no extension compare
            break;
        }
    }

    return changed;
}

void __wpadManageHandler(void) {
    WPADLibStatus status;
    s32 chan;

    status = WUDGetStatus();

    if (status != WPAD_LIB_STATUS_3) {
        if (status == WPAD_LIB_STATUS_2 && !_wpadInitialized) {
            _wpadInitialized = TRUE;
            _wpadReconnectWait = 0x32;
            WUDSetHidConnCallback(__wpadConnectionCallback);
            WUDSetHidRecvCallback(__wpadReceiveCallback);
        }
        return;
    }

    {
        BOOL skip;

        if (_wpadOnReconnect >= 0) {
            s32 onReconnect = _wpadOnReconnect;

            if (--_wpadReconnectWait <= 0) {
                DEBUGPrint("Reconnect Start!!\n");
                BTA_DmSendHciReset();
                OSCancelAlarm(&_wpadManageAlarm);
                WUDSetHidRecvCallback(NULL);
                ((void (*)(BOOL))WUDShutdown)(onReconnect);
            }

            skip = TRUE;
        } else {
            skip = FALSE;
        }

        if (skip) {
            return;
        }
    }

    if (_wpadAfhCnt == 60000) {
        u8* pWifiChannel = OSPhysicalToCached(OS_PHYS_WIFI_AFH_CHANNEL);

        DCInvalidateRange(pWifiChannel, 1);

        DEBUGPrint("Check the update of WiFi using channel\n");

        DEBUGPrint("WiFi uses channel = %d\n", *pWifiChannel);

        if (_wpadAfhChannel != *pWifiChannel) {
            BOOL enabled = OSDisableInterrupts();

            _wpadAfhChannel = *pWifiChannel;

            OSRestoreInterrupts(enabled);

            WUDSetDisableChannel(_wpadAfhChannel);
        }
    }

    if (SCCheckStatus() == SC_STATUS_OK) {
        WUDUpdateSCSetting();

        if (_wpadSCSetting) {
            _wpadDpdSense = __GetDpdSensitivity();
            _wpadSensorBarPos = __GetSensorBarPosition();
            _wpadRumbleFlag = __GetMotorMode();
            _wpadSpeakerVol = __GetSpeakerVolume();

            _wpadSCSetting = FALSE;
        }
    }

    for (chan = WPAD_CHAN0; chan < WPAD_MAX_CONTROLLERS; chan++) {
        WPADCB* p = __rvl_p_wpadcb[chan];

        if (p->used) {
            BOOL success = FALSE;
            if (_wpadExtCnt[chan] == 5) {
                WPADCommand command;

                if (WPADiGetCommand(&p->extCmdQueue, &command)) {
                    if (command.reportID == RPTID_SET_DATA_REPORT_MODE ||
                        p->wpInfo.attach) {

                        WPADCommand command2 = command;

                        s32 devHandle;
                        BOOL enabled;

                        enabled = OSDisableInterrupts();

                        if (__rvl_p_wpadcb[chan]->statusReqBusy) {
                            devHandle = -2;
                        } else {
                            WPADCB* p2 = __rvl_p_wpadcb[chan];
                            BOOL enabled2 = OSDisableInterrupts();

                            devHandle = p2->status;
                            OSRestoreInterrupts(enabled2);
                        }

                        OSRestoreInterrupts(enabled);

                        if (devHandle == 0) {
                            __wpadSendDataSub(chan, command2);
                        }

                        if (devHandle == -2) {
                            if ((s32)OS_TICKS_TO_SEC(__OSGetSystemTime() -
                                                     p->lastReportSendTime) >
                                    1 &&
                                _wpadSleepTime != 0) {

                                p->lastReportSendTime = __OSGetSystemTime();

                                {
                                    s32 st;
                                    WPADCB* p3 = __rvl_p_wpadcb[chan];

                                    enabled = OSDisableInterrupts();
                                    st = p3->status;
                                    OSRestoreInterrupts(enabled);

                                    if (st != WPAD_ERR_NO_CONTROLLER) {
                                        u8 devHandle2;
                                        BD_ADDR addr;
                                        BD_ADDR_PTR pAddr;
                                        WPADCB* p4 = __rvl_p_wpadcb[chan];

                                        enabled = OSDisableInterrupts();
                                        devHandle2 = (u8)p4->devHandle;
                                        OSRestoreInterrupts(enabled);

                                        pAddr = _WUDGetDevAddr(devHandle2);

                                        if (pAddr != NULL) {
                                            memcpy(addr, pAddr,
                                                   sizeof(BD_ADDR));
                                        } else {
                                            memset(addr, 0, sizeof(BD_ADDR));
                                        }

                                        btm_remove_acl(addr);
                                    }
                                }
                            }
                        }

                        if (devHandle == 0) {
                            WPADiPopCommand(&p->extCmdQueue);
                            success = TRUE;
                            _wpadExtCnt[chan] = 0;
                        }
                    }
                }
            }

            _wpadExtCnt[chan] = _wpadExtCnt[chan] == 5
                                    ? _wpadExtCnt[chan]
                                    : _wpadExtCnt[chan] + 1;

            if (!success) {
                WPADCommand command;

                if (WPADiGetCommand(&p->stdCmdQueue, &command)) {
                    if (command.reportID == RPTID_SET_DATA_REPORT_MODE ||
                        p->wpInfo.attach) {

                        WPADCommand command2 = command;

                        s32 devHandle;
                        BOOL enabled;

                        enabled = OSDisableInterrupts();

                        if (__rvl_p_wpadcb[chan]->statusReqBusy) {
                            devHandle = -2;
                        } else {
                            WPADCB* p2 = __rvl_p_wpadcb[chan];
                            BOOL enabled2 = OSDisableInterrupts();

                            devHandle = p2->status;
                            OSRestoreInterrupts(enabled2);
                        }

                        OSRestoreInterrupts(enabled);

                        if (devHandle == 0) {
                            __wpadSendDataSub(chan, command2);
                        }

                        if (devHandle == -2) {
                            if ((s32)OS_TICKS_TO_SEC(__OSGetSystemTime() -
                                                     p->lastReportSendTime) >
                                    1 &&
                                _wpadSleepTime != 0) {

                                p->lastReportSendTime = __OSGetSystemTime();

                                {
                                    s32 st;
                                    WPADCB* p3 = __rvl_p_wpadcb[chan];

                                    enabled = OSDisableInterrupts();
                                    st = p3->status;
                                    OSRestoreInterrupts(enabled);

                                    if (st != WPAD_ERR_NO_CONTROLLER) {
                                        u8 devHandle2;
                                        BD_ADDR addr;
                                        BD_ADDR_PTR pAddr;
                                        WPADCB* p4 = __rvl_p_wpadcb[chan];

                                        enabled = OSDisableInterrupts();
                                        devHandle2 = (u8)p4->devHandle;
                                        OSRestoreInterrupts(enabled);

                                        pAddr = _WUDGetDevAddr(devHandle2);

                                        if (pAddr != NULL) {
                                            memcpy(addr, pAddr,
                                                   sizeof(BD_ADDR));
                                        } else {
                                            memset(addr, 0, sizeof(BD_ADDR));
                                        }

                                        btm_remove_acl(addr);
                                    }
                                }
                            }
                        }

                        if (devHandle == 0) {
                            WPADiPopCommand(&p->stdCmdQueue);
                            success = TRUE;
                        }
                    }
                }
            }

            if (success) {
                p->motorBusy = FALSE;
            } else if (__GetCmdNumber(&p->stdCmdQueue) > 0) {
                p->motorBusy = FALSE;
            } else if (_wpadRumbleCnt[chan] == 5) {
                WPADCommand command;

                p->motorBusy = FALSE;
                command.reportID = RPTID_SET_RUMBLE;
                command.dataLength = RPT10_SIZE;
                command.dataBuf[RPT10_RUMBLE] = FALSE;
                command.cmdCB = NULL;
                __wpadSendDataSub(chan, command);
            }

            _wpadRumbleCnt[chan] =
                p->motorBusy ? _wpadRumbleCnt[chan] + 1 : 0;

            __wpadCalcControllerData(chan);
            __wpadCalcRadioQuality(chan);
        }

        if (p->UNK_0x991) {
            if (p->UNK_0x992-- < 0) {
                p->UNK_0x991 = 0;
            }
        }
    }

    _wpadSenseCnt = _wpadSenseCnt == 10 ? 0 : _wpadSenseCnt + 1;
    _wpadCheckCnt = _wpadCheckCnt == 5 ? 0 : _wpadCheckCnt + 1;
    _wpadAfhCnt = _wpadAfhCnt == 60000 ? 0 : _wpadAfhCnt + 1;

    BTA_HhGetAclQueueInfo();
}

void __wpadManageHandler0(OSAlarm* pAlarm, OSContext* pContext) {
    OSSwitchFiberEx((u32)pAlarm, (u32)pContext, 0, 0, __wpadManageHandler,
                    __wpadManageHandlerStack +
                        sizeof(__wpadManageHandlerStack));
}

void __wpadClearControlBlock(s32 chan) {
    WPADCB* p = __rvl_p_wpadcb[chan];

    p->wpInfoOut = NULL;
    p->motorRunning = FALSE;
    p->cmdBlkCB = NULL;
    p->extensionCB = NULL;
    p->samplingCB = NULL;
    p->samplingBuf = NULL;
    p->samplingBufIndex = 0;
    p->samplingBufSize = 0;
    p->dataFormat = WPAD_FMT_CORE_BTN;
    p->status = WPAD_ERR_NO_CONTROLLER;
    p->devType = WPAD_DEV_NOT_FOUND;
    p->devMode = WPAD_DEV_MODE_NORMAL;
    p->calibrated = FALSE;
    p->comboHeld = 0;
    p->statusReqBusy = FALSE;
    p->defaultDpdSize = 12;
    p->currentDpdCommand = WPAD_DPD_DISABLE;
    p->pendingDpdCommand = 0;
    p->filterDiffAcc = 0;
    p->filterSameAcc = 0;
    p->filterDiffDpd = 0;
    p->filterSameDpd = 0;
    p->filterDiffExt = 0;
    p->filterSameExt = 0;
    p->lastControllerDataUpdate = __OSGetSystemTime();
    p->lastReportSendTime = __OSGetSystemTime();
    p->UNK_0x910 = 0;
    p->UNK_0x8C9 = 0;
    p->UNK_0x8C4 = 0;
    p->wmReadDataPtr = NULL;
    p->wmReadAddress = 0;
    p->wmReadLength = 0;
    p->wmReadHadError = 0;
    p->devHandle = WUD_DEV_HANDLE_INVALID;
    p->used = FALSE;
    p->handshakeFinished = FALSE;
    p->configIndex = 0;
    p->radioQuality = WPAD_RADIO_QUALITY_BAD;
    p->radioQualityOkMs = 0;
    p->audioFrames = 0;
    p->UNK_0x982 = 0;
    p->radioSensitivity = 0;
    p->copyOutCount = 0;
    p->sleeping = TRUE;
    p->getInfoBusy = FALSE;
    p->getInfoCB = NULL;
    p->UNK_0x98E = 0;
    p->UNK_0x98F = 4;
    p->UNK_0x990 = WPAD_DEV_NOT_FOUND;
    p->UNK_0x991 = 0;
    p->UNK_0x992 = 0;

    memset(&p->wpInfo, 0, sizeof(WPADInfo));
    memset(&p->wmReadDataBuf, 0, sizeof(p->wmReadDataBuf));

    memset(&p->devConfig, 0, sizeof(WPADDevConfig));
    memset(&p->extConfig, 0, sizeof(WPADExtConfig));
    memset(&p->encryptionKey, 0, sizeof(p->encryptionKey));
    memset(&p->decryptAddTable, 0, sizeof(p->decryptAddTable));
    memset(&p->decryptXorTable, 0, sizeof(p->decryptXorTable));
    memset(&p->gameInfo, 0, sizeof(WPADGameInfo));

    p->rxBufIndex = 0;

    memset(p->rxBufs, 0, sizeof(p->rxBufs));

    ((WPADStatus*)p->rxBufs[0])->err = WPAD_ERR_NO_CONTROLLER;
    ((WPADStatus*)p->rxBufs[1])->err = WPAD_ERR_NO_CONTROLLER;
    memcpy(p->rxBufMain, p->rxBufs[0], RX_BUFFER_SIZE);

    p->UNK_0x38[0] = -1;

    p->stdCmdQueue.buffer = p->stdCmdQueueList;
    p->stdCmdQueue.capacity = ARRAY_SIZE(p->stdCmdQueueList);

    p->UNK_0x38[1] = -1;

    p->extCmdQueue.buffer = p->extCmdQueueList;
    p->extCmdQueue.capacity = ARRAY_SIZE(p->extCmdQueueList);

    WPADiClearQueue(&p->stdCmdQueue);
    WPADiClearQueue(&p->extCmdQueue);

    WPADiClearMemBlock(chan);

    _wpadExtCnt[chan] = 0;
    _wpadRumbleCnt[chan] = 0;
}

// TODO(kiwi) __rvl_wpadcb should be 32-byte aligned, but doing so breaks this function
static u8 FAKE_ALIGNMENT[0x10];
DECOMP_FORCEACTIVE(WPAD_c, _wpadHandle2PortTable, FAKE_ALIGNMENT);

void __wpadInitSub(void) {
    BOOL enabled;
    s32 chan;
    int i;

    enabled = OSDisableInterrupts();

    ACRWriteReg(ACR_GPIO1BOUT, ACRReadReg(ACR_GPIO1BOUT) | GPIO_SENSORBAR);

    OSRestoreInterrupts(enabled);

    for (i = 0; i < WUD_MAX_DEV_ENTRY; i++) {
        _wpadHandle2PortTable[i] = WUD_DEV_HANDLE_INVALID;
    }

    DEBUGPrint("WPADInit()\n");

    for (chan = 0; chan < WPAD_MAX_CONTROLLERS; chan++) {
        __rvl_p_wpadcb[chan] = &__rvl_wpadcb[chan];
        _wpadIsUsedChannel[chan] = FALSE;

        __rvl_wpadcb[chan].connectCB = NULL;
        __wpadClearControlBlock(chan);
        OSInitThreadQueue(&__rvl_wpadcb[chan].threadQueue);

        _wpadExtCnt[chan] = 0;
        _wpadRumbleCnt[chan] = 0;
    }

    _wpadSleepTime = 5;
    _wpadGameCode = OSGetAppGamename();
    _wpadGameType = OSGetAppType();
    _wpadDpdSense = __GetDpdSensitivity();
    _wpadSensorBarPos = __GetSensorBarPosition();
    _wpadRumbleFlag = __GetMotorMode();
    _wpadSpeakerVol = __GetSpeakerVolume();
    _wpadSenseCnt = 0;
    _wpadCheckCnt = 0;
    _wpadAfhCnt = 0;
    _wpadShutdownFlag = FALSE;
    _wpadSCSetting = TRUE;
    _wpadAfhChannel = -1;
    _wpadUsedCallback = NULL;

    OSRegisterVersion(__WPADVersion);

    if (_wpadVSMInit != NULL) {
        _wpadVSMInit();
    }

    if (_wpadTRNInit != NULL) {
        _wpadTRNInit();
    }

    if (_wpadGTRInit != NULL) {
        _wpadGTRInit();
    }

    if (_wpadDRMInit != NULL) {
        _wpadDRMInit();
    }

    OSCreateAlarm(&_wpadManageAlarm);
    OSSetPeriodicAlarm(&_wpadManageAlarm, OSGetTime(), OS_MSEC_TO_TICKS(1),
                       __wpadManageHandler0);
}

void WPADInit(void) {
    if (!_wpadRegisterShutdownFunc) {
        OSRegisterShutdownFunction(&ShutdownFunctionInfo);
        _wpadRegisterShutdownFunc = TRUE;
    }

    if (WUDInit()) {
        _wpadInitialized = FALSE;
        _wpadOnReconnect = -1;
        _wpadReconnectWait = 0x32;
        __wpadInitSub();
    }
}

DECOMP_INLINE void __WPADShutdown(void) {
    BOOL enabled = OSDisableInterrupts();
    int i;

    if (_wpadShutdownFlag) {
        OSRestoreInterrupts(enabled);
        return;
    }

    _wpadShutdownFlag = TRUE;

    WUDSetVisibility(FALSE, FALSE);

    for (i = WUD_MAX_DEV_ENTRY_FOR_STD + WPAD_CHAN0;
         i < WUD_MAX_DEV_ENTRY_FOR_STD + WPAD_MAX_CONTROLLERS; i++) {
        memset(&_scArray.devices[i], 0, sizeof(SCBtDeviceInfo));
    }

    OSCancelAlarm(&_wpadManageAlarm);

    WUDSetHidRecvCallback(NULL);
    WUDShutdown();

    OSRestoreInterrupts(enabled);
}


BOOL WPADStartSimpleSync(void) {
    return WUDStartSyncSimple();
}

BOOL WPADStartFastSimpleSync(void) {
    return WUDStartFastSyncSimple();
}

BOOL WPADStopSimpleSync(void) {
    return WUDStopSyncSimple();
}

BOOL WPADStartClearDevice(void) {
    return WUDStartClearDevice();
}

WPADSyncDeviceCallback
WPADSetSimpleSyncCallback(WPADSyncDeviceCallback pCallback) {
    return WUDSetSyncSimpleCallback(pCallback);
}

WPADClearDeviceCallback
WPADSetClearDeviceCallback(WPADClearDeviceCallback pCallback) {
    return WUDSetClearDeviceCallback(pCallback);
}

void WPADRegisterAllocator(WPADAllocFunc pAllocFunc, WPADFreeFunc pFreeFunc) {
    WUDRegisterAllocator(pAllocFunc, pFreeFunc);
}

u32 WPADGetWorkMemorySize(void) {
    return WUDGetAllocatedMemSize();
}

WPADLibStatus WPADGetStatus(void) {
    return (WPADLibStatus)WUDGetStatus();
}


u8 WPADGetSensorBarPosition(void) {
    BOOL enabled = OSDisableInterrupts();
    u8 pos = _wpadSensorBarPos;
    OSRestoreInterrupts(enabled);
    return pos;
}

void __wpadSetupConnectionCallback(s32 chan, s32 status) {
    WPADCB* p = __rvl_p_wpadcb[chan];

    if (status == WPAD_ERR_OK) {
        p->handshakeFinished = TRUE;

        if (p->connectCB != NULL) {
            p->connectCB(chan, status);
        }
    } else {
        BOOL enabled;
        s32 st;

        enabled = OSDisableInterrupts();
        st = p->status;
        OSRestoreInterrupts(enabled);

        if (st != WPAD_ERR_NO_CONTROLLER) {
            BOOL enabled;
            u8 devHandle;
            BD_ADDR addr;
            BD_ADDR_PTR pAddr;

            p = __rvl_p_wpadcb[chan];

            enabled = OSDisableInterrupts();
            devHandle = p->devHandle;
            OSRestoreInterrupts(enabled);

            pAddr = _WUDGetDevAddr(devHandle);

            if (pAddr != NULL) {
                memcpy(addr, pAddr, sizeof(BD_ADDR));
            } else {
                memset(addr, 0, sizeof(BD_ADDR));
            }

            btm_remove_acl(addr);
        }
    }
}

void __wpadAbortConnectionCallback(s32 chan, s32 status) {
    WPADCB* p = __rvl_p_wpadcb[chan];

    if (status != WPAD_ERR_OK) {
        WPADiClearQueue(&p->stdCmdQueue);

        {
            BOOL enabled;
            s32 st;

            p = __rvl_p_wpadcb[chan];

            enabled = OSDisableInterrupts();
            st = p->status;
            OSRestoreInterrupts(enabled);

            if (st != WPAD_ERR_NO_CONTROLLER) {
                BOOL enabled;
                u8 devHandle;
                BD_ADDR addr;
                BD_ADDR_PTR pAddr;

                p = __rvl_p_wpadcb[chan];

                enabled = OSDisableInterrupts();
                devHandle = p->devHandle;
                OSRestoreInterrupts(enabled);

                pAddr = _WUDGetDevAddr(devHandle);

                if (pAddr != NULL) {
                    memcpy(addr, pAddr, sizeof(BD_ADDR));
                } else {
                    memset(addr, 0, sizeof(BD_ADDR));
                }

                btm_remove_acl(addr);
            }
        }
    }
}

void __wpadInitConnectionCallback(s32 chan, s32 status) {
    WPADCB* p = __rvl_p_wpadcb[chan];
    BOOL enabled;
    u32 address;
    u16 size;
    u8 port;

    if (status == WPAD_ERR_NO_CONTROLLER) {
        return;
    }

    enabled = OSDisableInterrupts();

    p->configIndex = status == WPAD_ERR_OK ? 1 : 0;
    p->status = WPAD_ERR_OK;

    OSRestoreInterrupts(enabled);

    size = status == WPAD_ERR_OK ? 20 : 42;
    address =
        status == WPAD_ERR_OK ? WM_ADDR_MEM_176C : WM_ADDR_MEM_DEV_CONFIG_0;

    DEBUGPrint(" ==>this error means that the firmware is for NDEV %s\n",
               p->configIndex != 0 ? "2.0" : "2.1 or later");

    if (p->devType == 3) {
        port = 1;
    } else {
        port = (u8)(1 << chan);
    }

    WPADiSendSetReportType(&p->stdCmdQueue, WPAD_FMT_CORE_BTN, p->UNK_0x98E,
                           &__wpadAbortConnectionCallback);

    WPADiSendDPDCSB(&p->stdCmdQueue, FALSE, &__wpadAbortConnectionCallback);

    WPADiSendSetPort(&p->stdCmdQueue, port, &__wpadAbortConnectionCallback);

    WPADiSendReadData(&p->stdCmdQueue, p->wmReadDataBuf,
                      sizeof(WPADGameInfo), WM_ADDR_MEM_GAME_INFO_0,
                      &__wpadAbortConnectionCallback);

    WPADiSendReadData(&p->stdCmdQueue, p->wmReadDataBuf,
                      sizeof(WPADGameInfo), WM_ADDR_MEM_GAME_INFO_1,
                      &__wpadAbortConnectionCallback);

    WPADiSendReadData(&p->stdCmdQueue, p->wmReadDataBuf, size, address,
                      &__wpadSetupConnectionCallback);

    WPADiSendGetContStat(&p->stdCmdQueue, NULL, NULL);
}

s32 __wpadRetrieveChannel(WUDDevInfo* pInfo) {
    BD_ADDR_PTR pAddr;
    s32 result = WPAD_CHAN_INVALID;
    s32 i;

    pAddr = _WUDGetDevAddr(pInfo->devHandle);

    if (memcmp(pInfo->conf.devName, lbl_805607C4, 16) == 0) {
        result = 3;

        if (__rvl_p_wpadcb[3]->used) {
            btm_remove_acl(pAddr);
            return WPAD_CHAN_INVALID;
        }
    } else {
        for (i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
            if (WUDIsLatestDevice(i, pAddr) && !_wpadIsUsedChannel[i]) {
                result = i;
                break;
            }

            if (!_wpadIsUsedChannel[i] && result < 0) {
                result = i;
            }
        }
    }

    if (!WUDIsLatestDevice(result, pAddr)) {
        WUDSetDeviceHistory(result, pAddr);
    }

    _wpadIsUsedChannel[result] = TRUE;

    return result;
}

void __wpadConnectionCallback(WUDDevInfo* pInfo, u8 open) {
    UINT8 devHandle = pInfo->devHandle;
    const char* pStr = lbl_80560608;
    s32 chan;

    DEBUGPrint(pStr + 0x1D0, open ? "opened" : "closed");

    if (open) {
        WPADCB* p;
        WPADCommand command;

        chan = __wpadRetrieveChannel(pInfo);

        if (chan < WPAD_CHAN0) {
            return;
        }

        _wpadHandle2PortTable[devHandle] = chan;

        __wpadClearControlBlock(chan);

        p = __rvl_p_wpadcb[chan];

        if (memcmp(pInfo->conf.devName, pStr + 0x1E4, 16) == 0) {
            p->devType = WPAD_DEV_CORE;
            p->dataFormat = WPAD_FMT_CORE_BTN;
        } else if (memcmp(pInfo->conf.devName, pStr + 0x1BC, 16) == 0 &&
                   WUDIsLinkedWBC()) {

            p->devType = 3; // WBC
            p->dataFormat = 0xC;
        } else {
            p->devType = WPAD_DEV_FUTURE;
            p->dataFormat = 0;
        }

        p->devHandle = devHandle;
        p->used = TRUE;
        p->status = WPAD_ERR_OK;
        p->radioSensitivity = 100;
        p->sleeping = FALSE;
        p->UNK_0x98D = FALSE;
        p->UNK_0x990 = p->devType;

        {
            u32 readAddr = WM_ADDR_MEM_1770;
            u16 readLen = 1;

            command.reportID = RPTID_READ_DATA;
            command.dataLength = RPT17_SIZE;
            command.cmdCB = __wpadInitConnectionCallback;
            memcpy(&command.dataBuf[RPT17_DATA_SRC_ADDRESS], &readAddr,
                   sizeof(u32));
            memcpy(&command.dataBuf[RPT17_DATA_LENGTH], &readLen,
                   sizeof(u16));
            command.dstBuf = p->wmReadDataBuf;
            command.readLength = readLen;
            command.readAddress = readAddr;

            WPADiPushCommand(&p->stdCmdQueue, command);
        }

        __VIResetRFIdle();
    } else {
        WPADCB* p;
        WPADCommand command;

        chan = _wpadHandle2PortTable[devHandle];
        _wpadHandle2PortTable[devHandle] = WUD_DEV_HANDLE_INVALID;

        if (chan != WUD_DEV_HANDLE_INVALID) {
            p = __rvl_p_wpadcb[chan];
            p->status = WPAD_ERR_NO_CONTROLLER;

            if (p->cmdBlkCB != NULL) {
                p->cmdBlkCB(chan, WPAD_ERR_NO_CONTROLLER);
            } else if (p->memBlock_0x10 != NULL) {
                p->memBlock_0x10(chan, WPAD_ERR_NO_CONTROLLER);
            }

            while (WPADiGetCommand(&p->stdCmdQueue, &command)) {
                if (command.cmdCB != NULL) {
                    command.cmdCB(chan, WPAD_ERR_NO_CONTROLLER);
                }

                WPADiPopCommand(&p->stdCmdQueue);
            }

            DEBUGPrint(pStr + 0x1F8);

            if (p->samplingBuf != NULL) {
                WPADSetAutoSamplingBuf(chan, p->samplingBuf,
                                       p->samplingBufSize);
            }

            __wpadClearControlBlock(chan);
            _wpadIsUsedChannel[chan] = FALSE;

            if (p->connectCB != NULL) {
                p->connectCB(chan, WPAD_ERR_NO_CONTROLLER);
            }
        } else {
            DEBUGPrint(pStr + 0x210);
        }
    }
}

void __wpadReceiveCallback(UINT8 devHandle, UINT8* pReport, UINT16 len) {
    s32 result;
    u8 port = _wpadHandle2PortTable[devHandle];

    if (port < WPAD_MAX_CONTROLLERS) {
        result = WPADiHIDParser(port, pReport);

        if (result != 0) {
            DEBUGPrint(lbl_80560860, result);
        }
    }
}

static s32 WPADiGetStatus(s32 chan) {
    WPADCB* p = __rvl_p_wpadcb[chan];
    BOOL enabled = OSDisableInterrupts();

    s32 status = p->status;

    OSRestoreInterrupts(enabled);
    return status;
}

void WPADGetAccGravityUnit(s32 chan, u32 type, WPADAccGravityUnit* pAcc) {
    WPADCB* p = __rvl_p_wpadcb[chan];
    BOOL enabled = OSDisableInterrupts();

    if (pAcc != NULL) {
        switch (type) {
        case WPAD_ACC_GRAVITY_UNIT_CORE: {
            pAcc->x = p->devConfig.accX1g - p->devConfig.accX0g;
            pAcc->y = p->devConfig.accY1g - p->devConfig.accY0g;
            pAcc->z = p->devConfig.accZ1g - p->devConfig.accZ0g;
            break;
        }

        case WPAD_ACC_GRAVITY_UNIT_FS: {
            pAcc->x = p->extConfig.u.fs.accX1g - p->extConfig.u.fs.accX0g;
            pAcc->y = p->extConfig.u.fs.accY1g - p->extConfig.u.fs.accY0g;
            pAcc->z = p->extConfig.u.fs.accZ1g - p->extConfig.u.fs.accZ0g;
            break;
        }
        }
    }

    OSRestoreInterrupts(enabled);
}

void __wpadDisconnectCallback(s32 chan, s32 status) {
    WPADCB* p = __rvl_p_wpadcb[chan];

    if (status != WPAD_ERR_NO_CONTROLLER) {
        BTA_HhClose(p->devHandle);
    }
}

static void WPADiDisconnect(s32 chan, BOOL sleep) {
    WPADCB* p = __rvl_p_wpadcb[chan];
    s32 status;
    BOOL enabled;

    status = WPADiGetStatus(chan);
    if (status == WPAD_ERR_NO_CONTROLLER) {
        return;
    }

    if (sleep) {
        enabled = OSDisableInterrupts();

        if (p->sleeping) {
            OSRestoreInterrupts(enabled);
            return;
        }

        p->sleeping = TRUE;

        OSRestoreInterrupts(enabled);
        WPADControlLed(chan, 0, __wpadDisconnectCallback);
    } else {
        BD_ADDR addr;
        BD_ADDR_PTR pDevAddr;
        WPADCB* p2 = __rvl_p_wpadcb[chan];
        BOOL enabled = OSDisableInterrupts();
        s8 devHandle = p2->devHandle;

        OSRestoreInterrupts(enabled);

        pDevAddr = _WUDGetDevAddr(devHandle);

        if (pDevAddr != NULL) {
            WUD_BDCPY(addr, pDevAddr);
        } else {
            memset(addr, 0, BD_ADDR_LEN);
        }

        btm_remove_acl(addr);
    }
}

void WPADDisconnect(s32 chan) {
    s32 status;

    memset(&_scArray.devices[WUD_MAX_DEV_ENTRY_FOR_STD + chan], 0,
           sizeof(SCBtDeviceInfo));

    _scFlush = TRUE;

    WPADiDisconnect(chan, TRUE);
}

s32 WPADProbe(s32 chan, s32* pDevType) {
    WPADCB* p = __rvl_p_wpadcb[chan];
    BOOL enabled;
    s32 status;

    enabled = OSDisableInterrupts();

    if (pDevType != NULL) {
        *pDevType = p->devType;
    }

    status = p->status;

    if (status != WPAD_ERR_NO_CONTROLLER) {
        if (p->devType == WPAD_DEV_NOT_FOUND) {
            status = WPAD_ERR_NO_CONTROLLER;
        } else if (!p->handshakeFinished) {
            status = WPAD_ERR_COMMUNICATION_ERROR;
        }
    }

    OSRestoreInterrupts(enabled);
    return status;
}

WPADSamplingCallback WPADSetSamplingCallback(s32 chan,
                                             WPADSamplingCallback pCallback) {

    WPADCB* p;
    BOOL enabled;
    WPADSamplingCallback pOldCallback;

    DEBUGPrint("WPADSetSamplingCallback()\n");

    p = __rvl_p_wpadcb[chan];
    enabled = OSDisableInterrupts();

    pOldCallback = p->samplingCB;
    p->samplingCB = pCallback;

    OSRestoreInterrupts(enabled);
    return pOldCallback;
}

WPADConnectCallback WPADSetConnectCallback(s32 chan,
                                           WPADConnectCallback pCallback) {

    WPADCB* p;
    BOOL enabled;
    WPADConnectCallback pOldCallback;

    DEBUGPrint("WPADSetConnectCallback()\n");

    enabled = OSDisableInterrupts();
    p = __rvl_p_wpadcb[chan];

    pOldCallback = p->connectCB;
    p->connectCB = pCallback;

    OSRestoreInterrupts(enabled);
    return pOldCallback;
}

WPADExtensionCallback
WPADSetExtensionCallback(s32 chan, WPADExtensionCallback pCallback) {

    WPADCB* p;
    BOOL enabled;
    WPADExtensionCallback pOldCallback;

    DEBUGPrint("WPADSetExtensionCallback()\n");

    enabled = OSDisableInterrupts();
    p = __rvl_p_wpadcb[chan];

    pOldCallback = p->extensionCB;
    p->extensionCB = pCallback;

    OSRestoreInterrupts(enabled);
    return pOldCallback;
}

u32 WPADGetDataFormat(s32 chan) {
    BOOL enabled = OSDisableInterrupts();
    WPADCB* p = __rvl_p_wpadcb[chan];

    u32 format = p->dataFormat;

    OSRestoreInterrupts(enabled);
    return format;
}

s32 WPADSetDataFormat(s32 chan, u32 format) {
    WPADCB* p = __rvl_p_wpadcb[chan];
    s32 status;
    BOOL enabled;
    BOOL handshake;
    u32 currFormat;

    enabled = OSDisableInterrupts();

    handshake = p->handshakeFinished;
    status = p->status;
    currFormat = p->dataFormat;

    OSRestoreInterrupts(enabled);

    if (status == WPAD_ERR_NO_CONTROLLER) {
        goto _end;
    }

    if (!handshake) {
        status = WPAD_ERR_COMMUNICATION_ERROR;
        goto _end;
    }

    if (currFormat != format) {
        if (!WPADiSendSetReportType(&p->stdCmdQueue, format, p->UNK_0x98E,
                                    NULL)) {
            status = WPAD_ERR_COMMUNICATION_ERROR;
        } else {
            enabled = OSDisableInterrupts();
            p->dataFormat = format;
            OSRestoreInterrupts(enabled);

            status = WPAD_ERR_OK;
        }
    } else {
        status = WPAD_ERR_OK;
    }

_end:
    return status;
}


s32 WPADGetInfoAsync(s32 chan, WPADInfo* pInfo, WPADCallback pCallback) {
    BOOL handshake;
    s32 status;
    WPADCB* p = __rvl_p_wpadcb[chan];
    BOOL enabled;
    u8 getInfoBusy;

    enabled = OSDisableInterrupts();

    handshake = p->handshakeFinished;
    status = p->status;
    getInfoBusy = p->getInfoBusy;

    OSRestoreInterrupts(enabled);

    if (status == WPAD_ERR_NO_CONTROLLER) {
        goto _end;
    }

    if (!handshake || getInfoBusy) {
        status = WPAD_ERR_COMMUNICATION_ERROR;
        goto _end;
    }

    enabled = OSDisableInterrupts();

    p->getInfoBusy = TRUE;
    p->getInfoCB = pCallback;

    OSRestoreInterrupts(enabled);

    if (WPADiSendGetContStat(&p->stdCmdQueue, pInfo, __wpadInfoCallback)) {
        status = WPAD_ERR_OK;
        goto _end;
    }

    status = WPAD_ERR_COMMUNICATION_ERROR;

    enabled = OSDisableInterrupts();

    p->getInfoBusy = FALSE;
    p->getInfoCB = NULL;

    OSRestoreInterrupts(enabled);

_end:
    if (status != WPAD_ERR_OK && pCallback != NULL) {
        pCallback(chan, status);
    }

    return status;
}

void WPADControlMotor(s32 chan, u32 command) {
    WPADCB* p = __rvl_p_wpadcb[chan];
    BOOL enabled = OSDisableInterrupts();
    s32 status = p->status;

    if (status == WPAD_ERR_NO_CONTROLLER) {
        OSRestoreInterrupts(enabled);
        return;
    }

    if (!_wpadRumbleFlag && (command != WPAD_MOTOR_STOP || p->motorRunning != TRUE)) {
        OSRestoreInterrupts(enabled);
        return;
    }

    if (command == WPAD_MOTOR_STOP && p->motorRunning == FALSE ||
        command == WPAD_MOTOR_RUMBLE && p->motorRunning == TRUE) {
        OSRestoreInterrupts(enabled);
        return;
    }

    p->motorRunning = command == WPAD_MOTOR_STOP ? FALSE : TRUE;
    p->motorBusy = TRUE;

    OSRestoreInterrupts(enabled);
}

void WPADEnableMotor(BOOL enable) {
    BOOL enabled = OSDisableInterrupts();
    _wpadRumbleFlag = enable;
    OSRestoreInterrupts(enabled);
}

BOOL WPADIsMotorEnabled(void) {
    BOOL enabled = OSDisableInterrupts();
    BOOL motorEnabled = _wpadRumbleFlag;
    OSRestoreInterrupts(enabled);

    return motorEnabled;
}

s32 WPADControlLed(s32 chan, u8 flags, WPADCallback pCallback) {
    WPADCB* p = __rvl_p_wpadcb[chan];
    BOOL success;
    BOOL enabled;
    BOOL handshake;
    s32 status;

    status = WPAD_ERR_OK;

    enabled = OSDisableInterrupts();
    status = p->status;
    handshake = p->handshakeFinished;
    OSRestoreInterrupts(enabled);

    if (status != WPAD_ERR_NO_CONTROLLER) {
        if (!handshake) {
            status = WPAD_ERR_COMMUNICATION_ERROR;
        } else {
            success = WPADiSendSetPort(&p->stdCmdQueue, flags, pCallback);
            status = success ? 0 : WPAD_ERR_COMMUNICATION_ERROR;
        }
    }

    if (status != WPAD_ERR_OK && pCallback != NULL) {
        pCallback(chan, status);
    }

    return status;
}

BOOL WPADSaveConfig(WPADSaveCallback pCallback) {
    BOOL success = TRUE;
    BOOL enabled;
    u8 volume;
    u8 mode;

    if (SCCheckStatus() != SC_STATUS_OK) {
        return FALSE;
    }

    enabled = OSDisableInterrupts();
    volume = _wpadSpeakerVol;
    mode = _wpadRumbleFlag ? SC_MOTOR_ON : SC_MOTOR_OFF;
    OSRestoreInterrupts(enabled);

    success &= SCSetWpadSpeakerVolume(volume);
    success &= SCSetWpadMotorMode(mode);

    if (success) {
        SCFlushAsync(pCallback);
    } else if (pCallback != NULL) {
        pCallback(SC_STATUS_FATAL);
    }

    return success;
}

void WPADRead(s32 chan, WPADStatus* pStatus) {
    WPADCB* p = __rvl_p_wpadcb[chan];
    BOOL enabled = OSDisableInterrupts();
    u8 rxBufIndex = p->rxBufIndex != 0 ? 0 : 1;
    WPADStatus* pRxStatus = (WPADStatus*)p->rxBufs[rxBufIndex];
    u16 size;

    switch (p->dataFormat) {
    case WPAD_FMT_FS_BTN:
    case WPAD_FMT_FS_BTN_ACC:
    case WPAD_FMT_FS_BTN_ACC_DPD:
        size = 0x32;
        break;

    case WPAD_FMT_CLASSIC_BTN:
        size = 0x5A;
        break;

    case WPAD_FMT_CLASSIC_BTN_ACC:
        size = 0x2E;
        break;

    case WPAD_FMT_CLASSIC_BTN_ACC_DPD:
        size = 0x36;
        break;

    case WPAD_FMT_TR_BTN:
        size = 0x34;
        break;

    case WPAD_FMT_TR_BTN_ACC:
    case WPAD_FMT_BTN_ACC_DPD_EXTENDED:
        size = 0x4A;
        break;

    case WPAD_FMT_WBC_BTN_ACC:
        size = 0x36;
        break;

    default:
        size = 0x2A;
        break;
    }

    if (pRxStatus->err != 0) {
        size = 0x2A;
    }

    memcpy(pStatus, pRxStatus, size);

    OSRestoreInterrupts(enabled);
}

void WPADSetAutoSamplingBuf(s32 chan, void* pBuffer, u32 len) {
    WPADCB* p;
    BOOL enabled;
    s8 defaultErr;
    s32 fmtSize;
    int i;

    p = __rvl_p_wpadcb[chan];
    enabled = OSDisableInterrupts();

    defaultErr = p->status == WPAD_ERR_NO_CONTROLLER ? WPAD_ERR_NO_CONTROLLER
                                                     : WPAD_ERR_INVALID;

    switch (p->dataFormat) {
    case WPAD_FMT_FS_BTN:
    case WPAD_FMT_FS_BTN_ACC:
    case WPAD_FMT_FS_BTN_ACC_DPD:
        fmtSize = 0x32;
        break;

    case WPAD_FMT_CLASSIC_BTN:
    case WPAD_FMT_CLASSIC_BTN_ACC:
    case WPAD_FMT_CLASSIC_BTN_ACC_DPD:
    case WPAD_FMT_BTN_ACC_DPD_EXTENDED:
    case 15:
        fmtSize = 0x36;
        break;

    case WPAD_FMT_TR_BTN:
        fmtSize = 0x5A;
        break;

    case WPAD_FMT_TR_BTN_ACC:
        fmtSize = 0x2E;
        break;

    case WPAD_FMT_WBC_BTN_ACC:
        fmtSize = 0x34;
        break;

    case 13:
    case 14:
        fmtSize = 0x4A;
        break;

    default:
        fmtSize = 0x2A;
        break;
    }

    if (pBuffer != NULL) {
        memset(pBuffer, 0, fmtSize * len);

        for (i = 0; i < len; i++) {
            ((WPADStatus*)((u8*)pBuffer + fmtSize * i))->err = defaultErr;
        }

        p->samplingBufIndex = -1;
        p->samplingBufSize = len;
    }

    p->samplingBuf = pBuffer;

    OSRestoreInterrupts(enabled);
}

void WPADiExcludeButton(s32 chan) {
    WPADCB* p = __rvl_p_wpadcb[chan];
    BOOL enabled = OSDisableInterrupts();

    u8 rxBufIndex = p->rxBufIndex != 0 ? 0 : 1;
    void* pRxBuffer = p->rxBufs[rxBufIndex];

    WPADStatus* pStatus;
    WPADCLStatus* pStatusCL;

    pStatus = (WPADStatus*)pRxBuffer;

    if ((pStatus->button & (WPAD_BUTTON_LEFT | WPAD_BUTTON_RIGHT)) ==
        (WPAD_BUTTON_LEFT | WPAD_BUTTON_RIGHT)) {

        pStatus->button &= ~WPAD_BUTTON_RIGHT;
    }

    if ((pStatus->button & (WPAD_BUTTON_UP | WPAD_BUTTON_DOWN)) ==
        (WPAD_BUTTON_UP | WPAD_BUTTON_DOWN)) {

        pStatus->button &= ~WPAD_BUTTON_DOWN;
    }

    if (p->dataFormat == WPAD_FMT_CLASSIC_BTN ||
        p->dataFormat == WPAD_FMT_CLASSIC_BTN_ACC ||
        p->dataFormat == WPAD_FMT_CLASSIC_BTN_ACC_DPD) {
        pStatusCL = (WPADCLStatus*)pRxBuffer;

        if ((pStatusCL->clButton &
             (WPAD_BUTTON_CL_LEFT | WPAD_BUTTON_CL_RIGHT)) ==
            (WPAD_BUTTON_CL_LEFT | WPAD_BUTTON_CL_RIGHT)) {

            pStatusCL->clButton &= ~WPAD_BUTTON_CL_RIGHT;
        }

        if ((pStatusCL->clButton & (WPAD_BUTTON_CL_UP | WPAD_BUTTON_CL_DOWN)) ==
            (WPAD_BUTTON_CL_UP | WPAD_BUTTON_CL_DOWN)) {

            pStatusCL->clButton &= ~WPAD_BUTTON_CL_DOWN;
        }
    }

    OSRestoreInterrupts(enabled);
}

void WPADiCopyOut(s32 chan) {
    WPADCB* p = __rvl_p_wpadcb[chan];
    BOOL enabled = OSDisableInterrupts();

    u8 rxBufIndex = p->rxBufIndex != 0 ? 0 : 1;
    WPADStatus* pStatus = (WPADStatus*)p->rxBufs[rxBufIndex];

    if (p->samplingBuf != NULL) {
        if (++p->samplingBufIndex >= p->samplingBufSize) {
            p->samplingBufIndex = 0;
        }

        if (p->dataFormat == WPAD_FMT_CORE_BTN ||
            p->dataFormat == WPAD_FMT_CORE_BTN_ACC ||
            p->dataFormat == WPAD_FMT_CORE_BTN_ACC_DPD) {

            memcpy(&p->samplingBuf[p->samplingBufIndex], pStatus,
                   sizeof(WPADStatus));

        } else if (p->dataFormat == WPAD_FMT_FS_BTN ||
                   p->dataFormat == WPAD_FMT_FS_BTN_ACC ||
                   p->dataFormat == WPAD_FMT_FS_BTN_ACC_DPD) {

            memcpy(&p->samplingBufFS[p->samplingBufIndex], pStatus,
                   !pStatus->err ? sizeof(WPADFSStatus) : sizeof(WPADStatus));

        } else if (p->dataFormat == WPAD_FMT_CLASSIC_BTN ||
                   p->dataFormat == WPAD_FMT_CLASSIC_BTN_ACC ||
                   p->dataFormat == WPAD_FMT_CLASSIC_BTN_ACC_DPD) {

            memcpy(&p->samplingBufCL[p->samplingBufIndex], pStatus,
                   !pStatus->err ? sizeof(WPADCLStatus) : sizeof(WPADStatus));

        } else {
            memcpy(&p->samplingBufEx[p->samplingBufIndex], pStatus,
                   !pStatus->err ? sizeof(WPADStatusEx) : sizeof(WPADStatus));
        }
    }

    if (p->samplingCB != NULL) {
        p->samplingCB(chan);
    }

    p->copyOutCount++;
    OSRestoreInterrupts(enabled);
}

BOOL WPADIsSpeakerEnabled(s32 chan) {
    WPADCB* p = __rvl_p_wpadcb[chan];
    BOOL enabled = OSDisableInterrupts();

    BOOL spkEnabled = p->wpInfo.speaker;

    OSRestoreInterrupts(enabled);
    return spkEnabled;
}

s32 WPADControlSpeaker(s32 chan, u32 command, WPADCallback pCallback) {
    WPADCB* p;
    BOOL enabled;
    BOOL spkEnable;
    BOOL handshake;
    s32 status;

    // clang-format off
    u8 config[SPK_CONFIG_SIZE] = {0x00,
                                  0x00,                            // 4-bit ADPCM
                                  0xD0, 0x07,                      // 3000Hz (little-endian)
                                  WPAD_MAX_SPEAKER_VOLUME / 2 + 1, // Default volume
                                  0x0C, 0x0E                       // ???
    };
    // clang-format on

    p = __rvl_p_wpadcb[chan];

    enabled = OSDisableInterrupts();

    spkEnable = p->wpInfo.speaker;
    status = p->status;
    handshake = p->handshakeFinished;

    OSRestoreInterrupts(enabled);

    if (status == WPAD_ERR_NO_CONTROLLER) {
        goto _end;
    }

    if (!handshake) {
        status = WPAD_ERR_COMMUNICATION_ERROR;
        goto _end;
    }

    if (command == WPAD_SPEAKER_OFF) {
        if (!spkEnable) {
            status = WPAD_ERR_OK;
        } else {
            enabled = OSDisableInterrupts();

            if (__CanPushCmdQueue(&p->stdCmdQueue, 5)) {
                WPADiSendMuteSpeaker(&p->stdCmdQueue, TRUE, NULL);

                WPADiSendWriteDataCmd(&p->stdCmdQueue, 0x01, WM_REG_SPEAKER_01,
                                      NULL);
                WPADiSendWriteDataCmd(&p->stdCmdQueue, 0x00, WM_REG_SPEAKER_09,
                                      NULL);

                WPADiSendEnableSpeaker(&p->stdCmdQueue, FALSE, NULL);

                WPADiSendGetContStat(&p->stdCmdQueue, NULL, pCallback);

                OSRestoreInterrupts(enabled);
                return WPAD_ERR_OK;
            }

            status = WPAD_ERR_COMMUNICATION_ERROR;
            OSRestoreInterrupts(enabled);
        }
    } else {
        switch (command) {
        case WPAD_SPEAKER_ON:
        case WPAD_SPEAKER_5: {
            enabled = OSDisableInterrupts();

            if (__CanPushCmdQueue(&p->stdCmdQueue, 7)) {
                WPADiSendEnableSpeaker(&p->stdCmdQueue, TRUE, NULL);
                WPADiSendMuteSpeaker(&p->stdCmdQueue, TRUE, NULL);

                WPADiSendWriteDataCmd(&p->stdCmdQueue, 0x01, WM_REG_SPEAKER_09,
                                      NULL);
                // Sends 0x80 instead of 0x08?
                WPADiSendWriteDataCmd(&p->stdCmdQueue, 0x80, WM_REG_SPEAKER_01,
                                      NULL);

                config[4] = _wpadSpeakerVol;

                WPADiSendWriteData(&p->stdCmdQueue, &config, SPK_CONFIG_SIZE,
                                   WM_REG_SPEAKER_01, NULL);

                WPADiSendMuteSpeaker(&p->stdCmdQueue, FALSE, NULL);

                WPADiSendGetContStat(&p->stdCmdQueue, NULL, pCallback);

                OSRestoreInterrupts(enabled);
                return WPAD_ERR_OK;
            }

            status = WPAD_ERR_COMMUNICATION_ERROR;

            OSRestoreInterrupts(enabled);
            break;
        }

        case WPAD_SPEAKER_MUTE: {
            if (!WPADiSendMuteSpeaker(&p->stdCmdQueue, TRUE, pCallback)) {
                status = WPAD_ERR_COMMUNICATION_ERROR;
                break;
            }

            return WPAD_ERR_OK;
        }

        case WPAD_SPEAKER_UNMUTE: {
            if (!WPADiSendMuteSpeaker(&p->stdCmdQueue, FALSE, pCallback)) {
                status = WPAD_ERR_COMMUNICATION_ERROR;
                break;
            }

            return WPAD_ERR_OK;
        }

        case WPAD_SPEAKER_PLAY: {
            if (!WPADiSendWriteDataCmd(&p->stdCmdQueue, 0x01, WM_REG_SPEAKER_08,
                                       pCallback)) {

                status = WPAD_ERR_COMMUNICATION_ERROR;
                break;
            }

            return WPAD_ERR_OK;
        }
        }
    }

_end:
    if (pCallback != NULL) {
        pCallback(chan, status);
    }

    return status;
}

u8 WPADGetSpeakerVolume(void) {
    BOOL enabled = OSDisableInterrupts();
    u8 volume = _wpadSpeakerVol;
    OSRestoreInterrupts(enabled);
    return volume;
}

void WPADSetSpeakerVolume(u8 volume) {
    BOOL enabled = OSDisableInterrupts();
    _wpadSpeakerVol = __ClampSpeakerVolume(volume);
    OSRestoreInterrupts(enabled);
}

BOOL __wpadIsBusyStream(s32 chan) {
    BOOL enabled;
    WPADCB* p = __rvl_p_wpadcb[chan];
    u8 radioQuality;
    u32 devType;

    u8 bufferStatus;
    u16 btmBufferStatus;

    u8 audioFrames;
    s8 queueSize;
    u8 linkNumber;
    BOOL enabled2;

    enabled = OSDisableInterrupts();

    radioQuality = p->radioQuality;
    devType = p->devType;
    bufferStatus = WUDGetBufferStatus();

    enabled2 = OSDisableInterrupts();

    queueSize = p->stdCmdQueue.back - p->stdCmdQueue.front;
    if (queueSize < 0) {
        queueSize = (s8)(queueSize + p->stdCmdQueue.capacity);
    }

    OSRestoreInterrupts(enabled2);

    {
        WPADCB* p2 = __rvl_p_wpadcb[chan];
        BOOL enabled = OSDisableInterrupts();
        s32 status = p2->status;
        s8 devHandle = p2->devHandle;
        OSRestoreInterrupts(enabled);

        if (status != WPAD_ERR_NO_CONTROLLER) {
            _WUDGetQueuedSize(devHandle);
        }
    }

    {
        WPADCB* p2 = __rvl_p_wpadcb[chan];
        BOOL enabled = OSDisableInterrupts();
        s32 status = p2->status;
        s8 devHandle = p2->devHandle;
        OSRestoreInterrupts(enabled);

        if (status == WPAD_ERR_NO_CONTROLLER) {
            btmBufferStatus = 0;
        } else {
            btmBufferStatus = _WUDGetNotAckedSize(devHandle);
        }
    }

    audioFrames = p->audioFrames;

    linkNumber = _WUDGetLinkNumber();

    OSRestoreInterrupts(enabled);

    if (radioQuality != WPAD_RADIO_QUALITY_GOOD || btmBufferStatus > 3 ||
        bufferStatus == 10 || bufferStatus >= linkNumber * 2 + 2 ||
        devType == WPAD_DEV_UNKNOWN || queueSize >= 21 || audioFrames >= 1) {

        return TRUE;
    } else {
        return FALSE;
    }
}

BOOL WPADCanSendStreamData(s32 chan) {
    WPADCB* p = __rvl_p_wpadcb[chan];
    BOOL enabled;
    BOOL handshake;
    s32 status;

    enabled = OSDisableInterrupts();
    status = p->status;
    handshake = p->handshakeFinished;
    OSRestoreInterrupts(enabled);

    if (status == WPAD_ERR_NO_CONTROLLER || !handshake || __wpadIsBusyStream(chan)) {

        return FALSE;
    } else {
        return TRUE;
    }
}

s32 WPADSendStreamData(s32 chan, void* pData, u16 len) {
    WPADCB* p = __rvl_p_wpadcb[chan];
    BOOL enabled;
    BOOL handshake;
    s32 status;

    enabled = OSDisableInterrupts();
    status = p->status;
    handshake = p->handshakeFinished;
    OSRestoreInterrupts(enabled);

    if (status == WPAD_ERR_NO_CONTROLLER) {
        return WPAD_ERR_NO_CONTROLLER;
    }

    if (!handshake) {
        return WPAD_ERR_COMMUNICATION_ERROR;
    }

    if (__wpadIsBusyStream(chan)) {
        return WPAD_ERR_COMMUNICATION_ERROR;
    }

    if (!WPADiSendStreamData(&p->stdCmdQueue, pData, len)) {
        return WPAD_ERR_COMMUNICATION_ERROR;
    }

    enabled = OSDisableInterrupts();
    p->audioFrames++;
    OSRestoreInterrupts(enabled);

    return WPAD_ERR_OK;
}

u8 WPADGetDpdSensitivity(void) {
    return _wpadDpdSense;
}


BOOL WPADIsDpdEnabled(s32 chan) {
    WPADCB* p = __rvl_p_wpadcb[chan];
    BOOL enabled = OSDisableInterrupts();

    BOOL dpdEnabled = p->wpInfo.dpd;

    OSRestoreInterrupts(enabled);
    return dpdEnabled;
}


s32 WPADControlDpd(s32 chan, u32 command, WPADCallback pCallback) {
    WPADCB* p = __rvl_p_wpadcb[chan];
    BOOL enabled;
    BOOL dpdEnabled;
    BOOL handshake;
    u8 currCmd;
    u8 pendingCmd;
    s32 status;

    // clang-format off
    static const u8 cfg1[WPAD_MAX_DPD_SENS][DPD_CONFIG1_SIZE] = {
        {0x02, 0x00, 0x00, 0x71, 0x01, 0x00, 0x64, 0x00, 254},
        {0x02, 0x00, 0x00, 0x71, 0x01, 0x00, 0x96, 0x00, 180},
        {0x02, 0x00, 0x00, 0x71, 0x01, 0x00, 0xAA, 0x00, 100},
        {0x02, 0x00, 0x00, 0x71, 0x01, 0x00, 0xC8, 0x00,  54},
        {0x07, 0x00, 0x00, 0x71, 0x01, 0x00, 0x72, 0x00,  32}
    };
    // clang-format on

    // clang-format off
    static const u8 cfg2[WPAD_MAX_DPD_SENS][DPD_CONFIG2_SIZE] = {
        {0xFD, 0x05},
        {0xB3, 0x04},
        {0x63, 0x03},
        {0x35, 0x03},
        {0x1F, 0x03}
    };
    // clang-format on

    enabled = OSDisableInterrupts();

    dpdEnabled = p->wpInfo.dpd;
    currCmd = p->currentDpdCommand;
    pendingCmd = p->pendingDpdCommand;
    status = p->status;
    handshake = p->handshakeFinished;

    OSRestoreInterrupts(enabled);

    if (status == WPAD_ERR_NO_CONTROLLER) {
        goto _end;
    }

    if (!handshake) {
        status = WPAD_ERR_COMMUNICATION_ERROR;
        goto _end;
    }

    if (command == WPAD_DPD_DISABLE) {
        if (!dpdEnabled) {
            status = WPAD_ERR_OK;
        } else {
            enabled = OSDisableInterrupts();

            if (__CanPushCmdQueue(&p->stdCmdQueue, 3)) {
                p->pendingDpdCommand = command;

                WPADiSendEnableDPD(&p->stdCmdQueue, FALSE, NULL);
                WPADiSendDPDCSB(&p->stdCmdQueue, FALSE, &__wpadDpdCallback);

                WPADiSendGetContStat(&p->stdCmdQueue, NULL, pCallback);

                OSRestoreInterrupts(enabled);
                return WPAD_ERR_OK;
            }

            status = WPAD_ERR_COMMUNICATION_ERROR;

            OSRestoreInterrupts(enabled);
        }
    } else if (command != pendingCmd) {
        enabled = OSDisableInterrupts();

        if (__CanPushCmdQueue(&p->stdCmdQueue, 8)) {
            p->pendingDpdCommand = command;

            WPADiSendEnableDPD(&p->stdCmdQueue, TRUE, NULL);
            WPADiSendDPDCSB(&p->stdCmdQueue, TRUE, NULL);

            WPADiSendWriteDataCmd(&p->stdCmdQueue, 0x01, WM_REG_DPD_30, NULL);

            WPADiSendWriteData(&p->stdCmdQueue, cfg1[_wpadDpdSense - 1],
                               DPD_CONFIG1_SIZE, WM_REG_DPD_CONFIG_BLOCK_1,
                               NULL);

            WPADiSendWriteData(&p->stdCmdQueue, cfg2[_wpadDpdSense - 1],
                               DPD_CONFIG2_SIZE, WM_REG_DPD_CONFIG_BLOCK_2,
                               NULL);

            WPADiSendWriteDataCmd(&p->stdCmdQueue, command,
                                  WM_REG_DPD_DATA_FORMAT, NULL);
            WPADiSendWriteDataCmd(&p->stdCmdQueue, 0x08, WM_REG_DPD_30,
                                  &__wpadDpdCallback);

            WPADiSendGetContStat(&p->stdCmdQueue, NULL, pCallback);

            OSRestoreInterrupts(enabled);
            return WPAD_ERR_OK;
        }

        status = WPAD_ERR_COMMUNICATION_ERROR;

        OSRestoreInterrupts(enabled);
    }

_end:
    if (pCallback != NULL) {
        pCallback(chan, status);
    }

    return status;
}

void __wpadSendDataSub(s32 chan, WPADCommand command) {
    BOOL enabled;
    BOOL rumble;
    s8 devHandle;
    WPADCB* p;
    UINT8 reportID;
    BT_HDR* pBuffer;
    u8* pRptData;
    s32 status;
    u8* pCmdData;
    UINT16 len;

    p = __rvl_p_wpadcb[chan];
    reportID = command.reportID;
    pCmdData = command.dataBuf;
    len = command.dataLength;

    enabled = OSDisableInterrupts();

    devHandle = p->devHandle;
    status = p->status;

    if (devHandle < 0) {
        OSRestoreInterrupts(enabled);
        return;
    }

    p->status = WPAD_ERR_COMMUNICATION_ERROR;
    rumble = p->motorRunning & _wpadRumbleFlag;

    if (reportID == RPTID_SET_RUMBLE) {
        p->status = status;
    } else if (reportID == RPTID_SEND_SPEAKER_DATA) {
        p->status = status;
        p->audioFrames--;
    } else {
        switch (reportID) {
        case RPTID_WRITE_DATA: {
            break;
        }

        case RPTID_READ_DATA: {
            p->wmReadHadError = 0;
            p->wmReadAddress = command.readAddress;
            p->wmReadLength = command.readLength;
            p->wmReadDataPtr = command.dstBuf;
            break;
        }

        case RPTID_REQUEST_STATUS: {
            p->status = status;
            p->wpInfoOut = command.statusReportOut;
            p->statusReqBusy = TRUE;
            break;
        }

        default: {
            pCmdData[0] |= (1 << RPT_OUT_FLAG_REQUEST_ACK_RPT);
            break;
        }
        }
    }

    p->cmdBlkCB = command.cmdCB;
    p->lastReportID = reportID;
    p->lastReportSendTime = __OSGetSystemTime() + OS_SEC_TO_TICKS(2);
    p->UNK_0x910 = 0;

    OSRestoreInterrupts(enabled);

    DEBUGPrint(lbl_805606C8, devHandle, reportID);

    pBuffer = GKI_getbuf((u8)(len + 18));
    pBuffer->len = (u8)(len + 1);
    pBuffer->offset = 10;

    pRptData = &((u8*)(pBuffer + 1))[pBuffer->offset];
    pRptData[0] = reportID;
    memcpy(&pRptData[1], pCmdData, len);

    if (rumble) {
        pRptData[RPT_OUT_FLAGS] |= (1 << RPT_OUT_FLAG_RUMBLE);
    } else {
        pRptData[RPT_OUT_FLAGS] &= ~(1 << RPT_OUT_FLAG_RUMBLE);
    }

    BTA_HhSendData((UINT8)devHandle, pBuffer);
}

static BOOL WPADiSendSetPort(WPADCommandQueue* pQueue, u8 port,
                      WPADCallback pCallback) {
    BOOL success;
    WPADCommand command;

    command.reportID = RPTID_SET_PORT;
    command.dataLength = RPT11_SIZE;
    command.dataBuf[RPT11_LED] = port << 4;
    command.cmdCB = pCallback;

    success = WPADiPushCommand(pQueue, command);
    return success;
}

BOOL WPADiSendSetReportType(WPADCommandQueue* pQueue, s32 format,
                            BOOL contReport, WPADCallback pCallback) {
    BOOL success;
    WPADCommand command;

    command.reportID = RPTID_SET_DATA_REPORT_MODE;
    command.dataLength = RPT12_SIZE;
    command.dataBuf[RPT12_CONT_REPORT] = contReport ? 0 : 4;
    command.cmdCB = pCallback;

    switch (format) {
    case WPAD_FMT_CORE_BTN: {
        command.dataBuf[RPT12_DATA_REPORT_MODE] = RPTID_DATA_BTN;
        break;
    }
    case WPAD_FMT_CORE_BTN_ACC: {
        command.dataBuf[RPT12_DATA_REPORT_MODE] = RPTID_DATA_BTN_ACC;
        break;
    }

    case WPAD_FMT_CORE_BTN_ACC_DPD: {
        command.dataBuf[RPT12_DATA_REPORT_MODE] = RPTID_DATA_BTN_ACC_DPD12;
        break;
    }

    case WPAD_FMT_FS_BTN: {
        command.dataBuf[RPT12_DATA_REPORT_MODE] = RPTID_DATA_BTN_EXT8;
        break;
    }

    case WPAD_FMT_FS_BTN_ACC: {
        command.dataBuf[RPT12_DATA_REPORT_MODE] = RPTID_DATA_BTN_ACC_EXT16;
        break;
    }

    case WPAD_FMT_FS_BTN_ACC_DPD: {
        command.dataBuf[RPT12_DATA_REPORT_MODE] = RPTID_DATA_BTN_ACC_DPD10_EXT9;
        break;
    }

    case WPAD_FMT_CLASSIC_BTN: {
        command.dataBuf[RPT12_DATA_REPORT_MODE] = RPTID_DATA_BTN_EXT8;
        break;
    }

    case WPAD_FMT_CLASSIC_BTN_ACC: {
        command.dataBuf[RPT12_DATA_REPORT_MODE] = RPTID_DATA_BTN_ACC_EXT16;
        break;
    }

    case WPAD_FMT_CLASSIC_BTN_ACC_DPD: {
        command.dataBuf[RPT12_DATA_REPORT_MODE] = RPTID_DATA_BTN_ACC_DPD10_EXT9;
        break;
    }

    case WPAD_FMT_TR_BTN: {
        command.dataBuf[RPT12_DATA_REPORT_MODE] = RPTID_DATA_BTN_ACC_DPD18_1;
        break;
    }

    case WPAD_FMT_TR_BTN_ACC: {
        command.dataBuf[RPT12_DATA_REPORT_MODE] = RPTID_DATA_BTN_EXT8;
        break;
    }

    case WPAD_FMT_BTN_ACC_DPD_EXTENDED: {
        command.dataBuf[RPT12_DATA_REPORT_MODE] = RPTID_DATA_BTN_ACC_DPD10_EXT9;
        break;
    }

    case 15: {
        command.dataBuf[RPT12_DATA_REPORT_MODE] = RPTID_DATA_BTN_ACC_DPD10_EXT9;
        break;
    }

    case WPAD_FMT_WBC_BTN_ACC: {
        command.dataBuf[RPT12_DATA_REPORT_MODE] = RPTID_DATA_BTN_EXT19;
        break;
    }

    case 13: {
        command.dataBuf[RPT12_DATA_REPORT_MODE] = RPTID_DATA_BTN_ACC_EXT16;
        break;
    }

    case 14: {
        command.dataBuf[RPT12_DATA_REPORT_MODE] = RPTID_DATA_EXT21;
        break;
    }
    }

    success = WPADiPushCommand(pQueue, command);
    return success;
}

static BOOL WPADiSendEnableDPD(WPADCommandQueue* pQueue, BOOL enable,
                        WPADCallback pCallback) {
    BOOL success;
    WPADCommand command;

    command.reportID = RPTID_ENABLE_DPD;
    command.dataLength = RPT13_SIZE;
    command.dataBuf[RPT13_DPD_ENABLE] = enable ? 4 : 0;
    command.cmdCB = pCallback;

    success = WPADiPushCommand(pQueue, command);
    return success;
}

static BOOL WPADiSendEnableSpeaker(WPADCommandQueue* pQueue, BOOL enable,
                            WPADCallback pCallback) {
    BOOL success;
    WPADCommand command;

    command.reportID = RPTID_ENABLE_SPEAKER;
    command.dataLength = RPT14_SIZE;
    command.dataBuf[RPT14_SPEAKER_ENABLE] = enable ? 4 : 0;
    command.cmdCB = pCallback;

    success = WPADiPushCommand(pQueue, command);
    return success;
}

static BOOL WPADiSendGetContStat(WPADCommandQueue* pQueue, WPADInfo* pInfo,
                          WPADCallback pCallback) {
    BOOL success;
    WPADCommand command;

    command.reportID = RPTID_REQUEST_STATUS;
    command.dataLength = RPT15_SIZE;
    command.dataBuf[0] = 0;
    command.cmdCB = pCallback;
    command.statusReportOut = pInfo;

    success = WPADiPushCommand(pQueue, command);
    return success;
}

BOOL WPADiSendWriteDataCmd(WPADCommandQueue* pQueue, u8 cmd, u32 addr,
                           WPADCallback pCallback) {

    return WPADiSendWriteData(pQueue, &cmd, sizeof(u8), addr, pCallback);
}

BOOL WPADiSendWriteData(WPADCommandQueue* pQueue, const void* pSrc, u16 len,
                        u32 addr, WPADCallback pCallback) {
    BOOL success;
    WPADCommand command;
    u8 packedLen = len & 31;

    command.reportID = RPTID_WRITE_DATA;
    command.dataLength = RPT16_SIZE;
    command.cmdCB = pCallback;
    memcpy(&command.dataBuf[RPT16_DATA_DST_ADDRESS], &addr, sizeof(u32));
    memcpy(&command.dataBuf[RPT16_DATA_LENGTH], &packedLen, sizeof(u8));
    memcpy(&command.dataBuf[RPT16_DATA], pSrc, len);

    success = WPADiPushCommand(pQueue, command);
    return success;
}

BOOL WPADiSendReadData(WPADCommandQueue* pQueue, void* pDst, u16 len, u32 addr,
                       WPADCallback pCallback) {
    BOOL success;
    WPADCommand command;

    command.reportID = RPTID_READ_DATA;
    command.dataLength = RPT17_SIZE;
    command.cmdCB = pCallback;
    memcpy(&command.dataBuf[RPT17_DATA_SRC_ADDRESS], &addr, sizeof(u32));
    memcpy(&command.dataBuf[RPT17_DATA_LENGTH], &len, sizeof(u16));
    command.dstBuf = pDst;
    command.readLength = len;
    command.readAddress = addr;

    success = WPADiPushCommand(pQueue, command);
    return success;
}

static BOOL WPADiSendStreamData(WPADCommandQueue* pQueue, const void* pData, u16 len) {
    BOOL success;
    WPADCommand command;
    u8 packedLen = len << 3;

    command.reportID = RPTID_SEND_SPEAKER_DATA;
    command.dataLength = sizeof(command.dataBuf);
    command.dataBuf[RPT18_DATA_LENGTH] = packedLen;
    command.cmdCB = NULL;
    memcpy(&command.dataBuf[RPT18_DATA], pData, len);

    success = WPADiPushCommand(pQueue, command);
    return success;
}

static BOOL WPADiSendMuteSpeaker(WPADCommandQueue* pQueue, BOOL mute,
                          WPADCallback pCallback) {
    BOOL success;
    WPADCommand command;

    command.reportID = RPTID_MUTE_SPEAKER;
    command.dataLength = RPT19_SIZE;
    command.dataBuf[RPT19_SPEAKER_MUTE] = mute ? 4 : 0;
    command.cmdCB = pCallback;

    success = WPADiPushCommand(pQueue, command);
    return success;
}

static BOOL WPADiSendDPDCSB(WPADCommandQueue* pQueue, BOOL enable,
                     WPADCallback pCallback) {
    BOOL success;
    WPADCommand command;

    command.reportID = RPTID_SEND_DPD_CSB;
    command.dataLength = RPT1A_SIZE;
    command.dataBuf[RPT1A_DPD_CSB] = enable ? 4 : 0;
    command.cmdCB = pCallback;

    success = WPADiPushCommand(pQueue, command);
    return success;
}

BOOL __CanPushCmdQueue(const WPADCommandQueue* pQueue, s8 num) {
    s8 queueSize = __GetCmdNumber(pQueue);

    if ((u32)(queueSize + num) <= pQueue->capacity - 1) {
        return TRUE;
    } else {
        return FALSE;
    }
}

s8 __GetCmdNumber(const WPADCommandQueue* pQueue) {
    BOOL enabled;
    s8 remain;

    enabled = OSDisableInterrupts();

    remain = pQueue->back - pQueue->front;
    if (remain < 0) {
        remain = (s8)(remain + (s32)pQueue->capacity);
    }

    OSRestoreInterrupts(enabled);

    return remain;
}

void WPADiClearQueue(WPADCommandQueue* pQueue) {
    BOOL enabled = OSDisableInterrupts();

    pQueue->front = 0;
    pQueue->back = 0;
    memset(pQueue->buffer, 0, pQueue->capacity * sizeof(WPADCommand));

    OSRestoreInterrupts(enabled);
}

static BOOL WPADiPushCommand(WPADCommandQueue* pQueue, WPADCommand command) {
    BOOL enabled = OSDisableInterrupts();

    if (pQueue->capacity - 1 == __GetCmdNumber(pQueue)) {
        OSRestoreInterrupts(enabled);
        return FALSE;
    }

    // @bug Useless memset
    memset(&pQueue->buffer[pQueue->back], 0, sizeof(WPADCommand));
    memcpy(&pQueue->buffer[pQueue->back], &command, sizeof(WPADCommand));

    pQueue->back = pQueue->back == pQueue->capacity - 1 ? 0 : pQueue->back + 1;

    OSRestoreInterrupts(enabled);

    return TRUE;
}

static BOOL WPADiGetCommand(const WPADCommandQueue* pQueue, WPADCommand* pCommand) {
    BOOL enabled = OSDisableInterrupts();
    s8 remain = (s8)(pQueue->back - pQueue->front);

    if (remain < 0) {
        remain = (s8)(remain + pQueue->capacity);
    }

    OSRestoreInterrupts(enabled);

    if (remain == 0) {
        return FALSE;
    }

    enabled = OSDisableInterrupts();

    memcpy(pCommand, &pQueue->buffer[pQueue->front], sizeof(WPADCommand));

    OSRestoreInterrupts(enabled);
    return TRUE;
}

static BOOL WPADiPopCommand(WPADCommandQueue* pQueue) {
    BOOL enabled = OSDisableInterrupts();

    if (__GetCmdNumber(pQueue) == 0) {
        OSRestoreInterrupts(enabled);
        return FALSE;
    }

    memset(&pQueue->buffer[pQueue->front], 0, sizeof(WPADCommand));

    pQueue->front =
        pQueue->front == pQueue->capacity - 1 ? 0 : pQueue->front + 1;

    OSRestoreInterrupts(enabled);

    return TRUE;
}

static void __SetScreenSaverFlag(BOOL disable) {
    if (disable) {
        __VIResetRFIdle();
    }
}

static u8 __GetDpdSensitivity(void) {
    u8 result = SCGetBtDpdSensibility();

    if (result < WPAD_MIN_DPD_SENS) {
        result = WPAD_MIN_DPD_SENS;
    }

    if (result > WPAD_MAX_DPD_SENS) {
        result = WPAD_MAX_DPD_SENS;
    }

    return result;
}

static u8 __GetSensorBarPosition(void) {
    // SC_SENSOR_BAR_TOP ? WPAD_SENSOR_BAR_TOP : WPAD_SENSOR_BAR_BOTTOM
    return !!(SCGetWpadSensorBarPosition() == 1);
}

static u32 __GetMotorMode(void) {
    // SC_MOTOR_ON ? WPAD_MOTOR_ON : WPAD_MOTOR_OFF
    return !!(SCGetWpadMotorMode() == 1);
}

static u8 __ClampSpeakerVolume(u8 volume) {
    u8 result = volume;

    if (volume <= 0) {
        result = 0;
    }

    if (volume >= WPAD_MAX_SPEAKER_VOLUME) {
        result = WPAD_MAX_SPEAKER_VOLUME;
    }

    return result;
}

static u8 __GetSpeakerVolume(void) {
    u8 volume = SCGetWpadSpeakerVolume();
    return __ClampSpeakerVolume(volume);
}

void* __wpadNoAlloc(u32 size) {
#pragma unused(size)

    return NULL;
}

BOOL __wpadNoFree(void* pBlock) {
#pragma unused(pBlock)

    return FALSE;
}
extern int _enabledVSM;
int WPADIsEnabledVSM() { return _enabledVSM; }
extern int _enabledTRN;
int WPADIsEnabledTRN() { return _enabledTRN; }
int WPADIsEnabledGTR() { extern int _enabledGTR; return _enabledGTR; }
extern int _enabledDRM;

int WPADIsEnabledDRM() {
    return _enabledDRM;
}
int WBCSetupCalibration() { return 0; }
int WBCGetCalibrationStatus() { return 0; }
int WBCGetBatteryLevel() { return 0; }
int WBCReadDummy() {
    extern const char lbl_80560650[];
    DEBUGPrint(lbl_80560650);
    return -1;
}
int WBCSetZEROPointDummy() {
    extern char lbl_80560660[];
    DEBUGPrint(lbl_80560660);
    return -1;
}
int WBCGetTGCWeightDummy(void) {
    extern char lbl_80560678[];
    DEBUGPrint(lbl_80560678);
    return -1;
}
void __wpadCalcRecalibration(s32 chan, WPADStatus* pStatus) {
    WPADCB* p = __rvl_p_wpadcb[chan];
    u8 count = 0;

    if (p->dataFormat - WPAD_FMT_FS_BTN <= 2) {
        if (pStatus->button ==
            (WPAD_BUTTON_A | WPAD_BUTTON_B | WPAD_BUTTON_PLUS | WPAD_BUTTON_MINUS)) {
            count = 1;
        }
    }

    if (p->dataFormat - WPAD_FMT_CLASSIC_BTN <= 2) {
        WPADCLStatus* pStatusCL = (WPADCLStatus*)pStatus;

        if (pStatus->button ==
                (WPAD_BUTTON_A | WPAD_BUTTON_B | WPAD_BUTTON_PLUS |
                 WPAD_BUTTON_MINUS) ||
            (pStatusCL->clButton ==
                 (WPAD_BUTTON_CL_A | WPAD_BUTTON_CL_B | WPAD_BUTTON_CL_PLUS |
                  WPAD_BUTTON_CL_MINUS) &&
             pStatusCL->err == WPAD_ERR_OK)) {
            count = 1;
        }
    }

    p->comboHeld += count;

    if (p->comboHeld > 600) {
        WPADCB* pReset = __rvl_p_wpadcb[chan];
        BOOL enabled = OSDisableInterrupts();

        pReset->calibrated = FALSE;
        pReset->comboHeld = 0;

        OSRestoreInterrupts(enabled);
    }
}

void __wpadCalcControllerData(s32 chan) {
    WPADCB* p = __rvl_p_wpadcb[chan];
    BOOL changed = FALSE;
    BOOL dataChanged;
    BOOL enabled;
    u8 rxBufIndex;
    u8* pRxBuf;

    if (_wpadCheckCnt != 5) {
        return;
    }

    enabled = OSDisableInterrupts();

    rxBufIndex = p->rxBufIndex == 0 ? 1 : 0;
    pRxBuf = p->rxBufs[rxBufIndex];

    OSRestoreInterrupts(enabled);

    dataChanged = __wpadIsControllerDataChanged(p, pRxBuf, p->rxBufMain);
    __wpadCalcRecalibration(chan, (WPADStatus*)pRxBuf);

    if (dataChanged) {
        changed = TRUE;
        p->lastControllerDataUpdate = __OSGetSystemTime();
        memcpy(p->rxBufMain, pRxBuf, RX_BUFFER_SIZE);
    } else if (_wpadSleepTime != 0) {
        s32 time =
            (s32)OS_TICKS_TO_SEC(__OSGetSystemTime() - p->lastControllerDataUpdate);

        if (time > _wpadSleepTime * 60) {
            BOOL enabled;
            s32 status;

            p = __rvl_p_wpadcb[chan];

            enabled = OSDisableInterrupts();
            status = p->status;
            OSRestoreInterrupts(enabled);

            if (status != WPAD_ERR_NO_CONTROLLER) {
                BOOL enabled;
                u8 devHandle;
                BD_ADDR addr;
                BD_ADDR_PTR pAddr;

                p = __rvl_p_wpadcb[chan];

                enabled = OSDisableInterrupts();
                devHandle = p->devHandle;
                OSRestoreInterrupts(enabled);

                pAddr = _WUDGetDevAddr(devHandle);

                if (pAddr != NULL) {
                    memcpy(addr, pAddr, sizeof(BD_ADDR));
                } else {
                    memset(addr, 0, sizeof(BD_ADDR));
                }

                btm_remove_acl(addr);
            }
        }
    }

    if (((WPADStatus*)p->rxBufMain)->err != WPAD_ERR_OK &&
        ((WPADStatus*)p->rxBufMain)->err != WPAD_ERR_CORRUPTED) {
        memcpy(p->rxBufMain, pRxBuf, RX_BUFFER_SIZE);
    }

    if (changed) {
        __VIResetRFIdle();
    }
}


u8 WPADGetRadioSensitivity(s32 chan) {
    WPADCB* p = __rvl_p_wpadcb[chan];
    u8 sensitivity;
    BOOL enabled = OSDisableInterrupts();

    sensitivity = p->radioSensitivity;

    OSRestoreInterrupts(enabled);

    return sensitivity;
}
void WPADSetAutoSleepTime(int min) {
    BOOL enabled = OSDisableInterrupts();

    _wpadSleepTime = min;

    OSRestoreInterrupts(enabled);
}

void __wpadInfoCallback(s32 chan, s32 status) {
#pragma unused(status)

    WPADCB* p = __rvl_p_wpadcb[chan];

    if (p->getInfoCB != NULL) {
        ((void (*)(s32))p->getInfoCB)(chan);
    }

    p->getInfoCB = NULL;
    p->getInfoBusy = FALSE;
}

u8 WPADGetDpdFormat(s32 chan) {
    WPADCB* p = __rvl_p_wpadcb[chan];
    u8 format;
    BOOL enabled = OSDisableInterrupts();

    format = p->wpInfo.dpd ? p->currentDpdCommand : 0;

    OSRestoreInterrupts(enabled);

    return format;
}

void __wpadDpdCallback(s32 chan, s32 status) {
#pragma unused(status)

    WPADCB* p = __rvl_p_wpadcb[chan];

    p->currentDpdCommand = p->pendingDpdCommand;
    p->dpdBusy = 0;
    p->wpInfo.dpd = p->pendingDpdCommand != 0;
}

s32 WPADControlBLC(s32 chan, u8 command, WPADCallback pCallback) {
    WPADCB* p = __rvl_p_wpadcb[chan];
    BOOL enabled;
    s32 status;
    BOOL handshake;
    u8 buf[7];

    enabled = OSDisableInterrupts();
    status = p->status;
    handshake = p->handshakeFinished;
    OSRestoreInterrupts(enabled);

    if (status == WPAD_ERR_NO_CONTROLLER) {
    } else if (!handshake || !WUDIsLinkedWBC()) {
        status = WPAD_ERR_COMMUNICATION_ERROR;
    } else {
        buf[0] = buf[1] = buf[2] = 0xAA;
        buf[3] = 0x55;
        buf[4] = buf[5] = buf[6] = command;

        enabled = OSDisableInterrupts();

        switch (command) {
        case 0xAA: {
            BOOL enabled2;
            s8 queueSize;

            enabled2 = OSDisableInterrupts();

            queueSize = p->stdCmdQueue.back - p->stdCmdQueue.front;
            if (queueSize < 0) {
                queueSize = (s8)(queueSize + p->stdCmdQueue.capacity);
            }

            OSRestoreInterrupts(enabled2);

            if ((u32)(queueSize + 4) <= p->stdCmdQueue.capacity - 1) {
                WPADWriteExtReg(chan, buf, 7, 0xF1, NULL);
                WPADWriteExtReg(chan, buf, 1, 0xF1, NULL);
                WPADWriteExtReg(chan, buf, 1, 0xF1, NULL);
                WPADWriteExtReg(chan, buf, 1, 0xF1, pCallback);

                OSRestoreInterrupts(enabled);
                return WPAD_ERR_OK;
            }

            status = WPAD_ERR_COMMUNICATION_ERROR;
            break;
        }

        case 0x55: {
            status = WPADWriteExtReg(chan, buf, 7, 0xF1,
                                    pCallback);

            if (status != WPAD_ERR_OK) {
                break;
            }

            OSRestoreInterrupts(enabled);
            return WPAD_ERR_OK;
        }

        case 0x00: {
            status = WPADWriteExtReg(chan, buf, 1, 0xF1,
                                    pCallback);

            if (status != WPAD_ERR_OK) {
                break;
            }

            OSRestoreInterrupts(enabled);
            return WPAD_ERR_OK;
        }

        default: {
            status = WPAD_ERR_COMMUNICATION_ERROR;
            break;
        }
        }

        OSRestoreInterrupts(enabled);
    }

    if (pCallback != NULL) {
        pCallback(chan, status);
    }

    return status;
}
void* WPADIsUsedCallbackByKPAD() {
    return _wpadUsedCallback;
}
void WPADSetCallbackByKPAD(void (*callback)(void))
{
    _wpadUsedCallback = callback;
}
