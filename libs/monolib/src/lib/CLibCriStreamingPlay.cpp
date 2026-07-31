// CLibCriStreamingPlay - CRI streaming audio playback manager
// Inherits from CLibCri (which inherits CWorkThread, CDeviceVICb, IErrorWii)

#include "monolib/lib/CLibCriStreamingPlay.hpp"
#include <harness_catalog.h>
#include <stdint.h>
#include <string.h>

// External declarations for called functions
extern "C" {
    // ADXT functions
    extern ADXT ADXT_Create(int channels, void* buffer, u32 bufSize);
    extern void ADXT_Destroy(ADXT adxt);
    extern void ADXT_StartFnameRange(ADXT adxt, const char* filename);
    extern void ADXT_StartAfs(ADXT adxt, int fileId, int subfileId);
    extern void ADXT_Pause(ADXT adxt, int pause);
    extern int ADXT_GetStat(ADXT adxt);
    extern int ADXT_GetTimeReal(ADXT adxt);
    extern void ADXT_SetOutVol(ADXT adxt, int vol);
    extern void ADXT_SetOutPan(ADXT adxt, int ch, int pan);
    extern void ADXT_SetSvrFreq(ADXT adxt, int freq);
    extern void ADXT_AttachAhx(ADXT adxt, void* data, u32 size);
    extern void ADXT_DetachAhx(ADXT adxt);
    extern void ADXM_ExecMain(void);
    
    // CDevice functions
    extern int getFileSize__11CDeviceFileFPCc(const char* filename, int param);
    extern bool isSoundModeMono__9CDeviceSCFv(void);
    
    // Memory management
    extern void* getHandleMEM2__Q23mtl10MemManagerFv(void);
    extern void* allocate_head__Q23mtl10MemManagerFUlUli(void* handle, u32 size, u32 align);
    extern void* allocate_tail__Q23mtl10MemManagerFUlUli(void* handle, u32 size, u32 align);
    extern void deallocate__Q23mtl10MemManagerFPv(void* ptr);
    
    // CWorkThread
    extern void __ct__11CWorkThreadFPCcP11CWorkThreadi(const char*, void*, int);
    extern void __dt__11CWorkThreadFv(void* thisPtr, int flags);
    extern void wkStandbyLogin__11CWorkThreadFv(void* thisPtr);
    extern void wkStandbyLogout__11CWorkThreadFv(void* thisPtr);
    
    // CWorkControl
    extern bool hasFlow__12CWorkControlFv(void* thisPtr);
    
    // CException
    extern void func_804591BC__10CExceptionFP10IException(void* exception, void* param);
    extern void func_804591DC__10CExceptionFP10IException(void* exception, void* param);
    
    // Utility
    extern void func_8049B834(float param1, float param2, float param3, float* out1, float* out2);
    extern void __cvt_fp2unsigned(float f);
    
    // CRT
    extern void memset(void* dest, int val, size_t count);
    extern void strncpy(char* dest, const char* src, size_t count);
    extern char* strstr(const char* haystack, const char* needle);
    extern size_t strlen(const char* str);
    extern char* strcpy(char* dest, const char* src);
    extern void VIWaitForRetrace(void);
    extern void __dl__FPv(void* ptr);
    
    // Global state
    extern void* lbl_eu_806656E8;  // CLibCriStreamingPlay instance pointer
    extern void* lbl_eu_806656EC;  // MEM2 audio buffer
    extern const char* lbl_eu_806637A4;  // ".ahx" extension
    extern u32 lbl_eu_806637A0;    // sample rate divisor
    extern float lbl_eu_8066A508;  // 0.0f
    extern float lbl_eu_8066A50C;  // 1.0f  
    extern float lbl_eu_8066A510;  // 960.0f (volume scale)
    extern double lbl_eu_8066A518; // 2^52+2^31 double for int conversion
    extern float lbl_eu_8066A520;  // 0.01f (delta time)
    extern float lbl_eu_8066A524;  // 30.0f (pan scale)
    extern u32 lbl_eu_80523050[];  // volume lookup table
    extern const char lbl_eu_805230B8[]; // status strings
}

// Static instance and buffer pointers
static CLibCriStreamingPlay* sInstance = nullptr;
static void* sAudioBuffer = nullptr;

// Helper to find a stream entry by ID
static CStreamEntry* FindStreamEntry(int id) {
    CLibCriStreamingPlay* inst = (CLibCriStreamingPlay*)sInstance;
    if (!inst) return nullptr;
    
    CStreamEntry* entry = (CStreamEntry*)((u8*)inst + 0x1C8);
    for (int i = 0; i < 5; i++) {
        if (entry->mField4 == id) return entry;
        entry++;
    }
    return nullptr;
}

// Helper to look up volume from table
static int LookupVolume(int volIndex) {
    // Table-based volume lookup with linear interpolation
    u32* table = lbl_eu_80523050;
    int entry = *(int*)table;
    while (entry >= 0) {
        if (volIndex >= entry) {
            return *(int*)(table + 1);
        }
        int nextEntry = *(int*)(table + 2);
        if (volIndex > nextEntry) {
            int range = *(int*)(table + 3);
            int baseVal = *(int*)(table + 1);
            // Linear interpolation
            float t = (float)(volIndex - nextEntry) / (float)(entry - nextEntry);
            return baseVal + (int)((baseVal - range) * t);
        }
        table += 2;
        entry = *(int*)table;
    }
    return -960; // Minimum volume
}

// Constructor (us-8045f418)
CLibCriStreamingPlay::CLibCriStreamingPlay() {
    __ct__11CWorkThreadFPCcP11CWorkThreadi("CLibCriStreamingPlay", nullptr, 0);
    
    // Initialize CDeviceVICb vtable
    // (vtable setup happens through the base class chain)
    
    // Zero out stream entries
    CStreamEntry* entries = (CStreamEntry*)((u8*)this + 0x25C);
    for (int i = 0; i < 5; i++) {
        entries[i].mStreamFlags = 0;
    }
    
    // Initialize fields
    *(u32*)((u8*)this + 0x4AC) = 0;
    *(u32*)((u8*)this + 0x4B0) = 0;
    
    // Store instance pointer
    sInstance = this;
    
    // Set device type
    *(u32*)((u8*)this + 0x50) = 0x11;
    
    // Initialize exception handler area
    memset((u8*)this + 0x1C8, 0, 0x2E4);
    
    // Initialize default stream parameters
    CStreamEntry* e0 = (CStreamEntry*)((u8*)this + 0x1C8);
    e0->mField4 = 0;
    e0->mField4 = -1;
    
    CStreamEntry* e1 = (CStreamEntry*)((u8*)this + 0x25C);
    e1->mId = 1;
    e1->mField4 = -1;
    
    CStreamEntry* e2 = (CStreamEntry*)((u8*)this + 0x2F0);
    e2->mId = 2;
    e2->mField4 = -1;
    
    CStreamEntry* e3 = (CStreamEntry*)((u8*)this + 0x384);
    e3->mId = 3;
    e3->mField4 = -1;
    
    CStreamEntry* e4 = (CStreamEntry*)((u8*)this + 0x418);
    e4->mId = 4;
    e4->mField4 = -1;
    
    // Allocate MEM2 audio buffer (1MB - 0x6000)
    void* mem2 = getHandleMEM2__Q23mtl10MemManagerFv();
    sAudioBuffer = allocate_head__Q23mtl10MemManagerFUlUli(mem2, 0x10000 - 0x6000, 4);
    
    // Initialize exception
    void* excPtr = this ? (u8*)this + 0x1C4 : nullptr;
    func_804591BC__10CExceptionFP10IException(excPtr, nullptr);
}

// Destructor (us-8045f534)
CLibCriStreamingPlay::~CLibCriStreamingPlay() {
    if (!this) return;
    
    // Restore vtables
    // (vtable restoration through base class chain)
    
    // Clean up exception
    void* excPtr = (u8*)this + 0x1C4;
    func_804591DC__10CExceptionFP10IException(excPtr, nullptr);
    
    // Free audio buffer
    if (sAudioBuffer) {
        deallocate__Q23mtl10MemManagerFPv(sAudioBuffer);
        sAudioBuffer = nullptr;
    }
    
    // Clear instance
    sInstance = nullptr;
    
    // Call base class destructor
    __dt__11CWorkThreadFv(this, 0);
    
    // Free memory if requested
    // (flags parameter from compiler-generated dtor)
}

// func_8045B5AC - Start streaming playback (us-8045f5d8)
// Returns stream ID on success, -1 on failure
int CLibCriStreamingPlay::func_8045B5AC(const char* filename, int param2, bool param3, int param4, int param5, bool param6) {
    // Check file exists
    int fileSize = getFileSize__11CDeviceFileFPCc(filename, 0);
    if (fileSize < 0) return -1;
    
    // Find free stream entry
    CStreamEntry* entry = (CStreamEntry*)((u8*)sInstance + 0x1C8);
    for (int i = 0; i < 5; i++) {
        if (entry->mField4 == -1) {
            goto found;
        }
        entry++;
    }
    entry = nullptr;
    
found:
    if (!entry) return -1;
    
    // Clear usage flag
    entry->mStreamFlags = 0;
    
    // Check mono mode
    if (isSoundModeMono__9CDeviceSCFv()) {
        entry->mStreamFlags |= 0x40; // Mono flag
    } else {
        entry->mStreamFlags &= ~0x40;
    }
    
    // Copy filename
    strncpy(entry->mFilename, filename, 64);
    
    // Check for .ahx extension or forced AHX mode
    bool useAhx = (strstr(filename, lbl_eu_806637A4) != nullptr) || param6;
    if (useAhx) {
        entry->mStreamFlags |= 0x04; // AHX flag
    } else {
        entry->mStreamFlags &= ~0x04;
    }
    
    // Store playback parameters
    entry->mField4 = param2;
    
    // Calculate buffer size based on parameters
    u32 rate = lbl_eu_806637A0;
    int channels = (entry->mStreamFlags & 0x04) ? 2 : 2; // Based on flags
    u32 bufSize = ((rate / 10000) * 6 * 0x800) + channels * 0x60C0 + 100;
    entry->mBufSize = bufSize;
    
    // Allocate streaming buffer
    void* mem2 = getHandleMEM2__Q23mtl10MemManagerFv();
    entry->mBuffer = allocate_tail__Q23mtl10MemManagerFUlUli(mem2, bufSize, 4);
    
    if (!entry->mBuffer) {
        // Cleanup on allocation failure
        if (entry) {
            if (entry->mAdxt) {
                if (entry->mStreamFlags & 0x04) {
                    ADXT_DetachAhx(entry->mAdxt);
                }
                ADXT_Destroy(entry->mAdxt);
                entry->mAdxt = nullptr;
            }
            if (entry->mBuffer) {
                deallocate__Q23mtl10MemManagerFPv(entry->mBuffer);
                entry->mBuffer = nullptr;
            }
            entry->mField4 = -1;
            entry->mStreamFlags = 0;
        }
        return -1;
    }
    
    // Generate unique stream ID
    int* counterPtr = (int*)((u8*)sInstance + 0x4AC);
    int counter = *counterPtr;
    int streamId = ((counter & 0xFF) << 8) | (entry->mId & 0xFF);
    *counterPtr = counter + 1;
    entry->mField4 = streamId;
    
    // Initialize playback state
    entry->mField5C = 0;
    entry->mField60 = 0;
    entry->mVolume = 0.0f;
    entry->mTargetVol = 0.0f;
    entry->mFadeStart = 0.0f;
    entry->mFadeTarget = 1.0f;
    entry->mFadeTimer = 1.0f;
    entry->mField80 = 0;
    entry->mField84 = 0;
    entry->mField78 = 0.0f;
    
    // Set loop flag
    if (param3) {
        entry->mStreamFlags |= 0x20; // Loop flag
    } else {
        entry->mStreamFlags &= ~0x20;
    }
    
    // Store timing parameters
    entry->mField88 = param4;
    entry->mField8C = param5;
    
    // Check if we should start immediately
    if (!(entry->mStreamFlags & 0x08)) {
        int channels = (entry->mStreamFlags & 0x04) ? 2 : 2;
        entry->mAdxt = ADXT_Create(channels, entry->mBuffer, entry->mBufSize);
        
        // Attach AHX if needed
        if (entry->mStreamFlags & 0x04) {
            void* ahxData = (u8*)sAudioBuffer + (entry->mId << 13);
            ADXT_AttachAhx(entry->mAdxt, ahxData, 0x2000);
            ADXT_SetSvrFreq(entry->mAdxt, 30000);
        }
        
        // Start playback
        if (param4 >= 0) {
            ADXT_StartAfs(entry->mAdxt, param4, param5);
        } else {
            ADXT_StartFnameRange(entry->mAdxt, filename);
        }
        
        // Set output pan for mono
        if ((entry->mStreamFlags & 0x20) && !(entry->mStreamFlags & 0x04)) {
            ADXT_SetOutPan(entry->mAdxt, 1, 0);
            ADXT_SetOutPan(entry->mAdxt, 0, 0);
        }
        
        // Wait for playback to start if blocking
        if (param3) {
            int waitCount = 100;
            while (waitCount-- > 0) {
                ADXM_ExecMain();
                VIWaitForRetrace();
                if (ADXT_GetStat(entry->mAdxt) == 3) break;
            }
        }
    }
    
    return entry->mField4;
}

// func_8045B970 - Check if stream is playing (us-8045f9ac)
bool CLibCriStreamingPlay::func_8045B970(int id) {
    CStreamEntry* entry = (CStreamEntry*)((u8*)sInstance + 0x1C8);
    
    // Check first 5 stream entries
    for (int i = 0; i < 5; i++) {
        if (entry->mField4 == id && entry->mField4 != -1) return true;
        if (entry->mField4 != -1 && id == -1) return true;
        entry++;
    }
    return false;
}

// func_8045BAB0 - Stop a specific stream (us-8045faec)
void CLibCriStreamingPlay::func_8045BAB0(int id) {
    CStreamEntry* entry = FindStreamEntry(id);
    if (!entry) return;
    
    // Destroy ADXT handle
    if (entry->mAdxt) {
        if (entry->mStreamFlags & 0x04) {
            ADXT_DetachAhx(entry->mAdxt);
        }
        ADXT_Destroy(entry->mAdxt);
        entry->mAdxt = nullptr;
    }
    
    // Free buffer
    if (entry->mBuffer) {
        deallocate__Q23mtl10MemManagerFPv(entry->mBuffer);
        entry->mBuffer = nullptr;
    }
    
    // Reset entry
    entry->mField4 = -1;
    entry->mStreamFlags = 0;
}

// func_8045BBA0 - Stop all streams (us-8045fbdc)
void CLibCriStreamingPlay::func_8045BBA0() {
    CStreamEntry* entry = (CStreamEntry*)((u8*)sInstance + 0x1C8);
    
    for (int i = 0; i < 5; i++) {
        if (entry) {
            if (entry->mAdxt) {
                if (entry->mStreamFlags & 0x04) {
                    ADXT_DetachAhx(entry->mAdxt);
                }
                ADXT_Destroy(entry->mAdxt);
                entry->mAdxt = nullptr;
            }
            if (entry->mBuffer) {
                deallocate__Q23mtl10MemManagerFPv(entry->mBuffer);
                entry->mBuffer = nullptr;
            }
            entry->mField4 = -1;
            entry->mStreamFlags = 0;
        }
        entry++;
    }
}

// func_8045BC4C - Update stream volume with fade (us-8045fc88)
void CLibCriStreamingPlay::func_8045BC4C(int id, bool pause) {
    CStreamEntry* entry = FindStreamEntry(id);
    if (!entry) return;
    
    // Update pause state
    if (pause) {
        entry->mStreamFlags |= 0x01;
    } else {
        entry->mStreamFlags &= ~0x01;
    }
    
    // Calculate pause state including global pause
    u32 flags = entry->mStreamFlags;
    bool isPaused = (flags & 1) || (*(int*)((u8*)sInstance + 0x4B0) > 0);
    ADXT_Pause(entry->mAdxt, isPaused ? 1 : 0);
    
    // Calculate volume: volume * field78 * field7C
    float vol = entry->mVolume * entry->mField78 * entry->mField7C;
    
    // Override if in flow
    if (hasFlow__12CWorkControlFv(this)) {
        vol = 1.0f;
    }
    
    // Set output volume
    int dbVol = LookupVolume((int)(960.0f * vol));
    ADXT_SetOutVol(entry->mAdxt, dbVol);
}

// func_8045BE48 - Get playback position (us-8045fe84)
int CLibCriStreamingPlay::func_8045BE48(int id) {
    CStreamEntry* entry = FindStreamEntry(id);
    if (entry) {
        return entry->mField84;
    }
    return 0;
}

// wkUpdate - Main update loop (us-8045ff08)
void CLibCriStreamingPlay::wkUpdate() {
    CStreamEntry* entry = (CStreamEntry*)((u8*)this + 0x1C8);
    
    for (int i = 0; i < 5; i++) {
        if (entry->mAdxt) {
            // Process fade
            if (entry->mFadeTarget != 0.0f && 
                *(int*)((u8*)this + 0x4B0) == 0 &&
                !(entry->mStreamFlags & 0x01)) {
                
                entry->mFadeTimer += 0.01f; // delta time
                
                if (entry->mFadeTimer >= entry->mFadeTarget) {
                    // Fade complete
                    entry->mVolume = entry->mTargetVol;
                    entry->mFadeTarget = 0.0f;
                    
                    if (entry->mTargetVol == 0.0f) {
                        // Volume reached zero
                        if (entry->mFadeAction == 1) {
                            // Pause
                            entry->mStreamFlags |= 0x02;
                            u32 flags = entry->mStreamFlags;
                            bool isPaused = (flags & 1) || (*(int*)((u8*)this + 0x4B0) > 0);
                            ADXT_Pause(entry->mAdxt, isPaused ? 1 : 0);
                        } else if (entry->mFadeAction == 2) {
                            // Stop playback
                            if (entry) {
                                if (entry->mAdxt) {
                                    if (entry->mStreamFlags & 0x04) {
                                        ADXT_DetachAhx(entry->mAdxt);
                                    }
                                    ADXT_Destroy(entry->mAdxt);
                                    entry->mAdxt = nullptr;
                                }
                                if (entry->mBuffer) {
                                    deallocate__Q23mtl10MemManagerFPv(entry->mBuffer);
                                    entry->mBuffer = nullptr;
                                }
                                entry->mField4 = -1;
                                entry->mStreamFlags = 0;
                            }
                            goto nextEntry;
                        }
                    }
                } else {
                    // Interpolate volume
                    float t = entry->mFadeTimer / entry->mFadeTarget;
                    entry->mVolume = entry->mFadeStart + (entry->mTargetVol - entry->mFadeStart) * t;
                    
                    // Handle fade-pause state
                    if (entry->mFadeAction == 1 && (entry->mStreamFlags & 0x02)) {
                        if (entry->mVolume != 0.0f) {
                            // Unpause during fade
                            entry->mStreamFlags &= ~0x02;
                            u32 flags = entry->mStreamFlags;
                            bool isPaused = (flags & 1) || (*(int*)((u8*)this + 0x4B0) > 0);
                            ADXT_Pause(entry->mAdxt, isPaused ? 1 : 0);
                        }
                    }
                }
            }
            
            // Update volume
            float vol = entry->mVolume * entry->mField78 * entry->mField7C;
            if (hasFlow__12CWorkControlFv(this)) {
                vol = 1.0f;
            }
            int dbVol = LookupVolume((int)(960.0f * vol));
            ADXT_SetOutVol(entry->mAdxt, dbVol);
            
            // Check playback status
            int stat = ADXT_GetStat(entry->mAdxt);
            if (stat == 5 || stat == 6) {
                // Playback ended
                if (entry) {
                    if (entry->mAdxt) {
                        if (entry->mStreamFlags & 0x04) {
                            ADXT_DetachAhx(entry->mAdxt);
                        }
                        ADXT_Destroy(entry->mAdxt);
                        entry->mAdxt = nullptr;
                    }
                    if (entry->mBuffer) {
                        deallocate__Q23mtl10MemManagerFPv(entry->mBuffer);
                        entry->mBuffer = nullptr;
                    }
                    entry->mField4 = -1;
                    entry->mStreamFlags = 0;
                }
            } else if (stat != 0) {
                // Build status string
                char statusBuf[64];
                u32 statusLen = 0;
                const char* baseStr = lbl_eu_805230B8;
                
                switch (stat) {
                    case 0: strcpy(statusBuf, baseStr); statusLen = strlen(baseStr); break;
                    case 1: { const char* s = baseStr + 7; strcpy(statusBuf, s); statusLen = strlen(s); break; }
                    case 2: { const char* s = baseStr + 30; strcpy(statusBuf, s); statusLen = strlen(s); break; }
                    case 3: { const char* s = baseStr + 41; strcpy(statusBuf, s); statusLen = strlen(s); break; }
                    case 4: { const char* s = baseStr + 58; strcpy(statusBuf, s); statusLen = strlen(s); break; }
                    case 5: { const char* s = baseStr + 71; strcpy(statusBuf, s); statusLen = strlen(s); break; }
                    case 6: { const char* s = baseStr + 80; strcpy(statusBuf, s); statusLen = strlen(s); break; }
                }
                
                // Update playback position
                int time = ADXT_GetTimeReal(entry->mAdxt);
                float timeF = (float)time / 1000.0f / 0.01f;
                entry->mField84 = (u32)timeF;
            }
        }
        
    nextEntry:
        entry++;
    }
}

// wkStandbyLogin - Just calls base (us-80460428)
void CLibCriStreamingPlay::wkStandbyLogin() {
    wkStandbyLogin__11CWorkThreadFv(this);
}

// wkStandbyLogout - Cleanup on logout (us-8046042c)
void CLibCriStreamingPlay::wkStandbyLogout() {
    // Check if we're the active worker
    void* workerList = *(void**)((u8*)this + 0x60);
    if (*(void**)workerList == workerList) {
        // We're active, clean up all streams
        CStreamEntry* entry = (CStreamEntry*)((u8*)sInstance + 0x1C8);
        for (int i = 0; i < 5; i++) {
            if (entry) {
                if (entry->mAdxt) {
                    if (entry->mStreamFlags & 0x04) {
                        ADXT_DetachAhx(entry->mAdxt);
                    }
                    ADXT_Destroy(entry->mAdxt);
                    entry->mAdxt = nullptr;
                }
                if (entry->mBuffer) {
                    deallocate__Q23mtl10MemManagerFPv(entry->mBuffer);
                    entry->mBuffer = nullptr;
                }
                entry->mField4 = -1;
                entry->mStreamFlags = 0;
            }
            entry++;
        }
        wkStandbyLogout__11CWorkThreadFv(this);
    }
}

// OnPauseTrigger - Handle pause/unpause (us-804604e4)
void CLibCriStreamingPlay::OnPauseTrigger(bool paused) {
    if (paused) {
        (*(int*)((u8*)this + 0x4B0))++;
    } else {
        int count = *(int*)((u8*)this + 0x4B0) - 1;
        if (count < 0) count = 0;
        *(int*)((u8*)this + 0x4B0) = count;
    }
    
    // Update all active streams
    CStreamEntry* entry = (CStreamEntry*)((u8*)this + 0x1C8);
    for (int i = 0; i < 5; i++) {
        if (entry->mField4 != -1) {
            // Update pause state
            u32 flags = entry->mStreamFlags;
            bool isPaused = (flags & 1) || (*(int*)((u8*)this + 0x4B0) > 0);
            ADXT_Pause(entry->mAdxt, isPaused ? 1 : 0);
            
            // Update volume
            float vol = entry->mVolume * entry->mField78 * entry->mField7C;
            if (hasFlow__12CWorkControlFv(this)) {
                vol = 1.0f;
            }
            int dbVol = LookupVolume((int)(960.0f * vol));
            ADXT_SetOutVol(entry->mAdxt, dbVol);
        }
        entry++;
    }
}

// func_8045C67C - Get current volume (us-804606c0)
float CLibCriStreamingPlay::func_8045C67C(int id) {
    CStreamEntry* entry = FindStreamEntry(id);
    if (!entry) return 1.0f;
    return entry->mVolume;
}

// func_8045C700 - Set volume immediately (us-80460744)
void CLibCriStreamingPlay::func_8045C700(int id, float volume) {
    CStreamEntry* entry = FindStreamEntry(id);
    if (!entry) return;
    
    entry->mVolume = volume;
    entry->mTargetVol = volume;
    entry->mFadeStart = (float)volume;
    entry->mFadeTarget = 1.0f;
    entry->mFadeTimer = 1.0f;
    
    // Calculate and set volume
    float vol = (float)volume * entry->mField78 * entry->mField7C;
    if (hasFlow__12CWorkControlFv(this)) {
        vol = 1.0f;
    }
    int dbVol = LookupVolume((int)(960.0f * vol));
    ADXT_SetOutVol(entry->mAdxt, dbVol);
}

// func_8045C8B0 - Set volume with fade (us-804608f4)
void CLibCriStreamingPlay::func_8045C8B0(int id, float volume) {
    CStreamEntry* entry = FindStreamEntry(id);
    if (!entry) return;
    
    entry->mField7C = (float)volume;
    
    // Calculate and set volume
    float vol = entry->mVolume * entry->mField78 * (float)volume;
    if (hasFlow__12CWorkControlFv(this)) {
        vol = 1.0f;
    }
    int dbVol = LookupVolume((int)(960.0f * vol));
    ADXT_SetOutVol(entry->mAdxt, dbVol);
}

// func_8045CA4C - Start volume fade (us-80460a90)
void CLibCriStreamingPlay::func_8045CA4C(int id, float volume, float fadeTime, int action) {
    // If fadeTime <= 0, set volume immediately
    if (fadeTime <= 0.0f) {
        CStreamEntry* entry = FindStreamEntry(id);
        if (!entry) return;
        
        entry->mVolume = volume;
        entry->mTargetVol = volume;
        entry->mFadeStart = (float)volume;
        entry->mFadeTarget = 1.0f;
        entry->mFadeTimer = 1.0f;
        
        // Calculate and set volume
        float vol = (float)volume * entry->mField78 * entry->mField7C;
        if (hasFlow__12CWorkControlFv(this)) {
            vol = 1.0f;
        }
        int dbVol = LookupVolume((int)(960.0f * vol));
        ADXT_SetOutVol(entry->mAdxt, dbVol);
    }
    
    // Find entry and set up fade
    CStreamEntry* entry2 = FindStreamEntry(id);
    if (!entry2) return;
    
    entry2->mTargetVol = volume;
    entry2->mFadeStart = entry2->mVolume;
    if (volume == 0.0f) {
        entry2->mFadeAction = action;
    }
    entry2->mFadeTarget = fadeTime;
    entry2->mFadeTimer = 0.0f;
    
    // Clamp fade target
    if (entry2->mFadeTarget <= 0.0f) {
        entry2->mFadeTarget = 0.0f;
    }
}

// func_8045CCFC - Set pan/pitch (us-80460d40)
void CLibCriStreamingPlay::func_8045CCFC(int id, float param2, float param3, float param4) {
    CStreamEntry* entry = FindStreamEntry(id);
    if (!entry) return;
    
    float out1, out2;
    func_8049B834(param2, param3, param4, &out1, &out2);
    
    // Clamp minimum
    if (out1 < (float)param4) {
        out1 = (float)param4;
    }
    
    // Store and set volume
    entry->mField78 = (float)out1;
    float vol = entry->mVolume * (float)out1 * entry->mField7C;
    
    if (hasFlow__12CWorkControlFv(this)) {
        vol = 1.0f;
    }
    int dbVol = LookupVolume((int)(960.0f * vol));
    ADXT_SetOutVol(entry->mAdxt, dbVol);
    
    // Set pan if not stereo
    if (!(entry->mStreamFlags & 0x20)) {
        int pan = (int)(30.0f * out2);
        if (pan < -15) pan = -15;
        if (pan > 15) pan = 15;
        
        if (!(entry->mStreamFlags & 0x04)) {
            ADXT_SetOutPan(entry->mAdxt, 1, pan);
        }
        ADXT_SetOutPan(entry->mAdxt, 0, pan);
    }
}

// func_8045CF30 - Stop all streams (us-80460f74)
// Same as func_8045BBA0
void CLibCriStreamingPlay::func_8045CF30() {
    CStreamEntry* entry = (CStreamEntry*)((u8*)sInstance + 0x1C8);
    
    for (int i = 0; i < 5; i++) {
        if (entry) {
            if (entry->mAdxt) {
                if (entry->mStreamFlags & 0x04) {
                    ADXT_DetachAhx(entry->mAdxt);
                }
                ADXT_Destroy(entry->mAdxt);
                entry->mAdxt = nullptr;
            }
            if (entry->mBuffer) {
                deallocate__Q23mtl10MemManagerFPv(entry->mBuffer);
                entry->mBuffer = nullptr;
            }
            entry->mField4 = -1;
            entry->mStreamFlags = 0;
        }
        entry++;
    }
}

// func_8045CFDC - Calculate buffer size (us-80461020)
int CLibCriStreamingPlay::func_8045CFDC(int param) {
    u32 rate = lbl_eu_806637A0;
    int neg = -param;
    int flags = neg | param;
    int sign1 = flags >> 31;
    int sign2 = flags >> 31;
    int ch1 = sign1 + 2;
    u32 r3 = rate / 10000;
    int ch2 = sign2 + 2;
    u32 bufSize = (r3 * ch1 * 25000) / 1000;
    int size = (int)(bufSize * 6) + ch2 * 24768;
    size = (size + 0x800) & ~0x7FF;
    return size + 100;
}

// func_8045D03C - Check if stream is active (us-80461080)
bool CLibCriStreamingPlay::func_8045D03C(int id) {
    if (!sInstance) return false;
    
    CStreamEntry* entry = FindStreamEntry(id);
    if (!entry) return false;
    if (!entry->mAdxt) return false;
    
    // Check pause state
    u32 flags = entry->mStreamFlags;
    bool isPaused = (flags & 1) || (*(int*)((u8*)sInstance + 0x4B0) > 0);
    bool isStopped = (flags & 0x02) != 0;
    
    return isPaused || isStopped;
}

// func_8045D140 - CDeviceVICb thunk (us-80461184) - FULL_MATCH
void CLibCriStreamingPlay::func_8045D140() {
    CLibCriStreamingPlay* base = (CLibCriStreamingPlay*)((u8*)this - 0x1C4);
    base->func_8045CF30();
}

// func_8045D148 - CDeviceVICb thunk (us-8046118c) - FULL_MATCH
void CLibCriStreamingPlay::func_8045D148() {
    CLibCriStreamingPlay* base = (CLibCriStreamingPlay*)((u8*)this - 0x1C4);
    base->~CLibCriStreamingPlay();
}
