// CriWare Sofdec MPEG-2 VLC table initialization.
// All tables live in .bss and are filled at MPVVLC_Init time.
// Entries pack MPEG-2 VLC code info: (run << 8) | level-style codes for
// mb_type / mb_addr_inc / cbp / dc_size / run-level tables.

#include <harness_catalog.h>

extern u16 lbl_eu_80602FF8[];
extern u8 lbl_eu_80603400[];
extern u8 lbl_eu_80603488[];
extern u8 lbl_eu_80603510[];
extern u16 lbl_eu_80603618[];
extern u16 lbl_eu_80603660[];
extern u16 lbl_eu_806036A8[];
extern u16 lbl_eu_80603728[];
extern u16 lbl_eu_80603928[];
extern u16 lbl_eu_806039A8[];
extern u16 lbl_eu_80603AA8[];
extern u16 lbl_eu_80603AE8[];
extern u16 lbl_eu_80603BE8[];
extern u8 lbl_eu_80603C28[];
extern u8 lbl_eu_80604028[];
extern u32 lbl_eu_80604428[];
extern u32 lbl_eu_8051C2B8[];

extern int UTY_MemcpyDword(u32 *dst, const u32 *src, u32 n);

int MPVVLC_IsVlcSizErr(void) { return 0x0; }

void MPVVLC_Init() {}

/* P-picture macroblock type VLC table (32 entries) */
void mpvvlc_InitMbTypePpic(void) {
    u16 *p = lbl_eu_80603660;
    int i;

    *p++ = 0x1106;
    *p++ = 0x1205;
    *p++ = 0x1a05;
    *p++ = 0x0105;
    for (i = 0; i < 4; i++)
        *p++ = 0x0803;
    for (i = 0; i < 8; i++)
        *p++ = 0x0202;
    for (i = 0; i < 16; i++)
        *p++ = 0x0a01;
}

/* B-picture macroblock type VLC table (48 entries) */
void mpvvlc_InitMbTypeBpic(void) {
    u16 *p = lbl_eu_806036A8;
    int i;

    *p++ = 0x1f00;
    *p++ = 0x1106;
    *p++ = 0x1606;
    *p++ = 0x1a06;
    *p++ = 0x1e05;
    *p++ = 0x1e05;
    *p++ = 0x0105;
    *p++ = 0x0105;
    for (i = 0; i < 4; i++)
        *p++ = 0x0804;
    for (i = 0; i < 4; i++)
        *p++ = 0x0a04;
    for (i = 0; i < 8; i++)
        *p++ = 0x0403;
    for (i = 0; i < 8; i++)
        *p++ = 0x0603;
    for (i = 0; i < 16; i++)
        *p++ = 0x0c02;
    for (i = 0; i < 16; i++)
        *p++ = 0x0e02;
}

/* Coded-block-pattern sub-table 1 (luma pattern code table) */
u16 *mpvvlc_InitCbpSub1(u16 *tbl) {
    s16 *p = (s16 *)tbl;

    *p++ = 0;
    *p++ = 0;
    *p++ = -0x18f7;
    *p++ = -0x24f7;
    *p++ = -0x04f7;
    *p++ = -0x08f7;
    *p++ = -0x10f7;
    *p++ = -0x20f7;
    *p++ = -0x45f8;
    *p++ = -0x45f8;
    *p++ = -0x49f8;
    *p++ = -0x49f8;
    *p++ = -0x51f8;
    *p++ = -0x51f8;
    *p++ = -0x61f8;
    *p++ = -0x61f8;
    *p++ = 0x7908;
    *p++ = 0x7908;
    *p++ = 0x7508;
    *p++ = 0x7508;
    *p++ = 0x6d08;
    *p++ = 0x6d08;
    *p++ = 0x5d08;
    *p++ = 0x5d08;
    *p++ = -0x59f8;
    *p++ = -0x59f8;
    *p++ = -0x65f8;
    *p++ = -0x65f8;
    *p++ = 0x6508;
    *p++ = 0x6508;
    *p++ = 0x5908;
    *p++ = 0x5908;
    *p++ = -0x14f8;
    *p++ = -0x14f8;
    *p++ = -0x28f8;
    *p++ = -0x28f8;
    *p++ = -0x0cf8;
    *p++ = -0x0cf8;
    *p++ = -0x30f8;
    *p++ = -0x30f8;
    *p++ = -0x55f8;
    *p++ = -0x55f8;
    *p++ = -0x69f8;
    *p++ = -0x69f8;
    *p++ = -0x4df8;
    *p++ = -0x4df8;
    *p++ = -0x71f8;
    *p++ = -0x71f8;
    *p++ = 0x6908;
    *p++ = 0x6908;
    *p++ = 0x5508;
    *p++ = 0x5508;
    *p++ = 0x7108;
    *p++ = 0x7108;
    *p++ = 0x4d08;
    *p++ = 0x4d08;
    *p++ = -0x1cf8;
    *p++ = -0x1cf8;
    *p++ = -0x2cf8;
    *p++ = -0x2cf8;
    *p++ = -0x34f8;
    *p++ = -0x34f8;
    *p++ = -0x38f8;
    *p++ = -0x38f8;
    return tbl + 0x40;
}

/* Coded-block-pattern sub-table 2 (chroma pattern code tables) */
u16 *mpvvlc_InitCbpSub2(u16 *tbl) {
    s16 *p = (s16 *)tbl;
    s16 *q = (s16 *)(tbl + 0x50);
    int i, j;

    for (i = 0; i < 4; i++) *p++ = -0x5df9;
    for (i = 0; i < 4; i++) *p++ = -0x6df9;
    for (i = 0; i < 4; i++) *p++ = -0x75f9;
    for (i = 0; i < 4; i++) *p++ = -0x79f9;
    for (i = 0; i < 4; i++) *p++ = 0x6107;
    for (i = 0; i < 4; i++) *p++ = 0x5107;
    for (i = 0; i < 4; i++) *p++ = 0x4907;
    for (i = 0; i < 4; i++) *p++ = 0x4507;
    for (i = 0; i < 8; i++) *p++ = -0x00fa;
    for (i = 0; i < 8; i++) *p++ = -0x3cfa;
    for (i = 0; i < 8; i++) *p++ = 0x2406;
    for (i = 0; i < 8; i++) *p++ = 0x1806;
    for (i = 0; i < 16; i++) *p++ = -0x41fb;

    for (i = 0; i < 16; i++) *q++ = -0x7dfb;
    for (i = 0; i < 16; i++) *q++ = 0x7d05;
    for (i = 0; i < 16; i++) *q++ = 0x4105;
    for (i = 0; i < 16; i++) *q++ = 0x3805;
    for (i = 0; i < 16; i++) *q++ = 0x3405;
    for (i = 0; i < 16; i++) *q++ = 0x2c05;
    for (i = 0; i < 16; i++) *q++ = 0x1c05;
    for (i = 0; i < 16; i++) *q++ = 0x2805;
    for (i = 0; i < 16; i++) *q++ = 0x1405;
    for (i = 0; i < 16; i++) *q++ = 0x3005;
    for (i = 0; i < 16; i++) *q++ = 0x0c05;
    for (i = 0; i < 32; i++) *q++ = 0x2004;
    for (i = 0; i < 32; i++) *q++ = 0x1004;
    for (i = 0; i < 32; i++) *q++ = 0x0804;
    for (i = 0; i < 32; i++) *q++ = 0x0404;
    for (i = 0; i < 64; i++)
        *q++ = 0x3c03;

    return (u16 *)q;
}

/* Luma DC size table */
void mpvvlc_InitDcSizY(void) {
    u8 *p = lbl_eu_80603400;
    int i;

    for (i = 0; i < 32; i++) *p++ = 0x12;
    for (i = 0; i < 32; i++) *p++ = 0x22;
    for (i = 0; i < 16; i++) *p++ = 0x03;
    for (i = 0; i < 16; i++) *p++ = 0x33;
    for (i = 0; i < 16; i++) *p++ = 0x43;
    for (i = 0; i < 8; i++) *p++ = 0x54;
    for (i = 0; i < 4; i++) *p++ = 0x65;
    for (i = 0; i < 2; i++) *p++ = 0x76;
    for (i = 0; i < 2; i++) *p++ = 0x87;
}

/* Chroma DC size table */
void mpvvlc_InitDcSizC(void) {
    u8 *p = lbl_eu_80603488;
    int i;

    for (i = 0; i < 32; i++) *p++ = 0x02;
    for (i = 0; i < 32; i++) *p++ = 0x12;
    for (i = 0; i < 32; i++) *p++ = 0x22;
    for (i = 0; i < 16; i++) *p++ = 0x33;
    for (i = 0; i < 8; i++) *p++ = 0x44;
    for (i = 0; i < 4; i++) *p++ = 0x55;
    for (i = 0; i < 2; i++) *p++ = 0x66;
    *p++ = 0x77;
    *p++ = 0x88;
}

/* Luma DC size table (2nd form) */
void mpvvlc2_InitDcSizY(void) {
    u8 *p = lbl_eu_80603C28;
    int i;

    for (i = 0; i < 256; i++)
        *p++ = 0x12;
    for (i = 0; i < 256; i++)
        *p++ = 0x22;
    for (i = 0; i < 128; i++)
        *p++ = 0x03;
    for (i = 0; i < 128; i++)
        *p++ = 0x33;
    for (i = 0; i < 128; i++)
        *p++ = 0x43;
    for (i = 0; i < 64; i++)
        *p++ = 0x54;
    for (i = 0; i < 32; i++)
        *p++ = 0x65;
    for (i = 0; i < 16; i++)
        *p++ = 0x76;
    for (i = 0; i < 8; i++)
        *p++ = 0x87;
    for (i = 0; i < 4; i++)
        *p++ = 0x98;
    *p++ = 0xa9;
    *p++ = 0xa9;
    *p++ = 0xb9;
    *p++ = 0xb9;
}

/* Chroma DC size table (2nd form) */
void mpvvlc2_InitDcSizC(void) {
    u8 *p = lbl_eu_80604028;
    int i;

    for (i = 0; i < 256; i++)
        *p++ = 0x02;
    for (i = 0; i < 256; i++)
        *p++ = 0x12;
    for (i = 0; i < 256; i++)
        *p++ = 0x22;
    for (i = 0; i < 128; i++)
        *p++ = 0x33;
    for (i = 0; i < 64; i++)
        *p++ = 0x44;
    for (i = 0; i < 32; i++)
        *p++ = 0x55;
    for (i = 0; i < 16; i++)
        *p++ = 0x66;
    for (i = 0; i < 8; i++)
        *p++ = 0x77;
    for (i = 0; i < 4; i++)
        *p++ = 0x88;
    *p++ = 0x99;
    *p++ = 0x99;
    *p++ = 0xaa;
    *p++ = 0xba;
}

/* Intra run/level VLC table (128 dwords) */
void mpvvlc_InitIntRunLevel(void) {
    u32 *p = lbl_eu_80604428;
    int i;

    for (i = 0; i < 4; i++) *p++ = 0x00000000;
    for (i = 0; i < 4; i++) *p++ = 0x00064040;
    for (i = 0; i < 2; i++) *p++ = 0x00080202;
    for (i = 0; i < 2; i++) *p++ = 0x00080109;
    for (i = 0; i < 2; i++) *p++ = 0x00080400;
    for (i = 0; i < 2; i++) *p++ = 0x00080108;
    for (i = 0; i < 4; i++) *p++ = 0x00070107;
    for (i = 0; i < 4; i++) *p++ = 0x00070106;
    for (i = 0; i < 4; i++) *p++ = 0x00070201;
    for (i = 0; i < 4; i++) *p++ = 0x00070105;
    *p++ = 0x0009010d;
    *p++ = 0x00090600;
    *p++ = 0x0009010c;
    *p++ = 0x0009010b;
    *p++ = 0x00090203;
    *p++ = 0x00090301;
    *p++ = 0x00090500;
    *p++ = 0x0009010a;
    for (i = 0; i < 8; i++) *p++ = 0x00060300;
    for (i = 0; i < 8; i++) *p++ = 0x00060104;
    for (i = 0; i < 8; i++) *p++ = 0x00060103;
    for (i = 0; i < 16; i++) *p++ = 0x00050200;
    for (i = 0; i < 16; i++) *p++ = 0x00050102;
    for (i = 0; i < 32; i++) *p++ = 0x00040101;
}

/* I-picture macroblock address increment VLC tables */
void mpvvlc_InitMbaiIpic(void) {
    s16 *p = (s16 *)lbl_eu_80603728;
    s16 *q = (s16 *)lbl_eu_80603928;
    int v, i, j, k;

    for (i = 0; i < 16; i++)
        *p++ = 0x0240;
    *p++ = 0x023b;
    *p++ = 0x023b;
    for (i = 0; i < 12; i++)
        *p++ = 0x0240;
    *p++ = 0x022b;
    *p++ = 0x022b;
    for (i = 0; i < 16; i++)
        *p++ = 0x0240;

    v = 0x210;
    for (i = 0; i < 2; i++)
        for (j = 0; j < 6; j++) {
            *p++ = v | 0x440d;
            *p++ = v | 0x040c;
            v -= 0x10;
        }
    for (i = 0; i < 6; i++) {
        *p++ = v | 0x440c;
        *p++ = v | 0x440c;
        *p++ = v | 0x040b;
        *p++ = v | 0x040b;
        v -= 0x10;
    }
    for (i = 0; i < 2; i++)
        for (j = 0; j < 3; j++) {
            *p++ = v | 0x440a;
            *p++ = v | 0x440a;
            *p++ = v | 0x440a;
            *p++ = v | 0x440a;
            *p++ = v | 0x440a;
            *p++ = v | 0x440a;
            *p++ = v | 0x440a;
            *p++ = v | 0x440a;
            *p++ = v | 0x0409;
            *p++ = v | 0x0409;
            *p++ = v | 0x0409;
            *p++ = v | 0x0409;
            *p++ = v | 0x0409;
            *p++ = v | 0x0409;
            *p++ = v | 0x0409;
            *p++ = v | 0x0409;
            v -= 0x10;
        }
    for (i = 0; i < 2; i++) {
        *p++ = v | 0x4409;
        *p++ = v | 0x4409;
        *p++ = v | 0x4409;
        *p++ = v | 0x4409;
        *p++ = v | 0x4409;
        *p++ = v | 0x4409;
        *p++ = v | 0x4409;
        *p++ = v | 0x4409;
        *p++ = v | 0x4409;
        *p++ = v | 0x4409;
        *p++ = v | 0x4409;
        *p++ = v | 0x4409;
        *p++ = v | 0x4409;
        *p++ = v | 0x4409;
        *p++ = v | 0x4409;
        *p++ = v | 0x4409;
        *p++ = v | 0x0408;
        *p++ = v | 0x0408;
        *p++ = v | 0x0408;
        *p++ = v | 0x0408;
        *p++ = v | 0x0408;
        *p++ = v | 0x0408;
        *p++ = v | 0x0408;
        *p++ = v | 0x0408;
        *p++ = v | 0x0408;
        *p++ = v | 0x0408;
        *p++ = v | 0x0408;
        *p++ = v | 0x0408;
        *p++ = v | 0x0408;
        *p++ = v | 0x0408;
        *p++ = v | 0x0408;
        *p++ = v | 0x0408;
        v -= 0x10;
    }

    for (i = 0; i < 4; i++)
        *q++ = 0x0240;
    *q++ = 0x70 | 0x4407;
    *q++ = 0x70 | 0x0406;
    *q++ = 0x60 | 0x4407;
    *q++ = 0x60 | 0x0406;
    for (i = 0; i < 2; i++)
        *q++ = 0x50 | 0x4406;
    for (i = 0; i < 2; i++)
        *q++ = 0x50 | 0x0405;
    for (i = 0; i < 2; i++)
        *q++ = 0x40 | 0x4406;
    for (i = 0; i < 2; i++)
        *q++ = 0x40 | 0x0405;
    for (i = 0; i < 4; i++)
        *q++ = 0x30 | 0x4405;
    for (i = 0; i < 4; i++)
        *q++ = 0x30 | 0x0404;
    for (i = 0; i < 4; i++)
        *q++ = 0x20 | 0x4405;
    for (i = 0; i < 4; i++)
        *q++ = 0x20 | 0x0404;
    for (i = 0; i < 16; i++)
        *q++ = 0x10 | 0x4403;
    for (i = 0; i < 16; i++)
        *q++ = 0x10 | 0x0402;
}

/* P-picture macroblock address increment VLC tables */
void mpvvlc_InitMbaiPpic(void) {
    s16 *p = (s16 *)lbl_eu_806039A8;
    s16 *q = (s16 *)lbl_eu_80603AA8;
    int v;
    int i, j;

    for (i = 0; i < 8; i++)
        *p++ = 0x0240;
    *p++ = 0x023b;
    for (i = 0; i < 6; i++)
        *p++ = 0x0240;
    *p++ = 0x022b;
    for (i = 0; i < 8; i++)
        *p++ = 0x0240;

    v = 0x0210;
    for (i = 0; i < 12; i++) {
        *p++ = v | 0x000b;
        v -= 0x10;
    }
    *p++ = 0x015a;
    *p++ = -0x56a5;
    *p++ = 0x014a;
    *p++ = -0x56b5;
    *p++ = 0x013a;
    *p++ = -0x56c5;
    *p++ = 0x012a;
    *p++ = -0x56d5;
    *p++ = 0x011a;
    *p++ = -0x56e5;
    *p++ = 0x010a;
    *p++ = -0x56f5;

    v = 0xf0;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            *p++ = v | 0x0008;
            *p++ = v | 0xa00b;
            *p++ = v | 0x880a;
            *p++ = v | 0x880a;
            *p++ = v | 0xa809;
            *p++ = v | 0xa809;
            *p++ = v | 0xa809;
            *p++ = v | 0xa809;
            v -= 0x10;
        }
    }

    *p++ = 0x0097;
    *p++ = 0x0097;
    *p++ = -0x5f66;
    *p++ = -0x5f66;
    *p++ = -0x7767;
    *p++ = -0x7767;
    *p++ = -0x7767;
    *p++ = -0x7767;
    for (i = 0; i < 8; i++)
        *p++ = -0x5768;
    *p++ = 0x0087;
    *p++ = 0x0087;
    *p++ = -0x5f76;
    *p++ = -0x5f76;
    for (i = 0; i < 4; i++)
        *p++ = -0x7777;
    for (i = 0; i < 8; i++)
        *p++ = -0x5f78;

    *q++ = 0x0240;
    *q++ = 0x0240;
    *q++ = 0x0075;
    *q++ = 0x0065;
    *q++ = 0x0054;
    *q++ = -0x57ab;
    *q++ = 0x0044;
    *q++ = -0x57bb;
    *q++ = 0x0033;
    *q++ = -0x77cb;
    *q++ = -0x57cc;
    *q++ = -0x57cc;
    *q++ = 0x0023;
    *q++ = -0x77db;
    *q++ = -0x57dc;
    *q++ = -0x57dc;
    *q++ = 0x0011;
    *q++ = 0x0011;
    *q++ = -0x5fec;
    *q++ = -0x5fec;
    for (i = 0; i < 4; i++)
        *q++ = -0x77ed;
    for (i = 0; i < 8; i++)
        *q++ = -0x57ee;
}

/* B-picture macroblock address increment VLC tables */
void mpvvlc_InitMbaiBpic(void) {
    s16 *p = (s16 *)lbl_eu_80603AE8;
    s16 *q = (s16 *)lbl_eu_80603BE8;
    int v, i, j;

    for (i = 0; i < 8; i++)
        *p++ = 0x0240;
    *p++ = 0x023b;
    for (i = 0; i < 6; i++)
        *p++ = 0x0240;
    *p++ = 0x022b;
    for (i = 0; i < 8; i++)
        *p++ = 0x0240;

    v = 0x210;
    for (i = 0; i < 12; i++) {
        *p++ = v | 0x000b;
        v -= 0x10;
    }
    *p++ = 0x015a;
    *p++ = 0x015a;
    *p++ = 0x014a;
    *p++ = 0x014a;
    *p++ = 0x013a;
    *p++ = 0x013a;
    *p++ = 0x012a;
    *p++ = 0x012a;
    *p++ = 0x011a;
    *p++ = 0x011a;
    *p++ = 0x010a;
    *p++ = 0x010a;

    v = 0xf0;
    for (i = 0; i < 2; i++)
        for (j = 0; j < 3; j++) {
            *p++ = v | 0x0008;
            *p++ = v | 0x0008;
            *p++ = v | 0x900b;
            *p++ = v | 0x980b;
            *p++ = v | 0xb00a;
            *p++ = v | 0xb00a;
            *p++ = v | 0xb80a;
            *p++ = v | 0xb80a;
            *p++ = v | 0x0008;
            *p++ = v | 0x0008;
            *p++ = v | 0x900b;
            *p++ = v | 0x980b;
            *p++ = v | 0xb00a;
            *p++ = v | 0xb00a;
            *p++ = v | 0xb80a;
            *p++ = v | 0xb80a;
            *p++ = v | 0x0008;
            *p++ = v | 0x0008;
            *p++ = v | 0x900b;
            *p++ = v | 0x980b;
            *p++ = v | 0xb00a;
            *p++ = v | 0xb00a;
            *p++ = v | 0xb80a;
            *p++ = v | 0xb80a;
            v -= 0x10;
        }

    *p++ = 0x0097;
    *p++ = 0x0097;
    *p++ = -0x5f65;
    *p++ = -0x5765;
    *p++ = -0x6f66;
    *p++ = -0x6f66;
    *p++ = -0x6766;
    *p++ = -0x6766;
    for (i = 0; i < 4; i++)
        *p++ = -0x4f67;
    for (i = 0; i < 4; i++)
        *p++ = -0x4767;
    *p++ = 0x0087;
    *p++ = 0x0087;
    *p++ = -0x5f75;
    *p++ = -0x5775;
    *p++ = -0x6f76;
    *p++ = -0x6f76;
    *p++ = -0x6776;
    *p++ = -0x6776;
    for (i = 0; i < 4; i++)
        *p++ = -0x4f77;
    for (i = 0; i < 4; i++)
        *p++ = -0x4777;

    *q++ = 0x0240;
    *q++ = 0x0240;
    *q++ = 0x0075;
    *q++ = 0x0065;
    *q++ = 0x0054;
    *q++ = 0x0054;
    *q++ = 0x0044;
    *q++ = 0x0044;
    *q++ = 0x0033;
    *q++ = 0x0033;
    *q++ = -0x4fcb;
    *q++ = -0x47cb;
    *q++ = 0x0023;
    *q++ = 0x0023;
    *q++ = -0x4fdb;
    *q++ = -0x47db;
    *q++ = 0x0011;
    *q++ = 0x0011;
    *q++ = -0x5feb;
    *q++ = -0x57eb;
    *q++ = -0x6fec;
    *q++ = -0x6fec;
    *q++ = -0x67ec;
    *q++ = -0x67ec;
    for (i = 0; i < 4; i++)
        *q++ = -0x4ffd;
    for (i = 0; i < 4; i++)
        *q++ = -0x47fd;
}

/* Motion vector code tables */
void mpvvlc_InitMotion(void) {
    s16 *p = (s16 *)lbl_eu_80603510;
    s16 *q = (s16 *)lbl_eu_80603618;
    int n, i;

    for (i = 0; i < 24; i++)
        *p++ = 0x7f;
    for (n = 0x10; n >= 0x0b; n--) {
        *p++ = n | 0x0b00;
        *p++ = (s16)((u8)(-n) | 0x0b00);
    }
    for (n = 0x0a; n >= 0x08; n--) {
        *p++ = n | 0x0a00;
        *p++ = n | 0x0a00;
        *p++ = (s16)((u8)(-n) | 0x0a00);
        *p++ = (s16)((u8)(-n) | 0x0a00);
    }
    for (n = 0x07; n >= 0x05; n--) {
        for (i = 0; i < 8; i++)
            *p++ = n | 0x0800;
        for (i = 0; i < 8; i++)
            *p++ = (s16)((u8)(-n) | 0x0800);
    }
    for (i = 0; i < 8; i++)
        *p++ = 0x04 | 0x0700;
    for (i = 0; i < 8; i++)
        *p++ = (s16)((u8)(-0x04) | 0x0700);
    for (i = 0; i < 8; i++)
        *p++ = 0x02 | 0x0400;
    for (i = 0; i < 8; i++)
        *p++ = (s16)((u8)(-0x02) | 0x0400);

    *q++ = 0x7f;
    *q++ = 0x7f;
    *q++ = 0x03 | 0x0500;
    *q++ = (s16)((u8)(-0x03) | 0x0500);
    *q++ = 0x02 | 0x0400;
    *q++ = 0x02 | 0x0400;
    *q++ = (s16)((u8)(-0x02) | 0x0400);
    *q++ = (s16)((u8)(-0x02) | 0x0400);
    for (i = 0; i < 4; i++)
        *q++ = 0x01 | 0x0300;
    for (i = 0; i < 4; i++)
        *q++ = (s16)((u8)(-0x01) | 0x0300);
    for (i = 0; i < 16; i++)
        *q++ = 0x00 | 0x0100;
}

/* Set VLC table default pointers in the MPV context */
void mpvvlc_SetDflPtr(void) {
    u32 *g = (u32 *)lbl_eu_80602FF8;
    u32 *d = lbl_eu_8051C2B8;

    g[0x58c] = (u32)(g + 0x1cc);
    g[0x58d] = (u32)(g + 0x24c);
    g[0x58e] = (u32)(g + 0x26c);
    g[0x58f] = (u32)(g + 0x2ac);
    g[0x590] = (u32)(g + 0x2bc);
    g[0x591] = (u32)(g + 0x2fc);
    g[0x198] = (u32)(g + 0x19a);
    g[0x1aa] = (u32)(g + 0x1ac);
    g[0x144] = (u32)(g + 0x146);
    g[0x186] = (u32)(g + 0x188);
    g[0x592] = (u32)(g + 0x000);
    g[0x100] = (u32)(g + 0x102);
    g[0x122] = (u32)(g + 0x124);
    g[0x593] = (u32)(g + 0x30c);
    g[0x594] = (u32)(g + 0x40c);
    g[0x595] = (u32)(d + 0x000);
    g[0x596] = (u32)(d + 0x008);
    g[0x597] = (u32)(d + 0x010);
    g[0x598] = (u32)(d + 0x018);
    g[0x599] = (u32)(d + 0x020);
    g[0x59a] = (u32)(d + 0x028);
    g[0x59b] = (u32)(g + 0x50c);
}

/* Install run/level VLC tables (copied from the default table image) */
u32 *mpvvlc_SetVlcRunLevel(u32 *tbl) {
    u32 *g = (u32 *)lbl_eu_80602FF8;
    u32 *d = lbl_eu_8051C2B8;

    g[0x59b] = (u32)(tbl - 0x80);
    UTY_MemcpyDword(tbl - 0x80, &g[0x50c], 0x80);
    g[0x59a] = (u32)(tbl - 0x84);
    UTY_MemcpyDword(tbl - 0x84, &d[0x28], 4);
    g[0x599] = (u32)(tbl - 0x8c);
    UTY_MemcpyDword(tbl - 0x8c, &d[0x20], 8);
    g[0x598] = (u32)(tbl - 0x94);
    UTY_MemcpyDword(tbl - 0x94, &d[0x18], 8);
    g[0x597] = (u32)(tbl - 0x9c);
    UTY_MemcpyDword(tbl - 0x9c, &d[0x10], 8);
    g[0x596] = (u32)(tbl - 0xa4);
    UTY_MemcpyDword(tbl - 0xa4, &d[0x08], 8);
    g[0x595] = (u32)(tbl - 0xac);
    UTY_MemcpyDword(tbl - 0xac, &d[0x00], 8);
    return tbl - 0xac;
}
