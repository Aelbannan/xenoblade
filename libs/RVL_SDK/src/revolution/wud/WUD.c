#include "revolution/WUD/WUDInternal.h"

#include <revolution/BTE.h>
#include <revolution/NAND.h>
#include <revolution/OS.h>
#include <revolution/SC.h>
#include <revolution/WPAD.h>
#include <revolution/WUD.h>

#include <string.h>

#define DEBUGPrint WUD_DEBUGPrint

#define BT_VSC_NINTENDO_INSTALL_PATCH 0xFC4F
#define BT_VSC_NINTENDO_WRITE_PATCH 0xFC4C


void WUDSecurityCallback(tBTA_DM_SEC_EVT event, tBTA_DM_SEC* pData);
void WUDSearchCallback(tBTA_DM_SEARCH_EVT event, tBTA_DM_SEARCH* pData);
void WUDVendorSpecificCallback(UINT8 len, UINT8* pData);
void WUDDeviceStatusCallback(tBTM_DEV_STATUS status);
void WUDStoredLinkKeyCallback(void* p1);

static void WUDiRemovePatch(void);
static void WUDiWritePatch(void);
static void WUDiInstallPatch(void);



void __wudSyncHandler(void);
void __wudDeleteHandler(void);
void __wudStackHandler(void);
void __wudInitHandler(void);
void __wudShutdownHandler(void);
void __wudInitSub(void);
void WUDiMoveTopOfUnusedStdDevice(WUDDevInfo* pInfo);
void __wudLinkKeyEventStackCallback(void* p1);
void __wudSecurityEventStackCallback(tBTA_DM_SEC_EVT event,
                                     tBTA_DM_SEC* pData);
void __wudVendorSpecificEventStackCallback(UINT8 len, UINT8* pData);
void __wudSearchEventStackCallback(tBTA_DM_SEARCH_EVT event,
                                   tBTA_DM_SEARCH* pData);
void __wudDeviceStatusEventStackCallback(tBTM_DEV_STATUS status);
void __wudPowerMangeEventStackCallback(BD_ADDR addr, tBTM_PM_STATUS status,
                                       UINT16 value, UINT8 hciStatus);
void __wudSyncHandler0(OSAlarm* pAlarm, OSContext* pContext);
void __wudInitHandler0(OSAlarm* pAlarm, OSContext* pContext);
void __wudShutdownHandler0(OSAlarm* pAlarm, OSContext* pContext);
void __wudClearControlBlock(void);
void __wudAppendRuntimePatch(void);
BOOL __wudStartSyncDevice(u8 syncType, s8 syncLoopNum, u8 target,
                          BOOL syncSkipChecks);

extern int _wudNandPhase;
extern unsigned char _wudNandLocked;

extern int _wudReadNand;
extern int _wudAbortSync;
extern unsigned char _wudPatchRemoveCmd;
extern unsigned char _wudTarget;
extern int _linkedWBC;

extern char _wudWiiRemoteDescriptor[];
extern unsigned char _wudInstallNum;
extern unsigned char _wudPatchNum;
extern unsigned int _wudPatchAddress;
extern unsigned int _wudPatchOffset;
extern unsigned int _wudPatchSize;

extern u8 _wudDiscNumResps;
extern s8 _wudDiscRssi;
extern u8* _wudNandBufPtr;
extern u8 _wudNandWbcInfo[0xA0];
extern u32 _wudNandWbcCrc;
extern NANDFileInfo _wudNandFileInfo;
extern NANDCommandBlock _wudNandBlock;

extern char lbl_80665D68_80663438[];
extern char lbl_80665D70_80663440[];
extern char lbl_80665D78_80663448[];

void SCGetBtCmpDevInfoArray(SCBtCmpDevInfoArray* info);
BOOL SCSetBtCmpDevInfoArray(const SCBtCmpDevInfoArray* info);

WUDCB __rvl_wudcb;
WUDDevInfo _wudDiscWork;
WUDDiscResp _wudDiscResp;
SCBtDeviceInfoArray _scArray;
SCBtCmpDevInfoArray _spArray;
u8 __WUDHandlerStack[0x1000] ALIGN(32);

BD_ADDR_PTR _dev_handle_to_bda[WUD_MAX_DEV_ENTRY];
u16 _dev_handle_queue_size[WUD_MAX_DEV_ENTRY];
u16 _dev_handle_notack_num[WUD_MAX_DEV_ENTRY];

int _wudInitialized;
static u8 __bte_trace_level = BT_TRACE_LEVEL_NONE;

static OSAlarm _arm;

// _wudDiscNumResps/_wudDiscRssi are externs above

// clang-format off
static u8 descriptor[] = {
    0x05, 0x01, // Usage Page (Generic Desktop Ctrls)
    0x09, 0x05, // Usage (Game Pad)

    0xA1, 0x01, // Collection (Application)

    0x85, 0x10,       // Report ID (16)
    0x15, 0x00,       // Logical Minimum (0)
    0x26, 0xFF, 0x00, // Logical Maximum (255)
    0x75, 0x08,       // Report Size (8)
    0x95, 0x01,       // Report Count (1)
    0x06, 0x00, 0xFF, // Usage Page (Vendor Defined 0xFF00)
    0x09, 0x01,       // Usage (0x01)
    0x91, 0x00,       // Output (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)

    0x85, 0x11, // Report ID (17)
    0x95, 0x01, // Report Count (1)
    0x09, 0x01, // Usage (0x01)
    0x91, 0x00, // Output (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)

    0x85, 0x12, // Report ID (18)
    0x95, 0x02, // Report Count (2)
    0x09, 0x01, // Usage (0x01)
    0x91, 0x00, // Output (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)

    0x85, 0x13, // Report ID (19)
    0x95, 0x01, // Report Count (1)
    0x09, 0x01, // Usage (0x01)
    0x91, 0x00, // Output (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)

    0x85, 0x14, // Report ID (20)
    0x95, 0x01, // Report Count (1)
    0x09, 0x01, // Usage (0x01)
    0x91, 0x00, // Output (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)

    0x85, 0x15, // Report ID (21)
    0x95, 0x01, // Report Count (1)
    0x09, 0x01, // Usage (0x01)
    0x91, 0x00, // Output (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)

    0x85, 0x16, // Report ID (22)
    0x95, 0x15, // Report Count (21)
    0x09, 0x01, // Usage (0x01)
    0x91, 0x00, // Output (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)

    0x85, 0x17, // Report ID (23)
    0x95, 0x06, // Report Count (6)
    0x09, 0x01, // Usage (0x01)
    0x91, 0x00, // Output (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)

    0x85, 0x18, // Report ID (24)
    0x95, 0x15, // Report Count (21)
    0x09, 0x01, // Usage (0x01)
    0x91, 0x00, // Output (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)

    0x85, 0x19, // Report ID (25)
    0x95, 0x01, // Report Count (1)
    0x09, 0x01, // Usage (0x01)
    0x91, 0x00, // Output (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)

    0x85, 0x1A, // Report ID (26)
    0x95, 0x01, // Report Count (1)
    0x09, 0x01, // Usage (0x01)
    0x91, 0x00, // Output (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)

    0x85, 0x20, // Report ID (32)
    0x95, 0x06, // Report Count (6)
    0x09, 0x01, // Usage (0x01)
    0x81, 0x00, // Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)

    0x85, 0x21, // Report ID (33)
    0x95, 0x15, // Report Count (21)
    0x09, 0x01, // Usage (0x01)
    0x81, 0x00, // Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)

    0x85, 0x22, // Report ID (34)
    0x95, 0x04, // Report Count (4)
    0x09, 0x01, // Usage (0x01)
    0x81, 0x00, // Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)

    0x85, 0x30, // Report ID (48)
    0x95, 0x02, // Report Count (2)
    0x09, 0x01, // Usage (0x01)
    0x81, 0x00, // Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)

    0x85, 0x31, // Report ID (49)
    0x95, 0x05, // Report Count (5)
    0x09, 0x01, // Usage (0x01)
    0x81, 0x00, // Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)

    0x85, 0x32, // Report ID (50)
    0x95, 0x0A, // Report Count (10)
    0x09, 0x01, // Usage (0x01)
    0x81, 0x00, // Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)

    0x85, 0x33, // Report ID (51)
    0x95, 0x11, // Report Count (17)
    0x09, 0x01, // Usage (0x01)
    0x81, 0x00, // Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)

    0x85, 0x34, // Report ID (52)
    0x95, 0x15, // Report Count (21)
    0x09, 0x01, // Usage (0x01)
    0x81, 0x00, // Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)

    0x85, 0x35, // Report ID (53)
    0x95, 0x15, // Report Count (21)
    0x09, 0x01, // Usage (0x01)
    0x81, 0x00, // Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)

    0x85, 0x36, // Report ID (54)
    0x95, 0x15, // Report Count (21)
    0x09, 0x01, // Usage (0x01)
    0x81, 0x00, // Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)

    0x85, 0x37, // Report ID (55)
    0x95, 0x15, // Report Count (21)
    0x09, 0x01, // Usage (0x01)
    0x81, 0x00, // Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)

    0x85, 0x3D, // Report ID (61)
    0x95, 0x15, // Report Count (21)
    0x09, 0x01, // Usage (0x01)
    0x81, 0x00, // Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)

    0x85, 0x3E, // Report ID (62)
    0x95, 0x15, // Report Count (21)
    0x09, 0x01, // Usage (0x01)
    0x81, 0x00, // Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)

    0x85, 0x3F, // Report ID (63)
    0x95, 0x15, // Report Count (21)
    0x09, 0x01, // Usage (0x01)
    0x81, 0x00, // Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)

    0xC0,       // End Collection
};
// clang-format on

void* App_MEMalloc(u32 size) {
    DEBUGPrint("App_MEMalloc\n");
    return _wcb.allocFunc(size);
}

BOOL App_MEMfree(void* pBlock) {
    DEBUGPrint("App_MEMfree\n");
    return _wcb.freeFunc(pBlock);
}





















































BOOL WUDInit(void) {
    WUDCB* p = &_wcb;

    if (_wudInitialized) {
        return FALSE;
    }

    DEBUGPrint("BTA_Init() is started\n");

    BTA_Init();
    bta_sys_set_trace_level(__bte_trace_level);
    L2CA_SetTraceLevel(__bte_trace_level);
    SDP_SetTraceLevel(__bte_trace_level);

    DEBUGPrint("BTA_Init() is done\n");

    p->syncStdCB = NULL;
    p->syncSmpCB = NULL;
    p->clearDevCB = NULL;

    p->hidConnCB = NULL;
    p->hidRecvCB = NULL;

    p->initState = WUD_STATE_INIT_WAIT_FOR_INITIALIZATION;

    SCInit();

    OSCreateAlarm(&p->alarm);
    OSSetPeriodicAlarm(&p->alarm, OSGetTime(), OS_MSEC_TO_TICKS(10),
                       __wudInitHandler0);

    _wudInitialized = TRUE;
    return TRUE;
}

void WUDRegisterAllocator(WUDAllocFunc pAllocFunc, WUDFreeFunc pFreeFunc) {
    WUDCB* p = &_wcb;
    BOOL enabled;

    enabled = OSDisableInterrupts();

    p->allocFunc = pAllocFunc;
    p->freeFunc = pFreeFunc;

    OSRestoreInterrupts(enabled);
}


WUDLibStatus WUDGetStatus(void) {
    WUDCB* p = &_wcb;
    BOOL enabled = OSDisableInterrupts();

    s8 libStatus = p->libStatus;

    OSRestoreInterrupts(enabled);
    return libStatus;
}

u8 WUDGetBufferStatus(void) {
    WUDCB* p = &_wcb;
    u8 status;
    BOOL enabled = OSDisableInterrupts();

    status = p->bufferStatus1 - p->bufferStatus0;

    OSRestoreInterrupts(enabled);
    return status;
}

void WUDSetSniffMode(BD_ADDR addr, s32 interval) {
    WUDCB* p = &_wcb;
    tBTM_PM_PWR_MD block;

    if (interval > 0) {
        block.mode = BTM_PM_MD_SNIFF;
    } else {
        block.mode = BTM_PM_MD_ACTIVE;
    }

    block.max = interval;
    block.min = interval;
    block.attempt = 1;
    block.timeout = 0;

    BTM_SetPowerMode(p->pmID, addr, &block);
}

WUDSyncDeviceCallback
WUDSetSyncSimpleCallback(WUDSyncDeviceCallback pCallback) {

    WUDCB* p = &_wcb;
    BOOL enabled;
    WUDSyncDeviceCallback pOldCallback;

    DEBUGPrint("WUDSetSyncDeviceCallback\n");

    enabled = OSDisableInterrupts();

    pOldCallback = p->syncSmpCB;
    p->syncSmpCB = pCallback;

    OSRestoreInterrupts(enabled);
    return pOldCallback;
}

WUDClearDeviceCallback
WUDSetClearDeviceCallback(WUDClearDeviceCallback pCallback) {

    WUDCB* p = &_wcb;
    BOOL enabled;
    WUDClearDeviceCallback pOldCallback;

    DEBUGPrint("WUDSetClearDeviceCallback\n");

    enabled = OSDisableInterrupts();

    pOldCallback = p->clearDevCB;
    p->clearDevCB = pCallback;

    OSRestoreInterrupts(enabled);
    return pOldCallback;
}





BOOL WUDStartFastSyncSimple(void) {
    return __wudStartSyncDevice(WUD_SYNC_TYPE_SIMPLE, -1, 0, TRUE);
}



DECOMP_FORCEACTIVE(WUD_c,
                   "WUDCancelSyncDevice()\n");

BOOL WUDStopSyncSimple(void) {
    BOOL enabled;
    BOOL success = FALSE;
    WUDCB* p = &_wcb;

    enabled = OSDisableInterrupts();

    if (p->libStatus == WUD_LIB_STATUS_3) {
        if (p->syncLoopNum != 0) {
            if (p->syncState == WUD_STATE_SYNC_WAIT_FOR_SEARCH_RESULT) {
                BTA_DmSearchCancel();
            }

            p->syncLoopNum = 0;
        }

        success = TRUE;
    }

    OSRestoreInterrupts(enabled);
    return success;
}


BOOL WUDSetDisableChannel(s8 afhChannel) {
    extern char lbl_80562AE0[];
    extern char lbl_80562AF8[];
    int first = 0;
    int last = 0;
    BOOL success = FALSE;
    WUDCB* p = &_wcb;
    BOOL enabled;
    u32 libStatus;
    tBTM_STATUS status;

    DEBUGPrint(lbl_80562AE0);

    if ((u8)afhChannel > 13) {
        return FALSE;
    }

    enabled = OSDisableInterrupts();
    libStatus = p->libStatus;
    OSRestoreInterrupts(enabled);

    if (libStatus == WUD_LIB_STATUS_3) {
        if (!afhChannel) {
            first = 0xFF;
            last = 0xFF;
        } else {
            first = (afhChannel + 1) * 5 - 14;
            last = (afhChannel + 1) * 5 + 14;

            if (first < 0) {
                first = 0;
            }

            if (last > 78) {
                last = 78;
            }
        }

        status = BTM_SetAfhChannels(first, last);
        DEBUGPrint(lbl_80562AF8, status);

        success = TRUE;
    }

    return success;
}

WUDHidRecvCallback WUDSetHidRecvCallback(WUDHidRecvCallback pCallback) {
    WUDCB* p = &_wcb;
    BOOL enabled;
    WUDHidRecvCallback pOldCallback;

    DEBUGPrint("WUDSetHidRecvCallback()\n");

    enabled = OSDisableInterrupts();

    pOldCallback = p->hidRecvCB;
    p->hidRecvCB = pCallback;

    OSRestoreInterrupts(enabled);
    return pOldCallback;
}

WUDHidConnCallback WUDSetHidConnCallback(WUDHidConnCallback pCallback) {
    WUDCB* p = &_wcb;
    BOOL enabled;
    WUDHidConnCallback pOldCallback;

    DEBUGPrint("WUDSetHidConnCallback()\n");

    enabled = OSDisableInterrupts();

    pOldCallback = p->hidConnCB;
    p->hidConnCB = pCallback;

    OSRestoreInterrupts(enabled);
    return pOldCallback;
}

void WUDSetVisibility(u8 disc, u8 conn) {
    WUDCB* p = &_wcb;
    BOOL enabled = OSDisableInterrupts();

    p->discoverable = disc;
    p->connectable = conn;

    OSRestoreInterrupts(enabled);
    BTA_DmSetVisibility(disc, conn);
}




// clang-format off
static u8 patch_binary[] = {
    0x70, 0x99, 0x08, 0x00, // address (little-endian)
    0xB4, 0x00, 0x00, 0x00, // size (little-endian)

    // patch data
    0x88, 0x43, 0xD1, 0x07,
    0x09, 0x0C, 0x08, 0x43,
    0xA0, 0x62, 0x19, 0x23,
    0xDB, 0x01, 0x33, 0x80,
    0x7C, 0xF7, 0x88, 0xF8,
    0x28, 0x76, 0x80, 0xF7,
    0x17, 0xFF, 0x43, 0x78,
    0xEB, 0x70, 0x19, 0x23,
    0xDB, 0x01, 0x33, 0x87,
    0x7C, 0xF7, 0xBC, 0xFB,
    0x0B, 0x60, 0xA3, 0x7B,
    0x01, 0x49, 0x0B, 0x60,
    0x90, 0xF7, 0x96, 0xFB,
    0xD8, 0x1D, 0x08, 0x00,
    0x00, 0xF0, 0x04, 0xF8,
    0x00, 0x23, 0x79, 0xF7,
    0xE3, 0xFA, 0x00, 0x00,
    0x00, 0xB5, 0x00, 0x23,
    0x11, 0x49, 0x0B, 0x60,
    0x1D, 0x21, 0xC9, 0x03,
    0x0B, 0x60, 0x7D, 0x20,
    0x80, 0x01, 0x01, 0x38,
    0xFD, 0xD1, 0x0E, 0x4B,
    0x0E, 0x4A, 0x13, 0x60,
    0x47, 0x20, 0x00, 0x21,
    0x96, 0xF7, 0x96, 0xFF,
    0x46, 0x20, 0x00, 0x21,
    0x96, 0xF7, 0x92, 0xFF,
    0x0A, 0x4A, 0x13, 0x68,
    0x0A, 0x48, 0x03, 0x40,
    0x13, 0x60, 0x0A, 0x4A,
    0x13, 0x68, 0x0A, 0x48,
    0x03, 0x40, 0x13, 0x60,
    0x09, 0x4A, 0x13, 0x68,
    0x09, 0x48, 0x03, 0x40,
    0x13, 0x60, 0x00, 0xBD,
    0x24, 0x80, 0x0E, 0x00,
    0x81, 0x03, 0x0F, 0xFE,
    0x5C, 0x00, 0x0F, 0x00,
    0x60, 0xFC, 0x0E, 0x00,
    0xFE, 0xFF, 0x00, 0x00,
    0xFC, 0xFC, 0x0E, 0x00,
    0xFF, 0x9F, 0x00, 0x00,
    0x30, 0xFC, 0x0E, 0x00,
    0x7F, 0xFF, 0x00, 0x00
};
// clang-format on

// clang-format off
static WUDPatchList patch_install = {
    7,
    {
        {0x20, 0xBC, 0x65, 0x01, 0x00, 0x84, 0x42, 0x09, 0xD2, 0x84, 0x42, 0x09, 0xD1},
        {0x21, 0x84, 0x5A, 0x00, 0x00, 0x83, 0xF0, 0x74, 0xFF, 0x09, 0x0C, 0x08, 0x43},
        {0x22, 0x00, 0x61, 0x00, 0x00, 0x83, 0xF0, 0x40, 0xFC, 0x00, 0x00, 0x00, 0x00},
        {0x23, 0xCC, 0x9F, 0x01, 0x00, 0x6F, 0xF0, 0xE4, 0xFC, 0x03, 0x28, 0x7D, 0xD1},
        {0x24, 0x3C, 0x62, 0x01, 0x00, 0x28, 0x20, 0x00, 0xE0, 0x60, 0x8D, 0x23, 0x68},
        {0x25, 0x04, 0x12, 0x01, 0x00, 0x20, 0x1C, 0x20, 0x1C, 0x24, 0xE0, 0xB0, 0x21},
        {0x26, 0x74, 0x2F, 0x00, 0x00, 0x86, 0xF0, 0x18, 0xFD, 0x21, 0x4F, 0x3B, 0x60}
    }
};
// clang-format on

static u8 remove_patch[] = {0x00};

static u8 install_num = 0;
static u8 patch_num = 0;

static u32 addr = 0;
static s32 offset = 0;
static u32 size = 0;












DECOMP_FORCEACTIVE(WUD_c_1,
                   "WUDiCancelSync()\n");


void WUDiRegisterDevice(BD_ADDR addr) {
    char* pMsg = _wudWiiRemoteDescriptor;
    WUDCB* p = &_wcb;
    WUDDevInfo* pInfo;
    tBTA_STATUS status;
    BOOL enabled;

    enabled = OSDisableInterrupts();
    pInfo = WUDiGetDevInfo(addr);

    status = BTA_DmAddDevice(pInfo->devAddr, pInfo->linkKey, 0, FALSE);
    DEBUGPrint(pMsg + 0x97C, status);

    if (memcmp(pInfo->conf.devName, pMsg + 0x268, sizeof(LINK_KEY)) == 0 ||
        (memcmp(pInfo->conf.devName, pMsg + 0x27C, sizeof(LINK_KEY)) == 0 &&
         _linkedWBC)) {
        tBTA_HH_DEV_DESCR desc;
        desc.dsc_list = (u8*)pMsg;
        desc.dl_len = sizeof(descriptor);

        DEBUGPrint(pMsg + 0x998);
        BTA_HhAddDev(pInfo->devAddr, pInfo->hhAttrMask, pInfo->subclass,
                     pInfo->appID, desc);
    }

    if (pInfo->UNK_0x5B == 0 || pInfo->UNK_0x5B == 4 || pInfo->UNK_0x5B == 2 ||
        pInfo->UNK_0x5B == 5) {

        p->devNums++;
    } else {
        p->devSmpNums++;
    }

    OSRestoreInterrupts(enabled);
}

void WUDiRemoveDevice(BD_ADDR addr) {
    char* pMsg = _wudWiiRemoteDescriptor;
    WUDCB* p = &_wcb;
    WUDDevInfo* pInfo;
    tBTM_STATUS status;
    BOOL enabled = OSDisableInterrupts();

    DEBUGPrint(pMsg + 0x9A8);

    pInfo = WUDiGetDevInfo(addr);

    if (pInfo != NULL) {
        DEBUGPrint(pMsg + 0x9C0,
                   pInfo->devHandle, pInfo->devAddr[0], pInfo->devAddr[1],
                   pInfo->devAddr[2], pInfo->devAddr[3], pInfo->devAddr[4],
                   pInfo->devAddr[5]);

        DEBUGPrint(pMsg + 0x9F8);

        if (memcmp(pInfo->conf.devName, pMsg + 0x268, sizeof(LINK_KEY)) == 0 ||
            (memcmp(pInfo->conf.devName, pMsg + 0x27C, sizeof(LINK_KEY)) == 0 &&
             _linkedWBC)) {
            DEBUGPrint(pMsg + 0xA1C);
            DEBUGPrint(pMsg + 0xA30, pInfo->devHandle);
            BTA_HhRemoveDev(pInfo->devHandle);
        }

        status = BTA_DmRemoveDevice(pInfo->devAddr);
        DEBUGPrint(pMsg + 0xA40, status);

        if (pInfo->UNK_0x5B == 0 || pInfo->UNK_0x5B == 2 ||
            pInfo->UNK_0x5B == 4 || pInfo->UNK_0x5B == 5) {
            p->devNums--;
        } else {
            p->devSmpNums--;
        }

        memset(pInfo, 0, sizeof *pInfo);
    }

    OSRestoreInterrupts(enabled);
}

WUDDevInfo* WUDiGetDevInfo(BD_ADDR addr) {
    WUDCB* p = &_wcb;
    WUDDevInfo* pInfo;
    BOOL enabled;
    int i;

    pInfo = NULL;
    enabled = OSDisableInterrupts();

    for (i = 0; i < WUD_MAX_DEV_ENTRY_FOR_STD; i++) {
        if (WUD_BDCMP(p->stdDevs[i].devAddr, addr) == 0) {
            pInfo = &p->stdDevs[i];
            break;
        }
    }

    if (pInfo == NULL) {
        for (i = 0; i < WUD_MAX_DEV_ENTRY_FOR_SMP; i++) {
            if (WUD_BDCMP(p->smpDevs[i].devAddr, addr) == 0) {
                pInfo = &p->smpDevs[i];
                break;
            }
        }
    }

    OSRestoreInterrupts(enabled);
    return pInfo;
}







static u8 WUDiGetConnNumber(void) {
    WUDCB* p = &_wcb;
    BOOL enabled = OSDisableInterrupts();

    u8 num = p->connectedNum;

    OSRestoreInterrupts(enabled);
    return num;
}

static u8 WUDiGetLinkNum(void) {
    WUDCB* p = &_wcb;
    BOOL enabled = OSDisableInterrupts();

    u8 num = p->linkedNum;

    OSRestoreInterrupts(enabled);
    return num;
}

void WUDiMoveTopSmpDevInfoPtr(WUDDevInfo* pInfo) {
    WUDCB* p = &_wcb;
    BOOL enabled;
    int i;

    enabled = OSDisableInterrupts();

    for (i = 0; i < WUD_MAX_DEV_ENTRY_FOR_SMP; i++) {
        if (WUD_BDCMP(p->smpList[i].devInfo->devAddr, pInfo->devAddr) != 0) {
            continue;
        }

        if (WUD_BDCMP(p->smpListHead->devInfo->devAddr,
                      p->smpList[i].devInfo->devAddr) == 0) {
            break;
        }

        p->smpList[i].prev->next = p->smpList[i].next;

        if (WUD_BDCMP(p->smpListTail->devInfo->devAddr,
                      p->smpList[i].devInfo->devAddr) == 0) {
            p->smpListTail = p->smpList[i].prev;
        } else {
            p->smpList[i].next->prev = p->smpList[i].prev;
        }

        p->smpList[i].next = p->smpListHead;
        p->smpListHead->prev = &p->smpList[i];
        p->smpListHead = &p->smpList[i];

        p->smpList[i].prev = NULL;

        break;
    }

    OSRestoreInterrupts(enabled);
}

void WUDiMoveBottomSmpDevInfoPtr(WUDDevInfo* pInfo) {
    WUDCB* p = &_wcb;
    BOOL enabled;
    int i;

    enabled = OSDisableInterrupts();

    for (i = 0; i < WUD_MAX_DEV_ENTRY_FOR_SMP; i++) {
        if (WUD_BDCMP(p->smpList[i].devInfo->devAddr, pInfo->devAddr) != 0) {
            continue;
        }

        if (WUD_BDCMP(p->smpListTail->devInfo->devAddr,
                      p->smpList[i].devInfo->devAddr) == 0) {
            break;
        }

        p->smpList[i].next->prev = p->smpList[i].prev;

        if (WUD_BDCMP(p->smpListHead->devInfo->devAddr,
                      p->smpList[i].devInfo->devAddr) == 0) {
            p->smpListHead = p->smpList[i].next;
        } else {
            p->smpList[i].prev->next = p->smpList[i].next;
        }

        p->smpList[i].prev = p->smpListTail;
        p->smpListTail->next = &p->smpList[i];
        p->smpListTail = &p->smpList[i];

        p->smpList[i].next = NULL;

        break;
    }

    OSRestoreInterrupts(enabled);
}


void WUDiMoveTopOfDisconnectedSmpDevice(WUDDevInfo* pInfo) {
    WUDCB* p = &_wcb;
    int i;
    BOOL enabled;
    WUDDevInfoList* pIt;

#define WUD_SMP_DEV(p, i) (*(WUDDevInfo**)((u8*)(p) + (u32)(i) * 12 + 0x1C))
#define WUD_SMP_PREV(p, i) (*(WUDDevInfoList**)((u8*)(p) + ((u32)(i) * 12 + 0x20)))
#define WUD_SMP_NEXT(p, i) (*(WUDDevInfoList**)((u8*)(p) + ((u32)(i) * 12 + 0x24)))
#define WUD_SMP_LIST(p, i) ((WUDDevInfoList*)((u8*)(p) + (u32)(i) * 12 + 0x1C))

    enabled = OSDisableInterrupts();

    for (i = 0; i < WUD_MAX_DEV_ENTRY_FOR_SMP; i++) {

        if (WUD_BDCMP((*(WUDDevInfo**)((u8*)p + (u32)i * 12 + 0x1C))->devAddr,
                      pInfo->devAddr) != 0) {
            continue;
        }

        for (pIt = p->smpListHead; pIt; pIt = pIt->next) {
            if (WUD_BDCMP(pIt->devInfo->devAddr, pInfo->devAddr) == 0) {
                continue;
            }

            if (pIt->devInfo->status > 1) {
                continue;
            }

            if (WUD_BDCMP(p->smpListHead->devInfo->devAddr,
                          (*(WUDDevInfo**)((u8*)p + ((u32)i * 12 + 0x1C)))
                              ->devAddr) == 0) {
                if (pIt == p->smpListHead->next) {
                    break;
                }

                p->smpListHead = WUD_SMP_NEXT(p, i);
            } else {
                WUD_SMP_PREV(p, i)->next = WUD_SMP_NEXT(p, i);
            }

            WUD_SMP_NEXT(p, i)->prev = WUD_SMP_PREV(p, i);

            if (pIt != p->smpListHead) {
                WUD_SMP_PREV(p, i) = pIt->prev;
                WUD_SMP_NEXT(p, i) = pIt;

                pIt->prev->next = WUD_SMP_LIST(p, i);
                pIt->prev = WUD_SMP_LIST(p, i);
            } else {
                WUD_SMP_PREV(p, i) = pIt;
                WUD_SMP_NEXT(p, i) = pIt->next;

                pIt->next->prev = WUD_SMP_LIST(p, i);
                pIt->next = WUD_SMP_LIST(p, i);
            }

            break;
        }
    }

#undef WUD_SMP_DEV
#undef WUD_SMP_PREV
#undef WUD_SMP_NEXT
#undef WUD_SMP_LIST


    OSRestoreInterrupts(enabled);
}

void WUDiMoveTopStdDevInfoPtr(WUDDevInfo* pInfo) {
    WUDCB* p = &_wcb;
    BOOL enabled;
    int i;

    enabled = OSDisableInterrupts();

    for (i = 0; i < WUD_MAX_DEV_ENTRY_FOR_STD; i++) {
        if (WUD_BDCMP(p->stdList[i].devInfo->devAddr, pInfo->devAddr) != 0) {
            continue;
        }

        if (WUD_BDCMP(p->stdListHead->devInfo->devAddr,
                      p->stdList[i].devInfo->devAddr) == 0) {
            break;
        }

        p->stdList[i].prev->next = p->stdList[i].next;

        if (WUD_BDCMP(p->stdListTail->devInfo->devAddr,
                      p->stdList[i].devInfo->devAddr) == 0) {
            p->stdListTail = p->stdList[i].prev;
        } else {
            p->stdList[i].next->prev = p->stdList[i].prev;
        }

        p->stdList[i].next = p->stdListHead;
        p->stdListHead->prev = &p->stdList[i];
        p->stdListHead = &p->stdList[i];

        p->stdList[i].prev = NULL;

        break;
    }

    OSRestoreInterrupts(enabled);
}

void WUDiMoveBottomStdDevInfoPtr(WUDDevInfo* pInfo) {
    WUDCB* p = &_wcb;
    BOOL enabled;
    int i;

    enabled = OSDisableInterrupts();

    for (i = 0; i < WUD_MAX_DEV_ENTRY_FOR_STD; i++) {
        if (WUD_BDCMP(p->stdList[i].devInfo->devAddr, pInfo->devAddr) != 0) {
            continue;
        }

        if (WUD_BDCMP(p->stdListTail->devInfo->devAddr,
                      p->stdList[i].devInfo->devAddr) == 0) {
            break;
        }

        p->stdList[i].next->prev = p->stdList[i].prev;

        if (WUD_BDCMP(p->stdListHead->devInfo->devAddr,
                      p->stdList[i].devInfo->devAddr) == 0) {
            p->stdListHead = p->stdList[i].next;
        } else {
            p->stdList[i].prev->next = p->stdList[i].next;
        }

        p->stdList[i].prev = p->stdListTail;
        p->stdListTail->next = &p->stdList[i];
        p->stdListTail = &p->stdList[i];

        p->stdList[i].next = NULL;

        break;
    }

    OSRestoreInterrupts(enabled);
}


void WUDiMoveTopOfDisconnectedStdDevice(WUDDevInfo* pInfo) {
    WUDCB* p = &_wcb;
    int i;
    BOOL enabled;
    WUDDevInfoList* pIt;

    enabled = OSDisableInterrupts();

    for (i = 0; i < WUD_MAX_DEV_ENTRY_FOR_STD; i++) {
        if (WUD_BDCMP(p->stdList[i].devInfo->devAddr, pInfo->devAddr) != 0) {
            continue;
        }

        for (pIt = p->stdListHead; pIt; pIt = pIt->next) {
            if (WUD_BDCMP(pIt->devInfo->devAddr, pInfo->devAddr) == 0) {
                continue;
            }

            if (pIt->devInfo->status > 1) {
                continue;
            }

            if (WUD_BDCMP(p->stdListHead->devInfo->devAddr,
                          p->stdList[i].devInfo->devAddr) == 0) {
                if (pIt == p->stdListHead->next)
                    break;

                p->stdListHead = p->stdList[i].next;
            } else {
                p->stdList[i].prev->next = p->stdList[i].next;
            }

            p->stdList[i].next->prev = p->stdList[i].prev;

            if (pIt != p->stdListHead) {
                p->stdList[i].prev = pIt->prev;
                p->stdList[i].next = pIt;

                pIt->prev->next = &p->stdList[i];
                pIt->prev = &p->stdList[i];
            } else {
                p->stdList[i].prev = pIt;
                p->stdList[i].next = pIt->next;

                pIt->next->prev = &p->stdList[i];
                pIt->next = &p->stdList[i];
            }

            break;
        }
    }

    OSRestoreInterrupts(enabled);
}

BOOL WUDIsBusy(void) {
    WUDCB* p = &_wcb;
    BOOL enabled;

    enabled = OSDisableInterrupts();

    if (p->syncState == WUD_STATE_SYNC_START &&
        p->deleteState == WUD_STATE_DELETE_START &&
        p->stackState == WUD_STATE_STACK_INITIALIZED &&
        p->initState == 6) {

        OSRestoreInterrupts(enabled);
        return FALSE;
    }

    OSRestoreInterrupts(enabled);
    return TRUE;
}

u8 _WUDGetLinkNumber(void) {
    WUDCB* p = &_wcb;
    BOOL enabled = OSDisableInterrupts();

    u8 num = p->linkedNum;

    OSRestoreInterrupts(enabled);
    return num;
}

DECOMP_FORCEACTIVE(WUD_c_2,
                   "_WUDEnableTestMode\n",
                   "_WUDStartSyncDevice()\n",
                   "_WUDDeleteStoreDevice()\n",
                   "dev number = %d\n");

int WUDIsLinkedWBC() { extern int _linkedWBC; return _linkedWBC; }

void __wudSyncFlushCallback(SCStatus status) {
    extern char lbl_80562504[];
    WUDCB* p = &_wcb;

    DEBUGPrint(lbl_80562504, status, p->syncState);

    if (p->syncState != 0) {
        if (status == 0) {
            p->syncState = WUD_STATE_SYNC_COMPLETE;
        } else {
            p->syncState = WUD_STATE_SYNC_ERROR;
        }
    }
}

u8 __wudSyncPrepareSearch(void) {
    WUDCB* p = &_wcb;
    WUDDevInfo* pDev;
    BOOL enabled;
    u8 num;
    int i;

    enabled = OSDisableInterrupts();
    p->discoverable = 0;
    p->connectable = 0;
    OSRestoreInterrupts(enabled);

    BTA_DmSetVisibility(0, 0);

    if (p->syncLoopNum == 0) {
        return WUD_STATE_SYNC_DONE;
    }

    enabled = OSDisableInterrupts();
    num = p->linkedNum;
    OSRestoreInterrupts(enabled);

    if (num == 4) {
        enabled = OSDisableInterrupts();
        num = p->connectedNum;
        OSRestoreInterrupts(enabled);

        if (num == 4) {
            return WUD_STATE_SYNC_DONE;
        }
    }

    if (p->syncSkipChecks != 0) {
        for (i = 0; i < WUD_MAX_DEV_ENTRY; i++) {
            tBTM_PM_PWR_MD block;

            enabled = OSDisableInterrupts();
            pDev = ((u32)i <= 9)
                       ? &_wcb.stdDevs[i]
                       : &_wcb.smpDevs[i - WUD_MAX_DEV_ENTRY_FOR_STD];
            OSRestoreInterrupts(enabled);

            if (pDev->status == 9) {
                block.mode = BTM_PM_MD_ACTIVE;
                block.max = 0;
                block.min = 0;
                block.attempt = 1;
                block.timeout = 0;

                BTM_SetPowerMode(_wcb.pmID, pDev->devAddr, &block);
                return 1;
            }
        }
    }

    if (p->syncLoopNum > 0) {
        p->syncLoopNum--;
    }

    p->UNK_0x748 = 0x32;
    return WUD_STATE_SYNC_WAIT_FOR_START_SEARCH;
}

u8 __wudSyncTryConnect(void) {
    WUDCB* p = &_wcb;
    WUDDevInfoList* pIt;
    WUDDevInfo* pFound;
    BOOL enabled;
    u8 ret = WUD_STATE_SYNC_ERROR;

    if (memcmp(_wudDiscResp.devName, _wudWiiRemoteDescriptor + 0x268,
               sizeof(LINK_KEY)) == 0) {
        WUDDevInfo* pWork = &_wudDiscWork;

        pWork->status = 2;
        BTA_HhOpen(pWork->devAddr, BTA_HH_PROTO_RPT_MODE,
                   BTA_SEC_AUTHENTICATE);
        ret = WUD_STATE_SYNC_6;
    }

    if (_linkedWBC != 0) {
        if (memcmp(_wudDiscResp.devName, _wudWiiRemoteDescriptor + 0x27C,
                   sizeof(LINK_KEY)) == 0) {
            pFound = NULL;
            enabled = OSDisableInterrupts();

            for (pIt = p->stdListHead; pIt != NULL; pIt = pIt->next) {
                if (memcmp(pIt->devInfo, _wudWiiRemoteDescriptor + 0x27C,
                           sizeof(LINK_KEY)) == 0) {
                    pFound = pIt->devInfo;
                }
            }

            OSRestoreInterrupts(enabled);

            if (pFound != NULL) {
                DEBUGPrint(_wudWiiRemoteDescriptor + 0x290);

                if (pFound->status > 1) {
                    return ret;
                }

                if (memcmp(_wudDiscResp.devAddr, pFound->devAddr,
                           BD_ADDR_LEN) != 0) {
                    DEBUGPrint(_wudWiiRemoteDescriptor + 0x2B8);
                    WUDiMoveBottomStdDevInfoPtr(pFound);
                    WUDiRemoveDevice(pFound->devAddr);
                }
            }

            {
                WUDDevInfo* pWork = &_wudDiscWork;

                pWork->status = 2;
                BTA_HhOpen(pWork->devAddr, BTA_HH_PROTO_RPT_MODE,
                           BTA_SEC_AUTHENTICATE);
            }
            ret = WUD_STATE_SYNC_6;
        }
    }

    return ret;
}
void WUDShutdown(void) {
    WUDCB* p = &_wcb;
    BOOL enabled;
    int i;
    WUDDevInfoList* pIt;

    DEBUGPrint("WUDShutdown()\n");

    WUDSetVisibility(FALSE, FALSE);

    enabled = OSDisableInterrupts();

    if (WUDIsBusy()) {
        OSCancelAlarm(&p->alarm);
    }

    memset(_scArray.devices, 0,
           sizeof(SCBtDeviceInfo) * WUD_MAX_DEV_ENTRY_FOR_STD);

    for (i = 0, pIt = _wcb.stdListHead; pIt != NULL; pIt = pIt->next, i++) {
        WUD_BDCPY(_scArray.devices[i].addr, pIt->devInfo->devAddr);

        memcpy(&_scArray.devices[i].info, &pIt->devInfo->conf,
               sizeof(SCDevInfo));
    }

    p->shutdownState = WUD_STATE_SHUTDOWN_STORE_SETTINGS;

    OSCreateAlarm(&p->alarm);
    OSSetPeriodicAlarm(&p->alarm, OSGetTime(), OS_MSEC_TO_TICKS(10),
                       __wudShutdownHandler0);

    p->libStatus = WUD_LIB_STATUS_4;

    OSRestoreInterrupts(enabled);
}

u8 __wudSyncVirginStandard(void) {
    WUDCB* p = &_wcb;
    char* pMsg = _wudWiiRemoteDescriptor;
    WUDDevInfo* pWork = NULL;
    WUDDevInfo* pFound;
    WUDDevInfoList* pIt;
    BOOL enabled;
    u8 devNum;
    u32 crc;
    int i;

    if (_linkedWBC != 0) {
        if (memcmp(&_wudDiscWork, pMsg + 0x268, sizeof(LINK_KEY)) == 0) {
            pFound = NULL;
            enabled = OSDisableInterrupts();

            for (pIt = p->stdListHead; pIt != NULL; pIt = pIt->next) {
                if (memcmp(pIt->devInfo, pMsg + 0x27C,
                           sizeof(LINK_KEY)) == 0) {
                    pFound = pIt->devInfo;
                }
            }

            OSRestoreInterrupts(enabled);

            if (pFound != NULL) {
                enabled = OSDisableInterrupts();
                devNum = p->devNums;
                OSRestoreInterrupts(enabled);

                if (devNum == WUD_MAX_DEV_ENTRY_FOR_STD) {
                    WUDiMoveBottomStdDevInfoPtr(
                        p->stdListTail->prev->devInfo);
                    WUDiRemoveDevice(p->stdListTail->prev->devInfo->devAddr);
                }
            }
        }
    }

    enabled = OSDisableInterrupts();
    devNum = p->devNums;
    OSRestoreInterrupts(enabled);

    if (devNum == WUD_MAX_DEV_ENTRY_FOR_STD) {
        WUDiRemoveDevice(p->stdListTail->devInfo->devAddr);
    }

    enabled = OSDisableInterrupts();

    if (p->syncType == WUD_SYNC_TYPE_STANDARD) {
        for (i = 0; i < WUD_MAX_DEV_ENTRY_FOR_STD; i++) {
            if (p->stdDevs[i].status == 0) {
                DEBUGPrint(pMsg + 0x2CC, i);
                pWork = &p->stdDevs[i];
                break;
            }
        }
    } else {
        pWork = p->smpListTail->devInfo;
    }

    OSRestoreInterrupts(enabled);

    if (pWork == NULL) {
        return 0xFF;
    }

    if (pWork->status != 0) {
        return 0xFF;
    }

    memcpy(pWork, &_wudDiscWork, sizeof(WUDDevInfo));
    WUDiRegisterDevice(pWork->devAddr);
    WUDiMoveTopStdDevInfoPtr(pWork);

    if (_linkedWBC != 0) {
        if (memcmp(pWork, pMsg + 0x27C, sizeof(LINK_KEY)) == 0) {
            WUDiMoveTopOfUnusedStdDevice(pWork);

            memset(&_scArray.active[4].addr, 0, sizeof(SCBtDeviceInfo));
            memcpy(&_scArray.active[4].addr, pWork->devAddr,
                   sizeof(BD_ADDR));
            memcpy(&_scArray.active[4].info, pWork, sizeof(SCDevInfo));
            memcpy(&_scArray.active[4].info.at_0x14, pWork->linkKey,
                   sizeof(LINK_KEY));

            if (pWork->devAddr != NULL) {
                memcpy(&_wudNandWbcInfo[0], pWork->devAddr,
                       sizeof(BD_ADDR));
            }

            if (pWork->devAddr + 6 != NULL) {
                memcpy(&_wudNandWbcInfo[6], pWork->linkKey,
                       sizeof(LINK_KEY));
            }

            if (pWork != NULL) {
                memcpy(&_wudNandWbcInfo[0x16], pWork, sizeof(SCDevInfo));
            }

            {
                const u16* pData = (const u16*)&_wudNandWbcInfo[0];
                u32 sum = _wudNandWbcCrc >> 16;
                u16 invSum = _wudNandWbcCrc & 0xFFFF;
                int j;

                for (j = 0; j < 8; j++) {
                    sum += pData[0];
                    invSum = (u16)(invSum + ~pData[0]);
                    sum += pData[1];
                    invSum = (u16)(invSum + ~pData[1]);
                    sum += pData[2];
                    invSum = (u16)(invSum + ~pData[2]);
                    sum += pData[3];
                    invSum = (u16)(invSum + ~pData[3]);
                    sum += pData[4];
                    invSum = (u16)(invSum + ~pData[4]);
                    sum += pData[5];
                    invSum = (u16)(invSum + ~pData[5]);
                    sum += pData[6];
                    invSum = (u16)(invSum + ~pData[6]);
                    sum += pData[7];
                    invSum = (u16)(invSum + ~pData[7]);
                    pData += 8;
                }

                crc = (sum << 16) | invSum;
            }
            memcpy(&_wudNandWbcInfo[0x80], &crc, sizeof(crc));
        }
    }

    _dev_handle_to_bda[pWork->devHandle] = pWork->devAddr;
    return WUD_STATE_SYNC_STORED_LINK_KEY_TO_EEPROM;
}
u8 __wudSyncStoredDevInfoToNand(void) {
    extern char lbl_805625AC[];
    extern char lbl_80562544[];
    WUDCB* p;
    WUDDevInfoList* pIt;
    BOOL enabled;
    BOOL busy;
    u8 count;
    u8 num;
    u8 result;

    busy = (SCCheckStatus() == SC_STATUS_BUSY);
    if (busy) {
        return WUD_STATE_SYNC_STORED_DEV_INFO_TO_NAND;
    }

    memset(&_scArray.regist, 0, sizeof(_scArray.regist));

    enabled = OSDisableInterrupts();
    p = &_wcb;
    num = p->devNums;
    OSRestoreInterrupts(enabled);

    _scArray.numRegist = num;
    count = 0;

    for (pIt = p->stdListHead; pIt != NULL; pIt = pIt->next) {
        memcpy(&_scArray.regist[count].addr, pIt->devInfo->devAddr,
               sizeof(BD_ADDR));
        memcpy(&_scArray.regist[count].info, pIt->devInfo, sizeof(SCDevInfo));
        count++;
    }

    DEBUGPrint(lbl_805625AC, count);

    if (!SCSetBtDeviceInfoArray(&_scArray)) {
        return WUD_STATE_SYNC_STORED_DEV_INFO_TO_NAND;
    }

    result = WUD_STATE_SYNC_SC_FLUSH;
    if (_linkedWBC != 0 &&
        memcmp(&_wudDiscWork, lbl_80562544, sizeof(LINK_KEY)) == 0 &&
        SCGetProductGameRegion() == 0) {
        result = 0x64;
    }

    return result;
}
void __wudOpenWiiFitCallback(s32 result) {
    extern char lbl_805625D0[];

    if (_wcb.syncState != 0) {
        _wcb.syncState = (result == 0) ? 0x65 : 0x19;
    }

    if (_wcb.deleteState != 0) {
        _wcb.deleteState = (result == 0) ? 0x65 : 0x5;
    }

    _wudNandLocked = 0;
    DEBUGPrint(lbl_805625D0, result);
}

void __wudSeekWiiFitCallback(s32 result) {
    extern char lbl_805625E0[];

    if (_wcb.syncState != 0) {
        _wcb.syncState = ((u32)(result - 0x40000) == 0xAF18) ? 0x66 : 0x19;
    }

    if (_wcb.deleteState != 0) {
        _wcb.deleteState = ((u32)(result - 0x40000) == 0xAF18) ? 0x66 : 0x5;
    }

    _wudNandLocked = 0;
    DEBUGPrint(lbl_805625E0, result);
}

void __wudUpdateWiiFitCallback(s32 result) {
    extern char lbl_805625F0[];

    if (_wcb.syncState != 0) {
        _wcb.syncState = (result == 0x84) ? 0x67 : 0x19;
    }

    if (_wcb.deleteState != 0) {
        _wcb.deleteState = (result == 0x84) ? 0x67 : 0x5;
    }

    _wudNandLocked = 0;
    DEBUGPrint(lbl_805625F0, result);
}

void __wudCloseWiiFitCallback(s32 result) {
    extern char lbl_80562604[];

    if (_wcb.syncState != 0) {
        _wcb.syncState = 0x19;
    }

    if (_wcb.deleteState != 0) {
        _wcb.deleteState = 0x5;
    }

    _wudNandLocked = 0;
    DEBUGPrint(lbl_80562604, result);
}
u8 __wudSyncDone(void) {
    extern char lbl_80562618[];
    WUDCB* p = &_wcb;
    WUDDevInfo* pDev;
    WUDSyncDeviceCallback cb;
    BOOL enabled;
    int i;

    if (p->syncSkipChecks != 0) {
        for (i = 0; i < 16; i++) {
            tBTM_PM_PWR_MD block;

            enabled = OSDisableInterrupts();
            pDev = ((u32)i <= 9)
                       ? &_wcb.stdDevs[i]
                       : &_wcb.smpDevs[i - WUD_MAX_DEV_ENTRY_FOR_STD];
            OSRestoreInterrupts(enabled);

            if (pDev->status == 8) {
                block.mode = BTM_PM_MD_SNIFF;
                block.max = 8;
                block.min = 8;
                block.attempt = 1;
                block.timeout = 0;

                BTM_SetPowerMode(_wcb.pmID, pDev->devAddr, &block);
                return WUD_STATE_SYNC_DONE;
            }
        }
    }

    OSCancelAlarm(&p->alarm);

    if (_wudAbortSync == 0) {
        enabled = OSDisableInterrupts();
        _wcb.discoverable = FALSE;
        _wcb.connectable = TRUE;
        OSRestoreInterrupts(enabled);

        BTA_DmSetVisibility(0, 1);
    }

    cb = p->syncType == WUD_SYNC_TYPE_STANDARD ? p->syncStdCB : p->syncSmpCB;

    if (cb != NULL) {
        cb(1, p->syncedNum);
    }

    DEBUGPrint(lbl_80562618);
    return WUD_STATE_SYNC_START;
}
void __wudSyncHandler(void) {
    WUDCB* p = &_wcb;
    char* pMsg = _wudWiiRemoteDescriptor;
    BOOL enabled;

    switch (p->syncState) {
    case WUD_STATE_SYNC_PREPARE_SEARCH: {
        p->syncState = __wudSyncPrepareSearch();
        break;
    }

    case WUD_STATE_SYNC_WAIT_FOR_START_SEARCH: {
        u8 linkNum;
        u8 connNum;

        if (p->syncLoopNum == 0) {
            p->syncState = WUD_STATE_SYNC_DONE;
            break;
        }

        enabled = OSDisableInterrupts();
        linkNum = p->linkedNum;
        OSRestoreInterrupts(enabled);

        if (linkNum == WUD_MAX_CHANNELS) {
            enabled = OSDisableInterrupts();
            connNum = p->connectedNum;
            OSRestoreInterrupts(enabled);

            if (connNum == WUD_MAX_CHANNELS) {
                p->syncState = WUD_STATE_SYNC_DONE;
                break;
            }
        }

        p->UNK_0x748--;

        if (p->UNK_0x748 < 0) {
            p->syncState = WUD_STATE_SYNC_START_SEARCH;
        } else {
            p->syncState = WUD_STATE_SYNC_WAIT_FOR_START_SEARCH;
        }
        break;
    }

    case WUD_STATE_SYNC_START_SEARCH: {
        tBTA_DM_INQ dm_inq;

        dm_inq.mode = BTM_LIMITED_INQUIRY;
        dm_inq.max_resps = 1;
        dm_inq.report_dup = FALSE;

        if (p->syncSkipChecks) {
            dm_inq.duration = 5 - 2;
        } else if (p->syncType == WUD_SYNC_TYPE_STANDARD) {
            u8 linkNum;

            enabled = OSDisableInterrupts();
            linkNum = p->linkedNum;
            OSRestoreInterrupts(enabled);

            dm_inq.duration =
                (linkNum == WUD_MAX_CHANNELS - 1 ? 10 : 5) - p->syncLoopNum;
        } else {
            u8 linkNum;

            enabled = OSDisableInterrupts();
            linkNum = p->linkedNum;
            OSRestoreInterrupts(enabled);

            dm_inq.duration =
                linkNum == WUD_MAX_CHANNELS - 1 ? 10 - 2 : 5 - 2;
        }

        _wudDiscNumResps = 0;
        memset(&_wudDiscResp, 0, sizeof(WUDDiscResp));
        BTA_DmSearch(&dm_inq, 0, __wudSearchEventStackCallback);
        p->syncState = WUD_STATE_SYNC_WAIT_FOR_SEARCH_RESULT;
        break;
    }

    case WUD_STATE_SYNC_WAIT_FOR_SEARCH_RESULT: {
        u8 linkNum;
        u8 connNum;

        enabled = OSDisableInterrupts();
        linkNum = p->linkedNum;
        OSRestoreInterrupts(enabled);

        if (linkNum == WUD_MAX_CHANNELS) {
            enabled = OSDisableInterrupts();
            connNum = p->connectedNum;
            OSRestoreInterrupts(enabled);

            if (connNum == WUD_MAX_CHANNELS) {
                DEBUGPrint(pMsg + 0x3F8);
                BTA_DmSearchCancel();
                p->syncState = WUD_STATE_SYNC_CANCEL_SEARCH;
                break;
            }
        }

        p->syncState = WUD_STATE_SYNC_WAIT_FOR_SEARCH_RESULT;
        break;
    }

    case WUD_STATE_SYNC_CHECK_SEARCH_RESULT: {
        u8 nextState = WUD_STATE_SYNC_PREPARE_SEARCH;
        u8 linkNum;

        if (_wudDiscNumResps > 0) {
            if (memcmp(&_wudDiscResp.devName, pMsg + 0x268,
                       sizeof(LINK_KEY)) == 0) {
                nextState = WUD_STATE_SYNC_IS_EXISTED_DEVICE;
            }

            if (_linkedWBC != 0 &&
                memcmp(&_wudDiscResp.devName, pMsg + 0x27C,
                       sizeof(LINK_KEY)) == 0) {
                nextState = WUD_STATE_SYNC_IS_EXISTED_DEVICE;
            }

            if (_wudDiscRssi < p->syncRssi) {
                nextState = WUD_STATE_SYNC_PREPARE_SEARCH;
            }
        }

        if (p->syncType == WUD_SYNC_TYPE_STANDARD &&
            nextState == WUD_STATE_SYNC_PREPARE_SEARCH) {

            nextState = WUD_STATE_SYNC_WAIT_FOR_INCOMING;

            enabled = OSDisableInterrupts();
            linkNum = p->linkedNum;
            OSRestoreInterrupts(enabled);

            if (linkNum == WUD_MAX_CHANNELS - 1) {
                p->UNK_0x74A = 0xC8;
            } else {
                p->UNK_0x74A = 0x64;
            }

            enabled = OSDisableInterrupts();
            p->discoverable = FALSE;
            p->connectable = TRUE;
            OSRestoreInterrupts(enabled);

            BTA_DmSetVisibility(FALSE, TRUE);
        }

        p->syncState = nextState;
        break;
    }

    case WUD_STATE_SYNC_IS_EXISTED_DEVICE: {
        u8 nextState = WUD_STATE_SYNC_PREPARE_FOR_UNKNOWN_DEVICE;
        WUDDevInfo* pInfo = WUDiGetDevInfo(_wudDiscResp.devAddr);

        if (pInfo != NULL) {
            memcpy(&_wudDiscWork, pInfo, sizeof(WUDDevInfo));
            nextState = WUD_STATE_SYNC_PREPARE_FOR_EXISTED_DEVICE;
        }

        p->syncState = nextState;
        break;
    }

    case WUD_STATE_SYNC_PREPARE_FOR_EXISTED_DEVICE: {
        u8 nextState;

        _wudDiscWork.status = 1;

        switch (_wudDiscWork.UNK_0x5B) {
        case 0:
        case 2:
        case 4:
        case 5: {
            if (p->syncType == WUD_SYNC_TYPE_STANDARD) {
                WUDiMoveBottomStdDevInfoPtr(&_wudDiscWork);
                WUDiRemoveDevice(_wudDiscWork.devAddr);
                nextState = WUD_STATE_SYNC_PREPARE_FOR_UNKNOWN_DEVICE;
                break;
            }

            _wudDiscWork.UNK_0x5B = 4;
            nextState = WUD_STATE_SYNC_TRY_CONNECT;
            break;
        }

        case 1:
        case 3: {
            WUDiMoveBottomSmpDevInfoPtr(&_wudDiscWork);
            WUDiRemoveDevice(p->smpListTail->devInfo->devAddr);
            nextState = WUD_STATE_SYNC_PREPARE_FOR_UNKNOWN_DEVICE;
            break;
        }

        default: {
            nextState = WUD_STATE_SYNC_TRY_CONNECT;
            break;
        }
        }

        p->syncState = nextState;
        break;
    }

    case WUD_STATE_SYNC_PREPARE_FOR_UNKNOWN_DEVICE: {
        _wudDiscWork.status = 1;
        _wudDiscWork.UNK_0x5B =
            p->syncType == WUD_SYNC_TYPE_STANDARD ? 0 : 1;

        WUD_BDCPY(_wudDiscWork.devAddr, _wudDiscResp.devAddr);
        memcpy(&_wudDiscWork.conf, _wudDiscResp.devName,
               sizeof(_wudDiscResp.devName));
        memset(_wudDiscWork.linkKey, 0, sizeof(LINK_KEY));

        p->syncState = WUD_STATE_SYNC_TRY_CONNECT;
        break;
    }

    case WUD_STATE_SYNC_6:
    case WUD_STATE_SYNC_13:
    case WUD_STATE_SYNC_CANCEL_SEARCH: {
        break;
    }

    case WUD_STATE_SYNC_TRY_CONNECT: {
        p->syncState = __wudSyncTryConnect();
        break;
    }

    case WUD_STATE_SYNC_REGISTER_DEVICE: {
        u8 nextState = _wudDiscWork.UNK_0x5B;

        switch (nextState) {
        case 0: {
            nextState = WUD_STATE_SYNC_VIRGIN_STANDARD;
            break;
        }

        case 1: {
            nextState = WUD_STATE_SYNC_VIRGIN_SIMPLE;
            break;
        }

        case 2:
        case 4: {
            nextState = WUD_STATE_SYNC_STORED_LINK_KEY_TO_EEPROM;
            break;
        }

        case 3: {
            nextState = WUD_STATE_SYNC_COMPLETE;
            break;
        }

        case 5: {
            nextState = WUD_STATE_SYNC_CHANGE_SIMPLE_TO_STANDARD;
            break;
        }

        default: {
            break;
        }
        }

        p->syncState = nextState;
        break;
    }

    case WUD_STATE_SYNC_VIRGIN_SIMPLE: {
        WUDDevInfo* pWork;
        u8 nextState;
        int i;

        enabled = OSDisableInterrupts();
        if (p->devSmpNums == WUD_MAX_DEV_ENTRY_FOR_SMP) {
            WUDiRemoveDevice(p->smpListTail->devInfo->devAddr);
        }
        OSRestoreInterrupts(enabled);

        enabled = OSDisableInterrupts();

        if (p->syncType == WUD_SYNC_TYPE_STANDARD) {
            pWork = NULL;

            for (i = 0; i < WUD_MAX_DEV_ENTRY_FOR_STD; i++) {
                if (p->stdDevs[i].status == 0) {
                    DEBUGPrint(pMsg + 0x2CC, i);
                    pWork = &p->stdDevs[i];
                    break;
                }
            }
        } else {
            pWork = p->smpListTail->devInfo;
        }

        OSRestoreInterrupts(enabled);

        if (pWork == NULL) {
            p->syncState = WUD_STATE_SYNC_ERROR;
            break;
        }

        if (pWork->status != 0) {
            p->syncState = WUD_STATE_SYNC_ERROR;
            break;
        }

        memcpy(pWork, &_wudDiscWork, sizeof(WUDDevInfo));
        WUDiRegisterDevice(pWork->devAddr);
        WUDiMoveTopSmpDevInfoPtr(pWork);
        _dev_handle_to_bda[pWork->devHandle] = pWork->devAddr;

        p->syncState = WUD_STATE_SYNC_COMPLETE;
        break;
    }

    case WUD_STATE_SYNC_VIRGIN_STANDARD: {
        p->syncState = __wudSyncVirginStandard();
        break;
    }

    case WUD_STATE_SYNC_CHANGE_SIMPLE_TO_STANDARD: {
        WUDiMoveBottomSmpDevInfoPtr(&_wudDiscWork);
        WUDiRemoveDevice(p->smpListTail->devInfo->devAddr);
        p->syncState = __wudSyncVirginStandard();
        break;
    }

    case WUD_STATE_SYNC_STORED_LINK_KEY_TO_EEPROM: {
        WUDDevInfo* pInfo;
        u8 linkKeyState;

        linkKeyState = p->linkKeyState != WUD_STATE_LINK_KEY_START;

        if (linkKeyState) {
            p->syncState = WUD_STATE_SYNC_STORED_LINK_KEY_TO_EEPROM;
            break;
        }

        if (_linkedWBC != 0 &&
            memcmp(&_wudDiscWork, pMsg + 0x27C, sizeof(LINK_KEY)) == 0) {

            p->syncState = WUD_STATE_SYNC_STORED_DEV_INFO_TO_NAND;
            break;
        }

        pInfo = WUDiGetDevInfo(_wudDiscWork.devAddr);

        DEBUGPrint(pMsg + 0x360);
        DEBUGPrint(pMsg + 0x378, pInfo->devAddr[0], pInfo->devAddr[1],
                   pInfo->devAddr[2], pInfo->devAddr[3], pInfo->devAddr[4],
                   pInfo->devAddr[5]);

        // clang-format off
        DEBUGPrint(pMsg + 0x3A0,
                   pInfo->linkKey[0],  pInfo->linkKey[1],  pInfo->linkKey[2],  pInfo->linkKey[3],
                   pInfo->linkKey[4],  pInfo->linkKey[5],  pInfo->linkKey[6],  pInfo->linkKey[7],
                   pInfo->linkKey[8],  pInfo->linkKey[9],  pInfo->linkKey[10], pInfo->linkKey[11],
                   pInfo->linkKey[12], pInfo->linkKey[13], pInfo->linkKey[14], pInfo->linkKey[15]);
        // clang-format on

        p->linkKeyState = WUD_STATE_LINK_KEY_WRITING;

        BTM_WriteStoredLinkKey(1, &pInfo->devAddr, &pInfo->linkKey,
                               (tBTM_CMPL_CB*)__wudLinkKeyEventStackCallback);

        p->syncState = WUD_STATE_SYNC_WAIT_FOR_STORING;
        break;
    }

    case WUD_STATE_SYNC_WAIT_FOR_STORING: {
        if (p->linkKeyState != WUD_STATE_LINK_KEY_START) {
            p->syncState = WUD_STATE_SYNC_WAIT_FOR_STORING;
        } else {
            p->syncState = WUD_STATE_SYNC_STORED_DEV_INFO_TO_NAND;
        }
        break;
    }

    case WUD_STATE_SYNC_STORED_DEV_INFO_TO_NAND: {
        p->syncState = __wudSyncStoredDevInfoToNand();
        break;
    }

    case WUD_STATE_SYNC_SC_FLUSH: {
        enabled = OSDisableInterrupts();
        p->syncState = WUD_STATE_SYNC_13;
        OSRestoreInterrupts(enabled);
        SCFlushAsync((SCFlushCallback)__wudSyncFlushCallback);
        break;
    }

    case WUD_STATE_SYNC_WAIT_FOR_READING: {
        u8 nextState = WUD_STATE_SYNC_WAIT_FOR_READING;

        if (p->linkKeyState == WUD_STATE_LINK_KEY_START) {
            nextState = WUD_STATE_SYNC_VIRGIN_STANDARD;
            WUDiRemoveDevice(p->pairAddr);
        }

        p->syncState = nextState;
        break;
    }

    case WUD_STATE_SYNC_COMPLETE: {
        p->syncedNum++;
        memset(&_wudDiscWork, 0, sizeof(WUDDevInfo));
        p->syncState = p->syncType == WUD_SYNC_TYPE_STANDARD
                           ? WUD_STATE_SYNC_DONE
                           : WUD_STATE_SYNC_PREPARE_SEARCH;
        break;
    }

    case WUD_STATE_SYNC_ERROR: {
        memset(&_wudDiscWork, 0, sizeof(WUDDevInfo));
        p->syncState = p->syncType == WUD_SYNC_TYPE_STANDARD
                           ? WUD_STATE_SYNC_DONE
                           : WUD_STATE_SYNC_PREPARE_SEARCH;
        break;
    }

    case WUD_STATE_SYNC_DONE: {
        p->syncState = __wudSyncDone();
        break;
    }

    case WUD_STATE_SYNC_WAIT_FOR_INCOMING: {
        u8 nextState = WUD_STATE_SYNC_WAIT_FOR_INCOMING;
        u8 linkNum;
        u8 connNum;

        enabled = OSDisableInterrupts();
        linkNum = p->linkedNum;
        OSRestoreInterrupts(enabled);

        if (linkNum == WUD_MAX_CHANNELS) {
            enabled = OSDisableInterrupts();
            connNum = p->connectedNum;
            OSRestoreInterrupts(enabled);

            if (connNum == WUD_MAX_CHANNELS) {
                nextState = WUD_STATE_SYNC_DONE;
            }
        }

        p->UNK_0x74A--;

        if (p->UNK_0x74A < 0) {
            nextState = WUD_STATE_SYNC_PREPARE_SEARCH;
        }

        p->syncState = nextState;
        break;
    }

    case 0x64: {
        if (_wudNandLocked != 0) {
            break;
        }

        DEBUGPrint(pMsg + 0x454, pMsg + 0x428);
        _wudNandLocked = 1;

        NANDOpenAsync(pMsg + 0x428, &_wudNandFileInfo, NAND_ACCESS_RW,
                      (NANDAsyncCallback)__wudOpenWiiFitCallback,
                      &_wudNandBlock);
        break;
    }

    case 0x65: {
        if (_wudNandLocked != 0) {
            break;
        }

        _wudNandLocked = 1;

        NANDSeekAsync(&_wudNandFileInfo, 0x4AF18, NAND_SEEK_BEG,
                      (NANDAsyncCallback)__wudSeekWiiFitCallback,
                      &_wudNandBlock);
        break;
    }

    case 0x66: {
        if (_wudNandLocked != 0) {
            break;
        }

        DEBUGPrint(pMsg + 0x468);
        DEBUGPrint(pMsg + 0x478, _wudNandWbcInfo[0], _wudNandWbcInfo[1],
                   _wudNandWbcInfo[2], _wudNandWbcInfo[3],
                   _wudNandWbcInfo[4], _wudNandWbcInfo[5]);
        DEBUGPrint(pMsg + 0x498, _wudNandWbcInfo[0xD],
                   _wudNandWbcInfo[6], _wudNandWbcInfo[7],
                   _wudNandWbcInfo[8], _wudNandWbcInfo[9],
                   _wudNandWbcInfo[0xA], _wudNandWbcInfo[0xB],
                   _wudNandWbcInfo[0xC]);
        DEBUGPrint(pMsg + 0x498, _wudNandWbcInfo[0x15],
                   _wudNandWbcInfo[0xE], _wudNandWbcInfo[0xF],
                   _wudNandWbcInfo[0x10], _wudNandWbcInfo[0x11],
                   _wudNandWbcInfo[0x12], _wudNandWbcInfo[0x13],
                   _wudNandWbcInfo[0x14]);
        DEBUGPrint(lbl_80665D68_80663438, &_wudNandWbcInfo[0x16]);
        DEBUGPrint(pMsg + 0x4C4, _wudNandWbcInfo[0x80],
                   _wudNandWbcInfo[0x81], _wudNandWbcInfo[0x82],
                   _wudNandWbcInfo[0x83]);

        _wudNandLocked = 1;

        NANDWriteAsync(&_wudNandFileInfo, _wudNandWbcInfo, 0x84,
                       (NANDAsyncCallback)__wudUpdateWiiFitCallback,
                       &_wudNandBlock);
        break;
    }

    case 0x67: {
        if (_wudNandLocked != 0) {
            break;
        }

        _wudNandLocked = 1;
        NANDCloseAsync(&_wudNandFileInfo,
                       (NANDAsyncCallback)__wudCloseWiiFitCallback,
                       &_wudNandBlock);
        break;
    }

    default: {
        DEBUGPrint(pMsg + 0x4D8);
        break;
    }
    }
}

void __wudSyncHandler0(OSAlarm* pAlarm, OSContext* pContext) {
    OSSwitchFiberEx((u32)pAlarm, (u32)pContext, 0, 0, __wudSyncHandler,
                    __WUDHandlerStack + sizeof(__WUDHandlerStack));
}
void __wudDeleteFlushCallback(SCStatus status) {
    extern char lbl_805627BC[];
    WUDCB* p = &_wcb;

    DEBUGPrint(lbl_805627BC, status, p->deleteState);

    if (p->deleteState != 0) {
        p->deleteState = WUD_STATE_DELETE_DONE;
    }
}

u8 __wudDeleteDisconnectAll(void) {
    WUDCB* p = &_wcb;
    WUDDevInfo* pDev;
    int i;

    for (i = 0, pDev = p->stdDevs; i < WUD_MAX_DEV_ENTRY_FOR_STD;
         i++, pDev++) {
        if (pDev->status > 1) {
            btm_remove_acl(pDev->devAddr);
        }
    }

    for (i = 0, pDev = p->smpDevs; i < WUD_MAX_DEV_ENTRY_FOR_SMP;
         i++, pDev++) {
        if (pDev->status > 1) {
            btm_remove_acl(pDev->devAddr);
        }
    }

    return WUD_STATE_DELETE_CLEANUP_DATABASE;
}

u8 __wudDeleteCleanupDatabase(void) {
    WUDCB* p = &_wcb;
    BOOL enabled;
    u8 linkedNum;
    u8 buf[0x10];
    u32 crc;
    int i;

    enabled = OSDisableInterrupts();
    linkedNum = p->linkedNum;
    OSRestoreInterrupts(enabled);

    if (linkedNum != 0) {
        return WUD_STATE_DELETE_CLEANUP_DATABASE;
    }

    for (i = 0; i < WUD_MAX_DEV_ENTRY_FOR_STD; i++) {
        if (p->stdDevs[i].status == 1) {
            WUDiRemoveDevice(p->stdDevs[i].devAddr);
        }
    }

    for (i = 0; i < WUD_MAX_DEV_ENTRY_FOR_SMP; i++) {
        if (p->smpDevs[i].status == 1) {
            WUDiRemoveDevice(p->smpDevs[i].devAddr);
        }
    }

    if (_linkedWBC != 0) {
        if (SCGetProductGameRegion() == 0) {
            memset(buf, 0, sizeof(buf));
            memcpy(&_wudNandWbcInfo[6], buf, sizeof(buf));

            {
                const u16* pData = (const u16*)&_wudNandWbcInfo[0];
                u32 sum = _wudNandWbcCrc >> 16;
                u16 invSum = _wudNandWbcCrc & 0xFFFF;
                int j;

                for (j = 0; j < 8; j++) {
                    sum += pData[0];
                    invSum = (u16)(invSum + ~pData[0]);
                    sum += pData[1];
                    invSum = (u16)(invSum + ~pData[1]);
                    sum += pData[2];
                    invSum = (u16)(invSum + ~pData[2]);
                    sum += pData[3];
                    invSum = (u16)(invSum + ~pData[3]);
                    sum += pData[4];
                    invSum = (u16)(invSum + ~pData[4]);
                    sum += pData[5];
                    invSum = (u16)(invSum + ~pData[5]);
                    sum += pData[6];
                    invSum = (u16)(invSum + ~pData[6]);
                    sum += pData[7];
                    invSum = (u16)(invSum + ~pData[7]);
                    pData += 8;
                }

                crc = (sum << 16) | invSum;
            }
            memcpy(&_wudNandWbcInfo[0x80], &crc, sizeof(crc));
            return 0x64;
        }
        return WUD_STATE_DELETE_CLEANUP_SETTING;
    }

    return WUD_STATE_DELETE_CLEANUP_SETTING;
}

void __wudDeleteHandler(void) {
    WUDCB* p = &_wcb;
    BOOL success;
    BOOL enabled;
    u8 connectable;

    p->syncState = WUD_STATE_SYNC_START;

    switch (p->deleteState) {
    case WUD_STATE_DELETE_DISALLOW_INCOMING: {
        BTA_DmSetVisibility(FALSE, FALSE);
        p->deleteState = WUD_STATE_DELETE_DISCONNECT_ALL;
        break;
    }

    case WUD_STATE_DELETE_DISCONNECT_ALL: {
        p->deleteState = __wudDeleteDisconnectAll();
        break;
    }

    case WUD_STATE_DELETE_CLEANUP_DATABASE: {
        p->deleteState = __wudDeleteCleanupDatabase();
        break;
    }

    case WUD_STATE_DELETE_CLEANUP_SETTING: {
        if (SCCheckStatus() == SC_STATUS_BUSY) {
            break;
        }

        memset(&_scArray, 0, sizeof(_scArray));
        memset(&_spArray, 0, sizeof(_spArray));

        success = SCSetBtDeviceInfoArray(&_scArray);
        success |= SCSetBtCmpDevInfoArray(&_spArray);

        if (success) {
            p->deleteState = WUD_STATE_DELETE_6;
            SCFlushAsync((SCFlushCallback)__wudDeleteFlushCallback);
        } else {
            p->deleteState = WUD_STATE_DELETE_DONE;
        }
        break;
    }

    case WUD_STATE_DELETE_6: {
        break;
    }

    case WUD_STATE_DELETE_DONE: {
        enabled = OSDisableInterrupts();
        connectable = p->connectable;
        OSRestoreInterrupts(enabled);

        enabled = OSDisableInterrupts();
        p->discoverable = FALSE;
        p->connectable = connectable;
        OSRestoreInterrupts(enabled);

        BTA_DmSetVisibility(FALSE, connectable);

        OSCancelAlarm(&p->alarm);
        p->deleteState = WUD_STATE_DELETE_START;

        if (p->clearDevCB != NULL) {
            p->clearDevCB(WUD_RESULT_DELETE_COMPLETE);
        }

        p->deleteState = WUD_STATE_DELETE_START;
        break;
    }

    case 0x64: {
        if (_wudNandLocked != 0) {
            break;
        }

        DEBUGPrint(_wudWiiRemoteDescriptor + 0x454,
                   _wudWiiRemoteDescriptor + 0x428);
        _wudNandLocked = 1;

        NANDOpenAsync(_wudWiiRemoteDescriptor + 0x428, &_wudNandFileInfo,
                      NAND_ACCESS_RW, (NANDAsyncCallback)__wudOpenWiiFitCallback,
                      &_wudNandBlock);
        break;
    }

    case 0x65: {
        if (_wudNandLocked != 0) {
            break;
        }

        _wudNandLocked = 1;

        NANDSeekAsync(&_wudNandFileInfo, 0x4AF18, NAND_SEEK_BEG,
                      (NANDAsyncCallback)__wudSeekWiiFitCallback,
                      &_wudNandBlock);
        break;
    }

    case 0x66: {
        if (_wudNandLocked != 0) {
            break;
        }

        DEBUGPrint(_wudWiiRemoteDescriptor + 0x468);
        DEBUGPrint(_wudWiiRemoteDescriptor + 0x478, _wudNandWbcInfo[0],
                   _wudNandWbcInfo[1], _wudNandWbcInfo[2],
                   _wudNandWbcInfo[3], _wudNandWbcInfo[4],
                   _wudNandWbcInfo[5]);
        DEBUGPrint(_wudWiiRemoteDescriptor + 0x498, _wudNandWbcInfo[0xD],
                   _wudNandWbcInfo[6], _wudNandWbcInfo[7],
                   _wudNandWbcInfo[8], _wudNandWbcInfo[9],
                   _wudNandWbcInfo[0xA], _wudNandWbcInfo[0xB],
                   _wudNandWbcInfo[0xC]);
        DEBUGPrint(_wudWiiRemoteDescriptor + 0x498, _wudNandWbcInfo[0x15],
                   _wudNandWbcInfo[0xE], _wudNandWbcInfo[0xF],
                   _wudNandWbcInfo[0x10], _wudNandWbcInfo[0x11],
                   _wudNandWbcInfo[0x12], _wudNandWbcInfo[0x13],
                   _wudNandWbcInfo[0x14]);
        DEBUGPrint(lbl_80665D68_80663438, &_wudNandWbcInfo[0x16]);
        DEBUGPrint(_wudWiiRemoteDescriptor + 0x4C4, _wudNandWbcInfo[0x80],
                   _wudNandWbcInfo[0x81], _wudNandWbcInfo[0x82],
                   _wudNandWbcInfo[0x83]);

        _wudNandLocked = 1;

        NANDWriteAsync(&_wudNandFileInfo, _wudNandWbcInfo, 0x84,
                       (NANDAsyncCallback)__wudUpdateWiiFitCallback,
                       &_wudNandBlock);
        break;
    }

    case 0x67: {
        if (_wudNandLocked != 0) {
            break;
        }

        _wudNandLocked = 1;
        NANDCloseAsync(&_wudNandFileInfo,
                       (NANDAsyncCallback)__wudCloseWiiFitCallback,
                       &_wudNandBlock);
        break;
    }
    }
}

void __wudDeleteHandler0(OSAlarm* pAlarm, OSContext* pContext) {
    OSSwitchFiberEx((u32)pAlarm, (u32)pContext, 0, 0, __wudDeleteHandler,
                    __WUDHandlerStack + sizeof(__WUDHandlerStack));
}
BD_ADDR_PTR _WUDGetDevAddr(UINT8 handle) {
    BD_ADDR_PTR pAddr;
    BOOL enabled = OSDisableInterrupts();

    if (handle < WUD_MAX_DEV_ENTRY) {
        pAddr = _dev_handle_to_bda[handle];

    } else {
        pAddr = NULL;
    }

    OSRestoreInterrupts(enabled);
    return pAddr;
}

u16 _WUDGetQueuedSize(s8 handle) {
    u16 queuedSize;
    BOOL enabled = OSDisableInterrupts();

    if ((u8)handle <= WUD_MAX_DEV_ENTRY - 1) {
        queuedSize = _dev_handle_queue_size[(u8)handle];

    } else {
        queuedSize = 0;
    }

    OSRestoreInterrupts(enabled);
    return queuedSize;
}

u16 _WUDGetNotAckedSize(s8 handle) {
    u16 notAckedSize;
    BOOL enabled = OSDisableInterrupts();

    if ((u8)handle <= WUD_MAX_DEV_ENTRY - 1) {
        notAckedSize = _dev_handle_notack_num[(u8)handle];
    } else {
        notAckedSize = 0;
    }

    OSRestoreInterrupts(enabled);
    return notAckedSize;
}

u8 __wudStackCheckDeviceInfo(void) {
    WUDCB* p = &_wcb;
    WUDDevInfo* pDev;
    WUDDevInfo* pInfo;
    BOOL enabled;
    int i;

    if (p->linkKeyState == 0) {
        for (i = 0; i < WUD_MAX_DEV_ENTRY; i++) {
            enabled = OSDisableInterrupts();
            pDev = ((u32)i <= 9)
                       ? &_wcb.stdDevs[i]
                       : &_wcb.smpDevs[i - WUD_MAX_DEV_ENTRY_FOR_STD];
            OSRestoreInterrupts(enabled);

            if (pDev->status == 0) {
                continue;
            }

            if (pDev->UNK_0x5C == 1) {
                p->linkKeyState = WUD_STATE_LINK_KEY_DELETING;
                BTM_DeleteStoredLinkKey(
                    pDev->devAddr,
                    (tBTM_CMPL_CB*)__wudLinkKeyEventStackCallback);
                pDev->UNK_0x5C = 0;
                return WUD_STATE_STACK_CHECK_DEVICE_INFO;
            }

            if (pDev->UNK_0x5C != 3) {
                pInfo = WUDiGetDevInfo(pDev->devAddr);

                if (pInfo != NULL) {
                    enabled = OSDisableInterrupts();
                    memset(pInfo, 0, sizeof(WUDDevInfo));
                    OSRestoreInterrupts(enabled);
                }
            }
        }

        return WUD_STATE_STACK_DONE;
    }

    return WUD_STATE_STACK_CHECK_DEVICE_INFO;
}

void __wudStackHandler(void) {
    char* pMsg = _wudWiiRemoteDescriptor;
    WUDCB* p = &_wcb;
    tBTM_VERSION_INFO version;

    switch (p->stackState) {
    case WUD_STATE_STACK_GET_STORED_LINK_KEY:
        if (p->hhFlags == 1) {
            p->linkKeyState = WUD_STATE_LINK_KEY_READING;
            BTM_ReadStoredLinkKey(NULL,
                                  (tBTM_CMPL_CB*)__wudLinkKeyEventStackCallback);
        }

        p->stackState = WUD_STATE_STACK_CHECK_DEVICE_INFO;
        break;

    case WUD_STATE_STACK_CHECK_DEVICE_INFO:
        p->stackState = __wudStackCheckDeviceInfo();
        break;

    case WUD_STATE_STACK_DONE: {
        u8 nextState;

        if (p->linkKeyState == 0) {
            OSCancelAlarm(&p->alarm);
            BTM_ReadLocalVersion(&version);

            DEBUGPrint(pMsg + 0x524, version.hci_version);
            DEBUGPrint(pMsg + 0x53C, version.hci_revision);
            DEBUGPrint(pMsg + 0x554, version.lmp_version);
            DEBUGPrint(pMsg + 0x56C, version.lmp_subversion);
            DEBUGPrint(pMsg + 0x584, version.manufacturer);
            DEBUGPrint(pMsg + 0x59C, version.hci_revision & 0xFFF);

            if ((version.hci_revision & 0xFFF) == 0xA7) {
                __wudAppendRuntimePatch();
            } else {
                __wudInitSub();
            }

            nextState = WUD_STATE_STACK_INITIALIZED;
        } else {
            nextState = WUD_STATE_STACK_CHECK_DEVICE_INFO;
        }

        p->stackState = nextState;
        break;
    }
}
}

void __wudStackHandler0(OSAlarm* pAlarm, OSContext* pContext) {
    OSSwitchFiberEx((u32)pAlarm, (u32)pContext, 0, 0, __wudStackHandler,
                    __WUDHandlerStack + sizeof(__WUDHandlerStack));
}

void __wudInitFlushCallback(s32 result) {
    extern char lbl_8056288C[];

    DEBUGPrint(lbl_8056288C, result, _wcb.initState);
    _wcb.initState = 5;
}

void __wudInitDevInfo(void) {
    extern char _wudWiiRemoteDescriptor[];
    WUDCB* p = &_wcb;
    char* pMsg = _wudWiiRemoteDescriptor;
    BD_ADDR zeroAddr;
    WUDDevInfo* pDev;
    BOOL enabled;
    u8 numStd;
    int i;
    int j;
    int count = 0;

    memset(&_scArray, 0, sizeof(_scArray));
    memset(&_spArray, 0, sizeof(_spArray));
    memset(zeroAddr, 0, sizeof(BD_ADDR));

    SCGetBtDeviceInfoArray(&_scArray);
    SCGetBtCmpDevInfoArray(&_spArray);

    p->syncType = WUD_SYNC_TYPE_STANDARD;

    numStd = _scArray.numRegist;

    for (i = 0; i < WUD_MAX_DEV_ENTRY_FOR_STD; i++) {
        if (numStd == 0) {
            break;
        }

        if (memcmp(&_scArray.regist[i].info, pMsg + 0x268,
                   sizeof(LINK_KEY)) != 0) {
            memset(&_scArray.regist[i], 0, sizeof(SCBtDeviceInfo));
        }

        if (memcmp(&_scArray.regist[i].addr, zeroAddr, sizeof(BD_ADDR)) !=
            0) {
            goto process;
        }

        if (i >= WUD_MAX_DEV_ENTRY_FOR_STD - 1) {
            continue;
        }

        for (j = i + 1; j < WUD_MAX_DEV_ENTRY_FOR_STD; j++) {
            if (memcmp(&_scArray.regist[j].info, pMsg + 0x268,
                       sizeof(LINK_KEY)) == 0) {
                memcpy(&_scArray.regist[i], &_scArray.regist[j],
                       sizeof(SCBtDeviceInfo));
                memset(&_scArray.regist[j], 0, sizeof(SCBtDeviceInfo));
                goto process;
            }
        }

        continue;

    process:
        enabled = OSDisableInterrupts();

        if (p->syncType == WUD_SYNC_TYPE_STANDARD) {
            pDev = NULL;
            for (j = 0; j < WUD_MAX_DEV_ENTRY_FOR_STD; j++) {
                if (p->stdDevs[j].status == 0) {
                    DEBUGPrint(pMsg + 0x2CC, j);
                    pDev = &p->stdDevs[j];
                    break;
                }
            }
        } else {
            pDev = p->smpListTail->devInfo;
        }

        OSRestoreInterrupts(enabled);

        if (pDev != NULL) {
            memcpy(pDev->devAddr, &_scArray.regist[i].addr, sizeof(BD_ADDR));
            memcpy(pDev, &_scArray.regist[i].info, sizeof(SCDevInfo));

            pDev->status = 1;
            pDev->UNK_0x5B = 0;
            pDev->UNK_0x5C = 2;
            pDev->subclass = 2;
            pDev->hhAttrMask = 0x8074;
            pDev->appID = 3;

            DEBUGPrint(pMsg + 0x378, pDev->devAddr[0], pDev->devAddr[1],
                       pDev->devAddr[2], pDev->devAddr[3], pDev->devAddr[4],
                       pDev->devAddr[5]);
            DEBUGPrint(pMsg + 0x5F0, pDev);

            count++;
            numStd--;
        }
    }

    if (_linkedWBC != 0) {
        if (memcmp(&_scArray.active[4].info, pMsg + 0x27C,
                   sizeof(LINK_KEY)) == 0) {
            enabled = OSDisableInterrupts();

            if (p->syncType == WUD_SYNC_TYPE_STANDARD) {
                pDev = NULL;
                for (j = 0; j < WUD_MAX_DEV_ENTRY_FOR_STD; j++) {
                    if (p->stdDevs[j].status == 0) {
                        DEBUGPrint(pMsg + 0x2CC, j);
                        pDev = &p->stdDevs[j];
                        break;
                    }
                }
            } else {
                pDev = p->smpListTail->devInfo;
            }

            OSRestoreInterrupts(enabled);

            if (pDev == NULL) {
                pDev = &p->stdDevs[WUD_MAX_DEV_ENTRY_FOR_STD - 1];
                count--;
            }

            memcpy(pDev->devAddr, &_scArray.active[4].addr,
                   sizeof(BD_ADDR));
            memcpy(pDev, &_scArray.active[4].info, 0x13);
            memcpy(pDev->linkKey, (u8*)&_scArray.active[4].info + 0x14,
                   sizeof(LINK_KEY));

            pDev->status = 1;
            pDev->UNK_0x5B = 0;
            pDev->subclass = 2;
            pDev->hhAttrMask = 0x8074;
            pDev->appID = 3;
            pDev->UNK_0x5C = 3;

            DEBUGPrint(pMsg + 0x378, pDev->devAddr[0], pDev->devAddr[1],
                       pDev->devAddr[2], pDev->devAddr[3], pDev->devAddr[4],
                       pDev->devAddr[5]);
            DEBUGPrint(pMsg + 0x5F0, pDev);

            memset(&_scArray.regist[count], 0, sizeof(SCBtDeviceInfo));
            memcpy(&_scArray.regist[count].addr, &_scArray.active[4].addr,
                   sizeof(BD_ADDR));
            memcpy(&_scArray.regist[count].info,
                   (u8*)&_scArray.active[4].info, 0x13);

            count++;
        }
    }

    _scArray.numRegist = count;
    p->syncType = WUD_SYNC_TYPE_SIMPLE;
    p->initState = WUD_STATE_INIT_INITIALIZED;

    memset(&_spArray, 0, sizeof(_spArray));

    SCSetBtDeviceInfoArray(&_scArray);
    SCSetBtCmpDevInfoArray(&_spArray);
    SCFlushAsync((SCFlushCallback)__wudInitFlushCallback);
}

void __wudNandResultCallback(s32 result) {
    s32 phase = _wudNandPhase;
    _wudNandLocked = 0;

    switch (phase) {
    case 1: {
        s32 newPhase = 0xFF;
        if (result == 0) {
            newPhase = phase + 1;
        }
        _wudNandPhase = newPhase;
        break;
    }

    case 2: {
        s32 newPhase = 5;
        if (result == 0) {
            newPhase = phase + 1;
        }
        _wudNandPhase = newPhase;
        break;
    }

    case 3: {
        s32 newPhase = 5;
        if ((u32)(result - 0x40000) == 0xB000) {
            newPhase = phase + 1;
        }
        _wudNandPhase = newPhase;
        break;
    }

    default:
        _wudNandPhase = 6;
        break;
    }
}

void __wudNandFlushCallback(void) {
    _wudNandLocked = 0;
    _wudNandPhase = _wudNandPhase + 1;
}
u8 __wudGetDevInfoFromWiiFit(void) {
    WUDCB* p = &_wcb;
    char* pMsg = _wudWiiRemoteDescriptor;
    u8 nextState = WUD_STATE_INIT_GET_DEV_INFO;
    u8 zero[0x10] = {0};
    u32 crc;
    u32 savedCrc;
    s32 phase;

    if (_wudNandLocked != 0) {
        return WUD_STATE_INIT_GET_DEV_INFO;
    }

    if (_wudNandBufPtr == NULL || ((u32)_wudNandBufPtr & 0x1F) != 0) {
        OSPanic(lbl_80665D78_80663448, 0xA90, pMsg + 0x610);
    }

    phase = _wudNandPhase;

    switch (phase) {
    case 0: {
        s32 result = NANDInit();

        _wudNandPhase = result == 0 ? 1 : 0xFF;
        DEBUGPrint(pMsg + 0x668, result);
        break;
    }

    case 1: {
        if (SCGetProductGameRegion() == 0) {
            DEBUGPrint(pMsg + 0x454, pMsg + 0x428);
            _wudNandLocked = 1;
            _wudNandWbcCrc = 0;

            NANDOpenAsync(pMsg + 0x428, &_wudNandFileInfo, NAND_ACCESS_READ,
                          (NANDAsyncCallback)__wudNandResultCallback,
                          &_wudNandBlock);
        } else {
            _wudNandPhase = 6;
        }
        break;
    }

    case 2: {
        _wudNandLocked = 1;

        NANDSeekAsync(&_wudNandFileInfo, 0, NAND_SEEK_BEG,
                      (NANDAsyncCallback)__wudNandResultCallback,
                      &_wudNandBlock);
        break;
    }

    case 3: {
        _wudNandLocked = 1;

        NANDReadAsync(&_wudNandFileInfo, _wudNandBufPtr, 0x4B000,
                      (NANDAsyncCallback)__wudNandResultCallback,
                      &_wudNandBlock);
        break;
    }

    case 4: {
        memcpy(&_wudNandWbcInfo[0], _wudNandBufPtr + 0x4AF18, 0x80);
        memcpy(&savedCrc, _wudNandBufPtr + 0x4AF98, sizeof(u32));

        DEBUGPrint(pMsg + 0x678, savedCrc, savedCrc);

        {
            const u16* pData = (const u16*)_wudNandBufPtr;
            u32 sum = 0;
            u16 invSum = 0;
            int j;

            for (j = 0; j < 0x95E3; j++) {
                sum += pData[0];
                invSum = (u16)(invSum + ~pData[0]);
                sum += pData[1];
                invSum = (u16)(invSum + ~pData[1]);
                sum += pData[2];
                invSum = (u16)(invSum + ~pData[2]);
                sum += pData[3];
                invSum = (u16)(invSum + ~pData[3]);
                pData += 4;
            }

            crc = (sum << 16) | invSum;
        }
        _wudNandWbcCrc = crc;

            {
                const u16* pData = (const u16*)_wudNandBufPtr + 0x4AF18;
                u32 sum = crc >> 16;
                u16 invSum = crc & 0xFFFF;
                int j;

                for (j = 0; j < 8; j++) {
                    sum += pData[0];
                    invSum = (u16)(invSum + ~pData[0]);
                    sum += pData[1];
                    invSum = (u16)(invSum + ~pData[1]);
                    sum += pData[2];
                    invSum = (u16)(invSum + ~pData[2]);
                    sum += pData[3];
                    invSum = (u16)(invSum + ~pData[3]);
                    sum += pData[4];
                    invSum = (u16)(invSum + ~pData[4]);
                    sum += pData[5];
                    invSum = (u16)(invSum + ~pData[5]);
                    sum += pData[6];
                    invSum = (u16)(invSum + ~pData[6]);
                    sum += pData[7];
                    invSum = (u16)(invSum + ~pData[7]);
                    pData += 8;
                }

                crc = (sum << 16) | invSum;
            }
        DEBUGPrint(pMsg + 0x688, crc, crc);

        if (savedCrc != crc) {
            _wudNandPhase = 5;
            DEBUGPrint(pMsg + 0x698);
            break;
        }

        if (memcmp(&_wudNandWbcInfo[0], zero, sizeof(BD_ADDR)) == 0) {
            _wudNandPhase = 5;
            DEBUGPrint(pMsg + 0x6B8);
            break;
        }

        if (memcmp(OSGetAppGamename(), lbl_80665D70_80663440, 4) != 0 &&
            memcmp(&_wudNandWbcInfo[6], zero, sizeof(LINK_KEY)) == 0) {

            _wudNandPhase = 5;
            DEBUGPrint(pMsg + 0x6DC);
            break;
        }

        memset(_wudNandBufPtr, 0, 0x4B000);
        memset(&_scArray, 0, sizeof(_scArray));
        SCGetBtDeviceInfoArray(&_scArray);

        memcpy(&_scArray.active[4].addr, &_wudNandWbcInfo[0],
               sizeof(BD_ADDR));
        memcpy(&_scArray.active[4].info, &_wudNandWbcInfo[0x16],
               sizeof(SCDevInfo));
        memcpy(&_scArray.active[4].info.at_0x14, &_wudNandWbcInfo[6],
               sizeof(LINK_KEY));

        DEBUGPrint(pMsg + 0x70C);
        DEBUGPrint(pMsg + 0x478, _wudNandWbcInfo[0], _wudNandWbcInfo[1],
                   _wudNandWbcInfo[2], _wudNandWbcInfo[3],
                   _wudNandWbcInfo[4], _wudNandWbcInfo[5]);
        DEBUGPrint(pMsg + 0x498, _wudNandWbcInfo[0xD],
                   _wudNandWbcInfo[6], _wudNandWbcInfo[7],
                   _wudNandWbcInfo[8], _wudNandWbcInfo[9],
                   _wudNandWbcInfo[0xA], _wudNandWbcInfo[0xB],
                   _wudNandWbcInfo[0xC]);
        DEBUGPrint(pMsg + 0x498, _wudNandWbcInfo[0x15],
                   _wudNandWbcInfo[0xE], _wudNandWbcInfo[0xF],
                   _wudNandWbcInfo[0x10], _wudNandWbcInfo[0x11],
                   _wudNandWbcInfo[0x12], _wudNandWbcInfo[0x13],
                   _wudNandWbcInfo[0x14]);
        DEBUGPrint(lbl_80665D68_80663438, &_wudNandWbcInfo[0x16]);
        DEBUGPrint(pMsg + 0x4C4, _wudNandWbcInfo[0x80],
                   _wudNandWbcInfo[0x81], _wudNandWbcInfo[0x82],
                   _wudNandWbcInfo[0x83]);

        SCSetBtDeviceInfoArray(&_scArray);
        _wudNandLocked = 1;
        SCFlushAsync((SCFlushCallback)__wudNandFlushCallback);
        break;
    }

    case 5: {
        _wudNandLocked = 1;

        NANDCloseAsync(&_wudNandFileInfo,
                       (NANDAsyncCallback)__wudNandResultCallback,
                       &_wudNandBlock);
        break;
    }

    default: {
        nextState = WUD_STATE_INIT_DONE;
        break;
    }
    }

    return nextState;
}

void __wudInitHandler(void) {
    WUDCB* p = &_wcb;
    u8 nextState;

    switch (p->initState) {
    case WUD_STATE_INIT_WAIT_FOR_INITIALIZATION:
        nextState = WUD_STATE_INIT_WAIT_FOR_INITIALIZATION;

        if (OS_TICKS_TO_MSEC((u32)(__OSGetSystemTime() - __OSStartTime)) >
            500) {
            if (SCCheckStatus() != SC_STATUS_BUSY) {
                __wudClearControlBlock();
                nextState = WUD_STATE_INIT_DONE;

                if (_linkedWBC != 0) {
                    if (SCGetProductGameRegion() == 0) {
                        _wudNandLocked = 0;
                        nextState = WUD_STATE_INIT_GET_DEV_INFO;
                        _wudNandPhase = 0;
                    }
                }
            }
        }

        p->initState = nextState;
        break;

    case WUD_STATE_INIT_DONE:
        __wudInitDevInfo();
        break;

    case 5:
        OSCancelAlarm(&p->alarm);
        p->libStatus = 1;
        BTA_EnableBluetooth(
            (tBTA_DM_SEC_CBACK*)__wudSecurityEventStackCallback);
        p->initState = 6;
        break;

    case WUD_STATE_INIT_GET_DEV_INFO:
        p->initState = __wudGetDevInfoFromWiiFit();
        break;
    }
}

void __wudInitHandler0(OSAlarm* pAlarm, OSContext* pContext) {
    OSSwitchFiberEx((u32)pAlarm, (u32)pContext, 0, 0, __wudInitHandler,
                    __WUDHandlerStack + sizeof(__WUDHandlerStack));
}

void __wudShutdownFlushCallback(s32 result) {
    extern char lbl_805629E8[];

    DEBUGPrint(lbl_805629E8, result, _wcb.shutdownState);
    _wcb.shutdownState = WUD_STATE_SHUTDOWN_DONE;
}

void __wudShutdownHandler(void) {
    WUDCB* p = &_wcb;

    switch (p->shutdownState) {
    case WUD_STATE_SHUTDOWN_STORE_SETTINGS: {
        BOOL result = _wudReadNand;

        if (SCCheckStatus() != SC_STATUS_BUSY) {
            result &= SCSetBtDeviceInfoArray(&_scArray);
            result &= SCSetBtCmpDevInfoArray(&_spArray);

            if (result) {
                p->shutdownState = WUD_STATE_SHUTDOWN_FLUSH_SETTINGS;
                SCFlushAsync((SCFlushCallback)__wudShutdownFlushCallback);
            } else {
                p->shutdownState = WUD_STATE_SHUTDOWN_DONE;
            }
        }
        break;
    }

    case WUD_STATE_SHUTDOWN_DONE: {
        OSCancelAlarm(&p->alarm);
        BTA_DisableBluetooth();
        break;
    }
    }
}

void __wudShutdownHandler0(OSAlarm* pAlarm, OSContext* pContext) {
    OSSwitchFiberEx((u32)pAlarm, (u32)pContext, 0, 0, __wudShutdownHandler,
                    __WUDHandlerStack + sizeof(__WUDHandlerStack));
}
void __wudClearControlBlock(void) {
    extern char lbl_80562A20[];
    WUDCB* p = &_wcb;
    u8 h;
    int i;
    BOOL enabled;

    DEBUGPrint(lbl_80562A20);

    for (h = 0; h < WUD_MAX_DEV_ENTRY; h++) {
        _dev_handle_to_bda[h] = NULL;
        _dev_handle_queue_size[h] = 0;
        _dev_handle_notack_num[h] = 0;
    }

    p->smpListHead = &p->smpList[0];
    p->smpListTail = &p->smpList[WUD_MAX_DEV_ENTRY_FOR_SMP - 1];

    for (i = 0; i < WUD_MAX_DEV_ENTRY_FOR_SMP / 2; i++) {
        p->smpList[2 * i].devInfo =
            &p->smpDevs[WUD_MAX_DEV_ENTRY_FOR_SMP - 1 - i];
        p->smpList[2 * i].prev = i == 0 ? NULL : &p->smpList[i - 1];
        p->smpList[2 * i].next = i == WUD_MAX_DEV_ENTRY_FOR_SMP - 1
                                     ? NULL
                                     : &p->smpList[i + 1];
        p->smpList[2 * i + 1].devInfo =
            &p->smpDevs[WUD_MAX_DEV_ENTRY_FOR_SMP - 2 - i];
    }

    p->stdListHead = &p->stdList[0];
    p->stdListTail = &p->stdList[WUD_MAX_DEV_ENTRY_FOR_STD - 1];

    for (i = 0; i < WUD_MAX_DEV_ENTRY_FOR_STD; i++) {
        p->stdList[i].devInfo = &p->stdDevs[i];
        p->stdList[i].prev = i == 0 ? NULL : &p->stdList[i - 1];
        p->stdList[i].next = i == WUD_MAX_DEV_ENTRY_FOR_STD - 1
                                 ? NULL
                                 : &p->stdList[i + 1];
    }

    p->syncState = WUD_STATE_SYNC_START;
    p->linkKeyState = WUD_STATE_LINK_KEY_START;
    p->deleteState = WUD_STATE_DELETE_START;
    p->stackState = WUD_STATE_STACK_START;
    p->initState = WUD_STATE_INIT_START;
    p->shutdownState = WUD_STATE_SHUTDOWN_START;
    p->syncSkipChecks = FALSE;
    p->syncType = WUD_SYNC_TYPE_STANDARD;
    p->syncLoopNum = 1;
    p->connectable = FALSE;
    p->discoverable = FALSE;
    p->libStatus = WUD_LIB_STATUS_0;
    p->pmID = 0;
    p->syncRssi = -65;

    memset(p->hostAddr, 0, sizeof(BD_ADDR));
    memset(p->pairAddr, 0, sizeof(BD_ADDR));

    p->bufferStatus0 = 0;
    p->bufferStatus1 = 0xa;

    enabled = OSDisableInterrupts();

    memset(p->stdDevs, 0, sizeof(p->stdDevs));
    memset(p->smpDevs, 0, sizeof(p->smpDevs));

    p->devNums = 0;
    p->devSmpNums = 0;
    p->connectedNum = 0;
    p->linkedNum = 0;
    p->syncedNum = 0;

    OSRestoreInterrupts(enabled);
}
BOOL __wudStartSyncDevice(u8 syncType, s8 syncLoopNum, u8 target,
                           BOOL syncSkipChecks) {
    WUDCB* p = &_wcb;
    BOOL success = FALSE;
    BOOL enabled;
    BOOL busy;
    u32 libStatus;

    enabled = OSDisableInterrupts();
    libStatus = (u32)p->libStatus;
    OSRestoreInterrupts(enabled);

    if (libStatus == WUD_LIB_STATUS_3) {
        enabled = OSDisableInterrupts();

        if (p->syncState == WUD_STATE_SYNC_START &&
            p->deleteState == WUD_STATE_DELETE_START &&
            p->stackState == WUD_STATE_STACK_INITIALIZED &&
            p->initState == 6) {
            OSRestoreInterrupts(enabled);
            busy = FALSE;
        } else {
            OSRestoreInterrupts(enabled);
            busy = TRUE;
        }

        if (!busy) {
            enabled = OSDisableInterrupts();

            _wudTarget = target;
            p->syncState = WUD_STATE_SYNC_PREPARE_SEARCH;
            p->syncLoopNum = syncLoopNum;
            p->syncType = syncType;
            // normalize syncSkipChecks != 0 to 0/1 (retail -O4,p neg/or/rlwinm)
            p->syncSkipChecks = (u32)(-syncSkipChecks | syncSkipChecks) >> 31;
            p->syncedNum = 0;
            p->UNK_0x748 = 0x32;
            p->UNK_0x74A = 0xc8;

            OSCreateAlarm(&p->alarm);
            // OS_MSEC_TO_TICKS(20): busClock/4/1000*20 via fixed-point magic
            // (retail -O4,p codegen; -O4,s would emit divwu)
            OSSetPeriodicAlarm(
                &p->alarm, OSGetTime(),
                (u32)(__mulhwu(0x10624DD3, OS_BUS_CLOCK_SPEED >> 2) >> 6) *
                    20,
                __wudSyncHandler0);

            OSRestoreInterrupts(enabled);

            success = TRUE;
        }
    }

    return success;
}
BOOL WUDCancelSyncDevice(void) {
    BOOL enabled;
    BOOL success = FALSE;
    WUDCB* p = &_wcb;

    _wudAbortSync = TRUE;
    enabled = OSDisableInterrupts();

    if (p->libStatus == WUD_LIB_STATUS_3) {
        if (p->syncLoopNum != 0) {
            if (p->syncState == WUD_STATE_SYNC_WAIT_FOR_SEARCH_RESULT) {
                BTA_DmSearchCancel();
            }

            p->syncLoopNum = 0;
        }

        success = TRUE;
    }

    OSRestoreInterrupts(enabled);
    return success;
}

void __wudModuleRebootCallback(void) {
    extern char lbl_80562B4C[];

    DEBUGPrint(lbl_80562B4C);
    __wudInitSub();
}

void __wudInstallPatchCallback(tBTM_VSC_CMPL* p1) {
    u8 buf[WUD_PATCH_BUFFER_SIZE + 1];
    u8 num;

    if (_wudPatchNum == _wudInstallNum || p1 == NULL) {
        DEBUGPrint(_wudWiiRemoteDescriptor + 0x8B0);
        BTM_DeviceReset((tBTM_CMPL_CB*)__wudModuleRebootCallback);
    } else {
        num = MIN(_wudPatchNum - _wudInstallNum, WUD_MAX_PATCHES);

        buf[0] = num;
        memcpy(&buf[1],
               &_wudWiiRemoteDescriptor[0x1A4 + 1 + _wudInstallNum * 13],
               num * sizeof(WUDPatchCmd));

        _wudInstallNum += num;
        DEBUGPrint(_wudWiiRemoteDescriptor + 0x8C0);

        BTM_VendorSpecificCommand(BT_VSC_NINTENDO_INSTALL_PATCH,
                                  num * sizeof(WUDPatchCmd) + 1, buf,
                                  __wudInstallPatchCallback);
    }
}

void __wudWritePatchCallback(tBTM_VSC_CMPL* p1) {
    u8 buf2[WUD_PATCH_BUFFER_SIZE + 1];
    u8 buf[WUD_PATCH_BUFFER_SIZE + 1];
    u32 address;
    u8 length;
    int i;

    if (p1 != NULL) {
        if (_wudPatchSize == _wudPatchOffset) {
            _wudInstallNum = 0;
            _wudPatchNum = _wudWiiRemoteDescriptor[0x1A4];

            length = MIN(_wudPatchNum, WUD_MAX_PATCHES);
            buf2[0] = length;
            memcpy(&buf2[1], &_wudWiiRemoteDescriptor[0x1A5],
                   length * sizeof(WUDPatchCmd));

            _wudInstallNum += length;
            DEBUGPrint(_wudWiiRemoteDescriptor + 0x8C0);

            BTM_VendorSpecificCommand(BT_VSC_NINTENDO_INSTALL_PATCH,
                                      sizeof(u8) + length * sizeof(WUDPatchCmd),
                                      buf2, __wudInstallPatchCallback);
        } else {
            length = MIN(_wudPatchSize - _wudPatchOffset,
                         WUD_PATCH_BUFFER_SIZE - sizeof(u32));
            address = _wudPatchAddress + _wudPatchOffset;

            buf[0] = (u8)address;
            buf[1] = (u8)(address >> 16);
            buf[2] = (u8)(address >> 8);
            buf[3] = (u8)(address >> 24);

            for (i = 0; i < (s32)length; i++) {
                buf[4 + i] = _wudWiiRemoteDescriptor[0xE8 + 8 + i];
            }

            _wudPatchOffset += length;
            DEBUGPrint(_wudWiiRemoteDescriptor + 0x8D8, length);

            BTM_VendorSpecificCommand(BT_VSC_NINTENDO_WRITE_PATCH,
                                      sizeof(u32) + length, buf,
                                      __wudWritePatchCallback);
        }
    } else {
        DEBUGPrint(_wudWiiRemoteDescriptor + 0x8B0);
        BTM_DeviceReset((tBTM_CMPL_CB*)__wudModuleRebootCallback);
    }
}

void __wudRemovePatchCallback(tBTM_VSC_CMPL* p1) {
    u8 buf[WUD_PATCH_BUFFER_SIZE + 1];
    u32 address;
    u8 length;
    int i;

    DEBUGPrint(_wudWiiRemoteDescriptor + 0x8F8);

    if (p1 != NULL) {
        length = MIN(_wudPatchSize, WUD_PATCH_BUFFER_SIZE - sizeof(u32));
        address = _wudPatchAddress;

        buf[0] = (u8)address;
        buf[1] = (u8)(address >> 16);
        buf[2] = (u8)(address >> 8);
        buf[3] = (u8)(address >> 24);

        for (i = 0; i < (s32)length; i++) {
            buf[4 + i] = _wudWiiRemoteDescriptor[0xE8 + 8 + i];
        }

        _wudPatchOffset = length;
        DEBUGPrint(_wudWiiRemoteDescriptor + 0x8D8, length);

        BTM_VendorSpecificCommand(BT_VSC_NINTENDO_WRITE_PATCH, length + 4,
                                  buf,
                                  (tBTM_VSC_CMPL_CB*)__wudWritePatchCallback);
    } else {
        DEBUGPrint(_wudWiiRemoteDescriptor + 0x8B0);
        BTM_DeviceReset((tBTM_CMPL_CB*)__wudModuleRebootCallback);
    }
}
void __wudSuperPeekPokeCallback(void) {
    extern char lbl_80562BD8[];
    extern char lbl_80562BF0[];

    DEBUGPrint(lbl_80562BD8);
    DEBUGPrint(lbl_80562BF0);

    BTM_VendorSpecificCommand(BT_VSC_NINTENDO_INSTALL_PATCH, 1,
                              &_wudPatchRemoveCmd,
                              __wudRemovePatchCallback);
}

void __wudAppendRuntimePatch(void) {
    u8* pPatch = (u8*)_wudWiiRemoteDescriptor + 0xE8;

    DEBUGPrint(_wudWiiRemoteDescriptor + 0x938);

    _wudPatchAddress = pPatch[3];
    _wudPatchAddress = (_wudPatchAddress << 8) + pPatch[2];
    _wudPatchAddress = (_wudPatchAddress << 8) + pPatch[1];
    _wudPatchAddress = (_wudPatchAddress << 8) + pPatch[0];
    _wudPatchSize = pPatch[7];
    _wudPatchSize = (_wudPatchSize << 8) + pPatch[6];
    _wudPatchSize = (_wudPatchSize << 8) + pPatch[5];
    _wudPatchSize = (_wudPatchSize << 8) + pPatch[4];

    if (__OSInIPL != 0) {
        DEBUGPrint(_wudWiiRemoteDescriptor + 0x954);

        BTM_VendorSpecificCommand(0xFC0A, 9,
                                  (u8*)(_wudWiiRemoteDescriptor + 0xDC),
                                  (tBTM_VSC_CMPL_CB*)__wudSuperPeekPokeCallback);
    } else {
        DEBUGPrint(_wudWiiRemoteDescriptor + 0x928);

        BTM_VendorSpecificCommand(BT_VSC_NINTENDO_INSTALL_PATCH, 1,
                                  &_wudPatchRemoveCmd,
                                  __wudRemovePatchCallback);
    }
}

void __wudInitSub(void) {
    extern char lbl_80562C2C[];
    extern u32 lbl_8066C260;
    extern u16 lbl_8066C264;
    extern u8 lbl_8066C266;
    WUDCB* p = &_wcb;
    char devName[4];
    DEV_CLASS devClass;
    int i;
    BOOL enabled;

    *(u32*)devName = lbl_8066C260;
    *(u16*)&devClass[0] = lbl_8066C264;
    devClass[2] = lbl_8066C266;

    DEBUGPrint(lbl_80562C2C);

    BTA_DmSetDeviceName(devName);
    BTM_SetDeviceClass(devClass);

    BTM_RegisterForVSEvents(__wudVendorSpecificEventStackCallback);
    BTM_RegisterForDeviceStatusNotif(__wudDeviceStatusEventStackCallback);
    BTM_PmRegister(3, &p->pmID, __wudPowerMangeEventStackCallback);
    BTM_WritePageTimeout(0x8000);
    BTM_SetDefaultLinkPolicy(5);
    BTM_SetDefaultLinkSuperTout(0xc80);

    for (i = 0; i < WUD_MAX_DEV_ENTRY_FOR_STD; i++) {
        if (p->stdDevs[i].status == 1) {
            WUDiRegisterDevice(p->stdDevs[i].devAddr);
        }
    }

    for (i = 0; i < WUD_MAX_DEV_ENTRY_FOR_SMP; i++) {
        if (p->smpDevs[i].status == 1) {
            WUDiRegisterDevice(p->smpDevs[i].devAddr);
        }
    }

    enabled = OSDisableInterrupts();
    p->libStatus = WUD_LIB_STATUS_3;
    _wudReadNand = TRUE;
    OSRestoreInterrupts(enabled);

    enabled = OSDisableInterrupts();
    _wcb.discoverable = FALSE;
    _wcb.connectable = TRUE;
    OSRestoreInterrupts(enabled);

    BTA_DmSetVisibility(FALSE, TRUE);
}
void WUDiMoveTopOfUnusedStdDevice(WUDDevInfo* pInfo) {
    WUDCB* p = &_wcb;
    int i;
    BOOL enabled;
    WUDDevInfoList* pIt;

    enabled = OSDisableInterrupts();

    for (i = 0; i < WUD_MAX_DEV_ENTRY_FOR_STD; i++) {
        if (WUD_BDCMP(p->stdList[i].devInfo->devAddr, pInfo->devAddr) != 0) {
            continue;
        }

        for (pIt = p->stdListHead; pIt; pIt = pIt->next) {
            if (WUD_BDCMP(pIt->devInfo->devAddr, pInfo->devAddr) == 0) {
                continue;
            }

            if (pIt->devInfo->status != 0 && pIt != p->stdListTail) {
                continue;
            }

            if (WUD_BDCMP(p->stdListHead->devInfo->devAddr,
                          p->stdList[i].devInfo->devAddr) == 0) {
                if (pIt == p->stdListHead->next) {
                    break;
                }

                p->stdListHead = p->stdList[i].next;
            } else {
                p->stdList[i].prev->next = p->stdList[i].next;
            }

            p->stdList[i].next->prev = p->stdList[i].prev;

            if (pIt == p->stdListTail && pIt->devInfo->status != 0) {
                p->stdList[i].prev = pIt;
                p->stdList[i].next = NULL;

                pIt->next = &p->stdList[i];
                p->stdListTail = &p->stdList[i];
            } else if (pIt != p->stdListHead) {
                p->stdList[i].prev = pIt->prev;
                p->stdList[i].next = pIt;

                pIt->prev->next = &p->stdList[i];
                pIt->prev = &p->stdList[i];
            } else {
                p->stdList[i].prev = pIt;
                p->stdList[i].next = pIt->next;

                pIt->next->prev = &p->stdList[i];
                pIt->next = &p->stdList[i];
            }

            break;
        }
    }

    OSRestoreInterrupts(enabled);
}
void __wudCleanupStackCallback(s32 result) {
    extern char lbl_80562D24[];
    WUDCB* p = &_wcb;

    if (result == 0) {
        _wudInitialized = 0;
        p->libStatus = 0;
    } else {
        DEBUGPrint(lbl_80562D24);
    }
}
void __wudSecurityEventStackCallback(tBTA_DM_SEC_EVT event,
                                     tBTA_DM_SEC* pData) {
    WUDCB* p = &_wcb;
    char* pMsg = _wudWiiRemoteDescriptor;
    WUDDevInfo* pInfo;
    BOOL enabled;
    u8 linkedNum;
    int i;

    DEBUGPrint(pMsg + 0xA7C);

    switch (event) {
    case BTA_DM_ENABLE_EVT: {
        tBTA_DM_ENABLE* pEnable = &pData->enable;

        WUD_BDCPY(p->hostAddr, pEnable->bd_addr);

        DEBUGPrint(pMsg + 0xA98);
        DEBUGPrint(pMsg + 0xAA8, p->hostAddr[0], p->hostAddr[1],
                   p->hostAddr[2], p->hostAddr[3], p->hostAddr[4],
                   p->hostAddr[5]);

        BTA_HhEnable(BTA_SEC_AUTHENTICATE, WUDiHidHostEventCallback);
        p->stackState = WUD_STATE_STACK_GET_STORED_LINK_KEY;

        OSCreateAlarm(&p->alarm);
        OSSetPeriodicAlarm(&p->alarm, OSGetTime(), OS_MSEC_TO_TICKS(10),
                           __wudStackHandler0);
        p->libStatus = WUD_LIB_STATUS_2;
        break;
    }

    case BTA_DM_DISABLE_EVT: {
        DEBUGPrint(pMsg + 0xAD0);
        BTA_CleanUp((BTA_CleanUpCallback)__wudCleanupStackCallback);
        break;
    }

    case BTA_DM_PIN_REQ_EVT: {
        BD_ADDR pin;
        BD_ADDR_PTR pAddr;
        tBTA_DM_PIN_REQ* pPinReq = &pData->pin_req;

        DEBUGPrint(pMsg + 0xAE4);

        pAddr = p->syncType == WUD_SYNC_TYPE_STANDARD ? p->hostAddr
                                                      : pPinReq->bd_addr;

        if (_wudDiscWork.UNK_0x5B == 4) {
            WUDiRemoveDevice(_wudDiscWork.devAddr);
            btm_remove_acl(_wudDiscWork.devAddr);
        }

        _wudDiscWork.UNK_0x5A = 1;

        enabled = OSDisableInterrupts();
        pin[0] = pAddr[BD_ADDR_LEN - 1];
        pin[1] = pAddr[BD_ADDR_LEN - 2];
        pin[2] = pAddr[BD_ADDR_LEN - 3];
        pin[3] = pAddr[BD_ADDR_LEN - 4];
        pin[4] = pAddr[BD_ADDR_LEN - 5];
        pin[5] = pAddr[BD_ADDR_LEN - 6];
        OSRestoreInterrupts(enabled);

        BTA_DmPinReply(pPinReq->bd_addr, TRUE, BD_ADDR_LEN, pin);
        break;
    }

    case BTA_DM_AUTH_CMPL_EVT: {
        tBTA_DM_AUTH_CMPL* pAuthCmpl = &pData->auth_cmpl;

        DEBUGPrint(pMsg + 0xAF8);
        DEBUGPrint(pMsg + 0xB10, pAuthCmpl->bd_addr[0], pAuthCmpl->bd_addr[1],
                   pAuthCmpl->bd_addr[2], pAuthCmpl->bd_addr[3],
                   pAuthCmpl->bd_addr[4], pAuthCmpl->bd_addr[5]);

        // clang-format off
        DEBUGPrint(pMsg + 0xB38,
                   pAuthCmpl->key[0],  pAuthCmpl->key[1],  pAuthCmpl->key[2],  pAuthCmpl->key[3],
                   pAuthCmpl->key[4],  pAuthCmpl->key[5],  pAuthCmpl->key[6],  pAuthCmpl->key[7],
                   pAuthCmpl->key[8],  pAuthCmpl->key[9],  pAuthCmpl->key[10], pAuthCmpl->key[11],
                   pAuthCmpl->key[12], pAuthCmpl->key[13], pAuthCmpl->key[14], pAuthCmpl->key[15]);
        // clang-format on

        DEBUGPrint(pMsg + 0xB94, pAuthCmpl->key_type);

        if (pAuthCmpl->key_type != HCI_LKEY_TYPE_COMBINATION) {
            pInfo = WUDiGetDevInfo(pAuthCmpl->bd_addr);

            if (WUD_BDCMP(_wudDiscWork.devAddr, pAuthCmpl->bd_addr) == 0) {
                _wudDiscWork.status = 0xC;

                if (pInfo == NULL) {
                    pInfo = &_wudDiscWork;
                }
            }

            memcpy(pInfo->linkKey, pAuthCmpl->key, LINK_KEY_LEN);
        }
        break;
    }

    case BTA_DM_AUTHORIZE_EVT: {
        DEBUGPrint(pMsg + 0xBA4);
        break;
    }

    case BTA_DM_LINK_UP_EVT: {
        tBTA_DM_LINK_UP* pLinkUp = &pData->link_up;

        DEBUGPrint(pMsg + 0xBBC);
        DEBUGPrint(pMsg + 0xBD0, pLinkUp->bd_addr[0], pLinkUp->bd_addr[1],
                   pLinkUp->bd_addr[2], pLinkUp->bd_addr[3],
                   pLinkUp->bd_addr[4], pLinkUp->bd_addr[5]);

        pInfo = WUDiGetDevInfo(pLinkUp->bd_addr);
        if (pInfo == NULL &&
            WUD_BDCMP(pLinkUp->bd_addr, _wudDiscWork.devAddr) == 0) {
            pInfo = &_wudDiscWork;
        }

        enabled = OSDisableInterrupts();
        linkedNum = p->linkedNum;
        OSRestoreInterrupts(enabled);

        if (pInfo == NULL || linkedNum == WUD_MAX_CHANNELS) {
            btm_remove_acl(pLinkUp->bd_addr);

            DEBUGPrint(pMsg + 0xBFC,
                       pInfo == NULL ? pMsg + 0xC24 : pMsg + 0xC30,
                       pLinkUp->bd_addr[0], pLinkUp->bd_addr[1],
                       pLinkUp->bd_addr[2], pLinkUp->bd_addr[3],
                       pLinkUp->bd_addr[4], pLinkUp->bd_addr[5]);
            break;
        }

        pInfo->status = pInfo->status == 2 ? 0xC : 3;
        p->linkedNum++;
        break;
    }

    case BTA_DM_LINK_DOWN_EVT: {
        tBTA_DM_LINK_DOWN* pLinkDown = &pData->link_down;

        DEBUGPrint(pMsg + 0xC40);
        DEBUGPrint(pMsg + 0xBD0, pLinkDown->bd_addr[0], pLinkDown->bd_addr[1],
                   pLinkDown->bd_addr[2], pLinkDown->bd_addr[3],
                   pLinkDown->bd_addr[4], pLinkDown->bd_addr[5]);
        DEBUGPrint(pMsg + 0xC58, pLinkDown->status);

        pInfo = WUDiGetDevInfo(pLinkDown->bd_addr);

        if (pInfo != NULL) {
            pInfo->status = 1;
            p->linkedNum--;

            if (WUD_BDCMP(_wudDiscWork.devAddr, pLinkDown->bd_addr) == 0) {
                p->syncState = WUD_STATE_SYNC_ERROR;
            }

            if (pLinkDown->status == HCI_ERR_PEER_POWER_OFF) {
                for (i = 0; i < WUD_MAX_DEV_ENTRY_FOR_SMP; i++) {
                    if (WUD_BDCMP(_scArray.active[i].addr,
                                  pLinkDown->bd_addr) == 0) {

                        memset(&_scArray.active[i], 0,
                               sizeof(SCBtDeviceInfo));
                        _scFlush = TRUE;
                    }
                }
            }
        } else if (WUD_BDCMP(_wudDiscWork.devAddr, pLinkDown->bd_addr) == 0) {
            p->syncState = WUD_STATE_SYNC_ERROR;
            p->linkedNum--;
        } else {
            DEBUGPrint(pMsg + 0xC64);
        }

        if (p->linkedNum >= 0xFA) {
            OSReport(pMsg + 0xC80);
            p->linkedNum = 0;
        }
        break;
    }

    case BTA_DM_SIG_STRENGTH_EVT: {
        DEBUGPrint(pMsg + 0xCA4);
        break;
    }

    case BTA_DM_BUSY_LEVEL_EVT: {
        DEBUGPrint(pMsg + 0xCC0);
        break;
    }
    }
}
void __wudSearchEventStackCallback(tBTA_DM_SEARCH_EVT event,
                                   tBTA_DM_SEARCH* pData) {
    WUDCB* p = &_wcb;
    char* pMsg = _wudWiiRemoteDescriptor;
    s32 timeout;
    BOOL enabled;
    u8 linkedNum;

    switch (event) {
    case BTA_DM_INQ_RES_EVT: {
        tBTA_DM_INQ_RES* pResp = &pData->inq_res;

        // clang-format off
        DEBUGPrint(pMsg + 0xCFC,
                   pResp->bd_addr[0], pResp->bd_addr[1], pResp->bd_addr[2],
                   pResp->bd_addr[3], pResp->bd_addr[4], pResp->bd_addr[5],
                   pResp->dev_class[0], pResp->dev_class[1], pResp->dev_class[2],
                   pResp->rssi);
        // clang-format on

        _wudDiscRssi = pResp->rssi;

        if (p->syncSkipChecks == TRUE ||
            (p->syncSkipChecks == FALSE &&
             WUDiGetLinkNum() < WUD_MAX_CHANNELS - 1)) {

            timeout = 0x1900;
        } else {
            timeout = 0x8000;
        }

        BTM_WritePageTimeout(timeout);
        break;
    }

    case BTA_DM_INQ_CMPL_EVT: {
        DEBUGPrint(pMsg + 0xD40);
        break;
    }

    case BTA_DM_DISC_RES_EVT: {
        tBTA_DM_DISC_RES* dmDiscRes = &pData->disc_res;

        WUD_BDCPY(_wudDiscResp.devAddr, dmDiscRes->bd_addr);
        memcpy(_wudDiscResp.devName, dmDiscRes->bd_name,
               sizeof(_wudDiscResp.devName));

        _wudDiscResp.services = dmDiscRes->services;
        _wudDiscNumResps++;

        // clang-format off
        DEBUGPrint(pMsg + 0xD54,
                   _wudDiscResp.devAddr[0], _wudDiscResp.devAddr[1], _wudDiscResp.devAddr[2], 
                   _wudDiscResp.devAddr[3], _wudDiscResp.devAddr[4], _wudDiscResp.devAddr[5],
                   _wudDiscResp.devName, _wudDiscResp.services);
        // clang-format on
        break;
    }

    case BTA_DM_DISC_CMPL_EVT: {
        DEBUGPrint(pMsg + 0xD94);

        p->syncState = WUD_STATE_SYNC_CHECK_SEARCH_RESULT;
        break;
    }

    case BTA_DM_SEARCH_CANCEL_CMPL_EVT: {
        DEBUGPrint(pMsg + 0xDA8);

        BTM_VendorSpecificCommand(BT_VSC_NINTENDO_WRITE_PATCH, 0x1C,
                                  (u8*)(pMsg + 0x200), NULL);

        _wudDiscNumResps = 0;
        memset(&_wudDiscResp, 0, sizeof(WUDDiscResp));

        p->syncState = WUD_STATE_SYNC_CHECK_SEARCH_RESULT;
        break;
    }

    default: {
        DEBUGPrint(pMsg + 0xDB8);
        break;
    }
    }
}
void __wudVendorSpecificEventStackCallback(UINT8 len, UINT8* pData) {
    WUDCB* p = &_wcb;
    char* pMsg = _wudWiiRemoteDescriptor;
    BOOL enabled;
    u8 event = pData[0];

    switch (event) {
    case WUD_VSE_INITIATE_PAIRING: {
        WUDSyncDeviceCallback pSyncCallback;
        BOOL busy;

        DEBUGPrint(pMsg + 0xDE8);

        enabled = OSDisableInterrupts();

        if (p->syncState == WUD_STATE_SYNC_START &&
            p->deleteState == WUD_STATE_DELETE_START &&
            p->stackState == WUD_STATE_STACK_INITIALIZED &&
            p->initState == 6) {

            OSRestoreInterrupts(enabled);
            busy = FALSE;
        } else {
            OSRestoreInterrupts(enabled);
            busy = TRUE;
        }

        if (busy) {
            break;
        }

        enabled = OSDisableInterrupts();
        pSyncCallback = p->syncStdCB;
        OSRestoreInterrupts(enabled);

        if (pSyncCallback != NULL) {
            pSyncCallback(WUD_RESULT_SYNC_WAITING, 0);
        } else {
            __wudStartSyncDevice(WUD_SYNC_TYPE_STANDARD, 3, 0, FALSE);
        }
        break;
    }

    case WUD_VSE_DELETE_ALL_KEYS: {
        WUDClearDeviceCallback pClearCallback;
        s32 result;
        s8 libStatus;
        BOOL busy;

        DEBUGPrint(pMsg + 0xE00);

        enabled = OSDisableInterrupts();
        pClearCallback = p->clearDevCB;

        if (p->syncState == WUD_STATE_SYNC_START &&
            p->deleteState == WUD_STATE_DELETE_START &&
            p->stackState == WUD_STATE_STACK_INITIALIZED &&
            p->initState == 6) {

            OSRestoreInterrupts(enabled);
            busy = FALSE;
        } else {
            OSRestoreInterrupts(enabled);
            busy = TRUE;
        }

        result = busy ? WUD_RESULT_DELETE_BUSY : WUD_RESULT_DELETE_WAITING;
        OSRestoreInterrupts(enabled);

        if (pClearCallback != NULL) {
            pClearCallback(result);
            break;
        }

        DEBUGPrint(pMsg + 0x800);

        enabled = OSDisableInterrupts();
        libStatus = p->libStatus;
        OSRestoreInterrupts(enabled);

        if (libStatus == WUD_LIB_STATUS_3) {
            BOOL busy2;

            enabled = OSDisableInterrupts();

            if (p->syncState == WUD_STATE_SYNC_START &&
                p->deleteState == WUD_STATE_DELETE_START &&
                p->stackState == WUD_STATE_STACK_INITIALIZED &&
                p->initState == 6) {

                OSRestoreInterrupts(enabled);
                busy2 = FALSE;
            } else {
                OSRestoreInterrupts(enabled);
                busy2 = TRUE;
            }

            if (!busy2) {
                enabled = OSDisableInterrupts();
                p->deleteState = WUD_STATE_DELETE_DISALLOW_INCOMING;

                OSCreateAlarm(&p->alarm);
                OSSetPeriodicAlarm(&p->alarm, OSGetTime(),
                                   OS_MSEC_TO_TICKS(20), __wudDeleteHandler0);

                OSRestoreInterrupts(enabled);
            }
        }
        break;
    }

    case WUD_VSE_SI_PORT_STATUS: {
        p->serialPortStatus = pData[1];
        break;
    }

    case WUD_VSE_WATCH_DOG_RESET_HW: {
        OSPanic(lbl_80665D78_80663448, 0x14DE, pMsg + 0xE18);
        break;
    }
    }
}
void __wudDeviceStatusEventStackCallback(tBTM_DEV_STATUS status) {
    extern char lbl_805630F4[];
    extern char lbl_8056311C[];

    DEBUGPrint(lbl_805630F4);

    if (status == BTM_DEV_STATUS_CMD_TOUT) {
        OSReport(lbl_8056311C);
    }
}

void __wudLinkKeyEventStackCallback(void* p1) {
    WUDCB* p = &_wcb;
    char* pMsg = _wudWiiRemoteDescriptor;
    tBTM_BD_ADDR_LINK_KEY_PAIR* pPair;
    WUDDevInfo* pInfo;
    WUDDevInfo* pNewInfo;
    BOOL enabled;
    u8 numKeys;
    int i;
    int j;

    switch (*(u8*)p1) {
    case BTM_CB_EVT_RETURN_LINK_KEYS: {
        DEBUGPrint(pMsg + 0xE7C);

        pPair = (tBTM_BD_ADDR_LINK_KEY_PAIR*)((u8*)p1 + 2);
        numKeys = ((u8*)p1)[1];

        for (i = 0; i < numKeys; i++) {
            pInfo = WUDiGetDevInfo(pPair->bd_addr);

            // clang-format off
            DEBUGPrint(pMsg + 0xE9C,
                       pPair->bd_addr[0], pPair->bd_addr[1], pPair->bd_addr[2],
                       pPair->bd_addr[3], pPair->bd_addr[4], pPair->bd_addr[5]);

            DEBUGPrint(pMsg + 0xEC8,
                       pPair->link_key[0],  pPair->link_key[1],  pPair->link_key[2],  pPair->link_key[3],
                       pPair->link_key[4],  pPair->link_key[5],  pPair->link_key[6],  pPair->link_key[7],
                       pPair->link_key[8],  pPair->link_key[9],  pPair->link_key[10], pPair->link_key[11],
                       pPair->link_key[12], pPair->link_key[13], pPair->link_key[14], pPair->link_key[15]);
            // clang-format on

            if (pInfo == NULL) {
                DEBUGPrint(pMsg + 0xF24);

                enabled = OSDisableInterrupts();

                if (p->syncType == WUD_SYNC_TYPE_STANDARD) {
                    pNewInfo = NULL;

                    for (j = 0; j < WUD_MAX_DEV_ENTRY_FOR_STD; j++) {
                        if (p->stdDevs[j].status == 0) {
                            DEBUGPrint(pMsg + 0x2CC, j);
                            pNewInfo = &p->stdDevs[j];
                            break;
                        }
                    }
                } else {
                    pNewInfo = p->smpListTail->devInfo;
                }

                OSRestoreInterrupts(enabled);

                if (pNewInfo == NULL) {
                    enabled = OSDisableInterrupts();

                    for (j = 0; j < WUD_MAX_DEV_ENTRY_FOR_SMP; j++) {
                        if (p->smpDevs[j].status == 0) {
                            pNewInfo = &p->smpDevs[j];
                            break;
                        }
                    }

                    OSRestoreInterrupts(enabled);
                }

                if (pNewInfo != NULL) {
                    pNewInfo->UNK_0x5C = 1;
                    pNewInfo->status = 1;

                    WUD_BDCPY(pNewInfo->devAddr, pPair->bd_addr);
                    memcpy(pNewInfo->linkKey, pPair->link_key, LINK_KEY_LEN);
                }
            } else {
                WUD_BDCPY(pInfo->devAddr, pPair->bd_addr);
                memcpy(pInfo->linkKey, pPair->link_key, LINK_KEY_LEN);

                if (pInfo->UNK_0x5C == 2) {
                    pInfo->UNK_0x5C = 3;
                }

                WUD_BDCPY(p->pairAddr, pPair->bd_addr);
                DEBUGPrint(pMsg + 0xF48, p->pairAddr[0], p->pairAddr[1],
                           p->pairAddr[2], p->pairAddr[3], p->pairAddr[4],
                           p->pairAddr[5]);
            }

            pPair++;
        }

        break;
    }

    case BTM_CB_EVT_READ_STORED_LINK_KEYS: {
        tBTM_READ_STORED_LINK_KEY_COMPLETE* pRead = p1;

        DEBUGPrint(pMsg + 0xF70);
        DEBUGPrint(pMsg + 0xF94, pRead->status, pRead->max_keys,
                   pRead->read_keys);

        p->linkKeyState = WUD_STATE_LINK_KEY_START;
        break;
    }

    case BTM_CB_EVT_WRITE_STORED_LINK_KEYS: {
        tBTM_WRITE_STORED_LINK_KEY_COMPLETE* pWrite = p1;

        DEBUGPrint(pMsg + 0xFC0);
        DEBUGPrint(pMsg + 0xFE4, pWrite->status, pWrite->num_keys);

        p->linkKeyState = WUD_STATE_LINK_KEY_START;
        break;
    }

    case BTM_CB_EVT_DELETE_STORED_LINK_KEYS: {
        tBTM_DELETE_STORED_LINK_KEY_COMPLETE* pDelete = p1;

        DEBUGPrint(pMsg + 0x1000);
        DEBUGPrint(pMsg + 0xFE4, pDelete->status, pDelete->num_keys);

        if (pDelete->num_keys == 0) {
            DEBUGPrint(pMsg + 0x1024);
        }

        p->linkKeyState = WUD_STATE_LINK_KEY_START;
        break;
    }

    default: {
        OSPanic(lbl_80665D78_80663448, 0x157A, pMsg + 0x1044);
        break;
    }
    }
}

void __wudPowerMangeEventStackCallback(BD_ADDR addr, tBTM_PM_STATUS status,
                                       UINT16 value, UINT8 hciStatus) {
#pragma unused(value)

    extern char _wudWiiRemoteDescriptor[];
    char* pMsg = _wudWiiRemoteDescriptor;
    WUDDevInfo* pInfo;

    DEBUGPrint(pMsg + 0x1054);
    DEBUGPrint(pMsg + 0x1078, hciStatus);

    pInfo = WUDiGetDevInfo(addr);
    if (pInfo == NULL) {
        pInfo = &_wudDiscWork;

        if (memcmp(pInfo->devAddr, addr, sizeof(BD_ADDR)) != 0) {
            DEBUGPrint(pMsg + 0x108c);

            DEBUGPrint(pMsg + 0x10d0, addr[0], addr[1], addr[2], addr[3],
                       addr[4], addr[5], status);
            return;
        }
    }

    switch (status) {
    case BTM_PM_STS_ACTIVE:
        pInfo->status = 8;
        break;

    case BTM_PM_STS_SNIFF:
        pInfo->status = 9;
        break;
    }

    DEBUGPrint(pMsg + 0x10d0, pInfo->devAddr[0], pInfo->devAddr[1],
               pInfo->devAddr[2], pInfo->devAddr[3], pInfo->devAddr[4],
               pInfo->devAddr[5], pInfo->status);
}

WUDDevInfo* WUDiGetDiscoverDevice(void) { return &_wudDiscWork; }

void WUDSetDeviceHistory(u32 idx, BD_ADDR_PTR addr) {
    if (addr == NULL) {
        memset(&_scArray.regist[idx + 10], 0, sizeof(SCBtDeviceInfo));
    } else {
        memcpy(&_scArray.regist[idx + 10].addr, addr, sizeof(BD_ADDR));
    }

    _scFlush = 1;
}

BOOL WUDIsLatestDevice(u32 idx, BD_ADDR_PTR addr) {
    if (addr == NULL) {
        return FALSE;
    }

    return memcmp(&_scArray.regist[idx + 10].addr, addr, sizeof(BD_ADDR)) == 0;
}
void WUDUpdateSCSetting(void) {
    if (_scFlush) {
        if (SCSetBtDeviceInfoArray(&_scArray)) {
            SCFlushAsync(NULL);
            _scFlush = FALSE;
        }
    }
}
void WUDiSetDevAddrForHandle(u8 handle, BD_ADDR_PTR addr) { _dev_handle_to_bda[handle] = addr; }
BD_ADDR_PTR WUDiGetDevAddrForHandle(u8 handle) {
    return _dev_handle_to_bda[handle & 0xFF];
}
void WUDiSetQueueSizeForHandle(u8 handle, u16 size) {
    _dev_handle_queue_size[handle] = size;
}
void WUDiSetNotAckNumForHandle(u8 handle, u16 notAckNum) {
    _dev_handle_notack_num[handle] = notAckNum;
}
