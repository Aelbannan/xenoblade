// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/bte/stack/rfcomm/port_api
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <string.h>

typedef struct RfcMuxChannel RfcMuxChannel;
typedef struct RfcControlBlock RfcControlBlock;

struct RfcMuxChannel {
    u8 pad_0x00[0x62];
    u8 bd_addr[6];
    u16 lcid;
    u16 field_0x6a;
    u8 state;
    u8 initiator;
    u8 field_0x6e;
    u8 field_0x6f;
    u8 field_0x70;
    u8 field_0x71;
};

struct RfcControlBlock {
    u8 dlci;
    u8 pad_0x01[1];
    u8 field_0x02;
    u8 pad_0x03[1];
    u8 field_0x04;
    u8 credit_based;
    u8 pad_0x06[0x36];
    RfcMuxChannel* mcb[10];
    u8 pad_0x64[1];
    u8 field_0x65;
    u8 pad_0x66[0x3ae];
    u8 trace_level;
};

extern RfcControlBlock rfc_cb;
extern void rfcomm_l2cap_if_init(void);

void RFCOMM_Init(void) {
    memset(&rfc_cb, 0, sizeof(rfc_cb));
    rfc_cb.field_0x65 = 1;
    rfc_cb.trace_level = 5;
    rfcomm_l2cap_if_init();
}
