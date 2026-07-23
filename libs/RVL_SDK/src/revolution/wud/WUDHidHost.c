#include <revolution/WUD.h>

#include <string.h>

#define DEBUGPrint WUD_DEBUGPrint

// TODO(kiwi) Is this from BTA?
#define WUD_HH_CUSTOM_EVT 15

typedef struct WUD_HH_EVT15 {
    u16 status0;   // at 0x0
    u16 status1;   // at 0x2
    u16 linkedNum; // at 0x4

    struct {
        UINT8 handle;     // at 0x0
        UINT16 queueSize; // at 0x2
        UINT16 notAckNum; // at 0x4
    } devices[];          // at 0x6
} WUD_HH_EVT15;

void WUDiHidHostEventCallback(tBTA_HH_EVT event, tBTA_HH* pData) {
    WUDCB* p = &_wcb;
    WUDDevInfo* pInfo;
    tBTA_HH_CONN* pConn;
    tBTA_HH_CBDATA* pCbData;
    WUD_HH_EVT15* pEvt15;
    int i;

    switch (event) {
    case BTA_HH_ENABLE_EVT: {
        DEBUGPrint("BTA_HH_ENABLE_EVT\n");

        p->hhFlags |= 1;
        break;
    }

    case BTA_HH_DISABLE_EVT: {
        DEBUGPrint("BTA_HH_DISABLE_EVT\n");
        break;
    }

    case BTA_HH_OPEN_EVT: {
        pConn = &pData->conn;

        DEBUGPrint("BTA_HH_OPEN_EVT\n");

        DEBUGPrint("handle: %d, addr: %02x:%02x:%02x:%02x:%02x:%02x\n",
                   pConn->handle, pConn->bda[0], pConn->bda[1], pConn->bda[2],
                   pConn->bda[3], pConn->bda[4], pConn->bda[5]);

        if (pConn->status == BTA_HH_OK) {
            pInfo = WUDiGetDiscoverDevice();

            if (memcmp(pInfo->devAddr, pConn->bda, BD_ADDR_LEN) != 0) {
                pInfo = WUDiGetDevInfo(pConn->bda);
            }

            switch (pInfo->status) {
            case 12: {
                p->syncState = WUD_STATE_SYNC_REGISTER_DEVICE;
                break;
            }

            case 2: {
                p->syncState = WUD_STATE_SYNC_COMPLETE;
                break;
            }
            }

            pInfo->status = 8;
            pInfo->devHandle = pConn->handle;
            p->connectedNum++;

            pInfo = WUDiGetDevInfo(pConn->bda);
            if (pInfo == NULL) {
                pInfo = WUDiGetDiscoverDevice();
            }

            WUDiSetDevAddrForHandle(pConn->handle, pInfo->devAddr);
            WUDiSetQueueSizeForHandle(pConn->handle, 0);
            WUDiSetNotAckNumForHandle(pConn->handle, 0);

            if (pInfo->UNK_0x5B == 3 || pInfo->UNK_0x5B == 1) {
                WUDiMoveTopSmpDevInfoPtr(pInfo);
            } else if (!WUDIsLinkedWBC() ||
                       (WUDIsLinkedWBC() &&
                        memcmp(pInfo, "Nintendo RVL-CNT", 16) == 0)) {
                WUDiMoveTopStdDevInfoPtr(pInfo);
            }

            WUDSetSniffMode(pInfo->devAddr, 8);

            if (p->hidConnCB != NULL) {
                p->hidConnCB(pInfo, TRUE);
            }
        } else {
            DEBUGPrint("error code: %d\n", pConn->status);

            if (p->syncState != WUD_STATE_SYNC_START) {
                pInfo = WUDiGetDiscoverDevice();

                if (memcmp(pConn->bda, pInfo->devAddr, BD_ADDR_LEN) == 0 &&
                    pInfo->status == 2) {
                    if (WUDiGetDevInfo(pConn->bda) != NULL &&
                        pConn->status == 12) {
                        WUDiRemoveDevice(pConn->bda);
                        p->linkedNum--;
                    }

                    p->syncState = WUD_STATE_SYNC_ERROR;
                }
            } else if (WUDiGetDevInfo(pConn->bda) != NULL &&
                       pConn->status == 12) {
                pInfo = WUDiGetDevInfo(pConn->bda);
                if (pInfo != NULL) {
                    if (pInfo->UNK_0x5B == 3 || pInfo->UNK_0x5B == 1) {
                        WUDiMoveBottomSmpDevInfoPtr(pInfo);
                    } else {
                        WUDiMoveBottomStdDevInfoPtr(pInfo);
                    }
                }

                WUDiRemoveDevice(pConn->bda);
                p->linkedNum--;
            }
        }

        break;
    }

    case BTA_HH_CLOSE_EVT: {
        pCbData = &pData->dev_status;

        DEBUGPrint("BTA_HH_CLOSE_EVT\n");

        p->connectedNum--;

        DEBUGPrint("device handle : %d   status = %d\n", pCbData->handle,
                   pCbData->status);

        pInfo = WUDiGetDevInfo(WUDiGetDevAddrForHandle(pCbData->handle));
        if (pInfo != NULL) {
            if (pInfo->UNK_0x5B == 3 || pInfo->UNK_0x5B == 1) {
                WUDiMoveTopOfDisconnectedSmpDevice(pInfo);
            } else if (!WUDIsLinkedWBC() ||
                       (WUDIsLinkedWBC() &&
                        memcmp(pInfo, "Nintendo RVL-CNT", 16) == 0)) {
                WUDiMoveTopOfDisconnectedStdDevice(pInfo);
            }
        }

        WUDiSetDevAddrForHandle(pCbData->handle, NULL);
        WUDiSetQueueSizeForHandle(pCbData->handle, 0);
        WUDiSetNotAckNumForHandle(pCbData->handle, 0);

        if (p->hidConnCB != NULL) {
            p->hidConnCB(pInfo, FALSE);
        }
        break;
    }

    case BTA_HH_ADD_DEV_EVT: {
        pConn = &pData->dev_info;

        DEBUGPrint("BTA_HH_ADD_DEV_EVT\n");
        DEBUGPrint("result: %d, handle: %d, addr: %02x:%02x:%02x:%02x:%02x:%02x\n",
                   pConn->status, pConn->handle, pConn->bda[0], pConn->bda[1],
                   pConn->bda[2], pConn->bda[3], pConn->bda[4], pConn->bda[5]);

        pInfo = WUDiGetDevInfo(pConn->bda);
        pInfo->devHandle = pConn->handle;
        WUDiSetDevAddrForHandle(pConn->handle, pInfo->devAddr);
        WUDiSetQueueSizeForHandle(pConn->handle, 0);
        WUDiSetNotAckNumForHandle(pConn->handle, 0);
        break;
    }

    case BTA_HH_RMV_DEV_EVT: {
        pConn = &pData->dev_info;

        DEBUGPrint("BTA_HH_RMV_DEV_EVT\n");
        DEBUGPrint("result: %d, handle: %d, addr: %02x:%02x:%02x:%02x:%02x:%02x\n",
                   pConn->status, pConn->handle, pConn->bda[0], pConn->bda[1],
                   pConn->bda[2], pConn->bda[3], pConn->bda[4], pConn->bda[5]);
        break;
    }

    case WUD_HH_CUSTOM_EVT: {
        pEvt15 = (WUD_HH_EVT15*)pData;

        p->bufferStatus0 = pEvt15->status0;
        p->bufferStatus1 = pEvt15->status1;

        if (p->linkedNum < pEvt15->linkedNum) {
            OSReport("WARNING: link num count is modified.\n");
            p->linkedNum = pEvt15->linkedNum;
        }

        for (i = 0; i < pEvt15->linkedNum; i++) {
            if (pEvt15->devices[i].handle < WUD_MAX_DEV_ENTRY) {
                WUDiSetQueueSizeForHandle(pEvt15->devices[i].handle,
                                          pEvt15->devices[i].queueSize);
                WUDiSetNotAckNumForHandle(pEvt15->devices[i].handle,
                                          pEvt15->devices[i].notAckNum);
            }
        }

        break;
    }

    case BTA_HH_SET_RPT_EVT: {
        DEBUGPrint("BTA_HH_SET_RPT_EVT\n");
        break;
    }

    case BTA_HH_GET_RPT_EVT: {
        DEBUGPrint("BTA_HH_GET_RPT_EVT\n");
        break;
    }

    case BTA_HH_SET_PROTO_EVT: {
        DEBUGPrint("BTA_HH_SET_PROTO_EVT\n");
        break;
    }

    case BTA_HH_GET_PROTO_EVT: {
        DEBUGPrint("BTA_HH_GET_PROTO_EVT\n");
        break;
    }

    case BTA_HH_SET_IDLE_EVT: {
        DEBUGPrint("BTA_HH_SET_IDLE_EVT\n");
        break;
    }

    case BTA_HH_GET_IDLE_EVT: {
        DEBUGPrint("BTA_HH_GET_IDLE_EVT\n");
        break;
    }

    case BTA_HH_GET_DSCP_EVT: {
        DEBUGPrint("BTA_HH_GET_DCSP_EVT\n");
        break;
    }

    case BTA_HH_VC_UNPLUG_EVT: {
        DEBUGPrint("BTA_HH_VS_UNPLUG_EVT\n");
        break;
    }
    }
}

/* After callback string pool + jumptable; sizes include retail pad. */
static char s_invalidAppId[24] = "Invalid app_id [%d]\n";
static char s_btaHhCoOpen[20] = "bta_hh_co_open()\n";
static char s_btaHhCoClose[20] = "bta_hh_co_close()\n";

void bta_hh_co_data(UINT8 handle, UINT8* pReport, UINT16 len,
                    tBTA_HH_PROTO_MODE mode, UINT8 subClass, UINT8 appId) {

#pragma unused(mode)
#pragma unused(subClass)

    WUDCB* p = &_wcb;

    if (appId == 3) {
        if (p->hidRecvCB != NULL) {
            p->hidRecvCB(handle, pReport, len);
        }
    } else {
        DEBUGPrint(s_invalidAppId, appId);
    }
}

void bta_hh_co_open(UINT8 handle, UINT8 subClass, UINT16 attrMask,
                    UINT8 appId) {

#pragma unused(handle)
#pragma unused(subClass)
#pragma unused(attrMask)
#pragma unused(appId)

    DEBUGPrint(s_btaHhCoOpen);
}

void bta_hh_co_close(UINT8 handle, UINT8 appId) {
#pragma unused(handle)
#pragma unused(appId)

    DEBUGPrint(s_btaHhCoClose);
}

BOOLEAN
bta_dm_co_get_compress_memory(tBTA_SYS_ID id, UINT8** ppMemory,
                              UINT32* memorySize) {

#pragma unused(id)
#pragma unused(ppMemory)
#pragma unused(memorySize)

    return FALSE;
}
