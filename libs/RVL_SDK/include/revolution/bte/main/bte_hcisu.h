#ifndef BTE_HCISU_H
#define BTE_HCISU_H

#include "revolution/BTE/stack/include/bt_types.h"

/* HC_BT_HDR - HCI buffer header (size 0x08) */
typedef struct {
    UINT16 event;
    UINT16 len;
    UINT16 offset;
    UINT16 layer_specific;
} HC_BT_HDR;

/* tHCI_CFG - HCI configuration (size 0x04) */
typedef struct {
    UINT16 field_0x00;
    UINT16 field_0x02;
} tHCI_CFG;

/* HCI interface function types */
typedef void tHCI_INIT(UINT8, UINT8, UINT16);
typedef BOOLEAN tHCI_OPEN(tHCI_CFG *);
typedef void tHCI_CLOSE(void);
typedef BOOLEAN tHCI_SEND(HC_BT_HDR *);
typedef void tHCI_HANDLE_EVENT(UINT16);

/* tHCI_IF - HCI interface (size 0x14) */
typedef struct {
    tHCI_INIT           *init;
    tHCI_OPEN           *open;
    tHCI_CLOSE          *close;
    tHCI_SEND           *send;
    tHCI_HANDLE_EVENT   *handle_event;
} tHCI_IF;

extern tHCI_IF *p_hcisu_if;
extern tHCI_CFG *p_hcisu_cfg;

#endif /* BTE_HCISU_H */
