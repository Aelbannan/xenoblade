// CLibCriStreamingPlay - CRI streaming audio playback manager

// Suppress MWCC auto-vtable/typeinfo emissions: all class data ships from the
// dissolved retail blocks at the bottom of this TU (retail .rodata 0x80523050,
// .data 0x8056D028, .sdata 0x806637A0, .sbss 0x806656E8).
class __declspec(novtable) CLibCriStreamingPlay;

#include "monolib/lib/CLibCriStreamingPlay.hpp"
#include "monolib/core/CException.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkControl.hpp"
#include <cstring>

// 0x94-byte streaming slot; 5 slots live at instance+0x1C8 (stride 0x94).
// TU-local reconstruction (retail-verified offsets from CLibCriStreamingPlay.s).
struct StreamEntry {
    u32 slotId;      // 0x00 (0..4)
    s32  id;         // 0x04 (-1 = unused)
    u8*  adxt;       // 0x08
    u32 field_0x0C;
    u32 field_0x10;
    u32 bufSize;     // 0x14
    u8*  buffer;     // 0x18
    char name[0x40]; // 0x1C (64-char filename)
    u32 field_0x5C;
    u32 field_0x60;
    f32 field_0x64;  // current volume
    f32 field_0x68;  // target volume
    f32 field_0x6C;  // fade start volume
    f32 field_0x70;  // fade target
    f32 field_0x74;  // fade timer
    f32 field_0x78;  // volume multiplier
    f32 field_0x7C;  // volume multiplier 2
    u32 field_0x80;  // fade-end action
    u32 field_0x84;  // playback position
    u32 field_0x88;  // AFS file id
    u32 field_0x8C;  // AFS subfile id
    u32 flags;       // 0x90
}; // 0x94

// Streaming region tail: the 5 slots plus the instance-level counters.
struct CLibCriStreamingPlayData {
    StreamEntry entries[5]; // 0x1C8..0x4AC
    u32 streamCounter;      // 0x4AC
    s32 pauseCount;         // 0x4B0
}; // 0x4B4

extern "C" {
    // ADXT streaming functions
    void* ADXT_Create(int channels, void* buffer, u32 bufSize);
    void ADXT_Destroy(void* adxt);
    void ADXT_StartFnameRange(void* adxt, const char* filename);
    void ADXT_StartAfs(void* adxt, int fileId, int subfileId);
    void ADXT_Pause(void* adxt, int pause);
    int ADXT_GetStat(void* adxt);
    int ADXT_GetTimeReal(void* adxt);
    void ADXT_SetOutVol(void* adxt, int vol);
    void ADXT_SetOutPan(void* adxt, int ch, int pan);
    void ADXT_SetSvrFreq(void* adxt, int freq);
    void ADXT_AttachAhx(void* adxt, void* data, u32 size);
    void ADXT_DetachAhx(void* adxt);
    void ADXM_ExecMain(void);
    
    // Device functions
    int getFileSize__11CDeviceFileFPCc(const char* filename, int param);
    bool isSoundModeMono__9CDeviceSCFv(void);
    
    // Pan calculation: (float a, float b, float* out1, float* out2, in1, in2)
    void func_8049B834(float a, float b, float* out1, float* out2, const u8* in1, const u8* in2);
    
    // Memory management
    u32 getHandleMEM2__Q23mtl10MemManagerFv(void);
    void* allocate_head__Q23mtl10MemManagerFUlUli(u32 handle, u32 size, int align);
    void* allocate_tail__Q23mtl10MemManagerFUlUli(u32 handle, u32 size, int align);
    
    // CRT/OS
    void VIWaitForRetrace(void);
    void __cvt_fp2unsigned(float f);
    
    // Global state
    extern CLibCriStreamingPlay* lbl_eu_806656E8;
    extern void* lbl_eu_806656EC;
    extern const char* lbl_eu_806637A4;
    extern u32 lbl_eu_806637A0;
    extern float lbl_eu_8066A508;
    extern const float lbl_eu_8066A50C;
    extern float lbl_eu_8066A510;
    extern double lbl_eu_8066A518;
    extern float lbl_eu_8066A520;
    extern float lbl_eu_8066A524;
    extern const u32 lbl_eu_80523050[20];
    extern const u32 lbl_eu_805230B8[24];

    // Shared ".ahx" literal (monolibdata2 .sdata2 pool)
    extern const char lbl_eu_8066A500[];
    // RTTI base objects (strong copies live in other TUs)
    extern u32 __RTTI__10IWorkEvent[];
    extern u32 __RTTI__11CWorkThread[];

    // Dissolved retail class data (definitions at the bottom of this TU)
    extern u32 lbl_eu_8056D028[45];
    extern u32 lbl_eu_8056D0DC[5];
    extern u32 lbl_eu_806637A8[2];

    // CException second-base teardown (free-function call form)
    extern void func_804591DC__10CExceptionFP10IException(void*);

    // Vtable slot targets: IWorkEvent/CWorkThread defaults
    extern void WorkEvent1__10IWorkEventFPvPCc();
    extern void OnFileEvent__10IWorkEventFP10CEventFile();
    extern void WorkEvent3__10IWorkEventFPv();
    extern void WorkEvent4__10IWorkEventFv();
    extern void WorkEvent6__10IWorkEventFv();
    extern void WorkEvent7__10IWorkEventFv();
    extern void WorkEvent8__10IWorkEventFv();
    extern void WorkEvent9__10IWorkEventFv();
    extern void WorkEvent10__10IWorkEventFv();
    extern void WorkEvent11__10IWorkEventFv();
    extern void WorkEvent12__10IWorkEventFv();
    extern void WorkEvent13__10IWorkEventFv();
    extern void WorkEvent14__10IWorkEventFv();
    extern void WorkEvent15__10IWorkEventFv();
    extern void WorkEvent16__10IWorkEventFv();
    extern void WorkEvent17__10IWorkEventFv();
    extern void WorkEvent18__10IWorkEventFv();
    extern void WorkEvent19__10IWorkEventFv();
    extern void WorkEvent20__10IWorkEventFv();
    extern void WorkEvent21__10IWorkEventFv();
    extern void WorkEvent22__10IWorkEventFv();
    extern void WorkEvent23__10IWorkEventFv();
    extern void WorkEvent24__10IWorkEventFv();
    extern void WorkEvent25__10IWorkEventFv();
    extern void WorkEvent26__10IWorkEventFv();
    extern void WorkEvent27__10IWorkEventFv();
    extern void WorkEvent28__10IWorkEventFv();
    extern void WorkEvent29__10IWorkEventFv();
    extern void WorkEvent30__10IWorkEventFv();
    extern void WorkEvent31__10IWorkEventFv();
    extern void wkRender__11CWorkThreadFv();
    extern void wkRenderAfter__11CWorkThreadFv();
    extern void wkStandbyExceptionRetry__11CWorkThreadFUl();
    // Overridden virtuals defined in this TU (mangled spellings for the table)
    extern void OnPauseTrigger__20CLibCriStreamingPlayFb();
    extern void wkUpdate__20CLibCriStreamingPlayFv();
    extern void wkStandbyLogin__20CLibCriStreamingPlayFv();
    extern void wkStandbyLogout__20CLibCriStreamingPlayFv();
    extern void func_8045CF30__20CLibCriStreamingPlayFv();
    extern void func_8045D140__20CLibCriStreamingPlayFv();
}

// Volume lookup helper - searches table for volume dB value.
// Inlined by MWCC into every caller (retail inlines the loop).
static int lookupVolume(int volDb) {
    const int* table = reinterpret_cast<const int*>(lbl_eu_80523050);
    int hi;
    while ((hi = table[0]) >= 0) {
        if (volDb < hi) {
            return table[1];
        } else if (volDb > table[2]) {
            int lo = table[2];
            float num1 = (float)(table[1] - table[3]);
            float num2 = (float)(volDb - lo);
            float den = (float)(hi - lo);
            return table[3] + (int)(num1 * num2 / den);
        }
        table += 2;
    }
    return -960;
}

// Find a stream slot by id (inlined by MWCC; returns the slot pointer or NULL).
static StreamEntry* findStreamSlot(u32 id) {
    u8* entry = reinterpret_cast<u8*>(lbl_eu_806656E8) + 0x1C8;
    for (int i = 0; i < 5; i++) {
        if (*(u32*)(entry + 4) == id) {
            return reinterpret_cast<StreamEntry*>(entry);
        }
        entry += 0x94;
    }
    return NULL;
}

static void releaseSlot(StreamEntry* entry);

// Find a free (unused) stream slot; NULL if all five are in use.
static StreamEntry* findFreeSlot() {
    u8* entry = reinterpret_cast<u8*>(lbl_eu_806656E8) + 0x1C8;
    for (int i = 0; i < 5; i++) {
        if (*(u32*)(entry + 4) + 0x10000 == 0xFFFF) {
            return reinterpret_cast<StreamEntry*>(entry);
        }
        entry += 0x94;
    }
    return NULL;
}

// Constructor - initializes streaming audio manager
CLibCriStreamingPlay::CLibCriStreamingPlay(const char* pName, CWorkThread* pParent)
    : CLibCri(pName, pParent) {
    u8* base = (u8*)this;
    // novtable: install retail vtables by hand (primary at +0x0,
    // CDeviceVICb secondary at +0x1C4)
    *(volatile void**)base = (void*)&lbl_eu_8056D028;
    *(volatile void**)(base + 0x1C4) = (void*)((u8*)&lbl_eu_8056D028 + 0xA0);

    // Initialize stream entry flags to 0
    for (int i = 0; i < 5; i++) {
        *(u32*)(base + 0x25C + i * 0x94 + 0x90) = 0;
    }
    
    // Initialize counters
    *(u32*)(base + 0x4AC) = 0;
    *(u32*)(base + 0x4B0) = 0;
    
    // Store global instance
    lbl_eu_806656E8 = this;
    
    // Set work type
    *(u32*)(base + 0x50) = 0x11;
    
    // Clear stream data area
    memset(base + 0x1C8, 0, 0x2E4);
    
    // Initialize 5 stream slots: set slot IDs 0-4 and mark unused (-1)
    *(u32*)(base + 0x1C8) = 0;  *(u32*)(base + 0x1CC) = -1;
    *(u32*)(base + 0x25C) = 1;  *(u32*)(base + 0x260) = -1;
    *(u32*)(base + 0x2F0) = 2;  *(u32*)(base + 0x2F4) = -1;
    *(u32*)(base + 0x384) = 3;  *(u32*)(base + 0x388) = -1;
    *(u32*)(base + 0x418) = 4;  *(u32*)(base + 0x41C) = -1;
    
    // Allocate MEM2 audio buffer (~64KB)
    u32 mem2 = getHandleMEM2__Q23mtl10MemManagerFv();
    lbl_eu_806656EC = allocate_head__Q23mtl10MemManagerFUlUli(mem2, 0xA000, 4);
    
    // Initialize exception handler
    ((CException*)(base + 0x1C4))->func_804591BC(nullptr);
}

// Destructor (retail deleting-dtor form: null-this guard, flag > 0 frees).
// Defined as an extern "C" retail-symbol function so no implicit ~CLibCri()
// chain is emitted - retail's class has no CLibCri layer and destroys
// CWorkThread directly.
extern "C" void __dt__11CWorkThreadFv(void* self, int flag);
extern "C" void __dl__FPv(void* p);

extern "C" CLibCriStreamingPlay* __dt__20CLibCriStreamingPlayFv(CLibCriStreamingPlay* self, int flag) {
    u8* base = (u8*)self;
    u8* exc;

    if (self != nullptr) {
        // novtable: restore retail vtables (primary + CDeviceVICb secondary)
        *(void**)base = (void*)&lbl_eu_8056D028;
        *(void**)(base + 0x1C4) = (void*)((u8*)&lbl_eu_8056D028 + 0xA0);

        // Clean up exception handler (retail guards the subobject on this)
        exc = base;
        if (base != nullptr) {
            exc = base + 0x1C4;
        }
        func_804591DC__10CExceptionFP10IException((CException*)exc);

        // Free audio buffer
        if (lbl_eu_806656EC) {
            mtl::MemManager::deallocate(lbl_eu_806656EC);
            lbl_eu_806656EC = nullptr;
        }

        // Clear instance
        lbl_eu_806656E8 = nullptr;

        // Base destructor, then free when invoked as deleting dtor
        __dt__11CWorkThreadFv(self, 0);
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

// func_8045B5AC - Start streaming playback.
// Retail has no `this`: r3 = filename, r4 = alloc handle, r5 = loop flag,
// r6 = AFS file id, r7 = AFS subfile id, r8 = force-AHX flag.
// Returns the stream id, or -1 on failure.
// The shadow locals are declared in retail's colouring order (first-declared
// gets the highest callee-saved register): slot=r31 .. forceAhx=r24.
extern "C" int func_8045B5AC__20CLibCriStreamingPlayFv(const char* filename, int param2, bool loopFlag,
                  int afsId, int afsSubId, bool forceAhx) {
    StreamEntry* slot;
    int subId = afsSubId;
    int fileId = afsId;
    int loop = loopFlag;
    int handle = param2;
    const char* file = filename;
    int found;
    bool ahx = forceAhx;

    if (getFileSize__11CDeviceFileFPCc(file, 0) < 0) {
        return -1;
    }

    slot = findFreeSlot();
    if (slot == NULL) {
        return -1;
    }

    slot->flags = 0;
    if (isSoundModeMono__9CDeviceSCFv()) {
        slot->flags |= 0x40;
    } else {
        slot->flags &= ~0x40;
    }

    strncpy(slot->name, file, 0x40);

    // .ahx extension or forced AHX mode selects the AHX decoder.
    found = 0;
    if (strstr(file, lbl_eu_806637A4) != NULL || ahx) {
        found = 1;
    }
    if (found != 0) {
        slot->flags |= 0x04;
    } else {
        slot->flags &= ~0x04;
    }

    slot->field_0x0C = (u32)handle;

    // Streaming buffer size (ch = 2 for stereo, 1 for mono-AHX):
    //   (rate/10000 >> 6) * ch*25000 * 0xBA2F8BA3 >> 32 >> 5 * 6,
    // rounded up to 0x800, plus ch*24768 + 100.
    u32 rate = lbl_eu_806637A0;
    int bit = (int)((slot->flags >> 2) & 1);
    int ch1 = 2 - bit;
    u32 a = (rate / 10000) >> 6;
    u32 b = a * (u32)(ch1 * 25000);
    int negb = -bit;
    int signb = (negb | bit) >> 31;
    u32 c = (u32)(((u64)b * 0xBA2F8BA3u) >> 32);
    u32 d = (c >> 5) * 6;
    slot->bufSize = ((d + 0x800) & ~0x7FF) + (u32)((signb + 2) * 24768) + 100;

    slot->buffer = reinterpret_cast<u8*>(allocate_tail__Q23mtl10MemManagerFUlUli(
        (u32)handle, slot->bufSize, 0x20));
    if (slot->buffer == NULL) {
        if (slot->adxt != NULL) {
            if (slot->flags & 0x04) {
                ADXT_DetachAhx(slot->adxt);
            }
            ADXT_Destroy(slot->adxt);
            slot->adxt = NULL;
        }
        if (slot->buffer != NULL) {
            mtl::MemManager::deallocate(slot->buffer);
            slot->buffer = NULL;
        }
        slot->id = -1;
        slot->flags = 0;
        return -1;
    }

    // Generate a unique stream id: (slotIdx+1 & 0xFF) | (counter & 0xFF) << 8.
    u32* counterPtr = reinterpret_cast<u32*>(
        reinterpret_cast<u8*>(lbl_eu_806656E8) + 0x4AC);
    u32 counter = *counterPtr;
    slot->id = ((slot->slotId + 1) & 0xFF) | ((counter & 0xFF) << 8);
    *counterPtr = counter + 1;

    // Initialise playback state.
    slot->field_0x5C = 0;
    slot->field_0x60 = 0;
    slot->field_0x64 = lbl_eu_8066A508;
    slot->field_0x7C = lbl_eu_8066A508;
    slot->field_0x68 = lbl_eu_8066A508;
    slot->field_0x6C = lbl_eu_8066A508;
    slot->field_0x70 = lbl_eu_8066A50C;
    slot->field_0x74 = lbl_eu_8066A50C;
    slot->field_0x80 = 0;
    slot->field_0x84 = 0;
    slot->field_0x78 = lbl_eu_8066A508;

    if (loop) {
        slot->flags |= 0x20;
    } else {
        slot->flags &= ~0x20;
    }

    slot->field_0x88 = (u32)fileId;
    slot->field_0x8C = (u32)subId;

    // Create the ADX decoder unless this is a preload-only request (0x10).
    if (!(slot->flags & 0x10)) {
        int ch2 = 2 - (int)((slot->flags >> 2) & 1);
        slot->adxt = reinterpret_cast<u8*>(ADXT_Create(ch2, slot->buffer, slot->bufSize));

        if (slot->flags & 0x04) {
            u32 idx = slot->slotId << 13;
            ADXT_AttachAhx(slot->adxt, reinterpret_cast<u8*>(lbl_eu_806656EC) + idx, 0x2000);
            ADXT_SetSvrFreq(slot->adxt, 0x1e);
        }

        if (fileId >= 0) {
            ADXT_StartAfs(slot->adxt, fileId, subId);
        } else {
            ADXT_StartFnameRange(slot->adxt, file);
        }

        // Mono streams get a centered pan.
        if ((slot->flags & 0x40) && !(slot->flags & 0x04)) {
            ADXT_SetOutPan(slot->adxt, 1, 0);
            ADXT_SetOutPan(slot->adxt, 0, 0);
        }

        // Block until playback actually starts when requested.
        if (loop) {
            int waitCount = 100;
            while (true) {
                if (ADXT_GetStat(slot->adxt) == 3) {
                    break;
                }
                if (--waitCount <= 0) {
                    break;
                }
                ADXM_ExecMain();
                VIWaitForRetrace();
            }
        }
    }

    return slot->id;
}

// func_8045B970 - Check if stream ID is active.
// Unrolled-by-MWCC 5-slot scan: id == -1 (id+0x10000 wraps to 0xFFFF)
// matches any slot that is itself not unused (-1).
extern "C" bool func_8045B970__20CLibCriStreamingPlayFv(u32 id) {
    CLibCriStreamingPlayData* data = reinterpret_cast<CLibCriStreamingPlayData*>(
        reinterpret_cast<u8*>(lbl_eu_806656E8) + 0x1C8);

    for (int i = 0; i < 5; i++) {
        u32 v = (u32)data->entries[i].id;
        if (v == id && v + 0x10000 != 0xFFFF) return true;
        if (v + 0x10000 != 0xFFFF && id + 0x10000 == 0xFFFF) return true;
    }

    return false;
}

// func_8045BAB0 - Stop a specific stream.
// Unrolled match chain (same shape as func_8045BE48): reproduces retail's
// per-stage "bne next / b done" pairs and chained pointer advance.
extern "C" void func_8045BAB0__20CLibCriStreamingPlayFv(int id) {
    StreamEntry* entry = reinterpret_cast<StreamEntry*>(
        reinterpret_cast<u8*>(lbl_eu_806656E8) + 0x1C8);

    entry =
        (u32)entry->id == (u32)id ? entry :
        (u32)(++entry)->id == (u32)id ? entry :
        (u32)(++entry)->id == (u32)id ? entry :
        (u32)(++entry)->id == (u32)id ? entry :
        (u32)(++entry)->id == (u32)id ? entry :
        (entry = NULL);

    // Retail emits a doubled null-guard branch pair here.
    if (entry != NULL) {
        if (entry != NULL) {
            releaseSlot(entry);
        }
    }
}

// func_8045BBA0 - Stop all streams
void CLibCriStreamingPlay::func_8045BBA0() {
    StreamEntry* entry = reinterpret_cast<StreamEntry*>(
        reinterpret_cast<u8*>(lbl_eu_806656E8) + 0x1C8);

    for (u32 i = 0; i < 5; i++, entry++) {
        // Defensive per-entry null guard (retail keeps the check in-loop).
        if (entry == NULL) continue;

        // Destroy ADXT handle
        if (entry->adxt != NULL) {
            if (entry->flags & 0x04) ADXT_DetachAhx(entry->adxt);
            ADXT_Destroy(entry->adxt);
            entry->adxt = NULL;
        }

        // Free buffer
        if (entry->buffer != NULL) {
            mtl::MemManager::deallocate(entry->buffer);
            entry->buffer = NULL;
        }

        // Mark unused
        entry->id = -1;
        entry->flags = 0;
    }
}

// func_8045BC4C - Update stream pause/volume state.
// Retail has no `this`: r3 = stream id, r4 = pause flag.
// Unrolled match chain (same shape as func_8045BE48): reproduces retail's
// per-stage "bne next / b done" pairs and chained pointer advance.
extern "C" void func_8045BC4C__20CLibCriStreamingPlayFv(int id, bool pause) {
    StreamEntry* entry = reinterpret_cast<StreamEntry*>(
        reinterpret_cast<u8*>(lbl_eu_806656E8) + 0x1C8);

    entry =
        (u32)entry->id == (u32)id ? entry :
        (u32)(++entry)->id == (u32)id ? entry :
        (u32)(++entry)->id == (u32)id ? entry :
        (u32)(++entry)->id == (u32)id ? entry :
        (u32)(++entry)->id == (u32)id ? entry :
        (entry = NULL);

    if (entry != NULL) {
        if (pause) {
            entry->flags |= 1;
        } else {
            entry->flags &= ~1u;
        }

        // Paused when: stream pause bits set, global pause counter non-zero,
        // or a flow is active.
        u32 flags = entry->flags;
        int flow = CWorkControl::hasFlow();
        int pauseArg = (flags & 1) |
                       (*(s32*)((u8*)lbl_eu_806656E8 + 0x4B0) != 0) |
                       ((flags >> 1) & 1) | flow;
        ADXT_Pause(entry->adxt, pauseArg);

        // Output volume = level * mult1 * mult2, forced to 1.0 during a flow.
        float vol = entry->field_0x7C * (entry->field_0x64 * entry->field_0x78);
        if (CWorkControl::hasFlow()) {
            vol = lbl_eu_8066A50C;
        }
        ADXT_SetOutVol(entry->adxt, lookupVolume((int)(lbl_eu_8066A510 * vol)));
    }
}

// func_8045BE48 - Get playback position.
// Retail entry takes the stream id directly in r3 (see func_8045B5AC).
// The search must stay a conditional-expression chain: if/else and loop forms
// fuse the match exit into a single branch, while this shape reproduces
// retail's per-stage "bne next / b done" pair and chained pointer advance.
extern "C" int func_8045BE48__20CLibCriStreamingPlayFv(int id) {
    StreamEntry* entry = reinterpret_cast<StreamEntry*>(
        reinterpret_cast<u8*>(lbl_eu_806656E8) + 0x1C8);

    entry =
        (u32)entry->id == (u32)id ? entry :
        (u32)(++entry)->id == (u32)id ? entry :
        (u32)(++entry)->id == (u32)id ? entry :
        (u32)(++entry)->id == (u32)id ? entry :
        (u32)(++entry)->id == (u32)id ? entry :
        (entry = NULL);

    return entry != NULL ? (int)entry->field_0x84 : 0;
}

// Volume lookup helper used by wkUpdate: retail performs the interpolation
// with double arithmetic (dbl(hi-lo) * dbl(volDb-lo) / dbl(tbl1-tbl3)).
static int lookupVolumeDb(int volDb) {
    const int* table = reinterpret_cast<const int*>(lbl_eu_80523050);
    int hi;
    while ((hi = table[0]) >= 0) {
        if (volDb < hi) {
            return table[1];
        }
        int lo = table[2];
        if (volDb > lo) {
            return table[3] + (int)((double)(hi - lo) * (double)(volDb - lo) /
                                    (double)(table[1] - table[3]));
        }
        table += 2;
    }
    return -960;
}

// Release one stream slot: destroy the ADXT handle, free the buffer, mark unused.
// Inlined by MWCC at each call site.
static void releaseSlot(StreamEntry* entry) {
    if (entry->adxt != nullptr) {
        if (entry->flags & 4) ADXT_DetachAhx(entry->adxt);
        ADXT_Destroy(entry->adxt);
        entry->adxt = nullptr;
    }
    if (entry->buffer != nullptr) {
        mtl::MemManager::deallocate(entry->buffer);
        entry->buffer = nullptr;
    }
    entry->id = -1;
    entry->flags = 0;
}

// wkUpdate - Main update loop for all streams
void CLibCriStreamingPlay::wkUpdate() {
    const float fadeStep = lbl_eu_8066A520;
    const float volPct = lbl_eu_8066A510;
    StreamEntry* entry = reinterpret_cast<StreamEntry*>(
        reinterpret_cast<u8*>(this) + 0x1C8);
    const char* statusBase = reinterpret_cast<const char*>(lbl_eu_805230B8);

    for (u32 i = 0; i < 5; i++, entry++) {
        if (entry->adxt == nullptr) continue;

        // Fade processing runs only while a fade is pending, nothing is globally
        // paused, and the stream is not individually paused.
        if (!(entry->field_0x70 == lbl_eu_8066A50C ||
              *(s32*)((u8*)this + 0x4B0) != 0 || (entry->flags & 1))) {
            float timer = entry->field_0x74 + fadeStep;
            entry->field_0x74 = timer;

            if (timer >= entry->field_0x70) {
                // Fade finished: latch the end volume and reset the fade time.
                entry->field_0x64 = entry->field_0x68;
                entry->field_0x70 = lbl_eu_8066A50C;
                if (entry->field_0x68 == lbl_eu_8066A50C) {
                    // Faded back to neutral: apply the fade-end action.
                    u32 action = entry->field_0x80;
                    if (action == 1) {
                        // Pause in place at fade end.
                        u32 newFlags = entry->flags | 2;
                        entry->flags = newFlags;
                        int pauseArg = CWorkControl::hasFlow();
                        pauseArg |= (*(s32*)((u8*)lbl_eu_806656E8 + 0x4B0) != 0);
                        pauseArg |= (int)(newFlags & 1);
                        pauseArg |= (int)((newFlags >> 1) & 1);
                        ADXT_Pause(entry->adxt, pauseArg);
                    } else if (action == 2) {
                        // Stop and release the stream.
                        if (entry != nullptr) {
                            releaseSlot(entry);
                        }
                        continue;
                    }
                } else if (entry->flags & 2) {
                    // Resume a stream parked by a previous fade-end pause.
                    u32 newFlags = entry->flags & ~2;
                    entry->flags = newFlags;
                    int pauseArg = CWorkControl::hasFlow();
                    pauseArg |= (*(s32*)((u8*)lbl_eu_806656E8 + 0x4B0) != 0);
                    pauseArg |= (int)(newFlags & 1);
                    pauseArg |= (int)((newFlags >> 1) & 1);
                    ADXT_Pause(entry->adxt, pauseArg);
                }
            } else {
                // Interpolate the current volume toward the fade target.
                float t = timer / entry->field_0x70;
                entry->field_0x64 =
                    (entry->field_0x68 - entry->field_0x6C) * t + entry->field_0x6C;

                // Un-pause a fading-in stream once its volume moved off the mute point.
                if (entry->field_0x80 == 1 && (entry->flags & 2) &&
                    entry->field_0x64 != lbl_eu_8066A50C) {
                    u32 newFlags = entry->flags & ~2;
                    entry->flags = newFlags;
                    int pauseArg = CWorkControl::hasFlow();
                    pauseArg |= (*(s32*)((u8*)lbl_eu_806656E8 + 0x4B0) != 0);
                    pauseArg |= (int)(newFlags & 1);
                    pauseArg |= (int)((newFlags >> 1) & 1);
                    ADXT_Pause(entry->adxt, pauseArg);
                }
            }
        }    volumeSection:
        // Output volume = level * mult1 * mult2, forced to 1.0 while a flow is active.
        float vol = entry->field_0x7C * (entry->field_0x64 * entry->field_0x78);
        if (CWorkControl::hasFlow()) {
            vol = lbl_eu_8066A50C;
        }
        ADXT_SetOutVol(entry->adxt, lookupVolumeDb((int)(volPct * vol)));

        // Playback status handling.
        int stat = ADXT_GetStat(entry->adxt);
        if (stat == 5 || stat == 6) {
            // Stream finished: release it.
            if (entry != nullptr) {
                releaseSlot(entry);
            }
            continue;
        }

        {
            // Status-name scratch (result unused; retail discards it too).
            struct StatusName {
                char name[0x40];
                u32 len;
            } sn;
            sn.name[0] = '\0';
            sn.len = 0;

            if (stat == 0) {
                const char* msg = statusBase;
                sn.len = strlen(msg);
                strcpy(sn.name, msg);
            } else if (stat == 1) {
                const char* msg = statusBase + 7;
                sn.len = strlen(msg);
                strcpy(sn.name, msg);
            } else if (stat == 2) {
                const char* msg = statusBase + 30;
                sn.len = strlen(msg);
                strcpy(sn.name, msg);
            } else if (stat == 3) {
                const char* msg = statusBase + 41;
                sn.len = strlen(msg);
                strcpy(sn.name, msg);
            } else if (stat == 4) {
                const char* msg = statusBase + 58;
                sn.len = strlen(msg);
                strcpy(sn.name, msg);
            } else if (stat == 5) {
                const char* msg = statusBase + 71;
                sn.len = strlen(msg);
                strcpy(sn.name, msg);
            } else if (stat == 6) {
                const char* msg = statusBase + 80;
                sn.len = strlen(msg);
                strcpy(sn.name, msg);
            }

            // Track playback position from the decoded time (ms -> position units).
            int time = ADXT_GetTimeReal(entry->adxt);
            entry->field_0x84 = (u32)((f64)time / volPct / fadeStep);
        }
    }
}

// wkStandbyLogin - Just calls base class (tail call)
bool CLibCriStreamingPlay::wkStandbyLogin() {
    return CWorkThread::wkStandbyLogin();
}

// wkStandbyLogout - Cleanup all streams on logout
bool CLibCriStreamingPlay::wkStandbyLogout() {
    // Work queue must be empty (self-referencing list head) before logout.
    if (mChildren.mStartNodePtr->mNext == mChildren.mStartNodePtr) {
        u32 i = 0;
        StreamEntry* entry = reinterpret_cast<StreamEntry*>(
            reinterpret_cast<u8*>(lbl_eu_806656E8) + 0x1C8);

        for (; i < 5; i++, entry++) {
            // Defensive per-entry null guard (retail keeps the check in-loop).
            if (entry == NULL) continue;

            // Destroy ADXT handle
            if (entry->adxt != NULL) {
                if (entry->flags & 0x04) ADXT_DetachAhx(entry->adxt);
                ADXT_Destroy(entry->adxt);
                entry->adxt = NULL;
            }

            // Free buffer
            if (entry->buffer != NULL) {
                mtl::MemManager::deallocate(entry->buffer);
                entry->buffer = NULL;
            }

            // Mark unused
            entry->id = -1;
            entry->flags = 0;
        }

        return CWorkThread::wkStandbyLogout();
    }
    return false;
}

// OnPauseTrigger - Handle pause/unpause.
// Bumps the global pause counter, then refreshes every active slot's pause
// state and output volume. The pause counter is read back through the global
// singleton inside the loop, and the dB-volume table lookup is inlined here.
void CLibCriStreamingPlay::OnPauseTrigger(bool paused) {
    s32* pauseCounter = (s32*)((u8*)this + 0x4B0);

    if (paused) {
        (*pauseCounter)++;
    } else {
        // Clamp at zero so unmatched unpause requests can't go negative.
        (*pauseCounter)--;
        if (*pauseCounter < 0) *pauseCounter = 0;
    }

    StreamEntry* entry = reinterpret_cast<StreamEntry*>((u8*)this + 0x1C8);
    const float volPct = lbl_eu_8066A510;
    for (u32 i = 0; i < 5; i++, entry++) {
        if ((u32)(entry->id + 0x10000) == 0xFFFF) continue; // unused slot

        // Paused when: stream pause bits set, global pause counter non-zero,
        // or a flow is active (checked twice, matching retail).
        u32 flags = entry->flags;
        int flow = CWorkControl::hasFlow();
        s32 count = *(s32*)((u8*)lbl_eu_806656E8 + 0x4B0);
        int pauseArg = (int)(flags & 1) | (int)((flags >> 1) & 1) |
                       (int)(count != 0) | flow;
        ADXT_Pause(entry->adxt, pauseArg);

        // Output volume = level * mult1 * mult2, forced to 1.0 during a flow.
        float vol = entry->field_0x64 * entry->field_0x78 * entry->field_0x7C;
        if (CWorkControl::hasFlow()) {
            vol = lbl_eu_8066A50C;
        }
        int volDb = (int)(volPct * vol);
        int outVol = lookupVolume(volDb);
        ADXT_SetOutVol(entry->adxt, outVol);
    }
}

// func_8045C67C - Get current volume for stream.
// Unrolled match chain (same shape as func_8045BE48): reproduces retail's
// per-stage "bne next / b done" pairs and chained pointer advance.
extern "C" float func_8045C67C__20CLibCriStreamingPlayFv(int id) {
    StreamEntry* entry = reinterpret_cast<StreamEntry*>(
        reinterpret_cast<u8*>(lbl_eu_806656E8) + 0x1C8);

    entry =
        (u32)entry->id == (u32)id ? entry :
        (u32)(++entry)->id == (u32)id ? entry :
        (u32)(++entry)->id == (u32)id ? entry :
        (u32)(++entry)->id == (u32)id ? entry :
        (u32)(++entry)->id == (u32)id ? entry :
        (entry = NULL);

    return entry == NULL ? lbl_eu_8066A50C : entry->field_0x64;
}

// func_8045C700 - Set volume immediately.
// Unrolled match chain (same shape as func_8045BE48): reproduces retail's
// per-stage "bne next / b done" pairs and chained pointer advance.
extern "C" void func_8045C700__20CLibCriStreamingPlayFv(int id, float volume) {
    StreamEntry* entry = reinterpret_cast<StreamEntry*>(
        reinterpret_cast<u8*>(lbl_eu_806656E8) + 0x1C8);

    entry =
        (u32)entry->id == (u32)id ? entry :
        (u32)(++entry)->id == (u32)id ? entry :
        (u32)(++entry)->id == (u32)id ? entry :
        (u32)(++entry)->id == (u32)id ? entry :
        (u32)(++entry)->id == (u32)id ? entry :
        (entry = NULL);

    if (entry != NULL) {
        // Set volume and fade parameters
        entry->field_0x64 = volume;
        double dv = volume;
        entry->field_0x68 = volume;
        float vol = (float)dv;
        entry->field_0x6C = vol;
        entry->field_0x70 = lbl_eu_8066A50C;
        entry->field_0x74 = lbl_eu_8066A50C;

        // Calculate and set output volume
        float totalVol = entry->field_0x7C * (vol * entry->field_0x78);
        if (CWorkControl::hasFlow()) {
            totalVol = lbl_eu_8066A50C;
        }
        int outVol = lookupVolume((int)(lbl_eu_8066A510 * totalVol));
        ADXT_SetOutVol(entry->adxt, outVol);
    }
}

// func_8045C8B0 - Set volume multiplier.
// Unrolled match chain (same shape as func_8045BE48): reproduces retail's
// per-stage "bne next / b done" pairs and chained pointer advance.
extern "C" void func_8045C8B0__20CLibCriStreamingPlayFv(int id, float volume) {
    StreamEntry* entry = reinterpret_cast<StreamEntry*>(
        reinterpret_cast<u8*>(lbl_eu_806656E8) + 0x1C8);

    entry =
        (u32)entry->id == (u32)id ? entry :
        (u32)(++entry)->id == (u32)id ? entry :
        (u32)(++entry)->id == (u32)id ? entry :
        (u32)(++entry)->id == (u32)id ? entry :
        (u32)(++entry)->id == (u32)id ? entry :
        (entry = NULL);

    if (entry != NULL) {
        // Set volume multiplier, then recompute the output volume:
        // multiplier * (level * mult1), forced to 1.0 during a flow.
        entry->field_0x7C = volume;
        float totalVol = volume * (entry->field_0x64 * entry->field_0x78);
        if (CWorkControl::hasFlow()) {
            totalVol = lbl_eu_8066A50C;
        }
        int outVol = lookupVolume((int)(lbl_eu_8066A510 * totalVol));
        ADXT_SetOutVol(entry->adxt, outVol);
    }
}

// Unrolled-by-MWCC 5-slot scan by id; MWCC inlines this at each use site.
static StreamEntry* findSlotById(u32 id) {
    StreamEntry* entry = reinterpret_cast<StreamEntry*>(
        reinterpret_cast<u8*>(lbl_eu_806656E8) + 0x1C8);

    return
        (u32)entry->id == (u32)id ? entry :
        (u32)(++entry)->id == (u32)id ? entry :
        (u32)(++entry)->id == (u32)id ? entry :
        (u32)(++entry)->id == (u32)id ? entry :
        (u32)(++entry)->id == (u32)id ? entry :
        (entry = NULL);
}

// func_8045CA4C - Start a volume fade.
// Retail has no `this`: r3 = stream id, r4 = fade-end action, f1 = volume,
// f2 = fade time (see the CLibCri forwarding trampoline func_80459A88).
extern "C" void func_8045CA4C__20CLibCriStreamingPlayFv(int id, int action, float volume, float fadeTime) {
    // Immediate volume change when the fade time is <= 0.
    if (fadeTime <= lbl_eu_8066A50C) {
        StreamEntry* entry = findSlotById((u32)id);

        if (entry != NULL) {
            // Set volume and reset the fade state.
            entry->field_0x64 = volume;
            double dv = volume;
            entry->field_0x68 = volume;
            float level = (float)dv;
            entry->field_0x6C = level;
            entry->field_0x70 = lbl_eu_8066A50C;
            entry->field_0x74 = lbl_eu_8066A50C;

            // Total output volume: multiplier * (level * mult1), 0 during a flow.
            float totalVol = entry->field_0x7C * (level * entry->field_0x78);
            if (CWorkControl::hasFlow()) {
                totalVol = lbl_eu_8066A50C;
            }
            int volDb = (int)(lbl_eu_8066A510 * totalVol);
            ADXT_SetOutVol(entry->adxt, lookupVolume(volDb));
        }
    }

    // Always (re)set the fade parameters on the target entry.
    StreamEntry* entry = findSlotById((u32)id);

    if (entry != NULL) {
        entry->field_0x68 = volume;
        entry->field_0x6C = entry->field_0x64;
        // Latch the fade-end action only for a mute-to-zero fade.
        if (volume == lbl_eu_8066A50C) {
            entry->field_0x80 = action;
        }
        entry->field_0x70 = fadeTime;
        entry->field_0x74 = lbl_eu_8066A50C;
        if (entry->field_0x70 <= lbl_eu_8066A50C) {
            entry->field_0x70 = lbl_eu_8066A50C;
        }
    }
}

// func_8045CCFC - Set pan/volume from a 3D position pair.
// Retail has no `this`: r3 = stream id, r4/r5 = position pointers,
// f1/f2/f3 = the pan/volume parameters (see func_8049B834).
extern "C" void func_8045CCFC__20CLibCriStreamingPlayFv(int id, const u8* in2, const u8* in1,
                   float a, float b, float c) {
    // Unrolled match chain (same shape as func_8045BE48): reproduces retail's
    // per-stage "bne next / b done" pairs and chained pointer advance.
    StreamEntry* slot = reinterpret_cast<StreamEntry*>(
        reinterpret_cast<u8*>(lbl_eu_806656E8) + 0x1C8);

    slot =
        (u32)slot->id == (u32)id ? slot :
        (u32)(++slot)->id == (u32)id ? slot :
        (u32)(++slot)->id == (u32)id ? slot :
        (u32)(++slot)->id == (u32)id ? slot :
        (u32)(++slot)->id == (u32)id ? slot :
        (slot = NULL);

    if (slot != NULL) {
        // Compute base volume/pan from the two 3D positions.
        float pan, vol;
        func_8049B834(a, b, &pan, &vol, in1, in2);

        // Clamp the volume to the caller's minimum level.
        if (vol < c) {
            vol = c;
        }
        slot->field_0x78 = vol;

        // Total output volume: multiplier * (current * level), 0 during a flow.
        float totalVol = slot->field_0x64 * vol * slot->field_0x7C;
        if (CWorkControl::hasFlow()) {
            totalVol = lbl_eu_8066A50C;
        }

        // Convert to ADX output-volume units via the 100.0 percent scale, then
        // look up (with linear interpolation) the ADX volume from the dB table.
        int volDb = (int)(lbl_eu_8066A510 * totalVol);
        int outVol = lookupVolume(volDb);
        ADXT_SetOutVol(slot->adxt, outVol);

        // Set pan unless the stream is mono (flag 0x40).
        u32 flags = slot->flags;
        if (!(flags & 0x40)) {
            int panVal = (int)(lbl_eu_8066A524 * pan);
            if (panVal < -15) {
                panVal = -15;
            }
            if (panVal > 15) {
                panVal = 15;
            }
            if (!(flags & 0x04)) {
                ADXT_SetOutPan(slot->adxt, 1, panVal);
            }
            ADXT_SetOutPan(slot->adxt, 0, panVal);
        }
    }
}

// func_8045CF30 - Stop all streams (alternate entry)
void CLibCriStreamingPlay::func_8045CF30() {
    u32 i = 0;
    StreamEntry* entry = reinterpret_cast<StreamEntry*>(
        reinterpret_cast<u8*>(lbl_eu_806656E8) + 0x1C8);

    for (; i < 5; i++, entry++) {
        // Defensive per-entry null guard (retail keeps the check in-loop).
        if (entry == NULL) continue;

        // Destroy ADXT handle
        if (entry->adxt != NULL) {
            if (entry->flags & 0x04) ADXT_DetachAhx(entry->adxt);
            ADXT_Destroy(entry->adxt);
            entry->adxt = NULL;
        }

        // Free buffer
        if (entry->buffer != NULL) {
            mtl::MemManager::deallocate(entry->buffer);
            entry->buffer = NULL;
        }

        // Mark unused
        entry->id = -1;
        entry->flags = 0;
    }
}

// func_8045CFDC - Calculate buffer size for stream.
// Retail entry takes the channel flag directly in r3 (see also func_8045B5AC).
extern "C" int func_8045CFDC__20CLibCriStreamingPlayFv(int ch) {
    // Channel count: 1 when ch is non-zero (AHX/mono flag), else 2.
    int neg = -ch;
    ch = neg | ch;
    u32 t = ((ch >> 31) + 2) * 25000;
    u32 b = t * (lbl_eu_806637A0 / 1000);
    u32 c = b / 44;
    u32 d = c * 6;
    u32 e = (d + 0x800) & ~0x7FFu;
    return (int)(e + (u32)(((ch >> 31) + 2) * 24768) + 100);
}

// func_8045D03C - Check if stream is playing/paused.
// Unrolled match chain (same shape as func_8045BE48): reproduces retail's
// per-stage "bne next / b done" pairs and chained pointer advance.
extern "C" bool func_8045D03C__20CLibCriStreamingPlayFv(CLibCriStreamingPlay* self) {
    u32 flags;
    CLibCriStreamingPlay* inst = lbl_eu_806656E8;
    if (inst == NULL) return false;

    StreamEntry* entry = reinterpret_cast<StreamEntry*>(
        reinterpret_cast<u8*>(inst) + 0x1C8);

    entry =
        (u32)entry->id == (u32)self ? entry :
        (u32)(++entry)->id == (u32)self ? entry :
        (u32)(++entry)->id == (u32)self ? entry :
        (u32)(++entry)->id == (u32)self ? entry :
        (u32)(++entry)->id == (u32)self ? entry :
        (entry = NULL);

    if (entry == NULL) return false;
    if (entry->adxt == NULL) return false;

    // Paused when: stream pause bits set, global pause counter non-zero,
    // or the flow is active.
    flags = entry->flags;
    u32 bit0 = flags & 1;
    u32 bit1 = (flags >> 1) & 1;
    int flow = CWorkControl::hasFlow();
    s32 pauseCount = *(s32*)(reinterpret_cast<u8*>(inst) + 0x4B0);
    u32 paused = (u32)(pauseCount != 0);
    u32 bits = paused | bit0;
    return (flow | (bits | bit1)) != 0;
}

// func_8045D140 - CDeviceVICb thunk: adjust this and call func_8045CF30
void CLibCriStreamingPlay::func_8045D140() {
    CLibCriStreamingPlay* base = (CLibCriStreamingPlay*)((u8*)this - 0x1C4);
    base->func_8045CF30();
}

// func_8045D148 - CDeviceVICb thunk: adjust this and tail-branch to destructor
// (virtual ~ call would dispatch via vtable; retail tail-branches to __dt__ directly)
extern "C" void func_8045D148__20CLibCriStreamingPlayFv(void* self) {
    ((void(*)(void*))__dt__20CLibCriStreamingPlayFv)((char*)self - 0x1C4);
}

// --- Dissolved retail data (novtable + explicit retail-layout arrays) ---
// .rodata 0x80523050 (0xE8), .data 0x8056D028 (0xC8), .sdata 0x806637A0 (0x10),
// .sbss 0x806656E8 (0x8). Byte/reloc source: build/us/asm/monolib/src/lib/
// CLibCriStreamingPlay.s.

// dB volume lookup table (.rodata 0x80523050, read by lookupVolume)
extern "C" __declspec(section ".rodata") __attribute__((aligned(8))) const u32 lbl_eu_80523050[20] = {
    0x00000064, 0x00000000, 0x00000046, 0xFFFFFFE2,
    0x00000032, 0xFFFFFFC4, 0x00000028, 0xFFFFFFA6,
    0x0000001E, 0xFFFFFF88, 0x00000014, 0xFFFFFF60,
    0x0000000A, 0xFFFFFF38, 0x00000005, 0xFFFFFED4,
    0x00000000, 0xFFFFFC40, 0xFFFFFFFF, 0xFFFFFFFF
};

// monolib RTTI name string (.rodata 0x805230A0)
extern "C" __declspec(section ".rodata") const char lbl_eu_805230A0[21] = "CLibCriStreamingPlay";

// ADX status message strings, SJIS (.rodata 0x805230B8; wkUpdate slices it)
extern "C" __declspec(section ".rodata") const u32 lbl_eu_805230B8[24] = {
    0x92E28E7E, 0x92860041, 0x44582082, 0xCC837783,
    0x62835F8F, 0xEE95F18E, 0xE693BE92, 0x86008DC4,
    0x90B68F80, 0x94F59286, 0x00836683, 0x52815B83,
    0x6881958D, 0xC490B692, 0x86008366, 0x8352815B,
    0x83688F49, 0x97B9008D, 0xC490B68F, 0x4997B900,
    0x83478389, 0x815B94AD, 0x90B60000, 0x00000000
};

// AFS archive paths (.rodata 0x80523118 / 0x80523128)
extern "C" __declspec(section ".rodata") const char lbl_eu_80523118[15] = "hbm/jp/hbm.arc";
extern "C" __declspec(section ".rodata") __attribute__((aligned(8))) const char lbl_eu_80523128[16] = "hbm/nd/hbm.arc";

// vtable (.data 0x8056D028): primary {typeinfo, 0, slots...} + CDeviceVICb
// secondary at +0xA0 {typeinfo, -0x1C4, thunk dtor, thunk cb, stop-all}
extern "C" u32 lbl_eu_8056D028[45] __attribute__((aligned(8))) = {
    (u32)&lbl_eu_806637A8, 0,
    (u32)&__dt__20CLibCriStreamingPlayFv,
    (u32)&WorkEvent1__10IWorkEventFPvPCc, (u32)&OnFileEvent__10IWorkEventFP10CEventFile,
    (u32)&WorkEvent3__10IWorkEventFPv, (u32)&WorkEvent4__10IWorkEventFv,
    (u32)&OnPauseTrigger__20CLibCriStreamingPlayFb,
    (u32)&WorkEvent6__10IWorkEventFv, (u32)&WorkEvent7__10IWorkEventFv,
    (u32)&WorkEvent8__10IWorkEventFv, (u32)&WorkEvent9__10IWorkEventFv,
    (u32)&WorkEvent10__10IWorkEventFv, (u32)&WorkEvent11__10IWorkEventFv,
    (u32)&WorkEvent12__10IWorkEventFv, (u32)&WorkEvent13__10IWorkEventFv,
    (u32)&WorkEvent14__10IWorkEventFv, (u32)&WorkEvent15__10IWorkEventFv,
    (u32)&WorkEvent16__10IWorkEventFv, (u32)&WorkEvent17__10IWorkEventFv,
    (u32)&WorkEvent18__10IWorkEventFv, (u32)&WorkEvent19__10IWorkEventFv,
    (u32)&WorkEvent20__10IWorkEventFv, (u32)&WorkEvent21__10IWorkEventFv,
    (u32)&WorkEvent22__10IWorkEventFv, (u32)&WorkEvent23__10IWorkEventFv,
    (u32)&WorkEvent24__10IWorkEventFv, (u32)&WorkEvent25__10IWorkEventFv,
    (u32)&WorkEvent26__10IWorkEventFv, (u32)&WorkEvent27__10IWorkEventFv,
    (u32)&WorkEvent28__10IWorkEventFv, (u32)&WorkEvent29__10IWorkEventFv,
    (u32)&WorkEvent30__10IWorkEventFv, (u32)&WorkEvent31__10IWorkEventFv,
    (u32)&wkUpdate__20CLibCriStreamingPlayFv, (u32)&wkRender__11CWorkThreadFv,
    (u32)&wkRenderAfter__11CWorkThreadFv, (u32)&wkStandbyLogin__20CLibCriStreamingPlayFv,
    (u32)&wkStandbyLogout__20CLibCriStreamingPlayFv,
    (u32)&wkStandbyExceptionRetry__11CWorkThreadFUl,
    (u32)&lbl_eu_806637A8, 0xFFFFFE3C,
    (u32)&func_8045D148__20CLibCriStreamingPlayFv, (u32)&func_8045D140__20CLibCriStreamingPlayFv,
    (u32)&func_8045CF30__20CLibCriStreamingPlayFv
};

// RTTI base list (.data 0x8056D0DC)
extern "C" u32 lbl_eu_8056D0DC[5] = {
    (u32)&__RTTI__10IWorkEvent, 0, (u32)&__RTTI__11CWorkThread, 0, 0
};

// Sample-rate constant, 48000 Hz (.sdata 0x806637A0)
extern "C" __declspec(section ".sdata") __attribute__((aligned(8))) u32 lbl_eu_806637A0 = 48000;

// ".ahx" extension selector pointer (.sdata 0x806637A4)
extern "C" __declspec(section ".sdata") const char* lbl_eu_806637A4 = (const char*)lbl_eu_8066A500;

// monolib type descriptor {name, base list} (.sdata 0x806637A8)
extern "C" __declspec(section ".sdata") u32 lbl_eu_806637A8[2] = {
    (u32)&lbl_eu_805230A0, (u32)&lbl_eu_8056D0DC
};

// Streaming singletons (.sbss 0x806656E8)
CLibCriStreamingPlay* lbl_eu_806656E8 __attribute__((aligned(8)));
void* lbl_eu_806656EC;
