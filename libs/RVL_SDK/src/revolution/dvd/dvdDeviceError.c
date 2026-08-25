#include <revolution/OS.h>
#include <revolution/DVD.h>
#include <revolution/SC.h>

extern u16 OSSetFontEncode(u16);
extern void __DVDShowFatalMessage(void);
static u8 CheckBuffer[32] ALIGN(32);

// Retail .sdata: lowDone (4) then 4 zero pad bytes to the next unit's
// 8-aligned .sdata (gap_09_8066307C_sdata).
struct LowDone {
    volatile BOOL done;
};
static struct LowDone lowDone = { TRUE };
static volatile u32 lowIntType = 0;

static void lowCallback(u32 intType) {
    lowIntType = intType;
    lowDone.done = TRUE;
}

void __DVDShowDeviceErrorMessage(void);

BOOL __DVDCheckDevice(void) {
    u32 checkCode = 0x460A0000;
    u32 outOfRangeError = 0xFFFFFFFF;
    u32 reportKeyError = 0xFFFFFFFF;
    OSIOSRev iosRev;

    if (OSGetPhysicalMem2Size() == 0x08000000) {
        return TRUE;
    }

    __OSGetIOSRev(&iosRev);
    
    if (iosRev.idLo < 30 || iosRev.idLo >= 254) {
        return TRUE;
    }

    if (OS_BOOT_PARTITION_319C == 129) {
        checkCode = 0x7ED40000;
    }

    lowDone.done = FALSE;
    DVDLowUnencryptedRead((void*)CheckBuffer, 32, checkCode, lowCallback);\

    while (!lowDone.done) {

    }

    switch (lowIntType) {
        case 2:
            break;
        case 1:
            goto invalid;
            break;
        default:
            goto fatal;
            break;
    }

    lowDone.done = FALSE;
    DVDLowRequestError(lowCallback);

    while (!lowDone.done) {

    }

    outOfRangeError = DVDLowGetImmBufferReg();

    switch (lowIntType) {
        case 1:
        {
            if ((DVDLowGetImmBufferReg() & 0xFF000000) != 0) {
                goto recover;
                break;
            }

            switch (DVDLowGetImmBufferReg() & 0xFFFFFF) {
                case 0x52100:
                    break;
                default:
                    goto invalid;
                    break;
            }

            break;
        }

        default:
            goto fatal;
            break;
    }

    lowDone.done = FALSE;
    DVDLowReportKey((struct DVDVideoReportKey*)CheckBuffer, 0x40000, 0, lowCallback);

    while (!lowDone.done) {

    }

    switch (lowIntType) {
        case 2:
            break;
        case 1:
            goto invalid;
            break;
        default:
            goto fatal;
            break;
    }

    lowDone.done = FALSE;
    DVDLowRequestError(lowCallback);

    while (!lowDone.done) {

    }

    reportKeyError = DVDLowGetImmBufferReg();
    switch (lowIntType) {
        case 1:
            if ((DVDLowGetImmBufferReg() & 0xFF000000) != 0) {
                goto recover;
                break;
            }

            switch (DVDLowGetImmBufferReg() & 0xFFFFFF) {
                case 0x53100:
                case 0x52000:
                    break;
                default:
                    goto invalid;
                    break;
            }

            break;

        default:
            goto fatal;
            break;
    }

valid:
    return TRUE;

invalid:
    __DVDShowDeviceErrorMessage();
    return FALSE;

recover:
    return FALSE;

fatal:
    __DVDShowFatalMessage();
    return FALSE;
}

const char* const __DVDDeviceErrorMessage[] = {
    "\n\n\nエラーコード００１。\n"
    "不明なデバイスが見つかりました。",

    "\n\n\nError #001,\n"
    "unauthorized device has been detected.",

    //"\n\n\nFehler #001:\nEs wurde eine unzul?ssige Komponente\nentdeckt.
    "\n\n\nFehler #001:\n"
    "Es wurde eine unzul\xE4ssige Komponente\n"
    "entdeckt.",

    //"\n\n\nErreur 001:\nun dispositif non autoris? a ?t? d?tect?."
    "\n\n\nErreur 001:\n"
    "un dispositif non autoris\xE9 a \xE9t\xE9 d\xE9tect\xE9.",

    "\n\n\nError 001:\n"
    "Se ha detectado un dispositivo no\n"
    "autorizado.",

    "\n\n\nErrore #001:\n"
    "rilevato un dispositivo non autorizzato.",

    // Dutch: retail pools the literal directly as the 7th pointer; the
    // message is 47 bytes with its NUL, ending .data at 0x197 (no pad).
    "\n\n\nFout #001:\nongeoorloofd onderdeel gevonden.",
};

static void __DVDShowDeviceErrorMessage(void) {
    const char* message;
    const char* const* messageList;
    GXColor bg = { 0, 0, 0, 0 };
    GXColor fg;
    // .sdata2 is 8 bytes in retail: the fg color word then 4 zero pad bytes
    // (gap_11_80669794_sdata2) aligning the next unit's .sdata2. The [2]
    // array is load-bearing: a plain const scalar folds to an immediate and
    // the retail .sdata2 slot disappears (codegen differs).
    static const u32 fgColor[2] = { 0xFFFFFF00, 0 };
    *(u32*)&fg = fgColor[0];

    if (SCGetLanguage() == 0) {
        OSSetFontEncode(1);
    } else {
        OSSetFontEncode(0);
    }

    messageList = __DVDDeviceErrorMessage;

    if (SCGetLanguage() > 6) {
        message = messageList[1];
    } else {
        message = messageList[SCGetLanguage()];
    }

    OSFatal(fg, bg, message);
}
