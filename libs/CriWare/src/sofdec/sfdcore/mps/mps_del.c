#include <harness_catalog.h>

int MPS_CheckDelim(const u8 *buf) {
    if (buf[0] != 0 || buf[1] != 0 || buf[2] != 1)
        return 0;

    switch (buf[3]) {
        case 0xB9: return 0x00080000;
        case 0xBA: return 0x00010000;
        case 0xBB: return 0x00020000;
        default:
            if (buf[3] >= 0xBC)
                return 0x00040000;
            return 0;
    }
}
