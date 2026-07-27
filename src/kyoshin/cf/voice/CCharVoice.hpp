#pragma once

#include <types.h>

/**
 * CCharVoice -- single character voice playback instance.
 *
 * Each instance manages a voice file path and interacts with the sound
 * system to play/stop/update character voices.  The owner object decides
 * which voice profile (normal vs battle) is used.
 *
 * N.B.  The class is NOT declared with virtual functions even though it
 * has a vtable pointer at offset 0x3C.  The vtable (lbl_eu_805398B0) is
 * set up as assembly data and assigned manually in the constructor so
 * that the C-linkage symbol name __ct__CCharVoice is used (no C++
 * namespace mangling).
 *
 * Field layout (total size 0x40 = 64 bytes):
 *   0x00  mOwner            parent/owner object
 *   0x04  mVoiceId          current voice ID
 *   0x08  mPriorityCheck    priority value for play-through gate
 *   0x0C  mSoundHandle      handle from archive-voice sound system
 *   0x10  mFileName[0x20]   voice file path buffer (32 bytes)
 *   0x30  mFileNameLen      strlen of mFileName
 *   0x34  mField34          offset into mFileName for digit formatting
 *   0x38  mBattleSndHandle  sound handle for battle-voice path
 *   0x3C  mVtable           pointer to lbl_eu_805398B0 (vtable)
 */
struct CCharVoice {
    void* mOwner;            // 0x00
    s32   mVoiceId;          // 0x04
    s32   mPriorityCheck;    // 0x08
    s32   mSoundHandle;      // 0x0C
    char  mFileName[0x20];  // 0x10
    u32   mFileNameLen;      // 0x30
    s32   mField34;          // 0x34
    u16   mBattleSndHandle;  // 0x38
    // 2 bytes padding to 0x3C
    void* mVtable;           // 0x3C -- vtable pointer
};
