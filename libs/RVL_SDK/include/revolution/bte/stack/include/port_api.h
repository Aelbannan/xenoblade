/******************************************************************************
 *
 *  Copyright (C) 1999-2012 Broadcom Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

/******************************************************************************
 *
 *  this file contains the PORT API definitions
 *
 ******************************************************************************/
#ifndef PORT_API_H
#define PORT_API_H

#include "revolution/BTE/include/bt_target.h"

/*****************************************************************************
**  Constants and Types
*****************************************************************************/

/*
** Define port settings structure send from the application in the
** set settings request, or to the application in the set settings indication.
*/
typedef struct
{
    UINT8   baud_rate;
    UINT8   byte_size;
    UINT8   stop_bits;
    UINT8   parity;
    UINT8   parity_type;
    UINT8   fc_type;
    UINT8   rx_char1;
    UINT8   xon_char;
    UINT8   xoff_char;
} tPORT_STATE;

/*
** Define events that registered application can receive in the callback
*/

#define PORT_EV_RXCHAR  0x00000001   /* Any Character received */
#define PORT_EV_RXFLAG  0x00000002   /* Received certain character */
#define PORT_EV_TXEMPTY 0x00000004   /* Transmitt Queue Empty */
#define PORT_EV_CTS     0x00000008   /* CTS changed state */
#define PORT_EV_DSR     0x00000010   /* DSR changed state */
#define PORT_EV_RLSD    0x00000020   /* RLSD changed state */
#define PORT_EV_BREAK   0x00000040   /* BREAK received */
#define PORT_EV_ERR     0x00000080   /* Line status error occurred */
#define PORT_EV_RING    0x00000100   /* Ring signal detected */
#define PORT_EV_CTSS    0x00000400   /* CTS state */
#define PORT_EV_DSRS    0x00000800   /* DSR state */
#define PORT_EV_RLSDS   0x00001000   /* RLSD state */
#define PORT_EV_OVERRUN 0x00002000   /* receiver buffer overrun */
#define PORT_EV_TXCHAR  0x00004000   /* Any character transmitted */

#define PORT_EV_CONNECTED    0x00000200  /* RFCOMM connection established */
#define PORT_EV_CONNECT_ERR  0x00008000  /* Was not able to establish connection */
                                     /* or disconnected */
#define PORT_EV_FC      0x00010000   /* data flow enabled flag changed by remote */
#define PORT_EV_FCS     0x00020000   /* data flow enable status true = enabled */

/*
** Define the callback function prototypes.  Parameters are specific
** to each event and are described bellow
*/
typedef int  (tPORT_DATA_CALLBACK) (UINT16 port_handle, void *p_data, UINT16 len);

typedef void (tPORT_CALLBACK) (UINT32 code, UINT16 port_handle);

/*
** Define PORT result codes
*/
#define PORT_SUCCESS                0

#define PORT_ERR_BASE               0

#define PORT_UNKNOWN_ERROR          (PORT_ERR_BASE + 1)
#define PORT_ALREADY_OPENED         (PORT_ERR_BASE + 2)
#define PORT_CMD_PENDING            (PORT_ERR_BASE + 3)
#define PORT_APP_NOT_REGISTERED     (PORT_ERR_BASE + 4)
#define PORT_NO_MEM                 (PORT_ERR_BASE + 5)
#define PORT_NO_RESOURCES           (PORT_ERR_BASE + 6)
#define PORT_BAD_BD_ADDR            (PORT_ERR_BASE + 7)
#define PORT_BAD_HANDLE             (PORT_ERR_BASE + 9)
#define PORT_NOT_OPENED             (PORT_ERR_BASE + 10)
#define PORT_LINE_ERR               (PORT_ERR_BASE + 11)
#define PORT_START_FAILED           (PORT_ERR_BASE + 12)
#define PORT_PAR_NEG_FAILED         (PORT_ERR_BASE + 13)
#define PORT_PORT_NEG_FAILED        (PORT_ERR_BASE + 14)
#define PORT_SEC_FAILED             (PORT_ERR_BASE + 15)
#define PORT_PEER_CONNECTION_FAILED (PORT_ERR_BASE + 16)
#define PORT_PEER_FAILED            (PORT_ERR_BASE + 17)
#define PORT_PEER_TIMEOUT           (PORT_ERR_BASE + 18)
#define PORT_CLOSED                 (PORT_ERR_BASE + 19)
#define PORT_TX_FULL                (PORT_ERR_BASE + 20)
#define PORT_LOCAL_CLOSED           (PORT_ERR_BASE + 21)
#define PORT_LOCAL_TIMEOUT          (PORT_ERR_BASE + 22)
#define PORT_TX_QUEUE_DISABLED      (PORT_ERR_BASE + 23)
#define PORT_PAGE_TIMEOUT           (PORT_ERR_BASE + 24)
#define PORT_INVALID_SCN            (PORT_ERR_BASE + 25)

#endif  /* PORT_API_H */
