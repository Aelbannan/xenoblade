// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/bte/hci/hcisu_h2
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void hcisu_h2_usb_cback() {}

void hcisu_h2_receive_msg() {}

void hcisu_h2_send_msg_now() {}

extern unsigned char hcisu_h2_cb[];

void hcisu_h2_init(unsigned char arg0, unsigned char arg1, unsigned short arg2) {
    unsigned char *base = hcisu_h2_cb;
    base[0x1e] = 0;
    base[0x1a] = 0;
    base[0x1b] = 0;
    base[0x1c] = 0;
    base[0x1f] = arg0;
    base[0x20] = arg1;
    *((unsigned short *)(base + 0x12)) = arg2;
}

void hcisu_h2_open() {}

void hcisu_h2_close() {
    hcisu_h2_cb[0x1e] = 0;
    UUSB_Close();
    UUSB_Unregister();
}

void hcisu_h2_send() {}

void hcisu_h2_handle_event() {}
