// Auto-scaffolded catalog TU for kyoshin/cf/CfTFile
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/cf/CfTFile.hpp"
#include "monolib/scn/CScnTimeApi.hpp"

#include "kyoshin/cf/CfGameManagerData.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include <decomp.h>
#include <math.h>
#include <string.h>

// The retail ctor symbol __ct__cf_CfTFile is a C-linkage name (no C++ mangling
// markers), so it is emitted as a global function with a manual vtable store
// (same convention as __ct__cf_CfGimmick / __ct__cf_CfGimmickSaveOff). The
// class carries a manual vptr at +0x00 (set to lbl_eu_80526F98).
CfTFile* __ct__cf_CfTFile(CfTFile* self) {
    *(void**)self = (void*)lbl_eu_80526F98;
    self->mFile = 0;
    self->mField82C = 0;
    self->mField830 = 0;
    // 32-byte aligned sub-buffer pointer inside the object.
    self->mDataAligned = (u8*)(((u32)self + 0x23) & ~0x1Fu);
    memset(&self->mData, 0, sizeof(cf::CfTFileData));
    return self;
}

// Cancel the active file (if any), wipe the payload and reset the file-event
// flag word down to its top nibble.
void func_800699B0(CfTFile* self) {
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
void func_80069A18(CfTFile* self) {
    if (self->mFile != 0) {
        CDeviceFile::cancel(self->mFile);
        self->mFile = 0;
        memset(&self->mData, 0, sizeof(cf::CfTFileData));
    }
    self->mField830 = 0;
    self->mField82C = 0;
}

// Copy the payload block and the current-entry counter from src.
void func_80069A78(CfTFile* dst, CfTFile* src) {
    if (src != 0) {
        memcpy(&dst->mData, &src->mData, sizeof(cf::CfTFileData));
        dst->mField830 = src->mField830;
    }
}

// Load a file-event archive identified by the packed token `param`. Cancels
// any in-flight read, rebuilds the file name (either through the packed-token
// formatter func_800AA33C when the token's low 10 bits are non-zero, or from
// the static format string otherwise), then opens the common archive through
// the device layer. Returns 1 on success (token recorded in mField830), 0 on
// failure (flag word gets its bit 2 set).
int func_80069ACC(CfTFile* self, u32 param) {
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
        func_800AA33C(buf, param, 1, 1);
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
    CDeviceFile::func_8044F154(self->mFile, 3);
    if (self->mFile == 0) {
        self->mField82C |= 4;
        return 0;
    }
    self->mField830 = param;
    return 1;
}

extern "C" u32 func_80069C14(u8* self) {
    u32 x = *(u32*)((u8*)self + 0x828);
    return !(0 - x);
}

// First non-null entry of the mData+0x400 file-event section.
cf::CfTFileEntry* func_80069C28(CfTFile* self) {
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
static inline cf::CfTFileEntry* findEventEntry(cf::CfTFileData* data, int key) {
    int i;
    u16 count;
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

cf::CfTFileEntry* func_80069C78(CfTFile* self, int key) {
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
int CfTFile::OnFileEvent(CEventFile* pEventFile) {
    int result = 0;
    if (this->mFile == pEventFile->mFileHandle) {
        if (pEventFile->unk0 == 1 && pEventFile->field_14 != 0) {
            this->mField82C |= 2;
            this->mDataAligned[pEventFile->field_14] = 0;
            func_800C1CAC((u32)this->mDataAligned, (u32)&this->mData);
        }
        this->mFile = 0;
        result = 1;
    }
    return result;
}

// Retail dtor mangling of the global-scope CfTFile class.
CfTFile* __dt__7CfTFileFv(CfTFile* obj, int flag) {
    if (obj != nullptr && flag > 0) {
        operator delete(obj);
    }
    return obj;
}

float func_80069EA0() { return lbl_eu_80661AF0; }

void func_80069EA8(int param) {
    extern float lbl_eu_80666214;
    extern float lbl_eu_80666218;
    unsigned char val = lbl_eu_80663DA0;
    if (param != 0) {
        val |= 0x02;
        lbl_eu_80663DA0 = val;
        lbl_eu_80661AF0 = lbl_eu_80666214;
    } else {
        val &= 0x02;
        lbl_eu_80663DA0 = val;
        lbl_eu_80661AF0 = lbl_eu_80666218;
    }
}

bool func_80069EE0() { return CDeviceVI::isTvFormatPal(); }

// Returns the play time, converting the u16 frame counter through the
// shared float state (seconds vs frame-count split). The (float) cast of the
// u16 emits the retail 2^52 double-magic conversion (fsubs, not fsub).
float func_80069EE4() {
    if (lbl_eu_80663D94 < lbl_eu_80663D98) {
        return lbl_eu_80661AF0 * (float)lbl_eu_80661AF8;
    }
    return lbl_eu_80663D94 - lbl_eu_80663D98;
}

// Per-frame play-clock update: refresh the frame-delta scalar from the bdat
// flag, advance the shared clock by the u16 frame counter (Kahan-style
// residual kept in lbl_eu_80663D9C), then roll the elapsed-time counters once
// the clock passes the display threshold.
void func_80069F2C() {
    if (CfRes_getD80Flag()) {
        // Retail reuses the flag call's r3 as the scene-pointer argument.
        lbl_eu_80661AF0 = func_80496288((void*)CfRes_getD80Flag());
    }
    int paused = lbl_eu_80663DA0 & 1;
    float oldClock = lbl_eu_80663D94;
    lbl_eu_80663D98 = oldClock;
    if (!paused) {
        // Advance the clock by frames*rate, keeping a Kahan-style residual
        // (what the addition lost) in lbl_eu_80663D9C. The u16 frame counter
        // converts through MWCC's inline 2^52 double-magic (unsigned path).
        float delta = lbl_eu_80661AF0 * (float)(u32)lbl_eu_80661AF8;
        float sum = lbl_eu_80663D9C + delta;
        // Kahan update through the shared clock global; the residual keeps
        // what the addition lost.
        lbl_eu_80663D94 = oldClock + sum;
        lbl_eu_80663D9C = sum - (lbl_eu_80663D94 - oldClock);
        oldClock = lbl_eu_80663D94;
    }
    if ((float)(double)oldClock >= lbl_eu_80666228) {
        // Elapsed-day / year counters roll over once the clock passes the
        // display threshold.
        if (++lbl_eu_80661AF4 > 365) {
            lbl_eu_80661AF4 = 1;
            if (++lbl_eu_80661AF6 > 9999) {
                lbl_eu_80661AF6 = 9999;
            }
        }
        // Discarded-calibration style wrap: keep only the fmod remainder,
        // reset the residual, store the residual constant first.
        float t = (float)fmod(lbl_eu_80663D94, lbl_eu_80666230);
        lbl_eu_80663D9C = lbl_eu_80666238;
        lbl_eu_80663D94 = t;
    }
}

extern "C" void func_8006A028(float v) {
    float c = lbl_eu_80666238;
    lbl_eu_80663D94 = v;
    lbl_eu_80663D98 = v;
    lbl_eu_80663D9C = c;
}

// Play-clock advance: report the current rates as truncated u16 frames, then
// fold the caller's two u32 counters into the shared float clock (the u32 ->
// float casts emit MWCC's inline 2^52 double-magic conversion).
void func_8006A03C(u32 a, u32 b) {
    int y, x;
    x = (int)(float)fmod(lbl_eu_80663D94 / lbl_eu_8066623C, lbl_eu_80666240);
    y = (int)(float)fmod(lbl_eu_80663D94 / lbl_eu_80666248, lbl_eu_80666250);
    // Discarded fmod: retail still performs the call.
    fmod(lbl_eu_80663D94, lbl_eu_80666230);
    func_8006A53C((u16)x, (u16)y);
    lbl_eu_80663D9C = lbl_eu_80666238;
    lbl_eu_80663D98 = lbl_eu_80663D94;
    lbl_eu_80663D94 =
        lbl_eu_8066623C * (float)a + lbl_eu_80666248 * (float)b;
}

// Advance the shared play clock by one frame: the delta is fmod'ing the
// current second counter against the 60 Hz frame rate; the caller-supplied
// value is folded in through the 2^52 double-magic (u32 -> float).
void func_8006A12C(u32 v) {
    float t = (float)fmod(lbl_eu_80663D94 / lbl_eu_80666248, lbl_eu_80666250);
    union {
        u32 w[2];
        double d;
    } conv;
    conv.w[1] = v;
    conv.w[0] = 0x43300000;
    lbl_eu_80663D9C = lbl_eu_80666238;
    lbl_eu_80663D94 = lbl_eu_8066623C * (float)(conv.d - lbl_eu_80666220) + lbl_eu_80666248 * t;
}

// Same clock advance as func_8006A12C, but the frame-delta term is the
// truncated integer frame count (u16) of the fmod result instead of the
// caller-supplied value. Both int-to-float conversions use direct builtin
// casts so MWCC emits the single-rounded 0x43300000 magic (fsubs) rather
// than the double-rounded manual-union form.
void func_8006A1A0(u32 v) {
    float t = (float)fmod(lbl_eu_80663D94 / lbl_eu_80666248, lbl_eu_80666250);
    u16 m = (u16)(int)t;
    lbl_eu_80663D9C = lbl_eu_80666238;
    lbl_eu_80663D94 =
        lbl_eu_8066623C * (float)(u16)(int)t + lbl_eu_80666248 * (float)v;
}

// Snapshot the current play-clock rates into two u16 frame counters, and
// return the truncated frame count of the raw second counter. Mirrors the
// func_8006A6D0 fmod trio; the caller-supplied pair is written through the
// out pointers.
u16 func_8006A234(u16* outA, u16* outB) {
    *outA = (u16)(int)(float)fmod(lbl_eu_80663D94 / lbl_eu_8066623C, lbl_eu_80666240);
    *outB = (u16)(int)(float)fmod(lbl_eu_80663D94 / lbl_eu_80666248, lbl_eu_80666250);
    return (u16)(int)(lbl_eu_80666258 * (float)fmod(lbl_eu_80663D94, lbl_eu_80666230));
}

// True when the current sub-second frame falls in the [6,17] window of the
// 60 Hz second (u16 wrap-around range check on the frame counter).
int func_8006A2E0() {
    int frame = (int)(float)fmod(lbl_eu_80663D94 / lbl_eu_8066623C, lbl_eu_80666240);
    int result = 0;
    // +0x10000 before -6 keeps MWCC from folding the offset into one addi
    // (0x10000 does not fit addi), matching retail's addis+subi pair.
    if ((u16)(frame + 0x10000 - 6) <= 11) {
        result = 1;
    }
    return result;
}

// Play-time -> frame conversions: fmod the shared second counter against a
// rate constant, round to int (frames).
int func_8006A33C() {
    return (int)(float)fmod(lbl_eu_80663D94 / lbl_eu_8066623C, lbl_eu_80666240);
}

int func_8006A37C() {
    return (int)(float)fmod(lbl_eu_80663D94 / lbl_eu_80666248, lbl_eu_80666250);
}

int func_8006A3BC() {
    return (int)(lbl_eu_80666258 * (float)fmod(lbl_eu_80663D94, lbl_eu_80666260));
}

u16 func_8006A3FC(u16 val) { lbl_eu_80661AF8 = val; return val; }

u16 func_8006A404() { return lbl_eu_80661AF8; }

// Resolve the current clock time from the bdat table: the column names for
// the given mode are picked from the string table, the cells read back as
// bytes (minutes*60 + seconds). Falls back to the static u16 tables when the
// bdat file or the column is unavailable.
int func_8006A40C(int mode) {
    func_8003AA34();
    u32 bdat = func_80086B2C__Q22cf13CfGameManagerFv();
    int row = (int)func_80086B1C__Q22cf13CfGameManagerFv();
    const char* pSec;
    const char* pMin;
    if (bdat != 0) {
        pMin = 0;
        pSec = 0;
        if (mode == 1) {
            pMin = lbl_eu_804FB420;
            pSec = lbl_eu_804FB420 + 0xa;
        } else if (mode == 2) {
            pMin = lbl_eu_804FB420 + 0x14;
            pSec = lbl_eu_804FB420 + 0x1e;
        } else if (mode == 3) {
            pMin = lbl_eu_804FB420 + 0x28;
            pSec = lbl_eu_804FB420 + 0x32;
        } else if (mode == 4) {
            pMin = lbl_eu_804FB420 + 0x3c;
            pSec = lbl_eu_804FB420 + 0x44;
        }
        if (pMin != 0) {
            // Both column pointers are fetched before either byte is read
            // (retail spills both results, then loads the bytes).
            char* vMin = (char*)getBdatStringColumnValue((void*)bdat, pMin, row);
            char* vSec = (char*)getBdatStringColumnValue((void*)bdat, pSec, row);
            return vMin[0] * 60 + vSec[0];
        }
    }
    return lbl_eu_80527030[mode] * 60 + lbl_eu_8052703C[mode];
}

// True when `target` (minutes-of-day) falls inside the minute window bounded
// by `hi` and `lo`; a window whose start is past its end (lo < hi) wraps
// around the 24-hour 0x5A0-minute day. The comparisons are unsigned u16
// arithmetic, so the 0x5A0 cycle boundary is handled without a full modulo.
static int inWindow(u32 target, u32 hi, u32 lo) {
    if (lo < hi) {
        if (target < (u32)(u16)(lo + 0x5a0)) return 1;
        if (target < hi) return 0;
        if (target >= 0x5a0) return 0;
        return 1;
    }
    if (target < hi) return 0;
    if (target >= lo) return 0;
    return 1;
}

// Select the play-time display mode for the given minute-of-day `target`
// (b + a*60). Fetches the four bdat time values times[1..4], builds a
// wrap-shifted copy sel[] = times[2..4],times[1], then scans the three
// consecutive minute windows with wrap-around indexing; returns 1..3 for a
// window hit and 4 when the target falls in none of them.
int func_8006A53C(u16 a, u16 b) {
    u32 target = (u16)(b + a * 60);
    u16 times[5];
    u16 sel[4];
    int i = 1;
    do {
        times[i] = (u16)func_8006A40C(i);
    } while (++i <= 4);
    for (i = 0; i < 4; i++) {
        int idx = i + 2;
        if (idx >= 5) idx = 1;
        sel[i] = times[idx];
    }
    int result = 1;
    for (i = 1; i < 5; i += 2) {
        if (inWindow(target, times[i], sel[i - 1])) return result;
        result++;
        if (inWindow(target, times[i + 1], sel[i])) return result;
        result++;
    }
    return 4;
}

// Truncated frame counts of the two play-time rates, passed to the
// file-event entry matcher as u16 pairs. The intermediate fmod against
// lbl_eu_80666230 is a discarded calibration call kept for side effects.
void func_8006A6D0() {
    int a = (int)(float)fmod(lbl_eu_80663D94 / lbl_eu_8066623C, lbl_eu_80666240);
    int b = (int)(float)fmod(lbl_eu_80663D94 / lbl_eu_80666248, lbl_eu_80666250);
    fmod(lbl_eu_80663D94, lbl_eu_80666230);
    func_8006A53C((u16)a, (u16)b);
}

// Advance the file-event frame timer (60 Hz sub-second / second / minute
// fields) once the PAL/NTSC frame-count gate is reached. The timer is stored
// back to the global after each carry stage, mirroring the retail stores.
void func_8006A75C() {
    lbl_eu_80663DA4++;
    int pal = CDeviceVI::isTvFormatPal();
    if (lbl_eu_80663DA4 >= (pal ? 25 : 30)) {
        lbl_eu_80663DA4 = 0;
        CfFrameTimer t;
        t.raw = lbl_eu_80663DA8;
        t.subsec++;
        lbl_eu_80663DA8 = t.raw;
        if (t.subsec >= 60) {
            t.subsec = 0;
            t.sec++;
            lbl_eu_80663DA8 = t.raw;
            if (t.sec >= 60) {
                t.sec = 0;
                lbl_eu_80663DA8 = t.raw;
                if (t.min < 9999) {
                    t.min++;
                    lbl_eu_80663DA8 = t.raw;
                }
            }
        }
    }
}

// Read-only getter for the file event frame timer.
u32 func_8006A80C() {
    return lbl_eu_80663DA8;
}

void func_8006A814(u32* self) {
    lbl_eu_80663DA8 = *self;
}

// Reset the file-event state: clear the shared counters/flags and restart the
// frame timer with only the minute counter preserved.
void func_eu_8006B238() {
    CfFrameTimer timer;
    timer.raw = lbl_eu_80663DA8;
    lbl_eu_80663D90 = 0;
    lbl_eu_80663D94 = lbl_eu_80666238;
    lbl_eu_80663D98 = lbl_eu_80666238;
    lbl_eu_80663D9C = lbl_eu_80666238;
    lbl_eu_80661AF4 = 1;
    lbl_eu_80661AF6 = 1;
    lbl_eu_80663DA0 = 0;
    timer.min = 0;
    timer.subsec = 0;
    timer.sec = 0;
    lbl_eu_80663DA8 = timer.raw;
    lbl_eu_80663DA4 = 0;
}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
extern "C" void sinit_8006A820() {
    lbl_eu_80663DA8 = 0;
}
