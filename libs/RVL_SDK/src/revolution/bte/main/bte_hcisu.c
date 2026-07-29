#include <types.h>
#include "revolution/bte/gki/common/gki.h"
#include "revolution/bte/stack/include/btu.h"
#include "revolution/bte/main/bte_hcisu.h"

tHCI_IF *p_hcisu_if;
tHCI_CFG *p_hcisu_cfg;

void bte_hcisu_send(HC_BT_HDR *p_msg, UINT16 event)
{
    p_msg->event = event;

    if (p_hcisu_if)
        p_hcisu_if->send(p_msg);
    else
        GKI_freebuf(p_msg);
}

void bte_hcisu_task(void)
{
    if (p_hcisu_if)
    {
        if (p_hcisu_if->init)
            p_hcisu_if->init(2, 1, 0x800);

        if (p_hcisu_if->open)
            p_hcisu_if->open(p_hcisu_cfg);
    }
}

void bte_hcisu_close(void)
{
    if (p_hcisu_if)
    {
        if (p_hcisu_if->close)
            p_hcisu_if->close();
    }
}

void bta_ci_hci_msg_handler(void *p_data)
{
    GKI_send_msg(BTU_TASK, BTU_HCI_RCV_MBOX, p_data);
}
