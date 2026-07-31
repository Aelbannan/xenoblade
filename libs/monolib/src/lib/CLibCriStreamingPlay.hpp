#pragma once

#include <types.h>

// Forward declarations for CRI ADX types
struct ADXTObj;
typedef ADXTObj* ADXT;

// Streaming sound entry (0x94 bytes each, 5 slots)
struct CStreamEntry {
    u32 mId;           // 0x00 - stream identifier (sequence+slot packed)
    u32 mField4;       // 0x04 - packed id or -1
    ADXT mAdxt;        // 0x08 - ADXT handle
    u32 mFlags;        // 0x0C - stream flags
    char mFilename[64]; // 0x1C - filename
    u32 mBufSize;      // 0x14 - buffer size
    void* mBuffer;     // 0x18 - allocated buffer
    u32 mField5C;      // 0x5C
    u32 mField60;      // 0x60
    float mVolume;     // 0x64 - current volume
    float mTargetVol;  // 0x68 - target volume
    float mFadeStart;  // 0x6C - fade start volume
    float mFadeTarget; // 0x70 - fade target
    float mFadeTimer;  // 0x74 - fade timer
    float mField78;    // 0x78
    float mField7C;    // 0x7C
    u32 mFadeAction;   // 0x80 - fade end action (1=pause, 2=stop)
    u32 mField84;      // 0x84 - playback position
    u32 mField88;      // 0x88
    u32 mField8C;      // 0x8C
    u32 mStreamFlags;  // 0x90 - bitfield flags
};
static_assert(sizeof(CStreamEntry) == 0x94, "CStreamEntry must be 0x94 bytes");

class CLibCriStreamingPlay {
public:
    CLibCriStreamingPlay();
    virtual ~CLibCriStreamingPlay();
    
    // Playback control
    int func_8045B5AC(const char* filename, int param2, bool param3, int param4, int param5, bool param6);
    bool func_8045B970(int id);
    void func_8045BAB0(int id);
    void func_8045BBA0();  // stop all streams
    void func_8045BC4C(int id, bool pause);
    int func_8045BE48(int id);
    
    // WorkThread overrides
    void wkUpdate();
    void wkStandbyLogin();
    void wkStandbyLogout();
    void OnPauseTrigger(bool paused);
    
    // Volume/fade control
    float func_8045C67C(int id);
    void func_8045C700(int id, float volume);
    void func_8045C8B0(int id, float volume);
    void func_8045CA4C(int id, float volume, float fadeTime, int action);
    void func_8045CCFC(int id, float param2, float param3, float param4);
    
    // Cleanup/state
    void func_8045CF30();  // stop all (same as BBA0?)
    int func_8045CFDC(int param);
    bool func_8045D03C(int id);
    
    // Virtual thunks for CDeviceVICb
    void func_8045D140();
    void func_8045D148();

private:
    // TODO: add fields
};
