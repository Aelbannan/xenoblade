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

    void func_802A0B8C(void* owner);
    void func_802A0E08();
    void func_802A0FE8();
    bool func_802A109C(float volume, int voiceId, int priority);
    void func_802A1304();
};

// 4-byte zero blob at .rodata:0x805106D4 (empty default voice name).
// Declared as a struct so a struct-pointer local makes MWCC materialize
// the base address once (lis/addi) instead of rematerializing per use.
// The pad keeps the declared size > 8 bytes so MWCC does not treat it
// as an sbss-eligible small extern (which would force sda21 addressing).
struct CVoiceName {
    char str[4];
    char pad[12];
};

// Raw 3-float position block copied as raw bytes by func_802A0E08's
// fallback branch (retail copies it with lwz/stw, i.e. a struct copy).
// The union's u32 member keeps the copy integer-typed.
union CVoicePos {
    float f[3];  // 0x00-0x0B
    u32   u[3];
};

// Model transform returned by the owner's getModelPos virtual (slot 72):
// a 3x4 matrix whose translation sits at m[0][3], m[1][3], m[2][3]
// (offsets 0x0C, 0x1C, 0x2C).
struct CVoiceModelPos {
    u8    _00[0x0C];
    float x;  // 0x0C
    u8    _10[0x0C];
    float y;  // 0x1C
    u8    _20[0x0C];
    float z;  // 0x2C
};

// Owner-object interface used by func_802A0E08.  The owner vtable places
// getPosition at raw slot 41 (offset 0xac) and getModelPos at raw slot 72
// (offset 0x128); MWCC adds 2 implicit leading vtable slots, so declared
// slot P lands at offset (P+2)*4.  The padding virtuals keep offsets exact.
struct CVoiceOwnerIntf {
    virtual void v00();  virtual void v01();  virtual void v02();  virtual void v03();
    virtual void v04();  virtual void v05();  virtual void v06();  virtual void v07();
    virtual void v08();  virtual void v09();  virtual void v10();  virtual void v11();
    virtual void v12();  virtual void v13();  virtual void v14();  virtual void v15();
    virtual void v16();  virtual void v17();  virtual void v18();  virtual void v19();
    virtual void v20();  virtual void v21();  virtual void v22();  virtual void v23();
    virtual void v24();  virtual void v25();  virtual void v26();  virtual void v27();
    virtual void v28();  virtual void v29();  virtual void v30();  virtual void v31();
    virtual void v32();  virtual void v33();  virtual void v34();  virtual void v35();
    virtual void v36();  virtual void v37();  virtual void v38();  virtual void v39();
    virtual void v40();  // 0x00-0xA4
    virtual CVoicePos* getPosition();                            // slot 41 = 0xac
    virtual void v42();  virtual void v43();  virtual void v44();  virtual void v45();
    virtual void v46();  virtual void v47();  virtual void v48();  virtual void v49();
    virtual void v50();  virtual void v51();  virtual void v52();  virtual void v53();
    virtual void v54();  virtual void v55();  virtual void v56();  virtual void v57();
    virtual void v58();  virtual void v59();  virtual void v60();  virtual void v61();
    virtual void v62();  virtual void v63();  virtual void v64();  virtual void v65();
    virtual void v66();  virtual void v67();  virtual void v68();  virtual void v69();
    virtual void v70();  virtual void v71();  // 0xB0-0x120
    virtual CVoiceModelPos* getModelPos();                         // slot 72 = 0x128
};
