// Retail ~CfTFile elides the empty IWorkEvent base-dtor call.
#define IWORK_EVENT_INLINE_DTOR

#include "kyoshin/cf/CfTFile.hpp"
#include "monolib/scn/CScn.hpp"

#include "kyoshin/cf/CfGameManagerData.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include <decomp.h>
#include <math.h>
#include <string.h>

// Owner definitions of the file-event / play-clock state (retail .sbss/.sdata).
// The counter groups keep the retail symbol widths.
// Seconds-per-frame scale shared with CfResReloadImpl (retail global .sdata2
// object ahead of the TU's literal pool; local uses fold to the 30.0f literal).
// name-guess: gCfTFrameRate
extern const float gCfTFrameRate = 30.0f;
u32 gCfTEventFrameCounter;
f32 gCfTPlayClockSeconds;
f32 gCfTPlayClockPrevSeconds;
f32 gCfTPlayClockRemainder;
u32 gCfTClockDisplayFrame;
u32 gCfTFrameTimerRaw[2];
f32 gCfTPlayRate = 1.0f;
u16 gCfTCalendarDay = 1;
u16 gCfTCalendarYear = 1;
u16 gCfTFrameCounter[4] = {1, 0, 0, 0};
u16 gCfTFallbackHourTable[6] = {6, 12, 18, 0, 0, 0};
u16 gCfTFallbackMinuteTable[6] = {30, 30, 30, 30, 0, 0};

// Auto-scaffolded catalog TU for kyoshin/cf/CfTFile

cf::CfTFile::CfTFile() {
    mFile = 0;
    mField82C = 0;
    mField830 = 0;
    // 32-byte aligned sub-buffer pointer inside the object.
    mDataAligned = (u8*)(((u32)this + 0x23) & ~0x1Fu);
    memset(&mData, 0, sizeof(cf::CfTFileData));
}

// Cancel the active file (if any), wipe the payload and reset the file-event
// flag word down to its top nibble.
void CfTFile_CancelResetNibble(CfTFile* self) {
    if (self->mFile != 0) {
        CDeviceFile::cancel(self->mFile);
        self->mFile = 0;
        memset(&self->mData, 0, sizeof(cf::CfTFileData));
    }
    self->mField830 = 0;
    // Top-nibble mask keeps only bits 28-31 of the flag word (wrap-form
    // rlwinm encoding, matching the retail instruction).
    self->mField82C = DECOMP_PPC_RLWINM(self->mField82C, 0, 31, 28);
}

// Cancel the active file (if any), wipe the payload and reset the counters.
void CfTFile_CancelResetFull(CfTFile* self) {
    if (self->mFile != 0) {
        CDeviceFile::cancel(self->mFile);
        self->mFile = 0;
        memset(&self->mData, 0, sizeof(cf::CfTFileData));
    }
    self->mField830 = 0;
    self->mField82C = 0;
}

// Copy the payload block and the current-entry counter from src.
void CfTFile_CopyPayload(CfTFile* dst, CfTFile* src) {
    if (src != 0) {
        memcpy(&dst->mData, &src->mData, sizeof(cf::CfTFileData));
        dst->mField830 = src->mField830;
    }
}

// Load a file-event archive identified by the packed token `param`. Cancels
// any in-flight read, rebuilds the file name (either through the packed-token
// formatter Tok_FormatStr when the token's low 10 bits are non-zero, or from
// the static format string otherwise), then opens the common archive through
// the device layer. Returns 1 on success (token recorded in mField830), 0 on
// failure (flag word gets its bit 2 set).
int CfTFile_LoadEventArchive(CfTFile* self, u32 param) {
    if (self->mField830 == param) {
        return 1;
    }
    if (self->mFile != 0) {
        CDeviceFile::cancel(self->mFile);
        self->mFile = 0;
        memset(&self->mData, 0, sizeof(cf::CfTFileData));
    }
    self->mField830 = 0;
    // Clear the completed/error flags (bits 1-2) of the event flag word.
    self->mField82C &= ~6;

    // Decode the token's upper fields; the format path consumes them as
    // varargs, while the token formatter takes its own hard-coded flags.
    u32 field1 = (param >> 20) & 0x7F;
    u32 field2 = (param >> 10) & 0x3FF;

    ml::FixStr<64> buf(true);
    if ((param & 0x3FF) != 0) {
        Tok_FormatStr(buf, param, 1, 1);
    } else {
        buf.format(lbl_eu_804FB40C, field1, field2);
    }

    if (getFileSize__11CDeviceFileFPCc(buf.mString, 1) < 0) {
        self->mField82C |= 4;
        return 0;
    }

    memset(&self->mPad004, 0, sizeof(self->mPad004));
    self->mFile = CDeviceFile::readCommonArchiveFile(
        (mtl::ALLOC_HANDLE)(u32)self->mDataAligned, buf.mString,
        (IWorkEvent*)self, 0, 0);
    CDeviceFile::tryUpdateJobPriority(self->mFile, 3);
    if (self->mFile == 0) {
        self->mField82C |= 4;
        return 0;
    }
    self->mField830 = param;
    return 1;
}

struct CfTFile828View { u8 _pad[0x828]; u32 w828; };
extern "C" u32 CfTFile_TestField828(u8* self) {
    u32 x = ((CfTFile828View*)self)->w828;
    return !(0 - x);
}

// First non-null entry of the mData+0x400 file-event section.
cf::CfTFileEntry* CfTFile_FirstEntry3(CfTFile* self) {
    cf::CfTFileData* data = &self->mData;
    if (data != 0) {
        u16 count = data->mCount3;
        for (int i = 0; i < count; i++) {
            if (data->mEntries3[i].mField00 != 0) {
                return &data->mEntries3[i];
            }
        }
    }
    return 0;
}

// Match a file-event key against the five event tables (mEntries0..4) in
// order, returning a pointer to the first entry whose mKey equals `key`.
// Returns null when the key is zero or not present in any table.
// Retail inlines this section scan into the wrapper; keeping it as a
// parameterised helper preserves the unfused parameter guards.
// Match a file-event key against the five event tables (mEntries0..4) in
// order, returning a pointer to the first entry whose mKey equals `key`.
// Returns null when the key is zero or not present in any table.
// Retail inlines this section scan into the wrapper; keeping it as a
// parameterised helper preserves the unfused parameter guards.
static inline cf::CfTFileEntry* findEventEntry(cf::CfTFileData* data, int key) {
    u16 count;
    int i;
    if (data == 0) {
        return 0;
    }
    if (key != 0) {
        count = data->mCount0;
        for (i = 0; i < count; i++) {
            if (key == data->mEntries0[i].mKey) return &data->mEntries0[i];
        }
        count = data->mCount1;
        for (i = 0; i < count; i++) {
            if (key == data->mEntries1[i].mKey) return &data->mEntries1[i];
        }
        count = data->mCount2;
        for (i = 0; i < count; i++) {
            if (key == data->mEntries2[i].mKey) return &data->mEntries2[i];
        }
        count = data->mCount3;
        for (i = 0; i < count; i++) {
            if (key == data->mEntries3[i].mKey) return &data->mEntries3[i];
        }
        count = data->mCount4;
        for (i = 0; i < count; i++) {
            if (key == data->mEntries4[i].mKey) return &data->mEntries4[i];
        }
    }
    return 0;
}

cf::CfTFileEntry* CfTFile_FindEntryByKey(CfTFile* self, int key) {
    union {
        cf::CfTFileData* p;
        u32 w;
    } tables;
    tables.p = &self->mData;
    return findEventEntry(tables.p, key);
}

// Dispatch a completed file-event to the shared handler: fires only when the
// event's handle matches the active file and the payload byte is pending
// (field_14 non-zero), then clears the handle and reports success.
bool cf::CfTFile::OnFileEvent(CEventFile* pEventFile) {
    bool result = false;
    if (this->mFile == pEventFile->mFileHandle) {
        if (pEventFile->unk0 == 1 && pEventFile->field_14 != 0) {
            this->mField82C |= 2;
            this->mDataAligned[pEventFile->field_14] = 0;
            CmText_ParseWithStaticProc((u32)this->mDataAligned, (u32)&this->mData);
        }
        this->mFile = 0;
        result = true;
    }
    return result;
}

cf::CfTFile::~CfTFile() {}

float CfT_PlayRateGet() { return gCfTPlayRate; }

void func_80069EA8(int param) {
    if (param != 0) {
        unsigned char val = visionSharedStateFlags;
        float rate = 1.2f;
        val |= 0x02;
        visionSharedStateFlags = val;
        gCfTPlayRate = rate;
    } else {
        unsigned char val = visionSharedStateFlags;
        float rate = 1.0f;
        val &= 0x02;
        visionSharedStateFlags = val;
        gCfTPlayRate = rate;
    }
}

bool CfT_IsTvPal() { return CDeviceVI::isTvFormatPal(); }

// Returns the play time, converting the u16 frame counter through the
// shared float state (seconds vs frame-count split). The (float) cast of the
// u16 emits the retail 2^52 double-magic conversion (fsubs, not fsub).
float CfT_PlayTimeGet() {
    if (gCfTPlayClockSeconds < gCfTPlayClockPrevSeconds) {
        return gCfTPlayRate * (float)gCfTFrameCounter[0];
    }
    return gCfTPlayClockSeconds - gCfTPlayClockPrevSeconds;
}

// Per-frame play-clock update: refresh the frame-delta scalar from the bdat
// flag, advance the shared clock by the u16 frame counter (Kahan-style
// residual kept in gCfTPlayClockRemainder), then roll the elapsed-time counters once
// the clock passes the display threshold.
void CfT_PlayClockUpdate() {
    if (CfRes_getD80Flag()) {
        // Retail reuses the flag call's r3 as the scene-pointer argument.
        gCfTPlayRate = Scn_GetFrameDelta((void*)CfRes_getD80Flag());
    }
    int paused = visionSharedStateFlags & 1;
    float oldClock = gCfTPlayClockSeconds;
    gCfTPlayClockPrevSeconds = oldClock;
    if (!paused) {
        // Advance the clock by frames*rate, keeping a Kahan-style residual
        // (what the addition lost) in gCfTPlayClockRemainder. The u16 frame counter
        // converts through MWCC's inline 2^52 double-magic (unsigned path).
        float delta = gCfTPlayRate * (float)(u32)gCfTFrameCounter[0];
        float sum = gCfTPlayClockRemainder + delta;
        // Kahan update through the shared clock global; the residual keeps
        // what the addition lost.
        gCfTPlayClockSeconds = oldClock + sum;
        gCfTPlayClockRemainder = sum - (gCfTPlayClockSeconds - oldClock);
        oldClock = gCfTPlayClockSeconds;
    }
    if ((float)(double)oldClock >= 43200.0f) {
        // Elapsed-day / year counters roll over once the clock passes the
        // display threshold.
        if (++gCfTCalendarDay > 365) {
            gCfTCalendarDay = 1;
            if (++gCfTCalendarYear > 9999) {
                gCfTCalendarYear = 9999;
            }
        }
        // Discarded-calibration style wrap: keep only the fmod remainder,
        // reset the residual, store the residual constant first.
        float t = (float)fmod(gCfTPlayClockSeconds, 43200.0);
        gCfTPlayClockRemainder = 0.0f;
        gCfTPlayClockSeconds = t;
    }
}

extern "C" void CfT_PlayClockReset(float v) {
    float c = 0.0f;
    gCfTPlayClockSeconds = v;
    gCfTPlayClockPrevSeconds = v;
    gCfTPlayClockRemainder = c;
}

// Play-clock advance: report the current rates as truncated u16 frames, then
// fold the caller's two u32 counters into the shared float clock (the u32 ->
// float casts emit MWCC's inline 2^52 double-magic conversion).
void CfT_PlayClockAccumulate(u32 a, u32 b) {
    int y, x;
    x = (int)(float)fmod(gCfTPlayClockSeconds / 1800.0f, 24.0);
    y = (int)(float)fmod(gCfTPlayClockSeconds / 30.0f, 60.0);
    // Discarded fmod: retail still performs the call.
    fmod(gCfTPlayClockSeconds, 43200.0);
    CfT_SelectDisplayMode((u16)x, (u16)y);
    gCfTPlayClockRemainder = 0.0f;
    gCfTPlayClockPrevSeconds = gCfTPlayClockSeconds;
    gCfTPlayClockSeconds =
        1800.0f * (float)a + 30.0f * (float)b;
}

// Advance the shared play clock by one frame: the delta is fmod'ing the
// current second counter against the 60 Hz frame rate; the caller-supplied
// u32 folds in through MWCC's builtin u32 -> float conversion (plain 2^52
// magic, single fsubs, no xoris, no union). The A*t product is staged
// before the conversion so retail's fmuls-before-stack-lfd schedule holds.
void CfT_PlayClockAdvanceFrame(u32 v) {
    float t = (float)fmod(gCfTPlayClockSeconds / 30.0f, 60.0);
    gCfTPlayClockRemainder = 0.0f;
    gCfTPlayClockSeconds = 1800.0f * (float)v + 30.0f * t;
}

// Same clock advance as CfT_PlayClockAdvanceFrame, but the frame-delta term is the
// truncated integer frame count (u16) of the fmod result instead of the
// caller-supplied value. Both int-to-float conversions use direct builtin
// casts so MWCC emits the single-rounded 0x43300000 magic (fsubs) rather
// than the double-rounded manual-union form.
void CfT_PlayClockAdvanceInt(u32 v) {
    float t = (float)fmod(gCfTPlayClockSeconds / 30.0f, 60.0);
    u16 m = (u16)(int)t;
    gCfTPlayClockRemainder = 0.0f;
    gCfTPlayClockSeconds =
        1800.0f * (float)(u16)(int)t + 30.0f * (float)v;
}

// Snapshot the current play-clock rates into two u16 frame counters, and
// return the truncated frame count of the raw second counter. Mirrors the
// CfT_DispatchFrameCounts fmod trio; the caller-supplied pair is written through the
// out pointers.
u16 CfT_PlayClockSnapshot(u16* outA, u16* outB) {
    *outA = (u16)(int)(float)fmod(gCfTPlayClockSeconds / 1800.0f, 24.0);
    *outB = (u16)(int)(float)fmod(gCfTPlayClockSeconds / 30.0f, 60.0);
    return (u16)(int)(2.0f * (float)fmod(gCfTPlayClockSeconds, 43200.0));
}

// True when the current sub-second frame falls in the [6,17] window of the
// 60 Hz second (u16 wrap-around range check on the frame counter).
bool CfT_FrameInWindow() {
    int frame = (int)(float)fmod(gCfTPlayClockSeconds / 1800.0f, 24.0);
    bool result = false;
    // +0x10000 before -6 keeps MWCC from folding the offset into one addi
    // (0x10000 does not fit addi), matching retail's addis+subi pair.
    if ((u16)(frame + 0x10000 - 6) <= 11) {
        result = 1;
    }
    return result;
}

// Play-time -> frame conversions: fmod the shared second counter against a
// rate constant, round to int (frames).
int CfT_FrameCountA() {
    return (int)(float)fmod(gCfTPlayClockSeconds / 1800.0f, 24.0);
}

int CfT_FrameCountB() {
    return (int)(float)fmod(gCfTPlayClockSeconds / 30.0f, 60.0);
}

int CfT_FrameCountC() {
    return (int)(2.0f * (float)fmod(gCfTPlayClockSeconds, 30.0));
}

u16 CfT_FrameCounterStore(u16 val) { gCfTFrameCounter[0] = val; return val; }

u16 CfT_FrameCounterLoad() { return gCfTFrameCounter[0]; }

// Resolve the current clock time from the bdat table: the column names for
// the given mode are picked from the string table, the cells read back as
// bytes (minutes*60 + seconds). Falls back to the static u16 tables when the
// bdat file or the column is unavailable.
// name-guess: raw bdat cell word with its leading-byte view
union CfTBdatCell {
    u32 w;
    u8 b;
};

int CfT_GetClockTimeByMode(int mode) {
    Bdat_GetTable_AA34();
    u32 bdat = getGlobalPtr640A8__Q22cf13CfGameManagerFv();
    int row = (int)getGlobalWord64184Dup__Q22cf13CfGameManagerFv();
    const char* pSec;
    const char* pMin;
    if (bdat != 0) {
        pMin = 0;
        pSec = 0;
        if (mode == 1) {
            pMin = "morning_h";
            pSec = "morning_m";
        } else if (mode == 2) {
            pMin = "daytime_h";
            pSec = "daytime_m";
        } else if (mode == 3) {
            pMin = "evening_h";
            pSec = "evening_m";
        } else if (mode == 4) {
            pMin = "night_h";
            pSec = "night_m";
        }
        if (pMin != 0) {
            // The bdat cell word is left-justified: a u8 column value is the
            // word's leading byte, read back through the union.
            CfTBdatCell vMin;
            CfTBdatCell vSec;
            vMin.w = getBdatStringColumnValue((void*)bdat, pMin, row);
            vSec.w = getBdatStringColumnValue((void*)bdat, pSec, row);
            return vMin.b * 60 + vSec.b;
        }
    }
    return gCfTFallbackHourTable[mode] * 60 + gCfTFallbackMinuteTable[mode];
}

// Select the play-time display mode for the given minute-of-day `target`
// (b + a*60). Fetches the four bdat start times start[1..4] (1-based by
// mode), pairs each with the next mode's start end[i] (mode 4 wraps to 1),
// and returns the first mode whose [start, end) minute window holds the
// target; a window whose end is before its start wraps past the 0x5A0-minute
// day. Falls back to mode 4.
int CfT_SelectDisplayMode(u16 a, u16 b) {
    u16 target = b + a * 60;
    // Retail frame (0x40) reserves 10-entry tables; only [1..4] are used.
    u16 start[10];
    u16 end[10];
    int i;
    for (i = 1; i <= 4; i++) {
        start[i] = (u16)CfT_GetClockTimeByMode(i);
    }
    for (i = 1; i <= 4; i++) {
        int next = i + 1;
        if (next >= 5) next = 1;
        end[i] = start[next];
    }
    for (i = 1; i <= 4; i++) {
        if (end[i] < start[i]) {
            if (target < (u16)(end[i] + 0x5a0) || (target >= start[i] && target < 0x5a0)) return i;
        } else {
            if (target >= start[i] && target < end[i]) return i;
        }
    }
    return 4;
}

// Truncated frame counts of the two play-time rates, passed to the
// file-event entry matcher as u16 pairs. The intermediate fmod against
// the 43200.0 period is a discarded calibration call kept for side effects.
u32 CfT_DispatchFrameCounts() {
    int a = (int)(float)fmod(gCfTPlayClockSeconds / 1800.0f, 24.0);
    int b = (int)(float)fmod(gCfTPlayClockSeconds / 30.0f, 60.0);
    fmod(gCfTPlayClockSeconds, 43200.0);
    return CfT_SelectDisplayMode((u16)a, (u16)b);
}

// Advance the file-event frame timer (60 Hz sub-second / second / minute
// fields) once the PAL/NTSC frame-count gate is reached. The timer is stored
// back to the global after each carry stage, mirroring the retail stores.
void CfT_AdvanceFrameTimer() {
    gCfTClockDisplayFrame++;
    int pal = CDeviceVI::isTvFormatPal();
    if (gCfTClockDisplayFrame >= (pal ? 25 : 30)) {
        gCfTClockDisplayFrame = 0;
        CfFrameTimer t;
        t.raw = gCfTFrameTimerRaw[0];
        t.subsec++;
        gCfTFrameTimerRaw[0] = t.raw;
        if (t.subsec >= 60) {
            t.subsec = 0;
            t.sec++;
            gCfTFrameTimerRaw[0] = t.raw;
            if (t.sec >= 60) {
                t.sec = 0;
                gCfTFrameTimerRaw[0] = t.raw;
                if (t.min < 9999) {
                    t.min++;
                    gCfTFrameTimerRaw[0] = t.raw;
                }
            }
        }
    }
}

// Read-only getter for the file event frame timer.
u32 CfT_FrameTimerGet() {
    return gCfTFrameTimerRaw[0];
}

void CfT_FrameTimerSet(u32* self) {
    gCfTFrameTimerRaw[0] = *self;
}

// Reset the file-event state: clear the shared counters/flags and restart the
// frame timer with only the minute counter preserved.
void CfT_ResetEventState() {
    CfFrameTimer timer;
    timer.raw = gCfTFrameTimerRaw[0];
    gCfTEventFrameCounter = 0;
    gCfTPlayClockSeconds = 0.0f;
    gCfTPlayClockPrevSeconds = 0.0f;
    gCfTPlayClockRemainder = 0.0f;
    gCfTCalendarDay = 1;
    gCfTCalendarYear = 1;
    visionSharedStateFlags = 0;
    timer.min = 0;
    timer.subsec = 0;
    timer.sec = 0;
    gCfTFrameTimerRaw[0] = timer.raw;
    gCfTClockDisplayFrame = 0;
}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
extern "C" void sinit_8006A820() {
    gCfTFrameTimerRaw[0] = 0;
}
