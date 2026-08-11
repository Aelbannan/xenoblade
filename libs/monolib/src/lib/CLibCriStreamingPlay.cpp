// CLibCriStreamingPlay - CRI streaming audio playback manager

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
    extern float lbl_eu_8066A50C;
    extern float lbl_eu_8066A510;
    extern double lbl_eu_8066A518;
    extern float lbl_eu_8066A520;
    extern float lbl_eu_8066A524;
    extern u32 lbl_eu_80523050[];
    extern const char lbl_eu_805230B8[];
}

// Volume lookup helper - searches table for volume dB value.
// Inlined by MWCC into every caller (retail inlines the loop).
static int lookupVolume(int volDb) {
    int* table = reinterpret_cast<int*>(lbl_eu_80523050);
    while (true) {
        int entryVal = table[0];
        if (entryVal < 0) return -960;
        if (volDb >= entryVal) return table[1];
        int next = table[2];
        if (volDb > next) {
            int val1 = table[1];
            int val2 = table[3];
            return val2 + (int)((float)(volDb - next) * (float)(val1 - val2) /
                                (float)(entryVal - next));
        }
        table += 2;
    }
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
    // Initialize stream entry flags to 0
    u8* base = (u8*)this;
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

// Destructor - cleans up all resources
CLibCriStreamingPlay::~CLibCriStreamingPlay() {
    u8* base = (u8*)this;
    
    // Clean up exception handler
    ((CException*)(base + 0x1C4))->func_804591DC(nullptr);
    
    // Free audio buffer
    if (lbl_eu_806656EC) {
        mtl::MemManager::deallocate(lbl_eu_806656EC);
        lbl_eu_806656EC = nullptr;
    }
    
    // Clear instance
    lbl_eu_806656E8 = nullptr;
    
    // Call base destructor (no delete)
    CWorkThread::~CWorkThread();
}

// func_8045B5AC - Start streaming playback.
// Retail has no `this`: r3 = filename, r4 = alloc handle, r5 = loop flag,
// r6 = AFS file id, r7 = AFS subfile id, r8 = force-AHX flag.
// Returns the stream id, or -1 on failure.
int func_8045B5AC(const char* filename, int param2, bool loopFlag,
                  int afsId, int afsSubId, bool forceAhx) {
    if (getFileSize__11CDeviceFileFPCc(filename, 0) < 0) {
        return -1;
    }

    StreamEntry* slot = findFreeSlot();
    if (slot == NULL) {
        return -1;
    }

    slot->flags = 0;
    if (isSoundModeMono__9CDeviceSCFv()) {
        slot->flags |= 0x40;
    } else {
        slot->flags &= ~0x40;
    }

    strncpy(slot->name, filename, 0x40);

    // .ahx extension or forced AHX mode selects the AHX decoder.
    if (strstr(filename, lbl_eu_806637A4) != NULL || forceAhx) {
        slot->flags |= 0x04;
    } else {
        slot->flags &= ~0x04;
    }

    slot->field_0x0C = (u32)param2;

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
        (u32)param2, slot->bufSize, 0x20));
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

    if (loopFlag) {
        slot->flags |= 0x20;
    } else {
        slot->flags &= ~0x20;
    }

    slot->field_0x88 = (u32)afsId;
    slot->field_0x8C = (u32)afsSubId;

    // Create the ADX decoder unless this is a preload-only request (0x10).
    if (!(slot->flags & 0x10)) {
        int ch2 = 2 - (int)((slot->flags >> 2) & 1);
        slot->adxt = reinterpret_cast<u8*>(ADXT_Create(ch2, slot->buffer, slot->bufSize));

        if (slot->flags & 0x04) {
            u32 idx = slot->slotId << 13;
            ADXT_AttachAhx(slot->adxt, reinterpret_cast<u8*>(lbl_eu_806656EC) + idx, 0x2000);
            ADXT_SetSvrFreq(slot->adxt, 0x1e);
        }

        if (afsId >= 0) {
            ADXT_StartAfs(slot->adxt, afsId, afsSubId);
        } else {
            ADXT_StartFnameRange(slot->adxt, filename);
        }

        // Mono streams get a centered pan.
        if ((slot->flags & 0x40) && !(slot->flags & 0x04)) {
            ADXT_SetOutPan(slot->adxt, 1, 0);
            ADXT_SetOutPan(slot->adxt, 0, 0);
        }

        // Block until playback actually starts when requested.
        if (loopFlag) {
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

// func_8045B970 - Check if stream ID is active
bool CLibCriStreamingPlay::func_8045B970(int id) {
    u8* inst = (u8*)lbl_eu_806656E8;
    u8* entry = inst + 0x1C8;
    
    // Check each of 5 stream slots
    for (int i = 0; i < 5; i++) {
        u32 slotId = *(u32*)(entry + 4);
        // Check if this slot matches the requested ID
        if (slotId == (u32)id && (slotId + 0x10000) != 0xFFFF) return true;
        // Check if slot is unused and we're looking for any active
        if ((slotId + 0x10000) != 0xFFFF && ((u32)id + 0x10000) == 0xFFFF) return true;
        entry += 0x94;
    }
    
    return false;
}

// func_8045BAB0 - Stop a specific stream
void CLibCriStreamingPlay::func_8045BAB0(int id) {
    StreamEntry* entry = reinterpret_cast<StreamEntry*>(
        reinterpret_cast<u8*>(lbl_eu_806656E8) + 0x1C8);
    StreamEntry* found = NULL;

    // Find stream by ID
    for (int i = 0; i < 5; i++) {
        if (entry->id == id) {
            found = entry;
            break;
        }
        entry++;
    }

    if (found == NULL) return;

    // Destroy ADXT handle
    if (found->adxt != NULL) {
        if (found->flags & 0x04) ADXT_DetachAhx(found->adxt);
        ADXT_Destroy(found->adxt);
        found->adxt = NULL;
    }

    // Free buffer
    if (found->buffer != NULL) {
        mtl::MemManager::deallocate(found->buffer);
        found->buffer = NULL;
    }

    // Mark as unused
    found->id = -1;
    found->flags = 0;
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

// func_8045BC4C - Update stream pause/volume state
void CLibCriStreamingPlay::func_8045BC4C(int id, bool pause) {
    u8* inst = (u8*)lbl_eu_806656E8;
    u8* entry = inst + 0x1C8;
    u8* found = nullptr;
    
    // Find stream
    for (int i = 0; i < 5; i++) {
        if (*(u32*)(entry + 4) == (u32)id) {
            found = entry;
            break;
        }
        entry += 0x94;
    }
    
    if (!found) return;
    
    // Set pause flag
    if (pause) {
        *(u32*)(found + 0x90) |= 1;
    } else {
        *(u32*)(found + 0x90) &= ~1;
    }
    
    // Calculate pause state including global pause and flow
    u32 flags = *(u32*)(found + 0x90);
    int pauseCount = *(int*)(inst + 0x4B0);
    bool isPaused = (flags & 1) || (pauseCount > 0);
    bool flowActive = CWorkControl::hasFlow();
    ADXT_Pause(*(void**)(found + 8), (isPaused || flowActive) ? 1 : 0);
    
    // Calculate volume: volume * field78 * field7C
    float vol = *(float*)(found + 0x64) * *(float*)(found + 0x78) * *(float*)(found + 0x7C);
    if (flowActive) vol = lbl_eu_8066A50C; // 1.0
    
    int volDb = (int)(lbl_eu_8066A510 * vol);
    int outVol = lookupVolume(volDb);
    ADXT_SetOutVol(*(void**)(found + 8), outVol);
}

// func_8045BE48 - Get playback position
int CLibCriStreamingPlay::func_8045BE48(int id) {
    u8* inst = (u8*)lbl_eu_806656E8;
    u8* entry = inst + 0x1C8;
    
    for (int i = 0; i < 5; i++) {
        if (*(u32*)(entry + 4) == (u32)id) {
            return *(int*)(entry + 0x84);
        }
        entry += 0x94;
    }
    return 0;
}

// wkUpdate - Main update loop for all streams
void CLibCriStreamingPlay::wkUpdate() {
    u8* base = (u8*)this;
    u8* entry = base + 0x1C8;
    
    for (int i = 0; i < 5; i++) {
        void* adxt = *(void**)(entry + 8);
        if (!adxt) goto nextEntry;
        
        float fadeTarget = *(float*)(entry + 0x70);
        int pauseCount = *(int*)(base + 0x4B0);
        int streamFlags = *(u32*)(entry + 0x90);
        
        // Check if fade is active (fadeTarget != 1.0, no pause, no manual pause)
        if (fadeTarget != lbl_eu_8066A50C || pauseCount != 0 || (streamFlags & 1)) {
            // Process fade timer
            float fadeTimer = *(float*)(entry + 0x74);
            fadeTimer += lbl_eu_8066A520; // +0.01 delta
            *(float*)(entry + 0x74) = fadeTimer;
            
            if (fadeTimer >= fadeTarget) {
                // Fade complete - apply target volume
                float targetVol = *(float*)(entry + 0x68);
                *(float*)(entry + 0x64) = targetVol;
                *(float*)(entry + 0x70) = lbl_eu_8066A50C; // Reset fade target to 1.0
                
                if (targetVol == lbl_eu_8066A50C) {
                    // Target volume reached - check action
                    int action = *(int*)(entry + 0x80);
                    if (action == 1) {
                        // Pause at end of fade
                        *(u32*)(entry + 0x90) |= 2;
                        u32 f = *(u32*)(entry + 0x90);
                        bool p = (f & 1) || (pauseCount > 0);
                        bool flow = CWorkControl::hasFlow();
                        ADXT_Pause(*(void**)(entry + 8), (p || flow) ? 1 : 0);
                    } else if (action == 2) {
                        // Stop at end of fade
                        void* a = *(void**)(entry + 8);
                        if (a) {
                            if (*(u32*)(entry + 0x90) & 4) ADXT_DetachAhx(a);
                            ADXT_Destroy(a);
                            *(void**)(entry + 8) = nullptr;
                        }
                        void* b = *(void**)(entry + 0x18);
                        if (b) {
                            mtl::MemManager::deallocate(b);
                            *(void**)(entry + 0x18) = nullptr;
                        }
                        *(u32*)(entry + 4) = -1;
                        *(u32*)(entry + 0x90) = 0;
                        goto nextEntry;
                    }
                }
            } else {
                // Interpolate volume during fade
                float startVol = *(float*)(entry + 0x6C);
                float curVol = *(float*)(entry + 0x64);
                float t = fadeTimer / fadeTarget;
                *(float*)(entry + 0x64) = startVol + (curVol - startVol) * t;
                
                // Handle pause-during-fade
                if (*(int*)(entry + 0x80) == 1 && (*(u32*)(entry + 0x90) & 2)) {
                    if (*(float*)(entry + 0x64) != lbl_eu_8066A50C) {
                        *(u32*)(entry + 0x90) &= ~2;
                        u32 f = *(u32*)(entry + 0x90);
                        bool p = (f & 1) || (pauseCount > 0);
                        bool flow = CWorkControl::hasFlow();
                        ADXT_Pause(*(void**)(entry + 8), (p || flow) ? 1 : 0);
                    }
                }
            }
        }
        
        // Update volume
        {
            float vol = *(float*)(entry + 0x64) * *(float*)(entry + 0x78) * *(float*)(entry + 0x7C);
            bool flow = CWorkControl::hasFlow();
            if (flow) vol = lbl_eu_8066A50C;
            int volDb = (int)(lbl_eu_8066A510 * vol);
            int outVol = lookupVolume(volDb);
            ADXT_SetOutVol(*(void**)(entry + 8), outVol);
        }
        
        // Check playback status
        {
            int stat = ADXT_GetStat(*(void**)(entry + 8));
            if (stat == 5 || stat == 6) {
                // Playback ended - cleanup
                void* a = *(void**)(entry + 8);
                if (a) {
                    if (*(u32*)(entry + 0x90) & 4) ADXT_DetachAhx(a);
                    ADXT_Destroy(a);
                    *(void**)(entry + 8) = nullptr;
                }
                void* b = *(void**)(entry + 0x18);
                if (b) {
                    mtl::MemManager::deallocate(b);
                    *(void**)(entry + 0x18) = nullptr;
                }
                *(u32*)(entry + 4) = -1;
                *(u32*)(entry + 0x90) = 0;
            } else if (stat != 0) {
                // Active playback - update time position
                const char* baseStr = lbl_eu_805230B8;
                const char* statusStr = nullptr;
                
                switch (stat) {
                    case 0: statusStr = baseStr; break;
                    case 1: statusStr = baseStr + 7; break;
                    case 2: statusStr = baseStr + 30; break;
                    case 3: statusStr = baseStr + 41; break;
                    case 4: statusStr = baseStr + 58; break;
                    case 5: statusStr = baseStr + 71; break;
                    case 6: statusStr = baseStr + 80; break;
                }
                
                // Update playback position from time
                int time = ADXT_GetTimeReal(*(void**)(entry + 8));
                float timeF = (float)time;
                float pos = timeF / lbl_eu_8066A510 / lbl_eu_8066A520;
                *(u32*)(entry + 0x84) = (u32)pos;
            }
        }
        
    nextEntry:
        entry += 0x94;
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

// OnPauseTrigger - Handle pause/unpause
void CLibCriStreamingPlay::OnPauseTrigger(bool paused) {
    u8* base = (u8*)this;
    int* pauseCounter = (int*)(base + 0x4B0);
    
    if (paused) {
        (*pauseCounter)++;
    } else {
        (*pauseCounter)--;
        if (*pauseCounter < 0) *pauseCounter = 0;
    }
    
    // Update all active streams
    u8* entry = base + 0x1C8;
    for (int i = 0; i < 5; i++) {
        u32 slotId = *(u32*)(entry + 4);
        if ((slotId + 0x10000) == 0xFFFF) goto nextPause; // unused
        
        // Set pause state
        u32 flags = *(u32*)(entry + 0x90);
        bool isPaused = (flags & 1) || (*pauseCounter > 0);
        bool flowActive = CWorkControl::hasFlow();
        ADXT_Pause(*(void**)(entry + 8), (isPaused || flowActive) ? 1 : 0);
        
        // Update volume
        float vol = *(float*)(entry + 0x64) * *(float*)(entry + 0x78) * *(float*)(entry + 0x7C);
        if (flowActive) vol = lbl_eu_8066A50C;
        int volDb = (int)(lbl_eu_8066A510 * vol);
        int outVol = lookupVolume(volDb);
        ADXT_SetOutVol(*(void**)(entry + 8), outVol);
        
    nextPause:
        entry += 0x94;
    }
}

// func_8045C67C - Get current volume for stream
float CLibCriStreamingPlay::func_8045C67C(int id) {
    u8* inst = (u8*)lbl_eu_806656E8;
    u8* entry = inst + 0x1C8;
    u8* found = nullptr;
    
    for (int i = 0; i < 5; i++) {
        if (*(u32*)(entry + 4) == (u32)id) {
            found = entry;
            break;
        }
        entry += 0x94;
    }
    
    if (!found) return lbl_eu_8066A50C; // 1.0
    return *(float*)(found + 0x64);
}

// func_8045C700 - Set volume immediately
void CLibCriStreamingPlay::func_8045C700(int id, float volume) {
    u8* inst = (u8*)lbl_eu_806656E8;
    u8* entry = inst + 0x1C8;
    u8* found = nullptr;
    
    for (int i = 0; i < 5; i++) {
        if (*(u32*)(entry + 4) == (u32)id) {
            found = entry;
            break;
        }
        entry += 0x94;
    }
    
    if (!found) return;
    
    // Set volume and fade parameters
    *(float*)(found + 0x64) = volume;
    float vol = (float)volume;
    *(float*)(found + 0x68) = volume;
    *(float*)(found + 0x6C) = vol;
    *(float*)(found + 0x70) = lbl_eu_8066A50C; // 1.0
    *(float*)(found + 0x74) = lbl_eu_8066A50C;
    
    // Calculate and set output volume
    float totalVol = vol * *(float*)(found + 0x78) * *(float*)(found + 0x7C);
    bool flow = CWorkControl::hasFlow();
    if (flow) totalVol = lbl_eu_8066A50C;
    int volDb = (int)(lbl_eu_8066A510 * totalVol);
    int outVol = lookupVolume(volDb);
    ADXT_SetOutVol(*(void**)(found + 8), outVol);
}

// func_8045C8B0 - Set volume multiplier
void CLibCriStreamingPlay::func_8045C8B0(int id, float volume) {
    u8* inst = (u8*)lbl_eu_806656E8;
    u8* entry = inst + 0x1C8;
    u8* found = nullptr;
    
    for (int i = 0; i < 5; i++) {
        if (*(u32*)(entry + 4) == (u32)id) {
            found = entry;
            break;
        }
        entry += 0x94;
    }
    
    if (!found) return;
    
    // Set volume multiplier
    *(float*)(found + 0x7C) = (float)volume;
    
    // Calculate and set output volume
    float totalVol = *(float*)(found + 0x64) * *(float*)(found + 0x78) * (float)volume;
    bool flow = CWorkControl::hasFlow();
    if (flow) totalVol = lbl_eu_8066A50C;
    int volDb = (int)(lbl_eu_8066A510 * totalVol);
    int outVol = lookupVolume(volDb);
    ADXT_SetOutVol(*(void**)(found + 8), outVol);
}

// func_8045CA4C - Start a volume fade.
// Retail has no `this`: r3 = stream id, r4 = fade-end action, f1 = volume,
// f2 = fade time (see the CLibCri forwarding trampoline func_80459A88).
void func_8045CA4C(int id, int action, float volume, float fadeTime) {
    // Immediate volume change when the fade time is <= 0.
    if (fadeTime <= lbl_eu_8066A50C) {
        StreamEntry* slot = findStreamSlot((u32)id);
        if (slot != NULL) {
            slot->field_0x64 = volume;
            float level = volume;
            slot->field_0x68 = volume;
            slot->field_0x6C = level;
            float zero = lbl_eu_8066A50C;
            slot->field_0x70 = zero;
            slot->field_0x74 = zero;

            // Total output volume: level * 0x78 * 0x7C (0 during a flow).
            float totalVol = level * slot->field_0x78 * slot->field_0x7C;
            if (CWorkControl::hasFlow()) {
                totalVol = lbl_eu_8066A50C;
            }
            int volDb = (int)(lbl_eu_8066A510 * totalVol);
            ADXT_SetOutVol(slot->adxt, lookupVolume(volDb));
        }
    }

    // Always (re)set the fade parameters on the target entry.
    StreamEntry* slot = findStreamSlot((u32)id);
    if (slot != NULL) {
        slot->field_0x68 = volume;
        float zero = lbl_eu_8066A50C;
        slot->field_0x6C = slot->field_0x64;
        if (volume == zero) {
            slot->field_0x80 = action;
        }
        slot->field_0x70 = fadeTime;
        slot->field_0x74 = zero;
        if (slot->field_0x70 <= zero) {
            slot->field_0x70 = zero;
        }
    }
}

// func_8045CCFC - Set pan/volume from a 3D position pair.
// Retail has no `this`: r3 = stream id, r4/r5 = position pointers,
// f1/f2/f3 = the pan/volume parameters (see func_8049B834).
void func_8045CCFC(int id, const u8* in2, const u8* in1,
                   float a, float b, float c) {
    StreamEntry* slot = findStreamSlot((u32)id);
    if (slot == NULL) return;

    // Compute base volume/pan from the two 3D positions.
    float pan, vol;
    func_8049B834(a, b, &pan, &vol, in1, in2);

    // Clamp the volume to the caller's minimum level.
    if (vol < c) {
        vol = c;
    }
    float level = vol;
    slot->field_0x78 = level;

    // Total output volume: current * level * multiplier (0 during a flow).
    float totalVol = slot->field_0x64 * level * slot->field_0x7C;
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

// func_8045CFDC - Calculate buffer size for stream
int CLibCriStreamingPlay::func_8045CFDC(int channels) {
    u32 rate = lbl_eu_806637A0;
    int neg = -channels;
    int sign = (neg | channels) >> 31;
    int ch = sign + 2;
    
    u32 r3 = rate / 10000;
    int bufSize = (int)(r3 * ch * 25000);
    bufSize = bufSize / 1000 * 6 + ch * 24768;
    bufSize = (bufSize + 0x800) & ~0x7FF;
    return bufSize + 100;
}

// func_8045D03C - Check if stream is playing/paused
bool CLibCriStreamingPlay::func_8045D03C(int id) {
    CLibCriStreamingPlay* inst = lbl_eu_806656E8;
    if (inst == NULL) return false;

    StreamEntry* entry = reinterpret_cast<StreamEntry*>(
        reinterpret_cast<u8*>(inst) + 0x1C8);
    StreamEntry* found = NULL;

    // Find stream by ID
    for (int i = 0; i < 5; i++) {
        if (entry->id == id) {
            found = entry;
            break;
        }
        entry++;
    }

    if (found == NULL) return false;
    if (found->adxt == NULL) return false;

    // Paused when: stream pause flag set, global pause counter non-zero,
    // or the flow is active.
    u32 flags = found->flags;
    bool flow = CWorkControl::hasFlow();
    CLibCriStreamingPlayData* data = reinterpret_cast<CLibCriStreamingPlayData*>(
        reinterpret_cast<u8*>(inst) + 0x1C8);
    s32 pauseCount = data->pauseCount;

    return flow || (pauseCount != 0) || (flags & 1) || (flags & 2);
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
