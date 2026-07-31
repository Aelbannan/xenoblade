// CLibCriStreamingPlay - CRI streaming audio playback manager

#include "monolib/lib/CLibCriStreamingPlay.hpp"
#include <cstring>

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
    
    // WorkControl
    bool hasFlow__12CWorkControlFv(void* thisPtr);
    
    // Pan calculation
    void func_8049B834(float p1, float p2, float p3, float* o1, float* o2);
    
    // Memory management
    u32 getHandleMEM2__Q23mtl10MemManagerFv(void);
    void* allocate_head__Q23mtl10MemManagerFUlUli(u32 handle, u32 size, int align);
    void* allocate_tail__Q23mtl10MemManagerFUlUli(u32 handle, u32 size, int align);
    void deallocate__Q23mtl10MemManagerFPv(void* ptr);
    
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

// Volume lookup helper - searches table for volume dB value
static int lookupVolume(int volDb) {
    u32* table = lbl_eu_80523050;
    while (1) {
        int entry = *(int*)table;
        if (entry < 0) return -960;
        if (volDb >= entry) return *(int*)(table + 1);
        int next = *(int*)(table + 2);
        if (volDb > next) {
            int val1 = *(int*)(table + 1);
            int val2 = *(int*)(table + 3);
            // Linear interpolation
            float result = (float)(val1 - val2) * (float)(volDb - next) / (float)(entry - next);
            return val2 + (int)result;
        }
        table += 2;
    }
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
    func_804591BC__10CExceptionFP10IException(base + 0x1C4, nullptr);
}

// Destructor - cleans up all resources
CLibCriStreamingPlay::~CLibCriStreamingPlay() {
    u8* base = (u8*)this;
    
    // Clean up exception handler
    func_804591DC__10CExceptionFP10IException(base + 0x1C4, nullptr);
    
    // Free audio buffer
    if (lbl_eu_806656EC) {
        deallocate__Q23mtl10MemManagerFPv(lbl_eu_806656EC);
        lbl_eu_806656EC = nullptr;
    }
    
    // Clear instance
    lbl_eu_806656E8 = nullptr;
    
    // Call base destructor (no delete)
    CWorkThread::~CWorkThread();
}

// func_8045B5AC - Start streaming playback
// Returns stream ID on success, -1 on failure
int CLibCriStreamingPlay::func_8045B5AC(const char* filename, int param2, bool loopFlag, int afsId, int afsSubId, bool forceAhx) {
    u8* base = (u8*)this;
    
    // Check file exists
    int fileSize = getFileSize__11CDeviceFileFPCc(filename, 0);
    if (fileSize < 0) return -1;
    
    // Find free stream slot
    u8* inst = (u8*)lbl_eu_806656E8;
    u8* entry = inst + 0x1C8;
    u8* found = nullptr;
    
    for (int i = 0; i < 5; i++) {
        u32 slotId = *(u32*)(entry + 4);
        if ((slotId + 0x10000) == 0xFFFF) { // unused marker
            found = entry;
            break;
        }
        entry += 0x94;
    }
    
    if (!found) return -1;
    
    // Clear flags
    *(u32*)(found + 0x90) = 0;
    
    // Check mono mode
    if (isSoundModeMono__9CDeviceSCFv()) {
        *(u32*)(found + 0x90) |= 0x40;
    } else {
        *(u32*)(found + 0x90) &= ~0x40;
    }
    
    // Copy filename
    strncpy((char*)(found + 0x1C), filename, 64);
    
    // Check for .ahx extension or forced AHX mode
    bool useAhx = (strstr(filename, lbl_eu_806637A4) != nullptr) || forceAhx;
    if (useAhx) {
        *(u32*)(found + 0x90) |= 0x04;
    } else {
        *(u32*)(found + 0x90) &= ~0x04;
    }
    
    // Store source parameter
    *(u32*)(found + 0x0C) = param2;
    
    // Calculate buffer size
    u32 rate = lbl_eu_806637A0;
    int channels = (*(u32*)(found + 0x90) & 0x04) ? 2 : 2;
    u32 bufSize = (rate / 10000) * 6 * 0x800 + channels * 0x60C0 + 100;
    *(u32*)(found + 0x14) = bufSize;
    
    // Allocate streaming buffer
    u32 mem2 = getHandleMEM2__Q23mtl10MemManagerFv();
    void* buf = allocate_tail__Q23mtl10MemManagerFUlUli(mem2, bufSize, 4);
    *(void**)(found + 0x18) = buf;
    
    if (!buf) {
        // Cleanup on allocation failure
        if (found) {
            void* adxt = *(void**)(found + 8);
            if (adxt) {
                if (*(u32*)(found + 0x90) & 0x04) ADXT_DetachAhx(adxt);
                ADXT_Destroy(adxt);
                *(void**)(found + 8) = nullptr;
            }
            void* buffer = *(void**)(found + 0x18);
            if (buffer) {
                deallocate__Q23mtl10MemManagerFPv(buffer);
                *(void**)(found + 0x18) = nullptr;
            }
            *(u32*)(found + 4) = -1;
            *(u32*)(found + 0x90) = 0;
        }
        return -1;
    }
    
    // Generate unique stream ID
    u32 counter = *(u32*)(inst + 0x4AC);
    u32 slotIdx = *(u32*)(found);
    u32 streamId = ((slotIdx + 1) & 0xFF) | ((counter & 0xFF) << 8);
    *(u32*)(inst + 0x4AC) = counter + 1;
    *(u32*)(found + 4) = streamId;
    
    // Initialize playback state
    *(u32*)(found + 0x5C) = 0;
    *(u32*)(found + 0x60) = 0;
    *(float*)(found + 0x64) = lbl_eu_8066A508; // 0.0
    *(float*)(found + 0x7C) = lbl_eu_8066A508;
    *(float*)(found + 0x68) = lbl_eu_8066A508;
    *(float*)(found + 0x6C) = lbl_eu_8066A508;
    *(float*)(found + 0x70) = lbl_eu_8066A50C; // 1.0
    *(float*)(found + 0x74) = lbl_eu_8066A50C;
    *(u32*)(found + 0x80) = 0;
    *(u32*)(found + 0x84) = 0;
    *(float*)(found + 0x78) = lbl_eu_8066A508;
    
    // Set loop flag
    if (loopFlag) {
        *(u32*)(found + 0x90) |= 0x20;
    } else {
        *(u32*)(found + 0x90) &= ~0x20;
    }
    
    // Store AFS parameters
    *(u32*)(found + 0x88) = afsId;
    *(u32*)(found + 0x8C) = afsSubId;
    
    // Check if not in preload-only mode
    if (!(*(u32*)(found + 0x90) & 0x08)) {
        int ch = (*(u32*)(found + 0x90) & 0x04) ? 2 : 2;
        void* adxt = ADXT_Create(ch, *(void**)(found + 0x18), *(u32*)(found + 0x14));
        *(void**)(found + 8) = adxt;
        
        // Attach AHX data if needed
        if (*(u32*)(found + 0x90) & 0x04) {
            u32 idx = *(u32*)(found);
            void* ahxData = (u8*)lbl_eu_806656EC + (idx << 13);
            ADXT_AttachAhx(adxt, ahxData, 0x2000);
            ADXT_SetSvrFreq(adxt, 30000);
        }
        
        // Start playback
        if (afsId >= 0) {
            ADXT_StartAfs(*(void**)(found + 8), afsId, afsSubId);
        } else {
            ADXT_StartFnameRange(*(void**)(found + 8), filename);
        }
        
        // Set mono pan if needed
        u32 f = *(u32*)(found + 0x90);
        if ((f & 0x20) && !(f & 0x04)) {
            ADXT_SetOutPan(*(void**)(found + 8), 1, 0);
            ADXT_SetOutPan(*(void**)(found + 8), 0, 0);
        }
        
        // Wait for playback to start if blocking
        if (loopFlag) {
            int waitCount = 100;
            while (waitCount > 0) {
                ADXM_ExecMain();
                VIWaitForRetrace();
                if (ADXT_GetStat(*(void**)(found + 8)) == 3) break;
                waitCount--;
            }
        }
    }
    
    return *(int*)(found + 4);
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
    u8* inst = (u8*)lbl_eu_806656E8;
    u8* entry = inst + 0x1C8;
    u8* found = nullptr;
    
    // Find stream by ID
    for (int i = 0; i < 5; i++) {
        if (*(u32*)(entry + 4) == (u32)id) {
            found = entry;
            break;
        }
        entry += 0x94;
    }
    
    if (!found) return;
    
    // Destroy ADXT handle
    void* adxt = *(void**)(found + 8);
    if (adxt) {
        if (*(u32*)(found + 0x90) & 0x04) ADXT_DetachAhx(adxt);
        ADXT_Destroy(adxt);
        *(void**)(found + 8) = nullptr;
    }
    
    // Free buffer
    void* buf = *(void**)(found + 0x18);
    if (buf) {
        deallocate__Q23mtl10MemManagerFPv(buf);
        *(void**)(found + 0x18) = nullptr;
    }
    
    // Mark as unused
    *(u32*)(found + 4) = -1;
    *(u32*)(found + 0x90) = 0;
}

// func_8045BBA0 - Stop all streams
void CLibCriStreamingPlay::func_8045BBA0() {
    u8* inst = (u8*)lbl_eu_806656E8;
    u8* entry = inst + 0x1C8;
    
    for (int i = 0; i < 5; i++) {
        // Destroy ADXT handle
        void* adxt = *(void**)(entry + 8);
        if (adxt) {
            if (*(u32*)(entry + 0x90) & 0x04) ADXT_DetachAhx(adxt);
            ADXT_Destroy(adxt);
            *(void**)(entry + 8) = nullptr;
        }
        
        // Free buffer
        void* buf = *(void**)(entry + 0x18);
        if (buf) {
            deallocate__Q23mtl10MemManagerFPv(buf);
            *(void**)(entry + 0x18) = nullptr;
        }
        
        // Mark unused
        *(u32*)(entry + 4) = -1;
        *(u32*)(entry + 0x90) = 0;
        
        entry += 0x94;
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
    bool flowActive = hasFlow__12CWorkControlFv(this);
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
                        bool flow = hasFlow__12CWorkControlFv(this);
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
                            deallocate__Q23mtl10MemManagerFPv(b);
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
                        bool flow = hasFlow__12CWorkControlFv(this);
                        ADXT_Pause(*(void**)(entry + 8), (p || flow) ? 1 : 0);
                    }
                }
            }
        }
        
        // Update volume
        {
            float vol = *(float*)(entry + 0x64) * *(float*)(entry + 0x78) * *(float*)(entry + 0x7C);
            bool flow = hasFlow__12CWorkControlFv(this);
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
                    deallocate__Q23mtl10MemManagerFPv(b);
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

// wkStandbyLogin - Just calls base class
bool CLibCriStreamingPlay::wkStandbyLogin() {
    wkStandbyLogin__11CWorkThreadFv(this);
    return true;
}

// wkStandbyLogout - Cleanup all streams on logout
bool CLibCriStreamingPlay::wkStandbyLogout() {
    u8* inst = (u8*)lbl_eu_806656E8;
    
    // Check work queue
    void* queue = *(void**)((u8*)this + 0x60);
    if (*(void**)queue != queue) {
        return false;
    }
    
    // Stop all streams
    u8* entry = inst + 0x1C8;
    for (int i = 0; i < 5; i++) {
        void* adxt = *(void**)(entry + 8);
        if (adxt) {
            if (*(u32*)(entry + 0x90) & 4) ADXT_DetachAhx(adxt);
            ADXT_Destroy(adxt);
            *(void**)(entry + 8) = nullptr;
        }
        void* buf = *(void**)(entry + 0x18);
        if (buf) {
            deallocate__Q23mtl10MemManagerFPv(buf);
            *(void**)(entry + 0x18) = nullptr;
        }
        *(u32*)(entry + 4) = -1;
        *(u32*)(entry + 0x90) = 0;
        entry += 0x94;
    }
    
    // Call base logout
    wkStandbyLogout__11CWorkThreadFv(this);
    return true;
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
        bool flowActive = hasFlow__12CWorkControlFv(this);
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
    bool flow = hasFlow__12CWorkControlFv(this);
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
    bool flow = hasFlow__12CWorkControlFv(this);
    if (flow) totalVol = lbl_eu_8066A50C;
    int volDb = (int)(lbl_eu_8066A510 * totalVol);
    int outVol = lookupVolume(volDb);
    ADXT_SetOutVol(*(void**)(found + 8), outVol);
}

// func_8045CA4C - Start volume fade
void CLibCriStreamingPlay::func_8045CA4C(int id, float volume, float fadeTime, int action) {
    u8* inst = (u8*)lbl_eu_806656E8;
    
    // If fadeTime <= 0, apply immediately
    if (fadeTime <= lbl_eu_8066A50C) {
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
        
        *(float*)(found + 0x64) = volume;
        float vol = (float)volume;
        *(float*)(found + 0x68) = volume;
        *(float*)(found + 0x6C) = vol;
        *(float*)(found + 0x70) = lbl_eu_8066A50C;
        *(float*)(found + 0x74) = lbl_eu_8066A50C;
        
        float totalVol = vol * *(float*)(found + 0x78) * *(float*)(found + 0x7C);
        bool flow = hasFlow__12CWorkControlFv(this);
        if (flow) totalVol = lbl_eu_8066A50C;
        int volDb = (int)(lbl_eu_8066A510 * totalVol);
        int outVol = lookupVolume(volDb);
        ADXT_SetOutVol(*(void**)(found + 8), outVol);
    }
    
    // Find entry and set up fade parameters
    u8* entry2 = inst + 0x1C8;
    u8* found2 = nullptr;
    
    for (int i = 0; i < 5; i++) {
        if (*(u32*)(entry2 + 4) == (u32)id) {
            found2 = entry2;
            break;
        }
        entry2 += 0x94;
    }
    
    if (!found2) return;
    
    // Set fade parameters
    *(float*)(found2 + 0x68) = volume;
    float curVol = *(float*)(found2 + 0x64);
    *(float*)(found2 + 0x6C) = curVol;
    
    if (volume == lbl_eu_8066A50C) {
        *(int*)(found2 + 0x80) = action;
    }
    
    *(float*)(found2 + 0x70) = fadeTime;
    *(float*)(found2 + 0x74) = lbl_eu_8066A50C; // 0.0
    
    // Clamp fade target to >= 0
    if (*(float*)(found2 + 0x70) <= lbl_eu_8066A50C) {
        *(float*)(found2 + 0x70) = lbl_eu_8066A50C;
    }
}

// func_8045CCFC - Set pan and volume
void CLibCriStreamingPlay::func_8045CCFC(int id, float param2, float param3, float param4) {
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
    
    // Calculate pan/volume from parameters
    float outVol, outPan;
    func_8049B834(param2, param3, param4, &outVol, &outPan);
    
    // Clamp volume
    if (outVol < (float)param4) {
        outVol = (float)param4;
    }
    
    // Store and apply volume
    *(float*)(found + 0x78) = (float)outVol;
    
    float totalVol = *(float*)(found + 0x64) * (float)outVol * *(float*)(found + 0x7C);
    bool flow = hasFlow__12CWorkControlFv(this);
    if (flow) totalVol = lbl_eu_8066A50C;
    int volDb = (int)(lbl_eu_8066A510 * totalVol);
    int outVolDb = lookupVolume(volDb);
    ADXT_SetOutVol(*(void**)(found + 8), outVolDb);
    
    // Set pan if not in stereo mode
    u32 flags = *(u32*)(found + 0x90);
    if (!(flags & 0x20)) {
        float panF = lbl_eu_8066A524 * outPan; // 30.0 * pan
        int pan = (int)panF;
        if (pan < -15) pan = -15;
        if (pan > 15) pan = 15;
        
        if (!(flags & 0x04)) {
            ADXT_SetOutPan(*(void**)(found + 8), 1, pan);
        }
        ADXT_SetOutPan(*(void**)(found + 8), 0, pan);
    }
}

// func_8045CF30 - Stop all streams (alternate entry)
void CLibCriStreamingPlay::func_8045CF30() {
    u8* inst = (u8*)lbl_eu_806656E8;
    u8* entry = inst + 0x1C8;
    
    for (int i = 0; i < 5; i++) {
        void* adxt = *(void**)(entry + 8);
        if (adxt) {
            if (*(u32*)(entry + 0x90) & 4) ADXT_DetachAhx(adxt);
            ADXT_Destroy(adxt);
            *(void**)(entry + 8) = nullptr;
        }
        void* buf = *(void**)(entry + 0x18);
        if (buf) {
            deallocate__Q23mtl10MemManagerFPv(buf);
            *(void**)(entry + 0x18) = nullptr;
        }
        *(u32*)(entry + 4) = -1;
        *(u32*)(entry + 0x90) = 0;
        entry += 0x94;
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
    u8* inst = (u8*)lbl_eu_806656E8;
    if (!inst) return false;
    
    u8* entry = inst + 0x1C8;
    u8* found = nullptr;
    
    for (int i = 0; i < 5; i++) {
        if (*(u32*)(entry + 4) == (u32)id) {
            found = entry;
            break;
        }
        entry += 0x94;
    }
    
    if (!found) return false;
    if (!*(void**)(found + 8)) return false;
    
    // Check if paused or in pause-transition
    u32 flags = *(u32*)(found + 0x90);
    int pauseCount = *(int*)(inst + 0x4B0);
    bool isPaused = (flags & 1) || (pauseCount > 0);
    bool flowActive = hasFlow__12CWorkControlFv(this);
    
    return isPaused || flowActive || (flags & 2);
}

// func_8045D140 - CDeviceVICb thunk: adjust this and call func_8045CF30
void CLibCriStreamingPlay::func_8045D140() {
    CLibCriStreamingPlay* base = (CLibCriStreamingPlay*)((u8*)this - 0x1C4);
    base->func_8045CF30();
}

// func_8045D148 - CDeviceVICb thunk: adjust this and call destructor
void CLibCriStreamingPlay::func_8045D148() {
    CLibCriStreamingPlay* base = (CLibCriStreamingPlay*)((u8*)this - 0x1C4);
    base->~CLibCriStreamingPlay();
}
