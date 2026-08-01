// WPADHIDParser.c - Wiimote HID report parsing (RVL_SDK wpad library)
//
// Handler dispatch, status/read/ack report processing, DPD (IR) parsing,
// Classic Controller / FS / VS (Vitality Sensor) extension parsing and
// sensor-bar calibration.

#include <revolution/WPAD.h>
#include <revolution/WPAD/WPADInternal.h>
#include <revolution/OS.h>

#include <math.h>
#include <string.h>

void DEBUGPrint(const char* pFmt, ...);

int WUDIsLinkedWBC(void);
int WPADIsEnabledVSM(void);
int WPADIsEnabledTRN(void);
int WPADIsEnabledGTR(void);
int WPADIsEnabledDRM(void);
void WPADiCreateKey(s32 chan);
void WPADiCreateKeyFor3rd(s32 chan);
void WPADiDecode(s32 chan, u8* buf, u32 len, s32 offset);

extern WPADCB* __rvl_p_wpadcb[WPAD_MAX_CONTROLLERS];

#define PI 3.141592f

#define WPAD_DPD_IMG_RESO_WX 1024
#define WPAD_DPD_IMG_RESO_WY 768

#define HID_WPAD_BUTTON_MASK (u16)(0x9F1F)
#define HID_NEAREMPTY_MASK (u8)(0x80)

// Report handler prototypes (all take the output status buffer)
void __a1_20_status_report(u8 chan, u8* data, WPADStatusEx* status);
void __a1_21_user_data(u8 chan, u8* data, WPADStatusEx* status);
void __a1_22_ack(u8 chan, u8* data, WPADStatusEx* status);
void __a1_30_data_type(u8 chan, u8* data, WPADStatusEx* status);
void __a1_31_data_type(u8 chan, u8* data, WPADStatusEx* status);
void __a1_32_data_type(u8 chan, u8* data, WPADStatusEx* status);
void __a1_33_data_type(u8 chan, u8* data, WPADStatusEx* status);
void __a1_34_data_type(u8 chan, u8* data, WPADStatusEx* status);
void __a1_35_data_type(u8 chan, u8* data, WPADStatusEx* status);
void __a1_36_data_type(u8 chan, u8* data, WPADStatusEx* status);
void __a1_37_data_type(u8 chan, u8* data, WPADStatusEx* status);
void __a1_3d_data_type(u8 chan, u8* data, WPADStatusEx* status);
void __a1_3e_data_type(u8 chan, u8* data, WPADStatusEx* status);
void __a1_3f_data_type(u8 chan, u8* data, WPADStatusEx* status);
void __a1_unused_report(void);

// Incoming report dispatch table (report ID 0x20..0x3F)
void (*__a1_input_reports_array[])(u8 chan, u8* data, WPADStatusEx* status) = {
    __a1_20_status_report, __a1_21_user_data, __a1_22_ack,
    (void (*)(u8, u8*, WPADStatusEx*))__a1_unused_report,
    (void (*)(u8, u8*, WPADStatusEx*))__a1_unused_report,
    (void (*)(u8, u8*, WPADStatusEx*))__a1_unused_report,
    (void (*)(u8, u8*, WPADStatusEx*))__a1_unused_report,
    (void (*)(u8, u8*, WPADStatusEx*))__a1_unused_report,
    (void (*)(u8, u8*, WPADStatusEx*))__a1_unused_report,
    (void (*)(u8, u8*, WPADStatusEx*))__a1_unused_report,
    (void (*)(u8, u8*, WPADStatusEx*))__a1_unused_report,
    (void (*)(u8, u8*, WPADStatusEx*))__a1_unused_report,
    (void (*)(u8, u8*, WPADStatusEx*))__a1_unused_report,
    (void (*)(u8, u8*, WPADStatusEx*))__a1_unused_report,
    (void (*)(u8, u8*, WPADStatusEx*))__a1_unused_report,
    (void (*)(u8, u8*, WPADStatusEx*))__a1_unused_report,
    __a1_30_data_type, __a1_31_data_type, __a1_32_data_type, __a1_33_data_type,
    __a1_34_data_type, __a1_35_data_type, __a1_36_data_type, __a1_37_data_type,
    (void (*)(u8, u8*, WPADStatusEx*))__a1_unused_report,
    (void (*)(u8, u8*, WPADStatusEx*))__a1_unused_report,
    (void (*)(u8, u8*, WPADStatusEx*))__a1_unused_report,
    (void (*)(u8, u8*, WPADStatusEx*))__a1_unused_report,
    (void (*)(u8, u8*, WPADStatusEx*))__a1_unused_report,
    __a1_3d_data_type, __a1_3e_data_type, __a1_3f_data_type,
};

// Debug message table; DEBUGPrint strings are addressed relative to the
// report dispatch array, so they must live immediately after it in .data.
// (The 0x438/0x44C regions are separate .data objects referenced directly.)
static char __wpadDebugStrings[0x3B8] = {
    /* 0x080 */ "detaching extension during initialization."
    /* 0x0AA */ "\0"
    /* 0x0AC */ "Dpd Setting is ok."
    /* 0x0BE */ "\0"
    /* 0x0C0 */ "Dpd Setting is broken."
    /* 0x0D6 */ "\0"
    /* 0x0D8 */ "Old Firmware uses default values because it has no Dpd Setting."
    /* 0x117 */ "\0\0\0\0"
    /* 0x11C */ "x = %lf, y = %lf"
    /* 0x12C */ "\0\0\0"
    /* 0x130 */ "difaveX = %lf, difaveY = %lf"
    /* 0x14C */ "\0\0\0"
    /* 0x150 */ "deltaX  = %lf, deltaY  = %lf"
    /* 0x16C */ "\0\0\0"
    /* 0x170 */ "_wpadCalibrationX = %lf, _wpadCalibrationY = %lf"
    /* 0x1A0 */ "\0\0\0"
    /* 0x1A4 */ "centX  = %lf, centY  = %lf"
    /* 0x1BE */ "\0"
    /* 0x1C0 */ "_wpadRolag  = %lf"
    /* 0x1D1 */ "\0\0"
    /* 0x1D4 */ "Acc is ok."
    /* 0x1DE */ "\0"
    /* 0x1E0 */ "Acc is broken."
    /* 0x1EE */ "\0"
    /* 0x1F0 */ "0G:  accX = %d,  accY = %d,  accZ = %d"
    /* 0x216 */ "\0"
    /* 0x218 */ "1G:  accX = %d,  accY = %d,  accZ = %d"
    /* 0x23E */ "\0"
    /* 0x240 */ "Volume: %d,   Motor: %d"
    /* 0x257 */ "\0\0\0\0"
    /* 0x25C */ "check sum error."
    /* 0x26C */ "\0\0\0"
    /* 0x270 */ "0G:  fsaccX = %d,  fsaccY = %d,  fsaccZ = %d"
    /* 0x29C */ "\0\0\0"
    /* 0x2A0 */ "1G:  fsaccX = %d,  fsaccY = %d,  fsaccZ = %d"
    /* 0x2CC */ "\0\0\0"
    /* 0x2D0 */ "FS:  X = %d,  X max = %d,  X min = %d"
    /* 0x2F5 */ "\0\0"
    /* 0x2F8 */ "FS:  Y = %d,  Y max = %d,  Y min = %d"
    /* 0x31D */ "\0\0"
    /* 0x320 */ "CL:  X = %d,  X max = %d,  X min = %d"
    /* 0x345 */ "\0\0"
    /* 0x348 */ "CL:  Y = %d,  Y max = %d,  Y min = %d"
    /* 0x36D */ "\0\0"
    /* 0x370 */ "CR:  X = %d,  X max = %d,  X min = %d"
    /* 0x395 */ "\0\0"
    /* 0x398 */ "CR:  Y = %d,  Y max = %d,  Y min = %d"
    /* 0x3BD */ "\0\0"
    /* 0x3C0 */ "LR:  L = %d,  R = %d"
    /* 0x3D4 */ "\0\0\0"
    /* 0x3D8 */ "devId: %d, subId: %d"
    /* 0x3EC */ "\0\0\0"
    /* 0x3F0 */ "type : %d"
    /* 0x3F9 */ "\0\0"
    /* 0x3FC */ "mode : %d"
    /* 0x405 */ "\0\0"
    /* 0x408 */ "last : %d"
    /* 0x411 */ "\0\0"
    /* 0x414 */ "Keep calibration!"
    /* 0x425 */ "\0\0"
    /* 0x428 */ "Recalibration!"
    /* 0x436 */ "\0"
};

static char __wpadDebugString20[0x14] = "Received report 20";

static char __wpadDebugStrings2[0x124] = {
    /* 0x44C */ "initialize attachment"
    /* 0x461 */ "\0\0"
    /* 0x464 */ "read error happens!"
    /* 0x477 */ "\0\0\0\0"
    /* 0x47C */ "base addr: %08x"
    /* 0x48B */ "\0\0\0\0"
    /* 0x490 */ "length   : %d"
    /* 0x49D */ "\0\0"
    /* 0x4A0 */ "i2c = %04x"
    /* 0x4AA */ "\0"
    /* 0x4AC */ "enc = %d"
    /* 0x4B4 */ "\0\0\0"
    /* 0x4B8 */ "Access to extension register."
    /* 0x4D5 */ "\0\0"
    /* 0x4D8 */ "Decode!!!!"
    /* 0x4E2 */ "\0"
    /* 0x4E4 */ "    len = %d, addr = %04x"
    /* 0x4FD */ "\0\0"
    /* 0x500 */ "received data is out of range!"
    /* 0x51E */ "\0"
    /* 0x520 */ "Received ack!"
    /* 0x52D */ "\0\0"
    /* 0x530 */ "ack --> report ID = %02x, error code = %d"
    /* 0x559 */ "\0\0"
    /* 0x55C */ "invalid ack!"
    /* 0x568 */ "\0\0\0\0\0\0\0",
};

// Debug message accessors (strings are stored after the report array)
#define WPAD_DBG_MSG(off) ((const char*)__a1_input_reports_array + (off))

// Sensor bar / DPD calibration data, per channel
static f32 _wpadCalibrationX[5][WPAD_MAX_CONTROLLERS];

// Per-channel extension bookkeeping (sbss)
static u8 _wpadDevType[WPAD_MAX_CONTROLLERS];
static u8 _wpadDevMode[WPAD_MAX_CONTROLLERS];
static u8 _wpadCLCompt[WPAD_MAX_CONTROLLERS];
static u8 _wpadExtInitRetryCnt[WPAD_MAX_CONTROLLERS];
static u8 _recv_3e[WPAD_MAX_CONTROLLERS];
static u8 _recv_3f[WPAD_MAX_CONTROLLERS];

// Raw extension data and its "invalid" marker
static u8 _wpadExtRawData[21];
static const u8 _cExtInvalidData[21] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
};

// Vitality Sensor extension data (overlays the tail of WPADStatusEx)
typedef struct WPADVSStatus {
    u16 button;                          // at 0x00
    s16 accX;                            // at 0x02
    s16 accY;                            // at 0x04
    s16 accZ;                            // at 0x06
    DPDObject obj[WPAD_MAX_DPD_OBJECTS]; // at 0x08
    u8 dev;                              // at 0x28
    s8 err;                              // at 0x29

    s16 pulse0;  // at 0x2A
    s16 pulse1;  // at 0x2C
    s16 pulse2;  // at 0x2E
    s16 pulse3;  // at 0x30
    s16 pulse4;  // at 0x32
    u8 quality;  // at 0x34

    s16 wave1a; // at 0x36
    s16 wave1b; // at 0x38
    s16 wave1c; // at 0x3A
    s16 wave1d; // at 0x3C
    s16 wave1e; // at 0x3E
    u8 wave1f;  // at 0x40

    s16 wave2a; // at 0x42
    s16 wave2b; // at 0x44
    u8 wave2c;  // at 0x46
    u8 wave2d;  // at 0x47
    u8 wave2e;  // at 0x48
    u8 wave2f;  // at 0x49
} WPADVSStatus;

void __wpadAbortInitExtension(s32 chan, s32 result);
void __wpadGetDevConfig(s32 chan, s32 err);
void __wpadGetExtConfig(s32 chan, s32 err);
void __wpadGetExtType(s32 chan, s32 err);
void __wpadGetGameInfo(s32 chan, s32 err, u8 index);
s32 WPADiHIDParser(u8 port, u8* p_rpt);

void __parse_dpd_data(s32 chan, WPADStatusEx** status, u8 fmt, u8* data, s32 size);
void __parse_dpdex_data(s32 chan, WPADStatusEx** status, s32 index, u8* data, s32 size);
void __parse_cl_data(s32 chan, WPADStatusEx** status, s32 devMode, u8* data, s32 size);
void __parse_vs_data(s32 chan, WPADStatusEx** status, s32 dataFormat, u8* data, u32 size);

//
// DPD (IR camera) parsing
//

void __parse_dpd_data(s32 chan, WPADStatusEx** status, u8 fmt, u8* data, s32 size) {
    WPADCB* cb = __rvl_p_wpadcb[chan];
    u8 i;
    u8 x;
    u8 y;
    u8 xy;
    f32 dx;
    f32 dy;

    if (cb->dpdBusy) {
        (*status)->err = WPAD_ERR_INVALID;
        return;
    }

    if (fmt == WPAD_DPD_STANDARD) {
        for (i = 0; i < WPAD_MAX_DPD_OBJECTS; ++i) {
            if (i * 3 + 2 < size) {
                x = *(data + i * 3);
                y = *(data + i * 3 + 1);
                xy = *(data + i * 3 + 2);

                (*status)->obj[i].x =
                    (s16)((s16)((u16)x & 0xFF) | (u16)(((u16)xy & 0x30) << 4));
                (*status)->obj[i].y = (s16)((s16)(WPAD_DPD_IMG_RESO_WY - 1) -
                                            (s16)((s16)((u16)y & 0xFF) | (u16)(((u16)xy & 0xC0) << 2)));
                (*status)->obj[i].size = (u8)((xy & 0xF) & 0xFF);
                (*status)->obj[i].size =
                    (u8)((f32)(*status)->obj[i].size * (f32)(*status)->obj[i].size * PI);
                if ((*status)->obj[i].size == 0 || (*status)->obj[i].x == (s16)(WPAD_DPD_IMG_RESO_WX - 1) ||
                    (*status)->obj[i].y == (s16)(WPAD_DPD_IMG_RESO_WY - 1)) {
                    (*status)->obj[i].x = (s16)0;
                    (*status)->obj[i].y = (s16)(WPAD_DPD_IMG_RESO_WY - 1);
                    (*status)->obj[i].size = 0;
                }
                (*status)->obj[i].traceId = i;
            } else {
                (*status)->obj[i].x = (s16)0;
                (*status)->obj[i].y = (s16)(WPAD_DPD_IMG_RESO_WY - 1);
                (*status)->obj[i].size = 0;
                (*status)->obj[i].traceId = i;
            }
        }
    } else if (fmt == WPAD_DPD_BASIC) {
        for (i = 0; i < WPAD_MAX_DPD_OBJECTS; ++i) {
            x = *(data + i * 2);
            y = *(data + i * 2 + 1);
            xy = *(data + i * 3 + 2);

            if ((i % 2) == 0) {
                (*status)->obj[i].x =
                    (s16)((s16)((u16)x & 0xFF) | (u16)(((u16)xy & 0x30) << 4));
                (*status)->obj[i].y = (s16)((s16)(WPAD_DPD_IMG_RESO_WY - 1) -
                                            (s16)((s16)((u16)y & 0xFF) | (u16)(((u16)xy & 0xC0) << 2)));
            } else {
                (*status)->obj[i].x =
                    (s16)((s16)((u16)x & 0xFF) | (u16)(((u16)xy & 0x3) << 8));
                (*status)->obj[i].y = (s16)((s16)(WPAD_DPD_IMG_RESO_WY - 1) -
                                            (s16)((s16)((u16)y & 0xFF) | (u16)(((u16)xy & 0xC) << 6)));
            }
            if ((*status)->obj[i].x == (s16)(WPAD_DPD_IMG_RESO_WX - 1) ||
                (*status)->obj[i].y == (s16)(WPAD_DPD_IMG_RESO_WY - 1)) {
                (*status)->obj[i].x = (s16)0;
                (*status)->obj[i].y = (s16)(WPAD_DPD_IMG_RESO_WY - 1);
                (*status)->obj[i].size = 0;
            } else {
                (*status)->obj[i].size = 12;
            }
            (*status)->obj[i].traceId = i;
        }
    }

    // Rotate the object coordinates by the stored sensor-bar angle
    for (i = 0; i < WPAD_MAX_DPD_OBJECTS; ++i) {
        if ((*status)->obj[i].x == 0 && (*status)->obj[i].y == (s16)(WPAD_DPD_IMG_RESO_WY - 1)) {
            continue;
        }

        dx = (f32)(*status)->obj[i].x + _wpadCalibrationX[0][chan] - _wpadCalibrationX[2][chan];
        dy = (f32)(*status)->obj[i].y + _wpadCalibrationX[1][chan] - _wpadCalibrationX[3][chan];

        (*status)->obj[i].x = (s16)(s32)(_wpadCalibrationX[2][chan] +
            (dx * (f32)cos(-1.0f * _wpadCalibrationX[4][chan]) - dy * (f32)sin(-1.0f * _wpadCalibrationX[4][chan])));
        (*status)->obj[i].y = (s16)(s32)(_wpadCalibrationX[3][chan] +
            (dx * (f32)sin(-1.0f * _wpadCalibrationX[4][chan]) + dy * (f32)cos(-1.0f * _wpadCalibrationX[4][chan])));
    }
}

void __parse_dpdex_data(s32 chan, WPADStatusEx** status, s32 index, u8* data, s32 size) {
    (*status)->obj[(u8)index].x = (s16)((s16)((u16)data[0] & 0xFF) | (u16)(((u16)data[2] & 0x30) << 4));
    (*status)->obj[(u8)index].y = (s16)((s16)(WPAD_DPD_IMG_RESO_WY - 1) -
                                    (s16)((s16)((u16)data[1] & 0xFF) | (u16)(((u16)data[2] & 0xC0) << 2)));
    (*status)->exp[index].pixel = (s16)(((s16)((data[7] & 0xFF) << 8) | data[8]) & 0x3FF) << 6;
    (*status)->exp[index].radius = (u8)(data[2] & 0xF);

    (*status)->exp[index].range_x1 = (s16)(((s8)data[3] == -1) ? 0 : data[3]);
    (*status)->exp[index].range_y1 = (s16)(((s8)data[4] == -1) ? 0 : data[4]);
    (*status)->exp[index].range_x2 = (s16)(((s8)data[5] == -1) ? 0 : data[5]);
    (*status)->exp[index].range_y2 = (s16)(((s8)data[6] == -1) ? 0 : data[6]);

    (*status)->exp[index].range_x1 = (s16)((*status)->exp[index].range_x1 * 8);
    (*status)->exp[index].range_y1 = (s16)((s16)(WPAD_DPD_IMG_RESO_WY - 1) -
                                           (s16)((*status)->exp[index].range_y1 * 8));
    (*status)->exp[index].range_x2 = (s16)((*status)->exp[index].range_x2 * 8);
    (*status)->exp[index].range_y2 = (s16)((s16)(WPAD_DPD_IMG_RESO_WY - 1) -
                                           (s16)((*status)->exp[index].range_y2 * 8));

    (*status)->obj[(u8)index].size =
        (u16)((f32)(s8)(*status)->exp[index].radius * (f32)(s8)(*status)->exp[index].radius * PI);

    if ((*status)->obj[(u8)index].size == 0 || (*status)->obj[(u8)index].x == (s16)(WPAD_DPD_IMG_RESO_WX - 1) ||
        (*status)->obj[(u8)index].y == (s16)(WPAD_DPD_IMG_RESO_WY - 1) ||
        (*status)->exp[index].radius == 0xF) {
        (*status)->obj[(u8)index].x = (s16)0;
        (*status)->obj[(u8)index].y = (s16)(WPAD_DPD_IMG_RESO_WY - 1);
        (*status)->obj[(u8)index].size = 0;
        (*status)->exp[index].pixel = 0;
        (*status)->exp[index].radius = 0;
    }
    (*status)->obj[(u8)index].traceId = index;
}
//
// Classic Controller parsing
//

void __parse_cl_data(s32 chan, WPADStatusEx** status, s32 devMode, u8* data, s32 size) {
    WPADCB* cb = __rvl_p_wpadcb[chan];

    if (devMode == 2) {
        // Reduced mode: 10-bit sticks + 12 buttons
        ((WPADCLStatus*)(*status))->clLStickX =
            (s16)((s16)((s16)((s16)data[0] << 2) & (s16)0xFFFC) | (s16)((u16)data[4] & 0x3));
        ((WPADCLStatus*)(*status))->clRStickX =
            (s16)((s16)((s16)((s16)data[1] << 2) & (s16)0xFFFC) | (s16)(((u16)data[4] >> 2) & 0x3));
        ((WPADCLStatus*)(*status))->clLStickY =
            (s16)((s16)((s16)((s16)data[2] << 2) & (s16)0xFFFC) | (s16)(((u16)data[4] >> 4) & 0x3));
        ((WPADCLStatus*)(*status))->clRStickY =
            (s16)((s16)((s16)((s16)data[3] << 2) & (s16)0xFFFC) | (s16)(((u16)data[4] >> 6) & 0x3));
        ((WPADCLStatus*)(*status))->clTriggerL = data[5];
        ((WPADCLStatus*)(*status))->clTriggerR = (size >= 9) ? data[6] : 0;
        ((WPADCLStatus*)(*status))->clButton =
            (size >= 9) ? (u16)~(u16)((data[7] << 8) | data[8]) : 0;
    } else if (devMode == 3) {
        // Extended mode: 8-bit sticks + 12 buttons
        ((WPADCLStatus*)(*status))->clLStickX = (s16)((s16)data[0] << 2);
        ((WPADCLStatus*)(*status))->clRStickX = (s16)((s16)data[1] << 2);
        ((WPADCLStatus*)(*status))->clLStickY = (s16)((s16)data[2] << 2);
        ((WPADCLStatus*)(*status))->clRStickY = (s16)((s16)data[3] << 2);
        ((WPADCLStatus*)(*status))->clTriggerL = data[4];
        ((WPADCLStatus*)(*status))->clTriggerR = data[5];
        ((WPADCLStatus*)(*status))->clButton =
            (size >= 8) ? (u16)~(u16)((data[6] << 8) | data[7]) : 0;
    } else {
        // Standard mode: 6-bit sticks, reconstructed to 10-bit + extras
        ((WPADCLStatus*)(*status))->clLStickX = (s16)((u16)(data[0] & 0x3F) << 4);
        ((WPADCLStatus*)(*status))->clLStickY = (s16)((u16)(data[1] & 0x3F) << 4);
        ((WPADCLStatus*)(*status))->clRStickX = (s16)((s16)((s16)((s16)((s32)(s8)data[2] >> 7) & 0x1) |
                                                            (s16)(((u16)(data[1] >> 6) & 0x3) << 1) |
                                                            (s16)(((u16)(data[0] >> 6) & 0x3) << 3)) << 5);
        ((WPADCLStatus*)(*status))->clRStickY = (s16)((u16)(data[2] & 0x1F) << 5);
        ((WPADCLStatus*)(*status))->clTriggerL = (u8)((s8)((data[2] >> 2) & 0x7) << 5 | ((s8)data[3] >> 5) & 0x7);
        ((WPADCLStatus*)(*status))->clTriggerR = (u8)(data[3] & 0x1F) << 3;
        ((WPADCLStatus*)(*status))->clButton = (u16)~(u16)((data[4] << 8) | data[5]);
    }

    if (cb->devType == WPAD_DEV_CLASSIC) {
        ((WPADCLStatus*)(*status))->clLStickX -= 0x200;
        ((WPADCLStatus*)(*status))->clLStickY -= 0x200;
        ((WPADCLStatus*)(*status))->clRStickX -= 0x200;
        ((WPADCLStatus*)(*status))->clRStickY -= 0x200;
    } else {
        ((WPADCLStatus*)(*status))->clLStickX -= 0x200;
        ((WPADCLStatus*)(*status))->clLStickY -= 0x200;
    }

    if (cb->calibrated == 0) {
        cb->calibrated = 1;
        cb->extConfig.u.cl.lStickXCenter = ((WPADCLStatus*)(*status))->clLStickX;
        cb->extConfig.u.cl.lStickYCenter = ((WPADCLStatus*)(*status))->clLStickY;
        if (cb->devType == WPAD_DEV_CLASSIC) {
            cb->extConfig.u.cl.rStickXCenter = ((WPADCLStatus*)(*status))->clRStickX;
            cb->extConfig.u.cl.rStickYCenter = ((WPADCLStatus*)(*status))->clRStickY;
            cb->extConfig.u.cl.triggerLZero = ((WPADCLStatus*)(*status))->clTriggerL;
            cb->extConfig.u.cl.triggerRZero = ((WPADCLStatus*)(*status))->clTriggerR;
        } else {
            cb->extConfig.u.cl.rStickXCenter = 0;
            cb->extConfig.u.cl.rStickYCenter = 0;
            cb->extConfig.u.cl.triggerLZero = 0;
            cb->extConfig.u.cl.triggerRZero = 0;
        }
    }

    // Clamp stick/trigger deltas to +/-0x200 relative to the stored centers
    {
        s16 v;
        v = (s16)(((WPADCLStatus*)(*status))->clLStickX - cb->extConfig.u.cl.lStickXCenter);
        if (v < -0x200) {
            v = -0x200;
        }
        if (v > 0x1FF) {
            v = 0x1FF;
        }
        ((WPADCLStatus*)(*status))->clLStickX = v;

        v = (s16)(((WPADCLStatus*)(*status))->clLStickY - cb->extConfig.u.cl.lStickYCenter);
        if (v < -0x200) {
            v = -0x200;
        }
        if (v > 0x1FF) {
            v = 0x1FF;
        }
        ((WPADCLStatus*)(*status))->clLStickY = v;
    }

    if (cb->devType == WPAD_DEV_CLASSIC) {
        s16 v;
        v = (s16)(((WPADCLStatus*)(*status))->clRStickX - cb->extConfig.u.cl.rStickXCenter);
        if (v < -0x200) {
            v = -0x200;
        }
        if (v > 0x1FF) {
            v = 0x1FF;
        }
        ((WPADCLStatus*)(*status))->clRStickX = v;

        v = (s16)(((WPADCLStatus*)(*status))->clRStickY - cb->extConfig.u.cl.rStickYCenter);
        if (v < -0x200) {
            v = -0x200;
        }
        if (v > 0x1FF) {
            v = 0x1FF;
        }
        ((WPADCLStatus*)(*status))->clRStickY = v;

        v = (s16)(s8)(((WPADCLStatus*)(*status))->clTriggerL - cb->extConfig.u.cl.triggerLZero);
        if (v < 0) {
            v = 0;
        }
        if (v > 0xFF) {
            v = 0xFF;
        }
        ((WPADCLStatus*)(*status))->clTriggerL = (u8)v;

        v = (s16)(s8)(((WPADCLStatus*)(*status))->clTriggerR - cb->extConfig.u.cl.triggerRZero);
        if (v < 0) {
            v = 0;
        }
        if (v > 0xFF) {
            v = 0xFF;
        }
        ((WPADCLStatus*)(*status))->clTriggerR = (u8)v;
    }

    if (_wpadCLCompt[chan] != 0) {
        ((WPADCLStatus*)(*status))->clRStickX = 0;
        ((WPADCLStatus*)(*status))->clRStickY = 0;
        ((WPADCLStatus*)(*status))->clTriggerL = 0;
        ((WPADCLStatus*)(*status))->clTriggerR = 0;
    }
}

//
// Vitality Sensor parsing
//

void __parse_vs_data(s32 chan, WPADStatusEx** status, s32 dataFormat, u8* data, u32 size) {
    ((WPADVSStatus*)(*status))->wave1f = data[0];
    ((WPADVSStatus*)(*status))->wave1a =
        (s16)((data[6] >> 6) | ((data[1] << 2) & 0x3FC));
    ((WPADVSStatus*)(*status))->wave1b =
        (s16)(((data[2] << 2) & 0xFFFC) | ((data[6] >> 4) & 3));
    ((WPADVSStatus*)(*status))->wave1c =
        (s16)(((data[3] << 2) & 0xFFFC) | ((data[6] >> 2) & 3));
    ((WPADVSStatus*)(*status))->wave1d =
        (s16)(((data[4] << 2) & 0xFFFC) | (data[6] & 3));
    ((WPADVSStatus*)(*status))->wave1e =
        (s16)((data[7] >> 6) | ((data[5] << 2) & 0x3FC));
    ((WPADVSStatus*)(*status))->quality = data[8];

    ((WPADVSStatus*)(*status))->pulse0 =
        (s16)((data[14] >> 6) | ((data[9] << 2) & 0x3FC));
    ((WPADVSStatus*)(*status))->pulse1 =
        (s16)(((data[10] << 2) & 0xFFFC) | ((data[14] >> 4) & 3));
    ((WPADVSStatus*)(*status))->pulse2 =
        (s16)(((data[11] << 2) & 0xFFFC) | ((data[14] >> 2) & 3));
    ((WPADVSStatus*)(*status))->pulse3 =
        (s16)(((data[12] << 2) & 0xFFFC) | (data[14] & 3));
    ((WPADVSStatus*)(*status))->pulse4 =
        (s16)((data[15] >> 6) | ((data[13] << 2) & 0x3FC));
    ((WPADVSStatus*)(*status))->wave2a =
        (s16)(((data[7] << 4) & 0xF3F0) | ((data[15] >> 2) & 0xF));
    ((WPADVSStatus*)(*status))->wave2c = (u8)(data[15] & 0x3);

    if (size < 0x15) {
        ((WPADVSStatus*)(*status))->wave2b = 0;
        ((WPADVSStatus*)(*status))->wave2d = 0;
        ((WPADVSStatus*)(*status))->wave2e = 0;
        ((WPADVSStatus*)(*status))->wave2f = 0;
        return;
    }

    ((WPADVSStatus*)(*status))->wave2b =
        (s16)((data[17] >> 6) | ((data[16] << 2) & 0x3FC));
    ((WPADVSStatus*)(*status))->wave2d = (u8)(data[18] & 0x7);
    ((WPADVSStatus*)(*status))->wave2e = data[19];
    ((WPADVSStatus*)(*status))->wave2f = data[20];
}


//
// Core + accel report data
//

void __a1_30_data_type(u8 chan, u8* data, WPADStatusEx* status) {
    u16 rptBtn;
    WPADCB* cb;

    rptBtn = (u16)((data[2] << 8) | data[1]);
    cb = __rvl_p_wpadcb[chan];
    status->button = (u16)(rptBtn & HID_WPAD_BUTTON_MASK);
    status->dev = cb->devType;

    if (cb->dataFormat == WPAD_FMT_CORE_BTN) {
        status->err = WPAD_ERR_OK;
    } else {
        status->err = WPAD_ERR_INVALID;
    }

    cb->wpInfo.nearempty = (data[1] >> 7) & 1;
}

void __a1_31_data_type(u8 chan, u8* data, WPADStatusEx* status) {
    u16 rptBtn;
    WPADCB* cb;

    rptBtn = (u16)((data[2] << 8) | data[1]);
    cb = __rvl_p_wpadcb[chan];
    status->button = (u16)(rptBtn & HID_WPAD_BUTTON_MASK);
    status->dev = cb->devType;

    if (cb->dataFormat <= WPAD_FMT_CORE_BTN_ACC) {
        status->err = WPAD_ERR_OK;
    } else {
        status->err = WPAD_ERR_INVALID;
    }

    cb->wpInfo.nearempty = (data[1] >> 7) & 1;

    status->accX = (s16)((s16)((s16)((s16)((s16)((s16)data[3]) << 2) & (s16)0xFFFC) |
                             (s16)((s16)((u16)(data[1] >> 5)) & (s16)0x0003))) -
                   (s16)__rvl_p_wpadcb[chan]->devConfig.accX0g;
    status->accY = (s16)((s16)((s16)((s16)((s16)((s16)data[4]) << 2) & (s16)0xFFFC) |
                             (s16)((s16)((u16)(data[2] >> 4)) & (s16)0x0002))) -
                   (s16)__rvl_p_wpadcb[chan]->devConfig.accY0g;
    status->accZ = (s16)((s16)((s16)((s16)((s16)((s16)data[5]) << 2) & (s16)0xFFFC) |
                             (s16)((s16)((u16)(data[2] >> 5)) & (s16)0x0002))) -
                   (s16)__rvl_p_wpadcb[chan]->devConfig.accZ0g;
}

//
// Configuration reading from the Wiimote
//

void __wpadGetDevConfig(s32 chan, s32 err) {
    WPADCB* cb = __rvl_p_wpadcb[chan];
    u8* buf = cb->wmReadDataPtr;
    s32 i;
    s32 j;
    s32 sum;
    s32 index;
    DPDObject obj[WPAD_MAX_DPD_OBJECTS];
    s16 dummyObjX[WPAD_MAX_DPD_OBJECTS] = {0x7F, 0x380, 0x380, 0x7F};
    s16 dummyObjY[WPAD_MAX_DPD_OBJECTS] = {0x5D, 0x5D, 0x2A2, 0x2A2};
    f32 centX[WPAD_MAX_DPD_OBJECTS];
    f32 centY[WPAD_MAX_DPD_OBJECTS];
    f32 difaveX;
    f32 difaveY;
    f32 deltaX;
    f32 deltaY;
    f32 angle[WPAD_MAX_DPD_OBJECTS];
    f32 b[WPAD_MAX_DPD_OBJECTS];

    // Default DPD object positions
    for (i = 0; i < WPAD_MAX_DPD_OBJECTS; i++) {
        cb->devConfig.dpd[i].x = dummyObjX[i];
        cb->devConfig.dpd[i].y = dummyObjY[i];
        cb->devConfig.dpd[i].size = cb->defaultDpdSize;
        cb->devConfig.dpd[i].traceId = i;
    }

    // Default accelerometer calibration
    cb->devConfig.accX0g = 530;
    cb->devConfig.accY0g = 530;
    cb->devConfig.accZ0g = 530;
    cb->devConfig.accX1g = 636;
    cb->devConfig.accY1g = 636;
    cb->devConfig.accZ1g = 636;
    cb->devConfig.volume = 0;
    cb->devConfig.motor = 0;

    if (err != WPAD_ERR_OK) {
        return;
    }

    if (cb->configIndex == 0) {
        for (i = 0; i < 2; i++) {
            sum = 0;
            index = i * 11;
            for (j = index; j < index + 10; j++) {
                sum += buf[j];
            }

            if ((u8)(sum + 0x55) == buf[index + 10]) {
                DEBUGPrint(WPAD_DBG_MSG(0xAC));

                // Decode the four sensor-bar objects (basic format)
                obj[0].x = (s16)((s16)((u16)buf[index] & 0xFF) |
                                 (u16)((u16)(buf[index + 2] & 0x30) << 4));
                obj[0].y = (s16)((s16)(WPAD_DPD_IMG_RESO_WY - 1) -
                                 (s16)((s16)((u16)buf[index + 1] & 0xFF) | (u16)((u16)(buf[index + 2] & 0xC0) << 2)));
                obj[0].size = cb->defaultDpdSize;
                obj[0].traceId = 0;

                obj[1].x = (s16)((s16)((u16)buf[index + 3] & 0xFF) |
                                 (u16)((u16)(buf[index + 2] & 0x3) << 8));
                obj[1].y = (s16)((s16)(WPAD_DPD_IMG_RESO_WY - 1) -
                                 (s16)((s16)((u16)buf[index + 4] & 0xFF) | (u16)((u16)(buf[index + 2] & 0x3) << 6)));
                obj[1].size = cb->defaultDpdSize;
                obj[1].traceId = 1;

                obj[2].x = (s16)((s16)((u16)buf[index + 5] & 0xFF) |
                                 (u16)((u16)(buf[index + 7] & 0x30) << 4));
                obj[2].y = (s16)((s16)(WPAD_DPD_IMG_RESO_WY - 1) -
                                 (s16)((s16)((u16)buf[index + 6] & 0xFF) | (u16)((u16)(buf[index + 7] & 0xC0) << 2)));
                obj[2].size = cb->defaultDpdSize;
                obj[2].traceId = 2;

                obj[3].x = (s16)((s16)((u16)buf[index + 8] & 0xFF) |
                                 (u16)((u16)(buf[index + 7] & 0x3) << 8));
                obj[3].y = (s16)((s16)(WPAD_DPD_IMG_RESO_WY - 1) -
                                 (s16)((s16)((u16)buf[index + 9] & 0xFF) | (u16)((u16)(buf[index + 7] & 0x3) << 6)));
                obj[3].size = cb->defaultDpdSize;
                obj[3].traceId = 3;

                // Distribute the objects into the quadrant slots
                for (j = 0; j < WPAD_MAX_DPD_OBJECTS; j++) {
                    if (obj[j].x < 0x200 && obj[j].y < 0x180) {
                        cb->devConfig.dpd[0] = obj[j];
                    } else if (obj[j].x > 0x200 && obj[j].y < 0x180) {
                        cb->devConfig.dpd[1] = obj[j];
                    } else if (obj[j].x > 0x200 && obj[j].y > 0x180) {
                        cb->devConfig.dpd[2] = obj[j];
                    } else if (obj[j].x < 0x200 && obj[j].y > 0x180) {
                        cb->devConfig.dpd[3] = obj[j];
                    }
                }
                break;
            } else {
                DEBUGPrint(WPAD_DBG_MSG(0xC0));
            }
        }
    } else {
        DEBUGPrint(WPAD_DBG_MSG(0xD8));
    }

    // Sensor bar position / angle calibration
    difaveX = 0.0f;
    difaveY = 0.0f;

    for (i = 0; i < WPAD_MAX_DPD_OBJECTS; i++) {
        centX[i] = (f32)cb->devConfig.dpd[i].x;
        centY[i] = (f32)cb->devConfig.dpd[i].y;
        difaveX += (f32)(cb->devConfig.dpd[i].x - dummyObjX[i]);
        difaveY += (f32)(cb->devConfig.dpd[i].y - dummyObjY[i]);
        DEBUGPrint(WPAD_DBG_MSG(0x11C), centX[i], centY[i]);
    }
    difaveX *= 0.25f;
    difaveY *= 0.25f;

    deltaX = (f32)atan((126.5f + difaveX) / 1332.5f) - (f32)atan(126.5f / 1332.5f);
    deltaY = (f32)atan((93.0f + difaveY) / 1337.4f) - (f32)atan(93.0f / 1337.4f);

    _wpadCalibrationX[0][chan] = -1.0f * (f32)tan(deltaX) * 1332.5f;
    _wpadCalibrationX[1][chan] = -1.0f * (f32)tan(deltaY) * 1337.4f;

    DEBUGPrint(WPAD_DBG_MSG(0x130), difaveX, difaveY);
    DEBUGPrint(WPAD_DBG_MSG(0x150), deltaX, deltaY);
    DEBUGPrint(WPAD_DBG_MSG(0x170), _wpadCalibrationX[0][chan], _wpadCalibrationX[1][chan]);

    _wpadCalibrationX[2][chan] = 0.0f;
    _wpadCalibrationX[3][chan] = 0.0f;
    for (i = 0; i < WPAD_MAX_DPD_OBJECTS; i++) {
        centX[i] += _wpadCalibrationX[0][chan];
        centY[i] += _wpadCalibrationX[1][chan];
        _wpadCalibrationX[2][chan] += centX[i];
        _wpadCalibrationX[3][chan] += centY[i];
    }
    _wpadCalibrationX[2][chan] *= 0.25f;
    _wpadCalibrationX[3][chan] *= 0.25f;
    DEBUGPrint(WPAD_DBG_MSG(0x1A4), _wpadCalibrationX[2][chan], _wpadCalibrationX[3][chan]);

    // Sensor bar rotation angle ("rolag")
    _wpadCalibrationX[4][chan] = 0.0f;
    for (i = 0; i < WPAD_MAX_DPD_OBJECTS; i++) {
        angle[i] = (f32)atan((centY[i] - _wpadCalibrationX[2][chan]) /
                             (centX[i] - _wpadCalibrationX[3][chan]));
        b[i] = (f32)atan(((f32)dummyObjY[i] - 383.5f) / ((f32)dummyObjX[i] - 511.5f));
        _wpadCalibrationX[4][chan] += angle[i] - b[i];
    }
    _wpadCalibrationX[4][chan] *= 0.25f;
    DEBUGPrint(WPAD_DBG_MSG(0x1C0), _wpadCalibrationX[4][chan]);

    // Accelerometer calibration block
    for (i = 0; i < 2; i++) {
        sum = 0;
        index = i * 10;
        if (cb->configIndex == 0) {
            index += 22;
        }

        for (j = index; j < index + 9; j++) {
            sum += buf[j];
        }

        if ((u8)(sum + 0x55) == buf[index + 9]) {
            DEBUGPrint(WPAD_DBG_MSG(0x1D4));
            cb->devConfig.accX0g = (s16)((u16)(((u16)buf[index + 0]) << 2 & 0xFFFC) |
                                         (u16)(((u16)buf[index + 3]) >> 2 & 3));
            cb->devConfig.accY0g = (s16)((u16)(((u16)buf[index + 1]) << 2 & 0xFFFC) |
                                         (u16)(((u16)buf[index + 3]) >> 4 & 3));
            cb->devConfig.accZ0g = (s16)((u16)(((u16)buf[index + 2]) << 2 & 0xFFFC) |
                                         (u16)(((u16)buf[index + 3]) & 3));
            cb->devConfig.accX1g = (s16)((u16)(((u16)buf[index + 4]) << 2 & 0xFFFC) |
                                         (u16)(((u16)buf[index + 7]) >> 2 & 3));
            cb->devConfig.accY1g = (s16)((u16)(((u16)buf[index + 5]) << 2 & 0xFFFC) |
                                         (u16)(((u16)buf[index + 7]) >> 4 & 3));
            cb->devConfig.accZ1g = (s16)((u16)(((u16)buf[index + 6]) << 2 & 0xFFFC) |
                                         (u16)(((u16)buf[index + 7]) & 3));
            cb->devConfig.volume = (u8)(buf[index + 8] & 0x7F);
            cb->devConfig.motor = (u8)(buf[index + 8] & 0x80);
            break;
        } else {
            DEBUGPrint(WPAD_DBG_MSG(0x1E0));
        }
    }
    DEBUGPrint(WPAD_DBG_MSG(0x1F0), cb->devConfig.accX0g, cb->devConfig.accY0g, cb->devConfig.accZ0g);
    DEBUGPrint(WPAD_DBG_MSG(0x218), cb->devConfig.accX1g, cb->devConfig.accY1g, cb->devConfig.accZ1g);
    DEBUGPrint(WPAD_DBG_MSG(0x240), cb->devConfig.volume, cb->devConfig.motor);
}

void __wpadGetExtConfig(s32 chan, s32 err) {
    WPADCB* cb = __rvl_p_wpadcb[chan];
    u8* buf = cb->wmReadDataPtr;
    s32 i;
    s32 j;
    s32 sum;
    s32 index = -1;
    s32 k;

    if (err == -1) {
        cb->devType = 0xFD;
        cb->UNK_0x990 = 0xFD;
        cb->UNK_0x991 = 0;
    } else if (err == 0) {
        cb->devType = _wpadDevType[chan];
    } else {
        cb->devType = WPAD_DEV_NOT_SUPPORTED;
        cb->UNK_0x990 = WPAD_DEV_NOT_SUPPORTED;
        cb->UNK_0x991 = 0;
    }
    cb->devMode = _wpadDevMode[chan];

    if (cb->devType == WPAD_DEV_FREESTYLE || cb->devType == WPAD_DEV_CLASSIC) {
        for (i = 0; i < 2; i++) {
            sum = 0;
            k = i * 16;
            for (j = k; j < k + 14; j++) {
                sum += buf[j];
            }

            if ((u8)(sum + 0x55) == buf[k + 14] && (u8)(sum + 0xAA) == buf[k + 15]) {
                index = k;
            }
        }

        switch (cb->devType) {
        case WPAD_DEV_FREESTYLE:
            if (index < 0) {
                DEBUGPrint(WPAD_DBG_MSG(0x25C));
                cb->extConfig.u.fs.accX0g = 512;
                cb->extConfig.u.fs.accY0g = 512;
                cb->extConfig.u.fs.accZ0g = 512;
                cb->extConfig.u.fs.accX1g = 716;
                cb->extConfig.u.fs.accY1g = 716;
                cb->extConfig.u.fs.accZ1g = 716;
            } else {
                cb->extConfig.u.fs.accX0g = (s16)((u16)(((u16)buf[index + 0]) << 2 & 0xFFFC) |
                                                (u16)(((u16)buf[index + 3]) >> 2 & 3));
                cb->extConfig.u.fs.accY0g = (s16)((u16)(((u16)buf[index + 1]) << 2 & 0xFFFC) |
                                                (u16)(((u16)buf[index + 3]) >> 4 & 3));
                cb->extConfig.u.fs.accZ0g = (s16)((u16)(((u16)buf[index + 2]) << 2 & 0xFFFC) |
                                                (u16)(((u16)buf[index + 3]) & 3));
                cb->extConfig.u.fs.accX1g = (s16)((u16)(((u16)buf[index + 4]) << 2 & 0xFFFC) |
                                                (u16)(((u16)buf[index + 7]) >> 2 & 3));
                cb->extConfig.u.fs.accY1g = (s16)((u16)(((u16)buf[index + 5]) << 2 & 0xFFFC) |
                                                (u16)(((u16)buf[index + 7]) >> 4 & 3));
                cb->extConfig.u.fs.accZ1g = (s16)((u16)(((u16)buf[index + 6]) << 2 & 0xFFFC) |
                                                (u16)(((u16)buf[index + 7]) & 3));
                cb->extConfig.u.fs.at_0x04 = (s8)buf[index + 8];
                cb->extConfig.u.fs.at_0x02 = (s8)buf[index + 9];
                cb->extConfig.u.fs.at_0x0a = (s8)buf[index + 11];
                cb->extConfig.u.fs.at_0x08 = (s8)buf[index + 12];
                if (cb->calibrated == 0) {
                    cb->extConfig.u.fs.stickXCenter = (s8)buf[index + 10];
                    cb->extConfig.u.fs.stickYCenter = (s8)buf[index + 13];
                }

                DEBUGPrint(WPAD_DBG_MSG(0x270), cb->extConfig.u.fs.accX0g, cb->extConfig.u.fs.accY0g,
                           cb->extConfig.u.fs.accZ0g);
                DEBUGPrint(WPAD_DBG_MSG(0x2A0), cb->extConfig.u.fs.accX1g, cb->extConfig.u.fs.accY1g,
                           cb->extConfig.u.fs.accZ1g);
                DEBUGPrint(WPAD_DBG_MSG(0x2D0), cb->extConfig.u.fs.stickXCenter, cb->extConfig.u.fs.at_0x04,
                           cb->extConfig.u.fs.at_0x02);
                DEBUGPrint(WPAD_DBG_MSG(0x2F8), cb->extConfig.u.fs.stickYCenter, cb->extConfig.u.fs.at_0x0a,
                           cb->extConfig.u.fs.at_0x08);
            }
            break;

        case WPAD_DEV_CLASSIC:
            cb->extConfig.u.cl.at_0x04 = (s8)buf[index + 0];
            cb->extConfig.u.cl.at_0x02 = (s8)buf[index + 1];
            cb->extConfig.u.cl.at_0x0a = (s8)buf[index + 3];
            cb->extConfig.u.cl.at_0x08 = (s8)buf[index + 4];
            cb->extConfig.u.cl.at_0x10 = (s8)buf[index + 6];
            cb->extConfig.u.cl.at_0x0e = (s8)buf[index + 7];
            cb->extConfig.u.cl.at_0x16 = (s8)buf[index + 9];
            cb->extConfig.u.cl.at_0x14 = (s8)buf[index + 10];
            if (cb->calibrated == 0) {
                cb->extConfig.u.cl.lStickXCenter = (s8)buf[index + 2];
                cb->extConfig.u.cl.lStickYCenter = (s8)buf[index + 5];
                cb->extConfig.u.cl.rStickXCenter = (s8)buf[index + 8];
                cb->extConfig.u.cl.rStickYCenter = (s8)buf[index + 11];
                cb->extConfig.u.cl.triggerLZero = buf[index + 12];
                cb->extConfig.u.cl.triggerRZero = buf[index + 13];
            }

            DEBUGPrint(WPAD_DBG_MSG(0x320), cb->extConfig.u.cl.lStickXCenter, cb->extConfig.u.cl.at_0x04,
                       cb->extConfig.u.cl.at_0x02);
            DEBUGPrint(WPAD_DBG_MSG(0x348), cb->extConfig.u.cl.lStickYCenter, cb->extConfig.u.cl.at_0x0a,
                       cb->extConfig.u.cl.at_0x08);
            DEBUGPrint(WPAD_DBG_MSG(0x370), cb->extConfig.u.cl.rStickXCenter, cb->extConfig.u.cl.at_0x10,
                       cb->extConfig.u.cl.at_0x0e);
            DEBUGPrint(WPAD_DBG_MSG(0x398), cb->extConfig.u.cl.rStickYCenter, cb->extConfig.u.cl.at_0x16,
                       cb->extConfig.u.cl.at_0x14);
            DEBUGPrint(WPAD_DBG_MSG(0x3C0), cb->extConfig.u.cl.triggerLZero, cb->extConfig.u.cl.triggerRZero);
            break;
        }
    }

    if (!(cb->devType == WPAD_DEV_CLASSIC && cb->wmReadLength == 1) && cb->cmdBlkCB != NULL &&
        cb->cmdBlkCB == cb->extensionCB) {
        cb->cmdBlkCB(chan, cb->devType);
        cb->cmdBlkCB = NULL;
    }
}

void __wpadGetGameInfo(s32 chan, s32 err, u8 index) {
    WPADCB* cb = __rvl_p_wpadcb[chan];
    u8* buf = cb->wmReadDataPtr;
    s32 i;
    u32 sum = 0;

    if (err == WPAD_ERR_OK) {
        for (i = 0; i < 47; i++) {
            sum += buf[i];
        }

        if (buf[0x2F] == (u8)(sum + 0x55)) {
            memcpy(&cb->gameInfo, buf, sizeof(WPADGameInfo));
            cb->UNK_0x38[index] = WPAD_ERR_OK;
        } else {
            cb->UNK_0x38[index] = WPAD_ERR_INVALID;
        }
    } else {
        cb->UNK_0x38[index] = WPAD_ERR_INVALID;
    }
}

//
// Acknowledgement report
//

void __a1_22_ack(u8 chan, u8* data, WPADStatusEx* status) {
    WPADCB* cb = __rvl_p_wpadcb[chan];
    BOOL enable;
    u8 type;
    u8 err;
    s32 ackStatus;

    enable = OSDisableInterrupts();

    DEBUGPrint(WPAD_DBG_MSG(0x520));

    memcpy(status, cb->rxBufs[!cb->rxBufIndex], RX_BUFFER_SIZE);

    status->button =
        (u16)(((u16)((data[2] << 8) & 0xFF00) | (u16)(data[1] & 0xFF)) & HID_WPAD_BUTTON_MASK) |
        (u16)(status->button & 0x6000);

    if (status->dev != cb->devType) {
        status->dev = cb->devType;
        status->err = WPAD_ERR_INVALID;
    }

    type = data[3];
    err = data[4];
    ackStatus = (err == 0) ? WPAD_ERR_OK : WPAD_ERR_TRANSFER;

    DEBUGPrint(WPAD_DBG_MSG(0x530), type, err);

    if (cb->lastReportID == type) {
        if (cb->cmdBlkCB != NULL) {
            cb->cmdBlkCB(chan, ackStatus);
            cb->cmdBlkCB = NULL;
        }
        cb->status = WPAD_ERR_OK;
    } else {
        DEBUGPrint(WPAD_DBG_MSG(0x55C));
    }

    OSRestoreInterrupts(enable);
}

//
// Placeholder stubs for reports not yet matched
//

void __wpadAbortInitExtension(s32 chan, s32 result) {
    WPADCB* cb = __rvl_p_wpadcb[chan];
    u8 devType;

    if (result != 0) {
        WPADiClearQueue(&cb->extCmdQueue);
        cb->UNK_0x98D = 0;

        if (result == -1) {
            devType = 0xFD;
        } else if (cb->wpInfo.attach) {
            if (_wpadExtInitRetryCnt[chan]++ < 0x20) {
                WPADiClearQueue(&cb->extCmdQueue);
                WPADiSendSetReportType(&cb->extCmdQueue, cb->dataFormat, cb->UNK_0x98E,
                                       __wpadAbortInitExtension);
                cb->UNK_0x98D = 1;
                WPADiSendWriteDataCmd(&cb->extCmdQueue, 0x55, WM_REG_EXTENSION_F0,
                                      __wpadAbortInitExtension);
                WPADiSendWriteDataCmd(&cb->extCmdQueue, 0x0, WM_REG_EXTENSION_FB,
                                      __wpadAbortInitExtension);
                WPADiSendReadData(&cb->extCmdQueue, cb->wmReadDataBuf, 6, WM_REG_EXTENSION_FA,
                                  __wpadAbortInitExtension);
                return;
            }
            devType = 0xFC;
        } else {
            DEBUGPrint(WPAD_DBG_MSG(0x80));
            WPADiSendSetReportType(&cb->extCmdQueue, cb->dataFormat, cb->UNK_0x98E, NULL);
            devType = (u8)cb->dataFormat;
        }

        cb->devType = devType;
        cb->UNK_0x990 = devType;
        cb->UNK_0x991 = 0;

        if (cb->extensionCB != NULL) {
            cb->extensionCB(chan, devType);
        }
    }
}

void __wpadGetExtType(s32 chan, s32 err) {
    WPADCB* cb = __rvl_p_wpadcb[chan];
    u8* pExtId = cb->wmReadDataPtr;

    if (err == WPAD_ERR_OK) {
        DEBUGPrint(WPAD_DBG_MSG(0x3D8), pExtId[5], pExtId[0]);
        _wpadCLCompt[chan] = 0;
        _wpadDevMode[chan] = pExtId[4];

        switch (pExtId[5]) {
        case 0:
            _wpadDevType[chan] = 1;
            break;
        case 1:
            _wpadDevType[chan] = 2;
            break;
        case 2:
            _wpadDevType[chan] = WUDIsLinkedWBC() ? 3 : WPAD_DEV_NOT_FOUND;
            break;
        case 4:
            _wpadDevType[chan] = WPADIsEnabledVSM() ? 4 : WPAD_DEV_NOT_FOUND;
            break;
        case 0x10:
            _wpadDevType[chan] = WPADIsEnabledTRN() ? 0x10 : WPAD_DEV_NOT_FOUND;
            break;
        case 3:
            if (__OSInIPL != 0) {
                _wpadDevMode[chan] = 1;
                _wpadDevType[chan] = 2;
                _wpadCLCompt[chan] = 1;
            } else if (pExtId[0] == 0) {
                _wpadDevType[chan] = WPADIsEnabledGTR() ? 0x11 : WPAD_DEV_NOT_FOUND;
            } else if (pExtId[0] == 1) {
                _wpadDevType[chan] = WPADIsEnabledDRM() ? 0x12 : WPAD_DEV_NOT_FOUND;
            } else {
                _wpadDevType[chan] = WPAD_DEV_NOT_FOUND;
            }
            break;
        default:
            _wpadDevType[chan] = WPAD_DEV_NOT_FOUND;
            break;
        }

        if (_wpadDevType[chan] == WPAD_DEV_CLASSIC && (u8)(_wpadDevMode[chan] - 1) > 2) {
            _wpadDevType[chan] = WPAD_DEV_NOT_SUPPORTED;
        }

        DEBUGPrint(WPAD_DBG_MSG(0x3F0), _wpadDevType[chan]);
        DEBUGPrint(WPAD_DBG_MSG(0x3FC), _wpadDevMode[chan]);
        DEBUGPrint(WPAD_DBG_MSG(0x408), cb->UNK_0x990);

        if (cb->UNK_0x991 != 0 && cb->UNK_0x990 == _wpadDevType[chan] && cb->calibrated != 0) {
            DEBUGPrint(WPAD_DBG_MSG(0x414));
            cb->calibrated = 1;
        } else {
            DEBUGPrint(WPAD_DBG_MSG(0x428));
            cb->calibrated = 0;
        }

        cb->UNK_0x990 = _wpadDevType[chan];
        cb->UNK_0x991 = 0;

        if (_wpadDevType[chan] == WPAD_DEV_NOT_FOUND ||
            _wpadDevType[chan] == WPAD_DEV_NOT_SUPPORTED) {
            cb->devType = _wpadDevType[chan];
            cb->devMode = _wpadDevMode[chan];
            if (cb->extensionCB != NULL) {
                cb->extensionCB(chan, cb->devType);
            }
        } else {
            if ((_wpadDevType[chan] >= 1 && _wpadDevType[chan] <= 4) ||
                (_wpadDevType[chan] >= 0x11 && _wpadDevType[chan] <= 0x12)) {
                cb->UNK_0x98D = 2;
                WPADiCreateKey(chan);
            } else {
                cb->UNK_0x98D = 3;
                WPADiCreateKeyFor3rd(chan);
            }

            WPADiSendWriteDataCmd(&cb->extCmdQueue, 0xAA, WM_REG_EXTENSION_F0,
                                  __wpadAbortInitExtension);
            WPADiSendWriteData(&cb->extCmdQueue, cb->encryptionKey, 6, WM_REG_EXTENSION_40,
                               __wpadAbortInitExtension);
            WPADiSendWriteData(&cb->extCmdQueue, cb->encryptionKey + 6, 6,
                               WM_REG_EXTENSION_40 + 6, __wpadAbortInitExtension);
            WPADiSendWriteData(&cb->extCmdQueue, cb->encryptionKey + 12, 4,
                               WM_REG_EXTENSION_40 + 0xC, __wpadAbortInitExtension);

            if (_wpadDevType[chan] == 3) {
                WPADiSendWriteDataCmd(&cb->extCmdQueue, 0xAA, WM_REG_EXTENSION_CERT_CHALLENGE,
                                      __wpadAbortInitExtension);
                WPADiSendWriteDataCmd(&cb->extCmdQueue, 0xAA, WM_REG_EXTENSION_CERT_CHALLENGE,
                                      __wpadAbortInitExtension);
                WPADiSendWriteDataCmd(&cb->extCmdQueue, 0xAA, WM_REG_EXTENSION_CERT_CHALLENGE,
                                      __wpadAbortInitExtension);
            }

            WPADiSendReadData(&cb->extCmdQueue, cb->wmReadDataBuf, 0x20,
                              WM_REG_EXTENSION_CONFIG, cb->extensionCB);
        }
    } else {
        cb->devType = WPAD_DEV_NOT_SUPPORTED;
        cb->devMode = 0;
        cb->UNK_0x990 = WPAD_DEV_NOT_SUPPORTED;
        cb->UNK_0x991 = 0;
    }
}

s32 WPADiHIDParser(u8 port, u8* p_rpt) {
    WPADCB* cb = __rvl_p_wpadcb[port];
    WPADStatusEx* status;
    s32 result = WPAD_ERR_OK;

    if ((u8)(p_rpt[0] + 0xE0) <= 0x1F) {
        BOOL enable = OSDisableInterrupts();

        cb = __rvl_p_wpadcb[port];
        status = (WPADStatusEx*)cb->rxBufs[cb->rxBufIndex];

        if (p_rpt[0] != RPTID_DATA_BTN_ACC_DPD18_1 && p_rpt[0] != RPTID_DATA_BTN_ACC_DPD18_2) {
            memset(status, 0, RX_BUFFER_SIZE);
        }

        __a1_input_reports_array[p_rpt[0] - 0x20](port, p_rpt, status);

        if (cb->handshakeFinished == 0) {
            status->err = WPAD_ERR_INVALID;
        }

        if (p_rpt[0] != RPTID_DATA_BTN_ACC_DPD18_1 && p_rpt[0] != RPTID_DATA_BTN_ACC_DPD18_2) {
            cb->rxBufIndex = !cb->rxBufIndex;
        }

        OSRestoreInterrupts(enable);
        WPADiExcludeButton(port);
        WPADiCopyOut(port);
    } else {
        result = WPAD_ERR_NO_CONTROLLER;
    }

    return result;
}

void __a1_20_status_report(u8 chan, u8* data, WPADStatusEx* status) {
    WPADCB* cb = __rvl_p_wpadcb[chan];
    BOOL enable;
    s32 oldAttach = cb->wpInfo.attach;

    DEBUGPrint(__wpadDebugString20);

    enable = OSDisableInterrupts();

    if (cb->handshakeFinished == 0) {
        OSRestoreInterrupts(enable);
        return;
    }

    cb->wpInfo.attach = (data[3] & 0x2) >> 1;
    cb->wpInfo.lowBat = data[3] & 1;
    cb->wpInfo.led = (data[3] & 0xF0) >> 4;
    cb->wpInfo.protocol = 0;
    cb->wpInfo.firmware = data[5] & 0xF0;
    cb->wpInfo.nearempty = (data[1] & 0x80) >> 7;
    cb->wpInfo.dpd = (data[3] & 0x8) >> 3;
    cb->wpInfo.speaker = (data[3] & 0x4) >> 2;

    if (cb->devType == 3) {
        cb->wpInfo.battery = cb->UNK_0x98F;
    } else if (data[6] >= 0x55) {
        cb->wpInfo.battery = 4;
    } else if (data[6] >= 0x44) {
        cb->wpInfo.battery = 3;
    } else if (data[6] >= 0x33) {
        cb->wpInfo.battery = 2;
    } else if (data[6] >= 0x3) {
        cb->wpInfo.battery = 1;
    } else {
        cb->wpInfo.battery = 0;
    }

    if (cb->wpInfo.attach != 0) {
        if (oldAttach == 0) {
            DEBUGPrint(__wpadDebugStrings2);
            _wpadExtInitRetryCnt[chan] = 0;
            WPADiClearQueue(&__rvl_p_wpadcb[chan]->extCmdQueue);
            WPADiSendSetReportType(&__rvl_p_wpadcb[chan]->extCmdQueue,
                                   __rvl_p_wpadcb[chan]->dataFormat, __rvl_p_wpadcb[chan]->UNK_0x98E,
                                   __wpadAbortInitExtension);
            cb->UNK_0x98D = 1;
            WPADiSendWriteDataCmd(&__rvl_p_wpadcb[chan]->extCmdQueue, 0x55, WM_REG_EXTENSION_F0,
                                  __wpadAbortInitExtension);
            WPADiSendWriteDataCmd(&__rvl_p_wpadcb[chan]->extCmdQueue, 0x0, WM_REG_EXTENSION_FB,
                                  __wpadAbortInitExtension);
            WPADiSendReadData(&__rvl_p_wpadcb[chan]->extCmdQueue,
                              __rvl_p_wpadcb[chan]->wmReadDataBuf, 6, WM_REG_EXTENSION_FA,
                              __wpadAbortInitExtension);
            cb->devType = WPAD_DEV_UNKNOWN;
            cb->devMode = 0;
            if (cb->extensionCB != NULL) {
                cb->extensionCB(chan, WPAD_DEV_UNKNOWN);
            }
        }
    } else {
        cb->devType = WPAD_DEV_CORE;
        cb->devMode = 0;
        WPADiClearQueue(&cb->extCmdQueue);
        WPADiSendSetReportType(&cb->extCmdQueue, cb->dataFormat, cb->UNK_0x98E, NULL);
        if (oldAttach != 0) {
            cb->UNK_0x991 = 1;
            cb->UNK_0x992 = 0x12C;
            if (cb->extensionCB != NULL) {
                cb->extensionCB(chan, WPAD_DEV_CORE);
            }
        }
    }

    if (cb->wpInfoOut != NULL) {
        memcpy(cb->wpInfoOut, &cb->wpInfo, sizeof(WPADInfo));
        cb->wpInfoOut = NULL;
    }

    memcpy(status, __rvl_p_wpadcb[chan]->rxBufs[!__rvl_p_wpadcb[chan]->rxBufIndex], RX_BUFFER_SIZE);

    status->button =
        (u16)(((u16)((data[2] << 8) & 0xFF00) | (u16)(data[1] & 0xFF)) & HID_WPAD_BUTTON_MASK) |
        (u16)(status->button & 0x6000);

    if (status->dev != cb->devType) {
        status->dev = cb->devType;
        status->err = WPAD_ERR_INVALID;
    }

    if (cb->cmdBlkCB != NULL && cb->statusReqBusy != 0) {
        cb->cmdBlkCB(chan, WPAD_ERR_OK);
        cb->cmdBlkCB = NULL;
    }
    cb->statusReqBusy = 0;

    OSRestoreInterrupts(enable);
}

void __a1_21_user_data(u8 chan, u8* data, WPADStatusEx* status) {
    WPADCB* cb = __rvl_p_wpadcb[chan];
    BOOL enable;
    u16 addr;
    u16 dataAddr;
    u8 length;
    s16 offset;

    enable = OSDisableInterrupts();

    if ((data[3] & 0x0F) != 0) {
        DEBUGPrint(WPAD_DBG_MSG(0x464));
        cb->wmReadHadError = -1;

        if (cb->cmdBlkCB != NULL) {
            if (cb->extensionCB != cb->cmdBlkCB) {
                cb->cmdBlkCB(chan, WPAD_ERR_TRANSFER);
            }
            cb->cmdBlkCB = NULL;
        }
        cb->status = 0;
    }

    addr = (u16)cb->wmReadAddress;
    dataAddr = (u16)((data[4] << 8) | data[5]);
    length = (u8)((data[3] >> 4) + 1);
    offset = (s16)(dataAddr - addr);

    if (dataAddr < addr || dataAddr > (u16)(addr + cb->wmReadLength)) {
        DEBUGPrint(WPAD_DBG_MSG(0x500));
    } else {
        memcpy(cb->wmReadDataPtr + offset, data + 6, length);

        if ((u16)(addr + cb->wmReadLength) == dataAddr + length) {
            s32 err = (cb->wmReadHadError >> 31) & -3;

            DEBUGPrint(WPAD_DBG_MSG(0x47C), cb->wmReadAddress);
            DEBUGPrint(WPAD_DBG_MSG(0x490), cb->wmReadLength);
            DEBUGPrint(WPAD_DBG_MSG(0x4A0), cb->wmReadAddress >> 16, err);
            DEBUGPrint(WPAD_DBG_MSG(0x4AC), cb->UNK_0x98D);

            if ((cb->wmReadAddress >> 16) == 0x4A4) {
                DEBUGPrint(WPAD_DBG_MSG(0x4B8));

                if ((u8)(cb->UNK_0x98D - 2) <= 1) {
                    DEBUGPrint(WPAD_DBG_MSG(0x4D8));
                    DEBUGPrint(WPAD_DBG_MSG(0x4E4), length, addr);
                    WPADiDecode(chan, cb->wmReadDataPtr, length, addr);
                }
            }

            if ((cb->wmReadAddress == 0 && cb->configIndex == 0) ||
                (cb->wmReadAddress == 0x176C && cb->configIndex == 1)) {
                __wpadGetDevConfig(chan, err);
            }
            if (cb->wmReadAddress == WM_REG_EXTENSION_CONFIG) {
                __wpadGetExtConfig(chan, err);
            }
            if (cb->wmReadAddress == WM_REG_EXTENSION_FA) {
                __wpadGetExtType(chan, err);
            }
            if (cb->wmReadAddress == WM_ADDR_MEM_GAME_INFO_0) {
                __wpadGetGameInfo(chan, err, 0);
            }
            if (cb->wmReadAddress == WM_ADDR_MEM_GAME_INFO_1) {
                __wpadGetGameInfo(chan, err, 1);
            }

            if (cb->cmdBlkCB != NULL) {
                cb->cmdBlkCB(chan, err);
                cb->cmdBlkCB = NULL;
            }
            cb->status = 0;
        }
    }

    memcpy(status, cb->rxBufs[!cb->rxBufIndex], RX_BUFFER_SIZE);

    status->button =
        (u16)(((u16)((data[2] << 8) & 0xFF00) | (u16)(data[1] & 0xFF)) & HID_WPAD_BUTTON_MASK) |
        (u16)(status->button & 0x6000);

    if (status->dev != cb->devType) {
        status->dev = cb->devType;
        status->err = WPAD_ERR_INVALID;
    }

    OSRestoreInterrupts(enable);
}

void __a1_36_data_type(u8 chan, u8* data, WPADStatusEx* status) {
    WPADCB* cb = __rvl_p_wpadcb[chan];

    status->button = (u16)((data[2] << 8) | data[1]) & HID_WPAD_BUTTON_MASK;
    status->err = WPAD_ERR_INVALID;
    status->dev = cb->devType;

    cb->wpInfo.nearempty = (data[1] >> 7) & 1;

    __parse_dpd_data(chan, &status, cb->currentDpdCommand, data + 3, 10);

    memcpy(_wpadExtRawData, data + 13, 9);
    WPADiDecode(chan, data + 13, 9, 0);

    if (cb->wpInfo.attach && memcmp(_wpadExtRawData, _cExtInvalidData, 9) == 0 &&
        status->err == 0) {
        status->err = WPAD_ERR_CORRUPTED;
    }
}

void __a1_3d_data_type(u8 chan, u8* data, WPADStatusEx* status) {
    WPADCB* cb = __rvl_p_wpadcb[chan];

    if (cb->dataFormat == 0xE) {
        status->err = WPAD_ERR_OK;
    } else {
        status->err = WPAD_ERR_INVALID;
    }
    status->dev = cb->devType;

    memcpy(_wpadExtRawData, data + 1, 21);
    WPADiDecode(chan, data + 1, 21, 0);

    if (cb->wpInfo.attach) {
        if (cb->devType == 0x4) {
            __parse_vs_data(chan, &status, (u8)cb->dataFormat, data + 1, 21);
        }

        if (memcmp(_wpadExtRawData, _cExtInvalidData, 21) == 0 && status->err == 0) {
            status->err = WPAD_ERR_CORRUPTED;
        }
    }
}

void __a1_32_data_type(u8 chan, u8* data, WPADStatusEx* status) {
    WPADCB* cb = __rvl_p_wpadcb[chan];

    status->button = (u16)((data[2] << 8) | data[1]) & HID_WPAD_BUTTON_MASK;

    if (cb->dataFormat == 0 || cb->dataFormat == 3 || cb->dataFormat == 6 ||
        cb->dataFormat == 0xA) {
        status->err = WPAD_ERR_OK;
    } else {
        status->err = WPAD_ERR_INVALID;
    }
    status->dev = cb->devType;

    memcpy(_wpadExtRawData, data + 3, 8);
    cb->wpInfo.nearempty = (data[1] >> 7) & 1;
    WPADiDecode(chan, data + 3, 8, 0);

    if (cb->wpInfo.attach) {
        if (cb->devType == WPAD_DEV_FREESTYLE) {
            ((WPADFSStatus*)status)->fsStickX = data[3];
            ((WPADFSStatus*)status)->fsStickY = data[4];

            ((WPADFSStatus*)status)->fsAccX =
                (s16)((s16)((s16)((s16)((s16)((s16)data[5]) << 2) & (s16)0xFFFC) |
                         (s16)((s16)((u16)(data[8] >> 2)) & (s16)0x0003))) -
                (s16)cb->extConfig.u.fs.accX0g;
            ((WPADFSStatus*)status)->fsAccY =
                (s16)((s16)((s16)((s16)((s16)((s16)data[6]) << 2) & (s16)0xFFFC) |
                         (s16)((s16)((u16)(data[8] >> 4)) & (s16)0x0003))) -
                (s16)cb->extConfig.u.fs.accY0g;
            ((WPADFSStatus*)status)->fsAccZ =
                (s16)((s16)((s16)((s16)((s16)((s16)data[7]) << 2) & (s16)0xFFFC) |
                         (s16)((s16)((s8)data[8] >> 6) & (s16)0x0003))) -
                (s16)cb->extConfig.u.fs.accZ0g;

            ((WPADFSStatus*)status)->button =
                (u16)((u16)((WPADFSStatus*)status)->button |
                      (u16)((u16)((u16)(~(u16)data[8]) & (u16)0x3) << 13));

            if (cb->calibrated == 0) {
                cb->calibrated = 1;
                cb->extConfig.u.fs.stickXCenter =
                    (s8)((WPADFSStatus*)status)->fsStickX;
                cb->extConfig.u.fs.stickYCenter =
                    (s8)((WPADFSStatus*)status)->fsStickY;
            }

            {
                s16 v = (s16)((u8)((WPADFSStatus*)status)->fsStickX -
                               (u8)cb->extConfig.u.fs.stickXCenter);
                if (v < -0x80) {
                    v = -0x80;
                }
                if (v > 0x7F) {
                    v = 0x7F;
                }
                ((WPADFSStatus*)status)->fsStickX = (s8)v;
            }
            {
                s16 v = (s16)((u8)((WPADFSStatus*)status)->fsStickY -
                               (u8)cb->extConfig.u.fs.stickYCenter);
                if (v < -0x80) {
                    v = -0x80;
                }
                if (v > 0x7F) {
                    v = 0x7F;
                }
                ((WPADFSStatus*)status)->fsStickY = (s8)v;
            }
        } else if (cb->devType == WPAD_DEV_CLASSIC) {
            __parse_cl_data(chan, &status, cb->devMode, data + 3, 8);
        } else if (cb->devType == 0x10) {
            ((WPADTRStatus*)status)->brake = data[5];
            ((WPADTRStatus*)status)->mascon = data[6];
            ((WPADTRStatus*)status)->trButton =
                (u16)~(u16)((data[9] << 8) | data[10]);
        }

        if (memcmp(_wpadExtRawData, _cExtInvalidData, 8) == 0 && status->err == 0) {
            status->err = WPAD_ERR_CORRUPTED;
        }
    }
}

void __a1_33_data_type(u8 chan, u8* data, WPADStatusEx* status) {
    WPADCB* cb = __rvl_p_wpadcb[chan];

    status->button = (u16)((data[2] << 8) | data[1]) & HID_WPAD_BUTTON_MASK;
    status->dev = cb->devType;

    if (cb->dataFormat <= WPAD_FMT_CORE_BTN_ACC_DPD) {
        status->err = WPAD_ERR_OK;
    } else {
        status->err = WPAD_ERR_INVALID;
    }

    cb->wpInfo.nearempty = (data[1] >> 7) & 1;

    status->accX = (s16)((s16)((s16)((s16)((s16)((s16)data[3]) << 2) & (s16)0xFFFC) |
                         (s16)((s16)((u16)(data[1] >> 5)) & (s16)0x0003))) -
                   (s16)cb->devConfig.accX0g;
    status->accY = (s16)((s16)((s16)((s16)((s16)((s16)data[4]) << 2) & (s16)0xFFFC) |
                         (s16)((s16)((u16)(data[2] >> 4)) & (s16)0x0002))) -
                   (s16)cb->devConfig.accY0g;
    status->accZ = (s16)((s16)((s16)((s16)((s16)((s16)data[5]) << 2) & (s16)0xFFFC) |
                         (s16)((s16)((u16)(data[2] >> 5)) & (s16)0x0002))) -
                   (s16)cb->devConfig.accZ0g;

    __parse_dpd_data(chan, &status, cb->currentDpdCommand, data + 6, 12);
}

void __a1_34_data_type(u8 chan, u8* data, WPADStatusEx* status) {
    WPADCB* cb = __rvl_p_wpadcb[chan];

    status->button = (u16)((data[2] << 8) | data[1]) & HID_WPAD_BUTTON_MASK;

    if (cb->dataFormat == 0xC) {
        status->err = WPAD_ERR_OK;
    } else {
        status->err = WPAD_ERR_INVALID;
    }
    status->dev = cb->devType;

    memcpy(_wpadExtRawData, data + 3, 19);
    cb->wpInfo.nearempty = (data[1] >> 7) & 1;
    WPADiDecode(chan, data + 3, 19, 0);

    if (cb->wpInfo.attach) {
        if (cb->devType == 0x3) {
            ((WPADCLStatus*)status)->clButton = (u16)((data[3] << 8) | data[4]);
            ((WPADCLStatus*)status)->clLStickX = (s16)((data[5] << 8) | data[6]);
            ((WPADCLStatus*)status)->clLStickY = (s16)((data[7] << 8) | data[8]);
            ((WPADCLStatus*)status)->clRStickX = (s16)((data[9] << 8) | data[10]);
            ((WPADCLStatus*)status)->clRStickY = (s16)((data[13] << 8) | data[11]);

            if ((s32)(data[13] << 1) >= 0x104) {
                cb->UNK_0x98F = 4;
            } else if ((s32)(data[13] << 1) >= 0xFA) {
                cb->UNK_0x98F = 3;
            } else if ((s32)(data[13] << 1) >= 0xF0) {
                cb->UNK_0x98F = 2;
            } else if ((s32)(data[13] << 1) >= 0xD4) {
                cb->UNK_0x98F = 1;
            } else {
                cb->UNK_0x98F = 0;
            }

            if (cb->calibrated == 0) {
                cb->calibrated = 1;
            }
        }

        if (memcmp(_wpadExtRawData, _cExtInvalidData, 19) == 0 && status->err == 0) {
            status->err = WPAD_ERR_CORRUPTED;
        }
    }
}

void __a1_35_data_type(u8 chan, u8* data, WPADStatusEx* status) {
    WPADCB* cb = __rvl_p_wpadcb[chan];
    WPADCB* pCB;

    status->button = (u16)((data[2] << 8) | data[1]) & HID_WPAD_BUTTON_MASK;

    if (cb->dataFormat <= WPAD_FMT_CORE_BTN_ACC ||
        (cb->dataFormat >= WPAD_FMT_FS_BTN && cb->dataFormat <= WPAD_FMT_FS_BTN_ACC) ||
        (cb->dataFormat >= WPAD_FMT_CLASSIC_BTN && cb->dataFormat <= WPAD_FMT_CLASSIC_BTN_ACC) ||
        cb->dataFormat == 0xD) {
        status->err = WPAD_ERR_OK;
    } else {
        status->err = WPAD_ERR_INVALID;
    }
    status->dev = cb->devType;

    cb->wpInfo.nearempty = (data[1] >> 7) & 1;

    pCB = __rvl_p_wpadcb[chan];
    status->accX = (s16)((s16)((s16)((s16)((s16)((s16)data[3]) << 2) & (s16)0xFFFC) |
                             (s16)((s16)((u16)(data[1] >> 5)) & (s16)0x0003))) -
                   (s16)pCB->devConfig.accX0g;
    status->accY = (s16)((s16)((s16)((s16)((s16)((s16)data[4]) << 2) & (s16)0xFFFC) |
                             (s16)((s16)((u16)(data[2] >> 4)) & (s16)0x0002))) -
                   (s16)pCB->devConfig.accY0g;
    status->accZ = (s16)((s16)((s16)((s16)((s16)((s16)data[5]) << 2) & (s16)0xFFFC) |
                             (s16)((s16)((u16)(data[2] >> 5)) & (s16)0x0002))) -
                   (s16)pCB->devConfig.accZ0g;

    memcpy(_wpadExtRawData, data + 6, 0x10);
    WPADiDecode(chan, data + 6, 0x10, 0);

    if (cb->wpInfo.attach != 0) {
        if (cb->devType == WPAD_DEV_FREESTYLE) {
            pCB = __rvl_p_wpadcb[chan];
            ((WPADFSStatus*)status)->fsStickX = data[6];
            ((WPADFSStatus*)status)->fsStickY = data[7];

            ((WPADFSStatus*)status)->fsAccX =
                (s16)((s16)((s16)((s16)((s16)((s16)data[8]) << 2) & (s16)0xFFFC) |
                         (s16)((s16)((u16)(data[0xB] >> 2)) & (s16)0x0003))) -
                (s16)pCB->extConfig.u.fs.accX0g;
            ((WPADFSStatus*)status)->fsAccY =
                (s16)((s16)((s16)((s16)((s16)((s16)data[9]) << 2) & (s16)0xFFFC) |
                         (s16)((s16)((u16)(data[0xB] >> 4)) & (s16)0x0003))) -
                (s16)pCB->extConfig.u.fs.accY0g;
            ((WPADFSStatus*)status)->fsAccZ =
                (s16)((s16)((s16)((s16)((s16)((s16)data[0xA]) << 2) & (s16)0xFFFC) |
                         (s16)((s16)((s8)data[0xB] >> 6) & (s16)0x0003))) -
                (s16)pCB->extConfig.u.fs.accZ0g;

            ((WPADFSStatus*)status)->button =
                (u16)((u16)((WPADFSStatus*)status)->button |
                      (u16)((u16)((u16)(~(u16)data[0xB]) & (u16)0x3) << 13));

            if (pCB->calibrated == 0) {
                pCB->calibrated = 1;
                pCB->extConfig.u.fs.stickXCenter =
                    (s8)((WPADFSStatus*)status)->fsStickX;
                pCB->extConfig.u.fs.stickYCenter =
                    (s8)((WPADFSStatus*)status)->fsStickY;
            }

            {
                s16 v = (s16)((u8)((WPADFSStatus*)status)->fsStickX -
                               (u8)pCB->extConfig.u.fs.stickXCenter);
                if (v < -0x80) {
                    v = -0x80;
                }
                if (v > 0x7F) {
                    v = 0x7F;
                }
                ((WPADFSStatus*)status)->fsStickX = (s8)v;
            }
            {
                s16 v = (s16)((u8)((WPADFSStatus*)status)->fsStickY -
                               (u8)pCB->extConfig.u.fs.stickYCenter);
                if (v < -0x80) {
                    v = -0x80;
                }
                if (v > 0x7F) {
                    v = 0x7F;
                }
                ((WPADFSStatus*)status)->fsStickY = (s8)v;
            }
        } else if (cb->devType == WPAD_DEV_CLASSIC) {
            __parse_cl_data(chan, &status, cb->devMode, data + 6, 0x10);
        } else if (cb->devType == 4) {
            __parse_vs_data(chan, &status, (u8)cb->dataFormat, data + 6, 0x10);
        }

        if (memcmp(_wpadExtRawData, _cExtInvalidData, 0x10) == 0 && status->err == 0) {
            status->err = WPAD_ERR_CORRUPTED;
        }
    }
}

void __a1_37_data_type(u8 chan, u8* data, WPADStatusEx* status) {
    WPADCB* cb = __rvl_p_wpadcb[chan];
    WPADCB* pCB;
    WPADStatusEx* st = status;

    status->button = (u16)((data[2] << 8) | data[1]) & HID_WPAD_BUTTON_MASK;

    if (cb->dataFormat <= 8 || cb->dataFormat == 0xB || cb->dataFormat == 0xF) {
        status->err = WPAD_ERR_OK;
    } else {
        status->err = WPAD_ERR_INVALID;
    }
    status->dev = cb->devType;

    cb->wpInfo.nearempty = (data[1] >> 7) & 1;

    pCB = __rvl_p_wpadcb[chan];
    status->accX = (s16)((s16)((s16)((s16)((s16)((s16)data[3]) << 2) & (s16)0xFFFC) |
                             (s16)((s16)((u16)(data[1] >> 5)) & (s16)0x0003))) -
                   (s16)pCB->devConfig.accX0g;
    status->accY = (s16)((s16)((s16)((s16)((s16)((s16)data[4]) << 2) & (s16)0xFFFC) |
                             (s16)((s16)((u16)(data[2] >> 4)) & (s16)0x0002))) -
                   (s16)pCB->devConfig.accY0g;
    status->accZ = (s16)((s16)((s16)((s16)((s16)((s16)data[5]) << 2) & (s16)0xFFFC) |
                             (s16)((s16)((u16)(data[2] >> 5)) & (s16)0x0002))) -
                   (s16)pCB->devConfig.accZ0g;

    __parse_dpd_data(chan, &status, cb->currentDpdCommand, data + 6, 0xA);

    memcpy(_wpadExtRawData, data + 0x10, 6);
    WPADiDecode(chan, data + 0x10, 6, 0);

    if (cb->wpInfo.attach != 0) {
        if (cb->devType == WPAD_DEV_FREESTYLE) {
            pCB = __rvl_p_wpadcb[chan];
            ((WPADFSStatus*)st)->fsStickX = data[0x10];
            ((WPADFSStatus*)st)->fsStickY = data[0x11];

            ((WPADFSStatus*)st)->fsAccX =
                (s16)((s16)((s16)((s16)((s16)((s16)data[0x12]) << 2) & (s16)0xFFFC) |
                         (s16)((s16)((u16)(data[0x15] >> 2)) & (s16)0x0003))) -
                (s16)pCB->extConfig.u.fs.accX0g;
            ((WPADFSStatus*)st)->fsAccY =
                (s16)((s16)((s16)((s16)((s16)((s16)data[0x13]) << 2) & (s16)0xFFFC) |
                         (s16)((s16)((u16)(data[0x15] >> 4)) & (s16)0x0003))) -
                (s16)pCB->extConfig.u.fs.accY0g;
            ((WPADFSStatus*)st)->fsAccZ =
                (s16)((s16)((s16)((s16)((s16)((s16)data[0x14]) << 2) & (s16)0xFFFC) |
                         (s16)((s16)((s8)data[0x15] >> 6) & (s16)0x0003))) -
                (s16)pCB->extConfig.u.fs.accZ0g;

            ((WPADFSStatus*)st)->button =
                (u16)((u16)((WPADFSStatus*)st)->button |
                      (u16)((u16)((u16)(~(u16)data[0x15]) & (u16)0x3) << 13));

            if (pCB->calibrated == 0) {
                pCB->calibrated = 1;
                pCB->extConfig.u.fs.stickXCenter =
                    (s8)((WPADFSStatus*)st)->fsStickX;
                pCB->extConfig.u.fs.stickYCenter =
                    (s8)((WPADFSStatus*)st)->fsStickY;
            }

            {
                s16 v = (s16)((u8)((WPADFSStatus*)st)->fsStickX -
                               (u8)pCB->extConfig.u.fs.stickXCenter);
                if (v < -0x80) {
                    v = -0x80;
                }
                if (v > 0x7F) {
                    v = 0x7F;
                }
                ((WPADFSStatus*)st)->fsStickX = (s8)v;
            }
            {
                s16 v = (s16)((u8)((WPADFSStatus*)st)->fsStickY -
                               (u8)pCB->extConfig.u.fs.stickYCenter);
                if (v < -0x80) {
                    v = -0x80;
                }
                if (v > 0x7F) {
                    v = 0x7F;
                }
                ((WPADFSStatus*)st)->fsStickY = (s8)v;
            }
        } else if (cb->devType == WPAD_DEV_CLASSIC || cb->devType == 0x11 ||
                   cb->devType == 0x12) {
            __parse_cl_data(chan, &st, cb->devMode, data + 0x10, 6);
        }

        if (memcmp(_wpadExtRawData, _cExtInvalidData, 6) == 0 && status->err == 0) {
            status->err = WPAD_ERR_CORRUPTED;
        }
    }
}

void __a1_3e_data_type(u8 chan, u8* data, WPADStatusEx* status) {
    WPADCB* cb = __rvl_p_wpadcb[chan];
    BOOL enable;

    if (_recv_3e[chan] == 0 && _recv_3f[chan] == 0) {
        memset(status, 0, RX_BUFFER_SIZE);
    }

    status->button = (u16)((data[2] << 8) | data[1]) & HID_WPAD_BUTTON_MASK;

    if (cb->dataFormat <= WPAD_FMT_CORE_BTN_ACC || cb->dataFormat == 9) {
        status->err = WPAD_ERR_OK;
    } else {
        status->err = WPAD_ERR_INVALID;
    }
    status->dev = cb->devType;

    cb->wpInfo.nearempty = 0;

    status->accX = (s16)((s16)((s16)((s16)((s16)((s16)data[3]) << 2) & (s16)0xFFFC) |
                         (s16)((s16)((u16)(data[1] >> 6)) & (s16)0x0002))) -
                   (s16)cb->devConfig.accX0g;
    status->accZ = (s16)((s16)status->accZ |
                   (s16)((s16)((s16)((s16)((s16)data[2]) << 3) & (s16)0xFF00) |
                         (s16)((s16)((u16)(data[1] << 1)) & (s16)0x00C0)));

    __parse_dpdex_data(chan, &status, 0, data + 4, 0);
    __parse_dpdex_data(chan, &status, 1, data + 13, 0);

    enable = OSDisableInterrupts();
    _recv_3e[chan] = 1;

    if (_recv_3f[chan] != 0) {
        status->accZ = (s16)(status->accZ - cb->devConfig.accZ0g);
        cb->rxBufIndex = !cb->rxBufIndex;
        _recv_3f[chan] = 0;
        _recv_3e[chan] = 0;
    }

    OSRestoreInterrupts(enable);
}

void __a1_3f_data_type(u8 chan, u8* data, WPADStatusEx* status) {
    WPADCB* cb = __rvl_p_wpadcb[chan];
    BOOL enable;

    if (_recv_3e[chan] == 0 && _recv_3f[chan] == 0) {
        memset(status, 0, RX_BUFFER_SIZE);
    }

    status->button = (u16)((data[2] << 8) | data[1]) & HID_WPAD_BUTTON_MASK;

    if (cb->dataFormat <= WPAD_FMT_CORE_BTN_ACC || cb->dataFormat == 9) {
        status->err = WPAD_ERR_OK;
    } else {
        status->err = WPAD_ERR_INVALID;
    }
    status->dev = cb->devType;

    cb->wpInfo.nearempty = 0;

    status->accY = (s16)((s16)((s16)((s16)((s16)((s16)data[3]) << 2) & (s16)0xFFFC) |
                         (s16)((s16)((u16)(data[1] >> 6)) & (s16)0x0002))) -
                   (s16)cb->devConfig.accY0g;
    status->accZ = (s16)((s16)status->accZ |
                   (s16)((s16)((u16)(((u16)(data[1] >> 5) & 0x3) << 2)) |
                         (s16)((u16)(((u16)(data[2] >> 5) & 0x3) << 4))));

    __parse_dpdex_data(chan, &status, 2, data + 4, 0);
    __parse_dpdex_data(chan, &status, 3, data + 13, 0);

    enable = OSDisableInterrupts();
    _recv_3f[chan] = 1;

    if (_recv_3e[chan] != 0 && _recv_3f[chan] != 0) {
        status->accZ = (s16)(status->accZ - cb->devConfig.accZ0g);
        cb->rxBufIndex = !cb->rxBufIndex;
        _recv_3f[chan] = 0;
        _recv_3e[chan] = 0;
    }

    OSRestoreInterrupts(enable);
}

void __a1_unused_report(void) {}
