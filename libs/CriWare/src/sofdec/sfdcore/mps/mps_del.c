#include <harness_catalog.h>

/* 17.6 goto-gate: retail lays out a linear beq dispatch (cmpi B9/BA/BB) with
 * all five return blocks outlined after it plus a trailing `b` to the
 * >=0xBC check. Plain if/else-if inlines the returns (bne chain) and a
 * switch binary-searches the 3 consecutive cases (BA pivot) - neither
 * reaches FULL_MATCH. Gotos for multi-exit control-flow gates are OK. */
int MPS_CheckDelim(const u8 *buf) {
    int v;
    if (buf[0] != 0) goto ret0;
    if (buf[1] != 0) goto ret0;
    if (buf[2] != 1) goto ret0;
    v = buf[3];
    if (v == 0xB9) goto retB9;
    if (v == 0xBA) goto retBA;
    if (v != 0xBB) goto checkBC;
    goto retBB;
retB9:
    return 0x00080000;
retBA:
    return 0x00010000;
retBB:
    return 0x00020000;
checkBC:
    if ((u32)v >= 0xBC) {
        return 0x00040000;
    }
ret0:
    return 0;
}
