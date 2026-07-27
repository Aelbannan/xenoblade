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
    u8 _00[0x30];
    SFXAlphaState* alphaState;
    u8 _34[0x34];
    void* makeCnvFrameCallback;
    void* copyAlphaCallback;
    void* makeColorAdjustCallback;
    u32 bytesPerPixelOut;
} SFXConvertState;

typedef struct SFXZmvState {
    u8 _00[0x08];
    u32 active;
    u32 arg0;
    u32 arg1;
} SFXZmvState;
