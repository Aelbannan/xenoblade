#pragma once

#include <types.h>

typedef struct SFXSetState {
    u8 _00[0x04];
    u32 compoMode;
    u8 _08[0x08];
    u32 unitWidth;
    u8 _14[0x44];
    u32 splitField;
    u32 progOut;
    u8 _60[0x04];
    u32 cnvBottomUp;
} SFXSetState;

typedef void (*SFXAlphaCallback)(u32, u32, u32, u32);
typedef void (*SFXAlphaTableCallback)(int, int, int, int);

typedef struct SFXAlphaState {
    u32 _00;
    u32 needsUpdate;
    u32 arg0;
    u32 arg1;
    u32 arg2;
    u8 byte14;
    u8 byte15;
    u8 byte16;
    u8 _17;
    SFXAlphaCallback makeLumiTable;
    SFXAlphaTableCallback makeAlp3110Table;
    SFXAlphaTableCallback makeAlpTable;
} SFXAlphaState;

typedef struct SFXConvertState {
    u8 _00[0x04];
    u32 formatType;
    u32 _08;
    u32 _0C;
    u32 _10;
    u8 _14[0x10];
    u32 _24;
    u8 _28[0x08];
    SFXAlphaState* alphaState;
    u32 compoMode;
    u32 _38;
    u8 _3C[0x14];
    u32 _50;
    u8 _54[0x14];
    void* makeCnvFrameCallback;
    void* copyAlphaCallback;
    void* makeColorAdjustCallback;
    u32 bytesPerPixelOut;
} SFXConvertState;

typedef struct SFXStmInf {
    u32 srcType;
    u32 _04;
    u32 _08;
    u32 _0C;
    u32 _10;
    u32 _14;
    u32 _18;
    u32 _1C;
    u8 _20[0x04];
    u32 _24;
    u32 _28;
    u32 _2C;
    u8 _30[0x14];
    u32 width;
    u32 bytesPerLine;
    u32 _4C;
    u8 _50[0x38];
    u32 _88;
    u32 _8C;
    u32 _90;
    u32 _94;
} SFXStmInf;

typedef struct SFXCnvSrcBuf {
    u32 type;
    u32 _04;
    u32 _08;
    u32 _0C;
    u32 _10;
    u32 _14;
    u32 _18;
    u32 _1C;
    u32 _20;
    u32 _24;
    u32 _28;
    u32 _2C;
    u32 _30;
    u8 _34[0x30];
} SFXCnvSrcBuf;

typedef struct SFXDstBufInf {
    u32 numPlanes;
    u32 _04;
    u32 _08;
    u32 _0C;
    u32 _10;
    u8 _14[0x50];
} SFXDstBufInf;

typedef struct SFXZmvSlot {
    u32 active;
    u32 _04;
    u32 _08;
    u32 _0C;
    u32 _10;
    u8 _14[0x04];
    u32 _18;
    u32 _1C;
    u32 _20;
    u8 _24[0x04];
    u32 _28;
    u32 _2C;
    u32 _30;
    u8 _34[0x08];
    float _3C;
    float _40;
    u32 _44;
    u32 _48;
} SFXZmvSlot;

typedef struct SFXZmvState {
    u8 _00[0x08];
    u32 active;
    u32 arg0;
    u32 arg1;
} SFXZmvState;

typedef struct SFXZmvGlobal {
    u32 count;
    u32 _04;
    u32 maxCount;
    SFXZmvSlot slots[8];
} SFXZmvGlobal;

typedef struct SFXHandleState {
    u32 active;        /* 0x00 */
    u32 field_0x04;    /* 0x04 */
    u32 field_0x08;    /* 0x08 */
    u32 field_0x0C;    /* 0x0C */
    u32 field_0x10;    /* 0x10 */
    u32 tagFlag;       /* 0x14 */
    u32 tagVal1;       /* 0x18 */
    u32 tagVal2;       /* 0x1C */
    u32 field_0x20;    /* 0x20 */
    void* zmv;         /* 0x24 */
    u32 field_0x28;    /* 0x28 */
    u32 field_0x2C;    /* 0x2C */
    u32 field_0x30;    /* 0x30 */
    u32 field_0x34;    /* 0x34 */
    u32 alignW;        /* 0x38 */
    u32 bufASize;      /* 0x3C */
    u32 bufBSize;      /* 0x40 */
    u32 bufCSize;      /* 0x44 */
    u8 _48[0x08];      /* 0x48 */
    u32 width;         /* 0x50 */
    u32 height;        /* 0x54 */
    u32 field_0x58;    /* 0x58 */
    u8 _5C[0x08];      /* 0x5C */
    u32 field_0x64;    /* 0x64 */
} SFXHandleState;
