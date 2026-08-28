// CriWare Sofdec MPEG-2 VLC table initialization.
// All tables live in .bss and are filled at MPVVLC_Init time.
// Entries pack MPEG-2 VLC code info: (run << 8) | level-style codes for
// mb_type / mb_addr_inc / cbp / dc_size / run-level tables.

#include <harness_catalog.h>

// Absorbed from criware_data (retail bytes) - generated
__attribute__((aligned(8))) unsigned char __bss_fill_0[0x5E8]; // filler gap at 0x0 for .bss
u16 lbl_eu_80602FF8[0x200]; // bss off 0x5E8
unsigned char __bss_fill_1[0x8]; // filler gap at 0x9E8 for .bss
u8 lbl_eu_80603400[0x80]; // bss off 0x9F0
unsigned char __bss_fill_2[0x8]; // filler gap at 0xA70 for .bss
u8 lbl_eu_80603488[0x80]; // bss off 0xA78
unsigned char __bss_fill_3[0x8]; // filler gap at 0xAF8 for .bss
u8 lbl_eu_80603510[0x100]; // bss off 0xB00
unsigned char __bss_fill_4[0x8]; // filler gap at 0xC00 for .bss
u16 lbl_eu_80603618[0x20]; // bss off 0xC08
unsigned char __bss_fill_5[0x8]; // filler gap at 0xC48 for .bss
u16 lbl_eu_80603660[0x20]; // bss off 0xC50
unsigned char __bss_fill_6[0x8]; // filler gap at 0xC90 for .bss
u16 lbl_eu_806036A8[0x40]; // bss off 0xC98
u16 lbl_eu_80603728[0x100]; // bss off 0xD18
u16 lbl_eu_80603928[0x40]; // bss off 0xF18
u16 lbl_eu_806039A8[0x80]; // bss off 0xF98
u16 lbl_eu_80603AA8[0x20]; // bss off 0x1098
u16 lbl_eu_80603AE8[0x80]; // bss off 0x10D8
u16 lbl_eu_80603BE8[0x20]; // bss off 0x11D8
u8 lbl_eu_80603C28[0x400]; // bss off 0x1218
u8 lbl_eu_80604028[0x400]; // bss off 0x1618
u32 lbl_eu_80604428[0x80]; // bss off 0x1A18
unsigned char __bss_fill_7[0x90]; // trailing filler for .bss
__declspec(section ".rodata") __attribute__((aligned(8))) u32 lbl_eu_8051C2B8[0x2C] = {0x12011101, 0x10010F01, 0x03060210, 0x020F020E, 0x020D020C, 0x020B011F, 0x011E011D, 0x011C011B, 0x28002700, 0x26002500, 0x24002300, 0x22002100, 0x20000E01, 0x0D010C01, 0x0B010A01, 0x09010801, 0x1F001E00, 0x1D001C00, 0x1B001A00, 0x19001800, 0x17001600, 0x15001400, 0x13001200, 0x11001000, 0x020A0209, 0x03050403, 0x05020701, 0x06010F00, 0x0E000D00, 0x0C00011A, 0x01190118, 0x01170116, 0x0B000208, 0x03040A00, 0x04020207, 0x01150114, 0x09000113, 0x01120501, 0x03030800, 0x02060111, 0x01100205, 0x07000302, 0x0401010F, 0x010E0204}; // off 0x0


extern int UTY_MemcpyDword(u32 *dst, const u32 *src, u32 n);

u32 *mpvvlc_SetVlcRunLevel(u32 *tbl);

int MPVVLC_IsVlcSizErr(void) { return 0x0; }

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

    p[0] = 0;
    p[1] = 0;
    p[8] = p[9] = -0x45f8;
    p[10] = p[11] = -0x49f8;
    p[12] = p[13] = -0x51f8;
    p[14] = p[15] = -0x61f8;
    p[16] = p[17] = 0x7908;
    p[18] = p[19] = 0x7508;
    p[20] = p[21] = 0x6d08;
    p[22] = p[23] = 0x5d08;
    p[24] = p[25] = -0x59f8;
    p[26] = p[27] = -0x65f8;
    p[28] = p[29] = 0x6508;
    p[30] = p[31] = 0x5908;
    /* Single-use codes kept away from the paired constants so the scheduler
       leaves them as inline li/store pairs (matches retail) */
    p[2] = -0x18f7;
    p[3] = -0x24f7;
    p[4] = -0x04f7;
    p[5] = -0x08f7;
    p[6] = -0x10f7;
    p[7] = -0x20f7;
    p[32] = p[33] = -0x14f8;
    p[34] = p[35] = -0x28f8;
    p[36] = p[37] = -0xcf8;
    p[38] = p[39] = -0x30f8;
    p[40] = p[41] = -0x55f8;
    p[42] = p[43] = -0x69f8;
    p[44] = p[45] = -0x4df8;
    p[46] = p[47] = -0x71f8;
    p[48] = p[49] = 0x6908;
    p[50] = p[51] = 0x5508;
    p[52] = p[53] = 0x7108;
    p[54] = p[55] = 0x4d08;
    p[56] = p[57] = -0x1cf8;
    p[59] = p[58] = -0x2cf8;
    p[58] = p[59] = -0x2cf8;
    p[60] = p[61] = -0x34f8;
    p[62] = -0x38f8;
    /* Distinct expression form, same immediate: retail emits two li's here */
    p[63] = 0xc708 - 0x10000;
    return tbl + 0x40;
}


u16 *mpvvlc_InitCbpSub2(u16 *tbl) {
    s16 *p = (s16 *)tbl;
    s16 *q;
    int j;

    /* Chained assignments: one temp feeds each run of identical stores.
       Statement order tuned so MWCC defines the temps in retail's order */
    p[0] = p[1] = p[2] = p[3] = -0x5df9;
    p[4] = p[5] = p[6] = p[7] = -0x6df9;
    p[8] = p[9] = p[10] = p[11] = -0x75f9;
    p[12] = p[13] = p[14] = p[15] = -0x79f9;
    p[16] = p[17] = p[18] = p[19] = 0x6107;
    p[20] = p[21] = p[22] = p[23] = 0x5107;
    p[24] = p[25] = p[26] = p[27] = 0x4907;
    p[28] = p[29] = p[30] = p[31] = 0x4507;
    p[32] = p[33] = p[34] = p[35] = p[36] = p[37] = p[38] = p[39] = -0xfa;
    p[40] = p[41] = p[42] = p[43] = p[44] = p[45] = p[46] = p[47] = -0x3cfa;
    p[48] = p[49] = p[50] = p[51] = p[52] = p[53] = p[54] = p[55] = 0x2406;
    p[56] = p[57] = p[58] = p[59] = p[60] = p[61] = p[62] = p[63] = 0x1806;
    /* Last cbp group as singles: keeps its li out of the hoisted set */
    p[64] = -0x41fb;
    p[65] = -0x41fb;
    p[66] = -0x41fb;
    p[67] = -0x41fb;
    p[68] = -0x41fb;
    p[69] = -0x41fb;
    p[70] = -0x41fb;
    p[71] = -0x41fb;
    p[72] = -0x41fb;
    p[73] = -0x41fb;
    p[74] = -0x41fb;
    p[75] = -0x41fb;
    p[76] = -0x41fb;
    p[77] = -0x41fb;
    p[78] = -0x41fb;
    p[79] = -0x41fb;

    q = (s16 *)(tbl + 0x50);

    /* Singles: retail defines the first chroma constant just before use */
    q[0] = 0x81fb - 0x10000;
    q[1] = 0x81fb - 0x10000;
    q[2] = 0x81fb - 0x10000;
    q[3] = 0x81fb - 0x10000;
    q[4] = 0x81fb - 0x10000;
    q[5] = 0x81fb - 0x10000;
    q[6] = 0x81fb - 0x10000;
    q[7] = 0x81fb - 0x10000;
    q[8] = 0x81fb - 0x10000;
    q[9] = 0x81fb - 0x10000;
    q[10] = 0x81fb - 0x10000;
    q[11] = 0x81fb - 0x10000;
    q[12] = 0x81fb - 0x10000;
    q[13] = 0x81fb - 0x10000;
    q[14] = 0x81fb - 0x10000;
    q[15] = 0x81fb - 0x10000;
    q[16] = q[17] = q[18] = q[19] = q[20] = q[21] = q[22] = q[23] =
    q[24] = q[25] = q[26] = q[27] = q[28] = q[29] = q[30] = q[31] = 0x7d05;
    q[32] = q[33] = q[34] = q[35] = q[36] = q[37] = q[38] = q[39] =
    q[40] = q[41] = q[42] = q[43] = q[44] = q[45] = q[46] = q[47] = 0x4105;
    q[48] = q[49] = q[50] = q[51] = q[52] = q[53] = q[54] = q[55] =
    q[56] = q[57] = q[58] = q[59] = q[60] = q[61] = q[62] = q[63] = 0x3805;
    q[64] = q[65] = q[66] = q[67] = q[68] = q[69] = q[70] = q[71] =
    q[72] = q[73] = q[74] = q[75] = q[76] = q[77] = q[78] = q[79] = 0x3405;
    q[80] = q[81] = q[82] = q[83] = q[84] = q[85] = q[86] = q[87] =
    q[88] = q[89] = q[90] = q[91] = q[92] = q[93] = q[94] = q[95] = 0x2c05;
    q[96] = q[97] = q[98] = q[99] = q[100] = q[101] = q[102] = q[103] =
    q[104] = q[105] = q[106] = q[107] = q[108] = q[109] = q[110] = q[111] = 0x1c05;
    q[112] = q[113] = q[114] = q[115] = q[116] = q[117] = q[118] = q[119] =
    q[120] = q[121] = q[122] = q[123] = q[124] = q[125] = q[126] = q[127] = 0x2805;
    q[128] = q[129] = q[130] = q[131] = q[132] = q[133] = q[134] = q[135] =
    q[136] = q[137] = q[138] = q[139] = q[140] = q[141] = q[142] = q[143] = 0x1405;
    q[144] = q[145] = q[146] = q[147] = q[148] = q[149] = q[150] = q[151] =
    q[152] = q[153] = q[154] = q[155] = q[156] = q[157] = q[158] = q[159] = 0x3005;
    q[160] = q[161] = q[162] = q[163] = q[164] = q[165] = q[166] = q[167] =
    q[168] = q[169] = q[170] = q[171] = q[172] = q[173] = q[174] = q[175] = 0x0c05;
    q[176] = q[177] = q[178] = q[179] = q[180] = q[181] = q[182] = q[183] =
    q[184] = q[185] = q[186] = q[187] = q[188] = q[189] = q[190] = q[191] =
    q[192] = q[193] = q[194] = q[195] = q[196] = q[197] = q[198] = q[199] =
    q[200] = q[201] = q[202] = q[203] = q[204] = q[205] = q[206] = q[207] = 0x2004;
    q[208] = q[209] = q[210] = q[211] = q[212] = q[213] = q[214] = q[215] =
    q[216] = q[217] = q[218] = q[219] = q[220] = q[221] = q[222] = q[223] =
    q[224] = q[225] = q[226] = q[227] = q[228] = q[229] = q[230] = q[231] =
    q[232] = q[233] = q[234] = q[235] = q[236] = q[237] = q[238] = q[239] = 0x1004;
    q[240] = q[241] = q[242] = q[243] = q[244] = q[245] = q[246] = q[247] =
    q[248] = q[249] = q[250] = q[251] = q[252] = q[253] = q[254] = q[255] =
    q[256] = q[257] = q[258] = q[259] = q[260] = q[261] = q[262] = q[263] =
    q[264] = q[265] = q[266] = q[267] = q[268] = q[269] = q[270] = q[271] = 0x0804;
    q[272] = q[273] = q[274] = q[275] = q[276] = q[277] = q[278] = q[279] =
    q[280] = q[281] = q[282] = q[283] = q[284] = q[285] = q[286] = q[287] =
    q[288] = q[289] = q[290] = q[291] = q[292] = q[293] = q[294] = q[295] =
    q[296] = q[297] = q[298] = q[299] = q[300] = q[301] = q[302] = q[303] = 0x0404;
    /* Unroll factor 32: retail keeps this loop rolled with a 32-store body */
    for (j = 0; j < 64; j++)
        *q++ = 0x3c03;

    return (u16 *)q;
}

/* Luma DC size table */
void mpvvlc_InitDcSizY(void) {
    int i;
    s8 *p = (s8*)lbl_eu_80603400;
    s8 v = 0x12;

    for (i = 0; i < 32; i++) *p++ = v;
    for (i = 0; i < 32; i++) *p++ = 0x22;
    for (i = 0; i < 16; i++) *p++ = 0x03;
    for (i = 0; i < 16; i++) *p++ = 0x33;
    for (i = 0; i < 16; i++) *p++ = 0x43;
    for (i = 0; i < 8; i++) *p++ = 0x54;
    for (i = 0; i < 4; i++) *p++ = 0x65;
    for (i = 0; i < 2; i++) *p++ = 0x76;
    for (i = 0; i < 2; i++) *p++ = -121;
}

/* Chroma DC size table */
void mpvvlc_InitDcSizC(void) {
    s8 *p = (s8*)lbl_eu_80603488;
    int i;

    for (i = 0; i < 32; i++) *p++ = 0x02;
    for (i = 0; i < 32; i++) *p++ = 0x12;
    for (i = 0; i < 32; i++) *p++ = 0x22;
    for (i = 0; i < 16; i++) *p++ = 0x33;
    for (i = 0; i < 8; i++) *p++ = 0x44;
    for (i = 0; i < 4; i++) *p++ = 0x55;
    for (i = 0; i < 2; i++) *p++ = 0x66;
    *p++ = 0x77;
    *p++ = -120;
}

/* Luma DC size table (2nd form) */
void mpvvlc2_InitDcSizY(void) {
    s8 *p = (s8 *)lbl_eu_80603C28;
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
        *p++ = -0x79;
    for (i = 0; i < 4; i++)
        *p++ = -0x68;
    *p++ = -0x57;
    *p++ = -0x57;
    *p++ = -0x47;
    *p++ = -0x47;
}

/* Chroma DC size table (2nd form) */
void mpvvlc2_InitDcSizC(void) {
    s8 *p = (s8 *)lbl_eu_80604028;
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
        *p++ = -0x78;
    *p++ = -0x67;
    *p++ = -0x67;
    *p++ = -0x56;
    *p++ = -0x46;
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
    s16 *q;
    int v, i, j;

    /* Head fillers written out individually: retail emits plain sth, no rolled loop */
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x023b;
    *p++ = 0x023b;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x022b;
    *p++ = 0x022b;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;

    v = 0x210;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 6; j++) {
            *p++ = v | 0x440d;
            *p++ = v | 0x040c;
            v -= 0x10;
        }
    }
    for (i = 0; i < 6; i++) {
        *p++ = v | 0x440c;
        *p++ = v | 0x440c;
        *p++ = v | 0x040b;
        *p++ = v | 0x040b;
        v -= 0x10;
    }
    for (i = 0; i < 2; i++) {
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

    /* Second table address is materialized only here (matches retail); head unrolled */
    q = (s16 *)lbl_eu_80603928;
    *q++ = 0x0240;
    *q++ = 0x0240;
    *q++ = 0x0240;
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
    s16 *q;
    int v, i, j;

    /* Head fillers written out: retail emits individual sth, no rolled loop */
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x023b;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x022b;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;
    *p++ = 0x0240;

    /* Escape run written as a loop so MWCC keeps v live (ori/subi like retail) */
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

    /* Three 8-entry groups (codes 8/a00b/880a x2/a809 x4) per outer pass */
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

    /* Escape tails for v=0x90 and v=0x80: codes 7/a00a x2, 8809 x4, a808 x8 */
    v = 0x90;
    for (i = 0; i < 2; i++) {
        *p++ = v | 0x0007;
        *p++ = v | 0x0007;
        *p++ = v | 0xa00a;
        *p++ = v | 0xa00a;
        for (j = 0; j < 4; j++)
            *p++ = v | 0x8809;
        for (j = 0; j < 8; j++)
            *p++ = v | 0xa808;
        v -= 0x10;
    }

    /* Second table address is materialized only here (matches retail) */
    q = (s16 *)lbl_eu_80603AA8;
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
    for (i = 0; i < 2; i++)
        *q++ = 0x10 | 0xa004;
    for (i = 0; i < 4; i++)
        *q++ = 0x10 | 0x8803;
    for (i = 0; i < 8; i++)
        *q++ = 0x10 | 0xa802;
}

/* B-picture macroblock address increment VLC tables */
void mpvvlc_InitMbaiBpic(void) {
    s16 *p = (s16 *)lbl_eu_80603AE8;
    s16 *q;
    int v, i, j;

    /* Head: fillers of 0x240 split by the 9th (0x23b) and 16th (0x22b) codes */
    *p++ = 0x240;
    *p++ = 0x240;
    *p++ = 0x240;
    *p++ = 0x240;
    *p++ = 0x240;
    *p++ = 0x240;
    *p++ = 0x240;
    *p++ = 0x240;
    *p++ = 0x23b;
    *p++ = 0x240;
    *p++ = 0x240;
    *p++ = 0x240;
    *p++ = 0x240;
    *p++ = 0x240;
    *p++ = 0x240;
    *p++ = 0x22b;
    *p++ = 0x240;
    *p++ = 0x240;
    *p++ = 0x240;
    *p++ = 0x240;
    *p++ = 0x240;
    *p++ = 0x240;
    *p++ = 0x240;
    *p++ = 0x240;

    /* 12 macroblock-address-increment escape entries, descending by 0x10 */
    v = 0x210;
    for (i = 0; i < 12; i++) {
        *p++ = v | 0x000b;
        v -= 0x10;
    }

    *p++ = 0x15a;
    *p++ = 0x15a;
    *p++ = 0x14a;
    *p++ = 0x14a;
    *p++ = 0x13a;
    *p++ = 0x13a;
    *p++ = 0x12a;
    *p++ = 0x12a;
    *p++ = 0x11a;
    *p++ = 0x11a;
    *p++ = 0x10a;
    *p++ = 0x10a;

    /* Three 8-entry groups per outer pass */
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
            v -= 0x10;
        }

    /* Escape tails for v=0x90 and v=0x80 */
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

    /* Second table: plain literals, then a shared base 0x10 for the final
       group so retail emits ori chains off one register */
    q = (s16 *)lbl_eu_80603BE8;
    *q++ = 0x240;
    *q++ = 0x240;
    *q++ = 0x75;
    *q++ = 0x65;
    *q++ = 0x54;
    *q++ = 0x54;
    *q++ = 0x44;
    *q++ = 0x44;
    *q++ = 0x33;
    *q++ = 0x33;
    *q++ = -0x4fcb;
    *q++ = -0x47cb;
    *q++ = 0x23;
    *q++ = 0x23;
    *q++ = -0x4fdb;
    *q++ = -0x47db;
    *q++ = 0x11;
    *q++ = 0x11;
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

/* Set a motion vector code pair: (n | base) and ((u8)(-n) | base) */

/* Motion vector code tables.
   Entry for magnitude n is (n | base) or ((u8)-n | base); copy counts grow
   as magnitude shrinks (1/2/8/16), so MWCC fully unrolls every loop here. */
void mpvvlc_InitMotion(void) {
    s16 *p = (s16 *)lbl_eu_80603510;
    s16 *q;
    int i;
    int n;

    *p++ = 0x7f;
    for (i = 0; i < 23; i++)
        p[i] = 0x7f;

    /* Magnitudes are re-assigned per store pair; MWCC emits li/neg/clrlwi/ori
       chains instead of folding to immediates */
    n = 0x10;
    *p++ = n | 0x0b00;
    *p++ = (u8)-n | 0x0b00;
    n = 0x0f;
    *p++ = n | 0x0b00;
    *p++ = (u8)-n | 0x0b00;
    n = 0x0e;
    *p++ = n | 0x0b00;
    *p++ = (u8)-n | 0x0b00;
    n = 0x0d;
    *p++ = n | 0x0b00;
    *p++ = (u8)-n | 0x0b00;
    n = 0x0c;
    *p++ = n | 0x0b00;
    *p++ = (u8)-n | 0x0b00;
    n = 0x0b;
    *p++ = n | 0x0b00;
    *p++ = (u8)-n | 0x0b00;

    n = 0x0a;
    *p++ = n | 0x0a00;
    *p++ = n | 0x0a00;
    *p++ = (u8)-n | 0x0a00;
    *p++ = (u8)-n | 0x0a00;
    n = 9;
    *p++ = n | 0x0a00;
    *p++ = n | 0x0a00;
    *p++ = (u8)-n | 0x0a00;
    *p++ = (u8)-n | 0x0a00;
    n = 8;
    *p++ = n | 0x0a00;
    *p++ = n | 0x0a00;
    *p++ = (u8)-n | 0x0a00;
    *p++ = (u8)-n | 0x0a00;

    for (n = 7; n >= 5; n--) {
        for (i = 0; i < 8; i++)
            *p++ = n | 0x0800;
        for (i = 0; i < 8; i++)
            *p++ = (u8)-n | 0x0800;
    }

    /* Level 4 keeps its magnitude in a live variable so retail's neg/ori
       chain (instead of a folded constant) is reproduced */
    n = 4;
    for (i = 0; i < 16; i++)
        *p++ = n | 0x0700;

    /* Second table address is materialized only here (matches retail) */
    q = (s16 *)lbl_eu_80603618;
    *q++ = 0x7f;
    q[0] = 0x7f;
    n = 3;
    *q++ = n | 0x0500;
    *q++ = (u8)-n | 0x0500;
    n = 2;
    *q++ = n | 0x0400;
    *q++ = n | 0x0400;
    *q++ = (u8)-n | 0x0400;
    *q++ = (u8)-n | 0x0400;
    n = 1;
    for (i = 0; i < 4; i++)
        *q++ = n | 0x0300;
    for (i = 0; i < 4; i++)
        *q++ = (u8)-n | 0x0300;
    for (i = 0; i < 16; i++)
        *p++ = (u8)-n | 0x0700;
    for (i = 0; i < 16; i++)
        *q++ = (u8)-n | 0x0100;
}

/* Set VLC table default pointers in the MPV context.
   Each destination pointer is materialized into its own local first
   (retail's register-heavy schedule), then stored slot by slot. */
void mpvvlc_SetDflPtr(void) {
    u32 *d = lbl_eu_8051C2B8;
    u32 *g = (u32 *)lbl_eu_80602FF8;
    u32 *p_730 = g + 0x1cc;
    u32 *p_d20 = d + 0x08;
    u32 *p_930 = g + 0x24c;
    u32 *p_9b0 = g + 0x26c;
    u32 *p_ab0 = g + 0x2ac;
    u32 *p_af0 = g + 0x2bc;
    u32 *p_bf0 = g + 0x2fc;
    u32 *p_668 = g + 0x19a;
    u32 *p_6b0 = g + 0x1ac;
    u32 *p_518 = g + 0x146;
    u32 *p_620 = g + 0x188;
    u32 *p_000 = g;
    u32 *p_408 = g + 0x102;
    u32 *p_490 = g + 0x124;
    u32 *p_c30 = g + 0x30c;
    u32 *p_1030 = g + 0x40c;
    u32 *p_d40 = d + 0x10;
    u32 *p_d60 = d + 0x18;
    u32 *p_d80 = d + 0x20;
    u32 *p_da0 = d + 0x28;
    u32 *p_1430 = g + 0x50c;

    g[0x58c] = (u32)p_730;
    g[0x58d] = (u32)p_930;
    g[0x58e] = (u32)p_9b0;
    g[0x58f] = (u32)p_ab0;
    g[0x590] = (u32)p_af0;
    g[0x591] = (u32)p_bf0;
    g[0x198] = (u32)p_668;
    g[0x1aa] = (u32)p_6b0;
    g[0x144] = (u32)p_518;
    g[0x186] = (u32)p_620;
    g[0x592] = (u32)p_000;
    g[0x100] = (u32)p_408;
    g[0x122] = (u32)p_490;
    g[0x593] = (u32)p_c30;
    g[0x594] = (u32)p_1030;
    g[0x595] = (u32)d;
    g[0x596] = (u32)p_d20;
    g[0x597] = (u32)p_d40;
    g[0x598] = (u32)p_d60;
    g[0x599] = (u32)p_d80;
    g[0x59a] = (u32)p_da0;
    g[0x59b] = (u32)p_1430;
}

/* Master VLC table initialization; optionally installs per-context run/level
   copies starting 0x16c dwords into the supplied work area. */
void MPVVLC_Init(u32 *work) {
    u32 *g = (u32 *)lbl_eu_80602FF8;
    mpvvlc_InitMbaiIpic();
    mpvvlc_InitMbaiPpic();
    mpvvlc_InitMbaiBpic();
    mpvvlc_InitMbTypePpic();
    mpvvlc_InitMbTypeBpic();
    mpvvlc_InitMotion();
    mpvvlc_InitCbpSub2(mpvvlc_InitCbpSub1((u16 *)lbl_eu_80602FF8));
    mpvvlc_InitDcSizY();
    mpvvlc_InitDcSizC();
    mpvvlc2_InitDcSizY();
    mpvvlc2_InitDcSizC();
    mpvvlc_InitIntRunLevel();
    mpvvlc_SetDflPtr();

    if (work != NULL) {
        /* Install per-context run/level copies; work is reused as the table base */
        work = mpvvlc_SetVlcRunLevel(work + 0x16c);
        g[0x100] = (u32)(work - 0x20);
        UTY_MemcpyDword(work - 0x20, &g[0x102], 0x20);
        g[0x122] = (u32)(work - 0x40);
        UTY_MemcpyDword(work - 0x40, &g[0x124], 0x20);
        g[0x144] = (u32)(work - 0x80);
        UTY_MemcpyDword(work - 0x80, &g[0x146], 0x40);
        g[0x186] = (u32)(work - 0x90);
        UTY_MemcpyDword(work - 0x90, &g[0x188], 0x10);
        g[0x198] = (u32)(work - 0xa0);
        UTY_MemcpyDword(work - 0xa0, &g[0x19a], 0x10);
        g[0x1aa] = (u32)(work - 0xc0);
        UTY_MemcpyDword(work - 0xc0, &g[0x1ac], 0x20);
    }
}

/* Install run/level VLC tables (copied from the default table image). */
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
